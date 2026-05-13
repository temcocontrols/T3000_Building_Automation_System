# T3EngineDll (Windows)

This folder builds a **minimal Win32 `T3Engine.dll`** so the **T3000** tree can carry a native binary next to the app (e.g. **Debug** `LoadLibrary` smoke) while sharing headers with **ESP32** via the sibling **`..\T3Engine`** repo.

**Scope today:** shared **product identity layout** (`temco_product_desc.h`) and other **`..\T3Engine\include`** headers — **not** the monotonic clock hook (`t3_monotonic_ms`); that stays out of this DLL until you deliberately add it.

## Prerequisites

- **Visual Studio 2019 or later** with **Desktop development with C++** (MSVC **v143** matches `T3000_VS2019.vcxproj`; retarget if your team uses another toolset).
- **`..\T3Engine\include`** present (clone or submodule **T3Engine** next to this repo).

## Build

1. Open **`T3Engine.vcxproj`**.
2. **Win32** platform.
3. **Debug** or **Release** → **`Debug\T3Engine.dll`** / **`Release\T3Engine.dll`** (gitignored).

## Layout

| File | Role |
|------|------|
| **`T3Engine.vcxproj`** | DLL project; includes **`..\T3Engine\include`**. |
| **`t3_engine_exports.c`** | **`DllMain`**; includes **`temco_product_desc.h`** and a **compile-time size check** so Windows builds stay aligned with the shared descriptor layout (same idea as **winPC** / controller tooling). |
| **`.gitignore`** | **`Debug/`**, **`Release/`**, **`*.dll`**, **`*.pdb`**, etc. |

## Submodule

```powershell
git submodule update --init --recursive
```

**`T3Engine`** should live at **`..\T3Engine`** relative to this folder.

## T3000 deployment

Copy **`T3Engine.dll`** next to **`T3000.exe`** when you want the optional DLL presence check, or wire this project into the solution with post-build copy — same as other native dependencies.
