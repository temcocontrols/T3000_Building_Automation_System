// AllNodesDiaolg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "MainFrm.h"
#include "AllNodesDiaolg.h"


// CAllNodesDiaolg dialog
#define AN_MAINNAME 1
#define AN_NAME		2
#define AN_SerialID	3
#define AN_FLOORNAME	4
#define	AN_ROOMNAME	5
#define	AN_PRUDUCTNAME	6
#define AN_PRODUCTTYPE	7
#define AN_PRODUCTID	8
#define AN_SCREENID	    9
#define AN_BAUDRATE		10
#define	AN_GRAPHICID	11
#define AN_HDVERSION	12
#define AN_SWVERSION	13
#define AN_EPSIZE		14

IMPLEMENT_DYNAMIC(CAllNodesDiaolg, CDialog)

CAllNodesDiaolg::CAllNodesDiaolg(CWnd* pParent /*=NULL*/)
	: CDialog(CAllNodesDiaolg::IDD, pParent)
{

	m_bChanged=FALSE;
}

CAllNodesDiaolg::~CAllNodesDiaolg()
{
}

void CAllNodesDiaolg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_FlexGrid);
	DDX_Control(pDX, IDC_ADDBUTTON, m_AddBtn);
	DDX_Control(pDX, IDC_DELBUTTON, m_delBtn);
	DDX_Control(pDX, IDC_DELALLBUTTON, m_DelBtn);
	DDX_Control(pDX, IDC_EXITBUTTON, m_ExitBtn);
	DDX_Control(pDX, IDC_PRODUCTTYPECOMBO, m_productCombox);
	DDX_Control(pDX, IDC_TEXTEDIT, m_InputTextEdt);
	DDX_Control(pDX, IDC_COMBO1, m_SubLstCombox);
}


BEGIN_MESSAGE_MAP(CAllNodesDiaolg, CDialog)
	ON_BN_CLICKED(IDOK, &CAllNodesDiaolg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ADDBUTTON, &CAllNodesDiaolg::OnBnClickedAddbutton)
	ON_BN_CLICKED(IDC_DELBUTTON, &CAllNodesDiaolg::OnBnClickedDelbutton)
	ON_BN_CLICKED(IDC_DELALLBUTTON, &CAllNodesDiaolg::OnBnClickedDelallbutton)
	ON_BN_CLICKED(IDC_EXITBUTTON, &CAllNodesDiaolg::OnBnClickedExitbutton)
	ON_BN_CLICKED(IDCANCEL, &CAllNodesDiaolg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAllNodesDiaolg::OnCbnSelchangeCombo1)
	ON_EN_KILLFOCUS(IDC_TEXTEDIT, &CAllNodesDiaolg::OnEnKillfocusTextedit)
	ON_EN_SETFOCUS(IDC_TEXTEDIT, &CAllNodesDiaolg::OnEnSetfocusTextedit)
END_MESSAGE_MAP()


// CAllNodesDiaolg message handlers

BOOL CAllNodesDiaolg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_InputTextEdt.ShowWindow(SW_HIDE);
	m_productCombox.ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_ADDBUTTON)->ShowWindow(SW_HIDE);

	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	m_strMainBuildingName= pFrame->m_strCurMainBuildingName;
	m_strSubNetName= pFrame->m_strCurSubBuldingName;
	int k=0;
	for(UINT i=0;i<pFrame->m_subNetLst.size();i++)
	{
		if(m_strSubNetName.CompareNoCase(pFrame->m_subNetLst.at(i).strBuildingName)==0)
			k=i;
		m_SubLstCombox.AddString(pFrame->m_subNetLst.at(i).strBuildingName);
	}
	m_SubLstCombox.SetCurSel(k);

	m_SubLstCombox.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
	
	m_pCon.CreateInstance(_T("ADODB.Connection"));
	m_pRs.CreateInstance(_T("ADODB.Recordset"));
	m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

	ReloadAddBuildingDB();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CAllNodesDiaolg::OnBnClickedOk()
{
 

}

void CAllNodesDiaolg::ReloadAddBuildingDB()
{
	m_FlexGrid.Clear();
	
	m_FlexGrid.put_TextMatrix(0,0,_T(""));
	m_FlexGrid.put_ColWidth(0,400);

	m_FlexGrid.put_TextMatrix(0,AN_MAINNAME,_T("Main Building"));
	m_FlexGrid.put_ColWidth(AN_MAINNAME,800);

	m_FlexGrid.put_TextMatrix(0,AN_NAME,_T("Sub Net"));
	m_FlexGrid.put_ColWidth(AN_NAME,800);

	m_FlexGrid.put_TextMatrix(0,AN_SerialID,_T("Serial ID"));
	m_FlexGrid.put_ColWidth(AN_SerialID,800);

	m_FlexGrid.put_TextMatrix(0,AN_FLOORNAME,_T("Floor"));
	m_FlexGrid.put_ColWidth(AN_FLOORNAME,800);

	m_FlexGrid.put_TextMatrix(0,AN_ROOMNAME,_T("Room"));
	m_FlexGrid.put_ColWidth(AN_ROOMNAME,800);

	m_FlexGrid.put_TextMatrix(0,AN_PRUDUCTNAME,_T("Product Name"));
	m_FlexGrid.put_ColWidth(AN_PRUDUCTNAME,1300);

	m_FlexGrid.put_TextMatrix(0,AN_PRODUCTTYPE,_T("Product ID"));
	m_FlexGrid.put_ColWidth(AN_PRODUCTTYPE,1300);

	m_FlexGrid.put_TextMatrix(0,AN_PRODUCTID,_T("Address"));
	m_FlexGrid.put_ColWidth(AN_PRODUCTID,700);

	m_FlexGrid.put_TextMatrix(0,AN_SCREENID,_T("Screen ID"));
	m_FlexGrid.put_ColWidth(AN_SCREENID,2000);

	m_FlexGrid.put_TextMatrix(0,AN_BAUDRATE,_T("Baud Rate"));
	m_FlexGrid.put_ColWidth(AN_BAUDRATE,1300);

	m_FlexGrid.put_TextMatrix(0,AN_GRAPHICID,_T("Graphic Name"));
	m_FlexGrid.put_ColWidth(AN_GRAPHICID,1800);

	m_FlexGrid.put_TextMatrix(0,AN_HDVERSION,_T("HardW_Ver"));
	m_FlexGrid.put_ColWidth(AN_HDVERSION,1300);

	m_FlexGrid.put_TextMatrix(0,AN_SWVERSION,_T("SoftW_Ver"));
	m_FlexGrid.put_ColWidth(AN_SWVERSION,1300);
	m_FlexGrid.put_TextMatrix(0,AN_EPSIZE,_T("EPSize"));
	m_FlexGrid.put_ColWidth(AN_EPSIZE,800);

	CString strSql;
	strSql.Format(_T("select * from ALL_NODE where MainBuilding_Name = '%s' ORDER BY Product_ID ASC"),m_strMainBuildingName);
	m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);			
	m_FlexGrid.put_Rows(m_pRs->RecordCount+2);	
	int temp_row=0;
	CString str_temp;
	str_temp.Empty();
	_variant_t temp_variant;
	while(VARIANT_FALSE==m_pRs->EndOfFile)
	{	
	
		//m_FlexGrid.put_TextMatrix(temp_row,AN_PRODUCTTYPE,m_strID);

		++temp_row;
		CString strIndex; strIndex.Format(_T("%d"), temp_row);
		m_FlexGrid.put_TextMatrix(temp_row,0,strIndex);

		m_FlexGrid.put_TextMatrix(temp_row,AN_MAINNAME,m_strMainBuildingName);
		m_strSubNetName=m_pRs->GetCollect("Building_Name");//
		m_FlexGrid.put_TextMatrix(temp_row,AN_NAME,m_strSubNetName);
		temp_variant=m_pRs->GetCollect("Serial_ID");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_FlexGrid.put_TextMatrix(temp_row,AN_SerialID,m_strID);
	
		temp_variant=m_pRs->GetCollect("Floor_name");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_FlexGrid.put_TextMatrix(temp_row,AN_FLOORNAME,m_strID);

		temp_variant=m_pRs->GetCollect("Room_name");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_FlexGrid.put_TextMatrix(temp_row,AN_ROOMNAME,m_strID);
		
			temp_variant=m_pRs->GetCollect("Product_name");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_FlexGrid.put_TextMatrix(temp_row,AN_PRUDUCTNAME,m_strID);

		temp_variant=m_pRs->GetCollect("Product_class_ID");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_FlexGrid.put_TextMatrix(temp_row,AN_PRODUCTTYPE,m_strID);
		
		temp_variant=m_pRs->GetCollect("Product_ID");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_FlexGrid.put_TextMatrix(temp_row,AN_PRODUCTID,m_strID);

		temp_variant=m_pRs->GetCollect("Screen_Name");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_FlexGrid.put_TextMatrix(temp_row,AN_SCREENID,m_strID);



		temp_variant=m_pRs->GetCollect("Bautrate");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_FlexGrid.put_TextMatrix(temp_row,AN_BAUDRATE,m_strID);

		temp_variant=m_pRs->GetCollect("Background_imgID");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_FlexGrid.put_TextMatrix(temp_row,AN_GRAPHICID,m_strID);
		

		temp_variant=m_pRs->GetCollect("Hardware_Ver");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_FlexGrid.put_TextMatrix(temp_row,AN_HDVERSION,m_strID);
		

		temp_variant=m_pRs->GetCollect("Software_Ver");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_FlexGrid.put_TextMatrix(temp_row,AN_SWVERSION,m_strID);

		temp_variant=m_pRs->GetCollect("EPsize");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_FlexGrid.put_TextMatrix(temp_row,AN_EPSIZE,m_strID);
		m_pRs->MoveNext();
	}
	m_pRs->Close();
}
void CAllNodesDiaolg::SetBuildingMainName(CString strBuildName)
{
m_strMainBuildingName=strBuildName;
}


void CAllNodesDiaolg::OnBnClickedDelbutton()
{

	if(m_nCurRow==0||m_nCurRow==m_FlexGrid.get_Rows()-1)
	{
		AfxMessageBox(_T("Please select a item first!"));
		return;
	}
	CString strText;
	strText=m_FlexGrid.get_TextMatrix(m_nCurRow,AN_SerialID);
	if(strText.IsEmpty())
	{
		AfxMessageBox(_T("No item Selected!"));
	}
	CString strSql;
	strSql.Format(_T("delete * from ALL_NODE where Serial_ID ='%s'"),strText);
	CString strTemp;
	strTemp.Format(_T("Are you sure to delete thise item"));
	if(AfxMessageBox(strTemp,MB_OKCANCEL)==IDOK)
	{
		try
		{
			m_pCon->Execute(strSql.GetString(),NULL,adCmdText);	
		}
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}
	}
	ReloadAddBuildingDB();
	m_bChanged=TRUE;
}

void CAllNodesDiaolg::OnBnClickedDelallbutton()
{
	CString strSql;
	strSql=_T("delete * from ALL_NODE ");
	CString strTemp;
	strTemp.Format(_T("Are you sure to delete all the sub node(s)"));
	if(AfxMessageBox(strTemp,MB_OKCANCEL)==IDOK)
	{
		m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
	}
	ReloadAddBuildingDB();
	m_bChanged=TRUE;
}

void CAllNodesDiaolg::OnBnClickedExitbutton()
{
#if 1

    CAllNodesDiaolg::OnCancel();
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING,0,0);

#endif
	

}

void CAllNodesDiaolg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
BEGIN_EVENTSINK_MAP(CAllNodesDiaolg, CDialog)
	ON_EVENT(CAllNodesDiaolg, IDC_MSFLEXGRID1, DISPID_CLICK, CAllNodesDiaolg::ClickMsflexgrid1, VTS_NONE)
END_EVENTSINK_MAP()






void CAllNodesDiaolg::ClickMsflexgrid1()
{

}

void CAllNodesDiaolg::OnCbnSelchangeCombo1()
{
	CString strSelect;
	int nIdext=m_SubLstCombox.GetCurSel();
	if(nIdext>=0)
	{
		m_SubLstCombox.GetLBText(nIdext,strSelect);
		m_strSubNetName=strSelect;
		ReloadAddBuildingDB();
	}
}

void CAllNodesDiaolg::OnEnKillfocusTextedit()
{
	if(m_nCurRow==0)
		return;
}

void CAllNodesDiaolg::OnEnSetfocusTextedit()
{
	// TODO: Add your control notification handler code here
}

BOOL CAllNodesDiaolg::PreTranslateMessage(MSG* pMsg)
{
	return CDialog::PreTranslateMessage(pMsg);
}


void CAllNodesDiaolg::OnBnClickedAddbutton()
{
	/*alex*/
#if 1
	CString strSql;
	int nMaxRowIndext=m_FlexGrid.get_Rows()-1;

	//CString strSBuildingNameTemp;
	//strSBuildingNameTemp.Format(_T("Subnet_%d"), nMaxRowIndext);	

	// main building name
	CString strMainBuildName=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_MAINNAME);
	if(strMainBuildName.IsEmpty())
	{
		strMainBuildName = m_strMainBuildingName;
		//AfxMessageBox(_T("Input the building info in the last row of grid, the Main Building can not be empty."));
		//return ;
	}
	// sub net name
	CString strSubBuildingName=m_FlexGrid.get_TextMatrix(nMaxRowIndext,AN_NAME);
	if(strSubBuildingName.IsEmpty())
	{
		strSubBuildingName = m_strSubNetName;
	}
	// serial ID
	CString strSID;
	strSID=m_FlexGrid.get_TextMatrix(nMaxRowIndext,AN_SerialID);
	if(strSID.IsEmpty())
	{
		AfxMessageBox(_T("The Serial ID can not be empty, please input."));
		return ;
	}
	// Floor name
	CString strFloorName;
	strFloorName=m_FlexGrid.get_TextMatrix(nMaxRowIndext,AN_FLOORNAME);
	if(strFloorName.IsEmpty())
	{
		strFloorName.Format(_T("Floor_%d"), nMaxRowIndext);
	}
	// room name
	CString strRoomName;
	strRoomName=m_FlexGrid.get_TextMatrix(nMaxRowIndext,AN_ROOMNAME);
	if(strRoomName.IsEmpty())
	{
		strRoomName.Format(_T("Room_%d"), nMaxRowIndext);
	}

	// PRODUCT NAME
	CString strProName;
	strProName=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_PRUDUCTNAME);
	if(strProName.IsEmpty())
	{
		strProName=_T("New device");
	}


	// product type 
	CString strProType;
	strProType=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_PRODUCTTYPE);
	if(strProType.IsEmpty())
	{
		AfxMessageBox(_T("The Product type can not be empty, please input."));
		return ;
	}

	// product ID
	CString strProID;
	strProID=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_PRODUCTID);
	if(strProID.IsEmpty())
	{
		AfxMessageBox(_T("The Product ID can not be empty, please input."));
		return ;
	}


	// SCREEN ID
	CString strScreenID;
	strScreenID=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_SCREENID);
	if(strScreenID.IsEmpty())
	{		
		strScreenID = _T("Screen(S:") + strSID +_T("--") + strProID + _T(")");
	}

	// baud rate 
	CString strBaudrate;
	strBaudrate=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_BAUDRATE);
	if(strBaudrate.IsEmpty())
	{		
		int nType = _wtoi(strProType);
		if ( nType == 100 )  // NC
		{
			strBaudrate = _T("192.168.0.3");
		}
		else
		{
			strBaudrate = _T("19200");
		}		
	}

	// GRAPHIC ID 
	CString strGraphicID;
	strGraphicID=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_GRAPHICID);
	if(strGraphicID.IsEmpty())
	{		
		strGraphicID = _T("Clicking here to add a image");
	}

	// Hard VERSION
	CString strHdVersion;
	strHdVersion=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_HDVERSION);
	if(strHdVersion.IsEmpty())
	{		
		strHdVersion = _T("0");
	}
	// soft VERSION
	CString strStVersion;
	strStVersion=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_SWVERSION);
	if(strStVersion.IsEmpty())
	{		
		strStVersion = _T("0");
	}

	// EP Size
	CString strEPSize;
	strEPSize=m_FlexGrid.get_TextMatrix(nMaxRowIndext, AN_EPSIZE);
	if(strEPSize.IsEmpty())
	{		
		strEPSize = _T("0");
	}

	CString strCom = _T("0");

	strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize) values('"+strMainBuildName+"','"+strSubBuildingName +"','"+strSID+"','"+strFloorName+"','"+strRoomName+"','"+strProName+"','"+strProType+"','"+strProID+"','"+strScreenID+"','"+strBaudrate+"','"+strGraphicID+"','"+strHdVersion+"','"+strStVersion+"','"+strCom+"','"+strEPSize+"')"));
	//new nc//strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize,Mainnet_info) values('"+strMainBuildName+"','"+strSubBuildingName +"','"+strSID+"','"+strFloorName+"','"+strRoomName+"','"+strProName+"','"+strProType+"','"+strProID+"','"+strScreenID+"','"+strBaudrate+"','"+strGraphicID+"','"+strHdVersion+"','"+strStVersion+"','"+strCom+"','"+strEPSize+"','"+strMainnetInfo+"')"));
	try
	{



		TRACE(strSql);

		m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	ReloadAddBuildingDB();
#endif
}