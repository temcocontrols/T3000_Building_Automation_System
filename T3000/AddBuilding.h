#pragma once
//#include "afxwin.h"
#include "msflexgrid1.h"
#include "afxwin.h"

// CAddBuilding dialog
struct _buildName
{
	CString strMainBuildName;
	CString strSubBuildName;
};

class CAddBuilding : public CDialog
{
	DECLARE_DYNAMIC(CAddBuilding)

public:
	CAddBuilding(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddBuilding();

// Dialog Data
	enum { IDD = IDD_ADDBUILDING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddbuiding();
	virtual BOOL OnInitDialog();
private:
 
	int m_nCurRow;
	int m_nCurCol;

	CString m_strMainBuildingName;
	CString m_strBuildingName;
	CString m_strProtocol;
	CString	m_strIpAddress;
	CString	m_strComPort;
	CString m_strBaudrat;

	CString	m_strIpPort;
	CString	m_strBuilding_Name2;
	CString m_strMainBuildingName2;//hold old name.
	vector<CString> m_buildNameLst;//<main,sub>
	vector<_buildName> m_BuildNameLst;
public:
	int m_nCurSelectRow;
	CString m_strCurSeBuilding;
	CWinThread* pNCScanThread;
	CMsflexgrid m_NCFlexGrid;
	SYSTEMTIME time1;
	SYSTEMTIME time2;
	BOOL m_bRunningThread;
	
public:
	CComboBox m_AddBuiding_SetComBox;
	CEdit m_AddBuiding_SetEditCtrl;
	CMsflexgrid m_AddBuiding_FlexGrid;
	DECLARE_EVENTSINK_MAP()
	void ClickAddbuildingMsflexgrid();
	void EnterCellAddbuildingMsflexgrid();
	void SelChangeAddbuildingMsflexgrid();
	void ReloadAddBuildingDB();
	void Update_Recorder();
	afx_msg void OnEnKillfocusAddbuidingSetedit();
	afx_msg void OnCbnSelchangeAddbuidingSelectcombo();
	afx_msg void OnCbnKillfocusAddbuidingSelectcombo();

	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnSetfocusAddbuidingSetedit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL ValidAddress(CString sAddress,UINT& n1,UINT& n2,UINT& n3,UINT& n4);
	//BOOL ValidAddress(CString sAddress);

public:
	void SetBuildingMainName(CString strBuildName);
	void OnMainBuildingSelect();
	void OnMainBuildingUnSelect();
	void OnBuildingDelete();
	BOOL GetIPbyHostName(CString strHostName,CString &strIP);
protected:
	virtual void PostNcDestroy();
	BOOL IsValidCOM(const CString& strComPort);
public:
	afx_msg void OnStnClickedStatic1();
	afx_msg void OnCbnSelendokAddbuidingSelectcombo();
	CEdit m_mainBuildEdt;
	afx_msg void OnEnKillfocusMainbuildedit();
	afx_msg void OnEnSetfocusMainbuildedit();
	void DblClickAddbuildingMsflexgrid();
	afx_msg void OnBnClickedScanncbtn();
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButton m_NCScanBtn;
	afx_msg void OnBnClickedSavebutton();
	CButton m_SaveBtn;

	vector<CString> m_szComm;


	BOOL m_Changed;
	//////////////////////////////////////////////////////////////////////////
	//	Added by zgq; 2010-11-29;
	//  把按钮Exit和Save功能合并
	//
	void CAddBuilding::SaveAll();

	///////////////////////////////////////////////////////////////////////////
	// added by zgq; 2010-12-09; 获得计算机上的串口号
	//BOOL GetSerialComm(vector<CString>& szComm);


};
