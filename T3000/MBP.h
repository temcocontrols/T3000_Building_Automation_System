#include "msflexgrid1.h"
#include "afxwin.h"
#pragma once


#define WM_DLG_CLOSE           WM_USER + 200

#if 0
class CMbp : public CDialog
{
	DECLARE_DYNAMIC(CMbp)

public:
	CMbp(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMbp();

	enum { IDD = IDD_MBP };

	CMsflexgrid m_MBP_FlexGrid;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//DECLARE_MESSAGE_MAP()
	//void ClickMsflexgrid1();
	//DECLARE_EVENTSINK_MAP()
	
public:	
	//virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	DECLARE_EVENTSINK_MAP()
	void ClickMbp1();
};
#endif

class CMbp : public CDialog
{
private:
	CMenu *m_menu;
	void OnUpdateDisplaySigned(CCmdUI* pCmdUI);
	void OnCommandDisplaySigned();

public:
	CMbp(CWnd* pParent = NULL);
	BOOL Create();
	BOOL OnInitDialog();
	void ForceResize();
	//void OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu);
	CWnd* m_pMainWindow;

	CEdit mMbpEditCtrl;

// Dialog Data
    //{{AFX_DATA(C_Modeless)
    enum { IDD = IDD_MBP };
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(C_Modeless)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(C_Modeless)
    afx_msg void OnClose();
    virtual void OnOK();
    afx_msg void OnDestroy();
    //afx_msg void OnKillfocusTitle();
/*    afx_msg void OnGreen();
    afx_msg void OnRed();
    afx_msg void OnYellow();*/
    //}}AFX_MSG
/*    LONG OnMainRed(UINT wParam, LONG lParam);
    LONG OnMainGreen(UINT wParam, LONG lParam);
    LONG OnMainYellow(UINT wParam, LONG lParam);*/
    DECLARE_MESSAGE_MAP()
};



