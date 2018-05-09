// ScanSelectDlg.cpp : implementation file

#include "stdafx.h"
#include "T3000.h"
#include "ScanSelectDlg.h"

#include "global_variable_extern.h"
#include "MainFrm.h"

// CScanSelectDlg dialog

#if 1
IMPLEMENT_DYNAMIC(CScanSelectDlg, CDialog)

CScanSelectDlg::CScanSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScanSelectDlg::IDD, pParent)
	, m_FromID(1)
	, m_nDestID(254)
{
	m_strScanSubNetName=_T("");


}

CScanSelectDlg::~CScanSelectDlg()
{
}

void CScanSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_strScanSubNetNameCobox);
}


BEGIN_MESSAGE_MAP(CScanSelectDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CScanSelectDlg::OnBnClickedOk)

	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_EN_KILLFOCUS(IDC_MINEDIT, OnEnKillfocusMinedit)
	ON_EN_KILLFOCUS(IDC_MAXEDIT, OnEnKillfocusMaxedit)
END_MESSAGE_MAP()





// CScanDlg 消息处理程序
void CScanSelectDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	g_ScanSecurity=FALSE;
	g_Scanfully=TRUE;
	((CButton*) GetDlgItem(IDC_RADIO2))->SetCheck(BST_UNCHECKED);
	((CButton*) GetDlgItem(IDC_RADIO1))->SetCheck(BST_CHECKED);

	ShowSafeScanControl(g_ScanSecurity);
}
void CScanSelectDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	g_Scanfully=FALSE;
	g_ScanSecurity=TRUE;
	((CButton*) GetDlgItem(IDC_RADIO2))->SetCheck(BST_CHECKED);
	((CButton*) GetDlgItem(IDC_RADIO1))->SetCheck(BST_UNCHECKED);

	ShowSafeScanControl(g_ScanSecurity);
}
void CScanSelectDlg::OnBnClickedOk()
{
	//TODO: 在此添加控件通知处理程序代码
	//m_FromID
	//GetDlgItem(IDC_MINEDIT)->SetWindowText(strText);
	//strText.Format("%d",m_nDestID);
	//GetDlgItem(IDC_MAXEDIT)->SetWindowText(strText);
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);

	int nCurSel=m_strScanSubNetNameCobox.GetCurSel();
	if(nCurSel<0)
		return;
		
	//m_strScanSubNetName=pFrame->m_subNetLst.at(nCurSel).strBuildingName;
	if(pFrame->m_strCurSubBuldingName.IsEmpty())
	{
		AfxMessageBox(_T("There is no default building,please select a building First！"));
		return;
	}

	m_strScanSubNetName=pFrame->m_strCurSubBuldingName;
		
	if(g_ScanSecurity)
	{
		CString strText;
		GetDlgItem(IDC_MINEDIT)->GetWindowText(strText);
		int nID=-1;
		if(strText.IsEmpty())
		{
			AfxMessageBox(_T("Please input start ID"));
			return;
		}
		nID=_wtoi(strText);
	
		if(nID<=0||nID>=255)
		{
			AfxMessageBox(_T("Erro  start ID, Please check!"));
			return;
		}
		m_FromID=nID;


		GetDlgItem(IDC_MAXEDIT)->GetWindowText(strText);
		nID=-1;
		if(strText.IsEmpty())
		{
			AfxMessageBox(_T("Please input Max  ID!"));
			return;
		}
		nID=_wtoi(strText);
	
		if(nID<=0||nID>=255)
		{
			AfxMessageBox(_T("Erro  Max ID, Please check!"));
			return;
		}

		m_nDestID=nID;

		if(m_nDestID<m_FromID)
		{
			AfxMessageBox(_T("Max ID must Bigger than Min ID, Please Check!"));
			return;
		}

		g_nStartID=m_FromID;
		g_nEndID=m_nDestID;

	}
	
	OnOK();
}


void CScanSelectDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
BOOL CScanSelectDlg::OnInitDialog()
{

	CDialog::OnInitDialog();

	GetDlgItem(IDC_COMBO1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC22)->ShowWindow(SW_HIDE);

	CString strTemp;
	strTemp.LoadString(IDS_FULLYSCANSTR);
	GetDlgItem(IDC_RADIO1)->SetWindowText(strTemp);

	strTemp.LoadString(IDS_SAFESCANSTR);
	GetDlgItem(IDC_RADIO2)->SetWindowText(strTemp);
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	for(int i=0;i<(int)pFrame->m_subNetLst.size();i++)
	{
		m_strScanSubNetNameCobox.AddString(pFrame->m_subNetLst.at(i).strBuildingName);

	}
	m_strScanSubNetNameCobox.SetCurSel(0);


	if(g_ScanSecurity)
	{
		g_Scanfully=FALSE;
		((CButton*) GetDlgItem(IDC_RADIO2))->SetCheck(BST_CHECKED);
		((CButton*) GetDlgItem(IDC_RADIO1))->SetCheck(BST_UNCHECKED);
		CString strText;
		strText.Format(_T("%d"),m_FromID);//
		GetDlgItem(IDC_MINEDIT)->SetWindowText(strText);
		strText.Format(_T("%d"),m_nDestID);
		GetDlgItem(IDC_MAXEDIT)->SetWindowText(strText);
	}
	if(g_Scanfully)
	{
		g_ScanSecurity=FALSE;
		((CButton*) GetDlgItem(IDC_RADIO2))->SetCheck(BST_UNCHECKED);
		((CButton*) GetDlgItem(IDC_RADIO1))->SetCheck(BST_CHECKED);
	}
	ShowSafeScanControl(g_ScanSecurity);
	return TRUE;
}

void CScanSelectDlg::ShowSafeScanControl(BOOL bShow)
{
	if(bShow)
	{
		GetDlgItem(IDC_MINEDIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MAXEDIT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FROMSTATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MINSTATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DESTSTATIC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_MAXSTATIC)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_MINEDIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MAXEDIT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FROMSTATIC)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_MINSTATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DESTSTATIC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_MAXSTATIC)->ShowWindow(SW_HIDE);
	}
}

BOOL CScanSelectDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
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


void CScanSelectDlg::OnEnKillfocusMinedit()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
}

void CScanSelectDlg::OnEnKillfocusMaxedit()
{
	UpdateData(true);
	// TODO: 在此添加控件通知处理程序代码
}
#endif