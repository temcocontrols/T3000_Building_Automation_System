
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "FileView.h"
class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	void Read_Config();

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CFileView         m_wndFileView;
	 
	 

	int m_communication_type;
	int m_comport;
	int m_bradrate;
	HANDLE m_MultiRead_handle;
	CString m_ipaddress;
	int m_port;
	int m_connecttimeout;
// Generated message map functions
	BOOL m_isconnect;
    int  m_responsetimeout ;
   int   m_between_time ;
    // int m_response_timeout;

   /* int m_bradrate;
    int m_Commication_Type;
    CString m_comport;
    CString m_ipaddress;
    int m_between_time;
    int m_connect_timeout;
    int m_ipport;
    int m_response_timeout;*/

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnConnectionConnect32776();
	afx_msg void OnDestroy();
	afx_msg void OnConnectionQuickconnectf5();
	afx_msg void OnConnectionDisconnect();
	afx_msg void OnUpdateConnectionQuickconnectf5(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConnectionDisconnect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConnectionConnect32776(CCmdUI *pCmdUI);
    void  	Show_Data_Traffic_Window();
//	afx_msg void OnSetupRead32783();
	afx_msg void OnDispalyCommunication();
	afx_msg void OnUpdateDispalyCommunication(CCmdUI *pCmdUI);
     afx_msg void OnUpdateStatusBar(CCmdUI *pCmdUI);
	afx_msg void OnViewRegistervalueanalyzer();
	afx_msg void OnFunctionsTestcenter();
	 
	 
	afx_msg void OnToolsDevicetester();
	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
};


