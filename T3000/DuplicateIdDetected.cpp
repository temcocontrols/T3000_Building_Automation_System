// DuplicateIdDetected.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "DuplicateIdDetected.h"
#include "afxdialogex.h"
#include "global_function.h"
#include "ping.h"
// CDuplicateIdDetected dialog
DWORD WINAPI HanlePanelNumber(LPVOID lpvoid);
DWORD WINAPI DuplicatedProcess(LPVOID lPvoid);
DWORD WINAPI CheckIPvalidThread(LPVOID lpvoid);
DWORD WINAPI  ChangeDuplicateIPthread(LPVOID lpvoid);
CString temp_controller_ip;
int controller_port = 0;
bool first_need_changed = false;
bool second_need_changed = false;
CString cs_show_info;
int auto_close_seconds = 60;
int first_propose_id = 0;
int second_propose_id = 0;
extern CString local_enthernet_ip;
vector <int> id_exsit_in_db;
CString AutoDetectIP;
IMPLEMENT_DYNAMIC(CDuplicateIdDetected, CDialogEx)


CDuplicateIdDetected::CDuplicateIdDetected(int temp,CWnd* pParent /*=NULL*/)
	: CDialogEx(CDuplicateIdDetected::IDD, pParent)
{
	duplicate_mode = temp;
}

CDuplicateIdDetected::CDuplicateIdDetected(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDuplicateIdDetected::IDD, pParent)
{

}

CDuplicateIdDetected::~CDuplicateIdDetected()
{
}

void CDuplicateIdDetected::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DUPLICATE_TITLE, m_static_title);
	DDX_Control(pDX, IDC_STATIC_NETWORK_TITLE, m_network_title);
	DDX_Control(pDX, IDC_STATIC_DUP_NETWORK_ID, m_dup_network_id);
	DDX_Control(pDX, IDC_STATIC_DUP_NETWORK_CURRENT_ID, m_dup_current_id);
	DDX_Control(pDX, IDC_STATIC_DUP_NETWORK_PROPOSED_ID, m_dup_propsed_id);
	DDX_Control(pDX, IDC_STATIC_DUP_SERIAL_NUMBER, m_dup_serial_number);
	DDX_Control(pDX, IDC_STATIC_DUP_MODEL, m_dup_model);
	DDX_Control(pDX, IDC_STATIC_DUP_LABLE, m_dup_user_test);
	DDX_Control(pDX, IDC_STATIC_DUP_IN_DATABASE, m_dup_in_database);
	DDX_Control(pDX, IDC_STATIC_DUP_IN_DATABASE_INFO, m_dup_id_from_database);
	DDX_Control(pDX, IDC_STATIC_DUP_CURRENT_DEVICE_ID1, m_current_id_1);
	DDX_Control(pDX, IDC_STATIC_DUP_CURRENT_DEVICE_ID2, m_current_id_2);
	DDX_Control(pDX, IDC_EDIT_DEVICE_1, m_edit_propsed_id_1);
	DDX_Control(pDX, IDC_EDIT_DEVICE_2, m_edit_propsed_id_2);
	DDX_Control(pDX, IDC_STATIC_DUP_DEVICE_SERIAL_1, m_static_serial_1);
	DDX_Control(pDX, IDC_STATIC_DUP_DEVICE_SERIAL_2, m_static_serial_2);
	DDX_Control(pDX, IDC_STATIC_DUP_DEVICE_MODEL_1, m_device_model_1);
	DDX_Control(pDX, IDC_STATIC_DUP_DEVICE_MODEL_2, m_device_model_2);
	DDX_Control(pDX, IDC_STATIC_DUP_DEVICE_USERNAME_1, m_user_text_1);
	DDX_Control(pDX, IDC_STATIC_DUP_DEVICE_USERNAME_2, m_user_text_2);
	DDX_Control(pDX, IDC_STATIC_DUP_DEVICE_YES_NO_1, m_device_yes_no_1);
	DDX_Control(pDX, IDC_STATIC_DUP_DEVICE_YES_NO_2, m_device_yes_no_2);
	DDX_Control(pDX, IDC_STATIC_DUP_DEVICE_DATABASE_ID_1, m_id_from_db_1);
	DDX_Control(pDX, IDC_STATIC_DUP_DEVICE_DATABASE_ID_2, m_id_from_db_2);
	DDX_Control(pDX, IDC_STATIC_DUP_DEVICE_1, m_device1_title);
	DDX_Control(pDX, IDC_STATIC_DUP_DEVICE_2, m_device2_title);
	DDX_Control(pDX, IDC_STATIC_DUPLICATE_INFO, m_duplicate_ret_info);
}


BEGIN_MESSAGE_MAP(CDuplicateIdDetected, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DUPLICATE_DONE, &CDuplicateIdDetected::OnBnClickedButtonDuplicateDone)
	ON_BN_CLICKED(IDC_BUTTON_DUPLICATE_CANCEL, &CDuplicateIdDetected::OnBnClickedButtonDuplicateCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDuplicateIdDetected message handlers
HANDLE DuplicatedThread = NULL;
HANDLE PanelNumberThread = NULL;
HANDLE hCheckIPHandle = NULL;
HANDLE hChangeDuplicateIP = NULL;

BOOL CDuplicateIdDetected::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	first_need_changed = false;
	second_need_changed = false;
	auto_close_seconds = 60;
	cs_show_info.Empty();
	
	Initial_static();
	SetTimer(1,500,NULL);
	SetTimer(2,1000,NULL);
	GetDlgItem(IDC_EDIT_DEVICE_2)->SetFocus();

    if (duplicate_mode == CONFILIC_IPADDRESS)
    {
        if(hCheckIPHandle == NULL)
            hCheckIPHandle = CreateThread(NULL, NULL, CheckIPvalidThread, this, NULL, NULL);
    }

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
DWORD WINAPI CheckIPvalidThread(LPVOID lpvoid)
{
    CDuplicateIdDetected *mparent = (CDuplicateIdDetected *)lpvoid;
    Sleep(200);
    CString cs_confict_ip;
    cs_confict_ip = device_id_data_1.ip_address;
    CStringArray temp_ip_array;
    CString temp_last_ip;
    if (cs_confict_ip.IsEmpty())
    {
        goto end_get_validipthread;
    }



    SplitCStringA(temp_ip_array, cs_confict_ip, _T("."));
    if (temp_ip_array.GetSize() != 4)
    {
        goto end_get_validipthread;
    }

    int local_ip = 0;
    temp_last_ip = temp_ip_array.GetAt(3);
    local_ip = _wtoi(temp_last_ip);
    for (int  i = 50; i < 254; i++)
    {
        if (i == local_ip)
            continue;

        CString strIP;
        strIP.Format(_T("%s.%s.%s.%u"), temp_ip_array.GetAt(0), temp_ip_array.GetAt(1), temp_ip_array.GetAt(2), (unsigned char)i);
        CPing p1;
        CPingReply pr1;
        if (p1.Ping1((LPCTSTR)strIP, pr1))
        {
            //如果ping 的通就说明要改的IP地址是存在的，有人在用的;
            continue;
        }
        else
        {
            AutoDetectIP = strIP;
            mparent->SetTimer(3, 500, NULL);
            
            break;
        }


    }

end_get_validipthread:
    hCheckIPHandle = NULL;
    return true;
}
DWORD WINAPI ChangeDuplicateIPthread(LPVOID lpvoid)
{
    CDuplicateIdDetected *mparent = (CDuplicateIdDetected *)lpvoid;

    if (mparent->ip1_need_change)
        mparent->ChangeNetDeviceIP(mparent->original_ip1, mparent->cschaned_ip1, device_id_data_1.nSerial);

    if (mparent->ip2_need_change)
        mparent->ChangeNetDeviceIP(mparent->original_ip2, mparent->cschaned_ip2, device_id_data_2.nSerial);

    hChangeDuplicateIP = NULL;
    ::PostMessage(mparent->m_hWnd, WM_CLOSE, NULL ,NULL);

    return true;
}


DWORD WINAPI HanlePanelNumber(LPVOID lpvoid)
{
	CDuplicateIdDetected *mparent = (CDuplicateIdDetected *)lpvoid;

	if(first_need_changed)
	{
		 bool bret=Open_Socket2(device_id_data_1.ip_address,device_id_data_1.nport);
		 SetCommunicationType(1);
		 if(bret)
		 {
			 //first_propose_id
			 int write_ret = 0 ;
			write_ret =  write_one(255,36,first_propose_id);
			if(write_ret <0)
			{
				cs_show_info.Format(_T("try to change the first panel id failed..."));
				Sleep(1000);
				goto ret_label_panel_ret;
			}
		 }
	}

	if(second_need_changed)
	{
		bool bret=Open_Socket2(device_id_data_2.ip_address,device_id_data_2.nport);
		SetCommunicationType(1);
		if(bret)
		{
			//first_propose_id
			int write_ret = 0 ;
			write_ret =  write_one(255,36,second_propose_id);
			if(write_ret <0)
			{
				cs_show_info.Format(_T("try to change %s panel id failed..."),device_id_data_2.ip_address);
				Sleep(1000);
				goto ret_label_panel_ret;
			}
		}
	}
	cs_show_info.Format(_T("Operation Done!"));
	Sleep(2000);
	PostMessage(mparent->m_hWnd,WM_CLOSE,NULL,NULL);
	PanelNumberThread = NULL;
	return 1;

ret_label_panel_ret:
	mparent->GetDlgItem(IDC_BUTTON_DUPLICATE_DONE)->EnableWindow(TRUE);
	mparent->GetDlgItem(IDC_BUTTON_DUPLICATE_CANCEL)->EnableWindow(TRUE);

	PanelNumberThread = NULL;
	return TRUE;
}

DWORD WINAPI DuplicatedProcess(LPVOID lPvoid)
{
	CDuplicateIdDetected * mparent = (CDuplicateIdDetected *)lPvoid;
	
	 bool b=Open_Socket2(temp_controller_ip,controller_port);
	 SetCommunicationType(1);
	 Sleep(2000);
	 if(b)
	 {
		 cs_show_info.Format(_T("Ready to changing the ID . Please wait."));
		 unsigned short temp_buffer[10];
		
		 if(first_need_changed)
		 {

			 temp_buffer[0] = 0;
			 temp_buffer[1] =  (unsigned short)(device_id_data_1.nSerial & 0x000000ff);
			 temp_buffer[2] =  (unsigned short)((device_id_data_1.nSerial & 0x0000ff00)>>8);
			 temp_buffer[3] =   (unsigned short)((device_id_data_1.nSerial & 0x00ff0000)>>16);
			 temp_buffer[4] =  (unsigned short)((device_id_data_1.nSerial & 0xff000000)>>24);
			 temp_buffer[5] =  device_id_data_1.modbusID;
			 temp_buffer[6] = first_propose_id;
			 int ret=Write_Multi_org_short(255,(unsigned short *)temp_buffer,CONTROLLER_DUPLICATE_ID,7,10);
			 if(ret < 0)
			 {
				cs_show_info.Format(_T("try to change the first sub id failed..."));
				Sleep(1000);
				goto ret_label_write_ret;
			 }

		 }

		 if(second_need_changed)
		 {
			 temp_buffer[0] = 0;
			 temp_buffer[1] =  (unsigned short)(device_id_data_2.nSerial & 0x000000ff);
			 temp_buffer[2] =  (unsigned short)((device_id_data_2.nSerial & 0x0000ff00)>>8);
			 temp_buffer[3] =   (unsigned short)((device_id_data_2.nSerial & 0x00ff0000)>>16);
			 temp_buffer[4] =  (unsigned short)((device_id_data_2.nSerial & 0xff000000)>>24);
			 temp_buffer[5] =  device_id_data_2.modbusID;
			 temp_buffer[6] = second_propose_id;
			 int ret=Write_Multi_org_short(255,(unsigned short *)temp_buffer,CONTROLLER_DUPLICATE_ID,7,10);
			 if(ret < 0)
			 {
				cs_show_info.Format(_T("try to change the second sub id failed..."));
				Sleep(1000);
				goto ret_label_write_ret;
			 }

		 }



	 }
	 else
	 {
		 cs_show_info.Format(_T("Connect to panel failed ,please try again."));
	 }

	 int ret_ret = 0;
	 int recheck_time = 0;
	 do 
	 {
		ret_ret  = read_one(255,CONTROLLER_DUPLICATE_ID);
		if(ret_ret == 3)
		{
			cs_show_info.Format(_T("try to change the sub id failed..."));
			break;
		}
		cs_show_info.Format(_T("Waiting... (%d)"),5 - recheck_time);
		recheck_time ++ ;
		Sleep(1000);
	 } while ((ret_ret != 2) && (recheck_time < 5));
	 
	 if(ret_ret != 2)
	 {
		 cs_show_info.Format(_T("Operation failed..."));
	 }
	 else
	 {
		  cs_show_info.Format(_T("Operation Done!"));
		  Sleep(2000);
		  PostMessage(mparent->m_hWnd,WM_CLOSE,NULL,NULL);
		  DuplicatedThread = NULL;
		  return 1;
	 }

ret_label_write_ret:
//	mparent->GetDlgItem(IDC_BUTTON_DUPLICATE_DONE)->EnableWindow(TRUE);
//	mparent->GetDlgItem(IDC_BUTTON_DUPLICATE_CANCEL)->EnableWindow(TRUE);

	
	DuplicatedThread = NULL;
	return 1;
}

BOOL CDuplicateIdDetected::PreTranslateMessage(MSG* pMsg)
{
	

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDuplicateIdDetected::Initial_static()
{
	id_exsit_in_db.clear();
	if(duplicate_mode == CONFILIC_SUBID)
		m_static_title.SetWindowTextW(_T("Each device on the RS485 network needs a unique network ID or communications will fail.\r\nYou can change the ID using a plug and play feature built into the device.\r\nWould you like to change the address to the new ID?"));
	else if(duplicate_mode == CONFILIC_PANEL_ID)
		m_static_title.SetWindowTextW(_T("Each device on the same network needs a unique panel number or communications will fail.\r\nWould you like to change the panel number to the new value?"));
    else if(duplicate_mode == CONFILIC_IPADDRESS)
        m_static_title.SetWindowTextW(_T("Each device on the ethernet network needs a unique IP address or communications will fail.\r\nWould you like to change the IP address to a new IP?"));
	m_static_title.textColor(RGB(0,0,0));
	//m_static.bkColor(RGB(0,255,255));
	m_static_title.setFont(28,16,NULL,_T("Arial"));

	temp_controller_ip = device_id_data_1.ip_address;
	controller_port = device_id_data_1.nport;
	//device_id_data_1
	CString temp_subnet;
    if (duplicate_mode == CONFILIC_SUBID)
        temp_subnet.Format(_T("IP:%s  Panel %d:RS485 'MAIN' subnet"), temp_controller_ip, device_id_data_1.panal_number);
    else if (duplicate_mode == CONFILIC_PANEL_ID)
        temp_subnet.Format(_T("%s - %s panel number conflict"), device_id_data_1.ip_address, device_id_data_2.ip_address);
    else if(duplicate_mode == CONFILIC_IPADDRESS)
        temp_subnet.Format(_T("%s - %s IP address conflict"), device_id_data_1.ip_address, device_id_data_1.ip_address);

	CString temp_serial_1;CString temp_serial_2;
	CString temp_current_id_1;CString temp_current_id_2;
	CString temp_user_text_1;CString temp_user_text_2;
	CString temp_product_id_1;CString temp_product_id_2;

	temp_serial_1.Format(_T("%u"),device_id_data_1.nSerial);
	temp_serial_2.Format(_T("%u"),device_id_data_2.nSerial);
	if(duplicate_mode == CONFILIC_SUBID)
	{
		temp_current_id_1.Format(_T("%u"),device_id_data_1.modbusID);
		temp_current_id_2.Format(_T("%u"),device_id_data_2.modbusID);
	}
	else if(duplicate_mode == CONFILIC_PANEL_ID)
	{
		temp_current_id_1.Format(_T("%u"),device_id_data_1.panal_number);
		temp_current_id_2.Format(_T("%u"),device_id_data_2.panal_number);
	}
    else if (duplicate_mode == CONFILIC_IPADDRESS)
    {
        temp_current_id_1.Format(_T("%s"), device_id_data_1.ip_address);
        temp_current_id_2.Format(_T("%s"), device_id_data_2.ip_address);
    }


    temp_user_text_1 = _T("Unknown");//device_id_data_1.show_label_name;
	temp_user_text_2 = _T("Unknown");//device_id_data_2.show_label_name;

	temp_product_id_1 = _T(" ");//GetProductName(device_id_data_1.product_id);
	temp_product_id_2 = _T(" ");//GetProductName(device_id_data_2.product_id);

	m_network_title.SetWindowTextW(temp_subnet);
	m_network_title.textColor(RGB(0,0,0));
	//m_static.bkColor(RGB(0,255,255));
	m_network_title.setFont(24,16,NULL,_T("Arial"));


	
	m_duplicate_ret_info.textColor(RGB(0,0,255));
	//m_duplicate_ret_info.bkColor(RGB(0,255,255));
	m_duplicate_ret_info.setFont(24,16,NULL,_T("Arial"));


	m_dup_network_id.textColor(RGB(0,0,0));
	//m_static.bkColor(RGB(0,255,255));
	m_dup_network_id.setFont(24,16,NULL,_T("Arial"));

	if(duplicate_mode == CONFILIC_SUBID)
	{
		m_dup_current_id.SetWindowTextW(_T("Current ID :"));
		m_dup_propsed_id.SetWindowTextW(_T("Proposed ID :"));
	}
	else if(duplicate_mode == CONFILIC_PANEL_ID)
	{
		m_dup_current_id.SetWindowTextW(_T("Current Panel:"));
		m_dup_propsed_id.SetWindowTextW(_T("Proposed Panel:"));
	}
    else if (duplicate_mode == CONFILIC_IPADDRESS)
    {
        m_dup_current_id.SetWindowTextW(_T("Current IP:"));
        m_dup_propsed_id.SetWindowTextW(_T("Proposed IP:"));
    }

	m_dup_current_id.textColor(RGB(0,0,0));
	m_dup_current_id.setFont(24,16,NULL,_T("Arial"));

	m_dup_propsed_id.textColor(RGB(0,0,0));
	m_dup_propsed_id.setFont(24,16,NULL,_T("Arial"));


	m_dup_serial_number.textColor(RGB(0,0,0));
	m_dup_serial_number.setFont(24,16,NULL,_T("Arial"));

	m_dup_model.textColor(RGB(0,0,0));
	m_dup_model.setFont(24,16,NULL,_T("Arial"));

	m_dup_user_test.textColor(RGB(0,0,0));
	//m_dup_user_test.bkColor(RGB(0,255,255));
	m_dup_user_test.setFont(24,16,NULL,_T("Arial"));

	m_dup_in_database.textColor(RGB(0,0,0));
	//m_dup_in_database.bkColor(RGB(0,255,255));
	m_dup_in_database.setFont(24,16,NULL,_T("Arial"));

	m_dup_id_from_database.textColor(RGB(0,0,0));
	//m_dup_id_from_database.bkColor(RGB(0,255,255));
	m_dup_id_from_database.setFont(24,16,NULL,_T("Arial"));


	m_device1_title.textColor(RGB(0,0,0));
	//m_device1_title.bkColor(RGB(0,255,255));
	m_device1_title.setFont(24,16,NULL,_T("Arial"));

	m_device2_title.textColor(RGB(0,0,0));
	//m_device2_title.bkColor(RGB(0,255,255));
	m_device2_title.setFont(24,16,NULL,_T("Arial"));


	m_current_id_1.SetWindowTextW(temp_current_id_1);
	m_current_id_1.textColor(RGB(0,0,0));
	//m_current_id_1.bkColor(RGB(0,255,255));
	m_current_id_1.setFont(24,16,NULL,_T("Arial"));

	m_current_id_2.SetWindowTextW(temp_current_id_2);
	m_current_id_2.textColor(RGB(0,0,0));
	//m_current_id_2.bkColor(RGB(0,255,255));
	m_current_id_2.setFont(24,16,NULL,_T("Arial"));

	m_edit_propsed_id_1.SetWindowTextW(temp_current_id_1);
	m_edit_propsed_id_1.textColor(RGB(0,0,0));
	m_edit_propsed_id_1.bkColor(RGB(255,255,255));
	m_edit_propsed_id_1.setFont(24,16,NULL,_T("Arial"));

	m_edit_propsed_id_2.textColor(RGB(0,0,0));
	m_edit_propsed_id_2.bkColor(RGB(255,255,255));
	m_edit_propsed_id_2.setFont(24,16,NULL,_T("Arial"));

	m_static_serial_1.SetWindowTextW(temp_serial_1);
	m_static_serial_1.textColor(RGB(0,0,0));
	//m_static_serial_1.bkColor(RGB(0,255,255));
	m_static_serial_1.setFont(24,16,NULL,_T("Arial"));

	m_static_serial_2.SetWindowTextW(temp_serial_2);
	m_static_serial_2.textColor(RGB(0,0,0));
	//m_static_serial_2.bkColor(RGB(0,255,255));
	m_static_serial_2.setFont(24,16,NULL,_T("Arial"));
	
	m_device_model_1.SetWindowTextW(temp_product_id_1);
	m_device_model_1.textColor(RGB(0,0,0));
	//m_device_model_1.bkColor(RGB(0,255,255));
	m_device_model_1.setFont(24,16,NULL,_T("Arial"));

	m_device_model_2.SetWindowTextW(temp_product_id_2);
	m_device_model_2.textColor(RGB(0,0,0));
	//m_device_model_2.bkColor(RGB(0,255,255));
	m_device_model_2.setFont(24,16,NULL,_T("Arial"));

	m_user_text_1.SetWindowTextW(temp_user_text_1);
	m_user_text_1.textColor(RGB(0,0,0));
	//m_user_text_1.bkColor(RGB(0,255,255));
	m_user_text_1.setFont(24,16,NULL,_T("Arial"));

	m_user_text_2.SetWindowTextW(temp_user_text_2);
	m_user_text_2.textColor(RGB(0,0,0));
	//m_user_text_2.bkColor(RGB(0,255,255));
	m_user_text_2.setFont(24,16,NULL,_T("Arial"));

	m_device_yes_no_1.textColor(RGB(0,0,0));
	//m_device_yes_no_1.bkColor(RGB(0,255,255));
	m_device_yes_no_1.setFont(24,16,NULL,_T("Arial"));

	m_device_yes_no_2.textColor(RGB(0,0,0));
	//m_device_yes_no_2.bkColor(RGB(0,255,255));
	m_device_yes_no_2.setFont(24,16,NULL,_T("Arial"));

	m_id_from_db_1.textColor(RGB(0,0,0));
	//m_id_from_db_1.bkColor(RGB(0,255,255));
	m_id_from_db_1.setFont(24,16,NULL,_T("Arial"));

	m_id_from_db_2.textColor(RGB(0,0,0));
	//m_id_from_db_2.bkColor(RGB(0,255,255));
	m_id_from_db_2.setFont(24,16,NULL,_T("Arial"));


    if ((duplicate_mode == CONFILIC_SUBID) || (duplicate_mode == CONFILIC_PANEL_ID))
    {
        if ((device_id_data_1.nSerial != 0) && (device_id_data_2.nSerial != 0))
        {
            CString strsql;
            m_SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

            strsql.Format(_T("select * from ALL_NODE where Serial_ID = '%s'"), temp_serial_1);
            m_q = m_SqliteDBBuilding.execQuery((UTF8MBSTR)strsql);
            m_table = m_SqliteDBBuilding.getTable((UTF8MBSTR)strsql);

            int temp_count = m_table.numRows();
            CString temp_pid;
            _variant_t temp_variant;
            if (temp_count > 0)
            {
                while (!m_q.eof())//次Com port 下面的所有 设备;
                {
                    temp_pid = m_q.getValuebyName(L"Product_ID");
                    m_device_yes_no_1.SetWindowTextW(_T("Yes"));
                    m_id_from_db_1.SetWindowTextW(temp_pid);
                    break;
                }
            }
            else
            {
                m_device_yes_no_1.SetWindowTextW(_T("No"));
                m_id_from_db_1.SetWindowTextW(_T("-"));
            }


            strsql.Format(_T("select * from ALL_NODE where Serial_ID = '%s'"), temp_serial_2);
            m_q = m_SqliteDBBuilding.execQuery((UTF8MBSTR)strsql);
            m_table = m_SqliteDBBuilding.getTable((UTF8MBSTR)strsql);
            temp_count = m_table.numRows();

            if (temp_count > 0)
            {
                while (!m_q.eof())//此Com port 下面的所有 设备;
                {

                    temp_pid = m_q.getValuebyName(L"Product_ID");
                    m_device_yes_no_2.SetWindowTextW(_T("Yes"));
                    m_id_from_db_2.SetWindowTextW(temp_pid);

                    break;
                }
            }
            else
            {
                m_device_yes_no_2.SetWindowTextW(_T("No"));
                m_id_from_db_2.SetWindowTextW(_T("-"));
            }


            CString temp_parent_serial;
            temp_parent_serial.Format(_T("%u"), device_id_data_1.parent_serial_number);

            strsql.Format(_T("select * from ALL_NODE where Parent_SerialNum = '%s' order by Product_ID ASC"), temp_parent_serial);
            m_q = m_SqliteDBBuilding.execQuery((UTF8MBSTR)strsql);
            m_table = m_SqliteDBBuilding.getTable((UTF8MBSTR)strsql);

            temp_count = m_table.numRows();

            if (temp_count > 0)
            {
                while (!m_q.eof())//次Com port 下面的所有 设备;
                {

                    temp_pid = m_q.getValuebyName(L"Product_ID");
                    int temp_db_id = _wtoi(temp_pid);
                    id_exsit_in_db.push_back(temp_db_id);
                    m_q.nextRow();
                }
            }

            m_SqliteDBBuilding.closedb();
        }

        int db_id_aviliable = 0;
        for (int i = 10;i < 250;i++)
        {
            int find_id = false;
            for (int j = 0;j < id_exsit_in_db.size();j++)
            {
                if (i == id_exsit_in_db.at(j))
                {
                    find_id = true;
                    break;
                }
            }

            if (find_id == false)
            {
                db_id_aviliable = i;
                break;
            }
        }

        CString temp_10;

        temp_10.Format(_T("%u"), db_id_aviliable);
        m_edit_propsed_id_2.SetWindowTextW(temp_10);
        m_edit_propsed_id_2.textColor(RGB(255, 0, 0));
    }
    else if (duplicate_mode == CONFILIC_IPADDRESS)
    {
        m_edit_propsed_id_1.SetWindowTextW(temp_current_id_1);
        m_edit_propsed_id_2.SetWindowTextW(temp_current_id_2);

        temp_product_id_1 = GetProductName(device_id_data_1.product_id);
        temp_product_id_2 = GetProductName(device_id_data_2.product_id);

        m_device_model_1.SetWindowTextW(temp_product_id_1);
        m_device_model_2.SetWindowTextW(temp_product_id_2);
    }
}


void CDuplicateIdDetected::OnBnClickedButtonDuplicateDone()
{
	
	KillTimer(2);
    if ((duplicate_mode == CONFILIC_SUBID) || (duplicate_mode == CONFILIC_PANEL_ID))
    {
        GetDlgItem(IDC_BUTTON_DUPLICATE_DONE)->EnableWindow(FALSE);
        GetDlgItem(IDC_BUTTON_DUPLICATE_CANCEL)->EnableWindow(FALSE);
        CString original_id;
        CString TEMP_ID_1;CString TEMP_ID_2;
        GetDlgItem(IDC_STATIC_DUP_CURRENT_DEVICE_ID1)->GetWindowTextW(original_id);
        GetDlgItem(IDC_EDIT_DEVICE_1)->GetWindowTextW(TEMP_ID_1);
        GetDlgItem(IDC_EDIT_DEVICE_2)->GetWindowTextW(TEMP_ID_2);

        original_id.Trim();
        TEMP_ID_1.Trim();
        TEMP_ID_2.Trim();

        first_propose_id = _wtoi(TEMP_ID_1);
        second_propose_id = _wtoi(TEMP_ID_2);
        if (original_id.CompareNoCase(TEMP_ID_1) != 0)
        {
            first_need_changed = true;
            if (duplicate_mode == 1)
            {
                vector <int>::iterator itter;
                for (itter = exsit_panel_number.begin();itter != exsit_panel_number.end();++itter)
                {
                    if (*itter == first_propose_id)
                    {
                        MessageBox(_T("Panel number already exsit in the network,please change a new one."));
                        GetDlgItem(IDC_BUTTON_DUPLICATE_DONE)->EnableWindow(TRUE);
                        GetDlgItem(IDC_BUTTON_DUPLICATE_CANCEL)->EnableWindow(TRUE);
                        return;
                    }
                }
            }
        }

        if (original_id.CompareNoCase(TEMP_ID_2) != 0)
        {
            second_need_changed = true;
            if (duplicate_mode == 1)
            {
                vector <int>::iterator itter;
                for (itter = exsit_panel_number.begin();itter != exsit_panel_number.end();++itter)
                {
                    if (*itter == second_propose_id)
                    {
                        MessageBox(_T("Panel number already exsit in the network,please change a new one."));
                        GetDlgItem(IDC_BUTTON_DUPLICATE_DONE)->EnableWindow(TRUE);
                        GetDlgItem(IDC_BUTTON_DUPLICATE_CANCEL)->EnableWindow(TRUE);
                        return;
                    }
                }
            }
        }
    }
    else if (duplicate_mode == CONFILIC_IPADDRESS)
    {
        GetDlgItem(IDC_STATIC_DUP_CURRENT_DEVICE_ID1)->GetWindowTextW(original_ip1);
        GetDlgItem(IDC_STATIC_DUP_CURRENT_DEVICE_ID2)->GetWindowTextW(original_ip2);
        GetDlgItem(IDC_EDIT_DEVICE_1)->GetWindowTextW(cschaned_ip1);
        GetDlgItem(IDC_EDIT_DEVICE_2)->GetWindowTextW(cschaned_ip2);

        if ((original_ip1.IsEmpty()) || cschaned_ip1.IsEmpty())
            ip1_need_change = false;
        else if(original_ip1.CompareNoCase(cschaned_ip1) == 0)
            ip1_need_change = false;
        else
            ip1_need_change = true;

        if ((original_ip2.IsEmpty()) || cschaned_ip2.IsEmpty())
            ip2_need_change = false;
        else if (original_ip2.CompareNoCase(cschaned_ip2) == 0)
            ip2_need_change = false;
        else
            ip2_need_change = true;
    }
	

    if (duplicate_mode == CONFILIC_SUBID)
    {
        if (DuplicatedThread == NULL)
            DuplicatedThread = CreateThread(NULL, NULL, DuplicatedProcess, this, NULL, NULL);
    }
    else if (duplicate_mode == CONFILIC_PANEL_ID)
    {
        if (PanelNumberThread == NULL)
            PanelNumberThread = CreateThread(NULL, NULL, HanlePanelNumber, this, NULL, NULL);
    }
    else if (duplicate_mode == CONFILIC_IPADDRESS)
    {
        if (hChangeDuplicateIP == NULL)
            hChangeDuplicateIP = CreateThread(NULL, NULL, ChangeDuplicateIPthread, this, NULL, NULL);
    }

}


void CDuplicateIdDetected::OnBnClickedButtonDuplicateCancel()
{
	
	KillTimer(2);
	edit_confilct_mode = false;
	PostMessage(WM_CLOSE,NULL,NULL);
}


void CDuplicateIdDetected::OnTimer(UINT_PTR nIDEvent)
{
	 
	switch(nIDEvent)
	{
	case 1:
		m_duplicate_ret_info.SetWindowTextW(cs_show_info);
		break;
	case 2:
		{
			if(auto_close_seconds == 0)
			{
				KillTimer(2);
				PostMessage(WM_CLOSE,NULL,NULL);
			}
			else
			{
				auto_close_seconds --;
				cs_show_info.Format(_T("Close after (%d)"),auto_close_seconds);
			}
			
		}
		break;
    case 3:
    {
        KillTimer(3);
       m_edit_propsed_id_2.SetWindowTextW(AutoDetectIP);
    }
	default:
		break;
	}
	
	CDialogEx::OnTimer(nIDEvent);
}


BOOL CDuplicateIdDetected::ChangeNetDeviceIP(CString soldIP, CString newstrIP, unsigned int nSerialNum)
{
    BOOL ret = FALSE;

    CString strlog;
    int nRet = 0;
    short nmsgType = UPD_BROADCAST_QRY_MSG;
    const DWORD END_FLAG = 0x00000000;
    TIMEVAL time;
    time.tv_sec = 3;
    time.tv_usec = 1000;
    fd_set fdSocket;
    BYTE buffer[512] = { 0 };
    BYTE pSendBuf[1024];
    ZeroMemory(pSendBuf, 255);
    pSendBuf[0] = 0x66;
    memcpy(pSendBuf + 1, (BYTE*)&END_FLAG, 4);
    //int nSendLen = 17;
    int nSendLen = 21;
    int time_out = 0;
    USES_CONVERSION;
    CString stroldipaddress, strnewipadress, strlocalipaddress, strnewsubnet, strnewgateway;
    stroldipaddress = soldIP;
    strnewipadress = newstrIP;


    SOCKET sListen = NULL;

    for (int i = 0;i < g_Vector_Subnet.size();i++)
    {
        if (CheckTheSameSubnet(g_Vector_Subnet.at(i).StrIP, strnewipadress))
        {
            strnewgateway = g_Vector_Subnet.at(i).StrGetway;
            strnewsubnet = g_Vector_Subnet.at(i).StrMask;
            break;
        }
    }

    //GetNewIP(strnewipadress,allsubnets[i].StrIP);
    if (strnewipadress.Find(_T("0.0.0")) != -1)//对0.0.0.0的过滤掉
    {
        return FALSE;
    }


#pragma region new_socket
    SOCKET h_scan_Broad = NULL;
    SOCKADDR_IN h_scan_bcast;
    SOCKADDR_IN h_scan_siBind;
    h_scan_Broad = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    BOOL bBroadcast = TRUE;
    ::setsockopt(h_scan_Broad, SOL_SOCKET, SO_BROADCAST, (char*)&bBroadcast, sizeof(BOOL));
    int iMode = 1;
    ioctlsocket(h_scan_Broad, FIONBIO, (u_long FAR*) &iMode);

    BOOL bDontLinger = FALSE;
    setsockopt(h_scan_Broad, SOL_SOCKET, SO_DONTLINGER, (const char*)&bDontLinger, sizeof(BOOL));


    //SOCKADDR_IN bcast;
    h_scan_bcast.sin_family = AF_INET;
    //bcast.sin_addr.s_addr=nBroadCastIP;
    h_scan_bcast.sin_addr.s_addr = INADDR_BROADCAST;
    h_scan_bcast.sin_port = htons(UDP_BROADCAST_PORT);


    h_scan_siBind.sin_family = AF_INET;
    h_scan_siBind.sin_addr.s_addr = INADDR_ANY;



#pragma endregion new_socket
    char local_network_ip[255];
    local_enthernet_ip = device_id_data_1.NetCard_Address;//  M_.NetworkCard_Address;
    WideCharToMultiByte(CP_ACP, 0, local_enthernet_ip.GetBuffer(), -1, local_network_ip, 255, NULL, NULL);
    h_siBind.sin_family = AF_INET;
    h_siBind.sin_addr.s_addr = inet_addr(local_network_ip);
    if (-1 == bind(h_scan_Broad, (SOCKADDR*)&h_siBind, sizeof(h_siBind)))//把网卡地址强行绑定到Socket  
    {
        //MessageBox(_T("Network Initial Fail"));
        ret = FALSE;
    }



    LPCSTR szIP = W2A(stroldipaddress);
    DWORD dwIP = inet_addr(szIP);
    IN_ADDR ia;
    ia.S_un.S_addr = dwIP;
    //////////////////Old IP////////////////////////////////////
    pSendBuf[1] = ia.S_un.S_un_b.s_b1;
    pSendBuf[2] = ia.S_un.S_un_b.s_b2;
    pSendBuf[3] = ia.S_un.S_un_b.s_b3;
    pSendBuf[4] = ia.S_un.S_un_b.s_b4;
    ///////////////////New IP///////////////////////////////////////////
    szIP = W2A(strnewipadress);
    dwIP = inet_addr(szIP);
    ia.S_un.S_addr = dwIP;
    ///////////////////////////////////////////////////////////
    pSendBuf[5] = ia.S_un.S_un_b.s_b1;
    pSendBuf[6] = ia.S_un.S_un_b.s_b2;
    pSendBuf[7] = ia.S_un.S_un_b.s_b3;
    pSendBuf[8] = ia.S_un.S_un_b.s_b4;
    ////////////////////////////////////////////////////////////////////
    szIP = W2A(strnewsubnet);
    dwIP = inet_addr(szIP);
    ia.S_un.S_addr = dwIP;
    pSendBuf[9] = ia.S_un.S_un_b.s_b1;
    pSendBuf[10] = ia.S_un.S_un_b.s_b2;
    pSendBuf[11] = ia.S_un.S_un_b.s_b3;
    pSendBuf[12] = ia.S_un.S_un_b.s_b4;
    ////////////////////////////////////////////////////////////////////
    szIP = W2A(strnewgateway);
    dwIP = inet_addr(szIP);
    ia.S_un.S_addr = dwIP;
    pSendBuf[13] = ia.S_un.S_un_b.s_b1;
    pSendBuf[14] = ia.S_un.S_un_b.s_b2;
    pSendBuf[15] = ia.S_un.S_un_b.s_b3;
    pSendBuf[16] = ia.S_un.S_un_b.s_b4;

    memcpy(&pSendBuf[17], &nSerialNum, 4);

    FD_ZERO(&fdSocket);
    FD_SET(h_scan_Broad, &fdSocket);
    // 		fd_set fdSocket;
    // 		FD_ZERO(&fdSocket);	
    // 		FD_SET(h_scan_Broad, &fdSocket);



    nRet = ::sendto(h_scan_Broad, (char*)pSendBuf, nSendLen, 0, (sockaddr*)&h_bcast, sizeof(h_bcast));
    if (nRet == SOCKET_ERROR)
    {
        int  nError = WSAGetLastError();
        ret = FALSE;
        goto END_CHANGEIP_SCAN;

    }
    int nLen = sizeof(h_siBind);
    //while(pScanner->IsComScanRunning())
    //	Sleep(3000);
    fd_set fdRead = fdSocket;
    int nSelRet = ::select(0, &fdRead, NULL, NULL, &time);
    if (nSelRet == SOCKET_ERROR)
    {
        int nError = WSAGetLastError();
        ret = FALSE;
        goto END_CHANGEIP_SCAN;

    }

    if (nSelRet > 0)
    {
        ZeroMemory(buffer, 512);
        //Sleep(3000);
        do
        {

            int nRet = ::recvfrom(h_scan_Broad, (char*)buffer, 512, 0, (sockaddr*)&h_siBind, &nLen);
            //			int nRet = ::recvfrom(hBroad,(char*)&buffer[0], nsize, 0, (sockaddr*)&addrRemote, &nLen);
            BYTE szIPAddr[4] = { 0 };
            if (nRet > 0)
            {
                FD_ZERO(&fdSocket);
                if (buffer[0] == 0x67)//收到正确的回复了
                {

                    //SaveNewIPAddress(strnewipadress, stroldipaddress);
                    ret = TRUE;
                    MessageBox(_T("Successfull"));
                    CString strSql;
                    CppSQLite3DB SqliteDBBuilding;
                    CppSQLite3Table table;
                    CppSQLite3Query q;
                    SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

                    CString temp_serial_cs;
                    temp_serial_cs.Format(_T("%u"), nSerialNum);
                    strSql.Format(_T("select * from ALL_NODE where Serial_ID = '%s' "), temp_serial_cs);
                    //m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
                    q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
                    while (!q.eof())
                    {
                        strSql.Format(_T("update ALL_NODE set Bautrate='%s' where Serial_ID= '%s'"), strnewipadress, temp_serial_cs);
                        SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
                        q.nextRow();
                    }

                    //CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
                    //::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING, 0, 0);

                    break;
                }

                //SHOW_TX_RX_TS

                FD_ZERO(&fdSocket);
                FD_SET(h_scan_Broad, &fdSocket);
                nLen = sizeof(h_scan_siBind);
                fdRead = fdSocket;
                nSelRet = ::select(0, &fdRead, NULL, NULL, &time);

            }

        } while (nSelRet > 0);

    }
    else
    {
        MessageBox(_T("Fail!"));
        ret = FALSE;
    }
END_CHANGEIP_SCAN:
    closesocket(h_scan_Broad);
    h_scan_Broad = NULL;

    return ret;
}



