# Simulated job-site cameras

Four looping clips of the real early-stage lot (white ICF + rebar, wooden fence, orange Hitachi). Cameras face inward from the four corners so the same workers can be tracked across views. Ports avoid Project Management on 9192.

| Camera | Angle | RTSP |
| --- | --- | --- |
| sim-north | NW corner, looking southeast | rtsp://127.0.0.1:9292/sim-north |
| sim-east | NE corner, looking southwest | rtsp://127.0.0.1:9292/sim-east |
| sim-south | SE corner, looking northwest | rtsp://127.0.0.1:9292/sim-south |
| sim-west | SW corner, looking northeast | rtsp://127.0.0.1:9292/sim-west |

HTTP/API: http://127.0.0.1:9291
WebRTC: 127.0.0.1:9293

```powershell
powershell -ExecutionPolicy Bypass -File T3000\sidecar\start-sim.ps1
```

Then Tools → CCTV (or CRM #cctv) and **Add and play** each RTSP URL. Clips are 10s H.264 loops in `sim\`.

`start-sim.ps1` also arms **motion-triggered recording** (see [MOTION.md](MOTION.md)). Idle scene = no file growth. Clips land in `T3000\sidecar\recordings\`. Events: http://127.0.0.1:9294/events.json

If the sidecar is already running without the sim script:

```powershell
powershell -ExecutionPolicy Bypass -File T3000\sidecar\start-motion-record.ps1
```

## Automated test (no browser)

```powershell
powershell -ExecutionPolicy Bypass -File T3000\sidecar\test-sim.ps1
```

Starts go2rtc on 9291/9292/9293 if needed, checks ffmpeg.exe + go2rtc.exe sit together, lists sim-north/east/south/west, then pulls video/mp4 from each. Exit 0 = all pass, 1 = fail.

Motion idle vs walking clip (does not start extra browsers):

```powershell
powershell -ExecutionPolicy Bypass -File T3000\sidecar\test-motion.ps1
```
