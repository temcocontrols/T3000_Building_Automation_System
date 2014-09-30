
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
const int g_versionNO=20140926;
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
//	try
///	{
		m_bHiColorIcons = TRUE;
//	}
// 	catch (...)
// 	{
// 		
// 		AfxMessageBox(_T("1111"));
// 	}


	
}
// The one and only CT3000App object
CT3000App theApp;


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
  CADO ado;
  ado.OnInitADOConn();
  CString sql=_T("select * from ALL_NODE");
  ado.m_pRecordset=ado.OpenRecordset(sql);
  ALL_NODE temp;
  while(!ado.m_pRecordset->EndOfFile){
	   temp.MainBuilding_Name=ado.m_pRecordset->GetCollect(_T("MainBuilding_Name"));
	   temp.Building_Name=ado.m_pRecordset->GetCollect(_T("Building_Name"));
	   temp.Serial_ID=ado.m_pRecordset->GetCollect(_T("Serial_ID"));
	   temp.Floor_name=ado.m_pRecordset->GetCollect(_T("Floor_name"));
	   temp.Room_name=ado.m_pRecordset->GetCollect(_T("Room_name"));
	   temp.Product_name=ado.m_pRecordset->GetCollect(_T("Product_name"));
	   temp.Product_class_ID=ado.m_pRecordset->GetCollect(_T("Product_class_ID"));
	   temp.Product_ID=ado.m_pRecordset->GetCollect(_T("Product_ID"));
	   temp.Screen_Name=ado.m_pRecordset->GetCollect(_T("Screen_Name"));
	   temp.Bautrate=ado.m_pRecordset->GetCollect(_T("Bautrate"));
	   temp.Background_imgID=ado.m_pRecordset->GetCollect(_T("Background_imgID"));
	   temp.Hardware_Ver=ado.m_pRecordset->GetCollect(_T("Hardware_Ver"));
	   temp.Software_Ver=ado.m_pRecordset->GetCollect(_T("Software_Ver"));
	   temp.Com_Port=ado.m_pRecordset->GetCollect(_T("Com_Port"));
	   temp.EPsize=ado.m_pRecordset->GetCollect(_T("EPsize"));
	   temp.Protocol=ado.m_pRecordset->GetCollect(_T("Protocol"));
      
	   ado.m_pRecordset->MoveNext();

	   m_AllNodes.push_back(temp);
  }
  ado.CloseRecordset();
  ado.CloseConn();
 }



BOOL CT3000App::JudgeDB(){
 CADO ado;
 int versionno=0;
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
  ado.CloseConn();

 if (g_versionNO>versionno)//版本过低
 {
     //SetPaneString(0,_T("The version of DB is lower,Updating....."));
	 _variant_t temp_var;
	 _ConnectionPtr srcConTmp;
	 _RecordsetPtr srcRsTemp;
	 srcConTmp.CreateInstance("ADODB.Connection");
	 srcRsTemp.CreateInstance("ADODB.Recordset");
	 srcConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);//打开数据库
	//这里是All_NODE
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

	//Building
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

	 //CustomProductTable
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
	 //IONAME
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
	 //IONAME_Config
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
	 //LCNameConfig
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
	 //LightingController_Name
#if 1
	 srcRsTemp->Open(_T("select * from LightingController_Name"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);		
	 LightingController_Name LightingController_Name_Temp;
	 while(!srcRsTemp->EndOfFile){
	 LightingController_Name_Temp.Address=srcRsTemp->GetCollect(_T("Address"));
	 LightingController_Name_Temp.ID_No=srcRsTemp->GetCollect(_T("ID_No"));
	 LightingController_Name_Temp.OutputName=srcRsTemp->GetCollect(_T("OutputName"));
	 LightingController_Name_Temp.Status=srcRsTemp->GetCollect(_T("Status"));
	 LightingController_Name_Temp.Type=srcRsTemp->GetCollect(_T("Type"));

		 
		 m_LightingController_Name.push_back(LightingController_Name_Temp);
		 srcRsTemp->MoveNext();
	 }
	 srcRsTemp->Close();
#endif
	 //Product_Data
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
	 //Screen_Label
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
 
	 //Value_Range
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
	 //ALL_NODE
	 #if 1
		 for (int i=0;i<m_AllNodes.size();i++)
		 {
			 strsql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize) values('"
				 +m_AllNodes[i].MainBuilding_Name+"','"
				 +m_AllNodes[i].Building_Name+"','"
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
				 +m_AllNodes[i].EPsize+"')"));


			 srcConTmp->Execute(strsql.GetString(),NULL,adCmdText);	
		 }
     #endif

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
			     srcRsTemp->PutCollect("Building_Name",m_Building[i].Building_Name);
				 srcRsTemp->PutCollect("Protocal",m_Building[i].Protocal);
				 srcRsTemp->PutCollect("Com_Port",m_Building[i].Com_Port);
				 srcRsTemp->PutCollect("Ip_Address",m_Building[i].Ip_Address);
				 srcRsTemp->PutCollect("Ip_Port",m_Building[i].Ip_Port);
				 srcRsTemp->PutCollect("Braudrate",m_Building[i].Braudrate);
				 srcRsTemp->PutCollect("Default_SubBuilding",m_Building[i].Default_SubBuilding);
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
		 //IONAME
#if 1
srcConTmp->Execute(_T("delete * from IONAME"),NULL,adCmdText);
		 srcRsTemp->Open(_T("select * from IONAME"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
		 try{
			 for (int i=0;i<m_IONAME.size();i++)
			 {
				 srcRsTemp->AddNew();
				 srcRsTemp->PutCollect("SERIAL_ID",m_IONAME[i].SERIAL_ID);
				 srcRsTemp->PutCollect("INPUT1",m_IONAME[i].INPUT1);
				 srcRsTemp->PutCollect("INPUT2",m_IONAME[i].INPUT2);
				 srcRsTemp->PutCollect("INPUT3",m_IONAME[i].INPUT3);
				 srcRsTemp->PutCollect("INPUT4",m_IONAME[i].INPUT4);
				 srcRsTemp->PutCollect("INPUT5",m_IONAME[i].INPUT5);
				 srcRsTemp->PutCollect("INPUT6",m_IONAME[i].INPUT6);
				 srcRsTemp->PutCollect("INPUT7",m_IONAME[i].INPUT7);
				 srcRsTemp->PutCollect("INPUT8",m_IONAME[i].INPUT8);
				 srcRsTemp->PutCollect("INPUT9",m_IONAME[i].INPUT9);
				 srcRsTemp->PutCollect("OUTPUT1",m_IONAME[i].OUTPUT1);
				 srcRsTemp->PutCollect("OUTPUT2",m_IONAME[i].OUTPUT2);
				 srcRsTemp->PutCollect("OUTPUT3",m_IONAME[i].OUTPUT3);
				 srcRsTemp->PutCollect("OUTPUT4",m_IONAME[i].OUTPUT4);
				 srcRsTemp->PutCollect("OUTPUT5",m_IONAME[i].OUTPUT5);
				 srcRsTemp->PutCollect("OUTPUT6",m_IONAME[i].OUTPUT6);
				 srcRsTemp->PutCollect("OUTPUT7",m_IONAME[i].OUTPUT7);
				 srcRsTemp->PutCollect("SENSORNAME",m_IONAME[i].SENSORNAME);
				 
				 srcRsTemp->Update();
			 }
		 }
		 catch(_com_error &e){
			 AfxMessageBox(e.Description());
		 }
		 srcRsTemp->Close();
#endif
		 //IONAME_Config
#if 1
srcConTmp->Execute(_T("delete * from IONAME_CONFIG"),NULL,adCmdText);
		 srcRsTemp->Open(_T("select * from IONAME_CONFIG"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
		 try{
			 for (int i=0;i<m_IONAME_Config.size();i++)
			 {
				 srcRsTemp->AddNew();
				 srcRsTemp->PutCollect("InOutName",m_IONAME_Config[i].InOutName);
				 srcRsTemp->PutCollect("Type",m_IONAME_Config[i].Type);
				 srcRsTemp->PutCollect("Row",m_IONAME_Config[i].Row);
				 srcRsTemp->PutCollect("SerialNo",m_IONAME_Config[i].SerialNo);
				 srcRsTemp->Update();
			 }
		 }
		 catch(_com_error &e){
			 AfxMessageBox(e.Description());
		 }
		 srcRsTemp->Close();
#endif
		 //LCNameConfig
#if 1
srcConTmp->Execute(_T("delete * from LCNameConfigure"),NULL,adCmdText);
		 srcRsTemp->Open(_T("select * from LCNameConfigure"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
		 try{
			 for (int i=0;i<m_LCNameConfigure.size();i++)
			 {
				 srcRsTemp->AddNew();
				 srcRsTemp->PutCollect("SN",m_LCNameConfigure[i].SN);
				 srcRsTemp->PutCollect("Card",m_LCNameConfigure[i].Card);
				 srcRsTemp->PutCollect("Output",m_LCNameConfigure[i].Output);
				 srcRsTemp->PutCollect("OutputName",m_LCNameConfigure[i].OutputName);
				 srcRsTemp->Update();
			 }
		 }
		 catch(_com_error &e){
			 AfxMessageBox(e.Description());
		 }
		 srcRsTemp->Close();
#endif
		 //LightingController_Name
#if 1
srcConTmp->Execute(_T("delete * from LightingController_Name"),NULL,adCmdText);
		 srcRsTemp->Open(_T("select * from LightingController_Name"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
		 try{
			 for (int i=0;i<m_LightingController_Name.size();i++)
			 {
				 srcRsTemp->AddNew();
				 srcRsTemp->PutCollect("Address",m_LightingController_Name[i].Address);
				 srcRsTemp->PutCollect("Type",m_LightingController_Name[i].Type);
				 srcRsTemp->PutCollect("ID_No",m_LightingController_Name[i].ID_No);
				 srcRsTemp->PutCollect("OutputName",m_LightingController_Name[i].OutputName);
				 srcRsTemp->PutCollect("Status",m_LightingController_Name[i].Status);
				 srcRsTemp->Update();
			 }
		 }
		 catch(_com_error &e){
			 AfxMessageBox(e.Description());
		 }
		 srcRsTemp->Close();

#endif
		 //Product_Data
#if 1
srcConTmp->Execute(_T("delete * from Product_Data"),NULL,adCmdText);
		 srcRsTemp->Open(_T("select * from Product_Data"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
		 try{
			 for (int i=0;i<m_Product_Data.size();i++)
			 {
				 srcRsTemp->AddNew();
				 srcRsTemp->PutCollect("Serial_ID",m_Product_Data[i].Serial_ID);
				 srcRsTemp->PutCollect("Register_Data",m_Product_Data[i].Register_Data);
				 
				 srcRsTemp->Update();
			 }
		 }
		 catch(_com_error &e){
			 AfxMessageBox(e.Description());
		 }
		 srcRsTemp->Close();
#endif
		 //Screen_Label
#if 1
srcConTmp->Execute(_T("delete * from Screen_Label"),NULL,adCmdText);
		 srcRsTemp->Open(_T("select * from Screen_Label"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
		 try{
			 for (int i=0;i<m_Screen_Label.size();i++)
			 {
				 srcRsTemp->AddNew();
				 srcRsTemp->PutCollect("Cstatic_id",m_Screen_Label[i].Cstatic_id);
				 srcRsTemp->PutCollect("Serial_Num",m_Screen_Label[i].Serial_Num);
				 srcRsTemp->PutCollect("Point_X",m_Screen_Label[i].Point_X);
				 srcRsTemp->PutCollect("Point_Y",m_Screen_Label[i].Point_Y);
				 srcRsTemp->PutCollect("Height",m_Screen_Label[i].Height);
				 srcRsTemp->PutCollect("Width",m_Screen_Label[i].Width);
				 srcRsTemp->PutCollect("Tstat_id",m_Screen_Label[i].Tstat_id);
				 srcRsTemp->PutCollect("Status",m_Screen_Label[i].Status);
				 srcRsTemp->PutCollect("Tips",m_Screen_Label[i].Tips);
				 srcRsTemp->PutCollect("Input_or_Output",m_Screen_Label[i].Input_or_Output);
				 srcRsTemp->PutCollect("Text_Color",m_Screen_Label[i].Text_Color);
				 srcRsTemp->PutCollect("Back_Color",m_Screen_Label[i].Back_Color);
				 srcRsTemp->Update();
			 }
		 }
		 catch(_com_error &e){
			 AfxMessageBox(e.Description());
		 }
		 srcRsTemp->Close();
#endif

	 //Value_Range
	 #if 1
	 srcConTmp->Execute(_T("delete * from Value_Range"),NULL,adCmdText);
	   srcRsTemp->Open(_T("select * from Value_Range"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
	   try{
	   for (int i=0;i<m_Value_Range.size();i++)
	   {
		   srcRsTemp->AddNew();
		   srcRsTemp->PutCollect("SN",m_Value_Range[i].SN);
		   srcRsTemp->PutCollect("CInputNo",m_Value_Range[i].CInputNo);
		   srcRsTemp->PutCollect("CRange",m_Value_Range[i].CRange);
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


 return TRUE;

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
#if 1	
	try
	{

	TCHAR exeFullPath[MAX_PATH+1]; //
	GetModuleFileName(NULL, exeFullPath, MAX_PATH); //
	(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;//
	g_strDatabasefilepath=exeFullPath;//
	g_strExePth=g_strDatabasefilepath;//
	CreateDirectory(g_strExePth+_T("Database"),NULL);//creat database folder;//
	g_strOrigDatabaseFilePath=g_strExePth+_T("T3000.mdb");//
	g_strDatabasefilepath+=_T("Database\\T3000.mdb");//

	CString FilePath;
	HANDLE hFind;//
	WIN32_FIND_DATA wfd;//
	hFind = FindFirstFile(g_strDatabasefilepath, &wfd);//
	if (hFind==INVALID_HANDLE_VALUE)//说明当前目录下无t3000.mdb
	{
		//CopyFile(g_strOrigDatabaseFilePath,g_strDatabasefilepath,FALSE);//
		  //没有找到就创建一个默认的数据库
		FilePath=g_strExePth+_T("Database\\T3000.mdb");
		HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_T3000_DB2), _T("T3000_DB"));   
		HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);   


		LPVOID lpExe = LockResource(hGlobal);   
		CFile file;
		if(file.Open(FilePath, CFile::modeCreate | CFile::modeWrite))    
			file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));    
		file.Close();    
		::UnlockResource(hGlobal);   
		::FreeResource(hGlobal);
	}//
	
	 
	FindClose(hFind);//
	g_strDatabasefilepath=(CString)FOR_DATABASE_CONNECT+g_strDatabasefilepath;//
	g_strImgeFolder=g_strExePth+_T("Database\\image\\");//
	CreateDirectory(g_strImgeFolder,NULL);//

  
   BOOL Ret=JudgeDB();
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
  
	//CString strocx=g_strExePth+_T("MSFLXGRD.OCX");
	
	/*CStdioFile file;
	CString versionno;
	file.
	file.SetFilePath(_T("http://www.temcocontrols.com/ftp/software/T3000_Version.txt"));
	file.ReadString(versionno);
	file.Close();*/
	//CFile file;
//	file.Open(_T("http://www.temcocontrols.com/ftp/software/T3000_Version.txt"),modeRead);

 	InitModeName();//

	#if 0
	CInternetSession session;
	CInternetFile *file=NULL;
	try
	{
		INTERNET_PROXY_INFO  proxyinfo;
		proxyinfo.dwAccessType=INTERNET_OPEN_TYPE_PROXY;
		proxyinfo.lpszProxy=_T("192.168.0.4:8080 ");
		proxyinfo.lpszProxyBypass=NULL; 
		if (!session.SetOption(INTERNET_OPTION_PROXY,(LPVOID)&proxyinfo,sizeof(INTERNET_PROXY_INFO)))
		{
			AfxMessageBox(_T("UserName"));
		} 
		CString username=_T("alex");
		if(!session.SetOption(INTERNET_OPTION_PROXY_USERNAME,username.GetBuffer(),username.GetLength()+ 1)){

			AfxMessageBox(_T("UserName"));
		}
		CString password=_T("travel");
		if(!session.SetOption(INTERNET_OPTION_PROXY_PASSWORD,password.GetBuffer(),password.GetLength()+ 1)){

			AfxMessageBox(_T("Password"));
		}
		file=(CInternetFile*)session.OpenURL(_T("www.temcocontrols.com/ftp/software/T3000_Version.txt"));

	}
	catch (CInternetException* e)
	{
		file=NULL;
		e->Delete();

	}
	CString version;

	if (file)
	{
		while(file->ReadString(version)!=NULL){

		}
		AfxMessageBox(version);
	}
   #endif
	 

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
		AfxMessageBox(_T("Database operation to stop!"));

	}
#ifndef Fance_Enable_Test
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CT3000Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CT3000View));
#endif

#ifdef Fance_Enable_Test
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CT3000Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CDialogCM5_BacNet));
#endif




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
    strTile.Format(_T("T3000 Building Automation System Rev2014.9.29"));
   	m_pMainWnd->SetWindowText(strTile);//
  	m_pMainWnd->ShowWindow(SW_SHOW);
  	m_pMainWnd->UpdateWindow();
    //m_pMainWnd->SwitchToPruductType(18);  
	//((CMainFrame*)m_pMainWnd)->SwitchToPruductType(18);
	}
	catch (...)
	{
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
        

       AfxMessageBox(_T("Please install the database engine!"));
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
