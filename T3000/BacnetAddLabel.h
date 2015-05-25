#pragma once
#include "afxwin.h"

#include "CM5/CStatic/staticex.h"
// CBacnetAddLabel dialog

class CBacnetAddLabel : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetAddLabel)

public:
	CBacnetAddLabel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetAddLabel();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_ADD_LABEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStaticEx m_add_label_point;
	CStaticEx m_add_label_edit;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void FreshWindow();
};
