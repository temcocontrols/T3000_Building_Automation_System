// BacnetThirdPartyMain.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetThirdPartyMain.h"
#include "global_function.h"
#include "global_variable_extern.h"

// CBacnetThirdPartyMain

IMPLEMENT_DYNCREATE(CBacnetThirdPartyMain, CFormView)

CBacnetThirdPartyMain::CBacnetThirdPartyMain()
	: CFormView(IDD_DIALOG_BACNET_THIRD_PARTY_MAIN)
{

}

CBacnetThirdPartyMain::~CBacnetThirdPartyMain()
{
}

void CBacnetThirdPartyMain::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBacnetThirdPartyMain, CFormView)
    ON_BN_CLICKED(IDC_BUTTON_THIRD_OK, &CBacnetThirdPartyMain::OnBnClickedButtonThirdOk)
END_MESSAGE_MAP()


// CBacnetThirdPartyMain 诊断

#ifdef _DEBUG
void CBacnetThirdPartyMain::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CBacnetThirdPartyMain::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG



void CBacnetThirdPartyMain::Initial_combo()
{
    //m_combo_service_choice.AddString(_T("Read Property (12)"));
    //m_combo_service_choice.SetCurSel(0);
    //GetDlgItem(IDC_TOOL_NETWORK_EDIT)->SetWindowTextW(_T("0"));

    const char * temp = NULL;

    CString Add_CString;
    char temp_char[100];
    CString temp_1;

    CString temp_device_id;
    temp_device_id.Format(_T("%u"), g_bac_instance);
    GetDlgItem(IDC_EDIT_DEVICE_ID)->SetWindowTextW(temp_device_id);

    ((CComboBox *)GetDlgItem(IDC_COMBO_OBJ_TYPE))->ResetContent();
    ((CComboBox *)GetDlgItem(IDC_COMBO_PROPERTY_ID))->ResetContent();
    for (int i = 0;i <= OBJECT_LIGHTING_OUTPUT;i++)
    {
        temp_1.Empty();
        temp_1.Format(_T("( %d )"), i);
        Add_CString.Empty();
        memset(temp_char, 0, sizeof(temp_char));
        temp = bactext_object_type_name(i);
        strcpy_s(temp_char, 100, temp);
        ::MultiByteToWideChar(CP_ACP, 0, temp_char, strlen(temp_char) + 1, Add_CString.GetBuffer(MAX_PATH), MAX_PATH);
        Add_CString.ReleaseBuffer();
        Add_CString = Add_CString + temp_1;
        
        ((CComboBox *)GetDlgItem(IDC_COMBO_OBJ_TYPE))->AddString(Add_CString);
        ((CComboBox *)GetDlgItem(IDC_COMBO_OBJ_TYPE))->SetCurSel(OBJECT_DEVICE);
    }

    for (int i = 0;i <= PROP_EGRESS_ACTIVE;i++)
    {
        temp_1.Empty();
        temp_1.Format(_T("( %d )"), i);
        Add_CString.Empty();
        memset(temp_char, 0, sizeof(temp_char));
        temp = bactext_property_name(i);
        strcpy_s(temp_char, 100, temp);
        ::MultiByteToWideChar(CP_ACP, 0, temp_char, strlen(temp_char) + 1, Add_CString.GetBuffer(MAX_PATH), MAX_PATH);
        Add_CString.ReleaseBuffer();
        Add_CString = Add_CString + temp_1;
        ((CComboBox *)GetDlgItem(IDC_COMBO_PROPERTY_ID))->AddString(Add_CString);
        ((CComboBox *)GetDlgItem(IDC_COMBO_PROPERTY_ID))->SetCurSel(PROP_ALL);
    }

}


// CBacnetThirdPartyMain 消息处理程序

void CBacnetThirdPartyMain::Fresh()
{
    Initial_combo();
    Send_WhoIs_Global(-1, -1);
    Sleep(3000);
    return;
    BACNET_APPLICATION_DATA_VALUE temp_value;
    //int nret = Bacnet_Read_Properties_Blocking(3000, OBJECT_DEVICE, 3000, 76, temp_value, 4);
    int nret = Bacnet_Read_Property_Multiple(137445,(BACNET_OBJECT_TYPE)0,2,8);
    if (nret >= 0)
    {
        CFile file;
        CString temp_bacnet_logfile;
        temp_bacnet_logfile = g_achive_folder + _T("\\bacnetlog.txt");
        file.Open(temp_bacnet_logfile, CFile::modeRead, NULL);

        DWORD len = file.GetLength();

        char *Buf = new char[len + 1];

        Buf[len+1] = 0;  //0终止字符串，用于输出。

        file.Read(Buf, len);   //Read( void* lpBuf, UINT nCount ) lpBuf是用于接收读取到的数据的Buf指针nCount是从文件读取的字节数

        CString temp_cs;
        MultiByteToWideChar(CP_ACP, 0, (char *)Buf, (int)strlen((char *)Buf) + 1, temp_cs.GetBuffer(len), len);
        temp_cs.ReleaseBuffer();

        MessageBox(temp_cs);
    }
    //CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_AQ_TEMPERATURE); // 得到 Picture Control 句柄 ;
    //CString icon_temperature;


    //CString ApplicationFolder;
    //GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
    //PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
    //ApplicationFolder.ReleaseBuffer();
    //AQ_image_fordor = ApplicationFolder + _T("\\ResourceFile");
    //icon_temperature = AQ_image_fordor + _T("\\temperature.bmp");
    //HBITMAP bitmap;
    //bitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(), icon_temperature, IMAGE_BITMAP, 40, 120, LR_LOADFROMFILE);

    //CStatic *p = (CStatic *)GetDlgItem(IDC_STATIC_AQ_TEMPERATURE);
    ////设置静态控件窗口风格为位图居中显示  
    //p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
    ////将图片设置到Picture控件上  
    //p->SetBitmap(bitmap);





    //CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
    //pFrame->SetWindowTextW(_T("T3000 Building Automation System") + CurrentT3000Version);
    //if (h_tstat_aq_thread == NULL)
    //    h_tstat_aq_thread = CreateThread(NULL, NULL, Update_TstatAQ_Thread, this, NULL, NULL);
    //UpdateUI();


    //CString sound_full_path;
    //CStatic* pWnd_sound_pic = (CStatic*)GetDlgItem(IDC_STATIC_SOUND); // 得到 Picture Control 句柄 ;
    //if (product_register_value[TSTAT_AQ_SOUND] <= 10)
    //    sound_full_path = ApplicationFolder + _T("\\ResourceFile\\Icon\\sound_0.ico");
    //else if (product_register_value[TSTAT_AQ_SOUND] <= 30)
    //    sound_full_path = ApplicationFolder + _T("\\ResourceFile\\Icon\\sound_1.ico");
    //else if (product_register_value[TSTAT_AQ_SOUND] <= 50)
    //    sound_full_path = ApplicationFolder + _T("\\ResourceFile\\Icon\\sound_2.ico");
    //else
    //    sound_full_path = ApplicationFolder + _T("\\ResourceFile\\Icon\\sound_3.ico");
    //Bitmap icon_bitmap(sound_full_path);
    //HICON m_hIcon = ExtractIcon(AfxGetInstanceHandle(), sound_full_path, 0);
    //pWnd_sound_pic->ModifyStyle(0, SS_ICON | SS_CENTERIMAGE);
    //pWnd_sound_pic->SetIcon(m_hIcon);



}


void CBacnetThirdPartyMain::OnBnClickedButtonThirdOk()
{
    // TODO: 在此添加控件通知处理程序代码
    object_identifier = ((CComboBox *)GetDlgItem(IDC_COMBO_OBJ_TYPE))->GetCurSel();
    CString temp_cs;
    GetDlgItem(IDC_EDIT_OBJ_INSTANCE)->GetWindowTextW(temp_cs);
    object_instance = _wtoi(temp_cs);
    property_identifier = ((CComboBox *)GetDlgItem(IDC_COMBO_PROPERTY_ID))->GetCurSel();

    GetDlgItem(IDC_EDIT_DEVICE_ID)->GetWindowTextW(temp_cs);
    device_id = _wtoi(temp_cs);
    

    int nret = Bacnet_Read_Property_Multiple(device_id, (BACNET_OBJECT_TYPE)object_identifier, object_instance, property_identifier);
    if (nret >= 0)
    {
        CFile file;
        CString temp_bacnet_logfile;
        temp_bacnet_logfile = g_achive_folder + _T("\\bacnetlog.txt");
        file.Open(temp_bacnet_logfile, CFile::modeRead, NULL);
        DWORD len = file.GetLength();
        char *Buf = new char[len + 1];
        Buf[len + 1] = 0;  //0终止字符串，用于输出。
        file.Read(Buf, len);   //Read( void* lpBuf, UINT nCount ) lpBuf是用于接收读取到的数据的Buf指针nCount是从文件读取的字节数
        CString temp_cs;
        MultiByteToWideChar(CP_ACP, 0, (char *)Buf, (int)strlen((char *)Buf) + 1, temp_cs.GetBuffer(len), len);
        temp_cs.ReleaseBuffer();
        MessageBox(temp_cs);
    }
}
