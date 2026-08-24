# CCTV SOP: this app only

Standing rule (Maurice): cameras are viewed only from T3000 CCTV or CRM CCTV. Nothing else.

## Who may hold a stream
- T3000 CCTV (Tools, CCTV)
- CRM left-rail #cctv (same sidecar)
- The DVR recorder, SADP probe, and face scanner that this sidecar starts

## Who must be closed
On CCTV open, and again on Reconnect (R in the CRM header, next to the gear):
- leftover go2rtc / WebRTC / play_url sessions
- leftover ffmpeg (one-shot grabs, old transcodes)
- extra browser tabs of the go2rtc UI or duplicate CCTV pages
- VLC or any other RTSP player

Those leftovers eat Hikvision RTSP slots. That is why only one tile stays live.

## How reconnect works
1. CRM calls http://127.0.0.1:9294/reconnect (also /close-stale).
2. T3000/sidecar/reconnect.ps1 drops stale ffmpeg, restarts go2rtc on the live yaml (cam-64..67 only, no -main double pull).
3. CRM reloads the four tiles.

Do not open extra Chrome tabs to check a camera. Use Reconnect in this app.

## Storage
DVR path stays local (C:\Xdrive\T3000_DVR by default). Not the NAS.
