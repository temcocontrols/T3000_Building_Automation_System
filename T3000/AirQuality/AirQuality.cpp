// AirQuality.cpp : implementation file
//

#include "stdafx.h"
#include "../T3000.h"
#include "AirQuality.h"
//#include "../global_function.h"
#include "../MainFrm.h"
#include "../SQLiteDriver/CppSQLite3.h"


#include <bitset>
static BOOL ifAQdb = TRUE;
static BOOL writedb = TRUE;

CString Str_Display[4] = {_T("Scroll"),_T("Temperature"),_T("Humidity"),_T("AirQuanlity")};

#define  WM_AIRQUANITY_FRESH WM_USER + 98562
// CAirQuality


/// <summary>
/// This is a Thread to load data and fresh UI
/// Load data to cache ,send message to window.
/// </summary>
/// <param name="pParam"></param>
/// <returns></returns>
DWORD WINAPI CAirQuality_BackMainUIFresh(LPVOID pParam)
{
	CAirQuality* pdlg = (CAirQuality*)pParam;
	int heatbeat = 0;

    pdlg->hFirstThread = NULL;
    return 0;

	//return 0;
	//while (pdlg->IsWindowVisible())
	//{
		Sleep(2000);
		if (pdlg->IsWindowVisible())
		{
			if (is_connect())
			{
				if (pdlg->m_fresh_data)
				{
					if (!pdlg->m_fresh_data)
					{
						heatbeat ++;
						Sleep(6000);
						if (heatbeat > 10)
						{
							pdlg->m_fresh_data = FALSE;
							heatbeat = 0;
						}
                        goto end_aq_backthread;
					}
					int AddressValue = -1;
					if (pdlg->m_product_model == PM_HUMTEMPSENSOR)
					{
						AddressValue = 100;
					}
					else
					{
						AddressValue = pdlg->MODBUS_TEMPRATURE_CHIP_F;
					}

					Read_Multi(pdlg->m_current_TstatID, &product_register_value[AddressValue], AddressValue, 4, 5);


					if (!pdlg->m_fresh_data)
					{
						heatbeat ++;
						Sleep(6000);
						if (heatbeat > 10)
						{
							pdlg->m_fresh_data = FALSE;
							heatbeat = 0;
						}
                        goto end_aq_backthread;
					}


					if (pdlg->m_product_model == PM_HUMTEMPSENSOR)
					{
						AddressValue = 188;
					}
					else
					{
						AddressValue = pdlg->MODBUS_OUTPUT_RANGE;
					}

					Read_Multi(pdlg->m_current_TstatID, &product_register_value[AddressValue], AddressValue, 4, 5);


					if (!pdlg->m_fresh_data)
					{
                        goto end_aq_backthread;
					}

					if (pdlg->m_product_model == PM_HUMTEMPSENSOR)
					{
						AddressValue = 370;
					}
					else
					{
						AddressValue = pdlg->MODBUS_TEMP_CURRENT_OUTPUT;
					}


					Read_Multi(pdlg->m_current_TstatID, &product_register_value[AddressValue], AddressValue, 6, 5);


					::SendMessage(pdlg->m_hWnd,WM_AIRQUANITY_FRESH, 0, 0);
				}
			}
		}
	//}
end_aq_backthread:
    pdlg->hFirstThread = NULL;
	return 0;
}

IMPLEMENT_DYNCREATE(CAirQuality, CFormView)

CAirQuality::CAirQuality()
	: CFormView(CAirQuality::IDD)
{
	hFirstThread = NULL;
	m_fresh_data = TRUE;
}

CAirQuality::~CAirQuality()
{
}

void CAirQuality::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, m_edit_address);
	DDX_Control(pDX, IDC_EDIT_SN, m_edit_sn);
	DDX_Control(pDX, IDC_EDIT_BAUDRATE, m_edit_baudrate);
	DDX_Control(pDX, IDC_EDIT_FV, m_edit_fv);
	DDX_Control(pDX, IDC_EDIT_HV, m_edit_hv);
	DDX_Control(pDX, IDC_EDIT_MODEL, m_edit_model);
	DDX_Control(pDX, IDC_COMBO_DISPLAY, m_combox_displayer);
	DDX_Control(pDX, IDC_EDIT_BACKLIGHT_TIME_TIME, m_edit_backlightTime);
	DDX_Control(pDX, IDC_EDIT_BACKLIGHT_TIME, m_combox_backlightTime);
	DDX_Control(pDX, IDC_CHECK_USER, m_check_user);
	DDX_Control(pDX, IDC_CHECK_DEFAULT, m_check_default);
	DDX_Control(pDX, IDC_CHARTCTRL, m_ChartCtrl);
	DDX_Control(pDX,IDC_LIST_INPUT_AQ, m_input_list);
	DDX_Control(pDX,IDC_LIST_OUTPUT_AQ, m_output_list);
	DDX_Control(pDX,IDC_LIST_USER, m_user_list);
	DDX_Control(pDX,IDC_LIST_FACTORY, m_factory_list);
	DDX_Control(pDX, IDC_EDIT_CAL, m_edit_calibration);
	DDX_Control(pDX, IDC_EDIT_MAX_AQ, m_max_scale_aq_edit);
	DDX_Control(pDX, IDC_CO2_BAUDRATECOMBO, m_Combox_baudrate);
}

BEGIN_MESSAGE_MAP(CAirQuality, CFormView)
	ON_EN_KILLFOCUS(IDC_EDIT_MAX_AQ, &CAirQuality::OnEnKillfocusMaxScaleAQ)
	ON_EN_KILLFOCUS(IDC_EDIT_TSTAT_NAME,&CAirQuality::OnEnKillfocusChangeHumName)
	ON_EN_KILLFOCUS(IDC_EDIT_BACKLIGHT_TIME_TIME, &CAirQuality::OnEnKillfocustempBacklight)
	ON_CBN_SELCHANGE(IDC_EDIT_BACKLIGHT_TIME, &CAirQuality::OnEnChangeBacklight)
	ON_BN_CLICKED(IDC_CHANGE_ID, &CAirQuality::OnBnClickedButton_Change_ID)
	ON_BN_CLICKED(IDC_BUTTON_FACTORY_RESET, &CAirQuality::OnBnClickedButton_Factory_Reset)
	ON_BN_CLICKED(IDD_USER_TABLE_ADD, &CAirQuality::OnBnClickedButton_UserTable_Add)
	ON_BN_CLICKED(IDD_USER_TABLE_DEL, &CAirQuality::OnBnClickedButton_UserTable_Del)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPLAY, &CAirQuality::OnCbnSelchangeComboDisplay)
	ON_BN_CLICKED(IDC_CHECK_USER, &CAirQuality::OnBnClickedUser)
	ON_BN_CLICKED(IDC_CHECK_DEFAULT, &CAirQuality::OnBnClickedDefault)
	ON_MESSAGE(MY_RESUME_DATA, InputMessageCallBack_AQ)
	ON_MESSAGE(WM_REFRESH_BAC_INPUT_LIST,Fresh_Input_List_AQ)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Input_Item_AQ)
	ON_NOTIFY(NM_CLICK, IDC_LIST_INPUT_AQ, &CAirQuality::OnNMClickList1_AQ)
	ON_NOTIFY(NM_CLICK, IDC_LIST_OUTPUT_AQ, &CAirQuality::OnNMClickList2_AQ)
	ON_NOTIFY(NM_CLICK, IDC_LIST_USER, &CAirQuality::OnNMClickList_User) //OnNMClickList_Factory
	ON_WM_DESTROY()


	ON_BN_CLICKED(IDC_GRAPIC, &CAirQuality::OnBnClickedButton_Graphic)
	ON_BN_CLICKED(IDC_RADIO_TRANDUCER, &CAirQuality::OnBnClickedButton_Tranducer)
	ON_BN_CLICKED(IDC_RADIO_pid, &CAirQuality::OnBnClickedButton_PID)
	ON_CBN_SELENDCANCEL(IDC_CO2_BAUDRATECOMBO, &CAirQuality::OnCbnSelendcancelBaudratecombo)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CAirQuality, CDialog)
	//ON_EVENT(CAirQuality, IDC_MSFLEXGRID1, DISPID_CLICK, CAirQuality::ClickMsflexgrid1, VTS_NONE)
END_EVENTSINK_MAP()
// CAirQuality diagnostics

#ifdef _DEBUG
void CAirQuality::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAirQuality::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}


#endif
#endif //_DEBUG
// CAirQuality message handlers
void CAirQuality::Fresh()
{
	m_Combox_baudrate.ResetContent();
	m_Combox_baudrate.AddString(_T("9600"));
	m_Combox_baudrate.AddString(_T("19200"));
	m_Combox_baudrate.AddString(_T("38400"));
	m_Combox_baudrate.AddString(_T("57600"));
	m_Combox_baudrate.AddString(_T("115200"));

	g_NEED_MULTI_READ = FALSE;
	int AddressValue = -1;
	m_current_TstatID = g_tstat_id;
	if ((product_register_value[7] == PM_CO2_RS485 && product_register_value[14] == 6) || product_register_value[7] == PM_HUMTEMPSENSOR || product_register_value[7] == STM32_HUM_NET || product_register_value[7] == STM32_HUM_RS485)
	{
		m_product_model = PM_HUMTEMPSENSOR;
	}
	else
	{
		m_product_model = product_register_value[7];
	}


	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		AddressValue = 377;
	}
	else
	{
		AddressValue = 2000;
	}
	Read_Multi(m_current_TstatID, Default_Value, AddressValue, 30);
	Initial_RegisterList();
	ShowAirqualityDialog();

	if (product_register_value[714] == 0x56)
	{
		GetDlgItem(IDC_STATIC_NAME_TSTAT)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_TSTAT_NAME)->ShowWindow(TRUE);
		CString TstatName;
		TstatName.Format(_T("%s%s"), GetTextFromReg(715), GetTextFromReg(719));
		GetDlgItem(IDC_EDIT_TSTAT_NAME)->SetWindowText(TstatName);
	}
	else
	{
		GetDlgItem(IDC_STATIC_NAME_TSTAT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_TSTAT_NAME)->ShowWindow(SW_HIDE);
	}

	//Initial_UserList();
	//Initial_FactoryList();
	//ShowChecks();
	//InitChartData();
	CString strTemp;
	strTemp.Format(_T("%d"), product_register_value[9]);
    if (product_register_value[9] >= 30)   //hum version 大于等于30的 传感器不用校准;  老的传感器用的 27 以前的版本 ，旧的有校准表格;
    {
        GetDlgItem(IDC_GRAPIC)->EnableWindow(false);
    }
	GetDlgItem(IDC_EDIT_PIC_VERSION)->SetWindowText(strTemp);
	LoadRegistersGraphicMode();

	if (!g_fresh_Graphic)
	{
		// 		InitChartCtrl();
		// 		DrawerPoint();
		g_fresh_Graphic = TRUE;
	}
	Initial_InputList();
	Initial_OutputList();
	Initial_UserList();
	if (m_product_model == STM32_PRESSURE_NET)
	{
		GetDlgItem(IDC_RADIO_TRANDUCER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO_pid)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_RADIO_TRANDUCER)->ShowWindow(1);
		GetDlgItem(IDC_RADIO_pid)->ShowWindow(1);
	}
	if (product_register_value[530] == 0)
	{
		((CButton *)GetDlgItem(IDC_RADIO_TRANDUCER))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_RADIO_pid))->SetCheck(0);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RADIO_TRANDUCER))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO_pid))->SetCheck(1);
	}

	PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);


	hFirstThread = NULL;
	if (!hFirstThread)
	{
		hFirstThread = CreateThread(NULL,NULL, CAirQuality_BackMainUIFresh, this,NULL, 0);
        CloseHandle(hFirstThread);
        hFirstThread = NULL;
	}
}

void CAirQuality::OnCbnSelendcancelBaudratecombo()
{
	if (product_register_value[MODBUS_BAUDRATE] == m_Combox_baudrate.GetCurSel()) //Add this to judge weather this value need to change.
		return;

	/* Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_BAUDRATE,m_baudRateCombox.GetCurSel(),
	                     product_register_value[MODBUS_BAUDRATE],this->m_hWnd,IDC_BAUDRATECOMBO,_T("BAUDRATE"));*/
	int ret = 0;
	if (product_register_value[7] == STM32_PM25)
		ret = write_one(g_tstat_id, 15, m_Combox_baudrate.GetCurSel());
	else
	    ret = write_one(g_tstat_id, MODBUS_BAUDRATE, m_Combox_baudrate.GetCurSel());
	int index_brandrate = m_Combox_baudrate.GetCurSel();
	int brandrate = 19200;

	if (index_brandrate == 0)
	{
		brandrate = 9600;
	}
	else if (index_brandrate == 1)
	{
		brandrate = 19200;
	}
	else if (index_brandrate == 2)
	{
		brandrate = 38400;
	}
	else if (index_brandrate == 3)
	{
		brandrate = 57600;
	}
	else if (index_brandrate == 4)
	{
		brandrate = 115200;
	}
	CString SqlText;

	SqlText.Format(_T("update ALL_NODE set Baudrate = '%d' where Serial_ID='%d'"), brandrate, get_serialnumber());
	Change_BaudRate(brandrate);

	CppSQLite3DB SqliteDBBuilding;
	CppSQLite3Table table;
	CppSQLite3Query q;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
	SqliteDBBuilding.execDML((UTF8MBSTR)SqlText);
	SqliteDBBuilding.closedb();

	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	pFrame->ScanTstatInDB();
}

void CAirQuality::OnCbnSelchangeComboDisplay()
{
	int sel = m_combox_displayer.GetCurSel();

	if (product_register_value[MODBUS_SCROLL_MODE] == sel)
	{
		return;
	}
	int ret = write_one(m_current_TstatID, MODBUS_SCROLL_MODE, sel);
	if (ret > 0)
	{
		product_register_value[MODBUS_SCROLL_MODE] = sel;
	}
	else
	{
		AfxMessageBox(_T("Write Fail"));
	}

	m_combox_displayer.SetCurSel(product_register_value[MODBUS_SCROLL_MODE]);
}

void CAirQuality::OnCbnSelchangeComboBacklight()
{
}

void CAirQuality::ShowChecks()
{
	BOOL is_grad = TRUE;
	if (product_register_value[MODBUS_HUM_CURRENT_DEFAULT] == 1)
	{
		is_grad = FALSE;
	}
	if (is_grad)
	{
		m_check_default.SetCheck(TRUE);
		m_check_user.SetCheck(FALSE);
	}
	else
	{
		m_check_default.SetCheck(FALSE);
		m_check_user.SetCheck(TRUE);
	}

	Initial_UserList();
}

void CAirQuality::OnBnClickedUser()
{
	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		if (product_register_value[454] == 1)//Description is selected
		{
			ShowChecks();
			return;
		}


		int ret = write_one(m_current_TstatID, 454, 1);
		if (ret > 0)
		{
			product_register_value[454] = 1;
		}
		else
		{
			AfxMessageBox(_T("Setting Fail!"));
		}
		ShowChecks();
	}
	else
	{
		if (product_register_value[MODBUS_HUM_CURRENT_DEFAULT] == 1)//Description is selected
		{
			ShowChecks();
			return;
		}


		int ret = write_one(m_current_TstatID, MODBUS_HUM_CURRENT_DEFAULT, 1);
		if (ret > 0)
		{
			product_register_value[MODBUS_HUM_CURRENT_DEFAULT] = 1;
		}
		else
		{
			AfxMessageBox(_T("Setting Fail!"));
		}
		ShowChecks();
	}
}

void CAirQuality::OnBnClickedDefault()
{
	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		if (product_register_value[454] == 0)//Description is selected
		{
			ShowChecks();
			return;
		}


		int ret = write_one(m_current_TstatID, 454, 0);
		if (ret > 0)
		{
			product_register_value[454] = 0;
		}
		else
		{
			AfxMessageBox(_T("Setting Fail!"));
		}
		ShowChecks();
	}
	else
	{
		if (product_register_value[MODBUS_HUM_CURRENT_DEFAULT] == 0)//Description is selected
		{
			ShowChecks();
			return;
		}


		int ret = write_one(m_current_TstatID, MODBUS_HUM_CURRENT_DEFAULT, 0);
		if (ret > 0)
		{
			product_register_value[MODBUS_HUM_CURRENT_DEFAULT] = 0;
		}
		else
		{
			AfxMessageBox(_T("Setting Fail!"));
		}
		ShowChecks();
	}
}


void CAirQuality::OnDestroy()
{
	if (hFirstThread != NULL)
		TerminateThread(hFirstThread, 0);
	hFirstThread = NULL;
	CFormView::OnDestroy();
	
}

void CAirQuality::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
}

void CAirQuality::ShowAirqualityDialog()
{
	CString strTemp;
	int BacklightAddress = -1;
	strTemp.Format(_T("%d"), product_register_value[MODBUS_ADDRESS]);
	m_edit_address.SetWindowTextW(strTemp);
	int SN = product_register_value[MODBUS_SERIALNUMBER_HIWORD + 1] * 256 * 256 * 256 + product_register_value[MODBUS_SERIALNUMBER_HIWORD] * 256 * 256 + product_register_value[MODBUS_SERIALNUMBER_LOWORD + 1] * 256 + product_register_value[MODBUS_SERIALNUMBER_LOWORD];
	strTemp.Format(_T("%d"), SN);
	m_edit_sn.SetWindowTextW(strTemp);
	strTemp = GetProductName(product_register_value[7]);
	if (product_register_value[7] == STM32_PRESSURE_NET || product_register_value[7] == STM32_PRESSURE_RS3485)
	{
		if (product_register_value[700] == 0)
		{
			strTemp = L"PS100";
		}
		else if (product_register_value[700] == 1)
		{
			strTemp = L"PS230";
		}
		else if (product_register_value[700] == 10)
		{
			strTemp = L"PS8";
		}
		else if (product_register_value[700] == 20)
		{
			strTemp = L"PS1";
		}
	}
		
	m_edit_model.SetWindowTextW(strTemp);
	float fv = ((float)(short)product_register_value[MODBUS_VERSION_NUMBER_HI] * 256 + product_register_value[MODBUS_VERSION_NUMBER_LO]) / 10.0;
	strTemp.Format(_T("%0.1f"), fv);
	m_edit_fv.SetWindowTextW(strTemp);
	strTemp.Format(_T("%d"), product_register_value[MODBUS_HARDWARE_REV]);
	m_edit_hv.SetWindowTextW(strTemp);
	strTemp.Format(_T("%d"), product_register_value[366]);
	m_max_scale_aq_edit.SetWindowTextW(strTemp);
	if (product_register_value[7] != PM_AirQuality)
	{
		m_max_scale_aq_edit.ShowWindow(SW_HIDE);
	}
	else
	{
		m_max_scale_aq_edit.ShowWindow(SW_SHOW);
	}
	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		MODBUS_BAUDRATE = 185;
	}
	if (m_product_model == STM32_PRESSURE_NET)
	{
		MODBUS_BAUDRATE = 15;
	}
	if (product_register_value[7] == STM32_HUM_NET || product_register_value[7] == STM32_HUM_RS485)
	{
		MODBUS_BAUDRATE = 15;
	}

	// 	if (product_register_value[MODBUS_BAUDRATE]==0)
	// 	{
	// 		strTemp=_T("9600");
	// 
	// 	}
	// 	else
	// 	{
	// 		strTemp=_T("19200");
	// 	}
	// 	m_edit_baudrate.SetWindowTextW(strTemp);
	if (product_register_value[7] == STM32_PM25)
	{
		m_Combox_baudrate.SetCurSel(product_register_value[15]);
		m_combox_displayer.ShowWindow(0);
		GetDlgItem(IDC_LIST_INPUT_AQ)->ShowWindow(0);
		GetDlgItem(IDC_LIST_OUTPUT_AQ)->ShowWindow(0);
		GetDlgItem(IDC_LIST_USER)->ShowWindow(0);
	}
	else
		m_Combox_baudrate.SetCurSel(product_register_value[MODBUS_BAUDRATE]);

	m_combox_displayer.ResetContent();
	if (product_register_value[7] == PM_HUM_R)
	{
		for (int i = 0; i < 3; i++)
		{
			m_combox_displayer.AddString(Str_Display[i]);
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			m_combox_displayer.AddString(Str_Display[i]);
		}
	}

	if (product_register_value[MODBUS_SCROLL_MODE] >= 0 && product_register_value[MODBUS_SCROLL_MODE] < 4)
	{
		m_combox_displayer.SetCurSel(product_register_value[MODBUS_SCROLL_MODE]);
	}
	else
	{
		m_combox_displayer.SetCurSel(0);
	}
	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		strTemp.Format(_T("%d"), product_register_value[374]);
	}
	else
	{
		strTemp.Format(_T("%d"), product_register_value[MODBUS_HUMIDITY_FREQUENCY]);
	}


	GetDlgItem(IDC_EDIT_FREQUENCE)->SetWindowText(strTemp);
	if (m_product_model == STM32_PRESSURE_NET)
	{
		BacklightAddress = 836;
	}
	else
	{
		BacklightAddress = 361;
	}

	if (product_register_value[BacklightAddress] == 0)
	{
		//strTemp.Format(_T("%d"),(short)product_register_value[361]);

		m_edit_backlightTime.SetWindowTextW(_T("0"));
		m_combox_backlightTime.SetCurSel(0);
	}
	else if (product_register_value[BacklightAddress] == 255)
	{
		m_edit_backlightTime.SetWindowTextW(_T("255"));
		m_combox_backlightTime.SetCurSel(1);
	}
	else if (product_register_value[BacklightAddress] > 0 && product_register_value[BacklightAddress] < 255)
	{
		strTemp.Format(_T("%d"), (short)product_register_value[BacklightAddress]);
		m_edit_backlightTime.SetWindowTextW(strTemp);
		m_combox_backlightTime.SetCurSel(-1);
	}


	//IDC_STATIC_DISPLAY
	//IDC_STATIC_BLT
	//IDC_STATIC_BLS
	//IDC_EDIT_BACKLIGHT_TIME
	//IDC_EDIT_BACKLIGHT_TIME_TIME
	//IDC_STATIC_UNIT_TIME
	//IDC_COMBO_DISPLAY
	if (m_product_model == PM_HUMTEMPSENSOR || m_product_model == STM32_PRESSURE_NET)
	{
		GetDlgItem(IDC_STATIC_DISPLAY)->EnableWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_UNIT_TIME)->EnableWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_DISPLAY)->EnableWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_AQ)->EnableWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_MAX_AQ)->EnableWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_DISPLAY)->EnableWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BLT)->EnableWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_BLS)->EnableWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_BACKLIGHT_TIME)->EnableWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_BACKLIGHT_TIME_TIME)->EnableWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_UNIT_TIME)->EnableWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_DISPLAY)->EnableWindow(SW_SHOW);
	}
	if (product_register_value[7] == PM_AirQuality)
	{
		GetDlgItem(IDC_STATIC_AQ)->EnableWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_MAX_AQ)->EnableWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_STATIC_AQ)->EnableWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_MAX_AQ)->EnableWindow(SW_HIDE);
	}


}

void CAirQuality::Initial_UserList()
{
	//m_input_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	//m_input_list.InsertColumn(1, _T("Full Label"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	//m_input_list.InsertColumn(2, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	//m_input_list.InsertColumn(3, _T("Value"), 50, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	//m_input_list.InsertColumn(4, _T("Range"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	//m_input_list.InsertColumn(5, _T("Calibration"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

	CString strTemp;
	int AddressValue = 0;
	m_user_list.DeleteAllItems();
	while (m_user_list.DeleteColumn(0));


	m_user_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	m_user_list.SetExtendedStyle(m_user_list.GetExtendedStyle() | LVS_EX_GRIDLINES & (~LVS_EX_FULLROWSELECT));

	m_user_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_user_list.InsertColumn(1, _T("Full Label"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_user_list.InsertColumn(2, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_user_list.InsertColumn(3, _T("Value"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_user_list.InsertColumn(4, _T("Unit"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_user_list.InsertColumn(5, _T("Calibration"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);


	g_hwnd_now = this->m_hWnd;


	if (m_product_model == STM32_PRESSURE_NET)
	{
		return;
	}
	CString index;
	for (int i = 0; i < 4; i++)
	{
		index.Format(_T("%d"), i + 1);
		m_user_list.InsertItem(i, index);
	}
	int MODBUS_DEW_PT = 370; //unit.C
	int MODBUS_DEW_PT_F = MODBUS_DEW_PT + 1; //unit.F
	int MODBUS_PWS = MODBUS_DEW_PT + 2; //unit.hPa
	int MODBUS_MIX_RATIO = MODBUS_DEW_PT + 3; //unit.g/kg
	int MODBUS_ENTHALPY = MODBUS_DEW_PT + 4; //unit.kJ/kg

	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		MODBUS_DEW_PT = 482; //unit.C
		MODBUS_DEW_PT_F = MODBUS_DEW_PT + 1; //unit.F
		MODBUS_PWS = 488; //unit.hPa
		MODBUS_MIX_RATIO = 489; //unit.g/kg
		MODBUS_ENTHALPY = 490; //unit.kJ/kg
	}
	else
	{
		MODBUS_DEW_PT = 370; //unit.C
		MODBUS_DEW_PT_F = MODBUS_DEW_PT + 1; //unit.F
		MODBUS_PWS = MODBUS_DEW_PT + 2; //unit.hPa
		MODBUS_MIX_RATIO = MODBUS_DEW_PT + 3; //unit.g/kg
		MODBUS_ENTHALPY = MODBUS_DEW_PT + 4; //unit.kJ/kg
	}
	if (m_product_model == PM_HUMTEMPSENSOR || product_register_value[7] == PM_AirQuality || product_register_value[7] == PM_HUM_R)
	{
		m_user_list.SetItemText(0, 1,_T("Dew Point"));

		m_user_list.SetItemText(0, 2,NO_APPLICATION);


		if (product_register_value[121] == 0)
		{
			AddressValue = MODBUS_DEW_PT;
			strTemp.Format(_T("%0.1f"), ((float)((short)product_register_value[AddressValue])) / 10.0);
			m_user_list.SetItemText(0, 3, strTemp);
			m_user_list.SetItemText(0, 4,Global_String_C);
		}
		else
		{
			AddressValue = MODBUS_DEW_PT_F;
			strTemp.Format(_T("%0.1f"), ((float)((short)product_register_value[AddressValue])) / 10.0);
			m_user_list.SetItemText(0, 3, strTemp);
			m_user_list.SetItemText(0, 4,Glocal_String_F);
		}


		//strTemp.Format(_T("%d"),product_register_value[MODBUS_AQ_FILTER]);
		m_user_list.SetItemText(0, 5,NO_APPLICATION);

		//
		m_user_list.SetItemText(1, 1, L"Partial Pressure");
		m_user_list.SetItemText(1, 2,NO_APPLICATION);
		strTemp.Format(_T("%0.1f"), ((float)((short)product_register_value[MODBUS_PWS])) / 10.0);
		m_user_list.SetItemText(1, 3, strTemp);
		m_user_list.SetItemText(1, 4, L"hPa");
		m_user_list.SetItemText(1, 5,NO_APPLICATION);


		m_user_list.SetItemText(2, 1, L"Mixing Ratio");
		m_user_list.SetItemText(2, 2,NO_APPLICATION);
		strTemp.Format(_T("%0.1f"), ((float)((short)product_register_value[MODBUS_MIX_RATIO])) / 10.0);
		m_user_list.SetItemText(2, 3, strTemp);
		m_user_list.SetItemText(2, 4, L"g/Kg");
		m_user_list.SetItemText(2, 5,NO_APPLICATION);

		m_user_list.SetItemText(3, 1, L"Enthalpy");
		m_user_list.SetItemText(3, 2,NO_APPLICATION);
		strTemp.Format(_T("%0.1f"), ((float)((short)product_register_value[MODBUS_ENTHALPY])) / 10.0);
		m_user_list.SetItemText(3, 3, strTemp);
		m_user_list.SetItemText(3, 4, L"KJ/kg");
		m_user_list.SetItemText(3, 5,NO_APPLICATION);
	}
}

void CAirQuality::Initial_FactoryList()
{
	int rows = Default_Value[3];

	if (rows < 1 || m_product_model == PM_HUMTEMPSENSOR)
	{
		rows = 10;
	}

	m_factory_list.ShowWindow(SW_HIDE);
	m_factory_list.DeleteAllItems();
	while (m_factory_list.DeleteColumn(0));

	m_factory_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	m_factory_list.SetExtendedStyle(m_factory_list.GetExtendedStyle() | LVS_EX_GRIDLINES & (~LVS_EX_FULLROWSELECT));
	m_factory_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_factory_list.InsertColumn(1, _T("Frequency"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_factory_list.InsertColumn(2, _T("Humidity(%)"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	g_hwnd_now = this->m_hWnd;


	CString index;
	for (int i = 0; i < rows; i++)
	{
		index.Format(_T("%d"), i);
		m_factory_list.InsertItem(i, index);
	}

	CString strTemp;
	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		for (int row = 0; row < rows; row++)
		{
			strTemp.Format(_T("%d"), Default_Value[4 + 2 * row]);
			m_factory_list.SetItemText(row, 1, strTemp);


			strTemp.Format(_T("%0.1f"), ((float)((unsigned short)Default_Value[4 + 2 * row + 1])) / 10);
			m_factory_list.SetItemText(row, 2, strTemp);

			m_factory_list.SetCellEnabled(row, 0,FALSE);
			m_factory_list.SetCellEnabled(row, 1,FALSE);
			m_factory_list.SetItemBkColor(row, 0,DISABLE_COLOR_CELL);
			m_factory_list.SetItemBkColor(row, 1,DISABLE_COLOR_CELL);
			m_factory_list.SetItemBkColor(row, 2,DISABLE_COLOR_CELL);
		}
	}
	else
	{
		for (int row = 0; row < rows; row++)
		{
			strTemp.Format(_T("%d"), Default_Value[4 + 2 * row + 1]);

			m_factory_list.SetItemText(row, 1, strTemp);

			strTemp.Format(_T("%0.1f"), ((float)((unsigned short)Default_Value[4 + 2 * row])) / 10);

			m_factory_list.SetItemText(row, 2, strTemp);

			m_factory_list.SetCellEnabled(row, 0,FALSE);
			m_factory_list.SetCellEnabled(row, 1,FALSE);
			m_factory_list.SetItemBkColor(row, 0,DISABLE_COLOR_CELL);
			m_factory_list.SetItemBkColor(row, 1,DISABLE_COLOR_CELL);
			m_factory_list.SetItemBkColor(row, 2,DISABLE_COLOR_CELL);
		}
	}

	m_factory_list.ShowWindow(TRUE);
}


void CAirQuality::Initial_RegisterList()
{
#if 1
	MODBUS_SERIALNUMBER_LOWORD = 0;
	MODBUS_SERIALNUMBER_HIWORD = 2;
	MODBUS_VERSION_NUMBER_LO = 4;
	MODBUS_VERSION_NUMBER_HI = 5;
	MODBUS_ADDRESS = 6;
	MODBUS_PRODUCT_MODEL = 7;
	MODBUS_HARDWARE_REV = 8;
	MODBUS_PIC_VERSION = 9;
	MODBUS_HUM_PIC_VERSION = 10;
	MODBUS_ADDRESS_PLUG_N_PLAY = 11;
	MODBUS_BASE_ADDRESS = 15;
	MODBUS_UPDATE_STATUS = 16;
	MODBUS_UPDATE_PTR_HI = 17;
	MODBUS_UPDATE_PTR_LO = 18;
	MODBUS_SCRATCHPAD_ADDRESS = 19;
	MODBUS_TEMPRATURE_CHIP_F = 100;
	MODBUS_TEMPRATURE_CHIP = 101;
	MODBUS_HUM = 102;
	MODBUS_AQ = 103;
	MODBUS_CALIBRATION_TEMP = 104;
	MODBUS_CALIBRATION_AQ = 105;
	MODBUS_CALIBRATION_HUM = 106;
	MODBUS_TEMP_SELECT = 111;
	MODBUS_DAC_OFFSET = 112;
	MODBUS_DELAY = 113;
	MODBUS_SEQUENCE = 114;
	MODBUS_DEGC_OR_F = 121;
	MODBUS_POWERUP_MODE = 122;
	MODBUS_AUTO_ONLY = 123;
	MODBUS_OUTPUT_RANGE = 180;
	MODBUS_FACTORY_DEFAULTS = 181;
	MODBUS_EXTERNAL_SENSOR_0 = 182;
	MODBUS_EXTERNAL_SENSOR_1 = 183;
	MODBUS_INFO_BYTE = 184;
	MODBUS_BAUDRATE = 185;
	MODBUS_HUM_FILTER = 186;
	MODBUS_AQ_FILTER = 187;
	MODBUS_DIGITAL_IN1 = 188;
	MODBUS_CYCLING_DELAY = 190;
	MODBUS_CHANGOVER_DELAY = 191;
	MODBUS_OVERRIDE_TIMER = 192;
	MODBUS_OVERRIDE_TIMER_LEFT = 193;
	MODBUS_FILTER = 194;
	MODBUS_HEAT_COOL_CONFIG = 195;
	MODBUS_INTERNAL_TEMP_IC = 196;
	MODBUS_INTERNAL_THERMISTOR = 197;
	MODBUS_CALIBRATION_INTERNAL_THERMISTOR = 217;
	MODBUS_CALIBRATION_ANALOG_IN2 = 218;
	MODBUS_INPUT1_SELECT = 219;
	MODBUS_UNIVERSAL_NIGHTSET = 220;
	MODBUS_HEAT_ORIGINAL_TABLE = 221;
	MODBUS_COOL_ORIGINAL_TABLE = 222;
	MODBUS_PID2_MODE_OPERATION = 223;
	MODBUS_VALVE_TRAVEL_TIME = 224;
	MODBUS_VALVE_PERCENT = 225;
	MODBUS_INTERLOCK_OUTPUT1 = 226;
	MODBUS_INTERLOCK_OUTPUT2 = 227;
	MODBUS_INTERLOCK_OUTPUT3 = 228;
	MODBUS_INTERLOCK_OUTPUT4 = 229;
	MODBUS_INTERLOCK_OUTPUT5 = 230;
	MODBUS_INTERLOCK_OUTPUT6 = 231;
	MODBUS_INTERLOCK_OUTPUT7 = 232;
	MODBUS_SETPOINT_INCREASE = 233;
	MODBUS_FREEZE_TEMP_SETPOINT = 234;
	MODBUS_FREEZE_DELAY_ON = 235;
	MODBUS_FREEZE_DELAY_OFF = 236;
	MODBUS_ANALOG1_FUNCTION = 237;
	MODBUS_ANALOG2_FUNCTION = 238;
	MODBUS_DIGITAL1_FUNCTION = 239;
	MODBUS_DISPLAY_HUNDRED = 240;
	MODBUS_DISPLAY_TEN = 241;
	MODBUS_DISPLAY_DIGITAL = 242;
	MODBUS_DISPLAY_STATUS = 243;
	MODBUS_INPUT_MANU_ENABLE = 244;
	MODBUS_OUTPUT_MANU_ENABLE = 245;
	MODBUS_DIGITAL_INPUT = 246;
	MODBUS_DEAD_MASTER = 247;
	MODBUS_ROUND_DISPLAY = 248;
	MODBUS_MIN_ADDRESS = 249;
	MODBUS_MAX_ADDRESS = 250;
	MODBUS_FAN_GRIDPOINT = 251;
	MODBUS_MODE_GRIDPOINT = 252;
	MODBUS_HOLD_GRIDPOINT = 253;
	MODBUS_CONFIGURATION = 254;
	MODBUS_UTIL_MODE = 255;
	MODBUS_EEPROM_SIZE = 256;
	MODBUS_TIMER_SELECT = 257;
	MODBUS_OUTPUT1_FUNCTION = 258;
	MODBUS_ROTATION_OUT2 = 259;
	MODBUS_ROTATION_OUT3 = 260;
	MODBUS_ROTATION_OUT4 = 261;
	MODBUS_ROTATION_OUT5 = 262;
	MODBUS_OUTPUT2_FUNCTION = 263;
	MODBUS_OUTPUT3_FUNCTION = 264;
	MODBUS_OUTPUT4_FUNCTION = 265;
	MODBUS_OUTPUT5_FUNCTION = 266;
	MODBUS_PIR_STAGE = 267;
	MODBUS_AIR_CALIBRATION_POINT4 = 300;
	MODBUS_AIR_CALIBRATION_POINT1 = 301;
	MODBUS_AIR_CALIBRATION_POINT2 = 302;
	MODBUS_AIR_CALIBRATION_POINT3 = 303;
	MODUBS_HUMIDITY_RH = 304;
	MODBUS_HUMIDITY_FREQUENCY = 305;
	MODBUS_HUM_PIC_UPDATE = 306;
	MODBUS_HUM_CAL_NUM = 307;
	MODBUS_HUM_CAL_EREASE = 308;
	MODBUS_HUM_CURRENT_DEFAULT = 309;
	MODBUS_HUM_PICDATAOK = 310;
	MODBUS_HUM_OFFSETFLAG = 311;
	MODBUS_HUMCOUNT1_H = 312;
	MODBUS_HUMRH1_H = 313;
	MODBUS_HUMCOUNT2_H = 314;
	MODBUS_HUMRH2_H = 315;
	MODBUS_HUMCOUNT3_H = 316;
	MODBUS_HUMRH3_H = 317;
	MODBUS_HUMCOUNT4_H = 318;
	MODBUS_HUMRH4_H = 319;
	MODBUS_HUMCOUNT5_H = 320;
	MODBUS_HUMRH5_H = 321;
	MODBUS_HUMCOUNT6_H = 322;
	MODBUS_HUMRH6_H = 323;
	MODBUS_HUMCOUNT7_H = 324;
	MODBUS_HUMRH7_H = 325;
	MODBUS_HUMCOUNT8_H = 326;
	MODBUS_HUMRH8_H = 327;
	MODBUS_HUMCOUNT9_H = 328;
	MODBUS_HUMRH9_H = 329;
	MODBUS_HUMCOUNT10_H = 330;
	MODBUS_HUMRH10_H = 331;
	MODBUS_TEM_RANGE_LOWER = 332;
	MODBUS_TEM_RANGE_UPPER = 333;
	MODBUS_HUM_RANGE_LOWER = 334;
	MODBUS_HUM_RANGE_UPPER = 335;
	MODBUS_AQ_RANGE_LOWER = 336;
	MODBUS_AQ_RANGE_UPPER = 337;
	MODBUS_TEMP_CURRENT_OUTPUT = 338;
	MODBUS_HUM_CURRENT_OUTPUT = 339;
	MODBUS_AQ_CURRENT_OUTPUT = 340;
	MODBUS_TEMP_VOLTAGE_OUTPUT = 341;
	MODBUS_HUM_VOLTAGE_OUTPUT = 342;
	MODBUS_AQ_VOLTAGE_OUTPUT = 343;
	MODBUS_CALIBRATION_MODE = 344;
	MODBUS_SCROLL_MODE = 345;
	MODBUS_AQ_LEVEL0 = 346;
	MODBUS_AQ_LEVEL1 = 347;
	MODBUS_AQ_LEVEL2 = 348;
	MODBUS_OUTPUT_MODE = 349;
	MODBUS_TEMP_MANU_OUTPUT = 350;
	MODBUS_HUM_MANU_OUTPUT = 351;
	MODBUS_AQ_MANU_OUTPUT = 352;
	MODBUS_HUM_CALIBRATION_LOCKX = 353;
	MODBUS_HUM_CALIBRATION_LOCKY = 354;
#endif

	T3Register temp;

	CppSQLite3DB SqliteDBT3000;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
	CppSQLite3Query q;
#if 1
	if (!SqliteDBT3000.tableExists("AirQuanlity_Reglist"))
	{
		return;
	}
	CString SQL = _T("select * from AirQuanlity_Reglist");
	q = SqliteDBT3000.execQuery((UTF8MBSTR)SQL);

	while (!q.eof())
	{
		temp.regID = q.getIntField("RegID");

		temp.regName = q.getValuebyName(L"RegName");
		q.nextRow();
		m_vecT3Register.push_back(temp);
	}
	SqliteDBT3000.closedb();
#endif

	MODBUS_SERIALNUMBER_LOWORD = Get_RegID(_T("	MODBUS_SERIALNUMBER_LOWORD	"));
	MODBUS_SERIALNUMBER_HIWORD = Get_RegID(_T("	MODBUS_SERIALNUMBER_HIWORD	"));
	MODBUS_VERSION_NUMBER_LO = Get_RegID(_T("	MODBUS_VERSION_NUMBER_LO	"));
	MODBUS_VERSION_NUMBER_HI = Get_RegID(_T("	MODBUS_VERSION_NUMBER_HI	"));
	MODBUS_ADDRESS = Get_RegID(_T("	MODBUS_ADDRESS	"));
	MODBUS_PRODUCT_MODEL = Get_RegID(_T("	MODBUS_PRODUCT_MODEL	"));
	MODBUS_HARDWARE_REV = Get_RegID(_T("	MODBUS_HARDWARE_REV	"));
	MODBUS_PIC_VERSION = Get_RegID(_T("	MODBUS_PIC_VERSION	"));
	MODBUS_HUM_PIC_VERSION = Get_RegID(_T("	MODBUS_HUM_PIC_VERSION	"));
	MODBUS_ADDRESS_PLUG_N_PLAY = Get_RegID(_T("	MODBUS_ADDRESS_PLUG_N_PLAY	"));
	MODBUS_BASE_ADDRESS = Get_RegID(_T("	MODBUS_BASE_ADDRESS 	"));
	MODBUS_UPDATE_STATUS = Get_RegID(_T("	MODBUS_UPDATE_STATUS	"));
	MODBUS_UPDATE_PTR_HI = Get_RegID(_T("	MODBUS_UPDATE_PTR_HI	"));
	MODBUS_UPDATE_PTR_LO = Get_RegID(_T("	MODBUS_UPDATE_PTR_LO	"));
	MODBUS_SCRATCHPAD_ADDRESS = Get_RegID(_T("	MODBUS_SCRATCHPAD_ADDRESS	"));
	MODBUS_TEMPRATURE_CHIP_F = Get_RegID(_T("	MODBUS_TEMPRATURE_CHIP_F	"));
	MODBUS_TEMPRATURE_CHIP = Get_RegID(_T("	MODBUS_TEMPRATURE_CHIP 	"));
	MODBUS_HUM = Get_RegID(_T("	MODBUS_HUM            	"));
	MODBUS_AQ = Get_RegID(_T("	MODBUS_AQ            	"));
	MODBUS_CALIBRATION_TEMP = Get_RegID(_T("	MODBUS_CALIBRATION_TEMP    	"));
	MODBUS_CALIBRATION_AQ = Get_RegID(_T("	MODBUS_CALIBRATION_AQ 	"));
	MODBUS_CALIBRATION_HUM = Get_RegID(_T("	MODBUS_CALIBRATION_HUM 	"));
	MODBUS_TEMP_SELECT = Get_RegID(_T("	MODBUS_TEMP_SELECT	"));
	MODBUS_DAC_OFFSET = Get_RegID(_T("	MODBUS_DAC_OFFSET	"));
	MODBUS_DELAY = Get_RegID(_T("	MODBUS_DELAY	"));
	MODBUS_SEQUENCE = Get_RegID(_T("	MODBUS_SEQUENCE	"));
	MODBUS_DEGC_OR_F = Get_RegID(_T("	MODBUS_DEGC_OR_F 	"));
	MODBUS_POWERUP_MODE = Get_RegID(_T("	MODBUS_POWERUP_MODE  	"));
	MODBUS_AUTO_ONLY = Get_RegID(_T("	MODBUS_AUTO_ONLY 	"));
	MODBUS_OUTPUT_RANGE = Get_RegID(_T("	MODBUS_OUTPUT_RANGE	"));
	MODBUS_FACTORY_DEFAULTS = Get_RegID(_T("	MODBUS_FACTORY_DEFAULTS     	"));
	MODBUS_EXTERNAL_SENSOR_0 = Get_RegID(_T("	MODBUS_EXTERNAL_SENSOR_0	"));
	MODBUS_EXTERNAL_SENSOR_1 = Get_RegID(_T("	MODBUS_EXTERNAL_SENSOR_1	"));
	MODBUS_INFO_BYTE = Get_RegID(_T("	MODBUS_INFO_BYTE	"));
	MODBUS_BAUDRATE = Get_RegID(_T("	MODBUS_BAUDRATE    	"));
	MODBUS_HUM_FILTER = Get_RegID(_T("	MODBUS_HUM_FILTER	"));
	MODBUS_AQ_FILTER = Get_RegID(_T("	MODBUS_AQ_FILTER	"));
	MODBUS_DIGITAL_IN1 = Get_RegID(_T("	MODBUS_DIGITAL_IN1	"));
	MODBUS_CYCLING_DELAY = Get_RegID(_T("	MODBUS_CYCLING_DELAY 	"));
	MODBUS_CHANGOVER_DELAY = Get_RegID(_T("	MODBUS_CHANGOVER_DELAY	"));
	MODBUS_OVERRIDE_TIMER = Get_RegID(_T("	MODBUS_OVERRIDE_TIMER	"));
	MODBUS_OVERRIDE_TIMER_LEFT = Get_RegID(_T("	MODBUS_OVERRIDE_TIMER_LEFT	"));
	MODBUS_FILTER = Get_RegID(_T("	MODBUS_FILTER	"));
	MODBUS_HEAT_COOL_CONFIG = Get_RegID(_T("	MODBUS_HEAT_COOL_CONFIG	"));
	MODBUS_INTERNAL_TEMP_IC = Get_RegID(_T("	MODBUS_INTERNAL_TEMP_IC	"));
	MODBUS_INTERNAL_THERMISTOR = Get_RegID(_T("	MODBUS_INTERNAL_THERMISTOR	"));
	MODBUS_CALIBRATION_INTERNAL_THERMISTOR = Get_RegID(_T("	MODBUS_CALIBRATION_INTERNAL_THERMISTOR	"));
	MODBUS_CALIBRATION_ANALOG_IN2 = Get_RegID(_T("	MODBUS_CALIBRATION_ANALOG_IN2	"));
	MODBUS_INPUT1_SELECT = Get_RegID(_T("	MODBUS_INPUT1_SELECT                          	"));
	MODBUS_UNIVERSAL_NIGHTSET = Get_RegID(_T("	MODBUS_UNIVERSAL_NIGHTSET                  	"));
	MODBUS_HEAT_ORIGINAL_TABLE = Get_RegID(_T("	MODBUS_HEAT_ORIGINAL_TABLE                 	"));
	MODBUS_COOL_ORIGINAL_TABLE = Get_RegID(_T("	MODBUS_COOL_ORIGINAL_TABLE                 	"));
	MODBUS_PID2_MODE_OPERATION = Get_RegID(_T("	MODBUS_PID2_MODE_OPERATION	"));
	MODBUS_VALVE_TRAVEL_TIME = Get_RegID(_T("	MODBUS_VALVE_TRAVEL_TIME 	"));
	MODBUS_VALVE_PERCENT = Get_RegID(_T("	MODBUS_VALVE_PERCENT	"));
	MODBUS_INTERLOCK_OUTPUT1 = Get_RegID(_T("	MODBUS_INTERLOCK_OUTPUT1	"));
	MODBUS_INTERLOCK_OUTPUT2 = Get_RegID(_T("	MODBUS_INTERLOCK_OUTPUT2	"));
	MODBUS_INTERLOCK_OUTPUT3 = Get_RegID(_T("	MODBUS_INTERLOCK_OUTPUT3	"));
	MODBUS_INTERLOCK_OUTPUT4 = Get_RegID(_T("	MODBUS_INTERLOCK_OUTPUT4	"));
	MODBUS_INTERLOCK_OUTPUT5 = Get_RegID(_T("	MODBUS_INTERLOCK_OUTPUT5	"));
	MODBUS_INTERLOCK_OUTPUT6 = Get_RegID(_T("	MODBUS_INTERLOCK_OUTPUT6	"));
	MODBUS_INTERLOCK_OUTPUT7 = Get_RegID(_T("	MODBUS_INTERLOCK_OUTPUT7	"));
	MODBUS_SETPOINT_INCREASE = Get_RegID(_T("	MODBUS_SETPOINT_INCREASE	"));
	MODBUS_FREEZE_TEMP_SETPOINT = Get_RegID(_T("	MODBUS_FREEZE_TEMP_SETPOINT	"));
	MODBUS_FREEZE_DELAY_ON = Get_RegID(_T("	MODBUS_FREEZE_DELAY_ON	"));
	MODBUS_FREEZE_DELAY_OFF = Get_RegID(_T("	MODBUS_FREEZE_DELAY_OFF	"));
	MODBUS_ANALOG1_FUNCTION = Get_RegID(_T("	MODBUS_ANALOG1_FUNCTION	"));
	MODBUS_ANALOG2_FUNCTION = Get_RegID(_T("	MODBUS_ANALOG2_FUNCTION	"));
	MODBUS_DIGITAL1_FUNCTION = Get_RegID(_T("	MODBUS_DIGITAL1_FUNCTION	"));
	MODBUS_DISPLAY_HUNDRED = Get_RegID(_T("	MODBUS_DISPLAY_HUNDRED	"));
	MODBUS_DISPLAY_TEN = Get_RegID(_T("	MODBUS_DISPLAY_TEN	"));
	MODBUS_DISPLAY_DIGITAL = Get_RegID(_T("	MODBUS_DISPLAY_DIGITAL	"));
	MODBUS_DISPLAY_STATUS = Get_RegID(_T("	MODBUS_DISPLAY_STATUS	"));
	MODBUS_INPUT_MANU_ENABLE = Get_RegID(_T("	MODBUS_INPUT_MANU_ENABLE	"));
	MODBUS_OUTPUT_MANU_ENABLE = Get_RegID(_T("	MODBUS_OUTPUT_MANU_ENABLE	"));
	MODBUS_DIGITAL_INPUT = Get_RegID(_T("	MODBUS_DIGITAL_INPUT	"));
	MODBUS_DEAD_MASTER = Get_RegID(_T("	MODBUS_DEAD_MASTER	"));
	MODBUS_ROUND_DISPLAY = Get_RegID(_T("	MODBUS_ROUND_DISPLAY	"));
	MODBUS_MIN_ADDRESS = Get_RegID(_T("	MODBUS_MIN_ADDRESS 	"));
	MODBUS_MAX_ADDRESS = Get_RegID(_T("	MODBUS_MAX_ADDRESS	"));
	MODBUS_FAN_GRIDPOINT = Get_RegID(_T("	MODBUS_FAN_GRIDPOINT	"));
	MODBUS_MODE_GRIDPOINT = Get_RegID(_T("	MODBUS_MODE_GRIDPOINT	"));
	MODBUS_HOLD_GRIDPOINT = Get_RegID(_T("	MODBUS_HOLD_GRIDPOINT	"));
	MODBUS_CONFIGURATION = Get_RegID(_T("	MODBUS_CONFIGURATION	"));
	MODBUS_UTIL_MODE = Get_RegID(_T("	MODBUS_UTIL_MODE	"));
	MODBUS_EEPROM_SIZE = Get_RegID(_T("	MODBUS_EEPROM_SIZE	"));
	MODBUS_TIMER_SELECT = Get_RegID(_T("	MODBUS_TIMER_SELECT	"));
	MODBUS_OUTPUT1_FUNCTION = Get_RegID(_T("	MODBUS_OUTPUT1_FUNCTION	"));
	MODBUS_ROTATION_OUT2 = Get_RegID(_T("	MODBUS_ROTATION_OUT2	"));
	MODBUS_ROTATION_OUT3 = Get_RegID(_T("	MODBUS_ROTATION_OUT3	"));
	MODBUS_ROTATION_OUT4 = Get_RegID(_T("	MODBUS_ROTATION_OUT4	"));
	MODBUS_ROTATION_OUT5 = Get_RegID(_T("	MODBUS_ROTATION_OUT5	"));
	MODBUS_OUTPUT2_FUNCTION = Get_RegID(_T("	MODBUS_OUTPUT2_FUNCTION	"));
	MODBUS_OUTPUT3_FUNCTION = Get_RegID(_T("	MODBUS_OUTPUT3_FUNCTION	"));
	MODBUS_OUTPUT4_FUNCTION = Get_RegID(_T("	MODBUS_OUTPUT4_FUNCTION	"));
	MODBUS_OUTPUT5_FUNCTION = Get_RegID(_T("	MODBUS_OUTPUT5_FUNCTION	"));
	MODBUS_PIR_STAGE = Get_RegID(_T("	MODBUS_PIR_STAGE	"));
	MODBUS_AIR_CALIBRATION_POINT4 = Get_RegID(_T("	MODBUS_AIR_CALIBRATION_POINT4	"));
	MODBUS_AIR_CALIBRATION_POINT1 = Get_RegID(_T("	MODBUS_AIR_CALIBRATION_POINT1	"));
	MODBUS_AIR_CALIBRATION_POINT2 = Get_RegID(_T("	MODBUS_AIR_CALIBRATION_POINT2	"));
	MODBUS_AIR_CALIBRATION_POINT3 = Get_RegID(_T("	MODBUS_AIR_CALIBRATION_POINT3	"));
	MODUBS_HUMIDITY_RH = Get_RegID(_T("	MODUBS_HUMIDITY_RH 	"));
	MODBUS_HUMIDITY_FREQUENCY = Get_RegID(_T("	MODBUS_HUMIDITY_FREQUENCY	"));
	MODBUS_HUM_PIC_UPDATE = Get_RegID(_T("	MODBUS_HUM_PIC_UPDATE	"));
	MODBUS_HUM_CAL_NUM = Get_RegID(_T("	MODBUS_HUM_CAL_NUM 	"));
	MODBUS_HUM_CAL_EREASE = Get_RegID(_T("	MODBUS_HUM_CAL_EREASE 	"));
	MODBUS_HUM_CURRENT_DEFAULT = Get_RegID(_T("	MODBUS_HUM_CURRENT_DEFAULT 	"));
	MODBUS_HUM_PICDATAOK = Get_RegID(_T("	MODBUS_HUM_PICDATAOK 	"));
	MODBUS_HUM_OFFSETFLAG = Get_RegID(_T("	MODBUS_HUM_OFFSETFLAG  	"));
	MODBUS_HUMCOUNT1_H = Get_RegID(_T("	MODBUS_HUMCOUNT1_H   	"));
	MODBUS_HUMRH1_H = Get_RegID(_T("	MODBUS_HUMRH1_H	"));
	MODBUS_HUMCOUNT2_H = Get_RegID(_T("	MODBUS_HUMCOUNT2_H 	"));
	MODBUS_HUMRH2_H = Get_RegID(_T("	MODBUS_HUMRH2_H	"));
	MODBUS_HUMCOUNT3_H = Get_RegID(_T("	MODBUS_HUMCOUNT3_H 	"));
	MODBUS_HUMRH3_H = Get_RegID(_T("	MODBUS_HUMRH3_H	"));
	MODBUS_HUMCOUNT4_H = Get_RegID(_T("	MODBUS_HUMCOUNT4_H 	"));
	MODBUS_HUMRH4_H = Get_RegID(_T("	MODBUS_HUMRH4_H	"));
	MODBUS_HUMCOUNT5_H = Get_RegID(_T("	MODBUS_HUMCOUNT5_H 	"));
	MODBUS_HUMRH5_H = Get_RegID(_T("	MODBUS_HUMRH5_H	"));
	MODBUS_HUMCOUNT6_H = Get_RegID(_T("	MODBUS_HUMCOUNT6_H 	"));
	MODBUS_HUMRH6_H = Get_RegID(_T("	MODBUS_HUMRH6_H	"));
	MODBUS_HUMCOUNT7_H = Get_RegID(_T("	MODBUS_HUMCOUNT7_H 	"));
	MODBUS_HUMRH7_H = Get_RegID(_T("	MODBUS_HUMRH7_H	"));
	MODBUS_HUMCOUNT8_H = Get_RegID(_T("	MODBUS_HUMCOUNT8_H 	"));
	MODBUS_HUMRH8_H = Get_RegID(_T("	MODBUS_HUMRH8_H	"));
	MODBUS_HUMCOUNT9_H = Get_RegID(_T("	MODBUS_HUMCOUNT9_H 	"));
	MODBUS_HUMRH9_H = Get_RegID(_T("	MODBUS_HUMRH9_H	"));
	MODBUS_HUMCOUNT10_H = Get_RegID(_T("	MODBUS_HUMCOUNT10_H 	"));
	MODBUS_HUMRH10_H = Get_RegID(_T("	MODBUS_HUMRH10_H	"));
	MODBUS_TEM_RANGE_LOWER = Get_RegID(_T("	MODBUS_TEM_RANGE_LOWER  	"));
	MODBUS_TEM_RANGE_UPPER = Get_RegID(_T("	MODBUS_TEM_RANGE_UPPER	"));
	MODBUS_HUM_RANGE_LOWER = Get_RegID(_T("	MODBUS_HUM_RANGE_LOWER	"));
	MODBUS_HUM_RANGE_UPPER = Get_RegID(_T("	MODBUS_HUM_RANGE_UPPER	"));
	MODBUS_AQ_RANGE_LOWER = Get_RegID(_T("	MODBUS_AQ_RANGE_LOWER	"));
	MODBUS_AQ_RANGE_UPPER = Get_RegID(_T("	MODBUS_AQ_RANGE_UPPER 	"));
	MODBUS_TEMP_CURRENT_OUTPUT = Get_RegID(_T("	MODBUS_TEMP_CURRENT_OUTPUT      	"));
	MODBUS_HUM_CURRENT_OUTPUT = Get_RegID(_T("	MODBUS_HUM_CURRENT_OUTPUT	"));
	MODBUS_AQ_CURRENT_OUTPUT = Get_RegID(_T("	MODBUS_AQ_CURRENT_OUTPUT	"));
	MODBUS_TEMP_VOLTAGE_OUTPUT = Get_RegID(_T("	MODBUS_TEMP_VOLTAGE_OUTPUT	"));
	MODBUS_HUM_VOLTAGE_OUTPUT = Get_RegID(_T("	MODBUS_HUM_VOLTAGE_OUTPUT	"));
	MODBUS_AQ_VOLTAGE_OUTPUT = Get_RegID(_T("	MODBUS_AQ_VOLTAGE_OUTPUT  	"));
	MODBUS_CALIBRATION_MODE = Get_RegID(_T("	MODBUS_CALIBRATION_MODE  	"));
	MODBUS_SCROLL_MODE = Get_RegID(_T("	MODBUS_SCROLL_MODE	"));
	MODBUS_AQ_LEVEL0 = Get_RegID(_T("	MODBUS_AQ_LEVEL0	"));
	MODBUS_AQ_LEVEL1 = Get_RegID(_T("	MODBUS_AQ_LEVEL1	"));
	MODBUS_AQ_LEVEL2 = Get_RegID(_T("	MODBUS_AQ_LEVEL2	"));
	MODBUS_OUTPUT_MODE = Get_RegID(_T("	MODBUS_OUTPUT_MODE	"));
	MODBUS_TEMP_MANU_OUTPUT = Get_RegID(_T("	MODBUS_TEMP_MANU_OUTPUT	"));
	MODBUS_HUM_MANU_OUTPUT = Get_RegID(_T("	MODBUS_HUM_MANU_OUTPUT	"));
	MODBUS_AQ_MANU_OUTPUT = Get_RegID(_T("	MODBUS_AQ_MANU_OUTPUT	"));
	MODBUS_HUM_CALIBRATION_LOCKX = Get_RegID(_T("	MODBUS_HUM_CALIBRATION_LOCKX    	"));
	MODBUS_HUM_CALIBRATION_LOCKY = Get_RegID(_T("	MODBUS_HUM_CALIBRATION_LOCKY 	"));
}

int CAirQuality::Get_RegID(CString Name)
{
	Name.TrimLeft();
	Name.TrimRight();
	int regid = -1;
	vector<T3Register>::iterator iter;
	for (iter = m_vecT3Register.begin(); iter != m_vecT3Register.end(); iter++)
	{
		if (iter->regName == Name)
		{
			regid = iter->regID;
			break;
		}
	}
	return regid;
}

void CAirQuality::OnEnKillfocusMaxScaleAQ()
{
	m_fresh_data = FALSE;
	CString strTemp;
	m_edit_backlightTime.GetWindowText(strTemp);
	if (strTemp.IsEmpty())
	{
		return;
	}
	int backlighttime = -1;
	int BacklightAddress = -1;
	backlighttime = _wtoi(strTemp);

	if (backlighttime >= 0 && backlighttime <= 255)
	{
	}
	else
	{
		AfxMessageBox(_T("Please input the value>=0&&<=255"));
		return;
	}

	if (m_product_model == STM32_PRESSURE_NET)
	{
		BacklightAddress = 836;
	}
	else
	{
		BacklightAddress = 361;
	}

	if (product_register_value[BacklightAddress] == _wtoi(strTemp)) //Add this to judge weather this value need to change.
	{
		return;
	}

	unsigned short TempShort = (unsigned short)_wtoi(strTemp);
	int ret = write_one(m_current_TstatID, BacklightAddress, TempShort);
	if (ret > 0)
	{
		product_register_value[BacklightAddress] = TempShort;
		ShowAirqualityDialog();
	}
	else
	{
		AfxMessageBox(_T("Write Fail!"));
	}
}

void CAirQuality::OnEnKillfocusChangeHumName()
{
	CString strSerial;
	strSerial.Format(_T("%d"), get_serialnumber());

	CString oldname;
	CString newname;
	CString strSql;
	GetDlgItem(IDC_EDIT_TSTAT_NAME)->GetWindowText(newname);
	if (newname.IsEmpty())
	{
		return;
	}
	oldname.Format(_T("%s%s"), GetTextFromReg(715), GetTextFromReg(719));
	if (oldname.CompareNoCase(newname) == 0)
	{
		return;
	}
	if (newname.GetLength() > 17) //Check for length too long;
	{
		newname.Delete(16, newname.GetLength() - 16);
	}
	char cTemp1[16];
	memset(cTemp1, 0, 16);
	WideCharToMultiByte(CP_ACP, 0, newname.GetBuffer(), -1, cTemp1, 16, NULL, NULL);
	unsigned char Databuffer[16];
	memcpy_s(Databuffer, 16, cTemp1, 16);
	if (Write_Multi(m_current_TstatID, Databuffer, 715, 16, 10) > 0)
	{
		for (int i = 0; i < 16; i++)
		{
			product_register_value[715 + i] = Databuffer[i];
		}
		GetDlgItem(IDC_EDIT_TSTAT_NAME)->GetWindowText(newname);
		CppSQLite3DB SqliteDBBuilding;
		CppSQLite3Table table;
		CppSQLite3Query q;
		SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

		strSql.Format(_T("update ALL_NODE set Product_name='%s' where Serial_ID='%s'"), newname, strSerial);
		q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
		SqliteDBBuilding.closedb();
		CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd); // This is used to refresh the product tree structure 

		::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING, 0, 0);
	}
	/* else
	 {
	    GetDlgItem(IDC_EDIT_TSTAT_NAME)->GetWindowText(oldname);
	 }*/
}

void CAirQuality::OnEnKillfocustempBacklight()
{
	m_fresh_data = FALSE;
	CString strTemp;
	int BacklightAddress = -1;
	GetDlgItem(IDC_EDIT_BACKLIGHT_TIME_TIME)->GetWindowText(strTemp);
	if (strTemp.IsEmpty())
	{
		return;
	}
	int backlighttime = -1;

	backlighttime = _wtoi(strTemp);

	if (backlighttime >= 0 && backlighttime <= 255)
	{
	}
	else
	{
		AfxMessageBox(_T("Please input the value>=0&&<=255"));
		return;
	}
	if (m_product_model == STM32_PRESSURE_NET)
	{
		BacklightAddress = 836;
	}
	else
	{
		BacklightAddress = 361;
	}

	if (product_register_value[BacklightAddress] == _wtoi(strTemp)) //Check if the value has changed
	{
		return;
	}

	unsigned short TempShort = (unsigned short)_wtoi(strTemp);
	int ret = write_one(m_current_TstatID, BacklightAddress, TempShort);
	if (ret > 0)
	{
		product_register_value[BacklightAddress] = TempShort;
		ShowAirqualityDialog();
	}
	else
	{
		AfxMessageBox(_T("Write Fail!"));
	}
}

void CAirQuality::OnEnChangeBacklight()
{
	int sel = m_combox_backlightTime.GetCurSel();
	if (0 == sel)
	{
		m_edit_backlightTime.SetWindowTextW(_T("0"));
	}

	if (1 == sel)
	{
		m_edit_backlightTime.SetWindowTextW(_T("255"));
	}
	OnEnKillfocustempBacklight();
}


#include "../EreaseDlg.h"

void CAirQuality::OnBnClickedButton_Change_ID()
{
	CEreaseDlg Dlg;
	Dlg.DoModal();
}

#include "../GraphicMode.h"

void CAirQuality::OnBnClickedButton_Graphic()
{
	m_fresh_data = false;
	CGraphicMode dlg;
	dlg.DoModal();
	g_hwnd_now = this->m_hWnd;
}

void CAirQuality::OnBnClickedButton_Tranducer()
{
	if (product_register_value[530] == 1)
	{
		int ret = write_one(g_tstat_id, 530, 0);
		if (ret > 0)
		{
			product_register_value[530] = 0;
		}
	}
	else
	{
		return;
	}

	if (product_register_value[530] == 0)
	{
		((CButton *)GetDlgItem(IDC_RADIO_TRANDUCER))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_RADIO_pid))->SetCheck(0);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RADIO_TRANDUCER))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO_pid))->SetCheck(1);
	}
}

void CAirQuality::OnBnClickedButton_PID()
{
	if (product_register_value[530] == 0)
	{
		int ret = write_one(g_tstat_id, 530, 1);
		if (ret > 0)
		{
			product_register_value[530] = 1;
		}
	}
	else
	{
		return;
	}

	if (product_register_value[530] == 0)
	{
		((CButton *)GetDlgItem(IDC_RADIO_TRANDUCER))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_RADIO_pid))->SetCheck(0);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RADIO_TRANDUCER))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO_pid))->SetCheck(1);
	}
}

void CAirQuality::OnBnClickedButton_Factory_Reset()
{
	if (m_product_model == STM32_PRESSURE_NET)
	{
		write_one(m_current_TstatID, 16, 143);
		AfxMessageBox(L"Reset,Successfully");
		return;
	}

	if (AfxMessageBox(_T(" This will reset the module to the factory defaults, are you sure?")) == IDOK)
	{
		int ret = write_one(m_current_TstatID, 311, 1);
		write_one(m_current_TstatID, 309, 1);
		write_one(m_current_TstatID, 307, 0);
		write_one(m_current_TstatID, 346, 200);
		write_one(m_current_TstatID, 347, 400);
		write_one(m_current_TstatID, 348, 600);
		write_one(m_current_TstatID, 105, 500);
		if (ret > 0)
		{
			AfxMessageBox(_T("Factory Reset Successful!"));

			unsigned short Data[20];
			ret = Read_Multi(m_current_TstatID, Data, MODBUS_HUMCOUNT1_H, 20);
			if (ret > 0)
			{
				for (int i = 0; i < 20; i++)
				{
					product_register_value[MODBUS_HUMCOUNT1_H + i] = Data[i];
				}
			}
			Initial_UserList();
		}
	}
}


void CAirQuality::OnBnClickedButton_UserTable_Add()
{
	int AddressValue = -1;

	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		AddressValue = 455;
	}
	else
	{
		AddressValue = MODBUS_HUM_CAL_NUM;
	}
	if (product_register_value[AddressValue] >= 10)
	{
		AfxMessageBox(_T("Can't Add,Current Point can't be more than 10!"));
		return;
	}

	int ret = write_one(m_current_TstatID, AddressValue, product_register_value[AddressValue] + 1);
	if (ret > 0)
	{
		++product_register_value[AddressValue];
	}
	else
	{
		AfxMessageBox(_T("Add Fail,Try again!"));
		return;
	}

	unsigned short Data[20];

	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		AddressValue = 456;
	}
	else
	{
		AddressValue = MODBUS_HUMCOUNT1_H;
	}

	ret = Read_Multi(m_current_TstatID, Data, AddressValue, 20);
	if (ret > 0)
	{
		for (int i = 0; i < 20; i++)
		{
			product_register_value[AddressValue + i] = Data[i];
		}
	}
	Initial_UserList();
	InitChartData();

	//	UpdatePoint();
}

void CAirQuality::OnBnClickedButton_UserTable_Del()
{
	int AddressValue = -1;

	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		AddressValue = 455;
	}
	else
	{
		AddressValue = MODBUS_HUM_CAL_NUM;
	}

	if (product_register_value[AddressValue] <= 0)
	{
		AfxMessageBox(_T("Can't Add, current point can't be less than 0!"));
		return;
	}
	int ret = write_one(m_current_TstatID, AddressValue, product_register_value[AddressValue] - 1);
	if (ret > 0)
	{
		--product_register_value[AddressValue];

		int RegAddress;
		//int row =m_sensor_calibrate_table.get_Rows();
		int row = m_user_list.GetItemCount();
		row -= 1;//End Line
		if (m_product_model == PM_HUMTEMPSENSOR)
		{
			RegAddress = 456 + 2 * row;
		}
		else
		{
			RegAddress = MODBUS_HUMCOUNT1_H + 2 * row + 1;
		}

		int ret = write_one(m_current_TstatID, RegAddress, 0);
	}
	else
	{
		AfxMessageBox(_T("Add Failed, try again!"));
		return;
	}
	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		AddressValue = 456;
	}
	else
	{
		AddressValue = MODBUS_HUMCOUNT1_H;
	}
	unsigned short Data[20];
	ret = Read_Multi(m_current_TstatID, Data, AddressValue, 20);
	if (ret > 0)
	{
		for (int i = 0; i < 20; i++)
		{
			product_register_value[AddressValue + i] = Data[i];
		}
	}
	Initial_UserList();
	InitChartData();

	//	UpdatePoint();
}

LRESULT CAirQuality::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//
	if (WM_AIRQUANITY_FRESH == message)
	{
		Fresh_Input_Output();
	}
	if (MY_FRESH_DRAW_GRAPHIC == message)
	{
		//		UpdatePoint();
	}
	return CFormView::WindowProc(message, wParam, lParam);
}

BOOL CAirQuality::PreTranslateMessage(MSG* pMsg)
{
	
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			//CWnd *temp_focus=GetFocus();	//Click enter and the cursor will stay in this edit field.
			////GetDlgItem(IDC_REFRESHBUTTON)->SetFocus();
			//temp_focus->OnKillFocus();
			CWnd* temp_focus = GetFocus(); //Click enter and the cursor stays in this edit field.
			GetDlgItem(IDC_EDIT1_TEST)->SetFocus();
			temp_focus->SetFocus();

			return 1;
		}
	}

	return CFormView::PreTranslateMessage(pMsg);
}


void CAirQuality::InitChartData()
{
	m_Xmax = 0;
	m_Xmin = 0;
	m_Ymax = 0;
	m_Ymin = 0;
	m_Draw_User = 3;
	int dataline = 0;
	Registers_Infor temp;
	RegPoint temppoint;
	g_vectRegisters.clear();
	int AddressValue = -1;
	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		AddressValue = 455;
	}
	else
	{
		AddressValue = MODBUS_HUM_CAL_NUM;
	}


	int rows = product_register_value[AddressValue];
	if (rows == 1)
	{
		m_Draw_User = 1;
		if (product_register_value[7] == PM_HUMTEMPSENSOR)
		{
			AddressValue = 451;
			m_offset = (short)product_register_value[AddressValue];
		}
		else
		{
			AddressValue = MODBUS_CALIBRATION_HUM;
			m_offset = (short)product_register_value[MODBUS_CALIBRATION_HUM] - 500;
		}
	}
	else if (rows == 0)
	{
		m_Draw_User = 0;
	}
	else
	{
		m_Draw_User = 3;
	}
	if (m_Draw_User == 1)
	{
		if (m_product_model == PM_HUMTEMPSENSOR)
		{
			rows = 10;
			dataline = 0;
			temp.Reg_Name = _T("Factory Table Chart");
			temp.Point_Color = RGB(0,100,255);
			for (int row = 1; row <= rows; row++)
			{
				if (Default_Value[4 + 2 * (row - 1)] == 0)
				{
					continue;
				}
				Factory_Xdata[row - 1] = (short)Default_Value[4 + 2 * (row - 1)];

				temppoint.Time_offset = (short)Default_Value[4 + 2 * (row - 1)];

				Factory_Ydata[row - 1] = (short)Default_Value[4 + 2 * row - 1];

				temppoint.Value_offset = (short)Default_Value[4 + 2 * row - 1];

				temp.Reg_Point.push_back(temppoint);
				dataline++;
			}
			g_vectRegisters.push_back(temp);
			temp.Reg_Point.clear();

			m_Xmax = Factory_Xdata[0];
			m_Xmin = Factory_Xdata[0];
			m_Ymax = Factory_Ydata[0];
			m_Ymin = Factory_Ydata[0];

			for (int row = 0; row < dataline; row++)
			{
				if (m_Xmax < Factory_Xdata[row])
				{
					m_Xmax = Factory_Xdata[row];
				}

				if (m_Xmin > Factory_Xdata[row])
				{
					m_Xmin = Factory_Xdata[row];
				}

				if (m_Ymax < Factory_Ydata[row])
				{
					m_Ymax = Factory_Ydata[row];
				}

				if (m_Ymin > Factory_Ydata[row])
				{
					m_Ymin = Factory_Ydata[row];
				}
			}

			temp.Reg_Name = _T("User Table Chart");
			temp.Point_Color = RGB(255,0,0);

			for (int i = 0; i < dataline; i++)
			{
				User_Xdata[i] = (short)Factory_Xdata[i];
				temppoint.Time_offset = (short)Factory_Xdata[i];
				User_Ydata[i] = (short)Factory_Ydata[i] + m_offset;//
				temppoint.Value_offset = (short)Factory_Ydata[i] + m_offset;

				temp.Reg_Point.push_back(temppoint);
			}
			g_vectRegisters.push_back(temp);

			for (int row = 0; row < dataline; row++)
			{
				if (m_Ymax < User_Ydata[row])
				{
					m_Ymax = User_Ydata[row];
				}

				if (m_Ymin > User_Ydata[row])
				{
					m_Ymin = User_Ydata[row];
				}
			}
		}
		else
		{
			rows = Default_Value[3];
			temp.Reg_Name = _T("Factory Table Chart");
			temp.Point_Color = RGB(0,100,255);
			for (int row = 1; row <= rows; row++)
			{
				Factory_Xdata[row - 1] = (short)Default_Value[4 + 2 * (row - 1)];
				temppoint.Time_offset = (short)Default_Value[4 + 2 * (row - 1)];
				Factory_Ydata[row - 1] = (short)Default_Value[4 + 2 * row - 1];

				temppoint.Value_offset = (short)Default_Value[4 + 2 * row - 1];

				temp.Reg_Point.push_back(temppoint);
			}
			g_vectRegisters.push_back(temp);
			temp.Reg_Point.clear();

			m_Xmax = Factory_Xdata[0];
			m_Xmin = Factory_Xdata[0];
			m_Ymax = Factory_Ydata[0];
			m_Ymin = Factory_Ydata[0];

			for (int row = 0; row < rows; row++)
			{
				if (m_Xmax < Factory_Xdata[row])
				{
					m_Xmax = Factory_Xdata[row];
				}

				if (m_Xmin > Factory_Xdata[row])
				{
					m_Xmin = Factory_Xdata[row];
				}

				if (m_Ymax < Factory_Ydata[row])
				{
					m_Ymax = Factory_Ydata[row];
				}

				if (m_Ymin > Factory_Ydata[row])
				{
					m_Ymin = Factory_Ydata[row];
				}
			}

			temp.Reg_Name = _T("User Table Chart");
			temp.Point_Color = RGB(255,0,0);

			for (int i = 0; i < rows; i++)
			{
				User_Xdata[i] = (short)Factory_Xdata[i];
				temppoint.Time_offset = (short)Factory_Xdata[i];
				User_Ydata[i] = (short)Factory_Ydata[i] + m_offset;//
				temppoint.Value_offset = (short)Factory_Ydata[i] + m_offset;

				temp.Reg_Point.push_back(temppoint);
			}
			g_vectRegisters.push_back(temp);

			for (int row = 0; row < rows; row++)
			{
				if (m_Ymax < User_Ydata[row])
				{
					m_Ymax = User_Ydata[row];
				}

				if (m_Ymin > User_Ydata[row])
				{
					m_Ymin = User_Ydata[row];
				}
			}
		}
	}

	else if (m_Draw_User == 0)
	{
		rows = Default_Value[3];
		temp.Reg_Name = _T("Factory Table Chart");
		temp.Point_Color = RGB(0,100,255);
		for (int row = 1; row <= rows; row++)
		{
			Factory_Xdata[row - 1] = Default_Value[4 + 2 * row - 1];
			temppoint.Time_offset = Default_Value[4 + 2 * row - 1];
			Factory_Ydata[row - 1] = Default_Value[4 + 2 * (row - 1)];

			temppoint.Value_offset = Default_Value[4 + 2 * (row - 1)];

			temp.Reg_Point.push_back(temppoint);
		}
		g_vectRegisters.push_back(temp);
		temp.Reg_Point.clear();

		m_Xmax = Factory_Xdata[0];
		m_Xmin = Factory_Xdata[0];
		m_Ymax = Factory_Ydata[0];
		m_Ymin = Factory_Ydata[0];

		for (int row = 0; row < rows; row++)
		{
			if (m_Xmax < Factory_Xdata[row])
			{
				m_Xmax = Factory_Xdata[row];
			}

			if (m_Xmin > Factory_Xdata[row])
			{
				m_Xmin = Factory_Xdata[row];
			}

			if (m_Ymax < Factory_Ydata[row])
			{
				m_Ymax = Factory_Ydata[row];
			}

			if (m_Ymin > Factory_Ydata[row])
			{
				m_Ymin = Factory_Ydata[row];
			}
		}
	}
	else //
	{
		temp.Reg_Name = _T("User Table Chart");
		temp.Point_Color = RGB(255,0,0);
		for (int row = 1; row <= rows; row++)
		{
			if (m_product_model == PM_HUMTEMPSENSOR)
			{
				AddressValue = 456;
			}
			else
			{
				AddressValue = MODBUS_HUMCOUNT1_H;
			}

			if (product_register_value[MODBUS_HUMCOUNT1_H + 2 * (row - 1)] == 0)// Frequency is zero ,continue
			{
				continue;
			}

			if (m_product_model == PM_HUMTEMPSENSOR)
			{
				AddressValue = 456;

				User_Xdata[row - 1] = product_register_value[AddressValue + 2 * row - 1];
				temppoint.Time_offset = product_register_value[AddressValue + 2 * row - 1];

				User_Ydata[row - 1] = product_register_value[AddressValue + 2 * (row - 1)];
				temppoint.Value_offset = product_register_value[AddressValue + 2 * (row - 1)];
			}
			else
			{
				AddressValue = MODBUS_HUMCOUNT1_H;

				User_Xdata[row - 1] = product_register_value[MODBUS_HUMCOUNT1_H + 2 * (row - 1)];
				temppoint.Time_offset = product_register_value[MODBUS_HUMCOUNT1_H + 2 * (row - 1)];

				User_Ydata[row - 1] = product_register_value[MODBUS_HUMCOUNT1_H + 2 * row - 1];
				temppoint.Value_offset = product_register_value[MODBUS_HUMCOUNT1_H + 2 * row - 1];
			}


			temp.Reg_Point.push_back(temppoint);
			dataline++;
		}

		g_vectRegisters.push_back(temp);
		temp.Reg_Point.clear();
		m_Xmax = User_Xdata[0];
		m_Xmin = User_Xdata[0];
		m_Ymax = User_Ydata[0];
		m_Ymin = User_Ydata[0];


		for (int row = 0; row < dataline; row++)
		{
			if (User_Xdata[row] == 0)
			{
				continue;
			}
			if (m_Xmax < User_Xdata[row])
			{
				m_Xmax = User_Xdata[row];
			}

			if (m_Xmin > User_Xdata[row])
			{
				m_Xmin = User_Xdata[row];
			}

			if (m_Ymax < User_Ydata[row])
			{
				m_Ymax = User_Ydata[row];
			}

			if (m_Ymin > User_Ydata[row])
			{
				m_Ymin = User_Ydata[row];
			}
		}


		temp.Reg_Name = _T("Factory Table Chart");
		temp.Point_Color = RGB(0,100,255);

		rows = Default_Value[3];
		if (m_product_model == PM_HUMTEMPSENSOR)
		{
			rows = 10;
		}
		dataline = 0;
		if (m_product_model == PM_HUMTEMPSENSOR)
		{
			for (int row = 1; row <= rows; row++)
			{
				if (Default_Value[4 + 2 * row - 1] == 0)
				{
					continue;
				}
				Factory_Xdata[row - 1] = Default_Value[4 + 2 * (row - 1)];
				temppoint.Time_offset = Default_Value[4 + 2 * (row - 1)];
				Factory_Ydata[row - 1] = Default_Value[4 + 2 * row - 1];
				temppoint.Value_offset = Default_Value[4 + 2 * row - 1];

				temp.Reg_Point.push_back(temppoint);
				dataline++;
			}
			g_vectRegisters.push_back(temp);
		}
		else
		{
			for (int row = 1; row <= rows; row++)
			{
				if (Default_Value[4 + 2 * row - 1] == 0)
				{
					continue;
				}
				Factory_Xdata[row - 1] = Default_Value[4 + 2 * row - 1];
				temppoint.Time_offset = Default_Value[4 + 2 * row - 1];
				Factory_Ydata[row - 1] = Default_Value[4 + 2 * (row - 1)];
				temppoint.Value_offset = Default_Value[4 + 2 * (row - 1)];

				temp.Reg_Point.push_back(temppoint);
				dataline++;
			}
			g_vectRegisters.push_back(temp);
		}

		temp.Reg_Point.clear();
		for (int row = 0; row < dataline; row++)
		{
			if (Factory_Xdata[row] == 0)
			{
				continue;
			}
			if (m_Xmax < Factory_Xdata[row])
			{
				m_Xmax = Factory_Xdata[row];
			}

			if (m_Xmin > Factory_Xdata[row])
			{
				m_Xmin = Factory_Xdata[row];
			}

			if (m_Ymax < Factory_Ydata[row])
			{
				m_Ymax = Factory_Ydata[row];
			}

			if (m_Ymin > Factory_Ydata[row])
			{
				m_Ymin = Factory_Ydata[row];
			}
		}
	}

	while (m_Xmax % 10 != 0)
	{
		m_Xmax++;
	}
	while (m_Xmin % 10 != 0)
	{
		m_Xmin--;
	}

	while (m_Ymax % 10 != 0)
	{
		m_Ymax++;
	}
	while (m_Ymin % 10 != 0)
	{
		m_Ymin--;
	}
}


void CAirQuality::Initial_InputList()
{
	int AddressValue = -1;
	CString strTemp;
	// m_input_list.ShowWindow(SW_HIDE);
	m_input_list.DeleteAllItems();
	while (m_input_list.DeleteColumn(0));

	m_input_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	m_input_list.SetExtendedStyle(m_input_list.GetExtendedStyle() | LVS_EX_GRIDLINES & (~LVS_EX_FULLROWSELECT));
	m_input_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_input_list.InsertColumn(1, _T("Full Label"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(2, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(3, _T("Value"), 50, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(4, _T("Range"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(5, _T("Calibration"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(6, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	g_hwnd_now = this->m_hWnd;
	if (m_product_model == STM32_PRESSURE_NET)
	{
		strTemp = L"1";
		m_input_list.InsertItem(0, strTemp);
		m_input_list.SetItemText(0, 1, L"Pressure");
		if (product_register_value[708] == 0)
		{
			m_input_list.SetItemText(0, 2, L"Auto");
			if (product_register_value[702] == 0)
			{
				strTemp.Format(_T("%.2f"), ((float)((short)product_register_value[711])) / 100.0);
			}
			else
			{
				strTemp.Format(_T("%.1f"), ((float)((short)product_register_value[711])) / 10.0);
			}


			m_input_list.SetItemText(0, 3, strTemp);
		}
		else
		{
			m_input_list.SetItemText(0, 2, L"Manual");

			if (product_register_value[702] == 0)
			{
				strTemp.Format(_T("%0.01f"), ((float)((short)product_register_value[709])) / 100.0);
			}
			else
			{
				strTemp.Format(_T("%0.1f"), ((float)((short)product_register_value[709])) / 10.0);
			}

			m_input_list.SetItemText(0, 3, strTemp);
		}
		switch (product_register_value[702])
		{
		case 0:
			strTemp = L"inWC";//100
			break;
		case 1:
			strTemp = L"kpal";
			break;
		case 2:
			strTemp = L"PSI";//10
			break;
		case 3:
			strTemp = L"mmHg";
			break;
		case 4:
			strTemp = L"inHg";
			break;
		case 5:
			strTemp = L"Kg/CM2";
			break;
		case 6:
			strTemp = L"atmosphere";
			break;
		case 7:
			strTemp = L"bar";
			break;
		default:
			strTemp = L"";
			break;
		}

		m_input_list.SetItemText(0, 4, strTemp);

		if (product_register_value[774] == 0)
		{
			strTemp.Format(_T("%d"), product_register_value[712]);
		}
		else
		{
			strTemp.Format(_T("%d"), product_register_value[770]);
		}
		m_input_list.SetItemText(0, 5, strTemp);
		strTemp.Format(_T("%d"), product_register_value[705]);
		m_input_list.SetItemText(0, 6, strTemp);

		return;
	}
	m_input_list.InsertItem(0,_T("1"));
	m_input_list.InsertItem(1,_T("2"));

	//m_input_list.InsertItem(2,_T("3"));

	m_input_list.SetItemText(0, 1,_T("Temperature"));
	m_input_list.SetItemText(1, 1,_T("Humidity"));

	//AQ,HUM-R//HUM
	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		AddressValue = 370;
	}
	else
	{
		AddressValue = 349;
	}
	bitset<16> AM(product_register_value[AddressValue]);

	if (!AM[0])
	{
		m_input_list.SetItemText(0, 2,Global_String_AUTO);

		if (m_product_model == PM_HUMTEMPSENSOR)
		{
			AddressValue = 121;
		}
		else
		{
			AddressValue = MODBUS_DEGC_OR_F;
		}

		if (product_register_value[AddressValue] == 0)
		{
			if (m_product_model == PM_HUMTEMPSENSOR)
			{
				AddressValue = 101;
			}
			else
			{
				AddressValue = MODBUS_TEMPRATURE_CHIP;
			}

			strTemp.Format(_T("%0.1f"), ((float)((short)product_register_value[AddressValue])) / 10.0);
			m_input_list.SetItemText(0, 3, strTemp);
			m_input_list.SetItemText(0, 4,Global_String_C);
		}
		else
		{
			if (m_product_model == PM_HUMTEMPSENSOR)
			{
				AddressValue = 100;
			}
			else
			{
				AddressValue = MODBUS_TEMPRATURE_CHIP_F;
			}
			strTemp.Format(_T("%0.1f"), ((float)((short)product_register_value[AddressValue])) / 10.0);
			m_input_list.SetItemText(0, 3, strTemp);
			m_input_list.SetItemText(0, 4,Glocal_String_F);
		}
	}
	else
	{
		m_input_list.SetItemText(0, 2,Global_String_MANUAL);

		if (m_product_model == PM_HUMTEMPSENSOR)
		{
			AddressValue = 371;
		}
		else
		{
			AddressValue = 350;
		}

		strTemp.Format(_T("%0.1f"), ((float)((short)product_register_value[AddressValue])) / 10.0);
		m_input_list.SetItemText(0, 3, strTemp);

		if (m_product_model == PM_HUMTEMPSENSOR)
		{
			AddressValue = 121;
		}
		else
		{
			AddressValue = MODBUS_DEGC_OR_F;
		}
		if (product_register_value[AddressValue] == 0)
		{
			m_input_list.SetItemText(0, 4,Global_String_C);
		}
		else
		{
			m_input_list.SetItemText(0, 4,Glocal_String_F);
		}
	}

	if (!AM[1])
	{
		m_input_list.SetItemText(1, 2,Global_String_AUTO);

		if (m_product_model == PM_HUMTEMPSENSOR)
		{
			AddressValue = 373;
		}
		else
		{
			AddressValue = MODUBS_HUMIDITY_RH;
		}

		strTemp.Format(_T("%0.1f"), ((float)((short)product_register_value[AddressValue])) / 10.0);
		m_input_list.SetItemText(1, 3, strTemp);
		strTemp = _T("%");
		m_input_list.SetItemText(1, 4, strTemp);
	}
	else
	{
		m_input_list.SetItemText(1, 2,Global_String_MANUAL);

		if (m_product_model == PM_HUMTEMPSENSOR)
		{
			AddressValue = 372;
		}
		else
		{
			AddressValue = 351;
		}
		strTemp.Format(_T("%0.1f"), ((float)((short)product_register_value[AddressValue])) / 10.0);
		m_input_list.SetItemText(1, 3, strTemp);
		strTemp = _T("%");
		m_input_list.SetItemText(1, 4, strTemp);
	}

	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		AddressValue = 450;
	}
	else
	{
		AddressValue = MODBUS_OVERRIDE_TIMER_LEFT;
	}
	strTemp.Format(_T("%d"), (short)product_register_value[AddressValue]);
	m_input_list.SetItemText(0, 6, strTemp);
	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		if (product_register_value[454] == 0)
		{
			AddressValue = 479;
		}
		else
		{
			AddressValue = 451;
		}
	}
	else
	{
		AddressValue = MODBUS_HUM_FILTER;
	}
	//strTemp.Format(_T("%d"),product_register_value[AddressValue]);
	//strTemp.Format(_T("%d"),((short)product_register_value[AddressValue]));
	strTemp.Format(_T("%d"), (short)product_register_value[AddressValue]);

	m_input_list.SetItemText(1, 6, strTemp);

	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		strTemp.Format(_T("%d"), ((short)product_register_value[452]));
		m_input_list.SetItemText(0, 6, strTemp);
		strTemp.Format(_T("%d"), ((short)product_register_value[453]));
		m_input_list.SetItemText(1, 6, strTemp);

		AddressValue = 450;
		strTemp.Format(_T("%0.1f"), ((float)((short)product_register_value[AddressValue])) / 10.0);

		m_input_list.SetItemText(0, 5, strTemp);
		if (product_register_value[454] == 0)
		{
			AddressValue = 479;
		}
		else
		{
			AddressValue = 451;
		}
		strTemp.Format(_T("%0.1f"), ((float)((short)product_register_value[AddressValue])) / 10.0);

		m_input_list.SetItemText(1, 5, strTemp);
	}

	if (m_product_model == PM_AirQuality)
	{
		m_input_list.InsertItem(2,_T("3"));
		m_input_list.SetItemText(2, 1,_T("Air Quanlity"));
		if (!AM[2])
		{
			m_input_list.SetItemText(2, 2,Global_String_AUTO);

			strTemp.Format(_T("%d"), product_register_value[MODBUS_AQ]);
			m_input_list.SetItemText(2, 3, strTemp);
		}
		else
		{
			m_input_list.SetItemText(2, 2,Global_String_MANUAL);

			strTemp.Format(_T("%d"), product_register_value[352]);
			m_input_list.SetItemText(2, 3, strTemp);
		}
		//strTemp.Format(_T("%d"),product_register_value[MODBUS_AQ_FILTER]);
		strTemp.Format(_T("%0.1f"), ((float)((short)product_register_value[MODBUS_AQ_FILTER])) / 10.0);

		m_input_list.SetItemText(2, 5, strTemp);
		strTemp = _T("N/A");
		m_input_list.SetItemText(2, 4, strTemp);
	}

	if (product_register_value[7] == PM_HUM_R || product_register_value[7] == PM_AirQuality)
	{
		AddressValue = 104;//TEMP Calibration
		strTemp.Format(_T("%0.1f"), ((float)((short)product_register_value[AddressValue] - 500)) / 10.0);

		m_input_list.SetItemText(0, 5, strTemp);
		if (product_register_value[309] == 0)
		{
			AddressValue = 107;//HUM Calibration
			strTemp.Format(_T("%0.1f"), ((float)((short)product_register_value[AddressValue] - 500)) / 10.0);

			m_input_list.SetItemText(1, 5, strTemp);
		}
		else
		{
			AddressValue = 106;//HUM Calibration
			strTemp.Format(_T("%0.1f"), ((float)((short)product_register_value[AddressValue] - 500)) / 10.0);

			m_input_list.SetItemText(1, 5, strTemp);
		}

		if (product_register_value[7] == PM_AirQuality)
		{
			AddressValue = 105;//AQ Calibration
			strTemp.Format(_T("%0.1f"), ((float)((short)product_register_value[AddressValue] - 500)) / 10.0);

			m_input_list.SetItemText(2, 5, strTemp);
		}
	}

	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		m_input_list.InsertItem(2, _T("3"));
		m_input_list.SetItemText(2, 1, _T("Light"));
		if (!AM[2])
		{
			m_input_list.SetItemText(2, 2, Global_String_AUTO);
		}
		else
		{
			m_input_list.SetItemText(2, 2, Global_String_MANUAL);
		}
		strTemp.Format(_T("%d"), product_register_value[538]);
		m_input_list.SetItemText(2, 3, strTemp);

		strTemp.Format(_T("%d"), product_register_value[540]);

		m_input_list.SetItemText(2, 5, strTemp);
		//strTemp.Format(_T("%d"),product_register_value[MODBUS_AQ_FILTER]);
		strTemp.Format(_T("%d"), product_register_value[536]);

		m_input_list.SetItemText(2, 6, strTemp);
		strTemp = _T("lux");
		m_input_list.SetItemText(2, 4, strTemp);
	}
    if (product_register_value[7] != STM32_PM25)
		m_input_list.ShowWindow(SW_SHOW);
}


void CAirQuality::Initial_OutputList()
{
	int AddressValue = -1;
	CString strTemp;

	m_output_list.DeleteAllItems();
	while (m_output_list.DeleteColumn(0));

	m_output_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	m_output_list.SetExtendedStyle(m_output_list.GetExtendedStyle() | LVS_EX_GRIDLINES & (~LVS_EX_FULLROWSELECT));
	m_output_list.InsertColumn(0, _T("NUM"), 40, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	if (m_product_model == STM32_PRESSURE_NET)
	{
		m_output_list.InsertColumn(1, _T("Full Label"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	}
	else
	{
		m_output_list.InsertColumn(1, _T("Full Label"), 60, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	}

	m_output_list.InsertColumn(2, _T("Value"), 45, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(3, _T("Range"), 45, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(4, _T("Min Out Scale"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(5, _T("Max Out Scale"), 85, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);


	//Auto/Manual
	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		m_output_list.InsertColumn(6, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	}

	g_hwnd_now = this->m_hWnd;
	if (m_product_model == STM32_PRESSURE_NET)
	{
		m_output_list.InsertItem(0, _T("1"));
		m_output_list.SetItemText(0, 1, L"Pressure");
		int value = read_one(g_tstat_id, 3066);
		if (product_register_value[775] == 1 || product_register_value[775] == 2)
		{
			strTemp.Format(_T("%.2f"), ((float)(value) / 100.0));
		}
		else
		{
			strTemp.Format(_T("%.1f"), ((float)(value) / 10.0));
		}
		m_output_list.SetItemText(0, 2, strTemp);
		int output_range = product_register_value[775];

		if (output_range == 1)
		{
			strTemp = _T("0-10v");
		}
		else if (output_range == 2)
		{
			strTemp = _T("0-5v");
		}
		else if (output_range == 0)
		{
			strTemp = _T("4-20mA");
		}
		else
		{
			strTemp = _T("");
		}

		m_output_list.SetItemText(0, 3, strTemp);

		strTemp.Format(_T("%d"), (short)product_register_value[703]);
		switch (product_register_value[702])
		{
		case 0:
			strTemp.Format(_T("%.2f inWC"), ((float)((short)product_register_value[703])) / 100.0);//100
			break;
		case 1:
			strTemp = L"kpal";
			break;
		case 2:
			strTemp = L"PSI";//10
			strTemp.Format(_T("%.2f PSI"), ((float)((short)product_register_value[703])) / 10.0);
			break;
		case 3:
			strTemp = L"mmHg";
			break;
		case 4:
			strTemp = L"inHg";
			break;
		case 5:
			strTemp = L"Kg/CM2";
			break;
		case 6:
			strTemp = L"atmosphere";
			break;
		case 7:
			strTemp = L"bar";
			break;
		default:
			strTemp = L"";
			break;
		}

		m_output_list.SetItemText(0, 4, strTemp);

		switch (product_register_value[702])
		{
		case 0:
			strTemp.Format(_T("%.2f inWC"), ((float)((short)product_register_value[704])) / 100.0);//100
			break;
		case 1:
			strTemp = L"kpal";
			break;
		case 2:
			strTemp = L"PSI";//10
			strTemp.Format(_T("%.1f PSI"), ((float)((short)product_register_value[704])) / 10.0);
			break;
		case 3:
			strTemp = L"mmHg";
			break;
		case 4:
			strTemp = L"inHg";
			break;
		case 5:
			strTemp = L"Kg/CM2";
			break;
		case 6:
			strTemp = L"atmosphere";
			break;
		case 7:
			strTemp = L"bar";
			break;
		default:
			strTemp = L"";
			break;
		}

		m_output_list.SetItemText(0, 5, strTemp);
		return;
	}
	m_output_list.InsertItem(0,_T("1"));
	m_output_list.InsertItem(1,_T("2"));
	m_output_list.InsertItem(2,_T("3"));
	m_output_list.SetItemText(0, 1,_T("Temperature"));
	m_output_list.SetItemText(1, 1,_T("Humidity"));

	if (m_product_model == PM_AirQuality)
	{
		m_output_list.SetItemText(2, 1,_T("Air Quanlity"));
	}

	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		if (product_register_value[485] == 1)
		{
			m_output_list.SetItemText(1, 1,_T("Dew Point"));
		}
		else
		{
			m_output_list.SetItemText(1, 1,_T("Humidity"));
		}
		if (ListCtrlEx::ComboBox == m_output_list.GetColumnType(1))
		{
			ListCtrlEx::CStrList strlist;
			strlist.push_back(_T("Humidity"));
			strlist.push_back(_T("Dew Point"));

			m_output_list.SetCellStringList(1, 1, strlist);
		}
	}

	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		AddressValue = 443;
		strTemp.Format(_T("%0.1f"), ((float)(short)(0 - product_register_value[AddressValue])) / 10.0);
		m_output_list.SetItemText(0, 4, strTemp);
		if ((product_register_value[7] == PM_CO2_RS485 && product_register_value[14] == 6) || product_register_value[7] == STM32_HUM_NET || product_register_value[7] == STM32_HUM_RS485)
		{
			AddressValue = 285;
			strTemp.Format(_T("%0.1f"), ((float)(short)(product_register_value[AddressValue])) / 10.0);
			m_output_list.SetItemText(0, 4, strTemp);
		}
	}
	else
	{
		AddressValue = MODBUS_TEM_RANGE_LOWER;
		strTemp.Format(_T("%0.1f"), (float)(short)product_register_value[AddressValue] / 10.0);
		m_output_list.SetItemText(0, 4, strTemp);
	}


	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		AddressValue = 443;
		strTemp.Format(_T("%0.1f"), (float)(short)(1000 - product_register_value[AddressValue]) / 10.0);
		m_output_list.SetItemText(0, 5, strTemp);
		if ((product_register_value[7] == PM_CO2_RS485 && product_register_value[14] == 6) || product_register_value[7] == STM32_HUM_NET || product_register_value[7] == STM32_HUM_RS485)

		{
			AddressValue = 286;
			strTemp.Format(_T("%0.1f"), ((float)(short)(product_register_value[AddressValue])) / 10.0);
			m_output_list.SetItemText(0, 5, strTemp);
		}
	}
	else
	{
		AddressValue = MODBUS_TEM_RANGE_UPPER;
		strTemp.Format(_T("%0.1f"), (float)(short)product_register_value[AddressValue] / 10.0);
		m_output_list.SetItemText(0, 5, strTemp);
	}


	strTemp.Format(_T("0"));
	m_output_list.SetItemText(1, 4, strTemp);
	strTemp.Format(_T("100"));
	m_output_list.SetItemText(1, 5, strTemp);

	if (m_product_model == PM_HUMTEMPSENSOR && product_register_value[485] == 1)
	{
		strTemp.Format(_T("%0.1f"), (float)(short)product_register_value[486] / 10.0);
		m_output_list.SetItemText(1, 4, strTemp);
		strTemp.Format(_T("%0.1f"), (float)(short)product_register_value[487] / 10.0);
		m_output_list.SetItemText(1, 5, strTemp);
	}
	if ((product_register_value[7] == PM_CO2_RS485 && product_register_value[14] == 6) || product_register_value[7] == STM32_HUM_NET || product_register_value[7] == STM32_HUM_RS485)

	{
		AddressValue = 287;
		strTemp.Format(_T("%0.1f"), ((float)(short)(product_register_value[AddressValue])) / 10.0);
		m_output_list.SetItemText(1, 4, strTemp);
	}
	if ((product_register_value[7] == PM_CO2_RS485 && product_register_value[14] == 6) || product_register_value[7] == STM32_HUM_NET || product_register_value[7] == STM32_HUM_RS485)

	{
		AddressValue = 288;
		strTemp.Format(_T("%0.1f"), ((float)(short)(product_register_value[AddressValue])) / 10.0);
		m_output_list.SetItemText(1, 5, strTemp);
	}
	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		AddressValue = 186;
	}
	else
	{
		AddressValue = MODBUS_OUTPUT_RANGE;
	}

	int output_range = product_register_value[AddressValue];
	if (output_range == 1)
	{
		strTemp = _T("0-10v");
	}
	else if (output_range == 2)
	{
		strTemp = _T("0-5v");
	}
	else if (output_range == 3)
	{
		strTemp = _T("4-20mA");
	}
	else
	{
		strTemp = _T("");
	}
	m_output_list.SetItemText(0, 3, strTemp);

	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		AddressValue = 187;
	}
	else
	{
		AddressValue = MODBUS_OUTPUT_RANGE;
	}

	output_range = product_register_value[AddressValue];
	if (output_range == 1)
	{
		strTemp = _T("0-10v");
	}
	else if (output_range == 2)
	{
		strTemp = _T("0-5v");
	}
	else if (output_range == 3)
	{
		strTemp = _T("4-20mA");
	}
	else
	{
		strTemp = _T("");
	}
	m_output_list.SetItemText(1, 3, strTemp);

	if (m_product_model == PM_AirQuality)
	{
		m_output_list.SetItemText(2, 3, strTemp);
	}

	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		if (product_register_value[485] != 1)
		{
			float offsetvalue, Vtemp, Vhum;
			float Temp, HUM;
			offsetvalue = ((float)((short)product_register_value[443]));
			if (m_product_model == PM_HUMTEMPSENSOR)
			{
				AddressValue = 370;
			}
			else
			{
				AddressValue = 349;
			}
			bitset<16> AM(product_register_value[AddressValue]);
			if (!AM[0])
			{
				m_output_list.SetItemText(0, 6,Global_String_AUTO);

				if (product_register_value[186] == 1)
				{
					Temp = ((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP]));
					//Vtemp = (Temp + offsetvalue) / 100;
                    //Output value Calculation formula.
                    Vtemp = ((Temp + offsetvalue - (short)product_register_value[285])) / ((short)product_register_value[286] - (short)product_register_value[285]) * 10;
					if (Vtemp > 10)
					{
						Vtemp = 10;
					}
					if (Vtemp < 0)
					{
						Vtemp = 0;
					}
					strTemp.Format(_T("%.2f v"), Vtemp);
				}
				else if (product_register_value[186] == 2)
				{
					Temp = ((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP]));
			
                    //Output Value Caluclation.
                    Vtemp = ((Temp + offsetvalue - (short)product_register_value[285]))/ ((short)product_register_value[286] - (short)product_register_value[285]) *5;
					if (Vtemp > 5)
					{
						Vtemp = 5;
					}
					if (Vtemp < 0)
					{
						Vtemp = 0;
					}
					strTemp.Format(_T("%.2f v"), Vtemp);
				}
				else if (product_register_value[186] == 3)
				{
					if (product_register_value[MODBUS_DEGC_OR_F] == 0)
					{
						Temp = ((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP]));
					}
					else
					{
						Temp = ((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP_F]));
					}
                    //Output value Calculation formula.
                    Vtemp = ((Temp + offsetvalue - (short)product_register_value[285]) / ((short)product_register_value[286] - (short)product_register_value[285])) * 16 + 4;
                    if (Vtemp > 20)
                    {
                        Vtemp = 20;
                    }
                    if (Vtemp < 4)
                    {
                        Vtemp = 4;
                    }
					strTemp.Format(_T("%.2f ma"), Vtemp);
				}
				else
				{
					strTemp = _T("0");
				}
			}
			else
			{
				m_output_list.SetItemText(0, 6,Global_String_MANUAL);
				Temp = ((float)((short)product_register_value[371]));
				if (product_register_value[186] == 1)
				{
					Vtemp = (Temp + offsetvalue) / 100;
					if (Vtemp > 10)
					{
						Vtemp = 10;
					}
					if (Vtemp < 0)
					{
						Vtemp = 0;
					}
					strTemp.Format(_T("%.2f v"), Vtemp);
				}
				else if (product_register_value[186] == 2)
				{
					Vtemp = (Temp + offsetvalue) / 200;
					if (Vtemp > 5)
					{
						Vtemp = 5;
					}
					if (Vtemp < 0)
					{
						Vtemp = 0;
					}
					strTemp.Format(_T("%.2f v"), Vtemp);
				}
				else if (product_register_value[186] == 3)
				{
					Vtemp = ((Temp + offsetvalue) / 1000) * 16 + 4;
					if (Vtemp > 20)
					{
						Vtemp = 20;
					}
					if (Vtemp < 4)
					{
						Vtemp = 4;
					}
					strTemp.Format(_T("%.2f ma"), Vtemp);
				}
				else
				{
					strTemp = _T("0");
				}
			}
			m_output_list.SetItemText(0, 2, strTemp);
			CString humsignal;
			if (!AM[1])
			{
				m_output_list.SetItemText(1, 6,Global_String_AUTO);
				HUM = ((float)((short)product_register_value[373])) / 10;
				if (product_register_value[187] == 1)
				{
					Vhum = HUM / 10;
					if (Vhum > 10)
					{
						Vhum = 10;
					}
					if (Vhum < 0)
					{
						Vhum = 0;
					}
					humsignal.Format(_T("%.2fv"), Vhum);
				}
				else if (product_register_value[187] == 2)
				{
					Vhum = HUM / 20;
					if (Vhum > 5)
					{
						Vhum = 5;
					}
					if (Vhum < 0)
					{
						Vhum = 0;
					}
					humsignal.Format(_T("%.2fv"), Vhum);
				}
				else if (product_register_value[187] == 3)
				{
					Vhum = (HUM / 100) * 16 + 4;
					if (Vhum > 20)
					{
						Vhum = 20;
					}
					if (Vhum < 4)
					{
						Vhum = 4;
					}
					humsignal.Format(_T("%.2fma"), Vhum);
				}
				else
				{
					humsignal = _T("0");
				}
			}
			else
			{
				m_output_list.SetItemText(1, 6,Global_String_MANUAL);
				HUM = ((float)((short)product_register_value[372])) / 10;
				if (product_register_value[187] == 1)
				{
					Vhum = HUM / 10;
					if (Vhum > 10)
					{
						Vhum = 10;
					}
					if (Vhum < 0)
					{
						Vhum = 0;
					}
					humsignal.Format(_T("%.2fv"), Vhum);
				}
				else if (product_register_value[187] == 2)
				{
					Vhum = HUM / 20;
					if (Vhum > 5)
					{
						Vhum = 5;
					}
					if (Vhum < 0)
					{
						Vhum = 0;
					}
					humsignal.Format(_T("%.2fv"), Vhum);
				}
				else if (product_register_value[187] == 3)
				{
					Vhum = (HUM / 100) * 16 + 4;
					if (Vhum > 20)
					{
						Vhum = 20;
					}
					if (Vhum < 4)
					{
						Vhum = 4;
					}
					humsignal.Format(_T("%.2fma"), Vhum);
				}
			}
			m_output_list.SetItemText(1, 2, humsignal);
		}
		else
		{
			float offsetvalue, Vtemp, Vhum;
			float Temp, HUM;
			offsetvalue = ((float)((short)product_register_value[443]));
			if (m_product_model == PM_HUMTEMPSENSOR)
			{
				AddressValue = 370;
			}
			else
			{
				AddressValue = 349;
			}
			bitset<16> AM(product_register_value[AddressValue]);
			if (!AM[0])
			{
				m_output_list.SetItemText(0, 6,Global_String_AUTO);

				if (product_register_value[186] == 1)
				{
					Temp = ((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP]));
					Vtemp = (Temp + offsetvalue) / 100;
					if (Vtemp > 10)
					{
						Vtemp = 10;
					}
					if (Vtemp < 0)
					{
						Vtemp = 0;
					}
					strTemp.Format(_T("%.2f v"), Vtemp);
				}
				else if (product_register_value[186] == 2)
				{
					Temp = ((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP]));
					Vtemp = (Temp + offsetvalue) / 200;
					if (Vtemp > 5)
					{
						Vtemp = 5;
					}
					if (Vtemp < 0)
					{
						Vtemp = 0;
					}
					strTemp.Format(_T("%.2f v"), Vtemp);
				}
				else if (product_register_value[186] == 3)
				{
					if (product_register_value[MODBUS_DEGC_OR_F] == 0)
					{
						Temp = ((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP]));
						Vtemp = ((Temp + offsetvalue) / 1000) * 16 + 4;
						if (Vtemp > 20)
						{
							Vtemp = 20;
						}
						if (Vtemp < 4)
						{
							Vtemp = 4;
						}
					}
					else
					{
						Temp = ((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP_F]));
						Vtemp = ((Temp + offsetvalue) / 1000) * 16 + 4;
						if (Vtemp > 20)
						{
							Vtemp = 20;
						}
						if (Vtemp < 4)
						{
							Vtemp = 4;
						}
					}
					strTemp.Format(_T("%.2f ma"), Vtemp);
				}
				else
				{
					strTemp = _T("0");
				}
			}
			else
			{
				m_output_list.SetItemText(0, 6,Global_String_MANUAL);


				Temp = ((float)((short)product_register_value[371]));
				if (product_register_value[186] == 1)
				{
					Vtemp = (Temp + offsetvalue) / 100;
					if (Vtemp > 10)
					{
						Vtemp = 10;
					}
					if (Vtemp < 0)
					{
						Vtemp = 0;
					}
					strTemp.Format(_T("%.2f v"), Vtemp);
				}
				else if (product_register_value[186] == 2)
				{
					Vtemp = (Temp + offsetvalue) / 200;
					if (Vtemp > 5)
					{
						Vtemp = 5;
					}
					if (Vtemp < 0)
					{
						Vtemp = 0;
					}
					strTemp.Format(_T("%.2f v"), Vtemp);
				}
				else if (product_register_value[186] == 3)
				{
					Vtemp = ((Temp + offsetvalue) / 1000) * 16 + 4;
					if (Vtemp > 20)
					{
						Vtemp = 20;
					}
					if (Vtemp < 4)
					{
						Vtemp = 4;
					}
					strTemp.Format(_T("%.2f ma"), Vtemp);
				}
				else
				{
					strTemp = _T("0");
				}
			}
			m_output_list.SetItemText(0, 2, strTemp);

			CString humsignal;


			m_output_list.SetItemText(1, 6,NO_APPLICATION);
			HUM = ((float)((short)product_register_value[482])) / 10;
			if (product_register_value[187] == 1)
			{
				Vhum = HUM / 10;
				if (Vhum > 10)
				{
					Vhum = 10;
				}
				if (Vhum < 0)
				{
					Vhum = 0;
				}
				humsignal.Format(_T("%.2fv"), Vhum);
			}
			else if (product_register_value[187] == 2)
			{
				Vhum = HUM / 20;
				if (Vhum > 5)
				{
					Vhum = 5;
				}
				if (Vhum < 0)
				{
					Vhum = 0;
				}
				humsignal.Format(_T("%.2fv"), Vhum);
			}
			else if (product_register_value[187] == 3)
			{
				Vhum = (HUM / 100) * 16 + 4;
				if (Vhum > 20)
				{
					Vhum = 20;
				}
				if (Vhum < 4)
				{
					Vhum = 4;
				}
				humsignal.Format(_T("%.2fma"), Vhum);
			}


			m_output_list.SetItemText(1, 2, humsignal);
		}
	}
	else
	{
		if (output_range == 1 || output_range == 2)
		{
			strTemp.Format(_T("%0.2f V"), (float)(short)product_register_value[MODBUS_TEMP_VOLTAGE_OUTPUT] / 100.0);
			m_output_list.SetItemText(0, 2, strTemp);
			strTemp.Format(_T("%0.2f V"), (float)(short)product_register_value[MODBUS_HUM_VOLTAGE_OUTPUT] / 100.0);
			m_output_list.SetItemText(1, 2, strTemp);
		}
		else
		{
			strTemp.Format(_T("%0.1f mA"), (float)(short)product_register_value[MODBUS_TEMP_CURRENT_OUTPUT] / 10.0);
			m_output_list.SetItemText(0, 2, strTemp);
			strTemp.Format(_T("%0.1f mA"), (float)(short)product_register_value[MODBUS_HUM_CURRENT_OUTPUT] / 10.0);
			m_output_list.SetItemText(1, 2, strTemp);
		}
	}


	if (m_product_model == PM_AirQuality)
	{
		strTemp.Format(_T("%d"), product_register_value[MODBUS_AQ_RANGE_LOWER]);
		m_output_list.SetItemText(2, 4, strTemp);
		strTemp.Format(_T("%d"), product_register_value[MODBUS_AQ_RANGE_UPPER]);
		m_output_list.SetItemText(2, 5, strTemp);
		if (output_range == 1 || output_range == 2)
		{
			strTemp.Format(_T("%0.2f V"), (float)(short)product_register_value[MODBUS_AQ_VOLTAGE_OUTPUT] / 100.0);
			m_output_list.SetItemText(2, 2, strTemp);
		}
		else
		{
			strTemp.Format(_T("%0.1f mA"), (float)(short)product_register_value[MODBUS_AQ_CURRENT_OUTPUT] / 10.0);
			m_output_list.SetItemText(2, 2, strTemp);
		}
	}
	if (product_register_value[7] != STM32_PM25)
		m_output_list.ShowWindow(SW_SHOW);
}


LRESULT CAirQuality::InputMessageCallBack_AQ(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}
/// <summary>
/// Fresh_Input_List_AQ
/// </summary>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
LRESULT CAirQuality::Fresh_Input_List_AQ(WPARAM wParam, LPARAM lParam)
{
	CString strTemp;
	int AddressValue = -1;
	if (m_list_type == 0)
	{
		Initial_InputList();
	}
	else if (m_list_type == 1)
	{
		Initial_OutputList();
	}

	return 0;
}
/// <summary>
/// OnNMClickList1_AQ
/// </summary>
/// <param name="pNMHDR"></param>
/// <param name="pResult"></param>
void CAirQuality::OnNMClickList1_AQ(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int AddressValue = -1;
	CString temp_cstring;
	long lRow, lCol;
	m_input_list.Set_Edit(true);
	DWORD dwPos = GetMessagePos();//Get which line is clicked by user. Set the check box, when user enter Insert it will jump to dialog
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	m_input_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;
	int nItem = m_input_list.SubItemHitTest(&lvinfo);

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;

	if (lRow > m_input_list.GetItemCount()) //If the area hit exceeds the maximum line number the click is invalid
		return;
	if (lRow < 0)
		return;
	CString New_CString;
	CString temp_task_info;

	CString temp1;
	CStringArray temparray;

	if (lCol == 2) //A/M
	{
		if (m_product_model == STM32_PRESSURE_NET)
		{
			AddressValue = 708;
			if (product_register_value[708] == 1)
			{
				write_one(g_tstat_id, AddressValue, 0);
				product_register_value[708] = read_one(g_tstat_id, AddressValue);
			}
			else
			{
				write_one(g_tstat_id, AddressValue, 1);
				product_register_value[708] = read_one(g_tstat_id, AddressValue);
			}

			if (product_register_value[708] == 1)
			{
				m_input_list.SetItemText(0, 2, L"Manual");
			}
			else
			{
				m_input_list.SetItemText(0, 2, L"Auto");
			}
			return;
		}
		if (m_product_model == PM_HUMTEMPSENSOR)
		{
			AddressValue = 370;
		}
		else
		{
			AddressValue = 349;
		}

		bitset<16> AM(product_register_value[AddressValue]);
		if (AM[lRow])
		{
			AM[lRow] = FALSE;
		}
		else
		{
			AM[lRow] = TRUE;
		}
		int AM_Int = AM.to_ulong();
		if (product_register_value[AddressValue] != AM_Int)
		{
			int ret = write_one(m_current_TstatID, AddressValue, AM_Int);
			if (ret > 0)
			{
				product_register_value[AddressValue] = AM_Int;


				PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
			}
		}
	}
	if (lCol == 4)
	{
		if (lRow == 0)
		{
			if (m_product_model == PM_HUMTEMPSENSOR)
			{
				AddressValue = 121;
			}
			else
			{
				AddressValue = MODBUS_DEGC_OR_F;
			}

			if (product_register_value[AddressValue] == 0)
			{
				int ret = write_one(m_current_TstatID, AddressValue, 1);
				if (ret > 0)
				{
					product_register_value[AddressValue] = 1;
				}
				PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
			}
			else
			{
				int ret = write_one(m_current_TstatID, AddressValue, 0);
				if (ret > 0)
				{
					product_register_value[AddressValue] = 0;
				}
				PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
			}
		}
	}


	AddressValue = -1;
	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		AddressValue = 100;
	}
	else
	{
		AddressValue = MODBUS_TEMPRATURE_CHIP_F;
	}

	Read_Multi(m_current_TstatID, &product_register_value[AddressValue], AddressValue, 4, 5);
	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		AddressValue = 188;
	}
	else
	{
		AddressValue = MODBUS_OUTPUT_RANGE;
	}

	Read_Multi(m_current_TstatID, &product_register_value[AddressValue], AddressValue, 4, 5);
	if (m_product_model == PM_HUMTEMPSENSOR)
	{
		AddressValue = 370;
	}
	else
	{
		AddressValue = MODBUS_TEMP_CURRENT_OUTPUT;
	}


	Read_Multi(m_current_TstatID, &product_register_value[AddressValue], AddressValue, 6, 5);
	PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);

	m_list_type = 0;
}
/// <summary>
/// OnNMClickList2_AQ
/// </summary>
/// <param name="pNMHDR"></param>
/// <param name="pResult"></param>
void CAirQuality::OnNMClickList2_AQ(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CString temp_cstring;
	long lRow, lCol;
	m_output_list.Set_Edit(true);
	DWORD dwPos = GetMessagePos();
	//Get which line has been clicked by user.
     //Set the check box, when user hits Insert jump to dialog
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	m_output_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;
	int nItem = m_output_list.SubItemHitTest(&lvinfo);
	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;
	m_list_type = 1;

	if (lRow == 0 && lCol == 1)
	{
		m_output_list.Set_Edit(false);
		return;
	}
}
/// <summary>
/// 
/// </summary>
/// <param name="pNMHDR"></param>
/// <param name="pResult"></param>
void CAirQuality::OnNMClickList_User(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CString temp_cstring;
	long lRow, lCol;
	m_factory_list.Set_Edit(true);
	DWORD dwPos = GetMessagePos();//Get which line has been clicked
      					//Set the check box, when user hits Insert jump to the dialog
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	m_factory_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;
	int nItem = m_factory_list.SubItemHitTest(&lvinfo);

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;


	if (lRow > m_factory_list.GetItemCount()) //If the hit area exceeds the maximum line number, the click is invalid
		return;
	if (lRow < 0)
		return;
	// 	if (lCol == 2)
	// 	{
	// 		if (product_register_value[MODBUS_HUM_CURRENT_DEFAULT] == 1)
	// 		{
	// 			m_list_type = 3;
	// 		}
	// 		else
	// 		{
	// 			m_factory_list.Set_Edit(false);
	// 			return;
	// 		}
	// 	}
}
/// <summary>
/// Fresh_Input_Item_AQ
/// </summary>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
LRESULT CAirQuality::Fresh_Input_Item_AQ(WPARAM wParam, LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;
	int AddressValue = -1;
	CString temp_task_info;
	if (m_list_type == 0)
	{
		CString New_CString = m_input_list.GetItemText(Changed_Item, Changed_SubItem);
		CString cstemp_value;


		//Value
		if (Changed_SubItem == 3)//Value
		{
			CString strTemp;
			int ItemValue = (int)(_wtof(New_CString) * 10);
			int IntValue = _wtoi(New_CString);
			if (m_product_model == STM32_PRESSURE_NET)
			{
				if (product_register_value[708] == 1)
				{
					if (product_register_value[702] == 0)
					{
						write_one(g_tstat_id, 709, (int)(_wtof(New_CString) * 100));
					}
					else
					{
						write_one(g_tstat_id, 709, ItemValue);
					}
				}
				else
				{
					if (product_register_value[702] == 0)
					{
						write_one(g_tstat_id, 711, (int)(_wtof(New_CString) * 100));
					}
					else
					{
						write_one(g_tstat_id, 711, ItemValue);
					}
				}

				return 0;
			}
			if (m_product_model == PM_HUMTEMPSENSOR)
			{
				AddressValue = 370;
			}
			else
			{
				AddressValue = 349;
			}

			bitset<16> AM(product_register_value[AddressValue]);
			if (!AM[Changed_Item])//Auto
			{
				if (Changed_Item == 0)//
				{
					if (m_product_model == PM_HUMTEMPSENSOR)
					{
						AddressValue = 121;
					}
					else
					{
						AddressValue = MODBUS_DEGC_OR_F;
					}

					if (product_register_value[AddressValue] == 0)
					{
						if (m_product_model == PM_HUMTEMPSENSOR)
						{
							AddressValue = 101;
						}
						else
						{
							AddressValue = MODBUS_TEMPRATURE_CHIP;
						}

						if (product_register_value[AddressValue] != ItemValue)
						{
							int ret = write_one(m_current_TstatID, AddressValue, ItemValue);
							if (ret > 0)
							{
								product_register_value[AddressValue] = ItemValue;
							}
						}
					}
					else
					{
						if (m_product_model == PM_HUMTEMPSENSOR)
						{
							AddressValue = 100;
						}
						else
						{
							AddressValue = MODBUS_TEMPRATURE_CHIP_F;
						}

						if (product_register_value[AddressValue] != ItemValue)
						{
							int ret = write_one(m_current_TstatID, AddressValue, ItemValue);
							if (ret > 0)
							{
								product_register_value[AddressValue] = ItemValue;
							}
						}
					}
				}

				if (Changed_Item == 1)
				{
					if (m_product_model == PM_HUMTEMPSENSOR)
					{
						AddressValue = 373;
					}
					else
					{
						AddressValue = MODUBS_HUMIDITY_RH;
					}

					if (product_register_value[AddressValue] != ItemValue)
					{
						int ret = write_one(m_current_TstatID, AddressValue, ItemValue);
						if (ret > 0)
						{
							product_register_value[AddressValue] = ItemValue;
						}
					}
				}

				if (Changed_Item == 2)
				{
					if (product_register_value[MODBUS_AQ] != ItemValue)
					{
						int ret = write_one(m_current_TstatID, MODBUS_AQ, ItemValue);
						if (ret > 0)
						{
							product_register_value[MODBUS_AQ] = ItemValue;
						}
					}
				}
			}
			else//Manual
			{
				if (Changed_Item == 0)
				{
					if (m_product_model == PM_HUMTEMPSENSOR)
					{
						AddressValue = 371;
					}
					else
					{
						AddressValue = 350;
					}

					if (product_register_value[AddressValue] != ItemValue)
					{
						int ret = write_one(m_current_TstatID, AddressValue, ItemValue);
						if (ret > 0)
						{
							product_register_value[AddressValue] = ItemValue;
						}
					}
				}

				if (Changed_Item == 1)
				{
					if (m_product_model == PM_HUMTEMPSENSOR)
					{
						AddressValue = 372;
					}
					else
					{
						AddressValue = 351;
					}

					if (product_register_value[AddressValue] != ItemValue)
					{
						int ret = write_one(m_current_TstatID, AddressValue, ItemValue);
						if (ret > 0)
						{
							product_register_value[AddressValue] = ItemValue;
						}
					}
				}

				if (Changed_Item == 2)
				{
					if (m_product_model == PM_HUMTEMPSENSOR)
					{
						AddressValue = 372;
					}
					else
					{
						AddressValue = 351;
					}


					if (product_register_value[AddressValue] != IntValue)
					{
						int ret = write_one(m_current_TstatID, AddressValue, IntValue);
						if (ret > 0)
						{
							product_register_value[AddressValue] = IntValue;
						}
					}
				}
			}

			if (m_product_model == PM_HUMTEMPSENSOR && Changed_Item == 2)
			{
				/*strTemp.Format(_T("%d"), product_register_value[537]);
				m_input_list.SetItemText(2, 3, strTemp);*/
				int ret = write_one(g_tstat_id, 538, IntValue);
				if (ret > 0)
				{
					product_register_value[538] = IntValue;
				}
				strTemp.Format(_T("%d"), product_register_value[538]);
				m_input_list.SetItemText(2, 3, strTemp);
			}
		}
		//Filter
		if (Changed_SubItem == 6)
		{
			CString strTemp;
			int ItemValue = _wtoi(New_CString);

			if (Changed_Item == 0)
			{
				if (m_product_model == STM32_PRESSURE_NET)
				{
					write_one(g_tstat_id, 705, ItemValue);

					return 0;
				}


				if (m_product_model == PM_HUMTEMPSENSOR)
				{
					AddressValue = 452;
				}
				else
				{
					AddressValue = MODBUS_OVERRIDE_TIMER_LEFT;
				}

				if (product_register_value[AddressValue] != ItemValue)
				{
					int ret = write_one(m_current_TstatID, AddressValue, ItemValue);
					if (ret > 0)
					{
						product_register_value[AddressValue] = ItemValue;
					}
				}
			}
			if (Changed_Item == 1)
			{
				if (m_product_model == PM_HUMTEMPSENSOR)
				{
					AddressValue = 453;
				}
				else
				{
					AddressValue = MODBUS_HUM_FILTER;
				}

				if (product_register_value[AddressValue] != ItemValue)
				{
					int ret = write_one(m_current_TstatID, AddressValue, ItemValue);
					if (ret > 0)
					{
						product_register_value[AddressValue] = ItemValue;
					}
				}
			}
			if (Changed_Item == 2)
			{
				if (product_register_value[MODBUS_AQ_FILTER] != ItemValue)
				{
					int ret = write_one(m_current_TstatID, MODBUS_AQ_FILTER, ItemValue);
					if (ret > 0)
					{
						product_register_value[MODBUS_AQ_FILTER] = ItemValue;
					}
				}

				if (m_product_model == PM_HUMTEMPSENSOR)
				{
					/*strTemp.Format(_T("%d"), product_register_value[537]);
                            /TBD: Get rid of the hard coded register numbers
					m_input_list.SetItemText(2, 3, strTemp);*/
					int ret = write_one(g_tstat_id, 536, ItemValue);
					if (ret > 0)
					{
						product_register_value[536] = ItemValue;
					}
					strTemp.Format(_T("%d"), product_register_value[536]);
					m_input_list.SetItemText(2, 6, strTemp);
				}
			}
		}
		//Calibration
		if (Changed_SubItem == 5)
		{
			CString strTemp;
			int ItemValue = (int)(_wtof(New_CString) * 10);

			if (Changed_Item == 0)
			{
				if (m_product_model == STM32_PRESSURE_NET)
				{
					if (product_register_value[774] == 0)
					{
						write_one(g_tstat_id, 712, ItemValue);
					}
					else
					{
						write_one(g_tstat_id, 770, ItemValue);
					}
					return 0;
				}
				if (m_product_model == PM_HUMTEMPSENSOR)
				{
					AddressValue = 450;
				}
				else
				{
					AddressValue = 104;
					ItemValue += 500;
				}


				if (product_register_value[AddressValue] != ItemValue)
				{
					int ret = write_one(m_current_TstatID, AddressValue, ItemValue);
					if (ret > 0)
					{
						product_register_value[AddressValue] = ItemValue;
					}
				}
			}
			if (Changed_Item == 1)
			{
				if (m_product_model == PM_HUMTEMPSENSOR)
				{
					//AddressValue =451;
					if (product_register_value[454] == 0)
					{
						AddressValue = 479;
					}
					else
					{
						AddressValue = 451;
					}
				}
				else
				{
					if (product_register_value[309] == 0)
					{
						AddressValue = 107;
					}
					else
					{
						AddressValue = 106;
					}
					ItemValue += 500;
				}

				if (product_register_value[AddressValue] != ItemValue)
				{
					int ret = write_one(m_current_TstatID, AddressValue, ItemValue);
					if (ret > 0)
					{
						product_register_value[AddressValue] = ItemValue;
					}
				}
			}
			if (Changed_Item == 2)
			{
				if (product_register_value[105] != ItemValue)
				{
					int ret = write_one(m_current_TstatID, 105, ItemValue);
					if (ret > 0)
					{
						product_register_value[105] = ItemValue;
					}
				}
				if (m_product_model == PM_HUMTEMPSENSOR)
				{
					int ret = write_one(g_tstat_id, 540, ItemValue);
					if (ret > 0)
					{
						product_register_value[540] = ItemValue;
					}
					strTemp.Format(_T("%d"), product_register_value[540]);
					m_input_list.SetItemText(2, 5, strTemp);
				}
			}
		}
		/*if (Changed_SubItem ==6 )
		{
			if (m_product_model == PM_HUMTEMPSENSOR)
			{
				int ItemValue = _wtoi(New_CString);

				if (Changed_Item == 0)
				{

					AddressValue =452;



					if (product_register_value[AddressValue]!=ItemValue)
					{
						int ret = write_one(m_current_TstatID,AddressValue,ItemValue);
						if (ret> 0)
						{
							product_register_value[AddressValue]=ItemValue;
						}
					}

				}
				if (Changed_Item == 1)
				{

					AddressValue =453;


					if (product_register_value[AddressValue]!=ItemValue)
					{
						int ret = write_one(m_current_TstatID,AddressValue,ItemValue);
						if (ret> 0)
						{
							product_register_value[AddressValue]=ItemValue;
						}
					}

				}
			}
		}*/

		AddressValue = -1;
		if (m_product_model == PM_HUMTEMPSENSOR)
		{
			AddressValue = 100;
		}
		else
		{
			AddressValue = MODBUS_TEMPRATURE_CHIP_F;
		}

		Read_Multi(m_current_TstatID, &product_register_value[AddressValue], AddressValue, 10, 5);

		AddressValue = -1;
		if (m_product_model == PM_HUMTEMPSENSOR)
		{
			AddressValue = 450;
		}
		else
		{
			AddressValue = MODBUS_TEMPRATURE_CHIP_F;
		}
		Read_Multi(m_current_TstatID, &product_register_value[AddressValue], AddressValue, 10, 5);
		PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
	}
	else if (m_list_type == 1)
	{
		CString New_CString = m_output_list.GetItemText(Changed_Item, Changed_SubItem);
		if (m_product_model == PM_HUMTEMPSENSOR)
		{
			if (Changed_SubItem == 1)
			{
				if (Changed_Item == 1)
				{
					if (New_CString.CompareNoCase(_T("Dew Point")) == 0)
					{
						int ret = write_one(m_current_TstatID, 485, 1);
						if (ret > 0)
						{
							product_register_value[485] = 1;
						}
					}
					else
					{
						int ret = write_one(m_current_TstatID, 485, 0);
						if (ret > 0)
						{
							product_register_value[485] = 0;
						}
					}
				}
				else
				{
					m_output_list.Set_Edit(false);
					return 0;
				}
			}
		}

		int ItemValue = (int)(_wtof(New_CString) * 10);
		if (m_product_model == PM_HUMTEMPSENSOR)
		{
			if ((product_register_value[7] == PM_CO2_RS485 && product_register_value[14] == 6) || product_register_value[7] == STM32_HUM_NET || product_register_value[7] == STM32_HUM_RS485)

			{
				if (Changed_SubItem == 4)
				{
					if (Changed_Item == 0) //MODBUS_TEM_RANGE_LOWER
					{
						if (product_register_value[285] != ItemValue)
						{
							int ret = write_one(m_current_TstatID, 285, ItemValue);
							if (ret > 0)
							{
								product_register_value[285] = ItemValue;
								PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
							}
						}
					}
					else
					{
						if (product_register_value[287] != ItemValue)
						{
							int ret = write_one(m_current_TstatID, 287, ItemValue);
							if (ret > 0)
							{
								product_register_value[287] = ItemValue;
								PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
							}
						}
					}
				}
				if (Changed_SubItem == 5)
				{
					if (Changed_Item == 0) //MODBUS_TEM_RANGE_LOWER
					{
						if (product_register_value[286] != ItemValue)
						{
							int ret = write_one(m_current_TstatID, 286, ItemValue);
							if (ret > 0)
							{
								product_register_value[286] = ItemValue;
								PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
							}
						}
						else
						{
							m_output_list.Set_Edit(false);
							return 0;
						}
					}
					else
					{
						if (product_register_value[288] != ItemValue)
						{
							int ret = write_one(m_current_TstatID, 288, ItemValue);
							if (ret > 0)
							{
								product_register_value[288] = ItemValue;
								PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
							}
						}
					}
				}
			}
			else
			{
				if (Changed_SubItem == 4)
				{
					ItemValue = 0 - ItemValue;

					if (Changed_Item == 0) //MODBUS_TEM_RANGE_LOWER
					{
						if (product_register_value[443] != ItemValue)
						{
							int ret = write_one(m_current_TstatID, 443, ItemValue);
							if (ret > 0)
							{
								product_register_value[443] = ItemValue;
								PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
							}
						}
					}
					else
					{
						ItemValue = 0 - ItemValue;
						if (product_register_value[485] == 1)
						{
							if (product_register_value[486] != ItemValue)
							{
								int ret = write_one(m_current_TstatID, 486, ItemValue);
								if (ret > 0)
								{
									product_register_value[486] = ItemValue;
									PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
								}
							}
						}
						else
						{
							m_output_list.Set_Edit(false);
							return 0;
						}
					}
				}
				if (Changed_SubItem == 5)
				{
					if (Changed_Item == 0) //MODBUS_TEM_RANGE_LOWER
					{
						ItemValue = 1000 - ItemValue;
						if (product_register_value[443] != ItemValue)
						{
							int ret = write_one(m_current_TstatID, 443, ItemValue);
							if (ret > 0)
							{
								product_register_value[443] = ItemValue;
								PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
							}
						}
						else
						{
							m_output_list.Set_Edit(false);
							return 0;
						}
					}
					else
					{
						if (product_register_value[485] == 1)
						{
							if (product_register_value[487] != ItemValue)
							{
								int ret = write_one(m_current_TstatID, 487, ItemValue);
								if (ret > 0)
								{
									product_register_value[487] = ItemValue;
									PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
								}
							}
						}
						else
						{
							m_output_list.Set_Edit(false);
							return 0;
						}
					}
				}
			}
		}
		else
		{
			if (Changed_SubItem == 4)
			{
				if (Changed_Item == 0) //MODBUS_TEM_RANGE_LOWER
				{
					if (m_product_model == STM32_PRESSURE_NET)
					{
						MODBUS_TEM_RANGE_LOWER = 703;
						if (product_register_value[702] == 0)
						{
							ItemValue = (int)(_wtof(New_CString) * 100);
						}
					}

					if (((short)product_register_value[MODBUS_TEM_RANGE_LOWER]) != ItemValue)
					{
						int ret = write_one(m_current_TstatID, MODBUS_TEM_RANGE_LOWER, (unsigned short)ItemValue);
						if (ret > 0)
						{
							product_register_value[MODBUS_TEM_RANGE_LOWER] = ItemValue;
							PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
						}
					}
				}
				if (Changed_Item == 1)
				{
					if (product_register_value[MODBUS_HUM_RANGE_LOWER] != ItemValue)
					{
						int ret = write_one(m_current_TstatID, MODBUS_HUM_RANGE_LOWER, ItemValue);
						if (ret > 0)
						{
							product_register_value[MODBUS_HUM_RANGE_LOWER] = ItemValue;
							PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
						}
					}
				}
				if (Changed_Item == 2)
				{
					if (product_register_value[MODBUS_AQ_RANGE_LOWER] != ItemValue)
					{
						int ret = write_one(m_current_TstatID, MODBUS_AQ_RANGE_LOWER, ItemValue);
						if (ret > 0)
						{
							product_register_value[MODBUS_AQ_RANGE_LOWER] = ItemValue;
							PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
						}
					}
				}
			}

			if (Changed_SubItem == 5)
			{
				if (Changed_Item == 0) //MODBUS_TEM_RANGE_LOWER
				{
					if (m_product_model == STM32_PRESSURE_NET)
					{
						MODBUS_TEM_RANGE_UPPER = 704;

						if (product_register_value[702] == 0)
						{
							ItemValue = (int)(_wtof(New_CString) * 100);
						}
					}

					if (product_register_value[MODBUS_TEM_RANGE_UPPER] != ItemValue)
					{
						int ret = write_one(m_current_TstatID, MODBUS_TEM_RANGE_UPPER, (unsigned short)ItemValue);
						if (ret > 0)
						{
							product_register_value[MODBUS_TEM_RANGE_UPPER] = ItemValue;
							PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
						}
					}
				}
				if (Changed_Item == 1)
				{
					if (product_register_value[MODBUS_HUM_RANGE_UPPER] != ItemValue)
					{
						int ret = write_one(m_current_TstatID, MODBUS_HUM_RANGE_UPPER, ItemValue);
						if (ret > 0)
						{
							product_register_value[MODBUS_HUM_RANGE_UPPER] = ItemValue;
							PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
						}
					}
				}
				if (Changed_Item == 2)
				{
					if (product_register_value[MODBUS_AQ_RANGE_UPPER] != ItemValue)
					{
						int ret = write_one(m_current_TstatID, MODBUS_AQ_RANGE_UPPER, ItemValue);
						if (ret > 0)
						{
							product_register_value[MODBUS_AQ_RANGE_UPPER] = ItemValue;
							PostMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
						}
					}
				}
			}
		}
	}
	else
	{
		if (m_product_model == PM_HUMTEMPSENSOR)
		{
			if (Changed_SubItem == 2)
			{
				m_fresh_data = FALSE;
				CString New_CString = m_user_list.GetItemText(Changed_Item, Changed_SubItem);
				int ItemValue = (int)(_wtof(New_CString) * 10);
				int RegAddress;


				if (New_CString.IsEmpty())
				{
					Initial_UserList();
					return FALSE;
				}


				RegAddress = 456 + 2 * Changed_Item;

				if (product_register_value[RegAddress] == ItemValue) //Add this to judge weather this value need to change.
				{
					Initial_UserList();
					return FALSE;
				}

				int ret = write_one(m_current_TstatID, RegAddress, ItemValue);
				if (ret > 0)
				{
					product_register_value[RegAddress] = ItemValue;
				}
				else
				{
					AfxMessageBox(_T("Write Fail!"));
					m_user_list.Set_Edit(false);
					return false;
				}


				unsigned short Data[20];
				ret = Read_Multi(m_current_TstatID, Data, 456, 20);
				if (ret > 0)
				{
					for (int i = 0; i < 20; i++)
					{
						product_register_value[456 + i] = Data[i];
					}
				}
				Initial_UserList();

				InitChartData();
				//				UpdatePoint();
			}
		}
		else
		{
			if (Changed_SubItem == 2)
			{
				m_fresh_data = FALSE;
				CString New_CString = m_user_list.GetItemText(Changed_Item, Changed_SubItem);
				int ItemValue = (int)(_wtof(New_CString) * 10);
				int RegAddress;


				if (New_CString.IsEmpty())
				{
					Initial_UserList();
					return FALSE;
				}


				RegAddress = MODBUS_HUMCOUNT1_H + 2 * Changed_Item + 1;

				if (product_register_value[RegAddress] == ItemValue) //Check if the value has changed
				{
					m_fresh_data = TRUE;
					Initial_UserList();
					return FALSE;
				}

				int ret = write_one(m_current_TstatID, RegAddress, ItemValue);
				if (ret > 0)
				{
					product_register_value[RegAddress] = ItemValue;
				}
				else
				{
					AfxMessageBox(_T("Write Failed!"));
					m_user_list.Set_Edit(false);
					return false;
				}


				unsigned short Data[20];
				ret = Read_Multi(m_current_TstatID, Data, MODBUS_HUMCOUNT1_H, 20);
				if (ret > 0)
				{
					for (int i = 0; i < 20; i++)
					{
						product_register_value[MODBUS_HUMCOUNT1_H + i] = Data[i];
					}
				}
				Initial_UserList();

				InitChartData();
				//				UpdatePoint();
			}
		}
	}

	return 0;
}
/// <summary>
/// Fresh_Input_Output
/// </summary>
void CAirQuality::Fresh_Input_Output()
{
	CString strTemp;
	int AddressValue = -1;
#if 1 // Input list fresh
	Initial_InputList();
#endif

#if 1  //Fresh output list
	Initial_OutputList();

	Initial_UserList();

#endif
}
