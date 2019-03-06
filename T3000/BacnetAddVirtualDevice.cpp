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
END_MESSAGE_MAP()


// CBacnetAddVirtualDevice message handlers


BOOL CBacnetAddVirtualDevice::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
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
	pidname_map::iterator mtiterator;
	for (mtiterator=product_map.begin();mtiterator!=product_map.end();++mtiterator)
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_VIRTUAL_PID_NAME))->AddString(mtiterator->second);
		if(mtiterator->first== PM_MINIPANEL|| mtiterator->first == PM_MINIPANEL_ARM)
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
	CString str_n_port;
	CString str_panel_number;
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
	temp_pro3.Format(_T("%u"), PROTOCOL_VIRTUAL);

    if ((nproduct_id == PM_MINIPANEL) || (nproduct_id == PM_MINIPANEL_ARM))
    {
        CString offline_folder;
        offline_folder = g_strBuildingFolder + pFrame->m_strCurMainBuildingName;
        pFrame->HideBacnetWindow();
        CreateDirectory(offline_folder, NULL);//
        offline_folder = offline_folder + _T("\\VirtualDeviceData");
        CreateDirectory(offline_folder, NULL);//
        CString virtual_prg_filename;
        virtual_prg_filename.Format(_T("%s"), str_serialid);
        g_mac = _wtoi( modbus_id);
        g_bac_instance = _wtof(str_object_instance);
        offline_prg_path = offline_folder + _T("\\") +  virtual_prg_filename +_T(".prog");
        ClearBacnetData();
        SaveBacnetBinaryFile(offline_prg_path);
    }

	strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,\
		Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Com_Port,EPsize,\
		Protocol,Online_Status,Parent_SerialNum,Panal_Number,Object_Instance,Custom)  \
		values('"+temp_main_building+"','"+temp_sub_building+"','"+str_serialid+"','"+virtual_floor+"','"+virtual_room+"','"+pid_name+"','"+pid_class_value+"','"+modbus_id+"','""',\
		'"+str_ip_address+"','T3000_Default_Building_PIC.bmp','"+str_n_port+"','0','"+temp_pro3+"','1','"+str_parents_serial +"' ,'"+str_panel_number +"' ,'"+str_object_instance +"' ,'"+is_custom +"' )"));
	SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
	SqliteDBBuilding.closedb();
	MessageBox(_T("Add virtual device success"));
	PostMessage(WM_CLOSE,NULL,NULL);
	::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING, 0, 0);

}



void CBacnetAddVirtualDevice::OnBnClickedButtonVirtualCancel()
{
	
}
