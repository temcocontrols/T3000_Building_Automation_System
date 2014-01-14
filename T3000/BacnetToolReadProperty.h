#pragma once
#include "afxwin.h"


// CBacnetToolReadProperty dialog

class CBacnetToolReadProperty : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetToolReadProperty)

public:
	CBacnetToolReadProperty(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetToolReadProperty();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_TOOL_READ_PRP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnBacSend();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	void Initial_combo();
	CComboBox m_combo_service_choice;
	CComboBox m_combo_object_identifier;
	CComboBox m_combo_property_identifier;
	afx_msg void OnBnClickedBtnBacCancel();
	CEdit m_instance_edit;
};
