// BacnetTool.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetTool.h"
#include "afxdialogex.h"
#include "global_function.h"
#include "BacnetToolReadProperty.h"

bool read_property_cancel = true;
int object_instance = 0;
int object_identifier =0;
int property_identifier = 0;
int Read_Properties(uint32_t deviceid, BACNET_OBJECT_TYPE object_type,int property_id);
#define  WM_FRESH_BAC_TOOL_LISTVIEW WM_USER + 4002
const int EDIT_ONE = 1;

struct Htree_Info
{
	CString show_cstring;
	HTREEITEM hTreeItem;
	int treestatus;
};

struct _Bac_Tool_Device_Info
{
	Htree_Info Device_Htree;
	Htree_Info Htree_Update_time;
	Htree_Info Htree_IP_Address;
	Htree_Info Htree_Object_identifier;
	Htree_Info Htree_Vendor_identifier;
	Htree_Info Htree_Prop_object_name;




	//int Object_identifier;
	//int Vendor_identifier;
}Bac_Tool_Device_Info;
HWND bac_tool_hwnd;
vector <_Bac_Tool_Device_Info> m_bacnet_tool_device;
HTREEITEM ToolFirstChild ;
// CBacnetTool dialog
void Init_Service_Handlers_Tool(void);
void LocalIAmHandler_Tool(	uint8_t * service_request,	uint16_t service_len,	BACNET_ADDRESS * src);
void Localhandler_read_property_ack_tool(	uint8_t * service_request,	uint16_t service_len,
						BACNET_ADDRESS * src,	BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data);


void Localhandler_write_property_ack_tool(uint8_t * service_request, uint16_t service_len,
    BACNET_ADDRESS * src, BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data);
IMPLEMENT_DYNAMIC(CBacnetTool, CDialogEx)

CBacnetTool::CBacnetTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetTool::IDD, pParent)
{

}

CBacnetTool::~CBacnetTool()
{
}

void CBacnetTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BAC_TOOL_TREE, m_TreeCtrl);
}


BEGIN_MESSAGE_MAP(CBacnetTool, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BAC_TOOL_EXIT, &CBacnetTool::OnBnClickedButtonBacToolExit)
	ON_MESSAGE(WM_FRESH_BAC_TOOL_LISTVIEW,&CBacnetTool::FreshListView)
	ON_BN_CLICKED(IDC_BUTTON_BAC_TOOL_TEST2, &CBacnetTool::OnBnClickedButtonBacToolTest2)
END_MESSAGE_MAP()


// CBacnetTool message handlers


BOOL CBacnetTool::PreTranslateMessage(MSG* pMsg)
{
	

	return CDialogEx::PreTranslateMessage(pMsg);
}



LRESULT CBacnetTool::FreshListView(WPARAM wParam, LPARAM lParam)
{
	for (int i=0;i<(int)m_bacnet_tool_device.size();i++)
	{
		if(m_bacnet_tool_device.at(i).Device_Htree.hTreeItem == NULL)
			m_bacnet_tool_device.at(i).Device_Htree.hTreeItem = m_TreeCtrl.InsertItem(TVIF_TEXT,DEVICE_CS + m_bacnet_tool_device.at(i).Htree_Object_identifier.show_cstring, 0, 0, 0, 0, 0, ToolFirstChild, NULL);
		else
			m_TreeCtrl.SetItemText(m_bacnet_tool_device.at(i).Device_Htree.hTreeItem,DEVICE_CS + m_bacnet_tool_device.at(i).Htree_Object_identifier.show_cstring);

		if(m_bacnet_tool_device.at(i).Htree_Update_time.hTreeItem == NULL)
			m_bacnet_tool_device.at(i).Htree_Update_time.hTreeItem = m_TreeCtrl.InsertItem(TVIF_TEXT,LAST_UPDATE_CS + m_bacnet_tool_device.at(i).Htree_Update_time.show_cstring, 0, 0, 0, 0, 0 ,m_bacnet_tool_device.at(i).Device_Htree.hTreeItem,NULL);
		else
			m_TreeCtrl.SetItemText(m_bacnet_tool_device.at(i).Htree_Update_time.hTreeItem,LAST_UPDATE_CS + m_bacnet_tool_device.at(i).Htree_Update_time.show_cstring);

		if(m_bacnet_tool_device.at(i).Htree_IP_Address.hTreeItem == NULL)
			m_bacnet_tool_device.at(i).Htree_IP_Address.hTreeItem = m_TreeCtrl.InsertItem(TVIF_TEXT,IP_ADDRESS_CS + m_bacnet_tool_device.at(i).Htree_IP_Address.show_cstring, 0, 0, 0, 0, 0,m_bacnet_tool_device.at(i).Device_Htree.hTreeItem,NULL);
		else
			m_TreeCtrl.SetItemText(m_bacnet_tool_device.at(i).Htree_IP_Address.hTreeItem,IP_ADDRESS_CS + m_bacnet_tool_device.at(i).Htree_IP_Address.show_cstring);

		if(m_bacnet_tool_device.at(i).Htree_Object_identifier.hTreeItem == NULL)
			m_bacnet_tool_device.at(i).Htree_Object_identifier.hTreeItem = m_TreeCtrl.InsertItem(TVIF_TEXT,OBJECT_IDENTIFIER_CS + m_bacnet_tool_device.at(i).Htree_Object_identifier.show_cstring, 0, 0, 0, 0, 0,m_bacnet_tool_device.at(i).Device_Htree.hTreeItem,NULL);
		else
			m_TreeCtrl.SetItemText(m_bacnet_tool_device.at(i).Htree_Object_identifier.hTreeItem,OBJECT_IDENTIFIER_CS + m_bacnet_tool_device.at(i).Htree_Object_identifier.show_cstring);
		if(m_bacnet_tool_device.at(i).Htree_Vendor_identifier.hTreeItem == NULL)
			m_bacnet_tool_device.at(i).Htree_Vendor_identifier.hTreeItem = m_TreeCtrl.InsertItem(TVIF_TEXT,VENDOR_IDENTIFIER_CS + m_bacnet_tool_device.at(i).Htree_Vendor_identifier.show_cstring, 0, 0, 0, 0, 0,m_bacnet_tool_device.at(i).Device_Htree.hTreeItem,NULL);
		else
			m_TreeCtrl.SetItemText(m_bacnet_tool_device.at(i).Htree_Vendor_identifier.hTreeItem,VENDOR_IDENTIFIER_CS + m_bacnet_tool_device.at(i).Htree_Vendor_identifier.show_cstring);

		if(m_bacnet_tool_device.at(i).Htree_Prop_object_name.treestatus == TREE_NEED_NEW)
		{
			m_bacnet_tool_device.at(i).Htree_Prop_object_name.hTreeItem = m_TreeCtrl.InsertItem(TVIF_TEXT,m_bacnet_tool_device.at(i).Htree_Prop_object_name.show_cstring, 0, 0, 0, 0, 0,m_bacnet_tool_device.at(i).Device_Htree.hTreeItem,NULL);
			m_bacnet_tool_device.at(i).Htree_Prop_object_name.treestatus = TREE_ALREADY_EXSIT;
		}
		else if(m_bacnet_tool_device.at(i).Htree_Prop_object_name.treestatus == TREE_ALREADY_EXSIT)
		{
			m_TreeCtrl.SetItemText(m_bacnet_tool_device.at(i).Htree_Prop_object_name.hTreeItem,m_bacnet_tool_device.at(i).Htree_Prop_object_name.show_cstring);
		}
	}
	
	//Device_Htree
	return 0;
}

BOOL CBacnetTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	

	InitialTemcoLogo();
	InitialListView();

	InitialBacnetConnection();
	bac_tool_hwnd =  this->m_hWnd;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBacnetTool::InitialTemcoLogo()
{
	hBitmap =(HBITMAP)LoadImage(AfxGetInstanceHandle(),  
		MAKEINTRESOURCE(IDB_BITMAP_TMCO_LOGO),  
		IMAGE_BITMAP,0,0,  
		LR_LOADMAP3DCOLORS);  

	CStatic *pStatic=(CStatic *)GetDlgItem(IDC_STATIC_BAC_LOGO); //获得指向静态控件的指针 
	pStatic->ModifyStyle(0xF,SS_BITMAP/*|SS_CENTERIMAGE*/);  //设置静态控件的样式，使其位图居中  ;
	pStatic->SetBitmap(hBitmap);     //设置静态控件显示位图 
}

void CBacnetTool::InitialBacnetConnection()
{
	SOCKET my_sokect;
	Device_Set_Object_Instance_Number(4194300);
	address_init();
	Init_Service_Handlers_Tool();
	int ret_1 = Open_bacnetSocket2(_T("192.168.0.38"),BACNETIP_PORT + 12,my_sokect);
	bip_set_socket(my_sokect);
	bip_set_port(htons(47808));
	static in_addr BIP_Broadcast_Address;
	BIP_Broadcast_Address.S_un.S_addr =  inet_addr("255.255.255.255");
	bip_set_broadcast_addr((uint32_t)BIP_Broadcast_Address.S_un.S_addr);


	PHOSTENT  hostinfo;  
	char  name[255]; 
	CString  cs_myip; 
	if(  gethostname  (  name,  sizeof(name))  ==  0)  
	{  
		if((hostinfo  =  gethostbyname(name))  !=  NULL)  
		{  
			cs_myip  =  inet_ntoa  (*(struct  in_addr  *)*hostinfo->h_addr_list);  
		}  
	}  
	char cTemp1[255];
	memset(cTemp1,0,255);
	WideCharToMultiByte( CP_ACP, 0, cs_myip.GetBuffer(), -1, cTemp1, 255, NULL, NULL );


	static in_addr BIP_Address;
	BIP_Address.S_un.S_addr =  inet_addr(cTemp1);
	bip_set_addr((uint32_t)BIP_Address.S_un.S_addr);


    if (CM5_hThread == NULL)
    {
        CM5_hThread = CreateThread(NULL, NULL, MSTP_Receive, this, NULL, &nThreadID_mstp);
        CloseHandle(CM5_hThread);
    }
	Send_WhoIs_Global(-1, -1);
}


void CBacnetTool::InitialListView()
{
	m_TreeCtrl.SetTextColor(RGB(0,0,0));
	DWORD styles = GetWindowLong(m_TreeCtrl.m_hWnd,GWL_STYLE);
	styles |= TVS_HASLINES | TVS_LINESATROOT | TVS_INFOTIP |TVS_HASBUTTONS;
	SetWindowLong(m_TreeCtrl.m_hWnd,GWL_STYLE,styles);
	ToolFirstChild = m_TreeCtrl.InsertItem(_T("BACnet IP"),0,1);
}


void CBacnetTool::OnCancel()
{
	

	CDialogEx::OnCancel();
}


void Init_Service_Handlers_Tool(void)
{
	Device_Init(NULL);

	/* we need to handle who-is to support dynamic device binding */
	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_WHO_IS, handler_who_is);
	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_I_AM, LocalIAmHandler_Tool);



	apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_PRIVATE_TRANSFER,local_handler_conf_private_trans_ack);
	//apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY,Read_Property_feed_back);

	apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY,	Localhandler_read_property_ack_tool);


    apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_WRITE_PROPERTY, Localhandler_write_property_ack_tool);
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



void LocalIAmHandler_Tool(	uint8_t * service_request,	uint16_t service_len,	BACNET_ADDRESS * src)
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
	_Bac_Tool_Device_Info temp;
	/*memset(&temp,0,sizeof(_Bac_Tool_Device_Info));*/
	CTime time=CTime::GetCurrentTime();
	CString str=time.Format("%Y-%m-%d %H:%M:%S");
	temp.Htree_Update_time.show_cstring = str;


	address_add(device_id, max_apdu, src);
	temp.Htree_IP_Address.show_cstring.Format(_T("%d.%d.%d.%d"),(unsigned char)src->mac[0],(unsigned char)src->mac[1],(unsigned char)src->mac[2],(unsigned char)src->mac[3]);
	temp.Htree_Object_identifier.show_cstring.Format(_T("%d"),device_id);
	temp.Htree_Vendor_identifier.show_cstring.Format(_T("%d"),vendor_id);
	bool find_exsit = false;
	for (int i=0;i<(int)m_bacnet_tool_device.size();i++)
	{
		if(temp.Htree_Object_identifier.show_cstring.CompareNoCase(m_bacnet_tool_device.at(i).Htree_Object_identifier.show_cstring) ==0 ) 
		{
			m_bacnet_tool_device.at(i).Htree_Update_time.show_cstring = temp.Htree_Update_time.show_cstring;
			find_exsit = true;
		}
	}
	if(!find_exsit)
	{
		temp.Device_Htree.hTreeItem = NULL;
		temp.Htree_IP_Address.hTreeItem = NULL;
		temp.Htree_Object_identifier.hTreeItem = NULL;
		temp.Htree_Update_time.hTreeItem = NULL;
		temp.Htree_Vendor_identifier.hTreeItem = NULL;
		m_bacnet_tool_device.push_back(temp);
	}
	PostMessage(bac_tool_hwnd,WM_FRESH_BAC_TOOL_LISTVIEW,NULL,NULL);
	return;
#if 1
	if(src->mac_len==6)
		bac_cs_mac.Format(_T("%d"),src->mac[3]);
	else
		return;
#endif
	//	bac_cs_mac.Format(_T("%d"),vendor_id);

	bac_cs_device_id.Format(_T("%d"),device_id);


	::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,WM_COMMAND_WHO_IS,NULL);
	return;

}


void CBacnetTool::OnBnClickedButtonBacToolExit()
{
	
	Send_WhoIs_Global(-1, -1);
}

int Write_Properties(uint32_t deviceid, BACNET_OBJECT_TYPE object_type, int property_id)
{
    // uint32_t device_id = 0;
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
    int object_props[] = {
        property_id//PROP_MODEL_NAME//PROP_OBJECT_LIST
    };

        BACNET_APPLICATION_DATA_VALUE test123 = { 0x00 };
        test123.tag = TPYE_BACAPP_CHARACTER_STRING;
        test123.context_specific = false;
        test123.type.Character_String.length = 10;
        strcpy(test123.type.Character_String.value, "1123");



        g_invoke_id =  Send_Write_Property_Request(deviceid,
            object_type, deviceid,
            (BACNET_PROPERTY_ID)object_props[property], &test123,
            16,
            BACNET_ARRAY_ALL);

       // g_invoke_id = Send_Write_Property_Request(deviceid, object_type, deviceid, (BACNET_PROPERTY_ID)object_props[property], &test123, 1, BACNET_ARRAY_ALL);
        while (1)
        {
            Sleep(10);
            if (tsm_invoke_id_free(g_invoke_id))
            {
                break;
            }
            else
                continue;
        }



        return 1;

}

//extern  MSTP_Port;
//volatile struct mstp_port_struct_t MSTP_Port;
int Read_Properties(uint32_t deviceid, BACNET_OBJECT_TYPE object_type,uint32_t object_instance,int property_id)
{
   // uint32_t device_id = 0;
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
	int object_props[] = {
		property_id//PROP_MODEL_NAME//PROP_OBJECT_LIST
	};
	// const int object_props[] = {
	//PROP_OBJECT_LIST//PROP_MODEL_NAME//PROP_OBJECT_LIST
	// };
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

 //   if (address_count()) 
	//{
 //       if (address_get_by_index(index, &device_id, &max_apdu, &src)) 
	//	{
 //           if (object_props[property] < 0)
 //               next_device = true;
 //           else 
	//		{
                /*status*/ 
	g_invoke_id= Send_Read_Property_Request(deviceid,object_type,object_instance,(BACNET_PROPERTY_ID)object_props[property],BACNET_ARRAY_ALL);
              //  if (status)	Fance
              //      property++;
           // }
        //} else
        //    next_device = true;
        //if (next_device) {
        //    next_device = false;
        //   // index++;Fance
        //    if (index >= MAX_ADDRESS_CACHE)
        //        index = 0;
        //    property = 0;
        //}
    //}

    return g_invoke_id;
}


void Localhandler_write_property_ack_tool(
    uint8_t * service_request,
    uint16_t service_len,
    BACNET_ADDRESS * src,
    BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data)
{
    int len = 0;
    BACNET_READ_PROPERTY_DATA data;

    (void)src;
    (void)service_data;        /* we could use these... */
    len = rp_ack_decode_service_request(service_request, service_len, &data);
    //char my_pro_name[100];
    //char * temp = get_prop_name();
    //strcpy_s(my_pro_name,100,temp);
    Sleep(1);
}



void Localhandler_read_property_ack_tool(
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
	//char my_pro_name[100];
	//char * temp = get_prop_name();
	//strcpy_s(my_pro_name,100,temp);

#if 0
	fprintf(stderr, "Received Read-Property Ack!\n");
#endif
	if (len > 0)
	{
		local_rp_ack_print_data(&data);
		HTREEITEM Find_Instance_Tree;
		int Find_Item;
		for (int i=0;i<(int)m_bacnet_tool_device.size();i++)
		{
			if(data.object_instance == _wtoi(m_bacnet_tool_device.at(i).Htree_Object_identifier.show_cstring))
			{
				Find_Instance_Tree = m_bacnet_tool_device.at(i).Device_Htree.hTreeItem;
				Find_Item = i;
				char my_pro_name[100];
				char * temp = get_prop_name();
				strcpy_s(my_pro_name,100,temp);
				char temp_value[100];
				if(receive_object_value.value->tag == 7)
				memcpy_s(temp_value,100,&receive_object_value.value->type.Character_String.value,100);
				strcat(my_pro_name,":");
				strcat(my_pro_name,temp_value);
				CString temp_showvalue;
				MultiByteToWideChar( CP_ACP, 0, my_pro_name, (int)strlen(my_pro_name)+1, 
					temp_showvalue.GetBuffer(MAX_PATH), MAX_PATH );
				temp_showvalue.ReleaseBuffer();
				m_bacnet_tool_device.at(i).Htree_Prop_object_name.show_cstring = temp_showvalue;
				//如果treeview 对应的item 例如1001 没有此项 就在此设置为new;之后会在fresh tree 的函数里面将此项加进去，并设置为已经存在;
				if(m_bacnet_tool_device.at(i).Htree_Prop_object_name.treestatus != TREE_ALREADY_EXSIT)
					m_bacnet_tool_device.at(i).Htree_Prop_object_name.treestatus = TREE_NEED_NEW;
				PostMessage(bac_tool_hwnd,WM_FRESH_BAC_TOOL_LISTVIEW,NULL,NULL);
				//if(data.object_property == PROP_OBJECT_NAME)
				//{
				//	if(m_bacnet_tool_device.at(Find_Item).Htree_Prop_object_name == NULL)
				//		m_bacnet_tool_device.at(i).Htree_Prop_object_name = m_TreeCtrl.InsertItem(TVIF_TEXT,temp_showvalue, 0, 0, 0, 0, 0, Find_Instance_Tree, NULL);
				//	else
				//		m_TreeCtrl.SetItemText(m_bacnet_tool_device.at(i).Device_Htree,temp_showvalue);
				//}
				//break;
			}
		}

	}
}




void CBacnetTool::OnBnClickedButtonBacToolTest2()
{
	
	CBacnetToolReadProperty Dlg;
	Dlg.DoModal();
	if(!read_property_cancel)
	{
        //Write_Properties(object_instance, (BACNET_OBJECT_TYPE)object_identifier, property_identifier);
		Read_Properties(object_instance,(BACNET_OBJECT_TYPE)object_identifier,1,property_identifier/*PROP_OBJECT_LIST*/);
	}
		
	//Read_Properties(1001,OBJECT_DEVICE,PROP_OBJECT_NAME/*PROP_OBJECT_LIST*/);
}
