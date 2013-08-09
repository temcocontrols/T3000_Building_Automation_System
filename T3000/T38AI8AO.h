#pragma once



// T38AI8AO form view

class T38AI8AO : public CFormView
{
	DECLARE_DYNCREATE(T38AI8AO)

protected:
	T38AI8AO();           // protected constructor used by dynamic creation
	virtual ~T38AI8AO();

public:
	enum { IDD = IDD_T38AI8AO };
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


