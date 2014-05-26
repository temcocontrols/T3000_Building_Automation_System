#include "StdAfx.h"
#include "ComWriter.h"
#include "ISPDlg.h"


extern CString g_strFlashInfo;

UINT run_back_ground_flash_thread(LPVOID pParam);
UINT flashThread_ForExtendFormatHexfile(LPVOID pParam);

int flash_a_tstat(BYTE m_ID, unsigned int the_max_register_number_parameter, TS_UC *register_data_orginal, LPVOID pParam);
CComWriter::CComWriter(void)
{
	m_pFileBuffer = NULL;
	m_nBufLen = 0;
	
	m_nComPort = -1;
	//m_nModbusID;
	m_nBautrate = 0;			// 波特率

	m_pWorkThread = NULL;

	m_bStopWrite = FALSE;

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
		if (!WriteCommandtoReset())
		{
			return 0;
		}
	

		HCURSOR hc;//load mouse cursor
		hc = LoadCursor(NULL,IDC_WAIT);
		hc = SetCursor(hc);
		
		if(open_com(m_nComPort)==false)
		{
			CString srtInfo = _T("|Error :The com port is occupied!");
			//MessageBox(NULL, srtInfo, _T("ISP"), MB_OK);
			//AddStringToOutPuts(_T("Error :The com port is occupied!"));	
			OutPutsStatusInfo(srtInfo, FALSE);
			return 0;
		}
		else
		{
			CString strTemp;
			strTemp.Format(_T("COM%d"), m_nComPort);
			CString strTips = _T("|Open ") +  strTemp + _T(" successful.");
			OutPutsStatusInfo(strTips, FALSE);
			// AddStringToOutPuts(strTips);

		}

				
		CString strTips = _T("|Programming device...");
		OutPutsStatusInfo(strTips);
		//AddStringToOutPuts(strTips);
		m_pWorkThread=AfxBeginThread(run_back_ground_flash_thread, this); //create thread,read information	
		
		ASSERT(m_pWorkThread);
	}

	else if(m_nHexFileType == 2)
	{
		WirteExtendHexFileByCom();
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
		//AfxMessageBox(strTips);
		return FALSE;
	}

	int nRet = Write_One(m_szMdbIDs[0],16,127);   // 进入ISP模式
	Sleep(2000);
	int ModelID= read_one(m_szMdbIDs[0],7,5);
	if (ModelID>0)
	{
		if (ModelID==6||ModelID==7)//Tstat6,7检测芯片大小，其余用串口烧写的都不检测
		{
			int Chipsize=read_one(m_szMdbIDs[0],11,5);


			if (Chipsize<37)	//64K
			{
				if (m_nHexFileType==0)
				{
					strTips = _T("|hex file   matches with the chip!");
					OutPutsStatusInfo(strTips);
				} 
				else
				{
					int ii=0;
					while(ii<=5){
						int ret=Write_One(m_szMdbIDs[0],16,1);
						if (ret>0)
						{
							break;
						}
					}
					strTips = _T("|hex file doesn't match with the chip!");
					OutPutsStatusInfo(strTips);	
					AfxMessageBox(_T("hex file doesn't match with the chip"));
					return FALSE;
				}
			} 
			else	//128K
			{
				if (m_nHexFileType==0)
				{
					int ii=0;
					while(ii<=5){
						int ret=Write_One(m_szMdbIDs[0],33,1);
						if (ret>0)
						{
							break;
						}
					}

					while(ii<=5){
						int ret=Write_One(m_szMdbIDs[0],16,1);
						if (ret>0)
						{
							break;
						}
					}
					strTips = _T("|hex file doesn't match with the chip!");
					OutPutsStatusInfo(strTips);	
					AfxMessageBox(_T("hex file doesn't match with the chip"));
					return FALSE;
				} 
				else
				{
					strTips = _T("|hex file   matches with the chip!");
					OutPutsStatusInfo(strTips);
				}
			}
		}
	}




	close_com();
	//Sleep(1000);

	return TRUE;
}



UINT run_back_ground_flash_thread(LPVOID pParam)
{
	CComWriter* pWriter = (CComWriter*)(pParam);
	CString strFailureList;
	//	do
	//	{
	int nFlashRet=0;
	UINT i=0;
	int nFailureNum = 0;


	if(GetCommunicationType()==1)
	{
		if(Open_Socket2(pWriter->m_strIPAddr, pWriter->m_nIPPort)==false)
		{
			CString srtInfo = _T("|Error : Network init failed.");
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
			CString strTips = _T("|Connect to ") +  pWriter->m_strIPAddr + _T(" successful.");
			pWriter->OutPutsStatusInfo(strTips, FALSE);
			// AddStringToOutPuts(strTips);
		}

		int nRet = Write_One(pWriter->m_szMdbIDs[0],16,127);   // 进入ISP模式
		if(nRet<0)
		{
			CString strTips_isp = _T("Enter ISP Mode fail,please check the connection.");
			pWriter->OutPutsStatusInfo(strTips_isp);
			pWriter->WriteFinish(0);
			return 0;
		}
		CString strTips = _T("|Programming device...");
		pWriter->OutPutsStatusInfo(strTips);
	}
	
	for(i = 0; i < pWriter->m_szMdbIDs.size(); i++)
	{    

		CString strID;
		strID.Format(_T("|--------------->>ID-%d-<<---------------"), pWriter->m_szMdbIDs[i]);
		pWriter->OutPutsStatusInfo(strID);
		////显示flash之前的时间
		pWriter->OutPutsStatusInfo(_T("|--------->>Begin"));
		pWriter->OutPutsStatusInfo(_T("|-->>Begin Time:")+GetSysTime());
		//// 显示flash之前的设备状态信息

//		((CISPDlg*)pWriter->m_pParentWnd)->Show_Flash_DeviceInfor(pWriter->m_szMdbIDs[i]);
        if (!pWriter->UpdataDeviceInformation(pWriter->m_szMdbIDs[i]))
        {
			 
           pWriter->OutPutsStatusInfo(_T("The device can't match with the hex"));
#if 1		//复位
		   int Chipsize=read_one(pWriter->m_szMdbIDs[i],11,5);
		   if (Chipsize<37)	//64K
		   {
			    
			    
				   int ii=0;
				   while(ii<=5){
					   int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
					   if (ret>0)
					   {
						   break;
					   }
				   }
				 
			   
		   } 
		   else	//128K
		   {
			    
				   int ii=0;
				   while(ii<=5){
					   int ret=Write_One(pWriter->m_szMdbIDs[i],33,1);
					   if (ret>0)
					   {
						   break;
					   }
				   }

				   while(ii<=5){
					   int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
					   if (ret>0)
					   {
						   break;
					   }
				   }
				  
			   
			 
		   }
#endif



		   continue;
        }
         

		
		////////	ASSERT(pWriter->m_nBufLen <= c_nHexFileBufLen);
		//在此处进行数据库校验
		//int result=((CISPDlg*)pWriter->m_pParentWnd)->Judge_Model_Version();
		//if(result==OK_HEXFILEPREFIX)	 //验证成功
		//{

		//pWriter->OutPutsStatusInfo(_T("|Verifying successfully!")); 
		//pWriter->OutPutsStatusInfo(_T("|Your file matchs with the flashing device."));

		//} 
		//else if(result==ERR_HEXFILEPREFIX)	 //验证失败
		//{
		//pWriter->OutPutsStatusInfo(_T("|Verifying unsuccessfully!")); 
		//pWriter->OutPutsStatusInfo(_T("|Your file name can't matchs with the flashing device."));
		//continue;	
		//}
		//else					   //数据库中是空的，没有查到响应的prefix
		//{	pWriter->OutPutsStatusInfo(_T("|Your device information is not in Database!")); 
		//pWriter->OutPutsStatusInfo(_T("|Directly flash your device."));
		//}
		

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

					strFailureList+=strTemp;  // flash多个使用
					switch(nFlashRet)
					{
					case -1:strTemp.Format(_T("|ID %d Error : Please verify connection!"), pWriter->m_szMdbIDs[i]);break;
					case -2:strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);break;
					case -3:strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);break;
					case -4:strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);break;
					case -5:strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);break;
					case -6:strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);break;
					case -7:strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);break;
					case -8:strTemp.Format(_T("|ID %d Error : Update was interrupted! Please verify connection."), pWriter->m_szMdbIDs[i]);break;
					case -9:strTemp.Format(_T("|ID %d Error : Programming was canceled."), pWriter->m_szMdbIDs[i]);break;
					}
					pWriter->OutPutsStatusInfo(strTemp);
					AfxMessageBox(strTemp);
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
					//// 显示flash之前的设备状态信息
					Sleep(13000);//等待重启好之后
//					((CISPDlg*)pWriter->m_pParentWnd)->Show_Flash_DeviceInfor(pWriter->m_szMdbIDs[i]);
					pWriter->OutPutsStatusInfo(_T("|-->>End Time:")+GetSysTime());
					pWriter->OutPutsStatusInfo(_T("|------->>End"));
					AfxMessageBox(strText);				
				}


				if (p==0)
				{	
					int nTemp = Write_One(pWriter->m_szMdbIDs[i], 16, 8);
					nTemp = Write_One(pWriter->m_szMdbIDs[i], 33, 1);

				}


				Sleep(1000); //must if not ,have some wrong
			}

		}
		else
		{
			if((nFlashRet = flash_a_tstat(pWriter->m_szMdbIDs[i], pWriter->m_nBufLen, (TS_UC*)pWriter->m_pFileBuffer, pParam)) < 0 )
			{
				nFailureNum++;
				CString strTemp=_T("");
				strTemp.Format(_T("%d;"), pWriter->m_szMdbIDs[i]);  

				strFailureList+=strTemp;  // flash多个使用
				switch(nFlashRet)
				{
				case -1:strTemp.Format(_T("|ID %d Error : Please verify connection!"), pWriter->m_szMdbIDs[i]);break;
				case -2:strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);break;
				case -3:strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);break;
				case -4:strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);break;
				case -5:strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);break;
				case -6:strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);break;
				case -7:strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);break;
				case -8:strTemp.Format(_T("|ID %d Error : Update was interrupted! Please verify connection."), pWriter->m_szMdbIDs[i]);break;
				case -9:strTemp.Format(_T("|ID %d Error : Programming was canceled."), pWriter->m_szMdbIDs[i]);break;
				}
				pWriter->OutPutsStatusInfo(strTemp);
				pWriter->OutPutsStatusInfo(_T("|-->>End Time:")+GetSysTime());
				pWriter->OutPutsStatusInfo(_T("|------->>End"));
				AfxMessageBox(strTemp);
			}
			else
			{		
				CString strText;
				strText.Format(_T("|ID %d: Programming successful."), pWriter->m_szMdbIDs[i]);
				pWriter->OutPutsStatusInfo(strText);
				//// 显示flash之前的设备状态信息
				//Sleep(13000);//等待重启好之后
//				((CISPDlg*)pWriter->m_pParentWnd)->Show_Flash_DeviceInfor(pWriter->m_szMdbIDs[i]);
				pWriter->OutPutsStatusInfo(_T("|-->>End Time:")+GetSysTime());
				pWriter->OutPutsStatusInfo(_T("|------->>End"));
				//AfxMessageBox(strText);				
			}
		}
	}
 
	//}//For Test
	//*******************************************************************************
	//Sleep(500);
	pWriter->WriteFinish(nFlashRet);
	close_com();
	return 1;//close thread
	//	}while(1);


}

//////////////////////////////////////////////////////////////////////////
//the return value 1,successful,   return < 0 ,have some trouble
int flash_a_tstat(BYTE m_ID, unsigned int the_max_register_number_parameter, TS_UC *register_data_orginal, LPVOID pParam)
{
	CComWriter* pWriter = (CComWriter*)pParam;
	const int  RETRY_TIMES = 5;
	TS_UC *register_data=register_data_orginal;
	unsigned int ii=0;
	
	//*************inspect the flash that last flash position ***********************
	while(Read_One(m_ID,0xee10)<0) //the return value == -1 ,no connecting
	{     
		//AfxMessageBox("before write,need to open the com!");
		//close_com();
		if(ii<5)
		{
			ii++;
			continue;
		}
		return -1;//error -1
	}
	
	//************* begin to flash ***********************
	ii=0;
	if(Read_One(m_ID,0xee10)==0x40 || Read_One(m_ID,0xee10)==0x1f) // 读ee10， why？
	{
		if(IDOK==AfxMessageBox(_T("Previous Update was interrupted.\nPress OK to Resume.\nCancel to Restart."),MB_OKCANCEL))
		{ // 选确定
			ii=0xEE00+17;
			int l=0;//temp;<200
			do{
					int uc_temp1=Read_One(m_ID,ii);
					int uc_temp2=Read_One(m_ID,ii+1);
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
			}while(l<200);
		}
		else // 选取消
		{	
			//from 0000 flash update
			ii=0;//from 0000 register flash			
			CString srtInfo = _T("|Initializing device...");
			pWriter->OutPutsStatusInfo(srtInfo);
			//********************write register 16 value 0x7f **************
			Sleep(500);
			do{
				if(ii<RETRY_TIMES)
					if(-2==Write_One(m_ID,16,0x7f))
					{
						ii++;
						Sleep(6000);
					}
					else
						ii=0;
				else
				{
					//AfxMessageBox("Unable to Initialize...");
					//close_com();
					return -2;//error -2 Unable to Initialize...
				}
			}while(ii);

			//********************write register 16 value 0x3f **************
			ii=0;
			srtInfo = _T("|Erasing device...");
			pWriter->OutPutsStatusInfo(srtInfo);
			do{
				if(ii<RETRY_TIMES)
				{
					if(-2==Write_One(m_ID,16,0x3f))
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
						//AfxMessageBox("Unable to Erase...");
						//close_com();
						return -3;//error -3
				}
			}while(ii);
					
			//********************write register 16 value 0x1f **************
			ii=0;
			Sleep(7000);//must have this ,the Tstat need			
			do{
				if(ii<RETRY_TIMES)
					if(-2==Write_One(m_ID,16,0x1f))
						ii++;
					else
						ii=0;
				else
				{
						//AfxMessageBox("Unable to start programming...");
						//close_com();
						return -4;//error -4
				}
			}while(ii);
			//***************send data to com*************************
			ii=0;//to the register 0000
		}
	}
	else  // 读ee10 失败？
	{
		//from 0000 flash update
		ii=0;//from 0000 register flash
		CString srtInfo = _T("|Initializing device...");
		pWriter->OutPutsStatusInfo(srtInfo);
		//writing_row++;
		Sleep(500);

		//********************write register 16 value 0x7f **************
		do{
			if(ii<RETRY_TIMES)
				//if(-2==Write_One(m_ID,16,0x7f))
				if(Write_One(m_ID,16,0x7f)<0)
				{
						ii++;
				}
				else
						ii=0;
			else
			{
				//AfxMessageBox("Unable to Initialize...");
				//close_com();
				return -5;//error -5
			}
		}while(ii);
		
		//********************write register 16 value 0x3f **************
		ii=0;
		srtInfo = _T("|Erasing device...";);
		pWriter->OutPutsStatusInfo(srtInfo);
		//writing_row++;
		do{
				if(ii<RETRY_TIMES)
					//if(-2==Write_One(m_ID,16,0x3f))
					if(Write_One(m_ID,16,0x3f)==-2)
					{
							ii++;
					}
					else
							ii=0;
				else
				{
					//AfxMessageBox("Unable to Erase...");
					//close_com();
					return -6;//error -6
				}
			}while(ii);
			//********************write register 16 value 0x1f **************
			ii=0;
			Sleep(7000);//must have this ,the Tstat need			
	
			do{
				if(ii<RETRY_TIMES)
					//if(-2==Write_One(m_ID,16,0x1f))
					if(Write_One(m_ID,16,0x1f)<0)
						ii++;
					else
						ii=0;
				else
				{
					//AfxMessageBox("Unable to start programming...");
					//close_com();
					return -7;//error -7
				}
			}while(ii);

			//***************send data to com*************************
			ii=0;//to the register 0000
	} // 读ee10 的处理结束

	int persentfinished=0;
	CString srtInfo;
	srtInfo.Format(_T("|ID %d: Programming lines %d to %d.(0%%)"),m_ID,ii,ii+128);
	pWriter->OutPutsStatusInfo(srtInfo);
//	while(ii<=the_max_register_number_parameter)
	while(ii<the_max_register_number_parameter)
	{
		if (pWriter->m_bStopWrite)
		{
			return -9;
		}
		TS_UC data_to_send[160]={0};// buffer that writefile() will to use
		int itemp=0;

		persentfinished = ((ii+128)*100)/the_max_register_number_parameter;
		if(persentfinished>100)
			persentfinished=100;
		CString srtInfo;
		srtInfo.Format(_T("|ID %d: Programming lines %d to %d.(%d%%)"),m_ID,ii,ii+128,persentfinished);
		pWriter->OutPutsStatusInfo(srtInfo, TRUE);
		do{
			if(itemp<RETRY_TIMES)
			{
			    
				if(-2==write_multi(m_ID,&register_data[ii],ii,128))//to write multiple 128 bytes
					itemp++;
				else
					itemp=0;
			}
			else
			{
				//srtInfo.Format(_T("Communication was interrupted.Tryiny connect agina!"));
				//pWriter->OutPutsStatusInfo(srtInfo, TRUE);
				Sleep(5000);//如果 5次失败，则 等待5s后在开始继续续传2次。

				if(itemp<RETRY_TIMES+3)
				{
					if(-2==write_multi(m_ID,&register_data[ii],ii,128))//to write multiple 128 bytes
						itemp++;
					else
						itemp=0;
				}
				else
				{
					//AfxMessageBox("update was interrupted!\nPlease verify connection.");
					//close_com();
					return -8;//the com connection is wrong! error -8
				}
				
			}
		}while(itemp);
		/*CString temp,aline;
		for (int i=ii;i<ii+128;i++)
		{
		temp.Format(_T("%X"),register_data[i]);
		aline+=temp;
		}
		aline+=_T("\n");
		pWriter->OutPutsStatusInfo(aline,FALSE);*/
		ii+=128;
	}

	//********************write register 16 value 0x01 **************
	ii=0;
	do{
		if(ii<RETRY_TIMES)
		{
			if(-2==Write_One(m_ID,16,1))
				ii++;
			else
				ii=0;
		}
		else
		{
			return -8;//error -8
		}
	}while(ii);					
	
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
		//AfxMessageBox(_T("Thread has been stoped!"), MB_OK);
		//return TRUE;
	}
	else
	{
		BOOL bRet = TerminateThread(m_pWorkThread->m_hThread, 0);
		//AfxMessageBox(_T("Thread has been terminated!"), MB_OK);
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
// flash 完了，不论成功还是失败，都通知父窗口
// 参数就是flash线程的返回值
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
	WriteCommandtoReset();

	HCURSOR hc;//load mouse cursor
	hc = LoadCursor(NULL,IDC_WAIT);
	hc = SetCursor(hc);

	if(open_com(m_nComPort)==false)
	{
		CString srtInfo = _T("|Error :The com port is occupied!");
		MessageBox(NULL, srtInfo, _T("ISP"), MB_OK);
		//AddStringToOutPuts(_T("Error :The com port is occupied!"));	
		OutPutsStatusInfo(srtInfo, FALSE);
		return 0;
	}
	else
	{
		CString strTemp;
		strTemp.Format(_T("COM%d"), m_nComPort);
		CString strTips = _T("|Open ") +  strTemp + _T(" successful.");
		OutPutsStatusInfo(strTips, FALSE);
		// AddStringToOutPuts(strTips);

	}


	CString strTips = _T("|Programming device...");
	OutPutsStatusInfo(strTips);
	//AddStringToOutPuts(strTips);
	m_pWorkThread=AfxBeginThread(flashThread_ForExtendFormatHexfile, this); //create thread,read information	

	ASSERT(m_pWorkThread);

	return 1;
}




UINT flashThread_ForExtendFormatHexfile(LPVOID pParam)
{
	CComWriter* pWriter = (CComWriter*)(pParam);
	CString strFailureList;
	int nFlashRet=0;
	UINT i=0;
	int nFailureNum = 0;
  
	UINT times=0;

   #if 1

	   for(i = 0; i < pWriter->m_szMdbIDs.size(); i++)
	   {
		   CString strID;
		   strID.Format(_T("|Current Programming device ID is : %d"), pWriter->m_szMdbIDs[i]);
		   pWriter->OutPutsStatusInfo(strID);
		 //  pWriter->UpdataDeviceInformation(pWriter->m_szMdbIDs[i]);
           if (!pWriter->UpdataDeviceInformation(pWriter->m_szMdbIDs[i]))
           {
              // pWriter->OutPutsStatusInfo(_T("The device can't match with the hex"));
#if 1		//复位
			   int Chipsize=read_one(pWriter->m_szMdbIDs[i],11,5);
			   if (Chipsize<37)	//64K
			   {


				   int ii=0;
				   while(ii<=5){
					   int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
					   if (ret>0)
					   {
						   break;
					   }
				   }


			   } 
			   else	//128K
			   {

				   int ii=0;
				   while(ii<=5){
					   int ret=Write_One(pWriter->m_szMdbIDs[i],33,1);
					   if (ret>0)
					   {
						   break;
					   }
				   }

				   while(ii<=5){
					   int ret=Write_One(pWriter->m_szMdbIDs[i],16,1);
					   if (ret>0)
					   {
						   break;
					   }
				   }



			   }
#endif
			 
               continue;
           }
		   int nCount = 0;
		   for(UINT p = 0; p < pWriter->m_szHexFileFlags.size(); p++)
		   {
			   int nBufLen = pWriter->m_szHexFileFlags[p]-nCount;

			   if((nFlashRet = flash_a_tstat(pWriter->m_szMdbIDs[i], nBufLen, (TS_UC*)(pWriter->m_pExtendFileBuffer+nCount), pParam)) < 0 )
			   {
				   nFailureNum++;
				   CString strTemp=_T("");
				   strTemp.Format(_T("%d;"), pWriter->m_szMdbIDs[i]);  

				   strFailureList+=strTemp;  // flash多个使用
				   switch(nFlashRet)
				   {
				   case -1:strTemp.Format(_T("|ID %d Error : Please verify connection!"), pWriter->m_szMdbIDs[i]);break;
				   case -2:strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);break;
				   case -3:strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);break;
				   case -4:strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);break;
				   case -5:strTemp.Format(_T("|ID %d Error : Unable to Initialize..."), pWriter->m_szMdbIDs[i]);break;
				   case -6:strTemp.Format(_T("|ID %d Error : Unable to Erase..."), pWriter->m_szMdbIDs[i]);break;
				   case -7:strTemp.Format(_T("|ID %d Error : Unable to start programming..."), pWriter->m_szMdbIDs[i]);break;
				   case -8:strTemp.Format(_T("|ID %d Error : Update was interrupted! Please verify connection."), pWriter->m_szMdbIDs[i]);break;
				   case -9:strTemp.Format(_T("|ID %d Error : Programming was canceled."), pWriter->m_szMdbIDs[i]);break;
				   }
				   pWriter->OutPutsStatusInfo(strTemp);
				   //AfxMessageBox(strTemp);
				   break;
			   }
			   else
			   {									
				   nCount += nBufLen;

				   CString strText;
				   strText.Format(_T("|ID %d: Programming section %d finished."), pWriter->m_szMdbIDs[i], p);
				   pWriter->OutPutsStatusInfo(strText);

				   //AfxMessageBox(strText);				
			   }

			   if (p==0)
			   {	
				   int nTemp = Write_One(pWriter->m_szMdbIDs[i], 16, 8);
				   nTemp = Write_One(pWriter->m_szMdbIDs[i], 33, 1);

			   }

			   Sleep(500); //must if not ,have some wrong

		   }	


	   }

#endif
   
	    

		
		
		
		
		
		if(nFlashRet > 0) // flash 成功
		{
			CString strText;
			strText.Format(_T("|ID %d: Programming successful."), pWriter->m_szMdbIDs[i-1]);
			pWriter->OutPutsStatusInfo(strText);

			//AfxMessageBox(strText);		
		}
//*******************************************************************************

		pWriter->WriteFinish(nFlashRet);
		Sleep(500);
	
		close_com();
		return 1;//close thread
//	}while(1);


}
void CComWriter::SetHexInfor(Bin_Info temp){
int i;
for (  i=0;i<5;i++)
{
m_hexinfor.company[i]=temp.company[i];
}
for (  i=0;i<10;i++)
{
    m_hexinfor.product_name[i]=temp.product_name[i];
}
for (  i=0;i<3;i++)
{
    m_hexinfor.reserved[i]=temp.reserved[i];
}
m_hexinfor.software_high=temp.software_high;
m_hexinfor.software_low=temp.software_low;
}
BOOL CComWriter::UpdataDeviceInformation(int& ID){

    unsigned short Device_infor[10];
    CString str_ret,temp;
    int ret=read_multi(ID,&Device_infor[0],0,10);
   CString prodcutname=GetProductName(Device_infor[7]);
    for (int i=0;i<10;i++)
    {
        temp.Format(_T("%d "),Device_infor[i]);
        str_ret+=temp;
    }
    int nCount = str_ret.GetLength();
    WCHAR* strNew = new WCHAR[nCount+1];
	ZeroMemory(strNew, (nCount+1)*sizeof(WCHAR));
	LPCTSTR str = LPCTSTR(str_ret);
	memcpy(strNew, str, nCount*sizeof(WCHAR));
    prodcutname.MakeLower();
   CString hexproductname=_T("");
   for (int i=0;i<10;i++)
   {
  hexproductname.AppendFormat(_T("%c"),m_hexinfor.product_name[i]);
   }
  hexproductname.MakeLower();
 
  prodcutname.MakeUpper();
  hexproductname.MakeUpper();
  CString strtips;
  strtips.Format(_T("Your device is %s   Your hex file is fit for %s "),prodcutname.GetBuffer(),hexproductname.GetBuffer());


  OutPutsStatusInfo(strtips,false);
 // UpdateStatusInfo(strtips,FALSE);
    if(hexproductname.CompareNoCase(prodcutname)==0)
    {
	SendMessage(m_pParentWnd->m_hWnd, WM_UPDATA_DEVICE_INFORMATION, 0, LPARAM(strNew));
    return TRUE;
    }
    return FALSE;
	
	
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

//	if (m_nHexFileType == 0)	//Marked by Fance
//	{
	
		//AddStringToOutPuts(strTips);


		m_pWorkThread=AfxBeginThread(run_back_ground_flash_thread, this); //create thread,read information	
		ASSERT(m_pWorkThread);
//	}

	return 1;
}