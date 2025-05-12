#pragma once
#include "afxcmn.h"

#include "CM5/ListCtrlEx.h"

// CZigbeeRepeater 窗体视图

class CZigbeeRepeater : public CFormView
{
	DECLARE_DYNCREATE(CZigbeeRepeater)

public:
	CZigbeeRepeater();           // 动态创建所使用的受保护的构造函数
	virtual ~CZigbeeRepeater();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ZIGBEE_REPEATER };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
    void Fresh();
    virtual void OnInitialUpdate();
    afx_msg void OnCbnSelchangeComboRepeaterBaudrate();
    afx_msg void OnCbnSelchangeComboRepeaterMode();
    afx_msg void OnEnKillfocusEditModbusid();
    afx_msg void OnEnKillfocusEditChannel();
    afx_msg void OnEnKillfocusEditPanid();
    void UpdateDeviceDB(int nmodbusid, CString cs_baudrate);
    CString cs_serial_numer;
    ListCtrlEx::CListCtrlEx m_zigbee_repeater_list;
    void Initial_List();
    void Initial_Moudle_List();
    afx_msg void OnBnClickedButtonZigbeeRefresh();
    ListCtrlEx::CListCtrlEx m_zigbee_input_list;
};

const CString Repeater_Baudrate_Array[] =
{
    _T("9600"),
    _T("19200"), //6
    _T("38400"),
    _T("57600"),
    _T("115200")	//9
};

const CString Repeater_Mode_Array[] =
{
    _T("Coordinator"), //0 
    _T("Router")       //1
};

const int ZIGBEE_MOUDLE_ITEM = 0;
const int ZIGBEE_MOUDLE_MODBUSID = 1;
const int ZIGBEE_MOUDLE_VOLTAGE = 2;
const int ZIGBEE_MOUDLE_STATUS = 3;
const int ZIGBEE_MOUDLE_TEMPERATURE_1 = 4;
const int ZIGBEE_MOUDLE_TEMPERATURE_2 = 5;
const int ZIGBEE_MOUDLE_RSSI = 6;


const CString Repeater_STATUS_Array[] =
{
    _T("OFF"), //0 
    _T("ON"),   //1
    _T("UNKNOWN")
};
