#pragma once

#define MAX_NO_OF_GRIDS		5
// CMbPollOptions dialog

class CMbPollOptions : public CDialog
{
	DECLARE_DYNAMIC(CMbPollOptions)

public:
	CMbPollOptions(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMbPollOptions();

// Dialog Data
	enum { IDD = IDD_MBPOLL_OPTIONS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
//	CComboBox mNoOfGrids;
	virtual BOOL OnInitDialog();
//	int noOfGrids;
//	CString strNoOfGrids;
//	CString strEditTest;
	CComboBox comboFunction;
	CEdit editAddress;
	CEdit editQuantity;
	CEdit editScanRate;
	CEdit editSlaveId;
	CStatic staticAddress;
	CStatic staticFunction;
	CStatic staticQuantity;
	CStatic staticScanRate;
	CStatic staticSlaveId;
	CButton checkAddressInCell;
	CButton checkDisableOnError;
	CButton checkHideAliasColumn;
	CButton checkReadWriteDisabled;
	CComboBox comboDisplay;
	CButton radio10;
	CButton radio100;
	CButton radio20;
	CButton radio50;
	CButton radioFitToQty;
	virtual void OnOK();
	virtual void OnCancel();
	CButton checkTapData;
	afx_msg void OnCbnSelchangeComboFunction();
	CButton checkSingleFunction;
//	afx_msg void OnBnClickedOk();
//	afx_msg void OnBnClickedOk();
};

