#pragma once



// CT328IN form view

class CT328IN : public CFormView
{
	DECLARE_DYNCREATE(CT328IN)

protected:
	CT328IN();           // protected constructor used by dynamic creation
	virtual ~CT328IN();

public:
	enum { IDD = IDD_T328IN };
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


