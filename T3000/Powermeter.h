#pragma once


// CPowermeter �Ի��� - CPowermeter Dialog Box

class CPowermeter : public CFormView
{
	DECLARE_DYNAMIC(CPowermeter)

public:
	CPowermeter(CWnd* pParent = NULL);   // ��׼���캯�� - Standard constructor
	virtual ~CPowermeter();


    enum { IDD = IDD_DIALOG_POWER_METER};
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧�� - DDX/DDV support
    virtual void OnInitialUpdate();
	DECLARE_MESSAGE_MAP()

public:
    void Fresh();
};
