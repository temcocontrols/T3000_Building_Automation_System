// ARDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "ARDDlg.h"
#include "afxdialogex.h"
#include "ado/ADO.h"

// CARDDlg dialog

IMPLEMENT_DYNAMIC(CARDDlg, CDialogEx)

CARDDlg::CARDDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CARDDlg::IDD, pParent)
{

}

BOOL CARDDlg::OnInitDialog(){
CDialogEx::OnInitDialog();
m_ipaddress.SetWindowText(_T("97.77.120.35"));
m_porteditor.SetWindowText(_T("502"));
return TRUE; 
}
CARDDlg::~CARDDlg()
{
}

void CARDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ipaddress);
	DDX_Control(pDX, IDC_PORT, m_porteditor);
}


BEGIN_MESSAGE_MAP(CARDDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CARDDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CARDDlg message handlers


void CARDDlg::OnBnClickedOk()
{
   CString ip;
   CString strport;
   m_ipaddress.GetWindowText(ip);
   m_porteditor.GetWindowText(strport);
   if (ip.IsEmpty()||strport.IsEmpty())
   {
	   AfxMessageBox(_T("ip or port can not be empty!"));
	   return;
   }
   short port=_wtoi(strport);
   SetCommunicationType(1);
   BOOL is_open=Open_Socket2(ip,port);
   if (is_open)
   {
       //unsigned short data[2];
	   //int ModelID=Read_Multi(209,data,6,2);//  (255,6,20);
	   //CString show;
	   //show.Format(_T("Add=%d,Model=%d"),data[0],data[1]);
	   //ModelID=read_one(data[0],7);
	   //AfxMessageBox(show);
	   //show.Format(_T("Model=%d"),ModelID);
	   //AfxMessageBox(show);
	  unsigned short Data[10];
	  int ret=Read_Multi(255,Data,0,10);
	  if (ret<0)
	  {
	  AfxMessageBox(_T("Can't get the information of your device!\n Please try again."));
	  ret;
	  }

	   CString strSql;
	   CString strMainBuildName,strSubBuildingName,strSID,strFloorName,strRoomName,strProID;
	   CString strProName,strProType,strScreenID,strBaudrate,strGraphicID,strHdVersion,strStVersion,strCom,strEPSize;

	  
	   
	  /* CString temp_str=_T("select * from Building where Default_SubBuilding=-1");
	   m_pRs->Open(_variant_t(temp_str),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);	
	   int recordcount=m_pRs->GetRecordCount();
	   if(m_pRs->GetRecordCount()<=0)
	   {
		   AfxMessageBox(_T("There is no default building,please select a building First."));
		   return;
	   }


	   if(m_pRs->State)
		   m_pRs->Close(); 
	   if(m_pCon->State)
           m_pCon->Close();*/
	   CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	   strMainBuildName= pFrame->m_strCurMainBuildingName;
	   strSubBuildingName= pFrame->m_strCurSubBuldingName;
	   int SN=Data[3]*256*256*256+Data[2]*256*256+Data[1]*256+Data[0];
       strSID.Format(_T("%d"),SN);
	   strFloorName=_T("floor1");
	   strRoomName=_T("room1");
	   strProName.Format(_T("%s:%d--%d"),GetProductName(Data[7]).GetBuffer(),SN,Data[6]);
	   strProType.Format(_T("%d"),Data[7]);
	   strProID.Format(_T("%d"),Data[6]);

	   strScreenID.Format(_T("Screen(S:%d--%d)"),SN,Data[6]);
	   m_ipaddress.GetWindowText(strBaudrate);
	   strGraphicID=_T("Clicking here to add a image...");
	   strHdVersion=_T("0.0");
	   strStVersion=_T("0.0");
	   m_porteditor.GetWindowText(strCom);
	   strEPSize=_T("0.0");
	   try
	   {
		   CADO ado;
		   ado.OnInitADOConn();
           strSql.Format(_T("Select * from ALL_NODE where Product_name='%s'"),strProName.GetBuffer());
           ado.m_pRecordset=ado.OpenRecordset(strSql);
           if (!ado.m_pRecordset->EndOfFile)//有表但是没有对应序列号的值
           {
            AfxMessageBox(_T("Already Exists!"));
            return;
            //ado.m_pRecordset->Close();
           }
		   strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize) values('"
			   +strMainBuildName+"','"
			   +strSubBuildingName +"','"
			   +strSID+"','"
			   +strFloorName+"','"
			   +strRoomName+"','"
			   +strProName+"','"
			   +strProType+"','"
			   +strProID+"','"
			   +strScreenID+"','"
			   +strBaudrate+"','"
			   +strGraphicID+"','"
			   +strHdVersion+"','"
			   +strStVersion+"','"
			   +strCom+"','"
			   +strEPSize+"')"));
		   ado.m_pRecordset=ado.OpenRecordset(strSql);
		   ado.CloseConn();
		   AfxMessageBox(_T("Add OK!"));
           CDialogEx::OnOK();
	   }
	   
	   catch (CException* e)
	   {
		   AfxMessageBox(_T("Add Fail!"));
	   }
	  
	   
   }
   else
   {
	   AfxMessageBox(_T("Can not Connect"));
   }

}
