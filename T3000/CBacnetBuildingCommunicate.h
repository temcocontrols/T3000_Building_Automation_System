#pragma once
#include "global_function.h"

// CBacnetBuildingCommunicate 对话框

class CBacnetBuildingCommunicate : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetBuildingCommunicate)

public:
	CBacnetBuildingCommunicate(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBacnetBuildingCommunicate();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_BUILDING_COMMUNICATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void Initialize();
	vector<CString> m_szComm;
};
