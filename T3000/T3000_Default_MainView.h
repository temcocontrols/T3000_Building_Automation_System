#pragma once



// T3000_Default_MainView form view

class T3000_Default_MainView : public CFormView
{
	DECLARE_DYNCREATE(T3000_Default_MainView)

public:
	T3000_Default_MainView();           // protected constructor used by dynamic creation
	virtual ~T3000_Default_MainView();

public:
	enum { IDD = IDD_DIALOG_DEFAULT_MAIN_VIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void Fresh();
	afx_msg void OnBnClickedEditId();
};


