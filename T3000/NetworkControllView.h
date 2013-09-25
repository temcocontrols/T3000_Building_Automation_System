#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "msflexgrid1.h"


// CNetworkControllView form view
class CNetworkControllView : public CFormView
{
	DECLARE_DYNCREATE(CNetworkControllView)

public:
	CNetworkControllView();           // protected constructor used by dynamic creation
	virtual ~CNetworkControllView();

public:
	enum { IDD = IDD_NETWORKCONTROLLVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	float GetFirmwareVersion();
	BOOL CheckSettingChanged();
	void AddNewTimeServer(const CString& strTimeServer);  // 把新增的时间服务器加到列表里，并写入寄存器
	BOOL SaveTimeServerToRegister(const CString& strVal, const CString& strKey);
	BOOL LoadTimeServerFromRegister(const CString& strKey ,CString& strVal);
	void initTimeServerList();
	void InitGrid();
	void  ReadNCTable();
	void  AddNodeToNCTable(unsigned short* pNode);
public:
	virtual void OnInitialUpdate();
	CString GetIPFromHostName(CString& strHostName);
	void GetTstatInfo(int nID);
	void Fresh();
	CString  Get_MAC_Address();
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedTimesetbtn();	
	afx_msg void OnBnClickedGetbtn();
	afx_msg void OnBnClickedSavebtn();
	afx_msg void OnBnClickedRebootbtn();
	afx_msg void OnBnClickedFarcallbtn();
	afx_msg void OnBnClickedFarcallipBtn();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedAnuualbtn();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedApplybutton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeBaudrateCombx();
	afx_msg void OnEnKillfocusIdaddressEdit();
	afx_msg void OnBnClickedButtonUpdatetimeserver();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//afx_msg void OnBnClickedFindTstat();



public:
	CComboBox			m_baudRateCombox;
	int							m_nListenPort;
	BOOL						m_bWarningBldVersion;

	CButton					m_ReadOnlyCheckBtn;
	CComboBox			m_ipModelComBox;
	CIPAddressCtrl			m_ip_addressCtrl;
	CIPAddressCtrl			m_subnet_addressCtrl;
	CIPAddressCtrl			m_gateway_addressCtrl;
	CEdit						m_listenPortEdit;
	CString					m_strDate;
	CString					m_strTime;
	unsigned int				m_nSerialNum;
	float						m_firmwareVersion;
	int							m_hardware_version;
	int							m_nbldVersion;

	CEdit						m_idEdt;
	int							m_IDaddress;

	CString					m_strIP;
	CString					m_strGatewayIP;
	CString					m_strSubnetMask;

	vector<CString>		m_szServers;
	void TestFunc(CString strIP);


	CMsflexgrid				m_gridSub;




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


	}subnetwork;
	typedef struct SubnetTstat
	{
	  CString ID;
	  CString SN;
	}SubnetDevice;

	vector<SubnetDevice>_subnetwork;
	SubnetDevice m_subetwork;
	afx_msg void OnBnClickedButton1();
	CString m_Mac_Address;
};


