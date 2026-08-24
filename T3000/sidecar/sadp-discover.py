#!/usr/bin/env python3
"""SADP-style LAN probe. Inquiry only. Does not launch SADPTool.exe."""
from __future__ import annotations

import json
import os
import re
import socket
import sys
import time
import uuid
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import urlparse, parse_qs

MCAST = "239.255.255.250"
PORT = 37020


def _tag(xml: str, name: str) -> str:
    m = re.search(r"<%s>([^<]*)</%s>" % (name, name), xml, re.I)
    return m.group(1).strip() if m else ""


def _probe_xml() -> bytes:
    uid = str(uuid.uuid4()).upper()
    return (
        '<?xml version="1.0" encoding="utf-8"?>'
        "<Probe><Uuid>%s</Uuid><Types>inquiry</Types></Probe>" % uid
    ).encode("utf-8")


def _local_ipv4s() -> list[str]:
    ips = []
    try:
        hostname = socket.gethostname()
        for info in socket.getaddrinfo(hostname, None, socket.AF_INET):
            ip = info[4][0]
            if ip and not ip.startswith("127.") and ip not in ips:
                ips.append(ip)
    except OSError:
        pass
    # also the route used for multicast
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(("8.8.8.8", 80))
        ip = s.getsockname()[0]
        s.close()
        if ip and not ip.startswith("127.") and ip not in ips:
            ips.append(ip)
    except OSError:
        pass
    return ips or ["0.0.0.0"]


def sadp_scan(timeout: float = 3.0) -> list[dict]:
    """UDP multicast + broadcast inquiry, same as SADP discovery."""
    probe = _probe_xml()
    found: dict[str, dict] = {}
    socks: list[socket.socket] = []

    def add_sock(bind_ip: str) -> None:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        try:
            s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
        except (OSError, AttributeError):
            pass
        s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
        s.settimeout(0.25)
        try:
            s.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 4)
        except OSError:
            pass
        if bind_ip and bind_ip != "0.0.0.0":
            try:
                s.setsockopt(
                    socket.IPPROTO_IP, socket.IP_MULTICAST_IF, socket.inet_aton(bind_ip)
                )
            except OSError:
                pass
            s.bind((bind_ip, 0))
        else:
            s.bind(("", 0))
        socks.append(s)

    for ip in _local_ipv4s():
        try:
            add_sock(ip)
        except OSError:
            continue
    if not socks:
        add_sock("0.0.0.0")

    dests = [(MCAST, PORT), ("255.255.255.255", PORT)]
    for s in socks:
        for dest in dests:
            try:
                s.sendto(probe, dest)
            except OSError:
                continue

    end = time.time() + max(0.5, float(timeout))
    while time.time() < end:
        for s in socks:
            try:
                data, addr = s.recvfrom(8192)
            except socket.timeout:
                continue
            except OSError:
                continue
            xml = data.decode("utf-8", "replace")
            if "<" not in xml:
                continue
            ip = _tag(xml, "IPv4Address") or addr[0]
            if not ip or ip.startswith("127."):
                continue
            rec = {
                "src": "sadp",
                "name": _tag(xml, "DeviceDescription")
                or _tag(xml, "DeviceType")
                or ("hik-" + ip.split(".")[-1]),
                "hardware": _tag(xml, "DeviceType") or _tag(xml, "DeviceDescription"),
                "url": "http://%s:%s" % (ip, _tag(xml, "HttpPort") or "80"),
                "ip": ip,
                "mac": _tag(xml, "MAC"),
                "serial": _tag(xml, "DeviceSN") or _tag(xml, "SerialNumber"),
                "http_port": int(_tag(xml, "HttpPort") or "80" or 80),
                "cmd_port": int(_tag(xml, "CommandPort") or "8000" or 8000),
                "dhcp": _tag(xml, "DHCP"),
                "software": _tag(xml, "SoftwareVersion") or _tag(xml, "DSPVersion"),
                "rtsp": "rtsp://%s:554/Streaming/Channels/101" % ip,
                "rtsp_sub": "rtsp://%s:554/Streaming/Channels/102" % ip,
            }
            found[ip] = rec
    for s in socks:
        try:
            s.close()
        except OSError:
            pass
    return list(found.values())


class Handler(BaseHTTPRequestHandler):
    def log_message(self, fmt, *args):
        return

    def _send(self, code: int, body) -> None:
        raw = json.dumps(body, indent=2).encode("utf-8")
        self.send_response(code)
        self.send_header("Content-Type", "application/json")
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Content-Length", str(len(raw)))
        self.end_headers()
        self.wfile.write(raw)

    def do_OPTIONS(self):
        self.send_response(204)
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, OPTIONS")
        self.end_headers()

    def do_GET(self):
        u = urlparse(self.path)
        if u.path in ("/", "/discover", "/sadp", "/api/sadp"):
            qs = parse_qs(u.query)
            timeout = float((qs.get("timeout") or ["3"])[0] or 3)
            devices = sadp_scan(timeout)
            self._send(200, {"src": "sadp", "count": len(devices), "devices": devices})
            return
        self._send(404, {"error": "not found"})


def main(argv: list[str]) -> int:
    if argv and argv[0] in ("--serve", "serve"):
        host = "127.0.0.1"
        port = 9295
        httpd = ThreadingHTTPServer((host, port), Handler)
        print("sadp discover http://%s:%s/discover" % (host, port), flush=True)
        httpd.serve_forever()
        return 0
    timeout = 3.0
    if argv:
        try:
            timeout = float(argv[0])
        except ValueError:
            pass
    devices = sadp_scan(timeout)
    print(json.dumps({"src": "sadp", "count": len(devices), "devices": devices}, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
