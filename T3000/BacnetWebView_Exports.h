#ifndef BACNETWEBVIEW_EXPORTS_H
#define BACNETWEBVIEW_EXPORTS_H

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// BacnetWebView Direct Integration (Simplified Architecture)
// ============================================================================
// This header defines the minimal interface needed for Rust WebView integration.
// The Rust side uses Windows API GetProcAddress to dynamically load functions
// from T3000.exe at runtime, eliminating static linking dependencies.

// PRIMARY EXPORT: Direct bridge to real HandleWebViewMsg in BacnetWebView.cpp
// This is the ONLY function needed for the new simplified architecture
// Named BacnetWebView_HandleWebViewMsg to clearly indicate the source
__declspec(dllexport) int BacnetWebView_HandleWebViewMsg(int action, char* msg, int iLen);

// NEW EXPORTS: Enhanced device configuration access
// These functions provide direct access to g_Device_Basic_Setting array for complete device information
__declspec(dllexport) int GetDeviceBasicSettings(int panel_id, char* buffer, int buffer_size);
__declspec(dllexport) int GetDeviceNetworkConfig(int panel_id, char* buffer, int buffer_size);

// TRENDLOG EXPORTS: Direct access to monitor/trendlog data
// These functions provide trendlog data equivalent to BacnetMonitor dialog functionality
__declspec(dllexport) int BacnetWebView_GetTrendlogList(int panel_id, char* result_buffer, int buffer_size);
__declspec(dllexport) int BacnetWebView_GetTrendlogEntry(int panel_id, int monitor_index, char* result_buffer, int buffer_size);

// MONITOR DATA SYNC: Sync m_monitor_data to g_monitor_data[panel_id]
// Call this after loading device data to ensure webview can access monitor data
__declspec(dllexport) int BacnetWebView_SyncMonitorData(int panel_id);

#ifdef __cplusplus
}
#endif

#endif // BACNETWEBVIEW_EXPORTS_H
