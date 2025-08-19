// ISPDlg.cpp : implementation file
// 5.1.0  Added new product: weather station
// 5.0.9  Optimize Network writing speed
// 4.7.1 New method for retrieving serial numbers

#include "stdafx.h"
#include "ISP.h"
#include "ISPDlg.h"
#include "global_function.h"
#include "JPGLoader.h"
#include "afxwin.h"
#include "Splash.h"
#include "HexFileParser.h"
#include "BinFileParser.h"
#include "ComWriter.h"
#include "TFTPServer.h"
#include "FlashSN.h"
#include "ISPSetting.h"
#include <bitset>
#include "../T3000/Class/md5.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "BADO\BADO.h"
//Determine whether to use MSTP to update firmware
extern int SPECIAL_BAC_TO_MODBUS; //�ж��Ƿ���MSTP�����¹̼�;
unsigned char ready_towrite_mac[6] = { 0 };
unsigned int ready_towrite_sn = 0;
#ifdef ISP_BURNING_MODE
int burning_test_finished = 1;
HANDLE h_BurningTest_thread = NULL;
#endif
unsigned char firmware_md5[32] = { 0 };
int g_write_mac = 0;
int g_write_wifi_mac = 0;
HANDLE h_sn_mac_thread = NULL;

int g_sn_product_id = 0;
int g_write_hardware_version = 0;
CString sn_mac_info;
int g_comport = 0;
int g_mac_baudrate = 0;
CString g_mac_ip;
int g_mac_port;
int b_com_or_ip = 0;
int n_mac_reboot_time = 15;
int n_need_write_pid = 0;
vector <int> support_sensor_id;

extern Bin_Info        global_fileInfor;
//int g_CommunicationType;
unsigned int the_max_register_number_parameter_Count=0;
unsigned int the_max_register_number_parameter_Finished=0;
CString c_strSNRecordFileName = _T("Z:\\Serial_Records\\serial_records.txt");
bool auto_flash_mode = false;
CString AutoFlashConfigPath;
CString g_strExePath;
CString SettingPath;
CString g_repair_bootloader_file_path;
//Latest normal code path to be burned
CString g_update_newfirmware_file_path; //���µ�Ҫ��д�������Ĵ���·��;
//Hex file product PID
unsigned int n_hex_file_pid;//hex�ļ��Ĳ�ƷPID��
//0 No need to update boot   1 Need to update bootloader;   C1 is hex
int firmware_must_use_new_bootloader = 0;  //0 ���ø���boot   1 ��Ҫ����bootload;   C1Ϊhex
//If equals 1, it means we are now burning the new BootLoader
int new_bootload = 0; //�������1 ��˵��������д�����µ�BootLoader;
// 0 Normal mode   1 Burn boot mode    2 Burn normal mode code again
int com_port_flash_status = 0;  // 0 ����ģʽ   1 ��дbootģʽ    2 �ٴ���д����ģʽ�Ĵ���
//HANDLE get_file_thread_handle = NULL;
CString g_strFlashInfo;
const TCHAR c_strLogFileName[]=_T("Log_info.txt");				//log information file name
const TCHAR c_strDBFileName[]=_T("Database\\t3000.mdb") ;
//extern CString g_strExePath;
//const int BOOTLOADER_FILE_SIZE = 16384;
/*���������Ȼ��DLL�еı�����ͬ��������������û�й�ϵ��DLL�еĸñ����Ǳ���װ������ - This variable is the same as the variable in the DLL, but they are not related. The variable in the DLL is encapsulated.
�����ı�dll�е�g_Commu_type ͨ������ SetCommunicationType- If you want to change g_Commu_type in the DLL, use the function SetCommunicationType
*/ 
/* These variables are the same as the variables in the communications DLL but they are separate, the variables in the DLL are encapsulated. If you want to change the G_commu_type in the DLL use the function Setcommunicationtype */
 

int handle_write_sensor_info = 0;
//Number of all checked components
int n_component_count = 0; //���й�ѡ������ĸ���
//Value of the first 16 bits
unsigned short sensor_value1 = 0; //ǰ16λ��ֵ
//Value of the last 16 bits, reserved
unsigned short sensor_value2 = 0; //��16λ��ֵ �� Ԥ��
unsigned short FUNCTION_CHECK_SUM_REG = 65000;
unsigned short FUNCTION_CHECK_SUM_VALUE = 0x55;
unsigned short FUNCTION_SENSOR1_REG = 65001;
unsigned short FUNCTION_SENSOR2_REG = 65002;

int g_Commu_type=0;
UINT _PingThread(LPVOID pParam);
unsigned int com_error_delay_time = 0;
unsigned int com_error_delay_count = 0;
unsigned int n_check_temco_firmware = 1;
unsigned int Remote_timeout = 1000;
unsigned int nflash_receive_to_send_delay_time = 10;
CString Auto_flash_SN_connect_IP;
DWORD WINAPI GetFWFileProc(LPVOID lPvoid);
#define		WM_CLOSE_THREAD_MESSAGE				WM_USER + 2018
#define NUMBER_BAUDRATE 6
#define SN_MAC_SHOW_TIMER 4
#define TEST_ISP_TIMER 10
const int TEST_FINISHED_WAIT_TIME = 30;

const CString c_strBaudate[NUMBER_BAUDRATE] =
{
    _T("9600"),
    _T("19200"),
    _T("38400"),
    _T("57600"),
    _T("76800"),
    _T("115200")
};
class CAboutDlg : public CDialog
{
public:
    CAboutDlg();
    virtual ~CAboutDlg(void);

// Dialog Data
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    int ShowLogo();
    JPGLoader* m_pPicLoader;
// Implementation



protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
public:
    CStatic m_logoPic;
    //afx_msg void OnMainClear();
    afx_msg void OnBnClickedOk();
    virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    m_pPicLoader = NULL;
}
CAboutDlg::~CAboutDlg()
{
    /*if (m_pPicLoader)
    {
    	delete m_pPicLoader;
    }*/
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LOGOPIC, m_logoPic);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    ON_WM_PAINT()
    //ON_COMMAND(ID_MAIN_CLEAR, &CAboutDlg::OnMainClear)
    ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()

int CAboutDlg::ShowLogo()
{
    CString strPath = GetExePath(true);
    strPath = strPath + c_strLogoFileName;
    if (!m_pPicLoader)
    {
        m_pPicLoader = new JPGLoader;
    }

    CWnd* pWnd = (CWnd*)(&m_logoPic);

    ASSERT(pWnd);
    m_pPicLoader->SetDisplayWnd(pWnd);

    m_pPicLoader->SetJPGFileName(strPath);

    m_pPicLoader->LoadPic();
    int nRet = m_pPicLoader->ShowJPG();

    //pWnd->Invalidate(TRUE);

    //UpdateData(TRUE);

    return nRet;

}

void CAboutDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call CDialog::OnPaint() for painting messages
    ShowLogo();
}
// CISPDlg dialog




CISPDlg::CISPDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CISPDlg::IDD, pParent)
    , m_ID(_T(""))
    , m_ModelName(_T(""))
    ,m_FirmVer(_T(""))
    , m_HardVer(_T(""))

    , m_IPPort(0)


{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    //m_nTabSel = 0;
    m_bShowSN = FALSE;
    m_enable_sn_mac = false;
    FLASH_SUBID=FALSE;
    m_pComWriter = NULL;
    m_pTFTPServer = NULL;
    m_plogFile=new CStdioFile;
    m_pTCPFlasher = NULL;
    m_pFileBuf = NULL;

    m_FlashEnable = TRUE;

 
}
CISPDlg::~CISPDlg()
{
    if (m_pComWriter)
    {
        delete m_pComWriter;
        m_pComWriter = NULL;
    }
    if (m_pTFTPServer)
    {
        delete m_pTFTPServer;
        m_pTFTPServer = NULL;
    }
    if (m_plogFile)
    {
        delete m_plogFile;
        m_plogFile=NULL;
    }
    if(m_pTCPFlasher)
    {
        delete m_pTCPFlasher;
    }
    if (m_pFileBuf)
    {
        delete m_pFileBuf;
    }

    if (m_pFileBuffer)
    {
        delete[] m_pFileBuffer;
        m_pFileBuffer = NULL;
    }
  
}

void CISPDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_INFO, m_lbStatusInfo);
    //DDX_Control(pDX, IDC_TAB1, m_tab);

    DDX_Control(pDX, IDC_EDIT_MDBID1, id);
    DDX_Control(pDX, IDC_COMBO_COM, m_ComPort);
    DDX_Control(pDX, IDC_IPADDRESS_NC, m_ipAddress);
    DDX_Control(pDX, IDC_EDIT_NCPORT, m_ipPort);
    DDX_Control(pDX, IDC_CHECK_FLASH_SUBID, m_Check_SubID);
    DDX_Control(pDX, IDC_EDIT_MDBID2, m_SubID);
    DDX_Text(pDX, IDC_EDIT_MDBID1, m_ID);
    DDX_Control(pDX, IDC_BUTTON_PING2, m_Btn_ping);
    DDX_Text(pDX, IDC_Model_Name, m_ModelName);
    DDX_Text(pDX, IDC_Model_FIRMVER, m_FirmVer);
    DDX_Text(pDX, IDC_Model_HARDWAREVER, m_HardVer);
    DDX_Text(pDX, IDC_EDIT_NCPORT, m_IPPort);
    //  DDX_Text(pDX, IDC_BIN_INFORMATION, m_bin_inforamtion);
    //  DDX_Text(pDX, IDC_EDIT_FILEPATH2, m_strFirmwarepath);
    DDX_Control(pDX, IDC_EDIT_BAUDRATE, m_combox_baudrate);
    DDX_Control(pDX, IDC_STATIC_SN_INFO, m_static_info);
}

BEGIN_MESSAGE_MAP(CISPDlg, CDialog)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_SELFILE, &CISPDlg::OnBnClickedButtonSelfile)
    ON_BN_CLICKED(IDC_BUTTON_FLASH, &CISPDlg::OnBnClickedButtonFlash)
    //wether repleace the line information
    ON_MESSAGE(WM_REPLACE_STATUSINFO, OnReplaceStatusInfo)
    ON_MESSAGE(WM_ADD_STATUSINFO, OnAddStatusInfo)
    //Finish ֪ͨ Flash��ť�Ƿ����
    ON_MESSAGE(WM_FLASH_FINISH, OnFlashFinish)
    ON_MESSAGE(WM_FLASH_RESTATR_BOOT, OnFlashBoot_Update_boot)
    ON_MESSAGE(WM_FLASH_NEW_BOOT_FINISH, OnFlashNewBootFinish)
    
    ON_MESSAGE(WM_UPDATA_DEVICE_INFORMATION,Show_Flash_DeviceInfor)
    ON_WM_CLOSE()
    //ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CISPDlg::OnTcnSelchangeTab1)
    ON_WM_COPYDATA()
    ON_BN_CLICKED(IDC_COM, &CISPDlg::OnBnClickedCom)
    ON_BN_CLICKED(IDC_NET, &CISPDlg::OnBnClickedNet)
    ON_BN_CLICKED(IDC_CHECK_FLASH_SUBID, &CISPDlg::OnBnClickedCheckFlashSubid)

    ON_BN_CLICKED(IDC_BUTTON_PING2, &CISPDlg::OnBnClickedButtonPing2)

    ON_COMMAND(ID_MAIN_CLEAR, &CISPDlg::OnMainClear)
    ON_COMMAND(ID_SAVE_LOG_INFO, &CISPDlg::OnSaveLogInfo)
    ON_BN_CLICKED(IDC_CLEAR_LOG, &CISPDlg::OnBnClickedClearLog)
    ON_BN_CLICKED(IDC_SAVE_LOG, &CISPDlg::OnBnClickedSaveLog)

    ON_WM_CONTEXTMENU()

    ON_COMMAND(ID_MENU_APP, &CISPDlg::OnMenuApp)
    ON_COMMAND(ID_MENU_ABOUT, &CISPDlg::OnMenuAbout)
    ON_COMMAND(ID_MENU_SETTING, &CISPDlg::OnMenuSetting)
    //ON_COMMAND(ID_MENU_CHECKHEX, &CISPDlg::OnMenuCheckhex)
    ON_WM_TIMER()

    ON_BN_CLICKED(IDC_BUTTON_FLASH_SN, &CISPDlg::OnBnClickedButtonFlashSn)

    ON_MESSAGE(WM_CLOSE_THREAD_MESSAGE, &CISPDlg::Fresh_CloseSN_fcuntion)
    ON_CBN_SELCHANGE(IDC_COMBO_PM, &CISPDlg::OnCbnSelchangeComboPm)
    ON_BN_CLICKED(IDC_CHECK_NO_ITEM, &CISPDlg::OnBnClickedCheckNoItem)
#ifdef ISP_BURNING_MODE
    ON_BN_CLICKED(IDC_BUTTON_LOOP_FLASH, &CISPDlg::OnBnClickedButtonLoopFlash)
#endif
END_MESSAGE_MAP()


// CISPDlg message handlers
BOOL CISPDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    // Add "About..." menu item to system menu.
    CString filename;
    CString flashmethod;
    CString id;
    CString comport;
    CString BD;
    CString ip;
    CString ipport;

    CString subnote;
    CString subID;
    Send_WhoIs_Global(-1, -1);
    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);		// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    for (int i = 0; i<NUMBER_BAUDRATE; i++)
    {
        m_combox_baudrate.AddString (c_strBaudate[i]);
    }
    // TODO: Add extra initialization here
    CString ApplicationFolder;
    GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
    PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
    ApplicationFolder.ReleaseBuffer();


    AutoFlashConfigPath = ApplicationFolder + _T("//AutoFlashFile.ini");
    CFileFind fFind;



    g_strExePath=GetExePath(true);
    m_strLogoFileName=g_strExePath + _T("ResourceFile\\") + c_strLogoFileName;
    SettingPath = g_strExePath + _T("\\Setting.ini");
    //g_repair_bootloader_file_path = g_strExePath + _T("ResourceFile\\HexFile\\repair_bootloader_BB_rev1.hex");
    //���ļ��ж�ȡ���ò���
    m_cfgFileHandler.SetParentWnd(this);
    // m_cfgFileHandler.CreateConfigFile(g_strExePath + c_strCfgFileName);
    m_cfgFileHandler.CreateConfigFile();
    m_strCfgFilePath = g_strExePath + c_strCfgFileName;



    m_cfgFileHandler.ReadFromCfgFileForAll(
        filename,
        flashmethod,
        id,
        comport,
        BD,
        ip,
        ipport,
        subnote,
        subID
    );

    n_check_temco_firmware = GetPrivateProfileInt(_T("Setting"), _T("Check_Temco_Firmware"), 1, SettingPath);
    if (n_check_temco_firmware == 1)
    {
        WritePrivateProfileStringW(_T("Setting"), _T("Check_Temco_Firmware"), _T("1"), SettingPath);
    }

    //com_error_delay_time = GetPrivateProfileInt(_T("Setting"), _T("ComPortFlashErrorDelay"), 300, SettingPath);
    //com_error_delay_count = GetPrivateProfileInt(_T("Setting"), _T("ComPortFlashErrorCount"), 3, SettingPath);
    //if (com_error_delay_time == 300)
    //{
    //    WritePrivateProfileStringW(_T("Setting"), _T("ComPortFlashErrorDelay"), _T("300"), SettingPath);
    //    WritePrivateProfileStringW(_T("Setting"), _T("ComPortFlashErrorCount"), _T("1"), SettingPath);
    //}



    Remote_timeout = GetPrivateProfileInt(_T("Setting"),_T("REMOTE_TIMEOUT"),1000,SettingPath);
    if(Remote_timeout < 50)
        Remote_timeout = 1000;

    nflash_receive_to_send_delay_time = GetPrivateProfileInt(_T("Setting"), _T("ResponseTime"), 10, SettingPath);

    m_enable_sn_mac = GetPrivateProfileInt(_T("Setting"), _T("LocalTemcoSN"), 0, SettingPath);
    if (!m_enable_sn_mac)
    {
        WritePrivateProfileStringW(_T("Setting"), _T("LocalTemcoSN"), _T("0"), SettingPath);
    }
    else
    {

    }
    

    GetPrivateProfileStringW(_T("Setting"),_T("DB_IPADDRESS"),_T("NULL"),Auto_flash_SN_connect_IP.GetBuffer(MAX_PATH),MAX_PATH,SettingPath);
    Auto_flash_SN_connect_IP.ReleaseBuffer();
    if(Auto_flash_SN_connect_IP.CompareNoCase(_T("NULL")) == 0)
    {
        WritePrivateProfileStringW(_T("Setting"),_T("DB_IPADDRESS"),_T("192.168.0.202"),SettingPath);
        Auto_flash_SN_connect_IP = _T("192.168.0.202");
    }
    if(fFind.FindFile(AutoFlashConfigPath))
    {

        int command = GetPrivateProfileInt(_T("Data"),_T("Command"),0,AutoFlashConfigPath);
        if(command != START_AUTO_FLASH_COMMAND)
        {
            //DeleteFile(AutoFlashConfigPath);
            //MessageBox(_T("ISP Tool will exit soon."));
            auto_flash_mode = false;
            DeleteFile(AutoFlashConfigPath);
        }
        else
        {
            ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW); //This line changes the properties of the window;
            SetWindowPos(NULL,0,0,0,0,0);
            auto_flash_mode = true;
            GetPrivateProfileStringW(_T("Data"),_T("ID"),_T("255"),id.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
            id.ReleaseBuffer();
            GetPrivateProfileStringW(_T("Data"),_T("COM_OR_NET"),_T("COM"),flashmethod.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
            flashmethod.ReleaseBuffer();
            GetPrivateProfileStringW(_T("Data"),_T("COMPORT"),_T("COM1"),comport.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
            comport.ReleaseBuffer();
            GetPrivateProfileStringW(_T("Data"),_T("Baudrate"),_T("19200"),BD.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
            BD.ReleaseBuffer();

            GetPrivateProfileStringW(_T("Data"),_T("ID"),_T("255"),id.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
            id.ReleaseBuffer();

            GetPrivateProfileStringW(_T("Data"),_T("IPAddress"),_T("192.168.0.3"),ip.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
            ip.ReleaseBuffer();
            GetPrivateProfileStringW(_T("Data"),_T("IPPort"),_T("502"),ipport.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
            ipport.ReleaseBuffer();
            GetPrivateProfileStringW(_T("Data"),_T("Subnote"),_T("0"),subnote.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
            subnote.ReleaseBuffer();
            GetPrivateProfileStringW(_T("Data"),_T("SubID"),_T("255"),subID.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
            subID.ReleaseBuffer();
            GetPrivateProfileStringW(_T("Data"),_T("FirmwarePath"),_T(""),filename.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
            filename.ReleaseBuffer();
        }

    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Show splash screen


    if(!auto_flash_mode)
    {
        ShowSplashWnd(c_nSplashTime);
    }

    ////////////////////////////////////////////////////////////////////////////


    ////g_Commu_type=0;	//Initial write for COM port
    //((CButton *)GetDlgItem(IDC_COM))->SetCheck(TRUE);
    if (flashmethod.CompareNoCase(L"COM")==0)
    {
        COM_INPUT = TRUE;
    }
    else
    {
        COM_INPUT=FALSE;
    }


   
    FLASH_SUBID=FALSE;
    //Class COM��
    InitCombox();
    //Initialization of log files
    //Path to log file
    m_strLogFileName=g_strExePath+c_strLogFileName;
    if (!m_plogFile->Open(m_strLogFileName.GetString(),CStdioFile::modeRead))
    {
        if(m_plogFile->Open(m_strLogFileName.GetString(),CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate ))
        {
        }
        else
        {
            CString strTips = _T("|Cannot create a new Log file. Please try again.");
            UpdateStatusInfo(strTips, FALSE);
            m_plogFile->Close();
        }
    }
    m_plogFile->Close();
    //get db path;
    CString DB_T=_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=");
    m_strDatabasefilepath=DB_T+g_strExePath+c_strDBFileName;


    m_ToolTip= new CToolTipCtrl();
    m_ToolTip->Create(this);
    //ID
    CWnd* pW_ID=GetDlgItem(IDC_EDIT_MDBID1);
    m_ToolTip->AddTool(pW_ID,_T("You want to input more than one IDs,using ';'as a seperator.250;251;252;253"));
    //IDC_COMBO_COM
    CWnd* pW_COMBO_COM=GetDlgItem(IDC_COMBO_COM);
    m_ToolTip->AddTool(pW_COMBO_COM,_T("Please Select a COM Port"));
    //IDC_EDIT_BAUDRATE
    CWnd* pW_BAUDRATE=GetDlgItem(IDC_EDIT_BAUDRATE);
    m_ToolTip->AddTool(pW_BAUDRATE,_T("This baudrate is default:19200"));
    //IDC_IPADDRESS_NC
    //CWnd* pW_IPADDRESS=GetDlgItem(IDC_IPADDRESS_NC);
    //m_ToolTip->AddTool(pW_IPADDRESS,_T("This is IP for NC or Light Controller"));
    ////IDC_EDIT_NCPORT
    //CWnd* pW_NCPORT=GetDlgItem(IDC_IPADDRESS_NC);
    //m_ToolTip->AddTool(pW_NCPORT,_T("Input the port responding to IP"));
    ////IDC_CHECK_FLASH_SUBID
    //CWnd* pW_FLASH_SUBID=GetDlgItem(IDC_CHECK_FLASH_SUBID);
    //m_ToolTip->AddTool(pW_NCPORT,_T("Flash Sub node thought NC or LC."));
    //IDC_EDIT_MDBID2
    CWnd* pW_EDIT_MDBID2=GetDlgItem(IDC_EDIT_MDBID2);
    m_ToolTip->AddTool(pW_EDIT_MDBID2,_T("Input more then one IDs of the Sub nodes,using ';'as a seperator.250;251;252;253"));

    m_ToolTip->Activate(TRUE);


    CString TitleShow;
    CString m_strVersion;
    m_strVersion.LoadString(IDS_STR_VIRSIONNUM);
    TitleShow = _T("ISP Tool ") + m_strVersion;
    SetWindowTextW(TitleShow);

#ifdef ISP_BURNING_MODE
    GetDlgItem(IDC_BUTTON_LOOP_FLASH)->EnableWindow(1);
#endif

#ifndef ISP_BURNING_MODE
    GetDlgItem(IDC_BUTTON_LOOP_FLASH)->EnableWindow(0);
#endif // DEBUG


    if(IsDlgButtonChecked(IDC_CHECK_FLASH_SUBID)==1)
    {
        GetDlgItem(IDC_EDIT_MDBID2)->EnableWindow(1);
        FLASH_SUBID=TRUE;
    }
    else
    {
        GetDlgItem(IDC_EDIT_MDBID2)->EnableWindow(0);
        FLASH_SUBID=FALSE;
    }

    InitISPUI();

    if(!auto_flash_mode)
    {
        //initFlashSN();
    }
    else
    {

        SetTimer(1,200,NULL);
    }
    GetDlgItem(IDC_EDIT_NCPORT)->SetWindowText(ipport);
    GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(filename);

    // GetDlgItem(IDC_EDIT_BAUDRATE)->SetWindowText(_T("19200"));
    GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(filename);
    GetDlgItem(IDC_EDIT_MDBID1)->SetWindowText(id);
    GetDlgItem(IDC_COMBO_COM)->SetWindowText(comport);
    GetDlgItem(IDC_EDIT_BAUDRATE)->SetWindowText(BD);
    m_ipAddress.SetWindowText(ip);
    GetDlgItem(IDC_EDIT_NCPORT)->SetWindowText(ipport);
    GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(filename);


    /////////////////
    if(flashmethod.Find(_T("COM"))!=0)
    {

        g_Commu_type=1;
        COM_INPUT=FALSE;
        ((CButton *)GetDlgItem(IDC_COM))->SetCheck(FALSE);
        ((CButton *)GetDlgItem(IDC_NET))->SetCheck(TRUE);

        ((CButton *)GetDlgItem(IDC_CHECK_MSTP_UPDATE))->SetCheck(false);
        GetDlgItem(IDC_CHECK_MSTP_UPDATE)->EnableWindow(0);
    }
    else
    {
        COM_INPUT=TRUE;
        g_Commu_type=0;

        ((CButton *)GetDlgItem(IDC_COM))->SetCheck(TRUE);
        ((CButton *)GetDlgItem(IDC_NET))->SetCheck(FALSE);

        ((CButton *)GetDlgItem(IDC_CHECK_MSTP_UPDATE))->SetCheck(false);
        GetDlgItem(IDC_CHECK_MSTP_UPDATE)->EnableWindow(1);
    }
    if(subnote.CompareNoCase(_T("1"))==0)
    {
        ((CButton *)GetDlgItem(IDC_CHECK_FLASH_SUBID))->SetCheck(1);
        GetDlgItem(IDC_EDIT_MDBID2)->EnableWindow(1);
        FLASH_SUBID = TRUE;
    }
    else
    {
        ((CButton *)GetDlgItem(IDC_CHECK_FLASH_SUBID))->SetCheck(0);
        GetDlgItem(IDC_EDIT_MDBID2)->EnableWindow(0);
        FLASH_SUBID = FALSE;
    }
    GetDlgItem(IDC_EDIT_MDBID2)->SetWindowText(subID);

	 COM_NET_Set_ReadOnly();

// 	 CWnd* pEditFilePath = (CWnd*)GetDlgItem(IDC_EDIT_FILEPATH);
// 	 pEditFilePath->SetWindowText(filename);
// 	 int ret=Judge_BinHexFile(filename);
// 	 if (ret==0)
// 	 {
// 		 pEditFilePath->SetWindowText(_T(""));
// 		 return TRUE;
// 	 }
// 	 m_strHexFileName=filename;
// 	 ShowHexBinInfor(ret);
// 	 GetDlgItem(IDC_EDIT_FILEPATH)->GetWindowText(m_strHexFileName);
// 	 ShowHexBinInfor();
    return TRUE;  // return TRUE  unless you set the focus to a control
}


void CISPDlg::CheckAutoFlash()
{

}

void CISPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

struct ALL_PRODUCT_NAME_NODE {
    int id;
    CString name;
    int need_write_id;
};

vector <ALL_PRODUCT_NAME_NODE> pid_name_map;

CString cs_component_name[32];
void CISPDlg::ShowProductNameFromIni()
{
    m_static_info.SetWindowTextW(_T(""));
    m_static_info.textColor(RGB(255, 255, 255));
    m_static_info.bkColor(RGB(0, 0, 255));
    m_static_info.setFont(26, 18, NULL, _T("Arial"));

    CString AllID;

    GetPrivateProfileStringW(_T("ProductName"), _T("AllName"), _T(""), AllID.GetBuffer(MAX_PATH*10), MAX_PATH*10, _T("Z:\\Serial_Records\\ProductMode.ini"));

    AllID.ReleaseBuffer();

    if (AllID.IsEmpty())
    {
        return;
    }
    CStringArray temp_array;
    SplitCStringA(temp_array, AllID, _T(","));
    pid_name_map.clear();
    ((CComboBox *)GetDlgItem(IDC_COMBO_PM))->ResetContent();


    //*******************************************************************************************************************
    CString AllNeedChooseProduct;
    GetPrivateProfileStringW(_T("ProductName"), _T("NeedChooseProduct"), _T(""), AllNeedChooseProduct.GetBuffer(MAX_PATH * 10), MAX_PATH * 10, _T("Z:\\Serial_Records\\ProductMode.ini"));
    AllNeedChooseProduct.ReleaseBuffer();

    vector <int> temp_need_write_id;
    CStringArray temp_need_write_array;
    SplitCStringA(temp_need_write_array, AllNeedChooseProduct, _T(","));
    for (int i = 0; i < temp_need_write_array.GetSize(); i++)
    {
        CString temp_cs;
        temp_cs = temp_need_write_array.GetAt(i);
        int temp_int = 0;
        temp_int = _wtoi(temp_cs);
        temp_need_write_id.push_back(temp_int);
    }


    for (int i = 0; i < temp_array.GetSize(); i++)
    {
        CString temp_section;
        temp_section = temp_array.GetAt(i);
        CString cs_name;
        GetPrivateProfileStringW(_T("ProductName"), temp_section, _T(""), cs_name.GetBuffer(MAX_PATH), MAX_PATH, _T("Z:\\Serial_Records\\ProductMode.ini"));
        cs_name.ReleaseBuffer();
        ALL_PRODUCT_NAME_NODE temp_value;
        temp_value.id = _wtoi(temp_array.GetAt(i));
        temp_value.name = cs_name;
        temp_value.need_write_id = 0;
        for (int j = 0; j < temp_need_write_id.size(); j++)
        {
            if (temp_value.id == temp_need_write_id.at(j))
            {
                temp_value.need_write_id = 1;
                break;
            }
        }

        
        pid_name_map.push_back(temp_value);
        ((CComboBox *)GetDlgItem(IDC_COMBO_PM))->AddString(cs_name);
    }


    CString nsection;
    n_component_count = GetPrivateProfileInt(_T("ComponentType"), _T("Count"), 0,  _T("Z:\\Serial_Records\\ProductMode.ini"));
    GetDlgItem(IDC_STATIC_COM_NAME)->SetWindowTextW(_T("�빴ѡ���������"));
    
    CString AllSupportProduct_Sensor;
    GetPrivateProfileStringW(_T("ComponentType"), _T("SupportProduct"), _T(""), AllSupportProduct_Sensor.GetBuffer(MAX_PATH * 10), MAX_PATH * 10, _T("Z:\\Serial_Records\\ProductMode.ini"));
    AllSupportProduct_Sensor.ReleaseBuffer();
    support_sensor_id.clear();
    CStringArray temp_support_sensor_id_array;
    SplitCStringA(temp_support_sensor_id_array, AllSupportProduct_Sensor, _T(","));
    for (int i = 0; i < temp_support_sensor_id_array.GetSize(); i++)
    {
        CString temp_cs;
        temp_cs = temp_support_sensor_id_array.GetAt(i);
        int temp_int = 0;
        temp_int = _wtoi(temp_cs);
        support_sensor_id.push_back(temp_int);
    }

    GetDlgItem(IDC_CHECK_NO_ITEM)->SetWindowTextW(_T("0.NULL �޶���ģ��"));
    //Part 1 ��1���Ĵ����� 16�� ģ��;
    for (int i = 0; i < 16; i++)
    {
        if (i >= n_component_count)
        {
            GetDlgItem(IDC_CHECK_ITEM1 + i)->ShowWindow(SW_HIDE);
            continue;
        }
        cs_component_name[i].Empty();
        nsection.Format(_T("Name_%u"),i);
        GetPrivateProfileStringW(_T("ComponentType"), nsection, _T(""), cs_component_name[i].GetBuffer(MAX_PATH ), MAX_PATH , _T("Z:\\Serial_Records\\ProductMode.ini"));
        cs_component_name[i].ReleaseBuffer();
        GetDlgItem(IDC_CHECK_ITEM1 + i)->SetWindowTextW(cs_component_name[i]);
    }


    //Part 2 ��2���Ĵ����� 16�� ģ��; - Part 2 of the 16 modules for the second register
    for (int i = 0; i < 16; i++)
    {
        if (i+16 >= n_component_count)
        {
            GetDlgItem(IDC_CHECK_ITEM17 + i)->ShowWindow(SW_HIDE);
            continue;
        }
        cs_component_name[i+16].Empty();
        nsection.Format(_T("Name_%u"), i+16);
        GetPrivateProfileStringW(_T("ComponentType"), nsection, _T(""), cs_component_name[i+16].GetBuffer(MAX_PATH), MAX_PATH, _T("Z:\\Serial_Records\\ProductMode.ini"));
        cs_component_name[i+16].ReleaseBuffer();
        GetDlgItem(IDC_CHECK_ITEM17 + i)->SetWindowTextW(cs_component_name[i+16]);
    }

    


}

void CISPDlg::InitISPUI()
{
    CenterWindow(this);
    if(m_enable_sn_mac)
    //if(m_bShowSN)
    {
        Enable_Sensor_Window(false);
        ShowProductNameFromIni();
        WINDOWPLACEMENT wp;

        GetWindowPlacement(&wp);
        GetDlgItem(IDC_CHECK_MSTP_UPDATE)->ShowWindow(1); //��ʱ����ʾ mstp�� update��ť���豸��������֧��; - Temporarily not displaying the MSTP update button, as the device hardly supports it.

        CRect rc;
        CWnd* pWnd = GetDlgItem(IDC_STATIC_SEPERATOR);
        pWnd->GetWindowRect(&rc);
        if(n_component_count>16)
            wp.rcNormalPosition.right = rc.right + 500;
        else
            wp.rcNormalPosition.right = rc.right +250;
        wp.rcNormalPosition.bottom = rc.bottom + 150;

        SetWindowPlacement(&wp);

        GetDlgItem(IDC_STATIC_SEPERATOR)->ShowWindow(SW_NORMAL);
    }
    else
    {
        WINDOWPLACEMENT wp;
        GetWindowPlacement(&wp);
        GetDlgItem(IDC_CHECK_MSTP_UPDATE)->ShowWindow(0);
        CRect rc;
        CWnd* pWnd = GetDlgItem(IDC_STATIC_SEPERATOR);
        pWnd->GetWindowRect(&rc);
        //ScreenToClient(&rc);
        if (!auto_flash_mode)
        {
            wp.rcNormalPosition.right = rc.right + 8;
        }
        wp.rcNormalPosition.bottom = rc.bottom + 5;
        SetWindowPlacement(&wp);

        GetDlgItem(IDC_STATIC_SEPERATOR)->ShowWindow(SW_HIDE);
    }

}

void CISPDlg::initFlashSN()
{

    if (!m_bShowSN)
    {
        GetDlgItem(IDC_BUTTON_FLASH)->SetWindowText(_T("FLASH"));
    }
    else
    {
        GetDlgItem(IDC_BUTTON_FLASH)->SetWindowText(_T("FLASH SN"));
    }
    InitISPUI();



    //////////////////////////////////////////////////////////////////////////
    //
    int nHWVersion = 0;
    int nModelValue = 0;

    m_cfgFileHandler.GetFlashSNParam(g_strExePath + CString(c_strFlashSNCfgFileName), nHWVersion, nModelValue);

    CWnd* pWnd = GetDlgItem(IDC_EDIT_HWVERSION);
    CString strTemp;
    strTemp.Format(_T("%d"), nHWVersion);

    pWnd->SetWindowText(strTemp);

    //////////////////////////////////////////////////////////////////////////
    m_cfgFileHandler.GetProductModel(g_strExePath + CString(c_strProductModelFileName), m_mapModel);

    CComboBox* pModel = (CComboBox*)GetDlgItem(IDC_COMBO_PM);

    pModel->ResetContent();
    map<int, CString>::iterator m1 = m_mapModel.begin();


    int nCurrentSel = 0;
    for (int i = 0 ; m1 != m_mapModel.end( ); m1++, i++ )
    {
        pModel->AddString(m1->second);
        if (m1->first == nModelValue)
        {
            nCurrentSel = i;
        }
    }

    pModel->SetCurSel(nCurrentSel);

}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CISPDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CISPDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void CISPDlg::ShowSplashWnd(int nMillisecond)
{
    CSplashWnd::SetSplashPicFilePath(m_strLogoFileName);
    CSplashWnd::EnableSplashScreen();
    CSplashWnd::ShowSplashScreen(this);

    Sleep(nMillisecond);
    CSplashWnd::endSplashScreen();
}
void CISPDlg::OnBnClickedButtonSelfile()
{
    UpdateData();
    //CString strFilter = _T("hex File|*.hex|bin File|*.bin|all File|*.*||");
    CString strFilter = _T("hex File;bin File|*.hex;*.bin|all File|*.*||");
    CFileDialog dlg(true,_T("hex"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
    dlg.DoModal();
    m_strHexFileName=dlg.GetPathName();
    m_strFlashFileName=dlg.GetPathName();

    CWnd* pEditFilePath = (CWnd*)GetDlgItem(IDC_EDIT_FILEPATH);
    pEditFilePath->SetWindowText(m_strHexFileName);

}
void CISPDlg::SaveParamToConfigFile()
{

    GetDlgItem(IDC_EDIT_FILEPATH)->GetWindowText(m_strHexFileName); //m_strFlashFileName;//((CISPDlg*)GetParent())->GetFlashFileName();
    GetDlgItem(IDC_EDIT_FILEPATH)->GetWindowText(m_strFlashFileName);
    CString flashmethod;
    if (g_Commu_type==0)
    {
        flashmethod=_T("COM");
    }
    else
    {
        flashmethod=_T("Ethernet");
    }

    CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MDBID1);
    CString strMID;
    pEdit->GetWindowText(strMID);
    strMID.Trim();
    strMID.TrimLeft(_T("0"));

    // com port
    CString strComPort;
    m_ComPort.GetWindowText(strComPort);

    // Baudrate
    CString strBaudrate = _T("19200");
    GetDlgItem(IDC_EDIT_BAUDRATE)->GetWindowTextW(strBaudrate);
    //
    /*CString strFlashPage;
    strFlashPage = GetCurSelPageStr();*/
    //
    CString subnote;
    if(IsDlgButtonChecked(IDC_CHECK_FLASH_SUBID)==TRUE)
        subnote=_T("1");
    else
        subnote=_T("0");

    CString subID;
    GetDlgItemText(IDC_EDIT_MDBID2,subID);

    CString strip;
    m_ipAddress.GetWindowText(strip);
    CString stripport;
    GetDlgItem(IDC_EDIT_NCPORT)-> GetWindowText(stripport);
    m_cfgFileHandler.WriteToCfgFile(m_strHexFileName,
                                    flashmethod,
                                    strMID,
                                    strComPort,
                                    strBaudrate,
                                    strip,
                                    stripport,
                                    subnote,
                                    subID);
}

void CISPDlg::OnBnClickedButtonFlash()
{
#ifdef ISP_BURNING_MODE
    burning_test_finished = 0;
#endif
    firmware_must_use_new_bootloader = 0;
    UpdateData();
    SaveParamToConfigFile();	//Save data entered by the user

    m_FlashTimes = m_cfgFileHandler.GetFlashTimes();

    if (((CButton *)GetDlgItem(IDC_CHECK_MSTP_UPDATE))->GetCheck())
    {
        SPECIAL_BAC_TO_MODBUS = 1;
    }
    else
    {
        SPECIAL_BAC_TO_MODBUS = 0;
    }

    GetDlgItem(IDC_EDIT_FILEPATH)->GetWindowText(m_strHexFileName);
    string temp_md5 = MD5(ifstream(m_strHexFileName)).toString();

    //char md5_temp[32];
    //firmware_md5
    memcpy(firmware_md5, MD5(ifstream(m_strHexFileName)).digest(), 16);
    //for (int i = 0; i < 32; i++)
    //{
    //    firmware_md5[i] = ((unsigned short)md5_temp[i]  & 0x00ff);
    //}
    // =  MD5(ifstream(m_strHexFileName)).digest();
    CString MD5_value;
    MD5_value = temp_md5.c_str();
    CString temp_show;
    temp_show.Format(_T("Firmware MD5:"));
    temp_show = temp_show + MD5_value;
    UpdateStatusInfo(temp_show, FALSE);

    switch(Judge_Flash_Type())
    {
    case FLASH_TSTAT_COM:
    {
        SetCommunicationType(0);
        FlashByComport();
        break;
    }
    case FLASH_NC_LC_NET:
    {


        SetCommunicationType(1);
        SaveParamToConfigFile();
        FlashByNetwork();




        break;
    }
    case FLASH_SUBID_NET:
    {
        SetCommunicationType(1);
        FlashSubID();
        break;
    }

    default:
    {
        //Undefined type state
    }
    }


  

}
//Here's how to determine what kind of device is plugged into the PC.
unsigned int CISPDlg::Judge_Flash_Type()
{
    if (g_Commu_type==0)
    {
        return FLASH_TSTAT_COM;
    }
    else
    {

        if(FLASH_SUBID)
        {
            return FLASH_SUBID_NET;
        }
        else
        {
            return FLASH_NC_LC_NET;
        }



    }


}

afx_msg LRESULT CISPDlg::OnFlashBoot_Update_boot(WPARAM wParam, LPARAM lParam)
{
    int npid = (int)wParam;
    if (m_pComWriter)
    {
        delete m_pComWriter;
        m_pComWriter = NULL;
    }
    if (m_pFileBuffer)
    {
        delete[] m_pFileBuffer;
        m_pFileBuffer = NULL;
    }

    if (m_pTCPFlasher)
    {
        delete m_pTCPFlasher;
        m_pTCPFlasher = NULL;
    }
    EnableFlash(TRUE);

    if (m_pTFTPServer)
    {
        delete m_pTFTPServer;
        m_pTFTPServer = NULL;
    }

        if((npid == PM_MINIPANEL_ARM) || (PM_MINIPANEL == npid))
            g_repair_bootloader_file_path = g_strExePath + _T("ResourceFile\\HexFile\\repair_bootloader.hex");
        else if(npid == PM_TSTAT10)
            g_repair_bootloader_file_path = g_strExePath + _T("ResourceFile\\HexFile\\repair_bootloader_T10.hex");
        else if (npid == PM_TSTAT8)
            g_repair_bootloader_file_path = g_strExePath + _T("ResourceFile\\HexFile\\repair_bootloader_T8.hex");
        else if(((npid >= STM32_CO2_NET) && (npid <= STM32_PRESSURE_RS485)) || (npid == STM32_PM25))
            g_repair_bootloader_file_path = g_strExePath + _T("ResourceFile\\HexFile\\repair_bootloader_CO2.hex");
        else if (npid == PM_TSTAT9)
            return 1;
            //g_repair_bootloader_file_path = g_strExePath + _T("ResourceFile\\HexFile\\repair_bootloader_T9.hex");
        else 
            return 1;
        //g_repair_bootloader_file_path = g_strExePath + _T("ResourceFile\\HexFile\\repair_bootloader_NB_rev1.hex");


    //����Ҫ��д�Ĵ���·��; - Change the path of the code to be burned;
    GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(g_repair_bootloader_file_path);



    new_bootload = 1;
    com_port_flash_status = 1;

    SetTimer(1, 200, NULL);

    return 1;
}

afx_msg LRESULT CISPDlg::OnFlashNewBootFinish(WPARAM wParam, LPARAM lParam)
{
    int nRet = lParam;
    if (m_pComWriter)
    {
        delete m_pComWriter;
        m_pComWriter = NULL;
    }
    if (m_pFileBuffer)
    {
        delete[] m_pFileBuffer;
        m_pFileBuffer = NULL;
    }

    if (m_pTCPFlasher)
    {
        delete m_pTCPFlasher;
        m_pTCPFlasher = NULL;
    }
    EnableFlash(TRUE);

    if (m_pTFTPServer)
    {
        delete m_pTFTPServer;
        m_pTFTPServer = NULL;
    }

    //����Ҫ��д���µĹ̼���  ����·��; - Change the path of the new firmware to be burned;
    GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(g_update_newfirmware_file_path);
    new_bootload = 0;
    com_port_flash_status = 2;
    SetTimer(1, 200, NULL);

    return 1;
}



static int total_test_count = 0;
static int total_success_count = 0;
#ifdef ISP_BURNING_MODE
static int total_udp_count = 0;
static int total_udp_success_count = 0;
extern int temco_burning_mode;
#endif
void close_bac_com();
// NC ��ʹ��tftpЭ��������flash�� - NC uses TFTP protocol for flashing
afx_msg LRESULT CISPDlg::OnFlashFinish(WPARAM wParam, LPARAM lParam)
{
#ifdef ISP_BURNING_MODE
    total_udp_count++;
    if (temco_burning_mode == 1)
    {
        auto_flash_mode = false;
    }
#endif
    int nRet = lParam; // �߳�������Ϣ����������Ͳ�����, popup message in the thread
    // flash all done, release the resources
    if (m_pComWriter)
    {
        delete m_pComWriter;
        m_pComWriter = NULL;
        if (SPECIAL_BAC_TO_MODBUS)
        {
            close_bac_com();
            //int ret = Initial_bac(m_nComPort, _T(""), m_nBautrate);
        }
    }
    if (m_pFileBuffer)
    {
        delete[] m_pFileBuffer;
        m_pFileBuffer = NULL;
    }

    if (m_pTCPFlasher)
    {
        delete m_pTCPFlasher;
        m_pTCPFlasher = NULL;
    }
    EnableFlash(TRUE);

    if (m_bShowSN	)
    {
        FlashSN();
    }
	if (m_FlashTimes>0)
	{
#ifdef ISP_BURNING_MODE
		total_test_count ++ ;
		if(nRet)
		{
			total_success_count ++;
		}
#endif
		CString test_info;
		test_info.Format(_T("Test : %u  ,success : %u"),total_test_count,total_success_count);

		int nCount = test_info.GetLength();
		WCHAR* strNew = new WCHAR[nCount+1];
		ZeroMemory(strNew, (nCount+1)*sizeof(WCHAR));
		LPCTSTR str = LPCTSTR(test_info);
		memcpy(strNew, str, nCount*sizeof(WCHAR));
		PostMessage( WM_ADD_STATUSINFO, 0,LPARAM(strNew));

		SetTimer(TEST_ISP_TIMER,1000,NULL);


	}
	
    SetResponseTime(100);
	if(auto_flash_mode)
    {
        if (com_port_flash_status != 1) //�������д bootloader ��ģʽ  �Զ��ģ���Ҫ������д��ͻ���Ҫ�Ĵ������; - If it is the mode of flashing the bootloader automatically, it is necessary to continue flashing the code required by the customer.
        {
            auto_flash_mode = false;	//Flash completed, allow manual shutdown;
            if (nRet > 0)
            {
                WritePrivateProfileStringW(_T("Data"), _T("Command"), _T("2"), AutoFlashConfigPath);	//FLASH_SUCCESS
            }
            else
            {
                WritePrivateProfileStringW(_T("Data"), _T("Command"), _T("4"), AutoFlashConfigPath);	//FAILED_UNKNOW_ERROR
            }
            #ifdef ISP_BURNING_MODE
            burning_test_finished = 1; //�����ѭ��ģʽ�Ͳ��ر�; - If it is the loop mode, do not close it.
            return 1;
            #endif
            PostMessage(WM_CLOSE, NULL, NULL);
        }

    }
    if (nRet > 0)
    {
        CString strTips = _T("Programming successful. ");
        UpdateStatusInfo(strTips, FALSE);
    }

    if (m_pTFTPServer)
    {
        delete m_pTFTPServer;
        m_pTFTPServer = NULL;
    }

    if (com_port_flash_status == 2)
        com_port_flash_status = 0;
#ifdef ISP_BURNING_MODE


    if(nRet)
	{
		total_udp_success_count++;
	}
    CString strTips;
    strTips.Format(_T("Total Test : %u ,Success : %u"),total_udp_count,total_udp_success_count);
    UpdateStatusInfo(strTips, FALSE);
    burning_test_finished = 1;
#endif
    return 1;
}
int Add_log_count = 0;
afx_msg LRESULT CISPDlg::OnAddStatusInfo(WPARAM wParam, LPARAM lParam)
{
    CString strInfo = CString(LPTSTR(lParam));
    UpdateStatusInfo(strInfo, false);
    if(auto_flash_mode)
    {
        Add_log_count ++;
        CString temp;
        temp.Format(_T("%d"),Add_log_count);
        WritePrivateProfileStringW(_T("LogInfo"),_T("AddCount"),temp,AutoFlashConfigPath);
        CString section;
        section.Format(_T("AddText%d"),Add_log_count);
        WritePrivateProfileStringW(_T("LogInfo"),section,strInfo,AutoFlashConfigPath);
    }
    WCHAR* szTemp = (LPTSTR(lParam));
    delete [] szTemp;

    //UpdateStatusInfo(g_strFlashInfo, false);
    return 1;
}

afx_msg LRESULT CISPDlg::OnReplaceStatusInfo(WPARAM wParam, LPARAM lParam)
{
    static int test_loop_count = 0;
    CString strInfo = CString(LPTSTR(lParam));
    UpdateStatusInfo(strInfo, true);
    if(auto_flash_mode)
    {
        test_loop_count = (++test_loop_count)%5;

        if(strInfo.Find(_T("100")) || (test_loop_count == 0))
        {
            WritePrivateProfileStringW(_T("LogInfo"),_T("Replace_Refresh"),_T("1"),AutoFlashConfigPath);
            CString temp;
            temp.Format(_T("%d"),Add_log_count);
            WritePrivateProfileStringW(_T("LogInfo"),_T("ReplaceCount"),temp,AutoFlashConfigPath);
            CString section;
            section.Format(_T("ReplaceText%d"),Add_log_count);
            WritePrivateProfileStringW(_T("LogInfo"),section,strInfo,AutoFlashConfigPath);
        }

    }
    WCHAR* szTemp = (LPTSTR(lParam));
    delete[] szTemp;

    //UpdateStatusInfo(g_strFlashInfo, true);
    return 1;
}




////////////////////////////////////////////////////////////////////////////
// Parameters BOOL, =TRUE replace the current line, =FALSE add a new line
void CISPDlg::UpdateStatusInfo(const CString& strInfo, BOOL bReplace)
{
    int nLineCount = m_lbStatusInfo.GetCount();
    int nCurrentLineNum = nLineCount;
    if (bReplace)
    {
        nCurrentLineNum--;
        m_lbStatusInfo.DeleteString(nCurrentLineNum);
    }

    int nRet =m_lbStatusInfo.InsertString(nCurrentLineNum,strInfo);
    if (nRet == LB_ERR )
    {
        ASSERT(0);
    }
    m_lbStatusInfo.SetCurSel(nRet);

    UpdateData(FALSE);
}


void CISPDlg::OnClose()
{
    if(auto_flash_mode)
    {
        if(IDNO == MessageBox(_T("Are you sure you want to exit ,the ISP is not finished yet!"),_T("Warning"),MB_YESNO | MB_ICONINFORMATION))
        {
            return;
        }
    }

    CDialog::OnClose();
}

BOOL CISPDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class

    if(auto_flash_mode)
    {
        if(pMsg->message == WM_KEYDOWN)
        {
            if((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
                return true;
        }
    }

    if (pMsg->message == WM_KEYDOWN)
    {

        if (pMsg->wParam  == VK_RETURN)
        {
            OnBnClickedButtonFlash();
        }

        if (pMsg->wParam == VK_F2 )
        {
            if(GetKeyState(VK_MENU) == 1)
            {
                //initFlashSN();
            }
            //initFlashSN();
        }
    }
    if(m_ToolTip != NULL)
        m_ToolTip->RelayEvent(pMsg);
    return CDialog::PreTranslateMessage(pMsg);
}

void CISPDlg::SetFlashFileName(const CString& strFileName)
{
    CWnd* pWnd = GetDlgItem(IDC_EDIT_FILEPATH);
    pWnd->SetWindowText(strFileName);
}

CString CISPDlg::GetFlashFileName()
{
    CWnd* pWnd = GetDlgItem(IDC_EDIT_FILEPATH);
    pWnd->GetWindowText(m_strHexFileName);

    return m_strHexFileName;
}
void CISPDlg::EnableFlash(BOOL bEnable)
{
    m_FlashEnable = bEnable;
    CWnd* pBtn = GetDlgItem(IDC_BUTTON_FLASH);
    pBtn->EnableWindow(bEnable);
    CWnd* pBtn1 = GetDlgItem(IDC_CLEAR_LOG);
    pBtn1->EnableWindow(bEnable);
    CWnd* pBtn2 = GetDlgItem(IDC_SAVE_LOG);
    pBtn2->EnableWindow(bEnable);
    CWnd* pBtnsn = GetDlgItem(IDC_BUTTON_FLASH_SN);
    pBtnsn->EnableWindow(bEnable);

}
void CISPDlg::SaveFlashSNParamToFile()
{
    CString strHWV;
    CWnd* pWnd = GetDlgItem(IDC_EDIT_HWVERSION);
    pWnd->GetWindowText(strHWV);
    int nHWV = _wtoi(strHWV);

    CString strModel;
    CComboBox* pCbx =(CComboBox*) GetDlgItem(IDC_COMBO_PM);
    int nSel = pCbx->GetCurSel();

    pCbx->GetWindowText(strModel);

    CString strtemp = m_mapModel[nSel];

    map<int, CString>::iterator m1;
    int nModelVal= nSel;
    for( m1=m_mapModel.begin(); m1 != m_mapModel.end(); m1++ )
    {
        CString strVal = m1->second;

        if(strVal.CompareNoCase(strModel) == 0)
        {
            nModelVal = m1->first;
            break;
        }
    }

    m_cfgFileHandler.SaveFlashSNParam(g_strExePath + CString(c_strFlashSNCfgFileName) , nHWV, nModelVal);

}
//For Old UI
//Triggered when you select page
CString CISPDlg::GetCurSelPageStr()
{
    CString strPageInfo = CString(c_strCfgTstatSection);
    int nSel = m_nTabSel;
    switch(nSel)
    {
    case 0:
        strPageInfo = CString(c_strCfgTstatSection);
        break;
    case 1:
        strPageInfo = CString(c_strCfgNCSection);
        break;
    case 2:
        strPageInfo = CString(c_strCfgLightCtrlSection);
        break;
    default:
        ASSERT(0);
        break;
    }

    return strPageInfo;
}
//For Old UI
BOOL CISPDlg::GetFlashSNParam(int& nHWVerison, CString& strProductModel,int &nProductID)
{
    if (m_bShowSN)
    {
        //nProductModel = 1;

        CString strHWV;
        CWnd* pWnd = GetDlgItem(IDC_EDIT_HWVERSION);
        pWnd->GetWindowText(strHWV);
        nHWVerison = _wtoi(strHWV);

        CString strPM;
        pWnd = GetDlgItem(IDC_COMBO_PM);
        pWnd->GetWindowText(strProductModel);

        //nProductID=m_nproductid;
        CString strModel;
        CComboBox* pCbx =(CComboBox*) GetDlgItem(IDC_COMBO_PM);
        int nSel = pCbx->GetCurSel();

        pCbx->GetWindowText(strModel);

        CString strtemp = m_mapModel[nSel];

        map<int, CString>::iterator m1;
        int nModelVal= nSel;
        for( m1=m_mapModel.begin(); m1 != m_mapModel.end(); m1++ )
        {
            CString strVal = m1->second;

            if(strVal.CompareNoCase(strModel) == 0)
            {
                nProductID = m1->first;

                break;
            }
        }


    }
    else
    {
        nHWVerison = 0;
        return FALSE;
    }

    return TRUE;
}

//Ping
void CISPDlg::OnTestPing(const CString& strIP)
{
    m_strPingIP = strIP;
    AfxBeginThread(_PingThread, this);
}
UINT _PingThread(LPVOID pParam)
{
    CISPDlg* pDlg = (CISPDlg*)(pParam);
    CString strIP = pDlg->m_strPingIP;
    Ping(strIP, pDlg);
    return 1;
}

BOOL CISPDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
    // TODO: Add your message handler code here and/or call default
    int nSize = pCopyDataStruct->cbData;// = (strMsg.GetLength()+1)*sizeof(TCHAR); //(lstrlen(szBuffer)+1)*sizeof(TCHAR);
    //pCopyDataStruct->lpData =  (LPVOID)(LPCTSTR)strMsg;
    CString strMsg = CString(LPCTSTR(pCopyDataStruct->lpData));

    CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_INFO);
    //pList->AddString(strMsg);
    UpdateStatusInfo(strMsg, FALSE);
    //UpdateData(TRUE);
    return CDialog::OnCopyData(pWnd, pCopyDataStruct);
    /*return CDialog::OnCopyData(pWnd, pCopyDataStruct);*/
}
//Click the COM Port button setting
void CISPDlg::OnBnClickedCom()
{
    //GetDlgItem(IDC_COM)->SetWindowText(_T("Input ID"));
    //GetDlgItem(IDC_NET)->SetWindowText(_T("NET FLASH"));
    COM_INPUT=TRUE;
    g_Commu_type=0;
    COM_NET_Set_ReadOnly();
    GetDlgItem(IDC_CHECK_MSTP_UPDATE)->EnableWindow(1);
}
//Available Settings
void CISPDlg::COM_NET_Set_ReadOnly()
{
    if (COM_INPUT)
    {
        id.EnableWindow();
        m_ComPort.EnableWindow();
        // m_ComPort.SetWindowText(_T("Input more than one ID and Select COM"));
        m_ipAddress.EnableWindow(FALSE);
        m_ipPort.EnableWindow(FALSE);
        m_Check_SubID.EnableWindow(FALSE);
        m_SubID.EnableWindow(FALSE);
        m_Btn_ping.EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_MDBID2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BAUDRATE)->EnableWindow(TRUE);
    }
    else
    {
        id.EnableWindow(FALSE);
        m_ComPort.EnableWindow(FALSE);

        m_ipAddress.EnableWindow();
        m_ipPort.EnableWindow();
        m_Check_SubID.EnableWindow();
        m_Btn_ping.EnableWindow();
        set_SUBID();
        if(IsDlgButtonChecked(IDC_CHECK_FLASH_SUBID))
        {
            GetDlgItem(IDC_EDIT_MDBID2)->EnableWindow(1);
        }
        else
        {
            GetDlgItem(IDC_EDIT_MDBID2)->EnableWindow(0);
        }
		GetDlgItem(IDC_EDIT_BAUDRATE)->EnableWindow(FALSE);
    }
}
//if Click check box ,it means that click button .
void CISPDlg::set_SUBID()
{
    if (FLASH_SUBID)
    {
        m_SubID.EnableWindow();
    }
    else
    {
        m_SubID.EnableWindow(FALSE);
    }
}
void CISPDlg::OnBnClickedNet()
{
    COM_INPUT=FALSE;
    g_Commu_type=1;
    GetDlgItem(IDC_COM)->SetWindowText(_T("COM FLASH"));
    GetDlgItem(IDC_NET)->SetWindowText(_T("Input IP and Port"));
    COM_NET_Set_ReadOnly();
    ((CButton *)GetDlgItem(IDC_CHECK_MSTP_UPDATE))->SetCheck(false);
    GetDlgItem(IDC_CHECK_MSTP_UPDATE)->EnableWindow(0);
}

void CISPDlg::OnBnClickedCheckFlashSubid()
{
    m_Check_SubID.SetWindowTextW(_T("Flash SubNode By ID"));
    if(IsDlgButtonChecked(IDC_CHECK_FLASH_SUBID)==1)
    {
        GetDlgItem(IDC_EDIT_MDBID2)->EnableWindow(1);
        FLASH_SUBID=TRUE;
    }
    else
    {
        GetDlgItem(IDC_EDIT_MDBID2)->EnableWindow(0);
        FLASH_SUBID=FALSE;
    }
    //if (FLASH_SUBID)
    //{
    // FLASH_SUBID=FALSE;
    // m_Check_SubID.SetWindowTextW(_T("Flash SubNode By ID"));
    //}
    //else
    //{
    // FLASH_SUBID=TRUE;
    // m_Check_SubID.SetWindowTextW(_T("Input SubID Below"));
    //}

    // set_SUBID();
}


void CISPDlg::InitCombox(void)
{
    vector<CString> szComms;
    BOOL bRet = GetSerialComm_ISP(szComms);
    if (bRet && szComms.size()>0)
    {
        m_ComPort.Clear();
        for (UINT i = 0; i < szComms.size(); i++)
        {
            m_ComPort.AddString(szComms[i]);
        }
    }
    else  // can not get com port name from register, then add com port manually
    {
        CString strTips = _T("|Can not find any Com port on your computer.");
        UpdateStatusInfo(strTips, FALSE);
        m_ComPort.Clear();
        for (UINT i = 1; i < 9; i++)
        {
            CString strCom;
            strCom.Format(_T("COM%d"), i);
            m_ComPort.AddString(strCom);
        }
    }
}
// Function:
// Choose com port,Flash Device.

BOOL CISPDlg::FlashByComport(void)
{
    int TempID=0;
    int times=0;
    CString temp;
    if (m_bShowSN)
    {
        // PostMessage(WM_FLASH_FINISH, 0, LPARAM(1));
        // int	nRet =PostMessage(m_pParentWnd->m_hWnd, WM_FLASH_FINISH, 0, LPARAM(nFlashFlag));

        FlashSN();
        return FALSE;
    }
//     if (GetCommunicationType () == 0)
//     {
// 		if (!DetectBraudrate())
// 		{
// 			UpdateStatusInfo(_T("Detecting your Braudrate ,Failed"), FALSE);
// 			return FALSE;
// 		}
// 
// 		//SetResponseTime(60);
//     }
    if(!FileValidation(m_strHexFileName))
    {
        return FALSE;
    }

    if (g_Commu_type == 0)
    {
        int nPort = GetComPortNo();
        if(!ValidMdbIDString())
        {
            return FALSE;
        }

        if(!GetModbusID(m_szMdbIDs))
        {
            return FALSE;
        }
  //      BOOL flag=open_com(nPort);
		//Change_BaudRate (m_Brandrate);
  //      if (flag)
  //      {
  //          if (m_szMdbIDs.size()<=1)
  //          {
  //              TempID=m_szMdbIDs[0];
  //              if (TempID!=255)
  //              {
  //                  int realID=-1;
  //                  realID=Read_One(TempID,6);
  //                  while(realID<0) //the return value == -1 ,no connecting
  //                  {
  //                      if(times<5)
  //                      {
  //                          times++;
  //                          realID=Read_One(TempID,6);
  //                          continue;
  //                      }
  //                      else
  //                      {
  //                          break;
  //                      }

  //                  }
  //                  if (times>=5)
  //                  {
  //                      temp.Format(_T("ISPTool Can't connect to %d,device may be offline!\n"),TempID);
  //                      UpdateStatusInfo(temp,false);
  //                      if(auto_flash_mode)
  //                      {
  //                          PostMessage(WM_FLASH_FINISH, 0, LPARAM(0));
  //                      }
  //                      /*int ret=AfxMessageBox(temp,MB_OKCANCEL);
  //                      if(ret==IDOK)
  //                      {
  //                      	TempID=255;
  //                      	m_szMdbIDs[0]=255;
  //                      	temp.Format(_T("%d"),TempID);
  //                      	id.SetWindowText(temp);
  //                      }*/
  //                      return FALSE;

  //                  }
  //                  else
  //                  {
  //                      if (realID<0)
  //                      {
  //                          if(!auto_flash_mode)
  //                              AfxMessageBox(_T("Can't read the device ID,please check the connection."));
  //                          else
  //                          {
  //                              temp.Format(_T("Can't read the device ID,please check the connection."));
  //                              UpdateStatusInfo(temp,false);
  //                              PostMessage(WM_FLASH_FINISH, 0, LPARAM(0));
  //                          }
  //                          return FALSE;
  //                      }
  //                      if (realID!=TempID)
  //                      {
  //                          temp.Format(_T("The ID is %d,retry to flash it !\n"),realID);
  //                          if(!auto_flash_mode)
  //                              AfxMessageBox(temp);
  //                          UpdateStatusInfo(temp,false);
  //                          temp.Format(_T("%d"),realID);
  //                          id.SetWindowText(temp);
  //                          if(auto_flash_mode)
  //                          {
  //                              FlashByCom();
  //                              return TRUE;
  //                          }
  //                          return FALSE;

  //                      }



  //                  }
  //              }

  //          }
  //          else
  //          {
  //              for (UINT i=0; i<m_szMdbIDs.size(); i++)
  //              {
  //                  TempID=m_szMdbIDs[i];
  //                  while(Read_One(TempID,0xee10)<0) //the return value == -1 ,no connecting
  //                  {
  //                      if(times<5)
  //                      {
  //                          times++;
  //                          continue;
  //                      }
  //                      else
  //                      {
  //                          break;
  //                      }

  //                  }
  //                  if (times>=5)
  //                  {
  //                      temp.Format(_T("ISP   Tool Can't connect to %d\n"),TempID);
  //                      continue;
  //                      UpdateStatusInfo(temp,false);
  //                  }
  //              }
  //          }
  //          close_com();
  //      }
  //      else
  //      {
  //          UpdateStatusInfo(_T("Com Occupied"),false);
  //          if(!auto_flash_mode)
  //              AfxMessageBox(_T("Com Occupied"));
  //          else
  //          {
  //              WritePrivateProfileStringW(_T("Data"),_T("Command"),_T("4"),AutoFlashConfigPath);	//FAILED_UNKNOW_ERROR
  //              auto_flash_mode = false;//����Ϊfalse�� ����Ϊ�Զ�ģʽ���˳�����;
  //              PostMessage(WM_CLOSE,NULL,NULL);
  //              return FALSE;
  //          }
  //          return FALSE;
  //      }



        FlashByCom();
    }


    return TRUE;
}
/*
Author:	Alex
Date: 2012-10-30
Function:
Choose NET,Can Flash NC
*/
BOOL CISPDlg::FlashByNetwork(void)
{

    if(!FileValidation(m_strHexFileName))
    {
        return FALSE;
    }

//     if(BinFileValidation(m_strHexFileName))
//     {
//         CBinFileParser* pBinFile=new CBinFileParser;
//         pBinFile->SetBinFileName(m_strHexFileName);
//         m_pFileBuffer=new char[c_nBinFileBufLen];
//         memset(m_pFileBuffer, 0xFF, c_nBinFileBufLen);
//
//         int nDataSize=pBinFile->GetBinFileBuffer(m_pFileBuffer,c_nBinFileBufLen);
//         CString hexinfor=_T("The Bin For ");
//         hexinfor+=pBinFile->GetBinInfor();
//         CString strFilesize;
//         strFilesize.Format(_T("Bin size=%d Bs"),nDataSize);
//         GetDlgItem(IDC_HEX_SIZE)->SetWindowText(strFilesize);
//         m_strASIX=pBinFile->m_strASIX;
//         m_strProductName=pBinFile->m_strProductName;
//         GetDlgItem(IDC_BIN_INFORMATION)->SetWindowText(hexinfor);
//     }

    if (g_Commu_type == 1)
    {
        FlashByEthernet();
    }
    return TRUE;




    return TRUE;
}
/*
Author:	Alex
Date: 2012-10-30
Function:
Write hex files to the subnet through TCP/IP Via Gateway 
flash tstat/input/output board.
BOOL FlashSubID(void)
*/

BOOL CISPDlg::FlashSubID(void)
{
    if(!ValidMdbIDStringSUBID())
    {
        return FALSE;
    }
    GetModbusIDSUBID(m_szMdbIDs);
    OnFlashSubID();
    return TRUE;
}
BOOL CISPDlg::ValidMdbIDStringSUBID()
{
    CString strModbusID;
    GetDlgItem(IDC_EDIT_MDBID2)->GetWindowText(strModbusID);
    if (strModbusID==_T(""))
    {
        if(!auto_flash_mode)
            AfxMessageBox(_T("Please Input sub node IDs!"));
        else
        {
            CString temp;
            temp.Format(_T("The sub node ID is invalid. Please try scanning again first"));
            UpdateStatusInfo(temp,false);
            PostMessage(WM_FLASH_FINISH, 0, LPARAM(0));
        }
        return FALSE;
    }
    else
    {
        for(int i = 0; i < strModbusID.GetLength(); i++)
        {
            if(!(strModbusID.GetAt(i) >='0' && strModbusID.GetAt(i)<='9'))
            {
                if (strModbusID.GetAt(i) != ';')
                {
                    CString strTips =_T("Error: Modbus ID invalidation.");
                    UpdateStatusInfo(strTips, FALSE);
					if(!auto_flash_mode)
					 AfxMessageBox(strTips);
                    return FALSE;
                }
            }
        }
        return TRUE;
    }

}

int CISPDlg::GetModbusIDSUBID(vector<int>& szMdbIDs)
{
    szMdbIDs.clear();
    TCHAR c = ';';
    CString strSrc;
    GetDlgItem(IDC_EDIT_MDBID2)->GetWindowText(strSrc);
    int nPos =0;
    while(strSrc.GetLength() > 0)
    {
        nPos = strSrc.Find(c);

        if (nPos != 0)
        {
            if (nPos < 0)
            {
                nPos = strSrc.GetLength();
            }
            CString strID = strSrc.Left(nPos);
            int nID = _wtoi(strID);//.GetString());

            if (!(nID > 0 && nID <= 255))
            {
                CString strTips;
                strTips.Format(_T("Error : Wrong Modbus ID : %d. Please input correct ID."), nID);
                UpdateStatusInfo(strTips, FALSE);
                if(!auto_flash_mode)
                    AfxMessageBox(strTips);
                return FALSE;
            }
            szMdbIDs.push_back(nID);
        }
        // 		else if(nPos == 0) // In the first
        // 		{
        // 		}

        if (nPos < 0 || (nPos == strSrc.GetLength())) // Not found in the last 
        {
            return TRUE;
        }
        strSrc = strSrc.Mid(nPos+1);
    }

    return TRUE;
}

void CISPDlg::OnFlashSubID()
{


    if (m_pTCPFlasher)
    {
        delete m_pTCPFlasher;
        m_pTCPFlasher = NULL;
    }

    if (m_pFileBuffer)
    {
        delete[] m_pFileBuffer;
        m_pFileBuf= NULL;
    }

    CHexFileParser* pHexFile = new CHexFileParser;
    pHexFile->SetFileName(m_strFlashFileName);

    m_pFileBuf = new char[c_nHexFileBufLen];
    memset(m_pFileBuf, 0xFF, c_nHexFileBufLen);
    int nDataSize = pHexFile->GetHexFileBuffer(m_pFileBuf, c_nHexFileBufLen);


    //////////////////////////////////////////////////////////////////////////
    if(nDataSize > 0)
    {
        CString strTips = _T("Hex file verified okay.");
        UpdateStatusInfo(strTips, FALSE);

        //////////////////////////////////////////////////////////////////////////


        m_pTCPFlasher = new CComWriter;
        m_pTCPFlasher->SetParentWnd(this);
        CString ipaddress;
        m_ipAddress.GetWindowText(ipaddress);
        m_pTCPFlasher->SetIPAddr(ipaddress);



        CString strIPPort;


        m_pTCPFlasher->SetIPPort(m_IPPort);

        m_pTCPFlasher->SetModbusID(m_szMdbIDs);

        m_pTCPFlasher->SetFileBuffer((TS_UC*)m_pFileBuf, nDataSize);



//Fance
        if(pHexFile->GetHexFileFormatType() == HEXFILE_DATA)
        {
            m_pTCPFlasher->SetFileBuffer((TS_UC*)m_pFileBuf, nDataSize);

            m_pTCPFlasher->m_nHexFileType = HEXFILE_DATA;
        }
        else if(pHexFile->GetHexFileFormatType() == HEXFILE_LINERADDR)
        {
            vector<int> szFlags;
            pHexFile->GetExtendHexFileSectionFlag(szFlags);
            m_pTCPFlasher->SetExtendHexFileParam(szFlags, (TS_UC*)m_pFileBuf);

            m_pTCPFlasher->m_nHexFileType = HEXFILE_LINERADDR;
        }
//Fance
        //m_szHexFileFlags

        m_pTCPFlasher->m_hexbinfilepath=m_strFlashFileName;

        m_pTCPFlasher->Is_Ram = pHexFile->GetFileType();
        m_pTCPFlasher->BeginWirteByTCP();
        EnableFlash(FALSE);
    }
    else
    {
        CString strTips1 =_T("File error: The file is not a properly formatted HEX file.");
        UpdateStatusInfo(strTips1, FALSE);
        CString strTips2 =_T("Please reselect a right file.");
        UpdateStatusInfo(strTips2, FALSE);
        if(!auto_flash_mode)
            AfxMessageBox(strTips1+strTips2, MB_OK);

    }

    delete pHexFile;
}
/*
Author:	Alex
Date: 2012-10-30
Function:
Choose NET,Flash NC,LCBy net
*/
void CISPDlg::FlashByEthernet()
{
    /*   if (!Show_Flash_DeviceInfor_NET())
       {
            return ;
       }*/
    if (m_pTFTPServer)
    {
        delete m_pTFTPServer;
        m_pTFTPServer = NULL;
    }
    if (m_pFileBuffer)
    {
        delete[] m_pFileBuffer;
        m_pFileBuffer = NULL;
    }
    //  Bin_Info bin_infor;
//     Get_Binfile_Information(m_strFlashFileName.GetBuffer(),bin_infor);
//
//     CBinFileParser* pBinFile = new CBinFileParser;
//     pBinFile->SetBinFileName(m_strFlashFileName);
//     m_pFileBuffer = new char[c_nBinFileBufLen];
//     ZeroMemory(m_pFileBuffer, c_nBinFileBufLen);
// int nDataSize = pBinFile->GetBinFileBuffer(m_pFileBuffer, c_nBinFileBufLen);

    int nDataSize = 0;
	CString strTips2 =_T("Checking firmware file,please wait!");
	UpdateStatusInfo(strTips2, FALSE);

    if(BinFileValidation(m_strHexFileName))
    {
        CBinFileParser* pBinFile=new CBinFileParser;
        pBinFile->SetBinFileName(m_strHexFileName);
        m_pFileBuffer=new char[c_nBinFileBufLen];
        memset(m_pFileBuffer, 0xFF, c_nBinFileBufLen);
        nDataSize=pBinFile->GetBinFileBuffer(m_pFileBuffer,c_nBinFileBufLen);

      //  ShowHexBinInfor(2);
        delete pBinFile;
        pBinFile = NULL;
    }
    if(HexFileValidation (m_strHexFileName))
    {
        if(new_bootload == 0)   //�ڶ����� new boot �� ·������Ҫ������ǵ�һ���·��;
            g_update_newfirmware_file_path = m_strHexFileName;
        CHexFileParser* pHexFile = new CHexFileParser;
        pHexFile->SetFileName(m_strHexFileName);

        m_pFileBuffer = new char[c_nBinFileBufLen];
        memset(m_pFileBuffer, 0x00, c_nBinFileBufLen);
        //CString str2Tips = _T("Reading firmware file.Please wait..");
        //UpdateStatusInfo(str2Tips, FALSE);
        nDataSize = pHexFile->GetHexFileBuffer(m_pFileBuffer, c_nBinFileBufLen);//Get the file's buffer

        delete pHexFile;
        pHexFile = NULL;


    }

    if(nDataSize > 0)
    {
        CString strTips = _T("Firmware loading successful.");
        UpdateStatusInfo(strTips, FALSE);
        strTips.Format(_T("Firmware size : %.1f KB"), ((float)nDataSize) / 1024);
        UpdateStatusInfo(strTips, FALSE);

        CString strTips_version;// = _T("|Firmware bin file verified okay.");
        float hex_version = 0;
        hex_version = ((float)(global_fileInfor.software_high * 256 + global_fileInfor.software_low))/100;
        strTips_version.Format(_T("Hex file firmware version : %.2f"), hex_version);


        CString temp_hex_pid_name;
        MultiByteToWideChar(CP_ACP, 0, (char *)global_fileInfor.product_name, (int)strlen((char *)global_fileInfor.product_name) + 1, temp_hex_pid_name.GetBuffer(MAX_PATH), MAX_PATH);
        temp_hex_pid_name.ReleaseBuffer();
        temp_hex_pid_name.Trim();
        temp_hex_pid_name.MakeUpper();
        //ֻ��Hex PID Ϊminipanel arm �汾ʱ  �� �ſ���Ҫ�޸� bootload �İ汾; - Only when the Hex PID is the minipanel arm version can the bootloader version be repaired.
        //if ((temp_hex_pid_name.CompareNoCase(_T("MINI_ARM")) == 0 ) ||
        //    (temp_hex_pid_name.CompareNoCase(_T("MINIPANEL")) == 0))
        if (temp_hex_pid_name.CompareNoCase(_T("MINI_ARM")) == 0) 
        {
            n_hex_file_pid = PM_MINIPANEL_ARM;
            if (hex_version >= 60) //��������汾��˵����Ҫ512��flash �������ȸ���bootload; - If it is greater than this version, it means that 512 flash is needed, and the bootloader must be updated first.
            {
                firmware_must_use_new_bootloader = 1;
            }
        }
        else if (temp_hex_pid_name.CompareNoCase(_T("PID10")) == 0)
        {
            n_hex_file_pid = PM_TSTAT10;
            //��ʱ���ø���TSTAT10��bootloader - Temporarily not allowing the update of TSTAT10's bootloader
            //if (hex_version >= 51.08) //��������汾��˵����Ҫ512��flash �������ȸ���bootload; - If it is greater than this version, it means that 512 flash is needed, and the bootloader must be updated first.
            //{
            //    firmware_must_use_new_bootloader = 1;
            //}
        }
        else if (temp_hex_pid_name.CompareNoCase(_T("CO2ALL")) == 0)
        {
            n_hex_file_pid = STM32_CO2_NET;
            if (hex_version > 0.58) //�������58  �汾��˵����Ҫ512��flash �������ȸ���bootload; - If it is greater than this version, it means that 512 flash is needed, and the bootloader must be updated first.
            {
                firmware_must_use_new_bootloader = 1;
            }
        }

        UpdateStatusInfo(strTips_version, FALSE);

        m_pTFTPServer=new TFTPServer;
        DWORD dwIP = GetIPAddress();
        //m_IPAddr.GetAddress(dwIP);


        m_pTFTPServer->SetParentDlg(this);
        m_pTFTPServer->SetClientPort(m_IPPort);
        m_pTFTPServer->SetClientIP(dwIP);
        m_pTFTPServer->SetFileName(m_strFlashFileName);
        m_pTFTPServer->Set_FileProductName(m_strProductName);
        m_pTFTPServer->SetDataSource((BYTE*)m_pFileBuffer, nDataSize);

      //  m_pTFTPServer->Set_bininfor(temp1);
        m_pTFTPServer->m_FlashTimes = m_FlashTimes;
        m_pTFTPServer->FlashByEthernet();

        EnableFlash(FALSE);
    }
    else
    {
        CString strTips1 =_T("|Error: The file is not a properly formatted BIN file.");
        UpdateStatusInfo(strTips1, FALSE);
    }

//    delete pBinFile;
}
DWORD CISPDlg::GetIPAddress()
{
    DWORD dwIP=0;
    m_ipAddress.GetAddress(dwIP);

    return dwIP;
}
//0: not hex,not bin
//1 hex
//2 bin
int CISPDlg::Judge_BinHexFile(CString filepath)
{
    if (!HexFileValidation(filepath))
    {
        if (!BinFileValidation(filepath))
        {
            CString strTips;
            strTips = _T("|please select a *.HEX or *.BIN file");
            if(!auto_flash_mode)
                AfxMessageBox(strTips);
            UpdateStatusInfo(strTips, FALSE);
            return 0;
        }
        return 2;

    }
    else
    {
        return 1;
    }

}
/*
Author:	Alex
Date: 2012-10-25
Function:
verify the hex or bin file
*/
BOOL CISPDlg::FileValidation(const CString& strFileName)
{
    //  if (g_Commu_type==0 && !HexFileValidation(strFileName))
    //  {
    //      CString strTips;
    //      strTips = _T("|To Update using the Com port please select a *.HEX file");
    //if(!auto_flash_mode)
    //	 AfxMessageBox(strTips);
    //      UpdateStatusInfo(strTips, FALSE);
    //      return FALSE;
    //  }
    //  else if(g_Commu_type && !BinFileValidation(strFileName))
    //  {
    //      CString strTips;
    //      //strTips.Format(_T("%s isn't a BIN file."), strFileName);
    //      strTips = _T("|To Update over Ethernet please select a *.BIN file");
    //if(!auto_flash_mode)
    //	AfxMessageBox(strTips);
    //      UpdateStatusInfo(strTips, FALSE);
    //      return FALSE;
    //  }

    //  return TRUE;

    if (HexFileValidation (strFileName)||BinFileValidation(strFileName))
    {
        return TRUE;
    }
    CString strTips;
    //strTips.Format(_T("%s isn't a BIN file."), strFileName);
    strTips = _T("Please select a *.Bin or *.Hex file");
    //AfxMessageBox(strTips);
    UpdateStatusInfo(strTips, FALSE);
    return FALSE;
}
/*
Function: verify ID from user
*/
BOOL CISPDlg::ValidMdbIDString(void)
{
    CString strModbusID;
    GetDlgItem(IDC_EDIT_MDBID1)->GetWindowText(strModbusID);
    if (strModbusID==_T(""))
    {
        if(!auto_flash_mode)
            AfxMessageBox(_T("Please input device IDs"));
        return FALSE;
    }
    else
    {
    }
    for(int i = 0; i < strModbusID.GetLength(); i++)
    {
        if(!(strModbusID.GetAt(i) >='0' && strModbusID.GetAt(i)<='9'))
        {
            if (strModbusID.GetAt(i) != ';')
            {
                CString strTips =_T("|Error: Modbus ID not valid.");
                UpdateStatusInfo(strTips, FALSE);
                //AfxMessageBox(strTips);
                return FALSE;
            }
        }
    }
    return TRUE;


}
//����ModbusID
int CISPDlg::GetModbusID(vector<int>& szMdbIDs)
{
    szMdbIDs.clear();
    TCHAR c = ';';
    CString strSrc;
    GetDlgItem(IDC_EDIT_MDBID1)->GetWindowText(strSrc);
    int nPos =0;
    while(strSrc.GetLength() > 0)
    {
        nPos = strSrc.Find(c);

        if (nPos != 0)
        {
            if (nPos < 0)
            {
                nPos = strSrc.GetLength();
            }
            CString strID = strSrc.Left(nPos);
            int nID = _wtoi(strID);//.GetString());

            if (!(nID > 0 && nID <= 255))
            {
                CString strTips;
                strTips.Format(_T("|Error : Wrong Modbus ID : %d. Please input a valid ID."), nID);
                UpdateStatusInfo(strTips, FALSE);
                //AfxMessageBox(strTips);
                return FALSE;
            }
            szMdbIDs.push_back(nID);
        }
        if (nPos < 0 || (nPos == strSrc.GetLength())) // Not found in the last
        {
            return TRUE;
        }
        strSrc = strSrc.Mid(nPos+1);
    }
    return TRUE;
}
//When Flash by COM,use the function.
//0: not hex,not bin
//1 hex
//2 bin
#ifdef ISP_BURNING_MODE
 unsigned short Check_sum = 0;
#endif
void CISPDlg::FlashByCom()
{
    the_max_register_number_parameter_Count=0;
    the_max_register_number_parameter_Finished=0;
    if (m_pComWriter)
    {
        delete m_pComWriter;
        m_pComWriter = NULL;
    }

    if (m_pFileBuffer)
    {
        delete[] m_pFileBuffer;
        m_pFileBuffer= NULL;
    }
    int com_bin = 0;
    CHexFileParser* pHexFile = new CHexFileParser;
    pHexFile->SetFileName(m_strHexFileName);
    int nDataSize = 0;
    CString temp_name;
    temp_name = m_strHexFileName.Right(4);
    temp_name.MakeUpper();
    if (temp_name.CompareNoCase(_T(".BIN")) == 0)
    {

        CString strTips2 = _T("Checking firmware file,please wait!");
        UpdateStatusInfo(strTips2, FALSE);
        if (BinFileValidation(m_strHexFileName))
        {
            CBinFileParser* pBinFile = new CBinFileParser;
            pBinFile->SetBinFileName(m_strHexFileName);
            m_pFileBuffer = new char[c_nBinFileBufLen];
            memset(m_pFileBuffer, 0xFF, c_nBinFileBufLen);
            nDataSize = pBinFile->GetBinFileBuffer(m_pFileBuffer, c_nBinFileBufLen);

            CString strTips = _T("|Bin file verified okay.");
            UpdateStatusInfo(strTips, FALSE);
            strTips.Format(_T("Firmware size : %.1f KB"), ((float)nDataSize) / 1024);
            UpdateStatusInfo(strTips, FALSE);

            //  ShowHexBinInfor(2);
            delete pBinFile;
            pBinFile = NULL;
            com_bin = 1;
        }
    }
    else
    {

        m_pFileBuffer = new char[c_nHexFileBufLen];
        memset(m_pFileBuffer, 0x00, c_nHexFileBufLen);
        nDataSize = pHexFile->GetHexFileBuffer(m_pFileBuffer, c_nHexFileBufLen);//Get the file's buffer


    }


    if (nDataSize > 0)
    {
#ifdef ISP_BURNING_MODE
        Check_sum = 0;
#endif
        CString strTips = _T("|Firmware file verified okay.");
        UpdateStatusInfo(strTips, FALSE);
        strTips.Format(_T("Firmware size : %.1f KB"), ((float)nDataSize) / 1024);
        UpdateStatusInfo(strTips, FALSE);

        m_pComWriter = new CComWriter;
        m_pComWriter->SetModbusID(m_szMdbIDs);
        m_pComWriter->SetHexInfor(global_fileInfor);
        if(com_bin)
            m_pComWriter->SetHexFileType(HEXFILE_LINERADDR);
        else
            m_pComWriter->SetHexFileType(pHexFile->GetHexFileFormatType());
        m_pComWriter->Is_Ram = pHexFile->GetFileType();
        m_pComWriter->m_hexbinfilepath = m_strHexFileName;
        CString strBaudrate;
        GetDlgItem(IDC_EDIT_BAUDRATE)->GetWindowText(strBaudrate);
        int nBautrate = _wtoi(strBaudrate.GetBuffer());
        m_pComWriter->SetBautrate(nBautrate);
        SetResponseTime(30);
        int Baudrate_Index;
        for (int i = 0; i < NUMBER_BAUDRATE; i++)
        {
            if (strBaudrate.CompareNoCase(c_strBaudate[i]) == 0)
            {
                Baudrate_Index = i;
                break;
            }
        }
        m_pComWriter->m_index_Baudrate = Baudrate_Index; //m_combox_baudrate.GetCurSel ();
        m_pComWriter->m_FlashTimes = m_FlashTimes;
        int nPort = GetComPortNo();
        m_pComWriter->SetParentWnd(this);

        m_pComWriter->SetComPortNO(nPort);
        if (com_bin)
        {
            vector<int> szFlags;
            int packet_flag = nDataSize;
            szFlags.push_back(packet_flag);
            m_pComWriter->m_com_flash_binfile = 1;
            m_pComWriter->SetExtendHexFileParam(szFlags, (TS_UC*)m_pFileBuffer);
        }
        else
        {
            if (pHexFile->GetHexFileFormatType() == HEXFILE_DATA)
            {
                m_pComWriter->SetFileBuffer((TS_UC*)m_pFileBuffer, nDataSize);
            }
            else if (pHexFile->GetHexFileFormatType() == HEXFILE_LINERADDR)
            {
                vector<int> szFlags;
                pHexFile->GetExtendHexFileSectionFlag(szFlags);
                m_pComWriter->SetExtendHexFileParam(szFlags, (TS_UC*)m_pFileBuffer);
            }
        }


        CString temp_hex_pid_name;
        MultiByteToWideChar(CP_ACP, 0, (char*)global_fileInfor.product_name, (int)strlen((char*)global_fileInfor.product_name) + 1, temp_hex_pid_name.GetBuffer(MAX_PATH), MAX_PATH);
        temp_hex_pid_name.ReleaseBuffer();
        temp_hex_pid_name.Trim();
        temp_hex_pid_name.MakeUpper();
        //ֻ��Hex PID Ϊminipanel arm �汾ʱ  �� �ſ���Ҫ�޸� bootload �İ汾; - Only when the Hex PID is the minipanel arm version can the bootloader version be repaired.

        float hex_version = 0;


        if (temp_hex_pid_name.CompareNoCase(_T("MINI_ARM")) == 0)
        {
            CString strTips_version;
            hex_version = ((float)(global_fileInfor.software_high * 256 + global_fileInfor.software_low)) / 100;
            strTips_version.Format(_T("Hex file firmware version : %.2f"), hex_version);
            n_hex_file_pid = PM_MINIPANEL_ARM;
            if (hex_version >= 60) //��������汾��˵����Ҫ512��flash �������ȸ���bootload; - If it is greater than this version, it means that 512 flash is needed, and the bootloader must be updated first.
            {
                firmware_must_use_new_bootloader = 1;
                if (new_bootload == 0)   //�ڶ����� new boot �� ·������Ҫ������ǵ�һ���·��; - The second time is the path of the new boot, and the path of the first time needs to be saved.
                    g_update_newfirmware_file_path = m_strHexFileName;
            }
        }
        else if ((temp_hex_pid_name.CompareNoCase(_T("TSTAT8")) == 0) && (new_bootload == 0))
        {
            CString strTips_version;
            hex_version = ((float)(global_fileInfor.software_high * 256 + global_fileInfor.software_low));
            strTips_version.Format(_T("Hex file firmware version : %.2f"), hex_version);
            n_hex_file_pid = PM_MINIPANEL_ARM;
            if (hex_version >= 101) //��������汾��˵����Ҫ512��flash �������ȸ���bootload; - If it is greater than this version, it means that 512 flash is needed, and the bootloader must be updated first.
            {
                firmware_must_use_new_bootloader = 1;
                if (new_bootload == 0)   //�ڶ����� new boot �� ·������Ҫ������ǵ�һ���·��; - The second time is the path of the new boot, and the path of the first time needs to be saved.
                    g_update_newfirmware_file_path = m_strHexFileName;
            }
        }
        else if ((temp_hex_pid_name.CompareNoCase(_T("PID10")) == 0) && (new_bootload == 0))
        {
            CString strTips_version;
            hex_version = ((float)(global_fileInfor.software_high * 256 + global_fileInfor.software_low));
            strTips_version.Format(_T("Hex file firmware version : %.2f"), hex_version);
            n_hex_file_pid = PM_TSTAT10;
            if (hex_version >= 5109) //��������汾��˵����Ҫ512��flash �������ȸ���bootload; - If it is greater than this version, it means that 512 flash is needed, and the bootloader must be updated first.
            {
                firmware_must_use_new_bootloader = 1;
                if (new_bootload == 0)   //�ڶ����� new boot �� ·������Ҫ������ǵ�һ���·��; - The second time is the path of the new boot, and the path of the first time needs to be saved.
                    g_update_newfirmware_file_path = m_strHexFileName;
            }
        }
        else if ((temp_hex_pid_name.CompareNoCase(_T("CO2ALL")) == 0) && (new_bootload == 0))
        {
            CString strTips_version;
            hex_version = ((float)(global_fileInfor.software_high * 256 + global_fileInfor.software_low));
            strTips_version.Format(_T("Hex file firmware version : %.2f"), hex_version);
            n_hex_file_pid = STM32_CO2_NET;
            if (hex_version >= 59) //��������汾��˵����Ҫ512��flash �������ȸ���bootload; - If it is greater than this version, it means that 512 flash is needed, and the bootloader must be updated first.
            {
                firmware_must_use_new_bootloader = 1;
                if (new_bootload == 0)   //�ڶ����� new boot �� ·������Ҫ������ǵ�һ���·��; - The second time is the path of the new boot, and the path of the first time needs to be saved.
                    g_update_newfirmware_file_path = m_strHexFileName;
            }
        }

        int nRet = m_pComWriter->BeginWirteByCom();

        // Disable flash button
        if (nRet != 0) // Started writing.
        {
            EnableFlash(FALSE);
        }
    }
    else
    {
        CString strTips1 = _T("|File error: The file is not a properly formatted firmware file.");
        UpdateStatusInfo(strTips1, FALSE);
        CString strTips2 = _T("|Please select another file.");
        UpdateStatusInfo(strTips2, FALSE);
        if (!auto_flash_mode)
            AfxMessageBox(strTips1 + strTips2, MB_OK);

    }

    delete pHexFile;
}

int	CISPDlg::GetComPortNo()
{
    CString strComPort;
    m_ComPort.GetWindowText(strComPort);
    CString c = strComPort.Mid(3);
    int nPort = _wtoi (c.GetBuffer());
    return nPort;
}
void CISPDlg::OnBnClickedButtonPing2()
{
    UpdateData(TRUE);
    CString strIP,strPort;
    GetDlgItem(IDC_IPADDRESS_NC)->GetWindowText(strIP);
    if (strIP.GetLength() <= 6)
    {
        if(!auto_flash_mode)
            AfxMessageBox(_T("Please Input an IP address."));
        return;
    }
    OnTestPing(strIP);
    //Show_Flash_DeviceInfor_NET();

}
void Split_Cstring(CString Str,vector<CString>& ret_str,CString split_str)
{
    CString temp,str_temp;
    int index=0,start=0,count=-1;
    while (start<Str.GetLength())
    {
        int index=Str.Find(split_str,start);
        if (index!=-1)
        {
            str_temp.Empty();
            ++count;
            for (int i=start; i<index; i++ )
            {
                temp.Format(_T("%C"),Str.GetAt(i));
                str_temp+=temp;
            }
            ret_str.push_back(str_temp);
            start=index+1;
        }
        else
            break;

    }

}
//Update the product information that is being flashed
afx_msg LRESULT CISPDlg::Show_Flash_DeviceInfor(WPARAM wParam, LPARAM lParam)
{
    CString Device_info = CString(LPTSTR(lParam));
    vector<CString> vet_Device;
    Split_Cstring(Device_info,vet_Device,_T(" "));

    unsigned short US_Device_infor[10];
    for (int i=0; i<10; i++)
    {
        US_Device_infor[i]=_wtoi(vet_Device[i]);
    }

    int DeviceModel=-1;
    //DeviceModel=read_multi(ID,&Device_infor[0],0,10);//Get the model number that is being flashed, returns the name of the product

    m_ModelName=GetProductName(US_Device_infor[7]);
    if (m_ModelName.Find(_T("ERROR"))!=0)
    {

        float tstat_version2 = (float)(US_Device_infor[MODBUS_VERSION_NUMBER_HI]*256+US_Device_infor[MODBUS_VERSION_NUMBER_LO]);
        tstat_version2 /=10;

        m_FirmVer.Format(_T("%.1f"),tstat_version2);
        m_HardVer.Format(_T("%d"),US_Device_infor[MODBUS_HARDWARE_REV]);
        /*չʾ����Ϣlog���� - Show in message log box */
        UpdateStatusInfo(_T("|Device Information-Begin"),FALSE);
        UpdateStatusInfo(_T("|Model  Name:")+m_ModelName,FALSE);
        UpdateStatusInfo(_T("|Firmware Ver:")+m_FirmVer,FALSE);
        UpdateStatusInfo(_T("|Hardware Ver:")+m_HardVer,FALSE);
        UpdateStatusInfo(_T("|Device Information-End"),FALSE);
        UpdateData(FALSE); //��ʾ�ڽ����� - Show on the interface
    }
    else
    {
        m_ModelName=_T("");
        m_FirmVer=_T("");
        m_HardVer=_T("");
    }




    return TRUE;
    //  close_com();
}

BOOL CISPDlg::Show_Flash_DeviceInfor_NET()
{
    UpdateData(TRUE);
    CString strIP;
    GetDlgItem(IDC_IPADDRESS_NC)->GetWindowText(strIP);


    if (Open_Socket_Retry(strIP,m_IPPort))
    {
        int DeviceModel=-1;
        DeviceModel=read_one(255,MODBUS_PRODUCT_MODEL);//Get the model number that is being flashed, returns the name of the product
        if (DeviceModel<0)//Modbus does not communicate, 
        {
            return TRUE;
        }
        m_ModelName=GetProductName(DeviceModel);
        CString modelname=m_ModelName;
        CString productname=m_strProductName;
        modelname.MakeUpper();
        productname.MakeUpper();
        if (productname.Find(modelname)==-1)
        {
            CString infor;
            infor.Format(_T("The Bin file is for %s,but your device is %s"),m_strProductName.GetBuffer(),m_ModelName.GetBuffer());
            if(!auto_flash_mode)
                AfxMessageBox(infor);
            UpdateStatusInfo(infor,FALSE);
            return FALSE;
        }

        if (m_ModelName.Find(_T("ERROR"))!=0)
        {
            float tstat_version2=(float)read_one(255,MODBUS_VERSION_NUMBER_LO); //device version
            if(tstat_version2<=0)
                m_FirmVer.Format(_T("%0.1f"),tstat_version2);
            if(tstat_version2 >=240 && tstat_version2 <250)
                tstat_version2 /=10;
            else
            {
                tstat_version2 = (float)(read_one(255,MODBUS_VERSION_NUMBER_HI)*100+read_one(255,MODBUS_VERSION_NUMBER_LO));
                tstat_version2 /=100;
            }
            m_FirmVer.Format(_T("%.1f"),tstat_version2);
            m_HardVer.Format(_T("%d"),read_one(255,MODBUS_HARDWARE_REV));
            /*չʾ����Ϣlog����*/
            UpdateStatusInfo(_T("|Device Information-Begin"),FALSE);
            UpdateStatusInfo(_T("|Model  Name:")+m_ModelName,FALSE);
            UpdateStatusInfo(_T("|Firmware Ver:")+m_FirmVer,FALSE);
            UpdateStatusInfo(_T("|Hardware Ver:")+m_HardVer,FALSE);
            UpdateStatusInfo(_T("|Device Information-End"),FALSE);
            UpdateData(FALSE);
        }
        else
        {
            m_ModelName=_T("");
            m_FirmVer=_T("");
            m_HardVer=_T("");

        }
        return TRUE;
    }
    else
    {
        CString tips;
        tips.Format(_T("IP:%s,Port:%d can't open,You can Ping to test the connection"),strIP,m_IPPort);
        UpdateStatusInfo(tips,FALSE);
        return FALSE;
    }
    UpdateData(FALSE);
    close_com();
}
CString CISPDlg::Get_NET_Infor(CString strIPAdress,short nPort)
{
    CString modelname(_T(""));
    if (Open_Socket_Retry(strIPAdress,nPort))
    {
        int DeviceModel=-1;
        DeviceModel=read_one(255,MODBUS_PRODUCT_MODEL);//Got the model number of device, returns the name of the product
        modelname=GetProductName(DeviceModel);
        //m_ModelName.Format(_T("%d"),DeviceModel);
        if(modelname.Find(_T("ERROR"))!=-1)
        {
            return modelname;
        }

    }
    else
    {
        CString tips;
        tips.Format(_T("IP:%s,Port:%d can't open, press Ping to test the connection"),strIPAdress,nPort);
        UpdateStatusInfo(tips,FALSE);
    }

    return 	modelname;

}

void CISPDlg::OnMainClear()
{
    OnBnClickedClearLog();

}
void CISPDlg::OnSaveLogInfo()
{
    OnBnClickedSaveLog();
}


void CISPDlg::OnBnClickedClearLog()
{
    m_lbStatusInfo.ResetContent();
}


//* Write log information to a file 
void CISPDlg::OnBnClickedSaveLog()
{
    if (m_lbStatusInfo.GetCount()!=0)
    {

        if(m_plogFile->Open(m_strLogFileName.GetString(),CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate ))
        {
            for (int i=0; i<m_lbStatusInfo.GetCount(); i++)
            {
                CString logInfo;
                m_lbStatusInfo.GetText(i,logInfo);
                m_plogFile->WriteString(logInfo+_T("\n"));
            }

            m_plogFile->Close();
        }
        else
        {
            ASSERT(0);
            m_plogFile->Close();

        }

        //Open File
        //LPCSTR filename=m_strLogFileName.GetBuffer();
        //ShellExecute(NULL, "open","Log_info.txt",NULL, NULL, SW_SHOWNORMAL);
        ShellExecute(this->m_hWnd, _T("open"), m_strLogFileName, NULL, NULL, SW_SHOWNORMAL);
    }
    else
    {
        //Empty log
    }


}
//Command events can be triggered only when responding to context menus
void CISPDlg::OnContextMenu(CWnd*  pWnd , CPoint  point )
{
    //CPoint pt;	  //Gets the position of the mouse when no longer OnContextMenu
    //::GetCursorPos(&pt);
    //
    CRect rect;
    (GetDlgItem(IDC_LIST_INFO))->GetWindowRect(&rect);
    /* ScreenToClient(&rect);*/

    if (rect.PtInRect(point))
    {
        CWnd* pBtn = GetDlgItem(IDC_BUTTON_FLASH);
        if ((m_lbStatusInfo.GetCount()>0)&&pBtn->IsWindowEnabled())
        {
            CMenu menu;
            menu.LoadMenu(IDR_MENU1);//Menu Resource ID
            menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,point.x,point.y,this);     //m_newListCtrl��CListCtrl����
            menu.DestroyMenu();
        }

    }

}
/* Function: According to Model Name,return the hex file prefix of the Device Model Name */
CString CISPDlg::GetFilePrefix_FromDB(const CString& ModeName)
{
    _ConnectionPtr m_pConnection;
    AfxOleInit();
    m_pConnection.CreateInstance(__uuidof(Connection));
    try
    {

        m_pConnection->Open(m_strDatabasefilepath.GetString(),"","",adModeUnknown);

    }
    catch(_com_error e)
    {
        if(!auto_flash_mode)
            AfxMessageBox(_T("The database connection failed to confirm that the database Demo.mdb is under the current path!"));
    }
    _RecordsetPtr m_pRecordset;
    m_pRecordset.CreateInstance(__uuidof(Recordset));

    // ��ADO�����н��������Ҫ����try...catch()�����������Ϣ,
    // ��Ϊ����ʱ�ᾭ������һЩ���벻���Ĵ���jingzhou xu
       // The Try...catch () is used in the proposed statement in the ADO operation to catch the error message.
     //Because it can return unexpected errors.
    
    
    try
    {
        CString sql;
        sql.Format(_T("select HexPrefix from Products where Model='%s'"),ModeName);
        m_pRecordset->Open((_variant_t)sql,_variant_t((IDispatch *)m_pConnection,true),adOpenStatic,adLockOptimistic,adCmdText);
    }
    catch(_com_error *e)
    {
        if(!auto_flash_mode)
            AfxMessageBox(e->ErrorMessage());
    }
    _variant_t var;
    CString str_FilePrefix=_T("");
    try
    {
        if(!m_pRecordset->EndOfFile)  //Indicates that the product table has valid data
        {
            m_pRecordset->MoveFirst();
            var = m_pRecordset->GetCollect("HexPrefix");
            if(var.vt != VT_NULL)
                str_FilePrefix = (LPCSTR)_bstr_t(var);
        }
        else
        {
            //AfxMessageBox(_T("The data in the table is empty"));	 //No qualifying data, return NULL

            //return str_FilePrefix;
        }


    }
    catch(_com_error *e)
    {
        if(!auto_flash_mode)
            AfxMessageBox(e->ErrorMessage());
    }
    m_pRecordset->Close();
    m_pConnection->Close();

    return str_FilePrefix;

}
/*
Function:
Determine if the name of the device is compatible with the selected file
There are three possible situations
1.Match
2.No match
3.The database does not exist
*/
int CISPDlg::Judge_Model_Version()
{
    CString File_Prefix=GetFilePrefix_FromDB(m_ModelName);
    CStringArray strArry;
    //char div='\\';
    //decodeCString(m_strHexFileName,strArry,div);
    CString flashfileName=GetFileName_FromFilePath();//strArry[strArry.GetCount()-1];
    flashfileName.MakeUpper();
    File_Prefix.MakeUpper();
    if (File_Prefix.IsEmpty())
    {
        return NO_HEXFILEPREFIX;
    }
    else if (flashfileName.Find(File_Prefix)!=-1) //String Lookup
    {
        return OK_HEXFILEPREFIX;
    }
    else
    {
        return ERR_HEXFILEPREFIX;
    }

}



void CISPDlg::OnMenuApp()
{
    CDialog::OnOK();
}


void CISPDlg::OnMenuAbout()
{
    CAboutDlg dlg;
    dlg.DoModal();
}
/*
Function:
Extracts the name of a file from a user-selected file path��
*/
CString CISPDlg::GetFileName_FromFilePath()
{
    CStringArray strArry;
    ExtractString(strArry,m_strHexFileName,_T("\\"));
    CString str;
    str.Format(_T("%s"),strArry.GetAt(strArry.GetCount()-1));
    return str;
}




void CISPDlg::FlashSN()
{

    if (GetCommunicationType () == 0)
    {
        if (!DetectBraudrate ())
        {
            UpdateStatusInfo(_T("Detecting your Braudrate ,Failed"), FALSE);
            return;
        }
    }
    UpdateStatusInfo(_T("Detecting braudrate finished"), FALSE);
    CFlashSN* pFlashSN = new CFlashSN;
    int nComport = GetComPortNo();

    pFlashSN->SetComport(nComport);
    pFlashSN->SetFlashTpye(0);
    CString strBraudrate;
    GetDlgItem (IDC_EDIT_BAUDRATE)->GetWindowTextW(strBraudrate);
    pFlashSN->SetBrandrate (_wtoi (strBraudrate));
    if(!GetModbusID(m_szMdbIDs))
    {
        if(!auto_flash_mode)
            AfxMessageBox(_T("Please input a device ID "));
        return ;
    }
    int nHWVerison;
    int nSoftwareVersion;
    //	int nProductModel;
    CString  strProductModel;

    int ProductID;

    GetFlashSNParam(nHWVerison, strProductModel,ProductID);
    CString strSWV;
    CWnd* pWnd = GetDlgItem(IDC_EDIT_HWVERSION);
    pWnd->GetWindowText(strSWV);
    nSoftwareVersion = _wtoi(strSWV);
    pFlashSN->SetMBID(m_szMdbIDs[0]);

    CString username;
    //pWnd = GetDlgItem(IDC_FLASH_USER_ID);
    //pWnd->GetWindowText(username);
    username = _T("T3000");
    pFlashSN->SetFlashParam((static_cast<const CString>(strProductModel)), nHWVerison,ProductID,nSoftwareVersion,username);

    pFlashSN->StartWriteSN();
    CString SN;
    SN.Format(_T("%d"),pFlashSN->m_nSerialNumber);
    GetDlgItem(IDC_EDIT_SN)->SetWindowText(SN);
    delete pFlashSN;
}

// void CISPDlg::OnBnClickedButtonSelfile2()
// {
//     UpdateData();
//     //CString strFilter = _T("hex File|*.hex|bin File|*.bin|all File|*.*||");
//     CString strFilter = _T("bin File|*.bin|all File|*.*||");
//     CFileDialog dlg(true,_T("hex"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
//     dlg.DoModal();
//     m_strFirmwarepath=dlg.GetPathName();
//     // m_strFlashFileName=dlg.GetPathName();
//
//     CWnd* pEditFilePath = (CWnd*)GetDlgItem(IDC_EDIT_FILEPATH2);
//     pEditFilePath->SetWindowText(m_strFirmwarepath);
// }
void CISPDlg::OnMenuSetting()
{
    // TODO: Add your command handler code here
    CISPSetting Dlg;
    Dlg.DoModal();
}


//DWORD WINAPI GetFWFileProc(LPVOID lPvoid)
//{
//	return 0;
//}

static int start_test_time = 30;

void CISPDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    switch (nIDEvent)
    {
    case 1:
    {
        KillTimer(1);
        OnBnClickedButtonFlash();
    }
    break;
    case SN_MAC_SHOW_TIMER:
    {
        m_static_info.SetWindowTextW(sn_mac_info);
        if (ready_towrite_sn != 0)
        {
            CString temp_cs;
            temp_cs.Format(_T("%u"), ready_towrite_sn);
            GetDlgItem(IDC_EDIT_SN)->SetWindowTextW(temp_cs);
        }
        else
            GetDlgItem(IDC_EDIT_SN)->SetWindowTextW(_T(""));

        if ((g_write_mac == 1) || (g_write_wifi_mac == 1))
        {
            CString temp_mac;
            temp_mac.Format(_T("%02x-%02x-%02x-%02x-%02x-%02x"), ready_towrite_mac[0], ready_towrite_mac[1], ready_towrite_mac[2], ready_towrite_mac[3], ready_towrite_mac[4], ready_towrite_mac[5] );
            temp_mac.MakeUpper();
            GetDlgItem(IDC_EDIT_MAC_ADDRESS)->SetWindowTextW(temp_mac);
        }
    }
    break;
    case TEST_ISP_TIMER:
    {
        if (start_test_time == 0)
        {
            KillTimer(TEST_ISP_TIMER);
            ::PostMessage(GetDlgItem(IDC_BUTTON_FLASH)->m_hWnd, WM_LBUTTONDOWN, NULL, NULL);
            ::PostMessage(GetDlgItem(IDC_BUTTON_FLASH)->m_hWnd, WM_LBUTTONUP, NULL, NULL);
            start_test_time = TEST_FINISHED_WAIT_TIME;
            break;
        }
        CString test_info;
        test_info.Format(_T("Next test will start in %d (s)"), start_test_time);


        int nCount = test_info.GetLength();
        WCHAR* strNew = new WCHAR[nCount + 1];
        ZeroMemory(strNew, (nCount + 1) * sizeof(WCHAR));
        LPCTSTR str = LPCTSTR(test_info);
        memcpy(strNew, str, nCount * sizeof(WCHAR));
        if (start_test_time == TEST_FINISHED_WAIT_TIME)
            PostMessage(WM_ADD_STATUSINFO, 0, LPARAM(strNew));
        else
            PostMessage(WM_REPLACE_STATUSINFO, 0, LPARAM(strNew));

        start_test_time--;
    }
    break;
    }
    __super::OnTimer(nIDEvent);
}


BOOL CISPDlg::DetectBraudrate ()
{
    UpdateStatusInfo(_T("Detecting the com port baudrate......"), FALSE);
    m_szMdbIDs.clear ();
    GetModbusID (m_szMdbIDs);
    if (m_szMdbIDs.size ()<=0)
    {
        return FALSE;
    }

    int comport = GetComPortNo ();
    CString strBraudrate,strTips;
	m_Brandrate = 19200;
    if (open_com (comport))
    {

        //m_combox_baudrate.GetCurSel ();

        m_combox_baudrate.GetWindowTextW(strBraudrate);
        int intBraudreate =_wtoi(strBraudrate);
        strTips.Format (_T("Detecting your current Braudrate:%s"),strBraudrate);
        UpdateStatusInfo(strTips, FALSE);
        BOOL Ret=  Change_BaudRate (intBraudreate);
		m_Brandrate = intBraudreate;
        ASSERT(Ret) ;
        Sleep (500);

        if (read_one (m_szMdbIDs[0],6)>0)
        {
            // m_combox_baudrate.SetCurSel (braudrate);

            strTips.Format (_T("Detecting the com port baudrate:%s,Successful."),strBraudrate);
            UpdateStatusInfo(strTips, FALSE);

            return TRUE;
            //break;
        }
        for (int braudrate = 0; braudrate<NUMBER_BAUDRATE; braudrate++)
        {
            int intBraudreate =_wtoi(c_strBaudate[braudrate]);
            Ret= Change_BaudRate (intBraudreate);
			m_Brandrate = intBraudreate;
            ASSERT(Ret) ;
            Sleep (500);

            if (read_one (m_szMdbIDs[0],6)>0)
            {
                m_combox_baudrate.SetCurSel (braudrate);

                strTips.Format (_T("Detecting your Baudrate:%d,Successful."),intBraudreate);
                UpdateStatusInfo(strTips, FALSE);

                return TRUE;
                //break;
            }
            else
            {
                strTips.Format (_T("Detecting your Baudrate:%d,Failed."),intBraudreate);
                UpdateStatusInfo(strTips, FALSE);
            }
        }
        strTips.Format (_T("Detecting your current baudrate: Finished"));
        UpdateStatusInfo(strTips, FALSE);
        close_com ();
    }

    strTips.Format (_T("Detecting your Baudrate: Failed"));
    UpdateStatusInfo(strTips, FALSE);

    return FALSE;


}







LRESULT CISPDlg::Fresh_CloseSN_fcuntion(WPARAM wParam, LPARAM lParam)
{
    Sleep(1000);
    KillTimer(SN_MAC_SHOW_TIMER);
    GetDlgItem(IDC_BUTTON_FLASH_SN)->EnableWindow(TRUE);
    GetDlgItem(IDC_CHECK_FLASH_MAC)->EnableWindow(TRUE);
    GetDlgItem(IDC_CHECK_FLASH_WIFI_MAC)->EnableWindow(TRUE);
    close_com();
    h_sn_mac_thread = NULL;
    return 1;
}
DWORD WINAPI  CISPDlg::SN_MAC_Threadfun(LPVOID lpVoid)
{
    CISPDlg * m_parent = (CISPDlg *)lpVoid;
    unsigned short temp_read_reg[10];

    if (b_com_or_ip == 0)
    {
        SetCommunicationType(0);
        if (open_com(g_comport) == false)
        {
            sn_mac_info.Format(_T("�򿪴���%d ʧ��"), g_comport);
            Sleep(1000);
            ::PostMessage(m_parent->m_hWnd, WM_CLOSE_THREAD_MESSAGE, NULL, NULL);
            return 1;
        }
        else
        {
            sn_mac_info.Format(_T("�򿪴���%d �ɹ�"), g_comport);
            Sleep(1000);
        }
            Change_BaudRate(g_mac_baudrate);
    }
    else if (b_com_or_ip == 1)
    {
        SetCommunicationType(1);
        int nret = 0;
        for (int i = 0; i < 3; i++)
        {
            nret = Open_Socket_Retry(g_mac_ip, g_mac_port);
            if (nret > 0)
                break;
        }
        if (nret > 0)
        {
            sn_mac_info.Format(_T("����IP��%s,�˿�:%d �ɹ�"), g_mac_ip, g_mac_port);
            Sleep(1000);
        }
        else
        {
            sn_mac_info.Format(_T("����IP��%s,�˿�:%d ʧ��"), g_mac_ip, g_mac_port);
            Sleep(1000);
            ::PostMessage(m_parent->m_hWnd, WM_CLOSE_THREAD_MESSAGE, NULL, NULL);
            return 1;
        }
    }




    int temp_mu_ret = 0;
    temp_mu_ret = read_multi_tap(255, temp_read_reg, 0, 10);
    if (temp_mu_ret < 0)
    {
        sn_mac_info.Format(_T("��ȡ�豸������Ϣʧ��"));
        Sleep(1000);
        ::PostMessage(m_parent->m_hWnd,  WM_CLOSE_THREAD_MESSAGE, NULL, NULL);
        return 1;
    }
    else
    {
        sn_mac_info.Format(_T("��ȡ�豸������Ϣ�ɹ�"));
        Sleep(1000);
    }
    int n_ret_id = 0;
    int n_ret = 0;
    n_ret = Write_One_Retry(temp_read_reg[6], 16, 142);
    n_ret = Write_One_Retry(temp_read_reg[6], 16, 142);
    //if ((n_ret < 0) && (b_com_or_ip == 0)  &&  (n_ignore_15_142_command == 0))
    //{
    //    sn_mac_info.Format(_T("��ʼ��д������ʧ��"));
    //    ::PostMessage(m_parent->m_hWnd, WM_CLOSE_THREAD_MESSAGE, NULL, NULL);
    //    return 1;
    //}
    //else
    //{
        sn_mac_info.Format(_T("��ʼ��д������ɹ�"));
        Sleep(1000);

        for (int i = 0; i < n_mac_reboot_time; i++)
        {
            sn_mac_info.Format(_T("�ȴ��豸���� (%d)"), n_mac_reboot_time - i);
            Sleep(1000);
        }

        n_ret = Write_One_Retry(temp_read_reg[6], 16, 1,10);
        if ((n_ret < 0) && (b_com_or_ip == 0))
        {
            sn_mac_info.Format(_T("д������ʹ�豸��ת��Ӧ�ô���ʧ��"));
            Sleep(2000);
            ::PostMessage(m_parent->m_hWnd, WM_CLOSE_THREAD_MESSAGE, NULL, NULL);
            return 1;
        }
        else
        {
            sn_mac_info.Format(_T("д������ʹ�豸��ת��Ӧ�ô���ɹ�"));
            Sleep(2000);
        }

        for (int i = 0; i < n_mac_reboot_time; i++)
        {
            sn_mac_info.Format(_T("�ȴ��豸��ת��Ӧ�ô��� (%d)"), n_mac_reboot_time - i);
            Sleep(1000);
        }

    //}

    CBADO monitor_bado;
    monitor_bado.SetDBPath(_T("Z:\\Serial_Records\\serial_records.mdb"));
    monitor_bado.OnInitADOConn();

    CString strSql;
    strSql.Format(_T("select top 1 * from serial_records order by Serial_Number desc"));
    monitor_bado.m_pRecordset = monitor_bado.OpenRecordset(strSql);
    int device_id_count = monitor_bado.GetRecordCount(monitor_bado.m_pRecordset);
    _variant_t temp_variant;
    while (VARIANT_FALSE == monitor_bado.m_pRecordset->EndOfFile)
    {
        unsigned int temp_serial_number;
        temp_variant = monitor_bado.m_pRecordset->GetCollect("Serial_Number");
        if (temp_variant.vt != VT_NULL)
            temp_serial_number = temp_variant;

        ready_towrite_sn = temp_serial_number + 1;
        monitor_bado.m_pRecordset->MoveNext();
    }
    sn_mac_info.Format(_T("��ȡ�������к�: %u"), ready_towrite_sn);

    CTime Time_Click = CTime::GetCurrentTime();
    CString temp_g_write_hardware_version;
    temp_g_write_hardware_version.Format(_T("%u"), g_write_hardware_version);
    CString temp_serial_pid_type_name = GetProductName(temp_read_reg[7]);
    CString temp_time_format = Time_Click.Format(_T("%y/%m/%d %H:%M:%S"));
    CString temp_software_version;
    temp_software_version.Format(_T("%.1f"), (temp_read_reg[5] * 10 + temp_read_reg[4]) / 10.0);
    strSql.Format(_T("insert into serial_records values(%i,'%s','%s','%s','%s')"), ready_towrite_sn, temp_serial_pid_type_name, temp_g_write_hardware_version, temp_software_version, temp_time_format);
    monitor_bado.m_pConnection->Execute(strSql.GetString(), NULL, adCmdText);
    Sleep(1000);

    unsigned short low_serial = 0;
    unsigned short high_serial = 0;
    low_serial = ready_towrite_sn % 65536;
    high_serial = ready_towrite_sn / 65536;

    int n_ret_low = 0;
    int n_ret_high = 0;
    n_ret_low = Write_One_Retry(temp_read_reg[6], 0, low_serial,6);
    Sleep(1000);
    n_ret_high = Write_One_Retry(temp_read_reg[6], 2, high_serial,6);

    if ((n_ret_low < 0) || (n_ret_high < 0))
    {
        sn_mac_info.Format(_T("д�����к�ʧ��"));
        Sleep(1000);
        ::PostMessage(m_parent->m_hWnd, WM_CLOSE_THREAD_MESSAGE, NULL, NULL);
        return 1;
    }
    else
    {
        sn_mac_info.Format(_T("д�����кųɹ�:%u"), ready_towrite_sn);
        Sleep(1000);
    }

    n_ret_id = Write_One_Retry(temp_read_reg[6], 6, temp_read_reg[6],6);
    if (n_ret_id < 0)
    {
        sn_mac_info.Format(_T("д��Modbus IDʧ��"));
        Sleep(1000);
        ::PostMessage(m_parent->m_hWnd, WM_CLOSE_THREAD_MESSAGE, NULL, NULL);
        return 1;
    }
    else
    {
        sn_mac_info.Format(_T("д��Modbus ID�ɹ�:%d"), temp_read_reg[6]);
        Sleep(1000);
    }

    if (n_need_write_pid == 1)
    {
        //��Ҫ��д�µĲ�Ʒ�� - The new product ID needs to be written
        int n_ret_pid = Write_One_Retry(temp_read_reg[6], 7, g_sn_product_id,6);
        if (n_ret_id < 0)
        {
            sn_mac_info.Format(_T("д���ƷID��%dʧ��"), g_sn_product_id);
            Sleep(1000);
            ::PostMessage(m_parent->m_hWnd, WM_CLOSE_THREAD_MESSAGE, NULL, NULL);
            return 1;
        }
        else
        {
            sn_mac_info.Format(_T("д���ƷID��%d�ɹ�"), g_sn_product_id);
            Sleep(1000);
        }
    }
    else
    {
        //��Ҫ��д��ԭ�еĲ�Ʒ��  �����дһ�£����� ��Ʒ�����ױ��. - The new original product ID needs to be written, it is best to write it, otherwise the product ID is easy to change.
        int n_ret_pid = Write_One_Retry(temp_read_reg[6], 7, temp_read_reg[7],6);
        if (n_ret_id < 0)
        {
            sn_mac_info.Format(_T("д���ƷID��%dʧ��"), temp_read_reg[7]);
            Sleep(1000);
            ::PostMessage(m_parent->m_hWnd, WM_CLOSE_THREAD_MESSAGE, NULL, NULL);
            return 1;
        }
        else
        {
            sn_mac_info.Format(_T("д���ƷID��%d�ɹ�"), temp_read_reg[7]);
            Sleep(1000);
        }
    }

    

    int n_ret_harware = Write_One_Retry(temp_read_reg[6], 8, g_write_hardware_version,6);
    if (n_ret_id < 0)
    {
        sn_mac_info.Format(_T("д��Ӳ���汾��ʧ��"));
        Sleep(1000);
        ::PostMessage(m_parent->m_hWnd, WM_CLOSE_THREAD_MESSAGE, NULL, NULL);
        return 1;
    }
    else
    {
        sn_mac_info.Format(_T("д��Ӳ���汾�ųɹ�:%d"), g_write_hardware_version);
        Sleep(1000);
    }

    if (handle_write_sensor_info == 1)
    {
        //��Ҫȷ�� �Ƿ�֧��д��sensor ��־; - First, confirm whether to support writing sensor flags;
        unsigned short sensor_check_flag = 0;
        sensor_check_flag = read_one(temp_read_reg[6], FUNCTION_CHECK_SUM_REG, 6);
        sn_mac_info.Format(_T("����д�����ģ��У����Ϣ"));
        Sleep(1000);
        int n_ret_sensor_checksum = Write_One_Retry(temp_read_reg[6], FUNCTION_CHECK_SUM_REG, FUNCTION_CHECK_SUM_VALUE, 6);
        if (n_ret_sensor_checksum < 0)
        {
            sn_mac_info.Format(_T("д�����ģ��У����Ϣ ʧ��"));
            Sleep(1000);
            ::PostMessage(m_parent->m_hWnd, WM_CLOSE_THREAD_MESSAGE, NULL, NULL);
            return 1;
        }
        sn_mac_info.Format(_T("����д����豸��ģ����Ϣ"));
        Sleep(1000);
        int n_ret_sensor1 = Write_One_Retry(temp_read_reg[6], FUNCTION_SENSOR1_REG, sensor_value1, 6);
        if (n_ret_sensor1 < 0)
        {
            sn_mac_info.Format(_T("д�����ģ����Ϣ ��1���� ʧ��"));
            Sleep(1000);
            ::PostMessage(m_parent->m_hWnd, WM_CLOSE_THREAD_MESSAGE, NULL, NULL);
            return 1;
        }
        else
        {
            sn_mac_info.Format(_T("д�����ģ����Ϣ ��1���� �ɹ�"));
            Sleep(1000);
        }

        int n_ret_sensor2 = Write_One_Retry(temp_read_reg[6], FUNCTION_SENSOR2_REG, sensor_value2, 6);
        if (n_ret_sensor2 < 0)
        {
            sn_mac_info.Format(_T("д�����ģ����Ϣ ��2���� ʧ��"));
            Sleep(1000);
            ::PostMessage(m_parent->m_hWnd, WM_CLOSE_THREAD_MESSAGE, NULL, NULL);
            return 1;
        }
        else
        {
            sn_mac_info.Format(_T("д�����ģ����Ϣ ��2���� �ɹ�"));
            Sleep(1000);
        }
    }

    if (g_write_mac == 1)
    {
        strSql.Format(_T("select top 1 * from MAC order by ID desc"));
        monitor_bado.m_pRecordset = monitor_bado.OpenRecordset(strSql);
        device_id_count = monitor_bado.GetRecordCount(monitor_bado.m_pRecordset);

        while (VARIANT_FALSE == monitor_bado.m_pRecordset->EndOfFile)
        {
            unsigned char Temp_MAC[6];
            temp_variant = monitor_bado.m_pRecordset->GetCollect("100");
            if (temp_variant.vt != VT_NULL)
                Temp_MAC[0] = temp_variant;

            temp_variant = monitor_bado.m_pRecordset->GetCollect("101");
            if (temp_variant.vt != VT_NULL)
                Temp_MAC[1] = temp_variant;
            temp_variant = monitor_bado.m_pRecordset->GetCollect("102");
            if (temp_variant.vt != VT_NULL)
                Temp_MAC[2] = temp_variant;
            temp_variant = monitor_bado.m_pRecordset->GetCollect("103");
            if (temp_variant.vt != VT_NULL)
                Temp_MAC[3] = temp_variant;
            temp_variant = monitor_bado.m_pRecordset->GetCollect("104");
            if (temp_variant.vt != VT_NULL)
                Temp_MAC[4] = temp_variant;
            temp_variant = monitor_bado.m_pRecordset->GetCollect("105");
            if (temp_variant.vt != VT_NULL)
                Temp_MAC[5] = temp_variant;

            memcpy(ready_towrite_mac, Temp_MAC, 6 * sizeof(char));
            monitor_bado.m_pRecordset->MoveNext();
        }

        ready_towrite_mac[5] ++;
        if (ready_towrite_mac[5] > 254)
        {
            ready_towrite_mac[5] = 0;
            ready_towrite_mac[4]++;
        }

        if (ready_towrite_mac[4] > 254)
        {
            ready_towrite_mac[4] = 0;
            ready_towrite_mac[3]++;
        }

        if (ready_towrite_mac[3] > 254)
        {
            ready_towrite_mac[3] = 0;
            ready_towrite_mac[2]++;
        }

        if (ready_towrite_mac[2] > 254)
        {
            ready_towrite_mac[2] = 0;
            ready_towrite_mac[1]++;
        }

        if (ready_towrite_mac[1] > 254)
        {
            ready_towrite_mac[1] = 0;
            ready_towrite_mac[0]++;
        }
        CString temp_pid_type_name = GetProductName(temp_read_reg[7]);
        strSql.Format(_T("insert into MAC(100,101,102,103,104,105,Product_Name,Serial_Number,Record_Date) values(%i,%i,%i,%i,%i,%i,'%s',%i,'%s')"),
            ready_towrite_mac[0], ready_towrite_mac[1], ready_towrite_mac[2], ready_towrite_mac[3], ready_towrite_mac[4], ready_towrite_mac[5],
            temp_pid_type_name, ready_towrite_sn, temp_time_format);
        monitor_bado.m_pConnection->Execute(strSql.GetString(), NULL, adCmdText);

        sn_mac_info.Format(_T("׼��д��MAC��ַ:%x-%x-%x-%x-%x-%x"), g_write_hardware_version,
            ready_towrite_mac[1], ready_towrite_mac[2], ready_towrite_mac[3], ready_towrite_mac[4], ready_towrite_mac[5]);
        Sleep(1000);

        int n_ret_harware = Write_One_Retry(temp_read_reg[6], 93, 1,6);
        if (n_ret_id < 0)
        {
            sn_mac_info.Format(_T("д��MACʹ������ʧ��"));
            Sleep(1000);
            ::PostMessage(m_parent->m_hWnd, WM_CLOSE_THREAD_MESSAGE, NULL, NULL);
            return 1;
        }
        else
        {
            sn_mac_info.Format(_T("д��MACʹ������ɹ�"));
            Sleep(1000);
        }

        unsigned short write_value[6];
        for (int i = 0; i < 6; i++)
        {
            write_value[i] = ready_towrite_mac[i];
        }

        int n_mac_ret = 0;
        if ((temp_read_reg[7] == PM_MINIPANEL_ARM) || 
            temp_read_reg[7] == PM_MINIPANEL || 
            temp_read_reg[7] == PM_ESP32_T3_SERIES ||
            temp_read_reg[7] == PM_CM5)
        {
            n_mac_ret = write_multi_Short(temp_read_reg[6], write_value,100, 6);
        }
        else
        {
            n_mac_ret = write_multi_Short(temp_read_reg[6], write_value, 40, 6);
        }
        
        if (n_mac_ret < 0)
        {
            sn_mac_info.Format(_T("д��MAC��ַʧ��"));
            Sleep(1000);
            ::PostMessage(m_parent->m_hWnd, WM_CLOSE_THREAD_MESSAGE, NULL, NULL);
            return 1;
        }
        else
        {
            sn_mac_info.Format(_T("д��MAC��ַ�ɹ�"));
            Sleep(1000);
        }
    }




    if (g_write_wifi_mac == 1)
    {
        strSql.Format(_T("select top 1 * from MAC order by ID desc"));
        monitor_bado.m_pRecordset = monitor_bado.OpenRecordset(strSql);
        device_id_count = monitor_bado.GetRecordCount(monitor_bado.m_pRecordset);

        while (VARIANT_FALSE == monitor_bado.m_pRecordset->EndOfFile)
        {
            unsigned char Temp_MAC[6];
            temp_variant = monitor_bado.m_pRecordset->GetCollect("100");
            if (temp_variant.vt != VT_NULL)
                Temp_MAC[0] = temp_variant;

            temp_variant = monitor_bado.m_pRecordset->GetCollect("101");
            if (temp_variant.vt != VT_NULL)
                Temp_MAC[1] = temp_variant;
            temp_variant = monitor_bado.m_pRecordset->GetCollect("102");
            if (temp_variant.vt != VT_NULL)
                Temp_MAC[2] = temp_variant;
            temp_variant = monitor_bado.m_pRecordset->GetCollect("103");
            if (temp_variant.vt != VT_NULL)
                Temp_MAC[3] = temp_variant;
            temp_variant = monitor_bado.m_pRecordset->GetCollect("104");
            if (temp_variant.vt != VT_NULL)
                Temp_MAC[4] = temp_variant;
            temp_variant = monitor_bado.m_pRecordset->GetCollect("105");
            if (temp_variant.vt != VT_NULL)
                Temp_MAC[5] = temp_variant;

            memcpy(ready_towrite_mac, Temp_MAC, 6 * sizeof(char));
            monitor_bado.m_pRecordset->MoveNext();
        }

        ready_towrite_mac[5] ++;
        if (ready_towrite_mac[5] > 254)
        {
            ready_towrite_mac[5] = 0;
            ready_towrite_mac[4]++;
        }

        if (ready_towrite_mac[4] > 254)
        {
            ready_towrite_mac[4] = 0;
            ready_towrite_mac[3]++;
        }

        if (ready_towrite_mac[3] > 254)
        {
            ready_towrite_mac[3] = 0;
            ready_towrite_mac[2]++;
        }

        if (ready_towrite_mac[2] > 254)
        {
            ready_towrite_mac[2] = 0;
            ready_towrite_mac[1]++;
        }

        if (ready_towrite_mac[1] > 254)
        {
            ready_towrite_mac[1] = 0;
            ready_towrite_mac[0]++;
        }
        CString temp_pid_type_name = GetProductName(temp_read_reg[7]);
        strSql.Format(_T("insert into MAC(100,101,102,103,104,105,Product_Name,Serial_Number,Record_Date) values(%i,%i,%i,%i,%i,%i,'%s',%i,'%s')"),
            ready_towrite_mac[0], ready_towrite_mac[1], ready_towrite_mac[2], ready_towrite_mac[3], ready_towrite_mac[4], ready_towrite_mac[5],
            temp_pid_type_name, ready_towrite_sn, temp_time_format);
        monitor_bado.m_pConnection->Execute(strSql.GetString(), NULL, adCmdText);

        sn_mac_info.Format(_T("׼��д��WIFI MAC��ַ:%x-%x-%x-%x-%x-%x"), g_write_hardware_version,
            ready_towrite_mac[1], ready_towrite_mac[2], ready_towrite_mac[3], ready_towrite_mac[4], ready_towrite_mac[5]);
        Sleep(1000);


#if 1 //д wifi mac ��ַ
        //STATIC_IP_START_REG  2070  6 ���Ĵ���


        int n_ret_wifi_enable = Write_One_Retry(temp_read_reg[6], 2008, 0, 6);
        if (n_ret_wifi_enable < 0)
        {
            sn_mac_info.Format(_T("д��ʹ��WIFI MAC��ַ��־λʧ��"));
            Sleep(1000);
            ::PostMessage(m_parent->m_hWnd, WM_CLOSE_THREAD_MESSAGE, NULL, NULL);
            return 1;
        }
        else
        {
            sn_mac_info.Format(_T("д��ʹ��WIFI MAC��ַ��־λ �ɹ�"));
            Sleep(1000);
        }

        unsigned short write_value[6];
        for (int i = 0; i < 6; i++)
        {
            write_value[i] = ready_towrite_mac[i];
            if (write_value[i] < 0x10)
                write_value[i] = write_value[i] + 0x10;        //wifi��mac��ַ ��ЩλλС��0x10 д����ȥ - Some bits of the wifi MAC address are less than 0x10 and cannot be written in
        }
        write_value[0] = 0x18;
        int n_mac_ret = 0;

            n_mac_ret = write_multi_Short(temp_read_reg[6], write_value, 2070, 6);

        if (n_mac_ret < 0)
        {
            sn_mac_info.Format(_T("д��WIFI MAC��ַʧ��"));
            Sleep(1000);
            ::PostMessage(m_parent->m_hWnd, WM_CLOSE_THREAD_MESSAGE, NULL, NULL);
            return 1;
        }
        else
        {
            sn_mac_info.Format(_T("д��WIFI MAC��ַ�ɹ�"));
            Sleep(1000);
        }

        sn_mac_info.Format(_T("У��WIFI MAC��ַ"));
        Sleep(3000);
        unsigned short temp_read_mac[6] = { 0 };
        temp_mu_ret = read_multi_tap(temp_read_reg[6], temp_read_mac, 2070, 6);

        int ret_com = memcmp(write_value, temp_read_mac, 6);
        if (ret_com != 0)
        {
            sn_mac_info.Format(_T("У��WIFI MAC��ַ  ʧ��"));
            Sleep(1000);
            ::PostMessage(m_parent->m_hWnd, WM_CLOSE_THREAD_MESSAGE, NULL, NULL);
            return 1;
        }


#endif
    }



    sn_mac_info.Format(_T("���в����ѳɹ���ɣ�"));
    Sleep(1000);
    monitor_bado.CloseRecordset();
    monitor_bado.CloseConn();

    ::PostMessage(m_parent->m_hWnd, WM_CLOSE_THREAD_MESSAGE, NULL, NULL);
    return true;
}

void CISPDlg::OnBnClickedButtonFlashSn()
{


    // TODO: �ڴ���ӿؼ�֪ͨ���������� - Add control notification handler code here
    GetDlgItem(IDC_EDIT_MAC_ADDRESS)->SetWindowTextW(_T(""));
    GetDlgItem(IDC_EDIT_SN)->SetWindowTextW(_T(""));

    ((CButton *)GetDlgItem(IDC_CHECK_FLASH_MAC))->EnableWindow(false);
    ((CButton *)GetDlgItem(IDC_CHECK_FLASH_WIFI_MAC))->EnableWindow(false);
    
    if (((CButton *)GetDlgItem(IDC_CHECK_FLASH_MAC))->GetCheck())
    {
        g_write_mac = 1;
    }
    else
    {
        g_write_mac = 0;
    }

    if (((CButton *)GetDlgItem(IDC_CHECK_FLASH_WIFI_MAC))->GetCheck())
    {
        g_write_wifi_mac = 1;
    }
    else
    {
        g_write_wifi_mac = 0;
    }

    CString ProductName;
    ((CComboBox *)GetDlgItem(IDC_COMBO_PM))->GetWindowTextW(ProductName);
    
    CString temp_reboot_time;
    GetDlgItem(IDC_EDIT_RESTART_TIME)->GetWindowTextW(temp_reboot_time);
    n_mac_reboot_time = _wtoi(temp_reboot_time);
    if (n_mac_reboot_time == 0)
        n_mac_reboot_time = 10;


    g_comport = GetComPortNo();
    CString temp_baudrate;
    GetDlgItem(IDC_EDIT_BAUDRATE)->GetWindowTextW(temp_baudrate);
    g_mac_baudrate = _wtoi(temp_baudrate);
    CString temp_port;
    GetDlgItem(IDC_IPADDRESS_NC)->GetWindowText(g_mac_ip);
    GetDlgItem(IDC_EDIT_NCPORT)->GetWindowText(temp_port);
    g_mac_port = _wtoi(temp_port);

    if (((CButton *)GetDlgItem(IDC_COM))->GetCheck() == 1)
    {
        b_com_or_ip = 0;
    }
    else if (((CButton *)GetDlgItem(IDC_NET))->GetCheck() == 1)
    {
        b_com_or_ip = 1;
    }
    

    CString temp_hardware_version;
    GetDlgItem(IDC_EDIT_HWVERSION)->GetWindowTextW(temp_hardware_version);
    g_write_hardware_version = _wtoi(temp_hardware_version);

    if (g_write_hardware_version == 0)
    {
        m_static_info.SetWindowTextW(_T("Ӳ����Ʒ�Ų���Ϊ��"));
        ((CButton *)GetDlgItem(IDC_CHECK_FLASH_MAC))->EnableWindow(true);
        ((CButton *)GetDlgItem(IDC_CHECK_FLASH_WIFI_MAC))->EnableWindow(true);
        return;
    }

    vector<ALL_PRODUCT_NAME_NODE>::iterator itr = pid_name_map.begin();
    bool find_product_name = false;
    for (; itr != pid_name_map.end(); itr++)
    {
        if (ProductName.CompareNoCase(itr->name) == 0)
        {
            g_sn_product_id = itr->id;
            n_need_write_pid = itr->need_write_id;// 1;
            find_product_name = true;
            break;
        }
    }

 
    


    if (find_product_name == false)
    {
        m_static_info.SetWindowTextW(_T("��Ʒ��δ����"));
        ((CButton *)GetDlgItem(IDC_CHECK_FLASH_MAC))->EnableWindow(true);
        ((CButton *)GetDlgItem(IDC_CHECK_FLASH_WIFI_MAC))->EnableWindow(true);
        return;
    }
    if (handle_write_sensor_info)
    {
        if (((CButton *)GetDlgItem(IDC_CHECK_NO_ITEM))->GetCheck())
        {
            for (int j = 0; j < 16; j++)
            {
                ((CButton *)GetDlgItem(IDC_CHECK_ITEM1 + j))->SetCheck(false);
            }

            for (int j = 0; j < 16; j++)
            {
                ((CButton *)GetDlgItem(IDC_CHECK_ITEM17 + j))->SetCheck(false);
            }
        }


        sensor_value1 = 0;
        bitset<16> sensor_bit(sensor_value1);
        int n_index_1 = 0;
        if (n_component_count > 16)
            n_index_1 = 16;
        else
            n_index_1 = n_component_count;
        for (int i = 0; i < n_index_1; i++)
        {
            if (((CButton *)GetDlgItem(IDC_CHECK_ITEM1 + i))->GetCheck())
            {
                sensor_bit[i] = 1;
            }
            else
            {
                sensor_bit[i] = 0;
            }
        }
        sensor_value1 = (unsigned short)sensor_bit.to_ulong();
        Sleep(1);

        sensor_value2 = 0;
        bitset<16> sensor2_bit(sensor_value2);

        int n_index_2 = 0;
        if (n_component_count <= 16)
            n_index_2 = 0;
        else
            n_index_2 = n_component_count - 16;
        for (int i = 0; i < n_index_2; i++)
        {
            if (((CButton *)GetDlgItem(IDC_CHECK_ITEM17 + i))->GetCheck())
            {
                sensor2_bit[i] = 1;
            }
            else
            {
                sensor2_bit[i] = 0;
            }
        }
        sensor_value2 = (unsigned short)sensor2_bit.to_ulong();
        Sleep(1);

    }

    if ((sensor_value1 == 0) && (sensor_value2 == 0) && (handle_write_sensor_info == 1))
    {
        if (((CButton *)GetDlgItem(IDC_CHECK_NO_ITEM))->GetCheck()  == false)
        {
            m_static_info.SetWindowTextW(_T("��ȷ���Ҳഫ��������û�У��빴ѡ ������"));
            ((CButton *)GetDlgItem(IDC_CHECK_FLASH_MAC))->EnableWindow(true);
            ((CButton *)GetDlgItem(IDC_CHECK_FLASH_WIFI_MAC))->EnableWindow(true);
            return;
        }
    }
    
    if (h_sn_mac_thread == NULL)
    {
        SetTimer(SN_MAC_SHOW_TIMER, 300, NULL);
        h_sn_mac_thread = CreateThread(NULL, NULL, SN_MAC_Threadfun, this, NULL, NULL);
        CloseHandle(h_sn_mac_thread);
    }

}




void CISPDlg::OnCbnSelchangeComboPm()
{
    // TODO: �ڴ���ӿؼ�֪ͨ���������� - Add control notification handler code here
    CString strModel;
    CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_PM);
    int nSel = pCbx->GetCurSel();

    pCbx->GetLBText(nSel, strModel);


    vector<int>::iterator itr_sensor = support_sensor_id.begin();
    vector<ALL_PRODUCT_NAME_NODE>::iterator itr = pid_name_map.begin();
    handle_write_sensor_info = false;
    for (; itr != pid_name_map.end(); itr++)
    {
        if (strModel.CompareNoCase(itr->name) == 0)
        {
           
            for (; itr_sensor != support_sensor_id.end(); itr_sensor++)
            {
                Sleep(1);
                if (itr->id == itr_sensor[0])
                {
                    handle_write_sensor_info = true;
                    break;
                }
            }

            
            break;
        }
    }
    
    Enable_Sensor_Window(handle_write_sensor_info);
    
}

void CISPDlg::Enable_Sensor_Window(bool nenable)
{
    GetDlgItem(IDC_CHECK_NO_ITEM)->EnableWindow(nenable);
    for (int i = 0; i < 16; i++)
    {
        ((CButton *)GetDlgItem(IDC_CHECK_ITEM1 + i))->EnableWindow(nenable);
    }
    for (int i = 0; i < 16; i++)
    {
        ((CButton *)GetDlgItem(IDC_CHECK_ITEM17 + i))->EnableWindow(nenable);
    }
    if(nenable)
        GetDlgItem(IDC_STATIC_COM_NAME)->SetWindowTextW(_T("���빴ѡ���������"));
    else
        GetDlgItem(IDC_STATIC_COM_NAME)->SetWindowTextW(_T("���蹴ѡ���������"));
}





void CAboutDlg::OnBnClickedOk()
{
    // TODO: �ڴ���ӿؼ�֪ͨ���������� - Add control notification handler code here
    CDialog::OnOK();
}


BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ�� - Add additional initialization here
    CString release_note;
    CString temp;
    temp.Format(_T("Rev6.3.8  (2023-03-31)\r\n  1.Support update Tstat10 when it is in Mstp Mode  .\r\n  Tstat10 firmware version should be larger than 63.5\r\n"));
    release_note = release_note + temp;
    temp.Format(_T("Rev6.2.4  (2020-12-22)\r\n  1.Support update CO2 bootloader  .\r\n  2.If firmware version bigger than 59 \r\n    bootloader smaller than 67 \r\n    it will auto update bootloader\r\n"));
    release_note = release_note + temp;
    temp.Format(_T("Rev6.1.0  (2020-05-19)\r\n  1.Support update T3BB_LB_TB_NB bootloader  .\r\n  2.If firmware version bigger than 60.0 \r\n    bootloader smaller than 58 \r\n    it will auto update bootloader\r\n"));
    release_note = release_note + temp;
    temp.Format(_T("Rev6.0.9  (2020-05-09)\r\n  1.Internal functions, support different modules  .\r\n"));
    release_note = release_note + temp;
    temp.Format(_T("Rev6.0.8  (2019-09-19)\r\n  1.Support CM5 arm version firmware update  .\r\n"));
    release_note = release_note + temp;
    temp.Format(_T("Rev6.0.7  (2019-07-29)\r\n  1.Support TSTAT9 Wifi firmware update  .\r\n"));
    release_note = release_note + temp;
    temp.Format(_T("Rev6.0.5  (2019-04-22)\r\n  1.Support PM5E ARM .\r\n"));
    release_note = release_note + temp;
    temp.Format(_T("Rev6.0.4  (2019-03-18)\r\n  1.Support PM5E ARM .\r\n"));
    release_note = release_note + temp;
    temp.Format(_T("Rev6.0.3  (2019-01-02)\r\n  1.Support PM5E don't check.\r\n"));
    release_note = release_note + temp;
    temp.Format(_T("Rev6.0.2  (2018-12-25)\r\n  1.Support don't check temco firmware.\r\n"));
    release_note = release_note + temp;
    temp.Format(_T("Rev6.0.0  (2018-12-05)\r\n  1.Fix Hex flash ,using the old ISPTOOL may cause device reboot.\r\n  2.Support flash PM2.5\r\n"));
    release_note = release_note + temp;
    temp.Format(_T("Rev5.1.6  (2018-11-29)\r\n  1.Three times faster than before when flash by com port .\r\n"));
    release_note = release_note + temp;
    temp.Format(_T("Rev5.1.5  (2018-11-23)\r\n  1.Support baudrate 76800.\r\n"));
    release_note = release_note + temp;

    SetDlgItemTextW(IDC_EDIT_ISP, release_note);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // �쳣: OCX ����ҳӦ���� FALSE - Exception: OCX property pages should return FALSE
}

void CISPDlg::OnBnClickedCheckNoItem()
{
    // TODO: �ڴ���ӿؼ�֪ͨ���������� - Add control notification handler code here
    CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_NO_ITEM);
    int state = pBtn->GetCheck();
    //�����Ҳ���԰�һ������������ - The above can also be bound to a variable for processing
    if (state == 1) // ѡ�� - Selected
    {
        for (int j = 0; j < 16; j++)
        {
            ((CButton *)GetDlgItem(IDC_CHECK_ITEM1 + j))->SetCheck(false);
        }

        for (int j = 0; j < 16; j++)
        {
            ((CButton *)GetDlgItem(IDC_CHECK_ITEM17 + j))->SetCheck(false);
        }
    }


}
#ifdef ISP_BURNING_MODE
DWORD WINAPI  CISPDlg::Thread_BurningTest_fun(LPVOID lpVoid)
{
    CISPDlg* m_parent = (CISPDlg*)lpVoid;
    unsigned short temp_read_reg[10];
    while (temco_burning_mode)
    {
        if (burning_test_finished == 1)
        {
            burning_test_finished = 0;
            Sleep(30000);
            auto_flash_mode = 1;
            ::PostMessage(m_parent->GetDlgItem(IDC_BUTTON_FLASH)->m_hWnd, WM_LBUTTONDOWN, NULL, NULL);
            ::PostMessage(m_parent->GetDlgItem(IDC_BUTTON_FLASH)->m_hWnd, WM_LBUTTONUP, NULL, NULL);
        }
    }
    h_BurningTest_thread = NULL;
    return 0;
}
int temco_burning_mode = 0;

void CISPDlg::OnBnClickedButtonLoopFlash()
{
    // TODO: �ڴ���ӿؼ�֪ͨ���������� - Add control notification handler code here

    if (temco_burning_mode == 0)
    {
        temco_burning_mode = 1;
        GetDlgItem(IDC_BUTTON_LOOP_FLASH)->SetWindowText(_T("Runing"));
        burning_test_finished = 1;
        h_BurningTest_thread = CreateThread(NULL, NULL, Thread_BurningTest_fun, this, NULL, NULL);
        CloseHandle(h_BurningTest_thread);
    }
    else
    {
        temco_burning_mode = 0;
        auto_flash_mode = 0;
        GetDlgItem(IDC_BUTTON_LOOP_FLASH)->SetWindowText(_T("Burning Flash"));
        OnBnClickedSaveLog();
    }


    
}
#endif
