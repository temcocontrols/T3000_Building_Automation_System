-- T3000 CCTV persistence.
-- Do NOT alter `users` (ManageAccountDlg inserts two positional values).
-- People / plates / logins live next to users. Ignore zones live on cameras.

CREATE TABLE IF NOT EXISTS CCTV_Camera (
  id INTEGER PRIMARY KEY,
  name TEXT NOT NULL UNIQUE,
  title TEXT,
  angle TEXT,
  rtsp_url TEXT NOT NULL,
  play_url TEXT,
  enabled INTEGER NOT NULL DEFAULT 1,
  sort_order INTEGER NOT NULL DEFAULT 0
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

INSERT OR IGNORE INTO CCTV_Camera (name, title, angle, rtsp_url, play_url, enabled, sort_order) VALUES
 ('sim-north', 'NW looking SE', 'NW corner looking southeast', 'rtsp://127.0.0.1:9292/sim-north', 'http://127.0.0.1:9291/stream.html?src=sim-north', 1, 1),
 ('sim-east',  'NE looking SW', 'NE corner looking southwest', 'rtsp://127.0.0.1:9292/sim-east',  'http://127.0.0.1:9291/stream.html?src=sim-east',  1, 2),
 ('sim-south', 'SE looking NW', 'SE corner looking northwest', 'rtsp://127.0.0.1:9292/sim-south', 'http://127.0.0.1:9291/stream.html?src=sim-south', 1, 3),
 ('sim-west',  'SW looking NE', 'SW corner looking northeast', 'rtsp://127.0.0.1:9292/sim-west',  'http://127.0.0.1:9291/stream.html?src=sim-west',  1, 4);
