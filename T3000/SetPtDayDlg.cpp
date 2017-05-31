// SetPtDayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "SetPtDayDlg.h"


// CSetPtDayDlg dialog

IMPLEMENT_DYNAMIC(CSetPtDayDlg, CDialog)

CSetPtDayDlg::CSetPtDayDlg(CView* pParent /*=NULL*/)
	: CDialog(CSetPtDayDlg::IDD, pParent)
{
	m_pSetPtCtrl=new CSetptGraphicBar();
	m_pBkImg=NULL;
	m_pThumbImg=NULL;
	m_pNightSetPt=new CSetptGraphicBar();
	
}

CSetPtDayDlg::~CSetPtDayDlg()
{

}

void CSetPtDayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetPtDayDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CSetPtDayDlg message handlers

BOOL CSetPtDayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pSetPtCtrl->CreatSliderBar(CRect(30,50,120,400),0,this->m_hWnd);
	m_pNightSetPt->CreatSliderBar(CRect(220,50,310,400),1,this->m_hWnd);
	GetClientRect(&m_ClientRC);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSetPtDayDlg::OnPaint()
{
	
	CPaintDC dc(this); // device context for painting
	GetClientRect(&m_ClientRC);
	COwnMemDC memDC(&dc,m_ClientRC);
	memDC.FillSolidRect(&m_ClientRC,RGB(229,229,229));
	//CMemDC memDC(dc,this);
	m_pSetPtCtrl->DrawGraphicBar(&memDC);
	m_pNightSetPt->DrawGraphicBar(&memDC);
	
	if(m_pBkImg==NULL)
	{
		
		CString strbkImge=g_strExePth+_T("res\\setpt.jpg");
		m_pBkImg=Bitmap::FromFile(strbkImge);
		CString strThumbImge=g_strExePth+_T("res\\SetptThumb.jpg");
		m_pThumbImg=Bitmap::FromFile(strThumbImge);
		if(m_pBkImg!=NULL)
		{

			m_pSetPtCtrl->SetImg(m_pBkImg,m_pThumbImg);
			m_pNightSetPt->SetImg(m_pBkImg,m_pThumbImg);
		}
		
	}	
	if(m_pSetPtCtrl->m_pBkImg!=NULL)
	{	
		m_pSetPtCtrl->DrawGraphicBar(&memDC);
	
	}
	if(m_pNightSetPt->m_pBkImg!=NULL)
	{		m_pNightSetPt->DrawGraphicBar(&memDC);

	}
		
	



	

}
void CSetPtDayDlg::LoadImage()
{
	//CString strbkImge=g_strExePth+_T("res\\setpt.jpg");
	//m_pBkImg=Bitmap::FromFile(strbkImge);
	//int nw=m_pBkImg->GetWidth();
}

void CSetPtDayDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	 
	m_pSetPtCtrl->OnLButtonDown(nFlags,point);
	m_pNightSetPt->OnLButtonDown(nFlags,point);
	CDialog::OnLButtonDown(nFlags, point);
}

void CSetPtDayDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	 
	m_pSetPtCtrl->OnLButtonUp(nFlags,point);
	m_pNightSetPt->OnLButtonUp(nFlags,point);
	CDialog::OnLButtonUp(nFlags, point);
}

void CSetPtDayDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	 
	m_pSetPtCtrl->OnMouseMove(nFlags,point);
	m_pNightSetPt->OnMouseMove(nFlags,point);
	CDialog::OnMouseMove(nFlags, point);
}

BOOL CSetPtDayDlg::OnEraseBkgnd(CDC* pDC)
{
	 

	return 1;
	return CDialog::OnEraseBkgnd(pDC);
}

BOOL CSetPtDayDlg::PreTranslateMessage(MSG* pMsg)
{
	//  Add your specialized code here and/or call the base clas	
	if(pMsg->message == WM_KEYDOWN  )
	{
		if(pMsg->wParam == VK_RETURN||pMsg->wParam == VK_ESCAPE)
		{
			return true;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
