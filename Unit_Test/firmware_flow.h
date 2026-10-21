#ifndef FIRMWARE_FLOW_H
#define FIRMWARE_FLOW_H

#include <stddef.h>
#include <stdint.h>

/* Linked from the production function in ISP/global_function.cpp. */
int isp_resume_supported(unsigned product_id, unsigned bootloader_version, unsigned mini_type);

#define FW_MAX_WRITES 16
#define FW_META_LEN 6
#define FW_MD5_LEN 4

typedef struct {
    unsigned reg;
    unsigned value;
} fw_write_t;

typedef struct {
    unsigned product;
    unsigned reg11;
    unsigned reg14;
    unsigned mini;
    int dump_ok;
    int status_ret;
    unsigned status;
    int meta_ret;
    unsigned meta[FW_META_LEN];
    unsigned file_md5[FW_MD5_LEN];
    unsigned file_len;
} wifi_input_t;

typedef struct {
    int supported;
    int do_fresh;
    int continue_flash_count;
    int resume_bytes;
    int packets;
    int md5_match;
    int could_not_read;
    int clear_saved;
    int write_md5;
    int read_resume_block;
    int boot;
    fw_write_t writes[FW_MAX_WRITES];
    int write_count;
} wifi_plan_t;

typedef struct {
    unsigned product;
    unsigned reg11;
    unsigned reg14;
    int mini_read_result;
    int ee10_status;
    int reg16_status;
    int meta_ret;
    unsigned meta[FW_META_LEN];
    int resume_reg_result;
    unsigned file_md5[FW_MD5_LEN];
    unsigned file_len;
} serial_input_t;

typedef struct {
    int supported;
    int read_mini;
    int read_reg16;
    int read_meta;
    int read_ee10;
    int read_resume_count;
    int do_fresh;
    int start_offset;
    int md5_match;
    int could_not_read;
    int boot;
    fw_write_t writes[FW_MAX_WRITES];
    int write_count;
} serial_plan_t;

typedef struct {
    unsigned product;
    unsigned reg4;
    unsigned reg11;
    unsigned reg14;
    int mini_read_result;
    int meta_ret;
    unsigned info[FW_META_LEN];
    unsigned file_md5[FW_MD5_LEN];
} tcp_input_t;

typedef struct {
    int entered;
    int supported;
    int read_mini;
    int continue_bytes;
    int md5_match;
    int could_not_read;
    fw_write_t writes[FW_MAX_WRITES];
    int write_count;
} tcp_plan_t;

#define DEV_PART_SIZE (128 * 1024)

typedef struct {
    uint8_t image[DEV_PART_SIZE];
    uint8_t write_stamp[DEV_PART_SIZE];
    uint32_t resume_chunk;
    uint32_t next_offset;
    uint32_t image_end;
    uint32_t firmware_size;
    uint16_t block_no;
    uint16_t md5[4];
    uint8_t com_flash_high;
    uint8_t com_flash_low;
    int use_raw;
    int com_update;
    int run_begin;
    int ota_begin_count;
    int ota_write_count;
    int raw_write_count;
    int erase_count;
    int abort_count;
    int boot_set;
    int rebooted;
    int cleared;
    int finish_result;
    int ignored_status;
    int softap_suspend;
    int saved_flash_count;
    int flash_errors;
    int tftp_prepared;
    int tftp_fresh_ready;
    int handle_open;
    int magic_at_zero;
    int last_tftp_ack;
    int last_tftp_error;
    int dhcp_prepare_count;
} device_t;

typedef struct {
    int saw_7f;
    int saw_3f;
    int saw_1f;
    int saw_33;
    int saw_md5;
    int saw_resume_read;
    int offset;
    int erased;
    uint8_t image[DEV_PART_SIZE];
} old_device_t;

void wifi_plan(const wifi_input_t *in, wifi_plan_t *out);
void serial_bin_plan(const serial_input_t *in, serial_plan_t *out);
void serial_hex_plan(const serial_input_t *in, serial_plan_t *out);
void tcp_plan(const tcp_input_t *in, tcp_plan_t *out);

int wifi_blocks_to_send(unsigned file_len, int continue_flash_count, int *first_block);

void dev_reset(device_t *d);
void dev_set_saved(device_t *d, uint32_t chunk, const uint8_t *prefix, unsigned prefix_len);
int dev_status(device_t *d, uint8_t cmd);
int dev_rs485_packet(device_t *d, uint8_t addr_hi, uint8_t addr_lo, const uint8_t *data, int len);
void dev_dhcp_match(device_t *d);
int dev_tftp_data(device_t *d, uint16_t block, const uint8_t *payload, int len);

void old_dev_reset(old_device_t *d);
void old_dev_apply_writes(old_device_t *d, const fw_write_t *writes, int count);
void old_dev_program(old_device_t *d, const uint8_t *image, unsigned len);

int fw_plan_has_write(const fw_write_t *writes, int count, unsigned reg, unsigned value);

#endif
