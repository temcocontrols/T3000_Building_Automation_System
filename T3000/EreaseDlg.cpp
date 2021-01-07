// EreaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "EreaseDlg.h"
#include "global_function.h"
#include "../SQLiteDriver/CppSQLite3.h"

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
	
// 	if(AfxMessageBox(_T("Are you sure to erease the config parameters?"))==IDOK)
// 	{
    /*if(write_one(m_nTstatID,16,143)>0)
    	AfxMessageBox(_T("Ereased successfully!"));*/
        if (!is_connect ())
        {

             AfxMessageBox(_T("Please connect your device ,using modbus protocol!"));
             return ;
        }
     int protocol = GetCommunicationType ();
     Read_Multi (g_tstat_id,product_register_value,0,10);
    int times=5;
    CString newid;
    GetDlgItem(IDC_EDIT1)->GetWindowText(newid);
    int serialno=get_serialnumber();
    int ID=_wtoi(newid);
    if (newid!=g_tstat_id)
    {
        int ret = 0;
        if ((product_register_value[7] == PM_TSTAT9)||
            (product_register_value[7] == PM_TSTAT8))
        {
            int write_ret = 0;
            write_ret = write_one(g_tstat_id, 639, 0,5);
            Sleep(200);
        }

            ret = write_one(g_tstat_id, 6, ID);
            while (times >= 0 && ret<0)
            {
                Sleep(1000);
                ret = write_one(g_tstat_id, 6, ID);
                --times;
            }



        if (ret>0)
        {
			CppSQLite3DB SqliteDBBuilding;
			CppSQLite3Table table;
			CppSQLite3Query q;
			SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

            CString sql;
            sql.Format(_T("select * from ALL_NODE where Serial_ID='%d' "),serialno);
			q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);
			table = SqliteDBBuilding.getTable((UTF8MBSTR)sql);
            int Recount=table.numRows();
            if (Recount<=0)
            {
                CString strTips;
                strTips.Format(_T("Serial No=%d No Exsits!"),serialno);
                AfxMessageBox(strTips);
                return;
            }
            
            
                CString prodcut_name,product_id,screen_name;
                prodcut_name.Format(_T("%s:%d--%d"),GetProductName(product_register_value[7]),serialno,ID);
                product_id.Format(_T("%d"),ID);
                screen_name.Format(_T("Screen(S:%d--%d)"),serialno,ID);

				  try 
				  {
				 
					  CString strSql;
					  strSql.Format(_T("Update ALL_NODE set Product_name = '%s' ,Product_ID = '%s' ,Screen_Name = '%s'  where Serial_ID='%d' "),
					                 prodcut_name,
									  product_id,
									  screen_name,
									  serialno);
					
					SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
				  }
				  catch(...)
				  {


				  }
				  g_tstat_id=ID;

				  CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
				  ::PostMessage(pFrame->m_hWnd,WM_MYMSG_REFRESHBUILDING,0,0);
			 
			SqliteDBBuilding.closedb();
			} 
			else
			{
			AfxMessageBox(_T("Fail,Please try again!"));
			return;
			}
			} 
			else
			{
			AfxMessageBox(_T("The same ID ,you input!"));
			}
			

// 	}
// 	else
// 	{
// 		return;
// 	}
	
	OnOK();
}

BOOL CEreaseDlg::PreTranslateMessage(MSG* pMsg)
{
 
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

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
