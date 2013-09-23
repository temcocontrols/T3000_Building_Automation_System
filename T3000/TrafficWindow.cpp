// TrafficWindow.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "TrafficWindow.h"
#include "afxdialogex.h"
#include "MbpExterns.h"
#include "globle_function.h"
#include "MbPoll.h"

// TrafficWindow dialog

IMPLEMENT_DYNAMIC(TrafficWindow, CDialogEx)

TrafficWindow::TrafficWindow(CWnd* pParent /*=NULL*/)
	: CDialogEx(TrafficWindow::IDD, pParent)
{
	h_mbPollWindow = pParent;
}

TrafficWindow::~TrafficWindow()
{
}

void TrafficWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TRAFFIC_TEXT1, staticTrafficText);
	DDX_Control(pDX, IDC_LIST_TRAFFIC, listTrafficWindow);
	DDX_Control(pDX, IDC_BTN_CLEAR_TRAFFIC, btnClearTraffic);
	DDX_Control(pDX, IDC_CHECK_AUTOSCROLL, listAutoScroll);
}


BEGIN_MESSAGE_MAP(TrafficWindow, CDialogEx)
	ON_WM_NCDESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CLEAR_TRAFFIC, &TrafficWindow::OnClickedBtnClearTraffic)
	ON_BN_CLICKED(IDC_CHECK_AUTOSCROLL, &TrafficWindow::OnClickedCheckAutoscroll)
END_MESSAGE_MAP()


// TrafficWindow message handlers


BOOL TrafficWindow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CString temp;
	CString temp2;

	temp = L"";

	temp2.Format(_T("%d"), pollSlaveId[trafficSlotNo]);
	temp = temp + L"S=" + temp2;
	
	int tempVar;
	switch(pollFunction[trafficSlotNo])
	{
		case 0: tempVar = 1; break;
		case 1: tempVar = 2; break;
		case 2: tempVar = 3; break;
		case 3: tempVar = 4; break;
		case 4: tempVar = 5; break;
		case 5: tempVar = 6; break;
		case 6: tempVar = 15; break;
		case 7: tempVar = 16; break;
		default: tempVar = 0; break;
	}
	temp2.Format(_T("%02d"), tempVar);
	temp = temp + L", F=" + temp2;

	temp2.Format(_T("%d"), pollAddress[trafficSlotNo]);
	temp = temp + L", A=" + temp2;

	temp2.Format(_T("%d"), pollQuantity[trafficSlotNo]);
	temp = temp + L", Q=" + temp2;

	temp2.Format(_T("%d"), pollScanRate[trafficSlotNo]);
	temp = temp + L", SR=" + temp2;

	staticTrafficText.SetWindowTextW(temp);

	SetTimer(1, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void TrafficWindow::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialogEx::PostNcDestroy();
	boolTrafficWindowOpen = FALSE;
	//h_mbPollWindow->PostMessage(WM_TRAFFIC_CLOSED,0,0);
	delete this;
}

//BOOL TrafficWindow::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
//}


//BOOL TrafficWindow::DestroyWindow()
//{
//	// TODO: Add your specialized code here and/or call the base class
//
//	return CDialogEx::DestroyWindow();
//}

void TrafficWindow::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialogEx::OnCancel();
	OnTrafficWindowDestroy();	
}

void TrafficWindow::OnTrafficWindowDestroy()
{
	DestroyWindow();
}

void TrafficWindow::OnNcDestroy()
{
	CDialogEx::OnNcDestroy();
	trafficSlotNo = -1;
	// TODO: Add your message handler code here
}

CString TxStr;
CString RxStr;

void TrafficWindow::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	static unsigned int count = 0;
	CString temp;

	if (boolTrafficWindowOpen == TRUE)
	{
		if (insertDataString == 1)
		//if (1)
		{
			TxStr = L"";
			temp = L"";
			//temp.Format(_T("%04d-Tx (%02d): "), packetCount, sendDataLen);
			temp.Format(_T("%04d-Tx: "), packetCount);
			packetCount++;
			TxStr = TxStr + temp;
			for (int i = 0; i < sendDataLen; i++)
			{
				temp = L"";
				temp.Format(_T("%02X "), (unsigned char)sendData[i]);
				TxStr = TxStr + temp;
			}
			listTrafficWindow.InsertString(trafficStringIndex++, TxStr);

			RxStr = L"";
			temp = L"";
			//temp.Format(_T("%04d-Rx (%02d): "), packetCount, recvDataLen);
			temp.Format(_T("%04d-Rx: "), packetCount);
			packetCount++;
			RxStr = RxStr + temp;
			for (int i = 0; i < recvDataLen; i++)
			{
				temp = L"";
				temp.Format(_T("%02X "), (unsigned char)recvData[i]);
				RxStr = RxStr + temp;
			}
			listTrafficWindow.InsertString(trafficStringIndex++, RxStr);

			if (autoScroll == 1) listTrafficWindow.SetCurSel(trafficStringIndex - 1);
			
			insertDataString = 0;
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void TrafficWindow::OnClickedBtnClearTraffic()
{
	// TODO: Add your control notification handler code here
	listTrafficWindow.ResetContent();
	trafficStringIndex = 0;
}


void TrafficWindow::OnClickedCheckAutoscroll()
{
	// TODO: Add your control notification handler code here
	if (listAutoScroll.GetCheck() == TRUE)
		autoScroll = 1;
	else
		autoScroll = 0;
}

