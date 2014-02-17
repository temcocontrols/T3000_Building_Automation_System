// CalibrationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "CalibrationDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CCalibrationDlg dialog

IMPLEMENT_DYNAMIC(CCalibrationDlg, CDialogEx)

CCalibrationDlg::CCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCalibrationDlg::IDD, pParent)
    , m_hum(_T(""))
    , m_Start_Calibration(10000)
{
pMain=NULL;
}

CCalibrationDlg::~CCalibrationDlg()
{
pMain=NULL;
}

void CCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_HUM, m_hum);
    DDX_Control(pDX, IDC_MSFLEXGRID1, m_msflexgrid);
    DDX_Text(pDX, IDC_STARTREGISTER, m_Start_Calibration);
}


BEGIN_MESSAGE_MAP(CCalibrationDlg, CDialogEx)
    ON_BN_CLICKED(IDC_INSERT, &CCalibrationDlg::OnBnClickedInsert)
    ON_BN_CLICKED(IDC_RT, &CCalibrationDlg::OnBnClickedRt)
    ON_BN_CLICKED(IDCANCEL, &CCalibrationDlg::OnBnClickedCancel)
END_MESSAGE_MAP()
BOOL CCalibrationDlg::OnInitDialog(){
CDialogEx::OnInitDialog();

pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
//恢复T3000主线程
pMain->m_pFreshMultiRegisters->SuspendThread();
pMain->m_pRefreshThread->SuspendThread();//
pMain->m_pFreshTree->SuspendThread();

for (int i=0;i<24;i++)
{
    DataBuffer[i]=0;
}
m_msflexgrid.put_Cols(3);
m_msflexgrid.put_Rows(11);
m_msflexgrid.put_TextMatrix(0,0,_T("Point NO"));
m_msflexgrid.put_TextMatrix(0,1,_T("HUM(%)"));
m_msflexgrid.put_TextMatrix(0,2,_T("FRE(H)"));
//    //设置列宽	
m_msflexgrid.put_ColWidth(0,800);
m_msflexgrid.put_ColWidth(1,1500);
m_msflexgrid.put_ColWidth(2,1500);

CString index;
for (int i=1;i<=10;i++)
{
    index.Format(_T("P_%d"),i);
    m_msflexgrid.put_TextMatrix(i,0,index);
    m_msflexgrid.put_RowHeight(i,300);
    index.Format(_T("%d"),DataBuffer[2*(i-1)+1]);
    m_msflexgrid.put_TextMatrix(i,1,index);
    index.Format(_T("%d"),DataBuffer[2*i]);
    m_msflexgrid.put_TextMatrix(i,2,index);
}

return TRUE;
}


void CCalibrationDlg::OnBnClickedInsert()
{
     int ret=write_one(g_tstat_id,m_Start_Calibration+23,1);
     if (ret>0)
     {
         AfxMessageBox(_T("OK"));
     }
     else
     {
        AfxMessageBox(_T("Tray again!"));
     }
}


void CCalibrationDlg::OnBnClickedRt()
{
    UpdateData(TRUE);
    int ret=write_one(g_tstat_id,m_Start_Calibration,1);
    if (ret>0)
    {
        int times=0;
        ret=read_one(g_tstat_id,m_Start_Calibration);
        while(ret>0)
        {
          ++times;
          Sleep(1000);
          ret=read_one(g_tstat_id,m_Start_Calibration);
          if (times>10||ret==0)
          {
            break;
          }
        }

        if (ret==0)
        {
            Read_Multi(g_tstat_id,DataBuffer,m_Start_Calibration,24);
            CString index;
            for (int i=1;i<=10;i++)
            {
                
                index.Format(_T("%d"),DataBuffer[2*(i-1)+1]);
                m_msflexgrid.put_TextMatrix(i,1,index);
                index.Format(_T("%d"),DataBuffer[2*i]);
                m_msflexgrid.put_TextMatrix(i,2,index);
            }
        }
    }

}


void CCalibrationDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    pMain->m_pFreshMultiRegisters->ResumeThread();
    pMain->m_pRefreshThread->ResumeThread();//
    pMain->m_pFreshTree->ResumeThread();
    CDialogEx::OnCancel();
}
