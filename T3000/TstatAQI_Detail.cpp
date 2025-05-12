// TstatAQI_Detail.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "TstatAQI_Detail.h"
#include "afxdialogex.h"
#include "global_function.h"
#include "global_variable_extern.h"
#include "TstatAQ.h"
// CTstatAQI_Detail 对话框

const int aqi_picture_x = 1022;
const int aqi_picture_y = 530;

IMPLEMENT_DYNAMIC(CTstatAQI_Detail, CDialogEx)

CTstatAQI_Detail::CTstatAQI_Detail(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_AQI, pParent)
{

}

CTstatAQI_Detail::~CTstatAQI_Detail()
{
}

void CTstatAQI_Detail::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTstatAQI_Detail, CDialogEx)
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CTstatAQI_Detail 消息处理程序

HICON default_select;
BOOL CTstatAQI_Detail::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    CRect Temp_Rect;
    GetWindowRect(Temp_Rect);
    MoveWindow(Temp_Rect.left, Temp_Rect.top, aqi_picture_x, aqi_picture_y);
    // TODO:  在此添加额外的初始化
    default_select = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_SELECT), IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}


struct cus_value 
{
    int x, y;
};

cus_value  value1;
cus_value  value2;
cus_value  value3;
cus_value  value4;
cus_value  value5;
cus_value  value6;

PointF      pointF_C1(90, 230);
PointF      pointF_C2(90 - 4, 275);
PointF      pointF_C3(90 - 8, 320);
PointF      pointF_C4(90 - 12, 365);
PointF      pointF_C5(90 - 16, 410);
PointF      pointF_C6(90 - 16, 455);

PointF  Cus_Select(45, 170);
PointF  China_Select(190, 170);
PointF  USA_Select(360, 170);




extern CString bmp_AQI;
CRect mynew_rect3;	//用来存储 窗体应该有多大;
void CTstatAQI_Detail::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: 在此处添加消息处理程序代码
                       // 不为绘图消息调用 CDialogEx::OnPaint()

    value1.x = 0;
    value1.y = product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_FIRST_LINE];
    value2.x = value1.y;
    value2.y = product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_SECOND_LINE];
    value3.x = value2.y;
    value3.y = product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_THIRD_LINE];
    value4.x = value3.y;
    value4.y = product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_FOURTH_LINE];
    value5.x = value4.y;
    value5.y = product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_FIFTH_LINE];
    value6.x = value5.y;
    value6.y = product_register_value[TATAT_AQ_MODBUS_AQI_CUSTOMER_SECOND_LINE];

    CString temp_value_1;
    CString temp_value_2;
    CString temp_value_3;
    CString temp_value_4;
    CString temp_value_5;
    CString temp_value_6;
    temp_value_1.Format(_T("%d - %d"), value1.x, value1.y);
    temp_value_2.Format(_T("%d - %d"), value2.x, value2.y);
    temp_value_3.Format(_T("%d - %d"), value3.x, value3.y);
    temp_value_4.Format(_T("%d - %d"), value4.x, value4.y);
    temp_value_5.Format(_T("%d - %d"), value5.x, value5.y);
    temp_value_6.Format(_T("%d - 500"), value6.x);

                       //TRACE(_T("Screen Edit Paint\r\n"));
    ::GetWindowRect(BacNet_hwd, &mynew_rect3);	//获取 view的窗体大小;
    CMemDC memDC(dc, this);
    CRect rcClient;
    GetClientRect(&rcClient);

    memDC.GetDC().FillSolidRect(&rcClient, RGB(202, 208, 216));
    Graphics graphics(memDC.GetDC());
    Bitmap bitmap(bmp_AQI);
    graphics.DrawImage(&bitmap, 0, 0, aqi_picture_x , aqi_picture_y );

    Graphics *mygraphics;
    mygraphics = new Graphics(memDC.GetDC());

    SolidBrush  txt_color_brush1(Color(255, 0, 0, 0));
    SolidBrush  txt_color_brush2(Color(255, 255, 255, 255));
    FontFamily  UnitfontFamily(_T("Arial"));
    Gdiplus::Font        unitfont(&UnitfontFamily, 20, FontStyleRegular, UnitPixel);

    mygraphics->DrawString(temp_value_1, -1, &unitfont, pointF_C1, &txt_color_brush1);
    mygraphics->DrawString(temp_value_2, -1, &unitfont, pointF_C2, &txt_color_brush1);
    mygraphics->DrawString(temp_value_3, -1, &unitfont, pointF_C3, &txt_color_brush1);
    mygraphics->DrawString(temp_value_4, -1, &unitfont, pointF_C4, &txt_color_brush2);
    mygraphics->DrawString(temp_value_5, -1, &unitfont, pointF_C5, &txt_color_brush2);
    mygraphics->DrawString(temp_value_6, -1, &unitfont, pointF_C6, &txt_color_brush2);

    Bitmap icon_bitmap(default_select);
    if (product_register_value[TATAT_AQ_MODBUS_AQI_AREA] == 0)
    {

            mygraphics->DrawImage(&icon_bitmap, (int)USA_Select.X, (int)USA_Select.Y, (int)24, (int)24);

    }
    else if (product_register_value[TATAT_AQ_MODBUS_AQI_AREA] == 1)
    {

            mygraphics->DrawImage(&icon_bitmap, (int)China_Select.X, (int)China_Select.Y, (int)24, (int)24);

    }
    else
    {

            mygraphics->DrawImage(&icon_bitmap, (int)Cus_Select.X, (int)Cus_Select.Y, (int)24, (int)24);

    }
    delete mygraphics;
}


void CTstatAQI_Detail::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    //CString temp1;
    //temp1.Format(_T("%d,%d"), point.x, point.y);
    //MessageBox(temp1);
    if ((point.x >= (aqi_picture_x - 43)) && (point.y <= 23))
    {
        PostMessage(WM_CLOSE, NULL, NULL);
    }
    CDialogEx::OnLButtonDown(nFlags, point);
}


void CTstatAQI_Detail::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    switch (nIDEvent)
    {
    case 1:

        break;
    default:
        break;
    }
    CDialogEx::OnTimer(nIDEvent);
}
