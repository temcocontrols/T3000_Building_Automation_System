#pragma once



// CBacnetThirdPartyMain Form view
// CBacnetThirdPartyMain ������ͼ

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
	// DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    // Protected constructor used by dynamic creation
    CBacnetThirdPartyMain();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
    virtual ~CBacnetThirdPartyMain();
    void Fresh();
    void Initial_combo();
    afx_msg void OnBnClickedButtonThirdOk();
    int device_id;
    int object_identifier; 
    int object_instance;
    int property_identifier;
};


