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
		for(int i=0;i<(int)pFrame->m_product.size();i++)
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
			temp_grid_flash.hardware_revisin=(short)pFrame->m_product.at(i).hardware_version;
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
		
		for(int i=0;i<(int)m_grid_flash.size();i++)
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
					//case 2:stemp=g_strTstat5a;break;//Tstat5A
					//case 1:stemp=g_strTstat5b;break;//Tstat5A
					//case 3:stemp=g_strTstat5b;break;//Tstat5A
					//case 4:stemp=g_strTstat5c;break;//Tstat5A
					//case 12:stemp=g_strTstat5d;break;//Tstat5A
					//case 17:stemp=g_strTstat5f;break;
					//case 18:stemp=g_strTstat5g;break;
					//case 16:stemp=g_strTstat5e;break;
					//case 19:stemp=g_strTstat5h;break;
					//case 6:stemp=g_strTstat6;break;
					//case 7:stemp=g_strTstat7;break;
					//case PM_NC:stemp=g_strnetWork;break;
					//case LED_PRODUCT_MODEL:stemp=_T("Led");break;//Others
					//case PM_SOLAR:			stemp=_T("Solar");break;
					//case PM_ZIGBEE:			stemp=_T("ZigBee");break;
					//case T3_32I_PRODUCT_MODEL:stemp=_T("T3-32I");break;
					//case T3_8I_16O_PRODUCT_MODEL:stemp=_T("T3-8I-16O");break;
					//case PM_T3IOA:stemp=_T("T3-8O");break;
					//case PM_T38AIOD:stemp=_T("T3-8IOD");break;
					//default:stemp=_T("Others");break;
				case PM_TSTAT5A:
					stemp="TStat5A";
					break;
				case PM_TSTAT5B:
					stemp="TStat5B";
					break;
				case PM_TSTAT5B2:
					stemp="TStat5B2";
					break;
				case PM_TSTAT5C:
					stemp="TStat5C";
					break;
				case PM_TSTAT5D:
					stemp="TStat5D";
					break;
                case PM_TSTAT5E:
                    stemp="TStat5E";
                    break;
                case PM_PM5E:
                    stemp="PM5E";
                    break;
				case PM_TSTATRUNAR:
				    stemp="TStatRunar";
					break;
				case PM_TSTAT5F:
					stemp="TStat5F";
					break;
				case PM_TSTAT5G:
					stemp="TStat5G";
					break;
				case PM_TSTAT5H:
					stemp="TStat5H";
					break;
				case PM_TSTAT6:
					stemp="TStat6";
					break;
                case PM_TSTAT5i:
                    stemp="TStat5i";
                    break;
                case PM_TSTAT8:
                    stemp="TStat8";
                    break;
				case PM_TSTAT8_WIFI:
					stemp = "TStat8_Wifi";
					break;
				case PM_TSTAT8_OCC:
					stemp = "TStat8_Occ";
					break;
				case PM_TSTAT7_ARM:
					stemp = "TStat7_ARM";
					break;
				case PM_TSTAT8_220V:
					stemp = "TStat8_220V";
					break;
				case PM_TSTAT7:
					stemp="TStat7";
					break;
				case PM_NC:
					stemp="NC";
					break;
				case PM_CM5:
					stemp ="CM5";
					break;
				case PM_LightingController:
					stemp = "LC";
					break;
				case  PM_CO2_NET:
					stemp = "CO2 Net";
					break;
				case  PM_CO2_RS485:
					stemp = "CO2";
					break;
				case  STM32_CO2_NET:
					stemp = "CO2 Net";
					break;
				case  STM32_CO2_RS485:
					stemp = "CO2";
					break;
                case  PM_PRESSURE_SENSOR:
                    stemp = "Pressure";
                    break;
               
				case  PM_CO2_NODE:
					stemp = "CO2 Node";
					break;
				case PM_TSTAT6_HUM_Chamber:
					stemp =g_strHumChamber;
					break;

				case PM_T3PT10 :
					stemp="T3-PT10";
					break;
				case PM_T3IOA :
					stemp="T3-8O";
					break;
				case PM_T332AI :
					stemp="T3-32AI";
					break;
				case  PM_T38AI16O :
					stemp="T3-8AI160";
					break;
				case PM_T38I13O :
					stemp="T3-8I13O";
					break;
				case PM_T3PERFORMANCE :
					stemp="T3-Performance";
					break;
				case PM_T34AO :
					stemp="T3-4AO";
					break;
				case PM_T36CT :
					stemp="T3-6CT";
					break;
                case PM_T322AI:
                    stemp="T3-22I";
                    break;
                case PM_T38AI8AO6DO:
                    stemp="T3-8AI8AO6DO";
                    break;
				default:
					stemp="TStat";
					break;


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
//					TCHAR pbuf[50];
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
			for(int i=0;i<(int)m_grid_flash.size();i++)
				m_FlexGrid.put_TextMatrix(i+1,8,m_strHexFile);
		}
	}

	void CGridFlashDlg::OnBnClickedChecktoall()
	{	
		if(m_AllCheckBtn.GetCheck()==BST_CHECKED)
		{
			for(int i=0;i<(int)m_grid_flash.size();i++)
				m_FlexGrid.put_TextMatrix(i+1,8,m_strHexFile);
		}
		if(m_AllCheckBtn.GetCheck()==BST_UNCHECKED)
		{
			for(int i=0;i<(int)m_grid_flash.size();i++)
				m_FlexGrid.put_TextMatrix(i+1,8,_T(""));
		}
	}

	void CGridFlashDlg::Read_FromTable()
	{
		m_EnableSaveBtn.EnableWindow(FALSE);
		int iitemp;
		m_bFinished=FALSE;
		GetDlgItem(IDC_FLASHBUTTON)->EnableWindow(FALSE);
		for(iitemp=0;iitemp<(int)m_grid_flash.size();iitemp++)
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
			else if(str_temp==_T("T3-PT10"))
				m_grid_flash.at(iitemp).device =PM_T3PT10;//Others
			else if(str_temp==_T("T3-8O"))
				m_grid_flash.at(iitemp).device =PM_T3IOA;//Others
			else if(str_temp==_T("T3-8I16O"))
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

	 ////create thread,read information	
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

		GetExitCodeThread(pThread_flash->m_hThread,&dwExidCode);
		if(dwExidCode==STILL_ACTIVE)
		{
			TerminateThread(pThread_flash->m_hThread,dwExidCode);
			pThread_flash=NULL;
			Sleep(100);
		}
		SetCommunicationType(0);
		close_com();
		SetCommunicationType(1);
		close_com();
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
	 


