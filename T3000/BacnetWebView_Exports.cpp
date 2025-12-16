#include "stdafx.h"
#include "BacnetWebView_Exports.h"
// Required for MFC CString and conversion macros
#include <afx.h>
#include <atlconv.h>
#include <string>
#include <cstring>
#include "Json-cpp/include/json/json.h"
// Required for Str_Setting_Info structure definition and global variables
#include "CM5/ud_str.h"
#include "global_variable_extern.h"

static bool enable_t3_web_logging = true;
// Forward declarations
class CMainFrame;
extern void HandleWebViewMsg(CString msg, CString &outmsg, int msg_source = 0);

// Helper function for T3WebLog logging
void WriteToT3WebLog(const CString& functionName, const CString& logContent) {
    if (!enable_t3_web_logging) return;
    try {
        SYSTEMTIME st;
        GetSystemTime(&st);

        // Create directory path: T3WebLog\YYYY-MM\MMDD
        CString yearMonth;
        yearMonth.Format(_T("T3WebLog\\%04d-%02d"), st.wYear, st.wMonth);

        CString monthDay;
        monthDay.Format(_T("%02d%02d"), st.wMonth, st.wDay);

        CString logDir;
        logDir.Format(_T("%s\\%s"), yearMonth, monthDay);

        // Create directories if they don't exist
        CreateDirectory(_T("T3WebLog"), NULL);
        CreateDirectory(yearMonth, NULL);
        CreateDirectory(logDir, NULL);

        // Calculate 4-hour bucket (00-03, 04-07, 08-11, 12-15, 16-19, 20-23)
        int start_hour = (st.wHour / 4) * 4;
        int end_hour = start_hour + 3;

        // Create log file with pattern: T3_CppMsg_BacnetWebView_Exports_HHMM.txt
        CString logFile;
        logFile.Format(_T("%s\\T3_CppMsg_BacnetWebView_Exports_%02d%02d.txt"),
            logDir, start_hour, end_hour);

        CStdioFile file;
        // Use append mode to add multiple calls to same 4-hour period file
        if (file.Open(logFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate | CFile::typeText)) {
            // Move to end of file for appending
            file.SeekToEnd();

            // Log entry separator and timestamp
            CString logEntry;
            logEntry.Format(_T("=== %s [%04d-%02d-%02d %02d:%02d:%02d] ===\n"),
                functionName, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
            file.WriteString(logEntry);

            // Write the actual log content
            file.WriteString(logContent);
            file.WriteString(_T("\n"));

            file.WriteString(_T("=== End of Entry ===\r\n"));
            file.Close();
        }
    }
    catch (...) {
        // Silently ignore logging errors to prevent disrupting main functionality
    }
}

// Check if MFC application is properly initialized
bool IsAppInitialized() {
    try {
        CWinApp* pApp = AfxGetApp();
        if (!pApp) return false;

        CMainFrame* pFrame = (CMainFrame*)(pApp->m_pMainWnd);
        if (!pFrame) return false;

        return true;
    }
    catch (...) {
        return false;
    }
}

// Direct bridge to the real HandleWebViewMsg function in BacnetWebView.cpp
// This is the ONLY function Rust should call - provides direct access to BacnetWebView functionality
extern "C" int BacnetWebView_HandleWebViewMsg(int action, char* msg, int iLen) {
    // Validate input parameters
    if (!msg || iLen <= 0) {
        return -1;
    }

    // IMPORTANT: Save the input JSON BEFORE clearing the buffer!
    // Rust sends JSON like: {"action":"LOGGING_DATA","panelId":1,"serialNumber":237219}
    // Or for simple actions, it sends an empty buffer
    std::string input_json_str;

    // Check if buffer has actual JSON content (starts with '{')
    if (msg[0] == '{') {
        // Find null terminator safely
        size_t max_len = 0;
        for (size_t i = 0; i < (size_t)iLen; i++) {
            if (msg[i] == '\0') {
                max_len = i;
                break;
            }
        }
        if (max_len == 0) {
            max_len = iLen - 1; // No null terminator found, use max length
        }
        input_json_str = std::string(msg, max_len);
    }

    // Initialize the buffer to ensure it's null-terminated for the OUTPUT
    memset(msg, 0, iLen);

    // Check if MFC application is properly initialized before calling T3000 functions
    if (!IsAppInitialized()) {
        const char* error = "{\"error\":\"MFC application not initialized\"}";
        size_t error_len = strlen(error);
        if (error_len < (size_t)(iLen - 1)) {
            strncpy_s(msg, iLen, error, error_len);
            msg[error_len] = '\0';
        }

        // Log the initialization error
        CString logContent;
        logContent.Format(_T("ERROR: MFC application not initialized\nAction: %d\nBuffer size: %d\n"), action, iLen);
        WriteToT3WebLog(_T("BacnetWebView_HandleWebViewMsg"), logContent);

        return -2; // Special error code for initialization issues
    }

    try {
        // Log function entry with the INPUT JSON
        CString logContent;
        logContent.Format(_T("Function called with:\nAction: %d\nBuffer size: %d\nInput JSON length: %d\n"),
            action, iLen, (int)input_json_str.length());

        // Determine which JSON to use
        std::string json_msg;
        if (!input_json_str.empty()) {
            // Use the JSON that Rust sent us (has parameters like panelId, serialNumber)
            json_msg = input_json_str;
            logContent.AppendFormat(_T("Using input JSON from Rust: %S\n"), json_msg.c_str());
        } else {
            // Create simple JSON with just the action (for backward compatibility)
            json_msg = "{\"action\":" + std::to_string(action) + "}";
            logContent.AppendFormat(_T("Created simple JSON: %S\n"), json_msg.c_str());
        }

        logContent.AppendFormat(_T("About to convert std::string to CString...\n"));
        WriteToT3WebLog(_T("BacnetWebView_HandleWebViewMsg"), logContent);

        CString input_msg(json_msg.c_str());
        CString output_msg;

        logContent.Format(_T("Calling HandleWebViewMsg with input: %s\n"), input_msg);
        WriteToT3WebLog(_T("BacnetWebView_HandleWebViewMsg"), logContent);

        // Call the REAL HandleWebViewMsg function in BacnetWebView.cpp
        // Use default parameter for msg_source (0)
        HandleWebViewMsg(input_msg, output_msg);

        logContent.Format(_T("HandleWebViewMsg returned successfully\n"));
        WriteToT3WebLog(_T("BacnetWebView_HandleWebViewMsg"), logContent);

        // Debug logging to understand what's happening
        int output_len = output_msg.GetLength();

        logContent.AppendFormat(_T("HandleWebViewMsg returned %d characters\n"), output_len);

        // Safely convert CString result back to char* with proper bounds checking
        if (output_msg.IsEmpty()) {
            // Return empty JSON if no output - this might be the issue
            const char* debug_result = "{\"debug\":\"HandleWebViewMsg returned empty response\"}";
            size_t debug_len = strlen(debug_result);
            if (debug_len < (size_t)(iLen - 1)) {
                strncpy_s(msg, iLen, debug_result, debug_len);
                msg[debug_len] = '\0';

                logContent.AppendFormat(_T("WARNING: Empty response from HandleWebViewMsg\n"));
                WriteToT3WebLog(_T("BacnetWebView_HandleWebViewMsg"), logContent);

                return 0; // Return success with debug info
            }
            return -1; // Buffer too small even for debug message
        }

        // Convert output safely
        std::string result = CT2A(output_msg);
        size_t result_len = result.length();

        if (result_len < (size_t)(iLen - 1)) { // Leave space for null terminator
            strncpy_s(msg, iLen, result.c_str(), result_len);
            msg[result_len] = '\0'; // Ensure null termination

            logContent.AppendFormat(_T("SUCCESS: Returned %d bytes\n=== Full JSON Response ===\n%S\n=== End JSON Response ===\n"),
                (int)result_len, result.c_str());
            WriteToT3WebLog(_T("BacnetWebView_HandleWebViewMsg"), logContent);

            return 0; // Success
        }

        logContent.AppendFormat(_T("ERROR: Buffer too small (needed %d, have %d)\n"),
            (int)result_len, iLen);
        WriteToT3WebLog(_T("BacnetWebView_HandleWebViewMsg"), logContent);

        return -1; // Buffer too small
    }
    catch (...) {
        // Return safe error message if anything goes wrong
        const char* error = "{\"error\":\"Exception in BacnetWebView HandleWebViewMsg bridge\"}";
        size_t error_len = strlen(error);
        if (error_len < (size_t)(iLen - 1)) {
            strncpy_s(msg, iLen, error, error_len);
            msg[error_len] = '\0';
        } else {
            // If even error message is too big, just return empty JSON
            if (iLen >= 3) {
                strcpy_s(msg, iLen, "{}");
            }
        }
        return -1;
    }
}

// New FFI Function: Get Device Basic Settings from g_Device_Basic_Setting array
// This function provides access to the complete device configuration stored in g_Device_Basic_Setting[panel_id].reg
extern "C" int GetDeviceBasicSettings(int panel_id, char* buffer, int buffer_size) {
    // Validate input parameters
    if (!buffer || buffer_size <= 0 || panel_id < 0 || panel_id >= 256) {
        return -1;
    }

    // Initialize buffer
    memset(buffer, 0, buffer_size);

    // Check if MFC application is properly initialized
    if (!IsAppInitialized()) {
        const char* error = "{\"error\":\"MFC application not initialized\"}";
        size_t error_len = strlen(error);
        if (error_len < (size_t)(buffer_size - 1)) {
            strncpy_s(buffer, buffer_size, error, error_len);
            buffer[error_len] = '\0';
        }
        return -2;
    }

    try {
        // Access the Device_Basic_Setting for the specified panel
        Str_Setting_Info& device_settings = g_Device_Basic_Setting[panel_id];

        // Create JSON object with device configuration
        Json::Value json_response;
        Json::StreamWriterBuilder builder;

        // Basic device information
        json_response["panel_id"] = panel_id;
        json_response["panel_name"] = std::string((char*)device_settings.reg.panel_name, 20);
        json_response["serial_number"] = (int)device_settings.reg.n_serial_number;

        // Network configuration - the key data our Rust code needs
        char ip_str[16];
        sprintf(ip_str, "%d.%d.%d.%d",
                device_settings.reg.ip_addr[0],
                device_settings.reg.ip_addr[1],
                device_settings.reg.ip_addr[2],
                device_settings.reg.ip_addr[3]);
        json_response["ip_address"] = std::string(ip_str);

        json_response["modbus_id"] = (int)device_settings.reg.modbus_id;
        json_response["modbus_port"] = (int)device_settings.reg.modbus_port;
        json_response["object_instance"] = (int)device_settings.reg.object_instance;
        json_response["mstp_id"] = (int)device_settings.reg.mstp_id;
        json_response["panel_number"] = (int)device_settings.reg.panel_number;
        json_response["mini_type"] = (int)device_settings.reg.mini_type;

        // Additional network settings
        json_response["network_number"] = (int)device_settings.reg.network_number;
        json_response["mstp_network_number"] = (int)device_settings.reg.mstp_network_number;
        json_response["tcp_type"] = (int)device_settings.reg.tcp_type; // DHCP/Static

        // Convert JSON to string
        std::string json_string = Json::writeString(builder, json_response);

        // Copy to output buffer
        size_t json_len = json_string.length();
        if (json_len < (size_t)(buffer_size - 1)) {
            strncpy_s(buffer, buffer_size, json_string.c_str(), json_len);
            buffer[json_len] = '\0';
            return (int)json_len; // Return actual data length
        }

        return -1; // Buffer too small
    }
    catch (...) {
        const char* error = "{\"error\":\"Exception in GetDeviceBasicSettings\"}";
        size_t error_len = strlen(error);
        if (error_len < (size_t)(buffer_size - 1)) {
            strncpy_s(buffer, buffer_size, error, error_len);
            buffer[error_len] = '\0';
        }
        return -1;
    }
}

// New FFI Function: Get Device Network Configuration
// This function provides additional network configuration details
extern "C" int GetDeviceNetworkConfig(int panel_id, char* buffer, int buffer_size) {
    // Validate input parameters
    if (!buffer || buffer_size <= 0 || panel_id < 0 || panel_id >= 256) {
        return -1;
    }

    // Initialize buffer
    memset(buffer, 0, buffer_size);

    // Check if MFC application is properly initialized
    if (!IsAppInitialized()) {
        const char* error = "{\"error\":\"MFC application not initialized\"}";
        size_t error_len = strlen(error);
        if (error_len < (size_t)(buffer_size - 1)) {
            strncpy_s(buffer, buffer_size, error, error_len);
            buffer[error_len] = '\0';
        }
        return -2;
    }

    try {
        // Access the Device_Basic_Setting for the specified panel
        Str_Setting_Info& device_settings = g_Device_Basic_Setting[panel_id];

        // Create JSON object with network configuration
        Json::Value json_response;
        Json::StreamWriterBuilder builder;

        json_response["panel_id"] = panel_id;

        // Network configuration details
        char subnet_str[16];
        sprintf(subnet_str, "%d.%d.%d.%d",
                device_settings.reg.subnet[0],
                device_settings.reg.subnet[1],
                device_settings.reg.subnet[2],
                device_settings.reg.subnet[3]);
        json_response["subnet_mask"] = std::string(subnet_str);

        char gateway_str[16];
        sprintf(gateway_str, "%d.%d.%d.%d",
                device_settings.reg.gate_addr[0],
                device_settings.reg.gate_addr[1],
                device_settings.reg.gate_addr[2],
                device_settings.reg.gate_addr[3]);
        json_response["gateway"] = std::string(gateway_str);

        // MAC Address
        char mac_str[18];
        sprintf(mac_str, "%02X:%02X:%02X:%02X:%02X:%02X",
                device_settings.reg.mac_addr[0],
                device_settings.reg.mac_addr[1],
                device_settings.reg.mac_addr[2],
                device_settings.reg.mac_addr[3],
                device_settings.reg.mac_addr[4],
                device_settings.reg.mac_addr[5]);
        json_response["mac_address"] = std::string(mac_str);

        // Communication settings
        json_response["com0_config"] = (int)device_settings.reg.com0_config;
        json_response["com1_config"] = (int)device_settings.reg.com1_config;
        json_response["com2_config"] = (int)device_settings.reg.com2_config;

        // BACnet specific
        json_response["bacnet_ip_port"] = 47808; // Standard BACnet/IP port
        json_response["connection_type"] = device_settings.reg.tcp_type == 0 ? "DHCP" : "STATIC";

        // PC connection info (this might be derived from current connection)
        json_response["pc_ip_address"] = "192.168.1.100"; // Default/example - could be improved

        // Convert JSON to string
        std::string json_string = Json::writeString(builder, json_response);

        // Copy to output buffer
        size_t json_len = json_string.length();
        if (json_len < (size_t)(buffer_size - 1)) {
            strncpy_s(buffer, buffer_size, json_string.c_str(), json_len);
            buffer[json_len] = '\0';
            return (int)json_len; // Return actual data length
        }

        return -1;
    }
    catch (...) {
        const char* error = "{\"error\":\"Exception in GetDeviceNetworkConfig\"}";
        size_t error_len = strlen(error);
        if (error_len < (size_t)(buffer_size - 1)) {
            strncpy_s(buffer, buffer_size, error, error_len);
            buffer[error_len] = '\0';
        }
        return -1;
    }
}

// NEW TRENDLOG EXPORT FUNCTIONS
// ==============================

// Helper function to convert interval time to text (same as BacnetMonitor.cpp)
std::string IntervalToText(int total_seconds) {
    if (total_seconds == 0) {
        return "0 sec";
    }

    int hours = total_seconds / 3600;
    int minutes = (total_seconds % 3600) / 60;
    int seconds = total_seconds % 60;

    if (hours > 0) {
        if (minutes > 0) {
            return std::to_string(hours) + "h " + std::to_string(minutes) + "m";
        }
        return std::to_string(hours) + " hour" + (hours == 1 ? "" : "s");
    }
    else if (minutes > 0) {
        if (seconds > 0) {
            return std::to_string(minutes) + "m " + std::to_string(seconds) + "s";
        }
        return std::to_string(minutes) + " min";
    }
    else {
        return std::to_string(seconds) + " sec";
    }
}

// Get complete trendlog list for a device (same data as Fresh_Monitor_List)
extern "C" __declspec(dllexport) int BacnetWebView_GetTrendlogList(int panel_id, char* result_buffer, int buffer_size) {
    // Log function entry
    CString logContent;
    logContent.Format(_T("Function called with:\nPanel ID: %d\nBuffer size: %d\n"), panel_id, buffer_size);

    if (!result_buffer || buffer_size <= 0) {
        logContent.AppendFormat(_T("ERROR: Invalid parameters (null buffer or buffer_size <= 0)\n"));
        WriteToT3WebLog(_T("BacnetWebView_GetTrendlogList"), logContent);
        return -1;
    }

    // Initialize buffer
    memset(result_buffer, 0, buffer_size);

    if (!IsAppInitialized()) {
        const char* error = "{\"error\":\"MFC application not initialized\"}";
        size_t error_len = strlen(error);
        if (error_len < (size_t)(buffer_size - 1)) {
            strncpy_s(result_buffer, buffer_size, error, error_len);
        }

        logContent.AppendFormat(_T("ERROR: MFC application not initialized\n"));
        WriteToT3WebLog(_T("BacnetWebView_GetTrendlogList"), logContent);

        return -1;
    }

    try {
        Json::Value json_result;
        Json::StreamWriterBuilder builder;
        builder["indentation"] = "";

        json_result["success"] = true;
        json_result["panel_id"] = panel_id;

        // CRITICAL FIX: Determine which data source to use (g_monitor_data or m_monitor_data)
        // Problem: g_monitor_data[panel_id] is often empty, returning "Monitor 1", "Monitor 2" defaults
        // Solution: Fallback to m_monitor_data (UI's current data) when g_monitor_data is empty
        bool use_m_monitor_data = false;
        int monitors_count = 0;

        // Check if g_monitor_data[panel_id] exists and has real data
        if (panel_id >= 0 && (size_t)panel_id < g_monitor_data.size() && g_monitor_data[panel_id].size() > 0) {
            // Check if first monitor has real label (not empty or default "Monitor N")
            std::string first_label((char*)g_monitor_data[panel_id].at(0).label);
            bool has_real_data = !first_label.empty() && first_label.find("Monitor ") != 0;

            if (has_real_data) {
                // g_monitor_data has real data, use it
                monitors_count = (int)g_monitor_data[panel_id].size();
                use_m_monitor_data = false;
                logContent.AppendFormat(_T("✅ Using g_monitor_data[%d] with %d monitors (label='%S')\n"),
                    panel_id, monitors_count, first_label.c_str());
            } else {
                // g_monitor_data is empty or has default values, try m_monitor_data
                if (m_monitor_data.size() > 0) {
                    std::string m_first_label((char*)m_monitor_data.at(0).label);
                    monitors_count = (int)m_monitor_data.size();
                    use_m_monitor_data = true;
                    logContent.AppendFormat(_T("⚠️ g_monitor_data[%d] empty/default, FALLBACK to m_monitor_data with %d monitors (label='%S')\n"),
                        panel_id, monitors_count, m_first_label.c_str());
                } else {
                    // Both empty - use g_monitor_data anyway (will return defaults)
                    monitors_count = (int)g_monitor_data[panel_id].size();
                    use_m_monitor_data = false;
                    logContent.AppendFormat(_T("⚠️ WARNING: Both sources empty! Using g_monitor_data[%d] with %d monitors\n"),
                        panel_id, monitors_count);
                }
            }
        } else if (m_monitor_data.size() > 0) {
            // panel_id not in g_monitor_data or out of range, use m_monitor_data
            std::string m_first_label((char*)m_monitor_data.at(0).label);
            monitors_count = (int)m_monitor_data.size();
            use_m_monitor_data = true;
            logContent.AppendFormat(_T("⚠️ g_monitor_data[%d] not available, FALLBACK to m_monitor_data with %d monitors (label='%S')\n"),
                panel_id, monitors_count, m_first_label.c_str());
        } else {
            // No data available at all
            logContent.AppendFormat(_T("❌ ERROR: No monitor data available (neither g_monitor_data[%d] nor m_monitor_data)\n"), panel_id);
            WriteToT3WebLog(_T("BacnetWebView_GetTrendlogList"), logContent);

            json_result["success"] = false;
            json_result["error"] = "No monitor data available";
            json_result["total_monitors"] = 0;
            json_result["timestamp"] = std::time(nullptr);

            std::string json_string = Json::writeString(builder, json_result);
            size_t json_len = json_string.length();
            if (json_len < (size_t)(buffer_size - 1)) {
                strncpy_s(result_buffer, buffer_size, json_string.c_str(), json_len);
                result_buffer[json_len] = '\0';
                return (int)json_len;
            }
            return -1;
        }

        json_result["total_monitors"] = monitors_count;
        json_result["data_source"] = use_m_monitor_data ? "m_monitor_data" : "g_monitor_data";

        // Build trendlog array using the selected data source
        for (int i = 0; i < monitors_count; i++) {
            Json::Value trendlog_item;

            // Select the appropriate monitor data structure
            Str_monitor_point& monitor = use_m_monitor_data
                ? m_monitor_data.at(i)
                : g_monitor_data[panel_id].at(i);

            // NUM field (monitor index)
            trendlog_item["num"] = i;
            trendlog_item["id"] = std::string("MON") + std::to_string(i + 1);

            // LABEL field (from monitor data structure)
            std::string label_str((char*)monitor.label);
            trendlog_item["label"] = label_str.empty() ? ("Monitor " + std::to_string(i + 1)) : label_str;

            // INTERVAL field (calculated same as Fresh_Monitor_List)
            int interval_seconds = monitor.hour_interval_time * 3600 +
                                 monitor.minute_interval_time * 60 +
                                 monitor.second_interval_time;
            trendlog_item["interval_seconds"] = interval_seconds;
            trendlog_item["interval_text"] = IntervalToText(interval_seconds);

            // STATUS field (ON/OFF same as Fresh_Monitor_List)
            if (monitor.status == 1) {
                trendlog_item["status"] = "ON";
                trendlog_item["status_code"] = 1;
            } else {
                trendlog_item["status"] = "OFF";
                trendlog_item["status_code"] = 0;
            }

            // DATA SIZE field (calculated same as Fresh_Monitor_List)
            float data_size_kb = 0.0f;
            if ((Device_Misc_Data.reg.flag[0] == 0x55) && (Device_Misc_Data.reg.flag[1] == 0xff)) {
                unsigned long total_blocks = (unsigned long)Device_Misc_Data.reg.monitor_analog_block_num[i] +
                                           (unsigned long)Device_Misc_Data.reg.monitor_digital_block_num[i];
                data_size_kb = (float)total_blocks * 400.0f / 1000.0f;
            }
            trendlog_item["data_size_kb"] = data_size_kb;
            trendlog_item["data_size_text"] = std::to_string(data_size_kb).substr(0, 4);

            // Additional useful fields
            trendlog_item["num_inputs"] = (int)monitor.num_inputs;
            trendlog_item["an_inputs"] = (int)monitor.an_inputs;

            json_result["trendlogs"][i] = trendlog_item;
        }

        json_result["timestamp"] = std::time(nullptr);

        // Convert to JSON string
        std::string json_string = Json::writeString(builder, json_result);

        // Copy to result buffer
        size_t json_len = json_string.length();
        if (json_len < (size_t)(buffer_size - 1)) {
            strncpy_s(result_buffer, buffer_size, json_string.c_str(), json_len);
            result_buffer[json_len] = '\0';

            logContent.AppendFormat(_T("✅ SUCCESS: Returned %d monitors from %s, %d bytes\n"),
                monitors_count,
                use_m_monitor_data ? _T("m_monitor_data") : _T("g_monitor_data"),
                (int)json_len);
            logContent.AppendFormat(_T("=== Full JSON Response ===\n%S\n=== End JSON Response ===\n"), json_string.c_str());
            WriteToT3WebLog(_T("BacnetWebView_GetTrendlogList"), logContent);

            return (int)json_len;
        }

        logContent.AppendFormat(_T("ERROR: Buffer too small (needed %d, have %d)\n"),
            (int)json_len, buffer_size);
        WriteToT3WebLog(_T("BacnetWebView_GetTrendlogList"), logContent);

        return -1; // Buffer too small
    }
    catch (...) {
        const char* error = "{\"error\":\"Exception in GetTrendlogList\"}";
        size_t error_len = strlen(error);
        if (error_len < (size_t)(buffer_size - 1)) {
            strncpy_s(result_buffer, buffer_size, error, error_len);
        }

        logContent.AppendFormat(_T("EXCEPTION: Caught exception in GetTrendlogList\n"));
        WriteToT3WebLog(_T("BacnetWebView_GetTrendlogList"), logContent);

        return -1;
    }
}

// Get specific trendlog entry (same data as Fresh_Monitor_List for single item)
extern "C" __declspec(dllexport) int BacnetWebView_GetTrendlogEntry(int panel_id, int monitor_index, char* result_buffer, int buffer_size) {
    // Log function entry
    CString logContent;
    logContent.Format(_T("Function called with:\nPanel ID: %d\nMonitor Index: %d\nBuffer size: %d\n"),
        panel_id, monitor_index, buffer_size);

    if (!result_buffer || buffer_size <= 0 || monitor_index < 0 || monitor_index >= BAC_MONITOR_COUNT) {
        logContent.AppendFormat(_T("ERROR: Invalid parameters\n"));
        logContent.AppendFormat(_T("  null buffer: %s\n"), result_buffer ? _T("false") : _T("true"));
        logContent.AppendFormat(_T("  buffer_size <= 0: %s\n"), buffer_size <= 0 ? _T("true") : _T("false"));
        logContent.AppendFormat(_T("  monitor_index < 0: %s\n"), monitor_index < 0 ? _T("true") : _T("false"));
        logContent.AppendFormat(_T("  monitor_index >= BAC_MONITOR_COUNT(%d): %s\n"),
            BAC_MONITOR_COUNT, monitor_index >= BAC_MONITOR_COUNT ? _T("true") : _T("false"));
        WriteToT3WebLog(_T("BacnetWebView_GetTrendlogEntry"), logContent);
        return -1;
    }

    // Initialize buffer
    memset(result_buffer, 0, buffer_size);

    if (!IsAppInitialized()) {
        const char* error = "{\"error\":\"MFC application not initialized\"}";
        size_t error_len = strlen(error);
        if (error_len < (size_t)(buffer_size - 1)) {
            strncpy_s(result_buffer, buffer_size, error, error_len);
        }

        logContent.AppendFormat(_T("ERROR: MFC application not initialized\n"));
        WriteToT3WebLog(_T("BacnetWebView_GetTrendlogEntry"), logContent);

        return -1;
    }

    try {
        Json::Value json_result;
        Json::StreamWriterBuilder builder;
        builder["indentation"] = "";

        json_result["success"] = true;
        json_result["panel_id"] = panel_id;
        json_result["monitor_index"] = monitor_index;

        // CRITICAL FIX: Same fallback logic as GetTrendlogList
        // Determine which data source to use (g_monitor_data or m_monitor_data)
        bool use_m_monitor_data = false;
        int monitors_count = 0;

        // Check if g_monitor_data[panel_id] exists and has real data
        if (panel_id >= 0 && (size_t)panel_id < g_monitor_data.size() && g_monitor_data[panel_id].size() > 0) {
            std::string first_label((char*)g_monitor_data[panel_id].at(0).label);
            bool has_real_data = !first_label.empty() && first_label.find("Monitor ") != 0;

            if (has_real_data) {
                monitors_count = (int)g_monitor_data[panel_id].size();
                use_m_monitor_data = false;
                logContent.AppendFormat(_T("✅ Using g_monitor_data[%d] with %d monitors\n"),
                    panel_id, monitors_count);
            } else if (m_monitor_data.size() > 0) {
                monitors_count = (int)m_monitor_data.size();
                use_m_monitor_data = true;
                logContent.AppendFormat(_T("⚠️ FALLBACK to m_monitor_data with %d monitors\n"), monitors_count);
            } else {
                monitors_count = (int)g_monitor_data[panel_id].size();
                use_m_monitor_data = false;
                logContent.AppendFormat(_T("⚠️ Both sources empty, using g_monitor_data[%d]\n"), panel_id);
            }
        } else if (m_monitor_data.size() > 0) {
            monitors_count = (int)m_monitor_data.size();
            use_m_monitor_data = true;
            logContent.AppendFormat(_T("⚠️ FALLBACK to m_monitor_data with %d monitors\n"), monitors_count);
        } else {
            json_result["success"] = false;
            json_result["error"] = "No monitor data available";
            std::string json_string = Json::writeString(builder, json_result);
            size_t json_len = json_string.length();
            if (json_len < (size_t)(buffer_size - 1)) {
                strncpy_s(result_buffer, buffer_size, json_string.c_str(), json_len);
                result_buffer[json_len] = '\0';
                logContent.AppendFormat(_T("❌ ERROR: No monitor data available\n"));
                WriteToT3WebLog(_T("BacnetWebView_GetTrendlogEntry"), logContent);
                return (int)json_len;
            }
            return -1;
        }

        // Validate monitor_index
        if (monitor_index < 0 || monitor_index >= monitors_count) {
            json_result["success"] = false;
            json_result["error"] = "monitor_index out of range";
            json_result["available_monitors"] = monitors_count;
            std::string json_string = Json::writeString(builder, json_result);
            size_t json_len = json_string.length();
            if (json_len < (size_t)(buffer_size - 1)) {
                strncpy_s(result_buffer, buffer_size, json_string.c_str(), json_len);
                result_buffer[json_len] = '\0';
                logContent.AppendFormat(_T("ERROR: monitor_index %d out of range (have %d monitors)\n"),
                    monitor_index, monitors_count);
                WriteToT3WebLog(_T("BacnetWebView_GetTrendlogEntry"), logContent);
                return (int)json_len;
            }
            return -1;
        }

        // Get the monitor data from the selected source
        Str_monitor_point& monitor = use_m_monitor_data
            ? m_monitor_data.at(monitor_index)
            : g_monitor_data[panel_id].at(monitor_index);

        // Get single trendlog data (same fields as GetTrendlogList)
        Json::Value trendlog_item;

        // NUM field
        trendlog_item["num"] = monitor_index;
        trendlog_item["id"] = std::string("MON") + std::to_string(monitor_index + 1);

        // LABEL field
        std::string label_str((char*)monitor.label);
        trendlog_item["label"] = label_str.empty() ? ("Monitor " + std::to_string(monitor_index + 1)) : label_str;

        // INTERVAL field
        int interval_seconds = monitor.hour_interval_time * 3600 +
                             monitor.minute_interval_time * 60 +
                             monitor.second_interval_time;
        trendlog_item["interval_seconds"] = interval_seconds;
        trendlog_item["interval_text"] = IntervalToText(interval_seconds);

        // STATUS field
        if (monitor.status == 1) {
            trendlog_item["status"] = "ON";
            trendlog_item["status_code"] = 1;
        } else {
            trendlog_item["status"] = "OFF";
            trendlog_item["status_code"] = 0;
        }

        // DATA SIZE field
        float data_size_kb = 0.0f;
        if ((Device_Misc_Data.reg.flag[0] == 0x55) && (Device_Misc_Data.reg.flag[1] == 0xff)) {
            unsigned long total_blocks = (unsigned long)Device_Misc_Data.reg.monitor_analog_block_num[monitor_index] +
                                       (unsigned long)Device_Misc_Data.reg.monitor_digital_block_num[monitor_index];
            data_size_kb = (float)total_blocks * 400.0f / 1000.0f;
        }
        trendlog_item["data_size_kb"] = data_size_kb;
        trendlog_item["data_size_text"] = std::to_string(data_size_kb).substr(0, 4);

        // Additional fields
        trendlog_item["num_inputs"] = (int)monitor.num_inputs;
        trendlog_item["an_inputs"] = (int)monitor.an_inputs;

        // Input points details (same as Fresh_Monitor_Input_List)
        Json::Value inputs_array;
        for (int j = 0; j < MAX_POINTS_IN_MONITOR; j++) {
            if (j < monitor.num_inputs) {
                Json::Value input_item;
                input_item["index"] = j;
                input_item["panel"] = (int)monitor.inputs[j].panel;
                input_item["sub_panel"] = (int)monitor.inputs[j].sub_panel;
                input_item["point_type"] = (int)monitor.inputs[j].point_type;
                input_item["point_number"] = (int)monitor.inputs[j].number;
                input_item["network"] = (int)monitor.inputs[j].network;
                input_item["range"] = (int)monitor.range[j];
                input_item["network"] = (int)g_monitor_data[panel_id].at(monitor_index).inputs[j].network;
                input_item["range"] = (int)g_monitor_data[panel_id].at(monitor_index).range[j];
                inputs_array[j] = input_item;
            }
        }
        trendlog_item["inputs"] = inputs_array;

        json_result["trendlog"] = trendlog_item;
        json_result["data_source"] = use_m_monitor_data ? "m_monitor_data" : "g_monitor_data";
        json_result["timestamp"] = std::time(nullptr);

        logContent.AppendFormat(_T("✅ SUCCESS: Returning monitor %d '%S' from %s\n"),
            monitor_index, label_str.c_str(),
            use_m_monitor_data ? _T("m_monitor_data") : _T("g_monitor_data"));

        // Convert to JSON string
        std::string json_string = Json::writeString(builder, json_result);

        // Copy to result buffer
        size_t json_len = json_string.length();
        if (json_len < (size_t)(buffer_size - 1)) {
            strncpy_s(result_buffer, buffer_size, json_string.c_str(), json_len);
            result_buffer[json_len] = '\0';

            logContent.AppendFormat(_T("SUCCESS: Returned monitor entry, %d bytes\n"), (int)json_len);
            logContent.AppendFormat(_T("=== Full JSON Response ===\n%S\n=== End JSON Response ===\n"), json_string.c_str());
            WriteToT3WebLog(_T("BacnetWebView_GetTrendlogEntry"), logContent);

            return (int)json_len;
        }

        logContent.AppendFormat(_T("ERROR: Buffer too small (needed %d, have %d)\n"),
            (int)json_len, buffer_size);
        WriteToT3WebLog(_T("BacnetWebView_GetTrendlogEntry"), logContent);

        return -1; // Buffer too small
    }
    catch (...) {
        const char* error = "{\"error\":\"Exception in GetTrendlogEntry\"}";
        size_t error_len = strlen(error);
        if (error_len < (size_t)(buffer_size - 1)) {
            strncpy_s(result_buffer, buffer_size, error, error_len);
        }

        logContent.AppendFormat(_T("EXCEPTION: Caught exception in GetTrendlogEntry\n"));
        WriteToT3WebLog(_T("BacnetWebView_GetTrendlogEntry"), logContent);

        return -1;
    }
}

// Sync m_monitor_data to g_monitor_data[panel_id] to ensure webview can access monitor data
// This function should be called after loading a device or when monitor data is updated
extern "C" int BacnetWebView_SyncMonitorData(int panel_id) {
    CString logContent;
    logContent.AppendFormat(_T("=== BacnetWebView_SyncMonitorData START ===\n"));
    logContent.AppendFormat(_T("panel_id: %d\n"), panel_id);

    try {
        // Validate panel_id
        if (panel_id < 0 || panel_id >= 256) {
            logContent.AppendFormat(_T("ERROR: Invalid panel_id %d (must be 0-255)\n"), panel_id);
            WriteToT3WebLog(_T("BacnetWebView_SyncMonitorData"), logContent);
            return -1;
        }

        // Ensure g_monitor_data is large enough
        if (g_monitor_data.size() <= (size_t)panel_id) {
            logContent.AppendFormat(_T("ERROR: g_monitor_data.size()=%d is too small for panel_id %d\n"),
                (int)g_monitor_data.size(), panel_id);
            WriteToT3WebLog(_T("BacnetWebView_SyncMonitorData"), logContent);
            return -1;
        }

        // Check if m_monitor_data has any monitors
        int monitor_count = (int)m_monitor_data.size();
        logContent.AppendFormat(_T("m_monitor_data.size() = %d\n"), monitor_count);

        if (monitor_count == 0) {
            logContent.AppendFormat(_T("WARNING: m_monitor_data is empty, nothing to sync\n"));
            WriteToT3WebLog(_T("BacnetWebView_SyncMonitorData"), logContent);
            return 0;  // Not an error, just empty data
        }

        // Ensure g_monitor_data[panel_id] is the correct size
        if (g_monitor_data[panel_id].size() != m_monitor_data.size()) {
            logContent.AppendFormat(_T("Resizing g_monitor_data[%d] from %d to %d\n"),
                panel_id, (int)g_monitor_data[panel_id].size(), monitor_count);
            g_monitor_data[panel_id].resize(monitor_count);
        }

        // Copy all monitor data from m_monitor_data to g_monitor_data[panel_id]
        int synced_count = 0;
        for (int i = 0; i < monitor_count; i++) {
            g_monitor_data[panel_id].at(i) = m_monitor_data.at(i);
            synced_count++;
        }

        logContent.AppendFormat(_T("SUCCESS: Synced %d monitors to g_monitor_data[%d]\n"),
            synced_count, panel_id);

        // Log first monitor as verification
        if (synced_count > 0) {
            std::string label_str((char*)g_monitor_data[panel_id].at(0).label);
            logContent.AppendFormat(_T("First monitor label: '%S'\n"), label_str.c_str());
            logContent.AppendFormat(_T("First monitor status: %d\n"), (int)g_monitor_data[panel_id].at(0).status);
        }

        WriteToT3WebLog(_T("BacnetWebView_SyncMonitorData"), logContent);

        return synced_count;  // Return number of monitors synced
    }
    catch (const std::exception& e) {
        logContent.AppendFormat(_T("EXCEPTION: %S\n"), e.what());
        WriteToT3WebLog(_T("BacnetWebView_SyncMonitorData"), logContent);
        return -1;
    }
    catch (...) {
        logContent.AppendFormat(_T("EXCEPTION: Unknown exception\n"));
        WriteToT3WebLog(_T("BacnetWebView_SyncMonitorData"), logContent);
        return -1;
    }
}

