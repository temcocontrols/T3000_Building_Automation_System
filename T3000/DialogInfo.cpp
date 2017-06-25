// DialogInfo.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "DialogInfo.h"


// CDialogInfo dialog

IMPLEMENT_DYNAMIC(CDialogInfo, CDialog)

CDialogInfo::CDialogInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogInfo::IDD, pParent)
{

}

CDialogInfo::~CDialogInfo()
{
}

void CDialogInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogInfo, CDialog)
END_MESSAGE_MAP()


BOOL CDialogInfo::PreTranslateMessage(MSG* pMsg)
{
    
    if (IsWindowVisible())
    {
        if (pMsg->message == WM_LBUTTONDOWN||pMsg->message == WM_RBUTTONDOWN)
        {
            ShowWindow(SW_HIDE);
        }
    } 
    return CDialog::PreTranslateMessage(pMsg);
}
