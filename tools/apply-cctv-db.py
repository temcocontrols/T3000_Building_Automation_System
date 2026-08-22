"""Apply sites.db + user-side CCTV people tables. Drop camera tables from T3000.db."""
import json
import sqlite3
from pathlib import Path

root = Path(r"C:\Xdrive\T3000_Building_Automation_System")
t3000 = root / "Database" / "T3000.db"
sites = root / "Database" / "sites.db"

users_sql = (root / "Database" / "cctv-users.sql").read_text(encoding="utf-8")
sites_sql = (root / "Database" / "sites.sql").read_text(encoding="utf-8")

scon = sqlite3.connect(str(sites))
scon.execute("PRAGMA foreign_keys = ON")
scon.executescript(sites_sql)
scon.commit()
print("sites.db")
for row in scon.execute("SELECT id, name, slug FROM Site"):
    print(" site", row)
for row in scon.execute("SELECT id, name, title, rtsp_url FROM Camera ORDER BY sort_order"):
    print(" cam", row)
print(" ignore zones", scon.execute("SELECT COUNT(*) FROM IgnoreZone").fetchone()[0])

cams = [
    {"name": n, "title": t, "src": src}
    for n, t, src in scon.execute(
        "SELECT name, title, rtsp_url FROM Camera WHERE enabled=1 ORDER BY sort_order"
    )
]
scon.close()

ucon = sqlite3.connect(str(t3000))
ucon.execute("PRAGMA foreign_keys = ON")
ucon.executescript(users_sql)
# cameras do not belong in the users db
ucon.execute("DROP TABLE IF EXISTS CCTV_IgnoreZone")
ucon.execute("DROP TABLE IF EXISTS CCTV_Camera")
ucon.commit()
print("T3000.db people tables", [r[0] for r in ucon.execute(
    "SELECT name FROM sqlite_master WHERE type='table' AND name LIKE 'CCTV_%' ORDER BY name"
)])
print("users", list(ucon.execute("SELECT user_name_login FROM users")))
ucon.close()

nvr = Path.home() / "AppData" / "Local" / "T3000" / "nvr"
nvr.mkdir(parents=True, exist_ok=True)
(nvr / "cameras.json").write_text(json.dumps({"cameras": cams}, indent=2), encoding="utf-8")
print("wrote", nvr / "cameras.json")
