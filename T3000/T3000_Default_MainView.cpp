// T3000_Default_MainView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T3000_Default_MainView.h"
#include "MainFrm.h"
 

// T3000_Default_MainView

IMPLEMENT_DYNCREATE(T3000_Default_MainView, CFormView)

T3000_Default_MainView::T3000_Default_MainView()
	: CFormView(T3000_Default_MainView::IDD)
{

}

T3000_Default_MainView::~T3000_Default_MainView()
{
}

void T3000_Default_MainView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_IPMODEL_COMBO, m_ip_model);
    DDX_Control(pDX, IDC_IPADDRESS1, m_ipaddress);
    DDX_Control(pDX, IDC_IPADDRESS2, m_Subnet_Mask);
    DDX_Control(pDX, IDC_IPADDRESS3, m_GatewayAddress);
    DDX_Control(pDX, IDC_MAC_ADDRESS, m_Edit_MacAddress);
    DDX_Control(pDX, IDC_EDIT_LISTENPORT, m_ListenPort);
}

BEGIN_MESSAGE_MAP(T3000_Default_MainView, CFormView)
	ON_BN_CLICKED(IDC_EDIT_ID, &T3000_Default_MainView::OnBnClickedEditId)
    ON_BN_CLICKED(IDC_BUTTON_GO_TO_INPUT, &T3000_Default_MainView::OnBnClickedButtonGoToInput)
    ON_BN_CLICKED(IDC_APPLYBUTTON, &T3000_Default_MainView::OnBnClickedApplybutton)
    ON_CBN_SELCHANGE(IDC_IPMODEL_COMBO, &T3000_Default_MainView::OnCbnSelchangeIpmodelCombo)
END_MESSAGE_MAP()


// T3000_Default_MainView diagnostics

#ifdef _DEBUG
void T3000_Default_MainView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void T3000_Default_MainView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// T3000_Default_MainView message handlers


void T3000_Default_MainView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}

void T3000_Default_MainView::Fresh(){
    CString strTemp;
	strTemp.Format(_T("%d"),g_tstat_id);
	GetDlgItem(IDC_ID_EDIT)->SetWindowTextW(strTemp);
	strTemp.Format(_T("%d"),get_serialnumber());
	GetDlgItem(IDC_SERIALNUM_EDIT)->SetWindowTextW(strTemp);

	strTemp.Format(_T("%0.1f"),get_curtstat_version());
	GetDlgItem(IDC_FIRMWARV_EDIT)->SetWindowTextW(strTemp);
	strTemp.Format(_T("%d"),product_register_value[8]);//8
	GetDlgItem(IDC_HARDWAREV_EDIT)->SetWindowTextW(strTemp);
	int nModel=product_register_value[7];
	GetDlgItem(IDC_MODEL_EDIT)->SetWindowTextW(GetProductName(nModel));
	strTemp.Format(_T("%d"),product_register_value[9]);
	GetDlgItem(IDC_PIC_EDIT)->SetWindowText(strTemp);

    if (product_register_value[7] == PM_T322AI || product_register_value[7] == PM_T38AI8AO6DO)
    {
        CString strtemp;
        int CO2_NET_MODBUS_MAC_ADDRESS_START = 40;
        strtemp.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"),product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START],product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START+1],product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START+2],product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START+3],product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START+4],product_register_value[CO2_NET_MODBUS_MAC_ADDRESS_START+5]);
        m_Edit_MacAddress.SetWindowText(strtemp);

        int CO2_NET_MODBUS_IP_MODE = 46;
        m_ip_model.SetCurSel(product_register_value[CO2_NET_MODBUS_IP_MODE]);
        //CO2_NET_MODBUS_IP_ADDRESS_START
        //CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_START
        //CO2_NET_MODBUS_GATEWAY_ADDRESS_START
        //CO2_NET_MODBUS_TCP_SERVER_ENABLE
        //CO2_NET_MODBUS_LISTEN_PORT_AT_TCP_SERVER_MODE_START
        int   CO2_NET_MODBUS_IP_ADDRESS_START = 47;
        m_ipaddress.SetAddress((BYTE)product_register_value[CO2_NET_MODBUS_IP_ADDRESS_START],
            (BYTE)product_register_value[CO2_NET_MODBUS_IP_ADDRESS_START+1],
            (BYTE)product_register_value[CO2_NET_MODBUS_IP_ADDRESS_START+2],
            (BYTE)product_register_value[CO2_NET_MODBUS_IP_ADDRESS_START+3]);
         int   CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_START  =     51;
        m_Subnet_Mask.SetAddress((BYTE)product_register_value[CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_START],
            (BYTE)product_register_value[CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_START+1],
            (BYTE)product_register_value[CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_START+2],
            (BYTE)product_register_value[CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_START+3]);
             int   CO2_NET_MODBUS_GATEWAY_ADDRESS_START  =     55;

        m_GatewayAddress.SetAddress((BYTE)product_register_value[CO2_NET_MODBUS_GATEWAY_ADDRESS_START],
            (BYTE)product_register_value[CO2_NET_MODBUS_GATEWAY_ADDRESS_START+1],
            (BYTE)product_register_value[CO2_NET_MODBUS_GATEWAY_ADDRESS_START+2],
            (BYTE)product_register_value[CO2_NET_MODBUS_GATEWAY_ADDRESS_START+3]);
          int CO2_NET_MODBUS_LISTEN_PORT_AT_TCP_SERVER_MODE_START =  60;
        strtemp.Format(_T("%d"),product_register_value[CO2_NET_MODBUS_LISTEN_PORT_AT_TCP_SERVER_MODE_START]);
        m_ListenPort.SetWindowText(strtemp);
    }


}
#include "EreaseDlg.h"

void T3000_Default_MainView::OnBnClickedEditId()
{
	 CEreaseDlg dlg;
	 dlg.DoModal();

     CString str;
     str.Format (_T("%d"),g_tstat_id);

     GetDlgItem (IDC_ID_EDIT)->SetWindowTextW(str);
}


void T3000_Default_MainView::OnBnClickedButtonGoToInput()
{
     /* if (product_type == T3000_T3_MODULES )
     {
         SwitchToPruductType(DLG_DIALOG_T3_INPUTS_VIEW);
    }*/
    CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    pFrame->OnControlInputs ();
}


void T3000_Default_MainView::OnBnClickedApplybutton()
{
    int m_nListenPort=0;
    BeginWaitCursor();
    //m_nListenPort	
    CString strText;
    m_ListenPort.GetWindowText(strText);
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
    write_one(g_tstat_id,75,m_nListenPort);
    product_register_value[75]=m_nListenPort;
    BYTE address1,address2,address3,address4;
    m_ipaddress.GetAddress(address1,address2,address3,address4);

    int sel = m_ip_model.GetCurSel ();
     write_one(g_tstat_id,61,sel);

    int CO2_NET_MODBUS_IP_ADDRESS_GHOST_START = 62 ;
    int n=write_one(g_tstat_id,CO2_NET_MODBUS_IP_ADDRESS_GHOST_START,address1);
    n=write_one(g_tstat_id,CO2_NET_MODBUS_IP_ADDRESS_GHOST_START+1,address2);
    n=write_one(g_tstat_id,CO2_NET_MODBUS_IP_ADDRESS_GHOST_START+2,address3);
    n=write_one(g_tstat_id,CO2_NET_MODBUS_IP_ADDRESS_GHOST_START+3,address4);
    int CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_GHOST_START = 66;
    m_Subnet_Mask.GetAddress(address1,address2,address3,address4);
    n=write_one(g_tstat_id,CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_GHOST_START+0,address1);
    n=write_one(g_tstat_id,CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_GHOST_START+1,address2);
    n=write_one(g_tstat_id,CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_GHOST_START+2,address3);
    n=write_one(g_tstat_id,CO2_NET_MODBUS_SUBNET_MASK_ADDRESS_GHOST_START+3,address4);
    m_GatewayAddress.GetAddress(address1,address2,address3,address4);
    int CO2_NET_MODBUS_GATEWAY_ADDRESS_GHOST_START =   70;
    n=write_one(g_tstat_id,CO2_NET_MODBUS_GATEWAY_ADDRESS_GHOST_START,address1);
    n=write_one(g_tstat_id,CO2_NET_MODBUS_GATEWAY_ADDRESS_GHOST_START+1,address2);
    n=write_one(g_tstat_id,CO2_NET_MODBUS_GATEWAY_ADDRESS_GHOST_START+2,address3);
    n=write_one(g_tstat_id,CO2_NET_MODBUS_GATEWAY_ADDRESS_GHOST_START+3,address4);
    int CO2_NET_MODBUS_ENABLE_GHOST = 76;
    n=write_one(g_tstat_id,CO2_NET_MODBUS_ENABLE_GHOST,1);//使能之后

    Sleep(5*1000);



    CMainFrame* pPraent=(CMainFrame*)GetParent();

    CString strBuilding,strSubBuilding;
    strBuilding=pPraent->m_strCurMainBuildingName;
    strSubBuilding=pPraent->m_strCurSubBuldingName;
    CString strIP,strPort;
    strPort.Format(_T("%d"),product_register_value[75]);
    // BYTE address1,address2,address3,address4;
    m_ipaddress.GetAddress(address1,address2,address3,address4);
    strIP.Format(_T("%d.%d.%d.%d"),address1,address2,address3,address4);
    if(g_CommunicationType==1)//TCP
    {
       /* int nPort;
        nPort=_wtoi(strPort);*/
        //m_strIP=build_info.strIp; 
        g_CommunicationType=1;
        SetCommunicationType(g_CommunicationType);
        BOOL bOk=FALSE;
        bOk=Open_Socket2(strIP,m_nListenPort);
        CString strInfo;

        strInfo.Format((_T("Open IP:%s successful")),strIP);//prompt info;
        SetPaneString(1,strInfo);
        if(bOk)
        {
            try
            {
                CBADO bado;
                bado.SetDBPath(g_strCurBuildingDatabasefilePath);
                bado.OnInitADOConn(); 

                CString strSql;
                //strSql.Format(_T("update Building set Ip_Address='%s' where Ip_Address='%s'"),strIP,pPraent->m_strIP);
                //t_pCon->Execute(strSql.GetString(),NULL,adCmdText);

                // 改node
                CString strSID;

                strSID.Format(_T("%d"), get_serialnumber());
                strSql.Format(_T("update ALL_NODE set Bautrate='%s',Com_Port='%s' where Serial_ID='%s'"),strIP,strPort,strSID); //bautrate 放IP
                bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);

                bado.CloseConn();

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



    }
    EndWaitCursor();
}


void T3000_Default_MainView::OnCbnSelchangeIpmodelCombo()
{   
}
