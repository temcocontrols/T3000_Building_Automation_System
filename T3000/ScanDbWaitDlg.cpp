// ScanDbWaitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "ScanDbWaitDlg.h"
#include "TStatScanner.h"

// CScanDbWaitDlg dialog

IMPLEMENT_DYNAMIC(CScanDbWaitDlg, CDialog)

CScanDbWaitDlg::CScanDbWaitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScanDbWaitDlg::IDD, pParent)
	, m_strNetScanInfo(_T(""))
	, m_strComScanInfo(_T(""))
{
m_pScaner = NULL;	g_bCancelScan=FALSE;
m_strNetScanInfo = _T("Net Scan : Start UDP Scan.");
}

CScanDbWaitDlg::~CScanDbWaitDlg()
{
}

void CScanDbWaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INFOSTATIC, m_strInfopromp);
	DDX_Text(pDX, IDC_INFO_NET, m_strNetScanInfo);
	DDX_Text(pDX, IDC_INFO_COM, m_strComScanInfo);
}


BEGIN_MESSAGE_MAP(CScanDbWaitDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CScanDbWaitDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CScanDbWaitDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_EXITBUTTON, &CScanDbWaitDlg::OnBnClickedExitbutton)
	ON_WM_TIMER()
	ON_MESSAGE(WM_NETSCANINFO, OnNetScanInfo)
	ON_MESSAGE(WM_COMSCANINFO, OnComScanInfo)
	ON_MESSAGE(WM_BACNETCOMSCANINFO, OnBacnetComScanInfo)
	
END_MESSAGE_MAP()


void CScanDbWaitDlg::SetTStatScaner(CTStatScanner* pScanner)
{
	ASSERT(pScanner);
	m_pScaner = pScanner;
// SetTimer(1,100,NULL);
}
// CScanDbWaitDlg message handlers
void CScanDbWaitDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CScanDbWaitDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//OnCancel();
}

void CScanDbWaitDlg::OnBnClickedExitbutton()
{
	// TODO: Add your control notification handler code here
		g_bCancelScan=TRUE;
		m_pScaner->StopScan();
		OnCancel();
}

BOOL CScanDbWaitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	g_bCancelScan=FALSE;
	SetTimer(1,100,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CScanDbWaitDlg::OnTimer(UINT_PTR nIDEvent)
{
		static int nCount = 0;
	// TODO: Add your message handler code here and/or call default
		//if(g_ScanSecurity)
		if(0)
	{
		SetWindowText(_T("Scaning..."));
		if(g_ScnnedNum<=g_nEndID)
		{
			CString strTem;
			strTem.Format(_T("Scan from Address ID %d to %d ,Current scanning ID=%d."),g_nStartID,g_nEndID,g_ScnnedNum);
			if(m_strInfopromp.CompareNoCase(strTem)!=0)
				m_strInfopromp=strTem;
			UpdateData(FALSE);
		}
		else
		{

		//	CString strTem;
		//	strTem=g_strScanInfoPrompt;
			SetWindowText(_T("Checking Tstats status..."));

			m_strInfopromp=g_strScanInfoPrompt;
			UpdateData(FALSE);
		}
	}

	/*
	//if(g_Scanfully)
	if (m_pScaner)
	{
		//if(g_ScnnedNum<254)
		if(!m_pScaner->IsAllScanFinished())
		{
					
			if (nCount++ > 30)
			{
				nCount = 0;
			}
			switch(nCount)
			{
			case 5:m_strDot = _T(".");break;
			case 10:m_strDot = _T("..");break;
			case 15:m_strDot = _T("...");break;
			case 20:m_strDot = _T("....");break;
			case 25:m_strDot = _T(".....");break;
			case 30:m_strDot = _T("......");break;
			
			//default:strDot = _T("......");break;
			}
			CString strTemp;
			if (g_nStartID == -1) // 用这个来区分是scan TStat 还是scan NC
			{
				strTemp = _T("sending scan broadcast");
			}
			else
			{
				strTemp.Format(_T("from address ID %d to %d"),g_nStartID,g_nEndID);
			}
		
			CString strTip;
			strTip.Format(_T("Scanning %s, %s. Please wait%s"), g_strScanInfoPrompt, strTemp, m_strDot);
			
			m_strInfopromp=strTip;
			UpdateData(FALSE);
		}
		else
		{

			//CString strTem;
			//strTem=g_strScanInfoPrompt;
			//m_strInfopromp=g_strScanInfoPrompt;
			//UpdateData(FALSE);
		}
	}
	*/
//		if(g_ScnnedNum>=254)

	if (m_pScaner)
	{		
		if(!m_pScaner->IsAllScanFinished())
		{
			if (nCount++ > 30)
			{
				nCount = 0;
			}
			switch(nCount)
			{
			case 5:m_strDot = _T(".");break;
			case 10:m_strDot = _T("..");break;
			case 15:m_strDot = _T("...");break;
			case 20:m_strDot = _T("....");break;
			case 25:m_strDot = _T(".....");break;
			case 30:m_strDot = _T("......");break;

				//default:strDot = _T("......");break;
			}
			CString strTemp;
			CString strTip;
			strTip.Format(_T("T3000 is scanning, please wait%s"), m_strDot);

			m_strInfopromp=strTip;

			//#################################
			{
				CString strTemp;
				strTemp.Format(_T("from address ID %d to %d"), g_nStartID, g_nEndID);
				m_strComScanInfo.Format(_T("COM Scan : Scanning %s, %s."), g_strScanInfoPrompt, strTemp);

			}
			//#################################
			UpdateData(FALSE);
		}
		else
		{

			//CString strTem;
			//strTem=g_strScanInfoPrompt;
			//m_strInfopromp=g_strScanInfoPrompt;
			//UpdateData(FALSE);
		}
	}


	if (m_pScaner)
	{
		if (m_pScaner->IsAllScanFinished())
		{
			KillTimer(1);
			g_ScnnedNum=0;
			CScanDbWaitDlg::OnOK();
		}
	}
	CDialog::OnTimer(nIDEvent);
}
void CScanDbWaitDlg::SetPromtText(CString strInfo)
{
		m_strInfopromp=strInfo;
		UpdateData(FALSE);
}


LRESULT CScanDbWaitDlg::OnNetScanInfo(WPARAM wParam, LPARAM lParam)
{
	CString* pStr = (CString*)(wParam);
	m_strNetScanInfo = _T("Net Scan : ")  + *pStr;
	
	TRACE(_T("net scan info ! \n"));
	//UpdateData(FALSE);
	CWnd* pWnd = GetDlgItem(IDC_INFO_NET);
	pWnd->SetWindowText(m_strNetScanInfo);
	

	delete pStr;
	return 1;
}


LRESULT CScanDbWaitDlg::OnBacnetComScanInfo(WPARAM wParam, LPARAM lParam)
{
	CString* pStr = (CString*)(wParam);
	m_strComScanInfo = _T("Com Port Scan : ") + *pStr;

	//UpdateData(FALSE);

	CWnd* pWnd = GetDlgItem(IDC_INFO_MSTP);
	pWnd->SetWindowText(m_strComScanInfo);
	delete pStr;
	return 1;
}
LRESULT CScanDbWaitDlg::OnComScanInfo(WPARAM wParam, LPARAM lParam)
{
	CString* pStr = (CString*)(wParam);
	m_strComScanInfo = _T("Com Port Scan : ") + *pStr;
	
	//UpdateData(FALSE);
	
	CWnd* pWnd = GetDlgItem(IDC_INFO_COM);
	pWnd->SetWindowText(m_strComScanInfo);
	delete pStr;
	return 1;
}
