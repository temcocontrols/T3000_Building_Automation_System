// DisplayLEDLCDConfig.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "DisplayLEDLCDConfig.h"
#include "afxdialogex.h"
#include "DisplayConfig.h"
#include "LedsDialog.h"


// CDisplayLEDLCDConfig dialog

IMPLEMENT_DYNAMIC(CDisplayLEDLCDConfig, CDialogEx)

CDisplayLEDLCDConfig::CDisplayLEDLCDConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDisplayLEDLCDConfig::IDD, pParent)
{

}

CDisplayLEDLCDConfig::~CDisplayLEDLCDConfig()
{
}

void CDisplayLEDLCDConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDisplayLEDLCDConfig, CDialogEx)
    ON_BN_CLICKED(IDC_LED, &CDisplayLEDLCDConfig::OnBnClickedLed)
    ON_BN_CLICKED(IDC_LCD, &CDisplayLEDLCDConfig::OnBnClickedLcd)
END_MESSAGE_MAP()


// CDisplayLEDLCDConfig message handlers


void CDisplayLEDLCDConfig::OnBnClickedLed()
{
    CDialogEx::OnOK();
    CLedsDialog dialog;
    dialog.DoModal();
}


void CDisplayLEDLCDConfig::OnBnClickedLcd()
{
 CDialogEx::OnOK();
    if ((product_register_value[7]==PM_TSTAT5E||(product_register_value[7]==PM_TSTAT5G))||(product_register_value[7]==PM_TSTAT6)||(product_register_value[7]==PM_TSTAT5i)||(product_register_value[7]==PM_TSTAT7))
    {
        CDisplayConfig display_cfg;
        display_cfg.DoModal();
    } 
    else
    {
        AfxMessageBox(_T("This model of TStat don't support Display Config!"));
        return;
    }
}
