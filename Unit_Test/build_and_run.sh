#!/usr/bin/env bash
set -euo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
T3000="$(cd "$HERE/.." && pwd)"
BUILD="$HERE/build"
mkdir -p "$BUILD"

if [[ -n "${BOOT_ROOT:-}" && -d "$BOOT_ROOT" ]]; then
  BOOT="$(cd "$BOOT_ROOT" && pwd)"
elif [[ -d "$HERE/../../../Bootloader/T3_Bootloader" ]]; then
  BOOT="$(cd "$HERE/../../../Bootloader/T3_Bootloader" && pwd)"
else
  BOOT=""
fi

define_value() {
  local file="$1"
  local name="$2"
  local value
  # Keep only the number. Header comments must not become part of the macro.
  value="$(python3 - "$file" "$name" <<'PY'
import re
import sys
file_name, macro = sys.argv[1], sys.argv[2]
text = open(file_name, "rb").read().decode("latin1")
match = re.search(
    r"(?m)^\s*#define\s+%s\s+(0x[0-9A-Fa-f]+|\d+)\b" % re.escape(macro),
    text,
)
if not match:
    sys.exit(1)
sys.stdout.write(match.group(1))
PY
)" || {
    echo "Missing #define $name in $file" >&2
    return 1
  }
  printf '%s\n' "$value"
}

STRUCT="$T3000/ISP/Global_Struct.h"
GLOBAL_CPP="$T3000/ISP/global_function.cpp"

{
  echo "#ifndef ISP_CONSTANTS_H"
  echo "#define ISP_CONSTANTS_H"
  echo "/* Product ids used by the ISP tool. ProductModel.h is not read. */"
  cat <<'EOF'
#define PM_TSTAT5B 1
#define PM_TSTAT5A 2
#define PM_TSTAT5B2 3
#define PM_TSTAT5C 4
#define PM_TSTAT6 6
#define PM_TSTAT7 7
#define PM_TSTAT5i 8
#define PM_TSTAT8 9
#define PM_TSTAT10 10
#define PM_TSTAT5D 12
#define PM_AirQuality 13
#define PM_HUMTEMPSENSOR 14
#define PM_TSTATRUNAR 15
#define PM_TSTAT5E 16
#define PM_TSTAT5F 17
#define PM_TSTAT5G 18
#define PM_TSTAT5H 19
#define PM_T38I13O 20
#define PM_T3IOA 21
#define PM_T332AI 22
#define PM_T38AI16O 23
#define PM_ZIGBEE 24
#define PM_FLEXDRIVER 25
#define PM_T3PT10 26
#define PM_T3PERFORMANCE 27
#define PM_T34AO 28
#define PM_T36CT 29
#define PM_SOLAR 30
#define PM_FWMTRANSDUCER 31
#define PM_CO2_NET 32
#define PM_CO2_RS485 33
#define PM_CO2_NODE 34
#define PM_MINIPANEL 35
#define PM_PRESSURE 40
#define PM_PM5E 41
#define PM_HUM_R 42
#define PM_T322AI 43
#define PM_T38AI8AO6DO 44
#define PM_PRESSURE_SENSOR 45
#define PM_T3PT12 46
#define PM_T322AIVG 47
#define PM_T38IOVG 48
#define PM_T3PTVG 49
#define PM_CM5 50
#define PM_PM5E_ARM 51
#define STM32_PM25 52
#define PM_TSTAT9 59
#define PM_PWMETER 73
#define PM_MINIPANEL_ARM 74
#define PM_WEATHER_STATION 75
#define PM_ESP32_T3_SERIES 88
#define PM_TSTAT8_WIFI 91
#define PM_TSTAT8_OCC 92
#define PM_TSTAT7_ARM 93
#define PM_TSTAT8_220V 94
#define PM_T36CTA 95
#define PM_NC 100
#define PWM_TRANSDUCER 104
#define PM_LightingController 120
#define PM_BTU_METER 121
#define STM32_CO2_NET 210
#define STM32_CO2_RS485 211
#define STM32_HUM_NET 212
#define STM32_HUM_RS485 213
#define STM32_PRESSURE_NET 214
#define STM32_PRESSURE_RS485 215
#define STM32_CO2_NODE 216
EOF
  for name in \
    ISP_RESUME_BOOTLOADER_ALL ISP_RESUME_BOOTLOADER_LIMITED \
    ISP_REG_PRODUCT_MODEL ISP_REG_BOOT_REV_A ISP_REG_BOOT_REV_B \
    ISP_REG_UPDATE_STATUS ISP_REG_MINI_TYPE ISP_REG_RESUME_COUNT \
    ISP_REG_RESUME_BLOCK_LEN ISP_REG_MD5_START ISP_REG_MD5_COUNT \
    ISP_REG_FW_SIZE_HI ISP_REG_FW_SIZE_LO ISP_REG_SERIAL_SAVED_COUNT \
    ISP_REG_SERIAL_MD5_WRITE ISP_MINI_TSTAT10 ISP_MINI_HUB ISP_MINI_TSTAT11 \
    ISP_STATUS_IN_PROGRESS_1F ISP_STATUS_IN_PROGRESS_40 \
    ISP_CMD_CLEAR_SAVED_UPDATE ISP_RESUME_PACKET_BYTES
  do
    value="$(define_value "$STRUCT" "$name")" || exit 1
    echo "#define $name $value"
  done
  if [[ -n "$BOOT" ]]; then
    echo "#define OTA_PACKET_SIZE $(define_value "$BOOT/main/app_config.h" OTA_PACKET_SIZE)"
    echo "#define OTA_FLASH_CHUNK_SIZE $(define_value "$BOOT/main/app_config.h" OTA_FLASH_CHUNK_SIZE)"
    echo "#define HAVE_BOOTLOADER 1"
  else
    echo "#define OTA_PACKET_SIZE 128"
    echo "#define OTA_FLASH_CHUNK_SIZE 128"
    echo "#define HAVE_BOOTLOADER 0"
  fi
  echo "#endif"
} > "$BUILD/isp_constants.h"

{
  echo "#ifndef TEST_PATHS_H"
  echo "#define TEST_PATHS_H"
  echo "#define T3000_ROOT \"$T3000\""
  echo "#define BOOT_ROOT \"$BOOT\""
  echo "#endif"
} > "$BUILD/test_paths.h"

python3 - "$GLOBAL_CPP" "$BUILD/isp_device_supports_resume.c" <<'PY'
import sys
src, dest = sys.argv[1], sys.argv[2]
text = open(src, encoding="utf-8", errors="replace").read()
sig = "bool isp_device_supports_resume"
start = text.find(sig)
if start < 0:
    raise SystemExit("isp_device_supports_resume was not found")
brace = text.find("{", start)
if brace < 0:
    raise SystemExit("function body was not found")
depth = 0
end = -1
for i in range(brace, len(text)):
    if text[i] == "{":
        depth += 1
    elif text[i] == "}":
        depth -= 1
        if depth == 0:
            end = i
            break
if end < 0:
    raise SystemExit("function body did not close")
func = text[start:end + 1]
open(dest, "w", encoding="utf-8", newline="\n").write(
    '#include <stdbool.h>\n#include "isp_constants.h"\n\n'
    + func
    + "\n\nint isp_resume_supported(unsigned product_id, unsigned bootloader_version, unsigned mini_type)\n"
    + "{\n    return isp_device_supports_resume((unsigned short)product_id,\n"
    + "                                      (unsigned short)bootloader_version,\n"
    + "                                      (unsigned short)mini_type) ? 1 : 0;\n}\n"
)
PY

python3 - "$GLOBAL_CPP" "$BUILD/isp_used_apis.cpp" <<'PY'
"""Copy used ISP logic out of global_function.cpp for the host tests.

Production sources are not modified. Dialog, socket, and Modbus I/O functions
are left out because they need MFC and a device.
"""
import re
import sys


FUNCTIONS = [
    "turn_hex_str_to_ten_num",
    "turn_4_hex_char_to_unsigned_short",
    "turn_hex_char_to_int",
    "turn_hex_file_line_to_unsigned_char",
    "turn_int_to_unsigned_char",
    "turn_unsigned_short_to_hex_char",
    "turn_unsigned_to_char",
    "DoHEXCRC",
    "GetFirmwareUpdateName",
    "GetProductName",
    "check_bootloader_and_frimware",
]


def extract_function(text, name):
    needle = name + "("
    pos = 0
    while True:
        found = text.find(needle, pos)
        if found < 0:
            raise SystemExit("missing function " + name)
        line = text.rfind("\n", 0, found) + 1
        prefix = text[line:found]
        if prefix[:1].isspace() or ";" in prefix:
            pos = found + 1
            continue
        brace = text.find("{", found)
        if brace < 0:
            raise SystemExit("missing body for " + name)
        depth = 0
        end = None
        for index in range(brace, len(text)):
            if text[index] == "{":
                depth += 1
            elif text[index] == "}":
                depth -= 1
                if depth == 0:
                    end = index
                    break
        if end is None:
            raise SystemExit("unclosed body for " + name)
        return text[line:end + 1]


def main():
    source, dest = sys.argv[1], sys.argv[2]
    text = open(source, encoding="utf-8", errors="replace").read()
    bodies = "\n\n".join(extract_function(text, name) for name in FUNCTIONS)
    open(dest, "w", encoding="utf-8", newline="\n").write(PREAMBLE + "\n" + bodies + WRAPPERS)


PREAMBLE = r'''#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include "isp_constants.h"

#define _T(x) x
#define MB_ICONWARNING 0
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
typedef int BOOL;
typedef unsigned char TS_UC;
#define isp_max(a, b) (((a) > (b)) ? (a) : (b))

static char* isp_strrev(char* s)
{
    size_t n = std::strlen(s);
    for (size_t i = 0; i < n / 2; i++) {
        char tmp = s[i];
        s[i] = s[n - 1 - i];
        s[n - 1 - i] = tmp;
    }
    return s;
}
#define _strrev isp_strrev

class CString {
public:
    CString() {}
    CString(const char* s) : text_(s ? s : "") {}
    CString(const wchar_t* s) { assign_wide(s); }
    CString& operator=(const char* s) { text_ = s ? s : ""; return *this; }
    CString& operator=(const wchar_t* s) { assign_wide(s); return *this; }
    void Format(const char* fmt, ...)
    {
        char buf[128];
        va_list ap;
        va_start(ap, fmt);
        std::vsnprintf(buf, sizeof(buf), fmt, ap);
        va_end(ap);
        text_ = buf;
    }
    const char* c_str() const { return text_.c_str(); }
private:
    void assign_wide(const wchar_t* s)
    {
        text_.clear();
        if (!s) return;
        for (; *s; ++s) text_.push_back(static_cast<char>(*s & 0x7f));
    }
    std::string text_;
};

static void AfxMessageBox(const char*, int) {}

bool auto_flash_mode = false;
int com_port_flash_status = 0;
int firmware_must_use_new_bootloader = 0;

char turn_unsigned_to_char(unsigned short source);

'''

WRAPPERS = r'''

extern "C" void isp_set_auto_flash(int on)
{
    auto_flash_mode = on ? true : false;
}

extern "C" int isp_hex_str(const char* text)
{
    char buf[8];
    std::strncpy(buf, text, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = 0;
    return turn_hex_str_to_ten_num(buf);
}

extern "C" int isp_hex4(char* text)
{
    return (int)turn_4_hex_char_to_unsigned_short(text);
}

extern "C" int isp_hex_char(char c)
{
    return turn_hex_char_to_int(c);
}

extern "C" int isp_hex_line(char* text)
{
    return turn_hex_file_line_to_unsigned_char(text) ? 1 : 0;
}

extern "C" void isp_nibbles_to_bytes(char* nibbles, int count, unsigned char* out)
{
    turn_int_to_unsigned_char(nibbles, count, out);
}

extern "C" void isp_ushort_to_hex(unsigned short value, char* out)
{
    turn_unsigned_short_to_hex_char(out, value);
}

extern "C" int isp_hex_crc(unsigned char* buf, int length)
{
    return DoHEXCRC(buf, length) ? 1 : 0;
}

extern "C" void isp_product_name(int id, char* out, int out_len)
{
    CString name = GetProductName(id);
    std::strncpy(out, name.c_str(), (size_t)out_len - 1);
    out[out_len - 1] = 0;
}

extern "C" void isp_firmware_name(int id, char* out, int out_len)
{
    CString name = GetFirmwareUpdateName(id);
    std::strncpy(out, name.c_str(), (size_t)out_len - 1);
    out[out_len - 1] = 0;
}

extern "C" int isp_check_bootloader(int npid, int comport, unsigned reg_11, unsigned reg_14,
                                    int* update_value, unsigned char app_version,
                                    int must_new, int com_status)
{
    firmware_must_use_new_bootloader = must_new;
    com_port_flash_status = com_status;
    int update = *update_value;
    int result = check_bootloader_and_frimware(npid, comport,
                                               (unsigned short)reg_11,
                                               (unsigned short)reg_14,
                                               update, app_version);
    *update_value = update;
    return result;
}
'''


if __name__ == "__main__":
    main()
PY

CC="${CC:-gcc}"
CXX="${CXX:-g++}"
CFLAGS=(-std=c11 -Wall -Wextra -I"$HERE" -I"$BUILD")
SOURCES=(
  "$HERE/harness.c"
  "$HERE/firmware_flow.c"
  "$HERE/test_resume_support.c"
  "$HERE/test_host_flows.c"
  "$HERE/test_bootloader_flow.c"
  "$HERE/test_source_lock.c"
  "$HERE/test_used_apis.c"
  "$HERE/test_main.c"
  "$BUILD/isp_device_supports_resume.c"
)
OBJECTS=()
for src in "${SOURCES[@]}"; do
  obj="$BUILD/$(basename "${src%.c}").o"
  "$CC" "${CFLAGS[@]}" -c "$src" -o "$obj"
  OBJECTS+=("$obj")
done

"$CXX" -std=c++17 -Wall -Wextra -Wno-unused-variable -Wno-misleading-indentation -Wno-type-limits -fsigned-char -I"$HERE" -I"$BUILD" -c "$BUILD/isp_used_apis.cpp" -o "$BUILD/isp_used_apis.o"
OBJECTS+=("$BUILD/isp_used_apis.o")

EXE="$BUILD/run_firmware_update_tests"
"$CXX" "${OBJECTS[@]}" -o "$EXE"
"$EXE"
