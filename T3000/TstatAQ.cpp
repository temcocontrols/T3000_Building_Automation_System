// TstatAQ.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "TstatAQ.h"
#include "MainFrm.h"

// CTstatAQ
#define     WM_TSTAT_AQ_THREAD_READ                     WM_USER + 502
HANDLE h_tstat_aq_thread = NULL;
static int aqi_level = 0;
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
    DDX_Control(pDX, IDC_STATIC_AQI_INFO, m_aqi_title);
    DDX_Control(pDX, IDC_STATIC_AQI, m_static_info);
}

BEGIN_MESSAGE_MAP(CTstatAQ, CFormView)
    ON_MESSAGE(WM_TSTAT_AQ_THREAD_READ, UpdateUI)
    ON_EN_KILLFOCUS(IDC_EDIT_CO2_ON_TIME, &CTstatAQ::OnEnKillfocusEditCo2OnTime)
    ON_EN_KILLFOCUS(IDC_EDIT_CO2_OFF_TIME, &CTstatAQ::OnEnKillfocusEditCo2OffTime)
    ON_EN_KILLFOCUS(IDC_EDIT_PM_ON_TIME, &CTstatAQ::OnEnKillfocusEditPmOnTime)
    ON_EN_KILLFOCUS(IDC_EDIT_PM_OFF_TIME, &CTstatAQ::OnEnKillfocusEditPmOffTime)
    ON_BN_CLICKED(IDC_RADIO_DEG_C, &CTstatAQ::OnBnClickedRadioDegC)
    ON_BN_CLICKED(IDC_RADIO_DEG_F, &CTstatAQ::OnBnClickedRadioDegF)
    ON_CBN_SELCHANGE(IDC_COMBO_AQI_REGION, &CTstatAQ::OnCbnSelchangeComboAqiRegion)
    ON_EN_KILLFOCUS(IDC_EDIT_LEVEL_1, &CTstatAQ::OnEnKillfocusEditLevel1)
    ON_EN_KILLFOCUS(IDC_EDIT_LEVEL_2, &CTstatAQ::OnEnKillfocusEditLevel2)
    ON_EN_KILLFOCUS(IDC_EDIT_LEVEL_3, &CTstatAQ::OnEnKillfocusEditLevel3)
    ON_EN_KILLFOCUS(IDC_EDIT_LEVEL_4, &CTstatAQ::OnEnKillfocusEditLevel4)
    ON_EN_KILLFOCUS(IDC_EDIT_LEVEL_5, &CTstatAQ::OnEnKillfocusEditLevel5)
    ON_BN_CLICKED(IDC_BUTTON_CUS_AQI, &CTstatAQ::OnBnClickedButtonCusAqi)
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
CString AQ_image_fordor;
CString bmp_AQI;
void CTstatAQ::Fresh()
{
    CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_AQ_TEMPERATURE); // 得到 Picture Control 句柄 ;
    CString icon_temperature;

   
    CString ApplicationFolder;
    GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
    PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
    ApplicationFolder.ReleaseBuffer();
    AQ_image_fordor = ApplicationFolder + _T("\\ResourceFile");
    icon_temperature = AQ_image_fordor + _T("\\temperature.bmp");
    bmp_AQI = AQ_image_fordor + _T("\\AQI.bmp");
    HBITMAP bitmap;
    bitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(), icon_temperature, IMAGE_BITMAP, 40, 120, LR_LOADFROMFILE);
    CStatic *p = (CStatic *)GetDlgItem(IDC_STATIC_AQ_TEMPERATURE);
    //设置静态控件窗口风格为位图居中显示  
    p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
    //将图片设置到Picture控件上  
    p->SetBitmap(bitmap);

    //HBITMAP bitmap_api;
    //bitmap_api = (HBITMAP)LoadImage(AfxGetInstanceHandle(), bmp_AQI, IMAGE_BITMAP, 646, 437, LR_LOADFROMFILE);
    //CStatic *p2 = (CStatic *)GetDlgItem(IDC_STATIC_AQI);
    ////设置静态控件窗口风格为位图居中显示  
    //p2->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
    ////将图片设置到Picture控件上  
    //p2->SetBitmap(bitmap_api);








    CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
    pFrame->SetWindowTextW(cs_special_name + CurrentT3000Version);
    if (h_tstat_aq_thread == NULL)
        h_tstat_aq_thread = CreateThread(NULL, NULL, Update_TstatAQ_Thread, this, NULL, NULL);
    UpdateUI();


    CString sound_full_path;
    CStatic* pWnd_sound_pic = (CStatic*)GetDlgItem(IDC_STATIC_SOUND); // 得到 Picture Control 句柄 ;
    if(product_register_value[TSTAT_AQ_SOUND] <= 10)
        sound_full_path = ApplicationFolder + _T("\\ResourceFile\\Icon\\sound_0.ico");
    else if (product_register_value[TSTAT_AQ_SOUND] <= 30)
        sound_full_path = ApplicationFolder + _T("\\ResourceFile\\Icon\\sound_1.ico");
    else if (product_register_value[TSTAT_AQ_SOUND] <= 50)
        sound_full_path = ApplicationFolder + _T("\\ResourceFile\\Icon\\sound_2.ico");
    else 
        sound_full_path = ApplicationFolder + _T("\\ResourceFile\\Icon\\sound_3.ico");
    Bitmap icon_bitmap(sound_full_path);
    HICON m_hIcon = ExtractIcon(AfxGetInstanceHandle(), sound_full_path,0);
    pWnd_sound_pic->ModifyStyle(0, SS_ICON | SS_CENTERIMAGE);
    pWnd_sound_pic->SetIcon(m_hIcon);

   


}

void CTstatAQ::EnableCus(bool flag)
{
    if (!flag)
    {
        GetDlgItem(IDC_EDIT_LEVEL_1)->SetWindowTextW(_T(" "));
        GetDlgItem(IDC_EDIT_LEVEL_2)->SetWindowTextW(_T(" "));
        GetDlgItem(IDC_EDIT_LEVEL_3)->SetWindowTextW(_T(" "));
        GetDlgItem(IDC_EDIT_LEVEL_4)->SetWindowTextW(_T(" "));
        GetDlgItem(IDC_EDIT_LEVEL_5)->SetWindowTextW(_T(" "));
    }
    GetDlgItem(IDC_EDIT_LEVEL_1)->EnableWindow(flag);
    GetDlgItem(IDC_EDIT_LEVEL_2)->EnableWindow(flag);
    GetDlgItem(IDC_EDIT_LEVEL_3)->EnableWindow(flag);
    GetDlgItem(IDC_EDIT_LEVEL_4)->EnableWindow(flag);
    GetDlgItem(IDC_EDIT_LEVEL_5)->EnableWindow(flag);
}

void CTstatAQ::UpdateUI()
{

    CString cs_temp;
    cs_temp.Format(_T("%.1f"), ((float)product_register_value[TSTAT_AQ_TEMPERATURE]) / 10);

    CString cs_hum;
    cs_hum.Format(_T("%.1f"), ((float)product_register_value[TSTAT_AQ_HUM]) / 10);

    CString cs_ppm;
    cs_ppm.Format(_T("%d"), product_register_value[TSTAT_AQ_CO2]);

    CString cs_VOC;
    if(product_register_value[7] == PM_TSTAT_AQ)
        cs_VOC.Format(_T("%u"), product_register_value[TSTAT_AQ_VOC_AIRLAB]);
    else
        cs_VOC.Format(_T("%u"), product_register_value[TSTAT_AQ_VOC]);

    CString cs_light;
    cs_light.Format(_T("%u"), product_register_value[TSTAT_AQ_LIGHT]);

    CString cs_sound;
    cs_sound.Format(_T("%u"), product_register_value[TSTAT_AQ_SOUND]);

    CString cs_co2_on;
    CString cs_co2_off;
    CString cs_pm_on;
    CString cs_pm_off;
    cs_co2_on.Format(_T("%u"), product_register_value[TATAT_AQ_CO2_ON]);
    cs_co2_off.Format(_T("%u"), product_register_value[TATAT_AQ_CO2_OFF]);
    cs_pm_on.Format(_T("%u"), product_register_value[TATAT_AQ_PM_ON]);
    cs_pm_off.Format(_T("%u"), product_register_value[TATAT_AQ_PM_OFF]);

    GetDlgItem(IDC_STATIC_TEMPERATURE_VALUE)->SetWindowTextW(cs_temp);
    GetDlgItem(IDC_STATIC_HUM_VALUE)->SetWindowTextW(cs_hum);
    GetDlgItem(IDC_STATIC_CO2_VALUE)->SetWindowTextW(cs_ppm);
    GetDlgItem(IDC_STATIC_VOC_VALUE)->SetWindowTextW(cs_VOC);


    CString cs_api_value;
    cs_api_value.Format(_T("%u"), product_register_value[TATAT_AQ_MODBUS_AQI]);
    CString cs_api_level;
    cs_api_level.Format(_T("%u"), product_register_value[TATAT_AQ_MODBUS_AQI_LEVEL] + 1);

    if ((aqi_level != product_register_value[TATAT_AQ_MODBUS_AQI_LEVEL]) && product_register_value[TATAT_AQ_MODBUS_AQI_LEVEL]<=5)
    {
       aqi_level = product_register_value[TATAT_AQ_MODBUS_AQI_LEVEL];
        m_aqi_title.SetWindowTextW(AQI_Info_Status[aqi_level]);
        m_aqi_title.textColor(AQI_Info_Status_Color[aqi_level]);
        m_aqi_title.bkColor(AQI_Info_Status_Back_Color[aqi_level]);
    }

    CString temp_cus_api[5];
    temp_cus_api[0].Format(_T("%u"), product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_FIRST_LINE]);
    temp_cus_api[1].Format(_T("%u"), product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_SECOND_LINE]);
    temp_cus_api[2].Format(_T("%u"), product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_THIRD_LINE]);
    temp_cus_api[3].Format(_T("%u"), product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_FOURTH_LINE]);
    temp_cus_api[4].Format(_T("%u"), product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_FIFTH_LINE]);

    if (product_register_value[7] == PM_MULTI_SENSOR)
    {
        GetDlgItem(IDC_EDIT_CO2_ON_TIME)->EnableWindow(1);
        GetDlgItem(IDC_EDIT_CO2_OFF_TIME)->EnableWindow(1);
        GetDlgItem(IDC_EDIT_PM_ON_TIME)->EnableWindow(1);
        GetDlgItem(IDC_EDIT_PM_OFF_TIME)->EnableWindow(1);
        GetDlgItem(IDC_EDIT_CO2_ON_TIME)->SetWindowTextW(cs_co2_on);
        GetDlgItem(IDC_EDIT_CO2_OFF_TIME)->SetWindowTextW(cs_co2_off);
        GetDlgItem(IDC_EDIT_PM_ON_TIME)->SetWindowTextW(cs_pm_on);
        GetDlgItem(IDC_EDIT_PM_OFF_TIME)->SetWindowTextW(cs_pm_off);
        GetDlgItem(IDC_STATIC_LIGHT_VALUE)->SetWindowTextW(cs_light);
        GetDlgItem(IDC_STATIC_SOUND_VALUE)->SetWindowTextW(cs_sound);

        GetDlgItem(IDC_EDIT_AQI_VALUE)->SetWindowTextW(_T(" "));
        GetDlgItem(IDC_EDIT_AQI_LEVEL)->SetWindowTextW(_T(" "));
        GetDlgItem(IDC_COMBO_AQI_REGION)->SetWindowTextW(_T(" "));

        EnableCus(0);
    }
    else
    {
        GetDlgItem(IDC_EDIT_CO2_ON_TIME)->EnableWindow(0);
        GetDlgItem(IDC_EDIT_CO2_OFF_TIME)->EnableWindow(0);
        GetDlgItem(IDC_EDIT_PM_ON_TIME)->EnableWindow(0);
        GetDlgItem(IDC_EDIT_PM_OFF_TIME)->EnableWindow(0);
        GetDlgItem(IDC_EDIT_CO2_ON_TIME)->SetWindowTextW(_T(" "));
        GetDlgItem(IDC_EDIT_CO2_OFF_TIME)->SetWindowTextW(_T(" "));
        GetDlgItem(IDC_EDIT_PM_ON_TIME)->SetWindowTextW(_T(" "));
        GetDlgItem(IDC_EDIT_PM_OFF_TIME)->SetWindowTextW(_T(" "));
        GetDlgItem(IDC_STATIC_LIGHT_VALUE)->SetWindowTextW(_T("-"));
        GetDlgItem(IDC_STATIC_SOUND_VALUE)->SetWindowTextW(_T("-"));


        GetDlgItem(IDC_EDIT_AQI_VALUE)->SetWindowTextW(cs_api_value);
        GetDlgItem(IDC_EDIT_AQI_LEVEL)->SetWindowTextW(cs_api_level);
        CString cs_api_region;
        if (product_register_value[TATAT_AQ_MODBUS_AQI_AREA] == 0)
        {
            ((CComboBox *)GetDlgItem(IDC_COMBO_AQI_REGION))->SetCurSel(0);
            EnableCus(0);
        }
        else if (product_register_value[TATAT_AQ_MODBUS_AQI_AREA] == 1)
        {
            ((CComboBox *)GetDlgItem(IDC_COMBO_AQI_REGION))->SetCurSel(1);
            EnableCus(0);
        }
        else 
        {
            ((CComboBox *)GetDlgItem(IDC_COMBO_AQI_REGION))->SetCurSel(2);
            EnableCus(1);
        }
        
        GetDlgItem(IDC_EDIT_LEVEL_1)->SetWindowTextW(temp_cus_api[0]);
        GetDlgItem(IDC_EDIT_LEVEL_2)->SetWindowTextW(temp_cus_api[1]);
        GetDlgItem(IDC_EDIT_LEVEL_3)->SetWindowTextW(temp_cus_api[2]);
        GetDlgItem(IDC_EDIT_LEVEL_4)->SetWindowTextW(temp_cus_api[3]);
        GetDlgItem(IDC_EDIT_LEVEL_5)->SetWindowTextW(temp_cus_api[4]);
    }

    if (product_register_value[TSTAT_AQ_TEMP_UNIT] == 0)
    {
        ((CButton *)GetDlgItem(IDC_RADIO_DEG_C))->SetCheck(1);
        ((CButton *)GetDlgItem(IDC_RADIO_DEG_F))->SetCheck(0);
        GetDlgItem(IDC_STATIC_TEMP_UNITS)->SetWindowTextW(_T("Deg.C"));
        
    }
    else if (product_register_value[TSTAT_AQ_TEMP_UNIT] == 1)
    {
        ((CButton *)GetDlgItem(IDC_RADIO_DEG_C))->SetCheck(0);
        ((CButton *)GetDlgItem(IDC_RADIO_DEG_F))->SetCheck(1);
        GetDlgItem(IDC_STATIC_TEMP_UNITS)->SetWindowTextW(_T("Deg.F"));
    }

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

    CString cs_weight_total;
    cs_weight_total.Format(_T("%u"), product_register_value[TSTAT_AQ_WEIGHT_1] +
        product_register_value[TSTAT_AQ_WEIGHT_2_5] +
        product_register_value[TSTAT_AQ_WEIGHT_4] +
        product_register_value[TSTAT_AQ_WEIGHT_10]);

    GetDlgItem(IDC_STATIC_WEIGHT_PM1_0)->SetWindowTextW(cs_weight_pm1);
    GetDlgItem(IDC_STATIC_WEIGHT_PM2_5)->SetWindowTextW(cs_weight_pm2_5);
    GetDlgItem(IDC_STATIC_WEIGHT_PM4_0)->SetWindowTextW(cs_weight_pm4);
    GetDlgItem(IDC_STATIC_WEIGHT_PM10)->SetWindowTextW(cs_weight_pm10);
    GetDlgItem(IDC_STATIC_WEIGHT_TOTAL)->SetWindowTextW(cs_weight_total);

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

    CString cs_index_total;
    cs_index_total.Format(_T("%u"), product_register_value[TSTAT_AQ_INDEX_1] +
        product_register_value[TSTAT_AQ_INDEX_2_5] +
        product_register_value[TSTAT_AQ_INDEX_4] +
        product_register_value[TSTAT_AQ_INDEX_10]);

    GetDlgItem(IDC_STATIC_INDEX_PM1_0)->SetWindowTextW(cs_index_pm1);
    GetDlgItem(IDC_STATIC_INDEX_PM2_5)->SetWindowTextW(cs_index_pm2_5);
    GetDlgItem(IDC_STATIC_INDEX_PM4_0)->SetWindowTextW(cs_index_pm4);
    GetDlgItem(IDC_STATIC_INDEX_PM10)->SetWindowTextW(cs_index_pm10);
    GetDlgItem(IDC_STATIC_INDEX_TOTAL)->SetWindowTextW(cs_index_total);
}
// CTstatAQ 消息处理程序


void CTstatAQ::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    m_aqi_title.SetWindowTextW(_T(""));
    m_aqi_title.textColor(RGB(255, 255, 255));
    m_aqi_title.bkColor(RGB(0, 0, 255));
    m_aqi_title.setFont(28, 26, NULL, _T("Arial"));

    m_static_info.SetWindowTextW(_T("Air Quality"));
    m_static_info.textColor(RGB(255, 255, 255));
    m_static_info.bkColor(RGB(0, 0, 255));
    m_static_info.setFont(28, 26, NULL, _T("Arial"));
    // TODO: 在此添加专用代码和/或调用基类
}

DWORD WINAPI Update_TstatAQ_Thread(LPVOID lPvoid)
{
    CTstatAQ * mparent = (CTstatAQ *)lPvoid;
    while (mparent->IsWindowVisible())
    {
         Read_Multi(g_tstat_id, &product_register_value[100],100, 100, 5);
         Read_Multi(g_tstat_id, &product_register_value[700], 700, 100, 5);
         Read_Multi(g_tstat_id, &product_register_value[950], 950, 100, 5);
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


void CTstatAQ::OnEnKillfocusEditCo2OnTime()
{
    // TODO: 在此添加控件通知处理程序代码

    CString strText;
    GetDlgItem(IDC_EDIT_CO2_ON_TIME)->GetWindowText(strText);
    int nValue = (int)(_wtoi(strText));


    if (product_register_value[TATAT_AQ_CO2_ON] == nValue)	//Add this to judge weather this value need to change.
        return;


    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_CO2_ON, nValue,
        product_register_value[TATAT_AQ_CO2_ON], this->m_hWnd, IDC_EDIT_DTERM, _T("CO2 On Time"));
}


void CTstatAQ::OnEnKillfocusEditCo2OffTime()
{
    // TODO: 在此添加控件通知处理程序代码
    CString strText;
    GetDlgItem(IDC_EDIT_CO2_OFF_TIME)->GetWindowText(strText);
    int nValue = (int)(_wtoi(strText));


    if (product_register_value[TATAT_AQ_CO2_OFF] == nValue)	//Add this to judge weather this value need to change.
        return;


    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_CO2_OFF, nValue,
        product_register_value[TATAT_AQ_CO2_OFF], this->m_hWnd, IDC_EDIT_DTERM, _T("CO2 Off Time"));
}


void CTstatAQ::OnEnKillfocusEditPmOnTime()
{
    // TODO: 在此添加控件通知处理程序代码
    CString strText;
    GetDlgItem(IDC_EDIT_PM_ON_TIME)->GetWindowText(strText);
    int nValue = (int)(_wtoi(strText));


    if (product_register_value[TATAT_AQ_PM_ON] == nValue)	//Add this to judge weather this value need to change.
        return;


    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_PM_ON, nValue,
        product_register_value[TATAT_AQ_PM_ON], this->m_hWnd, IDC_EDIT_DTERM, _T("PM On Time"));
}


void CTstatAQ::OnEnKillfocusEditPmOffTime()
{
    // TODO: 在此添加控件通知处理程序代码
    CString strText;
    GetDlgItem(IDC_EDIT_PM_OFF_TIME)->GetWindowText(strText);
    int nValue = (int)(_wtoi(strText));


    if (product_register_value[TATAT_AQ_PM_OFF] == nValue)	//Add this to judge weather this value need to change.
        return;


    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_PM_OFF, nValue,
        product_register_value[TATAT_AQ_PM_OFF], this->m_hWnd, IDC_EDIT_DTERM, _T("PM Off Time"));
}


void CTstatAQ::OnBnClickedRadioDegC()
{
    // TODO: 在此添加控件通知处理程序代码
    int nValue = 0;


    if (product_register_value[TSTAT_AQ_TEMP_UNIT] == nValue)	//Add this to judge weather this value need to change.
        return;


    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TSTAT_AQ_TEMP_UNIT, nValue,
        product_register_value[TSTAT_AQ_TEMP_UNIT], this->m_hWnd, IDC_EDIT_DTERM, _T(" Deg.C "));
}


void CTstatAQ::OnBnClickedRadioDegF()
{
    // TODO: 在此添加控件通知处理程序代码
    int nValue = 1;


    if (product_register_value[TSTAT_AQ_TEMP_UNIT] == nValue)	//Add this to judge weather this value need to change.
        return;


    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TSTAT_AQ_TEMP_UNIT, nValue,
        product_register_value[TSTAT_AQ_TEMP_UNIT], this->m_hWnd, IDC_EDIT_DTERM, _T(" Deg.F "));
}


void CTstatAQ::OnCbnSelchangeComboAqiRegion()
{
    // TODO: 在此添加控件通知处理程序代码
    int n_value = 0;
    CString temp_string;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_AQI_REGION))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_AQI_REGION))->GetLBText(nSel, temp_string);
    if (temp_string.CompareNoCase(_T("USA")) == 0)
    {
        n_value = 0;
    }
    else if (temp_string.CompareNoCase(_T("China")) == 0)
    {
        n_value = 1;
    }
    else if (temp_string.CompareNoCase(_T("Custom")) == 0)
    {
        n_value = 2;
    }

    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_MODBUS_AQI_AREA, n_value,
        product_register_value[TATAT_AQ_MODBUS_AQI_AREA], this->m_hWnd, IDC_EDIT_DTERM, _T(" AQI Region "));
}



void CTstatAQ::OnEnKillfocusEditLevel1()
{
    // TODO: 在此添加控件通知处理程序代码

    CString temp_cstring;
    GetDlgItemTextW(IDC_EDIT_LEVEL_1, temp_cstring);
    unsigned int temp_value = unsigned int(_wtoi(temp_cstring));

    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_MODBUS_AQI_CUSTOMER_FIRST_LINE, temp_value,
        product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_FIRST_LINE], this->m_hWnd, IDC_EDIT_LEVEL_1, _T(" Custom Value "));
}


void CTstatAQ::OnEnKillfocusEditLevel2()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_cstring;
    GetDlgItemTextW(IDC_EDIT_LEVEL_2, temp_cstring);
    unsigned int temp_value = unsigned int(_wtoi(temp_cstring));

    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_MODBUS_AQI_CUSTOMER_SECOND_LINE, temp_value,
        product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_SECOND_LINE], this->m_hWnd, IDC_EDIT_LEVEL_2, _T(" Custom Value "));
}


void CTstatAQ::OnEnKillfocusEditLevel3()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_cstring;
    GetDlgItemTextW(IDC_EDIT_LEVEL_3, temp_cstring);
    unsigned int temp_value = unsigned int(_wtoi(temp_cstring));

    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_MODBUS_AQI_CUSTOMER_THIRD_LINE, temp_value,
        product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_THIRD_LINE], this->m_hWnd, IDC_EDIT_LEVEL_3, _T(" Custom Value "));
}


void CTstatAQ::OnEnKillfocusEditLevel4()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_cstring;
    GetDlgItemTextW(IDC_EDIT_LEVEL_4, temp_cstring);
    unsigned int temp_value = unsigned int(_wtoi(temp_cstring));

    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_MODBUS_AQI_CUSTOMER_FOURTH_LINE, temp_value,
        product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_FOURTH_LINE], this->m_hWnd, IDC_EDIT_LEVEL_4, _T(" Custom Value "));
}


void CTstatAQ::OnEnKillfocusEditLevel5()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_cstring;
    GetDlgItemTextW(IDC_EDIT_LEVEL_5, temp_cstring);
    unsigned int temp_value = unsigned int(_wtoi(temp_cstring));

    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_MODBUS_AQI_CUSTOMER_FIFTH_LINE, temp_value,
        product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_FIFTH_LINE], this->m_hWnd, IDC_EDIT_LEVEL_5, _T(" Custom Value "));
}



#include "TstatAQI_Detail.h"
void CTstatAQ::OnBnClickedButtonCusAqi()
{
    // TODO: 在此添加控件通知处理程序代码
    CTstatAQI_Detail dlg;
    dlg.DoModal();
}
