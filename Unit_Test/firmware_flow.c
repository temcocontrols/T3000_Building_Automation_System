#include "firmware_flow.h"

#include "isp_constants.h"

#include <string.h>

static void push_write(fw_write_t *writes, int *count, unsigned reg, unsigned value)
{
    if (*count >= FW_MAX_WRITES)
        return;
    writes[*count].reg = reg;
    writes[*count].value = value;
    (*count)++;
}

static unsigned boot_of(unsigned reg11, unsigned reg14)
{
    return (reg11 > reg14) ? reg11 : reg14;
}

static unsigned serial_mini(const serial_input_t *in, int *read_mini)
{
    unsigned boot = boot_of(in->reg11, in->reg14);
    *read_mini = 0;
    if ((boot == ISP_RESUME_BOOTLOADER_LIMITED) && (in->product != PM_TSTAT10)) {
        *read_mini = 1;
        if (in->mini_read_result > 0)
            return (unsigned)in->mini_read_result;
    }
    return 0;
}

void wifi_plan(const wifi_input_t *in, wifi_plan_t *out)
{
    int k;
    unsigned boot;
    int status_ret = -1;
    int meta_ret = -1;
    unsigned status = 0;
    unsigned meta[FW_META_LEN];
    int packets;
    int saved_bytes;
    int resume_bytes;
    int in_progress;
    int saved_update;

    memset(out, 0, sizeof(*out));
    memset(meta, 0, sizeof(meta));
    out->do_fresh = 1;
    out->continue_flash_count = 0;

    if (!in->dump_ok) {
        boot = 0;
        out->supported = isp_resume_supported(0, 0, 0);
    } else {
        boot = boot_of(in->reg11, in->reg14);
        out->supported = isp_resume_supported(in->product, boot, in->mini);
    }
    out->boot = (int)boot;

    if (!out->supported)
        return;

    out->read_resume_block = 1;
    status_ret = in->status_ret;
    meta_ret = in->meta_ret;
    if (status_ret > 0)
        status = in->status;
    if (meta_ret > 0) {
        memcpy(meta, in->meta, sizeof(meta));
        out->md5_match = 1;
        for (k = 0; k < ISP_REG_MD5_COUNT; k++) {
            if (meta[2 + k] != in->file_md5[k]) {
                out->md5_match = 0;
                break;
            }
        }
    }

    packets = (meta_ret > 0) ? (int)meta[0] : 0;
    saved_bytes = packets * ISP_RESUME_PACKET_BYTES;
    resume_bytes = saved_bytes;
    if ((saved_bytes % 512) != 0)
        resume_bytes = saved_bytes & ~4095;

    out->packets = packets;
    out->resume_bytes = resume_bytes;

    in_progress = (status_ret > 0)
        && (status == ISP_STATUS_IN_PROGRESS_1F || status == ISP_STATUS_IN_PROGRESS_40);
    saved_update = in_progress
        && (meta_ret > 0)
        && (resume_bytes > 0)
        && (resume_bytes < (int)in->file_len);

    if (status_ret <= 0 || meta_ret <= 0) {
        out->do_fresh = 1;
        out->could_not_read = 1;
    } else if (saved_update) {
        out->continue_flash_count = (resume_bytes / 512) + 1;
        out->do_fresh = 0;
    }

    if (out->do_fresh) {
        out->continue_flash_count = 0;
        if (in_progress && meta_ret > 0) {
            out->clear_saved = 1;
            push_write(out->writes, &out->write_count, ISP_REG_UPDATE_STATUS, ISP_CMD_CLEAR_SAVED_UPDATE);
        } else {
            out->write_md5 = 1;
            for (k = 0; k < ISP_REG_MD5_COUNT; k++)
                push_write(out->writes, &out->write_count, (unsigned)(ISP_REG_MD5_START + k), in->file_md5[k]);
            push_write(out->writes, &out->write_count, ISP_REG_FW_SIZE_HI, (in->file_len >> 16) & 0xFFFFu);
            push_write(out->writes, &out->write_count, ISP_REG_FW_SIZE_LO, in->file_len & 0xFFFFu);
        }
    }
}

void serial_bin_plan(const serial_input_t *in, serial_plan_t *out)
{
    int k;
    unsigned mini = 0;
    unsigned boot;
    int update_status;
    int do_fresh = 0;
    int resume_offset = 0;
    int md5_match = 0;

    memset(out, 0, sizeof(*out));
    boot = boot_of(in->reg11, in->reg14);
    out->boot = (int)boot;
    mini = serial_mini(in, &out->read_mini);
    out->supported = isp_resume_supported(in->product, boot, mini);
    out->read_ee10 = 1;
    update_status = in->ee10_status;

    if (out->supported) {
        out->read_reg16 = 1;
        if (in->reg16_status == ISP_STATUS_IN_PROGRESS_1F || in->reg16_status == ISP_STATUS_IN_PROGRESS_40)
            update_status = in->reg16_status;
    }

    if (out->supported
        && (update_status == ISP_STATUS_IN_PROGRESS_40 || update_status == ISP_STATUS_IN_PROGRESS_1F)) {
        out->read_meta = 1;
        if (in->meta_ret <= 0) {
            do_fresh = 1;
            out->could_not_read = 1;
        } else if ((int)in->meta[0] * ISP_RESUME_PACKET_BYTES <= 0) {
            do_fresh = 1;
        } else {
            md5_match = 1;
            for (k = 0; k < ISP_REG_MD5_COUNT; k++) {
                if (in->meta[2 + k] != in->file_md5[k]) {
                    md5_match = 0;
                    break;
                }
            }
            resume_offset = (int)in->meta[0] * ISP_RESUME_PACKET_BYTES;
        }
    } else {
        do_fresh = 1;
    }

    out->do_fresh = do_fresh;
    out->md5_match = md5_match;
    out->start_offset = 0;

    if (do_fresh) {
        if (out->supported) {
            for (k = 0; k < ISP_REG_MD5_COUNT; k++)
                push_write(out->writes, &out->write_count, (unsigned)(ISP_REG_MD5_START + k), in->file_md5[k]);
            push_write(out->writes, &out->write_count, ISP_REG_FW_SIZE_HI, (in->file_len >> 16) & 0xFFFFu);
            push_write(out->writes, &out->write_count, ISP_REG_FW_SIZE_LO, in->file_len & 0xFFFFu);
        }
        push_write(out->writes, &out->write_count, ISP_REG_UPDATE_STATUS, 0x7F);
        push_write(out->writes, &out->write_count, ISP_REG_UPDATE_STATUS, 0x3F);
    }
    push_write(out->writes, &out->write_count, ISP_REG_UPDATE_STATUS, ISP_STATUS_IN_PROGRESS_1F);

    if (in->product == PM_ESP32_T3_SERIES)
        out->start_offset = do_fresh ? 0 : resume_offset;
}

void serial_hex_plan(const serial_input_t *in, serial_plan_t *out)
{
    unsigned mini = 0;
    unsigned boot;
    int update_status;
    int resume_offset = -1;

    memset(out, 0, sizeof(*out));
    boot = boot_of(in->reg11, in->reg14);
    out->boot = (int)boot;
    mini = serial_mini(in, &out->read_mini);
    out->supported = isp_resume_supported(in->product, boot, mini);
    out->read_ee10 = 1;
    update_status = in->ee10_status;

    if (out->supported) {
        out->read_reg16 = 1;
        if (in->reg16_status == ISP_STATUS_IN_PROGRESS_1F || in->reg16_status == ISP_STATUS_IN_PROGRESS_40)
            update_status = in->reg16_status;
    }

    if (out->supported
        && (update_status == ISP_STATUS_IN_PROGRESS_40 || update_status == ISP_STATUS_IN_PROGRESS_1F)) {
        out->read_resume_count = 1;
        resume_offset = in->resume_reg_result;
    }

    if (resume_offset > 0) {
        out->do_fresh = 0;
        out->start_offset = resume_offset;
    } else {
        out->do_fresh = 1;
        out->start_offset = 0;
        if (out->supported && resume_offset < 0
            && (update_status == ISP_STATUS_IN_PROGRESS_40 || update_status == ISP_STATUS_IN_PROGRESS_1F))
            out->could_not_read = 1;
        push_write(out->writes, &out->write_count, ISP_REG_UPDATE_STATUS, 0x7F);
        push_write(out->writes, &out->write_count, ISP_REG_UPDATE_STATUS, 0x3F);
        push_write(out->writes, &out->write_count, ISP_REG_UPDATE_STATUS, ISP_STATUS_IN_PROGRESS_1F);
    }
}

void tcp_plan(const tcp_input_t *in, tcp_plan_t *out)
{
    int x;
    unsigned mini = 0;
    unsigned boot;
    int read_mini = 0;
    serial_input_t probe;

    memset(out, 0, sizeof(*out));
    memset(&probe, 0, sizeof(probe));
    probe.product = in->product;
    probe.reg11 = in->reg11;
    probe.reg14 = in->reg14;
    probe.mini_read_result = in->mini_read_result;
    boot = boot_of(in->reg11, in->reg14);
    mini = serial_mini(&probe, &read_mini);
    out->read_mini = read_mini;
    out->supported = isp_resume_supported(in->product, boot, mini);

    if ((in->reg4 == in->reg14) && (in->reg14 >= 20)
        && (in->product == PM_ESP32_T3_SERIES) && out->supported) {
        out->entered = 1;
        if (in->meta_ret <= 0) {
            out->continue_bytes = 0;
            out->could_not_read = 1;
        } else {
            out->md5_match = 1;
            for (x = 0; x < 4; x++) {
                if (in->info[x + 1] != in->file_md5[x])
                    out->md5_match = 0;
            }
            if (out->md5_match && in->info[0] != 0)
                out->continue_bytes = (int)in->info[0] * ISP_RESUME_PACKET_BYTES;
        }
        if (out->continue_bytes == 0) {
            for (x = 0; x < 4; x++)
                push_write(out->writes, &out->write_count, (unsigned)(ISP_REG_SERIAL_MD5_WRITE + x), in->file_md5[x]);
        }
    }

    push_write(out->writes, &out->write_count, ISP_REG_UPDATE_STATUS, 127);
}

int wifi_blocks_to_send(unsigned file_len, int continue_flash_count, int *first_block)
{
    unsigned sent = 0;
    unsigned offset = 0;
    int block = 0;
    *first_block = -1;
    while (offset < file_len) {
        unsigned chunk = file_len - offset;
        if (chunk > 512)
            chunk = 512;
        block++;
        offset += chunk;
        if (continue_flash_count > 0 && block < continue_flash_count)
            continue;
        if (*first_block < 0)
            *first_block = block;
        sent++;
    }
    return (int)sent;
}

int fw_plan_has_write(const fw_write_t *writes, int count, unsigned reg, unsigned value)
{
    int i;
    for (i = 0; i < count; i++) {
        if (writes[i].reg == reg && writes[i].value == value)
            return 1;
    }
    return 0;
}

static void dev_update_address(device_t *d, uint16_t pkt_count)
{
    uint16_t byte_addr = (uint16_t)(pkt_count * OTA_PACKET_SIZE);
    d->com_flash_high = (uint8_t)((byte_addr >> 8) & 0xFF);
    d->com_flash_low = (uint8_t)(byte_addr & 0xFF);
}

static void dev_clear_progress(device_t *d)
{
    d->resume_chunk = 0;
    d->firmware_size = 0;
    memset(d->md5, 0, sizeof(d->md5));
    d->cleared = 1;
    d->saved_flash_count++;
}

static void dev_prepare_tftp(device_t *d)
{
    uint32_t bytes = d->resume_chunk * OTA_PACKET_SIZE;
    uint32_t aligned = bytes;

    if ((bytes % 512u) != 0)
        aligned = bytes & ~4095u;

    if (d->resume_chunk == 0 || aligned == 0) {
        if (d->tftp_fresh_ready && d->handle_open && d->image_end == 0) {
            d->block_no = 1;
            d->use_raw = 0;
            return;
        }
        if (d->handle_open) {
            d->abort_count++;
            d->handle_open = 0;
        }
        d->ota_begin_count++;
        d->erase_count++;
        d->handle_open = 1;
        d->use_raw = 0;
        d->tftp_fresh_ready = 1;
        if (d->resume_chunk != 0) {
            d->resume_chunk = 0;
            d->saved_flash_count++;
        }
        d->block_no = 1;
        d->image_end = 0;
        d->next_offset = 0;
        return;
    }

    if (aligned != bytes && aligned < DEV_PART_SIZE) {
        uint32_t erase_len = (DEV_PART_SIZE - aligned) & ~4095u;
        if (erase_len > 0) {
            memset(d->image + aligned, 0xFF, erase_len);
            d->erase_count++;
        }
        d->resume_chunk = aligned / OTA_PACKET_SIZE;
        d->saved_flash_count++;
    }

    d->block_no = (uint16_t)((aligned / 512u) + 1u);
    d->image_end = aligned;
    d->next_offset = aligned;
    d->use_raw = 1;
    d->handle_open = 0;
    d->tftp_fresh_ready = 0;
}

void dev_reset(device_t *d)
{
    memset(d, 0, sizeof(*d));
    memset(d->image, 0xFF, sizeof(d->image));
    d->block_no = 1;
    d->finish_result = -1;
    dev_update_address(d, 0);
}

void dev_set_saved(device_t *d, uint32_t chunk, const uint8_t *prefix, unsigned prefix_len)
{
    unsigned n = prefix_len;
    if (n > DEV_PART_SIZE)
        n = DEV_PART_SIZE;
    d->resume_chunk = chunk;
    if (prefix != NULL && n > 0) {
        memcpy(d->image, prefix, n);
        d->image[0] = 0xE9;
        d->magic_at_zero = 0xE9;
    }
    d->image_end = chunk * OTA_PACKET_SIZE;
    d->next_offset = d->image_end;
    dev_update_address(d, (uint16_t)chunk);
}

int dev_status(device_t *d, uint8_t cmd)
{
    if (cmd == ISP_STATUS_IN_PROGRESS_1F) {
        d->com_update = 1;
        d->softap_suspend++;
        if (d->run_begin == 0) {
            if (d->resume_chunk == 0) {
                d->use_raw = 0;
                d->next_offset = 0;
            } else {
                d->use_raw = 1;
                d->next_offset = d->resume_chunk * OTA_PACKET_SIZE;
                d->handle_open = 0;
            }
            d->run_begin = 1;
        }
        dev_update_address(d, (uint16_t)d->resume_chunk);
        return 0;
    }

    if (cmd == ISP_CMD_CLEAR_SAVED_UPDATE) {
        dev_clear_progress(d);
        d->rebooted = 1;
        d->com_update = 0;
        return 0;
    }

    if (cmd == 0x3F) {
        d->next_offset = 0;
        d->use_raw = 0;
        d->com_update = 0;
        d->run_begin = 0;
        d->resume_chunk = 0;
        d->saved_flash_count++;
        if (d->tftp_fresh_ready && d->handle_open && d->image_end == 0) {
            d->block_no = 1;
            d->tftp_prepared = 1;
            return 0;
        }
        if (d->handle_open) {
            d->abort_count++;
            d->handle_open = 0;
        }
        d->ota_begin_count++;
        d->erase_count++;
        memset(d->image, 0xFF, sizeof(d->image));
        memset(d->write_stamp, 0, sizeof(d->write_stamp));
        d->handle_open = 1;
        d->tftp_prepared = 1;
        d->tftp_fresh_ready = 1;
        d->image_end = 0;
        d->block_no = 1;
        dev_update_address(d, 0);
        return 0;
    }

    if (cmd == 0x7F) {
        d->ignored_status++;
        return 0;
    }

    if (d->com_update && cmd == 1) {
        if (!d->use_raw) {
            if (!d->handle_open && d->ota_begin_count == 0)
                d->ota_begin_count++;
        } else if (d->image[0] != 0xE9) {
            d->finish_result = 100;
            return -1;
        }
        d->boot_set = 1;
        d->resume_chunk = 0;
        memset(d->md5, 0, sizeof(d->md5));
        d->firmware_size = 0;
        d->com_update = 0;
        d->run_begin = 0;
        d->next_offset = 0;
        d->finish_result = 400;
        d->saved_flash_count++;
        return 0;
    }

    d->ignored_status++;
    return 0;
}

int dev_rs485_packet(device_t *d, uint8_t addr_hi, uint8_t addr_lo, const uint8_t *data, int len)
{
    uint8_t prev_high = 0;
    uint8_t prev_low = 0;
    uint32_t byte_offset;
    int i;

    if (!d->com_update)
        return -1;
    if (len <= 0) {
        d->flash_errors++;
        return -1;
    }

    if (d->resume_chunk > 0) {
        uint16_t prev_byte_addr = (uint16_t)((d->resume_chunk - 1) * OTA_PACKET_SIZE);
        prev_high = (uint8_t)((prev_byte_addr >> 8) & 0xFF);
        prev_low = (uint8_t)(prev_byte_addr & 0xFF);
    }

    if (addr_hi == d->com_flash_high && addr_lo == d->com_flash_low) {
        byte_offset = d->resume_chunk * OTA_PACKET_SIZE;
        if (byte_offset + (uint32_t)len > DEV_PART_SIZE) {
            d->flash_errors++;
            return -1;
        }
        memcpy(d->image + byte_offset, data, (size_t)len);
        for (i = 0; i < len; i++)
            d->write_stamp[byte_offset + (uint32_t)i]++;
        d->next_offset = byte_offset + (uint32_t)len;
        if (d->use_raw)
            d->raw_write_count++;
        else
            d->ota_write_count++;
        d->flash_errors = 0;
        d->resume_chunk += 1;
        dev_update_address(d, (uint16_t)d->resume_chunk);
        if ((d->resume_chunk % 32u) == 0)
            d->saved_flash_count++;
        return 0;
    }

    if ((d->resume_chunk > 0) && (addr_hi == prev_high) && (addr_lo == prev_low)) {
        d->flash_errors = 0;
        return 1;
    }

    return -2;
}

void dev_dhcp_match(device_t *d)
{
    d->softap_suspend++;
    if (!d->tftp_prepared || (d->resume_chunk == 0 && d->block_no > 1)) {
        dev_prepare_tftp(d);
        d->dhcp_prepare_count++;
        d->tftp_prepared = 1;
    }
}

int dev_tftp_data(device_t *d, uint16_t block, const uint8_t *payload, int len)
{
    uint32_t write_offset;
    size_t write_len;
    uint32_t i;

    if (len < 0 || len > 512)
        return -1;

    if (block == 1 && d->resume_chunk == 0 && d->block_no > 1)
        dev_prepare_tftp(d);

    if (block > d->block_no) {
        d->last_tftp_error = 1;
        return -2;
    }
    if (block < d->block_no) {
        d->last_tftp_ack = (int)block;
        return 1;
    }

    write_offset = ((uint32_t)block - 1u) * 512u;
    write_len = (size_t)len;
    if (d->use_raw)
        write_len = (write_len + 15u) & ~15u;
    if (write_offset + write_len > DEV_PART_SIZE)
        return -1;

    memcpy(d->image + write_offset, payload, (size_t)len);
    if (d->use_raw && write_len > (size_t)len)
        memset(d->image + write_offset + (uint32_t)len, 0xFF, write_len - (size_t)len);
    for (i = 0; i < write_len; i++)
        d->write_stamp[write_offset + i]++;

    if (d->use_raw)
        d->raw_write_count++;
    else
        d->ota_write_count++;

    d->image_end = (((uint32_t)block - 1u) * 512u) + (uint32_t)len;
    d->block_no = block;
    if ((d->image_end % 512u) == 0) {
        d->resume_chunk = d->image_end / OTA_PACKET_SIZE;
        if ((d->image_end % 8192u) == 0)
            d->saved_flash_count++;
    }
    d->block_no = (uint16_t)(block + 1u);
    d->last_tftp_ack = (int)block;
    d->image[0] = (write_offset == 0 && len > 0) ? payload[0] : d->image[0];
    return 0;
}

void old_dev_reset(old_device_t *d)
{
    memset(d, 0, sizeof(*d));
    memset(d->image, 0xFF, sizeof(d->image));
}

void old_dev_apply_writes(old_device_t *d, const fw_write_t *writes, int count)
{
    int i;
    for (i = 0; i < count; i++) {
        if (writes[i].reg == ISP_REG_UPDATE_STATUS && writes[i].value == 0x7F)
            d->saw_7f = 1;
        else if (writes[i].reg == ISP_REG_UPDATE_STATUS && writes[i].value == 0x3F) {
            d->saw_3f = 1;
            d->erased = 1;
            d->offset = 0;
            memset(d->image, 0xFF, sizeof(d->image));
        } else if (writes[i].reg == ISP_REG_UPDATE_STATUS && writes[i].value == ISP_STATUS_IN_PROGRESS_1F)
            d->saw_1f = 1;
        else if (writes[i].reg == ISP_REG_UPDATE_STATUS && writes[i].value == ISP_CMD_CLEAR_SAVED_UPDATE)
            d->saw_33 = 1;
        else if (writes[i].reg == ISP_REG_RESUME_COUNT)
            d->saw_resume_read = 1;
        else if (writes[i].reg >= ISP_REG_MD5_START && writes[i].reg <= ISP_REG_FW_SIZE_LO)
            d->saw_md5 = 1;
    }
}

void old_dev_program(old_device_t *d, const uint8_t *image, unsigned len)
{
    if (len > DEV_PART_SIZE)
        len = DEV_PART_SIZE;
    memcpy(d->image, image, len);
    d->offset = (int)len;
}
