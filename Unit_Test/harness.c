#include "harness.h"

#include <stdio.h>

static int g_pass;
static int g_fail;

void harness_reset(void)
{
    g_pass = 0;
    g_fail = 0;
}

void harness_expect(int condition, const char *file, int line, const char *expr)
{
    if (condition) {
        g_pass++;
        return;
    }
    g_fail++;
    fprintf(stderr, "FAIL %s:%d  %s\n", file, line, expr);
}

int harness_failures(void)
{
    return g_fail;
}

int harness_passes(void)
{
    return g_pass;
}
