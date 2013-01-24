		// GridFlashDlg.cpp : implementation file
		//

		#include "stdafx.h"
		#include "T3000.h"
		#include "MainFrm.h"
		#include "GridFlashDlg.h"
		#include "global_variable_extern.h"
		#include "globle_function.h"
		#include "AfxMessageDialog.h"


		// CGridFlashDlg dialog
		const int p_mode=-100;
		#define RETRY_TIMES 10
		#define WRITE_MUTLI_NUMBER 128
		#define ERROR_HEAD	Success Serial#
 
		BOOL replace_flag=FALSE;
		UINT run_back_ground_flash_thread(LPVOID pParameter);

		bool is_a_tstat_by_product_model(int product_model)
		{
			switch(product_model)
			{
			case 1:return true;
			case 2:return true;
			case 3:return true;
			case 4:return true;
			case 12:return true;
			case 17:return true;
			case 18:return true;
			case 19:return true;
			}
			return false;
		}
		int flash_a_tstat(BYTE m_ID,unsigned int the_max_register_number,unsigned char *register_data_orginal,int baudrate_var,int &which_register_was_interrupt)
		{
			//the return value 1,successful
			//<0 ,have some trouble
			unsigned char *register_data=register_data_orginal;
			unsigned int ii=0;
		//**************************************************inspect the flash that last flash position **
				if(read_one(m_ID,0xee10)<0)//retry times is better
				{
					//read_one from globle_function.h
					//the return value == -1 ,no connecting
					//AfxMessageBox("before write,need to open the com!");
					//close_com();
					//error -1
					return -1;
				}
				{
					//from 0000 register flash
					//from 0000 flash update
					ii=0;
					showing_text="initializing!";
					Sleep(500);
					//********************write register 16 value 0x7f **************
					int temp_baudrate=baudrate_var;//get the input baudrate
					do{
						if(ii<RETRY_TIMES)
							if(-2==Write_One(m_ID,16,0x7f))//From Common.h
							{
								ii++;
								if(ii==RETRY_TIMES && baudrate_var==9600 && temp_baudrate!=19200)
								{
									Change_BaudRate(19200);//From Common.h
									temp_baudrate=19200;
									ii=1;//
								}
								Sleep(6000);
							}
							else
								ii=0;
						else
						{
							return -2;//error -2 Unable to Initialize...
						}
					}while(ii);
					//********************write register 16 value 0x3f **************
					ii=0;
					showing_text+="erasing!";
					do{
						if(ii<RETRY_TIMES)
							if(-2==Write_One(m_ID,16,0x3f))//From common.h
								ii++;
							else
								ii=0;
						else
						{
							return -6;//error -6
						}
					}while(ii);
					//********************write register 16 value 0x1f **************
					ii=0;
					Sleep(3000);
					//must have this ,the Tstat need	
				
				}

				while(ii<=the_max_register_number)
				{
					unsigned char data_to_send[160]={0};// buffer that writefile() will to use
						int itemp=0;
						replace_flag=TRUE;
						showing_text.Format(_T("initialized!erasing!programming!Programing progress %.1f%%"),100*((float)ii)/the_max_register_number);
						do{
							if(itemp<RETRY_TIMES)
							{   //From common.h
								if(-2==write_multi(m_ID,&register_data[ii],ii,WRITE_MUTLI_NUMBER))
									//to write multiple WRITE_MUTLI_NUMBER bytes
								{
									itemp++;
									//showing_text.Format("initialized!erasing!programming!Writing line %d to %d...",ii,ii+WRITE_MUTLI_NUMBER);
									replace_flag=TRUE;
									showing_text.Format(_T("initialized!erasing!programming!Programing progress %.1f%%"),100*((float)ii)/the_max_register_number);
								}
								else
								{
									itemp=0;
									which_register_was_interrupt=ii;
								}
							}
							else
							{
								return -8;//the com connection is wrong! error -8
							}
						}while(itemp);
						ii+=WRITE_MUTLI_NUMBER;
				}
		//********************write register 16 value 0x01 **************
				ii=0;
				which_register_was_interrupt=-9;
				//********************write register 16 value 01 **************
				do{
					if(ii<RETRY_TIMES)
						if(-2==Write_One(m_ID,16,1))
							ii++;
						else
							ii=0;
					else
					{
						return -8;//error -8
					}
				}while(ii);					
			Change_BaudRate(baudrate_var);//back baudrate
			return 1;
		}

		UINT Check_Status(LPVOID pParameter)
		{
			CGridFlashDlg* pDlg=(CGridFlashDlg*)pParameter;
			int iitemp;
			_ConnectionPtr pCon=NULL;
			_RecordsetPtr pRs=NULL;

			pCon.CreateInstance(_T("ADODB.Connection"));
			pRs.CreateInstance(_T("ADODB.Recordset"));
			UINT nSerial=0;
			UINT nSerialNumber=0;
			pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);
			for(iitemp=0;iitemp<pDlg->m_grid_flash.size();iitemp++)
			{
				int nTempID;
				int nID=-1;
				nTempID=pDlg->m_grid_flash.at(iitemp).ID;
		
			//	nID=read_one(nTempID,6,2);
					nSerial=pDlg->m_grid_flash.at(iitemp).serialnumber;
					unsigned short SerialNum[9];
					memset(SerialNum,0,sizeof(SerialNum));
					int nRet=0;
					nRet=Read_Multi(nTempID,&SerialNum[0],0,9,3);
					if(nRet>0)
					{
						nID=SerialNum[6];
						nSerialNumber=SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256+SerialNum[3]*256*256*256;
					}


	 		 if(nID>0&&nSerialNumber==nSerial) 
				{ 
					pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,11,_T("On line"));
					pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,9,_T("True"));//flash select
				 }
				 else
				 {
				   pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,11,_T("Off line"));
			 		pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,9,_T("False"));//flash select
				 }
				CString str_temp;
				int nEpSize=0;
				_variant_t temp_variant;
				str_temp.Format(_T("select * from ALL_NODE where Serial_ID = '%d'"),pDlg->m_grid_flash.at(iitemp).serialnumber);
				pRs->Open(str_temp.GetString(),_variant_t((IDispatch *)pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
				if(pRs!=NULL)
				{
				//temp_variant=pRs->GetCollect(_T("EPsize"));//MODBUS_EEPROM_SIZE,
				//if(temp_variant.vt!=VT_NULL)
				//	nEpSize=temp_variant;
				//else
				//	nEpSize=0;	 
				}
				if(nEpSize==1)
				{
					pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,7,_T("1K Bytes"));
				}
				if(nEpSize==0)
				{
					pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,7,_T("256 Bytes"));
				}
				if(pRs->State) 
					pRs->Close(); 
			}

			if(pRs->State) 
				pRs->Close(); 
			if(pCon->State)
				pCon->Close(); 
			return 1;
		}

		IMPLEMENT_DYNAMIC(CGridFlashDlg, CDialog)
		CGridFlashDlg::CGridFlashDlg(CWnd* pParent /*=NULL*/)
			: CDialog(CGridFlashDlg::IDD, pParent)
		{
			m_pBackCheckThread=NULL;
			pThread_flash=NULL;
			m_bStopLoadingfile=FALSE;
			m_bFinished=FALSE;
			m_bEnableSaveConfig=FALSE;
			inforline=0;
		}

		CGridFlashDlg::~CGridFlashDlg()
		{
		}

		void CGridFlashDlg::DoDataExchange(CDataExchange* pDX)
		{
			CDialog::DoDataExchange(pDX);
			DDX_Control(pDX, IDC_GRIDFLASH_MSFLEXGRID, m_FlexGrid);
			//	DDX_Control(pDX, IDC_SUBLIST, m_subNetList);
			DDX_Control(pDX, IDC_CHECKTOALL, m_AllCheckBtn);
			DDX_Control(pDX, IDC_HEXFILEPATHEDIT, m_hexFilePathNameEdit);
			DDX_Control(pDX, IDC_LIST2, m_infoListBox);
			DDX_Control(pDX, IDC_CHECK1, m_EnableSaveBtn);
		}


		BEGIN_MESSAGE_MAP(CGridFlashDlg, CDialog)
			ON_BN_CLICKED(IDC_OPENBUTTON, &CGridFlashDlg::OnBnClickedOpenbutton)
			ON_BN_CLICKED(IDC_CHECKTOALL, &CGridFlashDlg::OnBnClickedChecktoall)
			ON_BN_CLICKED(IDC_FLASHBUTTON, &CGridFlashDlg::OnBnClickedFlashbutton)
			ON_BN_CLICKED(IDCANCEL, &CGridFlashDlg::OnBnClickedCancel)
			ON_BN_CLICKED(IDC_BUTTON1, &CGridFlashDlg::OnBnClickedButton1)
			ON_BN_CLICKED(IDC_BUTTON2, &CGridFlashDlg::OnBnClickedButton2)
			ON_WM_TIMER()
			ON_WM_DESTROY()
			ON_BN_CLICKED(IDC_CHECK1, &CGridFlashDlg::OnBnClickedCheck1)
		END_MESSAGE_MAP()


		// CGridFlashDlg message handlers
 
		BOOL CGridFlashDlg::OnInitDialog()
		{
			CDialog::OnInitDialog();
			GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);

			m_strLogFilePath=g_strExePth+_T("burn_hex_Log");
			CreateDirectory(m_strLogFilePath,NULL);//create directory file

			m_FlexGrid.put_TextMatrix(0,1,_T("Serial#"));
			m_FlexGrid.put_TextMatrix(0,2,_T("ID"));
			m_FlexGrid.put_TextMatrix(0,3,_T("Baudrate"));
			m_FlexGrid.put_TextMatrix(0,4,_T("H/W ver"));
			m_FlexGrid.put_TextMatrix(0,5,_T("Device"));
			m_FlexGrid.put_TextMatrix(0,6,_T("S/W ver"));

			m_FlexGrid.put_TextMatrix(0,7,_T("EP size"));

			m_FlexGrid.put_TextMatrix(0,8,_T("Hex File"));
			m_FlexGrid.put_TextMatrix(0,9,_T("Enable"));
			m_FlexGrid.put_TextMatrix(0,10,_T("Status"));
			m_FlexGrid.put_TextMatrix(0,11,_T("Connetion"));
			m_FlexGrid.put_TextMatrix(0,12,_T("Burn Event"));
		
			m_FlexGrid.put_ColWidth(0,0);
			m_FlexGrid.put_ColWidth(1,800);
			m_FlexGrid.put_ColWidth(2,500);
			m_FlexGrid.put_ColWidth(3,800);
			m_FlexGrid.put_ColWidth(4,800);
			m_FlexGrid.put_ColWidth(5,700);
			m_FlexGrid.put_ColWidth(6,800);
			m_FlexGrid.put_ColWidth(7,900);
			m_FlexGrid.put_ColWidth(8,3200);
			m_FlexGrid.put_ColWidth(9,800);
			m_FlexGrid.put_ColWidth(10,700);
			m_FlexGrid.put_ColWidth(11,900);
			m_FlexGrid.put_ColWidth(12,1500);


			saved_path=g_strExePth+_T("burn_hex_Log");
			file_directory=saved_path;
			log_file_path=g_strExePth+_T("\\flash_log.txt");
			CreateDirectory(saved_path.GetString(),NULL);//create directory file

			CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	 
		 
		
			m_grid_flash.clear();
			for(int i=0;i<pFrame->m_product.size();i++)
			{
				int j=pFrame->m_product.at(i).product_id;
				grid_flash temp_grid_flash;
				temp_grid_flash.ID=pFrame->m_product.at(i).product_id;
				//j;//ID for get_serialnumber function
				temp_grid_flash.baudrate=pFrame->m_product.at(i).baudrate;
				//baudrate
				temp_grid_flash.serialnumber =pFrame->m_product.at(i).serial_number ;
				//serialnumber			
				temp_grid_flash.flash_or_no=true;
				temp_grid_flash.hardware_revisin=pFrame->m_product.at(i).hardware_version;
				//read_one(j ,8); ;
				temp_grid_flash.device=pFrame->m_product.at(i).product_class_id ;
				temp_grid_flash.software_version=pFrame->m_product.at(i).software_version;
				//get_tstat_version(j);

				temp_grid_flash.EpSize=pFrame->m_product.at(i).nEPsize;
				//if(temp_grid_flash.software_version>0)
				{
				//	if(make_sure_isp_mode(j)==1)
		 		//temp_grid_flash.software_version=p_mode;
				}
				// 屏蔽NC的设备信息
		/*	 	if(PM_NC==temp_grid_flash.device||NET_WORK_OR485_PRODUCT_MODEL==temp_grid_flash.device)
					continue; */

				m_grid_flash.push_back(temp_grid_flash);
			}

			CString stemp;
			m_FlexGrid.put_Rows(m_grid_flash.size()+2);
		
			for(int i=0;i<m_grid_flash.size();i++)
			{
					stemp.Format(_T("%ld"),m_grid_flash.at(i).serialnumber);
					m_FlexGrid.put_TextMatrix(i+1,1,stemp);//serialnumber
					stemp.Format(_T("%d"),m_grid_flash.at(i).ID);
					m_FlexGrid.put_TextMatrix(i+1,2,stemp);
					stemp.Format(_T("%d"),m_grid_flash.at(i).baudrate);
					m_FlexGrid.put_TextMatrix(i+1,3,stemp);//baudrate
					stemp.Format(_T("%d"),m_grid_flash.at(i).hardware_revisin);
					m_FlexGrid.put_TextMatrix(i+1,4,stemp);//hardware revision
                
					stemp.Format(_T("%d"),m_grid_flash.at(i).device);
				

					switch(m_grid_flash.at(i).device)
					{
						case 2:stemp=g_strTstat5a;break;//Tstat5A
						case 1:stemp=g_strTstat5b;break;//Tstat5A
						case 3:stemp=g_strTstat5b;break;//Tstat5A
						case 4:stemp=g_strTstat5c;break;//Tstat5A
						case 12:stemp=g_strTstat5d;break;//Tstat5A
						case 17:stemp=g_strTstat5f;break;
						case 18:stemp=g_strTstat5g;break;
						case 16:stemp=g_strTstat5e;break;
						case 19:stemp=g_strTstat5h;break;
						case 26:stemp=g_strTstat6;break;
						case 27:stemp=g_strTstat7;break;
						case PM_NC:stemp=g_strnetWork;break;
						case LED_PRODUCT_MODEL:stemp=_T("Led");break;//Others
						case PM_SOLAR:			stemp=_T("Solar");break;
						case PM_ZIGBEE:			stemp=_T("ZigBee");break;
						case T3_32I_PRODUCT_MODEL:stemp=_T("T3-32I");break;
						case T3_8I_16O_PRODUCT_MODEL:stemp=_T("T3-8I-16O");break;
						case PM_T3IOA:stemp=_T("T3-8IOA");break;
						case PM_T38AIOD:stemp=_T("T3-8IOD");break;
						default:stemp=_T("Others");break;
					}
					m_FlexGrid.put_TextMatrix(i+1,5,stemp);//product model ===== software version
					if(m_grid_flash.at(i).software_version==p_mode)
						stemp=_T("ISP");
					else
						stemp.Format(_T("%.1f"),m_grid_flash.at(i).software_version);
					m_FlexGrid.put_TextMatrix(i+1,6,stemp);//product model ===== software version
					m_FlexGrid.put_TextMatrix(i+1,10,_T(""));
					CString strSerial;
					strSerial.Format(_T("%d_burn_log"),m_grid_flash.at(i).serialnumber);
					CString strLogFile;

					strLogFile=file_directory+_T("\\")+strSerial+_T(".txt");
					WIN32_FIND_DATA fData;
					HANDLE hFile=NULL;
					hFile = FindFirstFile(strLogFile,&fData);
					if(hFile != INVALID_HANDLE_VALUE)
					{
						CFileStatus   st;
						CStdioFile fileLog(strLogFile,CFile::modeReadWrite | CFile::modeNoTruncate|CFile::typeText);
						TCHAR pbuf[50];
						CString strLine;
						CString strLastLine;
						fileLog.GetStatus(st);
						if(st.m_size>50)
						{
							fileLog.Seek(-50,fileLog.end);
							while(fileLog.ReadString(strLine))
							{
								strLastLine.Empty();
								strLastLine=strLine;
						
							}
							if(strLastLine.Find(_T("Success"))>=0)
							{
								SYSTEMTIME stUTC, stLocal;
								CString strDatatime;
								FileTimeToSystemTime(&(fData.ftLastWriteTime), &stUTC);
								SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
					
	    						strDatatime.Format(_T("%d. %d %d, %d:%d"), stLocal.wDay,stLocal.wMonth,stLocal.wYear,stLocal.wHour,stLocal.wMinute);
								m_FlexGrid.put_TextMatrix(i+1,12,strDatatime);
							}
						}
					
						fileLog.Close();
					}
					FindClose(hFile);
					for(int k=0;k<=12;k++)
					{
						if (i%2==1)
						{
							m_FlexGrid.put_Row(i+1);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(RGB(255,255,255));
						}
						else
						{
							m_FlexGrid.put_Row(i+1);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(RGB(236,232,230));
						}
				}
			}
	  m_pBackCheckThread=AfxBeginThread(Check_Status,this);
	  m_pBackCheckThread->m_bAutoDelete=FALSE;
	 
			SetTimer(1,10,NULL);
			return TRUE; 
			// return TRUE unless you set the focus to a control
			// EXCEPTION: OCX Property Pages should return FALSE
		}

		void CGridFlashDlg::OnBnClickedOpenbutton()
		{
			CString strFilter = _T("hex File|*.hex|all File|*.*||");
			CFileDialog dlg(true,_T("hex"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
			dlg.DoModal();
			CString strTemp;
			strTemp=dlg.GetPathName();		
			//save the path
			//CFile default_file;
			if(strTemp.IsEmpty())
			{
				return;
			}
			m_strHexFile=strTemp;
			m_hexFilePathNameEdit.SetWindowText(m_strHexFile);
			if(m_AllCheckBtn.GetCheck()==BST_CHECKED)
			{
				for(int i=0;i<m_grid_flash.size();i++)
					m_FlexGrid.put_TextMatrix(i+1,8,m_strHexFile);
			}
		}

		void CGridFlashDlg::OnBnClickedChecktoall()
		{	
			if(m_AllCheckBtn.GetCheck()==BST_CHECKED)
			{
				for(int i=0;i<m_grid_flash.size();i++)
					m_FlexGrid.put_TextMatrix(i+1,8,m_strHexFile);
			}
			if(m_AllCheckBtn.GetCheck()==BST_UNCHECKED)
			{
				for(int i=0;i<m_grid_flash.size();i++)
					m_FlexGrid.put_TextMatrix(i+1,8,_T(""));
			}
		}

		void CGridFlashDlg::Read_FromTable()
		{
			m_EnableSaveBtn.EnableWindow(FALSE);
			int iitemp;
			m_bFinished=FALSE;
			GetDlgItem(IDC_FLASHBUTTON)->EnableWindow(FALSE);
			for(iitemp=0;iitemp<m_grid_flash.size();iitemp++)
			{//get the grid value
				m_grid_flash.at(iitemp).hardware_revisin = _wtoi(m_FlexGrid.get_TextMatrix(iitemp+1,4));//hardware rev
				CString str_temp=m_FlexGrid.get_TextMatrix(iitemp+1,5);//software version ;;;;;product model

				if(str_temp==g_strTstat5a)
					m_grid_flash.at(iitemp).device =2;//Tstat5
				else if(str_temp==g_strTstat5b)
					m_grid_flash.at(iitemp).device =1;//Tstat5
				else if(str_temp==g_strTstat5b2)
					m_grid_flash.at(iitemp).device =3;//Tstat5
				else if(str_temp==g_strTstat5c)
					m_grid_flash.at(iitemp).device =4;//Tstat5
				else if(str_temp==g_strTstat5d)
					m_grid_flash.at(iitemp).device =12;//Tstat5D
				else if(str_temp==g_strTstat5f)
					m_grid_flash.at(iitemp).device =17;//Tstat
				else if(str_temp==g_strTstat5g)
					m_grid_flash.at(iitemp).device =18;//Tstat

				else if(str_temp==g_strTstat5e)
					m_grid_flash.at(iitemp).device =16;//Tstat
				else if(str_temp==g_strTstat5h)
					m_grid_flash.at(iitemp).device =19;//Tstat5
				else if(str_temp==g_strTstat6)
					m_grid_flash.at(iitemp).device =26;//Tstat6
				else if(str_temp==g_strTstat7)
					m_grid_flash.at(iitemp).device =27;//Tstat7

				else if(str_temp==g_strnetWork)
					m_grid_flash.at(iitemp).device =PM_NC;	
				else if(str_temp==_T("Solar"))
					m_grid_flash.at(iitemp).device =PM_SOLAR;
				else if(str_temp==_T("ZigBee"))
					m_grid_flash.at(iitemp).device =PM_ZIGBEE;
				else if(str_temp==_T("Led"))
					m_grid_flash.at(iitemp).device =LED_PRODUCT_MODEL;//Others
				else if(str_temp==_T("T3-8IOD"))
					m_grid_flash.at(iitemp).device =PM_T38AIOD;//Others
				else if(str_temp==_T("T3-8IOA"))
					m_grid_flash.at(iitemp).device =PM_T3IOA;//Others
				else if(str_temp==_T("T3-8I-16O"))
					m_grid_flash.at(iitemp).device =T3_8I_16O_PRODUCT_MODEL;//Others
				else if(str_temp==_T("T3-32I"))
					m_grid_flash.at(iitemp).device =T3_32I_PRODUCT_MODEL;//Others
				else if(str_temp==_T("Others"))
					m_grid_flash.at(iitemp).device =LED_PRODUCT_MODEL;
				m_grid_flash.at(iitemp).hex_file_path=m_FlexGrid.get_TextMatrix(iitemp+1,8);//hex file path
				str_temp=m_FlexGrid.get_TextMatrix(iitemp+1,9);	
				if(str_temp.CompareNoCase(_T("true"))==0)
				{
					m_grid_flash.at(iitemp).flash_or_no =true;
					m_FlexGrid.put_TextMatrix(iitemp+1,9,_T(""));
				}
				else
				{
					m_grid_flash.at(iitemp).flash_or_no =false;

				}
				m_grid_flash.at(iitemp).save_settings =true;

			}

		}
		void CGridFlashDlg::OnBnClickedFlashbutton()
		{
			if (m_grid_flash.size()!=0)
			{		
				Read_FromTable();//读出数据信息From FlexGrid
			  DWORD dwExidCode;
			 GetExitCodeThread(m_pBackCheckThread->m_hThread,&dwExidCode);
			if(dwExidCode==STILL_ACTIVE)
			{
				TerminateThread(m_pBackCheckThread->m_hThread,dwExidCode);
				m_pBackCheckThread=NULL;
				Sleep(100);
			}

			SuspendRefreshTreeviewThread();

			pThread_flash=AfxBeginThread(run_back_ground_flash_thread,this );////////////////////////create thread,read information	
			pThread_flash->m_bAutoDelete=FALSE;
			} 
			else
			{
				AfxMessageBox(_T("No Device in the list,Please Connect!."));
			}
		 

		}

		void CGridFlashDlg::OnBnClickedCancel()
		{
	 
			CDialog::OnCancel();
		}

		void CGridFlashDlg::OnBnClickedButton1()
		{
			m_bStopLoadingfile=TRUE;
		}

		void CGridFlashDlg::OnBnClickedButton2()
		{
			ShellExecute(this->m_hWnd, _T("open"), m_strLogFilePath, NULL, NULL, SW_SHOWNORMAL);

		}

		void CGridFlashDlg::OnTimer(UINT_PTR nIDEvent)
		{
			inforline=m_infoListBox.GetCount();
			if(nIDEvent==1)
			{
			// TODO: Add your message handler code here and/or call default
				CString strTemp;
				critical_section.Lock();
				strTemp=showing_text;
		 
				critical_section.Unlock();

				if(m_strInfoText.CompareNoCase(strTemp)!=0)
				{
					--inforline;
					m_strInfoText=strTemp;
					m_infoListBox.DeleteString(inforline);
					m_infoListBox.InsertString(inforline,m_strInfoText);
				}
				{

				}
 
			//	if(m_strInfoText.Find(_T("Success"))!=-1 || m_strInfoText.Find(_T("Failure"))!=-1)
				if(m_bFinished)
				{			
					m_EnableSaveBtn.EnableWindow(TRUE);
					GetDlgItem(IDC_FLASHBUTTON)->EnableWindow(TRUE);
					KillTimer(1);
					m_infoListBox.InsertString(inforline,m_strInfoText.GetString());
					m_strInfoText.Empty();
					CString temp_str=_T("To view the file please click on the \"Log File\" option");
					m_strInfoText=m_strInfoText+_T("\n")+temp_str+_T("\n");
					m_infoListBox.InsertString(0,temp_str.GetString());
					temp_str=_T("Loading status are stored into a Log file.");
					m_strInfoText=m_strInfoText+_T("\n")+temp_str+_T("\n");
					m_infoListBox.InsertString(0,temp_str.GetString());

				/*	AfxMessageBox(m_strInfoText);*/
				}

			}
			CDialog::OnTimer(nIDEvent);
		}


		void CGridFlashDlg::OnDestroy()
		{
			CDialog::OnDestroy();

			m_bStopLoadingfile=TRUE;
			Sleep(3000);

			DWORD dwExidCode;
			GetExitCodeThread(m_pBackCheckThread->m_hThread,&dwExidCode);
			if(dwExidCode==STILL_ACTIVE)
			{
				TerminateThread(m_pBackCheckThread->m_hThread,dwExidCode);
				m_pBackCheckThread=NULL;
				Sleep(100);
			}

			ContinueRefreshTreeviewThread();
		}
		BEGIN_EVENTSINK_MAP(CGridFlashDlg, CDialog)
			ON_EVENT(CGridFlashDlg, IDC_GRIDFLASH_MSFLEXGRID, DISPID_CLICK, CGridFlashDlg::ClickMsflexgrid1, VTS_NONE)
		END_EVENTSINK_MAP()

		void CGridFlashDlg::ClickMsflexgrid1()
		{
			long lRow,lCol;
			lRow = m_FlexGrid.get_RowSel();//
			lCol = m_FlexGrid.get_ColSel(); //
			UpdateData(false);
			if(lRow==0 || lCol==0)
				return;
			int nRowsCounts=m_FlexGrid.get_Rows();
			if(lRow==nRowsCounts-1)
				return;
			CRect rect;
			m_FlexGrid.GetWindowRect(rect); //
			ScreenToClient(rect); //	
			// 
			//
			CDC* pDC =GetDC();
			//
			int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
			int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
			//
			long y = m_FlexGrid.get_RowPos(lRow)/nTwipsPerDotY;
			long x = m_FlexGrid.get_ColPos(lCol)/nTwipsPerDotX;
			//
			long width = m_FlexGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
			long height = m_FlexGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
			//
			CRect rc(x,y,x+width,y+height);
			//
			rc.OffsetRect(rect.left+1,rect.top+1);
			//
			CString strValue = m_FlexGrid.get_TextMatrix(lRow,lCol);
			row_row=lRow;
			row_col=lCol;
			bool view=true;
			if(rc.top<rect.top+height || rc.bottom>rect.bottom+1 || rc.left<rect.left+1 || rc.right>rect.right+1)
				view=false;
		
			if(row_col==8)
			{
				CString strFilter = _T("hex File|*.hex|all File|*.*||");
				CFileDialog dlg(true,_T("hex"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
				dlg.DoModal();
				m_strHexFile=dlg.GetPathName();	
				if(!m_strHexFile.IsEmpty())
				m_FlexGrid.put_TextMatrix(row_row,row_col,m_strHexFile);				
			}
			else if(row_col==9)
			{
				CString temp_str=m_FlexGrid.get_TextMatrix(row_row,row_col);
				if(temp_str.CompareNoCase(_T("True"))==0)
				{
					m_FlexGrid.put_TextMatrix(row_row,row_col,_T("False"));
					m_grid_flash.at(row_row-1).flash_or_no=false;
				}
				else
				{
					m_grid_flash.at(row_row-1).flash_or_no=true;
					m_FlexGrid.put_TextMatrix(row_row,row_col,_T("True"));
				}
		
			}
			else if(lCol==1 || lCol==2 || lCol==3 || lCol==4 || lCol==5 || lCol==8)
			{
				return;
			}

		}
		void CGridFlashDlg::OnBnClickedCheck1()
		{
			//m_bEnableSaveConfig
			if(m_EnableSaveBtn.GetCheck()==BST_UNCHECKED)
			{
				m_bEnableSaveConfig=FALSE;
				m_EnableSaveBtn.SetCheck(BST_UNCHECKED);
			}
			if(m_EnableSaveBtn.GetCheck()==BST_CHECKED)
			{
				m_bEnableSaveConfig=TRUE;
				m_EnableSaveBtn.SetCheck(BST_CHECKED);
			}
		}

		void CGridFlashDlg::SuspendRefreshTreeviewThread()
		{
			CMainFrame* pFrm = NULL;
			pFrm= (CMainFrame*)GetParent();	
			pFrm->SuspendRefreshThread();

		}

		void CGridFlashDlg::ContinueRefreshTreeviewThread()
		{
			CMainFrame* pFrm = NULL;
			(CMainFrame*)pFrm= (CMainFrame*)GetParent();	
			pFrm->ContinueRefreshThread();
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// new flash code by zgq; reorganize the old code
		///*
		UINT run_back_ground_flash_thread(LPVOID pParameter)
		{
      
			CGridFlashDlg* pDlg=(CGridFlashDlg*)pParameter;
			int writing_row=0;
			int iitemp;
			unsigned int the_max_register_number=0;//the max register number to write from hex file
			int failure_number=0;
			int disabled_number=0;
			bool log_file_opened=false;//true==opened ;false = can't open
			CString save_file_path;//path of the save file		
			int baudrate=19200; 
			
			for(iitemp=0;iitemp<pDlg->m_grid_flash.size();iitemp++) 
	//从第一个开始 到最后一个 
			{
		//-------------FOR Begin------------------------//
				CStdioFile log_file;
				load_file_every_step temppp;
				log_file_opened=false;
				//true==opened ;false = can't open
				CStdioFile m_hex_file;
				if(pDlg->m_bStopLoadingfile)//检查中间客户有没有停止flash
				{
					return 1;//停止返回
				}
				if(pDlg->m_grid_flash.at(iitemp).device==100)//.....下一个//NC 
					 continue;
		 
				now_tstat_id=pDlg->m_grid_flash.at(iitemp).ID;//获得ID
				if(/*pDlg->m_grid_flash.at(iitemp).flash_or_no==*/true)//不管是否烧写过
				{		

					//可用 但是路径为空
					if(pDlg->m_grid_flash.at(iitemp).hex_file_path.IsEmpty())
					{
						failure_number++;
						pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,10,_T("FAIL"));
						pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,9,_T("False"));
						continue ;
					}
				   //路径如果不是空
					int nID=-1;
					BOOL bSaveconfigFile=true;
					save_file_path.Format(_T("%ld.txt"),pDlg->m_grid_flash.at(iitemp).serialnumber);
					CString strConfigpathName=pDlg->file_directory+_T("\\")+save_file_path;//get the directory

					if (g_CommunicationType==0)//COM
					{
							baudrate=19200;
						Change_BaudRate(19200);
						nID=read_one(now_tstat_id,6);
						//From ：globle_function 
						if(nID<=0)
						{
								baudrate=9600;
								Change_BaudRate(9600);
								nID=read_one(now_tstat_id,6);
						}
					}
					if (g_CommunicationType==1)
					{
						nID=read_one(now_tstat_id,6);
					}
					if(nID<0)
					{
						save_file_path.Format(_T("%ld.txt"),pDlg->m_grid_flash.at(iitemp).serialnumber);
						showing_text=_T("can not connect,serial number:")+save_file_path;
						failure_number++;
						pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,10,_T("FAIL"));
						continue ;
					}
			
					if(make_sure_isp_mode(nID)==1)//是否在isp模式
					{
							pDlg->m_grid_flash.at(iitemp).software_version=p_mode;
							bSaveconfigFile=false;
					 
					}
					//保存保存的日志文件中
					CString s;
					if(bSaveconfigFile)
					{
						if(pDlg->m_bEnableSaveConfig)
						{
							s=_T("Saving to ");
							s+=strConfigpathName;
							showing_text=s;
							Save2File_ForTwoFiles((LPTSTR)(LPCTSTR)strConfigpathName);
						}
					}
	//////////////////////////////////Alex-------------Open Hex文件并且验证////////////////////////////////////////////////////////////////////			
	//打开Hex文件
					  CString hexfilepath,Device,SW_ver;
					if(m_hex_file.Open(pDlg->m_grid_flash.at(iitemp).hex_file_path.GetString() ,CFile::modeRead | CFile::shareDenyNone))
					{
	////////////////////////////////////begin

	//////////////Flash-Begin
						CString strSerial;
						CString strID;
						if(True/*pDlg->m_grid_flash.at(iitemp).device==p_mode*/)
						{
							int nEpSize=0;
						/////////////////////这段代码主要是从数据库中 根据序列号 查出Epsize///////////////////////
 
#if 1
							switch(read_one(pDlg->m_grid_flash.at(iitemp).ID,7))
							{
							case PM_TSTAT6:
								nEpSize=Read_One2(pDlg->m_grid_flash.at(iitemp).ID,408,FALSE);
								break;
							case PM_TSTAT7:
								nEpSize=Read_One2(pDlg->m_grid_flash.at(iitemp).ID,408,FALSE);
								break;
							default:
								nEpSize=Read_One2(pDlg->m_grid_flash.at(iitemp).ID,326,FALSE);
								break;
							}
#endif
						/*	CString str;
							str.Format(_T("%d"),nEpSize);
							AfxMessageBox(str);*/
							

						//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							if(nEpSize==1)
							{
							
								/*float tstat_version2 = (float)(read_one(ID,MODBUS_VERSION_NUMBER_HI)*256+read_one(ID,MODBUS_VERSION_NUMBER_LO));	
								tstat_version2 /=10;*/
								if (pDlg->m_grid_flash.at(iitemp).software_version<29.8)
								{
									 
										showing_text=_T("Hardware don't match with software");
										failure_number++;
										pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,10,_T("EP FAIL"));
										continue;
									 
								}

								
							} 
							else
							{

							 
							/*float tstat_version2 = (float)(read_one(ID,MODBUS_VERSION_NUMBER_HI)*256+read_one(ID,MODBUS_VERSION_NUMBER_LO));	
							tstat_version2 /=10;*/
							if (pDlg->m_grid_flash.at(iitemp).software_version>29.8)
							{
								showing_text=_T("Hardware don't match with software");
								failure_number++;
								pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,10,_T("EP FAIL"));
								continue;
							}
							 



						     }
						 
						/////////////EMP-END///////////////////////////
					//////////////////////begin

					log_file_opened=false;
					CString strSerial;
					strSerial.Format(_T("%d_burn_log"),pDlg->m_grid_flash.at(iitemp).serialnumber);
					strSerial+=_T(".txt");
					pDlg->log_file_path=pDlg->saved_path+_T("\\")+strSerial;

					//if exist,delete it: 
					WIN32_FIND_DATA fData;
					HANDLE hFile=NULL;
					hFile = FindFirstFile(pDlg->log_file_path,&fData);
					if(hFile != INVALID_HANDLE_VALUE)
					{
						FindClose(hFile);
						CFile::Remove(pDlg->log_file_path);
						pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,12,_T(""));
					}
					else
					{
						FindClose(hFile);
					}
						//新建写入Log文件		
					if(log_file.Open(pDlg->log_file_path,CFile::modeCreate | CFile::modeReadWrite )!=0)
					{
						log_file_opened=true;
						CString strHex=pDlg->m_grid_flash.at(iitemp).hex_file_path;
						if(!strHex.IsEmpty())
						{
							log_file.WriteString(strHex);
						}
						CString ttt=_T("\r\n");
						log_file.WriteString(ttt);
					}//////////////////////////////////////

					unsigned char register_data[65535];//for 128 byte everytime to write
					for(long ltemp=0;ltemp<65535;ltemp++)
						register_data[ltemp]=255;
				 
					CString m_get_data_from_file;
						BOOL CRC=TRUE;
					/////////////*****************inspect the file*********************

					unsigned int ii=0;
					//循环读取文件，直到文件结束
					while(NULL!=m_hex_file.ReadString(m_get_data_from_file))
					{
						//get a line from the file,check "crc" for total file
						//AfxMessageBox(m_get_data_from_file);
						ii++;//the line number that the wrong hex file;
						char a[256]={'a'};
						unsigned char get_hex[128]={0};//get hex data,it is get from the line char
											//the number is (i-1)
						int ddd=m_get_data_from_file.GetLength();
						int i=0;
						for(i=1;i<ddd;i++)//get a line
							a[i-1]=m_get_data_from_file.GetAt(i);
						if(strlen(a)%2==0)//偶数位置
							turn_hex_file_line_to_unsigned_char(a);//turn every char to int 
						else
						{
							char p_c_temp[74]={'\0'};
							failure_number++;
							CRC=FALSE;
							pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,10,_T("FAIL"));
							continue ;
						}

						turn_int_to_unsigned_char(a,i-1,get_hex);//turn to hex 
						if(get_hex[3]==1)//for to seektobegin() function,because to end of the file
							break;
						unsigned char uctemp2=0;
						int j;
						for(j=0;j<i/2;j++)
							uctemp2+=get_hex[j];
						if(uctemp2%256!=0)
						{
							TCHAR p_c_temp[74]={'\0'};
	 								//sprintf(p_c_temp,"Error: the hex file had error at %d line!",ii);
	 								//AfxMessageBox(p_c_temp);				
							failure_number++;
							pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,10,_T("FAIL"));
							CRC=FALSE;
							continue ;
						}
						
						if(get_hex[1]==0 && get_hex[2]==0)
							get_hex[4]=255;//for the 0000 register to write 255
						unsigned int ltemp;
						ltemp=get_hex[1]*256+get_hex[2];
						for(j=0;j<get_hex[0];j++)
							register_data[ltemp+j]=get_hex[4+j];//get the data
						if(the_max_register_number<(ltemp+get_hex[0]))
							the_max_register_number=ltemp+get_hex[0]-1;

					}
					//end while

								
					//flash
					
					 m_hex_file.Close();//close the file		
					if (!CRC)
						continue;
					 
					//-------------------------------------------------up is to get the register value from the file----------------------------------------------------
					int j=0;
										
					//------------------------------------------------------------save-----------------------------------------------------
	
					if(pDlg->m_grid_flash.at(iitemp).save_settings == true && is_a_tstat_by_product_model(pDlg->m_grid_flash.at(iitemp).device) && pDlg->m_grid_flash.at(iitemp).software_version!=p_mode)//flash only
					{	
						if(log_file_opened==true)//write to log file
						{
							s+=_T("\r\n");
							log_file.WriteString(s);
						}
						Sleep(500);///maybe is better
					}
					//-----------------------------------------------------------------flash-------------------------------------------------------------
					CString m_str_temp=_T("");
					int temp_2=pDlg->m_grid_flash.at(iitemp).baudrate;
					int which_register_was_interrupt;

					if(can_be_writed_hex_file(pDlg->m_grid_flash.at(iitemp).device,register_data[0x100]))
					{
						//写一次

						if((j=flash_a_tstat(now_tstat_id,the_max_register_number,register_data,pDlg->m_grid_flash.at(iitemp).baudrate,which_register_was_interrupt))<0)//flash
						{
								failure_number++;
								pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,10,_T("FAIL"));
								switch(j)
								{
									case -1:m_str_temp.Format(_T("Serial#:%ld ID:%ld Error :Please verify connection!"),pDlg->m_grid_flash.at(iitemp).serialnumber,pDlg->m_grid_flash.at(iitemp).ID );break;
									case -2:m_str_temp.Format(_T("Serial#:%ld ID:%ld Error :Unable to Initialize..."),pDlg->m_grid_flash.at(iitemp).serialnumber,pDlg->m_grid_flash.at(iitemp).ID );break;
									case -3:m_str_temp.Format(_T("Serial#:%ld ID:%ld Error :Init. Unable to Erase..."),pDlg->m_grid_flash.at(iitemp).serialnumber,pDlg->m_grid_flash.at(iitemp).ID );break;
									case -4:m_str_temp.Format(_T("Serial#:%ld ID:%ld Error :Init, Erase。Unable to start programming..."),pDlg->m_grid_flash.at(iitemp).serialnumber,pDlg->m_grid_flash.at(iitemp).ID );break;
									case -5:m_str_temp.Format(_T("Serial#:%ld ID:%ld Error :Unable to Initialize..."),pDlg->m_grid_flash.at(iitemp).serialnumber,pDlg->m_grid_flash.at(iitemp).ID );break;
									case -6:m_str_temp.Format(_T("Serial#:%ld ID:%ld Error :Init. Unable to Erase..."),pDlg->m_grid_flash.at(iitemp).serialnumber,pDlg->m_grid_flash.at(iitemp).ID );break;
									case -7:m_str_temp.Format(_T("Serial#:%ld ID:%ld Error :Init, Erase. Unable to start programming..."),pDlg->m_grid_flash.at(iitemp).serialnumber,pDlg->m_grid_flash.at(iitemp).ID );break;
									case -8:m_str_temp.Format(_T("Serial#:%ld ID:%ld Error :Init, Erase, Program. Update was interrupted! Please verify connection.(Interrupted at %d)"),pDlg->m_grid_flash.at(iitemp).serialnumber,pDlg->m_grid_flash.at(iitemp).ID,which_register_was_interrupt);break;
								}
								showing_text=m_str_temp;
								if(log_file_opened==true)//write to log file
								{
										CString temp_log_str=m_str_temp+"\r\n";
										log_file.WriteString(temp_log_str);	
								}
	 
								Sleep(3000);
	 
						}
						else
						{
							Sleep(13000);
							//wait for the tstat restart
							if(pDlg->m_grid_flash.at(iitemp).save_settings == false)
							{
									int www=1;
									int one_or_two_times=0;
								do{
										int tstat_state=Read_One(now_tstat_id,99);
										if(tstat_state!=-2 && tstat_state!=255)
										{
											CString temp=pDlg->file_directory+_T("\\")+save_file_path;//get the directory
											write_one(now_tstat_id,16,143);//eeprom
											if(log_file_opened==true)//write to log file
											{
													s.Empty();
													s=_T("writed eeprom!\r\n");
													log_file.WriteString(s);
											}
											break;
										}
										else if(www==11 && one_or_two_times<1)
											if(pDlg->m_grid_flash.at(iitemp).baudrate==9600)
											{
													Change_BaudRate(19200);
														www=1;
														one_or_two_times++;
											}
											else
											{
													Change_BaudRate(9600);
													www=1;
													one_or_two_times++;
											}
											Sleep(1000);
											www++;
										}while(www<12);

										Sleep(13000);//wait for the tstat restart
									}
									else if(is_a_tstat_by_product_model(pDlg->m_grid_flash.at(iitemp).device))
									{//start in tstat software state
										
										if(pDlg->m_grid_flash.at(iitemp).software_version!=p_mode)
										{
											if(pDlg->m_bEnableSaveConfig)
											{
													int www=1;
													int one_or_two_times=0;
												do{
													int tstat_state=Read_One(now_tstat_id,99);
													showing_text=_T("Wait.Loading...");

													if(tstat_state!=-2 && tstat_state!=255)
													{
														s.Empty();
														s=_T("Loading from ");
														s+=save_file_path;
														///		this->SetWindowText(s.GetString());
															
														write_one(now_tstat_id,324,0);	
														CString temp=pDlg->file_directory+_T("\\")+save_file_path;//get the directory

														WIN32_FIND_DATA fData;
														HANDLE hFile=NULL;
														hFile = FindFirstFile(temp,&fData);
														if(hFile != INVALID_HANDLE_VALUE)
														{
															if(log_file_opened==true)
															{
																	////////////////////////////////
																	LoadFile2Tstat(temppp,(LPTSTR)(LPCTSTR)(temp),&log_file);
																	////////////////////////
															}
															else
															{
																LoadFile2Tstat(temppp,(LPTSTR)(LPCTSTR)(temp));
															}														
														}
														FindClose(hFile);
														if(log_file_opened==true)//write to log file
														{
															s+=_T("\r\n");
															log_file.WriteString(s);
														}
														break;
													}
													else if(www==11 && one_or_two_times<1)
														if(pDlg->m_grid_flash.at(iitemp).baudrate==9600)
														{
															Change_BaudRate(19200);
															www=1;
															one_or_two_times++;
														}
														else
														{
															Change_BaudRate(9600);
															www=1;
															one_or_two_times++;
														}
														Sleep(1000);
														www++;
													}while(www<12);

												}
												
											}
											else
											{
												temppp.seven_step=true;
												temppp.first_step =true;
												temppp.second_step =true;
												temppp.thurth_step =true;
												temppp.fifth_step =true;
												temppp.third_step =true;
												temppp.sixth_step =true;
											}
										}
									//	flashed_one=true;//flashed
										showing_text.Format(_T("Success Serial#:%ld ID:%ld"),pDlg->m_grid_flash.at(iitemp).serialnumber,pDlg->m_grid_flash.at(iitemp).ID );
										if(log_file_opened==true)//write to log file
										{
											CString temp_log_str=showing_text+_T("\r\n");
											log_file.WriteString(temp_log_str);	
											log_file.Close();
											log_file_opened=false;

											WIN32_FIND_DATA fData;
											HANDLE hFile=NULL;
											hFile = FindFirstFile(pDlg->log_file_path,&fData);
											if(hFile != INVALID_HANDLE_VALUE)
											{
												//according file end mark to check whether successfully......
												//.......

												SYSTEMTIME stUTC, stLocal;
												CString strDatatime;
												FileTimeToSystemTime(&(fData.ftLastWriteTime), &stUTC);
												SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
												strDatatime.Format(_T("%d. %d %d, %d:%d"), stLocal.wDay,stLocal.wMonth,stLocal.wYear,stLocal.wHour,stLocal.wMinute);
												pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,12,strDatatime);

											}

											pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,10,_T("OK"));
											pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,9,_T("False"));////////////////////////success one
			
											FindClose(hFile);
										}
										Sleep(3000);//for showing_text view
									}
								}
								else
								{
									pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,10,_T("FAIL"));
									failure_number++;/////////////////////////////failure number
									showing_text.Format(_T("Serial#:%ld ID:%ld Error::Device in Unknown, Hex file for  "),pDlg->m_grid_flash.at(iitemp).serialnumber,pDlg->m_grid_flash.at(iitemp).ID);
									showing_text+=get_product_name_by_product_model(register_data[0x100]);
	//								showing_text+=".should be ";
	//								showing_text+=get_product_name_by_product_model(m_grid_flash.at(iitemp).device);
									Sleep(3000);
									if(log_file_opened==true)//write to log file
									{
										CString temp_log_str=showing_text+_T("\r\n");
										log_file.WriteString(temp_log_str);	
										log_file.Close();
										log_file_opened=false;
									}
								}
								Sleep(1000);//must ,if not ,have some wrong		

//----------------Flash----end


	
	//////////////////////////////文件是否可以打开的end


				   }
				   }
	/////////////////////////Alex-------------Open 成功////////////////////////////////////////////////////////////////////////////////////
								else
				  {
									showing_text=_T("Open hex file failure!");
									if(log_file_opened==true)//write to log file
									{
										CString temp_log_str=showing_text+_T("\r\n");
										log_file.WriteString(temp_log_str);

										log_file.Close();
										log_file_opened=false;
									}
									failure_number++;
									pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,10,_T("FAIL"));
				}							//						
				}		
				else//用户已经flash了一次	,再次flash，系统自动跳过
				{
					
								pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,10,_T("Skipped"));
								showing_text.Format(_T("Serial#:%ld ID:%ld -- Skipped"),pDlg->m_grid_flash.at(iitemp).serialnumber,pDlg->m_grid_flash.at(iitemp).ID);
								Sleep(1000);
								if(log_file_opened==true)//write to log file
								{
									CString temp_log_str=showing_text+_T("\r\n");
									log_file.WriteString(temp_log_str);	
									log_file.Close();
									log_file_opened=false;
								}
								disabled_number++;						
			  }
		//For END----------------------------------
			}
		
	
			showing_text.Format(_T("%d Success!%d Skipped!%d Failure!"),pDlg->m_grid_flash.size()-failure_number-disabled_number,disabled_number,failure_number);
		


		 
	 
			pDlg->m_bFinished=TRUE;
			Sleep(2000);//sleep for multithreading 
					
			return 1;

		}


