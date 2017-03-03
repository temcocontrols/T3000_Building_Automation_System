#pragma once


// CWriteSingleCoilDlg dialog

class CWriteSingleCoilDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWriteSingleCoilDlg)

public:
	CWriteSingleCoilDlg(UINT Slave_ID = 255, UINT Address = 0, BOOL Close_Dlg = FALSE,BOOL Function=TRUE,BOOL PLC_ADDRESS=FALSE, BOOL Value=FALSE,CWnd* pParent = NULL);   // standard constructor
	virtual ~CWriteSingleCoilDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WRITE_SINGLE_COIL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	UINT m_slave_id;
	UINT m_address;
	BOOL m_close_dlg;
	BOOL m_function15;
	BOOL m_Value;
	BOOL m_PLC_Address;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void FreshUI();
	afx_msg void OnBnClickedRadioOn();
	afx_msg void OnBnClickedRadioOff();
	afx_msg void OnBnClickedCheckCloseDlg();
	afx_msg void OnBnClickedRadioSinglecoilReg();
	afx_msg void OnBnClickedRadioMultiplecoilReg();
};
