// AddBuilding.cpp : implementation file
//
#include "stdafx.h"
#include "T3000.h"
#include "MainFrm.h"

#include "AddBuilding.h"
#include "globle_function.h "

//AB means Add Building
#define AB_MAINNAME	1		// main building
#define AB_NAME	2			// sub net name
//#define AB_CONNECTION	3
#define	AB_PROTOCOL	3	// protocol 
#define	AB_IPADDRESS	4  // 
#define	AB_IPPORT	5
#define	AB_COMPORT	6
#define	AB_BAUDRAT	7


//#define MAX_FIELDS	10
CRITICAL_SECTION g_Lock;
#define UPD_BROADCAST_QRY_MSG 100
#define RESPONSE_MSG          UPD_BROADCAST_QRY_MSG+1
#define UDP_BROADCAST_PORT    1234
typedef struct infopack
{
 unsigned char msg;//RESPONSE_MSG
 unsigned char lenth;
 unsigned short* pNCInfo;
}NC_Info;

struct _NCInfo
{
	UINT nSerial;
	int nProductID;
	int modbusID;
	CString strIP;
	unsigned short nPort;
};
vector<_NCInfo> g_NCList;

SOCKET hBroad=NULL;
SOCKET sListen=NULL;
IMPLEMENT_DYNAMIC(CAddBuilding, CDialog)


CAddBuilding::CAddBuilding(CWnd* pParent /*=NULL*/)
	: CDialog(CAddBuilding::IDD, pParent)
{
	m_pRs=NULL;
	m_pCon=NULL;
	m_nCurRow=-1;
	m_nCurCol=-1;
	pNCScanThread=NULL;
	m_bRunningThread=FALSE;
}

CAddBuilding::~CAddBuilding()
{

}

void CAddBuilding::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDBUIDING_SELECTCOMBO, m_AddBuiding_SetComBox);
	DDX_Control(pDX, IDC_ADDBUIDING_SETEDIT, m_AddBuiding_SetEditCtrl);
	DDX_Control(pDX, IDC_ADDBUILDING_MSFLEXGRID, m_AddBuiding_FlexGrid);
	//DDX_Control(pDX, IDC_STATIC1, m_proptionCtrl);
	DDX_Control(pDX, IDC_MAINBUILDEDIT, m_mainBuildEdt);
	DDX_Control(pDX, IDC_ADDBUILDING_MSFLEXGRID2, m_NCFlexGrid);
	DDX_Control(pDX, IDC_SCANNCBTN, m_NCScanBtn);
	DDX_Control(pDX, IDC_SAVEBUTTON, m_SaveBtn);
}
BEGIN_MESSAGE_MAP(CAddBuilding, CDialog)
	ON_BN_CLICKED(IDC_AddBuiding, &CAddBuilding::OnBnClickedAddbuiding)
//	ON_BN_CLICKED(IDC_DELBUILDING, &CAddBuilding::OnBnClickedDelbuilding)
//	ON_BN_CLICKED(IDC_DELETEALLBUILDING, &CAddBuilding::OnBnClickedDeleteallbuilding)
	ON_EN_KILLFOCUS(IDC_ADDBUIDING_SETEDIT, &CAddBuilding::OnEnKillfocusAddbuidingSetedit)
	ON_CBN_SELCHANGE(IDC_ADDBUIDING_SELECTCOMBO, &CAddBuilding::OnCbnSelchangeAddbuidingSelectcombo)
	ON_CBN_KILLFOCUS(IDC_ADDBUIDING_SELECTCOMBO, &CAddBuilding::OnCbnKillfocusAddbuidingSelectcombo)
	ON_BN_CLICKED(ID_EXIT, &CAddBuilding::OnBnClickedExit)
	ON_BN_CLICKED(IDOK, &CAddBuilding::OnBnClickedOk)
	ON_EN_SETFOCUS(IDC_ADDBUIDING_SETEDIT, &CAddBuilding::OnEnSetfocusAddbuidingSetedit)
	ON_COMMAND(ID_BUILDINGSEL_SELECT,OnMainBuildingSelect)
	ON_COMMAND(ID_BUILDINGSEL_DELETE,OnBuildingDelete)
	
	ON_COMMAND(ID_BUILDINGSEL_UNSLECT,OnMainBuildingUnSelect)
	ON_STN_CLICKED(IDC_STATIC1, &CAddBuilding::OnStnClickedStatic1)
	ON_CBN_SELENDOK(IDC_ADDBUIDING_SELECTCOMBO, &CAddBuilding::OnCbnSelendokAddbuidingSelectcombo)
	ON_EN_KILLFOCUS(IDC_MAINBUILDEDIT, &CAddBuilding::OnEnKillfocusMainbuildedit)
	ON_EN_SETFOCUS(IDC_MAINBUILDEDIT, &CAddBuilding::OnEnSetfocusMainbuildedit)

	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SAVEBUTTON, &CAddBuilding::OnBnClickedSavebutton)
END_MESSAGE_MAP()




BOOL CAddBuilding::OnInitDialog()
{
	CDialog::OnInitDialog();
	::InitializeCriticalSection(&g_Lock);

	//GetSerialComm(m_szComm);
	//GetSerialComPortNumber(m_szComm);
	GetSerialComPortNumber1(m_szComm);
	CString strIPTest;
//	GetIPbyHostName(_T("www.google.com"),strIPTest);
	m_AddBuiding_SetComBox.ShowWindow(SW_HIDE);
	m_AddBuiding_SetEditCtrl.ShowWindow(SW_HIDE);
	m_mainBuildEdt.ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_SCANNCBTN)->ShowWindow(SW_HIDE);

	m_AddBuiding_FlexGrid.put_TextMatrix(0,AB_MAINNAME,_T("Main Building"));
	m_AddBuiding_FlexGrid.put_TextMatrix(0,AB_NAME,_T("Sub Net"));
	m_AddBuiding_FlexGrid.put_TextMatrix(0,AB_PROTOCOL,_T("Protocol"));
	m_AddBuiding_FlexGrid.put_TextMatrix(0,AB_IPADDRESS,_T("Ip Address"));
	m_AddBuiding_FlexGrid.put_TextMatrix(0,AB_IPPORT,_T("IP Port"));
	m_AddBuiding_FlexGrid.put_TextMatrix(0,AB_COMPORT,_T("COM Port"));
	m_AddBuiding_FlexGrid.put_TextMatrix(0,AB_BAUDRAT,_T("Baud Rate"));

	m_AddBuiding_FlexGrid.put_ColWidth(0,1100);
	m_AddBuiding_FlexGrid.put_ColWidth(AB_MAINNAME,1100);
	m_AddBuiding_FlexGrid.put_ColWidth(AB_NAME,1000);
	m_AddBuiding_FlexGrid.put_ColWidth(AB_PROTOCOL,1000);
	m_AddBuiding_FlexGrid.put_ColWidth(AB_IPADDRESS,1200);
	m_AddBuiding_FlexGrid.put_ColWidth(AB_IPPORT,1000);
	m_AddBuiding_FlexGrid.put_ColWidth(AB_COMPORT,1000);
	m_AddBuiding_FlexGrid.put_ColWidth(AB_BAUDRAT,1000);


	m_pCon.CreateInstance(_T("ADODB.Connection"));
	m_pRs.CreateInstance(_T("ADODB.Recordset"));
	m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

	ReloadAddBuildingDB();

	m_NCFlexGrid.put_TextMatrix(0,0,_T("Serial Number"));
	m_NCFlexGrid.put_TextMatrix(0,1,_T("Modbus ID"));
	m_NCFlexGrid.put_TextMatrix(0,2,_T("Product type"));
	m_NCFlexGrid.put_TextMatrix(0,3,_T("IP address"));
	m_NCFlexGrid.put_TextMatrix(0,4,_T("Port"));	
	m_NCFlexGrid.put_ColWidth(0,1100);
	m_NCFlexGrid.put_ColWidth(1,1100);
	m_NCFlexGrid.put_ColWidth(2,1100);
	m_NCFlexGrid.put_ColWidth(3,1400);
	m_NCFlexGrid.put_ColWidth(4,1100);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CAddBuilding, CDialog)
	ON_EVENT(CAddBuilding, IDC_ADDBUILDING_MSFLEXGRID, DISPID_CLICK, CAddBuilding::ClickAddbuildingMsflexgrid, VTS_NONE)
	ON_EVENT(CAddBuilding, IDC_ADDBUILDING_MSFLEXGRID, 71, CAddBuilding::EnterCellAddbuildingMsflexgrid, VTS_NONE)
	ON_EVENT(CAddBuilding, IDC_ADDBUILDING_MSFLEXGRID, 69, CAddBuilding::SelChangeAddbuildingMsflexgrid, VTS_NONE)
	ON_EVENT(CAddBuilding, IDC_ADDBUILDING_MSFLEXGRID, DISPID_DBLCLICK, CAddBuilding::DblClickAddbuildingMsflexgrid, VTS_NONE)
END_EVENTSINK_MAP()





void CAddBuilding::ClickAddbuildingMsflexgrid()
{

#if 1
	long lRow,lCol;
	lRow = m_AddBuiding_FlexGrid.get_RowSel();//
	lCol = m_AddBuiding_FlexGrid.get_ColSel(); 

	if(lRow<=0)
		return;
	m_nCurRow=lRow;
	m_nCurCol=lCol;
	m_strBuilding_Name2=m_AddBuiding_FlexGrid.get_TextMatrix(m_nCurRow,AB_NAME);
	m_strBuildingName=m_AddBuiding_FlexGrid.get_TextMatrix(m_nCurRow,AB_NAME);

	//m_BuildNameLst.at(m_BuildNameLst.size()-1).strMainBuildName
	if(m_nCurRow<m_AddBuiding_FlexGrid.get_Rows()-1)
	{
		m_strMainBuildingName2=m_BuildNameLst.at(m_nCurRow-1).strMainBuildName;
		//if(m_strMainBuildingName2.IsEmpty())
		//{
		//	m_strMainBuildingName2=m_BuildNameLst.at(m_nCurRow-1).strMainBuildName;
		//}
		m_strMainBuildingName=m_strMainBuildingName2;
	}	
	
	m_strProtocol=m_AddBuiding_FlexGrid.get_TextMatrix(m_nCurRow,AB_PROTOCOL);
	m_strIpAddress=m_AddBuiding_FlexGrid.get_TextMatrix(m_nCurRow,AB_IPADDRESS);
	m_strIpPort=m_AddBuiding_FlexGrid.get_TextMatrix(m_nCurRow,AB_IPPORT);
	m_strComPort=m_AddBuiding_FlexGrid.get_TextMatrix(m_nCurRow,AB_COMPORT);
	m_strBaudrat=m_AddBuiding_FlexGrid.get_TextMatrix(m_nCurRow,AB_BAUDRAT);
	

	CRect rect;
	m_AddBuiding_FlexGrid.GetWindowRect(rect); //
	ScreenToClient(rect); 
	CDC* pDC =GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//
	long y = m_AddBuiding_FlexGrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_AddBuiding_FlexGrid.get_ColPos(lCol)/nTwipsPerDotX;
	//
	long width = m_AddBuiding_FlexGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_AddBuiding_FlexGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//
	CRect rcCell(x,y,x+width,y+height);
	//
	rcCell.OffsetRect(rect.left+1,rect.top+1);
	ReleaseDC(pDC);
	CString strValue = m_AddBuiding_FlexGrid.get_TextMatrix(lRow,lCol);





	if(AB_PROTOCOL==lCol)
	{
		m_AddBuiding_SetComBox.ResetContent();
		m_AddBuiding_SetComBox.InsertString(0,_T("Modbus 485"));
		m_AddBuiding_SetComBox.InsertString(1,_T("Modbus TCP"));
		m_AddBuiding_SetComBox.InsertString(2,_T("Bacnet"));
		m_AddBuiding_SetComBox.MoveWindow(&rcCell,1); //
		m_AddBuiding_SetComBox.BringWindowToTop();
		m_AddBuiding_SetComBox.ShowWindow(SW_SHOW);//
		m_AddBuiding_SetComBox.SetFocus(); //
	}
	if(AB_COMPORT==lCol)
	{
		m_AddBuiding_SetComBox.ResetContent();
// 		m_AddBuiding_SetComBox.AddString(_T("COM1"));
// 		m_AddBuiding_SetComBox.AddString(_T("COM2"));
// 		m_AddBuiding_SetComBox.AddString(_T("COM3"));
// 		m_AddBuiding_SetComBox.AddString(_T("COM4"));
// 		m_AddBuiding_SetComBox.AddString(_T("COM5"));
// 		m_AddBuiding_SetComBox.AddString(_T("COM6"));
// 		m_AddBuiding_SetComBox.AddString(_T("COM7"));
// 		m_AddBuiding_SetComBox.AddString(_T("COM8"));
// 		m_AddBuiding_SetComBox.AddString(_T("COM9"));
		for (UINT i = 0; i < m_szComm.size(); i++)
		{
			m_AddBuiding_SetComBox.AddString(m_szComm[i]);
		}
		m_AddBuiding_SetComBox.MoveWindow(&rcCell,1); //
		m_AddBuiding_SetComBox.BringWindowToTop();
		m_AddBuiding_SetComBox.ShowWindow(SW_SHOW);//
		m_AddBuiding_SetComBox.SetFocus(); //
	}

#endif

}
void CAddBuilding::EnterCellAddbuildingMsflexgrid()
{
	
	// TODO: Add your message handler code here
}
void CAddBuilding::SelChangeAddbuildingMsflexgrid()
{
	// TODO: Add your message handler code here
}
void CAddBuilding::ReloadAddBuildingDB()
{
	
	//m_buildNameLst.clear();
	m_BuildNameLst.clear();
	CString strSql;
	//strSql.Format(_T("select * from Building where Main_BuildingName = '%s'"),m_strMainBuildingName);
	strSql.Format(_T("select * from Building order by Main_BuildingName"));
	m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);			
	m_AddBuiding_FlexGrid.put_Rows(m_pRs->RecordCount+2);	
	int temp_row=0;
	CString str_temp;
	str_temp.Empty();

	_variant_t temp_variant;
	while(VARIANT_FALSE==m_pRs->EndOfFile)
	{	
		++temp_row;
			
		int bdef=0;
		bdef=m_pRs->GetCollect(_T("Default_SubBuilding"));
		if(bdef==-1)//def building;
		{
			m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,0,_T("Select"));
		}
		else
		{
			m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,0,_T(""));

		}
		_buildName BuildNameNode;
		//m_mainBuildNameLst
		m_strMainBuildingName=m_pRs->GetCollect("Main_BuildingName");//
	//	m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,AB_NAME,m_strMainBuildingName);
	//	m_mainBuildNameLst.push_back(m_strMainBuildingName);
		if(m_BuildNameLst.size()==0)
		{
			//m_mainBuildNameLst.push_back(m_strMainBuildingName);
			m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,AB_MAINNAME,m_strMainBuildingName);
		}
			
		else
		{
			if(m_BuildNameLst.at(m_BuildNameLst.size()-1).strMainBuildName.CompareNoCase(m_strMainBuildingName)!=0)
			{
			//	m_mainBuildNameLst.push_back(m_strMainBuildingName);
				m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,AB_MAINNAME,m_strMainBuildingName);
			}
			else
			{
				m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,AB_MAINNAME,_T(""));
			}		
		}
		BuildNameNode.strMainBuildName=m_strMainBuildingName;


		//m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,AB_MAINNAME,m_strMainBuildingName);

		m_strBuildingName=m_pRs->GetCollect("Building_Name");//
	//	m_strBuilding_Name2=m_strBuildingName;
		m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,AB_NAME,m_strBuildingName);
		BuildNameNode.strSubBuildName=m_strBuildingName;
		m_BuildNameLst.push_back(BuildNameNode);
	//	m_buildNameLst.push_back(m_strBuildingName);



		temp_variant=m_pRs->GetCollect("Protocal");
		if(temp_variant.vt!=VT_NULL)
		{
			m_strProtocol=temp_variant;
			m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,AB_PROTOCOL,m_strProtocol);
		}			
		else
		{
			m_strProtocol=_T("");
			m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,AB_PROTOCOL,_T(""));
		}

		temp_variant=m_pRs->GetCollect("Ip_Address");
		if(temp_variant.vt!=VT_NULL)
			m_strIpAddress=temp_variant;
		else
			m_strIpAddress=_T("");
		
		if(m_strProtocol.CompareNoCase(_T("Modbus TCP"))==0)
		{
			m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,AB_IPADDRESS,m_strIpAddress);
		}
		else
		{   str_temp=NO_APPLICATION;//
			m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,AB_IPADDRESS,str_temp);
		}
	

		temp_variant=m_pRs->GetCollect("Ip_Port");
		if(temp_variant.vt!=VT_NULL)
			m_strIpPort=temp_variant;
		else
			m_strIpPort=_T("");

		
		if(m_strProtocol.CompareNoCase(_T("Modbus TCP"))==0)
		{
			m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,AB_IPPORT,m_strIpPort);
		}
		else
		{   str_temp=NO_APPLICATION;//
			m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,AB_IPPORT,str_temp);
		}
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////

		temp_variant=m_pRs->GetCollect("Com_Port");
		if(temp_variant.vt!=VT_NULL)
			m_strComPort=temp_variant;
		else
			m_strComPort=_T("");
		
		if(m_strProtocol.CompareNoCase(_T("Modbus 485"))==0)
		{
			if(IsValidCOM(m_strComPort))
			{
				m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,AB_COMPORT,m_strComPort);
			}
			else if(m_szComm.size()>0)
			{
				m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,AB_COMPORT,m_szComm[0]);
			}
		}
		else
		{
			str_temp=NO_APPLICATION;//
			m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,AB_COMPORT,str_temp);
		}
		

		//Baudrate
		temp_variant=m_pRs->GetCollect("Braudrate");
		if(temp_variant.vt!=VT_NULL)
			m_strBaudrat=temp_variant;
		else
			m_strBaudrat=_T("");
		
		if(m_strProtocol.CompareNoCase(_T("Modbus 485"))==0)
		{
			m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,AB_BAUDRAT,m_strBaudrat);
		}
		else
		{
			str_temp=NO_APPLICATION;//
			m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,AB_BAUDRAT,str_temp);
		}

		

		m_pRs->MoveNext();//
	}	
		++temp_row;	
		m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,0,_T(""));
		m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,1,_T(""));
		m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,2,_T(""));
		m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,3,_T(""));
		m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,4,_T(""));
		m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,5,_T(""));
		m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,6,_T(""));
		m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,7,_T(""));
		//m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,8,_T(""));

	if(m_pRs->State) 
		m_pRs->Close(); 
}
void CAddBuilding::OnEnKillfocusAddbuidingSetedit()
{

}

void CAddBuilding::OnCbnSelchangeAddbuidingSelectcombo()
{

}

void CAddBuilding::OnCbnKillfocusAddbuidingSelectcombo()
{
	m_AddBuiding_SetComBox.ShowWindow(SW_HIDE);
}
void CAddBuilding::Update_Recorder()
{
	CString strSql;
	strSql.Format(_T("update Building set Main_BuildingName='%s',Building_Name='%s',Protocal='%s',Ip_Address='%s',Ip_Port='%s',Com_Port='%s',Braudrate='%s' where Building_Name='%s' and Main_BuildingName='%s'"),m_strMainBuildingName,m_strBuildingName,m_strProtocol,m_strIpAddress,m_strIpPort,m_strComPort,m_strBaudrat,m_strBuilding_Name2,m_strMainBuildingName2);
	//m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);	
	
	m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
	if(m_nCurCol==AB_NAME)
	{
		m_strBuilding_Name2=m_strBuildingName;
	}
	if(m_pRs->State) 
		m_pRs->Close(); 	
	ReloadAddBuildingDB();
}

void CAddBuilding::OnBnClickedExit()
{
	// TODO: Add your control notification handler code here

	CDialog::OnOK();
}
void CAddBuilding::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
		
	OnOK();
}
void CAddBuilding::OnEnSetfocusAddbuidingSetedit()
{

	// TODO: Add your control notification handler code here

}

BOOL CAddBuilding::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::PreTranslateMessage(pMsg);
}





void CAddBuilding::SetBuildingMainName(CString strBuildName)
{
	m_strMainBuildingName=strBuildName;
}

void CAddBuilding::OnMainBuildingSelect()
{


}
void CAddBuilding::OnMainBuildingUnSelect()
{


}
void CAddBuilding::OnBuildingDelete()
{


}
void CAddBuilding::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	
	CDialog::PostNcDestroy();
}

void CAddBuilding::OnStnClickedStatic1()
{
	// TODO: Add your control notification handler code here
}

void CAddBuilding::OnCbnSelendokAddbuidingSelectcombo()
{
	CString strText;
	int nIndext=m_AddBuiding_SetComBox.GetCurSel();
	m_AddBuiding_SetComBox.GetLBText(nIndext,strText);

	if(m_nCurCol==AB_PROTOCOL)
	{
		m_strProtocol=strText;
	}
	if(m_nCurCol==AB_COMPORT)
	{
		m_strComPort=strText;
	}
	if(m_nCurCol==AB_BAUDRAT)
	{
		m_strBaudrat=strText;
	}

	if (m_nCurRow==m_AddBuiding_FlexGrid.get_Rows()-1)
	{
		
		m_AddBuiding_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
	}
	else
	{
		Update_Recorder();

	}
	m_AddBuiding_SetComBox.ShowWindow(SW_HIDE);
//	AfxMessageBox(m_strBuilding_Name2);

}

void CAddBuilding::OnEnKillfocusMainbuildedit()
{
	
}

void CAddBuilding::OnEnSetfocusMainbuildedit()
{
	
}

void CAddBuilding::DblClickAddbuildingMsflexgrid()
{

}





void CAddBuilding::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent==1)
	{
		GetSystemTime(&time2);
		if(((time2.wMinute*60+time2.wSecond)-(time1.wMinute*60+time1.wSecond))>=15)
		{
			m_NCScanBtn.EnableWindow(TRUE);
			m_SaveBtn.EnableWindow(TRUE);
			m_bRunningThread=FALSE;
			if(pNCScanThread!=NULL)
			{
				DWORD dwExidCode;

				GetExitCodeThread(pNCScanThread->m_hThread,&dwExidCode);
				if(dwExidCode==STILL_ACTIVE)
				TerminateThread(pNCScanThread->m_hThread,dwExidCode);
				pNCScanThread=NULL;
			}
			KillTimer(1);
		}
	}
	CDialog::OnTimer(nIDEvent);
}


BOOL CAddBuilding::GetIPbyHostName(CString strHostName,CString strIP)
{

	return TRUE;
}
void CAddBuilding::OnBnClickedSavebutton()
{

}



void CAddBuilding::SaveAll()
{

}



	
BOOL CAddBuilding::IsValidCOM(const CString& strComPort)
{
	for (UINT i = 0; i < m_szComm.size(); i++)
	{
		CString strTemp = m_szComm[i];
		if(strTemp.CompareNoCase(strComPort) ==0)
		{
			return TRUE;
		}
	}

	return FALSE;
	
}



void CAddBuilding::OnBnClickedAddbuiding()
{

}