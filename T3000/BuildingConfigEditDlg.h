#pragma once

#include "BuildingConfigration.h"
// CBuildingConfigEditDlg dialog

class CBuildingConfigEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBuildingConfigEditDlg)

public:
	CBuildingConfigEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBuildingConfigEditDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_BUILDING_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strBuildingName;
	CString m_strCity;
	CString m_strCountryName;
//	CEdit m_strElevation;
	CString m_strElevation;
	CString m_strLatitude;
	CString m_strlongitude;
	CString m_strstate;
	CString m_strstreet;
	CString m_strZip;
	Building_Config m_currentBuilding;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

	void BoundDataUpdate();
	CString m_strEngineeringUnits;
};
