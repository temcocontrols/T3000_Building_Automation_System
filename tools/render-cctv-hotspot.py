"""Photo-derived hotspot map: 2x2 of real sim stills, path across cameras."""
from pathlib import Path
from PIL import Image, ImageDraw, ImageFont, ImageFilter

FRAMES = Path(r"C:\Users\mauri\AppData\Local\Temp\cctv-frames")
OUT = Path(r"C:\Xdrive\T3000_Building_Automation_System\Documentation\cctv\t3000-cctv\images\10-hotspot-map.png")

def font(size):
    for p in (r"C:\Windows\Fonts\segoeui.ttf", r"C:\Windows\Fonts\arial.ttf"):
        try:
            return ImageFont.truetype(p, size)
        except OSError:
            pass
    return ImageFont.load_default()

order = [
    ("sim-north.jpg", "CAM NW  looking SE", "sim-north"),
    ("sim-east.jpg",  "CAM NE  looking SW", "sim-east"),
    ("sim-west.jpg",  "CAM SW  looking NE", "sim-west"),
    ("sim-south.jpg", "CAM SE  looking NW", "sim-south"),
]
stills = [Image.open(FRAMES / n).convert("RGB") for n, _, _ in order]
# common tile size
tw, th = 640, 360
stills = [s.resize((tw, th), Image.Resampling.LANCZOS) for s in stills]

pad, gap, header = 16, 8, 64
W = pad * 2 + tw * 2 + gap
H = header + pad + th * 2 + gap + 36
img = Image.new("RGB", (W, H), (8, 12, 18))
draw = ImageDraw.Draw(img, "RGBA")
f13, f16, f22 = font(13), font(16), font(22)

pos = {
    "sim-north": (pad, header),
    "sim-east":  (pad + tw + gap, header),
    "sim-west":  (pad, header + th + gap),
    "sim-south": (pad + tw + gap, header + th + gap),
}
for still, (fn, title, key) in zip(stills, order):
    x, y = pos[key]
    img.paste(still, (x, y))
    draw.rectangle([x, y, x + tw, y + 22], fill=(0, 0, 0, 160))
    draw.text((x + 8, y + 3), title, fill=(230, 236, 242), font=f13)

# Path of one person crossing the four inward views (pixel coords in each tile, then offset)
# Chosen from the stills: workers walk the ICF edge in each frame.
crossings = [
    # (cam, x_in_tile, y_in_tile, time)
    ("sim-west",  290, 210, "07:12"),
    ("sim-south", 180, 200, "09:40"),
    ("sim-east",  220, 190, "11:05"),
    ("sim-north", 360, 200, "14:20"),
    ("sim-west",  340, 180, "16:05"),
]

def abs_xy(cam, x, y):
    ox, oy = pos[cam]
    return (ox + x, oy + y)

pts = [abs_xy(c, x, y) for c, x, y, _ in crossings]
# linger blobs
heat = Image.new("RGBA", (W, H), (0, 0, 0, 0))
hd = ImageDraw.Draw(heat)
for p in pts:
    hd.ellipse([p[0] - 36, p[1] - 36, p[0] + 36, p[1] + 36], fill=(255, 90, 30, 80))
heat = heat.filter(ImageFilter.GaussianBlur(8))
img = Image.alpha_composite(img.convert("RGBA"), heat).convert("RGB")
draw = ImageDraw.Draw(img, "RGBA")

draw.line(pts, fill=(40, 220, 255), width=4)
for (cam, x, y, t), p in zip(crossings, pts):
    draw.rectangle([p[0] - 14, p[1] - 28, p[0] + 14, p[1] + 8], outline=(90, 255, 130), width=2)
    draw.rectangle([p[0] + 16, p[1] - 28, p[0] + 16 + 7 * len(t + cam), p[1] - 10], fill=(0, 0, 0, 170))
    draw.text((p[0] + 18, p[1] - 28), f"{t}  {cam}", fill=(170, 255, 190), font=f13)

draw.text((pad, 12), "T3000 CCTV  ·  daily path / hotspot", fill=(240, 246, 255), font=f22)
draw.text((pad, 38), "Derived from the four inward sim stills (same lot as the site photos). Person A  ·  2026-08-22  ·  open site, ICF + rebar", fill=(170, 182, 196), font=f16)
draw.text((pad, H - 28), "Green box = YOLO sighting in that camera. Cyan = same person across cameras. Orange = linger.", fill=(140, 154, 168), font=f13)

OUT.parent.mkdir(parents=True, exist_ok=True)
img.save(OUT, "PNG")
print("wrote", OUT, OUT.stat().st_size)
