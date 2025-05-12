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
		//自动删除相关记录，因为图象不存在，Label没有任何意思

	}
	m_nSerialNumber=nSerialNum;
	m_nTstatID=nTstatID;
	//m_strScreenName=strScreenName;


	
	ReloadLabelsFromDB();
	this->SetFocus();
	SetTimer(1,7000,NULL);//此定时器是用于刷新 Label的;
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
		if(bac_cm5_graphic == false)
		label.plabelCtrl->SetLabelInfo(label.tstat_id,label.input_or_output,label.status,label.clrTxt,label.bkColor);
		else
		label.plabelCtrl->SetLabelInfo_General(label.tstat_id,label.input_or_output,label.status,label.clrTxt,label.bkColor);
	}
	//m_Label.ShowWindow(SW_SHOW);

//	m_Label.SetRedraw(1);
	
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
	 


	return 1;//CFormView::OnEraseBkgnd(pDC);
}

void CGraphicView::OnLButtonDown(UINT nFlags, CPoint point)
{
	 

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
//int CGraphicView::HitTestEx(CPoint & point)
//{
//	for (int i=0;i<(int)m_RelayLabelLst.size();i++)
//	{
//		CRect rcItem;
//		m_RelayLabelLst.at(i).plabelCtrl->GetWindowRect(&rcItem);
//		ScreenToClient(&rcItem);
//		if(rcItem.PtInRect(point))
//		{
//			return i;
//		}
//	}
//}
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

		SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
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

				

				Label_information	label;
				label=m_RelayLabelLst.at(m_nFoucsIndext);

				try
				{



				CString strSql;
				strSql.Format(_T("delete   from Screen_Label where Serial_Num =%i and Tstat_id=%i and  Cstatic_id=%i"),label.nSerialNum,label.tstat_id,label.cstatic_id);
				 SqliteDBT3000.execDML((UTF8MBSTR)strSql);
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}
			 

				ReloadLabelsFromDB();
				Invalidate();
				m_nFoucsIndext=-1;
			}
		}

		break;

	case ID_DELALLLABELS:
		{		
			

			try
			{


			CString strSql;
			strSql.Format(_T("delete   from Screen_Label where Serial_Num =%i and Tstat_id=%i"),m_nSerialNumber,m_nTstatID);
			 SqliteDBT3000.execDML((UTF8MBSTR)strSql);
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}		
			 

			ReloadLabelsFromDB();
			Invalidate();
			m_nFoucsIndext=-1;
		}
		break;
	}
	SqliteDBT3000.closedb();
	
}

void CGraphicView::OnBnClickedApplybutton()
{
	if(g_GraphicModelevel==1)
		return;

	if(m_RelayLabelLst.size()<=0)
	{
		AfxMessageBox(_T("There is no label in current graphic view"));
		return;
	}
	ApplyGraphicLabelsDlg Dlg;
	Dlg.SetLabelLst(&m_RelayLabelLst);
	Dlg.DoModal();
}
void CGraphicView::ClearAllLabels()
{
	m_graphic_refresh_data.clear();
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
	
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
	CString strSql;
	//if(bac_cm5_graphic == false)
	strSql.Format(_T("select * from Screen_Label where Serial_Num =%i and Tstat_id=%i"),m_nSerialNumber,m_nTstatID);
	 
	q = SqliteDBT3000.execQuery((UTF8MBSTR)strSql);
	CString strtemp;
	strtemp.Empty();

	_variant_t temp_variant;
	int nTemp;
	CString strTemp;
	int nItem = 0;//用于记录有多少个需要刷新;
	while(!q.eof())
	{	
		Label_information label;

		label.nSerialNum=m_nSerialNumber;
		label.tstat_id=m_nTstatID;
		label.strScreenName=m_strScreenName;

		nTemp=q.getIntField("Cstatic_id");//
		label.cstatic_id=nTemp;

		nTemp=q.getIntField("Point_X");//
		label.point.x=nTemp;

		nTemp=q.getIntField("Point_Y");//
		label.point.y=nTemp;

		nTemp=q.getIntField("Height");//
		label.height=nTemp;

		nTemp=q.getIntField("Width");//
		label.width=nTemp;

		nTemp=q.getIntField("Status");//
		label.status=nTemp;
		
		strtemp=q.getValuebyName(L"Tips");//
	 
		label.strTips=strtemp;
		
		 


		nTemp=q.getIntField("Input_or_Output");//
		label.input_or_output=nTemp;

		
		strTemp=q.getValuebyName(L"Text_Color");//
		if(strTemp.IsEmpty())
		{
			label.clrTxt=0;
		}
		else
		{
			label.clrTxt=_wtol(strTemp);
		}
		
		
		strTemp=q.getValuebyName(L"Back_Color");//
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

		q.nextRow();
		_Graphic_Value_Info temp1;
		m_graphic_refresh_data.push_back(temp1);
		m_graphic_refresh_data.at(nItem).deviceid = label.tstat_id;
		m_graphic_refresh_data.at(nItem).value_type = label.input_or_output;
		m_graphic_refresh_data.at(nItem).value_item = label.status;
		m_graphic_refresh_data.at(nItem).control_pt = pLabel;
		if(label.input_or_output == 0)
		{
			m_graphic_refresh_data.at(nItem).command = READINPUT_T3000;
			m_graphic_refresh_data.at(nItem).entitysize = sizeof(Str_in_point);
		}
		else if(label.input_or_output == 1)
		{
			m_graphic_refresh_data.at(nItem).command = READOUTPUT_T3000;
			m_graphic_refresh_data.at(nItem).entitysize = sizeof(Str_out_point);
		}
		else if(label.input_or_output == 2)
		{
			m_graphic_refresh_data.at(nItem).command = READVARIABLE_T3000;
			m_graphic_refresh_data.at(nItem).entitysize = sizeof(Str_variable_point);
		}
		m_graphic_refresh_data.at(nItem).hWnd = this->m_hWnd;
		nItem ++;
	}

	SqliteDBT3000.closedb();
	
}
void CGraphicView::OnDestroy()
{
	CFormView::OnDestroy();

	ClearAllLabels();
	
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

	((CMainFrame*)(theApp.m_pMainWnd))->SwitchToPruductType(0);


}
void CGraphicView::saveLabelInfo(int nItem)
{
	if(g_GraphicModelevel==1)
		return;
	if(nItem<0)
		return;
	if(nItem>=(int)m_RelayLabelLst.size())
		return;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);

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
	SqliteDBT3000.execDML((UTF8MBSTR)strSql);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	 SqliteDBT3000.closedb();
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


			CppSQLite3DB SqliteDBBuilding;
			CppSQLite3Table table;
			CppSQLite3Query q;
			SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);


			try
			{


			CString strImgFileName;
			strImgFileName=_T("");
			CString strSql;
			strSql.Format(_T("update ALL_NODE set Background_imgID ='%s' where Serial_ID = '%d'"),strImgFileName,m_nSerialNumber);
			SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
			SqliteDBBuilding.closedb();
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
		if(bac_cm5_graphic == false)
		{
			if(m_nSerialNumber<=0)
			{
				AfxMessageBox(_T("Unavailable a Tstat!"));
				return;
			}
		}
		else
		{
			if((g_bac_instance<=0) || (g_mac<=0))
			{
				AfxMessageBox(_T("Unavailable CM5!"));
				return;
			}
		}
		CString strFilter = _T("bmp file|*.bmp|jpg file|*.jpg|png file|*.png|tif file|*.tif|all file|*.*||");
		CFileDialog dlg(true,_T("Open image file"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
		if(IDOK==dlg.DoModal())
		{
			CString strImgFilePathName=dlg.GetPathName();
			CString strImgFileName=dlg.GetFileName();
			CString strDestFileName=g_strImgeFolder+strImgFileName;

			CopyFile(strImgFilePathName,strDestFileName,FALSE);
			 
			CppSQLite3DB SqliteDBBuilding;
			CppSQLite3Table table;
			CppSQLite3Query q;
			SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

			try
			{

			CString strSql;
			strSql.Format(_T("update ALL_NODE set Background_imgID ='%s' where Serial_ID = '%d'"),strImgFileName,m_nSerialNumber);
			SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
			 
			 SqliteDBBuilding.closedb();
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
	 
	switch(nIDEvent)
	{
	case 1:
		if(bac_cm5_graphic == true)	//如果是CM5 的Bacnet的协议 ;原来TSTAT的 处理方法和过程不要动;
		{
			for (int i=0;i<(int)m_graphic_refresh_data.size();i++)
			{
				Post_Refresh_One_Message(m_graphic_refresh_data.at(i).deviceid,
					m_graphic_refresh_data.at(i).command,
					m_graphic_refresh_data.at(i).value_item,
					m_graphic_refresh_data.at(i).value_item,
					m_graphic_refresh_data.at(i).entitysize);
				CString temp;
				//	static int test_value;
#if 0
				if(m_graphic_refresh_data.at(i).value_type == 0)
				{
					//	test_value ++;
					//	temp.Format(_T("%d"),test_value);
					temp.Format(_T("%d"),m_Input_data.at(m_graphic_refresh_data.at(i).value_item).value);
				}
				else if(m_graphic_refresh_data.at(i).value_type == 1)
				{
					//test_value ++;
					//	test_value ++;
					//	temp.Format(_T("%d"),test_value);
					temp.Format(_T("%d"),m_Output_data.at(m_graphic_refresh_data.at(i).value_item).value);
				}
				else if(m_graphic_refresh_data.at(i).value_type == 2)
				{
					//	test_value ++;
					//	test_value ++;
					//	test_value ++;
					//	temp.Format(_T("%d"),test_value);
					temp.Format(_T("%d"),m_Variable_data.at(m_graphic_refresh_data.at(i).value_item).value);
				}

				m_graphic_refresh_data.at(i).control_pt->m_strValueText = temp;	//在Label上 设置显示的 值;
#endif
				m_graphic_refresh_data.at(i).control_pt->Invalidate();
				//	m_graphic_refresh_data.at(i).control_pt->i
			}

		}
		break;
	}
	CFormView::OnTimer(nIDEvent);
}
