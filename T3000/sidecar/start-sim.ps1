# Start four simulated construction-site cameras for T3000 CCTV.
# HTTP  http://127.0.0.1:9291
# RTSP  rtsp://127.0.0.1:9292/sim-north|east|south|west
# DVR   http://127.0.0.1:9294/settings
$ErrorActionPreference = "Stop"
$here = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $here
$env:PATH = "$here;" + $env:PATH

function Test-ScriptRunning([string]$leaf) {
  $hits = Get-CimInstance Win32_Process -Filter "Name = 'powershell.exe' OR Name = 'pwsh.exe'" -ErrorAction SilentlyContinue |
    Where-Object { $_.CommandLine -and ($_.CommandLine -like "*$leaf*") }
  return [bool]$hits
}

function Start-HiddenScript([string]$leaf) {
  $script = Join-Path $here $leaf
  if (-not (Test-Path $script)) { Write-Host "missing $script"; return }
  if (Test-ScriptRunning $leaf) {
    Write-Host "$leaf already running"
    return
  }
  Start-Process -FilePath "powershell.exe" -ArgumentList @(
    "-NoProfile", "-ExecutionPolicy", "Bypass", "-WindowStyle", "Hidden", "-File", $script
  ) -WorkingDirectory $here -WindowStyle Hidden
  Write-Host "started $leaf"
}

$candidates = @(
  (Join-Path $here "go2rtc.exe"),
  (Join-Path $here "..\..\T3000 Output\release\sidecar\go2rtc.exe"),
  (Join-Path $here "..\..\T3000 Output\debug\sidecar\go2rtc.exe")
)
$go2rtc = $candidates | Where-Object { Test-Path $_ } | Select-Object -First 1
if (-not $go2rtc) {
  Write-Host "go2rtc.exe not found. From the repo root run:"
  Write-Host "  powershell -ExecutionPolicy Bypass -File tools\fetch-nvr-sidecars.ps1"
  exit 1
}
if (-not (Test-Path (Join-Path $here "ffmpeg.exe"))) {
  Write-Host "ffmpeg.exe not found next to go2rtc. Put ffmpeg.exe in $here"
  exit 1
}

$config = Join-Path $here "go2rtc-sim.yaml"
$go2rtcUp = $false
try {
  $probe = Invoke-WebRequest -Uri "http://127.0.0.1:9291/api" -UseBasicParsing -TimeoutSec 2
  $go2rtcUp = $probe.StatusCode -ge 200
} catch { $go2rtcUp = $false }

if ($go2rtcUp) {
  Write-Host "go2rtc already running on http://127.0.0.1:9291"
} else {
  Get-Process go2rtc -ErrorAction SilentlyContinue | Stop-Process -Force
  Start-Sleep -Milliseconds 400
  Write-Host "Starting $go2rtc"
  Write-Host "Config  $config"
  Write-Host "HTTP    http://127.0.0.1:9291"
  Write-Host "North   rtsp://127.0.0.1:9292/sim-north"
  Write-Host "East    rtsp://127.0.0.1:9292/sim-east"
  Write-Host "South   rtsp://127.0.0.1:9292/sim-south"
  Write-Host "West    rtsp://127.0.0.1:9292/sim-west"
  Start-Process -FilePath $go2rtc -ArgumentList @("-config", $config) -WorkingDirectory $here
}

New-Item -ItemType Directory -Force -Path "C:\Xdrive\T3000_DVR" | Out-Null
Start-HiddenScript "start-motion-record.ps1"
Write-Host "DVR API  http://127.0.0.1:9294/settings"
Write-Host "Events   http://127.0.0.1:9294/events.json"
Write-Host "DVR store C:\Xdrive\T3000_DVR"

# SADP probe API (inquiry only; does not launch SADPTool.exe)
$sadp = Join-Path $here "sadp-discover.py"
if (Test-Path $sadp) {
  Start-Process -FilePath "python" -ArgumentList @($sadp, "--serve") -WorkingDirectory $here -WindowStyle Hidden
  Write-Host "SADP    http://127.0.0.1:9295/discover"
}
