﻿#pragma once

#include "CM5/ListCtrlEx.h"
// CBacnetBuildingProperty dialog
// CBacnetBuildingProperty 对话框

class CBacnetBuildingProperty : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetBuildingProperty)

public:
	// Standard constructor
	CBacnetBuildingProperty(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBacnetBuildingProperty();

// Dialog data
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_BUILDING_PROPERTY };
#endif

protected:
	// DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDbPropertyOk();
	ListCtrlEx::CListCtrlEx m_property_list;
	void Initial_List();
};
