#pragma once

#include "afxwin.h"
#include "msflexgrid1.h"

// CMannageBuidingDlg 对话框
typedef struct _MAINBUILDINGTAG
{
	CString strMainName;
	CString	strTelePhone;
	CString strAddress;
	int bActive;
}MAINBUILDING;

class CMannageBuidingDlg : public CDialog
{
	DECLARE_DYNAMIC(CMannageBuidingDlg)

public:
	CMannageBuidingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMannageBuidingDlg();

// 对话框数据
	enum { IDD = IDD_BUILDING_MANAGE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDelbutton();
	afx_msg void OnBnClickedAddbutton();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	void reLoadMainBuilding();
	CButton m_DelBtn;
	CButton m_AddBtn;
	CEdit m_NameEdt;
	int m_nTotalItem;

	_ConnectionPtr m_pCon;
	_RecordsetPtr m_pRs;
	CListBox m_BuildingNameLst;
	BOOL m_bIsAddOrModify;//true add false modify;


	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedOk();
//	afx_msg void OnLbnSetfocusBuildinglist();
	CButton m_defaultBuildignCheck;
	//afx_msg void OnLbnSelchangeBuildinglist();
	afx_msg void OnBnClickedDefaultbuildingCheck();
	CMsflexgrid m_FlexGrid;
	vector<MAINBUILDING> m_mainBuildingLst;
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgrid1();


	CString m_strDefBuildingName;
	CString m_strMainBuiding;
	CString m_strTelPhone;
	CString m_strAddress;

	int m_nCurRow;
	int m_nCurCol;
	CEdit m_textEdit;
	afx_msg void OnEnKillfocusEdit1();
	void Update_Recorder();
	void OnMainBuildingSelect();
	void OnMainBuildingUnSelect();
};
