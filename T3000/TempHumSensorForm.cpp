// TempHumSensorForm.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "TempHumSensorForm.h"
#include "afxdialogex.h"


// CTempHumSensorForm dialog

IMPLEMENT_DYNAMIC(CTempHumSensorForm, CDialogEx)

CTempHumSensorForm::CTempHumSensorForm(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTempHumSensorForm::IDD, pParent)
{

}

CTempHumSensorForm::~CTempHumSensorForm()
{

}

void CTempHumSensorForm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTempHumSensorForm, CDialogEx)
END_MESSAGE_MAP()



