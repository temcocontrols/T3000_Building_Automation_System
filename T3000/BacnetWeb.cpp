// BacnetWeb.cpp : 
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetWeb.h"
#include "afxdialogex.h"


// CBacnetWeb 

IMPLEMENT_DYNAMIC(CBacnetWeb, CDialogEx)

CBacnetWeb::CBacnetWeb(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_WEB, pParent)
{

}

CBacnetWeb::~CBacnetWeb()
{
}

void CBacnetWeb::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetWeb, CDialogEx)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CBacnetWeb 


BOOL CBacnetWeb::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  

    CRect rectClient;
    GetClientRect(&rectClient);
    if (!m_browser.Create(NULL, NULL, WS_VISIBLE, rectClient, this, 101))
    {
        return 0;
    }


    // Initialize the first URL.
    COleVariant noArg;
    //
    CString strURL("file:///D:/temp/python/render.html");
    //CString strURL("file:///Z:\\Temp\\setup\\wifi5.bmp");
    //CString strURL("https://temcocontrols.com/");
    m_browser.put_Silent(VARIANT_TRUE);
    m_browser.Navigate(strURL, &noArg, &noArg, &noArg, &noArg);


    return TRUE;  // return TRUE unless you set the focus to a control
                  // : OCX  FALSE
}


void CBacnetWeb::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: /

    CDialogEx::OnTimer(nIDEvent);
}
