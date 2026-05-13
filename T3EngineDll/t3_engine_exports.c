#include <windows.h>
#include "temco_product_desc.h"

/* Compile-time lockstep with firmware / shared `temco_product_desc` layout (winPC + controllers). */
typedef char temco_product_desc_size_check[(sizeof(temco_product_desc_t) == TEMCO_PRODUCT_DESC_SIZE) ? 1 : -1];

BOOL WINAPI DllMain(HINSTANCE inst, DWORD reason, LPVOID reserved)
{
    (void)inst;
    (void)reserved;
    switch (reason) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
    default:
        break;
    }
    return TRUE;
}
