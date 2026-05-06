# Resumable Firmware Update – Proposed Approach

## Overview
This document describes the proposed approach for implementing **resumable firmware update** using a custom bootloader.

The goal is to safely resume firmware updates after interruption (e.g., RS485 disconnect, tool closure), without risking corrupted firmware or unstable device behavior.

---

## Key Concept

Instead of blindly resuming from any byte position, the system will:

- Write firmware in **fixed-size chunks**
- **Verify each chunk**
- Store **last valid written offset**
- Resume only from a **safe and verified point**

---

## Firmware Update Flow

### Step-by-Step Process

1. **Start Update**
   - Device enters **"update in progress"** state
   - Bootloader takes control

2. **Chunk-Based Writing**
   - Firmware is divided into fixed-size chunks (e.g., 1KB / 4KB)
   - Each chunk is:
     - Written to flash
     - Verified (CRC/checksum)

3. **Progress Storage**
   - After each successful chunk:
     - Bootloader stores:
       - Last valid offset
       - Firmware identification (version/hash)

4. **Interruption Handling**
   - If update is interrupted:
     - Device remains in bootloader mode
     - Last valid offset is preserved

5. **Resume Request**
   - When update restarts:
     - Software requests device status
     - Device responds with:
       - Last valid offset
       - Firmware identification

6. **Validation**
   - Software verifies:
     - Firmware file matches device info
   - If mismatch:
     - Restart full update (safety fallback)

7. **Resume Update**
   - Software sends remaining firmware from last valid offset
   - Bootloader continues chunk-based writing + verification

8. **Completion**
   - After full firmware is written:
     - Perform final validation (full image check)
     - Clear "update in progress" flag
     - Jump to main application

---
## Flowchart

```mermaid
flowchart TD
A[Start Update] --> B[Bootloader Mode]
B --> C[Write Chunk]
C --> D[Verify Chunk]
D --> E[Save Offset]
E --> F{Interrupted}

F -->|No| C
F -->|Yes| G[Wait Reconnect]

G --> H[Request Status]
H --> I[Send Offset]

I --> J{Match}

J -->|No| K[Restart]
J -->|Yes| L[Resume]

L --> C

C --> M{Done}

M -->|No| C
M -->|Yes| N[Validate]

N --> O[Clear State]
O --> P[Run App]