// MbPollFunctions.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "MbPollFunctions.h"
#include "afxdialogex.h"
#include "MbpExterns.h"
#include "globle_function.h"


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
	MessageBox(L"Edit1");
}


void CMbPollFunctions::OnClickedButtonSend1()
{
	// TODO: Add your control notification handler code here
	MessageBox(L"Send1");
}


BOOL CMbPollFunctions::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	MessageBox(L"Init");
	editSlave1.SetWindowTextW(L"255");
	editAddress1.SetWindowTextW(L"1");
	editSize1.SetWindowTextW(L"10");

	InitListControl1();
	mbpollFuncList1.GetWindowRect(mbPollFuncListRect);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMbPollFunctions::InitListControl1()
{
	long style;
	style=GetWindowLong(mbpollFuncList1.m_hWnd,GWL_STYLE);
	style&=~LVS_TYPEMASK;
	style|=LVS_REPORT;	

	mbpollFuncList1.SetExtendedStyle(style);
	SetWindowLong(mbpollFuncList1.m_hWnd,GWL_STYLE,style);
	DWORD dwstyle=mbpollFuncList1.GetExtendedStyle();
	dwstyle|=LVS_EX_FULLROWSELECT; 
	dwstyle|=LVS_EX_GRIDLINES;     
	mbpollFuncList1.SetExtendedStyle(dwstyle);

	mbpollFuncList1.InsertColumn(0,_T(""),LVCFMT_LEFT,50);  
	mbpollFuncList1.InsertColumn(1,_T(""),LVCFMT_LEFT,50);

	mbpollFuncList1.SetTextColor(RGB(0,0,255));
	mbpollFuncList1.SetSortable(FALSE);
	mbpollFuncList1.SetGridLines(0);

	mbpollFuncList1.SetHeaderText(0,_T("REG ADDR"));
	mbpollFuncList1.SetHeaderText(1,_T("VALUE"));

	CString tempcs;
	for (int i = 0; i < 10/*ROWS_OF_MBPOLLLIST*/; i++)
	{
		tempcs.Format(_T(""),i);
		mbpollFuncList1.InsertItem(i,tempcs);
	}

	for (int i = 0; i < 10/*ROWS_OF_MBPOLLLIST*/; i++)
	{
		tempcs.Format(_T("%3d"), i);
		mbpollFuncList1.SetItemText(i, 0, tempcs);
	}
}


void CMbPollFunctions::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	MessageBox(L"OK1");

	CDialogEx::OnOK();
}


void CMbPollFunctions::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	MessageBox(L"Cancel1");

	CDialogEx::OnCancel();
}


