// CO2_AUTO_CALIBRATION.cpp : Implementation file
// CO2_AUTO_CALIBRATION.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "T3000.h"
#include "CO2_AUTO_CALIBRATION.h"
#include "afxdialogex.h"
#include "global_function.h"
#include "ShowMessageDlg.h"
// CCO2_AUTO_CALIBRATION dialog
// CCO2_AUTO_CALIBRATION �Ի���

extern int CO2_MODBUS_CO2_BKCAL_ONOFF;
extern int CO2_MODBUS_CO2_NATURE_LEVEL;
extern int CO2_MODBUS_CO2_MIN_ADJ;
extern int CO2_MODBUS_CO2_CAL_DAYS;
extern int CO2_MODBUS_CO2_LOWEST_X_DAYS;
extern int CO2_MODBUS_CO2_FIGURE;
extern int CO2_MODBUS_CO2_USER_ADJ;

IMPLEMENT_DYNAMIC(CCO2_AUTO_CALIBRATION, CDialogEx)

CCO2_AUTO_CALIBRATION::CCO2_AUTO_CALIBRATION(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_FIRMWARE_AUTO_CAL, pParent)
{

}

CCO2_AUTO_CALIBRATION::~CCO2_AUTO_CALIBRATION()
{
}

void CCO2_AUTO_CALIBRATION::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCO2_AUTO_CALIBRATION, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CCO2_AUTO_CALIBRATION::OnBnClickedButton1)
    ON_CBN_SELCHANGE(IDC_COMBO_DLG_CO2_FIRMWARE_AUTO_CAL, &CCO2_AUTO_CALIBRATION::OnCbnSelchangeComboDlgCo2FirmwareAutoCal)
    ON_BN_CLICKED(IDC_BUTTON2, &CCO2_AUTO_CALIBRATION::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON_ABC_HELP, &CCO2_AUTO_CALIBRATION::OnBnClickedButtonAbcHelp)
    ON_BN_CLICKED(IDC_BUTTON_CO2_SCD_HELP, &CCO2_AUTO_CALIBRATION::OnBnClickedButtonCo2ScdHelp)
    ON_BN_CLICKED(IDC_BUTTON_RE_CALIBRATION_SCD_DONE, &CCO2_AUTO_CALIBRATION::OnBnClickedButtonReCalibrationScdDone)
END_MESSAGE_MAP()


// CCO2_AUTO_CALIBRATION message handlers
// CCO2_AUTO_CALIBRATION ��Ϣ�������


void CCO2_AUTO_CALIBRATION::OnBnClickedButton1()
{
    // TODO: Add your control notification handler code here
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    int ret_value = 0;
    CString temp_cs;
    CString temp_cstring;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_DLG_CO2_FIRMWARE_AUTO_CAL))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_DLG_CO2_FIRMWARE_AUTO_CAL))->GetWindowText( temp_cs);

    int temp_value = product_register_value[CO2_MODBUS_CO2_BKCAL_ONOFF];
    for (int i = 0;i<sizeof(CO2_Node_Auto_Cal) / sizeof(CO2_Node_Auto_Cal[0]);i++)
    {
        if (temp_cs.CompareNoCase(CO2_Node_Auto_Cal[i]) == 0)
        {
            temp_value = i;
            break;
        }
    }
    int nret = write_one(g_tstat_id, CO2_MODBUS_CO2_BKCAL_ONOFF, temp_value);
    if (nret <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change status failed!"));
        goto write_failed;
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change status OK!"));
    }


    GetDlgItem(IDC_EDIT_DLG_FIRMWARE_NATURE_CO2)->GetWindowTextW(temp_cstring);
    int temp_int;
    temp_int = _wtoi(temp_cstring);
    if ((temp_int < 390) || (temp_int > 500))
    {
        MessageBox(_T("Nature's CO2 ppm should be in this range (390 - 500)"));
        return;
    }
    nret = write_one(g_tstat_id, CO2_MODBUS_CO2_NATURE_LEVEL, temp_int);
    if (nret <= 0)
    {
        goto write_failed;
    }


    GetDlgItem(IDC_EDIT_DLG_FIRMWARE_MAX_MIN_ADJ_PERDAY)->GetWindowTextW(temp_cstring);
    temp_int = _wtoi(temp_cstring);
    if ((temp_int < 1) || (temp_int > 10))
    {
        MessageBox(_T("Maximum adjustment per day should be in this range (1 - 10)"));
        return;
    }

    nret = write_one(g_tstat_id, CO2_MODBUS_CO2_MIN_ADJ, temp_int);
    if (nret <= 0)
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Background CO2 concentration failed!"));
       goto write_failed;

    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change Background CO2 concentration success!"));
    }

    GetDlgItem(IDC_EDIT_DLG_FIRMWARE_LOOK_DAYS)->GetWindowTextW(temp_cstring);
    temp_int = _wtoi(temp_cstring);
    if ((temp_int < 7) || (temp_int > 30))
    {
        MessageBox(_T("Watch days for lowest PPM reading should be in this range (7 - 30)"));
        return;
    }

    nret = write_one(g_tstat_id, CO2_MODBUS_CO2_CAL_DAYS, temp_int);
    if (nret <= 0)
    {
       goto write_failed;
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Operation Done!"));
    }

#if 0
    GetDlgItem(IDC_EDIT_LOWEST_X_DAYS)->GetWindowTextW(temp_cstring);
    temp_int = _wtoi(temp_cstring);
    //if ((temp_int < 7) || (temp_int > 30))
    //{
    //    MessageBox(_T("Maximum adjustment per day should be in this range (7 - 30)"));
    //    return;
    //}

    nret = write_one(g_tstat_id, CO2_MODBUS_CO2_LOWEST_X_DAYS, temp_int);
    if (nret <= 0)
    {
        goto write_failed;
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change current lowest value last x days success!"));
    }



    GetDlgItem(IDC_EDIT_FIGURE)->GetWindowTextW(temp_cstring);
    temp_int = _wtoi(temp_cstring);
    //if ((temp_int < 7) || (temp_int > 30))
    //{
    //    MessageBox(_T("Maximum adjustment per day should be in this range (7 - 30)"));
    //    return;
    //}

    nret = write_one(g_tstat_id, CO2_MODBUS_CO2_FIGURE, temp_int);
    if (nret <= 0)
    {
        goto write_failed;
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change current  automatic calibration figure success!"));
    }
#endif


    GetDlgItem(IDC_EDIT_USER_ADJ)->GetWindowTextW(temp_cstring);
    temp_int = _wtoi(temp_cstring);
    //if ((temp_int < 7) || (temp_int > 30))
    //{
    //    MessageBox(_T("Maximum adjustment per day should be in this range (7 - 30)"));
    //    return;
    //}

    nret = write_one(g_tstat_id, CO2_MODBUS_CO2_USER_ADJ, temp_int);
    if (nret <= 0)
    {
        goto write_failed;
    }
    else
    {
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Change user  adjustment success!"));
    }



    MessageBox(_T("Operation Done!"));
    return;

write_failed:
    MessageBox(_T("Write data timeout"));
    return;

}


BOOL CCO2_AUTO_CALIBRATION::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    GetDlgItem(IDC_EDIT_CO2_CALIBRATION_SCD)->SetWindowText(_T("0"));


    if ((product_register_value[7] == STM32_CO2_NET) ||
        (product_register_value[7] == STM32_CO2_RS485))
    {
        CO2_MODBUS_CO2_BKCAL_ONOFF = 3160;
        CO2_MODBUS_CO2_NATURE_LEVEL = 3161;
        CO2_MODBUS_CO2_MIN_ADJ = 3162;
        CO2_MODBUS_CO2_CAL_DAYS = 3163;
        CO2_MODBUS_CO2_LOWEST_X_DAYS = 3166;
        CO2_MODBUS_CO2_FIGURE = 3165;
        CO2_MODBUS_CO2_USER_ADJ = 212;
        int nread_ret = Read_Multi(g_tstat_id, &product_register_value[3100], 3100, 100, 5);
        if (nread_ret < 0)
        {
            MessageBox(_T("Read data timeout , please try again."));
            PostMessage(WM_CLOSE, NULL, NULL);
        }
        Sleep(300);
        nread_ret = Read_Multi(g_tstat_id, &product_register_value[CO2_MODBUS_CO2_USER_ADJ], CO2_MODBUS_CO2_USER_ADJ, 1, 5);
        if (nread_ret < 0)
        {
            MessageBox(_T("Read data timeout , please try again."));
            PostMessage(WM_CLOSE, NULL, NULL);
        }
    }
    else if ((product_register_value[7] == PM_TSTAT_AQ) ||
             (product_register_value[7] == PM_AIRLAB_ESP32))
    {
        CO2_MODBUS_CO2_BKCAL_ONOFF = 1960;
        CO2_MODBUS_CO2_NATURE_LEVEL = 1961;
        CO2_MODBUS_CO2_MIN_ADJ = 1962;
        CO2_MODBUS_CO2_CAL_DAYS = 1963;
        CO2_MODBUS_CO2_LOWEST_X_DAYS = 1966;
        CO2_MODBUS_CO2_FIGURE = 1965;
        CO2_MODBUS_CO2_USER_ADJ = 612;
        int nread_ret = Read_Multi(g_tstat_id, &product_register_value[1900], 1900, 100, 5);
        if (nread_ret < 0)
        {
            MessageBox(_T("Read data timeout , please try again."));
            PostMessage(WM_CLOSE, NULL, NULL);
        }
        Sleep(300);
        nread_ret = Read_Multi(g_tstat_id, &product_register_value[CO2_MODBUS_CO2_USER_ADJ], CO2_MODBUS_CO2_USER_ADJ, 1, 5);
        if (nread_ret < 0)
        {
            MessageBox(_T("Read data timeout , please try again."));
            PostMessage(WM_CLOSE, NULL, NULL);
        }
    }
    else
    {
        return 1;
    }
    // TODO: Add extra initialization here
    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    ((CComboBox *)GetDlgItem(IDC_COMBO_DLG_CO2_FIRMWARE_AUTO_CAL))->ResetContent();
    for (int i = 0; i < (sizeof(CO2_Node_Auto_Cal) / sizeof(CO2_Node_Auto_Cal[0])); i++)
    {
        ((CComboBox *)GetDlgItem(IDC_COMBO_DLG_CO2_FIRMWARE_AUTO_CAL))->AddString(CO2_Node_Auto_Cal[i]);
    }

    CString cs_temp;
    cs_temp.Format(_T("%d"), product_register_value[CO2_MODBUS_CO2_NATURE_LEVEL]);
    GetDlgItem(IDC_EDIT_DLG_FIRMWARE_NATURE_CO2)->SetWindowText(cs_temp);

    cs_temp.Format(_T("%d"), product_register_value[CO2_MODBUS_CO2_MIN_ADJ]);
    GetDlgItem(IDC_EDIT_DLG_FIRMWARE_MAX_MIN_ADJ_PERDAY)->SetWindowText(cs_temp);

    cs_temp.Format(_T("%d"), product_register_value[CO2_MODBUS_CO2_CAL_DAYS]);
    GetDlgItem(IDC_EDIT_DLG_FIRMWARE_LOOK_DAYS)->SetWindowText(cs_temp);


    cs_temp.Format(_T("%d"), product_register_value[CO2_MODBUS_CO2_LOWEST_X_DAYS]);
    GetDlgItem(IDC_EDIT_LOWEST_X_DAYS)->SetWindowText(cs_temp);

    cs_temp.Format(_T("%d"), (short)product_register_value[CO2_MODBUS_CO2_FIGURE]);
    GetDlgItem(IDC_EDIT_FIGURE)->SetWindowText(cs_temp);

    cs_temp.Format(_T("%d"), (short)product_register_value[CO2_MODBUS_CO2_USER_ADJ]);
    GetDlgItem(IDC_EDIT_USER_ADJ)->SetWindowText(cs_temp);



    if (product_register_value[CO2_MODBUS_CO2_BKCAL_ONOFF] <= (sizeof(CO2_Node_Auto_Cal) / sizeof(CO2_Node_Auto_Cal[0])))
        cs_temp = CO2_Node_Auto_Cal[product_register_value[CO2_MODBUS_CO2_BKCAL_ONOFF]];
    else
        cs_temp.Empty();
    ((CComboBox *)GetDlgItem(IDC_COMBO_DLG_CO2_FIRMWARE_AUTO_CAL))->SetWindowTextW(cs_temp);

    if (product_register_value[CO2_MODBUS_CO2_BKCAL_ONOFF] == 0)
    {
        GetDlgItem(IDC_EDIT_DLG_FIRMWARE_NATURE_CO2)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_DLG_FIRMWARE_MAX_MIN_ADJ_PERDAY)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_DLG_FIRMWARE_LOOK_DAYS)->EnableWindow(false);
        GetDlgItem(IDC_EDIT_USER_ADJ)->EnableWindow(false);

        if ((product_register_value[CO2_MODBUS_CO2_NATURE_LEVEL] < 390) || (product_register_value[CO2_MODBUS_CO2_NATURE_LEVEL] > 500))
        {
            GetDlgItem(IDC_EDIT_DLG_FIRMWARE_NATURE_CO2)->SetWindowText(_T("400"));
        }

        if ((product_register_value[CO2_MODBUS_CO2_MIN_ADJ] < 1) || (product_register_value[CO2_MODBUS_CO2_MIN_ADJ] > 10))
        {
            GetDlgItem(IDC_EDIT_DLG_FIRMWARE_MAX_MIN_ADJ_PERDAY)->SetWindowText(_T("1"));
        }

        if ((product_register_value[CO2_MODBUS_CO2_CAL_DAYS] < 7) || (product_register_value[CO2_MODBUS_CO2_CAL_DAYS] > 30))
        {
            GetDlgItem(IDC_EDIT_DLG_FIRMWARE_LOOK_DAYS)->SetWindowText(_T("7"));
        }
        


    }
    else
    {
        GetDlgItem(IDC_EDIT_DLG_FIRMWARE_NATURE_CO2)->EnableWindow(true);
        GetDlgItem(IDC_EDIT_DLG_FIRMWARE_MAX_MIN_ADJ_PERDAY)->EnableWindow(true);
        GetDlgItem(IDC_EDIT_DLG_FIRMWARE_LOOK_DAYS)->EnableWindow(true);
        GetDlgItem(IDC_EDIT_USER_ADJ)->EnableWindow(true);
    }
    GetDlgItem(IDC_EDIT_LOWEST_X_DAYS)->EnableWindow(false);
    GetDlgItem(IDC_EDIT_FIGURE)->EnableWindow(false);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // Exception: OCX property pages should return FALSE
                  // �쳣: OCX ����ҳӦ���� FALSE
}


void CCO2_AUTO_CALIBRATION::OnCbnSelchangeComboDlgCo2FirmwareAutoCal()
{
    // TODO: Add your control notification handler code here
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString temp_string;
    int nSel = ((CComboBox *)GetDlgItem(IDC_COMBO_DLG_CO2_FIRMWARE_AUTO_CAL))->GetCurSel();
    ((CComboBox *)GetDlgItem(IDC_COMBO_DLG_CO2_FIRMWARE_AUTO_CAL))->GetLBText(nSel, temp_string);
    if (temp_string.CompareNoCase(CO2_Node_Auto_Cal[0]) == 0)
    {
            GetDlgItem(IDC_EDIT_DLG_FIRMWARE_NATURE_CO2)->EnableWindow(false);
            GetDlgItem(IDC_EDIT_DLG_FIRMWARE_MAX_MIN_ADJ_PERDAY)->EnableWindow(false);
            GetDlgItem(IDC_EDIT_DLG_FIRMWARE_LOOK_DAYS)->EnableWindow(false);
            GetDlgItem(IDC_EDIT_USER_ADJ)->EnableWindow(false);
    }
    else
    {
            GetDlgItem(IDC_EDIT_DLG_FIRMWARE_NATURE_CO2)->EnableWindow(true);
            GetDlgItem(IDC_EDIT_DLG_FIRMWARE_MAX_MIN_ADJ_PERDAY)->EnableWindow(true);
            GetDlgItem(IDC_EDIT_DLG_FIRMWARE_LOOK_DAYS)->EnableWindow(true);
            GetDlgItem(IDC_EDIT_USER_ADJ)->EnableWindow(true);
    }
}


void CCO2_AUTO_CALIBRATION::OnBnClickedButton2()
{
    // TODO: Add your control notification handler code here
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    PostMessage(WM_CLOSE, NULL, NULL);
}


void CCO2_AUTO_CALIBRATION::OnBnClickedButtonAbcHelp()
{
    // TODO: Add your control notification handler code here
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString temp_ABC_Info;
    temp_ABC_Info.Format(_T("The Co2 sensor will watch for the lowest concentration measured over the previous week, this is assumed to be the natural background C02 concentration on the earth of fresh air, currently this is 400ppm. If the sensor has been over or below 400 ppm a small nudge factor is added or subtracted from the actual reading to self compensate for variation over time of the sensing element and infrared light source. You can disable this feature if the building isn��t hitting 400 ppm regularly during a given week. "));
    MessageBox(temp_ABC_Info);
}


void CCO2_AUTO_CALIBRATION::OnBnClickedButtonCo2ScdHelp()
{
    // TODO: Add your control notification handler code here
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CShowMessageDlg dlg;
    CString temp_message;
    temp_message.Format(_T("Sensirion CO2 sensor RE-CALIBRATION \r\nLocate the device in an environment with air having a stable CO2 concentration in the range of 400 ppm to 2000 ppm. \r\n \
1.Setting and controlling a known CO2 concentration in a sealed environment with the set CO2concentration acting as the reference value for FRC .\r\n \
2.Fresh air from the outside can be used as a reference. Outside air typically has a CO2 concentration of 400 ppm . expose the device to outside air, e.g. by placing it close to an open window or outside. Direct sun light, extreme temperatures,and strong air flow have to be prevented,After 5 minutes, apply FRC with the reference value 400 ppm ."));
    dlg.SetStaticText(temp_message);
    //dlg.SetStaticTextBackgroundColor(RGB(222, 222, 222));
    dlg.SetStaticTextColor(RGB(0, 0, 255));
    dlg.SetStaticTextSize(18, 14);
    dlg.SetEvent(EVENT_MESSAGE_ONLY);
    dlg.DoModal();
}


void CCO2_AUTO_CALIBRATION::OnBnClickedButtonReCalibrationScdDone()
{
    // TODO: Add your control notification handler code here
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString strCalibration;
    GetDlgItem(IDC_EDIT_CO2_CALIBRATION_SCD)->GetWindowText(strCalibration);

    unsigned short IntCalValue = _wtoi(strCalibration);
    if (IntCalValue >= 5000)
    {
        MessageBox(_T("Out of range . Calibration value range (0-4999)"));
        return;
    }

    CString cs_message;
    cs_message.Format(_T("Are you sure you want to write this calibration value (%d)? The CO2 sensor is in a %d PPM environment"), IntCalValue, IntCalValue);
    if (IDYES == MessageBox(cs_message, _T("Message"), MB_YESNOCANCEL))
    {
        if (write_one(g_tstat_id, 1951, IntCalValue, 3) < 0)
        {
            MessageBox(_T("Write data timeout,Please try again"));
            return;
        }
        else
        {
            MessageBox(_T("Write data success!"));
        }
    }


}
