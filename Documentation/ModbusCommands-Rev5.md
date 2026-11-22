# Modbus Command Set - Unified Proposal (Rev5)

Temco Controls. Proposal for the Modbus command revamp, written against the source in this repository and now keyed to the
**System Device List** agreed in review on 2026-11-20 (issue #504). Rev4 invented its own name scheme; Rev5 does not.
The device list is the registry, the **Panel Name (CBasic)** column is the address, and every command below takes that
name and nothing else.

Companion files: `ModbusCommands-Rev5.html` (single-page view). The Rev4 set - `ModbusCommands.md`, `ModbusCommands.html`, `ModbusCommands.xlsx` - stays in place beside it. The workbook is the register map of record until the Rev5 column set in section 2 is keyed into it.

---

## 0. What changed since Rev4

| Area | Rev4 | Rev5 |
| --- | --- | --- |
| Device name | invented `product-panel-address`, e.g. `vav-1-2`, kept in a name registry | the device's own **panel name**, exactly as it stands on the device, 20 characters, e.g. `VAV101` |
| Where it comes from | a new tree over `ALL_NODE` rows plus a naming rule | the **System Device List** table, column *Panel Name (CBasic)* - one source for Control Basic, Modbus and the tree |
| Transport | `MB_DEFINE_DEVICE` handle per device | the row's **Protocol / IP Address / Baud / Parity / Address** cells |
| `MB_DEFINE_DEVICE` | the primary way to reach anything | escape hatch only, for a meter or gateway that is not in the list |
| Legacy prefix | alias for a tree row | unchanged alias, `1.2.` and `199.4.` still resolve, now to a list row |
| Dotted paths | discouraged | never - names read `VAV101`, not a path |

---

## 1. Where the commands live

| Layer | File | What it exposes |
| --- | --- | --- |
| T3000 program keywords | `T3000ProgramMoudle/dllmain.cpp` | `MB_REGn`, `MB_COILn`, `MB_DISINPUTn`, `MB_INPUTREGn`, `MB_REG_FLOAT_*`, `MB_INPUT_FLOAT_*`, `MB_BLOCKREAD`, `MB_BLOCKWRITE`, `MB_BWCOIL`, and the `panel.slave.` prefix |
| Modbus Poll tools | `Modbus Poll/*.cpp` | one dialog per function code |
| Modbus DLL | `ModbusDllforVc/ModbusDllforVc/common.cpp` | 67 exports, 12 of them send traffic |

## 2. The registry: System Device List (issue #504)

The list is the twelve-column table agreed in the 2026-11-20 review. It is stored on the T3 devices themselves, so a
replacement PC rebuilds it from the hardware, and every tool - the tree, Control Basic, the Modbus commands, the poll
dialogs - reads the same rows.

| Column | Meaning | What Modbus takes from it |
| --- | --- | --- |
| **Main** | main-net item, or the parent panel of a sub-net row | route: the panel the frame is sent to |
| **Sub Net** | sub-net item name; blank on a main-net row | grouping only |
| **Serial ID** | device serial number | identity, diagnostics |
| **Product Name** | e.g. `VAV box`, `T3-BB`, `power meter` | display, default name suggestion |
| **Status** | online / offline | a row read as offline returns `MB_LAST_STATUS() = -5` without a frame |
| **Product ID** | model id | unit-id source when *Address* is blank |
| **Address** | sub id, or unit id on a main-net row | **unit id in the MBAP/PDU header** |
| **IP Address** | main-net rows only; blank on sub-net rows | **TCP host**, port 502 unless overridden |
| **Baud** | 9600 ... 115200 | RS485 attempt speed |
| **Parity** | N / E / O, with stop bits | RS485 framing |
| **Protocol** | `BACnet/IP`, `Modbus TCP`, `Modbus RS485` (BACnet/IP is the main-net default) | **which transport a command uses** |
| **Panel Name (CBasic)** | the device's own panel name, 20 characters | **the device argument of every command** |

Two consequences follow directly from the review:

- **Panel name is the address.** "Control Basic and our Modbus read/write commands address the device by that name." No
  second name is invented and none is kept in step.
- **Blank IP on sub-net rows is correct.** The IP belongs to the panel the device hangs off; the sub id in *Address* plus
  *Protocol / Baud / Parity* is what identifies and reaches it. A Modbus command needs no IP for such a device.

### 2.1 The panel-name rule, as used by the commands

| Rule | Detail |
| --- | --- |
| Shape | reads like `VAV101`, `AHU2-3`, `Meter1` - letters, digits, dash, underscore; at most **20 characters** |
| Source of truth | the device. The list shows the name as it stands; there is no rename dialog in the PC |
| Uniqueness | case-insensitive within one project; the list validation rejects a duplicate |
| No dots, no colons | the transport is not part of the name, so `1.2.VAV101` and `VAV101:502` are both rejected |
| Reserved | `REG`, `COIL`, `DI`, `IR`, `AI`, `AV`, `AO`, `BI`, `BO`, `BV`, `MSV`, `MB_*` |
| Rename on the device | the program line follows the device: one edit on the device, no edit in the program, because the name is looked up at run time |
| Not found | `MB_ERR_NO_DEVICE` (-5) plus a hint naming the closest match, never a silent zero |

The name is a lookup key, not a copy. Change it on the device and every program, trend and graphic that named it follows,
because all of them resolve through the same row.

## 3. Command map

Every command that exists today, the proposed replacement, and the method each one uses. Device argument is the panel name.

| # | Area | Op | FC | Today | New command | New method |
| --- | --- | --- | --- | --- | --- | --- |
| 1 | REG | READ | 0x03 | `VAR1 = 1.2.MB_REG7` | `VAR1 = MB_READ("VAV101", REG, 7)` | REG + count 1 -> FC 0x03, 1 register, panel 1 routed to sub id 2 |
| 2 | REG | WRITE | 0x06 | `1.2.MB_REG3 = VAR1` | `MB_WRITE("VAV101", REG, 3, VAR1)` | REG + single value -> FC 0x06, one register |
| 3 | COIL | READ | 0x01 | `VAR107 = 1.2.MB_COIL7` | `VAR107 = MB_READ("VAV101", COIL, 7)` | COIL + count 1 -> FC 0x01, one bit |
| 4 | DI | READ | 0x02 | `VAR108 = 1.2.MB_DISINPUT7` | `VAR108 = MB_READ("VAV101", DI, 7)` | DI + count 1 -> FC 0x02, one bit |
| 5 | IR | READ | 0x04 | `VAR109 = 1.2.MB_INPUTREG7` | `VAR109 = MB_READ("VAV101", IR, 7)` | IR + count 1 -> FC 0x04, one register |
| 6 | REG | READ F32 | 0x03 | `VAR100 = 1.2.MB_REG_FLOAT_ABCD33` | `VAR100 = MB_READ("VAV101", REG, 33, 2, F32, ABCD)` | REG + count 2 + type F32 + order ABCD -> FC 0x03, two registers |
| 7 | REG | READ F32 | 0x03 | `VAR101 = 1.2.MB_REG_FLOAT_CDAB44` | `VAR101 = MB_READ("VAV101", REG, 44, 2, F32, CDAB)` | REG + count 2 + type F32 + order CDAB -> FC 0x03, two registers |
| 8 | REG | READ F32 | 0x03 | `VAR102 = 1.2.MB_REG_FLOAT_BADC55` | `VAR102 = MB_READ("VAV101", REG, 55, 2, F32, BADC)` | REG + count 2 + type F32 + order BADC -> FC 0x03, two registers |
| 9 | REG | READ F32 | 0x03 | `VAR103 = 1.2.MB_REG_FLOAT_DCBA66` | `VAR103 = MB_READ("VAV101", REG, 66, 2, F32, DCBA)` | REG + count 2 + type F32 + order DCBA -> FC 0x03, two registers |
| 10 | IR | READ F32 | 0x04 | `VAR114 = 1.2.MB_INPUT_FLOAT_ABCD77` | `VAR114 = MB_READ("VAV101", IR, 77, 2, F32, ABCD)` | IR + count 2 + type F32 + order ABCD -> FC 0x04, two registers |
| 11 | REG | READ BLOCK | 0x03 | `VAR1 = MB_BLOCKREAD(1, 2, 100, 5)` | `VAR1[] = MB_READ("VAV101", REG, 100, 5)` | REG + count 5 -> FC 0x03, five registers, one frame |
| 12 | REG | WRITE BLOCK | 0x10 | `VAR1 = MB_BLOCKWRITE(1, 2, 100, 5, 10, 20, 30, 40, 50)` | `MB_WRITE("VAV101", REG, 100, 10, 20, 30, 40, 50)` | REG + value list -> FC 0x10, five registers |
| 13 | COIL | WRITE BLOCK | 0x0F | `VAR1 = MB_BWCOIL(1, 2, 0, 3, 1, 0, 1)` | `MB_WRITE("VAV101", COIL, 0, 1, 0, 1)` | COIL + status list -> FC 0x0F, three coils |
| 14 | REG | READ by IP | 0x03 | `VAR98 = 199.4.MB_REG55` | `VAR98 = MB_READ("Meter1", REG, 55)` | panel name resolves to the main-net row, FC 0x03 |
| 15 | REG | READ 32 | 0x03 | `VAR3 = 1.2.MB_REG3 * 65536 + 1.2.MB_REG4` | `VAR3 = MB_READ("VAV101", REG, 3, 2, U32, CDAB)` | REG + count 2 + type U32 -> FC 0x03, one frame |
| 16 | REG | READ bit | 0x03 | `VAR = 1.2.MB_REG7 & 1` | `VAR = MB_READ("VAV101", REG, 7, 1, BIT, bit=0)` | REG + type BIT + bit index -> FC 0x03 then extract the bit |
| 17 | ALL | DEVICE | - | `(no equivalent)` | `MB_DEFINE_DEVICE("ShopMeter", "TCP:192.168.0.44:502,TO=500,RT=2")` | ad-hoc device **not in the list**; registered devices never need this |
| 18 | ALL | STATUS | - | `(no equivalent)` | `MB_LAST_STATUS() / MB_LAST_EXCEPTION()` | 0 ok, -1 timeout, -2 CRC, -3 area, -4 range, -5 no device or offline, -6 exception |
| 19 | REG | WRITE F32 | 0x10 | `(no equivalent)` | `MB_WRITE("VAV101", REG, 400, 123.45, F32, ABCD)` | REG + type F32 + order -> FC 0x10, two registers |

### Examples

**1. REG READ - 0x03.** Read one holding register from the device the list calls `VAV101` (panel 1, sub id 2).

```text
40 VAR100 = 1.2.MB_REG7
```

```text
40 VAR100 = MB_READ("VAV101", REG, 7)
```

Same frame on the wire, only the spelling changes.

**2. REG WRITE - 0x06.** A write stops being a left-side assignment and becomes the same verb as read.

```text
230 1.2.MB_REG3 = VAR1 / 65536
230 MB_WRITE("VAV101", REG, 3, VAR1 / 65536)
```

**3. COIL READ - 0x01.** Reads already worked; the write half of this area is missing today and arrives with this set.

```text
120 VAR107 = 1.2.MB_COIL7
120 VAR107 = MB_READ("VAV101", COIL, 7)
```

**4. DI READ - 0x02.** Read only by definition; a write is rejected in the editor and returns `MB_ERR_AREA` at run time.

```text
130 VAR108 = 1.2.MB_DISINPUT7
130 VAR108 = MB_READ("VAV101", DI, 7)
```

**5. IR READ - 0x04.** Same function code, cleaner token.

```text
140 VAR109 = 1.2.MB_INPUTREG7
140 VAR109 = MB_READ("VAV101", IR, 7)
```

**6-9. REG READ F32 - 0x03.** Four float spellings collapse into a type and an order argument.

```text
40 VAR100 = 1.2.MB_REG_FLOAT_ABCD33
40 VAR100 = MB_READ("VAV101", REG, 33, 2, F32, ABCD)
```

`CDAB`, `BADC` and `DCBA` follow the same pattern; `BE` and `LE` stay as aliases for `ABCD` and `DCBA`.

**10. IR READ F32 - 0x04.** Four input-register float keywords exist in the firmware and in the demo program but are missing from the forum write-up.

```text
80 VAR114 = 1.2.MB_INPUT_FLOAT_ABCD77
80 VAR114 = MB_READ("VAV101", IR, 77, 2, F32, ABCD)
```

**11. REG READ BLOCK - 0x03.** Count limit 125 registers, the specification maximum. No silent splitting.

```text
200 VAR1 = MB_BLOCKREAD(1, 2, 100, 5)
200 VAR1[] = MB_READ("VAV101", REG, 100, 5)
```

**12. REG WRITE BLOCK - 0x10.** Legacy 25-value ceiling stays for `MB_BLOCKWRITE` only; the new verb allows the 123-register limit.

```text
210 VAR1 = MB_BLOCKWRITE(1, 2, 100, 5, 10, 20, 30, 40, 50)
210 MB_WRITE("VAV101", REG, 100, 10, 20, 30, 40, 50)
```

**13. COIL WRITE BLOCK - 0x0F.** Second undocumented gap; single coil write (0x05) gets a keyword for the first time.

```text
220 VAR1 = MB_BWCOIL(1, 2, 0, 3, 1, 0, 1)
220 MB_WRITE("VAV101", COIL, 0, 1, 0, 1)
```

**14. REG READ on a main-net device.** `199.4.` reached the meter by the last octet of its IP. In the list the meter carries a panel name and a *Protocol* of `Modbus TCP`, so the command names it.

```text
259 VAR98 = 199.4.MB_REG55
259 VAR98 = MB_READ("Meter1", REG, 55)
```

**15. REG READ 32 - 0x03.** Replaces arithmetic homework, one frame instead of two, `S32` available as well.

```text
251 VAR3 = 1.2.MB_REG3 * 65536 + 1.2.MB_REG4
251 VAR3 = MB_READ("VAV101", REG, 3, 2, U32, CDAB)
```

**16. REG READ bit - 0x03.** Masking still works, the typed read documents the intent.

```text
100 VAR105 = 1.2.MB_REG7 & 1
100 VAR105 = MB_READ("VAV101", REG, 7, 1, BIT, bit=0)
```

**17. Unlisted device.** A gateway that has never been scanned has no row, so it is defined inline - the one surviving use of `MB_DEFINE_DEVICE`.

```text
MB_DEFINE_DEVICE("ShopMeter", "TCP:192.168.0.44:502,TO=500,RT=2")
VAR_E = MB_READ("ShopMeter", REG, 300, 2, F32, ABCD)
```

**18. Status.** A failed read used to return the same shape as a zero, exception codes invisible.

```text
IF MB_LAST_STATUS() <> 0 THEN ALARM 1
```

**19. REG WRITE F32 - 0x10.** No float write exists in a program today, only integer and block writes.

```text
MB_WRITE("VAV101", REG, 400, 123.45, F32, ABCD)
```

## 4. How a name resolves

```text
MB_READ("VAV101", REG, 7)
   -> System Device List, column Panel Name (CBasic), case-insensitive exact match
   -> row: Main = ctl-1, Sub Net = VAV group, Address = 2, Protocol = Modbus RS485, Baud = 115200, Parity = N
   -> transport = Modbus RS485 on the panel named in Main, unit id = Address
   -> panel ctl-1: IP from its own row, port 502, MBAP unit id 2, PDU -> FC 03, address 7, count 1
```

Resolution order, unchanged in spirit and shorter in practice:

1. a row in the System Device List whose *Panel Name (CBasic)* matches;
2. an inline `MB_DEFINE_DEVICE` handle of the same name;
3. the legacy `panel.slave.` prefix or `IP:<octet>.<unit>` token, resolved to a row if one matches, otherwise treated
   as unregistered.

### 4.1 Per-protocol behaviour

| Protocol in the row | Host | Unit id | Notes |
| --- | --- | --- | --- |
| `Modbus TCP` | *IP Address*, port 502 | *Address*, else *Product ID*, else 1 | main-net device, one TCP frame, no routing |
| `Modbus RS485` | none - routed through the panel named in *Main* | *Address* | framing from *Baud* and *Parity*; blank *IP* is expected and correct |
| `BACnet/IP` | *IP Address* | - | the default for main-net T3 devices; a Modbus command against such a row is refused with `MB_ERR_AREA` and a hint, never silently re-routed |
| offline row (*Status*) | - | - | returns -5 without opening the socket, so a dead device cannot stall a program |

### 4.2 Worked case, end to end

```text
MB_WRITE("VAV101", REG, 3, VAR1)                 // FC 06 through panel ctl-1, unit 2, RS485 at 115200 N
VAR107 = MB_READ ("VAV101", COIL, 7)             // FC 01, same route
VAR100 = MB_READ ("VAV101", REG, 33, 2, F32, ABCD)
MB_WRITE("VAV101", COIL, 0, 1, 0, 1)             // FC 0F, three coils - no keyword exists for this today
STA    = MB_LAST_STATUS()                        // 0 ok, -1 timeout, -2 CRC, -5 no device or offline, -6 exception
```

Not one line names the panel, the port or the baud. Move `VAV101` from the RS485 subnet to its own Ethernet adapter,
rescan, and the row's *Protocol* becomes `Modbus TCP` with an IP; the program lines are untouched.

## 5. What the list must guarantee for the commands

The review already fixes most of this; these are the points the command set depends on.

| Requirement | Why the commands need it |
| --- | --- |
| Panel name unique per project, case-insensitive, 20 characters | the device argument of every command |
| Sub-net rows keep *IP Address* blank | stops anyone hard-coding a panel IP into a device that has none |
| *Protocol* reports what the device is actually set to | picks TCP or RS485 routing instead of an assumption |
| *Baud* and *Parity* sit on the sub-net row | the RS485 attempt is built from the row, not from a global setting |
| Offline rows keep their red status and grey background | a command against them returns -5 locally, at no cost on the wire |
| Modbus rows cached before delete (`cache\AllNodes_Modbus_YYYYMMDD_HHMMSS.sql`) | a mis-delete cannot break programs; `Undo (n)` puts the rows back |
| Current row highlighted blue | the row being edited is never confused with a bad one |

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

The function code is derived from area and direction: REG read 0x03 / write 0x06 / block 0x10, COIL read 0x01 / write
0x05 / block 0x0F, DI read 0x02, IR read 0x04. Limits: 125 registers per read, 123 per block write, 2000 bits per read,
1968 coils per block write, all checked before a frame is built. `device` is the panel name from the list, or an
`MB_DEFINE_DEVICE` handle for a device with no row.

## 8. Migration plan

| Phase | Work | Exit condition |
| --- | --- | --- |
| P0 | New verbs as translations onto the existing DLL calls; legacy keywords untouched | every new command produces the same bytes as its legacy pair, verified with the `_log` exports |
| P1 | Panel-name lookup over the System Device List | `MB_READ("VAV101", ...)` and `1.2.MB_REG7` reach the same unit with byte-identical traffic |
| P2 | Type system and byte order in one place | the eight float keywords become two aliases, no `* 65536 +` advice left |
| P3 | Coil and discrete areas, single and block writes | a program can drive a coil and read a discrete input through the same verb |
| P4 | Protocol column drives transport, offline rows short-circuit | a `Modbus TCP` row and a `Modbus RS485` row are reached by the same verb, an offline row costs no socket |
| P5 | Collapse `_2`, `_nocretical`, `_tap` onto one entry point | one implementation per function code |
| P6 | Docs and UI | this document replaces the forum write-up; the register tool shows the resolved row, protocol and last status |

## 9. Open decisions

1. Unit id when *Address* is blank on a main-net Modbus TCP row - fall back to *Product ID* (assumed here) or force an edit.
2. Array return for block reads, or a separate call name, since the program engine has no array type.
3. Whether a panel-name rename on the device keeps a PC-side alias for programs written against the old name, or simply
   follows the device with no alias at all.
4. `BIT` as a type or as an operator on a value.
5. Whether diagnostics 0x08 and exception status 0x07 are in scope.
6. Auto-splitting oversized blocks, or always `MB_ERR_RANGE`.

## 10. Provenance

Issue #504 *System Device List in T3000*, 2026-11-20 review - twelve-column layout, panel-name column, protocol/serial
settings, delete cache and undo. Forum: topic 2582 posts 1, 5, 6, 8; topic 2288 post 2; topic 156 post 1; topic 585 posts
2 and 5. Source: `common.cpp`, `dllmain.cpp`, `DemoPrg.txt`. Page: `ModbusCommands-Rev5.html`.
