// ManageAccountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "ManageAccountDlg.h"
#include "UserAcessSetDlg.h"

// CManageAccountDlg dialog

IMPLEMENT_DYNAMIC(CManageAccountDlg, CDialog)

CManageAccountDlg::CManageAccountDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManageAccountDlg::IDD, pParent)
{
	m_nCurRow=m_nCurCol=0;
}

CManageAccountDlg::~CManageAccountDlg()
{
	SqliteDBT3000.closedb();
}

void CManageAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_FlexGrid);
	DDX_Control(pDX, IDC_VALUEEDIT, m_valueEdit);
	DDX_Control(pDX, IDC_USEPRIVILEGECHECK, m_usePrivilegeCheck);
}


BEGIN_MESSAGE_MAP(CManageAccountDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CManageAccountDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_EXITBUTTON, &CManageAccountDlg::OnBnClickedExitbutton)
	ON_EN_KILLFOCUS(IDC_VALUEEDIT, &CManageAccountDlg::OnEnKillfocusValueedit)
	ON_BN_CLICKED(IDC_CONFIGBUTTON, &CManageAccountDlg::OnBnClickedConfigbutton)
	ON_BN_CLICKED(IDC_DELBUTTON, &CManageAccountDlg::OnBnClickedDelbutton)
	ON_BN_CLICKED(IDC_BUTTON1, &CManageAccountDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_USEPRIVILEGECHECK, &CManageAccountDlg::OnBnClickedUseprivilegecheck)
END_MESSAGE_MAP()


// CManageAccountDlg message handlers

void CManageAccountDlg::OnBnClickedOk()
{
	
	//OnOK();
}

void CManageAccountDlg::OnBnClickedExitbutton()
{
	
	CDialog::OnOK();
}

BOOL CManageAccountDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_valueEdit.ShowWindow(SW_HIDE);

	 SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
	CString strSql;
	strSql.Format(_T("select * from Userlogin"));
	q = SqliteDBT3000.execQuery((UTF8MBSTR)strSql);
	 _variant_t temp_variant;
	BOOL b_useLogin=false;
	int nTemp=0;
	while(!q.eof())
	{
		nTemp=q.getIntField("USE_PASSWORD");//
		 
		q.nextRow();
	}
	if(nTemp==-1)	
	{
		//m_ncPrivilegeCombx.SetCurSel(0);
		b_useLogin=TRUE;
		m_usePrivilegeCheck.SetCheck(BST_CHECKED);
	}
	else
	{
	//	m_ncPrivilegeCombx.SetCurSel(1);
		m_usePrivilegeCheck.SetCheck(BST_UNCHECKED);
		b_useLogin=FALSE;
	}
	 

	m_FlexGrid.put_ColWidth(1,1500);
	m_FlexGrid.put_ColWidth(2,1500);
	 

	ReloadUserDB();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CManageAccountDlg::ReloadUserDB()
{
	m_FlexGrid.Clear();
	m_AccountLst.clear();
		
	m_FlexGrid.put_ColAlignment(0,4);

	m_FlexGrid.put_TextMatrix(0,1,_T("User"));

	m_FlexGrid.put_TextMatrix(0,2,_T("Password"));
	q = SqliteDBT3000.execQuery("select * from users");
	table = SqliteDBT3000.getTable("select * from users");
	 
	 _variant_t temp_variant;
	int temp_row=0;
	CString strTemp;
	m_FlexGrid.put_Cols(3);
	

	m_FlexGrid.put_Rows(table.numRows()+2);	
	//for(int k=0;k++;k<3)
	//{
			//m_FlexGrid.put_Row(temp_row);m_FlexGrid.put_Col(k);
	//	m_FlexGrid.put_ColAlignment(k,4);
	//}m_AccountLst
	while(!q.eof())
	{	
		Account_Node Item;
		++temp_row;
		strTemp=q.getValuebyName(L"user_name_login");//
		 
		m_FlexGrid.put_ColAlignment(1,4);
		Item.strName=strTemp;
		m_FlexGrid.put_TextMatrix(temp_row,1,strTemp);

		strTemp=q.getValuebyName(L"password_login");//
		 
		m_FlexGrid.put_ColAlignment(2,4);
		Item.strpassword=strTemp;
		m_FlexGrid.put_TextMatrix(temp_row,2,strTemp);

		m_AccountLst.push_back(Item);
		q.nextRow();//
 
	}
	 


}
BEGIN_EVENTSINK_MAP(CManageAccountDlg, CDialog)
	ON_EVENT(CManageAccountDlg, IDC_MSFLEXGRID1, DISPID_CLICK, CManageAccountDlg::ClickMsflexgrid1, VTS_NONE)
END_EVENTSINK_MAP()

void CManageAccountDlg::ClickMsflexgrid1()
{
	long lRow,lCol;
	m_FlexGrid.put_TextMatrix(m_nCurRow,0,_T(""));
	lRow = m_FlexGrid.get_RowSel();//获取点击的行号	
	lCol = m_FlexGrid.get_ColSel(); //获取点击的列号
	m_nCurRow=lRow;
	m_nCurCol=lCol;

	m_strUserName=m_FlexGrid.get_TextMatrix(m_nCurRow,1);
	m_strPassword=m_FlexGrid.get_TextMatrix(m_nCurRow,2);

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
	if(m_nCurRow>0)
		m_FlexGrid.put_TextMatrix(m_nCurRow,0,_T("Select"));
	if(2==lCol||1==lCol)
	{
		//m_AddBuiding_FlexGrid.get_RowPos(lRow);
		m_valueEdit.MoveWindow(&rcCell,1);
		m_valueEdit.ShowWindow(SW_SHOW);
		m_valueEdit.SetWindowText(strValue);
		m_valueEdit.SetFocus();
	    int nLenth=strValue.GetLength();
		m_valueEdit.SetSel(nLenth,nLenth); //全选//
	}
}

void CManageAccountDlg::OnEnKillfocusValueedit()
{
	m_valueEdit.ShowWindow(SW_HIDE);
	if(m_nCurRow==0)
		return;
	CString strText;
	m_valueEdit.GetWindowText(strText);
	strText.Trim();
	if (m_nCurRow==m_FlexGrid.get_Rows()-1&&m_nCurRow>1)
	{

		if(m_nCurCol==1)
		{
			CString strTemp;
			for(UINT i=0;i<m_AccountLst.size();i++)		//modify by fance.
			//for(UINT i=0;i++;i<m_AccountLst.size())
			{
				strTemp=m_AccountLst.at(i).strName;
				if(strText.CompareNoCase(strTemp)==0)
				{
					AfxMessageBox(_T("The building Name has exist,please change another one!"));
					m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,_T(""));
					return;
				}
			}
			m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
		}
		if(m_nCurCol==2)
		{
			m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
		}
	}
	else
	{
		if(m_nCurCol==1)
		{
			if(m_nCurRow==1)
			{
				if(m_strUserName.CompareNoCase(strText)==0)
					return;
				else
				{
					AfxMessageBox(_T("You can not change admin account name!"));
					return;
				}
			}
			else
			{
				if(m_strUserName.CompareNoCase(strText)==0)
					return;
				CString strTemp;
				int n=m_AccountLst.size();
				for(UINT i=0;i<m_AccountLst.size();i++)
				{
					strTemp=m_AccountLst.at(i).strName;
					if(strText.CompareNoCase(strTemp)==0)
					{
						AfxMessageBox(_T("The building Name has exist,please change another one!"));
						return;
					}
				}
				m_strUserName=strText;
			//	m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,strText);

			}

			/*
			if(m_strUserName.CompareNoCase(strText)==0)
				return;
			CString strTemp;
			for(int i=0;i++;i<m_AccountLst.size())
			{
				strTemp=m_AccountLst.at(i).strName;
				if(strText.CompareNoCase(strTemp)==0)
				{
					AfxMessageBox(_T("The building Name has exist,please change another one!"));
					return;
				}
			}
				m_strUserName=strText;
				*/
				
		}
		if(m_nCurCol==2)
		{
			if(m_strPassword.CompareNoCase(strText)==0)
				return;
			m_strPassword=strText;
		}
		Update_Recorder();
	}

	
}
void CManageAccountDlg::Update_Recorder()
{
	
	try
	{

	CString strSql;
	CString strOldUser;
	strOldUser=m_AccountLst.at(m_nCurRow-1).strName;
	strSql.Format(_T("update users set user_name_login='%s',password_login='%s' where user_name_login='%s'"),m_strUserName,m_strPassword,strOldUser);
	 SqliteDBT3000.execDML((UTF8MBSTR)strSql);

	 
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	ReloadUserDB();
}
BOOL CManageAccountDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->wParam == VK_RETURN )
	{ 
		GetDlgItem(IDC_EXITBUTTON)->SetFocus();
		return true; 
	} 
	return CDialog::PreTranslateMessage(pMsg);
}

void CManageAccountDlg::OnBnClickedConfigbutton()
{
	if(m_nCurRow==0)
	{
		AfxMessageBox(_T("Please select a user first!"));
		return;
	}
	if(m_nCurRow==1)
	{
		AfxMessageBox(_T("'Admin account can access all the modules of device,need not config privileg."));
		return;
	}
	if(m_strUserName.IsEmpty())
	{
		AfxMessageBox(_T("Please select a user first!"));
		return;
	}
	if(m_nCurRow>=(m_FlexGrid.get_Rows()-1))
	{
		AfxMessageBox(_T("Please select a user first!"));
		return;
	}
	CUserAcessSetDlg Dlg;
	Dlg.SetConfigUserName(m_strUserName);
	Dlg.DoModal();
}

void CManageAccountDlg::OnBnClickedDelbutton()
{
	if(m_nCurRow==1)
	{
		AfxMessageBox(_T("'Admin' account can not be deleted!"));
		return;
	}

	else if(m_nCurRow>=(m_FlexGrid.get_Rows()-1) || m_nCurRow <1 )
	{
		AfxMessageBox(_T("Please select a user first!"));
		return;
	}
	//CString strText;
	//strText=m_FlexGrid.get_TextMatrix(m_nCurRow,m_nCurCol);
	if(m_strUserName.IsEmpty())
	{
		AfxMessageBox(_T("Please select a user first!"));
		return;
	}
	CString strSql;
	strSql.Format(_T("delete   from users where user_name_login='%s'"),m_strUserName);

	
	CString strTemp;
	strTemp.Format(_T("Are you sure to delete the user:'%s'"),m_strUserName);
	if(AfxMessageBox(strTemp,MB_OKCANCEL)==IDOK)
	{
		try
		{

		    SqliteDBT3000.execDML((UTF8MBSTR)strSql);
			 
		}
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}

	}
	ReloadUserDB();
}
void CManageAccountDlg::OnBnClickedButton1()
{
	
	int nLastRow=m_FlexGrid.get_Rows()-1;
	CString strUserName,strPassword;
	strUserName=m_FlexGrid.get_TextMatrix(nLastRow,1);
	strPassword=m_FlexGrid.get_TextMatrix(nLastRow,2);

	try
	{

	CString strSql;
	strSql.Format(_T("insert into users values('%s','%s')"),strUserName,strPassword);		
	SqliteDBT3000.execDML((UTF8MBSTR)strSql);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	ReloadUserDB();
}

void CManageAccountDlg::OnBnClickedUseprivilegecheck()
{
	 
	CString strSql;

	if(m_usePrivilegeCheck.GetCheck()==BST_UNCHECKED)
	{
		 strSql.Format(_T("update Userlogin set USE_PASSWORD = 0"));
	
	}
	else
	{
		 strSql.Format(_T("update Userlogin set USE_PASSWORD = -1"));
	
	}

	try
	{

		 
		SqliteDBT3000.execDML((UTF8MBSTR)strSql);
	 
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());

	}
}