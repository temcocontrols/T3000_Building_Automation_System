// CalibrationHumDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "CalibrationHumDlg.h"
#include "afxdialogex.h"

 

// CCalibrationHumDlg dialog

IMPLEMENT_DYNAMIC(CCalibrationHumDlg, CDialogEx)

CCalibrationHumDlg::CCalibrationHumDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCalibrationHumDlg::IDD, pParent)
	, m_hum(_T(""))
	, m_temp(_T(""))
{

}

CCalibrationHumDlg::~CCalibrationHumDlg()
{
}

void CCalibrationHumDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HUM, m_hum);
	DDX_Text(pDX, IDC_TEMP, m_temp);
}


BEGIN_MESSAGE_MAP(CCalibrationHumDlg, CDialogEx)
    ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CCalibrationHumDlg::OnBnClickedOk)
//	ON_EN_KILLFOCUS(IDC_HUM, &CCalibrationHumDlg::OnEnKillfocusHum)
//	ON_EN_KILLFOCUS(IDC_TEMP, &CCalibrationHumDlg::OnEnKillfocusTemp)
	ON_BN_CLICKED(IDCANCEL, &CCalibrationHumDlg::OnBnClickedCancel)
//	ON_EN_SETFOCUS(IDC_HUM, &CCalibrationHumDlg::OnEnSetfocusHum)
//	ON_EN_SETFOCUS(IDC_TEMP, &CCalibrationHumDlg::OnEnSetfocusTemp)
	ON_EN_KILLFOCUS(IDC_HUM_ED, &CCalibrationHumDlg::OnEnKillfocusHumEd)
	ON_EN_KILLFOCUS(IDC_TEMP_ED, &CCalibrationHumDlg::OnEnKillfocusTempEd)
END_MESSAGE_MAP()


// CCalibrationHumDlg message handlers


void CCalibrationHumDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();

}
BOOL CCalibrationHumDlg::OnInitDialog(){
if (is_connect())
{
  m_humid=read_one(255,6);
}
if (m_humid<0)
{
m_humid=255;
}
  pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;

//恢复T3000主线程
pMain->m_pFreshMultiRegisters->SuspendThread();
pMain->m_pRefreshThread->SuspendThread();//
pMain->m_pFreshTree->SuspendThread();
SetTimer(CALIBRATION_HUM,1000,NULL);

return TRUE;
}

void CCalibrationHumDlg::OnTimer(UINT_PTR nIDEvent){
if (CALIBRATION_HUM==nIDEvent)
{  if (!is_connect())
    return;
   int hum=read_one(m_humid,373);
    if (hum>0)
    {
	m_hum.Format(_T("%0.1f%%"),(float)hum/10.0);
    }
	int temp=read_one(m_humid,101);
	if (temp>0)
	{
	m_temp.Format(_T("%0.1f°C"),(float)temp/10.0);
	}
	UpdateData(FALSE);
   
}

}

//void CCalibrationHumDlg::OnEnKillfocusHum()
//{
//	 
//	 
//	 
//}


//void CCalibrationHumDlg::OnEnKillfocusTemp()
//{
//	
//	
//}


void CCalibrationHumDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//恢复T3000主线程
	pMain->m_pFreshMultiRegisters->ResumeThread();
	pMain->m_pRefreshThread->ResumeThread();//
	pMain->m_pFreshTree->ResumeThread();
	CDialogEx::OnCancel();
}


//void CCalibrationHumDlg::OnEnSetfocusHum()
//{
//	// TODO: Add your control notification handler code here
//
//}


//void CCalibrationHumDlg::OnEnSetfocusTemp()
//{
//	// TODO: Add your control notification handler code here
//	
//}


void CCalibrationHumDlg::OnEnKillfocusHumEd()
{	KillTimer(MiniPanelTimer);
CString temphum;
GetDlgItem(IDC_HUM_ED)->GetWindowText(temphum);
 
	int hum=(int)(_wtof(temphum)*10);
	if (hum>0)
	{
		write_one(m_humid,373,hum,2);
	}
	SetTimer(CALIBRATION_HUM,1000,NULL);
}


void CCalibrationHumDlg::OnEnKillfocusTempEd()
{    KillTimer(MiniPanelTimer);
     CString temptemp;
	 GetDlgItem(IDC_TEMP_ED)->GetWindowText(temptemp);

	int temp=(int)(_wtof(temptemp)*10);
	if (temp>0)
	{
		write_one(m_humid,101,temp,2);
	}
	SetTimer(CALIBRATION_HUM,1000,NULL);
}
