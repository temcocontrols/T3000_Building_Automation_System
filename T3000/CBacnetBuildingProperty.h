#pragma once

#include "CM5/ListCtrlEx.h"
// CBacnetBuildingProperty 

class CBacnetBuildingProperty : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetBuildingProperty)

public:
	CBacnetBuildingProperty(CWnd* pParent = nullptr);   // 
	virtual ~CBacnetBuildingProperty();

// 
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_BUILDING_PROPERTY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDbPropertyOk();
	ListCtrlEx::CListCtrlEx m_property_list;
	void Initial_List();
};
