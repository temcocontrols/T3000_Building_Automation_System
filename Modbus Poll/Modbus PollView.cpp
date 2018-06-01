
// Modbus PollView.cpp : implementation of the CModbusPollView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Modbus Poll.h"
#endif
#include "Modbus PollDoc.h"
#include "Modbus PollView.h"
#include "ChildFrm.h"
#include "WriteSingleRegisterDlg.h"
#include "WriteSingle_BinaryDlg.h"
#include "RegisterValueAnalyzerDlg.h"
#include "../SQLiteDriver/CppSQLite3.h"
#include "WriteFunctionDlg.h"
#include "WriteSingleCoilDlg.h"
 
// #include "CApplication.h"
// #include "CWorkbook.h"
// #include "CWorkbooks.h"
// #include "CWorksheet.h"
// #include "CWorkbooks.h"


// #include "excel9.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


DWORD WINAPI _Multi_Read_Fun03(LPVOID pParam);

IMPLEMENT_DYNCREATE(CModbusPollView, CFormView)

BEGIN_MESSAGE_MAP(CModbusPollView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
//	ON_BN_CLICKED(IDC_START, &CModbusPollView::OnBnClickedStart)
ON_WM_CTLCOLOR()
 
 
ON_COMMAND(ID_EDIT_ADD, &CModbusPollView::OnEditAdd)
ON_COMMAND(ID_SETUP_TXT_LOG, &CModbusPollView::OnSetupTxtLog)
ON_COMMAND(ID_SETUP_TXTLOGOUT_OFF, &CModbusPollView::OnSetupTxtlogoutOff)
ON_UPDATE_COMMAND_UI(ID_SETUP_TXTLOGOUT_OFF, &CModbusPollView::OnUpdateSetupTxtlogoutOff)
ON_UPDATE_COMMAND_UI(ID_SETUP_TXT_LOG, &CModbusPollView::OnUpdateSetupTxtLog)
ON_COMMAND(ID_SETUP_EXCELLOGINGOFF, &CModbusPollView::OnSetupExcellogingoff)
ON_UPDATE_COMMAND_UI(ID_SETUP_EXCELLOGINGOFF, &CModbusPollView::OnUpdateSetupExcellogingoff)
ON_COMMAND(ID_SETUP_EXCELLOGING, &CModbusPollView::OnSetupExcelloging)
ON_UPDATE_COMMAND_UI(ID_SETUP_EXCELLOGING, &CModbusPollView::OnUpdateSetupExcelloging)
ON_EN_KILLFOCUS(IDC_MODELNAME, &CModbusPollView::OnEnKillfocusModelname)
ON_COMMAND(ID_EDIT_CHANGEMODELNAME, &CModbusPollView::OnEditChangemodelname)
ON_COMMAND(ID_FUNCTIONS_TESTCENTER, &CModbusPollView::OnFunctionsTestcenter)
ON_COMMAND(ID_SETUP_USEASDEFAULT, &CModbusPollView::OnSetupUseasdefault)
ON_COMMAND(ID_FUNCTIONS_TESTWRITE, &CModbusPollView::OnFunctionsTestwrite)
 
ON_COMMAND(ID_EDIT_COPY, &CModbusPollView::OnEditCopy)
ON_COMMAND(ID_FILE_PRINT, &CModbusPollView::OnFilePrint32857)

ON_NOTIFY(NM_DBLCLK, IDC_MSFLEXGRID1, OnGridDblClick)
ON_NOTIFY(NM_CLICK, IDC_MSFLEXGRID1, OnGridClick)
ON_NOTIFY(GVN_ENDLABELEDIT, IDC_MSFLEXGRID1, OnGridEndEdit)



END_MESSAGE_MAP()

 


CModbusPollView::CModbusPollView()
	: CFormView(CModbusPollView::IDD)
{

	  m_Err=0;
	  m_Tx=0;
	  m_MBPoll_Function=3;
	  m_Slave_ID=GetPrivateProfileInt(_T("MBPOLL_VIEW_SETTING"),_T("SLAVE_ID"),255,g_configfile_path);
	  m_Function=GetPrivateProfileInt(_T("MBPOLL_VIEW_SETTING"),_T("FUNCTION"),2,g_configfile_path);
	  m_address=GetPrivateProfileInt(_T("MBPOLL_VIEW_SETTING"),_T("ADDRESS"),0,g_configfile_path);
	  m_Quantity=GetPrivateProfileInt(_T("MBPOLL_VIEW_SETTING"),_T("QUANTITY"),100,g_configfile_path);
	  m_Scan_Rate=GetPrivateProfileInt(_T("MBPOLL_VIEW_SETTING"),_T("SCANRATE"),1000,g_configfile_path);
	  m_Rows=GetPrivateProfileInt(_T("MBPOLL_VIEW_SETTING"),_T("ROWS"),0,g_configfile_path);
	  m_Display=GetPrivateProfileInt(_T("MBPOLL_VIEW_SETTING"),_T("DATA_TYPE"),1,g_configfile_path);
	  m_Hide_Alias_Columns=GetPrivateProfileInt(_T("MBPOLL_VIEW_SETTING"),_T("HIDEALIAS"),0,g_configfile_path);
	  m_Address_Cell=GetPrivateProfileInt(_T("MBPOLL_VIEW_SETTING"),_T("ADDRESSINCELL"),0,g_configfile_path);
	  m_PLC_Addresses=GetPrivateProfileInt(_T("MBPOLL_VIEW_SETTING"),_T("PLCBASE1"),0,g_configfile_path);

	  m_grid_cols=m_grid_rows=1;
	  m_Current_Col=m_Current_Row=1;
	  m_ischangedAddress=TRUE;

		    m_close_dlg=FALSE;
			m_function=FALSE;
			//m_MultiRead_handle=NULL;
			Show_Name=FALSE;
			m_isgetmodel=FALSE;
			m_logText=TRUE;
			m_logTextPath=_T("");
			m_logExcel=TRUE;


			m_ischangeModelName=FALSE;


			m_apply=TRUE;
			m_wronce=FALSE;
}

CModbusPollView::~CModbusPollView()
{
	 
    //2018 04 17 dufan  如果是成功的连接过 打开MB POLL 通过配置文件读取 modbus id ， 关闭时 恢复默认255.
    WritePrivateProfileStringW(_T("MBPOLL_Setting"), _T("Modbus ID"), _T("255"), g_configfile_path);
// 	if(m_MultiRead_handle != NULL)
// 		TerminateThread(m_MultiRead_handle, 0);
// 	m_MultiRead_handle=NULL;
}

CString CModbusPollView::Find_RegName(int index){
	vector<DBRegister>::iterator it;
	for (it=m_VecregisterData.begin();it!=m_VecregisterData.end();it++)
	{ 
		int tempregaddress=index+m_address;
		if (it->RegAddress==tempregaddress)
		{
			return it->RegName;
		}
	}
	return _T("");
}
void CModbusPollView::DoDataExchange(CDataExchange* pDX)
{//IDC_RICHEDIT_MOD_LIB
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_MsDataGrid);
	DDX_Control(pDX, IDC_CONNECTION_STATE, m_connectionState);
	DDX_Control(pDX, IDC_STATIC_TX_RX, m_Tx_Rx);
	 
	DDX_Control(pDX, IDC_MODELNAME, m_ModelNameRichEdit);
	DDX_Control(pDX, IDC_SLAVEID, m_SlaveIDRichEditCtrl);
	DDX_Control(pDX, IDC_RICHEDIT_MOD_LIB, m_modelib);
}

BOOL CModbusPollView::PreCreateWindow(CREATESTRUCT& cs)
{
	//  Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}
BEGIN_EVENTSINK_MAP(CModbusPollView, CFormView)
 
END_EVENTSINK_MAP()
void CModbusPollView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	COLORREF cf=RGB(212,208,200);
	m_connectionState.SetReadOnly(TRUE);
	m_connectionState.SetBackgroundColor(FALSE,cf);
	m_Tx_Rx.SetReadOnly(TRUE);
	m_Tx_Rx.SetBackgroundColor(FALSE,cf);
			 
	m_ModelNameRichEdit.SetReadOnly(TRUE);
	m_ModelNameRichEdit.SetBackgroundColor(FALSE,cf);

	m_SlaveIDRichEditCtrl.SetReadOnly(TRUE);
	m_SlaveIDRichEditCtrl.SetBackgroundColor(FALSE,cf);
	m_modelib.SetReadOnly(TRUE);
	m_modelib.SetBackgroundColor(FALSE,cf);
	ResizeParentToFit();

	CRect ViewRect;
	GetClientRect(&ViewRect);
	
	if (m_MsDataGrid.GetSafeHwnd())
	{
		m_MsDataGrid.MoveWindow(CRect(0,60,ViewRect.Width(),ViewRect.Height()),TRUE);
	}

	m_MsDataGrid.EnableDragAndDrop(TRUE);
	m_MsDataGrid.SetAutoSizeStyle();
	//m_MsDataGrid.SetEditable(FALSE);

	for (int i=0;i<127;i++)
	{
		m_DataBuffer[i]=0;
	}

	CString strTemp;
	if (m_Function==0)
	{
		m_MBPoll_Function=3;
	} 
	else if (m_Function==1)
	{
		m_MBPoll_Function=6;
	}
	else if (m_Function==2)
	{
		m_MBPoll_Function=16;
	}
	else
	{
		m_MBPoll_Function=3;
	}
	CString Stemp;
	//Stemp.Format(_T("Tx=%ld  Err=%d  ID=%d  F=%2d  SR=%d ms"),m_Tx,m_Err,m_Slave_ID,m_MBPoll_Function,m_Scan_Rate);
	Stemp.Format(_T("Tx=%ld:  "),m_Tx);
	strTemp+=Stemp;
	Stemp.Format(_T("Err=%d:  "),m_Err);
	strTemp+=Stemp;
	Stemp.Format(_T("ID=%d:  "),m_Slave_ID);
	strTemp+=Stemp;
	Stemp.Format(_T("F=%02d:  "),m_MBPoll_Function);
	strTemp+=Stemp;
	Stemp.Format(_T("Tx=%dms  "),m_Scan_Rate);
	strTemp+=Stemp;

	m_Tx_Rx.SetWindowText(strTemp);
	m_Tx_Rx.SetNumberDefaultFontSize(12);



    Fresh_View();

	
	 

	CModbusPollDoc *pdoc=GetDocument();
	if (pdoc->m_Slave_ID!=-1)
	{m_Slave_ID=pdoc->m_Slave_ID;
	m_Function=pdoc->m_Function;
	m_address=pdoc->m_address;
	m_Quantity=pdoc->m_Quantity;
	m_Scan_Rate=pdoc->m_Scan_Rate;
	m_Rows=pdoc->m_Rows;
	m_Display=pdoc->m_Display;
	m_Hide_Alias_Columns=pdoc->m_Hide_Alias_Columns;
	m_Address_Cell=pdoc->m_Address_Cell;
	m_PLC_Addresses=pdoc->m_PLC_Addresses;

	//////////////////////////////////////////////////////////////////////
	m_MBPoll_Function=pdoc->m_MBPoll_Function;
	//////////////////////////////////////////////////////////////////////
	m_data_rows=pdoc->m_data_rows;
	m_data_cols=pdoc->m_data_cols;
	m_grid_rows=pdoc->m_grid_rows;
	m_grid_cols=pdoc->m_grid_cols;
	m_Current_Row=pdoc->m_Current_Row;
	m_Current_Col=pdoc->m_Current_Col;
	m_close_dlg=pdoc->m_close_dlg;
	m_function=pdoc->m_function;
	/////////////////////////DataBuffer/////////////////////////////////////////
	for (int i=0;i<127;i++)
	{
		m_Alias[i]=pdoc->m_Alias[i];
		m_DataBuffer[i]=pdoc->m_DataBuffer[i];
	}
	}
	else
	{

		CChildFrame *pChildFrame=(CChildFrame *)GetParentFrame();

		pChildFrame->m_Slave_ID=m_Slave_ID;
		pChildFrame->m_Function=m_Function;
		pChildFrame->m_Quantity=m_Quantity;
		pChildFrame->m_Scan_Rate=m_Scan_Rate;
		pChildFrame->m_Rows=m_Rows;
		pChildFrame->m_Display=m_Display;
		pChildFrame->m_Hide_Alias_Columns=m_Hide_Alias_Columns;
		pChildFrame->m_Address_Cell=m_Address_Cell;
		pChildFrame->m_PLC_Adresses=m_PLC_Addresses;
		pChildFrame->m_address=m_address;
		pChildFrame=NULL;
	}
	
// 	m_connectionState.SetReadOnly(TRUE);
// 	m_Tx_Rx.SetReadOnly(TRUE);
//     m_ModelNameRichEdit.SetReadOnly(TRUE);
// 	m_SlaveIDRichEditCtrl.SetReadOnly(TRUE);

// 	if(m_MultiRead_handle != NULL)
// 		TerminateThread(m_MultiRead_handle, 0);
// 	m_MultiRead_handle=NULL;
// 	if (!m_MultiRead_handle)
// 	{
// 	 	m_MultiRead_handle = CreateThread(NULL,NULL,_Multi_Read_Fun03,this,NULL,0);
// 	}
}

void CModbusPollView::Fresh_View(){
	                Fresh_Data();
}
void CModbusPollView::Fresh_Data(){
	 /*
		-1:no connection
		-2:create write error
		-3:create read error
		-4:time out error
		-5:crc error
		*/
		//SetPaneString(1,g_StrConnection);
	if (!g_online)
	{
		m_connectionState.SetWindowText(L"NO CONNECTION");
		m_connectionState.SetStringFontSize(12);
		COLORREF cf=RGB(255,0,0);
		m_connectionState.SetStringColor(cf);
		m_cur_modelName = L"";
		//return;
	} 
	else
	{
		if (m_MultiReadReturnType==-1)
		{
			m_connectionState.SetWindowText(L"NO CONNECTION");
			m_connectionState.SetStringFontSize(12);
			COLORREF cf=RGB(255,0,0);
			m_connectionState.SetStringColor(cf);
			m_cur_modelName = L"";
			//return;
		}
		else if (m_MultiReadReturnType==-2)
		{
			m_connectionState.SetWindowText(L"Write Error");
			m_connectionState.SetStringFontSize(12);
			COLORREF cf=RGB(255,0,0);
			m_connectionState.SetStringColor(cf);
			m_cur_modelName = L"";
			//return;
		}
		else if (m_MultiReadReturnType==-3)
		{
			m_connectionState.SetWindowText(L"Read Error");
			m_connectionState.SetStringFontSize(12);
			COLORREF cf=RGB(255,0,0);
			m_connectionState.SetStringColor(cf);
			m_cur_modelName = L"";
			//return;
		}
		else if (m_MultiReadReturnType==-4)
		{
			m_connectionState.SetWindowText(L"Timeout Error");
			m_connectionState.SetStringFontSize(12);
			COLORREF cf=RGB(255,0,0);
			m_connectionState.SetStringColor(cf);
			m_cur_modelName = L"";
			//return;
		}
		else if (m_MultiReadReturnType==-5)
		{
			m_connectionState.SetWindowText(L"CRC Error");
			m_connectionState.SetStringFontSize(12);
			COLORREF cf=RGB(255,0,0);
			m_connectionState.SetStringColor(cf);
			m_cur_modelName = L"";
			//return;
		}
		else
		{
              m_connectionState.SetWindowText(L"Connected");
			  m_connectionState.SetStringFontSize(12);
			  COLORREF cf=RGB(100,100,0);
			  m_connectionState.SetStringColor(cf);
			  //SetPaneString(1,L"Connected");
		}
		
// 		m_connectionState.SetStringFontSize(14);
// 		COLORREF cf=RGB(0,0,0);
// 		m_connectionState.SetStringColor(cf);
	}
	

	//m_Tx_Rx.SetWindowText(strTemp);

	CString strTemp;
	if (m_Function==0)
	{
		m_MBPoll_Function=1;
	} 
	else if (m_Function==1)
	{
		m_MBPoll_Function=2;
	}
	else if (m_Function==2)
	{
		m_MBPoll_Function=3;
	}
	else if (m_Function == 3)
	{
		m_MBPoll_Function = 4;
	}
	else
	{
		m_MBPoll_Function=3;
	}
	CString Stemp;
	//Stemp.Format(_T("Tx=%ld  Err=%d  ID=%d  F=%2d  SR=%d ms"),m_Tx,m_Err,m_Slave_ID,m_MBPoll_Function,m_Scan_Rate);
	Stemp.Format(_T("Tx=%ld:  "),m_Tx);
	strTemp+=Stemp;
	Stemp.Format(_T("Err=%d:  "),m_Err);
	strTemp+=Stemp;
	Stemp.Format(_T("ID=%d:  "),m_Slave_ID);
	strTemp+=Stemp;
	Stemp.Format(_T("F=%02d:  "),m_MBPoll_Function);
	strTemp+=Stemp;
	Stemp.Format(_T("Tx=%dms  "),m_Scan_Rate);
	strTemp+=Stemp;

	m_Tx_Rx.SetWindowText(strTemp);
	m_Tx_Rx.SetStringFontSize(15);
	CString showmodelname;
 	if (m_isgetmodel)
 	{
		
		//Model Name:
		showmodelname=_T("Model Name:");
		m_modelib.SetWindowText(showmodelname);
		m_modelib.SetStringFontSize(13);
		CString showslaveid;
		showslaveid.Format(_T("ID:%d"),m_modeldata[0]);
		m_SlaveIDRichEditCtrl.SetWindowText(showslaveid);
		m_SlaveIDRichEditCtrl.SetStringFontSize(13);
	}
	 
	if (m_cur_modelNo!=m_modeldata[0])
	{
	  m_ischangedAddress=TRUE;
	}
	 //Initial_RegName();
	COLORREF  cf=RGB(255,255,255);
/*	m_modelname=GetProductName(m_modeldata[1]);*/
	/*if (m_modelname.IsEmpty())
	{

		m_ModelNameRichEdit.SetReadOnly(FALSE);
		m_ModelNameRichEdit.SetBackgroundColor(FALSE,cf);
	}
	else
	{*/
	   if (m_ischangeModelName)
	   {
		   m_ModelNameRichEdit.SetReadOnly(FALSE);
		   m_ModelNameRichEdit.SetBackgroundColor(FALSE,cf);
	   } 
	   else
	   {
		   

		   cf=RGB(212,208,200);
		   m_ModelNameRichEdit.SetReadOnly(TRUE);
		   m_ModelNameRichEdit.SetBackgroundColor(FALSE,cf);
		   showmodelname.Format(_T("%s"),m_cur_modelName.GetBuffer());
		   m_ModelNameRichEdit.SetWindowText(showmodelname);
		   m_ModelNameRichEdit.SetStringFontSize(13);
	   }
	    
	//}
	

	if (m_Rows==0)
	{
		m_data_rows=10;
	}
	else if (m_Rows==1)
	{
		m_data_rows=20;
	}
	else if (m_Rows==2)
	{
		m_data_rows=50;
	}
	else if (m_Rows==3)
	{
		m_data_rows=100;
	}
	else 
	{
		m_data_rows=m_Quantity;
	}
	m_data_cols=m_Quantity/m_data_rows;
	if (m_Quantity%m_data_rows!=0)
	{
		m_data_cols+=1;
	} 
	/*
	  m_Grid_Type_1:
	  m_Hide_Alias_Columns=0;
	  m_Address_Cell=0;
	  m_PLC_Addresses=0;

	  m_Grid_Type_2:
	  m_Hide_Alias_Columns=0;
	  m_Address_Cell=0;
	  m_PLC_Addresses=1;

	  m_Grid_Type_3:
	  m_Hide_Alias_Columns=0;
	  m_Address_Cell=1;
	  m_PLC_Addresses=0;


	  m_Grid_Type_4:
	  m_Hide_Alias_Columns=0;
	  m_Address_Cell=1;
	  m_PLC_Addresses=1;

	  m_Grid_Type_3:
	  m_Hide_Alias_Columns=1;
	  m_Address_Cell=0;
	  m_PLC_Addresses=0;

	  m_Grid_Type_3:
	  m_Hide_Alias_Columns=1;
	  m_Address_Cell=0;
	  m_PLC_Addresses=1;

	  m_Grid_Type_3:
	  m_Hide_Alias_Columns=1;
	  m_Address_Cell=1;
	  m_PLC_Addresses=0;


	  m_Grid_Type_3:
	  m_Hide_Alias_Columns=1;
	  m_Address_Cell=1;
	  m_PLC_Addresses=1;
	  
	*/
    /*
	如果 m_Hide_Alias=0;
	m_grid_cols=2*m_data_cols+1;
	m_grid_rows=m_data_rows+1;

	如果 m_Hide_Alias=1;
	m_grid_cols=m_data_cols+1;
	m_grid_rows=m_data_rows+1;
	*/

	if (m_Hide_Alias_Columns!=0)
	{
	m_grid_cols=2*m_data_cols+1;
	m_grid_rows=m_data_rows+1;
	} 
	else
	{
	m_grid_cols=3*m_data_cols+1;
	m_grid_rows=m_data_rows+1;
	}
	 
	TRY 
	{
		m_MsDataGrid.SetRowCount(m_grid_rows);
	    m_MsDataGrid.SetColumnCount(m_grid_cols);
		m_MsDataGrid.SetFixedRowCount(1);
		m_MsDataGrid.SetFixedColumnCount(1);
	}
	CATCH (CMemoryException, e)
	{
		e->ReportError();
		return;
	}
	END_CATCH

	CString index;
	CString DataTemp;
	int Index;
	int Index_start,Index_end,row;
	//写下标
	if (m_PLC_Addresses==1)
	{
		Index_start=1;
		Index_end=m_MsDataGrid.GetRowCount();

	} 
	else
	{
		Index_start=0;
		Index_end=m_MsDataGrid.GetRowCount()-1;
	}
	row=1;
	for (int i=Index_start;i<Index_end;i++)
	{

		index.Format(_T("%d"),i);
		m_MsDataGrid.SetItemText(row, 0, index);
		row++;
	}
	//把索引隐藏掉
 
	//m_MsDataGrid.SetColumnWidth(0, 0);
	//这里主要是不要显示index行，如果要显示的话，就要把这行屏蔽掉，就可以了
	//Modbus Poll的
	if (m_Hide_Alias_Columns!=0)
	{
		 //初始化第0行
		for (int i=1;i<m_MsDataGrid.GetColumnCount();i++)
		{
			index.Format(_T("%d"),m_address+(i-1)*(m_MsDataGrid.GetRowCount()-1));
		 
			m_MsDataGrid.SetItemText(0, i, index);
		}
		
 			for (int i=1;i<m_MsDataGrid.GetColumnCount();i++)
 			{
 				for (int j=0;j<m_MsDataGrid.GetRowCount();j++)
 				{
 
 					if (j==0)
 					{
 						//初始化第0行
 						if (i%2==1)
 						{
 							index=L"Address";
 							m_MsDataGrid.SetItemText(0,i,index);
							m_MsDataGrid.SetItemFgColour(0, i, RGB(139, 126, 102));
 						} 
 						else
 						{
 							index=L"Value";
 							m_MsDataGrid.SetItemText(0,i,index);
							m_MsDataGrid.SetItemFgColour(0, i, RGB(0, 0, 0));
 						}
 					} 
 					else
 					{
					    if (i%2==0)
					    {
						Index=(i/2-1)*(m_MsDataGrid.GetRowCount()-1)+(j-1);
					    } 
					    else
					    {
						Index=(i/2)*(m_MsDataGrid.GetRowCount()-1)+(j-1);
					    }
  						
  						if (i%2==1)//Add
  						{
  							if (Index<=m_Quantity)
  							{
  								CString StrTemp;
  								StrTemp.Format(_T("%d"),Get_Reg_Add(Index));
  								m_MsDataGrid.SetItemText(j,i,StrTemp);
								//m_MsDataGrid.SetTextColor(RGB(70, 130, 180));
								m_MsDataGrid.SetItemFgColour(j, i, RGB(139, 126, 102));
  								
							}
  						}
  						else//Value
  						{
  							if (Index<=m_Quantity)
  							{
  								m_MsDataGrid.SetItemText(j,i,Get_Data(Index));
								m_MsDataGrid.SetItemFgColour(j, i, RGB(0, 0, 0));
  							}
  						}
 					}
 				}
 			}
	}
	else{
		//初始化行
		for (int i=1;i<m_MsDataGrid.GetColumnCount();i++)
		{

			
			 
			for (int j=0;j<m_MsDataGrid.GetRowCount();j++)
			{
                 if (j==0)
                 {
					 //初始化第0行
					 if((i%3)==1)
					 {
						 index=L"Description";
						 m_MsDataGrid.SetItemText(0,i,index);
						 m_MsDataGrid.SetItemFgColour(0, i, RGB(70, 130, 180));
					 } 
					else if ((i%3)==2)
					 {
						 index=L"Address";
						 m_MsDataGrid.SetItemText(0,i,index);//	139 126 102
						 m_MsDataGrid.SetItemFgColour(0, i, RGB(139, 126, 102));
					 } 
					 else if ((i%3)==0)
					 {
						 index=L"Value";
						 m_MsDataGrid.SetItemText(0,i,index);
						 m_MsDataGrid.SetItemFgColour(0, i, RGB(0, 0, 0));
					 }
					 
                 } 
                 else
                 {
					 if ((i%3)==1)
					 {
						 Index=((i+2)/3-1)*(m_MsDataGrid.GetRowCount()-1)+(j-1);

						 m_MsDataGrid.SetItemText(j,i, m_Alias[Index]);
						 m_MsDataGrid.SetItemFgColour(j, i, RGB(70, 130, 180));

					 }
					 else if ((i%3)==2)
					 {
						 Index=((i+1)/3-1)*(m_MsDataGrid.GetRowCount()-1)+(j-1);
						 if (Index<=m_Quantity)
						 {
							 CString StrTemp;
							 StrTemp.Format(_T("%d"),Get_Reg_Add(Index));
							 m_MsDataGrid.SetItemText(j,i,StrTemp);
							 m_MsDataGrid.SetItemFgColour(j, i, RGB(139, 126, 102));
							 
						 }
						 //m_MsDataGrid.SetItemState(j, i, GVIS_READONLY);
					 }
					 else if ((i%3)==0)
					 {
						 Index=(i/3-1)*(m_MsDataGrid.GetRowCount()-1)+(j-1);
						 if (Index<=m_Quantity)
						 {
							 m_MsDataGrid.SetItemText(j,i,Get_Data(Index));
							 m_MsDataGrid.SetItemFgColour(j, i, RGB(0, 0, 0));
						 }
					 }


				 }
			}
		}
	}

	m_MsDataGrid.AutoSize();
      
     if(g_Draw_dlg!=NULL)  //&& g_Draw_dlg->IsWindowVisible()
     {
        
         for (vector<Registers_Infor>::iterator it=g_vectRegisters.begin();it!=g_vectRegisters.end();)
         {
             RegPoint pt ;
             pt.Time_offset=g_Time_Offset;
             pt.Value_offset=(unsigned short )m_DataBuffer[it->address-m_address];
             it->Reg_Point.push_back(pt);
             ++it;
         }
         g_Time_Offset+=5;
         if (g_Time_Offset%10==0)
         {
            ::SendMessage(g_Draw_dlg->m_hWnd,MY_FRESH_DRAW_GRAPHIC,0,0);
            Sleep(200);   //响应绘图
         } 
       
     }
     else
     {
        
     }
    
/////////////////记录数据 Txt 
	if (!m_logText)
	{
		if (!m_logTextPath.IsEmpty())
		{
			CString Data;
			 
				CString TxtString=Get_Now();
				Data.Format(_T("<%04d>"),m_address);
				TxtString+=Data;
				TxtString+=_T("    ");
				for (int i=0;i<m_Quantity;i++)
				{

					Data.Format(_T("%d    "),m_DataBuffer[i]);
					TxtString+=Data;

				}
				TxtString+=_T("\n");
				m_default_file.WriteString(TxtString);
			 
	 	}
		
		
	}
	CString str;
	if (!m_logExcel)
	{
	   ++m_curexcelrow;

	   str=Get_Now();
	   m_rgMyRge.SetItem(_variant_t((long)m_curexcelrow),_variant_t((long)1),_variant_t(str));
	   for (int i=0;i<m_Quantity;i++)
	   {
		   str.Format(_T("%d"),m_DataBuffer[i]);
		   m_rgMyRge.SetItem(_variant_t((long)m_curexcelrow),_variant_t((long)i+2),_variant_t(str));
	   }

	}
	// 	CString str;
// 	str = _T("医疗机构许可证信息查询结果");
// 	rgMyRge.SetItem(_variant_t((long)1),_variant_t((long)2),_variant_t(str)); 
// 	str = _T("许可证编号");
// 	rgMyRge.SetItem(_variant_t((long)2),_variant_t((long)1),_variant_t(str)); 
// 	str = _T("机构名称");
// 	rgMyRge.SetItem(_variant_t((long)2),_variant_t((long)2),_variant_t(str)); 
// 	str = _T("经营性质");
// 	rgMyRge.SetItem(_variant_t((long)2),_variant_t((long)3),_variant_t(str));
// 	str = _T("经济性质");
// 	rgMyRge.SetItem(_variant_t((long)2),_variant_t((long)4),_variant_t(str));
// 	str = _T("负责人");
// 	rgMyRge.SetItem(_variant_t((long)2),_variant_t((long)5),_variant_t(str));
// 	str = _T("法人代表");
// 	rgMyRge.SetItem(_variant_t((long)2),_variant_t((long)6),_variant_t(str));
// 	str = _T("诊疗科目");
// 	rgMyRge.SetItem(_variant_t((long)2),_variant_t((long)7),_variant_t(str));
// 	str =_T( "地址");
// 	rgMyRge.SetItem(_variant_t((long)2),_variant_t((long)8),_variant_t(str));
// 	str = _T("电话");
// 	rgMyRge.SetItem(_variant_t((long)2),_variant_t((long)8),_variant_t(str));
// 	str = _T("发证日期");
// 	rgMyRge.SetItem(_variant_t((long)2),_variant_t((long)9),_variant_t(str));
// 	str = _T("有效期截止");
// 	rgMyRge.SetItem(_variant_t((long)2),_variant_t((long)10),_variant_t(str));
// 	
// 	
// 	
// 	//ExcelApp.SetVisible(true); 
// 	
// 	
// 	 
// 	rgMyRge.AttachDispatch(wsMysheet.GetCells());
// 
// 
// 	for (int i = 0; i < 200; i++)
// 	{
// 
// 
//         rgMyRge1.AttachDispatch(rgMyRge.GetItem(COleVariant((long)3+i), COleVariant((long)3+i)).pdispVal, TRUE);
//         rgMyRge1.AttachDispatch(rgMyRge1.GetEntireRow(), TRUE);
// 
//         rgMyRge2.AttachDispatch(rgMyRge.GetItem(COleVariant((long)2+i), COleVariant((long)1+i)).pdispVal, TRUE);
//         rgMyRge2.AttachDispatch(rgMyRge2.GetEntireRow(), TRUE);
// 
//         rgMyRge2.Copy(covOptional);
//         rgMyRge1.Insert(COleVariant((long)3+i));
// 
// 		Sleep(2000);
// 		/*rgMyRge2.Copy(covOptional);
// 		rgMyRge1.Insert(COleVariant(1L));*/
// 	}
	//for (int i = 0; i < 5; i++)
	//{
	//	rgMyRge1.Insert(COleVariant(1L));
	//}
	
	//wbMyBook.PrintPreview(_variant_t(false)); 
	//释放对象 
	
	 
}
CString CModbusPollView::Get_Now(){
	 
		SYSTEMTIME st;
		CString strDate,strTime;
		GetLocalTime(&st);
		strDate.Format(_T("%4d-%2d-%2d"),st.wYear,st.wMonth,st.wDay);
		strTime.Format(_T("%2d:%2d:%2d:%4d"), st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
		strDate+=_T(" ");
		strDate+=strTime;
		return strDate;
}
CString CModbusPollView::Get_Data(int index){
	static CString tempStr;
	short int tempVal1;
	unsigned short int tempVal2;
	unsigned short int tempVal3;
	unsigned short int tempVal4;
	unsigned short int bitPosition = 0x8000;
	CString strToReturn;
	short int val=(short int )m_DataBuffer[index];
	switch (m_Display)
	{
	case 0:		/* Signed */
		tempVal1 = (short int)val;
		strToReturn.Format(_T("%06d"), tempVal1);
		if (m_Address_Cell==0){
           strToReturn.Format(_T("%d"), tempVal1);
		}
		break;
	case 1:		/* Unsigned */
		tempVal2 = (unsigned short int)val;
		strToReturn.Format(_T("%06d"), tempVal2);
		if (m_Address_Cell==0){
			strToReturn.Format(_T("%d"), tempVal2);
		}
		break;
	case 2:		/* Hex */
		tempVal3 = (unsigned short int)val;
		strToReturn.Format(_T("0x%04X"), tempVal3);
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
			if ((i+1)%4==0)
			{
				tempStr+=L" ";
			}
		}
		strToReturn = tempStr;
		break;

	default: 
		tempVal2 = (unsigned short int)val;
		strToReturn.Format(_T("%06d"), tempVal2);
		break;
	}

	CString ret;
	
	if (m_Address_Cell!=0)
	{
		ret.Format(_T("%d=%s"),Get_Reg_Add(index),strToReturn.GetBuffer());
	} 
	else
	{
		ret.Format(_T("%s"),strToReturn.GetBuffer());
	}
	
	return ret;
}
CString CModbusPollView::Get_Data_No_Address(int index){
	static CString tempStr;
	short int tempVal1;
	unsigned short int tempVal2;
	unsigned short int tempVal3;
	unsigned short int tempVal4;
	unsigned short int bitPosition = 0x8000;
	CString strToReturn;
	short int val=(short int )m_DataBuffer[index];
	switch (m_Display)
	{
	case 0:		/* Signed */
		tempVal1 = (short int)val;
		strToReturn.Format(_T("%06d"), tempVal1);
		if (m_Address_Cell==0){
			strToReturn.Format(_T("%d"), tempVal1);
		}
		break;
	case 1:		/* Unsigned */
		tempVal2 = (unsigned short int)val;
		strToReturn.Format(_T("%06d"), tempVal2);
		if (m_Address_Cell==0){
			strToReturn.Format(_T("%d"), tempVal2);
		}
		break;
	case 2:		/* Hex */
		tempVal3 = (unsigned short int)val;
		strToReturn.Format(_T("0x%04X"), tempVal3);
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
			if ((i+1)%4==0)
			{
				tempStr+=L" ";
			}
		}
		strToReturn = tempStr;
		break;
	default: 
		tempVal2 = (unsigned short int)val;
		strToReturn.Format(_T("%06d"), tempVal2);
		break;
	}

	CString ret;

	//if (m_Address_Cell!=0)
	//{
	//	ret.Format(_T("%d=%s"),Get_Reg_Add(index),strToReturn.GetBuffer());
	//} 
	//else
	//{
		ret.Format(_T("%s"),strToReturn.GetBuffer());
	//}

	return ret;
}
int CModbusPollView::Get_Reg_Add(int index){
	if (m_PLC_Addresses!=0)
	{
		return 40000+index+m_address;
	} 
	else
	{
		return index+m_address;
	}
}
void CModbusPollView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CModbusPollView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	if (!Show_Name)
	{
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
	}
	 
#endif
}

#ifdef _DEBUG
void CModbusPollView::AssertValid() const
{
	CFormView::AssertValid();
}

void CModbusPollView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CModbusPollDoc* CModbusPollView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CModbusPollDoc)));


	return (CModbusPollDoc*)m_pDocument;
}
#endif //_DEBUG


// CModbusPollView message handlers


void CModbusPollView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	 if (nType==SIZE_RESTORED)
	 {

		 CRect ViewRect;
		 GetClientRect(&ViewRect);
		 //TRACE(_T(" View:T=%d,B=%d,L=%d,R=%d\n"),ViewRect.top,ViewRect.bottom,ViewRect.left,ViewRect.right);
		// m_MsDataGrid.SetWindowPos(this,ViewRect.top,ViewRect.left,ViewRect.Width(),ViewRect.Height(),SWP_SHOWWINDOW|SWP_NOZORDER);
		 if (m_MsDataGrid.GetSafeHwnd())
		 {
		     m_MsDataGrid.MoveWindow(CRect(0,60,ViewRect.Width(),ViewRect.Height()),TRUE);
		 }
		
	 }
}

//volatile HANDLE Read_Mutex=NULL;
//DWORD WINAPI _Multi_Read_Fun03(LPVOID pParam){
//	 
//		CModbusPollView* pMBPollView=(CModbusPollView*)(pParam);
//		int ID;
// 		unsigned short DataBuffer[127];
//		unsigned short startAdd;
//		unsigned short quantity;
//		int sleep;
//
//		int Send_length;
//		int Rev_length;
//
//		 CString temp;
//
//		unsigned char rev_back_rawData[300],send_data[100];
//		 
//		//read_multi_log(m_ID,&rev_back_Data[0],m_startid,m_length,&send_data[0],&rev_back_rawData[0]);
//		Read_Mutex=CreateMutex(NULL,TRUE,_T("Read_Multi_Reg"));	//Add by Fance 
//		ReleaseMutex(Read_Mutex);//Add by Fance 
//
//		while(TRUE)
//		{
//			if (pMBPollView->m_hWnd==NULL)
//			{
//				Sleep(1000);
//				continue;
//			}
//			
//
//			if (!g_online)
//			{
//				Sleep(1000);
//				if (pMBPollView->m_hWnd!=NULL)
//				{
//					::PostMessage(pMBPollView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
//				}
//				continue;
//			}
//			//DataBuffer=pMBPollView->m_DataBuffer;
//			ID=pMBPollView->m_Slave_ID;
//			startAdd=pMBPollView->m_address;
//			quantity=pMBPollView->m_Quantity;
//			sleep=pMBPollView->m_Scan_Rate;
//
//			register_critical_section.Lock();
//			//read_multi_log(ID,&pMBPollView->m_DataBuffer[0],startAdd,quantity,&send_data[0],&rev_back_rawData[0],&Send_length,&Rev_length);
//			//read_multi_tap(ID,&DataBuffer[0],startAdd,quantity);
//			CString m_Tx,temp,m_Rx;
//			int ret=read_multi_log(ID,&DataBuffer[0],startAdd,quantity,&send_data[0],&rev_back_rawData[0],&Send_length,&Rev_length);
//
//			++g_Tx_Rx;
//			temp.Format(_T("%06d--"),g_Tx_Rx);
//			m_Tx+=temp;
//
//			for (int i=0;i<Send_length;i++)
//			{
//				temp.Format(_T("%02X "),send_data[i]);
//				m_Tx+=temp;
//			}
//			Traffic_Data(m_Tx);
//			
//			 
//			++g_Tx_Rx;
//			temp.Format(_T("%06d--"),g_Tx_Rx);
//			m_Rx+=temp;
//
//			for(int i=0;i<Rev_length;i++){
//				temp.Format(_T("%02X "),rev_back_rawData[i]);
//				m_Rx+=temp;
//			}
//			 
//			Traffic_Data(m_Rx);
//
//
//			if (ret>0)//读的正确之后，我们才把值传给view显示
//			{
//				memcpy_s(pMBPollView->m_DataBuffer,sizeof(pMBPollView->m_DataBuffer),DataBuffer,sizeof(DataBuffer));
//				++pMBPollView->m_Tx;
//			} 
//			else
//			{
//				++pMBPollView->m_Tx;
//				++pMBPollView->m_Err;
//			}
//			Sleep(sleep);
//			register_critical_section.Unlock();
//			if (pMBPollView->m_hWnd!=NULL)
//			{
//				::PostMessage(pMBPollView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
//			}
//
//		}
//}


LRESULT CModbusPollView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{


	if(MY_FRESH_MBPOLLVIEW==message){
		Fresh_Data();
		TRACE("Fresh_Data\n");
	}
	return CFormView::WindowProc(message, wParam, lParam);
}


 

 DWORD WINAPI _Multi_Read_Fun03(LPVOID pParam){
	CModbusPollView* pMBPollView=(CModbusPollView*)(pParam);
	int ID;
	unsigned short DataBuffer[127];
	unsigned short startAdd;
	unsigned short quantity;
	int sleep;

	int Send_length;
	int Rev_length;

	CString temp;

	unsigned char rev_back_rawData[300],send_data[100];


	while(TRUE){

	if (pMBPollView->m_hWnd==NULL)
	{
		Sleep(1000);
		 
	}


	if (!g_online)
	{
		Sleep(1000);
		if (pMBPollView->m_hWnd!=NULL)
		{
			::SendMessage(pMBPollView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
		}
		  continue;
	}
	//DataBuffer=pMBPollView->m_DataBuffer;
	ID=pMBPollView->m_Slave_ID;
	startAdd=pMBPollView->m_address;
	quantity=pMBPollView->m_Quantity;
	sleep=pMBPollView->m_Scan_Rate;



 
	CString m_Tx,m_Rx;
	int ret=0;
	register_critical_section.Lock();
	if (pMBPollView->m_PLC_Addresses==1)
	{
		ret=read_multi_log(ID,&DataBuffer[0],startAdd-1,quantity,&send_data[0],&rev_back_rawData[0],&Send_length,&Rev_length);
	}
	else
	{
		ret=read_multi_log(ID,&DataBuffer[0],startAdd,quantity,&send_data[0],&rev_back_rawData[0],&Send_length,&Rev_length);
	}
	register_critical_section.Unlock();
	++g_Tx_Rx;
	temp.Format(_T("%06d--"),g_Tx_Rx);
	m_Tx+=temp;
	for (int i=0;i<Send_length;i++)
	{
		temp.Format(_T("%02X "),send_data[i]);
		m_Tx+=temp;
	}
	Traffic_Data(m_Tx);
	++g_Tx_Rx;
	temp.Format(_T("%06d--"),g_Tx_Rx);
	m_Rx+=temp;
	for(int i=0;i<Rev_length;i++){
		temp.Format(_T("%02X "),rev_back_rawData[i]);
		m_Rx+=temp;
	}

	Traffic_Data(m_Rx);

	/*if (!pMBPollView->m_isgetmodel)
	{*/
		unsigned char rev_back_rawData[300],send_data[100];
		int tt=read_multi_log(ID,&pMBPollView->m_modeldata[0],6,2,&send_data[0],&rev_back_rawData[0],&Send_length,&Rev_length);
		
		
		if (tt>0)
		{
			pMBPollView->m_isgetmodel=TRUE;
		}
		else
		{
			pMBPollView->m_isgetmodel=FALSE;
		}
	/*}*/

	if (ret>0)//读的正确之后，我们才把值传给view显示
	{
		memcpy_s(pMBPollView->m_DataBuffer,sizeof(pMBPollView->m_DataBuffer),DataBuffer,sizeof(DataBuffer));
		++pMBPollView->m_Tx;
	} 
	else
	{
		++pMBPollView->m_Tx;
		++pMBPollView->m_Err;
	}
	

	if (pMBPollView->m_hWnd!=NULL)
	{
		::SendMessage(pMBPollView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
	}
	Sleep(sleep);
	}

	pMBPollView=NULL;
return 1;
}

BOOL CModbusPollView::DestroyWindow()
{
	
	 	
	return CFormView::DestroyWindow();
}

void CModbusPollView::OnEditAdd()
{
	int Index;
	if(m_Hide_Alias_Columns!=0)//都是数值的
	{
		 
        if (m_Current_Col%2!=0)
        {
            return;
        }
        else
        {

            Index=(m_Current_Col/2-1)*(m_MsDataGrid.GetRowCount()-1)+(m_Current_Row-1);
        }
	}
	else//含义名字的
	{
		if (m_Current_Col%3!=0)
		{
			    return;
		}
		else
		{
			 
			Index=(m_Current_Col/3-1)*(m_MsDataGrid.GetRowCount()-1)+(m_Current_Row-1);
		}

	 
	}
	 
	int address=Get_Reg_Add(Index);
	CString RegName;
	RegName.Format(_T("Reg:%d"),address);
	Registers_Infor temp_reginfor;
	temp_reginfor.Reg_Name=RegName;
	temp_reginfor.address=address;
	temp_reginfor.Point_Color=Get_Color(g_Color_Index);
	++g_Color_Index;
	g_vectRegisters.push_back(temp_reginfor);


}

COLORREF CModbusPollView::Get_Color(int i){
COLORREF Array[]={	    
	    
	RGB(0x8B, 0x00, 0x00),
	RGB(0xFF, 0x68, 0x20),
	RGB(0x8B, 0x8B, 0x00),
	RGB(0x00, 0x93, 0x00),
	RGB(0x38, 0x8E, 0x8E),
	RGB(0x00, 0x00, 0xFF),
	RGB(0x7B, 0x7B, 0xC0),
	RGB(0x66, 0x66, 0x66),


	    RGB(0x00, 0x00, 0x00),
		RGB(0xA5, 0x2A, 0x00),
		RGB(0x00, 0x40, 0x40),
		RGB(0x00, 0x55, 0x00),
		RGB(0x00, 0x00, 0x5E),
		RGB(0x00, 0x00, 0x8B),
		RGB(0x4B, 0x00, 0x82),
		RGB(0x28, 0x28, 0x28),

		

		RGB(0xFF, 0x00, 0x00),
		RGB(0xFF, 0xAD, 0x5B),
		RGB(0x32, 0xCD, 0x32),
		RGB(0x3C, 0xB3, 0x71),
		RGB(0x7F, 0xFF, 0xD4),
		RGB(0x7D, 0x9E, 0xC0),
		RGB(0x80, 0x00, 0x80),
		RGB(0x7F, 0x7F, 0x7F),
		
		RGB(0xFF, 0xC0, 0xCB),
		RGB(0xFF, 0xD7, 0x00),
		RGB(0xFF, 0xFF, 0x00),
		RGB(0x00, 0xFF, 0x00),
		RGB(0x40, 0xE0, 0xD0),
		RGB(0xC0, 0xFF, 0xFF),
		RGB(0x48, 0x00, 0x48),
		RGB(0xC0, 0xC0, 0xC0),

		RGB(0xFF, 0xE4, 0xE1),
		RGB(0xD2, 0xB4, 0x8C),
		RGB(0xFF, 0xFF, 0xE0),
		RGB(0x98, 0xFB, 0x98),
		RGB(0xAF, 0xEE, 0xEE),
		RGB(0x68, 0x83, 0x8B),
		RGB(0xE6, 0xE6, 0xFA),
		RGB(0xFF, 0xFF, 0xFF)
      };
return Array[i];
}
void CModbusPollView::OnSetupTxtLog()
{
	 m_logText=FALSE; 
     if (m_logTextPath.IsEmpty())
     {
		 CString strFilter;
		 strFilter =_T( "Text File|*.txt|All File|*.*|");
		 CFileDialog dlg(false,_T("txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
		 if (dlg.DoModal() != IDOK)
			 return;
		 m_logTextPath=dlg.GetPathName();
     }
	 if (!m_logTextPath.IsEmpty())
	 {
		 CString Data;
		 if(m_default_file.Open(m_logTextPath.GetBuffer(),CFile::modeCreate | CFile::modeWrite)!=0){
			 
		 }
		 else
		 {
			 AfxMessageBox(_T("Open file failure!"));
		 }
      
      }
}

void CModbusPollView::OnSetupTxtlogoutOff()
{
	 m_logText=TRUE;
	 m_default_file.Flush();
	 m_default_file.Close();
}

void CModbusPollView::OnUpdateSetupTxtlogoutOff(CCmdUI *pCmdUI)
{
	 pCmdUI->Enable(!m_logText);
}

void CModbusPollView::OnUpdateSetupTxtLog(CCmdUI *pCmdUI)
{
	 pCmdUI->Enable(m_logText);
}

void CModbusPollView::OnSetupExcellogingoff()
{
   
	 m_logExcel=TRUE;
	 m_rgMyRge.ReleaseDispatch(); 
	 m_wsMysheet.ReleaseDispatch(); 
	 m_wssMysheets.ReleaseDispatch(); 
	 m_wbMyBook.ReleaseDispatch(); 
	 m_wbsMyBooks.ReleaseDispatch(); 
	 m_ExcelApp.ReleaseDispatch(); 

}

void CModbusPollView::OnUpdateSetupExcellogingoff(CCmdUI *pCmdUI)
{
	 pCmdUI->Enable(!m_logExcel);
}


void CModbusPollView::OnSetupExcelloging()
{
 


   COleVariant covTrue((short)TRUE), covFalse((short)FALSE), covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	
	 
	//创建Excel 2000服务器(启动Excel) 
	if (!m_ExcelApp.CreateDispatch(_T("Excel.Application"),NULL)) 
	{ 
		AfxMessageBox(_T("Create Excel Fail!")); 
		exit(1); 
	} 
	m_ExcelApp.SetVisible(TRUE); 
	
	m_wbsMyBooks.AttachDispatch(m_ExcelApp.GetWorkbooks(),true); 
	
	m_wbMyBook.AttachDispatch(m_wbsMyBooks.Add(covOptional));
	//得到Worksheets 
	m_wssMysheets.AttachDispatch(m_wbMyBook.GetWorksheets(),true);
	//得到sheet1 
	m_wsMysheet.AttachDispatch(m_wssMysheets.GetItem(_variant_t("Sheet1")),true);
	//得到全部Cells，此时,rgMyRge是cells的集合 
	m_rgMyRge.AttachDispatch(m_wsMysheet.GetCells(),true); 
	m_rgMyRge.SetValue(_variant_t(""));
	m_curexcelrow=1;

	CString str;
	str=_T("Time");
	m_rgMyRge.SetItem(_variant_t((long)m_curexcelrow),_variant_t((long)m_curexcelrow),_variant_t(str));
	for (int i=0;i<m_Quantity;i++)
	{
	    str.Format(_T("%d"),m_address+i);
		m_rgMyRge.SetItem(_variant_t((long)m_curexcelrow),_variant_t((long)i+2),_variant_t(str));
	}
  	m_logExcel=FALSE;
}
//----
void CModbusPollView::OnUpdateSetupExcelloging(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_logExcel);
}
void CModbusPollView::Initial_RegName(){
	
	_variant_t temp_var;
	if (!m_isgetmodel)
	{
		m_VecregisterData.clear();
		// 		for (int i = 0; i < 127; i++)
		// 		{
		// 			m_Alias[i] = _T("");
		// 		}
		//memset(m_DataBuffer, 0, 127);
		return;
	}
	if ((!m_isgetmodel)&&(!m_ischangedAddress)&&(!m_ischangeModelName))
	{
		m_VecregisterData.clear();
// 		for (int i = 0; i < 127; i++)
// 		{
// 			m_Alias[i] = _T("");
// 		}
	//	memset(m_DataBuffer, 0, 127);
		return;
	}
	
	 
	 //	m_ischangeModelName=FALSE
	m_VecregisterData.clear();
	for (int i=0;i<127;i++)
	{
		m_Alias[i]=_T("");
	}
	CString TableName,RegName,RegAddress;
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3Query q;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
	if(SqliteDBT3000.tableExists("ProductsTypeRegisterTables"))
	{  
		CString sql,temp;
		sql.Format(_T("Select * from ProductsTypeRegisterTables where ProductType=%d"),m_modeldata[1]);
		m_cur_modelNo=m_modeldata[1];
		 q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);
		if (!q.eof())
		{
            
			TableName = q.getValuebyName(L"TableName");
			m_cur_TableName=TableName;
			 
			RegName =  q.getValuebyName(L"Col_RegName");
			m_cur_Col_RegName=RegName;
		 
			RegAddress = q.getValuebyName(L"Col_RegAddress");
			m_cur_col_RegAddress=RegAddress;
			 
			m_modelname = q.getValuebyName(L"ProductName");
			m_cur_modelName=m_modelname;
		}
	 
	}
	 
	if (TableName.GetLength()!=0)
	{
		if (SqliteDBT3000.tableExists((UTF8MBSTR)TableName))
		{
			DBRegister tempstuct;
			CString sql;
		    if (TableName.CompareNoCase(_T("CustomProductTable"))==0)
		    {
				sql.Format(_T("Select [%s],[%s] from %s where ModelNo=%d "),RegName.GetBuffer(),RegAddress.GetBuffer(),TableName.GetBuffer(),m_cur_modelNo);
				q=SqliteDBT3000.execQuery((UTF8MBSTR)sql);
				while (!q.eof())
				{

					tempstuct.RegAddress = q.getIntField((UTF8MBSTR)RegAddress);
					tempstuct.RegName = q.getValuebyName(RegName);
					m_VecregisterData.push_back(tempstuct);
					q.nextRow();
				}
		    } 
		    else
		    {
				sql.Format(_T("Select [%s],[%s] from %s "),RegName.GetBuffer(),RegAddress.GetBuffer(),TableName.GetBuffer());
				q=SqliteDBT3000.execQuery((UTF8MBSTR)sql);
				while (!q.eof())
				{

					 
					tempstuct.RegAddress = q.getIntField((UTF8MBSTR)RegAddress);
					 
					tempstuct.RegName = q.getValuebyName(RegName);
					m_VecregisterData.push_back(tempstuct);
					q.nextRow();
				}
		    }
		
			
		}


		if (m_VecregisterData.size()>=1)
		{
			int reglen=127;
			if (m_VecregisterData.size()<127)
			{
				reglen=m_VecregisterData.size();
			}
			for (int i=0;i<reglen;i++)
			{
				m_Alias[i]=Find_RegName(i);
			}
		}

		m_ischangedAddress=FALSE;
		m_isnewmodel=FALSE;
	}
 	else
 	{
 		m_isnewmodel=TRUE;
 		for (int i=0;i<127;i++)
 		{
 			m_Alias[i]=_T("");
 		}
 	}
	SqliteDBT3000.closedb();


	 
}

void CModbusPollView::OnEnKillfocusModelname()
{
    CString ModelName;
    GetDlgItem(IDC_MODELNAME)->GetWindowText(ModelName);
}
 

void CModbusPollView::OnEditChangemodelname()
{
	//m_ischangeModelName=TRUE;
}

 #include "TestCenter.h"
void CModbusPollView::OnFunctionsTestcenter()
{
    TestCenter dlg;
    dlg.DoModal (); 
}


// void CModbusPollView::MouseMoveMsflexgrid1(short Button, short Shift, long x, long y)
// {
// 	long lRow,lCol;
// 	lRow = m_MsDataGrid.get_RowSel();//获取点击的行号	
// 	lCol = m_MsDataGrid.get_ColSel(); //获取点击的列号
// 	TRACE(_T("lRow=%d,lCol=%d\n"),lRow,lCol);
// 
// 	CRect rect;
// 	m_MsDataGrid.GetWindowRect(rect); //获取表格控件的窗口矩形
// 	ScreenToClient(rect); //转换为客户区矩形	
// 	CDC* pDC =GetDC();
// 
// 	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
// 	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
// 	//计算选中格的左上角的坐标(象素为单位)
// //	long y = m_MsDataGrid.get_RowPos(lRow)/nTwipsPerDotY;
// 	//long x = m_MsDataGrid.get_ColPos(lCol)/nTwipsPerDotX;
// 	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
// 	//long width = m_MsDataGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
// //	long height = m_MsDataGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
// 	//形成选中个所在的矩形区域
// //	CRect rcCell(x,y,x+width,y+height);
// 	//转换成相对对话框的坐标
// //	rcCell.OffsetRect(rect.left+1,rect.top+1);
// //	ReleaseDC(pDC);
// 
// 	CString strValue = m_MsDataGrid.get_TextMatrix(lRow,lCol);
// }


void CModbusPollView::OnSetupUseasdefault()
{

	CString StrTemp;
	StrTemp.Format(_T("%d"),m_Slave_ID);
	WritePrivateProfileStringW(_T("MBPOLL_VIEW_SETTING"),_T("SLAVE_ID"),StrTemp,g_configfile_path);
	StrTemp.Format(_T("%d"),m_Function);
	WritePrivateProfileStringW(_T("MBPOLL_VIEW_SETTING"),_T("FUNCTION"),StrTemp,g_configfile_path);
	StrTemp.Format(_T("%d"),m_address);
	WritePrivateProfileStringW(_T("MBPOLL_VIEW_SETTING"),_T("ADDRESS"),StrTemp,g_configfile_path);
	StrTemp.Format(_T("%d"),m_Quantity);
	WritePrivateProfileStringW(_T("MBPOLL_VIEW_SETTING"),_T("QUANTITY"),StrTemp,g_configfile_path);
	StrTemp.Format(_T("%d"),m_Scan_Rate);
	WritePrivateProfileStringW(_T("MBPOLL_VIEW_SETTING"),_T("SCANRATE"),StrTemp,g_configfile_path);
	StrTemp.Format(_T("%d"),m_Rows);
	WritePrivateProfileStringW(_T("MBPOLL_VIEW_SETTING"),_T("ROWS"),StrTemp,g_configfile_path);
	StrTemp.Format(_T("%d"),m_Display);
	WritePrivateProfileStringW(_T("MBPOLL_VIEW_SETTING"),_T("DATA_TYPE"),StrTemp,g_configfile_path);
	StrTemp.Format(_T("%d"),m_Hide_Alias_Columns);
	WritePrivateProfileStringW(_T("MBPOLL_VIEW_SETTING"),_T("HIDEALIAS"),StrTemp,g_configfile_path);
	StrTemp.Format(_T("%d"),m_Address_Cell);
	WritePrivateProfileStringW(_T("MBPOLL_VIEW_SETTING"),_T("ADDRESSINCELL"),StrTemp,g_configfile_path);
	StrTemp.Format(_T("%d"),m_PLC_Addresses);
	WritePrivateProfileStringW(_T("MBPOLL_VIEW_SETTING"),_T("PLCBASE1"),StrTemp,g_configfile_path);
}


void CModbusPollView::OnFunctionsTestwrite()
{
	CWriteFunctionDlg dlg;
	dlg.m_device_id = m_Slave_ID;
	dlg.DoModal();
}




void CModbusPollView::OnEditCopy()
{
	m_MsDataGrid.OnEditCopy();
}


void CModbusPollView::OnFilePrint32857()
{
	m_MsDataGrid.Print();
}
void CModbusPollView::OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	long lRow, lCol;

	//Trace(_T("Double Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
	lRow=pItem->iRow; 
	lCol = pItem->iColumn;

	m_Current_Col = pItem->iColumn;
	m_Current_Row = pItem->iRow;

	CString strValue = m_MsDataGrid.GetItemText(lRow, lCol);

	

	int Index;
	UINT Address;
	if (m_Hide_Alias_Columns != 0)//都是数值的
	{
		//	Index=(lCol/3)*(m_MsDataGrid.get_Rows()-1)+(lRow-1);
		if (lCol % 2 == 0)
		{
			Index = (lCol / 2 - 1)*(m_MsDataGrid.GetRowCount() - 1) + (lRow - 1);
		}
		else
		{
			Index = (lCol / 2)*(m_MsDataGrid.GetRowCount() - 1) + (lRow - 1);
		}

		Show_Name = FALSE;
	}
	else//含义名字的
	{
		if (lCol % 3 == 1)
		{
			Show_Name = TRUE;
		}
		else
		{
			Show_Name = FALSE;


			if (lCol % 3 == 0)
			{
				Index = (lCol / 3 - 1)*(m_MsDataGrid.GetRowCount() - 1) + (lRow - 1);
			}
			else
			{
				Index = (lCol / 3)*(m_MsDataGrid.GetRowCount() - 1) + (lRow - 1);
			}


			//Index=(lCol/4)*(m_MsDataGrid.get_Rows()-1)+(lRow-1);
		}

		//Index=(lCol/2-1)*(m_MsDataGrid.get_Rows()-1)+(lRow-1);
	}
	if (Show_Name)
	{
// 		m_edit_name.ShowWindow(SW_SHOW);
// 		m_edit_name.SetWindowText(strValue);
// 		m_edit_name.MoveWindow(rcCell); //移动到选中格的位置，覆盖
// 		m_edit_name.BringWindowToTop();
// 		m_edit_name.SetFocus(); //获取焦点
	}
	else
	{
		Address = Get_Reg_Add(Index);
		CString Value = Get_Data_No_Address(Index);

		//CWriteSingleRegisterDlg(UINT Slave_ID=255,UINT Address=0,BOOL Close_Dlg=FALSE,BOOL Single_Function=FALSE,UINT DataFormat=0,int value=0,CWnd* pParent = NULL)
		if (m_Display < 3)
		{
			if (m_MBPoll_Function == 1)
			{
				CWriteSingleCoilDlg dlg(m_Slave_ID, Address, m_close_dlg, m_function, m_PLC_Addresses, _wtoi(Value),NULL);
			
				if (IDOK == dlg.DoModal())
				{
					m_close_dlg = dlg.m_close_dlg;
					m_function = dlg.m_function15;
				}
			}
			else
			{
				

				CWriteSingleRegisterDlg dlg(m_Slave_ID, Address, m_close_dlg, m_function, m_Display, m_PLC_Addresses, Value, NULL);
				if (IDOK == dlg.DoModal())
				{
					m_close_dlg = dlg.m_close_dlg;
					m_function = dlg.m_function;
				}
			}
			

		}
		else if (m_Display == 3)
		{

			CWriteSingle_BinaryDlg dlg;
			dlg.m_slave_id = m_Slave_ID;
			dlg.m_address = Address;
			dlg.m_close_dlg = m_close_dlg;
			dlg.m_function = m_function;
			dlg.m_data_format = m_Display;
			dlg.m_base_0 = m_PLC_Addresses;
			dlg.m_StrValue = Value;
			if (IDOK == dlg.DoModal())
			{
				m_close_dlg = dlg.m_close_dlg;
				m_function = dlg.m_function;
			}

		}
	}

}
void CModbusPollView::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	//Trace(_T("Clicked on row %d, col %d\n"), pItem->iRow, pItem->iColumn);
}
void CModbusPollView::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	CString Name = m_MsDataGrid.GetItemText(pItem->iRow, pItem->iColumn);
	int Index = ((pItem->iColumn + 2) / 3 - 1)*(m_MsDataGrid.GetRowCount() - 1) + (pItem->iRow - 1);
	//m_MsDataGrid.SetItemText(j, i, m_Alias[Index]); 

	m_Alias[Index] = Name;
	m_MsDataGrid.SetItemText(pItem->iRow, pItem->iColumn, Name);
	*pResult = 1;
}
BOOL CModbusPollView::PreTranslateMessage(MSG* pMsg)
{
	
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN
			|| pMsg->wParam == VK_ESCAPE)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;                    // DO NOT process further
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}



