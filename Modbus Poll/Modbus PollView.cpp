
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
#include "ado/ADO.h"



// #include "Application.h"
// #include "Workbooks.h"
// #include "Workbook.h"
// #include "Worksheets.h"
// #include "Worksheet.h"
// #include "Range.h"




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
ON_EN_KILLFOCUS(IDC_EDIT_NAME, &CModbusPollView::OnEnKillfocusEditName)
 
ON_COMMAND(ID_EDIT_ADD, &CModbusPollView::OnEditAdd)
ON_COMMAND(ID_SETUP_TXT_LOG, &CModbusPollView::OnSetupTxtLog)
ON_COMMAND(ID_SETUP_TXTLOGOUT_OFF, &CModbusPollView::OnSetupTxtlogoutOff)
ON_UPDATE_COMMAND_UI(ID_SETUP_TXTLOGOUT_OFF, &CModbusPollView::OnUpdateSetupTxtlogoutOff)
ON_UPDATE_COMMAND_UI(ID_SETUP_TXT_LOG, &CModbusPollView::OnUpdateSetupTxtLog)
ON_COMMAND(ID_SETUP_EXCELLOGINGOFF, &CModbusPollView::OnSetupExcellogingoff)
ON_UPDATE_COMMAND_UI(ID_SETUP_EXCELLOGINGOFF, &CModbusPollView::OnUpdateSetupExcellogingoff)
ON_COMMAND(ID_SETUP_EXCELLOGING, &CModbusPollView::OnSetupExcelloging)
ON_UPDATE_COMMAND_UI(ID_SETUP_EXCELLOGING, &CModbusPollView::OnUpdateSetupExcelloging)
END_MESSAGE_MAP()

 


CModbusPollView::CModbusPollView()
	: CFormView(CModbusPollView::IDD)
{
	  m_Slave_ID=255;
	  m_Function=0;
	  m_Quantity=10;
	  m_Scan_Rate=1000;
	  m_Rows=0;
	  m_Display=1;
	  m_Hide_Alias_Columns=0;
	  m_Address_Cell=0;
	  m_PLC_Addresses=0;

	  m_Err=0;
	  m_Tx=0;

	  m_address=0;
	  m_MBPoll_Function=3;

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
}

CModbusPollView::~CModbusPollView()
{
	 


// 	if(m_MultiRead_handle != NULL)
// 		TerminateThread(m_MultiRead_handle, 0);
// 	m_MultiRead_handle=NULL;
}
void CModbusPollView::Initial_RegName(){
	_variant_t temp_var;
	if (!m_isgetmodel)
	{
		return;
	}
	if (!m_ischangedAddress)
	{
		return;
	}
	CString TableName,RegName,RegAddress;
	CADO ado;
	ado.OnInitADOConn();
	if(ado.IsHaveTable(ado,_T("ProductsTypeRegisterTables")))
	{  
		CString sql,temp;
		sql.Format(_T("Select * from ProductsTypeRegisterTables where ProductType=%d"),m_modeldata[1]);
		ado.m_pRecordset=ado.OpenRecordset(sql);
		if (!ado.m_pRecordset->EndOfFile)
		{
			 
			temp_var=ado.m_pRecordset->GetCollect(_T("TableName"));
			if (temp_var.vt!=VT_NULL)
			{
				TableName=temp_var;
			}
			else
			{
                TableName=_T("");
			}
			temp_var=ado.m_pRecordset->GetCollect(_T("Col_RegName"));
			if (temp_var.vt!=VT_NULL)
			{
				RegName=temp_var;
			}
			else
			{
				RegName=_T("");
			}
			temp_var=ado.m_pRecordset->GetCollect(_T("Col_RegAddress"));
			if (temp_var.vt!=VT_NULL)
			{
				RegAddress=temp_var;
			}
			else
			{
				RegAddress=_T("");
			}
		}
		//while (!ado.m_pRecordset->EndOfFile)//有表但是没有对应序列号的值
		//{
		//	if (product_type==T3000_5ABCDFG_LED_ADDRESS)
		//	{
		//		tempstruct.Register_Address=ado.m_pRecordset->GetCollect(_T("Register_Address"));
		//		tempstruct.AddressName=ado.m_pRecordset->GetCollect(_T("TSTAT5_LED_AddressName"));
		//		tempstruct.DataType=ado.m_pRecordset->GetCollect(_T("TSTAT5_LED_DATATYPE"));
		//		tempstruct.length=ado.m_pRecordset->GetCollect(_T("TSTAT5_LED_LEN"));
		//		temp_var=ado.m_pRecordset->GetCollect(_T("TSTAT5_LED_DESCRIPTION"));
		//		if (temp_var.vt==VT_NULL)
		//		{
		//			tempstruct.Description=_T("");
		//		} 
		//		else
		//		{
		//			tempstruct.Description=temp_var;
		//		}
		//		temp_var=ado.m_pRecordset->GetCollect(_T("TSTAT5_LED_Operation"));
		//		if (temp_var.vt==VT_NULL)
		//		{
		//			tempstruct.Description=_T("");
		//		} 
		//		else
		//		{
		//			tempstruct.Operation=temp_var;
		//		}
		//		/*	tempstruct.Description=(CString)ado.m_pRecordset->GetCollect(_T("TSTAT5_LED_DESCRIPTION"));*/

		//	} 
		//	else if (product_type==T3000_5EH_LCD_ADDRESS)
		//	{
		//		_variant_t vartemp;
		//		tempstruct.Register_Address=ado.m_pRecordset->GetCollect(_T("Register_Address"));
		//		tempstruct.AddressName=ado.m_pRecordset->GetCollect(_T("TSTAT5_LCD_AddressName"));
		//		tempstruct.DataType=ado.m_pRecordset->GetCollect(_T("TSTAT5_LCD_DATATYPE"));
		//		tempstruct.length=ado.m_pRecordset->GetCollect(_T("TSTAT5_LCD_LEN"));
		//		//tempstruct.Description=(CString)ado.m_pRecordset->GetCollect(_T("TSTAT5_LCD_DESCRIPTION"));
		//		vartemp=ado.m_pRecordset->GetCollect(_T("TSTAT5_LCD_DESCRIPTION"));
		//		if (vartemp.vt==VT_NULL)
		//		{
		//			tempstruct.Description=_T("");
		//		} 
		//		else
		//		{
		//			tempstruct.Description=vartemp;
		//		} 
		//		temp_var=ado.m_pRecordset->GetCollect(_T("TSTAT5_LCD_Operation"));
		//		if (temp_var.vt==VT_NULL)
		//		{
		//			tempstruct.Description=_T("");
		//		} 
		//		else
		//		{
		//			tempstruct.Operation=temp_var;
		//		}
		//	}
		//	else if (product_type==T3000_6_ADDRESS)
		//	{
		//		tempstruct.Register_Address=ado.m_pRecordset->GetCollect(_T("Register_Address"));
		//		tempstruct.AddressName=ado.m_pRecordset->GetCollect(_T("TSTAT6_AddressName"));
		//		tempstruct.DataType=ado.m_pRecordset->GetCollect(_T("TSTAT6_DATATYPE"));
		//		tempstruct.length=ado.m_pRecordset->GetCollect(_T("TSTAT6_LEN"));
		//		temp_var=ado.m_pRecordset->GetCollect(_T("TSTAT6_DESCRIPTION")); 
		//		if (temp_var.vt==VT_NULL)
		//		{
		//			tempstruct.Description=_T("");
		//		} 
		//		else
		//		{
		//			tempstruct.Description=temp_var;
		//		}
		//		temp_var=ado.m_pRecordset->GetCollect(_T("TSTAT6_Operation"));
		//		if (temp_var.vt==VT_NULL)
		//		{
		//			tempstruct.Description=_T("");
		//		} 
		//		else
		//		{
		//			tempstruct.Operation=temp_var;
		//		}
		//	}
		//	if (tempstruct.AddressName.CompareNoCase(_T("RESERVED"))==0)
		//	{
		//		ado.m_pRecordset->MoveNext();
		//		continue;
		//	}
		//	m_VecregisterData.push_back(tempstruct);
		//	m_recordcount++;
		//	ado.m_pRecordset->MoveNext();
	}
	ado.CloseRecordset();
	if (TableName.GetLength()!=0)
	{
		if (ado.IsHaveTable(ado,TableName))
		{
			DBRegister tempstuct;
			CString sql;
			sql.Format(_T("Select [%s],[%s] from %s "),RegName.GetBuffer(),RegAddress.GetBuffer(),TableName.GetBuffer());
			ado.m_pRecordset=ado.OpenRecordset(sql);
			while (!ado.m_pRecordset->EndOfFile)
			{

				temp_var=ado.m_pRecordset->GetCollect(RegAddress.GetBuffer());
				if (temp_var.vt!=VT_NULL)
				{
					tempstuct.RegAddress=temp_var;
				}
				temp_var=ado.m_pRecordset->GetCollect(RegName.GetBuffer());

				if (temp_var.vt!=VT_NULL)
				{
					tempstuct.RegName=temp_var;
				}
				m_VecregisterData.push_back(tempstuct);
				ado.m_pRecordset->MoveNext();
			}
		}
		ado.CloseConn();

		if (m_VecregisterData.size()>1)
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
	}
	
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
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_MsDataGrid);
	DDX_Control(pDX, IDC_CONNECTION_STATE, m_connectionState);
	DDX_Control(pDX, IDC_STATIC_TX_RX, m_Tx_Rx);
	DDX_Control(pDX, IDC_EDIT_NAME, m_edit_name);
	DDX_Control(pDX, IDC_MODELNAME, m_ModelNameRichEdit);
	DDX_Control(pDX, IDC_SLAVEID, m_SlaveIDRichEditCtrl);
}

BOOL CModbusPollView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}
BEGIN_EVENTSINK_MAP(CModbusPollView, CFormView)
	ON_EVENT(CModbusPollView, IDC_MSFLEXGRID1, DISPID_DBLCLICK, CModbusPollView::DBClickMsflexgridDataGrid, VTS_NONE)
	ON_EVENT(CModbusPollView, IDC_MSFLEXGRID1, DISPID_CLICK, CModbusPollView::ClickMsflexgridDataGrid, VTS_NONE)
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
	 
	ResizeParentToFit();

	CRect ViewRect;
	GetClientRect(&ViewRect);
	//TRACE(_T(" View:T=%d,B=%d,L=%d,R=%d\n"),ViewRect.top,ViewRect.bottom,ViewRect.left,ViewRect.right);
	// m_MsDataGrid.SetWindowPos(this,ViewRect.top,ViewRect.left,ViewRect.Width(),ViewRect.Height(),SWP_SHOWWINDOW|SWP_NOZORDER);
	if (m_MsDataGrid.GetSafeHwnd())
	{
		m_MsDataGrid.MoveWindow(CRect(0,60,ViewRect.Width(),ViewRect.Height()),TRUE);
	}
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

	if (!g_online)
	{
		m_connectionState.SetWindowText(L"NO CONNECTION");
		m_connectionState.SetStringFontSize(12);
		COLORREF cf=RGB(255,0,0);
		m_connectionState.SetStringColor(cf);
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
			//return;
		}
		else if (m_MultiReadReturnType==-2)
		{
			m_connectionState.SetWindowText(L"Write Error");
			m_connectionState.SetStringFontSize(12);
			COLORREF cf=RGB(255,0,0);
			m_connectionState.SetStringColor(cf);
			//return;
		}
		else if (m_MultiReadReturnType==-3)
		{
			m_connectionState.SetWindowText(L"Read Error");
			m_connectionState.SetStringFontSize(12);
			COLORREF cf=RGB(255,0,0);
			m_connectionState.SetStringColor(cf);
			//return;
		}
		else if (m_MultiReadReturnType==-4)
		{
			m_connectionState.SetWindowText(L"Timeout Error");
			m_connectionState.SetStringFontSize(12);
			COLORREF cf=RGB(255,0,0);
			m_connectionState.SetStringColor(cf);
			//return;
		}
		else if (m_MultiReadReturnType==-5)
		{
			m_connectionState.SetWindowText(L"CRC Error");
			m_connectionState.SetStringFontSize(12);
			COLORREF cf=RGB(255,0,0);
			m_connectionState.SetStringColor(cf);
			//return;
		}
		else
		{
              m_connectionState.SetWindowText(L"");
			  SetPaneString(1,L"Connected");
		}
		
// 		m_connectionState.SetStringFontSize(14);
// 		COLORREF cf=RGB(0,0,0);
// 		m_connectionState.SetStringColor(cf);
	}
	

	//m_Tx_Rx.SetWindowText(strTemp);

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
	m_Tx_Rx.SetStringFontSize(15);
	if (m_isgetmodel)
	{
		m_modelname=GetProductName(m_modeldata[1]);
		CString showmodelname;
		showmodelname.Format(_T("Model Name:%s"),m_modelname.GetBuffer());
		m_ModelNameRichEdit.SetWindowText(showmodelname);
		m_ModelNameRichEdit.SetStringFontSize(13);
		CString showslaveid;
		showslaveid.Format(_T("ID:%d"),m_modeldata[0]);
		m_SlaveIDRichEditCtrl.SetWindowText(showslaveid);
		m_SlaveIDRichEditCtrl.SetStringFontSize(13);
	}
	Initial_RegName();
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
	m_MsDataGrid.put_Cols(m_grid_cols);
	m_MsDataGrid.put_Rows(m_grid_rows);

	CString index;
	CString DataTemp;
	int Index;
	int Index_start,Index_end,row;
	//写下标
	if (m_PLC_Addresses==1)
	{
		Index_start=1;
		Index_end=m_MsDataGrid.get_Rows();

	} 
	else
	{
		Index_start=0;
		Index_end=m_MsDataGrid.get_Rows()-1;
	}
	row=1;
	for (int i=Index_start;i<Index_end;i++)
	{
		index.Format(_T("%d"),i);
		m_MsDataGrid.put_TextMatrix(row,0,index);
		row++;
	}
	

	if (m_Hide_Alias_Columns!=0)
	{
		 //初始化第0行
		for (int i=1;i<m_MsDataGrid.get_Cols();i++)
		{
		index.Format(_T("%d"),m_address+(i-1)*(m_MsDataGrid.get_Rows()-1));
		m_MsDataGrid.put_TextMatrix(0,i,index);
		}
			for (int j=1;j<m_MsDataGrid.get_Cols();j++)
			{
				for (int i=1;i<m_MsDataGrid.get_Rows();i++)
				{
					Index=(j-1)*(m_MsDataGrid.get_Rows()-1)+(i-1);
					if (Index<=m_Quantity)
					{
						m_MsDataGrid.put_TextMatrix(i,j,Get_Data(Index));
					}
				}
			}


			for (int i=1;i<m_MsDataGrid.get_Cols();i++)
			{



				for (int j=0;j<m_MsDataGrid.get_Rows();j++)
				{

					if (j==0)
					{
						//初始化第0行
						if (i%2!=0)
						{
							index=L"Address";
							m_MsDataGrid.put_TextMatrix(0,i,index);
						} 
						else
						{
							index=L"Value";
							m_MsDataGrid.put_TextMatrix(0,i,index);
						}
					} 
					else
					{
						Index=(j-1)*(m_MsDataGrid.get_Rows()-1)+(i-1);
						if (i%2!=0)//Add
						{
							if (Index<=m_Quantity)
							{
								CString StrTemp;
								StrTemp.Format(_T("%d"),Get_Reg_Add(Index));
								m_MsDataGrid.put_TextMatrix(i,j,StrTemp);
							}
						}
						else//Value
						{
							if (Index<=m_Quantity)
							{
								m_MsDataGrid.put_TextMatrix(i,j,Get_Data(Index));
							}
						}
					}
				}
			}
	}
	else{
		//初始化行
		for (int i=1;i<m_MsDataGrid.get_Cols();i++)
		{

			
			 
			for (int j=0;j<m_MsDataGrid.get_Rows();j++)
			{
                 if (j==0)
                 {
					 //初始化第0行
					 if((i%3)==1)
					 {
						 index=L"Alias";
						 m_MsDataGrid.put_TextMatrix(0,i,index);
					 } 
					else if ((i%3)==2)
					 {
						 index=L"Address";
						 m_MsDataGrid.put_TextMatrix(0,i,index);
					 } 
					 else if ((i%3)==0)
					 {
						 index=L"Value";
						 m_MsDataGrid.put_TextMatrix(0,i,index);
					 }
					 
                 } 
                 else
                 {
					 if ((i%3)==1)
					 {
						 Index=((i+2)/3-1)*(m_MsDataGrid.get_Rows()-1)+(j-1);
						 m_MsDataGrid.put_TextMatrix(j,i, m_Alias[Index]);

					 }
					 else if ((i%3)==2)
					 {
						 Index=((i+1)/3-1)*(m_MsDataGrid.get_Rows()-1)+(j-1);
						 if (Index<=m_Quantity)
						 {
							 CString StrTemp;
							 StrTemp.Format(_T("%d"),Get_Reg_Add(Index));
							 m_MsDataGrid.put_TextMatrix(j,i,StrTemp);
							 
						 }
						 
					 }
					 else if ((i%3)==0)
					 {
						 Index=(i/3-1)*(m_MsDataGrid.get_Rows()-1)+(j-1);
						 if (Index<=m_Quantity)
						 {
							 m_MsDataGrid.put_TextMatrix(j,i,Get_Data(Index));
						 }
					 }


				 }
			}
		}
	}

	for (vector<Registers_Infor>::iterator it=g_vectRegisters.begin();it!=g_vectRegisters.end();)
	{
		RegPoint pt ;
		if (it->Reg_Point.size()==0)
		{
			pt.Time_offset=1;
			pt.Value_offset=(unsigned short )m_DataBuffer[it->address];
		}
		else
		{
			pt.Time_offset=it->Reg_Point[it->Reg_Point.size()-1].Time_offset+1;

			pt.Value_offset=(unsigned short )m_DataBuffer[it->address];
		}
		it->Reg_Point.push_back(pt);
		++it;
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
	}
	return CFormView::WindowProc(message, wParam, lParam);
}


HBRUSH CModbusPollView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);


	

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CModbusPollView::DBClickMsflexgridDataGrid(){
	long lRow,lCol;
	lRow = m_MsDataGrid.get_RowSel();//获取点击的行号	
	lCol = m_MsDataGrid.get_ColSel(); //获取点击的列号
	TRACE(_T("Click input grid!\n"));

	CRect rect;
	m_MsDataGrid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	CDC* pDC =GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_MsDataGrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_MsDataGrid.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_MsDataGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_MsDataGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rcCell(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rcCell.OffsetRect(rect.left+1,rect.top+1);
	ReleaseDC(pDC);
	CString strValue = m_MsDataGrid.get_TextMatrix(lRow,lCol);

	m_Current_Col=lCol;
	m_Current_Row=lRow;
	
	int Index;
    UINT Address;
	if (m_Hide_Alias_Columns!=0)//都是数值的
	{
		Index=(lCol-1)*(m_MsDataGrid.get_Rows()-1)+(lRow-1);
		Show_Name=FALSE;
	}
	else//含义名字的
	{
		if (lCol%2!=0)
		{
			Show_Name=TRUE;
		}
		else
		{
			Show_Name=FALSE;
			Index=(lCol/2-1)*(m_MsDataGrid.get_Rows()-1)+(lRow-1);
		}

		//Index=(lCol/2-1)*(m_MsDataGrid.get_Rows()-1)+(lRow-1);
	}
	if (Show_Name)
	{
		m_edit_name.ShowWindow(SW_SHOW);	
		m_edit_name.SetWindowText(strValue);
		m_edit_name.MoveWindow(rcCell); //移动到选中格的位置，覆盖
		m_edit_name.BringWindowToTop();
		m_edit_name.SetFocus(); //获取焦点

	} 
	else
	{
		Address=Get_Reg_Add(Index);
		CString Value=Get_Data_No_Address(Index);

		//CWriteSingleRegisterDlg(UINT Slave_ID=255,UINT Address=0,BOOL Close_Dlg=FALSE,BOOL Single_Function=FALSE,UINT DataFormat=0,int value=0,CWnd* pParent = NULL)
		if (m_Display<3)
		{
			CWriteSingleRegisterDlg dlg(m_Slave_ID,Address,m_close_dlg,m_function,m_Display,m_PLC_Addresses,Value,NULL);
			if (IDOK==dlg.DoModal())
			{
				m_close_dlg=dlg.m_close_dlg;
				m_function=dlg.m_function;
			}
			 
		}
		else if (m_Display==3)
		{

			CWriteSingle_BinaryDlg dlg;
			dlg.m_slave_id=m_Slave_ID;
			dlg.m_address=Address;
			dlg.m_close_dlg=m_close_dlg;
			dlg.m_function=m_function;
			dlg.m_data_format=m_Display;
			dlg.m_base_0=m_PLC_Addresses;
			dlg.m_StrValue=Value;
			if (IDOK==dlg.DoModal())
			{
				m_close_dlg=dlg.m_close_dlg;
				m_function=dlg.m_function;
			}

		}
	}
	
	

}


void CModbusPollView::ClickMsflexgridDataGrid(){
	long lRow,lCol;
	lRow = m_MsDataGrid.get_RowSel();//获取点击的行号	
	lCol = m_MsDataGrid.get_ColSel(); //获取点击的列号
	TRACE(_T("Click input grid!\n"));

	CRect rect;
	m_MsDataGrid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	CDC* pDC =GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_MsDataGrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_MsDataGrid.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_MsDataGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_MsDataGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rcCell(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rcCell.OffsetRect(rect.left+1,rect.top+1);
	ReleaseDC(pDC);
	CString strValue = m_MsDataGrid.get_TextMatrix(lRow,lCol);

	m_Current_Col=lCol;
	m_Current_Row=lRow;
	m_edit_name.ShowWindow(SW_HIDE);
	if (m_Hide_Alias_Columns!=0)//都是数值的
	{
		 
		Show_Name=FALSE;
	}
	else//含义名字的
	{
		if (lCol%2!=0)
		{
			Show_Name=TRUE;
		}
		else
		{
			Show_Name=FALSE;
			 
		}

		//Index=(lCol/2-1)*(m_MsDataGrid.get_Rows()-1)+(lRow-1);
	}

	if (Show_Name)
	{
		m_edit_name.ShowWindow(SW_SHOW);	
		m_edit_name.SetWindowText(strValue);
		m_edit_name.MoveWindow(rcCell); //移动到选中格的位置，覆盖
		m_edit_name.BringWindowToTop();
		m_edit_name.SetFocus(); //获取焦点

	} 

	   //          m_MsDataGrid.put_Row(lRow);
				//m_MsDataGrid.put_Col(lCol);
				////100 149 237
				//m_MsDataGrid.put_CellBackColor(RGB(100,150,240));

 //   if (lRow!=0&&lCol==0)
 //   {
	//	/*for (int row=1;row<m_msflexgrid.get_Rows();row++)
	//	{*/
	//		for (int i=1;i<m_MsDataGrid.get_Cols();i++){
	//			m_MsDataGrid.put_Row(lRow);
	//			m_MsDataGrid.put_Col(i);
	//			//100 149 237
	//			m_MsDataGrid.put_CellBackColor(RGB(100,150,240));
	//		}
	///*	}*/
	// 
 //   }
	//if (lCol!=0&&lRow==0)
	//{
	//	for (int row=1;row<m_MsDataGrid.get_Rows();row++)
	//	{
	//		/*for (int i=1;i<m_msflexgrid.get_Cols();i++){*/
	//			m_MsDataGrid.put_Row(row);
	//			m_MsDataGrid.put_Col(lCol);
	//			//100 149 237
	//			m_MsDataGrid.put_CellBackColor(RGB(100,150,240));
	//	/*	}*/
	//	}
	//}

}

void CModbusPollView::OnEnKillfocusEditName()
{
	 CString strText;
	 m_edit_name.GetWindowTextW(strText);
	 m_MsDataGrid.put_TextMatrix(m_Current_Row,m_Current_Col,strText);
     int Index=((m_Current_Col+1)/2-1)*(m_MsDataGrid.get_Rows()-1)+(m_Current_Row-1);
     m_Alias[Index]=strText;

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
			::PostMessage(pMBPollView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
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
		::PostMessage(pMBPollView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
	}
	Sleep(sleep);
	}

	pMBPollView=NULL;
return 1;
}

BOOL CModbusPollView::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	 	
	return CFormView::DestroyWindow();
}


//void CModbusPollView::OnViewRegistervalueanalyzer()
//{
//	 CRegisterValueAnalyzerDlg dlg;
//	 dlg.DoModal();
//}


void CModbusPollView::OnEditAdd()
{
	int Index;
	if(m_Hide_Alias_Columns!=0)//都是数值的
	{
		Index=(m_Current_Col-1)*(m_MsDataGrid.get_Rows()-1)+(m_Current_Row-1);
	 
	}
	else//含义名字的
	{
		if (m_Current_Col%2!=0)
		{
			 
		}
		else
		{
			 
			Index=(m_Current_Col/2-1)*(m_MsDataGrid.get_Rows()-1)+(m_Current_Row-1);
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
}


void CModbusPollView::OnUpdateSetupExcellogingoff(CCmdUI *pCmdUI)
{
	 pCmdUI->Enable(!m_logExcel);
}


void CModbusPollView::OnSetupExcelloging()
{
// 	_Application app;    
// 	Workbooks books;
// 	_Workbook book;
// 	Worksheets sheets;
// 	_Worksheet sheet;
// 	Range range;
// 
// 	Range rgMyRge1, rgMyRge2; 	
// 	CString rowContent;
// 	COleVariant covTrue((short)TRUE), covFalse((short)FALSE), covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
// 	Sleep(2000);
// 	if (!app.CreateDispatch(_T("Excel.Application"),NULL)) 
// 	{ 
// 		AfxMessageBox(_T("Create Excel false!")); 
// 		return;
// 	} 
// 
// 
// 
// 	m_logExcel=FALSE;


}


void CModbusPollView::OnUpdateSetupExcelloging(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_logExcel);
}
