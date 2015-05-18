// ARDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "ARDDlg.h"
#include "afxdialogex.h"
#include "ado/ADO.h"

// CARDDlg dialog

IMPLEMENT_DYNAMIC(CARDDlg, CDialogEx)

CARDDlg::CARDDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CARDDlg::IDD, pParent)
{

}

BOOL CARDDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetSerialComPortNumber1(m_szComm);
	for (UINT i = 0; i < m_szComm.size(); i++)
	{
		m_add_device_com_port.AddString(m_szComm[i]);
	}
	m_add_device_com_port.SetCurSel(0);

	m_add_device_baudrate.InsertString(0,_T("9600"));
	m_add_device_baudrate.InsertString(1,_T("19200"));
	m_add_device_baudrate.SetCurSel(1);
	m_add_device_modbus_id.SetWindowText(_T("255"));
	m_ipaddress.SetWindowText(_T("97.77.120.35"));
	m_porteditor.SetWindowText(_T("502"));

	((CButton *)GetDlgItem(IDC_RADIO_NET_DEVICE))->SetCheck(TRUE);//选上
	Enable_Net_UI(true);


	return TRUE; 
}

void CARDDlg::Enable_Net_UI(bool enable_net)
{
	m_is_net_device = enable_net;
	if(enable_net)
	{
		m_ipaddress.EnableWindow(1);
		m_porteditor.EnableWindow(1);

		m_add_device_com_port.EnableWindow(0);
		m_add_device_baudrate.EnableWindow(0);
		m_add_device_modbus_id.EnableWindow(0);
	}
	else
	{
		m_ipaddress.EnableWindow(0);
		m_porteditor.EnableWindow(0);

		m_add_device_com_port.EnableWindow(1);
		m_add_device_baudrate.EnableWindow(1);
		m_add_device_modbus_id.EnableWindow(1);
	}
}

CARDDlg::~CARDDlg()
{
}

void CARDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ipaddress);
	DDX_Control(pDX, IDC_PORT, m_porteditor);
	DDX_Control(pDX, IDC_COMBO_ADD_DEVICE_COMPORT, m_add_device_com_port);
	DDX_Control(pDX, IDC_COMBO_ADD_DEVICE_BAUDRATE, m_add_device_baudrate);
	DDX_Control(pDX, IDC_EDIT_ADD_DEVICE_MODBUS_ID, m_add_device_modbus_id);
}


BEGIN_MESSAGE_MAP(CARDDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CARDDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_NET_DEVICE, &CARDDlg::OnBnClickedRadioNetDevice)
	ON_BN_CLICKED(IDC_RADIO2, &CARDDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON_LOCAL_MSTP_DEVICE, &CARDDlg::OnBnClickedButtonLocalMstpDevice)
END_MESSAGE_MAP()


// CARDDlg message handlers


void CARDDlg::OnBnClickedOk()
{
   CString ip;
   CString strport;
   BOOL is_open = false;

   int temp_communication_type = GetLastCommunicationType();
   int temp_comport = 0;
   int temp_baudrate = 19200;
   int read_modbus_id = 255;
   CString temp_cs_port;
   CString temp_baud;
   CString temp_id;
   int nComport = 0;
   if(temp_communication_type == 0)
   {
	   temp_comport = GetLastOpenedComport();
	   temp_baudrate = GetLastSuccessBaudrate();
   }

   if(m_is_net_device)
   {
	   m_ipaddress.GetWindowText(ip);
	   m_porteditor.GetWindowText(strport);
	   if (ip.IsEmpty()||strport.IsEmpty())
	   {
		   AfxMessageBox(_T("ip or port can not be empty!"));
		   return;
	   }
	   short port=_wtoi(strport);
	   SetCommunicationType(1);
	   is_open=Open_Socket2(ip,port);
   }
   else
   {
	   SetCommunicationType(0);
	   close_com();

	   m_add_device_com_port.GetWindowTextW(temp_cs_port);
	   m_add_device_baudrate.GetWindowTextW(temp_baud);
	   m_add_device_modbus_id.GetWindowTextW(temp_id);
	   
	    nComport = _wtoi(temp_cs_port.Mid(3));
		int nbaudrate = _wtoi(temp_baud);
		read_modbus_id = _wtoi(temp_id);

		if((nbaudrate != 19200) && (nbaudrate != 9600))
		{
			is_open = false;
		}
		else
		{
			if(nComport > 0)
			{
				BOOL  bret = open_com(nComport);
				Change_BaudRate(nbaudrate);
				if(bret)
					is_open = true;
				else
					is_open = false;
			}
			else
			{
				is_open = false;
			}
		}



   }




   if (is_open)
   {
       //unsigned short data[2];
	   //int ModelID=Read_Multi(209,data,6,2);//  (255,6,20);
	   //CString show;
	   //show.Format(_T("Add=%d,Model=%d"),data[0],data[1]);
	   //ModelID=read_one(data[0],7);
	   //AfxMessageBox(show);
	   //show.Format(_T("Model=%d"),ModelID);
	   //AfxMessageBox(show);
	  unsigned short Data[10];
	  int ret=Read_Multi(read_modbus_id,Data,0,10);
	  if (ret<0)
	  {
	  AfxMessageBox(_T("Can't get the information of your device!\n Please try again."));
	   return;//	ret;
	  }

	   CString strSql;
	   CString strMainBuildName,strSubBuildingName,strSID,strFloorName,strRoomName,strProID;
	   CString strProName,strProType,strScreenID,strBaudrate,strGraphicID,strHdVersion,strStVersion,strCom,strEPSize;

	  
	   
	 
	   CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	   strMainBuildName= pFrame->m_strCurMainBuildingName;
	   strSubBuildingName= pFrame->m_strCurSubBuldingName;
	   int SN=Data[3]*256*256*256+Data[2]*256*256+Data[1]*256+Data[0];
       strSID.Format(_T("%d"),SN);
	   strFloorName=_T("floor1");
	   strRoomName=_T("room1");
	   strProName.Format(_T("%s:%d--%d"),GetProductName(Data[7]).GetBuffer(),SN,Data[6]);
	   strProType.Format(_T("%d"),Data[7]);
	   strProID.Format(_T("%d"),Data[6]);

	   strScreenID.Format(_T("Screen(S:%d--%d)"),SN,Data[6]);
	   if(m_is_net_device)
		m_ipaddress.GetWindowText(strBaudrate);
	   else
		   strBaudrate = temp_baud;
	   strGraphicID=_T("Clicking here to add a image...");
	   strHdVersion=_T("0.0");
	   strStVersion=_T("0.0");
	   if(m_is_net_device)
		 m_porteditor.GetWindowText(strCom);
	   else
	   {
		   CString temp1;
		   temp1.Format(_T("%d"),nComport);
			strCom = temp1;
	   }
	   strEPSize=_T("0.0");
	   try
	   {
		   CBADO bado;
		   bado.SetDBPath(g_strCurBuildingDatabasefilePath);
		   bado.OnInitADOConn(); 
           strSql.Format(_T("Select * from ALL_NODE where Product_name='%s'"),strProName.GetBuffer());
           bado.m_pRecordset=bado.OpenRecordset(strSql);
           if (!bado.m_pRecordset->EndOfFile)//有表但是没有对应序列号的值
           {
            AfxMessageBox(_T("Already Exists!"));
            return;
            //ado.m_pRecordset->Close();
           }
		   strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize) values('"
			   +strMainBuildName+"','"
			   +strSubBuildingName +"','"
			   +strSID+"','"
			   +strFloorName+"','"
			   +strRoomName+"','"
			   +strProName+"','"
			   +strProType+"','"
			   +strProID+"','"
			   +strScreenID+"','"
			   +strBaudrate+"','"
			   +strGraphicID+"','"
			   +strHdVersion+"','"
			   +strStVersion+"','"
			   +strCom+"','"
			   +strEPSize+"')"));
		   bado.m_pRecordset=bado.OpenRecordset(strSql);
		   bado.CloseConn();
		   AfxMessageBox(_T("Add OK!"));
           CDialogEx::OnOK();
	   }
	   
	   catch (CException* e)
	   {
		   AfxMessageBox(_T("Add Fail!"));
	   }
	  
	   
   }
   else
   {
	   AfxMessageBox(_T("Can not Connect"));
   }

}


void CARDDlg::OnBnClickedRadioNetDevice()
{
	// TODO: Add your control notification handler code here
	Enable_Net_UI(1);
}


void CARDDlg::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	Enable_Net_UI(0);
}


void CARDDlg::OnBnClickedButtonLocalMstpDevice()
{
	// TODO: Add your control notification handler code here
	CBADO bado;
	bado.SetDBPath(g_strCurBuildingDatabasefilePath);
	bado.OnInitADOConn(); 

	MessageBox(_T("Scan device may take a few minutes , Please wait!!"));
	vector<CString>				m_szComs;
	GetSerialComPortNumber1(m_szComs);
	int com_port_count =  m_szComs.size();
	for (int z=1;z<com_port_count;z++)
	{
		CString temp_cstring;
		temp_cstring = m_szComs.at(z).Right(m_szComs.at(z).GetLength() - 3);
		int com_port =  _wtoi(temp_cstring);
		Initial_bac(com_port);




		for (int x=0;x<25;x++)
		{
			//CString strInfo;strInfo.Format(_T("Scan Com%d,protocol Bacnet MSTP.Send global command time left(%d)"),temp_port,14-i);
			//pScan->ShowBacnetComScanInfo(strInfo);
			Send_WhoIs_Global(-1,-1);
			Sleep(1000);
		}




		for (int j=0;j<5;j++)
		{
			int ready_to_read_count =	m_bac_scan_com_data.size();

			CString strInfo;
			strInfo.Format(_T("Scan  Bacnet mstp.Found %d BACNET device"),ready_to_read_count);
			if((int)m_bac_scan_result_data.size()>= ready_to_read_count)
				break;
			TRACE(_T("gloab scan = %d\r\n"),ready_to_read_count);
			for (int i=0;i<ready_to_read_count;i++)
			{
				int	resend_count = 0;
				do 
				{
					resend_count ++;
					if(resend_count>50)
						break;
					g_invoke_id = GetPrivateData(
						m_bac_scan_com_data.at(i).device_id,
						GETSERIALNUMBERINFO,
						0,
						0,
						sizeof(Str_Serial_info));		

					Sleep(SEND_COMMAND_DELAY_TIME);
				} while (g_invoke_id<0);
				//	Sleep(1000);
			}
		}




		int ret_scan_count =	m_bac_scan_result_data.size(); 
		if(ret_scan_count == 0 )
		{
			MessageBox(_T("No bacnet mstp device found."));
			return;
		}
		CString bac_strInfo;
		bac_strInfo.Format(_T("Scan  Bacnetip.Found %d recognizable Bacnet device"),ret_scan_count);
		//pScan->ShowBacnetComScanInfo(bac_strInfo);
		TRACE(_T("serial scan = %d\r\n"),ret_scan_count);

		CString strMainBuildName,strSubBuildingName;
		CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
		strMainBuildName= pFrame->m_strCurMainBuildingName;
		strSubBuildingName= pFrame->m_strCurSubBuldingName;
		int product_count = pFrame->m_product.size();

		for (int scan_count = 0; scan_count < ret_scan_count ; scan_count ++)
		{
			bool exist_in_db = false;
			int m_product_count = 0;
			for (int a=0;a<product_count;a++)
			{
				if(m_bac_scan_result_data.at(scan_count).serialnumber == pFrame->m_product.at(a).serial_number)
				{
					m_product_count = a;
					exist_in_db = true;
					break;
				}
			}
			CString modbusid;
			CString str_ip_address;
			CString str_n_port;
			CString str_serialid;
			CString product_class_id;
			CString str_product_name;
			CString strSql;
// 			m_pCon.CreateInstance(_T("ADODB.Connection"));
// 			m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

			modbusid.Format(_T("%d"),m_bac_scan_result_data.at(scan_count).modbus_addr);


			str_ip_address.Format(_T("%u.%u.%u.%u"),(unsigned char)m_bac_scan_result_data.at(scan_count).ipaddress[0],
													(unsigned char)m_bac_scan_result_data.at(scan_count).ipaddress[1],
													(unsigned char)m_bac_scan_result_data.at(scan_count).ipaddress[2],
													(unsigned char)m_bac_scan_result_data.at(scan_count).ipaddress[3])  ;
			str_n_port.Format(_T("%d"),m_bac_scan_result_data.at(scan_count).modbus_port);
			str_serialid.Format(_T("%u"),m_bac_scan_result_data.at(scan_count).serialnumber);
			product_class_id.Format(_T("%d"),m_bac_scan_result_data.at(scan_count).product_type);
			str_product_name = GetProductName(m_bac_scan_result_data.at(scan_count).product_type);
			str_product_name = str_product_name + _T(":") + str_serialid + _T("-") + modbusid + _T("-") + str_ip_address;

			if(exist_in_db)
			{
				if((str_ip_address.CompareNoCase(pFrame->m_product.at(m_product_count).BuildingInfo.strIp) != 0) ||
					(m_bac_scan_result_data.at(scan_count).modbus_port != pFrame->m_product.at(m_product_count).ncomport) ||
					(m_bac_scan_result_data.at(scan_count).modbus_addr != pFrame->m_product.at(m_product_count).product_id) ||
					(m_bac_scan_result_data.at(scan_count).device_id != ((int)pFrame->m_product.at(m_product_count).hardware_version )))
				{
					CString instance_value;
					instance_value.Format(_T("%u"),m_bac_scan_result_data.at(scan_count).device_id);
					try
					{
						 
						strSql.Format(_T("update ALL_NODE set Bautrate ='%s' where Serial_ID = '%s'"),str_ip_address,str_serialid);
						bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);		
						strSql.Format(_T("update ALL_NODE set Com_Port ='%s' where Serial_ID = '%s'"),str_n_port,str_serialid);
						bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);		
						strSql.Format(_T("update ALL_NODE set Product_ID ='%s' where Serial_ID = '%s'"),modbusid,str_serialid);
						bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);	
						strSql.Format(_T("update ALL_NODE set Hardware_Ver ='%s' where Serial_ID = '%s'"),instance_value,str_serialid);
						bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);	
					}
					catch(_com_error *e)
					{
						AfxMessageBox(e->ErrorMessage());
					}
					 
					 bado.CloseRecordset();
					 bado.CloseConn();

					pFrame->m_product.at(m_product_count).ncomport = m_bac_scan_result_data.at(scan_count).modbus_port;
					pFrame->m_product.at(m_product_count).BuildingInfo.strIp = str_ip_address;
					pFrame->m_product.at(m_product_count).product_id = m_bac_scan_result_data.at(scan_count).modbus_addr;
				}

			}
			else
			{

			}

		}

	}
}
