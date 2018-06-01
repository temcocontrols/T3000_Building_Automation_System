// NetworkControllView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "NetworkControllView.h"
#include "global_function.h"
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


	InitGrid();

	

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

	Fresh();
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
				strPop.Format(_T("There is another device(Serial#=%s) has the ID ='%s', Please input a new one!"), strSID, strModbusID);
				AfxMessageBox(strPop);
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
		g_tstat_id=nID;
		CString strSerial;
		strSerial.Format(_T("%d"),m_nSerialNum);
		strSerial.Trim();
		strSql.Format(_T("update ALL_NODE set Product_ID ='%s' where Serial_ID ='%s'"),strModbusID,strSerial);
		m_pConTmp->Execute(strSql.GetString(),NULL,adCmdText);
	
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
		//        temp.Format("%d",IpAddr);
		//        MessageBox(temp);
		sockaddr.sin_addr.S_un.S_addr = htonl(*(DWORD*)test->h_addr_list[0]); // host
		sockaddr1.sin_addr.S_un.S_addr = ntohl(*(DWORD*)test->h_addr_list[0]); // net
		sockaddr2.sin_addr.S_un.S_addr = (*(DWORD*)test->h_addr_list[0]);
		temp.Format(_T("%s"),inet_ntoa(sockaddr.sin_addr));
		//        AfxMessageBox(temp);

	}
	WSACleanup();
}


CString CNetworkControllView::GetIPFromHostName(CString& strHostName) 
{ 
	//char	szhostname[128]; 
	CString	str; 
	//获得主机名 
//	if(   gethostname(szhostname,   128)   ==   0   ) 
	{
		//   获得主机ip地址 
		struct hostent* phost;
		int i;
		USES_CONVERSION;
		LPCSTR szHost = LPCSTR(W2A(strHostName));
		phost = gethostbyname(szHost);
		// m_hostname=szhostname;
		i=0;
		int j;
		int h_length=4;
		for( j =0; j < h_length; j++)
		{
			CString addr;
			if( j > 0)
				str += ".";
			addr.Format(_T("%u"), (unsigned int)((unsigned char*)phost->h_addr_list[i])[j]);
			str += addr;
		} 
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
	m_gridSub.put_TextMatrix(0,NO_FIELD, _T(""));	
	m_gridSub.put_TextMatrix(0,PRODUCT_FIELD,_T("Product Type"));
	m_gridSub.put_TextMatrix(0,SUBNET_FIELD,_T("Subnet"));
	m_gridSub.put_TextMatrix(0,SERIALID_FIELD,_T("Serial NO."));
	m_gridSub.put_TextMatrix(0,ADDRESS_FIELD,_T("Address"));
	m_gridSub.put_TextMatrix(0,STATUS_FIELD,_T("Status"));
	m_gridSub.put_TextMatrix(0,TIMESINCE_FIELD,_T("Time Since"));


	for(int i = 0; i < 7; i++)
	{
		m_gridSub.put_ColAlignment(i,4);
	}

	m_gridSub.put_ColWidth(0,600);		//no
	m_gridSub.put_ColWidth(1,1200);		//product
	m_gridSub.put_ColWidth(2,800);		//subnet
	m_gridSub.put_ColWidth(3,1400);		//sid
	m_gridSub.put_ColWidth(4,1400);		//add

	m_gridSub.put_ColWidth(5,1000);	//port
	m_gridSub.put_ColWidth(6,1200);	//time since
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


