# CRM: cameras by Building.ID

T3000 sites are `Building` rows in `Database\T3000.db`. CCTV follows `Building.ID`. CRM must not hardcode sim URLs.

## Database

- File: `C:\Xdrive\T3000_Building_Automation_System\Database\T3000.db` (SQLite)
- Catalog: `Building` / `Building_ALL`
- Cameras: `CCTV_Camera.building_id` = `Building.ID`

## IDs

| building_id | Site | Cameras |
| --- | --- | --- |
| 1 | Default_Building (shop) | Four sim streams if they stay on this building |
| 2 | 679 Victoria | That job’s cameras. Empty until a camera is added. Do not fall back to 1. |

## Query

```sql
SELECT name, title, angle, rtsp_url, play_url, sort_order
FROM CCTV_Camera
WHERE building_id = ? AND enabled = 1
ORDER BY sort_order;
```

Pass the selected site’s `Building.ID`. Zero rows means an empty grid / placeholder.

- Live tile / iframe: `play_url` (go2rtc `stream.html`)
- RTSP ingest: `rtsp_url`

Shop sim example (only when `building_id = 1` and those rows exist):

- `rtsp://127.0.0.1:9292/sim-north` … `sim-east` `sim-south` `sim-west`
- `http://127.0.0.1:9291/stream.html?src=sim-north` (same ids)

679 Victoria (`building_id = 2`) uses its own rows, even if none yet.
