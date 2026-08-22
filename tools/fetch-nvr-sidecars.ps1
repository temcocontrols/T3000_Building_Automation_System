# Download MIT-licensed NVR sidecars for T3000 (not committed to git).
# Usage:
#   powershell -ExecutionPolicy Bypass -File tools\fetch-nvr-sidecars.ps1
#   powershell -ExecutionPolicy Bypass -File tools\fetch-nvr-sidecars.ps1 -OutDir "T3000 Output\release\sidecar"

[CmdletBinding()]
param(
    [string]$OutDir = "",
    [string]$Go2rtcVersion = "1.9.14",
    [string]$MediamtxVersion = "1.20.1"
)

$ErrorActionPreference = "Stop"

if (-not $OutDir) {
    $repoRoot = Split-Path -Parent $PSScriptRoot
    $OutDir = Join-Path $repoRoot "T3000\sidecar"
}

New-Item -ItemType Directory -Force -Path $OutDir | Out-Null
$tmp = Join-Path $env:TEMP ("t3000-nvr-" + [guid]::NewGuid().ToString("N"))
New-Item -ItemType Directory -Force -Path $tmp | Out-Null

try {
    $go2rtcZip = Join-Path $tmp "go2rtc_win64.zip"
    $go2rtcUrl = "https://github.com/AlexxIT/go2rtc/releases/download/v$Go2rtcVersion/go2rtc_win64.zip"
    Write-Host "Downloading $go2rtcUrl"
    Invoke-WebRequest -Uri $go2rtcUrl -OutFile $go2rtcZip -UseBasicParsing
    Expand-Archive -Path $go2rtcZip -DestinationPath $tmp -Force
    $go2rtcExe = Get-ChildItem $tmp -Recurse -Filter "go2rtc.exe" | Select-Object -First 1
    if (-not $go2rtcExe) { throw "go2rtc.exe not found in zip" }
    Copy-Item $go2rtcExe.FullName (Join-Path $OutDir "go2rtc.exe") -Force
    Write-Host "Wrote $(Join-Path $OutDir 'go2rtc.exe')"

    $mtxZip = Join-Path $tmp "mediamtx.zip"
    $mtxUrl = "https://github.com/bluenviron/mediamtx/releases/download/v$MediamtxVersion/mediamtx_v${MediamtxVersion}_windows_amd64.zip"
    Write-Host "Downloading $mtxUrl"
    Invoke-WebRequest -Uri $mtxUrl -OutFile $mtxZip -UseBasicParsing
    Expand-Archive -Path $mtxZip -DestinationPath (Join-Path $tmp "mtx") -Force
    $mtxExe = Get-ChildItem (Join-Path $tmp "mtx") -Recurse -Filter "mediamtx.exe" | Select-Object -First 1
    if (-not $mtxExe) { throw "mediamtx.exe not found in zip" }
    Copy-Item $mtxExe.FullName (Join-Path $OutDir "mediamtx.exe") -Force
    Write-Host "Wrote $(Join-Path $OutDir 'mediamtx.exe')"
}
finally {
    Remove-Item -Recurse -Force $tmp -ErrorAction SilentlyContinue
}

Write-Host "Sidecars are MIT-licensed separate processes. See T3000\sidecar\NOTICE.txt and Documentation\nvr.md."
