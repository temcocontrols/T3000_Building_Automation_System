// BacnetProgram.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetProgram.h"
#include "afxdialogex.h"

#include "afxdialogex.h"
#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "globle_function.h"
#include "BacnetProgramEdit.h"

#include "gloab_define.h"
extern void copy_data_to_ptrpanel(int Data_type);//Used for copy the structure to the ptrpanel.
Str_program_point m_temp_program_data[BAC_PROGRAM_ITEM_COUNT];

// CBacnetProgram dialog

IMPLEMENT_DYNAMIC(CBacnetProgram, CDialogEx)

CBacnetProgram::CBacnetProgram(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetProgram::IDD, pParent)
{
	program_list_line = 0;
}

CBacnetProgram::~CBacnetProgram()
{
}

void CBacnetProgram::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROGRAM, m_program_list);
}


BEGIN_MESSAGE_MAP(CBacnetProgram, CDialogEx)
	ON_MESSAGE(MY_RESUME_DATA, ProgramMessageCallBack)
	ON_MESSAGE(WM_HOTKEY,&CBacnetProgram::OnHotKey)//快捷键消息映射手动加入
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Program_Item)
	ON_MESSAGE(WM_REFRESH_BAC_PROGRAM_LIST,Fresh_Program_List)
	ON_BN_CLICKED(IDC_BUTTON_PROGRAM_READ, &CBacnetProgram::OnBnClickedButtonProgramRead)
	ON_BN_CLICKED(IDC_BUTTON_PROGRAM_APPLY, &CBacnetProgram::OnBnClickedButtonProgramApply)
	ON_BN_CLICKED(IDC_BUTTON_PROGRAM_EDIT, &CBacnetProgram::OnBnClickedButtonProgramEdit)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PROGRAM, &CBacnetProgram::OnNMClickListProgram)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CBacnetProgram message handlers

LRESULT  CBacnetProgram::ProgramMessageCallBack(WPARAM wParam, LPARAM lParam)
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
		memcpy_s(&m_Program_data.at(pInvoke->mRow),sizeof(Str_program_point),&m_temp_program_data[pInvoke->mRow],sizeof(Str_program_point));//还原没有改对的值
		PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		//AfxMessageBox(Show_Results);
		//MessageBox(_T("Bacnet operation fail!"));
	}
	if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
		m_program_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_program_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
	m_program_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);


	if(pInvoke)
		delete pInvoke;
	return 0;
}


// CBacnetProgramEdit message handlers
LRESULT CBacnetProgram::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	if (wParam==KEY_INSERT)
	{
		OnBnClickedButtonProgramEdit();
	}
	return 0;
}


BOOL CBacnetProgram::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Initial_List();

	PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
	// TODO:  Add extra initialization here

	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_REFRESH);
	((CButton *)GetDlgItem(IDC_BUTTON_PROGRAM_READ))->SetIcon(hIcon);	
	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_OK);
	((CButton *)GetDlgItem(IDC_BUTTON_PROGRAM_APPLY))->SetIcon(hIcon);

	//RegisterHotKey(GetSafeHwnd(),KEY_INSERT,NULL,VK_INSERT);//Insert键
	SetTimer(1,BAC_LIST_REFRESH_TIME,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBacnetProgram::Reg_Hotkey()
{
	RegisterHotKey(GetSafeHwnd(),KEY_INSERT,NULL,VK_INSERT);//Insert键
}

void CBacnetProgram::Unreg_Hotkey()
{
	UnregisterHotKey(GetSafeHwnd(),KEY_INSERT);
}

void CBacnetProgram::Initial_List()
{
	m_program_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	//m_program_list.SetExtendedStyle(m_program_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_program_list.SetExtendedStyle(m_program_list.GetExtendedStyle() |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_program_list.InsertColumn(0, _T("Num"), 80, ListCtrlEx::CheckBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_program_list.InsertColumn(1, _T("Full Label"), 150, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_program_list.InsertColumn(2, _T("Status"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_program_list.InsertColumn(3, _T("Auto/Manual"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_program_list.InsertColumn(4, _T("Size"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_program_list.InsertColumn(5, _T("Run Status"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_program_list.InsertColumn(6, _T("Label"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_pragram_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_pragram_dlg_hwnd;

	CRect list_rect,win_rect;
	m_program_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_pragram_dlg_hwnd,win_rect);
	m_program_list.Set_My_WindowRect(win_rect);
	m_program_list.Set_My_ListRect(list_rect);

	m_program_list.DeleteAllItems();
	for (int i=0;i<(int)m_Program_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		temp_item.Format(_T("%d"),i+1);
		m_program_list.InsertItem(i,temp_item);
		m_program_list.SetCellEnabled(i,0,0);
		m_program_list.SetCellChecked(0,0,1);// default will set the first one checked
		if(ListCtrlEx::ComboBox == m_program_list.GetColumnType(PROGRAM_AUTO_MANUAL))
		{
			ListCtrlEx::CStrList strlist;
			strlist.push_back(Auto_Manual[0]);
			strlist.push_back(Auto_Manual[1]);
			m_program_list.SetCellStringList(i, PROGRAM_AUTO_MANUAL, strlist);
		}

		if(ListCtrlEx::ComboBox == m_program_list.GetColumnType(PROGRAM_STATUS))
		{
			ListCtrlEx::CStrList strlist;
			strlist.push_back(ProgramStatus[0]);
			strlist.push_back(ProgramStatus[1]);
			m_program_list.SetCellStringList(i, PROGRAM_STATUS, strlist);
		}

		for (int x=0;x<PROGRAM_COL_NUMBER;x++)
		{
			if((i%2)==0)
				m_program_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_program_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}

	}

	m_program_list.SetCellChecked(0,0,1);
}


void CBacnetProgram::OnBnClickedButtonProgramRead()
{
	// TODO: Add your control notification handler code here
	Post_Refresh_Message(g_bac_instance,READPROGRAM_T3000,0,BAC_PROGRAM_ITEM_COUNT - 1,sizeof(Str_program_point),BAC_PROGRAM_GROUP);
	PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
}

LRESULT CBacnetProgram::Fresh_Program_Item(WPARAM wParam,LPARAM lParam)
{
	int cmp_ret ;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	memcpy_s(&m_temp_program_data[Changed_Item],sizeof(Str_program_point),&m_Program_data.at(Changed_Item),sizeof(Str_program_point));


	CString temp_task_info;
	CString New_CString =  m_program_list.GetItemText(Changed_Item,Changed_SubItem);


	if(Changed_SubItem == PROGRAM_LABEL)
	{
		CString cs_temp = m_program_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= STR_PROGRAM_LABEL_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not higher than 8"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
			return 0;
		}
		cs_temp.MakeUpper();
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Program_data.at(Changed_Item).label,STR_PROGRAM_LABEL_LENGTH,cTemp1,STR_PROGRAM_LABEL_LENGTH);
	}

	if(Changed_SubItem == PROGRAM_FULL_LABLE)
	{
		CString cs_temp = m_program_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= STR_PROGRAM_DESCRIPTION_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not higher than 20"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
			return 0;
		}
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Program_data.at(Changed_Item).description,STR_PROGRAM_DESCRIPTION_LENGTH,cTemp1,STR_PROGRAM_DESCRIPTION_LENGTH);
	}

	if(Changed_SubItem == PROGRAM_STATUS)
	{
		CString cs_temp=m_program_list.GetItemText(Changed_Item,PROGRAM_STATUS);
		if(cs_temp.CompareNoCase(_T("OFF"))==0)
		{
			m_Program_data.at(Changed_Item).on_off=0;
		}
		else
		{
			m_Program_data.at(Changed_Item).on_off=1;
		}
	}
	if(Changed_SubItem == PROGRAM_AUTO_MANUAL)
	{
		CString cs_temp=m_program_list.GetItemText(Changed_Item,PROGRAM_AUTO_MANUAL);
		if(cs_temp.CompareNoCase(_T("Auto"))==0)
		{
			m_Program_data.at(Changed_Item).auto_manual=0;
		}
		else
		{
			m_Program_data.at(Changed_Item).auto_manual=1;
		}
	}
	cmp_ret = memcmp(&m_temp_program_data[Changed_Item],&m_Program_data.at(Changed_Item),sizeof(Str_program_point));
	if(cmp_ret!=0)
	{
		m_program_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Program List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITEPROGRAM_T3000,Changed_Item,Changed_Item,sizeof(Str_program_point),m_pragram_dlg_hwnd ,temp_task_info,Changed_Item,Changed_SubItem);
	}

	return 0;
}

LRESULT CBacnetProgram::Fresh_Program_List(WPARAM wParam,LPARAM lParam)
{
	// Str_in_point Get_Str_in_Point(int index);
	int Fresh_Item;
	int isFreshOne = (int)lParam;
	if(isFreshOne == REFRESH_ON_ITEM)
	{
		Fresh_Item = (int)wParam;
	}
	else
	{
		if(m_program_list.IsDataNewer((char *)&m_Program_data.at(0),sizeof(Str_program_point) * BAC_PROGRAM_ITEM_COUNT))
		{
			//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
			m_program_list.SetListData((char *)&m_Program_data.at(0),sizeof(Str_program_point) * BAC_PROGRAM_ITEM_COUNT);
		}
		else
		{
			return 0;
		}
	}



	bac_program_pool_size = 26624;
	bac_program_size = 0;
	bac_free_memory = 26624;
	for (int i=0;i<(int)m_Program_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		
		if(isFreshOne)
		{
			i = Fresh_Item;
		}

		MultiByteToWideChar( CP_ACP, 0, (char *)m_Program_data.at(i).description, (int)strlen((char *)m_Program_data.at(i).description)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();


		m_program_list.SetItemText(i,PROGRAM_FULL_LABLE,temp_des);


		if(m_Program_data.at(i).auto_manual==0)
			m_program_list.SetItemText(i,PROGRAM_AUTO_MANUAL,_T("Auto"));
		else
			m_program_list.SetItemText(i,PROGRAM_AUTO_MANUAL,_T("Manual"));

		if(m_Program_data.at(i).on_off==0)
			m_program_list.SetItemText(i,PROGRAM_STATUS,ProgramStatus[1]);
		else
			m_program_list.SetItemText(i,PROGRAM_STATUS,ProgramStatus[0]);

		temp_value.Format(_T("%d"),m_Program_data.at(i).bytes);
		m_program_list.SetItemText(i,PROGRAM_SIZE_LIST,temp_value);

		bac_program_size = bac_program_size + m_Program_data.at(i).bytes;

		if(m_Program_data.at(i).com_prg==0)
			m_program_list.SetItemText(i,PROGRAM_RUN_STATUS,_T("Normal"));
		else
			m_program_list.SetItemText(i,PROGRAM_RUN_STATUS,_T("Abnormal"));


		CString temp_des2;
		MultiByteToWideChar( CP_ACP, 0, (char *)m_Program_data.at(i).label, (int)strlen((char *)m_Program_data.at(i).label)+1, 
			temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des2.ReleaseBuffer();

		m_program_list.SetItemText(i,PROGRAM_LABEL,temp_des2);

		if(isFreshOne)
		{
			break;
		}

	}
	bac_free_memory = bac_program_pool_size - bac_program_size;
	copy_data_to_ptrpanel(TYPE_PROGRAM);
	return 0;
}


void CBacnetProgram::OnBnClickedButtonProgramApply()
{
	// TODO: Add your control notification handler code here
	for (int i=0;i<(int)m_Program_data.size();i++)
	{
		CString cs_temp=m_program_list.GetItemText(i,PROGRAM_FULL_LABLE);
		char cTemp[255];
		memset(cTemp,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp, 255, NULL, NULL );
		memcpy_s(m_Program_data.at(i).description,STR_PROGRAM_DESCRIPTION_LENGTH,cTemp,STR_PROGRAM_DESCRIPTION_LENGTH);

		cs_temp=m_program_list.GetItemText(i,PROGRAM_STATUS);
		if(cs_temp.CompareNoCase(_T("OFF"))==0)
		{
			m_Program_data.at(i).on_off=0;
		}
		else
		{
			m_Program_data.at(i).on_off=1;
		}

		cs_temp=m_program_list.GetItemText(i,PROGRAM_AUTO_MANUAL);
		if(cs_temp.CompareNoCase(_T("Auto"))==0)
		{
			m_Program_data.at(i).auto_manual=0;
		}
		else
		{
			m_Program_data.at(i).auto_manual=1;
		}

		cs_temp=m_program_list.GetItemText(i,PROGRAM_LABEL);
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Program_data.at(i).label,STR_PROGRAM_LABEL_LENGTH,cTemp1,STR_PROGRAM_LABEL_LENGTH);


	}
	Post_Write_Message(g_bac_instance,WRITEPROGRAM_T3000,0,15,sizeof(Str_program_point),BacNet_hwd);

}

//LRESULT  CBacnetProgram::ProgramResumeMessageCallBack(WPARAM wParam, LPARAM lParam)
//{
//	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
//	CString temp_cs = pInvoke->task_info;
//	bool msg_result=WRITE_FAIL;
//	msg_result = MKBOOL(wParam);
//	CString Show_Results;
//	if(msg_result)
//	{
//		//CString temp_ok;
//		//temp_ok = _T("Bacnet operation success!   Request ID:") +  temp_cs;
//
//		Show_Results = temp_cs + _T("Success!");
//		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
//
//		//SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_ok);
//#ifdef SHOW_MESSAGEBOX
//		MessageBox(Show_Results);
//#endif
//	}
//	else
//	{
//		Show_Results = temp_cs + _T("Fail!");
//		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
//#ifdef SHOW_ERROR_MESSAGE
//		MessageBox(Show_Results);
//#endif
//	}
//	if(pInvoke)
//		delete pInvoke;
//	return 0;
//}


void CBacnetProgram::OnBnClickedButtonProgramEdit()
{
	// TODO: Add your control notification handler code here
	for (int i=0;i<m_program_list.GetItemCount();++i)
	{
		if(m_program_list.GetCellChecked(i,0))
		{
			program_list_line = i;
			break;
		}
	}

#if 0
	int	resend_count = 0;
	do 
	{
		resend_count ++;
		if(resend_count>RESEND_COUNT)
			return;
		if(m_Program_data.at(program_list_line).bytes <450)
			g_invoke_id = GetPrivateData(g_bac_instance,READPROGRAMCODE_T3000,program_list_line,program_list_line,m_Program_data.at(program_list_line).bytes + 10);
			//g_invoke_id = GetPrivateData(g_bac_instance,READPROGRAMCODE_T3000,program_list_line,program_list_line,200);
		else
		{
			g_invoke_id = GetPrivateData(g_bac_instance,READPROGRAMCODE_T3000,program_list_line,program_list_line, 10);
			TRACE("m_Program_data.at(program_list_line).bytes = %d\r\n",m_Program_data.at(program_list_line).bytes);
		}
		Sleep(200);
	} while (g_invoke_id<0);



	if(g_invoke_id>=0)
	{
		CString temp_cs;
		temp_cs.Format(_T("Task ID = %d. Read program code from item %d "),g_invoke_id,program_list_line);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd,temp_cs);
	}
	KillTimer(1);
	this->ShowWindow(0);
	CBacnetProgramEdit dlg;
	dlg.DoModal();
	this->ShowWindow(1);
	SetTimer(1,BAC_LIST_REFRESH_TIME,NULL);
#endif
	::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,MENU_CLICK,TYPE_PROGRAMCODE);

	//PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
		//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd);
	
	//OnBnClickedButtonProgramRead();
}



void CBacnetProgram::OnNMClickListProgram(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_program_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_program_list.SubItemHitTest(&lvinfo);
	if(nItem!=-1)
	{
		m_program_list.SetCellChecked(nItem,0,1);
		program_list_line = nItem;
		for (int i=0;i<m_program_list.GetItemCount();++i)
		{
			if(i == nItem)
				continue;
			m_program_list.SetCellChecked(i,0,FALSE);
		}
	}


	// TODO: Add your control notification handler code here


	*pResult = 0;
}


void CBacnetProgram::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	UnregisterHotKey(GetSafeHwnd(),KEY_INSERT);
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	CDialogEx::OnClose();
}
void CBacnetProgram::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	//CDialogEx::OnCancel();
}

void CBacnetProgram::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(this->IsWindowVisible())
	{
	PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
	if(bac_select_device_online)
		Post_Refresh_Message(g_bac_instance,READPROGRAM_T3000,0,BAC_PROGRAM_ITEM_COUNT - 1,sizeof(Str_program_point),BAC_PROGRAM_GROUP);
	}
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CBacnetProgram::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		CRect list_rect,win_rect;
		m_program_list.GetWindowRect(list_rect);
		ScreenToClient(&list_rect);
		::GetWindowRect(m_pragram_dlg_hwnd,win_rect);
		m_program_list.Set_My_WindowRect(win_rect);
		m_program_list.Set_My_ListRect(list_rect);

		m_program_list.Get_clicked_mouse_position();
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}



