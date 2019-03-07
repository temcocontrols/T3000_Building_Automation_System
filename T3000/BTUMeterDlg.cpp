// BTUMeterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BTUMeterDlg.h"
#include "global_function.h"

#define WM_UPDATE_THREAD_READ  WM_USER + 201
// CBTUMeterDlg
IMPLEMENT_DYNCREATE(CBTUMeterDlg, CFormView)

CBTUMeterDlg::CBTUMeterDlg()
	: CFormView(IDD_DIALOG_BTUMETER)
{
}

CBTUMeterDlg::~CBTUMeterDlg()
{
}

void CBTUMeterDlg::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_ADDRESS, m_editor_address);
    DDX_Control(pDX, IDC_EDIT_SN, m_edit_sn);
    DDX_Control(pDX, IDC_EDIT_MODEL, m_edit_model);
    DDX_Control(pDX, IDC_EDIT_FV, m_edit_fv);
    DDX_Control(pDX, IDC_EDIT_HV, m_edit_hv);
    DDX_Control(pDX, IDC_CO2_BAUDRATECOMBO, m_combox_baudrate);
    DDX_Control(pDX, IDC_COMBO_UNIT, m_combo_temperature_unit);
    DDX_Control(pDX, IDC_COMBO_UNIT2, m_combo_flow_unit);
}

BEGIN_MESSAGE_MAP(CBTUMeterDlg, CFormView)
    ON_MESSAGE(WM_UPDATE_THREAD_READ, UpdateBTUUI)
//    ON_BN_CLICKED(IDC_BUTTON_CLEAR_UP_OFFSET, &CBTUMeterDlg::OnBnClickedButtonClearUpOffset)
//    ON_BN_CLICKED(IDC_BUTTON_CLEAR_UP_OFFSET2, &CBTUMeterDlg::OnBnClickedButtonClearUpOffset2)
    ON_WM_PAINT()
END_MESSAGE_MAP()


// CBTUMeterDlg diagnostics

#ifdef _DEBUG
void CBTUMeterDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CBTUMeterDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBTUMeterDlg message handlers





void CBTUMeterDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	 
}

HANDLE h_btu_thread = NULL;
CString btu_image_fordor;
void CBTUMeterDlg::Fresh()
{
    CString FilePath;

    CString ApplicationFolder;
    GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
    PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
    ApplicationFolder.ReleaseBuffer();
    btu_image_fordor = ApplicationFolder + _T("\\ResourceFile");



	m_combox_baudrate.ResetContent();
	m_combox_baudrate.AddString(_T("9600"));
	m_combox_baudrate.AddString(_T("19200"));
	m_combox_baudrate.AddString(_T("38400"));
	m_combox_baudrate.AddString(_T("57600"));
	m_combox_baudrate.AddString(_T("115200"));

    m_combo_temperature_unit.ResetContent();
    for (int i = 0; i < sizeof(BTU_TEMP_UNIT)/ sizeof(BTU_TEMP_UNIT[0]); i++)
    {
        m_combo_temperature_unit.AddString(BTU_TEMP_UNIT[i]);
    }

    m_combo_flow_unit.ResetContent();
    for (int i = 0; i < sizeof(BTU_FLOW_UNIT) / sizeof(BTU_FLOW_UNIT[0]); i++)
    {
        m_combo_flow_unit.AddString(BTU_FLOW_UNIT[i]);
    }

	CString strTemp;
	strTemp.Format(_T("%d"), product_register_value[6]);
	m_editor_address.SetWindowTextW(strTemp);
	int SN = product_register_value[2 + 1] * 256 * 256 * 256 + product_register_value[2] * 256 * 256 + product_register_value[1] * 256 + product_register_value[0];
	strTemp.Format(_T("%d"), SN);
	m_edit_sn.SetWindowTextW(strTemp);
	strTemp = GetProductName(product_register_value[7]);
 
	m_edit_model.SetWindowTextW(strTemp);
	float fv = ((float)(short)product_register_value[5] * 256 + product_register_value[4]) / 10.0;
	strTemp.Format(_T("%0.1f"), fv);
	m_edit_fv.SetWindowTextW(strTemp);
	strTemp.Format(_T("%d"), product_register_value[8]);
	m_edit_hv.SetWindowTextW(strTemp);
	m_combox_baudrate.SetCurSel(product_register_value[15]);

    if (h_btu_thread == NULL)
        h_btu_thread = CreateThread(NULL, NULL, Update_BTU_Thread, this, NULL, NULL);

}

DWORD WINAPI CBTUMeterDlg::Update_BTU_Thread(LPVOID lPvoid)
{
    CBTUMeterDlg * mparent = (CBTUMeterDlg *)lPvoid;
    while (mparent->IsWindowVisible())
    {
        for (int i = 0; i < 5; i++)
        {
            int itemp = 0;
            itemp = Read_Multi(g_tstat_id, &product_register_value[i * 100], i * 100, 100, 5);
        }
        ::PostMessage(mparent->m_hWnd, WM_UPDATE_THREAD_READ, NULL, NULL);
        Sleep(5000);
    }


    h_btu_thread = NULL;
    return 1;
}

float Datasum(UCHAR FloatByte1, UCHAR FloatByte2, UCHAR FloatByte3, UCHAR FloatByte4)
{
    float aa;
    UCHAR Sflag;
    USHORT Evalue;
    UINT Mvalue;
    UINT Mtemp;
    float mfloat = 0;
    UINT Etemp;
    UCHAR i;

    Sflag = 0x01 & (FloatByte1 >> 7);//indicate it is positive or negative value

    Evalue = FloatByte1 & 0x7f;
    Evalue = Evalue << 1;
    if ((FloatByte2 & 0x80) == 0x80)
        Evalue = Evalue | 0x01;
    else
        Evalue = Evalue & 0xfe;

    Mvalue = FloatByte2 & 0x7f;
    Mvalue = (Mvalue << 16);
    Mvalue |= ((USHORT)FloatByte3 << 8);
    Mvalue |= FloatByte4;


    for (i = 0;i<23;i++)
    {
        Mtemp = (Mvalue >> i) & 0x01;
        if (Mtemp != 0)
            mfloat += (float)1 / (Mtemp << (23 - i));
    }

    Etemp = 0x01 << (Evalue - 127);

    aa = (float)Etemp * (1 + mfloat);
    return aa;
}


LRESULT CBTUMeterDlg::UpdateBTUUI(WPARAM wParam, LPARAM lParam)
{
    if (this->IsWindowVisible() == false)
    {
        TerminateThread(h_btu_thread, 0);
        h_btu_thread = NULL;
    }
    float temp1 = 0;

    Total_heat.Format(_T("%u"), product_register_value[105] + product_register_value[104] * 65536);

    cs_upper_temp.Format(_T("%.1f"), ((float)product_register_value[106]) / 10);

    cs_bottom_temp.Format(_T("%.1f"), ((float)product_register_value[107]) / 10);

    CString cs_upper_temp_setpoint;
    cs_upper_temp_setpoint.Format(_T("%.1f"), ((float)product_register_value[109]) / 10);
    CString cs_bottom_temp_setpoint;
    cs_bottom_temp_setpoint.Format(_T("%.1f"), ((float)product_register_value[110]) / 10);

    GetDlgItem(IDC_EDIT_BTU_UP_TEMP)->SetWindowTextW(cs_upper_temp);
    GetDlgItem(IDC_EDIT_UP_OFFSET)->SetWindowTextW(cs_upper_temp_setpoint);
    GetDlgItem(IDC_EDIT_BTU_BOTTOM_TEMP)->SetWindowTextW(cs_bottom_temp);
    GetDlgItem(IDC_EDIT_BOTTOM_OFFSET)->SetWindowTextW(cs_bottom_temp_setpoint);
 

    if (product_register_value[111] == 1)
    {
        m_combo_temperature_unit.SetCurSel(1);
    }
    else 
    {
        m_combo_temperature_unit.SetCurSel(0);
    }

    if (product_register_value[312] <= (sizeof(BTU_FLOW_UNIT) / sizeof(BTU_FLOW_UNIT[0]) ))
    {
        m_combo_flow_unit.SetCurSel(product_register_value[312]);
        flow_rate_unit = BTU_AD_FLOW_UNIT[product_register_value[312]];
    }
    



    CString temp_p_total;
    CString temp_n_total;
    float float_1;
    product_register_value[306] = 0x1cc4;
    product_register_value[307] = 0x0060;
    UCHAR BYTE1;UCHAR BYTE2;UCHAR BYTE3;UCHAR BYTE4;
    BYTE1 = product_register_value[306] & 0x00ff;
    BYTE2 = (product_register_value[306] & 0xFF00) >> 8;
    BYTE3 = product_register_value[307] & 0x00ff;
    BYTE4 = (product_register_value[307] & 0xFF00) >> 8;
    float_1 = Datasum(BYTE1, BYTE2, BYTE3, BYTE4); // - 625.5
    temp1 = (unsigned int)(product_register_value[304] + product_register_value[305] * 65536) + float_1;
    temp_p_total.Format(_T("%.2f"), temp1);


    BYTE1 = product_register_value[310] & 0x00ff;
    BYTE2 = (product_register_value[310] & 0xFF00) >> 8;
    BYTE3 = product_register_value[311] & 0x00ff;
    BYTE4 = (product_register_value[310] & 0xFF00) >> 8;
    float_1 = Datasum(BYTE1, BYTE2, BYTE3, BYTE4); // - 625.5
    temp1 = (unsigned int)(product_register_value[308] + product_register_value[309] * 65536) + float_1;
    temp_n_total.Format(_T("%.2f"), temp1);

    BYTE1 = product_register_value[302] & 0x00ff;
    BYTE2 = (product_register_value[302] & 0xFF00) >> 8;
    BYTE3 = product_register_value[303] & 0x00ff;
    BYTE4 = (product_register_value[302] & 0xFF00) >> 8;
    float_1 = Datasum(BYTE1, BYTE2, BYTE3, BYTE4); // - 625.5
    flow_rate.Format(_T("%.2f"), float_1);


    GetDlgItem(IDC_EDIT_FLOW_RATE)->SetWindowTextW(flow_rate);
    GetDlgItem(IDC_EDIT_POS_TOTAL_FLOW)->SetWindowTextW(temp_p_total);
    GetDlgItem(IDC_EDIT_NE_TOTAL_FLOW)->SetWindowTextW(temp_n_total);

    if (product_register_value[314] == 0)
    {
        GetDlgItem(IDC_EDIT_BTU_PIPE_STATUS)->SetWindowTextW(_T("Normal"));
    }
    else
    {
        GetDlgItem(IDC_EDIT_BTU_PIPE_STATUS)->SetWindowTextW(_T("Pipe empty"));
    }

    Invalidate(1);
    return 1;
}


//void CBTUMeterDlg::OnBnClickedButtonClearUpOffset()
//{
//    // TODO: 在此添加控件通知处理程序代码
//}


//void CBTUMeterDlg::OnBnClickedButtonClearUpOffset2()
//{
//    // TODO: 在此添加控件通知处理程序代码
//}

const PointF FLOW_RATE(167, 31);

const PointF TOTAL_HEAT(50, 216);
const PointF TEMPERATURE_IN(332, 80);
const PointF TEMPERATURE_OUT(333, 237);


void CBTUMeterDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: 在此处添加消息处理程序代码
                       // 不为绘图消息调用 CFormView::OnPaint()+
    CFileFind temp_find;
    PointF icon_point;
    CMemDC memDC(dc, this);
    int icon_size_x = 667;
    int icon_size_y = 342;

    icon_point.X = 20;
    icon_point.Y = 400;

    CString icon_full_path;
    icon_full_path = btu_image_fordor + _T("\\BTU.jpg");


    CRect rcClient;
    GetClientRect(&rcClient);
    memDC.GetDC().FillSolidRect(&rcClient, GetSysColor(COLOR_WINDOW));
    Graphics graphics_icon(memDC.GetDC());
    if (temp_find.FindFile(icon_full_path))
    {
        Bitmap icon_bitmap(icon_full_path);
        graphics_icon.DrawImage(&icon_bitmap, (int)icon_point.X, (int)icon_point.Y, (int)icon_size_x, (int)icon_size_y);
    }

    FontFamily  ValuefontFamily(_T("Arial"));
    SolidBrush  Font_brush_temp(Color(255, 23, 48, 58));
    Gdiplus::Font  Scroll_font(&ValuefontFamily, 18, FontStyleRegular, UnitPixel);
    graphics_icon.SetSmoothingMode(SmoothingModeAntiAlias);
    PointF      scrollpointF(0, 0);
    scrollpointF.X = icon_point.X + FLOW_RATE.X;
    scrollpointF.Y = icon_point.Y + FLOW_RATE.Y;
    graphics_icon.DrawString(flow_rate + _T(" ") + flow_rate_unit, -1, &Scroll_font, scrollpointF, &Font_brush_temp);

    scrollpointF.X = icon_point.X + TOTAL_HEAT.X;
    scrollpointF.Y = icon_point.Y + TOTAL_HEAT.Y;
    graphics_icon.DrawString(Total_heat + _T("  KWH"), -1, &Scroll_font, scrollpointF, &Font_brush_temp);

    scrollpointF.X = icon_point.X + TEMPERATURE_IN.X;
    scrollpointF.Y = icon_point.Y + TEMPERATURE_IN.Y;
    graphics_icon.DrawString(cs_upper_temp , -1, &Scroll_font, scrollpointF, &Font_brush_temp);

    scrollpointF.X = icon_point.X + TEMPERATURE_OUT.X;
    scrollpointF.Y = icon_point.Y + TEMPERATURE_OUT.Y;
    graphics_icon.DrawString(cs_bottom_temp , -1, &Scroll_font, scrollpointF, &Font_brush_temp);
}
