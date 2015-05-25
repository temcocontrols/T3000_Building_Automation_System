// Flash_Multy.cpp : implementation file
// This file added by Fance Du 2014/06

#include "stdafx.h"
#include "T3000.h"
#include "Flash_Multy.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "globle_function.h"

// CFlash_Multy dialog
CString ApplicationFolder;
CString MultyISPtool_path;
CString Config_Path;

CString Edit_File_Path;


const int CHANGE_THE_ITEM_COLOR_BLUE = 1;//烧写开始，表示正在进行的1>正在进行的项目：就是蓝色
const int CHANGE_THE_ITEM_COLOR_RED = 2;//烧写失败2>烧写没有成功,就是红色
const int CHANGE_THE_ITEM_COLOR_GREEN = 3;//烧写成功3>烧写成功，是绿色
const int  CHANGE_THE_ITEM_COLOR_DEFAULT = 4;//默认
const int CHANGE_THE_ITEM_COLOR_MORE_GREEN = 5; //4>烧写成功&&配置文件，成功,
const int CHANGE_THE_ITEM_COLOR_LESS_RED = 6;//5>配置文件，失败

#define FLASH_COLOR_BLUE RGB(50,50,180)
#define FLASH_COLOR_RED  RGB(255,0,0)
#define FLASH_COLOR_GREEN  RGB(0,255,0)
#define FLASH_COLOR_DEFAULT  RGB(0,0,0)

#define CONFIG_COLOR_RED_FAIL  RGB(255,86,86)
#define CONFIG_COLOR_CONFIG_FLASH_GOOD  RGB(86,255,86)


vector <Str_flash_device> flash_device;

IMPLEMENT_DYNAMIC(CFlash_Multy, CDialogEx)

CFlash_Multy::CFlash_Multy(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFlash_Multy::IDD, pParent)
{
m_bTstatLoadFinished=FALSE;
 
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
END_MESSAGE_MAP()


// CFlash_Multy message handlers


BOOL CFlash_Multy::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_strLogFilePath=g_strExePth+_T("Load_config_Log");
	CreateDirectory(m_strLogFilePath,NULL);//create directory file
	Initial_List();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CFlash_Multy::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CFlash_Multy::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnClose();
}


void CFlash_Multy::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

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
	m_flash_multy_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_flash_multy_list.SetExtendedStyle(m_flash_multy_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_flash_multy_list.InsertColumn(FLASH_ITEM, _T("NUM"), 40, ListCtrlEx::CheckBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_flash_multy_list.InsertColumn(FLASH_SERIAL_NUMBER, _T("SN"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_flash_multy_list.InsertColumn(FLASH_ID, _T("ID"), 40, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_flash_multy_list.InsertColumn(FLASH_PRODUCT_NAME, _T("Name"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_flash_multy_list.InsertColumn(FLASH_COM_PORT, _T("Com"), 40, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_flash_multy_list.InsertColumn(FLASH_IPADDRESS, _T("IP"), 120, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_flash_multy_list.InsertColumn(FLASH_IPPORT, _T("Port"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_flash_multy_list.InsertColumn(FLASH_SUB_NOTE, _T("Is Sub"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_flash_multy_list.InsertColumn(FLASH_FILE_POSITION, _T("Hex Bin File"), 350, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_flash_multy_list.InsertColumn(FLASH_CONFIG_FILE_POSITION, _T("Configure File"), 350, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);	
	m_flash_multy_list.InsertColumn(FLASH_RESULTS, _T("Firmware Results"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_flash_multy_list.InsertColumn(FLASH_CONFIG_RESULTS, _T("Config Results"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_flash_multy_hwnd = this->m_hWnd;
	g_hwnd_now = m_flash_multy_hwnd;

	//CRect list_rect,win_rect;
	//m_flash_multy_list.GetWindowRect(list_rect);
	//ScreenToClient(&list_rect);
	//::GetWindowRect(m_input_dlg_hwnd,win_rect);
	//m_flash_multy_list.Set_My_WindowRect(win_rect);
	//m_flash_multy_list.Set_My_ListRect(list_rect);

	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	m_flash_multy_list.DeleteAllItems();


	for(int i=0;i<(int)pFrame->m_product.size();i++)
	{
		int j=pFrame->m_product.at(i).product_id;
		CString nitem;
		CString nSerialNum;
		CString nID;
		CString nProductName;
		CString nComport;
		CString nIPAddress;
		CString nIP_Port;
		CString n_is_sub;
		CString nFilePosition;
		CString nresults;
		nitem.Format(_T("%d"),i+1);
		nSerialNum.Format(_T("%d"),pFrame->m_product.at(i).serial_number);
		nID.Format(_T("%d"),pFrame->m_product.at(i).product_id);
		nProductName = GetProductName(pFrame->m_product.at(i).product_class_id);

		if(pFrame->m_product.at(i).BuildingInfo.strIp.IsEmpty())
		{
			nComport.Format(_T("%d"),pFrame->m_product.at(i).ncomport);
			n_is_sub = _T("NO");
			nIP_Port.Empty();
			nIPAddress.Empty();
		}
		else
		{
			nComport.Empty();
			nIPAddress = pFrame->m_product.at(i).BuildingInfo.strIp;
			nIP_Port.Format(_T("%d"),pFrame->m_product.at(i).ncomport);
			if(IsOurNetDevice(pFrame->m_product.at(i).product_class_id) == false)//如果不是网络设备就说明是TSTAT类的串口设备;
			{
				n_is_sub = _T("YES");
			}
			else
			{
				n_is_sub = _T("NO");
			}
		}

		m_flash_multy_list.InsertItem(i,nitem);
		m_flash_multy_list.SetItemText(i,FLASH_SERIAL_NUMBER,nSerialNum);
		m_flash_multy_list.SetItemText(i,FLASH_ID,nID);
		m_flash_multy_list.SetItemText(i,FLASH_PRODUCT_NAME,nProductName);
		m_flash_multy_list.SetItemText(i,FLASH_COM_PORT,nComport);
		m_flash_multy_list.SetItemText(i,FLASH_IPADDRESS,nIPAddress);
		m_flash_multy_list.SetItemText(i,FLASH_IPPORT,nIP_Port);
		m_flash_multy_list.SetItemText(i,FLASH_SUB_NOTE,n_is_sub);
		

		//FLASH_FILE_POSITION


		pFrame->GetProductFPTAndLocalPath(pFrame->m_product.at(i).product_class_id,StrULRPath,ProductHexBinName);
		StrBinHexPath = g_strExePth;
		StrBinHexPath+=_T("firmware\\");

		StrTemp.Format(_T("%s\\"),GetProductName(pFrame->m_product.at(i).product_class_id));
		StrBinHexPath+=StrTemp;
		StrBinHexPath+=ProductHexBinName;

		HANDLE hFind;//
		WIN32_FIND_DATA wfd;//
		hFind = FindFirstFile(StrBinHexPath, &wfd);//
		if ((hFind!=INVALID_HANDLE_VALUE)&&(!StrULRPath.IsEmpty()))//说明当前目录下无t3000.mdb
		{
			//filename=StrBinHexPath;
			m_flash_multy_list.SetItemText(i,FLASH_FILE_POSITION,StrBinHexPath);
		}
		for (int x=0;x<FLASH_MAX_COUNT;x++)
		{
			if((i%2)==0)
				m_flash_multy_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_flash_multy_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}

	}
	Initial_Parameter();
	return;
}


void CFlash_Multy::Initial_Parameter()
{
	
	GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
	PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
	ApplicationFolder.ReleaseBuffer();
	MultyISPtool_path = ApplicationFolder + _T("\\ISP.exe");
	Config_Path = ApplicationFolder + _T("\\AutoFlashFile.ini");
}

BOOL CFlash_Multy::IsOurNetDevice(int DevType)
{
	if (DevType == PM_LightingController 
		|| DevType == PM_NC
		|| DevType == PM_CO2_NET
		|| DevType == PM_MINIPANEL
		|| DevType == PM_CM5)
	{
		return TRUE;
	}

	return FALSE;
}


void CFlash_Multy::OnBnClickedButtonApplyWoAllSelect()
{
	// TODO: Add your control notification handler code here
 	int ncount = m_flash_multy_list.GetItemCount();
 	for (int i=0;i<ncount;i++)
 	{
  		BOOL is_checked = m_flash_multy_list.GetCellChecked(i,FLASH_ITEM);
  		if(is_checked)
			m_flash_multy_list.SetItemText(i,FLASH_FILE_POSITION,Edit_File_Path);
	}
}


void CFlash_Multy::OnBnClickedButtonMultySel()
{
	// TODO: Add your control notification handler code here

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
	if(tempfind.FindFile(Config_Path))
	{
		DeleteFile(Config_Path);
	}
	WritePrivateProfileStringW(_T("Data"),_T("Command"),_T("1"),Config_Path);
	if(ndevice_info.nIPaddress.IsEmpty())//串口
	{
		WritePrivateProfileStringW(_T("Data"),_T("COM_OR_NET"),_T("COM"),Config_Path);
		CString cs_comport;
		cs_comport = _T("COM") +  ndevice_info.ncomport;
		WritePrivateProfileStringW(_T("Data"),_T("COMPORT"),cs_comport,Config_Path);
		WritePrivateProfileStringW(_T("Data"),_T("Baudrate"),_T("19200"),Config_Path);

		CString nflash_id;
		nflash_id = ndevice_info.nID;
		WritePrivateProfileStringW(_T("Data"),_T("ID"),nflash_id,Config_Path);
	}
	else
	{
		WritePrivateProfileStringW(_T("Data"),_T("COM_OR_NET"),_T("NET"),Config_Path);
		WritePrivateProfileStringW(_T("Data"),_T("IPAddress"),ndevice_info.nIPaddress,Config_Path);

		CString n_tcpport;
		n_tcpport = ndevice_info.nipport;

		WritePrivateProfileStringW(_T("Data"),_T("IPPort"),n_tcpport,Config_Path);
		if(ndevice_info.b_is_sub == true)
		{
			WritePrivateProfileStringW(_T("Data"),_T("Subnote"),_T("1"),Config_Path);
			CString nsub_id;
			nsub_id = ndevice_info.nID;
			WritePrivateProfileStringW(_T("Data"),_T("SubID"),nsub_id,Config_Path);
		}
		else
		{
			WritePrivateProfileStringW(_T("Data"),_T("Subnote"),_T("0"),Config_Path);
		}
	}

	CString mypath = ndevice_info.file_position;

	WritePrivateProfileStringW(_T("Data"),_T("FirmwarePath"),mypath,Config_Path);





}



void CFlash_Multy::OnBnClickedButtonStatrt()
{
	// TODO: Add your control notification handler code here
	int nflashitemcount = 0;
	SetTimer(1,200,NULL);
	flash_device.clear();
	int ncount = m_flash_multy_list.GetItemCount();
 
	m_bTstatLoadFinished=FALSE;
 
	for (int i=0;i<ncount;i++)
	{
		BOOL is_checked = m_flash_multy_list.GetCellChecked(i,FLASH_ITEM);
		if(is_checked)
		{
			nflashitemcount ++;
			Str_flash_device temp;
			temp.nitem = i;
			

			temp.strSN= m_flash_multy_list.GetItemText(i,FLASH_SERIAL_NUMBER);
			temp.nID = m_flash_multy_list.GetItemText(i,FLASH_ID);
			temp.ncomport = m_flash_multy_list.GetItemText(i,FLASH_COM_PORT);
			temp.nIPaddress = m_flash_multy_list.GetItemText(i,FLASH_IPADDRESS);
			temp.nipport = m_flash_multy_list.GetItemText(i,FLASH_IPPORT);
			temp.devicename=m_flash_multy_list.GetItemText(i,FLASH_PRODUCT_NAME);
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
			temp.nresult = 0;
			flash_device.push_back(temp);
		}
	}
	if(nflashitemcount == 0)
	{
		MessageBox(_T("Please select one or more items."),_T("Notice"),MB_OK | MB_ICONINFORMATION);
		m_bTstatLoadFinished=TRUE;
		return;
	}
	else{
		HANDLE Call_ISP_Application = NULL;
		Call_ISP_Application =CreateThread(NULL,NULL,multy_isp_thread,this,NULL, NULL);
	}


}


/*
const int CHANGE_THE_ITEM_COLOR_BLUE = 1;//烧写开始，表示正在进行的1>正在进行的项目：就是蓝色
const int CHANGE_THE_ITEM_COLOR_RED = 2;//烧写失败2>烧写没有成功,就是红色
const int CHANGE_THE_ITEM_COLOR_GREEN = 3;//烧写成功3>烧写成功，是绿色
const int  CHANGE_THE_ITEM_COLOR_DEFAULT = 4;//默认
const int CHANGE_THE_ITEM_COLOR_MORE_GREEN = 5; //4>烧写成功&&配置文件，成功,
const int CHANGE_THE_ITEM_COLOR_LESS_RED = 6;//5>配置文件，失败

#define FLASH_COLOR_BLUE RGB(50,50,180)
#define FLASH_COLOR_RED  RGB(255,0,0)
#define FLASH_COLOR_GREEN  RGB(0,255,0)
#define FLASH_COLOR_DEFAULT  RGB(0,0,0)

#define CONFIG_COLOR_RED_FAIL  RGB(255,86,86)
#define CONFIG_COLOR_CONFIG_FLASH_GOOD  RGB(86,255,86)
*/


int multy_log_count = 1;	//用于读取改读多少count了;
DWORD WINAPI  CFlash_Multy::multy_isp_thread(LPVOID lpVoid)
{
	//Write_Config_Info
	CFlash_Multy *pParent = (CFlash_Multy *)lpVoid;
	int nflashdevicecount = (int)flash_device.size();
	int comport=0;
	BOOL IS_COM=TRUE;
	CString currentIp;
	int Port;
	BOOL is_connect_device=FALSE;
	 
	CString log_file_path;
	CString config_file_path;
	CFile config_file;//the hex file
	CStdioFile log_file;
	bool log_file_opened=false;
	for(int i=0;i<nflashdevicecount;i++)
	{

	if (!flash_device.at(i).file_position.IsEmpty())
	{
		log_file_opened=false;
		pParent->SetAutoConfig(flash_device.at(i));
		pParent->PostMessage(WM_MULTY_FLASH_MESSAGE,CHANGE_THE_ITEM_COLOR_BLUE,flash_device.at(i).nitem);
		multy_log_count = 1;
		WinExecAndWait(MultyISPtool_path,NULL,NULL,0);
		int nresult = GetPrivateProfileInt(_T("Data"),_T("Command"),FAILED_UNKNOW_ERROR,Config_Path);
		//flash_device.at(i).nresult = nresult;
		if(nresult == FLASH_SUCCESS)
			flash_device.at(i).nresult=CHANGE_THE_ITEM_COLOR_GREEN;
		//pParent->PostMessage(WM_MULTY_FLASH_MESSAGE,CHANGE_THE_ITEM_COLOR_GREEN,flash_device.at(i).nitem);
		else
			flash_device.at(i).nresult=CHANGE_THE_ITEM_COLOR_RED;
		//pParent->PostMessage(WM_MULTY_FLASH_MESSAGE,CHANGE_THE_ITEM_COLOR_RED,flash_device.at(i).nitem);

		if (nresult!=FLASH_SUCCESS)
		{
			continue;
		}
		Sleep(4000);
	}
	
	if (flash_device.at(i).config_file_position.IsEmpty())
	{
	continue;
	}
		#if 1

		
		if(is_connect()){
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
		if (!flash_device.at(i).ncomport.IsEmpty())//说明是非串口设备
		{
		  comport=_wtoi(flash_device.at(i).ncomport);
		  IS_COM=TRUE;
		}
		else{
		IS_COM=FALSE;
		currentIp=flash_device.at(i).nIPaddress;
		Port=_wtoi(flash_device.at(i).nipport);
		}
		 
		
		 if (IS_COM)
		 {
		    if (open_com(comport))
		    {
			is_connect_device=TRUE;
		    }
			else
			{
			is_connect_device=FALSE;
			}
		    SetCommunicationType(0);
		 }
		 else{
	
		     if (Open_Socket2(currentIp,Port))
		     {
			 is_connect_device=TRUE;
		     }
			 else{
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
		 nFlag = read_one(now_tstat_id,7,6);

		 if((nFlag == PM_TSTAT6) || (nFlag == PM_TSTAT7) || (nFlag == PM_TSTAT5i))
		 {
			 product_type =T3000_6_ADDRESS;
		 }
		 else if((nFlag == PM_TSTAT5E)||(product_register_value[7]==PM_TSTATRUNAR) || (nFlag == PM_TSTAT5H)||(nFlag == PM_TSTAT5G))
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
			 nFlag== PM_T38AI16O||
			 nFlag==PM_T38I13O||
			 nFlag==PM_T3PERFORMANCE||
			 nFlag==PM_T34AO||
			 nFlag==PM_T36CT 
			 )
		 {
			 product_type =T3000_T3_MODULES;
		 }
		 else{
		 product_type=nFlag;
		 }



		  if(config_file.Open(config_file_path,CFile::modeRead | CFile::shareDenyNone))
		{
	        
			CString strPath=config_file.GetFilePath();
			CString strFileName=config_file.GetFileTitle();
			open_file=true;
			config_file.Close();
		//	pDlg->m_infoListBox.InsertString(0,_T(""));
		//nowmoder=read_one(now_tstat_id,7,7);
			if(log_file_opened==true){
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
				else
				{
				LoadFile2Tstat(temppp,(LPTSTR)(LPCTSTR)config_file_path,&log_file);
				}
			}
// 			if(find_load_file_error(temppp))
// 			{
// 				CString for_showing_text;
// 				for_showing_text.Format(_T("ID :%d Load found Error"),now_tstat_id);
// 				change_showing_text_variable(for_showing_text);
// // 				pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,COL_LOADEVENT,_T(""));
// // 				failure_number++;	
// // 				pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,COL_STATUS,_T("Error"));
// 			}
// 			else
// 			{
// 				CString for_showing_text;
// 				for_showing_text.Format(_T("ID :%d Load Success"),now_tstat_id);
// 				log_file.WriteString(for_showing_text);
// // 				change_showing_text_variable(for_showing_text);
// // 				pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,COL_ENABLE,_T("False"));
// // 				pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,COL_STATUS,_T("OK"));
// 
// 				
// 		
// 			}
			//const int CHANGE_THE_ITEM_COLOR_MORE_GREEN = 5; //4>烧写成功&&配置文件，成功,
			//const int CHANGE_THE_ITEM_COLOR_LESS_RED = 6;//5>配置文件，失败
			     if (g_Vector_Write_Error.size()>0)
                 {
				  flash_device.at(i).nresult=CHANGE_THE_ITEM_COLOR_LESS_RED;
                 }
                 else{
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

	//::PostMessage(downloadfile_hwnd,WM_DOWNLOADFILE_MESSAGE,DOWNLOAD_RESULTS,NULL);
	return 0;
}

 
void CFlash_Multy::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	int temp_count = 0;
	temp_count = GetPrivateProfileInt(_T("LogInfo"),_T("AddCount"),0,Config_Path);
	if(multy_log_count <= temp_count)
	{
		CString temp_string;
		CString section;
		section.Format(_T("AddText%d"),multy_log_count);
		GetPrivateProfileStringW(_T("LogInfo"),section,_T(""),temp_string.GetBuffer(MAX_PATH),MAX_PATH,Config_Path);
		temp_string.ReleaseBuffer();
		multy_log_count ++;
		if(!temp_string.IsEmpty())
		{
			m_multy_flash_listbox.InsertString(m_multy_flash_listbox.GetCount(),temp_string);
			m_multy_flash_listbox.SetTopIndex(m_multy_flash_listbox.GetCount()-1);
		}
	}

	int is_replace_refresh = GetPrivateProfileInt(_T("LogInfo"),_T("Replace_Refresh"),0,Config_Path);
	if(is_replace_refresh)
	{
		int temp_replace_count = GetPrivateProfileInt(_T("LogInfo"),_T("ReplaceCount"),0,Config_Path);	
		CString temp_section;
		temp_section.Format(_T("ReplaceText%d"),temp_replace_count);
		CString replace_string;
		GetPrivateProfileStringW(_T("LogInfo"),temp_section,_T(""),replace_string.GetBuffer(MAX_PATH),MAX_PATH,Config_Path);
		replace_string.ReleaseBuffer();
		int nLineCount = m_multy_flash_listbox.GetCount();
		m_multy_flash_listbox.DeleteString(nLineCount - 1);

		m_multy_flash_listbox.InsertString(m_multy_flash_listbox.GetCount(),replace_string);
		m_multy_flash_listbox.SetTopIndex(m_multy_flash_listbox.GetCount()-1);
		WritePrivateProfileStringW(_T("LogInfo"),_T("Replace_Refresh"),_T("0"),Config_Path);	

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
		GetDlgItem(IDC_BUTTON_STATRT)->EnableWindow(FALSE);

		if (m_bTstatLoadFinished)
		{
			GetDlgItem(IDC_BUTTON_STATRT)->EnableWindow(TRUE);
			KillTimer(1);
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
const int CHANGE_THE_ITEM_COLOR_BLUE = 1;//烧写开始，表示正在进行的1>正在进行的项目：就是蓝色
const int CHANGE_THE_ITEM_COLOR_RED = 2;//烧写失败2>烧写没有成功,就是红色
const int CHANGE_THE_ITEM_COLOR_GREEN = 3;//烧写成功3>烧写成功，是绿色
const int  CHANGE_THE_ITEM_COLOR_DEFAULT = 4;//默认
const int CHANGE_THE_ITEM_COLOR_MORE_GREEN = 5; //4>烧写成功&&配置文件，成功,
const int CHANGE_THE_ITEM_COLOR_LESS_RED = 6;//5>配置文件，失败

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
	switch(main_command)
	{
	case CHANGE_THE_ITEM_COLOR_BLUE:
		{
			m_flash_multy_list.SetItemTextColor(sub_parameter,-1,FLASH_COLOR_BLUE);
			m_flash_multy_list.SetItemText(sub_parameter,FLASH_RESULTS,_T(""));
		}
		break;
	case CHANGE_THE_ITEM_COLOR_GREEN:
		{
			m_flash_multy_list.SetItemTextColor(sub_parameter,-1,FLASH_COLOR_GREEN);
			m_flash_multy_list.SetItemText(sub_parameter,FLASH_RESULTS,_T("Sucessful"));
		}
		break;
	case CHANGE_THE_ITEM_COLOR_RED:
		{
			m_flash_multy_list.SetItemTextColor(sub_parameter,-1,FLASH_COLOR_RED);
			m_flash_multy_list.SetItemText(sub_parameter,FLASH_RESULTS,_T("Fail"));
		}
		break;
	case CHANGE_THE_ITEM_COLOR_DEFAULT:
		{
			m_flash_multy_list.SetItemTextColor(sub_parameter,-1,FLASH_COLOR_DEFAULT);
			m_flash_multy_list.SetItemText(sub_parameter,FLASH_RESULTS,_T(""));
		}
		break;
	case CHANGE_THE_ITEM_COLOR_MORE_GREEN:{
	m_flash_multy_list.SetItemTextColor(sub_parameter,-1,CONFIG_COLOR_CONFIG_FLASH_GOOD);
	m_flash_multy_list.SetItemText(sub_parameter,FLASH_RESULTS,_T("Sucessful"));
	m_flash_multy_list.SetItemText(sub_parameter,FLASH_CONFIG_RESULTS,_T("Sucessful"));
	}
	break;
	case CHANGE_THE_ITEM_COLOR_LESS_RED:{
		m_flash_multy_list.SetItemTextColor(sub_parameter,-1,CONFIG_COLOR_RED_FAIL);
		m_flash_multy_list.SetItemText(sub_parameter,FLASH_RESULTS,_T("Sucessful"));
		m_flash_multy_list.SetItemText(sub_parameter,FLASH_CONFIG_RESULTS,_T("Fail"));
	}break;
	}
	return 0;
}



void CFlash_Multy::OnNMClickListFlashMulty(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	long lRow,lCol;
	m_flash_multy_list.Set_Edit(true);
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_flash_multy_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_flash_multy_list.SubItemHitTest(&lvinfo);

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;


	if(lRow>m_flash_multy_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow<0)
		return;

	if(lCol == FLASH_FILE_POSITION)
	{
		CString strFilter = _T("hex File;bin File|*.hex;*.bin|all File|*.*||");
		CFileDialog dlg(true,_T("hex"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
		if(IDOK==dlg.DoModal())
		{
			CString temp_path;
			temp_path=dlg.GetPathName();
			m_flash_multy_list.SetItemText(lRow,FLASH_FILE_POSITION,temp_path);
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
	//m_hexFilePathNameEdit.SetWindowText(m_strHexFile);

	/*if(m_AllCheckBtn.GetCheck()==BST_CHECKED)
	{
 
		int ncount = m_flash_multy_list.GetItemCount();
		for (int i=0;i<ncount;i++)
		{
			BOOL is_checked = m_flash_multy_list.GetCellChecked(i,FLASH_ITEM);
			if(is_checked)
				m_flash_multy_list.SetItemText(i,FLASH_FILE_POSITION,Edit_File_Path);
    
	   }
   }*/
}


void CFlash_Multy::OnBnClickedAllcheck()
{
    if (m_configure_path.IsEmpty())
    {
	AfxMessageBox(L"Your Config file path is empty,please select your config file.");
	return;
    }
	int ncount = m_flash_multy_list.GetItemCount();
	for (int i=0;i<ncount;i++)
	{
  		BOOL is_checked = m_flash_multy_list.GetCellChecked(i,FLASH_ITEM);
 		if(is_checked)
			m_flash_multy_list.SetItemText(i,FLASH_CONFIG_FILE_POSITION,m_configure_path);

	}
}
