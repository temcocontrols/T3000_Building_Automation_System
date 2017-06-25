// FreeCoolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "FreeCoolDlg.h"
#include "global_variable_extern.h"
#include "global_function.h"
// CFreeCoolDlg 对话框

IMPLEMENT_DYNAMIC(CFreeCoolDlg, CDialog)
CFreeCoolDlg::CFreeCoolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFreeCoolDlg::IDD, pParent)
{
}

CFreeCoolDlg::~CFreeCoolDlg()
{
}

void CFreeCoolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFreeCoolDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
//	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck2)
	ON_EN_KILLFOCUS(IDC_TIMEEDIT, OnEnKillfocusTimeedit)
	ON_EN_KILLFOCUS(IDC_AIREDIT, OnEnKillfocusAiredit)
	ON_EN_KILLFOCUS(IDC_TRGEDIT, OnEnKillfocusTrgedit)
	ON_EN_KILLFOCUS(IDC_TMPEDIT, OnEnKillfocusTmpedit)
	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CFreeCoolDlg 消息处理程序
BOOL CFreeCoolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	

	GetDlgItem(IDC_TMPEDIT)->EnableWindow(FALSE);
	//GetDlgItem(IDC_AIREDIT)->EnableWindow(FALSE);

	((CButton*)GetDlgItem(IDC_CHECK4))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK4))->ShowWindow(SW_HIDE);

	
	((CButton*)GetDlgItem(IDC_TIMEEDIT))->ShowWindow(SW_HIDE);
	
	
	short nValue=-1;
	nValue =read_one(g_tstat_id,350);
	if(nValue<0)
		nValue =read_one(g_tstat_id,350);
	if(nValue>0)
	{
		if(nValue&0x1)
		{
			((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(BST_CHECKED);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(BST_UNCHECKED);
		}
		

		if(nValue&0x10)
		{
		//	((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(BST_CHECKED);
			((CButton*)GetDlgItem(IDC_STATUSSTATIC))->SetWindowText(_T("OK"));
		}
		else
		{
			//((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(BST_UNCHECKED);
				((CButton*)GetDlgItem(IDC_STATUSSTATIC))->SetWindowText(_T("FAIL"));
		}

		if(nValue&0x2)
		{
			//((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(BST_CHECKED);
			((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(BST_CHECKED);
			((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(BST_UNCHECKED);
		}
		else
		{
		//	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(BST_UNCHECKED);
			
			((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(BST_CHECKED);
			((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(BST_UNCHECKED);

			((CButton*)GetDlgItem(IDC_STATUSSTATIC))->SetWindowText(_T(""));
		}


		
	}
	nValue =read_one(g_tstat_id,360);
	CString strTemp;
	strTemp.Format(_T("%d"),nValue);
	GetDlgItem(IDC_TIMEEDIT)->SetWindowText(strTemp);
	nValue =read_one(g_tstat_id,369);
	strTemp.Format(_T("%d"),nValue);
	GetDlgItem(IDC_AIREDIT)->SetWindowText(strTemp);

	nValue =read_one(g_tstat_id,361);
	strTemp.Format(_T("%.1f"),nValue/10.0);
	GetDlgItem(IDC_TRGEDIT)->SetWindowText(strTemp);

	nValue =read_one(g_tstat_id,359);
	strTemp.Format(_T("%.1f"),nValue/10.0);
	GetDlgItem(IDC_TMPEDIT)->SetWindowText(strTemp);

	SetTimer(1,25000,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
// 异常: OCX 属性页应返回 FALSE

}
BOOL CFreeCoolDlg::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message == WM_KEYDOWN  )
	{		
		if(pMsg->wParam == VK_RETURN )
		{
			CButton * pbutton = (CButton *)GetDlgItem(IDCANCEL);
			pbutton->SetFocus();

			return TRUE;
		}

	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CFreeCoolDlg::OnBnClickedCancel()
{

	
	OnCancel();
}

void CFreeCoolDlg::OnBnClickedCheck1()
{
	
	CButton* pButton=(CButton*)GetDlgItem(IDC_CHECK1);
	short nValue=-1;
	nValue =read_one(g_tstat_id,350);
	if(nValue<0)
		return;
	if(pButton->GetCheck()==BST_CHECKED)
	{
		nValue=nValue|0x1;
	}
	else
	{
		nValue=nValue&0xFE;
	}
	nValue =write_one(g_tstat_id,350,nValue);
}

void CFreeCoolDlg::OnBnClickedCheck2()
{
	/*
	CButton* pButton=(CButton*)GetDlgItem(IDC_CHECK2);
	short nValue=-1;
	nValue =read_one(tstat_id,350);
	if(nValue<0)
		return;
	if(pButton->GetCheck()==BST_CHECKED)
	{
		nValue=nValue|0x2;
	}
	else
	{
		nValue=nValue&0xFD;
	}
	nValue =write_one(tstat_id,350,nValue);
	*/

}

void CFreeCoolDlg::OnEnKillfocusTimeedit()
{
	
	CString strTemp; 
	GetDlgItem(IDC_TIMEEDIT)->GetWindowText(strTemp);
	int nValue=_wtoi(strTemp);
	write_one(g_tstat_id,360,nValue);

}

void CFreeCoolDlg::OnEnKillfocusAiredit()
{
	CString strTemp; 
	GetDlgItem(IDC_AIREDIT)->GetWindowText(strTemp);
	int nValue=_wtoi(strTemp);
	write_one(g_tstat_id,369,nValue);
	
}

void CFreeCoolDlg::OnEnKillfocusTrgedit()
{
	CString strTemp; 
	GetDlgItem(IDC_TRGEDIT)->GetWindowText(strTemp);

	float nValue=(float)_wtof(strTemp);
	strTemp.Format(_T("%.1f"),nValue);
	write_one(g_tstat_id,361,short(nValue*10));
	
}

void CFreeCoolDlg::OnEnKillfocusTmpedit()
{
	
}

void CFreeCoolDlg::OnBnClickedRadio1()
{
	
	CButton* pNCRadio=((CButton*)GetDlgItem(IDC_RADIO1));
	CButton* pLocalRadio=((CButton*)GetDlgItem(IDC_RADIO2));

	pNCRadio->SetCheck(BST_CHECKED);
	pLocalRadio->SetCheck(BST_UNCHECKED);



	short nValue=-1;
	nValue =read_one(g_tstat_id,350);
	if(nValue<0)
		return;
	nValue=nValue|0x2;
	//	nValue=nValue&0xFD;
	nValue =write_one(g_tstat_id,350,nValue);


	if(nValue&0x10)
	{
		//((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(BST_CHECKED);
		((CButton*)GetDlgItem(IDC_STATUSSTATIC))->SetWindowText(_T("OK"));
	}
	else
	{
		//((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(BST_UNCHECKED);
		((CButton*)GetDlgItem(IDC_STATUSSTATIC))->SetWindowText(_T("FAIL"));
	}

}

void CFreeCoolDlg::OnBnClickedRadio2()
{
	
	CButton* pNCRadio=((CButton*)GetDlgItem(IDC_RADIO1));
	CButton* pLocalRadio=((CButton*)GetDlgItem(IDC_RADIO2));

	pLocalRadio->SetCheck(BST_CHECKED);
	pNCRadio->SetCheck(BST_UNCHECKED);

		
	short nValue=-1;
	nValue =read_one(g_tstat_id,350);
	if(nValue<0)
		return;
	//nValue=nValue|0x2;
	nValue=nValue&0xFD;
	nValue =write_one(g_tstat_id,350,nValue);
	((CButton*)GetDlgItem(IDC_STATUSSTATIC))->SetWindowText(_T(""));
}

void CFreeCoolDlg::OnTimer(UINT nIDEvent)
{
	
	if(nIDEvent==1)
	{
		Refresh();

	}

	CDialog::OnTimer(nIDEvent);
}
void CFreeCoolDlg::Refresh()
{
	short nValue=-1;
	nValue =read_one(g_tstat_id,350);
	if(nValue>0)
	{
		if(nValue&0x1)
		{
			((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(BST_CHECKED);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(BST_UNCHECKED);
		}
		

		if(nValue&0x10)
		{
			//((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(BST_CHECKED);
			((CButton*)GetDlgItem(IDC_STATUSSTATIC))->SetWindowText(_T("OK"));
		}
		else
		{
			//((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_STATUSSTATIC))->SetWindowText(_T("FAIL"));
		}

		if(nValue&0x2)
		{
			((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(BST_CHECKED);
			((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(BST_UNCHECKED);
		}
		else
		{
				
			((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(BST_CHECKED);
			((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(BST_UNCHECKED);

			((CButton*)GetDlgItem(IDC_STATUSSTATIC))->SetWindowText(_T(""));
		}

	
	}
	nValue =read_one(g_tstat_id,360);
	CString strTemp;
	strTemp.Format(_T("%d"),nValue);
	GetDlgItem(IDC_TIMEEDIT)->SetWindowText(strTemp);
	nValue =read_one(g_tstat_id,369);
	strTemp.Format(_T("%d"),nValue);
	GetDlgItem(IDC_AIREDIT)->SetWindowText(strTemp);

	nValue =read_one(g_tstat_id,361);
	strTemp.Format(_T("%.1f"),nValue/10.0);
	GetDlgItem(IDC_TRGEDIT)->SetWindowText(strTemp);

	nValue =read_one(g_tstat_id,359);
	strTemp.Format(_T("%.1f"),nValue/10.0);
	GetDlgItem(IDC_TMPEDIT)->SetWindowText(strTemp);


	}
/*
// CFreeCoolDlg dialog

IMPLEMENT_DYNAMIC(CFreeCoolDlg, CDialog)

CFreeCoolDlg::CFreeCoolDlg(CWnd* pParent )
	: CDialog(CFreeCoolDlg::IDD, pParent)
{

}

CFreeCoolDlg::~CFreeCoolDlg()
{
}

void CFreeCoolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFreeCoolDlg, CDialog)
END_MESSAGE_MAP()


// CFreeCoolDlg message handlers

*/
