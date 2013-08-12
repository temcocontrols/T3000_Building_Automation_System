#pragma once



// T36CT form view

class T36CT : public CFormView
{
	DECLARE_DYNCREATE(T36CT)

protected:
	T36CT();           // protected constructor used by dynamic creation
	virtual ~T36CT();

public:
	enum { IDD = IDD_T36CT };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};


