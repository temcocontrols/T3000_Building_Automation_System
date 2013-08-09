#pragma once



// T38AI16O form view

class T38AI16O : public CFormView
{
	DECLARE_DYNCREATE(T38AI16O)

protected:
	T38AI16O();           // protected constructor used by dynamic creation
	virtual ~T38AI16O();

public:
	enum { IDD = IDD_T38AI16O };
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


