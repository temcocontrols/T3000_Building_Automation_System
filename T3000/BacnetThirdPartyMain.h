#pragma once



// CBacnetThirdPartyMain 窗体视图

class CBacnetThirdPartyMain : public CFormView
{
	DECLARE_DYNCREATE(CBacnetThirdPartyMain)

protected:
	
	

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_THIRD_PARTY_MAIN };
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
    CBacnetThirdPartyMain();           // 动态创建所使用的受保护的构造函数
    virtual ~CBacnetThirdPartyMain();
    void Fresh();
    void Initial_combo();
    afx_msg void OnBnClickedButtonThirdOk();
    int device_id;
    int object_identifier; 
    int object_instance;
    int property_identifier;
};


