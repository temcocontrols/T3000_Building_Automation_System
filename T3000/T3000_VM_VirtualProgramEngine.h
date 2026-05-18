#ifndef VIRTUAL_PROGRAM_ENGINE_H
#define VIRTUAL_PROGRAM_ENGINE_H

#pragma once

#include <windows.h>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <set>
#include <afxtempl.h>
#include <afxmt.h>

//=============================================================================
// Push conflicting Windows SDK / C++ macro definitions
// These macros (MAX, MIN, RGB, AND, OR, NOT, XOR, etc.) will replace
// enum values in this header, causing compilation failures.
// We push them, undef them for our enum definitions, then pop to restore.
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

//=============================================================================
// Type definitions copied from PARSER.h (defined unconditionally)
//=============================================================================

// Macro definitions
#define LOCAL_VARIABLE 0x82
#define FLOAT_TYPE     0x83
#define LONG_TYPE      0x84
#define INTEGER_TYPE   0x85
#define BYTE_TYPE      0x86
#define STRING_TYPE    0x87
#define FLOAT_TYPE_ARRAY     0x88
#define LONG_TYPE_ARRAY      0x89
#define INTEGER_TYPE_ARRAY   0x8A
#define BYTE_TYPE_ARRAY      0x8B
#define STRING_TYPE_ARRAY    0x8C

#define LOCAL_VAR 1
#define POINT_VAR 2
#define POINT_REMOTE_VAR 3
#define LABEL_VAR 4

#define LOCAL_POINT_PRG  0x09C
#define CONST_VALUE_PRG  0x09D
#define REMOTE_POINT_PRG 0x09E

// Token type enum
enum tok_types {
    DELIMITER, NUMBER, IDENTIFIER, KEYWORD, TEMP, BLOCKX, TIMES, COLON, SEMICOLON,
    GE=0x60, LE=0x61, NL=0x62, POW=0x65, MOD=0x66, MUL=0x67, DIV=0x68, MINUSUNAR=0x69, PLUS=0x6B,
    MINUS=0x6C, LT=0x6D, GT=0x6E,
    EQ=0x71, NE=0x72, XOR=0x73, AND=0x74, OR=0x75, NOT=0x76, EOI=0x77, TTIME=0x78, BIT_AND=0x79, BIT_OR=0x7A, INT_DIV=0x7B
};

// Command token enum
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
    _DATE=0x0A0, PTIME=0x0A1, SENSOR_ON=0x0A4, SENSOR_OFF=0x0A5, TO=0x0A6, STEP=0x0A7, THEN=0x0A8,
    LET=0x0A9, ALARM_AT_ALL=0x0AA, FINISHED=0x0AB, PRINT_ALARM_AT=0x0AC, PRINT_ALARM_AT_ALL=0x0AD,
    ARG=0x0AE, DO=0x0AF, WHILE=0x0B0, SWITCH=0x0B1, EOL=0x0B2, STRING=0x0B3
};

// Function token enum
enum tok_func {
    SUN=7, MON=8, TUE=2, WED=3, THU=4, FRI=5, SAT=6, COM_1=0x10,
    ABS=0x32, AVG=0x33, DOY=0x34, DOW=0x35, _INT=0x36, MAX=0x37, MIN=0x38, SQR=0x39, Tbl=0x3A,
    TIME=0x3B, TIME_ON=0x3C, TIME_OFF=0x3D, INTERVAL=0x3E, TIME_FORMAT=0x3F,
    WR_ON=0x40, WR_OFF=0x41, UNACK=0x42, _Status=0x47, RUNTIME=0x48, SCANS=0x4B, POWER_LOSS=0x4C, LN=0x4D, LN_1=0x4E,
    OUTPUTD=0x50, INKEYD=0x51, DOM=0x52, MOY=0x53, PIDPROP=0x56, PIDRATE=0x57, PIDRESET=0x58, CLEARPORT=0x59,
    JAN=0x0C1, FEB=0x0C2, MAR=0x0C3, APR=0x0C4, MAY=0x0C5, JUN=0x0C6, JUL=0x0C7, AUG=0x0C8,
    SEP=0x0C9, OCT=0x0CA, NOV=0x0CB, DEC=0x0CC, MB_BR=0x0CD, MB_BW=0x0CE, MB_BW_COIL=0x0CF, PVAR=0x0D0, SIN=0x0D1, COS=0x0D2, RGB=0x0D3
};

// BACnet point address struct
#pragma pack(push)
#pragma pack(1)
typedef struct {
    unsigned char identification_code1;
    unsigned char identification_code2;
    unsigned char identification_code3;
    unsigned char identification_code4;
    unsigned char instance_high_byte;
    unsigned char instance_mid_byte;
    unsigned char instance_low_byte;
    unsigned char point_type;
    unsigned char obj_number_high;
    unsigned char obj_number_mid;
    unsigned char obj_number_low;
    unsigned char reserve[3];
} Point_Bacnet;
#pragma pack(pop)

// Variable table struct
struct variable_table {
    char name[35];
    char label[9];
    unsigned char type;
    unsigned char var_type;
    int  l;
    int  c;
    unsigned char network;
    unsigned char sub_panel;
    unsigned char panel;
    unsigned char point_type;
    unsigned short num_point;
    Point_Bacnet b_point;
};

//=============================================================================
// Constants
//=============================================================================
#ifndef BAC_PROGRAM_ITEM_COUNT
#define BAC_PROGRAM_ITEM_COUNT 16
#endif

#define VM_MAX_STACK_DEPTH     256
#define VM_MAX_INSTRUCTIONS    10000    // Max instructions per scan
#define VM_DEFAULT_SCAN_MS     1000     // Default scan period 1s
#define VM_MAX_LOCAL_VARS      256

// Program state
enum ProgramState {
    STATE_STOPPED = 0,
    STATE_RUNNING = 1,
    STATE_PAUSED = 2,
    STATE_WAITING = 3,
    STATE_ERROR = 4
};

//=============================================================================
// VM instruction (parse result of single instruction)
//=============================================================================
struct VMInstruction {
    unsigned char opcode;
    std::vector<unsigned char> operands;  // Raw operand bytes
    std::vector<float> operandValues;     // Parsed values
    int lineNumber;                        // Source line number
};

//=============================================================================
// Local variable → global point mapping
// When a local_table variable name matches a global point's label
// (e.g. "COUNT" matches m_Variable_data[0].label),
// reads/writes to that local variable must be redirected to the global point.
//=============================================================================
struct LocalVarMapping {
    int localOffset;               // offset as it appears in bytecode (j+1 in local_table)
    unsigned char varType;         // FLOAT_TYPE, LONG_TYPE, etc.
    std::string name;              // variable name extracted from local_table
    unsigned char globalPointType; // KEY_VARIABLE/KEY_INPUT/KEY_OUTPUT/... (0 = not mapped)
    int globalIndex;               // index in global data vector (-1 = not mapped)
};

//=============================================================================
// Single program runtime context
//=============================================================================
struct ProgramContext {
    int programIndex;                      // Program index 0-15
    std::vector<unsigned char> bytecode;   // Program bytecode
    std::vector<variable_table> localTable; // Local variable table
    std::vector<variable_table> timeTable;  // Time table

    // Raw local table data bytes (Bug 5 fix: load bytes after 0xFE terminator)
    // Layout: [2 bytes Byte=0][2 bytes ind_local_table][local_table data...][time_table data...]
    std::vector<unsigned char> localTableBytes;

    // Parsed mapping: local variable offset → global point
    std::vector<LocalVarMapping> localVarMappings;
    int localTableDataStart;               // Byte offset of local_table data within localTableBytes (typically 4)

    int ip;                                // Instruction pointer
    int instructionCount;                  // Executed instruction count
    ProgramState state;                    // Running state

    // Stacks
    std::stack<float> valueStack;
    std::stack<int> returnStack;          // GOSUB return address stack

    // Local variable storage
    std::vector<float> localVariables;

    // FOR/NEXT loop stack
    struct ForLoop {
        int targetIp;
        float counter;
        float endValue;
        float step;
        unsigned char varIndex;
    };
    std::stack<ForLoop> forStack;

    // Wait state
    DWORD waitUntil;                       // Wait until timestamp
    bool waiting;

    // Error info
    std::string errorMessage;
    int errorLine;

    // Breakpoints
    std::set<int> breakpoints;

    // Last exec time (for INTERVAL calc)
    DWORD lastExecTime;
    DWORD totalRuntime;                    // Accumulated runtime (ms)

    ProgramContext() {
        programIndex = 0;
        ip = 0;
        instructionCount = 0;
        state = STATE_STOPPED;
        waiting = false;
        waitUntil = 0;
        errorLine = -1;
        lastExecTime = 0;
        totalRuntime = 0;
        localTableDataStart = 4;  // default: 2-byte Byte + 2-byte ind_local_table
    }

    void Reset() {
        ip = 0;
        instructionCount = 0;
        state = STATE_STOPPED;
        while (!valueStack.empty()) valueStack.pop();
        while (!returnStack.empty()) returnStack.pop();
        while (!forStack.empty()) forStack.pop();
        waiting = false;
        waitUntil = 0;
        errorMessage.clear();
        errorLine = -1;
        localVariables.clear();
        localVariables.resize(VM_MAX_LOCAL_VARS, 0.0f);
        // Note: localTableBytes and localVarMappings are NOT cleared here;
        // they persist across scans and are only loaded once via LoadProgram.
    }
};

//=============================================================================
// Virtual machine engine class
//=============================================================================
class CVirtualProgramEngine
{
public:
    CVirtualProgramEngine();
    virtual ~CVirtualProgramEngine();

    // Initialize/Shutdown
    BOOL Initialize();
    void Shutdown();

    // Bytecode loading
    BOOL LoadProgram(int programIndex, const unsigned char* bytecode, int byteLen);
    BOOL LoadLocalTable(int programIndex, const unsigned char* data, int dataLen);
    BOOL LoadTimeTable(int programIndex, const unsigned char* data, int dataLen);
    void ClearProgram(int programIndex);

    // Runtime control
    void StartAll();
    void StopAll();
    void PauseAll();
    void ResumeAll();

    void StartProgram(int programIndex);
    void StopProgram(int programIndex);
    void PauseProgram(int programIndex);
    void ResumeProgram(int programIndex);

    // Scan control
    void SetScanInterval(DWORD intervalMs);
    DWORD GetScanInterval() const { return m_dwScanInterval; }
    void ExecuteOneScan();

    // Debug control
    void SetBreakpoint(int programIndex, int lineNumber, BOOL enable);
    void ClearAllBreakpoints(int programIndex);
    BOOL SingleStep(int programIndex);
    BOOL StepOver(int programIndex);
    BOOL StepOut(int programIndex);

    // State query
    ProgramState GetProgramState(int programIndex) const;
    int GetCurrentLine(int programIndex) const;
    int GetInstructionCount(int programIndex) const;
    const std::string& GetErrorMessage(int programIndex) const;
    const std::stack<float>& GetStack(int programIndex) const;
    const std::vector<float>& GetLocalVariables(int programIndex) const;

    // Bytecode parsing (for debug display)
    std::vector<VMInstruction> Disassemble(int programIndex) const;

    // Statistics
    int GetActiveProgramCount() const;
    DWORD GetTotalExecutionTime() const { return m_dwTotalExecTime; }

    // Manual value setting (external call)
    void SetInputValue(int inputIndex, int32_t value);
    void SetOutputValue(int outputIndex, int32_t value);
    void SetVariableValue(int varIndex, int32_t value);
    void SetPvarValue(int pvarIndex, int32_t value);

private:
    // Internal execution
    void ExecuteAllPrograms();
    BOOL ExecuteProgram(ProgramContext& ctx);
    BOOL ExecuteInstruction(ProgramContext& ctx);

    // Bytecode reading helper
    float ReadFloat(const unsigned char* ptr);
    int ReadShort(const unsigned char* ptr);
    int ReadByte(const unsigned char* ptr);

    // Operand parsing
    BOOL ParseOperand(const unsigned char* ip, ProgramContext& ctx, float& outValue, int& bytesConsumed);

    // Variable read/write
    float ReadVariable(unsigned char varType, unsigned char pointType,
                       unsigned char panel, unsigned short numPoint);
    void WriteVariable(unsigned char varType, unsigned char pointType,
                       unsigned char panel, unsigned short numPoint, float value);

    // Local variable read/write (Bug 5 fix)
    float ReadLocalVariable(ProgramContext& ctx, unsigned char varType, int offset);
    void WriteLocalVariable(ProgramContext& ctx, unsigned char varType, int offset, float value);

    // Parse local_table and build variable→global-point mapping
    void ParseLocalTable(ProgramContext& ctx);

    // Read global data
    int32_t GetInputValue(int index);
    int32_t GetOutputValue(int index);
    int32_t GetVariableValue(int index);
    int32_t GetPvarValue(int index);
    void SetInputValueInternal(int index, int32_t value);
    void SetOutputValueInternal(int index, int32_t value);
    void SetVariableValueInternal(int index, int32_t value);
    void SetPvarValueInternal(int index, int32_t value);

    // Time functions
    float GetTimeFunc(unsigned char funcCode);
    float ExecFunction(ProgramContext& ctx, unsigned char funcCode);
    BOOL CheckWeeklySchedule(int schedIndex);
    BOOL CheckAnnualSchedule(int schedIndex);

    // PID calculation
    float CalculatePID(int controllerIndex, float input, float setpoint);

    // Instruction execution
    BOOL ExecAssign(ProgramContext& ctx);
    BOOL ExecGoto(ProgramContext& ctx);
    BOOL ExecGotoIf(ProgramContext& ctx);
    BOOL ExecGosub(ProgramContext& ctx);
    BOOL ExecReturn(ProgramContext& ctx);
    BOOL ExecIf(ProgramContext& ctx);
    BOOL ExecFor(ProgramContext& ctx);
    BOOL ExecNext(ProgramContext& ctx);
    BOOL ExecWhile(ProgramContext& ctx);
    BOOL ExecDo(ProgramContext& ctx);
    BOOL ExecEndPrg(ProgramContext& ctx);
    BOOL ExecWait(ProgramContext& ctx);
    BOOL ExecPrint(ProgramContext& ctx);
    BOOL ExecStartPrg(ProgramContext& ctx);
    BOOL ExecStop(ProgramContext& ctx);
    BOOL ExecCall(ProgramContext& ctx);

    // Operator execution
    float ExecBinaryOp(unsigned char op, float a, float b);
    float ExecUnaryOp(unsigned char op, float a);

    // UI notification (using HWND)
    void NotifyInputChanged(int index);
    void NotifyOutputChanged(int index);
    void NotifyVariableChanged(int index);
    void NotifyPvarChanged(int index);
    void NotifyProgramStateChanged(int index);

    // Error handling
    void SetError(ProgramContext& ctx, const char* fmt, ...);

    // Worker thread
    static UINT WorkerThreadProc(LPVOID pParam);
    void WorkerThread();

private:
    // Scan interval
    DWORD m_dwScanInterval;

    // Thread synchronization
    CCriticalSection m_csMain;
    CEvent m_eventShutdown;
    CEvent m_eventPause;
    CWinThread* m_pWorkerThread;
    BOOL m_bRunning;
    BOOL m_bPaused;

    // Program context array (16 programs)
    ProgramContext m_Programs[BAC_PROGRAM_ITEM_COUNT];

    // Bytecode disassembly cache
    std::map<int, std::vector<VMInstruction>> m_DisasmCache;

    // Statistics
    DWORD m_dwTotalExecTime;
    int m_nScanCount;

    // Function argument stack (for functions with args)
    std::stack<float> m_FunctionArgs;

    // INTERVAL tracking
    DWORD m_dwLastScanTime[BAC_PROGRAM_ITEM_COUNT];

    // RUNTIME tracking
    DWORD m_dwProgramStartTime[BAC_PROGRAM_ITEM_COUNT];

    // Call depth (for StepOut)
    int m_nCallDepth[BAC_PROGRAM_ITEM_COUNT];

    // Global data vectors are NOT members of this class.
    // They are declared as extern globals in global_variable_extern.h
    // and accessed directly by ReadVariable/WriteVariable.
};

// Global instance
extern CVirtualProgramEngine g_VirtualProgramEngine;

//=============================================================================
// Pop conflicting Windows SDK / C++ macro definitions (restore original values)
// This must be after all enum definitions that use these names
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

#endif // VIRTUAL_PROGRAM_ENGINE_H
