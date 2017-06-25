
// T3000.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "T3000.h"
#include "MainFrm.h"

#include "T3000Doc.h"
#include "T3000View.h"
#include "DialogCM5_BacNet.h"

#include "LoginDlg.h"

#include "iniFile.h"
#include "afxinet.h"
#include "T3000DefaultView.h"
#include "bado/BADO.h"
#include "../SQLiteDriver/CppSQLite3.h"
#include "../MultipleMonthCal32/MultipleMonthCalCtrl.h"
 
const int g_versionNO=20170215;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "global_variable.h"
ULONG_PTR g_gdiplusToken;
BEGIN_MESSAGE_MAP(CT3000App, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CT3000App::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_SAVE_CONFIG, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_VERSIONHISTORY,OnVersionInfo)
END_MESSAGE_MAP()
	
// CT3000App construction 
CT3000App::CT3000App()
{
	m_bHiColorIcons = TRUE;
	CurrentT3000Version=_T("    2017.5.31 ");
	T3000_Version = 20531;
	m_lastinterface=19;
}
// The one and only CT3000App object
CT3000App theApp;

UINT UpdateT3000Background(LPVOID pParam)
{
  CT3000App* T3000App = (CT3000App*)pParam;
  TCHAR exeFullPath[MAX_PATH+1]; //
  GetModuleFileName(NULL, exeFullPath, MAX_PATH); //
  (_tcsrchr(exeFullPath, _T('\\')))[1] = 0;//
#if 0//判断网站是否有新版本，网络连接不是或者没有新版本，直接进入
  CString T3000updatepath;
  T3000updatepath=(CString)exeFullPath+_T("T3000Update.exe");

  if (T3000App->JudgeT3000Version())
  {
	  int ret = AfxMessageBox(_T("Find new version Do you want to update?"),MB_YESNOCANCEL ,3);
	  if ( ret == IDYES)
	  {

		  ShellExecute(NULL, _T("open"), T3000updatepath.GetBuffer(), NULL, NULL, SW_SHOWNORMAL);
		  //	WinExecAndWait(T3000updatepath.GetBuffer(),NULL,NULL,0);
	  }
	  //KillProcessFromName(_T("T3000.exe")) ;
	 /* T3000App->ExitInstance();*/
	  return FALSE;

  }
  return TRUE;
#endif
return TRUE;
}
BOOL CT3000App::user_login()
{
	BOOL bRet=FALSE;
	CLoginDlg dlg(bRet);
	dlg.DoModal();
	if(g_buser_log_in==false)
		return false;
	return true;
}
// CT3000App initialization
BOOL CT3000App::RegisterOcx(LPCTSTR   OcxFileName)
{
	LPCTSTR   pszDllName   =   OcxFileName   ;			//ActiveX控件的路径及文件名       
	HINSTANCE   hLib   =   LoadLibrary(pszDllName);    //装载ActiveX控件   
	if   (hLib   <   (HINSTANCE)HINSTANCE_ERROR)   
	{   
		return   FALSE;   
	}   
	FARPROC   lpDllEntryPoint;     
	lpDllEntryPoint   =   GetProcAddress(hLib,(LPCSTR)(_T("DllRegisterServer")));     //获取注册函数DllRegisterServer地址   
	if(lpDllEntryPoint!=NULL)														 //调用注册函数DllRegisterServer   
	{   
		if(FAILED((*lpDllEntryPoint)()))   
		{//   AfxMessageBox(_T("false"));
			FreeLibrary(hLib);   
			return   FALSE;   
			
		}   
		return   TRUE;   
	}   
	else   
	{
	//	AfxMessageBox(_T("false"));
			return   FALSE;


	}
	
}
 
BOOL CT3000App::JudgeT3000Version(){
CString ftp_T3000Version;
ftp_T3000Version=GetContentFromURL(_T("http://www.temcocontrols.com/ftp/software/T3000_Version.txt"));
if (ftp_T3000Version.GetLength()>20)
{
return FALSE;
}

// COleDateTime ftpVersionTime,CurrentVersionTime;
// ftpVersionTime.ParseDateTime(ftp_T3000Version);
// CurrentVersionTime.ParseDateTime(CurrentT3000Version);
if (ftp_T3000Version>CurrentT3000Version)
{
return TRUE;
}
return FALSE;

}

void CT3000App::UpdateDB()
{
    BOOL is_update = FALSE;
    CppSQLite3DB SqliteDBT3000;
    SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath); 
    CppSQLite3Query q;
    q = SqliteDBT3000.execQuery("Select * from Version");
    int version =0;
    while(!q.eof())
    {
        int tempversion = q.getIntField("VersionNO");
        if (tempversion !=0)
        {
            version = tempversion;
        }
        q.nextRow();
    }
    q.finalize();
    SqliteDBT3000.closedb();
    if (version <g_versionNO)
    {
        is_update = TRUE;
    }
    if (is_update)
    {
		CppSQLite3DB SqliteDBT3000;
		SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
		CppSQLite3Query q;
		q = SqliteDBT3000.execQuery("Select * from Building_ALL");
		Building_ALL stemp_building_all;
		while (!q.eof())
		{
			stemp_building_all.Building_Name = q.getValuebyName(L"Building_Name");
			stemp_building_all.Default_Building = q.getIntField("Default_Build");
			stemp_building_all.Address = q.getValuebyName(L"Address");
			stemp_building_all.Telephone = q.getValuebyName(L"Telephone");
			m_Building_ALL.push_back(stemp_building_all);
			q.nextRow();
		}
		q.finalize();  
		q = SqliteDBT3000.execQuery("Select * from Building");
		Building stemp_building;
		while (!q.eof())
		{
			    stemp_building.Main_BuildingName = q.getValuebyName(L"Main_BuildingName");
				stemp_building.Building_Name = q.getValuebyName(L"Building_Name");
				stemp_building.Protocal = q.getValuebyName(L"Protocal");
				stemp_building.Com_Port = q.getValuebyName(L"Com_Port");
				stemp_building.Ip_Address = q.getValuebyName(L"Ip_Address");
				stemp_building.Ip_Port = q.getValuebyName(L"Ip_Port");
				stemp_building.Braudrate = q.getValuebyName(L"Braudrate");
				stemp_building.Default_SubBuilding = q.getIntField("Default_SubBuilding");
				stemp_building.Building_Path = q.getValuebyName(L"Building_Path");
				stemp_building.Longitude = q.getValuebyName(L"Longitude");
				stemp_building.Latitude = q.getValuebyName(L"Latitude");
				stemp_building.Elevation = q.getValuebyName(L"Elevation");
				stemp_building.ID = q.getIntField("ID");
				stemp_building.country = q.getValuebyName(L"country");
				stemp_building.state = q.getValuebyName(L"state");
				stemp_building.city = q.getValuebyName(L"city");
				stemp_building.street = q.getValuebyName(L"street");
			    stemp_building.ZIP = q.getValuebyName(L"ZIP");
				stemp_building.EngineeringUnits = q.getValuebyName(L"EngineeringUnits");
				q.nextRow();
			m_Building.push_back(stemp_building);
		 
		}
		q.finalize();
		SqliteDBT3000.closedb();

        remove((UTF8MBSTR)g_strDatabasefilepath);//删掉原有的数据库

        CString FilePath;
        HANDLE hFind;
        WIN32_FIND_DATA wfd;
        hFind = FindFirstFile(g_strDatabasefilepath, &wfd);//
        if (hFind==INVALID_HANDLE_VALUE)//不存在该文件
        {
           
            FilePath=g_strExePth+_T("Database\\T3000.db");
            HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_T3000DB1), _T("T3000DB"));   
            HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);   
            LPVOID lpExe = LockResource(hGlobal);   
            CFile file;
            if(file.Open(FilePath, CFile::modeCreate | CFile::modeWrite))    
                file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));    
            file.Close();    
            ::UnlockResource(hGlobal);   
            ::FreeResource(hGlobal);
          
        }  

		SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
		CString StrSql;
		StrSql = L"Delete from Building_ALL";
		SqliteDBT3000.execDML((UTF8MBSTR)StrSql);
		for (int i = 0 ;i<m_Building_ALL.size();i++)
		{
			StrSql.Format(_T("Insert Into Building_ALL(Building_Name,Default_Build,Telephone,Address)  Values('%s',%d,'%s','%s')"),
				m_Building_ALL.at(i).Building_Name
				, m_Building_ALL.at(i).Default_Building
				, m_Building_ALL.at(i).Telephone
				, m_Building_ALL.at(i).Address);
			SqliteDBT3000.execDML((UTF8MBSTR)StrSql);
		}

		StrSql = L"Delete from Building";
		SqliteDBT3000.execDML((UTF8MBSTR)StrSql);

		for (int j=0;j<m_Building.size();j++)
		{
			StrSql.Format(_T("Insert Into Building(Main_BuildingName,Building_Name,Protocal,Com_Port,Ip_Address,Ip_Port,Braudrate,Default_SubBuilding,Building_Path,Longitude,Elevation,ID,country,state,city,street,ZIP,EngineeringUnits) Values('%s','%s','%s','%s','%s','%s','%s',%d,'%s','%s','%s',%d,'%s','%s','%s','%s','%s','%s')"),
				m_Building.at(j).Main_BuildingName,
				m_Building.at(j).Building_Name,
				m_Building.at(j).Protocal,
				m_Building.at(j).Com_Port,
				m_Building.at(j).Ip_Address,
				m_Building.at(j).Ip_Port,
				m_Building.at(j).Braudrate,
				m_Building.at(j).Default_SubBuilding,
				m_Building.at(j).Building_Path,
				m_Building.at(j).Longitude,
				m_Building.at(j).Elevation,
				m_Building.at(j).ID,
				m_Building.at(j).country,
				m_Building.at(j).state,
				m_Building.at(j).city,
				m_Building.at(j).street,
				m_Building.at(j).ZIP,
				m_Building.at(j).EngineeringUnits);
			SqliteDBT3000.execDML((UTF8MBSTR)StrSql);
		}
		SqliteDBT3000.closedb();
    }
}


/// <summary>
/// T3000 Start ,from here
/// </summary>
/// <returns></returns>
BOOL CT3000App::InitInstance()
{
	 
	GetModulePath();
	CString strSource = g_strExePth + L"T3000Controls.dll";
	//
	try
	{
		//if (ReadDLLRegAsm()<1)
		{
			CopyFile(strSource, L"C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\T3000Controls.dll", FALSE);
			// ::ShellExecute(NULL, _T("open"), _T("C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\RegAsm.exe T3000Controls.dll"), _T(""), _T(""), SW_SHOW); 
			ShellExecute(NULL, _T("open"), _T("C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\RegAsm.exe"), L"C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\T3000Controls.dll", NULL, SW_HIDE);
		/*	SetDLLRegAsm(1);*/
// 			AfxMessageBox(L"Restart T3000,Please");
// 			return FALSE;
		}
		
		// InitCommonControlsEx() is required on Windows XP if an application
		// manifest specifies use of ComCtl32.dll version 6 or later to enable
		// visual styles.  Otherwise, any window creation will fail.

		INITCOMMONCONTROLSEX InitCtrls;
		InitCtrls.dwSize = sizeof(InitCtrls);

		InitCtrls.dwICC = ICC_WIN95_CLASSES;
		InitCommonControlsEx(&InitCtrls);

		Create_T3000_log_file();
		CMultipleMonthCalCtrl::RegisterControl();

		BOOL First_Start=TRUE;


		//CString CO2Timer;
		//CO2Timer=GetProductFirmwareTimeFromTemcoWebsite(_T("http://temcocontrols.com/ftp/firmware/MINIPANEL/"),_T("minipanel.bin"));
		//AfxMessageBox(CO2Timer);
		//  	CString ftp_T3000Version;
		//  	ftp_T3000Version=GetContentFromURL(_T("http://temcocontrols.com/ftp/firmware/CO2/CO2-W/"));
		//  	//AfxBeginThread(UpdateT3000Background,this);
		//  	CStringArray HtmlArray;
		//  	HtmlArray.RemoveAll();
		//  	SplitCStringA(HtmlArray,ftp_T3000Version,_T("<hr>"));
		//  	CString ImageString;
		//  	if (HtmlArray.GetSize()>1)
		//  	{
		//  	   ImageString=HtmlArray[1];
		//  	   HtmlArray.RemoveAll();
		//  	   SplitCStringA(HtmlArray,ImageString,_T("<img"));
		// 	   ImageString.Empty();
		//  	   for (int i=0;i<HtmlArray.GetSize();i++)
		//  	   {
		//  		   if (HtmlArray[i].Find(_T("CO2-W.hex"))!=-1)
		//  		   {
		//  			   ImageString=HtmlArray[i];
		//  			   break;
		//  		   }
		//  	   }
		// 	   CString VersionString;
		// 	   HtmlArray.RemoveAll();
		// 	   
		// 	   if(ImageString.GetLength()>1)
		// 	   {
		// 	      SplitCStringA(HtmlArray,ImageString,_T("</a>"));
		// 	   }
		// 	   if(HtmlArray.GetSize()>0)
		// 	   {
		// 	     ImageString=HtmlArray[1];
		// 	   }
		// 	   //               20-Nov-2014 00:36  152K
		// 	   ImageString.TrimLeft();
		// 	   ImageString.TrimRight();
		// 	   HtmlArray.RemoveAll();
		// 	   if(ImageString.GetLength()>1)
		// 	   {
		// 		   SplitCStringA(HtmlArray,ImageString,_T("  "));
		// 	   }
		// 	   if (HtmlArray.GetSize()>0)
		// 	   {
		// 	      VersionString=HtmlArray[0];
		// 	   }
		// 
		//  	 
		//  	}
		// 	
		//	versionstring=ImageString.g(index_start);
	//	Logger::WriteMessage("T3000 Unit Tester Begin....");
		CWinAppEx::InitInstance();
		HRESULT hr;
		//


		if(!AfxInitRichEdit())
		{
			AfxMessageBox(IDS_INIT_RICHEDIT_ERROR);//
			return FALSE;//
		}
		//AfxInitRichEdit2();
		if(!AfxInitRichEdit2())
		{
			AfxMessageBox(IDS_INIT_RICHEDIT_ERROR);//
			return FALSE;//
		}
		if (!AfxSocketInit())//
		{
			AfxMessageBox(IDP_SOCKETS_INIT_FAILED);//
			return FALSE;//
		}
		// Initialize OLE libraries
		if (!AfxOleInit())
		{
			AfxMessageBox(IDP_OLE_INIT_FAILED);
			return FALSE;
		}
		AfxEnableControlContainer();
		CString T3000_Rev;
		//	T3000_Rev=getURLContext(_T("www.temcocontrols.com/ftp/software/T3000_Version.txt"));

		CString ProgramName;
		ProgramName=  _T("T3000AppRunOnce");
		HANDLE hMutex=CreateMutex(NULL,TRUE,ProgramName); 
		if(GetLastError()==ERROR_ALREADY_EXISTS)
		{
			//SetForegroundWindow(FindWindow(NULL,ProgramName));
			exit(1);
			return true;
		}


#if 1	
		try
		{

			TCHAR exeFullPath[MAX_PATH+1]; //
			GetModuleFileName(NULL, exeFullPath, MAX_PATH); //
			(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;//
			g_strDatabasefilepath=exeFullPath;//
			g_strExePth=g_strDatabasefilepath;//




			CreateDirectory(g_strExePth+_T("Database"),NULL);//creat database folder;//

			g_achive_device_name_path = g_strDatabasefilepath + _T("Database") + _T("\\") + _T("temp\\") + _T("device_name.ini") ;
			g_achive_folder = g_strDatabasefilepath + _T("Database") + _T("\\") + _T("temp");
			g_achive_folder_temp_txt = g_achive_folder + _T("\\") + _T("prg_txt_file");
			g_achive_folder_temp_db = g_achive_folder + _T("\\") + _T("MonitorDatabaseFolder");
			g_cstring_ini_path = g_achive_folder + _T("\\MonitorIndex.ini");
			product_sort_way  = GetPrivateProfileInt(_T("Setting"),_T("ProductSort"),0,g_cstring_ini_path);
			if(product_sort_way == 0)
			{
				WritePrivateProfileStringW(_T("Setting"),_T("ProductSort"),_T("1"),g_cstring_ini_path);
				product_sort_way = SORT_BY_CONNECTION;
			}

			CString temp_cs_version;
			temp_cs_version.Format(_T("%d"),T3000_Version);

			WritePrivateProfileStringW(_T("Version"),_T("T3000"),temp_cs_version,g_cstring_ini_path);


			monitor_ignore_enable = GetPrivateProfileInt(_T("Setting"),_T("EnableMonitorValueIgnore"),0,g_cstring_ini_path);
			if(monitor_ignore_enable == 0)
			{
				WritePrivateProfileString(_T("Setting"),_T("EnableMonitorValueIgnore"),_T("0"),g_cstring_ini_path);
				WritePrivateProfileString(_T("Setting"),_T("MonitorValueIgnoreMax"),_T("10000000"),g_cstring_ini_path);
				WritePrivateProfileString(_T("Setting"),_T("MonitorValueIgnoreMin"),_T("-100000"),g_cstring_ini_path);
			}
			else if(monitor_ignore_enable == 1)
			{
				monitor_ignore_max_value = GetPrivateProfileInt(_T("Setting"),_T("MonitorValueIgnoreMax"),1000000,g_cstring_ini_path);
				monitor_ignore_min_value = GetPrivateProfileInt(_T("Setting"),_T("MonitorValueIgnoreMin"),-100000,g_cstring_ini_path);
			}




			g_achive_monitor_datatbase_path = g_achive_folder;
			g_achive_monitor_datatbase_path = g_achive_monitor_datatbase_path + _T("\\MonitorData.mdb");

			WIN32_FIND_DATA fd;
			BOOL ret = FALSE;
			HANDLE hFind_folder = FindFirstFile(g_achive_folder, &fd);
			if ((hFind_folder != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//目录存在
				ret = TRUE;

			}
			FindClose(hFind_folder);
			if(ret == false)
			{
				SECURITY_ATTRIBUTES attrib;
				attrib.bInheritHandle = FALSE;
				attrib.lpSecurityDescriptor = NULL;
				attrib.nLength = sizeof(SECURITY_ATTRIBUTES);

				CreateDirectory( g_achive_folder, &attrib);
			}

			ret = FALSE;
			hFind_folder = FindFirstFile(g_achive_folder_temp_txt, &fd);
			if ((hFind_folder != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//目录存在
				ret = TRUE;

			}
			FindClose(hFind_folder);
			if(ret == false)
			{
				SECURITY_ATTRIBUTES attrib;
				attrib.bInheritHandle = FALSE;
				attrib.lpSecurityDescriptor = NULL;
				attrib.nLength = sizeof(SECURITY_ATTRIBUTES);

				CreateDirectory( g_achive_folder_temp_txt, &attrib);
			}


			ret = FALSE;
			hFind_folder = FindFirstFile(g_achive_folder_temp_db, &fd);
			if ((hFind_folder != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//目录存在
				ret = TRUE;

			}
			FindClose(hFind_folder);
			if(ret == false)
			{
				SECURITY_ATTRIBUTES attrib;
				attrib.bInheritHandle = FALSE;
				attrib.lpSecurityDescriptor = NULL;
				attrib.nLength = sizeof(SECURITY_ATTRIBUTES);

				CreateDirectory( g_achive_folder_temp_db, &attrib);
			}

			HANDLE hFind;
			WIN32_FIND_DATA wfd;

// 			CString SqliteDLLPath=GetExePath(true)+L"sqlite3.dll";
// 			hFind = FindFirstFile(SqliteDLLPath, &wfd);
// 			if (hFind==INVALID_HANDLE_VALUE)
// 			{
// 
// 				HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_SQLITE3DLL1), _T("SQLITE3DLL"));   
// 				HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);   
// 				LPVOID lpExe = LockResource(hGlobal);   
// 				CFile file;
// 				if(file.Open(SqliteDLLPath, CFile::modeCreate | CFile::modeWrite))    
// 					file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));    
// 				file.Close();    
// 				::UnlockResource(hGlobal);   
// 				::FreeResource(hGlobal);
// 			}
			//IDR_MFC16DLL1

			CString MFC16DLLPath=GetExePath(true)+L"MFC16API.dll";
			hFind = FindFirstFile(MFC16DLLPath, &wfd);
			if (hFind==INVALID_HANDLE_VALUE)
			{

				HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MFC16DLL1), _T("MFC16DLL"));   
				HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);   
				LPVOID lpExe = LockResource(hGlobal);   
				CFile file;
				if(file.Open(MFC16DLLPath, CFile::modeCreate | CFile::modeWrite))    
					file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));    
				file.Close();    
				::UnlockResource(hGlobal);   
				::FreeResource(hGlobal);
			}


		 
			 
			


		 
			g_strDatabasefilepath+=_T("Database\\T3000.db");//



#if 1//如果沒有T3000 的情況下

			CString FilePath;
 
			hFind = FindFirstFile(g_strDatabasefilepath, &wfd);//
			if (hFind==INVALID_HANDLE_VALUE)//说明当前目录下无t3000.mdb
			{
				
				//没有找到就创建一个默认的数据库
				FilePath=g_strExePth+_T("Database\\T3000.db");
				HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_T3000DB1), _T("T3000DB"));   
				HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);   
				LPVOID lpExe = LockResource(hGlobal);   
				CFile file;
				if(file.Open(FilePath, CFile::modeCreate | CFile::modeWrite))    
					file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));    
				file.Close();    
				::UnlockResource(hGlobal);   
				::FreeResource(hGlobal);
				First_Start=TRUE;
			}  //
			else
			{
				First_Start=FALSE;
			}
			FindClose(hFind);//



			CString FilePath_Monitor;
			HANDLE hFind_Monitor;//
			WIN32_FIND_DATA wfd_monitor;//
			hFind_Monitor = FindFirstFile(g_achive_monitor_datatbase_path, &wfd_monitor);//
			if (hFind_Monitor==INVALID_HANDLE_VALUE)//说明当前目录下无MonitorData.db
			{
				//没有找到就创建一个默认的数据库
				FilePath_Monitor= g_achive_monitor_datatbase_path;
				HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MONITOR_DB2), _T("MONITOR_DB"));   
				HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);   
				LPVOID lpExe = LockResource(hGlobal);   
				CFile file;
				if(file.Open(FilePath_Monitor, CFile::modeCreate | CFile::modeWrite))    
					file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));    
				file.Close();    
				::UnlockResource(hGlobal);   
				::FreeResource(hGlobal);
			}  //
			FindClose(hFind_Monitor);//

#if 0
			int versionno_monitor = 0;
			//CADO tempado;
			//BOOL Ret=tempado.OnInitADOConn();
			CBADO tempado;
			tempado.SetDBPath(g_achive_monitor_datatbase_path);	//暂时不创建新数据库
			tempado.OnInitADOConn(); 


			if (tempado.IsHaveTable(tempado,_T("Version")))//有Version表
			{
				CString sql=_T("Select * from Version");
				tempado.m_pRecordset=tempado.OpenRecordset(sql);
				tempado.m_pRecordset->MoveFirst();
				while (!tempado.m_pRecordset->EndOfFile)
				{
					versionno_monitor=tempado.m_pRecordset->GetCollect(_T("VersionNO"));
					tempado.m_pRecordset->MoveNext();
				}
				tempado.CloseRecordset();


			} 

			if (versionno_monitor < 20150922)
			{
				CString StrSql;
				StrSql=_T("ALTER TABLE MonitorData ADD COLUMN Temp_Data Integer");
				tempado.m_pConnection->Execute(StrSql.GetBuffer(),NULL,adCmdText);
				StrSql=_T("ALTER TABLE MonitorData ADD COLUMN DisplayTime varchar(255)");
				tempado.m_pConnection->Execute(StrSql.GetBuffer(),NULL,adCmdText);


				StrSql=_T("INSERT INTO Version VALUES(20150922,'Add Temp and Time')");
				tempado.m_pConnection->Execute(StrSql.GetBuffer(),NULL,adCmdText);
			}
			tempado.CloseConn(); 
#endif



		//	g_strDatabasefilepath=(CString)FOR_DATABASE_CONNECT+g_strDatabasefilepath;//
			g_strImgeFolder=g_strExePth+_T("Database\\image\\");//
			CreateDirectory(g_strImgeFolder,NULL);//
			g_strBuildingFolder=g_strExePth+_T("Database\\Buildings\\");
			CreateDirectory(g_strBuildingFolder,NULL);//


			if (First_Start)
			{
				//创建Default_Building
				CString filebuildingPath;//=g_strBuildingFolder+m_Building.at(i).Main_BuildingName+_T("\\"); 
				filebuildingPath.Format(_T("%sDefault_Building\\"),g_strBuildingFolder);
				CreateDirectory(g_strBuildingFolder,NULL);

				CreateDirectory(filebuildingPath,NULL);

				filebuildingPath+=(CString)"Default_Building";//_T(".mdb");
				filebuildingPath+=_T(".db");

				DeleteFile(filebuildingPath);
				// 	 HANDLE hFind;//
				// 	 WIN32_FIND_DATA wfd;//
				//create building db file

				hFind = FindFirstFile(filebuildingPath, &wfd);//
				if (hFind==INVALID_HANDLE_VALUE)//说明当前目录下无t3000.mdb
				{

					HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_BUILDINGDB1), _T("BUILDINGDB"));   
					HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);   


					LPVOID lpExe = LockResource(hGlobal);   
					CFile file;
					if(file.Open(filebuildingPath, CFile::modeCreate | CFile::modeWrite))    
						file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));    
					file.Close();    
					::UnlockResource(hGlobal);   
					::FreeResource(hGlobal);
				}//
				FindClose(hFind);
				int index= filebuildingPath.Find(_T("Database"));
				filebuildingPath.Delete(0,index);

				CString sql;
				/*CADO ado; 
				ado.OnInitADOConn();

				sql.Format(_T("update Building set Building_Path = '%s'  where  Main_BuildingName = 'Default_Building' "),filebuildingPath);

				ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);

				ado.CloseConn();*/


			}



#endif


			//HKEY hkey;
			//char sz[256];
			//DWORD dwtype, sl = 256;

			//RegOpenKeyEx(HKEY_LOCAL_MACHINE,	_T("SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\StandardProfile"),	NULL, KEY_READ, &hkey);
			//RegQueryValueEx(hkey, _T("EnableFirewall"), NULL, &dwtype, (LPBYTE)sz, &sl);
			//DWORD dw_firewall;
			//dw_firewall = sz[0] + sz[1] * 256 + sz[2] * 256*256 + sz[3] * 256*256*256;
			//if(dw_firewall != 0 )
			//{
			//	AfxMessageBox(_T("Please turn off your firewall .If not , some broadcast communication may fail."));
			//}
			//
			//RegCloseKey(hkey);





			//先不考虑升级的情况
#if 0
		 	int Ret=JudgeDB();
			if (!Ret)
			{
				FilePath=g_strExePth+_T("Database\\T3000.db");
				DeleteFile(FilePath.GetBuffer());
				HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_T3000DB1), _T("T3000DB"));   
				HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);   


				LPVOID lpExe = LockResource(hGlobal);   
				CFile file;
				if(file.Open(FilePath, CFile::modeCreate | CFile::modeWrite))    
					file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));    
				file.Close();    
				::UnlockResource(hGlobal);   
				::FreeResource(hGlobal);
				//JudgeDB();

			}

#endif
              UpdateDB();

			//Sleep(1000);


			InitModeName();//




#endif

			// Standard initialization
			// If you are not using these features and wish to reduce the size
			// of your final executable, you should remove from the following
			// the specific initialization routines you do not need
			// Change the registry key under which our settings are stored
			//  You should modify this string to be something appropriate
			// such as the name of your company or organization
			SetRegistryKey(_T("Temco T3000 Application"));//
			LoadStdProfileSettings();  // Load standard INI file options (including MRU)//
			InitContextMenuManager();
			InitKeyboardManager();

			InitTooltipManager();
			CMFCToolTipInfo ttParams;
			ttParams.m_bVislManagerTheme = TRUE;
			theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
				RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams); 
#if 1
			hr=CoInitialize(NULL);//
			if(FAILED(hr)) 	//
			{
				AfxMessageBox(_T("Error Initialize the COM Interface"));//
				return FALSE;//
			} 

#endif


		}
		catch (...)
		{
			CFileFind temp_find;
			CString temp_123;
			temp_123 = g_strExePth +  _T("Database\\T3000.db");
			BOOL	nret = temp_find.FindFile(temp_123);
			if(nret)
			{
				DeleteFile(temp_123);
			}
			AfxMessageBox(_T("Update database!Please restart again."));
			exit(0);
		}

		CSingleDocTemplate* pDocTemplate;
		pDocTemplate = new CSingleDocTemplate(
			IDR_MAINFRAME,
			RUNTIME_CLASS(CT3000Doc),
			RUNTIME_CLASS(CMainFrame),       // main SDI frame window
			RUNTIME_CLASS(CT3000View));


		if (!pDocTemplate)
			return FALSE;

		AddDocTemplate(pDocTemplate);
		// Parse command line for standard shell commands, DDE, file open
		CCommandLineInfo cmdInfo;	
		ParseCommandLine(cmdInfo);

		//cmdInfo.m_nShellCommand   =   CCommandLineInfo::FileNothing; //lsc


		// Dispatch commands specified on the command line.  Will return FALSE if
		// app was launched with /RegServer, /Register, /Unregserver or /Unregister.

		if (!ProcessShellCommand(cmdInfo))
			return FALSE;


		GdiplusStartupInput gdiplusStartupInput;//
		GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);//

#if 1
 



		//if (g_bPrivilegeMannage)
		//{//for just quick debug,only on this computer
		//	if(!user_login())
		//	{
		//		AfxMessageBox(_T("Error password!"));	
		//		return false;
		//	}

		//}

#endif

		 ((CMainFrame*)m_pMainWnd)->InitViews();//
		CString strTile;
		strTile.Format(_T("T3000 Building Automation System"));
		strTile+=CurrentT3000Version;
		m_pMainWnd->SetWindowText(strTile);//
		m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
		m_pMainWnd->ShowWindow(SW_SHOW);
		m_pMainWnd->UpdateWindow();
	   ((CMainFrame*)m_pMainWnd)->SwitchToPruductType(DLG_DIALOG_DEFAULT_BUILDING); 

       m_szAppPath  = g_strExePth;
       m_szHelpFile = theApp.m_szAppPath + L"T3000_Help.chm";
// 		CString g_configfile_path =g_strExePth + g_strStartInterface_config;
// 		m_lastinterface= 19 ;//GetPrivateProfileInt(_T("T3000_START"),_T("Interface"),19,g_configfile_path); //由 杜帆 16-03-17屏蔽；进入不同的界面引起 T3000打开时报错。
// 		g_selected_serialnumber=GetPrivateProfileInt(_T("T3000_START"),_T("SerialNumber"),0,g_configfile_path);
// 		if (m_lastinterface!=19&&m_lastinterface!=24)
// 		{
// 			((CMainFrame*)m_pMainWnd)->SwitchToPruductType(m_lastinterface);
// 		}
// 		else
// 		{
// 			((CMainFrame*)m_pMainWnd)->SwitchToPruductType(19);
// 		}


	}
	catch (...)
	{
        CString FilePath;
        HANDLE hFind;
        WIN32_FIND_DATA wfd;

        remove((UTF8MBSTR)g_strDatabasefilepath);
     //g_strDatabasefilepath   =g_strExePth+_T("Database\\T3000.db");
        HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_T3000DB1), _T("T3000DB"));   
        HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);   
        LPVOID lpExe = LockResource(hGlobal);   
        CFile file;
        if(file.Open(FilePath, CFile::modeCreate | CFile::modeWrite))    
            file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));    
        file.Close();    
        ::UnlockResource(hGlobal);   
        ::FreeResource(hGlobal);
		CString str_msado;
		str_msado.Format(_T("%sREG_MSFLXGRD.bat"),g_strExePth.GetBuffer());
		::ShellExecute(NULL, _T("open"),str_msado.GetBuffer(), _T(""), _T(""), SW_SHOW);
		//vcredist_x86.zip

		//	::ShellExecute(NULL, _T("open"), _T("C:\\Program Files\\Temcocontrols\\T3000\\vcredist_x86.zip"), _T(""), _T(""), SW_SHOW);
		//这个要先试试，当电脑没有安装这个文件时，如何捕获这个信息，然后再执行这个。


		AfxMessageBox(_T("Database error! Please restart again ."));
		//::ShellExecute(NULL, _T("open"), _T("www.temcocontrols.com/ftp/software/AccessDatabaseEngine.zip"), _T(""), _T(""), SW_SHOW);
		//AfxMessageBox(_T("Open'T3000'Again,Please!"));


		return TRUE;

	}

	//Create_T3000_log_file();
	return TRUE;
}
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CAboutDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// App command to run the dialog
void CT3000App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CT3000App customization load/save methods

void CT3000App::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CT3000App::LoadCustomState()
{
}

void CT3000App::SaveCustomState()
{
}
void CT3000App::InitModeName()
{
	CString strInfoFile=g_strExePth+_T("CustomInfo.ini");
	HANDLE hFile;
	WIN32_FIND_DATA fData;
	hFile = FindFirstFile(strInfoFile,&fData);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		//m_strCompany=_T("Temco Controls LTD.");
		//m_strWebLinker=_T("http://www.temcocontrols.com");
		g_strTstat5a=_T("Tstat5A");
		g_strTstat5b=_T("Tstat5B");
		g_strTstat5b2=_T("Tstat5B2");
		g_strTstat5c=_T("Tstat5C");
		g_strTstat5d=_T("Tstat5D");
		g_strnetWork=_T("NC");
		g_strTstat5f=_T("Tstat5F");
		g_strTstat5g=_T("Tstat5G");
		g_strTstat5e=_T("Tstat5E");
		g_strTstat5h=_T("Tstat5H");
		g_strTstat6=_T("Tstat6");
		g_strTstat7=_T("Tstat7");
		g_strPressure=_T("Pressure");
		g_strOR485=_T("OR485");
		g_strHumChamber=_T("HumChamber");
	}
	else
	{
		CIniFile file(strInfoFile);
		g_strTstat5a = file.GetProfileString(_T("Section 3"),_T("Product1A"),_T("Tstat5A"));
		g_strTstat5b = file.GetProfileString(_T("Section 4"),_T("Product1B"),_T("Tstat5B"));
		g_strTstat5b2 = file.GetProfileString(_T("Section 5"),_T("Product1B2"),_T("Tstat5B2"));
		g_strTstat5c=file.GetProfileString(_T("Section 6"),_T("Product1C"),_T("Tstat5C"));
		g_strTstat5d=file.GetProfileString(_T("Section 7"),_T("Product1D"),_T("Tstat5D"));
		g_strnetWork=file.GetProfileString(_T("Section 8"),_T("NetworkController"),_T("NC"));		
		g_strTstat5f=file.GetProfileString(_T("Section 9"),_T("Product1F"),_T("Tstat5F"));
		g_strTstat5g=file.GetProfileString(_T("Section 10"),_T("Product1G"),_T("Tstat5G"));
		g_strTstat5e=file.GetProfileString(_T("Section 11"),_T("Product1E"),_T("Tstat5E"));
		g_strTstat5h=file.GetProfileString(_T("Section 12"),_T("Product1H"),_T("Tstat5H"));

// 		g_strTstat6=file.GetProfileString(_T("Section 13"),_T("Product6"),_T("Tstat6"));
// 		g_strTstat7=file.GetProfileString(_T("Section 14"),_T("Product7"),_T("Tstat7"));
	}
	FindClose(hFile);
}
// CT3000App message handlers
void CAboutDlg::OnBnClickedButton1()
{
	
	CString m_strWebLinker;
	m_strWebLinker.Format(_T("mailto:alex@temcocontrols.com?subject=feedback to temco &body=please add the attachment in the \n%sT3000.log "),g_strExePth);
	try{
          ShellExecute(GetSafeHwnd(), NULL,m_strWebLinker,   NULL, NULL,   SW_SHOWNORMAL);
	}
	catch(...)
	{
		AfxMessageBox(_T("Error:Can't find the email client in your pc!"));
	}
	
}
int CT3000App::ExitInstance()
{
	
	CoUninitialize( );
	return CWinAppEx::ExitInstance();
}
void CT3000App::CopyDirectory(CString strSrcPath,CString strDstPath)
{
	SHFILEOPSTRUCT FileOp;
	memset(&FileOp,0,sizeof(FileOp));
	FileOp.hwnd=NULL;   
	FileOp.wFunc=FO_COPY;   
	FileOp.pFrom=strSrcPath;   
	FileOp.pTo=strDstPath;
	FileOp.fFlags=FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_NOERRORUI|FOF_SILENT;
	FileOp.hNameMappings=NULL;
	FileOp.lpszProgressTitle=NULL;
	if(0==SHFileOperation(&FileOp))
		AfxMessageBox(_T("Success"));
	else
		AfxMessageBox(_T("Failed"));
  }
void CT3000App::OnVersionInfo()
{
	CString strHistotyFile=g_strExePth+_T("history.txt");
	ShellExecute(NULL, _T("open"), strHistotyFile, NULL, NULL, SW_SHOWNORMAL);
}

BOOL CT3000App::haveRegister()
{
	if (m_maxClients == 0)
		return FALSE;
	else 
		return TRUE;
}

void CT3000App::GetModulePath()
{
 

	TCHAR exeFullPath[MAX_PATH + 1]; //
	GetModuleFileName(NULL, exeFullPath, MAX_PATH); //
	(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;//
	g_strDatabasefilepath = exeFullPath;//
	g_strExePth = g_strDatabasefilepath;//

	
}

int CT3000App::GetSoftInstallDays()
{
	CFileStatus fileStatus;
	CTime curTime;
	CString strPath;
	CTimeSpan timeSpan;

	curTime=CTime::GetCurrentTime();
	//strPath=GetModulePath();

	strPath=strPath.Left(strPath.ReverseFind(_T('\\'))+1);
	strPath+=_T("T3000_Rev2.5.0.99.exe");

//	strPath+=_T("T3000.exe");
	CFile::GetStatus(strPath,fileStatus);

	//timeSpan=curTime-fileStatus.m_ctime;
	timeSpan=curTime-fileStatus.m_mtime;//这个是修改时间，不论你CUT，COPY都不会修改的

	return (int)timeSpan.GetDays();
}

void CT3000App::WriteNumber( CRegKey& key,CStringW valueName,DWORD value )
{
	key.SetDWORDValue(valueName,value);
}

BOOL CT3000App::ReadNameber( CRegKey& key,CStringW valueName,DWORD& value )
{
	DWORD s;
	if (key.QueryDWORDValue(valueName,s) == ERROR_SUCCESS)
	{
		value = s;
		return TRUE;
	}else
	{
		return FALSE;
	}
}

void CT3000App::Judgestore()
{
	HKEY hKEY;
	LPCTSTR data_Set = _T("Software\\Microsoft\\Windows NT\\CurrentVersion\\");//_T("Software\\Microsoft\\");//
	long ret0 = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set,0,KEY_READ,&hKEY);
	if (ret0!=ERROR_SUCCESS)
	{
		return;
	}

	LPBYTE  owner_Get = new BYTE[80];
	DWORD type_1 = REG_SZ;
	DWORD cbData_1 = 80;

	long ret1 = ::RegQueryValueEx(hKEY,_T("maxClients"),NULL,&type_1,owner_Get,&cbData_1);
	if (ret1!=ERROR_SUCCESS)
	{
		CRegKey key;
		key.Create(HKEY_LOCAL_MACHINE,data_Set);
		WriteNumber(key,_T("maxClients"),4315);

	}
	long ret2 = ::RegQueryValueEx(hKEY,_T("Regvalue"),NULL,&type_1,owner_Get,&cbData_1);
	if (ret2!=ERROR_SUCCESS)
	{
		CRegKey key;
		key.Create(HKEY_LOCAL_MACHINE,data_Set);
		WriteNumber(key,_T("Regvalue"),100);
	}
	delete[] owner_Get;
	::RegCloseKey(hKEY);
}

void CT3000App::ReadREG()
{
	CRegKey key;
	LPCTSTR data_Set = _T("Software\\Microsoft\\Windows NT\\CurrentVersion");//_T("Software\\Microsoft\\");//
	key.Open(HKEY_LOCAL_MACHINE,data_Set);
	ReadNameber(key,_T("maxClients"),m_maxClients);
	ReadNameber(key,_T("Regvalue"),password);
	if (password != 100)
	{
		WriteNumber(key,_T("Regvalue"),100);
		password =100;
	}
	if (m_maxClients !=4315&&m_maxClients!=0)
	{
		WriteNumber(key,_T("maxClients"),4315);
		m_maxClients = 15;

	}else if (m_maxClients == 0)
	{
		m_maxClients = 0;
	}else 
	{
		m_maxClients = m_maxClients - 4300;
		if (m_maxClients>password)
		{
			m_maxClients = 0;
		}
	}
}

int CT3000App::GetLanguage(){
	CRegKey key;
	LPCTSTR data_Set = _T("Software\\Microsoft\\Windows NT\\CurrentVersion");//_T("Software\\Microsoft\\");//
	key.Open(HKEY_LOCAL_MACHINE,data_Set);
	DWORD language;
	ReadNameber(key,_T("T3000"),language);
	return language;

}
int CT3000App::ReadDLLRegAsm() {
	CRegKey key;
	LPCTSTR data_Set = _T("Software\\Microsoft\\Windows NT\\CurrentVersion");//_T("Software\\Microsoft\\");//
	key.Open(HKEY_LOCAL_MACHINE, data_Set);
	DWORD language;
	ReadNameber(key, _T("T3000"), language);
	return language;

}

void CT3000App::SetLanguage(DWORD Last){
	CRegKey key;
	LPCTSTR data_Set = _T("Software\\Microsoft\\Windows NT\\CurrentVersion");//_T("Software\\Microsoft\\");//
	key.Open(HKEY_LOCAL_MACHINE,data_Set);
	WriteNumber(key,_T("T3000"),Last);
}
void CT3000App::SetDLLRegAsm(DWORD Last) {
	CRegKey key;
	LPCTSTR data_Set = _T("Software\\Microsoft\\Windows NT\\CurrentVersion");//_T("Software\\Microsoft\\");//
	key.Open(HKEY_LOCAL_MACHINE, data_Set);
	WriteNumber(key, _T("T3000"), Last);
}
#include "Dowmloadfile.h"
extern tree_product	m_product_isp_auto_flash;
bool update_t3000_only = false;
void CAboutDlg::OnBnClickedOk()
{

	update_t3000_only = true;
	m_product_isp_auto_flash.product_class_id =  199;
	Dowmloadfile Dlg;
	Dlg.DoModal();
	update_t3000_only = false;
CDialog::OnOK();
}


BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	CString temp_version;
	temp_version = _T("T3000 Building Automation System") + CurrentT3000Version;
	
	GetDlgItem(IDC_STATIC_ABOUT_INFO)->SetWindowText(temp_version);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
