# T3000 Firmware Update Flow - ISP Process

## Overview
This document describes the firmware update process with resumable capability, error recovery, and state management.

---

## System Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                    User Interface                               │
│            (Ctrl+F2 / Tools → Load Firmware)                    │
└─────────────────────────────────────────────────────────────────┘
                           ↓
┌─────────────────────────────────────────────────────────────────┐
│              MainFrm::OnBatchFlashHex()                          │
│          [MainFrm.cpp:3108 - Entry Point]                       │
└─────────────────────────────────────────────────────────────────┘
                           ↓
┌─────────────────────────────────────────────────────────────────┐
│            CFlash_Multy Dialog                                   │
│   • Device selection                                             │
│   • Firmware file (.hex) selection                               │
│   • Configuration file selection                                 │
└─────────────────────────────────────────────────────────────────┘
                           ↓
┌─────────────────────────────────────────────────────────────────┐
│            Update State Manager                                  │
│   • Initialize/Load update state                                │
│   • Checkpoint creation                                         │
│   • Progress tracking                                           │
└─────────────────────────────────────────────────────────────────┘
                           ↓
              ┌────────────────────┐
              │  Main Flash Thread │
              │ multy_isp_thread() │
              └────────────────────┘
                           ↓
         ┌─────────────────────────────────┐
         │   For Each Device (Sequential)   │
         └─────────────────────────────────┘
```

---

## Phase 1: Firmware Flash (ISP.exe)

### Entry Point
- **Handler**: `CFlash_Multy::multy_isp_thread()` [Flash_Multy.cpp:969]
- **Mode**: Sequential processing (one device at a time)
- **Thread**: Background worker thread with UI updates

### Flow Diagram

```
START
  ↓
[Load/Initialize Update State]
  ├─ Read state file (.isp_state)
  ├─ Determine resume point
  └─ Load checkpoint data
  ↓
[For Each Device]
  ├─ Check if device update already completed
  ├─ If YES → Skip to next device
  ├─ If NO → Continue
  ↓
[Pre-Flash Checks]
  ├─ Verify device connectivity
  ├─ Validate device type (TSTAT6, 5E, 5A, T3, etc.)
  ├─ Check bootloader version
  ├─ Verify firmware compatibility
  ├─ Check disk space
  └─ If ANY check fails → Log error → RETRY or SKIP
  ↓
[Save Progress Checkpoint]
  ├─ Update state: "Device X - Pre-Flash Check Passed"
  ├─ Save checkpoint to INI/JSON
  └─ Record timestamp
  ↓
[Bootloader Update (if required)]
  ├─ Determine if new bootloader needed
  │  ├─ Check firmware_must_use_new_bootloader flag
  │  └─ Compare bootloader versions
  ├─ If bootloader update needed:
  │  ├─ Set com_port_flash_status = 1 (flash boot mode)
  │  ├─ Save checkpoint: "Bootloader Flash Start"
  │  ├─ Invoke ISP.exe for bootloader
  │  ├─ Wait for completion
  │  ├─ Check result in INI
  │  ├─ If FAIL → Retry (max 3 times) → If still fail → Log error → Continue
  │  ├─ If SUCCESS → Save checkpoint: "Bootloader Flash Success"
  │  ├─ Device reboot (Sleep 4 seconds)
  │  ├─ Verify device reconnection
  │  └─ If NOT reconnected → Retry or Mark as failed
  └─ If bootloader update not needed → Skip this section
  ↓
[Main Firmware Flash]
  ├─ Set com_port_flash_status = 0 (normal mode)
  ├─ Save checkpoint: "Firmware Flash Start"
  ├─ Prepare ISP parameters:
  │  ├─ Device ID (Modbus/BACnet)
  │  ├─ Firmware file path
  │  ├─ COM port
  │  ├─ Baud rate
  │  └─ Firmware checksum/MD5
  ├─ Call CComWriter::BeginWriteByCom()
  │  ├─ Open COM port / Network socket
  │  ├─ Detect device MSTP if needed
  │  ├─ Enter flash mode
  │  ├─ Transfer firmware data:
  │  │  ├─ Read firmware file in chunks
  │  │  ├─ For each chunk:
  │  │  │  ├─ Send chunk to device
  │  │  │  ├─ Wait for ACK
  │  │  │  ├─ Verify checksum
  │  │  │  ├─ If CRC error → Retry chunk (max 3 times)
  │  │  │  ├─ Update progress bar
  │  │  │  └─ Save chunk checkpoint every N chunks
  │  │  ├─ Handle timeouts:
  │  │  │  ├─ com_error_delay_time = delay between retries
  │  │  │  ├─ com_error_delay_count = max retry attempts
  │  │  │  └─ Log error if exceeds max
  │  │  └─ Final verification:
  │  │     ├─ Compare MD5/Checksum
  │  │     └─ If mismatch → Retry from last checkpoint
  │  ├─ Verify firmware integrity:
  │  │  ├─ Check CRC
  │  │  ├─ Validate firmware header
  │  │  └─ If invalid → Retry flash
  │  └─ Exit flash mode
  ├─ Invoke ISP.exe via WinExecAndWait()
  │  ├─ Pass parameters via INI file
  │  ├─ Wait for ISP.exe completion
  │  ├─ Read result from INI file
  │  └─ Parse return code
  ├─ Check flash result:
  │  ├─ SUCCESS → Save checkpoint: "Firmware Flash Success"
  │  ├─ CRC ERROR → Retry (max 3 times)
  │  ├─ TIMEOUT → Retry with increased delay
  │  ├─ DEVICE NOT FOUND → Check connectivity → Retry
  │  └─ OTHER ERROR → Log detailed error → Continue to next device
  ├─ Device reboot:
  │  ├─ Set UI to blue (running)
  │  ├─ Sleep 4 seconds (allow device reboot)
  │  ├─ Attempt device reconnection (retry max 5 times)
  │  └─ If reconnected → Continue
  │     Else → Mark device as "Reboot Failed" → Continue
  └─ Save checkpoint: "Firmware Flash Complete"
  ↓
[Post-Flash Verification]
  ├─ Read device firmware version
  ├─ Compare with uploaded version
  ├─ If mismatch:
  │  ├─ Log version mismatch error
  │  ├─ Set device status to failed
  │  └─ Continue to next
  ├─ If match:
  │  ├─ Update device status to success
  │  └─ Post UI message (Green color)
  └─ Save checkpoint: "Phase 1 Complete"
  ↓
[Next Device or Proceed to Phase 2]
  ├─ If more devices → Loop back to "For Each Device"
  └─ If all devices done → Proceed to Phase 2
```

---

## Phase 2: Configuration Load

### Flow Diagram

```
START (Phase 2)
  ↓
[Load Configuration State]
  ├─ Read saved state from checkpoint
  └─ Skip devices with Phase 1 failures
  ↓
[For Each Device with Successful Firmware]
  ├─ Check if config already loaded
  ├─ If YES → Skip to next device
  ├─ If NO → Continue
  ↓
[Device Connection]
  ├─ Open COM port or network socket
  ├─ If port busy:
  │  ├─ Retry (max 3 times)
  │  ├─ If still busy → Log error → Skip device
  │  └─ Continue
  └─ Connection established
  ↓
[Device Type Detection]
  ├─ Send discovery command
  ├─ Parse device response:
  │  ├─ Check device type (TSTAT6, 5E, 5A, T3 Modules, etc.)
  │  ├─ Verify firmware version
  │  └─ Get device capabilities
  ├─ If not recognized:
  │  ├─ Log unknown device type
  │  ├─ Attempt generic load
  │  └─ If fails → Skip device
  └─ Device type confirmed
  ↓
[Select Configuration Loader]
  ├─ TSTAT6/5E → LoadFile2Tstat67()
  ├─ TSTAT5A → LoadFile2Tstat67() (variant)
  ├─ T3 Modules → LoadFile2Tstat_T3()
  ├─ Network Devices → LoadFile2NetworkDevice()
  ├─ BACnet Devices → LoadFile2BacnetDevice()
  └─ Other → LoadFile2GenericDevice()
  ↓
[Load Configuration File]
  ├─ Parse configuration file (.json/.xml/.bin)
  ├─ Validate configuration against device model
  ├─ Build configuration packets
  └─ If parsing error:
     ├─ Log detailed error
     └─ Skip device
  ↓
[Transfer Configuration]
  ├─ Save checkpoint: "Config Transfer Start"
  ├─ For each config parameter:
  │  ├─ Create Modbus/BACnet write command
  │  ├─ Send to device:
  │  │  ├─ Wait for ACK (max timeout: 5 seconds)
  │  │  ├─ If ACK received → Update progress → Continue
  │  │  ├─ If timeout:
  │  │  │  ├─ Retry (max 3 times)
  │  │  │  ├─ If still fails → Log error → Mark parameter failed
  │  │  │  └─ Continue to next parameter
  │  │  └─ If CRC error → Retry from last checkpoint
  │  └─ Update progress bar every N parameters
  ├─ Save progress checkpoint every N parameters:
  │  ├─ Save last successful parameter index
  │  ├─ Save timestamp
  │  └─ Allow resumption from this point
  └─ All parameters transferred
  ↓
[Configuration Verification]
  ├─ Read back configuration from device
  ├─ Compare with sent configuration:
  │  ├─ For each critical parameter:
  │  │  ├─ If matches → OK
  │  │  ├─ If mismatch:
  │  │  │  ├─ Retry write (max 2 times)
  │  │  │  ├─ If still mismatch → Log error → Mark parameter failed
  │  │  │  └─ Continue to next parameter
  │  │  └─ Track verification status
  │  └─ Calculate verification score (% parameters correct)
  ├─ If >= 95% parameters correct → Mark as SUCCESS
  ├─ If < 95% parameters correct → Mark as PARTIAL SUCCESS
  └─ Update UI status
  ↓
[Device Reboot (if needed)]
  ├─ Send reboot command to device
  ├─ Wait for device to reboot (Sleep 2-4 seconds)
  ├─ Attempt reconnection (retry max 5 times)
  ├─ If reconnected:
  │  ├─ Verify device is functional
  │  └─ Mark device as ONLINE
  └─ If not reconnected:
     ├─ Mark device as REBOOT_FAILED
     └─ Continue
  ↓
[Post-Configuration Status]
  ├─ SUCCESS (Green):
  │  ├─ All firmware + config transferred & verified
  │  └─ Device online and functional
  ├─ PARTIAL SUCCESS (Light Green):
  │  ├─ Firmware successful, config partial success
  │  └─ Device functional but some settings may be missing
  ├─ CONFIG FAILED (Light Red):
  │  ├─ Firmware successful, config failed
  │  └─ Device needs manual configuration
  └─ FAILED (Red):
     ├─ One or more critical phases failed
     └─ Device requires manual intervention
  ↓
[Save Final Checkpoint]
  ├─ Record Phase 2 completion status
  ├─ Save completion timestamp
  ├─ Log device final state
  └─ Move to next device
  ↓
[Next Device]
  ├─ If more devices → Loop back to "For Each Device"
  └─ If all devices done → Cleanup and finish
```

---

## Resumable Update Mechanism

### State Management

#### Update State File Format (`.isp_state.json`)

```json
{
  "update_session_id": "UUID-timestamp",
  "start_time": "2024-05-15T10:30:00Z",
  "last_update_time": "2024-05-15T10:35:42Z",
  "total_devices": 5,
  "firmware_file": "/path/to/firmware.hex",
  "firmware_md5": "abc123def456...",
  "config_file": "/path/to/config.json",

  "phase_status": {
    "phase1_firmware": "IN_PROGRESS",
    "phase2_config": "PENDING"
  },

  "devices": [
    {
      "device_id": "DEVICE_001",
      "modbus_id": 10,
      "device_type": "TSTAT6",
      "firmware_version_old": "1.2.3",
      "firmware_version_target": "1.3.0",

      "phase1_status": "IN_PROGRESS",
      "phase1_current_step": "FIRMWARE_FLASH",
      "bootloader_status": "SUCCESS",
      "bootloader_version": "2.1.0",

      "firmware_transfer": {
        "total_chunks": 256,
        "chunks_transferred": 128,
        "last_chunk_index": 127,
        "chunk_size": 1024,
        "last_checkpoint_time": "2024-05-15T10:35:00Z",
        "crc_ok": true
      },

      "verification": {
        "status": "IN_PROGRESS",
        "md5_match": false,
        "attempts": 1
      },

      "phase2_status": "PENDING",
      "config_transfer": {
        "total_parameters": 150,
        "parameters_transferred": 0,
        "last_parameter_index": -1,
        "verification_score": 0
      },

      "error_log": [
        {
          "timestamp": "2024-05-15T10:34:15Z",
          "step": "FIRMWARE_TRANSFER_CHUNK_50",
          "error_code": "CRC_ERROR",
          "error_message": "CRC mismatch for chunk 50",
          "retry_count": 2,
          "recovered": true
        }
      ],

      "retry_count": {
        "firmware_flash": 1,
        "config_transfer": 0,
        "total": 1
      }
    }
  ],

  "summary": {
    "completed": 0,
    "in_progress": 1,
    "failed": 0,
    "pending": 4
  }
}
```

### Checkpoint Types

#### 1. **Pre-Operation Checkpoint**
- Record: Device ID, operation type, parameters
- Timing: Before major operation starts
- Purpose: Allow resumption from this exact point

#### 2. **Progress Checkpoint**
- Record: Current progress (e.g., chunk number, parameter count)
- Timing: Periodically (every N chunks/parameters)
- Purpose: Reduce data re-transfer

#### 3. **Error Checkpoint**
- Record: Error type, location, recovery action
- Timing: When error occurs
- Purpose: Track issues for debugging

#### 4. **Milestone Checkpoint**
- Record: Major phase completion (bootloader, firmware, config)
- Timing: After each major phase
- Purpose: Skip completed phases on resume

### Recovery Strategies

```
RESUMABLE UPDATE FLOW:

User initiates update
  ↓
Load existing state file
  ├─ If found:
  │  ├─ Parse checkpoint data
  │  ├─ Verify checkpoint integrity
  │  ├─ Ask user: "Resume or restart?"
  │  ├─ If RESUME:
  │  │  ├─ Skip completed devices
  │  │  ├─ Resume from last checkpoint
  │  │  └─ Reload partial data
  │  └─ If RESTART:
  │     ├─ Backup old state file
  │     └─ Create new state file
  │
  └─ If NOT found:
     └─ Create new state file

During execution:
  ├─ Save checkpoint every N operations
  ├─ On error:
  │  ├─ If recoverable error:
  │  │  ├─ Save error checkpoint
  │  │  ├─ Retry (with exponential backoff)
  │  │  ├─ If retry succeeds → Update checkpoint → Continue
  │  │  └─ If retry fails → Mark device failed → Next device
  │  │
  │  └─ If unrecoverable error:
  │     ├─ Log detailed error
  │     ├─ Save error checkpoint
  │     └─ Skip device or ask user
  │
  └─ On user interrupt:
     ├─ Save current checkpoint
     ├─ Close connections gracefully
     └─ Exit (resumable state preserved)

On completion:
  ├─ Archive state file
  ├─ Generate completion report
  └─ Cleanup temporary files
```

---

## Error Handling & Recovery

### Error Categories

#### 1. **COM Port Errors**
```
ERROR: "The COM port is occupied!"
RECOVERY:
  ├─ Retry 3 times with 500ms delay
  ├─ Check for other applications using port
  ├─ If still occupied → Restart COM manager → Retry
  └─ If still failed → Skip device → Continue

ERROR: "No MSTP data on RS485"
RECOVERY:
  ├─ Check cable connections
  ├─ Verify device power
  ├─ Retry 2 times with 1 second delay
  └─ If failed → Mark device offline → Continue
```

#### 2. **Firmware Transfer Errors**
```
ERROR: "CRC Error"
RECOVERY:
  ├─ Retry last chunk (max 3 times)
  ├─ If succeeds → Continue from next chunk
  ├─ If fails → Restore from last checkpoint → Retry entire phase
  └─ If fails again → Mark device failed → Continue

ERROR: "Device Timeout"
RECOVERY:
  ├─ Increase com_error_delay_time
  ├─ Retry with backoff: 500ms → 1s → 2s
  ├─ Verify device is responding to commands
  └─ If no response → Check device power/connection

ERROR: "Firmware Mismatch"
RECOVERY:
  ├─ Re-read firmware version from device
  ├─ Compare with target version
  ├─ If mismatch persists → Retry flash (max 2 times)
  └─ If still mismatch → Manual intervention required
```

#### 3. **Configuration Errors**
```
ERROR: "Parameter Write Failed"
RECOVERY:
  ├─ Retry write (max 3 times)
  ├─ Skip failed parameter → Continue
  ├─ Mark device as "Partial Success"
  └─ Log which parameters failed

ERROR: "Verification Failed"
RECOVERY:
  ├─ Read parameter value again
  ├─ Compare with target
  ├─ If mismatch → Retry write
  └─ If persistent mismatch → Log and continue
```

### Retry Strategy

```
EXPONENTIAL BACKOFF:

Retry 1: Immediate retry
Retry 2: Wait 500ms → Retry
Retry 3: Wait 1000ms → Retry
Retry 4: Wait 2000ms → Retry
Max Retries: 3-5 (depending on error type)

For timeout errors:
  ├─ Increase timeout by 50%
  ├─ Increase retry delay
  └─ Add diagnostic logging
```

---

## UI Status Indicators

### Color Codes
- **Blue**: Update in progress
- **Green**: Firmware flash successful
- **Bright Green**: Both firmware + config successful
- **Light Green**: Config partial success (firmware OK)
- **Yellow**: Paused/Checkpointed
- **Light Red**: Firmware OK, config failed
- **Red**: Update failed (firmware or critical config)
- **Gray**: Skipped/Not updated

### Progress Bar
- Shows overall progress: `(devices_completed / total_devices) × 100%`
- Sub-progress: `(current_operation_progress) × 100%` within device

### Status Messages
```
[DEVICE_001] Phase 1: Firmware Flash → 50% Complete (128/256 chunks)
[DEVICE_001] Phase 1: Firmware Verification → In Progress
[DEVICE_001] Phase 2: Configuration Load → Pending
[DEVICE_002] Phase 1: Bootloader Flash → Success
[DEVICE_002] Phase 1: Firmware Flash → In Progress
```

---

## Thread Management

### Main Thread
- UI event handling
- Dialog management
- Status updates

### Flash Thread (`multy_isp_thread`)
- Sequential device processing
- Phase 1 (Firmware) execution
- Posts messages to UI via `WM_MULTY_FLASH_MESSAGE`

### ISP Thread
- Invokes ISP.exe subprocess
- Waits for completion
- Parses result from INI file

### Message Handling (`MultyFlashMessage`)
- Receives `WM_MULTY_FLASH_MESSAGE`
- Updates UI elements:
  - Device status color
  - Progress bar
  - Status text
  - Error messages

---

## Cleanup & Finalization

```
CLEANUP PHASE:

[Close ISP Thread]
  ├─ Terminate ISP.exe if still running
  ├─ Close file handles
  └─ Clean temporary INI files

[Restore Communication]
  ├─ Reopen serial/network connections
  ├─ Recreate broadcast socket
  ├─ Restore previous communication parameters
  └─ Verify connectivity to all devices

[Resume Tree Refresh]
  ├─ Update device list in main UI
  ├─ Refresh device properties
  ├─ Show updated firmware versions
  └─ Display final status for each device

[Archive State & Logs]
  ├─ Move state file to archive
  ├─ Append update log to history
  ├─ Generate completion report
  ├─ Email/notify on completion
  └─ Backup for troubleshooting

[Final Summary]
  ├─ Display:
  │  ├─ Total devices: X
  │  ├─ Successful: Y
  │  ├─ Failed: Z
  │  ├─ Partial Success: W
  │  └─ Time elapsed: HH:MM:SS
  └─ Save to history.txt
```

---

## Configuration Files

### INI Files Used

**Update Configuration (auto_config.ini)**
```ini
[DEVICE_001]
MODBUS_ID=10
DEVICE_TYPE=TSTAT6
COM_PORT=COM3
BAUD_RATE=19200
FIRMWARE_FILE=C:\path\to\firmware.hex
FIRMWARE_MD5=abc123def456
BOOTLOADER_FILE=C:\path\to\bootloader.hex
NEW_BOOTLOAD=1
```

**ISP Result File (isp_result.ini)**
```ini
[RESULT]
DEVICE_ID=DEVICE_001
STATUS=SUCCESS
FIRMWARE_VERSION=1.3.0
BOOTLOADER_VERSION=2.1.0
TIME_TAKEN=45.3
ERROR_CODE=0
ERROR_MESSAGE=
```

### State File Location
- **Path**: `{APP_DATA}/T3000/update_states/`
- **Pattern**: `update_session_{TIMESTAMP}.json`
- **Example**: `update_session_20240515_103000.json`

---

## Key Files Involved

| File | Location | Purpose |
|------|----------|---------|
| **MainFrm.cpp** | src/ | Entry point (OnBatchFlashHex) |
| **Flash_Multy.cpp** | src/ | Dialog & thread management |
| **Flash_Multy.h** | src/ | Dialog class definition |
| **ComWriter.cpp** | ISP/ | Firmware transfer logic |
| **ComWriter.h** | ISP/ | COM writer class |
| **BacnetMstp.cpp** | ISP/ | MSTP communication |
| **ISPDlg.h** | ISP/ | ISP dialog definitions |
| **TstatFlashDlg.cpp** | ISP/ | TSTAT flash dialog |

---

## Resume Update Flow

```
User launches T3000
  ↓
Check for incomplete update state
  ├─ Found: `update_session_*.json` exists
  │  ├─ Parse state file
  │  ├─ Show "Resume Update?" dialog
  │  ├─ If YES:
  │  │  ├─ Load all checkpoints
  │  │  ├─ Ask which devices to update
  │  │  ├─ Skip completed devices
  │  │  ├─ Resume from last checkpoint
  │  │  └─ Start update process
  │  └─ If NO:
  │     ├─ Delete/archive old state
  │     └─ User can start fresh update
  │
  └─ Not found: Continue normal startup

During resumed update:
  ├─ Reload device list from state
  ├─ Restore progress bar state
  ├─ Restore error log display
  ├─ Continue from last checkpoint
  ├─ Skip bootloader if already done
  ├─ Resume firmware from chunk N
  ├─ Or skip firmware if done
  ├─ Continue with config from parameter M
  └─ Generate completion report

On successful completion:
  ├─ Archive state file
  ├─ Clear "incomplete update" flag
  ├─ Display final status
  └─ Allow starting new update
```

---

## Best Practices

### Before Starting Update
- [ ] Backup current device configuration
- [ ] Verify firmware file integrity (MD5)
- [ ] Check all devices are online and responding
- [ ] Ensure stable network/COM connection
- [ ] Close other applications using COM ports
- [ ] Test with single device first

### During Update
- [ ] Monitor progress in real-time
- [ ] Do NOT disconnect power to devices
- [ ] Do NOT close application during update
- [ ] Do NOT start another update session
- [ ] Watch for error messages and retry suggestions

### After Update
- [ ] Verify devices are online
- [ ] Check firmware versions match targets
- [ ] Validate configuration parameters
- [ ] Test device communication
- [ ] Review update log for warnings
- [ ] Archive state file for troubleshooting

---

## Troubleshooting

### Issue: "Update Resumed but Device Shows Different State"
**Solution**: State file corruption
- [ ] Delete state file
- [ ] Restart update
- [ ] Check firmware version manually on device

### Issue: "Stuck at 50% Progress"
**Solution**: Thread deadlock or device hang
- [ ] Wait 5 minutes
- [ ] If still stuck: Press Escape to pause
- [ ] Check device power/connection
- [ ] Resume update or restart

### Issue: "CRC Error Repeats on Same Chunk"
**Solution**: Communication or hardware issue
- [ ] Check COM port cable
- [ ] Reduce baud rate (try 9600)
- [ ] Replace COM cable
- [ ] Try different COM port

### Issue: "Configuration Load Fails After Firmware Success"
**Solution**: Device firmware issue or config incompatibility
- [ ] Verify device firmware version
- [ ] Check config file format
- [ ] Try manual config load
- [ ] Check device logs for errors

---

## Performance Optimization

### Parallel Processing (Future Enhancement)
```
Currently: Sequential (one device at a time)
Proposed: Parallel processing with:
  ├─ Max 3-4 devices in parallel
  ├─ Each device on separate COM port/network
  ├─ Separate thread pool for each device
  └─ Shared state management with locks
```

### Bandwidth Optimization
```
Current:
  ├─ Firmware chunk size: 1024 bytes
  ├─ Config parameter batch: 10 parameters

Optimized:
  ├─ Firmware chunk size: 4096 bytes (if stable)
  ├─ Config parameter batch: 20 parameters
  └─ Enable pipelining for faster transfer
```

---

## Monitoring & Logging

### Log Levels
- **DEBUG**: Detailed step-by-step information
- **INFO**: Major phase changes and milestones
- **WARN**: Recoverable errors, retries
- **ERROR**: Unrecoverable errors, failures
- **FATAL**: System-level failures

### Log Files
- **Main Log**: `T3000_update_{date}.log`
- **Error Log**: `T3000_update_{date}_errors.log`
- **Debug Log**: `T3000_update_{date}_debug.log` (if debug mode enabled)
- **History**: `history.txt` (appended for each update)

---

**Document Version**: 1.0
**Last Updated**: 2024-05-15
**Author**: T3000 Development Team
