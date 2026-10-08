#pragma once
#include "afxdialogex.h"

/*
 * Hub SNMP agent configuration dialog.
 *
 * Primary transport : BACnet private transfer (READ_SNMP_CFG = 47 / WRITE_SNMP_CFG = 147)
 *                     using Str_Snmp_point (400 bytes) held in Device_Snmp_Point.
 * Fallback transport: Modbus holding registers 2451-2550 (see CSnmpConfig.cpp for the layout).
 *
 * The read must be done by the caller BEFORE DoModal (see
 * CBacnetSettingTcpip::OnBnClickedButtonSnmpConfig) so that no nested BACnet
 * request is issued while the dialog is being created.
 */
class CSnmpConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CSnmpConfig)

public:
	CSnmpConfig(CWnd* pParent = nullptr);
	virtual ~CSnmpConfig();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SNMP_CONFIG };
#endif

	/* Read Device_Snmp_Point from the device: BACnet first, Modbus fallback. */
	static BOOL Load();
	static BOOL LoadViaBacnet();
	static BOOL LoadViaModbus();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckEnableSnmp();
	afx_msg void OnBnClickedButtonWriteSnmpCfg();
	afx_msg void OnBnClickedButtonCancelSnmpCfg();

private:
	static void SetDefaults(Str_Snmp_point& pt);
	static BOOL IsBlank(const Str_Snmp_point& pt);
	BOOL    WriteViaBacnet();
	BOOL    WriteViaModbus();
	void    LoadToUi();
	void    SaveFromUi();
	void    UpdateUIState();
	BOOL    ValidateInput();
};
