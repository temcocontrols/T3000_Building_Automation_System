# Simulated job-site cameras

Four looping clips of the same early-stage construction site (no walls), cameras facing inward. Not ffmpeg test patterns. Ports avoid Project Management on 9192.

| Camera | Angle | RTSP |
| --- | --- | --- |
| sim-north | North fence, looking south | rtsp://127.0.0.1:9292/sim-north |
| sim-east | East fence, looking west | rtsp://127.0.0.1:9292/sim-east |
| sim-south | South gate, looking north | rtsp://127.0.0.1:9292/sim-south |
| sim-west | West fence, looking east | rtsp://127.0.0.1:9292/sim-west |

HTTP/API: http://127.0.0.1:9291  
WebRTC: 127.0.0.1:9293

```powershell
powershell -ExecutionPolicy Bypass -File T3000\sidecar\start-sim.ps1
```

Then Tools → CCTV (or CRM #cctv) and **Add and play** each RTSP URL. Clips are 10s H.264 loops in `sim\`.
