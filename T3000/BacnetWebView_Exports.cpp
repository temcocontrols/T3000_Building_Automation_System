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

// Forward declarations
class CMainFrame;
extern void HandleWebViewMsg(CString msg, CString &outmsg, int msg_source = 0);

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

    // Initialize the buffer to ensure it's null-terminated
    memset(msg, 0, iLen);

    // Check if MFC application is properly initialized before calling T3000 functions
    if (!IsAppInitialized()) {
        const char* error = "{\"error\":\"MFC application not initialized\"}";
        size_t error_len = strlen(error);
        if (error_len < (size_t)(iLen - 1)) {
            strncpy_s(msg, iLen, error, error_len);
            msg[error_len] = '\0';
        }
        return -2; // Special error code for initialization issues
    }

    try {
        // Create the JSON message with the action
        std::string json_msg = "{\"action\":" + std::to_string(action) + "}";

        // Convert to CString for the real MFC function
        CString input_msg(json_msg.c_str());
        CString output_msg;

        // Call the REAL HandleWebViewMsg function in BacnetWebView.cpp
        // Use default parameter for msg_source (0)
        HandleWebViewMsg(input_msg, output_msg);

        // Debug logging to understand what's happening
        int output_len = output_msg.GetLength();

        // Safely convert CString result back to char* with proper bounds checking
        if (output_msg.IsEmpty()) {
            // Return empty JSON if no output - this might be the issue
            const char* debug_result = "{\"debug\":\"HandleWebViewMsg returned empty response\"}";
            size_t debug_len = strlen(debug_result);
            if (debug_len < (size_t)(iLen - 1)) {
                strncpy_s(msg, iLen, debug_result, debug_len);
                msg[debug_len] = '\0';
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
            return 0; // Success
        }

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

        return -1; // Buffer too small
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
