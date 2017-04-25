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
 int nFlag = product_register_value[7];
    if ((product_register_value[7]==PM_TSTAT5E||product_register_value[7] == PM_PM5E
		||(product_register_value[7]==PM_TSTAT5G))||(product_register_value[7]==PM_TSTAT6)
		||(product_register_value[7]==PM_TSTAT5i)||(product_register_value[7]==PM_TSTAT8)
		||(product_register_value[7]==PM_TSTAT7) 
		|| (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V))
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
