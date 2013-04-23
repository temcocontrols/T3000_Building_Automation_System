// ImportDatabaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "ImportDatabaseDlg.h"

/*
l_pRecordSet是记录集指针   
  l_pConnection是数据库连接   
  l_pRecordset   =   l_pConnection->OpenSchema(adSchemaTables);   
  _variant_t   l_vDBTableName;   
  _bstr_t   bstrTableType;         
          while(!l_pRecordset->adoEOF)   
          {   
                  l_vDBTableName   =   l_pRecordset->GetCollect("TABLE_NAME");     
                  bstrTableType   =   l_pRecordset->GetCollect("TABLE_TYPE");   
                  if   ((bstrTableType   ==   (_bstr_t)"TABLE")||(bstrTableType   ==   (_bstr_t)"VIEWS"))   
  {   
  l_arrTableNames.Add((char   *)_bstr_t(l_vDBTableName));   
  }   
  l_pRecordset->MoveNext();   
          }   
          int   l_nTableCount   =   l_arrTableNames.GetSize();   
  如果l_nTableCount   ==   0   ，就说明没有表   
*/
// CImportDatabaseDlg dialog

struct _building
{
	CString strSrcbuildingName;
	CString strSrcProtocol;
	CString strSrIpAdress;
	CString strSrcIpPort;
	CString strSrcComPort;
	CString m_strBaudrate;
	CString strSrcTelephone;
	CString strSrcAddress;
	BOOL bSrcDefaut;
};
struct _BuildingNodes
{
	CString strBuildingName;
	CString strFloorName;
	CString strRoomName;
	CString strProudctName;
	int nProductClassID;
	int ProductID;
	CString strScreenName;
	UINT nSerialID;
	int nBaudrate;
	CString strImgName;
	int HWVersion;
	int SWVersion;
	int nEPsize;
};
struct pic_label///////////////////////////////////////////////////////////////////////////////
{
	int cstatic_id;//the id of the CStatic control button
	int input_or_output;//true == output ,false == input
	bool picture_or_text;//true == a picture;false == a text
	CString str;//path picture file path or text 
	CString the_value;//the value of select
	int width;//the cstatic width;
	int height;//the cstatic height
	CPoint point;//the rect left/top point
	int tstat_id;//the id of tstat	
	int status;//for input or output selection,such as Relay 1 ,Relay 2,Relay 3.
	COLORREF clrTxt;
	COLORREF bkColor;
};///////////////////////////////////////////////////////////////////////////////

//new version/////////////////////////
struct _Label_information///////////////////////////////////////////////////////////////////////////////
{
	int cstatic_id;//the id of the CStatic control button
	int input_or_output;//true == output ,false == input
	int nSerialNum;
	int tstat_id;//the id of tstat	
//	bool picture_or_text;//true == a picture;false == a text
//	CString str;//path picture file path or text 
	CString the_value;//the value of select
	int width;//the cstatic width;
	int height;//the cstatic height
	CPoint point;//the rect left/top point
	int status;//for input or output selection,such as Relay 1 ,Relay 2,Relay 3...
	COLORREF clrTxt;
	COLORREF bkColor;
	CString strTips;//indcatcor
	CString strScreenName;
	//CRelayLabel* plabelCtrl;
};///////////////////////////////////////////////////////////////////////////////

struct _MainBuilding
{
	CString strMainBuild;
	CString strTelephone;
	CString strAdress;
};
struct _SubBuilding
{
	CString strmainBuilding;
	CString strbuildingName;
	CString strProtocol;
	CString strIpAdress;
	CString strIpPort;
	CString strComPort;
	CString m_strBaudrate;
	//BOOL bSrcDefaut;
};
struct _TNodes
{
	CString strMainBuildingName;
	CString strSubBuildingName;
	CString strSerialID;

	CString strFloorName;
	CString strRoomName;
	CString strProudctName;
	CString strProductClassID;
	CString strProductID;
	CString strScreenName;

	CString strBaudrate;
	CString strImgName;
	CString strHWVersion;
	CString strSWVersion;
	CString strEPsize;

	CString strComport;
};


struct _IONameNodes
{	
	CString strSerialID;
	CString strSensorName;
	CString szInputName[8];
	CString szOutputName[7];

};

//extern struct Label_information;	//warning C4091: 'extern ' : ignored on left of 'Label_information' when no variable is declared

IMPLEMENT_DYNAMIC(CImportDatabaseDlg, CDialog)

CImportDatabaseDlg::CImportDatabaseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImportDatabaseDlg::IDD, pParent)
{
	m_bfromLower=TRUE;
	m_strSourceDbFileName.Empty();
}

CImportDatabaseDlg::~CImportDatabaseDlg()
{

}

void CImportDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_sourceDbEditCtrl);
	//DDX_Control(pDX, IDC_RADIO1, m_lowerBtn);
	//DDX_Control(pDX, IDC_RADIO2, m_upperBtn);
}


BEGIN_MESSAGE_MAP(CImportDatabaseDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CImportDatabaseDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BROWSE_BUTTON,OnClickedBrowsBtn)
	ON_BN_CLICKED(IDC_RADIO1, &CImportDatabaseDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CImportDatabaseDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()
// CImportDatabaseDlg message handlers

void CImportDatabaseDlg::OnBnClickedOk()
{
	BeginWaitCursor();
	if(m_strSourceDbFileName.IsEmpty())
	{
		AfxMessageBox(_T("Please select a T3000 database file you want to import to your system!"));
		return;
	}
	if(m_strSourceDbFileName.CompareNoCase(g_strOrigDatabaseFilePath)==0)
	{
		AfxMessageBox(_T("This is your T3000 database file,please select another one!"));
		return;
	}
	_ConnectionPtr srcConTmp;
	_RecordsetPtr srcRsTemp;
	srcConTmp.CreateInstance("ADODB.Connection");
	srcRsTemp.CreateInstance("ADODB.Recordset");

	CString strSourceDatabasefilepath=(CString)FOR_DATABASE_CONNECT+m_strSourceDbFileName;
	srcConTmp->Open(strSourceDatabasefilepath.GetString(),"","",adModeUnknown);

	srcRsTemp	=srcConTmp->OpenSchema(adSchemaTables);
	_variant_t	l_vDBTableName;
	_bstr_t	bstrTableType;
	vector<CString> tabNameVector;
	while(!srcRsTemp->EndOfFile)   
   {   
       l_vDBTableName = srcRsTemp->GetCollect("TABLE_NAME"); 
       bstrTableType  =srcRsTemp->GetCollect("TABLE_TYPE");   
       if((bstrTableType ==(_bstr_t)"TABLE")||(bstrTableType==(_bstr_t)("VIEWS")))  
	   {   
		  tabNameVector.push_back(l_vDBTableName);   
		}   
		 srcRsTemp->MoveNext();   
    }   
  	if(srcRsTemp->State)
			srcRsTemp->Close();
	m_bfromLower=FALSE;

	for(UINT i=0;i<tabNameVector.size();i++)
	{
		CString strtemp=tabNameVector.at(i);
		if(strtemp.CompareNoCase(_T("Every_product"))==0)
		{
			m_bfromLower=TRUE;
			break;
		}
	}
	if(m_bfromLower)//import data from version lower than ver2.5.0.0
	{
		
		srcRsTemp->Open(_T("select * from Building"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);		
		int nRecord=srcRsTemp->GetRecordCount();
		if (nRecord<=0)
		{
			AfxMessageBox(_T("There is no building data in the database file you select!"));
			return;
		}
		vector<_building> srcBuildingVector;//as the sub building name in dest database.
		_variant_t temp_variant;
		while(VARIANT_FALSE==srcRsTemp->EndOfFile)
		{
			_building InfoBuild;
			InfoBuild.strSrcbuildingName=srcRsTemp->GetCollect("Building_Name");//
			InfoBuild.strSrcProtocol=srcRsTemp->GetCollect("Protocal");//
			temp_variant=srcRsTemp->GetCollect("Ip_Address");
			if(temp_variant.vt!=VT_NULL)
				InfoBuild.strSrIpAdress=temp_variant;
			else
				InfoBuild.strSrIpAdress=_T("");

			temp_variant=srcRsTemp->GetCollect("Ip_Port");
			if(temp_variant.vt!=VT_NULL)
				InfoBuild.strSrcIpPort=temp_variant;
			else
				InfoBuild.strSrcIpPort=_T("");

			temp_variant=srcRsTemp->GetCollect("Com_Port");
			if(temp_variant.vt!=VT_NULL)
				InfoBuild.strSrcComPort=temp_variant;
			else
				InfoBuild.strSrcComPort=_T("");

			//Baudrate
			temp_variant=srcRsTemp->GetCollect("Braudrate");
			if(temp_variant.vt!=VT_NULL)
				InfoBuild.m_strBaudrate=temp_variant;
			else
				InfoBuild.m_strBaudrate=_T("");

			temp_variant=srcRsTemp->GetCollect("Telephone");//
			if(temp_variant.vt!=VT_NULL)
				InfoBuild.strSrcTelephone=temp_variant;
			else
				InfoBuild.strSrcTelephone=_T("");

			temp_variant=srcRsTemp->GetCollect("Address");//
			if(temp_variant.vt!=VT_NULL)
				InfoBuild.strSrcAddress=temp_variant;
			else
				InfoBuild.strSrcAddress=_T("");
			InfoBuild.bSrcDefaut=srcRsTemp->GetCollect("Default_Build");//
			srcBuildingVector.push_back(InfoBuild);
			srcRsTemp->MoveNext();//
		}
		if(srcRsTemp->State)
			srcRsTemp->Close();
		//check source mainbuilding 
/*
		m_pRs->Open(_T("select * from Building_ALL"),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);		
		vector<CString> MainBuildNameVector;
		CString strTemp;
		BOOL bFind=FALSE;
		while(VARIANT_FALSE==m_pRs->EndOfFile)
		{
			temp_variant=m_pRs->GetCollect("Building_Name");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			if(!strTemp.IsEmpty())
				MainBuildNameVector.push_back(strTemp);
			if(strTemp.CompareNoCase(_T("Main Building"))==0)
			{
				bFind=TRUE;
				break;

			}
			m_pRs->MoveNext();
				
		}
		*/
		/*
		CString strSql;
		CString strMainBuilding=_T("Main Building");;
		if(MainBuildNameVector.size()==0||!bFind)//
		{
			strMainBuilding=_T("Main Building");
			BOOL bDefault=0;
			CString strTelphone=_T("");
			CString strAdress=_T("");
			//strSql.Format(_T("insert into Building_ALL (Building_Name,Default_Build,Telephone,Address) values('"+strMainBuilding+"',bDefault,'"+strTelphone+"','"+strAdress+"')"));
			strSql.Format(_T("insert into Building_ALL (Building_Name,Telephone ,Address) values('"+strMainBuilding+"','"+strTelphone+"','"+strAdress+"')"));

			m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
		}
		//if(m_pRs->State)
		//	m_pRs->Close();
		*/
		CString strMainBuilding=_T("Main Building");
		CString strTemp;
		BOOL bFind=FALSE;
		CString strSql;

		vector<CString> SubBuildNameVector;
		m_pRs->Open(_T("select * from Building"),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);	

		while(VARIANT_FALSE==m_pRs->EndOfFile)
		{
			temp_variant=m_pRs->GetCollect("Building_Name");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			if(!strTemp.IsEmpty())
				SubBuildNameVector.push_back(strTemp);
			m_pRs->MoveNext();
		}
		if(m_pRs->State)
			m_pRs->Close();
	
		CString strbuildingName;
		CString strProtocol;
		CString strIpAdress;
		CString strIpPort;
		CString strComPort;
		CString strBaudrate;
		CString strTelephone;
		for(UINT i=0;i<srcBuildingVector.size();i++)
		{
			strbuildingName=srcBuildingVector.at(i).strSrcbuildingName;
			strProtocol=srcBuildingVector.at(i).strSrcProtocol;
			strIpAdress=srcBuildingVector.at(i).strSrIpAdress;
			strIpPort=srcBuildingVector.at(i).strSrcIpPort;
			strComPort=srcBuildingVector.at(i).strSrcComPort;
			bFind=FALSE;
			for(UINT k=0;k<SubBuildNameVector.size();k++)
			{
				if(strbuildingName.CompareNoCase(SubBuildNameVector.at(k))==0)
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind)
			{
				try
				{


				strSql.Format(_T("update Building set Main_BuildingName='%s', Building_Name='%s',Protocal='%s',Ip_Address='%s',Ip_Port='%s',Com_Port='%s',Braudrate='%s' where Building_Name='%s'"),strMainBuilding,strbuildingName,strProtocol,strProtocol,strIpPort,strComPort,strBaudrate,strbuildingName);
				m_pCon->Execute(strSql.GetString(),NULL,adCmdText);		
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}
			}
			else
			{	
				try
			{


				//////insert to sub building ,"Main Building" as the name of the building.
				strSql.Format(_T("insert into Building (Main_BuildingName,Building_Name,Protocal,Com_Port,Ip_Address,Ip_Port,Braudrate) values('"+strMainBuilding+"','"+strbuildingName+"','"+strProtocol+"','"+strComPort+"','"+strIpAdress+"','"+strIpPort+"','"+strBaudrate+"')"));
				m_pCon->Execute(strSql.GetString(),NULL,adCmdText);		
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}
			}

		}
		if(srcRsTemp->State)
			srcRsTemp->Close();
		srcRsTemp->Open(_T("select * from Every_product"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);		
		nRecord=srcRsTemp->GetRecordCount();
		
		vector<_BuildingNodes> nodesVector;
		_BuildingNodes buildingNode;
		if (nRecord>0)//find Tstats info insource database
		{
			while(VARIANT_FALSE==srcRsTemp->EndOfFile)
			{	
				buildingNode.strBuildingName=srcRsTemp->GetCollect("Building_Name");
				buildingNode.strFloorName=srcRsTemp->GetCollect("Floor_name");
				buildingNode.strRoomName=srcRsTemp->GetCollect("Room_name");
				buildingNode.strProudctName=srcRsTemp->GetCollect("Product_name");
				buildingNode.nProductClassID=srcRsTemp->GetCollect("Product_class_ID");
				buildingNode.ProductID=srcRsTemp->GetCollect("Product_ID");
				buildingNode.strScreenName=srcRsTemp->GetCollect("Screen_Name");
				buildingNode.nSerialID=srcRsTemp->GetCollect("Serial_ID");
				buildingNode.nBaudrate=srcRsTemp->GetCollect("Bautrate");
				buildingNode.strImgName=srcRsTemp->GetCollect("Background_Bmp");
				buildingNode.HWVersion=srcRsTemp->GetCollect("Hardware_Ver");
				buildingNode.SWVersion=srcRsTemp->GetCollect("Software_Ver");
				buildingNode.nEPsize=srcRsTemp->GetCollect("EPsize");
				nodesVector.push_back(buildingNode);
				srcRsTemp->MoveNext();//

			}	
		}
			if(srcRsTemp->State)
				srcRsTemp->Close();

		m_pRs->Open(_T("select * from ALL_NODE"),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);//	
		vector<CString> strSerialVector;
		while(VARIANT_FALSE==m_pRs->EndOfFile)
		{
			temp_variant=m_pRs->GetCollect("Serial_ID");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			if(!strTemp.IsEmpty())
				strSerialVector.push_back(strTemp);
			m_pRs->MoveNext();
		}
		if(m_pRs->State)
			m_pRs->Close();

		CString strSerialID;
		CString strBuildingName;
		CString strFloorName;
		CString strRoomName;
		CString strProudctName;
		CString strProductClassID;
		CString strProductID;
		CString strScreenName;
		CString strImgName;
		CString strHWVersion;
		CString strSWVersion;
		CString strEPsize;

		for(UINT i=0;i<nodesVector.size();i++)
		{

			bFind=FALSE;
			buildingNode=nodesVector.at(i);
			strSerialID.Format(_T("%d"),nodesVector.at(i).nSerialID);
			strBuildingName=nodesVector.at(i).strBuildingName;
			strFloorName=nodesVector.at(i).strFloorName;
			strRoomName=nodesVector.at(i).strRoomName;
			strProudctName=nodesVector.at(i).strProudctName;
			strProductClassID.Format(_T("%d"),nodesVector.at(i).nProductClassID);
			strProductID.Format(_T("%d"),nodesVector.at(i).ProductID);
			strScreenName=nodesVector.at(i).strScreenName;
			strBaudrate.Format(_T("%d"),nodesVector.at(i).nBaudrate);
			strImgName=nodesVector.at(i).strImgName;
			strHWVersion.Format(_T("%d"),nodesVector.at(i).HWVersion);
			strSWVersion.Format(_T("%d"),nodesVector.at(i).SWVersion);
			strEPsize.Format(_T("%d"),nodesVector.at(i).nEPsize);
			for(UINT k=0;k<strSerialVector.size();k++)
			{
				if(strSerialID.CompareNoCase(strSerialVector.at(k))==0)
				{
					bFind=TRUE;
					break;
				}

			}
			if(bFind)
			{
				try
				{


				strSql.Format(_T("update ALL_NODE set MainBuilding_Name='%s',Building_Name='%s',Floor_name='%s',Room_name='%s',Product_name='%s',Product_class_ID='%s',Product_ID='%s',Screen_Name='%s',Bautrate='%s',Background_imgID='%s',Hardware_Ver='%s',Software_Ver='%s',EPsize='%s' where Serial_ID='%s'"),strMainBuilding,strBuildingName,strFloorName,strRoomName,strProudctName,strProductClassID,strProductID,strScreenName,strBaudrate,strImgName,strHWVersion,strSWVersion,strEPsize,strSerialID);
  //new nc//strSql.Format(_T("update ALL_NODE set MainBuilding_Name='%s',Building_Name='%s',Floor_name='%s',Room_name='%s',Product_name='%s',Product_class_ID='%s',Product_ID='%s',Screen_Name='%s',Bautrate='%s',Background_imgID='%s',Hardware_Ver='%s',Software_Ver='%s',EPsize='%s',Mainnet_info='%s' where Serial_ID='%s'"),strMainBuilding,strBuildingName,strFloorName,strRoomName,strProudctName,strProductClassID,strProductID,strScreenName,strBaudrate,strImgName,strHWVersion,strSWVersion,strEPsize,strMainnetInfo,strSerialID);
				m_pCon->Execute(strSql.GetString(),NULL,adCmdText);		
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}
			}
			else
			{
				try
				{


				strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,EPsize) values('"+strMainBuilding+"','"+strBuildingName+"','"+strSerialID+"','"+strFloorName+"','"+strRoomName+"','"+strProudctName+"','"+strProductClassID+"','"+strProductID+"','"+strScreenName+"','"+strBaudrate+"','"+strImgName+"','"+strHWVersion+"','"+strSWVersion+"','"+strEPsize+"')"));
  //new nc//strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Mainnet_info,EPsize) values('"+strMainBuilding+"','"+strBuildingName+"','"+strSerialID+"','"+strFloorName+"','"+strRoomName+"','"+strProudctName+"','"+strProductClassID+"','"+strProductID+"','"+strScreenName+"','"+strBaudrate+"','"+strImgName+"','"+strHWVersion+"','"+strSWVersion+"','"+strEPsize+"','"+strMainnetInfo+"')"));
				m_pCon->Execute(strSql.GetString(),NULL,adCmdText);	
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}
			}

		}
	///====================================================end of lower version import========================================================
	}


	else//import date from new version:
	{

		CString	strSql;
		_variant_t temp_variant;
		BOOL bFind=FALSE;
		//new main building 
		/*
		srcRsTemp->Open(_T("select * from Building_ALL"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);		
		vector<_MainBuilding> srcMainBuildVector;
		//CString strTemp;
		BOOL bFind=FALSE;
		while(VARIANT_FALSE==srcRsTemp->EndOfFile)
		{
			_MainBuilding mainBuild;
			temp_variant=srcRsTemp->GetCollect("Building_Name");
			if(temp_variant.vt!=VT_NULL)
				mainBuild.strMainBuild=temp_variant;
			else
				mainBuild.strMainBuild=_T("");

			temp_variant=srcRsTemp->GetCollect("Telephone");
			if(temp_variant.vt!=VT_NULL)
				mainBuild.strTelephone=temp_variant;
			else
				mainBuild.strTelephone=_T("");
			temp_variant=srcRsTemp->GetCollect("Address");
			if(temp_variant.vt!=VT_NULL)
				mainBuild.strAdress=temp_variant;
			else
				mainBuild.strAdress=_T("");
			srcMainBuildVector.push_back(mainBuild);
			srcRsTemp->MoveNext();
		}
		if(srcRsTemp->State)
			srcRsTemp->Close();

		m_pRs->Open(_T("select * from Building_ALL"),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);		
		vector<_MainBuilding> dstMainBuildVector;
		//CString strTemp;
	//	BOOL bFind=FALSE;
		while(VARIANT_FALSE==m_pRs->EndOfFile)
		{
			_MainBuilding mainBuild;
			temp_variant=m_pRs->GetCollect("Building_Name");
			if(temp_variant.vt!=VT_NULL)
				mainBuild.strMainBuild=temp_variant;
			else
				mainBuild.strMainBuild=_T("");

			temp_variant=m_pRs->GetCollect("Telephone");
			if(temp_variant.vt!=VT_NULL)
				mainBuild.strTelephone=temp_variant;
			else
				mainBuild.strTelephone=_T("");
			temp_variant=m_pRs->GetCollect("Address");
			if(temp_variant.vt!=VT_NULL)
				mainBuild.strAdress=temp_variant;
			else
				mainBuild.strAdress=_T("");
			dstMainBuildVector.push_back(mainBuild);
			m_pRs->MoveNext();
		}
		if(m_pRs->State)
			m_pRs->Close();

		for(int i=0;i<srcMainBuildVector.size();i++)
		{
			bFind=FALSE;
			CString strMainBuilding=srcMainBuildVector.at(i).strMainBuild;
			BOOL bDefault=0;
			CString strTelphone=srcMainBuildVector.at(i).strTelephone;
			CString strAdress=srcMainBuildVector.at(i).strAdress;

			for(int k=0;k<dstMainBuildVector.size();k++)
			{
				if(srcMainBuildVector.at(i).strMainBuild.CompareNoCase(dstMainBuildVector.at(k).strMainBuild)==0)
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind)
			{
				//update
				strSql.Format(_T("update Building_ALL set Building_Name='%s',Telephone='%s',Address='%s'"),strMainBuilding,strTelphone,strAdress);
				m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
			}
			else
			{
				//insert
				strSql.Format(_T("insert into Building_ALL (Building_Name,Telephone ,Address) values('"+strMainBuilding+"','"+strTelphone+"','"+strAdress+"')"));
				m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
			}
		}
		*/
		
		////////////////////////////import subbuilding////////////////////////////////////////////////////////
		CString strmainBuilding;
		CString strbuildingName;
		CString strProtocol;
		CString strIpAdress;
		CString strIpPort;
		CString strComPort;
		CString strBaudrate;
		//BOOL bSrcDefaut;

		vector<_SubBuilding> srcSubBuildingVector;
		srcRsTemp->Open(_T("select * from Building"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);	
		while(VARIANT_FALSE==srcRsTemp->EndOfFile)
		{
			_SubBuilding srcSubBuid;
			temp_variant=srcRsTemp->GetCollect("Main_BuildingName");
			if(temp_variant.vt!=VT_NULL)
				srcSubBuid.strmainBuilding=temp_variant;
			else
				srcSubBuid.strmainBuilding=_T("");

			temp_variant=srcRsTemp->GetCollect("Building_Name");
			if(temp_variant.vt!=VT_NULL)
				srcSubBuid.strbuildingName=temp_variant;
			else
				srcSubBuid.strbuildingName=_T("");

			temp_variant=srcRsTemp->GetCollect("Protocal");
			if(temp_variant.vt!=VT_NULL)
				srcSubBuid.strProtocol=temp_variant;
			else
				srcSubBuid.strProtocol=_T("");

			temp_variant=srcRsTemp->GetCollect("Com_Port");
			if(temp_variant.vt!=VT_NULL)
				srcSubBuid.strComPort=temp_variant;
			else
				srcSubBuid.strComPort=_T("");
			temp_variant=srcRsTemp->GetCollect("Ip_Address");
			if(temp_variant.vt!=VT_NULL)
				srcSubBuid.strIpAdress=temp_variant;
			else
				srcSubBuid.strIpAdress=_T("");
			
			temp_variant=srcRsTemp->GetCollect("Ip_Port");
			if(temp_variant.vt!=VT_NULL)
				srcSubBuid.strIpPort=temp_variant;
			else
				srcSubBuid.strIpPort=_T("");
			temp_variant=srcRsTemp->GetCollect("Braudrate");
			if(temp_variant.vt!=VT_NULL)
				srcSubBuid.m_strBaudrate=temp_variant;
			else
				srcSubBuid.m_strBaudrate=_T("");
			srcSubBuildingVector.push_back(srcSubBuid);
			srcRsTemp->MoveNext();
		}
		if(srcRsTemp->State)
			srcRsTemp->Close();

		vector<CString> SubBuildNameVector;
		CString strTemp;
		m_pRs->Open(_T("select * from Building"),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);	
		while(VARIANT_FALSE==m_pRs->EndOfFile)
		{
			temp_variant=m_pRs->GetCollect("Building_Name");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			if(!strTemp.IsEmpty())
				SubBuildNameVector.push_back(strTemp);
			m_pRs->MoveNext();
		}
		if(m_pRs->State)
			m_pRs->Close();
		for(UINT i=0;i<srcSubBuildingVector.size();i++)
		{
			bFind=FALSE;
			strmainBuilding=srcSubBuildingVector.at(i).strmainBuilding;
			strbuildingName=srcSubBuildingVector.at(i).strbuildingName;
			strProtocol=srcSubBuildingVector.at(i).strProtocol;
			strIpAdress=srcSubBuildingVector.at(i).strIpAdress;
			strIpPort=srcSubBuildingVector.at(i).strIpPort;
			strComPort=srcSubBuildingVector.at(i).strComPort;
			strBaudrate=srcSubBuildingVector.at(i).m_strBaudrate;
			for(UINT k=0;k<SubBuildNameVector.size();k++)
			{
				if(strbuildingName.CompareNoCase(SubBuildNameVector.at(k))==0)
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind)
			{
				try
				{


				strSql.Format(_T("update Building set Building_Name='%s',Protocal='%s',Ip_Address='%s',Ip_Port='%s',Com_Port='%s',Braudrate='%s' where Building_Name='%s' and Main_BuildingName='%s'"),strbuildingName,strProtocol,strProtocol,strIpPort,strComPort,strBaudrate,strbuildingName,strmainBuilding);
				m_pCon->Execute(strSql.GetString(),NULL,adCmdText);		
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}
			}
			else
			{
				//////insert to sub building ,"Main Building" as the name of the building.
				strSql.Format(_T("insert into Building (Main_BuildingName,Building_Name,Protocal,Com_Port,Ip_Address,Ip_Port,Braudrate) values('"+strmainBuilding+"','"+strbuildingName+"','"+strProtocol+"','"+strComPort+"','"+strIpAdress+"','"+strIpPort+"','"+strBaudrate+"')"));
				m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
			}
		}

		////////////////////////////////////////import all nodes//////////////////////////////////////////////	
		srcRsTemp->Open(_T("select * from ALL_NODE"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);//	
		vector<_TNodes> nodesVector;
		while(VARIANT_FALSE==srcRsTemp->EndOfFile)
		{
			_TNodes TNode;

			temp_variant=srcRsTemp->GetCollect("Serial_ID");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			if(strTemp.IsEmpty())
				continue;
			TNode.strSerialID=strTemp;
			
			temp_variant=srcRsTemp->GetCollect("MainBuilding_Name");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			TNode.strMainBuildingName=strTemp;

			temp_variant=srcRsTemp->GetCollect("Building_Name");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			TNode.strSubBuildingName=strTemp;

			temp_variant=srcRsTemp->GetCollect("Floor_name");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			TNode.strFloorName=strTemp;

			temp_variant=srcRsTemp->GetCollect("Room_name");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			TNode.strRoomName=strTemp;

			temp_variant=srcRsTemp->GetCollect("Product_name");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			TNode.strProudctName=strTemp;

			temp_variant=srcRsTemp->GetCollect("Product_class_ID");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			TNode.strProductClassID=strTemp;

			temp_variant=srcRsTemp->GetCollect("Product_ID");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			TNode.strProductID=strTemp;

			temp_variant=srcRsTemp->GetCollect("Screen_Name");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			TNode.strScreenName=strTemp;

			temp_variant=srcRsTemp->GetCollect("Bautrate");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			TNode.strBaudrate=strTemp;


			temp_variant=srcRsTemp->GetCollect("Background_imgID");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			TNode.strImgName=strTemp;

			temp_variant=srcRsTemp->GetCollect("Hardware_Ver");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			TNode.strHWVersion=strTemp;

			temp_variant=srcRsTemp->GetCollect("Software_Ver");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			TNode.strSWVersion=strTemp;

			temp_variant=srcRsTemp->GetCollect("Com_Port");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			TNode.strComport=strTemp;

			temp_variant=srcRsTemp->GetCollect("EPsize");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			TNode.strEPsize=strTemp;


			nodesVector.push_back(TNode);
			srcRsTemp->MoveNext();

		}
		if(srcRsTemp->State)
			srcRsTemp->Close();



		CString strSerialID;
		CString strBuildingName;
		CString strSubBuildingName;
		CString strFloorName;
		CString strRoomName;
		CString strProudctName;
		CString strProductClassID;
		CString strProductID;
		CString strScreenName;
		CString strImgName;
		CString strHWVersion;
		CString strSWVersion;
		CString strEPsize;
		CString strComport;
		
		m_pRs->Open(_T("select * from ALL_NODE"),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);//	
		vector<CString> strSerialVector;
		while(VARIANT_FALSE==m_pRs->EndOfFile)
		{
			temp_variant=m_pRs->GetCollect("Serial_ID");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			if(!strTemp.IsEmpty())
				strSerialVector.push_back(strTemp);
			m_pRs->MoveNext();
		}
		if(m_pRs->State)
			m_pRs->Close();
	
		for(UINT i=0;i<nodesVector.size();i++)
		{

			bFind=FALSE;
			strSerialID=nodesVector.at(i).strSerialID;
			strBuildingName=nodesVector.at(i).strMainBuildingName;
			strSubBuildingName=nodesVector.at(i).strSubBuildingName;

			strFloorName=nodesVector.at(i).strFloorName;
			strRoomName=nodesVector.at(i).strRoomName;
			strProudctName=nodesVector.at(i).strProudctName;

			strProductClassID=nodesVector.at(i).strProductClassID;
			strProductID=nodesVector.at(i).strProductID;
			strScreenName=nodesVector.at(i).strScreenName;

			strBaudrate=nodesVector.at(i).strBaudrate;
			strImgName=nodesVector.at(i).strImgName;

			strHWVersion=nodesVector.at(i).strHWVersion;
			strSWVersion=nodesVector.at(i).strSWVersion;
			strEPsize=nodesVector.at(i).strEPsize;
			strComport=nodesVector.at(i).strComport;
			
			for(UINT k=0;k<strSerialVector.size();k++)
			{
				if(strSerialID.CompareNoCase(strSerialVector.at(k))==0)
				{
					bFind=TRUE;
					break;
				}

			}
			if(bFind)
			{
				try
				{

				strSql.Format(_T("update ALL_NODE set MainBuilding_Name='%s',Building_Name='%s',Floor_name='%s',Room_name='%s',Product_name='%s',Product_class_ID='%s',Product_ID='%s',Screen_Name='%s',Bautrate='%s',Background_imgID='%s',Hardware_Ver='%s',Software_Ver='%s',Com_Port='%s',EPsize='%s' where Serial_ID='%s'"),strBuildingName,strBuildingName,strFloorName,strRoomName,strProudctName,strProductClassID,strProductID,strScreenName,strBaudrate,strImgName,strHWVersion,strSWVersion,strComport,strEPsize,strSerialID);
				//new nc//strSql.Format(_T("update ALL_NODE set MainBuilding_Name='%s',Building_Name='%s',Floor_name='%s',Room_name='%s',Product_name='%s',Product_class_ID='%s',Product_ID='%s',Screen_Name='%s',Bautrate='%s',Background_imgID='%s',Hardware_Ver='%s',Software_Ver='%s',Com_Port='%s',EPsize='%s',Mainnet_info='%s' where Serial_ID='%s'"),strBuildingName,strBuildingName,strFloorName,strRoomName,strProudctName,strProductClassID,strProductID,strScreenName,strBaudrate,strImgName,strHWVersion,strSWVersion,strComport,strEPsize,strMainnetInfo,strSerialID);
			
				m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}
			}
			else
			{
				try
				{


				strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize) values('"+strBuildingName+"','"+strBuildingName+"','"+strSerialID+"','"+strFloorName+"','"+strRoomName+"','"+strProudctName+"','"+strProductClassID+"','"+strProductID+"','"+strScreenName+"','"+strBaudrate+"','"+strImgName+"','"+strHWVersion+"','"+strSWVersion+"','"+strComport+"','"+strEPsize+"')"));
				//new nc// strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Mainnet_info) values('"+strBuildingName+"','"+strBuildingName+"','"+strSerialID+"','"+strFloorName+"','"+strRoomName+"','"+strProudctName+"','"+strProductClassID+"','"+strProductID+"','"+strScreenName+"','"+strBaudrate+"','"+strImgName+"','"+strHWVersion+"','"+strSWVersion+"','"+strComport+"','"+strEPsize+"','"+strMainnetInfo+"')"));
				m_pCon->Execute(strSql.GetString(),NULL,adCmdText);		
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}
			}

		}

		
	/////////////////////////////////////IOName Table/////////////////////////////////////////////
	//ImportIONameTable(srcConTmp, srcRsTemp);
	{
		if(srcRsTemp->State)
			srcRsTemp->Close();
		srcRsTemp->Open(_T("select * from IONAME"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);//	
		vector<_IONameNodes> szNode;
// 		VARIANT temp_variant;
// 		CString strTemp;

		while(VARIANT_FALSE==srcRsTemp->EndOfFile)
		{
			_IONameNodes IONode;


			temp_variant=srcRsTemp->GetCollect("SERIAL_ID");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			if(strTemp.IsEmpty())
				continue;
			IONode.strSerialID=strTemp;

			temp_variant=srcRsTemp->GetCollect("SENSORNAME");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			IONode.strSensorName=strTemp;

			temp_variant=srcRsTemp->GetCollect("INPUT1");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			IONode.szInputName[0]=strTemp;

			for (UINT i = 0; i < 8; i++)
			{
				CString strIndex;
				strIndex.Format(_T("INPUT%d"), i+1);
				temp_variant=srcRsTemp->GetCollect((_variant_t)strIndex);
				if(temp_variant.vt!=VT_NULL)
					strTemp=temp_variant;
				else
					strTemp=_T("");
				IONode.szInputName[i]=strTemp;
			}

			for (UINT i = 0; i < 7; i++)
			{
				CString strIndex;
				strIndex.Format(_T("OUTPUT%d"), i+1);
				temp_variant=srcRsTemp->GetCollect((_variant_t)strIndex);
				if(temp_variant.vt!=VT_NULL)
					strTemp=temp_variant;
				else
					strTemp=_T("");
				IONode.szOutputName[i]=strTemp;
			}

			szNode.push_back(IONode);
			srcRsTemp->MoveNext();

		}
		if(srcRsTemp->State)
			srcRsTemp->Close();



		// 	CString strSerialID;
		// 	CString strBuildingName;
		// 	CString strSubBuildingName;
		// 	CString strFloorName;
		// 	CString strRoomName;
		// 	CString strProudctName;
		// 	CString strProductClassID;
		// 	CString strProductID;
		// 	CString strScreenName;
		// 	CString strImgName;
		// 	CString strHWVersion;
		// 	CString strSWVersion;
		// 	CString strEPsize;
		// 	CString strComport;

		m_pRs->Open(_T("select * from IONAME"),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);//	
		vector<CString> szSerialVector;
		while(VARIANT_FALSE==m_pRs->EndOfFile)
		{
			temp_variant=m_pRs->GetCollect("SERIAL_ID");
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			if(!strTemp.IsEmpty())
				szSerialVector.push_back(strTemp);
			m_pRs->MoveNext();
		}
		if(m_pRs->State)
			m_pRs->Close();


		//////////////////////////////////////////////////////////////////////////

		for(UINT i=0;i<szNode.size();i++)
		{
			BOOL bFind=FALSE;

			CString strSerialID= szNode[i].strSerialID;
			for(UINT k=0;k<szSerialVector.size();k++)
			{
				if(strSerialID.CompareNoCase(szSerialVector.at(k))==0)
				{
					bFind=TRUE;
					break;
				}

			}

			CString strSql;
			if(bFind)
			{		
				try
				{


				strSql.Format(_T("update IONAME set SERIAL_ID='%s',SENSORNAME='%s',INPUT1='%s',INPUT2='%s',INPUT3='%s',INPUT4='%s',INPUT5='%s',INPUT6='%s',INPUT7='%s',INPUT8='%s',OUTPUT1='%s',OUTPUT2='%s',OUTPUT3='%s',OUTPUT4='%s',OUTPUT5='%s',OUTPUT6='%s',OUTPUT7='%s' where Serial_ID='%s'") \
					,szNode[i].strSerialID,szNode[i].strSensorName,szNode[i].szInputName[0],szNode[i].szInputName[1],szNode[i].szInputName[2],szNode[i].szInputName[3],szNode[i].szInputName[4],szNode[i].szInputName[5],szNode[i].szInputName[6],szNode[i].szInputName[7] \
					,szNode[i].szOutputName[0],szNode[i].szOutputName[1],szNode[i].szOutputName[2],szNode[i].szOutputName[3],szNode[i].szOutputName[4],szNode[i].szOutputName[5],szNode[i].szOutputName[6],strSerialID);
				m_pCon->Execute(strSql.GetString(),NULL,adCmdText);		
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}
			}
			else
			{
				try
				{


				strSql.Format(_T("insert into IONAME (SERIAL_ID,SENSORNAME,INPUT1,INPUT2,INPUT3,INPUT4,INPUT5,INPUT6,INPUT7,INPUT8,OUTPUT1,OUTPUT2,OUTPUT3,OUTPUT4,OUTPUT5,OUTPUT6,OUTPUT7) values \
								 ('"+szNode[i].strSerialID+"','"+szNode[i].strSensorName+"','"+szNode[i].szInputName[0]+"','"+szNode[i].szInputName[1]+"','"+szNode[i].szInputName[2]+"','"+szNode[i].szInputName[3]+"','"+szNode[i].szInputName[4]+"','"+szNode[i].szInputName[5]+"','"+szNode[i].szInputName[6]+"','"+szNode[i].szInputName[7]+"' \
								 ,'"+szNode[i].szOutputName[0]+"','"+szNode[i].szOutputName[1]+"','"+szNode[i].szOutputName[2]+"','"+szNode[i].szOutputName[3]+"','"+szNode[i].szOutputName[4]+"','"+szNode[i].szOutputName[5]+"','"+szNode[i].szOutputName[6]+"')"));


				m_pCon->Execute(strSql.GetString(),NULL,adCmdText);		
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}
			}
		}
	}

	/////////////////////////////////////import Screen Label/////////////////////////////////////////////
	srcRsTemp->Open(_T("select * from Screen_Label"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);//	
	CString strtemp;
	int nSerial_Num;
	int nTemp;
	vector<_Label_information> labelsVector;
	while(VARIANT_FALSE==srcRsTemp->EndOfFile)
	{	
		_Label_information label;

		nSerial_Num=srcRsTemp->GetCollect("Serial_Num");//
		label.nSerialNum=nSerial_Num;

		nTemp=srcRsTemp->GetCollect("Cstatic_id");//
		label.cstatic_id=nTemp;

		nTemp=srcRsTemp->GetCollect("Point_X");//
		label.point.x=nTemp;

		nTemp=srcRsTemp->GetCollect("Point_Y");//
		label.point.y=nTemp;

		nTemp=srcRsTemp->GetCollect("Height");//
		label.height=nTemp;

		nTemp=srcRsTemp->GetCollect("Width");//
		label.width=nTemp;

		nTemp=srcRsTemp->GetCollect("Tstat_id");//
		label.tstat_id=nTemp;

		nTemp=srcRsTemp->GetCollect("Status");//
		label.status=nTemp;
		
		temp_variant=srcRsTemp->GetCollect("Tips");//
		if(temp_variant.vt!=VT_NULL)
			strtemp=temp_variant;
		else
			strtemp=_T("");
		label.strTips=_T("");//strtemp;

		nTemp=srcRsTemp->GetCollect("Input_or_Output");//
		label.input_or_output=nTemp;

		strTemp=srcRsTemp->GetCollect("Text_Color");//
		if(strTemp.IsEmpty())
		{
			label.clrTxt=0;
		}
		else
		{
			label.clrTxt=_wtol(strTemp);
		}
		
		
		strTemp=srcRsTemp->GetCollect("Back_Color");//
		if(strTemp.IsEmpty())
		{
			label.bkColor=254*255*255;
		}
		else
		{
				label.bkColor=_wtol(strTemp);
		}

		labelsVector.push_back(label);
		srcRsTemp->MoveNext();
	}
		
	if(srcRsTemp->State)
		srcRsTemp->Close();

		m_pRs->Open(_T("select distinct Serial_Num from Screen_Label"),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);//	
		strSerialVector.clear();
		vector<int> nSerialVector;
		while(VARIANT_FALSE==m_pRs->EndOfFile)
		{
			nTemp=m_pRs->GetCollect("Serial_Num");//

			nSerialVector.push_back(nTemp);
			m_pRs->MoveNext();
		}
		if(m_pRs->State)
			m_pRs->Close();
		
		for(UINT i=0;i<labelsVector.size();i++)
		{
			bFind=FALSE;
			for(UINT k=0;k<nSerialVector.size();k++)
			{
				if(labelsVector.at(i).nSerialNum==nSerialVector.at(k))
				{
					bFind=TRUE;
					break;
				}
			}
			if(bFind)
			{
				try
				{


				//delete it first;
				CString strSql;
				strSql.Format(_T("delete * from Screen_Label where Serial_Num=%i"),labelsVector.at(i).nSerialNum);
				m_pCon->Execute(strSql.GetString(),NULL,adCmdText);		
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}

			}
		}
		//and then insert;
		for(UINT i=0;i<labelsVector.size();i++)
		{
		//		strSql.Format(_T("insert into Screen_Label values(%i,%i,%i,%i,%i,%i,%i,%i,'%s',%i,%i,%i)"),nContyrolID,m_nSerialNum,50,50,m_nheight,m_nwidth,m_id,m_nstatus,m_strScreenName,m_input_or_output,m_clrTxt,m_bkColor);
	              
			CString strclrTxt,strclrBk;
			strclrTxt.Format(_T("%u"),labelsVector.at(i).clrTxt);
			strclrBk.Format(_T("%u"),labelsVector.at(i).bkColor);

			strSql.Format(_T("insert into Screen_Label (Cstatic_id,Serial_Num,Point_X,Point_Y,Height,Width,Tstat_id,Status,Tips,Input_or_Output,Text_Color,Back_Color) values(%i,%i,%i,%i,%i,%i,%i,%i,'%s',%i,'%s','%s')"),
				labelsVector.at(i).cstatic_id,
				labelsVector.at(i).nSerialNum,
				labelsVector.at(i).point.x,
				labelsVector.at(i).point.y,
				labelsVector.at(i).height,
				labelsVector.at(i).width,
				labelsVector.at(i).tstat_id,
				labelsVector.at(i).status,
				labelsVector.at(i).strTips,
				labelsVector.at(i).input_or_output,
				strclrTxt,
				strclrBk);
		//	AfxMessageBox(strSql);
			try
			{

	
				m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
		}
    }
	EndWaitCursor();
	
	OnOK();
}



void CImportDatabaseDlg::OnClickedBrowsBtn()
{
	CString strFilter = _T("mdb File|*.mdb|all File|*.*||");
	CFileDialog dlg(true,_T("hex"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
	dlg.DoModal();
	
	CString strTemp;
	strTemp=dlg.GetPathName();		
	CFile default_file;
	if(strTemp.IsEmpty())
	{
		return;
	}
	m_strSourceDbFileName=strTemp;
	m_sourceDbEditCtrl.SetWindowText(m_strSourceDbFileName);



}

BOOL CImportDatabaseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(BST_CHECKED);
	//m_upperBtn.SetCheck(BST_UNCHECKED);

	m_pCon.CreateInstance("ADODB.Connection");
	m_pRs.CreateInstance("ADODB.Recordset");
	m_pCon->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CImportDatabaseDlg::OnBnClickedRadio1()
{
	m_bfromLower=TRUE;
}

void CImportDatabaseDlg::OnBnClickedRadio2()
{
	m_bfromLower=FALSE;
	// TODO: Add your control notification handler code here
}



void CImportDatabaseDlg::ImportIONameTable(_ConnectionPtr&  srcConTmp, _RecordsetPtr& srcRsTemp)
{
	srcRsTemp->Open(_T("select * from IONAME"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);//	
	vector<_IONameNodes> szNode;
	VARIANT temp_variant;
	CString strTemp;
	while(VARIANT_FALSE==srcRsTemp->EndOfFile)
	{
		_IONameNodes IONode;


		temp_variant=srcRsTemp->GetCollect("SERIAL_ID");
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		if(strTemp.IsEmpty())
			continue;
		IONode.strSerialID=strTemp;

		temp_variant=srcRsTemp->GetCollect("SENSORNAME");
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		IONode.strSensorName=strTemp;

		temp_variant=srcRsTemp->GetCollect("INPUT1");
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		IONode.szInputName[0]=strTemp;
		
		for (UINT i = 0; i < 8; i++)
		{
			CString strIndex;
			strIndex.Format(_T("INPUT%d"), i+1);
			temp_variant=srcRsTemp->GetCollect((_variant_t)strIndex);
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			IONode.szInputName[i]=strTemp;
		}

		for (UINT i = 0; i < 7; i++)
		{
			CString strIndex;
			strIndex.Format(_T("OUTPUT%d"), i+1);
			temp_variant=srcRsTemp->GetCollect((_variant_t)strIndex);
			if(temp_variant.vt!=VT_NULL)
				strTemp=temp_variant;
			else
				strTemp=_T("");
			IONode.szOutputName[i]=strTemp;
		}

		szNode.push_back(IONode);
		srcRsTemp->MoveNext();

	}
	if(srcRsTemp->State)
	srcRsTemp->Close();



// 	CString strSerialID;
// 	CString strBuildingName;
// 	CString strSubBuildingName;
// 	CString strFloorName;
// 	CString strRoomName;
// 	CString strProudctName;
// 	CString strProductClassID;
// 	CString strProductID;
// 	CString strScreenName;
// 	CString strImgName;
// 	CString strHWVersion;
// 	CString strSWVersion;
// 	CString strEPsize;
// 	CString strComport;

	m_pRs->Open(_T("select * from IONAME"),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);//	
	vector<CString> strSerialVector;
	while(VARIANT_FALSE==m_pRs->EndOfFile)
	{
		temp_variant=m_pRs->GetCollect("SERIAL_ID");
		if(temp_variant.vt!=VT_NULL)
			strTemp=temp_variant;
		else
			strTemp=_T("");
		if(!strTemp.IsEmpty())
			strSerialVector.push_back(strTemp);
		m_pRs->MoveNext();
	}
	if(m_pRs->State)
	m_pRs->Close();


	//////////////////////////////////////////////////////////////////////////

	for(UINT i=0;i<szNode.size();i++)
	{
		BOOL bFind=FALSE;

		CString strSerialID= szNode[i].strSerialID;
		for(UINT k=0;k<strSerialVector.size();k++)
		{
			if(strSerialID.CompareNoCase(strSerialVector.at(k))==0)
			{
				bFind=TRUE;
				break;
			}

		}

		CString strSql;
		if(bFind)
		{			
			try
			{

			strSql.Format(_T("update IONAME set SERIAL_ID='%s',SENSORNAME='%s',INPUT1='%s',INPUT2='%s',INPUT3='%s',INPUT4='%s',INPUT5='%s',INPUT6='%s',INPUT7='%s',INPUT8='%s',OUTPUT1='%s',OUTPUT2='%s',OUTPUT3='%s',OUTPUT4='%s',OUTPUT5='%s',OUTPUT6='%s',OUTPUT7='%s' where Serial_ID='%s'") \
									,szNode[i].strSerialID,szNode[i].strSensorName,szNode[i].szInputName[0],szNode[i].szInputName[1],szNode[i].szInputName[2],szNode[i].szInputName[3],szNode[i].szInputName[4],szNode[i].szInputName[5],szNode[i].szInputName[6],szNode[i].szInputName[7] \
									,szNode[i].szOutputName[0],szNode[i].szOutputName[1],szNode[i].szOutputName[2],szNode[i].szOutputName[3],szNode[i].szOutputName[4],szNode[i].szOutputName[5],szNode[i].szOutputName[6],strSerialID);
			m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
		}
		else
		{
			try
			{

			strSql.Format(_T("insert into IONAME (SERIAL_ID,SENSORNAME,INPUT1,INPUT2,INPUT3,INPUT4,INPUT5,INPUT6,INPUT7,INPUT8,OUTPUT1,OUTPUT2,OUTPUT3,OUTPUT4,OUTPUT5,OUTPUT6,OUTPUT7) values \
				('"+szNode[i].strSerialID+"','"+szNode[i].strSensorName+"','"+szNode[i].szInputName[0]+"','"+szNode[i].szInputName[1]+"','"+szNode[i].szInputName[2]+"','"+szNode[i].szInputName[3]+"','"+szNode[i].szInputName[4]+"','"+szNode[i].szInputName[5]+"','"+szNode[i].szInputName[6]+"','"+szNode[i].szInputName[7]+"' \
				,'"+szNode[i].szOutputName[0]+"','"+szNode[i].szOutputName[1]+"','"+szNode[i].szOutputName[2]+"','"+szNode[i].szOutputName[3]+"','"+szNode[i].szOutputName[4]+"','"+szNode[i].szOutputName[5]+"','"+szNode[i].szOutputName[6]+"')"));
			
			
			m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
		}

	}
}