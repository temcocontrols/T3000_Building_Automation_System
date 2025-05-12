// LabelEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "LabelEditDlg.h"



IMPLEMENT_DYNAMIC(CLabelEditDlg, CDialog)

extern BOOL g_bRefresh;
CLabelEditDlg::CLabelEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLabelEditDlg::IDD, pParent)
{
	m_clrTxt=RGB(0,0,0);
	m_bkColor=RGB(224, 232, 246);
	m_strScreenName=_T("");
	m_SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

}

CLabelEditDlg::~CLabelEditDlg()
{
   
   m_SqliteDBBuilding.closedb();
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


	/*  m_input[INPUT_NUMBER]={g_strSensorName,g_strInName1,g_strInName2,g_strInName3,g_strInName4,g_strInName5,g_strInName6,g_strInName7,g_strInName8,g_strInHumName,g_strInCO2};

	  m_output[OUTPUT_NUMBER]={_T("Relay 1"),_T("Relay 2"),_T("Relay 3"),_T("Relay 4"),_T("Relay 5"),_T("Analog 1"),_T("Analog 2")};*/

	m_input[0]=g_strSensorName;
	m_input[1]=g_strInName1;
	m_input[2]=g_strInName2;
	m_input[3]=g_strInName3;
	m_input[4]=g_strInName4;
	m_input[5]=g_strInName5;
	m_input[6]=g_strInName6;
	m_input[7]=g_strInName7;
    m_input[8]=g_strInName8;
    m_input[9]=g_strInHumName;
    m_input[10]=g_strInCO2;
	m_output[0]=_T("Relay 1");
	m_output[1]=_T("Relay 2");
	m_output[2]=_T("Relay 3");
	m_output[3]=_T("Relay 4");
	m_output[4]=_T("Relay 5");
	m_output[5]=_T("Analog 1");
	m_output[6]=_T("RAnalog 2");
	 
	//m_pCon.CreateInstance(_T("ADODB.Connection"));
	//m_pRs.CreateInstance(_T("ADODB.Recordset"));
	//m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);
	
	
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
	m_statusComBox.SetCurSel(m_nstatus);
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
	UpdateData(0);
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
	strSql.Format(_T("update Screen_Label set Width=%i,Height=%i,Status=%i,Input_or_Output=%i,Text_Color='%s',Back_Color='%s' where Serial_Num =%i and Tstat_id=%i and  Cstatic_id=%i and Tips='%s'"),
	m_nwidth,m_nheight,m_nstatus,m_input_or_output,strclrTxt,strclrBk,m_nSerialNum,m_id,m_nControlID,m_strScreenName);
	 m_SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
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
	strSql.Format(_T("select * from Screen_Label where Serial_Num =%i and Tstat_id=%i and Tips='%s'"),m_nSerialNum,m_id,m_strScreenName);
	//m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
	m_q = m_SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);

	CString strtemp;
	strtemp.Empty();
	_variant_t temp_variant;
	int nTemp;
	int nContyrolID=START_ID;
	while(!m_q.eof())
	{//find the ControlID;
		 
        strtemp = m_q.getValuebyName(L"Cstatic_id");
		nTemp=_wtoi(strtemp);
		if(nTemp==nContyrolID)
			nContyrolID=nContyrolID+1;
		else
			break;
		m_q.nextRow();
	}
	  


	CString strclrTxt;
	CString strclrBk;
	strclrTxt.Format(_T("%u"),m_clrTxt);
	strclrBk.Format(_T("%u"),m_bkColor);
	strSql.Format(_T("insert into Screen_Label values(%i,%i,%i,%i,%i,%i,%i,%i,'%s',%i,'%s','%s')"),nContyrolID,m_nSerialNum,50,50,m_nheight,m_nwidth,m_id,m_nstatus,m_strScreenName,m_input_or_output,strclrTxt,strclrBk);
	 m_SqliteDBBuilding.execDML((UTF8MBSTR)strSql);


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
	 
}

void CLabelEditDlg::OnCbnSelchangeIocombox()
{
//	m_statusComBox.ResetContent();
int m_inRows;
	m_input_or_output=m_IOCombox.GetCurSel();
	if(bac_cm5_graphic == false)	//如果不是CM5 
	{
		if(m_input_or_output==0)
		{
			int m_nModel=product_register_value[MODBUS_PRODUCT_MODEL];
			switch (m_nModel)
			{
			case 3:m_inRows=4;break; // 5B
			case 2:
			case 1:m_inRows=3;break; // 5A
			case 4:m_inRows=5;break; // 5C
			case PM_TSTAT7:m_inRows=12;break;
			case PM_PRESSURE:
			case 12:m_inRows=5;break; // 5D 同 TStat7
			case PM_TSTAT6:	m_inRows=12;break;
			case PM_TSTAT5i:	m_inRows=12;break;
            case PM_TSTAT8:	m_inRows=12;break;
            case PM_TSTAT9:	m_inRows = 12;break;
			case PM_TSTAT8_WIFI:	m_inRows = 12; break;
			case PM_TSTAT8_OCC:	m_inRows = 12; break;
			case PM_TSTAT7_ARM:	m_inRows = 12; break;
			case PM_TSTAT8_220V:	m_inRows = 12; break;

			case 16:m_inRows=10;break; // 5E
            case PM_PM5E:m_inRows=10;break; // 5E
            case PM_PM5E_ARM:m_inRows = 10;break; // 5E
			case 17:m_inRows=5;break; // 5F
			case 18:m_inRows=5;break; // 5G
			case 19:m_inRows=9;break; // 5H
			default:
				break;
			}
			--m_inRows;//去掉一行


			GetDlgItem(IDC_REG_STATIC)->SetWindowText(_T("Input"));
			for(int i=0;i<m_inRows;i++)
			{
				m_statusComBox.AddString(m_input[i]);
			}
			if(m_nstatus>=0&&m_nstatus<=11)
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
	else//是Bacnet 协议;
	{
		if(m_input_or_output==0)
		{
			m_statusComBox.ResetContent();
			for (int i=1;i<=BAC_INPUT_ITEM_COUNT;i++)
			{
				CString temp1;
				temp1.Format(_T("Input%d"),i);
				m_statusComBox.AddString(temp1);
			}
			m_regEdit.ShowWindow(SW_HIDE);
			m_statusComBox.ShowWindow(SW_SHOW);
		}
		else if(m_input_or_output == 1)
		{
			m_statusComBox.ResetContent();
			for (int i=1;i<=BAC_OUTPUT_ITEM_COUNT;i++)
			{
				CString temp1;
				temp1.Format(_T("Output%d"),i);
				m_statusComBox.AddString(temp1);
			}
			m_regEdit.ShowWindow(SW_HIDE);
			m_statusComBox.ShowWindow(SW_SHOW);
		}
		else if(m_input_or_output == 2)
		{
			m_statusComBox.ResetContent();
			for (int i=1;i<=BAC_VARIABLE_ITEM_COUNT;i++)
			{
				CString temp1;
				temp1.Format(_T("Variable%d"),i);
				m_statusComBox.AddString(temp1);
			}
			m_regEdit.ShowWindow(SW_HIDE);
			m_statusComBox.ShowWindow(SW_SHOW);
		}


	}
	m_statusComBox.Invalidate(1);
//	UpdateData(false);

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
