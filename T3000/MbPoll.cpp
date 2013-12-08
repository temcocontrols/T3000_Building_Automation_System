// MbPoll.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "MbPoll.h"
#include "afxdialogex.h"
#include "MbpExterns.h"
#include "globle_function.h"
#include "TrafficWindow.h"
#include "RegDetails.h"

#define ROWS_OF_MBPOLLLIST 26
#define SHOW_ALL 0

TrafficWindow *trafficWindow = NULL;
// CMbPoll dialog

IMPLEMENT_DYNAMIC(CMbPoll, CDialog)

CMbPoll::CMbPoll(CWnd* pParent /*=NULL*/)
	: CDialog(CMbPoll::IDD, pParent)
{
	mParent = pParent;
	//checkTapData.SetCheck(1);
	strGrid1 = _T("");
	valNoOfGrids = 0;
	m_nCurHeight = 0;
	m_nScrollPos = 0;
	m_nCurWidth = 0;
	m_nHScrollPos = 0;
	radioButtonShow = 0;
}

CMbPoll::~CMbPoll()
{
	if (grid1Data != NULL){delete(grid1Data); grid1Data = NULL;}
	if (grid2Data != NULL){delete(grid2Data); grid2Data = NULL;}
	if (grid3Data != NULL){delete(grid3Data); grid3Data = NULL;}
	if (grid4Data != NULL){delete(grid4Data); grid4Data = NULL;}
	if (grid5Data != NULL){delete(grid5Data); grid5Data = NULL;}
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
	DDX_Control(pDX, IDC_STATIC_RUNNING1, staticRunning1);
	DDX_Control(pDX, IDC_STATIC_RUNNING2, staticRunning2);
	DDX_Control(pDX, IDC_STATIC_RUNNING3, staticRunning3);
	DDX_Control(pDX, IDC_STATIC_RUNNING4, staticRunning4);
	DDX_Control(pDX, IDC_STATIC_RUNNING5, staticRunning5);
	DDX_Control(pDX, IDC_STATIC_CONFIG6, staticConnectionStatus);
	DDX_Control(pDX, IDC_BUTTON_TRAFFIC1, btnShowTraffic1);
	DDX_Control(pDX, IDC_BUTTON_TRAFFIC2, btnShowTraffic2);
	DDX_Control(pDX, IDC_BUTTON_TRAFFIC3, btnShowTraffic3);
	DDX_Control(pDX, IDC_BUTTON_TRAFFIC4, btnShowTraffic4);
	DDX_Control(pDX, IDC_BUTTON_TRAFFIC5, btnShowTraffic5);
	DDX_Control(pDX, IDC_MBPOLL_LIST, mbPollList);
	DDX_Control(pDX, IDC_MBPOLL_CHECK1, mbpollCheck1);
	DDX_Control(pDX, IDC_MBPOLL_CHECK2, mbpollCheck2);
	DDX_Control(pDX, IDC_MBPOLL_CHECK3, mbpollCheck3);
	DDX_Control(pDX, IDC_MBPOLL_CHECK4, mbpollCheck4);
	DDX_Control(pDX, IDC_STATIC_DISPLAYTYPE, staticDisplayType);
}

void CMbPoll::startStopBtnState()
{
	if (pollSingleFunction[0] == 1)
	{
		btnStartStop1.SetWindowText(L"Execute");
		staticRunning1.SetWindowText(L" ");
	}
	else
	{
		(dataFlowStarted[0] == 1) ? (btnStartStop1.SetWindowText(L"Stop")) : (btnStartStop1.SetWindowText(L"Start"));
		(dataFlowStarted[0] == 1) ? (staticRunning1.SetWindowText(L"?")) : (staticRunning1.SetWindowText(L"X"));	
		(dataFlowStarted[0] == 1) ? (btnConfig1.EnableWindow(FALSE)) : (btnConfig1.EnableWindow(TRUE));
	}

#if SHOW_ALL
	if (pollSingleFunction[1] == 1)
	{
		btnStartStop2.SetWindowText(L"Execute");
		staticRunning2.SetWindowText(L" ");
	}
	else
	{
		(dataFlowStarted[1] == 1) ? (btnStartStop2.SetWindowText(L"Stop")) : (btnStartStop2.SetWindowText(L"Start"));
		(dataFlowStarted[1] == 1) ? (staticRunning2.SetWindowText(L"?")) : (staticRunning2.SetWindowText(L"X"));
		(dataFlowStarted[1] == 1) ? (btnConfig2.EnableWindow(FALSE)) : (btnConfig2.EnableWindow(TRUE));
	}
#endif

	if (pollSingleFunction[2] == 1)
	{
		btnStartStop3.SetWindowText(L"Execute");
		staticRunning3.SetWindowText(L" ");
	}
	else
	{
		(dataFlowStarted[2] == 1) ? (btnStartStop3.SetWindowText(L"Stop")) : (btnStartStop3.SetWindowText(L"Start"));
		(dataFlowStarted[2] == 1) ? (staticRunning3.SetWindowText(L"?")) : (staticRunning3.SetWindowText(L"X"));
#if SHOW_ALL
		(dataFlowStarted[2] == 1) ? (btnConfig3.EnableWindow(FALSE)) : (btnConfig3.EnableWindow(TRUE));
#endif
	}

	if (pollSingleFunction[3] == 1)
	{
		btnStartStop4.SetWindowText(L"Execute");
		staticRunning4.SetWindowText(L" ");
	}
	else
	{
		(dataFlowStarted[3] == 1) ? (btnStartStop4.SetWindowText(L"Stop")) : (btnStartStop4.SetWindowText(L"Start"));
		(dataFlowStarted[3] == 1) ? (staticRunning4.SetWindowText(L"?")) : (staticRunning4.SetWindowText(L"X"));
#if SHOW_ALL
		(dataFlowStarted[3] == 1) ? (btnConfig4.EnableWindow(FALSE)) : (btnConfig4.EnableWindow(TRUE));
#endif
	}

	if (pollSingleFunction[4] == 1)
	{
		btnStartStop5.SetWindowText(L"Execute");
		staticRunning5.SetWindowText(L" ");
	}
	else
	{
		(dataFlowStarted[4] == 1) ? (btnStartStop5.SetWindowText(L"Stop")) : (btnStartStop5.SetWindowText(L"Start"));
		(dataFlowStarted[4] == 1) ? (staticRunning5.SetWindowText(L"?")) : (staticRunning5.SetWindowText(L"X"));
#if SHOW_ALL
		(dataFlowStarted[4] == 1) ? (btnConfig5.EnableWindow(FALSE)) : (btnConfig5.EnableWindow(TRUE));
#endif
	}

	(tapDataMode == 1) ? checkTapData.SetCheck(1) : checkTapData.SetCheck(0);
}

void CMbPoll::restoreConfigs()
{
	if (pollConfigured[0] == 1)
	{
		updateStaticConfig(0);
		updateDataSharingVariables(0);
	}
	if (pollConfigured[1] == 1)
	{
		updateStaticConfig(1);
		updateDataSharingVariables(1);
	}
	if (pollConfigured[2] == 1)
	{
		updateStaticConfig(2);
		updateDataSharingVariables(2);
	}
	if (pollConfigured[3] == 1)
	{
		updateStaticConfig(3);
		updateDataSharingVariables(3);
	}
	if (pollConfigured[4] == 1)
	{
		updateStaticConfig(4);
		updateDataSharingVariables(4);
	}
}

BOOL CMbPoll::OnInitDialog()
{
	CDialog::OnInitDialog();

	MbPollOpen = 1;
	ctrlEditGrid1.ShowWindow(SW_HIDE);
	comboNoOfGrids.SetCurSel(noOfGridTables);
	//displayGrids(noOfGridTables);
	displayGrids(5);
	startStopBtnState();
	restoreConfigs();

	if (mbPollFirstTime == 1)
	{
		OnFileOpenWhileOpen();
		mbPollFirstTime = 0;
	}

	ctrlGrid1.put_TextMatrix(0, 0, L"Alias");
	ctrlGrid1.put_TextMatrix(0, 1, L"Addr");
	ctrlGrid1.put_TextMatrix(0, 2, L"Value");
	ctrlGrid1.put_ColWidth(1, 500);
	ctrlGrid1.put_ColWidth(2, 1400);
	
	ctrlGrid2.put_TextMatrix(0, 0, L"Alias");
	ctrlGrid2.put_TextMatrix(0, 1, L"Addr");
	ctrlGrid2.put_TextMatrix(0, 2, L"Value");
	ctrlGrid2.put_ColWidth(1, 500);
	ctrlGrid2.put_ColWidth(2, 1400);

	ctrlGrid3.put_TextMatrix(0, 0, L"Alias");
	ctrlGrid3.put_TextMatrix(0, 1, L"Addr");
	ctrlGrid3.put_TextMatrix(0, 2, L"Value");
	ctrlGrid3.put_ColWidth(1, 500);
	ctrlGrid3.put_ColWidth(2, 1400);

	ctrlGrid4.put_TextMatrix(0, 0, L"Alias");
	ctrlGrid4.put_TextMatrix(0, 1, L"Addr");
	ctrlGrid4.put_TextMatrix(0, 2, L"Value");
	ctrlGrid4.put_ColWidth(1, 500);
	ctrlGrid4.put_ColWidth(2, 1400);

	ctrlGrid5.put_TextMatrix(0, 0, L"Alias");
	ctrlGrid5.put_TextMatrix(0, 1, L"Addr");
	ctrlGrid5.put_TextMatrix(0, 2, L"Value");
	ctrlGrid5.put_ColWidth(1, 500);
	ctrlGrid5.put_ColWidth(2, 1400);

	restoreRegNames();

	SetTimer(1, 2000, NULL);
	SetTimer(2, 500, NULL);
	SetTimer(3, 50, NULL);

	mbpMenu = GetMenu();
	firstTime1 = 1;
	firstTime2 = 1;
	firstTime3 = 1;
	firstTime4 = 1;
	firstTime5 = 1;

#if 0
	connectionSuccessful = 1;	/* to be deleted */
#endif

	InitListCtrl();
	mbPollList.GetWindowRect(mbPollListRect);

	restoreRadioButtonState();

	GetWindowRect(m_rect);
	m_nScrollPos = 0;
	m_nHScrollPos = 0;

	hMbpollWnd = this->m_hWnd;

	displayThreadSemaphore = CreateSemaphore(NULL, 0, 10, NULL);
	hDispThread = CreateThread(NULL, NULL, displayThreadStarter, this, 0/*CREATE_SUSPENDED*/, &hDispThreadID);
	mbCommThreadSemaphore = CreateSemaphore(NULL, 0, 10, NULL);
	hMbCommThread = CreateThread(NULL, NULL, mbCommThreadStarter, this, 0/*CREATE_SUSPENDED*/, &hMbCommThreadID);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
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
	ON_COMMAND(ID_FILE_SAVE32884, &CMbPoll::OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, &CMbPoll::OnFileOpen)
	ON_BN_CLICKED(IDC_BUTTON_TRAFFIC1, &CMbPoll::OnClickedButtonTraffic1)
	ON_BN_CLICKED(IDC_BUTTON_TRAFFIC2, &CMbPoll::OnClickedButtonTraffic2)
	ON_BN_CLICKED(IDC_BUTTON_TRAFFIC3, &CMbPoll::OnClickedButtonTraffic3)
	ON_BN_CLICKED(IDC_BUTTON_TRAFFIC4, &CMbPoll::OnClickedButtonTraffic4)
	ON_BN_CLICKED(IDC_BUTTON_TRAFFIC5, &CMbPoll::OnClickedButtonTraffic5)
	ON_MESSAGE(WM_TRAFFIC_CLOSED, &CMbPoll::OnTrafficClosed)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_MBPOLL_CHECK1, &CMbPoll::OnClickedMbpollCheck1)
	ON_BN_CLICKED(IDC_MBPOLL_CHECK2, &CMbPoll::OnClickedMbpollCheck2)
	ON_BN_CLICKED(IDC_MBPOLL_CHECK3, &CMbPoll::OnClickedMbpollCheck3)
	ON_BN_CLICKED(IDC_MBPOLL_CHECK4, &CMbPoll::OnClickedMbpollCheck4)
	ON_MESSAGE(MY_MBPOLL_REG_DIALOG_MSG, &CMbPoll::OnMyMbpollRegDialogMsg)
	ON_COMMAND(ID_DISPLAYTYPE_SIGNED, &CMbPoll::OnDisplaytypeSigned)
	ON_COMMAND(ID_DISPLAYTYPE_UNSIGNED, &CMbPoll::OnDisplaytypeUnsigned)
	ON_COMMAND(ID_DISPLAYTYPE_HEX, &CMbPoll::OnDisplaytypeHex)
	ON_COMMAND(ID_DISPLAYTYPE_BINARY, &CMbPoll::OnDisplaytypeBinary)
	ON_UPDATE_COMMAND_UI(ID_DISPLAYTYPE_SIGNED, &CMbPoll::OnUpdateDisplaytypeSigned)
	ON_UPDATE_COMMAND_UI(ID_DISPLAYTYPE_UNSIGNED, &CMbPoll::OnUpdateDisplaytypeUnsigned)
	ON_UPDATE_COMMAND_UI(ID_DISPLAYTYPE_HEX, &CMbPoll::OnUpdateDisplaytypeHex)
	ON_UPDATE_COMMAND_UI(ID_DISPLAYTYPE_BINARY, &CMbPoll::OnUpdateDisplaytypeBinary)
	ON_COMMAND(ID_FUNCTIONS_WRITEREGISTERS, &CMbPoll::OnFunctionsWriteregisters)
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

void CMbPoll::InitListCtrl()
{
	long style;
	style=GetWindowLong(mbPollList.m_hWnd,GWL_STYLE);
	style&=~LVS_TYPEMASK;
	style|=LVS_REPORT;	
//	style|=LVS_EX_CHECKBOXES;

	mbPollList.SetExtendedStyle(style);
	SetWindowLong(mbPollList.m_hWnd,GWL_STYLE,style);
	DWORD dwstyle=mbPollList.GetExtendedStyle();
	dwstyle|=LVS_EX_FULLROWSELECT; 
	dwstyle|=LVS_EX_GRIDLINES;
	mbPollList.SetExtendedStyle(dwstyle);
	mbPollList.InsertColumn(0,_T(""),LVCFMT_LEFT,120);  
	mbPollList.InsertColumn(1,_T(""),LVCFMT_LEFT,50);
	mbPollList.InsertColumn(2,_T(""),LVCFMT_LEFT,120);
	mbPollList.InsertColumn(3,_T(""),LVCFMT_LEFT,120);
	mbPollList.InsertColumn(4,_T(""),LVCFMT_LEFT,50);
	mbPollList.InsertColumn(5,_T(" "),LVCFMT_LEFT,120);
	mbPollList.InsertColumn(6,_T(""),LVCFMT_LEFT,120);
	mbPollList.InsertColumn(7,_T(""),LVCFMT_LEFT,50);
	mbPollList.InsertColumn(8,_T(" "),LVCFMT_LEFT,120);
	mbPollList.InsertColumn(9,_T(""),LVCFMT_LEFT,120);
	mbPollList.InsertColumn(10,_T(""),LVCFMT_LEFT,50);
	mbPollList.InsertColumn(11,_T(" "),LVCFMT_LEFT,120);
	mbPollList.InsertColumn(12,_T(""),LVCFMT_LEFT,120);
	mbPollList.InsertColumn(13,_T(""),LVCFMT_LEFT,50);
	mbPollList.InsertColumn(14,_T(" "),LVCFMT_LEFT,120);

	//SurpportAutoInstall
	//m_list_control.InsertColumn(5,_T("444"),LVCFMT_CENTER,100);
	mbPollList.SetTextColor(RGB(0,0,255));

	//m_list_control.SortItems(0, FALSE); // sort the 1st column, ascending
	mbPollList.SetSortable(FALSE);

	mbPollList.SetGridLines(0);

	mbPollList.SetHeaderText(0,_T("ALIAS"));
	mbPollList.SetHeaderText(1,_T("ADDR"));
	mbPollList.SetHeaderText(2,_T("VALUE"));
	mbPollList.SetHeaderText(3,_T("ALIAS"));
	mbPollList.SetHeaderText(4,_T("ADDR"));
	mbPollList.SetHeaderText(5,_T("VALUE"));
	mbPollList.SetHeaderText(6,_T("ALIAS"));
	mbPollList.SetHeaderText(7,_T("ADDR"));
	mbPollList.SetHeaderText(8,_T("VALUE"));
	mbPollList.SetHeaderText(9,_T("ALIAS"));
	mbPollList.SetHeaderText(10,_T("ADDR"));
	mbPollList.SetHeaderText(11,_T("VALUE"));
	mbPollList.SetHeaderText(12,_T("ALIAS"));
	mbPollList.SetHeaderText(13,_T("ADDR"));
	mbPollList.SetHeaderText(14,_T("VALUE"));

	CString tempcs;
	for (int i = 0; i < ROWS_OF_MBPOLLLIST; i++)
	{
		tempcs.Format(_T(""),i);
		mbPollList.InsertItem(i,tempcs);
	}

	if (runPoll[0] == 1) restoreRegisterNames(0);
	if (runPoll[1] == 1) restoreRegisterNames(1);
	if (runPoll[2] == 1) restoreRegisterNames(2);
	if (runPoll[3] == 1) restoreRegisterNames(3);

	//mbPollList.SetItemText(4, 2, _T("Jay"));	// (row, col, value)

}

BOOL CMbPoll::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(nIDTemplate, pParentWnd);
}

void CMbPoll::OnBnClickedClose()
{
	// TODO: Add your control notification handler code here

	if (dataFlowStarted[0] == 1)
	{
		OnClickedButtonStartStop(ctrlGrid1, 0, btnStartStop1, btnConfig1);
	}
#if SHOW_ALL
	if (dataFlowStarted[1] == 1)
	{
		OnClickedButtonStartStop(ctrlGrid2, 1, btnStartStop2, btnConfig2);
	}
	if (dataFlowStarted[2] == 1)
	{
		OnClickedButtonStartStop(ctrlGrid3, 2, btnStartStop3, btnConfig3);
	}
	if (dataFlowStarted[3] == 1)
	{
		OnClickedButtonStartStop(ctrlGrid4, 3, btnStartStop4, btnConfig4);
	}
	if (dataFlowStarted[4] == 1)
	{
		OnClickedButtonStartStop(ctrlGrid5, 4, btnStartStop5, btnConfig5);
	}
#endif

	KillTimer(1);
	KillTimer(2);
	KillTimer(3);

	saveRegNames();
	saveRadioButtonState();
	CDialog::OnCancel();
	OnMbPollDestroyWindow();	
}

void CMbPoll::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	static unsigned int count = 0;
	static unsigned int a, b, c, d, e = 0;
	CString tempStr;
	CString convertedStr;
	static int qtyToShow[5] = {0};

	//tempStr.Format(_T("%d %d %d %d %d %d"),count, a, b, c, d ,e);
	tempStr.Format(_T("%d"),temporaryCount);

	ctrlLabel1.SetWindowTextW(tempStr);

	switch(nIDEvent)
	{
	case 1:	// 2s
		{
			//ReleaseSemaphore(ghSemaphore, 1, NULL);
			break;
		}
	case 2: // 500 ms
#if 1
		{
			ReleaseSemaphore(displayThreadSemaphore, 1, NULL);
#if 0
			if (connectionSuccessful == 1)
			{
				staticConnectionStatus.ShowWindow(FALSE);
			}
			else
			{
				staticConnectionStatus.ShowWindow(TRUE);
			}

			CString tempStr1;
			if (pollSingleFunction[0] != 1)	staticRunning1.SetWindowText(L"X");
			if (pollSingleFunction[1] != 1)	staticRunning2.SetWindowText(L"X");
			if (pollSingleFunction[2] != 1)	staticRunning3.SetWindowText(L"X");
			if (pollSingleFunction[3] != 1)	staticRunning4.SetWindowText(L"X");
			if (pollSingleFunction[4] != 1)	staticRunning5.SetWindowText(L"X");

			/*	update grid if, 
				config has been done (grid1Data != NULL), 
				Start button has been clicked (dataFlowStarted[0] == 1), 
				its not a write function where grid update is not required (pollFunction[0] <= 3)	*/
			if (((grid1Data != NULL) && (dataFlowStarted[0] == 1)) || (executeOnce[0] == 2))
			{
				if (pollSingleFunction[0] != 1)	staticRunning1.SetWindowText(L"?");
				if (executeOnce[0] == 2) executeOnce[0] = 0;
				if (putDataNow[0] == 1)
				{
					qtyToShow[0] = ((pollRows[0] == 0) || (pollQuantity[0] < pollRows[0])) ? pollQuantity[0] : pollRows[0];
					putDataNow[0] = 0;
				}
				for (int i = 0; i < qtyToShow[0]; i++)
				{
					tempStr1.Format(_T("%d"), (pollAddress[0] + i)); 
					ctrlGrid1.put_TextMatrix(i + 1, 1, tempStr1);
					updateStaticConfig(0, 1);
					if ((!((tapDataMode == 0) && (pollFunction[0] >= 4))) && (connectionSuccessful == 1))
					{
						//tempStr1.Format(_T("%d"), *((short int*)grid1Data + i)); 
						convertedStr = convertFunction(pollDisplay[0], (unsigned short)(*((short int*)grid1Data + i)));
						ctrlGrid1.put_TextMatrix(i + 1, 2, convertedStr);
					}
				}
			}

			if (((grid2Data != NULL)  && (dataFlowStarted[1] == 1)) || (executeOnce[1] == 2))
			{
				if (pollSingleFunction[1] != 1)	staticRunning2.SetWindowText(L"?");
				if (executeOnce[1] == 2) executeOnce[1] = 0;
				if (putDataNow[1] == 1)
				{
					qtyToShow[1] = ((pollRows[1] == 0) || (pollQuantity[1] < pollRows[1])) ? pollQuantity[1] : pollRows[1];
					putDataNow[1] = 0;
				}
				for (int i = 0; i < qtyToShow[1]; i++)
				{
					tempStr1.Format(_T("%d"), (pollAddress[1] + i)); 
					ctrlGrid2.put_TextMatrix(i + 1, 1, tempStr1);
					updateStaticConfig(1, 1);
					if ((!((tapDataMode == 0) && (pollFunction[1] >= 4))) && (connectionSuccessful == 1))
					{
						//tempStr1.Format(_T("%d"), *((short int*)grid2Data + i)); 
						convertedStr = convertFunction(pollDisplay[1], (unsigned short)(*((short int*)grid2Data + i)));
						ctrlGrid2.put_TextMatrix(i + 1, 2, convertedStr);
					}
				}
			}

			if (((grid3Data != NULL) && (dataFlowStarted[2] == 1)) || (executeOnce[2] == 2))
			{
				if (pollSingleFunction[2] != 1)	staticRunning3.SetWindowText(L"?");
				if (executeOnce[2] == 2) executeOnce[2] = 0;
				if (putDataNow[2] == 1)
				{
					qtyToShow[2] = ((pollRows[2] == 0) || (pollQuantity[2] < pollRows[2])) ? pollQuantity[2] : pollRows[2];
					putDataNow[2] = 0;
				}
				for (int i = 0; i < qtyToShow[2]; i++)
				{
					tempStr1.Format(_T("%d"), (pollAddress[2] + i)); 
					ctrlGrid3.put_TextMatrix(i + 1, 1, tempStr1);
					updateStaticConfig(2, 1);
					if ((!((tapDataMode == 0) && (pollFunction[2] >= 4))) && (connectionSuccessful == 1))
					{
						//tempStr1.Format(_T("%d"), *((short int*)grid3Data + i)); 
						convertedStr = convertFunction(pollDisplay[2], (unsigned short)(*((short int*)grid3Data + i)));
						ctrlGrid3.put_TextMatrix(i + 1, 2, convertedStr);
					}
				}
			}

			if (((grid4Data != NULL) && (dataFlowStarted[3] == 1)) || (executeOnce[3] == 2))
			{
				if (pollSingleFunction[3] != 1)	staticRunning4.SetWindowText(L"?");
				if (executeOnce[3] == 2) executeOnce[3] = 0;
				if (putDataNow[3] == 1)
				{
					qtyToShow[3] = ((pollRows[3] == 0) || (pollQuantity[3] < pollRows[3])) ? pollQuantity[3] : pollRows[3];
					putDataNow[3] = 0;
				}
				for (int i = 0; i < qtyToShow[3]; i++)
				{
					tempStr1.Format(_T("%d"), (pollAddress[3] + i)); 
					ctrlGrid4.put_TextMatrix(i + 1, 1, tempStr1);
					updateStaticConfig(3, 1);
					if ((!((tapDataMode == 0) && (pollFunction[3] >= 4))) && (connectionSuccessful == 1))
					{
						//tempStr1.Format(_T("%d"), *((short int*)grid4Data + i)); 
						convertedStr = convertFunction(pollDisplay[3], (unsigned short)(*((short int*)grid4Data + i)));
						ctrlGrid4.put_TextMatrix(i + 1, 2, convertedStr);
					}
				}
			}

			if (((grid5Data != NULL) && (dataFlowStarted[4] == 1)) || (executeOnce[4] == 2))
			{
				if (pollSingleFunction[4] != 1)	staticRunning5.SetWindowText(L"?");
				if (executeOnce[4] == 2) executeOnce[4] = 0;
				if (putDataNow[4] == 1)
				{
					qtyToShow[4] = ((pollRows[4] == 0) || (pollQuantity[4] < pollRows[4])) ? pollQuantity[4] : pollRows[4];
					putDataNow[4] = 0;
				}
				for (int i = 0; i < qtyToShow[4]; i++)
				{
					tempStr1.Format(_T("%d"), (pollAddress[4] + i)); 
					ctrlGrid5.put_TextMatrix(i + 1, 1, tempStr1);
					updateStaticConfig(4, 1);
					if ((!((tapDataMode == 0) && (pollFunction[4] >= 4))) && (connectionSuccessful == 1))
					{
						//tempStr1.Format(_T("%d"), *((short int*)grid5Data + i)); 
						convertedStr = convertFunction(pollDisplay[4], (unsigned short)(*((short int*)grid5Data + i)));
						ctrlGrid5.put_TextMatrix(i + 1, 2, convertedStr);
					}
				}
			}
#endif
			break;
		}
#endif
	case 3:
		{
			ReleaseSemaphore(mbCommThreadSemaphore, 1, NULL);
#if 0
			count++;
			if (dataFlowStarted[0] == 1)
			{
				if ((count % (pollScanRate[0]/50)) == 0)	
				{
					callMbFunc(0);
					a++;
				}
			}
			if (dataFlowStarted[1] == 1)
			{
				if ((count % (pollScanRate[1]/50)) == 0)	
				{
					callMbFunc(1);
					b++;
				}
			}
			if (dataFlowStarted[2] == 1)
			{
				if ((count % (pollScanRate[2]/50)) == 0)	
				{
					callMbFunc(2);
					c++;
				}
			}
			if (dataFlowStarted[3] == 1)
			{
				if ((count % (pollScanRate[3]/50)) == 0)	
				{
					callMbFunc(3);
					d++;
				}
			}
			if (dataFlowStarted[4] == 1)
			{
				if ((count % (pollScanRate[4]/50)) == 0)	
				{
					callMbFunc(4);
					e++;
				}
			}

			if (executeOnce[0] == 1)
			{
				callMbFunc(0);
				executeOnce[0] = 2;
			}
			if (executeOnce[1] == 1)
			{
				callMbFunc(1);
				executeOnce[1] = 2;
			}
			if (executeOnce[2] == 1)
			{
				callMbFunc(2);
				executeOnce[2] = 2;
			}
			if (executeOnce[3] == 1)
			{
				callMbFunc(3);
				executeOnce[3] = 2;
			}
			if (executeOnce[4] == 1)
			{
				callMbFunc(4);
				executeOnce[4] = 2;
			}
#endif
			//MessageBox(tempStr);
			break;
		}
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CMbPoll::callMbFunc(int slotNo)
{
	CString tempStr1;
	CMsflexgrid* grid;

	if (slotNo == 0) grid = &ctrlGrid1;
	if (slotNo == 1) grid = &ctrlGrid2;
	if (slotNo == 2) grid = &ctrlGrid3;
	if (slotNo == 3) grid = &ctrlGrid4;
	if (slotNo == 4) grid = &ctrlGrid5;

	if (((dataFlowStarted[slotNo] == 1) && (tapDataMode == 0)) || (executeOnce[slotNo] == 1))
	{
		//Read_One11(unsigned char device_var,unsigned short address);
		if (pollFunction[slotNo] == 0){}		// 01 Read Coils (0x)
		if (pollFunction[slotNo] == 1){}		// 02 Read Discrete Inputs (1x)
		if (pollFunction[slotNo] == 2)		// * 03 Read Holding Registers (4x) (Read_One11, read_multi11)
		{
			if (pollQuantity[slotNo] == 1)
			{
				unsigned short tempVar;
				tempVar = Read_One_t(pollSlaveId[slotNo], pollAddress[slotNo], slotNo);
			}
			else
			{
				unsigned short tempVar[1000] = {0};
				unsigned short *tempPtr = tempVar;
				read_multi_t(pollSlaveId[slotNo], tempPtr, pollAddress[slotNo], pollQuantity[slotNo], slotNo);
			}	
		}
		if (pollFunction[slotNo] == 3){}		// 04 Read Input Registers (3x)
		if (pollFunction[slotNo] == 4){}		// 05 Write Single Coil
		if (pollFunction[slotNo] == 5)			// * 06 Write Single Register (Write_One11)
		{
			if (pollQuantity[slotNo] == 1)
			{
				unsigned short tempVar;
				tempStr1 = grid->get_TextMatrix(1, 2);
				tempVar = _wtoi(tempStr1);
				Write_One_t(pollSlaveId[slotNo], pollAddress[slotNo], tempVar, slotNo);
			}
		}
		if (pollFunction[slotNo] == 6){}		// 15 Write Multiple Coils
		if (pollFunction[slotNo] == 7)			// * 16 Write Multiple Registers (write_multi11)
		{
			unsigned short tempVar[1000] = {0};
			unsigned short *tempPtr = tempVar;
			if (pollQuantity[slotNo] != 0)
			{
				for (int i = 0; i < pollQuantity[slotNo]; i++)
				{
					tempStr1 = grid->get_TextMatrix(i + 1, 2);
					tempVar[i] = _wtoi(tempStr1);
				}
			}
			write_multi_t(pollSlaveId[slotNo], (unsigned char*)(&tempVar[0]), pollAddress[slotNo], pollQuantity[slotNo] * 2, slotNo);
		}
	}
	if ((dataFlowStarted[slotNo] == 1) && (tapDataMode == 1))
	{
		//Read_One11(unsigned char device_var,unsigned short address);
		if (pollFunction[slotNo] == 0){}		// 01 Read Coils (0x)
		if (pollFunction[slotNo] == 1){}		// 02 Read Discrete Inputs (1x)
		if (pollFunction[slotNo] == 2){}		// * 03 Read Holding Registers (4x) (Read_One11, read_multi11)
		if (pollFunction[slotNo] == 3){}		// 04 Read Input Registers (3x)
		if (pollFunction[slotNo] == 4){}		// 05 Write Single Coil
		if (pollFunction[slotNo] == 5){ /* no code required */ }		// * 06 Write Single Register (Write_One11)
		if (pollFunction[slotNo] == 6){}		// 15 Write Multiple Coils
		if (pollFunction[slotNo] == 7){ /* no code required */ }		// * 16 Write Multiple Registers (write_multi11)
	}
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

void CMbPoll::saveRegNames()
{
#if 0
	int mallocQty[5] = {0};
	CMsflexgrid* grid;

	for (int i = 0; i < 5; i++)
	{
		if (i == 0) grid = &ctrlGrid1;
		if (i == 1) grid = &ctrlGrid2;
		if (i == 2) grid = &ctrlGrid3;
		if (i == 3) grid = &ctrlGrid4;
		if (i == 4) grid = &ctrlGrid5;

		regNames[i] = NULL;

		if (pollRows[i] == 0) 
		{
			mallocQty[i] = pollQuantity[i];
		}
		else 
		{
			mallocQty[i] = pollRows[i];
		}

		regNames[i] = (CString*)malloc(mallocQty[i] * sizeof(CString));

		for (int j = 0; j < mallocQty[i]; j++)
		{
			*(regNames[i] + j) = grid->get_TextMatrix(j + 1, 0);
		}

	}
#endif

	int mallocQty[5] = {0};
	CMsflexgrid* grid;

	regNames = L"";

	for (int i = 0; i < 5; i++)
	{
		if (i == 0) grid = &ctrlGrid1;
		if (i == 1) grid = &ctrlGrid2;
		if (i == 2) grid = &ctrlGrid3;
		if (i == 3) grid = &ctrlGrid4;
		if (i == 4) grid = &ctrlGrid5;

		if (pollRows[i] == 0) 
		{
			mallocQty[i] = pollQuantity[i];
		}
		else 
		{
			mallocQty[i] = pollRows[i];
		}

		for (int j = 0; j < mallocQty[i]; j++)
		{
			regNames = regNames + grid->get_TextMatrix(j + 1, 0) + L";";
		}
	}
}

void CMbPoll::restoreRegNames()
{
#if 0
	int mallocQty[5] = {0};
	CMsflexgrid* grid;

	for (int i = 0; i < 5; i++)
	{
		if (regNames[i] == NULL)
		{
			continue;
		}
		
		if (i == 0) grid = &ctrlGrid1;
		if (i == 1) grid = &ctrlGrid2;
		if (i == 2) grid = &ctrlGrid3;
		if (i == 3) grid = &ctrlGrid4;
		if (i == 4) grid = &ctrlGrid5;

		if (pollRows[i] == 0) 
		{
			mallocQty[i] = pollQuantity[i];
		}
		else 
		{
			mallocQty[i] = pollRows[i];
		}

		for (int j = 0; j < mallocQty[i]; j++)
		{
			grid->put_TextMatrix(j + 1, 0, *(regNames[i] + j));
		}
		regNames[i] = NULL;
	}
#endif

	int position;
	CString temp1, temp2;
	CMsflexgrid* grid;
	int gridNo = 0;
	int mallocQty[5] = {0};
	int rowNo = 0;

	//MessageBox(regNames);
	position = regNames.Find(L";");
	
	for (int i = 0; i < 5; i++)
	{
		if (pollRows[i] == 0) 
		{
			mallocQty[i] = pollQuantity[i];
		}
		else 
		{
			mallocQty[i] = pollRows[i];
		}
	}

	while (1)
	{
		position = regNames.Find(L";");
		if (position == -1) break;
		temp1 = regNames.Left(position);
		//MessageBox(temp1);

		if (position != 0)
		{
			regNames = regNames.Right(regNames.GetLength() - position - 1);
		}
		else
		{
			regNames = regNames.Right(regNames.GetLength() - 1);
		}
		//MessageBox(regNames);

		if (gridNo == 0) grid = &ctrlGrid1;
		if (gridNo == 1) grid = &ctrlGrid2;
		if (gridNo == 2) grid = &ctrlGrid3;
		if (gridNo == 3) grid = &ctrlGrid4;
		if (gridNo == 4) grid = &ctrlGrid5;

		temp2.Format(_T("gridNo = %d rowNo = %d"), gridNo, rowNo);
		//MessageBox(temp2);

		grid->put_TextMatrix(rowNo + 1, 0, temp1);
		rowNo++;
		if (rowNo == mallocQty[gridNo])
		{
			gridNo++;
			rowNo = 0;
		}

		temp2.Format(_T("gridNo = %d rowNo = %d"), gridNo, rowNo);
		if (gridNo == 5) break;

	}
}

void CMbPoll::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialog::OnOK();
}

void CMbPoll::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	saveRegNames();
	CDialog::OnCancel();
	OnMbPollDestroyWindow();
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
	case 5:
		{
		btnConfig1.ShowWindow(SW_SHOW);
#if SHOW_ALL
		btnConfig2.ShowWindow(SW_SHOW);
		btnConfig3.ShowWindow(SW_SHOW);
		btnConfig4.ShowWindow(SW_SHOW);
		//btnConfig5.ShowWindow(SW_SHOW);
#endif
		//ctrlGrid1.ShowWindow(SW_SHOW);
#if SHOW_ALL
		ctrlGrid2.ShowWindow(SW_SHOW);
		ctrlGrid3.ShowWindow(SW_SHOW);
		ctrlGrid4.ShowWindow(SW_SHOW);
		//ctrlGrid5.ShowWindow(SW_SHOW);
#endif
		staticConfig1.ShowWindow(SW_SHOW);
#if SHOW_ALL
		staticConfig2.ShowWindow(SW_SHOW);
		staticConfig3.ShowWindow(SW_SHOW);
		staticConfig4.ShowWindow(SW_SHOW);
		//staticConfig5.ShowWindow(SW_SHOW);
#endif
		btnStartStop1.ShowWindow(SW_SHOW);
#if SHOW_ALL
		btnStartStop2.ShowWindow(SW_SHOW);
		btnStartStop3.ShowWindow(SW_SHOW);
		btnStartStop4.ShowWindow(SW_SHOW);
		//btnStartStop5.ShowWindow(SW_SHOW);
#endif
		pollShown[0] = 1;
		pollShown[1] = 1;
		pollShown[2] = 1;
		pollShown[3] = 1;
		pollShown[4] = 1;
		putDataNow[0] = 1;
		putDataInGrid[0] = 1;
		putDataNow[1] = 1;
		putDataInGrid[1] = 1;
		putDataNow[2] = 1;
		putDataInGrid[2] = 1;
		putDataNow[3] = 1;
		putDataInGrid[3] = 1;
		putDataNow[4] = 1;
		putDataInGrid[4] = 1;
		break;
		}
	default:
		{
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
		btnStartStop1.ShowWindow(SW_SHOW);
		btnStartStop2.ShowWindow(SW_HIDE);
		btnStartStop3.ShowWindow(SW_HIDE);
		btnStartStop4.ShowWindow(SW_HIDE);
		btnStartStop5.ShowWindow(SW_HIDE);
		pollShown[0] = 1;
		pollShown[1] = 0;
		pollShown[2] = 0;
		pollShown[3] = 0;
		pollShown[4] = 0;
		putDataNow[0] = 1;
		putDataInGrid[0] = 1;
		pollConfigured[0] = 1;
		putDataNow[1] = 0;
		putDataInGrid[1] = 0;
		pollConfigured[1] = 0;
		putDataNow[2] = 0;
		putDataInGrid[2] = 0;
		pollConfigured[2] = 0;
		putDataNow[3] = 0;
		putDataInGrid[3] = 0;
		pollConfigured[3] = 0;
		putDataNow[4] = 0;
		putDataInGrid[4] = 0;
		break;
		}
	}
}

void CMbPoll::OnFileExit()
{
	// TODO: Add your command handler code here
	saveRegNames();
	OnMbPollDestroyWindow();
}

void CMbPoll::OnMbPollDestroyWindow()
{
	//noOfGridTables = comboNoOfGrids.GetCurSel();
	OnFileSaveWhileClose();
	MbPollOpen = 0;
	TerminateThread(hDispThread, 0);
	TerminateThread(hMbCommThread, 0);

	DestroyWindow();
}

void CMbPoll::OnClickedButtonConfig1()
{
	// TODO: Add your control notification handler code here
	curConf = 0;
	int alterDataFlow = 0;
	if (dataFlowStarted[0] == 1)
	{
		dataFlowStarted[0] = 0;
		alterDataFlow = 1;
	}
	int ret = mbPollOptions.DoModal();
	if (ret == IDOK)
	{
		pollConfigured[0] = 1;
		mbpollTotalCount[0] = 0;
		mbpollErrCount[0] = 0;
		updateStaticConfig(0);
		updateBtnLabel(0);
		updateDataSharingVariables(0);
	}
	if (alterDataFlow == 1)
	{
		dataFlowStarted[0] = 1;
	}
}

#if 1
void CMbPoll::OnClickedButtonConfig2()
{
	// TODO: Add your control notification handler code here
	curConf = 1;
	int alterDataFlow = 0;
	if (dataFlowStarted[1] == 1)
	{
		dataFlowStarted[1] = 0;
		alterDataFlow = 1;
	}
	int ret = mbPollOptions.DoModal();
	if (ret == IDOK)
	{
		pollConfigured[1] = 1;
		mbpollTotalCount[1] = 0;
		mbpollErrCount[1] = 0;
		updateStaticConfig(1);
		updateBtnLabel(1);
		updateDataSharingVariables(1);
	}
	if (alterDataFlow == 1)
	{
		dataFlowStarted[1] = 1;
	}
}

void CMbPoll::OnClickedButtonConfig3()
{
	// TODO: Add your control notification handler code here
	curConf = 2;
	int alterDataFlow = 0;
	if (dataFlowStarted[2] == 1)
	{
		dataFlowStarted[2] = 0;
		alterDataFlow = 1;
	}
	int ret = mbPollOptions.DoModal();
	if (ret == IDOK)
	{
		pollConfigured[2] = 1;
		mbpollTotalCount[2] = 0;
		mbpollErrCount[2] = 0;
		updateStaticConfig(2);
		updateBtnLabel(2);
		updateDataSharingVariables(2);
	}
	if (alterDataFlow == 1)
	{
		dataFlowStarted[2] = 1;
	}
}

void CMbPoll::OnClickedButtonConfig4()
{
	// TODO: Add your control notification handler code here
	curConf = 3;
	int alterDataFlow = 0;
	if (dataFlowStarted[3] == 1)
	{
		dataFlowStarted[3] = 0;
		alterDataFlow = 1;
	}
	int ret = mbPollOptions.DoModal();
	if (ret == IDOK)
	{
		pollConfigured[3] = 1;
		mbpollTotalCount[3] = 0;
		mbpollErrCount[3] = 0;
		updateStaticConfig(3);
		updateBtnLabel(3);
		updateDataSharingVariables(3);
	}
	if (alterDataFlow == 1)
	{
		dataFlowStarted[3] = 1;
	}
}

void CMbPoll::OnClickedButtonConfig5()
{
	// TODO: Add your control notification handler code here
	curConf = 4;
	int alterDataFlow = 0;
	if (dataFlowStarted[4] == 1)
	{
		dataFlowStarted[4] = 0;
		alterDataFlow = 1;
	}
	int ret = mbPollOptions.DoModal();
	if (ret == IDOK)
	{
		pollConfigured[4] = 1;
		mbpollTotalCount[4] = 0;
		mbpollErrCount[4] = 0;
		updateStaticConfig(4);
		updateBtnLabel(4);
		updateDataSharingVariables(4);
	}
	if (alterDataFlow == 1)
	{
		dataFlowStarted[4] = 1;
	}
}
#endif

void CMbPoll::OnClickedButtonStartStop1()
{
	// TODO: Add your control notification handler code here
	OnClickedButtonStartStop(ctrlGrid1, 0, btnStartStop1, btnConfig1);
}

#if 1
void CMbPoll::OnClickedButtonStartStop2()
{
	// TODO: Add your control notification handler code here
	OnClickedButtonStartStop(ctrlGrid2, 1, btnStartStop2, btnConfig2);
}

void CMbPoll::OnClickedButtonStartStop3()
{
	// TODO: Add your control notification handler code here
	OnClickedButtonStartStop(ctrlGrid3, 2, btnStartStop3, btnConfig3);
}

void CMbPoll::OnClickedButtonStartStop4()
{
	// TODO: Add your control notification handler code here
	OnClickedButtonStartStop(ctrlGrid4, 3, btnStartStop4, btnConfig4);
}

void CMbPoll::OnClickedButtonStartStop5()
{
	// TODO: Add your control notification handler code here
	OnClickedButtonStartStop(ctrlGrid5, 4, btnStartStop5, btnConfig5);
}
#endif

void CMbPoll::OnClickedButtonStartStop(CMsflexgrid &grid, int gridNum, CButton &btn, CButton &configbtn)
{
	// TODO: Add your control notification handler code here
	if (connectionSuccessful != 1) 
	{
		MessageBox(L"Please establish Modbus connection first.", L"NO CONNECTION");
		return;
	}
	if (pollConfigured[gridNum] != 1) 
	{
		MessageBox(L"Please do config first.", L"NOT CONFIGURED");
		return;
	}

	if (pollSingleFunction[gridNum] == 0)
	{
		if (dataFlowStarted[gridNum] == 0)
		{
			dataFlowStarted[gridNum] = 1;
			btn.SetWindowText(L"Stop");
			//configbtn.EnableWindow(FALSE);
			configbtn.EnableWindow(TRUE);
		}
		else
		{
			dataFlowStarted[gridNum] = 0;
			btn.SetWindowText(L"Start");
			configbtn.EnableWindow(TRUE);
		}
	}
	if (pollSingleFunction[gridNum] == 1)
	{
		if(executeOnce[gridNum] == 0) executeOnce[gridNum] = 1;
		btn.SetWindowText(L"Execute");
	}
}

void CMbPoll::OnClickedCheckTapData()
{
	// TODO: Add your control notification handler code here
	if (checkTapData.GetCheck() == TRUE)
	{
		//tapDataMode = 1;		// no tapping required
		tapDataMode = 0;
	}
	else
	{
		tapDataMode = 0;
	}
}

void CMbPoll::updateBtnLabel(int configNo)
{
	CButton *btnStartStop;
	CStatic *staticRunning;

	switch(configNo)
	{
	case 0:
		btnStartStop = &btnStartStop1;
		staticRunning = &staticRunning1;
		break;
	case 1:
		btnStartStop = &btnStartStop2;
		staticRunning = &staticRunning2;
		break;
	case 2:
		btnStartStop = &btnStartStop3;
		staticRunning = &staticRunning3;
		break;
	case 3:
		btnStartStop = &btnStartStop4;
		staticRunning = &staticRunning4;
		break;
	case 4:
		btnStartStop = &btnStartStop5;
		staticRunning = &staticRunning5;
		break;
	default:
		break;
	}

	if (pollSingleFunction[configNo] == 1)
	{
		btnStartStop->SetWindowText(L"Execute");
		staticRunning->SetWindowText(L" ");
		dataFlowStarted[configNo] = 0;
	}
	else
	{
		btnStartStop->SetWindowText(L"Start");
		dataFlowStarted[configNo] = 0;
	}
}

void CMbPoll::updateStaticConfig(int configNo, int noRowsUpdate)
{
	CString tempStrSlaveId;
	CString tempStrFunction;
	CString tempStrAddress;
	CString tempStrQuantity;
	CString tempStrScanRate;
	CString tempStrTotalCount;
	CString tempStrErrCount;
	CString tempStr;
	int tempVar;
	CMsflexgrid *grid;
	CStatic *staticConfig;

	if (pollConfigured[configNo] != 1) return;
	
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
	tempStrScanRate.Format(_T("%d"), pollScanRate[configNo]);
	tempStrTotalCount.Format(_T("%d"), mbpollTotalCount[configNo]);
	tempStrErrCount.Format(_T("%d"), mbpollErrCount[configNo]);

	tempStr = L"S=" + tempStrSlaveId + L", F=" + tempStrFunction + L", A=" + tempStrAddress + L", Q=" + tempStrQuantity + L", SR=" + tempStrScanRate + L", Tx=" + tempStrTotalCount + L", Err=" + tempStrErrCount;

	switch(configNo)
	{
	case 0:
		grid = &ctrlGrid1;
		staticConfig = &staticConfig1;
		break;
	case 1:
		grid = &ctrlGrid2;
		staticConfig = &staticConfig2;
		break;
	case 2:
		grid = &ctrlGrid3;
		staticConfig = &staticConfig3;
		break;
	case 3:
		grid = &ctrlGrid4;
		staticConfig = &staticConfig4;
		break;
	case 4:
		grid = &ctrlGrid5;
		staticConfig = &staticConfig5;
		break;
	default:
		break;
	}

	staticConfig->SetWindowText(tempStr);

	if (noRowsUpdate != 1)
	{
		if (pollRows[configNo] == 0) grid->put_Rows(pollQuantity[configNo] + 1);
		if (pollRows[configNo] == 10) grid->put_Rows(11);
		if (pollRows[configNo] == 20) grid->put_Rows(21);
		if (pollRows[configNo] == 50) grid->put_Rows(51);
		if (pollRows[configNo] == 100) grid->put_Rows(101);
	}
}

void CMbPoll::updateDataSharingVariables(int configNo)
{
	switch(configNo)
	{
	case 0:
		if (grid1Data != NULL)
		{
		/*delete(grid1Data); 
		grid1Data = NULL;*/
		}
		grid1Data = (short int*) malloc (sizeof(short int) * pollQuantity[configNo]);
		break;
	case 1:
		if (grid2Data != NULL){delete(grid2Data); grid2Data = NULL;}
		grid2Data = (short int*) malloc (sizeof(short int) * pollQuantity[configNo]);
		break;
	case 2:
		if (grid3Data != NULL){delete(grid3Data); grid3Data = NULL;}
		grid3Data = (short int*) malloc (sizeof(short int) * pollQuantity[configNo]);
		break;
	case 3:
		if (grid4Data != NULL){delete(grid4Data); grid4Data = NULL;}
		grid4Data = (short int*) malloc (sizeof(short int) * pollQuantity[configNo]);
		break;
	case 4:
		if (grid5Data != NULL){delete(grid5Data); grid5Data = NULL;}
		grid5Data = (short int*) malloc (sizeof(short int) * pollQuantity[configNo]);
		break;
	default:
		break;
	}
	putDataNow[configNo] = 1;
	putDataInGrid[configNo] = 1;
}

void CMbPoll::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	// TODO: Add your message handler code here
}

void CMbPoll::PostNcDestroy() 
{	
    CDialog::PostNcDestroy();
	mParent->PostMessage(WM_MBPOLL_CLOSED,0,0);
	delete this;
}

/*
Conversion function:
convType: 
0 = Signed
1 = Unsigned
2 = Hex
3 = Binary
*/

CString CMbPoll::convertFunction(int convType, unsigned short val)
{
	static CString tempStr;
	short int tempVal1;
	unsigned short int tempVal2;
	unsigned short int tempVal3;
	unsigned short int tempVal4;
	unsigned short int bitPosition = 0x8000;
	CString strToReturn;

	switch (convType)
	{
	case 0:		/* Signed */
		tempVal1 = (short int)val;
		strToReturn.Format(_T("%d"), tempVal1);
		break;
	case 1:		/* Unsigned */
		tempVal2 = (unsigned short int)val;
		strToReturn.Format(_T("%d"), tempVal2);
		break;
	case 2:		/* Hex */
		tempVal3 = (unsigned short int)val;
		strToReturn.Format(_T("%04X"), tempVal3);
		break;
	case 3:		/* Binary */
		tempVal4 = (unsigned short int)val;
		tempStr = L"";
		for (int i = 0; i < 16; i++)
		{
			if ((tempVal4 & bitPosition) == 0)
			{
				tempStr = tempStr + L"0";
			}
			else
			{
				tempStr = tempStr + L"1";
			}
			bitPosition = bitPosition >> 1;
		}
		strToReturn = tempStr;
		break;
	default: 
		tempVal2 = (unsigned short int)val;
		strToReturn.Format(_T("%d"), tempVal2);
		break;
	}
	return (strToReturn);
}

/*
const char * const quads[16][4] = {"0000","0001","0010","0011",
							"0100","0101","0110","0111",
							"1000","1001","1010","1011",
							"1100","1101","1110","1111"};

const char * hex_to_bin_quad(unsigned char c)
{
  if (c >= '0' && c <= '9') return quads[     c - '0'];
  if (c >= 'A' && c <= 'F') return quads[10 + c - 'A'];
  if (c >= 'a' && c <= 'f') return quads[10 + c - 'a'];
  return -1;
}
*/

void WriteCString (CFile& file, CString& str)
{
	int iChars = 1 + str.GetLength ();

	file.Write (&iChars, sizeof (iChars));
	file.Write ((LPVOID)(LPCTSTR) str, iChars * sizeof (TCHAR));
}

void ReadCString (CString& str, CFile& file)
{
	int iChars;

	file.Read (&iChars, sizeof (iChars));
	file.Read ((LPVOID) str.GetBuffer (iChars), iChars * sizeof (TCHAR));
	str.ReleaseBuffer ();
}

void CMbPoll::OnFileSave()
{
	// TODO: Add your command handler code here
	CFile f;
	CString strFilter = _T("Text file (*.txt)|*.txt|Rich Text file (*.rtf)|*.rtf|All files (*.*)|*.*||");
	CString temp;
	int qtyToShow[5];
	CMsflexgrid* grid;

	//CFileDialog dlg(true,_T("Save current Config"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
	CFileDialog dlg(false, _T("Save current Config"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, strFilter);

	if (dlg.DoModal() == IDOK)
	{
		CString strConfigFilePathName = dlg.GetPathName();
		//MessageBox(strConfigFilePathName);
		CString strConfigFileName = dlg.GetFileName();
		//MessageBox(strConfigFileName);
		//BOOL a = f.Open(strConfigFileName, CFile::modeCreate | CFile::modeWrite);
		BOOL a = f.Open(strConfigFilePathName, CFile::modeCreate | CFile::modeWrite);

		//CString str1;
		//str1 = L"Text1;Text2;Text3;";
		//WriteCString(f, str1);
		//f.Close();

#if SHOW_ALL
		for (int i = 0; i < 5; i++)
#endif
#if !SHOW_ALL
		for (int i = 0; i < 1; i++)
#endif
		{
			if (i == 0) grid = &ctrlGrid1;
			if (i == 1) grid = &ctrlGrid2;
			if (i == 2) grid = &ctrlGrid3;
			if (i == 3) grid = &ctrlGrid4;
			if (i == 4) grid = &ctrlGrid5;

			temp.Format(_T("%d"), i);				// grid number
			WriteCString(f, temp);
			temp.Format(_T("%d"), dataFlowStarted[i]);	// dataflow started
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollConfigured[i]);	// poll configured
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollSlaveId[i]);	// slave ID
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollFunction[i]);	// function
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollAddress[i]);	// address
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollQuantity[i]); // quantity
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollScanRate[i]); // scan rate
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollSingleFunction[i]); // single function
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollRows[i]);		// rows
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollDisplay[i]);	// display
			WriteCString(f, temp);

			qtyToShow[i] = ((pollRows[i] == 0) || (pollQuantity[i] < pollRows[i])) ? pollQuantity[i] : pollRows[i];
			for (int j = 0; j < qtyToShow[i]; j++)
			{
				//temp = grid->get_TextMatrix(j+1, 0);
				int rowNo = j % ROWS_OF_MBPOLLLIST;
				int colNo = (j / ROWS_OF_MBPOLLLIST) * 3;
				temp = mbPollList.GetItemText(rowNo, colNo);
				WriteCString(f, temp);
			}
		}
		f.Close();
	}
}

void CMbPoll::OnFileOpen()
{
	// TODO: Add your command handler code here

	int qtyToShow[5];
	CMsflexgrid* grid;
	CString strFilter = _T("Text file (*.txt)|*.txt|Rich Text file (*.rtf)|*.rtf|All files (*.*)|*.*||");
	CFile f;
	CString temp;

	CFileDialog dlg(true, _T("Open current Config"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, strFilter);
	if (dlg.DoModal() == IDOK)
	{
		CString strConfigFilePathName1 = dlg.GetPathName();
		//MessageBox(strConfigFilePathName1);
		CString strConfigFileName1 = dlg.GetFileName();
		//MessageBox(strConfigFileName1);
		//f.Open(strConfigFileName1, CFile::modeRead);
		f.Open(strConfigFilePathName1, CFile::modeRead);

#if SHOW_ALL
		for (int i = 0; i < 5; i++)
#endif
#if !SHOW_ALL
		for (int i = 0; i < 1; i++)
#endif
		{
			if (i == 0) grid = &ctrlGrid1;
			if (i == 1) grid = &ctrlGrid2;
			if (i == 2) grid = &ctrlGrid3;
			if (i == 3) grid = &ctrlGrid4;
			if (i == 4) grid = &ctrlGrid5;

			ReadCString(temp, f);		// grid number
			ReadCString(temp, f);		// dataflow started
			dataFlowStarted[i] = _wtoi(temp);
			ReadCString(temp, f);		// poll configured
			pollConfigured[i] = _wtoi(temp);
			ReadCString(temp, f);		// slave ID
			pollSlaveId[i] = _wtoi(temp);
			ReadCString(temp, f);		// function
			pollFunction[i] = _wtoi(temp);
			ReadCString(temp, f);		// address
			pollAddress[i] = _wtoi(temp);
			ReadCString(temp, f);		// quantity
			pollQuantity[i] = _wtoi(temp);
			ReadCString(temp, f);		// scan rate
			pollScanRate[i] = _wtoi(temp);
			ReadCString(temp, f);		// single function
			pollSingleFunction[i] = _wtoi(temp);
			ReadCString(temp, f);		// rows
			pollRows[i] = _wtoi(temp);
			ReadCString(temp, f);		// display
			pollDisplay[i] = _wtoi(temp);
			qtyToShow[i] = ((pollRows[i] == 0) || (pollQuantity[i] < pollRows[i])) ? pollQuantity[i] : pollRows[i];
			if (pollRows[i] == 0) grid->put_Rows(pollQuantity[i] + 1);
			if (pollRows[i] == 10) grid->put_Rows(11);
			if (pollRows[i] == 20) grid->put_Rows(21);
			if (pollRows[i] == 50) grid->put_Rows(51);
			if (pollRows[i] == 100) grid->put_Rows(101);
			for (int j = 0; j < qtyToShow[i]; j++)
			{
				ReadCString(temp, f);
				//grid->put_TextMatrix(j + 1, 0, temp);		// put_TextMatrix(row, col, text);
				int rowNo = j % ROWS_OF_MBPOLLLIST;
				int colNo = (j / ROWS_OF_MBPOLLLIST) * 3;
				mbPollList.SetItemText(rowNo, colNo, temp);
			}
			saveRegisterNames(i);
		}
		startStopBtnState();
		restoreConfigs();
		f.Close();
	}
}

void CMbPoll::OnClickedButtonTraffic1()
{
	// TODO: Add your control notification handler code here
	showTrafficWindow(0);
}

#if 1
void CMbPoll::OnClickedButtonTraffic2()
{
	// TODO: Add your control notification handler code here
	showTrafficWindow(1);
}

void CMbPoll::OnClickedButtonTraffic3()
{
	// TODO: Add your control notification handler code here
	showTrafficWindow(2);
}

void CMbPoll::OnClickedButtonTraffic4()
{
	// TODO: Add your control notification handler code here
	showTrafficWindow(3);
}

void CMbPoll::OnClickedButtonTraffic5()
{
	// TODO: Add your control notification handler code here
	showTrafficWindow(4);
}
#endif

void CMbPoll::showTrafficWindow(int slotNo)
{
	if (connectionSuccessful != 1) 
	{
		MessageBox(L"Please establish Modbus connection first.", L"NO CONNECTION");
		return;
	}
	if (pollConfigured[slotNo] != 1) 
	{
		MessageBox(L"Please do config first.", L"NOT CONFIGURED");
		return;
	}

	if (boolTrafficWindowOpen == FALSE)
	{
		if (trafficWindow != NULL) 
		{
			trafficWindow = NULL;
		}
		boolTrafficWindowOpen = TRUE;
		trafficStringIndex = 0;
		packetCount = 0;
		trafficSlotNo = slotNo;
		trafficWindow = new TrafficWindow;
		trafficWindow->Create(IDD_TRAFFIC_WINDOW,this);
		trafficWindow->ShowWindow(SW_SHOW);
		
	}
	else
	{
		boolTrafficWindowOpen = TRUE;
		trafficStringIndex = 0;
		packetCount = 0;
		trafficSlotNo = slotNo;
		trafficWindow->ShowWindow(SW_SHOW);
	}
}

afx_msg LRESULT CMbPoll::OnTrafficClosed(WPARAM wParam, LPARAM lParam)
{
    trafficWindow = NULL; 
	delete trafficWindow;
	boolTrafficWindowOpen = FALSE;
    return 0;
}

void CMbPoll::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here.
	m_nCurHeight = cy;
	m_nCurWidth = cx;

	int nScrollMax;
	
	if (cy < m_rect.Height())
	{
	     nScrollMax = m_rect.Height() - cy;
	}
	else
	     nScrollMax = 0;

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL; // SIF_ALL = SIF_PAGE | SIF_RANGE | SIF_POS;
	si.nMin = 0;
	si.nMax = nScrollMax;
	si.nPage = si.nMax/10;
	si.nPos = 0;
    SetScrollInfo(SB_VERT, &si, TRUE); 

	if (cx < m_rect.Width())
	{
	     nScrollMax = m_rect.Width() - cx;
	}
	else
	     nScrollMax = 0;

	//SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL; // SIF_ALL = SIF_PAGE | SIF_RANGE | SIF_POS;
	si.nMin = 0;
	si.nMax = nScrollMax;
	si.nPage = si.nMax/10;
	si.nPos = 0;
    SetScrollInfo(SB_HORZ, &si, TRUE); 

}

void CMbPoll::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int nDelta;
	int nMaxPos = m_rect.Width() - m_nCurWidth;

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (m_nHScrollPos >= nMaxPos)
			return;
		nDelta = min(nMaxPos/100,nMaxPos-m_nHScrollPos);
		break;

	case SB_LINEUP:
		if (m_nHScrollPos <= 0)
			return;
		nDelta = -min(nMaxPos/100,m_nHScrollPos);
		break;

         case SB_PAGEDOWN:
		if (m_nHScrollPos >= nMaxPos)
			return;
		nDelta = min(nMaxPos/10,nMaxPos-m_nHScrollPos);
		break;

	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_nHScrollPos;
		break;

	case SB_PAGEUP:
		if (m_nHScrollPos <= 0)
			return;
		nDelta = -min(nMaxPos/10,m_nHScrollPos);
		break;
	
         default:
		return;
	}
	m_nHScrollPos += nDelta;
	SetScrollPos(SB_HORZ,m_nHScrollPos,TRUE);
	ScrollWindow(-nDelta,0);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMbPoll::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int nDelta;
	int nMaxPos = m_rect.Height() - m_nCurHeight;

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;
		nDelta = min(nMaxPos/100,nMaxPos-m_nScrollPos);
		break;

	case SB_LINEUP:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -min(nMaxPos/100,m_nScrollPos);
		break;

         case SB_PAGEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;
		nDelta = min(nMaxPos/10,nMaxPos-m_nScrollPos);
		break;

	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_nScrollPos;
		break;

	case SB_PAGEUP:
		if (m_nScrollPos <= 0)
			return;
		nDelta = -min(nMaxPos/10,m_nScrollPos);
		break;
	
         default:
		return;
	}
	m_nScrollPos += nDelta;
	SetScrollPos(SB_VERT,m_nScrollPos,TRUE);
	ScrollWindow(0,-nDelta);
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

DWORD WINAPI CMbPoll::displayThreadStarter(LPVOID lpVoid)
{
	return ((CMbPoll*)lpVoid)->displayThread();
}

DWORD WINAPI CMbPoll::displayThread()
{
	DWORD dwWaitResult; 
	CString convertedStr;
	CString tempStr1;
	static int qtyToShow[5] = {0};
	int rowNo;
	int colNo;
	int tempVar;

	while(1)
	{
		dwWaitResult = WaitForSingleObject(displayThreadSemaphore, INFINITE/*0L*/);
		switch (dwWaitResult)
        {
			case WAIT_OBJECT_0:

				if (connectionSuccessful == 1)
				{
					staticConnectionStatus.ShowWindow(FALSE);
				}
				else
				{
					staticConnectionStatus.ShowWindow(TRUE);
				}

				if (pollSingleFunction[0] != 1)	staticRunning1.SetWindowText(L"X");
				if (pollSingleFunction[1] != 1)	staticRunning2.SetWindowText(L"X");
				if (pollSingleFunction[2] != 1)	staticRunning3.SetWindowText(L"X");
				if (pollSingleFunction[3] != 1)	staticRunning4.SetWindowText(L"X");
				if (pollSingleFunction[4] != 1)	staticRunning5.SetWindowText(L"X");

				/*	update grid if, 
					config has been done (grid1Data != NULL), 
					Start button has been clicked (dataFlowStarted[0] == 1), 
					its not a write function where grid update is not required (pollFunction[0] <= 3)	*/
				if (runPoll[0] == 1)
				{
					if (((grid1Data != NULL) && (dataFlowStarted[0] == 1)) || (executeOnce[0] == 2))
					{
						if (pollSingleFunction[0] != 1)	staticRunning1.SetWindowText(L"?");
						if (executeOnce[0] == 2) executeOnce[0] = 0;
						if (putDataNow[0] == 1)
						{
							qtyToShow[0] = ((pollRows[0] == 0) || (pollQuantity[0] < pollRows[0])) ? pollQuantity[0] : pollRows[0];
							putDataNow[0] = 0;
						}
						//CString temppp;
						//temppp.Format(_T("qty = %d"), qtyToShow[0]);
						//MessageBox(temppp);
						if (firstTime1 == 1)
						{
							for (int i = 0; i < ROWS_OF_MBPOLLLIST; i++)
							{
								mbPollList.SetItemText(i, 1, L"");
								mbPollList.SetItemText(i, 2, L"");
								mbPollList.SetItemText(i, 4, L"");
								mbPollList.SetItemText(i, 5, L"");
								mbPollList.SetItemText(i, 7, L"");
								mbPollList.SetItemText(i, 8, L"");
								mbPollList.SetItemText(i, 10, L"");
								mbPollList.SetItemText(i, 11, L"");
								mbPollList.SetItemText(i, 13, L"");
								mbPollList.SetItemText(i, 14, L"");
							}
						}

						for (int i = 0; i < qtyToShow[0]; i++)
						{
							rowNo = i % ROWS_OF_MBPOLLLIST;
							tempVar = i / ROWS_OF_MBPOLLLIST;
							colNo = 3 * tempVar + 1;

							if (firstTime1 == 1)
							{
								tempStr1.Format(_T("%d"), (pollAddress[0] + i)); 
								//ctrlGrid1.put_TextMatrix(i + 1, 1, tempStr1);	// uncomment if old grid is to be used
						
								mbPollList.SetItemText(rowNo, colNo, tempStr1);
							}

							updateStaticConfig(0, 1);
							if ((!((tapDataMode == 0) && (pollFunction[0] >= 4))) && (connectionSuccessful == 1))
							{
								if (firstTime1 == 0) 
								{
									//MessageBox(L"not ft, not disp");
									//if (prevData1[i] != (unsigned short)(*((short int*)grid1Data + i)))
									if (1)
									{
										//MessageBox(L"not ft, displayed");
										//tempStr1.Format(_T("%d"), *((short int*)grid1Data + i)); 
										convertedStr = convertFunction(menuChecked/*pollDisplay[0]*/, (unsigned short)(*((short int*)grid1Data + i)));
										//ctrlGrid1.put_TextMatrix(i + 1, 2, convertedStr);	// uncomment if old grid is to be used
										colNo = 3 * tempVar + 2;
										mbPollList.SetItemText(rowNo, colNo, convertedStr);
										prevData1[i] = (unsigned short)(*((short int*)grid1Data + i));
									}
								}
								if (firstTime1 == 1)
								{
									//MessageBox(L"firstTime");
									convertedStr = convertFunction(menuChecked/*pollDisplay[0]*/, (unsigned short)(*((short int*)grid1Data + i)));
									colNo = 3 * tempVar + 2;
									mbPollList.SetItemText(rowNo, colNo, convertedStr);
									prevData1[i] = (unsigned short)(*((short int*)grid1Data + i));
								}
							}
						}
						firstTime1 = 0;
					}
				}

				if (runPoll[1] == 1)
				{
					if (((grid2Data != NULL)  && (dataFlowStarted[1] == 1)) || (executeOnce[1] == 2))
					{
						if (pollSingleFunction[1] != 1)	staticRunning2.SetWindowText(L"?");
						if (executeOnce[1] == 2) executeOnce[1] = 0;
						if (putDataNow[1] == 1)
						{
							qtyToShow[1] = ((pollRows[1] == 0) || (pollQuantity[1] < pollRows[1])) ? pollQuantity[1] : pollRows[1];
							putDataNow[1] = 0;
						}
						for (int i = 0; i < qtyToShow[1]; i++)
						{
							tempStr1.Format(_T("%d"), (pollAddress[1] + i)); 
							//ctrlGrid2.put_TextMatrix(i + 1, 1, tempStr1);	// uncomment if old grid is to be used
							rowNo = i % ROWS_OF_MBPOLLLIST;
							tempVar = i / ROWS_OF_MBPOLLLIST;
							colNo = 3 * tempVar + 1;
							mbPollList.SetItemText(rowNo, colNo, tempStr1);

							updateStaticConfig(1, 1);
							if ((!((tapDataMode == 0) && (pollFunction[1] >= 4))) && (connectionSuccessful == 1))
							{
								if (firstTime2 == 0) 
								{
									if (prevData2[i] != (unsigned short)(*((short int*)grid2Data + i)))
									{
										//tempStr1.Format(_T("%d"), *((short int*)grid2Data + i)); 
										convertedStr = convertFunction(pollDisplay[1], (unsigned short)(*((short int*)grid2Data + i)));
										//ctrlGrid2.put_TextMatrix(i + 1, 2, convertedStr);	// uncomment if old grid is to be used
										colNo = 3 * tempVar + 2;
										mbPollList.SetItemText(rowNo, colNo, convertedStr);
										prevData2[i] = (unsigned short)(*((short int*)grid2Data + i));
									}
								}
								if (firstTime2 == 1)
								{
									convertedStr = convertFunction(pollDisplay[1], (unsigned short)(*((short int*)grid2Data + i)));
									colNo = 3 * tempVar + 2;
									mbPollList.SetItemText(rowNo, colNo, convertedStr);
									prevData2[i] = (unsigned short)(*((short int*)grid2Data + i));
								}
							}
						}
						firstTime2 = 0;
					}
				}

				if (runPoll[2] == 1)
				{
					if (((grid3Data != NULL) && (dataFlowStarted[2] == 1)) || (executeOnce[2] == 2))
					{
						if (pollSingleFunction[2] != 1)	staticRunning3.SetWindowText(L"?");
						if (executeOnce[2] == 2) executeOnce[2] = 0;
						if (putDataNow[2] == 1)
						{
							qtyToShow[2] = ((pollRows[2] == 0) || (pollQuantity[2] < pollRows[2])) ? pollQuantity[2] : pollRows[2];
							putDataNow[2] = 0;
						}
						for (int i = 0; i < qtyToShow[2]; i++)
						{
							tempStr1.Format(_T("%d"), (pollAddress[2] + i)); 
							//ctrlGrid3.put_TextMatrix(i + 1, 1, tempStr1);	// uncomment if old grid is to be used
							rowNo = i % ROWS_OF_MBPOLLLIST;
							tempVar = i / ROWS_OF_MBPOLLLIST;
							colNo = 3 * tempVar + 1;
							mbPollList.SetItemText(rowNo, colNo, tempStr1);

							updateStaticConfig(2, 1);
							if ((!((tapDataMode == 0) && (pollFunction[2] >= 4))) && (connectionSuccessful == 1))
							{
								if (firstTime3 == 0) 
								{
									if (prevData3[i] != (unsigned short)(*((short int*)grid3Data + i)))
									{
										convertedStr = convertFunction(pollDisplay[2], (unsigned short)(*((short int*)grid3Data + i)));
										//ctrlGrid3.put_TextMatrix(i + 1, 2, convertedStr);	// uncomment if old grid is to be used
										colNo = 3 * tempVar + 2;
										mbPollList.SetItemText(rowNo, colNo, convertedStr);
										prevData3[i] = (unsigned short)(*((short int*)grid3Data + i));
									}
								}
								if (firstTime3 == 1)
								{
									convertedStr = convertFunction(pollDisplay[2], (unsigned short)(*((short int*)grid3Data + i)));
									colNo = 3 * tempVar + 2;
									mbPollList.SetItemText(rowNo, colNo, convertedStr);
									prevData3[i] = (unsigned short)(*((short int*)grid3Data + i));
								}
							}
						}
						firstTime3 = 0;
					}
				}

				if (runPoll[3] == 1)
				{
					if (((grid4Data != NULL) && (dataFlowStarted[3] == 1)) || (executeOnce[3] == 2))
					{
						if (pollSingleFunction[3] != 1)	staticRunning4.SetWindowText(L"?");
						if (executeOnce[3] == 2) executeOnce[3] = 0;
						if (putDataNow[3] == 1)
						{
							qtyToShow[3] = ((pollRows[3] == 0) || (pollQuantity[3] < pollRows[3])) ? pollQuantity[3] : pollRows[3];
							putDataNow[3] = 0;
						}
						for (int i = 0; i < qtyToShow[3]; i++)
						{
							tempStr1.Format(_T("%d"), (pollAddress[3] + i)); 
							//ctrlGrid4.put_TextMatrix(i + 1, 1, tempStr1);	// uncomment if old grid is to be used
							rowNo = i % ROWS_OF_MBPOLLLIST;
							tempVar = i / ROWS_OF_MBPOLLLIST;
							colNo = 3 * tempVar + 1;
							mbPollList.SetItemText(rowNo, colNo, tempStr1);

							updateStaticConfig(3, 1);
							if ((!((tapDataMode == 0) && (pollFunction[3] >= 4))) && (connectionSuccessful == 1))
							{
								if (firstTime4 == 0) 
								{
									if (prevData4[i] != (unsigned short)(*((short int*)grid4Data + i)))
									{
										convertedStr = convertFunction(pollDisplay[3], (unsigned short)(*((short int*)grid4Data + i)));
										//ctrlGrid4.put_TextMatrix(i + 1, 2, convertedStr);	// uncomment if old grid is to be used
										colNo = 3 * tempVar + 2;
										mbPollList.SetItemText(rowNo, colNo, convertedStr);
										prevData4[i] = (unsigned short)(*((short int*)grid4Data + i));
									}
								}
								if (firstTime4 == 1)
								{
										convertedStr = convertFunction(pollDisplay[3], (unsigned short)(*((short int*)grid4Data + i)));
										colNo = 3 * tempVar + 2;
										mbPollList.SetItemText(rowNo, colNo, convertedStr);
										prevData4[i] = (unsigned short)(*((short int*)grid4Data + i));
								}
							}
						}
						firstTime4 = 0;
					}
				}

				if (runPoll[4] == 1)
				{
					if (((grid5Data != NULL) && (dataFlowStarted[4] == 1)) || (executeOnce[4] == 2))
					{
						if (pollSingleFunction[4] != 1)	staticRunning5.SetWindowText(L"?");
						if (executeOnce[4] == 2) executeOnce[4] = 0;
						if (putDataNow[4] == 1)
						{
							qtyToShow[4] = ((pollRows[4] == 0) || (pollQuantity[4] < pollRows[4])) ? pollQuantity[4] : pollRows[4];
							putDataNow[4] = 0;
						}
						for (int i = 0; i < qtyToShow[4]; i++)
						{
							tempStr1.Format(_T("%d"), (pollAddress[4] + i)); 
							//ctrlGrid5.put_TextMatrix(i + 1, 1, tempStr1);	// uncomment if old grid is to be used
							rowNo = i % ROWS_OF_MBPOLLLIST;
							tempVar = i / ROWS_OF_MBPOLLLIST;
							colNo = 3 * tempVar + 1;
							mbPollList.SetItemText(rowNo, colNo, tempStr1);

							updateStaticConfig(4, 1);
							if ((!((tapDataMode == 0) && (pollFunction[4] >= 4))) && (connectionSuccessful == 1))
							{
								if (firstTime5 == 0) 
								{
									if (prevData5[i] != (unsigned short)(*((short int*)grid5Data + i)))
									{
										convertedStr = convertFunction(pollDisplay[4], (unsigned short)(*((short int*)grid5Data + i)));
										//ctrlGrid5.put_TextMatrix(i + 1, 2, convertedStr);	// uncomment if old grid is to be used
										colNo = 3 * tempVar + 2;
										mbPollList.SetItemText(rowNo, colNo, convertedStr);
										prevData5[i] = (unsigned short)(*((short int*)grid5Data + i));
									}
								}
								if (firstTime5 == 1)
								{
									convertedStr = convertFunction(pollDisplay[4], (unsigned short)(*((short int*)grid5Data + i)));
									//ctrlGrid5.put_TextMatrix(i + 1, 2, convertedStr);	// uncomment if old grid is to be used
									colNo = 3 * tempVar + 2;
									mbPollList.SetItemText(rowNo, colNo, convertedStr);
									prevData5[i] = (unsigned short)(*((short int*)grid5Data + i));
								}
							}
						}
						firstTime5 = 0;
					}
				}

				temporaryCount++;
				break;
			case WAIT_TIMEOUT: 
				//temporaryCount++;
				break;
			default:
				break;
		}
	}
}

DWORD WINAPI CMbPoll::mbCommThreadStarter(LPVOID lpVoid)
{
	return ((CMbPoll*)lpVoid)->mbCommThread();
}

DWORD WINAPI CMbPoll::mbCommThread()
{
	static unsigned int count = 0;
	static unsigned int a, b, c, d, e = 0;
	DWORD dwWaitResult; 

	while(1)
	{
		dwWaitResult = WaitForSingleObject(mbCommThreadSemaphore, INFINITE/*0L*/);
		switch (dwWaitResult)
        {
			case WAIT_OBJECT_0:
				count++;
				if (dataFlowStarted[0] == 1)
				{
					if ((count % (pollScanRate[0]/50)) == 0)	
					{
						callMbFunc(0);
						a++;
					}
				}
				if (dataFlowStarted[1] == 1)
				{
					if ((count % (pollScanRate[1]/50)) == 0)	
					{
						callMbFunc(1);
						b++;
					}
				}
				if (dataFlowStarted[2] == 1)
				{
					if ((count % (pollScanRate[2]/50)) == 0)	
					{
						callMbFunc(2);
						c++;
					}
				}
				if (dataFlowStarted[3] == 1)
				{
					if ((count % (pollScanRate[3]/50)) == 0)	
					{
						callMbFunc(3);
						d++;
					}
				}
				if (dataFlowStarted[4] == 1)
				{
					if ((count % (pollScanRate[4]/50)) == 0)	
					{
						callMbFunc(4);
						e++;
					}
				}

				if (executeOnce[0] == 1)
				{
					callMbFunc(0);
					executeOnce[0] = 2;
				}
				if (executeOnce[1] == 1)
				{
					callMbFunc(1);
					executeOnce[1] = 2;
				}
				if (executeOnce[2] == 1)
				{
					callMbFunc(2);
					executeOnce[2] = 2;
				}
				if (executeOnce[3] == 1)
				{
					callMbFunc(3);
					executeOnce[3] = 2;
				}
				if (executeOnce[4] == 1)
				{
					callMbFunc(4);
					executeOnce[4] = 2;
				}
				break;
			case WAIT_TIMEOUT: 
				//temporaryCount++;
				break;
			default:
				break;
		}
	}
}

void CMbPoll::saveRadioButtonState()
{

}

void CMbPoll::OnClickedMbpollCheck1()
{
	// TODO: Add your control notification handler code here
	OnClickedMbpollCheck(0);
}

void CMbPoll::OnClickedMbpollCheck2()
{
	// TODO: Add your control notification handler code here
	OnClickedMbpollCheck(1);
}

void CMbPoll::OnClickedMbpollCheck3()
{
	// TODO: Add your control notification handler code here
	OnClickedMbpollCheck(2);
}

void CMbPoll::OnClickedMbpollCheck4()
{
	// TODO: Add your control notification handler code here
	OnClickedMbpollCheck(3);
}

void CMbPoll::showRadioButton()
{

}

void CMbPoll::restoreRadioButtonState()
{
	if (runPoll[0] == 1)
	{
		mbpollCheck1.SetCheck(TRUE);
		mbpollCheck2.SetCheck(FALSE);
		mbpollCheck3.SetCheck(FALSE);
		mbpollCheck4.SetCheck(FALSE);
	}
	else if (runPoll[1] == 1)
	{
		mbpollCheck1.SetCheck(FALSE);
		mbpollCheck2.SetCheck(TRUE);
		mbpollCheck3.SetCheck(FALSE);
		mbpollCheck4.SetCheck(FALSE);
	}
	else if (runPoll[2] == 1)
	{
		mbpollCheck1.SetCheck(FALSE);
		mbpollCheck2.SetCheck(FALSE);
		mbpollCheck3.SetCheck(TRUE);
		mbpollCheck4.SetCheck(FALSE);
	}
	else if (runPoll[3] == 1)
	{
		mbpollCheck1.SetCheck(FALSE);
		mbpollCheck2.SetCheck(FALSE);
		mbpollCheck3.SetCheck(FALSE);
		mbpollCheck4.SetCheck(TRUE);
	}
}

void CMbPoll::OnClickedMbpollCheck(int gridNo)
{
	/*mbPollList.DeleteAllItems();
	for (int i = 0; i < ROWS_OF_MBPOLLLIST; i++)
	{
		mbPollList.InsertItem(i,L"");
	}*/

	switch(gridNo)
	{
	case 0:
		restoreRegisterNames(0);
		mbpollCheck1.SetCheck(TRUE);
		mbpollCheck2.SetCheck(FALSE);
		mbpollCheck3.SetCheck(FALSE);
		mbpollCheck4.SetCheck(FALSE);
		runPoll[0] = 1;
		runPoll[1] = 0;
		runPoll[2] = 0;
		runPoll[3] = 0;
		runPoll[4] = 0;
		break;
	case 1:
		restoreRegisterNames(1);
		mbpollCheck1.SetCheck(FALSE);
		mbpollCheck2.SetCheck(TRUE);
		mbpollCheck3.SetCheck(FALSE);
		mbpollCheck4.SetCheck(FALSE);
		runPoll[0] = 0;
		runPoll[1] = 1;
		runPoll[2] = 0;
		runPoll[3] = 0;
		runPoll[4] = 0;
		break;
	case 2:
		restoreRegisterNames(2);
		mbpollCheck1.SetCheck(FALSE);
		mbpollCheck2.SetCheck(FALSE);
		mbpollCheck3.SetCheck(TRUE);
		mbpollCheck4.SetCheck(FALSE);
		runPoll[0] = 0;
		runPoll[1] = 0;
		runPoll[2] = 1;
		runPoll[3] = 0;
		runPoll[4] = 0;
		break;
	case 3:
		restoreRegisterNames(3);
		mbpollCheck1.SetCheck(FALSE);
		mbpollCheck2.SetCheck(FALSE);
		mbpollCheck3.SetCheck(FALSE);
		mbpollCheck4.SetCheck(TRUE);
		runPoll[0] = 0;
		runPoll[1] = 0;
		runPoll[2] = 0;
		runPoll[3] = 1;
		runPoll[4] = 0;
		break;
	default:
		break;
	}
}

void CMbPoll::saveRegisterNames(int gridNo)
{
	CString regStr;

	regStr = L"";

	for (int i = 0; i < 15; i++)	// col
	{
		for (int j = 0; j < ROWS_OF_MBPOLLLIST; j++)	// row
		{
			/*rowNo = j % ROWS_OF_MBPOLLLIST;
			tempVar = j / ROWS_OF_MBPOLLLIST;
			colNo = 3 * tempVar;*/

			regStr = regStr + mbPollList.GetItemText(j, i) + L";";
		}
	}

	//MessageBox(regStr);

	switch(gridNo)
	{
	case 0:
		registerNames1 = regStr;
		break;
	case 1:
		registerNames2 = regStr;
		break;
	case 2:
		registerNames3 = regStr;
		break;
	case 3:
		registerNames4 = regStr;
		break;
	default:
		break;
	}
}

void CMbPoll::restoreRegisterNames(int gridNo)
{
	CString regStr;
	int rowNo;
	int colNo;
	int tempVar;
	int position;
	CString temp1, temp2;
	int cellCount = 0;

	switch(gridNo)
	{
	case 0:
		regStr = registerNames1;
		break;
	case 1:
		regStr = registerNames2;
		break;
	case 2:
		regStr = registerNames3;
		break;
	case 3:
		regStr = registerNames4;
		break;
	default:
		break;
	}

	//MessageBox(regStr);

	mbPollList.DeleteAllItems();
	for (int i = 0; i < ROWS_OF_MBPOLLLIST; i++)
	{
		mbPollList.InsertItem(i,L"");
	}

	if (regStr == L"") return;

	while (1)
	{
		position = regStr.Find(L";");
		if (position == -1) 
		{
			//MessageBox(L"position = -1");
			break;
		}
		temp1 = regStr.Left(position);

		if (position != 0)
		{
			regStr = regStr.Right(regStr.GetLength() - position - 1);
		}
		else
		{
			regStr = regStr.Right(regStr.GetLength() - 1);
		}

		rowNo = cellCount % ROWS_OF_MBPOLLLIST;
		colNo = cellCount / ROWS_OF_MBPOLLLIST;

		mbPollList.SetItemText(rowNo, colNo, temp1);

		cellCount++;
	}
}

afx_msg LRESULT CMbPoll::OnMyMbpollRegDialogMsg(WPARAM wParam, LPARAM lParam)
{
	CRegDetails Dlg;
	CString temp1, temp2;
	int aliasRow;
	int aliasCol;
	int regValueRow;
	int regValueCol;
	int regAddrRow;
	int regAddrCol;

	aliasString = L"";
	regValueString = L"";
	regAddrString = L"";

	//temp1.Format(_T("row = %d col = %d"), wParam, lParam);
	//MessageBox(temp1);

	if ((wParam < 0) || (lParam < 0))
	{
		return -1;
	}

	aliasRow = wParam;
	aliasCol = (lParam / 3) * 3;
	regAddrRow = wParam;
	regAddrCol = ((lParam / 3) * 3) + 1;
	regValueRow = wParam;
	regValueCol = ((lParam / 3) * 3) + 2;
	aliasString = mbPollList.GetItemText(aliasRow, aliasCol);
	regValueString = mbPollList.GetItemText(regValueRow, regValueCol);
	regAddrString = mbPollList.GetItemText(regAddrRow, regAddrCol);
	
	/*MessageBox(aliasString);
	MessageBox(regValueString);
	MessageBox(regAddrString);*/

	editableValueField = 1;
	if (regDetailsOpenedFrom == 0)
	{
		if (((runPoll[0] == 1) && (pollFunction[0] < 4)) || 
			((runPoll[1] == 1) && (pollFunction[1] < 4)) || 
			((runPoll[2] == 1) && (pollFunction[2] < 4)) || 
			((runPoll[3] == 1) && (pollFunction[3] < 4)))
		{
			editableValueField = 0;
		}
	}
	
	if (Dlg.DoModal() == IDOK)
	{
		//MessageBox(aliasString);
		//MessageBox(regValueString);
		mbPollList.SetItemText(aliasRow, aliasCol, aliasString);
		if (editableValueField == 1)
		{
			mbPollList.SetItemText(regValueRow, regValueCol, regValueString);
		}
	}
	else
	{
	}

	if (runPoll[0] == 1)
	{
		saveRegisterNames(0);
	}
	if (runPoll[1] == 1)
	{
		saveRegisterNames(1);
	}
	if (runPoll[2] == 1)
	{
		saveRegisterNames(2);
	}
	if (runPoll[3] == 1)
	{
		saveRegisterNames(3);
	}

	return 0;
}

void CMbPoll::OnDisplaytypeSigned()
{
	// TODO: Add your command handler code here
	//MessageBox(L"Signed");
	//CheckMenuItem(ID_DISPLAYTYPE_SIGNED, MF_CHECKED);
	/*MENUITEMINFO miinfo;
	miinfo.cbSize = sizeof(MENUITEMINFO);
	miinfo.fMask = MIIM_CHECKMARKS;
	miinfo.hbmpChecked = NULL;
	miinfo.hbmpUnchecked = NULL;
	SetMenuItemInfo(IDR_MENU_MBP, ID_DISPLAYTYPE_SIGNED, FALSE, miinfo);*/
	menuChecked = 0;
	staticDisplayType.SetWindowTextW(L"SIGNED");
	//checkUncheckMenus(menuChecked);
}

void CMbPoll::OnDisplaytypeUnsigned()
{
	// TODO: Add your command handler code here
	//MessageBox(L"Unsigned");
	menuChecked = 1;
	staticDisplayType.SetWindowTextW(L"UNSIGNED");
	//checkUncheckMenus(menuChecked);
}

void CMbPoll::OnDisplaytypeHex()
{
	// TODO: Add your command handler code here
	//MessageBox(L"Hex");
	menuChecked = 2;
	staticDisplayType.SetWindowTextW(L"HEX");
	//checkUncheckMenus(menuChecked);
}

void CMbPoll::OnDisplaytypeBinary()
{
	// TODO: Add your command handler code here
	//MessageBox(L"Binary");
	menuChecked = 3;
	staticDisplayType.SetWindowTextW(L"BINARY");
	//checkUncheckMenus(menuChecked);
}

void CMbPoll::checkUncheckMenus(int menuCheckedVar)
{
	switch(menuCheckedVar)
	{
	case 0:
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_SIGNED, MF_CHECKED | MF_BYCOMMAND);
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_UNSIGNED, MF_UNCHECKED | MF_BYCOMMAND);
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_HEX, MF_UNCHECKED | MF_BYCOMMAND);
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_BINARY, MF_UNCHECKED | MF_BYCOMMAND);
		break;
	case 1:
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_SIGNED, MF_UNCHECKED | MF_BYCOMMAND);
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_UNSIGNED, MF_CHECKED | MF_BYCOMMAND);
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_HEX, MF_UNCHECKED | MF_BYCOMMAND);
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_BINARY, MF_UNCHECKED | MF_BYCOMMAND);
		break;
	case 2:
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_SIGNED, MF_UNCHECKED | MF_BYCOMMAND);
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_UNSIGNED, MF_UNCHECKED | MF_BYCOMMAND);
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_HEX, MF_CHECKED | MF_BYCOMMAND);
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_BINARY, MF_UNCHECKED | MF_BYCOMMAND);
		break;
	case 3:
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_SIGNED, MF_UNCHECKED | MF_BYCOMMAND);
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_UNSIGNED, MF_UNCHECKED | MF_BYCOMMAND);
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_HEX, MF_UNCHECKED | MF_BYCOMMAND);
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_BINARY, MF_CHECKED | MF_BYCOMMAND);
		break;
	default:
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_SIGNED, MF_CHECKED | MF_BYCOMMAND);
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_UNSIGNED, MF_UNCHECKED | MF_BYCOMMAND);
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_HEX, MF_UNCHECKED | MF_BYCOMMAND);
		mbpMenu->CheckMenuItem(ID_DISPLAYTYPE_BINARY, MF_UNCHECKED | MF_BYCOMMAND);
		break;
	}
}

void CMbPoll::OnUpdateDisplaytypeSigned(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CMbPoll::OnUpdateDisplaytypeUnsigned(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CMbPoll::OnUpdateDisplaytypeHex(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CMbPoll::OnUpdateDisplaytypeBinary(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

#if 0

#if 1
void CMbPoll::OnFileSaveWhileClose()
{
	// TODO: Add your command handler code here
	CFile f;
	//CString strFilter = _T("Text file (*.txt)|*.txt|Rich Text file (*.rtf)|*.rtf|All files (*.*)|*.*||");
	CString temp;
	int qtyToShow[5];
	CMsflexgrid* grid;

	//CFileDialog dlg(true,_T("Save current Config"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
	//CFileDialog dlg(false, _T("Save current Config"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, strFilter);

	//if (dlg.DoModal() == IDOK)
	if (1)
	{
		//CString strConfigFilePathName = dlg.GetPathName();
		//MessageBox(strConfigFilePathName);
		//CString strConfigFileName = dlg.GetFileName();
		//MessageBox(strConfigFileName);
		CString strConfigFilePathName1 = g_strExePth+_T("savedConfig.txt");


		//f.Open(strConfigFilePathName1.GetString(), CFile::modeRead);
		BOOL a = f.Open(strConfigFilePathName1.GetString(), CFile::modeCreate | CFile::modeWrite);

		//CString str1;
		//str1 = L"Text1;Text2;Text3;";
		//WriteCString(f, str1);
		//f.Close();

#if SHOW_ALL
		for (int i = 0; i < 5; i++)
#endif
#if !SHOW_ALL
		for (int i = 0; i < 1; i++)
#endif
		{
			if (i == 0) grid = &ctrlGrid1;
			if (i == 1) grid = &ctrlGrid2;
			if (i == 2) grid = &ctrlGrid3;
			if (i == 3) grid = &ctrlGrid4;
			if (i == 4) grid = &ctrlGrid5;

			temp.Format(_T("%d"), i);				// grid number
			WriteCString(f, temp);
			temp.Format(_T("%d"), dataFlowStarted[i]);	// dataflow started
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollConfigured[i]);	// poll configured
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollSlaveId[i]);	// slave ID
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollFunction[i]);	// function
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollAddress[i]);	// address
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollQuantity[i]); // quantity
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollScanRate[i]); // scan rate
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollSingleFunction[i]); // single function
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollRows[i]);		// rows
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollDisplay[i]);	// display
			WriteCString(f, temp);

			qtyToShow[i] = ((pollRows[i] == 0) || (pollQuantity[i] < pollRows[i])) ? pollQuantity[i] : pollRows[i];
			for (int j = 0; j < qtyToShow[i]; j++)
			{
				//temp = grid->get_TextMatrix(j+1, 0);
				int rowNo = j % ROWS_OF_MBPOLLLIST;
				int colNo = (j / ROWS_OF_MBPOLLLIST) * 3;
				temp = mbPollList.GetItemText(rowNo, colNo);
				WriteCString(f, temp);
			}
		}
		f.Close();
	}
}
#endif

void CMbPoll::OnFileOpenWhileOpen()
{
	// TODO: Add your command handler code here

	int qtyToShow[5];
	CMsflexgrid* grid;
	CString strFilter = _T("Text file (*.txt)|*.txt|Rich Text file (*.rtf)|*.rtf|All files (*.*)|*.*||");
	CFile f;
	CString temp;

	//CFileDialog dlg(true, _T("Open current Config"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, strFilter);
	//if (dlg.DoModal() == IDOK)
	if (1)
	{
		 CString strConfigFilePathName1 = g_strExePth+_T("savedConfig.txt");
		 
		
		f.Open(strConfigFilePathName1.GetString(), CFile::modeRead);

#if SHOW_ALL
		for (int i = 0; i < 5; i++)
#endif
#if !SHOW_ALL
		for (int i = 0; i < 1; i++)
#endif
		{
			if (i == 0) grid = &ctrlGrid1;
			if (i == 1) grid = &ctrlGrid2;
			if (i == 2) grid = &ctrlGrid3;
			if (i == 3) grid = &ctrlGrid4;
			if (i == 4) grid = &ctrlGrid5;

			ReadCString(temp, f);		// grid number
			ReadCString(temp, f);		// dataflow started
			dataFlowStarted[i] = _wtoi(temp);
			ReadCString(temp, f);		// poll configured
			pollConfigured[i] = _wtoi(temp);
			ReadCString(temp, f);		// slave ID
			pollSlaveId[i] = _wtoi(temp);
			ReadCString(temp, f);		// function
			pollFunction[i] = _wtoi(temp);
			ReadCString(temp, f);		// address
			pollAddress[i] = _wtoi(temp);
			ReadCString(temp, f);		// quantity
			pollQuantity[i] = _wtoi(temp);
			ReadCString(temp, f);		// scan rate
			pollScanRate[i] = _wtoi(temp);
			ReadCString(temp, f);		// single function
			pollSingleFunction[i] = _wtoi(temp);
			ReadCString(temp, f);		// rows
			pollRows[i] = _wtoi(temp);
			ReadCString(temp, f);		// display
			pollDisplay[i] = _wtoi(temp);
			qtyToShow[i] = ((pollRows[i] == 0) || (pollQuantity[i] < pollRows[i])) ? pollQuantity[i] : pollRows[i];
			if (pollRows[i] == 0) grid->put_Rows(pollQuantity[i] + 1);
			if (pollRows[i] == 10) grid->put_Rows(11);
			if (pollRows[i] == 20) grid->put_Rows(21);
			if (pollRows[i] == 50) grid->put_Rows(51);
			if (pollRows[i] == 100) grid->put_Rows(101);
			for (int j = 0; j < qtyToShow[i]; j++)
			{
				ReadCString(temp, f);
				//grid->put_TextMatrix(j + 1, 0, temp);		// put_TextMatrix(row, col, text);
				int rowNo = j % ROWS_OF_MBPOLLLIST;
				int colNo = (j / ROWS_OF_MBPOLLLIST) * 3;
				mbPollList.SetItemText(rowNo, colNo, temp);
			}
			saveRegisterNames(i);
		}
		startStopBtnState();
		restoreConfigs();
		f.Close();
	}
}

#endif

#if 1
void CMbPoll::OnFileSaveWhileClose()
{
	// TODO: Add your command handler code here
	CFile f;
	//CString strFilter = _T("Text file (*.txt)|*.txt|Rich Text file (*.rtf)|*.rtf|All files (*.*)|*.*||");
	CString temp;
	int qtyToShow[5];
	CMsflexgrid* grid;

	//CFileDialog dlg(true,_T("Save current Config"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
	//CFileDialog dlg(false, _T("Save current Config"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, strFilter);

	//if (dlg.DoModal() == IDOK)
	if (1)
	{
		//CString strConfigFilePathName = dlg.GetPathName();
		//MessageBox(strConfigFilePathName);
		//CString strConfigFileName = dlg.GetFileName();
		//MessageBox(strConfigFileName);
		BOOL a = f.Open(L"LightingController\\savedConfig.txt", CFile::modeCreate | CFile::modeWrite);

		//CString str1;
		//str1 = L"Text1;Text2;Text3;";
		//WriteCString(f, str1);
		//f.Close();

#if SHOW_ALL
		for (int i = 0; i < 5; i++)
#endif
#if !SHOW_ALL
		for (int i = 0; i < 1; i++)
#endif
		{
			if (i == 0) grid = &ctrlGrid1;
			if (i == 1) grid = &ctrlGrid2;
			if (i == 2) grid = &ctrlGrid3;
			if (i == 3) grid = &ctrlGrid4;
			if (i == 4) grid = &ctrlGrid5;

			temp.Format(_T("%d"), i);				// grid number
			WriteCString(f, temp);
			temp.Format(_T("%d"), dataFlowStarted[i]);	// dataflow started
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollConfigured[i]);	// poll configured
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollSlaveId[i]);	// slave ID
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollFunction[i]);	// function
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollAddress[i]);	// address
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollQuantity[i]); // quantity
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollScanRate[i]); // scan rate
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollSingleFunction[i]); // single function
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollRows[i]);		// rows
			WriteCString(f, temp);
			temp.Format(_T("%d"), pollDisplay[i]);	// display
			WriteCString(f, temp);

			qtyToShow[i] = ((pollRows[i] == 0) || (pollQuantity[i] < pollRows[i])) ? pollQuantity[i] : pollRows[i];
			for (int j = 0; j < qtyToShow[i]; j++)
			{
				//temp = grid->get_TextMatrix(j+1, 0);
				int rowNo = j % ROWS_OF_MBPOLLLIST;
				int colNo = (j / ROWS_OF_MBPOLLLIST) * 3;
				temp = mbPollList.GetItemText(rowNo, colNo);
				WriteCString(f, temp);
			}
		}
		f.Close();
	}
}
#endif

void CMbPoll::OnFileOpenWhileOpen()
{
	// TODO: Add your command handler code here

	int qtyToShow[5];
	CMsflexgrid* grid;
	CString strFilter = _T("Text file (*.txt)|*.txt|Rich Text file (*.rtf)|*.rtf|All files (*.*)|*.*||");
	CFile f;
	CString temp;

	//CFileDialog dlg(true, _T("Open current Config"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, strFilter);
	//if (dlg.DoModal() == IDOK)
	if (1)
	{
		//CString strConfigFilePathName1 = dlg.GetPathName();
		//MessageBox(strConfigFilePathName1);
		//CString strConfigFileName1 = dlg.GetFileName();
		//MessageBox(strConfigFileName1);
		//f.Open(strConfigFileName1, CFile::modeRead);
		f.Open(L"LightingController\\savedConfig.txt", CFile::modeRead);

#if SHOW_ALL
		for (int i = 0; i < 5; i++)
#endif
#if !SHOW_ALL
		for (int i = 0; i < 1; i++)
#endif
		{
			if (i == 0) grid = &ctrlGrid1;
			if (i == 1) grid = &ctrlGrid2;
			if (i == 2) grid = &ctrlGrid3;
			if (i == 3) grid = &ctrlGrid4;
			if (i == 4) grid = &ctrlGrid5;

			ReadCString(temp, f);		// grid number
			ReadCString(temp, f);		// dataflow started
			dataFlowStarted[i] = _wtoi(temp);
			ReadCString(temp, f);		// poll configured
			pollConfigured[i] = _wtoi(temp);
			ReadCString(temp, f);		// slave ID
			pollSlaveId[i] = _wtoi(temp);
			ReadCString(temp, f);		// function
			pollFunction[i] = _wtoi(temp);
			ReadCString(temp, f);		// address
			pollAddress[i] = _wtoi(temp);
			ReadCString(temp, f);		// quantity
			pollQuantity[i] = _wtoi(temp);
			ReadCString(temp, f);		// scan rate
			pollScanRate[i] = _wtoi(temp);
			ReadCString(temp, f);		// single function
			pollSingleFunction[i] = _wtoi(temp);
			ReadCString(temp, f);		// rows
			pollRows[i] = _wtoi(temp);
			ReadCString(temp, f);		// display
			pollDisplay[i] = _wtoi(temp);
			qtyToShow[i] = ((pollRows[i] == 0) || (pollQuantity[i] < pollRows[i])) ? pollQuantity[i] : pollRows[i];
			if (pollRows[i] == 0) grid->put_Rows(pollQuantity[i] + 1);
			if (pollRows[i] == 10) grid->put_Rows(11);
			if (pollRows[i] == 20) grid->put_Rows(21);
			if (pollRows[i] == 50) grid->put_Rows(51);
			if (pollRows[i] == 100) grid->put_Rows(101);
			for (int j = 0; j < qtyToShow[i]; j++)
			{
				ReadCString(temp, f);
				//grid->put_TextMatrix(j + 1, 0, temp);		// put_TextMatrix(row, col, text);
				int rowNo = j % ROWS_OF_MBPOLLLIST;
				int colNo = (j / ROWS_OF_MBPOLLLIST) * 3;
				mbPollList.SetItemText(rowNo, colNo, temp);
			}
			saveRegisterNames(i);
		}
		startStopBtnState();
		restoreConfigs();
		f.Close();
	}
}

void CMbPoll::OnFunctionsWriteregisters()
{
	regDetailsOpenedFrom = 1;
	int ret = mbPollFunctions.DoModal();
	if (ret == IDOK)
	{
		
	}
}




