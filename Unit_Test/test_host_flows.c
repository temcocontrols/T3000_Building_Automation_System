#include "harness.h"
#include "firmware_flow.h"
#include "isp_constants.h"

#include <string.h>

static void fill_md5(unsigned md5[4])
{
    md5[0] = 0x1111;
    md5[1] = 0x2222;
    md5[2] = 0x3333;
    md5[3] = 0x4444;
}

static wifi_input_t wifi_base(void)
{
    wifi_input_t in;
    memset(&in, 0, sizeof(in));
    in.product = PM_ESP32_T3_SERIES;
    in.reg11 = 14;
    in.reg14 = 48;
    in.mini = 0;
    in.dump_ok = 1;
    in.status_ret = 1;
    in.status = ISP_STATUS_IN_PROGRESS_1F;
    in.meta_ret = 1;
    in.meta[0] = 8;
    in.file_len = 2048;
    fill_md5(in.file_md5);
    fill_md5(&in.meta[2]);
    return in;
}

static serial_input_t serial_base(void)
{
    serial_input_t in;
    memset(&in, 0, sizeof(in));
    in.product = PM_ESP32_T3_SERIES;
    in.reg11 = 10;
    in.reg14 = 48;
    in.mini_read_result = -1;
    in.ee10_status = 0;
    in.reg16_status = ISP_STATUS_IN_PROGRESS_1F;
    in.meta_ret = 1;
    in.meta[0] = 8;
    in.resume_reg_result = 8 * ISP_RESUME_PACKET_BYTES;
    in.file_len = 4096;
    fill_md5(in.file_md5);
    fill_md5(&in.meta[2]);
    return in;
}

void test_wifi_new_and_old(void)
{
    wifi_input_t in;
    wifi_plan_t plan;
    int first = -1;
    int sent;

    in = wifi_base();
    wifi_plan(&in, &plan);
    EXPECT(plan.supported == 1);
    EXPECT(plan.boot == 48);
    EXPECT(plan.do_fresh == 0);
    EXPECT(plan.read_resume_block == 1);
    EXPECT(plan.md5_match == 1);
    EXPECT(plan.resume_bytes == 1024);
    EXPECT(plan.continue_flash_count == 3);
    EXPECT(plan.clear_saved == 0);
    EXPECT(plan.write_md5 == 0);
    EXPECT(plan.write_count == 0);
    EXPECT(plan.could_not_read == 0);
    sent = wifi_blocks_to_send(in.file_len, plan.continue_flash_count, &first);
    EXPECT(first == 3);
    EXPECT(sent == 2);

    in = wifi_base();
    in.meta[2] = 0xABCD;
    wifi_plan(&in, &plan);
    EXPECT(plan.md5_match == 0);
    EXPECT(plan.do_fresh == 0);
    EXPECT(plan.continue_flash_count == 3);
    EXPECT(plan.write_count == 0);

    in = wifi_base();
    in.reg11 = 48;
    in.reg14 = 10;
    wifi_plan(&in, &plan);
    EXPECT(plan.boot == 48);
    EXPECT(plan.supported == 1);
    EXPECT(plan.do_fresh == 0);

    in = wifi_base();
    in.meta[0] = 33;
    in.file_len = 8192;
    wifi_plan(&in, &plan);
    EXPECT(plan.packets == 33);
    EXPECT(plan.resume_bytes == 4096);
    EXPECT(plan.continue_flash_count == 9);
    EXPECT(plan.do_fresh == 0);

    in = wifi_base();
    in.meta[0] = 1;
    in.file_len = 8192;
    wifi_plan(&in, &plan);
    EXPECT(plan.resume_bytes == 0);
    EXPECT(plan.do_fresh == 1);
    EXPECT(plan.clear_saved == 1);
    EXPECT(plan.write_count == 1);
    EXPECT(plan.writes[0].reg == ISP_REG_UPDATE_STATUS);
    EXPECT(plan.writes[0].value == ISP_CMD_CLEAR_SAVED_UPDATE);
    EXPECT(plan.write_md5 == 0);

    in = wifi_base();
    in.meta[0] = 16;
    in.file_len = 2048;
    wifi_plan(&in, &plan);
    EXPECT(plan.resume_bytes == 2048);
    EXPECT(plan.do_fresh == 1);
    EXPECT(plan.clear_saved == 1);

    in = wifi_base();
    in.status = 0;
    wifi_plan(&in, &plan);
    EXPECT(plan.do_fresh == 1);
    EXPECT(plan.clear_saved == 0);
    EXPECT(plan.write_md5 == 1);
    EXPECT(plan.continue_flash_count == 0);
    EXPECT(plan.write_count == 6);
    EXPECT(plan.writes[0].reg == ISP_REG_MD5_START);
    EXPECT(plan.writes[0].value == 0x1111);
    EXPECT(plan.writes[4].reg == ISP_REG_FW_SIZE_HI);
    EXPECT(plan.writes[4].value == 0);
    EXPECT(plan.writes[5].reg == ISP_REG_FW_SIZE_LO);
    EXPECT(plan.writes[5].value == 2048);

    in = wifi_base();
    in.status_ret = 0;
    wifi_plan(&in, &plan);
    EXPECT(plan.could_not_read == 1);
    EXPECT(plan.do_fresh == 1);
    EXPECT(plan.clear_saved == 0);
    EXPECT(plan.write_md5 == 1);

    in = wifi_base();
    in.meta_ret = 0;
    in.status = ISP_STATUS_IN_PROGRESS_40;
    wifi_plan(&in, &plan);
    EXPECT(plan.could_not_read == 1);
    EXPECT(plan.clear_saved == 0);
    EXPECT(plan.write_md5 == 1);

    in = wifi_base();
    in.status = ISP_STATUS_IN_PROGRESS_40;
    wifi_plan(&in, &plan);
    EXPECT(plan.do_fresh == 0);
    EXPECT(plan.continue_flash_count == 3);

    in = wifi_base();
    in.product = 6;
    in.reg11 = 11;
    in.reg14 = 26;
    in.mini = ISP_MINI_HUB;
    wifi_plan(&in, &plan);
    EXPECT(plan.supported == 0);
    EXPECT(plan.read_resume_block == 0);
    EXPECT(plan.do_fresh == 1);
    EXPECT(plan.continue_flash_count == 0);
    EXPECT(plan.write_count == 0);
    EXPECT(plan.clear_saved == 0);
    EXPECT(plan.write_md5 == 0);
    sent = wifi_blocks_to_send(2048, plan.continue_flash_count, &first);
    EXPECT(first == 1);
    EXPECT(sent == 4);

    in = wifi_base();
    in.reg14 = 47;
    in.reg11 = 14;
    in.mini = 0;
    wifi_plan(&in, &plan);
    EXPECT(plan.boot == 47);
    EXPECT(plan.supported == 0);
    EXPECT(plan.read_resume_block == 0);
    EXPECT(plan.write_count == 0);

    in = wifi_base();
    in.reg14 = 47;
    in.mini = ISP_MINI_HUB;
    wifi_plan(&in, &plan);
    EXPECT(plan.supported == 1);
    EXPECT(plan.do_fresh == 0);
    EXPECT(plan.continue_flash_count == 3);

    in = wifi_base();
    in.product = PM_TSTAT10;
    in.reg14 = 47;
    in.mini = 0;
    wifi_plan(&in, &plan);
    EXPECT(plan.supported == 1);
    EXPECT(plan.do_fresh == 0);

    in = wifi_base();
    in.dump_ok = 0;
    in.product = PM_ESP32_T3_SERIES;
    in.reg14 = 48;
    wifi_plan(&in, &plan);
    EXPECT(plan.supported == 0);
    EXPECT(plan.read_resume_block == 0);
    EXPECT(plan.write_count == 0);

    in = wifi_base();
    in.reg14 = 46;
    in.mini = ISP_MINI_TSTAT11;
    wifi_plan(&in, &plan);
    EXPECT(plan.supported == 0);
    EXPECT(plan.write_count == 0);
}

void test_serial_new_and_old(void)
{
    serial_input_t in;
    serial_plan_t plan;
    tcp_input_t tcp;
    tcp_plan_t tcp_plan_out;

    in = serial_base();
    serial_bin_plan(&in, &plan);
    EXPECT(plan.supported == 1);
    EXPECT(plan.read_mini == 0);
    EXPECT(plan.read_ee10 == 1);
    EXPECT(plan.read_reg16 == 1);
    EXPECT(plan.read_meta == 1);
    EXPECT(plan.do_fresh == 0);
    EXPECT(plan.md5_match == 1);
    EXPECT(plan.start_offset == 1024);
    EXPECT(plan.write_count == 1);
    EXPECT(plan.writes[0].reg == ISP_REG_UPDATE_STATUS);
    EXPECT(plan.writes[0].value == ISP_STATUS_IN_PROGRESS_1F);
    EXPECT(fw_plan_has_write(plan.writes, plan.write_count, ISP_REG_UPDATE_STATUS, 0x7F) == 0);
    EXPECT(fw_plan_has_write(plan.writes, plan.write_count, ISP_REG_UPDATE_STATUS, 0x3F) == 0);

    in = serial_base();
    in.meta[3] = 0;
    serial_bin_plan(&in, &plan);
    EXPECT(plan.md5_match == 0);
    EXPECT(plan.do_fresh == 0);
    EXPECT(plan.start_offset == 1024);
    EXPECT(plan.write_count == 1);

    in = serial_base();
    in.ee10_status = 0;
    in.reg16_status = ISP_STATUS_IN_PROGRESS_40;
    serial_bin_plan(&in, &plan);
    EXPECT(plan.do_fresh == 0);
    EXPECT(plan.start_offset == 1024);

    in = serial_base();
    in.meta_ret = -1;
    serial_bin_plan(&in, &plan);
    EXPECT(plan.could_not_read == 1);
    EXPECT(plan.do_fresh == 1);
    EXPECT(plan.start_offset == 0);
    EXPECT(plan.write_count == 9);
    EXPECT(plan.writes[0].reg == ISP_REG_MD5_START);
    EXPECT(plan.writes[4].reg == ISP_REG_FW_SIZE_HI);
    EXPECT(plan.writes[5].reg == ISP_REG_FW_SIZE_LO);
    EXPECT(plan.writes[5].value == 4096);
    EXPECT(plan.writes[6].value == 0x7F);
    EXPECT(plan.writes[7].value == 0x3F);
    EXPECT(plan.writes[8].value == ISP_STATUS_IN_PROGRESS_1F);
    EXPECT(fw_plan_has_write(plan.writes, plan.write_count, ISP_REG_UPDATE_STATUS, 0x7F) == 1);
    EXPECT(fw_plan_has_write(plan.writes, plan.write_count, ISP_REG_UPDATE_STATUS, 0x3F) == 1);

    in = serial_base();
    in.meta[0] = 0;
    serial_bin_plan(&in, &plan);
    EXPECT(plan.do_fresh == 1);
    EXPECT(plan.could_not_read == 0);
    EXPECT(plan.start_offset == 0);
    EXPECT(fw_plan_has_write(plan.writes, plan.write_count, ISP_REG_UPDATE_STATUS, 0x7F) == 1);

    in = serial_base();
    in.reg16_status = 0;
    in.ee10_status = 0;
    serial_bin_plan(&in, &plan);
    EXPECT(plan.do_fresh == 1);
    EXPECT(plan.read_meta == 0);
    EXPECT(plan.start_offset == 0);
    EXPECT(plan.write_count == 9);

    in = serial_base();
    in.product = PM_TSTAT10;
    in.reg11 = 47;
    in.reg14 = 14;
    serial_bin_plan(&in, &plan);
    EXPECT(plan.supported == 1);
    EXPECT(plan.read_mini == 0);
    EXPECT(plan.do_fresh == 0);
    EXPECT(plan.start_offset == 0);
    EXPECT(plan.write_count == 1);

    in = serial_base();
    in.product = 6;
    in.reg11 = 4;
    in.reg14 = 26;
    serial_bin_plan(&in, &plan);
    EXPECT(plan.supported == 0);
    EXPECT(plan.read_mini == 0);
    EXPECT(plan.read_ee10 == 1);
    EXPECT(plan.read_reg16 == 0);
    EXPECT(plan.read_meta == 0);
    EXPECT(plan.do_fresh == 1);
    EXPECT(plan.start_offset == 0);
    EXPECT(plan.write_count == 3);
    EXPECT(plan.writes[0].value == 0x7F);
    EXPECT(plan.writes[1].value == 0x3F);
    EXPECT(plan.writes[2].value == ISP_STATUS_IN_PROGRESS_1F);
    EXPECT(fw_plan_has_write(plan.writes, plan.write_count, ISP_REG_MD5_START, 0x1111) == 0);

    in = serial_base();
    in.reg14 = 47;
    in.reg11 = 11;
    in.mini_read_result = -1;
    serial_bin_plan(&in, &plan);
    EXPECT(plan.boot == 47);
    EXPECT(plan.read_mini == 1);
    EXPECT(plan.supported == 0);
    EXPECT(plan.read_reg16 == 0);
    EXPECT(plan.write_count == 3);
    EXPECT(plan.writes[0].value == 0x7F);

    in = serial_base();
    in.reg14 = 47;
    in.mini_read_result = ISP_MINI_HUB;
    serial_bin_plan(&in, &plan);
    EXPECT(plan.read_mini == 1);
    EXPECT(plan.supported == 1);
    EXPECT(plan.do_fresh == 0);
    EXPECT(plan.start_offset == 1024);

    in = serial_base();
    in.reg14 = 47;
    in.mini_read_result = ISP_MINI_TSTAT11;
    serial_bin_plan(&in, &plan);
    EXPECT(plan.supported == 1);

    in = serial_base();
    serial_hex_plan(&in, &plan);
    EXPECT(plan.supported == 1);
    EXPECT(plan.read_resume_count == 1);
    EXPECT(plan.do_fresh == 0);
    EXPECT(plan.start_offset == 1024);
    EXPECT(plan.write_count == 0);
    EXPECT(plan.could_not_read == 0);

    in = serial_base();
    in.resume_reg_result = -1;
    serial_hex_plan(&in, &plan);
    EXPECT(plan.could_not_read == 1);
    EXPECT(plan.do_fresh == 1);
    EXPECT(plan.start_offset == 0);
    EXPECT(plan.write_count == 3);
    EXPECT(plan.writes[0].value == 0x7F);
    EXPECT(plan.writes[1].value == 0x3F);
    EXPECT(plan.writes[2].value == 0x1F);

    in = serial_base();
    in.resume_reg_result = 0;
    serial_hex_plan(&in, &plan);
    EXPECT(plan.do_fresh == 1);
    EXPECT(plan.could_not_read == 0);
    EXPECT(plan.write_count == 3);

    in = serial_base();
    in.product = 7;
    in.reg14 = 20;
    in.reg11 = 20;
    serial_hex_plan(&in, &plan);
    EXPECT(plan.supported == 0);
    EXPECT(plan.read_resume_count == 0);
    EXPECT(plan.read_reg16 == 0);
    EXPECT(plan.do_fresh == 1);
    EXPECT(plan.write_count == 3);
    EXPECT(plan.could_not_read == 0);

    memset(&tcp, 0, sizeof(tcp));
    tcp.product = PM_ESP32_T3_SERIES;
    tcp.reg4 = 48;
    tcp.reg11 = 48;
    tcp.reg14 = 48;
    tcp.mini_read_result = 0;
    tcp.meta_ret = 1;
    tcp.info[0] = 10;
    fill_md5(tcp.file_md5);
    tcp.info[1] = tcp.file_md5[0];
    tcp.info[2] = tcp.file_md5[1];
    tcp.info[3] = tcp.file_md5[2];
    tcp.info[4] = tcp.file_md5[3];
    tcp_plan(&tcp, &tcp_plan_out);
    EXPECT(tcp_plan_out.entered == 1);
    EXPECT(tcp_plan_out.supported == 1);
    EXPECT(tcp_plan_out.md5_match == 1);
    EXPECT(tcp_plan_out.continue_bytes == 10 * ISP_RESUME_PACKET_BYTES);
    EXPECT(tcp_plan_out.could_not_read == 0);
    EXPECT(tcp_plan_out.write_count == 1);
    EXPECT(tcp_plan_out.writes[0].reg == ISP_REG_UPDATE_STATUS);
    EXPECT(tcp_plan_out.writes[0].value == 127);

    tcp.info[1] = 0x9999;
    tcp_plan(&tcp, &tcp_plan_out);
    EXPECT(tcp_plan_out.md5_match == 0);
    EXPECT(tcp_plan_out.continue_bytes == 0);
    EXPECT(tcp_plan_out.write_count == 5);
    EXPECT(tcp_plan_out.writes[0].reg == ISP_REG_SERIAL_MD5_WRITE);
    EXPECT(tcp_plan_out.writes[0].value == 0x1111);
    EXPECT(tcp_plan_out.writes[4].value == 127);

    tcp.meta_ret = -1;
    tcp_plan(&tcp, &tcp_plan_out);
    EXPECT(tcp_plan_out.could_not_read == 1);
    EXPECT(tcp_plan_out.continue_bytes == 0);
    EXPECT(tcp_plan_out.writes[0].reg == ISP_REG_SERIAL_MD5_WRITE);
    EXPECT(tcp_plan_out.writes[4].value == 127);

    tcp.meta_ret = 1;
    tcp.info[1] = tcp.file_md5[0];
    tcp.reg4 = 40;
    tcp_plan(&tcp, &tcp_plan_out);
    EXPECT(tcp_plan_out.entered == 0);
    EXPECT(tcp_plan_out.continue_bytes == 0);
    EXPECT(tcp_plan_out.write_count == 1);
    EXPECT(tcp_plan_out.writes[0].value == 127);

    tcp.reg4 = 26;
    tcp.reg14 = 26;
    tcp.reg11 = 26;
    tcp.product = 6;
    tcp_plan(&tcp, &tcp_plan_out);
    EXPECT(tcp_plan_out.supported == 0);
    EXPECT(tcp_plan_out.entered == 0);
    EXPECT(tcp_plan_out.write_count == 1);
    EXPECT(tcp_plan_out.writes[0].value == 127);
    EXPECT(fw_plan_has_write(tcp_plan_out.writes, tcp_plan_out.write_count, ISP_REG_SERIAL_MD5_WRITE, 0x1111) == 0);
}
