// hangeIDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "hangeIDDlg.h"


// ChangeIDDlg dialog

IMPLEMENT_DYNAMIC(CChangeIDDlg, CDialog)

CChangeIDDlg::CChangeIDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangeIDDlg::IDD, pParent)
{
	m_nTime=30000;
	m_strPromtionText=_T("");
	m_nChange=0;
}

CChangeIDDlg::~CChangeIDDlg()
{
}

void CChangeIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChangeIDDlg, CDialog)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_IGNORERADIO, &CChangeIDDlg::OnBnClickedRadio2)
	ON_BN_KILLFOCUS(IDC_CHANGERADIO, &CChangeIDDlg::OnBnKillfocusRadio1)
	ON_BN_CLICKED(IDOK, &CChangeIDDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHANGEDATABSERADIO, &CChangeIDDlg::OnBnClickedChangedatabseradio)
END_MESSAGE_MAP()


// ChangeIDDlg message handlers

void CChangeIDDlg::OnTimer(UINT_PTR nIDEvent)
{
	 
	if(nIDEvent==1)
	{
		CDialog::OnCancel();
	}
	if(nIDEvent==2)
	{
		int nSeconds=m_nTime/1000-int((GetTickCount()-m_dwTimeStart)/1000);
		CString strTxt;
		strTxt.Format(_T("Cancel(%d sec.)"),nSeconds);
		//GetDlgItem(IDC_TIMERSTATIC)->SetWindowText(strTxt);
		GetDlgItem(IDCANCEL)->SetWindowText(strTxt);
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CChangeIDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	((CButton*) GetDlgItem(IDC_CHANGERADIO))->SetCheck(BST_CHECKED);
	SetTimer(1,m_nTime,NULL);
	SetTimer(2,200,NULL);
	Invalidate();
	m_dwTimeStart=GetTickCount();
	GotoDlgCtrl(GetDlgItem(IDCANCEL));
	

	//if(!m_bShowOkBtn)
	//	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CChangeIDDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CDialog::OnPaint() for painting messages

	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.left=rcClient.left+15;
	rcClient.top=rcClient.top+15;
	rcClient.bottom=rcClient.bottom-30;
	rcClient.right=rcClient.right-15;
	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(m_strPromtionText,&rcClient,DT_LEFT|DT_WORDBREAK);
	
	CString strText;
	strText.Format(_T("Database address %d is correct, node address will be changed to match database"),m_iddb);
	((CButton*) GetDlgItem(IDC_CHANGERADIO))->SetWindowText(strText);
	strText.Format(_T("Network address %d is correct, database will be changed to match"),m_idScanned);
	((CButton*) GetDlgItem(IDC_CHANGEDATABSERADIO))->SetWindowText(strText);
	strText.Format(_T("Cancel, ignore this for now."));
	((CButton*) GetDlgItem(IDC_IGNORERADIO))->SetWindowText(strText);
}

void CChangeIDDlg::SetPromtionTxt(UINT SerialDb,int idDb,UINT SerialScanned,int idScanned)
{
	
	m_serialdb=SerialDb,
	m_iddb=idDb;
	m_serialScaned=SerialScanned;
	m_idScanned=idScanned;
		
	m_strPromtionText.Format(_T("Found node with serial number %d at network address %d, This same node is also in the local database at address %d,Which would you like to accept:"),m_serialScaned,m_idScanned,m_iddb);
		
 //  [ ]  Network address 25 is correct, database will be changed to match

        //             [ ]  Database address 26 is correct, node address will be changed to match database

        //             [ ] Cancel, ignore this for now. 


}

void CChangeIDDlg::OnBnClickedRadio2()
{
	((CButton*) GetDlgItem(IDC_CHANGERADIO))->SetCheck(BST_UNCHECKED);
	((CButton*) GetDlgItem(IDC_IGNORERADIO))->SetCheck(BST_CHECKED);
	
	((CButton*) GetDlgItem(IDC_CHANGEDATABSERADIO))->SetCheck(BST_UNCHECKED);
	m_nChange=3;
}

void CChangeIDDlg::OnBnKillfocusRadio1()
{
	((CButton*) GetDlgItem(IDC_IGNORERADIO))->SetCheck(BST_UNCHECKED);
	((CButton*) GetDlgItem(IDC_CHANGERADIO))->SetCheck(BST_CHECKED);
	((CButton*) GetDlgItem(IDC_CHANGEDATABSERADIO))->SetCheck(BST_UNCHECKED);
	m_nChange=1;
}

void CChangeIDDlg::OnBnClickedOk()
{
	
	OnOK();
}

void CChangeIDDlg::OnBnClickedChangedatabseradio()
{
	((CButton*) GetDlgItem(IDC_IGNORERADIO))->SetCheck(BST_UNCHECKED);
	((CButton*) GetDlgItem(IDC_CHANGERADIO))->SetCheck(BST_UNCHECKED);
	((CButton*) GetDlgItem(IDC_CHANGEDATABSERADIO))->SetCheck(BST_CHECKED);
	m_nChange=2;
}
