// Flash_Multy.cpp : implementation file
// This file added by Fance Du 2014/06

#include "stdafx.h"
#include "T3000.h"
#include "Flash_Multy.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CFlash_Multy dialog
CString ApplicationFolder;
CString MultyISPtool_path;
CString Config_Path;

CString Edit_File_Path;

#define FLASH_COLOR_BLUE RGB(50,50,180)
#define FLASH_COLOR_RED  RGB(255,0,0)
#define FLASH_COLOR_GREEN  RGB(0,255,0)
#define FLASH_COLOR_DEFAULT  RGB(0,0,0)

vector <Str_flash_device> flash_device;

IMPLEMENT_DYNAMIC(CFlash_Multy, CDialogEx)

CFlash_Multy::CFlash_Multy(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFlash_Multy::IDD, pParent)
{

}

CFlash_Multy::~CFlash_Multy()
{
}

void CFlash_Multy::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FLASH_MULTY, m_flash_multy_list);
	DDX_Control(pDX, IDC_LIST_MULTY_INFORMATION, m_multy_flash_listbox);
}


BEGIN_MESSAGE_MAP(CFlash_Multy, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_APPLY_WO_ALL_SELECT, &CFlash_Multy::OnBnClickedButtonApplyWoAllSelect)
	ON_BN_CLICKED(IDC_BUTTON_MULTY_SEL, &CFlash_Multy::OnBnClickedButtonMultySel)
	ON_BN_CLICKED(IDC_BUTTON_STATRT, &CFlash_Multy::OnBnClickedButtonStatrt)
	ON_WM_TIMER()
	ON_MESSAGE(WM_MULTY_FLASH_MESSAGE,MultyFlashMessage)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FLASH_MULTY, &CFlash_Multy::OnNMClickListFlashMulty)
END_MESSAGE_MAP()


// CFlash_Multy message handlers


BOOL CFlash_Multy::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
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
	m_flash_multy_list.InsertColumn(FLASH_RESULTS, _T("Results"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);

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
	for (int i=0;i<ncount;i++)
	{
		BOOL is_checked = m_flash_multy_list.GetCellChecked(i,FLASH_ITEM);
		if(is_checked)
		{
			nflashitemcount ++;
			Str_flash_device temp;
			temp.nitem = i;
			temp.nID = m_flash_multy_list.GetItemText(i,FLASH_ID);
			temp.ncomport = m_flash_multy_list.GetItemText(i,FLASH_COM_PORT);
			temp.nIPaddress = m_flash_multy_list.GetItemText(i,FLASH_IPADDRESS);
			temp.nipport = m_flash_multy_list.GetItemText(i,FLASH_IPPORT);

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
			temp.nresult = 0;
			flash_device.push_back(temp);
		}
	}
	if(nflashitemcount == 0)
	{
		MessageBox(_T("Please select one or more items."),_T("Notice"),MB_OK | MB_ICONINFORMATION);
		return;
	}
	HANDLE Call_ISP_Application = NULL;
	Call_ISP_Application =CreateThread(NULL,NULL,multy_isp_thread,this,NULL, NULL);

}


const int CHANGE_THE_ITEM_COLOR_BLUE = 1;
const int CHANGE_THE_ITEM_COLOR_RED = 2;
const int CHANGE_THE_ITEM_COLOR_GREEN = 3;
const int  CHANGE_THE_ITEM_COLOR_DEFAULT = 4;


int multy_log_count = 1;	//用于读取改读多少count了;
DWORD WINAPI  CFlash_Multy::multy_isp_thread(LPVOID lpVoid)
{
	//Write_Config_Info
	CFlash_Multy *pParent = (CFlash_Multy *)lpVoid;
	int nflashdevicecount = (int)flash_device.size();
	for(int i=0;i<nflashdevicecount;i++)
	{
		pParent->SetAutoConfig(flash_device.at(i));
		pParent->PostMessage(WM_MULTY_FLASH_MESSAGE,CHANGE_THE_ITEM_COLOR_BLUE,flash_device.at(i).nitem);
		multy_log_count = 1;
		WinExecAndWait(MultyISPtool_path,NULL,NULL,0);
		int nresult = GetPrivateProfileInt(_T("Data"),_T("Command"),FAILED_UNKNOW_ERROR,Config_Path);
		flash_device.at(i).nresult = nresult;
		if(nresult == FLASH_SUCCESS)
			pParent->PostMessage(WM_MULTY_FLASH_MESSAGE,CHANGE_THE_ITEM_COLOR_GREEN,flash_device.at(i).nitem);
		else
			pParent->PostMessage(WM_MULTY_FLASH_MESSAGE,CHANGE_THE_ITEM_COLOR_RED,flash_device.at(i).nitem);

	}

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

	CDialogEx::OnTimer(nIDEvent);
}


LRESULT CFlash_Multy::MultyFlashMessage(WPARAM wParam,LPARAM lParam)
{
	int main_command = (int)wParam;
	int sub_parameter = (int)lParam;
	switch(main_command)
	{
	case CHANGE_THE_ITEM_COLOR_BLUE:
		{
			m_flash_multy_list.SetItemTextColor(sub_parameter,-1,FLASH_COLOR_BLUE);
		}
		break;
	case CHANGE_THE_ITEM_COLOR_GREEN:
		{
			m_flash_multy_list.SetItemTextColor(sub_parameter,-1,FLASH_COLOR_GREEN);
		}
		break;
	case CHANGE_THE_ITEM_COLOR_RED:
		{
			m_flash_multy_list.SetItemTextColor(sub_parameter,-1,FLASH_COLOR_RED);
		}
		break;
	case CHANGE_THE_ITEM_COLOR_DEFAULT:
		{
			m_flash_multy_list.SetItemTextColor(sub_parameter,-1,FLASH_COLOR_DEFAULT);
		}
		break;
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
