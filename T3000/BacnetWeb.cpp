// BacnetWeb.cpp : Implementation file
// BacnetWeb.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetWeb.h"
#include "afxdialogex.h"


// CBacnetWeb dialog box
// CBacnetWeb �Ի���

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


// CBacnetWeb message handlers
// CBacnetWeb ��Ϣ�������


BOOL CBacnetWeb::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO: Add additional initialization here
    // TODO:  �ڴ���Ӷ���ĳ�ʼ��

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
                  // �쳣: OCX ����ҳӦ���� FALSE
}


void CBacnetWeb::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add message handler code and/or call default
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    CDialogEx::OnTimer(nIDEvent);
}
