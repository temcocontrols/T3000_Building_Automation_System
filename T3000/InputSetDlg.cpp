// InputSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "InputSetDlg.h"
#include "global_function.h"
#include "BuildTable1.h"
#include "../SQLiteDriver/CppSQLite3.h"

#define INDEX_FIELD 0
#define NAME_FIELD 1
#define VALUE_FIELD 2
#define AM_FIELD 3
#define CAL_FIELD 4
#define FILTER 5
#define RANG_FIELD 6
#define FUN_FIELD 7
#define CUST_FIELD 8

#define TOTAL_COLS 8+1
#define TOTAL_ROWS 7
// CInputSetDlg dialog

#define ANALOG_RANG_NUMBER 12
 
// CString analog_range[ANALOG_RANG_NUMBER]={_T("Raw"),_T("10KC Therm"),_T("0-100%"),_T("On/Off"),_T("Custom Sensor"),_T("Off/On")};
// CString analog_range[ANALOG_RANG_NUMBER]={_T("Raw"),_T("10KF Therm"),_T("0-100%"),_T("On/Off"),_T("Custom Sensor"),_T("Off/On")};
//CString analog_range[ANALOG_RANG_NUMBER]={_T("UNUSED"),_T("10KC Therm"),_T("0-100%"),_T("On/Off"),_T("Custom Sensor1"),_T("Off/On"),_T("Custom Sensor2"),_T("Occupied/Unoccupied"),_T("Unoccupied/Occupied"),_T("Open/Close"),_T("Close/Open")};
//CString INPUT_FUNS[8]={_T("Normal"),_T("Freeze Protect"),_T("Occupancy Sensor"),_T("Sweep Off"),_T("Clock"),_T("Changeover Mode"),_T("Outside Temp"),_T("Airflow")};
//CString g_str5ERange2[5]={_T("Raw"),_T("Thermistor"),_T("On/Off"),_T("Off/On"),_T("%")};

IMPLEMENT_DYNAMIC(CInputSetDlg, CDialog)

CInputSetDlg::CInputSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputSetDlg::IDD, pParent)
	, m_filterValue(0)
	//, m_filterValue(0)
{
	m_nCurRow=m_nCurCol=0;
	InputThread=NULL;
	 b_is_fresh = false;
	   m_cvalue=0;
	   m_crange=0;
	     m_old_move_row = 0;
	     m_old_move_col = 0;
	 
}

CInputSetDlg::~CInputSetDlg()
{
	if(InputThread!=NULL)
	{
		TerminateThread(InputThread, 0);
	}
	InputThread = NULL;
	b_is_fresh = false;
}

void CInputSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_FlexGrid);
	DDX_Control(pDX, IDC_RANG_COMBO, m_RangCombox);
	DDX_Control(pDX, IDC_UPBUTTON, m_upButton);
	DDX_Control(pDX, IDC_DOWNBUTTON, m_downButton);
	DDX_Control(pDX, IDC_AMCOMBO, m_AmCombox);
	DDX_Control(pDX, IDC_INPUTFUNCOMBO, m_inputFinCombox);
	DDX_Control(pDX, IDC_CUSTOMBUTTON, m_customBtn);
	DDX_Control(pDX, IDC_INVALUEEDIT, m_inValueEdit);
	DDX_Control(pDX, IDC_VALUECOMBO, m_valueCombx);
	DDX_Control(pDX, IDC_INPUTNAMEEDIT, m_inputNameEdt);
	DDX_Control(pDX, IDC_FILTER, m_Filter);
	DDX_Text(pDX, IDC_FILTER, m_filterValue);
	DDV_MinMaxUInt(pDX, m_filterValue, 0, 100);
	DDX_Control(pDX, IDC_FILTER, m_Filter);
	DDX_Text(pDX, IDC_FILTER, m_filterValue);
}


BEGIN_MESSAGE_MAP(CInputSetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CInputSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CInputSetDlg::OnBnClickedCancel)
	//ON_CBN_SELCHANGE(IDC_RANG_COMBO, &CInputSetDlg::OnCbnSelchangeRangCombo)
	ON_CBN_KILLFOCUS(IDC_RANG_COMBO, &CInputSetDlg::OnCbnKillfocusRangCombo)
	ON_BN_CLICKED(IDC_UPBUTTON, &CInputSetDlg::OnBnClickedUpbutton)
	ON_BN_CLICKED(IDC_DOWNBUTTON, &CInputSetDlg::OnBnClickedDownbutton)
	ON_CBN_KILLFOCUS(IDC_AMCOMBO, &CInputSetDlg::OnCbnKillfocusAmcombo)
 	//ON_CBN_SELCHANGE(IDC_AMCOMBO, &CInputSetDlg::OnCbnSelchangeAmcombo)
	ON_CBN_SELCHANGE(IDC_INPUTFUNCOMBO, &CInputSetDlg::OnCbnSelchangeInputfuncombo)
	ON_CBN_KILLFOCUS(IDC_INPUTFUNCOMBO, &CInputSetDlg::OnCbnKillfocusInputfuncombo)
	ON_BN_CLICKED(IDC_CUSTOMBUTTON, &CInputSetDlg::OnBnClickedCustombutton)
	ON_BN_KILLFOCUS(IDC_CUSTOMBUTTON, &CInputSetDlg::OnBnKillfocusCustombutton)
	ON_BN_CLICKED(IDEXIT, &CInputSetDlg::OnBnClickedExit)
	//ON_EN_KILLFOCUS(IDC_INVALUEEDIT, &CInputSetDlg::OnEnKillfocusInvalueedit)
	//ON_CBN_KILLFOCUS(IDC_VALUECOMBO, &CInputSetDlg::OnCbnKillfocusValuecombo)
	ON_BN_CLICKED(IDC_REFRESHBUTTON, &CInputSetDlg::OnBnClickedRefreshbutton)
 	//ON_EN_KILLFOCUS(IDC_INPUTNAMEEDIT, &CInputSetDlg::OnEnKillfocusInputnameedit)
	ON_MESSAGE(WM_REFRESH_INPUTDLG, &CInputSetDlg::DealMessage)//Add by Fan
	ON_WM_CLOSE()
	ON_EN_KILLFOCUS(IDC_FILTER, &CInputSetDlg::OnEnKillfocusFilter)
	//ON_CBN_SELCHANGE(IDC_VALUECOMBO, &CInputSetDlg::OnCbnSelchangeValuecombo)
	ON_BN_CLICKED(IDC_FRESH, &CInputSetDlg::OnBnClickedFresh)
	ON_CBN_SELENDOK(IDC_RANG_COMBO, &CInputSetDlg::OnCbnSelendokRangCombo)
	ON_CBN_SELENDOK(IDC_VALUECOMBO, &CInputSetDlg::OnCbnSelendokValuecombo)
END_MESSAGE_MAP()

// CInputSetDlg message handlers
const int REFRESH_GRID=1;
const int ENABLE_REFRESH_BUTTON=2;
const int DEALWITH_GRIDCHANGED=3;
LRESULT  CInputSetDlg::DealMessage(WPARAM wParam,LPARAM lParam)
{
	int command = wParam;
	switch(command)
	{
	case REFRESH_GRID:
		Fresh_GridForAll();
		break;
	case ENABLE_REFRESH_BUTTON:
		GetDlgItem(IDC_REFRESHBUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDEXIT)->EnableWindow(TRUE);
		break;
	case DEALWITH_GRIDCHANGED:
	   DealWithGridChanged();
	default:
		break;
	}

	return 0;
}
void CInputSetDlg::DealWithGridChanged(){
if (m_nCurRow==0||m_nCurCol==0)
{
return ;
     
}
if (m_nCurCol == NAME_FIELD)
{
   OnEnKillfocusInputnameedit();
}
else if (m_nCurCol ==RANG_FIELD)
{
   OnCbnSelchangeRangCombo();
}
else if (m_nCurCol == VALUE_FIELD)
{  
OnEnKillfocusInvalueedit();
}

}
void CInputSetDlg::OnBnClickedOk()
{
	
	//OnOK();
}

//Add by Fan  2013 04 01
DWORD WINAPI CInputSetDlg::StartRefresh(LPVOID lpVoid)
{
	CInputSetDlg *pParent = (CInputSetDlg *)lpVoid;
	int i;
	
	register_critical_section.Lock();
	pParent->b_is_fresh = true;
	for( i=0;i<17;i++)
	{
		Read_Multi(g_tstat_id,&product_register_value[i*100],i*100,100);
	}
	register_critical_section.Unlock();
	memset(product_register_value,0,sizeof(product_register_value));
	memcpy_s(product_register_value,sizeof(product_register_value),product_register_value,sizeof(product_register_value));//
	//if ((product_register_value[7] == 6)||(product_register_value[7] == 7))//tstat6
	//{
	//	//product_register_value[]列表交换。
	//	memset(pParent->tempchange,0,sizeof(pParent->tempchange));
	//	int index = 0;

	//	for (int i = 0;i<512;i++)
	//	{
	//		index = reg_tstat6[i];
	//		pParent->tempchange[index] = product_register_value[i];
	//	}
	//	memcpy_s(product_register_value,sizeof(product_register_value),pParent->tempchange,sizeof(pParent->tempchange));
	//}

	pParent->PostMessage(WM_REFRESH_INPUTDLG,REFRESH_GRID,0);
	pParent->PostMessage(WM_REFRESH_INPUTDLG,ENABLE_REFRESH_BUTTON,0);
	pParent->b_is_fresh = false;
	pParent->InputThread=NULL;
	return 0;
}
void CInputSetDlg::FLEX_GRID1_PUT_COLOR_STR(int row,int col,COLORREF grid_color=FLEXGRID_CELL_COLOR) 
{
	//m_FlexGrid.put_TextMatrix(row,col+5,_T(""));
	if (col==-1)
	{
	for (int i=1;i<=m_FlexGrid.get_Rows();i++)
	{
		m_FlexGrid.put_Row(row);
		//m_FlexGrid.put_Col(col);
		m_FlexGrid.put_CellBackColor(grid_color);
	}
		
	}
	else
	{
		m_FlexGrid.put_Row(row);
		m_FlexGrid.put_Col(col);
		m_FlexGrid.put_CellBackColor(grid_color);
	}

}
void CInputSetDlg::HideAllControls(){
	m_inputNameEdt.ShowWindow(SW_HIDE);
	m_inValueEdit.ShowWindow(SW_HIDE);
	m_valueCombx.ShowWindow(SW_HIDE);

	m_RangCombox.ShowWindow(SW_HIDE);

	m_upButton.SetImage(IDB_UPBMP);
	m_downButton.SetImage(IDB_DOWNBMP);
	m_upButton.SetWindowText(_T(""));
	m_downButton.SetWindowText(_T(""));
	m_upButton.SizeToContent();
	m_downButton.SizeToContent();
	m_upButton.Invalidate();
	m_downButton.Invalidate();
	m_downButton.ShowWindow(SW_HIDE);
	m_upButton.ShowWindow(SW_HIDE);

	m_AmCombox.ShowWindow(SW_HIDE);
	m_inputFinCombox.ShowWindow(SW_HIDE);
	m_customBtn.ShowWindow(SW_HIDE);
}
void CInputSetDlg::ShowRangeCombox(){
	CRect temp_rect;
	m_RangCombox.GetWindowRect(temp_rect);
	POINT temp_point;
	temp_point.x = temp_rect.left;
	temp_point.y = temp_rect.top;
	SetCursorPos(temp_point.x, temp_point.y);
	mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
	mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
}
BOOL CInputSetDlg::OnInitDialog()
{
	int m_inRows=0x00;
	CString RowTitle;
	CDialog::OnInitDialog();
	m_inputNameEdt.ShowWindow(SW_HIDE);
	m_inValueEdit.ShowWindow(SW_HIDE);
	m_valueCombx.ShowWindow(SW_HIDE);

	m_RangCombox.ShowWindow(SW_HIDE);

	m_upButton.SetImage(IDB_UPBMP);
	m_downButton.SetImage(IDB_DOWNBMP);
	m_upButton.SetWindowText(_T(""));
	m_downButton.SetWindowText(_T(""));
	m_upButton.SizeToContent();
	m_downButton.SizeToContent();
	m_upButton.Invalidate();
	m_downButton.Invalidate();
	m_downButton.ShowWindow(SW_HIDE);
	m_upButton.ShowWindow(SW_HIDE);

	m_AmCombox.ShowWindow(SW_HIDE);
	m_inputFinCombox.ShowWindow(SW_HIDE);
	m_customBtn.ShowWindow(SW_HIDE);
	

	m_sn=product_register_value[0]+product_register_value[1]*256+product_register_value[2]*256*256+product_register_value[3]*256*256*256;
//	m_FlexGrid.CellAlignment(1)=flexAlignCenterCenter;
	m_nModel=product_register_value[MODBUS_PRODUCT_MODEL];
	switch (m_nModel)
	{
		case 3:m_inRows=4;break; // 5B
		case 2:
		case 1:m_inRows=3;break; // 5A
		case 4:m_inRows=5;break; // 5C
		case PM_TSTAT7:
		m_inRows=12;break;
		case PM_PRESSURE:
		case 12:m_inRows=5;break; // 5D 同 TStat7
		case PM_TSTAT6:	m_inRows=12;break;
        case PM_TSTAT5i:	m_inRows=12;break;
        case PM_TSTAT8:	m_inRows=12;break;
        case PM_TSTAT9:	m_inRows = 12;break;
		case PM_TSTAT8_WIFI:	m_inRows = 12; break;
		case PM_TSTAT8_OCC:	m_inRows = 12; break;
		case PM_TSTAT7_ARM:	m_inRows = 12; break;
		case PM_TSTAT8_220V:	m_inRows = 12; break;
		case 16:m_inRows=10;break; // 5E
        case PM_PM5E:m_inRows=10;break; // 5E
        case PM_PM5E_ARM:m_inRows = 10;break; // 5E
		case 17:m_inRows=5;break; // 5F
		case 18:m_inRows=5;break; // 5G
		case 19:m_inRows=9;break; // 5H
		default:
			break;
	}
	if (m_inRows == 0)
	{
		AfxMessageBox(_T("Please make sure you select the correct model!"), MB_ICONWARNING);
		return FALSE;
	}


	m_FlexGrid.put_Rows(m_inRows);

	m_FlexGrid.put_Cols(TOTAL_COLS);
	//m_FlexGrid.put_Rows(TOTAL_ROWS);
	m_FlexGrid.put_TextMatrix(0,INDEX_FIELD,_T(""));
	m_FlexGrid.put_ColWidth(INDEX_FIELD,400);

	m_FlexGrid.put_TextMatrix(0,NAME_FIELD,_T("Input Name"));
	m_FlexGrid.put_ColWidth(NAME_FIELD,1300);

	m_FlexGrid.put_TextMatrix(0,VALUE_FIELD,_T("Value"));
	m_FlexGrid.put_ColWidth(VALUE_FIELD, 1300);
	
	m_FlexGrid.put_TextMatrix(0,AM_FIELD,_T("Auto/Man"));
	m_FlexGrid.put_ColWidth(AM_FIELD,1000);

	m_FlexGrid.put_TextMatrix(0,CAL_FIELD,_T("Calibration"));
	m_FlexGrid.put_ColWidth(CAL_FIELD,750);	
	int nFlag = product_register_value[7];
	if ((product_register_value[7]==PM_TSTAT6)||(product_register_value[7]==PM_TSTAT7)||(product_register_value[7]==PM_TSTAT5i)||(product_register_value[7]==PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9)
		|| (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V)
		)
	{
		m_FlexGrid.put_TextMatrix(0,FILTER,_T("Filter"));
		m_FlexGrid.put_ColWidth(FILTER,750);
	} 
	else
	{
		m_FlexGrid.put_TextMatrix(0,FILTER,_T("Filter"));
		m_FlexGrid.put_ColWidth(FILTER,750);
	    m_FlexGrid.put_ColWidth(FILTER,0);
	}
		
	

	m_FlexGrid.put_TextMatrix(0,RANG_FIELD,_T("Range"));
	m_FlexGrid.put_ColWidth(RANG_FIELD,2000);

	m_FlexGrid.put_TextMatrix(0,FUN_FIELD,_T("Function"));
	m_FlexGrid.put_ColWidth(FUN_FIELD,1400);

	m_FlexGrid.put_TextMatrix(0,CUST_FIELD,_T("Custom Tables"));
	m_FlexGrid.put_ColWidth(CUST_FIELD,1100);
 
	UINT uint_temp=GetOEMCP();//get system is for chinese or english
	if(uint_temp!=936 && uint_temp!=950)
	{

		CString strTemp;
		strTemp.Format(_T("%cC"),176);
		m_strUnitList.push_back(strTemp);
		strTemp.Format(_T("%cF"),176);
		m_strUnitList.push_back(strTemp);
	}
	else//chinese.
	{
		CString strTemp;
		strTemp=_T("°C");
		m_strUnitList.push_back(strTemp);
		strTemp=_T("°F");
		m_strUnitList.push_back(strTemp);
	}

	//Add by Fan,  add a icon to  the button 2013 03 29
	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_REFRESH);
	((CButton *)GetDlgItem(IDC_REFRESHBUTTON))->SetIcon(hIcon);
	hIcon_Exit = AfxGetApp()->LoadIcon(IDI_ICON_EXIT);
	((CButton *)GetDlgItem(IDEXIT))->SetIcon(hIcon_Exit);

	if(m_nModel == 16||m_nModel == PM_PM5E || m_nModel == PM_PM5E_ARM)
	{
		Init_not_5ABCD_Grid();
		m_nCurCol=1;
		m_nCurRow=1;
		m_old_move_col=m_nCurCol;
		m_old_move_row=m_nCurRow;//put_CellBackColor
		m_FlexGrid.put_Row(m_nCurRow);
		m_FlexGrid.put_Col(m_nCurCol);
		m_old_Cell_color=m_FlexGrid.get_CellBackColor();


		SetCurrentCell();
		return true;
	}
	if (m_nModel==PM_TSTAT7||m_nModel==PM_TSTAT6||m_nModel==PM_TSTAT5i||m_nModel==PM_TSTAT8 || m_nModel == PM_TSTAT9
		|| (m_nModel == PM_TSTAT8_WIFI) || (m_nModel == PM_TSTAT8_OCC) || (m_nModel == PM_TSTAT7_ARM) || (m_nModel == PM_TSTAT8_220V))
	{
		InitGridtstat6();
// 		m_FlexGrid.put_RowSel(1);
// 		m_FlexGrid.put_ColSel(1);
		//m_FlexGrid.put_BackColorSel(RGB(0,255,0));
		//FLEX_GRID1_PUT_COLOR_STR(1,1);
		m_nCurCol=1;
		m_nCurRow=1;
		m_old_move_col=m_nCurCol;
		m_old_move_row=m_nCurRow;//put_CellBackColor
		m_FlexGrid.put_Row(m_nCurRow);
		m_FlexGrid.put_Col(m_nCurCol);
		m_old_Cell_color=m_FlexGrid.get_CellBackColor();

		
		SetCurrentCell();
		return TRUE;
	}

 


	for(int i=1;i<m_inRows;i++)
	{
		CString strIndex;
		strIndex.Format(_T("%d"), i-1);
		m_FlexGrid.put_TextMatrix(i,INDEX_FIELD,strIndex);
		switch(i)
		{
			case 1:
				RowTitle=g_strSensorName;
				break;
			case 2:
				RowTitle=g_strInName1;
				break;
			case 3:
				RowTitle=g_strInName2;
				break;
			case 4:
				RowTitle=g_strInName3;
				break;
			case 5:
				RowTitle=g_strInName4;
				break;
			case 6:
				RowTitle=g_strInName5;
				break;
		}
		m_FlexGrid.put_TextMatrix(i,1,RowTitle);
		m_FlexGrid.put_RowHeight(i,270);
		for(int k=0;k<TOTAL_COLS;k++)
		{
			m_FlexGrid.put_ColAlignment(k,4);
			if (i%2==1)
			{
				m_FlexGrid.put_Row(i);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				m_FlexGrid.put_Row(i);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(COLOR_CELL);
			}
		}
	}
	


	Fresh_Grid();
	

	m_nCurCol=1;
	m_nCurRow=1;
	m_old_move_col=m_nCurCol;
	m_old_move_row=m_nCurRow;//put_CellBackColor
	m_FlexGrid.put_Row(m_nCurRow);
	m_FlexGrid.put_Col(m_nCurCol);
	m_old_Cell_color=m_FlexGrid.get_CellBackColor();

 
	SetCurrentCell();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CInputSetDlg::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->lParam == WM_CLOSE)
		Sleep(1);
			BOOL flag=FALSE;
 



 
 
 	if (pMsg->message == WM_KEYDOWN)
 	{
		 if(GetFocus()->GetDlgCtrlID() == IDC_MSFLEXGRID1){
		 HideAllControls();
		  			int lRow = m_FlexGrid.get_RowSel();//获取点击的行号	
 			int lCol = m_FlexGrid.get_ColSel(); //获取点击的列号
			int lRow_Count = m_FlexGrid.get_Rows();
			int lCol_Count = m_FlexGrid.get_Cols();

 		/*	if (lRow==0||lCol==0)
 			{
 			 goto ENGMESSAGE;
 			}*/
 			
 	   switch(pMsg->wParam){
	   
 	   case VK_RETURN:{
 	   ClickMsflexgrid_Click();
 	   }
 	   break;
 	   case VK_LEFT:{
		    	   if (lRow==0||lCol==0)
		    	   {
		    		   goto ENGMESSAGE;
		    	   }
 	   lCol=lCol-1;

 	   flag=TRUE;
 	   }
	   break;
 	   case VK_UP:{
		   if (lRow==0||lCol==0)
		   {
			   goto ENGMESSAGE;
		   }
 	   lRow=lRow-1;
 	   flag=TRUE;
 	   }
	   break;
 	   case VK_RIGHT:{
	   if (lCol == lCol_Count-1)
	   {
	   goto ENGMESSAGE;
	   }
 	    lCol=lCol+1;

 		flag=TRUE;
 	   }
	   break;
 	   case VK_DOWN:{
		   if (lRow == lRow_Count-1)
		   {
			   goto ENGMESSAGE;
		   }
 	   lRow=lRow+1;
 	   flag=TRUE;
 	   }
 	   break;
 	   }
 
 	   if (lRow==0||lCol==0)
 	   {
 		   goto ENGMESSAGE;
 	   }
	    
	   if (!flag)
	   {
		   goto ENGMESSAGE;
	   }
	   if(m_old_move_col!=0){
		   FLEX_GRID1_PUT_COLOR_STR(m_old_move_row,m_old_move_col,m_old_Cell_color);
	   }
	   m_old_move_col=lCol;
	   m_old_move_row=lRow;//put_CellBackColor
	   m_FlexGrid.put_Row(lRow);
	   m_FlexGrid.put_Col(lCol);
	   m_old_Cell_color=m_FlexGrid.get_CellBackColor();
	   FLEX_GRID1_PUT_COLOR_STR(lRow,lCol,RGB(0,255,0));
	   return TRUE;
		 }

		  
		 if (pMsg->wParam == VK_RETURN)
		 {

			 if(GetFocus()->GetDlgCtrlID() == IDC_RANG_COMBO)
			 {
			    CString strText;
				m_RangCombox.GetWindowTextW(strText);
				m_RangCombox.ShowWindow(SW_HIDE);
				m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
				SetCurrentCell();
				PostMessage(WM_REFRESH_INPUTDLG,DEALWITH_GRIDCHANGED,0);
				 return TRUE;
			 } 

			 if(GetFocus()->GetDlgCtrlID() == IDC_INPUTNAMEEDIT)
			 {
				 CString strText;
				 m_inputNameEdt.GetWindowText(strText);
				 m_inputNameEdt.ShowWindow(SW_HIDE);
				 m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
			     SetCurrentCell();
				 PostMessage(WM_REFRESH_INPUTDLG,DEALWITH_GRIDCHANGED,0);
				 return TRUE;
			 } 

			 if(GetFocus()->GetDlgCtrlID() == IDC_INVALUEEDIT)
			 {
				 CString strText;
				 m_inValueEdit.GetWindowText(strText);
				 m_inValueEdit.ShowWindow(SW_HIDE);
				 m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
				 SetCurrentCell();
				 PostMessage(WM_REFRESH_INPUTDLG,DEALWITH_GRIDCHANGED,0);
				 return TRUE;
			 } 

			 if(GetFocus()->GetDlgCtrlID() == IDC_VALUECOMBO)
			 {
				 CString strText;
				 m_valueCombx.GetWindowText(strText);
				 m_valueCombx.ShowWindow(SW_HIDE);
				 m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
				 SetCurrentCell();
				 PostMessage(WM_REFRESH_INPUTDLG,DEALWITH_GRIDCHANGED,0);
				 return TRUE;
			 } 

		 }
 	}

	


ENGMESSAGE:
	return CDialog::PreTranslateMessage(pMsg);
}
void CInputSetDlg::SetCurrentCell(){
    m_FlexGrid.SetFocus();
	m_FlexGrid.put_Row(m_nCurRow);
	m_FlexGrid.put_Col(m_nCurCol);
	FLEX_GRID1_PUT_COLOR_STR(m_nCurRow,m_nCurCol,RGB(0,255,0));

}
void CInputSetDlg::OnBnClickedCancel()
{
	
	OnCancel();
}

void CInputSetDlg::Fresh_Grid()
{
    

	 

	if(m_nModel == 16)
	{
		Init_not_5ABCD_Grid();
		return ;
	}
	if (m_nModel==PM_TSTAT6||m_nModel==PM_TSTAT5i||m_nModel==PM_TSTAT8 || m_nModel == PM_TSTAT9 ||m_nModel==PM_TSTAT7
		|| (m_nModel == PM_TSTAT8_WIFI) || (m_nModel == PM_TSTAT8_OCC) || (m_nModel == PM_TSTAT7_ARM) || (m_nModel == PM_TSTAT8_220V))
	{
		//InitGridtstat6();
		Fresh_GridForTstat6();
		return ;
	}
 

	CString strUnit=GetTempUnit();
	CString strTemp;	
	int nValue=0;
	CString strAuto=_T("Auto");
	CString strman=_T("Manual");
//row 1:	
	if(m_FlexGrid.get_Row()>=1)
	{
		strTemp.Format(_T("%.1f"),product_register_value[MODBUS_TEMPRATURE_CHIP]/10.0);//101
		strTemp=strTemp+strUnit;
		m_FlexGrid.put_TextMatrix(1,VALUE_FIELD,strTemp);
		m_FlexGrid.put_TextMatrix(1,AM_FIELD,NO_APPLICATION);
		m_FlexGrid.put_TextMatrix(1,CAL_FIELD,_T("Adjust..."));
		m_FlexGrid.put_TextMatrix(1,RANG_FIELD,strUnit);
		m_FlexGrid.put_TextMatrix(1,FUN_FIELD,NO_APPLICATION);
		m_FlexGrid.put_TextMatrix(1,CUST_FIELD,NO_APPLICATION);
	}

//row 2:
	
	if(m_FlexGrid.get_Row()>=2)
	{
		
		
		nValue=product_register_value[MODBUS_INPUT_MANU_ENABLE];//MODBUS_INPUT_MANU_ENABLE309->545
		 
		if((nValue & 0x01)==1)
		{
			m_FlexGrid.put_TextMatrix(2,AM_FIELD,strman);	
			m_FlexGrid.put_Col(VALUE_FIELD);
			//m_FlexGrid.put_Row(2);
			m_FlexGrid.put_CellBackColor(COLOR_CELL);
		}
		else
		{
			m_FlexGrid.put_TextMatrix(2,AM_FIELD,strAuto);
			m_FlexGrid.put_Col(VALUE_FIELD);
			//m_FlexGrid.put_Row(2);
			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		}
		m_FlexGrid.put_TextMatrix(2,CAL_FIELD,_T("Adjust..."));
		strTemp.Empty();



		m_crange=0;
	 
			nValue=product_register_value[MODBUS_ANALOG_IN1];	//189
			if(nValue>=0)
			{

				if(product_register_value[MODBUS_DEGC_OR_F]==0)//121
				{
					strTemp=analog_range_TSTAT6[nValue];
				}
				else
				{
					strTemp=analog_range_TSTAT6[nValue];
				}
				m_FlexGrid.put_TextMatrix(2,RANG_FIELD,strTemp);
			}
	 


		


		strTemp.Empty();
		strUnit=GetTempUnit(product_register_value[MODBUS_ANALOG_IN1], 1);//188
		if(product_register_value[MODBUS_ANALOG_IN1]==4||product_register_value[MODBUS_ANALOG_IN1]==1)//188
		{	
			strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_EXTERNAL_SENSOR_0]/10);	//180
			strTemp=strTemp+strUnit;
		}
		if(product_register_value[MODBUS_ANALOG_IN1]==0)
		{
					strTemp.Format(_T("%d"),product_register_value[180]);
			// 		strTemp=strTemp;
			//strTemp=_T("Raw");//2.5.0.98
		}
		if(product_register_value[MODBUS_ANALOG_IN1]==2)
		{
			strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_0]);//180
			strTemp=strTemp+_T("%");
		}

		if ((nValue & 0x01)==1)//Auto Model
		{

			if(product_register_value[MODBUS_ANALOG_IN1]==3 || product_register_value[MODBUS_ANALOG_IN1]==5)
			{

				if (m_crange==6||m_crange==7)
				{

					if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==1)//181
						strTemp=_T("Occupied");
					if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==0)//181
						strTemp=_T("Unoccupied");


				} 
				else
				{

					if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==0)//181
						strTemp=_T("Off");
					if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==1)//181
						strTemp=_T("On");

				}


			}

			
		}
		else//Mannul
		{
			if(product_register_value[MODBUS_ANALOG_IN1]==3 || product_register_value[MODBUS_ANALOG_IN1]==5)
			{

				if (m_crange==6||m_crange==7)
				{
					
						if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==0)//181
							strTemp=_T("Unoccupied");
						if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==1)//181
							strTemp=_T("Occupied");


					

				} 
				else
				{
					
						if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==0)//181
							strTemp=_T("Off");
						if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==1)//181
							strTemp=_T("On");
					
				}


			}
		}
	
	
	
	
	
		m_FlexGrid.put_TextMatrix(2,VALUE_FIELD,strTemp);


		strTemp.Empty();
		nValue=product_register_value[MODBUS_ANALOG1_FUNCTION];//298
		strTemp=INPUT_FUNS[nValue];
		m_FlexGrid.put_TextMatrix(2,FUN_FIELD,strTemp);

		if(product_register_value[MODBUS_ANALOG_IN1]==4)//188
		{
			m_FlexGrid.put_TextMatrix(2,CUST_FIELD,_T("Custom..."));
		}
		else
		{
			m_customBtn.ShowWindow(SW_HIDE);
			m_FlexGrid.put_TextMatrix(2,CUST_FIELD,NO_APPLICATION);
		}
	}
//Row 3:
	if(m_FlexGrid.get_Row()>=3)
	{

	  
		nValue=product_register_value[MODBUS_INPUT_MANU_ENABLE];//MODBUS_INPUT_MANU_ENABLE309->545
		 
		if(((int)(nValue& 0x02)>>1)==1)
		{
			m_FlexGrid.put_TextMatrix(3,AM_FIELD,strman);
			m_FlexGrid.put_Col(VALUE_FIELD);
			//m_FlexGrid.put_Row(3);
			m_FlexGrid.put_CellBackColor(RGB(255,255,255));
		}
		else
		{
			m_FlexGrid.put_TextMatrix(3,AM_FIELD,strAuto);
			m_FlexGrid.put_Col(VALUE_FIELD);
			//m_FlexGrid.put_Row(3);
			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		}
		m_FlexGrid.put_TextMatrix(3,CAL_FIELD,_T("Adjust..."));
		strTemp.Empty();
		m_crange=0;
		
	 
			nValue=product_register_value[MODBUS_ANALOG_IN2];	//189
			if(nValue>=0)
			{

				if(product_register_value[MODBUS_DEGC_OR_F]==0)//121
				{
					strTemp=analog_range_TSTAT6[nValue];
				}
				else
				{
					strTemp=analog_range_TSTAT6[nValue];
				}
				m_FlexGrid.put_TextMatrix(3,RANG_FIELD,strTemp);
			}
		 
		 


		/////////////////////////////////
		strTemp.Empty();
		if(product_register_value[MODBUS_ANALOG_IN2]==4||product_register_value[MODBUS_ANALOG_IN2]==1)//189
		{
			strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_EXTERNAL_SENSOR_1]/10);	//181
			strTemp=strTemp+strUnit;
		}
		//if (product_register_value[189]==0||product_register_value[189]==2)
		if (product_register_value[MODBUS_ANALOG_IN2]==2)//2.5.0.98		//189
		{
			strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_1]);//181
			strTemp=strTemp+strUnit;
		}
		nValue=product_register_value[MODBUS_INPUT_MANU_ENABLE];
		if (((int)(nValue& 0x02)>>1)==1) 
		{
		
			if(product_register_value[MODBUS_ANALOG_IN2]==3 || product_register_value[MODBUS_ANALOG_IN2]==5)	//189
			{
				if (m_crange==6||m_crange==7)
				{

					if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==1)//181
						strTemp=_T("Occupied");
					if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==0)//181
						strTemp=_T("Unoccupied");




				} 
				else
				{

					if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==0)//181
						strTemp=_T("Off");
					if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==1)//181
						strTemp=_T("On");

				}



			}
		} 
		else
		{
			
			if(product_register_value[MODBUS_ANALOG_IN2]==3 || product_register_value[MODBUS_ANALOG_IN2]==5)	//189
			{
				if (m_crange==6||m_crange==7)
				{
					
						if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==1)//181
							strTemp=_T("Occupied");
						if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==0)//181
							strTemp=_T("Unoccupied");


					

				} 
				else
				{
					
						if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==0)//181
							strTemp=_T("Off");
						if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==1)//181
							strTemp=_T("On");
					
				}



			}
		}





 		if (product_register_value[MODBUS_ANALOG_IN2] == 0)	//189
 		{
 			strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_1]);//2.5.0.98
 		}
         

		m_FlexGrid.put_TextMatrix(3,VALUE_FIELD,strTemp);	

		strTemp.Empty();
		nValue=product_register_value[MODBUS_ANALOG2_FUNCTION];	//299
		strTemp=INPUT_FUNS[nValue];
		m_FlexGrid.put_TextMatrix(3,FUN_FIELD,strTemp);

		if(product_register_value[MODBUS_ANALOG_IN2]==4)	//189
		{
			m_FlexGrid.put_TextMatrix(3,CUST_FIELD,_T("Custom..."));
		}
		else
		{
			m_customBtn.ShowWindow(SW_HIDE);
			m_FlexGrid.put_TextMatrix(3,CUST_FIELD,NO_APPLICATION);
		}
	}
//row 4:
	if(m_FlexGrid.get_Row()>=4)
	{	
	    
       nValue=product_register_value[545];

	   CppSQLite3DB SqliteDBBuilding;
	   CppSQLite3Table table;
	   CppSQLite3Query q;
	   SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

	   if (SqliteDBBuilding.tableExists("Value_Range"))//有Version表
	   {
		   CString sql;
		   sql.Format(_T("Select * from Value_Range where CInputNo=%d and SN=%d"),m_FlexGrid.get_Row()-1,m_sn);
		   q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);

		   if (!q.eof())//有表但是没有对应序列号的值
		   {   
		    
		   while (!q.eof())
		   {

			   m_crange=q.getIntField("CRange");
			  q.nextRow();
		   }
		  
		   if (nValue==0)//Auto Model
		   { strTemp=_T("");
			   if((product_register_value[MODBUS_DIGITAL_IN1]==0)&&(m_crange==0))//546
			   {
				   CString strTemp2;
				   if(product_register_value[311]==0)
				   {
					   strTemp=_T("Off");
				   }
				   else
				   {
					   strTemp=_T("On");
				   }
				   strTemp2=_T("On/Off");
				   m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
			   }
			   else  if((product_register_value[MODBUS_DIGITAL_IN1]==0)&&(m_crange==2))//546
			   {
				   CString strTemp2;
				   if (product_register_value[311]==0)
				   {
					   strTemp=_T("UnOccupied");

				   }else
				   {
					   strTemp=_T("Occupied");
				   }


				   strTemp2=_T("Occupied/UnOccupied");
				   m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
			   }
			  else if ((product_register_value[MODBUS_DIGITAL_IN1]==1)&&(m_crange==1))//546
			   {
				   CString strTemp2;
				   if (product_register_value[311]==0)
				   {
					   strTemp=_T("Off");

				   }else
				   {
					   strTemp=_T("On");
				   }
				   strTemp2=_T("Off/On");
				   m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
			   }
			   else if ((product_register_value[MODBUS_DIGITAL_IN1]==1)&&(m_crange==3))//546
			   {
				   CString strTemp2;
				   if (product_register_value[311]==1)
				   {
					   strTemp=_T("Occupied");
				   }
				   else
				   {
					   strTemp=_T("UnOccupied");
				   }
				   strTemp2=_T("UnOccupied/Occupied");
				   m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
			   }
		   } 
		   else
		   {
		       strTemp=_T("");
			   if((product_register_value[MODBUS_DIGITAL_IN1]==0)&&(m_crange==0))//546
			   {
				   CString strTemp2;
				   if(product_register_value[311]==0)
				   {
					   strTemp=_T("Off");
				   }
				   else
				   {
					   strTemp=_T("On");
				   }
				   strTemp2=_T("On/Off");
				   m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
			   }
			   else  if((product_register_value[MODBUS_DIGITAL_IN1]==0)&&(m_crange==2))//546
			   {
				   CString strTemp2;
				   if (product_register_value[311]==0)
				   {
					   strTemp=_T("UnOccupied");

				   }else
				   {
					   strTemp=_T("Occupied");
				   }


				   strTemp2=_T("Occupied/UnOccupied");
				   m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
			   }
			  else if ((product_register_value[MODBUS_DIGITAL_IN1]==1)&&(m_crange==1))//546
			   {
				   CString strTemp2;
				   if(product_register_value[311]==0)
				   {
					   strTemp=_T("Off");
				   }
				   else
				   {
					   strTemp=_T("On");
				   }
				   strTemp2=_T("Off/On");
				   m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
			   }
			   else if ((product_register_value[MODBUS_DIGITAL_IN1]==1)&&(m_crange==3))//546
			   {
				   CString strTemp2;
				   if (product_register_value[311]==0)
				   {
					   strTemp=_T("UnOccupied");

				   }else
				   {
					   strTemp=_T("Occupied");
				   }
				   strTemp2=_T("UnOccupied/Occupied");
				   m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
			   }
		   }
		  
		  
		 
		   m_FlexGrid.put_TextMatrix(4,VALUE_FIELD,strTemp);
		   } 
		   else
		   {   
		   if (nValue==0)
		   { strTemp=_T("");
			   if (product_register_value[MODBUS_DIGITAL_IN1]==0)//546
			   {
				   CString strTemp2;

				   if (product_register_value[311]==1)
				   {
					   strTemp=_T("On");
				   }else
				   {
					   strTemp=_T("Off");
				   }
				   strTemp2=_T("On/Off");
				   m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
			   }
			   if (product_register_value[MODBUS_DIGITAL_IN1]==1)//546
			   { strTemp=_T("");
				   CString strTemp2;
				   if (product_register_value[311]==0)
				   {
					   strTemp=_T("Off");

				   }else
				   {
					   strTemp=_T("On");
				   }
				   strTemp2=_T("Off/On");
				   m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
			   }
			   
		   } 
		   else
		   {
		    
			   if (product_register_value[MODBUS_DIGITAL_IN1]==0)//546
			   {strTemp=_T("");
				   CString strTemp2;

				   if (product_register_value[311]==1)
				   {
					   strTemp=_T("On");
				   }else
				   {
					   strTemp=_T("Off");
				   }
				   strTemp2=_T("On/Off");
				   m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
			   }
			   if (product_register_value[MODBUS_DIGITAL_IN1]==1)//546
			   {
				   CString strTemp2;
				   if (product_register_value[311]==1)
				   {
					   strTemp=_T("On");
				   }else
				   {
					   strTemp=_T("Off");
				   }
				   strTemp2=_T("Off/On");
				   m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
			   }
		   }
			   
			   m_FlexGrid.put_TextMatrix(4,VALUE_FIELD,strTemp);
		   }

		    
	   }
	   else
	   { 
		   if (nValue==0)
		   {
			   if (product_register_value[MODBUS_DIGITAL_IN1]==0)//546
			   {strTemp=_T("");
				   CString strTemp2;

				   if (product_register_value[311]==1)
				   {
					   strTemp=_T("On");
				   }else
				   {
					   strTemp=_T("Off");
				   }
				   strTemp2=_T("On/Off");
				   m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
			   } 
			   if (product_register_value[MODBUS_DIGITAL_IN1]==1)//546
			   {strTemp=_T("");
				   CString strTemp2;
				   if (product_register_value[311]==0)
				   {
					   strTemp=_T("Off");

				   }else
				   {
					   strTemp=_T("On");
				   }
				   strTemp2=_T("Off/On");
				   m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
			   }

		   } 
		   else
		   {

			   
				   CString strTemp2;
strTemp=_T("");
				   if (product_register_value[311]==1)
				   {
					   strTemp=_T("On");
				   }else
				   {
					   strTemp=_T("Off");
				   }
				   strTemp2=_T("On/Off");
				   m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
			   
			  
		   }

		   m_FlexGrid.put_TextMatrix(4,VALUE_FIELD,strTemp);
	   }
	  SqliteDBBuilding.closedb();

	
	    


		
		
		nValue=product_register_value[MODBUS_HUM_CAL_EREASE];//309MODBUS_INPUT_MANU_ENABLE-545
		if(nValue==1)
		{
			m_FlexGrid.put_TextMatrix(4,AM_FIELD,strman);
			m_FlexGrid.put_Col(VALUE_FIELD);
			m_FlexGrid.put_Row(4);
			m_FlexGrid.put_CellBackColor(COLOR_CELL);
		}
		else
		{
			m_FlexGrid.put_TextMatrix(4,AM_FIELD,strAuto);
			m_FlexGrid.put_Col(VALUE_FIELD);
			m_FlexGrid.put_Row(4);
			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		}
		m_FlexGrid.put_TextMatrix(4,CAL_FIELD,NO_APPLICATION);
		
		strTemp.Empty();
		nValue=product_register_value[MODBUS_DIGITAL1_FUNCTION];	//300
		if(nValue>=0)
			strTemp=INPUT_FUNS[nValue];
		m_FlexGrid.put_TextMatrix(4,FUN_FIELD,strTemp);
		m_FlexGrid.put_TextMatrix(4,CUST_FIELD,NO_APPLICATION);
	}
}
void CInputSetDlg::Fresh_GridForAll(){
	if(m_nModel == 16)
	{
		Fresh_GridForTstat5E();
		return ;
	}
	if (m_nModel==PM_TSTAT6||m_nModel==PM_TSTAT5i||m_nModel==PM_TSTAT8 || m_nModel == PM_TSTAT9 ||m_nModel==PM_TSTAT7
		|| (m_nModel == PM_TSTAT8_WIFI) || (m_nModel == PM_TSTAT8_OCC) || (m_nModel == PM_TSTAT7_ARM) || (m_nModel == PM_TSTAT8_220V))
	{
		//InitGridtstat6();
		Fresh_GridForTstat6();
		return ;
	}


	CString strUnit=GetTempUnit();
	CString strTemp;	
	int nValue=0;
	CString strAuto=_T("Auto");
	CString strman=_T("Manual");
	//row 1:	
	if(m_FlexGrid.get_Row()>=1)
	{
		strTemp.Format(_T("%.1f"),product_register_value[MODBUS_TEMPRATURE_CHIP]/10.0);//101
		strTemp=strTemp+strUnit;
		m_FlexGrid.put_TextMatrix(1,VALUE_FIELD,strTemp);
		m_FlexGrid.put_TextMatrix(1,AM_FIELD,NO_APPLICATION);
		m_FlexGrid.put_TextMatrix(1,CAL_FIELD,_T("Adjust..."));
		m_FlexGrid.put_TextMatrix(1,RANG_FIELD,strUnit);
		m_FlexGrid.put_TextMatrix(1,FUN_FIELD,NO_APPLICATION);
		m_FlexGrid.put_TextMatrix(1,CUST_FIELD,NO_APPLICATION);
	}

	//row 2:

	if(m_FlexGrid.get_Row()>=2)
	{


		nValue=product_register_value[MODBUS_INPUT_MANU_ENABLE];//MODBUS_INPUT_MANU_ENABLE309->545

		if((nValue & 0x01)==1)
		{
			m_FlexGrid.put_TextMatrix(2,AM_FIELD,strman);	
			m_FlexGrid.put_Col(VALUE_FIELD);
			//m_FlexGrid.put_Row(2);
			m_FlexGrid.put_CellBackColor(COLOR_CELL);
		}
		else
		{
			m_FlexGrid.put_TextMatrix(2,AM_FIELD,strAuto);
			m_FlexGrid.put_Col(VALUE_FIELD);
			//m_FlexGrid.put_Row(2);
			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		}
		m_FlexGrid.put_TextMatrix(2,CAL_FIELD,_T("Adjust..."));
		strTemp.Empty();



		m_crange=0;
		 
				nValue=product_register_value[MODBUS_ANALOG_IN1];	//189
				if(nValue>=0)
				{

					if(product_register_value[MODBUS_DEGC_OR_F]==0)//121
					{
						strTemp=analog_range[nValue];
					}
					else
					{
						strTemp=analog_range[nValue];
					}
					m_FlexGrid.put_TextMatrix(2,RANG_FIELD,strTemp);
				}
		 

		 
			nValue=product_register_value[MODBUS_ANALOG_IN1];	//189
			if(nValue>=0)
			{

				if(product_register_value[MODBUS_DEGC_OR_F]==0)//121
				{
					strTemp=analog_range_TSTAT6[nValue];
				}
				else
				{
					strTemp=analog_range_TSTAT6[nValue];
				}
				m_FlexGrid.put_TextMatrix(2,RANG_FIELD,strTemp);
			}
		 





		strTemp.Empty();
		strUnit=GetTempUnit(product_register_value[MODBUS_ANALOG_IN1], 1);//188
		if(product_register_value[MODBUS_ANALOG_IN1]==4||product_register_value[MODBUS_ANALOG_IN1]==1)//188
		{	
			strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_EXTERNAL_SENSOR_0]/10);	//180
			strTemp=strTemp+strUnit;
		}
		if(product_register_value[MODBUS_ANALOG_IN1]==0)
		{
			strTemp.Format(_T("%d"),product_register_value[180]);
			// 		strTemp=strTemp;
			//strTemp=_T("Raw");//2.5.0.98
		}
		if(product_register_value[MODBUS_ANALOG_IN1]==2)
		{
			strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_0]);//180
			strTemp=strTemp+_T("%");
		}

		if ((nValue & 0x01)==1)//Auto Model
		{

			if(product_register_value[MODBUS_ANALOG_IN1]==3 || product_register_value[MODBUS_ANALOG_IN1]==5)
			{

				if (m_crange==6||m_crange==7)
				{

					if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==1)//181
						strTemp=_T("Occupied");
					if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==0)//181
						strTemp=_T("Unoccupied");


				} 
				else
				{

					if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==0)//181
						strTemp=_T("Off");
					if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==1)//181
						strTemp=_T("On");

				}


			}


		}
		else//Mannul
		{
			if(product_register_value[MODBUS_ANALOG_IN1]==3 || product_register_value[MODBUS_ANALOG_IN1]==5)
			{

				if (m_crange==6||m_crange==7)
				{

					if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==0)//181
						strTemp=_T("Unoccupied");
					if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==1)//181
						strTemp=_T("Occupied");




				} 
				else
				{

					if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==0)//181
						strTemp=_T("Off");
					if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==1)//181
						strTemp=_T("On");

				}


			}
		}





		m_FlexGrid.put_TextMatrix(2,VALUE_FIELD,strTemp);


		strTemp.Empty();
		nValue=product_register_value[MODBUS_ANALOG1_FUNCTION];//298
		strTemp=INPUT_FUNS[nValue];
		m_FlexGrid.put_TextMatrix(2,FUN_FIELD,strTemp);

		if(product_register_value[MODBUS_ANALOG_IN1]==4)//188
		{
			m_FlexGrid.put_TextMatrix(2,CUST_FIELD,_T("Custom..."));
		}
		else
		{
			m_customBtn.ShowWindow(SW_HIDE);
			m_FlexGrid.put_TextMatrix(2,CUST_FIELD,NO_APPLICATION);
		}
	}
	//Row 3:
	if(m_FlexGrid.get_Row()>=3)
	{


		nValue=product_register_value[MODBUS_INPUT_MANU_ENABLE];//MODBUS_INPUT_MANU_ENABLE309->545

		if(((int)(nValue& 0x02)>>1)==1)
		{
			m_FlexGrid.put_TextMatrix(3,AM_FIELD,strman);
			m_FlexGrid.put_Col(VALUE_FIELD);
			//m_FlexGrid.put_Row(3);
			m_FlexGrid.put_CellBackColor(RGB(255,255,255));
		}
		else
		{
			m_FlexGrid.put_TextMatrix(3,AM_FIELD,strAuto);
			m_FlexGrid.put_Col(VALUE_FIELD);
			//m_FlexGrid.put_Row(3);
			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		}
		m_FlexGrid.put_TextMatrix(3,CAL_FIELD,_T("Adjust..."));
		strTemp.Empty();
		m_crange=0;

	 
			nValue=product_register_value[MODBUS_ANALOG_IN2];	//189
			if(nValue>=0)
			{

				if(product_register_value[MODBUS_DEGC_OR_F]==0)//121
				{
					strTemp=analog_range_TSTAT6[nValue];
				}
				else
				{
					strTemp=analog_range_TSTAT6[nValue];
				}
				m_FlexGrid.put_TextMatrix(3,RANG_FIELD,strTemp);
			}
	 


		/////////////////////////////////
		strTemp.Empty();
		if(product_register_value[MODBUS_ANALOG_IN2]==4||product_register_value[MODBUS_ANALOG_IN2]==1)//189
		{
			strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_EXTERNAL_SENSOR_1]/10);	//181
			strTemp=strTemp+strUnit;
		}
		//if (product_register_value[189]==0||product_register_value[189]==2)
		if (product_register_value[MODBUS_ANALOG_IN2]==2)//2.5.0.98		//189
		{
			strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_1]);//181
			strTemp=strTemp+strUnit;
		}
		nValue=product_register_value[MODBUS_INPUT_MANU_ENABLE];
		if (((int)(nValue& 0x02)>>1)==1) 
		{

			if(product_register_value[MODBUS_ANALOG_IN2]==3 || product_register_value[MODBUS_ANALOG_IN2]==5)	//189
			{
				if (m_crange==6||m_crange==7)
				{

					if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==1)//181
						strTemp=_T("Occupied");
					if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==0)//181
						strTemp=_T("Unoccupied");




				} 
				else
				{

					if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==0)//181
						strTemp=_T("Off");
					if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==1)//181
						strTemp=_T("On");

				}



			}
		} 
		else
		{

			if(product_register_value[MODBUS_ANALOG_IN2]==3 || product_register_value[MODBUS_ANALOG_IN2]==5)	//189
			{
				if (m_crange==6||m_crange==7)
				{

					if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==1)//181
						strTemp=_T("Occupied");
					if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==0)//181
						strTemp=_T("Unoccupied");




				} 
				else
				{

					if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==0)//181
						strTemp=_T("Off");
					if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==1)//181
						strTemp=_T("On");

				}



			}
		}





		if (product_register_value[MODBUS_ANALOG_IN2] == 0)	//189
		{
			strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_1]);//2.5.0.98
		}


		m_FlexGrid.put_TextMatrix(3,VALUE_FIELD,strTemp);	

		strTemp.Empty();
		nValue=product_register_value[MODBUS_ANALOG2_FUNCTION];	//299
		strTemp=INPUT_FUNS[nValue];
		m_FlexGrid.put_TextMatrix(3,FUN_FIELD,strTemp);

		if(product_register_value[MODBUS_ANALOG_IN2]==4)	//189
		{
			m_FlexGrid.put_TextMatrix(3,CUST_FIELD,_T("Custom..."));
		}
		else
		{
			m_customBtn.ShowWindow(SW_HIDE);
			m_FlexGrid.put_TextMatrix(3,CUST_FIELD,NO_APPLICATION);
		}
	}
	//row 4:
	if(m_FlexGrid.get_Row()>=4)
	{	

		nValue=product_register_value[545];

		CppSQLite3DB SqliteDBT3000;
		CppSQLite3Table table;
		CppSQLite3Query q;
		SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);

		if (SqliteDBT3000.tableExists("Value_Range"))//有Version表
		{
			CString sql;
			sql.Format(_T("Select * from Value_Range where CInputNo=%d and SN=%d"),m_FlexGrid.get_Row()-1,m_sn);
			q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);

			if (!q.eof())//有表但是没有对应序列号的值
			{   
				while (!q.eof())
				{

					m_crange=q.getIntField("CRange");
					q.nextRow();
				}

			if (nValue==0)//Auto Model
			{ 
			strTemp=_T("");
			if((product_register_value[MODBUS_DIGITAL_IN1]==0)&&(m_crange==0))//546
			{
				CString strTemp2;
				if(product_register_value[311]==0)
				{
					strTemp=_T("Off");
				}
				else
				{
					strTemp=_T("On");
				}
				strTemp2=_T("On/Off");
				m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
			}
			else  if((product_register_value[MODBUS_DIGITAL_IN1]==0)&&(m_crange==2))//546
			{
				CString strTemp2;
				if (product_register_value[311]==0)
				{
					strTemp=_T("UnOccupied");

				}else
				{
					strTemp=_T("Occupied");
				}


				strTemp2=_T("Occupied/UnOccupied");
				m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
			}
			else if ((product_register_value[MODBUS_DIGITAL_IN1]==1)&&(m_crange==1))//546
			{
				CString strTemp2;
				if (product_register_value[311]==0)
				{
					strTemp=_T("Off");

				}else
				{
					strTemp=_T("On");
				}
				strTemp2=_T("Off/On");
				m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
			}
			else if ((product_register_value[MODBUS_DIGITAL_IN1]==1)&&(m_crange==3))//546
			{
				CString strTemp2;
				if (product_register_value[311]==1)
				{
					strTemp=_T("Occupied");
				}
				else
				{
					strTemp=_T("UnOccupied");
				}
				strTemp2=_T("UnOccupied/Occupied");
				m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
			}
			} 
			else
			{
				strTemp=_T("");
				if((product_register_value[MODBUS_DIGITAL_IN1]==0)&&(m_crange==0))//546
				{
					CString strTemp2;
					if(product_register_value[311]==0)
					{
						strTemp=_T("Off");
					}
					else
					{
						strTemp=_T("On");
					}
					strTemp2=_T("On/Off");
					m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
				}
				else  if((product_register_value[MODBUS_DIGITAL_IN1]==0)&&(m_crange==2))//546
				{
					CString strTemp2;
					if (product_register_value[311]==0)
					{
						strTemp=_T("UnOccupied");

					}else
					{
						strTemp=_T("Occupied");
					}


					strTemp2=_T("Occupied/UnOccupied");
					m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
				}
				else if ((product_register_value[MODBUS_DIGITAL_IN1]==1)&&(m_crange==1))//546
				{
					CString strTemp2;
					if(product_register_value[311]==0)
					{
						strTemp=_T("Off");
					}
					else
					{
						strTemp=_T("On");
					}
					strTemp2=_T("Off/On");
					m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
				}
				else if ((product_register_value[MODBUS_DIGITAL_IN1]==1)&&(m_crange==3))//546
				{
					CString strTemp2;
					if (product_register_value[311]==0)
					{
						strTemp=_T("UnOccupied");

					}else
					{
						strTemp=_T("Occupied");
					}
					strTemp2=_T("UnOccupied/Occupied");
					m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
				}
			}



			m_FlexGrid.put_TextMatrix(4,VALUE_FIELD,strTemp);
			} 
			else
			{   
				if (nValue==0)
				{ strTemp=_T("");
				if (product_register_value[MODBUS_DIGITAL_IN1]==0)//546
				{
					CString strTemp2;

					if (product_register_value[311]==1)
					{
						strTemp=_T("On");
					}else
					{
						strTemp=_T("Off");
					}
					strTemp2=_T("On/Off");
					m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
				}
				if (product_register_value[MODBUS_DIGITAL_IN1]==1)//546
				{ strTemp=_T("");
				CString strTemp2;
				if (product_register_value[311]==0)
				{
					strTemp=_T("Off");

				}else
				{
					strTemp=_T("On");
				}
				strTemp2=_T("Off/On");
				m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
				}

				} 
				else
				{

					if (product_register_value[MODBUS_DIGITAL_IN1]==0)//546
					{strTemp=_T("");
					CString strTemp2;

					if (product_register_value[311]==1)
					{
						strTemp=_T("On");
					}else
					{
						strTemp=_T("Off");
					}
					strTemp2=_T("On/Off");
					m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
					}
					if (product_register_value[MODBUS_DIGITAL_IN1]==1)//546
					{
						CString strTemp2;
						if (product_register_value[311]==1)
						{
							strTemp=_T("On");
						}else
						{
							strTemp=_T("Off");
						}
						strTemp2=_T("Off/On");
						m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
					}
				}

				m_FlexGrid.put_TextMatrix(4,VALUE_FIELD,strTemp);
			}

			 
		}
		else
		{ 
			if (nValue==0)
			{
				if (product_register_value[MODBUS_DIGITAL_IN1]==0)//546
				{strTemp=_T("");
				CString strTemp2;

				if (product_register_value[311]==1)
				{
					strTemp=_T("On");
				}else
				{
					strTemp=_T("Off");
				}
				strTemp2=_T("On/Off");
				m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
				} 
				if (product_register_value[MODBUS_DIGITAL_IN1]==1)//546
				{strTemp=_T("");
				CString strTemp2;
				if (product_register_value[311]==0)
				{
					strTemp=_T("Off");

				}else
				{
					strTemp=_T("On");
				}
				strTemp2=_T("Off/On");
				m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
				}

			} 
			else
			{


				CString strTemp2;
				strTemp=_T("");
				if (product_register_value[311]==1)
				{
					strTemp=_T("On");
				}else
				{
					strTemp=_T("Off");
				}
				strTemp2=_T("On/Off");
				m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);


			}

			m_FlexGrid.put_TextMatrix(4,VALUE_FIELD,strTemp);
		}
		 SqliteDBT3000.closedb();







		nValue=product_register_value[MODBUS_HUM_CAL_EREASE];//309MODBUS_INPUT_MANU_ENABLE-545
		if(nValue==1)
		{
			m_FlexGrid.put_TextMatrix(4,AM_FIELD,strman);
			m_FlexGrid.put_Col(VALUE_FIELD);
			m_FlexGrid.put_Row(4);
			m_FlexGrid.put_CellBackColor(COLOR_CELL);
		}
		else
		{
			m_FlexGrid.put_TextMatrix(4,AM_FIELD,strAuto);
			m_FlexGrid.put_Col(VALUE_FIELD);
			m_FlexGrid.put_Row(4);
			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		}
		m_FlexGrid.put_TextMatrix(4,CAL_FIELD,NO_APPLICATION);

		strTemp.Empty();
		nValue=product_register_value[MODBUS_DIGITAL1_FUNCTION];	//300
		if(nValue>=0)
			strTemp=INPUT_FUNS[nValue];
		m_FlexGrid.put_TextMatrix(4,FUN_FIELD,strTemp);
		m_FlexGrid.put_TextMatrix(4,CUST_FIELD,NO_APPLICATION);
	}

	SetCurrentCell();
}
BEGIN_EVENTSINK_MAP(CInputSetDlg, CDialog)
	//ON_EVENT(CInputSetDlg, IDC_MSFLEXGRID1, DISPID_CLICK, CInputSetDlg::ClickMsflexgrid1, VTS_NONE)
	//ON_EVENT(CInputSetDlg, IDC_MSFLEXGRID1, 71, CInputSetDlg::EnterCellMsflexgrid1, VTS_NONE)
//	ON_EVENT(CInputSetDlg, IDC_MSFLEXGRID1, DISPID_KEYDOWN, CInputSetDlg::KeyDownMsflexgrid1, VTS_PI2 VTS_I2)
	ON_EVENT(CInputSetDlg, IDC_MSFLEXGRID1, DISPID_DBLCLICK, CInputSetDlg::DblClickMsflexgrid1, VTS_NONE)
	ON_EVENT(CInputSetDlg, IDC_MSFLEXGRID1, DISPID_CLICK, CInputSetDlg::ClickMsflexgrid1, VTS_NONE)
END_EVENTSINK_MAP()

void CInputSetDlg::ClickMsflexgrid_Click()
{
	long lRow,lCol;
	
	lRow = m_FlexGrid.get_RowSel();//获取点击的行号	
	lCol = m_FlexGrid.get_ColSel(); //获取点击的列号
	if(lRow>m_FlexGrid.get_Rows()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow == 0) //如果点击标题行，也无效
		return;
	CRect rect;
	m_FlexGrid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	// MSFlexGrid控件的函数的长度单位是"缇(twips)"，
	//需要将其转化为像素，1440缇= 1英寸
	CDC* pDC =GetDC();
	//计算象素点和缇的转换比例
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_FlexGrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_FlexGrid.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_FlexGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_FlexGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rc.OffsetRect(rect.left+1,rect.top+1);
	//获取选中格的文本信息
	CString strValue = m_FlexGrid.get_TextMatrix(lRow,lCol);
	m_nCurRow=lRow;
	m_nCurCol=lCol;
	
	if (m_nModel == PM_TSTAT7||m_nModel == PM_TSTAT6||m_nModel == PM_TSTAT5i||m_nModel == PM_TSTAT8 || m_nModel == PM_TSTAT9
		|| (m_nModel == PM_TSTAT8_WIFI) || (m_nModel == PM_TSTAT8_OCC) || (m_nModel == PM_TSTAT7_ARM) || (m_nModel == PM_TSTAT8_220V))
	{
		OnClickTstat6Grid(m_nCurRow, m_nCurCol, rc);
		return;
	}
	if ((m_nModel == PM_TSTAT5E||m_nModel == PM_PM5E || m_nModel == PM_PM5E_ARM)||(product_register_value[7]==PM_TSTATRUNAR))
	{
		ClickMsflexgrid5E(m_nCurRow, m_nCurCol, rc);
		return;
	}
	
	if(lCol==VALUE_FIELD)
	{
		WORD wAM=product_register_value[MODBUS_INPUT_MANU_ENABLE];
		unsigned short AM_lrow4=product_register_value[MODBUS_HUM_CAL_EREASE];
		if(lRow==1)
		{
			m_inValueEdit.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_inValueEdit.ShowWindow(SW_SHOW);
			
			m_inValueEdit.BringWindowToTop();
			//m_RangCombox.SelectString(-1,strValue);
			m_inValueEdit.SetWindowText(strValue);
			m_inValueEdit.SetFocus(); //获取焦点
		}
		if(lRow==2 && (wAM & 0x01))
		{
			
			CppSQLite3DB SqliteDBBuilding;
			CppSQLite3Table table;
			CppSQLite3Query q;
			SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

			if (SqliteDBBuilding.tableExists("Value_Range"))//有Version表
			{
				CString sql;
				sql.Format(_T("Select * from Value_Range where CInputNo=%d and SN=%d"),1,get_serialnumber());
				q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);


				if (!q.eof())//有表但是没有对应序列号的值
				{    
					 
					while (!q.eof())
					{
						m_crange=q.getIntField("CRange");
						q.nextRow();
					}
						if(product_register_value[MODBUS_ANALOG_IN1]==3||product_register_value[MODBUS_ANALOG_IN1]==5)//On OFF
						{  	m_valueCombx.ResetContent();
					        if((product_register_value[MODBUS_ANALOG_IN1]==3)&&(m_crange==3))//On/OFF
							{
								m_valueCombx.InsertString(0,_T("Off"));
								m_valueCombx.InsertString(1,_T("On"));
					        }
						else if((product_register_value[MODBUS_ANALOG_IN1]==3)&&(m_crange==6))//On/OFF
						{
							m_valueCombx.InsertString(0,_T("Occupied"));
							m_valueCombx.InsertString(1,_T("UnOccupied"));
						} 
						if((product_register_value[MODBUS_ANALOG_IN1]==5)&&(m_crange==5))//On/OFF
						{
							m_valueCombx.InsertString(0,_T("Off"));
							m_valueCombx.InsertString(1,_T("On"));
						}
						else if((product_register_value[MODBUS_ANALOG_IN1]==5)&&(m_crange==7))//On/OFF
						{

							m_valueCombx.InsertString(0,_T("Occupied"));
							m_valueCombx.InsertString(1,_T("UnOccupied"));
						} 
						m_valueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
						m_valueCombx.ShowWindow(SW_SHOW);
						m_valueCombx.BringWindowToTop();
						m_valueCombx.SelectString(-1,strValue);
						m_valueCombx.SetFocus(); //获取焦点


					}
					else
					{
						m_inValueEdit.MoveWindow(rc); //移动到选中格的位置，覆盖
						m_inValueEdit.ShowWindow(SW_SHOW);

						m_inValueEdit.BringWindowToTop();
						//m_RangCombox.SelectString(-1,strValue);
						m_inValueEdit.SetWindowText(strValue);
						m_inValueEdit.SetFocus(); //获取焦点
					}
				} 
				else
				{
					if(product_register_value[MODBUS_ANALOG_IN1]==3||product_register_value[MODBUS_ANALOG_IN1]==5)//On OFF
					{	m_valueCombx.ResetContent();
						if((product_register_value[MODBUS_ANALOG_IN1]==3))//On/OFF
						{
							m_valueCombx.InsertString(0,_T("Off"));
							m_valueCombx.InsertString(1,_T("On"));
						}
						else if((product_register_value[MODBUS_ANALOG_IN1]==5))//On/OFF
						{
							m_valueCombx.InsertString(0,_T("Off"));
							m_valueCombx.InsertString(1,_T("On"));
						}
						m_valueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
						m_valueCombx.ShowWindow(SW_SHOW);
						m_valueCombx.BringWindowToTop();
						m_valueCombx.SelectString(-1,strValue);
						m_valueCombx.SetFocus(); //获取焦点

					}
					else
					{
						m_inValueEdit.MoveWindow(rc); //移动到选中格的位置，覆盖
						m_inValueEdit.ShowWindow(SW_SHOW);

						m_inValueEdit.BringWindowToTop();
						//m_RangCombox.SelectString(-1,strValue);
						m_inValueEdit.SetWindowText(strValue);
						m_inValueEdit.SetFocus(); //获取焦点
					}
				}

				SqliteDBBuilding.closedb();
			}
			else
			{
				if(product_register_value[MODBUS_ANALOG_IN1]==3||product_register_value[MODBUS_ANALOG_IN1]==5)//On OFF
				{    m_valueCombx.ResetContent();
					if((product_register_value[MODBUS_ANALOG_IN1]==3))//On/OFF
					{
						m_valueCombx.InsertString(0,_T("Off"));
						m_valueCombx.InsertString(1,_T("On"));
					}
					else if((product_register_value[MODBUS_ANALOG_IN1]==5))//On/OFF
					{
						m_valueCombx.InsertString(0,_T("Off"));
						m_valueCombx.InsertString(1,_T("On"));
					}
					m_valueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
					m_valueCombx.ShowWindow(SW_SHOW);
					m_valueCombx.BringWindowToTop();
					m_valueCombx.SelectString(-1,strValue);
					m_valueCombx.SetFocus(); //获取焦点

				}
				else
				{
					m_inValueEdit.MoveWindow(rc); //移动到选中格的位置，覆盖
					m_inValueEdit.ShowWindow(SW_SHOW);

					m_inValueEdit.BringWindowToTop();
					//m_RangCombox.SelectString(-1,strValue);
					m_inValueEdit.SetWindowText(strValue);
					m_inValueEdit.SetFocus(); //获取焦点
				}
			}	

			
		}
		if(lRow==3 && (wAM & 0x02))
		{
		



			CppSQLite3DB SqliteDBBuilding;
			CppSQLite3Table table;
			CppSQLite3Query q;
			SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

			if (SqliteDBBuilding.tableExists("Value_Range"))//有Version表
			{
				CString sql;
				sql.Format(_T("Select * from Value_Range where CInputNo=%d and SN=%d"),2,get_serialnumber());
				q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);


				if (!q.eof())//有表但是没有对应序列号的值
				{    
					 
					while (!q.eof())
					{
						m_crange=q.getIntField("CRange");
						q.nextRow();
					}

				 
					if(product_register_value[MODBUS_ANALOG_IN2]==3 || product_register_value[MODBUS_ANALOG_IN2]==5)//188
					{

						m_valueCombx.ResetContent();
						if (m_crange==6||m_crange==7)
						{
							if (m_crange==6)
							{
								
								m_valueCombx.InsertString(0,_T("Occupied"));
								m_valueCombx.InsertString(1,_T("Unoccupied"));
								
							} 
							else
							{
								m_valueCombx.InsertString(0,_T("Occupied"));
								m_valueCombx.InsertString(1,_T("Unoccupied"));
							}

						} 
						else
						{
							if (product_register_value[MODBUS_ANALOG_IN2]==3)
							{
								m_valueCombx.InsertString(0,_T("Off"));
								m_valueCombx.InsertString(1,_T("On"));
							} 
							else
							{
								m_valueCombx.InsertString(0,_T("Off"));
								m_valueCombx.InsertString(1,_T("On"));
							}
						}
						m_valueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
						m_valueCombx.ShowWindow(SW_SHOW);
						m_valueCombx.BringWindowToTop();
						m_valueCombx.SelectString(-1,strValue);
						m_valueCombx.SetFocus(); //获取焦点
					} 
					else
					{
						m_inValueEdit.MoveWindow(rc); //移动到选中格的位置，覆盖
						m_inValueEdit.ShowWindow(SW_SHOW);

						m_inValueEdit.BringWindowToTop();
						//m_RangCombox.SelectString(-1,strValue);
						m_inValueEdit.SetWindowText(strValue);
						m_inValueEdit.SetFocus(); //获取焦点
					}
				}
				else
				{
					if(product_register_value[MODBUS_ANALOG_IN2]==3 || product_register_value[MODBUS_ANALOG_IN2]==5)//188
					{

						m_valueCombx.ResetContent();
						{
							if (product_register_value[MODBUS_ANALOG_IN2]==3)
							{
								m_valueCombx.InsertString(0,_T("Off"));
								m_valueCombx.InsertString(1,_T("On"));
							} 
							else
							{
								m_valueCombx.InsertString(0,_T("Off"));
								m_valueCombx.InsertString(1,_T("On"));
							}
						}
						m_valueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
						m_valueCombx.ShowWindow(SW_SHOW);
						m_valueCombx.BringWindowToTop();
						m_valueCombx.SelectString(-1,strValue);
						m_valueCombx.SetFocus(); //获取焦点
					} 
					else
					{
						m_inValueEdit.MoveWindow(rc); //移动到选中格的位置，覆盖
						m_inValueEdit.ShowWindow(SW_SHOW);

						m_inValueEdit.BringWindowToTop();
						//m_RangCombox.SelectString(-1,strValue);
						m_inValueEdit.SetWindowText(strValue);
						m_inValueEdit.SetFocus(); //获取焦点
					}
				}

				 
			}

			 SqliteDBBuilding.closedb();



		}
		if(lRow==4 && (AM_lrow4==1))
		{
	
			m_valueCombx.ResetContent();


			CppSQLite3DB SqliteDBBuilding;
			CppSQLite3Table table;
			CppSQLite3Query q;
			SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

			if (SqliteDBBuilding.tableExists("Value_Range"))//有Version表
			{
				CString sql;
				sql.Format(_T("Select * from Value_Range where CInputNo=%d and SN=%d"),3,get_serialnumber());
				q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);


				if (!q.eof())//有表但是没有对应序列号的值
				{    
					 
					while (!q.eof())
					{
						m_crange=q.getIntField("CRange");
						q.nextRow();
					}

					if((product_register_value[546]==0)&&(m_crange==0))//On/OFF
					{
						m_valueCombx.InsertString(0,_T("Off"));
						m_valueCombx.InsertString(1,_T("On"));
					}
					else if((product_register_value[546]==0)&&(m_crange==2))//On/OFF
					{
						m_valueCombx.InsertString(0,_T("Occupied"));
						m_valueCombx.InsertString(1,_T("UnOccupied"));
					} 
					if((product_register_value[546]==1)&&(m_crange==1))//On/OFF
					{
						m_valueCombx.InsertString(0,_T("Off"));
						m_valueCombx.InsertString(1,_T("On"));
					}
					else if((product_register_value[546]==1)&&(m_crange==3))//On/OFF
					{

						m_valueCombx.InsertString(0,_T("Occupied"));
						m_valueCombx.InsertString(1,_T("UnOccupied"));
					} 


				} 
				else
				{
					if((product_register_value[546]==0))//On/OFF
					{
						m_valueCombx.InsertString(0,_T("Off"));
						m_valueCombx.InsertString(1,_T("On"));
					}
					
					if((product_register_value[546]==1))//On/OFF
					{
						m_valueCombx.InsertString(0,_T("Off"));
						m_valueCombx.InsertString(1,_T("On"));
					}
					


				}

				 
			}
			else
			{
				if((product_register_value[546]==0))//On/OFF
				{
					m_valueCombx.InsertString(0,_T("Off"));
					m_valueCombx.InsertString(1,_T("On"));
				}

				if((product_register_value[546]==1))//On/OFF
				{
					m_valueCombx.InsertString(0,_T("Off"));
					m_valueCombx.InsertString(1,_T("On"));
				}

			}
			 SqliteDBBuilding.closedb();
				m_valueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
				m_valueCombx.ShowWindow(SW_SHOW);
				m_valueCombx.BringWindowToTop();
				m_valueCombx.SelectString(-1,strValue);
				m_valueCombx.SetFocus(); //获取焦点
		}
		
	}
	if(lCol==RANG_FIELD)
	{	if(lRow==1)
		{
			m_RangCombox.ResetContent();
			for(UINT i=0;i<m_strUnitList.size();i++)
			{
				m_RangCombox.AddString(m_strUnitList.at(i));
			}
			m_RangCombox.ShowWindow(SW_SHOW);//显示控件
			m_RangCombox.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_RangCombox.BringWindowToTop();
			m_RangCombox.SelectString(-1,strValue);
			m_RangCombox.SetFocus(); //获取焦点
		}

		if(((lRow==2)&&(product_register_value[MODBUS_TEMP_SELECT]!=1))||(lRow==3))
		{
			//Raw;10K Therm;0-100%;ON/OFF;Custom Sensor;Off/On;
			m_RangCombox.ResetContent();
			if(product_register_value[121]==0)
			{
				for(int i=0;i<ANALOG_RANG_NUMBER;i++)
				{
					m_RangCombox.AddString(analog_range_TSTAT6[i]);
				}
			}
			if(product_register_value[121]==1)
			{
				for(int i=0;i<ANALOG_RANG_NUMBER;i++)
				{
					m_RangCombox.AddString(analog_range_TSTAT6[i]);
				}
			}

			m_RangCombox.ShowWindow(SW_SHOW);//显示控件
			m_RangCombox.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_RangCombox.BringWindowToTop();
			m_RangCombox.SelectString(-1,strValue);
			m_RangCombox.SetFocus(); //获取焦点
		}
		if(lRow==4) 
		{
			m_RangCombox.ResetContent();
			m_RangCombox.AddString(_T("On/Off"));
			m_RangCombox.AddString(_T("Off/On"));
			m_RangCombox.AddString(_T("Occupied/UnOccupied"));
			m_RangCombox.AddString(_T("UnOccupied/Occupied"));
			m_RangCombox.ShowWindow(SW_SHOW);//显示控件
			m_RangCombox.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_RangCombox.BringWindowToTop();
			m_RangCombox.SelectString(-1,strValue);
			m_RangCombox.SetFocus(); //获
		}
		ShowRangeCombox();
	}

	if(lCol==CAL_FIELD)
	{
		CRect rcUp(rc.left,rc.top,rc.left+rc.Width()/2,rc.bottom);
		CRect rcDown(rc.left+rc.Width()/2,rc.top,rc.right,rc.bottom);
		m_upButton.MoveWindow(rcUp);
		m_upButton.ShowWindow(SW_SHOW);
		m_upButton.BringWindowToTop();
		m_downButton.MoveWindow(rcDown);
		m_downButton.ShowWindow(SW_SHOW);
		m_downButton.BringWindowToTop();

		if(lRow==2)
		{
			if(product_register_value[MODBUS_ANALOG_IN1]==3||product_register_value[MODBUS_ANALOG_IN1]==5)
			{
					
				m_downButton.ShowWindow(SW_HIDE);
				m_upButton.ShowWindow(SW_HIDE);
			}
			else
			{
				m_downButton.ShowWindow(SW_SHOW);
				m_upButton.ShowWindow(SW_SHOW);
			}

		}
		if(lRow==3)
		{
			if(product_register_value[189]==3||product_register_value[189]==5)
			{
				m_downButton.ShowWindow(SW_HIDE);
				m_upButton.ShowWindow(SW_HIDE);
			}
			else
			{
				m_downButton.ShowWindow(SW_SHOW);
				m_upButton.ShowWindow(SW_SHOW);
			}

		}
		if(lRow==4)
		{
			m_downButton.ShowWindow(SW_HIDE);
			m_upButton.ShowWindow(SW_HIDE);
	
		}
	}
	if(lCol==AM_FIELD&&lRow!=1)
	{
		m_AmCombox.ShowWindow(SW_SHOW);//显示控件
		m_AmCombox.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_AmCombox.BringWindowToTop();
		m_AmCombox.SelectString(-1,strValue);
		m_AmCombox.SetFocus(); //获取焦点
	}
	if(FUN_FIELD==lCol&&lRow!=1)
	{
		if(lRow==2||lRow==3)
		{
			m_inputFinCombox.ResetContent();
			for(int i=0;i<8;i++)
				m_inputFinCombox.AddString(INPUT_FUNS[i]);
		}
		if(lRow==4)
		{
			m_inputFinCombox.ResetContent();
			for(int i=0;i<6;i++)
				m_inputFinCombox.AddString(INPUT_FUNS[i]);
		}

		
		m_inputFinCombox.ShowWindow(SW_SHOW);//显示控件
		m_inputFinCombox.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_inputFinCombox.BringWindowToTop();
		m_inputFinCombox.SelectString(-1,strValue);
		m_inputFinCombox.SetFocus(); //获取焦点
		
	}
	if(lRow==2&&CUST_FIELD==lCol&&product_register_value[MODBUS_ANALOG_IN1]==4)
	{
		m_customBtn.ShowWindow(SW_SHOW);
		m_customBtn.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_customBtn.BringWindowToTop();
		m_customBtn.SetFocus(); //获取焦点
	}
	if(lRow==3&&CUST_FIELD==lCol&&product_register_value[189]==4)
	{
		m_customBtn.ShowWindow(SW_SHOW);
		m_customBtn.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_customBtn.BringWindowToTop();
		m_customBtn.SetFocus(); //获取焦点
	}
	if(lCol==NAME_FIELD)
	{
		m_inputNameEdt.MoveWindow(&rc,1);
		m_inputNameEdt.ShowWindow(SW_SHOW);
		m_inputNameEdt.SetWindowText(strValue);
		m_inputNameEdt.SetFocus();
		int nLenth=strValue.GetLength();
		m_inputNameEdt.SetSel(nLenth,nLenth); //全选//

	}

}

void CInputSetDlg::OnCbnSelchangeRangCombo()
{
//	if( m_nModel == 16 || m_nModel == PM_TSTAT6 )
	if(m_nModel == PM_TSTAT6 || m_nModel == PM_TSTAT7|| m_nModel == PM_TSTAT8 || m_nModel == PM_TSTAT9 || m_nModel == PM_TSTAT5E|| m_nModel == PM_PM5E || m_nModel == PM_PM5E_ARM  ||m_nModel==PM_TSTATRUNAR|| m_nModel == PM_TSTAT5i
		|| (m_nModel == PM_TSTAT8_WIFI) || (m_nModel == PM_TSTAT8_OCC) || (m_nModel == PM_TSTAT7_ARM) || (m_nModel == PM_TSTAT8_220V))  //tstat6
	{
		OnCbnSelchangeRangComboFor5E();
	
		return;
	}
	m_downButton.ShowWindow(SW_HIDE);
	m_upButton.ShowWindow(SW_HIDE);

	if(m_nCurRow==1&&m_nCurCol==RANG_FIELD)
	{
		int nindext=m_RangCombox.GetCurSel();
		int ret=write_one(g_tstat_id,121,nindext);
		 if (ret>0)
		 {
		  product_register_value[121]=nindext;
		 }
		 else
		 {
		  AfxMessageBox(_T("Fail"));
		 }
	}

	if(m_nCurRow==2&&m_nCurCol==RANG_FIELD)
	{
		int nindext=m_RangCombox.GetCurSel();
		m_crange=nindext;
		if (nindext==6)
		{
		nindext=3;
		}
		if (nindext==7)
		{
		nindext=5;
		}
		 
		int ret=write_one(g_tstat_id,MODBUS_ANALOG_IN1,nindext);

		 if (ret>0)
		 {  
		 product_register_value[MODBUS_ANALOG_IN1]=nindext;


		 


		 if(product_register_value[MODBUS_ANALOG_IN1]==4)
		 {
			 m_FlexGrid.put_TextMatrix(2,CUST_FIELD,_T("Custom..."));
		 }
		 else
		 {
			 m_customBtn.ShowWindow(SW_HIDE);
			 m_FlexGrid.put_TextMatrix(2,CUST_FIELD,NO_APPLICATION);
		 }
		 } 
		 else
		 {
		   AfxMessageBox(_T("Try again"));
		 }
		
	}
	if(m_nCurRow==3&&m_nCurCol==RANG_FIELD)
	{
		int nindext=m_RangCombox.GetCurSel();
		m_crange=nindext;
		if (nindext==6)
		{
			m_crange=nindext;
			nindext=3;
		}
		if (nindext==7)
		{
			m_crange=7;
			nindext=5;
		}
		int ret=write_one(g_tstat_id,MODBUS_ANALOG_IN2,nindext);
		if (ret>0)
		{
			product_register_value[MODBUS_ANALOG_IN2]=nindext;

			CppSQLite3DB SqliteDBBuilding;
			CppSQLite3Table table;
			CppSQLite3Query q;
			SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

			CString sql;
			sql.Format(_T("Select * from Value_Range where CInputNo=%d and SN=%d"),m_nCurRow-1,m_sn);
			q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);

			if (!q.eof())//有表但是没有对应序列号的值
			{
				sql.Format(_T("update Value_Range set CRange = %d where CInputNo=%d and SN=%d "),m_crange,m_nCurRow-1,m_sn);
				SqliteDBBuilding.execDML((UTF8MBSTR)sql);
			}
			else
			{
				sql.Format(_T("Insert into Value_Range ( SN,CInputNo,CRange) values('%d','%d','%d')"),m_sn,m_nCurRow-1,m_crange);
				SqliteDBBuilding.execDML((UTF8MBSTR)sql);
			}
			SqliteDBBuilding.closedb();

		if(product_register_value[MODBUS_ANALOG_IN2]==4)
		{
			m_FlexGrid.put_TextMatrix(3,CUST_FIELD,_T("Custom..."));
		}
		else
		{
			m_customBtn.ShowWindow(SW_HIDE);
			m_FlexGrid.put_TextMatrix(3,CUST_FIELD,NO_APPLICATION);
		}
		} 
		else
		{
		AfxMessageBox(_T("Try again"));
		}
		
	}
	if(m_nCurRow==4&&m_nCurCol==RANG_FIELD)
	{
		int nindext=m_RangCombox.GetCurSel();
		m_crange=nindext;
		if (m_crange==0||m_crange==2)
		{
		nindext=0;
		} 
		else
		{
		nindext=1;
		}
		int ret=write_one(g_tstat_id,MODBUS_DIGITAL_IN1,nindext);
		if (ret>0)
		{
		product_register_value[MODBUS_DIGITAL_IN1]=nindext;


		CppSQLite3DB SqliteDBBuilding;
		CppSQLite3Table table;
		CppSQLite3Query q;
		SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);


		CString sql;
		sql.Format(_T("Select * from Value_Range where CInputNo=%d and SN=%d"),m_nCurRow-1,m_sn);
		q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);

		 
		if (!q.eof())//有表但是没有对应序列号的值
		{

			sql.Format(_T("update Value_Range set CRange = %d where CInputNo=%d and SN=%d "),m_crange,m_nCurRow-1,m_sn);
			SqliteDBBuilding.execDML((UTF8MBSTR)sql);
		}
		else
		{
			
			sql.Format(_T("Insert into Value_Range ( SN,CInputNo,CRange) values('%d','%d','%d')"),m_sn,m_nCurRow-1,m_crange);
		SqliteDBBuilding.execDML((UTF8MBSTR)sql);
		}
		 SqliteDBBuilding.closedb();
		}
		else
		{
		 AfxMessageBox(_T("Try again"));
		}

		


	}
	Fresh_GridForAll();
}

void CInputSetDlg::OnCbnKillfocusRangCombo()
{
	m_RangCombox.ShowWindow(SW_HIDE);
	 
}

void CInputSetDlg::OnBnClickedUpbutton()
{
	if (m_nModel == 16 || m_nModel == PM_TSTAT6||m_nModel == PM_TSTAT7|| m_nModel == PM_TSTAT5i || m_nModel == PM_TSTAT8 || m_nModel == PM_TSTAT9
		|| (m_nModel == PM_TSTAT8_WIFI) || (m_nModel == PM_TSTAT8_OCC) || (m_nModel == PM_TSTAT7_ARM) || (m_nModel == PM_TSTAT8_220V)) // for 5E
	{
		OnBnClickedUpbuttonFor5E();

		Fresh_GridForAll();
		return;
	}

	//the code below is for tstat5 ABCE use,this register address is special for others,so can't be changed. 
	//comments by Fance.
	if(m_nCurRow==1&&m_nCurCol==CAL_FIELD)
	{
		int nValue=product_register_value[101]+1;
		int ret=write_one(g_tstat_id,101,nValue);
		if (ret>0)
		{
		 product_register_value[101]=nValue;
		}
	}
	if(m_nCurRow==2&& m_nCurCol==CAL_FIELD)
	{
		int ret=write_one(g_tstat_id,180,product_register_value[180]+1);
		if (ret>0)
		{
		product_register_value[180]=product_register_value[180]+1;
		}
	}
	if(m_nCurRow==3&& m_nCurCol==CAL_FIELD)
	{
		int ret=write_one(g_tstat_id,181,product_register_value[181]+1);
		if (ret>0)
		{
		   product_register_value[181]=product_register_value[181]+1;
		}
	}
	Fresh_GridForAll();
}


//The button has hide ,unnecessary to change.
void CInputSetDlg::OnBnClickedDownbutton()
{
	if (m_nModel == 16 || m_nModel == PM_TSTAT6||m_nModel == PM_TSTAT7|| m_nModel == PM_TSTAT5i|| m_nModel == PM_TSTAT8 || m_nModel == PM_TSTAT9
		|| (m_nModel == PM_TSTAT8_WIFI) || (m_nModel == PM_TSTAT8_OCC) || (m_nModel == PM_TSTAT7_ARM) || (m_nModel == PM_TSTAT8_220V)) // for 5E
	{
		OnBnClickedDownbuttonFor5E();

		Fresh_GridForAll();
		return;
	}

	//the code below is for tstat5 ABCE use,this register address is special for others,so can't be changed. 
	//comments by Fance.
	if(m_nCurRow==1&&m_nCurCol==CAL_FIELD)
	{
		int nValue=product_register_value[101]-1;
		int ret=write_one(g_tstat_id,101,nValue);
		if (ret>0)
		{
		product_register_value[101]-=1;
		}
	}
	if(m_nCurRow==2&& m_nCurCol==CAL_FIELD)
	{
	    int nvalue=product_register_value[180]-1;
		int ret=write_one(g_tstat_id,180,product_register_value[180]-1);
		if (ret>0)
		{
		product_register_value[180]-=1;
		}
	}
	if(m_nCurRow==3&& m_nCurCol==CAL_FIELD)
	{
		 
		int nvalue=product_register_value[181]-1;
		int ret=write_one(g_tstat_id,181,product_register_value[181]-1);
		if (ret>0)
		{
			product_register_value[181]-=1;
		}
	}
	Fresh_GridForAll();
}

void CInputSetDlg::OnCbnKillfocusAmcombo()
{
	m_AmCombox.ShowWindow(SW_HIDE);
}

void CInputSetDlg::add_analog_rang(int i)
{

}


void CInputSetDlg::OnCbnSelchangeInputfuncombo()
{
	//	298	167	1	Low byte	W/R	Analog input1 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
	//	299	168	1	Low byte	W/R	Analog input2 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
	//	300	169	1	Low byte	W/R	(future)Analog input3 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
	//	301	170	1	Low byte	W/R	(future)Analog input4 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
	//	302	171	1	Low byte	W/R	(future)Analog input5 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
	//	303	172	1	Low byte	W/R	(future)Analog input6 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
	//	304	173	1	Low byte	W/R	(future)Analog input7 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
	//	305	174	1	Low byte	W/R	(future)Analog input8 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.


	int ret = 0;
	if (product_register_value[MODBUS_PRODUCT_MODEL] ==6)
	{
		if(m_nCurCol==FUN_FIELD)
		{
			int nchange=0;
			nchange=m_inputFinCombox.GetCurSel();
			ret=write_one(g_tstat_id,MODBUS_ANALOG1_FUNCTION+ m_nCurRow -2,nchange);//298 167 
			product_register_value[MODBUS_ANALOG1_FUNCTION+ m_nCurRow -2] = nchange;	//298 167
		}
	}
	else
	{
		if((m_nCurRow>=2)&&(m_nCurRow<=4))
		{
			int nchange=0;
			nchange=m_inputFinCombox.GetCurSel();
			ret=write_one(g_tstat_id,MODBUS_ANALOG1_FUNCTION+ m_nCurRow -2,nchange);//298 167 
			product_register_value[MODBUS_ANALOG1_FUNCTION+ m_nCurRow -2] = nchange;	//298 167
		}
	}
	if(ret<0)
	{
		AfxMessageBox(_T("Access Fail!Please try again!"));
	}
	else
	{
		AfxMessageBox(_T("Change success!"));
	}
	Fresh_GridForAll();

}

void CInputSetDlg::OnCbnKillfocusInputfuncombo()
{
	m_inputFinCombox.ShowWindow(SW_HIDE);
}

void CInputSetDlg::OnBnClickedCustombutton()
{
	if(m_nCurRow==2&&m_nCurCol==CUST_FIELD)
	{
		CBuildTable1 Dlg(2);
		Dlg.DoModal();
	}
	if(m_nCurRow==3&&m_nCurCol==CUST_FIELD)
	{
		CBuildTable1 Dlg(3);
		Dlg.DoModal();
	}
}

void CInputSetDlg::OnBnKillfocusCustombutton()
{
	//m_customBtn.ShowWindow(SW_HIDE);
}

void CInputSetDlg::OnBnClickedExit()
{
	OnCancel();
	//CDialog::OnOK();
}

void CInputSetDlg::OnEnKillfocusInvalueedit()
{
	if(m_nModel == 16 || m_nModel == PM_TSTAT6||m_nModel == PM_TSTAT7|| m_nModel == PM_TSTAT5i|| m_nModel == PM_TSTAT8 || m_nModel == PM_TSTAT9
		|| (m_nModel == PM_TSTAT8_WIFI) || (m_nModel == PM_TSTAT8_OCC) || (m_nModel == PM_TSTAT7_ARM) || (m_nModel == PM_TSTAT8_220V))
	{
		OnEnKillfocusInvalueeditFor5E();	//5E 以及更高的版本 不让改这一项，点击不会弹出 Edit框。;
		return;
	}
	int ret=0;

		m_inValueEdit.ShowWindow(SW_HIDE);
		CString strText;
		m_inValueEdit.GetWindowText(strText);
		if(strText.IsEmpty())
			return;
		int nValue=_wtoi(strText);
		if(m_nCurCol==VALUE_FIELD&&m_nCurRow==1)
		{
			ret=write_one(g_tstat_id,MODBUS_TEMPRATURE_CHIP,nValue*10);	//101
			if(ret<0)
			{
				AfxMessageBox(_T("Write Fail!Please try again!"));
				return;
			}
		}
		if(m_nCurCol==VALUE_FIELD&&m_nCurRow==2)
		{
			if(product_register_value[MODBUS_ANALOG_IN1]==4||product_register_value[MODBUS_ANALOG_IN1]==1)//188
			{
				int ret=write_one(g_tstat_id,MODBUS_EXTERNAL_SENSOR_0,nValue*10);//180
				if (ret>0)
				{
				product_register_value[MODBUS_EXTERNAL_SENSOR_0]=nValue*10;
			//	//AfxMessageBox(_T("Write Ok"));
				} 
				else
				{
				AfxMessageBox(_T("Write Error"));
				}
			}
			if (product_register_value[MODBUS_ANALOG_IN1]==0||product_register_value[MODBUS_ANALOG_IN1]==2)//188
			{
				int ret=write_one(g_tstat_id,MODBUS_EXTERNAL_SENSOR_0,nValue);	//180
				if (ret>0)
				{
					product_register_value[MODBUS_EXTERNAL_SENSOR_0]=nValue*10;
				//	//AfxMessageBox(_T("Write Ok"));
				} 
				else
				{
					AfxMessageBox(_T("Write Error"));
				}
			}//On/Off 通过combox选择。;
	
		}
		if(m_nCurCol==VALUE_FIELD&&m_nCurRow==3)
		{
			
			if(product_register_value[MODBUS_ANALOG_IN2]==4||product_register_value[MODBUS_ANALOG_IN2]==1)	//189
			{
				int ret=write_one(g_tstat_id,MODBUS_EXTERNAL_SENSOR_1,nValue*10);	//181
				if (ret>0)
				{
					product_register_value[MODBUS_EXTERNAL_SENSOR_1]=nValue*10;
					//AfxMessageBox(_T("Write Ok"));
				} 
				else
				{
					AfxMessageBox(_T("Write Error"));
				}
			}
			if (product_register_value[MODBUS_ANALOG_IN2]==0||product_register_value[MODBUS_ANALOG_IN2]==2)	//189
			{
			int ret=	write_one(g_tstat_id,MODBUS_EXTERNAL_SENSOR_1,nValue*10);	//181;
				if (ret>0)
				{
					product_register_value[MODBUS_EXTERNAL_SENSOR_1]=nValue*10;
					//AfxMessageBox(_T("Write Ok"));
				} 
				else
				{
					AfxMessageBox(_T("Write Error"));
				}
			}
		}
	Fresh_GridForAll();

}
//On/OFF Off=0,On=1;
//OFF/ON  Off=1, On=0;
void CInputSetDlg::OnCbnKillfocusValuecombo()
{
	if (m_nModel == 16 || m_nModel == PM_TSTAT6||m_nModel == PM_TSTAT7|| m_nModel == PM_TSTAT5i|| m_nModel == PM_TSTAT8 || m_nModel == PM_TSTAT9
		|| (m_nModel == PM_TSTAT8_WIFI) || (m_nModel == PM_TSTAT8_OCC) || (m_nModel == PM_TSTAT7_ARM) || (m_nModel == PM_TSTAT8_220V))
	{
		//OnCbnKillfocusValuecombo;//这里原来有错

// 		m_valueCombx.ShowWindow(SW_HIDE);
// 		int nItem=0;
// 		nItem=m_valueCombx.GetCurSel();
// 		if(m_nCurCol==VALUE_FIELD&&m_nCurRow==2)//找不到对应的tstat6
// 		{
// 			if(nItem==0)//off
// 				write_one(g_tstat_id,180,0);
// 			if(nItem==1)
// 				write_one(g_tstat_id,180,1);
// 		}
// 		if(m_nCurCol==VALUE_FIELD&&m_nCurRow==3)
// 		{
// 			if(nItem==0)//off
// 				write_one(g_tstat_id,181,0);
// 			if(nItem==1)
// 				write_one(g_tstat_id,181,1);
// 		}
// 		if(m_nCurCol==VALUE_FIELD&&m_nCurRow==4)
// 		{
// 			if (product_register_value[546]==0)
// 			{
// 				if(nItem==0)//off
// 					write_one(g_tstat_id,311,0);
// 				else
// 					write_one(g_tstat_id,311,1);
// 			}
// 			if (product_register_value[546]==1)
// 			{
// 
// 				if(nItem==0)
// 					write_one(g_tstat_id,311,1);//
// 				else
// 					write_one(g_tstat_id,311,0);//
// 			}
// 		}
// 		Fresh_Grid();
		return;
	}
	m_valueCombx.ShowWindow(SW_HIDE);
	
}

void CInputSetDlg::OnBnClickedRefreshbutton()
{

	GetDlgItem(IDC_REFRESHBUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDEXIT)->EnableWindow(FALSE);
	if(InputThread==NULL)
		InputThread = CreateThread(NULL,NULL,StartRefresh,this,NULL,NULL);

	/*

#if 1
	int i;
	register_critical_section.Lock();
	for(i=0;i<16;i++)
	{
		Read_Multi(g_tstat_id,&product_register_value[i*64],i*64,64);
	}
	register_critical_section.Unlock();
	memcpy_s(product_register_value,sizeof(product_register_value),product_register_value,sizeof(product_register_value));//
	if ((product_register_value[7] == 6)||(product_register_value[7] == 7))//tstat6
	{
			//product_register_value[]列表交换。
			memset(tempchange,0,sizeof(tempchange));
			int index = 0;

			for (int i = 0;i<512;i++)
			{
				index = reg_tstat6[i];
				tempchange[index] = product_register_value[i];
			}
			memcpy(product_register_value,tempchange,sizeof(product_register_value));
	}

#endif
	Fresh_Grid();
	*/
}

void CInputSetDlg::OnEnKillfocusInputnameedit()
{
	CString strText;
	m_inputNameEdt.GetWindowText(strText);
	m_inputNameEdt.ShowWindow(SW_HIDE);
	int lRow = m_FlexGrid.get_RowSel();//获取点击的行号	
	int lCol = m_FlexGrid.get_ColSel(); //获取点击的列号
	
	CString strInName;
	if(lCol!=NAME_FIELD||lRow==0)
		return;
	switch (lRow)
	{
	case 1:
		strInName=g_strSensorName;
		break;
	case 2:
		strInName=g_strInName1;
		break;
	case 3:
		strInName=g_strInName2;
		break;
	case 4:
		strInName=g_strInName3;
		break;
	case 5:
		strInName=g_strInName4;
		break;
	case 6:
		strInName=g_strInName5;
		break;
	case 7:
		strInName=g_strInName6;
		break;
	case 8:
		strInName=g_strInName7;
		break;
	case 9:
		strInName=g_strInName8;
		break;
	}

	if(strText.CompareNoCase(strInName)==0)
		return;
	int nFlag = product_register_value[7];
	if ((product_register_value[7]==PM_TSTAT5G)||(product_register_value[7]==PM_TSTAT5E)||(product_register_value[7]==PM_PM5E) || 
        (product_register_value[7] == PM_PM5E_ARM) ||(product_register_value[7]==PM_TSTATRUNAR)||(product_register_value[7]==PM_TSTAT6)||
        (product_register_value[7]==PM_TSTAT5i||(product_register_value[7]==PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9)
		|| (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V)
		||(product_register_value[7]==PM_TSTAT7)))
	{
		strText.TrimRight();
		unsigned char p[8];//input+input1

		for(int i=0;i<8;i++)
		{
			if(i<strText.GetLength())
				p[i]=strText.GetAt(i);
			else
				p[i]=0;
		}
		 
			 Write_Multi(g_tstat_id,p,MODBUS_AI1_CHAR1+4*(lRow-2),8);
			 
			 

		 
	}

	try
	{

		//if(g_serialNum>0&&product_register_value[6]>0)
		if(product_register_value[6]>0)
		{

			CppSQLite3DB SqliteDBBuilding;
			CppSQLite3Table table;
			CppSQLite3Query q;
			SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
;
			CString strSerial;
			strSerial.Format(_T("%d"),g_serialNum);

			CString strsql;
			strsql.Format(_T("select * from IONAME where SERIAL_ID = '%s'"),strSerial);
			//m_RsTmp->Open((_variant_t)strsql,_variant_t((IDispatch *)m_ConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
			q = SqliteDBBuilding.execQuery((UTF8MBSTR)strsql);


			if(!q.eof())//update
			{			
				CString strField;
				switch (lRow)
				{

				case 2:
					strField=_T("INPUT1");
					break;
				case 3:
					strField=_T("INPUT2");
					break;
				case 4:
					strField=_T("INPUT3");
					break;
				case 5:
					strField=_T("INPUT4");
					break;
				case 6:
					strField=_T("INPUT5");
					break;
				case 7:
					strField=_T("INPUT6");
					break;
				case 8:
					strField=_T("INPUT7");
					break;
				case 9:
					strField=_T("INPUT8");
					break;
				}

				try
				{

					CString str_temp;
					str_temp.Format(_T("update IONAME set "+strField+" = '"+strText+"' where SERIAL_ID = '"+strSerial+"'"));
					//AfxMessageBox(str_temp );
					SqliteDBBuilding.execDML((UTF8MBSTR)str_temp);
					m_FlexGrid.put_TextMatrix(lRow,lCol,strText);
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}

			}
			else//inerst
			{
				switch (lRow)
				{
				case 2:
					g_strInName1=strText;
					break;
				case 3:
					g_strInName2=strText;
					break;
				case 4:
					g_strInName3=strText;
					break;
				case 5:
					g_strInName4=strText;
					break;
				case 6:
					g_strInName5=strText;
					break;
				case 7:
					g_strInName6=strText;
					break;
				case 8:
					g_strInName7=strText;
					break;
				case 9:
					g_strInName8=strText;
					break;
				}

				CString g_strInName9;
				//g_strInName9 = _T("input9");Humidity Sensor
				g_strInName9 = _T("Humidity Sensor");
				CString	str_temp;
				str_temp.Format(_T("insert into IONAME values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')"),
					strSerial,
					g_strInName1,
					g_strInName2,
					g_strInName3,
					g_strInName4,
					g_strInName5,
					g_strInName6,
					g_strInName7,
					g_strOutName1,
					g_strOutName2,
					g_strOutName3,
					g_strOutName4,
					g_strOutName5,
					g_strOutName6,
					g_strOutName7,
					g_strInName8,
					g_strInHumName,
					g_strSensorName
					);
				try
				{

					SqliteDBBuilding.execDML((UTF8MBSTR)str_temp);
					m_FlexGrid.put_TextMatrix(lRow,lCol,strText);
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}
			}

			switch (lRow)
			{
			case 2:
				g_strInName1=strText;
				break;
			case 3:
				g_strInName2=strText;
				break;
			case 4:
				g_strInName3=strText;
				break;
			case 5:
				g_strInName4=strText;
				break;
			case 6:
				g_strInName5=strText;
				break;
			case 7:
				g_strInName6=strText;
				break;
			case 8:
				g_strInName7=strText;
				break;
			case 9:
				g_strInName8=strText;
				break;
			}
			SqliteDBBuilding.closedb();
// 			if(m_RsTmp->State) 
// 				m_RsTmp->Close(); 
// 			if(m_ConTmp->State)
// 				m_ConTmp->Close();	
		}	

	}
	catch (...)
	{


	}
	
}


// #define INDEX_FIELD 0
// #define NAME_FIELD 1
// #define VALUE_FIELD 2
// #define AM_FIELD 3
// #define CAL_FIELD 4
// #define RANG_FIELD 5
// #define FUN_FIELD 6
// #define CUST_FIELD 7
// 
// #define TOTAL_COLS 8
// #define TOTAL_ROWS 7
 
void CInputSetDlg::InitGridtstat6()
   {
 	   if((product_register_value[20]&2)==2)
 	   {
 	       m_disable_hum=TRUE;
 	   }
 	   else
 	   {
 		   m_disable_hum=FALSE;
 	   }
	   if((product_register_value[MODBUS_TSTAT6_CO2_AVALUE]>=0)&&(product_register_value[MODBUS_TSTAT6_CO2_AVALUE]<=3000))
	   {
		   m_disable_CO2=TRUE;
	   }
	   else
	   {
		   m_disable_CO2=FALSE;
	   }
	  

	   m_FlexGrid.put_TextMatrix(1,NAME_FIELD,g_strSensorName);
	   m_FlexGrid.put_TextMatrix(2,NAME_FIELD,g_strInName1);
	   m_FlexGrid.put_TextMatrix(3,NAME_FIELD,g_strInName2);
	   m_FlexGrid.put_TextMatrix(4,NAME_FIELD,g_strInName3);
	   m_FlexGrid.put_TextMatrix(5,NAME_FIELD,g_strInName4);
	   m_FlexGrid.put_TextMatrix(6,NAME_FIELD,g_strInName5);
	   m_FlexGrid.put_TextMatrix(7,NAME_FIELD,g_strInName6);
	   m_FlexGrid.put_TextMatrix(8,NAME_FIELD,g_strInName7);
	   m_FlexGrid.put_TextMatrix(9,NAME_FIELD,g_strInName8); 
	  m_FlexGrid.put_TextMatrix(10,NAME_FIELD,g_strInHumName);
	  m_FlexGrid.put_TextMatrix(11,NAME_FIELD,g_strInCO2);
	    
	   
	   CString strUnit=GetTempUnit();
	   CString strTemp;	
	   int nValue=0;
	   //float nValue=0;
	   CString strAuto=_T("Auto");
	   CString strman=_T("Manual");
	 
	   m_fFirmwareVersion = get_curtstat_version();

	   for(int i=1;i<=9;i++)
	   {	
		   CString strIndex;
		   strIndex.Format(_T("%d"), i-1);
		   m_FlexGrid.put_TextMatrix(i,INDEX_FIELD,strIndex);

		   for(int k=0;k<=8;k++)
		   {
			   m_FlexGrid.put_ColAlignment(k,4);
			   if (i%2==1)
			   {
				   m_FlexGrid.put_Row(i);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(RGB(255,255,255));
			   }
			   else
			   {
				   m_FlexGrid.put_Row(i);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(COLOR_CELL);
			   }
		   }

		  
		   if(m_fFirmwareVersion>33.3)
		   {
			   float fValue;

			   // Row1
			   if(i==1)
			   {
 
					   strTemp.Format(_T("%.1f"),product_register_value[MODBUS_INTERNAL_THERMISTOR]/10.0);//216

 


				   strTemp=strTemp+strUnit;
				   m_FlexGrid.put_TextMatrix(1,VALUE_FIELD,strTemp);
				   if (product_register_value[695]!=0)
				   {
				   m_FlexGrid.put_TextMatrix(1,AM_FIELD,_T("Manual"));
				   } 
				   else
				   {
				   m_FlexGrid.put_TextMatrix(1,AM_FIELD,_T("Auto"));
				   }
				   
				   m_FlexGrid.put_TextMatrix(1,CAL_FIELD,_T("Adjust..."));
				   m_FlexGrid.put_TextMatrix(1,RANG_FIELD,strUnit);
				   m_FlexGrid.put_TextMatrix(1,FUN_FIELD,NO_APPLICATION);
				   m_FlexGrid.put_TextMatrix(1,CUST_FIELD,NO_APPLICATION);

				   m_FlexGrid.put_Col(FILTER);
				   m_FlexGrid.put_Row(i);
				   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);

				   continue;
			   }



			   //////////////////////////////////////////////////////////////////////////
			   // column 1  Value


			   // 
			   // 						359	122	1	Low byte	W/R	ANALOG INPUT1 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			   // 						2	123	1	Low byte	W/R	ANALOG INPUT2 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			   // 						3	124	1	Low byte	W/R	ANALOG INPUT3 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			   // 						4	125	1	Low byte	W/R	ANALOG INPUT4 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			   // 						5	126	1	Low byte	W/R	ANALOG INPUT5 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			   // 						6	127	1	Low byte	W/R	ANALOG INPUT6 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			   // 						7	128	1	Low byte	W/R	ANALOG INPUT7 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			   // 						8	129	1	Low byte	W/R	ANALOG INPUT8 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON


			   // 
			   // 						367	131	2	Full	W/R	Analog input1 value
			   // 						10	132	2	Full	W/R	Analog input2 value
			   // 						11	133	2	Full	W/R	Analog input3 value
			   // 						12	134	2	Full	W/R	Analog input4 value
			   // 						13	135	2	Full	W/R	Analog input5 value
			   // 						14	136	2	Full	W/R	Analog input6 value
			   // 						15	137	2	Full	W/R	Analog input7 value
			   // 						16	138	2	Full	W/R	Analog input8 value




			   CString strValueUnit=GetTempUnit(product_register_value[MODBUS_ANALOG1_RANGE+i-2], 1); //5e=359   122


			   //309	141	2	Full	W/R	Input auto/ manual enable.
			   // column 2  Auto/Manual // Ö»ÓÐIN1£¬2²ÅÓÐ,internal sensor Ã»ÓÐ£¬´óÓÚ1 // 
			   if(i>1)//( i== 2 || i == 3)
			   {
				   nValue=product_register_value[MODBUS_INPUT_MANU_ENABLE];//309    141
				   BYTE bFilter=0x01;
				   bFilter = bFilter<< (i-2);
				   if((nValue & bFilter))
				   {
					   m_FlexGrid.put_TextMatrix(i,AM_FIELD,strman);			
				   }
				   else
				   {
					   m_FlexGrid.put_TextMatrix(i,AM_FIELD,strAuto);				
				   }
			   }
			   else
			   {
				   m_FlexGrid.put_Col(AM_FIELD);
				   m_FlexGrid.put_Row(i);
				   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
			   }

			   // column 3  Calibrate			
			    
				   m_FlexGrid.put_TextMatrix(i,CAL_FIELD,_T("Adjust..."));				
			    
			 
				strTemp.Format(_T("%d"),product_register_value[141+i]);
			    m_FlexGrid.put_TextMatrix(i,FILTER,strTemp);		
			   // column 4  Range 
			   //if( i >= 2 )

				m_crange=0;
			
					nValue=product_register_value[MODBUS_ANALOG1_RANGE+i-2];	//189
					if(nValue>=0)
					{
							strTemp=analog_range_TSTAT6[nValue]; 
					}
					m_crange=nValue;
				
			   m_FlexGrid.put_TextMatrix(i,RANG_FIELD,strTemp);
			   


			{
				if(product_register_value[MODBUS_ANALOG1_RANGE+i-2]==1)	//359  122
				{				
					fValue=float(product_register_value[MODBUS_ANALOG_INPUT1+i-2]/10.0);	//367   131
					strTemp.Format(_T("%.1f"),fValue);	

					strTemp +=strValueUnit;
				}
				else if(product_register_value[MODBUS_ANALOG1_RANGE+i-2]==3 || product_register_value[MODBUS_ANALOG1_RANGE+i-2]==5) // On/Off or Off/On ==1 On ==0 Off   359  122
				{						
					if (m_crange==9||m_crange==10)
					{
						int nValue=(product_register_value[MODBUS_ANALOG_INPUT1+i-2]); //367  131
						if (nValue == 0)
						{
							strTemp = _T("Closed");
						}
						else
						{
							strTemp = _T("Open");
						}	
					}
					else if (m_crange==7||m_crange==8)
					{
						 
							int nValue=(product_register_value[MODBUS_ANALOG_INPUT1+i-2]); //367  131
							if (nValue == 0)
							{
								strTemp = _T("Unoccupied");
							}
							else
							{
								strTemp = _T("Occupied");
							}	
					 
					}
					else
					{
						 
							int nValue=(product_register_value[MODBUS_ANALOG_INPUT1+i-2]); //367  131
							if (nValue == 0)
							{
								strTemp = _T("Off");
							}
							else
							{
								strTemp = _T("On");
							}	
						 
					}					
				}
				else if (product_register_value[MODBUS_ANALOG1_RANGE+i-2]==4 )  // custom sensor	359 122
				{					
					fValue=float(product_register_value[MODBUS_ANALOG_INPUT1+i-2]/10.0);	//367  131
					strTemp.Format(_T("%.1f"), (float)fValue/10.0);	
					strTemp +=strValueUnit;
				}
				else if(product_register_value[MODBUS_ANALOG1_RANGE+i-2]==2)	//359 122
				{
					nValue=product_register_value[MODBUS_ANALOG_INPUT1+i-2];		//367  131
					strTemp.Format(_T("%0.1f%%"),  (float)nValue);
				}
				else
				{
					strTemp.Format(_T("%d"),product_register_value[MODBUS_ANALOG_INPUT1+i-2]);//lsc
				}						
				m_FlexGrid.put_TextMatrix(i,VALUE_FIELD,strTemp);
				strTemp.Empty();
			}



		 


			   nValue=product_register_value[MODBUS_ANALOG1_FUNCTION+i-2];		//298   167
			   if(product_register_value[MODBUS_PRODUCT_MODEL] ==  6)
			   {
				   if (nValue>=0&&nValue<8)//tstat6
				   {		
					   strTemp=INPUT_FUNS[nValue];
					   m_FlexGrid.put_TextMatrix(i,FUN_FIELD,strTemp);
				   }
			   }
			   else
			   {
				   if(i== 2 || i==3)
				   {
					   nValue=product_register_value[MODBUS_ANALOG1_FUNCTION+i-2];
					   if (nValue>=0&&nValue<=7)
					   {
					    strTemp=INPUT_FUNS[nValue];
					   } 
					   else
					   {
					    strTemp=_T("Default");
					   }
					  
					   m_FlexGrid.put_TextMatrix(i,FUN_FIELD,strTemp);
				   }
				   else
				   {
					   m_FlexGrid.put_Col(FUN_FIELD);
					   m_FlexGrid.put_Row(i);
					   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
				   }
			   }
			   if(product_register_value[MODBUS_PRODUCT_MODEL] ==  6)
			   {
				   m_customBtn.ShowWindow(SW_HIDE);
				   m_FlexGrid.put_TextMatrix(i,CUST_FIELD,NO_APPLICATION);
			   }
			   else
			   {
				   if( i== 2 || i==3)
				   {
					   if(product_register_value[MODBUS_ANALOG_IN1]==4)	//188
					   {
						   m_FlexGrid.put_TextMatrix(i,CUST_FIELD,_T("Custom..."));
					   }
					   else
					   {
						   m_customBtn.ShowWindow(SW_HIDE);
						   m_FlexGrid.put_TextMatrix(i,CUST_FIELD,NO_APPLICATION);
					   }
				   }
				   else
				   {
					   m_FlexGrid.put_Col(CUST_FIELD);
					   m_FlexGrid.put_Row(i);
					   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);	
					   			
				   }
			   }
				
			 
			  

		   }
		   else  // another lower version of software	,The version too low, can't be changed .  Fance.	
		   {
			   int nValue;

			   if(product_register_value[341+i-2]==1)	//341
			   {
				   nValue=(int)(product_register_value[349+i-2]/10.0);
				   strTemp.Format(_T("%.1f"),nValue);
			   }
			   else
			   {
				   strTemp.Format(_T("%d"),product_register_value[349+i-2]);
			   }
			   m_FlexGrid.put_TextMatrix(i,VALUE_FIELD,strTemp);

			   strTemp.Empty();
			   int nItem=product_register_value[341+i-2];
			   if(nItem>=0&&nItem<=4)
			   {
				   strTemp=analog_range[nItem];					
			   }
			   m_FlexGrid.put_TextMatrix(i,AM_FIELD,strTemp);
		   }

	   }
	   if (!m_disable_hum)
	   {
		   for (int col=0;col<TOTAL_COLS;col++)
		   {
			   m_FlexGrid.put_Col(col);
			   m_FlexGrid.put_Row(10);
			   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		   }

	   }
	   else
	   {

		   CString temp;
		   if (product_register_value[MODBUS_TSTAT6_HUM_AM]==0)
		   {

			   temp.Format(_T("%0.1f%%"),(float)product_register_value[MODBUS_TSTAT6_HUM_AVALUE]/10.0);
			   m_FlexGrid.put_TextMatrix(10,AM_FIELD,strAuto);
		   }
		   else
		   {
			   m_FlexGrid.put_TextMatrix(10,AM_FIELD,strman);
			   temp.Format(_T("%0.1f%%"),(float)product_register_value[MODBUS_TSTAT6_HUM_MVALUE]/10);

		   }
		   m_FlexGrid.put_TextMatrix(10,VALUE_FIELD,temp);
		   m_FlexGrid.put_TextMatrix(10,CAL_FIELD,_T("Adjust..."));
		   temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_HUM_FILTER]);
		   m_FlexGrid.put_TextMatrix(10,FILTER,temp);
	   }
	 /*  m_FlexGrid.put_Col(FILTER);
				   m_FlexGrid.put_Row(i);
				   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);*/
	   if (!m_disable_CO2)
	   {
		   for (int col=0;col<TOTAL_COLS;col++)
		   {
			   m_FlexGrid.put_Col(col);
			   m_FlexGrid.put_Row(11);
			   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		   }
	   } 
	   else
	   {
		   CString temp;
		   if (product_register_value[MODBUS_TSTAT6_CO2_AM]==0)
		   {     strUnit=_T("ppm");
		   m_FlexGrid.put_TextMatrix(11,AM_FIELD,strAuto);
		   temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_AVALUE]);
		   temp+=strUnit;
		   m_FlexGrid.put_TextMatrix(11,VALUE_FIELD,temp);
		   }
		   else
		   {
			   m_FlexGrid.put_TextMatrix(11,AM_FIELD,strman);
			   temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_MVALUE]);
			   temp+=strUnit;
			   m_FlexGrid.put_TextMatrix(11,VALUE_FIELD,temp);
		   }	
		   m_FlexGrid.put_TextMatrix(11,CAL_FIELD,_T("Adjust..."));
		   temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_FILTER]);
		   m_FlexGrid.put_TextMatrix(11,FILTER,temp);
	   }
	   if (product_register_value[7] == PM_TSTAT7)
	   {
		   for (int row=6;row<=11;row++)
		   {
			   //   m_Input_Grid.put_TextMatrix(row,0,_T("UNUSED"));
			   m_FlexGrid.put_TextMatrix(row,1,_T("UNUSED"));
			   m_FlexGrid.put_TextMatrix(row,2,_T("UNUSED"));
			   m_FlexGrid.put_TextMatrix(row,3,_T("UNUSED"));
			   m_FlexGrid.put_TextMatrix(row,4,_T("UNUSED"));
			   m_FlexGrid.put_TextMatrix(row,5,_T("UNUSED"));
			   m_FlexGrid.put_TextMatrix(row,6,_T("UNUSED"));
			   m_FlexGrid.put_TextMatrix(row,7,_T("UNUSED"));
			   m_FlexGrid.put_TextMatrix(row,8,_T("UNUSED"));
		   }

	   }

   }
void CInputSetDlg::Fresh_GridForTstat6(){
	if((product_register_value[20]&2)==2)
	{
		m_disable_hum=TRUE;
	}
	else
	{
		m_disable_hum=FALSE;
	}
	if((product_register_value[MODBUS_TSTAT6_CO2_AVALUE]>=0)&&(product_register_value[MODBUS_TSTAT6_CO2_AVALUE]<=3000))
	{
		m_disable_CO2=TRUE;
	}
	else
	{
		m_disable_CO2=FALSE;
	}

	   CString strUnit=GetTempUnit();
	   CString strTemp;	
	   int nValue=0;
	   //float nValue=0;
	   CString strAuto=_T("Auto");
	   CString strman=_T("Manual");
	 
	   m_fFirmwareVersion = get_curtstat_version();

	   for(int i=1;i<=9;i++)
	   {	
		   CString strIndex;
		   strIndex.Format(_T("%d"), i-1);
		   m_FlexGrid.put_TextMatrix(i,INDEX_FIELD,strIndex);
		   if(m_fFirmwareVersion>33.3)
		   {
			   float fValue;

			   // Row1
			   if(i==1)
			   {
 
					   strTemp.Format(_T("%.1f"),product_register_value[MODBUS_INTERNAL_THERMISTOR]/10.0);//216

 


				   strTemp=strTemp+strUnit;
				   m_FlexGrid.put_TextMatrix(1,VALUE_FIELD,strTemp);
				   if (product_register_value[695]!=0)
				   {
				   m_FlexGrid.put_TextMatrix(1,AM_FIELD,_T("Manual"));
				   } 
				   else
				   {
				   m_FlexGrid.put_TextMatrix(1,AM_FIELD,_T("Auto"));
				   }
				   
				   m_FlexGrid.put_TextMatrix(1,CAL_FIELD,_T("Adjust..."));
				   m_FlexGrid.put_TextMatrix(1,RANG_FIELD,strUnit);
				   m_FlexGrid.put_TextMatrix(1,FUN_FIELD,NO_APPLICATION);
				   m_FlexGrid.put_TextMatrix(1,CUST_FIELD,NO_APPLICATION);

				   m_FlexGrid.put_Col(FILTER);
				   m_FlexGrid.put_Row(i);
				   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);

				   continue;
			   }
			   CString strValueUnit=GetTempUnit(product_register_value[MODBUS_ANALOG1_RANGE+i-2], 1); //5e=359   122


			   //309	141	2	Full	W/R	Input auto/ manual enable.
			   // column 2  Auto/Manual // Ö»ÓÐIN1£¬2²ÅÓÐ,internal sensor Ã»ÓÐ£¬´óÓÚ1 // 
			   if(i>1)//( i== 2 || i == 3)
			   {
				   nValue=product_register_value[MODBUS_INPUT_MANU_ENABLE];//309    141
				   BYTE bFilter=0x01;
				   bFilter = bFilter<< (i-2);
				   if((nValue & bFilter))
				   {
					   m_FlexGrid.put_TextMatrix(i,AM_FIELD,strman);			
				   }
				   else
				   {
					   m_FlexGrid.put_TextMatrix(i,AM_FIELD,strAuto);				
				   }
			   }
			   else
			   {
				   m_FlexGrid.put_Col(AM_FIELD);
				   m_FlexGrid.put_Row(i);
				   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
			   }

			   // column 3  Calibrate			
			    
				   m_FlexGrid.put_TextMatrix(i,CAL_FIELD,_T("Adjust..."));				
			    
			 
				strTemp.Format(_T("%d"),product_register_value[141+i]);
			    m_FlexGrid.put_TextMatrix(i,FILTER,strTemp);		
			   // column 4  Range 
			   //if( i >= 2 )

				m_crange=0;
				 
				
					nValue=product_register_value[MODBUS_ANALOG1_RANGE+i-2];	//189
					if(nValue>=0)
					{
							strTemp=analog_range[nValue]; 
					}
					m_crange=nValue;
			     m_FlexGrid.put_TextMatrix(i,RANG_FIELD,strTemp);
			   


			{
				if(product_register_value[MODBUS_ANALOG1_RANGE+i-2]==1)	//359  122
				{				
					fValue=float(product_register_value[MODBUS_ANALOG_INPUT1+i-2]/10.0);	//367   131
					strTemp.Format(_T("%.1f"),fValue);	

					strTemp +=strValueUnit;
				}
				else if(product_register_value[MODBUS_ANALOG1_RANGE+i-2]==3 || product_register_value[MODBUS_ANALOG1_RANGE+i-2]==5) // On/Off or Off/On ==1 On ==0 Off   359  122
				{						
					if (m_crange==9||m_crange==10)
					{
						int nValue=(product_register_value[MODBUS_ANALOG_INPUT1+i-2]); //367  131
						if (nValue == 0)
						{
							strTemp = _T("Closed");
						}
						else
						{
							strTemp = _T("Open");
						}	
					}
					else if (m_crange==7||m_crange==8)
					{
						 
							int nValue=(product_register_value[MODBUS_ANALOG_INPUT1+i-2]); //367  131
							if (nValue == 0)
							{
								strTemp = _T("Unoccupied");
							}
							else
							{
								strTemp = _T("Occupied");
							}	
					 
					}
					else
					{
						 
							int nValue=(product_register_value[MODBUS_ANALOG_INPUT1+i-2]); //367  131
							if (nValue == 0)
							{
								strTemp = _T("Off");
							}
							else
							{
								strTemp = _T("On");
							}	
						 
					}					
				}
				else if (product_register_value[MODBUS_ANALOG1_RANGE+i-2]==4 )  // custom sensor	359 122
				{					
					fValue=float(product_register_value[MODBUS_ANALOG_INPUT1+i-2]/10.0);	//367  131
					strTemp.Format(_T("%.1f"), (float)fValue/10.0);	
					strTemp +=strValueUnit;
				}
				else if(product_register_value[MODBUS_ANALOG1_RANGE+i-2]==2)	//359 122
				{
					nValue=product_register_value[MODBUS_ANALOG_INPUT1+i-2];		//367  131
					strTemp.Format(_T("%0.1f%%"),  (float)nValue);
				}
				else
				{
					strTemp.Format(_T("%d"),product_register_value[MODBUS_ANALOG_INPUT1+i-2]);//lsc
				}						
				m_FlexGrid.put_TextMatrix(i,VALUE_FIELD,strTemp);
				strTemp.Empty();
			}



		 


			   nValue=product_register_value[MODBUS_ANALOG1_FUNCTION+i-2];		//298   167
			   if(product_register_value[MODBUS_PRODUCT_MODEL] ==  6)
			   {
				   if (nValue>=0&&nValue<8)//tstat6
				   {		
					   strTemp=INPUT_FUNS[nValue];
					   m_FlexGrid.put_TextMatrix(i,FUN_FIELD,strTemp);
				   }
			   }
			   else
			   {
				   if(i== 2 || i==3)
				   {
					   nValue=product_register_value[MODBUS_ANALOG1_FUNCTION+i-2];
					   if (nValue>=0&&nValue<=7)
					   {
					    strTemp=INPUT_FUNS[nValue];
					   } 
					   else
					   {
					    strTemp=_T("Default");
					   }
					  
					   m_FlexGrid.put_TextMatrix(i,FUN_FIELD,strTemp);
				   }
				   else
				   {
					   m_FlexGrid.put_Col(FUN_FIELD);
					   m_FlexGrid.put_Row(i);
					   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
				   }
			   }
			   if(product_register_value[MODBUS_PRODUCT_MODEL] ==  6)
			   {
				   m_customBtn.ShowWindow(SW_HIDE);
				   m_FlexGrid.put_TextMatrix(i,CUST_FIELD,NO_APPLICATION);
			   }
			   else
			   {
				   if( i== 2 || i==3)
				   {
					   if(product_register_value[MODBUS_ANALOG_IN1]==4)	//188
					   {
						   m_FlexGrid.put_TextMatrix(i,CUST_FIELD,_T("Custom..."));
					   }
					   else
					   {
						   m_customBtn.ShowWindow(SW_HIDE);
						   m_FlexGrid.put_TextMatrix(i,CUST_FIELD,NO_APPLICATION);
					   }
				   }
				   else
				   {
					   m_FlexGrid.put_Col(CUST_FIELD);
					   m_FlexGrid.put_Row(i);
					   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);	
					   			
				   }
			   }
				
			 
			  

		   }
		   else  // another lower version of software	,The version too low, can't be changed .  Fance.	
		   {
			   int nValue;

			   if(product_register_value[341+i-2]==1)	//341
			   {
				   nValue=(int)(product_register_value[349+i-2]/10.0);
				   strTemp.Format(_T("%.1f"),nValue);
			   }
			   else
			   {
				   strTemp.Format(_T("%d"),product_register_value[349+i-2]);
			   }
			   m_FlexGrid.put_TextMatrix(i,VALUE_FIELD,strTemp);

			   strTemp.Empty();
			   int nItem=product_register_value[341+i-2];
			   if(nItem>=0&&nItem<=4)
			   {
				   strTemp=analog_range[nItem];					
			   }
			   m_FlexGrid.put_TextMatrix(i,RANG_FIELD,strTemp);
		   }

	   }
	   if (!m_disable_hum)
	   {
		   for (int col=0;col<TOTAL_COLS;col++)
		   {
			   m_FlexGrid.put_Col(col);
			   m_FlexGrid.put_Row(10);
			   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		   }

	   }
	   else
	   {

		   CString temp;
		   if (product_register_value[MODBUS_TSTAT6_HUM_AM]==0)
		   {

			   temp.Format(_T("%0.1f%%"),(float)product_register_value[MODBUS_TSTAT6_HUM_AVALUE]/10.0);
			   m_FlexGrid.put_TextMatrix(10,AM_FIELD,strAuto);
		   }
		   else
		   {
			   m_FlexGrid.put_TextMatrix(10,AM_FIELD,strman);
			   temp.Format(_T("%0.1f%%"),(float)product_register_value[MODBUS_TSTAT6_HUM_MVALUE]/10);

		   }
		   m_FlexGrid.put_TextMatrix(10,VALUE_FIELD,temp);
		   m_FlexGrid.put_TextMatrix(10,CAL_FIELD,_T("Adjust..."));
		   temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_HUM_FILTER]);
		   m_FlexGrid.put_TextMatrix(10,FILTER,temp);
	   }
	 /*  m_FlexGrid.put_Col(FILTER);
				   m_FlexGrid.put_Row(i);
				   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);*/
	   if (!m_disable_CO2)
	   {
		   for (int col=0;col<TOTAL_COLS;col++)
		   {
			   m_FlexGrid.put_Col(col);
			   m_FlexGrid.put_Row(11);
			   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		   }
	   } 
	   else
	   {
		   CString temp;
		   if (product_register_value[MODBUS_TSTAT6_CO2_AM]==0)
		   {     strUnit=_T("ppm");
		   m_FlexGrid.put_TextMatrix(11,AM_FIELD,strAuto);
		   temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_AVALUE]);
		   temp+=strUnit;
		   m_FlexGrid.put_TextMatrix(11,VALUE_FIELD,temp);
		   }
		   else
		   {
			   m_FlexGrid.put_TextMatrix(11,AM_FIELD,strman);
			   temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_MVALUE]);
			   temp+=strUnit;
			   m_FlexGrid.put_TextMatrix(11,VALUE_FIELD,temp);
		   }	
		   m_FlexGrid.put_TextMatrix(11,CAL_FIELD,_T("Adjust..."));
		   temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_FILTER]);
		   m_FlexGrid.put_TextMatrix(11,FILTER,temp);
	   }
	   if (product_register_value[7] == PM_TSTAT7)
	   {
		   for (int row=6;row<=11;row++)
		   {
			   //   m_Input_Grid.put_TextMatrix(row,0,_T("UNUSED"));
			   m_FlexGrid.put_TextMatrix(row,1,_T("UNUSED"));
			   m_FlexGrid.put_TextMatrix(row,2,_T("UNUSED"));
			   m_FlexGrid.put_TextMatrix(row,3,_T("UNUSED"));
			   m_FlexGrid.put_TextMatrix(row,4,_T("UNUSED"));
			   m_FlexGrid.put_TextMatrix(row,5,_T("UNUSED"));
			   m_FlexGrid.put_TextMatrix(row,6,_T("UNUSED"));
			   m_FlexGrid.put_TextMatrix(row,7,_T("UNUSED"));
			   m_FlexGrid.put_TextMatrix(row,8,_T("UNUSED"));
		   }

	   }

      SetCurrentCell();

}
void CInputSetDlg::Init_not_5ABCD_Grid()
{
	m_FlexGrid.put_TextMatrix(1,NAME_FIELD,g_strSensorName);
	m_FlexGrid.put_TextMatrix(2,NAME_FIELD,g_strInName1);
	m_FlexGrid.put_TextMatrix(3,NAME_FIELD,g_strInName2);
	m_FlexGrid.put_TextMatrix(4,NAME_FIELD,g_strInName3);
	m_FlexGrid.put_TextMatrix(5,NAME_FIELD,g_strInName4);
	m_FlexGrid.put_TextMatrix(6,NAME_FIELD,g_strInName5);
	m_FlexGrid.put_TextMatrix(7,NAME_FIELD,g_strInName6);
	m_FlexGrid.put_TextMatrix(8,NAME_FIELD,g_strInName7);
	m_FlexGrid.put_TextMatrix(9,NAME_FIELD,g_strInName8);

	CString strUnit=GetTempUnit();
	CString strTemp;	
	int nValue=0;
	//float nValue=0;
	CString strAuto=_T("Auto");
	CString strman=_T("Manual");


	for(int i=1;i<=9;i++)
	{	
		CString strIndex;
		strIndex.Format(_T("%d"), i-1);
		m_FlexGrid.put_TextMatrix(i,INDEX_FIELD,strIndex);

		for(int k=0;k<=6;k++)
		{
			m_FlexGrid.put_ColAlignment(k,4);
			if (i%2==1)
			{
				m_FlexGrid.put_Row(i);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				m_FlexGrid.put_Row(i);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(COLOR_CELL);
			}
		}

		CString strTemp;
		m_fFirmwareVersion = get_curtstat_version();
		if(m_fFirmwareVersion>33.3)
		{
			float fValue;

			// Row1
			if(i==1)
			{
				//strTemp.Format(_T("%.1f"),product_register_value[216]/10.0);
				//216	130	2	Full	W/R	Internal Thermistor Sensor - Shows the filtered, calibrated value of the internal thermistor sensor
				int nFlag = product_register_value[7];
				if(product_register_value[MODBUS_PRODUCT_MODEL] ==  16)	//5E 的这里和其他寄存器不一样，所以没办法合并;
				{
					strTemp.Format(_T("%.1f"),product_register_value[MODBUS_INTERNAL_THERMISTOR]/10.0);//216

				}
				else if ((product_register_value[7]==PM_TSTAT6)
					|| (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V)
					||(product_register_value[7]==PM_TSTAT7)||(product_register_value[7]==PM_TSTAT5i)||(product_register_value[7]==PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9))
				{
				   strTemp.Format(_T("%.1f"),product_register_value[MODBUS_INTERNAL_THERMISTOR]/10.0);
				}
				else
				{
					strTemp.Format(_T("%.1f"),product_register_value[MODBUS_INTERNAL_THERMISTOR]/10.0);  //121
				}

				strTemp=strTemp+strUnit;
				m_FlexGrid.put_TextMatrix(1,VALUE_FIELD,strTemp);
				m_FlexGrid.put_TextMatrix(1,AM_FIELD,NO_APPLICATION);
				m_FlexGrid.put_TextMatrix(1,CAL_FIELD,_T("Adjust..."));
				m_FlexGrid.put_TextMatrix(1,RANG_FIELD,strUnit);
				m_FlexGrid.put_TextMatrix(1,FUN_FIELD,NO_APPLICATION);
				m_FlexGrid.put_TextMatrix(1,CUST_FIELD,NO_APPLICATION);
				continue;
			}



			 

			CString strValueUnit=GetTempUnit(product_register_value[MODBUS_ANALOG1_RANGE+i-2], i-1); //5e=359   122
			{
				if(product_register_value[MODBUS_ANALOG1_RANGE+i-2]==1)	//359  122
				{				
					fValue=float(product_register_value[MODBUS_ANALOG_INPUT1+i-2]/10.0);	//367   131
					strTemp.Format(_T("%.1f"),fValue);	

					strTemp +=strValueUnit;
				}
				else if(product_register_value[MODBUS_ANALOG1_RANGE+i-2]==3 || product_register_value[MODBUS_ANALOG1_RANGE+i-2]==5) // On/Off or Off/On ==1 On ==0 Off   359  122
				{						
					int nValue=(product_register_value[MODBUS_ANALOG_INPUT1+i-2]); //367  131
					if (nValue == 1)
					{
						strTemp = _T("On");
					}
					else
					{
						strTemp = _T("Off");
					}	

				}
				else if (product_register_value[MODBUS_ANALOG1_RANGE+i-2]==4 )  // custom sensor	359 122
				{					
					fValue=float(product_register_value[MODBUS_ANALOG_INPUT1+i-2]/10.0);	//367  131
					strTemp.Format(_T("%.1f"), (float)fValue/10.0);	
					strTemp +=strValueUnit;
				}
				else if(product_register_value[MODBUS_ANALOG1_RANGE+i-2]==2)	//359 122
				{
					nValue=product_register_value[MODBUS_ANALOG_INPUT1+i-2];		//367  131
					strTemp.Format(_T("%0.1f%%"),  (float)nValue);
				}
				else
				{
					 strTemp.Format(_T("%d"),product_register_value[367+i-2]);//lsc
					//strTemp.Format(_T("Raw"));

				}						
				m_FlexGrid.put_TextMatrix(i,VALUE_FIELD,strTemp);
				strTemp.Empty();
			}

			//309	141	2	Full	W/R	Input auto/ manual enable.
			// column 2  Auto/Manual // Ö»ÓÐIN1£¬2²ÅÓÐ,internal sensor Ã»ÓÐ£¬´óÓÚ1 // 
			if(i>1)//( i== 2 || i == 3)
			{
				nValue=product_register_value[MODBUS_INPUT_MANU_ENABLE];//309    141
				BYTE bFilter=0x01;
				bFilter = bFilter<< (i-2);
				if((nValue & bFilter))
				{
					m_FlexGrid.put_TextMatrix(i,AM_FIELD,strman);			
				}
				else
				{
					m_FlexGrid.put_TextMatrix(i,AM_FIELD,strAuto);				
				}
			}
			else
			{
				m_FlexGrid.put_Col(AM_FIELD);
				m_FlexGrid.put_Row(i);
				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
			}

			// column 3  Calibrate			
			{
				m_FlexGrid.put_TextMatrix(i,CAL_FIELD,_T("Adjust..."));				
			}

			// column 4  Range 
			//if( i >= 2 )
			{
				int nItem=product_register_value[MODBUS_ANALOG1_RANGE+i-2];		//359  122
				if(nItem>=0 && nItem<=ANALOG_RANG_NUMBER)
				{
					strTemp=analog_range[nItem];
				}
				m_FlexGrid.put_TextMatrix(i,RANG_FIELD,strTemp);
			}

			//298	167	1	Low byte	W/R	Analog input1 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// column 5 Function Ö»ÓÐIN1£¬2²ÅÓÐ


			// 				298	167	1	Low byte	W/R	Analog input1 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				299	168	1	Low byte	W/R	Analog input2 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				169	1	Low byte	W/R	(future)Analog input3 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				170	1	Low byte	W/R	(future)Analog input4 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				171	1	Low byte	W/R	(future)Analog input5 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				172	1	Low byte	W/R	(future)Analog input6 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				173	1	Low byte	W/R	(future)Analog input7 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				174	1	Low byte	W/R	(future)Analog input8 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.


			nValue=product_register_value[MODBUS_ANALOG1_FUNCTION+i-2];		//298   167
			if(product_register_value[MODBUS_PRODUCT_MODEL] ==  6)
			{
				if (nValue>=0&&nValue<8)//tstat6
				{		
					strTemp=INPUT_FUNS[nValue];
					m_FlexGrid.put_TextMatrix(i,FUN_FIELD,strTemp);
				}
			}
			else
			{
				if(i== 2 || i==3)
				{
					nValue=product_register_value[298+i-2];
					strTemp=INPUT_FUNS[nValue];
					m_FlexGrid.put_TextMatrix(i,FUN_FIELD,strTemp);
				}
				else
				{
				m_FlexGrid.put_Col(FUN_FIELD);
				m_FlexGrid.put_Row(i);
				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
				}
			}

			//	}
			// 			else
			// 			{
			// 				m_FlexGrid.put_Col(FUN_FIELD);
			// 				m_FlexGrid.put_Row(i);
			// 				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
			// 			}
			// column 6 custom tables
			if(product_register_value[MODBUS_PRODUCT_MODEL] ==  6)
			{
				m_customBtn.ShowWindow(SW_HIDE);
				m_FlexGrid.put_TextMatrix(i,CUST_FIELD,NO_APPLICATION);
			}
			else
			{
				if( i== 2 || i==3)
				{
					if(product_register_value[MODBUS_ANALOG_IN1]==4)	//188
					{
						m_FlexGrid.put_TextMatrix(i,CUST_FIELD,_T("Custom..."));
					}
					else
					{
						m_customBtn.ShowWindow(SW_HIDE);
						m_FlexGrid.put_TextMatrix(i,CUST_FIELD,NO_APPLICATION);
					}
				}
				else
				{
					m_FlexGrid.put_Col(CUST_FIELD);
					m_FlexGrid.put_Row(i);
					m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
				}
			}

		}
		else  // another lower version of software	,The version too low, can't be changed .  Fance.	
		{
			int nValue;

			if(product_register_value[341+i-2]==1)	//341
			{
				nValue=(int)(product_register_value[349+i-2]/10.0);
				strTemp.Format(_T("%.1f"),nValue);
			}
			else
			{
				strTemp.Format(_T("%d"),product_register_value[349+i-2]);
			}
			m_FlexGrid.put_TextMatrix(i,VALUE_FIELD,strTemp);

			strTemp.Empty();
			int nItem=product_register_value[341+i-2];
			if(nItem>=0&&nItem<=4)
			{
				strTemp=analog_range[nItem];					
			}
			m_FlexGrid.put_TextMatrix(i,AM_FIELD,strTemp);
		}
	}	

}
void CInputSetDlg::Fresh_GridForTstat5E(){
 

	CString strUnit=GetTempUnit();
	CString strTemp;	
	int nValue=0;
	//float nValue=0;
	CString strAuto=_T("Auto");
	CString strman=_T("Manual");


	for(int i=1;i<=9;i++)
	{	
		CString strIndex;
		strIndex.Format(_T("%d"), i-1);
		m_FlexGrid.put_TextMatrix(i,INDEX_FIELD,strIndex);

		 

		CString strTemp;
		m_fFirmwareVersion = get_curtstat_version();
		if(m_fFirmwareVersion>33.3)
		{
			float fValue;

			// Row1
			if(i==1)
			{
				//strTemp.Format(_T("%.1f"),product_register_value[216]/10.0);
				//216	130	2	Full	W/R	Internal Thermistor Sensor - Shows the filtered, calibrated value of the internal thermistor sensor
				int nFlag = product_register_value[7];
				
				if(product_register_value[MODBUS_PRODUCT_MODEL] ==  16)	//5E 的这里和其他寄存器不一样，所以没办法合并;
				{
					strTemp.Format(_T("%.1f"),product_register_value[MODBUS_INTERNAL_THERMISTOR]/10.0);//216

				}
				else if ((product_register_value[7]==PM_TSTAT6)
					|| (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V)
					||(product_register_value[7]==PM_TSTAT7)||(product_register_value[7]==PM_TSTAT5i)||(product_register_value[7]==PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9))
				{
					strTemp.Format(_T("%.1f"),product_register_value[MODBUS_INTERNAL_THERMISTOR]/10.0);
				}
				else
				{
					strTemp.Format(_T("%.1f"),product_register_value[MODBUS_INTERNAL_THERMISTOR]/10.0);  //121
				}

				strTemp=strTemp+strUnit;
				m_FlexGrid.put_TextMatrix(1,VALUE_FIELD,strTemp);
				m_FlexGrid.put_TextMatrix(1,AM_FIELD,NO_APPLICATION);
				m_FlexGrid.put_TextMatrix(1,CAL_FIELD,_T("Adjust..."));
				m_FlexGrid.put_TextMatrix(1,RANG_FIELD,strUnit);
				m_FlexGrid.put_TextMatrix(1,FUN_FIELD,NO_APPLICATION);
				m_FlexGrid.put_TextMatrix(1,CUST_FIELD,NO_APPLICATION);
				continue;
			}

			CString strValueUnit=GetTempUnit(product_register_value[MODBUS_ANALOG1_RANGE+i-2], i-1); //5e=359   122
			{
				if(product_register_value[MODBUS_ANALOG1_RANGE+i-2]==1)	//359  122
				{				
					fValue=float(product_register_value[MODBUS_ANALOG_INPUT1+i-2]/10.0);	//367   131
					strTemp.Format(_T("%.1f"),fValue);	

					strTemp +=strValueUnit;
				}
				else if(product_register_value[MODBUS_ANALOG1_RANGE+i-2]==3 || product_register_value[MODBUS_ANALOG1_RANGE+i-2]==5) // On/Off or Off/On ==1 On ==0 Off   359  122
				{						
					int nValue=(product_register_value[MODBUS_ANALOG_INPUT1+i-2]); //367  131
					if (nValue == 1)
					{
						strTemp = _T("On");
					}
					else
					{
						strTemp = _T("Off");
					}	

				}
				else if (product_register_value[MODBUS_ANALOG1_RANGE+i-2]==4 )  // custom sensor	359 122
				{					
					fValue=float(product_register_value[MODBUS_ANALOG_INPUT1+i-2]/10.0);	//367  131
					strTemp.Format(_T("%.1f"), (float)fValue/10.0);	
					strTemp +=strValueUnit;
				}
				else if(product_register_value[MODBUS_ANALOG1_RANGE+i-2]==2)	//359 122
				{
					nValue=product_register_value[MODBUS_ANALOG_INPUT1+i-2];		//367  131
					strTemp.Format(_T("%0.1f%%"),  (float)nValue);
				}
				else
				{
					strTemp.Format(_T("%d"),product_register_value[367+i-2]);//lsc
					//strTemp.Format(_T("Raw"));

				}						
				m_FlexGrid.put_TextMatrix(i,VALUE_FIELD,strTemp);
				strTemp.Empty();
			}

			//309	141	2	Full	W/R	Input auto/ manual enable.
			// column 2  Auto/Manual // Ö»ÓÐIN1£¬2²ÅÓÐ,internal sensor Ã»ÓÐ£¬´óÓÚ1 // 
			if(i>1)//( i== 2 || i == 3)
			{
				nValue=product_register_value[MODBUS_INPUT_MANU_ENABLE];//309    141
				BYTE bFilter=0x01;
				bFilter = bFilter<< (i-2);
				if((nValue & bFilter))
				{
					m_FlexGrid.put_TextMatrix(i,AM_FIELD,strman);			
				}
				else
				{
					m_FlexGrid.put_TextMatrix(i,AM_FIELD,strAuto);				
				}
			}
			else
			{
				m_FlexGrid.put_Col(AM_FIELD);
				m_FlexGrid.put_Row(i);
				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
			}

			// column 3  Calibrate			
			{
				m_FlexGrid.put_TextMatrix(i,CAL_FIELD,_T("Adjust..."));				
			}

			// column 4  Range 
			//if( i >= 2 )
			{
				int nItem=product_register_value[MODBUS_ANALOG1_RANGE+i-2];		//359  122
				if(nItem>=0 && nItem<=ANALOG_RANG_NUMBER)
				{
					strTemp=analog_range_TSTAT6[nItem];
				}
				m_FlexGrid.put_TextMatrix(i,RANG_FIELD,strTemp);
			}

			//298	167	1	Low byte	W/R	Analog input1 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// column 5 Function Ö»ÓÐIN1£¬2²ÅÓÐ


			// 				298	167	1	Low byte	W/R	Analog input1 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				299	168	1	Low byte	W/R	Analog input2 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				169	1	Low byte	W/R	(future)Analog input3 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				170	1	Low byte	W/R	(future)Analog input4 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				171	1	Low byte	W/R	(future)Analog input5 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				172	1	Low byte	W/R	(future)Analog input6 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				173	1	Low byte	W/R	(future)Analog input7 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				174	1	Low byte	W/R	(future)Analog input8 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.


			nValue=product_register_value[MODBUS_ANALOG1_FUNCTION+i-2];		//298   167
			if(product_register_value[MODBUS_PRODUCT_MODEL] ==  6)
			{
				if (nValue>=0&&nValue<8)//tstat6
				{		
					strTemp=INPUT_FUNS[nValue];
					m_FlexGrid.put_TextMatrix(i,FUN_FIELD,strTemp);
				}
			}
			else
			{
				if(i== 2 || i==3)
				{
					nValue=product_register_value[298+i-2];
					strTemp=INPUT_FUNS[nValue];
					m_FlexGrid.put_TextMatrix(i,FUN_FIELD,strTemp);
				}
				else
				{
					m_FlexGrid.put_Col(FUN_FIELD);
					m_FlexGrid.put_Row(i);
					m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
				}
			}

			//	}
			// 			else
			// 			{
			// 				m_FlexGrid.put_Col(FUN_FIELD);
			// 				m_FlexGrid.put_Row(i);
			// 				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
			// 			}
			// column 6 custom tables
			if(product_register_value[MODBUS_PRODUCT_MODEL] ==  6)
			{
				m_customBtn.ShowWindow(SW_HIDE);
				m_FlexGrid.put_TextMatrix(i,CUST_FIELD,NO_APPLICATION);
			}else
			{
				if( i== 2 || i==3)
				{
					if(product_register_value[MODBUS_ANALOG_IN1]==4)	//188
					{
						m_FlexGrid.put_TextMatrix(i,CUST_FIELD,_T("Custom..."));
					}
					else
					{
						m_customBtn.ShowWindow(SW_HIDE);
						m_FlexGrid.put_TextMatrix(i,CUST_FIELD,NO_APPLICATION);
					}
				}
				else
				{
					m_FlexGrid.put_Col(CUST_FIELD);
					m_FlexGrid.put_Row(i);
					m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
				}
			}

		}
		else  // another lower version of software	,The version too low, can't be changed .  Fance.	
		{
			int nValue;

			if(product_register_value[341+i-2]==1)	//341
			{
				nValue=(int)(product_register_value[349+i-2]/10.0);
				strTemp.Format(_T("%.1f"),nValue);
			}
			else
			{
				strTemp.Format(_T("%d"),product_register_value[349+i-2]);
			}
			m_FlexGrid.put_TextMatrix(i,VALUE_FIELD,strTemp);

			strTemp.Empty();
			int nItem=product_register_value[341+i-2];
			if(nItem>=0&&nItem<=4)
			{
				strTemp=analog_range_TSTAT6[nItem];					
			}
			m_FlexGrid.put_TextMatrix(i,RANG_FIELD,strTemp);
		}
	}	

	SetCurrentCell();
}
void CInputSetDlg::OnClickTstat6Grid(int nRow, int nCol, CRect rcCell)
{
    CString txt=m_FlexGrid.get_TextMatrix(nRow,nCol);
 
    
 	if (nCol == INDEX_FIELD)// && nRow == 1) // 序号都不能改
 	{
 		return;
 	}
 
	if (nRow == 1 && ( nCol == FUN_FIELD||nCol == NAME_FIELD))
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	if (nCol == NAME_FIELD)
	{
		if ((nRow== 1)||(nRow==10)||(nRow==11))
			return;

		CString strValue = m_FlexGrid.get_TextMatrix(nRow,nCol);
		m_inputNameEdt.MoveWindow(rcCell); //移动到选中格的位置，覆盖
		m_inputNameEdt.ShowWindow(SW_SHOW);

		m_inputNameEdt.BringWindowToTop();
		//m_RangCombox.SelectString(-1,strValue);

		m_inputNameEdt.SetWindowText(strValue);
		m_inputNameEdt.SetFocus(); //获取焦点
	}
	//////////////////////////////////////////////////////////////////////////
	//
	if (nCol == VALUE_FIELD)
	{
	   
		//309	141	2	Full	W/R	Input auto/ manual enable.
		int nValue;// = product_register_value[309];
	 
		nValue=product_register_value[MODBUS_INPUT_MANU_ENABLE];

		int nFilter = 0x01;
		nFilter = nFilter << (m_nCurRow - 1);
		CString strValue = m_FlexGrid.get_TextMatrix(nRow,nCol);

		if (m_nCurRow==1)
		{
// 			if (product_register_value[695]!=0)
// 			{
				m_inValueEdit.MoveWindow(rcCell); //移动到选中格的位置，覆盖
				m_inValueEdit.ShowWindow(SW_SHOW);

				m_inValueEdit.BringWindowToTop();
				//m_RangCombox.SelectString(-1,strValue);

				m_inValueEdit.SetWindowText(strValue);
				m_inValueEdit.SetFocus(); //获取焦点
			/*}*/
		}
		else
		{
			//
			//int nRangeValue = product_register_value[359+m_nCurRow-2];
			//MODBUS_ANALOG1_RANGE
			int nRangeValue = product_register_value[MODBUS_ANALOG1_RANGE+m_nCurRow-2];
			if (nRangeValue == 3||nRangeValue==5)  // 如果是on/off，用combo
			{	
			    DealValue_Digit();
			    
			}
			else // 如果是值，用edit
			{		
				m_inValueEdit.MoveWindow(rcCell); //移动到选中格的位置，覆盖
				m_inValueEdit.ShowWindow(SW_SHOW);

				m_inValueEdit.BringWindowToTop();
				//m_RangCombox.SelectString(-1,strValue);
		
				m_inValueEdit.SetWindowText(strValue);
				m_inValueEdit.SetFocus(); //获取焦点

			}		
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//
	CString strAuto=_T("Auto");
	CString strman=_T("Manual");
	if(nCol==AM_FIELD) // A/M
	{
	OnCbnSelchangeAmcombo();
		//m_AmCombox.MoveWindow(&rcCell,1); //移动到选中格的位置
		//m_AmCombox.BringWindowToTop();
		//m_AmCombox.ShowWindow(SW_SHOW);//显示控件
		//m_AmCombox.SetFocus(); //获取焦点
		//CString strTemp;

		////309	141	2	Full	W/R	Input auto/ manual enable.
		//BYTE nValue;// =BYTE(product_register_value[309]);
	
		// 
		//nValue=BYTE(product_register_value[MODBUS_INPUT_MANU_ENABLE]);
		//BYTE nFilter = 0x01;
		//nFilter = nFilter << (nRow-1);
		//if(nValue & nFilter)
		//{
		//	strTemp = strman;			
		//}
		//else
		//{
		//	strTemp = strAuto;				
		//}
		//m_AmCombox.SetWindowText(strTemp);


	}

	//////////////////////////////////////////////////////////////////////////
	if(nCol==CAL_FIELD)    // calibrate buttons
	{	
		CRect rcUp(rcCell.left,rcCell.top,rcCell.left+rcCell.Width()/2,rcCell.bottom);
		CRect rcDown(rcCell.left+rcCell.Width()/2,rcCell.top,rcCell.right,rcCell.bottom);
		m_upButton.MoveWindow(rcUp);
		m_upButton.ShowWindow(SW_SHOW);
		m_upButton.BringWindowToTop();
		m_downButton.MoveWindow(rcDown);
		m_downButton.ShowWindow(SW_SHOW);
		m_downButton.BringWindowToTop();

		if(nRow==1)
		{
// 			int nRegisterNum = 110;
// 			if (product_register_value[111] == 2)
// 			{
// 				nRegisterNum = 317;
// 			}
// 
// 			if(product_register_value[188]==3)
// 			{
// 				m_downButton.ShowWindow(SW_HIDE);
// 				m_upButton.ShowWindow(SW_HIDE);
// 			}
// 			else
// 			{
				m_downButton.ShowWindow(SW_SHOW);
				m_upButton.ShowWindow(SW_SHOW);
//			}
		}
		else //(nRow==2)
		{
			//359	122	1	Low byte	W/R	ANALOG INPUT1 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			int nFlag = product_register_value[7];
			int nRange;// = product_register_value[359+nRow-2];
			
			if ((product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT6)||(product_register_value[7]== PM_TSTAT5i)
				|| (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V)
				||(product_register_value[7]== PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9))
			{
				nRange= product_register_value[122+nRow-2];
			}else
			{
				nRange = product_register_value[359+nRow-2];

			}

			
			if( nRange == 3 || nRange == 5)
			{

				m_downButton.ShowWindow(SW_HIDE);
				m_upButton.ShowWindow(SW_HIDE);
			}
			else
			{
				////309	141	2	Full	W/R	Input auto/ manual enable.
				BYTE nValue;// =BYTE(product_register_value[309]);
			
				 

				nValue=product_register_value[MODBUS_INPUT_MANU_ENABLE];
				BYTE nFilter = 0x01;
				nFilter = nFilter << (nRow-1);
				if(nValue & nFilter) // only sel Auto, can calibrate
				{
					m_downButton.ShowWindow(SW_HIDE);
					m_upButton.ShowWindow(SW_HIDE);
				}
				else
				{
					m_downButton.ShowWindow(SW_SHOW);
					m_upButton.ShowWindow(SW_SHOW);
				}
			}
		}		
	}
	if(nCol==FILTER)
	{

		if (nRow== 1)
			return;

		CString strValue = m_FlexGrid.get_TextMatrix(nRow,nCol);
		m_Filter.MoveWindow(rcCell); //移动到选中格的位置，覆盖
		m_Filter.ShowWindow(SW_SHOW);

		m_Filter.BringWindowToTop();
		//m_RangCombox.SelectString(-1,strValue);

		m_Filter.SetWindowText(strValue);
		m_Filter.SetFocus(); //获取焦点
	}
	//////////////////////////////////////////////////////////////////////////
	if(nCol==RANG_FIELD) 
	{
	if ((nRow==10)||(nRow==11))
	{
	return;
	}
		m_RangCombox.MoveWindow(&rcCell,1); //移动到选中格的位置
		m_RangCombox.BringWindowToTop();
		m_RangCombox.ShowWindow(SW_SHOW);//显示控件
		m_RangCombox.SetFocus(); //获取焦点
	
		CString strTemp;
		if(nRow == 1) // use 121
		{	
			m_RangCombox.ResetContent();
			for(UINT i=0;i<m_strUnitList.size();i++)
			{
				m_RangCombox.AddString(m_strUnitList.at(i));
			}
			//121	104	1	Low byte	W/R(Reboot after write)	DEGC_OR_F, engineering units, Deg C = 0, Deg F = 1
			int nFlag = product_register_value[7];
			int nValue;// = product_register_value[121];//tatat6 :104
			
			if ((product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT6)||(product_register_value[7]== PM_TSTAT5i)

				|| (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V)
				||(product_register_value[7]== PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9))
			{
				nValue = product_register_value[104];
			}else
			{
				nValue = product_register_value[121];
			}
			if (nValue==0||nValue==1)//tstat6
			strTemp = m_strUnitList[nValue];
		}
		else
		{
			m_RangCombox.ResetContent();	
			//359	122	1	Low byte	W/R	ANALOG INPUT1 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			//121	104	1	Low byte	W/R(Reboot after write)	DEGC_OR_F, engineering units, Deg C = 0, Deg F = 1
			int nValue;// = product_register_value[359+nRow-2];
// 			if (product_register_value[7] ==6)
// 			{
				nValue = product_register_value[122+nRow-2];
				if(product_register_value[104]==0)
				{
					for(int i=0;i<ANALOG_RANG_NUMBER;i++)
					{
						m_RangCombox.AddString(analog_range_TSTAT6[i]);
					}	
					strTemp = analog_range_TSTAT6[nValue];
				}
				if(product_register_value[104]==1)
				{
					for(int i=0;i<ANALOG_RANG_NUMBER;i++)
					{
						m_RangCombox.AddString(analog_range_TSTAT6[i]);
					}
					strTemp = analog_range_TSTAT6[nValue];
				}
// 			}
// 			else
// 			{
// 				nValue = product_register_value[359+nRow-2];
// 				if(product_register_value[121]==0)
// 				{
// 					for(int i=0;i<ANALOG_RANG_NUMBER;i++)
// 					{
// 						m_RangCombox.AddString(analog_range[i]);
// 					}	
// 					strTemp = analog_range[nValue];
// 				}
// 				if(product_register_value[121]==1)
// 				{
// 					for(int i=0;i<ANALOG_RANG_NUMBER;i++)
// 					{
// 						m_RangCombox.AddString(analog_range[i]);
// 					}
// 					strTemp = analog_range[nValue];
// 				}	
// 			}

		}
		m_RangCombox.SetWindowText(strTemp);
	//	int sel=m_RangCombox.GetCurSel();
	ShowRangeCombox();
	}

	//////////////////////////////////////////////////////////////////////////
	if(nCol == FUN_FIELD)
	{
		m_inputFinCombox.ResetContent();
		for(int i=0;i<8;i++)
			m_inputFinCombox.AddString(INPUT_FUNS[i]);
		
		m_inputFinCombox.ShowWindow(SW_SHOW);//显示控件
		m_inputFinCombox.MoveWindow(rcCell); //移动到选中格的位置，覆盖
		m_inputFinCombox.BringWindowToTop();
		//m_inputFinCombox.SelectString(-1,strValue);
		m_inputFinCombox.SetFocus(); //获取焦点
		//298	167	1	Low byte	W/R	Analog input1 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.

		int nValue;//=product_register_value[298+nRow-2];
		int nFlag = product_register_value[7];
		if ((product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT6)

			|| (nFlag == PM_TSTAT8_WIFI) || (nFlag == PM_TSTAT8_OCC) || (nFlag == PM_TSTAT7_ARM) || (nFlag == PM_TSTAT8_220V)
			||(product_register_value[7]== PM_TSTAT5i)||(product_register_value[7]== PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9))
		{
			nValue=product_register_value[167+nRow-2];
		}
		else
		{
			nValue=product_register_value[298+nRow-2];
		}
		CString strTemp(INPUT_FUNS[nValue]);
		m_inputFinCombox.SetWindowText(strTemp);

	}

 
}


// 359	122	1	Low byte	W/R	ANALOG INPUT1 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 2	123	1	Low byte	W/R	ANALOG INPUT2 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 3	124	1	Low byte	W/R	ANALOG INPUT3 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 4	125	1	Low byte	W/R	ANALOG INPUT4 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 5	126	1	Low byte	W/R	ANALOG INPUT5 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 6	127	1	Low byte	W/R	ANALOG INPUT6 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 7	128	1	Low byte	W/R	ANALOG INPUT7 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 8	129	1	Low byte	W/R	ANALOG INPUT8 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
//121	104	1	Low byte	W/R(Reboot after write)	DEGC_OR_F, engineering units, Deg C = 0, Deg F = 1


void CInputSetDlg::OnCbnSelchangeRangComboFor5E()
{	
	if(m_nCurRow==1&&m_nCurCol==RANG_FIELD)
	{
		int nindext=m_RangCombox.GetCurSel();
		//if ((product_register_value[7] == 6)||(product_register_value[7] == 7))//tstat6
		int ret=write_one(g_tstat_id,MODBUS_DEGC_OR_F,nindext);//121  104 

		if(ret<0)
		{
			AfxMessageBox(_T("Write error!Please try again!"));
			return;
		}
		product_register_value[MODBUS_DEGC_OR_F]=nindext;


	}


	//CString analog_range[ANALOG_RANG_NUMBER]={_T("UNUSED"),_T("10KC Therm"),_T("0-100%"),_T("On/Off"),_T("Custom Sensor1"),_T("Off/On"),
	//	_T("Custom Sensor2"),_T("Occupied/Unoccupied"),_T("Unoccupied/Occupied"),_T("Open/Close"),_T("Close/Open")};

	if(m_nCurRow >= 2 && m_nCurCol==RANG_FIELD)
	{
		  m_crange=m_RangCombox.GetCurSel();
		int regvalue=m_crange;
	
	
		int ret=write_one(g_tstat_id,MODBUS_ANALOG1_RANGE+m_nCurRow-2,regvalue);	//5e=359  6=122
		if(ret<0)
		{
			AfxMessageBox(_T("Write Fail!Please try again!"));
			return;
		}
		product_register_value[MODBUS_ANALOG1_RANGE+m_nCurRow-2] = regvalue;

		
	}

	Read_Multi(g_tstat_id,&product_register_value[MODBUS_TEMPRATURE_CHIP],MODBUS_TEMPRATURE_CHIP,20);
	if(m_nModel == PM_TSTAT6 || m_nModel == PM_TSTAT7||m_nModel == PM_TSTAT5i||m_nModel == PM_TSTAT8 || m_nModel == PM_TSTAT9
		|| (m_nModel == PM_TSTAT8_WIFI) || (m_nModel == PM_TSTAT8_OCC) || (m_nModel == PM_TSTAT7_ARM) || (m_nModel == PM_TSTAT8_220V)
		)  //tstat6
	{
	Fresh_GridForTstat6();
	}
	else{
	Fresh_GridForTstat5E();
	}
	


	//OnBnClickedRefreshbutton();//stat6 TSTAT6点击这时，再去刷新则会出现错误，崩溃.
	//Fresh_Grid();
}

//Recode by Fance ,the old one is too many and misellaneous with trifles
void CInputSetDlg::OnBnClickedUpbuttonFor5E()
{
	
	if(m_nCurRow==1&&m_nCurCol==CAL_FIELD)
	{
		int nValue=product_register_value[MODBUS_TEMPRATURE_CHIP]+1;	//101 121
		write_one(g_tstat_id,MODBUS_TEMPRATURE_CHIP,nValue);//101  121
		product_register_value[MODBUS_TEMPRATURE_CHIP] = nValue;	//101  121
	}
	else if (m_nCurRow<10)
	{
		int nAddr =MODBUS_ANALOG_INPUT1+m_nCurRow-2;	//367  131
		write_one(g_tstat_id, nAddr, product_register_value[nAddr]+1);
		product_register_value[nAddr] = product_register_value[nAddr]+1;//367  131
	}


	else if (m_nCurRow==10)
	{
		if (product_register_value[MODBUS_TSTAT6_HUM_AM]==0)
		{
			int nValue=product_register_value[MODBUS_TSTAT6_HUM_AVALUE]+1;
			if (nValue<0)
			{
				return;
			}
			int ret=write_one(g_tstat_id,MODBUS_TSTAT6_HUM_AVALUE,nValue);
			if (ret>0)
			{
				product_register_value[MODBUS_TSTAT6_HUM_AVALUE]=nValue;
			}

		}
		else 
		{

			int nValue=product_register_value[MODBUS_TSTAT6_HUM_MVALUE]+1;
			if (nValue<0)
			{
				return;
			}
			int ret=write_one(g_tstat_id,MODBUS_TSTAT6_HUM_MVALUE,nValue);
			if (ret>0)
			{
				product_register_value[MODBUS_TSTAT6_HUM_MVALUE]=nValue;
			}   
		}
		}
	else if (m_nCurRow==11)
	{
		if (product_register_value[MODBUS_TSTAT6_CO2_AM]==0)
		{      


			int nValue=product_register_value[MODBUS_TSTAT6_CO2_AVALUE]+1;
			if (nValue<0)
			{
				return;
			}
			int ret=write_one(g_tstat_id,MODBUS_TSTAT6_CO2_AVALUE,nValue);
			if (ret>0)
			{
				product_register_value[MODBUS_TSTAT6_CO2_AVALUE]=nValue;
			}   

		}
		else
		{


			int nValue=product_register_value[MODBUS_TSTAT6_CO2_MVALUE]+1;
			if (nValue<0)
			{
				return;
			}
			int ret=write_one(g_tstat_id,MODBUS_TSTAT6_CO2_MVALUE,nValue);
			if (ret>0)
			{
				product_register_value[MODBUS_TSTAT6_CO2_MVALUE]=nValue;
			} 

		}

	}


	Fresh_GridForAll();
}


void CInputSetDlg::OnBnClickedDownbuttonFor5E()
{
	


	 


		if(m_nCurRow==1&&m_nCurCol==CAL_FIELD)
		{
			int nValue=product_register_value[MODBUS_TEMPRATURE_CHIP]-1;	//101 121
			write_one(g_tstat_id,MODBUS_TEMPRATURE_CHIP,nValue);//101  121
			product_register_value[MODBUS_TEMPRATURE_CHIP] = nValue;	//101  121
		}
		else if (m_nCurRow<10)
		{
			int nAddr =MODBUS_ANALOG_INPUT1+m_nCurRow-2;	//367  131
			write_one(g_tstat_id, nAddr, product_register_value[nAddr]-1);
			product_register_value[MODBUS_ANALOG_INPUT1] = product_register_value[nAddr]-1;//367  131
		}
		else if (m_nCurRow==10)
		{
			if (product_register_value[MODBUS_TSTAT6_HUM_AM]==0)
			{
			     int nValue=product_register_value[MODBUS_TSTAT6_HUM_AVALUE]-1;
				 if (nValue<0)
				 {
				   return;
				 }
				 int ret=write_one(g_tstat_id,MODBUS_TSTAT6_HUM_AVALUE,nValue);
				 if (ret>0)
				 {
				 product_register_value[MODBUS_TSTAT6_HUM_AVALUE]=nValue;
				 }

			}
			else
			{
				 
				int nValue=product_register_value[MODBUS_TSTAT6_HUM_MVALUE]-1;
				if (nValue<0)
				{
					return;
				}
				int ret=write_one(g_tstat_id,MODBUS_TSTAT6_HUM_MVALUE,nValue);
				if (ret>0)
				{
					product_register_value[MODBUS_TSTAT6_HUM_MVALUE]=nValue;
				}   
			}
		}
		else if (m_nCurRow==11)
		{
			if (product_register_value[MODBUS_TSTAT6_CO2_AM]==0)
			{      


				int nValue=product_register_value[MODBUS_TSTAT6_CO2_AVALUE]-1;
				if (nValue<0)
				{
					return;
				}
				int ret=write_one(g_tstat_id,MODBUS_TSTAT6_CO2_AVALUE,nValue);
				if (ret>0)
				{
					product_register_value[MODBUS_TSTAT6_CO2_AVALUE]=nValue;
				}   

			}
			else
			{
				 

				int nValue=product_register_value[MODBUS_TSTAT6_CO2_MVALUE]-1;
				if (nValue<0)
				{
					return;
				}
				int ret=write_one(g_tstat_id,MODBUS_TSTAT6_CO2_MVALUE,nValue);
				if (ret>0)
				{
					product_register_value[MODBUS_TSTAT6_CO2_MVALUE]=nValue;
				} 

			}

		}
	Fresh_GridForAll();
}

void CInputSetDlg::OnCbnSelchangeAmcombo()
{
	
	//309	141	2	Full	W/R	Input auto/ manual enable.
	//第一行的时候   【695】=0为 auto 读内置传感器的，【695】=1 为manual模式 手动设置
	
 
	int sel;//=m_AmCombox.GetCurSel();
	 if (m_nCurRow==1)
	 {
	    sel=product_register_value[695];
		if (sel==0)
		{
		sel=1;
		}
		else
		{
		sel=0;
		}
			int nRet=write_one(g_tstat_id,695,sel);
		 
			if (nRet<0)
			{
				AfxMessageBox(_T("Access fail!Retry it later!"));
			}
			product_register_value[695]=sel;
			Read_Multi(g_tstat_id,&product_register_value[MODBUS_TEMPRATURE_CHIP],MODBUS_TEMPRATURE_CHIP,20);
	 }
	 else if (m_nCurRow==10)
	{
		sel=product_register_value[MODBUS_TSTAT6_HUM_AM];
		if (sel==0)
		{
			sel=1;
		}
		else
		{
			sel=0;
		}

		 
		  int nRet=write_one(g_tstat_id,MODBUS_TSTAT6_HUM_AM,sel);
		  if (nRet<0)
		  {
		  AfxMessageBox(_T("Access fail!Retry it later!"));
		  }
		  product_register_value[MODBUS_TSTAT6_HUM_AM]=sel;
		 

	}
	else if(m_nCurRow==11)
	{
		sel=product_register_value[MODBUS_TSTAT6_CO2_AM];
		if (sel==0)
		{
			sel=1;
		}
		else
		{
			sel=0;
		}

		
			int nRet=write_one(g_tstat_id,MODBUS_TSTAT6_CO2_AM,sel);
			if (nRet<0)
			{
				AfxMessageBox(_T("Access fail!Retry it later!"));
			}
			product_register_value[MODBUS_TSTAT6_CO2_AM]=sel;
	  
	}
	else
	{
		if (m_nCurRow==4)
		{
			 

			sel=product_register_value[545];
			if (sel==0)
			{
				sel=1;
			}
			else
			{
				sel=0;
			}


			int nRet = write_one(g_tstat_id,545,sel); 
			if(nRet<0)
			{
				AfxMessageBox(_T("Access fail!Retry it later!"));
			}
			product_register_value[545] = sel;

			
		}
		else
		{
		    
			CString StrAutoManual=m_FlexGrid.get_TextMatrix(m_nCurRow,m_nCurCol);
		   
	//		sel=product_register_value[MODBUS_INPUT_MANU_ENABLE];
// 			if (sel==0)
// 			{
// 				sel=1;
// 			}
// 			else
// 			{
// 				sel=0;
// 			}

			int	nValue = product_register_value[MODBUS_INPUT_MANU_ENABLE];//MODBUS_INPUT_MANU_ENABLE
			if(StrAutoManual.CompareNoCase(L"Auto")==0)
			{ // Manual
				int nFilter = 0x01;
				nFilter = nFilter << (m_nCurRow - 2);
				nValue = nValue | nFilter;
				m_downButton.ShowWindow(SW_HIDE);
				m_upButton.ShowWindow(SW_HIDE);
			}
			else
			{ // Auto
				int nFilter = 0xFE;
				nFilter = nFilter << (m_nCurRow - 2);
				nValue = nValue & nFilter;
			}	
			//MODBUS_INPUT_MANU_ENABLE
			int nRet = write_one(g_tstat_id,MODBUS_INPUT_MANU_ENABLE,nValue);//309  141
			if(nRet<0)
			{
				AfxMessageBox(_T("Access fail!Retry it later!"));
			}
			product_register_value[MODBUS_INPUT_MANU_ENABLE] = nValue;
		}//nValue=product_register_value[MODBUS_INPUT_MANU_ENABLE];
		
	}


	Fresh_GridForAll();
}
void CInputSetDlg::DealValue_Digit(){

	//m_valueCombx.ResetContent();
	//if (m_crange==9||m_crange==10)
	//{
	//	m_valueCombx.InsertString(0,_T("Unoccupied"));
	//	m_valueCombx.InsertString(1,_T("Occupied"));
	//}
	//else if (m_crange==7||m_crange==8)
	//{
	//	m_valueCombx.InsertString(0,_T("Closed"));
	//	m_valueCombx.InsertString(1,_T("Open"));
	//}
	//else{
	//	m_valueCombx.InsertString(0,_T("Off"));
	//	m_valueCombx.InsertString(1,_T("On"));
	//}


	//m_valueCombx.MoveWindow(rcCell); //移动到选中格的位置，覆盖
	//m_valueCombx.ShowWindow(SW_SHOW);
	//m_valueCombx.BringWindowToTop();
	////m_valueCombx.SelectString(-1,strValue);
	//m_valueCombx.SetFocus(); //获取焦点
	//m_valueCombx.SetWindowText(strValue);


	m_inValueEdit.ShowWindow(SW_HIDE);
	CString strText;
    strText=m_FlexGrid.get_TextMatrix(m_nCurRow,m_nCurCol);
	//m_inValueEdit.GetWindowText(strText);
	if(strText.IsEmpty())
		return;
		int nValue=0;
	if (strText.CompareNoCase(L"Unoccupied")==0||strText.CompareNoCase(L"Closed")==0||strText.CompareNoCase(L"Off")==0)
	{
	nValue=1;
	}
	else if (strText.CompareNoCase(L"Occupied")==0||strText.CompareNoCase(L"Open")==0||strText.CompareNoCase(L"On")==0)
	{
	nValue=0;
	}
	else{
	return;
	}
	
	 
	if(m_nCurCol==VALUE_FIELD)
	{
		if (m_nCurRow==1)
		{
			int ret=write_one(g_tstat_id,MODBUS_TEMPRATURE_CHIP,nValue*10);
			if (ret>0)
			{
				//product_register_value[MODBUS_TEMPRATURE_CHIP]=nValue*10;
				product_register_value[MODBUS_INTERNAL_THERMISTOR]=nValue*10;
			} 
			else
			{
				AfxMessageBox(_T("Write Error"));
			}


		}
		else if (m_nCurRow<10)
		{
			int nAddr =MODBUS_ANALOG_INPUT1+m_nCurRow-2;	//367  131
			if (nValue==product_register_value[nAddr])
			{
				return ;
			}

			int ret=write_one(g_tstat_id, nAddr, nValue);
			if (ret>0)
			{
				product_register_value[nAddr]=nValue;
			}
			else
			{
				AfxMessageBox(_T("Try again!"));
			}
		}
		else if (m_nCurRow==10)
		{
			// 		if (product_register_value[MODBUS_TSTAT6_HUM_AM]==0)
			// 		{
			// 			int nValue=product_register_value[MODBUS_TSTAT6_HUM_AVALUE]-1;
			// 			if (nValue<0)
			// 			{
			// 				return;
			// 			}
			// 			int ret=write_one(g_tstat_id,MODBUS_TSTAT6_HUM_AVALUE,nValue);
			// 			if (ret>0)
			// 			{
			// 				product_register_value[MODBUS_TSTAT6_HUM_AVALUE]=nValue;
			// 			}
			// 
			// 		}
			// 		else
			// 		{

			int RegnValue=product_register_value[MODBUS_TSTAT6_HUM_MVALUE];

			if (RegnValue==nValue*10)
			{
				return;
			}
			int ret=write_one(g_tstat_id,MODBUS_TSTAT6_HUM_MVALUE,nValue*10);
			if (ret>0)
			{
				product_register_value[MODBUS_TSTAT6_HUM_MVALUE]=nValue*10;
			}   
			else
			{
				AfxMessageBox(_T("Try again!"));
			}
			/*	}*/
		}
		else if (m_nCurRow==11)
		{
			if (product_register_value[MODBUS_TSTAT6_CO2_AM]==0)
			{      


				int RegnValue=product_register_value[MODBUS_TSTAT6_CO2_AVALUE];
				if (RegnValue==nValue)
				{
					return;
				}
				int ret=write_one(g_tstat_id,MODBUS_TSTAT6_CO2_AVALUE,nValue);
				if (ret>0)
				{
					product_register_value[MODBUS_TSTAT6_CO2_AVALUE]=nValue;
				}   
				else
				{
					AfxMessageBox(_T("Try again!"));
				}

			}
			else
			{


				int RegnValue=product_register_value[MODBUS_TSTAT6_CO2_MVALUE];
				if (RegnValue==nValue)
				{
					return;
				}
				int ret=write_one(g_tstat_id,MODBUS_TSTAT6_CO2_MVALUE,nValue);
				if (ret>0)
				{
					product_register_value[MODBUS_TSTAT6_CO2_MVALUE]=nValue;
				}
				else
				{
					AfxMessageBox(_T("Try again!"));
				} 

			}

		}

	}
	 
	Fresh_Grid();
}
 

void CInputSetDlg::OnEnKillfocusInvalueeditFor5E()
{
	m_inValueEdit.ShowWindow(SW_HIDE);
	CString strText;
	m_inValueEdit.GetWindowText(strText);
	if(strText.IsEmpty())
		return;
	int nValue=_wtoi(strText);

	if(m_nCurCol==VALUE_FIELD)
	{
		if (m_nCurRow==1)
		{
			int ret=write_one(g_tstat_id,MODBUS_TEMPRATURE_CHIP,nValue*10);
			if (ret>0)
			{
				//product_register_value[MODBUS_TEMPRATURE_CHIP]=nValue*10;
				product_register_value[MODBUS_INTERNAL_THERMISTOR]=nValue*10;
			} 
			else
			{
				AfxMessageBox(_T("Write Error"));
			}
		}
	 else if (m_nCurRow<10)
	{
	    int nAddr =MODBUS_ANALOG_INPUT1+m_nCurRow-2;	//367  131
	    if (nValue*10==product_register_value[nAddr])
	    {
		return ;
	    }
		
		int ret=write_one(g_tstat_id, nAddr, nValue*10);
		if (ret>0)
		{
		product_register_value[nAddr]=nValue*10;
		}
		else
		{
		  AfxMessageBox(_T("Try again!"));
		}
	}
	else if (m_nCurRow==10)
	{
// 		if (product_register_value[MODBUS_TSTAT6_HUM_AM]==0)
// 		{
// 			int nValue=product_register_value[MODBUS_TSTAT6_HUM_AVALUE]-1;
// 			if (nValue<0)
// 			{
// 				return;
// 			}
// 			int ret=write_one(g_tstat_id,MODBUS_TSTAT6_HUM_AVALUE,nValue);
// 			if (ret>0)
// 			{
// 				product_register_value[MODBUS_TSTAT6_HUM_AVALUE]=nValue;
// 			}
// 
// 		}
// 		else
// 		{

			int RegnValue=product_register_value[MODBUS_TSTAT6_HUM_MVALUE];

			if (RegnValue==nValue*10)
			{
				return;
			}
			int ret=write_one(g_tstat_id,MODBUS_TSTAT6_HUM_MVALUE,nValue*10);
			if (ret>0)
			{
				product_register_value[MODBUS_TSTAT6_HUM_MVALUE]=nValue*10;
			}   
			else
			{
			 AfxMessageBox(_T("Try again!"));
			}
	/*	}*/
	}
	else if (m_nCurRow==11)
	{
		if (product_register_value[MODBUS_TSTAT6_CO2_AM]==0)
		{      


			int RegnValue=product_register_value[MODBUS_TSTAT6_CO2_AVALUE];
			if (RegnValue==nValue)
			{
				return;
			}
			int ret=write_one(g_tstat_id,MODBUS_TSTAT6_CO2_AVALUE,nValue);
			if (ret>0)
			{
				product_register_value[MODBUS_TSTAT6_CO2_AVALUE]=nValue;
			}   
			else
			{
				AfxMessageBox(_T("Try again!"));
			}

		}
		else
		{


			int RegnValue=product_register_value[MODBUS_TSTAT6_CO2_MVALUE];
			if (RegnValue==nValue)
			{
				return;
			}
			int ret=write_one(g_tstat_id,MODBUS_TSTAT6_CO2_MVALUE,nValue);
			if (ret>0)
			{
				product_register_value[MODBUS_TSTAT6_CO2_MVALUE]=nValue;
			}
			else
			{
				AfxMessageBox(_T("Try again!"));
			} 

		}

	}

	}

	Fresh_GridForAll();
}


// On/Off select on == write reg 1, off == 0
void CInputSetDlg::OnCbnKillfocusValuecomboFor5E()
{
	m_valueCombx.ShowWindow(SW_HIDE);
	int nItem=0;
	nItem=m_valueCombx.GetCurSel();

	if (nItem == 0) // select on
	{
		int ret=write_one(g_tstat_id,367+m_nCurRow-2,1);
		if (ret<=0)
		{
		AfxMessageBox(_T("Write Error"));
		} 
		product_register_value[367+m_nCurRow-2]=1;
	}
	else
	{
	int ret=	write_one(g_tstat_id,367+m_nCurRow-2,0);
		if (ret<=0)
		{
			AfxMessageBox(_T("Write Error"));
		} 
		product_register_value[367+m_nCurRow-2]=0;
	}
	Fresh_GridForAll();
}

void CInputSetDlg::ClickMsflexgrid5E( int nRow, int nCol, CRect rcCell )
{
	if (nCol == INDEX_FIELD)
	// && nRow == 1) // 序号都不能改
	{
		return;
	}

	//lsc20120828
// 	if (nCol == NAME_FIELD  && nRow > 3)  // 只有2个输入
// 	{
// 		return;
// 	}
// 	if (nRow > 3  && (nCol == VALUE_FIELD  || nCol == RANG_FIELD  || nCol == FUN_FIELD  )) 
// 	{
// 		return;
// 	}
// 	if (nRow == 1 && (nCol == VALUE_FIELD  || nCol == FUN_FIELD || nCol == AM_FIELD || nCol == CAL_FIELD))
// 	{
// 		return;
// 	}

	//////////////////////////////////////////////////////////////////////////
	//
	if (nCol == NAME_FIELD)
	{
		CString strValue = m_FlexGrid.get_TextMatrix(nRow,nCol);
		m_inputNameEdt.MoveWindow(rcCell); //移动到选中格的位置，覆盖
		m_inputNameEdt.ShowWindow(SW_SHOW);

		m_inputNameEdt.BringWindowToTop();
		//m_RangCombox.SelectString(-1,strValue);

		m_inputNameEdt.SetWindowText(strValue);
		m_inputNameEdt.SetFocus(); //获取焦点
	}
	//////////////////////////////////////////////////////////////////////////
	//
	if (nCol == VALUE_FIELD)
	{
		int nValue = product_register_value[MODBUS_INPUT_MANU_ENABLE];
		int nFilter = 0x01;
		nFilter = nFilter << (m_nCurRow - 1);
		CString strValue = m_FlexGrid.get_TextMatrix(nRow,nCol);
		if (nValue & nFilter)
		{
			int nRangeValue = product_register_value[359+m_nCurRow-2];
			if (nRangeValue == 3)  // 如果是on/off，用combo
			{	
				m_valueCombx.ResetContent();

				m_valueCombx.InsertString(0,_T("On"));
				m_valueCombx.InsertString(1,_T("Off"));

				m_valueCombx.MoveWindow(rcCell); //移动到选中格的位置，覆盖
				m_valueCombx.ShowWindow(SW_SHOW);
				m_valueCombx.BringWindowToTop();
				//m_valueCombx.SelectString(-1,strValue);
				m_valueCombx.SetFocus(); //获取焦点
				m_valueCombx.SetWindowText(strValue);
			}
			else // 如果是值，用edit
			{		
				m_inValueEdit.MoveWindow(rcCell); //移动到选中格的位置，覆盖
				m_inValueEdit.ShowWindow(SW_SHOW);

				m_inValueEdit.BringWindowToTop();
				//m_RangCombox.SelectString(-1,strValue);

				m_inValueEdit.SetWindowText(strValue);
				m_inValueEdit.SetFocus(); //获取焦点

			}		
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//
	CString strAuto=_T("Auto");
	CString strman=_T("Manual");
	if(nCol==AM_FIELD) // A/M
	{
		m_AmCombox.MoveWindow(&rcCell,1); //移动到选中格的位置
		m_AmCombox.BringWindowToTop();
		m_AmCombox.ShowWindow(SW_SHOW);//显示控件
		m_AmCombox.SetFocus(); //获取焦点
		CString strTemp;
		BYTE nValue =BYTE(product_register_value[MODBUS_INPUT_MANU_ENABLE]);
		BYTE nFilter = 0x01;
		nFilter = nFilter << (nRow-1);
		if(nValue & nFilter)
		{
			strTemp = strman;			
		}
		else
		{
			strTemp = strAuto;				
		}
		m_AmCombox.SetWindowText(strTemp);
	}

	//////////////////////////////////////////////////////////////////////////
	if(nCol==CAL_FIELD)    // calibrate buttons
	{	
		CRect rcUp(rcCell.left,rcCell.top,rcCell.left+rcCell.Width()/2,rcCell.bottom);
		CRect rcDown(rcCell.left+rcCell.Width()/2,rcCell.top,rcCell.right,rcCell.bottom);
		m_upButton.MoveWindow(rcUp);
		m_upButton.ShowWindow(SW_SHOW);
		m_upButton.BringWindowToTop();
		m_downButton.MoveWindow(rcDown);
		m_downButton.ShowWindow(SW_SHOW);
		m_downButton.BringWindowToTop();

		if(nRow==1)
		{
			// 			int nRegisterNum = 110;
			// 			if (product_register_value[111] == 2)
			// 			{
			// 				nRegisterNum = 317;
			// 			}
			// 
			// 			if(product_register_value[188]==3)
			// 			{
			// 				m_downButton.ShowWindow(SW_HIDE);
			// 				m_upButton.ShowWindow(SW_HIDE);
			// 			}
			// 			else
			// 			{
			m_downButton.ShowWindow(SW_SHOW);
			m_upButton.ShowWindow(SW_SHOW);
			//			}
		}
		else //(nRow==2)
		{
			int nRange = product_register_value[359+nRow-2];
			if( nRange == 3 || nRange == 5)
			{

				m_downButton.ShowWindow(SW_HIDE);
				m_upButton.ShowWindow(SW_HIDE);
			}
			else
			{
				BYTE nValue =BYTE(product_register_value[MODBUS_INPUT_MANU_ENABLE]);
				BYTE nFilter = 0x01;
				nFilter = nFilter << (nRow-1);
				if(nValue & nFilter) // only sel Auto, can calibrate
				{
					m_downButton.ShowWindow(SW_HIDE);
					m_upButton.ShowWindow(SW_HIDE);
				}
				else
				{
					m_downButton.ShowWindow(SW_SHOW);
					m_upButton.ShowWindow(SW_SHOW);
				}
			}
		}		
	}

	//////////////////////////////////////////////////////////////////////////
	if(nCol==RANG_FIELD) 
	{	
		m_RangCombox.MoveWindow(&rcCell,1); //移动到选中格的位置
		m_RangCombox.BringWindowToTop();
		m_RangCombox.ShowWindow(SW_SHOW);//显示控件
		m_RangCombox.SetFocus(); //获取焦点

		CString strTemp;
		if(nRow == 1) // use 121
		{	
			m_RangCombox.ResetContent();
			for(UINT i=0;i<m_strUnitList.size();i++)
			{
				m_RangCombox.AddString(m_strUnitList.at(i));
			}
			int nValue = product_register_value[121];
			strTemp = m_strUnitList[nValue];
		}
		else
		{
			m_RangCombox.ResetContent();	
			int nValue = product_register_value[359+nRow-2];
			if(product_register_value[121]==0)
			{
				for(int i=0;i<ANALOG_RANG_NUMBER;i++)
				{
					m_RangCombox.AddString(analog_range_TSTAT6[i]);
				}	
				strTemp = analog_range_TSTAT6[nValue];
			}
			if(product_register_value[121]==1)
			{
				for(int i=0;i<ANALOG_RANG_NUMBER;i++)
				{
					m_RangCombox.AddString(analog_range_TSTAT6[i]);
				}
				strTemp = analog_range_TSTAT6[nValue];
			}	
		}
		m_RangCombox.SetWindowText(strTemp);

		ShowRangeCombox();
	}

	//////////////////////////////////////////////////////////////////////////
	if(nCol == FUN_FIELD)
	{
		m_inputFinCombox.ResetContent();
		for(int i=0;i<8;i++)
			m_inputFinCombox.AddString(INPUT_FUNS[i]);

		m_inputFinCombox.ShowWindow(SW_SHOW);//显示控件
		m_inputFinCombox.MoveWindow(rcCell); //移动到选中格的位置，覆盖
		m_inputFinCombox.BringWindowToTop();
		//m_inputFinCombox.SelectString(-1,strValue);
		m_inputFinCombox.SetFocus(); //获取焦点

		int nValue=product_register_value[298+nRow-2];
		CString strTemp(INPUT_FUNS[nValue]);
		m_inputFinCombox.SetWindowText(strTemp);

	}

	//////////////////////////////////////////////////////////////////////////
	if(nCol == CUST_FIELD)
	{
		if(nRow==2&&product_register_value[MODBUS_ANALOG_IN1]==4)
		{
			m_customBtn.ShowWindow(SW_SHOW);
			m_customBtn.MoveWindow(rcCell); //移动到选中格的位置，覆盖
			m_customBtn.BringWindowToTop();
			m_customBtn.SetFocus(); //获取焦点
		}
		if(nRow==3&&product_register_value[189]==4)
		{
			m_customBtn.ShowWindow(SW_SHOW);
			m_customBtn.MoveWindow(rcCell); //移动到选中格的位置，覆盖
			m_customBtn.BringWindowToTop();
			m_customBtn.SetFocus(); //获取焦点
		}
	}
	
}
 
void CInputSetDlg::OnClose()
{
	 
	if(b_is_fresh==true)
	{
		AfxMessageBox(_T("It's refreshing ,can not exit at this time!"));
		return;
	}

	CDialog::OnClose();
}


void CInputSetDlg::OnEnKillfocusFilter()
{
	UpdateData(TRUE);  CString str;
	if (m_nCurRow==10)
	{
		int ret=write_one(g_tstat_id,MODBUS_TSTAT6_HUM_FILTER,m_filterValue);
		if (ret>0)
		{
			product_register_value[MODBUS_TSTAT6_HUM_FILTER]=m_filterValue;

			str.Format(_T("%d"),m_filterValue);
			m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,str);

		} 
		else
		{
			m_filterValue=product_register_value[MODBUS_TSTAT6_HUM_FILTER];
			str.Format(_T("%d"),m_filterValue);
			m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,str);
		}
		return;
	}
	if (m_nCurRow==11)
	{
		int ret=write_one(g_tstat_id,MODBUS_TSTAT6_CO2_FILTER,m_filterValue);
		if (ret>0)
		{
			product_register_value[MODBUS_TSTAT6_CO2_FILTER]=m_filterValue;

			str.Format(_T("%d"),m_filterValue);
			m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,str);

		} 
		else
		{
			m_filterValue=product_register_value[MODBUS_TSTAT6_CO2_FILTER];
			str.Format(_T("%d"),m_filterValue);
			m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,str);
		}
		return;
	}

	int ret=write_one(g_tstat_id,141+m_nCurRow,m_filterValue);
	if (ret>0)
	{
	  product_register_value[141+m_nCurRow]=m_filterValue;
	  
	  str.Format(_T("%d"),m_filterValue);
	  m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,str);

	} 
	else
	{
	   m_filterValue=product_register_value[141+m_nCurRow];
	   str.Format(_T("%d"),m_filterValue);
	   m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,str);
	}
	UpdateData(FALSE);
}


void CInputSetDlg::OnCbnSelchangeValuecombo()
{
	int nItem=0;
	nItem=m_valueCombx.GetCurSel();
	if(m_nCurCol==VALUE_FIELD&&m_nCurRow==2)
	{
		if(nItem==0)//off
		{
			int ret=write_one(g_tstat_id,MODBUS_EXTERNAL_SENSOR_0,0);
			if (ret>0)
			{
				product_register_value[MODBUS_EXTERNAL_SENSOR_0]=0;
				////AfxMessageBox(_T("Write Ok"));
			} 
			else
			{
				AfxMessageBox(_T("Write Error"));
			}
		}
		if(nItem==1)
		{  
			int	ret=	write_one(g_tstat_id,MODBUS_EXTERNAL_SENSOR_0,1);
			if (ret>0)
			{
				product_register_value[MODBUS_EXTERNAL_SENSOR_0]=1;
				/*//AfxMessageBox(_T("Write Ok"));*/
			} 
			else
			{
				AfxMessageBox(_T("Write Error"));
			}
		}
	}
	if(m_nCurCol==VALUE_FIELD&&m_nCurRow==3)
	{
		if(nItem==0)//off
		{int ret=write_one(g_tstat_id,MODBUS_EXTERNAL_SENSOR_1,0);
		if (ret>0)
		{
			product_register_value[MODBUS_EXTERNAL_SENSOR_1]=0;
			////AfxMessageBox(_T("Write Ok"));
		} 
		else
		{
			AfxMessageBox(_T("Write Error"));
		}}
		if(nItem==1)
		{int ret=	write_one(g_tstat_id,MODBUS_EXTERNAL_SENSOR_1,1);
		if (ret>0)
		{
			product_register_value[MODBUS_EXTERNAL_SENSOR_1]=1;
			////AfxMessageBox(_T("Write Ok"));
		} 
		else
		{
			AfxMessageBox(_T("Write Error"));
		}}
	}
	if(m_nCurCol==VALUE_FIELD&&m_nCurRow==4)
	{
		if (product_register_value[546]==0)
		{
			if(nItem==0)//off
			{
				int ret=	write_one(g_tstat_id,MODBUS_DIGITAL_INPUT,0);
				{
					if (ret>0)
					{
						product_register_value[MODBUS_DIGITAL_INPUT]=0;
						////AfxMessageBox(_T("Write Ok"));

					} 
					else
					{
						AfxMessageBox(_T("Write Error"));
					}
				}
			}
			else
			{
				int ret=	write_one(g_tstat_id,MODBUS_DIGITAL_INPUT,1);
				if (ret>0)
				{
					product_register_value[MODBUS_DIGITAL_INPUT]=1;
				//	//AfxMessageBox(_T("Write Ok"));

				} 
				else
				{
					AfxMessageBox(_T("Write Error"));
				}
			}
		}



		if (product_register_value[546]==1)
		{

			if(nItem==0)
			{
				int ret=	write_one(g_tstat_id,MODBUS_DIGITAL_INPUT,1);
				if (ret>0)
				{
					product_register_value[MODBUS_DIGITAL_INPUT]=1;
				//	//AfxMessageBox(_T("Write Ok"));

				} 
				else
				{
					AfxMessageBox(_T("Write Error"));
				}
			}
			else
			{ 

				int ret=	write_one(g_tstat_id,MODBUS_DIGITAL_INPUT,0);
				if (ret>0)
				{
					product_register_value[MODBUS_DIGITAL_INPUT]=0;
					////AfxMessageBox(_T("Write Ok"));

				} 
				else
				{
					AfxMessageBox(_T("Write Error"));
				}
			}
		}	
	}
	Fresh_GridForAll();
}


void CInputSetDlg::OnBnClickedFresh()
{
	//for(int i=0;i<(9);i++)	//暂定为0 ，因为TSTAT6 目前为600多
	//{
	//	int itemp = 0;
	//	itemp = Read_Multi(g_tstat_id,&product_register_value[i*(100)],i*(100),100,5);

	//	Sleep(200);
	//}
	//Fresh_Grid();
}


 
void CInputSetDlg::DblClickMsflexgrid1()
{
	ClickMsflexgrid_Click();
}


void CInputSetDlg::ClickMsflexgrid1()
{
	long lRow,lCol;

	lRow = m_FlexGrid.get_RowSel();//获取点击的行号	
	lCol = m_FlexGrid.get_ColSel(); //获取点击的列号
	if(lRow>m_FlexGrid.get_Rows()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow == 0||lCol==0) //如果点击标题行，也无效
		return;
 
	 
	 HideAllControls();
	 

	 
	if(m_old_move_col!=0){
		FLEX_GRID1_PUT_COLOR_STR(m_old_move_row,m_old_move_col,m_old_Cell_color);
	}
	m_old_move_col=lCol;
	m_old_move_row=lRow;//put_CellBackColor
	m_FlexGrid.put_Row(lRow);
	m_FlexGrid.put_Col(lCol);
	m_old_Cell_color=m_FlexGrid.get_CellBackColor();
	FLEX_GRID1_PUT_COLOR_STR(lRow,lCol,RGB(0,255,0));


}


void CInputSetDlg::OnCbnSelendokRangCombo()
{
	int nFlag = product_register_value[7];
	if(m_nModel == PM_TSTAT6 || m_nModel == PM_TSTAT7|| m_nModel == PM_TSTAT5E|| m_nModel == PM_PM5E || m_nModel == PM_PM5E_ARM ||
        m_nModel==PM_TSTATRUNAR|| m_nModel == PM_TSTAT5i|| m_nModel == PM_TSTAT8 || m_nModel == PM_TSTAT9
		|| (m_nModel == PM_TSTAT8_WIFI) || (m_nModel == PM_TSTAT8_OCC) || (m_nModel == PM_TSTAT7_ARM) || (m_nModel == PM_TSTAT8_220V)
		)  //tstat6
	{
		OnCbnSelchangeRangComboFor5E();
		return;
	}
	m_downButton.ShowWindow(SW_HIDE);
	m_upButton.ShowWindow(SW_HIDE);

	if(m_nCurRow==1&&m_nCurCol==RANG_FIELD)
	{
		int nindext=m_RangCombox.GetCurSel();
		int ret=write_one(g_tstat_id,121,nindext);
		 if (ret>0)
		 {
		  product_register_value[121]=nindext;
		 }
		 else
		 {
		  AfxMessageBox(_T("Fail"));
		 }
	}

	if(m_nCurRow==2&&m_nCurCol==RANG_FIELD)
	{
		int nindext=m_RangCombox.GetCurSel();
		m_crange=nindext;
		if (nindext==6)
		{
		nindext=3;
		}
		if (nindext==7)
		{
		nindext=5;
		}
		 
		int ret=write_one(g_tstat_id,MODBUS_ANALOG_IN1,nindext);

		 if (ret>0)
		 {  
		 product_register_value[MODBUS_ANALOG_IN1]=nindext;


		 CppSQLite3DB SqliteDBT3000;
		 CppSQLite3Table table;
		 CppSQLite3Query q;
		 SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);

		 CString sql;
		 sql.Format(_T("Select * from Value_Range where CInputNo=%d and SN=%d"),m_nCurRow-1,m_sn);
		 q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);

		 if (!q.eof())//有表但是没有对应序列号的值
		 {

			 sql.Format(_T("update Value_Range set CRange = %d where CInputNo=%d and SN=%d "),m_crange,m_nCurRow-1,m_sn);
			 SqliteDBT3000.execDML((UTF8MBSTR)sql);
		 }
		 else
		 {
		    
			 sql.Format(_T("Insert into Value_Range( SN,CInputNo,CRange) values('%d','%d','%d')"),m_sn,m_nCurRow-1,m_crange);
			SqliteDBT3000.execDML((UTF8MBSTR)sql);
		 }
		 
		 SqliteDBT3000.closedb();


		 if(product_register_value[MODBUS_ANALOG_IN1]==4)
		 {
			 m_FlexGrid.put_TextMatrix(2,CUST_FIELD,_T("Custom..."));
		 }
		 else
		 {
			 m_customBtn.ShowWindow(SW_HIDE);
			 m_FlexGrid.put_TextMatrix(2,CUST_FIELD,NO_APPLICATION);
		 }
		 } 
		 else
		 {
		   AfxMessageBox(_T("Try again"));
		 }
		
	}
	if(m_nCurRow==3&&m_nCurCol==RANG_FIELD)
	{
		int nindext=m_RangCombox.GetCurSel();
		m_crange=nindext;
		if (nindext==6)
		{
			m_crange=nindext;
			nindext=3;
		}
		if (nindext==7)
		{
			m_crange=7;
			nindext=5;
		}
		int ret=write_one(g_tstat_id,MODBUS_ANALOG_IN2,nindext);
		if (ret>0)
		{
			product_register_value[MODBUS_ANALOG_IN2]=nindext;

			CppSQLite3DB SqliteDBT3000;
			CppSQLite3Table table;
			CppSQLite3Query q;
			SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);

			CString sql;
			sql.Format(_T("Select * from Value_Range where CInputNo=%d and SN=%d"),m_nCurRow-1,m_sn);
			q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);

			if (!q.eof())//有表但是没有对应序列号的值
			{
				sql.Format(_T("update Value_Range set CRange = %d where CInputNo=%d and SN=%d "),m_crange,m_nCurRow-1,m_sn);
				SqliteDBT3000.execDML((UTF8MBSTR)sql);
			}
			else
			{
				sql.Format(_T("Insert into Value_Range ( SN,CInputNo,CRange) values('%d','%d','%d')"),m_sn,m_nCurRow-1,m_crange);
				SqliteDBT3000.execDML((UTF8MBSTR)sql);
			}
		 
			 SqliteDBT3000.closedb();

		if(product_register_value[MODBUS_ANALOG_IN2]==4)
		{
			m_FlexGrid.put_TextMatrix(3,CUST_FIELD,_T("Custom..."));
		}
		else
		{
			m_customBtn.ShowWindow(SW_HIDE);
			m_FlexGrid.put_TextMatrix(3,CUST_FIELD,NO_APPLICATION);
		}
		} 
		else
		{
		AfxMessageBox(_T("Try again"));
		}
		
	}
	if(m_nCurRow==4&&m_nCurCol==RANG_FIELD)
	{
		int nindext=m_RangCombox.GetCurSel();
		m_crange=nindext;
		 
		int ret=write_one(g_tstat_id,MODBUS_DIGITAL_IN1,nindext);
		if (ret>0)
		{
		product_register_value[MODBUS_DIGITAL_IN1]=nindext;

		Sleep(200);
		}
	Fresh_GridForAll();
}
}

void CInputSetDlg::OnCbnSelendokValuecombo()
{
	
	OnCbnSelchangeValuecombo();
}
