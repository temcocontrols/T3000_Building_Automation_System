#pragma once
#include "global_function.h"

// CBacnetBuildingCommunicate 

class CBacnetBuildingCommunicate : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetBuildingCommunicate)

public:
	CBacnetBuildingCommunicate(CWnd* pParent = nullptr);   // 
	virtual ~CBacnetBuildingCommunicate();

// 
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_BUILDING_COMMUNICATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void Initialize();
	vector<CString> m_szComm;
};
