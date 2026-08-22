# Start four simulated construction-site cameras for T3000 CCTV.
# HTTP  http://127.0.0.1:9291
# RTSP  rtsp://127.0.0.1:9292/sim-north|east|south|west
$ErrorActionPreference = "Stop"
$here = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $here

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

$config = Join-Path $here "go2rtc-sim.yaml"
Write-Host "Starting $go2rtc"
Write-Host "Config  $config"
Write-Host "HTTP    http://127.0.0.1:9291"
Write-Host "North   rtsp://127.0.0.1:9292/sim-north   (north fence, looking south)"
Write-Host "East    rtsp://127.0.0.1:9292/sim-east    (east fence, looking west)"
Write-Host "South   rtsp://127.0.0.1:9292/sim-south   (south gate, looking north)"
Write-Host "West    rtsp://127.0.0.1:9292/sim-west    (west fence, looking east)"
Start-Process -FilePath $go2rtc -ArgumentList @("-config", $config) -WorkingDirectory $here
