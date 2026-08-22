# Automated CCTV sim checks. No browser. Exit 0 if all four cameras play.
#   powershell -ExecutionPolicy Bypass -File T3000\sidecar\test-sim.ps1
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

foreach ($exe in @("go2rtc.exe", "ffmpeg.exe")) {
  $p = Join-Path $here $exe
  if (Test-Path $p) { $sz = (Get-Item $p).Length; Pass $exe "$sz bytes" }
  else { Fail $exe "missing next to go2rtc in T3000\sidecar\" }
}

$clips = @("north", "east", "south", "west")
foreach ($c in $clips) {
  $mp4 = Join-Path $here "sim\$c.mp4"
  if (Test-Path $mp4) { Pass "clip $c.mp4" }
  else { Fail "clip $c.mp4" "missing" }
}

& (Join-Path $here "start-sim.ps1")
if ($LASTEXITCODE -and $LASTEXITCODE -ne 0) {
  Fail "start-sim.ps1" "exited $LASTEXITCODE"
}

$ready = $false
for ($i = 0; $i -lt 20; $i++) {
  $up = @()
  foreach ($port in 9291, 9292, 9293) {
    $c = Get-NetTCPConnection -LocalPort $port -State Listen -ErrorAction SilentlyContinue
    if ($c) { $up += $port }
  }
  if ($up.Count -eq 3) { $ready = $true; break }
  Start-Sleep -Milliseconds 400
}
if ($ready) { Pass "ports" "9291 9292 9293 listening" }
else { Fail "ports" "go2rtc did not bind 9291/9292/9293" }

$streams = $null
try {
  $streams = (Invoke-WebRequest -UseBasicParsing http://127.0.0.1:9291/api/streams -TimeoutSec 5).Content | ConvertFrom-Json
  Pass "api" "http://127.0.0.1:9291/api/streams"
} catch {
  Fail "api" $_.Exception.Message
}

$names = @("sim-north", "sim-east", "sim-south", "sim-west")
if ($streams) {
  foreach ($n in $names) {
    if ($streams.PSObject.Properties.Name -contains $n) { Pass "listed $n" }
    else { Fail "listed $n" "not in /api/streams" }
  }
}

function Test-SimVideo([string]$name) {
  $u = "http://127.0.0.1:9291/api/stream.mp4?src=$name"
  $req = [System.Net.HttpWebRequest]::Create($u)
  $req.Timeout = 10000
  $req.ReadWriteTimeout = 10000
  $req.AddRange(0, 65535)
  try {
    $resp = $req.GetResponse()
    $ct = [string]$resp.ContentType
    $s = $resp.GetResponseStream()
    $buf = New-Object byte[] 4096
    $n = $s.Read($buf, 0, 4096)
    $resp.Close()
    $ascii = [System.Text.Encoding]::ASCII.GetString($buf, 0, [Math]::Min($n, 32))
    $isMp4 = $ct -like "video/mp4*" -or $ascii.Contains("ftyp")
    $isFfmpegMiss = $ascii -match "ffmpeg" -or $ascii -match "executable file not found"
    if ($isFfmpegMiss) { return "ffmpeg missing: $ascii" }
    if ($n -lt 32) { return "too few bytes ($n) type=$ct" }
    if (-not $isMp4) { return "not video (type=$ct bytes=$n)" }
    return $null
  } catch {
    $msg = $_.Exception.Message
    if ($msg -match "500") { return "HTTP 500 (often missing ffmpeg)" }
    return $msg
  }
}

foreach ($n in $names) {
  $err = Test-SimVideo $n
  if ($err) { Fail $n $err }
  else { Pass $n "video/mp4 from $n" }
}

if ($fail -gt 0) {
  Write-Host "RESULT  FAIL  $fail check(s)"
  exit 1
}
Write-Host "RESULT  PASS  all sim cameras"
exit 0

