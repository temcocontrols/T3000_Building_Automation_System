# Merge CCTV into T3000 (developers)

CCTV is already on **Maurice**. It is not a second app. Tools → CCTV opens a WebView that talks to localhost sidecars. Push small changes to Maurice. Do not open a PR unless someone asks.

## What to take

| Path | Why |
| --- | --- |
| `T3000/T3000NvrSidecar.cpp` `.h` | Starts go2rtc / MediaMTX, copies UI into `%LOCALAPPDATA%\T3000\nvr\www` |
| `T3000/MainFrm.cpp` `OnToolsCameras` | Tools → CCTV |
| `T3000/ResourceFile/nvr/` | `index.html` live view, `hotspot.html` path map, `hotspot-demo.json` |
| `T3000/sidecar/` | Sim clips, `start-sim.ps1`, `test-sim.ps1`, `go2rtc-sim.yaml`. Do not commit `*.exe` |
| `Database/cctv.sql` | `CCTV_Camera` keyed to `Building.ID`. Apply with `tools/apply-cctv-db.py` |
| `Documentation/cctv/` | Operator book + this page |

## How it hooks

1. Menu: `ID_TOOLS_CAMERAS` → `CMainFrame::OnToolsCameras` → sidecar start → WebView `http://127.0.0.1:<go2rtc>/index.html`.
2. On start, `CopyUiIntoConfigWww` copies **all** of `ResourceFile\nvr\` (`index.html`, `hotspot.html`, `hotspot-demo.json`). A rebuild is enough; testers can also drop those three files next to `T3000.exe\ResourceFile\nvr\`.
3. Sites are `Building` / `Building_ALL` in `Database\T3000.db`. Cameras: `CCTV_Camera.building_id`.
   - `1` Default_Building (shop / four sim cams)
   - `2` 679_Victoria (no cams yet)
4. CRM reads the same table: `Documentation/cctv/crm-cameras-by-building.md`.

## Test without a browser

```powershell
powershell -ExecutionPolicy Bypass -File T3000\sidecar\test-sim.ps1
```

Need `go2rtc.exe` and `ffmpeg.exe` in `T3000\sidecar\`. Ports 9291/9292/9293 (not 9192 — Project Management).

## Do not

- Create `sites.db` or a `cursor/*` branch
- Hardcode sim URLs in CRM
- Fall back to building 1 when building 2 has zero cameras
- Commit sidecar `*.exe`
