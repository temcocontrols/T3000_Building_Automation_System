/*
 * Windows implementation of t3_engine.h — monotonic clock for T3Engine.
 *
 * Prefer QueryPerformanceCounter for a monotonic, high-resolution counter.
 * Fall back to GetTickCount64 if QPC is unavailable (rare), still monotonic
 * for practical scan intervals. Return type matches the portable API (uint32_t);
 * wrap behavior is identical to the POSIX smoke host build.
 */
#include "t3_engine.h"

#include <stdint.h>
#include <windows.h>

uint32_t t3_monotonic_ms(void)
{
    static LARGE_INTEGER s_freq;
    static int s_inited;
    LARGE_INTEGER counter;

    if (!s_inited) {
        if (!QueryPerformanceFrequency(&s_freq) || s_freq.QuadPart == 0)
            return (uint32_t)GetTickCount64();
        s_inited = 1;
    }
    if (!QueryPerformanceCounter(&counter))
        return (uint32_t)GetTickCount64();

    return (uint32_t)((counter.QuadPart * 1000ULL) / (uint64_t)s_freq.QuadPart);
}
