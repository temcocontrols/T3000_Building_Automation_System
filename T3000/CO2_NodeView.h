#pragma once

#include "CM5/ListCtrlEx.h"

// CCO2_NodeView 窗体视图

class CCO2_NodeView : public CFormView
{
	DECLARE_DYNCREATE(CCO2_NodeView)
public:
    CCO2_NodeView();           // 动态创建所使用的受保护的构造函数
    virtual ~CCO2_NodeView();
protected:
	
	

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CO2_NODE };
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
    afx_msg void OnEnChangeEditCo2NodeRollTimeInterval();
    ListCtrlEx::CListCtrlEx m_co2_node_list;
    void Initial_List();
    virtual void OnInitialUpdate();
    afx_msg LRESULT CCO2_NodeView::Fresh_CO2_Node_Item(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT CCO2_NodeView::Fresh_CO2_Node_List(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT CCO2_NodeView::UpdateUI(WPARAM wParam, LPARAM lParam);
    void Update_Static_UI();
    afx_msg void OnEnChangeEditCo2NodeModbusId();
    afx_msg void OnEnChangeEditCo2NodeBacklightTime();
    afx_msg void OnCbnSelchangeComboCo2NodeBaudrate();
    afx_msg void OnCbnSelchangeComboCo2NodeProtocol();
    afx_msg void OnCbnSelchangeComboCo2NodeTemperatureUnit();
    afx_msg void OnBnClickedRadio1();
    afx_msg void OnBnClickedRadio2();
    afx_msg void OnBnClickedRadio3();
    afx_msg void OnBnClickedRadio4();
    afx_msg void OnBnClickedRadio5();
    afx_msg void OnBnClickedRadio6();
    afx_msg void OnBnClickedRadio7();
    afx_msg void OnBnClickedRadio8();
    afx_msg void OnBnClickedRadio9();
    afx_msg void OnBnClickedRadio10();
    afx_msg void OnBnClickedRadio11();
    afx_msg void OnBnClickedRadio12();
    afx_msg void OnBnClickedRadio13();
    afx_msg void OnBnClickedRadio14();
    afx_msg void OnBnClickedRadio15();
    afx_msg void OnBnClickedRadio16();
    void UpdateRadio_Main();
    void UpdateRadio_ScrollDisplay();
    void Show_Main_radio();
    void Show_Scroll_radio();
    afx_msg void OnBnClickedButtonOk();
    afx_msg void OnEnKillfocusEditCo2NodeModbusId();
    afx_msg void OnEnKillfocusEditCo2NodeRollTimeInterval();
    afx_msg void OnEnKillfocusEditCo2NodeBacklightTime();
    afx_msg void OnEnKillfocusEditFairAlarmSetpoint();
    afx_msg void OnEnKillfocusEditPoorAlarmSetpoint();
    afx_msg void OnBnClickedRadioScroll1();
    afx_msg void OnBnClickedRadioScroll2();
    afx_msg void OnBnClickedRadioScroll3();
    afx_msg void OnBnClickedRadioScroll4();
    afx_msg void OnBnClickedRadioScroll5();
    afx_msg void OnBnClickedRadioScroll6();
    afx_msg void OnBnClickedRadioScroll7();
    afx_msg void OnBnClickedRadioScroll8();
    afx_msg void OnBnClickedRadioScroll9();
    afx_msg void OnBnClickedRadioScroll10();
    afx_msg void OnBnClickedRadioScroll11();
    afx_msg void OnBnClickedRadioScroll12();
    afx_msg void OnBnClickedRadioScroll13();
    afx_msg void OnBnClickedRadioScroll14();
    afx_msg void OnBnClickedRadioScroll15();
    afx_msg void OnBnClickedRadioScroll16();
    afx_msg void OnBnClickedRadioScroll17();
    afx_msg void OnBnClickedRadioScroll18();
    afx_msg void OnBnClickedRadioScroll19();
    afx_msg void OnBnClickedRadioScroll20();
    afx_msg void OnBnClickedRadioScroll21();
    afx_msg void OnBnClickedRadioScroll22();
};
static DWORD WINAPI UpdateCO2_Note_Thread(LPVOID lPvoid);
const int CO2_NODE_TYPE_NAME        = 0;
const int CO2_NODE_TYPE_VALUE       = 1;
const int CO2_NODE_NAME_UNIT        = 2;
const int CO2_NODE_SETPOINT         = 3;
const int CO2_NODE_OUTPUT_VALUE     = 4;
const int CO2_NODE_OUTPUT_RANGE = 5;
const int CO2_NODE_MIN_VALUE        = 6;
const int CO2_NODE_MAX_VALUE        = 7;
const int CO2_NODE_CALIBRATION = 8;


const int ROW_CO2 = 0;
const int ROW_TEMPERATURE = 1;
const int ROW_HUMIDITY = 2;

