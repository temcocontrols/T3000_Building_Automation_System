# WireGuard Modbus Register Map

## Overview

This document describes the Modbus register interface used to configure WireGuard VPN functionality on T3 ESP32 controllers.

WireGuard configuration is exposed through Modbus Holding Registers and can be accessed using:

* Function Code 03 (Read Holding Registers)
* Function Code 06 (Write Single Register)
* Function Code 16 (Write Multiple Registers)

All configuration changes are automatically saved to flash memory.

---

## Register Allocation

| Register Range | Description              |
| -------------- | ------------------------ |
| 2100           | WireGuard Enable         |
| 2101 - 2133    | Private Key              |
| 2134 - 2166    | Peer Public Key          |
| 2167 - 2199    | Pre-Shared Key           |
| 2200 - 2203    | Local VPN IP Address     |
| 2204           | WireGuard Port           |
| 2205 - 2208    | Peer Endpoint IP Address |
| 2209 - 2250    | Reserved                 |

---

## Register Summary

| Register  | Name             | Access | Description                 |
| --------- | ---------------- | ------ | --------------------------- |
| 2100      | WireGuard Enable | R/W    | Enable or disable WireGuard |
| 2101-2133 | Private Key      | R/W    | Local WireGuard private key |
| 2134-2166 | Peer Public Key  | R/W    | Remote peer public key      |
| 2167-2199 | Pre-Shared Key   | R/W    | Optional pre-shared key     |
| 2200      | Local IP Octet 1 | R/W    | Local VPN address           |
| 2201      | Local IP Octet 2 | R/W    | Local VPN address           |
| 2202      | Local IP Octet 3 | R/W    | Local VPN address           |
| 2203      | Local IP Octet 4 | R/W    | Local VPN address           |
| 2204      | WireGuard Port   | R/W    | UDP port                    |
| 2205      | Peer IP Octet 1  | R/W    | Peer endpoint address       |
| 2206      | Peer IP Octet 2  | R/W    | Peer endpoint address       |
| 2207      | Peer IP Octet 3  | R/W    | Peer endpoint address       |
| 2208      | Peer IP Octet 4  | R/W    | Peer endpoint address       |
| 2209-2250 | Reserved         | N/A    | Reserved for future use     |

---

## WireGuard Enable

### Register 2100

Controls whether the WireGuard interface is enabled.

| Value | Description |
| ----- | ----------- |
| 0     | Disabled    |
| 1     | Enabled     |

Example:

```text
2100 = 1
```

WireGuard is enabled.

---

## Key Storage Format

The following register blocks use identical encoding:

* Private Key (2101-2133)
* Peer Public Key (2134-2166)
* Pre-Shared Key (2167-2199)

Each Modbus register stores **two ASCII characters**.

### Register Encoding

```text
High Byte = Character 1
Low Byte  = Character 2
```

Example:

```text
Characters: A B

'A' = 0x41
'B' = 0x42

Register Value = 0x4142
```

Reading register value:

```text
0x4142
```

Results in:

```text
Character 1 = 'A'
Character 2 = 'B'
```

### Storage Capacity

Each key block contains:

```text
33 Registers × 2 Bytes = 66 Bytes
```

This capacity is sufficient for standard WireGuard Base64 encoded keys.

---

## Private Key

### Registers 2101 - 2133

Stores the local WireGuard private key.

| Property | Value                             |
| -------- | --------------------------------- |
| Access   | R/W                               |
| Capacity | 66 Bytes                          |
| Encoding | Two ASCII characters per register |

---

## Peer Public Key

### Registers 2134 - 2166

Stores the remote WireGuard peer public key.

| Property | Value                             |
| -------- | --------------------------------- |
| Access   | R/W                               |
| Capacity | 66 Bytes                          |
| Encoding | Two ASCII characters per register |

---

## Pre-Shared Key

### Registers 2167 - 2199

Stores the optional WireGuard pre-shared key.

| Property | Value                             |
| -------- | --------------------------------- |
| Access   | R/W                               |
| Capacity | 66 Bytes                          |
| Encoding | Two ASCII characters per register |

---

## Local VPN Address

### Registers 2200 - 2203

The VPN address is configured using four individual octets.

Example:

```text
10.0.0.2
```

| Register | Value |
| -------- | ----- |
| 2200     | 10    |
| 2201     | 0     |
| 2202     | 0     |
| 2203     | 2     |

---

## WireGuard Port

### Register 2204

Specifies the UDP port used by WireGuard.

Example:

```text
2204 = 51820
```

Supported Range:

```text
1 - 65535
```

The same port is used for:

* Local listening
* Peer communication

---

## Peer Endpoint Address

### Registers 2205 - 2208

Specifies the remote peer endpoint IP address.

Example:

```text
203.0.113.10
```

| Register | Value |
| -------- | ----- |
| 2205     | 203   |
| 2206     | 0     |
| 2207     | 113   |
| 2208     | 10    |

---

## Fixed Parameters

The following WireGuard parameters are fixed within firmware and are not configurable through Modbus.

### VPN Netmask

```text
255.255.255.0
```

### Persistent Keepalive

```text
25 Seconds
```

### Ping Address

```text
10.0.0.1
```

---

## Configuration Persistence

After a successful write to any WireGuard register, the controller automatically saves the updated configuration to non-volatile storage.

No additional save command is required.

---

## Multiple Register Write Support

WireGuard key blocks support Modbus Function Code 16 (Write Multiple Registers).

Example:

```text
Start Address : 2101
Quantity      : 33 Registers
```

This is the recommended method for writing:

* Private Key
* Peer Public Key
* Pre-Shared Key

The firmware automatically decodes the register values and stores the resulting key data.

---

## Example Configuration

### Local Device

```text
WireGuard Enable : 1
Local VPN IP     : 10.0.0.2
Port             : 51820
Peer Endpoint    : 203.0.113.10
```

### Register Values

| Register | Value |
| -------- | ----- |
| 2100     | 1     |
| 2200     | 10    |
| 2201     | 0     |
| 2202     | 0     |
| 2203     | 2     |
| 2204     | 51820 |
| 2205     | 203   |
| 2206     | 0     |
| 2207     | 113   |
| 2208     | 10    |

---

## Future Expansion

Registers `2209 - 2250` are reserved for future WireGuard enhancements such as:

* Tunnel Status
* Connection State
* Last Handshake Time
* RX/TX Statistics
* Peer Monitoring
* Diagnostic Information
* Multi-Peer Support

Future firmware versions may utilize these registers while maintaining backward compatibility.
