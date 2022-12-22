
// T3000.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "T3000.h"
#include "T3000DefaultView.h"
#include "T3000Doc.h"
#include "T3000View.h"

#include "LoginDlg.h"
#include "iniFile.h"

#include "../SQLiteDriver/CppSQLite3.h"
#include "../MultipleMonthCal32/MultipleMonthCalCtrl.h"
 
#include <windows.h>  
 

const unsigned int g_versionNO= 20221111;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "global_variable.h"

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

    char strASCIICompileTime[128] = { 0 };
    sprintf(strASCIICompileTime, "    %s ", __DATE__);

    //_CrtSetBreakAlloc(261); //257
    MultiByteToWideChar(CP_ACP, 0, (char *)strASCIICompileTime, (int)strlen(strASCIICompileTime) + 1, CurrentT3000Version.GetBuffer(MAX_PATH), MAX_PATH);
    CurrentT3000Version.ReleaseBuffer();

    //******************************************************
    // Release �汾����ʱ���δ˶Σ��˶� ��Ҫ���ڵ���ʱ ��ʾ ������ �����ӵİ汾.
//#ifdef _DEBUG
    char strTime[128] = { 0 }; // ȡСʱ�� С�汾��;
    CString Test_Version;  //   TIME ��DATE    
    memcpy(strTime, __TIME__, 2);
    MultiByteToWideChar(CP_ACP, 0, (char *)strTime, (int)strlen(strTime) + 1, Test_Version.GetBuffer(MAX_PATH), MAX_PATH);
    Test_Version.ReleaseBuffer();
	CurrentT3000Version= CurrentT3000Version  + Test_Version; //�ŷ� : Release �淢����ʱ�� ������ε��ͺ��� �����Զ���ȡ���������.
//#endif 
    //*******************************************************
    
	T3000_Version = g_versionNO; //

	m_lastinterface=19;
}

// The one and only CT3000App object
CT3000App theApp;

void CT3000App::UpdateDB()
{
    CppSQLite3DB SqliteDBT3000;
    SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath); 
    CppSQLite3Query q = SqliteDBT3000.execQuery("SELECT * from Version ORDER BY VersionNO DESC");
    int version = q.getIntField("VersionNO");
    q.finalize();
    SqliteDBT3000.closedb();

	BOOL is_update = (version < g_versionNO);
    if (is_update)
    {
		SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
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
				stemp_building.Baudrate = q.getValuebyName(L"Braudrate");
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

        remove((UTF8MBSTR)g_strDatabasefilepath);//ɾ��ԭ�е����ݿ�

        CString FilePath;
        HANDLE hFind;
        WIN32_FIND_DATA wfd;
        hFind = FindFirstFile(g_strDatabasefilepath, &wfd);//
        if (hFind==INVALID_HANDLE_VALUE)//�����ڸ��ļ�
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
				m_Building_ALL[i].Building_Name
				, m_Building_ALL[i].Default_Building
				, m_Building_ALL[i].Telephone
				, m_Building_ALL[i].Address);
			SqliteDBT3000.execDML((UTF8MBSTR)StrSql);
		}

		StrSql = L"Delete from Building";
		SqliteDBT3000.execDML((UTF8MBSTR)StrSql);

		for (int j=0;j<m_Building.size();j++)
		{
			StrSql.Format(_T("Insert Into Building(Main_BuildingName,Building_Name,Protocal,Com_Port,Ip_Address,Ip_Port,Braudrate,Default_SubBuilding,Building_Path,Longitude,Elevation,ID,country,state,city,street,ZIP,EngineeringUnits) Values('%s','%s','%s','%s','%s','%s','%s',%d,'%s','%s','%s',%d,'%s','%s','%s','%s','%s','%s')"),
				m_Building[j].Main_BuildingName,
				m_Building[j].Building_Name,
				m_Building[j].Protocal,
				m_Building[j].Com_Port,
				m_Building[j].Ip_Address,
				m_Building[j].Ip_Port,
				m_Building[j].Baudrate,
				m_Building[j].Default_SubBuilding,
				m_Building[j].Building_Path,
				m_Building[j].Longitude,
				m_Building[j].Elevation,
				m_Building[j].ID,
				m_Building[j].country,
				m_Building[j].state,
				m_Building[j].city,
				m_Building[j].street,
				m_Building[j].ZIP,
				m_Building[j].EngineeringUnits);
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
    //2018 04 23 �޸�bug Ĭд����ϵͳ����C�̵������װ�ؼ�ʧ��
    //����취  ��ȡϵͳ�����̷� ��Ȼ���ȡ��Ӧ����.
    CString Local_System_Path;
    TCHAR szPath[MAX_PATH];
    DWORD ret;
    ret = GetWindowsDirectory(szPath, MAX_PATH);
    if (!ret) 
        Local_System_Path = _T("C:\\Windows");
    else
        Local_System_Path = szPath;
	try
	{
		//if (ReadDLLRegAsm()<1)
		{
#if 1 // �ŷ�����  �� ���ɱ����� ��⵽  RegAsm.exe �ķ��ʲ��Ϸ��� ������;
            CString temp_dotnet_path;
            CString temp_t3000controlldll_path;
            CString temp_bacnetdll;
            CString temp_Regasm_path;
            temp_dotnet_path = Local_System_Path + _T("\\Microsoft.NET\\Framework\\v4.0.30319");
            temp_t3000controlldll_path = temp_dotnet_path + _T("\\T3000Controls.dll");
            temp_bacnetdll = temp_dotnet_path + _T("\\TemcoStandardBacnetTool.dll");
            temp_Regasm_path = temp_dotnet_path + _T("\\RegAsm.exe");
			CopyFile(strSource, temp_t3000controlldll_path, FALSE);
			// ::ShellExecute(NULL, _T("open"), _T("C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\RegAsm.exe T3000Controls.dll"), _T(""), _T(""), SW_SHOW); 
			ShellExecute(NULL, _T("open"), temp_Regasm_path, temp_t3000controlldll_path, NULL, SW_HIDE);
			strSource = g_strExePth + L"TemcoStandardBacnetTool.dll";
			CopyFile(strSource, temp_bacnetdll, FALSE);
			// ::ShellExecute(NULL, _T("open"), _T("C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\RegAsm.exe T3000Controls.dll"), _T(""), _T(""), SW_SHOW); 
			ShellExecute(NULL, _T("open"), temp_Regasm_path, temp_bacnetdll, NULL, SW_HIDE);

			strSource = g_strExePth+L"ControlBasicEditor.dll";
			temp_bacnetdll=temp_dotnet_path+L"\\ControlBasicEditor.dll";
			CopyFile(strSource, temp_bacnetdll, FALSE);
			strSource = g_strExePth + L"ControlBasicEditor.tlb";
			temp_bacnetdll = temp_dotnet_path + L"\\ControlBasicEditor.tlb";
			CopyFile(strSource, temp_bacnetdll, FALSE);
			strSource=_T("ControlBasicEditor.dll tlb:/ControlBasicEditor.tlb  /codebase");
			// ::ShellExecute(NULL, _T("open"), _T("C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\RegAsm.exe T3000Controls.dll"), _T(""), _T(""), SW_SHOW); 
			ShellExecute(NULL, _T("open"), temp_Regasm_path, strSource, NULL, SW_HIDE);

#endif
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
            CreateDirectory(g_strExePth + _T("ResourceFile"), NULL);//creat database folder;//
            CString cs_update_folder;
            cs_update_folder = g_strDatabasefilepath + _T("Database") + _T("\\") + _T("Update");
            DeleteDirectory(cs_update_folder);
			CreateDirectory(g_strExePth+_T("Database"),NULL);//creat database folder;//

			g_achive_device_name_path = g_strDatabasefilepath + _T("Database") + _T("\\") + _T("temp\\") + _T("device_name.ini") ;
			g_achive_folder = g_strDatabasefilepath + _T("Database") + _T("\\") + _T("temp");
			g_achive_folder_temp_txt = g_achive_folder + _T("\\") + _T("prg_txt_file");
			g_achive_folder_temp_db = g_achive_folder + _T("\\") + _T("MonitorDatabaseFolder");
			g_cstring_ini_path = g_achive_folder + _T("\\MonitorIndex.ini");
			
			// Use CIniFile to read and write .ini files
			CIniFile monitorindexfile(g_cstring_ini_path);
			product_sort_way = monitorindexfile.GetProfileIntW(_T("Setting"), _T("ProductSort"), 0);
			if(product_sort_way == 0)
			{
				monitorindexfile.WriteProfileStringW(_T("Setting"), _T("ProductSort"), _T("1"));
				product_sort_way = SORT_BY_CONNECTION;
			}

#if 0
            DEBUG_DELAY_TIME = 350;/*GetPrivateProfileInt(_T("Debug"), _T("SEND_COMMAND_DELAY_TIME"), 350, g_cstring_ini_path);
            if (DEBUG_DELAY_TIME == 350)
            {
                WritePrivateProfileStringW(_T("Debug"), _T("SEND_COMMAND_DELAY_TIME"), _T("350"), g_cstring_ini_path);
            }*/
#endif

			CString temp_cs_version;
			temp_cs_version.Format(_T("%d"),T3000_Version);
			monitorindexfile.WriteProfileStringW(_T("Version"), _T("T3000"), temp_cs_version);

			monitor_ignore_enable = monitorindexfile.GetProfileIntW(_T("Setting"), _T("EnableMonitorValueIgnore"), 0);
			if (monitor_ignore_enable == 0)
			{
				monitorindexfile.WriteProfileStringW(_T("Setting"), _T("EnableMonitorValueIgnore"), _T("0"));
				monitorindexfile.WriteProfileStringW(_T("Setting"), _T("MonitorValueIgnoreMax"), _T("10000000"));
				monitorindexfile.WriteProfileStringW(_T("Setting"), _T("MonitorValueIgnoreMin"), _T("-100000"));
			}

			if (monitor_ignore_enable == 1)
			{
				monitor_ignore_max_value = monitorindexfile.GetProfileIntW(_T("Setting"), _T("MonitorValueIgnoreMax"), 1000000);
				monitor_ignore_min_value = monitorindexfile.GetProfileIntW(_T("Setting"), _T("MonitorValueIgnoreMin"), -100000);
			}

			g_achive_monitor_datatbase_path = g_achive_folder;
			g_achive_monitor_datatbase_path = g_achive_monitor_datatbase_path + _T("\\MonitorData.mdb");

			WIN32_FIND_DATA fd;
			BOOL ret = FALSE;
			HANDLE hFind_folder = FindFirstFile(g_achive_folder, &fd);
			if ((hFind_folder != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//Ŀ¼����
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
				//Ŀ¼����
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
				//Ŀ¼����
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

#if 1//����]��T3000 ����r��

			CString FilePath;
 
			hFind = FindFirstFile(g_strDatabasefilepath, &wfd);//
			if (hFind==INVALID_HANDLE_VALUE)//˵����ǰĿ¼����t3000.mdb
			{
				
				//û���ҵ��ʹ���һ��Ĭ�ϵ����ݿ�
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
			if (hFind_Monitor==INVALID_HANDLE_VALUE)//˵����ǰĿ¼����MonitorData.db
			{
				//û���ҵ��ʹ���һ��Ĭ�ϵ����ݿ�
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




		//	g_strDatabasefilepath=(CString)FOR_DATABASE_CONNECT+g_strDatabasefilepath;//
			g_strImgeFolder=g_strExePth+_T("Database\\image\\");//
			CreateDirectory(g_strImgeFolder,NULL);//
			g_strBuildingFolder=g_strExePth+_T("Database\\Buildings\\");
			CreateDirectory(g_strBuildingFolder,NULL);//


			if (First_Start)
			{
				//����Default_Building
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
				if (hFind==INVALID_HANDLE_VALUE)//˵����ǰĿ¼����t3000.mdb
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
			//RUNTIME_CLASS(CT3000View));
            RUNTIME_CLASS(CT3000DefaultView));
        
        
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
		ULONG_PTR g_gdiplusToken;
		GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);//


		 ((CMainFrame*)m_pMainWnd)->InitViews();//
		CString strTile;
        strTile = cs_special_name;
		strTile+=CurrentT3000Version;
		m_pMainWnd->SetWindowText(strTile);//
		m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
		m_pMainWnd->ShowWindow(SW_SHOW);
		m_pMainWnd->UpdateWindow();
	   ((CMainFrame*)m_pMainWnd)->SwitchToPruductType(DLG_DIALOG_DEFAULT_BUILDING); 

       m_szAppPath  = g_strExePth;
       if(m_special_customer == 1) //����ǵ�һ���ͻ� �Ͷ���ΪCPR-1000-Help.chm
           m_szHelpFile = theApp.m_szAppPath + L"CPR-1000-Help.chm";
       else
            m_szHelpFile = theApp.m_szAppPath + L"T3000_Help.chm";
	   m_szTstatHelpFile = theApp.m_szAppPath + L"HelpDoc//" + L"TStat8_Help.chm";

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
		
		AfxMessageBox(_T("Database error! Please restart again ."));
		return TRUE;
	}

#ifdef LOCAL_DB_FUNCTION
	init_product_list();
#endif
	for (int i = 0; i < 256; i++)
	{
		g_bac_panel[i].panel_number = i;
		g_bac_panel[i].object_instance = 0;
		g_bac_panel[i].last_update_time = 0;
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

	}
	FindClose(hFile);
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
#include"Tstatrangedlg.h"
void CT3000App::OnVersionInfo()
{
	//CTstatRangeDlg dlg;
	//dlg.DoModal();
	//int rangevalue = dlg.m_current_range;

	/*CString strHistotyFile=g_strExePth+_T("history.txt");
	ShellExecute(NULL, _T("open"), strHistotyFile, NULL, NULL, SW_SHOWNORMAL);*/
	
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

void CT3000App::WriteNumber( CRegKey& key,CStringW valueName,DWORD value )
{
	key.SetDWORDValue(valueName,value);
}

BOOL CT3000App::ReadNameber( CRegKey& key,CStringW valueName,DWORD& value )
{
	return key.QueryDWORDValue(valueName, value) == ERROR_SUCCESS;
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
#include "CWebBrowser2.h"
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
	temp_version = cs_special_name + CurrentT3000Version;
	
	GetDlgItem(IDC_STATIC_ABOUT_INFO)->SetWindowText(temp_version);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
