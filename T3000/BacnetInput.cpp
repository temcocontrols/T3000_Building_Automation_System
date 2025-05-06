// BacnetInput.cpp : implementation file
// BacnetInput File Add by Fance 2013 0620

#include "stdafx.h"
#include "T3000.h"
#include "BacnetInput.h"
#include "afxdialogex.h"
#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "global_function.h"
#include <math.h>

#include "global_define.h"
#include "BacnetRange.h"
#include "MainFrm.h"
#include "CBacnetUnitsSelection.h"
extern tree_product selected_product_Node; // 
extern void copy_data_to_ptrpanel(int Data_type);//Used for copy the structure to the ptrpanel.
extern int initial_dialog;

static bool show_external =  false;
CRect Input_rect;
int INPUT_LIMITE_ITEM_COUNT = 0;
// CBacnetInput dialog

int changed_input_item = -1; // 

IMPLEMENT_DYNAMIC(CBacnetInput, CDialogEx)

CBacnetInput::CBacnetInput(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetInput::IDD, pParent)
{
  //  m_latest_protocol=3;
	window_max = true;
}


CBacnetInput::~CBacnetInput()
{
   
}

void CBacnetInput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_input_list);
	DDX_Control(pDX, IDC_STATIC_INPUT_ITEM_INFO, m_input_item_info);
}

BEGIN_MESSAGE_MAP(CBacnetInput, CDialogEx)
    ON_MESSAGE(MY_RESUME_DATA, InputMessageCallBack)
    ON_MESSAGE(WM_REFRESH_BAC_INPUT_LIST,Fresh_Input_List)
    ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Input_Item)	
    ON_BN_CLICKED(IDOK, &CBacnetInput::OnBnClickedOk)
    ON_NOTIFY(NM_CLICK, IDC_LIST1, &CBacnetInput::OnNMClickList1)
    ON_WM_TIMER()
    ON_WM_CLOSE()
    
    ON_WM_HELPINFO()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

// CBacnetInput message handlers
LRESULT  CBacnetInput::InputMessageCallBack(WPARAM wParam, LPARAM lParam)
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
		if((pInvoke->mRow < BAC_INPUT_ITEM_COUNT) && (pInvoke->mRow >= 0))
		{
            //if (!SPECIAL_BAC_TO_MODBUS) //dbus
            if ((!SPECIAL_BAC_TO_MODBUS) && (Bacnet_Private_Device(selected_product_Node.product_class_id))) //dbus
            {
                Post_Refresh_One_Message(g_bac_instance, READINPUT_T3000,
                    pInvoke->mRow, pInvoke->mRow, sizeof(Str_in_point));
                SetTimer(2, 2000, NULL);
            }
		}
		//Save_InputData_to_db(pInvoke->mRow);
	}
	else
	{
		memcpy_s(&m_Input_data.at(pInvoke->mRow),sizeof(Str_in_point),&m_temp_Input_data[pInvoke->mRow],sizeof(Str_in_point));//
		PostMessage(WM_REFRESH_BAC_INPUT_LIST,pInvoke->mRow,REFRESH_ON_ITEM);//
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);

	}
	if((pInvoke->mRow%2)==0)	// 
		m_input_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_input_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
	m_input_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);


	if(pInvoke)
		delete pInvoke;
	return 0;
}


BOOL CBacnetInput::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowTextW(_T("INPUT"));
#if 0
	m_input_item_info.SetWindowTextW(_T(""));
	m_input_item_info.textColor(RGB(0,0,255));
	//m_static.bkColor(RGB(0,255,255));
	m_input_item_info.setFont(20,14,NULL,_T("Arial"));
#endif
	
	Initial_List();
	PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
   
	SetTimer(INPUT_REFRESH_DATA_TIMER, BAC_LIST_REFRESH_INPUT_TIME,NULL);
	SetTimer(4, 15000, NULL);
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_INPUT_DEFAULT);
	SetIcon(m_hIcon,TRUE);
	//SetIcon(m_hIcon,FALSE);

	ShowWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBacnetInput::Reload_Unit_Type()
{

	for (int i = 0; i < (int)m_Input_data.size(); i++)	//Initial All first.
	{
		if (ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_RANGE))
		{
			ListCtrlEx::CStrList strlist;
			for (int j = 0; j < (int)sizeof(Units_Type) / sizeof(Units_Type[0]); j++)
			{
				strlist.push_back(Units_Type[j]);
			}
			m_input_list.SetCellStringList(i, INPUT_RANGE, strlist);
		}
	}

	int initial_count = 0;
	if (bacnet_device_type == BIG_MINIPANEL || bacnet_device_type == MINIPANELARM)	//Special Initial
	{
		if (BIG_MINIPANEL_IN_A > (int)m_Input_data.size())
			initial_count = (int)m_Input_data.size();
		else
			initial_count = BIG_MINIPANEL_IN_A;
		for (int i = 0; i < initial_count; i++)
		{
			if (ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_RANGE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Units_Analog_Only);
				m_input_list.SetCellStringList(i, INPUT_RANGE, strlist);
			}
		}
	}
	else if (bacnet_device_type == SMALL_MINIPANEL || bacnet_device_type == MINIPANELARM_LB)
	{
		if (SMALL_MINIPANEL_IN_A > (int)m_Input_data.size())
			initial_count = (int)m_Input_data.size();
		else
			initial_count = SMALL_MINIPANEL_IN_A;
		for (int i = 0; i < initial_count; i++)
		{
			if (ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_RANGE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Units_Analog_Only);
				m_input_list.SetCellStringList(i, INPUT_RANGE, strlist);
			}
		}
	}
	else if (bacnet_device_type == TINY_MINIPANEL)
	{
		if (TINY_MINIPANEL_IN_A > (int)m_Input_data.size())
			initial_count = (int)m_Input_data.size();
		else
			initial_count = TINY_MINIPANEL_IN_A;
		for (int i = 0; i < initial_count; i++)
		{
			if (ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_RANGE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Units_Analog_Only);
				m_input_list.SetCellStringList(i, INPUT_RANGE, strlist);
			}
		}
	}
	else if (bacnet_device_type == TINY_EX_MINIPANEL ||
		bacnet_device_type == MINIPANELARM_TB)
	{
		if (TINYEX_MINIPANEL_IN_A > (int)m_Input_data.size())
			initial_count = (int)m_Input_data.size();
		else
			initial_count = TINYEX_MINIPANEL_IN_A;
		for (int i = 0; i < initial_count; i++)
		{
			if (ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_RANGE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Units_Analog_Only);
				m_input_list.SetCellStringList(i, INPUT_RANGE, strlist);
			}
		}
	}
	else if (bacnet_device_type == T3_TB_11I)
	{
		if (TB_11I_IN_A > (int)m_Input_data.size())
			initial_count = (int)m_Input_data.size();
		else
			initial_count = TB_11I_IN_A;
		for (int i = 0; i < initial_count; i++)
		{
			if (ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_RANGE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Units_Analog_Only);
				m_input_list.SetCellStringList(i, INPUT_RANGE, strlist);
			}
		}
	}
	else if (bacnet_device_type == PRODUCT_CM5)
	{
		int analog_count;
		int digital_count;
		if (CM5_MINIPANEL_IN_A > (int)m_Input_data.size())
			analog_count = (int)m_Input_data.size();
		else
			analog_count = CM5_MINIPANEL_IN_A;

		digital_count = CM5_MINIPANEL_IN_D;

		for (int i = 0; i < analog_count; i++)
		{
			if (ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_RANGE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Units_Analog_Only);
				m_input_list.SetCellStringList(i, INPUT_RANGE, strlist);
			}
		}

		for (int i = analog_count; i < (analog_count + digital_count); i++)
		{
			if (ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_RANGE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Units_Digital_Only);
				m_input_list.SetCellStringList(i, INPUT_RANGE, strlist);
			}
		}
	}
	else if (bacnet_device_type == MINIPANELARM_NB)
	{
		if (BACNET_ROUTER_IN_A > (int)m_Input_data.size())
			initial_count = (int)m_Input_data.size();
		else
			initial_count = BACNET_ROUTER_IN_A;
	}
	else if (bacnet_device_type == T3_FAN_MODULE)
	{
		if (FAN_MODULE_IN_A > (int)m_Input_data.size())
			initial_count = (int)m_Input_data.size();
		else
			initial_count = FAN_MODULE_IN_A;
	}
	else if (bacnet_device_type == T3_ESP_RMC)
	{
		if (NG2_IN_A > (int)m_Input_data.size())
			initial_count = (int)m_Input_data.size();
		else
			initial_count = NG2_IN_A;
	}
	else if (bacnet_device_type == T3_NG2_TYPE2)
	{
		if (NG2_TYPE2_IN_A > (int)m_Input_data.size())
			initial_count = (int)m_Input_data.size();
		else
			initial_count = NG2_TYPE2_IN_A;
	}

}

//void CBacnetInput::InitialInputGrid()
//{
//    for (int z = 0;z < (int)m_Input_data.size();i++)
//    {
//        for (int i = 0; i < INPUT_COL_NUMBER; i++)
//        {
//            if (Get_Product_Input_Map(g_selected_product_id, i) == 0)
//            {
//                m_input_list.SetCellEnabled(z, i, false);
//                m_input_list.SetItemText(z, i, _T(""));
//            }
//            else
//            {
//                m_input_list.SetCellEnabled(z, i, true);
//            }
//        }
//    }
//}

void CBacnetInput::Initial_List()
{
    m_input_list.ShowWindow(SW_HIDE);
	m_input_list.DeleteAllItems();
	while ( m_input_list.DeleteColumn (0)) ;

	m_input_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	//m_input_list.SetExtendedStyle(m_input_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_input_list.SetExtendedStyle(m_input_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_input_list.InsertColumn(INPUT_NUM, _T("Input"), 40, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_input_list.InsertColumn(INPUT_PANEL, _T("Panel"), 40, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_input_list.InsertColumn(INPUT_FULL_LABLE, _T("Full Label"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_AUTO_MANUAL, _T("Auto/Man"), 65, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_VALUE, _T("Value"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_UNITE, _T("Units"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_RANGE, _T("Range"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_CAL, _T("Calibration"), 70, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_CAL_OPERATION, _T("Sign"), 50, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_FITLER, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_DECOM, _T("Status"), 60, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_JUMPER, _T("Signal Type"), 90, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_LABLE, _T("Label"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);

	m_input_list.InsertColumn(INPUT_EXTERNAL, _T("Type"), 0, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_PRODUCT, _T("Product Name"), 0, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_EXT_NUMBER, _T("Product Input"), 0, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.Setlistcolcharlimit(INPUT_FULL_LABLE,STR_IN_DESCRIPTION_LENGTH -1);
	m_input_list.Setlistcolcharlimit(INPUT_LABLE,STR_IN_LABEL-1);
	show_external =  false;

	m_input_dlg_hwnd = this->m_hWnd;
	//g_hwnd_now = m_input_dlg_hwnd;

	CRect list_rect,win_rect;
	m_input_list.GetWindowRect(list_rect);

	m_input_list.GetClientRect(Input_rect);

	ScreenToClient(&list_rect);
	::GetWindowRect(m_input_dlg_hwnd,win_rect);
	m_input_list.Set_My_WindowRect(win_rect);
	m_input_list.Set_My_ListRect(list_rect);
	m_input_list.SetListHwnd(this->m_hWnd);

	//m_input_list.DeleteAllItems();
	for (int i=0;i<(int)m_Input_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;

		if (i >= input_item_limit_count)	//vector
		{
			m_input_list.DeleteItem(i);
			continue;
		}




		temp_item.Format(_T("IN%d"),i+1);
		m_input_list.InsertItem(i,temp_item);



		if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_RANGE))
		{
			ListCtrlEx::CStrList strlist;
			for (int j=0;j<(int)sizeof(Units_Type)/sizeof(Units_Type[0]);j++)
			{
				strlist.push_back(Units_Type[j]);
			}
			m_input_list.SetCellStringList(i, INPUT_RANGE, strlist);		
		}

		if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_JUMPER))
		{
			ListCtrlEx::CStrList strlist;
			for (int j=0;j<(int)sizeof(JumperStatus)/sizeof(JumperStatus[0]);j++)
			{
                if (j == 4)   // 4 stor Dry Contact; 
                    continue;
				strlist.push_back(JumperStatus[j]);
			}
			m_input_list.SetCellStringList(i, INPUT_JUMPER, strlist);		
		}

		for (int x=0;x<INPUT_COL_NUMBER;x++)
		{
			if((i%2)==0)
				m_input_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_input_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}

		

	}
	m_input_list.InitListData();
	 m_input_list.ShowWindow(SW_SHOW);
}

void CBacnetInput::OnBnClickedOk()
{
	
//	CDialogEx::OnOK();
}


LRESULT CBacnetInput::Fresh_Input_Item(WPARAM wParam,LPARAM lParam)
{   
	int cmp_ret ;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	if(Changed_Item>= INPUT_LIMITE_ITEM_COUNT)
	{
		m_input_list.SetItemText(Changed_Item,Changed_SubItem,_T(""));
		return 0;
	}

    //
    if (Get_Product_Input_Map(g_selected_product_id, Changed_SubItem) == false)
    {
        return 0;
    }

	memcpy_s(&m_temp_Input_data[Changed_Item],sizeof(Str_in_point),&m_Input_data.at(Changed_Item),sizeof(Str_in_point));

	CString temp_task_info;
	CString New_CString =  m_input_list.GetItemText(Changed_Item,Changed_SubItem);
	CString cstemp_value;
	if(Changed_SubItem == INPUT_LABLE)
	{
		CString cs_temp = m_input_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= STR_IN_LABEL)	//
		{
			MessageBox(_T("Length can not greater than 8"),_T("Warning"));

#pragma region note_what_do	
			//,
			CRect list_rect,win_rect;
			m_input_list.GetWindowRect(list_rect);
			ScreenToClient(&list_rect);
			::GetWindowRect(m_input_dlg_hwnd,win_rect);
			m_input_list.Set_My_WindowRect(win_rect);
			m_input_list.Set_My_ListRect(list_rect);
			m_input_list.Get_clicked_mouse_position();
#pragma endregion note_what_do

			PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
			return 0;
		}
		cs_temp.Replace(_T("-"),_T("_"));
		cs_temp.MakeUpper();
		if(Check_Label_Exsit(cs_temp))
		{
			PostMessage(WM_REFRESH_BAC_INPUT_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0;
		}
		if (bacnet_device_type == PM_THIRD_PARTY_DEVICE) // handled the full label changes for third party bacnet device
		{
			BACNET_APPLICATION_DATA_VALUE* temp_value = new BACNET_APPLICATION_DATA_VALUE();
			temp_value->tag = TPYE_BACAPP_CHARACTER_STRING;
			temp_value->context_specific = false;
			WideCharToMultiByte(CP_ACP, 0, cs_temp.GetBuffer(), -1, temp_value->type.Character_String.value, MAX_CHARACTER_STRING_BYTES, NULL, NULL);
			temp_value->type.Character_String.encoding = 0;
			temp_value->type.Character_String.length = cs_temp.GetLength() + 1;
			int ObjectType = OBJECT_ANALOG_INPUT;
			if (m_Input_data.at(Changed_Item).digital_analog == BAC_UNITS_DIGITAL)
				ObjectType = OBJECT_BINARY_INPUT;

			int invoke_id = Bacnet_Write_Properties_Blocking(g_bac_instance,(BACNET_OBJECT_TYPE)ObjectType, m_Input_data_instance.at(Changed_Item), PROP_DESCRIPTION, temp_value);
		}
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Input_data.at(Changed_Item).label,STR_IN_LABEL,cTemp1,STR_IN_LABEL);
	}

	if(Changed_SubItem == INPUT_FULL_LABLE)
	{
		CString cs_temp = m_input_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= STR_IN_DESCRIPTION_LENGTH)	//
		{
			MessageBox(_T("Length can not higher than 20"),_T("Warning"));

#pragma region note_what_do	
			//,
			CRect list_rect,win_rect;
			m_input_list.GetWindowRect(list_rect);
			ScreenToClient(&list_rect);
			::GetWindowRect(m_input_dlg_hwnd,win_rect);
			m_input_list.Set_My_WindowRect(win_rect);
			m_input_list.Set_My_ListRect(list_rect);
			m_input_list.Get_clicked_mouse_position();
#pragma endregion note_what_do


			PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
			return 0;
		}

		if(Check_FullLabel_Exsit(cs_temp))
		{
			PostMessage(WM_REFRESH_BAC_INPUT_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0;
		}
		if (bacnet_device_type == PM_THIRD_PARTY_DEVICE) // handled the full label changes for third party bacnet device
		{
				BACNET_APPLICATION_DATA_VALUE* temp_value = new BACNET_APPLICATION_DATA_VALUE();
				temp_value->tag = TPYE_BACAPP_CHARACTER_STRING;
				temp_value->context_specific = false;
				WideCharToMultiByte(CP_ACP, 0, cs_temp.GetBuffer(), -1, temp_value->type.Character_String.value, MAX_CHARACTER_STRING_BYTES, NULL, NULL);
				temp_value->type.Character_String.encoding = 0;
				temp_value->type.Character_String.length = cs_temp.GetLength() + 1;
				int ObjectType = OBJECT_ANALOG_INPUT;
				if (m_Input_data.at(Changed_Item).digital_analog == BAC_UNITS_DIGITAL)
					ObjectType = OBJECT_BINARY_INPUT;
				int invoke_id = Bacnet_Write_Properties_Blocking(g_bac_instance, (BACNET_OBJECT_TYPE)ObjectType, m_Input_data_instance.at(Changed_Item), PROP_OBJECT_NAME, temp_value);
		}
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Input_data.at(Changed_Item).description,STR_IN_DESCRIPTION_LENGTH,cTemp1,STR_IN_DESCRIPTION_LENGTH);
	}


	if(Changed_SubItem == INPUT_AUTO_MANUAL)
	{
		CString temp_cs = m_input_list.GetItemText(Changed_Item,Changed_SubItem);
		if(temp_cs.CompareNoCase(_T("Auto"))==0 || temp_cs.CompareNoCase(_T("False")) == 0)
		{
			//m_input_list.SetCellEnabled(Changed_Item,INPUT_VALUE,0);
			m_Input_data.at(Changed_Item).auto_manual = BAC_AUTO ;
		}
		else
		{
			//m_input_list.SetCellEnabled(Changed_Item,INPUT_VALUE,1);
			m_Input_data.at(Changed_Item).auto_manual = BAC_MANUAL ;
		}
		if (bacnet_device_type == PM_THIRD_PARTY_DEVICE) // handled the full label changes for third party bacnet device
		{
			BACNET_APPLICATION_DATA_VALUE* temp_value = new BACNET_APPLICATION_DATA_VALUE();
			temp_value->tag = TPYE_BACAPP_BOOLEAN;
			temp_value->type.Boolean = m_Input_data.at(Changed_Item).auto_manual;
			int ObjectType = OBJECT_ANALOG_INPUT;
			if (m_Input_data.at(Changed_Item).digital_analog == BAC_UNITS_DIGITAL)
				ObjectType = OBJECT_BINARY_INPUT;
			int invoke_id = Bacnet_Write_Properties_Blocking(g_bac_instance, (BACNET_OBJECT_TYPE)ObjectType, m_Input_data_instance.at(Changed_Item), PROP_OUT_OF_SERVICE, temp_value);
		}
	}



	if(Changed_SubItem == INPUT_VALUE)
	{
		CString temp_cs = m_input_list.GetItemText(Changed_Item,Changed_SubItem);

		int temp_int = (int)(_wtof(temp_cs) * 1000);
		//int temp_int = _wtoi(temp_cs);
		m_Input_data.at(Changed_Item).value = temp_int;
		if (bacnet_device_type == PM_THIRD_PARTY_DEVICE) // handled the full label changes for third party bacnet device
		{
			BACNET_APPLICATION_DATA_VALUE* temp_value = new BACNET_APPLICATION_DATA_VALUE();
			temp_value->tag = m_Input_data.at(Changed_Item).control;
			if (temp_value->tag == TPYE_BACAPP_UNSIGNED) {
				temp_value->type.Unsigned_Int = temp_int;
			}
			else if (temp_value->tag == TPYE_BACAPP_SIGNED) {
				 temp_value->type.Signed_Int = temp_int;
			}
			else if (temp_value->tag == TPYE_BACAPP_REAL) {
				 temp_value->type.Real = temp_int;
			}
			else if (temp_value->tag == TPYE_BACAPP_DOUBLE) {
				 temp_value->type.Double = temp_int;
			}
			//temp_value->type.Unsigned_Int = temp_int;
			int ObjectType = OBJECT_ANALOG_INPUT;
			if (m_Input_data.at(Changed_Item).digital_analog == BAC_UNITS_DIGITAL)
				ObjectType = OBJECT_BINARY_INPUT;
			int invoke_id = Bacnet_Write_Properties_Blocking(g_bac_instance, (BACNET_OBJECT_TYPE)ObjectType, m_Input_data_instance.at(Changed_Item), PROP_PRESENT_VALUE, temp_value);
		}
	}

	


	if(Changed_SubItem==INPUT_CAL)
	{
		CString cs_temp=m_input_list.GetItemText(Changed_Item,INPUT_CAL);
		float temp_value = (float)_wtof(cs_temp);
        if (temp_value < 0)
        {
            m_Input_data.at(Changed_Item).calibration_sign = 1;
            m_input_list.SetItemText(Changed_Item, INPUT_CAL_OPERATION, _T("-"));
        }

        //Fandu 49.6  .  2019 09 05
        int cal_value;
        //if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub > 495)
        //{
        //    cal_value = (int)(temp_value * 100);
        //}
        //else
		    cal_value = (int)(temp_value * 10);
        cal_value = abs(cal_value);
		if((cal_value<0) || (cal_value >65535))
		{
			MessageBox(_T("Please Input an value between 0.0 - 6553.6"),_T("Warning"),MB_OK);
			PostMessage(WM_REFRESH_BAC_INPUT_LIST,Changed_Item,REFRESH_ON_ITEM);//
			return 0;
		}

		//int cal_value = _wtoi(cs_temp);
		
		m_Input_data.at(Changed_Item).calibration_l = cal_value & 0x00ff;
		m_Input_data.at(Changed_Item).calibration_h = (cal_value & 0xff00)>>8;
	}
	else if(Changed_SubItem==INPUT_FITLER)
	{
		CString cs_temp=m_input_list.GetItemText(Changed_Item,INPUT_FITLER);
		int  temp2 = _wtoi(cs_temp);
		if((temp2<0) || (temp2 >255))
		{
			MessageBox(_T("Please Input an value between 0 - 255"),_T("Warning"),MB_OK);
			PostMessage(WM_REFRESH_BAC_INPUT_LIST,Changed_Item,REFRESH_ON_ITEM);//
			return 0;
		}
		m_Input_data.at(Changed_Item).filter = (unsigned char)temp2;//(int8_t)(log((double)temp2)/log((double)2));
	}

	if(Changed_SubItem == INPUT_JUMPER)
	{
		CString temp_jump = m_input_list.GetItemText(Changed_Item,Changed_SubItem);
		for (int z=0;z<sizeof(JumperStatus)/sizeof(JumperStatus[0]);z++)
		{
			if(temp_jump.CompareNoCase(JumperStatus[z]) == 0)
			{
				unsigned char temp_value = 0;
				if((z == 0) || (z == 1) || (z == 2) || (z == 3) || (z == 4) || (z == 5))
					temp_value = z;
				unsigned char temp1;
				temp1 = m_Input_data.at(Changed_Item).decom ;
				temp1 = temp1 & 0x0f;
				temp1 = temp1 | (temp_value << 4);
				m_Input_data.at(Changed_Item).decom = temp1;
			}
		}

	}

	cmp_ret = memcmp(&m_temp_Input_data[Changed_Item],&m_Input_data.at(Changed_Item),sizeof(Str_in_point));
	if(cmp_ret!=0)
	{
		m_input_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Input List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITEINPUT_T3000,Changed_Item,Changed_Item,sizeof(Str_in_point),m_input_dlg_hwnd ,temp_task_info,Changed_Item,Changed_SubItem);
		Sleep(50);
		PostMessage(WM_REFRESH_BAC_INPUT_LIST,Changed_Item,REFRESH_ON_ITEM);
	}
	//m_input_list.Invalidate();
	return 0;
}



LRESULT CBacnetInput::Fresh_Input_List(WPARAM wParam, LPARAM lParam)
{
	int Fresh_Item;
	int isFreshOne = (bool)lParam;
	int  Minipanel_device = 1;
	int listCount = m_input_list.GetItemCount();
	if (listCount != input_item_limit_count) // for bacnet devices hiding columns
	{
		if (bacnet_device_type == PM_THIRD_PARTY_DEVICE) // for bacnet devices hiding columns
		{
			isFreshOne = false;
		}
	}
	if (bacnet_device_type == T38AI8AO6DO)
	{
		INPUT_LIMITE_ITEM_COUNT = 8;
		Minipanel_device = 0;
	}
	else if (bacnet_device_type == PID_T322AI)
	{
		INPUT_LIMITE_ITEM_COUNT = 22;
		Minipanel_device = 0;
	}
	else if (bacnet_device_type == PID_T332AI)
	{
		INPUT_LIMITE_ITEM_COUNT = 32;
		Minipanel_device = 0;
	}
	else if (bacnet_device_type == PWM_TRANSDUCER)
	{
		INPUT_LIMITE_ITEM_COUNT = 6;
		Minipanel_device = 0;
	}
	else if (bacnet_device_type == PID_T3PT12)
	{
		INPUT_LIMITE_ITEM_COUNT = 12;
		Minipanel_device = 0;
	}
	else if (bacnet_device_type == PM_T3_LC)
	{
		INPUT_LIMITE_ITEM_COUNT = 8;
		Minipanel_device = 0;
	}
	else if (bacnet_device_type == PID_T36CTA)
	{
		INPUT_LIMITE_ITEM_COUNT = 24;
		Minipanel_device = 0;
	}
	else if (bacnet_device_type == TINY_EX_MINIPANEL)
	{
		//INPUT_LIMITE_ITEM_COUNT = 8;
		Minipanel_device = 1;
	}
	else if ((bacnet_device_type == STM32_CO2_NET) ||
		(bacnet_device_type == STM32_CO2_RS485) ||
		(bacnet_device_type == STM32_HUM_NET) ||
		(bacnet_device_type == STM32_HUM_RS485) ||
		(bacnet_device_type == STM32_PRESSURE_NET))
	{
		INPUT_LIMITE_ITEM_COUNT = 3;
		Minipanel_device = 0;
	}
	else if (bacnet_device_type == PM_MULTI_SENSOR)
	{
		INPUT_LIMITE_ITEM_COUNT = 15;
		Minipanel_device = 0;
	}
	else if ((bacnet_device_type == PM_TSTAT_AQ) || (bacnet_device_type == PM_AIRLAB_ESP32))
	{
		INPUT_LIMITE_ITEM_COUNT = 32;
		Minipanel_device = 0;
	}
	//else if (bacnet_device_type == BACNET_ROUTER)
	//{
	//    INPUT_LIMITE_ITEM_COUNT = BACNET_ROUTER_IN_A + BACNET_ROUTER_IN_D;
	//    Minipanel_device = 1;
	//}
	else
	{
		INPUT_LIMITE_ITEM_COUNT = BAC_INPUT_ITEM_COUNT;
		if ((g_selected_product_id == PM_ESP32_T3_SERIES) &&
			((int)Device_Basic_Setting.reg.pro_info.firmware0_rev_main) * 10 + (int)Device_Basic_Setting.reg.pro_info.firmware0_rev_sub >= ESP32_IO_COUNT_REDEFINE_VERSION)
		{
			INPUT_LIMITE_ITEM_COUNT = DYNAMIC_INPUT_ITEM_COUNT;
		}
	}


	bool temp_need_show_external = false;
	for (int z = 0; z < (int)m_Input_data.size(); z++)
	{
		if (z >= INPUT_LIMITE_ITEM_COUNT)
			break;


		if ((m_Input_data.at(z).sub_id != 0) &&
			(m_Input_data.at(z).sub_product != 0))
		{
			temp_need_show_external = true;
			break;
		}

	}
	//if(show_external != temp_need_show_external)
	//{
	//	show_external = temp_need_show_external;
	//	if(temp_need_show_external)
	//	{
	//		CRect temp_rect;
	//		temp_rect = Input_rect;
	//		temp_rect.right = 1250;
	//		temp_rect.top = temp_rect.top + 22;
	//		m_input_list.MoveWindow(temp_rect);
	//		m_input_list.SetColumnWidth(INPUT_EXTERNAL,60);
	//		m_input_list.SetColumnWidth(INPUT_PRODUCT,80);
	//		m_input_list.SetColumnWidth(INPUT_EXT_NUMBER,80);
	//	}
	//	else
	//	{
	//		CRect temp_rect;
	//		temp_rect = Input_rect;
	//		temp_rect.right = 975;
	//		temp_rect.top = temp_rect.top + 22;
	//		m_input_list.MoveWindow(temp_rect);

	//		m_input_list.SetColumnWidth(INPUT_EXTERNAL,0);
	//		m_input_list.SetColumnWidth(INPUT_PRODUCT,0);
	//		m_input_list.SetColumnWidth(INPUT_EXT_NUMBER,0);
	//	}
	//}

	if (Minipanel_device == 0)	//nipanel
	{


		//m_input_list.SetColumnWidth(INPUT_EXTERNAL,0);
		m_input_list.SetColumnWidth(INPUT_PRODUCT, 0);
		m_input_list.SetColumnWidth(INPUT_EXT_NUMBER, 0);
	}
	else
	{
		if (temp_need_show_external)
		{
			//m_input_list.SetColumnWidth(INPUT_EXTERNAL,60);
			m_input_list.SetColumnWidth(INPUT_PRODUCT, 80);
			m_input_list.SetColumnWidth(INPUT_EXT_NUMBER, 80);
		}
		else
		{
			//m_input_list.SetColumnWidth(INPUT_EXTERNAL,0);
			m_input_list.SetColumnWidth(INPUT_PRODUCT, 0);
			m_input_list.SetColumnWidth(INPUT_EXT_NUMBER, 0);
		}
	}
	m_input_list.SetColumnWidth(INPUT_EXTERNAL, 60);

	if (isFreshOne == (int)REFRESH_ON_ITEM)
	{
		Fresh_Item = (int)wParam;
	}
	else
	{
		//ist 
		if ((m_input_list.IsDataNewer((char*)&m_Input_data.at(0), sizeof(Str_in_point) * BAC_INPUT_ITEM_COUNT) == false) && refresh_input == 0)
		{
			return 0;
		}
	}
	if (0)
	{
		int temp_select_raw = 0;
		int temp_select_col = 0;
		m_input_list.Get_Selected_Item(temp_select_raw, temp_select_col);
		m_input_list.SetItemBkColor(temp_select_raw, temp_select_col, LIST_ITEM_SELECTED, 0);
	}
#ifdef USE_THIRD_PARTY_FUNC
	if (bacnet_device_type == PM_THIRD_PARTY_DEVICE) // for bacnet devices hiding columns
	{
		//m_input_list.DeleteColumn(INPUT_AUTO_MANUAL);
		//m_input_list.InsertColumn(INPUT_AUTO_MANUAL, _T("Out of service"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
		//LVCOLUMN col;
		TCHAR szBuffer[80];

		LVCOLUMN col;
		col.mask = LVCF_TEXT;
		col.pszText = szBuffer;
		col.cchTextMax = 80;
		m_input_list.GetColumn(INPUT_AUTO_MANUAL, &col);
		col.pszText = _T("Out of Service");
		m_input_list.SetColumn(INPUT_AUTO_MANUAL, &col);
		m_input_list.SetColumnWidth(INPUT_RANGE, 0); // range and unit dupilcate value's no need to show.
		m_input_list.SetColumnWidth(INPUT_CAL, 0);
		m_input_list.SetColumnWidth(INPUT_CAL_OPERATION, 0);
		m_input_list.SetColumnWidth(INPUT_FITLER, 0);
		m_input_list.SetColumnWidth(INPUT_DECOM, 0);
		m_input_list.SetColumnWidth(INPUT_JUMPER, 0);
		m_input_list.SetColumnWidth(INPUT_PRODUCT, 0);
		m_input_list.SetColumnWidth(INPUT_EXT_NUMBER, 0);

	}
	else { // to show the column for non-bacnet devices
		//m_input_list.DeleteColumn(INPUT_AUTO_MANUAL);
		//m_input_list.InsertColumn(INPUT_AUTO_MANUAL, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
		TCHAR szBuffer[80];

		LVCOLUMN col;
		col.mask = LVCF_TEXT;
		col.pszText = szBuffer;
		col.cchTextMax = 65;
		m_input_list.GetColumn(INPUT_AUTO_MANUAL, &col);
		col.pszText = _T("Auto/Man");
		m_input_list.SetColumn(INPUT_AUTO_MANUAL, &col);
		m_input_list.SetColumnWidth(INPUT_RANGE, 100);
		m_input_list.SetColumnWidth(INPUT_CAL, 70);
		m_input_list.SetColumnWidth(INPUT_CAL_OPERATION, 50);
		m_input_list.SetColumnWidth(INPUT_FITLER, 60);
		m_input_list.SetColumnWidth(INPUT_DECOM, 60);
		m_input_list.SetColumnWidth(INPUT_JUMPER, 60);
	}
#endif
	CString temp1;
	//m_input_list.DeleteAllItems();
	for (int i = 0; i < (int)m_Input_data.size(); i++)
	{
		CString temp_item, temp_value, temp_cal, temp_filter, temp_status, temp_lable;
		CString temp_des;
		CString temp_units;

		if (isFreshOne)
		{
			i = Fresh_Item;
			if (i >= m_Input_data.size())
				return 1;
		}

		if (i >= input_item_limit_count)
			break;

		if (i >= INPUT_LIMITE_ITEM_COUNT)
		{
			for (int a = 0; a < INPUT_COL_NUMBER; a++)
			{
				m_input_list.SetItemText(i, a, _T(""));

			}
			continue;
		}
		if (bacnet_device_type == PM_THIRD_PARTY_DEVICE)
		{
			if (m_Input_data.at(i).digital_analog == BAC_UNITS_ANALOG) {

				temp_item.Format(_T("AI%d"), m_Input_data_instance.at(i));
			}
			else {
				temp_item.Format(_T("BI%d"), m_Input_data_instance.at(i));
			}
		}
		else
			temp_item.Format(_T("IN%d"), i + 1);

		if (refresh_input == 0)
		{
			if (isFreshOne != 1) // 
			{
				if (m_input_list.IsDataItemNewer((char*)&m_Input_data.at(i), sizeof(Str_in_point), i) == false)  //
				{
					continue; //
				}
			}
		}



		m_input_list.SetItemText(i, INPUT_NUM, temp_item);


		MultiByteToWideChar(CP_ACP, 0, (char*)m_Input_data.at(i).description, (int)strlen((char*)m_Input_data.at(i).description) + 1,
			temp_des.GetBuffer(MAX_PATH), MAX_PATH);
		temp_des.ReleaseBuffer();
		temp_des.Left(STR_IN_DESCRIPTION_LENGTH).Trim();

		m_input_list.SetItemText(i, INPUT_FULL_LABLE, temp_des);
		if (bacnet_device_type == PM_THIRD_PARTY_DEVICE)
		{
			if (m_Input_data.at(i).auto_manual == 0)
			{
				m_input_list.SetItemText(i, INPUT_AUTO_MANUAL, _T("False"));
			}
			else
			{
				m_input_list.SetItemText(i, INPUT_AUTO_MANUAL, _T("True"));
			}
		}
		else
		{
			if (m_Input_data.at(i).auto_manual == 0)
			{
				m_input_list.SetItemText(i, INPUT_AUTO_MANUAL, _T("Auto"));
			}
			else
			{
				m_input_list.SetItemText(i, INPUT_AUTO_MANUAL, _T("Manual"));
			}
		}


		if (m_Input_data.at(i).digital_analog == BAC_UNITS_ANALOG)
		{
			m_input_list.SetCellEnabled(i, INPUT_JUMPER, 0);  //nput range log al type um
			m_input_list.SetItemTextColor(i, INPUT_JUMPER, LIST_ITEM_DISABLE, 0);
			m_input_list.SetCellEnabled(i, INPUT_CAL, 1);
			m_input_list.SetCellEnabled(i, INPUT_CAL_OPERATION, 1);
			m_input_list.SetCellEnabled(i, INPUT_FITLER, 1);

			if ((m_Input_data.at(i).range >= 20) && (m_Input_data.at(i).range <= 24))
			{
				m_input_list.SetItemText(i, INPUT_UNITE, Analog_Customer_Units[m_Input_data.at(i).range - 20]);
				m_input_list.SetCellEnabled(i, INPUT_JUMPER, 1);  //nput range log al type um
				m_input_list.SetItemTextColor(i, INPUT_JUMPER, RGB(0, 0, 0), 0);
			}
			else if (m_Input_data.at(i).range < (sizeof(Input_List_Analog_Units) / sizeof(Input_List_Analog_Units[0])))
				m_input_list.SetItemText(i, INPUT_UNITE, Input_List_Analog_Units[m_Input_data.at(i).range]);
			else if (m_Input_data.at(i).range < (sizeof(Input_Analog_Units_Array) / sizeof(Input_Analog_Units_Array[0])))
				m_input_list.SetItemText(i, INPUT_UNITE, _T(""));
			else
				m_input_list.SetItemText(i, INPUT_RANGE, _T("Unused"));

			if (bacnet_device_type == PM_THIRD_PARTY_DEVICE)
			{
				CString inputunit;
				MultiByteToWideChar(CP_ACP, 0, (char*)bacnet_engineering_unit_names[m_Input_data.at(i).range].pString,
					(int)strlen((char*)bacnet_engineering_unit_names[m_Input_data.at(i).range].pString) + 1,
					inputunit.GetBuffer(MAX_PATH), MAX_PATH);
				inputunit.ReleaseBuffer();
				m_input_list.SetItemText(i, INPUT_UNITE, inputunit);
				//m_input_list.SetItemText(i, INPUT_RANGE, inputunit);
			}
			else if (m_Input_data.at(i).range == 0)
				m_input_list.SetItemText(i, INPUT_RANGE, _T("Unused"));
			else if (m_Input_data.at(i).range < (sizeof(Input_Analog_Units_Array) / sizeof(Input_Analog_Units_Array[0])))
			{
				if (bacnet_device_type == PM_T3PT12)
				{
					if (m_Input_data.at(i).range == 1)
						m_input_list.SetItemText(i, INPUT_RANGE, _T("PT-100 Deg.C"));
					else if (m_Input_data.at(i).range == 2)
						m_input_list.SetItemText(i, INPUT_RANGE, _T("PT-100 Deg.F"));
					else if (m_Input_data.at(i).range == 5)
						m_input_list.SetItemText(i, INPUT_RANGE, _T("PT-1000 Deg.C"));
					else if (m_Input_data.at(i).range == 6)
						m_input_list.SetItemText(i, INPUT_RANGE, _T("PT-1000 Deg.F"));
					else
					{
						if (m_Input_data.at(i).range <= (sizeof(Input_Analog_Units_Array) / sizeof(Input_Analog_Units_Array[0])))
							m_input_list.SetItemText(i, INPUT_RANGE, Input_Analog_Units_Array[m_Input_data.at(i).range]);
					}

				}
				else
					m_input_list.SetItemText(i, INPUT_RANGE, Input_Analog_Units_Array[m_Input_data.at(i).range]);
			}
			else
				m_input_list.SetItemText(i, INPUT_RANGE, _T("Out of range"));





			CString cstemp_value;
			float temp_float_value;
			temp_float_value = ((float)m_Input_data.at(i).value) / 1000;
			cstemp_value.Format(_T("%.2f"), temp_float_value);
			m_input_list.SetItemText(i, INPUT_VALUE, cstemp_value);

			unsigned short temp_cal_value = ((unsigned char)(m_Input_data.at(i).calibration_h)) * 256 + (unsigned char)m_Input_data.at(i).calibration_l;
			//if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub > 495)
			//{
			//    temp_cal.Format(_T("%.2f"), ((float)temp_cal_value) / 100);
			//}
			//else
			temp_cal.Format(_T("%.1f"), ((float)temp_cal_value) / 10);
			m_input_list.SetItemText(i, INPUT_CAL, temp_cal);
			if (m_Input_data.at(i).calibration_sign == 0)
			{
				m_input_list.SetItemText(i, INPUT_CAL_OPERATION, _T("+"));
			}
			else
			{
				m_input_list.SetItemText(i, INPUT_CAL_OPERATION, _T("-"));
			}
		}
		else if (m_Input_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
		{
			m_input_list.SetCellEnabled(i, INPUT_JUMPER, 0);  //nput range ital nal type
			m_input_list.SetItemTextColor(i, INPUT_JUMPER, LIST_ITEM_DISABLE, 0);
			m_input_list.SetItemText(i, INPUT_CAL, _T(""));
			m_input_list.SetCellEnabled(i, INPUT_CAL, 0);
			m_input_list.SetCellEnabled(i, INPUT_CAL_OPERATION, 0);
			m_input_list.SetCellEnabled(i, INPUT_FITLER, 0);

			if (m_Input_data.at(i).range == 0)
			{
				CString cstemp_value1;
				float temp_float_value1;
				temp_float_value1 = ((float)m_Input_data.at(i).value) / 1000;
				cstemp_value1.Format(_T("%.2f"), temp_float_value1);
				m_input_list.SetItemText(i, INPUT_VALUE, cstemp_value1);

				m_input_list.SetItemText(i, INPUT_RANGE, Digital_Units_Array[0]);
			}
			else if (m_Input_data.at(i).range <= 22)
				m_input_list.SetItemText(i, INPUT_RANGE, Digital_Units_Array[m_Input_data.at(i).range]);
			else if ((m_Input_data.at(i).range >= 23) && (m_Input_data.at(i).range <= 30))
			{
				if (receive_customer_unit)
					m_input_list.SetItemText(i, INPUT_RANGE, Custom_Digital_Range[m_Input_data.at(i).range - 23]);
				else
					m_input_list.SetItemText(i, INPUT_RANGE, Digital_Units_Array[0]);
			}
			else
				m_input_list.SetItemText(i, INPUT_RANGE, Digital_Units_Array[0]);
			m_input_list.SetItemText(i, INPUT_UNITE, _T(""));

			if ((m_Input_data.at(i).range > 30) || (m_Input_data.at(i).range == 0))
			{

			}
			else
			{

				CStringArray temparray;

				if ((m_Input_data.at(i).range < 23) && (m_Input_data.at(i).range != 0))
					temp1 = Digital_Units_Array[m_Input_data.at(i).range];
				else if ((m_Input_data.at(i).range >= 23) && (m_Input_data.at(i).range <= 30))
				{
					if (receive_customer_unit)
						temp1 = Custom_Digital_Range[m_Input_data.at(i).range - 23];
				}

				SplitCStringA(temparray, temp1, _T("/"));
				if ((temparray.GetSize() == 2))
				{
					if (m_Input_data.at(i).control == 0)
						m_input_list.SetItemText(i, INPUT_VALUE, temparray.GetAt(0));
					else
						m_input_list.SetItemText(i, INPUT_VALUE, temparray.GetAt(1));
				}
			}

		}

		temp_filter.Format(_T("%d"), (unsigned char)m_Input_data.at(i).filter);
		m_input_list.SetItemText(i, INPUT_FITLER, temp_filter);

		int temp_decom = 0;
		int temp_jumper = 0;
		temp_decom = m_Input_data.at(i).decom & 0x0f;
		temp_jumper = (m_Input_data.at(i).decom & 0xf0) >> 4;

		//ange 
		if ((temp_decom == 0) || (m_Input_data.at(i).range == 0) || bac_Invalid_range(m_Input_data.at(i).range))
		{
			temp_status.Format(Decom_Array[0]);
			m_input_list.SetItemTextColor(i, INPUT_DECOM, RGB(0, 0, 0), false);
		}
		else if (temp_decom == 1)
		{
			temp_status.Format(Decom_Array[1]);
			m_input_list.SetItemTextColor(i, INPUT_DECOM, RGB(255, 0, 0), false);
		}
		else if (temp_decom == 2)
		{
			temp_status.Format(Decom_Array[2]);
			m_input_list.SetItemTextColor(i, INPUT_DECOM, RGB(255, 0, 0), false);
		}
		else
		{
			temp_status.Empty();
			m_input_list.SetItemTextColor(i, INPUT_DECOM, RGB(0, 0, 0), false);
		}
		m_input_list.SetItemText(i, INPUT_DECOM, temp_status);


		if ((temp_jumper >= 0) && (temp_jumper < sizeof(JumperStatus) / sizeof(JumperStatus[0])))
		{
			temp_status.Format(JumperStatus[temp_jumper]);
		}
		else
		{
			temp_status.Format(JumperStatus[0]);
			m_Input_data.at(i).decom = m_Input_data.at(i).decom & 0x0f;	 //
		}

		//if(temp_jumper == 1)
		//{
		//	temp_status.Format(JumperStatus[1]);
		//}
		//else if(temp_jumper == 2)
		//{
		//	temp_status.Format(JumperStatus[2]);
		//}
		//else if(temp_jumper == 3)
		//{
		//	temp_status.Format(JumperStatus[3]);
		//}
		//else if(temp_jumper == 0)
		//{
		//	temp_status.Format(JumperStatus[0]);
		//}
		//else
		//{
		//	temp_status.Format(JumperStatus[0]);
		//	m_Input_data.at(i).decom = m_Input_data.at(i).decom & 0x0f;	 //
		//}
		m_input_list.SetItemText(i, INPUT_JUMPER, temp_status);


		CString main_sub_panel;


#pragma region External info
		if ((m_Input_data.at(i).sub_id != 0) &&
			//(m_Input_data.at(input_list_line).sub_number !=0) &&
			(m_Input_data.at(i).sub_product != 0) &&
			Minipanel_device == 1)
		{
			unsigned char temp_pid = m_Input_data.at(i).sub_product;
			if (
				(temp_pid == PM_T3PT10) ||
				(temp_pid == PM_T3IOA) ||
				(temp_pid == PM_T332AI) ||
				(temp_pid == PM_T38AI16O) ||
				(temp_pid == PM_T38I13O) ||
				(temp_pid == PM_T34AO) ||
				(temp_pid == PM_T322AI) ||
				(temp_pid == PM_T332AI_ARM) ||
				(temp_pid == PM_T38AI8AO6DO) ||
				(temp_pid == PM_T36CT) ||
				(temp_pid == PM_T36CTA) ||
				(temp_pid == PM_T3PT12) ||
				(temp_pid == PM_T3_LC)
				)
			{
				//m_input_item_info.ShowWindow(true);
				CString temp_name;
				temp_name = GetProductName(m_Input_data.at(i).sub_product);
				CString show_info;
				CString temp_id;
				CString temp_number;
				temp_number.Format(_T("Input%d"), (unsigned char)m_Input_data.at(i).sub_number + 1);
				m_input_list.SetItemText(i, INPUT_EXTERNAL, _T("External"));
				m_input_list.SetItemTextColor(i, INPUT_EXTERNAL, RGB(0, 0, 255), FALSE);
				m_input_list.SetItemText(i, INPUT_PRODUCT, temp_name);
				m_input_list.SetItemText(i, INPUT_EXT_NUMBER, temp_number);

				main_sub_panel.Format(_T("%d-%d"), (unsigned char)Station_NUM, (unsigned char)m_Input_data.at(i).sub_id);
				m_input_list.SetItemText(i, INPUT_PANEL, main_sub_panel);
			}
			else
			{
				main_sub_panel.Format(_T("%d"), (unsigned char)Station_NUM);
				m_input_list.SetItemText(i, INPUT_PANEL, main_sub_panel);

				m_input_list.SetItemText(i, INPUT_EXTERNAL, _T(""));
				m_input_list.SetItemText(i, INPUT_PRODUCT, _T(""));
				m_input_list.SetItemText(i, INPUT_EXT_NUMBER, _T(""));
			}
		}
		else
		{
			if (bacnet_device_type == PM_THIRD_PARTY_DEVICE)
			{
				CString panelID;
				panelID.Format(_T("%d"), (unsigned char)g_bac_instance);// temporarily assigned a bacnet_instance_id.

				m_input_list.SetItemText(i, INPUT_PANEL, panelID);
				m_input_list.SetItemText(i, INPUT_EXTERNAL, Output_Type_String[m_Input_data.at(i).digital_analog + 1]);
			}
			else {

				m_input_list.SetItemText(i, INPUT_PANEL, Statuspanel);
				int InputType = 0;
				if (Bacnet_Private_Device(selected_product_Node.product_class_id))
					bacnet_device_type = Device_Basic_Setting.reg.mini_type;
				InputType = GetInputType(selected_product_Node.product_class_id, bacnet_device_type, i + 1, m_Input_data.at(i).digital_analog);
				m_input_list.SetItemText(i, INPUT_EXTERNAL, Output_Type_String[InputType]);
			}

			//m_input_list.SetItemText(i,INPUT_EXTERNAL,_T(""));
			m_input_list.SetItemText(i, INPUT_PRODUCT, _T(""));
			m_input_list.SetItemText(i, INPUT_EXT_NUMBER, _T(""));
		}

#pragma endregion External info

		if ((selected_product_Node.product_class_id == PM_TSTAT_AQ) || //irlab 
			(selected_product_Node.product_class_id == PM_AIRLAB_ESP32))
		{
			if (product_register_value[4] < 106)
			{
				if ((i >= 3) && (i < 11))
					m_input_list.SetItemText(i, INPUT_UNITE, Airlab_Unit_String[i]);
			}
			else
			{
				bitset<16> module_type(product_register_sensor_flag[1]);
				if (product_register_sensor_flag[0] == 0x55) //
				{
					if (i == 2)
					{
						if (module_type.test(SENSOR_BIT_CO2) == false)
						{
							m_input_list.SetItemText(i, INPUT_RANGE, SensorStatus);
						}
					}
					else if (i == 3)
					{
						if (module_type.test(SENSOR_BIT_TVOC) == false)
						{
							m_input_list.SetItemText(i, INPUT_RANGE, SensorStatus);
						}
					}
					else if ((i >= 4) && (i <= 13))
					{
						if (module_type.test(SENSOR_BIT_PM2_5) == false)
						{
							m_input_list.SetItemText(i, INPUT_RANGE, SensorStatus);
						}
					}
					else if (i == 14)
					{
						if (module_type.test(SENSOR_BIT_SOUND) == false)
						{
							m_input_list.SetItemText(i, INPUT_RANGE, SensorStatus);
						}
					}
					else if (i == 15)
					{
						if (module_type.test(SENSOR_BIT_LIGHT) == false)
						{
							m_input_list.SetItemText(i, INPUT_RANGE, SensorStatus);
						}
					}

				}
			}

		}

		CString temp_des2;
		MultiByteToWideChar(CP_ACP, 0, (char*)m_Input_data.at(i).label, (int)strlen((char*)m_Input_data.at(i).label) + 1,
			temp_des2.GetBuffer(MAX_PATH), MAX_PATH);
		temp_des2.ReleaseBuffer();
		temp_des2 = temp_des2.Left(STR_IN_LABEL).Trim();
		m_input_list.SetItemText(i, INPUT_LABLE, temp_des2);

		for (int j = 0; j < INPUT_COL_NUMBER; j++)
		{
			//
			if (Get_Product_Input_Map(g_selected_product_id, j) == 1)
			{
				m_input_list.SetCellEnabled(i, j, true);
			}
			else
			{
				m_input_list.SetCellEnabled(i, j, false);
				m_input_list.SetItemText(i, j, _T(""));
			}
		}


		if (isFreshOne)
		{
			break;
		}
	}
	if (m_input_list.IsDataNewer((char*)&m_Input_data.at(0), sizeof(Str_in_point) * BAC_INPUT_ITEM_COUNT))
	{
		//ist 
		m_input_list.SetListData((char*)&m_Input_data.at(0), sizeof(Str_in_point) * BAC_INPUT_ITEM_COUNT);
	}
	copy_data_to_ptrpanel(TYPE_INPUT);
#ifdef LOCAL_DB_FUNCTION
	if (selected_product_Node.serial_number != 0)
		WriteDeviceDataIntoAccessDB(BAC_IN, input_item_limit_count, selected_product_Node.serial_number);
#endif
	if (isFreshOne != 1)
	{
		if (refresh_input > 0)
			refresh_input--;  //
	}
	return 0;
}


void CBacnetInput::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{


	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
 
	CString temp_cstring;
	long lRow,lCol;
	m_input_list.Set_Edit(true);
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos));
	m_input_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_input_list.SubItemHitTest(&lvinfo);
	if((nItem!=-1) && (nItem < BAC_INPUT_ITEM_COUNT))
	{
		input_list_line = nItem;
		if((m_Input_data.at(input_list_line).sub_id !=0) &&
			//(m_Input_data.at(input_list_line).sub_number !=0) &&
			(m_Input_data.at(input_list_line).sub_product !=0))
		{
			unsigned char temp_pid = m_Input_data.at(input_list_line).sub_product;
			if((temp_pid == PM_T3PT10) ||
				(temp_pid == PM_T3IOA) ||
				(temp_pid == PM_T332AI) ||
				(temp_pid == PM_T38AI16O) ||
				(temp_pid == PM_T38I13O) ||
				(temp_pid == PM_T34AO) ||
				(temp_pid == PM_T322AI) ||
				(temp_pid == PM_T332AI_ARM) ||
				(temp_pid == PM_T38AI8AO6DO) ||
				(temp_pid == PM_T36CT) ||
				(temp_pid == PM_T36CTA) ||
				(temp_pid == PM_T3_LC)
				)
			{
#if 0
				m_input_item_info.ShowWindow(true);
				CString temp_name;
				temp_name = GetProductName(m_Input_data.at(input_list_line).sub_product);
				CString show_info;
				CString temp_id;
				CString temp_number;
				temp_id.Format(_T(" Sub ID: %u        "),(unsigned char)m_Input_data.at(input_list_line).sub_id);
				temp_number.Format(_T("Input%d"),(unsigned char)m_Input_data.at(input_list_line).sub_number + 1);
				show_info = _T("Module:") + temp_name +_T("        ") + temp_id + temp_number;
				m_input_item_info.SetWindowTextW(show_info);
#endif

			}	
			else
			{
#if 0
				m_input_item_info.ShowWindow(false);
#endif
			}
		}
		else
		{
			m_input_item_info.ShowWindow(false);
		}
	}
	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;

	if(lRow>= INPUT_LIMITE_ITEM_COUNT)
		return;

	if(lRow>m_input_list.GetItemCount()) //
		return;
	if(lRow<0)
		return;

    if (Get_Product_Input_Map(g_selected_product_id, lCol) == false)
    {
        return ;
    }

	CString New_CString;
	CString temp_task_info;

	CString temp1;
	CStringArray temparray;
	if(lCol == INPUT_VALUE)
	{
		if(m_Input_data.at(lRow).auto_manual == BAC_AUTO)	//If it is auto mode, disable to change the value.
		{
			m_input_list.Set_Edit(false);
			return;
		}

		if(m_Input_data.at(lRow).digital_analog != BAC_UNITS_DIGITAL)
			return;



		memcpy_s(&m_temp_Input_data[lRow],sizeof(Str_in_point),&m_Input_data.at(lRow),sizeof(Str_in_point));


		if((m_Input_data.at(lRow).range < 23) &&(m_Input_data.at(lRow).range !=0))
			temp1 = Digital_Units_Array[m_Input_data.at(lRow).range];
		else if((m_Input_data.at(lRow).range >=23) && (m_Input_data.at(lRow).range <= 30))
		{
			if(receive_customer_unit)
				temp1 = Custom_Digital_Range[m_Input_data.at(lRow).range - 23];
			else
			{
				m_input_list.Set_Edit(false);
				return;
			}
		}
		else
			return;
		SplitCStringA(temparray,temp1,_T("/"));

		if(m_Input_data.at(lRow).control == 0)
		{
			m_Input_data.at(lRow).control = 1;
			m_input_list.SetItemText(lRow,INPUT_VALUE,temparray.GetAt(1));
			New_CString = temparray.GetAt(1);
		}
		else
		{
			m_Input_data.at(lRow).control = 0;
			m_input_list.SetItemText(lRow,INPUT_VALUE,temparray.GetAt(0));
			New_CString = temparray.GetAt(0);
		}

	}
	else if(lCol == INPUT_CAL_OPERATION)
	{
		if(m_Input_data.at(lRow).digital_analog == BAC_UNITS_DIGITAL)
			return;


		CString n_option1;
		CString n_option2;
		CString n_value;


		if((m_Input_data.at(lRow).calibration_l != 0) && (m_Input_data.at(lRow).calibration_h != 0))
		{
			float temp_float_value;
			unsigned short temp_cal_value;
			temp_cal_value = (m_Input_data.at(lRow).calibration_h << 8 ) + m_Input_data.at(lRow).calibration_l;
            if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub > 495)
            {
                temp_float_value = ((float)temp_cal_value) / 100;
                n_value.Format(_T("%.2f"), temp_float_value);
            }
            else
            {
                temp_float_value = ((float)temp_cal_value) / 10;
                n_value.Format(_T("%.1f"), temp_float_value);
            }

		}

		if(m_Input_data.at(lRow).calibration_sign == 0)
		{
			n_option1.Format(_T("+"));
			n_option2.Format(_T("-"));
			n_option1 = n_option1 + n_value;
			n_option2 = n_option2 + n_value;
		}
		else
		{
			n_option1.Format(_T("-"));
			n_option2.Format(_T("+"));
			n_option1 = n_option1 + n_value;
			n_option2 = n_option2 + n_value;
		}

		CString notic_message;
		notic_message.Format(_T("This will change the calibration of this input from %s to %s"),n_option1.GetString(),n_option2.GetString());
		if(IDYES != MessageBox(notic_message,_T("Warning"),MB_YESNOCANCEL))
			return;
		memcpy_s(&m_temp_Input_data[lRow],sizeof(Str_in_point),&m_Input_data.at(lRow),sizeof(Str_in_point));
		if(m_Input_data.at(lRow).calibration_sign == 0)
		{
			m_Input_data.at(lRow).calibration_sign = 1;
			m_input_list.SetItemText(lRow,INPUT_CAL_OPERATION,_T("-"));
			New_CString = _T("-");
		}
		else
		{
			m_Input_data.at(lRow).calibration_sign = 0;
			m_input_list.SetItemText(lRow,INPUT_CAL_OPERATION,_T("+"));
			New_CString = _T("+");
		}
	}
	else if(lCol == INPUT_AUTO_MANUAL)
	{
		memcpy_s(&m_temp_Input_data[lRow],sizeof(Str_in_point),&m_Input_data.at(lRow),sizeof(Str_in_point));
		if (bacnet_device_type == PM_THIRD_PARTY_DEVICE)
		{
			if (m_Input_data.at(lRow).auto_manual == 0)
			{

				m_Input_data.at(lRow).auto_manual = 1;
				m_input_list.SetItemText(lRow, INPUT_AUTO_MANUAL, _T("True"));
			}
			else
			{
				m_Input_data.at(lRow).auto_manual = 0;
				m_input_list.SetItemText(lRow, INPUT_AUTO_MANUAL, _T("False"));
			}
				BACNET_APPLICATION_DATA_VALUE* temp_value = new BACNET_APPLICATION_DATA_VALUE();
				temp_value->tag = TPYE_BACAPP_BOOLEAN;
				temp_value->type.Boolean = m_Input_data.at(lRow).auto_manual;
				int ObjectType = OBJECT_ANALOG_INPUT;
				if (m_Input_data.at(lRow).digital_analog == BAC_UNITS_DIGITAL)
					ObjectType = OBJECT_BINARY_INPUT;
				int invoke_id = Bacnet_Write_Properties_Blocking(g_bac_instance, (BACNET_OBJECT_TYPE)ObjectType, m_Input_data_instance.at(lRow), PROP_OUT_OF_SERVICE, temp_value);
		}
		else
		{
			if (m_Input_data.at(lRow).auto_manual == 0)
			{
				m_Input_data.at(lRow).auto_manual = 1;
				m_input_list.SetItemText(lRow, INPUT_AUTO_MANUAL, _T("Manual"));
				//m_input_list.SetCellEnabled(lRow,INPUT_VALUE,TRUE);
				New_CString = _T("Manual");
			}
			else
			{
				m_Input_data.at(lRow).auto_manual = 0;
				m_input_list.SetItemText(lRow, INPUT_AUTO_MANUAL, _T("Auto"));
				New_CString = _T("Auto");
			}
		}
		
	}
	else if(lCol == INPUT_RANGE)
	{
        if ((g_selected_product_id == PM_TSTAT_AQ) ||
			(g_selected_product_id == PM_AIRLAB_ESP32))
            return;;
        if (g_selected_product_id == PM_TSTAT10)
        {
            if (Device_Basic_Setting.reg.mini_type == T3_OEM)
            {
                if((lRow >= 13) && (lRow <= 17))
                    return;
            }
			if (Device_Basic_Setting.reg.mini_type == T3_OEM_12I)
			{
				if ((lRow >= 17) && (lRow <= 21))
					return;
			}
            if (Device_Basic_Setting.reg.mini_type == T3_TSTAT10)
            {
                if ((lRow >= 9) && (lRow <= 12))
                    return;
            }
        }
		if (PM_ESP32_T3_SERIES == g_selected_product_id)
		{
			if (Device_Basic_Setting.reg.mini_type == T3_ESP_RMC)
			{
				if ((lRow >= 16) && (lRow <= 17))  //IN17  IN18 is sensor
					return;
			}
			else if (Device_Basic_Setting.reg.mini_type == T3_NG2_TYPE2)
			{
				if ((lRow >= 24) && (lRow <= 29))  //IN25  IN30 ange
					return;
			}
		}
        m_dialog_signal_type = 0xff;
		BacnetRange dlg;


        if ((g_protocol == MODBUS_BACNET_MSTP) ||
            (g_protocol == PROTOCOL_BACNET_IP) || 
            (g_protocol_support_ptp == PROTOCOL_MB_PTP_TRANSFER))// MSTP_UDBUS 00
        {
           // if (!read_customer_unit)//c units
            {
                int ret_cusunits = GetPrivateData_Blocking(g_bac_instance, READUNIT_T3000, 0, BAC_CUSTOMER_UNITS_COUNT - 1, sizeof(Str_Units_element), 3);
                if (ret_cusunits < 0)
                {
                    temp_task_info.Format(_T("Read custom units failed!"));
                    SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_task_info);
                }
            }
        }
		bac_range_number_choose = m_Input_data.at(lRow).range;
        input_item_select_for_range = lRow;
		if(m_Input_data.at(lRow).digital_analog == BAC_UNITS_ANALOG)
		{
			bac_ranges_type = INPUT_RANGE_ANALOG_TYPE;
			if(m_Input_data.at(lRow).range > (sizeof(Input_Analog_Units_Array) / sizeof(Input_Analog_Units_Array[0])))
			{
				m_Input_data.at(lRow).range = 0;
				bac_range_number_choose = 0;
			}
		}
		else
		{
			bac_ranges_type = INPUT_RANGE_DIGITAL_TYPE;
			if(bac_Invalid_range(m_Input_data.at(lRow).range))
			{
				m_Input_data.at(lRow).range = 0;
				bac_range_number_choose = 0;
			}
		}
		


        if (m_Input_data.at(lRow).sub_product == PM_T3PT12)
        {
            dlg.m_device_type = PM_T3PT12;
           // bacnet_device_type = PM_T3PT12;
        }
			initial_dialog = 2;
			dlg.DoModal();

			if(range_cancel)
			{
				PostMessage(WM_REFRESH_BAC_INPUT_LIST,lRow,REFRESH_ON_ITEM);//
				return ;
			}

			if(bac_range_number_choose == 0)	//used us;
			{
				m_Input_data.at(lRow).digital_analog =  BAC_UNITS_ANALOG;
				bac_ranges_type = INPUT_RANGE_ANALOG_TYPE;
			}
			if(bac_ranges_type == INPUT_RANGE_ANALOG_TYPE)
			{
				m_Input_data.at(lRow).digital_analog =  BAC_UNITS_ANALOG;
				m_Input_data.at(lRow).range =  bac_range_number_choose;
				if(bacnet_device_type == PM_T3PT12)
				{
					if(m_Input_data.at(lRow).range == 1)
						m_input_list.SetItemText(lRow,INPUT_RANGE,_T("PT-100 Deg.C"));
					else if(m_Input_data.at(lRow).range == 2)
						m_input_list.SetItemText(lRow,INPUT_RANGE,_T("PT-100 Deg.F"));
					else if(m_Input_data.at(lRow).range == 5)
						m_input_list.SetItemText(lRow,INPUT_RANGE,_T("PT-1000 Deg.C"));
					else if(m_Input_data.at(lRow).range == 6)
						m_input_list.SetItemText(lRow,INPUT_RANGE,_T("PT-1000 Deg.F"));
					else
					{
						if(bac_range_number_choose <= (sizeof(Input_Analog_Units_Array) / sizeof(Input_Analog_Units_Array[0])))
							m_input_list.SetItemText(lRow,INPUT_RANGE,Input_Analog_Units_Array[bac_range_number_choose]);	
					}


					New_CString = _T("PT Type ");
				}
				else
				{
					m_input_list.SetItemText(lRow,INPUT_RANGE,Input_Analog_Units_Array[bac_range_number_choose]);		
					New_CString = Input_Analog_Units_Array[bac_range_number_choose];
				}


				m_input_list.SetItemText(lRow, INPUT_UNITE, Input_List_Analog_Units[bac_range_number_choose]);
                if ((bac_range_number_choose >= 20) && (bac_range_number_choose <= 24))//s range type
                {
                    m_Input_data.at(lRow).decom = m_dialog_signal_type;
					m_input_list.SetItemText(lRow, INPUT_UNITE, Analog_Customer_Units[bac_range_number_choose - 20]);//its 
                }

				
				
				unsigned short temp_cal_value;
				temp_cal_value = (m_Input_data.at(lRow).calibration_h << 8 ) + m_Input_data.at(lRow).calibration_l;


				CString cstemp_value;
                if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub > 495)
                {
                    cstemp_value.Format(_T("%.2f"), ((float)temp_cal_value) / 100);
                }
                else
                {
                    cstemp_value.Format(_T("%.1f"), ((float)temp_cal_value) / 10);
                }
				m_input_list.SetItemText(lRow,INPUT_CAL,cstemp_value);


				m_input_list.SetCellEnabled(lRow,INPUT_CAL,1);
				m_input_list.SetCellEnabled(lRow,INPUT_UNITE,1);

				float temp_float_value;
				temp_float_value = ((float)m_Input_data.at(lRow).value) / 1000;
				cstemp_value.Format(_T("%.2f"),temp_float_value);
				m_input_list.SetItemText(lRow,INPUT_VALUE,cstemp_value);	
                int temp_jumper = 0;
                if (m_dialog_signal_type != 0xff)  // 0xff 
                {
                    temp_jumper = m_dialog_signal_type >> 4;
                    if ((temp_jumper == 0) || (temp_jumper == 1) || (temp_jumper == 2) || (temp_jumper == 3) || (temp_jumper == 5))
                    {
                        unsigned char temp1;
                        temp1 = m_Input_data.at(lRow).decom;
                        temp1 = temp1 & 0x0f;
                        temp1 = temp1 | (temp_jumper << 4);
                        m_Input_data.at(lRow).decom = temp1;
                    }
                }

			}
			else if((bac_ranges_type == VARIABLE_RANGE_DIGITAL_TYPE) || (bac_ranges_type == INPUT_RANGE_DIGITAL_TYPE) || (bac_ranges_type == OUTPUT_RANGE_DIGITAL_TYPE))
			{
				m_Input_data.at(lRow).digital_analog =  BAC_UNITS_DIGITAL;
				m_Input_data.at(lRow).range =  bac_range_number_choose;
				m_input_list.SetItemText(lRow,INPUT_CAL,_T(""));
				m_input_list.SetCellEnabled(lRow,INPUT_CAL,0);
				m_input_list.SetItemText(lRow,INPUT_UNITE,_T(""));
				m_input_list.SetCellEnabled(lRow,INPUT_UNITE,0);

				

				CStringArray temparray;
				if((bac_range_number_choose >= 23) && (bac_range_number_choose <= 30))
				{
					temp1 = Custom_Digital_Range[bac_range_number_choose - 23];
				}
				else
					temp1 = Digital_Units_Array[bac_range_number_choose];//22 is the sizeof the array
				SplitCStringA(temparray,temp1,_T("/"));
				New_CString = temp1;
				if(m_Input_data.at(lRow).control == 1)//
				{
					if((temparray.GetSize()==2)&&(!temparray.GetAt(1).IsEmpty()))
					{
						m_input_list.SetItemText(lRow,INPUT_VALUE,temparray.GetAt(1));
					}
				}
				else
				{
					if((temparray.GetSize()==2)&&(!temparray.GetAt(0).IsEmpty()))
					{
						m_input_list.SetItemText(lRow,INPUT_VALUE,temparray.GetAt(0));
					}			
				}
				m_input_list.SetItemText(lRow,INPUT_RANGE,temp1);
			}
	}
	else if((bacnet_device_type == PM_T3PT12) && (lCol == INPUT_JUMPER))
	{
		m_input_list.Set_Edit(false);
		return;
	}
	else if (lCol == INPUT_UNITE && bacnet_device_type == PM_THIRD_PARTY_DEVICE) // for bacnet Thirdparty devices Units
	{
		CBacnetUnitsSelection unitDlg;

		bac_range_number_choose = m_Input_data.at(lRow).range;
		unitDlg.DoModal();
		if (!range_cancel)
		{
			m_Input_data.at(lRow).range = bac_range_number_choose;
			CString inputunit;
			MultiByteToWideChar(CP_ACP, 0, (char*)bacnet_engineering_unit_names[m_Input_data.at(lRow).range].pString,
				(int)strlen((char*)bacnet_engineering_unit_names[m_Input_data.at(lRow).range].pString) + 1,
				inputunit.GetBuffer(MAX_PATH), MAX_PATH);
			inputunit.ReleaseBuffer();
			m_input_list.SetItemText(lRow, INPUT_UNITE, inputunit);
			BACNET_APPLICATION_DATA_VALUE* temp_value = new BACNET_APPLICATION_DATA_VALUE();
			temp_value->tag = TPYE_BACAPP_ENUMERATED;
			temp_value->type.Enumerated = m_Input_data.at(lRow).range;
			int ObjectType = OBJECT_ANALOG_INPUT;
			if (m_Input_data.at(lRow).digital_analog == BAC_UNITS_DIGITAL)
				ObjectType = OBJECT_BINARY_INPUT;
			int invoke_id = Bacnet_Write_Properties_Blocking(g_bac_instance, (BACNET_OBJECT_TYPE)ObjectType, m_Input_data_instance.at(lRow), PROP_UNITS, temp_value);
		}
	}
	else{
		return;
	}





	m_input_list.Set_Edit(false);
	
	int cmp_ret = memcmp(&m_temp_Input_data[lRow],&m_Input_data.at(lRow),sizeof(Str_in_point));
	if(cmp_ret!=0)
	{
		

		changed_input_item = lRow;
		m_input_list.SetItemBkColor(lRow,lCol,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Input List Item%d .Changed to \"%s\" "),lRow + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITEINPUT_T3000,(int8_t)lRow,(int8_t)lRow,sizeof(Str_in_point),m_input_dlg_hwnd,temp_task_info,lRow,lCol);
		//SetTimer(UPDATE_INPUT_ONE_ITEM_TIMER,2000,NULL);  wifi 
	}



	*pResult = 0;
}



void CBacnetInput::Reset_Input_Rect()
{
	CRect temp_mynew_rect;
	::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//view

	CRect temp_window;
	GetWindowRect(&temp_window);

	if(window_max)
	{
		CRect temp_mynew_rect;
		::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//view
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height() - DELTA_HEIGHT, NULL);
	}
	else if((temp_window.Width() <= temp_mynew_rect.Width() ) && (temp_window.Height() <= temp_mynew_rect.Height()))
	{
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,0,0,SWP_NOSIZE );
	}
	else
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,700,700, NULL);
	
	//MoveWindow(temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(),1);
	return;
}

static int move_direction = 1;
static long ticktime = 0;
void CBacnetInput::OnTimer(UINT_PTR nIDEvent)
{
	 


	switch(nIDEvent)
	{
		if (hwait_write_thread != NULL) //     og
			break;
	case INPUT_REFRESH_DATA_TIMER:
		{
			if(offline_mode)
				break;
            if(g_protocol == PROTOCOL_BIP_TO_MSTP)
			{
				//PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
			}
			else if((this->IsWindowVisible()) && (Gsm_communication == false) &&  ((this->m_hWnd  == ::GetActiveWindow()) || (bacnet_view_number == TYPE_INPUT))  )	//GSM
			{
				PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
                if ((bac_select_device_online) && (g_protocol == PROTOCOL_BACNET_IP))
                {
                    
                    Post_Refresh_Message(g_bac_instance, READINPUT_T3000, 0, BAC_INPUT_ITEM_COUNT - 1, sizeof(Str_in_point), BAC_INPUT_GROUP);
                }
				else if((bac_select_device_online) && 
                    ((g_protocol == MODBUS_RS485) || 
                     (g_protocol == MODBUS_TCPIP) || 
                     (g_protocol == PROTOCOL_MSTP_TO_MODBUS) ||   //MSTP22 
                     (g_protocol == PROTOCOL_BIP_T0_MSTP_TO_MODBUS)))
				{
					if(read_each_485_fun_thread == NULL)
					{
						hide_485_progress = true;
                        //d file 6 19
						::PostMessage(BacNet_hwd,WM_RS485_MESSAGE,bacnet_device_type, READINPUT_T3000/*BAC_IN*/);//T
					}
				}
			}


		}
		break;
	case 2:	//
		{
			if(this->IsWindowVisible())
				PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
			KillTimer(2);
		}
		break;
	case UPDATE_INPUT_ONE_ITEM_TIMER:
		{
			KillTimer(UPDATE_INPUT_ONE_ITEM_TIMER);
			if (offline_mode)
			{
				break;
			}
			if((bac_select_device_online) && (g_protocol == PROTOCOL_BACNET_IP))
			{
				GetPrivateData_Blocking(g_bac_instance,READINPUT_T3000,changed_input_item,changed_input_item,sizeof(Str_in_point));
			}

			PostMessage(WM_REFRESH_BAC_INPUT_LIST,changed_input_item,REFRESH_ON_ITEM);
		}
		break;
	case 4:
		if ((SPECIAL_BAC_TO_MODBUS) && (bacnet_view_number == TYPE_INPUT) && (Bacnet_Private_Device(selected_product_Node.product_class_id)))
		{
			if (offline_mode)
			{
				break;
			}


//				Post_Refresh_Message(g_bac_instance, READINPUT_T3000, 0, BAC_INPUT_ITEM_COUNT - 1, sizeof(Str_in_point), 0); //lue
		}
		break;
	case 5:
		{
#if 0
			static int test_count = 0;
			if(move_direction == 1)
				test_count = (++ test_count )%4;
			else
				test_count = (++ test_count )%4 + 4;
			CString icon_2_full_path;
			CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_INPUT_PIC); // Picture Control 
			switch(test_count)
			{
			case 0:
				icon_2_full_path = _T("D:\\TestIcon\\Runstep1.ico");

				break;
			case 1:
				icon_2_full_path = _T("D:\\TestIcon\\Runstep2.ico");

				break;
			case 2:
				icon_2_full_path = _T("D:\\TestIcon\\Runstep3.ico");

				break;
			case 3:
				icon_2_full_path = _T("D:\\TestIcon\\Runstep4.ico");
				break;
			case 4:
				icon_2_full_path = _T("D:\\TestIcon\\1Runstep1.ico");

				break;
			case 5:
				icon_2_full_path = _T("D:\\TestIcon\\1Runstep2.ico");

				break;
			case 6:
				icon_2_full_path = _T("D:\\TestIcon\\1Runstep3.ico");

				break;
			case 7:
				icon_2_full_path = _T("D:\\TestIcon\\1Runstep4.ico");
				break;
			default:
				break;
			}
			Bitmap icon_bitmap(icon_2_full_path);

			HICON m_hIcon=ExtractIcon(AfxGetInstanceHandle(),icon_2_full_path,0);
			//if( m_hIcon == NULL )  
			//	return FALSE;
			//((CStatic*)GetDlgItem(IDC_PICYOULUO))->SetIcon(m_hIcon);

			//icon_bitmap=(HBITMAP)LoadImage(AfxGetInstanceHandle(), 
			//	MAKEINTRESOURCE(IDB_BITMAP1), 
			//	IMAGE_BITMAP,0,0, 
			//	LR_LOADMAP3DCOLORS); 


			pWnd->ModifyStyle(0, SS_ICON | SS_CENTERIMAGE);
			pWnd->SetIcon(m_hIcon); 
#endif
		}
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CBacnetInput::OnClose()
{
	 
	ShowWindow(FALSE);
	return;

	m_input_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	CDialogEx::OnClose();
}
void CBacnetInput::OnCancel()
{
	
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
//	CDialogEx::OnCancel();
}


BOOL CBacnetInput::PreTranslateMessage(MSG* pMsg)
{
	
	if((pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN))
	{
		CRect list_rect,win_rect;
		m_input_list.GetWindowRect(list_rect);
		ScreenToClient(&list_rect);
		::GetWindowRect(m_input_dlg_hwnd,win_rect);
		m_input_list.Set_My_WindowRect(win_rect);
		m_input_list.Set_My_ListRect(list_rect);

		m_input_list.Get_clicked_mouse_position();
		return TRUE;
	}
	else if((pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RIGHT))
	{
		long nowticktime=GetTickCount();//
		if(nowticktime - ticktime > 200)
		{
			move_direction = 1;
			CRect temp_rect;
			((CStatic*)GetDlgItem(IDC_STATIC_INPUT_PIC))->GetWindowRect(temp_rect);
			ScreenToClient(&temp_rect);
			if(temp_rect.left > 900)
				temp_rect.left = 900;
			((CStatic*)GetDlgItem(IDC_STATIC_INPUT_PIC))->MoveWindow(temp_rect.left + 4,temp_rect.top,temp_rect.Width(),temp_rect.Height(),1);
			ticktime = nowticktime;
		}

	}
	else if((pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_LEFT))
	{
		long nowticktime=GetTickCount();//
		if(nowticktime - ticktime > 200)
		{
			move_direction = 2;
			CRect temp_rect;
			((CStatic*)GetDlgItem(IDC_STATIC_INPUT_PIC))->GetWindowRect(temp_rect);
			ScreenToClient(&temp_rect);
			if(temp_rect.left < 100)
				temp_rect.left = 100;
			((CStatic*)GetDlgItem(IDC_STATIC_INPUT_PIC))->MoveWindow(temp_rect.left - 4,temp_rect.top,temp_rect.Width(),temp_rect.Height(),1);
			ticktime = nowticktime;
		}
	}
	else if(pMsg->message==WM_NCLBUTTONDBLCLK)
	{
		if(!window_max)
		{
			window_max = true;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//view
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), SWP_SHOWWINDOW);
		}
		else
		{
			window_max = false;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//view
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,500,700,SWP_SHOWWINDOW);
		}

		return 1; 
	}
	else if ((pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F2)) //
	{
		::PostMessage(BacNet_hwd, WM_FRESH_CM_LIST, MENU_CLICK, TYPE_INPUT);
		return TRUE;
	}

    CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    if (pFrame->m_pDialogInfo!=NULL&&pFrame->m_pDialogInfo->IsWindowVisible())
    {
        if (pMsg->message == WM_LBUTTONDOWN||pMsg->message == WM_RBUTTONDOWN)
        {
            ::PostMessage(MainFram_hwd,WM_LBUTTONDOWN,0,0);
        }
    }
   
	return CDialogEx::PreTranslateMessage(pMsg);
}

int GetInputLabelEx(Str_in_point temp_in, CString& ret_label, Point_Net* npoint)
{


	CString temp_des2;
	MultiByteToWideChar(CP_ACP, 0, (char*)temp_in.label, (int)strlen((char*)temp_in.label) + 1,
		ret_label.GetBuffer(MAX_PATH), MAX_PATH);
	ret_label.ReleaseBuffer();
	ret_label = ret_label.Trim();

	if (ret_label.IsEmpty())
	{
		if (npoint == NULL)
		{
			ret_label.Empty();
		}
		else
		{
			ret_label.Format(_T("%dIN%d"), npoint->panel, npoint->number + 1);
		}
	}


	return 1;
}

int GetInputLabel(int index, CString &ret_label, Point_Net * npoint )
{
	if(index >= BAC_INPUT_ITEM_COUNT)
	{
		ret_label.Empty();
		return -1;
	}

	int i = index;
	//if (npoint == NULL)
	//{
	//	ret_label.Format(_T("IN%u"), index + 1);
	//	return 1;
	//}
	//else
		return GetInputLabelEx(m_Input_data.at(i),  ret_label,  npoint);
}


int GetInputFullLabelEx(Str_in_point temp_in, CString& ret_full_label, Point_Net* npoint)
{

	MultiByteToWideChar(CP_ACP, 0, (char*)temp_in.description, (int)strlen((char*)temp_in.description) + 1,
		ret_full_label.GetBuffer(MAX_PATH), MAX_PATH);
	ret_full_label.ReleaseBuffer();
	ret_full_label = ret_full_label.Trim();
	if (ret_full_label.IsEmpty())
	{
		if (npoint == NULL)
		{
			ret_full_label.Empty();
		}
		else
		{
			ret_full_label.Format(_T("%dIN%d"), npoint->panel, npoint->number + 1);
		}
	}
	return 1;
}

int GetInputFullLabel(int index,CString &ret_full_label, Point_Net * npoint)
{
	if(index >= BAC_INPUT_ITEM_COUNT)
	{
		ret_full_label.Empty();
		return -1;
	}
	int i = index;
	//if (npoint == NULL)
	//{
	//	ret_full_label.Format(_T("IN%u"), index + 1);
	//	return 1;
	//}
	//else
		return GetInputFullLabelEx(m_Input_data.at(i), ret_full_label, npoint);
}

int GetInputValueEx(Str_in_point temp_in, CString& ret_cstring, CString& ret_unit, CString& Auto_M, int& digital_value)
{
	CStringArray temparray;
	CString temp1;

	if (temp_in.auto_manual == 1)
	{
		Auto_M = _T("M");
	}
	else
	{
		Auto_M.Empty();
	}

	if (temp_in.digital_analog == BAC_UNITS_ANALOG)
	{
		float temp_float_value;
		temp_float_value = ((float)temp_in.value) / 1000;
		ret_cstring.Format(_T("%.1f"), temp_float_value);

		if (temp_in.range < (sizeof(Input_List_Analog_Units) / sizeof(Input_List_Analog_Units[0])))
		{
			ret_unit = Input_List_Analog_Units[temp_in.range];
			if (temp_in.range == 0)
				ret_unit.Empty();
		}
		else
		{
			ret_unit.Empty();
			return -2;
		}
		digital_value = 2;//analog;
		return 1;
	}
	else if (temp_in.digital_analog == BAC_UNITS_DIGITAL)
	{
		if ((temp_in.range == 0))
		{
			float temp_float_value;
			temp_float_value = ((float)temp_in.value) / 1000;
			ret_cstring.Format(_T("%.1f"), temp_float_value);
		}
		else if ((temp_in.range > 30))
		{
			ret_cstring.Empty();
			return -1;
		}
		else
		{

			if ((temp_in.range < 23) && (temp_in.range != 0))
				temp1 = Digital_Units_Array[temp_in.range];
			else if ((temp_in.range >= 23) && (temp_in.range <= 30))
			{
				if (receive_customer_unit)
					temp1 = Custom_Digital_Range[temp_in.range - 23];
			}


			SplitCStringA(temparray, temp1, _T("/"));
			if ((temparray.GetSize() == 2))
			{
				if (temp_in.control == 0)
				{
					digital_value = 0;//analog;
					ret_cstring = temparray.GetAt(0);
					return 1;
				}
				else
				{
					digital_value = 1;//analog;
					ret_cstring = temparray.GetAt(1);
					return 1;
				}
			}
		}

	}
	return 1;
}

int GetInputValue(int index ,CString &ret_cstring,CString &ret_unit,CString &Auto_M,int &digital_value)
{
	CStringArray temparray;
	CString temp1;
	if(index >= BAC_INPUT_ITEM_COUNT)
	{
		ret_cstring.Empty();
		ret_unit.Empty();
		Auto_M.Empty();
		return -1;
	}
	return GetInputValueEx(m_Input_data.at(index), ret_cstring, ret_unit, Auto_M, digital_value);

	//if(m_Input_data.at(i).auto_manual == 1)
	//{
	//	Auto_M = _T("M");
	//}
	//else
	//{
	//	Auto_M.Empty();
	//}

	//if(m_Input_data.at(i).digital_analog == BAC_UNITS_ANALOG)
	//{
	//	float temp_float_value;
	//	temp_float_value = ((float)m_Input_data.at(i).value) / 1000;
	//	ret_cstring.Format(_T("%.1f"),temp_float_value);

	//	if(m_Input_data.at(i).range <  (sizeof(Input_List_Analog_Units)/sizeof(Input_List_Analog_Units[0])))
	//	{
	//		ret_unit =Input_List_Analog_Units[m_Input_data.at(i).range];
	//		if(m_Input_data.at(i).range == 0)
	//			ret_unit.Empty();
	//	}
	//	else
	//	{
	//		ret_unit.Empty();
	//		return -2;
	//	}
	//	digital_value = 2;//analog;
	//	return 1;
	//}
	//else if(m_Input_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
	//{
	//	if((m_Input_data.at(i).range == 0))
	//	{
	//		float temp_float_value;
	//		temp_float_value = ((float)m_Input_data.at(i).value) / 1000;
	//		ret_cstring.Format(_T("%.1f"),temp_float_value);
	//	}
	//	else if((m_Input_data.at(i).range>30))
	//	{
	//		ret_cstring.Empty();
	//		return -1;
	//	}
	//	else
	//	{

	//		if((m_Input_data.at(i).range < 23) &&(m_Input_data.at(i).range !=0))
	//			temp1 = Digital_Units_Array[m_Input_data.at(i).range];
	//		else if((m_Input_data.at(i).range >=23) && (m_Input_data.at(i).range <= 30))
	//		{
	//			if(receive_customer_unit)
	//				temp1 = Custom_Digital_Range[m_Input_data.at(i).range - 23];
	//		}


	//		SplitCStringA(temparray,temp1,_T("/"));
	//		if((temparray.GetSize()==2))
	//		{
	//			if(m_Input_data.at(i).control == 0)
	//			{
	//				digital_value = 0;//analog;
	//				ret_cstring = temparray.GetAt(0);
	//				return 1;
	//			}
	//			else
	//			{
	//				digital_value = 1;//analog;
	//				ret_cstring = temparray.GetAt(1);
	//				return 1;
	//			}
	//		}
	//	}

	//}
	//return 1;
}


/*
F1>>Help
this is responsible for F1
Go to the Help ID 
*/


BOOL CBacnetInput::OnHelpInfo(HELPINFO* pHelpInfo)
{
	 
	 
	/* if (m_latest_protocol==PROTOCOL_BACNET_IP){*/
		 HWND hWnd;
		 if(pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
		 else
			 hWnd =  ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_INPUTS);
			 return (hWnd != NULL);
// 		}
// 	else{
// 	::HtmlHelp(NULL, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_OVERVIEW);
// 	}
	return CDialogEx::OnHelpInfo(pHelpInfo);
}


void CBacnetInput::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	
	CRect rc;
	GetClientRect(rc);
	if(m_input_list.m_hWnd != NULL)
	{

		::SetWindowPos(this->m_hWnd, HWND_TOP, 0,0, 0,0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
		m_input_list.MoveWindow(&rc);
		//BringWindowToTop();
		//SetActiveWindow();
	}
}


void CBacnetInput::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);
	::SetWindowPos(this->m_hWnd, HWND_TOP, 0,0, 0,0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);

	//BringWindowToTop();
	//SetActiveWindow();
	
	
}


void CBacnetInput::OnSysCommand(UINT nID, LPARAM lParam)
{
	 
	if(nID == SC_MAXIMIZE)
	{
		if(window_max == false)
		{
			window_max = true;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//view
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), SWP_SHOWWINDOW);
		}
		else
		{
			window_max = false;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//view
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,500,700,SWP_SHOWWINDOW);
		}

		return;
	}
	CDialogEx::OnSysCommand(nID, lParam);
}


void CBacnetInput::GetInputUnitandValue(int i, CString& unit, CString& value)
{
	if (m_Input_data.at(i).digital_analog == BAC_UNITS_ANALOG)
	{


		if ((m_Input_data.at(i).range >= 20) && (m_Input_data.at(i).range <= 24))
		{
			unit = Analog_Customer_Units[m_Input_data.at(i).range - 20];
		}
		else if (m_Input_data.at(i).range < (sizeof(Input_List_Analog_Units) / sizeof(Input_List_Analog_Units[0])))
			unit = Input_List_Analog_Units[m_Input_data.at(i).range];
		else if (m_Input_data.at(i).range < (sizeof(Input_Analog_Units_Array) / sizeof(Input_Analog_Units_Array[0])))
			unit = _T("");


		if (bacnet_device_type == PM_THIRD_PARTY_DEVICE)
		{
			CString inputunit;
			MultiByteToWideChar(CP_ACP, 0, (char*)bacnet_engineering_unit_names[m_Input_data.at(i).range].pString,
				(int)strlen((char*)bacnet_engineering_unit_names[m_Input_data.at(i).range].pString) + 1,
				inputunit.GetBuffer(MAX_PATH), MAX_PATH);
			inputunit.ReleaseBuffer();
			unit = inputunit;
		}




		CString cstemp_value;
		float temp_float_value;
		temp_float_value = ((float)m_Input_data.at(i).value) / 1000;
		cstemp_value.Format(_T("%.2f"), temp_float_value);
		value = cstemp_value;


	}
	else if (m_Input_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
	{

		if (m_Input_data.at(i).range == 0)
		{
			CString cstemp_value1;
			float temp_float_value1;
			temp_float_value1 = ((float)m_Input_data.at(i).value) / 1000;
			cstemp_value1.Format(_T("%.2f"), temp_float_value1);
			value = cstemp_value1;

		}

		unit = _T("");

		if ((m_Input_data.at(i).range > 30) || (m_Input_data.at(i).range == 0))
		{

		}
		else
		{

			CStringArray temparray;
			CString temp1;
			if ((m_Input_data.at(i).range < 23) && (m_Input_data.at(i).range != 0))
				temp1 = Digital_Units_Array[m_Input_data.at(i).range];
			else if ((m_Input_data.at(i).range >= 23) && (m_Input_data.at(i).range <= 30))
			{
				if (receive_customer_unit)
					temp1 = Custom_Digital_Range[m_Input_data.at(i).range - 23];
			}

			SplitCStringA(temparray, temp1, _T("/"));
			if ((temparray.GetSize() == 2))
			{
				if (m_Input_data.at(i).control == 0)
					value = temparray.GetAt(0);
				else
					value = temparray.GetAt(1);
			}
		}

	}
}