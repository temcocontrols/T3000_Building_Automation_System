# T3000 CCTV user manual

For homeowners and building operators who already run **T3000** on a Windows PC.

This book describes **Tools → CCTV** on branch `CCTV`. It is the Temco product manual. Vendor folders next to this one (UniFi, Reolink, Synology) are inspiration only.

Figures `images/01`–`09` are labeled placeholders. **Real screenshots live in the pull-request conversation** until a Windows machine captures Tools → CCTV. Placeholder art is empty rooms and a box camera — **no people**.

---

## Current vs coming next

| Chapter | In this slice | Coming next |
| --- | --- | --- |
| What CCTV is | Cameras inside T3000, localhost sidecars | Cloud, phone app, AI |
| Tools → CCTV | Menu, CCTV window, sidecar start/stop | Dedicated CCTV workstation mode |
| First run / go2rtc | Live video after dropping `go2rtc.exe`; clear red panel if it is missing | Bundled installer that always includes the sidecar |
| ONVIF or RTSP | Discover on the LAN, or paste an RTSP URL | Guided camera wizard, credentials vault |
| Live view | Tiles (WebRTC, then MP4, then MJPEG) | PTZ joystick, multi-monitor wall |
| Playback | **Not shipped** | Timeline, clips, export |
| Events + T3000 alarms | **Not shipped** | Motion / disconnect → T3000 alarm |
| Recording | **Not shipped** (MediaMTX is optional and reserved) | Continuous / detection record, retention |
| Settings | **Not shipped** | Schedules, users, storage |

If a later chapter says **Coming next**, the control is not on the screen yet. Do not look for it in this build.

---

## 1. What CCTV is

T3000 CCTV is the camera page inside the same T3000 you already use for controllers, graphics, and alarms. You do not install a second NVR program for this first slice.

**What you get today**

- One menu item: **Tools → CCTV**
- A window titled **CCTV** that can find ONVIF cameras on your LAN or play an RTSP URL you paste
- Live video tiles on the T3000 PC
- Your camera list saved so it comes back the next time you open CCTV

**How it runs (you do not have to manage this every day)**

T3000 starts a small helper program named **go2rtc** on this PC only (`127.0.0.1`). Video never uses T3000’s existing ports **9103**, **9104**, or **3003**. Closing the CCTV window stops helpers that T3000 started.

**Coming next:** phone app, off-site relay, motion AI, and a full recorder UI. Those are not in this book’s “current” steps.

---

## 2. Tools → CCTV

![Tools menu: CCTV](images/01-tools-cctv.png)

1. Start **T3000** as usual (it asks for Administrator, same as always).
2. On the menu bar choose **Tools → CCTV**.
3. A WebView window titled **CCTV** opens. The heading on the page is **T3000 CCTV**.

If T3000 instead offers a download for **Microsoft Edge WebView2 Runtime**, install that (same runtime HVAC graphics already use), then choose **Tools → CCTV** again.

Closing the CCTV window returns you to the rest of T3000. Other tools are unchanged.

**Coming next:** a larger CCTV workspace (timeline, events, settings tabs) behind the same menu.

---

## 3. First run / missing go2rtc

![First run and missing go2rtc](images/02-first-run-go2rtc.png)

Live video needs **`go2rtc.exe`** next to T3000 (or in a `sidecar` folder). It is a separate MIT program. T3000 does not compile it and does not put it inside `T3000.exe`.

### When everything is in place

The top-right pills show **go2rtc :9191** in green. The Sidecar panel says the helper started (or that T3000 reused one already listening). **MediaMTX** may stay grey/off. That is normal: live view does not need it.

### When `go2rtc.exe` is missing

T3000 **still starts**. Every other T3000 tool still works. **Tools → CCTV** still opens.

The Sidecar panel turns **red** and says `go2rtc.exe was not found. T3000 is running normally.` It lists folders to drop the file into, plus the official Windows zip.

1. Download [go2rtc_win64.zip](https://github.com/AlexxIT/go2rtc/releases/download/v1.9.14/go2rtc_win64.zip).
2. Unzip and copy **`go2rtc.exe`** to **any one** of:
   - `T3000 Output\release\sidecar\go2rtc.exe` (or next to `T3000.exe`)
   - `T3000\sidecar\go2rtc.exe` in the source tree
3. Close CCTV and choose **Tools → CCTV** again. You do not reinstall T3000.

If something else already owns port **127.0.0.1:9191** and it is not go2rtc, CCTV opens offline and tells you the port is in use.

**Coming next:** the Windows installer can ship `go2rtc.exe` so operators never hunt for a zip. That is not required for this slice.

---

## 4. Discover ONVIF or paste RTSP

The PC and the camera must be on the **same LAN**. ONVIF discovery uses multicast. There is no bundled fake “demo camera.”

### Discover ONVIF

![ONVIF discovery](images/03-onvif-discover.png)

1. If the camera needs a login, type **Username** and **Password** first.
2. Click **Discover ONVIF**.
3. Wait a few seconds. Each device shows a name and URL.
4. Click **Add** on the camera you want.

If the list is empty, the camera is off, on another VLAN, or blocked from multicast. Try the RTSP steps below, or ask whoever set up the camera for its stream URL.

### Paste an RTSP URL

![Add RTSP](images/04-add-rtsp.png)

1. **Name** — a short label you will recognize, for example `lobby`.
2. **RTSP or ONVIF URL** — from the camera’s own web page, for example  
   `rtsp://admin:password@192.168.1.20/Streaming/Channels/101`
3. Click **Add and play**.

Any RTSP this Windows PC can reach works (a camera, another recorder, a phone app that publishes RTSP).

**Refresh list** pulls streams go2rtc already knows. **Remove** on a tile deletes that camera from the saved list.

Cameras persist in the CCTV window (`localStorage`) and in `%LOCALAPPDATA%\T3000\nvr\cameras.json`. Close CCTV and reopen **Tools → CCTV** — they should come back.

**Coming next:** a shorter “add camera” wizard and stored site credentials. Today you type the URL or discover ONVIF once.

---

## 5. Live view

![Live view tiles](images/05-live-view.png)

After Add or Discover, the right-hand **Live view** area shows a tile per camera.

**Success looks like this**

- Green **go2rtc :9191**
- Sidecar text such as “CCTV sidecar started (go2rtc on 127.0.0.1:9191).”
- **Moving video** in the tile (not a frozen snapshot)
- The T3000 status bar may repeat the sidecar message

The Windows viewer cannot play raw `rtsp://` in a video box. The page asks go2rtc for **WebRTC**, then **MP4**, then **MJPEG**. You do not pick the mode.

Tiles are muted by default so a wall of cameras does not blast audio. Use the tile controls if you need sound on a camera that has a microphone.

**Coming next:** larger video wall layouts, PTZ, and two-way talk. This slice is watch-live-on-this-PC.

---

## 6. Playback

![Playback — coming next](images/06-playback.png)

**Coming next.** This slice has no timeline, no calendar, and no clip export.

MediaMTX (if you drop `mediamtx.exe` beside go2rtc) reserves localhost ports for later playback. The CCTV header may show **MediaMTX :9197**. That does **not** mean you can scrub yesterday’s video yet.

Until playback ships, use the camera’s own app or NVR if you need history.

---

## 7. Events + T3000 alarms

![Events and alarms — coming next](images/07-events-alarms.png)

**Coming next.** CCTV does not yet raise T3000 alarms for motion, line-cross, or camera disconnect.

Today, building alarms stay on the controllers and T3000 alarm screens you already know. Camera motion, if you need it now, stays on the camera or a separate recorder.

The intended later story: a camera event becomes a T3000 alarm the operator already acknowledges — one product, one alarm list. That wiring is not in this build.

---

## 8. Recording

![Recording — coming next](images/08-recording.png)

**Coming next.** This slice does not offer Always / Detection / Never recording, schedules, or a recordings folder in the UI.

A `recordings` directory under `%LOCALAPPDATA%\T3000\nvr\` is reserved. MediaMTX config is written with record commented out. Do not expect files to appear there for the operator demo.

If you need recording today, use the camera’s SD card or an existing NVR, and use T3000 CCTV for **live** only.

---

## 9. Troubleshooting

![Future settings — coming next](images/09-future-settings.png)

There is no Settings page in this slice (schedules, retention, and users are **coming next**). Use this table first.

| You see | What to do |
| --- | --- |
| Prompt to install WebView2 | Install the [Edge WebView2 Runtime](https://developer.microsoft.com/en-us/microsoft-edge/webview2/), then Tools → CCTV again |
| Red “go2rtc.exe was not found” | Drop `go2rtc.exe` in `sidecar\` or next to `T3000.exe`, close CCTV, reopen it |
| Port 9191 in use | Close the other program using `127.0.0.1:9191`, or stop a leftover go2rtc, then reopen CCTV |
| Green go2rtc, no devices | Same LAN? Credentials entered? Try a pasted `rtsp://` URL from the camera’s web UI |
| Tile added, no motion | Confirm the URL in a VLC window on this PC. Wrong path or password is the usual cause |
| CCTV window closes and helpers stop | Expected. Open **Tools → CCTV** again when you want live view |
| Want playback, events, or record | Not in this slice — see chapters 6–8 |

Installer and build steps for technicians: [DEMO.md](../../../DEMO.md). Port and sidecar notes: [nvr.md](../../nvr.md).
