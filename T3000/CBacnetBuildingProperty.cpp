// CBacnetBuildingProperty.cpp: 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "CBacnetBuildingProperty.h"
#include "afxdialogex.h"


// CBacnetBuildingProperty 对话框

IMPLEMENT_DYNAMIC(CBacnetBuildingProperty, CDialogEx)

CBacnetBuildingProperty::CBacnetBuildingProperty(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BACNET_BUILDING_PROPERTY, pParent)
{

}

CBacnetBuildingProperty::~CBacnetBuildingProperty()
{
}

void CBacnetBuildingProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetBuildingProperty, CDialogEx)
END_MESSAGE_MAP()


// CBacnetBuildingProperty 消息处理程序


BOOL CBacnetBuildingProperty::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
