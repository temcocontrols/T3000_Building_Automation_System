// BacnetVariable.cpp : implementation file
// BacnetVariable.cpp Add by Fance 2013 08 21

#include "stdafx.h"
#include "T3000.h"
#include "BacnetVariable.h"
#include "afxdialogex.h"

#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "global_function.h"
#include "global_define.h"
#include "BacnetRange.h"
extern void copy_data_to_ptrpanel(int Data_type);//Used for copy the structure to the ptrpanel.
extern int initial_dialog;




IMPLEMENT_DYNAMIC(CBacnetVariable, CDialogEx)

CBacnetVariable::CBacnetVariable(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetVariable::IDD, pParent)
{
	window_max = true;
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
	ON_WM_HELPINFO()
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
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
		if((pInvoke->mRow < BAC_VARIABLE_ITEM_COUNT) && (pInvoke->mRow >= 0))
		{
			Post_Refresh_One_Message(g_bac_instance,READVARIABLE_T3000,
				pInvoke->mRow,pInvoke->mRow,sizeof(Str_variable_point));
			SetTimer(3,2000,NULL);
		}
	}
	else
	{
		memcpy_s(&m_Variable_data.at(pInvoke->mRow),sizeof(Str_variable_point),&m_temp_variable_data[pInvoke->mRow],sizeof(Str_variable_point));//还原没有改对的值
		PostMessage(WM_REFRESH_BAC_VARIABLE_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
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
	SetWindowTextW(_T("VARIABLE"));
	
	Initial_List();	//Initial the list of Variable,read from device;
	PostMessage(WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);

	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_REFRESH);
	((CButton *)GetDlgItem(IDC_BUTTON_VARIABLE_READ))->SetIcon(hIcon);	
	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_OK);
	((CButton *)GetDlgItem(IDC_BUTTON_VARIABLE_APPLY))->SetIcon(hIcon);

	HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_VARIABLE);
	SetIcon(m_hIcon,TRUE);

	SetTimer(1,BAC_LIST_REFRESH_TIME + 5000,NULL);

	//SetTimer(6,250,NULL);
	ShowWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CBacnetVariable::OnBnClickedButtonVariableRead()
{
	

	PostMessage(WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
}


void CBacnetVariable::Initial_List()
{
	m_variable_list.ShowWindow(SW_HIDE);
	m_variable_list.DeleteAllItems();
	while ( m_variable_list.DeleteColumn (0)) ;

	m_variable_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_variable_list.SetExtendedStyle(m_variable_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_variable_list.InsertColumn(VARIABLE_NUM, _T("Variable"), 70, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_variable_list.InsertColumn(VARIABLE_FULL_LABLE, _T("Full Label"), 200, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_variable_list.InsertColumn(VARIABLE_AUTO_MANUAL, _T("Auto/Manual"), 150, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_variable_list.InsertColumn(VARIABLE_VALUE, _T("Value"), 120, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_variable_list.InsertColumn(VARIABLE_UNITE, _T("Units"), 120, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_variable_list.InsertColumn(VARIABLE_LABLE, _T("Label"), 130, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_variable_list.Setlistcolcharlimit(VARIABLE_FULL_LABLE,STR_VARIABLE_DESCRIPTION_LENGTH -1);
	m_variable_list.Setlistcolcharlimit(VARIABLE_LABLE,STR_VARIABLE_LABEL-1);

	m_variable_dlg_hwnd = this->m_hWnd;
	//g_hwnd_now = m_variable_dlg_hwnd;
	m_variable_list.SetListHwnd(this->m_hWnd);
	CRect list_rect,win_rect;
	m_variable_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_input_dlg_hwnd,win_rect);
	m_variable_list.Set_My_WindowRect(win_rect);
	m_variable_list.Set_My_ListRect(list_rect);

	CTime TimeTemp(2017,1,1,0,0,0);
	m_variable_time_picker.SetFormat(_T("HH:mm"));
	m_variable_time_picker.SetTime(&TimeTemp);
	m_variable_time_picker.ShowWindow(SW_HIDE);

	m_variable_list.DeleteAllItems();
	for (int i=0;i<(int)m_Variable_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;

		if(i>=variable_item_limit_count)
			break;

		temp_item.Format(_T("VAR%d"),i+1);
		m_variable_list.InsertItem(i,temp_item);

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
	m_variable_list.InitListData();
	m_variable_list.ShowWindow(SW_SHOW);
}


LRESULT CBacnetVariable::Fresh_Variable_List(WPARAM wParam,LPARAM lParam)
{

	int Fresh_Item;
	int isFreshOne = (int)lParam;
	bool need_refresh_all = false;
	int Fresh_List_Now = (int)wParam;
	if( Fresh_List_Now == REFRESH_LIST_NOW)
	{
		need_refresh_all = true;
	}

	if(need_refresh_all == false)
	{
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

		if(i>=variable_item_limit_count)
		{
			return 0;
		}

		MultiByteToWideChar( CP_ACP, 0, (char *)m_Variable_data.at(i).description, (int)strlen((char *)m_Variable_data.at(i).description)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();
		m_variable_list.SetItemText(i,VARIABLE_FULL_LABLE,temp_des);
		if(m_Variable_data.at(i).auto_manual==0)
		{
			m_variable_list.SetItemText(i,VARIABLE_AUTO_MANUAL,_T("Auto"));
		}
		else
		{
			m_variable_list.SetItemText(i,VARIABLE_AUTO_MANUAL,_T("Manual"));
		}


		if(m_Variable_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
		{
			
			if((m_Variable_data.at(i).range == 0) || (m_Variable_data.at(i).range>30))
			{
				CString cstemp_value2;
				float temp_float_value1;
				temp_float_value1 = ((float)m_Variable_data.at(i).value) / 1000;
				cstemp_value2.Format(_T("%.3f"),temp_float_value1);
				m_variable_list.SetItemText(i,VARIABLE_VALUE,cstemp_value2);
				m_variable_list.SetItemText(i,VARIABLE_UNITE,Variable_Analog_Units_Array[0]);
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
						temp1 = Custom_Digital_Range[m_Variable_data.at(i).range - 23];
				}
				else
				{
					temp1 = Digital_Units_Array[0];
					m_variable_list.SetItemText(i,VARIABLE_UNITE,temp1);
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
		}
		else
		{
			if(m_Variable_data.at(i).range == 20)	//如果是时间;
			{
				m_variable_list.SetItemText(i,VARIABLE_UNITE,Variable_Analog_Units_Array[m_Variable_data.at(i).range]);
				char temp_char[50];
				int time_seconds = m_Variable_data.at(i).value / 1000;
				intervaltotextfull(temp_char,time_seconds,0,0);
				CString temp_11;
				MultiByteToWideChar( CP_ACP, 0, temp_char, strlen(temp_char) + 1, 
					temp_11.GetBuffer(MAX_PATH), MAX_PATH );
				temp_11.ReleaseBuffer();		
				m_variable_list.SetItemText(i,VARIABLE_VALUE,temp_11);

			}

			else if(m_Variable_data.at(i).range<sizeof(Variable_Analog_Units_Array)/sizeof(Variable_Analog_Units_Array[0]))
			{
			m_variable_list.SetItemText(i,VARIABLE_UNITE,Variable_Analog_Units_Array[m_Variable_data.at(i).range]);

			CString cstemp_value;
			float temp_float_value;
			temp_float_value = ((float)m_Variable_data.at(i).value) / 1000;
			cstemp_value.Format(_T("%.3f"),temp_float_value);
			m_variable_list.SetItemText(i,VARIABLE_VALUE,cstemp_value);
			}
			else if((m_Variable_data.at(i).range >= 34 ) && (m_Variable_data.at(i).range <= 38))
			{
				m_variable_list.SetItemText(i,VARIABLE_UNITE,Analog_Variable_Units[m_Variable_data.at(i).range - 34]);

				CString cstemp_value;
				float temp_float_value;
				temp_float_value = ((float)m_Variable_data.at(i).value) / 1000;
				cstemp_value.Format(_T("%.3f"),temp_float_value);
				m_variable_list.SetItemText(i,VARIABLE_VALUE,cstemp_value);
			}
			else
			{
				m_variable_list.SetItemText(i,VARIABLE_UNITE,Variable_Analog_Units_Array[0]);

				CString cstemp_value;
				float temp_float_value;
				temp_float_value = ((float)m_Variable_data.at(i).value) / 1000;
				cstemp_value.Format(_T("%.3f"),temp_float_value);
				m_variable_list.SetItemText(i,VARIABLE_VALUE,cstemp_value);
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

#pragma region note_what_do	
			//如下所做的事 达到 ,在弹窗后 继续选中客户输入的部分 ，毛总的要求;
			CRect list_rect,win_rect;
			m_variable_list.GetWindowRect(list_rect);
			ScreenToClient(&list_rect);
			::GetWindowRect(m_variable_dlg_hwnd,win_rect);
			m_variable_list.Set_My_WindowRect(win_rect);
			m_variable_list.Set_My_ListRect(list_rect);

			m_variable_list.Get_clicked_mouse_position();
#pragma endregion note_what_do

			PostMessage(WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
			return 0;
		}
		cs_temp.Replace(_T("-"),_T("_"));
		cs_temp.MakeUpper();
		if(Check_Label_Exsit(cs_temp))
		{
			PostMessage(WM_REFRESH_BAC_VARIABLE_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0;
		}
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
#pragma region note_what_do	
			//如下所做的事 达到 ,在弹窗后 继续选中客户输入的部分 ，毛总的要求;
			CRect list_rect,win_rect;
			m_variable_list.GetWindowRect(list_rect);
			ScreenToClient(&list_rect);
			::GetWindowRect(m_variable_dlg_hwnd,win_rect);
			m_variable_list.Set_My_WindowRect(win_rect);
			m_variable_list.Set_My_ListRect(list_rect);

			m_variable_list.Get_clicked_mouse_position();
#pragma endregion note_what_do
			PostMessage(WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
			return 0;
		}
		if(Check_FullLabel_Exsit(cs_temp))
		{
			PostMessage(WM_REFRESH_BAC_VARIABLE_LIST,Changed_Item,REFRESH_ON_ITEM);
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
			//m_variable_list.SetCellEnabled(Changed_Item,VARIABLE_VALUE,0);
			m_Variable_data.at(Changed_Item).auto_manual = BAC_AUTO;
		}
		else
		{
			//m_variable_list.SetCellEnabled(Changed_Item,VARIABLE_VALUE,1);
			m_Variable_data.at(Changed_Item).auto_manual = BAC_MANUAL;
		}
	}
	if(Changed_SubItem == VARIABLE_VALUE)//这里只用处理 ANALOG 的值就看要了， DIGITAL 的值在Click 事件中处理过了;
	{
		CString temp_cs = m_variable_list.GetItemText(Changed_Item,Changed_SubItem);

		int temp_int = (int)(_wtof(temp_cs) * 1000);
		m_Variable_data.at(Changed_Item).value = temp_int;
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
		}
		else
		{
			m_Variable_data.at(i).auto_manual = BAC_MANUAL;
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
				temp1 = Custom_Digital_Range[m_Variable_data.at(lRow).range - 23];
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


		myrect.left = myrect.left + list_rect.left - win_rect.left  - 6 ;
		myrect.right = myrect.right + list_rect.left - win_rect.left - 6;
		myrect.top = myrect.top -1 ;
		myrect.bottom = myrect.bottom + 3;
		m_variable_time_picker.BringWindowToTop();
		m_variable_time_picker.MoveWindow(myrect);

		
		CString Temp_CString =  m_variable_list.GetItemText(lRow,lCol);

		CStringArray TEMPCS;
		int temp_hour,temp_minute,temp_second;
		SplitCStringA(TEMPCS, Temp_CString, _T(":"));
		if((int)TEMPCS.GetSize() <3)
		{
			temp_hour = 0;
			temp_minute = 0;
			temp_second = 0;
			CTime TimeTemp(2017,1,1,temp_hour,temp_minute,0);
			m_variable_time_picker.SetFormat(_T("HH:mm:ss"));
			m_variable_time_picker.SetTime(&TimeTemp);
			m_variable_time_picker.SetFocus();
		}
		else
		{
			temp_hour = _wtoi(TEMPCS.GetAt(0));
			temp_minute = _wtoi(TEMPCS.GetAt(1));
			temp_second = _wtoi(TEMPCS.GetAt(2));
			if(temp_hour >=24)
				temp_hour = 0;
			if(temp_minute >=60)
				temp_minute = 0;
			if(temp_second >= 60)
				temp_second = 0;
			CTime TimeTemp(2017,1,1,temp_hour,temp_minute,temp_second);
			m_variable_time_picker.SetFormat(_T("HH:mm:ss"));
			m_variable_time_picker.SetTime(&TimeTemp);
			m_variable_time_picker.SetFocus();
		}
		m_variable_list.SetItemText(lRow,lCol,_T(""));
		m_variable_time_picker.Invalidate();
		SetTimer(2,100,NULL);
	}
	else if(lCol == VARIABLE_VALUE)
	{
		if(m_Variable_data.at(lRow).auto_manual == BAC_AUTO)
		{
			m_variable_list.Set_Edit(false);
			return;
		}
	}
	else if(lCol == VARIABLE_AUTO_MANUAL)
	{
		if(m_Variable_data.at(lRow).auto_manual == 0)
		{
			m_Variable_data.at(lRow).auto_manual = 1;
			m_variable_list.SetItemText(lRow,VARIABLE_AUTO_MANUAL,_T("Manual"));
			New_CString = _T("Manual");
		}
		else
		{
			m_Variable_data.at(lRow).auto_manual = 0;
			m_variable_list.SetItemText(lRow,VARIABLE_AUTO_MANUAL,_T("Auto"));
			New_CString = _T("Auto");
		}
		temp_task_info.Format(_T("Write Variable List Item%d .Changed to \"%s\" "),lRow + 1,New_CString);
	}
	else if(lCol == VARIABLE_UNITE)
	{


		BacnetRange dlg;

		//点击产品的时候 需要读custom units，老的产品firmware 说不定没有 这些，所以不强迫要读到;
		if(!read_customer_unit)
		{

			int temp_invoke_id = -1;
			int send_status = true;
			int	resend_count = 0;
			for (int z=0;z<3;z++)
			{
				do 
				{
					resend_count ++;
					if(resend_count>5)
					{
						send_status = false;
						break;
					}
					temp_invoke_id =  GetPrivateData(
						g_bac_instance,
						READUNIT_T3000,
						0,
						BAC_CUSTOMER_UNITS_COUNT - 1,
						sizeof(Str_Units_element));		

					Sleep(SEND_COMMAND_DELAY_TIME);
				} while (g_invoke_id<0);
				if(send_status)
				{
					for (int z=0;z<1000;z++)
					{
						Sleep(1);
						if(tsm_invoke_id_free(temp_invoke_id))
						{
							read_customer_unit = true;
							break;
						}
						else
							continue;
					}

				}
				if(read_customer_unit)
					break;
			}

		}

		CString temp_info;
		if(GetPrivateData_Blocking(g_bac_instance,READVARIABLE_T3000,0,4,sizeof(Str_variable_uint_point)) > 0)
		{
			temp_info.Format(_T("Read variable custmer units success."));
			SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_info);
		}
		else
		{
			temp_info.Format(_T("Read variable custmer units success."));
			SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_info);
		}

		if(m_Variable_data.at(lRow).digital_analog == BAC_UNITS_ANALOG)
		{
			bac_ranges_type = VARIABLE_RANGE_ANALOG_TYPE;
			if(m_Variable_data.at(lRow).range > (sizeof(Variable_Analog_Units_Array) / sizeof(Variable_Analog_Units_Array[0])))
			{
				m_Variable_data.at(lRow).range = 0;
				bac_range_number_choose = 0;
			}
		}
		else
		{
			bac_ranges_type = VARIABLE_RANGE_DIGITAL_TYPE;
			if(m_Variable_data.at(lRow).range > 30)
			{
				m_Variable_data.at(lRow).range = 0;
				bac_range_number_choose = 0;
			}
		}


			initial_dialog = 1;
			bac_range_number_choose = m_Variable_data.at(lRow).range;
			dlg.DoModal();
			if(range_cancel)
			{
				PostMessage(WM_REFRESH_BAC_VARIABLE_LIST,lRow,REFRESH_ON_ITEM);//这里调用 刷新线程重新刷新会方便一点;
				return ;
			}
			if(bac_range_number_choose == 0)	//如果选择的是 unused 就认为是analog 的unused;这样 能显示对应的value;
			{
				m_Variable_data.at(lRow).digital_analog =  BAC_UNITS_ANALOG;
				bac_ranges_type = VARIABLE_RANGE_ANALOG_TYPE;
			}

			if(bac_ranges_type == VARIABLE_RANGE_ANALOG_TYPE)
			{
				m_Variable_data.at(lRow).digital_analog = BAC_UNITS_ANALOG;
				m_Variable_data.at(lRow).range = bac_range_number_choose;
				if((bac_range_number_choose >= 34) && (bac_range_number_choose <= 38))
				{
					m_variable_list.SetItemText(lRow,lCol,Analog_Variable_Units[bac_range_number_choose - 34]);
				}
				else
				m_variable_list.SetItemText(lRow,lCol,Variable_Analog_Units_Array[bac_range_number_choose]);



				if(m_Variable_data.at(lRow).range == 20)	//如果是时间;
				{
					char temp_char[50];
					int time_seconds = m_Variable_data.at(lRow).value / 1000;
					intervaltotextfull(temp_char,time_seconds,0,0);

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
					//temp1.Format(_T("%s"), Custom_Digital_Range[bac_range_number_choose - 23]);
					temp1 = Custom_Digital_Range[bac_range_number_choose - 23];
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
	{
		return;
	}

	int cmp_ret = memcmp(&m_temp_variable_data[lRow],&m_Variable_data.at(lRow),sizeof(Str_variable_point));
	if(cmp_ret!=0)
	{
		m_variable_list.SetItemBkColor(lRow,lCol,LIST_ITEM_CHANGED_BKCOLOR);
		Post_Write_Message(g_bac_instance,WRITEVARIABLE_T3000,lRow,lRow,sizeof(Str_variable_point),m_variable_dlg_hwnd,temp_task_info,lRow,lCol);
	}
	*pResult = 0;
}





void CBacnetVariable::OnTimer(UINT_PTR nIDEvent)
{
	 
	switch(nIDEvent)
	{
	case 1:
		{
			if(g_protocol == PROTOCOL_BIP_TO_MSTP)
			{
				PostMessage(WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
			}
			else if((this->IsWindowVisible()) && (Gsm_communication == false) &&  ((this->m_hWnd  == ::GetActiveWindow()) || (bacnet_view_number == TYPE_VARIABLE))  )	//GSM连接时不要刷新;
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
	case 3:
		{
			//在更改某一列之后要在读取此列的值，并刷新此列;

			if(this->IsWindowVisible())
				PostMessage(WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
			KillTimer(3);

		}
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CBacnetVariable::OnNMKillfocusDatetimepicker2Variable(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	CTime temp_time;CString temp_cs;
	int chour,cmin,csend;
	m_variable_time_picker.GetTime(temp_time);
	chour = temp_time.GetHour();
	cmin = temp_time.GetMinute();
	csend = temp_time.GetSecond();


	if((chour == 0) &&(cmin	==0) && (csend == 0))
	{
		temp_cs.Empty();
	}
	else
		temp_cs.Format(_T("%02d:%02d:%02d"),chour,cmin,csend);
	m_variable_list.SetItemText(m_row,m_col,temp_cs);

	m_variable_time_picker.ShowWindow(SW_HIDE);

	int write_value;
	write_value =( chour*3600 + cmin * 60 + csend) * 1000;
	m_Variable_data.at(m_row).value = write_value;
	CString temp_task_info;
	temp_task_info.Format(_T("Write Variable Time Item%d .Changed Time to \"%s\" "),m_row + 1,temp_cs);
	Post_Write_Message(g_bac_instance,WRITEVARIABLE_T3000,m_row,m_row,sizeof(Str_variable_point),BacNet_hwd,temp_task_info);



	*pResult = 0;
}


BOOL CBacnetVariable::PreTranslateMessage(MSG* pMsg)
{
	
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
	else if(pMsg->message==WM_NCLBUTTONDBLCLK)
	{
		if(!window_max)
		{
			window_max = true;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), SWP_SHOWWINDOW);
		}
		else
		{
			window_max = false;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left  + 60 ,temp_mynew_rect.top + 60,500,700,SWP_SHOWWINDOW);
		}


		return 1; 
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetVariable::OnClose()
{
	 
	ShowWindow(FALSE);
	return;
	KillTimer(1);
	m_variable_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	CDialogEx::OnClose();
}


void CBacnetVariable::OnCancel()
{
	
	//KillTimer(1);
	//m_variable_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	//CDialogEx::OnCancel();
}



int GetVariableLabel(int index,CString &ret_label, Point_Net * npoint)
{
	if(index >= BAC_VARIABLE_ITEM_COUNT)
	{
		ret_label.Empty();
		return -1;
	}
	int i = index;
	CString temp_des2;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_Variable_data.at(i).label, (int)strlen((char *)m_Variable_data.at(i).label)+1, 
		ret_label.GetBuffer(MAX_PATH), MAX_PATH );
	ret_label.ReleaseBuffer();

	CString temp_cs;
	temp_cs = ret_label;
	temp_cs = temp_cs.Trim();
	if(temp_cs.IsEmpty())
	{
        if (npoint == NULL)
            temp_cs.Format(_T("VAR%u"), index + 1);
        else
            temp_cs.Format(_T("%dVAR%u"), npoint->panel, index + 1);
		ret_label = temp_cs;
	}


	return 1;
}

int GetVariableFullLabel(int index,CString &ret_full_label, Point_Net * npoint)
{
	if(index >= BAC_VARIABLE_ITEM_COUNT)
	{
		ret_full_label.Empty();
		return -1;
	}
	int i = index;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_Variable_data.at(i).description, (int)strlen((char *)m_Variable_data.at(i).description)+1, 
		ret_full_label.GetBuffer(MAX_PATH), MAX_PATH );
	ret_full_label.ReleaseBuffer();

	CString temp_cs;
	temp_cs = ret_full_label;
	temp_cs = temp_cs.Trim();
	if(temp_cs.IsEmpty())
	{
        if (npoint == NULL)
		    temp_cs.Format(_T("VAR%u"),index + 1);
        else
            temp_cs.Format(_T("%dVAR%u"), npoint->panel, index + 1);
		ret_full_label = temp_cs;
	}

	return 1;
}

int GetVariableValue(int index ,CString &ret_cstring,CString &ret_unit,CString &Auto_M,int &digital_value)
{
	CStringArray temparray;
	CString temp1;
	if(index >= BAC_VARIABLE_ITEM_COUNT)
	{
		ret_cstring.Empty();
		ret_unit.Empty();
		Auto_M.Empty();
		return -1;
	}
	int i = index;

	if(m_Variable_data.at(i).auto_manual == 1)
	{
		Auto_M = _T("M");
	}
	else
	{
		Auto_M.Empty();
	}

	if(m_Variable_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
	{
		if(m_Variable_data.at(i).range>30)
		{
			ret_cstring = _T(" ");
			return RANGE_ERROR;
		}
		else
		{
			if((m_Variable_data.at(i).range < 23) /*&&(m_Variable_data.at(i).range !=0)*/)
				temp1 = Digital_Units_Array[m_Variable_data.at(i).range];
			else if((m_Variable_data.at(i).range >=23) && (m_Variable_data.at(i).range <= 30))
			{
				if(receive_customer_unit)
					temp1 = Custom_Digital_Range[m_Variable_data.at(i).range - 23];
			}
			else
			{
				ret_cstring = _T(" ");
				return RANGE_ERROR;
			}


			SplitCStringA(temparray,temp1,_T("/"));
			if((temparray.GetSize()==2))
			{
				if(m_Variable_data.at(i).control == 0)
				{
					digital_value = 0;
					ret_cstring = temparray.GetAt(0);
					return 1;
				}
				else
				{
					digital_value = 1;
					ret_cstring = temparray.GetAt(1);
				}
			}

		}
	}
	else
	{
		if(m_Variable_data.at(i).range == 20)	//如果是时间;
		{
			ret_unit = Variable_Analog_Units_Array[m_Variable_data.at(i).range];
			char temp_char[50];
			int time_seconds = m_Variable_data.at(i).value / 1000;
			intervaltotextfull(temp_char,time_seconds,0,0);

			MultiByteToWideChar( CP_ACP, 0, temp_char, strlen(temp_char) + 1, 
				ret_cstring.GetBuffer(MAX_PATH), MAX_PATH );
			ret_cstring.ReleaseBuffer();	
			digital_value = 2;

		}
		else if((m_Variable_data.at(i).range<=sizeof(Variable_Analog_Units_Array)/sizeof(Variable_Analog_Units_Array[0]))/* && (m_Variable_data.at(i).range != 0)*/)
		{
			ret_unit = Variable_Analog_Units_Array[m_Variable_data.at(i).range];
			if(m_Variable_data.at(i).range == 0)
				ret_unit.Empty();
			CString cstemp_value;
			float temp_float_value;
			temp_float_value = ((float)m_Variable_data.at(i).value) / 1000;
			ret_cstring.Format(_T("%.1f"),temp_float_value);
			digital_value = 2;
		}
		else
		{
			ret_cstring = _T(" ");
			ret_cstring.Empty();
			return RANGE_ERROR;
		}
	}

	return 1;
}


BOOL CBacnetVariable::OnHelpInfo(HELPINFO* pHelpInfo)
{
	HWND hWnd;
	if (pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
	else
		hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_6_5_VARIABLES);
	return (hWnd != NULL);
	return CDialogEx::OnHelpInfo(pHelpInfo);
}


void CBacnetVariable::Reset_Variable_Rect()
{

	CRect temp_mynew_rect;
	::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;

	CRect temp_window;
	GetWindowRect(&temp_window);

	if(window_max)
	{
		CRect temp_mynew_rect;
		::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), NULL);
	}
	else if((temp_window.Width() <= temp_mynew_rect.Width() ) && (temp_window.Height() <= temp_mynew_rect.Height()))
	{
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,0,0,SWP_NOSIZE );
	}
	else
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left + 60,temp_mynew_rect.top + 60,700,700, NULL);


	return;

}


void CBacnetVariable::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	
	CRect rc;
	GetClientRect(rc);
	if(m_variable_list.m_hWnd != NULL)
	{
		::SetWindowPos(this->m_hWnd, HWND_TOP, 0,0, 0,0,  SWP_NOSIZE | SWP_NOMOVE);
		m_variable_list.MoveWindow(&rc);
	}
}


void CBacnetVariable::OnSysCommand(UINT nID, LPARAM lParam)
{
	 

	if(nID == SC_MAXIMIZE)
	{
		if(window_max == false)
		{
			window_max = true;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), SWP_SHOWWINDOW);
		}
		else
		{
			window_max = false;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left  + 60 ,temp_mynew_rect.top + 60,500,700,SWP_SHOWWINDOW);
		}
		return;
	}

	CDialogEx::OnSysCommand(nID, lParam);
}
