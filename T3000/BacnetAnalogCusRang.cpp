// BacnetAnalogCusRang.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetAnalogCusRang.h"
#include "afxdialogex.h"
#include "global_function.h"

CString C_or_V;

int max_unit = 0xffffffff;
int min_unit = 0x7fffffff;

int max_voltage = 0xffffffff;
int min_voltage = 0x7fffffff;

int voltage_up = -1;
int voltage_down = -1;

int value_up = -1;
int value_down = -1;

int firmware_support_high_precision = 0; // 0xef   0.01.  firmwware need Rev 64.8
int enable_high_precision = 0; // 0xef   0.01.  firmwware need Rev 64.8
// CBacnetAnalogCusRang dialog
HWND temp_gloab_hwnd =NULL;
IMPLEMENT_DYNAMIC(CBacnetAnalogCusRang, CDialogEx)

CBacnetAnalogCusRang::CBacnetAnalogCusRang(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetAnalogCusRang::IDD, pParent)
{
    n_point_count = 1;
    max_unit = 0xffffffff;
    min_unit = 0x7fffffff;

    max_voltage = 0xffffffff;
    min_voltage = 0x7fffffff;

    voltage_up = -1;
    voltage_down = -1;

    value_up = -1;
    value_down = -1;
}

CBacnetAnalogCusRang::~CBacnetAnalogCusRang()
{
}

void CBacnetAnalogCusRang::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDD_DIALOG_BACNET_RANGE_LIST, m_analog_cus_range_list);
    DDX_Control(pDX, IDC_SLIDER_BAC_DEF_1, m_cus_analog_range_ctrl_0);
    DDX_Control(pDX, IDC_SLIDER_BAC_DEF_2, m_cus_analog_range_ctrl_1);
    DDX_Control(pDX, IDC_SLIDER_BAC_DEF_3, m_cus_analog_range_ctrl_2);
    DDX_Control(pDX, IDC_SLIDER_BAC_DEF_4, m_cus_analog_range_ctrl_3);
    DDX_Control(pDX, IDC_SLIDER_BAC_DEF_5, m_cus_analog_range_ctrl_4);
    DDX_Control(pDX, IDC_SLIDER_BAC_DEF_6, m_cus_analog_range_ctrl_5);
    DDX_Control(pDX, IDC_SLIDER_BAC_DEF_7, m_cus_analog_range_ctrl_6);
    DDX_Control(pDX, IDC_SLIDER_BAC_DEF_8, m_cus_analog_range_ctrl_7);
    DDX_Control(pDX, IDC_SLIDER_BAC_DEF_9, m_cus_analog_range_ctrl_8);
    DDX_Control(pDX, IDC_SLIDER_BAC_DEF_10, m_cus_analog_range_ctrl_9);
    DDX_Control(pDX, IDC_SLIDER_BAC_DEF_11, m_cus_analog_range_ctrl_10);
    DDX_Control(pDX, IDC_STATIC_DATA_STATUS, m_static_data_status);
}


BEGIN_MESSAGE_MAP(CBacnetAnalogCusRang, CDialogEx)
	 ON_WM_VSCROLL()
	ON_MESSAGE(MY_RESUME_DATA, AnalogRangeTblMessageCallBack)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_AnalogCusRange_Item)
	ON_MESSAGE(WM_REFRESH_BAC_ANALOGCUSRANGE_LIST,Fresh_AnalogCusRange_List)
	ON_WM_CLOSE()
	  ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
	  ON_EN_KILLFOCUS(IDC_EDIT_BAC_CUS_ANALOG_UNIT, &CBacnetAnalogCusRang::OnEnKillfocusEditBacCusAnalogUnit)
	  ON_WM_PAINT()
	  ON_WM_TIMER()
    ON_EN_KILLFOCUS(IDC_EDITCUS_POINT_VALUE, &CBacnetAnalogCusRang::OnEnKillfocusEditcusPointValue)
    ON_EN_KILLFOCUS(IDC_EDIT_MIN_VOLT_VALUE, &CBacnetAnalogCusRang::OnEnKillfocusEditMinVoltValue)
    ON_EN_KILLFOCUS(IDC_EDIT_MIN_VALUE_VALUE, &CBacnetAnalogCusRang::OnEnKillfocusEditMinValueValue)
    ON_EN_KILLFOCUS(IDC_EDIT_MAX_VOLT_VALUE3, &CBacnetAnalogCusRang::OnEnKillfocusEditMaxVoltValue3)
    ON_EN_KILLFOCUS(IDC_EDIT_MAX_VALUE_VALUE, &CBacnetAnalogCusRang::OnEnKillfocusEditMaxValueValue)
    ON_BN_CLICKED(IDC_BUTTON_APPLY, &CBacnetAnalogCusRang::OnBnClickedButtonApply)
    ON_CBN_KILLFOCUS(IDC_COMBO_CUSRANGE_STIGNALTYPE, &CBacnetAnalogCusRang::OnCbnKillfocusComboCusrangeStignaltype)
    ON_NOTIFY(NM_CLICK, IDD_DIALOG_BACNET_RANGE_LIST, &CBacnetAnalogCusRang::OnNMClickDialogBacnetRangeList)
    ON_CBN_SELCHANGE(IDC_COMBO_PRECISION, &CBacnetAnalogCusRang::OnCbnSelchangeComboPrecision)
END_MESSAGE_MAP()


// CBacnetAnalogCusRang message handlers
LRESULT  CBacnetAnalogCusRang::AnalogRangeTblMessageCallBack(WPARAM wParam, LPARAM lParam)
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
	}
	else
	{
        // Revert values that were not correctly modified
		memcpy_s(&m_Program_data.at(pInvoke->mRow),sizeof(Str_program_point),&m_temp_program_data[pInvoke->mRow],sizeof(Str_program_point));//��ԭû�иĶԵ�ֵ
		PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
	}
	if((pInvoke->mRow%2)==0)  //�ָ�ǰ���� ���� ��ɫ;, Restore foreground and background colors
		m_analog_cus_range_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_analog_cus_range_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
	m_analog_cus_range_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);

	Fresh_AnalogCusRange_List(analog_range_tbl_line,analog_range_tbl_line);

	if(pInvoke)
		delete pInvoke;
	return 0;
}
CString cs_windowtext;
BOOL CBacnetAnalogCusRang::OnInitDialog()
{
	CDialogEx::OnInitDialog();

    m_static_data_status.SetWindowTextW(_T(""));
    m_static_data_status.textColor(RGB(255, 0, 0));
    m_static_data_status.setFont(24, 14, NULL, _T("Arial"));
    m_static_data_status.bkColor(RGB(0,255,255));

	m_tooltips.Create(GetDlgItem(IDC_SLIDER_BAC_DEF_1));

    SetWindowTextW(cs_windowtext);

	m_tooltips.Activate(TRUE);

    if (Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub >= 648)
    {
		firmware_support_high_precision = 1;
	}
    else
    {
		firmware_support_high_precision = 0;
	}


	Initial_List();
    InitialPointCount();
	Fresh_AnalogCusRange_List(analog_range_tbl_line, analog_range_tbl_line);
	SetTimer(1,5000,NULL);
    SetTimer(2, 500, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CBacnetAnalogCusRang::OnCancel()
{
	
	Fresh_AnalogCusRange_Item(0,0);
	//g_hwnd_now =	temp_gloab_hwnd;
	//analog_cus_range_dlg = NULL;
	CDialogEx::OnCancel();
}


void CBacnetAnalogCusRang::Initial_List()
{
	m_analog_cus_range_list.ShowWindow(SW_HIDE);
	m_analog_cus_range_list.DeleteAllItems();
	while ( m_analog_cus_range_list.DeleteColumn (0)) ;

#pragma region jumpersetting

    ((CComboBox *)GetDlgItem(IDC_COMBO_CUSRANGE_STIGNALTYPE))->ResetContent();
  

        for (int j = 0;j<(int)sizeof(JumperStatus) / sizeof(JumperStatus[0]);j++)
        {
            // Previously, for some reason, both 0 and 4 represented Thermistor Dry Contact; 
            // to avoid showing duplicates in the dropdown, one is filtered out here
            if (j == 4)   //��ǰ����ĳЩԭ��  0  �� 4 ������ Thermistor Dry Contact; ����������ϣ����ʾ���� һ���ģ����Թ��˵�
                continue;
            ((CComboBox *)GetDlgItem(IDC_COMBO_CUSRANGE_STIGNALTYPE))->AddString(JumperStatus[j]);
        }
#pragma endregion
        ((CComboBox*)GetDlgItem(IDC_COMBO_PRECISION))->ResetContent();
        ((CComboBox*)GetDlgItem(IDC_COMBO_PRECISION))->AddString(_T("0.1"));
        ((CComboBox*)GetDlgItem(IDC_COMBO_PRECISION))->AddString(_T("0.01"));
        if (firmware_support_high_precision)
        {
            ((CComboBox*)GetDlgItem(IDC_COMBO_PRECISION))->ShowWindow(1);
            ((CComboBox*)GetDlgItem(IDC_STATIC_PRECISION))->ShowWindow(1);
            if ((unsigned char)m_analog_custmer_range.at(analog_range_tbl_line).table_name[8] == 0xef)
            {
                enable_high_precision = 1;
                ((CComboBox*)GetDlgItem(IDC_COMBO_PRECISION))->SetWindowText(_T("0.01"));
            }
            else
            {
                enable_high_precision = 0;
                ((CComboBox*)GetDlgItem(IDC_COMBO_PRECISION))->SetWindowText(_T("0.1"));
			}
        }
        else
        {
            ((CComboBox*)GetDlgItem(IDC_COMBO_PRECISION))->ShowWindow(0);
            ((CComboBox*)GetDlgItem(IDC_STATIC_PRECISION))->ShowWindow(0);
            // If the firmware doesn't support it, even if a new program is loaded with 0.01 precision, the firmware will execute it using 0.1 precision
            enable_high_precision = 0;  //�̼���֧�ֵĻ� ����load �µ�prog ��0.01���Ƚ��� �̼����ᰴ0.1��ȥִ��
        }
     

	int temp_jumper = 0;
	CString Unit_temp;
	temp_jumper = (m_dialog_signal_type & 0xf0 ) >> 4;
	if(temp_jumper == 1)
	{
        GetDlgItem(IDC_STATIC_CUS_MIN_LABLE)->SetWindowTextW(_T("Current :"));
        GetDlgItem(IDC_STATIC_CUS_MAX_LABLE2)->SetWindowTextW(_T("Current :"));
        GetDlgItem(IDC_STATIC_VOLTS_OR_CURRENT)->SetWindowTextW(_T("Current (ma):"));
		Unit_temp.Format(_T("Current (4-20ma)"));
		C_or_V.Format(_T("ma"));
        ((CComboBox *)GetDlgItem(IDC_COMBO_CUSRANGE_STIGNALTYPE))->SetWindowText(JumperStatus[temp_jumper]);
	}
	else if(temp_jumper == 2)
	{
        GetDlgItem(IDC_STATIC_CUS_MIN_LABLE)->SetWindowTextW(_T("Volts :"));
        GetDlgItem(IDC_STATIC_CUS_MAX_LABLE2)->SetWindowTextW(_T("Volts :"));
        GetDlgItem(IDC_STATIC_VOLTS_OR_CURRENT)->SetWindowTextW(_T("Volts :"));
		Unit_temp.Format(_T("Voltage (0-5V)"));
		C_or_V.Format(_T("V"));
        ((CComboBox *)GetDlgItem(IDC_COMBO_CUSRANGE_STIGNALTYPE))->SetWindowText(JumperStatus[temp_jumper]);
	}
	else if(temp_jumper == 3)
	{
        GetDlgItem(IDC_STATIC_CUS_MIN_LABLE)->SetWindowTextW(_T("Volts :"));
        GetDlgItem(IDC_STATIC_CUS_MAX_LABLE2)->SetWindowTextW(_T("Volts :"));
        GetDlgItem(IDC_STATIC_VOLTS_OR_CURRENT)->SetWindowTextW(_T("Volts :"));
		Unit_temp.Format(_T("Voltage (0-10V)"));
		C_or_V.Format(_T("V"));
        ((CComboBox *)GetDlgItem(IDC_COMBO_CUSRANGE_STIGNALTYPE))->SetWindowText(JumperStatus[temp_jumper]);
	}
    else if ((temp_jumper == 0) || (temp_jumper == 4))
    {
        GetDlgItem(IDC_STATIC_CUS_MIN_LABLE)->SetWindowTextW(_T("Volts :"));
        GetDlgItem(IDC_STATIC_CUS_MAX_LABLE2)->SetWindowTextW(_T("Volts :"));
        GetDlgItem(IDC_STATIC_VOLTS_OR_CURRENT)->SetWindowTextW(_T("Volts :"));
        Unit_temp.Format(_T("Voltage"));
        C_or_V.Format(_T("V"));
        ((CComboBox *)GetDlgItem(IDC_COMBO_CUSRANGE_STIGNALTYPE))->SetWindowText(JumperStatus[temp_jumper]);
    }
	else 
	{
        GetDlgItem(IDC_STATIC_CUS_MIN_LABLE)->SetWindowTextW(_T("Volts :"));
        GetDlgItem(IDC_STATIC_CUS_MAX_LABLE2)->SetWindowTextW(_T("Volts :"));
        GetDlgItem(IDC_STATIC_VOLTS_OR_CURRENT)->SetWindowTextW(_T("Volts :"));
		Unit_temp.Format(_T("Voltage"));
		C_or_V.Format(_T("V"));
	}



	m_analog_cus_range_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	//m_analog_cus_range_list.SetExtendedStyle(m_analog_cus_range_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_analog_cus_range_list.SetExtendedStyle(m_analog_cus_range_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_analog_cus_range_list.InsertColumn(0, Unit_temp, 70, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_analog_cus_range_list.InsertColumn(1, _T("Value"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);

	temp_gloab_hwnd = g_hwnd_now;
	//g_hwnd_now = this->m_hWnd;
	analog_cus_range_dlg = this->m_hWnd;

	CRect list_rect,win_rect;
	m_analog_cus_range_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_input_dlg_hwnd,win_rect);
	m_analog_cus_range_list.Set_My_WindowRect(win_rect);
	m_analog_cus_range_list.Set_My_ListRect(list_rect);
	m_analog_cus_range_list.Support_Col_0_Edit(true);
	m_analog_cus_range_list.SetListHwnd(this->m_hWnd);
	//m_analog_cus_range_list.DeleteAllItems();
	for (int i=0;i<11;i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;

        // The size of the vector remains constant; this variable is used to limit the number of items displayed
		if(i>=input_item_limit_count)	//vector�Ĵ�Сʼ�ղ��� ,�ôα����� Լ�� Ҫ��ʾ�� item ����;
			break;

		temp_item.Format(_T("%.1f"),i*0.5);
		m_analog_cus_range_list.InsertItem(i,temp_item);



		for (int x=0;x<2;x++)
		{
			if((i%2)==0)
				m_analog_cus_range_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_analog_cus_range_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}
	}
	m_analog_cus_range_list.InitListData();
	m_analog_cus_range_list.ShowWindow(SW_SHOW);
}



LRESULT CBacnetAnalogCusRang::Fresh_AnalogCusRange_Item(WPARAM wParam,LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;
	CString cs_info;

    CString cs_temp = m_analog_cus_range_list.GetItemText(Changed_Item, ANALOG_CUS_RANGE_RBL_VALUE);

    if (Changed_Item >= n_point_count) 
    {
        //�ж��û��ǲ�����������, Determine if the user wants to add a new point
        if(cs_temp.IsEmpty())
            return 0;
        for (int j = Changed_Item; j < 11; j++)
        {
            m_analog_custmer_range.at(analog_range_tbl_line).dat[j].m_volts = 0;
            m_analog_custmer_range.at(analog_range_tbl_line).dat[j].m_value = 0;
        }
        
        m_analog_cus_range_list.SetItemText(Changed_Item, 0, _T(""));
        m_analog_cus_range_list.SetItemText(Changed_Item, 1, _T(""));
        Changed_Item = n_point_count;
        m_analog_cus_range_list.SetItemText(Changed_Item, 0, cs_temp);
        m_analog_cus_range_list.SetCellEnabled(Changed_Item, 1, true);

        float temp_value = _wtof(cs_temp);
        if (enable_high_precision == 1)
        {
            // The last byte of the table name needs to be marked as high precision 0.01
            m_analog_custmer_range.at(analog_range_tbl_line).table_name[8] = 0xef; //table ���ֵ����һ���ֽ���Ҫ��ʶΪ�߾��� 0.01
            if ((temp_value > 655.35) || (temp_value < 0))
            {
                m_analog_cus_range_list.SetItemText(Changed_Item, 0, _T(""));
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Value Out of range"));
                return 0;
            }
            m_analog_custmer_range.at(analog_range_tbl_line).dat[Changed_Item].m_volts = (unsigned short)(temp_value * 100);
        }
        else 
        {
            if ((temp_value > 6553.5) || (temp_value < 0))
            {
                m_analog_cus_range_list.SetItemText(Changed_Item, 0, _T(""));
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Value Out of range"));
                return 0;
            }
            m_analog_custmer_range.at(analog_range_tbl_line).dat[Changed_Item].m_volts = (unsigned short)(temp_value * 10);
        }

 

    }

    // If even one value is modified, reload all values from the interface into memory and write them to the device
	for (int i=0;i<11;i++)//ֻҪ���޸�1���������½����������е�ֵ ���� �ڴ� ��д�� �豸;
	{
			CString cs_temp = m_analog_cus_range_list.GetItemText(i,ANALOG_CUS_RANGE_RBL_VALUE);
			float temp_value =_wtof(cs_temp);
            if (enable_high_precision == 1)
            {
                if ((temp_value > 655.35) || (temp_value < 0))
                {
					m_analog_cus_range_list.SetItemText(Changed_Item, 0, _T(""));
					SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Value Out of range"));
					continue;
				}
                m_analog_custmer_range.at(analog_range_tbl_line).dat[i].m_volts = (unsigned short)(temp_value * 100);
            }
            else
            {
                if ((temp_value > 6553.5) || (temp_value < 0))
                {
                    m_analog_cus_range_list.SetItemText(Changed_Item, 0, _T(""));
                    SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Value Out of range"));
                    continue;
                }
                m_analog_custmer_range.at(analog_range_tbl_line).dat[i].m_volts = (unsigned short)(temp_value * 10);
            }


			CString cs_temp1 = m_analog_cus_range_list.GetItemText(i,ANALOG_CUS_RANGE_RBL_UNIT);
			
            if (enable_high_precision == 1)
            {
                float temp_value1 = _wtof(cs_temp1);
                m_analog_custmer_range.at(analog_range_tbl_line).dat[i].m_value = (unsigned int)(temp_value1 * 100); ;
            }
            else
            {
                int temp_value1 = _wtoi(cs_temp1);
                m_analog_custmer_range.at(analog_range_tbl_line).dat[i].m_value = temp_value1;
            }
			
			if(i==0)
			{
				GetDlgItem(IDC_STATIC_VC_1)->SetWindowText(cs_temp);
				GetDlgItem(IDC_STATIC_VALUE_1)->SetWindowText(cs_temp1);
			}
	}

	bool ret_check = false;
	ret_check = CheckAllDataValid();
	if(!ret_check)
	{
		//MessageBox(_T("Make sure table values are of steadily increasing or decreasing values, reversing inflection will cause the lookup function to fail."));
        m_static_data_status.ShowWindow(SW_SHOW);
        m_static_data_status.SetWindowTextW(_T("Inflection point\r\n detected"));
		return 0;
	}
    else
    {
        m_static_data_status.ShowWindow(SW_HIDE);
    }
    //if ((g_selected_product_id == PM_T38AI8AO6DO) ||
    //    (g_selected_product_id == PM_T322AI))
    //{

    //}
    //else
    //{
        CString temp_task_info;
        temp_task_info.Format(_T("Update Analog Range  "));
        Post_Write_Message(g_bac_instance, WRITEANALOG_CUS_TABLE_T3000, analog_range_tbl_line, analog_range_tbl_line, sizeof(Str_table_point), analog_cus_range_dlg, temp_task_info, Changed_Item, Changed_SubItem);
    //}

	return 0;
}


BOOL CBacnetAnalogCusRang::CheckAllDataValid()
{
	int slop_value[11];

	int voltage[11];
	int mvalue[11];
	voltage_up = -1;
	voltage_down = -1;

	value_up = -1;
	value_down = -1;

	memset(voltage,0,sizeof(voltage));
	memset(mvalue,0,sizeof(mvalue));


	max_voltage = 0xffffffff;
	min_voltage = 0x7fffffff;

	for (int i=0;i<11;i++)
	{
        if (i >= n_point_count)
            break;
	
		voltage[i] = m_analog_custmer_range.at(analog_range_tbl_line).dat[i+1].m_volts - m_analog_custmer_range.at(analog_range_tbl_line).dat[i].m_volts;
	
		mvalue[i] = m_analog_custmer_range.at(analog_range_tbl_line).dat[i+1].m_value - m_analog_custmer_range.at(analog_range_tbl_line).dat[i].m_value;
	}
	for (int i=0;i<11;i++)
	{
        if (i >= n_point_count - 1)
            break;

		if(mvalue[i] > 0)
		{
			value_up = 1;
		}
		else if(mvalue[i] < 0)
		{
			value_down = 1;
		}
	}

	for (int i=0;i<11;i++)
	{
        if (i >= n_point_count - 1)
            break;

		if(voltage[i] > 0)
		{
			voltage_up = 1;
		}
		else if(voltage[i] < 0)
		{
			voltage_down = 1;
		}
	}
    if((value_up == 1) && (value_up == value_down))
	{
		return 0;
	}

    if ((voltage_up == 1) && (voltage_up == voltage_down))
    {
        return 0;
    }


	return 1;
}

LRESULT CBacnetAnalogCusRang::Fresh_AnalogCusRange_List(WPARAM wParam,LPARAM lParam)
{
	// Str_in_point Get_Str_in_Point(int index);
    max_unit = 0xffffffff;
    min_unit = 0x7fffffff;

    char temp_char[10] = { 0 };
    // The last digit is used to indicate precision, distinguishing it from the old version's 0.1
    if ((unsigned char)m_analog_custmer_range.at(analog_range_tbl_line).table_name[8] != 0xef) //���һλ������ʶ ���� ����ɰ汾��0.1 ����
    {
        memcpy_s(temp_char, 9, (char*)m_analog_custmer_range.at(analog_range_tbl_line).table_name, 9);
    }
    else
    {
        memcpy_s(temp_char, 9, (char*)m_analog_custmer_range.at(analog_range_tbl_line).table_name, 8);
    }

	CString temp_show_unit;
	MultiByteToWideChar( CP_ACP, 0, (char *)temp_char, (int)strlen(temp_char)+1,
		temp_show_unit.GetBuffer(MAX_PATH), MAX_PATH );
	temp_show_unit.ReleaseBuffer();


	if(temp_show_unit.GetLength() > 9)
		temp_show_unit = temp_show_unit.Left(9);
	Analog_Customer_Units[analog_range_tbl_line] = temp_show_unit;
	((CEdit *)GetDlgItem(IDC_EDIT_BAC_CUS_ANALOG_UNIT))->SetWindowText(temp_show_unit);
	
	for (int i=0;i<11;i++)
	{
        if (i >= n_point_count)
        {
            m_analog_cus_range_list.SetItemText(i, 0, _T(""));
            m_analog_cus_range_list.SetItemText(i, 1, _T(""));
            continue;
        }
           
		CString n_value_2byte;
		CString n_unite_4byte;
		//if((i!=0) && (m_analog_custmer_range.at(analog_range_tbl_line).dat[i].value == 0))
		//{
		//	//n_value_2byte.Format(_T("-"));
		//	//n_unite_4byte.Format(_T("-"));
		//	n_value_2byte.Empty();
		//	n_unite_4byte.Empty();
		//}
		//else
		//{
        if (enable_high_precision == 1)
        {
            n_value_2byte.Format(_T("%.2f"), ((float)m_analog_custmer_range.at(analog_range_tbl_line).dat[i].m_volts) / 100);
            n_unite_4byte.Format(_T("%.2f"), ((float)m_analog_custmer_range.at(analog_range_tbl_line).dat[i].m_value) / 100);
        }
        else
        {
            n_value_2byte.Format(_T("%.1f"), ((float)m_analog_custmer_range.at(analog_range_tbl_line).dat[i].m_volts) / 10);
            n_unite_4byte.Format(_T("%d"), m_analog_custmer_range.at(analog_range_tbl_line).dat[i].m_value);
        }
		//}

            if (max_unit < m_analog_custmer_range.at(analog_range_tbl_line).dat[i].m_value)
                max_unit = m_analog_custmer_range.at(analog_range_tbl_line).dat[i].m_value;
            if (min_unit > m_analog_custmer_range.at(analog_range_tbl_line).dat[i].m_value)
                min_unit = m_analog_custmer_range.at(analog_range_tbl_line).dat[i].m_value;
		m_analog_cus_range_list.SetItemText(i,0,n_value_2byte);
		m_analog_cus_range_list.SetItemText(i,1,n_unite_4byte);

		if(i==0)
		{
			if (!n_value_2byte.IsEmpty())
				GetDlgItem(IDC_STATIC_VC_1)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_1)->SetWindowText(n_unite_4byte);
		}
		else if(i==1)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_2)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_2)->SetWindowText(n_unite_4byte);
		}
		else if(i==2)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_3)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_3)->SetWindowText(n_unite_4byte);
		}
		else if(i==3)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_4)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_4)->SetWindowText(n_unite_4byte);
		}
		else if(i==4)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_5)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_5)->SetWindowText(n_unite_4byte);
		}
		else if(i==5)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_6)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_6)->SetWindowText(n_unite_4byte);
		}
		else if(i==6)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_7)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_7)->SetWindowText(n_unite_4byte);
		}
		else if(i==7)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_8)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_8)->SetWindowText(n_unite_4byte);
		}
		else if(i==8)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_9)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_9)->SetWindowText(n_unite_4byte);
		}
		else if(i==9)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_10)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_10)->SetWindowText(n_unite_4byte);
		}
		else if(i==10)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_11)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_11)->SetWindowText(n_unite_4byte);
		}
	}



    
	SetSlideRange();

	return 0;
}

void CBacnetAnalogCusRang::SetSlideRange()
{
	if(max_unit == min_unit)
	{
		max_unit = min_unit + 1;
	}
	  m_cus_analog_range_ctrl_0.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_1.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_2.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_3.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_4.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_5.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_6.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_7.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_8.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_9.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_10.SetRange(min_unit,max_unit);	

	  m_cus_analog_range_ctrl_0.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[0].m_value -min_unit));
	  m_cus_analog_range_ctrl_1.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[1].m_value -min_unit));
	  m_cus_analog_range_ctrl_2.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[2].m_value -min_unit));
	  m_cus_analog_range_ctrl_3.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[3].m_value -min_unit));
	  m_cus_analog_range_ctrl_4.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[4].m_value -min_unit));
	  m_cus_analog_range_ctrl_5.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[5].m_value -min_unit));
	  m_cus_analog_range_ctrl_6.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[6].m_value -min_unit));
	  m_cus_analog_range_ctrl_7.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[7].m_value -min_unit));
	  m_cus_analog_range_ctrl_8.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[8].m_value -min_unit));
	  m_cus_analog_range_ctrl_9.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[9].m_value -min_unit));
	  m_cus_analog_range_ctrl_10.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[10].m_value -min_unit));

      Handle_Static_ctrl_0();
      Handle_Static_ctrl_1();
      Handle_Static_ctrl_2();
      Handle_Static_ctrl_3();
      Handle_Static_ctrl_4();
      Handle_Static_ctrl_5();
      Handle_Static_ctrl_6();
      Handle_Static_ctrl_7();
      Handle_Static_ctrl_8();
      Handle_Static_ctrl_9();
      Handle_Static_ctrl_10();

	  UpdateData();
}


void CBacnetAnalogCusRang::OnClose()
{
	 
	//g_hwnd_now =	temp_gloab_hwnd;
	//analog_cus_range_dlg = NULL;
	CDialogEx::OnClose();
}


BOOL CBacnetAnalogCusRang::PreTranslateMessage(MSG* pMsg)
{
	
	CWnd * window_focus =	GetFocus();
	if(window_focus == NULL)
		return 0;
	int temp_focus_id = GetFocus()->GetDlgCtrlID();
	if(temp_focus_id == IDC_EDIT_BAC_CUS_ANALOG_UNIT )
	{
		if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		{
			UpdateCusAnalogUnit();
			return TRUE;
		}
	}
    else if ((temp_focus_id == IDC_EDITCUS_POINT_VALUE) ||
             (temp_focus_id == IDC_EDIT_MIN_VOLT_VALUE) ||
             (temp_focus_id == IDC_EDIT_MIN_VALUE_VALUE) ||
             (temp_focus_id == IDC_EDIT_MAX_VOLT_VALUE3) ||
             (temp_focus_id == IDC_EDIT_MAX_VALUE_VALUE))
    {
        if (pMsg->wParam == VK_RETURN)
        {
            ReSetSlideAndList();
            if (temp_focus_id == IDC_EDIT_MAX_VALUE_VALUE)
            {
                ReAllocationValue();
            }
            if (temp_focus_id == IDC_EDIT_MAX_VOLT_VALUE3)
            {
                ReAllocationVolts();
            }

            return TRUE;
        }
    }
	else if((pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)  || (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE))
	{
		CWnd * window_focus =	GetFocus();
		if(window_focus == NULL)
			return CDialogEx::PreTranslateMessage(pMsg);
		if((GetFocus()->GetDlgCtrlID() == IDD_DIALOG_BACNET_RANGE_LIST) && (pMsg->wParam==VK_RETURN))
		{
			CRect list_rect,win_rect;
			m_analog_cus_range_list.GetWindowRect(list_rect);
			ScreenToClient(&list_rect);
			::GetWindowRect(analog_cus_range_dlg,win_rect);
			m_analog_cus_range_list.Set_My_WindowRect(win_rect);
			m_analog_cus_range_list.Set_My_ListRect(list_rect);

			m_analog_cus_range_list.Get_clicked_mouse_position();
			return TRUE;
		}
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}



void CBacnetAnalogCusRang::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{




	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_0.m_hWnd)
	{
        Handle_Static_ctrl_0();
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_1.m_hWnd)
	{
        Handle_Static_ctrl_1();
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_2.m_hWnd)
	{
        Handle_Static_ctrl_2();
	}	
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_3.m_hWnd)
	{
        Handle_Static_ctrl_3();
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_4.m_hWnd)
	{
        Handle_Static_ctrl_4();
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_5.m_hWnd)
	{
        Handle_Static_ctrl_5();
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_6.m_hWnd)
	{
        Handle_Static_ctrl_6();
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_7.m_hWnd)
	{
        Handle_Static_ctrl_7();
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_8.m_hWnd)
	{
        Handle_Static_ctrl_8();
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_9.m_hWnd)
	{
        Handle_Static_ctrl_9();
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_10.m_hWnd)
	{
        Handle_Static_ctrl_10();
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CBacnetAnalogCusRang::OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult)
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	UINT nID =pNMHDR->idFrom;
	if (pTTT->uFlags & TTF_IDISHWND)
	{
		// idFrom is actually the HWND of the tool
		nID = ::GetDlgCtrlID((HWND)nID);
		if(nID)
		{
			//  ���������Ҫ��ʾ��Tooltips�����Ը�����Ҫ������
            // This is the tooltip you want to display, customizable as needed
			CString strToolTips;
			strToolTips.Format(_T("%d"), m_tipvalue);
			pTTT->lpszText=strToolTips.AllocSysString();

			pTTT->hinst = NULL;
			return(TRUE);
		}
	}

	return(FALSE);
}


void CBacnetAnalogCusRang::OnEnKillfocusEditBacCusAnalogUnit()
{
	

	UpdateCusAnalogUnit();
}


void CBacnetAnalogCusRang::UpdateCusAnalogUnit()
{
	CString temp_cs;
	((CEdit *)GetDlgItem(IDC_EDIT_BAC_CUS_ANALOG_UNIT))->GetWindowTextW(temp_cs);
	if(temp_cs.GetLength() > 9)
	{
		MessageBox(_T("Nodes label length must between 1-9"),_T("Notice"),MB_OK | MB_ICONINFORMATION);
		return;
	}

	char cTemp1[255];
	memset(cTemp1,0,255);
	WideCharToMultiByte( CP_ACP, 0, temp_cs.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
    if (enable_high_precision == 1)
    {
        memcpy_s(m_analog_custmer_range.at(analog_range_tbl_line).table_name, 9, cTemp1, 8);
        // The last byte of the table name must be marked as high precision 0.01
        m_analog_custmer_range.at(analog_range_tbl_line).table_name[8] = 0xef; //table ���ֵ����һ���ֽ���Ҫ��ʶΪ�߾��� 0.01
    }
    else
    {
        memcpy_s(m_analog_custmer_range.at(analog_range_tbl_line).table_name, 9, cTemp1, 9);
    }


	//m_analog_custmer_range.at(analog_range_tbl_line).table_name
	bool ret_check = false;
	ret_check = CheckAllDataValid();
	if(!ret_check)
	{
		//MessageBox(_T("Make sure table values are of steadily increasing or decreasing values, reversing inflection will cause the lookup function to fail. "));
        m_static_data_status.ShowWindow(SW_SHOW);
        m_static_data_status.SetWindowTextW(_T("Inflection point\r\n detected"));
		return ;
	}
    else
    {
        m_static_data_status.ShowWindow(SW_HIDE);
    }
	CString temp_task_info;
	temp_task_info.Format(_T("Update Analog Range Unit : %s  "),temp_cs);
	Post_Write_Message(g_bac_instance,WRITEANALOG_CUS_TABLE_T3000,analog_range_tbl_line,analog_range_tbl_line,sizeof(Str_table_point),analog_cus_range_dlg ,temp_task_info);
    Analog_Customer_Units[analog_range_tbl_line] = temp_cs;
}


void CBacnetAnalogCusRang::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	

	
}




void CBacnetAnalogCusRang::OnTimer(UINT_PTR nIDEvent)
{

    switch (nIDEvent)
    {
    case 1:
    {
        static int n_loop_count = 0;
        n_loop_count = (++n_loop_count) % 4;
        if (n_loop_count == 0)
            Post_Refresh_Message(g_bac_instance, READINPUT_T3000, input_item_select_for_range, input_item_select_for_range, sizeof(Str_in_point), 1 /*BAC_INPUT_GROUP*/);

        float real_voltage = ((float)m_Input_data.at(input_item_select_for_range).value) / 1000;

        int temp_index_of = 0;
        for (int i = 0;i < 10;i++)
        {
            if ((real_voltage >= m_analog_custmer_range.at(analog_range_tbl_line).dat[i].m_volts / 10) &&
                (real_voltage <= m_analog_custmer_range.at(analog_range_tbl_line).dat[i + 1].m_volts / 10))
            {
                temp_index_of = i;
                break;
            }
        }

        float show_value = 0;
        if (voltage_up)
        {
            if (max_voltage != min_voltage)
            {
                //show_value =  ((float)(max_unit - min_unit))/((float)(max_voltage - min_voltage)/10) *  (real_voltage -((float)min_voltage)/10) + min_unit;
                float y2 = m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of + 1].m_value;
                float y1 = m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of].m_value;
                float x2 = m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of + 1].m_volts;
                float x1 = m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of].m_volts;
                float x3 = real_voltage * 10;
                if (x2 == x1)
                    show_value = 0;
                else
                    show_value = (y2 - y1) / (x2 - x1)*(x3 - x1) + y1;
                //show_value =  ((float)(m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of+1].unit - m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of].unit))/(((float)(m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of+1].value - m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of].value))/10) *  (real_voltage -((float)m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of].value)/10) + m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of].value;
            }
        }
        CString temp_voltage;
        temp_voltage.Format(_T("%.2f"), real_voltage);
        ((CStatic *)GetDlgItem(IDC_STATIC_CUS_ANALOG_VALUE))->SetWindowText(temp_voltage);
        //
        CString temp_unit;
        ((CEdit *)GetDlgItem(IDC_EDIT_BAC_CUS_ANALOG_UNIT))->GetWindowTextW(temp_unit);
        CString ret_string;
        ret_string.Format(_T("IN%u  %.3f  %s"), input_item_select_for_range + 1, show_value, temp_unit);
        ((CEdit *)GetDlgItem(IDC_STATIC_CUS_RANGE_VALUE))->SetWindowText(ret_string);
        //TRACE(_T("%f\r\n"),show_value);
    }
    break;
    case 2:
    {
        KillTimer(2);
        ReSetSlideAndList();
        SetSlideRange();
    }
    break;
    default:
        break;
    }
    CDialogEx::OnTimer(nIDEvent);
}


void CBacnetAnalogCusRang::OnEnKillfocusEditcusPointValue()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    // TODO: Add control notification handler code here
    ReSetSlideAndList();
    PostMessage(WM_LIST_ITEM_CHANGED, NULL, NULL);
}


void CBacnetAnalogCusRang::OnEnKillfocusEditMinVoltValue()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    // TODO: Add control notification handler code here
    ReSetSlideAndList();
    ReAllocationVolts();
    PostMessage(WM_LIST_ITEM_CHANGED, NULL, NULL);
}


void CBacnetAnalogCusRang::OnEnKillfocusEditMinValueValue()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    // TODO: Add control notification handler code here
    ReSetSlideAndList();
    ReAllocationValue();
    PostMessage(WM_LIST_ITEM_CHANGED, NULL, NULL);
}


void CBacnetAnalogCusRang::OnEnKillfocusEditMaxVoltValue3()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    // TODO: Add control notification handler code here
    ReSetSlideAndList();
    ReAllocationVolts();
    PostMessage(WM_LIST_ITEM_CHANGED, NULL, NULL);
}


void CBacnetAnalogCusRang::OnEnKillfocusEditMaxValueValue()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    // TODO: Add control notification handler code here
    ReSetSlideAndList();
    ReAllocationValue();
    PostMessage(WM_LIST_ITEM_CHANGED, NULL, NULL);
}


void CBacnetAnalogCusRang::InitialPointCount()
{
    int n_count = 1;
    for (int i = 1; i < 11; i++)
    {
        if (max_unit < m_analog_custmer_range.at(analog_range_tbl_line).dat[i-1].m_value)
            max_unit = m_analog_custmer_range.at(analog_range_tbl_line).dat[i-1].m_value;
        if (min_unit > m_analog_custmer_range.at(analog_range_tbl_line).dat[i-1].m_value)
            min_unit = m_analog_custmer_range.at(analog_range_tbl_line).dat[i-1].m_value;

        if ((m_analog_custmer_range.at(analog_range_tbl_line).dat[i].m_volts != 0) /*&&
            (m_analog_custmer_range.at(analog_range_tbl_line).dat[i].m_value != 0)*/)
        {
            n_count++;
        }
        else
        {
            for (int j = i;j < 11;j++)
            {
                m_analog_cus_range_list.SetItemText(j, 0, _T(""));
                m_analog_cus_range_list.SetItemText(j, 1, _T(""));
            }
            break;
        }
    }
    n_point_count = n_count;
    bool ret_check = false;
    ret_check = CheckAllDataValid();
    if (!ret_check)
    {
        m_static_data_status.ShowWindow(SW_SHOW);
        m_static_data_status.SetWindowTextW(_T("Inflection point\r\n detected"));
    }
    else
    {
        m_static_data_status.ShowWindow(SW_HIDE);
    }
    CString temp_cs;
    temp_cs.Format(_T("%d"), n_count);
    GetDlgItem(IDC_EDITCUS_POINT_VALUE)->SetWindowTextW(temp_cs);

    CString first_volts;
    CString first_value;
    CString last_volts;
    CString last_value;
    if (enable_high_precision)
    {
        first_volts.Format(_T("%.2f"), ((float)m_analog_custmer_range.at(analog_range_tbl_line).dat[0].m_volts) / 100);
        last_volts.Format(_T("%.2f"), ((float)m_analog_custmer_range.at(analog_range_tbl_line).dat[n_point_count - 1].m_volts) / 100);
        first_value.Format(_T("%.2f"), ((float)m_analog_custmer_range.at(analog_range_tbl_line).dat[0].m_value) / 100);
        last_value.Format(_T("%.2f"), ((float)m_analog_custmer_range.at(analog_range_tbl_line).dat[n_point_count - 1].m_value) / 100);
    }
    else
    {
        first_volts.Format(_T("%.1f"), ((float)m_analog_custmer_range.at(analog_range_tbl_line).dat[0].m_volts) / 10);
        last_volts.Format(_T("%.1f"), ((float)m_analog_custmer_range.at(analog_range_tbl_line).dat[n_point_count - 1].m_volts) / 10);
        first_value.Format(_T("%d"), m_analog_custmer_range.at(analog_range_tbl_line).dat[0].m_value);
        last_value.Format(_T("%d"), m_analog_custmer_range.at(analog_range_tbl_line).dat[n_point_count - 1].m_value);
    }
        


    GetDlgItem(IDC_EDIT_MIN_VOLT_VALUE)->SetWindowTextW(first_volts);
    GetDlgItem(IDC_EDIT_MIN_VALUE_VALUE)->SetWindowTextW(first_value);
    GetDlgItem(IDC_EDIT_MAX_VOLT_VALUE3)->SetWindowTextW(last_volts);
    GetDlgItem(IDC_EDIT_MAX_VALUE_VALUE)->SetWindowTextW(last_value);

    ShowWindowByCount(n_count);
}


void CBacnetAnalogCusRang::ShowWindowByCount(int n_count)
{
    if (n_count > 11)
        n_count = 11;
    m_cus_analog_range_ctrl_0.ShowWindow(false);
    m_cus_analog_range_ctrl_1.ShowWindow(false);
    m_cus_analog_range_ctrl_2.ShowWindow(false);
    m_cus_analog_range_ctrl_3.ShowWindow(false);
    m_cus_analog_range_ctrl_4.ShowWindow(false);
    m_cus_analog_range_ctrl_5.ShowWindow(false);
    m_cus_analog_range_ctrl_6.ShowWindow(false);
    m_cus_analog_range_ctrl_7.ShowWindow(false);
    m_cus_analog_range_ctrl_8.ShowWindow(false);
    m_cus_analog_range_ctrl_9.ShowWindow(false);
    m_cus_analog_range_ctrl_10.ShowWindow(false);
    GetDlgItem(IDC_STATIC_VALUE_1)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VALUE_2)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VALUE_3)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VALUE_4)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VALUE_5)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VALUE_6)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VALUE_7)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VALUE_8)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VALUE_9)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VALUE_10)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VALUE_11)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VC_1)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VC_2)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VC_3)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VC_4)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VC_5)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VC_6)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VC_7)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VC_8)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VC_9)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VC_10)->ShowWindow(false);
    GetDlgItem(IDC_STATIC_VC_11)->ShowWindow(false);


    for (int i = 0; i < 11; i++)
    {
        m_analog_cus_range_list.SetCellEnabled(i, 0, true);
        m_analog_cus_range_list.SetCellEnabled(i, 1, false);
    }

    if (n_count >= 11)
    {
        m_cus_analog_range_ctrl_10.ShowWindow(true);
        GetDlgItem(IDC_STATIC_VALUE_11)->ShowWindow(true);
        GetDlgItem(IDC_STATIC_VC_11)->ShowWindow(true);
       // m_analog_cus_range_list.SetCellEnabled(10, 0, true);
        m_analog_cus_range_list.SetCellEnabled(10, 1, true);
    }
    if (n_count >= 10)
    {
        m_cus_analog_range_ctrl_9.ShowWindow(true);
        GetDlgItem(IDC_STATIC_VALUE_10)->ShowWindow(true);
        GetDlgItem(IDC_STATIC_VC_10)->ShowWindow(true);
       // m_analog_cus_range_list.SetCellEnabled(9, 0, true);
        m_analog_cus_range_list.SetCellEnabled(9, 1, true);
    }

    if (n_count >= 9)
    {
        m_cus_analog_range_ctrl_8.ShowWindow(true);
        GetDlgItem(IDC_STATIC_VALUE_9)->ShowWindow(true);
        GetDlgItem(IDC_STATIC_VC_9)->ShowWindow(true);
       // m_analog_cus_range_list.SetCellEnabled(8, 0, true);
        m_analog_cus_range_list.SetCellEnabled(8, 1, true);
    }

    if (n_count >= 8)
    {
        m_cus_analog_range_ctrl_7.ShowWindow(true);
        GetDlgItem(IDC_STATIC_VALUE_8)->ShowWindow(true);
        GetDlgItem(IDC_STATIC_VC_8)->ShowWindow(true);
       // m_analog_cus_range_list.SetCellEnabled(7, 0, true);
        m_analog_cus_range_list.SetCellEnabled(7, 1, true);
    }

    if (n_count >= 7)
    {
        m_cus_analog_range_ctrl_6.ShowWindow(true);
        GetDlgItem(IDC_STATIC_VALUE_7)->ShowWindow(true);
        GetDlgItem(IDC_STATIC_VC_7)->ShowWindow(true);
       // m_analog_cus_range_list.SetCellEnabled(6, 0, true);
        m_analog_cus_range_list.SetCellEnabled(6, 1, true);
    }

    if (n_count >= 6)
    {
        m_cus_analog_range_ctrl_5.ShowWindow(true);
        GetDlgItem(IDC_STATIC_VALUE_6)->ShowWindow(true);
        GetDlgItem(IDC_STATIC_VC_6)->ShowWindow(true);
       // m_analog_cus_range_list.SetCellEnabled(5, 0, true);
        m_analog_cus_range_list.SetCellEnabled(5, 1, true);
    }

    if (n_count >= 5)
    {
        m_cus_analog_range_ctrl_4.ShowWindow(true);
        GetDlgItem(IDC_STATIC_VALUE_5)->ShowWindow(true);
        GetDlgItem(IDC_STATIC_VC_5)->ShowWindow(true);
       // m_analog_cus_range_list.SetCellEnabled(4, 0, true);
        m_analog_cus_range_list.SetCellEnabled(4, 1, true);
    }

    if (n_count >= 4)
    {
        m_cus_analog_range_ctrl_3.ShowWindow(true);
        GetDlgItem(IDC_STATIC_VALUE_4)->ShowWindow(true);
        GetDlgItem(IDC_STATIC_VC_4)->ShowWindow(true);
       // m_analog_cus_range_list.SetCellEnabled(3, 0, true);
        m_analog_cus_range_list.SetCellEnabled(3, 1, true);
    }

    if (n_count >= 3)
    {
        m_cus_analog_range_ctrl_2.ShowWindow(true);
        GetDlgItem(IDC_STATIC_VALUE_3)->ShowWindow(true);
        GetDlgItem(IDC_STATIC_VC_3)->ShowWindow(true);
       // m_analog_cus_range_list.SetCellEnabled(2, 0, true);
        m_analog_cus_range_list.SetCellEnabled(2, 1, true);
    }

    if (n_count >= 2)
    {
        m_cus_analog_range_ctrl_1.ShowWindow(true);
        GetDlgItem(IDC_STATIC_VALUE_2)->ShowWindow(true);
        GetDlgItem(IDC_STATIC_VC_2)->ShowWindow(true);
       // m_analog_cus_range_list.SetCellEnabled(1, 0, true);
        m_analog_cus_range_list.SetCellEnabled(1, 1, true);
    }

    if (n_count >= 1)
    {
        m_cus_analog_range_ctrl_0.ShowWindow(true);
        GetDlgItem(IDC_STATIC_VALUE_1)->ShowWindow(true);
        GetDlgItem(IDC_STATIC_VC_1)->ShowWindow(true);
       // m_analog_cus_range_list.SetCellEnabled(0, 0, true);
        m_analog_cus_range_list.SetCellEnabled(0, 1, true);
    }
}

void CBacnetAnalogCusRang::ReSetSlideAndList()
{
    CString temp_point_value;
    CString temp_min_volts;
    CString temp_min_value;
    CString temp_max_volts;
    CString temp_max_value;

    int n_min_volts;
    int n_min_value;
    int n_max_volts;
    int n_max_value;

    ((CEdit *)GetDlgItem(IDC_EDITCUS_POINT_VALUE))->GetWindowTextW(temp_point_value);
    ((CEdit *)GetDlgItem(IDC_EDIT_MIN_VOLT_VALUE))->GetWindowTextW(temp_min_volts);
    ((CEdit *)GetDlgItem(IDC_EDIT_MIN_VALUE_VALUE))->GetWindowTextW(temp_min_value);
    ((CEdit *)GetDlgItem(IDC_EDIT_MAX_VOLT_VALUE3))->GetWindowTextW(temp_max_volts);
    ((CEdit *)GetDlgItem(IDC_EDIT_MAX_VALUE_VALUE))->GetWindowTextW(temp_max_value);

    n_point_count = _wtoi(temp_point_value);
    if (n_point_count > 11)
        n_point_count = 11;
    if (enable_high_precision)
    {
        n_min_volts = _wtof(temp_min_volts) * 100;
        n_min_value = _wtof(temp_min_value) * 100;;
        n_max_volts = _wtof(temp_max_volts) * 100;
        n_max_value = _wtof(temp_max_value) * 100;
    }
    else
    {
        n_min_volts = _wtof(temp_min_volts) * 10;
        n_min_value = _wtof(temp_min_value);
        n_max_volts = _wtof(temp_max_volts) * 10;
        n_max_value = _wtof(temp_max_value);
    }



    m_analog_custmer_range.at(analog_range_tbl_line).dat[0].m_volts = n_min_volts;
    m_analog_custmer_range.at(analog_range_tbl_line).dat[0].m_value = n_min_value;

    m_analog_custmer_range.at(analog_range_tbl_line).dat[n_point_count - 1].m_volts = n_max_volts;
    m_analog_custmer_range.at(analog_range_tbl_line).dat[n_point_count - 1].m_value = n_max_value;

    max_unit = n_max_value;
    min_unit = n_min_value;

    max_voltage = n_max_volts;
    min_voltage = n_min_volts;

    ShowWindowByCount(n_point_count);
    Fresh_AnalogCusRange_List(NULL, NULL);
    bool ret_check = false;
    ret_check = CheckAllDataValid();
    if (!ret_check)
    {
        m_static_data_status.ShowWindow(SW_SHOW);
        m_static_data_status.SetWindowTextW(_T("Inflection point\r\n detected"));
    }
    else
    {
        m_static_data_status.ShowWindow(SW_HIDE);
    }
    SetSlideRange();
}


void CBacnetAnalogCusRang::ReAllocationValue()
{
    if (n_point_count > 2)
    {
        int delta_value = 0;
        delta_value = (max_unit - min_unit) / (n_point_count - 1);


        for (int i = 0; i < n_point_count; i++)
        {
            m_analog_custmer_range.at(analog_range_tbl_line).dat[i].m_value = min_unit + delta_value*i;
        }
    }
}

void CBacnetAnalogCusRang::ReAllocationVolts()
{
    if (n_point_count > 2)
    {
        int delta_volt = 0;
        delta_volt = (max_voltage - min_voltage) / (n_point_count - 1);


        for (int i = 0; i < n_point_count; i++)
        {
            m_analog_custmer_range.at(analog_range_tbl_line).dat[i].m_volts = min_voltage + delta_volt*i;
        }
    }
}


void CBacnetAnalogCusRang::Handle_Static_ctrl_0()
{
    int CurPos;
    int temp_value;
    //float fvalue; 0oi-i--9999999999
    CRect temp_1;
    CRect temp_2;

    int ret_top = 0;
    CurPos = m_cus_analog_range_ctrl_0.GetPos();

    temp_value = max_unit + min_unit - CurPos;
    CString strtext;
    if (enable_high_precision == 1)
    {
        strtext.Format(_T("%.2f"), (float)temp_value / 100);
    }
    else
        strtext.Format(_T("%d"), temp_value);
    m_tipvalue = temp_value;
    if(n_point_count>=1)
        m_analog_cus_range_list.SetItemText(0, 1, strtext);
    m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_1));
    m_tooltips.Update();
    m_cus_analog_range_ctrl_0.GetWindowRect(temp_1);
    GetDlgItem(IDC_STATIC_VALUE_1)->GetWindowRect(temp_2);
    ScreenToClient(temp_1);
    ScreenToClient(temp_2);
    float n_persent = ((float)(CurPos - min_unit)) / (max_unit - min_unit);
    ret_top = temp_1.top + (320) * n_persent;
    GetDlgItem(IDC_STATIC_VALUE_1)->SetWindowPos(NULL, temp_2.left, ret_top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    GetDlgItem(IDC_STATIC_VALUE_1)->SetWindowText(strtext);
}

void CBacnetAnalogCusRang::Handle_Static_ctrl_1()
{
    int CurPos;
    int temp_value;
    CRect temp_1;
    CRect temp_2;
    int ret_top = 0;
    CurPos = m_cus_analog_range_ctrl_1.GetPos();
    temp_value = max_unit + min_unit - CurPos;
    CString strtext;
    if (enable_high_precision == 1)
    {
        strtext.Format(_T("%.2f"), (float)temp_value / 100);
    }
    else
        strtext.Format(_T("%d"), temp_value);
    m_tipvalue = temp_value;
    if (n_point_count >= 2)
        m_analog_cus_range_list.SetItemText(1, 1, strtext);
    m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_2));
    m_tooltips.Update();
    m_cus_analog_range_ctrl_1.GetWindowRect(temp_1);
    GetDlgItem(IDC_STATIC_VALUE_2)->GetWindowRect(temp_2);
    ScreenToClient(temp_1);
    ScreenToClient(temp_2);
    float n_persent = ((float)(CurPos - min_unit)) / (max_unit - min_unit);
    ret_top = temp_1.top + (320) * n_persent;
    GetDlgItem(IDC_STATIC_VALUE_2)->SetWindowPos(NULL, temp_2.left, ret_top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    GetDlgItem(IDC_STATIC_VALUE_2)->SetWindowText(strtext);
}

void CBacnetAnalogCusRang::Handle_Static_ctrl_2()
{
    int CurPos;
    int temp_value;
    CRect temp_1;
    CRect temp_2;
    int ret_top = 0;
    CurPos = m_cus_analog_range_ctrl_2.GetPos();
    temp_value = max_unit + min_unit - CurPos;
    CString strtext;
    if (enable_high_precision == 1)
    {
        strtext.Format(_T("%.2f"), (float)temp_value / 100);
    }
    else
        strtext.Format(_T("%d"), temp_value);
    m_tipvalue = temp_value;
    if (n_point_count >= 3)
    m_analog_cus_range_list.SetItemText(2, 1, strtext);
    m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_3));
    m_tooltips.Update();
    m_cus_analog_range_ctrl_2.GetWindowRect(temp_1);
    GetDlgItem(IDC_STATIC_VALUE_3)->GetWindowRect(temp_2);
    ScreenToClient(temp_1);
    ScreenToClient(temp_2);
    float n_persent = ((float)(CurPos - min_unit)) / (max_unit - min_unit);
    ret_top = temp_1.top + (320) * n_persent;
    GetDlgItem(IDC_STATIC_VALUE_3)->SetWindowPos(NULL, temp_2.left, ret_top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    GetDlgItem(IDC_STATIC_VALUE_3)->SetWindowText(strtext);
}

void CBacnetAnalogCusRang::Handle_Static_ctrl_3()
{
    int CurPos;
    int temp_value;
    CRect temp_1;
    CRect temp_2;
    int ret_top = 0;
    CurPos = m_cus_analog_range_ctrl_3.GetPos();
    temp_value = max_unit + min_unit - CurPos;
    CString strtext;
    if (enable_high_precision == 1)
    {
        strtext.Format(_T("%.2f"), (float)temp_value / 100);
    }
    else
        strtext.Format(_T("%d"), temp_value);
    m_tipvalue = temp_value;
    if (n_point_count >= 4)
    m_analog_cus_range_list.SetItemText(3, 1, strtext);
    m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_4));
    m_tooltips.Update();
    m_cus_analog_range_ctrl_3.GetWindowRect(temp_1);
    GetDlgItem(IDC_STATIC_VALUE_4)->GetWindowRect(temp_2);
    ScreenToClient(temp_1);
    ScreenToClient(temp_2);
    float n_persent = ((float)(CurPos - min_unit)) / (max_unit - min_unit);
    ret_top = temp_1.top + (320) * n_persent;
    GetDlgItem(IDC_STATIC_VALUE_4)->SetWindowPos(NULL, temp_2.left, ret_top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    GetDlgItem(IDC_STATIC_VALUE_4)->SetWindowText(strtext);
}

void CBacnetAnalogCusRang::Handle_Static_ctrl_4()
{
    int CurPos;
    int temp_value;
    CRect temp_1;
    CRect temp_2;
    int ret_top = 0;
    CurPos = m_cus_analog_range_ctrl_4.GetPos();
    temp_value = max_unit + min_unit - CurPos;
    CString strtext;
    if (enable_high_precision == 1)
    {
        strtext.Format(_T("%.2f"), (float)temp_value / 100);
    }
    else
        strtext.Format(_T("%d"), temp_value);
    m_tipvalue = temp_value;
    if (n_point_count >= 5)
    m_analog_cus_range_list.SetItemText(4, 1, strtext);
    m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_5));
    m_tooltips.Update();
    m_cus_analog_range_ctrl_4.GetWindowRect(temp_1);
    GetDlgItem(IDC_STATIC_VALUE_5)->GetWindowRect(temp_2);
    ScreenToClient(temp_1);
    ScreenToClient(temp_2);
    float n_persent = ((float)(CurPos - min_unit)) / (max_unit - min_unit);
    ret_top = temp_1.top + (320) * n_persent;
    GetDlgItem(IDC_STATIC_VALUE_5)->SetWindowPos(NULL, temp_2.left, ret_top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    GetDlgItem(IDC_STATIC_VALUE_5)->SetWindowText(strtext);
}

void CBacnetAnalogCusRang::Handle_Static_ctrl_5()
{
    int CurPos;
    int temp_value;
    CRect temp_1;
    CRect temp_2;
    int ret_top = 0;
    CurPos = m_cus_analog_range_ctrl_5.GetPos();
    temp_value = max_unit + min_unit - CurPos;
    CString strtext;
    if (enable_high_precision == 1)
    {
        strtext.Format(_T("%.2f"), (float)temp_value / 100);
    }
    else
        strtext.Format(_T("%d"), temp_value);
    m_tipvalue = temp_value;
    if (n_point_count >= 6)
    m_analog_cus_range_list.SetItemText(5, 1, strtext);
    m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_6));
    m_tooltips.Update();
    m_cus_analog_range_ctrl_5.GetWindowRect(temp_1);
    GetDlgItem(IDC_STATIC_VALUE_6)->GetWindowRect(temp_2);
    ScreenToClient(temp_1);
    ScreenToClient(temp_2);
    float n_persent = ((float)(CurPos - min_unit)) / (max_unit - min_unit);
    ret_top = temp_1.top + (320) * n_persent;
    GetDlgItem(IDC_STATIC_VALUE_6)->SetWindowPos(NULL, temp_2.left, ret_top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    GetDlgItem(IDC_STATIC_VALUE_6)->SetWindowText(strtext);
}

void CBacnetAnalogCusRang::Handle_Static_ctrl_6()
{
    int CurPos;
    int temp_value;
    CRect temp_1;
    CRect temp_2;
    int ret_top = 0;
    CurPos = m_cus_analog_range_ctrl_6.GetPos();
    temp_value = max_unit + min_unit - CurPos;
    CString strtext;
    if (enable_high_precision == 1)
    {
        strtext.Format(_T("%.2f"), (float)temp_value / 100);
    }
    else
        strtext.Format(_T("%d"), temp_value);
    m_tipvalue = temp_value;
    if (n_point_count >= 7)
    m_analog_cus_range_list.SetItemText(6, 1, strtext);
    m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_7));
    m_tooltips.Update();
    m_cus_analog_range_ctrl_6.GetWindowRect(temp_1);
    GetDlgItem(IDC_STATIC_VALUE_7)->GetWindowRect(temp_2);
    ScreenToClient(temp_1);
    ScreenToClient(temp_2);
    float n_persent = ((float)(CurPos - min_unit)) / (max_unit - min_unit);
    ret_top = temp_1.top + (320) * n_persent;
    GetDlgItem(IDC_STATIC_VALUE_7)->SetWindowPos(NULL, temp_2.left, ret_top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    GetDlgItem(IDC_STATIC_VALUE_7)->SetWindowText(strtext);
}

void CBacnetAnalogCusRang::Handle_Static_ctrl_7()
{
    int CurPos;
    int temp_value;
    CRect temp_1;
    CRect temp_2;
    int ret_top = 0;
    CurPos = m_cus_analog_range_ctrl_7.GetPos();
    temp_value = max_unit + min_unit - CurPos;
    CString strtext;
    if (enable_high_precision == 1)
    {
        strtext.Format(_T("%.2f"), (float)temp_value / 100);
    }
    else
        strtext.Format(_T("%d"), temp_value);
    m_tipvalue = temp_value;
    if (n_point_count >= 8)
    m_analog_cus_range_list.SetItemText(7, 1, strtext);
    m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_8));
    m_tooltips.Update();
    m_cus_analog_range_ctrl_7.GetWindowRect(temp_1);
    GetDlgItem(IDC_STATIC_VALUE_8)->GetWindowRect(temp_2);
    ScreenToClient(temp_1);
    ScreenToClient(temp_2);
    float n_persent = ((float)(CurPos - min_unit)) / (max_unit - min_unit);
    ret_top = temp_1.top + (320) * n_persent;
    GetDlgItem(IDC_STATIC_VALUE_8)->SetWindowPos(NULL, temp_2.left, ret_top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    GetDlgItem(IDC_STATIC_VALUE_8)->SetWindowText(strtext);
}

void CBacnetAnalogCusRang::Handle_Static_ctrl_8()
{
    int CurPos;
    int temp_value;
    CRect temp_1;
    CRect temp_2;
    int ret_top = 0;
    CurPos = m_cus_analog_range_ctrl_8.GetPos();
    temp_value = max_unit + min_unit - CurPos;
    CString strtext;
    if (enable_high_precision == 1)
    {
        strtext.Format(_T("%.2f"), (float)temp_value / 100);
    }
    else
        strtext.Format(_T("%d"), temp_value);
    m_tipvalue = temp_value;
    if (n_point_count >= 9)
    m_analog_cus_range_list.SetItemText(8, 1, strtext);
    m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_9));
    m_tooltips.Update();
    m_cus_analog_range_ctrl_8.GetWindowRect(temp_1);
    GetDlgItem(IDC_STATIC_VALUE_9)->GetWindowRect(temp_2);
    ScreenToClient(temp_1);
    ScreenToClient(temp_2);
    float n_persent = ((float)(CurPos - min_unit)) / (max_unit - min_unit);
    ret_top = temp_1.top + (320) * n_persent;
    GetDlgItem(IDC_STATIC_VALUE_9)->SetWindowPos(NULL, temp_2.left, ret_top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    GetDlgItem(IDC_STATIC_VALUE_9)->SetWindowText(strtext);
}

void CBacnetAnalogCusRang::Handle_Static_ctrl_9()
{
    int CurPos;
    int temp_value;
    CRect temp_1;
    CRect temp_2;
    int ret_top = 0;
    CurPos = m_cus_analog_range_ctrl_9.GetPos();
    temp_value = max_unit + min_unit - CurPos;
    CString strtext;
    if (enable_high_precision == 1)
    {
        strtext.Format(_T("%.2f"), (float)temp_value / 100);
    }
    else
        strtext.Format(_T("%d"), temp_value);
    m_tipvalue = temp_value;
    if (n_point_count >= 10)
    m_analog_cus_range_list.SetItemText(9, 1, strtext);
    m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_10));
    m_tooltips.Update();
    m_cus_analog_range_ctrl_9.GetWindowRect(temp_1);
    GetDlgItem(IDC_STATIC_VALUE_10)->GetWindowRect(temp_2);
    ScreenToClient(temp_1);
    ScreenToClient(temp_2);
    float n_persent = ((float)(CurPos - min_unit)) / (max_unit - min_unit);
    ret_top = temp_1.top + (320) * n_persent;
    GetDlgItem(IDC_STATIC_VALUE_10)->SetWindowPos(NULL, temp_2.left, ret_top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    GetDlgItem(IDC_STATIC_VALUE_10)->SetWindowText(strtext);
}

void CBacnetAnalogCusRang::Handle_Static_ctrl_10()
{
    int CurPos;
    int temp_value;
    CRect temp_1;
    CRect temp_2;
    int ret_top = 0;
    CurPos = m_cus_analog_range_ctrl_10.GetPos();
    temp_value = max_unit + min_unit - CurPos;
    CString strtext;
    if (enable_high_precision == 1)
    {
        strtext.Format(_T("%.2f"), (float)temp_value / 100);
    }
    else
        strtext.Format(_T("%d"), temp_value);
    m_tipvalue = temp_value;
    if (n_point_count >= 11)
    m_analog_cus_range_list.SetItemText(10, 1, strtext);
    m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_11));
    m_tooltips.Update();
    m_cus_analog_range_ctrl_10.GetWindowRect(temp_1);
    GetDlgItem(IDC_STATIC_VALUE_11)->GetWindowRect(temp_2);
    ScreenToClient(temp_1);
    ScreenToClient(temp_2);
    float n_persent = ((float)(CurPos - min_unit)) / (max_unit - min_unit);
    ret_top = temp_1.top + (320) * n_persent;
    GetDlgItem(IDC_STATIC_VALUE_11)->SetWindowPos(NULL, temp_2.left, ret_top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    GetDlgItem(IDC_STATIC_VALUE_11)->SetWindowText(strtext);
}


void CBacnetAnalogCusRang::OnBnClickedButtonApply()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    // TODO: Add control notification handler code here
}


void CBacnetAnalogCusRang::OnCbnKillfocusComboCusrangeStignaltype()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    // TODO: Add control notification handler code here
    CString temp_string;
    int n_value = 0xff;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_CUSRANGE_STIGNALTYPE))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_CUSRANGE_STIGNALTYPE))->GetLBText(nSel, temp_string);
    for (int i = 0;i < sizeof(JumperStatus) / sizeof(JumperStatus[0]);i++)
    {
        if (temp_string.CompareNoCase(JumperStatus[i]) == 0)
        {
            n_value = i;
            break;
        }
    }

    unsigned char  temp_value = 0;
    temp_value = n_value;
   
    unsigned char temp1;
    temp1 = m_dialog_signal_type;
    temp1 = temp1 & 0x0f;
    temp1 = temp1 | (temp_value << 4);
    m_dialog_signal_type = temp1;


    Initial_List();
    InitialPointCount();
    Fresh_AnalogCusRange_List(analog_range_tbl_line, analog_range_tbl_line);
}


void CBacnetAnalogCusRang::OnNMClickDialogBacnetRangeList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    // TODO: Add control notification handler code here
    *pResult = 0;

    long lRow, lCol;
    m_analog_cus_range_list.Set_Edit(false);
    DWORD dwPos = GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
    CPoint point(GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos));
    m_analog_cus_range_list.ScreenToClient(&point);
    LVHITTESTINFO lvinfo;
    lvinfo.pt = point;
    lvinfo.flags = LVHT_ABOVE;
    int nItem = m_analog_cus_range_list.SubItemHitTest(&lvinfo);

    lRow = lvinfo.iItem;
    lCol = lvinfo.iSubItem;

    // If the clicked area exceeds the maximum row number, the click is invalid
    if (lRow >= n_point_count) //����������������кţ���������Ч��
        return;

    m_analog_cus_range_list.Set_Edit(true);
}


void CBacnetAnalogCusRang::OnCbnSelchangeComboPrecision()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    // TODO: Add control notification handler code here
    CString temp_string;
    int n_value = 0;
    int nSel = ((CComboBox*)GetDlgItem(IDC_COMBO_PRECISION))->GetCurSel();
    ((CComboBox*)GetDlgItem(IDC_COMBO_PRECISION))->GetLBText(nSel, temp_string);
    for (int i = 0; i < sizeof(JumperStatus) / sizeof(JumperStatus[0]); i++)
    {
        if (temp_string.CompareNoCase(_T("0.01")) == 0)
        {
            enable_high_precision = 1;
            n_value = 0xef;
            break;
        }
    }
    if (n_value != 0xef)
        enable_high_precision = 0;
    m_analog_custmer_range.at(analog_range_tbl_line).table_name[8] = n_value;
    UpdateCusAnalogUnit();
    PostMessage(WM_REFRESH_BAC_ANALOGCUSRANGE_LIST, NULL, NULL);
}
