
// T3000.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "T3000.h"
#include "MainFrm.h"

#include "T3000Doc.h"
#include "T3000View.h"
#include "T3000TableView.h"
#include "DialogCM5_BacNet.h"

#include "LoginDlg.h"

#include "iniFile.h"
#include "afxinet.h"
#include "T3000DefaultView.h"
#include "bado/BADO.h"
const int g_versionNO=20150605;
//CString const CurrentT3000Version=_T("2014.12.24");
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "global_variable.h"
ULONG_PTR g_gdiplusToken;
BEGIN_MESSAGE_MAP(CT3000App, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CT3000App::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_SAVE_CONFIG, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_VERSIONHISTORY,OnVersionInfo)
END_MESSAGE_MAP()
	
// CT3000App construction 
CT3000App::CT3000App()
{
 
		m_bHiColorIcons = TRUE;
		CurrentT3000Version=_T("    2015.12.23");
//	}
// 	catch (...)
// 	{
// 		
// 		AfxMessageBox(_T("1111"));
// 	}
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
	LPCTSTR   pszDllName   =   OcxFileName   ;     //ActiveX控件的路径及文件名       
	HINSTANCE   hLib   =   LoadLibrary(pszDllName);   //装载ActiveX控件   
	if   (hLib   <   (HINSTANCE)HINSTANCE_ERROR)   
	{   
		return   FALSE;   
	}   
	FARPROC   lpDllEntryPoint;     
	lpDllEntryPoint   =   GetProcAddress(hLib,(LPCSTR)(_T("DllRegisterServer")));   //获取注册函数DllRegisterServer地址   
	if(lpDllEntryPoint!=NULL)       //调用注册函数DllRegisterServer   
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
 void CT3000App::ImportData(){
//   CADO ado;
//   ado.OnInitADOConn();
	 CBADO bado;
	 bado.SetDBPath(g_strCurBuildingDatabasefilePath);
	 bado.OnInitADOConn(); 
  CString sql=_T("select * from ALL_NODE");
  bado.m_pRecordset=bado.OpenRecordset(sql);
  ALL_NODE temp;
  while(!bado.m_pRecordset->EndOfFile){
	   temp.MainBuilding_Name=bado.m_pRecordset->GetCollect(_T("MainBuilding_Name"));
	   temp.Building_Name=bado.m_pRecordset->GetCollect(_T("Building_Name"));
	   temp.Serial_ID=bado.m_pRecordset->GetCollect(_T("Serial_ID"));
	   temp.Floor_name=bado.m_pRecordset->GetCollect(_T("Floor_name"));
	   temp.Room_name=bado.m_pRecordset->GetCollect(_T("Room_name"));
	   temp.Product_name=bado.m_pRecordset->GetCollect(_T("Product_name"));
	   temp.Product_class_ID=bado.m_pRecordset->GetCollect(_T("Product_class_ID"));
	   temp.Product_ID=bado.m_pRecordset->GetCollect(_T("Product_ID"));
	   temp.Screen_Name=bado.m_pRecordset->GetCollect(_T("Screen_Name"));
	   temp.Bautrate=bado.m_pRecordset->GetCollect(_T("Bautrate"));
	   temp.Background_imgID=bado.m_pRecordset->GetCollect(_T("Background_imgID"));
	   temp.Hardware_Ver=bado.m_pRecordset->GetCollect(_T("Hardware_Ver"));
	   temp.Software_Ver=bado.m_pRecordset->GetCollect(_T("Software_Ver"));
	   temp.Com_Port=bado.m_pRecordset->GetCollect(_T("Com_Port"));
	   temp.EPsize=bado.m_pRecordset->GetCollect(_T("EPsize"));
	   temp.Protocol=bado.m_pRecordset->GetCollect(_T("Protocol"));
      
	   bado.m_pRecordset->MoveNext();

	   m_AllNodes.push_back(temp);
  }
  bado.CloseRecordset();
  bado.CloseConn();
 }

 BOOL CT3000App::Is_haveTable(CString TableName){
 CADO ado;
 BOOL Ret=ado.OnInitADOConn();
 if (!Ret)
 {
	 return Ret;
 }
 if (ado.IsHaveTable(ado,TableName))//有Version表
 {
 return TRUE;
 }

 return FALSE;
 }

 BOOL CT3000App::JudgeDB(){
     int versionno=0;
     CString StrSql;
     CADO ado;
     BOOL Ret=ado.OnInitADOConn();

     if (!Ret)
     {
         return Ret;
     }

     if (ado.IsHaveTable(ado,_T("Version")))//有Version表
     {
         CString sql=_T("Select * from Version");
         ado.m_pRecordset=ado.OpenRecordset(sql);
         ado.m_pRecordset->MoveFirst();
         while (!ado.m_pRecordset->EndOfFile)
         {
             versionno=ado.m_pRecordset->GetCollect(_T("VersionNO"));
             ado.m_pRecordset->MoveNext();
         }
         ado.CloseRecordset();


     } 
     CString Builing_DBPath;
     StrSql=_T("Select * From Building Where Default_SubBuilding = -1");
     ado.m_pRecordset = ado.OpenRecordset(StrSql);
     if (!ado.m_pRecordset->EndOfFile)
     {
         Builing_DBPath = ado.m_pRecordset->GetCollect(_T("Building_Path"));
     }
     ado.CloseConn();
     int index=Builing_DBPath.Find(_T("Database"));
     Builing_DBPath.Delete(0,index);

     g_strCurBuildingDatabasefilePath = GetExePath(true)+ Builing_DBPath;
     if ((g_versionNO>versionno)&&(versionno<=20141116))                 //版本过低
     {
         //SetPaneString(0,_T("The version of DB is lower,Updating....."));
         _variant_t temp_var;
         _ConnectionPtr srcConTmp;
         _RecordsetPtr srcRsTemp;
         srcConTmp.CreateInstance("ADODB.Connection");
         srcRsTemp.CreateInstance("ADODB.Recordset");
         srcConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);//打开数据库
         //这里是All_NODE
         if (Is_haveTable(_T("ALL_NODE")))
         {

#if 1
             srcRsTemp->Open(_T("select * from ALL_NODE"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);		
             ALL_NODE temp;
             while(!srcRsTemp->EndOfFile){
                 temp.MainBuilding_Name=srcRsTemp->GetCollect(_T("MainBuilding_Name"));
                 temp.Building_Name=srcRsTemp->GetCollect(_T("Building_Name"));
                 temp.Serial_ID=srcRsTemp->GetCollect(_T("Serial_ID"));
                 temp.Floor_name=srcRsTemp->GetCollect(_T("Floor_name"));
                 temp.Room_name=srcRsTemp->GetCollect(_T("Room_name"));
                 temp.Product_name=srcRsTemp->GetCollect(_T("Product_name"));
                 temp.Product_class_ID=srcRsTemp->GetCollect(_T("Product_class_ID"));
                 temp.Product_ID=srcRsTemp->GetCollect(_T("Product_ID"));
                 temp.Screen_Name=srcRsTemp->GetCollect(_T("Screen_Name"));
                 temp_var=srcRsTemp->GetCollect(_T("Bautrate"));
                 temp.Custom = srcRsTemp->GetCollect(_T("Custom"));
                 // temp.Com_Port = srcRsTemp->GetCollect(_T("Com_Port"));

                 if (temp_var.vt!=VT_NULL)
                 { temp.Bautrate=temp_var;
                 }
                 else
                 {
                     temp.Bautrate=_T("");
                 }
                 temp.Background_imgID=srcRsTemp->GetCollect(_T("Background_imgID"));
                 temp_var=srcRsTemp->GetCollect(_T("Hardware_Ver"));
                 if (temp_var.vt!=VT_NULL)
                 { temp.Hardware_Ver=temp_var;
                 }
                 else
                 {
                     temp.Hardware_Ver=_T("");
                 }
                 temp_var=srcRsTemp->GetCollect(_T("Software_Ver"));
                 if (temp_var.vt!=VT_NULL)
                 { temp.Software_Ver=temp_var;
                 }
                 else
                 {
                     temp.Software_Ver=_T("");
                 }
                 temp_var=srcRsTemp->GetCollect(_T("Com_Port"));
                 if (temp_var.vt!=VT_NULL)
                 { 
                     temp.Com_Port=temp_var;
                 }
                 else
                 {
                     temp.Com_Port=_T("");
                 }
                 temp_var=srcRsTemp->GetCollect(_T("EPsize"));
                 if (temp_var.vt!=VT_NULL)
                 { temp.EPsize=temp_var;
                 }
                 else
                 {
                     temp.EPsize=_T("");
                 }
                 temp_var=srcRsTemp->GetCollect(_T("Protocol"));
                 if (temp_var.vt!=VT_NULL)
                 { temp.Protocol=temp_var;
                 }
                 else
                 {
                     temp.Protocol=_T("");
                 }
                 srcRsTemp->MoveNext();
                 m_AllNodes.push_back(temp);
             }
             srcRsTemp->Close();
#endif
         }
         /*
         vector<Building> m_Building;
         vector<Building_ALL> m_Building_ALL;
         vector<CustomProductTable>  m_CustomProductTable;
         vector<IONAME> m_IONAME;
         vector<IONAME_Config> m_IONAME_Config;
         vector<LCNameConfig> m_LCNameConfig;
         vector<LightingController_Name> m_LightingController_Name;
         vector<Product_Data> m_Product_Data;
         vector<Screen_Label> m_Screen_Label;
         */
         //Building_ALL
         if (Is_haveTable(_T("Building_ALL")))
         {
#if 1

             srcRsTemp->Open(_T("select * from Building_ALL"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);		
             Building_ALL Building_ALL_Temp;
             while(!srcRsTemp->EndOfFile){
                 Building_ALL_Temp.Building_Name=srcRsTemp->GetCollect(_T("Building_Name"));
                 Building_ALL_Temp.Address=srcRsTemp->GetCollect(_T("Address"));
                 Building_ALL_Temp.Telephone=srcRsTemp->GetCollect(_T("Telephone"));
                 Building_ALL_Temp.Default_Building=srcRsTemp->GetCollect(_T("Default_Build"));
                 m_Building_ALL.push_back(Building_ALL_Temp);
                 srcRsTemp->MoveNext();
             }
             srcRsTemp->Close();
#endif
         }


         //Building
         if (Is_haveTable(_T("Building")))
         {
#if 1
             srcRsTemp->Open(_T("select * from Building"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);		
             Building Building_Temp;
             while(!srcRsTemp->EndOfFile){

                 Building_Temp.Braudrate=srcRsTemp->GetCollect(_T("Braudrate"));
                 Building_Temp.Building_Name=srcRsTemp->GetCollect(_T("Building_Name"));
                 Building_Temp.Com_Port=srcRsTemp->GetCollect(_T("Com_Port"));
                 Building_Temp.Default_SubBuilding=srcRsTemp->GetCollect(_T("Default_SubBuilding"));
                 Building_Temp.Ip_Address=srcRsTemp->GetCollect(_T("Ip_Address"));
                 Building_Temp.Ip_Port=srcRsTemp->GetCollect(_T("Ip_Port"));
                 Building_Temp.Main_BuildingName=srcRsTemp->GetCollect(_T("Main_BuildingName"));
                 Building_Temp.Protocal=srcRsTemp->GetCollect(_T("Protocal"));
                 m_Building.push_back(Building_Temp);
                 srcRsTemp->MoveNext();
             }
             srcRsTemp->Close();
#endif
         }


         //CustomProductTable
         if (Is_haveTable(_T("CustomProductTable")))
         {
#if 1
             srcRsTemp->Open(_T("select * from CustomProductTable"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);		
             CustomProductTable CustomProductTable_Temp;
             while(!srcRsTemp->EndOfFile){
                 CustomProductTable_Temp.ModelNo=srcRsTemp->GetCollect(_T("ModelNo"));
                 CustomProductTable_Temp.Reg_Description=srcRsTemp->GetCollect(_T("Reg_Description"));
                 CustomProductTable_Temp.Reg_ID=srcRsTemp->GetCollect(_T("Reg_ID"));

                 m_CustomProductTable.push_back(CustomProductTable_Temp);
                 srcRsTemp->MoveNext();
             }
             srcRsTemp->Close();
#endif
         }


         //IONAME
         if (Is_haveTable(_T("IONAME")))
         {
#if 1
             srcRsTemp->Open(_T("select * from IONAME"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);		
             IONAME IONAME_Temp;
             while(!srcRsTemp->EndOfFile){
                 IONAME_Temp.INPUT1=srcRsTemp->GetCollect(_T("INPUT1"));
                 IONAME_Temp.INPUT2=srcRsTemp->GetCollect(_T("INPUT2"));
                 IONAME_Temp.INPUT3=srcRsTemp->GetCollect(_T("INPUT3"));
                 IONAME_Temp.INPUT4=srcRsTemp->GetCollect(_T("INPUT4"));
                 IONAME_Temp.INPUT5=srcRsTemp->GetCollect(_T("INPUT5"));
                 IONAME_Temp.INPUT6=srcRsTemp->GetCollect(_T("INPUT6"));
                 IONAME_Temp.INPUT7=srcRsTemp->GetCollect(_T("INPUT7"));
                 IONAME_Temp.INPUT8=srcRsTemp->GetCollect(_T("INPUT8"));
                 IONAME_Temp.INPUT9=srcRsTemp->GetCollect(_T("INPUT9"));
                 IONAME_Temp.OUTPUT1=srcRsTemp->GetCollect(_T("OUTPUT1"));
                 IONAME_Temp.OUTPUT2=srcRsTemp->GetCollect(_T("OUTPUT2"));
                 IONAME_Temp.OUTPUT3=srcRsTemp->GetCollect(_T("OUTPUT3"));
                 IONAME_Temp.OUTPUT4=srcRsTemp->GetCollect(_T("OUTPUT4"));
                 IONAME_Temp.OUTPUT5=srcRsTemp->GetCollect(_T("OUTPUT5"));
                 IONAME_Temp.OUTPUT6=srcRsTemp->GetCollect(_T("OUTPUT6"));
                 IONAME_Temp.OUTPUT7=srcRsTemp->GetCollect(_T("OUTPUT7"));
                 IONAME_Temp.SENSORNAME=srcRsTemp->GetCollect(_T("SENSORNAME"));
                 IONAME_Temp.SERIAL_ID=srcRsTemp->GetCollect(_T("SERIAL_ID"));


                 m_IONAME.push_back(IONAME_Temp);
                 srcRsTemp->MoveNext();
             }
             srcRsTemp->Close();
#endif
         }

         //IONAME_Config
         if (Is_haveTable(_T("IONAME_CONFIG"))) {
#if 1
             srcRsTemp->Open(_T("select * from IONAME_CONFIG"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);		
             IONAME_Config IONAME_Config_Temp;
             while(!srcRsTemp->EndOfFile){
                 IONAME_Config_Temp.InOutName=srcRsTemp->GetCollect(_T("InOutName"));
                 IONAME_Config_Temp.Row=srcRsTemp->GetCollect(_T("Row"));
                 IONAME_Config_Temp.SerialNo=srcRsTemp->GetCollect(_T("SerialNo"));
                 IONAME_Config_Temp.Type=srcRsTemp->GetCollect(_T("Type"));

                 m_IONAME_Config.push_back(IONAME_Config_Temp);
                 srcRsTemp->MoveNext();
             }
             srcRsTemp->Close();
#endif

         }

         //LCNameConfig
         if (Is_haveTable(_T("LCNameConfigure")))
         {
#if 1
             srcRsTemp->Open(_T("select * from LCNameConfigure"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);		
             LCNameConfigure LCNameConfigure_Temp;
             while(!srcRsTemp->EndOfFile){
                 LCNameConfigure_Temp.Card=srcRsTemp->GetCollect(_T("Card"));
                 LCNameConfigure_Temp.Output=srcRsTemp->GetCollect(_T("Output"));
                 LCNameConfigure_Temp.OutputName=srcRsTemp->GetCollect(_T("OutputName"));
                 LCNameConfigure_Temp.SN=srcRsTemp->GetCollect(_T("SN"));
                 m_LCNameConfigure.push_back(LCNameConfigure_Temp);
                 srcRsTemp->MoveNext();
             }
             srcRsTemp->Close();
#endif
         }



         //Product_Data
         if (Is_haveTable(_T("Product_Data")))
         {
#if 1
             srcRsTemp->Open(_T("select * from Product_Data"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);		
             Product_Data Product_Data_Temp;
             while(!srcRsTemp->EndOfFile){
                 Product_Data_Temp.Serial_ID=srcRsTemp->GetCollect(_T("Serial_ID"));
                 Product_Data_Temp.Register_Data=srcRsTemp->GetCollect(_T("Register_Data"));
                 m_Product_Data.push_back(Product_Data_Temp);
                 srcRsTemp->MoveNext();
             }
             srcRsTemp->Close();
#endif
         }

         //Screen_Label
         if (Is_haveTable(_T("Screen_Label")))
         {
#if 1
             srcRsTemp->Open(_T("select * from Screen_Label"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);		
             Screen_Label Screen_Label_Temp;
             while(!srcRsTemp->EndOfFile){
                 Screen_Label_Temp.Back_Color=srcRsTemp->GetCollect(_T("Back_Color"));
                 Screen_Label_Temp.Cstatic_id=srcRsTemp->GetCollect(_T("Cstatic_id"));
                 Screen_Label_Temp.Height=srcRsTemp->GetCollect(_T("Height"));
                 Screen_Label_Temp.Input_or_Output=srcRsTemp->GetCollect(_T("Input_or_Output"));
                 Screen_Label_Temp.Point_X=srcRsTemp->GetCollect(_T("Point_X"));
                 Screen_Label_Temp.Point_Y=srcRsTemp->GetCollect(_T("Point_Y"));
                 Screen_Label_Temp.Serial_Num=srcRsTemp->GetCollect(_T("Serial_Num"));
                 Screen_Label_Temp.Status=srcRsTemp->GetCollect(_T("Status"));
                 Screen_Label_Temp.Text_Color=srcRsTemp->GetCollect(_T("Text_Color"));
                 Screen_Label_Temp.Tips=srcRsTemp->GetCollect(_T("Tips"));
                 Screen_Label_Temp.Tstat_id=srcRsTemp->GetCollect(_T("Tstat_id"));
                 Screen_Label_Temp.Width=srcRsTemp->GetCollect(_T("Width"));

                 m_Screen_Label.push_back(Screen_Label_Temp);
                 srcRsTemp->MoveNext();
             }
             srcRsTemp->Close();
#endif
         }


         //Value_Range
         if (Is_haveTable(_T("Value_Range")))
         {
#if 1
             srcRsTemp->Open(_T("select * from Value_Range"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);		
             Value_Range Value_Range_Temp;
             while(!srcRsTemp->EndOfFile){
                 Value_Range_Temp.SN=srcRsTemp->GetCollect(_T("SN"));
                 Value_Range_Temp.CInputNo=srcRsTemp->GetCollect(_T("CInputNo"));
                 Value_Range_Temp.CRange=srcRsTemp->GetCollect(_T("CRange"));
                 srcRsTemp->MoveNext();
                 m_Value_Range.push_back(Value_Range_Temp);
             }
             srcRsTemp->Close();
#endif
         }



         if (srcConTmp->State)
         {srcConTmp->Close();
         }




#if 1//刪除老的T3000.mdb ，拷貝一個新的到當前目錄
         CString filePath=g_strExePth+_T("Database\\T3000.mdb");
         DeleteFile(filePath);
         HANDLE hFind;//
         WIN32_FIND_DATA wfd;//
         hFind = FindFirstFile(filePath, &wfd);//
         if (hFind==INVALID_HANDLE_VALUE)//说明当前目录下无t3000.mdb
         {
             //CopyFile(g_strOrigDatabaseFilePath,g_strDatabasefilepath,FALSE);//
             //没有找到就创建一个默认的数据库
             filePath=g_strExePth+_T("Database\\T3000.mdb");
             HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_T3000_DB2), _T("T3000_DB"));   
             HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);   


             LPVOID lpExe = LockResource(hGlobal);   
             CFile file;
             if(file.Open(filePath, CFile::modeCreate | CFile::modeWrite))    
                 file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));    
             file.Close();    
             ::UnlockResource(hGlobal);   
             ::FreeResource(hGlobal);
         }//
#endif






#if 1		//把原来T3000 里面的老数据导入到Building的数据表中

         CString strsql;
         srcConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
         try
         {

             //Building_ALL
#if 1
             srcConTmp->Execute(_T("delete * from Building_ALL"),NULL,adCmdText);	
             srcRsTemp->Open(_T("select * from Building_ALL"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
             try{
                 for (int i=0;i<m_Building_ALL.size();i++)
                 {
                     srcRsTemp->AddNew();
                     srcRsTemp->PutCollect("Building_Name",m_Building_ALL[i].Building_Name);
                     srcRsTemp->PutCollect("Default_Build",m_Building_ALL[i].Default_Building);
                     srcRsTemp->PutCollect("Telephone",m_Building_ALL[i].Telephone);
                     srcRsTemp->PutCollect("Address",m_Building_ALL[i].Address);


                     srcRsTemp->Update();
                 }
             }
             catch(_com_error &e){
                 AfxMessageBox(e.Description());
             }
             srcRsTemp->Close();
#endif

             //Building   Building_Path
#if 1
             srcConTmp->Execute(_T("delete * from Building"),NULL,adCmdText);
             srcRsTemp->Open(_T("select * from Building"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
             try{
                 for (int i=0;i<m_Building.size();i++)
                 {
#if 1//創建一個默認的Building:DefaultBuilding
                     CString filebuildingPath;//=g_strBuildingFolder+m_Building.at(i).Main_BuildingName+_T("\\"); 
                     filebuildingPath.Format(_T("%s%s\\"),g_strBuildingFolder,(CString)m_Building.at(i).Main_BuildingName);
                     CreateDirectory(g_strBuildingFolder,NULL);
                     CreateDirectory(filebuildingPath,NULL);
                     filebuildingPath+=(CString)m_Building.at(i).Main_BuildingName;//_T(".mdb");
                     filebuildingPath+=_T(".mdb");

                     DeleteFile(filebuildingPath);
                     // 	 HANDLE hFind;//
                     // 	 WIN32_FIND_DATA wfd;//
                     //create building db file

                     hFind = FindFirstFile(filebuildingPath, &wfd);//
                     if (hFind==INVALID_HANDLE_VALUE)//说明当前目录下无t3000.mdb
                     {

                         HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_BUILDING_DB), _T("BUILDING_DB"));   
                         HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);   


                         LPVOID lpExe = LockResource(hGlobal);   
                         CFile file;
                         if(file.Open(filebuildingPath, CFile::modeCreate | CFile::modeWrite))    
                             file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));    
                         file.Close();    
                         ::UnlockResource(hGlobal);   
                         ::FreeResource(hGlobal);
                     }//

#endif

                     int index= filebuildingPath.Find(_T("Database"));
                     filebuildingPath.Delete(0,index);
                     if (m_Building.at(i).Default_SubBuilding)
                     {
                         g_strCurBuildingDatabasefilePath=GetExePath(true)+filebuildingPath;
                     }
                     m_Building.at(i).Building_Path=(_variant_t)filebuildingPath;
                     srcRsTemp->AddNew();
                     srcRsTemp->PutCollect("Main_BuildingName",m_Building[i].Main_BuildingName);
                     srcRsTemp->PutCollect("Building_Name",m_Building[i].Main_BuildingName);
                     srcRsTemp->PutCollect("Protocal",m_Building[i].Protocal);
                     srcRsTemp->PutCollect("Com_Port",m_Building[i].Com_Port);
                     srcRsTemp->PutCollect("Ip_Address",m_Building[i].Ip_Address);
                     srcRsTemp->PutCollect("Ip_Port",m_Building[i].Ip_Port);
                     srcRsTemp->PutCollect("Braudrate",m_Building[i].Braudrate);
                     srcRsTemp->PutCollect("Default_SubBuilding",m_Building[i].Default_SubBuilding);
                     srcRsTemp->PutCollect("Building_Path", m_Building.at(i).Building_Path);
                     srcRsTemp->Update();
                 }
             }
             catch(_com_error &e){
                 AfxMessageBox(e.Description());
             }
             srcRsTemp->Close();
#endif



         }
         catch(_com_error *e)
         {
             AfxMessageBox(e->ErrorMessage());
         }



         if (srcConTmp->State)
         {srcConTmp->Close();
         }
         srcConTmp=NULL;
#endif



         CBADO bado;
         bado.SetDBPath(g_strCurBuildingDatabasefilePath);
         bado.OnInitADOConn();
#if 1		//把原来T3000 里面的老数据导入到Building的数据表中



         try
         {
             //ALL_NODE
#if 1
             for (int i=0;i<m_AllNodes.size();i++)
             {
                 strsql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,Custom,EPsize) values('"
                     +m_AllNodes[i].MainBuilding_Name+"','"
                     +m_AllNodes[i].MainBuilding_Name+"','"
                     +m_AllNodes[i].Serial_ID+"','"
                     +m_AllNodes[i].Floor_name+"','"
                     +m_AllNodes[i].Room_name+"','"
                     +m_AllNodes[i].Product_name+"','"
                     +m_AllNodes[i].Product_class_ID+"','"
                     +m_AllNodes[i].Product_ID+"','"
                     +m_AllNodes[i].Screen_Name+"','"
                     +m_AllNodes[i].Bautrate+"','"
                     +m_AllNodes[i].Background_imgID+"','"
                     +m_AllNodes[i].Hardware_Ver+"','"
                     +m_AllNodes[i].Software_Ver+"','"
                     +m_AllNodes[i].Com_Port+"','"
                     +m_AllNodes[i].Custom+"','"
                     +m_AllNodes[i].EPsize+"')"));


                 bado.m_pConnection->Execute(strsql.GetString(),NULL,adCmdText);	
             }
#endif


             //IONAME
#if 1
             bado.m_pConnection->Execute(_T("delete * from IONAME"),NULL,adCmdText);
             //srcRsTemp->Open(_T("select * from IONAME"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
             bado.m_pRecordset=bado.OpenRecordset(_T("select * from IONAME"));
             try{
                 for (int i=0;i<m_IONAME.size();i++)
                 {
                     bado.m_pRecordset->AddNew();
                     bado.m_pRecordset->PutCollect("SERIAL_ID",m_IONAME[i].SERIAL_ID);
                     bado.m_pRecordset->PutCollect("INPUT1",m_IONAME[i].INPUT1);
                     bado.m_pRecordset->PutCollect("INPUT2",m_IONAME[i].INPUT2);
                     bado.m_pRecordset->PutCollect("INPUT3",m_IONAME[i].INPUT3);
                     bado.m_pRecordset->PutCollect("INPUT4",m_IONAME[i].INPUT4);
                     bado.m_pRecordset->PutCollect("INPUT5",m_IONAME[i].INPUT5);
                     bado.m_pRecordset->PutCollect("INPUT6",m_IONAME[i].INPUT6);
                     bado.m_pRecordset->PutCollect("INPUT7",m_IONAME[i].INPUT7);
                     bado.m_pRecordset->PutCollect("INPUT8",m_IONAME[i].INPUT8);
                     bado.m_pRecordset->PutCollect("INPUT9",m_IONAME[i].INPUT9);
                     bado.m_pRecordset->PutCollect("OUTPUT1",m_IONAME[i].OUTPUT1);
                     bado.m_pRecordset->PutCollect("OUTPUT2",m_IONAME[i].OUTPUT2);
                     bado.m_pRecordset->PutCollect("OUTPUT3",m_IONAME[i].OUTPUT3);
                     bado.m_pRecordset->PutCollect("OUTPUT4",m_IONAME[i].OUTPUT4);
                     bado.m_pRecordset->PutCollect("OUTPUT5",m_IONAME[i].OUTPUT5);
                     bado.m_pRecordset->PutCollect("OUTPUT6",m_IONAME[i].OUTPUT6);
                     bado.m_pRecordset->PutCollect("OUTPUT7",m_IONAME[i].OUTPUT7);
                     bado.m_pRecordset->PutCollect("SENSORNAME",m_IONAME[i].SENSORNAME);

                     bado.m_pRecordset->Update();
                 }
             }
             catch(_com_error &e){
                 AfxMessageBox(e.Description());
             }
             //srcRsTemp->Close();
             bado.CloseRecordset();
#endif
             //IONAME_Config
#if 1
             bado.m_pConnection->Execute(_T("delete * from IONAME_CONFIG"),NULL,adCmdText);
             //srcRsTemp->Open(_T("select * from IONAME_CONFIG"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
             bado.m_pRecordset=bado.OpenRecordset(_T("select * from IONAME_CONFIG"));
             try{
                 for (int i=0;i<m_IONAME_Config.size();i++)
                 {
                     bado.m_pRecordset->AddNew();
                     bado.m_pRecordset->PutCollect("InOutName",m_IONAME_Config[i].InOutName);
                     bado.m_pRecordset->PutCollect("Type",m_IONAME_Config[i].Type);
                     bado.m_pRecordset->PutCollect("Row",m_IONAME_Config[i].Row);
                     bado.m_pRecordset->PutCollect("SerialNo",m_IONAME_Config[i].SerialNo);
                     bado.m_pRecordset->Update();
                 }
             }
             catch(_com_error &e){
                 AfxMessageBox(e.Description());
             }
             bado.CloseRecordset();
             //srcRsTemp->Close();
#endif
             //LCNameConfig
#if 1
             bado.m_pConnection->Execute(_T("delete * from LCNameConfigure"),NULL,adCmdText);
             //srcRsTemp->Open(_T("select * from LCNameConfigure"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
             bado.m_pRecordset=bado.OpenRecordset(_T("select * from LCNameConfigure"));
             try{
                 for (int i=0;i<m_LCNameConfigure.size();i++)
                 {
                     bado.m_pRecordset->AddNew();
                     bado.m_pRecordset->PutCollect("SN",m_LCNameConfigure[i].SN);
                     bado.m_pRecordset->PutCollect("Card",m_LCNameConfigure[i].Card);
                     bado.m_pRecordset->PutCollect("Output",m_LCNameConfigure[i].Output);
                     bado.m_pRecordset->PutCollect("OutputName",m_LCNameConfigure[i].OutputName);
                     bado.m_pRecordset->Update();
                 }
             }
             catch(_com_error &e){
                 AfxMessageBox(e.Description());
             }
             bado.CloseRecordset();
             //srcRsTemp->Close();
#endif


             //Product_Data
#if 1
             bado.m_pConnection->Execute(_T("delete * from Product_Data"),NULL,adCmdText);
             //srcRsTemp->Open(_T("select * from Product_Data"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
             bado.m_pRecordset=bado.OpenRecordset(_T("select * from Product_Data"));
             try{
                 for (int i=0;i<m_Product_Data.size();i++)
                 {
                     bado.m_pRecordset->AddNew();
                     bado.m_pRecordset->PutCollect("Serial_ID",m_Product_Data[i].Serial_ID);
                     bado.m_pRecordset->PutCollect("Register_Data",m_Product_Data[i].Register_Data);

                     bado.m_pRecordset->Update();
                 }
             }
             catch(_com_error &e){
                 AfxMessageBox(e.Description());
             }
             //srcRsTemp->Close();
             bado.CloseRecordset();
#endif
             //Screen_Label
#if 1
             bado.m_pConnection->Execute(_T("delete * from Screen_Label"),NULL,adCmdText);
             //srcRsTemp->Open(_T("select * from Screen_Label"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
             bado.m_pRecordset=bado.OpenRecordset(_T("select * from Screen_Label"));
             try{
                 for (int i=0;i<m_Screen_Label.size();i++)
                 {
                     bado.m_pRecordset->AddNew();
                     bado.m_pRecordset->PutCollect("Cstatic_id",m_Screen_Label[i].Cstatic_id);
                     bado.m_pRecordset->PutCollect("Serial_Num",m_Screen_Label[i].Serial_Num);
                     bado.m_pRecordset->PutCollect("Point_X",m_Screen_Label[i].Point_X);
                     bado.m_pRecordset->PutCollect("Point_Y",m_Screen_Label[i].Point_Y);
                     bado.m_pRecordset->PutCollect("Height",m_Screen_Label[i].Height);
                     bado.m_pRecordset->PutCollect("Width",m_Screen_Label[i].Width);
                     bado.m_pRecordset->PutCollect("Tstat_id",m_Screen_Label[i].Tstat_id);
                     bado.m_pRecordset->PutCollect("Status",m_Screen_Label[i].Status);
                     bado.m_pRecordset->PutCollect("Tips",m_Screen_Label[i].Tips);
                     bado.m_pRecordset->PutCollect("Input_or_Output",m_Screen_Label[i].Input_or_Output);
                     bado.m_pRecordset->PutCollect("Text_Color",m_Screen_Label[i].Text_Color);
                     bado.m_pRecordset->PutCollect("Back_Color",m_Screen_Label[i].Back_Color);
                     bado.m_pRecordset->Update();
                 }
             }
             catch(_com_error &e){
                 AfxMessageBox(e.Description());
             }
             bado.CloseRecordset();
#endif

             //Value_Range
#if 1
             bado.m_pConnection->Execute(_T("delete * from Value_Range"),NULL,adCmdText);
             //srcRsTemp->Open(_T("select * from Value_Range"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
             bado.m_pRecordset=bado.OpenRecordset(_T("select * from Value_Range"));
             try{
                 for (int i=0;i<m_Value_Range.size();i++)
                 {
                     bado.m_pRecordset->AddNew();
                     bado.m_pRecordset->PutCollect("SN",m_Value_Range[i].SN);
                     bado.m_pRecordset->PutCollect("CInputNo",m_Value_Range[i].CInputNo);
                     bado.m_pRecordset->PutCollect("CRange",m_Value_Range[i].CRange);
                     bado.m_pRecordset->Update();
                 }
             }
             catch(_com_error &e){
                 AfxMessageBox(e.Description());
             }
             bado.CloseRecordset();
             //	srcRsTemp->Close();
#endif

         }
         catch(_com_error *e)
         {
             AfxMessageBox(e->ErrorMessage());
         }
         bado.CloseConn();
#endif 



     }
     else if ((g_versionNO>versionno)&&(versionno>20141116 && versionno <20150415))
     {

         _variant_t temp_var;
         _ConnectionPtr srcConTmp;
         _RecordsetPtr srcRsTemp;
         srcConTmp.CreateInstance("ADODB.Connection");
         srcRsTemp.CreateInstance("ADODB.Recordset");
         srcConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);//打开数据库
         //这里是All_NODE

         //Building_ALL
         if (Is_haveTable(_T("Building_ALL")))
         {
#if 1

             srcRsTemp->Open(_T("select * from Building_ALL"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);		
             Building_ALL Building_ALL_Temp;
             while(!srcRsTemp->EndOfFile){
                 Building_ALL_Temp.Building_Name=srcRsTemp->GetCollect(_T("Building_Name"));
                 Building_ALL_Temp.Address=srcRsTemp->GetCollect(_T("Address"));
                 Building_ALL_Temp.Telephone=srcRsTemp->GetCollect(_T("Telephone"));
                 Building_ALL_Temp.Default_Building=srcRsTemp->GetCollect(_T("Default_Build"));
                 m_Building_ALL.push_back(Building_ALL_Temp);
                 srcRsTemp->MoveNext();
             }
             srcRsTemp->Close();
#endif
         }
         CString StrTemp;

         //Building
         if (Is_haveTable(_T("Building")))
         {
#if 1
             srcRsTemp->Open(_T("select * from Building"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);		
             Building Building_Temp;
             while(!srcRsTemp->EndOfFile){

                 Building_Temp.Braudrate=srcRsTemp->GetCollect(_T("Braudrate"));
                 Building_Temp.Building_Name=srcRsTemp->GetCollect(_T("Building_Name"));
                 Building_Temp.Com_Port=srcRsTemp->GetCollect(_T("Com_Port"));
                 Building_Temp.Default_SubBuilding=srcRsTemp->GetCollect(_T("Default_SubBuilding"));
                 Building_Temp.Ip_Address=srcRsTemp->GetCollect(_T("Ip_Address"));
                 Building_Temp.Ip_Port=srcRsTemp->GetCollect(_T("Ip_Port"));
                 Building_Temp.Main_BuildingName=srcRsTemp->GetCollect(_T("Main_BuildingName"));
                 Building_Temp.Protocal=srcRsTemp->GetCollect(_T("Protocal"));
                 StrTemp=srcRsTemp->GetCollect(_T("Building_Path"));
                 int index= StrTemp.Find(_T("Database"));
                 StrTemp.Delete(0,index);

                 Building_Temp.Building_Path=StrTemp;  
                 m_Building.push_back(Building_Temp);
                 srcRsTemp->MoveNext();
             }
             srcRsTemp->Close();
#endif
         }


         //CustomProductTable
         if (Is_haveTable(_T("CustomProductTable")))
         {
#if 1
             srcRsTemp->Open(_T("select * from CustomProductTable"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);		
             CustomProductTable CustomProductTable_Temp;
             while(!srcRsTemp->EndOfFile){
                 CustomProductTable_Temp.ModelNo=srcRsTemp->GetCollect(_T("ModelNo"));
                 CustomProductTable_Temp.Reg_Description=srcRsTemp->GetCollect(_T("Reg_Description"));
                 CustomProductTable_Temp.Reg_ID=srcRsTemp->GetCollect(_T("Reg_ID"));

                 m_CustomProductTable.push_back(CustomProductTable_Temp);
                 srcRsTemp->MoveNext();
             }
             srcRsTemp->Close();
#endif
         }

         if (srcConTmp->State)
         {srcConTmp->Close();
         }


         CString filePath=g_strExePth+_T("Database\\T3000.mdb");
         DeleteFile(filePath);



         HANDLE hFind;//
         WIN32_FIND_DATA wfd;//
         hFind = FindFirstFile(filePath, &wfd);//
         if (hFind==INVALID_HANDLE_VALUE)//说明当前目录下无t3000.mdb
         {
             //CopyFile(g_strOrigDatabaseFilePath,g_strDatabasefilepath,FALSE);//
             //没有找到就创建一个默认的数据库
             filePath=g_strExePth+_T("Database\\T3000.mdb");
             HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_T3000_DB2), _T("T3000_DB"));   
             HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);   


             LPVOID lpExe = LockResource(hGlobal);   
             CFile file;
             if(file.Open(filePath, CFile::modeCreate | CFile::modeWrite))    
                 file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));    
             file.Close();    
             ::UnlockResource(hGlobal);   
             ::FreeResource(hGlobal);
         }//
         CString strsql;
         srcConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);

         try
         {


             //Building_ALL
#if 1
             srcConTmp->Execute(_T("delete * from Building_ALL"),NULL,adCmdText);	
             srcRsTemp->Open(_T("select * from Building_ALL"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
             try{
                 for (int i=0;i<m_Building_ALL.size();i++)
                 {
                     srcRsTemp->AddNew();
                     srcRsTemp->PutCollect("Building_Name",m_Building_ALL[i].Building_Name);
                     srcRsTemp->PutCollect("Default_Build",m_Building_ALL[i].Default_Building);
                     srcRsTemp->PutCollect("Telephone",m_Building_ALL[i].Telephone);
                     srcRsTemp->PutCollect("Address",m_Building_ALL[i].Address);


                     srcRsTemp->Update();
                 }
             }
             catch(_com_error &e){
                 AfxMessageBox(e.Description());
             }
             srcRsTemp->Close();
#endif

             //Building
#if 1
             srcConTmp->Execute(_T("delete * from Building"),NULL,adCmdText);
             srcRsTemp->Open(_T("select * from Building"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
             try{
                 for (int i=0;i<m_Building.size();i++)
                 {
                     srcRsTemp->AddNew();
                     srcRsTemp->PutCollect("Main_BuildingName",m_Building[i].Main_BuildingName);
                     srcRsTemp->PutCollect("Building_Name",m_Building[i].Main_BuildingName);
                     srcRsTemp->PutCollect("Protocal",m_Building[i].Protocal);
                     srcRsTemp->PutCollect("Com_Port",m_Building[i].Com_Port);
                     srcRsTemp->PutCollect("Ip_Address",m_Building[i].Ip_Address);
                     srcRsTemp->PutCollect("Ip_Port",m_Building[i].Ip_Port);
                     srcRsTemp->PutCollect("Braudrate",m_Building[i].Braudrate);
                     srcRsTemp->PutCollect("Default_SubBuilding",m_Building[i].Default_SubBuilding);
                     srcRsTemp->PutCollect("Building_Path",m_Building[i].Building_Path);
                     srcRsTemp->Update();
                 }
             }
             catch(_com_error &e){
                 AfxMessageBox(e.Description());
             }
             srcRsTemp->Close();
#endif

             //CustomProductTable
#if 1
             srcConTmp->Execute(_T("delete * from CustomProductTable"),NULL,adCmdText);
             srcRsTemp->Open(_T("select * from CustomProductTable"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
             try{
                 for (int i=0;i<m_CustomProductTable.size();i++)
                 {
                     srcRsTemp->AddNew();
                     srcRsTemp->PutCollect("ModelNo",m_CustomProductTable[i].ModelNo);
                     srcRsTemp->PutCollect("Reg_Description",m_CustomProductTable[i].Reg_Description);
                     srcRsTemp->PutCollect("Reg_ID",m_CustomProductTable[i].Reg_ID);
                     srcRsTemp->Update();
                 }
             }
             catch(_com_error &e){
                 AfxMessageBox(e.Description());
             }
             srcRsTemp->Close();

#endif


         }
         catch(_com_error *e)
         {
             AfxMessageBox(e->ErrorMessage());
         }



         if (srcConTmp->State)
         {srcConTmp->Close();
         }
         srcConTmp=NULL;





     }
     else {

         CADO updateADO;
         updateADO.OnInitADOConn();

         if (versionno <=20150415)
         {
             StrSql=_T("ALTER TABLE CustomProductTable ADD COLUMN Para_Type varchar(255),Counts_Number Integer,Property varchar(255)");
             updateADO.m_pConnection->Execute(StrSql.GetBuffer(),NULL,adCmdText);
             StrSql=_T("INSERT INTO Version VALUES(20150605,'更新CustomProductTable')");
             updateADO.m_pConnection->Execute(StrSql.GetBuffer(),NULL,adCmdText);
         }
         /*if (versionno <=20150605)
         {
            StrSql= _T("Insert Table ProductsTypeRegisterTables Values(20,'T3_RegisterList','T3-8I13O','RegID','T3-8I13O')");
         }*/
         updateADO.CloseConn(); 

     }

     int BuildingVersion;
     CBADO bado;
     bado.SetDBPath(g_strCurBuildingDatabasefilePath);
     bado.OnInitADOConn(); 
     bado.m_pRecordset = bado.OpenRecordset(_T("Select * From Version"));
     while(VARIANT_FALSE==bado.m_pRecordset->EndOfFile){
         BuildingVersion = bado.m_pRecordset->GetCollect(_T("VersionNO"));
         bado.m_pRecordset->MoveNext();
     }

     if(BuildingVersion <=20150105){
         StrSql=_T("ALTER TABLE ALL_NODE ADD COLUMN Custom varchar(255)"); 
         bado.m_pConnection->Execute(StrSql.GetBuffer(),NULL,adCmdText);
         StrSql=_T("Update ALL_NODE Set Custom = '0' ");
         bado.m_pConnection->Execute(StrSql.GetBuffer(),NULL,adCmdText);
         StrSql=_T("Create Table BatchFlashResult(SN integer,FirmwarePath varchar(255),ConfigPath varchar(255),FirmwareResult tinyint,ConfigResult tinyint)");
         bado.m_pConnection->Execute(StrSql.GetBuffer(),NULL,adCmdText);
         StrSql=_T("INSERT INTO Version VALUES(20150605,'更新ALL_NODE,创建BatchFlashResult')");
         bado.m_pConnection->Execute(StrSql.GetBuffer(),NULL,adCmdText);
     }
     if (BuildingVersion <=20150605)
     {
         StrSql=_T("ALTER TABLE ALL_NODE ADD COLUMN Parent_SerialNum varchar(255)"); 
         bado.m_pConnection->Execute(StrSql.GetBuffer(),NULL,adCmdText);
         StrSql=_T("Update ALL_NODE Set Parent_SerialNum = '0' ");
         bado.m_pConnection->Execute(StrSql.GetBuffer(),NULL,adCmdText);

         StrSql=_T("INSERT INTO Version VALUES(20150617,'更新ALL_NODE ,添加Parent_SerialNum')");
         bado.m_pConnection->Execute(StrSql.GetBuffer(),NULL,adCmdText);  
     }
     if (BuildingVersion <=20150617)
     {
         StrSql=_T("ALTER TABLE ALL_NODE ADD COLUMN Panal_Number varchar(255)"); 
         bado.m_pConnection->Execute(StrSql.GetBuffer(),NULL,adCmdText);
         StrSql=_T("Update ALL_NODE Set Panal_Number = '0' ");
         bado.m_pConnection->Execute(StrSql.GetBuffer(),NULL,adCmdText);

         StrSql=_T("ALTER TABLE ALL_NODE ADD COLUMN Object_Instance varchar(255)"); 
         bado.m_pConnection->Execute(StrSql.GetBuffer(),NULL,adCmdText);
         StrSql=_T("Update ALL_NODE Set Object_Instance = '0' ");
         bado.m_pConnection->Execute(StrSql.GetBuffer(),NULL,adCmdText);


         StrSql=_T("INSERT INTO Version VALUES(20150623,'更新ALL_NODE ,添加Panal_Number,Object_Instance')");
         bado.m_pConnection->Execute(StrSql.GetBuffer(),NULL,adCmdText);  
     }

     bado.CloseConn();
     return TRUE;
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
BOOL CT3000App::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
	try
	{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
 	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	Create_T3000_log_file();
	

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
	CWinAppEx::InitInstance();
	HRESULT hr;//


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


	

	g_strOrigDatabaseFilePath=g_strExePth+_T("T3000.mdb");//
	g_strDatabasefilepath+=_T("Database\\T3000.mdb");//



	#if 1//如果沒有T3000 的情況下

	CString FilePath;
	HANDLE hFind;//
	WIN32_FIND_DATA wfd;//
	hFind = FindFirstFile(g_strDatabasefilepath, &wfd);//
	if (hFind==INVALID_HANDLE_VALUE)//说明当前目录下无t3000.mdb
	{
		//CopyFile(g_strOrigDatabaseFilePath,g_strDatabasefilepath,FALSE);//
		//没有找到就创建一个默认的数据库
		FilePath=g_strExePth+_T("Database\\T3000.mdb");
		HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_T3000_DB), _T("T3000_DB"));   
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
	if (hFind_Monitor==INVALID_HANDLE_VALUE)//说明当前目录下无MonitorData.mdb
	{
		//没有找到就创建一个默认的数据库
		FilePath_Monitor= g_achive_monitor_datatbase_path;
		HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MONITOR_DB1), _T("MONITOR_DB"));   
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

	int versionno = 0;
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
			versionno=tempado.m_pRecordset->GetCollect(_T("VersionNO"));
			tempado.m_pRecordset->MoveNext();
		}
		tempado.CloseRecordset();


	} 

	if (versionno < 20150922)
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



	
	g_strDatabasefilepath=(CString)FOR_DATABASE_CONNECT+g_strDatabasefilepath;//
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
		filebuildingPath+=_T(".mdb");

		DeleteFile(filebuildingPath);
		// 	 HANDLE hFind;//
		// 	 WIN32_FIND_DATA wfd;//
		//create building db file

		hFind = FindFirstFile(filebuildingPath, &wfd);//
		if (hFind==INVALID_HANDLE_VALUE)//说明当前目录下无t3000.mdb
		{

			HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_BUILDING_DB), _T("BUILDING_DB"));   
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
		CADO ado; 
		ado.OnInitADOConn();

		sql.Format(_T("update Building set Building_Path = '%s'  where  Main_BuildingName = 'Default_Building' "),filebuildingPath);

		 ado.m_pConnection->Execute(sql.GetString(),NULL,adCmdText);

		ado.CloseConn();
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
	 #if 1
	int Ret=JudgeDB();
	if (!Ret)
	{
        FilePath=g_strExePth+_T("Database\\t3000.mdb");
        DeleteFile(FilePath.GetBuffer());
        HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_T3000_DB2), _T("T3000_DB"));   
        HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);   


        LPVOID lpExe = LockResource(hGlobal);   
        CFile file;
        if(file.Open(FilePath, CFile::modeCreate | CFile::modeWrite))    
            file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));    
        file.Close();    
        ::UnlockResource(hGlobal);   
        ::FreeResource(hGlobal);
		JudgeDB();

	}
	 
	#endif


    
	 

	InitModeName();//

 
	 

#endif

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
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
		temp_123 = g_strExePth +  _T("Database\\T3000.mdb");
		BOOL	nret = temp_find.FindFile(temp_123);
		if(nret)
		{
			DeleteFile(temp_123);
		}
		AfxMessageBox(_T("Database operation to stop!"));
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
////////////////////////////////////////////////////////
	_ConnectionPtr m_pCon;
	_RecordsetPtr m_pRs;
	m_pCon.CreateInstance(_T("ADODB.Connection"));
	hr=m_pRs.CreateInstance(_T("ADODB.Recordset"));
	if(FAILED(hr)) 	
	{
	    AfxMessageBox(_T("Load msado12.dll erro"));
	      return FALSE;
	}
	m_pCon->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
	m_pRs->Open(_T("select * from Userlogin"),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);		
	int nRecord=m_pRs->GetRecordCount();
	if (nRecord<=0)
	{
		g_bPrivilegeMannage=FALSE;
	}
	else
	{
		int nUse;
		_variant_t temp_variant;
		temp_variant=m_pRs->GetCollect("USE_PASSWORD");//
		if(temp_variant.vt!=VT_NULL)
			nUse=temp_variant;
		else
			nUse=0;
		if(nUse==-1)
		{
			g_bPrivilegeMannage=TRUE;
		}
		else
		{
			g_bPrivilegeMannage=FALSE;
		}
	}
	m_pRs->Close();
	m_pCon->Close();



	if (g_bPrivilegeMannage)
	{//for just quick debug,only on this computer
		if(!user_login())
		{
			AfxMessageBox(_T("Error password!"));	
			return false;
		}
		
	}

#endif

  	((CMainFrame*)m_pMainWnd)->InitViews();//
    CString strTile;
    strTile.Format(_T("T3000 Building Automation System"));
	strTile+=CurrentT3000Version;
   	m_pMainWnd->SetWindowText(strTile);//
  	m_pMainWnd->ShowWindow(SW_SHOW);
  	m_pMainWnd->UpdateWindow();
   // ((CMainFrame*)m_pMainWnd)->SwitchToPruductType(DLG_DIALOG_DEFAULT_BUILDING); 

	m_szAppPath  = g_strExePth;
	m_szHelpFile = theApp.m_szAppPath + L"T3000_Help.chm";
	CString g_configfile_path =g_strExePth + g_strStartInterface_config;
	m_lastinterface=GetPrivateProfileInt(_T("T3000_START"),_T("Interface"),19,g_configfile_path);
	g_selected_serialnumber=GetPrivateProfileInt(_T("T3000_START"),_T("SerialNumber"),0,g_configfile_path);
    if (m_lastinterface!=19&&m_lastinterface!=24)
    {
    ((CMainFrame*)m_pMainWnd)->SwitchToPruductType(m_lastinterface);
    }
    else
    {
        ((CMainFrame*)m_pMainWnd)->SwitchToPruductType(19);
    }

	
	}
	catch (...)
	{
		CFileFind temp_find;
		CString temp_123;
		temp_123 = g_strExePth +  _T("Database\\T3000.mdb");
		BOOL	nret = temp_find.FindFile(temp_123);
		if(nret)
		{
			DeleteFile(temp_123);
		}
	//	AfxMessageBox(_T("Double click 'REG_msado15.dll',Please!\nAt C:\\Program Files\\Temcocontrols\\T3000"));

// 		CString strFilter = _T("hex File;bin File|*.hex;*.bin|all File|*.*||");
// 		CFileDialog dlg(true,_T("hex"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
// 		dlg.DoModal();

//		CFileDialog dlg(true,NULL,_T("C:\Program Files\Common Files\System\ado"));
		//dlg.lpstrInitialDir = "..\\hisdata";
		//dlg.op
////
	//		OPENFILENAME

		
//		if (dlg.DoModal()==IDOK)
//		{
// 			path = dlg.GetPathName();
// 			pLogFile = fopen("Log.txt", "wt+");   
// 			fprintf(pLogFile, "%s", (LPCSTR)path); 
// 			fclose(pLogFile);
// 			pLogFile = NULL; 
//		}


// 		CFileDialog fileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
// 			NULL,NULL);//_T("工作表(*.xls)|*.xls|文本文件(*.txt)|*.txt||")
// 		fileDlg.m_ofn.lpstrInitialDir = _T("C:\\Program Files\\Temcocontrols\\T3000\\REG_msado15.dll.bat");
// 		fileDlg.DoModal();



		//::ShellExecute(NULL, _T("open"), _T("C:\\Program Files\\Temcocontrols\\T3000\\REG_msado15.dll.bat"), _T(""), _T(""), SW_SHOW);
		//vcredist_x86.zip
		
	//	::ShellExecute(NULL, _T("open"), _T("C:\\Program Files\\Temcocontrols\\T3000\\vcredist_x86.zip"), _T(""), _T(""), SW_SHOW);
		//这个要先试试，当电脑没有安装这个文件时，如何捕获这个信息，然后再执行这个。
       // http://www.temcocontrols.com/ftp/software/AccessDatabaseEngine.zip
       CString str_msado;
       str_msado.Format(_T("%sREG_MSFLXGRD.bat"),g_strExePth.GetBuffer());
        ::ShellExecute(NULL, _T("open"),str_msado.GetBuffer(), _T(""), _T(""), SW_SHOW);
        //vcredist_x86.zip

        //	::ShellExecute(NULL, _T("open"), _T("C:\\Program Files\\Temcocontrols\\T3000\\vcredist_x86.zip"), _T(""), _T(""), SW_SHOW);
        //这个要先试试，当电脑没有安装这个文件时，如何捕获这个信息，然后再执行这个。
        

       AfxMessageBox(_T("Database error! Please restart again ."));
       ::ShellExecute(NULL, _T("open"), _T("www.temcocontrols.com/ftp/software/AccessDatabaseEngine.zip"), _T(""), _T(""), SW_SHOW);
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
	// TODO: Add your specialized code here and/or call the base class
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

CString CT3000App::GetModulePath()
{
	wchar_t path[MAX_PATH];
	GetModuleFileName(NULL,path,MAX_PATH);
	return CString(path);
}

int CT3000App::GetSoftInstallDays()
{
	CFileStatus fileStatus;
	CTime curTime;
	CString strPath;
	CTimeSpan timeSpan;

	curTime=CTime::GetCurrentTime();
	strPath=GetModulePath();

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

void CT3000App::SetLanguage(DWORD Last){
	CRegKey key;
	LPCTSTR data_Set = _T("Software\\Microsoft\\Windows NT\\CurrentVersion");//_T("Software\\Microsoft\\");//
	key.Open(HKEY_LOCAL_MACHINE,data_Set);
	WriteNumber(key,_T("T3000"),Last);
}

void CAboutDlg::OnBnClickedOk()
{
// TODO: Add your control notification handler code here
CDialog::OnOK();
}
