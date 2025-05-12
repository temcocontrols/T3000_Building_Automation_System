// CDialogISPTOOL.cpp : implementation file
//

#include "stdafx.h"
#include "../T3000.h"
#include "CDialogISPTOOL.h"



// CCDialogISPTOOL dialog

IMPLEMENT_DYNAMIC(CCDialogISPTOOL, CDialog)

CCDialogISPTOOL::CCDialogISPTOOL(CWnd* pParent /*=NULL*/)
	: CDialog(CCDialogISPTOOL::IDD, pParent)
{

	pDialogInfo = NULL;
}

CCDialogISPTOOL::~CCDialogISPTOOL()
{
	if (pDialogInfo != NULL)
	{
		delete pDialogInfo;
		pDialogInfo = NULL;
	}

}

void CCDialogISPTOOL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID_ALLNODE, m_flexgridISP);
}


BEGIN_MESSAGE_MAP(CCDialogISPTOOL, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CCDialogISPTOOL::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_FLASH, &CCDialogISPTOOL::OnBnClickedButtonFlash)
END_MESSAGE_MAP()


// CCDialogISPTOOL message handlers

void CCDialogISPTOOL::OnBnClickedButtonSelect()
{
	if (pDialogInfo!=NULL)
	{
		delete pDialogInfo;
		pDialogInfo = NULL;
	}

	UpdateData();
	CString strFilter = _T("hex File;bin File|*.hex;*.bin|all File|*.*||");
	CFileDialog dlg(true,_T("hex"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
	dlg.DoModal();
	m_strHexFileName=dlg.GetPathName();
	CWnd* pEditFilePath = (CWnd*)GetDlgItem(IDC_EDIT_FILEPATHE);
	pEditFilePath->SetWindowText(m_strHexFileName);
	UpdateData(false);	
}

void CCDialogISPTOOL::OnBnClickedButtonFlash()
{	
	if ((m_strHexFileName.CompareNoCase(_T("")) == 0)||m_strHexFileName.GetLength()<5)//
	{
		CString str;
		str.Format(_T("Selects 'File path',Please! "));
		AfxMessageBox(str);
	}else 
	{
// 		CString str;
// 		str.Format(_T("Processing Information,please wait..."));
// 		AfxMessageBox(str,MB_DEFBUTTON1);//MB_ICONQUESTION
		try
		{
			if(pDialogInfo==NULL)
			{
				pDialogInfo = new CDialogInfo;
				pDialogInfo->Create(IDD_DIALOG_INFO,this);
				pDialogInfo->ShowWindow(SW_SHOW);
				pDialogInfo->CenterWindow();
			}

		}
		catch (...)//这个无效，当pDialogInfo->Create(IDC_STATIC_INFO,this);中的ID写错时，这个函数没有throw抛出错误，所以捕获不到
		{

		}
	}


}

BOOL CCDialogISPTOOL::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	m_strMainBuildingName= pFrame->m_strCurMainBuildingName;
	m_strSubNetName= pFrame->m_strCurSubBuldingName;


// 	m_InputTextEdt.ShowWindow(SW_HIDE);
// 	m_productCombox.ShowWindow(SW_HIDE);
// 	//GetDlgItem(IDC_ADDBUTTON)->ShowWindow(SW_HIDE);
// 
// 	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
// 	m_strMainBuildingName= pFrame->m_strCurMainBuildingName;
// 	m_strSubNetName= pFrame->m_strCurSubBuldingName;
// 	int k=0;
// 	for(UINT i=0;i<pFrame->m_subNetLst.size();i++)
// 	{
// 		if(m_strSubNetName.CompareNoCase(pFrame->m_subNetLst.at(i).strBuildingName)==0)
// 			k=i;
// 		m_SubLstCombox.AddString(pFrame->m_subNetLst.at(i).strBuildingName);
// 	}
// 	m_SubLstCombox.SetCurSel(k);
// 
// 	m_SubLstCombox.ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);

	m_pCon.CreateInstance(_T("ADODB.Connection"));
	m_pRs.CreateInstance(_T("ADODB.Recordset"));
	m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

	AllBuildingDBDisplay();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCDialogISPTOOL::AllBuildingDBDisplay()
{
#if 1
	try
	{
		m_flexgridISP.Clear();

		m_flexgridISP.put_TextMatrix(0,0,_T(""));
		m_flexgridISP.put_ColWidth(0,400);

		m_flexgridISP.put_TextMatrix(0,AN_MAINNAME,_T("Main Building"));
		m_flexgridISP.put_ColWidth(AN_MAINNAME,800);

		m_flexgridISP.put_TextMatrix(0,AN_NAME,_T("Sub Net"));
		m_flexgridISP.put_ColWidth(AN_NAME,800);

		m_flexgridISP.put_TextMatrix(0,AN_SerialID,_T("Serial ID"));
		m_flexgridISP.put_ColWidth(AN_SerialID,800);

		m_flexgridISP.put_TextMatrix(0,AN_FLOORNAME,_T("Floor"));
		m_flexgridISP.put_ColWidth(AN_FLOORNAME,800);

		m_flexgridISP.put_TextMatrix(0,AN_ROOMNAME,_T("Room"));
		m_flexgridISP.put_ColWidth(AN_ROOMNAME,800);

		m_flexgridISP.put_TextMatrix(0,AN_PRUDUCTNAME,_T("Product Name"));
		m_flexgridISP.put_ColWidth(AN_PRUDUCTNAME,1300);

		m_flexgridISP.put_TextMatrix(0,AN_PRODUCTTYPE,_T("Product ID"));
		m_flexgridISP.put_ColWidth(AN_PRODUCTTYPE,1300);

		m_flexgridISP.put_TextMatrix(0,AN_PRODUCTID,_T("Address"));
		m_flexgridISP.put_ColWidth(AN_PRODUCTID,700);

		m_flexgridISP.put_TextMatrix(0,AN_SCREENID,_T("Screen ID"));
		m_flexgridISP.put_ColWidth(AN_SCREENID,2000);

		m_flexgridISP.put_TextMatrix(0,AN_BAUDRATE,_T("Baud Rate"));
		m_flexgridISP.put_ColWidth(AN_BAUDRATE,1300);

		m_flexgridISP.put_TextMatrix(0,AN_GRAPHICID,_T("Graphic Name"));
		m_flexgridISP.put_ColWidth(AN_GRAPHICID,1800);

		m_flexgridISP.put_TextMatrix(0,AN_HDVERSION,_T("HardW_Ver"));
		m_flexgridISP.put_ColWidth(AN_HDVERSION,1300);

		m_flexgridISP.put_TextMatrix(0,AN_SWVERSION,_T("SoftW_Ver"));
		m_flexgridISP.put_ColWidth(AN_SWVERSION,1300);
		m_flexgridISP.put_TextMatrix(0,AN_EPSIZE,_T("EPSize"));
		m_flexgridISP.put_ColWidth(AN_EPSIZE,800);

	CString strSql;
	strSql.Format(_T("select * from ALL_NODE where MainBuilding_Name = '%s' ORDER BY Product_ID ASC"),m_strMainBuildingName);
	m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);			
	m_flexgridISP.put_Rows(m_pRs->RecordCount+2);	
	int temp_row=0;
	CString str_temp;
	str_temp.Empty();
	_variant_t temp_variant;
	while(VARIANT_FALSE==m_pRs->EndOfFile)
	{	

		//m_flexgridISP.put_TextMatrix(temp_row,AN_PRODUCTTYPE,m_strID);

		++temp_row;
		CString strIndex; strIndex.Format(_T("%d"), temp_row);
		m_flexgridISP.put_TextMatrix(temp_row,0,strIndex);

		m_flexgridISP.put_TextMatrix(temp_row,AN_MAINNAME,m_strMainBuildingName);
		m_strSubNetName=m_pRs->GetCollect("Building_Name");//
		m_flexgridISP.put_TextMatrix(temp_row,AN_NAME,m_strSubNetName);
		temp_variant=m_pRs->GetCollect("Serial_ID");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_flexgridISP.put_TextMatrix(temp_row,AN_SerialID,m_strID);

		temp_variant=m_pRs->GetCollect("Floor_name");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_flexgridISP.put_TextMatrix(temp_row,AN_FLOORNAME,m_strID);

		temp_variant=m_pRs->GetCollect("Room_name");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_flexgridISP.put_TextMatrix(temp_row,AN_ROOMNAME,m_strID);

		temp_variant=m_pRs->GetCollect("Product_name");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_flexgridISP.put_TextMatrix(temp_row,AN_PRUDUCTNAME,m_strID);

		temp_variant=m_pRs->GetCollect("Product_class_ID");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_flexgridISP.put_TextMatrix(temp_row,AN_PRODUCTTYPE,m_strID);

		temp_variant=m_pRs->GetCollect("Product_ID");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_flexgridISP.put_TextMatrix(temp_row,AN_PRODUCTID,m_strID);

		temp_variant=m_pRs->GetCollect("Screen_Name");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_flexgridISP.put_TextMatrix(temp_row,AN_SCREENID,m_strID);



		temp_variant=m_pRs->GetCollect("Bautrate");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_flexgridISP.put_TextMatrix(temp_row,AN_BAUDRATE,m_strID);

		temp_variant=m_pRs->GetCollect("Background_imgID");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_flexgridISP.put_TextMatrix(temp_row,AN_GRAPHICID,m_strID);


		temp_variant=m_pRs->GetCollect("Hardware_Ver");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_flexgridISP.put_TextMatrix(temp_row,AN_HDVERSION,m_strID);


		temp_variant=m_pRs->GetCollect("Software_Ver");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_flexgridISP.put_TextMatrix(temp_row,AN_SWVERSION,m_strID);

		temp_variant=m_pRs->GetCollect("EPsize");//
		if(temp_variant.vt!=VT_NULL)
			m_strID=temp_variant;
		else
			m_strID=_T("");
		m_flexgridISP.put_TextMatrix(temp_row,AN_EPSIZE,m_strID);
		m_pRs->MoveNext();
	}
	m_pRs->Close();


		}
		catch (...)
		{
			CString strerror;
			strerror.Format(_T("read all node from db?"));
			SetPaneString(2,strerror);

		}

#endif

}BEGIN_EVENTSINK_MAP(CCDialogISPTOOL, CDialog)
ON_EVENT(CCDialogISPTOOL, IDC_MSFLEXGRID_ALLNODE, DISPID_CLICK, CCDialogISPTOOL::ClickMsflexgridAllnode, VTS_NONE)
END_EVENTSINK_MAP()

void CCDialogISPTOOL::ClickMsflexgridAllnode()
{
	if (pDialogInfo!=NULL)
	{
		delete pDialogInfo;
		pDialogInfo = NULL;
	}
}
