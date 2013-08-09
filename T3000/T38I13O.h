#pragma once



// T38I13O form view

class T38I13O : public CFormView
{
	DECLARE_DYNCREATE(T38I13O)

protected:
	T38I13O();           // protected constructor used by dynamic creation
	virtual ~T38I13O();

public:
	enum { IDD = IDD_T38I13O };
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


