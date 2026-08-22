#pragma once

// First-slice ONVIF / live-view NVR hosted as localhost sidecar processes.
// go2rtc and MediaMTX stay out-of-process (may be 64-bit). Do not LoadLibrary
// them into 32-bit T3000.exe. They are MIT-licensed; T3000 talks HTTP/WS only.
//
// Ports (all 127.0.0.1 — do not collide with Rust API 9103/9104 or Quasar 3003):
//   go2rtc  HTTP/API/WebUI  9191
//   go2rtc  RTSP            9192
//   go2rtc  WebRTC          9193
//   MediaMTX RTSP           9194
//   MediaMTX playback       9196
//   MediaMTX API            9197

#define T3000_NVR_GO2RTC_API_PORT     9191
#define T3000_NVR_GO2RTC_RTSP_PORT    9192
#define T3000_NVR_GO2RTC_WEBRTC_PORT  9193
#define T3000_NVR_MEDIAMTX_RTSP_PORT  9194
#define T3000_NVR_MEDIAMTX_PLAY_PORT  9196
#define T3000_NVR_MEDIAMTX_API_PORT   9197

struct T3000NvrLaunchResult
{
	bool go2rtcOk;
	bool go2rtcAlreadyRunning;
	bool go2rtcMissing;
	bool go2rtcPortBusy;
	bool mediamtxOk;
	bool mediamtxAlreadyRunning;
	bool mediamtxMissing;
	CString message;
	CString viewUrl;
};

CString T3000Nvr_GetConfigDir();
CString T3000Nvr_FindSidecarExe(LPCTSTR fileName);
T3000NvrLaunchResult T3000Nvr_StartSidecars();
void T3000Nvr_StopSidecars();
void T3000Nvr_HandleWebMessage(const CString& msg, CString& outmsg);
