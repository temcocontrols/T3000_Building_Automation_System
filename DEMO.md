# T3000 CCTV — Windows teammate demo

**This branch is for a live demo on a normal T3000 Windows box. Do not merge until people have seen it running.**

The Linux cloud agent that opened the PR **cannot** start T3000.exe, WebView2, or a LAN camera. Follow these steps on your VS2019 machine. There is no special installer and you do not need master.

Developer background: [Documentation/nvr.md](Documentation/nvr.md).

---

## 1. Check out this branch and build (same as the rest of T3000)

```bat
git fetch origin
git checkout CCTV
```

1. Open **`T3000 - VS2019.sln`** in Visual Studio 2019.
2. Toolbar: configuration **Release** (or Debug), platform **Win32**.
3. **Build → Build Solution** (Ctrl+Shift+B).

Same MFC / x86 / requireAdministrator path as always. Sidecar `.exe` files are **not** required to compile. If the build fails, it is a normal T3000 compile error — see [README_Build.md](README_Build.md).

**Ignore the red GitHub check named “Testing the MSBuild” on this PR.** That workflow has been failing on `master` as well (missing MFC and .NET Framework 4.5.2 targeting pack on `windows-latest`). It dies before compiling `T3000NvrSidecar.cpp`. Your local VS2019 box is the real build.

Built app:

| Config | T3000.exe |
| --- | --- |
| Release | `T3000 Output\release\T3000.exe` |
| Debug | `T3000 Output\debug\T3000.exe` |

Post-build already copies `T3000\ResourceFile\nvr\index.html` next to that exe.

You also need the **Microsoft Edge WebView2 Runtime** (same as HVAC graphics / Modbus Register v2). If CCTV warns that it is missing, install it from https://developer.microsoft.com/en-us/microsoft-edge/webview2/ then retry.

---

## 2. Drop `go2rtc.exe` where the launcher will find it

Live video needs **go2rtc** (MIT, separate process, 64-bit is fine). **MediaMTX is optional** for this demo.

### Official Windows zips (no script required)

1. Download **[go2rtc_win64.zip](https://github.com/AlexxIT/go2rtc/releases/download/v1.9.14/go2rtc_win64.zip)** from [go2rtc v1.9.14](https://github.com/AlexxIT/go2rtc/releases/tag/v1.9.14).
2. Unzip. Copy **`go2rtc.exe`** to **any one** of these folders (create `sidecar` if needed):

**Preferred (no rebuild):** next to the exe you just built

```
T3000 Output\release\sidecar\go2rtc.exe
```

or simply

```
T3000 Output\release\go2rtc.exe
```

(Debug: the same under `T3000 Output\debug\`.)

**Also works:** source-tree folder (launcher walks `..\..\T3000\sidecar` from the output dir)

```
T3000\sidecar\go2rtc.exe
```

A Release post-build will also copy `T3000\sidecar\*.exe` into `T3000 Output\release\sidecar\` if they are already there.

### Optional MediaMTX (record/playback home, not needed for live view)

- Zip: **[mediamtx_v1.20.1_windows_amd64.zip](https://github.com/bluenviron/mediamtx/releases/download/v1.20.1/mediamtx_v1.20.1_windows_amd64.zip)**
- Drop `mediamtx.exe` in the **same** folder as `go2rtc.exe`.
- The CCTV header shows **MediaMTX :9197** when it is up, **MediaMTX off** when it is not. Live tiles still work.

### Optional: fetch script instead of a browser download

From the repo root, in PowerShell:

```powershell
powershell -ExecutionPolicy Bypass -File tools\fetch-nvr-sidecars.ps1
```

That writes `T3000\sidecar\go2rtc.exe` and `T3000\sidecar\mediamtx.exe`. Then either rebuild once, or copy those two files into `T3000 Output\release\sidecar\`.

**Do not commit the `.exe` files.**

---

## 3. Open the CCTV view

1. Run **`T3000.exe`** (the one you built — it will ask for Administrator, as usual).
2. Menu: **Tools → CCTV**.
3. A WebView2 window titled **CCTV** opens.

T3000 starts `go2rtc.exe` (and `mediamtx.exe` if present) on localhost, then loads `http://127.0.0.1:9191/index.html`. Closing that window stops processes T3000 started.

---

## 4. Point at a camera

The PC and the camera must be on the **same LAN** (ONVIF discovery uses multicast).

### Option A — ONVIF on the LAN

1. If the camera needs a login, type **Username** / **Password** first.
2. Click **Discover ONVIF**.
3. Wait a few seconds. Devices appear in the list (name + URL).
4. Click **Add** on the one you want.

### Option B — paste an RTSP URL (no ONVIF camera)

1. **Name:** e.g. `lobby`
2. **RTSP or ONVIF URL:** from the camera’s web UI, for example  
   `rtsp://admin:password@192.168.1.20/Streaming/Channels/101`
3. Click **Add and play**.

Any RTSP the Windows box can reach works (another recorder, a camera, a phone app that publishes RTSP). There is no fake “demo stream” bundled. Without a reachable URL you still see the CCTV UI and sidecar status, but no video.

WebView2 cannot play raw `rtsp://` inside a `<video>` tag. The page asks go2rtc for **WebRTC**, then **MP4**, then **MJPEG**.

---

## 5. What success looks like

- Header pills: **go2rtc :9191** (green). MediaMTX green only if you dropped that exe.
- Sidecar panel: something like “CCTV sidecar started (go2rtc on 127.0.0.1:9191).”
- After Add / Discover: a **live tile** with moving video (not a frozen snapshot).
- Status bar at the bottom of T3000 may repeat the sidecar message.

Close **CCTV**, reopen **Tools → CCTV** — saved cameras come back (WebView `localStorage` + `%LOCALAPPDATA%\T3000\nvr\cameras.json`).

---

## 6. What you should see if sidecar exes are missing

Leave `go2rtc.exe` out on purpose if you want to show this:

1. T3000 still starts and the rest of the app is unchanged.
2. **Tools → CCTV** still opens a CCTV window (no crash).
3. The sidecar panel is **red** and says `go2rtc.exe was not found. T3000 is running normally.`
4. It lists the folders to drop the file into, plus the official zip URL.
5. You can still use every other T3000 tool.

Drop `go2rtc.exe` into one of those folders, close CCTV, open it again — no reinstall.

---

## 7. Ports (localhost only — 9103 / 9104 / 3003 stay free)

| Process | Bind | Port | Used for |
| --- | --- | --- | --- |
| go2rtc | 127.0.0.1 | **9191** | CCTV page + API |
| go2rtc | 127.0.0.1 | 9192 | RTSP republish |
| go2rtc | 127.0.0.1 | 9193 | WebRTC |
| MediaMTX | 127.0.0.1 | 9194 / 9196 / 9197 | Optional; later record/playback |

Already taken by T3000 and **left alone**:

| Port | Owner |
| --- | --- |
| **9103** | Rust `t3_webview_api` HTTP |
| **9104** | Rust WebSocket |
| **3003** | Quasar / T3000Webview dev UI |

If something else already owns **9191** and it is not go2rtc, CCTV opens offline and tells you the port is in use.

Config written on each open: `%LOCALAPPDATA%\T3000\nvr\` (`go2rtc.yaml`, `mediamtx.yml`, `www\index.html`).

---

## Quick checklist for the person running the demo

- [ ] Branch `CCTV` built Win32 in VS2019
- [ ] `go2rtc.exe` sitting next to `T3000.exe` or in `sidecar\`
- [ ] **Tools → CCTV** opens
- [ ] Green **go2rtc :9191**
- [ ] ONVIF **Discover** or a pasted `rtsp://...` shows **live video**
- [ ] (Optional) Hide `go2rtc.exe` and reopen CCTV — red message, T3000 still works
