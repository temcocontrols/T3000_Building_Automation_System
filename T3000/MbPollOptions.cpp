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
	DDX_Control(pDX, IDC_CHECK_HIDE_ALIAS_COLUMN, checkHideAliasColumn);
	DDX_Control(pDX, IDC_COMBO_DISPLAY, comboDisplay);
	DDX_Control(pDX, IDC_RADIO_10, radio10);
	DDX_Control(pDX, IDC_RADIO_100, radio100);
	DDX_Control(pDX, IDC_RADIO_20, radio20);
	DDX_Control(pDX, IDC_RADIO_50, radio50);
	DDX_Control(pDX, IDC_RADIO_FIT_TO_QTY, radioFitToQty);
	DDX_Control(pDX, IDC_CHECK_FUNCTION1, checkSingleFunction);
}

BEGIN_MESSAGE_MAP(CMbPollOptions, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_FUNCTION, &CMbPollOptions::OnCbnSelchangeComboFunction)
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
	optionsSingleFunction = pollSingleFunction[curConf];
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
	tempStr.Format(_T("%d"), optionsSingleFunction);
	
	if (optionsSingleFunction == 1)
	{
		checkSingleFunction.SetCheck(TRUE);
	}
	else
	{
		checkSingleFunction.SetCheck(FALSE);
	}

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
	int outOfRangeData = 0;

	editSlaveId.GetWindowText(tempStr);
	pollSlaveId[curConf] = _wtoi(tempStr);
	if (!((pollSlaveId[curConf] >= 1) && (pollSlaveId[curConf] <= 255)))
	{
		outOfRangeData = 1;
		MessageBox(L"Slave ID shall be: 1 to 255");
	}
	pollFunction[curConf] = comboFunction.GetCurSel();
	editAddress.GetWindowText(tempStr);
	pollAddress[curConf] = _wtoi(tempStr);
	if (!(pollAddress[curConf] >= 0))
	{
		outOfRangeData = 2;
		MessageBox(L"Address shall be: >= 0");
	}
	editQuantity.GetWindowText(tempStr);
	pollQuantity[curConf] = _wtoi(tempStr);
	if (!(pollQuantity[curConf] >= 0))
	{
		outOfRangeData = 3;
		MessageBox(L"Quantity shall be: >= 0");
	}
	editScanRate.GetWindowText(tempStr);
	pollScanRate[curConf] = _wtoi(tempStr);
	//if (!((pollScanRate[curConf] >= 500) && ((pollScanRate[curConf] % 500) == 0)))
	if (!(pollScanRate[curConf] >= 500))
	{
		outOfRangeData = 4;
		MessageBox(L"Scan Rate shall be: >= 500 ms");
	}
	/* In order to have lesser load on processor rounding off timings to multiple of 500 ms */
	int modulus;
	modulus = pollScanRate[curConf] % 500;
	if (modulus < 250)
	{
		pollScanRate[curConf] += (-1 * modulus);
	}
	else
	{
		pollScanRate[curConf] += modulus;
	}

	if (checkSingleFunction.GetCheck() == TRUE) 
		pollSingleFunction[curConf] = 1;
	else
		pollSingleFunction[curConf] = 0;

	if (radio10.GetCheck() == TRUE) pollRows[curConf] = 10;
	if (radio20.GetCheck() == TRUE) pollRows[curConf] = 20;
	if (radio50.GetCheck() == TRUE) pollRows[curConf] = 50;
	if (radio100.GetCheck() == TRUE) pollRows[curConf] = 100;
	if (radioFitToQty.GetCheck() == TRUE) pollRows[curConf] = 0;
	pollDisplay[curConf] = comboDisplay.GetCurSel();
	pollHideAliasColumn[curConf] = checkHideAliasColumn.GetCheck();
	pollAddressInCell[curConf] = checkAddressInCell.GetCheck();

	if (outOfRangeData == 0)
	{
		CDialog::OnOK();
	}
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
	optionsSingleFunction = pollSingleFunction[curConf];
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


void CMbPollOptions::OnCbnSelchangeComboFunction()
{
	// TODO: Add your control notification handler code here
	int functionSelection;

	functionSelection = comboFunction.GetCurSel();
	switch(comboFunction.GetCurSel())
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 6:
		case 7:
			editQuantity.SetReadOnly(FALSE);
			editQuantity.SetWindowText(L"10");
			break;
		case 4:
		case 5:
			editQuantity.SetWindowText(L"1");
			editQuantity.SetReadOnly(TRUE);
			break;
		default:
			editQuantity.SetReadOnly(FALSE);
			editQuantity.SetWindowText(L"1");
			break;
	}
}

