#pragma once


// CPowermeter 对话框

class CPowermeter : public CFormView
{
	DECLARE_DYNAMIC(CPowermeter)

public:
	CPowermeter(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPowermeter();


    enum { IDD = IDD_DIALOG_POWER_METER};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual void OnInitialUpdate();
	DECLARE_MESSAGE_MAP()

public:
    void Fresh();
};
