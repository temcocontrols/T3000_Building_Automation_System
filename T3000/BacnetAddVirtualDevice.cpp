// BacnetAddVirtualDevice.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetAddVirtualDevice.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "global_function.h"

// CBacnetAddVirtualDevice dialog

IMPLEMENT_DYNAMIC(CBacnetAddVirtualDevice, CDialogEx)

CBacnetAddVirtualDevice::CBacnetAddVirtualDevice(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetAddVirtualDevice::IDD, pParent)
{

}

CBacnetAddVirtualDevice::~CBacnetAddVirtualDevice()
{
}

void CBacnetAddVirtualDevice::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetAddVirtualDevice, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_VIRTUAL_OK, &CBacnetAddVirtualDevice::OnBnClickedButtonVirtualOk)
	ON_BN_CLICKED(IDC_BUTTON_VIRTUAL_CANCEL, &CBacnetAddVirtualDevice::OnBnClickedButtonVirtualCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_VIRTUAL_PID_NAME, &CBacnetAddVirtualDevice::OnCbnSelchangeComboVirtualPidName)
END_MESSAGE_MAP()


// CBacnetAddVirtualDevice message handlers


BOOL CBacnetAddVirtualDevice::OnInitDialog()
{
	CDialogEx::OnInitDialog();
//#ifdef LOCAL_DB_FUNCTION
	init_product_list();
//#endif

	HICON temp_icon_ok = AfxGetApp()->LoadIconW(IDI_ICON_OK);
	HICON temp_icon_cancel = AfxGetApp()->LoadIconW(IDI_ICON_EXIT);

	((CButton *)GetDlgItem(IDC_BUTTON_VIRTUAL_OK))->SetIcon(temp_icon_ok);
	((CButton *)GetDlgItem(IDC_BUTTON_VIRTUAL_CANCEL))->SetIcon(temp_icon_cancel);
	
	UI_Initial();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBacnetAddVirtualDevice::UI_Initial()
{
	//IDC_COMBO_VIRTUAL_PID_NAME
	//GetProductName
	
	for (int i = 0; i < m_product_iocount.size(); i++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_VIRTUAL_PID_NAME))->AddString(m_product_iocount.at(i).cs_name);
	}
	

	srand((unsigned)time(NULL));
	unsigned int temp_value;
	temp_value = rand() % (10000000) + 1000000;
	CString temp_serial;
	temp_serial.Format(_T("%u"), temp_value);
	GetDlgItem(IDC_EDIT_VIRTUAL_SERIAL_NUMBER)->SetWindowTextW(temp_serial);
	GetDlgItem(IDC_EDIT_VIRTUAL_MODBUS_ID)->SetWindowTextW(_T("1"));
	CString temp_main_building;
	CString temp_sub_building;
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	temp_main_building = pFrame->m_strCurMainBuildingName;
	temp_sub_building = pFrame->m_strCurSubBuldingName;


	GetDlgItem(IDC_EDIT_VIRTUAL_MAIN_BUILDING)->SetWindowTextW(temp_main_building);
	GetDlgItem(IDC_EDIT_VIRTUAL_SUB_BUILDING)->SetWindowTextW(temp_sub_building);
	GetDlgItem(IDC_EDIT_VIRTUAL_MAIN_BUILDING)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_VIRTUAL_SUB_BUILDING)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_VIRTUAL_FLOOR)->SetWindowTextW(_T("Virtual_Floor1"));
	GetDlgItem(IDC_EDIT_VIRTUAL_ROOM)->SetWindowTextW(_T("Virtual_Room1"));
	GetDlgItem(IDC_EDIT_VIRTUAL_FLOOR)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_VIRTUAL_ROOM)->EnableWindow(FALSE);


#if 0
	pidname_map::iterator mtiterator;



	for (mtiterator= m_product_iocount.begin();mtiterator!= m_product_iocount.end();++mtiterator)
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_VIRTUAL_PID_NAME))->AddString(mtiterator->second);
		if(mtiterator->first== PM_MINIPANEL|| mtiterator->first == PM_MINIPANEL_ARM  || mtiterator->first == PM_ESP32_T3_SERIES)
		{
			((CComboBox *)GetDlgItem(IDC_COMBO_VIRTUAL_PID_NAME))->SetWindowTextW(mtiterator->second);
			GetDlgItem(IDC_EDIT_VIRTUAL_PID)->SetWindowTextW(_T("35"));
			GetDlgItem(IDC_EDIT_VIRTUAL_PID)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_VIRTUAL_LABEL_NAME)->SetWindowTextW(_T("Virtual_T3BB"));
			srand((unsigned)time(NULL));
			unsigned int temp_value;
			temp_value = rand()%(10000000) + 1000000;
			CString temp_serial;
			temp_serial.Format(_T("%u"),temp_value);
			GetDlgItem(IDC_EDIT_VIRTUAL_SERIAL_NUMBER)->SetWindowTextW(temp_serial);
			GetDlgItem(IDC_EDIT_VIRTUAL_MODBUS_ID)->SetWindowTextW(_T("1"));
			CString temp_main_building;
			CString temp_sub_building;
			CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
			temp_main_building= pFrame->m_strCurMainBuildingName;
			temp_sub_building= pFrame->m_strCurSubBuldingName;


			GetDlgItem(IDC_EDIT_VIRTUAL_MAIN_BUILDING)->SetWindowTextW(temp_main_building);
			GetDlgItem(IDC_EDIT_VIRTUAL_SUB_BUILDING)->SetWindowTextW(temp_sub_building);
			GetDlgItem(IDC_EDIT_VIRTUAL_MAIN_BUILDING)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_VIRTUAL_SUB_BUILDING)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_VIRTUAL_FLOOR)->SetWindowTextW(_T("Virtual_Floor1"));
			GetDlgItem(IDC_EDIT_VIRTUAL_ROOM)->SetWindowTextW(_T("Virtual_Room1"));
			GetDlgItem(IDC_EDIT_VIRTUAL_FLOOR)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_VIRTUAL_ROOM)->EnableWindow(FALSE);
		}
	}
	//
#endif
}


BOOL CBacnetAddVirtualDevice::PreTranslateMessage(MSG* pMsg)
{
	

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetAddVirtualDevice::OnBnClickedButtonVirtualOk()
{
	
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    int nproduct_id = 0;
	CString temp_main_building;
	CString temp_sub_building;
	CString str_serialid;
	CString virtual_floor;
	CString virtual_room;
	CString pid_name;
	CString pid_class_value;
	CString modbus_id;
	CString str_parents_serial;
	CString is_custom;
	CString str_object_instance;
	CString temp_pro3 =_T("1");
	CString str_ip_address;
	CString str_n_port = _T("502");
	CString str_panel_number;

	CString temp_sub_pid;
	GetDlgItem(IDC_EDIT_VIRTUAL_SUB_PID)->GetWindowText(temp_sub_pid);

	temp_main_building= pFrame->m_strCurMainBuildingName;
	temp_sub_building= pFrame->m_strCurSubBuldingName;
	GetDlgItemText(IDC_EDIT_VIRTUAL_SERIAL_NUMBER,str_serialid);
	GetDlgItemText(IDC_EDIT_VIRTUAL_FLOOR,virtual_floor);
	GetDlgItemText(IDC_EDIT_VIRTUAL_ROOM,virtual_room);
	GetDlgItemText(IDC_EDIT_VIRTUAL_LABEL_NAME,pid_name);
	GetDlgItemText(IDC_EDIT_VIRTUAL_PID,pid_class_value);
	GetDlgItemText(IDC_EDIT_VIRTUAL_MODBUS_ID,modbus_id);
    nproduct_id = _wtoi(pid_class_value);
	str_object_instance = str_serialid;
	str_panel_number = modbus_id ;
	CppSQLite3DB SqliteDBBuilding;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
	CString strSql;
	temp_pro3.Format(_T("%u"), MODBUS_TCPIP);

    if ((nproduct_id == PM_MINIPANEL) || (nproduct_id == PM_MINIPANEL_ARM) || (nproduct_id == PM_ESP32_T3_SERIES))
    {
        CString offline_folder;
        offline_folder = g_strBuildingFolder + pFrame->m_strCurMainBuildingName;
        pFrame->HideBacnetWindow();
        CreateDirectory(offline_folder, NULL);//
        //offline_folder = offline_folder + _T("\\VirtualDeviceData");
		offline_folder = g_strExePth + _T("Database\\temp");
        CreateDirectory(offline_folder, NULL);//
        CString virtual_prg_filename;
        virtual_prg_filename.Format(_T("%s"), str_serialid);
        g_mac = _wtoi( modbus_id);
        g_bac_instance = _wtof(str_object_instance);
        offline_prg_path = offline_folder + _T("\\") +  virtual_prg_filename +_T(".prog");
        ClearBacnetData();
        SaveBacnetBinaryFile(offline_prg_path);
    }

	CString temp_screen_name;
	temp_screen_name.Format(_T("1^%s^"), temp_sub_pid);

	strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,\
		Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Com_Port,EPsize,\
		Protocol,Online_Status,Parent_SerialNum,Panal_Number,Object_Instance,Custom)  \
		values('"+temp_main_building+"','"+temp_sub_building+"','"+str_serialid+"','"+virtual_floor+"','"+virtual_room+"','"+pid_name+"','"+pid_class_value+"','"+modbus_id+"','" + temp_screen_name + "',\
		'"+str_ip_address+"','Modbus_and_Bacnet','"+str_n_port+"','0','"+temp_pro3+"','1','"+str_parents_serial +"' ,'"+str_panel_number +"' ,'"+str_object_instance +"' ,'"+is_custom +"' )"));
	SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
	SqliteDBBuilding.closedb();
	Initial_All_Point();
	Device_Basic_Setting.reg.mini_type = _wtoi(temp_sub_pid);
	Device_Basic_Setting.reg.modbus_id = _wtoi(str_panel_number);
	Device_Basic_Setting.reg.object_instance = _wtoi(str_object_instance);
	Device_Basic_Setting.reg.n_serial_number = _wtoi(str_serialid);
	Initial_Virtual_Device_Setting();
	WideCharToMultiByte(CP_ACP, 0, pid_name.GetBuffer(), -1, Device_Basic_Setting.reg.panel_name, 20, NULL, NULL);
#ifdef LOCAL_DB_FUNCTION
		WriteDeviceDataIntoAccessDB(BAC_IN, input_item_limit_count, Device_Basic_Setting.reg.n_serial_number);
#endif
		SaveBacnetBinaryFile(offline_prg_path);

	MessageBox(_T("Add virtual device success"));
	PostMessage(WM_CLOSE,NULL,NULL);
	::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING, 0, 0);

}



void CBacnetAddVirtualDevice::OnBnClickedButtonVirtualCancel()
{
	
}


void CBacnetAddVirtualDevice::OnCbnSelchangeComboVirtualPidName()
{
	// TODO: 
	CString temp_string;
	int nSel = ((CComboBox*)GetDlgItem(IDC_COMBO_VIRTUAL_PID_NAME))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO_VIRTUAL_PID_NAME))->GetLBText(nSel, temp_string);
	if (nSel >= 0)
	{
		CString temp_pid;
		temp_pid.Format(_T("%d"), m_product_iocount.at(nSel).pid);
		CString temp_sub_pid;
		temp_sub_pid.Format(_T("%d"), m_product_iocount.at(nSel).sub_pid);
		GetDlgItem(IDC_EDIT_VIRTUAL_PID)->SetWindowTextW(temp_pid);
		GetDlgItem(IDC_EDIT_VIRTUAL_SUB_PID)->SetWindowTextW(temp_sub_pid);
		CString temp_serial;
		GetDlgItem(IDC_EDIT_VIRTUAL_SERIAL_NUMBER)->GetWindowTextW(temp_serial);
		CString Temp_name;
		Temp_name.Format(_T("%s-%s(VD)"), m_product_iocount.at(nSel).cs_name, temp_serial);
		GetDlgItem(IDC_EDIT_VIRTUAL_LABEL_NAME)->SetWindowTextW(Temp_name);

		CString cs_ai_count;
		CString cs_bi_count;
		CString cs_ao_count;
		CString cs_bo_count;
		CString cs_total_input;
		CString cs_total_output;
		cs_ai_count.Format(_T("%d"), m_product_iocount.at(nSel).ai_count);
		cs_bi_count.Format(_T("%d"), m_product_iocount.at(nSel).bi_count);
		cs_ao_count.Format(_T("%d"), m_product_iocount.at(nSel).ao_count);
		cs_bo_count.Format(_T("%d"), m_product_iocount.at(nSel).bo_count);
		cs_total_input.Format(_T("%d"), m_product_iocount.at(nSel).input_count);
		cs_total_output.Format(_T("%d"), m_product_iocount.at(nSel).output_count);
		GetDlgItem(IDC_EDIT_VIRTUAL_AI_COUNTS)->SetWindowTextW(cs_ai_count);
		GetDlgItem(IDC_EDIT_VIRTUAL_DI_COUNTS)->SetWindowTextW(cs_bi_count);
		GetDlgItem(IDC_EDIT_VIRTUAL_AO_COUNTS)->SetWindowTextW(cs_ao_count);
		GetDlgItem(IDC_EDIT_VIRTUAL_DO_COUNTS)->SetWindowTextW(cs_bo_count);
		GetDlgItem(IDC_EDIT_VIRTUAL_TOTAL_INPUT_COUNTS)->SetWindowTextW(cs_total_input);
		GetDlgItem(IDC_EDIT_VIRTUAL_TOTAL_OUTPUT_COUNTS2)->SetWindowTextW(cs_total_output);

	}
}
