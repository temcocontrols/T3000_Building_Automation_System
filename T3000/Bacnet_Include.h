#pragma once

#include "address.h"
#include "abort.h"
#include "alarm_ack.h"
#include "apdu.h"
#include "arcnet.h"
#include "arf.h"
#include "awf.h"
#include "bacaddr.h"
#include "bacapp.h"
#include "bacdcode.h"
#include "bacdef.h"
#include "bacdevobjpropref.h"
#include "bacenum.h"
#include "bacerror.h"
#include "bacint.h"

#include "bacprop.h"
#include "bacpropstates.h"
#include "bacreal.h"
#include "bacstr.h"

#include "bactext.h"
#include "bigend.h"
//#include "bip.h"
#include "bits.h"
#include "bvlc.h"
#include "bytes.h"
#include "client.h"
#include "config.h"
#include "cov.h"
#include "crc.h"
//#include "datalink.h"
#include "datetime.h"
#include "dcc.h"
#include "debug.h"
#include "dlenv.h"
#include "dlmstp.h"
#include "ethernet.h"
#include "event.h"
#include "fifo.h"
#include "filename.h"
#include "get_alarm_sum.h"
#include "getevent.h"
#include "handlers.h"
#include "iam.h"
#include "ihave.h"
#include "indtext.h"

#include "key.h"
#include "keylist.h"
#include "lso.h"
#include "memcopy.h"
#include "mstp.h"
#include "mstpdef.h"
#include "mstptext.h"
#include "mydata.h"
#include "npdu.h"
#include "objects.h"
#include "proplist.h"
#include "ptransfer.h"
#include "rd.h"
#include "readrange.h"
#include "reject.h"
#include "ringbuf.h"
#include "rp.h"
#include "rpm.h"
#include "sbuf.h"
#include "timestamp.h"
#include "timesync.h"
#include "tsm.h"
//#include "txbuf.h"
#include "version.h"
#include "whohas.h"

#include "whois.h"
#include "wp.h"
#include "wpm.h"
#include "trendlog.h"
#include "stdbool.h"
#include "nc.h"
#include "msv.h"
#include "mso.h"
#include "ms-input.h"
#include "lsp.h"
#include "lo.h"
#include "lc.h"
#include "device.h"
#include "csv.h"
#include "bv.h"
#include "bo.h"
#include "bi.h"
#include "bacfile.h"
#include "av.h"
#include "ao.h"
#include "ai.h"
#include "h_pt_a.h"


#define BACNET_TYPE_NULL                      0
#define BACNET_TYPE_BOOLEAN                   1
#define BACNET_TYPE_UNSIGNED                  2
#define BACNET_TYPE_SIGNED                    3
#define BACNET_TYPE_REAL                      4
#define BACNET_TYPE_DOUBLE                    5
#define BACNET_TYPE_OCTET_STRING              6
#define BACNET_TYPE_CHARACTER_STRING          7
#define BACNET_TYPE_BIT_STRING                8
#define BACNET_TYPE_ENUMERATED                9
#define BACNET_TYPE_DATE                      10
#define BACNET_TYPE_TIME                      11
#define BACNET_TYPE_OBJECT_ID                 12

typedef struct bacnet_rp_info  //bacnet 相关的部分必须放在 bacnet include 的头文件中;//Fan
{
    unsigned int bacnet_instance;
    BACNET_OBJECT_TYPE object_type;
    uint32_t object_item_number;
    int property_id;
    BACNET_APPLICATION_DATA_VALUE value;
    int invoke_id;
}str_bacnet_rp_info;

typedef struct bacnet_standard_Info
{
    //CRelayLabel* control_pt;
    //HWND hWnd;
    uint32_t deviceid;
    int8_t standard_command;  //0  非标      1 标准;
    int object_type;
    uint32_t object_instance;
    int property_id;
    int lable_index;
    int last_resault; // -1 没读到   -2 无whois     1 数据正常；
    CString cs_value_show;  //显示出来的value 值;
    BACNET_APPLICATION_DATA_VALUE value;
}str_bacnet_screen_info;

//#include ""
//#include ""
//#include ""
//#include ""
//#include ""
//#include ""
//#include ""
//#include ""
//#include <stdint.h>
////#include <stdbool.h>
//INPUT int abort_encode_apdu(
//	uint8_t * apdu,
//	uint8_t invoke_id,
//	uint8_t abort_reason,
//	bool server);
//
//INPUT int abort_decode_service_request(
//	uint8_t * apdu,
//	unsigned apdu_len,
//	uint8_t * invoke_id,
//	uint8_t * abort_reason);