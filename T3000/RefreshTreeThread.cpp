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
	//   perform and per-thread initialization here

   return TRUE;
}

int CRefreshTreeThread::ExitInstance()
{
	//   perform any per-thread cleanup here
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
		
	return CWinThread::Run();
}
