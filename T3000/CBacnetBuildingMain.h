#pragma once
#include "CBacnetBuildingManagement.h"
#include "CBacnetBuildingIOPoints.h"

// CBacnetBuildingMain 窗体视图
const int BUILDING_MAX_UI = 3;
class CBacnetBuildingMain : public CFormView
{
	DECLARE_DYNCREATE(CBacnetBuildingMain)

public:
	CBacnetBuildingMain();           // 动态创建所使用的受保护的构造函数
	virtual ~CBacnetBuildingMain();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_BUILDING_MAIN };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	void Fresh();
	void InitialTab();
	CTabCtrl m_main_building_tab;
	CBacnetBuildingManagement m_page_building_info;
	CBacnetBuildingIOPoints m_page_building_io;
	CDialog* building_tab_dialog[BUILDING_MAX_UI];
	int m_CurSelTab;
	void InitScrollbar();
	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTabBuildingMain(NMHDR* pNMHDR, LRESULT* pResult);
	void OnHTreeItemClick(NMHDR* pNMHDR, LRESULT* pResult);
	void InitBuildingFloor();
};

// CBacnetBuildingMain 消息处理程序
const int BUILDING_IO = 0;
const int BUILDING_BASIC = 1;

const int BUILDING_COMMUNICATION = 2;


