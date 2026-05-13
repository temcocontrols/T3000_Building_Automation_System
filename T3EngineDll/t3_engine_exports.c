/*
 * T3Engine.dll — exports visible to T3000 (and other Win32 hosts) via GetProcAddress.
 * Keep names stable: the MFC app resolves T3Engine_MonotonicMs by string at runtime.
 */
#include "t3_engine.h"

#include <stdint.h>

/* Monotonic milliseconds for timing hooks in the shared core; implemented in t3_clock_win.c. */
__declspec(dllexport) uint32_t __cdecl T3Engine_MonotonicMs(void)
{
    return t3_monotonic_ms();
}
