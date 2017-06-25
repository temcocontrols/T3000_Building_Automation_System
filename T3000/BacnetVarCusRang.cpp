// BacnetVarCusRang.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetVarCusRang.h"
#include "afxdialogex.h"


// CBacnetVarCusRang dialog

IMPLEMENT_DYNAMIC(CBacnetVarCusRang, CDialogEx)

CBacnetVarCusRang::CBacnetVarCusRang(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetVarCusRang::IDD, pParent)
{

}

CBacnetVarCusRang::~CBacnetVarCusRang()
{
}

void CBacnetVarCusRang::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetVarCusRang, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BAC_VAR_OK, &CBacnetVarCusRang::OnBnClickedButtonBacVarOk)
	ON_BN_CLICKED(IDC_BUTTON_BAC_VAR_CANCEL, &CBacnetVarCusRang::OnBnClickedButtonBacVarCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CBacnetVarCusRang message handlers

#include "global_function.h"
void CBacnetVarCusRang::OnBnClickedButtonBacVarOk()
{
	
	GetDlgItemText(IDC_EDIT_BAC_VAR_CUS_RANGE1,Analog_Variable_Units[0]);
	GetDlgItemText(IDC_EDIT_BAC_VAR_CUS_RANGE2,Analog_Variable_Units[1]);
	GetDlgItemText(IDC_EDIT_BAC_VAR_CUS_RANGE3,Analog_Variable_Units[2]);
	GetDlgItemText(IDC_EDIT_BAC_VAR_CUS_RANGE4,Analog_Variable_Units[3]);
	GetDlgItemText(IDC_EDIT_BAC_VAR_CUS_RANGE5,Analog_Variable_Units[4]);
	for (int i=0;i<BAC_VARIABLE_CUS_UNIT_COUNT;i++)
	{
		char cTemp[20];
		if(Analog_Variable_Units[i].GetLength()>19)
			Analog_Variable_Units[i] = Analog_Variable_Units[i].Left(19);
		WideCharToMultiByte( CP_ACP, 0, Analog_Variable_Units[i].GetBuffer(), -1, cTemp, 255, NULL, NULL );
		memcpy(m_variable_analog_unite.at(i).variable_cus_unite,cTemp,20);
	}
	
	if(Write_Private_Data_Blocking(WRITEVARUNIT_T3000,0,BAC_VARIABLE_CUS_UNIT_COUNT - 1) > 0)
	{
		MessageBox(_T("Write OK!"));
	}
	else
	{
		MessageBox(_T("Write Failed!"));
	}

}


void CBacnetVarCusRang::OnBnClickedButtonBacVarCancel()
{
	
}


BOOL CBacnetVarCusRang::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	Initial_Static_Unit();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBacnetVarCusRang::Initial_Static_Unit()
{
	GetDlgItem(IDC_EDIT_BAC_VAR_CUS_RANGE1)->SetWindowTextW(Analog_Variable_Units[0]);
	GetDlgItem(IDC_EDIT_BAC_VAR_CUS_RANGE2)->SetWindowTextW(Analog_Variable_Units[1]);
	GetDlgItem(IDC_EDIT_BAC_VAR_CUS_RANGE3)->SetWindowTextW(Analog_Variable_Units[2]);
	GetDlgItem(IDC_EDIT_BAC_VAR_CUS_RANGE4)->SetWindowTextW(Analog_Variable_Units[3]);
	GetDlgItem(IDC_EDIT_BAC_VAR_CUS_RANGE5)->SetWindowTextW(Analog_Variable_Units[4]);
}

void CBacnetVarCusRang::OnClose()
{
	 

	CDialogEx::OnClose();
}


BOOL CBacnetVarCusRang::PreTranslateMessage(MSG* pMsg)
{
	

	return CDialogEx::PreTranslateMessage(pMsg);
}
