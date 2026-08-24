"""T3000 DVR settings API on 127.0.0.1:9294."""
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
import json
import os
import sqlite3
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
SETTINGS = os.path.join(HERE, "dvr-settings.json")
NVR = os.path.normpath(os.path.join(HERE, "..", "ResourceFile", "nvr", "index.html"))
DB = os.path.normpath(os.path.join(HERE, "..", "..", "Database", "T3000.db"))
SYNC = os.path.join(HERE, "dvr-sync-db.py")


def read_settings():
    with open(SETTINGS, encoding="utf-8") as f:
        return f.read()


def write_settings(body: str):
    obj = json.loads(body)
    if not obj.get("storage_path"):
        obj["storage_path"] = r"C:\Xdrive\T3000_DVR"
    text = json.dumps(obj, indent=2)
    with open(SETTINGS, "w", encoding="utf-8") as f:
        f.write(text)
    try:
        subprocess.run([sys.executable, SYNC], check=False, cwd=HERE)
    except Exception as exc:
        print("db sync failed", exc)
    return text


class Handler(BaseHTTPRequestHandler):
    def log_message(self, fmt, *args):
        sys.stderr.write("dvr-api " + (fmt % args) + "\n")

    def _cors(self):
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")
        self.send_header("Cache-Control", "no-store")

    def _send(self, code, ctype, data: bytes):
        self.send_response(code)
        self.send_header("Content-Type", ctype)
        self._cors()
        self.send_header("Content-Length", str(len(data)))
        self.end_headers()
        if data:
            self.wfile.write(data)

    def do_OPTIONS(self):
        self._send(204, "text/plain", b"")

    def do_GET(self):
        path = (self.path.split("?", 1)[0] or "/").rstrip("/") or "/"
        if path == "/settings":
            self._send(200, "application/json; charset=utf-8", read_settings().encode("utf-8"))
            return
        if path in ("/", "/nvr", "/ui", "/index.html"):
            with open(NVR, "rb") as f:
                self._send(200, "text/html; charset=utf-8", f.read())
            return
        self._send(404, "text/plain; charset=utf-8", b"not found")

    def do_POST(self):
        path = (self.path.split("?", 1)[0] or "/").rstrip("/") or "/"
        if path != "/settings":
            self._send(404, "text/plain; charset=utf-8", b"not found")
            return
        n = int(self.headers.get("Content-Length") or 0)
        body = self.rfile.read(n).decode("utf-8") if n else "{}"
        try:
            text = write_settings(body)
            self._send(200, "application/json; charset=utf-8", text.encode("utf-8"))
        except Exception as exc:
            self._send(400, "text/plain; charset=utf-8", str(exc).encode("utf-8"))


if __name__ == "__main__":
    httpd = ThreadingHTTPServer(("127.0.0.1", 9294), Handler)
    print("dvr-api listening http://127.0.0.1:9294/settings", flush=True)
    httpd.serve_forever()
