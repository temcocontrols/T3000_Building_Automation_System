#pragma once


// CBacnetAddVirtualDevice dialog

class CBacnetAddVirtualDevice : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetAddVirtualDevice)

public:
	CBacnetAddVirtualDevice(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetAddVirtualDevice();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_ADD_VIRTUAL_DEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void UI_Initial();
	afx_msg void OnBnClickedButtonVirtualOk();
	afx_msg void OnBnClickedButtonVirtualCancel();
};
