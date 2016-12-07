#pragma once
#include "msflexgrid1.h"
#include "afxwin.h"

// CZigbeeInformationDlg dialog

class CZigbeeInformationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CZigbeeInformationDlg)

public:
	CZigbeeInformationDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CZigbeeInformationDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ZIGBEE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CMsflexgrid m_zigbee_tstat_table;
	CComboBox m_combox_zigbee;
	CComboBox m_combox_channel;
	afx_msg void OnBnClickedBtnTopological();
	afx_msg void OnBnClickedButtonZigbeeReboot();
};
