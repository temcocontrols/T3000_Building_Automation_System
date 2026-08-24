# Motion-triggered recording

CCTV records **only when there is motion**. There is no 24/7 writer. Idle scene = clip count and bytes stay the same.

This is not a second NVR. It watches go2rtc stream names already on the sidecar (sim-north/east/south/west and any live names such as cam-64) and uses the sidecar `ffmpeg.exe` for frame-diff + a short clip.

## Arm

Automatic when you start the sim:

```powershell
powershell -ExecutionPolicy Bypass -File T3000\sidecar\start-sim.ps1
```

`start-sim.ps1` starts go2rtc, then arms this recorder.

If the sidecar is already running (live 679 or sim):

```powershell
powershell -ExecutionPolicy Bypass -File T3000\sidecar\start-motion-record.ps1
```

## Where clips land

`T3000\sidecar\recordings\`

Events index: `T3000\sidecar\recordings\events.json`  
HTTP (localhost only): http://127.0.0.1:9294/events.json

## How it shows in CCTV

Tools → CCTV (or CRM #cctv, same WebView) has a **Motion events** list. It polls the events HTTP and can play a clip. The hotspot page lists the same events under the map.

## Proof

```powershell
powershell -ExecutionPolicy Bypass -File T3000\sidecar\test-motion.ps1
```

Idle (detector sees no change) must report clip count / bytes unchanged. Sim walking clips are allowed to produce one motion clip.

## Do not

- Do not turn on go2rtc `#record` (that is continuous).
- Do not uncomment MediaMTX `record: yes`.
- Do not commit `recordings\` or live camera passwords.
