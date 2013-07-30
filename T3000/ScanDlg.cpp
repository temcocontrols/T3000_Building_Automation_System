// ScanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "ScanDlg.h"
#include "GridButton.h"
#include "TStatScanner.h"





// scan dialog �б����еĶ���
#define SCAN_TABLE_TYPE					0
#define SCAN_TABLE_BUILDING				1
#define SCAN_TABLE_FLOOR					2
#define SCAN_TABLE_ROOM					3
#define SCAN_TABLE_SUBNET				4
#define SCAN_TABLE_SERIALID				5
#define SCAN_TABLE_ADDRESS				6
#define SCAN_TABLE_COMPORT			7

// #define SCAN_TABLE_CONFLICT			8
// #define SCAN_TABLE_FIXCONFLICT		9
///#define SCAN_TABLE_BAUDRATE			6
#define SCAN_TABLE_PROTOCOL			8



IMPLEMENT_DYNAMIC(CScanDlg, CDialog)

CScanDlg::CScanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScanDlg::IDD, pParent)
{
	InitializeCriticalSection(&m_csGrid);
	m_pScanner = NULL;
	m_szGridEditPos.cx = -1;
	m_szGridEditPos.cy = -1;
}

CScanDlg::~CScanDlg()
{
	DeleteCriticalSection(&m_csGrid);
}

void CScanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_flexGrid);

	DDX_Control(pDX, IDC_EDIT_GRIDEDIT, m_editGrid);
}


BEGIN_MESSAGE_MAP(CScanDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SCANALL, &CScanDlg::OnBnClickedButtonScanall)
	ON_BN_CLICKED(IDC_BUTTON_AUTO, &CScanDlg::OnBnClickedButtonAuto)
	ON_BN_CLICKED(IDC_BUTTON_MANUAL, &CScanDlg::OnBnClickedButtonManual)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CScanDlg::OnBnClickedButtonExit)
	ON_MESSAGE(WM_SCANFINISH, 	OnScanFinish)
	ON_MESSAGE(WM_ADDCOMSCAN, 	OnAddComScanRet)
	ON_MESSAGE(WM_ADDNETSCAN, 	OnAddNetScanRet)

	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CScanDlg message handlers



void CScanDlg::OnBnClickedButtonExit()
{
	SaveAllNodeToDB();
	Release();
	CDialog::OnOK();
}

BOOL CScanDlg::OnInitDialog()
{	
	CDialog::OnInitDialog();	

	ASSERT(m_pScanner);	
	InitScanGrid();	
//	Sleep(3000);
	AddComDeviceToGrid(m_pScanner->m_szTstatScandRet);
	AddNetDeviceToGrid(m_pScanner->m_szNCScanRet);



	return TRUE;
}
					
void CScanDlg::InitScanGrid()
{
	//m_flexGrid.put_TextMatrix(0,0,_T("NO."));
	m_flexGrid.put_TextMatrix(0,0,_T("Model"));
	m_flexGrid.put_TextMatrix(0,1,_T("Building"));
	m_flexGrid.put_TextMatrix(0,2,_T("Floor"));
	m_flexGrid.put_TextMatrix(0,3,_T("Room"));
	m_flexGrid.put_TextMatrix(0,4,_T("Sub_net"));
	m_flexGrid.put_TextMatrix(0,5,_T("Serial#"));
	m_flexGrid.put_TextMatrix(0,6,_T("Address"));
	m_flexGrid.put_TextMatrix(0,7,_T("Port"));
	m_flexGrid.put_TextMatrix(0,8,_T("Protocol"));
	// 	m_flexGrid.put_TextMatrix(0,8,_T("Confilct"));
	// 	m_flexGrid.put_TextMatrix(0,9,_T("Fix Conflict"));

	for(int i = 0; i < 9; i++)
	{
		m_flexGrid.put_ColAlignment(i,4);
	}

	m_flexGrid.put_ColWidth(0,1000);	//type
	m_flexGrid.put_ColWidth(1,800);		//building
	m_flexGrid.put_ColWidth(2,800);		//floor
	m_flexGrid.put_ColWidth(3,800);		//room
	m_flexGrid.put_ColWidth(4,800);		//subnet

	m_flexGrid.put_ColWidth(5,1000);		//serial#
	m_flexGrid.put_ColWidth(6,1400);	//Address
	m_flexGrid.put_ColWidth(7,800);		//Port
	m_flexGrid.put_ColWidth(8,1200);	//protocol

// 	m_flexGrid.put_ColWidth(8,600);		//if conflict
// 	m_flexGrid.put_ColWidth(9,800);		//fix
	
}

void CScanDlg::AddNetDeviceToGrid(vector<_NetDeviceInfo*>& szList)
{
	//EnterCriticalSection(&m_csGrid);
	int nRSize = m_flexGrid.get_Rows();
	int nSize = szList.size();

	m_flexGrid.put_Rows(nSize + nRSize);

	for (UINT i = 0; i < szList.size(); i++)
	{
		_NetDeviceInfo* pNetInfo = szList[i];
		
		//////////////////////////////////////////////////////////////////////////
		//CString strNO;
		//strNO.Format(_T("%d"), i);
		//m_flexGrid.put_TextMatrix(i+1,0,strNO); 

		CString strType = pNetInfo->m_pNet->GetProductName();		
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_TYPE,strType); 

		CString strBuilding = pNetInfo->m_pNet->GetBuildingName();		
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_BUILDING,strBuilding); 

		CString strFloor = pNetInfo->m_pNet->GetFloorName();
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_FLOOR,strFloor); 

		CString strRoom = pNetInfo->m_pNet->GetRoomName();
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_ROOM,strRoom); 

		CString strSubnet = pNetInfo->m_pNet->GetSubnetName();
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_SUBNET,strSubnet); 

		CString strSerailID;
		int nSID = pNetInfo->m_pNet->GetSerialID();
		strSerailID.Format(_T("%d"), nSID);
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_SERIALID,strSerailID); 
		/////
		DWORD dwIP =pNetInfo->m_pNet->GetIPAddr();				
		in_addr ad;
		ad.S_un.S_addr = dwIP;
		CString strAddr(inet_ntoa(ad));
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_ADDRESS,strAddr); 
		/////
		CString strPort;
		int nPort = pNetInfo->m_pNet->GetIPPort();
		strPort.Format(_T("%d"), nPort);
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_COMPORT,strPort); 
		////
		CString strProtocol;		
		strProtocol.Format(_T("TCP/IP"));
		m_flexGrid.put_TextMatrix(i+nRSize,SCAN_TABLE_PROTOCOL,strProtocol); 
		////
		// CString strConflict;
	}

	//LeaveCriticalSection(&m_csGrid);
}


int CScanDlg::GetAllNodeFromDataBase()
{
	//_ConnectionPtr m_pCon;//for ado connection
	//_RecordsetPtr m_pRs;//for ado 

	m_pCon.CreateInstance("ADODB.Connection");
	m_pCon->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
	m_pRs.CreateInstance("ADODB.Recordset");

	_variant_t temp_variant;
	CString strTemp;

	CString temp_str=_T("select * from ALL_NODE");
	m_pRs->Open(_variant_t(temp_str),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);	

	//return m_pRs->get_RecordCount();

	int nTemp = 0;
	while(VARIANT_FALSE==m_pRs->EndOfFile)
	{
		nTemp++;
		int nDefault=0;
		CString strDevType = m_pRs->GetCollect("Product_class_ID");

		CTStatBase* pNode = NULL;
		if (strDevType.CompareNoCase(_T("100")) == 0)	 //100 =  NC
		{
			pNode = new CTStat_Net;
			// IP Addr
			CString strIP = m_pRs->GetCollect("Product_ID");
			//((CTStat_Net*)(pNode))->SetIPAddr(strIP);
			
			// Port
			CString strPort = m_pRs->GetCollect("Bautrate");
			((CTStat_Net*)(pNode))->SetIPAddr(_wtoi(strPort));

			m_szNetNodes.push_back(((CTStat_Net*)(pNode)));
		}
		else
		{
			pNode = new CTStat_Dev;
			// BaudRate
			CString strBaudRate = m_pRs->GetCollect("Bautrate");
			((CTStat_Dev*)(pNode))->SetBaudRate(_wtoi(strBaudRate));

			m_szComNodes.push_back(((CTStat_Dev*)(pNode)));

			CString strComPort = m_pRs->GetCollect("Com_Port");
			strComPort = strComPort.Mid(3);
			((CTStat_Dev*)(pNode))->SetComPort(_wtoi(strComPort));

			CString strEPSize = m_pRs->GetCollect("EPsize");
			((CTStat_Dev*)(pNode))->SetEPSize(int(_wtoi(strEPSize)));

		}
		pNode->SetProductType(_wtoi(strDevType));

		pNode->SetBuildingName(m_pRs->GetCollect("MainBuilding_Name"));
		pNode->SetSubnetName(m_pRs->GetCollect("Building_Name"));

		pNode->SetFloorName(m_pRs->GetCollect("Floor_Name"));
		pNode->SetRoomName(m_pRs->GetCollect("Room_Name"));

		CString strID = m_pRs->GetCollect("Product_ID");		
		pNode->SetDevID(_wtoi(strID));
		
		CString strHwv = m_pRs->GetCollect("Hardware_Ver");
		pNode->SetHardwareVersion(float(_wtof(strHwv)));
		CString strSwv = m_pRs->GetCollect("Software_Ver");
		pNode->SetSoftwareVersion(float(_wtof(strSwv)));
	




		CString strSerialID = m_pRs->GetCollect("Serial_ID");		
		(pNode)->SetSerialID(_wtoi(strSerialID));
		
		m_pRs->MoveNext();		
	}
	return m_szNetNodes.size() + m_szComNodes.size();
}




LRESULT CScanDlg::OnAddComScanRet(WPARAM wParam, LPARAM lParam)
{
	//AddComDeviceToGrid(m_pScanner->m_szTsatScandRet);
	return 1;
}

LRESULT CScanDlg::OnAddNetScanRet(WPARAM wParam, LPARAM lParam)
{
	//AddNetDeviceToGrid(m_pScanner->m_szNCScanRet);
	return 1;
}


LRESULT CScanDlg::OnScanFinish(WPARAM wParam, LPARAM lParam)
{

	AddComDeviceToGrid(m_pScanner->m_szTstatScandRet);
	AddNetDeviceToGrid(m_pScanner->m_szNCScanRet);

	//OpenDefaultCom();
	return 1;
}



// do something clean, call it before scan every time.
void CScanDlg::Release()
{	
	for (UINT i = 0; i < m_szComNodes.size(); i++)
	{
		delete m_szComNodes[i];
	}
	m_szComNodes.clear();
	for (UINT i = 0; i < m_szNetNodes.size(); i++)
	{
		delete m_szNetNodes[i];
	}
	m_szNetNodes.clear();


// 	for (UINT i = 0; i < m_szNCScanRet.size(); i++)
// 	{
// 		delete m_szNCScanRet[i];
// 	}
// 	m_szNCScanRet.clear();
// 
// 	for (UINT i = 0; i < m_szTSScanRet.size(); i++)
// 	{
// 		delete m_szTSScanRet[i];
// 	}
// 	m_szTSScanRet.clear();


	//vector<Conflict_Groups*>	m_szConflictNet;
	//vector<Conflict_Groups*>	m_szConflictCom;
	
// 	if (m_pScanner != NULL)
// 	{
// 		m_pScanner->Release();
// 		delete m_pScanner;
// 		m_pScanner = NULL;
// 	}
}


void CScanDlg::OpenDefaultCom()
{	
	CString strSql;
	strSql.Format(_T("select * from Building order by Main_BuildingName"));
	//_RecordsetPtr pRs;
	//_ConnectionPtr pCon;
	
	//pCon.CreateInstance("ADODB.Connection");
	//pCon->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
	//pRs.CreateInstance("ADODB.Recordset");

	m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);			
	_variant_t temp_variant;
	CString strDefaultCom = 0;		
	while(VARIANT_FALSE==m_pRs->EndOfFile)
	{	
		int bSel = 0;
		bSel=m_pRs->GetCollect(_T("Default_SubBuilding"));
		if(bSel==-1)//def building;
		{
			strDefaultCom=m_pRs->GetCollect(_T("Com_Port"));
		}			
	}

	if (strDefaultCom.GetLength() == 0)
	{
		return;
	}
	int nCom = _wtoi(strDefaultCom.Right(1));
	open_com(nCom);
	m_pCon->Close();
	
}



CTStat_Dev* CScanDlg::FindComDeviceBySerialIDInDB(DWORD dwSerialID)
{
	for (UINT i = 0 ; i < m_szComNodes.size(); i++)
	{
		CTStat_Dev* pDev = m_szComNodes[i];
		if (dwSerialID ==  pDev->GetSerialID())
		{
			return pDev;
		}
	}
	return NULL;
}


CTStat_Net* CScanDlg::FindNetDeviceBySerialIDInDB(DWORD dwSerialID)
{
	for (UINT i = 0 ; i < m_szNetNodes.size(); i++)
	{
		CTStat_Net* pNet = m_szNetNodes[i];
		if (dwSerialID ==  pNet->GetSerialID())
		{
			return pNet;
		}
	}

	return NULL;
}


void CScanDlg::SetGridCellColor(int nRow, int nCol, COLORREF clr)
{
	m_flexGrid.put_Row(nRow);
	m_flexGrid.put_Col(nCol);
	m_flexGrid.put_CellBackColor(clr);
}


void CScanDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	//RecoveryID();
	Release();

	CDialog::OnClose();
}



// 
// // ��ָ��λ�û�button
// void CScanDlg::DrawButtonOnGrid(int iRow, int iCol)
// {
// 	CRect rcCell = CalcGridCellRect(iRow, iCol);
//  	CButton*  pBtnTemp = (CButton*)GetDlgItem(IDC_BUTTON_AUTO);
// 
// // 	pBtn->MoveWindow(rcCell,1);
// // 	pBtn->SetFocus();
// // 	pBtn->BringWindowToTop();
// 
// 
// 	CGridButton*  pBtn = new CGridButton;
// 	pBtn->Create(_T("Fix"), BS_CENTER, rcCell, this, 5000);
// 	pBtn->SetFont(pBtnTemp->GetFont(), TRUE);
// 	pBtn->SetPosition(iRow, iCol);
// 	
// 	// 	pBtn->MoveWindow(rcCell,1);
// 	 	pBtn->SetFocus();
// 	 	pBtn->BringWindowToTop();
// 		pBtn->ShowWindow(SW_NORMAL);
// 		m_szGridBtns.push_back(pBtn);
// }


BOOL CScanDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
return CDialog::OnCommand(wParam, lParam);
}


void CScanDlg::ShowInputEditBox(int iRow, int iCol)
{	
//	int iRow,iCol;
	m_editGrid.SetWindowText(_T(""));
	CRect rc = CalcGridCellRect(iRow, iCol);  // �϶�ID��
	m_editGrid.MoveWindow(rc, TRUE);
	m_editGrid.SetFocus();
	m_editGrid.BringWindowToTop();
	m_editGrid.ShowWindow(SW_NORMAL);
	m_szGridEditPos.cx = iRow;
	m_szGridEditPos.cy = iCol;
	
}


CRect CScanDlg::CalcGridCellRect(int iRow, int iCol )
{
	CRect rect;
	m_flexGrid.GetWindowRect(rect); //��ȡ���ؼ��Ĵ��ھ���
	ScreenToClient(rect); //ת��Ϊ�ͻ�������	
	CDC* pDC = GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//����ѡ�и�����Ͻǵ�����(����Ϊ��λ)
	long y = m_flexGrid.get_RowPos(iRow)/nTwipsPerDotY;
	long x = m_flexGrid.get_ColPos(iCol)/nTwipsPerDotX;
	//����ѡ�и�ĳߴ�(����Ϊ��λ)����1��ʵ�ʵ����У����ּ�1��Ч������
	long width = m_flexGrid.get_ColWidth(iCol)/nTwipsPerDotX+1;
	long height = m_flexGrid.get_RowHeight(iRow)/nTwipsPerDotY+1;
	//�γ�ѡ�и����ڵľ�������
	CRect rcCell(x,y,x+width,y+height);
	//ת������ԶԻ��������
	rcCell.OffsetRect(rect.left+2,rect.top+2);	
	rcCell.InflateRect(-1,-1,-1,-1);
	//CString strValue = m_flexGrid.get_TextMatrix(iRow,iCol);

	//pDC->Draw3dRect(rcCell, RGB(255,0,0), RGB(192,192,192));
	//pDC->DrawText(_T("Fix"), &rcCell, DT_CENTER);

	return rcCell;
	ReleaseDC(pDC);

}


BEGIN_EVENTSINK_MAP(CScanDlg, CDialog)
ON_EVENT(CScanDlg, IDC_MSFLEXGRID1, DISPID_CLICK, CScanDlg::ClickMsflexgrid1, VTS_NONE)
END_EVENTSINK_MAP()

void CScanDlg::ClickMsflexgrid1()
{
	CSize szTemp;
	CalcClickPos(szTemp);// ������е�λ��
	if (szTemp == m_szGridEditPos)
	{
		return;	
	}
	
	
	// 3�����
	// 1������������Ч������ʾ����¼�����ݲ��ƶ�edit
	// 2��������Ч������ʧȥ���㣬�ص�edit����ʾ����¼������
	// 3���˳����ص�edit����ʾ����¼�����ݣ������������ݿ�

	if (IsValidClick(szTemp))  
	{	
		if(m_szGridEditPos.cx != -1) // ���ǵ�һ�ε������Ҫ��¼��һ�ε�����
		{
			GetGridEditString();    // ��¼edit������			
		}
		ShowInputEditBox(szTemp.cx, szTemp.cy);		
		m_szGridEditPos = szTemp;							
	}	
	else// ���ڲ���ɵĵط��� destroy
	{	
		GetGridEditString();    // ��¼edit������
		DestroyFlexEdit();	     
	}
// 	if(GetFocus()->m_hWnd != m_editGrid.m_hWnd)
// 	{
// 		
// 	}



}

void CScanDlg::GetGridEditString() // ��¼edit������
{
	CString strText ;
	m_editGrid.GetWindowText(strText)	;
	if (strText.GetLength() == 0)  // û���룬ɶҲ����
	{
		return;
	}
	CTStatBase* pInfo = FindDeviceByRowNum(m_szGridEditPos.cx);
	if (pInfo == NULL)
	{
		ASSERT(pInfo);
		return;
	}
	switch(m_szGridEditPos.cy)
	{
	case 1:
		pInfo->SetBuildingName(strText);
		break;
	case 2:
		pInfo->SetFloorName(strText);
		break;
	case 3:
		pInfo->SetRoomName(strText);
		break;
	default:
		break;
	}

	m_flexGrid.put_TextMatrix(m_szGridEditPos.cx, m_szGridEditPos.cy, strText);
}


//////////////////////////////////////////////////////////////////////////
// return 1, ���� 
// return 0, ��Ч��������ǲ�Ӧ�÷�Ӧ��
// return 3, ��grid֮����߲�֪��ʲô�ط�
int CScanDlg::IsValidClick(CSize szTemp)
{
	if (szTemp.cy  <= 4 && szTemp.cy >0 &&
		szTemp.cx > 0)	
	{
		return 1;
	}
	return 0;

}


void CScanDlg::CalcClickPos(CSize& size)
{
	long lRow,lCol;
	lRow = m_flexGrid.get_RowSel();//��ȡ������к�	
	lCol = m_flexGrid.get_ColSel(); //��ȡ������к�

	if(lRow<=0)
		return;
	size.cx=lRow;
	size.cy = lCol;
}


void CScanDlg::DestroyFlexEdit()
{
	m_editGrid.ShowWindow(SW_HIDE);	
	CString strText;
	m_editGrid.GetWindowText(strText);		
}


// �����ݿ����ֵȡ���ŵ��ڴ�
// ���ڴ��ֵ����д�����ݿ�
//
// ֻ����д buildingname��floorname��roomname��address
// ��ˣ���Ҫ��Ĵ���д��ֻ��address
void CScanDlg::GetDataFromGrid()
{
	int nCount = m_flexGrid.get_Rows();
	for(int i = 1; i < nCount; i++)
	{
// 		if ()
// 		{
// 		}
		// Building
		CString strBuildingName = m_flexGrid.get_TextMatrix(i, SCAN_TABLE_BUILDING);
		// Floor
		CString strFloorName = m_flexGrid.get_TextMatrix(i, SCAN_TABLE_FLOOR);
		// Room
		CString strRoomName = m_flexGrid.get_TextMatrix(i, SCAN_TABLE_ROOM);
		// subnet
		CString strSubnetName = m_flexGrid.get_TextMatrix(i, SCAN_TABLE_SUBNET);
		// Serial#
		CString strSerialID = m_flexGrid.get_TextMatrix(i, SCAN_TABLE_SERIALID);
		int nSerialID = _wtoi(strSerialID);
		// Address
		//CString strAddress = m_flexGrid.get_TextMatrix(i, SCAN_TABLE_ADDRESS);
		//int nAddress = _wtoi(strAddress);
		// Com/IP
		//CString strCom = m_flexGrid.get_TextMatrix(i, SCAN_TABLE_COMPORT);
		// BaudRate/Port
		//CString strPort = m_flexGrid.get_TextMatrix(i, 6);
		//int nPort = _wtoi(nPort);
		// Protocol		
		
		CTStatBase* pInfo = FindDeviceByRowNum(i);
		ASSERT(pInfo);
		pInfo->SetBuildingName(strBuildingName);
		pInfo->SetFloorName(strFloorName);
		pInfo->SetRoomName(strRoomName);
		pInfo->SetSubnetName(strSubnetName);
		//pInfo->m_pDev->SetDevID(nAddress);
		//Write_One(pInfo->m_pDev->GetDevID(), 6, nAddress);  // ��ʱдID���Ĵ���	
	}
}

// �����нڵ㱣�浽���ݿ⣬AllNodes
void CScanDlg::SaveAllNodeToDB()
{
	GetAllNodeFromDataBase();
	GetDataFromGrid();	
	CombineDBandScanRet();

	m_pCon.CreateInstance("ADODB.Connection");
	m_pCon->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);

	// ��ɾ�����ݿ� // maurice˵��Ҫɾ

	try
	{

	CString strSql;
	strSql.Format(_T("delete * from ALL_NODE"));
	m_pCon->Execute(strSql.GetString(),NULL,adCmdText);

	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	for (UINT i = 0; i < m_pScanner->m_szTstatScandRet.size(); i++)
	{
		CTStat_Dev* pDevInfo = m_pScanner->m_szTstatScandRet[i]->m_pDev;
		WriteOneDevInfoToDB(pDevInfo);
	}

	for (UINT i = 0; i < m_pScanner->m_szNCScanRet.size(); i++)
	{
		CTStat_Net* pNetInfo = m_pScanner->m_szNCScanRet[i]->m_pNet;
		WriteOneNetInfoToDB(pNetInfo);
	}

	// ���ݿ��д��ȥ.
	for (UINT i = 0; i < m_szComNodes.size(); i++)
	{
		CTStat_Dev* pDevInfo = m_szComNodes[i];
		WriteOneDevInfoToDB(pDevInfo);
	}

	for (UINT i = 0; i < m_szNetNodes.size(); i++)
	{
		CTStat_Net* pNetInfo = m_szNetNodes[i];
		WriteOneNetInfoToDB(pNetInfo);
	}


	m_pCon->Close();
}


// �ϲ����ݿ��scan������Ա�������ݿ�
// ���Serial ID��ͬ�ģ�����grid������Ϊ׼
void CScanDlg::CombineDBandScanRet()
{
	//vector<CTStat_Dev*> szDB;
	BOOL bFind = FALSE;
	for (UINT i = 0; i < m_pScanner->m_szTstatScandRet.size(); i++)
	{
		CTStat_Dev* pDev = m_pScanner->m_szTstatScandRet[i]->m_pDev;
		int nSID = pDev->GetSerialID();
		bFind = FALSE;
		for(UINT j = 0; j < m_szComNodes.size(); j++)
		{
			CTStat_Dev* pDBDev = m_szComNodes[j];
			int nDBSID= pDBDev->GetSerialID();
			if (nDBSID == nSID) // ��ȣ�ɾ�����ݿ��������scan ���
			{
// 				pDBDev->SetBuildingName(pDev->GetBuildingName());
// 				pDBDev->SetFloorName(pDev->GetFloorName());
// 				pDBDev->SetRoomName(pDev->GetRoomName());
// 				pDBDev->SetSubnetName(pDBDev->GetSubnetName());
// 				//pDBDev->SetBuildingName();		
				delete pDBDev;
				pDBDev = NULL;
				m_szComNodes.erase(m_szComNodes.begin() + j);
				j--;
			}			
		}
	}	
	

	for (UINT i = 0; i < m_pScanner->m_szNCScanRet.size(); i++)
	{
		CTStat_Net* pNet = m_pScanner->m_szNCScanRet[i]->m_pNet;
		int nSID = pNet->GetSerialID();
		bFind = FALSE;
		for(UINT j = 0; j < m_szNetNodes.size(); j++)
		{
			CTStat_Net* pDBNet = m_szNetNodes[j];
			int nDBSID= pDBNet->GetSerialID();
			if (nDBSID == nSID) // ��ȣ���ֵ
			{
// 				pDBNet->SetBuildingName(pDBNet->GetBuildingName());
// 				pDBNet->SetFloorName(pDBNet->GetFloorName());
// 				pDBNet->SetRoomName(pDBNet->GetRoomName());
// 				pDBNet->SetSubnetName(pDBNet->GetSubnetName());
// 				pDBNet->SetIPAddr(pDBNet->GetIPAddr());

				delete pDBNet;
				pDBNet = NULL;
				m_szNetNodes.erase(m_szNetNodes.begin() + j);
				j--;
				//pDBDev->SetBuildingName();			
			}			
		}
	}	
}


//�����OӋ
void CScanDlg::WriteOneDevInfoToDB( CTStat_Dev* pDev)
{
	ASSERT(pDev);
	
	CString strBuildingName = pDev->GetBuildingName();

	CString strFloorName = pDev->GetFloorName();

	CString strRoomName = pDev->GetRoomName();	
	
	CString strID;
	int nID = pDev->GetDevID();	
	strID.Format(_T("%d"),  nID);

	CString strProductName = pDev->GetProductName();

	CString strSerialID;
	strSerialID.Format(_T("%d"), pDev->GetSerialID());



	 int nClassID = pDev->GetProductType();
	CString strClassID;
	strClassID.Format(_T("%d"), nClassID);

	int nBaudRate = pDev->GetBaudRate();
	CString strBaudRate;
	strBaudRate.Format(_T("%d"), nBaudRate);

	CString strScreenName;
	strScreenName.Format(_T("Screen(S:%d--%d)"), pDev->GetSerialID(), pDev->GetDevID() );

	CString strBackground_bmp=_T("Clicking here to add a image...");

	CString strHWV;
	strHWV.Format(_T("%.1f"), pDev->GetHardwareVersion());

	CString strSWV;
	strSWV.Format(_T("%.1f"), pDev->GetSoftwareVersion());

	CString strCom;
	strCom.Format(_T("COM%d"), pDev->GetComPort());
	
	CString strSql;

	CString strSubNetName=pDev->GetSubnetName();//_T("Sub_net1");

	CString strEpSize;
	strEpSize.Format(_T("%d"), pDev->GetEPSize());

	try
	{

	strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize) values('"+strBuildingName+"','"+strSubNetName+"','"+strSerialID+"','"+strFloorName+"','"+strRoomName+"','"+strProductName+"','"+strClassID+"','"+strID+"','"+strScreenName+"','"+strBaudRate+"','"+strBackground_bmp+"','"+strHWV+"','"+strSWV+"','"+strCom+"','"+strEpSize+"')"));
	//new nc // strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize) values('"+strBuildingName+"','"+strSubNetName+"','"+strSerialID+"','"+strFloorName+"','"+strRoomName+"','"+strProductName+"','"+strClassID+"','"+strID+"','"+strScreenName+"','"+strBaudRate+"','"+strBackground_bmp+"','"+strHWV+"','"+strSWV+"','"+strCom+"','"+strEPSize+"','"+strMainnetInfo+"')"));
	m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
}

void CScanDlg::WriteOneNetInfoToDB( CTStat_Net* pNet)
{
 	ASSERT(pNet);
// 	_ConnectionPtr t_pCon;//for ado connection
// 	t_pCon.CreateInstance(_T("ADODB.Connection"));
// 	t_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

	CString strBuildingName = pNet->GetBuildingName();

	CString strFloorName = pNet->GetFloorName();

	CString strRoomName = pNet->GetRoomName();	

	// Modbus ID
	CString strID;
	int nID = pNet->GetDevID();	
	strID.Format(_T("%d"),  nID);

	CString strProductName = pNet->GetProductName();

	CString strSerialID;
	strSerialID.Format(_T("%d"), pNet->GetSerialID());

	int nClassID = pNet->GetProductType();
	CString strClassID;
	strClassID.Format(_T("%d"), nClassID);

	CString strScreenName;
	strScreenName.Format(_T("Screen(S:%d--%d)"), pNet->GetSerialID(), pNet->GetDevID() );

	CString strBackground_bmp=_T("Clicking here to add a image...");

	CString strHWV;
	strHWV.Format(_T("%0.1f"), pNet->GetHardwareVersion());

	CString strSWV;
	strSWV.Format(_T("%0.1f"), pNet->GetSoftwareVersion());

	// 	CString strCom;
	// 	strCom.Format(_T("COM%d"), pInfo->m_pNet->GetComPort());

	CString strSql;
	CString strSubNetName = pNet->GetSubnetName();
	//CString strSubNetName=_T("Sub_net1");

	//CString strEpSize;
	//strEpSize.Format(_T("%d"), pInfo->m_pNet->GetEPSize());

	CString strIP;
	in_addr ia;
	ia.S_un.S_addr = pNet->GetIPAddr();
	strIP = CString(inet_ntoa(ia));	


	CString strPort;
	strPort.Format(_T("%d"), pNet->GetIPPort());

	CString strEPSize = _T("0");
	

	strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize) values('"+strBuildingName+"','"+strSubNetName+"','"+strSerialID+"','"+strFloorName+"','"+strRoomName+"','"+strProductName+"','"+strClassID+"','"+strID+"','"+strScreenName+"','"+strIP+"','"+strBackground_bmp+"','"+strHWV+"','"+strSWV+"','"+strPort+"','"+strEPSize+"')"));
	//new nc // strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize) values('"+strBuildingName+"','"+strSubNetName+"','"+strSerialID+"','"+strFloorName+"','"+strRoomName+"','"+strProductName+"','"+strClassID+"','"+strID+"','"+strScreenName+"','"+strIP+"','"+strBackground_bmp+"','"+strHWV+"','"+strSWV+"','"+strPort+"','"+strEPSize+"','"+strMainnetInfo+"')"));
	try
	{

		m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////


void CScanDlg::SetScanner(CTStatScanner* pScanner)
{
	m_pScanner = pScanner;
}


void CScanDlg::AddComDeviceToGrid(vector<_ComDeviceInfo*>& szList)
{
	int nSize = 1;//m_flexGrid.get_Rows();
	VARIANT vRow; 
	vRow.vt = VT_I4;//ָ���������� 
	vRow.lVal =1;     //��ֵ 
	//EnterCriticalSection(&m_csGrid);
	TRACE(_T("scan has found node == %d\n"), szList.size());
	m_flexGrid.put_Rows(szList.size() + nSize);
	for (UINT i = 0; i < szList.size(); i++)
	{
		//vRow.lVal = 1+i;
		//m_flexGrid.AddItem(_T(""), vRow);
		_ComDeviceInfo* pDevInfo = szList[i];
		//////////////////////////////////////////////////////////////////////////		
		// 		CString strNO;
		// 		strNO.Format(_T("%d"), i);
		// 		m_flexGrid.put_TextMatrix(i+nSize,0,strNO); 

		CString strType = pDevInfo->m_pDev->GetProductName();
		strType = strType.Left(strType.Find(_T(":")));
		m_flexGrid.put_TextMatrix(i+nSize,SCAN_TABLE_TYPE,strType); 

		CString strBuilding = pDevInfo->m_pDev->GetBuildingName();
		m_flexGrid.put_TextMatrix(i+nSize,SCAN_TABLE_BUILDING,strBuilding); 

		CString strFloor = pDevInfo->m_pDev->GetFloorName();
		m_flexGrid.put_TextMatrix(i+nSize,SCAN_TABLE_FLOOR,strFloor); 

		CString strRoom = pDevInfo->m_pDev->GetRoomName();
		m_flexGrid.put_TextMatrix(i+nSize,SCAN_TABLE_ROOM,strRoom); 

		CString strSubnet = 	pDevInfo->m_pDev->GetSubnetName();
		m_flexGrid.put_TextMatrix(i+nSize,SCAN_TABLE_SUBNET,strSubnet); 


		CString strSerialID;
		int nSID = pDevInfo->m_pDev->GetSerialID();
		strSerialID.Format(_T("%d"), nSID);
		m_flexGrid.put_TextMatrix(i+nSize,SCAN_TABLE_SERIALID,strSerialID); 
		///// ID
		int nID = 0;

		//// conflict
		CString strConflict;
		if (pDevInfo->m_bConflict)
		{
			
			strConflict = _T("YES");
			//SetGridCellColor(i+nSize, SCAN_TABLE_CONFLICT, RGB(192,0,0));

			TRACE("Draw Button's SrcID = %d\n", nID);
			//DrawButtonOnGrid(nSize + i, SCAN_TABLE_FIXCONFLICT);
			int nID = pDevInfo->m_nSourceID;	
			int nID1 = pDevInfo->m_pDev->GetDevID();	
			CString strID; strID.Format(_T("%d"), nID);
			CString strID1; strID1.Format(_T("%d"), nID1);

			strConflict = _T("More than one TStat on ID XXX, the Serial No strSerial has been changed as XXX. ");

		}
		else
		{
			//nID = pDevInfo->m_pDev->GetDevID();	
			strConflict = _T("NO");
			//DrawButtonOnGrid(nSize + i, 8);
		}
			
		nID = pDevInfo->m_pDev->GetDevID();	
		CString strID;
		strID.Format(_T("%d"), nID);
		m_flexGrid.put_TextMatrix(i+nSize,SCAN_TABLE_ADDRESS,strID); 
		///// port
		int nPort = pDevInfo->m_pDev->GetComPort();	
		CString strPort;
		if(pDevInfo->m_pDev->m_mainnet_info.m_ProductType == 0)
		{
			strPort.Format(_T("COM%d"), nPort);
		}
		else if(pDevInfo->m_pDev->m_mainnet_info.m_ProductType == 100)
		{
			strPort.Format(_T("NC:COM%d"), nPort);
		}
		m_flexGrid.put_TextMatrix(i+nSize,SCAN_TABLE_COMPORT,strPort); 
		//// protocol
		CString strProtocol;		
		strProtocol.Format(_T("Modbus 485"));
		m_flexGrid.put_TextMatrix(i+nSize,SCAN_TABLE_PROTOCOL,strProtocol); 
		//// 
		//m_flexGrid.put_TextMatrix(i+nSize,8,strConflict); 
	}
	//LeaveCriticalSection(&m_csGrid);
	//DrawButtonOnGrid(1, 9);
}




CTStatBase* CScanDlg::FindDeviceByRowNum( int nRow )
{
	//ASSERT(nRow < m_flexGrid.get_GridLines());
	int nRows = m_flexGrid.get_Rows();

	CString strSID = m_flexGrid.get_TextMatrix(nRow, SCAN_TABLE_SERIALID);
	int nSID = _wtoi(strSID);


	for (UINT i = 0 ; i < m_pScanner->m_szTstatScandRet.size(); i++)
	{
		_ComDeviceInfo* pInfo = m_pScanner->m_szTstatScandRet[i];
		if (pInfo->m_pDev->GetSerialID() == nSID)
		{
			return pInfo->m_pDev;
		}
	}

	for (UINT i = 0 ; i < m_pScanner->m_szNCScanRet.size(); i++)
	{
		_NetDeviceInfo* pInfo = m_pScanner->m_szNCScanRet[i];
		if (pInfo->m_pNet->GetSerialID() == nSID)
		{
			return pInfo->m_pNet;
		}
	}

	return NULL;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// below the code disposed
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CScanDlg::OnBnClickedButtonScanall()
{
//	DoScanAllDevice();
}

void CScanDlg::OnBnClickedButtonAuto()
{
	//AutoFixComConflict();
}

void CScanDlg::OnBnClickedButtonManual()
{
	// TODO: Add your control notification handler code here
//	AfxMessageBox(_T("111"), MB_OK);
}

