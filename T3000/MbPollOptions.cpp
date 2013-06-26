// MbPollOptions.cpp : implementation file

#include "stdafx.h"
#include "T3000.h"
#include "MbPollOptions.h"
#include "afxdialogex.h"
#include "MbpExterns.h"


// CMbPollOptions dialog

IMPLEMENT_DYNAMIC(CMbPollOptions, CDialog)

CMbPollOptions::CMbPollOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CMbPollOptions::IDD, pParent)
{

}

CMbPollOptions::~CMbPollOptions()
{
}

void CMbPollOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FUNCTION, comboFunction);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, editAddress);
	DDX_Control(pDX, IDC_EDIT_QUANTITY, editQuantity);
	DDX_Control(pDX, IDC_EDIT_SCAN_RATE, editScanRate);
	DDX_Control(pDX, IDC_EDIT_SLAVE_ID, editSlaveId);
	DDX_Control(pDX, IDC_STATIC_ADDRESS, staticAddress);
	DDX_Control(pDX, IDC_STATIC_FUNCTION, staticFunction);
	DDX_Control(pDX, IDC_STATIC_QUANTITY, staticQuantity);
	DDX_Control(pDX, IDC_STATIC_SCAN_RATE, staticScanRate);
	DDX_Control(pDX, IDC_STATIC_SLAVE_ID, staticSlaveId);
	DDX_Control(pDX, IDC_CHECK_ADDRESS_IN_CELL, checkAddressInCell);
	DDX_Control(pDX, IDC_CHECK_DISABLE_ON_ERROR, checkDisableOnError);
	DDX_Control(pDX, IDC_CHECK_HIDE_ALIAS_COLUMN, checkHideAliasColumn);
	DDX_Control(pDX, IDC_CHECK_READ_WRITE_DISABLED, checkReadWriteDisabled);
	DDX_Control(pDX, IDC_COMBO_DISPLAY, comboDisplay);
	DDX_Control(pDX, IDC_RADIO_10, radio10);
	DDX_Control(pDX, IDC_RADIO_100, radio100);
	DDX_Control(pDX, IDC_RADIO_20, radio20);
	DDX_Control(pDX, IDC_RADIO_50, radio50);
	DDX_Control(pDX, IDC_RADIO_FIT_TO_QTY, radioFitToQty);
}


BEGIN_MESSAGE_MAP(CMbPollOptions, CDialog)
END_MESSAGE_MAP()


// CMbPollOptions message handlers


BOOL CMbPollOptions::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: Add your specialized code here and/or call the base class


	return CDialog::Create(lpszTemplateName, pParentWnd);
}

BOOL CMbPollOptions::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	
	CString tempStr;

	optionsSlaveId = pollSlaveId[curConf];
	optionsFunction = pollFunction[curConf];
	optionsAddress = pollAddress[curConf];
	optionsQuantity = pollQuantity[curConf];
	optionsScanRate = pollScanRate[curConf];
	optionsRows = pollRows[curConf];
	optionsDisplay = pollDisplay[curConf];
	optionsHideAliasColumn = pollHideAliasColumn[curConf];
	optionsAddressInCell = pollAddressInCell[curConf];

	tempStr.Format(_T("%d"), optionsSlaveId);
	editSlaveId.SetWindowText(tempStr);
	comboFunction.SetCurSel(optionsFunction);
	tempStr.Format(_T("%d"), optionsAddress);
	editAddress.SetWindowText(tempStr);
	tempStr.Format(_T("%d"), optionsQuantity);
	editQuantity.SetWindowText(tempStr);
	tempStr.Format(_T("%d"), optionsScanRate);
	editScanRate.SetWindowText(tempStr);
	radio10.SetCheck(optionsRows == 10);
	radio20.SetCheck(optionsRows == 20);
	radio50.SetCheck(optionsRows == 50);
	radio100.SetCheck(optionsRows == 100);
	radioFitToQty.SetCheck(optionsRows == 0);
	comboDisplay.SetCurSel(optionsDisplay);
	checkHideAliasColumn.SetCheck(optionsHideAliasColumn);
	checkAddressInCell.SetCheck(optionsAddressInCell);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CMbPollOptions::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CString tempStr;

	editSlaveId.GetWindowText(tempStr);
	pollSlaveId[curConf] = _wtoi(tempStr);
	pollFunction[curConf] = comboFunction.GetCurSel();
	editAddress.GetWindowText(tempStr);
	pollAddress[curConf] = _wtoi(tempStr);
	editQuantity.GetWindowText(tempStr);
	pollQuantity[curConf] = _wtoi(tempStr);
	editScanRate.GetWindowText(tempStr);
	pollScanRate[curConf] = _wtoi(tempStr);
	if (radio10.GetCheck() == TRUE) pollRows[curConf] = 10;
	if (radio20.GetCheck() == TRUE) pollRows[curConf] = 20;
	if (radio50.GetCheck() == TRUE) pollRows[curConf] = 50;
	if (radio100.GetCheck() == TRUE) pollRows[curConf] = 100;
	if (radioFitToQty.GetCheck() == TRUE) pollRows[curConf] = 0;
	pollDisplay[curConf] = comboDisplay.GetCurSel();
	pollHideAliasColumn[curConf] = checkHideAliasColumn.GetCheck();
	pollAddressInCell[curConf] = checkAddressInCell.GetCheck();

	CDialog::OnOK();
}


void CMbPollOptions::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	/*optionsSlaveId = pollSlaveId[curConf];
	optionsFunction = pollFunction[curConf];
	optionsAddress = pollAddress[curConf];
	optionsQuantity = pollQuantity[curConf];
	optionsScanRate = pollScanRate[curConf];
	optionsRows = pollRows[curConf];
	optionsDisplay = pollDisplay[curConf];
	optionsHideAliasColumn = pollHideAliasColumn[curConf];
	optionsAddressInCell = pollAddressInCell[curConf];*/

	CDialog::OnCancel();
}


/*
void CMbPollOptions::OnClickedCheckTapData()
{
	// TODO: Add your control notification handler code here
	if (checkTapData.GetCheck() == TRUE)
	{
		editScanRate.EnableWindow(FALSE);
	}
	else
	{
		editScanRate.EnableWindow(TRUE);
	}
}
*/



