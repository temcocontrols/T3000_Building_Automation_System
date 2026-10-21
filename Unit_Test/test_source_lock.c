#include "harness.h"
#include "isp_constants.h"
#include "test_paths.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *read_file(const char *path)
{
    FILE *f;
    long n;
    char *buf;

    f = fopen(path, "rb");
    if (f == NULL) {
        fprintf(stderr, "cannot open %s\n", path);
        return NULL;
    }
    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return NULL;
    }
    n = ftell(f);
    if (n < 0) {
        fclose(f);
        return NULL;
    }
    if (fseek(f, 0, SEEK_SET) != 0) {
        fclose(f);
        return NULL;
    }
    buf = (char *)malloc((size_t)n + 1u);
    if (buf == NULL) {
        fclose(f);
        return NULL;
    }
    if (fread(buf, 1, (size_t)n, f) != (size_t)n) {
        free(buf);
        fclose(f);
        return NULL;
    }
    buf[n] = '\0';
    fclose(f);
    return buf;
}

static void expect_has(const char *path, const char *needle)
{
    char *text = read_file(path);
    if (text == NULL) {
        EXPECT(0);
        return;
    }
    if (strstr(text, needle) == NULL)
        fprintf(stderr, "missing in %s: %s\n", path, needle);
    EXPECT(strstr(text, needle) != NULL);
    free(text);
}

void test_source_lock(void)
{
    expect_has(T3000_ROOT "/ISP/global_function.cpp", "bool isp_device_supports_resume");
    expect_has(T3000_ROOT "/ISP/global_function.cpp", "bootloader_version >= ISP_RESUME_BOOTLOADER_ALL");
    expect_has(T3000_ROOT "/ISP/global_function.cpp", "bootloader_version == ISP_RESUME_BOOTLOADER_LIMITED");
    expect_has(T3000_ROOT "/ISP/Global_Struct.h", "ISP_RESUME_BOOTLOADER_ALL");
    expect_has(T3000_ROOT "/ISP/Global_Struct.h", "ISP_RESUME_PACKET_BYTES");
    expect_has(T3000_ROOT "/ISP/TFTPServer.cpp", "saved_bytes & ~4095");
    expect_has(T3000_ROOT "/ISP/TFTPServer.cpp", "continue_flash_count = (resume_bytes / 512) + 1");
    expect_has(T3000_ROOT "/ISP/TFTPServer.cpp", "uses a full update");
    expect_has(T3000_ROOT "/ISP/TFTPServer.cpp", "ISP_CMD_CLEAR_SAVED_UPDATE");
    expect_has(T3000_ROOT "/ISP/TFTPServer.cpp", "MD5 mismatch: resuming saved update");
    expect_has(T3000_ROOT "/ISP/TFTPServer.cpp", "if (package_number < continue_flash_count)");
    expect_has(T3000_ROOT "/ISP/ComWriter.cpp", "Could not read registers 1991-1996. Starting from the beginning.");
    expect_has(T3000_ROOT "/ISP/ComWriter.cpp", "mudbus_write_one(m_ID, 16, 0x7f)");
    expect_has(T3000_ROOT "/ISP/ComWriter.cpp", "mudbus_write_one(m_ID, 16, 0x3f)");
    expect_has(T3000_ROOT "/ISP/ComWriter.cpp", "mudbus_write_one(m_ID, 16, 0x1f)");
    expect_has(T3000_ROOT "/ISP/ComWriter.cpp", "ReadResumeByteOffset");
#if HAVE_BOOTLOADER
    expect_has(BOOT_ROOT "/main/app_main.c", "OTA fresh begin");
    expect_has(BOOT_ROOT "/main/app_main.c", "OTA resume: raw write from offset=%lu");
    expect_has(BOOT_ROOT "/main/app_main.c", "SKIP_FIRMWARE_HMAC_VALIDATION 1");
    expect_has(BOOT_ROOT "/main/temco_dfu.c", "bytes & ~4095u");
    expect_has(BOOT_ROOT "/main/temco_dfu.c", "TFTP resume at byte");
    expect_has(BOOT_ROOT "/main/app_config.h", "OTA_PACKET_SIZE      128u");
    expect_has(BOOT_ROOT "/main/modbus.h", "ERASE_USED_FLASH");
#endif

    EXPECT(OTA_PACKET_SIZE == 128u);
    EXPECT(ISP_CMD_CLEAR_SAVED_UPDATE == 0x33);
    EXPECT(ISP_STATUS_IN_PROGRESS_1F == 0x1F);
    EXPECT(ISP_REG_UPDATE_STATUS == 16);
    EXPECT(ISP_REG_RESUME_COUNT == 1991);
    EXPECT(ISP_REG_MD5_START == 1993);
    EXPECT(ISP_REG_FW_SIZE_LO == 1998);
}
