"""Render the T3000 CCTV daily path / hotspot demo map."""
import json
from pathlib import Path
from PIL import Image, ImageDraw, ImageFont, ImageFilter

ROOT = Path(r"C:\Xdrive\T3000_Building_Automation_System")
DATA = json.loads((ROOT / "T3000" / "ResourceFile" / "nvr" / "hotspot-demo.json").read_text(encoding="utf-8"))
OUT = ROOT / "Documentation" / "cctv" / "t3000-cctv" / "images" / "10-hotspot-map.png"

W, H = 1400, 900
PAD = 70
MAP = (PAD, PAD + 36, W - 280, H - 40)

def xy(nx, ny):
    x0, y0, x1, y1 = MAP
    return (x0 + nx * (x1 - x0), y0 + ny * (y1 - y0))

def font(size):
    for name in ("segoeui.ttf", "arial.ttf", "C:\\Windows\\Fonts\\segoeui.ttf", "C:\\Windows\\Fonts\\arial.ttf"):
        try:
            return ImageFont.truetype(name, size)
        except OSError:
            continue
    return ImageFont.load_default()

img = Image.new("RGB", (W, H), (10, 18, 32))
draw = ImageDraw.Draw(img, "RGBA")
f16, f14, f20, f28 = font(16), font(14), font(20), font(28)

# lot
draw.rectangle(MAP, fill=(46, 42, 34), outline=(90, 78, 58), width=3)
# wooden fence hash
x0, y0, x1, y1 = MAP
for i in range(int(x0), int(x1), 18):
    draw.line([(i, y0), (i + 8, y0)], fill=(140, 96, 48), width=4)
    draw.line([(i, y1), (i + 8, y1)], fill=(140, 96, 48), width=4)
for i in range(int(y0), int(y1), 18):
    draw.line([(x0, i), (x0, i + 8)], fill=(140, 96, 48), width=4)
    draw.line([(x1, i), (x1, i + 8)], fill=(140, 96, 48), width=4)

# ICF foundation
icf = [xy(0.34, 0.34), xy(0.66, 0.34), xy(0.66, 0.66), xy(0.34, 0.66)]
draw.polygon(icf, fill=(230, 230, 222), outline=(190, 190, 182))
# rebar dots
ix0, iy0 = xy(0.38, 0.38)
ix1, iy1 = xy(0.62, 0.62)
x = ix0
while x < ix1:
    y = iy0
    while y < iy1:
        draw.ellipse([x - 2, y - 2, x + 2, y + 2], fill=(90, 40, 30))
        y += 16
    x += 16
draw.text((ix0, iy0 - 22), "ICF + rebar", fill=(40, 40, 40), font=f14)

# excavator
ex, ey = xy(0.78, 0.42)
draw.rounded_rectangle([ex - 28, ey - 14, ex + 36, ey + 18], 4, fill=(232, 110, 28))
draw.polygon([(ex + 36, ey - 4), (ex + 70, ey - 28), (ex + 74, ey - 20), (ex + 40, ey + 6)], fill=(200, 90, 20))
draw.text((ex - 20, ey + 22), "Hitachi", fill=(255, 200, 140), font=f14)

# trees / houses suggestion north of lot
for hx, label in ((0.25, ""), (0.45, "houses"), (0.65, "")):
    px, py = xy(hx, 0.04)
    draw.rectangle([px - 16, py - 22, px + 16, py + 6], fill=(70, 78, 88))
    draw.polygon([(px - 22, py - 22), (px, py - 40), (px + 22, py - 22)], fill=(48, 72, 48))
draw.text((xy(0.40, 0.005)[0], MAP[1] - 28), "street / houses", fill=(160, 170, 180), font=f14)

# future walls
for wall in DATA.get("future_walls", []):
    pts = [xy(*p) for p in wall]
    draw.line(pts, fill=(180, 200, 220, 140), width=2)
draw.text((xy(0.36, 0.31)[0], xy(0.36, 0.31)[1] - 18), "future walls", fill=(170, 190, 210), font=f14)

# hotspots
heat = Image.new("RGBA", (W, H), (0, 0, 0, 0))
hd = ImageDraw.Draw(heat)
for hspot in DATA["hotspots"]:
    cx, cy = xy(hspot["x"], hspot["y"])
    r = 28 + 40 * float(hspot.get("w", 0.7))
    hd.ellipse([cx - r, cy - r, cx + r, cy + r], fill=(255, 70, 40, 70))
    hd.ellipse([cx - r * 0.55, cy - r * 0.55, cx + r * 0.55, cy + r * 0.55], fill=(255, 180, 40, 90))
heat = heat.filter(ImageFilter.GaussianBlur(12))
img = Image.alpha_composite(img.convert("RGBA"), heat).convert("RGB")
draw = ImageDraw.Draw(img, "RGBA")

# path
pts = [xy(a, b) for a, b in DATA["path"]]
draw.line(pts, fill=(80, 220, 255), width=5)
for p in pts:
    draw.ellipse([p[0] - 3, p[1] - 3, p[0] + 3, p[1] + 3], fill=(220, 250, 255))

# cameras + FOV
fov = {"SE": (1, 1), "SW": (-1, 1), "NW": (-1, -1), "NE": (1, -1)}
for cam in DATA["cameras"]:
    cx, cy = xy(cam["x"], cam["y"])
    dx, dy = fov.get(cam["facing"], (1, 1))
    draw.polygon([(cx, cy), (cx + dx * 70, cy + dy * 18), (cx + dx * 18, cy + dy * 70)], fill=(30, 120, 200, 50), outline=(90, 180, 255, 160))
    draw.ellipse([cx - 8, cy - 8, cx + 8, cy + 8], fill=(20, 90, 170), outline=(180, 220, 255))
    draw.text((cx + 12, cy - 18), cam["id"], fill=(200, 230, 255), font=f14)

# YOLO sightings
for s in DATA["sightings"]:
    sx, sy = xy(s["x"], s["y"])
    draw.rectangle([sx - 16, sy - 28, sx + 16, sy + 10], outline=(80, 255, 120), width=2)
    tag = f"{s['t']}  {s['camera']}"
    draw.rectangle([sx + 18, sy - 32, sx + 18 + 7 * len(tag), sy - 12], fill=(8, 20, 12, 200))
    draw.text((sx + 20, sy - 32), tag, fill=(140, 255, 170), font=f14)

# chrome
draw.text((24, 16), "T3000 CCTV  ·  daily path / hotspot", fill=(240, 246, 255), font=f28)
draw.text((24, 50), f"{DATA['site']}   {DATA['day']}   {DATA['person']['name']}   open site, no walls", fill=(160, 176, 196), font=f16)

# legend
lx, ly = W - 250, 90
draw.rounded_rectangle([lx - 16, ly - 16, W - 24, H - 40], 8, fill=(16, 28, 46), outline=(50, 70, 96))
draw.text((lx, ly), "Legend", fill=(230, 236, 245), font=f20)
items = [
    ((20, 90, 170), "Camera + FOV"),
    ((80, 220, 255), "Daily path"),
    ((255, 120, 50), "Hotspot (linger)"),
    ((80, 255, 120), "YOLO box / time"),
    ((180, 200, 220), "Future walls"),
    ((230, 230, 222), "ICF + rebar"),
    ((232, 110, 28), "Excavator"),
]
yy = ly + 40
for color, label in items:
    draw.rectangle([lx, yy, lx + 18, yy + 12], fill=color)
    draw.text((lx + 28, yy - 2), label, fill=(200, 210, 222), font=f14)
    yy += 28
draw.text((lx, yy + 10), "Map grows as walls go up.\nSame four sim cameras\n(building_id=1 shop sim).", fill=(150, 164, 184), font=f14)

OUT.parent.mkdir(parents=True, exist_ok=True)
img.save(OUT, "PNG")
print("wrote", OUT, OUT.stat().st_size)
