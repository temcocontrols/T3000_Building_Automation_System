#pragma once

#include "CM5/ListCtrlEx.h"
//#include "MainFrm.h"
#include "CBacnetBMD.h"
// CBacnetBuildingManagement 窗体视图

class CBacnetBuildingManagement : public CDialogEx
{
	DECLARE_DYNCREATE(CBacnetBuildingManagement)

public:
	CBacnetBuildingManagement();           // 动态创建所使用的受保护的构造函数
	virtual ~CBacnetBuildingManagement();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_BUILDING_MANAGEMENT };
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
	ListCtrlEx::CListCtrlEx m_root_list;
	void Initial_List();
	afx_msg void OnBnClickedButtonBmAdd();
	afx_msg void OnBnClickedButtonBmDelete();
	afx_msg void OnBnClickedButtonBmDone();
	void LoadFileShowToList();
	void ShowListToTree();
	void TreeInital();
	//CMainFrame* pFrame;

	int nGroupCount ; // 有多少个Group 
	CString groupname[255];
	int n_group_kids_count[255];
	void SaveAllIntoIniFile();
	CBacnetBMD BuildingNode;
	afx_msg void OnBnClickedButtonBmSave();
	void UpdateList();
	void SetDataTreeCtrl();
	//CImageTreeCtrl* tree_ctrl;
	afx_msg void OnBnClickedButtonDbDone();
};

const int BM_ROOT_ITEM = 0;
const int BM_ROOT_GROUP_NAME = 1;
const int BM_ROOT_TOTAL_INPUT = 2;
const int BM_ROOT_TOTAL_OUTPUT = 3;
const int BM_ROOT_TOTAL_VARIABLE = 4;


