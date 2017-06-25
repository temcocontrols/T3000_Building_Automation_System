// TargetGrid.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrm.h"
#include "TemcoDataGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 

/////////////////////////////////////////////////////////////////////////////
// CTemcoDataGrid
#define  IDC_STATIC_NAME 5000
CTemcoDataGrid::CTemcoDataGrid()
{
	
}
void CTemcoDataGrid::Set_ParentWind(CWnd* pParent){
 m_pParent=pParent;
 m_ColName = new CStatic;
 m_ColName->Create(_T("my static"), WS_CHILD|WS_VISIBLE|SS_CENTER, 
	 CRect(10,10,150,50), m_pParent);
}
CTemcoDataGrid::~CTemcoDataGrid()
{
if (m_ColName!=NULL)
{
delete m_ColName;
m_ColName=NULL;
}
}

void CTemcoDataGrid::Initialize()
{
	
	
}



BEGIN_MESSAGE_MAP(CTemcoDataGrid, CWnd)
	//{{AFX_MSG_MAP(CTemcoDataGrid)
	ON_COMMAND(ID_CLEAR_CELL, OnClearCell)
	//}}AFX_MSG_MAP
//	ON_WM_CTLCOLOR()
//	ON_WM_MOUSEACTIVATE()
//ON_WM_RBUTTONDOWN()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CTemcoDataGrid, CMsflexgrid)
    //{{AFX_EVENTSINK_MAP(CTemcoDataGrid)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT_REFLECT(CTemcoDataGrid, -606 /* MouseMove */, OnMouseMove, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT_REFLECT(CTemcoDataGrid, -604 /* KeyUp */, OnKeyUp, VTS_PI2 VTS_I2)
	ON_EVENT_REFLECT(CTemcoDataGrid, -607 /* MouseUp */, OnMouseUp, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTemcoDataGrid message handlers

BOOL CTemcoDataGrid::OnKeyUp(short FAR* KeyCode, short Shift) 
{
	

	if (*KeyCode == VK_SPACE || *KeyCode == VK_DELETE)
	{
		v_ClearCell(get_Row(), get_Col(), TRUE);
	}
	
	return FALSE;
}


void CTemcoDataGrid::v_ClearCell(int row, int col, BOOL b_informSource)
{

	CString str_text =get_TextMatrix(row,col);
	
	if (!str_text.IsEmpty())
	{
	   put_TextMatrix(row,col,_T(""));
	 
	}
}


void CTemcoDataGrid::OnClearCell()
{
	v_ClearCell(get_Row(), get_Col(), TRUE);
}

//
BOOL CTemcoDataGrid::OnMouseUp(short Button, short Shift, long x, long y) 
{
	
	int row = get_MouseRow();
	int col = get_MouseCol();

	
	if (Button&0x2) // 2 - vbRightButton 
	{
			CMenu bar;
			if (bar.LoadMenu(IDR_ASSIGNMENT_MENU))
			{
				CMenu& popup = *bar.GetSubMenu(0);
				ASSERT(popup.m_hMenu != NULL);

				CPoint point(x,y);
				ClientToScreen(&point);

			
				popup.TrackPopupMenu(TPM_RIGHTBUTTON,
					point.x, point.y,
					this); // route commands through main window

			}
	}
	else if (Button&0x1)
	{
		int row = get_MouseRow();
		int col = get_MouseCol();

		  put_ColPosition(m_SelCurCol,col);
		put_BackColorSel(get_GridColorFixed()) ;
	 
		m_ColName->ShowWindow(SW_HIDE);
		//Write_GridCol();
	}
	return FALSE;
}
//void CTemcoDataGrid::Write_GridCol(){
//int Cols=get_Cols();
//	CString name;
//	for (int i=0;i<Cols;i++)
//	{
//		name=get_TextMatrix(0,i);
//		m_Inifile.WriteProfileInt(m_Section,name,i);
//	}
//}
BOOL CTemcoDataGrid::OnMouseMove(short Button, short Shift, long x, long y) 
{
	//  Add your control  handler code here
	//ACE(_T("Mouse move\n"));

	if (Button&0x1) // 1 - vbLeftButton 
	{
	    int col = get_MouseCol();
		CRect rect;
		GetWindowRect(&rect);                 //获取FlexGrid控件的窗口矩形
		m_pParent->ScreenToClient(&rect);                 //转换为客户区矩形
		CDC* pDC=GetDC();
		//MSFlexGrid 控件的函数的长度单位是“缇(twips)”，需要将其转化为像素，1440 缇 = 1 英寸
		//计算象素点和缇的转换比例
		int nTwipsPerDotX=1440/pDC->GetDeviceCaps(LOGPIXELSX);
		int nTwipsPerDotY=1440/pDC->GetDeviceCaps(LOGPIXELSY);

		long y = get_RowPos(0)/nTwipsPerDotY;
		long x = get_ColPos(col)/nTwipsPerDotX;
		//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
		long width = get_ColWidth(m_SelCurCol)/nTwipsPerDotX+1;
		long height = get_RowHeight(m_SelCurRow)/nTwipsPerDotY+1;
		//形成选中个所在的矩形区域
		CRect rcCell(x,y,x+width,y+height);
		//转换成相对对话框的坐标
		rcCell.OffsetRect(rect.left+1,rect.top+1);
		m_ColName->MoveWindow(rcCell);
		m_ColName->ShowWindow(SW_SHOW);
	  put_BackColorSel(get_GridColor());  
	}

	return FALSE;
}


//HBRUSH CTemcoDataGrid::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CMsflexgrid::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	
//	switch(m_pParent->GetDlgCtrlID())
//	{
//	case IDC_STATIC_NAME:
//		{
//			pDC->SetTextColor(RGB(255,255,255));
//			pDC->SetBkColor(RGB(0,255,255));
//			pDC->SetBkMode(TRANSPARENT);
//			HBRUSH B = CreateSolidBrush(RGB(0,0,0));
//			return (HBRUSH)B;
//		}
//	}
//	
//
//	//return hbr;
//}


//int CTemcoDataGrid::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
//{
//	 
//
//	return CMsflexgrid::OnMouseActivate(pDesktopWnd, nHitTest, message);
//}


//void CTemcoDataGrid::OnRButtonDown(UINT nFlags, CPoint point)
//{
//	 
//	
//	CMsflexgrid::OnRButtonDown(nFlags, point);
//}


void CTemcoDataGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	 
	m_SelCurCol=get_MouseCol();
	m_SelCurRow=get_MouseRow();

	m_ColName = new CStatic;
	m_ColName->Create(_T("my static"), WS_CHILD|WS_VISIBLE|SS_CENTER, 
		CRect(10,10,150,50), m_pParent);
	m_CurColName=get_TextMatrix(0,m_SelCurCol); 


		
	   m_ColName->SetWindowText(m_CurColName);
 put_BackColorSel(get_GridColorFixed());
	CMsflexgrid::OnLButtonDown(nFlags, point);
}
