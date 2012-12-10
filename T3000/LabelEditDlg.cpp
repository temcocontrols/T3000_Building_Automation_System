// LabelEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "LabelEditDlg.h"

#define START_ID 20000
// CLabelEditDlg dialog
#define INPUT_NUMBER 4
#define OUTPUT_NUMBER 7
//CString m_input[INPUT_NUMBER]={_T("Internal"),_T("Analog 1"),_T("Analog 2"),_T("Digital 1")};
CString m_input[INPUT_NUMBER]={_T("Internal Sensor"),_T("Input2"),_T("Input3"),_T("Input4")};
CString m_output[OUTPUT_NUMBER]={_T("Relay 1"),_T("Relay 2"),_T("Relay 3"),_T("Relay 4"),_T("Relay 5"),_T("Analog 1"),_T("Analog 2")};

IMPLEMENT_DYNAMIC(CLabelEditDlg, CDialog)

extern BOOL g_bRefresh;
CLabelEditDlg::CLabelEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLabelEditDlg::IDD, pParent)
{
	m_clrTxt=RGB(0,0,0);
	m_bkColor=RGB(224, 232, 246);
	m_strScreenName=_T("");
}

CLabelEditDlg::~CLabelEditDlg()
{
}

void CLabelEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXTBUTTON, m_textClorBtn);
	DDX_Control(pDX, IDC_BKCLORBTN, m_bkClrBtn);
	DDX_Control(pDX, IDC_IOCOMBOX, m_IOCombox);
	DDX_Control(pDX, IDC_STATUSCOMBO, m_statusComBox);
	DDX_Control(pDX, IDC_REGEDIT, m_regEdit);
	DDX_Control(pDX, IDC_WIDTHEDIT, m_widthEdit);
	DDX_Control(pDX, IDC_HEIGHTEDIT, m_heightEdt);
}


BEGIN_MESSAGE_MAP(CLabelEditDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLabelEditDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_OK, &CLabelEditDlg::OnBnNewClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLabelEditDlg::OnBnNewClickedCancel)
	ON_CBN_SELCHANGE(IDC_IOCOMBOX, &CLabelEditDlg::OnCbnSelchangeIocombox)
	ON_CBN_SELCHANGE(IDC_STATUSCOMBO, &CLabelEditDlg::OnCbnSelchangeStatuscombo)
	ON_EN_KILLFOCUS(IDC_REGEDIT, &CLabelEditDlg::OnEnKillfocusRegedit)
	ON_EN_KILLFOCUS(IDC_WIDTHEDIT, &CLabelEditDlg::OnEnKillfocusWidthedit)
	ON_EN_KILLFOCUS(IDC_HEIGHTEDIT, &CLabelEditDlg::OnEnKillfocusHeightedit)
	ON_BN_CLICKED(IDC_EXITBUTTON, &CLabelEditDlg::OnBnClickedExitbutton)
	ON_BN_CLICKED(IDC_BKCLORBTN, &CLabelEditDlg::OnBnClickedBkclorbtn)
	ON_BN_CLICKED(IDC_TEXTBUTTON, &CLabelEditDlg::OnBnClickedTextbutton)
END_MESSAGE_MAP()


// CLabelEditDlg message handlers

void CLabelEditDlg::OnBnClickedOk()
{
//	OnOK();
}
void CLabelEditDlg::OnBnNewClickedOk()
{
	

	g_bRefresh=TRUE;
	SaveToDb();
	CDialog::OnOK();
}

void CLabelEditDlg::OnBnNewClickedCancel()
{

	CDialog::OnCancel();
}
BOOL CLabelEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pCon.CreateInstance(_T("ADODB.Connection"));
	m_pRs.CreateInstance(_T("ADODB.Recordset"));
	m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);
	
	
	/*
	CString strSql;
	strSql.Format(_T("select * from Screen_Label where Serial_Num ='%d'and Screen_Name='%s'and Tstat_id=%i"),m_nSerialNumber,m_strScreenName,m_nTstatID);
	//strSql.Format(_T("select * from Screen_Label where Tstat_id=%i"),m_nTstatID);
	m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
	CString strtemp;
	strtemp.Empty();
	*/

//m_textClorBtn.EnableAutomaticButton(_T("Automatic"),m_clrTxt);
	//m_bkClrBtn.EnableAutomaticButton(_T("Automatic"), m_bkColor);
	CString strTemp;
	strTemp.Format(_T("%d"),m_id);
	GetDlgItem(IDC_IDEDIT)->SetWindowText(strTemp);
	strTemp.Format(_T("%d"),m_nSerialNum);
	GetDlgItem(IDC_SERIALEDIT)->SetWindowText(strTemp);
	strTemp.Format(_T("%d"),m_nwidth);
	GetDlgItem(IDC_WIDTHEDIT)->SetWindowText(strTemp);
	strTemp.Format(_T("%d"),m_nheight);
	GetDlgItem(IDC_HEIGHTEDIT)->SetWindowText(strTemp);
	if(m_input_or_output>=0&&m_input_or_output<=2)
		m_IOCombox.SetCurSel(m_input_or_output);
	OnCbnSelchangeIocombox();
	m_textClorBtn.EnableAutomaticButton(_T("Automatic"),m_clrTxt);
	m_textClorBtn.EnableOtherButton(_T("Other"));
	m_textClorBtn.SetColor((COLORREF)-1);
	m_textClorBtn.SetColumnsNumber(10);
	m_bkClrBtn.EnableAutomaticButton(_T("Automatic"), m_bkColor);
	m_bkClrBtn.EnableOtherButton(_T("Other"));
	m_bkClrBtn.SetColor((COLORREF)-1);
	m_bkClrBtn.SetColumnsNumber(10);
	/*
	if(m_input_or_output==2)
	{
		CString strTemp;
		strTemp.Format(_T("%d"),m_nstatus);
		

		m_regEdit.ShowWindow(SW_SHOW);
		m_statusComBox.ShowWindow(SW_HIDE);
	}
	else
	{

	}
	*/
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLabelEditDlg::SetInitValueForEdit(int id,int serialNum,CString strScreen,int idControl,int in_out_put,int nWidth,int nHeight,int nStatus,int clrText,int bkClr)
{
	m_nControlID=idControl;
	m_id=id;
	m_input_or_output=in_out_put;//1 == output ,0 == input
	m_nSerialNum=serialNum;
	m_nwidth=nWidth;//the cstatic width;
	m_nheight=nHeight;//the cstatic height
	m_nstatus=nStatus;//for input or output selection,such as Relay 1 ,Relay 2,Relay 3...
	m_clrTxt=clrText;
	m_bkColor=bkClr;
	m_strScreenName=strScreen;

}
void CLabelEditDlg::SetCommandType(int nType)
{
	m_nCommandType=nType;
}
void CLabelEditDlg::SaveEditValue()
{
	try
	{

	CString strSql;
	CString strclrTxt;
	CString strclrBk;
	strclrTxt.Format(_T("%u"),m_clrTxt);
	strclrBk.Format(_T("%u"),m_bkColor);
	strSql.Format(_T("update Screen_Label set Width=%i,Height=%i,Status=%i, Tips='%s',Input_or_Output=%i,Text_Color='%s',Back_Color='%s' where Serial_Num =%i and Tstat_id=%i and  Cstatic_id=%i"),
		m_nwidth,m_nheight,m_nstatus,_T(""),m_input_or_output,strclrTxt,strclrBk,m_nSerialNum,m_id,m_nControlID);
	m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
}
void CLabelEditDlg::AddLabel()
{

	try
	{

	CString strSql;
	strSql.Format(_T("select * from Screen_Label where Serial_Num =%i and Tstat_id=%i"),m_nSerialNum,m_id);
	m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
	CString strtemp;
	strtemp.Empty();
	_variant_t temp_variant;
	int nTemp;
	int nContyrolID=START_ID;
	while(VARIANT_FALSE==m_pRs->EndOfFile)
	{//find the ControlID;
		temp_variant=m_pRs->GetCollect("Cstatic_id");//
		if(temp_variant.vt!=VT_NULL)
			strtemp=temp_variant;
		else
			strtemp=_T("");
		nTemp=_wtoi(strtemp);
		if(nTemp==nContyrolID)
			nContyrolID=nContyrolID+1;
		else
			break;
		m_pRs->MoveNext();
	}
	if(m_pRs->State) 
		m_pRs->Close();


	CString strclrTxt;
	CString strclrBk;
	strclrTxt.Format(_T("%u"),m_clrTxt);
	strclrBk.Format(_T("%u"),m_bkColor);
	strSql.Format(_T("insert into Screen_Label values(%i,%i,%i,%i,%i,%i,%i,%i,'%s',%i,'%s','%s')"),nContyrolID,m_nSerialNum,50,50,m_nheight,m_nwidth,m_id,m_nstatus,m_strScreenName,m_input_or_output,strclrTxt,strclrBk);
	m_pCon->Execute(strSql.GetString(),NULL,adCmdText);


	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

}
void CLabelEditDlg::SaveToDb()
{
	if(m_nCommandType==1)
	{//add a new label
		AddLabel();
	}
	if(m_nCommandType==2)
	{
		//edit current label ;//update
		SaveEditValue();
	}
	if(m_pCon->State)
	m_pCon->Close();
}

void CLabelEditDlg::OnCbnSelchangeIocombox()
{
	m_statusComBox.ResetContent();
	m_input_or_output=m_IOCombox.GetCurSel();
	if(m_input_or_output==0)
	{
		GetDlgItem(IDC_REG_STATIC)->SetWindowText(_T("Input"));
		for(int i=0;i<INPUT_NUMBER;i++)
		{
			m_statusComBox.AddString(m_input[i]);
		}
		if(m_nstatus>=0&&m_nstatus<=7)
			m_statusComBox.SetCurSel(m_nstatus);
		m_regEdit.ShowWindow(SW_HIDE);
		m_statusComBox.ShowWindow(SW_SHOW);
	}
	if(m_input_or_output==1)
	{
		
		for(int i=0;i<OUTPUT_NUMBER;i++)
		{
			m_statusComBox.AddString(m_output[i]);
		}
	//	m_status_ctrl.SetCurSel(0);
		GetDlgItem(IDC_REG_STATIC)->SetWindowText(_T("Output"));
		if(m_nstatus>=0&&m_nstatus<=7)
			m_statusComBox.SetCurSel(m_nstatus);
			
		m_regEdit.ShowWindow(SW_HIDE);
		m_statusComBox.ShowWindow(SW_SHOW);
	}
	if(m_input_or_output==2)
	{
		CString strTemp;
		strTemp.Format(_T("%d"),m_nstatus);
		
		m_regEdit.SetWindowText(strTemp);
		m_regEdit.ShowWindow(SW_SHOW);
		m_statusComBox.ShowWindow(SW_HIDE);
	}

}

void CLabelEditDlg::OnCbnSelchangeStatuscombo()
{
	m_nstatus=m_statusComBox.GetCurSel();
}

void CLabelEditDlg::OnEnKillfocusRegedit()
{
	CString strText;
	m_regEdit.GetWindowText(strText);
	if(strText.IsEmpty())
		return;
	m_nstatus=_wtoi(strText);
}

void CLabelEditDlg::OnEnKillfocusWidthedit()
{
	// TODO: Add your control notification handler code here
	CString strText;
	m_widthEdit.GetWindowText(strText);
	if(strText.IsEmpty())
		return;
	m_nwidth=_wtoi(strText);
}

void CLabelEditDlg::OnEnKillfocusHeightedit()
{
	CString strText;
	m_heightEdt.GetWindowText(strText);
	if(strText.IsEmpty())
		return;
	m_nheight=_wtoi(strText);
}

void CLabelEditDlg::OnBnClickedExitbutton()
{
	CLabelEditDlg::OnCancel();
}

void CLabelEditDlg::OnBnClickedBkclorbtn()
{
	COLORREF color = m_bkClrBtn.GetColor();
	if (color == -1)
	{
		color = m_bkClrBtn.GetAutomaticColor();
	}
	m_bkColor=color;


}

void CLabelEditDlg::OnBnClickedTextbutton()
{

	COLORREF color = m_textClorBtn.GetColor();
	if (color == -1)
	{
		color = m_textClorBtn.GetAutomaticColor();
	}
	m_clrTxt=color;

}
