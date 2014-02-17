// EreaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "EreaseDlg.h"
#include "globle_function.h"
#include "ado/ADO.h"
#include "MainFrm.h"

// CEreaseDlg dialog

IMPLEMENT_DYNAMIC(CEreaseDlg, CDialog)

CEreaseDlg::CEreaseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEreaseDlg::IDD, pParent)
{

}

CEreaseDlg::~CEreaseDlg()
{
}

void CEreaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_tstatIDEdit);
}


BEGIN_MESSAGE_MAP(CEreaseDlg, CDialog)
	ON_BN_CLICKED(ID_MYOK, &CEreaseDlg::OnBnClickedMyOk)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CEreaseDlg::OnEnKillfocusEdit1)
END_MESSAGE_MAP()


// CEreaseDlg message handlers

void CEreaseDlg::OnBnClickedMyOk()
{
	// TODO: Add your control notification handler code here
	if(AfxMessageBox(_T("Are you sure to erease the config parameters?"))==IDOK)
	{
		/*if(write_one(m_nTstatID,16,143)>0)
			AfxMessageBox(_T("Ereased successfully!"));*/
			CString newid;
			GetDlgItem(IDC_EDIT1)->GetWindowText(newid);
			int serialno=get_serialnumber();
			int ID=_wtoi(newid);
			if (newid!=g_tstat_id)
			{
			int ret=write_one(g_tstat_id,6,ID);
			if (ret>0)
			{
			  CADO ado;
			  ado.OnInitADOConn();
			  CString sql;
			  sql.Format(_T("select * from ALL_NODE where Serial_ID='%d' "),serialno);
			  ado.m_pRecordset = ado.OpenRecordset(sql);
			  ado.m_pRecordset->MoveFirst();
			  while(!ado.m_pRecordset->EndOfFile)
			  {
			      CString prodcut_name,product_id,screen_name;
				  prodcut_name.Format(_T("%s:%d--%d"),GetProductName(product_register_value[7]),serialno,ID);
				  product_id.Format(_T("%d"),ID);
				  screen_name.Format(_T("Screen(S:%d--%d)"),serialno,ID);

				  try 
				  {
					  ado.m_pRecordset->PutCollect("Product_name",(_bstr_t)(prodcut_name));
					  ado.m_pRecordset->PutCollect("Product_ID",(_bstr_t)(product_id));
					  ado.m_pRecordset->PutCollect("Screen_Name",(_bstr_t)(screen_name));
					  ado.m_pRecordset->Update();
					  ado.m_pRecordset->MoveNext();

				  }
				  catch(...)
				  {


				  }
				  g_tstat_id=ID;

				  CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
				  ::PostMessage(pFrame->m_hWnd,WM_MYMSG_REFRESHBUILDING,0,0);
			  }
			} 
			else
			{
			AfxMessageBox(_T("Fail,Please try again!"));
			}
			} 
			else
			{
			AfxMessageBox(_T("The same ID ,you input!"));
			}
			

	}
	else
	{
		return;
	}
	
	OnOK();
}

BOOL CEreaseDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->wParam == VK_RETURN )
	{ 
		//GetDlgItem(ID_MYOK)->SetFocus();
		OnBnClickedMyOk();
		return true; 
	} 
	return CDialog::PreTranslateMessage(pMsg);
}

void CEreaseDlg::OnEnKillfocusEdit1()
{
	CString strText;
	m_tstatIDEdit.GetWindowText(strText);
	if(strText.IsEmpty())
		return;
	int nID=_wtoi(strText);
	if(nID<=0||nID>255)
	{
		AfxMessageBox(_T("Error, the modbus ID should be in the rang from 0 to 255!"));
		strText.Format(_T("%d"),m_nTstatID);
		m_tstatIDEdit.SetWindowText(strText);
		return;
	}
	m_nTstatID=nID;

}

BOOL CEreaseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_nTstatID=g_tstat_id;
	CString strText;
	strText.Format(_T("%d"),m_nTstatID);
	m_tstatIDEdit.SetWindowText(strText);


	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
