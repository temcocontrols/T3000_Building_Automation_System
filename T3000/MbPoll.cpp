// MbPoll.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "MbPoll.h"
#include "afxdialogex.h"
#include "MbpExterns.h"


// CMbPoll dialog

IMPLEMENT_DYNAMIC(CMbPoll, CDialog)

CMbPoll::CMbPoll(CWnd* pParent /*=NULL*/)
	: CDialog(CMbPoll::IDD, pParent)
{
	mParent = pParent;
	/*if (mParent == NULL)
	{
	MessageBox(L"mParent is NULL");
	}
	else
	{
	MessageBox(L"mParent is not NULL");
	}*/
	strGrid1 = _T("");
	valNoOfGrids = 0;
}

CMbPoll::~CMbPoll()
{
}

void CMbPoll::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LABEL1, ctrlLabel1);
	DDX_Control(pDX, IDC_EDIT_FOR_GRID, ctrlEditGrid1);
	DDX_Text(pDX, IDC_EDIT_FOR_GRID, strGrid1);
	DDX_Control(pDX, IDC_MBPOLL_GRID1, ctrlGrid1);
	DDX_Control(pDX, IDC_MBPOLL_GRID2, ctrlGrid2);
	DDX_Control(pDX, IDC_MBPOLL_GRID3, ctrlGrid3);
	DDX_Control(pDX, IDC_MBPOLL_GRID4, ctrlGrid4);
	DDX_Control(pDX, IDC_MBPOLL_GRID5, ctrlGrid5);
	DDX_Control(pDX, IDC_COMBO_NoOFGRIDS, comboNoOfGrids);
	DDX_CBIndex(pDX, IDC_COMBO_NoOFGRIDS, valNoOfGrids);
	DDX_Control(pDX, IDC_BUTTON_CONFIG1, btnConfig1);
	DDX_Control(pDX, IDC_BUTTON_CONFIG2, btnConfig2);
	DDX_Control(pDX, IDC_BUTTON_CONFIG3, btnConfig3);
	DDX_Control(pDX, IDC_BUTTON_CONFIG4, btnConfig4);
	DDX_Control(pDX, IDC_BUTTON_CONFIG5, btnConfig5);
	DDX_Control(pDX, IDC_STATIC_CONFIG1, staticConfig1);
	DDX_Control(pDX, IDC_STATIC_CONFIG2, staticConfig2);
	DDX_Control(pDX, IDC_STATIC_CONFIG3, staticConfig3);
	DDX_Control(pDX, IDC_STATIC_CONFIG4, staticConfig4);
	DDX_Control(pDX, IDC_STATIC_CONFIG5, staticConfig5);
	DDX_Control(pDX, IDC_BUTTON_START_STOP1, btnStartStop1);
	DDX_Control(pDX, IDC_BUTTON_START_STOP2, btnStartStop2);
	DDX_Control(pDX, IDC_BUTTON_START_STOP3, btnStartStop3);
	DDX_Control(pDX, IDC_BUTTON_START_STOP4, btnStartStop4);
	DDX_Control(pDX, IDC_BUTTON_START_STOP5, btnStartStop5);
	DDX_Control(pDX, IDC_CHECK_TAP_DATA, checkTapData);
}

BEGIN_MESSAGE_MAP(CMbPoll, CDialog)
	ON_WM_NCDESTROY()
//	ON_BN_CLICKED(IDCANCEL, &CMbPoll::OnBnClickedCancel)
	ON_BN_CLICKED(IDCLOSE, &CMbPoll::OnBnClickedClose)
	ON_WM_TIMER()
	ON_EN_KILLFOCUS(IDC_EDIT_FOR_GRID, &CMbPoll::OnKillfocusEditForGrid)
	ON_CBN_SELCHANGE(IDC_COMBO_NoOFGRIDS, &CMbPoll::OnSelchangeComboNoofgrids)
	ON_COMMAND(ID_FILE_EXIT, &CMbPoll::OnFileExit)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG1, &CMbPoll::OnClickedButtonConfig1)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG2, &CMbPoll::OnClickedButtonConfig2)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG3, &CMbPoll::OnClickedButtonConfig3)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG4, &CMbPoll::OnClickedButtonConfig4)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG5, &CMbPoll::OnClickedButtonConfig5)
	ON_BN_CLICKED(IDC_BUTTON_START_STOP1, &CMbPoll::OnClickedButtonStartStop1)
	ON_BN_CLICKED(IDC_BUTTON_START_STOP2, &CMbPoll::OnClickedButtonStartStop2)
	ON_BN_CLICKED(IDC_BUTTON_START_STOP3, &CMbPoll::OnClickedButtonStartStop3)
	ON_BN_CLICKED(IDC_BUTTON_START_STOP4, &CMbPoll::OnClickedButtonStartStop4)
	ON_BN_CLICKED(IDC_BUTTON_START_STOP5, &CMbPoll::OnClickedButtonStartStop5)
	ON_BN_CLICKED(IDC_CHECK_TAP_DATA, &CMbPoll::OnClickedCheckTapData)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CMbPoll, CDialog)
	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID1, DISPID_CLICK, CMbPoll::ClickGrid1, VTS_NONE)
	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID1, 71, CMbPoll::EnterCellGrid1, VTS_NONE)
	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID1, 69, CMbPoll::SelChangeGrid1, VTS_NONE)
	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID1, DISPID_DBLCLICK, CMbPoll::DblClickGrid1, VTS_NONE)

	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID2, DISPID_CLICK, CMbPoll::ClickGrid2, VTS_NONE)
	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID2, 71, CMbPoll::EnterCellGrid2, VTS_NONE)
	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID2, 69, CMbPoll::SelChangeGrid2, VTS_NONE)
	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID2, DISPID_DBLCLICK, CMbPoll::DblClickGrid2, VTS_NONE)

	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID3, DISPID_CLICK, CMbPoll::ClickGrid3, VTS_NONE)
	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID3, 71, CMbPoll::EnterCellGrid3, VTS_NONE)
	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID3, 69, CMbPoll::SelChangeGrid3, VTS_NONE)
	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID3, DISPID_DBLCLICK, CMbPoll::DblClickGrid3, VTS_NONE)

	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID4, DISPID_CLICK, CMbPoll::ClickGrid4, VTS_NONE)
	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID4, 71, CMbPoll::EnterCellGrid4, VTS_NONE)
	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID4, 69, CMbPoll::SelChangeGrid4, VTS_NONE)
	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID4, DISPID_DBLCLICK, CMbPoll::DblClickGrid4, VTS_NONE)

	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID5, DISPID_CLICK, CMbPoll::ClickGrid5, VTS_NONE)
	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID5, 71, CMbPoll::EnterCellGrid5, VTS_NONE)
	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID5, 69, CMbPoll::SelChangeGrid5, VTS_NONE)
	ON_EVENT(CMbPoll, IDC_MBPOLL_GRID5, DISPID_DBLCLICK, CMbPoll::DblClickGrid5, VTS_NONE)
END_EVENTSINK_MAP()


// CMbPoll message handlers

void CMbPoll::ClickGrid(CMsflexgrid &grid)
{
	long lRow,lCol;
	lRow = grid.get_RowSel();
	lCol = grid.get_ColSel();

	currentRow = lRow;
	currentCol = lCol;

	CRect rect;
	grid.GetWindowRect(rect);
	ScreenToClient(rect);
	CDC* pDC =GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;

	long y = grid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = grid.get_ColPos(lCol)/nTwipsPerDotX;

	long width = grid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = grid.get_RowHeight(lRow)/nTwipsPerDotY+1;

	CRect rcCell(x,y,x+width,y+height);
	rcCell.OffsetRect(rect.left+1,rect.top+1);
	ReleaseDC(pDC);
	
	CString strValue = grid.get_TextMatrix(lRow,lCol);

	ctrlEditGrid1.MoveWindow(&rcCell,1);
	ctrlEditGrid1.ShowWindow(SW_SHOW);
	ctrlEditGrid1.SetWindowText(strValue);
	ctrlEditGrid1.SetFocus();

	int nLenth=strValue.GetLength();
	ctrlEditGrid1.SetSel(0,nLenth);	
}

#if 1
void CMbPoll::ClickGrid1(){ClickGrid(ctrlGrid1); curGridForEdit = 1;}
void CMbPoll::ClickGrid2(){ClickGrid(ctrlGrid2); curGridForEdit = 2;}
void CMbPoll::ClickGrid3(){ClickGrid(ctrlGrid3); curGridForEdit = 3;}
void CMbPoll::ClickGrid4(){ClickGrid(ctrlGrid4); curGridForEdit = 4;}
void CMbPoll::ClickGrid5(){ClickGrid(ctrlGrid5); curGridForEdit = 5;}
void CMbPoll::EnterCellGrid1(){/*MessageBox(L"EnterCell");*/}
void CMbPoll::SelChangeGrid1(){/*MessageBox(L"SelChange");*/}
void CMbPoll::DblClickGrid1(){/*MessageBox(L"DblClick");*/}
void CMbPoll::EnterCellGrid2(){/*MessageBox(L"EnterCell");*/}
void CMbPoll::SelChangeGrid2(){/*MessageBox(L"SelChange");*/}
void CMbPoll::DblClickGrid2(){/*MessageBox(L"DblClick");*/}
void CMbPoll::EnterCellGrid3(){/*MessageBox(L"EnterCell");*/}
void CMbPoll::SelChangeGrid3(){/*MessageBox(L"SelChange");*/}
void CMbPoll::DblClickGrid3(){/*MessageBox(L"DblClick");*/}
void CMbPoll::EnterCellGrid4(){/*MessageBox(L"EnterCell");*/}
void CMbPoll::SelChangeGrid4(){/*MessageBox(L"SelChange");*/}
void CMbPoll::DblClickGrid4(){/*MessageBox(L"DblClick");*/}
void CMbPoll::EnterCellGrid5(){/*MessageBox(L"EnterCell");*/}
void CMbPoll::SelChangeGrid5(){/*MessageBox(L"SelChange");*/}
void CMbPoll::DblClickGrid5(){/*MessageBox(L"DblClick");*/}
#endif

BOOL CMbPoll::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(nIDTemplate, pParentWnd);
}

void CMbPoll::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	// TODO: Add your message handler code here
}

void CMbPoll::PostNcDestroy() 
{	
    CDialog::PostNcDestroy();

	/*if (GetParent() == NULL)
	{
		MessageBox(L"GetParent returns NULL");
	}
	else
	{
		MessageBox(L"GetParent ok");
	}*/
	mParent->PostMessage(WM_MBPOLL_CLOSED,0,0);
	delete this;
}

void CMbPoll::OnBnClickedClose()
{
	// TODO: Add your control notification handler code here
	KillTimer(1);
	KillTimer(2);
	CDialog::OnCancel();
	OnMbPollDestroyWindow();	
}

void CMbPoll::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	static int a = 0;
	CString tempStr;
	tempStr.Format(_T("%d"),a);
	ctrlLabel1.SetWindowTextW(tempStr);
	a++;

	switch(nIDEvent)
	{
	case 1:	// 2s
		{
			//testFunc();
			break;
		}
	case 2: // 500 ms
		{
			if ((pollShown[0] == 1) && (grid1Data == NULL)) break;
			if ((pollShown[1] == 1) && (grid1Data == NULL)) break;
			if ((pollShown[2] == 1) && (grid1Data == NULL)) break;
			if ((pollShown[3] == 1) && (grid1Data == NULL)) break;
			if ((pollShown[4] == 1) && (grid1Data == NULL)) break;

			CString tempStr1;
			if (grid1Data != NULL) 
			{
				for (int i = 0; i < pollQuantity[0]; i++)
				{
					tempStr1.Format(_T("%x"), (pollAddress[0] + i)); 
					ctrlGrid1.put_TextMatrix(i + 1, 1, tempStr1);
					tempStr1.Format(_T("%d"), *((short int*)grid1Data + i)); 
					ctrlGrid1.put_TextMatrix(i + 1, 2, tempStr1);
				}
			}

			if (grid2Data != NULL) 
			{
				for (int i = 0; i < pollQuantity[1]; i++)
				{
					tempStr1.Format(_T("%x"), (pollAddress[1] + i)); 
					ctrlGrid2.put_TextMatrix(i + 1, 1, tempStr1);
					tempStr1.Format(_T("%d"), *((short int*)grid2Data + i)); 
					ctrlGrid2.put_TextMatrix(i + 1, 2, tempStr1);
				}
			}

			if (grid3Data != NULL) 
			{
				for (int i = 0; i < pollQuantity[2]; i++)
				{
					tempStr1.Format(_T("%x"), (pollAddress[2] + i)); 
					ctrlGrid3.put_TextMatrix(i + 1, 1, tempStr1);
					tempStr1.Format(_T("%d"), *((short int*)grid3Data + i)); 
					ctrlGrid3.put_TextMatrix(i + 1, 2, tempStr1);
				}
			}

			if (grid4Data != NULL) 
			{
				for (int i = 0; i < pollQuantity[3]; i++)
				{
					tempStr1.Format(_T("%x"), (pollAddress[3] + i)); 
					ctrlGrid4.put_TextMatrix(i + 1, 1, tempStr1);
					tempStr1.Format(_T("%d"), *((short int*)grid4Data + i)); 
					ctrlGrid4.put_TextMatrix(i + 1, 2, tempStr1);
				}
			}

			if (grid5Data != NULL) 
			{
				for (int i = 0; i < pollQuantity[4]; i++)
				{
					tempStr1.Format(_T("%x"), (pollAddress[4] + i)); 
					ctrlGrid5.put_TextMatrix(i + 1, 1, tempStr1);
					tempStr1.Format(_T("%d"), *((short int*)grid5Data + i)); 
					ctrlGrid5.put_TextMatrix(i + 1, 2, tempStr1);
				}
			}

			break;
		}
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);

}

void CMbPoll::OnKillfocusEditForGrid()
{
	// TODO: Add your control notification handler code here

	CString strText;
	ctrlEditGrid1.GetWindowText(strText);

	if (curGridForEdit == 1)
		ctrlGrid1.put_TextMatrix(currentRow, currentCol, strText);
	if (curGridForEdit == 2)
		ctrlGrid2.put_TextMatrix(currentRow, currentCol, strText);
	if (curGridForEdit == 3)
		ctrlGrid3.put_TextMatrix(currentRow, currentCol, strText);
	if (curGridForEdit == 4)
		ctrlGrid4.put_TextMatrix(currentRow, currentCol, strText);
	if (curGridForEdit == 5)
		ctrlGrid5.put_TextMatrix(currentRow, currentCol, strText);
}

void CMbPoll::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialog::OnOK();
}

void CMbPoll::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnCancel();
	OnMbPollDestroyWindow();
}

BOOL CMbPoll::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	ctrlEditGrid1.ShowWindow(SW_HIDE);
	comboNoOfGrids.SetCurSel(noOfGridTables);
	displayGrids(noOfGridTables);
	tapDataMode = 0;

	ctrlGrid1.put_TextMatrix(0, 0, L"Alias");
	ctrlGrid1.put_TextMatrix(0, 1, L"Addr");
	ctrlGrid1.put_TextMatrix(0, 2, L"Value");

	ctrlGrid2.put_TextMatrix(0, 0, L"Alias");
	ctrlGrid2.put_TextMatrix(0, 1, L"Addr");
	ctrlGrid2.put_TextMatrix(0, 2, L"Value");

	ctrlGrid3.put_TextMatrix(0, 0, L"Alias");
	ctrlGrid3.put_TextMatrix(0, 1, L"Addr");
	ctrlGrid3.put_TextMatrix(0, 2, L"Value");

	ctrlGrid4.put_TextMatrix(0, 0, L"Alias");
	ctrlGrid4.put_TextMatrix(0, 1, L"Addr");
	ctrlGrid4.put_TextMatrix(0, 2, L"Value");

	ctrlGrid5.put_TextMatrix(0, 0, L"Alias");
	ctrlGrid5.put_TextMatrix(0, 1, L"Addr");
	ctrlGrid5.put_TextMatrix(0, 2, L"Value");

	SetTimer(1, 2000, NULL);
	SetTimer(2, 500, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMbPoll::OnSelchangeComboNoofgrids()
{
	// TODO: Add your control notification handler code here
	displayGrids(comboNoOfGrids.GetCurSel());
}

void CMbPoll::displayGrids(int number)
{

	switch(number)
	{
	case 0:
		btnConfig1.ShowWindow(SW_HIDE);
		btnConfig2.ShowWindow(SW_HIDE);
		btnConfig3.ShowWindow(SW_HIDE);
		btnConfig4.ShowWindow(SW_HIDE);
		btnConfig5.ShowWindow(SW_HIDE);
		ctrlGrid1.ShowWindow(SW_HIDE);
		ctrlGrid2.ShowWindow(SW_HIDE);
		ctrlGrid3.ShowWindow(SW_HIDE);
		ctrlGrid4.ShowWindow(SW_HIDE);
		ctrlGrid5.ShowWindow(SW_HIDE);
		staticConfig1.ShowWindow(SW_HIDE);
		staticConfig2.ShowWindow(SW_HIDE);
		staticConfig3.ShowWindow(SW_HIDE);
		staticConfig4.ShowWindow(SW_HIDE);
		staticConfig5.ShowWindow(SW_HIDE);
		ctrlEditGrid1.ShowWindow(SW_HIDE);
		/*btnStartStop1.ShowWindow(SW_HIDE);
		btnStartStop2.ShowWindow(SW_HIDE);
		btnStartStop3.ShowWindow(SW_HIDE);
		btnStartStop4.ShowWindow(SW_HIDE);
		btnStartStop5.ShowWindow(SW_HIDE);*/
		pollShown[0] = 0;
		pollShown[1] = 0;
		pollShown[2] = 0;
		pollShown[3] = 0;
		pollShown[4] = 0;
		break;
	case 1:
		btnConfig1.ShowWindow(SW_SHOW);
		btnConfig2.ShowWindow(SW_HIDE);
		btnConfig3.ShowWindow(SW_HIDE);
		btnConfig4.ShowWindow(SW_HIDE);
		btnConfig5.ShowWindow(SW_HIDE);
		ctrlGrid1.ShowWindow(SW_SHOW);
		ctrlGrid2.ShowWindow(SW_HIDE);
		ctrlGrid3.ShowWindow(SW_HIDE);
		ctrlGrid4.ShowWindow(SW_HIDE);
		ctrlGrid5.ShowWindow(SW_HIDE);
		staticConfig1.ShowWindow(SW_SHOW);
		staticConfig2.ShowWindow(SW_HIDE);
		staticConfig3.ShowWindow(SW_HIDE);
		staticConfig4.ShowWindow(SW_HIDE);
		staticConfig5.ShowWindow(SW_HIDE);
		/*btnStartStop1.ShowWindow(SW_SHOW);
		btnStartStop2.ShowWindow(SW_HIDE);
		btnStartStop3.ShowWindow(SW_HIDE);
		btnStartStop4.ShowWindow(SW_HIDE);
		btnStartStop5.ShowWindow(SW_HIDE);*/
		pollShown[0] = 1;
		pollShown[1] = 0;
		pollShown[2] = 0;
		pollShown[3] = 0;
		pollShown[4] = 0;
		break;
	case 2:
		btnConfig1.ShowWindow(SW_SHOW);
		btnConfig2.ShowWindow(SW_SHOW);
		btnConfig3.ShowWindow(SW_HIDE);
		btnConfig4.ShowWindow(SW_HIDE);
		btnConfig5.ShowWindow(SW_HIDE);
		ctrlGrid1.ShowWindow(SW_SHOW);
		ctrlGrid2.ShowWindow(SW_SHOW);
		ctrlGrid3.ShowWindow(SW_HIDE);
		ctrlGrid4.ShowWindow(SW_HIDE);
		ctrlGrid5.ShowWindow(SW_HIDE);
		staticConfig1.ShowWindow(SW_SHOW);
		staticConfig2.ShowWindow(SW_SHOW);
		staticConfig3.ShowWindow(SW_HIDE);
		staticConfig4.ShowWindow(SW_HIDE);
		staticConfig5.ShowWindow(SW_HIDE);
		/*btnStartStop1.ShowWindow(SW_SHOW);
		btnStartStop2.ShowWindow(SW_SHOW);
		btnStartStop3.ShowWindow(SW_HIDE);
		btnStartStop4.ShowWindow(SW_HIDE);
		btnStartStop5.ShowWindow(SW_HIDE);*/
		pollShown[0] = 1;
		pollShown[1] = 1;
		pollShown[2] = 0;
		pollShown[3] = 0;
		pollShown[4] = 0;
		break;
	case 3:
		btnConfig1.ShowWindow(SW_SHOW);
		btnConfig2.ShowWindow(SW_SHOW);
		btnConfig3.ShowWindow(SW_SHOW);
		btnConfig4.ShowWindow(SW_HIDE);
		btnConfig5.ShowWindow(SW_HIDE);
		ctrlGrid1.ShowWindow(SW_SHOW);
		ctrlGrid2.ShowWindow(SW_SHOW);
		ctrlGrid3.ShowWindow(SW_SHOW);
		ctrlGrid4.ShowWindow(SW_HIDE);
		ctrlGrid5.ShowWindow(SW_HIDE);
		staticConfig1.ShowWindow(SW_SHOW);
		staticConfig2.ShowWindow(SW_SHOW);
		staticConfig3.ShowWindow(SW_SHOW);
		staticConfig4.ShowWindow(SW_HIDE);
		staticConfig5.ShowWindow(SW_HIDE);
		/*btnStartStop1.ShowWindow(SW_SHOW);
		btnStartStop2.ShowWindow(SW_SHOW);
		btnStartStop3.ShowWindow(SW_SHOW);
		btnStartStop4.ShowWindow(SW_HIDE);
		btnStartStop5.ShowWindow(SW_HIDE);*/
		pollShown[0] = 1;
		pollShown[1] = 1;
		pollShown[2] = 1;
		pollShown[3] = 0;
		pollShown[4] = 0;
		break;
	case 4:
		btnConfig1.ShowWindow(SW_SHOW);
		btnConfig2.ShowWindow(SW_SHOW);
		btnConfig3.ShowWindow(SW_SHOW);
		btnConfig4.ShowWindow(SW_SHOW);
		btnConfig5.ShowWindow(SW_HIDE);
		ctrlGrid1.ShowWindow(SW_SHOW);
		ctrlGrid2.ShowWindow(SW_SHOW);
		ctrlGrid3.ShowWindow(SW_SHOW);
		ctrlGrid4.ShowWindow(SW_SHOW);
		ctrlGrid5.ShowWindow(SW_HIDE);
		staticConfig1.ShowWindow(SW_SHOW);
		staticConfig2.ShowWindow(SW_SHOW);
		staticConfig3.ShowWindow(SW_SHOW);
		staticConfig4.ShowWindow(SW_SHOW);
		staticConfig5.ShowWindow(SW_HIDE);
		/*btnStartStop1.ShowWindow(SW_SHOW);
		btnStartStop2.ShowWindow(SW_SHOW);
		btnStartStop3.ShowWindow(SW_SHOW);
		btnStartStop4.ShowWindow(SW_SHOW);
		btnStartStop5.ShowWindow(SW_HIDE);*/
		pollShown[0] = 1;
		pollShown[1] = 1;
		pollShown[2] = 1;
		pollShown[3] = 1;
		pollShown[4] = 0;
		break;
	case 5:
		btnConfig1.ShowWindow(SW_SHOW);
		btnConfig2.ShowWindow(SW_SHOW);
		btnConfig3.ShowWindow(SW_SHOW);
		btnConfig4.ShowWindow(SW_SHOW);
		btnConfig5.ShowWindow(SW_SHOW);
		ctrlGrid1.ShowWindow(SW_SHOW);
		ctrlGrid2.ShowWindow(SW_SHOW);
		ctrlGrid3.ShowWindow(SW_SHOW);
		ctrlGrid4.ShowWindow(SW_SHOW);
		ctrlGrid5.ShowWindow(SW_SHOW);
		staticConfig1.ShowWindow(SW_SHOW);
		staticConfig2.ShowWindow(SW_SHOW);
		staticConfig3.ShowWindow(SW_SHOW);
		staticConfig4.ShowWindow(SW_SHOW);
		staticConfig5.ShowWindow(SW_SHOW);
		/*btnStartStop1.ShowWindow(SW_SHOW);
		btnStartStop2.ShowWindow(SW_SHOW);
		btnStartStop3.ShowWindow(SW_SHOW);
		btnStartStop4.ShowWindow(SW_SHOW);
		btnStartStop5.ShowWindow(SW_SHOW);*/
		pollShown[0] = 1;
		pollShown[1] = 1;
		pollShown[2] = 1;
		pollShown[3] = 1;
		pollShown[4] = 1;
		break;
	default:
		btnConfig1.ShowWindow(SW_SHOW);
		btnConfig2.ShowWindow(SW_HIDE);
		btnConfig3.ShowWindow(SW_HIDE);
		btnConfig4.ShowWindow(SW_HIDE);
		btnConfig5.ShowWindow(SW_HIDE);
		ctrlGrid1.ShowWindow(SW_SHOW);
		ctrlGrid2.ShowWindow(SW_HIDE);
		ctrlGrid3.ShowWindow(SW_HIDE);
		ctrlGrid4.ShowWindow(SW_HIDE);
		ctrlGrid5.ShowWindow(SW_HIDE);
		staticConfig1.ShowWindow(SW_SHOW);
		staticConfig2.ShowWindow(SW_HIDE);
		staticConfig3.ShowWindow(SW_HIDE);
		staticConfig4.ShowWindow(SW_HIDE);
		staticConfig5.ShowWindow(SW_HIDE);
		/*btnStartStop1.ShowWindow(SW_SHOW);
		btnStartStop2.ShowWindow(SW_HIDE);
		btnStartStop3.ShowWindow(SW_HIDE);
		btnStartStop4.ShowWindow(SW_HIDE);
		btnStartStop5.ShowWindow(SW_HIDE);*/
		pollShown[0] = 1;
		pollShown[1] = 0;
		pollShown[2] = 0;
		pollShown[3] = 0;
		pollShown[4] = 0;
		break;
	}
	if ((pollShown[0] == 1) && (pollConfigured[0] == 1))
	{;}
}

void CMbPoll::OnFileExit()
{
	// TODO: Add your command handler code here
	OnMbPollDestroyWindow();
}

void CMbPoll::OnMbPollDestroyWindow()
{
	noOfGridTables = comboNoOfGrids.GetCurSel();
	DestroyWindow();
}

void CMbPoll::OnClickedButtonConfig1()
{
	// TODO: Add your control notification handler code here
	curConf = 0;
	int ret = mbPollOptions.DoModal();
	if (ret == IDOK)
	{
		updateStaticConfig(0);
		updateDataSharingVariables(0);
	}
}

void CMbPoll::OnClickedButtonConfig2()
{
	// TODO: Add your control notification handler code here
	curConf = 1;
	int ret = mbPollOptions.DoModal();
	if (ret == IDOK)
	{
		updateStaticConfig(1);
		updateDataSharingVariables(1);
	}
}

void CMbPoll::OnClickedButtonConfig3()
{
	// TODO: Add your control notification handler code here
	curConf = 2;
	int ret = mbPollOptions.DoModal();
	if (ret == IDOK)
	{
		updateStaticConfig(2);
		updateDataSharingVariables(2);
	}
}

void CMbPoll::OnClickedButtonConfig4()
{
	// TODO: Add your control notification handler code here
	curConf = 3;
	int ret = mbPollOptions.DoModal();
	if (ret == IDOK)
	{
		updateStaticConfig(3);
		updateDataSharingVariables(3);
	}
}

void CMbPoll::OnClickedButtonConfig5()
{
	// TODO: Add your control notification handler code here
	curConf = 4;
	int ret = mbPollOptions.DoModal();
	if (ret == IDOK)
	{
		updateStaticConfig(4);
		updateDataSharingVariables(4);
	}
}

void CMbPoll::updateStaticConfig(int configNo)
{
	CString tempStrSlaveId;
	CString tempStrFunction;
	CString tempStrAddress;
	CString tempStrQuantity;
	CString tempStr;
	int tempVar;
	
	tempStrSlaveId.Format(_T("%d"), pollSlaveId[configNo]);

	switch(pollFunction[configNo])
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
	tempStrFunction.Format(_T("%02d"), tempVar);
	tempStrAddress.Format(_T("%d"), pollAddress[configNo]);
	tempStrQuantity.Format(_T("%d"), pollQuantity[configNo]);

	tempStr = L"S = " + tempStrSlaveId + L", F = " + tempStrFunction + L"\nA = " + tempStrAddress + L", Q = " + tempStrQuantity;

	switch(configNo)
	{
	case 0:
		staticConfig1.SetWindowText(tempStr);
		if (pollRows[configNo] == 0) ctrlGrid1.put_Rows(pollQuantity[configNo] + 1);
		if (pollRows[configNo] == 10) ctrlGrid1.put_Rows(11);
		if (pollRows[configNo] == 20) ctrlGrid1.put_Rows(21);
		if (pollRows[configNo] == 50) ctrlGrid1.put_Rows(51);
		if (pollRows[configNo] == 100) ctrlGrid1.put_Rows(101);
		break;
	case 1:
		staticConfig2.SetWindowText(tempStr);
		if (pollRows[configNo] == 0) ctrlGrid2.put_Rows(pollQuantity[configNo] + 1);
		if (pollRows[configNo] == 10) ctrlGrid2.put_Rows(11);
		if (pollRows[configNo] == 20) ctrlGrid2.put_Rows(21);
		if (pollRows[configNo] == 50) ctrlGrid2.put_Rows(51);
		if (pollRows[configNo] == 100) ctrlGrid2.put_Rows(101);
		break;
	case 2:
		staticConfig3.SetWindowText(tempStr);
		if (pollRows[configNo] == 0) ctrlGrid3.put_Rows(pollQuantity[configNo] + 1);
		if (pollRows[configNo] == 10) ctrlGrid3.put_Rows(11);
		if (pollRows[configNo] == 20) ctrlGrid3.put_Rows(21);
		if (pollRows[configNo] == 50) ctrlGrid3.put_Rows(51);
		if (pollRows[configNo] == 100) ctrlGrid3.put_Rows(101);
		break;
	case 3:
		staticConfig4.SetWindowText(tempStr);
		if (pollRows[configNo] == 0) ctrlGrid4.put_Rows(pollQuantity[configNo] + 1);
		if (pollRows[configNo] == 10) ctrlGrid4.put_Rows(11);
		if (pollRows[configNo] == 20) ctrlGrid4.put_Rows(21);
		if (pollRows[configNo] == 50) ctrlGrid4.put_Rows(51);
		if (pollRows[configNo] == 100) ctrlGrid4.put_Rows(101);
		break;
	case 4:
		staticConfig5.SetWindowText(tempStr);
		if (pollRows[configNo] == 0) ctrlGrid5.put_Rows(pollQuantity[configNo] + 1);
		if (pollRows[configNo] == 10) ctrlGrid5.put_Rows(11);
		if (pollRows[configNo] == 20) ctrlGrid5.put_Rows(21);
		if (pollRows[configNo] == 50) ctrlGrid5.put_Rows(51);
		if (pollRows[configNo] == 100) ctrlGrid5.put_Rows(101);
		break;
	default:
		break;
	}
}

void CMbPoll::updateDataSharingVariables(int configNo)
{
	switch(configNo)
	{
	case 0:
		if (grid1Data != NULL){delete(grid1Data); grid1Data = NULL;}
		grid1Data = (short int*) malloc (sizeof(short int) * pollQuantity[configNo]);
		putDataInGrid1 = 1;
		break;
	case 1:
		if (grid2Data != NULL){delete(grid2Data); grid2Data = NULL;}
		grid2Data = (short int*) malloc (sizeof(short int) * pollQuantity[configNo]);
		putDataInGrid2 = 1;
		break;
	case 2:
		if (grid3Data != NULL){delete(grid3Data); grid3Data = NULL;}
		grid3Data = (short int*) malloc (sizeof(short int) * pollQuantity[configNo]);
		putDataInGrid3 = 1;
		break;
	case 3:
		if (grid4Data != NULL){delete(grid4Data); grid4Data = NULL;}
		grid4Data = (short int*) malloc (sizeof(short int) * pollQuantity[configNo]);
		putDataInGrid4 = 1;
		break;
	case 4:
		if (grid5Data != NULL){delete(grid5Data); grid5Data = NULL;}
		grid5Data = (short int*) malloc (sizeof(short int) * pollQuantity[configNo]);
		putDataInGrid5 = 1;
		break;
	default:
		break;
	}
	pollConfigured[configNo] = 1;
}

void CMbPoll::OnClickedButtonStartStop1()
{
	// TODO: Add your control notification handler code here
	OnClickedButtonStartStop(ctrlGrid1);
}

void CMbPoll::OnClickedButtonStartStop2()
{
	// TODO: Add your control notification handler code here
	OnClickedButtonStartStop(ctrlGrid2);
}

void CMbPoll::OnClickedButtonStartStop3()
{
	// TODO: Add your control notification handler code here
	OnClickedButtonStartStop(ctrlGrid3);
}

void CMbPoll::OnClickedButtonStartStop4()
{
	// TODO: Add your control notification handler code here
	OnClickedButtonStartStop(ctrlGrid4);
}

void CMbPoll::OnClickedButtonStartStop5()
{
	// TODO: Add your control notification handler code here
	OnClickedButtonStartStop(ctrlGrid5);
}

void CMbPoll::OnClickedButtonStartStop(CMsflexgrid &grid)
{
	// TODO: Add your control notification handler code here
}

void CMbPoll::OnClickedCheckTapData()
{
	// TODO: Add your control notification handler code here
	if (checkTapData.GetCheck() == TRUE)
	{
		tapDataMode = 1;
	}
	else
	{
		tapDataMode = 0;
	}
}


