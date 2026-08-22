# T3000 Cameras / NVR (first slice)

Temco customers get cameras plus building automation in one T3000 install. v1 launches localhost sidecar processes, discovers ONVIF cameras, and shows live video in the existing WebView2 host. Motion detection, AI, mobile apps, full timeline playback, and cloud relay are out of scope.

T3000.exe stays 32-bit MFC. Sidecars are **separate processes** (64-bit is fine) talking HTTP on 127.0.0.1. Do not load go2rtc or MediaMTX as a DLL into T3000.exe.

## What we ship in this repo

| Piece | Location | Notes |
| --- | --- | --- |
| Launcher / lifecycle | `T3000/T3000NvrSidecar.cpp` | `CreateProcess`, job object, port checks |
| Cameras UI | `T3000/ResourceFile/nvr/index.html` | Self-contained page (T3000Webview is a submodule) |
| Menu entry | Tools → **Cameras (NVR)** | Opens WebView2 |
| Fetch script | `tools/fetch-nvr-sidecars.ps1` | Downloads MIT binaries for devs / CI |
| Sidecar folder | `T3000/sidecar/` | Place `go2rtc.exe` and `mediamtx.exe` here |

Large binaries are **not** committed. The MSI picks them up only if they are copied into `T3000 Output\release\sidecar\` before Advanced Installer's `AddFolder` of the release tree.

## Sidecar bricks

| Binary | License | Role in v1 |
| --- | --- | --- |
| [go2rtc](https://github.com/AlexxIT/go2rtc) v1.9.14 | MIT | ONVIF discovery, RTSP ingest, WebRTC / MSE / MP4 live view |
| [MediaMTX](https://github.com/bluenviron/mediamtx) v1.20.1 | MIT | Layout + config for later record/playback. Optional for live view. |

FFmpeg is not shipped. Do not vendor GPL/AGPL NVR code (ZoneMinder, Moonfire, OpenNVR, motion) and do not statically link GPL into T3000.exe or `t3_webview_api.dll`.

## Obtain the sidecars (developers)

From a PowerShell prompt at the repo root:

```powershell
powershell -ExecutionPolicy Bypass -File tools\fetch-nvr-sidecars.ps1
```

This writes:

- `T3000\sidecar\go2rtc.exe`
- `T3000\sidecar\mediamtx.exe`

A VS2019 post-build step copies `ResourceFile\nvr\` to the output tree and, if present, `sidecar\*.exe` next to `T3000.exe`.

You can also drop the exes next to `T3000.exe` or under `nvr\`. Search order:

1. `{T3000.exe dir}\sidecar\go2rtc.exe` (same for `mediamtx.exe`)
2. `{T3000.exe dir}\go2rtc.exe`
3. `{T3000.exe dir}\nvr\go2rtc.exe`

T3000 still builds and runs if the exes are missing. Opening Cameras shows a clear message in the WebView.

## Ports (127.0.0.1 only)

Do not use 9103 (Rust HTTP), 9104 (Rust WS), or 3003 (Quasar dev).

| Service | Port | Purpose |
| --- | --- | --- |
| go2rtc HTTP / API / UI | 9191 | Live view page + `/api` |
| go2rtc RTSP | 9192 | Local RTSP republish |
| go2rtc WebRTC | 9193 | WebRTC ICE |
| MediaMTX RTSP | 9194 | Future record ingest |
| MediaMTX playback | 9196 | Future VOD |
| MediaMTX API | 9197 | Health / later control |

## Config and user data

Generated under `%LOCALAPPDATA%\T3000\nvr\` (same LocalAppData root as WebView2's `T3000` user-data folder):

- `go2rtc.yaml` — rewritten on each Cameras launch
- `mediamtx.yml` — record/playback hook (`record` stays commented)
- `www\index.html` — copy of the Cameras page served by go2rtc `static_dir`
- `cameras.json` — persisted camera list (also kept in WebView `localStorage`)
- `recordings\` — reserved for MediaMTX

## How to open the view

1. Build T3000 (VS2019, Win32, Release or Debug) with or without sidecar exes.
2. Fetch or copy `go2rtc.exe` into `sidecar\` (and optionally `mediamtx.exe`).
3. Run T3000 and choose **Tools → Cameras (NVR)**.
4. T3000 starts go2rtc (and MediaMTX if present), then opens WebView2 at `http://127.0.0.1:9191/index.html`.
5. Closing the Cameras window stops processes T3000 started. Exit of T3000 also stops them. An already-running go2rtc on 9191 is reused and not killed.

## Point at a camera

- **ONVIF:** click **Discover ONVIF**, enter credentials if needed, then **Add**.
- **RTSP:** set a name and URL such as `rtsp://user:pass@192.168.1.20/Streaming/Channels/101`, then **Add and play**.
- Live tiles prefer go2rtc WebRTC, then `/api/stream.mp4`, then MJPEG. WebView2 does not play raw RTSP.

## Installer / CI

`T3000.aip` is not edited (Advanced Installer projects are easy to break without a local test). `Build.yml` / `Release.yml`:

1. Copy `T3000\ResourceFile\nvr\` into `T3000 Output\release\ResourceFile\nvr\`.
2. Optionally run `tools/fetch-nvr-sidecars.ps1 -OutDir "T3000 Output\release\sidecar"` so the existing `AddFolder "APPDIR" ".\T3000 Output\release"` includes the MIT exes.
3. Do **not** add go2rtc/MediaMTX to the SignPath PE list (third-party, like sqlite).

## License isolation

T3000 remains MIT. go2rtc and MediaMTX remain separate MIT programs. Notices: `T3000/sidecar/NOTICE.txt`.
