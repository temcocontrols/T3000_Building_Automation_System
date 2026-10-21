#include "harness.h"

#include <stdio.h>

void test_resume_support(void);
void test_wifi_new_and_old(void);
void test_serial_new_and_old(void);
void test_bootloader_flow(void);
void test_source_lock(void);
void test_used_apis(void);

int main(void)
{
    harness_reset();
    test_resume_support();
    test_wifi_new_and_old();
    test_serial_new_and_old();
    test_bootloader_flow();
    test_source_lock();
    test_used_apis();

    printf("unit tests: %d passed, %d failed\n", harness_passes(), harness_failures());
    return harness_failures() == 0 ? 0 : 1;
}
