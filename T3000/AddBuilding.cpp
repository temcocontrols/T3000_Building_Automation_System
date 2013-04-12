// AddBuilding.cpp : implementation file
//
#include "stdafx.h"
#include "T3000.h"
#include "MainFrm.h"

#include "AddBuilding.h"

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
	ON_BN_CLICKED(IDC_SCANNCBTN, &CAddBuilding::OnBnClickedScanncbtn)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SAVEBUTTON, &CAddBuilding::OnBnClickedSavebutton)
END_MESSAGE_MAP()


// CAddBuilding message handlers
/* 2011-10-08; 因为老毛要自动填入，所以注释掉，换另一个版本
void CAddBuilding::OnBnClickedAddbuiding()
{
	CString strSql;
	int nMaxRowIndext=m_AddBuiding_FlexGrid.get_Rows()-1;

	CString strMainBuildName=_T("222");	
	CString strSubBuildingName=_T("333");

	strMainBuildName=m_AddBuiding_FlexGrid.get_TextMatrix(nMaxRowIndext,AB_MAINNAME);
	if(strMainBuildName.IsEmpty())
	{
		AfxMessageBox(_T("Input the building info in the last row of grid, the building and sub building NAME can not be empty."));
		return ;
	}

	strSubBuildingName=m_AddBuiding_FlexGrid.get_TextMatrix(nMaxRowIndext,AB_NAME);
	if(strSubBuildingName.IsEmpty())
	{
		AfxMessageBox(_T("The new sub net's NAME can not be empty, please input."));
		return ;
	}
	for(UINT i=0;i<m_BuildNameLst.size();i++)
	{
		if(strSubBuildingName.CollateNoCase(m_BuildNameLst.at(i).strSubBuildName)==0&&strMainBuildName.CompareNoCase(m_BuildNameLst.at(i).strMainBuildName)==0)
		{
			AfxMessageBox(_T("There is already sub net in Building , please change it."));
			return;
		}			
	}


	CString strProtocol;
	strProtocol=m_AddBuiding_FlexGrid.get_TextMatrix(nMaxRowIndext,AB_PROTOCOL);
	if(strProtocol.IsEmpty())
	{
		AfxMessageBox(_T("The new sub net's PROTOCOL can not be empty, please input."));
		return ;
	}
	
	CString strIP;
	strIP=m_AddBuiding_FlexGrid.get_TextMatrix(nMaxRowIndext,AB_IPADDRESS);
	if(strProtocol.CompareNoCase(_T("Modbus TCP"))==0)
	{
		if(strIP.IsEmpty())
		{
			AfxMessageBox(_T("The new sub net's Ip address can not be empty, please input."));
			return ;
		}
	}
	if(strProtocol.CompareNoCase(_T("Modbus 485"))==0)
	{
		if(strIP.IsEmpty())
			strIP=_T("N/A");
	}
	
	CString strIpPort;
	strIpPort=m_AddBuiding_FlexGrid.get_TextMatrix(nMaxRowIndext,AB_IPPORT);
	strIpPort = _T("N/A");
	if(strProtocol.CompareNoCase(_T("Modbus TCP"))==0)
	{
		if(strIpPort.IsEmpty())
		{
			strIpPort=_T("6001");
		}
	}
	if(strProtocol.CompareNoCase(_T("Modbus 485"))==0)
	{
		if(strIP.IsEmpty())
			strIpPort=_T("N/A");
	}
//	#define	AB_COMPORT	6
//#define	AB_BAUDRAT	7
	CString strCOMPort;
	strCOMPort=m_AddBuiding_FlexGrid.get_TextMatrix(nMaxRowIndext,AB_COMPORT);
	if(strCOMPort.IsEmpty())
	{
		strCOMPort=_T("COM1");
	}
	CString strCOMPortBraud;
	strCOMPortBraud=m_AddBuiding_FlexGrid.get_TextMatrix(nMaxRowIndext,AB_BAUDRAT);
	if(strCOMPortBraud.IsEmpty())
	{
		strCOMPortBraud=_T("19200");
	}
	BOOL bDefault =FALSE;
	strSql.Format(_T("insert into Building (Main_BuildingName,Building_Name,Protocal,Com_Port,Ip_Address,Ip_Port,Braudrate) values('"+strMainBuildName+"','"+strSubBuildingName+"','"+strProtocol+"','"+strCOMPort+"','"+strIP+"','"+strIpPort+"','"+strCOMPortBraud+"')"));
	m_pCon->Execute(strSql.GetString(),NULL,adCmdText);

	Update_Recorder();
}
*/

/*
void CAddBuilding::OnBnClickedDelbuilding()
{

	if(m_nCurRow>=(m_AddBuiding_FlexGrid.get_Rows()-1) || m_nCurRow <1 )
	{
		AfxMessageBox(_T("Please select a sub net first!"));
		return;
	}
	CString strSql;
	strSql.Format(_T("delete * from Building where Building_Name='%s' and Main_BuildingName='%s'"),m_strBuilding_Name2,m_strMainBuildingName);

	
	CString strTemp;
	strTemp.Format(_T("Are you sure to delete the sub net:'%s'"),m_strBuilding_Name2);
	if(AfxMessageBox(strTemp,MB_OKCANCEL)==IDOK)
	{
		m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
	}
	//Update_Recorder();
	ReloadAddBuildingDB();
	
}
*/

/*
void CAddBuilding::OnBnClickedDeleteallbuilding()
{
	CString strSql;
	strSql.Format(_T("delete * from Building where Main_BuildingName='%s'"),m_strMainBuildingName);
	CString strTemp;
	strTemp.Format(_T("Are you sure to delete all the sub net(s)"));
	if(AfxMessageBox(strTemp,MB_OKCANCEL)==IDOK)
	{
		m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
	}
	Update_Recorder();

}
*/

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
	lRow = m_AddBuiding_FlexGrid.get_RowSel();//获取点击的行号	
	lCol = m_AddBuiding_FlexGrid.get_ColSel(); //获取点击的列号

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
	m_AddBuiding_FlexGrid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	CDC* pDC =GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_AddBuiding_FlexGrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_AddBuiding_FlexGrid.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_AddBuiding_FlexGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_AddBuiding_FlexGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rcCell(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rcCell.OffsetRect(rect.left+1,rect.top+1);
	ReleaseDC(pDC);
	CString strValue = m_AddBuiding_FlexGrid.get_TextMatrix(lRow,lCol);


	if(0==lCol)
	{
		//ID_BUILDINGSEL_SELECT
		CMenu menu;
		menu.LoadMenu(IDR_BUILDINGSELE_MENU);
		CMenu *pmenu=menu.GetSubMenu(0);
		CPoint point;
		GetCursorPos(&point);
		pmenu->TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN ,point.x,point.y,this);	

	}
	if(lCol==AB_MAINNAME)
	{
		m_mainBuildEdt.MoveWindow(&rcCell,1);
		m_mainBuildEdt.ShowWindow(SW_SHOW);
		m_mainBuildEdt.SetWindowText(strValue);
		m_mainBuildEdt.SetFocus();
		int nLenth=strValue.GetLength();
		m_mainBuildEdt.SetSel(0,nLenth); //全选//
	}
	if(AB_NAME==lCol||AB_IPADDRESS==lCol||AB_IPPORT==lCol)
	{
		//m_AddBuiding_FlexGrid.get_RowPos(lRow);
		m_AddBuiding_SetEditCtrl.MoveWindow(&rcCell,1);
		m_AddBuiding_SetEditCtrl.ShowWindow(SW_SHOW);
		m_AddBuiding_SetEditCtrl.SetWindowText(strValue);
		m_AddBuiding_SetEditCtrl.SetFocus();
		int nLenth=strValue.GetLength();
		m_AddBuiding_SetEditCtrl.SetSel(0,nLenth); //全选//
	}

	/*
	if(AB_CONNECTION==lCol)
	{
		m_AddBuiding_SetComBox.ResetContent();
		m_AddBuiding_SetComBox.InsertString(0,_T("COM Port"));
		m_AddBuiding_SetComBox.InsertString(1,_T("IP（VIA NC)"));
		m_AddBuiding_SetComBox.InsertString(2,_T("IP(Direct)"));
		m_AddBuiding_SetComBox.MoveWindow(&rcCell,1); //移动到选中格的位置
		m_AddBuiding_SetComBox.BringWindowToTop();
		m_AddBuiding_SetComBox.ShowWindow(SW_SHOW);//显示控件
		m_AddBuiding_SetComBox.SetFocus(); //获取焦点
	}
	*/
	if(AB_PROTOCOL==lCol)
	{
		m_AddBuiding_SetComBox.ResetContent();
		m_AddBuiding_SetComBox.InsertString(0,_T("Modbus 485"));
		m_AddBuiding_SetComBox.InsertString(1,_T("Modbus TCP"));
		m_AddBuiding_SetComBox.InsertString(2,_T("Bacnet"));
		m_AddBuiding_SetComBox.MoveWindow(&rcCell,1); //移动到选中格的位置
		m_AddBuiding_SetComBox.BringWindowToTop();
		m_AddBuiding_SetComBox.ShowWindow(SW_SHOW);//显示控件
		m_AddBuiding_SetComBox.SetFocus(); //获取焦点
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
		m_AddBuiding_SetComBox.MoveWindow(&rcCell,1); //移动到选中格的位置
		m_AddBuiding_SetComBox.BringWindowToTop();
		m_AddBuiding_SetComBox.ShowWindow(SW_SHOW);//显示控件
		m_AddBuiding_SetComBox.SetFocus(); //获取焦点
	}
	if(AB_BAUDRAT==lCol)
	{
		m_AddBuiding_SetComBox.ResetContent();
		m_AddBuiding_SetComBox.AddString(_T("9600"));
		m_AddBuiding_SetComBox.AddString(_T("19200"));
		m_AddBuiding_SetComBox.MoveWindow(&rcCell,1); //移动到选中格的位置
	//	m_AddBuiding_SetComBox.BringWindowToTop();
		m_AddBuiding_SetComBox.ShowWindow(SW_SHOW);//显示控件
		m_AddBuiding_SetComBox.SetFocus(); //获取焦点
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

		/*
		temp_variant=m_pRs->GetCollect("ConnectType");//
		if(temp_variant.vt!=VT_NULL)
			m_strConnetion=temp_variant;
		else
			m_strConnetion=_T("");
		m_AddBuiding_FlexGrid.put_TextMatrix(temp_row,AB_CONNECTION,m_strConnetion);

		*/

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
#if 1
		CString strText;
		m_AddBuiding_SetEditCtrl.GetWindowText(strText);
		m_strMainBuildingName = m_AddBuiding_FlexGrid.get_TextMatrix(m_nCurRow, AB_MAINNAME);
		if (m_nCurRow==m_AddBuiding_FlexGrid.get_Rows()-1)
		{
			if(m_nCurCol==AB_NAME)
			{
				CString strTemp;
				//for(UINT i=0;i++;i<m_BuildNameLst.size())  // commented by zgq;2010-12-06,不知道这种写法是什么意思？				
				for(UINT i=0;i<m_BuildNameLst.size();i++) 
				{
					strTemp=m_BuildNameLst.at(i).strSubBuildName;
					if(strText.CompareNoCase(strTemp)==0 && m_BuildNameLst.at(i).strMainBuildName.CompareNoCase(m_strMainBuildingName)==0) 
					{
						AfxMessageBox(_T("The Subnet Name has existed, please input a unused name."));
						return;
					}
				}

				m_AddBuiding_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strText);

				m_strBuildingName=strText;
				m_strBuilding_Name2=strText;
			}
			
			if(m_nCurCol==AB_IPADDRESS)
			{
				if(!ValidAddress(strText))
				{
					if(strText.IsEmpty())
					{
						AfxMessageBox(_T("Please input a validate IP adreess or domain name!"));
						return;
					}
					/*
					CStringIP;
					if(!GetIPbyHostName(strText,CStringIP))
					{
						AfxMessageBox(_T("Can not get a validate IP adreess from the domain name!"));
						return;
					}
					m_strIpAddress=CStringIP;
					m_AddBuiding_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
					return;
					*/

					
				}
				//if(m_strIpAddress.CompareNoCase(strText)==0)
				//{
				//	return;
				//}
				m_strIpAddress=strText;
				m_AddBuiding_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
			}


			if(m_nCurCol==AB_IPPORT)
			{
				//if(m_strIpPort.CompareNoCase(strText)==0)
				//{
				//	return;
				//}
				m_strIpPort=strText;
				m_AddBuiding_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
			}			
		}
		else
		{		
			if(m_nCurCol==AB_NAME && m_strBuilding_Name2.CompareNoCase(strText)!=0)
			{
				CString strTemp;
				for(UINT i=0;i<m_BuildNameLst.size();i++)
				{
					strTemp=m_BuildNameLst.at(i).strSubBuildName;
					if(strText.CompareNoCase(strTemp)==0&&m_strMainBuildingName2.CompareNoCase(m_BuildNameLst.at(i).strMainBuildName)==0)
					{
						AfxMessageBox(_T("The building Name has exist, please change another one."));
						return;
					}
				}
				/*Good Code//using Format is OK, use strSql= Error..??
				CString strSql;
				//strSql.Format(_T("update Building set Building_Name = '%s', where Building_Name = '%s'"),strText,m_strBuildingName);//erro
				 strSql.Format(_T("update Building set Building_Name = '"+strText+"'  where Building_Name =  '"+m_strBuildingName+"'"));
				_pCon->Execute(strSql.GetString(),NULL,adCmdText);	
				m_strBuildingName=strText;
				*/
				m_strBuildingName=strText;
				
			}
			if(m_nCurCol==AB_IPADDRESS)
			{
		
				if(!ValidAddress(strText))
				{
					
					//if(strText.CompareNoCase((CString)NO_APPLICATION)==0)
					//return;

					if(strText.IsEmpty())
					{
						AfxMessageBox(_T("Please input a validate IP adreess or domain name!"));
						return;
					}
				
					/*
					CStringIP;
					if(!GetIPbyHostName(strText,CStringIP))
					{
						AfxMessageBox(_T("Can not get a validate IP adreess from the domain name!"));
						return;
					}
					m_strIpAddress=CStringIP;
					m_AddBuiding_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
					*/
					//return;
				}
				//if(m_strIpAddress.CompareNoCase(strText)==0)
				//{
				//	return;
				//}
				m_strIpAddress=strText;
			}

			if(m_nCurCol==AB_IPPORT)
			{
				//if(m_strIpPort.CompareNoCase(strText)==0)
				//{
				//	return;
				//}
				m_strIpPort=strText;
			}
			Update_Recorder();
		}
	
		m_AddBuiding_SetEditCtrl.ShowWindow(SW_HIDE);
#endif
}

void CAddBuilding::OnCbnSelchangeAddbuidingSelectcombo()
{
	/*
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
	*/
}

void CAddBuilding::OnCbnKillfocusAddbuidingSelectcombo()
{
	m_AddBuiding_SetComBox.ShowWindow(SW_HIDE);
}
void CAddBuilding::Update_Recorder()
{
	try
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

	}
	catch (...)
	{

	}


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

		SaveAll();//scan，常要特意选择一个COM，即使看到的是正确也要选一下，否则就说串口号不对
// 		CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
// 		pMain->Treestatus();
//	OnOK();
}
void CAddBuilding::OnEnSetfocusAddbuidingSetedit()
{

	// TODO: Add your control notification handler code here

}

BOOL CAddBuilding::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->wParam == VK_RETURN )
	{ 
		GetDlgItem(ID_EXIT)->SetFocus();
		return true; 
	} 
	return CDialog::PreTranslateMessage(pMsg);
}



BOOL CAddBuilding::ValidAddress(CString sAddress,UINT& n1,UINT& n2,UINT& n3,UINT& n4)
{
	int nPos;
	CString sTemp=sAddress;
	n1=_wtoi(sTemp);
	nPos=sTemp.Find(_T("."));
	if(nPos==-1) return false;
	sTemp=sTemp.Mid(nPos+1);

	n2=_wtoi(sTemp);
	nPos=sTemp.Find(_T("."));
	if(nPos==-1) return false;
	sTemp=sTemp.Mid(nPos+1);

	n3=_wtoi(sTemp);
	nPos=sTemp.Find(_T("."));
	if(nPos==-1) return false;
	sTemp=sTemp.Mid(nPos+1);

	n4=_wtoi(sTemp);

	if(n1<0 ||n1>255) return false;
	if(n2<0 ||n2>255) return false;
	if(n3<0 ||n3>255) return false;
	if(n4<0 ||n4>255) return false;

	return true;
}
BOOL CAddBuilding::ValidAddress(CString sAddress)
{
	int nPos;
	UINT n1,n2,n3,n4;
	CString sTemp=sAddress;
	n1=_wtoi(sTemp);
	nPos=sTemp.Find(_T("."));
	if(nPos==-1) return false;
	sTemp=sTemp.Mid(nPos+1);

	n2=_wtoi(sTemp);
	nPos=sTemp.Find(_T("."));
	if(nPos==-1) return false;
	sTemp=sTemp.Mid(nPos+1);
	n3=_wtoi(sTemp);
	nPos=sTemp.Find(_T("."));
	if(nPos==-1) return false;
	sTemp=sTemp.Mid(nPos+1);
	n4=_wtoi(sTemp);
	if(n1<0 ||n1>255) return false;
	if(n2<0 ||n2>255) return false;
	if(n3<0 ||n3>255) return false;
	if(n4<0 ||n4>255) return false;
	return TRUE;
}

void CAddBuilding::SetBuildingMainName(CString strBuildName)
{
	m_strMainBuildingName=strBuildName;
}

void CAddBuilding::OnMainBuildingSelect()
{
try
{

	CString execute_str=_T("update Building set Default_SubBuilding = 0 where Default_SubBuilding = -1");
	m_pCon->Execute(execute_str.GetString(),NULL,adCmdText);	
	execute_str.Format(_T("update Building set Default_SubBuilding = -1 where Building_Name = '%s'and Main_BuildingName= '%s'"),m_strBuildingName,m_strMainBuildingName);
	m_pCon->Execute(execute_str.GetString(),NULL,adCmdText);
}
catch (...)
{

}

	ReloadAddBuildingDB();
}
void CAddBuilding::OnMainBuildingUnSelect()
{
try
{

	CString execute_str=_T("update Building set Default_SubBuilding = 0 where Default_SubBuilding = -1");
	m_pCon->Execute(execute_str.GetString(),NULL,adCmdText);	
}
catch (...)
{

}

	ReloadAddBuildingDB();
}
void CAddBuilding::OnBuildingDelete()
{
	if(m_nCurRow>=(m_AddBuiding_FlexGrid.get_Rows()-1) || m_nCurRow <1 )
	{
		return;
	}
	CString strSql;
	strSql.Format(_T("delete * from Building where Building_Name='%s' and Main_BuildingName='%s'"),m_strBuilding_Name2,m_strMainBuildingName);
try
{


	CString strTemp;
	strTemp.Format(_T("Are you sure to delete the building:'%s->%s'"),m_strMainBuildingName,m_strBuilding_Name2);
	if(AfxMessageBox(strTemp,MB_OKCANCEL)==IDOK)
	{
		m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
	}
}
catch (...)
{

}
	ReloadAddBuildingDB();

}
void CAddBuilding::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	if(pNCScanThread!=NULL)
	{
		DWORD dwExidCode;
		GetExitCodeThread(pNCScanThread->m_hThread,&dwExidCode);
		if(dwExidCode==STILL_ACTIVE)
		TerminateThread(pNCScanThread->m_hThread,dwExidCode);
		pNCScanThread=NULL;
	}
	
	::EnterCriticalSection(&g_Lock);
	if(g_NCList.size()>0)
	{
		g_NCList.clear();
	}	
	::LeaveCriticalSection(&g_Lock);

	::DeleteCriticalSection(&g_Lock);
	Sleep(150);
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	::PostMessage(pFrame->m_hWnd,WM_MYMSG_REFRESHBUILDING,0,0);

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
	CString strText;
	m_mainBuildEdt.GetWindowText(strText);
	if(strText.IsEmpty())
	{
		m_mainBuildEdt.ShowWindow(SW_HIDE);
		return;
	}

	if (m_nCurRow==m_AddBuiding_FlexGrid.get_Rows()-1)
	{
		CString strTemp;
		for(UINT i=0;i<m_BuildNameLst.size();i++)
		{
			strTemp=m_BuildNameLst.at(i).strMainBuildName;
			if(strText.CompareNoCase(strTemp)==0&&m_strBuilding_Name2.CompareNoCase(m_BuildNameLst.at(i).strSubBuildName)==0)
			{
				AfxMessageBox(_T("The building Name has exist, please change another one."));
					m_mainBuildEdt.ShowWindow(SW_HIDE);
				return;
			}
		}
		m_AddBuiding_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
	}
	else
	{
		if(m_nCurCol==AB_MAINNAME && m_strMainBuildingName2.CompareNoCase(strText)!=0)
		{
			CString strTemp;
			for(UINT i=0;i<m_BuildNameLst.size();i++)
			{
				strTemp=m_BuildNameLst.at(i).strMainBuildName;
				if(strText.CompareNoCase(strTemp)==0&&m_strBuilding_Name2.CompareNoCase(m_BuildNameLst.at(i).strSubBuildName)==0)
				{
					AfxMessageBox(_T("The building Name has exist, please change another one."));
						m_mainBuildEdt.ShowWindow(SW_HIDE);
					return;
				}
			}
			m_AddBuiding_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
			m_strMainBuildingName=strText;				
			Update_Recorder();
		}
	}
	m_mainBuildEdt.ShowWindow(SW_HIDE);
}

void CAddBuilding::OnEnSetfocusMainbuildedit()
{
	
}

void CAddBuilding::DblClickAddbuildingMsflexgrid()
{
	return;
	long lRow,lCol;
	lRow = m_AddBuiding_FlexGrid.get_RowSel();	//获取点击的行号	
	lCol = m_AddBuiding_FlexGrid.get_ColSel();	//获取点击的列号

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
		m_strMainBuildingName=m_strMainBuildingName2;
	}
	m_strProtocol=m_AddBuiding_FlexGrid.get_TextMatrix(m_nCurRow,AB_PROTOCOL);
	m_strIpAddress=m_AddBuiding_FlexGrid.get_TextMatrix(m_nCurRow,AB_IPADDRESS);
	m_strIpPort=m_AddBuiding_FlexGrid.get_TextMatrix(m_nCurRow,AB_IPPORT);
	m_strComPort=m_AddBuiding_FlexGrid.get_TextMatrix(m_nCurRow,AB_COMPORT);
	m_strBaudrat=m_AddBuiding_FlexGrid.get_TextMatrix(m_nCurRow,AB_BAUDRAT);
	
	OnMainBuildingSelect();
}
/*
This is interface between T3000 and machine for UDP broadcast feature:

 

#define UPD_BROADCAST_QRY_MSG 100

#define RESPONSE_MSG              UPD_BROADCAST_QRY_MSG+1

#define UDP_BROADCAST_PORT    1234

struct

{

 ubyte msg;//RESPONSE_MSG

 UBYTE lenth;

 char* pNCInfo;

}NC_Info;

//serial 

pNCInfo[0]-reg0;

pNCInfo[1]-reg1;

pNCInfo[2]-reg2;

NCInfo[3]-reg3;

NCInfo[4]-reg4;//product Type

//moudbus address
pNCInfo[5]-reg6;



//ipaddress;
pNCInfo[6]-reg107;

pNCInfo[7]-reg108;

pNCInfo[8]-reg109;

pNCInfo[9]-reg110;

//tcp port.

pNCInfo[10]-reg120;

when receiving the message UPD_BROADCAST_QRY_MSG from T3000,

send the data package filled with the registers value and msgtype to T3000.
*/

/*
#define UPD_BROADCAST_QRY_MSG 100
#define RESPONSE_MSG          UPD_BROADCAST_QRY_MSG+1
#define UDP_BROADCAST_PORT    1234
typedef struct infopack
{
 unsigned char msg;//RESPONSE_MSG
 unsigned char lenth;
 unsigned short* pNCInfo;
}NC_Info;

*/
UINT _BroadCastScanNCFun(LPVOID pParam)
{
	CAddBuilding* pDlg=(CAddBuilding*) pParam;
	IP_ADAPTER_INFO pAdapterInfo;
	ULONG len = sizeof(pAdapterInfo); 
	if(GetAdaptersInfo(&pAdapterInfo, &len) != ERROR_SUCCESS) 
	{
		return 0;
	}
//SOCKADDR_IN sockAddress;   // commented by zgq;2010-12-06; unreferenced local variable
	UINT nGatewayIP,nLocalIP,nMaskIP;
	nGatewayIP=inet_addr(pAdapterInfo.GatewayList.IpAddress.String);
	nLocalIP=inet_addr(pAdapterInfo.IpAddressList.IpAddress.String);
	nMaskIP=inet_addr(pAdapterInfo.IpAddressList.IpMask.String);
	UINT nBroadCastIP;
	nBroadCastIP=(~nMaskIP)|nLocalIP;
	char* chBroadCast;
	in_addr in;
	in.S_un.S_addr=nBroadCastIP;
	chBroadCast=inet_ntoa(in);
	hBroad=::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	BOOL bBroadcast=TRUE;
	::setsockopt(hBroad,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));
	int iMode=1;
	ioctlsocket(hBroad,FIONBIO, (u_long FAR*) &iMode);

	SOCKADDR_IN bcast;
	bcast.sin_family=AF_INET;
	bcast.sin_addr.s_addr=nBroadCastIP;
	bcast.sin_port=htons(UDP_BROADCAST_PORT);
	short nmsgType=UPD_BROADCAST_QRY_MSG;
	::sendto(hBroad,(char*)&nmsgType,sizeof(short),0,(sockaddr*)&bcast,sizeof(bcast));

	Sleep(10);
	closesocket(hBroad);
	hBroad=NULL;

	sListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sListen == INVALID_SOCKET)
	{
		printf("Failed socket() \n");
		return 0;
	}
	iMode=1;
	ioctlsocket(sListen,FIONBIO, (u_long FAR*) &iMode);
	int nTimeout=1000;
	BOOL bReuse=TRUE;
	::setsockopt(sListen,SOL_SOCKET,SO_REUSEADDR,(char*)&bReuse,sizeof(BOOL));
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(4321);
 	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if(::bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("Failed bind() \n");
		return 0;
	}
	if(::listen(sListen, 5) == SOCKET_ERROR)
	{
		printf("Failed listen() \n");
		return 0;
	}
	fd_set fdSocket;
	FD_ZERO(&fdSocket);
	FD_SET(sListen, &fdSocket);
	int k=0;
	while(pDlg->m_bRunningThread)
	{
		fd_set fdRead = fdSocket;
		int nRet = ::select(0, &fdRead, NULL, NULL, NULL);
		if(nRet > 0)
		{
			fd_set fdRead = fdSocket;
			int nRet = ::select(0, &fdRead, NULL, NULL, NULL);
			if(nRet > 0)
			{
	
				for(int i=0; i<(int)fdSocket.fd_count; i++)
				{
					if(FD_ISSET(fdSocket.fd_array[i], &fdRead))
					{
						if(fdSocket.fd_array[i] == sListen)		// （1）监听套节字接收到新连接
						{
							if(fdSocket.fd_count < FD_SETSIZE)
							{
								sockaddr_in addrRemote;
								int nAddrLen = sizeof(addrRemote);
								SOCKET sNew = ::accept(sListen, (SOCKADDR*)&addrRemote, &nAddrLen);
								FD_SET(sNew, &fdSocket);
							}
							else
							{
								continue;
							}
						}
						else
						{
							unsigned short buffer[256];
							int nRecv = ::recv(fdSocket.fd_array[i], (char*)buffer, 256, 0);
							if(nRecv > 0)						// （2）可读
							{			
								if(buffer[0]==RESPONSE_MSG)
								{
									int nLen=buffer[1];
									if(nLen>=0)
									{
										k++;
										unsigned short dataPackage[11];
										memcpy(&dataPackage[0],&buffer[2],nLen*sizeof(unsigned short));
										_NCInfo ncInfo;
										ncInfo.nSerial=dataPackage[0]+dataPackage[1]*256+dataPackage[2]*256*256+dataPackage[3]*256*256*256;
										ncInfo.nProductID=dataPackage[4];
										ncInfo.modbusID=dataPackage[5];
										CString strTemp;
										strTemp.Format(_T("%d.%d.%d.%d"),dataPackage[6],dataPackage[7],dataPackage[8],dataPackage[9]);
										ncInfo.strIP=strTemp;
										ncInfo.nPort=(dataPackage[10]);
										::EnterCriticalSection(&g_Lock);
										g_NCList.push_back(ncInfo);
									
										pDlg->m_NCFlexGrid.put_Rows(g_NCList.size()+1);
										strTemp.Format(_T("%d"),ncInfo.nSerial);
										pDlg->m_NCFlexGrid.put_TextMatrix(k,0,strTemp);

										strTemp.Format(_T("%d"),ncInfo.modbusID);
										pDlg->m_NCFlexGrid.put_TextMatrix(k,1,strTemp);
										
										strTemp.Format(_T("%d"),ncInfo.nProductID);
										pDlg->m_NCFlexGrid.put_TextMatrix(k,2,strTemp);

										pDlg->m_NCFlexGrid.put_TextMatrix(k,3,ncInfo.strIP);
										strTemp.Format(_T("%d"),ncInfo.nPort);
										pDlg->m_NCFlexGrid.put_TextMatrix(k,4,strTemp);
										::LeaveCriticalSection(&g_Lock);
									}
								}
							}
							else								
							{
								::closesocket(fdSocket.fd_array[i]);
								FD_CLR(fdSocket.fd_array[i], &fdSocket);
							}
						}
					}
				}
			}
		}///end of nret

	}//end of while
	closesocket(sListen);
	sListen=NULL;
	return 1;

}

// UDP 广播模式查找网络设备，不仅仅是NC，也有其他
UINT _BroadCastScanNCFunByUDP(LPVOID pParam)
{
///*
	CAddBuilding* pDlg=(CAddBuilding*) pParam;
	IP_ADAPTER_INFO pAdapterInfo;
	ULONG len = sizeof(pAdapterInfo); 
	if(GetAdaptersInfo(&pAdapterInfo, &len) != ERROR_SUCCESS) 
	{
		return 0;
	}
	//SOCKADDR_IN sockAddress;
	UINT nGatewayIP,nLocalIP,nMaskIP;
	nGatewayIP=inet_addr(pAdapterInfo.GatewayList.IpAddress.String);
	nLocalIP=inet_addr(pAdapterInfo.IpAddressList.IpAddress.String);
	nMaskIP=inet_addr(pAdapterInfo.IpAddressList.IpMask.String);
	UINT nBroadCastIP;
	nBroadCastIP=(~nMaskIP)|nLocalIP;
	char* chBroadCast;
	in_addr in;
	in.S_un.S_addr=nBroadCastIP;
	chBroadCast=inet_ntoa(in);
	SOCKET hBroad=::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	BOOL bBroadcast=TRUE;
	::setsockopt(hBroad,SOL_SOCKET,SO_BROADCAST,(char*)&bBroadcast,sizeof(BOOL));
	int iMode=1;
	ioctlsocket(hBroad,FIONBIO, (u_long FAR*) &iMode);//非阻塞设置
	SOCKADDR_IN bcast;
	bcast.sin_family=AF_INET;
	bcast.sin_addr.s_addr=nBroadCastIP;
	bcast.sin_port=htons(UDP_BROADCAST_PORT);
	short nmsgType=UPD_BROADCAST_QRY_MSG;
	::sendto(hBroad,(char*)&nmsgType,sizeof(short),0,(sockaddr*)&bcast,sizeof(bcast));
	// 接收广播
	SOCKADDR_IN addrRemote;	
	int nLen = sizeof(addrRemote);
	//unsigned short msg;
	//NC_Info info;
	//int nTimeout=1000;
	//BOOL bReuse=TRUE;
	//::setsockopt(hBroad,SOL_SOCKET,SO_REUSEADDR,(char*)&bReuse,sizeof(BOOL));
	SYSTEMTIME time1;
	SYSTEMTIME time2;
	GetSystemTime(&time1);
	GetSystemTime(&time2);
	int i=0;
	unsigned short buffer[50];
	while(TRUE)
	{	
		GetSystemTime(&time2);
	   if(((time2.wMinute*60+time2.wSecond)-(time1.wMinute*60+time1.wSecond))>=15)
		{
			pDlg->m_NCScanBtn.EnableWindow(TRUE);
			//return 0;
		}
		memset(buffer,0,sizeof(buffer));
		memset(&addrRemote,0,sizeof(addrRemote));
		int nsize=sizeof(buffer);
		int nRet = ::recvfrom(hBroad,(char*)&buffer[0], nsize, 0, (sockaddr*)&addrRemote, &nLen);
		if(nRet > 0)
		{
		//	::AfxMessageBox(_T("received data."));
			if(buffer[0]==RESPONSE_MSG)
			{
			//	::AfxMessageBox(_T("received 101 msg data package."));
				nLen=buffer[1];
				//int n =sizeof(char)+sizeof(unsigned char)+sizeof( unsigned short)*9;
				if(nLen>=0)
				{
					i++;
					unsigned short dataPackage[11];
					memcpy(&dataPackage[0],&buffer[2],nLen*sizeof(unsigned short));
					_NCInfo ncInfo;
					ncInfo.nSerial=dataPackage[0]+dataPackage[1]*256+dataPackage[2]*256*256+dataPackage[3]*256*256*256;
					ncInfo.nProductID=dataPackage[4];
					ncInfo.modbusID=dataPackage[5];
					CString strTemp;
					strTemp.Format(_T("%d.%d.%d.%d"),dataPackage[6],dataPackage[7],dataPackage[8],dataPackage[9]);
					ncInfo.strIP=strTemp;
					ncInfo.nPort=(dataPackage[10]);
					//ncInfo.strIP=
					::EnterCriticalSection(&g_Lock);
					g_NCList.push_back(ncInfo);
				
					pDlg->m_NCFlexGrid.put_Rows(g_NCList.size()+1);
					strTemp.Format(_T("%d"),ncInfo.nSerial);
					pDlg->m_NCFlexGrid.put_TextMatrix(i,0,strTemp);

					strTemp.Format(_T("%d"),ncInfo.modbusID);
					pDlg->m_NCFlexGrid.put_TextMatrix(i,1,strTemp);
					
					strTemp.Format(_T("%d"),ncInfo.nProductID);
					pDlg->m_NCFlexGrid.put_TextMatrix(i,2,strTemp);

					pDlg->m_NCFlexGrid.put_TextMatrix(i,3,ncInfo.strIP);
					strTemp.Format(_T("%d"),ncInfo.nPort);
					pDlg->m_NCFlexGrid.put_TextMatrix(i,4,strTemp);
					::LeaveCriticalSection(&g_Lock);
				}
			
			}
		}
		//Sleep(10);
	}
	return 0;
	//*/
}


void CAddBuilding::OnBnClickedScanncbtn()
{
	m_NCScanBtn.EnableWindow(FALSE);
	m_SaveBtn.EnableWindow(FALSE);
	m_bRunningThread=TRUE;
	if(pNCScanThread!=NULL)
	{
		DWORD dwExidCode;
		GetExitCodeThread(pNCScanThread->m_hThread,&dwExidCode);
		if(dwExidCode==STILL_ACTIVE)
		{
			TerminateThread(pNCScanThread->m_hThread,dwExidCode);
			pNCScanThread=NULL;
		}
	}

	::EnterCriticalSection(&g_Lock);
	if(g_NCList.size()>0)
	{
		g_NCList.clear();
	}
	if(sListen!=NULL)
		closesocket(sListen);
	if(hBroad!=NULL)
		closesocket(hBroad);
	//SetTimer(1,5000,NULL);
	::LeaveCriticalSection(&g_Lock);
	SetTimer(1,50,NULL);
	GetSystemTime(&time1);
	//pNCScanThread=AfxBeginThread(_BroadCastScanNCFun,this);
	pNCScanThread=AfxBeginThread(_BroadCastScanNCFunByUDP,this);
	pNCScanThread->m_bAutoDelete=FALSE;
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
	WSAData   wsdata; 
	WORD wVersionRequested=MAKEWORD(2,0);   
    int  err=WSAStartup(wVersionRequested,&wsdata);   
    hostent     *pHostent=NULL   ;   
    // const char   *addr;		 // commented by zgq;2010-12-06; unreferenced local variable
	// char   hostName[255];	// commented by zgq;2010-12-06; unreferenced local variable
    pHostent   =   gethostbyname(CW2A(strHostName));   
    if(pHostent==NULL)   
       return   false;  
    if(pHostent->h_addr_list==NULL)   
        return   false;   
    sockaddr_in   sa;   
    memcpy(&sa.sin_addr.s_addr,pHostent->h_addr_list[0],pHostent->h_length);     
	strIP.Format(_T("%d.%d.%d.%d"),sa.sin_addr.S_un.S_un_b.s_b1,sa.sin_addr.S_un.S_un_b.s_b2,sa.sin_addr.S_un.S_un_b.s_b3,sa.sin_addr.S_un.S_un_b.s_b4); 
	WSACleanup();
	return TRUE;
}
void CAddBuilding::OnBnClickedSavebutton()
{
	/*
	_ConnectionPtr srcConTmp;
	_RecordsetPtr srcRsTemp;
	srcConTmp.CreateInstance("ADODB.Connection");
	srcRsTemp.CreateInstance("ADODB.Recordset");
	CString strSourceDatabasefilepath=g_strDatabasefilepath;
	srcConTmp->Open(strSourceDatabasefilepath.GetString(),"","",adModeUnknown);
	srcRsTemp	=srcConTmp->OpenSchema(adSchemaTables);
	_variant_t	l_vDBTableName;
	_bstr_t	bstrTableType;
	vector<CString> tabNameVector;
	 while(!srcRsTemp->EndOfFile)   
	 {   
       l_vDBTableName = srcRsTemp->GetCollect("TABLE_NAME"); 
       bstrTableType  =srcRsTemp->GetCollect("TABLE_TYPE");   
       if((bstrTableType ==(_bstr_t)"TABLE")||(bstrTableType==(_bstr_t)("VIEWS")))  
	   {   
		  tabNameVector.push_back(l_vDBTableName);   
		}   
		 srcRsTemp->MoveNext();   
	}   
  	if(srcRsTemp->State)
		srcRsTemp->Close();
	for(int i=0;i<tabNameVector.size();i++)
	{
	  CString strtemp=tabNameVector.at(i);
	  if(strtemp.CompareNoCase(_T("Every_product"))==0)
	  {
		  break;
	  }
	}
	*/

/*
	CString temp_str=_T("select * from Building where Default_SubBuilding=-1");
	m_pRs->Open(_variant_t(temp_str),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);	
	if(m_pRs->GetRecordCount()<=0)
	{
		AfxMessageBox(_T("There is no default building,please select a building First."));
		return;
	}
	if(m_pRs->State)
		m_pRs->Close(); 
	_ConnectionPtr t_pCon;//for ado connection
	t_pCon.CreateInstance(_T("ADODB.Connection"));
	t_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

	CString strfloor_name;//
	CString strroom_name;//
	CString strproduct_name;
	CString strproduct_class_id;
	CString strproduct_id;
	CString strscreen_name;
	CString strbackground_bmp;
	CString strSerialnumber;
	CString strmainBuildName;
	CString strSubBuildName;
	CString strIPAddress;
	CString strIPPortNum;


	CString stemp;
	
	CString strBaudRate=_T("0");
	CString strHardVersion=_T("0");
	CString strSoftwareVersion=_T("");
	CString strEpSize("");
//	CString strIspVersion("");

	strmainBuildName=m_strMainBuildingName;
	strSubBuildName=m_strBuilding_Name2;

	for(int j=0;j<g_NCList.size();j++)
	{
		strfloor_name=_T("Floor_xx");
		strroom_name=_T("Room_xx");
		stemp=_T("WiFiTstat5E");
		strproduct_name.Format(_T("%s:%d--%d"),stemp,g_NCList.at(j).nSerial,g_NCList.at(j).modbusID);
		strproduct_class_id.Format(_T("%d"),g_NCList.at(j).nProductID);
		strproduct_id.Format(_T("%d"),g_NCList.at(j).modbusID);
        strscreen_name.Format(_T("Sceen(S:%d--%d)"),g_NCList.at(j).nSerial,g_NCList.at(j).modbusID);
		strbackground_bmp=_T("Clicking here to add a image...");
		strSerialnumber.Format(_T("%d"), g_NCList.at(j).nSerial);
		strIPAddress=g_NCList.at(j).strIP;
		strIPPortNum.Format(_T("%d"),g_NCList.at(j).nPort);

		CString strSql;
		strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,EPsize，IP_ADDRESS，IP_PORTNUMBER) values('"+strmainBuildName+"','"+strSubBuildName+"','"+strSerialnumber+"','"+strfloor_name+"','"+strroom_name+"','"+strproduct_name+"','"+strproduct_class_id+"','"+strproduct_id+"','"+strscreen_name+"','"+strBaudRate+"','"+strbackground_bmp+"','"+strHardVersion+"','"+strSoftwareVersion+"','"+strEpSize+"','"+strIPAddress+"','"+strIPPortNum+"')"));
		try
		{
			t_pCon->Execute(strSql.GetString(),NULL,adCmdText);
		}
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}
	}
	*/
}


//////////////////////////////////////////////////////////////////////////
//	Added by zgq; 2010-11-29;
//  把按钮Exit和Save功能合并
//
//
void CAddBuilding::SaveAll()
{
	CString temp_str=_T("select * from Building where Default_SubBuilding=-1");
	m_pRs->Open(_variant_t(temp_str),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);	
	if(m_pRs->GetRecordCount()<=0)
	{
		AfxMessageBox(_T("There is no default building,please select a building First."));
		return;
	}
	if(m_pRs->State)
		m_pRs->Close(); 
	_ConnectionPtr t_pCon;//for ado connection
	t_pCon.CreateInstance(_T("ADODB.Connection"));
	t_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

	CString strfloor_name;//
	CString strroom_name;//
	CString strproduct_name;
	CString strproduct_class_id;
	CString strproduct_id;
	CString strscreen_name;
	CString strbackground_bmp;
	CString strSerialnumber;
	CString strmainBuildName;
	CString strSubBuildName;
	CString strIPAddress;
	CString strIPPortNum;


	CString stemp;

	CString strBaudRate=_T("0");
	CString strHardVersion=_T("0");
	CString strSoftwareVersion=_T("");
	CString strEpSize("");
	//	CString strIspVersion("");

	strmainBuildName=m_strMainBuildingName;
	strSubBuildName=m_strBuilding_Name2;

	for(UINT j=0;j<g_NCList.size();j++)
	{
		strfloor_name=_T("Floor_xx");
		strroom_name=_T("Room_xx");
		stemp=_T("WiFiTstat5E");
		strproduct_name.Format(_T("%s:%d--%d"),stemp,g_NCList.at(j).nSerial,g_NCList.at(j).modbusID);
		strproduct_class_id.Format(_T("%d"),g_NCList.at(j).nProductID);
		strproduct_id.Format(_T("%d"),g_NCList.at(j).modbusID);
		strscreen_name.Format(_T("Sceen(S:%d--%d)"),g_NCList.at(j).nSerial,g_NCList.at(j).modbusID);
		strbackground_bmp=_T("Clicking here to add a image...");
		strSerialnumber.Format(_T("%d"), g_NCList.at(j).nSerial);
		strIPAddress=g_NCList.at(j).strIP;
		strIPPortNum.Format(_T("%d"),g_NCList.at(j).nPort);

		CString strSql;
		strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,EPsize，IP_ADDRESS，IP_PORTNUMBER) values('"+strmainBuildName+"','"+strSubBuildName+"','"+strSerialnumber+"','"+strfloor_name+"','"+strroom_name+"','"+strproduct_name+"','"+strproduct_class_id+"','"+strproduct_id+"','"+strscreen_name+"','"+strBaudRate+"','"+strbackground_bmp+"','"+strHardVersion+"','"+strSoftwareVersion+"','"+strEpSize+"','"+strIPAddress+"','"+strIPPortNum+"')"));
		//new nc// strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,EPsize，IP_ADDRESS，IP_PORTNUMBER) values('"+strmainBuildName+"','"+strSubBuildName+"','"+strSerialnumber+"','"+strfloor_name+"','"+strroom_name+"','"+strproduct_name+"','"+strproduct_class_id+"','"+strproduct_id+"','"+strscreen_name+"','"+strBaudRate+"','"+strbackground_bmp+"','"+strHardVersion+"','"+strSoftwareVersion+"','"+strEpSize+"','"+strIPAddress+"','"+strIPPortNum+"')"));
		try
		{
			t_pCon->Execute(strSql.GetString(),NULL,adCmdText);
		}
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}
	}
}


/*
BOOL CAddBuilding::GetSerialComm(vector<CString>& szComm)
{
	LPCTSTR strRegEntry = _T("HARDWARE\\DEVICEMAP\\SERIALCOMM\\");

	HKEY   hKey;   
	LONG   lReturnCode=0;   
	lReturnCode=::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strRegEntry, 0, KEY_READ, &hKey);   

	if(lReturnCode==ERROR_SUCCESS)   
	{  
		DWORD dwIndex = 0;
		WCHAR lpValueName[MAX_PATH];
		ZeroMemory(lpValueName, MAX_PATH);
		DWORD lpcchValueName = MAX_PATH; 
		DWORD lpReserved = 0; 
		DWORD lpType = REG_SZ; 
		BYTE		lpData[MAX_PATH]; 
		ZeroMemory(lpData, MAX_PATH);
		DWORD lpcbData = MAX_PATH;
		dwIndex = 0;
		while(RegEnumValue(	hKey, dwIndex, lpValueName, &lpcchValueName, 0, &lpType, lpData, &lpcbData ) != ERROR_NO_MORE_ITEMS)
		{   
			//TRACE("Registry's   Read!");   
			dwIndex++;

			lpcchValueName = MAX_PATH; 
			lpValueName[0] = '\0'; 

			WCHAR ch[MAX_PATH];
			ZeroMemory(ch, MAX_PATH);
			memcpy(ch, lpData, lpcbData);
			CString str = CString(ch);
			szComm.push_back(str);

			ZeroMemory(lpData, MAX_PATH);
			lpcbData = MAX_PATH;

		}   
		::RegCloseKey(hKey);   		   

		return TRUE;
	}
	
	return FALSE;   
}
*/

	
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
	CString strSql;
	int nMaxRowIndext=m_AddBuiding_FlexGrid.get_Rows()-1;

// 	CString strMBuildNameTemp;
// 	strMBuildNameTemp.Format(_T("Building_%d"), nMaxRowIndext);	
	CString strSBuildingNameTemp;
	strSBuildingNameTemp.Format(_T("Subnet_%d"), nMaxRowIndext);	

	CString strMainBuildName=m_AddBuiding_FlexGrid.get_TextMatrix(nMaxRowIndext,AB_MAINNAME);
	if(strMainBuildName.IsEmpty())
	{
		AfxMessageBox(_T("Input the building info in the last row of grid, the building and sub building NAME can not be empty."));
		return ;
	}

	CString strSubBuildingName=m_AddBuiding_FlexGrid.get_TextMatrix(nMaxRowIndext,AB_NAME);
	if(strSubBuildingName.IsEmpty())
	{
		//AfxMessageBox(_T("The new sub net's NAME can not be empty, please input."));
		//return ;
		strSubBuildingName = strSBuildingNameTemp;
	}
	for(UINT i=0;i<m_BuildNameLst.size();i++)
	{
		if(strSubBuildingName.CollateNoCase(m_BuildNameLst.at(i).strSubBuildName)==0&&strMainBuildName.CompareNoCase(m_BuildNameLst.at(i).strMainBuildName)==0)
		{
			AfxMessageBox(_T("There is already sub net in Building , please change it."));
			return;
		}			
	}


	CString strProtocol;
	strProtocol=m_AddBuiding_FlexGrid.get_TextMatrix(nMaxRowIndext,AB_PROTOCOL);
	if(strProtocol.IsEmpty())
	{
		//AfxMessageBox(_T("The new sub net's PROTOCOL can not be empty, please input."));
		//return ;
		strProtocol = _T("Modbus 485");

	}

	CString strIP;
	strIP=m_AddBuiding_FlexGrid.get_TextMatrix(nMaxRowIndext,AB_IPADDRESS);
	if(strProtocol.CompareNoCase(_T("Modbus TCP"))==0)
	{
		if(strIP.IsEmpty())
		{
			strIP = _T("192.168.0.3");
		}
	}
	if(strProtocol.CompareNoCase(_T("Modbus 485"))==0)
	{
		if(strIP.IsEmpty())
			strIP=_T("N/A");
	}

	CString strIpPort;
	strIpPort=m_AddBuiding_FlexGrid.get_TextMatrix(nMaxRowIndext,AB_IPPORT);
	if(strProtocol.CompareNoCase(_T("Modbus TCP"))==0)
	{
		if(strIpPort.IsEmpty())
		{
			strIpPort=_T("6001");
		}
	}
	if(strProtocol.CompareNoCase(_T("Modbus 485"))==0)
	{
		if(strIP.IsEmpty())
			strIpPort=_T("N/A");
	}
	//	#define	AB_COMPORT	6
	//#define	AB_BAUDRAT	7
	CString strCOMPort;
	strCOMPort=m_AddBuiding_FlexGrid.get_TextMatrix(nMaxRowIndext,AB_COMPORT);
	if(strCOMPort.IsEmpty())
	{
		if(m_szComm.size() > 0)
		{
			strCOMPort=m_szComm[0];
		}
		else
		{
			strCOMPort=_T("No Com");
		}
	}
	CString strCOMPortBraud;
	strCOMPortBraud=m_AddBuiding_FlexGrid.get_TextMatrix(nMaxRowIndext,AB_BAUDRAT);
	if(strCOMPortBraud.IsEmpty())
	{
		strCOMPortBraud=_T("19200");
	}

	try
	{
	BOOL bDefault =FALSE;
	strSql.Format(_T("insert into Building (Main_BuildingName,Building_Name,Protocal,Com_Port,Ip_Address,Ip_Port,Braudrate) values('"+strMainBuildName+"','"+strSubBuildingName+"','"+strProtocol+"','"+strCOMPort+"','"+strIP+"','"+strIpPort+"','"+strCOMPortBraud+"')"));
	m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	Update_Recorder();
}