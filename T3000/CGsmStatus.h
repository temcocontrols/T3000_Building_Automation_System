#pragma once
#include "afxdialogex.h"

class CGsmStatus : public CDialogEx
{
	DECLARE_DYNAMIC(CGsmStatus)

public:
	CGsmStatus(CWnd* pParent = nullptr);
	virtual ~CGsmStatus();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GSM_STATUS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonGsmRefresh();
	afx_msg void OnBnClickedButtonGsmClose();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

private:
	BOOL RefreshFromDevice();
	BOOL LoadViaBacnet();
	BOOL LoadViaModbus();
	void ApplyToUi();
	void UpdateSimButtons();
	void DrawSimButton(LPDRAWITEMSTRUCT lpDrawItemStruct, BOOL present);
	static CString ModemStateName(unsigned char state);
	static CString PppStateName(unsigned char state);
	static CString FlagsText(unsigned char flags);

	BOOL m_sim1_present;
	BOOL m_sim2_present;
};
