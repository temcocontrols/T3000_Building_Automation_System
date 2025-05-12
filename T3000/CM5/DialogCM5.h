#pragma once
#include "afxwin.h"

#include "../msflexgrid1.h"
#include "../global_function.h"
#include "../Weekly_Routines.h"
#include "../Annual_Routines.h"
#include "../Config_Routines.h"
#include "../global_variable_extern.h"

#include "../MainFrm.h"
#include "afxcmn.h"
#include "../msflexgrid1.h"




#define FLEXGRID_CELL_COLOR						13421772
#define FLEXGRID_CELL_GRAY_COLOR				13421772	

#define  DATATIME_TIMER 1
#define THE_INPUT_OUTPUT_LENGTH 14
typedef struct Subnetwork						
{						
	CString	m_coolingSet;				
	CString	m_heatingSet;				
	CString	m_setpoint; 				
	CString	m_roomTemper;				
	int	    m_mode;				
	CString	m_outputState;				
	CString	m_nightHeating;				
	CString	m_nightCooling;				
	int	m_tstatProduct;				
	CString m_modbusaddr;
	int  m_Occupied;
	CString  m_overridevalue;
	CString m_SerialNumber; 
	CString	m_nightHeatingDB;				
	CString	m_nightCoolingDB;						
}subnetwork;




// CDialogCM5 form view

class CDialogCM5 : public CFormView
{
	DECLARE_DYNCREATE(CDialogCM5)

protected:
int m_type;
public:
	CDialogCM5();           // protected constructor used by dynamic creation
	virtual ~CDialogCM5();

public:
	enum { IDD = IDD_DIALOG_CM5 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


public:
	int m_IDaddressCM5;
	UINT m_nSerialNumCM5;
	float m_firmwareVersionCM5;
	int m_hardware_versionCM5;
	CString m_strDateCM5;
	CString m_strTime;
	CString m_heating;
	CString m_cooling;
	CString m_coating;
	CString m_result;
	CComboBox m_combo_prioritymode;
	int m_priorityinterval;
	CString m_countprioritytime;
	CMsflexgrid m_Output_GridCM5_;
	CMsflexgrid m_msflexgrid_input;
	CMsflexgrid m_msflexgridoutput;
	CEdit m_edit_input;
	CEdit m_edit_output;
	CComboBox m_combobox2_input;
	CString m_static_datetime;
	CString m_combo2_cstring;

	CString m_tempGridString;
	HANDLE CM5_OutputThread;
	BOOL b_is_fresh;
public://events
	afx_msg void OnBnClickedButtonSyncwithpc();
	afx_msg void OnBnClickedButtonSaveConfig();
	afx_msg void OnBnClickedButtonweeklyschedule();
	afx_msg void OnBnClickedButtonRefreshAll();
	afx_msg void OnBnClickedButtonannualschedule();
	afx_msg void OnBnClickedButtonidschedule();
	afx_msg void OnBnClickedButtonCheckDemo();
	afx_msg void OnBnClickedButtoninputtype();
	afx_msg void OnEnKillfocusEditInput();
	afx_msg void OnEnKillfocusEditoutput();
	afx_msg void OnClose();
	afx_msg void OnEnKillfocusEditNum();
	afx_msg void OnEnKillfocusEditInterval();
	afx_msg void OnEnKillfocusEditDTS1();
	afx_msg void OnEnKillfocusEditDTS2();
	afx_msg void OnEnKillfocusEditDTS3();
	afx_msg void OnEnKillfocusEditDTS4();
	afx_msg void OnEnKillfocusEditDTS5();
	afx_msg void OnEnKillfocusEditDTS6();
	afx_msg void OnEnKillfocusEditDTS7();
	afx_msg void OnEnKillfocusEditDTS8();
	afx_msg void OnEnKillfocusEditDTS9();
	afx_msg void OnEnKillfocusEditDTS10();
	afx_msg LRESULT DealWithMessage(WPARAM wParam,LPARAM lParam);
public://messages override
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString GetTextReg(unsigned short reg,BOOL IsRead);
	 BOOL  Get_Data_Bit(UINT Data, int n,int N);//获取一个第八位数据，s:system:进制，n;num:这个数是几位的，第N位是1还是0

public:
	void Fresh();
	void ShowDialogData();
	
	int Get_RegID(int lRow);
	void Show_DisplayEdit(int num);
public:

	vector<subnetwork>_subnetwork;
	subnetwork m_subetwork;
private:
	int m_switch;
	int m_cellrow;
	int m_cellcol;
	int m_cellrowout;
	int m_cellcolout;
	int num;
	
	DECLARE_EVENTSINK_MAP()
	void ClickInputMsflexgrid();
	void DblClickInputMsflexgrid();

	void ClickOutputMsflexgrid();
	void DblClickOutputMsflexgrid();
	void DBClickInputMsflexgrid2();
	void ClickSubTstatgrid();
	void ClickInputMsflexgrid2();
	void DblClickInputMsflexgrid2()	;
	void OnCbnSelchangeCombx();
	void OnCbnSelchangeType();
	void UpdateGrid();
public:
	CString m_bootloaderversion;
	CComboBox m_Baudrate;
	CComboBox m_ipModelComBox;
	CIPAddressCtrl m_ip_addressCtrl;
	CIPAddressCtrl m_subnet_addressCtrl;
	CIPAddressCtrl m_gateway_addressCtrl;
	CString m_nListenPort;
	CEdit m_Mac_Address;
private:
	CButton m_checkdemo;
public:
	CComboBox m_combox_enable;
	int m_Display_Num;
	int m_Interval;
	int m_dts1;
	int m_dts2;
	int m_dts3;
	int m_dts4;
	int m_dts5;
	int m_dts6;
	int m_dts7;
	int m_dts8;
	int m_dts9;
	int m_dts10;
	CProgressCtrl m_progressctrl;
	CMsflexgrid m_subtstatgrid;
	CMsflexgrid m_DIGrid;
};


