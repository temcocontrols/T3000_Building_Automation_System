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
Str_annual_routine_point m_temp_annual_data[BAC_ANNUAL_ROUTINES_COUNT];
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
	ON_MESSAGE(MY_RESUME_DATA, AnnualMessageCallBack)
	ON_MESSAGE(WM_HOTKEY,&BacnetAnnualRoutine::OnHotKey)//快捷键消息映射手动加入;
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_BAC_ANNULE_LIST, &BacnetAnnualRoutine::OnNMClickListBacAnnuleList)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Annual_Routine_Item)
	ON_MESSAGE(WM_REFRESH_BAC_ANNUAL_LIST,Fresh_Annual_Routine_List)
	ON_BN_CLICKED(IDC_BUTTON_ANNUAL_EDIT, &BacnetAnnualRoutine::OnBnClickedButtonAnnualEdit)
//	ON_MESSAGE(MY_RESUME_DATA, AnnualResumeMessageCallBack)
END_MESSAGE_MAP()


// BacnetAnnualRoutine message handlers


LRESULT  BacnetAnnualRoutine::AnnualMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	CString Show_Results;
	CString temp_cs = pInvoke->task_info;
	if(msg_result)
	{
		Show_Results = temp_cs + _T("Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		//MessageBox(_T("Bacnet operation success!"));
	}
	else
	{
		memcpy_s(&m_Annual_data.at(pInvoke->mRow),sizeof(Str_annual_routine_point),&m_temp_annual_data[pInvoke->mRow],sizeof(Str_annual_routine_point));//还原没有改对的值;
		PostMessage(WM_REFRESH_BAC_ANNUAL_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		//AfxMessageBox(Show_Results);
		//MessageBox(_T("Bacnet operation fail!"));
	}
	

	if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
		m_annualr_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_annualr_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
	m_annualr_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);

	if(pInvoke)
		delete pInvoke;
	return 0;
}


BOOL BacnetAnnualRoutine::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		CRect list_rect,win_rect;
		m_annualr_list.GetWindowRect(list_rect);
		ScreenToClient(&list_rect);
		::GetWindowRect(m_annual_dlg_hwnd,win_rect);
		m_annualr_list.Set_My_WindowRect(win_rect);
		m_annualr_list.Set_My_ListRect(list_rect);
		m_annualr_list.Get_clicked_mouse_position();
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//const int KEY_INSERT = 1020;
LRESULT BacnetAnnualRoutine::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	if (wParam==KEY_INSERT)
	{
		OnBnClickedButtonAnnualEdit();
		//AnnualRout_InsertDia Dlg;
		//Dlg.DoModal();
	}
	return 0;
}



BOOL BacnetAnnualRoutine::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	Initial_List();
//	RegisterHotKey(GetSafeHwnd(),KEY_INSERT,NULL,VK_INSERT);
	PostMessage(WM_REFRESH_BAC_ANNUAL_LIST,NULL,NULL);
	SetTimer(1,BAC_LIST_REFRESH_TIME,NULL);
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

	CRect list_rect,win_rect;
	m_annualr_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_annual_dlg_hwnd,win_rect);
	m_annualr_list.Set_My_WindowRect(win_rect);
	m_annualr_list.Set_My_ListRect(list_rect);

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

		for (int x=0;x<ANNUAL_COL_NUMBER;x++)
		{
			if((i%2)==0)
				m_annualr_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_annualr_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}

	}
	m_annualr_list.SetCellChecked(0,0,1);// default will choose the first one

}


void BacnetAnnualRoutine::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	UnregisterHotKey(GetSafeHwnd(),KEY_INSERT);
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	KillTimer(1);
	m_annual_dlg_hwnd = NULL;
	CDialogEx::OnClose();
}

void BacnetAnnualRoutine::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	//KillTimer(1);
	//m_annual_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	//CDialogEx::OnCancel();
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
	int Fresh_Item;
	int isFreshOne = (int)lParam;
	if(isFreshOne == REFRESH_ON_ITEM)
	{
		Fresh_Item = (int)wParam;
	}
	else
	{
		if(m_annualr_list.IsDataNewer((char *)&m_Annual_data.at(0),sizeof(Str_annual_routine_point) * BAC_ANNUAL_ROUTINES_COUNT))
		{
			//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
			m_annualr_list.SetListData((char *)&m_Annual_data.at(0),sizeof(Str_annual_routine_point) * BAC_ANNUAL_ROUTINES_COUNT);
		}
		else
		{
			return 0;
		}
	}


	for (int i=0;i<(int)m_Annual_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;

		if(isFreshOne)
		{
			i = Fresh_Item;
		}

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

		if(isFreshOne)
		{
			break;
		}
	}
	copy_data_to_ptrpanel(TYPE_WEEKLY);
	//MessageBox("1");
	return 0;
}


LRESULT BacnetAnnualRoutine::Fresh_Annual_Routine_Item(WPARAM wParam,LPARAM lParam)
{
	int cmp_ret ;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	CString temp_task_info;
	CString New_CString =  m_annualr_list.GetItemText(Changed_Item,Changed_SubItem);

	//先保存 原来的值，等结束的时候来比对，看是否有改变，有改变就进行写动作;
	memcpy_s(&m_temp_annual_data[Changed_Item],sizeof(Str_annual_routine_point),&m_Annual_data.at(Changed_Item),sizeof(Str_annual_routine_point));

	if(Changed_SubItem == ANNUAL_ROUTINE_LABLE)
	{
		CString cs_temp = m_annualr_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= STR_ANNUAL_LABEL_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not higher than 8"),_T("Warning"));
			PostMessage(WM_REFRESH_BAC_ANNUAL_LIST,NULL,NULL);
			return 0;
		}

		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Annual_data.at(Changed_Item).label,STR_ANNUAL_LABEL_LENGTH,cTemp1,STR_ANNUAL_LABEL_LENGTH);
	}

	if(Changed_SubItem == ANNUAL_ROUTINE_FULL_LABEL)
	{
		CString cs_temp = m_annualr_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= STR_ANNUAL_DESCRIPTION_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not higher than 20"),_T("Warning"));
			PostMessage(WM_REFRESH_BAC_ANNUAL_LIST,NULL,NULL);
			return 0;
		}

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
	cmp_ret = memcmp(&m_temp_annual_data[Changed_Item],&m_Annual_data.at(Changed_Item),sizeof(Str_annual_routine_point));
	if(cmp_ret!=0)
	{
		m_annualr_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
	temp_task_info.Format(_T("Write Annual Routine List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
	Post_Write_Message(g_bac_instance,WRITEANNUALROUTINE_T3000,Changed_Item,Changed_Item,sizeof(Str_annual_routine_point),m_annual_dlg_hwnd ,temp_task_info,Changed_Item,Changed_SubItem);
	}
	return 0;
}

void BacnetAnnualRoutine::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(this->IsWindowVisible())
	{
	PostMessage(WM_REFRESH_BAC_ANNUAL_LIST,NULL,NULL);
	Post_Refresh_Message(g_bac_instance,READANNUALROUTINE_T3000,0,BAC_ANNUAL_ROUTINES_COUNT - 1,sizeof(Str_annual_routine_point),BAC_ANNUAL_GROUP);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void BacnetAnnualRoutine::OnBnClickedButtonAnnualEdit()
{
	// TODO: Add your control notification handler code here
	for (int i=0;i<m_annualr_list.GetItemCount();++i)
	{
		if(m_annualr_list.GetCellChecked(i,0))
		{
			annual_list_line = i;
			break;
		}
	}

	::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ANNUALCODE);

}

void BacnetAnnualRoutine::Reg_Hotkey()
{
	RegisterHotKey(GetSafeHwnd(),KEY_INSERT,NULL,VK_INSERT);//Insert键
}

void BacnetAnnualRoutine::Unreg_Hotkey()
{
	UnregisterHotKey(GetSafeHwnd(),KEY_INSERT);
}

