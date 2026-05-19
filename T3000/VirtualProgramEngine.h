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

// Include the adapter layer for decode.c
#include "vm_shim.h"

//=============================================================================
// Constants
//=============================================================================
#ifndef BAC_PROGRAM_ITEM_COUNT
#define BAC_PROGRAM_ITEM_COUNT 16
#endif

#define VM_MAX_STACK_DEPTH     256
#define VM_MAX_LOCAL_VARS      256

//=============================================================================
// Program state
//=============================================================================
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
// Simplified: bytecode/localTableBytes/ip/state kept;
// own execution stacks (valueStack/forStack) removed — decode.c manages them.
//=============================================================================
struct ProgramContext {
    int programIndex;                      // Program index 0-15
    std::vector<unsigned char> bytecode;   // Program bytecode (passed to decode.c)
    std::vector<unsigned char> localTableBytes; // Raw local table data bytes

    int ip;                                // Instruction pointer (unused in decode.c path, kept for debug)
    int instructionCount;                  // Executed instruction count
    ProgramState state;                    // Running state

    // FOR/NEXT loop stack (kept for debug compatibility)
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
    }

    void Reset() {
        ip = 0;
        instructionCount = 0;
        state = STATE_STOPPED;
        while (!forStack.empty()) forStack.pop();
        waiting = false;
        waitUntil = 0;
        errorMessage.clear();
        errorLine = -1;
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

    // decode.c integration: raw bytecode execution
    // Calls exec_program(current_prg, prog_code) from decode.c
    BOOL ExecuteProgram(ProgramContext& ctx);

private:
    // Execute all programs in one scan
    void ExecuteAllPrograms();

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

    // INTERVAL tracking
    DWORD m_dwLastScanTime[BAC_PROGRAM_ITEM_COUNT];

    // RUNTIME tracking
    DWORD m_dwProgramStartTime[BAC_PROGRAM_ITEM_COUNT];

    // Call depth (for StepOver/StepOut)
    int m_nCallDepth[BAC_PROGRAM_ITEM_COUNT];

    // Note: Global data vectors (m_Input_data, m_Output_data, etc.) are NOT
    // members of this class. They are declared as extern globals in
    // global_variable_extern.h and accessed via get_point_value / put_point_value.
};

// Global instance
extern CVirtualProgramEngine g_VirtualProgramEngine;

#endif // VIRTUAL_PROGRAM_ENGINE_H
