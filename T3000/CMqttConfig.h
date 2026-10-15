#pragma once
#include "afxdialogex.h"

/*
 * Hub MQTT client configuration dialog.
 *
 * Primary transport : BACnet private transfer (READ_MQTT_CFG = 46 / WRITE_MQTT_CFG = 146)
 *                     using Str_Mqtt_point (400 bytes) held in Device_Mqtt_Point.
 * Fallback transport: Modbus holding registers 2301-2450 (see CMqttConfig.cpp for the layout).
 *
 * The read must be done by the caller BEFORE DoModal (see
 * CBacnetSettingTcpip::OnBnClickedButtonMqttConfig) so that no nested BACnet
 * request is issued while the dialog is being created.
 */
class CMqttConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CMqttConfig)

public:
	CMqttConfig(CWnd* pParent = nullptr);
	virtual ~CMqttConfig();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MQTT_CONFIG };
#endif

	/* Read Device_Mqtt_Point from the device: BACnet first, Modbus fallback. */
	static BOOL Load();
	static BOOL LoadViaBacnet();
	static BOOL LoadViaModbus();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckEnableMqtt();
	afx_msg void OnBnClickedButtonWriteMqttCfg();
	afx_msg void OnBnClickedButtonCancelMqttCfg();

private:
	static void SetDefaults(Str_Mqtt_point& pt);
	static BOOL IsBlank(const Str_Mqtt_point& pt);
	BOOL    WriteViaBacnet();
	BOOL    WriteViaModbus();
	void    LoadToUi();
	void    SaveFromUi();
	void    UpdateUIState();
	BOOL    ValidateInput();
};
