"""Apply CCTV tables to the existing T3000.db (Building = site). Remove leftover sites.db."""
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

print("Building sites:")
for row in con.execute("SELECT ID, Main_BuildingName, Building_Name, city, street FROM Building"):
    print(" ", row)
print("CCTV_Camera:")
for row in con.execute("SELECT id, building_name, name, title, rtsp_url FROM CCTV_Camera ORDER BY sort_order"):
    print(" ", row)
print("ignore zones", con.execute("SELECT COUNT(*) FROM CCTV_IgnoreZone").fetchone()[0])
print("people", con.execute("SELECT COUNT(*) FROM CCTV_Person").fetchone()[0])
print("users", list(con.execute("SELECT user_name_login FROM users")))

cams = [
    {"name": n, "title": t, "src": src}
    for n, t, src in con.execute(
        "SELECT name, title, rtsp_url FROM CCTV_Camera WHERE enabled=1 ORDER BY sort_order"
    )
]
con.close()

nvr = Path.home() / "AppData" / "Local" / "T3000" / "nvr"
nvr.mkdir(parents=True, exist_ok=True)
(nvr / "cameras.json").write_text(json.dumps({"cameras": cams}, indent=2), encoding="utf-8")
print("wrote", nvr / "cameras.json")

for leftover in (root / "Database" / "sites.db", root / "Database" / "sites.sql", root / "Database" / "cctv-users.sql"):
    if leftover.exists():
        leftover.unlink()
        print("removed", leftover)
