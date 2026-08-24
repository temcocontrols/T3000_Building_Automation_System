# Arm motion-triggered recording for the T3000 CCTV sidecar.
# Records ONLY when ffmpeg frame-diff sees motion. Idle = no writer, no file growth.
#   powershell -ExecutionPolicy Bypass -File T3000\sidecar\start-motion-record.ps1
param(
  [switch]$IdleProof,
  [switch]$DemoSim,
  [switch]$Once,
  [double]$Seconds = 0
)
$ErrorActionPreference = "Stop"
$here = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $here
$env:PATH = "$here;" + $env:PATH

$py = Get-Command python -ErrorAction SilentlyContinue
if (-not $py) { $py = Get-Command py -ErrorAction SilentlyContinue }
if (-not $py) {
  Write-Host "python not found. Install Python 3 or add it to PATH."
  exit 1
}

$script = Join-Path $here "motion-record.py"
if (-not (Test-Path $script)) {
  Write-Host "missing $script"
  exit 1
}

$outDir = Join-Path $here "recordings"
New-Item -ItemType Directory -Force -Path $outDir | Out-Null

$argList = @($script, "--out", $outDir)
if ($IdleProof) { $argList += "--idle-proof" }
if ($DemoSim) { $argList += "--demo-sim" }
if ($Once) { $argList += "--once" }
if ($Seconds -gt 0) { $argList += @("--seconds", "$Seconds") }

$pidFile = Join-Path $outDir "motion-record.pid"
if (-not $IdleProof -and -not $DemoSim -and $Seconds -le 0) {
  if (Test-Path $pidFile) {
    $old = 0
    [void][int]::TryParse((Get-Content $pidFile -ErrorAction SilentlyContinue), [ref]$old)
    if ($old -gt 0) {
      $alive = Get-Process -Id $old -ErrorAction SilentlyContinue
      if ($alive) {
        Write-Host "already armed  pid=$old  clips $outDir"
        Write-Host "events  http://127.0.0.1:9294/events.json"
        exit 0
      }
    }
  }
}

Write-Host "Arming motion record (no 24/7 writers)"
Write-Host "Clips   $outDir"
Write-Host "Events  http://127.0.0.1:9294/events.json"
$p = Start-Process -FilePath $py.Source -ArgumentList $argList -WorkingDirectory $here -WindowStyle Hidden -PassThru
Set-Content -Path $pidFile -Value $p.Id -Encoding ascii
Write-Host "armed   pid=$($p.Id)"
if ($IdleProof -or $DemoSim -or $Seconds -gt 0) {
  Wait-Process -Id $p.Id
  if ($null -ne $p.ExitCode) { exit $p.ExitCode }
}
