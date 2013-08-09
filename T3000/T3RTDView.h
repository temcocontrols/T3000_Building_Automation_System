#pragma once



// CT3RTDView form view

class CT3RTDView : public CFormView
{
	DECLARE_DYNCREATE(CT3RTDView)

protected:
	CT3RTDView();           // protected constructor used by dynamic creation
	virtual ~CT3RTDView();

public:
	enum { IDD = IDD_T3RTD };
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


