// BacnetVariable.cpp : implementation file
// BacnetVariable.cpp Add by Fance 2013 08 21

#include "stdafx.h"
#include "T3000.h"
#include "BacnetVariable.h"
#include "afxdialogex.h"

#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "globle_function.h"
#include "gloab_define.h"
#include "BacnetRange.h"
extern void copy_data_to_ptrpanel(int Data_type);//Used for copy the structure to the ptrpanel.
Str_variable_point m_temp_variable_data[BAC_VARIABLE_ITEM_COUNT];
IMPLEMENT_DYNAMIC(CBacnetVariable, CDialogEx)

CBacnetVariable::CBacnetVariable(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetVariable::IDD, pParent)
{

}

CBacnetVariable::~CBacnetVariable()
{
}

void CBacnetVariable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VARIABLE, m_variable_list);
	DDX_Control(pDX, IDC_DATETIMEPICKER2_VARIABLE, m_variable_time_picker);
}


BEGIN_MESSAGE_MAP(CBacnetVariable, CDialogEx)
	ON_MESSAGE(MY_RESUME_DATA, VariableMessageCallBack)
	ON_BN_CLICKED(IDC_BUTTON_VARIABLE_READ, &CBacnetVariable::OnBnClickedButtonVariableRead)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Variable_Item)
//	ON_MESSAGE(MY_RESUME_DATA, VariableResumeMessageCallBack)
	ON_MESSAGE(WM_REFRESH_BAC_VARIABLE_LIST,Fresh_Variable_List)
	ON_BN_CLICKED(IDC_BUTTON_VARIABLE_APPLY, &CBacnetVariable::OnBnClickedButtonVariableApply)
	ON_NOTIFY(NM_CLICK, IDC_LIST_VARIABLE, &CBacnetVariable::OnNMClickListVariable)
	ON_WM_TIMER()
	ON_NOTIFY(NM_KILLFOCUS, IDC_DATETIMEPICKER2_VARIABLE, &CBacnetVariable::OnNMKillfocusDatetimepicker2Variable)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CBacnetVariable message handlers
LRESULT  CBacnetVariable::VariableMessageCallBack(WPARAM wParam, LPARAM lParam)
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
		memcpy_s(&m_Variable_data.at(pInvoke->mRow),sizeof(Str_variable_point),&m_temp_variable_data[pInvoke->mRow],sizeof(Str_variable_point));//还原没有改对的值
		PostMessage(WM_REFRESH_BAC_VARIABLE_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		//AfxMessageBox(Show_Results);
		//MessageBox(_T("Bacnet operation fail!"));
	}
	if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
		m_variable_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_variable_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
	m_variable_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);

	if(pInvoke)
		delete pInvoke;
	return 0;
}


BOOL CBacnetVariable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	Initial_List();	//Initial the list of Variable,read from device;
	PostMessage(WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);

	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_REFRESH);
	((CButton *)GetDlgItem(IDC_BUTTON_VARIABLE_READ))->SetIcon(hIcon);	
	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_OK);
	((CButton *)GetDlgItem(IDC_BUTTON_VARIABLE_APPLY))->SetIcon(hIcon);
	SetTimer(1,BAC_LIST_REFRESH_TIME,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CBacnetVariable::OnBnClickedButtonVariableRead()
{
	// TODO: Add your control notification handler code here

	PostMessage(WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
}


void CBacnetVariable::Initial_List()
{
	m_variable_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_variable_list.SetExtendedStyle(m_variable_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_variable_list.InsertColumn(VARIABLE_NUM, _T("NUM"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_variable_list.InsertColumn(VARIABLE_FULL_LABLE, _T("Full Label"), 200, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_variable_list.InsertColumn(VARIABLE_AUTO_MANUAL, _T("Auto/Manual"), 150, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_variable_list.InsertColumn(VARIABLE_VALUE, _T("Value"), 120, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_variable_list.InsertColumn(VARIABLE_UNITE, _T("Units"), 120, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_variable_list.InsertColumn(VARIABLE_LABLE, _T("Label"), 130, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_variable_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_variable_dlg_hwnd;

	CRect list_rect,win_rect;
	m_variable_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_input_dlg_hwnd,win_rect);
	m_variable_list.Set_My_WindowRect(win_rect);
	m_variable_list.Set_My_ListRect(list_rect);

	CTime TimeTemp(2014,1,1,0,0,0);
	m_variable_time_picker.SetFormat(_T("HH:mm"));
	m_variable_time_picker.SetTime(&TimeTemp);
	m_variable_time_picker.ShowWindow(SW_HIDE);

	m_variable_list.DeleteAllItems();
	for (int i=0;i<(int)m_Variable_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		temp_item.Format(_T("%d"),i+1);
		m_variable_list.InsertItem(i,temp_item);
		//if(ListCtrlEx::ComboBox == m_variable_list.GetColumnType(VARIABLE_AUTO_MANUAL))
		//{
		//	ListCtrlEx::CStrList strlist;
		//	strlist.push_back(_T("Auto"));
		//	strlist.push_back(_T("Manual"));
		//	m_variable_list.SetCellStringList(i, VARIABLE_AUTO_MANUAL, strlist);
		//}

		if(ListCtrlEx::ComboBox == m_variable_list.GetColumnType(VARIABLE_UNITE))
		{
			ListCtrlEx::CStrList strlist;

			for (int i=0;i<(int)sizeof(Units_Type)/sizeof(Units_Type[0]);i++)
			{
				strlist.push_back(Units_Type[i]);
			}
			m_variable_list.SetCellStringList(i, VARIABLE_UNITE, strlist);		
		}

		for (int x=0;x<VARIABLE_COL_NUMBER;x++)
		{
			if((i%2)==0)
				m_variable_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_variable_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}

	}
}


LRESULT CBacnetVariable::Fresh_Variable_List(WPARAM wParam,LPARAM lParam)
{

	int Fresh_Item;
	int isFreshOne = (int)lParam;
	if(isFreshOne == REFRESH_ON_ITEM)
	{
		Fresh_Item = (int)wParam;
	}
	else
	{
		if(m_variable_list.IsDataNewer((char *)&m_Variable_data.at(0),sizeof(Str_variable_point) * BAC_VARIABLE_ITEM_COUNT))
		{
			//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
			m_variable_list.SetListData((char *)&m_Variable_data.at(0),sizeof(Str_variable_point) * BAC_VARIABLE_ITEM_COUNT);
		}
		else
		{
			return 0;
		}
	}


	for (int i=0;i<(int)m_Variable_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;

		if(isFreshOne)
		{
			i = Fresh_Item;
		}

		MultiByteToWideChar( CP_ACP, 0, (char *)m_Variable_data.at(i).description, (int)strlen((char *)m_Variable_data.at(i).description)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();
		m_variable_list.SetItemText(i,VARIABLE_FULL_LABLE,temp_des);
		if(m_Variable_data.at(i).auto_manual==0)
		{
			m_variable_list.SetItemText(i,VARIABLE_AUTO_MANUAL,_T("Auto"));
			m_variable_list.SetCellEnabled(i,VARIABLE_VALUE,0);
		}
		else
		{
			m_variable_list.SetItemText(i,VARIABLE_AUTO_MANUAL,_T("Manual"));
			m_variable_list.SetCellEnabled(i,VARIABLE_VALUE,1);
		}


		if(m_Variable_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
		{
			

			if(m_Variable_data.at(i).range>30)
			{
				//m_variable_list.SetItemText(i,VARIABLE_UNITE,Digital_Units_Array[0]);
				m_variable_list.SetItemText(i,VARIABLE_UNITE,_T("Out of range"));
			}
			else
			{
				CString temp1;
				CStringArray temparray;

				if((m_Variable_data.at(i).range < 23) &&(m_Variable_data.at(i).range !=0))
					temp1 = Digital_Units_Array[m_Variable_data.at(i).range];
				else if((m_Variable_data.at(i).range >=23) && (m_Variable_data.at(i).range <= 30))
				{
					if(receive_customer_unit)
						temp1 = temp_unit_no_index[m_Variable_data.at(i).range - 23];
				}

				SplitCStringA(temparray,temp1,_T("/"));
				if((temparray.GetSize()==2))
				{
					if(m_Variable_data.at(i).control == 0)
						m_variable_list.SetItemText(i,VARIABLE_VALUE,temparray.GetAt(0));
					else
						m_variable_list.SetItemText(i,VARIABLE_VALUE,temparray.GetAt(1));
					m_variable_list.SetItemText(i,VARIABLE_UNITE,temp1);
				}
				
			}
#if 0
			m_variable_list.SetItemText(i,VARIABLE_UNITE,Digital_Units_Array[m_Variable_data.at(i).range]);	//单位 这个要商量 看要怎么搞;
			if((m_Variable_data.at(i).range >= 12)&&(m_Variable_data.at(i).range <= 22))
			{
				CString temp1;
				CStringArray temparray;
				temp1 = Digital_Units_Array[m_Variable_data.at(i).range - 11];//11 is the sizeof the array
				SplitCStringA(temparray,temp1,_T("/"));
				if((temparray.GetSize()==2)&&(!temparray.GetAt(1).IsEmpty()))
				{
					m_variable_list.SetItemText(i,VARIABLE_VALUE,temparray.GetAt(1));
				}
				m_variable_list.SetItemText(i,VARIABLE_UNITE,temp1);
			}
			else if((m_Variable_data.at(i).range >= 1)&&(m_Variable_data.at(i).range <= 11))
			{
				CString temp1;
				CStringArray temparray;
				temp1 = Digital_Units_Array[m_Variable_data.at(i).range];
				SplitCStringA(temparray,temp1,_T("/"));
				if((temparray.GetSize()==2)&&(!temparray.GetAt(0).IsEmpty()))
				{
					m_variable_list.SetItemText(i,VARIABLE_VALUE,temparray.GetAt(0));
				}
				m_variable_list.SetItemText(i,VARIABLE_UNITE,temp1);
			}
			else
			{
				m_variable_list.SetItemText(i,VARIABLE_VALUE,_T("0"));
			}
#endif
		}
		else
		{
			if(m_Variable_data.at(i).range == 20)	//如果是时间;
			{
				m_variable_list.SetItemText(i,VARIABLE_UNITE,Variable_Analog_Units_Array[m_Variable_data.at(i).range]);
				char temp_char[50];
				int time_seconds = m_Variable_data.at(i).value / 1000;
				intervaltotext(temp_char,time_seconds,0,0);
				CString temp_11;
				MultiByteToWideChar( CP_ACP, 0, temp_char, strlen(temp_char) + 1, 
					temp_11.GetBuffer(MAX_PATH), MAX_PATH );
				temp_11.ReleaseBuffer();		
				m_variable_list.SetItemText(i,VARIABLE_VALUE,temp_11);

				//temp_value.Format(_T("%d"),m_Variable_data.at(i).value);
				//m_variable_list.SetItemText(i,VARIABLE_VALUE,temp_value);
			}
			else if(m_Variable_data.at(i).range<=sizeof(Variable_Analog_Units_Array)/sizeof(Variable_Analog_Units_Array[0]))
			{
			m_variable_list.SetItemText(i,VARIABLE_UNITE,Variable_Analog_Units_Array[m_Variable_data.at(i).range]);

			CString cstemp_value;
			float temp_float_value;
			temp_float_value = ((float)m_Variable_data.at(i).value) / 1000;
			cstemp_value.Format(_T("%.3f"),temp_float_value);
			m_variable_list.SetItemText(i,VARIABLE_VALUE,cstemp_value);

			//temp_value.Format(_T("%d"),m_Variable_data.at(i).value);
			//m_variable_list.SetItemText(i,VARIABLE_VALUE,temp_value);
			}
		}
		
		CString temp_des2;
		MultiByteToWideChar( CP_ACP, 0, (char *)m_Variable_data.at(i).label, (int)strlen((char *)m_Variable_data.at(i).label)+1, 
			temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des2.ReleaseBuffer();
		m_variable_list.SetItemText(i,VARIABLE_LABLE,temp_des2);


		if(isFreshOne)
		{
			break;
		}

	}
	copy_data_to_ptrpanel(TYPE_VARIABLE);
	//MessageBox("1");
	return 0;
}
LRESULT CBacnetVariable::Fresh_Variable_Item(WPARAM wParam,LPARAM lParam)
{
	int cmp_ret ;//compare if match it will 0;

	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;
	CString temp_task_info;
	CString New_CString =  m_variable_list.GetItemText(Changed_Item,Changed_SubItem);
	CString cstemp_value;
	memcpy_s(&m_temp_variable_data[Changed_Item],sizeof(Str_variable_point),&m_Variable_data.at(Changed_Item),sizeof(Str_variable_point));


	if(Changed_SubItem == VARIABLE_LABLE)
	{
		CString cs_temp = m_variable_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= STR_VARIABLE_LABEL)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not higher than 9"),_T("Warning"));
			PostMessage(WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
			return 0;
		}
		cs_temp.MakeUpper();
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Variable_data.at(Changed_Item).label,STR_VARIABLE_LABEL,cTemp1,STR_VARIABLE_LABEL);
	}

	if(Changed_SubItem == VARIABLE_FULL_LABLE)
	{
		CString cs_temp = m_variable_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= STR_VARIABLE_DESCRIPTION_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not higher than 20"),_T("Warning"));
			PostMessage(WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
			return 0;
		}

		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Variable_data.at(Changed_Item).description,STR_VARIABLE_DESCRIPTION_LENGTH,cTemp1,STR_VARIABLE_DESCRIPTION_LENGTH);
	}


	if(Changed_SubItem == VARIABLE_AUTO_MANUAL)
	{
		CString temp_cs = m_variable_list.GetItemText(Changed_Item,Changed_SubItem);
		if(temp_cs.CompareNoCase(_T("Auto"))==0)
		{
			m_variable_list.SetCellEnabled(Changed_Item,VARIABLE_VALUE,0);
			m_Variable_data.at(Changed_Item).auto_manual = BAC_AUTO;
		}
		else
		{
			m_variable_list.SetCellEnabled(Changed_Item,VARIABLE_VALUE,1);
			m_Variable_data.at(Changed_Item).auto_manual = BAC_MANUAL;
		}
	}
	if(Changed_SubItem == VARIABLE_VALUE)//这里只用处理 ANALOG 的值就看要了， DIGITAL 的值在Click 事件中处理过了;
	{
		CString temp_cs = m_variable_list.GetItemText(Changed_Item,Changed_SubItem);

		int temp_int = (int)(_wtof(temp_cs) * 1000);
		m_Variable_data.at(Changed_Item).value = temp_int;
		//int temp_int = _wtoi(temp_cs);
		//m_Variable_data.at(Changed_Item).value = temp_int;
	}

	cmp_ret = memcmp(&m_temp_variable_data[Changed_Item],&m_Variable_data.at(Changed_Item),sizeof(Str_variable_point));
	if(cmp_ret!=0)
	{
		m_variable_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
	temp_task_info.Format(_T("Write Variable List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
	Post_Write_Message(g_bac_instance,WRITEVARIABLE_T3000,Changed_Item,Changed_Item,sizeof(Str_variable_point),m_variable_dlg_hwnd ,temp_task_info,Changed_Item,Changed_SubItem);
	}
	return 0;
}


void CBacnetVariable::OnBnClickedButtonVariableApply()
{
	// TODO: Add your control notification handler code here
	for (int i=0;i<(int)m_Variable_data.size();i++)
	{
		CString cs_temp=m_variable_list.GetItemText(i,VARIABLE_FULL_LABLE);
		char cTemp[255];
		memset(cTemp,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp, 255, NULL, NULL );
		memcpy_s(m_Variable_data.at(i).description,21,cTemp,21);

		cs_temp=m_variable_list.GetItemText(i,VARIABLE_AUTO_MANUAL);
		if(cs_temp.CompareNoCase(_T("Auto"))==0)
		{
			m_Variable_data.at(i).auto_manual= BAC_AUTO;
			m_variable_list.SetCellEnabled(i,VARIABLE_VALUE,0);
		}
		else
		{
			m_Variable_data.at(i).auto_manual = BAC_MANUAL;
			m_variable_list.SetCellEnabled(i,VARIABLE_VALUE,1);
		}


		cs_temp=m_variable_list.GetItemText(i,VARIABLE_UNITE);
		int index_number=0;

		cs_temp=m_variable_list.GetItemText(i,VARIABLE_LABLE);
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Variable_data.at(i).label,STR_IN_LABEL,cTemp1,STR_IN_LABEL);
	}
	Post_Write_Message(g_bac_instance,WRITEVARIABLE_T3000,0,19,sizeof(Str_variable_point),BacNet_hwd);
		
}


void CBacnetVariable::OnNMClickListVariable(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	long lRow,lCol;
	m_variable_list.Set_Edit(true);
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_variable_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_variable_list.SubItemHitTest(&lvinfo);

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;


	if(lRow>m_variable_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow<0)
		return;



	CString temp1;
	m_row = lRow;
	m_col = lCol;

	memcpy_s(&m_temp_variable_data[lRow],sizeof(Str_variable_point),&m_Variable_data.at(lRow),sizeof(Str_variable_point));
	CString New_CString;
	CString temp_task_info;
	if((lCol == VARIABLE_VALUE) &&(m_Variable_data.at(lRow).digital_analog == BAC_UNITS_DIGITAL ) && (m_Variable_data.at(lRow).auto_manual != BAC_AUTO))
	{
		
		
		CString temp1;
		CStringArray temparray;

		if((m_Variable_data.at(lRow).range < 23) &&(m_Variable_data.at(lRow).range !=0))
			temp1 = Digital_Units_Array[m_Variable_data.at(lRow).range];
		else if((m_Variable_data.at(lRow).range >=23) && (m_Variable_data.at(lRow).range <= 30))
		{
			if(receive_customer_unit)
				temp1 = temp_unit_no_index[m_Variable_data.at(lRow).range - 23];
			else
			{
				m_variable_list.Set_Edit(false);
				return;
			}
		}
		else
			return;
		SplitCStringA(temparray,temp1,_T("/"));



		if(m_Variable_data.at(lRow).control == 0)
		{
			m_Variable_data.at(lRow).control = 1;
			m_variable_list.SetItemText(lRow,VARIABLE_VALUE,temparray.GetAt(1));
			New_CString = temparray.GetAt(1);
		}
		else
		{
			m_Variable_data.at(lRow).control = 0;
			m_variable_list.SetItemText(lRow,VARIABLE_VALUE,temparray.GetAt(0));
			New_CString = temparray.GetAt(0);
		}
		m_variable_list.Set_Edit(false);

		temp_task_info.Format(_T("Write Variable List Item%d .Changed to \"%s\" "),lRow + 1,New_CString);
	}
	else if((lCol == VARIABLE_VALUE) &&(m_Variable_data.at(lRow).digital_analog == BAC_UNITS_ANALOG ) && (m_Variable_data.at(lRow).auto_manual == BAC_MANUAL) && (m_Variable_data.at(lRow).range == 20))
	{
		m_variable_list.Set_Edit(false);
		m_variable_time_picker.ShowWindow(SW_SHOW);
		CRect list_rect,win_rect;
		m_variable_list.GetWindowRect(list_rect);
		GetWindowRect(win_rect);
		CRect myrect;
		m_variable_list.GetSubItemRect(lRow,lCol,LVIR_BOUNDS,myrect);


		myrect.left = myrect.left + list_rect.left - win_rect.left;
		myrect.right = myrect.right + list_rect.left - win_rect.left;
		myrect.top = myrect.top + 11;
		myrect.bottom = myrect.bottom + 13;

		m_variable_time_picker.BringWindowToTop();
		m_variable_time_picker.MoveWindow(myrect);

		
		CString Temp_CString =  m_variable_list.GetItemText(lRow,lCol);

		CStringArray TEMPCS;
		int temp_hour,temp_minute;
		SplitCStringA(TEMPCS, Temp_CString, _T(":"));
		if((int)TEMPCS.GetSize() <2)
		{
			temp_hour = 0;
			temp_minute = 0;
			CTime TimeTemp(2014,1,1,temp_hour,temp_minute,0);
			m_variable_time_picker.SetFormat(_T("HH:mm"));
			m_variable_time_picker.SetTime(&TimeTemp);
			m_variable_time_picker.SetFocus();
		}
		else
		{
			temp_hour = _wtoi(TEMPCS.GetAt(0));
			temp_minute = _wtoi(TEMPCS.GetAt(1));
			CTime TimeTemp(2014,1,1,temp_hour,temp_minute,0);
			m_variable_time_picker.SetFormat(_T("HH:mm"));
			m_variable_time_picker.SetTime(&TimeTemp);
			m_variable_time_picker.SetFocus();
		}
		m_variable_list.SetItemText(lRow,lCol,_T(""));
		m_variable_time_picker.Invalidate();
		SetTimer(2,100,NULL);
	}
	else if(lCol == VARIABLE_AUTO_MANUAL)
	{
		if(m_Variable_data.at(lRow).auto_manual == 0)
		{
			m_Variable_data.at(lRow).auto_manual = 1;
			m_variable_list.SetItemText(lRow,VARIABLE_AUTO_MANUAL,_T("Manual"));
			m_variable_list.SetCellEnabled(lRow,VARIABLE_VALUE,TRUE);
			New_CString = _T("Manual");
		}
		else
		{
			m_Variable_data.at(lRow).auto_manual = 0;
			m_variable_list.SetItemText(lRow,VARIABLE_AUTO_MANUAL,_T("Auto"));
			m_variable_list.SetCellEnabled(lRow,VARIABLE_VALUE,FALSE);
			New_CString = _T("Auto");
		}
		temp_task_info.Format(_T("Write Variable List Item%d .Changed to \"%s\" "),lRow + 1,New_CString);
	}
	else if(lCol == VARIABLE_UNITE)
	{
		if(m_Variable_data.at(lRow).digital_analog == BAC_UNITS_ANALOG)
		{
			bac_ranges_type = VARIABLE_RANGE_ANALOG_TYPE;
		}
		else
		{
			bac_ranges_type = VARIABLE_RANGE_DIGITAL_TYPE;
		}

		BacnetRange dlg;

		if(!read_customer_unit)
		{
			for (int i=0;i<BAC_CUSTOMER_UNIT_GROUP;i++)
			{
				int	resend_count = 0;
				do 
				{
					resend_count ++;
					if(resend_count>50)
						break;
					g_invoke_id = GetPrivateData(
						g_bac_instance,
						READUNIT_T3000,
						0 + i*4 ,
						3 + i*4 ,
						sizeof(Str_Units_element));		

					Sleep(SEND_COMMAND_DELAY_TIME);
				} while (g_invoke_id<0);
			}
			read_customer_unit = true;
			Sleep(1000);
		}

		//CString temp_cs = m_variable_list.GetItemText(lRow,lCol);
		//if(temp_cs.CompareNoCase(Units_Type[UNITS_TYPE_ANALOG])==0)
		//{


			bac_range_number_choose = m_Variable_data.at(lRow).range;
			//bac_ranges_type = VARIABLE_RANGE_ANALOG_TYPE;
			dlg.DoModal();
			if(range_cancel)
			{
				PostMessage(WM_REFRESH_BAC_VARIABLE_LIST,lRow,REFRESH_ON_ITEM);//这里调用 刷新线程重新刷新会方便一点;
				return ;
			}

			if(bac_ranges_type == VARIABLE_RANGE_ANALOG_TYPE)
			{
				m_Variable_data.at(lRow).digital_analog = BAC_UNITS_ANALOG;
				m_Variable_data.at(lRow).range = bac_range_number_choose;
				m_variable_list.SetItemText(lRow,lCol,Variable_Analog_Units_Array[bac_range_number_choose]);



				if(m_Variable_data.at(lRow).range == 20)	//如果是时间;
				{
					char temp_char[50];
					int time_seconds = m_Variable_data.at(lRow).value / 1000;
					intervaltotext(temp_char,time_seconds,0,0);

					CString temp_11;
					MultiByteToWideChar( CP_ACP, 0, temp_char, strlen(temp_char) + 1, 
						temp_11.GetBuffer(MAX_PATH), MAX_PATH );
					temp_11.ReleaseBuffer();		
					m_variable_list.SetItemText(lRow,VARIABLE_VALUE,temp_11);

				}
				else
				{
					CString cstemp_value;
					float temp_float_value;
					temp_float_value = ((float)m_Variable_data.at(lRow).value) / 1000;
					cstemp_value.Format(_T("%.3f"),temp_float_value);
					m_variable_list.SetItemText(lRow,VARIABLE_VALUE,cstemp_value);

					/*				cstemp_value.Format(_T("%d"),m_Variable_data.at(Changed_Item).value);
					m_variable_list.SetItemText(Changed_Item,VARIABLE_VALUE,cstemp_value);*/	
				}
			}
			else if((bac_ranges_type == VARIABLE_RANGE_DIGITAL_TYPE) || (bac_ranges_type == INPUT_RANGE_DIGITAL_TYPE) || (bac_ranges_type == OUTPUT_RANGE_DIGITAL_TYPE))
			{
				m_Variable_data.at(lRow).digital_analog = BAC_UNITS_DIGITAL;
				m_Variable_data.at(lRow).range = bac_range_number_choose;


				CStringArray temparray;
				if((bac_range_number_choose >= 23) && (bac_range_number_choose <= 30))
				{
					//temp1.Format(_T("%s"), temp_unit_no_index[bac_range_number_choose - 23]);
					temp1 = temp_unit_no_index[bac_range_number_choose - 23];
				}
				else
					temp1 = Digital_Units_Array[bac_range_number_choose];//22 is the sizeof the array

				SplitCStringA(temparray,temp1,_T("/"));





				if(m_Variable_data.at(lRow).control == 1)
				{
					if((temparray.GetSize()==2)&&(!temparray.GetAt(1).IsEmpty()))
					{
						m_variable_list.SetItemText(lRow,VARIABLE_VALUE,temparray.GetAt(1));
					}
				}
				else
				{
					if((temparray.GetSize()==2)&&(!temparray.GetAt(0).IsEmpty()))
					{
						m_variable_list.SetItemText(lRow,VARIABLE_VALUE,temparray.GetAt(0));
					}			
				}
				m_variable_list.SetItemText(lRow,VARIABLE_UNITE,temp1);
			}
		
	
	}
	else
		return;

	int cmp_ret = memcmp(&m_temp_variable_data[lRow],&m_Variable_data.at(lRow),sizeof(Str_variable_point));
	if(cmp_ret!=0)
	{
		m_variable_list.SetItemBkColor(lRow,lCol,LIST_ITEM_CHANGED_BKCOLOR);
		Post_Write_Message(g_bac_instance,WRITEVARIABLE_T3000,lRow,lRow,sizeof(Str_variable_point),m_variable_dlg_hwnd,temp_task_info,lRow,lCol);
	}
//	GetDlgItem(IDC_BUTTON_VARIABLE_APPLY)->SetFocus();
	*pResult = 0;
}





void CBacnetVariable::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1:
		{
			if(this->IsWindowVisible())
			{
			::PostMessage(m_variable_dlg_hwnd,WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
			if(bac_select_device_online)
				Post_Refresh_Message(g_bac_instance,READVARIABLE_T3000,0,BAC_VARIABLE_ITEM_COUNT - 1,sizeof(Str_variable_point),BAC_VARIABLE_GROUP);
			}
		}
		break;
	case 2:
		{
			KillTimer(2);
			m_variable_time_picker.Invalidate();
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CBacnetVariable::OnNMKillfocusDatetimepicker2Variable(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CTime temp_time;CString temp_cs;
	int chour,cmin;
	m_variable_time_picker.GetTime(temp_time);
	chour = temp_time.GetHour();
	cmin = temp_time.GetMinute();



	if((chour == 0) &&(cmin	==0))
	{
		temp_cs.Empty();
	}
	else
		temp_cs.Format(_T("%02d:%02d"),chour,cmin);
	m_variable_list.SetItemText(m_row,m_col,temp_cs);

	m_variable_time_picker.ShowWindow(SW_HIDE);

	int write_value;
	write_value =( chour*3600 + cmin * 60) * 1000;
	m_Variable_data.at(m_row).value = write_value;
	CString temp_task_info;
	temp_task_info.Format(_T("Write Variable Time Item%d .Changed Time to \"%s\" "),m_row + 1,temp_cs);
	Post_Write_Message(g_bac_instance,WRITEVARIABLE_T3000,m_row,m_row,sizeof(Str_variable_point),BacNet_hwd,temp_task_info);



	*pResult = 0;
}


BOOL CBacnetVariable::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN  )
	{
		if(pMsg->wParam == VK_RETURN)
		{

			CRect list_rect,win_rect;
			m_variable_list.GetWindowRect(list_rect);
			ScreenToClient(&list_rect);
			::GetWindowRect(m_variable_dlg_hwnd,win_rect);
			m_variable_list.Set_My_WindowRect(win_rect);
			m_variable_list.Set_My_ListRect(list_rect);

			m_variable_list.Get_clicked_mouse_position();

			CWnd *temp_focus=GetFocus();	//Maurice require ,click enter and the cursor still in this edit or combobox.
			GetDlgItem(IDC_BUTTON_VARIABLE_READ)->SetFocus();
			temp_focus->SetFocus();
			return 1;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetVariable::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1);
	m_variable_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	CDialogEx::OnClose();
}


void CBacnetVariable::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	//KillTimer(1);
	//m_variable_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	//CDialogEx::OnCancel();
}
