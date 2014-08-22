// BacnetScreenEdit.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetScreenEdit.h"
#include "afxdialogex.h"
#include "T3000.h"
#include "MainFrm.h"

#include "LabelEditDlg.h"

extern CBacnetScreenEdit * ScreenEdit_Window ;
// CBacnetScreenEdit dialog
// CGraphicView
#define XStart 0
//#define YStart 30
#define YStart 0
CRect mynew_rect;	//用来存储 窗体应该有多大;

const int SCREEN_HOTKEY_COUNT = 9;
int m_screenHotKeyID[SCREEN_HOTKEY_COUNT] = 
{
	4001,
	4002,
	4003,
	4004,
	4005,
	4006,
	4007,
	4008,
	4009
};

IMPLEMENT_DYNAMIC(CBacnetScreenEdit, CDialogEx)

CBacnetScreenEdit::CBacnetScreenEdit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetScreenEdit::IDD, pParent)
{

}

CBacnetScreenEdit::~CBacnetScreenEdit()
{
}

void CBacnetScreenEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetScreenEdit, CDialogEx)
	ON_MESSAGE(WM_HOTKEY,&CBacnetScreenEdit::OnHotKey)//快捷键消息映射手动加入
	ON_BN_CLICKED(IDC_BUTTON_SCREEN_EDIT_TEST, &CBacnetScreenEdit::OnBnClickedButtonScreenEditTest)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

	ON_WM_TIMER()
	ON_MESSAGE(MY_REDRAW_WINDOW, RedrawScreeneditWindow)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CBacnetScreenEdit::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CBacnetScreenEdit::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_SCREEN_EXIT, &CBacnetScreenEdit::OnBnClickedButtonScreenExit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CBacnetScreenEdit::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_ALL, &CBacnetScreenEdit::OnBnClickedButtonDeleteAll)
END_MESSAGE_MAP()


// CBacnetScreenEdit message handlers
LRESULT  CBacnetScreenEdit::RedrawScreeneditWindow(WPARAM wParam, LPARAM lParam)
{

	::GetWindowRect(BacNet_hwd,&mynew_rect);	//获取 view的窗体大小;
	MoveWindow(mynew_rect.left,mynew_rect.top,mynew_rect.Width(),mynew_rect.Height(),1);
	ReloadLabelsFromDB();
	Invalidate();
	return 0;
}

void CBacnetScreenEdit::OnBnClickedButtonScreenEditTest()
{
	// TODO: Add your control notification handler code here
}


BOOL CBacnetScreenEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		if(m_nFoucsIndext >= 0)
		{
			saveLabelInfo(m_nFoucsIndext);
			m_nFoucsIndext = -1;
		}
	}

	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) 
	{
		if(m_nFoucsIndext >= 0)
		{
			ReloadLabelsFromDB();
			Invalidate();
			m_nFoucsIndext = -1;
			return 0;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}




LRESULT CBacnetScreenEdit::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	UINT fuModifiers = (UINT) LOWORD(lParam);  // key-modifier flags    
	UINT uVirtKey = (UINT) HIWORD(lParam);     // virtual-key code    

	
	if( MOD_SHIFT == fuModifiers && VK_UP == uVirtKey )  //Screen
	{  

		CPoint mypoint;
		GetCursorPos(&mypoint);
		if(mypoint.y>10)
			mypoint.y=mypoint.y-10;
		else
			mypoint.y = 0;
		SetCursorPos(mypoint.x,mypoint.y);
	}  
	else if(MOD_SHIFT == fuModifiers && VK_DOWN == uVirtKey)//Program
	{

		CPoint mypoint;
		GetCursorPos(&mypoint);
		if(mypoint.y<m_cyScreen - 10)
			mypoint.y = mypoint.y + 10;
		else
			mypoint.y = m_cyScreen;
		SetCursorPos(mypoint.x,mypoint.y);
	}
	else if(MOD_SHIFT == fuModifiers && VK_LEFT == uVirtKey)//Input
	{
		CPoint mypoint;
		GetCursorPos(&mypoint);
		if(mypoint.x>10)
			mypoint.x = mypoint.x - 10;
		else
			mypoint.x = 0;
		SetCursorPos(mypoint.x,mypoint.y);
	}
	else if(MOD_SHIFT == fuModifiers && VK_RIGHT == uVirtKey)//Output
	{
		CPoint mypoint;
		GetCursorPos(&mypoint);
		if(mypoint.x<m_cxScreen)
			mypoint.x = mypoint.x + 10;
		else
			mypoint = m_cxScreen;
		SetCursorPos(mypoint.x,mypoint.y);
	}
	else if(MOD_ALT == fuModifiers && 'M' == uVirtKey)
	{
		CPoint my_point;
		GetCursorPos(&my_point);

		int n=HitTestEx(my_point);
		if(n>=0)
			m_nFoucsIndext=n;
		else
			return 0;

		m_LbtDown=TRUE;
		SetCapture();
	}
	else if(m_screenHotKeyID[4] == wParam)//UP
	{
		CPoint mypoint;
		GetCursorPos(&mypoint);
		if(mypoint.y>0)
			mypoint.y--;
		SetCursorPos(mypoint.x,mypoint.y);
	}
	else if(m_screenHotKeyID[5] == wParam)//DOWN
	{
		CPoint mypoint;
		GetCursorPos(&mypoint);
		if(mypoint.y<m_cyScreen)
			mypoint.y++;
		SetCursorPos(mypoint.x,mypoint.y);
	}
	else if(m_screenHotKeyID[6] == wParam)//LEFT
	{
		CPoint mypoint;
		GetCursorPos(&mypoint);
		if(mypoint.x>0)
			mypoint.x--;
		SetCursorPos(mypoint.x,mypoint.y);
	}
	else if(m_screenHotKeyID[7] == wParam)//RIGHT
	{
		CPoint mypoint;
		GetCursorPos(&mypoint);
		if(mypoint.x<m_cxScreen)
			mypoint.x++;
		SetCursorPos(mypoint.x,mypoint.y);
	}


	if (wParam==KEY_INSERT)	//If the cursor not in the range of the label or other objects,it will means add label
	{
		if(m_nFoucsIndext<0)
			Add_Label();
		else
			Edit_Label();
	}
	return 0;
}

BOOL CBacnetScreenEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//暂时加入，到时候要删掉的，不在这加，debug;
	RegisterHotKey(GetSafeHwnd(),KEY_INSERT,NULL,VK_INSERT);//Insert键
	// TODO:  Add extra initialization here
	
	m_cxScreen=GetSystemMetrics(SM_CXSCREEN);
	m_cyScreen=GetSystemMetrics(SM_CYSCREEN);
	
	::GetWindowRect(BacNet_hwd,&mynew_rect);	//获取 view的窗体大小;
	
	MoveWindow(mynew_rect.left,mynew_rect.top,mynew_rect.Width(),mynew_rect.Height(),1);
	//MoveWindow(0,0,m_cxScreen,m_cyScreen,1);
	InitGraphic(g_serialNum,g_bac_instance);

	SetClassLong(this->GetSafeHwnd(),GCL_HCURSOR ,(LONG)LoadCursor(NULL , IDC_CROSS));//IDC_ARROW



#ifdef _DEBUG //debug版本   
	int nRet = RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[0],MOD_SHIFT,VK_UP); 
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey SHIFT + UP failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[1],MOD_SHIFT,VK_DOWN); 
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey SHIFT + DOWN failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[2],MOD_SHIFT,VK_LEFT); 
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey SHIFT + LEFT failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[3],MOD_SHIFT,VK_RIGHT);
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey SHIFT + RIGHT failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[4],NULL,VK_UP);
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey  UP failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[5],NULL,VK_DOWN);
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey  DOWN failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[6],NULL,VK_LEFT);
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey  LEDT failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[7],NULL,VK_RIGHT);
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey  RIGHT failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[8],MOD_ALT,'M');
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey ALT + M failure"));  
#else //release版本   
	RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[0],MOD_SHIFT,VK_UP); 
	RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[1],MOD_SHIFT,VK_DOWN); 
	RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[2],MOD_SHIFT,VK_LEFT); 
	RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[3],MOD_SHIFT,VK_RIGHT); 
	RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[4],NULL,VK_UP); 
	RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[5],NULL,VK_DOWN); 
	RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[6],NULL,VK_LEFT); 
	RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[7],NULL,VK_RIGHT); 
	RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[8],MOD_ALT,'M'); 
#endif  

	m_screenedit_dlg_hwnd = this->m_hWnd;
	SetTimer(1,7000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


#pragma region MY_CODE
void CBacnetScreenEdit::InitGraphic(int nSerialNum,int nInstanceID)
{
	//m_strImgPathName=g_strImgeFolder+g_strImagePathName;
	CString PicFileTips;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_screen_data.at(screen_list_line).picture_file, (int)strlen((char *)m_screen_data.at(screen_list_line).picture_file)+1, 
		PicFileTips.GetBuffer(MAX_PATH), MAX_PATH );
	PicFileTips.ReleaseBuffer();

	m_strImgPathName=g_strImgeFolder+ PicFileTips ;//_T("sample1.bmp");
	WIN32_FIND_DATA fData;
	HANDLE hFile=NULL; 
	hFile = FindFirstFile(m_strImgPathName,&fData);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		m_bImgExist=TRUE;
		FindClose(hFile);
	}
	else
	{
		m_bImgExist=FALSE;
		FindClose(hFile);
		//自动删除相关记录，因为图象不存在，Label没有任何意思;

	}
	m_nSerialNumber=nSerialNum;
	m_nTstatID=nInstanceID;
	//m_strScreenName=strScreenName;



	ReloadLabelsFromDB();
	this->SetFocus();
	SetTimer(1,7000,NULL);//此定时器是用于刷新 Label的;
}


void CBacnetScreenEdit::ReloadLabelsFromDB()
{
	ClearAllLabels();
	
	m_pCon.CreateInstance(_T("ADODB.Connection"));
	m_pRs.CreateInstance(_T("ADODB.Recordset"));
	m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

	CString PicFileTips;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_screen_data.at(screen_list_line).picture_file, (int)strlen((char *)m_screen_data.at(screen_list_line).picture_file)+1, 
	PicFileTips.GetBuffer(MAX_PATH), MAX_PATH );
	PicFileTips.ReleaseBuffer();

	//CString temp_screen_item;
	//temp_screen_item.Format(_T("%d"),screen_list_line);
	CString strSql;
	//if(bac_cm5_graphic == false)
	strSql.Format(_T("select * from Screen_Label where Serial_Num =%i and Tstat_id=%i and Tips='%s'"),m_nSerialNumber,m_nTstatID,PicFileTips);
	//else
	//strSql.Format(_T("select * from Screen_Label where Serial_Num =%i and Tstat_id=%i"),g_bac_instance,g_mac);
	//strSql.Format(_T("select * from Screen_Label where Tstat_id=%i"),m_nTstatID);
	m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
	CString strtemp;
	strtemp.Empty();

	_variant_t temp_variant;
	int nTemp;
	CString strTemp;
	int nItem = 0;//用于记录有多少个需要刷新;
	::GetWindowRect(BacNet_hwd,&mynew_rect);	//获取 view的窗体大小;
	while(VARIANT_FALSE==m_pRs->EndOfFile)
	{	
		Label_information label;
		int temp_x_persent;
		int temp_y_persent;
		label.nSerialNum=m_nSerialNumber;
		label.tstat_id=m_nTstatID;
		label.strScreenName=m_strScreenName;//This is what the tips, I use this to identify the graphics group.

		nTemp=m_pRs->GetCollect("Cstatic_id");//
		label.cstatic_id=nTemp;

		nTemp=m_pRs->GetCollect("Point_X");//
		temp_x_persent=nTemp;
			label.point.x = (int)((temp_x_persent*mynew_rect.Width())/100);
		nTemp=m_pRs->GetCollect("Point_Y");//
		temp_y_persent = nTemp;
		label.point.y=(int)((temp_y_persent*mynew_rect.Height())/100 - YStart);

		nTemp=m_pRs->GetCollect("Height");//
		label.height=nTemp;

		nTemp=m_pRs->GetCollect("Width");//
		label.width=nTemp;

		nTemp=m_pRs->GetCollect("Status");//
		label.status=nTemp;
		
		temp_variant=m_pRs->GetCollect("Tips");//
		if(temp_variant.vt!=VT_NULL)
		{
		strtemp=temp_variant;
		label.strTips = strtemp;
		//int temp_tip_value =  _wtoi(strtemp);
		//if(screen_list_line != temp_tip_value)
		//	continue;
		}
		else
		{
			continue;
		}
	//		strtemp=_T("");
		label.strTips=strtemp;
		

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

	if(m_pRs->State) 
		m_pRs->Close(); 
	if(m_pCon->State)
		m_pCon->Close();
	
}

void CBacnetScreenEdit::ClearAllLabels()
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


void CBacnetScreenEdit::PainNoImageInfo(CDC* pDC)
{
	const int iOffset = 20;
	CString strInfo=_T("No image has been configed for current device, please config one first. ");

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

void CBacnetScreenEdit::Edit_Label()
{
	if(m_nFoucsIndext<0)
		return;
	if(!m_bImgExist)
	{
		AfxMessageBox(_T("No image has been configed for current TSTAT, please config one first!"));
		return;
	}
	//	g_bRefresh=FALSE;
	CString PicFileTips;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_screen_data.at(screen_list_line).picture_file, (int)strlen((char *)m_screen_data.at(screen_list_line).picture_file)+1, 
		PicFileTips.GetBuffer(MAX_PATH), MAX_PATH );
	PicFileTips.ReleaseBuffer();

	CLabelEditDlg dlg;
	dlg.SetCommandType(2);
	//dlg.SetInitValueForEdit(m_nTstatID,m_nSerialNumber,m_strScreenName,0);
	dlg.SetInitValueForEdit(m_nTstatID,m_nSerialNumber,PicFileTips,m_RelayLabelLst.at(m_nFoucsIndext).cstatic_id,m_RelayLabelLst.at(m_nFoucsIndext).input_or_output,
		m_RelayLabelLst.at(m_nFoucsIndext).width,m_RelayLabelLst.at(m_nFoucsIndext).height,m_RelayLabelLst.at(m_nFoucsIndext).status,
		m_RelayLabelLst.at(m_nFoucsIndext).clrTxt,m_RelayLabelLst.at(m_nFoucsIndext).bkColor);
	dlg.DoModal();
	//	if(g_bRefresh)
	//	{
	ReloadLabelsFromDB();
	Invalidate();
	//	}

}


void CBacnetScreenEdit::Add_Label()
{
	if(!m_bImgExist)
	{
		AfxMessageBox(_T("No image has been configed for current TSTAT, please config one first!"));
		return;
	}
//	g_bRefresh=FALSE;
	CString PicFileTips;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_screen_data.at(screen_list_line).picture_file, (int)strlen((char *)m_screen_data.at(screen_list_line).picture_file)+1, 
		PicFileTips.GetBuffer(MAX_PATH), MAX_PATH );
	PicFileTips.ReleaseBuffer();

	CLabelEditDlg dlg;
	dlg.SetCommandType(1);
	//dlg.SetInitValueForEdit(m_nTstatID,m_nSerialNumber,m_strScreenName,0);
	dlg.SetInitValueForEdit(m_nTstatID,m_nSerialNumber,PicFileTips,0);
	dlg.DoModal();
//	if(g_bRefresh)
//	{
		ReloadLabelsFromDB();
		Invalidate();
//	}

}

int CBacnetScreenEdit::HitTestEx(CPoint & point)
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

void CBacnetScreenEdit::saveLabelInfo(int nItem)
{
	//if(g_GraphicModelevel==1)
	//	return;
	if(nItem<0)
		return;
	if(nItem>=(int)m_RelayLabelLst.size())
		return;
	m_pCon.CreateInstance(_T("ADODB.Connection"));
	m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);
	::GetWindowRect(BacNet_hwd,&mynew_rect);	//获取 view的窗体大小;
	Label_information label;
	label=m_RelayLabelLst.at(nItem);
	CRect rcItem;
	label.plabelCtrl->GetWindowRect(&rcItem);
	ScreenToClient(&rcItem);
	int nLeft,nTop;
	nLeft = (rcItem.left*100)/mynew_rect.Width();
	nTop = (rcItem.top*100)/(mynew_rect.Height());

	//nLeft=rcItem.left-XStart;
	//nTop=rcItem.top-YStart;
	CString PicFileTips;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_screen_data.at(screen_list_line).picture_file, (int)strlen((char *)m_screen_data.at(screen_list_line).picture_file)+1, 
		PicFileTips.GetBuffer(MAX_PATH), MAX_PATH );
	PicFileTips.ReleaseBuffer();

	CString temp_screen_item = PicFileTips;
	try
	{


		CString strSql;
		strSql.Format(_T("update Screen_Label set Point_X=%i,Point_Y=%i where Serial_Num =%i and Tstat_id=%i and  Cstatic_id=%i and Tips='%s'"),
			nLeft,nTop,label.nSerialNum,label.tstat_id,label.cstatic_id,temp_screen_item);
		m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	if(m_pCon->State)
		m_pCon->Close();
}

#pragma endregion

void CBacnetScreenEdit::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	TRACE(_T("Screen Edit Paint\r\n"));
	::GetWindowRect(BacNet_hwd,&mynew_rect);	//获取 view的窗体大小;

	
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages

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
		//graphics.DrawImage(&bitmap,XStart,YStart,bitmap.GetWidth(),bitmap.GetHeight());
		//graphics.DrawImage(&bitmap,XStart,YStart,m_cxScreen,m_cyScreen);
		graphics.DrawImage(&bitmap,XStart ,YStart,mynew_rect.Width(),mynew_rect.Height() - YStart);
		

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

}


void CBacnetScreenEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int n=HitTestEx(point);
	if(n>=0)
		m_nFoucsIndext=n;
	else
	{
		m_nFoucsIndext = -1;
		return;
	}
	//int nstate=m_bLockBtn.GetCheck();
	//if(m_bLockBtn.GetCheck()==BST_CHECKED)
	//	return;
	m_LbtDown=TRUE;
	SetCapture();

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CBacnetScreenEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_LbtDown&&m_nFoucsIndext>=0)
	{
		saveLabelInfo(m_nFoucsIndext);
	}

	m_LbtDown=FALSE;
	Invalidate();
	ReleaseCapture();
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CBacnetScreenEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
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
	CDialogEx::OnMouseMove(nFlags, point);
}

void CBacnetScreenEdit::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	SetClassLong(this->GetSafeHwnd(),GCL_HCURSOR ,(LONG)LoadCursor(NULL , IDC_ARROW));//IDC_ARROW
	for (int i=0;i<SCREEN_HOTKEY_COUNT;i++)
	{
		UnregisterHotKey(GetSafeHwnd(), m_screenHotKeyID[i]);   
	}
	UnregisterHotKey(GetSafeHwnd(),KEY_INSERT);
	::PostMessage(m_screen_dlg_hwnd,WM_SCREENEDIT_CLOSE,NULL,NULL);
	
	KillTimer(1);
	CDialogEx::OnCancel();
}




void CBacnetScreenEdit::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	
	if(this->IsWindowVisible())
	{
		for (int i=0;i<(int)m_graphic_refresh_data.size();i++)
		{
			Post_Refresh_One_Message(m_graphic_refresh_data.at(i).deviceid,
				m_graphic_refresh_data.at(i).command,
				m_graphic_refresh_data.at(i).value_item,
				m_graphic_refresh_data.at(i).value_item,
				m_graphic_refresh_data.at(i).entitysize);
			m_graphic_refresh_data.at(i).control_pt->Invalidate();
		}
		Invalidate(0);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CBacnetScreenEdit::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	Add_Label();
}



void CBacnetScreenEdit::OnBnClickedButtonEdit()
{
	// TODO: Add your control notification handler code here
	if(m_nFoucsIndext<0)
	{
		MessageBox(_T("Please select a label first!"),_T("Notice"),MB_OK | MB_ICONINFORMATION);
		return;
	}
	Edit_Label();
}


void CBacnetScreenEdit::OnBnClickedButtonScreenExit()
{
	// TODO: Add your control notification handler code here
	PostMessage(WM_CLOSE,NULL,NULL);
}


void CBacnetScreenEdit::OnBnClickedButtonDelete()
{
	// TODO: Add your control notification handler code here
	if(m_nFoucsIndext<0)
	{
		MessageBox(_T("Please select a label first!"),_T("Notice"),MB_OK | MB_ICONINFORMATION);
		return;
	}

	if(AfxMessageBox(_T("Do you really want to delete current label?"),MB_YESNO)==IDYES)
	{
		m_pCon.CreateInstance(_T("ADODB.Connection"));
		m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

		Label_information	label;
		label=m_RelayLabelLst.at(m_nFoucsIndext);

		try
		{
			CString strSql;
			strSql.Format(_T("delete * from Screen_Label where Serial_Num =%i and Tstat_id=%i and  Cstatic_id=%i and Tips='%s'"),label.nSerialNum,label.tstat_id,label.cstatic_id,label.strTips);
			m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
		}
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}
		if(m_pCon->State) 
			m_pCon->Close(); 

		ReloadLabelsFromDB();
		Invalidate();
		m_nFoucsIndext=-1;
	}
}


void CBacnetScreenEdit::OnBnClickedButtonDeleteAll()
{
	// TODO: Add your control notification handler code here

	if(AfxMessageBox(_T("Do you really want to delete all labels?"),MB_YESNO)==IDYES)
	{
		//m_pCon.CreateInstance(_T("ADODB.Connection"));
		//m_pRs.CreateInstance(_T("ADODB.Recordset"));
		//m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);
		m_pCon.CreateInstance(_T("ADODB.Connection"));
		m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

		Label_information	label;
		label=m_RelayLabelLst.at(m_nFoucsIndext);

		try
		{
			CString strSql;
			strSql.Format(_T("delete * from Screen_Label where Serial_Num =%i and Tstat_id=%i and Tips='%s'"),label.nSerialNum,label.tstat_id,label.strTips);
			m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
		}
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}
		if(m_pCon->State) 
			m_pCon->Close(); 

		ReloadLabelsFromDB();
		Invalidate();
		m_nFoucsIndext=-1;
	}
}
