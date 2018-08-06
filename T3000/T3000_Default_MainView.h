#pragma once
#include "afxwin.h"



// T3000_Default_MainView form view

class T3000_Default_MainView : public CFormView
{
	DECLARE_DYNCREATE(T3000_Default_MainView)

public:
	T3000_Default_MainView();           // protected constructor used by dynamic creation
	virtual ~T3000_Default_MainView();

public:
	enum { IDD = IDD_DIALOG_DEFAULT_MAIN_VIEW };
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
	virtual void OnInitialUpdate();
	void Fresh();
	afx_msg void OnBnClickedEditId();
//    afx_msg void OnBnClickedButtonGoToInput();
    CComboBox m_ip_model;
    CIPAddressCtrl m_ipaddress;
    CIPAddressCtrl m_Subnet_Mask;
    CIPAddressCtrl m_GatewayAddress;
    CEdit m_Edit_MacAddress;
    CEdit m_ListenPort;
    afx_msg void OnBnClickedApplybutton();
    afx_msg void OnCbnSelchangeIpmodelCombo();
	CComboBox m_brandrate_combox;
	CEdit m_Editor_macmstp;
	CComboBox m_mstpmodbus_combox;
	CEdit m_BipPort_Edit;
	afx_msg void OnEnKillfocusEditBipport();
	afx_msg void OnCbnSelchangeComboMstpModbus();
	afx_msg void OnEnKillfocusEditMstpMac();
	afx_msg void OnCbnKillfocusBaudratecombo();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonWifiConfig();
	CEdit m_Edit_AESPassword;
	afx_msg void OnBnClickedCheckPassword();
	afx_msg void OnCbnSelchangeComboMrd();
	CComboBox m_combox_mrd;
	bool m_checkpassword;
	afx_msg void OnEnKillfocusEditAesEncrypt();
	afx_msg void OnEnKillfocusEditNetworkid();
	afx_msg void OnEnKillfocusEditNodeid();
	afx_msg void OnEnKillfocusEditFreid();
	CEdit m_Edit_Network;
	CEdit m_Edit_NodeID;
	CEdit m_Edit_FrequenceID;
    afx_msg void OnEnChangeEditAesEncrypt();
};


