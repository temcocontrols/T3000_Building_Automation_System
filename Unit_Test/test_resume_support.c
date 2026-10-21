#include "harness.h"
#include "firmware_flow.h"
#include "isp_constants.h"

#include <stdio.h>

static void expect_support(unsigned product, unsigned boot, unsigned mini, int expect)
{
    int got = isp_resume_supported(product, boot, mini);
    if (got != expect) {
        fprintf(stderr, "  product=%u boot=%u mini=%u got=%d expect=%d\n",
                product, boot, mini, got, expect);
    }
    EXPECT(got == expect);
}

void test_resume_support(void)
{
    expect_support(PM_ESP32_T3_SERIES, 48, 0, 1);
    expect_support(PM_ESP32_T3_SERIES, 49, 1, 1);
    expect_support(PM_ESP32_T3_SERIES, 54, ISP_MINI_HUB, 1);
    expect_support(PM_ESP32_T3_SERIES, 100, 0, 1);
    expect_support(PM_ESP32_T3_SERIES, ISP_RESUME_BOOTLOADER_ALL, 0, 1);

    expect_support(PM_TSTAT10, 47, 0, 1);
    expect_support(PM_ESP32_T3_SERIES, 47, ISP_MINI_TSTAT10, 1);
    expect_support(PM_ESP32_T3_SERIES, 47, ISP_MINI_HUB, 1);
    expect_support(PM_ESP32_T3_SERIES, 47, ISP_MINI_TSTAT11, 1);
    expect_support(1, 47, ISP_MINI_HUB, 1);

    expect_support(PM_ESP32_T3_SERIES, 47, 0, 0);
    expect_support(PM_ESP32_T3_SERIES, 47, 1, 0);
    expect_support(PM_ESP32_T3_SERIES, 47, 88, 0);
    expect_support(PM_TSTAT10, 46, 0, 0);
    expect_support(PM_ESP32_T3_SERIES, 46, ISP_MINI_HUB, 0);
    expect_support(PM_ESP32_T3_SERIES, 26, ISP_MINI_HUB, 0);
    expect_support(PM_ESP32_T3_SERIES, 0, ISP_MINI_HUB, 0);
    expect_support(6, 47, 0, 0);
    expect_support(6, 99, ISP_MINI_HUB, 0);
    expect_support(PM_TSTAT10, 48, 0, 0);
    expect_support(10, 48, ISP_MINI_TSTAT10, 0);
    expect_support(0, 0, 0, 0);
    expect_support(PM_ESP32_T3_SERIES, 47, 255, 0);

    EXPECT(ISP_RESUME_BOOTLOADER_ALL == 48);
    EXPECT(ISP_RESUME_BOOTLOADER_LIMITED == 47);
    EXPECT(ISP_RESUME_PACKET_BYTES == 128);
    EXPECT(PM_ESP32_T3_SERIES == 88);
    EXPECT(PM_TSTAT10 == 10);
    EXPECT(ISP_MINI_HUB == 30);
    EXPECT(ISP_MINI_TSTAT10 == 9);
    EXPECT(ISP_MINI_TSTAT11 == 31);
}
