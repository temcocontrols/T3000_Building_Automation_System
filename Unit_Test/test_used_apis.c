#include "harness.h"
#include "isp_constants.h"

#include <string.h>

void isp_set_auto_flash(int on);
int isp_hex_str(const char *text);
int isp_hex4(char *text);
int isp_hex_char(char c);
int isp_hex_line(char *text);
void isp_nibbles_to_bytes(char *nibbles, int count, unsigned char *out);
void isp_ushort_to_hex(unsigned short value, char *out);
int isp_hex_crc(unsigned char *buf, int length);
void isp_product_name(int id, char *out, int out_len);
void isp_firmware_name(int id, char *out, int out_len);
int isp_check_bootloader(int npid, int comport, unsigned reg_11, unsigned reg_14,
                         int *update_value, unsigned char app_version,
                         int must_new, int com_status);

static void expect_name(void (*fn)(int, char *, int), int id, const char *want)
{
    char got[64];
    fn(id, got, (int)sizeof(got));
    EXPECT(strcmp(got, want) == 0);
}

void test_used_apis(void)
{
    char line[8];
    char nibbles[4];
    char hex[8];
    unsigned char bytes[4];
    unsigned char crc_ok[3];
    unsigned char crc_bad[2];
    int update;

    isp_set_auto_flash(0);
    EXPECT(isp_hex_str("A5") == 165);
    EXPECT(isp_hex_str("10") == 16);
    EXPECT(isp_hex_str("ff") == 255);
    EXPECT(isp_hex_str("00") == 0);
    EXPECT(isp_hex_str("G0") == -1);
    isp_set_auto_flash(1);
    EXPECT(isp_hex_str("zz") == -1);
    isp_set_auto_flash(0);

    strcpy(line, "00AB");
    EXPECT(isp_hex4(line) == 0x00AB);
    strcpy(line, "FFFF");
    EXPECT(isp_hex4(line) == 65535);
    strcpy(line, "GG");
    EXPECT(isp_hex4(line) == (int)(unsigned short)-1);
    strcpy(line, "12345");
    EXPECT(isp_hex4(line) == (int)(unsigned short)-1);
    EXPECT(line[0] == 0);

    EXPECT(isp_hex_char('0') == 0);
    EXPECT(isp_hex_char('9') == 9);
    EXPECT(isp_hex_char('a') == 10);
    EXPECT(isp_hex_char('F') == 15);
    EXPECT(isp_hex_char('g') == -1);
    EXPECT(isp_hex_char(' ') == -1);

    strcpy(line, "A5");
    EXPECT(isp_hex_line(line) == 1);
    EXPECT((unsigned char)line[0] == 10);
    EXPECT((unsigned char)line[1] == 5);
    strcpy(line, "AG");
    EXPECT(isp_hex_line(line) == 0);

    nibbles[0] = 0x0A;
    nibbles[1] = 0x05;
    nibbles[2] = 0x01;
    nibbles[3] = 0x00;
    isp_nibbles_to_bytes(nibbles, 4, bytes);
    EXPECT(bytes[0] == 0xA5);
    EXPECT(bytes[1] == 0x10);

    isp_ushort_to_hex(0x00AB, hex);
    EXPECT(strcmp(hex, "AB") == 0);
    isp_ushort_to_hex(0x000A, hex);
    EXPECT(strcmp(hex, "0A") == 0);
    isp_ushort_to_hex(0, hex);
    EXPECT(strcmp(hex, "00") == 0);

    crc_ok[0] = 1;
    crc_ok[1] = 2;
    crc_ok[2] = 253;
    EXPECT(isp_hex_crc(crc_ok, 3) == 1);
    crc_bad[0] = 1;
    crc_bad[1] = 2;
    EXPECT(isp_hex_crc(crc_bad, 2) == 0);
    EXPECT(isp_hex_crc(crc_ok, 0) == 1);

    expect_name(isp_product_name, PM_TSTAT10, "TStat10");
    expect_name(isp_product_name, PM_TSTAT8, "TStat8");
    expect_name(isp_product_name, PM_TSTAT6, "TStat6");
    expect_name(isp_product_name, PM_MINIPANEL, "MiniPanel");
    expect_name(isp_product_name, PM_MINIPANEL_ARM, "MiniPanel");
    expect_name(isp_product_name, STM32_CO2_NET, "CO2NET");
    expect_name(isp_product_name, PM_ESP32_T3_SERIES, "PID88");
    expect_name(isp_product_name, 999, "PID999");

    expect_name(isp_firmware_name, PM_TSTAT10, "PID10");
    expect_name(isp_firmware_name, PM_TSTAT8, "TStat8");
    expect_name(isp_firmware_name, PM_TSTAT6, "TStat6");
    expect_name(isp_firmware_name, PM_MINIPANEL, "MiniPanel");
    expect_name(isp_firmware_name, STM32_PM25, "PM2.5");
    expect_name(isp_firmware_name, PM_ESP32_T3_SERIES, "PID88");

    update = -99;
    EXPECT(isp_check_bootloader(STM32_CO2_NET, 1, 10, 10, &update, 59, 1, 0) == 1);
    EXPECT(update == -99);

    update = -99;
    EXPECT(isp_check_bootloader(PM_TSTAT8, 1, 48, 0, &update, 0, 1, 0) == 1);
    EXPECT(update == 1);
    update = -99;
    EXPECT(isp_check_bootloader(PM_TSTAT8, 1, 49, 0, &update, 0, 1, 0) == 1);
    EXPECT(update == 0);
    update = -99;
    EXPECT(isp_check_bootloader(PM_TSTAT8, 1, 0, 0, &update, 0, 1, 0) == 1);
    EXPECT(update == 0);
    update = -99;
    EXPECT(isp_check_bootloader(PM_TSTAT8, 1, 40, 40, &update, 0, 0, 0) == 1);
    EXPECT(update == 0);

    update = -99;
    EXPECT(isp_check_bootloader(PM_MINIPANEL_ARM, 0, 61, 0, &update, 0, 1, 0) == -1);
    EXPECT(update == 0);
    update = -99;
    EXPECT(isp_check_bootloader(PM_MINIPANEL_ARM, 1, 61, 0, &update, 0, 1, 0) == 1);
    EXPECT(update == 1);
    update = -99;
    EXPECT(isp_check_bootloader(PM_MINIPANEL, 0, 10, 61, &update, 0, 1, 0) == -1);
    EXPECT(update == 0);

    update = -99;
    EXPECT(isp_check_bootloader(PM_TSTAT10, 1, 53, 0, &update, 0, 1, 0) == 1);
    EXPECT(update == 1);
    update = -99;
    EXPECT(isp_check_bootloader(PM_TSTAT10, 1, 54, 0, &update, 0, 1, 0) == 1);
    EXPECT(update == 0);

    update = -99;
    EXPECT(isp_check_bootloader(STM32_CO2_NET, 1, 55, 0, &update, 0, 1, 0) == 1);
    EXPECT(update == 1);
    update = -99;
    EXPECT(isp_check_bootloader(STM32_PRESSURE_RS485, 1, 56, 0, &update, 0, 1, 0) == 1);
    EXPECT(update == 0);
    update = -99;
    EXPECT(isp_check_bootloader(STM32_PM25, 1, 10, 10, &update, 0, 1, 0) == 1);
    EXPECT(update == 1);

    update = -99;
    EXPECT(isp_check_bootloader(PM_TSTAT8, 1, 20, 20, &update, 0, 1, 2) == 1);
    EXPECT(update == -99);
    update = -99;
    EXPECT(isp_check_bootloader(PM_TSTAT6, 1, 10, 10, &update, 0, 1, 0) == 1);
    EXPECT(update == -99);
}
