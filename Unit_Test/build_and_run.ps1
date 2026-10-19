$ErrorActionPreference = "Stop"

$Here = Split-Path -Parent $MyInvocation.MyCommand.Path
$T3000 = (Resolve-Path (Join-Path $Here "..")).Path
$Boot = $null
if ($env:BOOT_ROOT -and (Test-Path $env:BOOT_ROOT)) {
    $Boot = (Resolve-Path $env:BOOT_ROOT).Path
} else {
    $sibling = Join-Path $Here "..\..\..\Bootloader\T3_Bootloader"
    if (Test-Path $sibling) { $Boot = (Resolve-Path $sibling).Path }
}
$haveBoot = 0
if ($Boot) { $haveBoot = 1 }
$Build = Join-Path $Here "build"
New-Item -ItemType Directory -Force -Path $Build | Out-Null

function Find-Gcc() {
    $cmd = Get-Command gcc -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }
    foreach ($candidate in @(
        "$env:TEMP\w64devkit\w64devkit\bin\gcc.exe",
        "C:\Users\Lenoovo\AppData\Local\Temp\w64devkit\w64devkit\bin\gcc.exe",
        "C:\msys64\ucrt64\bin\gcc.exe",
        "C:\msys64\mingw64\bin\gcc.exe",
        "C:\mingw\bin\gcc.exe"
    )) {
        if (Test-Path $candidate) { return $candidate }
    }
    throw "gcc was not found. The Hub unit tests use $env:TEMP\w64devkit\w64devkit\bin\gcc.exe."
}

function Get-DefineValue([string]$File, [string]$Name) {
    $match = Select-String -Path $File -Pattern "^\s*#define\s+$Name\s+(0x[0-9A-Fa-f]+|\d+)" | Select-Object -First 1
    if (-not $match) { throw "Missing #$Name in $File" }
    return $match.Matches[0].Groups[1].Value
}

$Product = Join-Path $T3000 "T3000\ProductModel.h"
$Struct = Join-Path $T3000 "ISP\Global_Struct.h"
$GlobalCpp = Join-Path $T3000 "ISP\global_function.cpp"

$names = @(
    @{ File = $Product; Name = "PM_ESP32_T3_SERIES" },
    @{ File = $Product; Name = "PM_TSTAT10" },
    @{ File = $Product; Name = "PM_TSTAT8" },
    @{ File = $Product; Name = "PM_TSTAT6" },
    @{ File = $Product; Name = "PM_MINIPANEL" },
    @{ File = $Product; Name = "PM_MINIPANEL_ARM" },
    @{ File = $Product; Name = "STM32_CO2_NET" },
    @{ File = $Product; Name = "STM32_PRESSURE_RS485" },
    @{ File = $Product; Name = "STM32_PM25" },
    @{ File = $Struct; Name = "ISP_RESUME_BOOTLOADER_ALL" },
    @{ File = $Struct; Name = "ISP_RESUME_BOOTLOADER_LIMITED" },
    @{ File = $Struct; Name = "ISP_REG_PRODUCT_MODEL" },
    @{ File = $Struct; Name = "ISP_REG_BOOT_REV_A" },
    @{ File = $Struct; Name = "ISP_REG_BOOT_REV_B" },
    @{ File = $Struct; Name = "ISP_REG_UPDATE_STATUS" },
    @{ File = $Struct; Name = "ISP_REG_MINI_TYPE" },
    @{ File = $Struct; Name = "ISP_REG_RESUME_COUNT" },
    @{ File = $Struct; Name = "ISP_REG_RESUME_BLOCK_LEN" },
    @{ File = $Struct; Name = "ISP_REG_MD5_START" },
    @{ File = $Struct; Name = "ISP_REG_MD5_COUNT" },
    @{ File = $Struct; Name = "ISP_REG_FW_SIZE_HI" },
    @{ File = $Struct; Name = "ISP_REG_FW_SIZE_LO" },
    @{ File = $Struct; Name = "ISP_REG_SERIAL_SAVED_COUNT" },
    @{ File = $Struct; Name = "ISP_REG_SERIAL_MD5_WRITE" },
    @{ File = $Struct; Name = "ISP_MINI_TSTAT10" },
    @{ File = $Struct; Name = "ISP_MINI_HUB" },
    @{ File = $Struct; Name = "ISP_MINI_TSTAT11" },
    @{ File = $Struct; Name = "ISP_STATUS_IN_PROGRESS_1F" },
    @{ File = $Struct; Name = "ISP_STATUS_IN_PROGRESS_40" },
    @{ File = $Struct; Name = "ISP_CMD_CLEAR_SAVED_UPDATE" },
    @{ File = $Struct; Name = "ISP_RESUME_PACKET_BYTES" }
)

$header = New-Object System.Collections.Generic.List[string]
$header.Add("#ifndef ISP_CONSTANTS_H")
$header.Add("#define ISP_CONSTANTS_H")
$header.Add("/* Generated from production headers. Do not edit. */")
foreach ($item in $names) {
    $value = Get-DefineValue $item.File $item.Name
    $header.Add("#define $($item.Name) $value")
}
if ($haveBoot -eq 1) {
    $otaSize = Get-DefineValue (Join-Path $Boot "main\app_config.h") "OTA_PACKET_SIZE"
    $otaChunk = Get-DefineValue (Join-Path $Boot "main\app_config.h") "OTA_FLASH_CHUNK_SIZE"
} else {
    $otaSize = "128"
    $otaChunk = "128"
}
$header.Add("#define OTA_PACKET_SIZE $otaSize")
$header.Add("#define OTA_FLASH_CHUNK_SIZE $otaChunk")
$header.Add("#define HAVE_BOOTLOADER $haveBoot")
$header.Add("#endif")
[System.IO.File]::WriteAllLines((Join-Path $Build "isp_constants.h"), $header)

$t3000Fwd = $T3000.Replace("\", "/")
$bootFwd = ""
if ($Boot) { $bootFwd = $Boot.Replace("\", "/") }
$paths = @(
    "#ifndef TEST_PATHS_H",
    "#define TEST_PATHS_H",
    "#define T3000_ROOT `"$t3000Fwd`"",
    "#define BOOT_ROOT `"$bootFwd`"",
    "#endif"
)
[System.IO.File]::WriteAllLines((Join-Path $Build "test_paths.h"), $paths)

$text = [System.IO.File]::ReadAllText($GlobalCpp)
$sig = "bool isp_device_supports_resume"
$start = $text.IndexOf($sig)
if ($start -lt 0) { throw "isp_device_supports_resume was not found" }
$brace = $text.IndexOf("{", $start)
if ($brace -lt 0) { throw "function body was not found" }
$depth = 0
$end = -1
for ($i = $brace; $i -lt $text.Length; $i++) {
    if ($text[$i] -eq "{") { $depth++ }
    elseif ($text[$i] -eq "}") {
        $depth--
        if ($depth -eq 0) { $end = $i; break }
    }
}
if ($end -lt 0) { throw "function body did not close" }
$func = $text.Substring($start, $end - $start + 1)
$unit = @"
#include <stdbool.h>
#include "isp_constants.h"

$func

int isp_resume_supported(unsigned product_id, unsigned bootloader_version, unsigned mini_type)
{
    return isp_device_supports_resume((unsigned short)product_id,
                                      (unsigned short)bootloader_version,
                                      (unsigned short)mini_type) ? 1 : 0;
}
"@
[System.IO.File]::WriteAllText((Join-Path $Build "isp_device_supports_resume.c"), $unit)

$Python = $null
foreach ($candidate in @("python", "py", "python3")) {
    if (Get-Command $candidate -ErrorAction SilentlyContinue) { $Python = $candidate; break }
}
if (-not $Python) { throw "python was not found" }
$extractor = Join-Path $Build "extract_used_apis.py"
[System.IO.File]::WriteAllText($extractor, @'
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


def product_defines(path):
    lines = []
    pattern = re.compile(
        r"^\s*#define\s+([A-Za-z_][A-Za-z0-9_]*)\s+(0x[0-9A-Fa-f]+|\d+)\b"
    )
    for raw in open(path, encoding="utf-8", errors="replace"):
        match = pattern.match(raw)
        if match:
            lines.append("#ifndef %s" % match.group(1))
            lines.append("#define %s %s" % (match.group(1), match.group(2)))
            lines.append("#endif")
    return "\n".join(lines)


def main():
    source, products, dest = sys.argv[1], sys.argv[2], sys.argv[3]
    text = open(source, encoding="utf-8", errors="replace").read()
    bodies = "\n\n".join(extract_function(text, name) for name in FUNCTIONS)
    open(dest, "w", encoding="utf-8", newline="\n").write(PREAMBLE + product_defines(products) + "\n\n" + bodies + WRAPPERS)


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
'@)
& $Python $extractor $GlobalCpp (Join-Path $T3000 "T3000\ProductModel.h") (Join-Path $Build "isp_used_apis.cpp")
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

$Gcc = Find-Gcc
$Gxx = Join-Path (Split-Path $Gcc) "g++.exe"
if (-not (Test-Path $Gxx)) { throw "g++ was not found next to $Gcc" }

$cflags = @("-std=c11", "-Wall", "-Wextra", "-I$Here", "-I$Build")
$sources = @(
    (Join-Path $Here "harness.c"),
    (Join-Path $Here "firmware_flow.c"),
    (Join-Path $Here "test_resume_support.c"),
    (Join-Path $Here "test_host_flows.c"),
    (Join-Path $Here "test_bootloader_flow.c"),
    (Join-Path $Here "test_source_lock.c"),
    (Join-Path $Here "test_used_apis.c"),
    (Join-Path $Here "test_main.c"),
    (Join-Path $Build "isp_device_supports_resume.c")
)
$objects = @()
foreach ($src in $sources) {
    $obj = Join-Path $Build ([System.IO.Path]::GetFileNameWithoutExtension($src) + ".o")
    & $Gcc @cflags -c $src -o $obj
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
    $objects += $obj
}

$usedObj = Join-Path $Build "isp_used_apis.o"
& $Gxx @("-std=c++17", "-Wall", "-Wextra", "-Wno-unused-variable", "-Wno-misleading-indentation", "-Wno-type-limits", "-fsigned-char", "-I$Here", "-I$Build") -c (Join-Path $Build "isp_used_apis.cpp") -o $usedObj
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
$objects += $usedObj

$exe = Join-Path $Build "run_firmware_update_tests.exe"
& $Gxx @objects -o $exe
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

& $exe
exit $LASTEXITCODE
