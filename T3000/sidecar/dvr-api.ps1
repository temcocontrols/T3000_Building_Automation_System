# Launch the Python DVR settings API on 127.0.0.1:9294
$ErrorActionPreference = "Stop"
$here = Split-Path -Parent $MyInvocation.MyCommand.Path
$py = Join-Path $here "dvr-api.py"
$mutex = New-Object System.Threading.Mutex($false, "Global\T3000DvrApi")
if (-not $mutex.WaitOne(0)) {
  Write-Host "dvr-api already running"
  exit 0
}
$python = "C:\Python314\python.exe"
if (-not (Test-Path $python)) { $python = "python" }
Write-Host "starting $py"
& $python $py
