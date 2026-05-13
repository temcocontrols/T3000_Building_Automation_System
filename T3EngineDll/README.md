# T3Engine.dll (T3000 integration)

This folder is linked from the repository **[README](../README.md)** (section *T3Engine*) and **[README_Build](../README_Build.md)** (*T3Engine native DLL*).

Native **Win32 DLL** that builds the **T3Engine** portable core with **Windows-specific** implementations of the host hooks in `include/t3_engine.h` (from the [T3Engine](https://github.com/temcocontrols/T3_Engine) tree).

## Layout

| File | Role |
|------|------|
| `T3Engine.vcxproj` | MSVC project: **C17**, **Win32**, output under `T3000 Output\debug\` / `release\`, include path `..\T3Engine\include`. |
| `t3_clock_win.c` | `t3_monotonic_ms()` using **QueryPerformanceCounter**, fallback **GetTickCount64**. |
| `t3_engine_exports.c` | DLL exports (e.g. `T3Engine_MonotonicMs`) resolved by T3000 via `GetProcAddress`. |

## Runtime (current bring-up)

`T3000.exe` loads **`T3Engine.dll`** from the same directory as the executable (`g_strExePth`). Early in `CT3000App::InitInstance`, the app **smoke-loads** the DLL and calls **`T3Engine_MonotonicMs`** once so the export path is verified. See comments in `T3000\T3000.cpp` around that block; flip `#if 1` to `#if 0` to disable during development.

## PR notes (for GitHub Desktop / reviewers)

- **Submodule:** `T3Engine\` is a **git submodule** (see `.gitmodules`). Fresh clones need `git submodule update --init --recursive` before building the DLL. The parent repo stores a **pinned commit** of T3_Engine; to move forward, `cd T3Engine`, `git fetch` / `git checkout <branch>`, test, then in the parent `git add T3Engine` and commit the new gitlink.
- **Header rename:** Engine public header is **`t3_engine.h`** (not `t3_platform.h`). All DLL sources include `t3_engine.h`.
- **Separate PRs:** Do **not** mix unrelated **.NET `TargetFrameworkVersion`** or **COM (`RegisterForComInterop`)** changes with T3Engine integration; those belong in their own PRs.
- **BACnet reuse:** T3000 already has substantial BACnet-related code. When extending the engine, prefer **shared behavior and tests** where the language and layer match; see **`docs/NEXT-STEPS.md`** in the T3Engine repo for the full roadmap and architecture notes.

## Planned next steps

1. Add **T3Engine** and **T3EngineDll** to the solution in a way every developer builds the DLL before F5 (project dependencies).
2. Grow **exports** deliberately (naming + `.def` if needed) as more core APIs are exposed to MFC.
3. Wire **ESP-IDF** firmware to the **same** `t3_engine.h` contract on the controller side.

Canonical planning doc (current integration branch): **[NEXT-STEPS.md on `docs/smoke-ci-and-team-readme`](https://github.com/temcocontrols/T3_Engine/blob/docs/smoke-ci-and-team-readme/docs/NEXT-STEPS.md)** — after merge to `main`, use the `main` URL instead.
