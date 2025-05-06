// BacnetThirdPartyMain.cpp : 
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


// CBacnetThirdPartyMain 

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


// CBacnetThirdPartyMain 

void CBacnetThirdPartyMain::Fresh()
{
    Initial_combo();
    Send_WhoIs_Global(-1, -1);
    Sleep(3000);
    return;
    BACNET_APPLICATION_DATA_VALUE temp_value;
    int nret = Bacnet_Read_Property_Multiple(137445,(BACNET_OBJECT_TYPE)0,2,8);
    if (nret >= 0)
    {
        CFile file;
        CString temp_bacnet_logfile;
        temp_bacnet_logfile = g_achive_folder + _T("\\bacnetlog.txt");
        file.Open(temp_bacnet_logfile, CFile::modeRead, NULL);

        DWORD len = file.GetLength();

        char *Buf = new char[len + 1];

        Buf[len+1] = 0;  //0

        file.Read(Buf, len);   //Read( void* lpBuf, UINT nCount ) lpBufBufnCount

        CString temp_cs;
        MultiByteToWideChar(CP_ACP, 0, (char *)Buf, (int)strlen((char *)Buf) + 1, temp_cs.GetBuffer(len), len);
        temp_cs.ReleaseBuffer();

        MessageBox(temp_cs);
    }
   

}


void CBacnetThirdPartyMain::OnBnClickedButtonThirdOk()
{
    // TODO: 
    object_identifier = ((CComboBox *)GetDlgItem(IDC_COMBO_OBJ_TYPE))->GetCurSel();
    CString temp_cs;
    GetDlgItem(IDC_EDIT_OBJ_INSTANCE)->GetWindowTextW(temp_cs);
    object_instance = _wtoi(temp_cs);
    property_identifier = ((CComboBox *)GetDlgItem(IDC_COMBO_PROPERTY_ID))->GetCurSel();

    GetDlgItem(IDC_EDIT_DEVICE_ID)->GetWindowTextW(temp_cs);
    device_id = _wtoi(temp_cs);

    CString product_name;
    BACNET_APPLICATION_DATA_VALUE temp_value;
    //for (int objId = 0; objId <= OBJECT_LIGHTING_OUTPUT; objId++)
    {
        int invoke_id = Bacnet_Read_Properties_Blocking(g_bac_instance, (BACNET_OBJECT_TYPE)object_identifier, object_instance, (BACNET_PROPERTY_ID)property_identifier, temp_value, 3);
        if (invoke_id >= 0)
        {
            CFile file;
            CString temp_bacnet_logfile;
            temp_bacnet_logfile = g_achive_folder + _T("\\bacnetlog.txt");
            file.Open(temp_bacnet_logfile, CFile::modeRead, NULL);
            DWORD len = file.GetLength();
            char* Buf = new char[len + 1];
            Buf[len + 1] = 0;  //0
            file.Read(Buf, len);   //Read( void* lpBuf, UINT nCount ) lpBufBufnCount
            CString temp_cs;
            MultiByteToWideChar(CP_ACP, 0, (char*)Buf, (int)strlen((char*)Buf) + 1, bacnet_string.GetBuffer(len), len);
            bacnet_string.ReleaseBuffer();
            MessageBox(bacnet_string);
        }
    }
   
}
