// Table_Tamplete.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "Table_Tamplete.h"
#include "afxdialogex.h"


// CTable_Tamplete dialog

IMPLEMENT_DYNAMIC(CTable_Tamplete, CDialog)

CTable_Tamplete::CTable_Tamplete(CWnd* pParent /*=NULL*/)
	: CDialog(CTable_Tamplete::IDD, pParent)
{

}

CTable_Tamplete::~CTable_Tamplete()
{
}

void CTable_Tamplete::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MSFLEXGRID1, m_msgrid);
}


BEGIN_MESSAGE_MAP(CTable_Tamplete, CDialog)
END_MESSAGE_MAP()


// CTable_Tamplete message handlers
