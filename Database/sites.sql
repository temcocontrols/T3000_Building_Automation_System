-- New sites database. Cameras and ignore zones live here.
-- People, plates, and logins stay in Database/T3000.db (users).

CREATE TABLE IF NOT EXISTS Site (
  id INTEGER PRIMARY KEY,
  name TEXT NOT NULL,
  slug TEXT NOT NULL UNIQUE,
  address TEXT,
  notes TEXT
);

CREATE TABLE IF NOT EXISTS Camera (
  id INTEGER PRIMARY KEY,
  site_id INTEGER NOT NULL REFERENCES Site(id) ON DELETE CASCADE,
  name TEXT NOT NULL,
  title TEXT,
  angle TEXT,
  rtsp_url TEXT NOT NULL,
  play_url TEXT,
  enabled INTEGER NOT NULL DEFAULT 1,
  sort_order INTEGER NOT NULL DEFAULT 0,
  UNIQUE (site_id, name)
);

CREATE TABLE IF NOT EXISTS IgnoreZone (
  id INTEGER PRIMARY KEY,
  camera_id INTEGER NOT NULL REFERENCES Camera(id) ON DELETE CASCADE,
  name TEXT,
  polygon_json TEXT NOT NULL,
  enabled INTEGER NOT NULL DEFAULT 1
);

INSERT OR IGNORE INTO Site (id, name, slug, address, notes) VALUES
 (1, 'Victoria ICF', 'victoria-icf', NULL, 'White ICF + rebar lot. Four inward cameras. Add more as walls go up.');

INSERT OR IGNORE INTO Camera (site_id, name, title, angle, rtsp_url, play_url, enabled, sort_order) VALUES
 (1, 'sim-north', 'NW looking SE', 'NW corner looking southeast', 'rtsp://127.0.0.1:9292/sim-north', 'http://127.0.0.1:9291/stream.html?src=sim-north', 1, 1),
 (1, 'sim-east',  'NE looking SW', 'NE corner looking southwest', 'rtsp://127.0.0.1:9292/sim-east',  'http://127.0.0.1:9291/stream.html?src=sim-east',  1, 2),
 (1, 'sim-south', 'SE looking NW', 'SE corner looking northwest', 'rtsp://127.0.0.1:9292/sim-south', 'http://127.0.0.1:9291/stream.html?src=sim-south', 1, 3),
 (1, 'sim-west',  'SW looking NE', 'SW corner looking northeast', 'rtsp://127.0.0.1:9292/sim-west',  'http://127.0.0.1:9291/stream.html?src=sim-west',  1, 4);
