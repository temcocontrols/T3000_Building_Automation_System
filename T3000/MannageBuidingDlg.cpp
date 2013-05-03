// MannageBuidingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "MannageBuidingDlg.h"
#include "AddBuilding.h"


//#include "SubBuildingDlg.h"
// CMannageBuidingDlg 对话框

IMPLEMENT_DYNAMIC(CMannageBuidingDlg, CDialog)
CMannageBuidingDlg::CMannageBuidingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMannageBuidingDlg::IDD, pParent)
{
	m_strDefBuildingName.Empty();
	m_nCurRow=-1;
	m_nCurCol=-1;
	m_nTotalItem=0;
}

CMannageBuidingDlg::~CMannageBuidingDlg()
{

}

void CMannageBuidingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DELBUTTON, m_DelBtn);
	DDX_Control(pDX, IDC_ADDBUTTON, m_AddBtn);
//	DDX_Control(pDX, IDC_INPUTBUTTON, m_InputBtn);
	DDX_Control(pDX, IDC_DEFAULTBUILDING_CHECK, m_defaultBuildignCheck);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_FlexGrid);
	DDX_Control(pDX, IDC_EDIT1, m_textEdit);
}


BEGIN_MESSAGE_MAP(CMannageBuidingDlg, CDialog)
	ON_BN_CLICKED(IDC_DELBUTTON, OnBnClickedDelbutton)
	ON_BN_CLICKED(IDC_ADDBUTTON, OnBnClickedAddbutton)
//	ON_BN_CLICKED(IDC_INPUTBUTTON, OnBnClickedInputbutton)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_NEXT, OnBnClickedNext)
	ON_BN_CLICKED(IDOK, &CMannageBuidingDlg::OnBnClickedOk)
	ON_COMMAND(ID_BUILDINGSEL_SELECT,OnMainBuildingSelect)
	ON_COMMAND(ID_BUILDINGSEL_UNSLECT,OnMainBuildingUnSelect)



	ON_BN_CLICKED(IDC_DEFAULTBUILDING_CHECK, &CMannageBuidingDlg::OnBnClickedDefaultbuildingCheck)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CMannageBuidingDlg::OnEnKillfocusEdit1)
END_MESSAGE_MAP()


// CMannageBuidingDlg 消息处理程序

void CMannageBuidingDlg::OnBnClickedDelbutton()
{
	if(m_nCurRow==0||m_nCurRow>=m_FlexGrid.get_Rows()-1)
	{
		AfxMessageBox(_T("Please Select a building name!"));
		return;
	}

	try
	{

	CString strSql;

	strSql.Format(_T("delete *  from Building_ALL where Building_Name='%s'"), m_strMainBuiding);
	m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	reLoadMainBuilding();
}

void CMannageBuidingDlg::OnBnClickedAddbutton()
{
	CString strSql;
	int nMaxRowIndext=m_FlexGrid.get_Rows()-1;
	CString strBuildingName=_T("");
	strBuildingName=m_FlexGrid.get_TextMatrix(nMaxRowIndext,1);
	if(strBuildingName.IsEmpty())
	{
		AfxMessageBox(_T("The new Main building's NAME can not be empty, please input."));
		return ;
	}
	try
	{

	m_strMainBuiding=strBuildingName;
	strSql.Format(_T("insert into Building_ALL (Building_Name,Telephone ,Address) values('"+m_strMainBuiding+"','"+m_strTelPhone+"','"+m_strAddress+"')"));
	m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	Update_Recorder();

}

/*

void CMannageBuidingDlg::OnBnClickedInputbutton()
{
	// TODO: 在此添加控件通知处理程序代码

	
	CString strSql;
	CString strName;
	m_NameEdt.GetWindowText(strName);
	

	if(m_bIsAddOrModify)//add
	{

		if(strName.IsEmpty())
		{
			AfxMessageBox(_T("Please input the building name!"));
			return;
		}
		int nCouts;
		nCouts=m_BuildingNameLst.GetCount();
		if(nCouts>0)
		{
			for(int i=0;i<nCouts;i++)
			{
				CString strBuildingname;
				m_BuildingNameLst.GetText(i,strBuildingname);
				if(strName.CompareNoCase(strBuildingname)==0)
				{
					AfxMessageBox(_T("The building has been existed,please input another one!"));
					return;
				}
			}
		}

		strSql.Format(_T("insert into Building_ALL (Building_Name) values('%s')"), strName);
		m_pCon->Execute(strSql.GetString(),NULL,adCmdText);			
	//	m_pRs->Open("select * from Building_ALL",_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
		m_BuildingNameLst.AddString(strName);

		m_NameEdt.ShowWindow(SW_HIDE);
		m_InputBtn.ShowWindow(SW_HIDE);
	}
	else
	{
		if(strName.IsEmpty())
		{
			AfxMessageBox(_T("Please input the building name!"));
			return;
		}
		int nIndext;
		nIndext=m_BuildingNameLst.GetCurSel();
		if(nIndext<0)
		{
			AfxMessageBox(_T("Please select the building name which will be changed"));
			return;
		}
		CString strBuildingname;
		m_BuildingNameLst.GetText(nIndext,strBuildingname);
		if(strName.CompareNoCase(strBuildingname)==0)
		{
			AfxMessageBox(_T("The building has been existed,please input another one!"));
			return;
		}

		strSql.Format(_T("update Building_ALL set Building_Name ='%s' where Building_Name='%s'"), strName,strBuildingname);
		m_pCon->Execute(strSql.GetString(),NULL,adCmdText);			
		m_BuildingNameLst.DeleteString(nIndext);
		m_BuildingNameLst.AddString(strName);

		m_NameEdt.ShowWindow(SW_HIDE);
		m_InputBtn.ShowWindow(SW_HIDE);
		

	}


	
}*/
		

void CMannageBuidingDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
BOOL CMannageBuidingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_defaultBuildignCheck.ShowWindow(SW_HIDE);

	m_FlexGrid.put_Cols(4);	

	m_FlexGrid.put_TextMatrix(0,1,_T("Main Building"));
	m_FlexGrid.put_TextMatrix(0,2,_T("TelePhone"));
	m_FlexGrid.put_TextMatrix(0,3,_T("Address"));

	m_FlexGrid.put_ColWidth(0,1400);
	m_FlexGrid.put_ColWidth(1,1200);
	m_FlexGrid.put_ColWidth(2,1200);
	m_FlexGrid.put_ColWidth(3,2000);

	m_bIsAddOrModify=true;

//	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
	m_pCon.CreateInstance(_T("ADODB.Connection"));
	m_pRs.CreateInstance(_T("ADODB.Recordset"));
	m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);

	m_textEdit.ShowWindow(SW_HIDE);
	reLoadMainBuilding();
	return true;
}

void CMannageBuidingDlg::OnBnClickedNext()
{
	// TODO: 在此添加控件通知处理程序代码

	if(m_mainBuildingLst.size()<=0)
		return;
	int lRow = m_FlexGrid.get_RowSel();//获取点击的行号	
	if(lRow<=0)
		return;
	if(lRow==m_FlexGrid.get_Rows()-1)
		return;
	//AfxMessageBox(_T("Please select building name first!"));
	CString strBuildingname;
	strBuildingname=m_mainBuildingLst.at(lRow-1).strMainName;
	if(strBuildingname.IsEmpty())
		return;
	CLOSE_DATABASE
	CAddBuilding Dlg;
	Dlg.SetBuildingMainName(strBuildingname);
	Dlg.DoModal();
	OnCancel();
}

void CMannageBuidingDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}


void CMannageBuidingDlg::OnBnClickedDefaultbuildingCheck()
{

	
	if(m_defaultBuildignCheck.GetCheck()==BST_UNCHECKED)
	{
		if(m_nCurRow==0||m_nCurRow>=m_FlexGrid.get_Rows()-1)
		{
			//m_defaultBuildignCheck.SetCheck(BST_CHECKED)
			return;
		}
		try
		{

		CString execute_str=_T("update Building_ALL set Default_Build = 0 where Default_Build = -1");
		m_pCon->Execute(execute_str.GetString(),NULL,adCmdText);
		}
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}	
	}

	if(m_defaultBuildignCheck.GetCheck()==BST_CHECKED)
	{
		if(m_nCurRow==0||m_nCurRow>=m_FlexGrid.get_Rows()-1)
		{
			m_defaultBuildignCheck.SetCheck(BST_UNCHECKED);
			return;
		}
		try
		{

		CString execute_str=_T("update Building_ALL set Default_Build = 0 where Default_Build = -1");
		m_pCon->Execute(execute_str.GetString(),NULL,adCmdText);	
		execute_str.Format(_T("update Building_ALL set Default_Build = -1 where Building_Name = '%s'"),m_strMainBuiding);
		m_pCon->Execute(execute_str.GetString(),NULL,adCmdText);
		}
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}
	}
	reLoadMainBuilding();
}
void CMannageBuidingDlg::reLoadMainBuilding()
{
	m_pRs->Open(_T("select * from Building_ALL"),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
	CString strBuldingName;
	m_nTotalItem=m_pRs->RecordCount;
	m_FlexGrid.put_Rows(m_pRs->RecordCount+2);	
	int temp_row=0;
	CString str_temp=_T("");
	str_temp.Empty();
	_variant_t temp_variant;
	MAINBUILDING buidingItem;
	while(DB_SELECT_FOUND)
	{
		++temp_row;
		str_temp=m_pRs->GetCollect(_T("Building_Name"));
		m_FlexGrid.put_TextMatrix(temp_row,1,str_temp);
		buidingItem.strMainName=str_temp;

		int bdef=0;
		bdef=m_pRs->GetCollect(_T("Default_Build"));
		if(bdef==-1)//def building;
		{
			m_strDefBuildingName=str_temp;
			m_FlexGrid.put_TextMatrix(temp_row,0,_T("Default"));
			CString strdefCeckText;
			strdefCeckText.Format(_T("Set %s as a default building when T3000 startup"),m_strDefBuildingName);
			m_defaultBuildignCheck.SetWindowText(strdefCeckText);

		}
		else
		{
			m_FlexGrid.put_TextMatrix(temp_row,0,_T(""));

		}
		buidingItem.bActive=bdef;
		temp_variant=m_pRs->GetCollect("Telephone");//
		if(temp_variant.vt!=VT_NULL)
			str_temp=temp_variant;
		else
			str_temp=_T("");

		m_FlexGrid.put_TextMatrix(temp_row,2,str_temp);

		buidingItem.strTelePhone=str_temp;
		temp_variant=m_pRs->GetCollect("Address");//
		if(temp_variant.vt!=VT_NULL)
			str_temp=temp_variant;
		else
			str_temp=_T("");

		buidingItem.strAddress=str_temp;
		m_FlexGrid.put_TextMatrix(temp_row,3,str_temp);

		m_mainBuildingLst.push_back(buidingItem);
		m_pRs->MoveNext();
	}
		++temp_row;	
		m_FlexGrid.put_TextMatrix(temp_row,1,_T(""));
		m_FlexGrid.put_TextMatrix(temp_row,2,_T(""));
		m_FlexGrid.put_TextMatrix(temp_row,3,_T(""));
	if(m_pRs->State) 
		m_pRs->Close(); 

}BEGIN_EVENTSINK_MAP(CMannageBuidingDlg, CDialog)
ON_EVENT(CMannageBuidingDlg, IDC_MSFLEXGRID1, DISPID_CLICK, CMannageBuidingDlg::ClickMsflexgrid1, VTS_NONE)
END_EVENTSINK_MAP()

void CMannageBuidingDlg::ClickMsflexgrid1()
{
	long lRow,lCol;
	lRow = m_FlexGrid.get_RowSel();//获取点击的行号	
	lCol = m_FlexGrid.get_ColSel(); //获取点击的列号

	m_nCurRow=lRow;
	m_nCurCol=lCol;
	m_strMainBuiding=m_FlexGrid.get_TextMatrix(lRow,1);
	m_strTelPhone=m_FlexGrid.get_TextMatrix(lRow,2);
	m_strAddress=m_FlexGrid.get_TextMatrix(lRow,3);
		
	if(m_strMainBuiding.CompareNoCase(m_strDefBuildingName)!=0)
	{
		m_defaultBuildignCheck.SetCheck(BST_UNCHECKED);
	}
	else
	{
		m_defaultBuildignCheck.SetCheck(BST_CHECKED);
	}
	
	CString strdefCeckText;
	strdefCeckText.Format(_T("Set %s as a default building when T3000 startup"),m_strMainBuiding);
	m_defaultBuildignCheck.SetWindowText(strdefCeckText);


	CRect rect;
	m_FlexGrid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	CDC* pDC =GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_FlexGrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_FlexGrid.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_FlexGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_FlexGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rcCell(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rcCell.OffsetRect(rect.left+1,rect.top+1);
	ReleaseDC(pDC);
	CString strValue = m_FlexGrid.get_TextMatrix(lRow,lCol);
	if(0==lCol)
	{
		//ID_BUILDINGSEL_SELECT
		CMenu menu;
		menu.LoadMenu(IDR_BUILDINGSELE_MENU);
		CMenu *pmenu=menu.GetSubMenu(0);
		CPoint point;
		GetCursorPos(&point);
		pmenu->TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN ,point.x,point.y,this);	

	}
	if(1==lCol||2==lCol||3==lCol)
	{
		//m_AddBuiding_FlexGrid.get_RowPos(lRow);
		m_textEdit.MoveWindow(&rcCell,1);
		m_textEdit.ShowWindow(SW_SHOW);
		m_textEdit.SetWindowText(strValue);
		m_textEdit.SetFocus();
		int nLenth=strValue.GetLength();
		m_textEdit.SetSel(0,nLenth); //全选//
	}
}

void CMannageBuidingDlg::OnEnKillfocusEdit1()
{

	if(m_nCurRow==0)
		return;
	CString strText;
	m_textEdit.GetWindowText(strText);
	strText.Trim();
	m_textEdit.ShowWindow(SW_HIDE);
	if (m_nCurRow==m_FlexGrid.get_Rows()-1)
	{
		if(m_nCurCol==1)
		{
			if(m_strMainBuiding.CompareNoCase(strText)==0)
				return;
			CString strTemp;
			for(UINT i=0;i<m_mainBuildingLst.size();i++)		//modify by fance.
			//for(UINT i=0;i++;i<m_mainBuildingLst.size())
			{
				strTemp=m_mainBuildingLst.at(i).strMainName;
				if(strText.CompareNoCase(strTemp)==0)
				{
					AfxMessageBox(_T("The building Name has exist,please change another one!"));
					return;

				}
			}
				m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
				m_strMainBuiding=strText;
				//m_strBuilding_Name2=strText;
				
				
		}
		if(m_nCurCol==2)
		{
			m_strTelPhone=strText;
		}
		if(m_nCurCol==3)
		{
			m_strAddress=strText;
		}
		m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
		
	}
	else
	{
		if(m_nCurCol==1)
		{
			if(m_strMainBuiding.CompareNoCase(strText)==0)
				return;
			CString strTemp;
			for(UINT i=0;i<m_mainBuildingLst.size();i++)		//modify by fance.
			//for(int i=0;i++;i<m_mainBuildingLst.size())
			{
				strTemp=m_mainBuildingLst.at(i).strMainName;
				if(strText.CompareNoCase(strTemp)==0)
				{
					AfxMessageBox(_T("The building Name has exist,please change another one!"));
					return;

				}
			}
				
				m_strMainBuiding=strText;
				//m_strBuilding_Name2=strText;
				
				
		}
		if(m_nCurCol==2)
		{
			m_strTelPhone=strText;
		}
		if(m_nCurCol==3)
		{
			m_strAddress=strText;
		}
		m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
	
		Update_Recorder();
	}
		
	
}
void CMannageBuidingDlg::Update_Recorder()
{
	return;
	
	try
	{

	CString strSql;
	strSql.Format(_T("update Building_ALL set Building_Name='%s',Telephone='%s',Address='%s'"),m_strMainBuiding,m_strTelPhone,m_strAddress);
	m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);	
	m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
	if(m_pRs->State) 
		m_pRs->Close(); 	
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	reLoadMainBuilding();

}

void CMannageBuidingDlg::OnMainBuildingSelect()
{
	try
	{


	CString execute_str=_T("update Building_ALL set Default_Build = 0 where Default_Build = -1");
	m_pCon->Execute(execute_str.GetString(),NULL,adCmdText);	
	execute_str.Format(_T("update Building_ALL set Default_Build = -1 where Building_Name = '%s'"),m_strMainBuiding);
	m_pCon->Execute(execute_str.GetString(),NULL,adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	reLoadMainBuilding();
}
void CMannageBuidingDlg::OnMainBuildingUnSelect()
{
	try
	{


	CString execute_str=_T("update Building_ALL set Default_Build = 0 where Default_Build = -1");
	m_pCon->Execute(execute_str.GetString(),NULL,adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}	
	reLoadMainBuilding();
}
