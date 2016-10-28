#pragma once
#include "msflexgrid1.h"
#include "afxwin.h"

// CUserAcessSetDlg dialog
typedef struct USER_LEVEL
{
	CString strMainBuildingName;
	CString strSubBuildingName;
	unsigned int nSerialNumber;
	int ProductID;//the id of product ;Modbus protocol need
	int product_class_id;//7 product model
	int baudrate;
	DWORD MainLevel;
	DWORD ParamerLevel;
	DWORD Outputtable_level;
	DWORD NC_level;
	DWORD Graphic_level;
    DWORD Burnhex_level;
	DWORD Loadconfig_level;
	DWORD Buildingset_level;
	DWORD Allscreen_level;
	CString strUserName;
	CString strFloorName;
	CString strRoomName;
}USERLEVEL;

typedef struct VERY_PRODUCT
{
	unsigned int nSerialNumber;
	int ProductID;//the id of product ;Modbus protocol need
	int product_class_id;//7 product model
	CString strFloorName;
	CString strBuildingName;
	CString strRoomName;
	CString strPhone;
	CString strMainBuildingName;
}VERYPRODCT;

typedef struct USER_SINGLE_LEVEL
{
	CString strUserName;
	DWORD network_controllerL;
	DWORD burnhexL;
	DWORD loadconfigL;
	DWORD databaseL;
	DWORD routinesL;
}USER_SINGLE_LEVEL;

class CUserAcessSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserAcessSetDlg)

public:
	CUserAcessSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserAcessSetDlg();

// Dialog Data
	enum { IDD = IDD_USERPRIVILEGDLG };

protected:
	afx_msg void OnCbnSelchangeCombo1();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CMsflexgrid m_FlexGrid;
	CMsflexgrid m_FlexGrid2;
	virtual BOOL OnInitDialog();
	CComboBox m_subNetList;
	void SetConfigUserName(CString strUserName);
	void InserProductToUserSetDB();
public:
	CString m_strUserName;
 
	vector<USERLEVEL> m_UserLevelLst;
	vector<VERYPRODCT> m_VeryProdctLst;
	CString m_strMainBuilding;
	CString m_strSubNetName;
	int m_nCurRow;
	int m_nCurCol;

	int m_nCurRow2;
	int m_nCurCol2;

	void ReloadUserLevelDB();
	CComboBox m_userLeveSetBox;
	DECLARE_EVENTSINK_MAP()
	void ClickAddbuildingMsflexgrid();
	afx_msg void OnCbnSelchangeLevelsetcombo();
	afx_msg void OnCbnKillfocusLevelsetcombo();
	//CComboBox m_ncPrivilegeCombx;
	afx_msg void OnBnClickedCancel();

	void ClickAddbuildingMsflexgrid2();
	void InserSingleSetConfig();
	void ReloadSingleLevelSetDB();
	CComboBox m_singleLevelSetCombox;
	afx_msg void OnCbnSelchangeSinglesetcombo();
	afx_msg void OnCbnKillfocusSinglesetcombo();
};
