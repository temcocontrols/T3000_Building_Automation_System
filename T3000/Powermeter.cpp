// Powermeter.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "Powermeter.h"
#include "afxdialogex.h"


// CPowermeter 对话框

IMPLEMENT_DYNAMIC(CPowermeter, CFormView)

CPowermeter::CPowermeter(CWnd* pParent /*=NULL*/)
	: CFormView(CPowermeter::IDD)
{

}

CPowermeter::~CPowermeter()
{
}

void CPowermeter::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPowermeter, CFormView)
END_MESSAGE_MAP()


// CPowermeter 消息处理程序

void CPowermeter::Fresh()
{

}

void CPowermeter::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
}
