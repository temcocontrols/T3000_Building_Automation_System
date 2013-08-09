#pragma once



// T332AI form view

class T332AI : public CFormView
{
	DECLARE_DYNCREATE(T332AI)

protected:
	T332AI();           // protected constructor used by dynamic creation
	virtual ~T332AI();

public:
	enum { IDD = IDD_T332AI };
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


