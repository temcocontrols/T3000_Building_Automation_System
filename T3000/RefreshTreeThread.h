#pragma once

class CMainFrame;

// CRefreshTreeThread
#define WM_DOREFRESH    WM_USER + 1000


class CRefreshTreeThread : public CWinThread
{
	DECLARE_DYNCREATE(CRefreshTreeThread)

protected:
	CRefreshTreeThread();           // protected constructor used by dynamic creation
	virtual ~CRefreshTreeThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	
	void SetMainWnd(CMainFrame* pMain);

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnIdle(LONG lCount);
	void OnRefreshTree(WPARAM wParam,LPARAM lParam);

private:
	CMainFrame* m_pMyMainWnd;

public:
	virtual int Run();
};


