# Sync dvr-settings.json into T3000.db CCTV_DvrSetting / CCTV_DvrCamera
import json, os, sqlite3, sys

here = os.path.dirname(os.path.abspath(__file__))
settings_path = os.path.join(here, "dvr-settings.json")
db_path = os.path.normpath(os.path.join(here, "..", "..", "Database", "T3000.db"))
if len(sys.argv) > 1:
    settings_path = sys.argv[1]
if len(sys.argv) > 2:
    db_path = sys.argv[2]

obj = json.load(open(settings_path, encoding="utf-8"))
con = sqlite3.connect(db_path)
cur = con.cursor()
cur.executescript(
    """
CREATE TABLE IF NOT EXISTS CCTV_DvrSetting (
  key TEXT PRIMARY KEY,
  value TEXT NOT NULL
);
CREATE TABLE IF NOT EXISTS CCTV_DvrCamera (
  camera_name TEXT PRIMARY KEY,
  enabled INTEGER NOT NULL DEFAULT 1
);
"""
)
pairs = [
    ("storage_path", str(obj.get("storage_path", r"C:\Xdrive\T3000_DVR"))),
    ("motion_only", str(int(obj.get("motion_only", 1)))),
    ("preroll_sec", str(int(obj.get("preroll_sec", 3)))),
    ("postroll_sec", str(int(obj.get("postroll_sec", 8)))),
    ("retention_days", str(int(obj.get("retention_days", 14)))),
    ("max_gb", str(int(obj.get("max_gb", 100)))),
]
for k, v in pairs:
    cur.execute("INSERT OR REPLACE INTO CCTV_DvrSetting(key,value) VALUES (?,?)", (k, v))
cams = obj.get("cameras") or {}
if isinstance(cams, dict):
    for name, en in cams.items():
        cur.execute(
            "INSERT OR REPLACE INTO CCTV_DvrCamera(camera_name, enabled) VALUES (?,?)",
            (str(name), int(en)),
        )
con.commit()
con.close()
print("synced", db_path)
