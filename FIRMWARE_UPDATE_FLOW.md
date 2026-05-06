# T3000 Firmware Update Flow - Complete Technical Analysis
## Ctrl+F2 / Tools → Load Firmware (Single Device)

---

## 1. ENTRY POINT & MENU DEFINITION

### Menu Item Definition
- **File**: `T3000/T3000.rc` (Resource File)
- **Menu ID**: `ID_FILE_BATCHBURNHEX`
- **Shortcut**: `Ctrl+F2`
- **Display Text**: "Load firmware for a single device"

### Message Map Binding
- **Location**: [T3000/MainFrm.cpp](T3000/MainFrm.cpp#L294)
- **Binding**: `ON_COMMAND(ID_FILE_BATCHBURNHEX, OnBatchFlashHex)`

---

## 2. MAIN HANDLER FUNCTION

### Function: `CMainFrame::OnBatchFlashHex()`
- **Location**: [T3000/MainFrm.cpp](T3000/MainFrm.cpp#L3108)
- **Class**: `CMainFrame` (MFC Application Main Window)

### Pre-Flash Setup Phase
```cpp
// Step 1: Save current state
b_pause_refresh_tree = BATCH_FLASH_HEX;  // Pause tree refresh
bool temp_status = g_bPauseMultiRead;
g_bPauseMultiRead = true;
int temp_type = GetCommunicationType();  // Save communication type (Serial=0 or Network=1)

// Step 2: Close existing connections
BOOL bDontLinger = FALSE;
setsockopt(h_Broad, SOL_SOCKET, SO_DONTLINGER, (const char*)&bDontLinger, sizeof(BOOL));
closesocket(h_Broad);  // Close broadcast socket
SetCommunicationType(0);  // Set to serial mode
close_com();  // Close serial port (free for ISP tool)

// Step 3: Create firmware update dialog
CFlash_Multy dlg;
dlg.DoModal();  // Modal dialog - blocks until user completes or cancels
```

### Post-Flash Cleanup Phase
```cpp
// Step 4: Restore communication
if(temp_type == 0) {
    int comport = GetLastOpenedComport();
    open_com(comport);  // Reopen serial port
} else {
    // Network mode restoration
}

SetCommunicationType(temp_type);  // Restore communication type

// Step 5: Recreate broadcast socket
h_Broad = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
BOOL bBroadcast = TRUE;
::setsockopt(h_Broad, SOL_SOCKET, SO_BROADCAST, (char*)&bBroadcast, sizeof(BOOL));
int iMode = 1;
ioctlsocket(h_Broad, FIONBIO, (u_long FAR*) &iMode);

// Step 6: Resume tree refresh
h_bcast.sin_family = AF_INET;
h_bcast.sin_addr.s_addr = INADDR_BROADCAST;
h_bcast.sin_port = htons(UDP_BROADCAST_PORT);
```

---

## 3. FIRMWARE UPDATE DIALOG

### Class: `CFlash_Multy`
- **Header File**: [T3000/Flash_Multy.h](T3000/Flash_Multy.h)
- **Implementation**: [T3000/Flash_Multy.cpp](T3000/Flash_Multy.cpp)
- **Base Class**: `CDialogEx`
- **Resource ID**: `IDD_DIALOG_MULTY_FLASH`

### Dialog Initialization: `OnInitDialog()`
**Location**: [Flash_Multy.cpp:175](T3000/Flash_Multy.cpp#L175)

**Initialization Steps:**

1. **Initial_List()** - Populate device list
   - Reads device tree
   - Inserts devices into list control
   - Columns: ID, Product Name, Serial Number, COM Port, Baudrate, IP Address, IP Port

2. **GetProductType()** - Detect device type
   - Identifies product type based on register values
   - Types: TSTAT5, TSTAT6, T3 Modules, etc.

3. **Get_Device_Firmware()** - Read current firmware versions
   - Queries each device for current firmware revision
   - Displays in "Current Firmware Version" column

**Data Structure:**
```cpp
typedef struct {
    int nitem;
    CString nID;
    CString devicename;
    CString strSN;              // Serial Number
    CString ncomport;           // COM Port
    CString nBaudrate;          // Baud Rate
    CString nIPaddress;         // IP Address
    CString nipport;            // IP Port
    CString file_position;      // Firmware file path (.hex)
    CString config_file_position; // Config file path
    int nresult;                // Flash result
    int cofnigresult;           // Config result
    bool online;                // Device online status
    float software_rev;         // Current firmware version
    float newest_rev;           // Latest available version
    CString file_rev;           // Selected firmware file version
    bool need_flash;            // Flash needed flag
    unsigned char product_id;   // Product ID
    bool select_status;         // User selection checkbox
} Str_flash_device;
```

### UI Elements
- **List Control**: `IDC_LIST_FLASH_MULTY`
  - Displays all devices in network
  - Checkbox for device selection
  - Shows current/target firmware versions
  - Shows firmware file paths
  - Shows config file paths

- **Buttons**:
  - "Select Firmware File" - Browse for .hex file
  - "Select Config File" - Browse for config file
  - "START" - Begin firmware update

---

## 4. USER INTERACTION PHASE

### User Selection
1. **Check devices** to update (checkbox column 0)
2. **Select firmware file** (.hex) - typically stored in `Database/Firmware/`
3. **Select configuration file** (optional) - product-specific configs
4. **Click START button** - Triggers flash process

### Button Handler: `OnBnClickedButtonStatrt()`
**Location**: [Flash_Multy.cpp:789](T3000/Flash_Multy.cpp#L789)

```cpp
// Validation
if (nflashitemcount == 0) {
    MessageBox(_T("Please select one or more items."), _T("Notice"), MB_OK | MB_ICONINFORMATION);
    return;
}

// Build device list into flash_device vector
for (int i = 0; i < ncount; i++) {
    if (!m_flash_multy_list.GetCellChecked(i, 0))
        continue;  // Skip unchecked devices
    
    Str_flash_device temp;
    temp.nitem = i;
    temp.strSN = m_flash_multy_list.GetItemText(i, FLASH_SERIAL_NUMBER);
    temp.nID = m_flash_multy_list.GetItemText(i, FLASH_ID);
    temp.ncomport = m_flash_multy_list.GetItemText(i, FLASH_COM_PORT);
    temp.nBaudrate = m_flash_multy_list.GetItemText(i, FLASH_BAUDRATE);
    temp.nIPaddress = m_flash_multy_list.GetItemText(i, FLASH_IPADDRESS);
    temp.nipport = m_flash_multy_list.GetItemText(i, FLASH_IPPORT);
    temp.devicename = m_flash_multy_list.GetItemText(i, FLASH_PRODUCT_NAME);
    temp.file_position = m_flash_multy_list.GetItemText(i, FLASH_FILE_POSITION);
    temp.config_file_position = m_flash_multy_list.GetItemText(i, FLASH_CONFIG_FILE_POSITION);
    
    temp.need_flash = true;
    flash_device.push_back(temp);  // Add to processing queue
}

// Create background thread for ISP execution
Call_ISP_Application = CreateThread(NULL, NULL, multy_isp_thread, this, NULL, NULL);
```

---

## 5. BACKGROUND THREAD - ISP EXECUTION

### Function: `multy_isp_thread()`
**Location**: [Flash_Multy.cpp:969](T3000/Flash_Multy.cpp#L969)
**Type**: Static thread function (DWORD WINAPI)

### **PHASE 1: FIRMWARE FLASHING**

```cpp
for(int i = 0; i < nflashdevicecount; i++) {
    if (flash_device.at(i).nresult != OPERATION_SUCCESS) {
        if (!flash_device.at(i).file_position.IsEmpty()) {
            
            // 1. Save configuration to INI file
            pParent->SetAutoConfig(flash_device.at(i));  // Write to AutoFlashConfig.ini
            
            // 2. Post UI update: "Running" (Blue color)
            pParent->PostMessage(WM_MULTY_FLASH_MESSAGE, CHANGE_THE_ITEM_COLOR_BLUE, 
                                 flash_device.at(i).nitem);
            
            // 3. Execute ISP.exe external tool
            CString MultyISPtool_path = ApplicationFolder + _T("\\ISP.exe");
            WinExecAndWait(MultyISPtool_path, NULL, NULL, 0);
            
            // 4. Read ISP result from INI file
            int nresult = GetPrivateProfileInt(_T("Data"), _T("Command"), 
                                               FAILED_UNKNOW_ERROR, AutoFlashConfigPath);
            
            // 5. Handle result
            if (nresult == FLASH_SUCCESS) {
                flash_device.at(i).nresult = CHANGE_THE_ITEM_COLOR_GREEN;
                pParent->PostMessage(WM_MULTY_FLASH_MESSAGE, CHANGE_THE_ITEM_COLOR_GREEN,
                                    flash_device.at(i).nitem);  // Green
                Sleep(4000);  // Wait for device reboot
            } else {
                flash_device.at(i).nresult = CHANGE_THE_ITEM_COLOR_RED;
                pParent->PostMessage(WM_MULTY_FLASH_MESSAGE, CHANGE_THE_ITEM_COLOR_RED,
                                    flash_device.at(i).nitem);  // Red
                continue;  // Skip config for failed device
            }
        }
    }
```

### **PHASE 2: CONFIGURATION FILE LOADING**

```cpp
    // Skip if config file is empty or already applied
    if (flash_device.at(i).cofnigresult == 3 || flash_device.at(i).config_file_position.IsEmpty())
        continue;
    
    // 1. Open communication (Serial OR Network)
    if (!flash_device.at(i).ncomport.IsEmpty()) {
        // Serial communication
        int comport = _wtoi(flash_device.at(i).ncomport);
        int baudrate = _wtoi(flash_device.at(i).nBaudrate);
        
        if (open_com(comport)) {
            is_connect_device = TRUE;
            Change_BaudRate(baudrate);
            SetCommunicationType(0);  // Serial mode
        }
    } else {
        // Network communication
        CString currentIp = flash_device.at(i).nIPaddress;
        int Port = _wtoi(flash_device.at(i).nipport);
        
        if (Open_Socket2(currentIp, Port)) {
            is_connect_device = TRUE;
            SetCommunicationType(1);  // Network mode
        }
    }
    
    if (!is_connect_device)
        continue;
    
    // 2. Detect product type
    Read_Multi(now_tstat_id, product_register_value, 0, 10);
    int nFlag = product_register_value[7];
    
    if (nFlag == PM_TSTAT6 || nFlag == PM_TSTAT7 || nFlag == PM_TSTAT8 || nFlag == PM_TSTAT9) {
        product_type = T3000_6_ADDRESS;
    } else if (nFlag == PM_TSTAT5E || nFlag == PM_PM5E || nFlag == PM_TSTAT5H) {
        product_type = T3000_5EH_LCD_ADDRESS;
    } else if (nFlag == PM_TSTAT5A || nFlag == PM_TSTAT5B || nFlag == PM_TSTAT5C) {
        product_type = T3000_5ABCDFG_LED_ADDRESS;
    } else if (nFlag == PM_T3PT10 || nFlag == PM_T332AI || nFlag == PM_T38AI16O) {
        product_type = T3000_T3_MODULES;
    }
    
    // 3. Load configuration file (product-specific handler)
    if(config_file.Open(config_file_path, CFile::modeRead | CFile::shareDenyNone)) {
        
        if (product_type == T3000_6_ADDRESS) {
            LoadFile2Tstat67(temppp, (LPTSTR)(LPCTSTR)config_file_path, &log_file);
        } else if (product_type == T3000_T3_MODULES) {
            LoadFile2Tstat_T3(temppp, (LPTSTR)(LPCTSTR)config_file_path, &log_file);
        } else if (product_type == PM_LightingController) {
            load_file_2_schedule_LC((LPTSTR)(LPCTSTR)config_file_path, now_tstat_id, log_file);
        } else if (product_type == PM_NC) {
            load_file_2_schedule_NC((LPTSTR)(LPCTSTR)config_file_path, now_tstat_id, log_file);
        } else {
            LoadFile2Tstat(temppp, (LPTSTR)(LPCTSTR)config_file_path, &log_file);
        }
    }
    
    // 4. Check for write errors
    if (g_Vector_Write_Error.size() > 0) {
        flash_device.at(i).nresult = CHANGE_THE_ITEM_COLOR_LESS_RED;  // Config failed
    } else {
        flash_device.at(i).nresult = CHANGE_THE_ITEM_COLOR_MORE_GREEN;  // Success
    }
    
    // 5. Close communication
    if (IS_COM) {
        SetCommunicationType(0);
        close_com();
    } else {
        SetCommunicationType(1);
        close_com();
    }
    
    // 6. Post UI update with result
    pParent->PostMessage(WM_MULTY_FLASH_MESSAGE, flash_device.at(i).nresult, 
                        flash_device.at(i).nitem);
}

// Thread complete
pParent->m_bTstatLoadFinished = TRUE;
return 0;
```

---

## 6. UI MESSAGE HANDLER

### Function: `MultyFlashMessage()`
**Location**: [Flash_Multy.cpp:1582](T3000/Flash_Multy.cpp#L1582)
**Message ID**: `WM_MULTY_FLASH_MESSAGE`

**Parameters:**
- `wParam`: Command/Status code
- `lParam`: Device list item index

### Status Codes & Colors
```cpp
// Status codes with UI colors
CHANGE_THE_ITEM_COLOR_BLUE = 1;         // Running (Blue)
CHANGE_THE_ITEM_COLOR_RED = 2;          // Failed (Red)
CHANGE_THE_ITEM_COLOR_GREEN = 3;        // Firmware Success (Green)
CHANGE_THE_ITEM_COLOR_DEFAULT = 4;      // Clear
CHANGE_THE_ITEM_COLOR_MORE_GREEN = 5;   // Firmware + Config Success (Bright Green)
CHANGE_THE_ITEM_COLOR_LESS_RED = 6;     // Firmware Success + Config Failed (Light Red)
MASS_FLASH_MESSAGE = 200;               // Device already up-to-date

// Color definitions
#define FLASH_COLOR_BLUE RGB(50,50,180)
#define FLASH_COLOR_RED RGB(255,0,0)
#define FLASH_COLOR_GREEN RGB(0,255,0)
#define CONFIG_COLOR_RED_FAIL RGB(255,86,86)
#define CONFIG_COLOR_CONFIG_FLASH_GOOD RGB(86,120,86)
```

### UI Update Logic
```cpp
switch(main_command) {
    case CHANGE_THE_ITEM_COLOR_BLUE:
        m_flash_multy_list.SetItemTextColor(sub_parameter, FLASH_RESULTS, FLASH_COLOR_BLUE);
        m_flash_multy_list.SetItemText(sub_parameter, FLASH_RESULTS, _T("Running"));
        break;
    
    case CHANGE_THE_ITEM_COLOR_GREEN:
        m_flash_multy_list.SetItemTextColor(sub_parameter, FLASH_RESULTS, FLASH_COLOR_GREEN);
        m_flash_multy_list.SetItemText(sub_parameter, FLASH_RESULTS, _T("Sucessful"));
        break;
    
    case CHANGE_THE_ITEM_COLOR_RED:
        m_flash_multy_list.SetItemTextColor(sub_parameter, FLASH_RESULTS, FLASH_COLOR_RED);
        m_flash_multy_list.SetItemText(sub_parameter, FLASH_RESULTS, _T("Fail"));
        break;
    
    case CHANGE_THE_ITEM_COLOR_MORE_GREEN:
        m_flash_multy_list.SetItemTextColor(sub_parameter, -1, CONFIG_COLOR_CONFIG_FLASH_GOOD);
        m_flash_multy_list.SetItemText(sub_parameter, FLASH_CONFIG_RESULTS, _T("Sucessful"));
        break;
    
    case CHANGE_THE_ITEM_COLOR_LESS_RED:
        m_flash_multy_list.SetItemTextColor(sub_parameter, -1, CONFIG_COLOR_RED_FAIL);
        m_flash_multy_list.SetItemText(sub_parameter, FLASH_RESULTS, _T("Sucessful"));
        m_flash_multy_list.SetItemText(sub_parameter, FLASH_CONFIG_RESULTS, _T("Fail"));
        break;
}
```

---

## 7. EXTERNAL ISP TOOL

### ISP.exe Application
- **Type**: External executable
- **Location**: Typically in `Application Folder\ISP.exe`
- **Purpose**: Writes .hex firmware directly to device hardware
- **Communication**: Via serial port or network (Modbus/BACnet protocol)

### Configuration via INI File
- **Config File Path**: `AutoFlashConfigPath`
- **INI Sections**:
  - `[Data]` - Flash commands and results
  - `[Device]` - Target device info (serial number, IP, port)
  - `[Firmware]` - .hex file path and parameters

### ISP Result Codes
```cpp
#define FLASH_SUCCESS 0             // Flash completed successfully
#define FAILED_UNKNOW_ERROR -1      // Unknown error
#define FAILED_DEVICE_NOT_FOUND -2  // Device not found
#define FAILED_FLASH_TIMEOUT -3     // Timeout during flash
```

---

## 8. CONFIGURATION FILE LOADING

### Product-Specific Loaders
Each product type has specialized config loader:

1. **TSTAT6/7/8/9**: `LoadFile2Tstat67()`
2. **TSTAT5E/5H/5G**: `LoadFile2Tstat()` (generic)
3. **T3 Modules**: `LoadFile2Tstat_T3()`
4. **Lighting Controller**: `load_file_2_schedule_LC()`
5. **Network Controller**: `load_file_2_schedule_NC()`

### Config File Format
- **Type**: Text-based configuration
- **Content**: Device registers, schedules, programs
- **Communication**: Modbus/BACnet protocol
- **Logging**: All writes logged to `Load_config_Log\<SN>.txt`

### Loading Process
1. Parse configuration file
2. Extract register/program data
3. Open serial or network communication
4. Send configuration data in Modbus packets
5. Verify writes
6. Log results and errors
7. Close communication

---

## 9. DATA PERSISTENCE

### SQLite Database Storage
- **Database File**: Current building database (`.mdb`)
- **Table**: `BatchFlashResult`
- **Function**: `ParameterSaveToDB()` - [Flash_Multy.cpp:916](T3000/Flash_Multy.cpp#L916)

### Stored Information
```sql
CREATE TABLE BatchFlashResult (
    SN INTEGER,
    FirmwarePath TEXT,
    ConfigPath TEXT,
    FirmwareResult INTEGER,
    ConfigResult INTEGER
);
```

### Mass Flash Log Files
- **Location**: `Load_config_Log\` folder
- **Files**: `<SerialNumber>.txt` for each device
- **Content**: Configuration load operations and results

### INI Files
- **ProductPath.ini**: Maps product IDs to firmware versions
- **AutoFlashConfig.ini**: Current flash job configuration
- **Mass Flash Result INI**: Status tracking for batch operations

---

## 10. KEY GLOBAL VARIABLES

```cpp
// Device list for current flash batch
vector <Str_flash_device> flash_device;

// Download info for cloud-based firmware
vector <Str_download_firmware_info> download_info_type;

// Thread handles
HANDLE Call_ISP_Application;           // ISP thread handle
HANDLE Check_Online_Thread;            // Device online monitor

// Paths
CString ApplicationFolder;              // T3000 executable folder
CString MultyISPtool_path;             // Path to ISP.exe
CString AutoFlashConfigPath;           // Auto flash config INI path
CString g_ext_mass_flash_path;         // Mass flash result INI

// Global state
bool b_pause_refresh_tree;             // Pause tree refresh during flash
bool g_bPauseMultiRead;                // Pause multi-device reads
int multy_log_count;                   // Log counter
```

---

## 11. ERROR HANDLING

### Common Failure Scenarios

1. **No Device Selected**
   - Message: "Please select one or more items."
   - Recovery: User selects devices and retries

2. **Device Not Online**
   - Device marked with red "Offline" indicator
   - Skipped during flash process

3. **ISP Execution Failed**
   - Result shows RED "Fail"
   - Config phase skipped for that device

4. **Configuration Write Failed**
   - Status: RED "Less_Red" (firmware OK, config failed)
   - Logged to `<SN>.txt`
   - Device left with new firmware but old config

5. **Communication Port In Use**
   - ISP tool expects free COM/network port
   - T3000 closes all ports before flashing

### Error Logging
- **Log Location**: `<Application Folder>\Load_config_Log\<SN>.txt`
- **Log Content**: All register writes, errors, and completion status
- **Purpose**: Troubleshooting failed flash operations

---

## 12. FILE LOCATIONS & PATHS

### Key Directories
```
T3000 Installation Folder/
├── T3000.exe                          (Main application)
├── ISP.exe                            (Firmware flash tool)
├── Database/
│   ├── Firmware/
│   │   ├── *.hex                      (Firmware files)
│   │   ├── *.ini                      (Config files)
│   │   └── ProductPath.ini            (Product-to-firmware mapping)
│   └── Buildings/
│       └── <CurrentBuilding>.mdb      (SQLite database)
├── Load_config_Log/                   (Configuration logs)
├── AutoFlashConfig.ini               (Current flash config)
└── LoadFirmware.ini                   (Mass flash results)
```

---

## 13. PERFORMANCE & TIMING

### Typical Operation Times
- **Dialog Initialization**: 2-5 seconds (scans all devices)
- **Per Device Firmware Flash**: 30-60 seconds (via ISP.exe)
- **Device Reboot Wait**: 4 seconds (Sleep timer)
- **Per Device Config Load**: 5-15 seconds (register writes)
- **Total for 1 Device**: 40-80 seconds
- **Total for 10 Devices**: 6-15 minutes

### Thread Management
- **Main Thread**: UI handling, dialog management
- **ISP Thread**: Sequential device processing (not parallel)
- **Monitor Thread**: (Optional) Device online status checking

---

## 14. SUMMARY OF FILES INVOLVED

| File | Purpose |
|------|---------|
| `MainFrm.cpp` | Main window, entry point handler |
| `Flash_Multy.cpp` | Dialog UI, thread management |
| `Flash_Multy.h` | Data structures, dialog definition |
| `Dowmloadfile.cpp` | Cloud firmware download |
| `global_function.cpp` | ISP execution, com port management |
| `T3000.rc` | Menu resource, keyboard shortcut |
| `resource.h` | Resource IDs |
| `T3000.mdb` | SQLite database |

---

## 15. QUICK REFERENCE - FUNCTION CALL SEQUENCE

```
User: Ctrl+F2 or Tools→Load Firmware
    ↓
MainFrm::OnBatchFlashHex()
    ├─ Pause communications
    ├─ Close serial/network
    ├─ CFlash_Multy::DoModal()
    │   ├─ OnInitDialog()
    │   │   ├─ Initial_List()
    │   │   ├─ GetProductType()
    │   │   └─ Get_Device_Firmware()
    │   ├─ [User selects devices & files]
    │   └─ OnBnClickedButtonStatrt()
    │       └─ CreateThread(multy_isp_thread)
    │           ├─ Phase 1: Firmware Flash
    │           │   ├─ SetAutoConfig()
    │           │   ├─ WinExecAndWait(ISP.exe)
    │           │   └─ Check result → PostMessage()
    │           └─ Phase 2: Config Load
    │               ├─ Open communication
    │               ├─ Load product-specific config
    │               ├─ Write to device
    │               └─ PostMessage()
    ├─ MultyFlashMessage() → Update UI colors
    └─ Restore communications & exit
```

---

**Generated**: 2024 T3000 Building Automation System
**Version**: Complete firmware update flow documentation
