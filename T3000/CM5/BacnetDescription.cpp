#include "stdafx.h"
#include "BacnetDescription.h"
BacnetDescription::BacnetDescription(void)
{
    hwait_readdes_thread = NULL;
}

BacnetDescription::~BacnetDescription(void)
{

}


void BacnetDescription::GetAllPanels()
{
	if (hwait_readdes_thread == NULL)
	{
        hwait_readdes_thread = CreateThread(NULL, NULL, Read_BacnetPanelLable_Thread, this, NULL, NULL);
		CloseHandle(hwait_readdes_thread);
	}
}



extern bool sort_by_panelnumber(Bacnet_RemotePoint_Info object1, Bacnet_RemotePoint_Info object2);


DWORD WINAPI  BacnetDescription::Read_BacnetPanelLable_Thread(LPVOID lpVoid)
{
	BacnetDescription* pParent = (BacnetDescription*)lpVoid;
    pParent->m_bacnet_panel.clear();
    Send_WhoIs_Global(-1, -1);
    Sleep(1000);
    int ncount = address_count();
    Str_Setting_Info temp_setting_info;
    temp_setting_info = Device_Basic_Setting; //;
    for (int i = 0; i < ncount; i++)
    {
        unsigned int ndevice_id = 0;
        unsigned int nmax_apdu = 0;
        BACNET_ADDRESS src;
        address_get_by_index(i, &ndevice_id, &nmax_apdu, &src);

        bool find_device = false;
        for (int j = 0; j < pParent->m_bacnet_panel.size(); j++)
        {
            if (pParent->m_bacnet_panel.at(j).ndeviceid == ndevice_id)
            {
                find_device = true;
            }
        }
        if (find_device)
            continue;
        Str_Setting_Info temp_setting_buffer;

        unsigned int t1 = GetTickCount();//(ms);
        // 
        Device_Basic_Setting.reg.object_instance = 0;
        if (GetPrivateData_Blocking(ndevice_id, READ_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), 1) > 0)
        {
            SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Read data success!"));
            temp_setting_buffer = Device_Basic_Setting;
        }
        else
        {
            Sleep(10);
            continue;
        }
        unsigned int t2 = GetTickCount();//(ms);


        CString temp_cs;
        if (temp_setting_buffer.reg.object_instance == ndevice_id)
        {
            TRACE(_T("%d exsit,time %d \n"), ndevice_id, t2 - t1);
            Bacnet_RemotePoint_Info temp_screen;
            temp_screen.ndeviceid = ndevice_id;
            temp_screen.npanelnum = temp_setting_buffer.reg.panel_number;
            pParent->m_bacnet_panel.push_back(temp_screen);
        }
        else
        {
            TRACE(_T("%d not exsit,time %d\n"), ndevice_id, t2 - t1);
        }
        //vector ;
        Sleep(1);
    }
    temp_setting_info = Device_Basic_Setting; //;
    //object instance panel 
    //for (int i = 0; i < pParent->m_bacnet_panel.size(); i++)
    //{
    //    if (pParent->m_bacnet_panel.at(i).npanelnum != 0)
    //        continue;
    //    if (GetPrivateData_Blocking(pParent->m_bacnet_panel.at(i).ndeviceid, READ_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), 1) > 0)
    //    {
    //        if (Device_Basic_Setting.reg.object_instance == pParent->m_bacnet_panel.at(i).ndeviceid)
    //            pParent->m_bacnet_panel.at(i).npanelnum = Device_Basic_Setting.reg.panel_number;
    //        else
    //            pParent->m_bacnet_panel.at(i).npanelnum = 0;
    //    }
    //    Sleep(SEND_COMMAND_DELAY_TIME);
    //}
    sort(pParent->m_bacnet_panel.begin(), pParent->m_bacnet_panel.end(), sort_by_panelnumber);
    pParent->hwait_readdes_thread = NULL;
    //Sleep(15000);
    //panellabeldescription;


	return 1;
}

#if 0
DWORD WINAPI  CBacnetScreenEdit::ReadAllPanelThreadfun(LPVOID lpVoid)
{
    //Write_Config_Info
    CBacnetScreenEdit* pParent = (CBacnetScreenEdit*)lpVoid;
    m_remote_screen_data.clear();
    while (1)
    {
        Send_WhoIs_Global(-1, -1);
        Sleep(1000);
        int ncount = address_count();
        for (int i = 0; i < ncount; i++)
        {
            unsigned int ndevice_id = 0;
            unsigned int nmax_apdu = 0;
            BACNET_ADDRESS src;
            address_get_by_index(i, &ndevice_id, &nmax_apdu, &src);

            bool find_device = false;
            for (int j = 0; j < m_remote_screen_data.size(); j++)
            {
                if (m_remote_screen_data.at(j).ndeviceid == ndevice_id)
                {
                    find_device = true;
                }
            }
            if (find_device)
                continue;


            unsigned int t1 = GetTickCount();//(ms);
            // 
            Device_Basic_Setting.reg.object_instance = 0;
            if (GetPrivateData_Blocking(ndevice_id, READ_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), 1) > 0)
            {
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Read data success!"));
            }
            unsigned int t2 = GetTickCount();//(ms);
            Sleep(1);
            //Device_Basic_Setting.reg

            CString temp_cs;
            if (Device_Basic_Setting.reg.object_instance == ndevice_id)
            {
                TRACE(_T("%d exsit,time %d \n"), ndevice_id, t2 - t1);
                Bacnet_RemotePoint_Info temp_screen;
                temp_screen.ndeviceid = ndevice_id;
                temp_screen.npanelnum = 0;
                m_remote_screen_data.push_back(temp_screen);
            }
            else
            {
                TRACE(_T("%d not exsit,time %d\n"), ndevice_id, t2 - t1);
            }
            //vector ;
            Sleep(1);
        }

        //object instance panel 
        for (int i = 0; i < m_remote_screen_data.size(); i++)
        {
            if (m_remote_screen_data.at(i).npanelnum != 0)
                continue;
            if (GetPrivateData_Blocking(m_remote_screen_data.at(i).ndeviceid, READ_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), 1) > 0)
            {
                if (Device_Basic_Setting.reg.object_instance == m_remote_screen_data.at(i).ndeviceid)
                    m_remote_screen_data.at(i).npanelnum = Device_Basic_Setting.reg.panel_number;
                else
                    m_remote_screen_data.at(i).npanelnum = 0;
            }
            Sleep(SEND_COMMAND_DELAY_TIME);
        }
        sort(m_remote_screen_data.begin(), m_remote_screen_data.end(), sort_by_panelnumber);
        Sleep(15000);
        //panellabeldescription;

        if (ScreenEdit_Window == NULL)
        {
            h_read_all_panel_des_thread = NULL;
            return true;
        }
    }
    return true;
}
#endif