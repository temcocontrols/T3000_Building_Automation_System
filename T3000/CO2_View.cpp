// CO2_View.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "CO2_View.h"
#include "globle_function.h"

// CCO2_View

IMPLEMENT_DYNCREATE(CCO2_View, CFormView)

CCO2_View::CCO2_View()
	: CFormView(CCO2_View::IDD)
	, m_alarm_ctrl_manual_auto(FALSE)
	, m_alarm_on_time(0)
	, m_alarm_off_time(0)
	, m_edit_pre_alarm_setpoint(0)
	, m_edit_alarm_setpoint(0)
	, m_edit_calibrating_offset(0)
{

	m_co2_firmwareversion = 0.0f;
	m_co2_hardwareversion = 0;
	m_co2_serialNumber = 0;
}

CCO2_View::~CCO2_View()
{
}

void CCO2_View::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ID_CO2_EDIT, m_co2_idAdressEdit);
	DDX_Control(pDX, IDC_CO2_BRAUDRATECOMBO, m_co2_braudRateCombox);
	DDX_Text(pDX, IDC_FIRMWARV_CO2_EDIT, m_co2_firmwareversion);
	DDX_Text(pDX, IDC_HARDWAREV_CO2_EDIT, m_co2_hardwareversion);
	DDX_Text(pDX, IDC_SERIALNUM_CO2_EDIT, m_co2_serialNumber);
	DDX_Control(pDX, IDC_CO2_TEMP_UNIT, m_co2_temp_unit);
	DDX_Control(pDX, IDC_CO2_ALARM_STATE, m_co2_alarm_state);
	DDX_Control(pDX, IDC_RADIO_ALARM_MANUAL, m_co2_alarm_mode_ctr);
	DDX_Radio(pDX, IDC_RADIO_ALARM_MANUAL, m_alarm_ctrl_manual_auto);
	DDX_Text(pDX, IDC_CO2_ALARM_ON_TIME, m_alarm_on_time);
	DDX_Text(pDX, IDC_CO2_ALARM_OFF_TIME, m_alarm_off_time);
	DDX_Text(pDX, IDC_CO2_PREPARE_ALARM_SETPOINT, m_edit_pre_alarm_setpoint);
	DDX_Text(pDX, IDC_CO2_ALARM_SETPOINT, m_edit_alarm_setpoint);
	DDX_Text(pDX, IDC_CO2_CALIBRATING_OFFSET, m_edit_calibrating_offset);
	DDX_Control(pDX, IDC_MSFLEXGRID_CO2, m_flex_co2_grid);
}

BEGIN_MESSAGE_MAP(CCO2_View, CFormView)
	ON_BN_CLICKED(IDC_CO2_ENABLEIDBUTTON, &CCO2_View::OnBnClickedCo2Enableidbutton)
END_MESSAGE_MAP()


// CCO2_View diagnostics

#ifdef _DEBUG
void CCO2_View::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCO2_View::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CCO2_View::Fresh()
{
	CString temp_id;

	float m_fFirmwareVersion=0;
	UINT m_nSerialNumber=0;
	unsigned short m_nHardwareVersion=0;
	temp_id.Format(_T("%d"),product_register_value[6]);
	m_co2_idAdressEdit.SetWindowTextW(temp_id);

	if(product_register_value[105]>=0&&product_register_value[105]<=1)	
		m_co2_braudRateCombox.SetCurSel(product_register_value[105]);
	else
		m_co2_braudRateCombox.SetCurSel(0);

	m_co2_firmwareversion=get_curtstat_version();
	m_co2_serialNumber=get_serialnumber();
	m_co2_hardwareversion=product_register_value[8];//8
	m_co2_temp_unit.ResetContent();
	m_co2_temp_unit.AddString(_T("¡æ"));
	m_co2_temp_unit.AddString(_T("¨H"));
	Get_CO2_Temperature_unit(cs_temp_unit);
	m_co2_temp_unit.SetCurSel(product_register_value[103]);

	CO2_Alarm_Set();
	CO2_Grid_init();

	UpdateData(FALSE);
}

void CCO2_View::CO2_Grid_init()
{
	m_flex_co2_grid.Clear();
	m_flex_co2_grid.put_Rows(11);
	for(int i=1;i<=10;i++)
	{
		CString strTemp;
		strTemp.Format(_T("%d"),i);
		m_flex_co2_grid.put_TextMatrix(i,0,strTemp);
	}
	m_flex_co2_grid.put_Cols(5);

	m_flex_co2_grid.put_TextMatrix(0,0,_T(""));
	m_flex_co2_grid.put_ColWidth(0,400);

	m_flex_co2_grid.put_TextMatrix(0,CO2_PPM,_T("CO2 PPM"));
	m_flex_co2_grid.put_ColWidth(CO2_PPM,1000);

	m_flex_co2_grid.put_TextMatrix(0,PREPARE_ALARM_PPM,_T("Prepare Alarm PPM"));
	m_flex_co2_grid.put_ColWidth(PREPARE_ALARM_PPM, 1600);

	m_flex_co2_grid.put_TextMatrix(0,ALARM_PPM,_T("Alarm PPM"));
	m_flex_co2_grid.put_ColWidth(ALARM_PPM,1000);

	m_flex_co2_grid.put_TextMatrix(0,CALIBRATION_PPM_OFFSET,_T("Calibration PPM Offset"));
	m_flex_co2_grid.put_ColWidth(CALIBRATION_PPM_OFFSET,1800);
}


void CCO2_View::CO2_Alarm_Set()
{
	CString cs_alarm_on_time,cs_alarm_off_time;
	bool alarm_mode=ALARM_AUTO;
	unsigned short temp_data=product_register_value[118];
	temp_data=temp_data>>7;
	if(temp_data==1)
	{
		m_alarm_ctrl_manual_auto = 0;
		m_co2_alarm_state.EnableWindow(TRUE);
		GetDlgItem(IDC_CO2_ALARM_ON_TIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_CO2_ALARM_OFF_TIME)->EnableWindow(TRUE);
	}
	else
	{
		m_alarm_ctrl_manual_auto = 1;
		m_co2_alarm_state.EnableWindow(FALSE);
		GetDlgItem(IDC_CO2_ALARM_ON_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_CO2_ALARM_OFF_TIME)->EnableWindow(FALSE);
	}
	temp_data=product_register_value[118];
	temp_data=temp_data & 0x0003;
	if(temp_data<3)
	m_co2_alarm_state.SetCurSel(temp_data);

	cs_alarm_on_time.Format(_T("%d"),product_register_value[119]);
	cs_alarm_off_time.Format(_T("%d"),product_register_value[120]);

	m_alarm_on_time = product_register_value[119];
	m_alarm_off_time = product_register_value[120];

	m_edit_pre_alarm_setpoint = product_register_value[122];
	m_edit_alarm_setpoint = product_register_value[123];
	m_edit_calibrating_offset = product_register_value[124];
	UpdateData(FALSE);
	//if(product_register_value[118])
}

void CCO2_View::Get_CO2_Temperature_unit(CString &strTemp)
{
	UINT uint_temp=GetOEMCP();//get system is for chinese or english
	if(uint_temp!=936 && uint_temp!=950)
	{
		if(product_register_value[MODBUS_DEGC_OR_F]==0)	//121
		{
			strTemp.Format(_T("%cC"),176);
		}
		else
		{
			strTemp.Format(_T("%cF"),176);
		}
	}
	else
	{
		//Chinese.
		if(product_register_value[MODBUS_DEGC_OR_F]==0)//121
		{
			strTemp=_T("¡æ");
		}
		else
		{
			strTemp=_T("¨H");
		}
	}
}
// CCO2_View message handlers



void CCO2_View::OnBnClickedCo2Enableidbutton()
{
	// TODO: Add your control notification handler code here
	if(m_co2_idAdressEdit.IsWindowEnabled())
	{
		m_co2_idAdressEdit.EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_CO2_ENABLEIDBUTTON))->SetWindowTextW(_T("Enable Change"));
	}
	else
	{
		m_co2_idAdressEdit.EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_CO2_ENABLEIDBUTTON))->SetWindowTextW(_T("Disable Change"));
	}
}
BEGIN_EVENTSINK_MAP(CCO2_View, CFormView)
	ON_EVENT(CCO2_View, IDC_MSFLEXGRID_CO2, DISPID_CLICK, CCO2_View::ClickMsflexgridCo2, VTS_NONE)
END_EVENTSINK_MAP()


void CCO2_View::ClickMsflexgridCo2()
{
	// TODO: Add your message handler code here
}
