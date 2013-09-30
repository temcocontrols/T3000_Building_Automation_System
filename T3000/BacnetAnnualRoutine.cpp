// BacnetAnnualRoutine.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetAnnualRoutine.h"
#include "afxdialogex.h"

#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "globle_function.h"
#include "gloab_define.h"
#include "BacnetRange.h"

#include "AnnualRout_InsertDia.h"
// BacnetAnnualRoutine dialog
extern void copy_data_to_ptrpanel(int Data_type);//Used for copy the structure to the ptrpanel.
IMPLEMENT_DYNAMIC(BacnetAnnualRoutine, CDialogEx)

BacnetAnnualRoutine::BacnetAnnualRoutine(CWnd* pParent /*=NULL*/)
	: CDialogEx(BacnetAnnualRoutine::IDD, pParent)
{
	annual_list_line = 0;
}

BacnetAnnualRoutine::~BacnetAnnualRoutine()
{
}

void BacnetAnnualRoutine::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BAC_ANNULE_LIST, m_annualr_list);
}


BEGIN_MESSAGE_MAP(BacnetAnnualRoutine, CDialogEx)
	ON_MESSAGE(WM_HOTKEY,&BacnetAnnualRoutine::OnHotKey)//快捷键消息映射手动加入

	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_BAC_ANNULE_LIST, &BacnetAnnualRoutine::OnNMClickListBacAnnuleList)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Annual_Routine_Item)
	ON_MESSAGE(WM_REFRESH_BAC_ANNUAL_LIST,Fresh_Annual_Routine_List)
//	ON_MESSAGE(MY_RESUME_DATA, AnnualResumeMessageCallBack)
END_MESSAGE_MAP()


// BacnetAnnualRoutine message handlers


BOOL BacnetAnnualRoutine::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::PreTranslateMessage(pMsg);
}

const int KEY_INSERT = 1020;
LRESULT BacnetAnnualRoutine::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	if (wParam==KEY_INSERT)
	{
		AnnualRout_InsertDia Dlg;
		Dlg.DoModal();
	}
	return 0;
}



BOOL BacnetAnnualRoutine::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	Initial_List();
	RegisterHotKey(GetSafeHwnd(),KEY_INSERT,NULL,VK_INSERT);
	PostMessage(WM_REFRESH_BAC_ANNUAL_LIST,NULL,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void BacnetAnnualRoutine::Initial_List()
{
	m_annualr_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_annualr_list.SetExtendedStyle(m_annualr_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_annualr_list.InsertColumn(ANNUAL_ROUTINE_NUM, _T("NUM"), 60, ListCtrlEx::CheckBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_annualr_list.InsertColumn(ANNUAL_ROUTINE_FULL_LABEL, _T("Full Label"), 150, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_annualr_list.InsertColumn(ANNUAL_ROUTINE_AUTO_MANUAL, _T("Auto/Manual"), 90, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_annualr_list.InsertColumn(ANNUAL_ROUTINE_VALUE, _T("Value"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_annualr_list.InsertColumn(ANNUAL_ROUTINE_LABLE, _T("Label"), 90, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_annual_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_annual_dlg_hwnd;

	m_annualr_list.DeleteAllItems();
	for (int i=0;i<(int)m_Annual_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		temp_item.Format(_T("%d"),i+1);
		m_annualr_list.InsertItem(i,temp_item);
		m_annualr_list.SetCellEnabled(i,ANNUAL_ROUTINE_NUM,0);
		if(ListCtrlEx::ComboBox == m_annualr_list.GetColumnType(ANNUAL_ROUTINE_AUTO_MANUAL))
		{
			ListCtrlEx::CStrList strlist;
			strlist.push_back(_T("Auto"));
			strlist.push_back(_T("Manual"));
			m_annualr_list.SetCellStringList(i, ANNUAL_ROUTINE_AUTO_MANUAL, strlist);
		}


		if(ListCtrlEx::ComboBox == m_annualr_list.GetColumnType(ANNUAL_ROUTINE_VALUE))
		{
			ListCtrlEx::CStrList strlist;
			strlist.push_back(_T("OFF"));
			strlist.push_back(_T("ON"));
			m_annualr_list.SetCellStringList(i, ANNUAL_ROUTINE_VALUE, strlist);
		}

	}
	m_annualr_list.SetCellChecked(0,0,1);// default will choose the first one

}


void BacnetAnnualRoutine::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	UnregisterHotKey(GetSafeHwnd(),KEY_INSERT);
	CDialogEx::OnClose();
}


void BacnetAnnualRoutine::OnNMClickListBacAnnuleList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_annualr_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_annualr_list.SubItemHitTest(&lvinfo);
	if(nItem!=-1)
	{
		m_annualr_list.SetCellChecked(nItem,0,1);
		annual_list_line = nItem;
		for (int i=0;i<m_annualr_list.GetItemCount();++i)
		{
			if(i == nItem)
				continue;
			m_annualr_list.SetCellChecked(i,0,FALSE);
		}
	}


	*pResult = 0;
}
LRESULT BacnetAnnualRoutine::Fresh_Annual_Routine_List(WPARAM wParam,LPARAM lParam)
{
	for (int i=0;i<(int)m_Annual_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;


		MultiByteToWideChar( CP_ACP, 0, (char *)m_Annual_data.at(i).description, (int)strlen((char *)m_Annual_data.at(i).description)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();
		m_annualr_list.SetItemText(i,ANNUAL_ROUTINE_FULL_LABEL,temp_des);
		if(m_Annual_data.at(i).auto_manual==0)
		{
			m_annualr_list.SetItemText(i,ANNUAL_ROUTINE_AUTO_MANUAL,_T("Auto"));
			m_annualr_list.SetCellEnabled(i,ANNUAL_ROUTINE_VALUE,0);
		}
		else
		{
			m_annualr_list.SetItemText(i,ANNUAL_ROUTINE_AUTO_MANUAL,_T("Manual"));
			m_annualr_list.SetCellEnabled(i,ANNUAL_ROUTINE_VALUE,1);
		}


		if(m_Annual_data.at(i).value==0)
			m_annualr_list.SetItemText(i,ANNUAL_ROUTINE_VALUE,_T("OFF"));
		else
			m_annualr_list.SetItemText(i,ANNUAL_ROUTINE_VALUE,_T("ON"));

	

		CString temp_des2;
		MultiByteToWideChar( CP_ACP, 0, (char *)m_Annual_data.at(i).label, (int)strlen((char *)m_Annual_data.at(i).label)+1, 
			temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des2.ReleaseBuffer();
		m_annualr_list.SetItemText(i,ANNUAL_ROUTINE_LABLE,temp_des2);

	}
	copy_data_to_ptrpanel(TYPE_WEEKLY);
	//MessageBox("1");
	return 0;
}


LRESULT BacnetAnnualRoutine::Fresh_Annual_Routine_Item(WPARAM wParam,LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	CString temp_task_info;
	CString New_CString =  m_annualr_list.GetItemText(Changed_Item,Changed_SubItem);

	if(Changed_SubItem == ANNUAL_ROUTINE_LABLE)
	{
		CString cs_temp = m_annualr_list.GetItemText(Changed_Item,Changed_SubItem);
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Annual_data.at(Changed_Item).label,STR_ANNUAL_LABEL_LENGTH,cTemp1,STR_ANNUAL_LABEL_LENGTH);
	}

	if(Changed_SubItem == ANNUAL_ROUTINE_FULL_LABEL)
	{
		CString cs_temp = m_annualr_list.GetItemText(Changed_Item,Changed_SubItem);
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Annual_data.at(Changed_Item).description,STR_ANNUAL_DESCRIPTION_LENGTH,cTemp1,STR_ANNUAL_DESCRIPTION_LENGTH);
	}

	if(Changed_SubItem == ANNUAL_ROUTINE_VALUE)
	{
		CString cs_temp=m_annualr_list.GetItemText(Changed_Item,ANNUAL_ROUTINE_VALUE);
		if(cs_temp.CompareNoCase(_T("OFF"))==0)
		{
			m_Annual_data.at(Changed_Item).value=0;
		}
		else
		{
			m_Annual_data.at(Changed_Item).value=1;
		}
	}
	if(Changed_SubItem == ANNUAL_ROUTINE_AUTO_MANUAL)
	{
		CString cs_temp=m_annualr_list.GetItemText(Changed_Item,ANNUAL_ROUTINE_AUTO_MANUAL);
		if(cs_temp.CompareNoCase(_T("Auto"))==0)
		{
			m_Annual_data.at(Changed_Item).auto_manual=0;
			m_annualr_list.SetCellEnabled(Changed_Item,ANNUAL_ROUTINE_VALUE,0);
		}
		else
		{
			m_Annual_data.at(Changed_Item).auto_manual=1;
			m_annualr_list.SetCellEnabled(Changed_Item,ANNUAL_ROUTINE_VALUE,1);
		}
	}
	temp_task_info.Format(_T("Write Annual Routine List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
	Post_Write_Message(bac_gloab_device_id,WRITEANNUALROUTINE_T3000,Changed_Item,Changed_Item,sizeof(Str_annual_routine_point),BacNet_hwd ,temp_task_info);

	return 0;
}

