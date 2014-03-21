// CO2NetView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "CO2NetView.h"
#include "MainFrm.h"


// CCO2NetView

IMPLEMENT_DYNCREATE(CCO2NetView, CFormView)

CCO2NetView::CCO2NetView()
	: CFormView(CCO2NetView::IDD)
{
Flag_Reg=FALSE;
}

CCO2NetView::~CCO2NetView()
{
}

void CCO2NetView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ID_CO2_EDIT, m_edit_IDAddress);
    DDX_Control(pDX, IDC_SERIALNUM_CO2_EDIT, m_edit_SN);
    DDX_Control(pDX, IDC_FIRMWARV_CO2_EDIT, m_Edit_FVevsion);
    DDX_Control(pDX, IDC_HARDWAREV_CO2_EDIT, m_Edit_HardVer);
    DDX_Control(pDX, IDC_CO2_BRAUDRATECOMBO, m_Combox_braudrate);
    DDX_Control(pDX, IDC_EDIT_CO2_HUMIDITY, m_Edit_RHum);
    DDX_Control(pDX, IDC_RADIO_ALARM_MANUAL, m_AlarmSetting_M);
    DDX_Control(pDX, IDC_RADIO_ALARM_AUTO, m_AlarmSetting_Auto);
    DDX_Control(pDX, IDC_CO2_ALARM_STATE, m_Combox_CO2State);

    DDX_Control(pDX, IDC_EDIT_CO2_PASSWOR, m_Edit_Password);
    DDX_Control(pDX, IDC_COMBO_IPModel, m_Combox_IpModel);
    DDX_Control(pDX, IDC_MAC_ADDRESS, m_Edit_MACAddress);
    DDX_Control(pDX, IDC_IPADDRESS_IP, m_IPCtrl_IpAddress);
    DDX_Control(pDX, IDC_IPADDRESS_Gateway, m_IpCtrl_Gateway);
    DDX_Control(pDX, IDC_IPADDRESS_Subnet, m_IpCtrl_Subnet);
    DDX_Control(pDX, IDC_EDIT_ListeningPort, m_Edit_Port);
    DDX_Control(pDX, IDC_EDIT_EXTERNAL_TEMP, m_Edit_External_Temp);
    DDX_Control(pDX, IDC_EDIT_INTERNAL_TEMP, m_Internal_Temp);
    DDX_Control(pDX, IDC_CO2_TEMP_UNIT, m_Combox_TempUnit);
    DDX_Control(pDX, IDC_COMBO_CO2_SENSOR_SEL, m_Edit_SensorSelect);
    DDX_Control(pDX, IDC_CO2_CALIBRATING_OFFSET, m_Edit_Calibrate_Offset);
    DDX_Control(pDX, IDC_CO2_ALARM_SETPOINT, m_Edit_Alarm_Setpoint);
    DDX_Control(pDX, IDC_CO2_PREPARE_ALARM_SETPOINT, m_Edit_PreAlarm_Setpoint);
    DDX_Control(pDX, IDC_CO2_ALARM_ON_TIME, m_Edit_AlarmOn);
    DDX_Control(pDX, IDC_CO2_ALARM_OFF_TIME, m_Edit_AlarmOff);
    DDX_Control(pDX, IDC_EDIT_CO2_BLOCK_TIME, m_Edit_MenuBlockTimes);
    DDX_Control(pDX, IDC_EDIT_CO2_BACKLIGHT_TIME, m_Edit_BacklightTime);
    DDX_Control(pDX, IDC_RADIO_HUMIDITY_HEAT_ENABLE, m_HeatingHumiditySensor_Enable);
    DDX_Control(pDX, IDC_RADIO_HUMIDITY_HEAT_DISABLE, m_HeatingHumiditySensor_Disable);
    DDX_Control(pDX, IDC_ENABLE_PASSWORD, m_Check_Enable_Password);
    DDX_Control(pDX, IDC_MSFLEXGRID1, m_msflexgrid);
}

BEGIN_MESSAGE_MAP(CCO2NetView, CFormView)
    ON_CBN_SELCHANGE(IDC_CO2_TEMP_UNIT, &CCO2NetView::OnCbnSelchangeCo2TempUnit)
    ON_BN_CLICKED(IDC_BTN_CO2_CLEAR_CAL, &CCO2NetView::OnBnClickedBtnCo2ClearCal)
END_MESSAGE_MAP()


// CCO2NetView diagnostics
void CCO2NetView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
     
}
#ifdef _DEBUG
void CCO2NetView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCO2NetView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CCO2NetView::Fresh(){
if (!Flag_Reg)
{
Initial_Registerlist();
Flag_Reg=TRUE;
}
for(int i=0;i<27;i++){
Read_Multi(g_tstat_id,&product_register_value[i*100],i*100,100);
}
Initial_Window();
}
void CCO2NetView::Initial_Window(){
CString strTemp;
int IntTemp;
IntTemp=product_register_value[CO2_NET_MODBUS_ADDRESS];
strTemp.Format(_T("%d"),IntTemp);
m_edit_IDAddress.SetWindowText(strTemp);
IntTemp=product_register_value[CO2_NET_MODBUS_SERIALNUMBER_LOWORD]+product_register_value[CO2_NET_MODBUS_SERIALNUMBER_LOWORD+1]*255+
        product_register_value[CO2_NET_MODBUS_SERIALNUMBER_HIWORD]*255*255+product_register_value[CO2_NET_MODBUS_SERIALNUMBER_HIWORD+1]*255*255*255;
strTemp.Format(_T("%d"),IntTemp);
m_edit_SN.SetWindowText(strTemp);
strTemp.Format(_T("%u.%u"),product_register_value[CO2_NET_MODBUS_SOFTWARE_VERSION_HI],product_register_value[CO2_NET_MODBUS_SOFTWARE_VERSION_LO]);
m_Edit_FVevsion.SetWindowText(strTemp);
strTemp.Format(_T("%u"),product_register_value[CO2_NET_MODBUS_HARDWARE_REV]);
m_Edit_HardVer.SetWindowText(strTemp);
m_Combox_braudrate.ResetContent();
m_Combox_braudrate.AddString(_T("9600"));
m_Combox_braudrate.AddString(_T("19200"));
m_Combox_braudrate.SetCurSel(product_register_value[CO2_NET_MODBUS_BAUDRATE]);
float fTemp;
fTemp = (float)product_register_value[CO2_NET_MODBUS_HUMIDITY]/10;
if(fTemp>100)
{
    (CEdit *)GetDlgItem(IDC_EDIT_CO2_HUMIDITY)->EnableWindow(FALSE);
    //m_humidity_value = 0;
    m_Edit_RHum.SetWindowText(_T("0"));
}
else
{
    strTemp.Format(_T("%0.1f"),fTemp);
    m_Edit_RHum.SetWindowText(strTemp);
    (CEdit *)GetDlgItem(IDC_EDIT_CO2_HUMIDITY)->EnableWindow(TRUE);
}

// CCO2NetView message handlers
void CCO2NetView::OnCbnSelchangeCo2TempUnit()
{
     
}
#include "Table_Tamplete.h"

void CCO2NetView::OnBnClickedBtnCo2ClearCal()
{
     CTable_Tamplete dlg;
    // dlg=new CTable_Tamplete;
   //  dlg->SetWindowText(_T("External Calibration List"));
     dlg.DoModal();
}