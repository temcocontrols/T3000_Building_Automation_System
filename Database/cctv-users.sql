-- People / plates for CCTV live next to users in T3000.db.
-- Do NOT alter `users` (ManageAccountDlg inserts two positional values).
-- Cameras and ignore zones are in Database/sites.db.

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
