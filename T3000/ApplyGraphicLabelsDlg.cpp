// ApplyGraphicLabelsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "MainFrm.h"
#include "ApplyGraphicLabelsDlg.h"
#include "../SQLiteDriver/CppSQLite3.h"

// ApplyGraphicLabelsDlg dialog

IMPLEMENT_DYNAMIC(ApplyGraphicLabelsDlg, CDialog)

ApplyGraphicLabelsDlg::ApplyGraphicLabelsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ApplyGraphicLabelsDlg::IDD, pParent)
{
	m_strMainBuilding.Empty();
	m_strSubNetName.Empty();
	m_nTotalRecoders=0;

}

ApplyGraphicLabelsDlg::~ApplyGraphicLabelsDlg()
{
}

void ApplyGraphicLabelsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDBUILDING_MSFLEXGRID, m_FlexGrid);
	DDX_Control(pDX, IDC_COMBO1, m_subNetList);
}


BEGIN_MESSAGE_MAP(ApplyGraphicLabelsDlg, CDialog)
	ON_BN_CLICKED(IDOK, &ApplyGraphicLabelsDlg::OnBnClickedOk)
	ON_COMMAND(ID_IDR_UNSELECT,OnUnSelect)
	ON_COMMAND(ID_IDR_SELECT32824,OnSelect)
	ON_COMMAND(ID_IDR_SELECTALL,OnSelectAll)
	ON_CBN_SELCHANGE(IDC_COMBO1, &ApplyGraphicLabelsDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()

// ApplyGraphicLabelsDlg message handlers
void ApplyGraphicLabelsDlg::OnUnSelect()
{
	m_FlexGrid.put_TextMatrix(m_nCurRow,0,_T("UnSelect"));
}
void ApplyGraphicLabelsDlg::OnSelect()
{

   if (m_nCurRow==0)
   {
   return;
   }
   

	m_FlexGrid.put_TextMatrix(m_nCurRow,0,_T("Select"));

}

void ApplyGraphicLabelsDlg::OnSelectAll()
{
	for(int i=0;i<m_nTotalRecoders;i++)
	{
		m_FlexGrid.put_TextMatrix(i+1,0,_T("Select"));
	}
}
void ApplyGraphicLabelsDlg::SetLabelLst(vector<Label_information>* pLabelLst)
{
	m_labelList.clear();
	for(int i=0;i<(int)pLabelLst->size();i++)
	{
		m_labelList.push_back(pLabelLst->at(i));
	}
}
void ApplyGraphicLabelsDlg::SetBuilding(CString strMainBuilding,CString strSubNetName)
{
	m_strMainBuilding=strMainBuilding;
	m_strSubNetName=strSubNetName;

}
BOOL ApplyGraphicLabelsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
		
// 	m_pCon.CreateInstance(_T("ADODB.Connection"));
// 	m_pRs.CreateInstance(_T("ADODB.Recordset"));
// 	m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);


	SetWindowText(_T("Set graphic labels to the others dialog"));
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	m_strMainBuilding= pFrame->m_strCurMainBuildingName;
	m_strSubNetName= pFrame->m_strCurSubBuldingName;
	m_subNetList.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);

//	m_strScanSubNetNameCobox.AddString(_T("All subNets..."));

/*	int k=0;
	for(int i=0;i<pFrame->m_subNetLst.size();i++)
	{
		if(m_strSubNetName.CompareNoCase(pFrame->m_subNetLst.at(i).strBuildingName)==0)
			k=i;
		m_subNetList.AddString(pFrame->m_subNetLst.at(i).strBuildingName);
	}
	if(m_subNetList.GetCount()>0)
		m_subNetList.SetCurSel(k);
		*/

	ReloadAddBuildingDB();

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void ApplyGraphicLabelsDlg::OnBnClickedOk()
{
	ApplyTo();

	OnOK();
}
BEGIN_EVENTSINK_MAP(ApplyGraphicLabelsDlg, CDialog)
	ON_EVENT(ApplyGraphicLabelsDlg, IDC_ADDBUILDING_MSFLEXGRID, DISPID_CLICK, ApplyGraphicLabelsDlg::ClickAddbuildingMsflexgrid, VTS_NONE)
END_EVENTSINK_MAP()

void ApplyGraphicLabelsDlg::ClickAddbuildingMsflexgrid()
{
	
	long lRow,lCol;
	lRow = m_FlexGrid.get_RowSel();//获取点击的行号	
	lCol = m_FlexGrid.get_ColSel(); //获取点击的列号

	m_nCurRow=lRow;
	m_nCurCol=lCol;
	/*
	m_strBuilding_Name2=m_AddBuiding_FlexGrid.get_TextMatrix(lRow,AB_NAME);
	m_strBuildingName=m_AddBuiding_FlexGrid.get_TextMatrix(lRow,AB_NAME);
	m_strConnetion=m_AddBuiding_FlexGrid.get_TextMatrix(lRow,AB_CONNECTION);
	m_strProtocol=m_AddBuiding_FlexGrid.get_TextMatrix(lRow,AB_PROTOCOL);
	m_strIpAddress=m_AddBuiding_FlexGrid.get_TextMatrix(lRow,AB_IPADDRESS);
	m_strIpPort=m_AddBuiding_FlexGrid.get_TextMatrix(lRow,AB_IPPORT);
	m_strComPort=m_AddBuiding_FlexGrid.get_TextMatrix(lRow,AB_COMPORT);
	m_strBaudrat=m_AddBuiding_FlexGrid.get_TextMatrix(lRow,AB_BAUDRAT);
	*/



	CRect rect;
	m_FlexGrid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	CDC* pDC =GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_FlexGrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_FlexGrid.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_FlexGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_FlexGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rcCell(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rcCell.OffsetRect(rect.left+1,rect.top+1);
	ReleaseDC(pDC);
	CString strValue = m_FlexGrid.get_TextMatrix(lRow,lCol);

	if(0==lRow)
		return;

	if(0==lCol)
	{
		//ID_BUILDINGSEL_SELECT
		CMenu menu;
		menu.LoadMenu(IDR_APPLYGRAPHICLABELS);
		CMenu *pmenu=menu.GetSubMenu(0);
		CPoint point;
		GetCursorPos(&point);
		pmenu->TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN ,point.x,point.y,this);	

	}
}

void ApplyGraphicLabelsDlg::OnCbnSelchangeCombo1()
{
	CString strText;
	int nIdext=m_subNetList.GetCurSel();
	if(nIdext>=0)
	{
		m_subNetList.GetLBText(nIdext,strText);
		m_strSubNetName=strText;

	}
	ReloadAddBuildingDB();
	
}
void ApplyGraphicLabelsDlg::ReloadAddBuildingDB()
{
	m_FlexGrid.Clear();
	
	//m_FlexGrid.put_ColAlignment(0,4);
	CppSQLite3DB SqliteDBBuilding;
	CppSQLite3Table table;
	CppSQLite3Query q;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);


	m_FlexGrid.put_TextMatrix(0,1,_T("Serial ID"));
	m_FlexGrid.put_ColWidth(1,1000);
	m_FlexGrid.put_TextMatrix(0,2,_T("Product ID"));
	m_FlexGrid.put_ColWidth(2,1000);

	m_FlexGrid.put_TextMatrix(0,3,_T("Floor"));
	m_FlexGrid.put_ColWidth(3,1300);

	m_FlexGrid.put_TextMatrix(0,4,_T("Room"));
	m_FlexGrid.put_ColWidth(4,1300);

	m_FlexGrid.put_TextMatrix(0,5,_T("Product Name"));
	m_FlexGrid.put_ColWidth(5,1400);

	CString strSql;
	strSql.Format(_T("select * from ALL_NODE where MainBuilding_Name = '%s'and Building_Name='%s'"),m_strMainBuilding,m_strSubNetName);
	//m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);			
	q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
	table = SqliteDBBuilding.getTable((UTF8MBSTR)strSql);
	m_nTotalRecoders=table.numRows();
	m_FlexGrid.put_Rows(m_nTotalRecoders+1);	
	int temp_row=0;
	CString str_temp;
	str_temp.Empty();
	_variant_t temp_variant;
	while(!q.eof())
	{	
		 
			m_strID = q.getValuebyName(L"Product_class_ID");
		if(_wtoi(m_strID)==100)// not include NC
		{
				q.nextRow();
				continue;
		}
			

		++temp_row;
	
		
		m_FlexGrid.put_TextMatrix(temp_row,0,_T("Select"));

	 
			m_strID = q.getValuebyName(L"Serial_ID");
		m_FlexGrid.put_ColAlignment(1,4);
		m_FlexGrid.put_TextMatrix(temp_row,1,m_strID);
	
		 
			m_strID = q.getValuebyName(L"Product_ID");
		m_FlexGrid.put_ColAlignment(2,4);
		m_FlexGrid.put_TextMatrix(temp_row,2,m_strID);
	 
			m_strID = q.getValuebyName(L"Floor_name");
		m_FlexGrid.put_ColAlignment(3,4);
		m_FlexGrid.put_TextMatrix(temp_row,3,m_strID);

		 
			m_strID = q.getValuebyName(L"Room_name");
		m_FlexGrid.put_ColAlignment(4,4);
		m_FlexGrid.put_TextMatrix(temp_row,4,m_strID);
		
			 
			m_strID = q.getValuebyName(L"Product_name");
		m_FlexGrid.put_ColAlignment(5,4);
		m_FlexGrid.put_TextMatrix(temp_row,5,m_strID);
		q.nextRow();
	}
	SqliteDBBuilding.closedb();
}
void ApplyGraphicLabelsDlg::ApplyTo()
{
	CString strTemp;
	strTemp=_T("All the graphic & babels for current TSTAT will be applied to all the others,are you sure to continue?");
	if(AfxMessageBox(strTemp,MB_OKCANCEL)==IDOK)
	{

	}
	else
		return;
	
	CppSQLite3DB SqliteDBBuilding;
	CppSQLite3Table table;
	CppSQLite3Query q;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);


	CString strSelect;
	CString strSerialID;
	CString strProductID;
	int nSerialID;
	int nProductID;
	CString strSql;
	for(int i=0;i<m_nTotalRecoders;i++)
	{
		strSelect=m_FlexGrid.get_TextMatrix(i+1,0);
		if(strSelect.CompareNoCase(_T("Select"))==0)
		{
			strSerialID=m_FlexGrid.get_TextMatrix(i+1,1);
			strProductID=m_FlexGrid.get_TextMatrix(i+1,2);
			if(strSerialID.IsEmpty()||strProductID.IsEmpty())
			{
				continue;
			}
			else
			{
				nSerialID=_wtoi(strSerialID);
				nProductID=_wtoi(strProductID);
			}
			try
			{


			CString strSql;
			strSql.Format(_T("delete   from Screen_Label where Serial_Num=%i and Tstat_id=%i"),nSerialID,nProductID);
			SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
			int nContyrolID=20000;
			for(int i=0;i<(int)m_labelList.size();i++)
			{
				/*
	int cstatic_id;//the id of the CStatic control button
	int input_or_output;//true == output ,false == input
	int nSerialNum;
	int tstat_id;//the id of tstat	
//	bool picture_or_text;//true == a picture;false == a text
//	CString str;//path picture file path or text 
	CString the_value;//the value of select
	int width;//the cstatic width;
	int height;//the cstatic height
	CPoint point;//the rect left/top point
	int status;//for input or output selection,such as Relay 1 ,Relay 2,Relay 3...
	COLORREF clrTxt;

	CString strTips;//indcatcor
	CString strScreenName;
	CRelayLabel* plabelCtrl;
	*/
				Label_information labelInfo=m_labelList.at(i);
				int nLeft=labelInfo.point.x;
				int nTop=labelInfo.point.y;
				int nWidth=labelInfo.width;
				int nHeight=labelInfo.height;
				CString strScreenName=labelInfo.strScreenName;
				int input_or_output=labelInfo.input_or_output;
				COLORREF clrTxt=labelInfo.clrTxt;
				COLORREF bkColor=labelInfo.bkColor;
				CString strClrTxt;
				CString strbkColor;
				strClrTxt.Format(_T("%u"),clrTxt);
				strbkColor.Format(_T("%u"),bkColor);


				int status=labelInfo.status;
				strSql.Format(_T("insert into Screen_Label values(%i,%i,%i,%i,%i,%i,%i,%i,'%s',%i,'%s','%s')"),nContyrolID,nSerialID,nLeft,nTop,
				
				nHeight,nWidth,
				nProductID,
				status,
				strScreenName,//tips
				input_or_output,
				//strScreenName,
				strClrTxt,
				strbkColor);
				try
				{



					 SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}
				nContyrolID++;
			}

		}
	}
	 SqliteDBBuilding.closedb();
}
LRESULT ApplyGraphicLabelsDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	

	return CDialog::WindowProc(message, wParam, lParam);
}
