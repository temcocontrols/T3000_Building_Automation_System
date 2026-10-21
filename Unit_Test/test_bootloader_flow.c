#include "harness.h"
#include "firmware_flow.h"
#include "isp_constants.h"

#include <string.h>

static void make_image(uint8_t *image, unsigned len)
{
    unsigned i;
    for (i = 0; i < len; i++)
        image[i] = (uint8_t)((i * 3u) + 1u);
    image[0] = 0xE9;
}

static void test_rs485_fresh_then_resume(void)
{
    static device_t dev;
    uint8_t image[2048];
    uint8_t stamp_before[1024];
    unsigned offset;
    wifi_input_t wifi;
    wifi_plan_t wifi_out;
    serial_input_t serial;
    serial_plan_t serial_out;

    make_image(image, sizeof(image));
    dev_reset(&dev);
    EXPECT(dev_status(&dev, 0x7F) == 0);
    EXPECT(dev.ignored_status == 1);
    EXPECT(dev.erase_count == 0);
    EXPECT(dev.com_update == 0);

    EXPECT(dev_status(&dev, 0x3F) == 0);
    EXPECT(dev.erase_count == 1);
    EXPECT(dev.ota_begin_count == 1);
    EXPECT(dev.block_no == 1);
    EXPECT(dev_status(&dev, 0x3F) == 0);
    EXPECT(dev.erase_count == 1);
    EXPECT(dev.ota_begin_count == 1);

    EXPECT(dev_status(&dev, ISP_STATUS_IN_PROGRESS_1F) == 0);
    EXPECT(dev.use_raw == 0);
    EXPECT(dev.run_begin == 1);
    EXPECT(dev.softap_suspend == 1);
    EXPECT(dev_status(&dev, ISP_STATUS_IN_PROGRESS_1F) == 0);
    EXPECT(dev.ota_begin_count == 1);

    for (offset = 0; offset < 1024; offset += OTA_PACKET_SIZE) {
        uint8_t hi = dev.com_flash_high;
        uint8_t lo = dev.com_flash_low;
        EXPECT(dev_rs485_packet(&dev, hi, lo, image + offset, OTA_PACKET_SIZE) == 0);
    }
    EXPECT(dev.resume_chunk == 8);
    EXPECT(dev.ota_write_count == 8);
    EXPECT(dev.raw_write_count == 0);
    EXPECT(memcmp(dev.image, image, 1024) == 0);
    EXPECT(dev_rs485_packet(&dev, dev.com_flash_high, dev.com_flash_low, image, 0) == -1);
    EXPECT(dev.flash_errors == 1);

    EXPECT(dev_rs485_packet(&dev, 0x03, 0x80, image, OTA_PACKET_SIZE) == 1);
    EXPECT(dev.resume_chunk == 8);
    EXPECT(dev.ota_write_count == 8);

    EXPECT(dev_rs485_packet(&dev, 0x12, 0x34, image, OTA_PACKET_SIZE) == -2);
    EXPECT(dev.resume_chunk == 8);

    memcpy(stamp_before, dev.write_stamp, sizeof(stamp_before));
    dev.run_begin = 0;
    dev.com_update = 0;

    memset(&wifi, 0, sizeof(wifi));
    wifi.product = PM_ESP32_T3_SERIES;
    wifi.reg11 = 48;
    wifi.reg14 = 47;
    wifi.dump_ok = 1;
    wifi.status_ret = 1;
    wifi.status = ISP_STATUS_IN_PROGRESS_1F;
    wifi.meta_ret = 1;
    wifi.meta[0] = 8;
    wifi.file_md5[0] = 1;
    wifi.meta[2] = 1;
    wifi.file_len = 2048;
    wifi_plan(&wifi, &wifi_out);
    EXPECT(wifi_out.supported == 1);
    EXPECT(wifi_out.do_fresh == 0);
    EXPECT(wifi_out.resume_bytes == 1024);
    EXPECT(wifi_out.continue_flash_count == 3);

    memset(&serial, 0, sizeof(serial));
    serial.product = PM_ESP32_T3_SERIES;
    serial.reg11 = 48;
    serial.reg14 = 14;
    serial.ee10_status = 0;
    serial.reg16_status = ISP_STATUS_IN_PROGRESS_1F;
    serial.meta_ret = 1;
    serial.meta[0] = 8;
    serial.meta[2] = 0x1111;
    serial.file_md5[0] = 0x9999;
    serial.file_len = 2048;
    serial_bin_plan(&serial, &serial_out);
    EXPECT(serial_out.do_fresh == 0);
    EXPECT(serial_out.md5_match == 0);
    EXPECT(serial_out.start_offset == 1024);
    EXPECT(serial_out.write_count == 1);
    EXPECT(serial_out.writes[0].value == ISP_STATUS_IN_PROGRESS_1F);

    EXPECT(dev_status(&dev, ISP_STATUS_IN_PROGRESS_1F) == 0);
    EXPECT(dev.use_raw == 1);
    EXPECT(dev.erase_count == 1);
    EXPECT(dev.next_offset == 1024);

    for (offset = 1024; offset < 2048; offset += OTA_PACKET_SIZE) {
        uint8_t hi = dev.com_flash_high;
        uint8_t lo = dev.com_flash_low;
        EXPECT(dev_rs485_packet(&dev, hi, lo, image + offset, OTA_PACKET_SIZE) == 0);
    }
    EXPECT(dev.raw_write_count == 8);
    EXPECT(dev.resume_chunk == 16);
    EXPECT(memcmp(dev.image, image, 2048) == 0);
    EXPECT(memcmp(dev.write_stamp, stamp_before, sizeof(stamp_before)) == 0);
    EXPECT(dev_status(&dev, 1) == 0);
    EXPECT(dev.boot_set == 1);
    EXPECT(dev.finish_result == 400);
    EXPECT(dev.resume_chunk == 0);
    EXPECT(dev.com_update == 0);
}

void test_tftp_resume_matches_host(void)
{
    static device_t dev;
    uint8_t image[8192];
    wifi_input_t in;
    wifi_plan_t plan;
    int first = -1;
    unsigned block;
    unsigned offset;

    make_image(image, sizeof(image));
    dev_reset(&dev);
    dev_set_saved(&dev, 33, image, 33 * OTA_PACKET_SIZE);

    memset(&in, 0, sizeof(in));
    in.product = PM_ESP32_T3_SERIES;
    in.reg14 = 48;
    in.dump_ok = 1;
    in.status_ret = 1;
    in.status = ISP_STATUS_IN_PROGRESS_40;
    in.meta_ret = 1;
    in.meta[0] = 33;
    in.file_len = 8192;
    in.file_md5[0] = 5;
    in.meta[2] = 9;
    wifi_plan(&in, &plan);
    EXPECT(plan.do_fresh == 0);
    EXPECT(plan.md5_match == 0);
    EXPECT(plan.resume_bytes == 4096);
    EXPECT(plan.continue_flash_count == 9);

    dev_dhcp_match(&dev);
    EXPECT(dev.dhcp_prepare_count == 1);
    EXPECT(dev.use_raw == 1);
    EXPECT(dev.resume_chunk == 4096 / OTA_PACKET_SIZE);
    EXPECT(dev.block_no == plan.continue_flash_count);
    EXPECT(dev.erase_count == 1);
    EXPECT(wifi_blocks_to_send(in.file_len, plan.continue_flash_count, &first) == 8);
    EXPECT(first == 9);

    dev_dhcp_match(&dev);
    EXPECT(dev.dhcp_prepare_count == 1);

    EXPECT(dev_tftp_data(&dev, 8, image + 7 * 512, 512) == 1);
    EXPECT(dev.raw_write_count == 0);
    EXPECT(dev_tftp_data(&dev, 20, image, 512) == -2);
    EXPECT(dev.last_tftp_error == 1);

    for (block = 9; block <= 16; block++) {
        offset = (block - 1u) * 512u;
        EXPECT(dev_tftp_data(&dev, (uint16_t)block, image + offset, 512) == 0);
    }
    EXPECT(memcmp(dev.image, image, 8192) == 0);
    EXPECT(dev.block_no == 17);
    EXPECT(dev.resume_chunk == 8192 / OTA_PACKET_SIZE);
    EXPECT(dev.saved_flash_count >= 1);
    EXPECT(dev_tftp_data(&dev, 16, image + 15 * 512, 512) == 1);
}

void test_tftp_fresh_and_short_tail(void)
{
    static device_t dev;
    uint8_t image[600];
    uint8_t tail[100];
    unsigned i;

    make_image(image, sizeof(image));
    for (i = 0; i < sizeof(tail); i++)
        tail[i] = (uint8_t)(0xA0 + i);

    dev_reset(&dev);
    dev_dhcp_match(&dev);
    EXPECT(dev.use_raw == 0);
    EXPECT(dev.block_no == 1);
    EXPECT(dev.ota_begin_count == 1);
    EXPECT(dev.erase_count == 1);
    EXPECT(dev_tftp_data(&dev, 1, image, 512) == 0);
    EXPECT(dev.ota_write_count == 1);
    EXPECT(dev.use_raw == 0);
    EXPECT(memcmp(dev.image, image, 512) == 0);

    dev_reset(&dev);
    dev.resume_chunk = 4;
    dev.use_raw = 1;
    dev.block_no = 2;
    dev.tftp_prepared = 1;
    EXPECT(dev_tftp_data(&dev, 2, tail, 100) == 0);
    EXPECT(dev.raw_write_count == 1);
    EXPECT(memcmp(dev.image + 512, tail, 100) == 0);
    EXPECT(dev.image[512 + 100] == 0xFF);
    EXPECT(dev.image[512 + 111] == 0xFF);
    EXPECT(dev.write_stamp[512 + 111] == 1);
}

void test_finish_magic_and_clear(void)
{
    static device_t dev;

    dev_reset(&dev);
    dev.resume_chunk = 4;
    EXPECT(dev_status(&dev, ISP_STATUS_IN_PROGRESS_1F) == 0);
    EXPECT(dev.use_raw == 1);
    EXPECT(dev_status(&dev, 1) == -1);
    EXPECT(dev.finish_result == 100);
    EXPECT(dev.boot_set == 0);

    dev.image[0] = 0xE9;
    EXPECT(dev_status(&dev, 1) == 0);
    EXPECT(dev.boot_set == 1);
    EXPECT(dev.finish_result == 400);

    dev_reset(&dev);
    dev.resume_chunk = 4;
    dev.md5[0] = 0x1234;
    dev.firmware_size = 1000;
    EXPECT(dev_status(&dev, ISP_CMD_CLEAR_SAVED_UPDATE) == 0);
    EXPECT(dev.rebooted == 1);
    EXPECT(dev.cleared == 1);
    EXPECT(dev.resume_chunk == 0);
    EXPECT(dev.md5[0] == 0);
    EXPECT(dev.firmware_size == 0);
    EXPECT(dev.com_update == 0);
}

void test_address_wrap_and_old_device_session(void)
{
    static device_t dev;
    old_device_t old;
    serial_input_t serial;
    serial_plan_t plan;
    wifi_input_t wifi;
    wifi_plan_t wifi_out;
    uint8_t image[256];
    uint8_t pkt[OTA_PACKET_SIZE];

    dev_reset(&dev);
    dev.resume_chunk = 512;
    memset(pkt, 0x5A, sizeof(pkt));
    EXPECT(dev_status(&dev, ISP_STATUS_IN_PROGRESS_1F) == 0);
    EXPECT(dev.use_raw == 1);
    EXPECT(dev.com_flash_high == 0);
    EXPECT(dev.com_flash_low == 0);
    EXPECT(dev_rs485_packet(&dev, 0x00, 0x00, pkt, OTA_PACKET_SIZE) == 0);
    EXPECT(dev.resume_chunk == 513);
    EXPECT(dev.raw_write_count == 1);
    EXPECT(dev.image[512u * OTA_PACKET_SIZE] == 0x5A);

    make_image(image, sizeof(image));
    memset(&serial, 0, sizeof(serial));
    serial.product = 6;
    serial.reg11 = 26;
    serial.reg14 = 14;
    serial.ee10_status = ISP_STATUS_IN_PROGRESS_1F;
    serial.reg16_status = ISP_STATUS_IN_PROGRESS_1F;
    serial.meta_ret = 1;
    serial.meta[0] = 20;
    serial.file_len = 256;
    serial_bin_plan(&serial, &plan);
    EXPECT(plan.supported == 0);
    EXPECT(plan.read_meta == 0);
    EXPECT(plan.start_offset == 0);

    old_dev_reset(&old);
    old_dev_apply_writes(&old, plan.writes, plan.write_count);
    EXPECT(old.saw_7f == 1);
    EXPECT(old.saw_3f == 1);
    EXPECT(old.saw_1f == 1);
    EXPECT(old.saw_33 == 0);
    EXPECT(old.saw_md5 == 0);
    EXPECT(old.erased == 1);
    old_dev_program(&old, image, sizeof(image));
    EXPECT(old.offset == 256);
    EXPECT(memcmp(old.image, image, sizeof(image)) == 0);

    memset(&wifi, 0, sizeof(wifi));
    wifi.dump_ok = 1;
    wifi.product = 8;
    wifi.reg11 = 30;
    wifi.reg14 = 30;
    wifi.mini = ISP_MINI_HUB;
    wifi.status_ret = 1;
    wifi.status = ISP_STATUS_IN_PROGRESS_1F;
    wifi.meta_ret = 1;
    wifi.meta[0] = 50;
    wifi.file_len = 8192;
    wifi_plan(&wifi, &wifi_out);
    EXPECT(wifi_out.supported == 0);
    EXPECT(wifi_out.read_resume_block == 0);
    EXPECT(wifi_out.continue_flash_count == 0);
    EXPECT(wifi_out.write_count == 0);
}

void test_bootloader_flow(void)
{
    EXPECT(OTA_PACKET_SIZE == 128u);
    EXPECT(OTA_FLASH_CHUNK_SIZE == 128u);
    test_rs485_fresh_then_resume();
    test_tftp_resume_matches_host();
    test_tftp_fresh_and_short_tail();
    test_finish_magic_and_clear();
    test_address_wrap_and_old_device_session();
}
