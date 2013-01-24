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

extern struct Label_information;

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

}



void CImportDatabaseDlg::OnClickedBrowsBtn()
{




}

BOOL CImportDatabaseDlg::OnInitDialog()
{
	

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

}