﻿// TstatAQ.cpp : 实现文件 - Implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "TstatAQ.h"
#include "MainFrm.h"
#include "CTstatAQ_PM25_Parameter.h"
// CTstatAQ
//CString  show_fan_picture = _T("C:\\temp\\fan\\fan_0.png");; //
#define     WM_TSTAT_AQ_THREAD_READ                     WM_USER + 502
HANDLE h_tstat_aq_thread = NULL;
static int aqi_level = 0;
IMPLEMENT_DYNCREATE(CTstatAQ, CFormView)

CTstatAQ::CTstatAQ()
	: CFormView(IDD_DIALOG_TSTAT_AQ)
{

}

CTstatAQ::~CTstatAQ()
{
}

void CTstatAQ::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_AQI_INFO, m_aqi_title);
    DDX_Control(pDX, IDC_STATIC_AQI, m_static_info);
    DDX_Control(pDX, IDC_LIST_AIRLAB, m_airlab_list);
    DDX_Control(pDX, IDC_DATETIMEPICKER_TIME_REMAIN, m_time_trigger);
}

BEGIN_MESSAGE_MAP(CTstatAQ, CFormView)
    ON_MESSAGE(WM_TSTAT_AQ_THREAD_READ, UpdateUI)
    ON_MESSAGE(WM_LIST_ITEM_CHANGED, Fresh_Airlab_Item)
    ON_MESSAGE(MY_RESUME_DATA, AirlabMessageCallBack)
    ON_EN_KILLFOCUS(IDC_EDIT_CO2_ON_TIME, &CTstatAQ::OnEnKillfocusEditCo2OnTime)
    ON_EN_KILLFOCUS(IDC_EDIT_CO2_OFF_TIME, &CTstatAQ::OnEnKillfocusEditCo2OffTime)
    ON_EN_KILLFOCUS(IDC_EDIT_PM_ON_TIME, &CTstatAQ::OnEnKillfocusEditPmOnTime)
    ON_EN_KILLFOCUS(IDC_EDIT_PM_OFF_TIME, &CTstatAQ::OnEnKillfocusEditPmOffTime)
    ON_BN_CLICKED(IDC_RADIO_DEG_C, &CTstatAQ::OnBnClickedRadioDegC)
    ON_BN_CLICKED(IDC_RADIO_DEG_F, &CTstatAQ::OnBnClickedRadioDegF)
    ON_CBN_SELCHANGE(IDC_COMBO_AQI_REGION, &CTstatAQ::OnCbnSelchangeComboAqiRegion)
    ON_EN_KILLFOCUS(IDC_EDIT_LEVEL_1, &CTstatAQ::OnEnKillfocusEditLevel1)
    ON_EN_KILLFOCUS(IDC_EDIT_LEVEL_2, &CTstatAQ::OnEnKillfocusEditLevel2)
    ON_EN_KILLFOCUS(IDC_EDIT_LEVEL_3, &CTstatAQ::OnEnKillfocusEditLevel3)
    ON_EN_KILLFOCUS(IDC_EDIT_LEVEL_4, &CTstatAQ::OnEnKillfocusEditLevel4)
    ON_EN_KILLFOCUS(IDC_EDIT_LEVEL_5, &CTstatAQ::OnEnKillfocusEditLevel5)
    ON_BN_CLICKED(IDC_BUTTON_CUS_AQI, &CTstatAQ::OnBnClickedButtonCusAqi)
    ON_BN_CLICKED(IDC_BUTTON_AUTO_CAL, &CTstatAQ::OnBnClickedButtonAutoCal)
    ON_WM_TIMER()
    ON_NOTIFY(NM_CLICK, IDC_LIST_AIRLAB, &CTstatAQ::OnNMClickListAirlab)
    ON_NOTIFY(NM_KILLFOCUS, IDC_DATETIMEPICKER_TIME_REMAIN, &CTstatAQ::OnNMKillfocusDatetimepickerTimeRemain)
    ON_BN_CLICKED(IDC_BUTTON_AIRLAB_PARAMETER, &CTstatAQ::OnBnClickedButtonAirlabParameter)
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_BUTTON_HELP_WBGT, &CTstatAQ::OnBnClickedButtonHelpWbgt)
END_MESSAGE_MAP()


// CTstatAQ 诊断 - CTstatAQ diagnostics

#ifdef _DEBUG
void CTstatAQ::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTstatAQ::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


LRESULT  CTstatAQ::AirlabMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageWriteOneInfo* Write_Struct_feedback = (_MessageWriteOneInfo*)lParam;
	bool msg_result = WRITE_FAIL;
	msg_result = MKBOOL(wParam);

	CString Show_Results;
	CString temp_cs = Write_Struct_feedback->Changed_Name;
	if (msg_result)
	{
		Show_Results = temp_cs + _T("Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS, Show_Results);
	}
	else
	{
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS, Show_Results);

	}

	if (Write_Struct_feedback)
		delete Write_Struct_feedback;
	return 0;
}
void CTstatAQ::PM2_5_Window(int nflag)
{
	if (nflag == 0)
	{
		//m_airlab_list.ShowWindow(0);  暂时所有产品都不显示，这个表格是鸡肋; - Temporarily hide for all products, this table is useless;
		GetDlgItem(IDC_RADIO_DEG_C)->EnableWindow(0);
		GetDlgItem(IDC_RADIO_DEG_F)->EnableWindow(0);
		GetDlgItem(IDC_BUTTON_HELP_WBGT)->EnableWindow(0);
		GetDlgItem(IDC_BUTTON_AUTO_CAL)->EnableWindow(0);

		GetDlgItem(IDC_STATIC_CO2_VALUE)->SetWindowTextW(_T("-"));
		GetDlgItem(IDC_STATIC_LIGHT_VALUE)->SetWindowTextW(_T("-"));
		GetDlgItem(IDC_STATIC_OCC_STATUS)->SetWindowTextW(_T("-"));
		GetDlgItem(IDC_STATIC_SOUND_VALUE)->SetWindowTextW(_T("-"));
		GetDlgItem(IDC_STATIC_HUM_VALUE)->SetWindowTextW(_T("-"));
		GetDlgItem(IDC_STATIC_VOC_VALUE)->SetWindowTextW(_T("-"));
		GetDlgItem(IDC_STATIC_FIR_AMBIENT_TEMP)->SetWindowTextW(_T("-"));
		GetDlgItem(IDC_STATIC_FIR_REMOTE_TEMP2)->SetWindowTextW(_T("-"));
	}
	else
	{
		//m_airlab_list.ShowWindow(0);
		GetDlgItem(IDC_RADIO_DEG_C)->EnableWindow(1);
		GetDlgItem(IDC_RADIO_DEG_F)->EnableWindow(1);
		GetDlgItem(IDC_BUTTON_HELP_WBGT)->EnableWindow(1);
		GetDlgItem(IDC_BUTTON_AIRLAB_PARAMETER)->EnableWindow(1);
		GetDlgItem(IDC_BUTTON_AUTO_CAL)->EnableWindow(1);
	}
}

CString AQ_image_fordor;
CString bmp_AQI;
CString jpg_airlab;
void CTstatAQ::Fresh()
{
	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_AQ_TEMPERATURE); // 得到 Picture Control 句柄 ; - Get Picture Control handle;
	CString icon_temperature;


	CString ApplicationFolder;
	GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
	PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
	ApplicationFolder.ReleaseBuffer();
	AQ_image_fordor = ApplicationFolder + _T("\\ResourceFile");
	icon_temperature = AQ_image_fordor + _T("\\temperature.bmp");
	//bmp_AQI = AQ_image_fordor + _T("\\AQI.bmp");
	bmp_AQI = AQ_image_fordor + _T("\\AQI.jpg");
	jpg_airlab = AQ_image_fordor + _T("\\Airlab.jpg");
	HBITMAP bitmap;
	bitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(), icon_temperature, IMAGE_BITMAP, 40, 120, LR_LOADFROMFILE);
	CStatic* p = (CStatic*)GetDlgItem(IDC_STATIC_AQ_TEMPERATURE);
	//设置静态控件窗口风格为位图居中显示 - Set static control window style to bitmap center display  
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	//将图片设置到Picture控件上 - Set image to Picture control  
	p->SetBitmap(bitmap);


	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	pFrame->SetWindowTextW(cs_special_name + CurrentT3000Version);
	if (h_tstat_aq_thread == NULL)
	{
		h_tstat_aq_thread = CreateThread(NULL, NULL, Update_TstatAQ_Thread, this, NULL, NULL);
		CloseHandle(h_tstat_aq_thread);
	}
	Initial_List();
	if (product_register_value[7] == STM32_PM25)
	{
		UpdatePM25UI();
		PM2_5_Window(0);
	}
	else
	{
		UpdateUI();
		PM2_5_Window(1);
	}


	CString sound_full_path;
	CStatic* pWnd_sound_pic = (CStatic*)GetDlgItem(IDC_STATIC_SOUND); // 得到 Picture Control 句柄 - Get the Picture Control handle ;
	if (product_register_value[TSTAT_AQ_SOUND] <= 10)
		sound_full_path = ApplicationFolder + _T("\\ResourceFile\\Icon\\sound_0.ico");
	else if (product_register_value[TSTAT_AQ_SOUND] <= 30)
		sound_full_path = ApplicationFolder + _T("\\ResourceFile\\Icon\\sound_1.ico");
	else if (product_register_value[TSTAT_AQ_SOUND] <= 50)
		sound_full_path = ApplicationFolder + _T("\\ResourceFile\\Icon\\sound_2.ico");
	else
		sound_full_path = ApplicationFolder + _T("\\ResourceFile\\Icon\\sound_3.ico");
	Bitmap icon_bitmap(sound_full_path);
	HICON m_hIcon = ExtractIcon(AfxGetInstanceHandle(), sound_full_path, 0);
	pWnd_sound_pic->ModifyStyle(0, SS_ICON | SS_CENTERIMAGE);
	pWnd_sound_pic->SetIcon(m_hIcon);
	m_airlab_list.ShowWindow(0);


	if ((product_register_value[7] == PM_TSTAT_AQ) ||
		(product_register_value[7] == PM_AIRLAB_ESP32))
	{
		SetTimer(1, 1000, NULL);
	}
	GetDlgItem(IDC_BUTTON_DONE)->SetFocus();
}

void CTstatAQ::EnableCus(bool flag)
{
	if (!flag)
	{
		GetDlgItem(IDC_EDIT_LEVEL_1)->SetWindowTextW(_T(" "));
		GetDlgItem(IDC_EDIT_LEVEL_2)->SetWindowTextW(_T(" "));
		GetDlgItem(IDC_EDIT_LEVEL_3)->SetWindowTextW(_T(" "));
		GetDlgItem(IDC_EDIT_LEVEL_4)->SetWindowTextW(_T(" "));
		GetDlgItem(IDC_EDIT_LEVEL_5)->SetWindowTextW(_T(" "));
	}
	GetDlgItem(IDC_EDIT_LEVEL_1)->EnableWindow(flag);
	GetDlgItem(IDC_EDIT_LEVEL_2)->EnableWindow(flag);
	GetDlgItem(IDC_EDIT_LEVEL_3)->EnableWindow(flag);
	GetDlgItem(IDC_EDIT_LEVEL_4)->EnableWindow(flag);
	GetDlgItem(IDC_EDIT_LEVEL_5)->EnableWindow(flag);
}

void CTstatAQ::UpdatePM25UI()
{
	CString temp_cus_api[5];
	temp_cus_api[0].Format(_T("%u"), product_register_value[PM25_MODBUS_AQI_FIRST_LINE]);
	temp_cus_api[1].Format(_T("%u"), product_register_value[PM25_MODBUS_AQI_SECOND_LINE]);
	temp_cus_api[2].Format(_T("%u"), product_register_value[PM25_MODBUS_AQI_THIRD_LINE]);
	temp_cus_api[3].Format(_T("%u"), product_register_value[PM25_MODBUS_AQI_FOURTH_LINE]);
	temp_cus_api[4].Format(_T("%u"), product_register_value[PM25_MODBUS_AQI_FIFTH_LINE]);
	GetDlgItem(IDC_EDIT_LEVEL_1)->SetWindowTextW(temp_cus_api[0]);
	GetDlgItem(IDC_EDIT_LEVEL_2)->SetWindowTextW(temp_cus_api[1]);
	GetDlgItem(IDC_EDIT_LEVEL_3)->SetWindowTextW(temp_cus_api[2]);
	GetDlgItem(IDC_EDIT_LEVEL_4)->SetWindowTextW(temp_cus_api[3]);
	GetDlgItem(IDC_EDIT_LEVEL_5)->SetWindowTextW(temp_cus_api[4]);

	CString cs_weight_pm1;
	if (product_register_value[PM25_MODBUS_PM25_WEIGHT_1_0] != 0)
		cs_weight_pm1.Format(_T("%u"), product_register_value[PM25_MODBUS_PM25_WEIGHT_1_0]);
	else
		cs_weight_pm1 = _T("-");
	CString cs_weight_pm2_5;
	if (product_register_value[PM25_MODBUS_PM25_WEIGHT_2_5] != 0)
		cs_weight_pm2_5.Format(_T("%u"), product_register_value[PM25_MODBUS_PM25_WEIGHT_2_5]);
	else
		cs_weight_pm2_5 = _T("-");
	CString cs_weight_pm4;
	if (product_register_value[PM25_MODBUS_PM25_WEIGHT_4_0] != 0)
		cs_weight_pm4.Format(_T("%u"), product_register_value[PM25_MODBUS_PM25_WEIGHT_4_0]);
	else
		cs_weight_pm4 = _T("-");
	CString cs_weight_pm10;
	if (product_register_value[PM25_MODBUS_PM25_WEIGHT_10] != 0)
		cs_weight_pm10.Format(_T("%u"), product_register_value[PM25_MODBUS_PM25_WEIGHT_10]);
	else
		cs_weight_pm10 = _T("-");

	CString cs_weight_total;
	cs_weight_total.Format(_T("%u"), product_register_value[PM25_MODBUS_PM25_WEIGHT_1_0] +
		product_register_value[PM25_MODBUS_PM25_WEIGHT_2_5] +
		product_register_value[PM25_MODBUS_PM25_WEIGHT_4_0] +
		product_register_value[PM25_MODBUS_PM25_WEIGHT_10]);

	GetDlgItem(IDC_STATIC_WEIGHT_PM1_0)->SetWindowTextW(cs_weight_pm1);
	GetDlgItem(IDC_STATIC_WEIGHT_PM2_5)->SetWindowTextW(cs_weight_pm2_5);
	GetDlgItem(IDC_STATIC_WEIGHT_PM4_0)->SetWindowTextW(cs_weight_pm4);
	GetDlgItem(IDC_STATIC_WEIGHT_PM10)->SetWindowTextW(cs_weight_pm10);
	GetDlgItem(IDC_STATIC_WEIGHT_TOTAL)->SetWindowTextW(cs_weight_total);

	CString cs_index_pm1;
	if (product_register_value[PM25_MODBUS_PM25_NUMBER_1_0] != 0)
		cs_index_pm1.Format(_T("%u"), product_register_value[PM25_MODBUS_PM25_NUMBER_1_0]);
	else
		cs_index_pm1 = _T("-");
	CString cs_index_pm2_5;
	if (product_register_value[PM25_MODBUS_PM25_NUMBER_2_5] != 0)
		cs_index_pm2_5.Format(_T("%u"), product_register_value[PM25_MODBUS_PM25_NUMBER_2_5]);
	else
		cs_index_pm2_5 = _T("-");
	CString cs_index_pm4;
	if (product_register_value[PM25_MODBUS_PM25_NUMBER_4_0] != 0)
		cs_index_pm4.Format(_T("%u"), product_register_value[PM25_MODBUS_PM25_NUMBER_4_0]);
	else
		cs_index_pm4 = _T("-");
	CString cs_index_pm10;
	if (product_register_value[PM25_MODBUS_PM25_NUMBER_10] != 0)
		cs_index_pm10.Format(_T("%u"), product_register_value[PM25_MODBUS_PM25_NUMBER_10]);
	else
		cs_index_pm10 = _T("-");

	CString cs_index_total;
	cs_index_total.Format(_T("%u"), product_register_value[PM25_MODBUS_PM25_NUMBER_1_0] +
		product_register_value[PM25_MODBUS_PM25_NUMBER_2_5] +
		product_register_value[PM25_MODBUS_PM25_NUMBER_4_0] +
		product_register_value[PM25_MODBUS_PM25_NUMBER_10]);

	GetDlgItem(IDC_STATIC_INDEX_PM1_0)->SetWindowTextW(cs_index_pm1);
	GetDlgItem(IDC_STATIC_INDEX_PM2_5)->SetWindowTextW(cs_index_pm2_5);
	GetDlgItem(IDC_STATIC_INDEX_PM4_0)->SetWindowTextW(cs_index_pm4);
	GetDlgItem(IDC_STATIC_INDEX_PM10)->SetWindowTextW(cs_index_pm10);
	GetDlgItem(IDC_STATIC_INDEX_TOTAL)->SetWindowTextW(cs_index_total);

	CString cs_api_value;
	CString cs_api_level;
	cs_api_value.Format(_T("%u"), product_register_value[PM25_MODBUS_AQI]);
	cs_api_level.Format(_T("%u"), product_register_value[PM25_MODBUS_AQI_LEVEL] + 1);
	GetDlgItem(IDC_EDIT_AQI_VALUE)->SetWindowTextW(cs_api_value);
	GetDlgItem(IDC_EDIT_AQI_LEVEL)->SetWindowTextW(cs_api_level);

	if ((aqi_level != product_register_value[PM25_MODBUS_AQI_LEVEL]) && product_register_value[PM25_MODBUS_AQI_LEVEL] <= 5)
	{
		aqi_level = product_register_value[PM25_MODBUS_AQI_LEVEL];
		m_aqi_title.SetWindowTextW(AQI_Info_Status[aqi_level]);
		m_aqi_title.textColor(AQI_Info_Status_Color[aqi_level]);
		m_aqi_title.bkColor(AQI_Info_Status_Back_Color[aqi_level]);
	}

	CString cs_api_region;
	if (product_register_value[PM25_MODBUS_PM25_AREA] == 0)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AQI_REGION))->SetCurSel(0);
		EnableCus(0);
	}
	else if (product_register_value[PM25_MODBUS_PM25_AREA] == 1)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AQI_REGION))->SetCurSel(1);
		EnableCus(0);
	}
	else
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AQI_REGION))->SetCurSel(2);
		EnableCus(1);
	}


	return;
}

void CTstatAQ::UpdateUI()
{
	int temp_software_version = 0;
	temp_software_version = product_register_value[4];
	CString cs_temp;
	cs_temp.Format(_T("%.1f"), ((float)product_register_value[TSTAT_AQ_TEMPERATURE]) / 10);

	CString cs_hum;
	cs_hum.Format(_T("%.1f"), ((float)product_register_value[TSTAT_AQ_HUM]) / 10);

	CString cs_ppm;
	cs_ppm.Format(_T("%d"), product_register_value[TSTAT_AQ_CO2]);

	CString cs_VOC;
	if ((product_register_value[7] == PM_TSTAT_AQ) ||
		(product_register_value[7] == PM_AIRLAB_ESP32))
		cs_VOC.Format(_T("%u"), product_register_value[TSTAT_AQ_VOC_AIRLAB]);
	else
		cs_VOC.Format(_T("%u"), product_register_value[TSTAT_AQ_VOC]);

	CString cs_light;
	cs_light.Format(_T("%u"), product_register_value[TSTAT_AQ_LIGHT]);

	CString cs_sound;
	cs_sound.Format(_T("%u"), product_register_value[TSTAT_AQ_SOUND]);

	CString cs_co2_on;
	CString cs_co2_off;
	CString cs_pm_on;
	CString cs_pm_off;
	if ((product_register_value[7] == PM_TSTAT_AQ) ||
		(product_register_value[7] == PM_AIRLAB_ESP32))
	{
		cs_co2_on.Format(_T("%u"), product_register_value[1020]);
		cs_co2_off.Format(_T("%u"), product_register_value[1021]);
		cs_pm_on.Format(_T("%u"), product_register_value[1022]);
		cs_pm_off.Format(_T("%u"), product_register_value[1023]);
	}
	else
	{
		cs_co2_on.Format(_T("%u"), product_register_value[TATAT_AQ_CO2_ON]);
		cs_co2_off.Format(_T("%u"), product_register_value[TATAT_AQ_CO2_OFF]);
		cs_pm_on.Format(_T("%u"), product_register_value[TATAT_AQ_PM_ON]);
		cs_pm_off.Format(_T("%u"), product_register_value[TATAT_AQ_PM_OFF]);
	}


	GetDlgItem(IDC_STATIC_TEMPERATURE_VALUE)->SetWindowTextW(cs_temp);
	GetDlgItem(IDC_STATIC_HUM_VALUE)->SetWindowTextW(cs_hum);
	GetDlgItem(IDC_STATIC_CO2_VALUE)->SetWindowTextW(cs_ppm);
	GetDlgItem(IDC_STATIC_VOC_VALUE)->SetWindowTextW(cs_VOC);

	if (product_register_value[4] >= 140)
	{
		GetDlgItem(IDC_STATIC_WBGT_TEMPERATURE)->EnableWindow(1);
		GetDlgItem(IDC_BUTTON_HELP_WBGT)->EnableWindow(1);
		CString temp_wbgt;
		temp_wbgt.Format(_T("%.1f"), (float)product_register_value[TSTAT_AQ_WBGT] / 10.0);
		GetDlgItem(IDC_STATIC_WBGT_TEMPERATURE)->SetWindowTextW(temp_wbgt);
	}
	else
	{
		GetDlgItem(IDC_STATIC_WBGT_TEMPERATURE)->EnableWindow(0);
		GetDlgItem(IDC_BUTTON_HELP_WBGT)->EnableWindow(0);
		GetDlgItem(IDC_STATIC_WBGT_TEMPERATURE)->SetWindowTextW(_T(""));
	}

	CString cs_api_value;
	CString cs_api_level;
	cs_api_value.Format(_T("%u"), product_register_value[TATAT_AQ_MODBUS_AQI]);

	cs_api_level.Format(_T("%u"), product_register_value[TATAT_AQ_MODBUS_AQI_LEVEL] + 1);

	if ((aqi_level != product_register_value[TATAT_AQ_MODBUS_AQI_LEVEL]) && product_register_value[TATAT_AQ_MODBUS_AQI_LEVEL] <= 5)
	{
		aqi_level = product_register_value[TATAT_AQ_MODBUS_AQI_LEVEL];
		m_aqi_title.SetWindowTextW(AQI_Info_Status[aqi_level]);
		m_aqi_title.textColor(AQI_Info_Status_Color[aqi_level]);
		m_aqi_title.bkColor(AQI_Info_Status_Back_Color[aqi_level]);
	}

	CString temp_cus_api[5];
	temp_cus_api[0].Format(_T("%u"), product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_FIRST_LINE]);
	temp_cus_api[1].Format(_T("%u"), product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_SECOND_LINE]);
	temp_cus_api[2].Format(_T("%u"), product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_THIRD_LINE]);
	temp_cus_api[3].Format(_T("%u"), product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_FOURTH_LINE]);
	temp_cus_api[4].Format(_T("%u"), product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_FIFTH_LINE]);

	if ((product_register_value[7] == PM_MULTI_SENSOR) ||
		(product_register_value[7] == PM_TSTAT_AQ) ||
		(product_register_value[7] == PM_AIRLAB_ESP32))
	{

		if (temp_software_version >= 118)
		{
			GetDlgItem(IDC_BUTTON_AIRLAB_PARAMETER)->EnableWindow(1);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_AIRLAB_PARAMETER)->EnableWindow(0);
		}

		if (temp_software_version >= 112)
		{
			GetDlgItem(IDC_EDIT_CO2_ON_TIME)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_CO2_OFF_TIME)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_PM_ON_TIME)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_PM_OFF_TIME)->EnableWindow(1);
		}
		else
		{
			GetDlgItem(IDC_EDIT_CO2_ON_TIME)->EnableWindow(0);
			GetDlgItem(IDC_EDIT_CO2_OFF_TIME)->EnableWindow(0);
			GetDlgItem(IDC_EDIT_PM_ON_TIME)->EnableWindow(0);
			GetDlgItem(IDC_EDIT_PM_OFF_TIME)->EnableWindow(0);
		}
		GetDlgItem(IDC_EDIT_CO2_ON_TIME)->SetWindowTextW(cs_co2_on);
		GetDlgItem(IDC_EDIT_CO2_OFF_TIME)->SetWindowTextW(cs_co2_off);
		GetDlgItem(IDC_EDIT_PM_ON_TIME)->SetWindowTextW(cs_pm_on);
		GetDlgItem(IDC_EDIT_PM_OFF_TIME)->SetWindowTextW(cs_pm_off);
		GetDlgItem(IDC_STATIC_LIGHT_VALUE)->SetWindowTextW(cs_light);
		GetDlgItem(IDC_STATIC_SOUND_VALUE)->SetWindowTextW(cs_sound);

		GetDlgItem(IDC_EDIT_AQI_VALUE)->SetWindowTextW(_T(" "));
		GetDlgItem(IDC_EDIT_AQI_LEVEL)->SetWindowTextW(_T(" "));
		GetDlgItem(IDC_COMBO_AQI_REGION)->SetWindowTextW(_T(" "));

		EnableCus(0);
	}
	if (product_register_value[7] == PM_TSTAT_AQ)
	{
		if (temp_software_version < 112)
		{
			GetDlgItem(IDC_EDIT_CO2_ON_TIME)->EnableWindow(0);
			GetDlgItem(IDC_EDIT_CO2_OFF_TIME)->EnableWindow(0);
			GetDlgItem(IDC_EDIT_PM_ON_TIME)->EnableWindow(0);
			GetDlgItem(IDC_EDIT_PM_OFF_TIME)->EnableWindow(0);
			GetDlgItem(IDC_EDIT_CO2_ON_TIME)->SetWindowTextW(_T(" "));
			GetDlgItem(IDC_EDIT_CO2_OFF_TIME)->SetWindowTextW(_T(" "));
			GetDlgItem(IDC_EDIT_PM_ON_TIME)->SetWindowTextW(_T(" "));
			GetDlgItem(IDC_EDIT_PM_OFF_TIME)->SetWindowTextW(_T(" "));
		}

		GetDlgItem(IDC_EDIT_AQI_VALUE)->SetWindowTextW(cs_api_value);
		GetDlgItem(IDC_EDIT_AQI_LEVEL)->SetWindowTextW(cs_api_level);
		CString cs_api_region;
		if (product_register_value[TATAT_AQ_MODBUS_AQI_AREA] == 0)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_AQI_REGION))->SetCurSel(0);
			EnableCus(0);
		}
		else if (product_register_value[TATAT_AQ_MODBUS_AQI_AREA] == 1)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_AQI_REGION))->SetCurSel(1);
			EnableCus(0);
		}
		else
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_AQI_REGION))->SetCurSel(2);
			EnableCus(1);
		}

		GetDlgItem(IDC_EDIT_LEVEL_1)->SetWindowTextW(temp_cus_api[0]);
		GetDlgItem(IDC_EDIT_LEVEL_2)->SetWindowTextW(temp_cus_api[1]);
		GetDlgItem(IDC_EDIT_LEVEL_3)->SetWindowTextW(temp_cus_api[2]);
		GetDlgItem(IDC_EDIT_LEVEL_4)->SetWindowTextW(temp_cus_api[3]);
		GetDlgItem(IDC_EDIT_LEVEL_5)->SetWindowTextW(temp_cus_api[4]);
	}
	else
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AQI_REGION))->EnableWindow(0);
	}

	if (product_register_value[TSTAT_AQ_TEMP_UNIT] == 0)
	{
		((CButton*)GetDlgItem(IDC_RADIO_DEG_C))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO_DEG_F))->SetCheck(0);
		GetDlgItem(IDC_STATIC_TEMP_UNITS)->SetWindowTextW(_T("Deg.C"));
		GetDlgItem(IDC_STATIC_WBGT_UNITS)->SetWindowTextW(_T("Deg.C"));
	}
	else if (product_register_value[TSTAT_AQ_TEMP_UNIT] == 1)
	{
		((CButton*)GetDlgItem(IDC_RADIO_DEG_C))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_DEG_F))->SetCheck(1);
		GetDlgItem(IDC_STATIC_TEMP_UNITS)->SetWindowTextW(_T("Deg.F"));
		GetDlgItem(IDC_STATIC_WBGT_UNITS)->SetWindowTextW(_T("Deg.F"));
	}

	CString cs_weight_pm1;
	if (product_register_value[TSTAT_AQ_WEIGHT_1] != 0)
		cs_weight_pm1.Format(_T("%u"), product_register_value[TSTAT_AQ_WEIGHT_1]);
	else
		cs_weight_pm1 = _T("-");
	CString cs_weight_pm2_5;
	if (product_register_value[TSTAT_AQ_WEIGHT_2_5] != 0)
		cs_weight_pm2_5.Format(_T("%u"), product_register_value[TSTAT_AQ_WEIGHT_2_5]);
	else
		cs_weight_pm2_5 = _T("-");
	CString cs_weight_pm4;
	if (product_register_value[TSTAT_AQ_WEIGHT_4] != 0)
		cs_weight_pm4.Format(_T("%u"), product_register_value[TSTAT_AQ_WEIGHT_4]);
	else
		cs_weight_pm4 = _T("-");
	CString cs_weight_pm10;
	if (product_register_value[TSTAT_AQ_WEIGHT_10] != 0)
		cs_weight_pm10.Format(_T("%u"), product_register_value[TSTAT_AQ_WEIGHT_10]);
	else
		cs_weight_pm10 = _T("-");

	CString cs_weight_total;
	cs_weight_total.Format(_T("%u"), product_register_value[TSTAT_AQ_WEIGHT_1] +
		product_register_value[TSTAT_AQ_WEIGHT_2_5] +
		product_register_value[TSTAT_AQ_WEIGHT_4] +
		product_register_value[TSTAT_AQ_WEIGHT_10]);

	GetDlgItem(IDC_STATIC_WEIGHT_PM1_0)->SetWindowTextW(cs_weight_pm1);
	GetDlgItem(IDC_STATIC_WEIGHT_PM2_5)->SetWindowTextW(cs_weight_pm2_5);
	GetDlgItem(IDC_STATIC_WEIGHT_PM4_0)->SetWindowTextW(cs_weight_pm4);
	GetDlgItem(IDC_STATIC_WEIGHT_PM10)->SetWindowTextW(cs_weight_pm10);
	GetDlgItem(IDC_STATIC_WEIGHT_TOTAL)->SetWindowTextW(cs_weight_total);

	CString cs_index_pm1;
	if (product_register_value[TSTAT_AQ_INDEX_1] != 0)
		cs_index_pm1.Format(_T("%u"), product_register_value[TSTAT_AQ_INDEX_1]);
	else
		cs_index_pm1 = _T("-");
	CString cs_index_pm2_5;
	if (product_register_value[TSTAT_AQ_INDEX_2_5] != 0)
		cs_index_pm2_5.Format(_T("%u"), product_register_value[TSTAT_AQ_INDEX_2_5]);
	else
		cs_index_pm2_5 = _T("-");
	CString cs_index_pm4;
	if (product_register_value[TSTAT_AQ_INDEX_4] != 0)
		cs_index_pm4.Format(_T("%u"), product_register_value[TSTAT_AQ_INDEX_4]);
	else
		cs_index_pm4 = _T("-");
	CString cs_index_pm10;
	if (product_register_value[TSTAT_AQ_INDEX_10] != 0)
		cs_index_pm10.Format(_T("%u"), product_register_value[TSTAT_AQ_INDEX_10]);
	else
		cs_index_pm10 = _T("-");

	CString cs_index_total;
	cs_index_total.Format(_T("%u"), product_register_value[TSTAT_AQ_INDEX_1] +
		product_register_value[TSTAT_AQ_INDEX_2_5] +
		product_register_value[TSTAT_AQ_INDEX_4] +
		product_register_value[TSTAT_AQ_INDEX_10]);

	GetDlgItem(IDC_STATIC_INDEX_PM1_0)->SetWindowTextW(cs_index_pm1);
	GetDlgItem(IDC_STATIC_INDEX_PM2_5)->SetWindowTextW(cs_index_pm2_5);
	GetDlgItem(IDC_STATIC_INDEX_PM4_0)->SetWindowTextW(cs_index_pm4);
	GetDlgItem(IDC_STATIC_INDEX_PM10)->SetWindowTextW(cs_index_pm10);
	GetDlgItem(IDC_STATIC_INDEX_TOTAL)->SetWindowTextW(cs_index_total);

	if (product_register_value[7] != PM_TSTAT_AQ)
	{
		for (int i = 0; i < 4; i++)
		{
			m_airlab_list.SetCellEnabled(i, AIRLAB_TRIGGER, 0);
			m_airlab_list.SetCellEnabled(i, AIRLAB_TIME, 0);
		}

	}
	if (product_register_sensor_flag[0] == 0x55)
	{
		CString temp_value;
		temp_value.Format(_T("%u"), product_register_value[TSTAT_AQ_CO2]);
		bitset<16> module_type(product_register_sensor_flag[1]);
		if (module_type.test(SENSOR_BIT_CO2) == true)
		{
			m_airlab_list.SetCellEnabled(0, AIRLAB_TRIGGER, 1);
			m_airlab_list.SetCellEnabled(0, AIRLAB_TIME, 1);

			int temp_interval_time;
			char temp_char[50];
			temp_interval_time = product_register_value[TSTAT_AQ_CO2_TIMER] * 60;
			intervaltotext(temp_char, temp_interval_time, 0, 0);
			CString temp_CO2_timer;
			MultiByteToWideChar(CP_ACP, 0, temp_char, (int)strlen(temp_char) + 1,
				temp_CO2_timer.GetBuffer(MAX_PATH), MAX_PATH);
			temp_CO2_timer.ReleaseBuffer();


			CString temp_CO2_trigger;
			temp_CO2_trigger.Format(_T("%u"), product_register_value[TSTAT_AQ_CO2_TRIGGER]);
			//CString temp_CO2_timer;
			//temp_CO2_timer.Format(_T("%u"), product_register_value[TSTAT_AQ_CO2_TIMER]);
			CString temp_CO2_timeleft;
			temp_CO2_timeleft.Format(_T("%u"), product_register_value[TSTAT_AQ_CO2_COUNT_DOWN]);
			CString temp_CO2_alarm;
			if (product_register_value[TSTAT_AQ_CO2_ALARM_ON] != 0)
				temp_CO2_alarm = _T("ON");
			else
				temp_CO2_alarm = _T("OFF");
			m_airlab_list.SetItemText(AIRLAB_CO2, AIRLAB_VALUE, temp_value);
			m_airlab_list.SetItemText(AIRLAB_CO2, AIRLAB_UNIT, _T("PPM"));
			m_airlab_list.SetItemText(AIRLAB_CO2, AIRLAB_TRIGGER, temp_CO2_trigger);
			m_airlab_list.SetItemText(AIRLAB_CO2, AIRLAB_TIME, temp_CO2_timer);
			m_airlab_list.SetItemText(AIRLAB_CO2, AIRLAB_TIME_LEFT, temp_CO2_timeleft);
			m_airlab_list.SetItemText(AIRLAB_CO2, AIRLAB_ALARM, temp_CO2_alarm);
		}
		else
		{
			GetDlgItem(IDC_STATIC_CO2_VALUE)->SetWindowTextW(_T("No Sensor"));
			m_airlab_list.SetCellEnabled(AIRLAB_CO2, AIRLAB_TRIGGER, 0);
			m_airlab_list.SetCellEnabled(AIRLAB_CO2, AIRLAB_TIME, 0);
			m_airlab_list.SetItemText(AIRLAB_CO2, AIRLAB_VALUE, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_CO2, AIRLAB_UNIT, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_CO2, AIRLAB_TRIGGER, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_CO2, AIRLAB_TIME, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_CO2, AIRLAB_TIME_LEFT, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_CO2, AIRLAB_ALARM, _T("-"));
		}
	}
	else
	{
		m_airlab_list.SetItemText(0, AIRLAB_VALUE, _T("-"));
		m_airlab_list.SetItemText(0, AIRLAB_UNIT, _T("-"));
		m_airlab_list.SetItemText(0, AIRLAB_TRIGGER, _T("-"));
		m_airlab_list.SetItemText(0, AIRLAB_TIME, _T("-"));
		m_airlab_list.SetCellEnabled(0, AIRLAB_TIME, 0);
		m_airlab_list.SetItemText(0, AIRLAB_TIME_LEFT, _T("-"));
	}

	if (product_register_sensor_flag[0] == 0x55)
	{
		CString temp_value;
		temp_value.Format(_T("%u"), product_register_value[TSTAT_AQ_OCC_VALUE]);
		bitset<16> module_type(product_register_sensor_flag[1]);
		if (module_type.test(SENSOR_BIT_FAR_INFRA_RED) == true)
		{
			CString temp_ambient;
			CString temp_remote;
			temp_ambient.Format(_T("%.1f"), ((float)product_register_value[TSTAT_AQ_TEMP_AMBIENT]) / 10);
			temp_remote.Format(_T("%.1f"), ((float)product_register_value[TSTAT_AQ_TEMP_REMOTE]) / 10);

			GetDlgItem(IDC_STATIC_FIR_AMBIENT_TEMP)->SetWindowTextW(temp_ambient);
			GetDlgItem(IDC_STATIC_FIR_REMOTE_TEMP2)->SetWindowTextW(temp_remote);
		}
		else
		{
			GetDlgItem(IDC_STATIC_FIR_AMBIENT_TEMP)->SetWindowTextW(_T("-"));
			GetDlgItem(IDC_STATIC_FIR_REMOTE_TEMP2)->SetWindowTextW(_T("-"));
		}
	}
	else
	{
		GetDlgItem(IDC_STATIC_FIR_AMBIENT_TEMP)->SetWindowTextW(_T("-"));
		GetDlgItem(IDC_STATIC_FIR_REMOTE_TEMP2)->SetWindowTextW(_T("-"));
	}

	if (product_register_sensor_flag[0] == 0x55)
	{
		CString temp_value;
		temp_value.Format(_T("%u"), product_register_value[TSTAT_AQ_OCC_VALUE]);
		bitset<16> module_type(product_register_sensor_flag[1]);
		if (module_type.test(SENSOR_BIT_OCC) == true)
		{
			m_airlab_list.SetCellEnabled(AIRLAB_OCC, AIRLAB_TRIGGER, 1);
			m_airlab_list.SetCellEnabled(AIRLAB_OCC, AIRLAB_TIME, 1);

			int temp_interval_time;
			char temp_char[50];
			temp_interval_time = product_register_value[TSTAT_AQ_OCC_TIMER] * 60;
			intervaltotext(temp_char, temp_interval_time, 0, 0);
			CString temp_OCC_timer;
			MultiByteToWideChar(CP_ACP, 0, temp_char, (int)strlen(temp_char) + 1,
				temp_OCC_timer.GetBuffer(MAX_PATH), MAX_PATH);
			temp_OCC_timer.ReleaseBuffer();


			CString temp_OCC_trigger;
			temp_OCC_trigger.Format(_T("%u"), product_register_value[TSTAT_AQ_OCC_TRIGGER]);
			//CString temp_OCC_timer;
			//temp_OCC_timer.Format(_T("%u"), product_register_value[TSTAT_AQ_OCC_TIMER]);
			CString temp_OCC_timeleft;
			temp_OCC_timeleft.Format(_T("%u"), product_register_value[TSTAT_AQ_OCC_COUNT_DOWN]);
			CString temp_OCC_alarm;
			if (product_register_value[TSTAT_AQ_OCC_ALARM_ON] != 0)
				temp_OCC_alarm = _T("ON");
			else
				temp_OCC_alarm = _T("OFF");
			m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_TRIGGER, temp_OCC_trigger);
			m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_TIME, temp_OCC_timer);
			m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_TIME_LEFT, temp_OCC_timeleft);
			m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_ALARM, temp_OCC_alarm);
			if (product_register_value[736] == 1)
			{
				m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_VALUE, temp_value);
				m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_UNIT, _T("Occupied"));
				GetDlgItem(IDC_STATIC_OCC_STATUS)->SetWindowTextW(_T("Occupied"));
			}
			else
			{
				m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_VALUE, temp_value);
				m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_UNIT, _T("Unoccupied"));
				GetDlgItem(IDC_STATIC_OCC_STATUS)->SetWindowTextW(_T("Unoccupied"));

			}
		}
		else
		{
			GetDlgItem(IDC_STATIC_OCC_STATUS)->SetWindowTextW(_T("No Sensor"));
			m_airlab_list.SetCellEnabled(AIRLAB_OCC, AIRLAB_TRIGGER, 0);
			m_airlab_list.SetCellEnabled(AIRLAB_OCC, AIRLAB_TIME, 0);
			m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_VALUE, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_UNIT, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_TRIGGER, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_TIME, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_TIME_LEFT, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_ALARM, _T("-"));
			GetDlgItem(IDC_STATIC_OCC_STATUS)->SetWindowTextW(_T("-"));
		}
	}
	else
	{
		m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_VALUE, _T("-"));
		m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_UNIT, _T("-"));
		m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_TRIGGER, _T("-"));
		m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_TIME, _T("-"));
		m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_TIME_LEFT, _T("-"));
		m_airlab_list.SetItemText(AIRLAB_OCC, AIRLAB_ALARM, _T("-"));
	}
	if (product_register_sensor_flag[0] == 0x55)
	{
		CString temp_value;
		temp_value.Format(_T("%u"), product_register_value[TSTAT_AQ_LIGHT]);
		bitset<16> module_type(product_register_sensor_flag[1]);

		if (module_type.test(SENSOR_BIT_LIGHT) == true)
		{
			m_airlab_list.SetCellEnabled(AIRLAB_LIGHT, AIRLAB_TRIGGER, 1);
			m_airlab_list.SetCellEnabled(AIRLAB_LIGHT, AIRLAB_TIME, 1);
			GetDlgItem(IDC_STATIC_LIGHT_VALUE)->SetWindowTextW(temp_value);
			int temp_interval_time;
			char temp_char[50];
			temp_interval_time = product_register_value[TSTAT_AQ_LIGHT_TIMER] * 60;
			intervaltotext(temp_char, temp_interval_time, 0, 0);
			CString temp_light_timer;
			MultiByteToWideChar(CP_ACP, 0, temp_char, (int)strlen(temp_char) + 1,
				temp_light_timer.GetBuffer(MAX_PATH), MAX_PATH);
			temp_light_timer.ReleaseBuffer();
			CString temp_light_trigger;
			temp_light_trigger.Format(_T("%u"), product_register_value[TSTAT_AQ_LIGHT_TRIGGER]);
			//CString temp_light_timer;
			//temp_light_timer.Format(_T("%u"), product_register_value[TSTAT_AQ_LIGHT_TIMER]);
			CString temp_light_timeleft;
			temp_light_timeleft.Format(_T("%u"), product_register_value[TSTAT_AQ_LIGHT_COUNT_DOWN]);
			CString temp_light_alarm;
			if (product_register_value[TSTAT_AQ_LIGHT_ALARM_ON] != 0)
				temp_light_alarm = _T("ON");
			else
				temp_light_alarm = _T("OFF");

			m_airlab_list.SetItemText(AIRLAB_LIGHT, AIRLAB_VALUE, temp_value);
			m_airlab_list.SetItemText(AIRLAB_LIGHT, AIRLAB_UNIT, _T("Lux"));
			m_airlab_list.SetItemText(AIRLAB_LIGHT, AIRLAB_TRIGGER, temp_light_trigger);
			m_airlab_list.SetItemText(AIRLAB_LIGHT, AIRLAB_TIME, temp_light_timer);
			m_airlab_list.SetItemText(AIRLAB_LIGHT, AIRLAB_TIME_LEFT, temp_light_timeleft);
			m_airlab_list.SetItemText(AIRLAB_LIGHT, AIRLAB_ALARM, temp_light_alarm);
		}
		else
		{
			GetDlgItem(IDC_STATIC_LIGHT_VALUE)->SetWindowTextW(_T("No Sensor"));
			m_airlab_list.SetCellEnabled(AIRLAB_LIGHT, AIRLAB_TRIGGER, 0);
			m_airlab_list.SetCellEnabled(AIRLAB_LIGHT, AIRLAB_TIME, 0);
			m_airlab_list.SetItemText(AIRLAB_LIGHT, AIRLAB_VALUE, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_LIGHT, AIRLAB_UNIT, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_LIGHT, AIRLAB_TRIGGER, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_LIGHT, AIRLAB_TIME, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_LIGHT, AIRLAB_TIME_LEFT, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_LIGHT, AIRLAB_ALARM, _T("-"));
		}
	}
	else
	{
		m_airlab_list.SetItemText(AIRLAB_LIGHT, AIRLAB_VALUE, _T("-"));
		m_airlab_list.SetItemText(AIRLAB_LIGHT, AIRLAB_UNIT, _T("-"));
		m_airlab_list.SetItemText(AIRLAB_LIGHT, AIRLAB_TRIGGER, _T("-"));
		m_airlab_list.SetItemText(AIRLAB_LIGHT, AIRLAB_TIME, _T("-"));
		m_airlab_list.SetItemText(AIRLAB_LIGHT, AIRLAB_TIME_LEFT, _T("-"));
	}
	if (product_register_sensor_flag[0] == 0x55)
	{
		CString temp_value;
		temp_value.Format(_T("%u"), product_register_value[TSTAT_AQ_SOUND]);
		bitset<16> module_type(product_register_sensor_flag[1]);

		if (module_type.test(SENSOR_BIT_SOUND) == true)
		{
			m_airlab_list.SetCellEnabled(AIRLAB_SOUND, AIRLAB_TRIGGER, 1);
			m_airlab_list.SetCellEnabled(AIRLAB_SOUND, AIRLAB_TIME, 1);
			GetDlgItem(IDC_STATIC_SOUND_VALUE)->SetWindowTextW(temp_value);

			int temp_interval_time;
			char temp_char[50];
			temp_interval_time = product_register_value[TSTAT_AQ_SOUND_TIMER] * 60;
			intervaltotext(temp_char, temp_interval_time, 0, 0);
			CString temp_sound_timer;
			MultiByteToWideChar(CP_ACP, 0, temp_char, (int)strlen(temp_char) + 1,
				temp_sound_timer.GetBuffer(MAX_PATH), MAX_PATH);
			temp_sound_timer.ReleaseBuffer();

			CString temp_sound_trigger;
			temp_sound_trigger.Format(_T("%u"), product_register_value[TSTAT_AQ_SOUND_TRIGGER]);
			//CString temp_sound_timer;
			//temp_sound_timer.Format(_T("%u"), product_register_value[TSTAT_AQ_SOUND_TIMER]);
			CString temp_sound_timeleft;
			temp_sound_timeleft.Format(_T("%u"), product_register_value[TSTAT_AQ_SOUND_COUNT_DOWN]);
			CString temp_sound_alarm;
			if (product_register_value[TSTAT_AQ_SOUND_ALARM_ON] != 0)
				temp_sound_alarm = _T("ON");
			else
				temp_sound_alarm = _T("OFF");
			m_airlab_list.SetItemText(AIRLAB_SOUND, AIRLAB_VALUE, temp_value);
			m_airlab_list.SetItemText(AIRLAB_SOUND, AIRLAB_UNIT, _T("dBA"));
			m_airlab_list.SetItemText(AIRLAB_SOUND, AIRLAB_TRIGGER, temp_sound_trigger);
			m_airlab_list.SetItemText(AIRLAB_SOUND, AIRLAB_TIME, temp_sound_timer);
			m_airlab_list.SetItemText(AIRLAB_SOUND, AIRLAB_TIME_LEFT, temp_sound_timeleft);
			m_airlab_list.SetItemText(AIRLAB_SOUND, AIRLAB_ALARM, temp_sound_alarm);
		}
		else
		{
			GetDlgItem(IDC_STATIC_SOUND_VALUE)->SetWindowTextW(_T("No Sensor"));
			m_airlab_list.SetCellEnabled(AIRLAB_SOUND, AIRLAB_TRIGGER, 0);
			m_airlab_list.SetCellEnabled(AIRLAB_SOUND, AIRLAB_TIME, 0);
			m_airlab_list.SetItemText(AIRLAB_SOUND, AIRLAB_VALUE, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_SOUND, AIRLAB_UNIT, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_SOUND, AIRLAB_TRIGGER, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_SOUND, AIRLAB_TIME, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_SOUND, AIRLAB_TIME_LEFT, _T("-"));
			m_airlab_list.SetItemText(AIRLAB_SOUND, AIRLAB_ALARM, _T("-"));
		}
	}
}
// CTstatAQ 消息处理程序 - CTstatAQ message handlers



void CTstatAQ::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_aqi_title.SetWindowTextW(_T(""));
	m_aqi_title.textColor(RGB(255, 255, 255));
	m_aqi_title.bkColor(RGB(0, 0, 255));
	m_aqi_title.setFont(28, 26, NULL, _T("Arial"));

	m_static_info.SetWindowTextW(_T("Air Quality"));
	m_static_info.textColor(RGB(255, 255, 255));
	m_static_info.bkColor(RGB(0, 0, 255));
	m_static_info.setFont(28, 26, NULL, _T("Arial"));
	// TODO: 在此添加专用代码和/或调用基类 - TODO: Add your specialized code and/or call the base class
}

DWORD WINAPI Update_TstatAQ_Thread(LPVOID lPvoid)
{
	CTstatAQ* mparent = (CTstatAQ*)lPvoid;
	while (mparent->IsWindowVisible())
	{
		Read_Multi(g_tstat_id, &product_register_value[100], 100, 100, 5);
		Sleep(SEND_COMMAND_DELAY_TIME);
		Read_Multi(g_tstat_id, &product_register_value[600], 600, 100, 5);
		Sleep(SEND_COMMAND_DELAY_TIME);
		Read_Multi(g_tstat_id, &product_register_value[700], 700, 100, 5);
		Sleep(SEND_COMMAND_DELAY_TIME);
		Read_Multi(g_tstat_id, &product_register_value[950], 950, 100, 5);
		Sleep(SEND_COMMAND_DELAY_TIME);
		PostMessage(mparent->m_hWnd, WM_TSTAT_AQ_THREAD_READ, NULL, NULL);
		Sleep(30000);
	}
	mparent->KillTimer(1);

	h_tstat_aq_thread = NULL;
	return 1;
}

LRESULT CTstatAQ::UpdateUI(WPARAM wParam, LPARAM lParam)
{
	Fresh();
	return 0;
}


void CTstatAQ::OnEnKillfocusEditCo2OnTime()
{
	// TODO: 在此添加控件通知处理程序代码 - TODO: Add your control notification handler code here

	CString strText;
	GetDlgItem(IDC_EDIT_CO2_ON_TIME)->GetWindowText(strText);
	int nValue = (int)(_wtoi(strText));


	if (product_register_value[TATAT_AQ_CO2_ON] == nValue)	//Add this to judge weather this value need to change.
		return;


	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_CO2_ON, nValue,
		product_register_value[TATAT_AQ_CO2_ON], this->m_hWnd, IDC_EDIT_DTERM, _T("CO2 On Time"));
}


void CTstatAQ::OnEnKillfocusEditCo2OffTime()
{
	// TODO: 在此添加控件通知处理程序代码 - TODO: Add your control notification handler code here
	CString strText;
	GetDlgItem(IDC_EDIT_CO2_OFF_TIME)->GetWindowText(strText);
	int nValue = (int)(_wtoi(strText));


	if (product_register_value[TATAT_AQ_CO2_OFF] == nValue)	//Add this to judge weather this value need to change.
		return;


	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_CO2_OFF, nValue,
		product_register_value[TATAT_AQ_CO2_OFF], this->m_hWnd, IDC_EDIT_DTERM, _T("CO2 Off Time"));
}


void CTstatAQ::OnEnKillfocusEditPmOnTime()
{
	// TODO: 在此添加控件通知处理程序代码 - TODO: Add your control notification handler code here
	CString strText;
	GetDlgItem(IDC_EDIT_PM_ON_TIME)->GetWindowText(strText);
	int nValue = (int)(_wtoi(strText));


	if (product_register_value[TATAT_AQ_PM_ON] == nValue)	//Add this to judge weather this value need to change.
		return;


	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_PM_ON, nValue,
		product_register_value[TATAT_AQ_PM_ON], this->m_hWnd, IDC_EDIT_DTERM, _T("PM On Time"));
}


void CTstatAQ::OnEnKillfocusEditPmOffTime()
{
	// TODO: 在此添加控件通知处理程序代码 - TODO: Add your control notification handler code here
	CString strText;
	GetDlgItem(IDC_EDIT_PM_OFF_TIME)->GetWindowText(strText);
	int nValue = (int)(_wtoi(strText));


	if (product_register_value[TATAT_AQ_PM_OFF] == nValue)	//Add this to judge weather this value need to change.
		return;


	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_PM_OFF, nValue,
		product_register_value[TATAT_AQ_PM_OFF], this->m_hWnd, IDC_EDIT_DTERM, _T("PM Off Time"));
}


void CTstatAQ::OnBnClickedRadioDegC()
{
	// TODO: 在此添加控件通知处理程序代码 - TODO: Add your control notification handler code here
	int nValue = 0;


	if (product_register_value[TSTAT_AQ_TEMP_UNIT] == nValue)	//Add this to judge weather this value need to change.
		return;


	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TSTAT_AQ_TEMP_UNIT, nValue,
		product_register_value[TSTAT_AQ_TEMP_UNIT], this->m_hWnd, IDC_EDIT_DTERM, _T(" Deg.C "));
}


void CTstatAQ::OnBnClickedRadioDegF()
{
	// TODO: 在此添加控件通知处理程序代码 - TODO: Add your control notification handler code here
	int nValue = 1;


	if (product_register_value[TSTAT_AQ_TEMP_UNIT] == nValue)	//Add this to judge weather this value need to change.
		return;


	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TSTAT_AQ_TEMP_UNIT, nValue,
		product_register_value[TSTAT_AQ_TEMP_UNIT], this->m_hWnd, IDC_EDIT_DTERM, _T(" Deg.F "));
}


void CTstatAQ::OnCbnSelchangeComboAqiRegion()
{
	// TODO: 在此添加控件通知处理程序代码 - TODO: Add your control notification handler code here
	int n_value = 0;
	CString temp_string;
	int nSel = ((CComboBox*)GetDlgItem(IDC_COMBO_AQI_REGION))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO_AQI_REGION))->GetLBText(nSel, temp_string);
	if (temp_string.CompareNoCase(_T("USA")) == 0)
	{
		n_value = 0;
	}
	else if (temp_string.CompareNoCase(_T("China")) == 0)
	{
		n_value = 1;
	}
	else if (temp_string.CompareNoCase(_T("Custom")) == 0)
	{
		n_value = 2;
	}
	if (product_register_value[7] == STM32_PM25)
	{
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, PM25_MODBUS_PM25_AREA, n_value,
			product_register_value[TATAT_AQ_MODBUS_AQI_AREA], this->m_hWnd, IDC_EDIT_DTERM, _T(" AQI Region "));
	}
	else
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_MODBUS_AQI_AREA, n_value,
			product_register_value[TATAT_AQ_MODBUS_AQI_AREA], this->m_hWnd, IDC_EDIT_DTERM, _T(" AQI Region "));
}



void CTstatAQ::OnEnKillfocusEditLevel1()
{
	// TODO: 在此添加控件通知处理程序代码 - TODO: Add your control notification handler code here

	CString temp_cstring;
	GetDlgItemTextW(IDC_EDIT_LEVEL_1, temp_cstring);
	unsigned int temp_value = unsigned int(_wtoi(temp_cstring));
	if (product_register_value[7] == STM32_PM25)
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, PM25_MODBUS_AQI_FIRST_LINE, temp_value,
			product_register_value[PM25_MODBUS_AQI_FIRST_LINE], this->m_hWnd, IDC_EDIT_LEVEL_1, _T(" Custom Value 1 "));
	else
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_MODBUS_AQI_CUSTOMER_FIRST_LINE, temp_value,
			product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_FIRST_LINE], this->m_hWnd, IDC_EDIT_LEVEL_1, _T(" Custom Value 1 "));
}


void CTstatAQ::OnEnKillfocusEditLevel2()
{
	// TODO: 在此添加控件通知处理程序代码 - TODO: Add your control notification handler code here
	CString temp_cstring;
	GetDlgItemTextW(IDC_EDIT_LEVEL_2, temp_cstring);
	unsigned int temp_value = unsigned int(_wtoi(temp_cstring));
	if (product_register_value[7] == STM32_PM25)
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, PM25_MODBUS_AQI_SECOND_LINE, temp_value,
			product_register_value[PM25_MODBUS_AQI_SECOND_LINE], this->m_hWnd, IDC_EDIT_LEVEL_2, _T(" Custom Value 2 "));
	else
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_MODBUS_AQI_CUSTOMER_SECOND_LINE, temp_value,
			product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_SECOND_LINE], this->m_hWnd, IDC_EDIT_LEVEL_2, _T(" Custom Value 2 "));
}


void CTstatAQ::OnEnKillfocusEditLevel3()
{
	// TODO: 在此添加控件通知处理程序代码 - TODO: Add your control notification handler code here
	CString temp_cstring;
	GetDlgItemTextW(IDC_EDIT_LEVEL_3, temp_cstring);
	unsigned int temp_value = unsigned int(_wtoi(temp_cstring));
	if (product_register_value[7] == STM32_PM25)
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, PM25_MODBUS_AQI_THIRD_LINE, temp_value,
			product_register_value[PM25_MODBUS_AQI_THIRD_LINE], this->m_hWnd, IDC_EDIT_LEVEL_3, _T(" Custom Value 3 "));
	else
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_MODBUS_AQI_CUSTOMER_THIRD_LINE, temp_value,
			product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_THIRD_LINE], this->m_hWnd, IDC_EDIT_LEVEL_3, _T(" Custom Value 3 "));
}


void CTstatAQ::OnEnKillfocusEditLevel4()
{
	// TODO: 在此添加控件通知处理程序代码 - TODO: Add your control notification handler code here
	CString temp_cstring;
	GetDlgItemTextW(IDC_EDIT_LEVEL_4, temp_cstring);
	unsigned int temp_value = unsigned int(_wtoi(temp_cstring));
	if (product_register_value[7] == STM32_PM25)
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, PM25_MODBUS_AQI_FOURTH_LINE, temp_value,
			product_register_value[PM25_MODBUS_AQI_FOURTH_LINE], this->m_hWnd, IDC_EDIT_LEVEL_4, _T(" Custom Value 4 "));
	else
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_MODBUS_AQI_CUSTOMER_FOURTH_LINE, temp_value,
			product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_FOURTH_LINE], this->m_hWnd, IDC_EDIT_LEVEL_4, _T(" Custom Value 4 "));
}


void CTstatAQ::OnEnKillfocusEditLevel5()
{
	// TODO: 在此添加控件通知处理程序代码 - TODO: Add your control notification handler code here
	CString temp_cstring;
	GetDlgItemTextW(IDC_EDIT_LEVEL_5, temp_cstring);
	unsigned int temp_value = unsigned int(_wtoi(temp_cstring));
	if (product_register_value[7] == STM32_PM25)
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, PM25_MODBUS_AQI_FIFTH_LINE, temp_value,
			product_register_value[PM25_MODBUS_AQI_FIFTH_LINE], this->m_hWnd, IDC_EDIT_LEVEL_5, _T(" Custom Value 5 "));
	else
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_MODBUS_AQI_CUSTOMER_FIFTH_LINE, temp_value,
			product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_FIFTH_LINE], this->m_hWnd, IDC_EDIT_LEVEL_5, _T(" Custom Value 5 "));
}



#include "TstatAQI_Detail.h"
void CTstatAQ::OnBnClickedButtonCusAqi()
{
	// TODO: 在此添加控件通知处理程序代码 - TODO: Add your control notification handler code here
	CTstatAQI_Detail dlg;
	dlg.DoModal();
}

#include "CO2_AUTO_CALIBRATION.h"
void CTstatAQ::OnBnClickedButtonAutoCal()
{
	// TODO: 在此添加控件通知处理程序代码 - TODO: Add your control notification handler code here
	CCO2_AUTO_CALIBRATION Dlg;
	Dlg.DoModal();

}


void CTstatAQ::Initial_List()
{
	m_airlab_list.ShowWindow(SW_HIDE);
	m_airlab_list.DeleteAllItems();
	while (m_airlab_list.DeleteColumn(0));

	m_airlab_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	//m_airlab_list.SetExtendedStyle(m_airlab_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_airlab_list.SetExtendedStyle(m_airlab_list.GetExtendedStyle() | LVS_EX_GRIDLINES & (~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_airlab_list.InsertColumn(AIRLAB_TYPE, _T("Type"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_airlab_list.InsertColumn(AIRLAB_VALUE, _T("Value"), 60, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_airlab_list.InsertColumn(AIRLAB_UNIT, _T("Unit"), 75, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_airlab_list.InsertColumn(AIRLAB_TRIGGER, _T("Trigger Value"), 75, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_airlab_list.InsertColumn(AIRLAB_TIME, _T("Trigger Time(m)"), 90, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_airlab_list.InsertColumn(AIRLAB_TIME_LEFT, _T("Time Left(s)"), 70, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_airlab_list.InsertColumn(AIRLAB_ALARM, _T("Alarm"), 60, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);

	//m_pragram_dlg_hwnd = this->m_hWnd;
	m_airlab_list.SetListHwnd(this->m_hWnd);

	m_airlab_list.DeleteAllItems();
	for (int i = 0; i < 4; i++)
	{
		CString temp_item, temp_value, temp_cal, temp_filter, temp_status, temp_lable;
		CString temp_des;
		CString temp_units;


		temp_item.Format(_T("%d"), i + 1);
		m_airlab_list.InsertItem(i, temp_item);
		m_airlab_list.SetCellEnabled(i, 0, 0);

		for (int x = 0; x < AIRLAB_MAX_COUNT; x++)
		{
			if ((i % 2) == 0)
				m_airlab_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_airlab_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
		}

	}

	m_airlab_list.SetItemText(0, 0, _T("CO2"));
	m_airlab_list.SetItemText(1, 0, _T("OCC"));
	m_airlab_list.SetItemText(2, 0, _T("Sound"));
	m_airlab_list.SetItemText(3, 0, _T("Light"));

	m_airlab_list.ShowWindow(SW_SHOW);
}

LRESULT CTstatAQ::Fresh_Airlab_Item(WPARAM wParam, LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	if (!((Changed_SubItem == AIRLAB_TRIGGER) || (Changed_SubItem == AIRLAB_TIME)))
	{
		return 1;
	}

	if (Changed_Item > 3)
		return 1;

	CString New_CString = m_airlab_list.GetItemText(Changed_Item, Changed_SubItem);


	if (Changed_Item == AIRLAB_CO2)
	{
		unsigned int temp_value = unsigned int(_wtoi(New_CString));
		if (Changed_SubItem == AIRLAB_TRIGGER)
		{
			Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TSTAT_AQ_CO2_TRIGGER, temp_value,
				product_register_value[TSTAT_AQ_CO2_TRIGGER], this->m_hWnd, NULL, _T(" CO2 Trigger "));
		}
		else if (Changed_SubItem == AIRLAB_TIME)
		{
			Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TSTAT_AQ_CO2_TIMER, temp_value,
				product_register_value[TSTAT_AQ_CO2_TIMER], this->m_hWnd, NULL, _T(" CO2 time "));
		}
	}
	else if (Changed_Item == AIRLAB_OCC)
	{
		unsigned int temp_value = unsigned int(_wtoi(New_CString));
		if (Changed_SubItem == AIRLAB_TRIGGER)
		{
			Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TSTAT_AQ_OCC_TRIGGER, temp_value,
				product_register_value[TSTAT_AQ_OCC_TRIGGER], this->m_hWnd, NULL, _T(" OCC Trigger "));
		}
		else if (Changed_SubItem == AIRLAB_TIME)
		{
			Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TSTAT_AQ_OCC_TIMER, temp_value,
				product_register_value[TSTAT_AQ_OCC_TIMER], this->m_hWnd, NULL, _T(" OCC time "));
		}
	}
	else if (Changed_Item == AIRLAB_SOUND)
	{
		unsigned int temp_value = unsigned int(_wtoi(New_CString));
		if (Changed_SubItem == AIRLAB_TRIGGER)
		{
			Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TSTAT_AQ_SOUND_TRIGGER, temp_value,
				product_register_value[TSTAT_AQ_SOUND_TRIGGER], this->m_hWnd, NULL, _T(" Sound Trigger "));
		}
		else if (Changed_SubItem == AIRLAB_TIME)
		{
			Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TSTAT_AQ_SOUND_TIMER, temp_value,
				product_register_value[TSTAT_AQ_SOUND_TIMER], this->m_hWnd, NULL, _T(" Sound time "));
		}
	}
	else if (Changed_Item == AIRLAB_LIGHT)
	{
		unsigned int temp_value = unsigned int(_wtoi(New_CString));
		if (Changed_SubItem == AIRLAB_TRIGGER)
		{
			Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TSTAT_AQ_LIGHT_TRIGGER, temp_value,
				product_register_value[TSTAT_AQ_LIGHT_TRIGGER], this->m_hWnd, NULL, _T(" Sound Trigger "));
		}
		else if (Changed_SubItem == AIRLAB_TIME)
		{
			Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TSTAT_AQ_LIGHT_TIMER, temp_value,
				product_register_value[TSTAT_AQ_LIGHT_TIMER], this->m_hWnd, NULL, _T(" Sound time "));
		}
	}
	return 1;
}


void CTstatAQ::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值 - TODO: Add your message handler code here and/or call default
	static int  fan_status = 0;
	switch (nIDEvent)
	{
	case 1:
	{

		if (product_register_sensor_flag[0] == 0x55)
		{
			CString temp_value;

			bitset<16> module_type(product_register_sensor_flag[1]);
			if (module_type.test(4) == true) //CO2
			{
				if (product_register_value[TSTAT_AQ_CO2_COUNT_DOWN] > 0)
					product_register_value[TSTAT_AQ_CO2_COUNT_DOWN]--;
				temp_value.Format(_T("%u"), product_register_value[TSTAT_AQ_CO2_COUNT_DOWN]);
				m_airlab_list.SetItemText(0, AIRLAB_TIME_LEFT, temp_value);
			}
			if (module_type.test(3) == true) //OCC
			{
				if (product_register_value[TSTAT_AQ_OCC_COUNT_DOWN] > 0)
					product_register_value[TSTAT_AQ_OCC_COUNT_DOWN]--;
				temp_value.Format(_T("%u"), product_register_value[TSTAT_AQ_OCC_COUNT_DOWN]);
				m_airlab_list.SetItemText(1, AIRLAB_TIME_LEFT, temp_value);
			}

			if (module_type.test(8) == true) //sound
			{
				if (product_register_value[TSTAT_AQ_SOUND_COUNT_DOWN] > 0)
					product_register_value[TSTAT_AQ_SOUND_COUNT_DOWN]--;
				temp_value.Format(_T("%u"), product_register_value[TSTAT_AQ_SOUND_COUNT_DOWN]);
				m_airlab_list.SetItemText(2, AIRLAB_TIME_LEFT, temp_value);
			}

			if (module_type.test(7) == true) //Light
			{
				if (product_register_value[TSTAT_AQ_LIGHT_COUNT_DOWN] > 0)
					product_register_value[TSTAT_AQ_LIGHT_COUNT_DOWN]--;
				temp_value.Format(_T("%u"), product_register_value[TSTAT_AQ_LIGHT_COUNT_DOWN]);
				m_airlab_list.SetItemText(3, AIRLAB_TIME_LEFT, temp_value);
			}
		}
	}
	break;
	case 2:
		KillTimer(2);
		m_time_trigger.Invalidate();
		break;
	default:
		break;
	}

	CFormView::OnTimer(nIDEvent);
}


void CTstatAQ::OnNMClickListAirlab(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码 - TODO: Add your control notification handler code here
	*pResult = 0;

	DWORD dwPos = GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point(GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos));
	m_airlab_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;
	m_airlab_list.Set_Edit(true);
	int nItem = m_airlab_list.SubItemHitTest(&lvinfo);

	long lRow, lCol;
	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;

	if (lRow > m_airlab_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的 - If the click area exceeds the maximum row number, the click is invalid.
		return;
	if (lRow < 0)
		return;

	if (lCol == AIRLAB_TIME)
	{
		CString Temp_CString = m_airlab_list.GetItemText(lRow, lCol);
		Temp_CString.Trim();
		if (Temp_CString.CompareNoCase(_T("-")) == 0)
			return;
		m_row = lRow;
		m_col = lCol;
		m_airlab_list.Set_Edit(false);
		m_time_trigger.ShowWindow(SW_SHOW);
		CRect list_rect, win_rect;
		m_airlab_list.GetWindowRect(list_rect);
		ScreenToClient(list_rect); //转换为客户区矩形 - Convert to client area rectangle
		GetWindowRect(win_rect);
		CRect myrect;
		m_airlab_list.GetSubItemRect(lRow, lCol, LVIR_BOUNDS, myrect);


		myrect.left = myrect.left + list_rect.left;
		myrect.right = myrect.right + list_rect.left + 2;
		myrect.top = myrect.top + list_rect.top;
		myrect.bottom = myrect.top + 20;


		m_time_trigger.BringWindowToTop();
		m_time_trigger.MoveWindow(myrect);



		CStringArray TEMPCS;
		int temp_hour, temp_minute, temp_second;
		SplitCStringA(TEMPCS, Temp_CString, _T(":"));

		if ((int)TEMPCS.GetSize() == 2)
		{
			temp_hour = _wtoi(TEMPCS.GetAt(0));
			temp_minute = _wtoi(TEMPCS.GetAt(1));

			CTime TimeTemp(2021, 1, 1, temp_hour, temp_minute, 0);
			m_time_trigger.SetFormat(_T("HH:mm"));
			m_time_trigger.SetTime(&TimeTemp);
			m_time_trigger.SetFocus();
		}
		else
		{
			temp_hour = _wtoi(TEMPCS.GetAt(0));
			temp_minute = _wtoi(TEMPCS.GetAt(1));
			temp_second = _wtoi(TEMPCS.GetAt(2));
			if (temp_hour >= 24)
				temp_hour = 0;
			if (temp_minute >= 60)
				temp_minute = 0;
			if (temp_second >= 60)
				temp_second = 15;
			CTime TimeTemp(2021, 1, 1, temp_hour, temp_minute, temp_second);
			m_time_trigger.SetFormat(_T("HH:mm"));
			m_time_trigger.SetTime(&TimeTemp);
			m_time_trigger.SetFocus();
		}
		m_airlab_list.SetItemText(lRow, lCol, _T(""));
		m_time_trigger.Invalidate();

		SetTimer(2, 100, NULL);
	}



}


void CTstatAQ::OnNMKillfocusDatetimepickerTimeRemain(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码 - TODO: Add your control notification handler code here
	CTime temp_time; CString temp_cs;
	int chour, cmin, csecond;
	m_time_trigger.GetTime(temp_time);
	chour = temp_time.GetHour();
	cmin = temp_time.GetMinute();
	csecond = temp_time.GetSecond();


	if ((chour == 0) && (cmin == 0) && (csecond == 0))
	{
		temp_cs.Empty();
	}
	else
		temp_cs.Format(_T("%02d:%02d"), chour, cmin);
	m_airlab_list.SetItemText(m_row, m_col, temp_cs);

	m_time_trigger.ShowWindow(SW_HIDE);
	unsigned int temp_value = chour * 60 + cmin;
	if (m_row == AIRLAB_LIGHT)
	{
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TSTAT_AQ_LIGHT_TIMER, temp_value,
			product_register_value[TSTAT_AQ_LIGHT_TRIGGER], this->m_hWnd, NULL, _T(" Light Trigger "));
	}
	else  if (m_row == AIRLAB_SOUND)
	{
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TSTAT_AQ_SOUND_TIMER, temp_value,
			product_register_value[TSTAT_AQ_SOUND_TIMER], this->m_hWnd, NULL, _T(" Sound Trigger "));
	}
	else  if (m_row == AIRLAB_OCC)
	{
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TSTAT_AQ_OCC_TIMER, temp_value,
			product_register_value[TSTAT_AQ_OCC_TIMER], this->m_hWnd, NULL, _T(" OCC Trigger "));
	}
	else  if (m_row == AIRLAB_CO2)
	{
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TSTAT_AQ_CO2_TIMER, temp_value,
			product_register_value[TSTAT_AQ_CO2_TIMER], this->m_hWnd, NULL, _T(" CO2 Trigger "));
	}
	*pResult = 0;
}

#include "CTstatAQ_Parameter.h"
void CTstatAQ::OnBnClickedButtonAirlabParameter()
{
	// TODO: 在此添加控件通知处理程序代码 - TODO: Add your control notification handler code here
	int read_ret = 0;
	if (product_register_value[7] == STM32_PM25)
	{
		read_ret = Read_Multi(g_tstat_id, &product_register_value[100], 100, 100, 5);
		if (read_ret < 0)
		{
			MessageBox(_T("Read data timeout!"));
			return;
		}
		CTstatAQ_PM25_Parameter  mDlg;
		mDlg.DoModal();
	}
	else
	{

		read_ret = Read_Multi(g_tstat_id, &product_register_value[1900], 1900, 100, 5);
		if (read_ret < 0)
		{
			MessageBox(_T("Read data timeout!"));
			return;
		}
		CTstatAQ_Parameter dlg;
		dlg.DoModal();
	}
}

//String show_fan_picture_old;
void CTstatAQ::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码 - Add your message handler code here
	// 不为绘图消息调用 CFormView::OnPaint() - Do not call CFormView::OnPaint() for drawing messages
//CMemDC memDC(dc, this);
//CRect rcClient(100,100,200,200);
//memDC.GetDC().FillSolidRect(&rcClient, RGB(238, 245, 250));

}

#include "ShowMessageDlg.h"
void CTstatAQ::OnBnClickedButtonHelpWbgt()
{
    // TODO: 在此添加控件通知处理程序代码 - TODO: Add your control notification handler code here
    CShowMessageDlg dlg;
    CString TotalMessage;
    TotalMessage = _T("What is WBGT?\r\nWBGT is an experimental forecast tool indicating expected heat stress on the human body when in direct sunlight. It estimates the effect of temperature, relative humidity, wind speed, and solar radiation on humans using a combination of temperatures from three thermometers:\r\n\
A Wet bulb measures the temperature read by a thermometer covered in a wet cloth.As water evaporates from the cloth, evaporation cools the thermometer.This mirrors how the human body cools itself with sweat.\r\n\
A black globe is used to measure solar radiation. Solar radiation heats the globe and wind blowing across it cools the globe.\r\n\
A Dry bulb calculates the air temperature measured in the shade. It is the temperature you would see on your thermometer outside.\r\n\
With WBGT forecasts available by region up to 7 days in advance, it is a useful tool for planning ahead.\r\n\r\n\
Who should use WBGT?\r\n\
This experimental tool is most useful for active, acclimatized people such as outdoor workers, athletes, and anyone else performing strenuous outdoor activities — and has been used for decades by military agencies, OSHA, and marathon organizers.");

    dlg.SetStaticText(TotalMessage);
    //dlg.SetStaticTextBackgroundColor(RGB(222, 222, 222));
    dlg.SetStaticTextColor(RGB(0, 0, 0));
    dlg.SetStaticTextSize(20, 14);
    dlg.SetEvent(EVENT_MESSAGE_ONLY);
    dlg.SetMessageWindowSize(10, 10, 1000, 450);
    dlg.DoModal();
}
