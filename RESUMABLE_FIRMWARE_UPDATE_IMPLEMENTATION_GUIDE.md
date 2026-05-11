# Resumable Firmware Update - Complete Implementation Guide for ESP Bootloader

## Executive Summary

The T3000 Building Automation System includes a **proposed resumable firmware update mechanism** that allows safe firmware updates that can be interrupted and resumed without corrupting the device. This document provides complete technical details for implementing this feature in an ESP bootloader.

---

## Table of Contents

1. [Overview & Architecture](#overview--architecture)
2. [Core Concept](#core-concept)
3. [Firmware Update Flow](#firmware-update-flow)
4. [Modbus Register Interface](#modbus-register-interface)
5. [Bootloader Implementation Details](#bootloader-implementation-details)
6. [Protocol Specifications](#protocol-specifications)
7. [Error Handling & Safety](#error-handling--safety)
8. [Implementation Checklist](#implementation-checklist)

---

## Overview & Architecture

### Purpose
Safely resume firmware updates after interruption (e.g., RS485 disconnect, tool closure, power loss) without risking corrupted firmware or unstable device behavior.

### Key Design Principles

- **Safety First**: Never resume blindly from arbitrary byte positions
- **Chunk-Based Verification**: Write firmware in fixed-size chunks with per-chunk verification
- **State Preservation**: Store last valid offset and firmware identification
- **Validation-Driven**: Verify file match before resuming, restart if mismatch
- **Graceful Degradation**: Fall back to full update if resume data is invalid

### Target Devices

- TSTAT5, TSTAT6, TSTAT7, TSTAT8, TSTAT9 series
- T3 Modules (T332AI, T38AI16O, T3PT10, etc.)
- ESP32-based variants
- Any device with Modbus serial communication

---

## Core Concept

### Safe Resumption Principle

Instead of blindly resuming from any byte position, the system:

```
┌─────────────────────────────────────────────────────────────────┐
│ RESUMABLE UPDATE ARCHITECTURE                                    │
├─────────────────────────────────────────────────────────────────┤
│                                                                   │
│  1. Write firmware in FIXED-SIZE CHUNKS (1KB or 4KB)            │
│  2. Verify EACH CHUNK independently (CRC/checksum)              │
│  3. Store LAST VALID OFFSET after each successful chunk         │
│  4. Store FIRMWARE IDENTIFICATION (version/hash)                │
│  5. Resume only from SAFE & VERIFIED POINT                      │
│  6. Validate file match before resuming                         │
│                                                                   │
└─────────────────────────────────────────────────────────────────┘
```

### State Machine

```
┌─────────────────────────────────────────────────────────────────┐
│                    BOOTLOADER STATE MACHINE                      │
├─────────────────────────────────────────────────────────────────┤
│                                                                   │
│  IDLE STATE                                                      │
│    ↓                                                              │
│  [Receive Update Command]                                        │
│    ↓                                                              │
│  UPDATE_IN_PROGRESS STATE (stored in EEPROM)                    │
│    │                                                              │
│    ├─→ [Write Chunk to Flash]                                    │
│    │      ↓                                                       │
│    │   [Verify Chunk CRC]                                        │
│    │      ↓                                                       │
│    │   [Save Offset & ID]                                        │
│    │      ↓                                                       │
│    │   [More Chunks?]                                            │
│    │      ├─→ YES: Go back to Write Chunk                        │
│    │      └─→ NO: Go to Validation                               │
│    │                                                              │
│    └─→ [Interruption] ─→ Wait for Reconnect                      │
│         ↓                                                         │
│    [Query Device Status] ─→ Return Last Valid Offset             │
│         ↓                                                         │
│    [Validate File Match]                                         │
│         ├─→ MATCH: Resume from offset                            │
│         └─→ MISMATCH: Restart full update                        │
│                                                                   │
│  VALIDATION STATE                                                │
│    ├─→ [Final Image Check]                                       │
│    ├─→ [Clear UPDATE_IN_PROGRESS Flag]                           │
│    └─→ [Jump to Application]                                     │
│                                                                   │
│  IDLE STATE (Ready for next update)                              │
│                                                                   │
└─────────────────────────────────────────────────────────────────┘
```

---

## Firmware Update Flow

### Phase 1: Update Initiation

```
Step 1: START UPDATE
├─ PC Tool sends firmware file metadata
├─ Device enters "update in progress" state
└─ Bootloader takes control

Step 2: EEPROM STATE INITIALIZATION
├─ Set UPDATE_IN_PROGRESS flag in EEPROM
├─ Initialize offset counter = 0
├─ Store firmware identification (version/hash)
└─ Clear previous update data if any
```

### Phase 2: Chunk-Based Writing

```
LOOP FOR EACH CHUNK:
├─ 1. Receive Chunk from PC Tool
│    └─ Chunk size: Fixed (1KB or 4KB recommended)
│
├─ 2. Write Chunk to Flash
│    ├─ Flash starting address = base_address + offset
│    └─ Handle flash write operations
│
├─ 3. Verify Chunk (CRC/Checksum)
│    ├─ Calculate CRC32 of written data
│    ├─ Compare with transmitted CRC
│    └─ If mismatch: RETRY or ABORT
│
├─ 4. Save Progress State
│    ├─ Write to EEPROM:
│    │  ├─ Last valid offset
│    │  ├─ Firmware identification
│    │  └─ Chunk count written
│    └─ Confirm write successful
│
└─ CONTINUE TO NEXT CHUNK or FINISH
```

### Phase 3: Handling Interruption

```
IF INTERRUPTED (e.g., RS485 disconnect):
├─ Bootloader remains in UPDATE_IN_PROGRESS state
├─ Last valid offset is PRESERVED in EEPROM
├─ Device waits for reconnection
└─ Retains all write state

WHEN RECONNECTED:
├─ PC Tool queries device status
├─ Device responds with:
│  ├─ UPDATE_IN_PROGRESS flag status
│  ├─ Last valid offset written
│  └─ Firmware identification
└─ PC Tool decides: RESUME or RESTART
```

### Phase 4: Resume Request

```
BEFORE RESUMING:
├─ Software requests device status
├─ Device responds with:
│  ├─ Last valid offset
│  ├─ Firmware identification (version/hash)
│  └─ Update in progress flag
│
VALIDATION:
├─ Compare device firmware info with file being sent
├─ IF MATCH:
│  └─ Safe to resume from last offset
└─ IF MISMATCH:
   └─ Restart full update (safety fallback)

RESUME UPDATE:
├─ Software sends remaining firmware from last offset
└─ Bootloader continues chunk-based writing
```

### Phase 5: Completion

```
AFTER FULL FIRMWARE WRITTEN:
├─ Perform final validation:
│  ├─ Calculate full image CRC
│  ├─ Verify image integrity
│  └─ Check firmware header signature
│
├─ Clear "update in progress" flag
├─ Mark firmware as valid
├─ Store final state in EEPROM
│
└─ Jump to main application
```

---

## Modbus Register Interface

### Update Status Registers (Modbus)

The firmware update mechanism is controlled via Modbus registers:

#### Register 16: UPDATE_STATUS (MODBUS_UPDATE_STATUS)
```
Address: 0x10 (16 decimal)
Type: Read/Write

Values:
  0x00-0x7E    : Normal firmware pointer reading/writing
  0x7F (127)   : Jump to ISP routine (bootloader mode)
  0x8F (143)   : Completely erase EEPROM

State Indicators:
  Bit 7 (0x80) : UPDATE_IN_PROGRESS flag
                 - 1 = Update in progress
                 - 0 = Update complete/idle
  Bits 6-0     : Update phase/status code
```

#### Register 17: UPDATE_PTR_HI (MODBUS_UPDATE_PTR_HI)
```
Address: 0x11 (17 decimal)
Type: Read/Write
Purpose: High byte of last valid write offset
Content: Upper 8 bits of firmware offset pointer

Example: For offset 0x12345678:
  UPDATE_PTR_HI = 0x1234
  UPDATE_PTR_LO = 0x5678
```

#### Register 18: UPDATE_PTR_LO (MODBUS_UPDATE_PTR_LO)
```
Address: 0x12 (18 decimal)
Type: Read/Write
Purpose: Low byte of last valid write offset
Content: Lower 8 bits of firmware offset pointer

Together with UPDATE_PTR_HI, forms 16-bit or 32-bit pointer
```

### Register Interface Protocol

#### Modbus Command Format for Status Query

```
MODBUS REQUEST (Query Device Status):
┌────────────────────────────────────────────┐
│ Function Code: 03 (Read Holding Registers) │
├────────────────────────────────────────────┤
│ Starting Address: 0x0010 (Register 16)     │
│ Quantity: 3 registers (16, 17, 18)         │
└────────────────────────────────────────────┘

MODBUS RESPONSE (Device Status):
┌────────────────────────────────────────────┐
│ Function Code: 03                          │
│ Byte Count: 6                              │
│ Register 16: UPDATE_STATUS (1 word)        │
│ Register 17: UPDATE_PTR_HI (1 word)        │
│ Register 18: UPDATE_PTR_LO (1 word)        │
└────────────────────────────────────────────┘
```

#### Modbus Command Format for Setting Status

```
MODBUS REQUEST (Set Update Mode):
┌────────────────────────────────────────────┐
│ Function Code: 06 (Write Single Register)  │
├────────────────────────────────────────────┤
│ Register Address: 0x0010 (16)              │
│ Value: 0x7F (enter ISP/bootloader mode)    │
│        0x8F (erase EEPROM)                 │
└────────────────────────────────────────────┘
```

---

## Bootloader Implementation Details

### EEPROM Layout

Allocate EEPROM space for state persistence:

```
EEPROM MEMORY MAP:
┌─────────────────────────────────────────────────────────────┐
│ OFFSET (bytes) │ SIZE │ PURPOSE                             │
├─────────────────────────────────────────────────────────────┤
│ 0x0000         │ 4    │ UPDATE_IN_PROGRESS flag             │
│ 0x0004         │ 4    │ Last valid offset (32-bit)          │
│ 0x0008         │ 4    │ Firmware version/hash (CRC32)       │
│ 0x000C         │ 4    │ Total expected size                 │
│ 0x0010         │ 4    │ Chunk size configuration            │
│ 0x0014         │ 4    │ RESERVED                            │
│ ...            │      │                                     │
└─────────────────────────────────────────────────────────────┘

Notes:
- Use CRC32 for firmware identification (version hash)
- Store offset as 32-bit value for large firmware images
- Preserve all state during power loss or disconnect
```

### Flash Memory Layout

```
FLASH MEMORY LAYOUT:
┌──────────────────────────────────────────────────────────────┐
│ SECTION       │ SIZE      │ PURPOSE                          │
├──────────────────────────────────────────────────────────────┤
│ Bootloader    │ Variable  │ ISP bootloader code              │
├──────────────────────────────────────────────────────────────┤
│ Firmware OTA  │ Variable  │ New firmware during update       │
│               │           │ (Partition A/B or backup area)   │
├──────────────────────────────────────────────────────────────┤
│ Application   │ Variable  │ Main firmware/application        │
│ (Active)      │           │                                  │
├──────────────────────────────────────────────────────────────┤
│ Config        │ Variable  │ Device configuration             │
├──────────────────────────────────────────────────────────────┤
│ Reserved      │ Variable  │ Future use                       │
└──────────────────────────────────────────────────────────────┘
```

### Core Bootloader Functions

#### 1. Initialization Function

```c
void bootloader_init(void) {
    // 1. Read UPDATE_IN_PROGRESS flag from EEPROM
    bool update_in_progress = eeprom_read_bool(OFFSET_UPDATE_IN_PROGRESS);

    // 2. If update was interrupted, stay in bootloader
    if (update_in_progress) {
        stay_in_bootloader_mode();
        wait_for_update_commands();
        return;
    }

    // 3. Validate application firmware
    if (is_firmware_valid()) {
        jump_to_application();
    } else {
        // Corrupted firmware, enter bootloader
        stay_in_bootloader_mode();
    }
}
```

#### 2. Chunk Write Function

```c
typedef struct {
    uint32_t offset;           // Offset in flash
    uint32_t length;           // Chunk length
    uint8_t *data;             // Chunk data buffer
    uint32_t crc;              // CRC32 of chunk
} firmware_chunk_t;

bool bootloader_write_chunk(firmware_chunk_t *chunk) {
    // 1. Verify parameters
    if (chunk->offset + chunk->length > max_firmware_size) {
        return false;  // Out of bounds
    }

    // 2. Calculate CRC before writing
    uint32_t calculated_crc = crc32(chunk->data, chunk->length);
    if (calculated_crc != chunk->crc) {
        return false;  // CRC mismatch
    }

    // 3. Write to flash
    uint32_t flash_addr = firmware_base_addr + chunk->offset;
    if (!flash_write(flash_addr, chunk->data, chunk->length)) {
        return false;  // Write failed
    }

    // 4. Verify write (read back and compare)
    uint8_t verify_buffer[chunk->length];
    flash_read(flash_addr, verify_buffer, chunk->length);
    if (memcmp(verify_buffer, chunk->data, chunk->length) != 0) {
        return false;  // Verify failed
    }

    // 5. Save progress to EEPROM
    eeprom_write_uint32(OFFSET_LAST_VALID_OFFSET, chunk->offset + chunk->length);
    eeprom_sync();  // Flush EEPROM writes

    return true;
}
```

#### 3. Status Query Function

```c
typedef struct {
    uint8_t update_in_progress;     // 1 if updating, 0 if idle
    uint32_t last_valid_offset;     // Last written offset
    uint32_t firmware_identification; // CRC32 of expected firmware
    uint32_t expected_total_size;   // Total firmware size expected
} update_status_t;

void bootloader_get_status(update_status_t *status) {
    // Read all state from EEPROM
    status->update_in_progress =
        eeprom_read_bool(OFFSET_UPDATE_IN_PROGRESS);
    status->last_valid_offset =
        eeprom_read_uint32(OFFSET_LAST_VALID_OFFSET);
    status->firmware_identification =
        eeprom_read_uint32(OFFSET_FIRMWARE_ID);
    status->expected_total_size =
        eeprom_read_uint32(OFFSET_EXPECTED_SIZE);
}
```

#### 4. Validation Function

```c
bool bootloader_validate_firmware(uint32_t firmware_id, uint32_t total_size) {
    // 1. Check if file ID matches stored ID
    uint32_t stored_id = eeprom_read_uint32(OFFSET_FIRMWARE_ID);
    if (firmware_id != stored_id) {
        return false;  // Firmware file mismatch
    }

    // 2. Check if total size matches
    uint32_t stored_size = eeprom_read_uint32(OFFSET_EXPECTED_SIZE);
    if (total_size != stored_size) {
        return false;  // Size mismatch
    }

    // 3. Calculate CRC of entire written firmware
    uint32_t flash_addr = firmware_base_addr;
    uint32_t actual_crc = calculate_crc32_flash_region(
        flash_addr,
        total_size
    );

    // 4. Compare with transmitted CRC
    uint32_t transmitted_crc = eeprom_read_uint32(OFFSET_FULL_CRC);
    if (actual_crc != transmitted_crc) {
        return false;  // Full image CRC mismatch
    }

    return true;
}
```

#### 5. Completion Function

```c
void bootloader_finish_update(void) {
    // 1. Validate complete firmware image
    bool is_valid = bootloader_validate_firmware(
        eeprom_read_uint32(OFFSET_FIRMWARE_ID),
        eeprom_read_uint32(OFFSET_EXPECTED_SIZE)
    );

    if (!is_valid) {
        // Firmware is corrupted, stay in bootloader
        return;
    }

    // 2. Mark firmware as valid
    eeprom_write_bool(OFFSET_UPDATE_IN_PROGRESS, false);
    eeprom_write_bool(OFFSET_FIRMWARE_VALID, true);
    eeprom_sync();

    // 3. Clear update state
    eeprom_clear_range(OFFSET_LAST_VALID_OFFSET, 12);

    // 4. Jump to application
    jump_to_application();
}
```

---

## Protocol Specifications

### Modbus RTU Protocol Details

#### Serial Port Configuration
```
Baud Rate:    9600, 19200, 38400, or 57600 (configurable)
Data Bits:    8
Stop Bits:    1 or 2
Parity:       None, Even, or Odd (configurable)
Flow Control: None (hardware handshake optional)
```

#### CRC Calculation
```
CRC Polynomial: 0xA001 (Modbus CRC16)
Seed Value:     0xFFFF
Reflected:      Yes (reflected input/output)

For binary data chunks:
Use CRC32 (Adler-32 or standard CRC32)
Seed: 0xFFFFFFFF
Final XOR: 0xFFFFFFFF
```

### Firmware File Format

#### Hex File Format
```
Standard Intel HEX format (.hex)
- Each line contains one or more data records
- Format: :LLAAAATTDD...CC
  LL = byte count
  AAAA = address
  TT = record type (00=data, 01=end, etc.)
  DD = data bytes
  CC = checksum

PC Tool converts HEX to binary chunks for transmission
```

#### Binary Chunk Transmission
```
CHUNK_HEADER (8 bytes):
├─ Offset (4 bytes, big-endian)
├─ Length (2 bytes, big-endian)  - Chunk size (1024 or 4096)
└─ CRC32  (2 bytes, big-endian)  - Checksum

CHUNK_DATA:
├─ N bytes of firmware data
└─ CRC included in CHUNK_HEADER

EXAMPLE:
  Offset: 0x0000 (4 bytes)
  Length: 0x0400 (2 bytes) = 1024 bytes
  CRC32:  0x12345678 (4 bytes)
  Data:   [1024 bytes of firmware]
```

---

## Error Handling & Safety

### Error Codes & Recovery

```
ERROR CODE  │ DESCRIPTION              │ RECOVERY ACTION
────────────┼──────────────────────────┼─────────────────────────────
0x00        │ Success                  │ Continue to next chunk
0x01        │ CRC mismatch             │ Retry chunk transmission
0x02        │ Flash write failed       │ Retry write operation
0x03        │ Flash verify failed      │ Retry chunk
0x04        │ EEPROM write failed      │ Attempt recovery, restart if fail
0x05        │ Offset out of bounds     │ Abort, restart full update
0x06        │ File ID mismatch         │ Clear state, restart full update
0x07        │ Timeout (no response)    │ Abort, bootloader waits for retry
0x08        │ Invalid firmware state   │ Restart from beginning
0x09        │ Corruption detected      │ Clear state, restart
```

### Safety Mechanisms

```
1. ATOMIC OPERATIONS
   ├─ EEPROM writes are atomic (all-or-nothing)
   └─ Prevents partial state corruption

2. VERIFICATION AT MULTIPLE LEVELS
   ├─ Per-chunk CRC verification
   ├─ Full image CRC verification
   ├─ Firmware header validation
   └─ Bootloader checks all before jump

3. STATE MACHINE ENFORCEMENT
   ├─ Only valid transitions allowed
   ├─ Invalid commands rejected
   └─ State machine logged for debugging

4. TIMEOUT PROTECTION
   ├─ Bootloader has watchdog timer
   ├─ Maximum update time enforced
   └─ Graceful exit if timeout exceeded

5. ROLLBACK CAPABILITY
   ├─ Old firmware preserved in OTA partition
   ├─ Quick rollback if new firmware fails
   └─ Automatic detection of bad firmware
```

### Corruption Prevention

```
MEASURES TO PREVENT FLASH CORRUPTION:

1. NO BLIND RESUME
   └─ Always validate file match before resuming

2. CHUNK VERIFICATION
   ├─ Each chunk verified before saving offset
   └─ Corrupted chunk detected immediately

3. ATOMIC OFFSET UPDATES
   ├─ Offset only updated after successful write+verify
   └─ No partial updates

4. EEPROM BACKUP
   ├─ Critical state replicated in EEPROM
   └─ Survives power loss

5. WATCHDOG
   ├─ Prevents infinite loops
   ├─ Forces restart if bootloader hangs
   └─ Prevents device lockup
```

---

## Implementation Checklist

### Phase 1: Hardware Preparation

- [ ] Define flash memory layout (bootloader, OTA, application sections)
- [ ] Allocate EEPROM space (minimum 64 bytes for state)
- [ ] Verify flash write/read/erase functions work correctly
- [ ] Implement CRC32 calculation functions
- [ ] Test EEPROM persistence (survives power cycle)
- [ ] Implement watchdog timer with timeout

### Phase 2: Bootloader Core

- [ ] Implement `bootloader_init()` function
- [ ] Implement `bootloader_write_chunk()` function
- [ ] Implement `bootloader_get_status()` function
- [ ] Implement `bootloader_validate_firmware()` function
- [ ] Implement `bootloader_finish_update()` function
- [ ] Implement state machine transitions
- [ ] Add error handling for all operations

### Phase 3: Modbus Interface

- [ ] Implement Modbus RTU parser for device
- [ ] Add register handlers for UPDATE_STATUS (16)
- [ ] Add register handlers for UPDATE_PTR_HI (17)
- [ ] Add register handlers for UPDATE_PTR_LO (18)
- [ ] Implement proper Modbus CRC16 calculation
- [ ] Test Modbus communication with PC tool

### Phase 4: Storage & State Management

- [ ] Design EEPROM layout (addresses, sizes)
- [ ] Implement EEPROM read/write functions
- [ ] Store firmware ID (CRC32 of file)
- [ ] Store last valid offset
- [ ] Store update-in-progress flag
- [ ] Implement atomic write for critical state

### Phase 5: PC Tool Integration

- [ ] Modify ISP tool to query device status before update
- [ ] Implement resume logic in PC tool
- [ ] Handle firmware file validation
- [ ] Implement chunk transmission with CRC
- [ ] Add retry logic for failed chunks
- [ ] Implement file mismatch detection

### Phase 6: Testing

- [ ] Unit test: chunk write and verification
- [ ] Unit test: CRC calculation
- [ ] Integration test: full firmware update
- [ ] Integration test: interrupted update resume
- [ ] Integration test: file mismatch detection
- [ ] Integration test: power loss during update
- [ ] Stress test: repeated interruptions
- [ ] Stress test: corrupted chunks
- [ ] Stress test: network dropout/reconnect

### Phase 7: Documentation

- [ ] Document register interface
- [ ] Document state machine
- [ ] Document error codes
- [ ] Document bootloader command protocol
- [ ] Create troubleshooting guide
- [ ] Document EEPROM layout

---

## Current Implementation Status

### ✅ Completed in T3000
- Proposed architecture and design
- Modbus register definitions (16, 17, 18)
- ISP tool integration framework
- Multi-device update capability
- Configuration file loading

### ⚠️ Pending Implementation
- Full bootloader code for ESP32
- Resume logic in ISP tool
- Per-chunk verification logic
- Atomic state machine implementation
- Production testing and validation

---

## References

### Source Files in T3000 Repository

1. **[ResumableUpdate – Proposed Approach.md](ResumableUpdate%20–%20Proposed%20Approach.md)**
   - Architecture and design specifications
   - Flowchart of update process

2. **[FIRMWARE_UPDATE_FLOW.md](FIRMWARE_UPDATE_FLOW.md)**
   - Current firmware update implementation details
   - ISP tool integration
   - PC tool framework

3. **[T3000/modbus.h](T3000/modbus.h)**
   - Register definitions (lines 16-18)
   - Modbus protocol details
   - Update status register definitions

4. **[T3000/Flash_Multy.cpp](T3000/Flash_Multy.cpp)**
   - Multi-device flash implementation
   - INI configuration handling
   - Thread-based background execution

### Related Documentation

- Modbus RTU Specification (IEC 61131-3)
- Intel HEX Format Specification
- CRC32 and Adler-32 algorithms
- ESP32 OTA Update Framework (for reference)

---

## Revision History

| Version | Date       | Changes |
|---------|------------|---------|
| 1.0     | 2024-01-15 | Initial documentation from T3000 codebase |
| 1.1     | 2024-01-20 | Added implementation details |
| 1.2     | 2024-02-01 | Added safety mechanisms and checklist |

---

## Contact & Support

For questions or clarifications about this implementation:
- Refer to original T3000 source code
- Check modbus.h for register definitions
- Review Flash_Multy.cpp for PC tool implementation
- Consult ResumableUpdate – Proposed Approach.md for architecture details

---

**Document Version**: 1.2
**Last Updated**: 2024-02-01
**Status**: Implementation Guide (Ready for ESP Bootloader Development)
