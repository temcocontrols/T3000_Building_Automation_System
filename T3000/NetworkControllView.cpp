// NetworkControllView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "NetworkControllView.h"
#include "globle_function.h"
#include "Weekly_Routines.h"
#include "Annual_Routines.h"
#include "Config_Routines.h"
#include "AfxMessageDialog.h"
#include "Timer_Calibrate.h"

#include "MainFrm.h"
// CNetworkControllView

const CString c_strTimeserverList[5] = {_T("time.windows.com"),_T("time.nist.gov"),_T("time-nw.nist.gov"),_T("time-a.nist.gov"),_T("time-b.nist.gov")};
#define  _OLD_NC_VERSION 9

#define NO_FIELD						0
#define PRODUCT_FIELD				1
#define SUBNET_FIELD					2
#define SERIALID_FIELD				3
#define ADDRESS_FIELD				4
#define STATUS_FIELD					5
#define TIMESINCE_FIELD				6



IMPLEMENT_DYNCREATE(CNetworkControllView, CFormView)

CNetworkControllView::CNetworkControllView()
	: CFormView(CNetworkControllView::IDD)
	, m_IDaddress(0)
	, m_nSerialNum(0)
	, m_firmwareVersion(0)
	, m_hardware_version(0)
	, m_strDate(_T(""))
	, m_strTime(_T(""))
	, m_nListenPort(6001)
	, m_bWarningBldVersion(FALSE)
{
	
}

CNetworkControllView::~CNetworkControllView()
{
	m_bWarningBldVersion = FALSE;
}

void CNetworkControllView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_IDADDRESS_EDIT, m_IDaddress);
	DDX_Text(pDX, IDC_SERIALNUM_EDIT, m_nSerialNum);
	DDX_Text(pDX, IDC_EDIT4, m_firmwareVersion);
	DDX_Text(pDX, IDC_EDIT5, m_hardware_version);
	DDX_Text(pDX, IDC_EDIT6, m_strDate);
	DDX_Text(pDX, IDC_EDIT7, m_strTime);
	DDX_Text(pDX, IDC_EDIT8, m_nListenPort);


	DDX_Control(pDX, IDC_CHECK1, m_ReadOnlyCheckBtn);
	DDX_Control(pDX, IDC_IPMODEL_COMBO, m_ipModelComBox);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ip_addressCtrl);
	DDX_Control(pDX, IDC_IPADDRESS2, m_subnet_addressCtrl);
	DDX_Control(pDX, IDC_IPADDRESS3, m_gateway_addressCtrl);
	DDX_Control(pDX, IDC_EDIT8, m_listenPortEdit);
	DDX_Control(pDX, IDC_BAUDRATE_COMBX, m_baudRateCombox);
	DDX_Control(pDX, IDC_IDADDRESS_EDIT, m_idEdt);
	DDX_Control(pDX, IDC_MSFLEXGRID_SUB, m_gridSub);
}

BEGIN_MESSAGE_MAP(CNetworkControllView, CFormView)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CHECK1, &CNetworkControllView::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_TIMESETBTN, &CNetworkControllView::OnBnClickedTimesetbtn)
	ON_BN_CLICKED(IDC_GETBTN, &CNetworkControllView::OnBnClickedGetbtn)
	ON_BN_CLICKED(IDC_SAVEBTN, &CNetworkControllView::OnBnClickedSavebtn)
	ON_BN_CLICKED(IDC_REBOOTBTN, &CNetworkControllView::OnBnClickedRebootbtn)
	ON_BN_CLICKED(IDC_FARCALLBTN, &CNetworkControllView::OnBnClickedFarcallbtn)
	ON_BN_CLICKED(IDC_FARCALLIP_BTN, &CNetworkControllView::OnBnClickedFarcallipBtn)
	ON_BN_CLICKED(IDC_BUTTON8, &CNetworkControllView::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_ANUUALBTN, &CNetworkControllView::OnBnClickedAnuualbtn)
	ON_BN_CLICKED(IDC_BUTTON10, &CNetworkControllView::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_APPLYBUTTON, &CNetworkControllView::OnBnClickedApplybutton)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS1, &CNetworkControllView::OnIpnFieldchangedIpaddress1)
	ON_CBN_SELCHANGE(IDC_BAUDRATE_COMBX, &CNetworkControllView::OnCbnSelchangeBaudrateCombx)
	ON_EN_KILLFOCUS(IDC_IDADDRESS_EDIT, &CNetworkControllView::OnEnKillfocusIdaddressEdit)
	ON_BN_CLICKED(IDC_BUTTON_UPDATETIMESERVER, &CNetworkControllView::OnBnClickedButtonUpdatetimeserver)
	//ON_BN_CLICKED(IDC_FIND_TSTAT, &CNetworkControllView::OnBnClickedFindTstat)
	ON_BN_CLICKED(IDC_BUTTON1, &CNetworkControllView::OnBnClickedButton1)
END_MESSAGE_MAP()


// CNetworkControllView diagnostics

#ifdef _DEBUG
void CNetworkControllView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNetworkControllView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CNetworkControllView message handlers

BOOL CNetworkControllView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rcClient;
	pDC->FillSolidRect(&rcClient,RGB(202,208,212));
	return CFormView::OnEraseBkgnd(pDC);
}

void CNetworkControllView::OnInitialUpdate()
{
	
	CFormView::OnInitialUpdate();
	if(g_NetWorkLevel==1)
	{
		CAfxMessageDialog dlg;
		CString strPromption;
		strPromption.LoadString(IDS_STRNOPRIVILEGE);
		dlg.SetPromtionTxt(strPromption);
		dlg.DoModal();
	}
	GetDlgItem(IDC_MacSTATIC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_MACEDIT)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_GETBTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SAVEBTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FARCALLIP_BTN)->ShowWindow(SW_HIDE);
	/*
	GetDlgItem(IDC_GETBTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SAVEBTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_REBOOTBTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FARCALLBTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_FARCALLIP_BTN)->ShowWindow(SW_HIDE);
	*/

	SetTimer(1,20000,NULL);
	// TODO: Add your specialized code here and/or call the base class

	//////////////////////////////////////////////////////////////////////////
	// init timesever combobox
	initTimeServerList();


	//InitGrid();

	

}



void CNetworkControllView::Fresh()
{
	GetDlgItem(IDC_SERIALSTATIC)->SetFocus();
	m_IDaddress=multi_register_value[6];
	m_hardware_version=multi_register_value[8];
	m_nSerialNum=get_serialnumber();
	//m_firmwareVersion=get_curtstat_version();
	GetFirmwareVersion();
	m_nbldVersion = multi_register_value[14];
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_BLDVERSION);
	CString strBldVer; strBldVer.Format(_T("%d"), m_nbldVersion);
	pEdit->SetWindowText(strBldVer);
	if (m_nbldVersion < _OLD_NC_VERSION && !m_bWarningBldVersion && m_nSerialNum != 0 && m_hardware_version < 4) // 序列号不等于0防止掉线时版本寄存器为0弹这个消息
	{	
		m_bWarningBldVersion = TRUE;
		CString strBldVer;
		strBldVer.Format(_T("NC(ID: %d) boot loader's current version is %d, it's too low, please upgrade it."), m_nSerialNum, m_nbldVersion);
		AfxMessageBox(strBldVer);

	}
	m_hardware_version=multi_register_value[8];

	
	if(multi_register_value[1]<10)
		m_strDate.Format(_T("%d%d-%d-%d"),multi_register_value[200],multi_register_value[201],multi_register_value[202],multi_register_value[204]);
	else
		m_strDate.Format(_T("%d%d-%d-%d"),multi_register_value[0],multi_register_value[201],multi_register_value[202],multi_register_value[204]);


	m_strTime.Format(_T("%02d:%02d"),multi_register_value[205],multi_register_value[206]);

	
	m_ip_addressCtrl.SetAddress((BYTE)multi_register_value[107],(BYTE)multi_register_value[108],(BYTE)multi_register_value[109],(BYTE)multi_register_value[110]);
	m_subnet_addressCtrl.SetAddress((BYTE)multi_register_value[111],(BYTE)multi_register_value[112],(BYTE)multi_register_value[113],(BYTE)multi_register_value[114]);
	m_gateway_addressCtrl.SetAddress((BYTE)multi_register_value[115],(BYTE)multi_register_value[116],(BYTE)multi_register_value[117],(BYTE)multi_register_value[118]);
	OnBnClickedCheck1();
		
	if(multi_register_value[12]==0)
	{
		m_baudRateCombox.SetCurSel(0);
	}
		
	if(multi_register_value[12]==1)
	{
		m_baudRateCombox.SetCurSel(1);
	}

	
	if(multi_register_value[106]==0)
	{
		m_ipModelComBox.SetCurSel(0);
	}
	if(multi_register_value[106]==1)
	{
		m_ipModelComBox.SetCurSel(1);
	}


	
	m_nListenPort=multi_register_value[120];
	InitGrid();

	UpdateData(false);
}
void CNetworkControllView::OnBnClickedCheck1()
{
	if (m_ReadOnlyCheckBtn.GetCheck()==BST_CHECKED) 
	{
		m_ipModelComBox.EnableWindow(TRUE);
		//m_listenPortEdit.EnableWindow(TRUE);
		//m_ip_addressCtrl.EnableWindow(TRUE);
		//m_subnet_addressCtrl.EnableWindow(TRUE);
		//m_gateway_addressCtrl.EnableWindow(TRUE);
		GetDlgItem(IDC_APPLYBUTTON)->EnableWindow(TRUE);
	}
	else
	{
		m_ipModelComBox.EnableWindow(FALSE);
		//m_listenPortEdit.EnableWindow(FALSE);
		//m_ip_addressCtrl.EnableWindow(FALSE);
		//m_subnet_addressCtrl.EnableWindow(FALSE);
		//m_gateway_addressCtrl.EnableWindow(FALSE);
		GetDlgItem(IDC_APPLYBUTTON)->EnableWindow(FALSE);
	}
}

void CNetworkControllView::OnBnClickedTimesetbtn()
{
	g_bPauseMultiRead=TRUE;
	g_bEnableRefreshTreeView = FALSE;
	Sleep(50);
	Timer_Calibrate dlg;
	dlg.DoModal();
	
	g_bEnableRefreshTreeView = TRUE;
	g_bPauseMultiRead=FALSE;
}

void CNetworkControllView::OnBnClickedGetbtn()
{
	if(g_NetWorkLevel==1)
		return;
	write_one(g_tstat_id,132,1);
	Sleep(4000);
	// TODO: Add your control notification handler code here
}

void CNetworkControllView::OnBnClickedSavebtn()
{
	if(g_NetWorkLevel==1)
		return;
	write_one(g_tstat_id,131,1);
	Sleep(5000);
	// TODO: Add your control notification handler code here
}

void CNetworkControllView::OnBnClickedRebootbtn()
{
	if(g_NetWorkLevel==1)
		return;
	write_one(g_tstat_id,133,1);
	// TODO: Add your control notification handler code here
}

void CNetworkControllView::OnBnClickedFarcallbtn()
{
	if(g_NetWorkLevel==1)
		return;
	write_one(g_tstat_id,130,0);
	Sleep(5000);//Sleep because network controller is busy now 
}

void CNetworkControllView::OnBnClickedFarcallipBtn()
{
	if(g_NetWorkLevel==1)
		return;
	write_one(g_tstat_id,130,1);
	Sleep(5000);//Sleep because network controller is busy now 
}

void CNetworkControllView::OnBnClickedButton8()
{
	if(g_NetWorkLevel==1)
		return;
	
	g_bEnableRefreshTreeView = FALSE;



	g_bPauseMultiRead=TRUE;
	Weekly_Routines dlg;
	dlg.DoModal();

	g_bPauseMultiRead=FALSE;


	g_bEnableRefreshTreeView = TRUE;
}

void CNetworkControllView::OnBnClickedAnuualbtn()
{
	if(g_NetWorkLevel==1)
		return;
	g_bPauseMultiRead=TRUE;
	Annual_Routines dlg;
	dlg.DoModal();
	g_bPauseMultiRead=FALSE;
}

void CNetworkControllView::OnBnClickedButton10()
{	
	if(g_NetWorkLevel==1)
		return;
		
	g_bPauseMultiRead=TRUE;
	CConfig_Routines dlg;
	dlg.DoModal();
	g_bPauseMultiRead=FALSE;
	// TODO: Add your control notification handler code here
}

void CNetworkControllView::OnBnClickedApplybutton()
{
	if(!CheckSettingChanged()) // if setting changed,return TRUE
	{
		return; // unchanged, don't write register
	}

	if(g_NetWorkLevel==1)
		return;
	BeginWaitCursor();
	//m_nListenPort	
	CString strText;
	m_listenPortEdit.GetWindowText(strText);
	if(!strText.IsEmpty())
		m_nListenPort=_wtoi(strText);
	if(m_nListenPort>=12767)
	{
		AfxMessageBox(_T("The listen port number is too big, please change it."));
		return;
	}
	if(m_nListenPort<=0)
	{
		AfxMessageBox(_T("The listen port number must be greater than 0!"));
		return;
	}
	write_one(g_tstat_id,120,m_nListenPort);
	BYTE address1,address2,address3,address4;
	m_ip_addressCtrl.	GetAddress(address1,address2,address3,address4);
	int n=write_one(g_tstat_id,107,address1);
	n=write_one(g_tstat_id,108,address2);
	n=write_one(g_tstat_id,109,address3);
	n=write_one(g_tstat_id,110,address4);
	m_subnet_addressCtrl.GetAddress(address1,address2,address3,address4);
	write_one(g_tstat_id,111,address1);
	write_one(g_tstat_id,112,address2);
	write_one(g_tstat_id,113,address3);
	write_one(g_tstat_id,114,address4);
	m_gateway_addressCtrl.GetAddress(address1,address2,address3,address4);
	write_one(g_tstat_id,115,address1);
	write_one(g_tstat_id,116,address2);
	write_one(g_tstat_id,117,address3);
	write_one(g_tstat_id,118,address4);
	write_one(g_tstat_id,131,1);
	Sleep(1000);//Sleep(10000); // ???
	write_one(g_tstat_id,133,1);
	Sleep(5000);	//Sleep(5000); // wait for nc restart

	CMainFrame* pPraent=(CMainFrame*)GetParent();
	//pPraent->ReFresh();

	/*
	unsigned short variable[13]={0};
	int nRet=Read_Multi(g_tstat_id,variable,106,13,3);/////////////////////////read
	m_ipModelComBox.SetCurSel(variable[0]);
	m_ip_addressCtrl.SetAddress(variable[1],variable[2],variable[3],variable[4]);
	m_subnet_addressCtrl.SetAddress(variable[5],variable[6],variable[7],variable[8]);
	m_gateway_addressCtrl.SetAddress(variable[9],variable[10],variable[11],variable[12]);

*/
	
	CString strBuilding,strSubBuilding;
	strBuilding=pPraent->m_strCurMainBuildingName;
	strSubBuilding=pPraent->m_strCurSubBuldingName;
	CString strIP,strPort;
	strPort.Format(_T("%d"),multi_register_value[120]);
	m_ip_addressCtrl.GetAddress(address1,address2,address3,address4);
	strIP.Format(_T("%d.%d.%d.%d"),address1,address2,address3,address4);
	if(g_CommunicationType==1)//TCP
	{
		int nPort;
		nPort=_wtoi(strPort);
		//m_strIP=build_info.strIp; 
		g_CommunicationType=1;
		SetCommunicationType(g_CommunicationType);
		BOOL bOk=FALSE;
		bOk=Open_Socket2(strIP,nPort);
		CString strInfo;

		strInfo.Format((_T("Open IP:%s successful")),strIP);//prompt info;
		SetPaneString(1,strInfo);
		if(bOk)
		{
			try
			{

			_ConnectionPtr t_pCon;//for ado connection
			t_pCon.CreateInstance(_T("ADODB.Connection"));
			t_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);
			CString strSql;
			strSql.Format(_T("update Building set Ip_Address='%s' where Ip_Address='%s'"),strIP,pPraent->m_strIP);
			t_pCon->Execute(strSql.GetString(),NULL,adCmdText);

			// 改node
			CString strSID;
			strSID.Format(_T("%d"), m_nSerialNum);
			strSql.Format(_T("update ALL_NODE set Bautrate='%s' where Serial_ID='%s'"),strIP, strSID); //bautrate 放IP
			t_pCon->Execute(strSql.GetString(),NULL,adCmdText);


			if(t_pCon->State)
				t_pCon->Close();
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
			pPraent->m_strIP=strIP;
			pPraent->ScanTstatInDB();

		}
		else
		{

		}


/*
		_ConnectionPtr t_pCon;//for ado connection
		t_pCon.CreateInstance(_T("ADODB.Connection"));
		t_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);
		CString strSql;
		strSql.Format(_T("update Building set Ip_Address='%s',Ip_Port='%s' where Main_BuildingName='%s' and Building_Name='%s'"),strIP,strPort,strBuilding,strSubBuilding);
		t_pCon->Execute(strSql.GetString(),NULL,adCmdText);
		if(t_pCon->State)
			t_pCon->Close();
		pPraent->OnConnect();
		*/
	}
	//write_one(g_tstat_id,131,1);
	Fresh();
	EndWaitCursor();

}

void CNetworkControllView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CView* pNewView = pMain->m_pViews[1];
	CView* pActiveView = pMain->GetActiveView();

	if ( pNewView != pActiveView )    // current is CM5 dialog
	{
		KillTimer(1);
		return;
	}

	//Fresh();
	CFormView::OnTimer(nIDEvent);
}

void CNetworkControllView::OnDestroy()
{
	CFormView::OnDestroy();
	KillTimer(1);
	// TODO: Add your message handler code here
}

void CNetworkControllView::OnIpnFieldchangedIpaddress1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CNetworkControllView::OnCbnSelchangeBaudrateCombx()
{
	BeginWaitCursor();
	int nSel;
	nSel=m_baudRateCombox.GetCurSel();
	int nRet=-1;
	nRet=write_one(g_tstat_id,12,nSel);
	if(nRet>0)
	{
		if(nSel==0)
		{
			m_baudRateCombox.SetCurSel(0);
			Change_BaudRate(9600);
		}
		if(nRet==1)
		{
			m_baudRateCombox.SetCurSel(1);
			Change_BaudRate(19200);
		}
	}
}
void CNetworkControllView::OnEnKillfocusIdaddressEdit()
{	
	m_nSerialNum=get_serialnumber();
	CString strSerialNum;
	strSerialNum.Format(_T("%d"), m_nSerialNum);

	CString strModbusID;
	m_idEdt.GetWindowText(strModbusID);
	strModbusID.Trim();
	if(strModbusID.IsEmpty())
		return;
	int nID=_wtoi(strModbusID);
	if(nID<=0||nID>=255)
	{
	//	AfxMessageBox(_T("Please input a correct ID!"));
		return;
	}
	if(g_tstat_id==nID)
		return;
	_ConnectionPtr m_pConTmp;
	_RecordsetPtr m_pRsTemp;
	m_pConTmp.CreateInstance("ADODB.Connection");
	m_pRsTemp.CreateInstance("ADODB.Recordset");
	m_pConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
	
	CString strSql;
	strSql.Format(_T("select * from ALL_NODE where Product_ID ='%s'"),strModbusID);
	m_pRsTemp->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
	if(m_pRsTemp->GetRecordCount()>0)
	{
		while(VARIANT_FALSE==m_pRsTemp->EndOfFile)
		{			
			CString strSID=m_pRsTemp->GetCollect("Serial_ID");
			CString strPID=m_pRsTemp->GetCollect("Product_ID");
		
			if( strSID.CompareNoCase(strSerialNum)!=0 && strPID.CompareNoCase(strModbusID)==0 )
			{
				CString strPop;
			//	strPop.Format(_T("There is another device(Serial#=%s) has the ID ='%s', Please input a new one!"), strSID, strModbusID);//scan 点击NC后，再点击它下面的TSTAT
			//	AfxMessageBox(strPop);//scan
				if(m_pRsTemp->State)
					m_pRsTemp->Close();		
				return;	

			}

			m_pRsTemp->MoveNext();
		}
	}

	if(m_pRsTemp->State)
		m_pRsTemp->Close();
	int nRet=0;
	nRet=write_one(g_tstat_id, 6,nID);
	if(nRet>0)
	{
		try
		{

		g_tstat_id=nID;
		CString strSerial;
		strSerial.Format(_T("%d"),m_nSerialNum);
		strSerial.Trim();
		strSql.Format(_T("update ALL_NODE set Product_ID ='%s' where Serial_ID ='%s'"),strModbusID,strSerial);
		m_pConTmp->Execute(strSql.GetString(),NULL,adCmdText);
		}
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}
	
	}
	if(m_pConTmp->State)
		m_pConTmp->Close();
	CMainFrame* pPraent=(CMainFrame*)GetParent();
	pPraent->ScanTstatInDB();
	Fresh();

}

BOOL CNetworkControllView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->wParam == VK_RETURN )
	{ 
		GetDlgItem(IDC_SERIALSTATIC)->SetFocus();
		return true; 
	} 
	return CFormView::PreTranslateMessage(pMsg);
}



	


void CNetworkControllView::OnBnClickedButtonUpdatetimeserver()
{
	BeginWaitCursor();

	BOOL bSyncSuccess = FALSE;
	CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_TIMESERVERLIST);
	CString strTimeServer;
	//pCbx->GetLBText(pCbx->GetCurSel(), strTimeServer);
	pCbx->GetWindowText(strTimeServer);

	CString strTips = _T("NC is synchronizing internet time server, please wait.");
	SetPaneString(1, strTips);



	USES_CONVERSION;
	LPCSTR szIP = W2A(strTimeServer);
	struct hostent* pHost = NULL;
	SOCKADDR_IN sockaddr;
	//TestFunc(strTimeServer);

	//return;
	//////////////////////////////////////////////////////////////////////////
	if(isalpha(szIP[0]))  // by domain name
	{
	
		pHost = gethostbyname(szIP);
		if(pHost != NULL)
		{
			sockaddr.sin_addr.S_un.S_addr = (*(DWORD*)pHost->h_addr_list[0]);
		}
		else
		{
			AfxMessageBox(_T("Time synchronization needs in the internet, please check your net work settings."));
			return;
		}

	}
	else // by ip
	{		
		sockaddr.sin_addr.S_un.S_addr =inet_addr(szIP);
	}
	// write to nc
	int n=write_one(g_tstat_id,178, BYTE(sockaddr.sin_addr.S_un.S_un_b.s_b1 ));
	n=write_one(g_tstat_id,179,BYTE(sockaddr.sin_addr.S_un.S_un_b.s_b2 ));
	n=write_one(g_tstat_id,180,BYTE(sockaddr.sin_addr.S_un.S_un_b.s_b3 ));
	n=write_one(g_tstat_id,181,BYTE(sockaddr.sin_addr.S_un.S_un_b.s_b4 ));

	n=write_one(g_tstat_id,177,6); // 命令寄存器，同步命令

	Sleep(5000);  // 等待NC 同步
	AddNewTimeServer(strTimeServer);

	int nCount = 0;
	while(nCount < 5)
	{
		int nRet = Read_One(m_IDaddress, 199);
		if(nRet == 1)
		{
			bSyncSuccess = TRUE;
			break;
		}
		nCount++;
		Sleep(1000);
	}

	if (bSyncSuccess)
	{
		strTips = _T("NC synchronize internet time server success.");		
	}
	else
	{
		strTips = _T("NC synchronize internet time server failed.");		
	}
	AfxMessageBox(strTips);
	SetPaneString(1, strTips);


	EndWaitCursor();
	
}

void CNetworkControllView::TestFunc(CString strIP)
{
	WSADATA WsaData;
	SOCKADDR_IN sockaddr,sockaddr1,sockaddr2;
	WSAStartup(0x0101,&WsaData);
	hostent *test = NULL;

	CString pAddr = strIP;
	USES_CONVERSION;
	LPCSTR szIP = W2A(strIP);
	test = gethostbyname(szIP);
	if(test != NULL)
	{
		DWORD IpAddr = *test->h_name;
		CString temp;
 
		sockaddr.sin_addr.S_un.S_addr = htonl(*(DWORD*)test->h_addr_list[0]); // host
		sockaddr1.sin_addr.S_un.S_addr = ntohl(*(DWORD*)test->h_addr_list[0]); // net
		sockaddr2.sin_addr.S_un.S_addr = (*(DWORD*)test->h_addr_list[0]);
		temp.Format(_T("%s"),inet_ntoa(sockaddr.sin_addr));
		//        AfxMessageBox(temp);

	}
	WSACleanup();
}

/*
FunctionName:GetIPFromHostName
Comment:Alex
Date:2012/11/30
Purpose:
可以通过域名解析服务器
得到一个网站或者主机对应的IP地址
*/
CString CNetworkControllView::GetIPFromHostName(CString& strHostName) 
{ 
	  
	   CString	str=_T(""); //For IP
		struct hostent* phost;
		int i;
		USES_CONVERSION;
		LPCSTR szHost = LPCSTR(W2A(strHostName));
		phost = gethostbyname(szHost);
		// m_hostname=szhostname;
		if (phost!=NULL)
		{
			i=0;
		int j;
		int h_length=4;
		for( j =0; j < h_length; j++)
		{
			CString addr;
			//这里当时IP的时候如何处理
			if( j > 0)
				str += ".";
			addr.Format(_T("%u"), (unsigned int)((unsigned char*)phost->h_addr_list[i])[j]);
			str += addr;
		} 
		} 
		else
		{
		}
		
	 
	return str; 
} 


// 
// void CNetworkControllView::OnBnClickedFindTstat()
// {
// 	for (int i = 250 ; i < 254; i++)
// 	{
// 		int nRet = read_one(i, 7, 3);
// 		if (nRet > 0)
// 		{
// 			GetTstatInfo(7);
// 		}
// 	}
// }


// 读取NC信息，并加入到数据库
void CNetworkControllView::GetTstatInfo(int nID)
{


}


//
float CNetworkControllView::GetFirmwareVersion()
{
	int nVersionLo = multi_register_value[MODBUS_VERSION_NUMBER_LO];//tstat version			
	int	nVersionHi = multi_register_value[MODBUS_VERSION_NUMBER_HI];

	m_firmwareVersion = float(nVersionHi) + float(nVersionLo/100.0);
	
	return m_firmwareVersion;
}

/*
FunctionName:CheckSettingChanged
Comment:Alex
Date:2012/11/30
Purpose:
确定设定变化的状态
*/
BOOL CNetworkControllView::CheckSettingChanged()
{
	BYTE f1,f2,f3,f4;
	m_ip_addressCtrl.GetAddress(f1,f2,f3,f4);
	
	if (multi_register_value[107] != f1 
		|| multi_register_value[108] != f2 
		|| multi_register_value[109] != f3 
		|| multi_register_value[110] != f4 )
	{
		return TRUE;
	}

	m_subnet_addressCtrl.GetAddress(f1,f2,f3,f4);
	if (multi_register_value[111] != f1 
		|| multi_register_value[112] != f2 
		|| multi_register_value[113] != f3 
		|| multi_register_value[114] != f4 )
	{
		return TRUE;
	}

	m_gateway_addressCtrl.GetAddress(f1,f2,f3,f4);
	if (multi_register_value[115] != f1 
		|| multi_register_value[116] != f2 
		|| multi_register_value[117] != f3 
		|| multi_register_value[118] != f4 )
	{
		return TRUE;
	}

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT8);
	CString strPort;
	pEdit->GetWindowText(strPort);
		
	int nPort = _wtoi(strPort);

	if (m_nListenPort != nPort)
	{
		return TRUE;		
	}

	return FALSE;
}

//*



//HKEY_CURRENT_USER\Software\Temco T3000 Application\T3000\Timeserver


const CString strTSRegRoot = _T("Software\\Temco T3000 Application\\T3000\\Timeserver");
void CNetworkControllView::AddNewTimeServer(const CString& strTimeServer)
{
	BOOL bFind = FALSE;
	UINT i =0;
	for (i = 0; i < m_szServers.size(); i++)
	{
		if (strTimeServer.CompareNoCase(m_szServers[i]) == 0)
		{
			bFind = TRUE;
		}
	}

	if (!bFind)  // new time server
	{		
				
		CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_TIMESERVERLIST);
		if(m_szServers.size() >= 10)
		{
			for (UINT j =5; j < 9; j++)
			{
				m_szServers[j] = m_szServers[j+1];
			}
			m_szServers[9]=strTimeServer;
			pCbx->DeleteString(5);
		}
		else
		{
			m_szServers.push_back(strTimeServer);
		}
		// 
		

		// write to register
		for (UINT i = 5; i < m_szServers.size(); i++)
		{	
			CString strIndex;
			int nsize = m_szServers.size();
			strIndex.Format(_T("server%d"), i+1);
			SaveTimeServerToRegister(m_szServers[i], strIndex);
		}

		pCbx->AddString(strTimeServer);
	}

}
//*/




BOOL CNetworkControllView::SaveTimeServerToRegister(const CString& strVal, const CString& strKey)
{
	CRegKey reg;

	//CString strEntry = strTSRegRoot + _T("\\")+strKey;

	if(reg.Create(HKEY_CURRENT_USER, strTSRegRoot) == ERROR_SUCCESS)
	{
		LONG lRet = reg.SetStringValue(strKey, strVal);
		if(lRet != ERROR_SUCCESS)
		{ // failed
			ASSERT(0);
		}
	}	
	else // 没有这个表项
	{
		ASSERT(0);		
		return FALSE;
	}	
	reg.Close();

	return TRUE;
}

BOOL CNetworkControllView::LoadTimeServerFromRegister(const CString& strKey ,CString& strVal)
{
	CRegKey reg;
	
	//CString strEntry = strTSRegRoot + _T("\\")+ strKey;
	LPTSTR  szRet = new TCHAR[128];
	ZeroMemory(szRet, sizeof(TCHAR)*128);
	DWORD dwSize = 128;

	if(reg.Open(HKEY_CURRENT_USER, strTSRegRoot) == ERROR_SUCCESS)
	{
		if(reg.QueryStringValue(strKey, szRet, &dwSize) == ERROR_SUCCESS)
		{
			strVal = CString(szRet);
		}
		else
		{
			delete []szRet;
			return FALSE;
		}	
	}	
	else // 没有这个表项
	{
		delete []szRet;
		return FALSE;
	}


	reg.Close();

	delete []szRet;
	return TRUE;
}



void CNetworkControllView::initTimeServerList()
{

	m_szServers.clear();
	CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_TIMESERVERLIST);
	pCbx->ResetContent();
	pCbx->Clear();
	for (int i = 0 ; i < 5; i++)
	{
		m_szServers.push_back(c_strTimeserverList[i]);
	}

	CString strIndex;
	CString strValue;
	for (int i = 6; i <= 10; i++)
	{
		strIndex.Format(_T("server%d"), i);
		if(LoadTimeServerFromRegister(strIndex, strValue))
		{
			m_szServers.push_back(strValue);
		}
		else
		{
			break;
		}
	}

	for (UINT n = 0; n < m_szServers.size(); n++)
	{
		pCbx->AddString(m_szServers[n]);
	}
	pCbx->SetCurSel(0);

}




void CNetworkControllView::InitGrid()
{
// 		7000~7001	2	How many nodes in the table
// 		7002~7003	2	Net# and Address of first node in table
// 		7004~7013	10	Serial # and so on (register1~register10)
// 		7014	1	cooling setpoint value of Sub Tstats(register380)
// 		7015	1	setpoint value of Sub Tstats(register135)
// 		7016	1	heating setpoint value of Sub Tstats(register136)
// 		7017	1	room temperture value of Sub Tstats(register101)
// 		7018	1	mode of Sub Tstats(register107)
// 		7019	1	output state of Sub Tstats(register108)
// 		7020	1	night heatting deadband of Sub Tstats(register123)
// 		7021	1	night cooling deadband  of Sub Tstats(register124)
// 		7022	1	night heatting setpoint value of Sub Tstats(register182)
// 		7023	1	night cooling setpoint value of Sub Tstats(register183)
// 		7024	1	Occupied status of SUB tstats(register184.0) (?Not sure what this is?)
// 		7025	1	Unoccupied Override Timer(register211) 
// 		7026~7028	3	Reserved
// 		7029	1	State:on-line or off-line
// 		7030~7031	2	Time HH:MM since last comms with 1st node in table

// 	m_gridSub.put_TextMatrix(0,0,_T("Product Type"));
// 	m_gridSub.put_TextMatrix(0,1,_T("Subnet"));
// 	m_gridSub.put_TextMatrix(0,2,_T("Serial No."));
// 	m_gridSub.put_TextMatrix(0,3,_T("Address"));
// 	m_gridSub.put_TextMatrix(0,4,_T("Status"));
// 	m_gridSub.put_TextMatrix(0,5,_T("Time Since"));


	//设置排/行数量
	
	
	m_gridSub.put_Cols(10);//这句一定要有，如果下面再增加显示一外，而这里不变还是5，则会出错
	m_gridSub.put_Rows(501);//包括标题栏
	//set row high
	m_gridSub.put_WordWrap(TRUE);
	m_gridSub.put_RowHeight(0,500);
	//设置列宽
	m_gridSub.put_ColWidth(0,400);
	m_gridSub.put_ColWidth(1,800);
	m_gridSub.put_ColWidth(2,800);
	m_gridSub.put_ColWidth(3,1000);
	m_gridSub.put_ColWidth(4,1000);
	m_gridSub.put_ColWidth(5,1000);
	m_gridSub.put_ColWidth(6,1000);
	m_gridSub.put_ColWidth(7,1000);
	m_gridSub.put_ColWidth(8,1200);
	m_gridSub.put_ColWidth(9,1200);



	m_gridSub.put_TextMatrix(0,0,_T("No"));
	m_gridSub.put_TextMatrix(0,1,_T("Device"));
	m_gridSub.put_TextMatrix(0,2,_T("Address"));		  // 		7002~7003
	m_gridSub.put_TextMatrix(0,3,_T("Cooling\nSetpoint"));// 		7014
	m_gridSub.put_TextMatrix(0,4,_T("Heating\nSetpoint"));// 		7016
	m_gridSub.put_TextMatrix(0,5,_T("Setpoint"));		  // 		7015
	m_gridSub.put_TextMatrix(0,6,_T("Room\nTemperture")); // 		7017
	m_gridSub.put_TextMatrix(0,7,_T("Mode"));			  // 		7018
	m_gridSub.put_TextMatrix(0,8,_T("Night Heating\nSetpoint"));// 		7022
	m_gridSub.put_TextMatrix(0,9,_T("Night Cooling\nSetpoint"));// 		7023


	for(int i = 0; i < 10; i++)
	{
		m_gridSub.put_ColAlignment(i,4);
	}

	WORD data[30*500+2];
	memset(data,0,sizeof(data));
	int ret3 = Read_One(g_tstat_id,7001);
	int ret = Read_One(g_tstat_id,7000);//NC扫描到的TSTAT数量,2个字节 先放在高字节
	if (ret>0&&ret<4)
	{
		Read_Multi(g_tstat_id,&data[0],7002,ret*30);
	}
	else if(ret<=245)
	{
		int ret1 = Read_One(g_tstat_id,7003);//第一个TSTAT ID地址
		if (ret1!= 255&&ret1!=0)
		{
			int i =0;
			for (i;i<ret;i++)
			{
				if ((data[i*30+1]!=0)&&(i>0))
				{
					Read_Multi(g_tstat_id,&data[30*i],7002+i*30,30);
				}else
				{
					ret = i;
					break;
				}
				
			}
		}else
		{
			ret = 0;//不显示TSTAT列表
		}

		
	}else
	{
		ret = 0;
	}

	if (ret!=0)
	{
		ret = ret+ret3*256;
	}
	
	_subnetwork.clear();
	for (int i = 0;i<ret;i++)
	{


		m_subetwork.m_coolingSet.Format(_T("%.1f°C"),(float)data[i*30+12]/10);
		m_subetwork.m_heatingSet.Format(_T("%.1f°C"),(float)data[i*30+14]/10);
		m_subetwork.m_setpoint.Format(_T("%.1f°C"),(float)data[i*30+13]/10); 
		m_subetwork.m_roomTemper.Format(_T("%.1f°C"),(float)data[i*30+15]/10); 
		m_subetwork.m_mode = m_buffer[i*30+16];     
		m_subetwork.m_outputState.Format(_T("%.1f"),(float)data[i*30+17]/10); 
		m_subetwork.m_nightHeating.Format(_T("%.1f°C"),(float)data[i*30+20]/10);
		m_subetwork.m_nightCooling.Format(_T("%.1f°C"),(float)data[i*30+21]/10); 
		m_subetwork.m_tstatProduct = data[i*30+5]; 
		m_subetwork.m_modbusaddr.Format(_T("%d"),data[i*30+1]);
		_subnetwork.push_back(m_subetwork);

	}


	CString m_num,strtemp1,strtemp2;


	for (int i=1;i<ret+1;i++)
	{
// 		int ret = _subnetwork.at(i-1).m_modbusaddr.Compare(_T("0"));
// 		if(ret == 0)
// 			continue;

		m_num.Format(_T("%d"),i);
		m_gridSub.put_TextMatrix(i,0,m_num);//第一列：序号



		switch(_subnetwork.at(i-1).m_tstatProduct)
		{
		case 1:strtemp1=g_strTstat5b;break;
		case 2:strtemp1=g_strTstat5a;break;
		case 3:strtemp1=g_strTstat5b;break;
		case 4:strtemp1=g_strTstat5c;break;
		case 12:			strtemp1=g_strTstat5d;break;
		case 100:		strtemp1=g_strnetWork;break;
		case NET_WORK_OR485_PRODUCT_MODEL:strtemp1=g_strOR485;break;
		case 17:strtemp1=g_strTstat5f;break;
		case 18:strtemp1=g_strTstat5g;break;
		case 16:strtemp1=g_strTstat5e;break;
		case 19:strtemp1=g_strTstat5h;break;

		case 26:strtemp1=g_strTstat6;break;
		case 27:strtemp1=g_strTstat7;break;
		case 13:
		case 14:break;
		default:strtemp1=g_strTstat5a;break;
		}
		m_gridSub.put_TextMatrix(i,1,strtemp1);

		m_gridSub.put_TextMatrix(i,2,_subnetwork.at(i-1).m_modbusaddr);
		m_gridSub.put_TextMatrix(i,3,_subnetwork.at(i-1).m_coolingSet);
		m_gridSub.put_TextMatrix(i,4,_subnetwork.at(i-1).m_heatingSet);
		m_gridSub.put_TextMatrix(i,5,_subnetwork.at(i-1).m_setpoint);
		m_gridSub.put_TextMatrix(i,6,_subnetwork.at(i-1).m_roomTemper);







		switch (_subnetwork.at(i-1).m_tstatProduct)		 
		{
		case 0:
			strtemp2.Format(_T("%s"),_T("COASTING"));
			//coasting_temp++;
			break;
		case 1:
			strtemp2.Format(_T("%s"),_T("COOLING1"));
			//cooling_temp++;
			break;
		case 2:
			strtemp2.Format(_T("%s"),_T("COOLING2"));	
			//cooling_temp++;
			break;
		case 3:
			strtemp2.Format(_T("%s"),_T("COOLING3"));
			//cooling_temp++;
			break;
		case 4:
			strtemp2.Format(_T("%s"),_T("HEATING1"));
			//heating_temp++;
			break;
		case 5:
			strtemp2.Format(_T("%s"),_T("HEATING2"));
			//heating_temp++;
			break;
		case 6:
			strtemp2.Format(_T("%s"),_T("HEATING3"));
			//heating_temp++;
			break;

		default:
			strtemp2.Format(_T("%s"),_T("COASTING"));
			//coasting_temp++;
			break;
		}
		m_gridSub.put_TextMatrix(i,7,strtemp2);
 

		m_gridSub.put_TextMatrix(i,8,_subnetwork.at(i-1).m_nightHeating);
		m_gridSub.put_TextMatrix(i,9,_subnetwork.at(i-1).m_nightCooling);
	}


}



const int TABLE_NODE_NUM_REG = 7000;
const int TABLE_NODE_SIZE = 20;				// 一个node占用20寄存器
void  CNetworkControllView::ReadNCTable()
{
	WORD wNumLow = Read_One(m_IDaddress, TABLE_NODE_NUM_REG);
	if (wNumLow < 0)
	{
		return;
	}
	WORD wNum = Read_One(m_IDaddress, TABLE_NODE_NUM_REG+1);
	wNum = (wNum << 8) + wNumLow;


	unsigned short* szNode = new unsigned short[TABLE_NODE_SIZE];
	
	for (int i = 0; i < wNum; i++)
	{
		ZeroMemory(szNode, TABLE_NODE_SIZE);
		int nRet = Read_Multi(m_IDaddress, szNode, TABLE_NODE_NUM_REG+2, TABLE_NODE_SIZE);
		if (nRet >0)
		{
			AddNodeToNCTable(szNode);
		}
			
	}

	delete []szNode;
}


void  CNetworkControllView::AddNodeToNCTable(unsigned short* pNode)
{
	int nRow = m_gridSub.get_Row();
	int nSubnet = pNode[0];
	int nAddress = pNode[1];
	long nSerialNo = pNode[2]+pNode[3]*256+pNode[4]*256*256+pNode[5]*256*256*256;
	
	float fSWVersion = pNode[6] + pNode[7]*256;
	int nProductType = pNode[9];
	
	int nStatus = pNode[17];

	int nHour = pNode[18];
	int nMin = pNode[19];

	CString strTemp;

	strTemp.Format(_T("%d"), nRow);
	m_gridSub.put_TextMatrix(nRow,NO_FIELD, strTemp);	
	strTemp = get_product_class_name_by_model_ID(nProductType);
	m_gridSub.put_TextMatrix(nRow,PRODUCT_FIELD,strTemp);
	strTemp.Format(_T("%d"), nSubnet);
	m_gridSub.put_TextMatrix(nRow,SUBNET_FIELD,strTemp);
	strTemp.Format(_T("%d"), nSerialNo);
	m_gridSub.put_TextMatrix(nRow,SERIALID_FIELD,strTemp);
	strTemp.Format(_T("%d"), nAddress);
	m_gridSub.put_TextMatrix(nRow,ADDRESS_FIELD,strTemp);
 	if (nStatus == 0)
 	{
		strTemp = _T("Off line");
 	}else
	{
		strTemp = _T("On line");
	}
 	m_gridSub.put_TextMatrix(nRow, STATUS_FIELD,strTemp);
	
	strTemp.Format(_T("%d:%d"), nHour, nMin);
	m_gridSub.put_TextMatrix(nRow,TIMESINCE_FIELD,strTemp);



}



void CNetworkControllView::OnBnClickedButton1()
{
	//Clear schedules//0917

	//if ( AfxMessageBox(_T("Reset to factory configs"),MB_YESNO ,4) == IDYES )
	if ( AfxMessageBox(_T("Are you sure clear schedules?"),MB_YESNO ,4) == IDYES )
	{
		// 	if (timeserve.CompareNoCase(strTemp)!= 0)
		// 	{

		int iRet = write_one(g_tstat_id,15,85);
		if (iRet>0)

			AfxMessageBox(_T("Clear schedules successful!"));
		else
			AfxMessageBox(_T("Clear schedules failure!\nPlease try again."));

	}
}
