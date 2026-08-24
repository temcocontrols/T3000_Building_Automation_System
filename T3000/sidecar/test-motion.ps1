# Proof: idle scene does not grow clips; sim walking can produce one clip.
#   powershell -ExecutionPolicy Bypass -File T3000\sidecar\test-motion.ps1
param()
$ErrorActionPreference = "Stop"
$here = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $here
$env:PATH = "$here;" + $env:PATH

$fail = 0
function Pass([string]$name, [string]$detail = "") {
  Write-Host "PASS  $name$(if ($detail) { "  $detail" })"
}
function Fail([string]$name, [string]$detail) {
  Write-Host "FAIL  $name  $detail"
  $script:fail++
}

$py = Get-Command python -ErrorAction SilentlyContinue
if (-not $py) { Fail "python" "missing"; if ($fail) { exit 1 } }
$script = Join-Path $here "motion-record.py"
if (-not (Test-Path $script)) { Fail "motion-record.py" "missing" }
if (-not (Test-Path (Join-Path $here "ffmpeg.exe"))) { Fail "ffmpeg.exe" "missing next to sidecar" }

$outDir = Join-Path $here "recordings"
New-Item -ItemType Directory -Force -Path $outDir | Out-Null

function ClipStats {
  $files = @(Get-ChildItem $outDir -Filter *.mp4 -ErrorAction SilentlyContinue)
  $bytes = 0
  foreach ($f in $files) { $bytes += $f.Length }
  return @{ Count = $files.Count; Bytes = $bytes }
}

$before = ClipStats

# 1) Detector seeing no change must not write clips.
& python $script --idle-proof --seconds 12 --out $outDir --no-http --streams cam-64
if ($LASTEXITCODE -and $LASTEXITCODE -ne 0) {
  # streams may be sim-only; retry without pinning a name
  & python $script --idle-proof --seconds 8 --out $outDir --no-http --demo-sim
}
$afterIdle = ClipStats
if ($afterIdle.Count -eq $before.Count -and $afterIdle.Bytes -eq $before.Bytes) {
  Pass "idle" "clip count $($afterIdle.Count) bytes $($afterIdle.Bytes) unchanged"
} else {
  Fail "idle" "grew from $($before.Count)/$($before.Bytes) to $($afterIdle.Count)/$($afterIdle.Bytes)"
}

# 2) Positive: people walking in official sim clips.
& python $script --demo-sim --once --seconds 20 --clip 6 --out $outDir --no-http --streams sim-north
$afterPos = ClipStats
if ($afterPos.Count -gt $afterIdle.Count) {
  Pass "motion-clip" "clips $($afterIdle.Count) -> $($afterPos.Count) bytes $($afterPos.Bytes)"
} else {
  Fail "motion-clip" "no new clip from sim walking"
}

if ($fail -gt 0) {
  Write-Host "RESULT  FAIL  $fail check(s)"
  exit 1
}
Write-Host "RESULT  PASS  idle does not grow files; sim motion writes a clip"
exit 0
