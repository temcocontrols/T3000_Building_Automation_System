"""Apply Database/cctv.sql to T3000.db and seed cameras.json for the T3000 CCTV view."""
import json
import sqlite3
from pathlib import Path

root = Path(r"C:\Xdrive\T3000_Building_Automation_System")
db = root / "Database" / "T3000.db"
sql = (root / "Database" / "cctv.sql").read_text(encoding="utf-8")
con = sqlite3.connect(str(db))
con.execute("PRAGMA foreign_keys = ON")
con.executescript(sql)
con.commit()

print("CCTV_Camera:")
for row in con.execute("SELECT id, name, title, rtsp_url, enabled FROM CCTV_Camera ORDER BY sort_order"):
    print(" ", row)
print("CCTV_IgnoreZone count", con.execute("SELECT COUNT(*) FROM CCTV_IgnoreZone").fetchone()[0])
print("CCTV_Person count", con.execute("SELECT COUNT(*) FROM CCTV_Person").fetchone()[0])
print("users still", list(con.execute("SELECT user_name_login FROM users")))

cams = []
for name, title, src in con.execute(
    "SELECT name, title, rtsp_url FROM CCTV_Camera WHERE enabled=1 ORDER BY sort_order"
):
    cams.append({"name": name, "title": title, "src": src})
con.close()

nvr = Path.home() / "AppData" / "Local" / "T3000" / "nvr"
nvr.mkdir(parents=True, exist_ok=True)
(nvr / "www").mkdir(exist_ok=True)
(nvr / "recordings").mkdir(exist_ok=True)
payload = {"cameras": cams}
(nvr / "cameras.json").write_text(json.dumps(payload, indent=2), encoding="utf-8")
print("wrote", nvr / "cameras.json")
print(json.dumps(payload, indent=2))
