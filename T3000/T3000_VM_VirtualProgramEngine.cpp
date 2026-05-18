#include "stdafx.h"
#include "T3000.h"
#include "global_function.h"
#include "global_define.h"

// Get PARSER.h definitions from VirtualProgramEngine.h (inlined copy)

#include "T3000_VM_VirtualProgramEngine.h"

// Forward declaration (definition is below in this file)
static unsigned char VmKeyFromPrgPointType(unsigned char prgPointType);

//=============================================================================
// NUCLEAR OPTION: Undef conflicting Windows SDK / C++ macro definitions
// These macros will replace enum values, causing compilation failures
// This compilation unit (VirtualProgramEngine.cpp) does not use these SDK macros
//=============================================================================
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

//=============================================================================
// Global window HWND declarations
// HWNDs maintained by main window (BacnetView), VM notifies via PostMessage
// Note: No longer depends on dialog class definitions
//=============================================================================
#ifndef _VM_EXTERN_HWNDS
#define _VM_EXTERN_HWNDS
extern HWND m_input_dlg_hwnd;       // CBacnetInput window handle
extern HWND m_output_dlg_hwnd;      // CBacnetOutput window handle
extern HWND m_variable_dlg_hwnd;     // CBacnetVariable window handle
extern HWND m_pragram_dlg_hwnd;      // CBacnetProgram window handle
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
        m_Programs[i].localVariables.resize(VM_MAX_LOCAL_VARS, 0.0f);
        m_dwLastScanTime[i] = GetTickCount();
        m_dwProgramStartTime[i] = GetTickCount();
        m_nCallDepth[i] = 0;
    }
}

//=============================================================================
// Function evaluation (compiler-aligned)
// BacNetProgram_transplant.cpp emits functions as RPN tokens.
// For variable-arg functions (AVG/MAX/MIN/COM_1/MB_* /RGB/PVAR), the token is
// followed by a 1-byte argument count.
//=============================================================================
float CVirtualProgramEngine::ExecFunction(ProgramContext& ctx, unsigned char funcCode)
{
    auto popArg = [&ctx](float& out) -> bool {
        if (ctx.valueStack.empty()) return false;
        out = ctx.valueStack.top();
        ctx.valueStack.pop();
        return true;
    };

    const unsigned char* code = ctx.bytecode.data();

    // Advance past opcode first; some functions may consume an immediate count byte.
    ctx.ip++;

    SYSTEMTIME st;
    GetLocalTime(&st);

    switch (funcCode) {
        case TIME:
            return (float)(st.wHour * 3600 + st.wMinute * 60 + st.wSecond);
        case DOW:
            return (float)st.wDayOfWeek + 1;
        case DOM:
            return (float)st.wDay;
        case DOY: {
            SYSTEMTIME startOfYear = { (WORD)st.wYear, (WORD)1, 0, (WORD)1, 0, 0, 0, 0 };
            FILETIME ft;
            SystemTimeToFileTime(&startOfYear, &ft);
            ULARGE_INTEGER start, current;
            start.LowPart = ft.dwLowDateTime;
            start.HighPart = ft.dwHighDateTime;
            SystemTimeToFileTime(&st, &ft);
            current.LowPart = ft.dwLowDateTime;
            current.HighPart = ft.dwHighDateTime;
            return (float)((current.QuadPart - start.QuadPart) / (10000000ULL * 60 * 60 * 24)) + 1;
        }
        case MOY:
            return (float)st.wMonth;
        case INTERVAL: {
            DWORD now = GetTickCount();
            DWORD interval = (now - m_dwLastScanTime[ctx.programIndex]) / 1000;
            return (float)interval;
        }
        case SCANS:
            return (float)m_nScanCount;
        case RUNTIME:
            return (float)(GetTickCount() - m_dwProgramStartTime[ctx.programIndex]) / 1000.0f;

        case JAN: return 1.0f;
        case FEB: return 2.0f;
        case MAR: return 3.0f;
        case APR: return 4.0f;
        case MAY: return 5.0f;
        case JUN: return 6.0f;
        case JUL: return 7.0f;
        case AUG: return 8.0f;
        case SEP: return 9.0f;
        case OCT: return 10.0f;
        case NOV: return 11.0f;
        case DEC: return 12.0f;

        case ABS:
        case SQR:
        case LN:
        case LN_1:
        case SIN:
        case COS:
        case _INT: {
            float a;
            if (!popArg(a)) return 0.0f;
            switch (funcCode) {
                case ABS: return fabsf(a);
                case SQR: return sqrtf(a);
                case LN: return logf(a);
                case LN_1: return logf(a + 1.0f);
                case SIN: return sinf(a);
                case COS: return cosf(a);
                case _INT: return floorf(a);
            }
            return 0.0f;
        }

        case PIDPROP:
        case PIDRATE:
        case PIDRESET: {
            float idxF;
            if (!popArg(idxF)) return 0.0f;
            int ctrlIndex = (int)idxF;
            if (ctrlIndex < 0 || ctrlIndex >= (int)m_controller_data.size()) return 0.0f;
            const Str_controller_point& ctrl = m_controller_data[ctrlIndex];
            switch (funcCode) {
                case PIDPROP: return (float)ctrl.proportional / 10.0f;
                case PIDRATE: return (float)ctrl.reset;
                case PIDRESET: return (float)ctrl.rate;
            }
            return 0.0f;
        }

        case MAX:
        case MIN:
        case AVG:
        case COM_1:
        case MB_BR:
        case MB_BW:
        case MB_BW_COIL:
        case PVAR:
        case RGB: {
            if (ctx.ip >= (int)ctx.bytecode.size()) return 0.0f;
            unsigned char argc = code[ctx.ip];
            ctx.ip++;
            if (argc == 0) return 0.0f;

            if (funcCode == AVG) {
                float sum = 0.0f;
                for (unsigned char i = 0; i < argc; i++) {
                    float a;
                    if (!popArg(a)) return 0.0f;
                    sum += a;
                }
                return sum / (float)argc;
            }

            if (funcCode == MAX || funcCode == MIN) {
                float a;
                if (!popArg(a)) return 0.0f;
                float best = a;
                for (unsigned char i = 1; i < argc; i++) {
                    if (!popArg(a)) return 0.0f;
                    if (funcCode == MAX) best = (a > best) ? a : best;
                    else best = (a < best) ? a : best;
                }
                return best;
            }

            for (unsigned char i = 0; i < argc; i++) {
                float a;
                if (!popArg(a)) break;
            }
            return 0.0f;
        }

        default:
            return 0.0f;
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
    // Otherwise the worker thread can start, see m_bRunning==FALSE,
    // and exit immediately.
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
            // Paused, reset pause event and continue waiting
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
//=============================================================================
BOOL CVirtualProgramEngine::LoadProgram(int programIndex, const unsigned char* bytecode, int byteLen)
{
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return FALSE;
    }

    CSingleLock lock(&m_csMain, TRUE);

    ProgramContext& ctx = m_Programs[programIndex];

    // Skip 2-byte length header at start
    int startOffset = 2;
    if (byteLen <= startOffset) {
        return FALSE;
    }

    // Find 0xFE end marker (DORU_SYSTEM program terminator)
    int codeEnd = startOffset;
    while (codeEnd < byteLen && bytecode[codeEnd] != 0xFE) {
        codeEnd++;
    }

    // Store code bytes INCLUDING the 0xFE terminator as a sentinel.
    // This matches `BacNetProgram_transplant.cpp` output layout and lets the VM
    // stop cleanly at end-of-program each scan.
    if (codeEnd < byteLen && bytecode[codeEnd] == 0xFE) {
        codeEnd++;  // include 0xFE
    }
    ctx.bytecode.assign(bytecode + startOffset, bytecode + codeEnd);
    ctx.ip = 0;
    ctx.instructionCount = 0;
    ctx.state = STATE_STOPPED;

    // Bug 5 fix: Load local_table data bytes (after 0xFE terminator to end of buffer)
    ctx.localTableBytes.clear();
    ctx.localVarMappings.clear();
    if (codeEnd < byteLen) {
        ctx.localTableBytes.assign(bytecode + codeEnd, bytecode + byteLen);
    }

    // Parse local_table and build variable→global-point mapping
    ParseLocalTable(ctx);

    // Clear disassembly cache
    m_DisasmCache.erase(programIndex);

    return TRUE;
}

BOOL CVirtualProgramEngine::LoadLocalTable(int programIndex, const unsigned char* data, int dataLen)
{
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return FALSE;
    }

    CSingleLock lock(&m_csMain, TRUE);

    ProgramContext& ctx = m_Programs[programIndex];
    ctx.localTable.clear();

    // Parse local_table data
    // Format: [count:2 bytes][variable_table entries...]
    const unsigned char* ptr = data;
    int offset = 0;

    while (offset + (int)sizeof(variable_table) <= dataLen) {
        variable_table var;
        memcpy(&var, ptr, sizeof(variable_table));
        ctx.localTable.push_back(var);
        ptr += sizeof(variable_table);
        offset += sizeof(variable_table);
    }

    // Adjust local variable storage size
    ctx.localVariables.resize(VM_MAX_LOCAL_VARS, 0.0f);

    return TRUE;
}

BOOL CVirtualProgramEngine::LoadTimeTable(int programIndex, const unsigned char* data, int dataLen)
{
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return FALSE;
    }

    CSingleLock lock(&m_csMain, TRUE);

    ProgramContext& ctx = m_Programs[programIndex];
    ctx.timeTable.clear();

    // Parse time_table data (same format as local_table)
    const unsigned char* ptr = data;
    int offset = 0;

    while (offset + (int)sizeof(variable_table) <= dataLen) {
        variable_table var;
        memcpy(&var, ptr, sizeof(variable_table));
        ctx.timeTable.push_back(var);
        ptr += sizeof(variable_table);
        offset += sizeof(variable_table);
    }

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
    ctx.localTable.clear();
    ctx.localTableBytes.clear();
    ctx.localVarMappings.clear();
    ctx.timeTable.clear();
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
        TRACE("[VM] Scan=%u interval=%u ms active=%d\n", (unsigned)m_nScanCount, (unsigned)m_dwScanInterval, GetActiveProgramCount());
    }
#endif

    ExecuteAllPrograms();
}

//=============================================================================
// Internal execution
//=============================================================================
void CVirtualProgramEngine::ExecuteAllPrograms()
{
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

        // Check wait state
        if (m_Programs[i].waiting) {
            DWORD now = GetTickCount();
            if (now >= m_Programs[i].waitUntil) {
                m_Programs[i].waiting = false;
                m_Programs[i].state = STATE_RUNNING;
            } else {
                continue;
            }
        }

        // Execute program
        if ((m_Programs[i].state == STATE_RUNNING || m_Programs[i].state == STATE_WAITING) &&
            !m_Programs[i].bytecode.empty()) {
            ExecuteProgram(m_Programs[i]);
        }
    }
}

BOOL CVirtualProgramEngine::ExecuteProgram(ProgramContext& ctx)
{
    ctx.instructionCount = 0;

    while (ctx.ip < (int)ctx.bytecode.size() && ctx.instructionCount < VM_MAX_INSTRUCTIONS) {
        // Skip non-executable markers without counting as an instruction
        unsigned char op = ctx.bytecode[ctx.ip];
        if (op == 0xFE) {
            // End-of-program marker (DORU_SYSTEM)
            break;
        }
        if (op == 0x01 || op == 0xFF) {
            ExecuteInstruction(ctx);
            continue;
        }

        // Check breakpoint
        int currentLine = ctx.ip;  // Simplified: use ip directly as line number

        // Check breakpoint
        if (ctx.breakpoints.find(currentLine) != ctx.breakpoints.end()) {
            ctx.state = STATE_PAUSED;
            break;
        }

        // Execute single instruction
        if (!ExecuteInstruction(ctx)) {
            ctx.state = STATE_ERROR;
            return FALSE;
        }

        ctx.instructionCount++;
    }

    // If IP reached end (missing ENDPRG), loop back for next scan cycle.
    if (ctx.ip >= (int)ctx.bytecode.size() ||
        (ctx.ip < (int)ctx.bytecode.size() && ctx.bytecode[ctx.ip] == 0xFE)) {
        ctx.ip = 0;
    }

    // Check instruction count limit
    if (ctx.instructionCount >= VM_MAX_INSTRUCTIONS) {
        SetError(ctx, "Exceeded maximum instruction count - possible infinite loop");
        ctx.state = STATE_ERROR;
        return FALSE;
    }

    return TRUE;
}

BOOL CVirtualProgramEngine::ExecuteInstruction(ProgramContext& ctx)
{
    if (ctx.ip >= (int)ctx.bytecode.size()) {
        return FALSE;
    }

    const unsigned char* code = ctx.bytecode.data();
    unsigned char opcode = code[ctx.ip];

    BOOL result = TRUE;

#ifdef _DEBUG
    // VM trace: log every instruction for debugging
    if (ctx.instructionCount < 200) {
        TRACE("[VM] prg=%d ip=%d op=0x%02X stack=%d\n",
            ctx.programIndex, ctx.ip, (unsigned)opcode, (int)ctx.valueStack.size());
    }
#endif

    // Handle line number marker (0x01 + 2-byte line number)
    if (opcode == 0x01) {
        ctx.ip += 3;  // Skip: 0x01 marker + 2-byte line number
        return TRUE;
    }

    // Expression/statement delimiter used by DORU_SYSTEM compiler (0xFF)
    // It can appear between operands/grouped expressions, so just advance.
    if (opcode == 0xFF) {
        ctx.ip++;
        return TRUE;
    }

    // End-of-program marker used by DORU_SYSTEM compiler (0xFE)
    if (opcode == 0xFE) {
        // Stop executing further in this scan; caller will reset ip to 0.
        return TRUE;
    }

    // Bug 2 fix: Handle local variable reference opcodes (0x82-0x9B)
    // These are encoded as: type(1 byte) + offset(2 bytes)
    // Must check BEFORE the default case's function call range (0x30-0xD3)
    // to avoid treating them as function codes.
    if (opcode >= 0x82 && opcode <= 0x9B) {
        unsigned char varType = opcode;
        if (ctx.ip + 3 > (int)ctx.bytecode.size()) {
            SetError(ctx, "LocalVarRef: truncated (type=0x%02X, ip=%d)", varType, ctx.ip);
            return FALSE;
        }
        int offset = ReadShort(code + ctx.ip + 1);
        float value = ReadLocalVariable(ctx, varType, offset);
        ctx.valueStack.push(value);
        ctx.ip += 3;  // 1 byte type + 2 bytes offset
        return TRUE;
    }

    switch (opcode) {
        // Assign instruction
        case ASSIGN:
        case ASSIGNAR:
            result = ExecAssign(ctx);
            break;

        // Control flow instruction
        case GOTO:
            result = ExecGoto(ctx);
            break;

        case GOTOIF:
            result = ExecGotoIf(ctx);
            break;

        case GOSUB:
            result = ExecGosub(ctx);
            break;

        case RETURN:
            result = ExecReturn(ctx);
            break;

        case IF:
        case IFP:
        case IFM:
            result = ExecIf(ctx);
            break;

        case FOR:
            result = ExecFor(ctx);
            break;

        case NEXT:
            result = ExecNext(ctx);
            break;

        case WHILE:
            result = ExecWhile(ctx);
            break;

        case DO:
            result = ExecDo(ctx);
            break;

        case ENDPRG:
            result = ExecEndPrg(ctx);
            break;

        case WAIT:
            result = ExecWait(ctx);
            break;

        // Subroutine call
        case CALLB:
            result = ExecCall(ctx);
            break;

        // Print instruction
        case PRINT:
        case PRINT_AT:
        case PRINT_ALARM_AT:
        case PRINT_ALARM_AT_ALL:
            result = ExecPrint(ctx);
            break;

        // Program control
        case STARTPRG:
            result = ExecStartPrg(ctx);
            break;

        case STOP:
            result = ExecStop(ctx);
            break;

        // Comment - skip
        case REM:
            ctx.ip++;
            break;

        // ELSE marker encountered during THEN block execution
        // Format: ELSE(0x10) + 0xFF + <endTarget:2>
        // Means THEN block is done; jump past the ELSE block.
        case ELSE: {
            ctx.ip++;  // skip ELSE opcode
            if (ctx.ip < (int)ctx.bytecode.size() && code[ctx.ip] == 0xFF) {
                ctx.ip++;  // skip 0xFF
                if (ctx.ip + 2 <= (int)ctx.bytecode.size()) {
                    int endTarget = ReadShort(code + ctx.ip);
                    ctx.ip += 2;
                    // Bug 1 fix: subtract 2
                    ctx.ip = (endTarget >= 2) ? (endTarget - 2) : 0;
                } else {
                    SetError(ctx, "ELSE: truncated jump address");
                    return FALSE;
                }
            } else {
                // ELSE without 0xFF — just skip it
            }
            break;
        }

        // Operators - pop operands from stack, push result
        case PLUS:
        case MINUS:
        case MUL:
        case DIV:
        case MOD:
        case INT_DIV:
        case POW:
        case BIT_AND:
        case BIT_OR:
        case XOR:
        case AND:
        case OR:
        case EQ:
        case NE:
        case LT:
        case GT:
        case LE:
        case GE: {
            if (ctx.valueStack.size() < 2) {
                SetError(ctx, "Stack underflow in binary operation");
                return FALSE;
            }
            float b = ctx.valueStack.top(); ctx.valueStack.pop();
            float a = ctx.valueStack.top(); ctx.valueStack.pop();
            ctx.valueStack.push(ExecBinaryOp(opcode, a, b));
            ctx.ip++;
            break;
        }

        // Unary operator
        case NOT:
        case MINUSUNAR: {
            if (ctx.valueStack.empty()) {
                SetError(ctx, "Stack underflow in unary operation");
                return FALSE;
            }
            float a = ctx.valueStack.top(); ctx.valueStack.pop();
            ctx.valueStack.push(ExecUnaryOp(opcode, a));
            ctx.ip++;
            break;
        }

        // Constant - read 4-byte long value, divide by 1000 to get float
        // Encoding: long(fvar * 1000 + 0.1)
        case CONST_VALUE_PRG: {
            int32_t lval;
            memcpy(&lval, code + ctx.ip + 1, 4);
            float value = (float)lval / 1000.0f;
            ctx.valueStack.push(value);
            ctx.ip += 5;  // 1 byte opcode + 4 bytes value
            break;
        }

        // Local point reference
        // MyPoint format: number(1 byte) + point_type(1 byte)
        // point_type is already +1 (so KEY_VARIABLE=3, etc.)
        // number is 0-based index
        case LOCAL_POINT_PRG: {
            ctx.ip++;  // skip 0x9C marker
            unsigned char number = ReadByte(code + ctx.ip); ctx.ip++;
            unsigned char pointType = ReadByte(code + ctx.ip); ctx.ip++;

            float value = ReadVariable(0, VmKeyFromPrgPointType(pointType), 0, number);
            ctx.valueStack.push(value);
            break;
        }

        // Remote point reference
        // Format: Point_Bacnet (13 bytes) starts with 0x55,0xFF,0x55,0xFF
        // or Point_Net format
        case REMOTE_POINT_PRG: {
            ctx.ip++;  // skip 0x9E marker
            // Check if Point_Bacnet format
            if (ctx.ip + 4 <= (int)ctx.bytecode.size() &&
                code[ctx.ip] == 0x55 && code[ctx.ip+1] == 0xFF &&
                code[ctx.ip+2] == 0x55 && code[ctx.ip+3] == 0xFF) {
                ctx.ip += sizeof(Point_Bacnet);  // 13 bytes
            } else {
                // Point_Net or other format, skip 5 bytes
                ctx.ip += 5;
            }
            ctx.valueStack.push(0.0f);  // Remote point read not yet supported
            break;
        }

        // Function call
        default: {
            // token
            if (opcode >= 0x30 && opcode <= 0xD3) {
                float result = ExecFunction(ctx, opcode);
                ctx.valueStack.push(result);
            } else {
                // Unknown opcode
                SetError(ctx, "Unknown opcode: 0x%02X at position %d", opcode, ctx.ip);
                return FALSE;
            }
        }
    }

    return result;
}

//=============================================================================
// Assign instruction
//=============================================================================
BOOL CVirtualProgramEngine::ExecAssign(ProgramContext& ctx)
{
    const unsigned char* code = ctx.bytecode.data();
    ctx.ip++;  // skip ASSIGN opcode

    // DORU_SYSTEM code format (see `BacNetProgram_transplant.cpp`):
    // ASSIGN + <LHS varref> + <RHS expression tokens (RPN)> + 0xFF
    if (ctx.ip >= (int)ctx.bytecode.size()) {
        SetError(ctx, "Assign: missing target");
        return FALSE;
    }

    // 1) Parse LHS target
    unsigned char marker = ReadByte(code + ctx.ip);
    unsigned char lhsNumber = 0;
    unsigned char lhsPointType = 0;

    // Bug 3 fix: Support local variable reference (0x82-0x9B) as LHS
    if (marker >= 0x82 && marker <= 0x9B) {
        unsigned char varType = marker;
        int offset = ReadShort(code + ctx.ip + 1);
        ctx.ip += 3;  // skip type + 2-byte offset

        // 2) Evaluate RHS expression until delimiter (0xFF) / next line marker (0x01) / end
        while (ctx.ip < (int)ctx.bytecode.size()) {
            unsigned char op = code[ctx.ip];
            if (op == 0xFF || op == 0x01 || op == 0xFE) {
                break;
            }
            if (!ExecuteInstruction(ctx)) {
                return FALSE;
            }
        }

        if (ctx.valueStack.empty()) {
            SetError(ctx, "Assign: RHS produced no value");
            return FALSE;
        }

        float value = ctx.valueStack.top();
        ctx.valueStack.pop();

        // 3) Consume delimiter if present
        if (ctx.ip < (int)ctx.bytecode.size() && code[ctx.ip] == 0xFF) {
            ctx.ip++;
        }

        // 4) Write to local variable
        WriteLocalVariable(ctx, varType, offset, value);
        return TRUE;
    }

    if (marker == LOCAL_POINT_PRG) {
        ctx.ip++; // skip 0x9C
        if (ctx.ip + 2 > (int)ctx.bytecode.size()) {
            SetError(ctx, "Assign: truncated local point");
            return FALSE;
        }
        lhsNumber = (unsigned char)ReadByte(code + ctx.ip); ctx.ip++;
        lhsPointType = (unsigned char)ReadByte(code + ctx.ip); ctx.ip++;
        lhsPointType = VmKeyFromPrgPointType(lhsPointType);
    } else if (marker == REMOTE_POINT_PRG) {
        // Remote write not supported yet, but we must skip its bytes to keep IP consistent.
        ctx.ip++; // skip 0x9E
        if (ctx.ip + 4 <= (int)ctx.bytecode.size() &&
            code[ctx.ip] == 0x55 && code[ctx.ip + 1] == 0xFF &&
            code[ctx.ip + 2] == 0x55 && code[ctx.ip + 3] == 0xFF) {
            ctx.ip += sizeof(Point_Bacnet);
        } else {
            ctx.ip += 5;
        }
        SetError(ctx, "Assign: remote target not supported");
        return FALSE;
    } else {
        SetError(ctx, "Assign: invalid target marker 0x%02X", marker);
        return FALSE;
    }

    // 2) Evaluate RHS expression until delimiter (0xFF) / next line marker (0x01) / end
    // The compiler emits RHS in RPN and expects evaluation to happen on the main value stack.
    // Do not swap/clear the stack here.
    while (ctx.ip < (int)ctx.bytecode.size()) {
        unsigned char op = code[ctx.ip];
        if (op == 0xFF || op == 0x01 || op == 0xFE) {
            break;
        }
        if (!ExecuteInstruction(ctx)) {
            return FALSE;
        }
    }

    if (ctx.valueStack.empty()) {
        SetError(ctx, "Assign: RHS produced no value");
        return FALSE;
    }

    float value = ctx.valueStack.top();
    ctx.valueStack.pop();

    // 3) Consume delimiter if present
    if (ctx.ip < (int)ctx.bytecode.size() && code[ctx.ip] == 0xFF) {
        ctx.ip++;
    }

    // 4) Write-back
    WriteVariable(0, lhsPointType, 0, lhsNumber, value);
    return TRUE;
}

//=============================================================================
// GOTO instruction
// Bug 1 fix: targetIp is based on full buffer (incl. 2-byte length header),
// but VM bytecode starts at offset 2, so subtract 2.
//=============================================================================
BOOL CVirtualProgramEngine::ExecGoto(ProgramContext& ctx)
{
    const unsigned char* code = ctx.bytecode.data();
    ctx.ip++;  // skip GOTO opcode

    // Read target address (compiler patched based on full-buffer layout)
    int targetIp = ReadShort(code + ctx.ip);
    ctx.ip += 2;

    // Bug 1 fix: subtract 2 to convert from full-buffer offset to VM bytecode offset
    if (targetIp >= 2) {
        ctx.ip = targetIp - 2;
    } else {
        ctx.ip = 0;
    }

    return TRUE;
}

//=============================================================================
// GOTOIF instruction
// Bug 1 fix: targetIp needs -2 adjustment.
//=============================================================================
BOOL CVirtualProgramEngine::ExecGotoIf(ProgramContext& ctx)
{
    if (ctx.valueStack.empty()) {
        SetError(ctx, "GotoIf: stack empty");
        return FALSE;
    }

    float cond = ctx.valueStack.top();
    ctx.valueStack.pop();

    const unsigned char* code = ctx.bytecode.data();
    ctx.ip++;  // skip GOTOIF opcode

    // Read target address
    int targetIp = ReadShort(code + ctx.ip);
    ctx.ip += 2;

    // If condition is true, jump to target
    if (cond != 0.0f) {
        // Bug 1 fix: subtract 2
        if (targetIp >= 2) {
            ctx.ip = targetIp - 2;
        } else {
            ctx.ip = 0;
        }
    }

    return TRUE;
}

//=============================================================================
// GOSUB instruction
// Bug 1 fix: targetIp needs -2 adjustment.
//=============================================================================
BOOL CVirtualProgramEngine::ExecGosub(ProgramContext& ctx)
{
    const unsigned char* code = ctx.bytecode.data();
    ctx.ip++;  // skip GOSUB opcode

    // Read target address
    int targetIp = ReadShort(code + ctx.ip);
    ctx.ip += 2;

    // Save return address
    ctx.returnStack.push(ctx.ip);
    m_nCallDepth[ctx.programIndex]++;

    // Bug 1 fix: subtract 2
    if (targetIp >= 2) {
        ctx.ip = targetIp - 2;
    } else {
        ctx.ip = 0;
    }

    return TRUE;
}

//=============================================================================
// RETURN instruction
//=============================================================================
BOOL CVirtualProgramEngine::ExecReturn(ProgramContext& ctx)
{
    if (ctx.returnStack.empty()) {
        SetError(ctx, "Return: return stack empty");
        return FALSE;
    }

    ctx.ip = ctx.returnStack.top();
    ctx.returnStack.pop();
    m_nCallDepth[ctx.programIndex]--;

    return TRUE;
}

//=============================================================================
// IF instruction
// Bug 1 fix: falseTarget and ELSE block jump addresses need -2 adjustment.
//=============================================================================
BOOL CVirtualProgramEngine::ExecIf(ProgramContext& ctx)
{
    const unsigned char* code = ctx.bytecode.data();

    // New DORU_SYSTEM layout (per compiler output):
    //   IF/IFP/IFM (0x0E/0x11/0x12)
    //   + <expr tokens (RPN)>
    //   + 0xFF
    //   + <jump addr (2 bytes)>   <-- Bug 1: needs -2
    //   + [ELSE (0x10) + 0xFF + <jump addr (2 bytes)>]?   <-- Bug 1: needs -2

    const int ifIp = ctx.ip;
    ctx.ip++; // consume IF opcode

    // Strict new-format parsing:
    // We must find the 0xFF delimiter that terminates the IF condition expression
    // before reading the 2-byte jump address.
    // To avoid scanning/corrupt-state on malformed bytecode, cap search distance.
    const int maxScan = 512;
    const int scanLimit = min((int)ctx.bytecode.size(), ctx.ip + maxScan);

    // Evaluate expression tokens until we reach the delimiter.
    // Do NOT treat 0x01 as terminator here: in compiler output, line markers can exist
    // between statements, but IF condition must always end with 0xFF.
    while (ctx.ip < scanLimit && ctx.ip < (int)ctx.bytecode.size()) {
        unsigned char op = code[ctx.ip];
        if (op == 0xFF) {
            break;
        }
        // If we hit end-of-program, this IF is malformed.
        if (op == 0xFE) {
            SetError(ctx, "If: hit end-of-program before delimiter 0xFF (ip=%d)", ctx.ip);
            return FALSE;
        }
        if (!ExecuteInstruction(ctx)) {
            return FALSE;
        }
    }

    // If delimiter not found within window, attempt legacy fallback in a conservative way.
    if (ctx.ip >= (int)ctx.bytecode.size() || code[ctx.ip] != 0xFF) {
        // Legacy fallback: IF opcode was followed by immediate elseIp.
        // This is only attempted if we haven't consumed any tokens (meaning next bytes were
        // not a valid expression start), and there is enough room for a short.
        if (ctx.ip == ifIp + 1 && (ifIp + 3) <= (int)ctx.bytecode.size()) {
            if (ctx.valueStack.empty()) {
                SetError(ctx, "If(legacy): stack empty");
                return FALSE;
            }
            float condLegacy = ctx.valueStack.top();
            ctx.valueStack.pop();
            int elseIpLegacy = ReadShort(code + ctx.ip);
            ctx.ip += 2;
            if (condLegacy == 0.0f) {
                // Bug 1 fix: -2
                if (elseIpLegacy >= 2) {
                    ctx.ip = elseIpLegacy - 2;
                } else {
                    ctx.ip = 0;
                }
            }
            return TRUE;
        }

        SetError(ctx, "If: missing delimiter 0xFF (start=%d, ip=%d)", ifIp, ctx.ip);
        return FALSE;
    }

    if (ctx.valueStack.empty()) {
        SetError(ctx, "If: condition expression produced no value");
        return FALSE;
    }

    float cond = ctx.valueStack.top();
    ctx.valueStack.pop();

    ctx.ip++; // consume 0xFF
    if (ctx.ip + 2 > (int)ctx.bytecode.size()) {
        SetError(ctx, "If: truncated jump address");
        return FALSE;
    }
    int falseTarget = ReadShort(code + ctx.ip);
    ctx.ip += 2;

    if (cond == 0.0f) {
        // Bug 1 fix: subtract 2
        if (falseTarget >= 2) {
            ctx.ip = falseTarget - 2;
        } else {
            ctx.ip = 0;
        }
        return TRUE;
    }

    // If condition true: continue into THEN block.
    // The compiler layout is:
    //   IF + <expr> + 0xFF + <falseTarget:2> + <THEN block> + ELSE + 0xFF + <endTarget:2> + <ELSE block>
    // When condition is TRUE, we fall through to execute the THEN block.
    // We do NOT skip over ELSE here — the THEN block's statements will execute
    // sequentially, and when they hit the ELSE marker (0x10), ExecuteInstruction
    // will handle it as a jump to skip the ELSE block.
    // When condition is FALSE, we already jumped to falseTarget above.
    //
    // Nothing else to do here — just continue executing from current ip.

    return TRUE;
}

//=============================================================================
// FOR instruction
// Bug 1 fix: targetIp needs -2 adjustment (stored adjusted, used directly in NEXT).
//=============================================================================
BOOL CVirtualProgramEngine::ExecFor(ProgramContext& ctx)
{
    const unsigned char* code = ctx.bytecode.data();
    ctx.ip++;  // skip FOR opcode

    // Compiler format (BacNetProgram_transplant.cpp):
    //   FOR
    //   + pcodvar(NAME,...)   // encoded as a var reference token stream
    //   + <expr(start)> 0xFF
    //   + <expr(end)>   0xFF
    //   + <expr(step)>  0xFF
    //   + <targetIp:2 bytes>  // patched by compiler

    auto evalExprUntilFF = [this, &ctx, code]() -> bool {
        // Evaluate RPN tokens until the 0xFF delimiter.
        const int maxScan = 2048;
        const int scanLimit = min((int)ctx.bytecode.size(), ctx.ip + maxScan);
        while (ctx.ip < (int)ctx.bytecode.size() && ctx.ip < scanLimit) {
            unsigned char op = code[ctx.ip];
            if (op == 0xFF) {
                ctx.ip++; // consume delimiter
                return true;
            }
            if (op == 0xFE) {
                SetError(ctx, "For: hit end-of-program before delimiter 0xFF (ip=%d)", ctx.ip);
                return false;
            }
            if (!ExecuteInstruction(ctx)) {
                return false;
            }
        }
        SetError(ctx, "For: missing delimiter 0xFF (ip=%d)", ctx.ip);
        return false;
    };

    // 1) Parse loop variable reference
    if (ctx.ip >= (int)ctx.bytecode.size()) {
        SetError(ctx, "For: missing loop variable");
        return FALSE;
    }

    int varIndex = -1;
    unsigned char marker = code[ctx.ip];
    if (marker == LOCAL_POINT_PRG) {
        ctx.ip++; // skip 0x9C
        if (ctx.ip + 2 > (int)ctx.bytecode.size()) {
            SetError(ctx, "For: truncated loop variable");
            return FALSE;
        }
        unsigned char number = ReadByte(code + ctx.ip); ctx.ip++;
        unsigned char pointType = ReadByte(code + ctx.ip); ctx.ip++;
        // Loop counter must be a local variable.
        // pointType is already +1 in compiler output (VmKeyFromPrgPointType maps it).
        if (VmKeyFromPrgPointType(pointType) != KEY_VARIABLE) {
            SetError(ctx, "For: loop variable point type must be VARIABLE (got %u)", (unsigned)pointType);
            return FALSE;
        }
        varIndex = (int)number;
    } else {
        // Conservative legacy fallback: older encoding used 1-byte var index.
        varIndex = (int)ReadByte(code + ctx.ip);
        ctx.ip++;
    }

    if (varIndex < 0 || varIndex >= (int)ctx.localVariables.size()) {
        SetError(ctx, "For: loop variable index out of range: %d", varIndex);
        return FALSE;
    }

    // 2) Evaluate start/end/step expressions
    if (!evalExprUntilFF()) return FALSE;
    if (ctx.valueStack.empty()) {
        SetError(ctx, "For: start expression produced no value");
        return FALSE;
    }
    float startVal = ctx.valueStack.top(); ctx.valueStack.pop();

    if (!evalExprUntilFF()) return FALSE;
    if (ctx.valueStack.empty()) {
        SetError(ctx, "For: end expression produced no value");
        return FALSE;
    }
    float endVal = ctx.valueStack.top(); ctx.valueStack.pop();

    if (!evalExprUntilFF()) return FALSE;
    if (ctx.valueStack.empty()) {
        SetError(ctx, "For: step expression produced no value");
        return FALSE;
    }
    float step = ctx.valueStack.top(); ctx.valueStack.pop();
    if (step == 0.0f) {
        // Avoid infinite loop.
        SetError(ctx, "For: step cannot be 0");
        return FALSE;
    }

    // 3) Read patched targetIp
    if (ctx.ip + 2 > (int)ctx.bytecode.size()) {
        SetError(ctx, "For: truncated target address");
        return FALSE;
    }
    int targetIp = ReadShort(code + ctx.ip);
    ctx.ip += 2;

    // Bug 1 fix: store adjusted targetIp (subtract 2) so NEXT can use it directly
    if (targetIp >= 2) {
        targetIp = targetIp - 2;
    } else {
        targetIp = 0;
    }

    // 4) Initialize variable + push loop frame
    ctx.localVariables[varIndex] = startVal;

    ProgramContext::ForLoop loop;
    loop.counter = startVal;
    loop.endValue = endVal;
    loop.step = step;
    loop.targetIp = targetIp;
    loop.varIndex = (unsigned char)varIndex;

    ctx.forStack.push(loop);
    return TRUE;
}

//=============================================================================
// NEXT instruction
// Note: loop.targetIp is already adjusted (Bug 1 fix applied in ExecFor).
//=============================================================================
BOOL CVirtualProgramEngine::ExecNext(ProgramContext& ctx)
{
    const unsigned char* code = ctx.bytecode.data();
    ctx.ip++;  // skip NEXT opcode

    // Compiler frequently emits NEXT + pcodvar(var) (i.e. var reference).
    // We accept either full varref or legacy single-byte index.
    int varIndex = -1;
    if (ctx.ip < (int)ctx.bytecode.size() && code[ctx.ip] == LOCAL_POINT_PRG) {
        ctx.ip++; // 0x9C
        if (ctx.ip + 2 > (int)ctx.bytecode.size()) {
            SetError(ctx, "Next: truncated loop variable");
            return FALSE;
        }
        unsigned char number = ReadByte(code + ctx.ip); ctx.ip++;
        unsigned char pointType = ReadByte(code + ctx.ip); ctx.ip++;
        if (VmKeyFromPrgPointType(pointType) != KEY_VARIABLE) {
            SetError(ctx, "Next: loop variable point type must be VARIABLE");
            return FALSE;
        }
        varIndex = (int)number;
    } else if (ctx.ip < (int)ctx.bytecode.size()) {
        varIndex = (int)ReadByte(code + ctx.ip);
        ctx.ip++;
    }

    if (ctx.forStack.empty()) {
        SetError(ctx, "Next: for stack empty");
        return FALSE;
    }

    ProgramContext::ForLoop& loop = ctx.forStack.top();

    // Prefer the FOR frame's var index. If NEXT provided one, validate it.
    int frameVarIndex = (int)loop.varIndex;
    if (varIndex >= 0 && varIndex != frameVarIndex) {
        // Keep running but signal mismatch (corrupted bytecode or unsupported pattern)
        SetError(ctx, "Next: variable mismatch (next=%d, for=%d)", varIndex, frameVarIndex);
        return FALSE;
    }
    varIndex = frameVarIndex;

    // Increment counter
    loop.counter += loop.step;

    // Update loop variable
    if (varIndex >= 0 && varIndex < (int)ctx.localVariables.size()) {
        ctx.localVariables[varIndex] = loop.counter;
    }

    // Check if should continue loop
    BOOL continueLoop;
    if (loop.step > 0) {
        continueLoop = (loop.counter <= loop.endValue);
    } else {
        continueLoop = (loop.counter >= loop.endValue);
    }

    if (continueLoop) {
        // loop.targetIp is already adjusted by -2 in ExecFor
        ctx.ip = loop.targetIp;
    } else {
        ctx.forStack.pop();
    }

    return TRUE;
}

//=============================================================================
// WHILE instruction
// Bug 1 fix: endIp needs -2 adjustment.
//=============================================================================
BOOL CVirtualProgramEngine::ExecWhile(ProgramContext& ctx)
{
    if (ctx.valueStack.empty()) {
        SetError(ctx, "While: stack empty");
        return FALSE;
    }

    float cond = ctx.valueStack.top();
    ctx.valueStack.pop();

    const unsigned char* code = ctx.bytecode.data();
    ctx.ip++;  // skip WHILE opcode

    // Read loop end address
    int endIp = ReadShort(code + ctx.ip);
    ctx.ip += 2;

    // Bug 1 fix: subtract 2
    if (cond == 0.0f) {
        if (endIp >= 2) {
            ctx.ip = endIp - 2;
        } else {
            ctx.ip = 0;
        }
    }

    return TRUE;
}

//=============================================================================
// DO instruction
//=============================================================================
BOOL CVirtualProgramEngine::ExecDo(ProgramContext& ctx)
{
    // DO instruction, LOOP/WHILE
    const unsigned char* code = ctx.bytecode.data();
    ctx.ip++;  // skip DO opcode

    // Save current IP to return stack (for loop return)
    ctx.returnStack.push(ctx.ip);

    return TRUE;
}

//=============================================================================
// ENDPRG instruction
//=============================================================================
BOOL CVirtualProgramEngine::ExecEndPrg(ProgramContext& ctx)
{
    // Reset instruction pointer for next scan cycle
    // Programs loop continuously in a BAS system
    ctx.ip = 0;
    return TRUE;
}

//=============================================================================
// WAIT instruction
//=============================================================================
BOOL CVirtualProgramEngine::ExecWait(ProgramContext& ctx)
{
    if (ctx.valueStack.empty()) {
        SetError(ctx, "Wait: stack empty");
        return FALSE;
    }

    float seconds = ctx.valueStack.top();
    ctx.valueStack.pop();

    ctx.waitUntil = GetTickCount() + (DWORD)(seconds * 1000);
    ctx.waiting = true;
    ctx.state = STATE_WAITING;

    ctx.ip++;  // skip WAIT opcode

    // Skip possible parameters (for format compatibility)
    const unsigned char* code = ctx.bytecode.data();
    if (ctx.ip < (int)ctx.bytecode.size()) {
        unsigned char marker = code[ctx.ip];
        if (marker == LOCAL_POINT_PRG) {
            ctx.ip++;  // skip 0x9C
            ctx.ip += 2;  // MyPoint: number(1) + point_type(1)
        } else if (marker == CONST_VALUE_PRG) {
            ctx.ip++;  // skip 0x9D
            ctx.ip += 4;  // 4-byte long value
        }
    }

    return TRUE;
}

//=============================================================================
// CALL instruction
//=============================================================================
BOOL CVirtualProgramEngine::ExecCall(ProgramContext& ctx)
{
    const unsigned char* code = ctx.bytecode.data();
    ctx.ip++;  // skip CALL opcode

    // Read subroutine index
    unsigned char subIndex = ReadByte(code + ctx.ip); ctx.ip++;

    // TODO: Implement subroutine call
    // Skip for now

    return TRUE;
}

//=============================================================================
// PRINT instruction
//=============================================================================
BOOL CVirtualProgramEngine::ExecPrint(ProgramContext& ctx)
{
    ctx.ip++;  // skip PRINT opcode

    // Read and discard print args (no actual output)
    while (ctx.ip < (int)ctx.bytecode.size()) {
        const unsigned char* code = ctx.bytecode.data();
        unsigned char marker = code[ctx.ip];

        // Check if statement end
        if (marker == EOL || marker == NL || marker == SEMICOLON) {
            ctx.ip++;
            break;
        }

        // Check if operator (statement end)
        if (marker >= 0x07 && marker <= 0x2F) {
            break;
        }

        // Skip this operand based on marker type
        if (marker == CONST_VALUE_PRG) {
            ctx.ip += 5;  // 1 byte opcode + 4 bytes value
        } else if (marker == LOCAL_POINT_PRG) {
            ctx.ip++;  // skip 0x9C
            ctx.ip += 2;  // MyPoint: number(1) + point_type(1)
        } else if (marker == REMOTE_POINT_PRG) {
            ctx.ip++;  // skip 0x9E
            // Point_Bacnet (13 bytes) or Point_Net (5 bytes)
            if (ctx.ip + 4 <= (int)ctx.bytecode.size() &&
                code[ctx.ip] == 0x55 && code[ctx.ip+1] == 0xFF &&
                code[ctx.ip+2] == 0x55 && code[ctx.ip+3] == 0xFF) {
                ctx.ip += sizeof(Point_Bacnet);  // 13 bytes
            } else {
                ctx.ip += 5;  // Point_Net or other format
            }
        } else {
            ctx.ip++;
        }
    }

    return TRUE;
}

//=============================================================================
// STARTPRG instruction
// Bug 4 fix: Compiler format is STARTPRG(0x1C) + pcodvar(point reference),
// e.g. "1c 9c 00 01" = START MYOUT1 (MYOUT1 = OUT point index 0).
// This writes 1.0f to the target output point (turning it ON).
// It does NOT pop from the value stack.
//=============================================================================
BOOL CVirtualProgramEngine::ExecStartPrg(ProgramContext& ctx)
{
    const unsigned char* code = ctx.bytecode.data();
    ctx.ip++;  // skip STARTPRG opcode (0x1C)

    // Read point reference: LOCAL_POINT_PRG (0x9C) + number(1) + pointType(1)
    if (ctx.ip >= (int)ctx.bytecode.size()) {
        SetError(ctx, "StartPrg: missing point reference");
        return FALSE;
    }

    unsigned char marker = ReadByte(code + ctx.ip);
    if (marker != LOCAL_POINT_PRG) {
        // Legacy fallback: treat as program index (pop from stack)
        // NOTE: ctx.valueStack may have leftover values from IF condition evaluation
        // so we only pop if the stack has entries AND the marker is not a known opcode.
        if (!ctx.valueStack.empty()) {
            ctx.valueStack.pop();  // discard leftover
        }
        int targetIndex = 0;
        if (!ctx.valueStack.empty()) {
            float programNum = ctx.valueStack.top();
            ctx.valueStack.pop();
            targetIndex = (int)programNum;
        }
        if (targetIndex >= 0 && targetIndex < BAC_PROGRAM_ITEM_COUNT) {
            StartProgram(targetIndex);
        }
        return TRUE;
    }

    ctx.ip++;  // skip 0x9C
    if (ctx.ip + 1 >= (int)ctx.bytecode.size()) {
        SetError(ctx, "StartPrg: truncated point reference");
        return FALSE;
    }

    unsigned char number = ReadByte(code + ctx.ip); ctx.ip++;
    unsigned char pointType = ReadByte(code + ctx.ip); ctx.ip++;

    // Map pointType (+1 encoded) back to KEY_* and write 1.0f (ON)
    unsigned char vmPointType = VmKeyFromPrgPointType(pointType);
    WriteVariable(0, vmPointType, 0, number, 1.0f);

    return TRUE;
}

//=============================================================================
// STOP instruction
// Bug 5 fix: STOP MYOUT1 means "turn OFF output MYOUT1" (write 0.0f),
// NOT "stop the entire program execution".
// Compiler format: STOP(0x1D) + pcodvar(point reference), e.g. "1d 9c 00 01".
//=============================================================================
BOOL CVirtualProgramEngine::ExecStop(ProgramContext& ctx)
{
    const unsigned char* code = ctx.bytecode.data();
    ctx.ip++;  // skip STOP opcode (0x1D)

    // Read point reference: LOCAL_POINT_PRG (0x9C) + number(1) + pointType(1)
    if (ctx.ip >= (int)ctx.bytecode.size()) {
        SetError(ctx, "Stop: missing point reference");
        return FALSE;
    }

    unsigned char marker = ReadByte(code + ctx.ip);

    // If no valid marker, just skip (no-op STOP)
    if (marker == 0xFF || marker == 0x01 || marker == 0xFE) {
        // End of statement, nothing to stop
        return TRUE;
    }

    if (marker != LOCAL_POINT_PRG) {
        // Unknown marker - skip it
        ctx.ip++;
        return TRUE;
    }

    ctx.ip++;  // skip 0x9C
    if (ctx.ip + 1 >= (int)ctx.bytecode.size()) {
        SetError(ctx, "Stop: truncated point reference");
        return FALSE;
    }

    unsigned char number = ReadByte(code + ctx.ip); ctx.ip++;
    unsigned char pointType = ReadByte(code + ctx.ip); ctx.ip++;

    // Map pointType (+1 encoded) back to KEY_* and write 0.0f (OFF)
    unsigned char vmPointType = VmKeyFromPrgPointType(pointType);
    WriteVariable(0, vmPointType, 0, number, 0.0f);

    return TRUE;
}

//=============================================================================
// Binary operator
//=============================================================================

float CVirtualProgramEngine::ExecBinaryOp(unsigned char op, float a, float b)
{
    switch (op) {
        case PLUS:      return a + b;
        case MINUS:     return a - b;
        case MUL:       return a * b;
        case DIV:       return (b != 0.0f) ? (a / b) : 0.0f;
        case MOD:       return (float)((int)a % (int)b);
        case INT_DIV:   return (float)((int)a / (int)b);
        case POW:       return powf(a, b);
        case BIT_AND:   return (float)((int)a & (int)b);
        case BIT_OR:    return (float)((int)a | (int)b);
        case XOR:       return (float)((int)a ^ (int)b);
        case AND:       return (a != 0.0f && b != 0.0f) ? 1.0f : 0.0f;
        case OR:        return (a != 0.0f || b != 0.0f) ? 1.0f : 0.0f;
        case EQ:        return (a == b) ? 1.0f : 0.0f;
        case NE:        return (a != b) ? 1.0f : 0.0f;
        case LT:        return (a < b) ? 1.0f : 0.0f;
        case GT:        return (a > b) ? 1.0f : 0.0f;
        case LE:        return (a <= b) ? 1.0f : 0.0f;
        case GE:        return (a >= b) ? 1.0f : 0.0f;
        default:        return 0.0f;
    }
}

//=============================================================================
// Unary operator
//=============================================================================

float CVirtualProgramEngine::ExecUnaryOp(unsigned char op, float a)
{
    switch (op) {
        case NOT:        return (a == 0.0f) ? 1.0f : 0.0f;
        case MINUSUNAR:  return -a;
        default:         return a;
    }
}

//=============================================================================
// Bytecode reading helper
//=============================================================================
static unsigned char VmKeyFromPrgPointType(unsigned char prgPointType)
{
    // BacnetProgram_transplant.cpp encodes MyPoint.point_type as (point_type + 1)
    // where point_type follows OUT=0, IN=1, VAR=2, PRG=4, etc.
    // The VM ReadVariable/WriteVariable use KEY_* enums (KEY_INPUT=1, KEY_OUTPUT=2, KEY_VARIABLE=3, KEY_PROGRAM=4 ...)
    // Map common cases explicitly.
    switch (prgPointType) {
        case 1: return KEY_OUTPUT;   // OUT(0)+1
        case 2: return KEY_INPUT;    // IN(1)+1
        case 3: return KEY_VARIABLE; // VAR(2)+1
        case 5: return KEY_PROGRAM;  // PRG(4)+1
        default:
            // Fallback: best-effort shift (keeps existing behavior for other types)
            return prgPointType;
    }
}

float CVirtualProgramEngine::ReadFloat(const unsigned char* ptr)
{
    float value;
    memcpy(&value, ptr, sizeof(float));
    return value;
}

int CVirtualProgramEngine::ReadShort(const unsigned char* ptr)
{
    int value = *(short*)ptr;
    return value;
}

int CVirtualProgramEngine::ReadByte(const unsigned char* ptr)
{
    return *ptr;
}

//=============================================================================
// Operand parsing
//=============================================================================
BOOL CVirtualProgramEngine::ParseOperand(const unsigned char* ip, ProgramContext& ctx, float& outValue, int& bytesConsumed)
{
    const unsigned char* p = ip;
    unsigned char marker = (unsigned char)ReadByte(p);
    p += 1;

    switch (marker) {
        case CONST_VALUE_PRG: {
            // Encoding: long(fvar * 1000 + 0.1)
            int32_t lval;
            memcpy(&lval, p, 4);
            outValue = (float)lval / 1000.0f;
            p += 4;
            bytesConsumed = (int)(p - ip);
            return TRUE;
        }

        case LOCAL_POINT_PRG: {
            // MyPoint format: number(1 byte) + point_type(1 byte)
            // point_type is already +1 (KEY_VARIABLE=3, etc.)
            // number is 0-based index
            unsigned char number = (unsigned char)ReadByte(p); p += 1;
            unsigned char pointType = (unsigned char)ReadByte(p); p += 1;

            outValue = ReadVariable(0, VmKeyFromPrgPointType(pointType), 0, number);
            bytesConsumed = (int)(p - ip);
            return TRUE;
        }

        case REMOTE_POINT_PRG: {
            // Check if Point_Bacnet format (starts with 0x55,0xFF,0x55,0xFF)
            if (p + 4 <= ip + 20 && (unsigned char)p[0] == 0x55 && (unsigned char)p[1] == 0xFF &&
                (unsigned char)p[2] == 0x55 && (unsigned char)p[3] == 0xFF) {
                p += sizeof(Point_Bacnet);  // 13 bytes
            } else {
                p += 5;  // Point_Net or other format
            }
            outValue = 0.0f;  // Remote point read not yet supported
            bytesConsumed = (int)(p - ip);
            return TRUE;
        }

        default:
            SetError(ctx, "ParseOperand: unknown marker 0x%02X", marker);
            bytesConsumed = 0;
            return FALSE;
    }
}

//=============================================================================
// Parse local_table and build variable→global-point mapping
//
// After 0xFE, the buffer layout is:
//   [2 bytes Byte=0][2 bytes ind_local_table][local_table data...][time_table data...]
//
// Each local_table entry:
//   [type:1byte][data:k bytes][name\0]
//   FLOAT_TYPE(0x83): k=4   LONG_TYPE(0x84): k=4
//   INTEGER_TYPE(0x85): k=2  BYTE_TYPE(0x86): k=1
//
// In pcodvar(), the compiler writes offset = j+1 where j is the byte index
// of the type byte within the local_table data (after the 4-byte header).
// So the actual byte position in localTableBytes = localTableDataStart + offset.
//
// When a variable name matches a global point's label (e.g. "COUNT" = VAR1),
// we redirect reads/writes to that global point.
//=============================================================================
void CVirtualProgramEngine::ParseLocalTable(ProgramContext& ctx)
{
    ctx.localVarMappings.clear();
    ctx.localTableDataStart = 4;  // default header size

    if (ctx.localTableBytes.size() < 4) return;

    // Read header
    // [0-1]: Byte (always 0), [2-3]: ind_local_table
    int indLocalTable = *(short*)&ctx.localTableBytes[2];
    if (indLocalTable <= 0) return;

    // Data starts after 4-byte header
    const unsigned char* data = ctx.localTableBytes.data() + 4;
    int dataLen = indLocalTable;
    if (4 + dataLen > (int)ctx.localTableBytes.size()) {
        dataLen = (int)ctx.localTableBytes.size() - 4;
    }

    int j = 0;  // position within data
    while (j < dataLen) {
        unsigned char type = data[j];
        int k = 0;

        switch (type) {
            case FLOAT_TYPE:
            case LONG_TYPE:
                k = 4;
                break;
            case INTEGER_TYPE:
                k = 2;
                break;
            case BYTE_TYPE:
                k = 1;
                break;
            default:
                // Array types or unknown — skip using the same logic as createlocaltable
                switch (type) {
                    case 0x88: case 0x89: k = 4; break;  // FLOAT/LONG ARRAY
                    case 0x8A: k = 2; break;              // INTEGER ARRAY
                    case 0x8B: case 0x87: k = 1; break;   // BYTE ARRAY / STRING
                    default: k = 0; break;
                }
                if (k > 0 && j + 1 + 4 <= dataLen) {
                    int dim1 = *(int*)&data[j + 1];
                    int dim2 = *(int*)&data[j + 3];
                    if (dim1)
                        k = 4 + dim1 * dim2 * k;
                    else
                        k = 4 + dim2 * k;
                } else {
                    // Unknown type, can't parse further
                    return;
                }
                break;
        }

        // offset in bytecode = j + 1 (position of variable's data within data[])
        int localOffset = j + 1;

        // Name starts after type(1) + data(k)
        int nameStart = j + 1 + k;
        if (nameStart >= dataLen) break;

        const char* name = (const char*)&data[nameStart];

        // Build mapping entry
        LocalVarMapping mapping;
        mapping.localOffset = localOffset;
        mapping.varType = type;
        mapping.name = name;
        mapping.globalPointType = 0;
        mapping.globalIndex = -1;

        // Try to match name against global point labels
        // Search m_Variable_data first, then m_Input_data, m_Output_data, m_Program_data
        bool found = false;

        for (int i = 0; i < (int)m_Variable_data.size() && !found; i++) {
            if (strncmp((const char*)m_Variable_data[i].label, name, 8) == 0) {
                mapping.globalPointType = KEY_VARIABLE;
                mapping.globalIndex = i;
                found = true;
            }
        }
        for (int i = 0; i < (int)m_Input_data.size() && !found; i++) {
            if (strncmp((const char*)m_Input_data[i].label, name, 8) == 0) {
                mapping.globalPointType = KEY_INPUT;
                mapping.globalIndex = i;
                found = true;
            }
        }
        for (int i = 0; i < (int)m_Output_data.size() && !found; i++) {
            if (strncmp((const char*)m_Output_data[i].label, name, 8) == 0) {
                mapping.globalPointType = KEY_OUTPUT;
                mapping.globalIndex = i;
                found = true;
            }
        }
        for (int i = 0; i < (int)m_Program_data.size() && !found; i++) {
            if (strncmp((const char*)m_Program_data[i].label, name, 8) == 0) {
                mapping.globalPointType = KEY_PROGRAM;
                mapping.globalIndex = i;
                found = true;
            }
        }
        for (int i = 0; i < (int)m_pvar_data.size() && !found; i++) {
            if (strncmp((const char*)m_pvar_data[i].label, name, 8) == 0) {
                mapping.globalPointType = KEY_VARIABLE;  // pvar uses VAR path
                mapping.globalIndex = i;
                found = true;
            }
        }

        ctx.localVarMappings.push_back(mapping);

        // Advance past: type(1) + data(k) + name + null terminator
        j = nameStart + (int)strlen(name) + 1;
    }
}

//=============================================================================
// Local variable read/write
// These now support mapping: if a local variable is mapped to a global point
// (e.g. "COUNT" → m_Variable_data[0]), reads/writes are redirected to the
// global point via ReadVariable/WriteVariable.
//
// For unmapped local variables, the offset from bytecode is relative to the
// local_table data area (after the 4-byte header in localTableBytes).
// Actual byte position = localTableDataStart + offset.
//=============================================================================
float CVirtualProgramEngine::ReadLocalVariable(ProgramContext& ctx, unsigned char varType, int offset)
{
    if (offset < 0) return 0.0f;

    // Check mapping first: if this local variable maps to a global point, read from there
    for (const auto& m : ctx.localVarMappings) {
        if (m.localOffset == offset) {
            if (m.globalPointType != 0 && m.globalIndex >= 0) {
                return ReadVariable(0, m.globalPointType, 0, (unsigned short)m.globalIndex);
            }
            break;  // found mapping but not global — use local storage
        }
    }

    // Not mapped to global: read from localTableBytes with corrected base offset
    int adjustedOffset = ctx.localTableDataStart + offset;
    const unsigned char* data = ctx.localTableBytes.data();

    switch (varType) {
        case FLOAT_TYPE: {
            if (adjustedOffset + 4 > (int)ctx.localTableBytes.size()) return 0.0f;
            float val;
            memcpy(&val, data + adjustedOffset, sizeof(float));
            return val;
        }
        case LONG_TYPE: {
            if (adjustedOffset + 4 > (int)ctx.localTableBytes.size()) return 0.0f;
            int32_t val;
            memcpy(&val, data + adjustedOffset, sizeof(int32_t));
            return (float)val;
        }
        case INTEGER_TYPE: {
            if (adjustedOffset + 2 > (int)ctx.localTableBytes.size()) return 0.0f;
            short val;
            memcpy(&val, data + adjustedOffset, sizeof(short));
            return (float)val;
        }
        case BYTE_TYPE:
            if (adjustedOffset >= (int)ctx.localTableBytes.size()) return 0.0f;
            return (float)data[adjustedOffset];
        default:
            return 0.0f;
    }
}

void CVirtualProgramEngine::WriteLocalVariable(ProgramContext& ctx, unsigned char varType, int offset, float value)
{
    if (offset < 0) return;

    // Check mapping first: if this local variable maps to a global point, write there
    for (const auto& m : ctx.localVarMappings) {
        if (m.localOffset == offset) {
            if (m.globalPointType != 0 && m.globalIndex >= 0) {
                WriteVariable(0, m.globalPointType, 0, (unsigned short)m.globalIndex, value);
                return;
            }
            break;  // found mapping but not global — use local storage
        }
    }

    // Not mapped to global: write to localTableBytes with corrected base offset
    int adjustedOffset = ctx.localTableDataStart + offset;
    unsigned char* data = ctx.localTableBytes.data();

    switch (varType) {
        case FLOAT_TYPE: {
            if (adjustedOffset + 4 > (int)ctx.localTableBytes.size()) return;
            float val = value;
            memcpy(data + adjustedOffset, &val, sizeof(float));
            break;
        }
        case LONG_TYPE: {
            if (adjustedOffset + 4 > (int)ctx.localTableBytes.size()) return;
            int32_t val = (int32_t)value;
            memcpy(data + adjustedOffset, &val, sizeof(int32_t));
            break;
        }
        case INTEGER_TYPE: {
            if (adjustedOffset + 2 > (int)ctx.localTableBytes.size()) return;
            short val = (short)value;
            memcpy(data + adjustedOffset, &val, sizeof(short));
            break;
        }
        case BYTE_TYPE: {
            if (adjustedOffset >= (int)ctx.localTableBytes.size()) return;
            data[adjustedOffset] = (unsigned char)value;
            break;
        }
        default:
            break;
    }
}

//=============================================================================
// Variable read/write
//=============================================================================
float CVirtualProgramEngine::ReadVariable(unsigned char varType, unsigned char pointType,
                                           unsigned char panel, unsigned short numPoint)
{
    int index = numPoint;

    switch (pointType) {
        case KEY_INPUT:
            if (index >= 0 && index < (int)m_Input_data.size()) {
                int32_t val = m_Input_data[index].value;
                // digital_analog: 1=analog(value*10), 0=digital(0/1)
                if (m_Input_data[index].digital_analog == 1) {
                    return (float)val / 1000.0f;  // analog: value stored as value*1000
                } else {
                    return (float)val;  // digital: 0 or 1
                }
            }
            break;

        case KEY_OUTPUT:
            if (index >= 0 && index < (int)m_Output_data.size()) {
                int32_t val = m_Output_data[index].value;
                // digital_analog: 1=analog(value*10), 0=digital(0/1)
                if (m_Output_data[index].digital_analog == 1) {
                    return (float)val / 1000.0f;
                } else {
                    return (float)val;
                }
            }
            break;

        case KEY_VARIABLE:
            if (index >= 0 && index < (int)m_Variable_data.size()) {
                int32_t val = m_Variable_data[index].value;
                // In this project variables are stored/scaled as value*1000.
                // Treat VAR points as analog by default to match UI and compiler encoding.
                return (float)val / 1000.0f;
            }
            break;

        case KEY_PROGRAM:
            // Program state
            if (index >= 0 && index < (int)m_Program_data.size()) {
                return (m_Program_data[index].on_off ? 1.0f : 0.0f);
            }
            break;

        case KEY_PID:
            if (index >= 0 && index < (int)m_controller_data.size()) {
                int32_t val = m_controller_data[index].value;
                return (float)val / 1000.0f;
            }
            break;

        case KEY_SCH:
            // Schedule state
            if (index >= 0 && index < (int)m_Weekly_data.size()) {
                return (m_Weekly_data[index].value ? 1.0f : 0.0f);
            }
            break;

        case KEY_HOL:
            // Holiday state
            return 0.0f;
    }

    return 0.0f;
}

void CVirtualProgramEngine::WriteVariable(unsigned char varType, unsigned char pointType,
                                          unsigned char panel, unsigned short numPoint, float value)
{
    int index = numPoint;

    switch (pointType) 
    {
        case KEY_INPUT: 
        if (index >= 0 && index < (int)m_Input_data.size()) 
        {
            Str_in_point& in = m_Input_data[index];
            if (in.digital_analog == 1) {
                in.value = (int32_t)(value * 1000);
            }
            else {
                in.value = (int32_t)(value > 0 ? 1 : 0);
                in.control = (int32_t)(value > 0 ? 1 : 0);
            }
           
            NotifyInputChanged(index);
        }
        break;
        case KEY_OUTPUT: {
            if (index >= 0 && index < (int)m_Output_data.size()) {
                Str_out_point& out = m_Output_data[index];
                // digital_analog: 1=analog, 0=digital
                if (out.digital_analog == 1) {
                    out.value = (int32_t)(value * 1000);  // analog: store as value*1000
                } else {
                    out.value = (int32_t)(value > 0 ? 1 : 0);  // digital: 0 or 1
                    out.control = (int32_t)(value > 0 ? 1 : 0);
                }
                NotifyOutputChanged(index);
            }
            break;
        }

        case KEY_VARIABLE: {
            if (index >= 0 && index < (int)m_Variable_data.size()) {
                Str_variable_point& var = m_Variable_data[index];
                // Variables are stored as value*1000 (analog) in this codebase.
                // Do not clamp to 0/1 based on digital_analog since many VARs default to 0.
                if (var.digital_analog == 1) {
                    var.value = (int32_t)(value * 1000);
                }
                else {
                    var.value = (int32_t)(value > 0 ? 1 : 0);  // digital: 0 or 1
                    var.control = (int32_t)(value > 0 ? 1 : 0);  // ← 这行
                }


                NotifyVariableChanged(index);
            }
            break;
        }

        case KEY_PID: {
            if (index >= 0 && index < (int)m_controller_data.size()) {
                m_controller_data[index].value = (int32_t)(value * 1000);
            }
            break;
        }

        //case LOCAL_VAR:
        case LOCAL_VARIABLE: {
            // Write to local variable
            if (index >= 0 && index < VM_MAX_LOCAL_VARS) {
                m_Programs[panel].localVariables[index] = value;
            }
            break;
        }
    }
}

//=============================================================================
// Read global data
//=============================================================================
int32_t CVirtualProgramEngine::GetInputValue(int index)
{
    if (index >= 0 && index < (int)m_Input_data.size()) {
        return m_Input_data[index].value;
    }
    return 0;
}

int32_t CVirtualProgramEngine::GetOutputValue(int index)
{
    if (index >= 0 && index < (int)m_Output_data.size()) {
        return m_Output_data[index].value;
    }
    return 0;
}

int32_t CVirtualProgramEngine::GetVariableValue(int index)
{
    if (index >= 0 && index < (int)m_Variable_data.size()) {
        return m_Variable_data[index].value;
    }
    return 0;
}

int32_t CVirtualProgramEngine::GetPvarValue(int index)
{
    if (index >= 0 && index < (int)m_pvar_data.size()) {
        return m_pvar_data[index].value;
    }
    return 0;
}

void CVirtualProgramEngine::SetInputValueInternal(int index, int32_t value)
{
    if (index >= 0 && index < (int)m_Input_data.size()) {
        m_Input_data[index].value = value;
        NotifyInputChanged(index);
    }
}

void CVirtualProgramEngine::SetOutputValueInternal(int index, int32_t value)
{
    if (index >= 0 && index < (int)m_Output_data.size()) {
        m_Output_data[index].value = value;
        NotifyOutputChanged(index);
    }
}

void CVirtualProgramEngine::SetVariableValueInternal(int index, int32_t value)
{
    if (index >= 0 && index < (int)m_Variable_data.size()) {
        m_Variable_data[index].value = value;
        NotifyVariableChanged(index);
    }
}

void CVirtualProgramEngine::SetPvarValueInternal(int index, int32_t value)
{
    if (index >= 0 && index < (int)m_pvar_data.size()) {
        m_pvar_data[index].value = value;
        NotifyPvarChanged(index);
    }
}

void CVirtualProgramEngine::SetInputValue(int index, int32_t value)
{
    CSingleLock lock(&m_csMain, TRUE);
    SetInputValueInternal(index, value);
}

void CVirtualProgramEngine::SetOutputValue(int index, int32_t value)
{
    CSingleLock lock(&m_csMain, TRUE);
    SetOutputValueInternal(index, value);
}

void CVirtualProgramEngine::SetVariableValue(int index, int32_t value)
{
    CSingleLock lock(&m_csMain, TRUE);
    SetVariableValueInternal(index, value);
}

void CVirtualProgramEngine::SetPvarValue(int index, int32_t value)
{
    CSingleLock lock(&m_csMain, TRUE);
    SetPvarValueInternal(index, value);
}

//=============================================================================
// Time functions
//=============================================================================
float CVirtualProgramEngine::GetTimeFunc(unsigned char funcCode)
{
    SYSTEMTIME st;
    GetLocalTime(&st);

    switch (funcCode) {
        case TIME: {
            // Return seconds since midnight
            return (float)(st.wHour * 3600 + st.wMinute * 60 + st.wSecond);
        }

        case DOW: {
            // Return day of week (1=Sunday, 7=Saturday)
            return (float)st.wDayOfWeek + 1;
        }

        case DOM: {
            // Return day of month
            return (float)st.wDay;
        }

        case DOY: {
            // Return day of year
            SYSTEMTIME startOfYear = { (WORD)st.wYear, (WORD)1, 0, (WORD)1, 0, 0, 0, 0 };
            FILETIME ft;
            SystemTimeToFileTime(&startOfYear, &ft);
            ULARGE_INTEGER start, current;
            start.LowPart = ft.dwLowDateTime;
            start.HighPart = ft.dwHighDateTime;
            SystemTimeToFileTime(&st, &ft);
            current.LowPart = ft.dwLowDateTime;
            current.HighPart = ft.dwHighDateTime;
            return (float)((current.QuadPart - start.QuadPart) / (10000000 * 60 * 60 * 24)) + 1;
        }

        case MOY: {
            // Return month
            return (float)st.wMonth;
        }

        case INTERVAL: {
            // Return last scan interval (seconds)
            DWORD now = GetTickCount();
            DWORD interval = (now - m_dwLastScanTime[m_Programs[0].programIndex]) / 1000;
            return (float)interval;
        }

        case SCANS: {
            return (float)m_nScanCount;
        }

        case RUNTIME: {
            return (float)(GetTickCount() - m_dwProgramStartTime[m_Programs[0].programIndex]) / 1000.0f;
        }

        case ABS:
        case SQR:
        case LN:
        case LN_1:
        case SIN:
        case COS: {
            // These functions need arguments
            if (m_FunctionArgs.empty()) return 0.0f;
            float val = m_FunctionArgs.top();
            m_FunctionArgs.pop();

            switch (funcCode) {
                case ABS: return fabsf(val);
                case SQR: return sqrtf(val);
                case LN: return logf(val);
                case LN_1: return logf(val + 1.0f);
                case SIN: return sinf(val);
                case COS: return cosf(val);
            }
            break;
        }

        case _INT: {
            if (m_FunctionArgs.empty()) return 0.0f;
            float val = m_FunctionArgs.top();
            m_FunctionArgs.pop();
            return floorf(val);
        }

        case MAX: {
            if (m_FunctionArgs.size() < 2) return 0.0f;
            float b = m_FunctionArgs.top(); m_FunctionArgs.pop();
            float a = m_FunctionArgs.top(); m_FunctionArgs.pop();
            return (a > b) ? a : b;
        }

        case MIN: {
            if (m_FunctionArgs.size() < 2) return 0.0f;
            float b = m_FunctionArgs.top(); m_FunctionArgs.pop();
            float a = m_FunctionArgs.top(); m_FunctionArgs.pop();
            return (a < b) ? a : b;
        }

        case AVG: {
            if (m_FunctionArgs.empty()) return 0.0f;
            int count = (int)m_FunctionArgs.top(); m_FunctionArgs.pop();
            float sum = 0.0f;
            for (int i = 0; i < count && !m_FunctionArgs.empty(); i++) {
                sum += m_FunctionArgs.top();
                m_FunctionArgs.pop();
            }
            return (count > 0) ? (sum / count) : 0.0f;
        }

        //
        case JAN: return 1.0f;
        case FEB: return 2.0f;
        case MAR: return 3.0f;
        case APR: return 4.0f;
        case MAY: return 5.0f;
        case JUN: return 6.0f;
        case JUL: return 7.0f;
        case AUG: return 8.0f;
        case SEP: return 9.0f;
        case OCT: return 10.0f;
        case NOV: return 11.0f;
        case DEC: return 12.0f;

        // PID
        case PIDPROP:
        case PIDRATE:
        case PIDRESET: {
            if (m_FunctionArgs.empty()) return 0.0f;
            int ctrlIndex = (int)m_FunctionArgs.top(); m_FunctionArgs.pop();
            if (ctrlIndex < 0 || ctrlIndex >= (int)m_controller_data.size()) {
                return 0.0f;
            }
            const Str_controller_point& ctrl = m_controller_data[ctrlIndex];
            switch (funcCode) {
                case PIDPROP: return (float)ctrl.proportional / 10.0f;
                case PIDRATE: return (float)ctrl.reset;
                case PIDRESET: return (float)ctrl.rate;
            }
            break;
        }

        // Modbus  (stub)
        case MB_BR:
        case MB_BW:
        case MB_BW_COIL:
            return 0.0f;  // Modbus

        //  (stub)
        case COM_1:
        case OUTPUTD:
        case INKEYD:
        case CLEARPORT:
            return 0.0f;  //

        //  (stub)
        case REMOTE_GET:
        case REMOTE_SET:
            return 0.0f;  //

        //  (stub)
        case PHONE:
        case HANGUP:
            return 0.0f;  //

        default:
            return 0.0f;
    }

    return 0.0f;
}

//=============================================================================
// Schedule/Holiday check
//=============================================================================
// Note: Str_weekly_routine_point and Str_annual_routine_point have no day/time/value array fields
// Weekly/Annual schedule detailed time data is stored in separate structures like Wr_one_day
// Simplified: use .value field for current schedule state (synced from device)
BOOL CVirtualProgramEngine::CheckWeeklySchedule(int schedIndex)
{
    if (schedIndex < 0 || schedIndex >= (int)m_Weekly_data.size()) {
        return FALSE;
    }

    // .value field in Str_weekly_routine_point represents schedule on/off state
    // value=1 means on, value=0 means off
    return (m_Weekly_data[schedIndex].value != 0);
}

BOOL CVirtualProgramEngine::CheckAnnualSchedule(int schedIndex)
{
    if (schedIndex < 0 || schedIndex >= (int)m_Annual_data.size()) {
        return FALSE;
    }

    // .value field in Str_annual_routine_point represents holiday on/off state
    // value=1 means active, value=0 means inactive
    return (m_Annual_data[schedIndex].value != 0);
}

//=============================================================================
// PID calculation (simplified)
//=============================================================================
float CVirtualProgramEngine::CalculatePID(int controllerIndex, float input, float setpoint)
{
    if (controllerIndex < 0 || controllerIndex >= (int)m_controller_data.size()) {
        return 0.0f;
    }

    const Str_controller_point& ctrl = m_controller_data[controllerIndex];

    // Simplified PID calculation
    // output = Kp * e + Ki * ∫e dt + Kd * de/dt
    // Using incremental PID here

    float error = setpoint - input;
    float Kp = 1.0f / (ctrl.proportional / 10.0f);  // Proportional coefficient
    float Ki = Kp / (ctrl.reset * 60.0f);           // Integral coefficient (reset = integral time)
    float Kd = Kp * (ctrl.rate * 60.0f);             // Derivative coefficient (rate = derivative time)

    // Simplified: return proportional term only
    float output = Kp * error;

    // Limit output range (-100%% to 100%%)
    output = (output > 100.0f) ? 100.0f : ((output < -100.0f) ? -100.0f : output);

    return output;
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

    BOOL result = ExecuteInstruction(ctx);

    if (!result) {
        ctx.state = STATE_ERROR;
    }

    return result;
}

BOOL CVirtualProgramEngine::StepOver(int programIndex)
{
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return FALSE;
    }

    // StepOver: execute current subroutine call then pause
    // Simplified: single step until call depth decreases
    CSingleLock lock(&m_csMain, TRUE);

    int targetDepth = m_nCallDepth[programIndex];

    while (m_nCallDepth[programIndex] >= targetDepth &&
           m_Programs[programIndex].state != STATE_ERROR &&
           m_Programs[programIndex].state != STATE_STOPPED) {
        if (!ExecuteInstruction(m_Programs[programIndex])) {
            m_Programs[programIndex].state = STATE_ERROR;
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CVirtualProgramEngine::StepOut(int programIndex)
{
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return FALSE;
    }

    // StepOut: execute until returning to previous call level
    CSingleLock lock(&m_csMain, TRUE);

    int targetDepth = m_nCallDepth[programIndex] - 1;
    if (targetDepth < 0) targetDepth = 0;

    while (m_nCallDepth[programIndex] > targetDepth &&
           m_Programs[programIndex].state != STATE_ERROR &&
           m_Programs[programIndex].state != STATE_STOPPED) {
        if (!ExecuteInstruction(m_Programs[programIndex])) {
            m_Programs[programIndex].state = STATE_ERROR;
            return FALSE;
        }
    }

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

const std::stack<float>& CVirtualProgramEngine::GetStack(int programIndex) const
{
    static std::stack<float> empty;
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return empty;
    }
    return m_Programs[programIndex].valueStack;
}

const std::vector<float>& CVirtualProgramEngine::GetLocalVariables(int programIndex) const
{
    static std::vector<float> empty;
    if (programIndex < 0 || programIndex >= BAC_PROGRAM_ITEM_COUNT) {
        return empty;
    }
    return m_Programs[programIndex].localVariables;
}

//=============================================================================
// Bytecode disassembly
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

        //
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
                    inst.operandValues.push_back((float)code[ip]);     // varType
                    inst.operandValues.push_back((float)code[ip + 1]);  // pointType
                    inst.operandValues.push_back((float)code[ip + 2]);  // panel
                    inst.operandValues.push_back((float)(*(short*)(code + ip + 3)));  // numPoint
                    ip += 6;
                }
                break;
            }
        }

        result.push_back(inst);
        lineNum++;
    }

    // Save to cache
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
// UI notification (using global HWND)
//=============================================================================
void CVirtualProgramEngine::NotifyInputChanged(int index)
{
    // Send custom message to refresh UI
    // Need to handle WM_REFRESH_BAC_INPUT_LIST message in dialog
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
    // PVAR uses Variable window for refresh
#ifdef WM_REFRESH_BAC_VARIABLE_LIST
    if (m_variable_dlg_hwnd && ::IsWindow(m_variable_dlg_hwnd)) {
        ::PostMessage(m_variable_dlg_hwnd, WM_REFRESH_BAC_VARIABLE_LIST,
                      REFRESH_ON_ITEM, index + 1000);  // Use offset to distinguish
    }
#endif
}

void CVirtualProgramEngine::NotifyProgramStateChanged(int index)
{
    // Notify program state changed
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

    // Output debug info
    TRACE("Program %d Error at IP %d: %s\n", ctx.programIndex, ctx.ip, buffer);
}
