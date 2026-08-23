# Daily path / hotspot map

The map is the **four inward camera stills**, not a cartoon lot. Path and YOLO boxes sit on those frames. Same ICF + rebar lot as the site photos.

![Hotspot map from sim stills](t3000-cctv/images/10-hotspot-map.png)

## Sources

- Stills from `T3000/sidecar/sim/{north,east,south,west}.mp4` (CAM NW/NE/SE/SW)
- Site photos in `C:\Xdrive\679Victoria\*.jpg` match those frames
- 2D registration: `C:\Xdrive\679Victoria\Rev10\2D\679Victoria_Architectural_V6_Nov02.pdf` site plan
- 3D cameras: `C:\Xdrive\679Victoria\Rev10\3D\Camera.SLDASM`, `Camera2.SLDASM`

## Files

- UI: `T3000/ResourceFile/nvr/hotspot.html`
- Demo path: `T3000/ResourceFile/nvr/hotspot-demo.json`
- Render: `tools/render-cctv-hotspot.py`
- Image: `Documentation/cctv/t3000-cctv/images/10-hotspot-map.png`
