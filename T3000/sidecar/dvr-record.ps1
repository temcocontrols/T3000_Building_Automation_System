# T3000 motion-only DVR recorder.
# Reads dvr-settings.json. Idle = no ffmpeg writers, no file growth.
# Motion: jpeg frame-diff via go2rtc /api/frame.jpeg then record RTSP clip.
$ErrorActionPreference = "Continue"
$here = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $here
$env:PATH = "$here;" + $env:PATH

$mutex = New-Object System.Threading.Mutex($false, "Global\T3000DvrRecord")
if (-not $mutex.WaitOne(0)) {
  Write-Host "dvr-record already running"
  exit 0
}

$settingsPath = Join-Path $here "dvr-settings.json"
$ffmpeg = Join-Path $here "ffmpeg.exe"
$go2rtcHttp = "http://127.0.0.1:9291"
$go2rtcRtsp = "rtsp://127.0.0.1:9292"
$prevDir = Join-Path $env:TEMP "t3000-dvr-prev"
New-Item -ItemType Directory -Force -Path $prevDir | Out-Null

$script:writers = @{}
$script:lastSettingsWrite = $null

function Get-StoragePath($cfg) {
  $p = [string]$cfg.storage_path
  if (-not $p) { $p = "C:\Xdrive\T3000_DVR" }
  return $p
}

function Write-Log($storage, $msg) {
  $line = "[{0}] {1}" -f (Get-Date -Format "yyyy-MM-dd HH:mm:ss"), $msg
  try {
    New-Item -ItemType Directory -Force -Path $storage | Out-Null
    Add-Content -Path (Join-Path $storage "dvr-record.log") -Value $line -Encoding UTF8
  } catch {}
  Write-Host $line
}

function Write-Event($storage, $obj) {
  try {
    Add-Content -Path (Join-Path $storage "events.jsonl") -Value ($obj | ConvertTo-Json -Compress) -Encoding UTF8
  } catch {}
}

function Read-Cfg {
  if (-not (Test-Path $settingsPath)) { return $null }
  try { return Get-Content -Raw $settingsPath | ConvertFrom-Json } catch { return $null }
}

function Camera-Enabled($cfg, $name) {
  if (-not $cfg.cameras) { return $true }
  $prop = $cfg.cameras.PSObject.Properties[$name]
  if ($null -eq $prop) { return $true }
  return [int]$prop.Value -ne 0
}

function Get-StreamNames {
  try {
    $r = Invoke-WebRequest -Uri "$go2rtcHttp/api/streams" -UseBasicParsing -TimeoutSec 3
    $data = $r.Content | ConvertFrom-Json
    $names = @()
    $data.PSObject.Properties | ForEach-Object {
      if ($_.Name -notmatch '-main$') { $names += $_.Name }
    }
    return $names
  } catch {
    return @()
  }
}

function Get-Frame($name, $dest) {
  try {
    Invoke-WebRequest -Uri "$go2rtcHttp/api/frame.jpeg?src=$([uri]::EscapeDataString($name))" -OutFile $dest -UseBasicParsing -TimeoutSec 4
    return (Test-Path $dest) -and ((Get-Item $dest).Length -gt 200)
  } catch {
    return $false
  }
}

function Get-DiffScore($prev, $curr) {
  if (-not (Test-Path $prev) -or -not (Test-Path $curr)) { return 0 }
  $args = @(
    "-hide_banner", "-loglevel", "info",
    "-i", $prev, "-i", $curr,
    "-filter_complex", "blend=all_mode=difference,format=gray,scale=160:90,signalstats",
    "-f", "null", "-"
  )
  $out = & $ffmpeg @args 2>&1 | Out-String
  if ($out -match 'YAVG:\s*([0-9.]+)') {
    return [double]$Matches[1]
  }
  return 0
}

function Start-Clip($cfg, $cam, $score) {
  $storage = Get-StoragePath $cfg
  if ($script:writers.ContainsKey($cam) -and -not $script:writers[$cam].HasExited) {
    return
  }
  $post = [int]$cfg.postroll_sec
  if ($post -lt 1) { $post = 8 }
  $pre = [int]$cfg.preroll_sec
  $dir = Join-Path $storage $cam
  New-Item -ItemType Directory -Force -Path $dir | Out-Null
  $stamp = Get-Date -Format "yyyyMMdd-HHmmss"
  $outFile = Join-Path $dir "$stamp.mp4"
  $rtsp = "$go2rtcRtsp/$cam"
  $ffArgs = @(
    "-hide_banner", "-loglevel", "error",
    "-rtsp_transport", "tcp",
    "-i", $rtsp,
    "-t", "$post",
    "-an",
    "-c", "copy",
    "-movflags", "+faststart",
    "-y", $outFile
  )
  Write-Log $storage "RECORD $cam score=$score postroll=${post}s preroll_best_effort=${pre}s -> $outFile"
  $p = Start-Process -FilePath $ffmpeg -ArgumentList $ffArgs -WorkingDirectory $here -WindowStyle Hidden -PassThru
  $script:writers[$cam] = $p
  Write-Event $storage @{
    ts = (Get-Date).ToUniversalTime().ToString("o")
    cam = $cam
    event = "motion"
    score = $score
    file = $outFile
    postroll_sec = $post
    preroll_sec = $pre
  }
}

function Enforce-Retention($cfg) {
  $storage = Get-StoragePath $cfg
  if (-not (Test-Path $storage)) { return }
  $days = [int]$cfg.retention_days
  if ($days -lt 1) { $days = 14 }
  $cutoff = (Get-Date).AddDays(-$days)
  Get-ChildItem -Path $storage -Recurse -Filter *.mp4 -ErrorAction SilentlyContinue |
    Where-Object { $_.LastWriteTime -lt $cutoff } |
    ForEach-Object {
      Write-Log $storage "retention delete $($_.FullName)"
      Remove-Item -Force $_.FullName -ErrorAction SilentlyContinue
    }
  $maxGb = [double]$cfg.max_gb
  if ($maxGb -le 0) { $maxGb = 100 }
  $files = @(Get-ChildItem -Path $storage -Recurse -Filter *.mp4 -ErrorAction SilentlyContinue | Sort-Object LastWriteTime)
  $bytes = ($files | Measure-Object Length -Sum).Sum
  if (-not $bytes) { return }
  $limit = $maxGb * 1GB
  foreach ($f in $files) {
    if ($bytes -le $limit) { break }
    Write-Log $storage "max_gb delete $($f.FullName)"
    $bytes -= $f.Length
    Remove-Item -Force $f.FullName -ErrorAction SilentlyContinue
  }
}

function Sweep-Writers {
  $dead = @()
  foreach ($k in @($script:writers.Keys)) {
    if ($script:writers[$k] -and $script:writers[$k].HasExited) { $dead += $k }
  }
  foreach ($k in $dead) { $script:writers.Remove($k) }
}

$boot = Read-Cfg
$storage0 = if ($boot) { Get-StoragePath $boot } else { "C:\Xdrive\T3000_DVR" }
New-Item -ItemType Directory -Force -Path $storage0 | Out-Null
Write-Log $storage0 "dvr-record start motion-only pid=$PID"

while ($true) {
  $cfg = Read-Cfg
  if (-not $cfg) {
    Start-Sleep -Seconds 2
    continue
  }
  $storage = Get-StoragePath $cfg
  New-Item -ItemType Directory -Force -Path $storage | Out-Null
  Sweep-Writers

  if ([int]$cfg.motion_only -eq 0) {
    # off means off — no continuous record
    Start-Sleep -Seconds 2
    continue
  }

  $names = Get-StreamNames
  $threshold = 12
  if ($cfg.motion_threshold) { $threshold = [double]$cfg.motion_threshold }
  foreach ($name in $names) {
    if (-not (Camera-Enabled $cfg $name)) { continue }
    $curr = Join-Path $prevDir "$name.curr.jpg"
    $prev = Join-Path $prevDir "$name.prev.jpg"
    if (-not (Get-Frame $name $curr)) { continue }
    if (Test-Path $prev) {
      $score = Get-DiffScore $prev $curr
      if ($score -ge $threshold) {
        Start-Clip $cfg $name $score
      }
    }
    Copy-Item -Force $curr $prev
  }

  Enforce-Retention $cfg
  Start-Sleep -Seconds 1
}
