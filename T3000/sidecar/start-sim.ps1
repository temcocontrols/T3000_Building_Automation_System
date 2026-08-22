# Start four simulated construction-site cameras for T3000 CCTV.
# HTTP  http://127.0.0.1:9291
# RTSP  rtsp://127.0.0.1:9292/sim-north|east|south|west
$ErrorActionPreference = "Stop"
$here = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $here
$env:PATH = "$here;" + $env:PATH

Get-Process go2rtc -ErrorAction SilentlyContinue | Stop-Process -Force
Start-Sleep -Milliseconds 400

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
Write-Host "Starting $go2rtc"
Write-Host "Config  $config"
Write-Host "HTTP    http://127.0.0.1:9291"
Write-Host "North   rtsp://127.0.0.1:9292/sim-north"
Write-Host "East    rtsp://127.0.0.1:9292/sim-east"
Write-Host "South   rtsp://127.0.0.1:9292/sim-south"
Write-Host "West    rtsp://127.0.0.1:9292/sim-west"
Start-Process -FilePath $go2rtc -ArgumentList @("-config", $config) -WorkingDirectory $here
