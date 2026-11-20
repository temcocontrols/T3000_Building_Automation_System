# Modbus Command Set - Unified Proposal (Rev2)

Temco Controls. Proposal for the Modbus command revamp, written against the source in this repository and checked line by line against the published forum write-up.

Companion files: `ModbusCommands.xlsx` (mapping workbook) and `ModbusCommands.html` (split view page).

## 1. Where the commands live

| Layer | File | What it exposes |
| --- | --- | --- |
| T3000 program keywords | `T3000ProgramMoudle/dllmain.cpp` | `MB_REGn`, `MB_COILn`, `MB_DISINPUTn`, `MB_INPUTREGn`, `MB_REG_FLOAT_*`, `MB_INPUT_FLOAT_*`, `MB_BLOCKREAD`, `MB_BLOCKWRITE`, `MB_BWCOIL`, and the `panel.slave.` prefix |
| Modbus Poll tools | `Modbus Poll/*.cpp` | one dialog per function code |
| Modbus DLL | `ModbusDllforVc/ModbusDllforVc/common.cpp` | 67 exports, 12 of them send traffic |

## 2. Command map

Every command that exists today, the proposed replacement, and the method each one uses.

| # | Area | Op | FC | Today | New command | New method |
| --- | --- | --- | --- | --- | --- | --- |

| 1 | REG | READ | 0x03 | `VAR1 = 1.2.MB_REG7` | `VAR1 = MB_READ("SUB:1.2", REG, 7)` | REG + count 1 -> FC 0x03, 1 register, unit id 2, routed through panel 1 |

| 2 | REG | WRITE | 0x06 | `1.2.MB_REG3 = VAR1` | `MB_WRITE("SUB:1.2", REG, 3, VAR1)` | REG + single value -> FC 0x06, one register |

| 3 | COIL | READ | 0x01 | `VAR107 = 1.2.MB_COIL7` | `VAR107 = MB_READ("SUB:1.2", COIL, 7)` | COIL + count 1 -> FC 0x01, one bit |

| 4 | DI | READ | 0x02 | `VAR108 = 1.2.MB_DISINPUT7` | `VAR108 = MB_READ("SUB:1.2", DI, 7)` | DI + count 1 -> FC 0x02, one bit |

| 5 | IR | READ | 0x04 | `VAR109 = 1.2.MB_INPUTREG7` | `VAR109 = MB_READ("SUB:1.2", IR, 7)` | IR + count 1 -> FC 0x04, one register |

| 6 | REG | READ F32 | 0x03 | `VAR100 = 1.2.MB_REG_FLOAT_ABCD33` | `VAR100 = MB_READ("SUB:1.2", REG, 33, 2, F32, ABCD)` | REG + count 2 + type F32 + order ABCD -> FC 0x03, two registers |

| 7 | REG | READ F32 | 0x03 | `VAR101 = 1.2.MB_REG_FLOAT_CDAB44` | `VAR101 = MB_READ("SUB:1.2", REG, 44, 2, F32, CDAB)` | REG + count 2 + type F32 + order CDAB -> FC 0x03, two registers |

| 8 | REG | READ F32 | 0x03 | `VAR102 = 1.2.MB_REG_FLOAT_BADC55` | `VAR102 = MB_READ("SUB:1.2", REG, 55, 2, F32, BADC)` | REG + count 2 + type F32 + order BADC -> FC 0x03, two registers |

| 9 | REG | READ F32 | 0x03 | `VAR103 = 1.2.MB_REG_FLOAT_DCBA66` | `VAR103 = MB_READ("SUB:1.2", REG, 66, 2, F32, DCBA)` | REG + count 2 + type F32 + order DCBA -> FC 0x03, two registers |

| 10 | IR | READ F32 | 0x04 | `VAR114 = 1.2.MB_INPUT_FLOAT_ABCD77` | `VAR114 = MB_READ("SUB:1.2", IR, 77, 2, F32, ABCD)` | IR + count 2 + type F32 + order ABCD -> FC 0x04, two registers |

| 11 | REG | READ BLOCK | 0x03 | `VAR1 = MB_BLOCKREAD(1, 2, 100, 5)` | `VAR1[] = MB_READ("SUB:1.2", REG, 100, 5)` | REG + count 5 -> FC 0x03, five registers, one frame |

| 12 | REG | WRITE BLOCK | 0x10 | `VAR1 = MB_BLOCKWRITE(1, 2, 100, 5, 10, 20, 30, 40, 50)` | `MB_WRITE("SUB:1.2", REG, 100, 10, 20, 30, 40, 50)` | REG + value list -> FC 0x10, five registers |

| 13 | COIL | WRITE BLOCK | 0x0F | `VAR1 = MB_BWCOIL(1, 2, 0, 3, 1, 0, 1)` | `MB_WRITE("SUB:1.2", COIL, 0, 1, 0, 1)` | COIL + status list -> FC 0x0F, three coils |

| 14 | REG | READ by IP | 0x03 | `VAR98 = 199.4.MB_REG55` | `VAR98 = MB_READ("IP:199.4", REG, 55)` | IP transport string -> panel resolution by last octet, FC 0x03 |

| 15 | REG | READ 32 | 0x03 | `VAR3 = 1.2.MB_REG3 * 65536 + 1.2.MB_REG4` | `VAR3 = MB_READ("SUB:1.2", REG, 3, 2, U32, CDAB)` | REG + count 2 + type U32 -> FC 0x03, one frame |

| 16 | REG | READ bit | 0x03 | `VAR = 1.2.MB_REG7 & 1` | `VAR = MB_READ("SUB:1.2", REG, 7, 1, BIT, bit=0)` | REG + type BIT + bit index -> FC 0x03 then extract the bit |

| 17 | ALL | DEVICE | - | `(no equivalent)` | `MB_DEFINE_DEVICE("Meter1", "TCP:192.168.0.199:502,TO=500,RT=2")` | TCP / RTU / SUB / IP transport in one handle, optional timeout and retries |

| 18 | ALL | STATUS | - | `(no equivalent)` | `MB_LAST_STATUS() / MB_LAST_EXCEPTION()` | 0 ok, -1 timeout, -2 CRC, -3 area, -4 range, -5 no device, -6 exception |

| 19 | REG | WRITE F32 | 0x10 | `(no equivalent)` | `MB_WRITE("SUB:1.2", REG, 400, 123.45, F32, ABCD)` | REG + type F32 + order -> FC 0x10, two registers |


### Examples

**1. REG READ - 0x03**  

Today: Read one holding register from unit 2 on panel 1.  

```text
40 VAR100 = 1.2.MB_REG7
```

```text
40 VAR100 = MB_READ("SUB:1.2", REG, 7)
```

Same frame on the wire, only the spelling changes.

**2. REG WRITE - 0x06**  

Today: Write one holding register. Today a write is an assignment, not a call.  

```text
230 1.2.MB_REG3 = VAR1 / 65536
```

```text
230 MB_WRITE("SUB:1.2", REG, 3, VAR1 / 65536)
```

Write stops being a left-side assignment and becomes the same verb as read.

**3. COIL READ - 0x01**  

Today: Read one coil bit. Read only: there is no program keyword to write a coil.  

```text
120 VAR107 = 1.2.MB_COIL7
```

```text
120 VAR107 = MB_READ("SUB:1.2", COIL, 7)
```

Reads already worked; the write half of this area is missing today and arrives with this set.

**4. DI READ - 0x02**  

Today: Read one discrete input bit. Read only by definition.  

```text
130 VAR108 = 1.2.MB_DISINPUT7
```

```text
130 VAR108 = MB_READ("SUB:1.2", DI, 7)
```

A write to DI is rejected in the editor and returns MB_ERR_AREA at run time.

**5. IR READ - 0x04**  

Today: Read one input register. Read only by definition.  

```text
140 VAR109 = 1.2.MB_INPUTREG7
```

```text
140 VAR109 = MB_READ("SUB:1.2", IR, 7)
```

Same function code, cleaner token.

**6. REG READ F32 - 0x03**  

Today: Read a 32-bit float, high word first, from two holding registers.  

```text
40 VAR100 = 1.2.MB_REG_FLOAT_ABCD33
```

```text
40 VAR100 = MB_READ("SUB:1.2", REG, 33, 2, F32, ABCD)
```

One of four float spellings replaced by a type and an order argument.

**7. REG READ F32 - 0x03**  

Today: Read a 32-bit float, word swapped.  

```text
50 VAR101 = 1.2.MB_REG_FLOAT_CDAB44
```

```text
50 VAR101 = MB_READ("SUB:1.2", REG, 44, 2, F32, CDAB)
```

CDAB and BE/LE aliases accepted, the 2025 proposal used BE/LE.

**8. REG READ F32 - 0x03**  

Today: Read a 32-bit float, byte swapped within each word.  

```text
60 VAR102 = 1.2.MB_REG_FLOAT_BADC55
```

```text
60 VAR102 = MB_READ("SUB:1.2", REG, 55, 2, F32, BADC)
```

No behavioural change.

**9. REG READ F32 - 0x03**  

Today: Read a 32-bit float, both word and byte swapped.  

```text
70 VAR103 = 1.2.MB_REG_FLOAT_DCBA66
```

```text
70 VAR103 = MB_READ("SUB:1.2", REG, 66, 2, F32, DCBA)
```

DCBA also accepted as LE.

**10. IR READ F32 - 0x04**  

Today: Four input-register float keywords that exist in the firmware and the demo program but are missing from the forum write-up.  

```text
80 VAR114 = 1.2.MB_INPUT_FLOAT_ABCD77
```

```text
80 VAR114 = MB_READ("SUB:1.2", IR, 77, 2, F32, ABCD)
```

Undocumented gap found in the source. Four keywords collapse into one area change.

**11. REG READ BLOCK - 0x03**  

Today: Read n contiguous holding registers in one frame.  

```text
200 VAR1 = MB_BLOCKREAD(1, 2, 100, 5)
```

```text
200 VAR1[] = MB_READ("SUB:1.2", REG, 100, 5)
```

Count limit 125 registers, the specification maximum. No silent splitting.

**12. REG WRITE BLOCK - 0x10**  

Today: Write n contiguous holding registers in one frame. Legacy program limit is 25 values.  

```text
210 VAR1 = MB_BLOCKWRITE(1, 2, 100, 5, 10, 20, 30, 40, 50)
```

```text
210 MB_WRITE("SUB:1.2", REG, 100, 10, 20, 30, 40, 50)
```

Legacy 25-value ceiling stays for MB_BLOCKWRITE only, the new verb allows the 123-register specification limit.

**13. COIL WRITE BLOCK - 0x0F**  

Today: Block write coils. Present in the parser, absent from the forum documentation.  

```text
220 VAR1 = MB_BWCOIL(1, 2, 0, 3, 1, 0, 1)
```

```text
220 MB_WRITE("SUB:1.2", COIL, 0, 1, 0, 1)
```

Second undocumented gap. Single coil write (FC 0x05) gets a keyword for the first time.

**14. REG READ by IP - 0x03**  

Today: Read a device selected by the last octet of its IP, unit id 4. If the local panel number is also 199, the panel number wins.  

```text
259 VAR98 = 199.4.MB_REG55
```

```text
259 VAR98 = MB_READ("IP:199.4", REG, 55)
```

The ambiguous prefix becomes an explicit transport token, the precedence rule is unchanged.

**15. REG READ 32 - 0x03**  

Today: Customer-facing trick to assemble a 32-bit value from two registers, two separate frames today.  

```text
251 VAR3 = 1.2.MB_REG3 * 65536 + 1.2.MB_REG4
```

```text
251 VAR3 = MB_READ("SUB:1.2", REG, 3, 2, U32, CDAB)
```

Replaces arithmetic homework, one frame instead of two, S32 available as well.

**16. REG READ bit - 0x03**  

Today: Read a whole register and mask a single bit in the program.  

```text
100 VAR105 = 1.2.MB_REG7 & 1
```

```text
100 VAR105 = MB_READ("SUB:1.2", REG, 7, 1, BIT, bit=0)
```

Masking still works, the typed read documents the intent.

**17. ALL DEVICE - -**  

Today: Today the transport lives inside the address prefix. There is no way to name a device once and reuse it.  

```text
MB_DEFINE_DEVICE("Meter1", "TCP:192.168.0.199:502,TO=500,RT=2")\nVAR_E = MB_READ("Meter1", REG, 300, 2, F32, ABCD)
```

New capability. The 2025 forum proposal used MB_DEFINE_DEVICE but it was never implemented.

**18. ALL STATUS - -**  

Today: A failed read returns the same shape as a zero, exception codes are invisible.  

```text
IF MB_LAST_STATUS() <> 0 THEN ALARM 1
```

New capability. Makes a failed poll detectable in a program.

**19. REG WRITE F32 - 0x10**  

Today: No float write exists in a program today, only integer writes and block writes.  

```text
MB_WRITE("SUB:1.2", REG, 400, 123.45, F32, ABCD)
```

New capability, symmetric with the float read.

## 3. Forum cross-check

Each command in the forum write-up, what it claims, and what the source does.

| # | Forum statement | Source reality | Verdict |
| --- | --- | --- | --- |

| 1 | Read Holding Registers, FC 0x03 | Matches common.cpp: Read_One, Read_One2, Read_One_tap all build 0x03. | Confirmed |

| 2 | Function code per area: 0x01 coils, 0x02 discrete inputs, 0x03 holding, 0x04 input | Matches. Modbus_Standard_Read takes function_code as an argument for exactly these four. | Confirmed |

| 3 | Write of a register is an assignment, not a write command | Matches the parser, the left side of an equals sign selects 0x06. | Confirmed |

| 4 | Block write uses Write Multiple Registers and caps at 25 values | FC is 0x10 as stated. The 25-value cap is the legacy program limit, the specification allows 123. | Confirmed, limit explained |

| 5 | MB_BLOCKWRITE(2, 100, 5, 10, ...) takes slave, start, length | The shipped demo program uses MB_BLOCKWRITE(1, 2, 100, 5, 10, ...) with a leading panel argument. The forum example is missing that argument. | Discrepancy |

| 6 | Coils are read with 0x01 | Confirmed, but the DLL also implements 0x05 and 0x0F while no program keyword exposes them. | Half documented |

| 7 | Discrete inputs read with 0x02, read only | Confirmed. | Confirmed |

| 8 | Input registers read with 0x04 | Confirmed. | Confirmed |

| 9 | Float reads in ABCD, CDAB, BADC, DCBA | Confirmed, and the source shows the same four orders also exist for input registers as MB_INPUT_FLOAT_*, which the write-up never mentions. | Confirmed + gap |

| 10 | 199.4.MB_REG55 means IP ending 199, unit 4, panel wins on collision | Confirmed by the addressing rule and the demo program lines 259 and 260. | Confirmed |

| 11 | 32-bit values are assembled as MB_REG3 * 65536 + MB_REG4 | Confirmed as the only way today, two frames per value. This is what the typed read replaces. | Confirmed, to be retired |

| 12 | 60 and 70 were stray values in the block write example | The forum correction is right, the demo program carries no such values. | Corrected on the forum |

| 13 | MB_DEFINE_DEVICE, MB_READ_BLOCK, MB_READ_FLOAT and friends | Not implemented. The team confirmed in post 8 that MB_DEFINE_DEVICE is not supported in the programming environment yet. | Proposal only |

| 14 | MB_BLOCKREAD and MB_BWCOIL | Both are live parser tokens in dllmain.cpp, tokens MB_BR and MB_BW_COIL. | Undocumented |

| 15 | Modbus RTU and Modbus TCP both reachable from one controller, subnets transparent | Confirmed by the network programming overview, subnet unit id shares the Bacnet MAC parameter. | Confirmed |

### Gaps found in the published list

- `MB_BLOCKREAD` and `MB_BWCOIL` are live parser tokens in `dllmain.cpp` and appear in no forum post.
- The four `MB_INPUT_FLOAT_*` keywords exist in firmware and in `DemoPrg.txt` but the forum write-up only covers the holding-register float set.
- Coil writes (0x05 and 0x0F) are implemented in the DLL with no program keyword, so a program cannot drive a coil today.
- The forum block-write example omits the leading panel argument that the shipped demo program uses.
- `MB_DEFINE_DEVICE` was proposed in March 2025 and the team confirmed it was not supported; it is still not supported.

## 4. Devices and transport

| Token | Meaning | Example |
| --- | --- | --- |

| `TCP:<ip>[:<port>]` | Modbus TCP, port defaults to 502 | `MB_DEFINE_DEVICE("Meter1","TCP:192.168.0.199")` |

| `RTU:COM<n>,<baud>,<parity>,<stop>,<slave>` | Serial port on the PC | `MB_DEFINE_DEVICE("Pump1","RTU:COM3,9600,N,1,5")` |

| `SUB:<panel>.<slave>` | Device on the RS485 subnet of a controller, reached transparently | `MB_DEFINE_DEVICE("VAV7","SUB:1.2")` |

| `IP:<last_octet>.<slave>` | Legacy shortcut, kept for migration | `MB_READ("IP:199.4", REG, 55)` |

| `[,TO=<ms>][,RT=<n>][,ID=<unit>]` | Optional timeout, retries and unit id override on any transport | `"TCP:192.168.0.199:502,TO=500,RT=2"` |


Resolution order: defined handle, then explicit token, then the legacy `panel.slave` prefix. Panel number wins over IP octet on collision, as today.

## 5. Device handles: the `SUB:1.2` case

A subnet device is not a different kind of command, it is a different route to the same commands. `MB_DEFINE_DEVICE` binds a name to the route once, so `VAV7` and an Ethernet meter are used identically and the dotted prefix disappears from the program body.

| Handle | Transport | Panel | Slave | Today's form | FCs reachable | What it means |
| --- | --- | --- | --- | --- | --- | --- |

| `VAV7` | `SUB:1.2` | 1 | 2 | `1.2.` | 03 06 10 01 05 0F 02 04 | VAV box on the RS485 subnet of panel 1. Panel 1 is the controller whose IP ends in .1; the frame is routed to it and reaches unit id 2 on the subnet. Reads, writes, blocks and coils all work through the one handle. |

| `Meter1` | `TCP:192.168.0.199:502,TO=500,RT=2` | - | - | `199.x. prefix today` | 03 06 10 01 05 0F 02 04 | Ethernet meter by IP, with its own timeout and retries instead of the global response time. |

| `Pump1` | `RTU:COM3,9600,N,1,5` | - | 5 | `no prefix today, COM port is chosen in the UI` | 03 06 10 01 05 0F 02 04 | Serial device on the PC COM port. The port settings live in the handle, once. |

| `IP:199.4` | `legacy shortcut, no registration` | 199 | 4 | `199.4.` | 03 06 10 02 04 | The old dotted prefix kept as a token so untouched programs keep working. Panel wins over IP octet on collision. |


### Worked case

```text
MB_DEFINE_DEVICE("VAV7", "SUB:1.2")            // panel 1, slave 2, RS485 subnet, routed transparently

VAR1   = MB_READ ("VAV7", REG, 7)               // FC 03, unit 2 through panel 1 - same bytes as 1.2.MB_REG7
MB_WRITE("VAV7", REG, 3, VAR1)                  // FC 06 - same bytes as 1.2.MB_REG3 = VAR1
VAR100 = MB_READ ("VAV7", REG, 33, 2, F32, ABCD) // FC 03, two registers - replaces MB_REG_FLOAT_ABCD33
VAR107 = MB_READ ("VAV7", COIL, 7)              // FC 01
MB_WRITE("VAV7", COIL, 0, 1, 0, 1)             // FC 0F, three coils - no keyword exists for this today
STA    = MB_LAST_STATUS()                        // 0 ok, -1 timeout, -2 CRC, -5 no device, -6 exception
```

On the wire the handle produces a normal Modbus TCP request to the panel: MBAP header to panel 1 (the controller whose IP ends in `.1`), unit id 2 in the PDU header, and the panel forwards it down its RS485 subnet. A program that used `1.2.MB_REG7` gets byte-identical traffic, so P0 can swap spellings without touching a single device.

The handle also carries what the prefix cannot: poll rate, timeout, retries, an optional unit id override, and a display name that matches the device tree in the UI. Today those live per call (`SetResponseTime`) or nowhere. `AddDeviceDlg` becomes the place that writes the registration, the program file only reads it.

## 6. Types and byte order

| Type | Registers | Meaning |
| --- | --- | --- |
| `U16` | 1 | unsigned 16-bit, default |
| `S16` | 1 | signed 16-bit |
| `U32` | 2 | unsigned 32-bit |
| `S32` | 2 | signed 32-bit |
| `F32` | 2 | IEEE-754 single |
| `BIT` | 1 | one bit, with `bit=<0..15>` |

Order tokens: `ABCD`, `CDAB`, `BADC`, `DCBA`, with `BE` and `LE` kept as aliases for `ABCD` and `DCBA`.

## 7. Method of operation

```text
MB_READ (device, area, address [, count] [, type [, order]])
MB_WRITE(device, area, address, value | value_list [, type [, order]])
```

The function code is derived from area and direction: REG read 0x03 / write 0x06 / block 0x10, COIL read 0x01 / write 0x05 / block 0x0F, DI read 0x02, IR read 0x04. Limits: 125 registers per read, 123 per block write, 2000 bits per read, 1968 coils per block write, all checked before a frame is built.

## 8. Migration plan

| Phase | Work | Exit condition |
| --- | --- | --- |
| P0 | New verbs as translations onto the existing DLL calls; legacy keywords untouched | every new command produces the same bytes as its legacy pair, verified with the `_log` exports |
| P1 | Type system and byte order in one place | the eight float keywords become two aliases, no `* 65536 +` advice left |
| P2 | Coil and discrete areas, single and block writes | a program can drive a coil and read a discrete input through the same verb |
| P3 | Collapse `_2`, `_nocretical`, `_tap` onto one entry point | one implementation per function code |
| P4 | Docs and UI | this document replaces the forum write-up, the register tool shows the resolved device and last status |

## 9. Open decisions

1. Array return for block reads, or a separate call name, since the program engine has no array type.
2. `MB_DEFINE_DEVICE` in the program file or in the project database so UI and program share handles.
3. `BIT` as a type or as an operator on a value.
4. Whether diagnostics 0x08 and exception status 0x07 are in scope.
5. Auto-splitting oversized blocks, or always `MB_ERR_RANGE`.

## 10. Provenance

Forum: topic 2582 posts 1, 5, 6, 8; topic 2288 post 2; topic 156 post 1; topic 585 posts 2 and 5. Source: `common.cpp`, `dllmain.cpp`, `DemoPrg.txt`. Workbook: `ModbusCommands.xlsx`. Page: `ModbusCommands.html`.
