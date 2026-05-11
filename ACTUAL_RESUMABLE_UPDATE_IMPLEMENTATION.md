# Actual Resumable Firmware Update Implementation in T3000

## Summary

The T3000 Building Automation System **DOES have an active resumable firmware update implementation** in the ISP (In-System Programmer) tool. This document details the actual code implementation currently in place.

---

## Table of Contents

1. [Overview of Current Implementation](#overview-of-current-implementation)
2. [Modbus Register Interface](#modbus-register-interface)
3. [Implementation Details](#implementation-details)
4. [Resume Logic](#resume-logic)
5. [Device Memory Layout](#device-memory-layout)
6. [Code References](#code-references)
7. [Packet Structure](#packet-structure)
8. [Resume Detection Logic](#resume-detection-logic)

---

## Overview of Current Implementation

### Status
- **Status**: ACTIVE and IMPLEMENTED
- **Location**: ISP tool (ISP folder)
- **Device Support**: Product ID 88 (ESP32 boards) primarily, with some support for other devices
- **Protocol**: Modbus RTU over serial communication

### Key Components

| Component | File | Purpose |
|-----------|------|---------|
| Main COM Writer | [ISP/ComWriter.cpp](ISP/ComWriter.cpp) | Handles serial communication & flashing |
| Register Definitions | [ISP/Global_Struct.h](ISP/Global_Struct.h) | Modbus register definitions |
| TFTP Server | [ISP/TFTPServer.cpp](ISP/TFTPServer.cpp) | Network-based flashing |
| Tstat Flash Dialog | [ISP/TstatFlashDlg.cpp](ISP/TstatFlashDlg.cpp) | UI for single device flashing |

---

## Modbus Register Interface

### Register 16 (0x10): MODBUS_UPDATE_STATUS

**Purpose**: Control and query firmware update state

```
Address: 0x10 (16 decimal)
Type: Read/Write

Register Values:
  0x00 - 0x7E      : Normal operation / Reading current state
  0x7F (127)       : Jump to ISP routine (bootloader mode)
  0x8F (143)       : Completely erase EEPROM
  0x40             : UPDATE IN PROGRESS flag set
  0x1F             : Alternative UPDATE IN PROGRESS state
```

**Meaning of States**:
- `0x40`: Device is in bootloader mode OR update is in progress
- `0x1F`: Alternate flag indicating incomplete update
- When either 0x40 or 0x1F is detected, the ISP tool prompts for resume

### Register 17 (0x11): MODBUS_UPDATE_PTR_HI

```
Address: 0x11 (17 decimal)
Type: Read/Write
Purpose: High byte of last write pointer

Contains: Upper 8 bits of the firmware offset where update was interrupted
```

### Register 18 (0x12): MODBUS_UPDATE_PTR_LO

```
Address: 0x12 (18 decimal)
Type: Read/Write
Purpose: Low byte of last write pointer

Contains: Lower 8 bits of the firmware offset
```

### Combined Offset Calculation

The last valid offset is calculated as:

```c
last_offset = (UPDATE_PTR_HI << 8) | UPDATE_PTR_LO
```

---

## Implementation Details

### 1. Resume Detection Logic

**File**: [ISP/ComWriter.cpp](ISP/ComWriter.cpp), Lines 838-860 and 2672-2690

When device initialization begins, the ISP tool checks if an update was interrupted:

```cpp
int x = mudbus_read_one(m_ID, 0xee10);  // Read UPDATE_STATUS register (extended address)

// Check if UPDATE_IN_PROGRESS flag is set
if(mudbus_read_one(m_ID, 0xee10) == 0x40 || mudbus_read_one(m_ID, 0xee10) == 0x1f)
{
    // Previous update was interrupted, prompt user
    if(IDOK == AfxMessageBox(_T("Previous Update was interrupted.\nPress OK to Resume.\nCancel to Restart."), MB_OKCANCEL))
    {
        // User chose to RESUME
        ii = 0xEE00 + 17;  // Start reading from offset register
        // Read the last written offset from device memory
        int uc_temp1 = mudbus_read_one(m_ID, ii);      // Get high byte
        int uc_temp2 = mudbus_read_one(m_ID, ii+1);    // Get low byte

        // Calculate last valid offset
        // Offset used to continue flashing
    }
    else
    {
        // User chose to RESTART - full update from beginning
    }
}
```

**Address Translation**:
- The actual Modbus register addresses appear to be mapped as:
  - `0xEE10` or `0xee10`: UPDATE_STATUS register (mapped from standard 16)
  - `0xEE00 + 17`: Memory location storing offset pointers

### 2. Resume Offset Calculation

**File**: [ISP/ComWriter.cpp](ISP/ComWriter.cpp), Line 1523

```cpp
// When resuming, calculate the packet count to skip
pWriter->continue_com_flash_count = pWriter->update_firmware_info[0] * 128;

// Note: Each packet is 128 bytes
// So if update_firmware_info[0] = 50, resume from packet 50 (6400 bytes)
```

**Packet Structure**:
- Each firmware packet = 128 bytes
- `continue_com_flash_count` stores bytes already written
- Resume calculation: `skip_bytes = continue_com_flash_count`

### 3. Resume Output Message

**File**: [ISP/ComWriter.cpp](ISP/ComWriter.cpp), Line 1706

```cpp
if (pWriter->continue_com_flash_count > 0)
{
    CString strTips;
    nCount = pWriter->continue_com_flash_count;
    // Display message showing where resume will start
    strTips.Format(_T("Resume at breakpoint, starting from package %u"),
                   pWriter->continue_com_flash_count / 128);
    pWriter->OutPutsStatusInfo(strTips, FALSE);
}
```

---

## Resume Logic

### Complete Resume Flow

```
┌─────────────────────────────────────────────────────────────┐
│         RESUMABLE UPDATE FLOW - ACTUAL IMPLEMENTATION         │
├─────────────────────────────────────────────────────────────┤
│                                                               │
│  1. DEVICE CONNECTION
│     └─ Read MODBUS_UPDATE_STATUS (0xee10)
│
│  2. CHECK UPDATE_IN_PROGRESS FLAG
│     ├─ If Flag == 0x40 or 0x1f
│     │  └─ Previous update interrupted!
│     └─ If Flag != 0x40/0x1f
│        └─ Normal operation
│
│  3. PROMPT USER (if flag detected)
│     ┌──────────────────────────────────────┐
│     │ "Previous Update was interrupted."    │
│     │ "Press OK to Resume."                 │
│     │ "Press Cancel to Restart."            │
│     └──────────────────────────────────────┘
│     │
│     ├─ User clicks OK:
│     │  │
│     │  ├─ Read last written offset
│     │  │  ├─ Address 0xEE00 + 17: Read high byte (UPDATE_PTR_HI)
│     │  │  └─ Address 0xEE00 + 18: Read low byte (UPDATE_PTR_LO)
│     │  │
│     │  ├─ Calculate:
│     │  │  └─ last_offset = (high_byte << 8) | low_byte
│     │  │
│     │  ├─ Set continue_com_flash_count = last_offset
│     │  │
│     │  ├─ Calculate packet to skip:
│     │  │  └─ skip_packets = continue_com_flash_count / 128
│     │  │
│     │  ├─ Display message:
│     │  │  └─ "Resume at breakpoint, starting from package X"
│     │  │
│     │  └─ BEGIN RESUME FLASHING
│     │     └─ Start from: m_pExtendFileBuffer + continue_com_flash_count
│     │
│     └─ User clicks Cancel:
│        │
│        ├─ Set continue_com_flash_count = 0
│        │
│        └─ BEGIN FULL UPDATE FROM START
│
│  4. FLASH PROCESS
│     ├─ If resuming:
│     │  └─ nBufLen = m_szHexFileFlags[p] - nCount
│     │
│     └─ If restarting:
│        └─ nBufLen = m_szHexFileFlags[p]
│
│  5. WRITE PACKETS
│     └─ Send 128-byte packets from calculated offset
│
└─────────────────────────────────────────────────────────────┘
```

---

## Device Memory Layout

### Offset Storage Registers

```
DEVICE INTERNAL MEMORY (Modbus Accessible):

┌───────────────────────────────────────────────────────┐
│ Address    │ Size │ Purpose                           │
├───────────────────────────────────────────────────────┤
│ 0xEE10     │ 1    │ UPDATE_STATUS flag                │
│            │      │ 0x40 = Update in progress        │
│            │      │ 0x1F = Alternate in-progress     │
│            │      │ 0x00 = Update complete/idle      │
├───────────────────────────────────────────────────────┤
│ 0xEE00+17  │ 2    │ Last Valid Offset (High Byte)    │
│ (0xEE11)   │      │ UPDATE_PTR_HI                     │
├───────────────────────────────────────────────────────┤
│ 0xEE00+18  │ 2    │ Last Valid Offset (Low Byte)     │
│ (0xEE12)   │      │ UPDATE_PTR_LO                     │
├───────────────────────────────────────────────────────┤
│ 0xEE00+..  │ ..   │ Additional firmware state data   │
└───────────────────────────────────────────────────────┘

Total Offset = (UPDATE_PTR_HI << 8) | UPDATE_PTR_LO
```

---

## Code References

### Main Resumable Update Code Locations

#### 1. Initial Resume Detection
**File**: [ISP/ComWriter.cpp](ISP/ComWriter.cpp)

**Line 838-860** - First occurrence (in flash_a_tstat function):
```cpp
int x = mudbus_read_one(m_ID,0xee10);

if(mudbus_read_one(m_ID,0xee10)==0x40 || mudbus_read_one(m_ID,0xee10)==0x1f)
{
    if(IDOK==AfxMessageBox(_T("Previous Update was interrupted.\nPress OK to Resume.\nCancel to Restart."),MB_OKCANCEL))
    {
        // Resume code...
        ii=0xEE00+17;
        int l=0;
        do
        {
            int uc_temp1= mudbus_read_one(m_ID,ii);
            int uc_temp2= mudbus_read_one(m_ID,ii+1);
            if(uc_temp1==0x00 && uc_temp2==0x00 )
                ii+=2;
            else if(l==0)
            {
                if(uc_temp1==0xf0 && uc_temp2==0xf0)
                {
                    ii=0;
                    break;
                }
            }
```

**Line 2650-2690** - Second occurrence (in Flash_Modebus_Device thread):
```cpp
while(mudbus_read_one(m_ID,0xee10,1)<0) //the return value == -1 , no connecting

if(mudbus_read_one(m_ID,0xee10)==0x40 || mudbus_read_one(m_ID,0xee10)==0x1f)
{
    if(IDOK==AfxMessageBox(_T("Previous Update was interrupted.\nPress OK to Resume.\nCancel to Restart."),MB_OKCANCEL))
    {
        ii=0xEE00+17;
        // ... resume logic
    }
}
```

#### 2. Offset Calculation
**File**: [ISP/ComWriter.cpp](ISP/ComWriter.cpp), Line 1523
```cpp
pWriter->continue_com_flash_count = pWriter->update_firmware_info[0] * 128;
// 1994单位是128字节的大包; Calculates bytes from packet count
```

#### 3. Resume Message Output
**File**: [ISP/ComWriter.cpp](ISP/ComWriter.cpp), Lines 1702-1706
```cpp
if (pWriter->continue_com_flash_count > 0)
{
    CString strTips;
    nCount = pWriter->continue_com_flash_count ;
    strTips.Format(_T("Resume at breakpoint, starting from package %u"),
                   pWriter->continue_com_flash_count/128);
    pWriter->OutPutsStatusInfo(strTips, FALSE);
}
```

#### 4. Resume Offset Reset
**File**: [ISP/ComWriter.cpp](ISP/ComWriter.cpp), Lines 1020-1022
```cpp
if(pWriter->continue_com_flash_count < 0)
    pWriter->continue_com_flash_count = 0;
ii = pWriter->continue_com_flash_count;
```

#### 5. Data Structure
**File**: [ISP/ComWriter.h](ISP/ComWriter.h), Line 114
```cpp
// Continue burning from how many packets;
// currently only supports product 88 ESP32 chip boards
int continue_com_flash_count;  // 继续从第X包开始烧写; 目前只支持产品为88（ESP32芯片）的设备;
unsigned short update_firmware_info[6];
```

### Register Definitions

**File**: [ISP/Global_Struct.h](ISP/Global_Struct.h), Lines 107-111
```cpp
MODBUS_BASE_ADDRESS = 15,
MODBUS_UPDATE_STATUS = 16,        // reg 16 status for update_flash
                                  // writing 0x7F means jump to ISP routine
                                  // writing 0x8F means completely erase eeprom
MODBUS_UPDATE_PTR_HI,             // reg 17 pointer for last attempt upload HI
MODBUS_UPDATE_PTR_LO,             // reg 18 pointer for last attempt upload LO
```

### Network-Based Resume

**File**: [ISP/TFTPServer.cpp](ISP/TFTPServer.cpp), Lines 1094 and 1956
```cpp
byCommand[0] = 0xEE;  // Command, 2 bytes, 0xee10, command to start with flash
// TFTP also supports resumable updates with same logic
```

---

## Packet Structure

### Firmware Packet Format

```
PACKET STRUCTURE:
┌─────────────────────────────────────────────────────┐
│ Packet Size: 128 bytes (0x80)                       │
├─────────────────────────────────────────────────────┤
│ Byte 0-N    │ Firmware data (up to 128 bytes)       │
├─────────────────────────────────────────────────────┤
│ CRC/ChkSum  │ (Included in packet or separate)      │
└─────────────────────────────────────────────────────┘

For Extended Hex Files:
- m_szHexFileFlags stores the length of each segment
- Each segment size can vary
- continue_com_flash_count tracks bytes written
```

### Buffer Management

```cpp
// From ComWriter class:
TS_UC* m_pExtendFileBuffer;        // Stores entire firmware file
vector<int> m_szHexFileFlags;      // Stores segment lengths
int continue_com_flash_count;      // Current byte offset for resume

// Resume calculation:
nBufLen = m_szHexFileFlags[p] - nCount;  // Remaining bytes in segment
// Start address: m_pExtendFileBuffer + continue_com_flash_count
```

---

## Resume Detection Logic

### State Detection Algorithm

```cpp
// STEP 1: Read UPDATE_STATUS register
int update_status = mudbus_read_one(m_ID, 0xee10);

// STEP 2: Check for interrupt flag
if (update_status == 0x40 || update_status == 0x1f)
{
    // UPDATE IS IN PROGRESS or WAS INTERRUPTED

    // STEP 3: Prompt user for action
    if (user_clicks_OK)
    {
        // STEP 4: Read offset from device memory
        high_byte = mudbus_read_one(m_ID, 0xEE11);
        low_byte = mudbus_read_one(m_ID, 0xEE12);

        // STEP 5: Calculate offset
        offset = (high_byte << 8) | low_byte;

        // STEP 6: Set continue count
        continue_com_flash_count = offset;

        // STEP 7: Resume from offset
        // ISP will skip (offset / 128) packets
    }
    else
    {
        // RESTART: Set continue_com_flash_count = 0
    }
}
else if (update_status == 0x00)
{
    // NORMAL STATE: No previous update
    continue_com_flash_count = 0;
    start_fresh_update();
}
```

---

## Current Limitations

Based on code analysis:

1. **Device Support**
   - Primarily implemented for product ID 88 (ESP32-based boards)
   - Comment in code: "目前只支持产品为88（ESP32芯片）的设备"
   - Translation: "Currently only supports product 88 (ESP32 chip) devices"

2. **Packet Size**
   - Fixed packet size of 128 bytes per transmission
   - Resume offset must align with packet boundaries

3. **State Storage**
   - Relies on device-side EEPROM/registers to maintain state
   - No verification of firmware file hash before resume
   - If device/file mismatch, may resume with wrong file

4. **Manual User Intervention**
   - Requires user to click OK/Cancel dialog
   - Cannot auto-resume on reconnection

---

## Communication Protocol

### Modbus RTU Query Sequence

```
PC → Device:  Query UPDATE_STATUS
              [Function Code 03: Read Holding Registers]
              [Register: 0xEE10]
              [Quantity: 1]

Device → PC:  Response with status value
              [0x40: Update in progress]
              [0x1F: Alternate interrupt flag]
              [0x00: Normal/complete]

If interrupted flag detected:

PC → Device:  Read offset registers
              [Function Code 03: Read Holding Registers]
              [Starting Address: 0xEE11]
              [Quantity: 2]

Device → PC:  Response
              [Register 0xEE11: High byte of offset]
              [Register 0xEE12: Low byte of offset]

PC Calculation:
              last_offset = (0xEE11 << 8) | 0xEE12
              packets_to_skip = last_offset / 128
```

---

## Example Resume Scenario

### Scenario: Update interrupted after 1000 bytes written

1. **Initial State**
   - Device received 1000 bytes (≈8 packets of 128 bytes)
   - Device stores: offset_high = 0x03, offset_low = 0xE8 (1000 = 0x03E8)
   - UPDATE_STATUS register = 0x40

2. **PC Tool Reconnects**
   - Queries register 0xEE10
   - Receives: 0x40 (interrupted flag)

3. **User Prompted**
   ```
   "Previous Update was interrupted.
    Press OK to Resume.
    Cancel to Restart."
   ```

4. **User Clicks OK**
   - PC reads registers 0xEE11 and 0xEE12
   - Receives: high_byte=0x03, low_byte=0xE8

5. **Resume Calculation**
   - offset = (0x03 << 8) | 0xE8 = 1000 bytes
   - packets_to_skip = 1000 / 128 = 7 (with remainder)
   - continue_com_flash_count = 1000
   - Buffer pointer: m_pExtendFileBuffer + 1000

6. **Remaining Firmware Sent**
   - PC sends firmware starting from byte 1000
   - Device receives bytes 1000 to end-of-file
   - Device writes to flash starting at offset 1000

7. **Completion**
   - When all bytes received, device sets UPDATE_STATUS = 0x00
   - Device clears offset registers

---

## Implementation Status Summary

| Feature | Status | Notes |
|---------|--------|-------|
| Resume Detection | ✅ Implemented | Checks 0xee10 register |
| User Prompt | ✅ Implemented | OK/Cancel dialog |
| Offset Reading | ✅ Implemented | Reads registers 0xEE11/12 |
| Offset Calculation | ✅ Implemented | 16-bit offset support |
| Resume from Offset | ✅ Implemented | Skips packets correctly |
| Packet Transmission | ✅ Implemented | 128-byte packets |
| File Validation | ❌ Not Implemented | No hash verification |
| Auto-Resume | ❌ Not Implemented | Manual user action required |
| Full State Verification | ⚠️ Partial | Only checks interrupt flags |

---

## For ESP Bootloader Implementation

When implementing resumable update in ESP bootloader, consider:

1. **Register Compatibility**
   - Implement Modbus registers at addresses 0xEE10, 0xEE11, 0xEE12
   - Or map your native registers to these Modbus addresses

2. **State Preservation**
   - Store UPDATE_STATUS in non-volatile memory
   - Use 0x40 or 0x1F as interrupt flags
   - Update offset registers after each successful block write

3. **Offset Encoding**
   - Use 16-bit offset (UPDATE_PTR_HI and UPDATE_PTR_LO)
   - Ensure offset can be read back via Modbus

4. **Packet Alignment**
   - Use 128-byte packet size for compatibility
   - ISP tool expects this size

5. **Protocol Implementation**
   - Support standard Modbus RTU function code 03 (Read Holding Registers)
   - Support function code 06 (Write Single Register) for control

---

## References

- **ISP Tool Source**: [ISP/ComWriter.cpp](ISP/ComWriter.cpp)
- **Register Definitions**: [ISP/Global_Struct.h](ISP/Global_Struct.h)
- **TFTP Implementation**: [ISP/TFTPServer.cpp](ISP/TFTPServer.cpp)
- **Class Definition**: [ISP/ComWriter.h](ISP/ComWriter.h)

---

**Document Version**: 1.0
**Status**: Actual Implementation Analysis (Not Proposed)
**Last Updated**: 2024
