
// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "Modbus Poll.h"

#include "ChildFrm.h"

#include "ReadWriteDefinitionDlg.h"
//#include "Modbus PollView.h"

#include "CommunicationTrafficDlg.h"
#include "WriteSingleRegisterDlg.h"
#include "WriteMultiRegistersDlg.h"
#include "WriteSingle_BinaryDlg.h"
#include "ReadWrite_Multiple_RegDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_COMMAND(ID_SETUP_READ, &CChildFrame::OnSetupRead)
	ON_COMMAND(ID_SETUP_READ32783, &CChildFrame::OnSetupRead32783)
//	ON_COMMAND(ID_DISPALY_COMMUNICATION, &CChildFrame::OnDispalyCommunication)
//	ON_WM_CREATE()
ON_COMMAND(ID_DISPALY_SIGNED, &CChildFrame::OnDispalySigned)
ON_COMMAND(ID_DISPALY_UNSIGNED, &CChildFrame::OnDispalyUnsigned)
ON_COMMAND(ID_DISPALY_HEX, &CChildFrame::OnDispalyHex)
ON_COMMAND(ID_DISPALY_LONG, &CChildFrame::OnDispalyLong)
ON_COMMAND(ID_DISPALY_LONGINVERSE, &CChildFrame::OnDispalyLonginverse)
ON_COMMAND(ID_DISPALY_FLOATINVERSE, &CChildFrame::OnDispalyFloatinverse)
ON_COMMAND(ID_DISPALY_FLOAT, &CChildFrame::OnDispalyFloat)
ON_COMMAND(ID_DISPALY_DOUBLEINVERSE, &CChildFrame::OnDispalyDoubleinverse)
ON_COMMAND(ID_DISPALY_DOUBLE, &CChildFrame::OnDispalyDouble)
ON_COMMAND(ID_DISPALY_BINARY, &CChildFrame::OnDispalyBinary)
ON_UPDATE_COMMAND_UI(ID_DISPALY_BINARY, &CChildFrame::OnUpdateDispalyBinary)
ON_UPDATE_COMMAND_UI(ID_DISPALY_DOUBLE, &CChildFrame::OnUpdateDispalyDouble)
ON_UPDATE_COMMAND_UI(ID_DISPALY_DOUBLEINVERSE, &CChildFrame::OnUpdateDispalyDoubleinverse)
ON_UPDATE_COMMAND_UI(ID_DISPALY_FLOAT, &CChildFrame::OnUpdateDispalyFloat)
ON_UPDATE_COMMAND_UI(ID_DISPALY_FLOATINVERSE, &CChildFrame::OnUpdateDispalyFloatinverse)
ON_UPDATE_COMMAND_UI(ID_DISPALY_HEX, &CChildFrame::OnUpdateDispalyHex)
ON_UPDATE_COMMAND_UI(ID_DISPALY_LONG, &CChildFrame::OnUpdateDispalyLong)
ON_UPDATE_COMMAND_UI(ID_DISPALY_LONGINVERSE, &CChildFrame::OnUpdateDispalyLonginverse)
ON_UPDATE_COMMAND_UI(ID_DISPALY_SIGNED, &CChildFrame::OnUpdateDispalySigned)
ON_UPDATE_COMMAND_UI(ID_DISPALY_UNSIGNED, &CChildFrame::OnUpdateDispalyUnsigned)
ON_COMMAND(ID_DISPALY_PLCADDRESS, &CChildFrame::OnDispalyPlcaddress)
ON_COMMAND(ID_DISPALY_PROTOCOLADDRESSES, &CChildFrame::OnDispalyProtocoladdresses)
ON_UPDATE_COMMAND_UI(ID_DISPALY_PLCADDRESS, &CChildFrame::OnUpdateDispalyPlcaddress)
ON_UPDATE_COMMAND_UI(ID_DISPALY_PROTOCOLADDRESSES, &CChildFrame::OnUpdateDispalyProtocoladdresses)
ON_WM_CREATE()
ON_COMMAND(ID_FUNCTIONS_06, &CChildFrame::OnFunctions06)
ON_COMMAND(ID_FUNCTIONS_16, &CChildFrame::OnFunctions16)
ON_COMMAND(ID_FUNCTIONS_23, &CChildFrame::OnFunctions23)
ON_COMMAND(ID_FUNCTIONS_05, &CChildFrame::OnFunctions05)
ON_COMMAND(ID_FUNCTIONS_15, &CChildFrame::OnFunctions15)
END_MESSAGE_MAP()

// CChildFrame construction/destruction
CChildFrame::CChildFrame()
{

		 m_apply=TRUE;
		 m_wronce=FALSE;
}

CChildFrame::~CChildFrame()
{

}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// 
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame message handlers
void CChildFrame::OnSetupRead()
{
	
    CModbusPollView* pModbusView=(CModbusPollView*)GetActiveView();
	
	CReadWriteDefinitionDlg dlg;
	dlg.m_slave_id= pModbusView->m_Slave_ID;
	dlg.m_function=pModbusView->m_Function;
	dlg.m_quantity=pModbusView->m_Quantity;
	dlg.m_scan_rate=pModbusView->m_Scan_Rate;
	dlg.m_rows=pModbusView->m_Rows;
	dlg.m_data_formate=pModbusView->m_Display;
	dlg.m_hide_alias_col=pModbusView->m_Hide_Alias_Columns;
	dlg.m_address_in_cell=pModbusView->m_Address_Cell;
	dlg.m_plc_addresses=pModbusView->m_PLC_Addresses;
	dlg.m_address=pModbusView->m_address;
    dlg.m_apply=pModbusView->m_apply;
	dlg.m_wronce=pModbusView->m_wronce;
	dlg.m_pWnd=this->m_hWnd;
	if (IDOK==dlg.DoModal())
	{
		m_Slave_ID=dlg.m_slave_id;
		m_Function=dlg.m_function;
		m_Quantity=dlg.m_quantity;
		m_Scan_Rate=dlg.m_scan_rate;
		m_Rows=dlg.m_rows;
		m_Display=dlg.m_data_formate;
		m_Hide_Alias_Columns=dlg.m_hide_alias_col;
		m_Address_Cell=dlg.m_address_in_cell;
		m_PLC_Adresses=dlg.m_plc_addresses;
		m_address=dlg.m_address;
		m_apply=dlg.m_apply;
		m_wronce=dlg.m_wronce;
	}	
 //CModbusPollView* pModbusView=(CModbusPollView*)GetActiveView();
 pModbusView->m_Slave_ID=m_Slave_ID;
 pModbusView->m_Function=m_Function;
 pModbusView->m_Quantity=m_Quantity;
 pModbusView->m_Scan_Rate=m_Scan_Rate;
 pModbusView->m_Rows=m_Rows;
 pModbusView->m_Display=m_Display;
 pModbusView->m_Hide_Alias_Columns=m_Hide_Alias_Columns;
 pModbusView->m_Address_Cell=m_Address_Cell;
 pModbusView->m_PLC_Addresses=m_PLC_Adresses;
 pModbusView->m_address=m_address;
 pModbusView->m_ischangedAddress=TRUE;

 pModbusView->m_apply=m_apply;
 pModbusView->m_wronce=m_wronce;
::SendMessage(pModbusView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
 //pModbusView->OnInitialUpdate();
 pModbusView=NULL;
}


void CChildFrame::OnSetupRead32783()
{
	  CModbusPollView* pModbusView=(CModbusPollView*)GetActiveView();
	  pModbusView->m_Tx=0;
	  pModbusView->m_Err=0;
	  ::SendMessage(pModbusView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
	  pModbusView=NULL;

}

void CChildFrame::Show_Data_Traffic_Window(){

}
void CChildFrame::OnDispalySigned()
{
	CModbusPollView* pModbusView=(CModbusPollView*)GetActiveView();
	pModbusView->m_Display=0;
	m_Display=0;
	::SendMessage(pModbusView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
	pModbusView=NULL;
}


void CChildFrame::OnDispalyUnsigned()
{
	CModbusPollView* pModbusView=(CModbusPollView*)GetActiveView();
	pModbusView->m_Display=1;
	m_Display=1;
	::SendMessage(pModbusView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
	pModbusView=NULL;
}


void CChildFrame::OnDispalyHex()
{
	CModbusPollView* pModbusView=(CModbusPollView*)GetActiveView();
	pModbusView->m_Display=2;
	m_Display=2;
	::SendMessage(pModbusView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
	pModbusView=NULL;
}


void CChildFrame::OnDispalyLong()
{
	CModbusPollView* pModbusView=(CModbusPollView*)GetActiveView();
	pModbusView->m_Display=4;
	m_Display=4;
	::SendMessage(pModbusView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
	pModbusView=NULL;
}


void CChildFrame::OnDispalyLonginverse()
{
	CModbusPollView* pModbusView=(CModbusPollView*)GetActiveView();
	pModbusView->m_Display=5;
	m_Display=5;
	::SendMessage(pModbusView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
	pModbusView=NULL;
}


void CChildFrame::OnDispalyFloatinverse()
{
	CModbusPollView* pModbusView=(CModbusPollView*)GetActiveView();
	pModbusView->m_Display=7;
	m_Display=7;
	::SendMessage(pModbusView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
	pModbusView=NULL;
}


void CChildFrame::OnDispalyFloat()
{
	CModbusPollView* pModbusView=(CModbusPollView*)GetActiveView();
	pModbusView->m_Display=6;
	m_Display=6;
	::SendMessage(pModbusView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
	pModbusView=NULL;
}


void CChildFrame::OnDispalyDoubleinverse()
{
	CModbusPollView* pModbusView=(CModbusPollView*)GetActiveView();
	pModbusView->m_Display=9;
	m_Display=9;
	::SendMessage(pModbusView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
	pModbusView=NULL; 
}


void CChildFrame::OnDispalyDouble()
{
	CModbusPollView* pModbusView=(CModbusPollView*)GetActiveView();
	pModbusView->m_Display=8;
	m_Display=8;
	::SendMessage(pModbusView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
	pModbusView=NULL;
}


void CChildFrame::OnDispalyBinary()
{
	CModbusPollView* pModbusView=(CModbusPollView*)GetActiveView();
	pModbusView->m_Display=3;
	m_Display=3;
	::SendMessage(pModbusView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
	pModbusView=NULL;
}


void CChildFrame::OnUpdateDispalyBinary(CCmdUI *pCmdUI)
{
	 pCmdUI->SetCheck(m_Display==3);
}


void CChildFrame::OnUpdateDispalyDouble(CCmdUI *pCmdUI)
{
	  pCmdUI->SetCheck(m_Display==8);
}


void CChildFrame::OnUpdateDispalyDoubleinverse(CCmdUI *pCmdUI)
{
	  pCmdUI->SetCheck(m_Display==9);
}


void CChildFrame::OnUpdateDispalyFloat(CCmdUI *pCmdUI)
{
    pCmdUI->SetCheck(m_Display==6);
}


void CChildFrame::OnUpdateDispalyFloatinverse(CCmdUI *pCmdUI)
{
	  pCmdUI->SetCheck(m_Display==7);
}


void CChildFrame::OnUpdateDispalyHex(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_Display==2);
}


void CChildFrame::OnUpdateDispalyLong(CCmdUI *pCmdUI)
{
	  pCmdUI->SetCheck(m_Display==4);
}


void CChildFrame::OnUpdateDispalyLonginverse(CCmdUI *pCmdUI)
{
	  pCmdUI->SetCheck(m_Display==5);
}


void CChildFrame::OnUpdateDispalySigned(CCmdUI *pCmdUI)
{
	 pCmdUI->SetCheck(m_Display==0);
}


void CChildFrame::OnUpdateDispalyUnsigned(CCmdUI *pCmdUI)
{
	 pCmdUI->SetCheck(m_Display==1);
}


void CChildFrame::OnDispalyPlcaddress()
{
	 
	CModbusPollView* pModbusView=(CModbusPollView*)GetActiveView();
	pModbusView->m_PLC_Addresses=1;
	m_PLC_Adresses=1;
	++m_address;
	pModbusView->m_address=m_address;
	
	::SendMessage(pModbusView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
	pModbusView=NULL;
}


void CChildFrame::OnDispalyProtocoladdresses()
{
	CModbusPollView* pModbusView=(CModbusPollView*)GetActiveView();
	pModbusView->m_PLC_Addresses=0;
	--m_address;
   pModbusView->m_address=m_address;
	m_PLC_Adresses=0;
 
	::SendMessage(pModbusView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
	pModbusView=NULL;
}


void CChildFrame::OnUpdateDispalyPlcaddress(CCmdUI *pCmdUI)
{
	/*if (m_Address_Cell==0)
	{
		pCmdUI->Enable(FALSE);
		return;
	}*/
	pCmdUI->SetCheck(m_PLC_Adresses==1);
}


void CChildFrame::OnUpdateDispalyProtocoladdresses(CCmdUI *pCmdUI)
{
	/*if (m_Address_Cell==0)
	{
		pCmdUI->Enable(FALSE);
		return;
	}*/
	pCmdUI->SetCheck(m_PLC_Adresses==0);
}


int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	 



	

	return 0;
}


void CChildFrame::OnFunctions06()
{
	 CModbusPollView* pModbusView=(CModbusPollView*)GetActiveView();
	 BOOL Show_Name=FALSE;
	 int Index;
	 UINT Address;
	int lCol=pModbusView->m_Current_Col;
	int lRow=pModbusView->m_Current_Row;
	 if (m_Hide_Alias_Columns!=0)//都是数值的
	 {
		 Index=(lCol-1)*(pModbusView->m_data_rows)+(lRow-1);
		 Show_Name=FALSE;
	 }
	 else//含义名字的
	 {
		 if (lCol%2!=0)
		 {
			 Show_Name=FALSE;
			 Index=(lCol+1/2-1)*(pModbusView->m_data_rows)+(lRow-1);
		 }
		 else
		 {
			 Show_Name=FALSE;
			 Index=(lCol/2-1)*(pModbusView->m_data_rows)+(lRow-1);
		 }
		 

	 
	 }
	 if (Show_Name)
	 {
		 

	 } 
	 else
	 {
		 Address=pModbusView->Get_Reg_Add(Index);
		 CString Value=pModbusView->Get_Data_No_Address(Index);
		 //CWriteSingleRegisterDlg(UINT Slave_ID=255,UINT Address=0,BOOL Close_Dlg=FALSE,BOOL Single_Function=FALSE,UINT DataFormat=0,int value=0,CWnd* pParent = NULL)
		 if (m_Display<3)
		 {
			 CWriteSingleRegisterDlg dlg(m_Slave_ID,Address,pModbusView->m_close_dlg,pModbusView->m_function,m_Display,m_PLC_Adresses,Value,NULL);
			 if (IDOK==dlg.DoModal())
			 {
				 pModbusView->m_close_dlg=dlg.m_close_dlg;
				 pModbusView->m_function=dlg.m_function;
			 }
			 
		 }
		 else if (m_Display==3)
		 {

		 CWriteSingle_BinaryDlg dlg;
		 dlg.m_slave_id=m_Slave_ID;
		 dlg.m_address=Address;
		 dlg.m_close_dlg=pModbusView->m_close_dlg;
		 dlg.m_function=pModbusView->m_function;
		 dlg.m_data_format=m_Display;
		 dlg.m_base_0=m_PLC_Adresses;
		 dlg.m_StrValue=Value;
		 if (IDOK==dlg.DoModal())
		 {
			 pModbusView->m_close_dlg=dlg.m_close_dlg;
			 pModbusView->m_function=dlg.m_function;
		 }

		 }
	 }
	 pModbusView=NULL;
}


void CChildFrame::OnFunctions16()
{
	CModbusPollView* pModbusView=(CModbusPollView*)GetActiveView();
     
	 CWriteMultiRegistersDlg dlg(m_Slave_ID,m_address,m_Quantity,
		                         pModbusView->m_DataBuffer,m_Display,
		                         m_PLC_Adresses,NULL);
	 dlg.DoModal();
}


void CChildFrame::OnFunctions23()
{
	 CModbusPollView* pModbusView=(CModbusPollView*)GetActiveView();
	 CReadWrite_Multiple_RegDlg dlg;
	 dlg.m_slave_id=m_Slave_ID;
	 dlg.m_relative_address=m_address;
	 dlg.m_address=m_address;
	 dlg.m_quantity=m_Quantity;
	 dlg.m_result_address=m_address;
	 dlg.m_result_quantity=m_Quantity;
	 dlg.m_DataBuffer=pModbusView->m_DataBuffer;
	 dlg.m_dataformate=m_Display;
	 dlg.m_base0=m_PLC_Adresses;
	 dlg.DoModal();
}




LRESULT CChildFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
      CModbusPollView* pModbusView=(CModbusPollView*)GetActiveView();
	  CReadWriteDefinitionDlg *pdlg=(CReadWriteDefinitionDlg *)wParam;
	if (message==MY_READ_ONCE)
	{
		m_apply=pdlg->m_apply;
		m_wronce=pdlg->m_wronce;
		pModbusView->m_apply=m_apply;
		pModbusView->m_wronce=m_wronce;
		//::PostMessage(pModbusView->m_hWnd,MY_FRESH_MBPOLLVIEW,0,0);
		return 1;
	} 


	return CMDIChildWndEx::WindowProc(message, wParam, lParam);
}

#include "WriteSingleCoilDlg.h"

void CChildFrame::OnFunctions05()
{
	CModbusPollView* pModbusView = (CModbusPollView*)GetActiveView();
	BOOL Show_Name = FALSE;
	int Index;
	UINT Address;
	int lCol = pModbusView->m_Current_Col;
	int lRow = pModbusView->m_Current_Row;
	if (m_Hide_Alias_Columns != 0)//都是数值的
	{
		Index = (lCol - 1)*(pModbusView->m_data_rows) + (lRow - 1);
		Show_Name = FALSE;
	}
	else//含义名字的
	{
		if (lCol % 2 != 0)
		{
			Show_Name = FALSE;
			Index = (lCol + 1 / 2 - 1)*(pModbusView->m_data_rows) + (lRow - 1);
		}
		else
		{
			Show_Name = FALSE;
			Index = (lCol / 2 - 1)*(pModbusView->m_data_rows) + (lRow - 1);
		}



	}
	if (Show_Name)
	{


	}
	else
	{
		Address = pModbusView->Get_Reg_Add(Index);
		CString Value = pModbusView->Get_Data_No_Address(Index);
		//CWriteSingleRegisterDlg(UINT Slave_ID=255,UINT Address=0,BOOL Close_Dlg=FALSE,BOOL Single_Function=FALSE,UINT DataFormat=0,int value=0,CWnd* pParent = NULL)
		 
// 			CWriteSingleRegisterDlg dlg(m_Slave_ID, Address, pModbusView->m_close_dlg, pModbusView->m_function, m_Display, m_PLC_Adresses, Value, NULL);
// 			if (IDOK == dlg.DoModal())
// 			{
// 				pModbusView->m_close_dlg = dlg.m_close_dlg;
// 				pModbusView->m_function = dlg.m_function;
// 			}

			CWriteSingleCoilDlg dlg(m_Slave_ID, Address, pModbusView->m_close_dlg, pModbusView->m_function, m_PLC_Adresses, _wtoi(Value), NULL);

			if (IDOK == dlg.DoModal())
			{
				pModbusView->m_close_dlg = dlg.m_close_dlg;
				pModbusView->m_function = dlg.m_function15;

			}

		 
		 
	}
	pModbusView = NULL;
}

#include "WriteMultipleCoilsDlg.h"
void CChildFrame::OnFunctions15()
{
	/*CWriteMultipleCoilsDlg dlg;
	dlg.DoModal();*/

	CModbusPollView* pModbusView = (CModbusPollView*)GetActiveView();

	  CWriteMultipleCoilsDlg dlg(m_Slave_ID, m_address, m_Quantity,
		pModbusView->m_DataBuffer, m_Display,
		m_PLC_Adresses, NULL);
	   dlg.DoModal();

	 
}
