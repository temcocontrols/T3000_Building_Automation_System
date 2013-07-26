// GridLoad.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "GridLoad.h"
#include "MainFrm.h"
#include "globle_function.h"

namespace NC_Define
{	
	const CString strSERIALNO= _T("Serial#");
	const CString strMBID = _T("ID");
	const CString strBAUDRATE = _T("Baudrate");
	const CString strHWVERSION= _T("H/W ver");
	const CString strSWVERSION= _T("S/W ver");
	const CString strDEVICETYPE= _T("Device");
	const CString strFILENAME = _T("Configure File");
	const CString strENABLE = _T("Enable");
	const CString strSTATUS= _T("Status");
	const CString strCONNECTION= _T("Connection");
	const CString stLOADEVENT= _T("Load Event");

	const int COL_SERIALNO = 1;
	const int COL_MBID = 2;
	const int COL_BAUDRATE = 3;
	const int COL_HWVERSION= 4;
	const int COL_SWVERSION =5;
	const int COL_DEVICETYPE = 6;
	const int COL_FILENAME = 7;
	const int COL_ENABLE = 8;
	const int COL_STATUS = 9;
	const int COL_CONNECTION = 10;
	const int COL_LOADEVENT = 11;

};


using namespace NC_Define;

//CCriticalSection critical_section;
UINT _NC_LoadThread(LPVOID pParam);

UINT Check_LoadStatus(LPVOID pParameter)
{
	CGridLoad* pDlg=(CGridLoad*)pParameter;
	//pDlg->BeginWaitCursor();
	UINT iitemp;
	UINT nSerial;
	UINT nSerialNumber;
	for(iitemp=0;iitemp<pDlg->m_grid_load.size();iitemp++)
	{
		if (pDlg->m_bStopLoadingfile)
		{
			break;
		}

		int nTempID;
		int nID=-1;
		nTempID=pDlg->m_grid_load.at(iitemp).ID;
		nSerial=pDlg->m_grid_load.at(iitemp).serialnumber;

	//	nID=read_one(nTempID,6,2);
		
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

			pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,COL_CONNECTION ,_T("On line"));
			pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,COL_ENABLE ,_T("True"));//flash select
		}
		else
		{
			pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,COL_CONNECTION ,_T("Off line"));
			pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,COL_ENABLE ,_T("False"));//flash select
		}
	}

	//pDlg->EndWaitCursor();
	return 1;
}


UINT run_back_ground_load_thread(LPVOID pParam)
{
	CGridLoad* pDlg=(CGridLoad*)pParam;
	pDlg->m_infoListBox.ResetContent();

	int failure_number=0;
	int disabled_number=0;
	

	for(UINT iitemp=0;iitemp<pDlg->m_grid_load.size();iitemp++)
	{
		CString log_file_path;
		CFile config_file;//the hex file
		CStdioFile log_file;
		bool log_file_opened=false;
		int nowmoder=0;
		if(pDlg->m_bStopLoadingfile)
		{
			break;
		}
		if(!pDlg->m_grid_load.at(iitemp).flash_or_no)
			continue;
		
		if(!pDlg->CheckLoadFileForTStat(pDlg->m_grid_load.at(iitemp)))
		{
			continue;
		}

		int nID=-1;
		int nTempID =pDlg->m_grid_load.at(iitemp).ID;//get temp tstat id
		if (g_CommunicationType==0)
		{
			int baudrate=19200;
			Change_BaudRate(19200);
			nID=read_one(nTempID,6);
			if(nID<=0)
			{
			baudrate=9600;
			Change_BaudRate(9600);
			nID=read_one(nTempID,6);
			}
		}
		if (g_CommunicationType==1)
		{
			nID=read_one(nTempID,6);
		}
		now_tstat_id=nID;
		if(now_tstat_id>0)
		{
			pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,COL_CONNECTION,_T("On line"));

		}
		else{
			pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,COL_CONNECTION,_T("Off line"));
			pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,COL_ENABLE,_T("False"));//flash select
			failure_number++;	
			pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,COL_STATUS,_T("Error"));
			continue;
			}

		load_file_every_step temppp;
		temppp.eight_step=temppp.fifth_step=temppp.first_step=temppp.keep_down=temppp.second_step=temppp.seven_step=temppp.sixth_step=temppp.third_step=temppp.thurth_step=FALSE;
		bool open_file=false;
		CString strSerial;
		strSerial.Format(_T("%d"),pDlg->m_grid_load.at(iitemp).serialnumber);
		strSerial+=_T(".txt");
		log_file_path=pDlg->m_strLogFilePath+_T("\\")+strSerial;

		WIN32_FIND_DATA fData;
		HANDLE hFile=NULL;
		hFile = FindFirstFile(log_file_path,&fData);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			CFile::Remove(log_file_path);
			pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,COL_LOADEVENT,_T(""));
		}
		FindClose(hFile);
			
		//added the header marker.
		if(log_file.Open(log_file_path,CFile::modeCreate | CFile::modeReadWrite)!=0)
		{
			log_file_opened=true;
			CString strHex=pDlg->m_grid_load.at(iitemp).hex_file_path;
			if(!strHex.IsEmpty())
			{
				log_file.WriteString(strHex);
			}
			CString ttt=_T("\r\n\r\n");
			log_file.Write(ttt.GetString(),ttt.GetLength());
		}

		if(config_file.Open(pDlg->m_grid_load.at(iitemp).hex_file_path.GetString() ,CFile::modeRead | CFile::shareDenyNone))
		{
	//		write_one(now_tstat_id,324,0);	
			CString strPath=config_file.GetFilePath();
			CString strFileName=config_file.GetFileTitle();
			open_file=true;
			pDlg->m_infoListBox.InsertString(0,_T(""));
			nowmoder=read_one(now_tstat_id,7,7);
			if(log_file_opened==true)
				{
				if (nowmoder==6||nowmoder==7)
				{
				LoadFile2Tstat67(temppp,(LPTSTR)(LPCTSTR)pDlg->m_grid_load.at(iitemp).hex_file_path.GetString(),&log_file);
				} 
				else
				{
				LoadFile2Tstat(temppp,(LPTSTR)(LPCTSTR)pDlg->m_grid_load.at(iitemp).hex_file_path.GetString(),&log_file);
				}
				
				}
			if(find_load_file_error(temppp))
			{
				CString for_showing_text;
				for_showing_text.Format(_T("ID :%d Load found Error"),now_tstat_id);
				change_showing_text_variable(for_showing_text);
				pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,COL_LOADEVENT,_T(""));
				failure_number++;	
				pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,COL_STATUS,_T("Error"));
			}
			else
			{
				CString for_showing_text;
				for_showing_text.Format(_T("ID :%d Load Success"),now_tstat_id);
				log_file.WriteString(for_showing_text);
				change_showing_text_variable(for_showing_text);
				pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,COL_ENABLE,_T("False"));
				pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,COL_STATUS,_T("OK"));

				
				SYSTEMTIME stUTC, stLocal;
				CString strDatatime;
				FileTimeToSystemTime(&(fData.ftLastWriteTime), &stUTC);
				BOOL bRet= SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
				if (!bRet)
				{
					GetSystemTime(&stUTC);
					SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
				}
				strDatatime.Format(_T("%02d. %02d %d, %02d:%02d"), stLocal.wDay,stLocal.wMonth,stLocal.wYear,stLocal.wHour,stLocal.wMinute);
				pDlg->m_FlexGrid.put_TextMatrix(iitemp+1,COL_LOADEVENT,strDatatime);
			}
		//	else
				//failure_number++;
				config_file.Close();//close file
				if(log_file.m_hFile!=NULL)
					log_file.Close();
				
		}		
		//p_list_box->ResetContent();
	}
	pDlg->m_bTstatLoadFinished=TRUE;

	pDlg->m_eTstatLoadFinish.SetEvent(); // signal
	return 1;
}
// CGridLoad dialog

IMPLEMENT_DYNAMIC(CGridLoad, CDialog)

CGridLoad::CGridLoad(CWnd* pParent /*=NULL*/)
	: CDialog(CGridLoad::IDD, pParent)
{
	m_pLoadBackCheckThread=NULL;
	m_ploadThread=NULL;
	m_strInfoText=_T("");

	m_bStopLoadingfile=false;
	m_bTstatLoadFinished=FALSE;
	m_bNCLoadFinished = FALSE;

	

}

CGridLoad::~CGridLoad()
{

}

void CGridLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_FlexGrid);
	DDX_Control(pDX, IDC_SUBLIST, m_SubListCtrl);
	DDX_Control(pDX, IDC_ALLCHECK, m_AllCheckBtn);
	DDX_Control(pDX, IDC_HEXPATHNAMEEDIT, m_hexFilePathNameEdit);
	DDX_Control(pDX, IDC_LOADLOGLIST, m_infoListBox);
}


BEGIN_MESSAGE_MAP(CGridLoad, CDialog)
	ON_BN_CLICKED(IDC_BUTTON4, &CGridLoad::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_OPENBUTTON, &CGridLoad::OnBnClickedOpenbutton)
	ON_BN_CLICKED(IDC_ALLCHECK, &CGridLoad::OnBnClickedAllcheck)
	ON_CBN_SELCHANGE(IDC_SUBLIST, &CGridLoad::OnCbnSelchangeSublist)
	ON_BN_CLICKED(IDC_CONFIGBUTTON, &CGridLoad::OnBnClickedConfigbutton)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BROWSETLOGBTN, &CGridLoad::OnBnClickedBrowsetlogbtn)
	ON_BN_CLICKED(IDC_CANCELLOADBUTTON, &CGridLoad::OnBnClickedCancelloadbutton)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO1, &CGridLoad::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CGridLoad::OnBnClickedRadio2)
END_MESSAGE_MAP()
// CGridLoad message handlers

void CGridLoad::OnBnClickedButton4()
{
	SendMessage(WM_CLOSE,0,0);
	// TODO: Add your control notification handler code here
}

BOOL CGridLoad::OnInitDialog()
{
	CDialog::OnInitDialog();

	initRadio();

	m_SubListCtrl.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CANCELLOADBUTTON)->ShowWindow(SW_HIDE);

	m_strLogFilePath=g_strExePth+_T("Load_config_Log");
	CreateDirectory(m_strLogFilePath,NULL);//create directory file

	m_FlexGrid.put_TextMatrix(0,COL_SERIALNO ,strSERIALNO );
	m_FlexGrid.put_TextMatrix(0,COL_MBID ,strMBID );
	m_FlexGrid.put_TextMatrix(0,COL_BAUDRATE ,strBAUDRATE );
	m_FlexGrid.put_TextMatrix(0,COL_HWVERSION ,strHWVERSION);
	m_FlexGrid.put_TextMatrix(0,COL_SWVERSION ,strSWVERSION);
	m_FlexGrid.put_TextMatrix(0,COL_DEVICETYPE ,strDEVICETYPE);
	m_FlexGrid.put_TextMatrix(0,COL_FILENAME ,strFILENAME );
	m_FlexGrid.put_TextMatrix(0,COL_ENABLE ,strENABLE );
	m_FlexGrid.put_TextMatrix(0,COL_STATUS ,strSTATUS);
	m_FlexGrid.put_TextMatrix(0,COL_CONNECTION ,strCONNECTION);
	m_FlexGrid.put_TextMatrix(0,COL_LOADEVENT ,stLOADEVENT);
	
	m_FlexGrid.put_ColWidth(0,0);
	m_FlexGrid.put_ColWidth(COL_SERIALNO,800);
	m_FlexGrid.put_ColWidth(COL_MBID,400);
	m_FlexGrid.put_ColWidth(COL_BAUDRATE,600);
	m_FlexGrid.put_ColWidth(COL_HWVERSION,600);
	m_FlexGrid.put_ColWidth(COL_SWVERSION,600);
	m_FlexGrid.put_ColWidth(COL_DEVICETYPE,900);
	m_FlexGrid.put_ColWidth(COL_FILENAME,3400);
	m_FlexGrid.put_ColWidth(COL_ENABLE,800);
	m_FlexGrid.put_ColWidth(COL_STATUS,1000);
	m_FlexGrid.put_ColWidth(COL_CONNECTION,900);
	m_FlexGrid.put_ColWidth(COL_LOADEVENT,1700);
	//add sub net to combolist
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
//	m_srScanSubNetNameCobox.AddString(_T("All subNets..."));
	CString strTemp;
	for(UINT i=0;i<pFrame->m_subNetLst.size();i++)
	{
		strTemp=pFrame->m_subNetLst.at(i).strBuildingName;
		m_SubListCtrl.AddString(strTemp);
		if(strTemp.CompareNoCase(pFrame->m_strCurSubBuldingName)==0)
			m_SubListCtrl.SetCurSel(i);
	}

	m_grid_load.clear();
//	found_net_work();//////////////////inspect found net work controller

	for(UINT i=0;i<pFrame->m_product.size();i++)
	{
// 		if(PM_NC!=pFrame->m_product.at(i).product_class_id && !m_bProductType)
// 		{
// 			continue;
// 		}
// 		if(PM_NC==pFrame->m_product.at(i).product_class_id && m_bProductType)//||NET_WORK_OR485_PRODUCT_MODEL==pFrame->m_product.at(i).product_class_id)
// 			continue;
	
		GRID_LOAD temp_grid_flash;
	//	tstat_id=temp_grid_flash.ID=pFrame->m_product.at(i).product_id;//ID       for get_serialnumber function
		temp_grid_flash.ID=pFrame->m_product.at(i).product_id;//ID       for get_serialnumber function
		temp_grid_flash.baudrate=pFrame->m_product.at(i).baudrate;//baudrate
		temp_grid_flash.serialnumber =pFrame->m_product.at(i).serial_number ;//serialnumber			
		temp_grid_flash.flash_or_no=true;
		temp_grid_flash.hardware_revisin=(short)pFrame->m_product.at(i).hardware_version;//read_one(j ,8,1);//m_product.at(i).hardware_version ;
		temp_grid_flash.device=pFrame->m_product.at(i).product_class_id ;
		temp_grid_flash.software_version=pFrame->m_product.at(i).software_version;//get_tstat_version(j);

		m_grid_load.push_back(temp_grid_flash);
	}

	m_FlexGrid.put_Rows(m_grid_load.size()+2);

	for(UINT i=0;i<m_grid_load.size();i++)
	{
		strTemp.Format(_T("%ld"),m_grid_load.at(i).serialnumber);
		m_FlexGrid.put_TextMatrix(i+1,COL_SERIALNO ,strTemp);//serialnumber

		strTemp.Format(_T("%d"),m_grid_load.at(i).ID);
		m_FlexGrid.put_TextMatrix(i+1,COL_MBID ,strTemp);//ID

		strTemp.Format(_T("%d"),m_grid_load.at(i).baudrate);
		m_FlexGrid.put_TextMatrix(i+1,COL_BAUDRATE ,strTemp);//baudrate

		strTemp.Format(_T("%d"),m_grid_load.at(i).hardware_revisin);					
		m_FlexGrid.put_TextMatrix(i+1,COL_HWVERSION ,strTemp);//hardware revision
	
		strTemp.Format(_T("%0.1f"),m_grid_load.at(i).software_version);					
		m_FlexGrid.put_TextMatrix(i+1,COL_SWVERSION ,strTemp);//hardware revision

		switch(m_grid_load.at(i).device)
		{
			case 2:strTemp=g_strTstat5a;break;
			case 1:strTemp=g_strTstat5b;break;
			case 3:strTemp=g_strTstat5b;break;
			case 4:strTemp=g_strTstat5c;break;
			case 12:strTemp=g_strTstat5d;break;
			case PM_NC:strTemp=g_strnetWork;break;
			case 17: strTemp=g_strTstat5f;break;
			case 18:strTemp=g_strTstat5g;break;
			case 16:strTemp=g_strTstat5e;break;
			case 19:strTemp=g_strTstat5h;break;
			case 6:strTemp=g_strTstat6;break;
			case 7:strTemp=g_strTstat7;break;
			case PM_TSTAT6_HUM_Chamber:strTemp=g_strHumChamber;break;

			default:strTemp=g_strTstat5a;break;
		}
		m_FlexGrid.put_TextMatrix(i+1,COL_DEVICETYPE ,strTemp);//product model ===== software version
		if(m_grid_load.at(i).flash_or_no==false)
			m_FlexGrid.put_TextMatrix(i+1,COL_ENABLE ,_T("false"));//flash select
		else
			m_FlexGrid.put_TextMatrix(i+1,COL_ENABLE ,_T("True"));//flash select
		m_FlexGrid.put_TextMatrix(i+1,COL_STATUS ,_T(""));

		if (m_grid_load.at(i).hardware_revisin<0)
		{
			m_FlexGrid.put_TextMatrix(i+1,COL_CONNECTION ,_T("Off line"));
			m_FlexGrid.put_TextMatrix(i+1, COL_ENABLE ,_T("False"));//flash select
		}
		else
		{
			m_FlexGrid.put_TextMatrix(i+1,COL_CONNECTION ,_T("On line"));
			m_FlexGrid.put_TextMatrix(i+1, COL_ENABLE ,_T("True"));//flash select
		}

		CString strSerial;
		strSerial.Format(_T("%d"),m_grid_load.at(i).serialnumber);
		CString strLogFile;
		strLogFile=m_strLogFilePath+_T("\\")+strSerial+_T(".txt");
		WIN32_FIND_DATA fData;
		HANDLE hFile=NULL;
		hFile = FindFirstFile(strLogFile,&fData);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			CFileStatus   st;
			CStdioFile fileLog(strLogFile,CFile::modeReadWrite | CFile::modeNoTruncate|CFile::typeText);
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
				if(strLastLine.Find(_T("Load Success"))>0)
				{
					SYSTEMTIME stUTC, stLocal;
					CString strDatatime;
					FileTimeToSystemTime(&(fData.ftLastWriteTime), &stUTC);
					BOOL bRet = SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
					if (!bRet)
					{
						GetSystemTime(&stUTC);
						SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
					}
		    		strDatatime.Format(_T("%02d. %02d %d, %02d:%02d"), stLocal.wDay,stLocal.wMonth,stLocal.wYear,stLocal.wHour,stLocal.wMinute);
					m_FlexGrid.put_TextMatrix(i+1,COL_LOADEVENT,strDatatime);

				}
			}
			fileLog.Close();
		}
		FindClose(hFile);

	}

		
	m_pLoadBackCheckThread=AfxBeginThread(Check_LoadStatus,this);////////////////////////create thread,read information	
	m_pLoadBackCheckThread->m_bAutoDelete=false;
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CGridLoad::initRadio()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_RADIO1);
	pBtn->SetCheck(1);
	pBtn = (CButton*)GetDlgItem(IDC_RADIO2);
	pBtn->SetCheck(0);

	m_bProductType = TRUE;
	
}


void CGridLoad::OnBnClickedOpenbutton()
{
	CString strFilter = _T("txt File|*.txt|all File|*.*||");
	CFileDialog dlg(true,_T("hex"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
	dlg.DoModal();
	CString strTemp;
	strTemp=dlg.GetPathName();		
	//save the path
	CFile default_file;
	if(strTemp.IsEmpty())
	{
		return;
	}
	m_strHexFile=strTemp;
	m_hexFilePathNameEdit.SetWindowText(m_strHexFile);

	if(m_AllCheckBtn.GetCheck()==BST_CHECKED)
	{
		for(UINT i=0;i<m_grid_load.size();i++)
			m_FlexGrid.put_TextMatrix(i+1,COL_FILENAME,m_strHexFile);
	}
	//UpdateData(false);
	//click_1=true;
}

void CGridLoad::OnBnClickedAllcheck()
{	
	if(m_AllCheckBtn.GetCheck()==BST_CHECKED)
	{
		for(UINT i=0;i<m_grid_load.size();i++)
			m_FlexGrid.put_TextMatrix(i+1,COL_FILENAME,m_strHexFile);
	}
 	if(m_AllCheckBtn.GetCheck()==BST_UNCHECKED)
	{
		for(UINT i=0;i<m_grid_load.size();i++)
			m_FlexGrid.put_TextMatrix(i+1,COL_FILENAME,_T(""));
	}
}

void CGridLoad::OnCbnSelchangeSublist()
{
	return;

	const CString strSERIALNO= _T("Serial#");
	const CString strMBID = _T("ID");
	const CString strBAUDRATE = _T("Baudrate");
	const CString strHWVERSION= _T("Hardware version");
	const CString strSWVERSION= _T("Software version");
	const CString strDEVICETYPE= _T("Device");
	const CString strFILENAME = _T("Configure File");
	const CString strENABLE = _T("Enable");
	const CString strSTATUS= _T("Status");
	const CString strCONNECTION= _T("Connetion");
	const CString stLOADEVENT= _T("Load Event");

	int nRows=m_FlexGrid.get_Rows();
	for(int i=0;i<nRows;i++)
	{
		for(int k=0;k<11;k++)
		{
		//	m_FlexGrid.
		}
	}
	m_grid_load.clear();
	m_FlexGrid.Clear();


	int nSelect=m_SubListCtrl.GetCurSel();
	if(nSelect<0)
		return;
	CString strCurSubNet;
	CString strMainBuilding;
	m_SubListCtrl.GetLBText(nSelect,strCurSubNet);
	if(strCurSubNet.IsEmpty())
		return;
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	strMainBuilding=pFrame->m_strCurMainBuildingName;
	if(strMainBuilding.IsEmpty())
		return;
	CString str_temp;
	_ConnectionPtr pCon=NULL;
	_RecordsetPtr pRs=NULL;
	pCon.CreateInstance(_T("ADODB.Connection"));
	pRs.CreateInstance(_T("ADODB.Recordset"));
	pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);
	str_temp.Format(_T("select * from ALL_NODE where MainBuilding_Name = '%s' and Building_Name = '%s'"),strMainBuilding,strCurSubNet);
	pRs->Open(str_temp.GetString(),_variant_t((IDispatch *)pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
	if(pRs!=NULL||pRs->GetRecordCount()<=0)
	{
		if(pRs->State) 
			pRs->Close(); 
		if(pCon->State)
			pCon->Close(); 
		return;
	}
	_variant_t temp_variant;
	CString strTemp;

	while(VARIANT_FALSE==pRs->EndOfFile)
	{
		GRID_LOAD temp_grid_flash;
		temp_variant=pRs->GetCollect("Serial_ID");//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		temp_grid_flash.serialnumber=_wtoi(strTemp);

		temp_variant=pRs->GetCollect("Product_ID");//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		temp_grid_flash.ID=_wtoi(strTemp);

		temp_variant=pRs->GetCollect(_T("Bautrate"));//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		temp_grid_flash.baudrate=_wtoi(strTemp);
		
		temp_variant=pRs->GetCollect(_T("Hardware_Ver"));//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		temp_grid_flash.hardware_revisin=(short)_wtof(strTemp);

		temp_variant=pRs->GetCollect(_T("Software_Ver"));//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		temp_grid_flash.software_version=(short)_wtof(strTemp);

		temp_variant=pRs->GetCollect(_T("EPsize"));//
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		temp_grid_flash.EpSize=_wtoi(strTemp);

		m_grid_load.push_back(temp_grid_flash);
		pRs->MoveNext();
	}
}
BEGIN_EVENTSINK_MAP(CGridLoad, CDialog)
	ON_EVENT(CGridLoad, IDC_MSFLEXGRID1, DISPID_CLICK, CGridLoad::ClickMsflexgrid1, VTS_NONE)
END_EVENTSINK_MAP()

void CGridLoad::ClickMsflexgrid1()
{
	long lRow,lCol;
	lRow = m_FlexGrid.get_RowSel();//��ȡ������к�	
	lCol = m_FlexGrid.get_ColSel(); //��ȡ������к�
	UpdateData(false);
	if(lRow==0 || lCol==0)
		return;
	int nRowsCounts=m_FlexGrid.get_Rows();
	
	if(lRow==nRowsCounts-1)
		return;
	CRect rect;
	m_FlexGrid.GetWindowRect(rect); //��ȡ���ؼ��Ĵ��ھ���
	ScreenToClient(rect); //ת��Ϊ�ͻ�������	
	// MSFlexGrid�ؼ��ĺ����ĳ��ȵ�λ��"�(twips)"��
	//��Ҫ����ת��Ϊ���أ�1440�= 1Ӣ��
	CDC* pDC =GetDC();
	//�������ص��羵�ת������
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//����ѡ�и�����Ͻǵ�����(����Ϊ��λ)
	long y = m_FlexGrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_FlexGrid.get_ColPos(lCol)/nTwipsPerDotX;
	//����ѡ�и�ĳߴ�(����Ϊ��λ)����1��ʵ�ʵ����У����ּ�1��Ч������
	long width = m_FlexGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_FlexGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//�γ�ѡ�и����ڵľ�������
	CRect rc(x,y,x+width,y+height);
	//ת������ԶԻ��������
	rc.OffsetRect(rect.left+1,rect.top+1);
	//��ȡѡ�и���ı���Ϣ
	CString strValue = m_FlexGrid.get_TextMatrix(lRow,lCol);
	row_row=lRow;
	row_col=lCol;
	bool view=true;
	if(rc.top<rect.top+height || rc.bottom>rect.bottom+1 || rc.left<rect.left+1 || rc.right>rect.right+1)
		view=false;
	if(row_col==COL_FILENAME)
	{
		CString strFilter = _T("txt File|*.txt|all File|*.*||");
		CFileDialog dlg(true,_T("txt"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
		dlg.DoModal();
		m_strHexFile=dlg.GetPathName();	
		if(!m_strHexFile.IsEmpty())
		m_FlexGrid.put_TextMatrix(row_row,row_col,m_strHexFile);				
	}
	else if(row_col==COL_ENABLE)
	{
		CString temp_str=m_FlexGrid.get_TextMatrix(row_row,row_col);
		if(temp_str.IsEmpty())
			return;
		if(temp_str.CompareNoCase(_T("True"))==0)
		{
			m_FlexGrid.put_TextMatrix(row_row,row_col,_T("False"));
			m_grid_load.at(row_row-1).flash_or_no=false;
		}
		else
		{
			m_grid_load.at(row_row-1).flash_or_no=true;
			m_FlexGrid.put_TextMatrix(row_row,row_col,_T("True"));
		}
	
	}
	else if(lCol==COL_SERIALNO  || lCol==COL_MBID  || lCol==COL_BAUDRATE  || lCol==COL_HWVERSION  || lCol==COL_SWVERSION  ||lCol==COL_DEVICETYPE   || lCol==COL_STATUS )
	{
		return;
	}

}

void CGridLoad::OnBnClickedConfigbutton()
{
	UINT iitemp;
	m_bTstatLoadFinished=FALSE;
	m_bNCLoadFinished = FALSE;

	GetDlgItem(IDC_CONFIGBUTTON)->EnableWindow(FALSE);

	for(iitemp=0;iitemp<m_grid_load.size();iitemp++)
	{//get the grid value
		m_grid_load.at(iitemp).hardware_revisin = _wtoi(m_FlexGrid.get_TextMatrix(iitemp+1,COL_HWVERSION));//hardware rev
		CString str_temp=m_FlexGrid.get_TextMatrix(iitemp+1,COL_DEVICETYPE);//software version ;;;;;product model
			
			if(str_temp==g_strTstat5a)
				m_grid_load.at(iitemp).device =2;//Tstat5
			else if(str_temp==g_strTstat5b)
				m_grid_load.at(iitemp).device =1;//Tstat5
			else if(str_temp==g_strTstat5b2)
				m_grid_load.at(iitemp).device =3;//Tstat5
			else if(str_temp==g_strTstat5c)
				m_grid_load.at(iitemp).device =4;//Tstat5
			else if(str_temp==g_strTstat5d)
				m_grid_load.at(iitemp).device =12;//Tstat5
			else if(str_temp==g_strnetWork)
				m_grid_load.at(iitemp).device =PM_NC;
			else if(str_temp==g_strTstat5f)
			   m_grid_load.at(iitemp).device =17;//Tstat5f
			else if(str_temp==g_strTstat5g)
			   m_grid_load.at(iitemp).device =18;//Tstat5g
			else if(str_temp==g_strTstat5e)
			   m_grid_load.at(iitemp).device =16;//Tstat5e
			else if(str_temp==g_strTstat5h)
			   m_grid_load.at(iitemp).device =19;//Tstat5e
			else if(str_temp==g_strTstat6)
				m_grid_load.at(iitemp).device =6;//Tstat6
			else if(str_temp==g_strTstat5h)
				m_grid_load.at(iitemp).device =7;//Tstat7
			m_grid_load.at(iitemp).hex_file_path=m_FlexGrid.get_TextMatrix(iitemp+1,COL_FILENAME);//hex file path
			m_grid_load.at(iitemp).hex_file_path.Trim();
			str_temp=m_FlexGrid.get_TextMatrix(iitemp+1,COL_ENABLE);//flash select 
			if(str_temp.CompareNoCase(_T("true"))==0)
				m_grid_load.at(iitemp).flash_or_no =true;//Tstat5A
			else
				m_grid_load.at(iitemp).flash_or_no =false;//Tstat5A		
		}
	
		DWORD dwExidCode;
		GetExitCodeThread(m_pLoadBackCheckThread->m_hThread,&dwExidCode);
		if(dwExidCode==STILL_ACTIVE)
		{
			TerminateThread(m_pLoadBackCheckThread->m_hThread,dwExidCode);
			m_pLoadBackCheckThread=NULL;

			Sleep(300);
		}

	//	SetPaneString("Loading...");
		//m_ploadThread=AfxBeginThread(run_back_ground_load_thread,this);////////////////////////create thread,read information	
// 		if (m_bProductType)
// 		{
// 			m_ploadThread=AfxBeginThread(run_back_ground_load_thread,this);////////////////////////create thread,read information	
// 		}
// 		else
// 		{
// 			m_ploadThread=AfxBeginThread(_NC_LoadThread,this);////////////////////////create thread,read information	
// 		}
		{
			m_eTstatLoadFinish.ResetEvent(); //no signal
			m_ploadThread=AfxBeginThread(run_back_ground_load_thread,this);////////////////////////create thread,read information	
			m_ploadThread=AfxBeginThread(_NC_LoadThread,this);////////////////////////create thread,read information	
		}
	//	showing_text="";
		SetTimer(1,10,NULL);/////////////10 ms is better 

}


void CGridLoad::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		CString strTemp;
		critical_section.Lock();
		strTemp=showing_text;
		critical_section.Unlock();

		if(m_strInfoText.CompareNoCase(strTemp)!=0)
		{
			m_strInfoText=strTemp;
			int n=-1;
			n=m_strInfoText.Find(_T("\r\n"));
			if(n>=0)
			m_strInfoText.Left(n);
			m_infoListBox.InsertString(0,m_strInfoText);
		}
		
	//	if(m_strInfoText.Find(_T("Success"))!=-1 || m_strInfoText.Find(_T("Failure"))!=-1)
		if(m_bTstatLoadFinished && m_bNCLoadFinished)
		{			
			GetDlgItem(IDC_CONFIGBUTTON)->EnableWindow(TRUE);

			KillTimer(1);
			m_infoListBox.InsertString(0,m_strInfoText.GetString());
			CString temp_str=_T("To view the file please click on the \"Log File\" option");
			m_strInfoText.Empty();
			m_strInfoText=temp_str+_T("\n");
			m_infoListBox.InsertString(0,temp_str.GetString());
			temp_str=_T("Loading status are stored into a Log file.");
			m_strInfoText=m_strInfoText+_T("\n")+temp_str+_T("\n");
			m_infoListBox.InsertString(-1,temp_str.GetString());

			AfxMessageBox(m_strInfoText);
		}
	//	else
	//		m_infoListBox.InsertString(0,m_strInfoText);
	}

	CDialog::OnTimer(nIDEvent);
}
/*
void CGridLoad::_change_showing_text_variable(CString str)
{
	critical_section.Lock();
	showing_text=str;
	critical_section.Unlock();
	
}
\*/



void CGridLoad::OnBnClickedBrowsetlogbtn()
{
	ShellExecute(this->m_hWnd, _T("open"), m_strLogFilePath, NULL, NULL, SW_SHOWNORMAL);
}

void CGridLoad::OnBnClickedCancelloadbutton()
{
	m_bStopLoadingfile=TRUE;
}

void CGridLoad::OnDestroy()
{
	CDialog::OnDestroy();

	m_bStopLoadingfile=TRUE;
	Sleep(200);
	DWORD dwExidCode;
	if(m_pLoadBackCheckThread)
	{
		//Sleep(300);
		GetExitCodeThread(m_pLoadBackCheckThread->m_hThread,&dwExidCode);
		if(dwExidCode==STILL_ACTIVE)
		{
			TerminateThread(m_pLoadBackCheckThread->m_hThread,dwExidCode);
			m_pLoadBackCheckThread=NULL;
		}
	}

	if(m_ploadThread!=NULL)
	{
		GetExitCodeThread(m_ploadThread->m_hThread,&dwExidCode);
		if(dwExidCode==STILL_ACTIVE)
		{
			TerminateThread(m_ploadThread->m_hThread,dwExidCode);
			m_ploadThread=NULL;
			Sleep(300);
		}
	}

}

void CGridLoad::OnBnClickedRadio1()
{
	m_bProductType = TRUE;
	m_FlexGrid.Clear();
	LoadDeviceToGrid();
}

void CGridLoad::OnBnClickedRadio2()
{
	m_bProductType = FALSE;
	m_FlexGrid.Clear();
	LoadDeviceToGrid();
}



void CGridLoad::LoadDeviceToGrid()
{
	m_SubListCtrl.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CANCELLOADBUTTON)->ShowWindow(SW_HIDE);

	m_strLogFilePath=g_strExePth+_T("Load_config_Log");
	CreateDirectory(m_strLogFilePath,NULL);//create directory file

	m_FlexGrid.put_TextMatrix(0,COL_SERIALNO ,strSERIALNO );
	m_FlexGrid.put_TextMatrix(0,COL_MBID ,strMBID );
	m_FlexGrid.put_TextMatrix(0,COL_BAUDRATE ,strBAUDRATE );
	m_FlexGrid.put_TextMatrix(0,COL_HWVERSION ,strHWVERSION);
	m_FlexGrid.put_TextMatrix(0,COL_SWVERSION ,strSWVERSION);
	m_FlexGrid.put_TextMatrix(0,COL_DEVICETYPE ,strDEVICETYPE);
	m_FlexGrid.put_TextMatrix(0,COL_FILENAME ,strFILENAME );
	m_FlexGrid.put_TextMatrix(0,COL_ENABLE ,strENABLE );
	m_FlexGrid.put_TextMatrix(0,COL_STATUS ,strSTATUS);
	m_FlexGrid.put_TextMatrix(0,COL_CONNECTION ,strCONNECTION);
	m_FlexGrid.put_TextMatrix(0,COL_LOADEVENT ,stLOADEVENT);

	m_FlexGrid.put_ColWidth(0,0);
	m_FlexGrid.put_ColWidth(COL_SERIALNO,800);
	m_FlexGrid.put_ColWidth(COL_MBID,400);
	m_FlexGrid.put_ColWidth(COL_BAUDRATE,600);
	m_FlexGrid.put_ColWidth(COL_HWVERSION,600);
	m_FlexGrid.put_ColWidth(COL_SWVERSION,600);
	m_FlexGrid.put_ColWidth(COL_DEVICETYPE,900);
	m_FlexGrid.put_ColWidth(COL_FILENAME,3400);
	m_FlexGrid.put_ColWidth(COL_ENABLE,800);
	m_FlexGrid.put_ColWidth(COL_STATUS,1000);
	m_FlexGrid.put_ColWidth(COL_CONNECTION,900);
	m_FlexGrid.put_ColWidth(COL_LOADEVENT,1700);
	//add sub net to combolist
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	//	m_srScanSubNetNameCobox.AddString(_T("All subNets..."));
	CString strTemp;
	for(UINT i=0;i<pFrame->m_subNetLst.size();i++)
	{
		strTemp=pFrame->m_subNetLst.at(i).strBuildingName;
		m_SubListCtrl.AddString(strTemp);
		if(strTemp.CompareNoCase(pFrame->m_strCurSubBuldingName)==0)
			m_SubListCtrl.SetCurSel(i);


	}

	m_grid_load.clear();
	//	found_net_work();//////////////////inspect found net work controller
	for(UINT i=0;i<pFrame->m_product.size();i++)
	{
		GRID_LOAD temp_grid_flash;
		//	tstat_id=temp_grid_flash.ID=pFrame->m_product.at(i).product_id;//ID       for get_serialnumber function
		temp_grid_flash.ID=pFrame->m_product.at(i).product_id;//ID       for get_serialnumber function
		temp_grid_flash.baudrate=pFrame->m_product.at(i).baudrate;//baudrate
		temp_grid_flash.serialnumber =pFrame->m_product.at(i).serial_number ;//serialnumber			
		temp_grid_flash.flash_or_no=true;
		temp_grid_flash.hardware_revisin=(short)pFrame->m_product.at(i).hardware_version;//read_one(j ,8,1);//m_product.at(i).hardware_version ;
		temp_grid_flash.device=pFrame->m_product.at(i).product_class_id ;
		temp_grid_flash.software_version=pFrame->m_product.at(i).software_version;//get_tstat_version(j);
// 		if(PM_NC==temp_grid_flash.device||NET_WORK_OR485_PRODUCT_MODEL==temp_grid_flash.device)
// 			continue;


// 		if(m_bProductType)
// 		{
// 			if(PM_NC==temp_grid_flash.device||NET_WORK_OR485_PRODUCT_MODEL==temp_grid_flash.device)
// 				continue;
// 		}
// 		else
// 		{
// 			if(PM_NC!=temp_grid_flash.device)
// 				continue;
// 		}

		m_grid_load.push_back(temp_grid_flash);
	}

	m_FlexGrid.put_Rows(m_grid_load.size()+2);

	for(UINT i=0;i<m_grid_load.size();i++)
	{
		strTemp.Format(_T("%ld"),m_grid_load.at(i).serialnumber);
		m_FlexGrid.put_TextMatrix(i+1,COL_SERIALNO ,strTemp);//serialnumber

		strTemp.Format(_T("%d"),m_grid_load.at(i).ID);
		m_FlexGrid.put_TextMatrix(i+1,COL_MBID ,strTemp);//ID

		strTemp.Format(_T("%d"),m_grid_load.at(i).baudrate);
		m_FlexGrid.put_TextMatrix(i+1,COL_BAUDRATE ,strTemp);//baudrate

		strTemp.Format(_T("%d"),m_grid_load.at(i).hardware_revisin);					
		m_FlexGrid.put_TextMatrix(i+1,COL_HWVERSION, strTemp);//hardware revision
		strTemp.Format(_T("%0.1f"),m_grid_load.at(i).software_version);					
		m_FlexGrid.put_TextMatrix(i+1,COL_SWVERSION, strTemp);//hardware revision
		switch(m_grid_load.at(i).device)
		{
		case 2:strTemp=g_strTstat5a;break;
		case 1:strTemp=g_strTstat5b;break;
		case 3:strTemp=g_strTstat5b;break;
		case 4:strTemp=g_strTstat5c;break;
		case 12:strTemp=g_strTstat5d;break;
		case PM_NC:strTemp=g_strnetWork;break;
		case 17: strTemp=g_strTstat5f;break;
		case 18:strTemp=g_strTstat5g;break;
		case 16:strTemp=g_strTstat5e;break;
		case 19:strTemp=g_strTstat5h;break;
		case 6:strTemp=g_strTstat6;break;
		case 7:strTemp=g_strTstat7;break;
		case PM_TSTAT6_HUM_Chamber:strTemp=g_strHumChamber;break;
		default:strTemp=g_strTstat5a;break;
		}
		m_FlexGrid.put_TextMatrix(i+1,COL_DEVICETYPE ,strTemp);//product model ===== software version
		if(m_grid_load.at(i).flash_or_no==false)
			m_FlexGrid.put_TextMatrix(i+1,COL_ENABLE ,_T("false"));//flash select
		else
			m_FlexGrid.put_TextMatrix(i+1,COL_ENABLE ,_T("True"));//flash select
		m_FlexGrid.put_TextMatrix(i+1,COL_STATUS ,_T(""));

		if (m_grid_load.at(i).hardware_revisin<0)
		{
			m_FlexGrid.put_TextMatrix(i+1,COL_CONNECTION ,_T("Off line"));
			m_FlexGrid.put_TextMatrix(i+1,COL_ENABLE ,_T("False"));//flash select
		}
		else
		{
			m_FlexGrid.put_TextMatrix(i+1,COL_CONNECTION ,_T("On line"));
			m_FlexGrid.put_TextMatrix(i+1,COL_ENABLE ,_T("True"));//flash select
		}

		CString strSerial;
		strSerial.Format(_T("%d"),m_grid_load.at(i).serialnumber);
		CString strLogFile;
		strLogFile=m_strLogFilePath+_T("\\")+strSerial+_T(".txt");
		WIN32_FIND_DATA fData;
		HANDLE hFile=NULL;
		hFile = FindFirstFile(strLogFile,&fData);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			CFileStatus   st;
			CStdioFile fileLog(strLogFile,CFile::modeReadWrite | CFile::modeNoTruncate|CFile::typeText);
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
				if(strLastLine.Find(_T("Load Success"))>0)
				{
					SYSTEMTIME stUTC, stLocal;
					CString strDatatime;
					FileTimeToSystemTime(&(fData.ftLastWriteTime), &stUTC);
					SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
					strDatatime.Format(_T("%02d. %02d %d, %02d:%02d"), stLocal.wDay,stLocal.wMonth,stLocal.wYear,stLocal.wHour,stLocal.wMinute);
					m_FlexGrid.put_TextMatrix(i+1,COL_LOADEVENT,strDatatime);

				}
			}
			fileLog.Close();
		}
		FindClose(hFile);

	}

	m_pLoadBackCheckThread=AfxBeginThread(Check_LoadStatus,this);////////////////////////create thread,read information	
	m_pLoadBackCheckThread->m_bAutoDelete=false;

	
}


UINT _NC_LoadThread(LPVOID pParam)
{
	CGridLoad* pDlg = (CGridLoad*)(pParam);
		
	::WaitForSingleObject(pDlg->m_eTstatLoadFinish, INFINITE);

	int failure_number=0;
	for(UINT i =0; i < pDlg->m_grid_load.size(); i++)
	{
		GRID_LOAD glItem =pDlg->m_grid_load[i];
		
		CString log_file_path;
		CFile config_file;//the hex file
		CStdioFile log_file;
		bool log_file_opened=false;
		if(pDlg->m_bStopLoadingfile)
		{
			break;
		}
		if(!pDlg->m_grid_load.at(i).flash_or_no)
			continue;

		if(!pDlg->CheckLoadFileForNC(glItem))
		{
			continue;
		}



		int nID=-1;
		int nTempID =pDlg->m_grid_load.at(i).ID;//get temp tstat id
		if (g_CommunicationType==0)
		{
			int baudrate=19200;
			Change_BaudRate(19200);
			nID=read_one(nTempID,6);
			if(nID<=0)
			{
				baudrate=9600;
				Change_BaudRate(9600);
				nID=read_one(nTempID,6);
			}
		}
		if (g_CommunicationType==1)
		{
			nID=read_one(nTempID,6);
		}

		now_tstat_id=nID;
		if(now_tstat_id>0)
		{
			pDlg->m_FlexGrid.put_TextMatrix(i+1,COL_CONNECTION,_T("On line"));

		}
		else
		{
			pDlg->m_FlexGrid.put_TextMatrix(i+1,COL_CONNECTION,_T("Off line"));
			pDlg->m_FlexGrid.put_TextMatrix(i+1,COL_ENABLE,_T("False"));//flash select
			failure_number++;	
			pDlg->m_FlexGrid.put_TextMatrix(i+1,COL_STATUS,_T("Error"));
			continue;
		}

		bool open_file=false;
		CString strSerial;
		strSerial.Format(_T("%d"),pDlg->m_grid_load.at(i).serialnumber);
		strSerial+=_T(".txt");
		log_file_path=pDlg->m_strLogFilePath+_T("\\")+strSerial;

		WIN32_FIND_DATA fData;
		HANDLE hFile=NULL;
		hFile = FindFirstFile(log_file_path,&fData);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			CFile::Remove(log_file_path);
			pDlg->m_FlexGrid.put_TextMatrix(i+1,COL_LOADEVENT,_T(""));
		}
		FindClose(hFile);

		//added the header marker.
		if(log_file.Open(log_file_path,CFile::modeCreate | CFile::modeReadWrite)!=0)
		{
			log_file_opened=true;
			CString strHex=pDlg->m_grid_load.at(i).hex_file_path;
			if(!strHex.IsEmpty())
			{
				log_file.WriteString(strHex);
			}
			CString ttt=_T("\r\n\r\n");
			log_file.Write(ttt.GetString(),ttt.GetLength());
		}

		CString strConfigFile = pDlg->m_grid_load.at(i).hex_file_path.GetString();

		//if(config_file.Open(strConfigFile,CFile::modeRead | CFile::shareDenyNone))
		{
			//		write_one(now_tstat_id,324,0);	
// 			CString strPath=config_file.GetFilePath();
// 			CString strFileName=config_file.GetFileTitle();
// 			open_file=true;
			pDlg->m_infoListBox.InsertString(0,_T(""));

			load_file_2_schedule((LPTSTR)(LPCTSTR)strConfigFile, now_tstat_id, log_file);

		}


		{
			CString for_showing_text;
			for_showing_text.Format(_T("ID :%d Load Success"),now_tstat_id);
			log_file.WriteString(for_showing_text);
			change_showing_text_variable(for_showing_text);
			pDlg->m_FlexGrid.put_TextMatrix(i+1,COL_ENABLE,_T("False"));
			pDlg->m_FlexGrid.put_TextMatrix(i+1,COL_STATUS,_T("OK"));


			SYSTEMTIME stUTC, stLocal;
			CString strDatatime;
			FileTimeToSystemTime(&(fData.ftLastWriteTime), &stUTC);
			SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
			strDatatime.Format(_T("%02d. %02d %d, %02d:%02d"), stLocal.wDay,stLocal.wMonth,stLocal.wYear,stLocal.wHour,stLocal.wMinute);
			pDlg->m_FlexGrid.put_TextMatrix(i+1,COL_LOADEVENT,strDatatime);
		}
		//	else
		//failure_number++;
//		config_file.Close();//close file
 		if(log_file.m_hFile!=NULL)
 			log_file.Close();
	}		
	//p_list_box->ResetContent();

	pDlg->m_bNCLoadFinished=TRUE;

	return 1;
}


BOOL CGridLoad::CheckLoadFileForNC(GRID_LOAD& glItem)
{
	BOOL bRet = FALSE;
	if(glItem.device != PM_NC)
	{
		return FALSE;
	}
	CStdioFile file;
	if(file.Open(glItem.hex_file_path,CFile::modeRead | CFile::shareDenyNone))
	{
		CString strRead;
		file.ReadString(	strRead);
		file.ReadString(	strRead);
		strRead.Trim();
		strRead = strRead.Right(2);
		if(strRead.CompareNoCase(_T("NC")) == 0)
		{
			bRet = TRUE;
		}
	}

	file.Close();

	return bRet;			
}


BOOL CGridLoad::CheckLoadFileForTStat(GRID_LOAD& glItem)
{
	BOOL bRet = FALSE;
	if(glItem.device > PM_TSTAT5H)  // ����tstat
	{
		return FALSE;
	}
	CStdioFile file;
	if(file.Open(glItem.hex_file_path,CFile::modeRead | CFile::shareDenyNone))
	{
		CString strRead;
		file.ReadString(	strRead);
		strRead.Trim();
		if(strRead.Find(_T("Tstat")) != -1)
		{
			bRet = TRUE;
		}
	}

	file.Close();

	return bRet;			
}