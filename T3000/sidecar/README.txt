T3000 CCTV sidecar binaries
===========================

Open Tools → CCTV. Merge notes: Documentation\cctv\DEVELOPER.md. Demo: DEMO.md at the repo root.

Drop go2rtc.exe here (and optionally mediamtx.exe). Official zips:

  https://github.com/AlexxIT/go2rtc/releases/download/v1.9.14/go2rtc_win64.zip
  https://github.com/bluenviron/mediamtx/releases/download/v1.20.1/mediamtx_v1.20.1_windows_amd64.zip

Or:

  powershell -ExecutionPolicy Bypass -File ..\..\tools\fetch-nvr-sidecars.ps1

Then either rebuild, or copy the exes next to T3000.exe / T3000 Output\release\sidecar\.
Do not commit the .exe files.


Motion-triggered recording (no 24/7 writers): start-motion-record.ps1 / MOTION.md
Clips: recordings\   Events: http://127.0.0.1:9294/events.json

