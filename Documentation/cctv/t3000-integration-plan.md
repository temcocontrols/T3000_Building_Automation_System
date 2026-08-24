# T3000 CCTV integration plan

Status: documentation first. Implementation follows on the **Maurice** branch after this review. No extra agent branches.

The goal is one product: T3000 customers get building automation and cameras in the same app. CRM is the shop/office viewer of the same sidecar, not a second NVR.

## What already works (local Maurice, 679 Victoria)

This is the live desk stack, not a future sketch.

- **Sidecar** next to T3000: go2rtc on `127.0.0.1:9291/9292/9293`, motion recorder + pages on `9294`, SADP on `9295`, face/heatmap on `9296`.
- **Cameras:** Hikvision RTSP at `192.168.1.64-67` (cam-64..67). Sub stream only for viewing. No extra Chrome / VLC / go2rtc UI.
- **CRM** left tree `#cctv` iframes the sidecar pages: Live, Heatmap, People, Events, Settings.
- **Live:** 2x2 tiles. Timeline in that window (play/pause, next/back event, zoom, hourly ticks). Play skips idle gaps. Each tile shows that camera's own clip or stays live.
- **Record:** motion-only to `C:\Xdrive\T3000_DVR`. 1 second pre-roll ring from go2rtc (not a camera setting). Post-roll from DVR settings.
- **People:** auto-enroll as Person1, Person2. Name, Unknown (?), Ignore. Stored in `cctv.people` and `faces\names.json`.
- **Heatmap:** floor dots + FOV cones, drag to move, cone handle to rotate, persist per building. Today / Week / All / Reset.
- **Events:** list + clip player, existing clips seeded.
- **Postgres:** schema `cctv` in the same local CRM database (`127.0.0.1:5432`, database `postgres`): `people`, `cameras`, `settings`, `heatmap_cells`, `events`, `person_sightings`. Not the NAS.
- **SOP:** only T3000 CCTV / CRM CCTV may hold a stream. Reconnect drops leftover ffmpeg/go2rtc sessions. See [sop-view-only.md](sop-view-only.md).

T3000 already has **Tools -> CCTV** (WebView + sidecar) from the earlier NVR slice. That hook is the product door. Do not invent a second viewer.

## Product rule

One sidecar, one UI chrome, two shells:

| Shell | Who | How |
| --- | --- | --- |
| T3000 | Customer / installer | Tools -> CCTV WebView loads `127.0.0.1:9294/live` (and the same People / Heatmap / Events / Settings pages) |
| Temco CRM | Office | Left-tree `#cctv` iframes those same URLs |

If a feature is not on `9294`, it does not exist. CRM and T3000 must not grow private copies of live, timeline, or people.

## Sites and cameras

- T3000 source of sites is **`Building`** in `Database\T3000.db` (plus `Building_ALL`). One row per building. CCTV cameras attach to `Building.ID`.
- CRM/PM buildings live in Postgres `pm.buildings` today (same IDs: 1 = Default_Building, 2 = 679_Victoria). Harmonize later. Until then do not treat `T3000.db` as CRM's live site list.
- After merge: T3000 Building is the catalog. Sidecar and CRM read the same IDs. `cctv.cameras.building_id` matches `Building.ID`.

## Process and storage

- T3000 start (or first Tools -> CCTV) runs `T3000\sidecar\start-dvr-background.ps1` if go2rtc / recorder are down. Login startup shortcut already exists on the Maurice desk; the installer should do the same for customers.
- Reconnect from the Live page. Do not open extra browser tabs to check a camera.
- Clips stay on a **local** disk (`C:\Xdrive\T3000_DVR` here). Not the office NAS. Settings: storage path, motion-only, 1s pre-roll, post-roll, retention, per-camera enable.
- Face models and `faces\` stay next to that DVR path.

## Database split (until harmonize)

| Data | Now | After T3000 merge |
| --- | --- | --- |
| Buildings / sites | T3000.db `Building` + CRM `pm.buildings` | T3000.db is catalog; CRM mirrors IDs |
| Cameras | `pm.cameras` + `cctv.cameras` | `cctv.cameras` (building_id = Building.ID), T3000 UI edits this |
| DVR settings | `dvr-settings.json` + T3000.db `CCTV_DvrSetting` + `cctv.settings` | One write path: sidecar `/settings` persists JSON + T3000.db + `cctv.settings` |
| People, events, heatmap | `cctv.*` + files under T3000_DVR | Same. T3000 WebView talks to `9294`, not raw SQL |

Do not create a third `sites.db`.

## Phases (code after this PR)

### 1. T3000 WebView is the same app

- Tools -> CCTV loads `http://127.0.0.1:9294/live` (not a second HTML tree).
- Optional in-window nav: Live, Heatmap, People, Events, Settings — same URLs as CRM.
- Start sidecar if ports are down. Show a one-line "recorder not running" if 9294 is dead.
- Reconnect + SOP apply.

Done when: installer / debug build opens four 679 (or sim) tiles from Tools -> CCTV with no extra Chrome.

### 2. Building picker

- WebView takes `?building=2` from the current T3000 building.
- Changing building in T3000 reloads the same pages with that id.
- Discover (SADP) adds rows to `cctv.cameras` for that building.

Done when: two buildings can each have their own cameras without mixing tiles.

### 3. One settings write path

- Gear / Settings page is the only editor.
- Writes JSON, T3000.db, and `cctv.settings` together (already started on 9294).
- Retention job deletes old clips using those numbers.

### 4. People and events in T3000

- Same People and Events pages inside the WebView.
- No separate T3000 face dialog.
- Watchlist / Ignore stay as now.

### 5. Ship extras (only when 1-4 are boring)

- Bookmark/protect an event from retention.
- Heatmap decay + occupancy count.
- After-hours notify for named / first-seen (not every motion).
- Merge duplicate PersonN rows.

## Out of scope for this PR

- No sidecar or CRM code in this pull request.
- No new git branch.
- Do not reopen or stack on draft PR 491 (`cursor/onvif-nvr-sidecar-dd58`).
- Do not enable ONVIF device management unless Maurice asks. 679 is Hikvision RTSP.
- Do not put camera passwords in git.

## How to review

Read this file and [sop-view-only.md](sop-view-only.md). Confirm Tools -> CCTV is the customer door and `9294` is the only UI. Implementation is a later Maurice push, not this PR.
