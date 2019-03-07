// BacnetProgramDebug.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetProgramDebug.h"
#include "global_function.h"
#include "afxdialogex.h"
#include "BacnetRange.h"
unsigned int point_number = 0;
unsigned int point_type = 0;
extern int initial_dialog;

extern int pointtotext_for_controller(char *buf,Point_T3000 *point);
extern char *ispoint(char *token,int *num_point,byte *var_type, byte *point_type, int *num_panel, int *num_net, int network, byte panel, int *netpresent);
#define UPDATE_DEBUG_DATA_TIMER           3
IMPLEMENT_DYNAMIC(CBacnetProgramDebug, CDialogEx)

CBacnetProgramDebug::CBacnetProgramDebug(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetProgramDebug::IDD, pParent)
{

}

CBacnetProgramDebug::~CBacnetProgramDebug()
{
}

void CBacnetProgramDebug::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROGRAM_DEBUG, m_program_debug_list);
	DDX_Control(pDX, IDC_DATETIMEPICKER_PRG_VARIABLE, m_prg_debug_variable_time_picker);
}


BEGIN_MESSAGE_MAP(CBacnetProgramDebug, CDialogEx)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Program_Debug_Item)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PROGRAM_DEBUG, &CBacnetProgramDebug::OnNMClickListProgramDebug)
	ON_WM_TIMER()
	ON_NOTIFY(NM_KILLFOCUS, IDC_DATETIMEPICKER_PRG_VARIABLE, &CBacnetProgramDebug::OnNMKillfocusDatetimepickerPrgVariable)
END_MESSAGE_MAP()


// CBacnetProgramDebug message handlers




BOOL CBacnetProgramDebug::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	SetTimer(UPDATE_DEBUG_DATA_TIMER,5000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CBacnetProgramDebug::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		CRect list_rect,win_rect;
		m_program_debug_list.GetWindowRect(list_rect);
		ScreenToClient(&list_rect);
		::GetWindowRect(m_program_debug_list_hwnd,win_rect);
		m_program_debug_list.Set_My_WindowRect(win_rect);
		m_program_debug_list.Set_My_ListRect(list_rect);

		m_program_debug_list.Get_clicked_mouse_position();
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetProgramDebug::OnCancel()
{
	
	KillTimer(UPDATE_DEBUG_DATA_TIMER);
	CDialogEx::OnCancel();
}


void CBacnetProgramDebug::OnClose()
{
	 
	KillTimer(UPDATE_DEBUG_DATA_TIMER);
	CDialogEx::OnClose();
}




void CBacnetProgramDebug::Initial_List(unsigned int list_type)
{
	m_program_debug_list.ShowWindow(SW_HIDE);
	m_program_debug_list.DeleteAllItems();
	while ( m_program_debug_list.DeleteColumn (0)) ;
	int width_length = 0;
	CTime TimeTemp(2017,1,1,0,0,0);
	m_prg_debug_variable_time_picker.SetFormat(_T("HH:mm"));
	m_prg_debug_variable_time_picker.SetTime(&TimeTemp);
	m_prg_debug_variable_time_picker.ShowWindow(SW_HIDE);
	switch(list_type)
	{
	case BAC_OUT://OUT
		{
			m_program_debug_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
			m_program_debug_list.SetExtendedStyle(m_program_debug_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
			m_program_debug_list.InsertColumn(OUTPUT_NUM, _T("Output"), 60, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
			m_program_debug_list.InsertColumn(OUTPUT_PANEL, _T("Panel"), 60, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);

			m_program_debug_list.InsertColumn(OUTPUT_FULL_LABLE, _T("Full Label"), 140, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(OUTPUT_AUTO_MANUAL, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(OUTPUT_HW_SWITCH, _T("HOA Switch"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(OUTPUT_VALUE, _T("Value"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(OUTPUT_UNITE, _T("Units"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(OUTPUT_RANGE, _T("Range"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);

			m_program_debug_list.InsertColumn(OUTPUT_LOW_VOLTAGE, _T("Low V"), 50, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(OUTPUT_HIGH_VOLTAGE, _T("High V"), 50, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);

			m_program_debug_list.InsertColumn(OUTPUT_PWM_PERIOD, _T("PWM Period"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(OUTPUT_DECOM, _T("Status"), 70, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(OUTPUT_LABLE, _T("Label"), 70, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list_hwnd = this->m_hWnd;
			g_hwnd_now = m_program_debug_list_hwnd;
				CString temp_item;//,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
				//CString temp_des;
				//CString temp_units;


				temp_item.Format(_T("%u-OUT%d"),Station_NUM, point_number+1);
				m_program_debug_list.InsertItem(0,temp_item);


				if(ListCtrlEx::ComboBox == m_program_debug_list.GetColumnType(OUTPUT_RANGE))
				{
					ListCtrlEx::CStrList strlist;
					for (int aaa=0;aaa<(int)sizeof(Units_Type)/sizeof(Units_Type[0]);aaa++)
					{
						strlist.push_back(Units_Type[aaa]);
					}
					m_program_debug_list.SetCellStringList(0, OUTPUT_RANGE, strlist);		
				}

				if(ListCtrlEx::ComboBox == m_program_debug_list.GetColumnType(OUTPUT_DECOM))
				{
					ListCtrlEx::CStrList strlist;
					for (int m=0;m<(int)(sizeof(Output_Decom_Array)/sizeof(Output_Decom_Array[0]));m++)
					{
						strlist.push_back(Output_Decom_Array[m]);
					}
					m_program_debug_list.SetCellStringList(0, OUTPUT_DECOM, strlist);
				}


				width_length = 60 + 140 + 80 + 80 + 80 + 80 + 100 + 80 + 70 + 70 + 50 + 70 + 50 + 50;
				SetWindowPos(NULL,0,0,width_length + 70,120 + 50,SWP_NOMOVE);
				::SetWindowPos(m_program_debug_list.m_hWnd,NULL,0,0,width_length + 10,80,SWP_NOMOVE);
				Fresh_Program_List(list_type);


		}
		break;
	case BAC_IN://IN
		{

			m_program_debug_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
			m_program_debug_list.SetExtendedStyle(m_program_debug_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
			m_program_debug_list.InsertColumn(INPUT_NUM, _T("Input"), 50, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
			m_program_debug_list.InsertColumn(INPUT_PANEL, _T("Panel"), 50, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
			m_program_debug_list.InsertColumn(INPUT_FULL_LABLE, _T("Full Label"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(INPUT_AUTO_MANUAL, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(INPUT_VALUE, _T("Value"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(INPUT_UNITE, _T("Units"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(INPUT_RANGE, _T("Range"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(INPUT_CAL, _T("Calibration"), 70, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(INPUT_CAL_OPERATION, _T("Sign"), 50, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(INPUT_FITLER, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(INPUT_DECOM, _T("Status"), 60, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(INPUT_JUMPER, _T("Jumper"), 90, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(INPUT_LABLE, _T("Label"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list_hwnd = this->m_hWnd;
			g_hwnd_now = m_program_debug_list_hwnd;

			CRect list_rect,win_rect;
			m_program_debug_list.GetWindowRect(list_rect);
			ScreenToClient(&list_rect);
			::GetWindowRect(m_program_debug_list_hwnd,win_rect);
			m_program_debug_list.Set_My_WindowRect(win_rect);
			m_program_debug_list.Set_My_ListRect(list_rect);


				CString temp_item;//,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
				//CString temp_des;
				//CString temp_units;


				temp_item.Format(_T("%u-IN%d"),Station_NUM, point_number+1);
				m_program_debug_list.InsertItem(0,temp_item);



				if(ListCtrlEx::ComboBox == m_program_debug_list.GetColumnType(INPUT_RANGE))
				{
					ListCtrlEx::CStrList strlist;
					for (int i=0;i<(int)sizeof(Units_Type)/sizeof(Units_Type[0]);i++)
					{
						strlist.push_back(Units_Type[i]);
					}
					m_program_debug_list.SetCellStringList(0, INPUT_RANGE, strlist);		
				}

				if(ListCtrlEx::ComboBox == m_program_debug_list.GetColumnType(INPUT_JUMPER))
				{
					ListCtrlEx::CStrList strlist;
					for (int i=0;i<(int)sizeof(JumperStatus)/sizeof(JumperStatus[0]);i++)
					{
						strlist.push_back(JumperStatus[i]);
					}
					m_program_debug_list.SetCellStringList(0, INPUT_JUMPER, strlist);		
				}

				width_length =50 + 100 + 80 + 80 + 80 + 100 + 70 + 50 + 60 + 60 + 90 + 80 + 50 + 60;
				SetWindowPos(NULL,0,0,width_length + 70,120 + 50,SWP_NOMOVE);
				::SetWindowPos(m_program_debug_list.m_hWnd,NULL,0,0,width_length + 10,80,SWP_NOMOVE);
				Fresh_Program_List(list_type);

		}
		break;
	case BAC_VAR://VAR
		{
			m_program_debug_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
			m_program_debug_list.SetExtendedStyle(m_program_debug_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
			m_program_debug_list.InsertColumn(VARIABLE_NUM, _T("Variable"), 70, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
			m_program_debug_list.InsertColumn(VARIABLE_FULL_LABLE, _T("Full Label"), 150, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(VARIABLE_AUTO_MANUAL, _T("Auto/Manual"), 150, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(VARIABLE_VALUE, _T("Value"), 120, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(VARIABLE_UNITE, _T("Units"), 120, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(VARIABLE_LABLE, _T("Label"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list_hwnd = this->m_hWnd;

			g_hwnd_now = m_program_debug_list_hwnd;



			CString temp_item;
			temp_item.Format(_T("%u-VAR%d"),Station_NUM, point_number+1);
			m_program_debug_list.InsertItem(0,temp_item);

			ListCtrlEx::CStrList strlist;

			for (int i=0;i<(int)sizeof(Units_Type)/sizeof(Units_Type[0]);i++)
			{
				strlist.push_back(Units_Type[i]);
			}
			m_program_debug_list.SetCellStringList(0, VARIABLE_UNITE, strlist);	
			width_length = 70 + 150 + 150 + 120 + 120 + 100 + 50;
			SetWindowPos(NULL,0,0,width_length + 70,120 + 50,SWP_NOMOVE);
			::SetWindowPos(m_program_debug_list.m_hWnd,NULL,0,0,width_length - 20,70,SWP_NOMOVE);
			Fresh_Program_List(point_type);
		}
		break;
	case BAC_PID://CON
		{
			m_program_debug_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
			m_program_debug_list.SetExtendedStyle(m_program_debug_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
			m_program_debug_list.InsertColumn(CONTROLLER_NUM, _T("NUM"), 40, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
			m_program_debug_list.InsertColumn(CONTROLLER_INPUT, _T("Input"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(CONTROLLER_INPUTVALUE, _T("Value"), 60, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(CONTROLLER_INPUTUNITS, _T("Units"), 60, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(CONTROLLER_AUTO_MANUAL, _T("A/M"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(CONTROLLER_OUTPUT, _T("Output"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(CONTROLLER_SETPOINT, _T("Setpoint"), 60, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(CONTROLLER_SETVALUE, _T("Set Value"), 60, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(CONTROLLER_SETPOINTUNITS, _T("Units"), 70, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(CONTROLLER_ACTION, _T("Action"), 50, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(CONTROLLER_PROPORTIONAL, _T("Prop"), 50, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(CONTROLLER_RESET, _T("Int"), 50, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(CONTROLLER_I_TIME, _T("Time"), 50, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(CONTROLLER_RATE, _T("Der"), 50, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
			m_program_debug_list.InsertColumn(CONTROLLER_BIAS, _T("Bias"), 50, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);


			m_program_debug_list_hwnd = this->m_hWnd;
			m_program_debug_list.SetListHwnd(this->m_hWnd);

			CString temp_item;
			temp_item.Format(_T("%u-PID%d"),Station_NUM, point_number+1);
			m_program_debug_list.InsertItem(0,temp_item);


			ListCtrlEx::CStrList strlist;

			for (int i=0;i<(int)sizeof(PID_Time_Type)/sizeof(PID_Time_Type[0]);i++)
			{
				strlist.push_back(PID_Time_Type[i]);
			}
			m_program_debug_list.SetCellStringList(0, CONTROLLER_I_TIME, strlist);		
			width_length = 40 + 80 + 60 + 60 + 80 + 100 + 60 + 60 + 70 + 50 +50 +50 +50 +50 +50 +50 +50;
			SetWindowPos(NULL,0,0,width_length + 70,120 + 50,SWP_NOMOVE);
			::SetWindowPos(m_program_debug_list.m_hWnd,NULL,0,0,width_length + 10,80,SWP_NOMOVE);
			Fresh_Program_List(point_type);


		}
		break;
    case BAC_HOL:
    {
        m_program_debug_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
        m_program_debug_list.SetExtendedStyle(m_program_debug_list.GetExtendedStyle() | LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
        m_program_debug_list.InsertColumn(ANNUAL_ROUTINE_NUM, _T("NUM"), 60, ListCtrlEx::CheckBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        m_program_debug_list.InsertColumn(ANNUAL_ROUTINE_FULL_LABEL, _T("Full Label"), 150, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_program_debug_list.InsertColumn(ANNUAL_ROUTINE_AUTO_MANUAL, _T("Auto/Manual"), 90, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_program_debug_list.InsertColumn(ANNUAL_ROUTINE_VALUE, _T("Value"), 80, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_program_debug_list.InsertColumn(ANNUAL_ROUTINE_LABLE, _T("Label"), 90, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);

        m_program_debug_list_hwnd = this->m_hWnd;
        g_hwnd_now = m_program_debug_list_hwnd;

        m_program_debug_list.SetListHwnd(this->m_hWnd);

        m_program_debug_list.DeleteAllItems();
        CString temp_item;
        temp_item.Format(_T("%d"), 0 + 1);
        m_program_debug_list.InsertItem(0, temp_item);
        m_program_debug_list.SetCellEnabled(0, ANNUAL_ROUTINE_NUM, 0);


        if (ListCtrlEx::ComboBox == m_program_debug_list.GetColumnType(ANNUAL_ROUTINE_VALUE))
        {
            ListCtrlEx::CStrList strlist;
            strlist.push_back(_T("OFF"));
            strlist.push_back(_T("ON"));
            m_program_debug_list.SetCellStringList(0, ANNUAL_ROUTINE_VALUE, strlist);
        }

        width_length = 60 + 150 + 90 + 80 + 90;
        SetWindowPos(NULL, 0, 0, width_length + 70, 120 + 50, SWP_NOMOVE);
        ::SetWindowPos(m_program_debug_list.m_hWnd, NULL, 0, 0, width_length + 10, 100, SWP_NOMOVE);
        Fresh_Program_List(list_type);

    }
        break;
    case BAC_SCH:
    {
        m_program_debug_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
        //m_program_debug_list.SetExtendedStyle(m_program_debug_list.GetExtendedStyle() | LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
        m_program_debug_list.InsertColumn(WEEKLY_ROUTINE_NUM, _T("NUM"), 60, ListCtrlEx::CheckBox, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
        m_program_debug_list.InsertColumn(WEEKLY_ROUTINE_FULL_LABLE, _T("Full Label"), 150, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_program_debug_list.InsertColumn(WEEKLY_ROUTINE_AUTO_MANUAL, _T("Auto/Manual"), 90, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_program_debug_list.InsertColumn(WEEKLY_ROUTINE_OUTPUT, _T("Output"), 80, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_program_debug_list.InsertColumn(WEEKLY_ROUTINE_HOLIDAY1, _T("Holiday1"), 90, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_program_debug_list.InsertColumn(WEEKLY_ROUTINE_STATE1, _T("State1"), 70, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_program_debug_list.InsertColumn(WEEKLY_ROUTINE_HOLIDAY2, _T("Holiday2"), 90, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_program_debug_list.InsertColumn(WEEKLY_ROUTINE_STATE2, _T("State2"), 70, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_program_debug_list.InsertColumn(WEEKLY_ROUTINE_LABEL, _T("Label"), 90, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);

        m_program_debug_list_hwnd = this->m_hWnd;
        g_hwnd_now = m_program_debug_list_hwnd;

        m_program_debug_list.SetListHwnd(this->m_hWnd);

        m_program_debug_list.DeleteAllItems();
        CString temp_item;
        temp_item.Format(_T("%d"), 0 + 1);
        m_program_debug_list.InsertItem(0, temp_item);
        m_program_debug_list.SetCellEnabled(0, WEEKLY_ROUTINE_NUM, 0);

        if (ListCtrlEx::ComboBox == m_program_debug_list.GetColumnType(WEEKLY_ROUTINE_AUTO_MANUAL))
        {
            ListCtrlEx::CStrList strlist;
            strlist.push_back(_T("Auto"));
            strlist.push_back(_T("Manual"));
            m_program_debug_list.SetCellStringList(0, WEEKLY_ROUTINE_AUTO_MANUAL, strlist);
        }

        if (ListCtrlEx::ComboBox == m_program_debug_list.GetColumnType(WEEKLY_ROUTINE_OUTPUT))
        {
            ListCtrlEx::CStrList strlist;
            strlist.push_back(_T("OFF"));
            strlist.push_back(_T("ON"));
            m_program_debug_list.SetCellStringList(0, WEEKLY_ROUTINE_OUTPUT, strlist);
        }

        width_length = 60 + 150 + 90 + 80 + 90 + 70 + 90 + 70 + 90;
        SetWindowPos(NULL, 0, 0, width_length + 70, 120 + 50, SWP_NOMOVE);
        ::SetWindowPos(m_program_debug_list.m_hWnd, NULL, 0, 0, width_length + 10, 100, SWP_NOMOVE);
        Fresh_Program_List(list_type);

    }
        break;
	default:
		break;
	}
	m_program_debug_list.ShowWindow(SW_SHOW);
	return;
}



int CBacnetProgramDebug::Fresh_Program_List(unsigned int list_type)
{


		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		switch(list_type)
		{
		case BAC_OUT://OUT
			{
					CString temp1;
				MultiByteToWideChar( CP_ACP, 0, (char *)m_Output_data.at(point_number).description, (int)strlen((char *)m_Output_data.at(point_number).description)+1, 
					temp_des.GetBuffer(MAX_PATH), MAX_PATH );
				temp_des.ReleaseBuffer();

				m_program_debug_list.SetItemText(0,OUTPUT_FULL_LABLE,temp_des);


				int digital_special_output_count = 0;
				int analog_special_output_count = 0;

				m_program_debug_list.SetItemText(0,OUTPUT_HW_SWITCH,_T(""));

				CString low_voltage,high_voltage;
				if(m_Output_data.at(point_number).low_voltage == 0)
					low_voltage.Empty();
				else
					low_voltage.Format(_T("%.1f"),m_Output_data.at(point_number).low_voltage/10);

				if(m_Output_data.at(point_number).high_voltage == 0)
					high_voltage.Empty();
				else
					high_voltage.Format(_T("%.1f"),m_Output_data.at(point_number).high_voltage/10);

				m_program_debug_list.SetItemText(0,OUTPUT_LOW_VOLTAGE,low_voltage);	
				m_program_debug_list.SetItemText(0,OUTPUT_HIGH_VOLTAGE,high_voltage);	


				if((bacnet_device_type == BIG_MINIPANEL || bacnet_device_type == MINIPANELARM || bacnet_device_type == MINIPANELARM_TB || bacnet_device_type == MINIPANELARM_LB || bacnet_device_type == MINIPANELARM_LB || bacnet_device_type == MINIPANELARM_TB) || ((bacnet_device_type == SMALL_MINIPANEL)) || (bacnet_device_type == TINY_MINIPANEL) || (bacnet_device_type == TINY_EX_MINIPANEL))
				{
					if (bacnet_device_type == BIG_MINIPANEL || bacnet_device_type == MINIPANELARM)
					{
						digital_special_output_count = BIG_MINIPANEL_OUT_D;
						analog_special_output_count = BIG_MINIPANEL_OUT_A;
					}
					else if(bacnet_device_type == SMALL_MINIPANEL || bacnet_device_type == MINIPANELARM_LB)
					{
						digital_special_output_count = SMALL_MINIPANEL_OUT_D;
						analog_special_output_count = SMALL_MINIPANEL_OUT_A;
					}
					else if(bacnet_device_type == TINY_MINIPANEL)
					{
						digital_special_output_count = TINY_MINIPANEL_OUT_D;
						analog_special_output_count = TINY_MINIPANEL_OUT_A;
					}
					else if (bacnet_device_type == TINY_EX_MINIPANEL || bacnet_device_type == MINIPANELARM_TB)
					{
						digital_special_output_count = TINYEX_MINIPANEL_OUT_D;
						analog_special_output_count = TINYEX_MINIPANEL_OUT_A;
					}
					if(point_number < (digital_special_output_count +analog_special_output_count) )
					{
						if(m_Output_data.at(point_number).hw_switch_status == HW_SW_OFF)
						{
							m_program_debug_list.SetItemText(0,OUTPUT_HW_SWITCH,_T("MAN-OFF"));
							m_program_debug_list.SetCellEnabled(0,OUTPUT_AUTO_MANUAL,0);
						}
						else if(m_Output_data.at(point_number).hw_switch_status == HW_SW_HAND)
						{
							m_program_debug_list.SetItemText(0,OUTPUT_HW_SWITCH,_T("MAN-ON"));
							m_program_debug_list.SetCellEnabled(0,OUTPUT_AUTO_MANUAL,0);
						}
						else
						{
							m_program_debug_list.SetItemText(0,OUTPUT_HW_SWITCH,_T("AUTO"));
							m_program_debug_list.SetCellEnabled(0,OUTPUT_AUTO_MANUAL,1);
							if(m_Output_data.at(point_number).auto_manual==0)	//In output table if it is auto ,the value can't be edit by user
							{
								m_program_debug_list.SetItemText(0,OUTPUT_AUTO_MANUAL,_T("Auto"));
								//m_program_debug_list.SetCellEnabled(i,OUTPUT_VALUE,0);
							}
							else
							{
								m_program_debug_list.SetItemText(0,OUTPUT_AUTO_MANUAL,_T("Manual"));
								//m_program_debug_list.SetCellEnabled(i,OUTPUT_VALUE,1);
							}
						}
					}
					else
					{
						m_program_debug_list.SetItemText(0,OUTPUT_HW_SWITCH,_T("AUTO"));
						m_program_debug_list.SetCellEnabled(0,OUTPUT_AUTO_MANUAL,1);
						if(m_Output_data.at(point_number).auto_manual==0)	//In output table if it is auto ,the value can't be edit by user
						{
							m_program_debug_list.SetItemText(0,OUTPUT_AUTO_MANUAL,_T("Auto"));
							//m_program_debug_list.SetCellEnabled(i,OUTPUT_VALUE,0);
						}
						else
						{
							m_Output_data.at(point_number).auto_manual = 1;
							m_program_debug_list.SetItemText(0,OUTPUT_AUTO_MANUAL,_T("Manual"));
							//m_program_debug_list.SetCellEnabled(i,OUTPUT_VALUE,1);
						}
					}
				}
				else
				{
					m_program_debug_list.SetCellEnabled(0,OUTPUT_AUTO_MANUAL,1);
					if(m_Output_data.at(point_number).auto_manual==0)	//In output table if it is auto ,the value can't be edit by user
					{
						m_program_debug_list.SetItemText(0,OUTPUT_AUTO_MANUAL,_T("Auto"));
						//m_program_debug_list.SetCellEnabled(i,OUTPUT_VALUE,0);
					}
					else
					{
						m_Output_data.at(point_number).auto_manual = 1;
						m_program_debug_list.SetItemText(0,OUTPUT_AUTO_MANUAL,_T("Manual"));
						//m_program_debug_list.SetCellEnabled(i,OUTPUT_VALUE,1);
					}
				}




				m_program_debug_list.SetCellEnabled(0,OUTPUT_UNITE,0);
				if(m_Output_data.at(point_number).digital_analog == BAC_UNITS_ANALOG)
				{
					m_program_debug_list.SetCellEnabled(0,OUTPUT_LOW_VOLTAGE,1);
					m_program_debug_list.SetCellEnabled(0,OUTPUT_HIGH_VOLTAGE,1);
					m_program_debug_list.SetCellEnabled(0,OUTPUT_PWM_PERIOD,1);

					if(m_Output_data.at(point_number).range == 0)
						m_program_debug_list.SetItemText(0,OUTPUT_RANGE,_T("Unused"));
					else if(m_Output_data.at(point_number).range < (sizeof(OutPut_List_Analog_Range)/sizeof(OutPut_List_Analog_Range[0])))
						m_program_debug_list.SetItemText(0,OUTPUT_RANGE,OutPut_List_Analog_Range[m_Output_data.at(point_number).range]);
					else
						m_program_debug_list.SetItemText(0,OUTPUT_RANGE,_T("Out of range"));

					if(m_Output_data.at(point_number).range < (sizeof(OutPut_List_Analog_Units)/sizeof(OutPut_List_Analog_Units[0])))
						m_program_debug_list.SetItemText(0,OUTPUT_UNITE,OutPut_List_Analog_Units[m_Output_data.at(point_number).range]);
					else
						m_program_debug_list.SetItemText(0,OUTPUT_UNITE,_T("Unused"));

					CString temp_low,temp_high;
					temp_low.Format(_T("%d"),0);
					temp_high.Format(_T("%d"),0);


					CString temp_value;
					temp_value.Format(_T("%.2f"),((float)m_Output_data.at(point_number).value) / 1000);
					m_program_debug_list.SetItemText(0,OUTPUT_VALUE,temp_value);

				}
				else if(m_Output_data.at(point_number).digital_analog == BAC_UNITS_DIGITAL)
				{
					m_program_debug_list.SetCellEnabled(0,OUTPUT_LOW_VOLTAGE,0);
					m_program_debug_list.SetCellEnabled(0,OUTPUT_HIGH_VOLTAGE,0);
					m_program_debug_list.SetCellEnabled(0,OUTPUT_PWM_PERIOD,0);
					m_program_debug_list.SetItemText(0,OUTPUT_UNITE,_T(""));

					if(m_Output_data.at(point_number).range == 0)
					{
						CString temp_value2;
						temp_value2.Format(_T("%.2f"),((float)m_Output_data.at(point_number).value) / 1000);
						m_program_debug_list.SetItemText(0,OUTPUT_VALUE,temp_value2);

						m_program_debug_list.SetItemText(0,OUTPUT_RANGE,Digital_Units_Array[0]);
					}
					else if(m_Output_data.at(point_number).range<=22)
						m_program_debug_list.SetItemText(0,OUTPUT_RANGE,Digital_Units_Array[m_Output_data.at(point_number).range]);
					else if((m_Output_data.at(point_number).range >= 23) && (m_Output_data.at(point_number).range <= 30))
					{
						if(receive_customer_unit)
							m_program_debug_list.SetItemText(0,OUTPUT_RANGE,Custom_Digital_Range[m_Output_data.at(point_number).range - 23]);
						else
							m_program_debug_list.SetItemText(0,OUTPUT_RANGE,Digital_Units_Array[0]);
					}
					else
						m_program_debug_list.SetItemText(0,OUTPUT_RANGE,Digital_Units_Array[0]);

					if((m_Output_data.at(point_number).range>30) || (m_Output_data.at(point_number).range == 0))
					{
						//m_program_debug_list.SetItemText(i,OUTPUT_UNITE,Digital_Units_Array[0]);
					}
					else
					{

						CStringArray temparray;

						if((m_Output_data.at(point_number).range < 23) &&(m_Output_data.at(point_number).range !=0))
							temp1 = Digital_Units_Array[m_Output_data.at(point_number).range];
						else if((m_Output_data.at(point_number).range >=23) && (m_Output_data.at(point_number).range <= 30))
						{
							if(receive_customer_unit)
								temp1 = Custom_Digital_Range[m_Output_data.at(point_number).range - 23];
						}
						else
						{
							temp1.Empty();
							m_program_debug_list.SetItemText(0,OUTPUT_VALUE,_T(""));
						}

						SplitCStringA(temparray,temp1,_T("/"));
						if((temparray.GetSize()==2))
						{
							if(m_Output_data.at(point_number).control == 0)
								m_program_debug_list.SetItemText(0,OUTPUT_VALUE,temparray.GetAt(0));
							else
								m_program_debug_list.SetItemText(0,OUTPUT_VALUE,temparray.GetAt(1));
						}
					}
				}

				if(m_Output_data.at(point_number).decom==0)
					temp_status.Format(Output_Decom_Array[0]);
				else if(m_Output_data.at(point_number).decom==1)
					temp_status.Format(Output_Decom_Array[1]);
				else
					temp_status.Empty();
				m_program_debug_list.SetItemText(0,OUTPUT_DECOM,temp_status);

				CString temp_pwm_period;
				temp_pwm_period.Format(_T("%u"),(unsigned char)m_Output_data.at(point_number).pwm_period);
				m_program_debug_list.SetItemText(0,OUTPUT_PWM_PERIOD,temp_pwm_period);

				CString temp_des2;
				MultiByteToWideChar( CP_ACP, 0, (char *)m_Output_data.at(point_number).label, (int)strlen((char *)m_Output_data.at(point_number).label)+1, 
					temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
				temp_des2.ReleaseBuffer();
				m_program_debug_list.SetItemText(0,OUTPUT_LABLE,temp_des2);

			
			}
			break;
		case BAC_IN://IN
			{
				CString temp1;


					CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
					CString temp_des;
					CString temp_units;


					MultiByteToWideChar( CP_ACP, 0, (char *)m_Input_data.at(point_number).description, (int)strlen((char *)m_Input_data.at(point_number).description)+1, 
						temp_des.GetBuffer(MAX_PATH), MAX_PATH );
					temp_des.ReleaseBuffer();

					m_program_debug_list.SetItemText(0,INPUT_FULL_LABLE,temp_des);
					if(m_Input_data.at(point_number).auto_manual==0)
					{
						m_program_debug_list.SetItemText(0,INPUT_AUTO_MANUAL,_T("Auto"));
					}
					else
					{
						m_program_debug_list.SetItemText(0,INPUT_AUTO_MANUAL,_T("Manual"));
					}

					if(m_Input_data.at(point_number).digital_analog == BAC_UNITS_ANALOG)
					{

						m_program_debug_list.SetCellEnabled(0,INPUT_CAL,1);

						if(m_Input_data.at(point_number).range <  (sizeof(Input_List_Analog_Units)/sizeof(Input_List_Analog_Units[0])))
							m_program_debug_list.SetItemText(0,INPUT_UNITE,Input_List_Analog_Units[m_Input_data.at(point_number).range]);
						else if(m_Input_data.at(point_number).range < (sizeof(Input_Analog_Units_Array)/sizeof(Input_Analog_Units_Array[0])))
							m_program_debug_list.SetItemText(0,INPUT_UNITE,_T(""));
						else
							m_program_debug_list.SetItemText(0,INPUT_RANGE,_T("Unused"));

						if(m_Input_data.at(point_number).range == 0)
							m_program_debug_list.SetItemText(0,INPUT_RANGE,_T("Unused"));
						else if(m_Input_data.at(point_number).range <  (sizeof(Input_Analog_Units_Array)/sizeof(Input_Analog_Units_Array[0])))
							m_program_debug_list.SetItemText(0,INPUT_RANGE,Input_Analog_Units_Array[m_Input_data.at(point_number).range]);
						else
							m_program_debug_list.SetItemText(0,INPUT_RANGE,_T("Out of range"));





						CString cstemp_value;
						float temp_float_value;
						temp_float_value = ((float)m_Input_data.at(point_number).value) / 1000;
						cstemp_value.Format(_T("%.2f"),temp_float_value);
						m_program_debug_list.SetItemText(0,INPUT_VALUE,cstemp_value);

						unsigned short temp_cal_value = ((unsigned char)(m_Input_data.at(point_number).calibration_h)) *256 + (unsigned char)m_Input_data.at(point_number).calibration_l;

						temp_cal.Format(_T("%.1f"),((float)temp_cal_value)/10);
						m_program_debug_list.SetItemText(0,INPUT_CAL,temp_cal);
						if(m_Input_data.at(point_number).calibration_sign == 0)
						{
							m_program_debug_list.SetItemText(0,INPUT_CAL_OPERATION,_T("+"));
						}
						else
						{
							m_program_debug_list.SetItemText(0,INPUT_CAL_OPERATION,_T("-"));
						}
					}
					else if(m_Input_data.at(point_number).digital_analog == BAC_UNITS_DIGITAL)
					{

						m_program_debug_list.SetItemText(0,INPUT_CAL,_T(""));

						if(m_Input_data.at(point_number).range == 0)
						{
							CString cstemp_value1;
							float temp_float_value1;
							temp_float_value1 = ((float)m_Input_data.at(point_number).value) / 1000;
							cstemp_value1.Format(_T("%.2f"),temp_float_value1);
							m_program_debug_list.SetItemText(0,INPUT_VALUE,cstemp_value1);

							m_program_debug_list.SetItemText(0,INPUT_RANGE,Digital_Units_Array[0]);
						}
						else if(m_Input_data.at(point_number).range<=22)
							m_program_debug_list.SetItemText(0,INPUT_RANGE,Digital_Units_Array[m_Input_data.at(point_number).range]);
						else if((m_Input_data.at(point_number).range >= 23) && (m_Input_data.at(point_number).range <= 30))
						{
							if(receive_customer_unit)
								m_program_debug_list.SetItemText(0,INPUT_RANGE,Custom_Digital_Range[m_Input_data.at(point_number).range - 23]);
							else
								m_program_debug_list.SetItemText(0,INPUT_RANGE,Digital_Units_Array[0]);
						}
						else
							m_program_debug_list.SetItemText(0,INPUT_RANGE,Digital_Units_Array[0]);
						m_program_debug_list.SetItemText(0,INPUT_UNITE,_T(""));

						if((m_Input_data.at(point_number).range>30)  || (m_Input_data.at(point_number).range == 0))
						{
							//m_program_debug_list.SetItemText(i,INPUT_UNITE,Digital_Units_Array[0]);
						}
						else
						{

							CStringArray temparray;

							if((m_Input_data.at(point_number).range < 23) &&(m_Input_data.at(point_number).range !=0))
								temp1 = Digital_Units_Array[m_Input_data.at(point_number).range];
							else if((m_Input_data.at(point_number).range >=23) && (m_Input_data.at(point_number).range <= 30))
							{
								if(receive_customer_unit)
									temp1 = Custom_Digital_Range[m_Input_data.at(point_number).range - 23];
							}


							//temp1 = Digital_Units_Array[m_Input_data.at(i).range];
							SplitCStringA(temparray,temp1,_T("/"));
							if((temparray.GetSize()==2))
							{
								if(m_Input_data.at(point_number).control == 0)
									m_program_debug_list.SetItemText(0,INPUT_VALUE,temparray.GetAt(0));
								else
									m_program_debug_list.SetItemText(0,INPUT_VALUE,temparray.GetAt(1));
							}
						}

					}


					//temp_filter.Format(_T("%d"),(int)pow((double)2,(int)m_Input_data.at(i).filter));
					temp_filter.Format(_T("%d"),(unsigned char)m_Input_data.at(point_number).filter);
					m_program_debug_list.SetItemText(0,INPUT_FITLER,temp_filter);

					int temp_decom = 0;
					int temp_jumper = 0;
					temp_decom = m_Input_data.at(point_number).decom & 0x0f;
					temp_jumper = (m_Input_data.at(point_number).decom & 0xf0 ) >> 4;

					//如果range 是0 或者 不在正常范围内，就不要显示 open short 的报警 状态;
					if((temp_decom==0) || (m_Input_data.at(point_number).range == 0) || (m_Input_data.at(point_number).range > 30))
					{
						temp_status.Format(Decom_Array[0]);
						m_program_debug_list.SetItemTextColor(0,INPUT_DECOM,RGB(0,0,0),false);
					}
					else if(temp_decom==1)
					{
						temp_status.Format(Decom_Array[1]);
						m_program_debug_list.SetItemTextColor(0,INPUT_DECOM,RGB(255,0,0),false);
					}
					else if(temp_decom==2)
					{
						temp_status.Format(Decom_Array[2]);
						m_program_debug_list.SetItemTextColor(0,INPUT_DECOM,RGB(255,0,0),false);
					}
					else
					{
						temp_status.Empty();
						m_program_debug_list.SetItemTextColor(0,INPUT_DECOM,RGB(0,0,0),false);
					}
					m_program_debug_list.SetItemText(0,INPUT_DECOM,temp_status);


					if(temp_jumper == 1)
					{
						temp_status.Format(JumperStatus[1]);
					}
					else if(temp_jumper == 2)
					{
						temp_status.Format(JumperStatus[2]);
					}
					else if(temp_jumper == 3)
					{
						temp_status.Format(JumperStatus[3]);
					}
					else if(temp_jumper == 0)
					{
						temp_status.Format(JumperStatus[0]);
					}
					else
					{
						temp_status.Format(JumperStatus[0]);
						m_Input_data.at(point_number).decom = m_Input_data.at(point_number).decom & 0x0f;	 //如果最高位不是 有效值，清零;
					}
					m_program_debug_list.SetItemText(0,INPUT_JUMPER,temp_status);

					CString temp_des2;
					MultiByteToWideChar( CP_ACP, 0, (char *)m_Input_data.at(point_number).label, (int)strlen((char *)m_Input_data.at(point_number).label)+1, 
						temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
					temp_des2.ReleaseBuffer();

					m_program_debug_list.SetItemText(0,INPUT_LABLE,temp_des2);



#pragma region External_info
					CString main_sub_panel;
					if((m_Input_data.at(point_number).sub_id !=0) &&
						//(m_Input_data.at(input_list_line).sub_number !=0) &&
						(m_Input_data.at(point_number).sub_product !=0))
					{
						unsigned char temp_pid = m_Input_data.at(point_number).sub_product;
						if((temp_pid == PM_T3PT10) ||
							(temp_pid == PM_T3IOA) ||
							(temp_pid == PM_T332AI) ||
							(temp_pid == PM_T38AI16O) ||
							(temp_pid == PM_T38I13O) ||
							(temp_pid == PM_T34AO) ||
							(temp_pid == PM_T322AI) ||
							(temp_pid == PM_T38AI8AO6DO) ||
							(temp_pid == PM_T36CT)||
							(temp_pid == PM_T36CTA)||
							(temp_pid == PM_T3_LC)
							)
						{
							//m_input_item_info.ShowWindow(true);
							CString temp_name;
							temp_name = GetProductName(m_Input_data.at(point_number).sub_product);
							CString show_info;
							CString temp_id;
							CString temp_number;

							main_sub_panel.Format(_T("%d-%d"),(unsigned char)Station_NUM,(unsigned char)m_Input_data.at(point_number).sub_id);
							m_program_debug_list.SetItemText(0,INPUT_PANEL,main_sub_panel);
						}	
						else
						{
							main_sub_panel.Format(_T("%d"),(unsigned char)Station_NUM);
							m_program_debug_list.SetItemText(0,INPUT_PANEL,main_sub_panel);
						}
					}
					else
					{
						main_sub_panel.Format(_T("%d"),(unsigned char)Station_NUM);
						m_program_debug_list.SetItemText(0,INPUT_PANEL,main_sub_panel);
					}

#pragma endregion External_info

				
			}
			break;
		case BAC_VAR://VAR
			{

				MultiByteToWideChar( CP_ACP, 0, (char *)m_Variable_data.at(point_number).description, (int)strlen((char *)m_Variable_data.at(point_number).description)+1, 
					temp_des.GetBuffer(MAX_PATH), MAX_PATH );
				temp_des.ReleaseBuffer();
				m_program_debug_list.SetItemText(0,VARIABLE_FULL_LABLE,temp_des);
				if(m_Variable_data.at(point_number).auto_manual==0)
				{
					m_program_debug_list.SetItemText(0,VARIABLE_AUTO_MANUAL,_T("Auto"));
				}
				else
				{
					m_program_debug_list.SetItemText(0,VARIABLE_AUTO_MANUAL,_T("Manual"));
				}


				if(m_Variable_data.at(point_number).digital_analog == BAC_UNITS_DIGITAL)
				{

					if((m_Variable_data.at(point_number).range == 0) || (m_Variable_data.at(point_number).range>30))
					{
						CString cstemp_value2;
						float temp_float_value1;
						temp_float_value1 = ((float)m_Variable_data.at(point_number).value) / 1000;
						cstemp_value2.Format(_T("%.3f"),temp_float_value1);
						m_program_debug_list.SetItemText(0,VARIABLE_VALUE,cstemp_value2);
						m_program_debug_list.SetItemText(0,VARIABLE_UNITE,Variable_Analog_Units_Array[0]);
					}
					else
					{
						CString temp1;
						CStringArray temparray;

						if((m_Variable_data.at(point_number).range < 23) &&(m_Variable_data.at(point_number).range !=0))
							temp1 = Digital_Units_Array[m_Variable_data.at(point_number).range];
						else if((m_Variable_data.at(point_number).range >=23) && (m_Variable_data.at(point_number).range <= 30))
						{
							if(receive_customer_unit)
								temp1 = Custom_Digital_Range[m_Variable_data.at(point_number).range - 23];
						}
						else
						{
							temp1 = Digital_Units_Array[0];
							m_program_debug_list.SetItemText(0,VARIABLE_UNITE,temp1);
						}

						SplitCStringA(temparray,temp1,_T("/"));
						if((temparray.GetSize()==2))
						{
							if(m_Variable_data.at(point_number).control == 0)
								m_program_debug_list.SetItemText(0,VARIABLE_VALUE,temparray.GetAt(0));
							else
								m_program_debug_list.SetItemText(0,VARIABLE_VALUE,temparray.GetAt(1));
							m_program_debug_list.SetItemText(0,VARIABLE_UNITE,temp1);
						}

					}

				}
				else
				{
					if(m_Variable_data.at(point_number).range == 20)	//如果是时间;
					{
						m_program_debug_list.SetItemText(0,VARIABLE_UNITE,Variable_Analog_Units_Array[m_Variable_data.at(point_number).range]);
						char temp_char[50];
						int time_seconds = m_Variable_data.at(point_number).value / 1000;
						intervaltotextfull(temp_char,time_seconds,0,0);
						CString temp_11;
						MultiByteToWideChar( CP_ACP, 0, temp_char, strlen(temp_char) + 1, 
							temp_11.GetBuffer(MAX_PATH), MAX_PATH );
						temp_11.ReleaseBuffer();		
						m_program_debug_list.SetItemText(0,VARIABLE_VALUE,temp_11);
					}
					else if(m_Variable_data.at(point_number).range<=sizeof(Variable_Analog_Units_Array)/sizeof(Variable_Analog_Units_Array[0]))
					{
						m_program_debug_list.SetItemText(0,VARIABLE_UNITE,Variable_Analog_Units_Array[m_Variable_data.at(point_number).range]);

						CString cstemp_value;
						float temp_float_value;
						temp_float_value = ((float)m_Variable_data.at(point_number).value) / 1000;
						cstemp_value.Format(_T("%.3f"),temp_float_value);
						m_program_debug_list.SetItemText(0,VARIABLE_VALUE,cstemp_value);
					}
					else
					{
						m_program_debug_list.SetItemText(0,VARIABLE_UNITE,Variable_Analog_Units_Array[0]);

						CString cstemp_value;
						float temp_float_value;
						temp_float_value = ((float)m_Variable_data.at(point_number).value) / 1000;
						cstemp_value.Format(_T("%.3f"),temp_float_value);
						m_program_debug_list.SetItemText(0,VARIABLE_VALUE,cstemp_value);
					}
					
				}

				CString temp_des2;
				MultiByteToWideChar( CP_ACP, 0, (char *)m_Variable_data.at(point_number).label, (int)strlen((char *)m_Variable_data.at(point_number).label)+1, 
					temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
				temp_des2.ReleaseBuffer();
				m_program_debug_list.SetItemText(0,VARIABLE_LABLE,temp_des2);
			}
			break;
		case BAC_PID:
			{
				CString temp_des2;
				CString temp_des3;
				CString temp_set_unit;
				temp_des2.Empty();

				if( (BAC_IN + 1) == m_controller_data.at(point_number).input.point_type)
				{
					if(m_controller_data.at(point_number).input.number< BAC_INPUT_ITEM_COUNT)
					{	
						MultiByteToWideChar( CP_ACP, 0, (char *)m_Input_data.at(m_controller_data.at(point_number).input.number ).label, 
							(int)strlen((char *)m_Input_data.at(m_controller_data.at(point_number).input.number ).label)+1, 
							temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
						temp_des2.ReleaseBuffer();	

						//如果是小叶的设备,因为没有input 就直接显示in2之类.
						if(bacnet_device_type == STM32_HUM_NET)
						{
							temp_des2.Empty();
						}


						if(temp_des2.GetLength()>9)
							temp_des2.Format(_T("%d-IN%d"),m_controller_data.at(point_number).input.panel,m_controller_data.at(point_number).input.number + 1);
						if(temp_des2.IsEmpty())
							temp_des2.Format(_T("%d-IN%d"),m_controller_data.at(point_number).input.panel,m_controller_data.at(point_number).input.number + 1);

						//temp_des3.Format(_T("%d"),m_Input_data.at(m_controller_data.at(i).input.number - 1).value);	
						CString cstemp_value;
						float temp_float_value;
						temp_float_value = ((float)m_controller_data.at(point_number).input_value) / 1000;

						cstemp_value.Format(_T("%.1f"),temp_float_value);
						m_program_debug_list.SetItemText(point_number,CONTROLLER_INPUTVALUE,cstemp_value);


						//********************************************************************************************************
						// bind the the input units.
						int x = m_controller_data.at(point_number).input.number;
						if(m_Input_data.at(x).digital_analog == BAC_UNITS_ANALOG)
						{
							m_program_debug_list.SetItemText(0,CONTROLLER_INPUTUNITS,Input_List_Analog_Units[m_Input_data.at(x).range]);
						}

						//如果是小叶的设备,因为没有input 就直接显示rang 对应的值.
						if(bacnet_device_type == STM32_HUM_NET)
						{
							if(m_controller_data.at(point_number).units < sizeof(Input_List_Analog_Units)/sizeof(Input_List_Analog_Units[0]))
							{
								m_program_debug_list.SetItemText(0,CONTROLLER_INPUTUNITS,Input_List_Analog_Units[m_controller_data.at(point_number).units]);
							}
						}

					}
				}
				else if((BAC_VAR + 1) == m_controller_data.at(point_number).input.point_type)
				{
					if(m_controller_data.at(point_number).input.number< BAC_VARIABLE_ITEM_COUNT)
					{	
						MultiByteToWideChar( CP_ACP, 0, (char *)m_Variable_data.at(m_controller_data.at(point_number).input.number ).label, 
							(int)strlen((char *)m_Variable_data.at(m_controller_data.at(point_number).input.number ).label)+1, 
							temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
						temp_des2.ReleaseBuffer();	



						if(temp_des2.GetLength()>9)
							temp_des2.Format(_T("%d-VAR%d"),m_controller_data.at(point_number).input.panel,m_controller_data.at(point_number).input.number + 1);
						if(temp_des2.IsEmpty())
							temp_des2.Format(_T("%d-VAR%d"),m_controller_data.at(point_number).input.panel,m_controller_data.at(point_number).input.number + 1);

						//temp_des3.Format(_T("%d"),m_Input_data.at(m_controller_data.at(i).input.number - 1).value);	
						CString cstemp_value;
						float temp_float_value;
						temp_float_value = ((float)m_controller_data.at(point_number).input_value) / 1000;

						cstemp_value.Format(_T("%.1f"),temp_float_value);
						m_program_debug_list.SetItemText(0,CONTROLLER_INPUTVALUE,cstemp_value);


						//********************************************************************************************************
						// bind the the input units.
						int x = m_controller_data.at(point_number).input.number;
						if(m_Variable_data.at(x).digital_analog == BAC_UNITS_ANALOG)
						{
							if(m_Variable_data.at(x).range <= sizeof(Variable_Analog_Units_Array)/sizeof(Variable_Analog_Units_Array[0]))
								m_program_debug_list.SetItemText(0,CONTROLLER_INPUTUNITS,Variable_Analog_Units_Array[m_Variable_data.at(x).range]);
						}

					}
				}
				else
				{
					m_program_debug_list.SetItemText(0,CONTROLLER_INPUTVALUE,_T(""));
					m_program_debug_list.SetItemText(0,CONTROLLER_INPUTUNITS,_T(""));
				}

				m_program_debug_list.SetItemText(0,CONTROLLER_INPUT,temp_des2);

				float persend_data;
				persend_data = ((float)m_controller_data.at(point_number).value)/ 1000;
				CString temp_output_value;
				temp_output_value.Format(_T("%.1f%%"),persend_data);
				m_program_debug_list.SetItemText(0,CONTROLLER_OUTPUT,temp_output_value);

				temp_des2.Empty();
				temp_des3.Empty();
				temp_set_unit.Empty();
				if(m_controller_data.at(point_number).setpoint.point_type == (BAC_OUT + 1)) 
				{
					if(m_controller_data.at(point_number).setpoint.number< BAC_OUTPUT_ITEM_COUNT)
					{

					}
				}
				else if(m_controller_data.at(point_number).setpoint.point_type == (BAC_IN + 1)) 
				{
					if(m_controller_data.at(point_number).setpoint.number< BAC_INPUT_ITEM_COUNT)
					{
						int num_point,num_panel,num_net,k;
						Point_T3000 point;
						point.number = m_controller_data.at(point_number).setpoint.number;
						point.number = point.number + 1;	//input setpoint 是从 0 开始计数的 ，但是要去找point label 要从1开始;
						point.panel = m_controller_data.at(point_number).setpoint.panel;
						point.point_type = m_controller_data.at(point_number).setpoint.point_type - 1;	//调用 ispoint的时候要减一;
						byte point_type,var_type;

						int temp_network = 0;
						char buf[255];
						char q[17];
						pointtotext_for_controller(q, &point);

						char * temp1 = ispoint(q,&num_point,&var_type, &point_type, &num_panel, &num_net, temp_network, point.panel, &k);
						if(temp1!=NULL)
						{
							if(strlen(temp1) < 255)
							{
								strcpy(buf,temp1);

								MultiByteToWideChar( CP_ACP, 0, (char *)buf,(int)strlen((char *)buf)+1, 
									temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
								temp_des2.ReleaseBuffer();	

								float temp_float_value;
								temp_float_value = ((float)m_controller_data.at(point_number).setpoint_value) / 1000;
								temp_des3.Format(_T("%.1f"),temp_float_value);



								int x = m_controller_data.at(point_number).setpoint.number;
								if(m_Input_data.at(x).digital_analog == BAC_UNITS_ANALOG)
								{

									if(m_Input_data.at(x).range <  (sizeof(Input_List_Analog_Units)/sizeof(Input_List_Analog_Units[0])))
										temp_set_unit = Input_List_Analog_Units[m_Input_data.at(x).range];

								}
							}
						}

					}
				}
				else if(m_controller_data.at(point_number).setpoint.point_type == (BAC_VAR + 1))	//Variable
				{
					if(m_controller_data.at(point_number).setpoint.number< BAC_VARIABLE_ITEM_COUNT)
					{
						int num_point,num_panel,num_net,k;
						Point_T3000 point;
						point.number = m_controller_data.at(point_number).setpoint.number;
						point.number = point.number + 1;	//input setpoint 是从 0 开始计数的 ，但是要去找point label 要从1开始;
						point.panel = m_controller_data.at(point_number).setpoint.panel;
						point.point_type = m_controller_data.at(point_number).setpoint.point_type - 1;	//调用 ispoint的时候要减一;
						byte point_type,var_type;

						int temp_network = 0;
						char buf[255];
						char q[17];
						pointtotext_for_controller(q, &point);

						char * temp1 = ispoint(q,&num_point,&var_type, &point_type, &num_panel, &num_net, temp_network, point.panel, &k);
						if(temp1!=NULL)
						{
							if(strlen(temp1) < 255)
							{
								strcpy(buf,temp1);

								MultiByteToWideChar( CP_ACP, 0, (char *)buf,(int)strlen((char *)buf)+1, 
									temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
								temp_des2.ReleaseBuffer();	

								float temp_float_value;
								temp_float_value = ((float)m_controller_data.at(point_number).setpoint_value) / 1000;
								temp_des3.Format(_T("%.1f"),temp_float_value);



								int x = m_controller_data.at(point_number).setpoint.number;
								if(m_Variable_data.at(x).digital_analog == BAC_UNITS_DIGITAL)
								{


									if(m_Variable_data.at(x).range>30)
									{
										temp_set_unit = _T("");
									}
									else
									{
										CString temp1;
										CStringArray temparray;

										if((m_Variable_data.at(x).range < 23) &&(m_Variable_data.at(x).range !=0))
											temp1 = Digital_Units_Array[m_Variable_data.at(x).range];
										else if((m_Variable_data.at(x).range >=23) && (m_Variable_data.at(x).range <= 30))
										{
											if(receive_customer_unit)
												temp1 = Custom_Digital_Range[m_Variable_data.at(x).range - 23];
										}

										SplitCStringA(temparray,temp1,_T("/"));
										if((temparray.GetSize()==2))
										{
											if(m_Variable_data.at(x).control == 0)
												temp_set_unit = temparray.GetAt(0);
											else
												temp_set_unit = temparray.GetAt(1);
										}

									}
								}
								else
								{
									if(m_Variable_data.at(x).range == 20)	//如果是时间;
									{
										temp_set_unit = Variable_Analog_Units_Array[m_Variable_data.at(x).range];
										char temp_char[50];
										int time_seconds = m_Variable_data.at(x).value / 1000;
										intervaltotext(temp_char,time_seconds,0,0);
										CString temp_11;
										MultiByteToWideChar( CP_ACP, 0, temp_char, strlen(temp_char) + 1, 
											temp_11.GetBuffer(MAX_PATH), MAX_PATH );
										temp_11.ReleaseBuffer();		

										temp_set_unit = temp_11;
										//temp_value.Format(_T("%d"),m_Variable_data.at(i).value);
										//m_variable_list.SetItemText(i,VARIABLE_VALUE,temp_value);
									}
									else if(m_Variable_data.at(x).range<=sizeof(Variable_Analog_Units_Array)/sizeof(Variable_Analog_Units_Array[0]))
									{
										//m_variable_list.SetItemText(i,VARIABLE_UNITE,Variable_Analog_Units_Array[m_Variable_data.at(i).range]);
										temp_set_unit = Variable_Analog_Units_Array[m_Variable_data.at(x).range];
										//CString cstemp_value;
										//float temp_float_value;
										//temp_float_value = ((float)m_Variable_data.at(i).value) / 1000;
										//cstemp_value.Format(_T("%.3f"),temp_float_value);
										//m_variable_list.SetItemText(i,VARIABLE_VALUE,cstemp_value);

										//temp_value.Format(_T("%d"),m_Variable_data.at(i).value);
										//m_variable_list.SetItemText(i,VARIABLE_VALUE,temp_value);
									}
								}





							}
						}
					}
				}


				if(bacnet_device_type == STM32_HUM_NET)
				{
					temp_des3.Format(_T("%.2f"),((float)m_controller_data.at(point_number).setpoint_value)/1000);
				}

				m_program_debug_list.SetItemText(0,CONTROLLER_SETPOINT,temp_des2);
				m_program_debug_list.SetItemText(0,CONTROLLER_SETVALUE,temp_des3);
				m_program_debug_list.SetItemText(0,CONTROLLER_SETPOINTUNITS,temp_set_unit);

				if(m_controller_data.at(point_number).repeats_per_min == 0)
				{
					m_program_debug_list.SetItemText(0,CONTROLLER_I_TIME,PID_Time_Type[0]);
				}
				else
				{
					m_program_debug_list.SetItemText(0,CONTROLLER_I_TIME,PID_Time_Type[1]);
				}

				if(m_controller_data.at(point_number).action == 0)
				{
					m_program_debug_list.SetItemText(0,CONTROLLER_ACTION,_T("-"));
				}
				else if(m_controller_data.at(point_number).action >= 1)
				{
					m_program_debug_list.SetItemText(0,CONTROLLER_ACTION,_T("+"));
				}
				//else
				//{
				//	m_program_debug_list.SetItemText(i,CONTROLLER_SETPOINTUNITS,_T(""));
				//}

				temp_des3.Format(_T("%d"),m_controller_data.at(point_number).proportional);
				m_program_debug_list.SetItemText(0,CONTROLLER_PROPORTIONAL,temp_des3);

				temp_des3.Format(_T("%d"),m_controller_data.at(point_number).reset);
				m_program_debug_list.SetItemText(0,CONTROLLER_RESET,temp_des3);


				if(m_controller_data.at(point_number).auto_manual==0)
				{
					m_program_debug_list.SetItemText(0,CONTROLLER_AUTO_MANUAL,_T("Auto"));
					m_program_debug_list.SetCellEnabled(0,CONTROLLER_OUTPUT,0);
				}
				else
				{
					m_program_debug_list.SetItemText(0,CONTROLLER_AUTO_MANUAL,_T("Manual"));
					m_program_debug_list.SetCellEnabled(0,CONTROLLER_OUTPUT,1);
				}


				if(m_controller_data.at(point_number).bias<=100)
				{
					temp_des3.Format(_T("%d"),m_controller_data.at(point_number).bias);
					m_program_debug_list.SetItemText(0,CONTROLLER_BIAS,temp_des3);
				}
				else
				{
					m_controller_data.at(point_number).bias = 0;
					m_program_debug_list.SetItemText(0,CONTROLLER_BIAS,_T("0"));
				}

				if(m_controller_data.at(point_number).rate<=200)
				{
					temp_des3.Format(_T("%.2f"),((float)m_controller_data.at(point_number).rate)/100);
					m_program_debug_list.SetItemText(0,CONTROLLER_RATE,temp_des3);
				}
				else
				{
					m_controller_data.at(point_number).rate = 0;
					m_program_debug_list.SetItemText(0,CONTROLLER_RATE,_T("0"));
				}
			}
			break;
        case BAC_HOL:
        {

            MultiByteToWideChar(CP_ACP, 0, (char *)m_Annual_data.at(point_number).description, (int)strlen((char *)m_Annual_data.at(point_number).description) + 1,
                temp_des.GetBuffer(MAX_PATH), MAX_PATH);
            temp_des.ReleaseBuffer();
            m_program_debug_list.SetItemText(0, ANNUAL_ROUTINE_FULL_LABEL, temp_des);
            if (m_Annual_data.at(point_number).auto_manual == 0)
            {
                m_program_debug_list.SetItemText(0, ANNUAL_ROUTINE_AUTO_MANUAL, _T("Auto"));
                m_program_debug_list.SetCellEnabled(0, ANNUAL_ROUTINE_VALUE, 0);
            }
            else
            {
                m_program_debug_list.SetItemText(0, ANNUAL_ROUTINE_AUTO_MANUAL, _T("Manual"));
                m_program_debug_list.SetCellEnabled(0, ANNUAL_ROUTINE_VALUE, 1);
            }


            if (m_Annual_data.at(point_number).value == 0)
                m_program_debug_list.SetItemText(0, ANNUAL_ROUTINE_VALUE, _T("OFF"));
            else
                m_program_debug_list.SetItemText(0, ANNUAL_ROUTINE_VALUE, _T("ON"));



            CString temp_des2;
            MultiByteToWideChar(CP_ACP, 0, (char *)m_Annual_data.at(point_number).label, (int)strlen((char *)m_Annual_data.at(point_number).label) + 1,
                temp_des2.GetBuffer(MAX_PATH), MAX_PATH);
            temp_des2.ReleaseBuffer();
            m_program_debug_list.SetItemText(0, ANNUAL_ROUTINE_LABLE, temp_des2);

        }
            break;
        case BAC_SCH:
        {
            MultiByteToWideChar(CP_ACP, 0, (char *)m_Weekly_data.at(point_number).description, (int)strlen((char *)m_Weekly_data.at(point_number).description) + 1,
                temp_des.GetBuffer(MAX_PATH), MAX_PATH);
            temp_des.ReleaseBuffer();
            m_program_debug_list.SetItemText(0, WEEKLY_ROUTINE_FULL_LABLE, temp_des);
            if (m_Weekly_data.at(point_number).auto_manual == 0)
            {
                m_program_debug_list.SetItemText(0, WEEKLY_ROUTINE_AUTO_MANUAL, _T("Auto"));
                m_program_debug_list.SetCellEnabled(0, WEEKLY_ROUTINE_OUTPUT, 0);
            }
            else
            {
                m_program_debug_list.SetItemText(0, WEEKLY_ROUTINE_AUTO_MANUAL, _T("Manual"));
                m_program_debug_list.SetCellEnabled(0, WEEKLY_ROUTINE_OUTPUT, 1);
            }


            //uint8_t value ;  /* (1 bit; 0=off, 1=on)*/
            //uint8_t auto_manual;  /* (1 bit; 0=auto, 1=manual)*/
            //uint8_t override_1_value;  /* (1 bit; 0=off, 1=on)*/
            //uint8_t override_2_value;  /* (1 bit; 0=off, 1=on)*/
            //uint8_t off  ;
            //uint8_t unused	; /* (11 bits)*/
            if (m_Weekly_data.at(point_number).value == 0)
                m_program_debug_list.SetItemText(0, WEEKLY_ROUTINE_OUTPUT, _T("OFF"));
            else
                m_program_debug_list.SetItemText(0, WEEKLY_ROUTINE_OUTPUT, _T("ON"));

            if (m_Weekly_data.at(point_number).override_1_value == 0)
                m_program_debug_list.SetItemText(0, WEEKLY_ROUTINE_STATE1, _T("OFF"));
            else
                m_program_debug_list.SetItemText(0, WEEKLY_ROUTINE_STATE1, _T("ON"));

            if (m_Weekly_data.at(point_number).override_2_value == 0)
                m_program_debug_list.SetItemText(0, WEEKLY_ROUTINE_STATE2, _T("OFF"));
            else
                m_program_debug_list.SetItemText(0, WEEKLY_ROUTINE_STATE2, _T("ON"));

            CString temp_des2;
            MultiByteToWideChar(CP_ACP, 0, (char *)m_Weekly_data.at(point_number).label, (int)strlen((char *)m_Weekly_data.at(point_number).label) + 1,
                temp_des2.GetBuffer(MAX_PATH), MAX_PATH);
            temp_des2.ReleaseBuffer();
            m_program_debug_list.SetItemText(0, WEEKLY_ROUTINE_LABEL, temp_des2);


            if ((m_Weekly_data.at(point_number).override_1.point_type == BAC_HOL + 1) && (m_Weekly_data.at(point_number).override_1.number < BAC_HOLIDAY_COUNT))
            {
                m_program_debug_list.SetItemText(0, WEEKLY_ROUTINE_HOLIDAY1, HolLable[m_Weekly_data.at(point_number).override_1.number]);

            }
            else
            {
                m_program_debug_list.SetItemText(0, WEEKLY_ROUTINE_HOLIDAY1, _T(""));
            }

            if ((m_Weekly_data.at(point_number).override_2.point_type == BAC_HOL + 1) && (m_Weekly_data.at(point_number).override_2.number < 8))
            {
                m_program_debug_list.SetItemText(0, WEEKLY_ROUTINE_HOLIDAY2, HolLable[m_Weekly_data.at(point_number).override_2.number]);
            }
            else
            {
                m_program_debug_list.SetItemText(0, WEEKLY_ROUTINE_HOLIDAY2, _T(""));
            }

        }
            break;
		default:
			break;
		}


	return 0;
}




LRESULT CBacnetProgramDebug::Fresh_Program_Debug_Item(WPARAM wParam,LPARAM lParam)
{


	int cmp_ret ;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	//point_number
	switch(point_type)
	{
	case BAC_OUT: //OUT
		{
			CString temp_task_info;
			CString New_CString =  m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);
			CString cstemp_value;
			//先保存 原来的值，等结束的时候来比对，看是否有改变，有改变就进行写动作;
			memcpy_s(&m_temp_output_data[point_number],sizeof(Str_out_point),&m_Output_data.at(point_number),sizeof(Str_out_point));

			if(Changed_SubItem == OUTPUT_PWM_PERIOD)
			{
				CString temp_cs = m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);
				int temp_int = (int)(_wtof(temp_cs));
				if((temp_int<0) || (temp_int >=256))
				{
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Data out of range!"));
					PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,Changed_Item,REFRESH_ON_ITEM);
					return 0;
				}
				m_Output_data.at(point_number).pwm_period = (unsigned char)temp_int;
			}

			if(Changed_SubItem == OUTPUT_LABLE)
			{
				CString cs_temp = m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);
				if(cs_temp.GetLength()>= STR_OUT_LABEL)	//长度不能大于结构体定义的长度;
				{
					MessageBox(_T("Length can not higher than 8"),_T("Warning"));
					PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
					return 0;
				}
				cs_temp.Replace(_T("-"),_T("_"));
				cs_temp.MakeUpper();
				if(Check_Label_Exsit(cs_temp))
				{
					//PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,Changed_Item,REFRESH_ON_ITEM);
					return 0;
				}
				char cTemp1[255];
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_Output_data.at(point_number).label,STR_OUT_LABEL,cTemp1,STR_OUT_LABEL);
			}

			if(Changed_SubItem == OUTPUT_FULL_LABLE)
			{
				CString cs_temp = m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);
				if(cs_temp.GetLength()>= STR_OUT_DESCRIPTION_LENGTH)	//长度不能大于结构体定义的长度;
				{
					MessageBox(_T("Length can not higher than 18"),_T("Warning"));
					//PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
					return 0;
				}
				if(Check_FullLabel_Exsit(cs_temp))
				{
					//PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,Changed_Item,REFRESH_ON_ITEM);
					return 0;
				}
				char cTemp1[255];
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_Output_data.at(point_number).description,STR_OUT_DESCRIPTION_LENGTH,cTemp1,STR_OUT_DESCRIPTION_LENGTH);
			}

			if(Changed_SubItem==OUTPUT_AUTO_MANUAL)	//If auto man changed to manual , the value can be edit by user
			{
				CString temp_cs = m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);
				if(temp_cs.CompareNoCase(_T("Auto"))==0)
				{
					//m_program_debug_list.SetCellEnabled(Changed_Item,OUTPUT_VALUE,0);
					m_Output_data.at(point_number).auto_manual = BAC_AUTO ;
				}
				else
				{
					//m_program_debug_list.SetCellEnabled(Changed_Item,OUTPUT_VALUE,1);
					m_Output_data.at(point_number).auto_manual = BAC_MANUAL ;
				}
			}

			if(Changed_SubItem == OUTPUT_VALUE)
			{
				CString temp_cs = m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);
#if 0
				int temp_int = (int)(_wtof(temp_cs) * 1000);
				m_Output_data.at(Changed_Item).value = temp_int;
#endif
				//	int temp_int = _wtoi(temp_cs);
				int temp_int = (int)(_wtof(temp_cs) * 1000);
				m_Output_data.at(point_number).value = temp_int;
			}


			if(Changed_SubItem == OUTPUT_DECOM)
			{
				CString temp_cs = m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);
				if(temp_cs.CompareNoCase(_T("OK"))==0)
				{
					m_Output_data.at(point_number).decom = BAC_DECOM_YES ;
				}
				else if(temp_cs.CompareNoCase(_T("-"))==0)
				{
					m_Output_data.at(point_number).decom = BAC_DECOM_NO ;
				}
			}

			cmp_ret = memcmp(&m_temp_output_data[point_number],&m_Output_data.at(point_number),sizeof(Str_out_point));
			if(cmp_ret!=0)
			{
				//m_program_debug_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
				temp_task_info.Format(_T("Write Output List Item%d .Changed to \"%s\" "),point_number + 1,New_CString);
				Post_Write_Message(g_bac_instance,WRITEOUTPUT_T3000,point_number,point_number,sizeof(Str_out_point),m_output_dlg_hwnd,temp_task_info,Changed_Item,Changed_SubItem);
			}
		}
		break;
	case BAC_IN: //IN
		{
			memcpy_s(&m_temp_Input_data[point_number],sizeof(Str_in_point),&m_Input_data.at(point_number),sizeof(Str_in_point));

			CString temp_task_info;
			CString New_CString =  m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);
			CString cstemp_value;
			if(Changed_SubItem == INPUT_LABLE)
			{
				CString cs_temp = m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);
				if(cs_temp.GetLength()>= STR_IN_LABEL)	//长度不能大于结构体定义的长度;
				{
					MessageBox(_T("Length can not greater than 8"),_T("Warning"));
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
				char cTemp1[255];
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_Input_data.at(point_number).label,STR_IN_LABEL,cTemp1,STR_IN_LABEL);
			}

			if(Changed_SubItem == INPUT_FULL_LABLE)
			{
				CString cs_temp = m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);
				if(cs_temp.GetLength()>= STR_IN_DESCRIPTION_LENGTH)	//长度不能大于结构体定义的长度;
				{
					MessageBox(_T("Length can not higher than 20"),_T("Warning"));
					PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
					return 0;
				}

				if(Check_FullLabel_Exsit(cs_temp))
				{
					PostMessage(WM_REFRESH_BAC_INPUT_LIST,Changed_Item,REFRESH_ON_ITEM);
					return 0;
				}

				char cTemp1[255];
				memset(cTemp1,0,255);
				WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
				memcpy_s(m_Input_data.at(point_number).description,STR_IN_DESCRIPTION_LENGTH,cTemp1,STR_IN_DESCRIPTION_LENGTH);
			}


			if(Changed_SubItem == INPUT_AUTO_MANUAL)
			{
				CString temp_cs = m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);
				if(temp_cs.CompareNoCase(_T("Auto"))==0)
				{
					m_Input_data.at(point_number).auto_manual = BAC_AUTO ;
				}
				else
				{
					m_Input_data.at(point_number).auto_manual = BAC_MANUAL ;
				}
			}



			if(Changed_SubItem == INPUT_VALUE)
			{
				CString temp_cs = m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);

				int temp_int = (int)(_wtof(temp_cs) * 1000);
				//int temp_int = _wtoi(temp_cs);
				m_Input_data.at(point_number).value = temp_int;
			}




			if(Changed_SubItem==INPUT_CAL)
			{
				CString cs_temp=m_program_debug_list.GetItemText(Changed_Item,INPUT_CAL);
				float temp_value = _wtof(cs_temp);
				int cal_value = (int)(temp_value * 10);
				if((cal_value<0) || (cal_value >65535))
				{
					MessageBox(_T("Please Input an value between 0.0 - 6553.6"),_T("Warning"),MB_OK);
					PostMessage(WM_REFRESH_BAC_INPUT_LIST,Changed_Item,REFRESH_ON_ITEM);//这里调用 刷新线程重新刷新会方便一点;
					return 0;
				}

				//int cal_value = _wtoi(cs_temp);

				m_Input_data.at(point_number).calibration_l = cal_value & 0x00ff;
				m_Input_data.at(point_number).calibration_h = (cal_value & 0xff00)>>8;
			}
			else if(Changed_SubItem==INPUT_FITLER)
			{
				CString cs_temp=m_program_debug_list.GetItemText(Changed_Item,INPUT_FITLER);
				int  temp2 = _wtoi(cs_temp);
				if((temp2<0) || (temp2 >255))
				{
					MessageBox(_T("Please Input an value between 0 - 255"),_T("Warning"),MB_OK);
					PostMessage(WM_REFRESH_BAC_INPUT_LIST,Changed_Item,REFRESH_ON_ITEM);//这里调用 刷新线程重新刷新会方便一点;
					return 0;
				}
				m_Input_data.at(point_number).filter = (unsigned char)temp2;//(int8_t)(log((double)temp2)/log((double)2));
			}

			if(Changed_SubItem == INPUT_JUMPER)
			{
				CString temp_jump = m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);
				for (int z=0;z<sizeof(JumperStatus)/sizeof(JumperStatus[0]);z++)
				{
					if(temp_jump.CompareNoCase(JumperStatus[z]) == 0)
					{
						unsigned char temp_value = 0;
						if((z == 0) || (z == 1) || (z == 2) || (z == 3) || (z == 4))
							temp_value = z;
						unsigned char temp1;
						temp1 = m_Input_data.at(Changed_Item).decom ;
						temp1 = temp1 & 0x0f;
						temp1 = temp1 | (temp_value << 4);
						m_Input_data.at(point_number).decom = temp1;
					}
				}

			}

			cmp_ret = memcmp(&m_temp_Input_data[point_number],&m_Input_data.at(point_number),sizeof(Str_in_point));
			if(cmp_ret!=0)
			{
				//m_program_debug_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
				temp_task_info.Format(_T("Write Input List Item%d .Changed to \"%s\" "),point_number + 1,New_CString);
				Post_Write_Message(g_bac_instance,WRITEINPUT_T3000,point_number,point_number,sizeof(Str_in_point),m_input_dlg_hwnd ,temp_task_info,Changed_Item,Changed_SubItem);
			}
		}
		break;
	case BAC_VAR: //VAR
		{
			int cmp_ret ;//compare if match it will 0;

			int Changed_Item = (int)wParam;
			int Changed_SubItem = (int)lParam;
			CString temp_task_info;
			CString New_CString =  m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);
			CString cstemp_value;
			memcpy_s(&m_temp_variable_data[point_number],sizeof(Str_variable_point),&m_Variable_data.at(point_number),sizeof(Str_variable_point));


			if(Changed_SubItem == VARIABLE_LABLE)
			{
				CString cs_temp = m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);
				if(cs_temp.GetLength()>= STR_VARIABLE_LABEL)	//长度不能大于结构体定义的长度;
				{
					MessageBox(_T("Length can not higher than 9"),_T("Warning"));
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
				memcpy_s(m_Variable_data.at(point_number).label,STR_VARIABLE_LABEL,cTemp1,STR_VARIABLE_LABEL);
			}

			if(Changed_SubItem == VARIABLE_FULL_LABLE)
			{
				CString cs_temp = m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);
				if(cs_temp.GetLength()>= STR_VARIABLE_DESCRIPTION_LENGTH)	//长度不能大于结构体定义的长度;
				{
					MessageBox(_T("Length can not higher than 20"),_T("Warning"));
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
				memcpy_s(m_Variable_data.at(point_number).description,STR_VARIABLE_DESCRIPTION_LENGTH,cTemp1,STR_VARIABLE_DESCRIPTION_LENGTH);
			}


			if(Changed_SubItem == VARIABLE_AUTO_MANUAL)
			{
				CString temp_cs = m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);
				if(temp_cs.CompareNoCase(_T("Auto"))==0)
				{
					m_Variable_data.at(point_number).auto_manual = BAC_AUTO;
				}
				else
				{
					m_Variable_data.at(point_number).auto_manual = BAC_MANUAL;
				}
			}
			if(Changed_SubItem == VARIABLE_VALUE)//这里只用处理 ANALOG 的值就看要了， DIGITAL 的值在Click 事件中处理过了;
			{
				CString temp_cs = m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);

				int temp_int = (int)(_wtof(temp_cs) * 1000);
				m_Variable_data.at(point_number).value = temp_int;
			}

			cmp_ret = memcmp(&m_temp_variable_data[point_number],&m_Variable_data.at(point_number),sizeof(Str_variable_point));
			if(cmp_ret!=0)
			{
				temp_task_info.Format(_T("Write Variable List Item%d .Changed to \"%s\" "),point_number + 1,New_CString);
				Post_Write_Message(g_bac_instance,WRITEVARIABLE_T3000,point_number,point_number,sizeof(Str_variable_point),m_variable_dlg_hwnd ,temp_task_info,Changed_Item,Changed_SubItem);
			}
		}
		break;
    case BAC_SCH:
    {
        int cmp_ret;//compare if match it will 0;
        int Changed_Item = (int)wParam;
        int Changed_SubItem = (int)lParam;

        CString temp_task_info;
        CString New_CString = m_program_debug_list.GetItemText(Changed_Item, Changed_SubItem);
        memcpy_s(&m_temp_weekly_data[point_number], sizeof(Str_weekly_routine_point), &m_Weekly_data.at(point_number), sizeof(Str_weekly_routine_point));
        if (Changed_SubItem == WEEKLY_ROUTINE_LABEL)
        {
            CString cs_temp = m_program_debug_list.GetItemText(Changed_Item, Changed_SubItem);
            if (cs_temp.GetLength() >= STR_WEEKLY_LABEL_LENGTH)	//长度不能大于结构体定义的长度;
            {
                MessageBox(_T("Length can not higher than 8"), _T("Warning"));
                PostMessage(WM_REFRESH_BAC_WEEKLY_LIST, NULL, NULL);
                return 0;
            }
            cs_temp.MakeUpper();
            if (Check_Label_Exsit(cs_temp))
            {
                PostMessage(WM_REFRESH_BAC_WEEKLY_LIST, Changed_Item, REFRESH_ON_ITEM);
                return 0;
            }
            char cTemp1[255];
            memset(cTemp1, 0, 255);
            WideCharToMultiByte(CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL);
            memcpy_s(m_Weekly_data.at(point_number).label, STR_WEEKLY_LABEL_LENGTH, cTemp1, STR_WEEKLY_LABEL_LENGTH);
        }

        if (Changed_SubItem == WEEKLY_ROUTINE_FULL_LABLE)
        {
            CString cs_temp = m_program_debug_list.GetItemText(Changed_Item, Changed_SubItem);
            if (cs_temp.GetLength() >= STR_WEEKLY_DESCRIPTION_LENGTH)	//长度不能大于结构体定义的长度;
            {
                MessageBox(_T("Length can not higher than 20"), _T("Warning"));
                PostMessage(WM_REFRESH_BAC_WEEKLY_LIST, NULL, NULL);
                return 0;
            }
            if (Check_FullLabel_Exsit(cs_temp))
            {
                PostMessage(WM_REFRESH_BAC_WEEKLY_LIST, Changed_Item, REFRESH_ON_ITEM);
                return 0;
            }
            char cTemp1[255];
            memset(cTemp1, 0, 255);
            WideCharToMultiByte(CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL);
            memcpy_s(m_Weekly_data.at(point_number).description, STR_WEEKLY_DESCRIPTION_LENGTH, cTemp1, STR_WEEKLY_DESCRIPTION_LENGTH);
        }

        if (Changed_SubItem == WEEKLY_ROUTINE_OUTPUT)
        {
            CString cs_temp = m_program_debug_list.GetItemText(Changed_Item, WEEKLY_ROUTINE_OUTPUT);
            if (cs_temp.CompareNoCase(_T("OFF")) == 0)
            {
                m_Weekly_data.at(point_number).value = 0;
            }
            else
            {
                m_Weekly_data.at(point_number).value = 1;
            }
        }
        if (Changed_SubItem == WEEKLY_ROUTINE_AUTO_MANUAL)
        {
            CString cs_temp = m_program_debug_list.GetItemText(Changed_Item, WEEKLY_ROUTINE_AUTO_MANUAL);
            if (cs_temp.CompareNoCase(_T("Auto")) == 0)
            {
                m_Weekly_data.at(point_number).auto_manual = 0;
                m_program_debug_list.SetCellEnabled(Changed_Item, WEEKLY_ROUTINE_OUTPUT, 0);
            }
            else
            {
                m_Weekly_data.at(point_number).auto_manual = 1;
                m_program_debug_list.SetCellEnabled(Changed_Item, WEEKLY_ROUTINE_OUTPUT, 1);
            }
        }

        if (Changed_SubItem == WEEKLY_ROUTINE_HOLIDAY1)
        {
            CString cs_temp = m_program_debug_list.GetItemText(Changed_Item, WEEKLY_ROUTINE_HOLIDAY1);
            if (cs_temp.CompareNoCase(_T(" ")) == 0)
            {
                m_Weekly_data.at(point_number).override_1.panel = 0;
                m_Weekly_data.at(point_number).override_1.point_type = 0;
                m_Weekly_data.at(point_number).override_1.number = 0;
            }
            else
            {
                int sel_index = 1;
                for (int x = 0; x < BAC_HOLIDAY_COUNT; x++)
                {
                    if (cs_temp.CompareNoCase(HolLable[x]) == 0)
                        sel_index = x + 1;
                }

                cs_temp = cs_temp.Right(1);
                int temp_value = _wtoi(cs_temp);
                m_Weekly_data.at(point_number).override_1.panel = Station_NUM;
                m_Weekly_data.at(point_number).override_1.point_type = BAC_HOL + 1;
                m_Weekly_data.at(point_number).override_1.number = temp_value - 1;
            }


        }

        if (Changed_SubItem == WEEKLY_ROUTINE_HOLIDAY2)
        {
            CString cs_temp = m_program_debug_list.GetItemText(Changed_Item, WEEKLY_ROUTINE_HOLIDAY2);
            if (cs_temp.CompareNoCase(_T(" ")) == 0)
            {
                m_Weekly_data.at(point_number).override_2.panel = 0;
                m_Weekly_data.at(point_number).override_2.point_type = 0;
                m_Weekly_data.at(point_number).override_2.number = 0;
            }
            else
            {
                int sel_index = 1;
                for (int x = 0; x < BAC_HOLIDAY_COUNT; x++)
                {
                    if (cs_temp.CompareNoCase(HolLable[x]) == 0)
                        sel_index = x + 1;
                }

                cs_temp = cs_temp.Right(1);
                int temp_value = _wtoi(cs_temp);
                m_Weekly_data.at(point_number).override_2.panel = Station_NUM;
                m_Weekly_data.at(point_number).override_2.point_type = BAC_HOL + 1;
                m_Weekly_data.at(point_number).override_2.number = temp_value - 1;
            }

        }

        cmp_ret = memcmp(&m_temp_weekly_data[point_number], &m_Weekly_data.at(point_number), sizeof(Str_weekly_routine_point));
        if (cmp_ret != 0)
        {
            m_program_debug_list.SetItemBkColor(Changed_Item, Changed_SubItem, LIST_ITEM_CHANGED_BKCOLOR);
            temp_task_info.Format(_T("Write Weekly Routine List Item%d .Changed to \"%s\" "), Changed_Item + 1, New_CString);
            Post_Write_Message(g_bac_instance, WRITESCHEDULE_T3000, point_number, point_number, sizeof(Str_weekly_routine_point), m_weekly_dlg_hwnd, temp_task_info, Changed_Item, Changed_SubItem);
        }
    }
        break;
    case BAC_HOL:
    {
        int cmp_ret;//compare if match it will 0;
        int Changed_Item = (int)wParam;
        int Changed_SubItem = (int)lParam;

        CString temp_task_info;
        CString New_CString = m_program_debug_list.GetItemText(Changed_Item, Changed_SubItem);

        //先保存 原来的值，等结束的时候来比对，看是否有改变，有改变就进行写动作;
        memcpy_s(&m_temp_annual_data[point_number], sizeof(Str_annual_routine_point), &m_Annual_data.at(point_number), sizeof(Str_annual_routine_point));

        if (Changed_SubItem == ANNUAL_ROUTINE_LABLE)
        {
            CString cs_temp = m_program_debug_list.GetItemText(Changed_Item, Changed_SubItem);
            if (cs_temp.GetLength() >= STR_ANNUAL_LABEL_LENGTH)	//长度不能大于结构体定义的长度;
            {
                MessageBox(_T("Length can not higher than 8"), _T("Warning"));
                PostMessage(WM_REFRESH_BAC_ANNUAL_LIST, NULL, NULL);
                return 0;
            }
            if (Check_Label_Exsit(cs_temp))
            {
                PostMessage(WM_REFRESH_BAC_ANNUAL_LIST, Changed_Item, REFRESH_ON_ITEM);
                return 0;
            }

            char cTemp1[255];
            memset(cTemp1, 0, 255);
            WideCharToMultiByte(CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL);
            memcpy_s(m_Annual_data.at(point_number).label, STR_ANNUAL_LABEL_LENGTH, cTemp1, STR_ANNUAL_LABEL_LENGTH);
        }

        if (Changed_SubItem == ANNUAL_ROUTINE_FULL_LABEL)
        {
            CString cs_temp = m_program_debug_list.GetItemText(Changed_Item, Changed_SubItem);
            if (cs_temp.GetLength() >= STR_ANNUAL_DESCRIPTION_LENGTH)	//长度不能大于结构体定义的长度;
            {
                MessageBox(_T("Length can not higher than 20"), _T("Warning"));
                PostMessage(WM_REFRESH_BAC_ANNUAL_LIST, NULL, NULL);
                return 0;
            }
            if (Check_FullLabel_Exsit(cs_temp))
            {
                PostMessage(WM_REFRESH_BAC_ANNUAL_LIST, Changed_Item, REFRESH_ON_ITEM);
                return 0;
            }
            char cTemp1[255];
            memset(cTemp1, 0, 255);
            WideCharToMultiByte(CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL);
            memcpy_s(m_Annual_data.at(point_number).description, STR_ANNUAL_DESCRIPTION_LENGTH, cTemp1, STR_ANNUAL_DESCRIPTION_LENGTH);
        }

        if (Changed_SubItem == ANNUAL_ROUTINE_VALUE)
        {
            CString cs_temp = m_program_debug_list.GetItemText(Changed_Item, ANNUAL_ROUTINE_VALUE);
            if (cs_temp.CompareNoCase(_T("OFF")) == 0)
            {
                m_Annual_data.at(point_number).value = 0;
            }
            else
            {
                m_Annual_data.at(point_number).value = 1;
            }
        }
        if (Changed_SubItem == ANNUAL_ROUTINE_AUTO_MANUAL)
        {
            CString cs_temp = m_program_debug_list.GetItemText(Changed_Item, ANNUAL_ROUTINE_AUTO_MANUAL);
            if (cs_temp.CompareNoCase(_T("Auto")) == 0)
            {
                m_Annual_data.at(point_number).auto_manual = 0;
                m_program_debug_list.SetCellEnabled(Changed_Item, ANNUAL_ROUTINE_VALUE, 0);
            }
            else
            {
                m_Annual_data.at(point_number).auto_manual = 1;
                m_program_debug_list.SetCellEnabled(Changed_Item, ANNUAL_ROUTINE_VALUE, 1);
            }
        }
        cmp_ret = memcmp(&m_temp_annual_data[point_number], &m_Annual_data.at(point_number), sizeof(Str_annual_routine_point));
        if (cmp_ret != 0)
        {
            m_program_debug_list.SetItemBkColor(Changed_Item, Changed_SubItem, LIST_ITEM_CHANGED_BKCOLOR);
            temp_task_info.Format(_T("Write Annual Routine List Item%d .Changed to \"%s\" "), Changed_Item + 1, New_CString);
            Post_Write_Message(g_bac_instance, WRITEHOLIDAY_T3000, point_number, point_number, sizeof(Str_annual_routine_point), m_annual_dlg_hwnd, temp_task_info, Changed_Item, Changed_SubItem);
        }
    }
        break;
	default:
		{

		}
		break;
	}


	

	return 0;
}







void CBacnetProgramDebug::OnNMClickListProgramDebug(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	*pResult = 0;


	long lRow,lCol;
	m_program_debug_list.Set_Edit(true);
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_program_debug_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_program_debug_list.SubItemHitTest(&lvinfo);

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;


	if(lRow>m_program_debug_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow<0)
		return;



	CString temp1;
	//m_row = lRow;
	//m_col = lCol;


	switch(point_type)
	{
	case BAC_OUT: //OUT
		{
			CString New_CString;
			CString temp_task_info;
			CString temp1;
			CStringArray temparray;
			if(lCol == OUTPUT_VALUE)
			{
				if(m_Output_data.at(point_number).auto_manual == BAC_AUTO)	//If it is auto mode, disable to change the value.
				{
					m_program_debug_list.Set_Edit(false);
					return;
				}

				if(m_Output_data.at(point_number).digital_analog != BAC_UNITS_DIGITAL)
					return;



				memcpy_s(&m_temp_output_data[point_number],sizeof(Str_out_point),&m_Output_data.at(point_number),sizeof(Str_out_point));





				if((m_Output_data.at(point_number).range < 23) &&(m_Output_data.at(point_number).range !=0))
					temp1 = Digital_Units_Array[m_Output_data.at(point_number).range];
				else if((m_Output_data.at(point_number).range >=23) && (m_Output_data.at(point_number).range <= 30))
				{
					if(receive_customer_unit)
						temp1 = Custom_Digital_Range[m_Output_data.at(point_number).range - 23];
					else
					{
						m_program_debug_list.Set_Edit(false);
						return;
					}
				}
				else
					return;

				SplitCStringA(temparray,temp1,_T("/"));
				if(m_Output_data.at(point_number).control == 0)
				{
					m_Output_data.at(point_number).control = 1;
					m_program_debug_list.SetItemText(0,OUTPUT_VALUE,temparray.GetAt(1));
					New_CString = temparray.GetAt(1);
				}
				else
				{
					m_Output_data.at(point_number).control = 0;
					m_program_debug_list.SetItemText(0,OUTPUT_VALUE,temparray.GetAt(0));
					New_CString = temparray.GetAt(0);
				}
			}
			else if(lCol == OUTPUT_AUTO_MANUAL)
			{
				memcpy_s(&m_temp_output_data[point_number],sizeof(Str_out_point),&m_Output_data.at(point_number),sizeof(Str_out_point));
				if(m_Output_data.at(point_number).auto_manual == 0)
				{
					m_Output_data.at(point_number).auto_manual = 1;
					m_program_debug_list.SetItemText(0,OUTPUT_AUTO_MANUAL,_T("Manual"));
					//m_program_debug_list.SetCellEnabled(lRow,OUTPUT_VALUE,TRUE);
					New_CString = _T("Manual");
				}
				else
				{
					m_Output_data.at(point_number).auto_manual = 0;
					m_program_debug_list.SetItemText(0,OUTPUT_AUTO_MANUAL,_T("Auto"));
					//m_program_debug_list.SetCellEnabled(lRow,OUTPUT_VALUE,FALSE);
					New_CString = _T("Auto");
				}
			}
			else if(lCol == OUTPUT_RANGE)
			{



				//CString temp_cs = m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);
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

				if(m_Output_data.at(point_number).digital_analog == BAC_UNITS_ANALOG)
				{
					bac_ranges_type = OUTPUT_RANGE_ANALOG_TYPE;
					if(m_Output_data.at(point_number).range > (sizeof(Output_Analog_Units_Array) / sizeof(Output_Analog_Units_Array[0])))
					{
						m_Output_data.at(point_number).range = 0;
						bac_range_number_choose = 0;
					}
				}
				else
				{
					bac_ranges_type = OUTPUT_RANGE_DIGITAL_TYPE;
					if(m_Output_data.at(point_number).range > 30)
					{
						m_Output_data.at(point_number).range = 0;
						bac_range_number_choose = 0;
					}
				}


				//if(temp_cs.CompareNoCase(Units_Type[UNITS_TYPE_ANALOG])==0)
				//{
				initial_dialog = 3;
				bac_range_number_choose = m_Output_data.at(point_number).range;
				//bac_ranges_type = OUTPUT_RANGE_ANALOG_TYPE;
				dlg.DoModal();
				if(range_cancel)
				{
					PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,lRow,REFRESH_ON_ITEM);//这里调用 刷新线程重新刷新会方便一点;
					return ;
				}
				if(bac_range_number_choose == 0)	//如果选择的是 unused 就认为是analog 的unused;这样 能显示对应的value;
				{
					m_Output_data.at(point_number).digital_analog =  BAC_UNITS_ANALOG;
					bac_ranges_type = OUTPUT_RANGE_ANALOG_TYPE;
				}

				if(bac_ranges_type == OUTPUT_RANGE_ANALOG_TYPE)
				{
					m_Output_data.at(point_number).digital_analog =  BAC_UNITS_ANALOG;
					m_Output_data.at(point_number).range =  bac_range_number_choose;
					m_program_debug_list.SetItemText(0,OUTPUT_UNITE,Output_Analog_Units_Show[bac_range_number_choose]);		
					m_program_debug_list.SetItemText(0,OUTPUT_RANGE,OutPut_List_Analog_Range[bac_range_number_choose]);	


					CString cstemp_value;
					cstemp_value.Format(_T("%d"),m_Output_data.at(point_number).value);
					m_program_debug_list.SetItemText(0,OUTPUT_VALUE,cstemp_value);	
				}
				else if((bac_ranges_type == VARIABLE_RANGE_DIGITAL_TYPE) || (bac_ranges_type == INPUT_RANGE_DIGITAL_TYPE) || (bac_ranges_type == OUTPUT_RANGE_DIGITAL_TYPE))
				{
					m_Output_data.at(point_number).digital_analog =  BAC_UNITS_DIGITAL;
					m_Output_data.at(point_number).range =  bac_range_number_choose;


					CStringArray temparray;

					if((bac_range_number_choose >= 23) && (bac_range_number_choose <= 30))
					{
						//temp1.Format(_T("%s"), Custom_Digital_Range[bac_range_number_choose - 23]);
						temp1 = Custom_Digital_Range[bac_range_number_choose - 23];
					}
					else
						temp1 = Digital_Units_Array[bac_range_number_choose];//22 is the sizeof the array



					SplitCStringA(temparray,temp1,_T("/"));

					if(m_Output_data.at(point_number).control == 1)
					{
						if((temparray.GetSize()==2)&&(!temparray.GetAt(1).IsEmpty()))
						{
							m_program_debug_list.SetItemText(0,OUTPUT_VALUE,temparray.GetAt(1));
						}
					}
					else
					{
						if((temparray.GetSize()==2)&&(!temparray.GetAt(0).IsEmpty()))
						{
							m_program_debug_list.SetItemText(0,OUTPUT_VALUE,temparray.GetAt(0));
						}	
					}
					m_program_debug_list.SetItemText(0,OUTPUT_RANGE,temp1);
					m_program_debug_list.SetItemText(0,OUTPUT_UNITE,_T(""));//如果是数字单位 Unit 要清空;
				}

			}
			else
			{
				return;
			}

			//return;



			m_program_debug_list.Set_Edit(false);

			int cmp_ret = memcmp(&m_temp_output_data[point_number],&m_Output_data.at(point_number),sizeof(Str_out_point));
			if(cmp_ret!=0)
			{
				//m_program_debug_list.SetItemBkColor(lRow,lCol,LIST_ITEM_CHANGED_BKCOLOR);
				temp_task_info.Format(_T("Write Output List Item%d .Changed to \"%s\" "),point_number + 1,New_CString);
				Post_Write_Message(g_bac_instance,WRITEOUTPUT_T3000,point_number,point_number,sizeof(Str_out_point),m_output_dlg_hwnd,temp_task_info,lRow,lCol);
			}
		}
		break;
	case BAC_IN: //IN
		{
			CString New_CString;
			CString temp_task_info;

			CString temp1;
			CStringArray temparray;
			if(lCol == INPUT_VALUE)
			{
				if(m_Input_data.at(point_number).auto_manual == BAC_AUTO)	//If it is auto mode, disable to change the value.
				{
					m_program_debug_list.Set_Edit(false);
					return;
				}

				if(m_Input_data.at(point_number).digital_analog != BAC_UNITS_DIGITAL)
				{
					return;
				}



				memcpy_s(&m_temp_Input_data[point_number],sizeof(Str_in_point),&m_Input_data.at(point_number),sizeof(Str_in_point));


				if((m_Input_data.at(point_number).range < 23) &&(m_Input_data.at(point_number).range !=0))
					temp1 = Digital_Units_Array[m_Input_data.at(point_number).range];
				else if((m_Input_data.at(point_number).range >=23) && (m_Input_data.at(point_number).range <= 30))
				{
					if(receive_customer_unit)
						temp1 = Custom_Digital_Range[m_Input_data.at(point_number).range - 23];
					else
					{
						m_program_debug_list.Set_Edit(false);
						return;
					}
				}
				else
					return;
				SplitCStringA(temparray,temp1,_T("/"));

				if(m_Input_data.at(point_number).control == 0)
				{
					m_Input_data.at(point_number).control = 1;
					m_program_debug_list.SetItemText(0,INPUT_VALUE,temparray.GetAt(1));
					New_CString = temparray.GetAt(1);
				}
				else
				{
					m_Input_data.at(point_number).control = 0;
					m_program_debug_list.SetItemText(0,INPUT_VALUE,temparray.GetAt(0));
					New_CString = temparray.GetAt(0);
				}

			}
			else if(lCol == INPUT_CAL_OPERATION)
			{
				CString n_option1;
				CString n_option2;
				CString n_value;


				if((m_Input_data.at(point_number).calibration_l != 0) && (m_Input_data.at(point_number).calibration_h != 0))
				{
					float temp_float_value;
					unsigned short temp_cal_value;
					temp_cal_value = (m_Input_data.at(point_number).calibration_h << 8 ) + m_Input_data.at(point_number).calibration_l;
					temp_float_value = ((float)temp_cal_value) / 10;
					n_value.Format(_T("%.1f"),temp_float_value);
				}

				if(m_Input_data.at(point_number).calibration_sign == 0)
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
				memcpy_s(&m_temp_Input_data[point_number],sizeof(Str_in_point),&m_Input_data.at(point_number),sizeof(Str_in_point));
				if(m_Input_data.at(point_number).calibration_sign == 0)
				{
					m_Input_data.at(point_number).calibration_sign = 1;
					m_program_debug_list.SetItemText(lRow,INPUT_CAL_OPERATION,_T("-"));
					New_CString = _T("-");
				}
				else
				{
					m_Input_data.at(point_number).calibration_sign = 0;
					m_program_debug_list.SetItemText(lRow,INPUT_CAL_OPERATION,_T("+"));
					New_CString = _T("+");
				}
			}
			else if(lCol == INPUT_AUTO_MANUAL)
			{
				memcpy_s(&m_temp_Input_data[point_number],sizeof(Str_in_point),&m_Input_data.at(point_number),sizeof(Str_in_point));
				if(m_Input_data.at(point_number).auto_manual == 0)
				{
					m_Input_data.at(point_number).auto_manual = 1;
					m_program_debug_list.SetItemText(0,INPUT_AUTO_MANUAL,_T("Manual"));
					//m_program_debug_list.SetCellEnabled(lRow,INPUT_VALUE,TRUE);
					New_CString = _T("Manual");
				}
				else
				{
					m_Input_data.at(point_number).auto_manual = 0;
					m_program_debug_list.SetItemText(0,INPUT_AUTO_MANUAL,_T("Auto"));
					//m_program_debug_list.SetCellEnabled(lRow,INPUT_VALUE,FALSE);
					New_CString = _T("Auto");
				}
			}
			else if(lCol == INPUT_RANGE)
			{

				//CString temp_cs = m_program_debug_list.GetItemText(Changed_Item,Changed_SubItem);
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
						} while (temp_invoke_id<0);
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
				bac_range_number_choose = m_Input_data.at(point_number).range;

				if(m_Input_data.at(point_number).digital_analog == BAC_UNITS_ANALOG)
				{
					bac_ranges_type = INPUT_RANGE_ANALOG_TYPE;
					if(m_Input_data.at(point_number).range > (sizeof(Input_Analog_Units_Array) / sizeof(Input_Analog_Units_Array[0])))
					{
						m_Input_data.at(point_number).range = 0;
						bac_range_number_choose = 0;
					}
				}
				else
				{
					bac_ranges_type = INPUT_RANGE_DIGITAL_TYPE;
					if(m_Input_data.at(point_number).range > 30)
					{
						m_Input_data.at(point_number).range = 0;
						bac_range_number_choose = 0;
					}
				}


				//if(temp_cs.CompareNoCase(Units_Type[UNITS_TYPE_ANALOG])==0)
				//{
				initial_dialog = 2;
				dlg.DoModal();
				if(range_cancel)
				{
					PostMessage(WM_REFRESH_BAC_INPUT_LIST,lRow,REFRESH_ON_ITEM);//这里调用 刷新线程重新刷新会方便一点;
					return ;
				}
				if(bac_range_number_choose == 0)	//如果选择的是 unused 就认为是analog 的unused;这样 能显示对应的value;
				{
					m_Input_data.at(point_number).digital_analog =  BAC_UNITS_ANALOG;
					bac_ranges_type = INPUT_RANGE_ANALOG_TYPE;
				}
				if(bac_ranges_type == INPUT_RANGE_ANALOG_TYPE)
				{
					m_Input_data.at(point_number).digital_analog =  BAC_UNITS_ANALOG;
					m_Input_data.at(point_number).range =  bac_range_number_choose;
					m_program_debug_list.SetItemText(0,INPUT_RANGE,Input_Analog_Units_Array[bac_range_number_choose]);		
					m_program_debug_list.SetItemText(0,INPUT_UNITE,Input_List_Analog_Units[bac_range_number_choose]);	
					New_CString = Input_Analog_Units_Array[bac_range_number_choose];
					unsigned short temp_cal_value;
					temp_cal_value = (m_Input_data.at(point_number).calibration_h << 8 ) + m_Input_data.at(point_number).calibration_l;


					CString cstemp_value;
					cstemp_value.Format(_T("%.1f"),((float)temp_cal_value)/10);
					m_program_debug_list.SetItemText(0,INPUT_CAL,cstemp_value);


					m_program_debug_list.SetCellEnabled(0,INPUT_CAL,1);
					m_program_debug_list.SetCellEnabled(0,INPUT_UNITE,1);
					float temp_float_value;
					temp_float_value = ((float)m_Input_data.at(point_number).value) / 1000;
					cstemp_value.Format(_T("%.2f"),temp_float_value);
					m_program_debug_list.SetItemText(0,INPUT_VALUE,cstemp_value);	
				}
				else if((bac_ranges_type == VARIABLE_RANGE_DIGITAL_TYPE) || (bac_ranges_type == INPUT_RANGE_DIGITAL_TYPE) || (bac_ranges_type == OUTPUT_RANGE_DIGITAL_TYPE))
				{
					m_Input_data.at(point_number).digital_analog =  BAC_UNITS_DIGITAL;
					m_Input_data.at(point_number).range =  bac_range_number_choose;

					m_program_debug_list.SetItemText(0,INPUT_CAL,_T(""));
					m_program_debug_list.SetCellEnabled(0,INPUT_CAL,0);
					m_program_debug_list.SetItemText(0,INPUT_UNITE,_T(""));
					m_program_debug_list.SetCellEnabled(0,INPUT_UNITE,0);



					CStringArray temparray;
					if((bac_range_number_choose >= 23) && (bac_range_number_choose <= 30))
					{
						temp1 = Custom_Digital_Range[bac_range_number_choose - 23];
					}
					else
						temp1 = Digital_Units_Array[bac_range_number_choose];//22 is the sizeof the array
					SplitCStringA(temparray,temp1,_T("/"));
					New_CString = temp1;
					if(m_Input_data.at(point_number).control == 1)
					{
						if((temparray.GetSize()==2)&&(!temparray.GetAt(1).IsEmpty()))
						{
							m_program_debug_list.SetItemText(0,INPUT_VALUE,temparray.GetAt(1));
						}
					}
					else
					{
						if((temparray.GetSize()==2)&&(!temparray.GetAt(0).IsEmpty()))
						{
							m_program_debug_list.SetItemText(0,INPUT_VALUE,temparray.GetAt(0));
						}			
					}
					m_program_debug_list.SetItemText(0,INPUT_RANGE,temp1);
				}
			}
			else
				return;





			m_program_debug_list.Set_Edit(false);

			int cmp_ret = memcmp(&m_temp_Input_data[point_number],&m_Input_data.at(point_number),sizeof(Str_in_point));
			if(cmp_ret!=0)
			{
				temp_task_info.Format(_T("Write Input List Item%d .Changed to \"%s\" "),point_number + 1,New_CString);
				Post_Write_Message(g_bac_instance,WRITEINPUT_T3000,(int8_t)point_number,(int8_t)point_number,sizeof(Str_in_point),m_input_dlg_hwnd,temp_task_info,lRow,lCol);
			}

		}
		break;
	case BAC_VAR: //VAR
		{
			//*************************************************************************************************************************************
			memcpy_s(&m_temp_variable_data[point_number],sizeof(Str_variable_point),&m_Variable_data.at(point_number),sizeof(Str_variable_point));
			CString New_CString;
			CString temp_task_info;
			if((lCol == VARIABLE_VALUE) &&(m_Variable_data.at(point_number).digital_analog == BAC_UNITS_DIGITAL ) && (m_Variable_data.at(point_number).auto_manual != BAC_AUTO))
			{


				CString temp1;
				CStringArray temparray;

				if((m_Variable_data.at(point_number).range < 23) &&(m_Variable_data.at(point_number).range !=0))
					temp1 = Digital_Units_Array[m_Variable_data.at(point_number).range];
				else if((m_Variable_data.at(point_number).range >=23) && (m_Variable_data.at(point_number).range <= 30))
				{
					if(receive_customer_unit)
						temp1 = Custom_Digital_Range[m_Variable_data.at(point_number).range - 23];
					else
					{
						m_program_debug_list.Set_Edit(false);
						return;
					}
				}
				else
					return;
				SplitCStringA(temparray,temp1,_T("/"));



				if(m_Variable_data.at(point_number).control == 0)
				{
					m_Variable_data.at(point_number).control = 1;
					m_program_debug_list.SetItemText(0,VARIABLE_VALUE,temparray.GetAt(1));
					New_CString = temparray.GetAt(1);
				}
				else
				{
					m_Variable_data.at(point_number).control = 0;
					m_program_debug_list.SetItemText(0,VARIABLE_VALUE,temparray.GetAt(0));
					New_CString = temparray.GetAt(0);
				}
				m_program_debug_list.Set_Edit(false);

				temp_task_info.Format(_T("Write Variable List Item%d .Changed to \"%s\" "),point_number + 1,New_CString);
			}
			else if((lCol == VARIABLE_VALUE) &&(m_Variable_data.at(point_number).digital_analog == BAC_UNITS_ANALOG ) && (m_Variable_data.at(point_number).auto_manual == BAC_MANUAL) && (m_Variable_data.at(point_number).range == 20))
			{
				m_program_debug_list.Set_Edit(false);
				m_prg_debug_variable_time_picker.ShowWindow(SW_SHOW);
				CRect list_rect,win_rect;
				m_program_debug_list.GetWindowRect(list_rect);
				GetWindowRect(win_rect);
				CRect myrect;
				m_program_debug_list.GetSubItemRect(0,lCol,LVIR_BOUNDS,myrect);


				myrect.left = myrect.left + list_rect.left - win_rect.left ;
				myrect.right = myrect.right + list_rect.left - win_rect.left;
				//myrect.top = myrect.top + 11;
				//myrect.bottom = myrect.bottom + 13;
				myrect.top = myrect.top + 18;
				myrect.bottom = myrect.bottom + 20;
				m_prg_debug_variable_time_picker.BringWindowToTop();
				m_prg_debug_variable_time_picker.MoveWindow(myrect);


				CString Temp_CString =  m_program_debug_list.GetItemText(0,lCol);

				CStringArray TEMPCS;
				int temp_hour,temp_minute;
				SplitCStringA(TEMPCS, Temp_CString, _T(":"));
				if((int)TEMPCS.GetSize() <2)
				{
					temp_hour = 0;
					temp_minute = 0;
					CTime TimeTemp(2015,1,1,temp_hour,temp_minute,0);
					m_prg_debug_variable_time_picker.SetFormat(_T("HH:mm"));
					m_prg_debug_variable_time_picker.SetTime(&TimeTemp);
					m_prg_debug_variable_time_picker.SetFocus();
				}
				else
				{
					temp_hour = _wtoi(TEMPCS.GetAt(0));
					temp_minute = _wtoi(TEMPCS.GetAt(1));
					CTime TimeTemp(2015,1,1,temp_hour,temp_minute,0);
					m_prg_debug_variable_time_picker.SetFormat(_T("HH:mm"));
					m_prg_debug_variable_time_picker.SetTime(&TimeTemp);
					m_prg_debug_variable_time_picker.SetFocus();
				}
				m_program_debug_list.SetItemText(0,lCol,_T(""));
				m_prg_debug_variable_time_picker.Invalidate();
				SetTimer(2,100,NULL);
			}
			else if(lCol == VARIABLE_VALUE)
			{
				if(m_Variable_data.at(point_number).auto_manual == BAC_AUTO)
				{
					m_program_debug_list.Set_Edit(false);
					return;
				}
			}
			else if(lCol == VARIABLE_AUTO_MANUAL)
			{
				if(m_Variable_data.at(point_number).auto_manual == 0)
				{
					m_Variable_data.at(point_number).auto_manual = 1;
					m_program_debug_list.SetItemText(lRow,VARIABLE_AUTO_MANUAL,_T("Manual"));
					//m_program_debug_list.SetCellEnabled(lRow,VARIABLE_VALUE,TRUE);
					New_CString = _T("Manual");
				}
				else
				{
					m_Variable_data.at(point_number).auto_manual = 0;
					m_program_debug_list.SetItemText(lRow,VARIABLE_AUTO_MANUAL,_T("Auto"));
					//m_program_debug_list.SetCellEnabled(lRow,VARIABLE_VALUE,FALSE);
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

				if(m_Variable_data.at(point_number).digital_analog == BAC_UNITS_ANALOG)
				{
					bac_ranges_type = VARIABLE_RANGE_ANALOG_TYPE;
					if(m_Variable_data.at(point_number).range > (sizeof(Variable_Analog_Units_Array) / sizeof(Variable_Analog_Units_Array[0])))
					{
						m_Variable_data.at(point_number).range = 0;
						bac_range_number_choose = 0;
					}
				}
				else
				{
					bac_ranges_type = VARIABLE_RANGE_DIGITAL_TYPE;
					if(m_Variable_data.at(point_number).range > 30)
					{
						m_Variable_data.at(point_number).range = 0;
						bac_range_number_choose = 0;
					}
				}


				initial_dialog = 1;
				bac_range_number_choose = m_Variable_data.at(point_number).range;
				//bac_ranges_type = VARIABLE_RANGE_ANALOG_TYPE;
				dlg.DoModal();
				if(range_cancel)
				{
					Fresh_Program_List(2);
					//PostMessage(WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);//这里调用 刷新线程重新刷新会方便一点;
					return ;
				}
				if(bac_range_number_choose == 0)	//如果选择的是 unused 就认为是analog 的unused;这样 能显示对应的value;
				{
					m_Variable_data.at(point_number).digital_analog =  BAC_UNITS_ANALOG;
					bac_ranges_type = VARIABLE_RANGE_ANALOG_TYPE;
				}

				if(bac_ranges_type == VARIABLE_RANGE_ANALOG_TYPE)
				{
					m_Variable_data.at(point_number).digital_analog = BAC_UNITS_ANALOG;
					m_Variable_data.at(point_number).range = bac_range_number_choose;
					m_program_debug_list.SetItemText(0,lCol,Variable_Analog_Units_Array[bac_range_number_choose]);



					if(m_Variable_data.at(point_number).range == 20)	//如果是时间;
					{
						char temp_char[50];
						int time_seconds = m_Variable_data.at(point_number).value / 1000;
						intervaltotextfull(temp_char,time_seconds,0,0);

						CString temp_11;
						MultiByteToWideChar( CP_ACP, 0, temp_char, strlen(temp_char) + 1, 
							temp_11.GetBuffer(MAX_PATH), MAX_PATH );
						temp_11.ReleaseBuffer();		
						m_program_debug_list.SetItemText(0,VARIABLE_VALUE,temp_11);

					}
					else
					{
						CString cstemp_value;
						float temp_float_value;
						temp_float_value = ((float)m_Variable_data.at(point_number).value) / 1000;
						cstemp_value.Format(_T("%.3f"),temp_float_value);
						m_program_debug_list.SetItemText(0,VARIABLE_VALUE,cstemp_value);
					}
				}
				else if((bac_ranges_type == VARIABLE_RANGE_DIGITAL_TYPE) || (bac_ranges_type == INPUT_RANGE_DIGITAL_TYPE) || (bac_ranges_type == OUTPUT_RANGE_DIGITAL_TYPE))
				{
					m_Variable_data.at(point_number).digital_analog = BAC_UNITS_DIGITAL;
					m_Variable_data.at(point_number).range = bac_range_number_choose;


					CStringArray temparray;
					if((bac_range_number_choose >= 23) && (bac_range_number_choose <= 30))
					{
						temp1 = Custom_Digital_Range[bac_range_number_choose - 23];
					}
					else
						temp1 = Digital_Units_Array[bac_range_number_choose];//22 is the sizeof the array

					SplitCStringA(temparray,temp1,_T("/"));





					if(m_Variable_data.at(point_number).control == 1)
					{
						if((temparray.GetSize()==2)&&(!temparray.GetAt(1).IsEmpty()))
						{
							m_program_debug_list.SetItemText(0,VARIABLE_VALUE,temparray.GetAt(1));
						}
					}
					else
					{
						if((temparray.GetSize()==2)&&(!temparray.GetAt(0).IsEmpty()))
						{
							m_program_debug_list.SetItemText(0,VARIABLE_VALUE,temparray.GetAt(0));
						}			
					}
					m_program_debug_list.SetItemText(0,VARIABLE_UNITE,temp1);
				}


			}
			else
			{
				//m_program_debug_list.Set_Edit(false);
				return;
			}

			int cmp_ret = memcmp(&m_temp_variable_data[point_number],&m_Variable_data.at(point_number),sizeof(Str_variable_point));
			if(cmp_ret!=0)
			{
				//m_program_debug_list.SetItemBkColor(lRow,lCol,LIST_ITEM_CHANGED_BKCOLOR);
				Post_Write_Message(g_bac_instance,WRITEVARIABLE_T3000,point_number,point_number,sizeof(Str_variable_point),m_variable_dlg_hwnd,temp_task_info,0,lCol);
			}
			//*************************************************************************************************************************************
		}
		break;
    case BAC_SCH:
    {

    }
        break;
    case BAC_HOL:
    {
        long lRow, lCol;
        lRow = lvinfo.iItem;
        lCol = lvinfo.iSubItem;
        CString New_CString;
        CString temp_task_info;

        if (lCol == ANNUAL_ROUTINE_AUTO_MANUAL)
        {
            //先保存 原来的值，等结束的时候来比对，看是否有改变，有改变就进行写动作;
            memcpy_s(&m_temp_annual_data[point_number], sizeof(Str_annual_routine_point), &m_Annual_data.at(point_number), sizeof(Str_annual_routine_point));

            if (m_Annual_data.at(point_number).auto_manual == 0)
            {
                m_Annual_data.at(point_number).auto_manual = 1;
                m_program_debug_list.SetItemText(0, ANNUAL_ROUTINE_AUTO_MANUAL, _T("Manual"));
                m_program_debug_list.SetCellEnabled(0, ANNUAL_ROUTINE_VALUE, TRUE);
                New_CString = _T("Manual");
            }
            else
            {
                m_Annual_data.at(point_number).auto_manual = 0;
                m_program_debug_list.SetItemText(0, ANNUAL_ROUTINE_AUTO_MANUAL, _T("Auto"));
                m_program_debug_list.SetCellEnabled(0, ANNUAL_ROUTINE_VALUE, FALSE);
                New_CString = _T("Auto");
            }
            temp_task_info.Format(_T("Write Annual List Item%d .Changed to \"%s\" "), lRow + 1, New_CString);
        }
        else
            return;

        int cmp_ret = memcmp(&m_temp_annual_data[point_number], &m_Annual_data.at(point_number), sizeof(Str_annual_routine_point));
        if (cmp_ret != 0)
        {
            m_program_debug_list.SetItemBkColor(0, lCol, LIST_ITEM_CHANGED_BKCOLOR);
            temp_task_info.Format(_T("Write Annual Routine List Item%d .Changed to \"%s\" "), point_number + 1, New_CString);
            Post_Write_Message(g_bac_instance, WRITEHOLIDAY_T3000, point_number, point_number, sizeof(Str_annual_routine_point), m_annual_dlg_hwnd, temp_task_info, 0, lCol);
        }
    }
        break;
	default:
		break;
	}
}


void CBacnetProgramDebug::OnTimer(UINT_PTR nIDEvent)
{
	 
	switch(nIDEvent)
	{
	case 2:
		{
			KillTimer(2);
			m_prg_debug_variable_time_picker.Invalidate();
		}
		break;
	case UPDATE_DEBUG_DATA_TIMER:
		{
			if((bac_select_device_online)&& (g_protocol == PROTOCOL_BACNET_IP))
			{
				switch((point_type))
				{
				case BAC_OUT:
					if(point_number < BAC_OUTPUT_ITEM_COUNT)
					{
						Post_Refresh_One_Message(g_bac_instance,READOUTPUT_T3000,point_number,point_number,	sizeof(Str_out_point));
						Fresh_Program_List(point_type);
					}
					break;
				case BAC_IN:
					if(point_number < BAC_INPUT_ITEM_COUNT)
					{
						Post_Refresh_One_Message(g_bac_instance,READINPUT_T3000,point_number,point_number,	sizeof(Str_in_point));
						Fresh_Program_List(point_type);
					}
					break;
				case BAC_VAR:
					if(point_number < BAC_VARIABLE_ITEM_COUNT)
					{
						Post_Refresh_One_Message(g_bac_instance,READVARIABLE_T3000,point_number,point_number,	sizeof(Str_variable_point));
						Fresh_Program_List(point_type);
					}
					break;
				default:
					break;
				}
				
			}
		}
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CBacnetProgramDebug::OnNMKillfocusDatetimepickerPrgVariable(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	if(!m_prg_debug_variable_time_picker.IsWindowVisible())
		return;
	if(m_Variable_data.at(point_number).range != 20)
		return;
	CTime temp_time;CString temp_cs;
	int chour,cmin;
	m_prg_debug_variable_time_picker.GetTime(temp_time);
	chour = temp_time.GetHour();
	cmin = temp_time.GetMinute();



	if((chour == 0) &&(cmin	==0))
	{
		temp_cs.Empty();
	}
	else
		temp_cs.Format(_T("%02d:%02d"),chour,cmin);
	m_program_debug_list.SetItemText(0,VARIABLE_VALUE,temp_cs);

	m_prg_debug_variable_time_picker.ShowWindow(SW_HIDE);

	int write_value;
	write_value =( chour*3600 + cmin * 60) * 1000;
	m_Variable_data.at(point_number).value = write_value;
	CString temp_task_info;
	temp_task_info.Format(_T("Write Variable Time Item%d .Changed Time to \"%s\" "),point_number + 1,temp_cs);
	Post_Write_Message(g_bac_instance,WRITEVARIABLE_T3000,point_number,point_number,sizeof(Str_variable_point),BacNet_hwd,temp_task_info);



	*pResult = 0;

}
