#pragma once



// CT3Performance form view

class CT3Performance : public CFormView
{
	DECLARE_DYNCREATE(CT3Performance)

protected:
	CT3Performance();           // protected constructor used by dynamic creation
	virtual ~CT3Performance();

public:
	enum { IDD = IDD_T3PERFORMANCEe };
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


