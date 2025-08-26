#include "StdAfx.h"
#include "ComWriter.h"
#include "ISPDlg.h"
#include "BacnetMstp.h"
extern vector <_Bac_Scan_Com_Info> m_bac_handle_Iam_data;
extern unsigned int g_mstp_deviceid ;
extern int SPECIAL_BAC_TO_MODBUS ;
// If equal to 1, it means that the new BootLoader is being flashed now
extern int new_bootload ; //�������1 ��˵��������д�����µ�BootLoader;
// 0 normal mode, 1 flash boot mode
extern int com_port_flash_status ;  // 0 ����ģʽ   1 ��дbootģʽ
extern unsigned int n_check_temco_firmware;
extern bool auto_flash_mode;
extern CString g_strFlashInfo;
 extern Bin_Info        global_fileInfor;
extern unsigned int the_max_register_number_parameter_Count;
extern unsigned int the_max_register_number_parameter_Finished;
extern unsigned int com_error_delay_time ;
extern unsigned int com_error_delay_count ;
unsigned short Device_infor[18] = { 0 };
void close_bac_com();
#ifdef ISP_BURNING_MODE
extern  unsigned short Check_sum ;
extern int temco_burning_mode;
#endif
UINT Flash_Modebus_Device(LPVOID pParam);
UINT flashThread_ForExtendFormatHexfile(LPVOID pParam);
UINT flashThread_ForExtendFormatHexfile_RAM(LPVOID pParam);
int flash_a_tstat_RAM(BYTE m_ID,int section, unsigned int the_max_register_number_parameter, TS_UC *register_data_orginal, LPVOID pParam);
int flash_a_tstat(BYTE m_ID, unsigned int the_max_register_number_parameter, TS_UC *register_data_orginal, LPVOID pParam);
// 0 no need to update boot, 1 need to update bootloader; C1 is hex
extern int firmware_must_use_new_bootloader ;  //0 ���ø���boot   1 ��Ҫ����bootload;   C1Ϊhex
extern CString g_repair_bootloader_file_path;

CComWriter::CComWriter(void)
{
    m_pFileBuffer = NULL;
    m_pFileRepairBuffer = NULL;
    m_nBufLen = 0;

    m_nComPort = -1;
    //m_nModbusID;
    // Baud rate
    m_nBautrate = 0;			// ������ - Baud rate
    m_com_flash_binfile = 0;
    m_pWorkThread = NULL;
    continue_com_flash_count = 0;
    m_bStopWrite = FALSE;
    m_subnet_flash = 0;
    m_nHexFileType = HEXFILE_DATA;
}

CComWriter::~CComWriter(void)
{

}


void CComWriter::SetParentWnd(CWnd* pWnd)
{
    m_pParentWnd = pWnd;
}

void CComWriter::SetFileBuffer(TS_UC* pBuf, int nLen)
{
    m_pFileBuffer = pBuf;
    m_nBufLen = nLen;
}

void CComWriter::SetModbusID(vector<int>& szMdbIds)
{
    m_szMdbIDs = szMdbIds;
}

void CComWriter::SetComPortNO(int nComPortNO)
{
    m_nComPort = nComPortNO;
}

void CComWriter::SetBautrate(int nBautrate)
{
    m_nBautrate = nBautrate;
}

void CComWriter::SetExtendHexFileParam(vector<int>& szFlags, TS_UC* pBuf)
{
    m_szHexFileFlags.clear();
    m_szHexFileFlags = szFlags;

    m_pExtendFileBuffer = pBuf;
}


int CComWriter::BeginWirteByCom()
{
    ASSERT(m_szMdbIDs.size() > 0);
    ASSERT(m_nComPort >= 0);

    if (m_nHexFileType == 0)
    {
// 		if (!WriteCommandtoReset())
// 		{
// 			return 0;
// 		}


        HCURSOR hc;//load mouse cursor
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);

        if(open_com(m_nComPort)==false)
        {
            CString srtInfo;
            srtInfo = _T("|Error :The com port is occupied!");
            //MessageBox(NULL, srtInfo, _T("ISP"), MB_OK);
            //AddStringToOutPuts(_T("Error :The com port is occupied!"));
            OutPutsStatusInfo(srtInfo, FALSE);
            srtInfo = _T("You need to shut down all other applications and ");
            OutPutsStatusInfo(srtInfo, FALSE);
            srtInfo = _T("background tasks that might be occupying this COM port!");
            OutPutsStatusInfo(srtInfo, FALSE);
            return 0;
        }
        else
        {
            CString strTemp;
            strTemp.Format(_T("COM%d"), m_nComPort);
            CString strTips;
            strTips.Format(_T("|Open %s successful."), strTemp);
            OutPutsStatusInfo(strTips, FALSE);
            Change_BaudRate(m_nBautrate);

        }


        CString strTips;
        strTips = _T("|Programming device...");
        OutPutsStatusInfo(strTips);
        //AddStringToOutPuts(strTips);
        //m_pWorkThread=AfxBeginThread(run_back_ground_flash_thread, this); //create thread,read information
        m_pWorkThread=AfxBeginThread(Flash_Modebus_Device, this); //create thread,read information

        ASSERT(m_pWorkThread);
    }

    else if(m_nHexFileType == 2)
    {
#pragma region detect_mstp_shutdown
		// Don't judge the serial port protocol when flashing for now, otherwise it will be slow when burning modbus
		if (SPECIAL_BAC_TO_MODBUS)  //��ʱ��Ҫ�ڵ�flash��ʱ�� �жϴ��ڵ�Э�飬��Ȼ��modbus��ʱ����� ��
		{
			// Used to detect serial port MSTP data
			baudrate_def temp_baudrate_ret[100] = { 0 }; //���ڼ�⴮��MSTP����
			int find_mstp_protocal = 0;
			find_mstp_protocal = Check_Mstp_Comport(m_nComPort, temp_baudrate_ret, m_nBautrate);
			if ((find_mstp_protocal == -101) || (find_mstp_protocal == -100))
			{
				CString srtInfo = _T("|Error :The com port is occupied!");
				OutPutsStatusInfo(srtInfo, FALSE);
				return 0;
			}

			if (find_mstp_protocal != 1)
			{
				CString srtInfo = _T("|Error :No MSTP data was found on the RS485 serial bus!");
				OutPutsStatusInfo(srtInfo, FALSE);
				return 0;
			}
		}
#if 0
        int temp_loop_count = 0;
        while ( (find_mstp_protocal > 0) && (temp_loop_count < 3))
        {
            CString srtInfo;
            srtInfo.Format(_T("The current RS485 port uses Bacnet MSTP. (%d)"), temp_loop_count);
            OutPutsStatusInfo(srtInfo, FALSE);

            // Initialize bacnet mstp protocol
            int ret = Initial_bac(m_nComPort, _T(""), m_nBautrate); //��ʼ��bacnet mstp Э��
            CString temp_cs;
            if (ret <= 0)
            {
                temp_cs.Format(_T("Failed to open serial port :%d"), m_nComPort);
                OutPutsStatusInfo(temp_cs, FALSE);
                WriteFinish(0);
                return 0;
            }
            else
            {
                break;
            }
            //Send_WhoIs_Global(-1, -1);
// Don't use mute for now
#if 0  //�Ȳ����ñ���
            srtInfo = _T("Disabling Bacnet MSTP and switching over to Modbus");
            OutPutsStatusInfo(srtInfo, FALSE);
            int nret = 0;
            int loop1_count = 0;
            do
            {
                // Make Temco's bacnet devices on the bus silent; since it's a broadcast, send it in a loop
                nret = ShutDownMstpGlobal(4);  //�������� Temco�� bacnet �豸  ����; ��Ϊ�ǹ㲥 �� ����ѭ����
                Sleep(2000);
                loop1_count++;
            } while ((nret <= 0) && loop1_count < 5);
            // Make Temco's bacnet devices on the bus silent; since it's a broadcast, send it in a loop
            nret = ShutDownMstpGlobal(4);  //�������� Temco�� bacnet �豸  ����; ��Ϊ�ǹ㲥 �� ����ѭ����
            Sleep(1000);

            close_bac_com();
            find_mstp_protocal = Check_Mstp_Comport(m_nComPort, &temp_baudrate_ret, m_nBautrate);  //�ٴ�ȷ�������� �������� - Confirm that all devices on the bus are silent again
            temp_loop_count++; //���ѭ��3�� - Loop up to 3 times
#endif
        }


        if (find_mstp_protocal > 0)
        {
            CString temp_cs = _T("Some devices on the RS485 bus are running Bacnet MSTP");
            OutPutsStatusInfo(temp_cs, FALSE);
            temp_cs = _T("Remove these devices and try again.");
            OutPutsStatusInfo(temp_cs, FALSE);
            temp_cs = _T("Update the firmware failed.");
            OutPutsStatusInfo(temp_cs, FALSE);
            return 0;
        }
#endif
#pragma endregion



        if (Is_Ram)
        {
            WirteExtendHexFileByCom_RAM();
        }
        else
        {
            WirteExtendHexFileByCom();
        }
    }

    return 1;
}




BOOL CComWriter::WriteCommandtoReset()
{
    //if(open_com_for_Command(m_nComPort)==false)
    CString strTips ;//= _T("|Error :The com port is occupied!");
    if(open_com(m_nComPort)==false)
    {
        strTips = _T("|Error :The com port is occupied!");
        OutPutsStatusInfo(strTips);
        strTips = _T("You need to shut down all other applications and");
        OutPutsStatusInfo(strTips, FALSE);
        strTips = _T("background tasks that might be occupying this COM port!");
        OutPutsStatusInfo(strTips, FALSE);
        return FALSE;
    }

    int nRet = Write_One(m_szMdbIDs[0],16,127);   // Enter ISP mode
    //Sleep(2000);
    //Add by Fance  �����Ӧ�ô������� ISP  16д127��  ��Ҫ�� 11�żĴ���  11�� ����1  ˵����ת�ɹ�����������ȴ�
    //TBD: Explain this comment better
    // If you want to read 11th register 11th or more than 1 when you jump from the application code to the ISP 16 write 127, the jump succeeds, otherwise wait
     
    strTips = _T("Wait device jump to isp mode!");
    OutPutsStatusInfo(strTips);
    int re_count = 0;
    int nnn_ret = 0;

    do
    {
        nnn_ret  = read_one(m_szMdbIDs[0],11);
        if(nnn_ret > 1)
            break;
        Sleep(1000);
        re_count ++;
        if(re_count == 15)
            break;
    }
    while (nnn_ret > 1);


    int ModelID= read_one(m_szMdbIDs[0],7,5);
    if (ModelID>0)
    {
        if (ModelID==6||ModelID==7||ModelID==8)//Tstat6,7,8 Detecting chip flash size��        {
        {
            int Chipsize=read_one(m_szMdbIDs[0],11,5);


            if (Chipsize<37)	//64K
            {
                if (m_nHexFileType==0)
                {
                    strTips = _T("|hex file flash size matches with the device CPU!");
                    OutPutsStatusInfo(strTips);
                }
                else
                {
                    int ii=0;
                    while(ii<=5)
                    {
                        int ret=Write_One(m_szMdbIDs[0],16,1);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                    }
                    strTips = _T("|hex file doesn't match the device CPU!");
                    OutPutsStatusInfo(strTips);
                    return FALSE;
                }
            }
            else	//128K
            {
                if (m_nHexFileType==0)
                {
                    int ii=0;
                    while(ii<=5)
                    {
                        int ret=Write_One(m_szMdbIDs[0],33,1);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                    }
                    ii=0;
                    while(ii<=5)
                    {
                        int ret=Write_One(m_szMdbIDs[0],16,1);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                    }
                    strTips = _T("|hex file doesn't match the device CPU!");
                    OutPutsStatusInfo(strTips);
                    return FALSE;
                }
                else
                {
                    strTips = _T("|hex file   matches with the device!");
                    OutPutsStatusInfo(strTips);
                }
            }
        }
    }




    close_com();
    //Sleep(1000);

    return TRUE;
}



UINT Flash_Modebus_Device(LPVOID pParam)
{
    CComWriter* pWriter = (CComWriter*)(pParam);
    CString strFailureList;
    CString strTips;
    //	do
    //	{
    int nFlashRet=0;
    UINT i=0;
    int nFailureNum = 0;
    CString strTips_isp1 = _T("Wait for the device jump to update mode.");

    if(GetCommunicationType()==1)
    {
        if(Open_Socket2(pWriter->m_strIPAddr, pWriter->m_nIPPort)==false)
        {
            CString srtInfo = _T("|Error : Network init failed.");
            if (!auto_flash_mode)
                MessageBox(NULL, srtInfo, _T("ISP"), MB_OK);
            //AddStringToOutPuts(_T("Error :The com port is occupied!"));
            pWriter->OutPutsStatusInfo(srtInfo, FALSE);
            pWriter->WriteFinish(0);
            return 0;
        }
        else
        {
            CString strTemp;
            //strTemp.Format(_T("COM%d"), m_nComPort);
            CString strTips = _T("|Connection to ") +  pWriter->m_strIPAddr + _T(" successful.");
            pWriter->OutPutsStatusInfo(strTips, FALSE);
            // AddStringToOutPuts(strTips);
            SetCommunicationType(1);
        }
    }


    //First test if the device is online;
    unsigned short temp_read_reg[50];
    memset(temp_read_reg,0,50);

    int retry_count = 0;
    int temp_mu_ret = 0;


    int test_count = 0;


    pWriter->OutPutsStatusInfo(strTips_isp1);




    for(i = 0; i < pWriter->m_szMdbIDs.size(); i++)
    {

       // for (int Time =0; Time < pWriter->m_FlashTimes; Time++)
        {
            retry_count = 0;
            temp_mu_ret = 0;
            do
            {
                temp_mu_ret = read_multi_tap(pWriter->m_szMdbIDs[i],temp_read_reg,0,40);
                if(temp_mu_ret > 0)
                    break;
                retry_count++;
                if(retry_count > 5)
                {
                    CString strTips_isp = _T("Subnet device is off line,please check the connection.");
                    pWriter->OutPutsStatusInfo(strTips_isp);
                    nFlashRet = false;
                    goto end_tcp_flash_mode;
                }

                CString temp_123;
                temp_123.Format(_T("Wait device reboot.(%d)"),5 - retry_count);
                pWriter->OutPutsStatusInfo(temp_123,true);
                Sleep(2000);


            }
            while (temp_mu_ret < 0);
            CString strTips_isp = _T("Device is online.");
            pWriter->OutPutsStatusInfo(strTips_isp);
            if(temp_read_reg[11] <=1)
            {
//             CString strTips_isp = _T("Check the firmware version failed.(reg11 error)");
//             pWriter->OutPutsStatusInfo(strTips_isp);
//             nFlashRet = false;
//             goto end_tcp_flash_mode;
            }

            while(test_count <=8)
            {
                int nRet = Write_One(pWriter->m_szMdbIDs[i],16,127);   // Enter ISP mode
                if(nRet >= 0)
                    break;
                if(nRet < 0)
                {
                    test_count ++ ;
                }
                if(test_count == 8)
                {
                    CString strTips_isp = _T("Write ISP command failed.(127)");
                    pWriter->OutPutsStatusInfo(strTips_isp);
                    break;
                }

                CString temp_1234;
                temp_1234.Format(_T("Send flash update command to device.(%d)"),8 - test_count);
                pWriter->OutPutsStatusInfo(temp_1234,true);

            }


            Sleep(2000);	//Wait for device to enter ISP

            CString strID;
            strID.Format(_T("|--------------->>ID-%d-<<---------------"), pWriter->m_szMdbIDs[i]);
            pWriter->OutPutsStatusInfo(strID);
            ////��ʾflash֮ǰ��ʱ��
            pWriter->OutPutsStatusInfo(_T("|--------->>Begin"));
            pWriter->OutPutsStatusInfo(_T("|-->>Begin Time:")+GetSysTime());
            //// ��ʾflash֮ǰ���豸״̬��Ϣ
            BOOL Flag_HEX_BIN=FALSE;
//		     ((CISPDlg*)pWriter->m_pParentWnd)->Show_Flash_DeviceInfor(pWriter->m_szMdbIDs[i]);
            pWriter->m_szMdbIDs[i] = temp_read_reg[6];
            if (pWriter->UpdataDeviceInformation_ex(temp_read_reg[7]))
            {
                int Chipsize_6 = 0;
                strTips = _T("|Programming device...");
                pWriter->OutPutsStatusInfo(strTips);


                int ModelID= temp_read_reg[7];
                if (ModelID>0)
                {
                    if (ModelID==6||ModelID==7||ModelID==8)//Tstat6,7 Detect chip flash size
                    {
                        Chipsize_6 = temp_read_reg[11];


                        if (Chipsize_6<37)	//64K
                        {
                            if (pWriter->m_nHexFileType==0)
                            {
                                strTips = _T("|hex file   matches the device CPU!");
                                pWriter->OutPutsStatusInfo(strTips);
                                Flag_HEX_BIN=TRUE;
                            }
                            else
                            {
                                int ii=0;
                                while(ii<=5)
                                {
                                    int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
                                    if (ret>0)
                                    {
                                        break;
                                    }
                                    ii++;
                                }
                                strTips = _T("|hex file doesn't match with the device CPU!");
                                pWriter->OutPutsStatusInfo(strTips);
                                Flag_HEX_BIN=FALSE;
                            }
                        }
                        else	//128K
                        {
                            if (pWriter->m_nHexFileType==0)
                            {
                                int ii=0;
                                while(ii<=5)
                                {
                                    int ret=Write_One(pWriter->m_szMdbIDs[i],33,1);
                                    if (ret>0)
                                    {
                                        break;
                                    }
                                    ii++;
                                }
                                ii=0;
                                while(ii<=5)
                                {
                                    int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
                                    if (ret>0)
                                    {
                                        break;
                                    }
                                    ii++;
                                }
                                strTips = _T("|hex file doesn't match the device CPU!");
                                pWriter->OutPutsStatusInfo(strTips);
                                Flag_HEX_BIN=FALSE;
                            }
                            else
                            {
                                strTips = _T("|hex file   matches the device CPU!");
                                pWriter->OutPutsStatusInfo(strTips);
                                Flag_HEX_BIN=TRUE;
                            }
                        }
                    }
                    else
                    {
                        Flag_HEX_BIN=TRUE;
                    }
                }
                else
                {
                    strTips = _T("Read product type timeout!");
                    pWriter->OutPutsStatusInfo(strTips);
                    nFlashRet = false;
                    goto	 end_tcp_flash_mode;
                }
                int Chipsize = 0;
                //  pWriter->OutPutsStatusInfo(_T("The device doesn��t match with the hex file"));
#if 1		//Reset
                if (ModelID==6||ModelID==7||ModelID==8)
                    Chipsize = Chipsize_6;
                else
                    Chipsize = temp_read_reg[11];

                if (Chipsize<37)	//64K
                {


                    int ii=0;
                    while(ii<=5)
                    {
                        int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                    }


                }
                else	//128K
                {

                    int ii=0;
                    while(ii<=5)
                    {
                        int ret=Write_One(pWriter->m_szMdbIDs[i],33,0);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                    }
                    //ii=0;
                    //while(ii<=5){
                    // int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
                    // if (ret>0)
                    // {
                    //  break;
                    // }
                    // ii++;
                    //}



                }
#endif




            }

            if (!Flag_HEX_BIN)
            {
                continue;
            }

            if(pWriter->m_nHexFileType == 2)
            {
                CString show_section;
                show_section.Format(_T("Total section %d"),pWriter->m_szHexFileFlags.size());
                pWriter->OutPutsStatusInfo(show_section);
                int nCount = 0;
                for(UINT p = 0; p < pWriter->m_szHexFileFlags.size(); p++)
                {
                    int nBufLen = pWriter->m_szHexFileFlags[p]-nCount;

                    //if((nFlashRet = flash_a_tstat(pWriter->m_szMdbIDs[i], pWriter->m_nBufLen, (TS_UC*)pWriter->m_pFileBuffer, pParam)) < 0 )
                    if((nFlashRet = flash_a_tstat(pWriter->m_szMdbIDs[i],nBufLen, (TS_UC*)pWriter->m_pFileBuffer+nCount, pParam)) < 0 )
                    {
                        nFailureNum++;
                        CString strTemp=_T("");
                        strTemp.Format(_T("%d;"), pWriter->m_szMdbIDs[i]);

                        strFailureList+=strTemp;  // flash���ʹ��
                        switch(nFlashRet)
                        {
                        case -1:
                            strTemp.Format(_T("|ID %d Error : Please verify connection!"), pWriter->m_szMdbIDs[i]);
                            break;
                        case -2:
                            strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                            break;
                        case -3:
                            strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                            break;
                        case -4:
                            strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                            break;
                        case -5:
                            strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                            break;
                        case -6:
                            strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                            break;
                        case -7:
                            strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                            break;
                        case -8:
                            strTemp.Format(_T("|ID %d Error : Update was interrupted! Please verify connection."), pWriter->m_szMdbIDs[i]);
                            break;
                        case -9:
                            strTemp.Format(_T("|ID %d Error : Programming was canceled."), pWriter->m_szMdbIDs[i]);
                            break;
                        }
                        pWriter->OutPutsStatusInfo(strTemp);
                        pWriter->OutPutsStatusInfo(_T("|-->>End Time:")+GetSysTime());
                        pWriter->OutPutsStatusInfo(_T("|------->>End"));
                        break;

                    }
                    else
                    {
                        nCount += nBufLen;


                        CString strText;
                        strText.Format(_T("|ID %d: Programming section %d finished."), pWriter->m_szMdbIDs[i], p);
                        pWriter->OutPutsStatusInfo(strText);


                        /*CString strText;
                        strText.Format(_T("|ID %d: Programming successful."), pWriter->m_szMdbIDs[i]);
                        pWriter->OutPutsStatusInfo(strText);*/
                        //// ��ʾflash֮ǰ���豸״̬��Ϣ
                        int time_count = 3;
                        for (int i=0; i<time_count; i++)
                        {
                            CString temp_123;
                            temp_123.Format(_T("Wait device initial.(%d)"),3 - i);
                            pWriter->OutPutsStatusInfo(temp_123,true);
                            Sleep(1000);
                        }
                        //Sleep(13000);//�ȴ�������֮��
//					((CISPDlg*)pWriter->m_pParentWnd)->Show_Flash_DeviceInfor(pWriter->m_szMdbIDs[i]);
                        pWriter->OutPutsStatusInfo(_T("|-->>End Time:")+GetSysTime());
                        pWriter->OutPutsStatusInfo(_T("|------->>End"));
                    }


                    if (p==0)
                    {
                        int ii=0;
                        while(ii<=5)
                        {
                            int ret=Write_One(pWriter->m_szMdbIDs[i], 16, 8);
                            if (ret>0)
                            {
                                break;
                            }
                            ii++;
                        }
                        ii=0;
                        while(ii<=5)
                        {
                            int ret=Write_One(pWriter->m_szMdbIDs[i], 33, 1);
                            if (ret>0)
                            {
                                break;
                            }
                            ii++;
                        }

                    }


                    Sleep(1000); //Must delay here to give CPU time to write
                }

            }
            else
            {
                if((nFlashRet = flash_a_tstat(pWriter->m_szMdbIDs[i], pWriter->m_nBufLen, (TS_UC*)pWriter->m_pFileBuffer, pParam)) < 0 )
                {
                    nFailureNum++;
                    CString strTemp=_T("");
                    strTemp.Format(_T("%d;"), pWriter->m_szMdbIDs[i]);

                    strFailureList+=strTemp;  // flash���ʹ��
                    switch(nFlashRet)
                    {
                    case -1:
                        strTemp.Format(_T("|ID %d Error : Please verify connection!"), pWriter->m_szMdbIDs[i]);
                        break;
                    case -2:
                        strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -3:
                        strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -4:
                        strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -5:
                        strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -6:
                        strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -7:
                        strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -8:
                        strTemp.Format(_T("|ID %d Error : Update was interrupted! Please verify connection."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -9:
                        strTemp.Format(_T("|ID %d Error : Programming was canceled."), pWriter->m_szMdbIDs[i]);
                        break;
                    }
                    pWriter->OutPutsStatusInfo(strTemp);
                    pWriter->OutPutsStatusInfo(_T("|-->>End Time:")+GetSysTime());
                    pWriter->OutPutsStatusInfo(_T("|------->>End"));
                }
                else
                {
                    CString strText;
                    strText.Format(_T("|ID %d: Programming successful."), pWriter->m_szMdbIDs[i]);
                    pWriter->OutPutsStatusInfo(strText);
                    //// ��ʾflash֮ǰ���豸״̬��Ϣ
                    //Sleep(13000);//�ȴ�������֮��
//				((CISPDlg*)pWriter->m_pParentWnd)->Show_Flash_DeviceInfor(pWriter->m_szMdbIDs[i]);
                    pWriter->OutPutsStatusInfo(_T("|-->>End Time:")+GetSysTime());
                    pWriter->OutPutsStatusInfo(_T("|------->>End"));
                }
            }
        }
    }
end_tcp_flash_mode :
    //}//For Test
    //*******************************************************************************
    //Sleep(500);
    pWriter->WriteFinish(nFlashRet);
    close_com();
    return 1;//close thread
    //	}while(1);


}


//////////////////////////////////////////////////////////////////////////
//the return value 1,successful,   return < 0 , fail
int flash_a_tstat(BYTE m_ID, unsigned int the_max_register_number_parameter, TS_UC *register_data_orginal, LPVOID pParam)
{

    CComWriter* pWriter = (CComWriter*)pParam;
    const int  RETRY_TIMES = 10;
    TS_UC *register_data=register_data_orginal;
    unsigned int ii=0;

    //*************inspect the flash at the last flash position ***********************
    int x = mudbus_read_one(m_ID,0xee10);


    //************* begin to flash ***********************
    ii=0;
    if(mudbus_read_one(m_ID,0xee10)==0x40 || mudbus_read_one(m_ID,0xee10)==0x1f) // ��ee10�� why��
    {
        if(IDOK==AfxMessageBox(_T("Previous Update was interrupted.\nPress OK to Resume.\nCancel to Restart."),MB_OKCANCEL))
        {
            // ѡȷ��
            ii=0xEE00+17;
            int l=0;//temp;<200
            do
            {
                int uc_temp1= mudbus_read_one(m_ID,ii);
                int uc_temp2= mudbus_read_one(m_ID,ii+1);
                if(uc_temp1==0x00 && uc_temp2==0x00 )
                    ii+=2;
                else if(l==0)
                {
                    if(uc_temp1==0xf0 && uc_temp2==0xf0)
                    {
                        ii=0;
                        break;
                    }
                }
                else
                {
                    ii=uc_temp1*256+uc_temp2;
                    break;
                }
                l++;
            }
            while(l<200);
        }
        else // ѡȡ��
        {
            //from 0000 flash update
            ii=0;//from 0000 register flash
            CString srtInfo = _T("|Initializing device...");
            pWriter->OutPutsStatusInfo(srtInfo);
            //********************write register 16 value 0x7f **************
            Sleep(500);
            do
            {
                if(ii<RETRY_TIMES)
                    if(-2== mudbus_write_one(m_ID,16,0x7f))
                    {
                        ii++;
                        Sleep(6000);
                    }
                    else
                        ii=0;
                else
                {
                    return -2;// Unable to Initialize...
                }
            }
            while(ii);

            //********************write register 16 value 0x3f **************
            ii=0;
            srtInfo = _T("|Erasing device...");
            pWriter->OutPutsStatusInfo(srtInfo);
            do
            {
                if(ii<RETRY_TIMES)
                {
                    if(-2== mudbus_write_one(m_ID,16,0x3f))
                    {
                        ii++;
                    }
                    else
                    {
                        ii=0;
                    }
                }
                else
                {
                    return -3;//error -3
                }
            }
            while(ii);

            //********************write register 16 value 0x1f **************
            ii=0;
            Sleep(7000);//must have this ,the Tstat need
            do
            {
                if(ii<RETRY_TIMES)
                    if(-2== mudbus_write_one(m_ID,16,0x1f))
                        ii++;
                    else
                        ii=0;
                else
                {
                    return -4;//error -4
                }
            }
            while(ii);
            //***************send data to com*************************
            ii=0;//to the register 0000
        }
    }
    else  // Failed to read Eeprom chip?
    {
        //from 0000 flash update
        ii=0;//from 0000 register flash
        CString srtInfo = _T("|Initializing device...");
        pWriter->OutPutsStatusInfo(srtInfo);
        //writing_row++;
        Sleep(500);

        //********************write register 16 value 0x7f **************
        //x=Write_One(m_ID,16,0x7f);
        do
        {
            if(mudbus_write_one(m_ID,16,0x7f)<0)
            {
                ii++;
                Sleep(1000);
            }
            else
            {
                break;
            }

        }
        while(ii<RETRY_TIMES);

        //********************write register 16 value 0x3f **************
        ii=0;
        srtInfo = _T("|Erasing device...";);
        pWriter->OutPutsStatusInfo(srtInfo);
  

        do
        {
            if(mudbus_write_one(m_ID,16,0x3f)<0)
            {
                ii++;
                Sleep(1000);
            }
            else
            {
                break;
            }

        }
        while(ii<RETRY_TIMES);

        //********************write register 16 value 0x1f **************
        ii=0;
        Sleep(7000);//Delay required while device writes to flash


        do
        {
            if(mudbus_write_one(m_ID,16,0x1f)<0)
            {
                ii++;
                Sleep(1000);
            }
            else
            {
                break;
            }

        }
        while(ii<RETRY_TIMES);


        //***************send data to com*************************
        ii=0;//to the register 0000
    } //End of reading EEprom chip

    int persentfinished=0;
    CString srtInfo;
    srtInfo.Format(_T("|ID %d: Programming lines %d to %d.(0%%)"),m_ID,ii,ii+128);
    pWriter->OutPutsStatusInfo(srtInfo);
    if (Device_infor[7] == 88)
    {
        if(pWriter->continue_com_flash_count < 0)
            pWriter->continue_com_flash_count = 0;
        ii = pWriter->continue_com_flash_count;
        the_max_register_number_parameter = the_max_register_number_parameter + ii;
        Sleep(4000);
    }
    int one_flash_package = 128;
    if (SPECIAL_BAC_TO_MODBUS)
    {
        one_flash_package = 256;
    }
    while(ii<the_max_register_number_parameter)
    {
        if (pWriter->m_bStopWrite)
        {
            return -9;
        }

        TS_UC data_to_send[160]= {0}; // buffer that writefile() will to use
        int itemp=0;
        
        persentfinished = ((ii+ one_flash_package)*100)/the_max_register_number_parameter;
        if(persentfinished>100)
            persentfinished=100;
        CString srtInfo;
        srtInfo.Format(_T("|ID %d: Programming lines %d to %d.(%d%%)"),m_ID,ii,ii+ one_flash_package,persentfinished);
        pWriter->OutPutsStatusInfo(srtInfo, TRUE);
        do
        {
            if(itemp<RETRY_TIMES)
            {
                
                if (-2 == mudbus_write_single_short(m_ID, &register_data[ii], ii, 128))//to write multiple 128 bytes  //���� �����SPECIAL_BAC_TO_MODBUS ���� 128x2
                {
                    itemp++;
                    Sleep(300);
                }
                else
                    itemp = 0;

            }
            else
            {
                //srtInfo.Format(_T("Communication was interrupted.Trying to connect again!"));
                //pWriter->OutPutsStatusInfo(srtInfo, TRUE);
                Sleep(5000);//After 5 fails, wait for 5s and try two more times.

                if(itemp<RETRY_TIMES+3)
                {
                    mudbus_read_one(255,1);//If the connection is disconnected automatically open the last port;
                    if(-2== mudbus_write_single_short(m_ID,&register_data[ii],ii,128))//to write multiple 128 bytes
                        itemp++;
                    else
                        itemp=0;
                }
                else
                {
                    return -8;//the com connection failed! error -8
                }

            }
        }
        while(itemp);
        ii+= one_flash_package;
    }

    //********************write register 16 value 0x01 **************
    ii=0;
    do
    {
        if(ii<RETRY_TIMES)
        {
            if(-2== mudbus_write_one(m_ID,16,1))
                ii++;
            else
                ii=0;
        }
        else
        {
            return -8;//error -8
        }
    }
    while(ii);

    return 1;
}
//////////////////////////////////////////////////////////////////////////
//the return value 1,successful,   return < 0 ,fail
int flash_a_tstat_RAM(BYTE m_ID,int section, unsigned int the_max_register_number_parameter, TS_UC *register_data_orginal, LPVOID pParam)
{
    CComWriter* pWriter = (CComWriter*)pParam;
    const int  RETRY_TIMES = 10;
    TS_UC *register_data=register_data_orginal;
    unsigned int ii=0;



    int persentfinished=0;
    if(section==0)
    {
        CString srtInfo;
        srtInfo.Format(_T("|ID %d: Programming lines %d to %d.(0%%)"),m_ID,ii,ii+128);
        pWriter->OutPutsStatusInfo(srtInfo);
        if (pWriter->Is_Ram == HEX_TYPE_ARM_64K)
        {
            return 1;
        }
    }
    else
    {
        persentfinished = ((the_max_register_number_parameter_Finished)*100)/the_max_register_number_parameter_Count;
        CString srtInfo;
        srtInfo.Format(_T("|ID %d: Programming lines %d to %d.(%d%%)"),m_ID,ii,ii+128,persentfinished);
        pWriter->OutPutsStatusInfo(srtInfo,TRUE);
    }
    ii=0;
    do
    {

        if(mudbus_write_one(m_ID,12,section)<0)
        {
            ii++;
            Sleep(200);
        }
        else
        {
            ii=0;
            break;
        }
    }
    while(ii<RETRY_TIMES);
    if (ii >= RETRY_TIMES)
    {
        CString str;
        str.Format(_T("Write 12 = %d Failed"),section);
		if(!auto_flash_mode)
			AfxMessageBox(str);
        return -1;

    }
    //	while(ii<=the_max_register_number_parameter)
    while(ii<the_max_register_number_parameter)
    {
        if (pWriter->m_bStopWrite)
        {
            return -9;
        }
        TS_UC data_to_send[160]= {0}; // buffer that writefile() will to use
        int itemp=0;

        //persentfinished = ((ii+128)*100)/the_max_register_number_parameter;
        persentfinished = ((the_max_register_number_parameter_Finished+ii+128)*100)/the_max_register_number_parameter_Count;
        if(persentfinished>100)
            persentfinished=100;
        CString srtInfo;
        srtInfo.Format(_T("|ID %d: Programming lines %d to %d.(%d%%)"),m_ID,the_max_register_number_parameter_Finished+ii,the_max_register_number_parameter_Finished+ii+128,persentfinished);
        pWriter->OutPutsStatusInfo(srtInfo, TRUE);

         

        do
        {
            /*   if(itemp<RETRY_TIMES)
               {*/
            //ii+0x8000
            static long temp_last_time = 0;
            if(mudbus_write_single_short(m_ID,&register_data[ii],ii,128)<0)//to write multiple 128 bytes
            {
                long tempticktime = GetTickCount();
                int temp_p = tempticktime - temp_last_time;
                TRACE(_T("f(%d): %u  time:%u\r\n"),ii, tempticktime, tempticktime - temp_last_time);
                temp_last_time = tempticktime;
                if (itemp <= com_error_delay_count)
                    Sleep(com_error_delay_time);
                else
                    Sleep(300 + itemp*100); //дʧ�� ����300ms�� ����;
                itemp++;
                
            }
            else
            {
#ifdef ISP_BURNING_MODE
                    for (int j = 0; j < 128; j++)
                    {
                        Check_sum = Check_sum + register_data[ii + j];
                    }
                //Check_sum = Check_sum % 0x10000;
#endif
                long nowticktime = GetTickCount();
                TRACE(_T("s(%d): %u time:%u\r\n"),ii, nowticktime , nowticktime - temp_last_time);
                temp_last_time = nowticktime;
                itemp=0;
                break;
            }


            /*     }*/


        }
        while(itemp<RETRY_TIMES*2);


        if (itemp >= RETRY_TIMES*2)
        {
            CString str;
            str.Format(_T("Write Flash ID = %d To = 128*%d "),m_ID,ii);
			if(!auto_flash_mode)
				 AfxMessageBox(str);
            return -8;

        }

        ii+=128;
    }




    return 1;
}

BOOL CComWriter::EndFlash()
{
    return StopWrite();
}

BOOL CComWriter::StopWrite()
{
    m_bStopWrite = TRUE;
    if(WaitForSingleObject(m_pWorkThread->m_hThread, 3000) == WAIT_OBJECT_0)
    {

    }
    else
    {
        BOOL bRet = TerminateThread(m_pWorkThread->m_hThread, 0);

    }


    m_pWorkThread = NULL;
    return TRUE;
}


void CComWriter::OutPutsStatusInfo(const CString& strInfo, BOOL bReplace)
{
    int nCount = strInfo.GetLength();
    WCHAR* strNew = new WCHAR[nCount+1];
    ZeroMemory(strNew, (nCount+1)*sizeof(WCHAR));
    LPCTSTR str = LPCTSTR(strInfo);
    memcpy(strNew, str, nCount*sizeof(WCHAR));
    //g_strFlashInfo = strInfo;
    int nRet = 0;
    if (bReplace)
    {
        nRet =PostMessage(m_pParentWnd->m_hWnd, WM_REPLACE_STATUSINFO, 0, LPARAM(strNew));
    }
    else
    {
        nRet =PostMessage(m_pParentWnd->m_hWnd, WM_ADD_STATUSINFO, 0,LPARAM(strNew));
    }

}


//////////////////////////////////////////////////////////////////////////
// Flash finished, regardless of success or failure, notify the parent window
// parameter is the return value of the flash thread
void CComWriter::WriteFinish(int nFlashFlag)
{
    int	nRet =PostMessage(m_pParentWnd->m_hWnd, WM_FLASH_FINISH, 0, LPARAM(nFlashFlag));
}


void CComWriter::SetHexFileType(HEXFILE_FORMAT nHexFileType)
{
    m_nHexFileType = nHexFileType;
}


int CComWriter::WirteExtendHexFileByCom()
{
    //WriteCommandtoReset();

    HCURSOR hc;//load mouse cursor
    hc = LoadCursor(NULL,IDC_WAIT);
    hc = SetCursor(hc);
    if (SPECIAL_BAC_TO_MODBUS)
    {
        int ret = Initial_bac(m_nComPort, _T(""), m_nBautrate);
        CString temp_cs;
        if (ret <= 0)
        {
            temp_cs.Format(_T("Failed to open serial port :%d"), m_nComPort);
            OutPutsStatusInfo(temp_cs, FALSE);
            WriteFinish(0);
            return 0;
        }
        else
        {
            temp_cs.Format(_T("Initial Bacnet MSTP port :%d baudrate:%d success!"), m_nComPort, m_nBautrate);
            OutPutsStatusInfo(temp_cs, FALSE);
        }
        Sleep(1);
    }
    else
    {
        if (open_com(m_nComPort) == false)
        {
            CString srtInfo = _T("|Error :The com port is occupied!");
            OutPutsStatusInfo(srtInfo, FALSE);
            srtInfo = _T("You need to shut down all other applications and");
            OutPutsStatusInfo(srtInfo, FALSE);
            srtInfo = _T("background tasks that might be occupying this COM port!");
            OutPutsStatusInfo(srtInfo, FALSE);
            WriteFinish(0);
            return 0;
        }
        else
        {
            CString strTemp;
            strTemp.Format(_T("COM%d"), m_nComPort);
            CString strTips = _T("|Open ") + strTemp + _T(" successful.");
            OutPutsStatusInfo(strTips, FALSE);
            Change_BaudRate(m_nBautrate);
        }
    }

    CString strTips = _T("|Programming device...");
    OutPutsStatusInfo(strTips);
    m_pWorkThread=AfxBeginThread(flashThread_ForExtendFormatHexfile, this); //create thread,read information

    ASSERT(m_pWorkThread);

    return 1;
}




int CComWriter::WirteExtendHexFileByCom_RAM()
{
    HCURSOR hc;//load mouse cursor
    hc = LoadCursor(NULL,IDC_WAIT);
    hc = SetCursor(hc);

    if (SPECIAL_BAC_TO_MODBUS)
    {
        int ret = Initial_bac(m_nComPort, _T(""), m_nBautrate);
        CString temp_cs;
        if (ret <= 0)
        {
            temp_cs.Format(_T("Failed to open serial port :%d"), m_nComPort);
            OutPutsStatusInfo(temp_cs, FALSE);
            WriteFinish(0);
            return 0;
        }
        else
        {
            temp_cs.Format(_T("Initial Bacnet MSTP port :%d baudrate:%d success!"), m_nComPort, m_nBautrate);
            OutPutsStatusInfo(temp_cs, FALSE);
        }
        Sleep(1);
    }
    else
    {
        if (open_com(m_nComPort) == false)
        {
            CString srtInfo = _T("|Error :The com port is occupied!");

            OutPutsStatusInfo(srtInfo, FALSE);
            srtInfo = _T("You need to shut down all other applications and");
            OutPutsStatusInfo(srtInfo, FALSE);
            srtInfo = _T("background tasks that might be occupying this COM port!");
            OutPutsStatusInfo(srtInfo, FALSE);
            WriteFinish(0);
            return 0;
        }
        else
        {
            CString strTemp;
            strTemp.Format(_T("COM%d"), m_nComPort);
            CString strTips = _T("|Open ") + strTemp + _T(" successful.");
            OutPutsStatusInfo(strTips, FALSE);
            Change_BaudRate(m_nBautrate);

        }
    }



    CString strTips = _T("|Programming device...");
    OutPutsStatusInfo(strTips);
    //AddStringToOutPuts(strTips);
    m_pWorkThread=AfxBeginThread(flashThread_ForExtendFormatHexfile_RAM, this); //create thread,read information

    ASSERT(m_pWorkThread);

    return 1;
}

extern unsigned char firmware_md5[32] ;
UINT flashThread_ForExtendFormatHexfile(LPVOID pParam)
{
    CComWriter* pWriter = (CComWriter*)(pParam);
    CString strFailureList;
    CString strTips;
    int nFlashRet=0;
    UINT i=0;
    int nFailureNum = 0;

    UINT times=0;

#if 1

    for(i = 0; i < pWriter->m_szMdbIDs.size(); i++)
    {
        if (SPECIAL_BAC_TO_MODBUS)
        {
            g_mstp_deviceid = 0;
            //����ID�õ���Ӧ�� device id.
            CString strtemp;
            strtemp.Format(_T("|Reading Device Object Instance"));
            pWriter->OutPutsStatusInfo(strtemp);
            strtemp.Format(_T(" "));
            pWriter->OutPutsStatusInfo(strtemp);
            int temp_found_device_id = 0;
            for (int x = 0; x < 30; x++)
            {
                CString temp_found_id = _T("Found MAC ID:");
                Send_WhoIs_Global(-1, -1);
                Sleep(2000);
                if (m_bac_handle_Iam_data.size() > 0)
                {
                    for (int j = 0; j < m_bac_handle_Iam_data.size(); j++)
                    {
                        if (j != 0)
                            temp_found_id = temp_found_id + _T(",");
                        CString temp_id;
                        temp_id.Format(_T("%d"), m_bac_handle_Iam_data.at(j).macaddress);
                        temp_found_id = temp_found_id + temp_id;
                        if (m_bac_handle_Iam_data.at(j).macaddress == pWriter->m_szMdbIDs[i])
                        {
                            temp_found_device_id = m_bac_handle_Iam_data.at(j).device_id;
                            break;
                        }
                    }
                }

                if (temp_found_device_id > 0)
                {
                    g_mstp_deviceid = temp_found_device_id;
                    CString strtemp;
                    strtemp.Format(_T("|ID %d is online.Device instance : %u!"), pWriter->m_szMdbIDs[i], g_mstp_deviceid);
                    pWriter->OutPutsStatusInfo(strtemp);
                    break;
                }
                pWriter->OutPutsStatusInfo(temp_found_id, true);
            }
            if (g_mstp_deviceid == 0)
            {
                CString strtemp;
                strtemp.Format(_T("|ID %d is offline.Update failed!"), pWriter->m_szMdbIDs[i]);
                pWriter->OutPutsStatusInfo(strtemp);
                goto end_isp_flash;
            }
        }

     //   for (int Time =0; Time < pWriter->m_FlashTimes; Time++)
        {
            CString strID;
            strID.Format(_T("|Current Programming device ID is : %d"), pWriter->m_szMdbIDs[i]);
            pWriter->OutPutsStatusInfo(strID);
            BOOL Flag_HEX_BIN=FALSE;
            if (pWriter->UpdataDeviceInformation(pWriter->m_szMdbIDs[i]))
            {
                if ((Device_infor[4] == Device_infor[14]) &&   
                    (Device_infor[14] >= 20) &&
                    (Device_infor[7] == 88))
                {
                    unsigned short wirte_md5[4];
                    int compare_ret = 1;


                    //���88esp���豸��bootloader ����
                    int n_ret = modbus_read_multi(pWriter->m_szMdbIDs[i], &pWriter->update_firmware_info[0], 1994, 6);
                    if (n_ret > 0)
                    {
                        //�ȶ�MD5
                        for (int x = 0; x < 4; x++)
                        {
                            wirte_md5[x] = firmware_md5[2 * x] * 256 + firmware_md5[2 * x + 1];
                            if (pWriter->update_firmware_info[x + 1] != wirte_md5[x])
                            {
                                compare_ret = -1;
                                //break;
                            }
                        }

                        if ((compare_ret == 1) && (pWriter->update_firmware_info[0] != 0))
                        {
                            pWriter->continue_com_flash_count = pWriter->update_firmware_info[0] * 128; // 1994��ŵ��Ǵ�����д�˶��ٰ�;
                        }
                        else
                        {
                            


                            pWriter->continue_com_flash_count = 0;
                        }



                       
                    }
                    else
                    {
                        pWriter->continue_com_flash_count = 0;
                    }

                    if (pWriter->continue_com_flash_count == 0)
                    {
                        for (int y = 0; y < 4; y++)
                        {
                            int ret = mudbus_write_one(pWriter->m_szMdbIDs[i], 1995 + y, wirte_md5[y],3);


                        }
                        
                    }
                    //continue_com_flash_count =
                }
                int nRet = mudbus_write_one(pWriter->m_szMdbIDs[i],16,127);   // Enter ISP mode
                if(nRet < 0)
                    mudbus_write_one(pWriter->m_szMdbIDs[i],16,127);   // Enter ISP mode

                //TBD: explain this comment better
 /*  If you jump from the application code to the ISP 16 write 127, you need to read 11th register 11th number greater than 1 description of the jump success, otherwise continue to wait; */

                strTips = _T("Wait device jump to isp mode!");
                pWriter->OutPutsStatusInfo(strTips);
                int re_count = 0;
                int nnn_ret = 0;
                do
                {
                    nnn_ret  = mudbus_read_one(pWriter->m_szMdbIDs[i],11);
                    if(nnn_ret > 1)
                        break;
                    Sleep(1000);
                    re_count ++;
                    if(re_count == 15)
                        break;
                }
                while (nnn_ret > 1);

                // Sleep(2000);
                int ModelID= mudbus_read_one(pWriter->m_szMdbIDs[i],7,5);
                if (ModelID>0)
                {
                    if (ModelID==6||ModelID==7||ModelID==8)//Tstat6,7,8 Detect CPU flash size.                     {
                    {
                        int Chipsize= mudbus_read_one(pWriter->m_szMdbIDs[i],11,5);
                        if(Chipsize < 0)
                        {
                            nFlashRet = false;
                            goto end_isp_flash;
                        }


                        if (Chipsize<37)	//64K
                        {
                            if (pWriter->m_nHexFileType==0)
                            {
                                strTips = _T("|hex file   matches with the chip!");
                                pWriter->OutPutsStatusInfo(strTips);
                                Flag_HEX_BIN=TRUE;
                            }
                            else
                            {
                                int ii=0;
                                while(ii<=5)
                                {
                                    int ret= mudbus_write_one(pWriter->m_szMdbIDs[i],16,1);
                                    if (ret>0)
                                    {
                                        break;
                                    }
                                    ii++;
                                }
                                strTips = _T("|hex file doesn't match with the device CPU!");
                                pWriter->OutPutsStatusInfo(strTips);
                                Flag_HEX_BIN=FALSE;

                            }
                        }
                        else	//128K
                        {
                            if (pWriter->m_nHexFileType==0)
                            {
                                int ii=0;
                                while(ii<=5)
                                {
                                    int ret= mudbus_write_one(pWriter->m_szMdbIDs[i],33,0);
                                    if (ret>0)
                                    {
                                        break;
                                    }
                                    ii++;
                                }

                                strTips = _T("|hex file doesn't match with the device CPU!");
                                pWriter->OutPutsStatusInfo(strTips);
                                Flag_HEX_BIN=FALSE;
                            }
                            else
                            {
                                strTips = _T("|hex file   matches with the device CPU!");
                                pWriter->OutPutsStatusInfo(strTips);
                                Flag_HEX_BIN=TRUE;
                            }
                        }
                    }
                }
                else
                {
                    Flag_HEX_BIN=TRUE;
                }

                // pWriter->OutPutsStatusInfo(_T("The device doesn��t match with the hex file"));
#if 1		//��λ
                int Chipsize= mudbus_read_one(pWriter->m_szMdbIDs[i],11,5);
                if (Chipsize<37)	//64K
                {


                    int ii=0;
                    while(ii<=5)
                    {
                        if (Device_infor[7] == 88)
                            break;
                        int ret= mudbus_write_one(pWriter->m_szMdbIDs[i],16,1);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                    }


                }
                else	//128K
                {

                    int ii=0;
                    while(ii<=5)
                    {
                        int ret= mudbus_write_one(pWriter->m_szMdbIDs[i],33,0);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                    }



                }
#endif

            }
            if (pWriter->m_com_flash_binfile == 0)
            {
                if (!Flag_HEX_BIN)
                {
                    continue;
                }
            }


            int nCount = 0;
            if (pWriter->continue_com_flash_count > 0)
            {
                CString strTips;
                nCount = pWriter->continue_com_flash_count ;
                strTips.Format(_T("Resume at breakpoint, starting from package %u"), pWriter->continue_com_flash_count/128);
                pWriter->OutPutsStatusInfo(strTips, FALSE);
            }
            for(UINT p = 0; p < pWriter->m_szHexFileFlags.size(); p++)
            {
                int nBufLen = pWriter->m_szHexFileFlags[p]-nCount;
                if (Device_infor[7] == 88)
                {
                    nFlashRet = flash_a_tstat(pWriter->m_szMdbIDs[i], nBufLen, (TS_UC*)(pWriter->m_pExtendFileBuffer), pParam);
                }
                else
                {
                    nFlashRet = flash_a_tstat(pWriter->m_szMdbIDs[i], nBufLen, (TS_UC*)(pWriter->m_pExtendFileBuffer + nCount), pParam);
                }
                //if((nFlashRet = flash_a_tstat(pWriter->m_szMdbIDs[i], nBufLen, (TS_UC*)(pWriter->m_pExtendFileBuffer+nCount), pParam)) < 0 )
                if(nFlashRet < 0)
                {
                    nFailureNum++;
                    CString strTemp=_T("");
                    strTemp.Format(_T("%d;"), pWriter->m_szMdbIDs[i]);

                    strFailureList+=strTemp;  // flash���ʹ��
                    switch(nFlashRet)
                    {
                    case -1:
                        strTemp.Format(_T("|ID %d Error : Please verify connection!"), pWriter->m_szMdbIDs[i]);
                        break;
                    case -2:
                        strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -3:
                        strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -4:
                        strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -5:
                        strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -6:
                        strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -7:
                        strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -8:
                        strTemp.Format(_T("|ID %d Error : Update was interrupted! Please verify connection."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -9:
                        strTemp.Format(_T("|ID %d Error : Programming was canceled."), pWriter->m_szMdbIDs[i]);
                        break;
                    }
                    pWriter->OutPutsStatusInfo(strTemp);
                    break;
                }
                else
                {
                    nCount += nBufLen;

                    CString strText;
                    strText.Format(_T("|ID %d: Programming section %d finished."), pWriter->m_szMdbIDs[i], p);
                    pWriter->OutPutsStatusInfo(strText);

                }

                if (p==0)
                {
                    int ii=0;
                    while(ii<=5)
                    {
                        int ret= mudbus_write_one(pWriter->m_szMdbIDs[i], 16, 8);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                    }
                    ii=0;
                    while(ii<=5)
                    {
                        int ret= mudbus_write_one(pWriter->m_szMdbIDs[i], 33, 1);
                        if (ret>0)
                        {
                            break;
                        }
                        ii++;
                        if(ii == 5)
                        {
                            nFlashRet = false;
                            goto end_isp_flash;
                        }
                    }
                }

                Sleep(500); //Must delay here

            }

            if ((Device_infor[7] == 88) && (nFlashRet > 0))
            {
                int temp_flag = 0;
                temp_flag = mudbus_read_one(pWriter->m_szMdbIDs[i], 23, 5);
                CString strText;
                if (temp_flag == 0x51)
                {
                    strText.Format(_T("|ID %d: update esp_ota_end failed."), pWriter->m_szMdbIDs[i]);
                    pWriter->OutPutsStatusInfo(strText);
                    nFlashRet = -1;
                }
                else if (temp_flag == 0x52)
                {
                    strText.Format(_T("|ID %d: esp_ota_set_boot_partition  failed."), pWriter->m_szMdbIDs[i]);
                    pWriter->OutPutsStatusInfo(strText);
                    nFlashRet = -1;
                }
                else if (temp_flag == 0x53)
                {
                    strText.Format(_T("|ID %d: esp ota success."), pWriter->m_szMdbIDs[i]);
                    pWriter->OutPutsStatusInfo(strText);
                    nFlashRet = 1;
                }
                else if (temp_flag < 0)
                {
                    nFlashRet = 1;
                }
                else
                {
                    strText.Format(_T("|ID %d: Device update status unknown."), pWriter->m_szMdbIDs[i]);
                    pWriter->OutPutsStatusInfo(strText);
                    nFlashRet = 1;
                }
            }

            if(nFlashRet > 0) // flash success            {
            {
                CString strText;
                strText.Format(_T("|ID %d: Programming successful."), pWriter->m_szMdbIDs[i]);
                pWriter->OutPutsStatusInfo(strText);
            }
        }
    }

#endif









//*******************************************************************************
end_isp_flash:
    pWriter->WriteFinish(nFlashRet);
    Sleep(500);

    close_com();
    if (SPECIAL_BAC_TO_MODBUS)
        close_bac_com();
    return 1;//close thread
//	}while(1);


}
void CComWriter::SetHexInfor(Bin_Info temp)
{
    int i;
    for (  i=0; i<5; i++)
    {
        m_hexinfor.company[i]=temp.company[i];
    }
    for (  i=0; i<10; i++)
    {
        m_hexinfor.product_name[i]=temp.product_name[i];
    }
    for (  i=0; i<3; i++)
    {
        m_hexinfor.reserved[i]=temp.reserved[i];
    }
    m_hexinfor.software_high=temp.software_high;
    m_hexinfor.software_low=temp.software_low;
}



BOOL CComWriter::UpdataDeviceInformation_ex(unsigned short device_productID)
{
    CString strtips;

    CString str_ret,temp;
    
    CString hexproductname=_T("");
    //  int ret=read_multi(ID,&Device_infor[0],0,10);

    if (n_check_temco_firmware == 0)
        return 1;

    CString prodcutname=GetProductName(device_productID);
	if (device_productID == 255 || device_productID == 0)
	{
		return TRUE;
	}



    for (int i=0; i<10; i++)
    {
        hexproductname.AppendFormat(_T("%c"),global_fileInfor.product_name[i]);
    }
    prodcutname.MakeLower();


    hexproductname.MakeLower();

    prodcutname.MakeUpper();
    hexproductname.MakeUpper();

	CString Temco_logo;
	MultiByteToWideChar( CP_ACP, 0, (char *)global_fileInfor.company,
		(int)strlen(global_fileInfor.company)+1,
		Temco_logo.GetBuffer(MAX_PATH), MAX_PATH );
	Temco_logo.ReleaseBuffer();
	Temco_logo.MakeUpper();


    if(hexproductname.CompareNoCase(prodcutname)==0)
    {
        return TRUE;
    }//TStatRunar
    else if ((
                 (prodcutname.CompareNoCase(_T("tstat5e"))==0)
                 ||(prodcutname.CompareNoCase(_T("tstat5h"))==0)
                 ||(prodcutname.CompareNoCase(_T("tstat5g"))==0)
             )
             &&(hexproductname.CompareNoCase(_T("tstat5lcd"))==0)
            )
    {
        return TRUE;
    }
    else if (((prodcutname.CompareNoCase(_T("tstat5a"))==0)
              ||(prodcutname.CompareNoCase(_T("tstat5b"))==0)
              ||(prodcutname.CompareNoCase(_T("tstat5c"))==0)
              ||(prodcutname.CompareNoCase(_T("tstat5d"))==0)
              ||(prodcutname.CompareNoCase(_T("tstat5f"))==0)
             )
             &&(hexproductname.CompareNoCase(_T("tstat5led"))==0)
            )
    {
        return TRUE;
    }
	else if (((prodcutname.CompareNoCase(_T("co2net")) == 0)
		|| (prodcutname.CompareNoCase(_T("co2rs485")) == 0)
		|| (prodcutname.CompareNoCase(_T("humnet")) == 0)
		|| (prodcutname.CompareNoCase(_T("humrs485")) == 0)
        || (prodcutname.CompareNoCase(_T("PM2.5")) == 0)
		|| (prodcutname.CompareNoCase(_T("psnet")) == 0)
		)
		&& (hexproductname.CompareNoCase(_T("co2all")) == 0)
		)
	{
		return TRUE;
	}
    else if ((prodcutname.CompareNoCase(_T("CO2")) == 0 ) &&
        hexproductname.CompareNoCase(_T("CO2 NET")) == 0)
    {
        return TRUE;
    }
    else if ((hexproductname.CompareNoCase(_T("tstat6"))==0)&&(prodcutname.CompareNoCase(_T("tstat5i"))==0))
    {
        return TRUE;
    }
    else if ((hexproductname.CompareNoCase(_T("CM5_ARM")) == 0) && (prodcutname.CompareNoCase(_T("CM5")) == 0))
    {
        return TRUE;
    }
    else
    {
        strtips.Format(_T("Your device is %s   but the hex file is for %s "),prodcutname.GetBuffer(),hexproductname.GetBuffer());
        OutPutsStatusInfo(strtips,false);
        return FALSE;
    }



}

int CComWriter::Fix_Tstat10_76800_baudrate()
{

    if (Device_infor[7] != 10)
        return 0;
    if (m_nBautrate != 76800) //����ѡ��Ĳ����ʲ���76800���������  // If the baud rate selected in the interface is not 76800, ignore it
        return 0;
    if (Device_infor[14] >= 79) //�̼��汾����79��˵���Ѿ��޸�����  // If firmware version is greater than 79, it means it has been fixed
        return 0;
    if ((Device_infor[14] == 0) && (Device_infor[11] > 50)) //˵����BootLoader��  // Indicates it's in BootLoader
    {
        return 0;
        Sleep(1);
    }
    else if ((Device_infor[14] > 0) && (Device_infor[11] == 1)) //˵����Ӧ�ô�����  // Indicates it's in application code
    {
        return 1;
        Sleep(1);
    }


}

int CComWriter::UpdataDeviceInformation(int& ID)
{
    if (n_check_temco_firmware == 0)
        return 1;
    CString strtips;

    CString str_ret,temp;
 
    CString hexproductname=_T("");

    int ret=0;
    int resend_count = 0;
    do
    {
        ret = modbus_read_multi(ID,&Device_infor[0],0,18);
        if(ret >= 0)
            break;
        resend_count ++ ;
        if(resend_count >8)
        {
            strtips.Format(_T("Device is offline,Please check the connection!"));
            OutPutsStatusInfo(strtips,false);
            return FALSE;
        }
        else
        {
            strtips.Format(_T("Reading device version information!(%d)"), resend_count);
            if(resend_count == 1)
                OutPutsStatusInfo(strtips, false);
            else
                OutPutsStatusInfo(strtips, true);
        }
        Sleep(1000);

    }
    while (ret < 0);

	if (Device_infor[7] == 255 || Device_infor[7] == 0)
	{
		return TRUE;
	}
    


    int temp_boot_version = isp_max(Device_infor[11], Device_infor[14]);
    CString strtips_version;
    if(temp_boot_version != 0)
        strtips_version.Format(_T("Bootloader version : %u"), temp_boot_version);
    else
        strtips_version.Format(_T("Bootloader version : unknown"));
    OutPutsStatusInfo(strtips_version, false);


    CString prodcutname= GetFirmwareUpdateName(Device_infor[7]);

    


    MultiByteToWideChar(CP_ACP, 0, (char *)global_fileInfor.product_name,
        (int)strlen(global_fileInfor.product_name) + 1,
        hexproductname.GetBuffer(MAX_PATH), MAX_PATH);
    hexproductname.ReleaseBuffer();
    hexproductname.Left(10);
    //for (int i=0; i<10; i++)
    //{
    //    hexproductname.AppendFormat(_T("%c"),global_fileInfor.product_name[i]);
    //}
	CString Temco_logo;
	MultiByteToWideChar( CP_ACP, 0, (char *)global_fileInfor.company,
		(int)strlen(global_fileInfor.company)+1,
		Temco_logo.GetBuffer(MAX_PATH), MAX_PATH );
	Temco_logo.ReleaseBuffer();
	Temco_logo.MakeUpper();

// 	if (Temco_logo.Find(L"CO2")!=-1)
// 	{
// 		hexproductname = Temco_logo;
// 	}

    prodcutname.MakeLower();


    hexproductname.MakeLower();
	if (hexproductname.CompareNoCase(L"mini_arm") == 0)
	{
		hexproductname = L"Minipanel";
	}
    prodcutname.MakeUpper();
    hexproductname.MakeUpper();
	hexproductname.TrimLeft();
	hexproductname.TrimRight();
    int Ret_Result = 1;


    if(hexproductname.CompareNoCase(prodcutname)==0)
    {
        Ret_Result= TRUE;
    }//TStatRunar
    else if ((
                 (prodcutname.CompareNoCase(_T("tstat5e"))==0)
                 ||(prodcutname.CompareNoCase(_T("tstat5h"))==0)
                 ||(prodcutname.CompareNoCase(_T("tstat5g"))==0)
             )
             &&(hexproductname.CompareNoCase(_T("tstat5lcd"))==0)
            )
    {
        Ret_Result= TRUE;
    }
    else if (((prodcutname.CompareNoCase(_T("tstat5a"))==0)
              ||(prodcutname.CompareNoCase(_T("tstat5b"))==0)
              ||(prodcutname.CompareNoCase(_T("tstat5c"))==0)
              ||(prodcutname.CompareNoCase(_T("tstat5d"))==0)
              ||(prodcutname.CompareNoCase(_T("tstat5f"))==0)
             )
             &&(hexproductname.CompareNoCase(_T("tstat5led"))==0)
            )
    {
        Ret_Result= TRUE;
    }

	else if (((prodcutname.CompareNoCase(_T("co2net")) == 0)
		|| (prodcutname.CompareNoCase(_T("co2rs485")) == 0)
		|| (prodcutname.CompareNoCase(_T("humnet")) == 0)
		|| (prodcutname.CompareNoCase(_T("humrs485")) == 0)
        || (prodcutname.CompareNoCase(_T("PM2.5")) == 0)
		|| (prodcutname.CompareNoCase(_T("psnet")) == 0)
		)
		&& (hexproductname.CompareNoCase(_T("co2all")) == 0)
		)
	{
        Ret_Result = TRUE;
	}
    else if ((hexproductname.CompareNoCase(_T("tstat6"))==0)&&(prodcutname.CompareNoCase(_T("tstat5i"))==0))
    {
        Ret_Result= TRUE;
    }
    else if ((hexproductname.CompareNoCase(_T("CM5_ARM")) == 0) && (prodcutname.CompareNoCase(_T("CM5")) == 0))
    {
        Ret_Result = TRUE;
    }
    else
    {
        strtips.Format(_T("Your device is %s   but the hex file is for %s "),prodcutname.GetBuffer(),hexproductname.GetBuffer());
        OutPutsStatusInfo(strtips,false);
        Ret_Result = FALSE;
        return Ret_Result;
    }
    int c2_update_boot = 0;

   Ret_Result = check_bootloader_and_frimware(Device_infor[7], 0, Device_infor[11], Device_infor[14], c2_update_boot, Device_infor[4]);

    if (c2_update_boot == 1)
    {
        CString str_temp_tips;
        str_temp_tips.Format(_T("New bootloader available ,need update!"));
        OutPutsStatusInfo(str_temp_tips, false);
        PostMessage(m_pParentWnd->m_hWnd, WM_FLASH_RESTATR_BOOT, Device_infor[7], 0);
        Ret_Result = 2;
    }
#if 0
    if (com_port_flash_status == 0)
    {
        if ((Device_infor[7] == PM_TSTAT8) ||
            (Device_infor[7] == PM_TSTAT9)  ||
            (Device_infor[7] == PM_MINIPANEL_ARM) ||
            (Device_infor[7] == PM_MINIPANEL) ||
            (Device_infor[7] == PM_TSTAT10))
        {
            int c2_update_boot = false;
            int device_version = 0;


            if (firmware_must_use_new_bootloader == 0)
                c2_update_boot = false;
            else
            {
                int temp_bootloader_version = 0;
                temp_bootloader_version = isp_max(Device_infor[11], Device_infor[14]);
                if ((Device_infor[7] == PM_TSTAT8) && (temp_bootloader_version <= 48))
                {
                    c2_update_boot = true;
                }
                else if ((Device_infor[7] == PM_MINIPANEL_ARM) && (temp_bootloader_version < 62))
                {
                    c2_update_boot = false; //��֧�ִ��ڸ��� - Does not support serial port update
                    Ret_Result = -1;
                }
                else if ((Device_infor[7] == PM_MINIPANEL) && (temp_bootloader_version < 62))
                {
                    c2_update_boot = false; //��֧�ִ��ڸ��� - Does not support serial port update
                    Ret_Result = -1;
                }
                else if ((Device_infor[7] == PM_TSTAT10) && (temp_bootloader_version < 54))
                {
                    c2_update_boot = true;
                }
                else
                    c2_update_boot = false;
            }

            if (c2_update_boot == 1)
            {
                strtips.Format(_T("New bootloader available ,need update!"), resend_count);
                OutPutsStatusInfo(strtips, false);
                PostMessage(m_pParentWnd->m_hWnd, WM_FLASH_RESTATR_BOOT, Device_infor[7], 0);
                Ret_Result = 2;
            }
        }
    }
#endif




    return Ret_Result;
    if (Ret_Result)
    {
        if (Device_infor[4]==global_fileInfor.software_low&&Device_infor[5]==global_fileInfor.software_high)
        {
            strtips.Format(_T("The firmware is already up to date"),prodcutname.GetBuffer(),hexproductname.GetBuffer());
            OutPutsStatusInfo(strtips,false);
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }


}
void CComWriter::SetIPAddr(const CString& strIPAddr)
{
    m_strIPAddr = strIPAddr;
}

void CComWriter::SetIPPort(int nIPPort)
{
    m_nIPPort = nIPPort;
}

int CComWriter::BeginWirteByTCP()
{
    ASSERT(m_szMdbIDs.size() > 0);

    if (m_nHexFileType == 2&&Is_Ram)	//Marked by Fance
    {
        if(GetCommunicationType()==1)
        {
            if(Open_Socket2(m_strIPAddr, m_nIPPort)==false)
            {
                CString srtInfo = _T("|Error : Network init failed.");
                if (!auto_flash_mode)
                    MessageBox(NULL, srtInfo, _T("ISP"), MB_OK);
                //AddStringToOutPuts(_T("Error :The com port is occupied!"));
                OutPutsStatusInfo(srtInfo, FALSE);
                WriteFinish(0);
                return 0;
            }
            else
            {
                CString strTemp;
                //strTemp.Format(_T("COM%d"), m_nComPort);
                CString strTips = _T("|Connect to ") +  m_strIPAddr + _T(" successful.");
                OutPutsStatusInfo(strTips, FALSE);
                // AddStringToOutPuts(strTips);
                SetCommunicationType(1);
            }
        }
        m_subnet_flash = 1; //���ñ�־λ���Ǵ� TCP ת �ӿ� ����д; - Set flag to indicate flashing from TCP to sub-port
        CString strTips = _T("|Programming device...");
        OutPutsStatusInfo(strTips);
        //AddStringToOutPuts(strTips);
        m_pWorkThread=AfxBeginThread(flashThread_ForExtendFormatHexfile_RAM, this); //create thread,read information
        ASSERT(m_pWorkThread);
    }
    else
    {

        //AddStringToOutPuts(strTips);


        //m_pWorkThread=AfxBeginThread(run_back_ground_flash_thread, this); //create thread,read information
        m_pWorkThread=AfxBeginThread(Flash_Modebus_Device, this); //create thread,read information
        ASSERT(m_pWorkThread);
    }

    return 1;
}

typedef enum
{
    F_INITIAL,
    F_START_SHUTDOWN,
    F_SUCCESS,
    F_TIMEOUT,
}E_FLAG_SHUTDOWN;


int fun_shutdown(LPVOID pParam,int nretry_count)
{
    CComWriter* pWriter = (CComWriter*)(pParam);
    int nret_value = 0;
    for (int i = 0; i < nretry_count; i++)
    {
        int nflag = F_INITIAL;
        unsigned short silent_command = 0; //��λʱ��  ��λ���� - High byte time low byte command
        silent_command = 0x0A00 | F_START_SHUTDOWN;
        int  nRet = mudbus_write_one(255, 99, F_START_SHUTDOWN, 10);   // ��Ĭ��ʼ�� - Silent initialization
        if (nRet >= 0)
        {
            CString srtInfo = _T("Connecting the subdevice , Waiting.");
            pWriter->OutPutsStatusInfo(srtInfo);
            int retry_count = 0;
            do
            {
                srtInfo = srtInfo + _T(".");
                Sleep(3000);
                pWriter->OutPutsStatusInfo(srtInfo, true);
                nflag = mudbus_read_one(255, 99, 3);
                retry_count++;
            } while (((nflag <= 0) || (nflag == F_START_SHUTDOWN)) && (retry_count < 10));


            if (nflag == F_TIMEOUT)
            {
                CString srtInfo = _T("MSTP cannot be disabled on a device in the bus.");
                pWriter->OutPutsStatusInfo(srtInfo);
                nret_value = -1;
                continue;
            }
            else if (nflag == F_SUCCESS)
            {
                CString srtInfo = _T("Preparing firmware update via RS485port.");
                pWriter->OutPutsStatusInfo(srtInfo);
                nret_value = 1;
                return 1;
            }
            else
            {
                CString srtInfo = _T("Send command timeout.");
                pWriter->OutPutsStatusInfo(srtInfo);
                nret_value = -2;
                continue;
            }
        }
        else
        {
            nret_value = -3;
            continue;
        }
    }
    return nret_value;
}

UINT flashThread_ForExtendFormatHexfile_RAM(LPVOID pParam)
{
    CComWriter* pWriter = (CComWriter*)(pParam);
    CString strFailureList;
    CString strTips;
    int nFlashRet=0;
    UINT i=0;
    int nFailureNum = 0;
    UINT times=0;
    const int  RETRY_TIMES = 10;
    CString StrCurrentTime;


    g_mstp_deviceid = 0;
    m_bac_handle_Iam_data.clear();
#if 1

    StrCurrentTime.Format(_T("|>>>StartTime:%s"),GetSysTime());
    pWriter->OutPutsStatusInfo(StrCurrentTime);
    BOOL Flag_HEX_BIN = FALSE;



    for(i = 0; i < pWriter->m_szMdbIDs.size(); i++)
    {
#ifdef ISP_BURNING_MODE
        Check_sum = 0;
#endif
        if (SPECIAL_BAC_TO_MODBUS)
        {
            //����ID�õ���Ӧ�� device id. - Get the corresponding device ID based on ID
            CString strtemp;
            strtemp.Format(_T("|Reading Device Object Instance"));
            pWriter->OutPutsStatusInfo(strtemp);
            strtemp.Format(_T(" "));
            pWriter->OutPutsStatusInfo(strtemp);
            int temp_found_device_id = 0;
            for (int x = 0; x < 30; x++)
            {
                CString temp_found_id = _T("Found MAC ID:");
                Send_WhoIs_Global(-1, -1);
                Sleep(2000);
                if (m_bac_handle_Iam_data.size() > 0)
                {
                    for (int j = 0; j < m_bac_handle_Iam_data.size(); j++)
                    {
                        if (j != 0)
                            temp_found_id = temp_found_id + _T(",");
                        CString temp_id;
                        temp_id.Format(_T("%d"), m_bac_handle_Iam_data.at(j).macaddress);
                        temp_found_id = temp_found_id + temp_id;
                        if (m_bac_handle_Iam_data.at(j).macaddress == pWriter->m_szMdbIDs[i])
                        {
                            temp_found_device_id = m_bac_handle_Iam_data.at(j).device_id;
                            break;
                        }
                    }
                }

                if (temp_found_device_id > 0)
                {
                    g_mstp_deviceid = temp_found_device_id;
                    CString strtemp;
                    strtemp.Format(_T("|ID %d is online.Device instance : %u!"), pWriter->m_szMdbIDs[i], g_mstp_deviceid);
                    pWriter->OutPutsStatusInfo(strtemp);
                    break;
                }
                pWriter->OutPutsStatusInfo(temp_found_id,true);
            }

            if (g_mstp_deviceid == 0)
            {
                CString strtemp;
                strtemp.Format(_T("|ID %d is offline.Update failed!"), pWriter->m_szMdbIDs[i]);
                pWriter->OutPutsStatusInfo(strtemp);
                goto end_isp_flash;
            }
        }

#pragma region mstp_silent_part
        if (pWriter->m_subnet_flash)  //�����flash ���豸 ��������һ��; - If it is a flash sub-device, run this section
        {
            int nret = 0;
            unsigned short temp_register[100] = { 0 };
            nret = modbus_read_multi(255, &temp_register[0], 0, 100, 6);
            if (nret>=0)
            {
                //ֻ�� �ܹ������豸�� �Ÿ� ��Ĭ��һ��; - Only devices that can connect to sub-devices can use this silent method.
                if ((temp_register[7] == PM_MINIPANEL) ||
                    (temp_register[7] == PM_TSTAT10) ||
                    (temp_register[7] == PM_MINIPANEL_ARM) ||
                    (temp_register[7] == PM_ESP32_T3_SERIES))
                {
                    int firmware_ver = temp_register[5] * 10 + temp_register[4];
                    if (firmware_ver >= 636)
                    {
                        int nret1 = fun_shutdown(pWriter,2);
                        if (nret1 < 0)
                        {
                            goto end_isp_flash;
                        }
#pragma region shutdown_function


#if 0
                        int nflag = F_INITIAL;
                        int  nRet = mudbus_write_one(255, 99, F_START_SHUTDOWN, 10);   // ��Ĭ��ʼ�� - Silent initialization
                        if (nRet >= 0)
                        {
                            CString srtInfo = _T("Connecting the subdevice , Waiting.");
                            pWriter->OutPutsStatusInfo(srtInfo);
                            int retry_count = 0;
                            do
                            {
                                srtInfo = srtInfo + _T(".");
                                Sleep(3000);
                                pWriter->OutPutsStatusInfo(srtInfo, true);
                                nflag = mudbus_read_one(255, 99, 3);
                                retry_count++;
                            } while (((nflag <= 0) || (nflag == F_START_SHUTDOWN)) && (retry_count < 10));


                            if (nflag == F_TIMEOUT)
                            {
                                CString srtInfo = _T("MSTP cannot be disabled on a device in the bus.");
                                pWriter->OutPutsStatusInfo(srtInfo);
                                goto end_isp_flash;
                            }
                            else if (nflag == F_SUCCESS)
                            {
                                CString srtInfo = _T("Preparing firmware update via RS485port.");
                                pWriter->OutPutsStatusInfo(srtInfo);
                            }
                            else
                            {
                                CString srtInfo = _T("Send command timeout.");
                                pWriter->OutPutsStatusInfo(srtInfo);
                                goto end_isp_flash;
                            }
                        }
                        else
                        {

                        }
#endif
#pragma endregion
                    }

                }
            }
            else
            {
                CString srtInfo = _T("Reading the device firmware version timed out.");
                pWriter->OutPutsStatusInfo(srtInfo);
                goto end_isp_flash;
            }
        }

#pragma endregion


        while (0)
        {
            Sleep(1000);
           int  nRet = mudbus_read_one(pWriter->m_szMdbIDs[i], 7);
           if (nRet > 0)
           {
               CString strID;
               strID.Format(_T("|Read ID is : %d"), nRet);
               pWriter->OutPutsStatusInfo(strID);
           }
           else
           {
               CString strID;
               strID.Format(_T("|Read Error"));
               pWriter->OutPutsStatusInfo(strID);
           }
            Sleep(1);
        }
        if (pWriter->m_subnet_flash == 0)
             mudbus_write_one(255, 16, 0x0455, 3);
        Sleep(100);
       //for (int Time =0; Time < pWriter->m_FlashTimes; Time++)
        {
            CString strID;
            strID.Format(_T("|Current Programming device ID is : %d"), pWriter->m_szMdbIDs[i]);
            pWriter->OutPutsStatusInfo(strID);

            BOOL Flag_HEX_BIN=FALSE;
            int nret_device_info = pWriter->UpdataDeviceInformation(pWriter->m_szMdbIDs[i]);
            int temp_ret3 = pWriter->Fix_Tstat10_76800_baudrate();
            if (nret_device_info == 1)
            {
                Flag_HEX_BIN =TRUE;
                int  nRet = mudbus_write_one(pWriter->m_szMdbIDs[i],16,127,3);   // ����ISPģʽ - Enter ISP mode
                if (temp_ret3 == 1) //����76800 tstat10 ��ת��BootLoader�� ������BootLoader�����⣬�޸�Ϊ57600�����ʺ�ͨѶ  // It's 76800 tstat10 jumping to BootLoader, and BootLoader has problems, modified to 57600 baud rate for communication
                {
                    CString strtempnotice;
                    strtempnotice.Format(_T("The actual baud rate in ISP mode is 57600"));
                    pWriter->OutPutsStatusInfo(strtempnotice);
                    strtempnotice.Format(_T("Temporarily switch the baud rate to 57600"));
                    pWriter->OutPutsStatusInfo(strtempnotice);
                    Change_BaudRate(57600);
                }
                Sleep (2000);
                nRet = mudbus_read_one(pWriter->m_szMdbIDs[i],11);
				//Wait for the device to enter the ISP mode, some devices jump slower than others, can��t read after reboot, retry;
                if (nRet <= 0)
                {
                    bool read_bootloader_ret = false;
                    for (int x = 0; x < 10; x++)
                    {
                        Sleep(2000);
                        nRet = mudbus_read_one(pWriter->m_szMdbIDs[i], 11);
                        if (nRet <= 0)
                        {
                            CString srtInfo;
                            srtInfo.Format(_T("|Firmware update is being prepared, please wait! (%d)!"), x + 1);
                            pWriter->OutPutsStatusInfo(srtInfo);
                            continue;
                        }
                        else
                        {
                            CString srtInfo;
                            read_bootloader_ret = true;
                            srtInfo.Format(_T("|Reading bootloader version OK,ready to update!"));
                            pWriter->OutPutsStatusInfo(srtInfo);
                            break;
                        }
                    }
#if 0
					 Sleep(3000);
					 nRet = Read_One(pWriter->m_szMdbIDs[i],11);
					 if(nRet<= 0)
					 {
                         Sleep(3000);
                         nRet = Read_One(pWriter->m_szMdbIDs[i], 11);
                         if (nRet <= 0)
                         {
                             CString srtInfo = _T("|Failed read bootloader version...continue update!");
                             pWriter->OutPutsStatusInfo(srtInfo);
                         }
						 //if(!auto_flash_mode)
							// AfxMessageBox(_T("Failed to enter ISP Mode!"));
						 goto end_isp_flash;
					 }
#endif
                }

       //         if (nRet >=41)//֧�ֶ���������л��� - Support for switching between multiple baud rates
       //         {
       //             if (GetCommunicationType () == 0)
       //             {
       //                 nRet = Write_One (pWriter->m_szMdbIDs[i],15,4); //Switch the main program baud rate to 115200
       //                 close_com ();
       //                 if(open_com(pWriter->m_nComPort)==false)
       //                 {
       //                     return 0;
       //                 }
       //                 else
       //                 {
       //                     bool is_ok =  Change_BaudRate (115200);
							//SetResponseTime(10);
       //                 }
       //                 // Change_BaudRate (115200);
       //             }

       //         }
				 
              //  Sleep (500);--

                int m_ID=pWriter->m_szMdbIDs[i];

              //  Sleep(2000);--


#if 1
                //*************inspect the flash that last flash position ***********************
                int ii=0;
                while(mudbus_read_one(m_ID,0xee10,1)<0) //the return value == -1 ,no connecting
                {
                    if(ii<5)
                    {
                        ii++;
                        continue;
                    }
                    else
                    {
                        ii=0;
                        break;
                    }//error -1
                }
                if (ii >= 5)
                {
					if(!auto_flash_mode)
					 AfxMessageBox(_T("No connection!"));
                    goto end_isp_flash;

                }
                //************* begin to flash ***********************
                ii=0;
                if(mudbus_read_one(m_ID,0xee10)==0x40 || mudbus_read_one(m_ID,0xee10)==0x1f) // ��ee10�� why��
                {
                    if(IDOK==AfxMessageBox(_T("Previous Update was interrupted.\nPress OK to Resume.\nCancel to Restart."),MB_OKCANCEL))  // Select OK
                    {
                        ii=0xEE00+17;
                        int l=0;//temp;<200
                        do
                        {
                            int uc_temp1= mudbus_read_one(m_ID,ii);
                            int uc_temp2= mudbus_read_one(m_ID,ii+1);
                            if(uc_temp1==0x00 && uc_temp2==0x00 )
                                ii+=2;
                            else if(l==0)
                            {
                                if(uc_temp1==0xf0 && uc_temp2==0xf0)
                                {
                                    ii=0;
                                    break;
                                }
                            }
                            else
                            {
                                ii=uc_temp1*256+uc_temp2;
                                break;
                            }
                            l++;
                        }
                        while(l<200);
                    }
                    else // ѡȡ�� - Select Cancel
                    {
                        //from 0000 flash update
                        ii=0;//from 0000 register flash
                        CString srtInfo = _T("|Initializing device...");
                        pWriter->OutPutsStatusInfo(srtInfo);
                        //********************write register 16 value 0x7f **************
                        Sleep(500);
                        do
                        {
                            if(ii<RETRY_TIMES)
                                if(-2== mudbus_write_one(m_ID,16,0x7f))
                                {
                                    ii++;
                                    Sleep(6000);
                                }
                                else
                                {
                                    ii=0;//error -2 Unable to Initialize...
                                    break;
                                }
                        }
                        while(ii);

                        if (ii >= RETRY_TIMES)
                        {

                            AfxMessageBox(_T("Write 16 = 0x7f Failed"));
                            goto end_isp_flash;

                        }

                        //********************write register 16 value 0x3f **************
                        ii=0;
                        srtInfo = _T("|Erasing device...");
                        pWriter->OutPutsStatusInfo(srtInfo);
                        do
                        {
                            if(ii<RETRY_TIMES)
                            {
                                if(-2== mudbus_write_one(m_ID,16,0x3f))
                                {
                                    ii++;
                                }

                            }
                            else
                            {
                                ii=0;
                                break;
                                //return -3;//error -3
                            }
                        }
                        while(ii);
                        if (ii >= RETRY_TIMES)
                        {
							 if(!auto_flash_mode)
								AfxMessageBox(_T("Write 16 = 0x3f Failed"));
                            goto end_isp_flash;

                        }
                        //********************write register 16 value 0x1f **************
                        ii=0;
                        Sleep(7000);//must have this ,the Tstat need
                        do
                        {
                            if(ii<RETRY_TIMES)
                                if(-2== mudbus_write_one(m_ID,16,0x1f))
                                    ii++;

                                else
                                {
                                    ii=0;
                                    break;
                                    //return -4;//error -4
                                }
                        }
                        while(ii);

                        if (ii >= RETRY_TIMES)
                        {
							if(!auto_flash_mode)
								 AfxMessageBox(_T("Write 16 = 0x1f Failed"));
                            goto end_isp_flash;

                        }

                        //***************send data to com*************************
                        ii=0;//to the register 0000
                    }
                }
                else  // Read Eeprom chip failed��
                {
                    //from 0000 flash update
                    ii=0;//from 0000 register flash
                    CString srtInfo = _T("|Initializing device...");
                    pWriter->OutPutsStatusInfo(srtInfo);
                    //writing_row++;
                    Sleep(500);

                    //********************write register 16 value 0x7f **************
                    do
                    {
                        if(ii<3)

                        {
                            if(mudbus_write_one(m_ID,16,0x7f)<0)  //T3ϵ�е� �ǲ���ظ�������� ��T3 ֱ�Ӿ͸�λ��. - The T3 series will not respond to this command, and T3 will reset directly.
                            {
                                ii++;
                                continue;
                            }
                            else
                            {
                                ii=0;
                                break;
                                //return -5;//error -5
                            }
                        }

                        else
                        {
                            ii=0;
                            break;
                            //return -5;//error -5
                        }


                    }
                    while(ii<RETRY_TIMES);

                    //********************write register 16 value 0x3f **************
                    ii=0;
                    srtInfo = _T("|Erasing device";);
                    pWriter->OutPutsStatusInfo(srtInfo);
                    //writing_row++;
                    do
                    {
                        if(ii<3)
                        {
                            //if(-2==Write_One(m_ID,16,0x3f))
                            if(mudbus_write_one(m_ID,16,0x3f)<0)
                            {
                                ii++;
                                continue;
                            }
                            else
                            {
                                ii=0;
                                break;
                                //return -5;//error -5
                            }
                        }
                        else
                        {
                            ii=0;
                            break;
                            //return -6;//error -6
                        }
                    }
                    while(ii<RETRY_TIMES);
                    //********************write register 16 value 0x1f **************
                    ii=0;

                    CString temp_net;
                    temp_net = _T(".");
                    srtInfo = _T("|Erasing device";);
                    int delay_count = 14;
                    if ((Device_infor[7] >= 210) && (Device_infor[7] <= 216))
                    {
                        delay_count = 4;
                    }
                    for (size_t x = 0; x < delay_count; x++)
                    {
                        srtInfo = srtInfo + _T(".");                       
                        pWriter->OutPutsStatusInfo(srtInfo,1);
                        Sleep(500);
                    }
                    //Sleep(7000);//must have this ,the Tstat need

                    int Cpu_Type = mudbus_read_one(m_ID, 65010, 3);
                    if ((Cpu_Type == 1) || (Cpu_Type == 2))
                    {
                        com_error_delay_time = 3000;
                        com_error_delay_count = 10;
                    }
                    else
                    {
                        com_error_delay_time = 300;
                        com_error_delay_count = 0;
                    }

                    do
                    {
                        if(ii<RETRY_TIMES)
                        {
                            //if(-2==Write_One(m_ID,16,0x1f))
                            if(mudbus_write_one(m_ID,16,0x1f)<0)
                            {
                                ii++;
                                continue;
                            }
                            else
                            {
                                ii=0;
                                break;
                                //return -5;//error -5
                            }
                        }
                        else
                        {
                            ii=0;
                            break;
                            //return -5;//error -5
                        }


                    }
                    while(ii<RETRY_TIMES);

                    //***************send data to com*************************
                    ii=0;//to the register 0000
                } // Read Eeprom chip done

#endif

            }
            else if (nret_device_info == 2)
            {
                Sleep(1);
                return 1;
            }
            else if (nret_device_info == -1)
            {
                CString srtInfo;
                srtInfo.Format(_T("The new firmware needs update bootloader first."));
                pWriter->OutPutsStatusInfo(srtInfo);
                srtInfo.Format(_T("Do not support update bootloader through serial port."));
                pWriter->OutPutsStatusInfo(srtInfo);
                srtInfo.Format(_T("Please use the network to update the firmware"));
                pWriter->OutPutsStatusInfo(srtInfo);
                srtInfo.Format(_T(" "));
                pWriter->OutPutsStatusInfo(srtInfo);
                continue;
            }
            else
            {
                continue;
            }
            if (!Flag_HEX_BIN)
            {
                continue;
            }

            if (pWriter->m_subnet_flash == 0)
            {
                mudbus_write_one(255, 16, 0x0455, 3);  //ȫ�ֹ㲥  0x04 ���� 4����   ������ mstp���豸������Ĭ - Global broadcast 0x04 represents 4 minutes, allowing all MSTP devices to continue silent
                Sleep(100);
            }

            the_max_register_number_parameter_Count=pWriter->m_szHexFileFlags[pWriter->m_szHexFileFlags.size()-1];
            the_max_register_number_parameter_Finished = 0;

            int nCount = 0;
            UINT p = 0;
            for(  p = 0; p < pWriter->m_szHexFileFlags.size(); p++)
            {
                int nBufLen = pWriter->m_szHexFileFlags[p]-nCount;

                if((nFlashRet = flash_a_tstat_RAM(pWriter->m_szMdbIDs[i],p, nBufLen, (TS_UC*)(pWriter->m_pExtendFileBuffer+nCount), pParam)) < 0 )
                {
                    nFailureNum++;
                    CString strTemp=_T("");
                    strTemp.Format(_T("%d;"), pWriter->m_szMdbIDs[i]);

                    strFailureList+=strTemp;  // flash Multiple use
                    switch(nFlashRet)
                    {
                    case -1:
                        strTemp.Format(_T("|ID %d Error : Please verify connection!"), pWriter->m_szMdbIDs[i]);
                        break;
                    case -2:
                        strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -3:
                        strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -4:
                        strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -5:
                        strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -6:
                        strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -7:
                        strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -8:
                        strTemp.Format(_T("|ID %d Error : Update was interrupted! Please verify connection."), pWriter->m_szMdbIDs[i]);
                        break;
                    case -9:
                        strTemp.Format(_T("|ID %d Error : Programming was canceled."), pWriter->m_szMdbIDs[i]);
                        break;
                    }
                    pWriter->OutPutsStatusInfo(strTemp);
                    Sleep(1000);
                    goto end_isp_flash;

                }
                else
                {


                    nCount += nBufLen;
                    the_max_register_number_parameter_Finished=nCount;

                }

                Sleep(500); //Must delay

            }



#ifdef ISP_BURNING_MODE
            CString strTips_checksum;
            strTips_checksum.Format(_T("Serial Port CheckSum : 0x%04x"), Check_sum);
            pWriter->OutPutsStatusInfo(strTips_checksum, FALSE);
#endif

            int ii=0;
            do
            {

                if(mudbus_write_one(pWriter->m_szMdbIDs[i],16,1)<0)
                {
                    ii++;
                }


                else
                {
                    ii=0;
                    break;
                    //return -8;//error -8
                }
            }
            while(ii<RETRY_TIMES);
#ifdef ISP_BURNING_MODE
            unsigned short temp_check_sum[4] ;
            int nret = modbus_read_multi(pWriter->m_szMdbIDs[i], &temp_check_sum[0], 28, 4,5);
            if (nret >= 0)
            {
                CString srtInfoSum;
                srtInfoSum.Format(_T("Read Device CheckSum : 0x%04x"), temp_check_sum[0]);
                pWriter->OutPutsStatusInfo(srtInfoSum);
            }
            else
            {
                CString srtInfoSum;
                srtInfoSum.Format(_T("Read Device CheckSum : Timeout"));
                pWriter->OutPutsStatusInfo(srtInfoSum);
            }

            if (temco_burning_mode)
            {
                static int total_count = 0;
                static int total_error = 0;
                static int total_success = 0;
                total_count++;
                if (temp_check_sum[0] != Check_sum)
                {
                    total_error++;
                }
                else
                {
                    total_success++;
                }
                CString srtInfoRet;
                srtInfoRet.Format(_T("Success %d ,error %d , total %d"), total_success, total_error, total_count);
                pWriter->OutPutsStatusInfo(srtInfoRet);
                for (int z = 0; z < 15; z++)
                {
                    Sleep(1000);
                    CString temp_info;
                    temp_info.Format(_T("Wait for device jump to normal mode!(%d)"), 15 - z);
                    if(z == 0)
                        pWriter->OutPutsStatusInfo(temp_info,0);
                    else
                        pWriter->OutPutsStatusInfo(temp_info, 1);
                }
                
            }
#endif

            Sleep (6000);

            //if (GetCommunicationType () == 0)
            //{
            //    if (pWriter->m_index_Baudrate>=0)
            //    {
            //        Write_One (pWriter->m_szMdbIDs[i],15,pWriter->m_index_Baudrate); //Reduce the current baud rate
            //    }
            //}


            if(nFlashRet > 0) // flash �ɹ� - Flash successful
            {
                CString strText;
                strText.Format(_T("|ID %d: Programming successful."), pWriter->m_szMdbIDs[i]);
                pWriter->OutPutsStatusInfo(strText);
                StrCurrentTime.Format(_T("|>>>EndTime:%s"),GetSysTime());
                pWriter->OutPutsStatusInfo(StrCurrentTime);
            }
        }


    }

#endif


//*******************************************************************************
end_isp_flash:
    Sleep(500);
    //pWriter->WriteFinish(nFlashRet);
    if (new_bootload == 1)
    {
        PostMessage(pWriter->m_pParentWnd->m_hWnd, WM_FLASH_NEW_BOOT_FINISH, 0, LPARAM(nFlashRet));
    }
    else
    {
        pWriter->WriteFinish(nFlashRet);
    }
    Sleep(500);

    close_com();
    return 1;//close thread
//	}while(1);


}


