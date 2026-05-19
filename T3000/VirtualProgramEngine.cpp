#include "stdafx.h"
#include "T3000.h"
#include "global_function.h"
#include "global_define.h"

#include "VirtualProgramEngine.h"

//=============================================================================
// VM_DEFAULT_SCAN_MS: default scan interval in milliseconds
//=============================================================================
#ifndef VM_DEFAULT_SCAN_MS
#define VM_DEFAULT_SCAN_MS 1000
#endif

//=============================================================================
// decode.c integration: include the embedded engine
// It will use vm_shim.h for all its dependencies instead of embedded headers.
//
// STRATEGY: Before including decode.c, we:
//   1. Undef Windows SDK / MFC macros that conflict with decode.c enum values
//   2. Define the original type names as aliases to our VM_ prefixed types
//      (so decode.c — which uses UN_Time, Point, Point_Net, etc. — sees
//       the correct embedded layout rather than MFC's conflicting types)
//   3. Skip decode.c's own #include of missing headers (bac_point.h, etc.)
//
// After #include "decode.c", we restore everything.
//=============================================================================

// Skip decode.c's own #include directives for missing embedded headers
// (these are replaced by vm_shim.h)
#define BAC_POINT_H
#define BASIC_H
#define USER_DATA_H
#define ALARM_H

// Undef conflicting Windows SDK / MFC macro definitions
// These must be gone before the #include "decode.c" so the enum values
// (MAX, MIN, RGB, DATE, INT, etc.) can be defined without conflict.
#ifdef MAX
#undef MAX
#endif
#ifdef MIN
#undef MIN
#endif
#ifdef RGB
#undef RGB
#endif
#ifdef AND
#undef AND
#endif
#ifdef OR
#undef OR
#endif
#ifdef NOT
#undef NOT
#endif
#ifdef XOR
#undef XOR
#endif
#ifdef MOD
#undef MOD
#endif
#ifdef DIV
#undef DIV
#endif
#ifdef DELETE
#undef DELETE
#endif
#ifdef CREATE
#undef CREATE
#endif
#ifdef OPEN
#undef OPEN
#endif
#ifdef CLOSE
#undef CLOSE
#endif
#ifdef STOP
#undef STOP
#endif
#ifdef WAIT
#undef WAIT
#endif
// Windows SDK also typedef's DATE and INT — undef them so the enum
// values in vm_shim.h's tokens / tok_func enums can compile.
// NOTE: typedefs cannot be restored, but that's fine since decode.c
// compiles in its own namespace.
#ifdef DATE
#undef DATE
#endif
#ifdef INT
#undef INT
#endif

// Undef conflicting type/function names
// stack: decode.c uses a global S32_T stack[256], which conflicts with std::stack
// We rename it to vm_stack during decode.c compilation
#define stack vm_stack
#undef Status  // Windows SDK has a Status function; decode.c has a tok_func enum value
#define Status VM_Status  // Map decode.c's Status enum value to VM_Status

// decode.c operand() uses global 'value' variable (not declared locally)
// decode.c MAX/MIN cases use global 'v' as temp variable (not declared locally)
// Map them to our vm_value_global / vm_v_global
#define value vm_value_global
#define v vm_v_global

// Map enum values that conflict with Windows typedefs
#define DATE   VM_DATE
#define INT    VM_INT

// Also undef any MFC type names that would conflict with VM_ prefixed types.
// NOTE: In MSVC, #ifdef CAN detect typedef names (treated as preprocessor symbols).
// But if that fails, we simply #undef unconditionally (will be a no-op if not defined).
// We guard with #ifndef to avoid "undefined macro" warnings.
#ifndef Point
#define Point  VM_Point
#endif
#ifndef UN_Time
#define UN_Time VM_UN_Time
#endif
#ifndef Point_Net
#define Point_Net VM_Point_Net
#endif
#ifndef Alarm_point
#define Alarm_point VM_Alarm_point
#endif
#ifndef Str_points_ptr
#define Str_points_ptr VM_Str_points_ptr
#endif
#ifndef Station_NUM
#define Station_NUM VM_Station_NUM
#endif

// Now include decode.c — it will see Point==VM_Point, UN_Time==VM_UN_Time, etc.
// Wrap in extern "C" so decode.c's function definitions get C linkage,
// matching the extern "C" declarations in vm_shim.h.
extern "C" {
#include "decode.c"
}

// Restore the preprocessor namespace — undo our type remapping macros.
// After this point, Point/UN_Time/etc. no longer expand to VM_ versions.
// We use UNCONDITIONAL #undef (not #ifndef guard):
//   - #ifndef guard FAILS when the original #define was skipped because Point was already defined
//     by MFC headers. Then Point remains defined, and #ifndef Point is FALSE, so #undef is skipped.
//   - Unconditional #undef is always safe: it's a no-op if the macro was never defined.
#undef Point
#undef UN_Time
#undef Point_Net
#undef Alarm_point
#undef Str_points_ptr
#undef Station_NUM
#undef stack
#undef Status
#undef DATE
#undef INT
#undef value
#undef v

//=============================================================================
// Global window HWND declarations
// HWNDs maintained by main window (BacnetView), VM notifies via PostMessage
// Note: No longer depends on dialog class definitions
//=============================================================================
#ifndef _VM_EXTERN_HWNDS
#define _VM_EXTERN_HWNDS
extern HWND m_input_dlg_hwnd;       // CBacnetInput window handle
extern HWND m_output_dlg_hwnd;      // CBacnetOutput window handle
extern HWND m_variable_dlg_hwnd;    // CBacnetVariable window handle
extern HWND m_pragram_dlg_hwnd;     // CBacnetProgram window handle (note: pragram not program)
#endif

//=============================================================================
// Global data vector declarations (defined in main app, declared in global_variable_extern.h)
// These are the same extern globals used throughout T3000. The VM reads/writes
// them directly so it stays in sync with the rest of the application.
//=============================================================================
extern vector<Str_in_point> m_Input_data;
extern vector<Str_out_point> m_Output_data;
extern vector<Str_variable_point> m_Variable_data;
extern vector<Str_program_point> m_Program_data;
extern vector<Str_controller_point> m_controller_data;
extern vector<Str_weekly_routine_point> m_Weekly_data;
extern vector<Str_annual_routine_point> m_Annual_data;
extern vector<Str_variable_point> m_pvar_data;

//=============================================================================
// REFRESH_ON_ITEM constant default value (use 0 if not defined)
//=============================================================================
#ifndef REFRESH_ON_ITEM
#define REFRESH_ON_ITEM 0
#endif

//=============================================================================
// decode.c requires several global variables that we provide here.
// These are declared as extern in vm_shim.h.
// VM_ prefix types match the VM_ prefixed type names defined in vm_shim.h.
//=============================================================================

// decode.c execution globals (managed per-scan by ExecuteProgram)
// NOTE: stack renamed to vm_stack to avoid conflict with std::stack
U8_T* prog = NULL;                     // Current bytecode position pointer
S32_T vm_stack[256];                   // Expression evaluation stack (renamed from stack)
S32_T* index_stack = NULL;             // Stack pointer (= vm_stack)
int cond = 0;                           // Last condition result
S32_T op1 = 0, op2 = 0;               // Binary op operands
S32_T* pn = NULL;                       // Time table pointer

// Timing globals — just_load, miliseclast_cur, miliseclast are defined in decode_vm.inc
// (they are defined with initializers inside decode.c, so we must not redefine them here)

// system_timer: mapped to GetTickCount()/10 to match embedded 10ms tick
// Updated at the start of each ExecuteOneScan call.
U32_T system_timer = 0;

// time_buf: pointer to time data buffer (decode.c: time_buf = (U8_T*)prog)
U8_T* time_buf = NULL;

// Alarm globals
U8_T  alarm_flag = 0;
U8_T  new_alarm_flag = 0;
S8_T  ind_alarm_panel = 0;
S8_T  alarm_panel[32];  // Changed from Alarm_panel_list to S8_T[32]
U8_T  alarm_at_all = OFF;

// Alarm message buffer (decode.c writes alarm messages here)
S8_T  message[128];

// Panel number (station number)
// NOTE: VM_ prefix to avoid conflict with MFC's int Station_NUM (global_variable_extern.h).
// In ExecuteProgram we sync it: VM_Station_NUM = (U8_T)Station_NUM;
U8_T  VM_Station_NUM = 1;

// Remote panel list (stub — used by Status function)
U16_T remote_panel_num = 0;
Remote_panel_db_item remote_panel_db[32] = {};

// Panel number for Modbus operations
U8_T panel_number = 1;

// ICON config — defined in decode_vm.inc (icon_config is defined with initializer in decode.c)
// U8_T icon_config = 0; // REMOVED: duplicate with decode_vm.inc

// Array pointers (not supported in MFC VM, provide empty stubs)
ArrayInfo arrays[10] = {};
long* arrays_address[10] = {};

// Inputs array (referenced by SENSOR_ON/SENSOR_OFF — stub)
Input_s inputs[64] = {};

// Controller array (referenced by PID functions)
Controller_item controllers[32] = {};

// Schedule times (referenced by WR_ON/WR_OFF — stub)
Wr_time_item wr_times[8][7] = {};

// Serial port struct (referenced by COM1)
RS232_Cmd rs232_cmd = {};

// Misc embedded externals (stub)
U8_T  current_online[32] = {};
VM_UN_Time Rtc = {};                    // NOTE: VM_UN_Time (decode.c layout), not MFC's UN_Time
U16_T Test[50] = {};
uint8_t flag_start_scan_network = 0;
uint8_t start_scan_network_count = 0;
uint16_t scan_network_bacnet_count = 0;
uint8_t flag_start_scan_mstp = 0;
uint8_t start_scan_mstp_count = 0;
uint32_t net_health[4] = {};
uint16_t current_page = 0;

//=============================================================================
// get_point_value — read a point value into *val_ptr
// T3000 compiler and MFC both use ×100 fixed-point for analog values.
// decode.c uses the same ×100 scale — no conversion needed.
// Digital values: 0 or 1, stored as 0 or 1000 for decode.c compatibility.
// point->point_type is +1 encoded: 1=OUT, 2=IN, 3=VAR
// Returns: 0 on success
//=============================================================================
S16_T get_point_value(VM_Point *point, S32_T *val_ptr)
{
    if (!point || !val_ptr) return -1;

    // Decode: embedded uses +1 encoding
    unsigned char pt = point->point_type - 1;  // 0=OUT, 1=IN, 2=VAR
    int index = point->number;

    TRACE("[VM] get_point_value: pt=%d idx=%d\n", (int)pt, index);

    switch (pt) {
        case PT_OUTPUT:  // 0 = OUTPUT (point_type was 1, minus 1 = 0)
            if (index >= 0 && index < (int)m_Output_data.size()) {
                if (m_Output_data[index].digital_analog == 1) {
                    // Analog: MFC ×100 = decode.c ×100, no conversion needed
                    *val_ptr = (S32_T)m_Output_data[index].value;
                } else {
                    // Digital: return 0 or 1 for decode.c (NOT uses C ! operator)
                    *val_ptr = m_Output_data[index].value ? 1 : 0;
                }
                return 0;
            }
            break;

        case PT_INPUT:   // 1 = INPUT (point_type was 2, minus 1 = 1)
            if (index >= 0 && index < (int)m_Input_data.size()) {
                if (m_Input_data[index].digital_analog == 1) {
                    // Analog: MFC ×100 = decode.c ×100, no conversion needed
                    *val_ptr = (S32_T)m_Input_data[index].value;
                } else {
                    // Digital: return 0 or 1 for decode.c (NOT uses C ! operator)
                    *val_ptr = m_Input_data[index].value ? 1 : 0;
                }
                return 0;
            }
            break;

        case PT_VARIABLE:  // 2 = VARIABLE (point_type was 3, minus 1 = 2)
            if (index >= 0 && index < (int)m_Variable_data.size()) {
                if (m_Variable_data[index].digital_analog == 1) {
                    // Analog: MFC ×100 = decode.c ×100, no conversion needed
                    *val_ptr = (S32_T)m_Variable_data[index].value;
                } else {
                    // Digital: return 0 or 1 for decode.c (NOT uses C ! operator)
                    *val_ptr = m_Variable_data[index].value ? 1 : 0;
                }
                return 0;
            }
            break;

        default:
            // Unrecognized point type
            *val_ptr = 0;
            return -1;
    }

    *val_ptr = 0;
    return -1;
}

//=============================================================================
// put_point_value — write *val_ptr to a point
// T3000 compiler and MFC both use ×100 fixed-point for analog values.
// decode.c uses the same ×100 scale — no conversion needed.
// Digital values: decode.c uses 0/1000, convert to MFC's 0/1.
// point->point_type is +1 encoded: 1=OUT, 2=IN, 3=VAR
// aux: 0=digital, 1=analog (ANALOG/DIGITAL from decode.c)
// prog_op: PROGR=0, OPERATOR=1
// Returns: 0 on success
//=============================================================================
S16_T put_point_value(VM_Point *point, S32_T *val_ptr, S16_T aux, S16_T prog_op)
{
    if (!point || !val_ptr) return -1;

    // Decode: embedded uses +1 encoding
    unsigned char pt = point->point_type - 1;  // 0=OUT, 1=IN, 2=VAR
    int index = point->number;
    S32_T val = *val_ptr;

    TRACE("[VM] put_point_value: pt=%d idx=%d val=%ld aux=%d\n", (int)pt, index, (long)val, (int)aux);

    switch (pt) {
        case PT_OUTPUT:  // 0 = OUTPUT
            if (index >= 0 && index < (int)m_Output_data.size()) {
                if (m_Output_data[index].digital_analog == 1) {
                    // Analog point: write value directly
                    m_Output_data[index].value = val;
                } else {
                    // Digital point: set control to reflect new state
                    // decode.c's ASSIGN always passes aux=ANALOG, but for digital
                    // points we must update control (which the UI displays)
                    m_Output_data[index].value = (val != 0) ? 1 : 0;
                    m_Output_data[index].control = (val != 0) ? 1 : 0;
                }
                // Notify UI via PostMessage
                if (m_output_dlg_hwnd) {
                    ::PostMessage(m_output_dlg_hwnd, WM_REFRESH_BAC_OUTPUT_LIST, REFRESH_ON_ITEM, index);
                }
                return 0;
            }
            break;

        case PT_INPUT:   // 1 = INPUT
            if (index >= 0 && index < (int)m_Input_data.size()) {
                if (m_Input_data[index].digital_analog == 1) {
                    // Analog point: write value directly
                    m_Input_data[index].value = val;
                } else {
                    // Digital point: set control to reflect new state
                    m_Input_data[index].value = (val != 0) ? 1 : 0;
                    m_Input_data[index].control = (val != 0) ? 1 : 0;
                }
                // Notify UI via PostMessage
                if (m_input_dlg_hwnd) {
                    ::PostMessage(m_input_dlg_hwnd, WM_REFRESH_BAC_INPUT_LIST, REFRESH_ON_ITEM, index);
                }
                return 0;
            }
            break;

        case PT_VARIABLE:  // 2 = VARIABLE
            if (index >= 0 && index < (int)m_Variable_data.size()) {
                if (m_Variable_data[index].digital_analog == 1) {
                    // Analog point: write value directly
                    m_Variable_data[index].value = val;
                } else {
                    // Digital point: set control to reflect new state
                    m_Variable_data[index].value = (val != 0) ? 1 : 0;
                    m_Variable_data[index].control = (val != 0) ? 1 : 0;
                }
                // Notify UI via PostMessage
                if (m_variable_dlg_hwnd) {
                    ::PostMessage(m_variable_dlg_hwnd, WM_REFRESH_BAC_VARIABLE_LIST, REFRESH_ON_ITEM, index);
                }
                return 0;
            }
            break;

        default:
            return -1;
    }

    return -1;
}

//=============================================================================
// Remote/network point stubs (not supported in MFC VM)
// NOTE: Uses VM_Point_Net* to match decode.c's call signature.
//=============================================================================
S16_T put_net_point_value(VM_Point_Net *point, S32_T *val_ptr, S16_T aux, S16_T prog_op, U8_T mode)
{
    // Remote points are not supported in the T3000 MFC VM.
    // decode.c handles the case where remote points aren't available gracefully.
    return 0;
}

S16_T get_net_point_value(VM_Point_Net *p, S32_T *val_ptr, U8_T mode, U8_T flag)
{
    // Remote points not supported — return 0 (off/false)
    if (val_ptr) *val_ptr = 0;
    return 0;
}

// Global variables used by decode.c's operand() and MAX/MIN cases
// Remapped via #define value vm_value_global / #define v vm_v_global
S32_T vm_value_global = 0;
S32_T vm_v_global = 0;

//=============================================================================
// Alarm stubs (log output instead of actual alarm generation)
//=============================================================================
S16_T generatealarm(S8_T *mes, S16_T prg, S16_T panel, S16_T type,
                    S8_T alarmatall, S8_T indalarmpanel, S8_T *alarmpanel, S8_T printalarm)
{
    // Log alarm message for debugging; actual alarm handling is done by T3000
    if (mes && *mes) {
        TRACE("[VM Alarm] Prg=%d Panel=%d Type=%d: %s\n",
              (int)prg, (int)panel, (int)type, (const char*)mes);
    }
    return 0;
}

void generate_program_alarm(U8_T type, U8_T prg)
{
    TRACE("[VM Program Alarm] Type=%d Prg=%d\n", (int)type, (int)prg);
}

//=============================================================================
// Get_Mini_Type stub (always return 0 — not a mini controller)
//=============================================================================
U8_T Get_Mini_Type(void)
{
    return 0;
}

//=============================================================================
// Set_icon_config stub
//=============================================================================
void Set_icon_config(U8_T icon_config_val)
{
    icon_config = icon_config_val;
}

//=============================================================================
// send_rs232_command stub (for COM1 function)
//=============================================================================
void send_rs232_command(void)
{
    // Serial port commands not supported in MFC VM
}

// LocalVarMapping, ProgramContext, CVirtualProgramEngine are defined in VirtualProgramEngine.h
// (removed duplicate definitions that were causing C2011 errors)





//=============================================================================
// Constructor/Destructor
//=============================================================================
CVirtualProgramEngine::CVirtualProgramEngine()
    : m_dwScanInterval(VM_DEFAULT_SCAN_MS)
    , m_pWorkerThread(NULL)
    , m_bRunning(FALSE)
    , m_bPaused(FALSE)
    , m_dwTotalExecTime(0)
    , m_nScanCount(0)
{
    for (int i = 0; i < BAC_PROGRAM_ITEM_COUNT; i++) {
        m_Programs[i].programIndex = i;
        m_dwLastScanTime[i] = GetTickCount();
        m_dwProgramStartTime[i] = GetTickCount();
        m_nCallDepth[i] = 0;
    }
}

CVirtualProgramEngine::~CVirtualProgramEngine()
{
    Shutdown();
}

// Global instance
CVirtualProgramEngine g_VirtualProgramEngine;

//=============================================================================
// Initialize/Shutdown
//=============================================================================
BOOL CVirtualProgramEngine::Initialize()
{
    CSingleLock lock(&m_csMain, TRUE);

    if (m_bRunning) {
        return FALSE;
    }

    // Reset all program contexts
    for (int i = 0; i < BAC_PROGRAM_ITEM_COUNT; i++) {
        m_Programs[i].Reset();
    }

    // Create shutdown event
    m_eventShutdown.ResetEvent();
    m_eventPause.ResetEvent();

    // Mark running before starting thread.
    m_bRunning = TRUE;
    m_bPaused = FALSE;

    // Start worker thread
    m_pWorkerThread = AfxBeginThread(WorkerThreadProc, this, THREAD_PRIORITY_NORMAL);
    if (m_pWorkerThread == NULL) {
        m_bRunning = FALSE;
        return FALSE;
    }

    return TRUE;
}

void CVirtualProgramEngine::Shutdown()
{
    CSingleLock lock(&m_csMain, TRUE);

    if (!m_bRunning) {
        return;
    }

    // Stop worker thread
    m_bRunning = FALSE;
    m_eventShutdown.SetEvent();

    if (m_pWorkerThread != NULL) {
        ::WaitForSingleObject(m_pWorkerThread->m_hThread, 5000);
        m_pWorkerThread = NULL;
    }

    // Stop all programs
    StopAll();
}

//=============================================================================
// Worker thread
//=============================================================================
UINT CVirtualProgramEngine::WorkerThreadProc(LPVOID pParam)
{
    CVirtualProgramEngine* pEngine = (CVirtualProgramEngine*)pParam;
    if (pEngine) {
        pEngine->WorkerThread();
    }
    return 0;
}

void CVirtualProgramEngine::WorkerThread()
{
    while (m_bRunning) {
        // Check if paused
        DWORD waitResult = WaitForSingleObject(m_eventPause, 100);
        if (waitResult == WAIT_OBJECT_0) {
            m_eventPause.ResetEvent();
        }

        if (!m_bRunning) break;

        // Execute one scan
        {
            CSingleLock lock(&m_csMain, TRUE);
            ExecuteOneScan();
        }

        // Wait for next scan cycle
        if (m_bRunning) {
            Sleep(m_dwScanInterval);
        }
    }
}

//=============================================================================
// Bytecode loading
// decode.c expects the full bytecode buffer layout:
//   [2-byte base_len][base code][2-byte local_len][local data][2-byte time_len][time data][0xFE]
// We store the original raw bytecode (with 2-byte length header) and
// re-construct the full layout each scan with zeroed time table.
//=============================================================================
BOOL CVirtualProgramEngine::LoadProgram(int programIndex, const unsigned char* bytecode, int byteLen)
{
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return FALSE;
    }

    CSingleLock lock(&m_csMain, TRUE);

    ProgramContext& ctx = m_Programs[programIndex];

    // DEBUG: Dump first bytes of incoming bytecode
    TRACE("[VM] LoadProgram(%d): byteLen=%d, header[0..19]=", programIndex, byteLen);
    for (int i = 0; i < (byteLen < 20 ? byteLen : 20); i++) {
        TRACE("%02X ", bytecode[i]);
    }
    TRACE("\n");

    // Store the original raw bytecode (with 2-byte length header)
    // This preserves the exact format needed to reconstruct the full decode.c buffer.
    ctx.bytecode.assign(bytecode, bytecode + byteLen);

    ctx.ip = 0;
    ctx.instructionCount = 0;
    ctx.state = STATE_STOPPED;

    // Clear disassembly cache
    m_DisasmCache.erase(programIndex);

    return TRUE;
}

void CVirtualProgramEngine::ClearProgram(int programIndex)
{
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return;
    }

    CSingleLock lock(&m_csMain, TRUE);

    ProgramContext& ctx = m_Programs[programIndex];
    ctx.bytecode.clear();
    ctx.Reset();

    m_DisasmCache.erase(programIndex);
}

//=============================================================================
// Runtime control
//=============================================================================
void CVirtualProgramEngine::StartAll()
{
    CSingleLock lock(&m_csMain, TRUE);

    for (int i = 0; i < BAC_PROGRAM_ITEM_COUNT; i++) {
        StartProgram(i);
    }

    if (!m_bRunning) {
        Initialize();
    }
}

void CVirtualProgramEngine::StopAll()
{
    CSingleLock lock(&m_csMain, TRUE);

    for (int i = 0; i < BAC_PROGRAM_ITEM_COUNT; i++) {
        StopProgram(i);
    }
}

void CVirtualProgramEngine::PauseAll()
{
    CSingleLock lock(&m_csMain, TRUE);

    m_bPaused = TRUE;
    m_eventPause.SetEvent();

    for (int i = 0; i < BAC_PROGRAM_ITEM_COUNT; i++) {
        if (m_Programs[i].state == STATE_RUNNING) {
            m_Programs[i].state = STATE_PAUSED;
        }
    }
}

void CVirtualProgramEngine::ResumeAll()
{
    CSingleLock lock(&m_csMain, TRUE);

    m_bPaused = FALSE;

    for (int i = 0; i < BAC_PROGRAM_ITEM_COUNT; i++) {
        if (m_Programs[i].state == STATE_PAUSED) {
            m_Programs[i].state = STATE_RUNNING;
        }
    }
}

void CVirtualProgramEngine::StartProgram(int programIndex)
{
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return;
    }

    CSingleLock lock(&m_csMain, TRUE);

    ProgramContext& ctx = m_Programs[programIndex];
    if (ctx.bytecode.empty()) {
        return;
    }

    ctx.Reset();
    ctx.state = STATE_RUNNING;
    m_dwLastScanTime[programIndex] = GetTickCount();
    m_dwProgramStartTime[programIndex] = GetTickCount();

    NotifyProgramStateChanged(programIndex);
}

void CVirtualProgramEngine::StopProgram(int programIndex)
{
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return;
    }

    CSingleLock lock(&m_csMain, TRUE);

    ProgramContext& ctx = m_Programs[programIndex];
    ctx.state = STATE_STOPPED;
    ctx.Reset();

    NotifyProgramStateChanged(programIndex);
}

void CVirtualProgramEngine::PauseProgram(int programIndex)
{
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return;
    }

    CSingleLock lock(&m_csMain, TRUE);

    if (m_Programs[programIndex].state == STATE_RUNNING) {
        m_Programs[programIndex].state = STATE_PAUSED;
        NotifyProgramStateChanged(programIndex);
    }
}

void CVirtualProgramEngine::ResumeProgram(int programIndex)
{
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return;
    }

    CSingleLock lock(&m_csMain, TRUE);

    if (m_Programs[programIndex].state == STATE_PAUSED) {
        m_Programs[programIndex].state = STATE_RUNNING;
        NotifyProgramStateChanged(programIndex);
    }
}

//=============================================================================
// Scan control
//=============================================================================
void CVirtualProgramEngine::SetScanInterval(DWORD intervalMs)
{
    CSingleLock lock(&m_csMain, TRUE);
    m_dwScanInterval = max(100, intervalMs);  // Min 100ms
}

void CVirtualProgramEngine::ExecuteOneScan()
{
    m_nScanCount++;
    m_dwTotalExecTime += m_dwScanInterval;

#ifdef _DEBUG
    static DWORD s_lastTrace = 0;
    DWORD now = GetTickCount();
    if (now - s_lastTrace >= 5000) {
        s_lastTrace = now;
        TRACE("[VM] Scan=%u interval=%u ms active=%d\n",
              (unsigned)m_nScanCount, (unsigned)m_dwScanInterval, GetActiveProgramCount());
    }
#endif

    ExecuteAllPrograms();
}

//=============================================================================
// Internal execution
//=============================================================================
void CVirtualProgramEngine::ExecuteAllPrograms()
{
    // Update system_timer to match embedded 10ms tick
    system_timer = GetTickCount() / 10;

    for (int i = 0; i < BAC_PROGRAM_ITEM_COUNT; i++) {
        // Check if program should run
        BOOL shouldRun = TRUE;

        // Check on_off switch (from global program data)
        if (!m_Program_data.empty() && i < (int)m_Program_data.size()) {
            shouldRun = (m_Program_data[i].on_off == 1);
        }

        if (!shouldRun) {
            if (m_Programs[i].state != STATE_STOPPED) {
                m_Programs[i].state = STATE_STOPPED;
                NotifyProgramStateChanged(i);
            }
            continue;
        }

        // Check wait state (from previous exec_program WAIT return=1)
        if (m_Programs[i].waiting) {
            DWORD now = GetTickCount();
            if (now >= m_Programs[i].waitUntil) {
                m_Programs[i].waiting = false;
                m_Programs[i].state = STATE_RUNNING;
            } else {
                continue;
            }
        }

        // Execute program if running
        if ((m_Programs[i].state == STATE_RUNNING || m_Programs[i].state == STATE_WAITING) &&
            !m_Programs[i].bytecode.empty()) {
            ExecuteProgram(m_Programs[i]);
        }
    }
}

//=============================================================================
// Execute a single program using decode.c exec_program()
// decode.c expects the full bytecode buffer layout:
//   [2-byte base_len][base code][3 extra bytes][2-byte local_len][local data][2-byte time_len][time data]
// The program_code[] from T3000 already contains this format, so we pass it
// directly to exec_program without reconstruction.
//=============================================================================

BOOL CVirtualProgramEngine::ExecuteProgram(ProgramContext& ctx)
{
    if (ctx.bytecode.empty()) {
        TRACE("[VM] ExecuteProgram(%d): bytecode empty, skipping\n", ctx.programIndex);
        return FALSE;
    }

    // Throttled basic trace (once per 5 seconds)
    {
#ifdef _DEBUG
        static DWORD s_lastExecTrace[64] = {0};
        DWORD now = GetTickCount();
        if (ctx.programIndex < 64 && now - s_lastExecTrace[ctx.programIndex] >= 5000) {
            s_lastExecTrace[ctx.programIndex] = now;
            TRACE("[VM] ExecuteProgram(%d): bytecode size=%d\n", ctx.programIndex, (int)ctx.bytecode.size());
        }
#endif
    }

    // Compute millis since last scan for this program
    DWORD now = GetTickCount();
    miliseclast_cur = now - m_dwLastScanTime[ctx.programIndex];
    m_dwLastScanTime[ctx.programIndex] = now;

    ctx.instructionCount = 0;

    // Sync VM_Station_NUM from MFC's Station_NUM (int) before exec_program runs.
    // MFC's Station_NUM is declared in global_variable_extern.h.
    // decode.c reads VM_Station_NUM as a global U8_T.
    extern int Station_NUM;  // MFC's global variable
    VM_Station_NUM = (U8_T)Station_NUM;

    // Reconstruct the full bytecode buffer for decode.c:
    // decode.c exec_program() expects the following layout:
    //   [2-byte base_len][base_code (base_len bytes)][3 extra bytes][2-byte local_len][local_data][2-byte time_len][time_data]
    // The program_code[] buffer from T3000 already contains this exact layout.
    // We stored it via LoadProgram(), but program_code_length may equal the fixed
    // buffer size (2000). So we need to compute the actual used length from the headers.
    //
    // CRITICAL: We previously tried to reconstruct the buffer by splitting on 0xFE,
    // but that's wrong because decode.c does NOT use 0xFE for navigation — it uses
    // the 2-byte length headers. Also, the "+3" in `prog += nbytes+2+3` means
    // there are 3 extra bytes between base_code and local_len that we must preserve.

    const unsigned char* raw = ctx.bytecode.data();
    int rawLen = (int)ctx.bytecode.size();

    if (rawLen < 2) return FALSE;

    // Read base_len from first 2 bytes (little-endian)
    unsigned short base_len = raw[0] | ((unsigned short)raw[1] << 8);

    // DEBUG: Detailed bytecode diagnostics
    {
#ifdef _DEBUG
        static int s_dumpCount = 0;
        if (s_dumpCount < 3) {  // Only dump first 3 times to avoid log spam
            s_dumpCount++;
            TRACE("[VM] ExecuteProgram(%d) DIAG: rawLen=%d, base_len=%d, header[0..19]=",
                  ctx.programIndex, rawLen, base_len);
            for (int i = 0; i < (rawLen < 20 ? rawLen : 20); i++) {
                TRACE("%02X ", raw[i]);
            }
            TRACE("\n");
        }
#endif
    }

    if (base_len == 0 || base_len > 2000) {
        TRACE("[VM] ExecuteProgram(%d) ERROR: Invalid base_len=%d\n", ctx.programIndex, base_len);
        return FALSE;
    }

    // Navigate the bytecode headers to find the actual used length
    // Format: [2:base_len][base_len:base_code][3:extra][2:local_len][local_len:local_data][2:time_len][time_len:time_data]
    int pos = 2 + base_len + 3;  // Skip to local_len position
    if (pos + 2 > rawLen) {
        TRACE("[VM] ExecuteProgram(%d) ERROR: Bytecode too short for local_len at pos %d (rawLen=%d)\n",
              ctx.programIndex, pos, rawLen);
        return FALSE;
    }
    unsigned short local_len = raw[pos] | ((unsigned short)raw[pos+1] << 8);
    pos += 2 + local_len;  // Skip to time_len position

    if (pos + 2 > rawLen) {
        TRACE("[VM] ExecuteProgram(%d) ERROR: Bytecode too short for time_len at pos %d (rawLen=%d)\n",
              ctx.programIndex, pos, rawLen);
        return FALSE;
    }
    unsigned short time_len = raw[pos] | ((unsigned short)raw[pos+1] << 8);
    int actualLen = pos + 2 + time_len;  // Total actual bytes used

    TRACE("[VM] ExecuteProgram(%d) PARSED: base_len=%d, local_len=%d, time_len=%d, actualLen=%d\n",
          ctx.programIndex, base_len, local_len, time_len, actualLen);

    // Use the original bytecode buffer directly — no reconstruction needed!
    // The program_code[] data already has the correct format that decode.c expects.
    // We just need to make sure exec_program only sees the valid portion.

    // Initialize decode.c globals before calling exec_program
    index_stack = vm_stack;
    prog = ctx.bytecode.data();  // vector data is mutable; decode.c writes *(prog-1)

    // Reset decode.c per-scan state
    alarm_flag = 0;
    new_alarm_flag = 0;
    ind_alarm_panel = 0;
    alarm_at_all = OFF;
    memset(message, 0, sizeof(message));
    memset(vm_stack, 0, sizeof(vm_stack));
    cond = 0;
    op1 = op2 = 0;

    // Call the embedded execution engine
    TRACE("[VM] >>> exec_program(%d) ENTERING, rawLen=%d, base_len=%d, local_len=%d, time_len=%d\n",
          ctx.programIndex, rawLen, base_len, local_len, time_len);
    S16_T result = exec_program(ctx.programIndex, prog);

    // DEBUG: Log execution result
    TRACE("[VM] <<< exec_program(%d) returned %d\n", ctx.programIndex, (int)result);

    // Check for WAIT state (exec_program returns 1 when waiting)
    if (result == 1) {
        // WAIT in progress — set wait state and record wake time
        // The miliseclast_cur value has been stored in the bytecode (decode.c modifies it)
        // We approximate by setting a reasonable wait timeout.
        m_Programs[ctx.programIndex].waiting = true;
        m_Programs[ctx.programIndex].waitUntil = GetTickCount() + 1000;  // default 1s
        m_Programs[ctx.programIndex].state = STATE_WAITING;
        return TRUE;
    }

    // Check for error
    if (result == -1) {
        ctx.state = STATE_ERROR;
        ctx.errorMessage = "Program execution error - invalid bytecode";
        ctx.errorLine = 0;
        return FALSE;
    }

    // Check for ENDPRG (result == 1 from ENDPRG case) or normal end (result == 0)
    // Reset ip for next scan cycle
    ctx.ip = 0;

    return TRUE;
}

//=============================================================================
// State query
//=============================================================================
ProgramState CVirtualProgramEngine::GetProgramState(int programIndex) const
{
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return STATE_STOPPED;
    }
    return m_Programs[programIndex].state;
}

int CVirtualProgramEngine::GetCurrentLine(int programIndex) const
{
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return -1;
    }
    return m_Programs[programIndex].ip;
}

int CVirtualProgramEngine::GetInstructionCount(int programIndex) const
{
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return 0;
    }
    return m_Programs[programIndex].instructionCount;
}

const std::string& CVirtualProgramEngine::GetErrorMessage(int programIndex) const
{
    static std::string empty;
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return empty;
    }
    return m_Programs[programIndex].errorMessage;
}

//=============================================================================
// Debug control
//=============================================================================
void CVirtualProgramEngine::SetBreakpoint(int programIndex, int lineNumber, BOOL enable)
{
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return;
    }

    CSingleLock lock(&m_csMain, TRUE);

    if (enable) {
        m_Programs[programIndex].breakpoints.insert(lineNumber);
    } else {
        m_Programs[programIndex].breakpoints.erase(lineNumber);
    }
}

void CVirtualProgramEngine::ClearAllBreakpoints(int programIndex)
{
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return;
    }

    CSingleLock lock(&m_csMain, TRUE);
    m_Programs[programIndex].breakpoints.clear();
}

// Single-step execution: execute one decode.c instruction
// For single-stepping, we use a limited exec_program with instruction limit
BOOL CVirtualProgramEngine::SingleStep(int programIndex)
{
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return FALSE;
    }

    CSingleLock lock(&m_csMain, TRUE);

    ProgramContext& ctx = m_Programs[programIndex];
    if (ctx.bytecode.empty()) {
        return FALSE;
    }

    if (ctx.state != STATE_PAUSED && ctx.state != STATE_STOPPED) {
        ctx.state = STATE_PAUSED;
    }

    // Use original bytecode buffer directly (same as ExecuteProgram)
    prog = ctx.bytecode.data();
    index_stack = vm_stack;

    // Sync VM_Station_NUM before stepping
    extern int Station_NUM;
    VM_Station_NUM = (U8_T)Station_NUM;

    // Single-step: call exec_program but it will run up to its internal 2000-instruction limit
    // This is a simplification; true single-step would need bytecode-level tracing.
    S16_T result = exec_program(programIndex, prog);

    if (result < 0) {
        ctx.state = STATE_ERROR;
        return FALSE;
    }

    return TRUE;
}

BOOL CVirtualProgramEngine::StepOver(int programIndex)
{
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return FALSE;
    }

    CSingleLock lock(&m_csMain, TRUE);
    // Simplified: execute one full program scan
    return ExecuteProgram(m_Programs[programIndex]);
}

BOOL CVirtualProgramEngine::StepOut(int programIndex)
{
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return FALSE;
    }

    CSingleLock lock(&m_csMain, TRUE);
    // Simplified: execute one full program scan
    return ExecuteProgram(m_Programs[programIndex]);
}

//=============================================================================
// Bytecode disassembly (delegated to embedded engine for now)
//=============================================================================
std::vector<VMInstruction> CVirtualProgramEngine::Disassemble(int programIndex) const
{
    std::vector<VMInstruction> result;

    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return result;
    }

    const ProgramContext& ctx = m_Programs[programIndex];

    // Check cache
    auto it = m_DisasmCache.find(programIndex);
    if (it != m_DisasmCache.end()) {
        return it->second;
    }

    const unsigned char* code = ctx.bytecode.data();
    int ip = 0;
    int lineNum = 0;

    while (ip < (int)ctx.bytecode.size()) {
        VMInstruction inst;
        inst.lineNumber = lineNum;
        inst.opcode = code[ip];
        inst.operands.assign(code + ip + 1, code + ctx.bytecode.size());

        ip++;

        switch (inst.opcode) {
            case ASSIGN:
            case ASSIGNAR:
            case GOTO:
            case GOTOIF:
            case GOSUB:
            case IF:
            case IFP:
            case IFM:
            case FOR:
            case NEXT:
            case WHILE:
            case DO:
            case WAIT:
            case CALLB: {
                if (ip + 1 < (int)ctx.bytecode.size()) {
                    inst.operandValues.push_back((float)(*(short*)(code + ip)));
                    ip += 2;
                }
                break;
            }

            case CONST_VALUE_PRG: {
                if (ip + 4 <= (int)ctx.bytecode.size()) {
                    float val;
                    memcpy(&val, code + ip, sizeof(float));
                    inst.operandValues.push_back(val);
                    ip += 4;
                }
                break;
            }

            case LOCAL_POINT_PRG: {
                ip++;  // marker
                if (ip + 5 < (int)ctx.bytecode.size()) {
                    inst.operandValues.push_back((float)code[ip]);       // number
                    inst.operandValues.push_back((float)code[ip + 1]);    // point_type
                    ip += 2;
                }
                break;
            }
        }

        result.push_back(inst);
        lineNum++;
    }

    const_cast<CVirtualProgramEngine*>(this)->m_DisasmCache[programIndex] = result;
    return result;
}

//=============================================================================
// Statistics
//=============================================================================
int CVirtualProgramEngine::GetActiveProgramCount() const
{
    int count = 0;
    for (int i = 0; i < BAC_PROGRAM_ITEM_COUNT; i++) {
        if (m_Programs[i].state == STATE_RUNNING ||
            m_Programs[i].state == STATE_WAITING) {
            count++;
        }
    }
    return count;
}

//=============================================================================
// Manual value setting (external call)
//=============================================================================
void CVirtualProgramEngine::SetInputValue(int index, int32_t value)
{
    CSingleLock lock(&m_csMain, TRUE);
    if (index >= 0 && index < (int)m_Input_data.size()) {
        m_Input_data[index].value = value;
        NotifyInputChanged(index);
    }
}

void CVirtualProgramEngine::SetOutputValue(int index, int32_t value)
{
    CSingleLock lock(&m_csMain, TRUE);
    if (index >= 0 && index < (int)m_Output_data.size()) {
        m_Output_data[index].value = value;
        NotifyOutputChanged(index);
    }
}

void CVirtualProgramEngine::SetVariableValue(int index, int32_t value)
{
    CSingleLock lock(&m_csMain, TRUE);
    if (index >= 0 && index < (int)m_Variable_data.size()) {
        m_Variable_data[index].value = value;
        NotifyVariableChanged(index);
    }
}

void CVirtualProgramEngine::SetPvarValue(int index, int32_t value)
{
    CSingleLock lock(&m_csMain, TRUE);
    if (index >= 0 && index < (int)m_pvar_data.size()) {
        m_pvar_data[index].value = value;
        NotifyPvarChanged(index);
    }
}

//=============================================================================
// UI notification (using global HWND)
//=============================================================================
void CVirtualProgramEngine::NotifyInputChanged(int index)
{
#ifdef WM_REFRESH_BAC_INPUT_LIST
    if (m_input_dlg_hwnd && ::IsWindow(m_input_dlg_hwnd)) {
        ::PostMessage(m_input_dlg_hwnd, WM_REFRESH_BAC_INPUT_LIST,
                      REFRESH_ON_ITEM, index);
    }
#endif
}

void CVirtualProgramEngine::NotifyOutputChanged(int index)
{
#ifdef WM_REFRESH_BAC_OUTPUT_LIST
    if (m_output_dlg_hwnd && ::IsWindow(m_output_dlg_hwnd)) {
        ::PostMessage(m_output_dlg_hwnd, WM_REFRESH_BAC_OUTPUT_LIST,
                      REFRESH_ON_ITEM, index);
    }
#endif
}

void CVirtualProgramEngine::NotifyVariableChanged(int index)
{
#ifdef WM_REFRESH_BAC_VARIABLE_LIST
    if (m_variable_dlg_hwnd && ::IsWindow(m_variable_dlg_hwnd)) {
        ::PostMessage(m_variable_dlg_hwnd, WM_REFRESH_BAC_VARIABLE_LIST,
                      REFRESH_ON_ITEM, index);
    }
#endif
}

void CVirtualProgramEngine::NotifyPvarChanged(int index)
{
#ifdef WM_REFRESH_BAC_VARIABLE_LIST
    if (m_variable_dlg_hwnd && ::IsWindow(m_variable_dlg_hwnd)) {
        ::PostMessage(m_variable_dlg_hwnd, WM_REFRESH_BAC_VARIABLE_LIST,
                      REFRESH_ON_ITEM, index + 1000);  // offset to distinguish from VAR
    }
#endif
}

void CVirtualProgramEngine::NotifyProgramStateChanged(int index)
{
#ifdef WM_REFRESH_BAC_PROGRAM_LIST
    if (m_pragram_dlg_hwnd && ::IsWindow(m_pragram_dlg_hwnd)) {
        ::PostMessage(m_pragram_dlg_hwnd, WM_REFRESH_BAC_PROGRAM_LIST,
                      REFRESH_ON_ITEM, index);
    }
#endif
}

//=============================================================================
// Error handling
//=============================================================================
void CVirtualProgramEngine::SetError(ProgramContext& ctx, const char* fmt, ...)
{
    char buffer[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    ctx.errorMessage = buffer;
    ctx.errorLine = ctx.ip;

    TRACE("Program %d Error at IP %d: %s\n", ctx.programIndex, ctx.ip, buffer);
}
