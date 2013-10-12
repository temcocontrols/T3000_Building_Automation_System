// DialogCM5_BacNet.cpp : implementation file
// DialogCM5 Bacnet programming by Fance 2013 05 01

#include "stdafx.h"
#include "T3000.h"
#include "DialogCM5_BacNet.h"
#include "CM5\MyOwnListCtrl.h"
#include "BacnetInput.h"
#include "BacnetOutput.h"
#include "BacnetProgram.h"
#include "BacnetVariable.h"
#include "globle_function.h"

#include "gloab_define.h"
#include "datalink.h"
#include "BacnetWait.h"
#include "Bacnet_Include.h"
#include "CM5\ud_str.h"
#include "BacnetWeeklyRoutine.h"
#include "BacnetAnnualRoutine.h"

#include "AnnualRout_InsertDia.h"
#include "BacnetController.h"
//bool CM5ProcessPTA(	BACNET_PRIVATE_TRANSFER_DATA * data);
CString temp_device_id,temp_mac,temp_vendor_id;
HANDLE hwait_thread;
BacnetWait *WaitDlg=NULL;
// CDialogCM5_BacNet

_Refresh_Info Bacnet_Refresh_Info;

//#define WM_SEND_OVER     WM_USER + 1287
// int m_Input_data_length;
extern void  init_info_table( void );
extern void Init_table_bank();

IMPLEMENT_DYNCREATE(CDialogCM5_BacNet, CFormView)

CDialogCM5_BacNet::CDialogCM5_BacNet()
	: CFormView(CDialogCM5_BacNet::IDD)
{
	m_MSTP_THREAD = true;
	//CM5_hThread = NULL;
}

CDialogCM5_BacNet::~CDialogCM5_BacNet()
{
}

void CDialogCM5_BacNet::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST1, m_device_list_info);
	DDX_Control(pDX, IDC_TIME_PICKER, m_cm5_time_picker);
	DDX_Control(pDX, IDC_DATE_PICKER, m_cm5_date_picker);
	DDX_Control(pDX, IDC_STATIC_BAC_STATUS, m_bac_static_status);
}

BEGIN_MESSAGE_MAP(CDialogCM5_BacNet, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CDialogCM5_BacNet::OnBnClickedButtonTest)
	ON_MESSAGE(WM_FRESH_CM_LIST,Fresh_UI)





	ON_MESSAGE(MY_RESUME_DATA, AllMessageCallBack)
	ON_MESSAGE(WM_DELETE_WAIT_DLG,Delete_Wait_Dlg)	



	ON_BN_CLICKED(IDC_BUTTON_BAC_TEST, &CDialogCM5_BacNet::OnBnClickedButtonBacTest)
	ON_BN_CLICKED(IDC_BAC_ENABLE_EDIT_TIME, &CDialogCM5_BacNet::OnBnClickedBacEnableEditTime)
	ON_NOTIFY(NM_KILLFOCUS, IDC_DATE_PICKER, &CDialogCM5_BacNet::OnNMKillfocusDatePicker)
	ON_NOTIFY(NM_KILLFOCUS, IDC_TIME_PICKER, &CDialogCM5_BacNet::OnNMKillfocusTimePicker)
	ON_NOTIFY(NM_SETFOCUS, IDC_DATE_PICKER, &CDialogCM5_BacNet::OnNMSetfocusDatePicker)
	ON_NOTIFY(NM_SETFOCUS, IDC_TIME_PICKER, &CDialogCM5_BacNet::OnNMSetfocusTimePicker)
	ON_BN_CLICKED(IDC_BTN_BAC_WRITE_TIME, &CDialogCM5_BacNet::OnBnClickedBtnBacWriteTime)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDialogCM5_BacNet diagnostics

#ifdef _DEBUG
void CDialogCM5_BacNet::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDialogCM5_BacNet::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

//The window which created by the button ,will delete when the wait dialog send this message,to this window.
//It means ,it has done .we don't needed.
LRESULT CDialogCM5_BacNet::Delete_Wait_Dlg(WPARAM wParam,LPARAM lParam)
{
	if(WaitDlg!=NULL)
	{
		Sleep(1000);
		delete WaitDlg;
		WaitDlg = NULL;


		if(bac_read_which_list == BAC_READ_INPUT_LIST)
		{
			if(bac_input_read_results)
			{
			 CBacnetInput DLG;
			 DLG.DoModal();
			}
			else
				MessageBox(_T("Inputs list read time out!"));	
			return 0;
		}

		if(bac_read_which_list == BAC_READ_OUTPUT_LIST)
		{
			if(bac_output_read_results)
			{
				CBacnetOutput DLG;
				DLG.DoModal();
			}
			else
				MessageBox(_T("Outputs list read time out!"));
			return 0;
		}

		if(bac_read_which_list == BAC_READ_VARIABLE_LIST)
		{
			if(bac_variable_read_results)
			{
				CBacnetVariable DLG;
				DLG.DoModal();
			}
			else
				MessageBox(_T("Variable list read time out!"));
			return 0;
		}

		if(bac_read_which_list == BAC_READ_PROGRAM_LIST)
		{
			if(bac_program_read_results)
			{
				CBacnetProgram DLG;
				DLG.DoModal();
			}
			else
				MessageBox(_T("Program list read time out!"));
			return 0;
		}

		if(bac_read_which_list == BAC_READ_WEEKLY_LIST)
		{
			if(bac_weekly_read_results)
			{
				BacnetWeeklyRoutine Dlg;
				Dlg.DoModal();
			}
			else
				MessageBox(_T("Weekly Routine list read time out!"));
		}

		if(bac_read_which_list == BAC_READ_ANNUAL_LIST)
		{
			if(bac_annual_read_results)
			{
				BacnetAnnualRoutine Dlg;
				Dlg.DoModal();
			}
			else
				MessageBox(_T("Annual Routine list read time out!"));
		}
		if(bac_read_which_list == BAC_READ_CONTROLLER_LIST)
		{
			if(bac_controller_read_results)
			{
				BacnetController Dlg;
				Dlg.DoModal();
			}
			else
				MessageBox(_T("Controller list read time out!"));
		}
	}
	return 0;
}
// CDialogCM5_BacNet message handlers

LRESULT  CDialogCM5_BacNet::AllMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	CString Show_Results;
	CString temp_cs = pInvoke->task_info;
	if(msg_result)
	{
		for (int i=0;i<BAC_PROGRAM_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Program_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Program_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_OUTPUT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Output_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Output_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_INPUT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Input_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Input_Info[i].task_result = true;
		}
		for (int i=0;i<BAC_VARIABLE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Variable_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Variable_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_WEEKLY_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Weekly_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Weekly_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_ANNUAL_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Annual_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Annual_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_TIME_COMMAND_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Time_Command[i].invoke_id)
				Bacnet_Refresh_Info.Read_Time_Command[i].task_result = true;
		}
		for (int i=0;i<BAC_CONTROLLER_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Controller_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Controller_Info[i].task_result = true;
		}
		Show_Results = temp_cs + _T("Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		//MessageBox(_T("Bacnet operation success!"));
	}
	else
	{
		for (int i=0;i<BAC_PROGRAM_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Program_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Program_Info[i].task_result = false;

		}

		for (int i=0;i<BAC_OUTPUT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Output_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Output_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_INPUT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Input_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Input_Info[i].task_result = false;
		}
		for (int i=0;i<BAC_VARIABLE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Variable_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Variable_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_WEEKLY_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Weekly_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Weekly_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_ANNUAL_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Annual_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Annual_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_TIME_COMMAND_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Time_Command[i].invoke_id)
				Bacnet_Refresh_Info.Read_Time_Command[i].task_result = false;
		}
		for (int i=0;i<BAC_CONTROLLER_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Controller_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Controller_Info[i].task_result = false;
		}
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		//MessageBox(_T("Bacnet operation fail!"));
	}
	if(pInvoke)
		delete pInvoke;
	return 0;
}

void CDialogCM5_BacNet::OnBnClickedButtonTest()
{
	// TODO: Add your control notification handler code here
	Send_WhoIs_Global(-1,-1);
}


void CDialogCM5_BacNet::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	#ifdef Fance_Enable_Test
	Fresh();//Fance
	Initial_All_Point();
	#endif
	// TODO: Add your specialized code here and/or call the base class
}
void CDialogCM5_BacNet::Initial_All_Point()
{
	m_Input_data.clear();
	m_Variable_data.clear();
	m_Output_data.clear();
	m_Program_data.clear();
	m_Weekly_data.clear();
	m_Annual_data.clear();
	m_Schedual_Time_data.clear();
	m_controller_data.clear();
	for(int i=0;i<BAC_INPUT_ITEM_COUNT;i++)
	{
		Str_in_point temp_in;
		m_Input_data.push_back(temp_in);
	}
	for(int i=0;i<BAC_OUTPUT_ITEM_COUNT;i++)
	{
		Str_out_point temp_out;
		m_Output_data.push_back(temp_out);
	}
	for (int i=0;i<BAC_VARIABLE_ITEM_COUNT;i++)
	{
		Str_variable_point temp_variable;
		m_Variable_data.push_back(temp_variable);
	}
	for(int i=0;i<BAC_PROGRAM_ITEM_COUNT;i++)
	{
		Str_program_point temp_program;
		m_Program_data.push_back(temp_program);
	}
	for(int i=0;i<BAC_WEEKLY_ROUTINES_COUNT;i++)
	{
		Str_weekly_routine_point temp_weekly;
		m_Weekly_data.push_back(temp_weekly);
	}
	for (int i=0;i<BAC_ANNUAL_ROUTINES_COUNT;i++)
	{
		Str_annual_routine_point temp_annual;
		m_Annual_data.push_back(temp_annual);
	}

	for (int i=0;i<BAC_WEEKLY_ROUTINES_COUNT;i++)
	{
		Str_schedual_time_point temp_schedual;
		m_Schedual_Time_data.push_back(temp_schedual);
	}

	for (int i=0;i<BAC_CONTROLLER_COUNT;i++)
	{
		Str_controller_point temp_controller;
		m_controller_data.push_back(temp_controller);
	}

}
//INPUT int test_function_return_value();
void CDialogCM5_BacNet::Fresh()
{


	BACNET_ADDRESS src = {
		0
	};  /* address where message came from */
	uint16_t pdu_len = 0;
	unsigned timeout = 100;     /* milliseconds */
	BACNET_ADDRESS my_address, broadcast_address;
	char my_port[50];
	static bool has_run_once=false;
	if(!has_run_once)
	{
		has_run_once =true;
		bac_program_pool_size = 26624;
		bac_program_size = 0;
		bac_free_memory = 26624;
		hwait_thread = NULL;
		close_com();
		Device_Set_Object_Instance_Number(4194300);
		address_init();
		Init_Service_Handlers();
		dlmstp_set_baud_rate(19200);
		dlmstp_set_mac_address(0);
		dlmstp_set_max_info_frames(DEFAULT_MAX_INFO_FRAMES);
		dlmstp_set_max_master(DEFAULT_MAX_MASTER);
		memset(my_port,0,50);

		CString Program_Path,Program_ConfigFile_Path;
		int g_com=0;
		GetModuleFileName(NULL,Program_Path.GetBuffer(MAX_PATH),MAX_PATH);  //获取当前运行的绝对地址;
		PathRemoveFileSpec(Program_Path.GetBuffer(MAX_PATH) );            //返回绝对地址的上层目录?;
		Program_Path.ReleaseBuffer();
		Program_ConfigFile_Path = Program_Path + _T("\\MyConfig.ini");

		CFileFind fFind;
		if(!fFind.FindFile(Program_ConfigFile_Path))
		{
			WritePrivateProfileStringW(_T("Setting"),_T("ComPort"),_T("1"),Program_ConfigFile_Path);
		}
		g_com = GetPrivateProfileInt(_T("Setting"),_T("ComPort"),1,Program_ConfigFile_Path);
		CString temp_cs;
		temp_cs.Format(_T("COM%d"),g_com);

		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, temp_cs.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		temp_cs.ReleaseBuffer();


		sprintf(my_port,cTemp1);
		dlmstp_init(my_port);


		//Initial_List();
//		BacNet_hwd = this->m_hWnd;
		//    dlenv_init();
		datalink_get_broadcast_address(&broadcast_address);
		//    print_address("Broadcast", &broadcast_address);
		datalink_get_my_address(&my_address);
	//		print_address("Address", &my_address);

		CM5_hThread =CreateThread(NULL,NULL,MSTP_Receive,this,NULL, &nThreadID);
		BacNet_hwd = this->m_hWnd;
		Send_WhoIs_Global(-1,-1);

		
		m_cm5_date_picker.EnableWindow(0);
		m_cm5_time_picker.EnableWindow(0);
		GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->EnableWindow(0);
		GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(FALSE);
		m_cm5_time_picker.SetFormat(_T("HH:mm"));

		m_bac_static_status.SetWindowTextW(_T("Disconnected"));
		m_bac_static_status.textColor(RGB(255,255,255));
		m_bac_static_status.bkColor(RGB(255,0,0));
		m_bac_static_status.setFont(26,18,NULL,_T("Cambria"));
		SetTimer(1,500,NULL);


		init_info_table();
		Init_table_bank();
	}
}









typedef struct BACnet_Object_Property_Value_Own {
	BACNET_OBJECT_TYPE object_type;
	uint32_t object_instance;
	BACNET_PROPERTY_ID object_property;
	uint32_t array_index;
	BACNET_APPLICATION_DATA_VALUE value;
	HTREEITEM t_PropertyChild;
} BACNET_OBJECT_PROPERTY_VALUE_Own;

typedef struct _DEVICE_INFO
{
	uint32_t i_device_id;
	uint32_t i_vendor_id;
	uint32_t i_mac;
	HTREEITEM t_DeviceChild;
	vector	<BACnet_Object_Property_Value_Own> my_Property_value;

}DEVICE_INFO;



vector <_DEVICE_INFO> g_device_info;

//Str_in_point Private_data[100];
//int Private_data_length;
//HWND      m_input_dlg_hwnd;
//HWND      m_pragram_dlg_hwnd;


//extern  MSTP_Port;
volatile struct mstp_port_struct_t MSTP_Port;
static void Read_Properties(
    void)
{
    uint32_t device_id = 0;
    bool status = false;
    unsigned max_apdu = 0;
    BACNET_ADDRESS src;
    bool next_device = false;
    static unsigned index = 0;
    static unsigned property = 0;
    /* list of required (and some optional) properties in the
       Device Object
       note: you could just loop through
       all the properties in all the objects. */
	 const int object_props[] = {
	PROP_OBJECT_LIST//PROP_MODEL_NAME//PROP_OBJECT_LIST
	 };
    //const int object_props[] = {
    //    PROP_OBJECT_IDENTIFIER,
    //    PROP_OBJECT_NAME,
    //    PROP_OBJECT_TYPE,
    //    PROP_SYSTEM_STATUS,
    //    PROP_VENDOR_NAME,
    //    PROP_VENDOR_IDENTIFIER,
    //    PROP_MODEL_NAME,
    //    PROP_FIRMWARE_REVISION,
    //    PROP_APPLICATION_SOFTWARE_VERSION,
    //    PROP_PROTOCOL_VERSION,
    //    PROP_PROTOCOL_SERVICES_SUPPORTED,
    //    PROP_PROTOCOL_OBJECT_TYPES_SUPPORTED,
    //    PROP_MAX_APDU_LENGTH_ACCEPTED,
    //    PROP_SEGMENTATION_SUPPORTED,
    //    PROP_LOCAL_TIME,
    //    PROP_LOCAL_DATE,
    //    PROP_UTC_OFFSET,
    //    PROP_DAYLIGHT_SAVINGS_STATUS,
    //    PROP_APDU_SEGMENT_TIMEOUT,
    //    PROP_APDU_TIMEOUT,
    //    PROP_NUMBER_OF_APDU_RETRIES,
    //    PROP_TIME_SYNCHRONIZATION_RECIPIENTS,
    //    PROP_MAX_MASTER,
    //    PROP_MAX_INFO_FRAMES,
    //    PROP_DEVICE_ADDRESS_BINDING,
    //    /* note: PROP_OBJECT_LIST is missing cause
    //       we need to get it with an index method since
    //       the list could be very large */
    //    /* some proprietary properties */
    //    514, 515,
    //    /* end of list */
    //    -1
    //};

    if (address_count()) {
        if (address_get_by_index(index, &device_id, &max_apdu, &src)) {
            if (object_props[property] < 0)
                next_device = true;
            else {
                /*status*/ g_invoke_id= Send_Read_Property_Request(device_id,  /* destination device */
                    OBJECT_DEVICE, device_id, (BACNET_PROPERTY_ID)object_props[property],
                    BACNET_ARRAY_ALL);
              //  if (status)	Fance
              //      property++;
            }
        } else
            next_device = true;
        if (next_device) {
            next_device = false;
           // index++;Fance
            if (index >= MAX_ADDRESS_CACHE)
                index = 0;
            property = 0;
        }
    }

    return;
}
uint32_t gloab_device_id;
uint32_t g_vendor_id;
uint32_t g_mac;
HTREEITEM FirstChild ;
HTREEITEM SecondChild ;
HTREEITEM DeviceChild;



#define  PRINT_ENABLED 1
void local_rp_ack_print_data(
	BACNET_READ_PROPERTY_DATA * data)
{

	int len = 0;
	uint8_t *application_data;
	int application_data_len;
	bool first_value = true;
	bool print_brace = false;

	

	if (data) 
	{
		application_data = data->application_data;
		application_data_len = data->application_data_len;
		/* FIXME: what if application_data_len is bigger than 255? */
		/* value? need to loop until all of the len is gone... */
		for (;;) 
		{
			BACnet_Object_Property_Value_Own object_value;  /* for bacapp printing */
			BACNET_APPLICATION_DATA_VALUE value;        /* for decode value data */
			len = bacapp_decode_application_data(application_data,(uint8_t) application_data_len, &value);
			if (first_value && (len < application_data_len)) 
			{
				first_value = false;
#if PRINT_ENABLED
				fprintf(stdout, "{");
#endif
				print_brace = true;
			}
			object_value.object_type = data->object_type;
			object_value.object_instance = data->object_instance;
			object_value.object_property = data->object_property;
			object_value.array_index = data->array_index;
			object_value.value = value;


			if(g_device_info.at(0).my_Property_value.size()==0)
			{
				g_device_info.at(0).my_Property_value.push_back(object_value);
			}
			else
			{
				bool find_id=false;
				for (int i=0;i<g_device_info.at(0).my_Property_value.size();i++)
				{
					if((g_device_info.at(0).my_Property_value.at(i).value.type.Object_Id.instance!=object_value.value.type.Object_Id.instance)
						||(g_device_info.at(0).my_Property_value.at(i).value.type.Object_Id.type!=object_value.value.type.Object_Id.type))
						continue;
					else
					{
						find_id = true;
						break;
					}
				}
				if(!find_id)
					g_device_info.at(0).my_Property_value.push_back(object_value);
			}


		//	g_device_info.at(0).my_Property_value.push_back(object_value);

		//	g_device_info.at(0).my_Property_value.push_back(object_value);

				//TRACE("%d",object_value);
		//	bacapp_print_value(stdout, &object_value);
			if (len > 0) 
			{
				if (len < application_data_len) 
				{
					application_data += len;
					application_data_len -= len;
					/* there's more! */

#if PRINT_ENABLED
					fprintf(stdout, ",");
#endif
				} 
				else 
				{
					break;
				}
			} 
			else 
			{
				break;
			}
		}
#if PRINT_ENABLED
		if (print_brace)
			fprintf(stdout, "}");
		fprintf(stdout, "\r\n");
#endif
	}
}

/*
 * This is called when we receive a private transfer packet.
 * We parse the data, send the private part for processing and then send the
 * response which the application generates.
 * If there are any BACnet level errors we send an error response from here.
 * If there are any application level errors they will be packeged up in the
 * response block which we send back to the originator of the request.
 *
 */



 







void Localhandler_read_property_ack(
	uint8_t * service_request,
	uint16_t service_len,
	BACNET_ADDRESS * src,
	BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data)
{
	int len = 0;
	BACNET_READ_PROPERTY_DATA data;

	(void) src;
	(void) service_data;        /* we could use these... */
	len = rp_ack_decode_service_request(service_request, service_len, &data);
#if 0
	fprintf(stderr, "Received Read-Property Ack!\n");
#endif
	if (len > 0)
	{
		local_rp_ack_print_data(&data);
		::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,WM_COMMAND_WHO_IS,NULL);
	}
}

//static void Read_Property_feed_back( uint8_t * service_request,
//	uint16_t service_len,
//	BACNET_ADDRESS * src,
//	BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data)
//{
//	AfxMessageBox("Receive");
//}



static void LocalIAmHandler(
    uint8_t * service_request,
    uint16_t service_len,
    BACNET_ADDRESS * src)
{
	
    int len = 0;
    uint32_t device_id = 0;
    unsigned max_apdu = 0;
    int segmentation = 0;
    uint16_t vendor_id = 0;

    (void) src;
    (void) service_len;
    len =  iam_decode_service_request(service_request, &device_id, &max_apdu,
        &segmentation, &vendor_id);




    fprintf(stderr, "Received I-Am Request");
    if (len != -1) 
	{
        fprintf(stderr, " from %u!\n", device_id);
        address_add(device_id, max_apdu, src);
    } else
        fprintf(stderr, "!\n");

	if(src->mac_len==1)
		temp_mac.Format(_T("%d"),src->mac[0]);

	temp_device_id.Format(_T("%d"),device_id);
	temp_vendor_id.Format(_T("%d"),vendor_id);

	::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,WM_COMMAND_WHO_IS,NULL);
	return;

	//gloab_device_id = device_id;
	//g_vendor_id = vendor_id;
	//if(src->mac_len==1)
	//	g_mac=src->mac[0];
	
	////_DEVICE_INFO temp_info;
	////temp_info.i_device_id = device_id;
	////temp_info.i_vendor_id = vendor_id;
	////if(src->mac_len==1)
	////	temp_info.i_mac=src->mac[0];
	////if(g_device_info.size()==0)
	////{
	////	g_device_info.push_back(temp_info);
	////}
	////else
	////{
	////	bool find_id=false;
	////	for (int i=0;i<g_device_info.size();i++)
	////	{
	////		if(g_device_info.at(i).i_device_id!=device_id)
	////			continue;
	////		else
	////			find_id = true;
	////	}
	////	if(!find_id)
	////		g_device_info.push_back(temp_info);
	////	else
	////		return ;
	////}
	////
	////::PostMessage(BacNet_hwd,WM_ADD_LIST,NULL,NULL);
    return;
}


extern int Station_NUM;
LRESULT CDialogCM5_BacNet::Fresh_UI(WPARAM wParam,LPARAM lParam)
{
	int command_type = wParam;
	int button_click = 0;

	int temp_year;
	CTime	TimeTemp;
	switch(command_type)
	{
	case WM_COMMAND_WHO_IS:
		((CStatic *)GetDlgItem(IDC_STATIC_CM_DEVICE_ID))->SetWindowTextW(temp_device_id);
		bac_gloab_device_id = _wtoi(temp_device_id);
		((CStatic *)GetDlgItem(IDC_STATIC_CM5_MAC))->SetWindowTextW(temp_mac);
		bac_gloab_panel = _wtoi(temp_mac);
		Station_NUM = bac_gloab_panel;;
		((CStatic *)GetDlgItem(IDC_STATIC_CM5_VENDOR_ID))->SetWindowTextW(temp_vendor_id);
		break;
	case TIME_COMMAND:
		if(Device_time.year<2000)
			temp_year = Device_time.year + 2000;
		TimeTemp = CTime(temp_year,Device_time.month,Device_time.dayofmonth,Device_time.ti_hour,Device_time.ti_min,Device_time.ti_sec);
		

		m_cm5_time_picker.SetFormat(_T("HH:mm"));
		m_cm5_time_picker.SetTime(&TimeTemp);

		//m_cm5_date_picker.SetFormat(_T("YY/MM/DD"));
		m_cm5_date_picker.SetTime(&TimeTemp);
		break;;
	case MENU_CLICK:
		button_click = lParam;
		if(button_click == WRITEPRGFLASH_COMMAND)
		{
			WriteFlash();
		}
		else
		{
			Show_Wait_Dialog_And_SendMessage(button_click);
		}
		break;
	default: 
		break;
	}

	
	return 0;
}

DWORD WINAPI   CDialogCM5_BacNet::MSTP_Receive(LPVOID lpVoid)
{
	BACNET_ADDRESS src = {0};
	uint16_t pdu_len;
	CDialogCM5_BacNet *mparent = (CDialogCM5_BacNet *)lpVoid;
	uint8_t Rx_Buf[MAX_MPDU] = { 0 };
	while(mparent->m_MSTP_THREAD)
	{
		pdu_len = dlmstp_receive(&src, &Rx_Buf[0], MAX_MPDU, INFINITE);
		if(pdu_len==0)
			continue;
		npdu_handler(&src, &Rx_Buf[0], pdu_len);
		//CString TEMP1;
		//TEMP1.Format("%s",Rx_Buf);
		//	AfxMessageBox(TEMP1);
	}
	return 0;
}

static void Init_Service_Handlers(
	void)
{
	Device_Init(NULL);

	/* we need to handle who-is to support dynamic device binding */
	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_WHO_IS, handler_who_is);
	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_I_AM, LocalIAmHandler);



	apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_PRIVATE_TRANSFER,local_handler_conf_private_trans_ack);
	//apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY,Read_Property_feed_back);

	apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY,	Localhandler_read_property_ack);
	/* set the handler for all the services we don't implement */
	/* It is required to send the proper reject message... */
	apdu_set_unrecognized_service_handler_handler
		(handler_unrecognized_service);
	/* we must implement read property - it's required! */
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROPERTY,
		handler_read_property);
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROP_MULTIPLE,
		handler_read_property_multiple);
	/* handle the data coming back from confirmed requests */
	//   apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY,handler_read_property_ack);
#if defined(BACFILE)
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_ATOMIC_READ_FILE,
		handler_atomic_read_file);
#endif
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_SUBSCRIBE_COV,
		handler_cov_subscribe);

////#if 0
////	/* Adding these handlers require the project(s) to change. */
////#if defined(BACFILE)
////	apdu_set_confirmed_handler(SERVICE_CONFIRMED_ATOMIC_WRITE_FILE,
////		handler_atomic_write_file);
////#endif
////	apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_RANGE,
////		handler_read_range);
////	apdu_set_confirmed_handler(SERVICE_CONFIRMED_REINITIALIZE_DEVICE,
////		handler_reinitialize_device);
////	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_UTC_TIME_SYNCHRONIZATION,
////		handler_timesync_utc);
////	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_TIME_SYNCHRONIZATION,
////		handler_timesync);
////	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_COV_NOTIFICATION,
////		handler_ucov_notification);
////	/* handle communication so we can shutup when asked */
////	apdu_set_confirmed_handler(SERVICE_CONFIRMED_DEVICE_COMMUNICATION_CONTROL,
////		handler_device_communication_control);
////#endif
}

//void CDialogCM5_BacNet::Initial_List()
//{
//	long style;
//	style=GetWindowLong(m_device_list_info.m_hWnd,GWL_STYLE);
//	style&=~LVS_TYPEMASK;
//	style|=LVS_REPORT;	
//	style|=LVS_EX_CHECKBOXES;
//	//	style|=RC_CHKBOX_SINGLE;
//
//	m_device_list_info.SetExtendedStyle(style);
//	SetWindowLong(m_device_list_info.m_hWnd,GWL_STYLE,style);//list_infomation.m_hWnd´
//	DWORD dwstyle=m_device_list_info.GetExtendedStyle();
//	dwstyle|=LVS_EX_FULLROWSELECT; //
//	dwstyle|=LVS_EX_GRIDLINES;     //
//	m_device_list_info.SetExtendedStyle(dwstyle);
//	m_device_list_info.InsertColumn(0,_T("Device ID"),LVCFMT_CENTER,100);  
//	m_device_list_info.InsertColumn(1,_T("MAC"),LVCFMT_CENTER,200);
//	m_device_list_info.InsertColumn(2,_T("Vendor ID"),LVCFMT_CENTER,200);
//	m_device_list_info.SetTextColor(RGB(0,0,255));
//	m_device_list_info.SetCheckboxeStyle(RC_CHKBOX_SINGLE);
//	//m_list_control.SortItems(0, FALSE); // sort the 1st column, ascending
//	m_device_list_info.SetSortable(FALSE);
//}










//void CDialogCM5_BacNet::OnBnClickedButton1()
//{
//	// TODO: Add your control notification handler code here
//	//WritePrivateData();
//}



void CDialogCM5_BacNet::Show_Wait_Dialog_And_SendMessage(int read_list_type)
{
	bac_read_which_list = read_list_type;
	if(WaitDlg==NULL)
	{
		WaitDlg = new BacnetWait;
		WaitDlg->Create(IDD_DIALOG_BACNET_WAIT,this);
		WaitDlg->ShowWindow(SW_SHOW);

		RECT RECT_SET1;
		GetWindowRect(&RECT_SET1);
		//	GetClientRect(&RECT_SET1);
		WaitDlg->MoveWindow(RECT_SET1.left+100,RECT_SET1.bottom-200,560/*RECT_SET1.left+270*//*RECT_SET1.right/2+20*/,100);
	}

	//::PostMessage(BacNet_hwd,WM_SEND_OVER,0,0);
	if(hwait_thread==NULL)
	{
		hwait_thread =CreateThread(NULL,NULL,Send_read_Command_Thread,this,NULL, NULL);

	}
}



static int resend_count=0;
DWORD WINAPI  CDialogCM5_BacNet::Send_read_Command_Thread(LPVOID lpVoid)
{
	CDialogCM5_BacNet *pParent = (CDialogCM5_BacNet *)lpVoid;
	
	for (int i=0;i<BAC_TIME_COMMAND_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_TIME_COMMAND) || (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			Bacnet_Refresh_Info.Read_Time_Command[i].command = 0;
			Bacnet_Refresh_Info.Read_Time_Command[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Time_Command[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Time_Command[i].end_instance = 0;
			Bacnet_Refresh_Info.Read_Time_Command[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Time_Command[i].resend_count =0;
			Bacnet_Refresh_Info.Read_Time_Command[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Time_Command[i].invoke_id = -1;
			bac_time_command_read_results = 0;
		}
	}

	for(int i=0;i<BAC_WEEKLY_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_WEEKLY_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			Bacnet_Refresh_Info.Read_Weekly_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].has_resend_yes_or_no = false;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].resend_count = 0;

			Bacnet_Refresh_Info.Read_Weekly_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].invoke_id = -1;

			bac_weekly_read_results = 0;
		}
	}

	for(int i=0;i<BAC_ANNUAL_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_ANNUAL_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			Bacnet_Refresh_Info.Read_Annual_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Annual_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Annual_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Annual_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Annual_Info[i].has_resend_yes_or_no = false;
			Bacnet_Refresh_Info.Read_Annual_Info[i].resend_count = 0;

			Bacnet_Refresh_Info.Read_Annual_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Annual_Info[i].invoke_id = -1;

			bac_annual_read_results = 0;
		}
	}
	

	for (int i=0;i<BAC_PROGRAM_GROUP;i++)
	{

		if((bac_read_which_list == BAC_READ_PROGRAM_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			Bacnet_Refresh_Info.Read_Program_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Program_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Program_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Program_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Program_Info[i].has_resend_yes_or_no = false;
			Bacnet_Refresh_Info.Read_Program_Info[i].resend_count = 0;

			Bacnet_Refresh_Info.Read_Program_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Program_Info[i].invoke_id = -1;

			bac_program_read_results = 0;
		}
	}

	for (int i=0;i<BAC_INPUT_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_INPUT_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			Bacnet_Refresh_Info.Read_Input_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Input_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Input_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Input_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Input_Info[i].has_resend_yes_or_no = false;
			Bacnet_Refresh_Info.Read_Input_Info[i].resend_count = 0;
			Bacnet_Refresh_Info.Read_Input_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Input_Info[i].invoke_id = -1;
			bac_input_read_results = 0;

		}
	}


	for (int i=0;i<BAC_OUTPUT_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_OUTPUT_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			Bacnet_Refresh_Info.Read_Output_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Output_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Output_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Output_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Output_Info[i].has_resend_yes_or_no = false;
			Bacnet_Refresh_Info.Read_Output_Info[i].resend_count = 0;
			Bacnet_Refresh_Info.Read_Output_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Output_Info[i].invoke_id = -1;
			bac_output_read_results = 0;

		}
	}

	for (int i=0;i<BAC_VARIABLE_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_VARIABLE_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			Bacnet_Refresh_Info.Read_Variable_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Variable_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Variable_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Variable_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Variable_Info[i].has_resend_yes_or_no = false;
			Bacnet_Refresh_Info.Read_Variable_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Variable_Info[i].invoke_id = -1;
			Bacnet_Refresh_Info.Read_Variable_Info[i].resend_count = 0;
			bac_variable_read_results = 0;
		}
	}

	for (int i=0;i<BAC_CONTROLLER_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_CONTROLLER_LIST) || (bac_read_which_list == BAC_READ_ALL_LIST))
		{
			Bacnet_Refresh_Info.Read_Controller_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Controller_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Controller_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Controller_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Controller_Info[i].has_resend_yes_or_no = false;
			Bacnet_Refresh_Info.Read_Controller_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Controller_Info[i].invoke_id = -1;
			Bacnet_Refresh_Info.Read_Controller_Info[i].resend_count = 0;
			bac_controller_read_results = 0;
		}
	}

	for (int i=0;i<BAC_TIME_COMMAND_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_TIME_COMMAND) || (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>20)
					goto myend;
				g_invoke_id = GetPrivateData(bac_gloab_device_id,
					TIME_COMMAND,0,
					0,
					sizeof(Time_block_mini));
				Sleep(200);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Time_Command[i].command = TIME_COMMAND;
			Bacnet_Refresh_Info.Read_Time_Command[i].device_id = bac_gloab_device_id;
			Bacnet_Refresh_Info.Read_Time_Command[i].start_instance = 0;
			Bacnet_Refresh_Info.Read_Time_Command[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Time_Command[i].invoke_id = g_invoke_id;

			CString temp_cs_show;
			temp_cs_show.Format(_T("Task ID = %d. Read device time "),g_invoke_id);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);

			//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,_T("1111111111111"));
			//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);
		}
	}
	// TODO: Add your control notification handler code here
	for (int i=0;i<BAC_WEEKLY_GROUP;i++)
	{

		if((bac_read_which_list == BAC_READ_WEEKLY_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>20)
					goto myend;
				g_invoke_id = GetPrivateData(bac_gloab_device_id,
					READWEEKLYROUTINE_T3000,(BAC_READ_GROUP_NUMBER)*i,
					3+(BAC_READ_GROUP_NUMBER)*i,
					sizeof(Str_weekly_routine_point));
				Sleep(200);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Weekly_Info[i].command = READWEEKLYROUTINE_T3000;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].device_id = bac_gloab_device_id;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].invoke_id = g_invoke_id;

			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Weekly Routine Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Weekly_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Weekly_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}

	for (int i=0;i<BAC_ANNUAL_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_ANNUAL_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>20)
					goto myend;
				g_invoke_id = GetPrivateData(bac_gloab_device_id,
					READANNUALROUTINE_T3000,(BAC_READ_GROUP_NUMBER)*i,
					3+(BAC_READ_GROUP_NUMBER)*i,
					sizeof(Str_annual_routine_point));
				Sleep(200);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Annual_Info[i].command = READANNUALROUTINE_T3000;
			Bacnet_Refresh_Info.Read_Annual_Info[i].device_id = bac_gloab_device_id;
			Bacnet_Refresh_Info.Read_Annual_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Annual_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Annual_Info[i].invoke_id = g_invoke_id;


			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Annual Routine Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Annual_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Annual_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}


	for (int i=0;i<BAC_OUTPUT_GROUP;i++)
	{
		
		if((bac_read_which_list == BAC_READ_OUTPUT_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>20)
					goto myend;
				g_invoke_id = GetPrivateData(bac_gloab_device_id,
											 READOUTPUT_T3000,(BAC_READ_GROUP_NUMBER)*i,
											 3+(BAC_READ_GROUP_NUMBER)*i,
											 sizeof(Str_out_point));
				Sleep(200);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Output_Info[i].command = READOUTPUT_T3000;
			Bacnet_Refresh_Info.Read_Output_Info[i].device_id = bac_gloab_device_id;
			Bacnet_Refresh_Info.Read_Output_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Output_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Output_Info[i].invoke_id = g_invoke_id;

			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Output List Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Output_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Output_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}



	for (int i=0;i<BAC_INPUT_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_INPUT_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>20)
					goto myend;
				g_invoke_id = GetPrivateData(bac_gloab_device_id,READINPUT_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i,sizeof(Str_in_point));
				Sleep(200);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Input_Info[i].command = READINPUT_T3000;
			Bacnet_Refresh_Info.Read_Input_Info[i].device_id = bac_gloab_device_id;
			Bacnet_Refresh_Info.Read_Input_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Input_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Input_Info[i].invoke_id = g_invoke_id;


			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Input List Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Input_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Input_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}

	for (int i=0;i<BAC_VARIABLE_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_VARIABLE_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			int resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>20)
					goto myend;
				g_invoke_id = GetPrivateData(bac_gloab_device_id,READVARIABLE_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i,sizeof(Str_variable_point));
				//TRACE(_T("g_invoke_id = %d\r\n",g_invoke_id));
				Sleep(200);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Variable_Info[i].command = READVARIABLE_T3000;
			Bacnet_Refresh_Info.Read_Variable_Info[i].device_id = bac_gloab_device_id;
			Bacnet_Refresh_Info.Read_Variable_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Variable_Info[i].end_instance = 3+(BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Variable_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Variable List Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Variable_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Variable_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}

	for (int i=0;i<BAC_PROGRAM_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_PROGRAM_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			int resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>20)
					goto myend;
				g_invoke_id = GetPrivateData(bac_gloab_device_id,READPROGRAM_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i,sizeof(Str_program_point));
				Sleep(200);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Program_Info[i].command = READPROGRAM_T3000;
			Bacnet_Refresh_Info.Read_Program_Info[i].device_id = bac_gloab_device_id;
			Bacnet_Refresh_Info.Read_Program_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Program_Info[i].end_instance = 3+(BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Program_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Program List Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Program_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Program_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}


	for (int i=0;i<BAC_CONTROLLER_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_CONTROLLER_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			int resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>20)
					goto myend;
				g_invoke_id = GetPrivateData(bac_gloab_device_id,READCONTROLLER_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i,sizeof(Str_controller_point));
				Sleep(200);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Controller_Info[i].command = READCONTROLLER_T3000;
			Bacnet_Refresh_Info.Read_Controller_Info[i].device_id = bac_gloab_device_id;
			Bacnet_Refresh_Info.Read_Controller_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Controller_Info[i].end_instance = 3+(BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Controller_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Controller List Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Controller_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Controller_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}



		hwait_thread = NULL;

	return 0;
		//::PostMessage(BacNet_hwd,WM_SEND_OVER,0,0);
myend:	hwait_thread = NULL;
		AfxMessageBox(_T("Send Command Timeout!"));
		::PostMessage(BacNet_hwd,WM_DELETE_WAIT_DLG,0,0);
	return 0;
}


void CDialogCM5_BacNet::WriteFlash()
{
	// TODO: Add your control notification handler code here
	//WRITEPRGFLASH_COMMAND
	int resend_count = 0;
	do 
	{
		resend_count ++;
		if(resend_count>10)
			break;
		g_invoke_id = GetPrivateData(bac_gloab_device_id,WRITEPRGFLASH_COMMAND,0,0,0);
		Sleep(100);
	} while (g_invoke_id<0);

	if(g_invoke_id>0)
	{
		CString temp_cs_show;
		temp_cs_show.Format(_T("Task ID = %d. Write into flash "),g_invoke_id);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);
	}


	//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);
}








void CDialogCM5_BacNet::OnBnClickedButtonBacTest()
{
	// TODO: Add your control notification handler code here

	int	resend_count = 0;
	do 
	{
		resend_count ++;
		if(resend_count>20)
			return;
		g_invoke_id = GetPrivateData(bac_gloab_device_id,
			TIME_COMMAND,0,
			0,
			sizeof(Time_block_mini));
		Sleep(200);
	} while (g_invoke_id<0);

	CString temp_cs_show;
	temp_cs_show.Format(_T("Task ID = %d. Read time "),g_invoke_id);
	Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);

	//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);


}


void CDialogCM5_BacNet::OnBnClickedBacEnableEditTime()
{
	// TODO: Add your control notification handler code here
	static bool edit_status = false;
	if(edit_status == false)
	{
		edit_status = true;
		m_cm5_date_picker.EnableWindow(1);
		m_cm5_time_picker.EnableWindow(1);
		GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->EnableWindow(1);
		GetDlgItem(IDC_BAC_ENABLE_EDIT_TIME)->SetWindowTextW(_T("Disable Edit"));
	}
	else
	{
		edit_status = false;
		m_cm5_date_picker.EnableWindow(0);
		m_cm5_time_picker.EnableWindow(0);
		GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->EnableWindow(0);
		GetDlgItem(IDC_BAC_ENABLE_EDIT_TIME)->SetWindowTextW(_T("Enable Edit"));
	}

}

void CDialogCM5_BacNet::Get_Time_Edit_By_Control()
{
	CTime temp_day;

	m_cm5_date_picker.GetTime(temp_day);
	Device_time.year = temp_day.GetYear();
	Device_time.month = temp_day.GetMonth();
	Device_time.dayofmonth = temp_day.GetDay();
	Device_time.dayofweek = temp_day.GetDayOfWeek();
	Device_time.dayofyear = 1;

	
	CTime temp_time;
	m_cm5_time_picker.GetTime(temp_time);
	Device_time.ti_hour = temp_time.GetHour();
	Device_time.ti_min = temp_time.GetMinute();
	Device_time.ti_sec = temp_time.GetMinute();
	CString temp_task_info;
	temp_task_info.Format(_T("Write Device Time.Changed to %02d-%02d %02d:%02d  "),
		Device_time.month,Device_time.dayofmonth,
		Device_time.ti_hour,Device_time.ti_min);
	Post_Write_Message(bac_gloab_device_id,RESTARTMINI_COMMAND,0,0,sizeof(Time_block_mini),this->m_hWnd,temp_task_info);
}

void CDialogCM5_BacNet::OnNMKillfocusDatePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	Get_Time_Edit_By_Control();
	//GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(0);
	*pResult = 0;
}


void CDialogCM5_BacNet::OnNMKillfocusTimePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	Get_Time_Edit_By_Control();
	//GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(0);
	*pResult = 0;
}




void CDialogCM5_BacNet::OnNMSetfocusDatePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(TRUE);
	*pResult = 0;
}


void CDialogCM5_BacNet::OnNMSetfocusTimePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(TRUE);
	*pResult = 0;
}


void CDialogCM5_BacNet::OnBnClickedBtnBacWriteTime()
{
	// TODO: Add your control notification handler code here
	int aaa = Get_MSTP_Connect_Status();
	TRACE(_T("Status is %d\r\n"),aaa);
}


void CDialogCM5_BacNet::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	bool connect_results = Get_MSTP_Connect_Status();
	static bool Old_connect_results = false;
	
	switch(nIDEvent)
	{
	case 1:
		if(Old_connect_results != connect_results)//Prevent repeatedly to redraw the Static.
		{
			if(connect_results)
			{
				m_bac_static_status.SetWindowTextW(_T("Connected"));
				m_bac_static_status.textColor(RGB(0,0,255));
				m_bac_static_status.bkColor(RGB(0,255,0));
			}
			else
			{
				m_bac_static_status.SetWindowTextW(_T("Disconnected"));
				m_bac_static_status.textColor(RGB(255,255,255));
				m_bac_static_status.bkColor(RGB(255,0,0));
			}
		}
		break;
	default:
		break;
	}
	Old_connect_results = connect_results;
	CFormView::OnTimer(nIDEvent);
}
