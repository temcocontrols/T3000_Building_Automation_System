// BacnetVariable.cpp : implementation file
// BacnetVariable.cpp Add by Fance 2013 08 21

#include "stdafx.h"
#include "T3000.h"
#include "BacnetPvar.h"
#include "afxdialogex.h"

#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "global_function.h"
#include "global_define.h"
#include "BacnetRange.h"
#include "MainFrm.h"
#include "CBacnetUnitsSelection.h"
extern void copy_data_to_ptrpanel(int Data_type);//Used for copy the structure to the ptrpanel.
extern int initial_dialog;
// Selected device information
extern tree_product selected_product_Node; // 选中的设备信息;



IMPLEMENT_DYNAMIC(CBacnetPvar, CDialogEx)

CBacnetPvar::CBacnetPvar(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetPvar::IDD, pParent)
{
	window_max = true;
}

CBacnetPvar::~CBacnetPvar()
{
}

void CBacnetPvar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PVARIABLE, m_pvar_list);
	DDX_Control(pDX, IDC_DATETIMEPICKER2_PVARIABLE, m_pvar_time_picker);
}


BEGIN_MESSAGE_MAP(CBacnetPvar, CDialogEx)
	ON_MESSAGE(MY_RESUME_DATA, VariableMessageCallBack)
	ON_BN_CLICKED(IDC_BUTTON_PVARIABLE_READ, &CBacnetPvar::OnBnClickedButtonVariableRead)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Pvar_Item)
//	ON_MESSAGE(MY_RESUME_DATA, VariableResumeMessageCallBack)
	ON_MESSAGE(WM_REFRESH_BAC_PVAR_LIST,Fresh_Pvar_List)
	ON_BN_CLICKED(IDC_BUTTON_pVARIABLE_APPLY, &CBacnetPvar::OnBnClickedButtonVariableApply)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PVARIABLE, &CBacnetPvar::OnNMClickListVariable)
	ON_WM_TIMER()
	ON_NOTIFY(NM_KILLFOCUS, IDC_DATETIMEPICKER2_PVARIABLE, &CBacnetPvar::OnNMKillfocusDatetimepicker2Variable)
	ON_WM_CLOSE()
	ON_WM_HELPINFO()
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CBacnetPvar message handlers
LRESULT  CBacnetPvar::VariableMessageCallBack(WPARAM wParam, LPARAM lParam)
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
            // Not Modbus protocol, call the following refresh single item function
            if ((!SPECIAL_BAC_TO_MODBUS) && (Bacnet_Private_Device(selected_product_Node.product_class_id))) //不是转Modbus的协议的 就调用下面的刷新单条.)
            {
                Post_Refresh_One_Message(g_bac_instance, READVARIABLE_T3000,
                    pInvoke->mRow, pInvoke->mRow, sizeof(Str_variable_point));
                SetTimer(3, 2000, NULL);
            }

		}
	}
	else
	{
		// Restore incorrect values
		memcpy_s(&m_pvar_data.at(pInvoke->mRow),sizeof(Str_variable_point),&m_temp_pvar_data[pInvoke->mRow],sizeof(Str_variable_point));//还原没有改对的值
		PostMessage(WM_REFRESH_BAC_PVAR_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
	}
	// Restore foreground and background colors
	if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
		m_pvar_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_pvar_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
	m_pvar_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);

	if(pInvoke)
		delete pInvoke;
	return 0;
}


BOOL CBacnetPvar::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowTextW(_T("Proprietary Bacnet Variable"));
	
	Initial_List();	//Initial the list of Variable,read from device;
	PostMessage(WM_REFRESH_BAC_PVAR_LIST,NULL,NULL);

	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_REFRESH);
	((CButton *)GetDlgItem(IDC_BUTTON_PVARIABLE_READ))->SetIcon(hIcon);
	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_OK);
	((CButton *)GetDlgItem(IDC_BUTTON_pVARIABLE_APPLY))->SetIcon(hIcon);

	HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_VARIABLE);
	SetIcon(m_hIcon,TRUE);

	SetTimer(1, BAC_LIST_REFRESH_TIME,NULL);
	SetTimer(4, 15000, NULL);
	ShowWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CBacnetPvar::OnBnClickedButtonVariableRead()
{
	

	PostMessage(WM_REFRESH_BAC_PVAR_LIST,NULL,NULL);
}


void CBacnetPvar::Initial_List()
{
	m_pvar_list.ShowWindow(SW_HIDE);
	m_pvar_list.DeleteAllItems();
	while (m_pvar_list.DeleteColumn(0));

	m_pvar_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	m_pvar_list.SetExtendedStyle(m_pvar_list.GetExtendedStyle() | LVS_EX_GRIDLINES & (~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_pvar_list.InsertColumn(VARIABLE_NUM, _T("Pvar"), 70, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_pvar_list.InsertColumn(VARIABLE_FULL_LABLE, _T("Full Label"), 200, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_pvar_list.InsertColumn(VARIABLE_AUTO_MANUAL, _T("Auto/Manual"), 150, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_pvar_list.InsertColumn(VARIABLE_VALUE, _T("Value"), 120, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_pvar_list.InsertColumn(VARIABLE_UNITE, _T("Units"), 0, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_pvar_list.InsertColumn(VARIABLE_LABLE, _T("Label"), 130, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_pvar_list.Setlistcolcharlimit(VARIABLE_FULL_LABLE, STR_VARIABLE_DESCRIPTION_LENGTH - 1);
	m_pvar_list.Setlistcolcharlimit(VARIABLE_LABLE, STR_VARIABLE_LABEL - 1);

	m_pvar_dlg_hwnd = this->m_hWnd;
	m_pvar_list.SetListHwnd(this->m_hWnd);
	CRect list_rect, win_rect;
	m_pvar_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_input_dlg_hwnd, win_rect);
	m_pvar_list.Set_My_WindowRect(win_rect);
	m_pvar_list.Set_My_ListRect(list_rect);

	CTime TimeTemp(2017, 1, 1, 0, 0, 0);
	m_pvar_time_picker.SetFormat(_T("HH:mm"));
	m_pvar_time_picker.SetTime(&TimeTemp);
	m_pvar_time_picker.ShowWindow(SW_HIDE);

	m_pvar_list.DeleteAllItems();
#if 1
	for (int i = 0; i < (int)m_pvar_data.size(); i++)
	{
		CString temp_item, temp_value, temp_cal, temp_filter, temp_status, temp_lable;
		CString temp_des;
		CString temp_units;

		if (i >= variable_item_limit_count)
			break;

		temp_item.Format(_T("Pvar%d"), i + 1);
		m_pvar_list.InsertItem(i, temp_item);

		if (ListCtrlEx::ComboBox == m_pvar_list.GetColumnType(VARIABLE_UNITE))
		{
			ListCtrlEx::CStrList strlist;

			for (int i = 0; i < (int)sizeof(Units_Type) / sizeof(Units_Type[0]); i++)
			{
				strlist.push_back(Units_Type[i]);
			}
			m_pvar_list.SetCellStringList(i, VARIABLE_UNITE, strlist);
		}

		for (int x = 0; x < VARIABLE_COL_NUMBER; x++)
		{
			if ((i % 2) == 0)
				m_pvar_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_pvar_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
		}

	}
#endif
	m_pvar_list.InitListData();
	m_pvar_list.ShowWindow(SW_SHOW);
}


LRESULT CBacnetPvar::Fresh_Pvar_List(WPARAM wParam, LPARAM lParam)
{

	int Fresh_Item;
	int isFreshOne = (int)lParam;

	if (isFreshOne == REFRESH_ON_ITEM)
	{
		Fresh_Item = (int)wParam;
	}
	else
	{
		if (m_pvar_list.IsDataNewer((char*)&m_pvar_data.at(0), sizeof(Str_variable_point) * BAC_VARIABLE_ITEM_COUNT))
		{
			// Avoid list flickering during refresh; don't refresh list when there's no data change
			//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
			m_pvar_list.SetListData((char*)&m_pvar_data.at(0), sizeof(Str_variable_point) * BAC_VARIABLE_ITEM_COUNT);
		}
		else
		{
			return 0;
		}
	}

	if (bacnet_device_type == PM_THIRD_PARTY_DEVICE) // for bacnet devices hiding columns
	{
		TCHAR szBuffer[80];
		LVCOLUMN col;
		col.mask = LVCF_TEXT;
		col.pszText = szBuffer;
		col.cchTextMax = 80;
		m_pvar_list.GetColumn(VARIABLE_AUTO_MANUAL, &col);
		col.pszText = _T("Out of Service");
		m_pvar_list.SetColumn(VARIABLE_AUTO_MANUAL, &col);


	}
	else { // to show the column for non-bacnet devices
		TCHAR szBuffer[80];
		LVCOLUMN col;
		col.mask = LVCF_TEXT;
		col.pszText = szBuffer;
		col.cchTextMax = 80;
		m_pvar_list.GetColumn(VARIABLE_AUTO_MANUAL, &col);
		col.pszText = _T("Auto/Manual");
		m_pvar_list.SetColumn(VARIABLE_AUTO_MANUAL, &col);

	}


	for (int i = 0; i < (int)m_pvar_data.size(); i++)
	{
		CString temp_item, temp_value, temp_cal, temp_filter, temp_status, temp_lable;
		CString temp_des;
		CString temp_units;

		if (isFreshOne)
		{
			i = Fresh_Item;
		}

		if (i >= variable_item_limit_count)
		{
			return 0;
		}

		MultiByteToWideChar(CP_ACP, 0, (char*)m_pvar_data.at(i).description, (int)strlen((char*)m_pvar_data.at(i).description) + 1,
			temp_des.GetBuffer(MAX_PATH), MAX_PATH);
		temp_des.ReleaseBuffer();
		temp_des = temp_des.Left(STR_VARIABLE_DESCRIPTION_LENGTH).Trim();
		m_pvar_list.SetItemText(i, VARIABLE_FULL_LABLE, temp_des);
		if (bacnet_device_type == PM_THIRD_PARTY_DEVICE)
		{
			if (m_pvar_data.at(i).auto_manual == 0)
			{
				m_pvar_list.SetItemText(i, VARIABLE_AUTO_MANUAL, _T("False"));
			}
			else
			{
				m_pvar_list.SetItemText(i, VARIABLE_AUTO_MANUAL, _T("True"));
			}
		}
		else {
			if (m_pvar_data.at(i).auto_manual == 0)
			{
				m_pvar_list.SetItemText(i, VARIABLE_AUTO_MANUAL, _T("Auto"));
			}
			else
			{
				m_pvar_list.SetItemText(i, VARIABLE_AUTO_MANUAL, _T("Manual"));
			}
		}


		if (m_pvar_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
		{

			if ((m_pvar_data.at(i).range == 0) || ((m_pvar_data.at(i).range > 30) && (m_pvar_data.at(i).range < 100)))
			{
				CString cstemp_value2;
				float temp_float_value1;
				temp_float_value1 = ((float)m_pvar_data.at(i).value) / 1000;
				cstemp_value2.Format(_T("%.3f"), temp_float_value1);
				m_pvar_list.SetItemText(i, VARIABLE_VALUE, cstemp_value2);
				m_pvar_list.SetItemText(i, VARIABLE_UNITE, Variable_Analog_Units_Array[0]);
			}
			else if ((m_pvar_data.at(i).range >= 101) && (m_pvar_data.at(i).range <= 104))
			{
				if (read_msv_table)
					m_pvar_list.SetItemText(i, VARIABLE_UNITE, Custom_Msv_Range[m_pvar_data.at(i).range - 101]);
				int get_name_ret = 0;
				CString cstemp_value2;
				float temp_float_value1;
				temp_float_value1 = ((float)m_pvar_data.at(i).value) / 1000;
				get_name_ret = Get_Msv_Item_Name(m_pvar_data.at(i).range - 101, (int)temp_float_value1, cstemp_value2);
				if (get_name_ret < 0)  //若没有找到对应 就默认显示 浮点数;
					cstemp_value2.Format(_T("%.3f"), temp_float_value1);
				m_pvar_list.SetItemText(i, VARIABLE_VALUE, cstemp_value2);
			}
			else
			{
				CString temp1;
				CStringArray temparray;

				if ((m_pvar_data.at(i).range < 23) && (m_pvar_data.at(i).range != 0))
					temp1 = Digital_Units_Array[m_pvar_data.at(i).range];
				else if ((m_pvar_data.at(i).range >= 23) && (m_pvar_data.at(i).range <= 30))
				{
					if (receive_customer_unit)
						temp1 = Custom_Digital_Range[m_pvar_data.at(i).range - 23];
				}
				else
				{
					temp1 = Digital_Units_Array[0];
					m_pvar_list.SetItemText(i, VARIABLE_UNITE, temp1);
				}

				SplitCStringA(temparray, temp1, _T("/"));
				if ((temparray.GetSize() == 2))
				{
					if (m_pvar_data.at(i).control == 0)
						m_pvar_list.SetItemText(i, VARIABLE_VALUE, temparray.GetAt(0));
					else
						m_pvar_list.SetItemText(i, VARIABLE_VALUE, temparray.GetAt(1));
					m_pvar_list.SetItemText(i, VARIABLE_UNITE, temp1);
				}

			}
		}
		else
		{
			// If it's a time value
			if (m_pvar_data.at(i).range == 20)	//如果是时间;
			{
				m_pvar_list.SetItemText(i, VARIABLE_UNITE, Variable_Analog_Units_Array[m_pvar_data.at(i).range]);
				char temp_char[50];
				CString temp_11;
				//if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub < 620)
				//{			
				int time_seconds = m_pvar_data.at(i).value / 1000;
				intervaltotextfull(temp_char, time_seconds, 0, 0);

				//}
				//else
				//{
				//	int time_seconds = m_pvar_data.at(i).value;
				//	intervaltotext_2022_full(temp_char, time_seconds, 0);
				//}
				MultiByteToWideChar(CP_ACP, 0, temp_char, strlen(temp_char) + 1,
					temp_11.GetBuffer(MAX_PATH), MAX_PATH);
				temp_11.ReleaseBuffer();
				temp_11 = temp_11.Left(STR_VARIABLE_DESCRIPTION_LENGTH).Trim();

				m_pvar_list.SetItemText(i, VARIABLE_VALUE, temp_11);

			}

			else if (m_pvar_data.at(i).range < sizeof(Variable_Analog_Units_Array) / sizeof(Variable_Analog_Units_Array[0]))
			{
				m_pvar_list.SetItemText(i, VARIABLE_UNITE, Variable_Analog_Units_Array[m_pvar_data.at(i).range]);

				CString cstemp_value;
				float temp_float_value;
				temp_float_value = ((float)m_pvar_data.at(i).value) / 1000;
				cstemp_value.Format(_T("%.3f"), temp_float_value);
				m_pvar_list.SetItemText(i, VARIABLE_VALUE, cstemp_value);
			}
			else if ((m_pvar_data.at(i).range >= 34) && (m_pvar_data.at(i).range <= 38))
			{
				m_pvar_list.SetItemText(i, VARIABLE_UNITE, Analog_Variable_Units[m_pvar_data.at(i).range - 34]);

				CString cstemp_value;
				float temp_float_value;
				temp_float_value = ((float)m_pvar_data.at(i).value) / 1000;
				cstemp_value.Format(_T("%.3f"), temp_float_value);
				m_pvar_list.SetItemText(i, VARIABLE_VALUE, cstemp_value);
			}
			else if ((m_pvar_data.at(i).range >= 101) && (m_pvar_data.at(i).range <= 104))
			{
				if (read_msv_table)
					m_pvar_list.SetItemText(i, VARIABLE_UNITE, Custom_Msv_Range[m_pvar_data.at(i).range - 101]);
				int get_name_ret = 0;
				CString cstemp_value2;
				float temp_float_value1;
				temp_float_value1 = ((float)m_pvar_data.at(i).value) / 1000;
				get_name_ret = Get_Msv_Item_Name(m_pvar_data.at(i).range - 101, (int)temp_float_value1, cstemp_value2);
				if (get_name_ret < 0)  //若没有找到对应 就默认显示 浮点数;
					cstemp_value2.Format(_T("%.3f"), temp_float_value1);
				m_pvar_list.SetItemText(i, VARIABLE_VALUE, cstemp_value2);
			}
			else if ((m_pvar_data.at(i).range >= 101) && (m_pvar_data.at(i).range <= 104))
			{
				if (read_msv_table)
					m_pvar_list.SetItemText(i, VARIABLE_UNITE, Custom_Msv_Range[m_pvar_data.at(i).range - 101]);
				int get_name_ret = 0;
				CString cstemp_value2;
				float temp_float_value1;
				temp_float_value1 = ((float)m_pvar_data.at(i).value) / 1000;
				get_name_ret = Get_Msv_Item_Name(m_pvar_data.at(i).range - 101, (int)temp_float_value1, cstemp_value2);
				if (get_name_ret < 0)  //若没有找到对应 就默认显示 浮点数;
					cstemp_value2.Format(_T("%.3f"), temp_float_value1);
				m_pvar_list.SetItemText(i, VARIABLE_VALUE, cstemp_value2);
			}
			else
			{
				m_pvar_list.SetItemText(i, VARIABLE_UNITE, Variable_Analog_Units_Array[0]);

				CString cstemp_value;
				float temp_float_value;
				temp_float_value = ((float)m_pvar_data.at(i).value) / 1000;
				cstemp_value.Format(_T("%.3f"), temp_float_value);
				m_pvar_list.SetItemText(i, VARIABLE_VALUE, cstemp_value);
			}

		}
		if (bacnet_device_type == PM_THIRD_PARTY_DEVICE)
		{
			CString varunit;
			MultiByteToWideChar(CP_ACP, 0, (char*)bacnet_engineering_unit_names[m_pvar_data.at(i).range].pString,
				(int)strlen((char*)bacnet_engineering_unit_names[m_pvar_data.at(i).range].pString) + 1,
				varunit.GetBuffer(MAX_PATH), MAX_PATH);
			varunit.ReleaseBuffer();
			m_pvar_list.SetItemText(i, VARIABLE_UNITE, varunit);
		}

		CString temp_des2;
		MultiByteToWideChar(CP_ACP, 0, (char*)m_pvar_data.at(i).label, (int)strlen((char*)m_pvar_data.at(i).label) + 1,
			temp_des2.GetBuffer(MAX_PATH), MAX_PATH);
		temp_des2.ReleaseBuffer();
		temp_des2 = temp_des2.Left(STR_VARIABLE_LABEL).Trim();
		m_pvar_list.SetItemText(i, VARIABLE_LABLE, temp_des2);


		if (isFreshOne)
		{
			break;
		}

	}
	copy_data_to_ptrpanel(TYPE_VARIABLE);
#ifdef LOCAL_DB_FUNCTION
	if (selected_product_Node.serial_number != 0)
		WriteDeviceDataIntoAccessDB(BAC_VAR, variable_item_limit_count, selected_product_Node.serial_number);
#endif
	return 0;
}
LRESULT CBacnetPvar::Fresh_Pvar_Item(WPARAM wParam,LPARAM lParam)
{
	int cmp_ret ;//compare if match it will 0;

	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;
	CString temp_task_info;
	CString New_CString =  m_pvar_list.GetItemText(Changed_Item,Changed_SubItem);
	CString cstemp_value;
	memcpy_s(&m_temp_pvar_data[Changed_Item],sizeof(Str_variable_point),&m_pvar_data.at(Changed_Item),sizeof(Str_variable_point));


	if(Changed_SubItem == VARIABLE_LABLE)
	{
		CString cs_temp = m_pvar_list.GetItemText(Changed_Item,Changed_SubItem);
		// Length cannot exceed structure definition length
		if(cs_temp.GetLength()>= STR_VARIABLE_LABEL)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not higher than 9"),_T("Warning"));

#pragma region note_what_do	
			// The following actions achieve, after the popup, continue to select the customer input part, Mr. Mao's requirement
			//如下所做的事 达到 ,在弹窗后 继续选中客户输入的部分 ，毛总的要求;
			CRect list_rect,win_rect;
			m_pvar_list.GetWindowRect(list_rect);
			ScreenToClient(&list_rect);
			::GetWindowRect(m_variable_dlg_hwnd,win_rect);
			m_pvar_list.Set_My_WindowRect(win_rect);
			m_pvar_list.Set_My_ListRect(list_rect);

			m_pvar_list.Get_clicked_mouse_position();
#pragma endregion note_what_do

			PostMessage(WM_REFRESH_BAC_PVAR_LIST,NULL,NULL);
			return 0;
		}
		cs_temp.Replace(_T("-"),_T("_"));
		cs_temp.MakeUpper();
		if(Check_Label_Exsit(cs_temp))
		{
			PostMessage(WM_REFRESH_BAC_PVAR_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0;
		}
		
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_pvar_data.at(Changed_Item).label,STR_VARIABLE_LABEL,cTemp1,STR_VARIABLE_LABEL);
	}

	if(Changed_SubItem == VARIABLE_FULL_LABLE)
	{
		CString cs_temp = m_pvar_list.GetItemText(Changed_Item,Changed_SubItem);
		// Length cannot exceed structure definition length
		if(cs_temp.GetLength()>= STR_VARIABLE_DESCRIPTION_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not higher than 20"),_T("Warning"));
#pragma region note_what_do	
			// The following actions achieve, after the popup, continue to select the customer input part, Mr. Mao's requirement
			//如下所做的事 达到 ,在弹窗后 继续选中客户输入的部分 ，毛总的要求;
			CRect list_rect,win_rect;
			m_pvar_list.GetWindowRect(list_rect);
			ScreenToClient(&list_rect);
			::GetWindowRect(m_variable_dlg_hwnd,win_rect);
			m_pvar_list.Set_My_WindowRect(win_rect);
			m_pvar_list.Set_My_ListRect(list_rect);

			m_pvar_list.Get_clicked_mouse_position();
#pragma endregion note_what_do
			PostMessage(WM_REFRESH_BAC_PVAR_LIST,NULL,NULL);
			return 0;
		}
		if(Check_FullLabel_Exsit(cs_temp))
		{
			PostMessage(WM_REFRESH_BAC_PVAR_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0;
		}
		//if (bacnet_device_type == PM_THIRD_PARTY_DEVICE) // handled the full label changes for third party bacnet device
		//{
		//	BACNET_APPLICATION_DATA_VALUE* temp_value = new BACNET_APPLICATION_DATA_VALUE();
		//	temp_value->tag = TPYE_BACAPP_CHARACTER_STRING;
		//	temp_value->context_specific = false;
		//	WideCharToMultiByte(CP_ACP, 0, cs_temp.GetBuffer(), -1, temp_value->type.Character_String.value, MAX_CHARACTER_STRING_BYTES, NULL, NULL);
		//	temp_value->type.Character_String.encoding = 0;
		//	temp_value->type.Character_String.length = cs_temp.GetLength() + 1;
		//	int ObjectType = OBJECT_ANALOG_INPUT;
		//	if (m_pvar_data.at(Changed_Item).digital_analog == BAC_UNITS_DIGITAL)
		//		ObjectType = OBJECT_BINARY_INPUT;
		//	int invoke_id = Bacnet_Write_Properties_Blocking(g_bac_instance, (BACNET_OBJECT_TYPE)ObjectType, m_pvar_data_instance.at(Changed_Item), PROP_OBJECT_NAME, temp_value);
		//}
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_pvar_data.at(Changed_Item).description,STR_VARIABLE_DESCRIPTION_LENGTH,cTemp1,STR_VARIABLE_DESCRIPTION_LENGTH);
	}


	if(Changed_SubItem == VARIABLE_AUTO_MANUAL)
	{
		CString temp_cs = m_pvar_list.GetItemText(Changed_Item,Changed_SubItem);
		if(temp_cs.CompareNoCase(_T("Auto"))==0 || temp_cs.CompareNoCase(_T("False")) == 0 )
		{
			//m_pvar_list.SetCellEnabled(Changed_Item,VARIABLE_VALUE,0);
			m_pvar_data.at(Changed_Item).auto_manual = BAC_AUTO;
		}
		else
		{
			//m_pvar_list.SetCellEnabled(Changed_Item,VARIABLE_VALUE,1);
			m_pvar_data.at(Changed_Item).auto_manual = BAC_MANUAL;
		}
	}
	// Only handle ANALOG values here, DIGITAL values are already handled in Click event
	if(Changed_SubItem == VARIABLE_VALUE)//这里只用处理 ANALOG 的值就看要了， DIGITAL 的值在Click 事件中处理过了;
	{
		CString temp_cs = m_pvar_list.GetItemText(Changed_Item,Changed_SubItem);

		int temp_int = (int)(_wtof(temp_cs) * 1000);
		m_pvar_data.at(Changed_Item).value = temp_int;
	}

	cmp_ret = memcmp(&m_temp_pvar_data[Changed_Item],&m_pvar_data.at(Changed_Item),sizeof(Str_variable_point));
	if(cmp_ret!=0)
	{
		m_pvar_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
	temp_task_info.Format(_T("Write program variable List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
	Post_Write_Message(g_bac_instance, WRITE_PVARIABLE_T3000,Changed_Item,Changed_Item,sizeof(Str_variable_point),m_variable_dlg_hwnd ,temp_task_info,Changed_Item,Changed_SubItem);
	}
	return 0;
}


void CBacnetPvar::OnBnClickedButtonVariableApply()
{
	
	for (int i=0;i<(int)m_pvar_data.size();i++)
	{
		CString cs_temp=m_pvar_list.GetItemText(i,VARIABLE_FULL_LABLE);
		char cTemp[255];
		memset(cTemp,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp, 255, NULL, NULL );
		memcpy_s(m_pvar_data.at(i).description,21,cTemp,21);

		cs_temp=m_pvar_list.GetItemText(i,VARIABLE_AUTO_MANUAL);
		if(cs_temp.CompareNoCase(_T("Auto"))==0)
		{
			m_pvar_data.at(i).auto_manual= BAC_AUTO;
		}
		else
		{
			m_pvar_data.at(i).auto_manual = BAC_MANUAL;
		}


		cs_temp=m_pvar_list.GetItemText(i,VARIABLE_UNITE);
		int index_number=0;

		cs_temp=m_pvar_list.GetItemText(i,VARIABLE_LABLE);
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_pvar_data.at(i).label,STR_IN_LABEL,cTemp1,STR_IN_LABEL);
	}
	Post_Write_Message(g_bac_instance,WRITEVARIABLE_T3000,0,19,sizeof(Str_variable_point),BacNet_hwd);
		
}


void CBacnetPvar::OnNMClickListVariable(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	long lRow,lCol;
	m_pvar_list.Set_Edit(true);
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos));
	m_pvar_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_pvar_list.SubItemHitTest(&lvinfo);

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;


	if(lRow>m_pvar_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow<0)
		return;



	CString temp1;
	m_row = lRow;
	m_col = lCol;

	memcpy_s(&m_temp_pvar_data[lRow],sizeof(Str_variable_point),&m_pvar_data.at(lRow),sizeof(Str_variable_point));
	CString New_CString;
	CString temp_task_info;
	if((lCol == VARIABLE_VALUE) &&(m_pvar_data.at(lRow).digital_analog == BAC_UNITS_DIGITAL ) && (m_pvar_data.at(lRow).auto_manual != BAC_AUTO))
	{
		
		
		CString temp1;
		CStringArray temparray;

		if((m_pvar_data.at(lRow).range < 23) &&(m_pvar_data.at(lRow).range !=0))
			temp1 = Digital_Units_Array[m_pvar_data.at(lRow).range];
		else if((m_pvar_data.at(lRow).range >=23) && (m_pvar_data.at(lRow).range <= 30))
		{
			if(receive_customer_unit)
				temp1 = Custom_Digital_Range[m_pvar_data.at(lRow).range - 23];
			else
			{
				m_pvar_list.Set_Edit(false);
				return;
			}
		}
		else
			return;
		SplitCStringA(temparray,temp1,_T("/"));



		if(m_pvar_data.at(lRow).control == 0)
		{
			m_pvar_data.at(lRow).control = 1;
			m_pvar_list.SetItemText(lRow,VARIABLE_VALUE,temparray.GetAt(1));
			New_CString = temparray.GetAt(1);
		}
		else
		{
			m_pvar_data.at(lRow).control = 0;
			m_pvar_list.SetItemText(lRow,VARIABLE_VALUE,temparray.GetAt(0));
			New_CString = temparray.GetAt(0);
		}
		m_pvar_list.Set_Edit(false);

		temp_task_info.Format(_T("Write Variable List Item%d .Changed to \"%s\" "),lRow + 1,New_CString);
	}
	else if((lCol == VARIABLE_VALUE) &&(m_pvar_data.at(lRow).digital_analog == BAC_UNITS_ANALOG ) && (m_pvar_data.at(lRow).auto_manual == BAC_MANUAL) && (m_pvar_data.at(lRow).range == 20))
	{
		m_pvar_list.Set_Edit(false);
		m_pvar_time_picker.ShowWindow(SW_SHOW);
		CRect list_rect,win_rect;
		m_pvar_list.GetWindowRect(list_rect);
		GetWindowRect(win_rect);
		CRect myrect;
		m_pvar_list.GetSubItemRect(lRow,lCol,LVIR_BOUNDS,myrect);


		myrect.left = myrect.left + list_rect.left - win_rect.left  - 6 ;
		myrect.right = myrect.right + list_rect.left - win_rect.left - 6;
		myrect.top = myrect.top -1 ;
		myrect.bottom = myrect.bottom + 3;
		m_pvar_time_picker.BringWindowToTop();
		m_pvar_time_picker.MoveWindow(myrect);

		
		CString Temp_CString =  m_pvar_list.GetItemText(lRow,lCol);

		CStringArray TEMPCS;
		int temp_hour,temp_minute,temp_second;
		SplitCStringA(TEMPCS, Temp_CString, _T(":"));
		if((int)TEMPCS.GetSize() <3)
		{
			temp_hour = 0;
			temp_minute = 0;
			temp_second = 0;
			CTime TimeTemp(2017,1,1,temp_hour,temp_minute,0);
			m_pvar_time_picker.SetFormat(_T("HH:mm:ss"));
			m_pvar_time_picker.SetTime(&TimeTemp);
			m_pvar_time_picker.SetFocus();
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
			m_pvar_time_picker.SetFormat(_T("HH:mm:ss"));
			m_pvar_time_picker.SetTime(&TimeTemp);
			m_pvar_time_picker.SetFocus();
		}
		m_pvar_list.SetItemText(lRow,lCol,_T(""));
		m_pvar_time_picker.Invalidate();
		SetTimer(2,100,NULL);
	}
    else if ((lCol == VARIABLE_VALUE) && 
              (m_pvar_data.at(lRow).digital_analog == BAC_UNITS_ANALOG) && 
              (m_pvar_data.at(lRow).auto_manual == BAC_MANUAL) 
              && ((m_pvar_data.at(lRow).range == 101) || 
				  (m_pvar_data.at(lRow).range == 102) || 
				  (m_pvar_data.at(lRow).range == 103) ||
				  (m_pvar_data.at(lRow).range == 104)))
    {
        m_pvar_list.Set_Edit(false);
        int range_index = m_pvar_data.at(lRow).range - 101;
        CString cstempNextItemString;
        int ntempNextValue;
        int find_ret = 0;
        find_ret = Get_Msv_next_Name_and_Value_BySearchValue(range_index,m_pvar_data.at(lRow).value/1000, cstempNextItemString, ntempNextValue);;
        if (find_ret >= 0)
        {
            m_pvar_data.at(lRow).value = ntempNextValue * 1000;
            m_pvar_list.SetItemText(lRow, VARIABLE_VALUE, cstempNextItemString);
        }
		else if(find_ret == -2)
		{
			m_pvar_data.at(lRow).value = m_msv_data.at(range_index).msv_data[0].msv_value * 1000;
			find_ret = Get_Msv_next_Name_and_Value_BySearchValue(range_index, m_pvar_data.at(lRow).value / 1000, cstempNextItemString, ntempNextValue);
			if (find_ret >= 0)
			{
				m_pvar_data.at(lRow).value = ntempNextValue * 1000;
				m_pvar_list.SetItemText(lRow, VARIABLE_VALUE, cstempNextItemString);
			}
		}

        //Custom_Msv_Range[range_index]
    }
	else if(lCol == VARIABLE_VALUE)
	{
		if(m_pvar_data.at(lRow).auto_manual == BAC_AUTO)
		{
			m_pvar_list.Set_Edit(false);
			return;
		}
	}
	else if(lCol == VARIABLE_AUTO_MANUAL)
	{
		//if (bacnet_device_type == PM_THIRD_PARTY_DEVICE) // handled the full label changes for third party bacnet device
		//{
		//	if (m_pvar_data.at(lRow).auto_manual == 0)
		//	{

		//		m_pvar_data.at(lRow).auto_manual = 1;
		//		m_pvar_list.SetItemText(lRow, VARIABLE_AUTO_MANUAL, _T("True"));
		//	}
		//	else
		//	{
		//		m_pvar_data.at(lRow).auto_manual = 0;
		//		m_pvar_list.SetItemText(lRow, VARIABLE_AUTO_MANUAL, _T("False"));
		//	}
		//	BACNET_APPLICATION_DATA_VALUE* temp_value = new BACNET_APPLICATION_DATA_VALUE();
		//	temp_value->tag = TPYE_BACAPP_BOOLEAN;
		//	temp_value->type.Boolean = m_pvar_data.at(lRow).auto_manual;
		//	int ObjectType = OBJECT_ANALOG_VALUE;
		//	if (m_pvar_data.at(lRow).digital_analog == BAC_UNITS_DIGITAL)
		//		ObjectType = OBJECT_BINARY_VALUE;
		//	int invoke_id = Bacnet_Write_Properties_Blocking(g_bac_instance, (BACNET_OBJECT_TYPE)ObjectType, m_pvar_data_instance.at(lRow), PROP_OUT_OF_SERVICE, temp_value);
		//}
		//else {
			if (m_pvar_data.at(lRow).auto_manual == 0)
			{
				m_pvar_data.at(lRow).auto_manual = 1;
				m_pvar_list.SetItemText(lRow, VARIABLE_AUTO_MANUAL, _T("Manual"));
				New_CString = _T("Manual");
			}
			else
			{
				m_pvar_data.at(lRow).auto_manual = 0;
				m_pvar_list.SetItemText(lRow, VARIABLE_AUTO_MANUAL, _T("Auto"));
				New_CString = _T("Auto");
			}
		//}
		
		
		temp_task_info.Format(_T("Write Variable List Item%d .Changed to \"%s\" "),lRow + 1,New_CString);
	}
	//else if (lCol == VARIABLE_UNITE && bacnet_device_type == PM_THIRD_PARTY_DEVICE) // for bacnet Thirdparty devices Units
	//{
	//CBacnetUnitsSelection unitDlg;
	//bac_range_number_choose = m_pvar_data.at(lRow).range;
	//unitDlg.DoModal();
	//if (!range_cancel)
	//{
	//	m_pvar_data.at(lRow).range = bac_range_number_choose;
	//	CString inputunit;
	//	MultiByteToWideChar(CP_ACP, 0, (char*)bacnet_engineering_unit_names[m_Output_data.at(lRow).range].pString,
	//		(int)strlen((char*)bacnet_engineering_unit_names[m_Output_data.at(lRow).range].pString) + 1,
	//		inputunit.GetBuffer(MAX_PATH), MAX_PATH);
	//	inputunit.ReleaseBuffer();
	//	m_pvar_list.SetItemText(lRow, VARIABLE_UNITE, inputunit);
	//	BACNET_APPLICATION_DATA_VALUE* temp_value = new BACNET_APPLICATION_DATA_VALUE();
	//	temp_value->tag = TPYE_BACAPP_UNSIGNED;
	//	temp_value->type.Enumerated = m_pvar_data.at(lRow).range;
	//	int ObjectType = OBJECT_ANALOG_VALUE;
	//	if (m_pvar_data.at(lRow).digital_analog == BAC_UNITS_DIGITAL)
	//		ObjectType = OBJECT_BINARY_VALUE;
	//	int invoke_id = Bacnet_Write_Properties_Blocking(g_bac_instance, (BACNET_OBJECT_TYPE)ObjectType, m_pvar_data_instance.at(lRow), PROP_PRESENT_VALUE, temp_value);
	//}
	//}
	else if(lCol == VARIABLE_UNITE)
	{

        CString temp_info;
		BacnetRange dlg;

		//点击产品的时候 需要读custom units，老的产品firmware 说不定没有 这些，所以不强迫要读到;
		if(!read_customer_unit)
		{

			int temp_invoke_id = -1;
			int send_status = true;
			int	resend_count = 0;
            if ((g_protocol == P_BACNET_MSTP) || (g_protocol == P_BACNET_IP) || (g_protocol == P_MODBUS_TCP))
            {
                if (GetPrivateData_Blocking(g_bac_instance, READUNIT_T3000, 0, BAC_CUSTOMER_UNITS_COUNT - 1, sizeof(Str_Units_element), 5) > 0)
                {
                    temp_info.Format(_T("Read digital custom units success."));
                    SetPaneString(BAC_SHOW_MISSION_RESULTS, temp_info);
                    read_customer_unit = true;
                }
                else
                {
                    temp_info.Format(_T("Read digital custom units success."));
                    SetPaneString(BAC_SHOW_MISSION_RESULTS, temp_info);
                }

            }
#if 0
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
#endif

		}


        if ((g_protocol == P_BACNET_MSTP) || (g_protocol == P_BACNET_IP) || (g_protocol == P_MODBUS_TCP))
        {
            if (GetPrivateData_Blocking(g_bac_instance, READVARUNIT_T3000, 0, 4, sizeof(Str_variable_uint_point)) > 0)
            {
                temp_info.Format(_T("Read variable custmer units success."));
                SetPaneString(BAC_SHOW_MISSION_RESULTS, temp_info);
            }
            else
            {
                temp_info.Format(_T("Read variable custmer units success."));
                SetPaneString(BAC_SHOW_MISSION_RESULTS, temp_info);
            }
        }

		if(m_pvar_data.at(lRow).digital_analog == BAC_UNITS_ANALOG)
		{
			bac_ranges_type = VARIABLE_RANGE_ANALOG_TYPE;
			if(  ( m_pvar_data.at(lRow).range > ((sizeof(Variable_Analog_Units_Array) / sizeof(Variable_Analog_Units_Array[0])) ) + 6) &&
                 (m_pvar_data.at(lRow).range != 101) && 
				 (m_pvar_data.at(lRow).range != 102) && 
				 (m_pvar_data.at(lRow).range != 103) &&
				 (m_pvar_data.at(lRow).range != 104)
                ) 
			{
				m_pvar_data.at(lRow).range = 0;
				bac_range_number_choose = 0;
			}
		}
		else
		{
			bac_ranges_type = VARIABLE_RANGE_DIGITAL_TYPE;
			if(bac_Invalid_range(m_pvar_data.at(lRow).range))
			{
				m_pvar_data.at(lRow).range = 0;
				bac_range_number_choose = 0;
			}
		}


			initial_dialog = 1;
			bac_range_number_choose = m_pvar_data.at(lRow).range;
			dlg.DoModal();
			if(range_cancel)
			{
				PostMessage(WM_REFRESH_BAC_PVAR_LIST,lRow,REFRESH_ON_ITEM);//这里调用 刷新线程重新刷新会方便一点;
				return ;
			}
			if(bac_range_number_choose == 0)	//如果选择的是 unused 就认为是analog 的unused;这样 能显示对应的value;
			{
				m_pvar_data.at(lRow).digital_analog =  BAC_UNITS_ANALOG;
				bac_ranges_type = VARIABLE_RANGE_ANALOG_TYPE;
			}

			if(bac_ranges_type == VARIABLE_RANGE_ANALOG_TYPE)
			{
				m_pvar_data.at(lRow).digital_analog = BAC_UNITS_ANALOG;
				m_pvar_data.at(lRow).range = bac_range_number_choose;
				if((bac_range_number_choose >= 34) && (bac_range_number_choose <= 38))
				{
					m_pvar_list.SetItemText(lRow,lCol,Analog_Variable_Units[bac_range_number_choose - 34]);
				}
				else
				m_pvar_list.SetItemText(lRow,lCol,Variable_Analog_Units_Array[bac_range_number_choose]);



				if(m_pvar_data.at(lRow).range == 20)	//如果是时间;
				{
					char temp_char[50];
					//if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub < 620)
					//{
						int time_seconds = m_pvar_data.at(lRow).value / 1000;
						intervaltotextfull(temp_char, time_seconds, 0, 0);

					//}
					//else
					//{
					//	int time_seconds = m_pvar_data.at(lRow).value;
					//	intervaltotext_2022_full(temp_char, time_seconds, 0);
					//}
					CString temp_11;
					MultiByteToWideChar(CP_ACP, 0, temp_char, strlen(temp_char) + 1,
						temp_11.GetBuffer(MAX_PATH), MAX_PATH);
					temp_11.ReleaseBuffer();
					m_pvar_list.SetItemText(lRow, VARIABLE_VALUE, temp_11);


				}
				else
				{
					CString cstemp_value;
					float temp_float_value;
					temp_float_value = ((float)m_pvar_data.at(lRow).value) / 1000;
					cstemp_value.Format(_T("%.3f"),temp_float_value);
					m_pvar_list.SetItemText(lRow,VARIABLE_VALUE,cstemp_value);

					/*				cstemp_value.Format(_T("%d"),m_pvar_data.at(Changed_Item).value);
					m_pvar_list.SetItemText(Changed_Item,VARIABLE_VALUE,cstemp_value);*/	
				}
			}
			else if((bac_ranges_type == VARIABLE_RANGE_DIGITAL_TYPE) || (bac_ranges_type == INPUT_RANGE_DIGITAL_TYPE) || (bac_ranges_type == OUTPUT_RANGE_DIGITAL_TYPE))
			{
				m_pvar_data.at(lRow).digital_analog = BAC_UNITS_DIGITAL;
				m_pvar_data.at(lRow).range = bac_range_number_choose;


				CStringArray temparray;
                if ((bac_range_number_choose >= 23) && (bac_range_number_choose <= 30))
                {
                    //temp1.Format(_T("%s"), Custom_Digital_Range[bac_range_number_choose - 23]);
                    temp1 = Custom_Digital_Range[bac_range_number_choose - 23];
                }
                else if (bac_range_number_choose < 23)
                    temp1 = Digital_Units_Array[bac_range_number_choose];//22 is the sizeof the array
                else if((bac_range_number_choose >= 101) && (bac_range_number_choose <= 104))
                {
                        if (read_msv_table)
                            temp1 = Custom_Msv_Range[bac_range_number_choose - 101];
                        else
                            temp1 = _T("MSV");
                        m_pvar_data.at(lRow).digital_analog = BAC_UNITS_ANALOG;
                }
                    
				SplitCStringA(temparray,temp1,_T("/"));





				if(m_pvar_data.at(lRow).control == 1)
				{
					if((temparray.GetSize()==2)&&(!temparray.GetAt(1).IsEmpty()))
					{
						m_pvar_list.SetItemText(lRow,VARIABLE_VALUE,temparray.GetAt(1));
					}
				}
				else
				{
					if((temparray.GetSize()==2)&&(!temparray.GetAt(0).IsEmpty()))
					{
						m_pvar_list.SetItemText(lRow,VARIABLE_VALUE,temparray.GetAt(0));
					}			
				}
				m_pvar_list.SetItemText(lRow,VARIABLE_UNITE,temp1);
				
			}
			//if (bacnet_device_type == PM_THIRD_PARTY_DEVICE) // handled the full label changes for third party bacnet device
			//{
			//	CString cs_temp = m_pvar_list.GetItemText(lRow, VARIABLE_UNITE);
			//	BACNET_APPLICATION_DATA_VALUE* temp_value = new BACNET_APPLICATION_DATA_VALUE();
			//	temp_value->tag = TPYE_BACAPP_UNSIGNED;
			//	temp_value->type.Unsigned_Int = (int)_wtof(cs_temp);
			//	int ObjectType = OBJECT_ANALOG_VALUE;
			//	if (m_Output_data.at(lRow).digital_analog == BAC_UNITS_DIGITAL)
			//		ObjectType = OBJECT_BINARY_VALUE;
			//	int invoke_id = Bacnet_Write_Properties_Blocking(g_bac_instance, (BACNET_OBJECT_TYPE)ObjectType, m_pvar_data_instance.at(lRow), PROP_PRESENT_VALUE, temp_value);
			//}
	}
	else if (lCol == VARIABLE_FULL_LABLE)
	{
		CString cs_temp = m_pvar_list.GetItemText(lRow, VARIABLE_FULL_LABLE);
		if (strcmp((char*)cs_temp.GetBuffer(cs_temp.GetLength()), (char*)m_pvar_data.at(lRow).description) == 0)
		{
			//if (bacnet_device_type == PM_THIRD_PARTY_DEVICE) // handled the full label changes for third party bacnet device
			//{
			//	BACNET_APPLICATION_DATA_VALUE* temp_value = new BACNET_APPLICATION_DATA_VALUE();
			//		temp_value->tag = TPYE_BACAPP_CHARACTER_STRING;
			//		temp_value->context_specific = false;
			//		WideCharToMultiByte(CP_ACP, 0, cs_temp.GetBuffer(), -1, temp_value->type.Character_String.value, MAX_CHARACTER_STRING_BYTES, NULL, NULL);
			//		temp_value->type.Character_String.encoding = 0;
			//		temp_value->type.Character_String.length = cs_temp.GetLength() + 1;
			//		int ObjectType = OBJECT_ANALOG_VALUE;
			//		if (m_Output_data.at(lRow).digital_analog == BAC_UNITS_DIGITAL)
			//			ObjectType = OBJECT_BINARY_VALUE;

			//	int invoke_id = Bacnet_Write_Properties_Blocking(g_bac_instance, (BACNET_OBJECT_TYPE)ObjectType, m_pvar_data_instance.at(lRow), PROP_OBJECT_NAME, temp_value);
			//}
		}
	}
	else if (lCol == VARIABLE_LABLE)
	{
		CString cs_temp = m_pvar_list.GetItemText(lRow, VARIABLE_LABLE);
		if (strcmp((char*)cs_temp.GetBuffer(cs_temp.GetLength()), (char*)m_pvar_data.at(lRow).label) == 0)
		{
			//if (bacnet_device_type == PM_THIRD_PARTY_DEVICE) // handled the full label changes for third party bacnet device
			//{
			//	BACNET_APPLICATION_DATA_VALUE* temp_value = new BACNET_APPLICATION_DATA_VALUE();
			//	temp_value->tag = TPYE_BACAPP_CHARACTER_STRING;
			//	temp_value->context_specific = false;
			//	WideCharToMultiByte(CP_ACP, 0, cs_temp.GetBuffer(), -1, temp_value->type.Character_String.value, MAX_CHARACTER_STRING_BYTES, NULL, NULL);
			//	temp_value->type.Character_String.encoding = 0;
			//	temp_value->type.Character_String.length = cs_temp.GetLength() + 1;
			//	int ObjectType = OBJECT_ANALOG_VALUE;
			//	if (m_Output_data.at(lRow).digital_analog == BAC_UNITS_DIGITAL)
			//		ObjectType = OBJECT_BINARY_VALUE;

			//	int invoke_id = Bacnet_Write_Properties_Blocking(g_bac_instance, (BACNET_OBJECT_TYPE)ObjectType, m_pvar_data_instance.at(lRow), PROP_DESCRIPTION, temp_value);
			//}
		}
	}
	else
	{
		return;
	}

	int cmp_ret = memcmp(&m_temp_pvar_data[lRow],&m_pvar_data.at(lRow),sizeof(Str_variable_point));
	if(cmp_ret!=0)
	{
		m_pvar_list.SetItemBkColor(lRow,lCol,LIST_ITEM_CHANGED_BKCOLOR);
		Post_Write_Message(g_bac_instance,WRITEVARIABLE_T3000,lRow,lRow,sizeof(Str_variable_point),m_variable_dlg_hwnd,temp_task_info,lRow,lCol);
	}
	*pResult = 0;
}





void CBacnetPvar::OnTimer(UINT_PTR nIDEvent)
{
	 
	switch(nIDEvent)
	{
	case 1:
		{
            if ((bacnet_view_number == TYPE_VARIABLE) && (g_protocol == PROTOCOL_BIP_TO_MSTP))
			{
				PostMessage(WM_REFRESH_BAC_PVAR_LIST,NULL,NULL);
			}
			else if((bacnet_view_number == TYPE_VARIABLE) && (Gsm_communication == false) &&  ((this->m_hWnd  == ::GetActiveWindow()) || (bacnet_view_number == TYPE_VARIABLE))  )	//GSM连接时不要刷新;
			{
			::PostMessage(m_variable_dlg_hwnd,WM_REFRESH_BAC_PVAR_LIST,NULL,NULL);
			if(bac_select_device_online)
				Post_Refresh_Message(g_bac_instance,READVARIABLE_T3000,0,BAC_VARIABLE_ITEM_COUNT - 1,sizeof(Str_variable_point),BAC_VARIABLE_GROUP);
			}
		}
		break;
	case 2:
		{
			KillTimer(2);
			m_pvar_time_picker.Invalidate();
		}
		break;
	case 3:
		{
			//在更改某一列之后要在读取此列的值，并刷新此列;

			if(this->IsWindowVisible())
				PostMessage(WM_REFRESH_BAC_PVAR_LIST,NULL,NULL);
			KillTimer(3);

		}
		break;
	case 4:
		if ((SPECIAL_BAC_TO_MODBUS) && (bacnet_view_number == TYPE_VARIABLE) && (Bacnet_Private_Device(selected_product_Node.product_class_id)))
		{
			Post_Refresh_Message(g_bac_instance, READVARIABLE_T3000, 0, BAC_VARIABLE_ITEM_COUNT - 1, sizeof(Str_variable_point), 0);
		}
		
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CBacnetPvar::OnNMKillfocusDatetimepicker2Variable(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	CTime temp_time;CString temp_cs;
	int chour,cmin,csend;
	m_pvar_time_picker.GetTime(temp_time);
	chour = temp_time.GetHour();
	cmin = temp_time.GetMinute();
	csend = temp_time.GetSecond();


	if((chour == 0) &&(cmin	==0) && (csend == 0))
	{
		temp_cs.Empty();
	}
	else
		temp_cs.Format(_T("%02d:%02d:%02d"),chour,cmin,csend);
	m_pvar_list.SetItemText(m_row,m_col,temp_cs);

	m_pvar_time_picker.ShowWindow(SW_HIDE);

	int write_value;
	//if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub < 620)
	//{
		write_value = (chour * 3600 + cmin * 60 + csend) * 1000;
	//}
	//else
	//{
	//	write_value = (chour * 100 * 1000 + cmin * 1000 + csend * 10);
	//}

	m_pvar_data.at(m_row).value = write_value;
	CString temp_task_info;
	temp_task_info.Format(_T("Write Variable Time Item%d .Changed Time to \"%s\" "),m_row + 1,temp_cs);
	Post_Write_Message(g_bac_instance,WRITEVARIABLE_T3000,m_row,m_row,sizeof(Str_variable_point),BacNet_hwd,temp_task_info);



	*pResult = 0;
}


BOOL CBacnetPvar::PreTranslateMessage(MSG* pMsg)
{
	
	if((pMsg->message == WM_KEYDOWN  ) && (pMsg->wParam == VK_RETURN))
	{
			CRect list_rect,win_rect;
			m_pvar_list.GetWindowRect(list_rect);
			ScreenToClient(&list_rect);
			::GetWindowRect(m_variable_dlg_hwnd,win_rect);
			m_pvar_list.Set_My_WindowRect(win_rect);
			m_pvar_list.Set_My_ListRect(list_rect);

			m_pvar_list.Get_clicked_mouse_position();

			CWnd *temp_focus=GetFocus();	//Maurice require ,click enter and the cursor still in this edit or combobox.
			GetDlgItem(IDC_BUTTON_VARIABLE_READ)->SetFocus();
			temp_focus->SetFocus();
			return 1;
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
	else if ((pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F2)) //老毛要求按F2立刻刷新值;
	{
		::PostMessage(BacNet_hwd, WM_FRESH_CM_LIST, MENU_CLICK, TYPE_VARIABLE);
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetPvar::OnClose()
{
	 
	ShowWindow(FALSE);
	return;
	KillTimer(1);
	m_variable_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	CDialogEx::OnClose();
}


void CBacnetPvar::OnCancel()
{
	
	//KillTimer(1);
	//m_variable_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	//CDialogEx::OnCancel();
}


BOOL CBacnetPvar::OnHelpInfo(HELPINFO* pHelpInfo)
{
	HWND hWnd;
	if (pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
	else
		hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_6_5_VARIABLES);
	return (hWnd != NULL);
	return CDialogEx::OnHelpInfo(pHelpInfo);
}


void CBacnetPvar::Reset_Pvar_Rect()
{

	CRect temp_mynew_rect;
	::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;

	CRect temp_window;
	GetWindowRect(&temp_window);

	if(window_max)
	{
		CRect temp_mynew_rect;
		::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height() - DELTA_HEIGHT, NULL);
	}
	else if((temp_window.Width() <= temp_mynew_rect.Width() ) && (temp_window.Height() <= temp_mynew_rect.Height()))
	{
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,0,0,SWP_NOSIZE );
	}
	else
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left + 60,temp_mynew_rect.top + 60,700,700, NULL);


	return;

}


void CBacnetPvar::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	
	CRect rc;
	GetClientRect(rc);
	if(m_pvar_list.m_hWnd != NULL)
	{
		::SetWindowPos(this->m_hWnd, HWND_TOP, 0,0, 0,0,  SWP_NOSIZE | SWP_NOMOVE);
		m_pvar_list.MoveWindow(&rc);
	}
}


void CBacnetPvar::OnSysCommand(UINT nID, LPARAM lParam)
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
