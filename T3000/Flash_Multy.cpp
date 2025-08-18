// Flash_Multy.cpp : implementation file
// This file added by Fance Du 2014/06

#include "stdafx.h"
#include "T3000.h"
#include "Flash_Multy.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "global_function.h"
#include "Dowmloadfile.h"
#include "../SQLiteDriver/CppSQLite3.h"
#include "BinFileParser.h"
#include "HexFileParser.h"

const DWORD c_nHexFileBufLen = 0x7FFFF;
//int Auto_stage = 0;
HANDLE h_thread_get_version = NULL;
// CFlash_Multy dialog
CString ApplicationFolder;
CString MultyISPtool_path;
extern CString AutoFlashConfigPath;

CString Edit_File_Path;
extern tree_product	m_product_isp_auto_flash;
int Get_File_Version(LPCTSTR filepath,unsigned char &HighVersion,unsigned char &LowVersion);

// Flash start, indicates ongoing 1>Ongoing project: blue color
const int CHANGE_THE_ITEM_COLOR_BLUE = 1;//��д��ʼ����ʾ���ڽ��е�1>���ڽ��е���Ŀ��������ɫ
// Flash failed 2>Flash not successful, red color
const int CHANGE_THE_ITEM_COLOR_RED = 2;//��дʧ��2>��дû�гɹ�,���Ǻ�ɫ
// Flash successful 3>Flash successful, green color
const int CHANGE_THE_ITEM_COLOR_GREEN = 3;//��д�ɹ�3>��д�ɹ�������ɫ
// Default
const int  CHANGE_THE_ITEM_COLOR_DEFAULT = 4;//Ĭ��
// 4>Flash successful && config file successful
const int CHANGE_THE_ITEM_COLOR_MORE_GREEN = 5; //4>��д�ɹ�&&�����ļ����ɹ�,
// 5>Config file failed
const int CHANGE_THE_ITEM_COLOR_LESS_RED = 6;//5>�����ļ���ʧ��

// Thread passes display messages to interface list
const int MASS_FLASH_MESSAGE = 200; //�̴߳��ݸ�����List ����ʾ��Ϣ;
// Success
const int OPERATION_SUCCESS = 3; //�ɹ�;

const int  CHANGE_ONE_ITEM = 10;

#define FLASH_COLOR_BLUE RGB(50,50,180)
#define FLASH_COLOR_RED  RGB(255,0,0)
#define FLASH_COLOR_GREEN  RGB(0,255,0)
#define FLASH_COLOR_DEFAULT  RGB(0,0,0)

#define CONFIG_COLOR_RED_FAIL  RGB(255,86,86)
#define CONFIG_COLOR_CONFIG_FLASH_GOOD  RGB(86,120,86)


vector <Str_flash_device> flash_device;

vector <Str_download_firmware_info> download_info_type;

IMPLEMENT_DYNAMIC(CFlash_Multy, CDialogEx)

CFlash_Multy::CFlash_Multy(CWnd* pParent /*=NULL*/)
    : CDialogEx(CFlash_Multy::IDD, pParent)
    , m_bool_flash_different_version(TRUE)
{
    m_bTstatLoadFinished=FALSE;
    m_select_all = FALSE;
    m_Start= FALSE;
}

CFlash_Multy::~CFlash_Multy()
{
}

void CFlash_Multy::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_FLASH_MULTY, m_flash_multy_list);
    DDX_Control(pDX, IDC_LIST_MULTY_INFORMATION, m_multy_flash_listbox);
    DDX_Control(pDX, IDC_EDIT_MULTI_FILE_POSITION2, m_configure_edit);
    DDX_Control(pDX, IDC_ALLCHECK, m_AllCheckBtn);
    DDX_Check(pDX, IDC_CHECK_FLASH_DIFFERENT_VERSION, m_bool_flash_different_version);
}


BEGIN_MESSAGE_MAP(CFlash_Multy, CDialogEx)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BUTTON_APPLY_WO_ALL_SELECT, &CFlash_Multy::OnBnClickedButtonApplyWoAllSelect)
    ON_BN_CLICKED(IDC_BUTTON_MULTY_SEL, &CFlash_Multy::OnBnClickedButtonMultySel)
    ON_BN_CLICKED(IDC_BUTTON_STATRT, &CFlash_Multy::OnBnClickedButtonStatrt)
    ON_WM_TIMER()
    ON_MESSAGE(WM_MULTY_FLASH_MESSAGE,MultyFlashMessage)
    ON_NOTIFY(NM_CLICK, IDC_LIST_FLASH_MULTY, &CFlash_Multy::OnNMClickListFlashMulty)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BUTTON_MULTY_SEL2, &CFlash_Multy::OnBnClickedButtonMultySel2)
    ON_BN_CLICKED(IDC_ALLCHECK, &CFlash_Multy::OnBnClickedAllcheck)
    ON_BN_CLICKED(IDC_CHECK1, &CFlash_Multy::OnBnClickedCheck1)
    ON_BN_CLICKED(IDC_BUTTON_APPLY_WO_ALL_SELECT2, &CFlash_Multy::OnBnClickedButtonApplyWoAllSelect2)
    ON_BN_CLICKED(IDC_RESET_ALL, &CFlash_Multy::OnBnClickedResetAll)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BUTTON_UPDATE_FIRMWARE, &CFlash_Multy::OnBnClickedButtonUpdateFirmware)
END_MESSAGE_MAP()


// CFlash_Multy message handlers


BOOL CFlash_Multy::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    //Auto_stage = 0;
    m_strLogFilePath=g_strExePth+_T("Load_config_Log");
    CreateDirectory(m_strLogFilePath,NULL);//create directory file
    Initial_List();
    int nflashitemcount = 0;
    CString StrTemp;
    flash_device.clear();
    int ncount = m_flash_multy_list.GetItemCount();


	 CString	g_configfile_path=g_strExePth+_T("") MY_CONFIG;
                     
				  

    m_bTstatLoadFinished=FALSE;
    m_Start =TRUE;
    for (int i=0; i<ncount; i++)
    {
        nflashitemcount ++;
        Str_flash_device temp;
        temp.nitem = i;

		temp.strSN= m_flash_multy_list.GetItemText(i,FLASH_SERIAL_NUMBER);
		temp.nID = m_flash_multy_list.GetItemText(i,FLASH_ID);
		temp.ncomport = m_flash_multy_list.GetItemText(i,FLASH_COM_PORT);
		temp.nBaudrate = m_flash_multy_list.GetItemText(i,FLASH_BAUDRATE);
		temp.nIPaddress = m_flash_multy_list.GetItemText(i,FLASH_IPADDRESS);
		temp.nipport = m_flash_multy_list.GetItemText(i,FLASH_IPPORT);
		temp.devicename=m_flash_multy_list.GetItemText(i,FLASH_PRODUCT_NAME);
		temp.product_id = _wtoi(m_flash_multy_list.GetItemText(i,FLASH_PRODUCT_ID));
        temp.software_rev = _wtof(m_flash_multy_list.GetItemText(i, FLASH_FIRMWARE_VERSION));
		CString temp_sub = m_flash_multy_list.GetItemText(i,FLASH_SUB_NOTE);
		if(temp_sub.CompareNoCase(_T("YES")) == 0)
		{
			temp.b_is_sub = true;
		}
		else
		{
			temp.b_is_sub = false;
		}

        temp.file_position = m_flash_multy_list.GetItemText(i,FLASH_FILE_POSITION);
        temp.config_file_position=m_flash_multy_list.GetItemText(i,FLASH_CONFIG_FILE_POSITION);


        CString temp_serial;
        temp_serial = temp.strSN;
        GetPrivateProfileInt(_T("MassFlash"), temp_serial, 0, g_ext_mass_flash_path);
        //FLASH_RESULTS
        //FLASH_CONFIG_RESULTS
        StrTemp=m_flash_multy_list.GetItemText(i,FLASH_RESULTS);
        if (StrTemp.CompareNoCase(_T("Sucessful"))==0)
        {
            temp.nresult = 3;
        }
        else
        {
            temp.nresult = 0;
        }

        StrTemp=m_flash_multy_list.GetItemText(i,FLASH_CONFIG_RESULTS);
        if (StrTemp.CompareNoCase(_T("Sucessful"))==0)
        {
            temp.cofnigresult = 3;
        }
        else
        {
            temp.cofnigresult = 0;
        }
		StrTemp = m_flash_multy_list.GetItemText(i, FLASH_ONLINE);
		if (StrTemp.CompareNoCase(L"Online") == 0)
		{
			temp.online = true;
		} 
		else
		{
			temp.online = false;
		}
        flash_device.push_back(temp);
    }

   /*  if(Check_Online_Thread != NULL)
         TerminateThread(Check_Online_Thread, 0);
     Check_Online_Thread=NULL;
     Check_Online_Thread =CreateThread(NULL,NULL,multy_check_online,this,NULL, NULL);*/
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CFlash_Multy::PreTranslateMessage(MSG* pMsg)
{
    

    return CDialogEx::PreTranslateMessage(pMsg);
}


void CFlash_Multy::OnClose()
{
     

    CDialogEx::OnClose();
}


void CFlash_Multy::OnCancel()
{
    

    CDialogEx::OnCancel();
}
//
//const int FLASH_ITEM = 0;
//const int FLASH_SERIAL_NUMBER = 1;
//const int FLASH_ID = 2;
//const int FLASH_PRODUCT_NAME = 3;
//const int FLASH_COM_PORT = 4;
//const int FLASH_IPADDRESS = 5;
//const int FLASH_IPPORT = 6;
//const int FLASH_SUB_NOTE = 7;
//const int FLASH_FILE_POSITION = 8;
//const int FLASH_RESULTS = 9;

void CFlash_Multy::Initial_List()
{


    CString ProductHexBinName,StrTemp;
    CString StrBinHexPath,StrULRPath;
    CString strSql;
    _variant_t temp_variant;
    m_flash_multy_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
    m_flash_multy_list.SetExtendedStyle(m_flash_multy_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
    m_flash_multy_list.InsertColumn(FLASH_ITEM, _T("NUM"), 40, ListCtrlEx::CheckBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
    m_flash_multy_list.InsertColumn(FLASH_SERIAL_NUMBER, _T("SN"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_flash_multy_list.InsertColumn(FLASH_ID, _T("Panel ID"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_flash_multy_list.InsertColumn(FLASH_PRODUCT_NAME, _T("Name"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_flash_multy_list.InsertColumn(FLASH_COM_PORT, _T("Com"), 40, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_flash_multy_list.InsertColumn(FLASH_BAUDRATE,_T("Baudrate"),40,ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_flash_multy_list.InsertColumn(FLASH_IPADDRESS, _T("IP"), 120, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_flash_multy_list.InsertColumn(FLASH_IPPORT, _T("Port"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_flash_multy_list.InsertColumn(FLASH_SUB_NOTE, _T("Is Sub"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_flash_multy_list.InsertColumn(FLASH_ONLINE, _T("Is Online"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_flash_multy_list.InsertColumn(FLASH_CURRENT_FIRMWARE, _T("Current Firmware"), 0, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_flash_multy_list.InsertColumn(FLASH_FILE_REV, _T("Latest Firmware"), 0, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_flash_multy_list.InsertColumn(FLASH_FILE_POSITION, _T("Hex Bin File"), 120, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_flash_multy_list.InsertColumn(FLASH_CONFIG_FILE_POSITION, _T("Configuration file"), 120, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_flash_multy_list.InsertColumn(FLASH_RESULTS, _T("Firmware Results"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_flash_multy_list.InsertColumn(FLASH_CONFIG_RESULTS, _T("Configuration Results"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_flash_multy_list.InsertColumn(FLASH_PRODUCT_ID, _T("PID"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_flash_multy_list.InsertColumn(FLASH_FIRMWARE_VERSION, _T("FW Version"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    
    m_flash_multy_hwnd = this->m_hWnd;
    g_hwnd_now = m_flash_multy_hwnd;


    //CRect list_rect,win_rect;
    //m_flash_multy_list.GetWindowRect(list_rect);
    //ScreenToClient(&list_rect);
    //::GetWindowRect(m_input_dlg_hwnd,win_rect);
    //m_flash_multy_list.Set_My_WindowRect(win_rect);
    //m_flash_multy_list.Set_My_ListRect(list_rect);

	CppSQLite3DB SqliteDBBuilding;
	CppSQLite3Table table;
	CppSQLite3Query q;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);


    CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    m_flash_multy_list.DeleteAllItems();
	//when initial the dialog , Checkbox will be initialised from the configuration file T3000.ini

    for(int i=0; i<(int)pFrame->m_product.size(); i++)
    {
        int j=pFrame->m_product.at(i).product_id;
        CString nitem;
        CString nSerialNum;
        CString nID;
        CString nProductName;
        CString nComport;
		CString nBaudrate;
        CString nIPAddress;
        CString nIP_Port;
        CString n_is_sub;
        CString nFilePosition;
        CString nresults;
        CString nproduct_id;
        int Firmware_Result;
        int Config_Result;
        CString StrTemp;
        nitem.Format(_T("%d"),i+1);
        nSerialNum.Format(_T("%d"),pFrame->m_product.at(i).serial_number);
        nID.Format(_T("%d"),pFrame->m_product.at(i).product_id);
        nProductName = pFrame->m_product.at(i).NameShowOnTree;//GetProductName(pFrame->m_product.at(i).product_class_id);
        nproduct_id.Format(_T("%u"),(unsigned char)pFrame->m_product.at(i).product_class_id);
		if(pFrame->m_product.at(i).note_parent_serial_number == 0)
		{
			if(pFrame->m_product.at(i).BuildingInfo.strIp.FindOneOf(_T("."))!=-1)
			{
				nComport.Empty();
				nIPAddress = pFrame->m_product.at(i).BuildingInfo.strIp;
				nIP_Port.Format(_T("%d"),pFrame->m_product.at(i).ncomport);
			}
			else
			{
				nComport.Format(_T("%d"),pFrame->m_product.at(i).ncomport);
				nBaudrate.Format(_T("%d"),pFrame->m_product.at(i).baudrate);			
				nIP_Port.Empty();
				nIPAddress.Empty();
			}
			n_is_sub = _T("NO");

		}
        else
        {
            nComport.Empty();
            nIPAddress = pFrame->m_product.at(i).BuildingInfo.strIp;
            nIP_Port.Format(_T("%d"),pFrame->m_product.at(i).ncomport);
            // If not a network device, it means it's a TSTAT class serial port device
            //if(IsOurNetDevice(pFrame->m_product.at(i).product_class_id) == false)//������������豸��˵����TSTAT��Ĵ����豸;
           // {
                n_is_sub = _T("YES");
           // }
           // else
           // {
           //     n_is_sub = _T("NO");
           // }
        }

        m_flash_multy_list.InsertItem(i,nitem);
        int temp_checked = GetPrivateProfileInt(_T("MassFlash"), nSerialNum, 0, g_ext_mass_flash_path);
        if (temp_checked)
        {
            m_flash_multy_list.SetCellChecked(i, FLASH_ITEM, 1);
        }
        else
            m_flash_multy_list.SetCellChecked(i, FLASH_ITEM, 0);
        m_flash_multy_list.SetCellEnabled(i, FLASH_ITEM,0);
        m_flash_multy_list.SetItemText(i,FLASH_SERIAL_NUMBER,nSerialNum);
        m_flash_multy_list.SetItemText(i,FLASH_ID,nID);
        m_flash_multy_list.SetItemText(i,FLASH_PRODUCT_NAME,nProductName);
        m_flash_multy_list.SetItemText(i,FLASH_COM_PORT,nComport);
		m_flash_multy_list.SetItemText(i,FLASH_BAUDRATE,nBaudrate);
        m_flash_multy_list.SetItemText(i,FLASH_IPADDRESS,nIPAddress);
        m_flash_multy_list.SetItemText(i,FLASH_IPPORT,nIP_Port);
        m_flash_multy_list.SetItemText(i,FLASH_SUB_NOTE,n_is_sub);
        m_flash_multy_list.SetItemText(i,FLASH_PRODUCT_ID,nproduct_id);
		if (pFrame->m_product.at(i).status)
		{
			m_flash_multy_list.SetItemText(i, FLASH_ONLINE, _T("Online"));
            m_flash_multy_list.SetItemTextColor(i, FLASH_ONLINE, RGB(0, 0, 0));
			//m_flash_multy_list.SetCellChecked(i, 0, TRUE);
		} 
		else
		{
			m_flash_multy_list.SetItemText(i, FLASH_ONLINE, _T("Offline"));
            m_flash_multy_list.SetItemTextColor(i, FLASH_ONLINE, RGB(255, 0, 0));
			//m_flash_multy_list.SetCellChecked(i, 0, FALSE);
		}
        CString temp_firmware_path;
        GetPrivateProfileStringW(_T("FirmwareFile"), nSerialNum,_T(""), temp_firmware_path.GetBuffer(MAX_PATH), MAX_PATH, g_ext_mass_flash_path);
        temp_firmware_path.ReleaseBuffer();
        m_flash_multy_list.SetItemText(i, FLASH_FILE_POSITION, temp_firmware_path);
        int temp_results = GetPrivateProfileInt(_T("FlashResult"), nSerialNum, 0, g_ext_mass_flash_path);
        if (temp_results == 1)//ʧ��, Failed
        {
            m_flash_multy_list.SetItemText(i, FLASH_RESULTS, _T("Fail"));
        }
        else if (temp_results == 2) //�ɹ�, Success
        {
            m_flash_multy_list.SetItemText(i, FLASH_RESULTS, _T("Success"));
        }
        else if (temp_results == 3) //up to date
        {
            m_flash_multy_list.SetItemText(i, FLASH_RESULTS, _T("Up to date"));
        }
        else if(temp_checked == false)
        {
            m_flash_multy_list.SetItemText(i, FLASH_RESULTS, _T(""));
        }
        else
        {
            m_flash_multy_list.SetItemText(i, FLASH_RESULTS, _T(""));
        }
        strSql.Format(_T("Select * from BatchFlashResult where SN=%d"),pFrame->m_product.at(i).serial_number);
        q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
        if (!q.eof())
        {

            

			StrTemp = q.getValuebyName(L"FirmwarePath");
            m_flash_multy_list.SetItemText(i,FLASH_FILE_POSITION,StrTemp);

             
            StrTemp = q.getValuebyName(L"ConfigPath");
            m_flash_multy_list.SetItemText(i,FLASH_CONFIG_FILE_POSITION,StrTemp);

          
				Firmware_Result = q.getIntField("FirmwareResult");
         
            if (Firmware_Result ==3 )//&& Config_Result == 3
            {
                //PostMessage(WM_MULTY_FLASH_MESSAGE,CHANGE_THE_ITEM_COLOR_MORE_GREEN,i);
                m_flash_multy_list.SetItemTextColor(i,-1,CONFIG_COLOR_CONFIG_FLASH_GOOD);
                m_flash_multy_list.SetItemText(i,FLASH_RESULTS,_T("Sucessful"));
                m_flash_multy_list.SetItemText(i,FLASH_CONFIG_RESULTS,_T("Sucessful"));
            }
            else
            {
                if (Firmware_Result == 3)
                {
                    // PostMessage(WM_MULTY_FLASH_MESSAGE,CHANGE_THE_ITEM_COLOR_GREEN,i);
                    m_flash_multy_list.SetItemTextColor(i,-1,FLASH_COLOR_GREEN);
                    m_flash_multy_list.SetItemText(i,FLASH_RESULTS,_T("Sucessful"));
                }
				/*else if (Config_Result == 3)
				{
					m_flash_multy_list.SetItemTextColor(i,-1,FLASH_COLOR_GREEN);
					m_flash_multy_list.SetItemText(i,FLASH_CONFIG_RESULTS,_T("Sucessful"));
				}*/
                else
                {
                    // PostMessage(WM_MULTY_FLASH_MESSAGE,CHANGE_THE_ITEM_COLOR_DEFAULT,i);
                    m_flash_multy_list.SetItemTextColor(i,-1,FLASH_COLOR_DEFAULT);
                    m_flash_multy_list.SetItemText(i,FLASH_RESULTS,_T(""));
                    m_flash_multy_list.SetItemText(i,FLASH_CONFIG_RESULTS,_T(""));
                }
            }


            //m_flash_multy_list.SetItemText(i,FLASH_CONFIG_RESULTS,StrTemp);



        }
        CString device_fw_version;
        device_fw_version.Format(_T("%.1f"), pFrame->m_product.at(i).software_version);


        m_flash_multy_list.SetItemText(i, FLASH_FIRMWARE_VERSION, device_fw_version);

        for (int x=0; x<FLASH_MAX_COUNT; x++)
        {
            if((i%2)==0)
                m_flash_multy_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
            else
                m_flash_multy_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
        }

    }
    SqliteDBBuilding.closedb();
    Initial_Parameter();
    return;
}


void CFlash_Multy::Initial_Parameter()
{

    GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
    PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
    ApplicationFolder.ReleaseBuffer();
    MultyISPtool_path = ApplicationFolder + _T("\\ISP.exe");
    AutoFlashConfigPath = ApplicationFolder + _T("\\AutoFlashFile.ini");
}

BOOL CFlash_Multy::IsOurNetDevice(int DevType)
{
    if (DevType == PM_LightingController
            || DevType == PM_NC
            || DevType == PM_CO2_NET
            || DevType == PM_MINIPANEL
		    || DevType == PM_MINIPANEL_ARM
            || DevType == PM_ESP32_T3_SERIES
            || DevType == PM_CM5
			|| DevType == STM32_CO2_NET
		|| DevType == STM32_HUM_NET
		|| DevType == STM32_PRESSURE_NET
		|| DevType == STM32_CO2_NODE
		
		)
    {
        return TRUE;
    }

    return FALSE;
}


void CFlash_Multy::OnBnClickedButtonApplyWoAllSelect()
{
    

    int ncount = m_flash_multy_list.GetItemCount();
    for (int i=0; i<ncount; i++)
    {
        CString Product_Name = m_flash_multy_list.GetItemText(i,FLASH_PRODUCT_NAME);
        BOOL is_checked = m_flash_multy_list.GetCellChecked(i,FLASH_ITEM);
        if(is_checked&&Product_Firmware_Check(Product_Name,Edit_File_Path))
        {
            m_flash_multy_list.SetItemText(i,FLASH_FILE_POSITION,Edit_File_Path);


// Du Fan disabled - there are various errors below, cannot get firmware information and will cause crashes
#if 0    //�ŷ�����  ����� �������󣬻�ȡ�����̼���Ϣ�����ᵼ�±���;
            BOOL is_good = FALSE;
            Bin_Info file_infor;
            if (HexFileValidation(Edit_File_Path))
            {
                Get_HexFile_Information(Edit_File_Path,file_infor);
                is_good = TRUE;
            }
            if (BinFileValidation(Edit_File_Path))
            {
                Get_Binfile_Information(Edit_File_Path,file_infor);
                is_good = TRUE;
            }
            float rev = 0;
            CString  StrTemp = _T("");
            if (is_good)
            {

                rev=((float)(file_infor.software_high*256+file_infor.software_low))/10;
                StrTemp.Format(_T("%0.1f"),rev);
                CString hexname;
                for (int i=0; i<10; i++)
                {
                    hexname.AppendFormat(_T("%c"),file_infor.product_name[i]);
                }

                if (hexname.CompareNoCase(L"minipanel")==0||hexname.CompareNoCase(L"cm5")==0)
                {
                    rev = (float)file_infor.software_high + ((float)file_infor.software_low)/10;
                    StrTemp.Format(_T("%0.1f"),rev);
                    /* flash_device.at(i).file_rev = StrTemp; */
                }
            }
            else
            {

                CString  StrTemp = _T("");
            }
            m_flash_multy_list.SetItemText(i,FLASH_FILE_REV,StrTemp);
#endif
        }
    }

    ParameterSaveToDB();
}

BOOL CFlash_Multy::Product_Firmware_Check(CString ProductName,CString FirmwareFilePath)
{
    // Don't do repetitive things here, let ISP tool complete firmware verification
    return 1;   //��Ҫ���������ظ������� ��У��̼������齻��ISP tool ȥ���;




    Bin_Info file_infor;
    CString hexproductname=_T("");
    BOOL is_good;
	char* m_pFileBuf;
    if (HexFileValidation(FirmwareFilePath))
    {
		 CHexFileParser* pHexFile = new CHexFileParser;
		pHexFile->SetFileName(FirmwareFilePath);

		m_pFileBuf = new char[c_nHexFileBufLen];
		memset(m_pFileBuf, 0xFF, c_nHexFileBufLen);
		int nDataSize = pHexFile->GetHexFileBuffer(m_pFileBuf, c_nHexFileBufLen); 
		file_infor = pHexFile->global_fileInfor;

		if (pHexFile)
		{
			delete pHexFile;
			pHexFile = NULL;
		}
		 

    }
    if (BinFileValidation(FirmwareFilePath))
    {
//         Get_Binfile_Information(FirmwareFilePath,file_infor);
//         is_good = TRUE;
      /*  if(READ_SUCCESS != Get_Binfile_Information(FirmwareFilePath,file_infor))
        {
            is_good = FALSE ;
        }
        else
        {
            is_good = TRUE;
        }*/

 

		CBinFileParser* pBinFile=new CBinFileParser;
		pBinFile->SetBinFileName(FirmwareFilePath);
		m_pFileBuf=new char[c_nHexFileBufLen];
		memset(m_pFileBuf, 0xFF, c_nHexFileBufLen);
		int nDataSize=pBinFile->GetBinFileBuffer(m_pFileBuf,c_nHexFileBufLen);
		 file_infor = pBinFile->global_fileInfor;
		delete pBinFile;
		pBinFile = NULL;


    }
    BOOL Ret_Result = TRUE;
    if (TRUE)
    {

		MultiByteToWideChar( CP_ACP, 0, file_infor.product_name, (int)strlen(file_infor.product_name)+1,hexproductname.GetBuffer(MAX_PATH), MAX_PATH );
		hexproductname.ReleaseBuffer();
		hexproductname = hexproductname.Left(10);
		hexproductname.Trim();
        //for (int i=0; i<10; i++)
        //{
        //    hexproductname.AppendFormat(_T("%c"),file_infor.product_name[i]);
        //}



        if(hexproductname.CompareNoCase(ProductName)==0)
        {
            Ret_Result= TRUE;
        }//TStatRunar
        else if ((
                     (ProductName.CompareNoCase(_T("tstat5e"))==0)
                     ||(ProductName.CompareNoCase(_T("tstat5h"))==0)
                     ||(ProductName.CompareNoCase(_T("tstat5g"))==0)
                 )
                 &&(ProductName.CompareNoCase(_T("tstat5lcd"))==0)
                )
        {
            Ret_Result= TRUE;
        }
        else if (((ProductName.CompareNoCase(_T("tstat5a"))==0)
                  ||(ProductName.CompareNoCase(_T("tstat5b"))==0)
                  ||(ProductName.CompareNoCase(_T("tstat5c"))==0)
                  ||(ProductName.CompareNoCase(_T("tstat5d"))==0)
                  ||(ProductName.CompareNoCase(_T("tstat5f"))==0)
                 )
                 &&(hexproductname.CompareNoCase(_T("tstat5led"))==0)
                )
        {
            Ret_Result= TRUE;
        }
        else if ((hexproductname.CompareNoCase(_T("tstat6"))==0)&&(ProductName.CompareNoCase(_T("tstat5i"))==0))
        {
            Ret_Result= TRUE;
        }
		else if ((hexproductname.CompareNoCase(_T("T322I"))==0)&&(ProductName.CompareNoCase(_T("T3-22I"))==0))
		{
			Ret_Result= TRUE;
		}
		else if ((hexproductname.CompareNoCase(_T("MiniPanel"))==0)&&(ProductName.CompareNoCase(_T("T3-8AI8AO6DO"))==0))
		{
			Ret_Result= TRUE;
		}	
		else if ((hexproductname.CompareNoCase(_T("T38IO"))==0)&&(ProductName.CompareNoCase(_T("T3Controller"))==0))
		{
			Ret_Result= TRUE;
		}	
		
        else
        {
            Ret_Result= FALSE;
        }


    }
    else
    {
        Ret_Result = FALSE;
    }
    return Ret_Result;

}
void CFlash_Multy::OnBnClickedButtonMultySel()
{
    

    CString strFilter = _T("hex File;bin File|*.hex;*.bin|all File|*.*||");
    CFileDialog dlg(true,_T("hex"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
    if(IDOK==dlg.DoModal())
    {
        Edit_File_Path=dlg.GetPathName();
        ((CEdit *)GetDlgItem(IDC_EDIT_MULTI_FILE_POSITION))->SetWindowTextW(Edit_File_Path);
    }

}

void CFlash_Multy::SetAutoConfig(Str_flash_device ndevice_info)
{
    CFileFind tempfind;
    CString temp_isp_info;
    if(tempfind.FindFile(AutoFlashConfigPath))
    {
        DeleteFile(AutoFlashConfigPath);
    }
    WritePrivateProfileStringW(_T("Data"),_T("Command"),_T("1"),AutoFlashConfigPath);
    if(ndevice_info.nIPaddress.IsEmpty())// Serial port
    {
        WritePrivateProfileStringW(_T("Data"),_T("COM_OR_NET"),_T("COM"),AutoFlashConfigPath);
        CString cs_comport;
        cs_comport = _T("COM") +  ndevice_info.ncomport;


        WritePrivateProfileStringW(_T("Data"),_T("COMPORT"),cs_comport,AutoFlashConfigPath);
        WritePrivateProfileStringW(_T("Data"),_T("Baudrate"),ndevice_info.nBaudrate,AutoFlashConfigPath);

        CString nflash_id;
        nflash_id = ndevice_info.nID;

        WritePrivateProfileStringW(_T("Data"),_T("ID"),nflash_id,AutoFlashConfigPath);

        temp_isp_info.Format(_T("Communications port : "));
        temp_isp_info = temp_isp_info + cs_comport;
        m_multy_flash_listbox.InsertString(m_multy_flash_listbox.GetCount(),temp_isp_info);
        temp_isp_info.Format(_T("ISP baudrate : %s"),ndevice_info.nipport);
        m_multy_flash_listbox.InsertString(m_multy_flash_listbox.GetCount(),temp_isp_info);
        temp_isp_info.Format(_T("Device ID :"));
        temp_isp_info = temp_isp_info + nflash_id;
        m_multy_flash_listbox.InsertString(m_multy_flash_listbox.GetCount(),temp_isp_info);
    }
    else
    {
        WritePrivateProfileStringW(_T("Data"),_T("COM_OR_NET"),_T("NET"),AutoFlashConfigPath);
        WritePrivateProfileStringW(_T("Data"),_T("IPAddress"),ndevice_info.nIPaddress,AutoFlashConfigPath);

        temp_isp_info.Format(_T("Communications port : network"));
        m_multy_flash_listbox.InsertString(m_multy_flash_listbox.GetCount(),temp_isp_info);
        temp_isp_info.Format(_T("IP Address : "));
        temp_isp_info = temp_isp_info + ndevice_info.nIPaddress;
        m_multy_flash_listbox.InsertString(m_multy_flash_listbox.GetCount(),temp_isp_info);


        CString n_tcpport;
        n_tcpport.Format(_T("%s"),ndevice_info.nipport);

        temp_isp_info.Format(_T("Port : "));
        temp_isp_info = temp_isp_info + n_tcpport;
        m_multy_flash_listbox.InsertString(m_multy_flash_listbox.GetCount(),temp_isp_info);

        WritePrivateProfileStringW(_T("Data"),_T("IPPort"),n_tcpport,AutoFlashConfigPath);
        if(ndevice_info.b_is_sub == true)
        {
            WritePrivateProfileStringW(_T("Data"),_T("Subnote"),_T("1"),AutoFlashConfigPath);
            CString nsub_id;
            nsub_id.Format(_T("%s"),ndevice_info.nID);
            WritePrivateProfileStringW(_T("Data"),_T("SubID"),nsub_id,AutoFlashConfigPath);

            temp_isp_info.Format(_T("Device is subnote."));
            m_multy_flash_listbox.InsertString(m_multy_flash_listbox.GetCount(),temp_isp_info);
        }
        else
        {
            WritePrivateProfileStringW(_T("Data"),_T("Subnote"),_T("0"),AutoFlashConfigPath);
            temp_isp_info.Format(_T("Device is not a subnote."));
            m_multy_flash_listbox.InsertString(m_multy_flash_listbox.GetCount(),temp_isp_info);
        }
    }

    CString mypath = ndevice_info.file_position;

    WritePrivateProfileStringW(_T("Data"),_T("FirmwarePath"),mypath,AutoFlashConfigPath);





}



void CFlash_Multy::OnBnClickedButtonStatrt()
{
    
    UpdateData();

    SetTimer(1,200,NULL);
    int nflashitemcount = 0;
    CString StrTemp;
    flash_device.clear();
    int ncount = m_flash_multy_list.GetItemCount();

    m_bTstatLoadFinished=FALSE;
    m_Start =TRUE;
    for (int i=0; i<ncount; i++)
    {
        if (!m_flash_multy_list.GetCellChecked(i,0))
        {
            continue;
        }
        nflashitemcount ++;
        Str_flash_device temp;
        temp.nitem = i;


        temp.strSN= m_flash_multy_list.GetItemText(i,FLASH_SERIAL_NUMBER);
        temp.nID = m_flash_multy_list.GetItemText(i,FLASH_ID);
        temp.ncomport = m_flash_multy_list.GetItemText(i,FLASH_COM_PORT);
		temp.nBaudrate = m_flash_multy_list.GetItemText(i,FLASH_BAUDRATE);
        temp.nIPaddress = m_flash_multy_list.GetItemText(i,FLASH_IPADDRESS);
        temp.nipport = m_flash_multy_list.GetItemText(i,FLASH_IPPORT);
        temp.devicename=m_flash_multy_list.GetItemText(i,FLASH_PRODUCT_NAME);
        temp.product_id = _wtoi(m_flash_multy_list.GetItemText(i, FLASH_PRODUCT_ID));
        temp.software_rev = _wtof(m_flash_multy_list.GetItemText(i, FLASH_FIRMWARE_VERSION));
        CString temp_sub = m_flash_multy_list.GetItemText(i,FLASH_SUB_NOTE);
        if(temp_sub.CompareNoCase(_T("YES")) == 0)
        {
            temp.b_is_sub = true;
        }
        else
        {
            temp.b_is_sub = false;
        }

        temp.file_position = m_flash_multy_list.GetItemText(i,FLASH_FILE_POSITION);
        temp.config_file_position=m_flash_multy_list.GetItemText(i,FLASH_CONFIG_FILE_POSITION);

        //FLASH_RESULTS
        //FLASH_CONFIG_RESULTS
        StrTemp=m_flash_multy_list.GetItemText(i,FLASH_RESULTS);
        if (StrTemp.CompareNoCase(_T("Sucessful"))==0)
        {
            temp.nresult = OPERATION_SUCCESS;
        }
        else
        {
            temp.nresult = 0;
        }

        StrTemp=m_flash_multy_list.GetItemText(i,FLASH_CONFIG_RESULTS);
        if (StrTemp.CompareNoCase(_T("Sucessful"))==0)
        {
            temp.cofnigresult = OPERATION_SUCCESS;
        }
        else
        {
            temp.cofnigresult = 0;
        }
        //StrTemp = m_flash_multy_list.GetItemText(i,FLASH_CURRENT_FIRMWARE);
        //temp.software_rev = _wtof(StrTemp);
        temp.file_rev  =   m_flash_multy_list.GetItemText(i,FLASH_FILE_REV);

        if (m_bool_flash_different_version)
        {
            //if (_wtof(temp.file_rev)-temp.software_rev>0.1)
            //{
                temp.need_flash = true;
            //}
            //else
            //{
            //    temp.need_flash = false;
            //}
        }
        else
        {
            temp.need_flash = true;
        }
		temp.need_flash = true;
        StrTemp = m_flash_multy_list.GetItemText(i,FLASH_ONLINE);
        if (StrTemp.CompareNoCase(_T("Online"))!=0)
        {
            temp.need_flash = false;
            temp.online = false;
        }
        else
        {
            temp.online = true;
        }
        m_flash_multy_list.SetItemText(i, FLASH_RESULTS,_T("Waiting"));
        flash_device.push_back(temp);
    }

    if(nflashitemcount == 0)
    {
        MessageBox(_T("Please select one or more items."),_T("Notice"),MB_OK | MB_ICONINFORMATION);
        m_bTstatLoadFinished=TRUE;
        return;
    }
    else
    {

        if(Call_ISP_Application != NULL)
            TerminateThread(Call_ISP_Application, 0);
        Call_ISP_Application=NULL;
        Call_ISP_Application =CreateThread(NULL,NULL,multy_isp_thread,this,NULL, NULL);
    }


}

void CFlash_Multy::ParameterSaveToDB()
{
	CppSQLite3DB SqliteDBBuilding;
	CppSQLite3Table table;
	CppSQLite3Query q;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

    CString StrSql;
    CString StrHexPath;
    CString StrConfigPath;
    for (int i =0 ; i <m_flash_multy_list.GetItemCount(); i++)
    {
        int SN = _wtoi(m_flash_multy_list.GetItemText(i,FLASH_SERIAL_NUMBER));
        StrHexPath = m_flash_multy_list.GetItemText(i,FLASH_FILE_POSITION);
        StrConfigPath =  m_flash_multy_list.GetItemText(i,FLASH_CONFIG_FILE_POSITION);
        StrSql.Format(_T("Select * From BatchFlashResult Where SN = %d"),SN);
		q = SqliteDBBuilding.execQuery((UTF8MBSTR)StrSql);

        if (!q.eof())
        {
            StrSql.Format(_T("Update BatchFlashResult Set FirmwarePath = '%s' , ConfigPath ='%s' ,FirmwareResult = 0,ConfigResult=0 Where SN = %d "),StrHexPath,StrConfigPath,SN);

        }
        else
        {
            StrSql.Format(_T("Insert Into BatchFlashResult (SN,FirmwarePath,ConfigPath,FirmwareResult,ConfigResult)  Values (%d,'%s','%s',0,0)"),SN,StrHexPath,StrConfigPath);
        }
        SqliteDBBuilding.execDML((UTF8MBSTR)StrSql);
    }
    SqliteDBBuilding.closedb();
}

/*
// Flash start, indicates ongoing 1>Ongoing project: blue color
const int CHANGE_THE_ITEM_COLOR_BLUE = 1;//��д��ʼ����ʾ���ڽ��е�1>���ڽ��е���Ŀ��������ɫ
// Flash failed 2>Flash not successful, red color
const int CHANGE_THE_ITEM_COLOR_RED = 2;//��дʧ��2>��дû�гɹ�,���Ǻ�ɫ
// Flash successful 3>Flash successful, green color
const int CHANGE_THE_ITEM_COLOR_GREEN = 3;//��д�ɹ�3>��д�ɹ�������ɫ
// Default
const int  CHANGE_THE_ITEM_COLOR_DEFAULT = 4;//Ĭ��
// 4>Flash successful && config file successful
const int CHANGE_THE_ITEM_COLOR_MORE_GREEN = 5; //4>��д�ɹ�&&�����ļ����ɹ�,
// 5>Config file failed
const int CHANGE_THE_ITEM_COLOR_LESS_RED = 6;//5>�����ļ���ʧ��

#define FLASH_COLOR_BLUE RGB(50,50,180)
#define FLASH_COLOR_RED  RGB(255,0,0)
#define FLASH_COLOR_GREEN  RGB(0,255,0)
#define FLASH_COLOR_DEFAULT  RGB(0,0,0)

#define CONFIG_COLOR_RED_FAIL  RGB(255,86,86)
#define CONFIG_COLOR_CONFIG_FLASH_GOOD  RGB(86,255,86)
*/
//void CFlash_Multy::GetDevice_Firmware_Version()
//{
//    
//}

// Used to read how many counts need to be changed
int multy_log_count = 1;	//���ڶ�ȡ�Ķ�����count��;
DWORD WINAPI  CFlash_Multy::multy_isp_thread(LPVOID lpVoid)
{
    //Write_Config_Info
    CFlash_Multy *pParent = (CFlash_Multy *)lpVoid;
    int nflashdevicecount = (int)flash_device.size();
    int comport=0;
	int baudrate = 0;
    BOOL IS_COM=TRUE;
    CString currentIp;
    int Port;
    BOOL is_connect_device=FALSE;

    CString log_file_path;
    CString config_file_path;
    CFile config_file;//the hex file
    CStdioFile log_file;
    bool log_file_opened=false;
    for(int i=0; i<nflashdevicecount; i++)
    {
//         if (!flash_device.at(i).need_flash&&flash_device.at(i).config_file_position.IsEmpty())
//         {
//             continue;
//         }
		//flash_device.at(i).need_flash
        if (true)
        {
            if (flash_device.at(i).nresult != OPERATION_SUCCESS)
            {
                if (!flash_device.at(i).file_position.IsEmpty())
                {
                    while(!pParent->m_Start)
                    {
                        Sleep(100);
                        continue;
                    }//��������Ƿ�Ϊ��ʼ״̬, Check if its currently in start state
                    CString temp_serial;
                    temp_serial = flash_device.at(i).strSN;
                    CString temp_time;
                    unsigned int temp_int_time = time(NULL);
                    temp_time.Format(_T("%u"), temp_int_time);
                    unsigned int last_oper_time = 0;
                    last_oper_time = GetPrivateProfileInt(temp_serial, _T("Time"), 0, g_ext_mass_flash_path);
                    //if (temp_int_time - last_oper_time < 3600 * 2)
                    //{
                    //    pParent->PostMessage(WM_MULTY_FLASH_MESSAGE, MASS_FLASH_MESSAGE, flash_device.at(i).nitem);
                    //    WritePrivateProfileStringW(_T("FlashResult"), temp_serial, _T("3"), g_ext_mass_flash_path); //Up to date
                    //    continue;
                    //}

                    CString temp_ProductPath = ApplicationFolder + _T("\\Database\\Firmware\\ProductPath.ini");
                    CString temp_pid;
                    temp_pid.Format(_T("%d"), flash_device.at(i).product_id);
                    CString temp_newest_rev;
                    GetPrivateProfileString(_T("Version"), temp_pid, _T("0"), temp_newest_rev.GetBuffer(MAX_PATH),MAX_PATH, temp_ProductPath);
                    temp_newest_rev.ReleaseBuffer();
                    flash_device.at(i).newest_rev = _wtof(temp_newest_rev);
                    if ((fabs(flash_device.at(i).newest_rev - flash_device.at(i).software_rev*10) <= 0.00001) && (flash_device.at(i).newest_rev > 0) && flash_device.at(i).software_rev > 0)
                    {
                        pParent->PostMessage(WM_MULTY_FLASH_MESSAGE, MASS_FLASH_MESSAGE, flash_device.at(i).nitem);
                        WritePrivateProfileStringW(_T("FlashResult"), temp_serial, _T("3"), g_ext_mass_flash_path); //Up to date
                        continue;
                    }

                    WritePrivateProfileStringW(temp_serial, _T("Time"), temp_time, g_ext_mass_flash_path);  //��¼���豸����ʱ��, Record time of this device
                    log_file_opened=false;
                    pParent->SetAutoConfig(flash_device.at(i));
                    pParent->PostMessage(WM_MULTY_FLASH_MESSAGE,CHANGE_THE_ITEM_COLOR_BLUE,flash_device.at(i).nitem);
                    multy_log_count = 1;
                    WinExecAndWait(MultyISPtool_path,NULL,NULL,0);
                    int nresult = GetPrivateProfileInt(_T("Data"),_T("Command"),FAILED_UNKNOW_ERROR,AutoFlashConfigPath);
                    //flash_device.at(i).nresult = nresult;
                    if(nresult == FLASH_SUCCESS)
                    {
                        flash_device.at(i).nresult=CHANGE_THE_ITEM_COLOR_GREEN;
                        pParent->PostMessage(WM_MULTY_FLASH_MESSAGE,CHANGE_THE_ITEM_COLOR_GREEN,flash_device.at(i).nitem);

                        WritePrivateProfileStringW(_T("FlashResult"), temp_serial, _T("2"), g_ext_mass_flash_path); //�ɹ�, Success
                    }
                    else
                    {
                        flash_device.at(i).nresult=CHANGE_THE_ITEM_COLOR_RED;
                        pParent->PostMessage(WM_MULTY_FLASH_MESSAGE,CHANGE_THE_ITEM_COLOR_RED,flash_device.at(i).nitem);
                        CString temp_serial;
                        temp_serial = flash_device.at(i).strSN;
                        WritePrivateProfileStringW(_T("FlashResult"), temp_serial, _T("1"), g_ext_mass_flash_path); //ʧ��, Failed
                    }
                    if (nresult!=FLASH_SUCCESS)
                    {
                        continue;
                    }
                    Sleep(4000);
                }
            }
        }


        // Check if it's currently in start state
        //��������Ƿ�Ϊ��ʼ״̬

        if (flash_device.at(i).cofnigresult==3||flash_device.at(i).config_file_position.IsEmpty())
        {
            continue;
        }
//     if (!flash_device.at(i).need_flash)
//     {
//         continue;
//     }
#if 1


        if(is_connect())
        {
            SetCommunicationType(0);
            close_com();
            SetCommunicationType(1);
            close_com();
        }
        g_Vector_Write_Error.clear();
        config_file_path=flash_device.at(i).config_file_position;
        if (config_file_path.IsEmpty())
        {
            continue;
        }
        if (!flash_device.at(i).ncomport.IsEmpty())//˵���ǷǴ����豸
        {
            comport=_wtoi(flash_device.at(i).ncomport);
			baudrate = _wtoi(flash_device.at(i).nBaudrate);
            IS_COM=TRUE;
        }
        else
        {
            IS_COM=FALSE;
            currentIp=flash_device.at(i).nIPaddress;
            Port=_wtoi(flash_device.at(i).nipport);
        }


        if (IS_COM)
        {
            if (open_com(comport))
            {
                is_connect_device=TRUE;
				Change_BaudRate(baudrate);
            }
            else
            {
                is_connect_device=FALSE;
            }
            SetCommunicationType(0);
        }
        else
        {

            if (Open_Socket2(currentIp,Port))
            {
                is_connect_device=TRUE;
            }
            else
            {
                is_connect_device=FALSE;
            }
            SetCommunicationType(1);
        }

        if (!is_connect_device)
        {
            continue;
        }

        load_file_every_step temppp;
        temppp.eight_step=temppp.fifth_step=temppp.first_step=temppp.keep_down=temppp.second_step=temppp.seven_step=temppp.sixth_step=temppp.third_step=temppp.thurth_step=FALSE;
        bool open_file=false;
        int nowmoder=0;
        CString strSerial;
        strSerial.Format(_T("%s"),flash_device.at(i).strSN);
        strSerial+=_T(".txt");
        log_file_path=pParent->m_strLogFilePath+_T("\\")+strSerial;

        if(log_file.Open(log_file_path,CFile::modeCreate | CFile::modeReadWrite)!=0)
        {
            log_file_opened=true;
            CString config_path=flash_device.at(i).config_file_position;
            if(!config_path.IsEmpty())
            {
                log_file.WriteString(config_path);
            }
            CString ttt=_T("\r\n\r\n");
            log_file.Write(ttt.GetString(),ttt.GetLength());
        }
        showing_text.Format(_T("------------------------------%s Begin--------------------------------"),flash_device.at(i).strSN);
        now_tstat_id=_wtoi(flash_device.at(i).nID);
        int nFlag=0;
        //nFlag = read_one(now_tstat_id,7,6);
        Read_Multi(now_tstat_id,product_register_value,0,10);
        nFlag = product_register_value[7];
        if((nFlag == PM_TSTAT6) || (nFlag == PM_TSTAT7)|| (nFlag == PM_TSTAT8) || (nFlag == PM_TSTAT9) || (nFlag == PM_TSTAT5i)
			|| (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V))
        {
            product_type =T3000_6_ADDRESS;
        }
        else if((nFlag == PM_TSTAT5E||nFlag == PM_PM5E || nFlag == PM_PM5E_ARM)||(product_register_value[7]==PM_TSTATRUNAR) || (nFlag == PM_TSTAT5H)||(nFlag == PM_TSTAT5G))
        {
            product_type = T3000_5EH_LCD_ADDRESS;
        }
        else if((nFlag == PM_TSTAT5A) ||(nFlag == PM_TSTAT5B) ||
                (nFlag ==PM_TSTAT5C ) || (nFlag == PM_TSTAT5D) || (nFlag == PM_TSTAT5F) )
        {
            product_type =T3000_5ABCDFG_LED_ADDRESS;
        }
        else if (nFlag==PM_T3PT10||
                 nFlag==PM_T3IOA||
                 nFlag==PM_T332AI||
                 nFlag==PM_T38AI16O||
                 nFlag==PM_T38I13O||
                 nFlag==PM_T3PERFORMANCE||
                 nFlag==PM_T34AO||
                 nFlag==PM_T36CT||
                 nFlag==PM_T322AI||
                 nFlag == PM_T332AI_ARM ||
                 nFlag==PM_T38AI8AO6DO
                )
        {
            product_type = T3000_T3_MODULES;
        }
        else
        {
            product_type = nFlag;
        }



        if(config_file.Open(config_file_path,CFile::modeRead | CFile::shareDenyNone))
        {

            CString strPath=config_file.GetFilePath();
            CString strFileName=config_file.GetFileTitle();
            open_file=true;
            config_file.Close();
            //	pDlg->m_infoListBox.InsertString(0,_T(""));
            //nowmoder=read_one(now_tstat_id,7,7);
            if(log_file_opened==true)
            {
                if (product_type==T3000_6_ADDRESS)
                {
                    LoadFile2Tstat67(temppp,(LPTSTR)(LPCTSTR)config_file_path,&log_file);
                }
                else if (product_type==T3000_T3_MODULES)
                {
                    //LoadT3Modules(temppp,(LPTSTR)(LPCTSTR)pDlg->m_grid_load.at(iitemp).hex_file_path.GetString(),&log_file);
                    LoadFile2Tstat_T3(temppp,(LPTSTR)(LPCTSTR)config_file_path,&log_file);
                }
                else if (product_type==PM_LightingController)
                {
                    //config_file.Close();

                    load_file_2_schedule_LC((LPTSTR)(LPCTSTR)config_file_path, now_tstat_id, log_file);
                }
                else if (product_type==PM_NC)
                {

                    load_file_2_schedule_NC((LPTSTR)(LPCTSTR)config_file_path, now_tstat_id, log_file);
                }
                else if (product_type == PM_CO2_NODE)
                {
                    LoadFile2Tstat_T3(temppp,(LPTSTR)(LPCTSTR)config_file_path,&log_file);
                }
                else
                {
                    LoadFile2Tstat(temppp,(LPTSTR)(LPCTSTR)config_file_path,&log_file);
                }
            }

            if (g_Vector_Write_Error.size()>0)
            {
                flash_device.at(i).nresult=CHANGE_THE_ITEM_COLOR_LESS_RED;
            }
            else
            {
                flash_device.at(i).nresult=CHANGE_THE_ITEM_COLOR_MORE_GREEN;
            }

            /*     if (config_file.m_hFile!=NULL)
                 {config_file.Close();
                 }*/

            if(log_file.m_hFile!=NULL)
                log_file.Close();
            showing_text.Format(_T("------------------------------%s END--------------------------------"),flash_device.at(i).strSN);
        }



        if (IS_COM)
        {

            SetCommunicationType(0);
            close_com();
        }
        else
        {
            SetCommunicationType(1);
            close_com();
        }
#endif
        //	flash_device.at(i).nresult=CHANGE_THE_ITEM_COLOR_RED;
        pParent->PostMessage(WM_MULTY_FLASH_MESSAGE,flash_device.at(i).nresult,flash_device.at(i).nitem);
    }
    pParent->m_bTstatLoadFinished=TRUE;


    return 0;
}
DWORD WINAPI  CFlash_Multy::multy_check_online(LPVOID lpVoid)
{
    CFlash_Multy *pParent = (CFlash_Multy *)lpVoid;

    int nflashitemcount = 0;
    CString StrTemp;
    pParent->GetDlgItem(IDC_BUTTON_STATRT)->EnableWindow(FALSE);
    pParent->GetDlgItem(IDC_BUTTON_UPDATE_FIRMWARE)->EnableWindow(FALSE);

    int nflashdevicecount = (int)flash_device.size();
    int comport=0;
	int baudrate = 19200;
    BOOL IS_COM=TRUE;
    CString currentIp;
    int Port;
    BOOL is_connect_device=FALSE;
    float softrev;
    CString log_file_path;
    CString config_file_path;
    CFile config_file;//the hex file
    CStdioFile log_file;
    bool log_file_opened=false;
    for(int i=0; i<nflashdevicecount; i++)
    {
        if(is_connect())
        {
            SetCommunicationType(0);
            close_com();
            SetCommunicationType(1);
            close_com();
        }
        Sleep(100);
        if (!flash_device.at(i).ncomport.IsEmpty())//˵���ǷǴ����豸
        {
            comport=_wtoi(flash_device.at(i).ncomport);
			baudrate = _wtoi(flash_device.at(i).nBaudrate);
            IS_COM=TRUE;
        }
        else
        {
            IS_COM=FALSE;
            currentIp=flash_device.at(i).nIPaddress;
            Port=_wtoi(flash_device.at(i).nipport);
        }

        CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
        bool is_online = false;
        for (int j=0; j<pFrame->m_product.size(); j++)
        {

            unsigned int temp_serial = _wtoi(flash_device.at(i).strSN);
            if(temp_serial == pFrame->m_product.at(j).serial_number)
            {
                is_online = pFrame->m_product.at(j).status;
                break;
            }
        }

        if((IS_COM == FALSE) && (is_online == false))
        {
            is_connect_device=FALSE;
        }
        else  if (IS_COM)  //��������
        {
            if (open_com(comport))
            {
                is_connect_device=TRUE;
				Change_BaudRate(baudrate);
            }
            else
            {
                is_connect_device=FALSE;
            }
            SetCommunicationType(0);
        }
        else
        {

            if (Open_Socket2(currentIp,Port))
            {
                is_connect_device=TRUE;
            }
            else
            {
                is_connect_device=FALSE;
            }
            SetCommunicationType(1);
        }
        Sleep(200);
        if(is_connect_device)
        {
            unsigned short DataBuffer[10];
            int ret = Read_Multi(_wtoi(flash_device.at(i).nID),DataBuffer,0,10,5);
            if (ret > 0)
            {
                softrev =0.0;
                if(DataBuffer[7] == PM_CM5 || DataBuffer[7] == PM_MINIPANEL || DataBuffer[7] == PM_MINIPANEL_ARM  || DataBuffer[7] == PM_ESP32_T3_SERIES)
                {
                    softrev = (float)DataBuffer[5]+((float)DataBuffer[4])/10;

                }
				else
				{
					softrev = ((float)(DataBuffer[5]*256+ DataBuffer[4]))/10;
				}
                flash_device.at(i).software_rev  = softrev;
                flash_device.at(i).online = true;
            }
            else
            {
                flash_device.at(i).software_rev  = 0;
                flash_device.at(i).online = false;
            }

        }
        else
        {
            flash_device.at(i).software_rev  = 0;
            flash_device.at(i).online = false;
        }

        Bin_Info file_infor;
        BOOL is_good = FALSE;
        if (HexFileValidation(flash_device.at(i).file_position))
        {
            Get_HexFile_Information(flash_device.at(i).file_position,file_infor);
            is_good = TRUE;
        }
        if (BinFileValidation(flash_device.at(i).file_position))
        {
            Get_Binfile_Information(flash_device.at(i).file_position,file_infor);
            is_good = TRUE;
        }
        if (is_good)
        {
            float rev;
            rev=((float)(file_infor.software_high*256+file_infor.software_low))/10;
            StrTemp.Format(_T("%0.1f"),rev);
            flash_device.at(i).file_rev = StrTemp ;
            CString hexname;
            for (int j=0; j<10; j++)
            {
                hexname.AppendFormat(_T("%c"),file_infor.product_name[j]);
            }

            if (hexname.CompareNoCase(L"minipanel")==0||hexname.CompareNoCase(L"cm5")==0)
            {
                rev = (float)file_infor.software_high + ((float)file_infor.software_low)/10;
                StrTemp.Format(_T("%0.1f"),rev);
                flash_device.at(i).file_rev = StrTemp;
            }
        }
        else
        {
            flash_device.at(i).file_rev = _T("");
        }


        pParent->PostMessage(WM_MULTY_FLASH_MESSAGE,10,i);
    }
    if(is_connect())
    {
        SetCommunicationType(0);
        close_com();
        SetCommunicationType(1);
        close_com();
    }
    pParent->GetDlgItem(IDC_BUTTON_STATRT)->EnableWindow(TRUE);
    pParent->GetDlgItem(IDC_BUTTON_UPDATE_FIRMWARE)->EnableWindow(TRUE);
    return 0;
}

void CFlash_Multy::OnTimer(UINT_PTR nIDEvent)
{
     
    int temp_count = 0;
    temp_count = GetPrivateProfileInt(_T("LogInfo"),_T("AddCount"),0,AutoFlashConfigPath);
    if(multy_log_count <= temp_count)
    {
        CString temp_string;
        CString section;
        section.Format(_T("AddText%d"),multy_log_count);
        GetPrivateProfileStringW(_T("LogInfo"),section,_T(""),temp_string.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
        temp_string.ReleaseBuffer();
        multy_log_count ++;
        if(!temp_string.IsEmpty())
        {
            m_multy_flash_listbox.InsertString(m_multy_flash_listbox.GetCount(),temp_string);
            m_multy_flash_listbox.SetTopIndex(m_multy_flash_listbox.GetCount()-1);
        }
    }

    int is_replace_refresh = GetPrivateProfileInt(_T("LogInfo"),_T("Replace_Refresh"),0,AutoFlashConfigPath);
    if(is_replace_refresh)
    {
        int temp_replace_count = GetPrivateProfileInt(_T("LogInfo"),_T("ReplaceCount"),0,AutoFlashConfigPath);
        CString temp_section;
        temp_section.Format(_T("ReplaceText%d"),temp_replace_count);
        CString replace_string;
        GetPrivateProfileStringW(_T("LogInfo"),temp_section,_T(""),replace_string.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
        replace_string.ReleaseBuffer();
        int nLineCount = m_multy_flash_listbox.GetCount();
        m_multy_flash_listbox.DeleteString(nLineCount - 1);

        m_multy_flash_listbox.InsertString(m_multy_flash_listbox.GetCount(),replace_string);
        m_multy_flash_listbox.SetTopIndex(m_multy_flash_listbox.GetCount()-1);
        WritePrivateProfileStringW(_T("LogInfo"),_T("Replace_Refresh"),_T("0"),AutoFlashConfigPath);

    }

    m_StrTips_new=showing_text;

// 	CString strTemp;
// 	critical_section.Lock();
// 	strTemp=showing_text;
// 	critical_section.Unlock();

// 	if(m_strInfoText.CompareNoCase(strTemp)!=0)
// 	{
// 		m_strInfoText=strTemp;
// 		int n=-1;
// 		n=m_strInfoText.Find(_T("\r\n"));
// 		if(n>=0)
// 			m_strInfoText.Left(n);
    if(nIDEvent == 1)
    {
        GetDlgItem(IDC_BUTTON_UPDATE_FIRMWARE)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_STATRT)->EnableWindow(FALSE);
        // GetDlgItem(IDC_BUTTON_STATRT)->ShowWindow(FALSE);
        /*if (m_Start)
        {
           GetDlgItem(IDC_BUTTON_STATRT)->SetWindowText(_T("Stop"));
        }
        else
        {
           GetDlgItem(IDC_BUTTON_STATRT)->SetWindowText(_T("Start"));
        }*/

        if (m_bTstatLoadFinished)
        {
            GetDlgItem(IDC_BUTTON_STATRT)->EnableWindow(TRUE);
            GetDlgItem(IDC_BUTTON_UPDATE_FIRMWARE)->EnableWindow(TRUE);
            //GetDlgItem(IDC_BUTTON_STATRT)->ShowWindow(TRUE);

            KillTimer(1);
            m_multy_flash_listbox.InsertString(m_multy_flash_listbox.GetCount(), _T("The selected device has been updated!"));
            m_multy_flash_listbox.SetTopIndex(m_multy_flash_listbox.GetCount() - 1);
        }


        if (showing_text.IsEmpty()||m_StrTips_new.CompareNoCase(m_StrTips_old)==0)
        {
            return;
        }
        m_multy_flash_listbox.InsertString(m_multy_flash_listbox.GetCount(),showing_text);
        m_multy_flash_listbox.SetTopIndex(m_multy_flash_listbox.GetCount()-1);
        //m_multy_flash_listbox.InsertString(m_list_row,showing_text);
        m_StrTips_old=m_StrTips_new;



    }
    /*	}*/

    //	if(m_strInfoText.Find(_T("Success"))!=-1 || m_strInfoText.Find(_T("Failure"))!=-1)
    /*if(m_bTstatLoadFinished && m_bNCLoadFinished)
    {
    	GetDlgItem(IDC_CONFIGBUTTON)->EnableWindow(TRUE);

    	KillTimer(1);
    	m_infoListBox.InsertString(0,m_strInfoText.GetString());
    	CString temp_str=_T("To view the file please click on the \"Log File\" option");
    	m_strInfoText.Empty();
    	m_strInfoText=temp_str+_T("\n");
    	m_infoListBox.InsertString(0,temp_str.GetString());
    	temp_str=_T("Loading status are stored into a Log file.");
    	m_strInfoText=m_strInfoText+_T("\n")+temp_str+_T("\n");
    	m_infoListBox.InsertString(-1,temp_str.GetString());

    	AfxMessageBox(m_strInfoText);
    }*/

    CDialogEx::OnTimer(nIDEvent);
}
/*
const int CHANGE_THE_ITEM_COLOR_BLUE = 1;//��д��ʼ����ʾ���ڽ��е�1>���ڽ��е���Ŀ��������ɫ
const int CHANGE_THE_ITEM_COLOR_RED = 2;//��дʧ��2>��дû�гɹ�,���Ǻ�ɫ
const int CHANGE_THE_ITEM_COLOR_GREEN = 3;//��д�ɹ�3>��д�ɹ�������ɫ
const int  CHANGE_THE_ITEM_COLOR_DEFAULT = 4;//Ĭ��
const int CHANGE_THE_ITEM_COLOR_MORE_GREEN = 5; //4>��д�ɹ�&&�����ļ����ɹ�,
const int CHANGE_THE_ITEM_COLOR_LESS_RED = 6;//5>�����ļ���ʧ��

#define FLASH_COLOR_BLUE RGB(50,50,180)
#define FLASH_COLOR_RED  RGB(255,0,0)
#define FLASH_COLOR_GREEN  RGB(0,255,0)
#define FLASH_COLOR_DEFAULT  RGB(0,0,0)

#define CONFIG_COLOR_RED_FAIL  RGB(255,86,86)
#define CONFIG_COLOR_CONFIG_FLASH_GOOD  RGB(86,255,86)
*/

LRESULT CFlash_Multy::MultyFlashMessage(WPARAM wParam,LPARAM lParam)
{
    int main_command = (int)wParam;
    int sub_parameter = (int)lParam;
    CString strTips;
    if (main_command  == CHANGE_ONE_ITEM)
    {
        if (flash_device.at(sub_parameter-1).online)
        {
            m_flash_multy_list.SetItemText(sub_parameter,FLASH_ONLINE,_T("Online"));
            m_flash_multy_list.SetItemTextColor(sub_parameter, FLASH_ONLINE, RGB(0, 0, 0));
        }
        else
        {
            m_flash_multy_list.SetItemText(sub_parameter,FLASH_ONLINE,_T("Offline"));
            m_flash_multy_list.SetItemTextColor(sub_parameter, FLASH_ONLINE, RGB(255, 0, 0));
        }
        strTips.Format(_T("%0.1f"),flash_device.at(sub_parameter-1).software_rev);
        m_flash_multy_list.SetItemText(sub_parameter,FLASH_CURRENT_FIRMWARE,strTips);

        m_flash_multy_list.SetItemText(sub_parameter,FLASH_FILE_REV,flash_device.at(sub_parameter-1).file_rev);

    }
	//CppSQLite3DB SqliteDBBuilding;
	//CppSQLite3Table table;
	//CppSQLite3Query q;
	//SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

    CString StrSql;
    switch(main_command)
    {
    case CHANGE_THE_ITEM_COLOR_BLUE:
    {
        m_flash_multy_list.SetItemTextColor(sub_parameter, FLASH_RESULTS,FLASH_COLOR_BLUE);
        m_flash_multy_list.SetItemText(sub_parameter,FLASH_RESULTS,_T("Running"));
    }
    break;
    case CHANGE_THE_ITEM_COLOR_GREEN:
    {
        m_flash_multy_list.SetItemTextColor(sub_parameter, FLASH_RESULTS,FLASH_COLOR_GREEN);
        m_flash_multy_list.SetItemText(sub_parameter,FLASH_RESULTS,_T("Sucessful"));
  //      StrSql.Format(_T("Update BatchFlashResult Set FirmwareResult = 3 Where SN = %d "),_wtoi(flash_device.at(sub_parameter-1).strSN));
  //      //bado.OpenRecordset(StrSql);
		//SqliteDBBuilding.execDML((UTF8MBSTR)StrSql);
    }
    break;
    case CHANGE_THE_ITEM_COLOR_RED:
    {
        m_flash_multy_list.SetItemTextColor(sub_parameter, FLASH_RESULTS,FLASH_COLOR_RED);
        m_flash_multy_list.SetItemText(sub_parameter,FLASH_RESULTS,_T("Fail"));
    }
    break;
    case CHANGE_THE_ITEM_COLOR_DEFAULT:
    {
        m_flash_multy_list.SetItemTextColor(sub_parameter, FLASH_RESULTS,FLASH_COLOR_DEFAULT);
        m_flash_multy_list.SetItemText(sub_parameter,FLASH_RESULTS,_T(""));
        m_flash_multy_list.SetItemText(sub_parameter,FLASH_CONFIG_RESULTS,_T(""));
    }
    break;
    case CHANGE_THE_ITEM_COLOR_MORE_GREEN:
    {
        m_flash_multy_list.SetItemTextColor(sub_parameter,-1,CONFIG_COLOR_CONFIG_FLASH_GOOD);
        //m_flash_multy_list.SetItemText(sub_parameter,FLASH_RESULTS,_T("Sucessful"));
        m_flash_multy_list.SetItemText(sub_parameter,FLASH_CONFIG_RESULTS,_T("Sucessful"));
        //if (sub_parameter > 0)
        //{
        //    StrSql.Format(_T("Update BatchFlashResult Set ConfigResult = 3 Where SN = %d "), _wtoi(flash_device.at(sub_parameter - 1).strSN));
        //    SqliteDBBuilding.execDML((UTF8MBSTR)StrSql);
        //}
    }
    break;
    case CHANGE_THE_ITEM_COLOR_LESS_RED:
    {
        m_flash_multy_list.SetItemTextColor(sub_parameter,-1,CONFIG_COLOR_RED_FAIL);
        m_flash_multy_list.SetItemText(sub_parameter,FLASH_RESULTS,_T("Sucessful"));
        m_flash_multy_list.SetItemText(sub_parameter,FLASH_CONFIG_RESULTS,_T("Fail"));
    }
    break;
    case MASS_FLASH_MESSAGE:
    {
        m_flash_multy_list.SetItemTextColor(sub_parameter, -1, CONFIG_COLOR_CONFIG_FLASH_GOOD);
        m_flash_multy_list.SetItemText(sub_parameter, FLASH_RESULTS, _T("Up to date"));
    }
        break;
    }
    //SqliteDBBuilding.closedb();
    return 0;
}



void CFlash_Multy::OnNMClickListFlashMulty(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    
    *pResult = 0;

    long lRow,lCol;
    m_flash_multy_list.Set_Edit(true);
    DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
    CPoint point( GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos));
    m_flash_multy_list.ScreenToClient(&point);
    LVHITTESTINFO lvinfo;
    lvinfo.pt=point;
    lvinfo.flags=LVHT_ABOVE;
    int nItem=m_flash_multy_list.SubItemHitTest(&lvinfo);

    lRow = lvinfo.iItem;
    lCol = lvinfo.iSubItem;
    CString Product_Name = m_flash_multy_list.GetItemText(lRow,FLASH_PRODUCT_NAME);
    int total_count = m_flash_multy_list.GetItemCount();
    if(lRow> total_count) //����������������кţ���������Ч��
        return;
    if(lRow<0)
        return;
    if (lCol == FLASH_ITEM)
    {
        
        for (int z = 0; z < total_count; z++) //��Ŀǰѡ�еĴ洢����;�´�ֱ����ʾ��Щ�Ѿ�ѡ�е�;
        {
            CString temp_serial = m_flash_multy_list.GetItemText(z, FLASH_SERIAL_NUMBER);
            bool temp_check = m_flash_multy_list.GetCellChecked(z, FLASH_ITEM);
            if (z == lRow)
            {
                if (temp_check)
                {
                    m_flash_multy_list.SetCellChecked(z, FLASH_ITEM, 0);
                    WritePrivateProfileStringW(_T("MassFlash"), temp_serial, NULL, g_ext_mass_flash_path);
                }
                else
                {
                    m_flash_multy_list.SetCellChecked(z, FLASH_ITEM, 1);
                    WritePrivateProfileStringW(_T("MassFlash"), temp_serial, _T("1"), g_ext_mass_flash_path);
                }
            }
            else
            {            
                if (temp_check)
                    WritePrivateProfileStringW(_T("MassFlash"), temp_serial, _T("1"), g_ext_mass_flash_path);
                else
                    WritePrivateProfileStringW(_T("MassFlash"), temp_serial, NULL, g_ext_mass_flash_path);
            }
        }
    }
    else if(lCol == FLASH_FILE_POSITION)
    {
        CString strFilter = _T("hex File;bin File|*.hex;*.bin|all File|*.*||");
        CFileDialog dlg(true,_T("hex"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
        if(IDOK==dlg.DoModal())
        {
            CString temp_path;
            temp_path=dlg.GetPathName();
            if (Product_Firmware_Check(Product_Name,temp_path))
            {
                m_flash_multy_list.SetItemText(lRow,FLASH_FILE_POSITION,temp_path);
            }
            else
            {
                temp_path.Format(_T("The firmware file is not fit for %s"),Product_Name);
                MessageBox(temp_path);
            }

        }
    }
    else if(lCol == FLASH_CONFIG_FILE_POSITION)
    {

        CString strFilter = _T("txt File|*.txt|all File|*.*||");
        CFileDialog dlg(true,_T("txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
        if(IDOK==dlg.DoModal())
        {
            CString temp_path;
            temp_path=dlg.GetPathName();
            if (Configuration_File_Check(Product_Name,temp_path))
            {
                m_flash_multy_list.SetItemText(lRow,FLASH_CONFIG_FILE_POSITION,temp_path);
            }
            else
            {
                temp_path.Format(_T("The configuration file is not fit for %s"),Product_Name);
                MessageBox(temp_path);
            }

        }
    }

}


void CFlash_Multy::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    if (nType==SIZE_RESTORED)
    {

//		CRect ViewRect;
//		GetClientRect(&ViewRect);
//		//TRACE(_T(" View:T=%d,B=%d,L=%d,R=%d\n"),ViewRect.top,ViewRect.bottom,ViewRect.left,ViewRect.right);
//		// m_MsDataGrid.SetWindowPos(this,ViewRect.top,ViewRect.left,ViewRect.Width(),ViewRect.Height(),SWP_SHOWWINDOW|SWP_NOZORDER);
//// 		if (m_flash_multy_list.GetSafeHwnd())
//// 		{
//// 			m_flash_multy_list.MoveWindow(CRect(0,70,ViewRect.Width(),ViewRect.Height()-200),TRUE);
//// 		}
//		//m_multy_flash_listbox
//  		if (m_multy_flash_listbox.GetSafeHwnd())
//  		{
//  			m_flash_multy_list.MoveWindow(CRect(0,ViewRect.Height()-220,ViewRect.Width(),ViewRect.Height()),TRUE);
//  		}
    }
}


void CFlash_Multy::OnBnClickedButtonMultySel2()
{
    CString strFilter = _T("txt File|*.txt|all File|*.*||");
    CFileDialog dlg(true,_T("hex"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
    dlg.DoModal();
    CString strTemp;
    strTemp=dlg.GetPathName();
    //save the path
    CFile default_file;
    if(strTemp.IsEmpty())
    {
        return;
    }
    m_configure_edit.SetWindowTextW(strTemp);
    m_configure_path=strTemp;

}


void CFlash_Multy::OnBnClickedAllcheck()
{
    if (m_configure_path.IsEmpty())
    {
        AfxMessageBox(L"Your Config file path is empty,please select your config file.");
        return;
    }
    int ncount = m_flash_multy_list.GetItemCount();
    for (int i=0; i<ncount; i++)
    {
        BOOL is_checked = m_flash_multy_list.GetCellChecked(i,FLASH_ITEM);
        if(is_checked)
        {
            CString ProductName = m_flash_multy_list.GetItemText(i,FLASH_PRODUCT_NAME);
            if (Configuration_File_Check(ProductName,m_configure_path))
            {
                m_flash_multy_list.SetItemText(i,FLASH_CONFIG_FILE_POSITION,m_configure_path);
            }

        }

    }
    ParameterSaveToDB();
}


void CFlash_Multy::OnBnClickedCheck1()
{
    if (!m_select_all)
    {
        for (int i = 0; i<m_flash_multy_list.GetItemCount(); i++)
        {
            m_flash_multy_list.SetCellChecked(i,0,TRUE);
            CString temp_serial;
            temp_serial = m_flash_multy_list.GetItemText(i, FLASH_SERIAL_NUMBER);
            WritePrivateProfileStringW(_T("MassFlash"), temp_serial, _T("1"), g_ext_mass_flash_path);
        }
        m_select_all = TRUE;
    }
    else
    {
        for (int i = 0; i<m_flash_multy_list.GetItemCount(); i++)
        {
            m_flash_multy_list.SetCellChecked(i,0,FALSE);
            CString temp_serial;
            temp_serial = m_flash_multy_list.GetItemText(i, FLASH_SERIAL_NUMBER);
            WritePrivateProfileStringW(_T("MassFlash"), temp_serial, NULL, g_ext_mass_flash_path);
        }
        m_select_all = FALSE;
    }


}
BOOL CFlash_Multy::Configuration_File_Check(CString ProductName,CString ConfigurationFilePath)
{
    wifstream inf;//file
    inf.open(ConfigurationFilePath,ios_base::in);



    TCHAR buf[1024];
    bool ret=false;

    inf.getline(buf,1024);
    if (find_sub_chars(buf,_T("Config File")))
    {
        inf.getline(buf,1024);//T3000 �汾��
        if (find_sub_chars(buf, ProductName))
        {
            ret=true;
        }
        inf.getline(buf,1024);
        if (find_sub_chars(buf,ProductName))
        {
            ret=true;
        }

    }

    inf.close();
    return ret;


}
void CFlash_Multy::OnBnClickedButtonApplyWoAllSelect2()
{
    OnBnClickedAllcheck();
}


void CFlash_Multy::OnBnClickedResetAll()
{
  //  GetDlgItem(IDC_EDIT_MULTI_FILE_POSITION)->SetWindowText(_T(""));
    //GetDlgItem(IDC_EDIT_MULTI_FILE_POSITION2)->SetWindowText(_T(""));
    int ncount = m_flash_multy_list.GetItemCount();
    for (int i=0; i<ncount; i++)
    {
        //m_flash_multy_list.SetItemText(i,FLASH_FILE_POSITION,_T(""));
       // m_flash_multy_list.SetItemText(i,FLASH_CONFIG_FILE_POSITION,_T(""));
        m_flash_multy_list.SetItemText(i,FLASH_RESULTS,_T(""));
        m_flash_multy_list.SetItemText(i,FLASH_CONFIG_RESULTS,_T(""));
        m_flash_multy_list.SetItemTextColor(i,-1,FLASH_COLOR_DEFAULT);
    }
}


void CFlash_Multy::OnDestroy()
{
    KillProcessFromName(_T("ISP.exe"));
    if(Call_ISP_Application != NULL)
        TerminateThread(Call_ISP_Application, 0);
    Call_ISP_Application=NULL;
    if(Check_Online_Thread != NULL)
        TerminateThread(Check_Online_Thread, 0);
    Check_Online_Thread=NULL;


    CDialogEx::OnDestroy();

    
}

//��ȡ���ݿ����ж��� ��Ʒ����;
void CFlash_Multy::GetProductType()
{
    download_info_type.clear();
    for (int i=0; i<flash_device.size(); i++)
    {
        if (!m_flash_multy_list.GetCellChecked(i,FLASH_ITEM))
        {
            continue;
        }

        if(flash_device.at(i).product_id >= 200)
            continue;
        if(flash_device.at(i).online == false)
            continue;
        Str_download_firmware_info push_item;
        memset(&push_item,0,sizeof(Str_download_firmware_info));
        if(download_info_type.size() == 0)
        {
            push_item.download_product_type =  flash_device.at(i).product_id;
            download_info_type.push_back(push_item);
            continue;
        }
        else
        {
            bool already_in_vector = false;
            for (int j=0; j<download_info_type.size(); j++)
            {
                if(flash_device.at(i).product_id == download_info_type.at(j).download_product_type)
                {
                    already_in_vector = true;
                    break;
                }
            }
            if(already_in_vector == false)
            {
                push_item.download_product_type = flash_device.at(i).product_id;
                download_info_type.push_back(push_item);
                continue;
            }
        }
    }

    // download_product_type
}

void CFlash_Multy::Get_Device_Firmware()
{
    for (int i = 0; i < flash_device.size(); i++)
    {
        if (!m_flash_multy_list.GetCellChecked(i, FLASH_ITEM))
        {
            continue;
        }
        if (flash_device.at(i).online == 0)
        {
            continue;
        }
        close_com();
        close_bac_com();
        unsigned int temp_time_now = time(NULL);
        int temp_time = GetPrivateProfileInt(_T("LastUpdateTime"), flash_device.at(i).strSN, 0, g_ext_mass_flash_path);
        if ((temp_time_now - temp_time) > (3600 * 24))
        {

        }
        else
        {
            CString temp_ver;

            GetPrivateProfileString(_T("DeviceFirmwareVersion"), flash_device.at(i).strSN,_T("0"), temp_ver.GetBuffer(MAX_PATH), MAX_PATH, g_ext_mass_flash_path);
            temp_ver.ReleaseBuffer();
            m_flash_multy_list.SetItemText(i, FLASH_FIRMWARE_VERSION, temp_ver);
            flash_device.at(i).software_rev = _wtof(temp_ver);
            continue;
        }
        //GetPrivateProfileInt(_T("DeviceVersion"), flash_device.at(i).strSN, 0, g_ext_mass_flash_path);

        if (flash_device.at(i).ncomport.IsEmpty() && flash_device.at(i).nBaudrate.IsEmpty())
        {
            //���������ӵ��豸;
            unsigned short temp_port = 0;
            temp_port = _wtoi(flash_device.at(i).nipport);
            if (Open_Socket_Retry(flash_device.at(i).nIPaddress, temp_port,1))
            {
                int multy_ret = 0;
                unsigned short temp_buffer[20];
                unsigned char temp_id = 0;
                temp_id = _wtoi(flash_device.at(i).nID);
                SetCommunicationType(1);
                multy_ret = Read_Multi(temp_id, temp_buffer, 0, 10, 5);
                if (multy_ret >= 0)
                {
                    CString temp_cs;
                    temp_cs.Format(_T("%u"), temp_time_now);
                    WritePrivateProfileString(_T("LastUpdateTime"), flash_device.at(i).strSN, temp_cs, g_ext_mass_flash_path);

                    flash_device.at(i).software_rev = temp_buffer[5] + ((float)temp_buffer[6]) / 10.0;
                    flash_device.at(i).online = 1;
                    m_flash_multy_list.SetItemText(i, FLASH_ONLINE, _T("Online"));
                    m_flash_multy_list.SetItemTextColor(i, FLASH_ONLINE, RGB(0, 0, 0));
                    temp_cs.Format(_T("%.1f"), flash_device.at(i).software_rev);
                    WritePrivateProfileString(_T("DeviceFirmwareVersion"), flash_device.at(i).strSN, temp_cs, g_ext_mass_flash_path);

                }
                else
                {
                    flash_device.at(i).software_rev = 0;
                }
            }
            else
            {
                flash_device.at(i).software_rev = 0;
                flash_device.at(i).online = 0;
                m_flash_multy_list.SetItemText(i, FLASH_ONLINE, _T("Offline"));
                m_flash_multy_list.SetItemTextColor(i, FLASH_ONLINE, RGB(255, 0, 0));
            }
        }
        else
        {

        }

        CString temp_version;
        temp_version.Format(_T("%.1f"), flash_device.at(i).software_rev);
        TRACE(_T("item :%d   %s\r\n"),i, temp_version);
        m_flash_multy_list.SetItemText(i, FLASH_FIRMWARE_VERSION, temp_version);
    }
}



//DWORD WINAPI  CFlash_Multy::get_fwversion_thread(LPVOID lpVoid)
//{
//    //Write_Config_Info
//    Auto_stage = 1;
//    CFlash_Multy* pParent = (CFlash_Multy*)lpVoid;
//    pParent->Get_Device_Firmware();
//    Auto_stage = 2;
//    ::PostMessage(pParent->m_hWnd,WM_COMMAND,MAKEWPARAM(IDC_BUTTON_UPDATE_FIRMWARE, BN_CLICKED), NULL);
//    return 0;
//}
void CFlash_Multy::OnBnClickedButtonUpdateFirmware()
{
    //if (Auto_stage == 0)
    //{
    //    h_thread_get_version = CreateThread(NULL, NULL, get_fwversion_thread, this, NULL, NULL);
    //    return;
    //}

    GetProductType();
    flash_multi_auto = true;
   
    for (int z= 0; z< download_info_type.size(); z++)
    {
        m_product_isp_auto_flash.baudrate = 19200;
        m_product_isp_auto_flash.BuildingInfo.strIp.Empty();
        m_product_isp_auto_flash.ncomport =  1;

        m_product_isp_auto_flash.product_class_id =  download_info_type.at(z).download_product_type;
        m_product_isp_auto_flash.product_id =  35;

        Dowmloadfile Dlg;
        Dlg.DoModal();
    }


    //�ж�����ȡ���ļ����Ƿ���ڣ������ھ����;
    for (int m=0; m<download_info_type.size(); m++)
    {
        CString temp_file_path;
        MultiByteToWideChar( CP_ACP, 0, (char *)download_info_type.at(m).firmware_file_path,
                             (int)strlen((char *)(char *)download_info_type.at(m).firmware_file_path)+1,
                             temp_file_path.GetBuffer(MAX_PATH), MAX_PATH );
        temp_file_path.ReleaseBuffer();

        if(!temp_file_path.IsEmpty())
        {
            CFileFind temp_find ;
            if(!temp_find.FindFile(temp_file_path))
            {
                memset(download_info_type.at(m).firmware_file_path,0,255);
            }
        }
    }

    //����ȡ���� ���°汾�� ����� ���;
    for (int i=0; i<flash_device.size(); i++)
    {
        if (!m_flash_multy_list.GetCellChecked(i, FLASH_ITEM))
        {
            continue;
        }

        bool find_product_and_firmware_exsit = false;
        for (int j=0; j<download_info_type.size(); j++)
        {
            if((flash_device.at(i).product_id == download_info_type.at(j).download_product_type) &&
                    (strlen(download_info_type.at(j).firmware_file_path) > 0))
            {
                CString temp_serial;
                temp_serial = flash_device.at(i).strSN;

                CString temp_firmware_path;
                MultiByteToWideChar( CP_ACP, 0, (char *)download_info_type.at(j).firmware_file_path,
                                     (int)strlen((char *)(char *)download_info_type.at(j).firmware_file_path)+1,
                                     temp_firmware_path.GetBuffer(MAX_PATH), MAX_PATH );
                temp_firmware_path.ReleaseBuffer();
                WritePrivateProfileStringW(_T("FirmwareFile"), temp_serial, temp_firmware_path, g_ext_mass_flash_path);
                m_flash_multy_list.SetItemText(i,FLASH_FILE_POSITION,temp_firmware_path);

                unsigned char m_high;
                unsigned char m_low;
                Get_File_Version(temp_firmware_path,m_high,m_low);




                float temp_file_float;
                temp_file_float = ((float)(m_high*256 + m_low)) / 10;
                CString temp_file_version;
                temp_file_version.Format(_T("%.1f"),temp_file_float);
                m_flash_multy_list.SetItemText(i,FLASH_FILE_REV,temp_file_version);
                //flash_device.at(i).
                //Get_HexFile_Information
                //Get_Binfile_Information
            }
        }
    }
    

    flash_multi_auto = false;
    OnBnClickedButtonStatrt();
}

//�� TemcoProductFirmware   �����һ��;
int Get_File_Version(LPCTSTR filepath,unsigned char &HighVersion,unsigned char &LowVersion)
{
    CFileFind fFind;
    if(!fFind.FindFile(filepath))
        return FILE_NOT_FIND;
    CString orignal_path = filepath;
    CString npath = filepath;
    npath = npath.Right(3);
    npath.MakeUpper();
    Bin_Info temp1;
    if(npath.CompareNoCase(_T("HEX")) == 0 )
    {
        int ret_return = Get_HexFile_Information(filepath,temp1);
        if(ret_return == READ_SUCCESS)
        {
            HighVersion = temp1.software_high;
            LowVersion = temp1.software_low;
        }
        return ret_return;

    }
    else if(npath.CompareNoCase(_T("BIN")) == 0 )
    {
        int ret_return = Get_Binfile_Information(filepath,temp1);
        if(ret_return == READ_SUCCESS)
        {
            if((strcmp(temp1.product_name,"MiniPanel") == 0) || (strcmp(temp1.product_name,"CM5")))
            {
                CString convert_software_high;
                convert_software_high.Format(_T("%u"),temp1.software_high);
                CString convert_software_low;
                convert_software_low.Format(_T("%u"),temp1.software_low);

                int temp_value_high = 0;
                temp_value_high = _wtoi(convert_software_high);
                int temp_value_low = 0;
                temp_value_low = _wtoi(convert_software_low);

                unsigned short new_version = 0;
                new_version = temp_value_high *10 + temp_value_low;

                HighVersion = (new_version & 0xff00)>>8;
                LowVersion = (new_version & 0x00ff);

            }
            else
            {
                HighVersion = temp1.software_high;
                LowVersion = temp1.software_low;
            }

        }
        return ret_return;
    }
    else if(npath.CompareNoCase(_T("ZIP")) == 0)
    {
        if(orignal_path.Find(_T("T3000")))
        {

        }
        else
        {
            return FILE_NOT_FIND;
        }
    }
    else
    {
        return FILE_NOT_FIND;
    }
    return READ_SUCCESS;
}


