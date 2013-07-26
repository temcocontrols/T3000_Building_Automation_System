// GraphicView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "MainFrm.h"
#include "GraphicView.h"
#include "LabelEditDlg.h"
#include "ApplyGraphicLabelsDlg.h"
#include "AfxMessageDialog.h"

// CGraphicView
#define XStart 10
#define YStart 70

BOOL g_bRefresh=FALSE;
IMPLEMENT_DYNCREATE(CGraphicView, CFormView)

CGraphicView::CGraphicView()
	: CFormView(CGraphicView::IDD)
{
	m_LbtDown=TRUE;
	m_nFoucsIndext=-1;
	m_bImgExist=FALSE;
}

CGraphicView::~CGraphicView()
{
}

void CGraphicView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DELBUTTON, m_delMenuBtn);
	DDX_Control(pDX, IDC_LOCKCHECK, m_bLockBtn);
	DDX_Control(pDX, IDC_IMGCNFIGBUTTON, m_ImgConfigBtn);
}

BEGIN_MESSAGE_MAP(CGraphicView, CFormView)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_ADDBUTTON, &CGraphicView::OnBnClickedAddbutton)
	ON_BN_CLICKED(IDC_EDTBUTTON, &CGraphicView::OnBnClickedEdtbutton)
	ON_BN_CLICKED(IDC_DELBUTTON, &CGraphicView::OnBnClickedDelbutton)
	ON_BN_CLICKED(IDC_APPLYBUTTON, &CGraphicView::OnBnClickedApplybutton)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_GOBACKBUTTON, &CGraphicView::OnBnClickedGobackbutton)
	ON_BN_CLICKED(IDC_IMGCNFIGBUTTON, &CGraphicView::OnBnClickedImgcnfigbutton)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CGraphicView diagnostics

#ifdef _DEBUG
void CGraphicView::AssertValid() const
{
	CFormView::AssertValid();
}
#endif //_DEBUG

void CGraphicView::InitGraphic(int nSerialNum,int nTstatID)
{
//	register_critical_section.Lock();
//	memcpy((unsigned short*)&(multi_register_value[0]),(unsigned short*)&multi_register_value[0],sizeof(multi_register_value));
//	register_critical_section.Unlock();
	//m_strImgPathName=g_strImagePathName;
	m_strImgPathName=g_strImgeFolder+g_strImagePathName;

	WIN32_FIND_DATA fData;
	HANDLE hFile=NULL;
	hFile = FindFirstFile(m_strImgPathName,&fData);
	if(hFile != INVALID_HANDLE_VALUE)
	{
	//	m_delMenuBtn.ShowWindow(SW_SHOW);
	//	GetDlgItem(IDC_ADDBUTTON)->ShowWindow(SW_SHOW);
	//	GetDlgItem(IDC_EDTBUTTON)->ShowWindow(SW_SHOW);
	//	GetDlgItem(IDC_APPLYBUTTON)->ShowWindow(SW_SHOW);
		m_bImgExist=TRUE;
		FindClose(hFile);
	}
	else
	{
		m_bImgExist=FALSE;
	//	m_delMenuBtn.ShowWindow(SW_HIDE);
	//	GetDlgItem(IDC_ADDBUTTON)->ShowWindow(SW_HIDE);
	//	GetDlgItem(IDC_EDTBUTTON)->ShowWindow(SW_HIDE);
	//	GetDlgItem(IDC_APPLYBUTTON)->ShowWindow(SW_HIDE);
		FindClose(hFile);
		//�Զ�ɾ����ؼ�¼����Ϊͼ�󲻴��ڣ�Labelû���κ���˼

	}
	m_nSerialNumber=nSerialNum;
	m_nTstatID=nTstatID;
	//m_strScreenName=strScreenName;


	
	ReloadLabelsFromDB();
	this->SetFocus();
}
// CGraphicView message handlers

void CGraphicView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CMemDC memDC(dc,this);
	CRect rcClient;
	GetClientRect(&rcClient);
	//memDC.GetDC().FillSolidRect(&rcClient,::GetSysColor (COLOR_3DLIGHT));
//	if(g_strImagePathName.IsEmpty())
	if(!m_bImgExist)
		PainNoImageInfo(&memDC.GetDC());
	else
	{	
		//	CMemDC memDC(dc,this);
			memDC.GetDC().FillSolidRect(&rcClient,RGB(202,208,216));
			Graphics graphics(memDC.GetDC());
			Bitmap bitmap(m_strImgPathName);
			graphics.DrawImage(&bitmap,XStart,YStart,bitmap.GetWidth(),bitmap.GetHeight());

	}
	for(int i=0;i<(int)m_RelayLabelLst.size();i++)
	{
		if(i==m_nFoucsIndext&& !m_LbtDown)
		{
			CRect rcItem;
			m_RelayLabelLst.at(i).plabelCtrl->GetWindowRect(&rcItem);
			ScreenToClient(&rcItem);
			rcItem.DeflateRect(-1,-1,-1,-1);
			HBRUSH hOldBrush;
				
			HPEN hOldPen;
			hOldBrush=(HBRUSH)memDC.GetDC().SelectObject(GetStockObject(NULL_BRUSH));
			CPen pen(1,2,RGB(0,0,201));
			hOldPen=(HPEN)memDC.GetDC().SelectObject(pen);
			memDC.GetDC().Rectangle(rcItem);
			memDC.GetDC().SelectObject(hOldBrush);
			memDC.GetDC().SelectObject(hOldPen);
						
		//	m_RelayLabelLst.at(i).plabelCtrl->SetWindowText(_T(""));

		}
		Label_information label;
		label=m_RelayLabelLst.at(i);

		//m_RelayLabelLst.at(i).plabelCtrl->SetLabelInfo(label.tstat_id,label.input_or_output,label.status,label.clrTxt,label.bkColor);

		label.plabelCtrl->SetLabelInfo(label.tstat_id,label.input_or_output,label.status,label.clrTxt,label.bkColor);

	}
	//m_Label.ShowWindow(SW_SHOW);

//	m_Label.SetRedraw(1);
	// TODO: Add your message handler code here
	// Do not call CFormView::OnPaint() for painting messages
}
void CGraphicView::PainNoImageInfo(CDC* pDC)
{
	const int iOffset = 20;
	CString strInfo=_T("No image has been configed for current TSTAT, please config one first. ");

	CFont* pFontOld = (CFont*) pDC->SelectStockObject (DEFAULT_GUI_FONT);
	ASSERT (pFontOld != NULL);

	CRect rectClient;
	GetClientRect (&rectClient);
	pDC->FillSolidRect(&rectClient,RGB(238,245,250));

	CRect rectText = rectClient;
	rectText.DeflateRect (iOffset, iOffset);
	pDC->DrawText (strInfo, rectText, DT_CALCRECT | DT_WORDBREAK);

	rectText.OffsetRect (	(rectClient.Width () - rectText.Width () - 2 * iOffset) / 2,
							(rectClient.Height () - rectText.Height () - 2 * iOffset) / 2);

	CRect rectFrame = rectText;
	rectFrame.InflateRect (iOffset, iOffset);

	pDC->FillSolidRect (rectFrame, ::GetSysColor (COLOR_INFOBK));
	
	rectFrame.DeflateRect (1, 1);
	pDC->Draw3dRect (rectFrame, ::GetSysColor (COLOR_3DSHADOW), 
					::GetSysColor (COLOR_3DLIGHT));

	rectFrame.DeflateRect (2, 2);
	pDC->Draw3dRect (rectFrame, ::GetSysColor (COLOR_3DSHADOW), 
					::GetSysColor (COLOR_3DLIGHT));

	pDC->SetTextColor (::GetSysColor (COLOR_INFOTEXT));
	pDC->SetBkMode (TRANSPARENT);

	pDC->DrawText (strInfo, rectText, DT_WORDBREAK);

	pDC->SelectObject (pFontOld);

}

BOOL CGraphicView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default


	return 1;//CFormView::OnEraseBkgnd(pDC);
}

void CGraphicView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	int n=HitTestEx(point);
	if(n>=0)
		m_nFoucsIndext=n;
	else
		return;
	//int nstate=m_bLockBtn.GetCheck();
	if(m_bLockBtn.GetCheck()==BST_CHECKED)
		return;
	m_LbtDown=TRUE;
	SetCapture();

	CFormView::OnLButtonDown(nFlags, point);
}

void CGraphicView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_LbtDown&&m_nFoucsIndext>=0)
	{
		saveLabelInfo(m_nFoucsIndext);
	}

	m_LbtDown=FALSE;
	Invalidate();
	ReleaseCapture();

	CFormView::OnLButtonUp(nFlags, point);
}

void CGraphicView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if(m_bLockBtn.GetCheck()==BST_CHECKED)
		return;
	if(m_nFoucsIndext>=0&&m_LbtDown)
	{
		CRect rcItem;
		m_RelayLabelLst.at(m_nFoucsIndext).plabelCtrl->GetWindowRect(&rcItem);
		CRect rcNew;
		rcNew.left=point.x - rcItem.Width()/2;
		rcNew.top=point.y - rcItem.Height()/2;
		rcNew.right=point.x + rcItem.Width()/2;
		rcNew.bottom=point.y + rcItem.Height()/2;
		m_RelayLabelLst.at(m_nFoucsIndext).plabelCtrl->MoveWindow(&rcNew,1);
		//ScreenToClient(&rcItem);

	}
	CFormView::OnMouseMove(nFlags, point);
}
int CGraphicView::HitTestEx(CPoint & point)
{
	for(int i=0;i<(int)m_RelayLabelLst.size();i++)
	{
		CRect rcItem;
		m_RelayLabelLst.at(i).plabelCtrl->GetWindowRect(&rcItem);
		ScreenToClient(&rcItem);
		if (rcItem.PtInRect(point))
		{

		//	m_nFoucsIndext=i;
			return i;
		}
	}
	return -1;
}
INT_PTR CGraphicView::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	// TODO: Add your specialized code here and/or call the base class
/*
	int nIndext=-1;
	int i=0;
	for( i=0;i<m_RelayLabelLst.size();i++)
	{
		CRect rcItem;
		m_RelayLabelLst.at(i).plabelCtrl->GetWindowRect(&rcItem);
		ScreenToClient(&rcItem);
		if (rcItem.PtInRect(point))
		{
			nIndext=i;
			break;
		}
	}
	pTI->uId=m_RelayLabelLst.at(i).cstatic_id;
	pTI->lpszText=_T("hello");
	*/


	return CFormView::OnToolHitTest(point, pTI);
}

void CGraphicView::OnBnClickedAddbutton()
{
	if(g_GraphicModelevel==1)
		return;
	if(!m_bImgExist)
	{
		AfxMessageBox(_T("No image has been configed for current TSTAT, please config one first!"));
		return;
	}
	g_bRefresh=FALSE;
	CLabelEditDlg dlg;
	dlg.SetCommandType(1);
	dlg.SetInitValueForEdit(m_nTstatID,m_nSerialNumber,m_strScreenName,0);
	dlg.DoModal();
	if(g_bRefresh)
	{
		ReloadLabelsFromDB();
		Invalidate();
	}

}

void CGraphicView::OnBnClickedEdtbutton()
{
	if(g_GraphicModelevel==1)
		return;

	if(m_nFoucsIndext<0)
	{
		AfxMessageBox(_T("Please Select A Label Item First!"));
		return;
	}
	g_bRefresh=FALSE;
	Label_information labelInfo;
	labelInfo=m_RelayLabelLst.at(m_nFoucsIndext);
	CLabelEditDlg dlg;
	dlg.SetCommandType(2);
	dlg.SetInitValueForEdit(labelInfo.tstat_id,labelInfo.nSerialNum,m_strScreenName,labelInfo.cstatic_id,labelInfo.input_or_output,labelInfo.width,labelInfo.height,labelInfo.status,labelInfo.clrTxt,labelInfo.bkColor);
	dlg.DoModal();
	if(g_bRefresh)
	{
		ReloadLabelsFromDB();
		Invalidate();
	}

}

void CGraphicView::OnBnClickedDelbutton()
{
	if(g_GraphicModelevel==1)
		return;
	switch (m_delMenuBtn.m_nMenuResult)
	{
	case ID_DELLABEL:
		{
			if(m_nFoucsIndext<0)
			{
				AfxMessageBox(_T("Please select a label item first!"));
				return;
			}

			if(AfxMessageBox(_T("Do you really want to delete current label?"),MB_YESNO)==IDYES)
			{
					//AfxMessageBox(_T("Delete it"));

				m_pCon.CreateInstance(_T("ADODB.Connection"));
				m_pRs.CreateInstance(_T("ADODB.Recordset"));
				m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

				Label_information	label;
				label=m_RelayLabelLst.at(m_nFoucsIndext);

				try
				{



				CString strSql;
				strSql.Format(_T("delete * from Screen_Label where Serial_Num =%i and Tstat_id=%i and  Cstatic_id=%i"),label.nSerialNum,label.tstat_id,label.cstatic_id);
				m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}
				if(m_pRs->State) 
				m_pRs->Close(); 

				ReloadLabelsFromDB();
				Invalidate();
				m_nFoucsIndext=-1;
			}
		}

		break;

	case ID_DELALLLABELS:
		{		
			m_pCon.CreateInstance(_T("ADODB.Connection"));
			m_pRs.CreateInstance(_T("ADODB.Recordset"));
			m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

			try
			{


			CString strSql;
			strSql.Format(_T("delete * from Screen_Label where Serial_Num =%i and Tstat_id=%i"),m_nSerialNumber,m_nTstatID);
			m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}		
			if(m_pRs->State) 
				m_pRs->Close(); 

			ReloadLabelsFromDB();
			Invalidate();
			m_nFoucsIndext=-1;
		}
		break;
	}

	// TODO: Add your control notification handler code here
}

void CGraphicView::OnBnClickedApplybutton()
{
	if(g_GraphicModelevel==1)
		return;

	if(m_RelayLabelLst.size()<=0)
	{
		AfxMessageBox(_T("There is no lable in current graphic view"));
		return;
	}
	ApplyGraphicLabelsDlg Dlg;
	Dlg.SetLabelLst(&m_RelayLabelLst);
	Dlg.DoModal();
}
void CGraphicView::ClearAllLabels()
{
	if(g_GraphicModelevel==1)
		return;
	for(int i=0;i<(int)m_RelayLabelLst.size();i++)
	{
		if(m_RelayLabelLst.at(i).plabelCtrl!=NULL&&m_RelayLabelLst.at(i).plabelCtrl->m_hWnd!=NULL)
		{
			m_RelayLabelLst.at(i).plabelCtrl->DestroyWindow();
			delete m_RelayLabelLst.at(i).plabelCtrl;
		}
		
	}
	m_RelayLabelLst.clear();
}
void CGraphicView::ReloadLabelsFromDB()
{
	ClearAllLabels();

	m_pCon.CreateInstance(_T("ADODB.Connection"));
	m_pRs.CreateInstance(_T("ADODB.Recordset"));
	m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);
	CString strSql;
	strSql.Format(_T("select * from Screen_Label where Serial_Num =%i and Tstat_id=%i"),m_nSerialNumber,m_nTstatID);
	//strSql.Format(_T("select * from Screen_Label where Tstat_id=%i"),m_nTstatID);
	m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
	CString strtemp;
	strtemp.Empty();

	_variant_t temp_variant;
	int nTemp;
	CString strTemp;
	while(VARIANT_FALSE==m_pRs->EndOfFile)
	{	
		Label_information label;

		label.nSerialNum=m_nSerialNumber;
		label.tstat_id=m_nTstatID;
		label.strScreenName=m_strScreenName;

		nTemp=m_pRs->GetCollect("Cstatic_id");//
		label.cstatic_id=nTemp;

		nTemp=m_pRs->GetCollect("Point_X");//
		label.point.x=nTemp;

		nTemp=m_pRs->GetCollect("Point_Y");//
		label.point.y=nTemp;

		nTemp=m_pRs->GetCollect("Height");//
		label.height=nTemp;

		nTemp=m_pRs->GetCollect("Width");//
		label.width=nTemp;

		nTemp=m_pRs->GetCollect("Status");//
		label.status=nTemp;
		
		temp_variant=m_pRs->GetCollect("Tips");//
		if(temp_variant.vt!=VT_NULL)
			strtemp=temp_variant;
		else
			strtemp=_T("");
		label.strTips=strtemp;
		
		/*
		temp_variant=m_pRs->GetCollect("Tips");//
		if(temp_variant.vt!=VT_NULL)
			strtemp=temp_variant;
		else
			strtemp=_T("");
		label.strTips=strtemp;
		*/


		nTemp=m_pRs->GetCollect("Input_or_Output");//
		label.input_or_output=nTemp;

		
		strTemp=m_pRs->GetCollect("Text_Color");//
		if(strTemp.IsEmpty())
		{
			label.clrTxt=0;
		}
		else
		{
			label.clrTxt=_wtol(strTemp);
		}
		
		
		strTemp=m_pRs->GetCollect("Back_Color");//
		if(strTemp.IsEmpty())
		{
			label.bkColor=254*255*255;
		}
		else
		{
				label.bkColor=_wtol(strTemp);
		}
	//	label.bkColor=nTemp;
		CRelayLabel* pLabel=new(CRelayLabel);

		pLabel->Create(_T(""),WS_CHILD|WS_VISIBLE|SS_CENTER,CRect(XStart+label.point.x,YStart+label.point.y,XStart+label.point.x+label.width,YStart+label.point.y+label.height), this,label.cstatic_id);

		//(int TstatID,int input_or_output,int nStatus,COLORREF textClr,COLORREF bkClr)
		pLabel->SetLabelInfo(label.tstat_id,label.input_or_output,label.status,label.clrTxt,label.bkColor);
		pLabel->MoveWindow(CRect(XStart+label.point.x,YStart+label.point.y,XStart+label.point.x+label.width,YStart+label.point.y+label.height));
		label.plabelCtrl=pLabel;
		m_RelayLabelLst.push_back(label);

		m_pRs->MoveNext();

	}

	if(m_pRs->State) 
		m_pRs->Close(); 
	if(m_pCon->State)
		m_pCon->Close();
	
}
void CGraphicView::OnDestroy()
{
	CFormView::OnDestroy();

	ClearAllLabels();
	// TODO: Add your message handler code here
}

void CGraphicView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	if(g_GraphicModelevel==1)
	{
		CAfxMessageDialog dlg;
		CString strPromption;
		strPromption.LoadString(IDS_STRNOPRIVILEGE);
		dlg.SetPromtionTxt(strPromption);
		dlg.DoModal();
		return;
	}
	m_menu.LoadMenu(IDR_DELEBTNMENU);
	m_delMenuBtn.m_hMenu = m_menu.GetSubMenu(0)->GetSafeHmenu();
	m_delMenuBtn.m_bOSMenu = FALSE;

	m_ImgConfigmenu.LoadMenu(IDR_IMGCONFIGMENU);
	m_ImgConfigBtn.m_hMenu = m_ImgConfigmenu.GetSubMenu(0)->GetSafeHmenu();
	m_ImgConfigBtn.m_bOSMenu = FALSE;

	m_bLockBtn.SetImage(IDB_CHECKNO_BITMAP);
	m_bLockBtn.SetCheckedImage(IDB_CHECK_BITMAP);
//	m_bLockBtn.SizeToContent();
	m_bLockBtn.m_nFlatStyle = CMFCButton::BUTTONSTYLE_SEMIFLAT;
	m_bLockBtn.SetCheck(BST_CHECKED);
}

void CGraphicView::OnBnClickedGobackbutton()
{
	((CMainFrame*)(theApp.m_pMainWnd))->SwitchToPruductType(DLG_T3000_VIEW);
}
void CGraphicView::saveLabelInfo(int nItem)
{
	if(g_GraphicModelevel==1)
		return;
	if(nItem<0)
		return;
	if(nItem>=(int)m_RelayLabelLst.size())
		return;
	m_pCon.CreateInstance(_T("ADODB.Connection"));
	m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

	Label_information label;
	label=m_RelayLabelLst.at(nItem);
	CRect rcItem;
	label.plabelCtrl->GetWindowRect(&rcItem);
	ScreenToClient(&rcItem);
	int nLeft,nTop;
	nLeft=rcItem.left-XStart;
	nTop=rcItem.top-YStart;
	try
	{


	CString strSql;
	strSql.Format(_T("update Screen_Label set Point_X=%i,Point_Y=%i where Serial_Num =%i and Tstat_id=%i and  Cstatic_id=%i"),
		nLeft,nTop,label.nSerialNum,label.tstat_id,label.cstatic_id);
	m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	if(m_pCon->State)
		m_pCon->Close();
}

void CGraphicView::OnBnClickedImgcnfigbutton()
{
	switch (m_ImgConfigBtn.m_nMenuResult)
	{
	case ID_IMGCONFIG_DELETE:
		{
			
			if(m_nSerialNumber<=0)
			{
				AfxMessageBox(_T("Unavailable a Tstat!"));
				return;
			}
			WIN32_FIND_DATA fData;
			HANDLE hFile=NULL;
			hFile = FindFirstFile(m_strImgPathName,&fData);
			if(hFile != INVALID_HANDLE_VALUE)
			{
			   CFile::Remove(m_strImgPathName);
			}	
			FindClose(hFile);

			_ConnectionPtr tmpCon;
			_RecordsetPtr tmppRs;
			tmpCon.CreateInstance(_T("ADODB.Connection"));
			tmppRs.CreateInstance(_T("ADODB.Recordset"));
			tmpCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);
			try
			{


			CString strImgFileName;
			strImgFileName=_T("");
			CString strSql;
			strSql.Format(_T("update ALL_NODE set Background_imgID ='%s' where Serial_ID = '%d'"),strImgFileName,m_nSerialNumber);
			tmpCon->Execute(strSql.GetString(),NULL,adCmdText);
			if(tmppRs->State) 
				tmppRs->Close(); 
			if(tmpCon->State)
				tmpCon->Close();	
			g_strImagePathName=strImgFileName;
			m_strImgPathName=g_strImgeFolder+g_strImagePathName;
			InitGraphic(m_nSerialNumber,m_nTstatID);
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}

			Invalidate();
		}
		break;

	case ID_IMGCONFIG_AND:
		
		{
		if(m_nSerialNumber<=0)
		{
			AfxMessageBox(_T("Unavailable a Tstat!"));
			return;
		}
		CString strFilter = _T("bmp file|*.bmp|jpg file|*.jpg|png file|*.png|tif file|*.tif|all file|*.*||");
		CFileDialog dlg(true,_T("Open image file"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
		if(IDOK==dlg.DoModal())
		{
			CString strImgFilePathName=dlg.GetPathName();
			CString strImgFileName=dlg.GetFileName();
			CString strDestFileName=g_strImgeFolder+strImgFileName;

			CopyFile(strImgFilePathName,strDestFileName,FALSE);
			_ConnectionPtr tmpCon;
			_RecordsetPtr tmppRs;
			tmpCon.CreateInstance(_T("ADODB.Connection"));
			tmppRs.CreateInstance(_T("ADODB.Recordset"));
			tmpCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

			try
			{

			CString strSql;
			strSql.Format(_T("update ALL_NODE set Background_imgID ='%s' where Serial_ID = '%d'"),strImgFileName,m_nSerialNumber);
			tmpCon->Execute(strSql.GetString(),NULL,adCmdText);
			if(tmppRs->State) 
				tmppRs->Close(); 
			if(tmpCon->State)
				tmpCon->Close();

			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
			g_strImagePathName=strImgFileName;
			m_strImgPathName=g_strImgeFolder+g_strImagePathName;
			InitGraphic(m_nSerialNumber,m_nTstatID);
			Invalidate();
		}
		break;
		}
	}
}

void CGraphicView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CFormView::OnTimer(nIDEvent);
}
