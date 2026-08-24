-- CCTV tables in the existing T3000.db.
-- Sites are the Building rows (Buildings [Alt-B]).
-- Do NOT alter `users` or `Building` (existing inserts are positional / named).
-- People and plates stay next to users. Ignore zones stay on cameras.

CREATE TABLE IF NOT EXISTS CCTV_Camera (
  id INTEGER PRIMARY KEY,
  building_id INTEGER NOT NULL,
  building_name VARCHAR(50) NOT NULL,
  name TEXT NOT NULL,
  title TEXT,
  angle TEXT,
  rtsp_url TEXT NOT NULL,
  play_url TEXT,
  enabled INTEGER NOT NULL DEFAULT 1,
  sort_order INTEGER NOT NULL DEFAULT 0,
  UNIQUE (building_id, name)
);

CREATE TABLE IF NOT EXISTS CCTV_IgnoreZone (
  id INTEGER PRIMARY KEY,
  camera_id INTEGER NOT NULL REFERENCES CCTV_Camera(id) ON DELETE CASCADE,
  name TEXT,
  polygon_json TEXT NOT NULL,
  enabled INTEGER NOT NULL DEFAULT 1
);

CREATE TABLE IF NOT EXISTS CCTV_Person (
  user_name_login VARCHAR(50) PRIMARY KEY,
  display_name TEXT,
  role TEXT NOT NULL,
  trade TEXT,
  company TEXT,
  license_plate TEXT,
  account_enabled INTEGER NOT NULL DEFAULT 1,
  notes TEXT
);

CREATE TABLE IF NOT EXISTS CCTV_Plate (
  id INTEGER PRIMARY KEY,
  user_name_login VARCHAR(50) NOT NULL,
  plate TEXT NOT NULL,
  vehicle TEXT
);

INSERT OR IGNORE INTO CCTV_Camera (building_id, building_name, name, title, angle, rtsp_url, play_url, enabled, sort_order)
SELECT 1, 'Default_Building', 'sim-north', 'NW looking SE', 'NW corner looking southeast', 'rtsp://127.0.0.1:9292/sim-north', 'http://127.0.0.1:9291/stream.html?src=sim-north', 1, 1
WHERE EXISTS (SELECT 1 FROM Building WHERE ID=1);
INSERT OR IGNORE INTO CCTV_Camera (building_id, building_name, name, title, angle, rtsp_url, play_url, enabled, sort_order)
SELECT 1, 'Default_Building', 'sim-east', 'NE looking SW', 'NE corner looking southwest', 'rtsp://127.0.0.1:9292/sim-east', 'http://127.0.0.1:9291/stream.html?src=sim-east', 1, 2
WHERE EXISTS (SELECT 1 FROM Building WHERE ID=1);
INSERT OR IGNORE INTO CCTV_Camera (building_id, building_name, name, title, angle, rtsp_url, play_url, enabled, sort_order)
SELECT 1, 'Default_Building', 'sim-south', 'SE looking NW', 'SE corner looking northwest', 'rtsp://127.0.0.1:9292/sim-south', 'http://127.0.0.1:9291/stream.html?src=sim-south', 1, 3
WHERE EXISTS (SELECT 1 FROM Building WHERE ID=1);
INSERT OR IGNORE INTO CCTV_Camera (building_id, building_name, name, title, angle, rtsp_url, play_url, enabled, sort_order)
SELECT 1, 'Default_Building', 'sim-west', 'SW looking NE', 'SW corner looking northeast', 'rtsp://127.0.0.1:9292/sim-west', 'http://127.0.0.1:9291/stream.html?src=sim-west', 1, 4
WHERE EXISTS (SELECT 1 FROM Building WHERE ID=1);

-- DVR recorder settings (motion-only). Do not rewrite CCTV_Camera rows.
CREATE TABLE IF NOT EXISTS CCTV_DvrSetting (
  key TEXT PRIMARY KEY,
  value TEXT NOT NULL
);
CREATE TABLE IF NOT EXISTS CCTV_DvrCamera (
  camera_name TEXT PRIMARY KEY,
  enabled INTEGER NOT NULL DEFAULT 1
);
INSERT OR IGNORE INTO CCTV_DvrSetting(key,value) VALUES
 ('storage_path','C:\Xdrive\T3000_DVR'),
 ('motion_only','1'),
 ('preroll_sec','3'),
 ('postroll_sec','8'),
 ('retention_days','14'),
 ('max_gb','100');
