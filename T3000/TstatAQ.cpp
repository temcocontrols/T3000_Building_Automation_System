// TstatAQ.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "TstatAQ.h"
#include "MainFrm.h"

// CTstatAQ
#define     WM_TSTAT_AQ_THREAD_READ                     WM_USER + 502
HANDLE h_tstat_aq_thread = NULL;
IMPLEMENT_DYNCREATE(CTstatAQ, CFormView)

CTstatAQ::CTstatAQ()
	: CFormView(IDD_DIALOG_TSTAT_AQ)
{

}

CTstatAQ::~CTstatAQ()
{
}

void CTstatAQ::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTstatAQ, CFormView)
    ON_MESSAGE(WM_TSTAT_AQ_THREAD_READ, UpdateUI)
END_MESSAGE_MAP()


// CTstatAQ 诊断

#ifdef _DEBUG
void CTstatAQ::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTstatAQ::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CTstatAQ::Fresh()
{
    ShowWindow(SW_SHOW);
    CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
    pFrame->SetWindowTextW(_T("T3000 Building Automation System") + CurrentT3000Version);
    if (h_tstat_aq_thread == NULL)
        h_tstat_aq_thread = CreateThread(NULL, NULL, Update_TstatAQ_Thread, this, NULL, NULL);
    UpdateUI();

}

void CTstatAQ::UpdateUI()
{
    CString cs_temp;
    cs_temp.Format(_T("%.1f"), ((float)product_register_value[TSTAT_AQ_TEMPERATURE]) / 10);

    CString cs_hum;
    cs_hum.Format(_T("%.1f"), ((float)product_register_value[TSTAT_AQ_HUM]) / 10);

    CString cs_ppm;
    cs_ppm.Format(_T("%d"), product_register_value[TSTAT_AQ_CO2]);

    CString cs_AQ;
    cs_AQ.Format(_T("%d"), product_register_value[TSTAT_AQ_AQ]);

    GetDlgItem(IDC_STATIC_TEMPERATURE_VALUE)->SetWindowTextW(cs_temp);
    GetDlgItem(IDC_STATIC_HUM_VALUE)->SetWindowTextW(cs_hum);
    GetDlgItem(IDC_STATIC_CO2_VALUE)->SetWindowTextW(cs_ppm);
    GetDlgItem(IDC_STATIC_AQ_VALUE)->SetWindowTextW(cs_AQ);

    CString cs_weight_pm1;
    if (product_register_value[TSTAT_AQ_WEIGHT_1] != 0)
        cs_weight_pm1.Format(_T("%u"), product_register_value[TSTAT_AQ_WEIGHT_1]);
    else
        cs_weight_pm1 = _T("-");
    CString cs_weight_pm2_5;
    if(product_register_value[TSTAT_AQ_WEIGHT_2_5] != 0)
        cs_weight_pm2_5.Format(_T("%u"), product_register_value[TSTAT_AQ_WEIGHT_2_5]);
    else
        cs_weight_pm2_5 = _T("-");
    CString cs_weight_pm4;
    if(product_register_value[TSTAT_AQ_WEIGHT_4] != 0)
        cs_weight_pm4.Format(_T("%u"), product_register_value[TSTAT_AQ_WEIGHT_4]);
    else
        cs_weight_pm4 = _T("-");
    CString cs_weight_pm10;
    if(product_register_value[TSTAT_AQ_WEIGHT_10] != 0)
        cs_weight_pm10.Format(_T("%u"), product_register_value[TSTAT_AQ_WEIGHT_10]);
    else
        cs_weight_pm10 = _T("-");

    GetDlgItem(IDC_STATIC_WEIGHT_PM1_0)->SetWindowTextW(cs_weight_pm1);
    GetDlgItem(IDC_STATIC_WEIGHT_PM2_5)->SetWindowTextW(cs_weight_pm2_5);
    GetDlgItem(IDC_STATIC_WEIGHT_PM4_0)->SetWindowTextW(cs_weight_pm4);
    GetDlgItem(IDC_STATIC_WEIGHT_PM10)->SetWindowTextW(cs_weight_pm10);

    CString cs_index_pm1;
    if(product_register_value[TSTAT_AQ_INDEX_1] != 0)
        cs_index_pm1.Format(_T("%u"), product_register_value[TSTAT_AQ_INDEX_1]);
    else
        cs_index_pm1 = _T("-");
    CString cs_index_pm2_5;
    if(product_register_value[TSTAT_AQ_INDEX_2_5] != 0)
        cs_index_pm2_5.Format(_T("%u"), product_register_value[TSTAT_AQ_INDEX_2_5]);
    else
        cs_index_pm2_5 = _T("-");
    CString cs_index_pm4;
    if(product_register_value[TSTAT_AQ_INDEX_4] != 0)
        cs_index_pm4.Format(_T("%u"), product_register_value[TSTAT_AQ_INDEX_4]);
    else
        cs_index_pm4 = _T("-");
    CString cs_index_pm10;
    if(product_register_value[TSTAT_AQ_INDEX_10] != 0)
        cs_index_pm10.Format(_T("%u"), product_register_value[TSTAT_AQ_INDEX_10]);
    else
        cs_index_pm10 = _T("-");

    GetDlgItem(IDC_STATIC_INDEX_PM1_0)->SetWindowTextW(cs_index_pm1);
    GetDlgItem(IDC_STATIC_INDEX_PM2_5)->SetWindowTextW(cs_index_pm2_5);
    GetDlgItem(IDC_STATIC_INDEX_PM4_0)->SetWindowTextW(cs_index_pm4);
    GetDlgItem(IDC_STATIC_INDEX_PM10)->SetWindowTextW(cs_index_pm10);
}
// CTstatAQ 消息处理程序


void CTstatAQ::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    // TODO: 在此添加专用代码和/或调用基类
}

DWORD WINAPI Update_TstatAQ_Thread(LPVOID lPvoid)
{
    CTstatAQ * mparent = (CTstatAQ *)lPvoid;
    while (mparent->IsWindowVisible())
    {
         Read_Multi(g_tstat_id, &product_register_value[100],100, 100, 5);
         Read_Multi(g_tstat_id, &product_register_value[700], 700, 100, 5);

        //for (int i = 0; i < 5; i++)
        //{
        //    int itemp = 0;
        //    itemp = Read_Multi(g_tstat_id, &product_register_value[i * 100], i * 100, 100, 5);
        //}
        PostMessage(mparent->m_hWnd, WM_TSTAT_AQ_THREAD_READ, NULL, NULL);
        Sleep(7000);
    }


    h_tstat_aq_thread = NULL;
    return 1;
}

LRESULT CTstatAQ::UpdateUI(WPARAM wParam, LPARAM lParam)
{
    Fresh();
    return 0;
}
