#ifndef VM_SHIM_H
#define VM_SHIM_H

// vm_shim.h — Adapter layer for decode.c
// Replaces embedded headers (bac_point.h, basic.h, user_data.h, alarm.h)
// Provides all types, macros, globals, and function declarations that decode.c needs.
//
// CONFLICT RESOLUTION STRATEGY (VM_ prefix):
//   Several types that decode.c needs (UN_Time, Point, Point_Net, Alarm_point,
//   Str_points_ptr, Station_NUM) have different layouts than MFC's同名 types.
//   We define them here with VM_ prefix, and VirtualProgramEngine.cpp
//   #defines the original names to the VM_ names only during the #include "decode.c"
//   phase. This ensures decode.c sees the correct embedded layout while MFC
//   code elsewhere is unaffected.

#pragma once

#include <windows.h>
#include <cstdint>
#include <cstring>

//=============================================================================
// Undef conflicting Windows SDK / C++ macro definitions
// decode.c enum values conflict with Windows SDK macros (MAX, MIN, RGB, etc.)
// We push/undef before enum definitions, then pop after.
//=============================================================================
#ifdef MAX
#pragma push_macro("MAX")
#undef MAX
#define VM_NEED_POP_MAX
#endif
#ifdef MIN
#pragma push_macro("MIN")
#undef MIN
#define VM_NEED_POP_MIN
#endif
#ifdef RGB
#pragma push_macro("RGB")
#undef RGB
#define VM_NEED_POP_RGB
#endif
#ifdef AND
#pragma push_macro("AND")
#undef AND
#define VM_NEED_POP_AND
#endif
#ifdef OR
#pragma push_macro("OR")
#undef OR
#define VM_NEED_POP_OR
#endif
#ifdef NOT
#pragma push_macro("NOT")
#undef NOT
#define VM_NEED_POP_NOT
#endif
#ifdef XOR
#pragma push_macro("XOR")
#undef XOR
#define VM_NEED_POP_XOR
#endif
#ifdef MOD
#pragma push_macro("MOD")
#undef MOD
#define VM_NEED_POP_MOD
#endif
#ifdef DIV
#pragma push_macro("DIV")
#undef DIV
#define VM_NEED_POP_DIV
#endif
#ifdef DELETE
#pragma push_macro("DELETE")
#undef DELETE
#define VM_NEED_POP_DELETE
#endif
#ifdef CREATE
#pragma push_macro("CREATE")
#undef CREATE
#define VM_NEED_POP_CREATE
#endif
#ifdef OPEN
#pragma push_macro("OPEN")
#undef OPEN
#define VM_NEED_POP_OPEN
#endif
#ifdef CLOSE
#pragma push_macro("CLOSE")
#undef CLOSE
#define VM_NEED_POP_CLOSE
#endif
#ifdef STOP
#pragma push_macro("STOP")
#undef STOP
#define VM_NEED_POP_STOP
#endif
#ifdef WAIT
#pragma push_macro("WAIT")
#undef WAIT
#define VM_NEED_POP_WAIT
#endif

// DATE and INT conflict with Windows SDK typedefs.
// Solution: We use VM_DATE and VM_INT in the enum, and VirtualProgramEngine.cpp
// maps them back via #define DATE VM_DATE / #define INT VM_INT during decode.c inclusion.

//=============================================================================
// Type definitions (from embedded types.h / ud_str.h)
// VM_ prefix avoids collision with MFC types of the same name.
// VirtualProgramEngine.cpp maps the original names to these via #define
// only during the #include "decode.c" phase.
//=============================================================================
typedef uint8_t  U8_T;
typedef int8_t   S8_T;
typedef uint16_t U16_T;
typedef int16_t  S16_T;
typedef uint32_t U32_T;
typedef long     S32_T;  // Use 'long' (not int32_t) for Windows compatibility with decode.c long* usage
typedef uint32_t u32;
typedef uint8_t  Byte;

// Float <-> unsigned long union (same memory layout)
typedef union {
    float  fdata;
    unsigned long ldata;
} FloatLongType;

// Time structure — VM_ prefix (MFC has a different UN_Time layout)
// Embedded uses union with Clk sub-structure for Rtc.Clk.week, Rtc.Clk.day, etc.
typedef union {
    struct {
        U8_T sec;
        U8_T min;
        U8_T hour;
        U8_T day;
        U8_T week;
        U8_T mon;
        U8_T year;
        U16_T day_of_year;
        S8_T is_dst;
    } Clk;
    U8_T all[10];
} VM_UN_Time;

// Point address struct (2 bytes, packed) — VM_ prefix (MFC Point_T3000 is 3 bytes)
// decode.c uses this to call get_point_value / put_point_value.
#pragma pack(push)
#pragma pack(1)
typedef struct {
    unsigned char number;
    unsigned char point_type;
} VM_Point;
#pragma pack(pop)

// Network point address struct (5 bytes, packed) — VM_ prefix
// decode.c uses: point.sub_id, point.network_number, point.panel, point.number, point.point_type
#pragma pack(push)
#pragma pack(1)
typedef struct {
    unsigned char number;
    unsigned char point_type;
    unsigned char panel;
    unsigned char sub_id;          // NOT sub_panel!
    unsigned char network_number;  // NOT network!
} VM_Point_Net;
#pragma pack(pop)

// Alarm point struct — VM_ prefix (MFC may have a different layout)
#pragma pack(push)
#pragma pack(1)
typedef struct {
    U8_T  panel;
    U8_T  cmd;
    U8_T  index;
    U8_T  del;
    U32_T alarm_time;
    S8_T  alarm_count;
    S8_T  alarm_message[81];
    U8_T  reserved[10];
} VM_Alarm_point;
#pragma pack(pop)

// Alarm panel list
#pragma pack(push)
#pragma pack(1)
typedef struct {
    U8_T  num;
    U8_T  list[8];
} Alarm_panel_list;
#pragma pack(pop)

// Inputs array (referenced by SENSOR_ON/SENSOR_OFF and VM_Str_points_ptr)
// Must be defined BEFORE VM_Str_points_ptr uses Input_s*
struct Input_s {
    U8_T in_sen_on;
    U8_T in_sen_off;
};
extern Input_s inputs[64];

// Str_points_ptr — VM_ prefix (MFC has a different union layout)
// decode.c uses: sptr.pin = &inputs[...]; sptr.pin->in_sen_on, sptr.pin->in_sen_off
// Input_s is defined above (referenced by this type).
#pragma pack(push)
#pragma pack(1)
typedef struct {
    Input_s *pin;   // Points to inputs array entries
} VM_Str_points_ptr;
#pragma pack(pop)

//=============================================================================
// Basic macros and constants (from embedded basic.h)
//=============================================================================
#define DIGITAL             1
#define ANALOG             0
#define PROG_SIZE           2000
#define LOCAL               1
#define LOCAL_VAR           1
#define POINT_VAR           2
#define POINT_REMOTE_VAR    3
#define LABEL_VAR           4
#define LOCAL_POINT_PRG     0x09C
#define CONST_VALUE_PRG     0x09D
#define REMOTE_POINT_PRG    0x09E
#define ALL_PRINT           1
#define ONLY_PRINT          2
#define DISCONNECT_PRINT    0
#define On                  1
#define OFF                 0
#define MAN                 1
#define AUTO                0
#define ANA                 0
#define DIG                 1
#define PROGR               0
#define OPERATOR            1
#define LOCAL_VARIABLE      0x82
#define FLOAT_TYPE          0x83
#define LONG_TYPE           0x84
#define INTEGER_TYPE        0x85
#define BYTE_TYPE           0x86
#define STRING_TYPE         0x87
#define FLOAT_TYPE_ARRAY    0x88
#define LONG_TYPE_ARRAY     0x89
#define INTEGER_TYPE_ARRAY  0x8A
#define BYTE_TYPE_ARRAY     0x8B
#define STRING_TYPE_ARRAY   0x8C

// Alarm constants
#define VIRTUAL_ALARM       0
#define TEMPERATURE         1
#define GENERAL_ALARM       2
#define PRINTER_ALARM       10

// Point type: embedded +1 encoding (OUT=0+1=1, IN=1+1=2, VAR=2+1=3, PRG=4+1=5)
// NOTE: The #define values are +1 encoded (matching Point.point_type in bytecode).
// get_point_value/put_point_value subtract 1 before comparing with these 0-based constants:
#define PT_OUTPUT           0
#define PT_INPUT            1
#define PT_VARIABLE         2
// Original +1 values for reference / compatibility:
#define POINT_TYPE_OUTPUT   1
#define POINT_TYPE_INPUT    2
#define POINT_TYPE_VARIABLE 3
#define POINT_TYPE_PROGRAM  5

// Float byte order constants (used by Float_to_Byte, Byte_to_Float)
#define FLOAT_TYPE_ABCD     0
#define FLOAT_TYPE_CDAB     1
#define FLOAT_TYPE_BADC     2
#define FLOAT_TYPE_DCBA     3

// Long time test constant (decode.c line 1257)
#define LONGTIMETEST        0x7FFFFFFF

// Byte extraction macros (decode.c lines 1388-1389, 1417-1418)
#define LOW_BYTE(w)         ((U8_T)((w) & 0xFF))
#define HIGH_BYTE(w)        ((U8_T)((w) >> 8))

// Point type enum value for VAR (decode.c line 1389)
#define VAR                 2  // ENUM_VAR

// Modbus coil register type (decode.c line 1418)
#define MB_COIL_REG         10

//=============================================================================
// Token type enum (tok_types)
//=============================================================================
enum tok_types {
    DELIMITER, NUMBER, IDENTIFIER, KEYWORD, TEMP, BLOCKX, TIMES, COLON, SEMICOLON,
    GE=0x60, LE=0x61, NL=0x62, POW=0x65, MOD=0x66, MUL=0x67, DIV=0x68, MINUSUNAR=0x69, PLUS=0x6B,
    MINUS=0x6C, LT=0x6D, GT=0x6E,
    EQ=0x71, NE=0x72, XOR=0x73, AND=0x74, OR=0x75, NOT=0x76, EOI=0x77, TTIME=0x78, BITAND=0x79,
    BITOR=0x7a,INTDIV=0x7b
};

//=============================================================================
// Command token enum (tokens)
// NOTE: DATE is defined here as an enum value (0x0A0). Windows SDK also
// defines DATE as a typedef. We undef DATE before this enum and restore after.
//=============================================================================
enum tokens {
    BEEP=0x07, ASSIGNAR=0x08, ASSIGN=0x09, CLEARX=0x0A, FOR=0x0B, NEXT=0x0D, IF=0x0E,
    ELSE=0x10, IFP=0x11, IFM=0x12, GOTO=0x13, GOSUB=0x14, RETURN=0x15, ENDPRG=0x17, PRINT=0x18,
    REM=0x1A, PRINT_AT=0x1B, STARTPRG=0x1C, STOP=0x1D, WAIT=0x1E, HANGUP=0x1F,
    PHONE=0x20, ALARM_AT=0x21, REMOTE_SET=0x22, RUN_MACRO=0x23, REMOTE_GET=0x24,
    ENABLEX=0x25, DISABLEX=0x26, ON_ERROR=0x27, SET_PRINTER=0x28, ASSIGNARRAY_1=0x29, GOTOIF=0x2A,
    ON_ALARM=0x2B, ASSIGNARRAY_2=0x2C, OPEN=0x2D, CLOSE=0x2E, CALLB=0x2F,
    DECLARE=0x30, ASSIGNARRAY=0x31,
    ON=0x40, Alarm=0x41, DALARM=0x43, USER_A=0x45, USER_B=0x46,
    DIM=0x8A,
    VM_DATE=0x0A0, PTIME=0x0A1, SENSOR_ON=0x0A4, SENSOR_OFF=0x0A5, TO=0x0A6, STEP=0x0A7, THEN=0x0A8,
    LET=0x0A9, ALARM_AT_ALL=0x0AA, FINISHED=0x0AB, PRINT_ALARM_AT=0x0AC, PRINT_ALARM_AT_ALL=0x0AD,
    ARG=0x0AE, DO=0x0AF, WHILE=0x0B0, SWITCH=0x0B1, EOL=0x0B2, STRING=0x0B3,
    ICON1=0xb4, ICON2=0xb5, ICON3=0xb6, ICON4=0xb7
};

//=============================================================================
// Function token enum (tok_func)
// NOTE: INT is defined here as an enum value (0x36). Windows SDK also
// defines INT as a typedef. We undef INT before this enum and restore after.
//=============================================================================
enum tok_func {
    SUN=7, MON=8, TUE=2, WED=3, THU=4, FRI=5, SAT=6,
    ABS=0x32, AVG=0x33, DOY=0x34, DOW=0x35, VM_INT=0x36, MAX=0x37, MIN=0x38, SQR=0x39, Tbl=0x3A,
    TIME=0x3B, TIME_ON=0x3C, TIME_OFF=0x3D, INTERVAL=0x3E, TIME_FORMAT=0x3F,
    WR_ON=0x40, WR_OFF=0x41, UNACK=0x42, VM_Status=0x47, RUNTIME=0x48, SCANS=0x4B, POWER_LOSS=0x4C,
    LN=0x4D, LN_1=0x4E,
    OUTPUTD=0x50, INKEYD=0x51, DOM=0x52, MOY=0x53, PIDPROP=0x56, PIDDERIV=0x57, PIDINT=0x58, CLEARPORT=0x59,
    JAN=0x0C1, FEB=0x0C2, MAR=0x0C3, APR=0x0C4, MAYM=0x0C5, JUN=0x0C6, JUL=0x0C7, AUG=0x0C8,
    SEP=0x0C9, OCT=0x0CA, NOV=0x0CB, DEC=0x0CC, COM1=0x10, MB_BW=0x0CE, MB_BW_COIL=0x0CF,
    SIN=0x0D1, COS=0x0D2, RGB=0x0D3
};

//=============================================================================
// Restore conflicting Windows SDK / C++ macro definitions
//=============================================================================
#ifdef VM_NEED_POP_MAX
#pragma pop_macro("MAX")
#undef VM_NEED_POP_MAX
#endif
#ifdef VM_NEED_POP_MIN
#pragma pop_macro("MIN")
#undef VM_NEED_POP_MIN
#endif
#ifdef VM_NEED_POP_RGB
#pragma pop_macro("RGB")
#undef VM_NEED_POP_RGB
#endif
#ifdef VM_NEED_POP_AND
#pragma pop_macro("AND")
#undef VM_NEED_POP_AND
#endif
#ifdef VM_NEED_POP_OR
#pragma pop_macro("OR")
#undef VM_NEED_POP_OR
#endif
#ifdef VM_NEED_POP_NOT
#pragma pop_macro("NOT")
#undef VM_NEED_POP_NOT
#endif
#ifdef VM_NEED_POP_XOR
#pragma pop_macro("XOR")
#undef VM_NEED_POP_XOR
#endif
#ifdef VM_NEED_POP_MOD
#pragma pop_macro("MOD")
#undef VM_NEED_POP_MOD
#endif
#ifdef VM_NEED_POP_DIV
#pragma pop_macro("DIV")
#undef VM_NEED_POP_DIV
#endif
#ifdef VM_NEED_POP_DELETE
#pragma pop_macro("DELETE")
#undef VM_NEED_POP_DELETE
#endif
#ifdef VM_NEED_POP_CREATE
#pragma pop_macro("CREATE")
#undef VM_NEED_POP_CREATE
#endif
#ifdef VM_NEED_POP_OPEN
#pragma pop_macro("OPEN")
#undef VM_NEED_POP_OPEN
#endif
#ifdef VM_NEED_POP_CLOSE
#pragma pop_macro("CLOSE")
#undef VM_NEED_POP_CLOSE
#endif
#ifdef VM_NEED_POP_STOP
#pragma pop_macro("STOP")
#undef VM_NEED_POP_STOP
#endif
#ifdef VM_NEED_POP_WAIT
#pragma pop_macro("WAIT")
#undef VM_NEED_POP_WAIT
#endif
// NOTE: DATE and INT are typedefs (not macros), so they cannot be restored
// via pop_macro. They remain undefined after this point.
// This is intentional — we redefine them as enum values below.

//=============================================================================
// Struct/typedef definitions needed before global variable declarations
// These must be outside extern "C" block (they are types, not variables)
//=============================================================================

// Remote panel list (referenced by decode.c Status function)
#pragma pack(push)
#pragma pack(1)
typedef struct {
    U8_T panel;
    U8_T sub_id;
} Remote_panel_db_item;
#pragma pack(pop)

// Array pointers (not supported in MFC VM, provide stubs)
struct ArrayInfo { int length; };

// Controller array (referenced by PID)
#pragma pack(push)
#pragma pack(1)
typedef struct {
    U8_T  auto_manual;
    char  proportional;
    char  prop_high;
    char  rate;
    char  reset;
    S32_T value;
} Controller_item;
#pragma pack(pop)

// Schedule times (referenced by WR_ON/WR_OFF)
#define MAX_SCHEDULES_PER_WEEK 9
#pragma pack(push)
#pragma pack(1)
typedef struct {
    struct {
        U8_T hours;
        U8_T minutes;
    } time[MAX_SCHEDULES_PER_WEEK];
} Wr_time_item;
#pragma pack(pop)

// Serial port struct (referenced by COM1)
#pragma pack(push)
#pragma pack(1)
typedef struct {
    U8_T cmd[10];
    U8_T type;
    U16_T baut;
    U8_T len;
    U16_T res;
} RS232_Cmd;
#pragma pack(pop)

//=============================================================================
// External global variables (provided by VirtualProgramEngine.cpp)
// These must be declared before including decode.c.
// Wrapped in extern "C" because decode.c is compiled with C linkage.
// VM_ prefix types match the definitions above.
//=============================================================================
extern "C" {

extern uint8_t current_online[32];
extern VM_UN_Time Rtc;
extern U16_T Test[50];

extern uint8_t flag_start_scan_network;
extern uint8_t start_scan_network_count;
extern uint16_t scan_network_bacnet_count;
extern uint8_t flag_start_scan_mstp;
extern uint8_t start_scan_mstp_count;

extern uint32_t net_health[4];
extern uint16_t current_page;

// decode.c execution globals
extern U8_T* prog;
extern S32_T vm_stack[256];
extern S32_T* index_stack;
extern int cond;
extern S32_T op1, op2;
extern S32_T* pn;

// Timing globals
extern S8_T  just_load;
extern U32_T miliseclast_cur;
extern U32_T miliseclast;
extern U32_T system_timer;
extern U8_T* time_buf;

// Alarm globals
extern U8_T  alarm_flag;
extern U8_T  new_alarm_flag;
extern S8_T  ind_alarm_panel;
extern S8_T  alarm_panel[32];
extern U8_T  alarm_at_all;
extern S8_T  message[128];
extern U8_T  VM_Station_NUM;

// decode.c global value/v variables (remapped via #define in VirtualProgramEngine.cpp)
extern S32_T vm_value_global;
extern S32_T vm_v_global;

// Remote panel list
extern U16_T remote_panel_num;
extern Remote_panel_db_item remote_panel_db[32];

// Panel number for Modbus operations
extern U8_T panel_number;

// ICON config
extern U8_T icon_config;

// Array pointers
extern ArrayInfo arrays[10];
extern long* arrays_address[10];

// Controller array
extern Controller_item controllers[32];

// Schedule times
extern Wr_time_item wr_times[8][7];

// Serial port struct
extern RS232_Cmd rs232_cmd;

} // extern "C"

//=============================================================================
// Function declarations
// All point-handling functions use VM_ prefix types to match decode.c calls
// after VirtualProgramEngine.cpp applies the #define remapping.
//=============================================================================
// get_point_value / put_point_value: core point I/O (implemented in VirtualProgramEngine.cpp)
extern "C" S16_T get_point_value(VM_Point *point, S32_T *val_ptr);
extern "C" S16_T put_point_value(VM_Point *point, S32_T *val_ptr, S16_T aux, S16_T prog_op);

// Remote/network point I/O (stub implementations)
extern "C" S16_T put_net_point_value(VM_Point_Net *point, S32_T *val_ptr, S16_T aux, S16_T prog_op, U8_T mode);
extern "C" S16_T get_net_point_value(VM_Point_Net *p, S32_T *val_ptr, U8_T mode, U8_T flag);

// Alarm generation (stub implementations)
extern "C" S16_T generatealarm(S8_T *mes, S16_T prg, S16_T panel, S16_T type, S8_T alarmatall,
                    S8_T indalarmpanel, S8_T *alarmpanel, S8_T printalarm);
extern "C" void generate_program_alarm(U8_T type, U8_T prg);

// Local variable operations (implemented in decode.c)
extern "C" void put_local_var(U8_T *p, S32_T value, U8_T *local);
extern "C" S32_T localvalue(U8_T *p, U8_T *local);
extern "C" S16_T put_local_array(U8_T *p, S32_T value, S32_T v1, S32_T v2, U8_T *local);
extern "C" void get_ay_elem(S32_T *value, U8_T *local);  // Referenced by operand() in decode.c

// Expression evaluation and stack (implemented in decode.c)
extern "C" S32_T veval_exp(U8_T *local);
extern "C" S32_T operand(U8_T **buf, U8_T *local);
extern "C" void push(S32_T value);
extern "C" S32_T pop(void);
extern "C" void pushlong(unsigned long value);
extern "C" unsigned long poplong(void);

// Icon config (stub)
extern "C" U8_T Get_Mini_Type(void);
extern "C" void Set_icon_config(U8_T icon_config);

// rs232 send (stub for COM1)
extern "C" void send_rs232_command(void);

// exec_program: the main entry point from decode.c
// Returns: 0=normal end, 1=ENDPRG, -1=error, <2000=running
extern "C" S16_T exec_program(S16_T current_prg, U8_T *prog_code);

#endif // VM_SHIM_H
