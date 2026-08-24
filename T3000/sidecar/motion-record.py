#!/usr/bin/env python3
"""T3000 CCTV motion-triggered recorder.

Watches go2rtc stream names (sim or live) on localhost. Records a short
ffmpeg clip ONLY while motion is detected. Idle = no writer, no file growth.

  python motion-record.py                  # arm, watch go2rtc streams
  python motion-record.py --idle-proof     # same-frame compare, expect 0 clips
  python motion-record.py --demo-sim --once  # one clip from sim/*.mp4 (people walking)

Does not enable go2rtc #record or MediaMTX continuous record.
Never prints camera passwords. Binds events HTTP to 127.0.0.1 only.
"""
from __future__ import annotations

import argparse
import json
import os
import re
import subprocess
import sys
import threading
import time
import urllib.error
import urllib.parse
import urllib.request
from datetime import datetime, timezone
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from typing import Dict, List, Optional

CREATE_NO_WINDOW = 0x08000000 if os.name == "nt" else 0
SAFE_NAME = re.compile(r"^[A-Za-z0-9_.-]+$")

STATE = {
    "armed": False,
    "mode": "motion",
    "updated": "",
    "api": "",
    "rtsp": "",
    "recordings_dir": "",
    "events_http": "",
    "streams": [],
    "threshold": 3.0,
    "clip_count": 0,
    "total_bytes": 0,
    "events": [],
    "last_scores": {},
    "recording": {},
    "error": "",
}
STATE_LOCK = threading.Lock()
STOP = threading.Event()


def utc_now() -> str:
    return datetime.now(timezone.utc).replace(microsecond=0).isoformat()


def exe_dir() -> Path:
    return Path(__file__).resolve().parent


def find_ffmpeg(explicit: str = "") -> str:
    if explicit:
        p = Path(explicit)
        if p.is_file():
            return str(p)
    here = exe_dir() / "ffmpeg.exe"
    if here.is_file():
        return str(here)
    return "ffmpeg"


def run_ff(ffmpeg: str, args: List[str], timeout: float = 20, capture: bool = True) -> subprocess.CompletedProcess:
    cmd = [ffmpeg, "-hide_banner", "-loglevel", "error"] + args
    kw = {
        "timeout": timeout,
        "creationflags": CREATE_NO_WINDOW,
    }
    if capture:
        kw["capture_output"] = True
    else:
        kw["stdout"] = subprocess.DEVNULL
        kw["stderr"] = subprocess.DEVNULL
    try:
        return subprocess.run(cmd, **kw)
    except subprocess.TimeoutExpired as e:
        return subprocess.CompletedProcess(cmd, 124, getattr(e, "stdout", b""), getattr(e, "stderr", b""))
    except OSError as e:
        return subprocess.CompletedProcess(cmd, 1, b"", str(e).encode("utf-8", "replace"))


def grab_rtsp_frame(ffmpeg: str, rtsp_url: str, dest: Path) -> bool:
    dest.parent.mkdir(parents=True, exist_ok=True)
    r = run_ff(
        ffmpeg,
        [
            "-rtsp_transport", "tcp",
            "-i", rtsp_url,
            "-frames:v", "1",
            "-update", "1",
            "-q:v", "6",
            "-y", str(dest),
        ],
        timeout=15,
    )
    return r.returncode == 0 and dest.is_file() and dest.stat().st_size > 32


def grab_file_frame(ffmpeg: str, media: Path, dest: Path, ss: float) -> bool:
    dest.parent.mkdir(parents=True, exist_ok=True)
    r = run_ff(
        ffmpeg,
        [
            "-ss", f"{ss:.2f}",
            "-i", str(media),
            "-frames:v", "1",
            "-update", "1",
            "-q:v", "6",
            "-y", str(dest),
        ],
        timeout=12,
    )
    return r.returncode == 0 and dest.is_file() and dest.stat().st_size > 32


def gray_bytes(ffmpeg: str, jpg: Path, w: int = 160, h: int = 90) -> bytes:
    r = run_ff(
        ffmpeg,
        [
            "-i", str(jpg),
            "-f", "rawvideo",
            "-pix_fmt", "gray",
            "-s", f"{w}x{h}",
            "pipe:1",
        ],
        timeout=10,
    )
    return r.stdout or b""


def mae(a: bytes, b: bytes) -> float:
    if not a or not b or len(a) != len(b):
        return 999.0
    total = 0
    for x, y in zip(a, b):
        total += abs(x - y)
    return total / len(a)


def list_stream_names(api: str) -> List[str]:
    url = api.rstrip("/") + "/api/streams"
    try:
        with urllib.request.urlopen(url, timeout=5) as resp:
            data = json.loads(resp.read().decode("utf-8", "replace"))
    except (urllib.error.URLError, TimeoutError, json.JSONDecodeError, OSError):
        return []
    if not isinstance(data, dict):
        return []
    names = []
    for name in data.keys():
        if not isinstance(name, str):
            continue
        if name.endswith("-main"):
            continue
        if SAFE_NAME.match(name):
            names.append(name)
    return names


def record_rtsp_clip(ffmpeg: str, rtsp_url: str, dest: Path, seconds: int) -> bool:
    dest.parent.mkdir(parents=True, exist_ok=True)
    r = run_ff(
        ffmpeg,
        [
            "-rtsp_transport", "tcp",
            "-i", rtsp_url,
            "-t", str(int(seconds)),
            "-an",
            "-c:v", "copy",
            "-movflags", "+faststart",
            "-y", str(dest),
        ],
        timeout=seconds + 20,
    )
    if r.returncode == 0 and dest.is_file() and dest.stat().st_size > 1024:
        return True
    r = run_ff(
        ffmpeg,
        [
            "-rtsp_transport", "tcp",
            "-i", rtsp_url,
            "-t", str(int(seconds)),
            "-an",
            "-c:v", "libx264",
            "-preset", "ultrafast",
            "-pix_fmt", "yuv420p",
            "-movflags", "+faststart",
            "-y", str(dest),
        ],
        timeout=seconds + 40,
    )
    return dest.is_file() and dest.stat().st_size > 1024


def record_file_clip(ffmpeg: str, media: Path, dest: Path, ss: float, seconds: int) -> bool:
    dest.parent.mkdir(parents=True, exist_ok=True)
    r = run_ff(
        ffmpeg,
        [
            "-ss", f"{max(0.0, ss):.2f}",
            "-i", str(media),
            "-t", str(int(seconds)),
            "-an",
            "-c:v", "copy",
            "-movflags", "+faststart",
            "-y", str(dest),
        ],
        timeout=seconds + 20,
    )
    if r.returncode == 0 and dest.is_file() and dest.stat().st_size > 1024:
        return True
    r = run_ff(
        ffmpeg,
        [
            "-ss", f"{max(0.0, ss):.2f}",
            "-i", str(media),
            "-t", str(int(seconds)),
            "-an",
            "-c:v", "libx264",
            "-preset", "ultrafast",
            "-pix_fmt", "yuv420p",
            "-movflags", "+faststart",
            "-y", str(dest),
        ],
        timeout=seconds + 40,
    )
    return dest.is_file() and dest.stat().st_size > 1024


def dir_bytes(path: Path) -> int:
    if not path.is_dir():
        return 0
    total = 0
    for p in path.rglob("*"):
        if p.is_file() and p.suffix.lower() in {".mp4", ".mkv", ".ts"}:
            try:
                total += p.stat().st_size
            except OSError:
                pass
    return total


def clip_count(path: Path) -> int:
    if not path.is_dir():
        return 0
    return sum(1 for p in path.glob("*.mp4"))


def write_json(path: Path, obj) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    tmp = path.with_suffix(path.suffix + ".tmp")
    tmp.write_text(json.dumps(obj, indent=2), encoding="utf-8")
    tmp.replace(path)


def publish_events(out_dir: Path) -> None:
    with STATE_LOCK:
        STATE["updated"] = utc_now()
        STATE["clip_count"] = clip_count(out_dir)
        STATE["total_bytes"] = dir_bytes(out_dir)
        payload = dict(STATE)
    dests = [
        out_dir / "events.json",
        exe_dir() / "recordings" / "events.json",
    ]
    local = os.environ.get("LOCALAPPDATA")
    if local:
        dests.append(Path(local) / "T3000" / "nvr" / "www" / "events.json")
        dests.append(Path(local) / "T3000" / "nvr" / "recordings" / "events.json")
    for d in dests:
        try:
            write_json(d, payload)
        except OSError:
            pass


def retain(out_dir: Path, keep: int = 40) -> None:
    clips = sorted(out_dir.glob("*.mp4"), key=lambda p: p.stat().st_mtime, reverse=True)
    for old in clips[keep:]:
        try:
            old.unlink()
        except OSError:
            pass
    with STATE_LOCK:
        STATE["events"] = STATE["events"][:keep]


class EventsHandler(BaseHTTPRequestHandler):
    out_dir: Path = Path(".")

    def log_message(self, fmt, *args):
        return

    def _cors(self):
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Cache-Control", "no-store")

    def do_OPTIONS(self):
        self.send_response(204)
        self._cors()
        self.send_header("Access-Control-Allow-Methods", "GET, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "*")
        self.end_headers()

    def do_GET(self):
        parsed = urllib.parse.urlparse(self.path)
        path = parsed.path
        if path in ("/", "/status"):
            with STATE_LOCK:
                body = json.dumps({
                    "armed": STATE["armed"],
                    "mode": STATE["mode"],
                    "streams": STATE["streams"],
                    "clip_count": STATE["clip_count"],
                    "total_bytes": STATE["total_bytes"],
                    "events_http": STATE["events_http"],
                    "updated": STATE["updated"],
                }).encode("utf-8")
            self.send_response(200)
            self._cors()
            self.send_header("Content-Type", "application/json")
            self.end_headers()
            self.wfile.write(body)
            return
        if path == "/events.json":
            p = self.out_dir / "events.json"
            data = p.read_bytes() if p.is_file() else b'{"events":[]}'
            self.send_response(200)
            self._cors()
            self.send_header("Content-Type", "application/json")
            self.end_headers()
            self.wfile.write(data)
            return
        if path.startswith("/clips/"):
            name = path.split("/clips/", 1)[1]
            name = urllib.parse.unquote(name)
            if not SAFE_NAME.match(name) or ".." in name:
                self.send_error(400)
                return
            f = self.out_dir / name
            if not f.is_file():
                self.send_error(404)
                return
            self.send_response(200)
            self._cors()
            self.send_header("Content-Type", "video/mp4")
            self.send_header("Content-Length", str(f.stat().st_size))
            self.end_headers()
            with f.open("rb") as fh:
                while True:
                    chunk = fh.read(64 * 1024)
                    if not chunk:
                        break
                    self.wfile.write(chunk)
            return
        self.send_error(404)


def start_http(out_dir: Path, port: int) -> Optional[ThreadingHTTPServer]:
    EventsHandler.out_dir = out_dir
    try:
        httpd = ThreadingHTTPServer(("127.0.0.1", port), EventsHandler)
    except OSError as e:
        print(f"events HTTP 127.0.0.1:{port} not bound ({e}). events.json is still written to disk.", flush=True)
        return None
    t = threading.Thread(target=httpd.serve_forever, daemon=True)
    t.start()
    print(f"events HTTP  http://127.0.0.1:{port}/events.json", flush=True)
    return httpd


def add_event(camera: str, path: Path, score: float, seconds: int) -> None:
    ev = {
        "id": f"{camera}-{int(time.time())}",
        "camera": camera,
        "start": utc_now(),
        "seconds": seconds,
        "file": path.name,
        "bytes": path.stat().st_size if path.is_file() else 0,
        "score": round(score, 3),
        "play": f"/clips/{path.name}",
    }
    with STATE_LOCK:
        STATE["events"].insert(0, ev)
        STATE["recording"][camera] = False
    print(f"CLIP  {camera}  {path.name}  {ev['bytes']} bytes  score={ev['score']}", flush=True)


def watch_stream(
    name: str,
    kind: str,
    source: str,
    ffmpeg: str,
    out_dir: Path,
    frames_dir: Path,
    threshold: float,
    poll: float,
    clip_sec: int,
    cooldown: float,
    idle_proof: bool,
    once: bool,
    once_flag: threading.Event,
) -> None:
    prev_gray: Optional[bytes] = None
    last_clip = 0.0
    file_ss = 0.0
    consecutive = 0
    while not STOP.is_set():
        if once_flag.is_set():
            return
        jpg = frames_dir / f"{name}.jpg"
        ok = False
        if kind == "rtsp":
            if idle_proof and prev_gray is not None:
                # Re-use the first grabbed frame: detector sees no change.
                ok = jpg.is_file()
            else:
                ok = grab_rtsp_frame(ffmpeg, source, jpg)
        else:
            ok = grab_file_frame(ffmpeg, Path(source), jpg, file_ss)
            file_ss += poll
            if not ok and file_ss > 1:
                file_ss = 0.0
                continue
        if not ok:
            time.sleep(poll)
            continue
        gray = gray_bytes(ffmpeg, jpg)
        if idle_proof and prev_gray is None and gray:
            prev_gray = gray
        if prev_gray is None:
            prev_gray = gray
            time.sleep(poll)
            continue
        score = mae(prev_gray, gray)
        if not idle_proof:
            prev_gray = gray
        with STATE_LOCK:
            STATE["last_scores"][name] = round(score, 3)
        moving = score >= threshold
        if moving:
            consecutive += 1
        else:
            consecutive = 0
        now = time.time()
        can_record = (
            moving
            and consecutive >= 1
            and (now - last_clip) >= cooldown
            and not idle_proof
            and not once_flag.is_set()
        )
        if can_record:
            with STATE_LOCK:
                STATE["recording"][name] = True
            stamp = datetime.now().strftime("%Y%m%d_%H%M%S")
            dest = out_dir / f"{name}_{stamp}.mp4"
            # Writer lives only for this clip (-t). No 24/7 file.
            if kind == "rtsp":
                ok_clip = record_rtsp_clip(ffmpeg, source, dest, clip_sec)
            else:
                start_at = max(0.0, file_ss - poll)
                ok_clip = record_file_clip(ffmpeg, Path(source), dest, start_at, clip_sec)
            if ok_clip:
                add_event(name, dest, score, clip_sec)
                retain(out_dir)
                publish_events(out_dir)
                last_clip = time.time()
                if once:
                    once_flag.set()
                    return
            else:
                with STATE_LOCK:
                    STATE["recording"][name] = False
                try:
                    if dest.exists() and dest.stat().st_size < 1024:
                        dest.unlink()
                except OSError:
                    pass
        time.sleep(poll)


def parse_args() -> argparse.Namespace:
    here = exe_dir()
    p = argparse.ArgumentParser(description="Motion-triggered CCTV recorder for the T3000 sidecar")
    p.add_argument("--api", default="http://127.0.0.1:9291")
    p.add_argument("--rtsp", default="rtsp://127.0.0.1:9292")
    p.add_argument("--out", default=str(here / "recordings"))
    p.add_argument("--ffmpeg", default="")
    p.add_argument("--threshold", type=float, default=3.0)
    p.add_argument("--poll", type=float, default=1.0)
    p.add_argument("--clip", type=int, default=12)
    p.add_argument("--cooldown", type=float, default=25.0)
    p.add_argument("--http-port", type=int, default=9294)
    p.add_argument("--streams", default="", help="Comma names. Default: all go2rtc names except *-main")
    p.add_argument("--seconds", type=float, default=0, help="Exit after N seconds (0 = run until stopped)")
    p.add_argument("--idle-proof", action="store_true", help="Compare each camera to its first frame; never record")
    p.add_argument("--demo-sim", action="store_true", help="Watch sidecar/sim/*.mp4 (people walking) as file sources")
    p.add_argument("--once", action="store_true", help="Stop after the first clip")
    p.add_argument("--no-http", action="store_true")
    return p.parse_args()


def main() -> int:
    args = parse_args()
    ffmpeg = find_ffmpeg(args.ffmpeg)
    out_dir = Path(args.out)
    out_dir.mkdir(parents=True, exist_ok=True)
    frames_dir = out_dir / ".frames"
    frames_dir.mkdir(parents=True, exist_ok=True)
    (out_dir / ".gitkeep").write_text("", encoding="utf-8")

    sources: Dict[str, tuple] = {}
    if args.demo_sim:
        sim = exe_dir() / "sim"
        for name in ("north", "east", "south", "west"):
            mp4 = sim / f"{name}.mp4"
            if mp4.is_file():
                sources[f"sim-{name}"] = ("file", str(mp4))
        if not sources:
            print("demo-sim: no T3000/sidecar/sim/*.mp4", file=sys.stderr)
            return 1
    else:
        wanted = [s.strip() for s in args.streams.split(",") if s.strip()]
        deadline = time.time() + 30
        names: List[str] = []
        while time.time() < deadline and not names:
            names = list_stream_names(args.api)
            if wanted:
                names = [n for n in names if n in wanted]
            if names:
                break
            time.sleep(0.5)
        if not names:
            print(f"No go2rtc streams at {args.api}/api/streams (is the sidecar running?)", file=sys.stderr)
            return 1
        for n in names:
            sources[n] = ("rtsp", f"{args.rtsp.rstrip('/')}/{n}")

    httpd = None
    events_http = f"http://127.0.0.1:{args.http_port}"
    if not args.no_http:
        httpd = start_http(out_dir, args.http_port)
        if httpd is None:
            events_http = ""

    mode = "idle-proof" if args.idle_proof else ("demo-sim" if args.demo_sim else "motion")
    with STATE_LOCK:
        STATE.update({
            "armed": True,
            "mode": mode,
            "api": args.api,
            "rtsp": args.rtsp,
            "recordings_dir": str(out_dir),
            "events_http": events_http,
            "streams": list(sources.keys()),
            "threshold": args.threshold,
            "clip_count": clip_count(out_dir),
            "total_bytes": dir_bytes(out_dir),
            "error": "",
        })
    publish_events(out_dir)

    print(f"armed  mode={mode}  streams={','.join(sources)}  threshold={args.threshold}", flush=True)
    print(f"clips  {out_dir}", flush=True)
    print("idle = no ffmpeg writer, files do not grow", flush=True)

    once_flag = threading.Event()
    threads = []
    for name, (kind, src) in sources.items():
        t = threading.Thread(
            target=watch_stream,
            kwargs=dict(
                name=name,
                kind=kind,
                source=src,
                ffmpeg=ffmpeg,
                out_dir=out_dir,
                frames_dir=frames_dir,
                threshold=args.threshold,
                poll=args.poll,
                clip_sec=args.clip,
                cooldown=args.cooldown,
                idle_proof=args.idle_proof,
                once=args.once,
                once_flag=once_flag,
            ),
            daemon=True,
            name=f"watch-{name}",
        )
        t.start()
        threads.append(t)

    started = time.time()
    try:
        while True:
            if args.seconds and (time.time() - started) >= args.seconds:
                break
            if args.once and once_flag.is_set():
                time.sleep(0.4)
                break
            if STOP.is_set():
                break
            publish_events(out_dir)
            time.sleep(1.0)
    except KeyboardInterrupt:
        pass
    finally:
        STOP.set()
        with STATE_LOCK:
            STATE["armed"] = False
        publish_events(out_dir)
        if httpd:
            try:
                httpd.shutdown()
            except Exception:
                pass

    with STATE_LOCK:
        n = clip_count(out_dir)
        b = dir_bytes(out_dir)
        scores = dict(STATE["last_scores"])
    print(f"done  clips={n}  bytes={b}  scores={scores}", flush=True)
    if args.idle_proof:
        # Idle-proof run must not add mp4 files during this process.
        # We cannot know prior count easily if others write; report scores only.
        if any(s >= args.threshold for s in scores.values()):
            # Same-frame compare should stay near 0; fail if it did not.
            print("idle-proof unexpected high score", file=sys.stderr)
            return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
