// MbPollFunctions.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "MbPollFunctions.h"
#include "afxdialogex.h"
#include "MbpExterns.h"
#include "globle_function.h"
#include "RegDetails.h"

// CMbPollFunctions dialog

IMPLEMENT_DYNAMIC(CMbPollFunctions, CDialogEx)

CMbPollFunctions::CMbPollFunctions(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMbPollFunctions::IDD, pParent)
{

}

CMbPollFunctions::~CMbPollFunctions()
{
}

void CMbPollFunctions::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CANCEL1, btnCancel1);
	DDX_Control(pDX, IDC_BUTTON_EDIT1, btnEdit1);
	DDX_Control(pDX, IDC_BUTTON_SEND1, btnSend1);
	//  DDX_Control(pDX, IDC_EDIT_ADDRESS1, btnAddress1);
	//  DDX_Control(pDX, IDC_EDIT_SIZE1, btnSize1);
	//  DDX_Control(pDX, IDC_EDIT_SLAVE1, btnSlave1);
	DDX_Control(pDX, IDC_EDIT_ADDRESS1, editAddress1);
	DDX_Control(pDX, IDC_EDIT_SIZE1, editSize1);
	DDX_Control(pDX, IDC_EDIT_SLAVE1, editSlave1);
	DDX_Control(pDX, IDC_LIST_REG1, listReg1);
}


BEGIN_MESSAGE_MAP(CMbPollFunctions, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL1, &CMbPollFunctions::OnClickedButtonCancel1)
	ON_BN_CLICKED(IDC_BUTTON_EDIT1, &CMbPollFunctions::OnClickedButtonEdit1)
	ON_BN_CLICKED(IDC_BUTTON_SEND1, &CMbPollFunctions::OnClickedButtonSend1)
	ON_MESSAGE(MY_MBPOLL_WRITE_REG_MSG, &CMbPollFunctions::OnMyMbpollWriteRegMsg)
	//	ON_EN_UPDATE(IDC_EDIT_SIZE1, &CMbPollFunctions::OnEnUpdateEditSize1)
	ON_EN_KILLFOCUS(IDC_EDIT_SIZE1, &CMbPollFunctions::OnEnKillfocusEditSize1)
	ON_EN_KILLFOCUS(IDC_EDIT_ADDRESS1, &CMbPollFunctions::OnEnKillfocusEditAddress1)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CMbPollFunctions::OnHdnItemclickListReg1)
END_MESSAGE_MAP()


// CMbPollFunctions message handlers


void CMbPollFunctions::OnClickedButtonCancel1()
{
	// TODO: Add your control notification handler code here
	MessageBox(L"Cancel1");
}

void CMbPollFunctions::OnClickedButtonEdit1()
{
	// TODO: Add your control notification handler code here
	int posn;
	POSITION pos = listReg1.GetFirstSelectedItemPosition();
	CString temp;
	temp.Format(_T("%d"),pos);
	posn = (int)pos;
	//MessageBox(temp);
	::PostMessage(hMbpollWritePopWnd,MY_MBPOLL_WRITE_REG_MSG,posn-1,1);
}

void CMbPollFunctions::OnClickedButtonSend1()
{
	// TODO: Add your control notification handler code here
	//MessageBox(L"Send1");
	int slaveID;
	int addrVal;
	int sizeVal;
	CString tempStr;
	unsigned short tempVar[1000] = {0};
	int retVal;
	
	editSlave1.GetWindowTextW(tempStr);
	slaveID = _wtoi(tempStr);
	editAddress1.GetWindowTextW(tempStr);
	addrVal = _wtoi(tempStr);
	editSize1.GetWindowTextW(tempStr);
	sizeVal = _wtoi(tempStr);

	for (int i = 0; i < sizeVal; i++)
	{
		tempStr = listReg1.GetItemText(i, 1);
		tempVar[i] = _wtoi(tempStr);
		//MessageBox(tempStr);
	}
	retVal = write_multi(slaveID, (unsigned char*)(&tempVar[0]), addrVal, sizeVal*2);

	if (retVal < 0)
	{
		MessageBox(L"Send Again. Write not successful.");
	}

	/*
	retVal = write_multi(device_var, to_write, start_address, length);

	if (pollQuantity[slotNo] != 0)
			{
				for (int i = 0; i < pollQuantity[slotNo]; i++)
				{
					tempStr1 = grid->get_TextMatrix(i + 1, 2);
					tempVar[i] = _wtoi(tempStr1);
				}
			}
			write_multi_t(pollSlaveId[slotNo], (unsigned char*)(&tempVar[0]), pollAddress[slotNo], pollQuantity[slotNo] * 2, slotNo);
			*/
}

BOOL CMbPollFunctions::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CString tempStr;

	tempStr.Format(_T("%d"), writeRegPopupSlaveIdValue);
	editSlave1.SetWindowTextW(tempStr);
	tempStr.Format(_T("%d"), writeRegPopupAddressValue);
	editAddress1.SetWindowTextW(tempStr);
	tempStr.Format(_T("%d"), writeRegPopupSizeValue);
	editSize1.SetWindowTextW(tempStr);

	InitListControl1();
	listReg1.GetWindowRect(mbPollFuncListRect);

	hMbpollWritePopWnd = this->m_hWnd;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMbPollFunctions::InitListControl1()
{
	int position;
	int rowNo = 0;
	int addrVal;

	long style;
	style=GetWindowLong(listReg1.m_hWnd,GWL_STYLE);
	style&=~LVS_TYPEMASK;
	style|=LVS_REPORT;	

	listReg1.SetExtendedStyle(style);
	SetWindowLong(listReg1.m_hWnd,GWL_STYLE,style);
	DWORD dwstyle=listReg1.GetExtendedStyle();
	dwstyle|=LVS_EX_FULLROWSELECT; 
	dwstyle|=LVS_EX_GRIDLINES;     
	listReg1.SetExtendedStyle(dwstyle);

	listReg1.InsertColumn(0,_T(""),LVCFMT_LEFT,50);  
	listReg1.InsertColumn(1,_T(""),LVCFMT_LEFT,50);

	listReg1.SetTextColor(RGB(0,0,255));
	listReg1.SetSortable(FALSE);
	listReg1.SetGridLines(0);

	listReg1.SetHeaderText(0,_T("ADDR"));
	listReg1.SetHeaderText(1,_T("VALUE"));

	CString tempcs;

	listReg1.DeleteAllItems();
	for (int i = 0; i < writeRegPopupSizeValue; i++)
	{
		listReg1.InsertItem(i,L"");
	}

	if (writePopupRegStr == L"")
	{
		for (int i = 0; i < writeRegPopupSizeValue; i++)
		{
			tempcs.Format(_T("%03d"), i);
			listReg1.SetItemText(i, 0, tempcs);
			tempcs.Format(_T("%03d"), 0);
			listReg1.SetItemText(i, 1, tempcs);
		}
		return;
	}

	addrVal = writeRegPopupAddressValue;

	while (1)
	{
		position = writePopupRegStr.Find(L";");
		if (position == -1) 
		{
			break;
		}
		tempcs = writePopupRegStr.Left(position);
		if (position != 0)
		{
			writePopupRegStr = writePopupRegStr.Right(writePopupRegStr.GetLength() - position - 1);
		}
		else
		{
			writePopupRegStr = writePopupRegStr.Right(writePopupRegStr.GetLength() - 1);
		}
		listReg1.SetItemText(rowNo, 1, tempcs);
		tempcs.Format(_T("%03d"), addrVal);
		listReg1.SetItemText(rowNo, 0, tempcs);
		rowNo++;
		addrVal++;
	}

	/*
	for (int i = 0; i < writeRegPopupSizeValue; i++)
	{
		tempcs.Format(_T("%03d"), i);
		listReg1.SetItemText(i, 0, tempcs);
		tempcs.Format(_T("%03d"), 0);
		listReg1.SetItemText(i, 1, tempcs);
	}*/
}

void CMbPollFunctions::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	regDetailsOpenedFrom = 0;

	CDialogEx::OnOK();
}

void CMbPollFunctions::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	regDetailsOpenedFrom = 0;
	
	CString tempStr;

	writePopupRegStr = L"";

	editSlave1.GetWindowTextW(tempStr);
	writeRegPopupSlaveIdValue = _wtoi(tempStr);

	editAddress1.GetWindowTextW(tempStr);
	writeRegPopupAddressValue = _wtoi(tempStr);

	editSize1.GetWindowTextW(tempStr);
	writeRegPopupSizeValue = _wtoi(tempStr);

	for (int i = 0; i < writeRegPopupSizeValue; i++)
	{
		tempStr = listReg1.GetItemText(i, 1);
		writePopupRegStr = writePopupRegStr + tempStr + L";";
	}

	//MessageBox(writePopupRegStr);

	CDialogEx::OnCancel();
}

afx_msg LRESULT CMbPollFunctions::OnMyMbpollWriteRegMsg(WPARAM wParam, LPARAM lParam)
{
	CRegDetails Dlg;
	int aliasRow;
	int aliasCol;
	int regValueRow;
	int regValueCol;
	int regAddrRow;
	int regAddrCol;

	editableValueField = 1;

	aliasString = L"";
	regValueString = L"";
	regAddrString = L"";

	if ((wParam < 0) || (lParam < 0))
	{
		return -1;
	}

	aliasRow = wParam;
	aliasCol = lParam;
	regAddrRow = wParam;
	regAddrCol = 0;
	regValueRow = wParam;
	regValueCol = 1;

	//aliasString = listReg1.GetItemText(aliasRow, aliasCol);
	regValueString = listReg1.GetItemText(regValueRow, regValueCol);
	regAddrString = listReg1.GetItemText(regAddrRow, regAddrCol);

	if (Dlg.DoModal() == IDOK)
	{
		listReg1.SetItemText(regValueRow, regValueCol, regValueString);
	}
	else
	{
	}

	return 0;
}

void CMbPollFunctions::OnEnKillfocusEditSize1()
{
	// TODO: Add your control notification handler code here
	listReg1.DeleteAllItems();

	int noOfRows = 0;
	CString tempStr;

	editSize1.GetWindowTextW(tempStr);

	//MessageBox(strSize);

	noOfRows = _wtoi(tempStr);
	writeRegPopupSizeValue = noOfRows;

	for (int i = 0; i < writeRegPopupSizeValue; i++)
	{
		listReg1.InsertItem(i,L"");
	}

	for (int i = 0; i < writeRegPopupSizeValue; i++)
	{
		tempStr.Format(_T("%03d"), writeRegPopupAddressValue + i);
		listReg1.SetItemText(i, 0, tempStr);
		tempStr.Format(_T("%03d"), 0);
		listReg1.SetItemText(i, 1, tempStr);
	}

}

void CMbPollFunctions::OnEnKillfocusEditAddress1()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	listReg1.DeleteAllItems();

	int addrVal = 0;
	CString tempStr;

	editAddress1.GetWindowTextW(tempStr);

	//MessageBox(strSize);

	addrVal = _wtoi(tempStr);
	writeRegPopupAddressValue = addrVal;

	for (int i = 0; i < writeRegPopupSizeValue; i++)
	{
		listReg1.InsertItem(i,L"");
	}

	for (int i = 0; i < writeRegPopupSizeValue; i++)
	{
		tempStr.Format(_T("%03d"), writeRegPopupAddressValue + i);
		listReg1.SetItemText(i, 0, tempStr);
		tempStr.Format(_T("%03d"), 0);
		listReg1.SetItemText(i, 1, tempStr);
	}
	//writeRegPopupSlaveIdValue;

}

void CMbPollFunctions::OnHdnItemclickListReg1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	MessageBox(L"item clicked");
	*pResult = 0;
}
