// RefreshTreeThread.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "RefreshTreeThread.h"
#include "MainFrm.h"


// CRefreshTreeThread

HANDLE  g_hRefreshEvent;

IMPLEMENT_DYNCREATE(CRefreshTreeThread, CWinThread)




CRefreshTreeThread::CRefreshTreeThread()
{

}

CRefreshTreeThread::~CRefreshTreeThread()
{
}

BOOL CRefreshTreeThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here

   return TRUE;
}

int CRefreshTreeThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}



BEGIN_MESSAGE_MAP(CRefreshTreeThread, CWinThread)
	ON_THREAD_MESSAGE(WM_DOREFRESH, OnRefreshTree)
END_MESSAGE_MAP()



void CRefreshTreeThread::OnRefreshTree(WPARAM wParam,LPARAM lParam)
{
		m_pMyMainWnd->DoFreshAll();
}



BOOL CRefreshTreeThread::OnIdle(LONG lCount)
{
	// TODO: Add your specialized code here and/or call the base class
// #ifdef _DEBUG
// 	TRACE("IDEL COUNT = %d \n", lCount);
// #endif
	return CWinThread::OnIdle(lCount);
}


void CRefreshTreeThread::SetMainWnd(CMainFrame* pMain)
{
	m_pMyMainWnd = pMain;
}
int CRefreshTreeThread::Run()
{
	// TODO: Add your specialized code here and/or call the base class	
	return CWinThread::Run();
}
