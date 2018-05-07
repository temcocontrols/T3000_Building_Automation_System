// DisplayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "DisplayDlg.h"
#include "afxdialogex.h"
 #include "global_function.h"

// CDisplayDlg dialog
   #define  THE_CHAR_LENGTH 8
IMPLEMENT_DYNAMIC(CDisplayDlg, CDialog)
UINT _FreshThread(LPVOID pParam){
CDisplayDlg* pdlg = (CDisplayDlg*)pParam;
pdlg->InitDlgData();
return 0;
}
CDisplayDlg::CDisplayDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDisplayDlg::IDD, pParent)
{

}

CDisplayDlg::~CDisplayDlg()
{
}

void CDisplayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUT_MSFLEXGRID, m_FlexGrid1);
	DDX_Control(pDX, IDC_SEQ_COM, m_ItemValueCombx);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressctrl);
}


BEGIN_MESSAGE_MAP(CDisplayDlg, CDialog)
	ON_EN_KILLFOCUS(IDC_LINE_1, &CDisplayDlg::OnEnKillfocusLine1)
	ON_EN_KILLFOCUS(IDC_LINE_2, &CDisplayDlg::OnEnKillfocusLine2)
	ON_EN_KILLFOCUS(IDC_INPUT_1, &CDisplayDlg::OnEnKillfocusInput1)
	ON_EN_KILLFOCUS(IDC_INPUT_2, &CDisplayDlg::OnEnKillfocusInput2)
	ON_EN_KILLFOCUS(IDC_INPUT_3, &CDisplayDlg::OnEnKillfocusInput3)
	ON_EN_KILLFOCUS(IDC_INPUT_4, &CDisplayDlg::OnEnKillfocusInput4)
	ON_EN_KILLFOCUS(IDC_INPUT_5, &CDisplayDlg::OnEnKillfocusInput5)
	ON_EN_KILLFOCUS(IDC_INPUT_6, &CDisplayDlg::OnEnKillfocusInput6)
	ON_EN_KILLFOCUS(IDC_INPUT_7, &CDisplayDlg::OnEnKillfocusInput7)
	ON_EN_KILLFOCUS(IDC_INPUT_8, &CDisplayDlg::OnEnKillfocusInput8)
	ON_EN_KILLFOCUS(IDC_INPUT_9, &CDisplayDlg::OnEnKillfocusInput9)
	ON_CBN_SELCHANGE(IDC_SEQ_COM, &CDisplayDlg::OnCbnSelchangeSeqCom)
	ON_BN_CLICKED(IDC_BUTTON1, &CDisplayDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CDisplayDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDisplayDlg message handlers
void CDisplayDlg::InitDlgData()
{

	m_progressctrl.ShowWindow(TRUE);
	m_progressctrl.SetRange(0,10);
	register_critical_section.Lock();
	for(int i=0;i<10;i++)
	{

		m_progressctrl.SetPos(i);
		Read_Multi(g_tstat_id,&product_register_value[i*64],i*64,64);

	} 
	register_critical_section.Unlock();
	GetDlgItem(IDC_LINE_1)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Line1"))));
	GetDlgItem(IDC_LINE_2)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Line2")))); 
	//UpdateData(FALSE);
	GetDlgItem(IDC_INPUT_1)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Input1"))));
	//UpdateData(FALSE);
	GetDlgItem(IDC_INPUT_2)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Input2"))));
	//UpdateData(FALSE);
	GetDlgItem(IDC_INPUT_3)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Input3"))));
	//UpdateData(FALSE);
	GetDlgItem(IDC_INPUT_4)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Input4"))));
	//UpdateData(FALSE);
	GetDlgItem(IDC_INPUT_5)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Input5"))));
	//UpdateData(FALSE);
	GetDlgItem(IDC_INPUT_6)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Input6"))));
	//UpdateData(FALSE);
	GetDlgItem(IDC_INPUT_7)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Input7"))));
	//UpdateData(FALSE);
	GetDlgItem(IDC_INPUT_8)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Input8"))));

	CString temp;
	int display=GetValueFromID(Get_RegID(multi_register_value[7],_T("Display Number")));
	temp.Format(_T("%d"),display);
	GetDlgItem(IDC_INPUT_9)->SetWindowText(temp);
	int  FirstID=Get_RegID(multi_register_value[7],_T("GridFirstID"));	
	m_display_number=GetValueFromID(Get_RegID(multi_register_value[7],_T("Display Number")));
	if (m_display_number>=0)
	{
		int display_number=m_display_number+1;
		m_FlexGrid1.put_Rows(display_number);
		m_FlexGrid1.put_Cols(2);
		m_FlexGrid1.put_TextMatrix(0,0,_T("Seq_NO"));
		m_FlexGrid1.put_TextMatrix(0,1,_T("Display_Choice"));
		m_FlexGrid1.put_ColWidth(0,800);
		m_FlexGrid1.put_ColWidth(1,2200);
		for (int i=1;i<=m_display_number;i++)
		{
			CString str_No;
			str_No.Format(_T("%d"),i);
			m_FlexGrid1.put_TextMatrix(i,0,str_No);
			int choice1=	 GetValueFromID(FirstID+i-1);

			m_FlexGrid1.put_TextMatrix(i,1,GetSel(choice1));


		}
	}
	m_progressctrl.SetPos(10);
	m_progressctrl.ShowWindow(SW_HIDE);

}
BOOL CDisplayDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
 
	GetDlgItem(IDC_LINE_1)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Line1"))));
	GetDlgItem(IDC_LINE_2)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Line2")))); 
	//UpdateData(FALSE);
	GetDlgItem(IDC_INPUT_1)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Input1"))));
	//UpdateData(FALSE);
	GetDlgItem(IDC_INPUT_2)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Input2"))));
	//UpdateData(FALSE);
	GetDlgItem(IDC_INPUT_3)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Input3"))));
	//UpdateData(FALSE);
	GetDlgItem(IDC_INPUT_4)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Input4"))));
	//UpdateData(FALSE);
	GetDlgItem(IDC_INPUT_5)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Input5"))));
	//UpdateData(FALSE);
	GetDlgItem(IDC_INPUT_6)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Input6"))));
	//UpdateData(FALSE);
	GetDlgItem(IDC_INPUT_7)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Input7"))));
	//UpdateData(FALSE);
	GetDlgItem(IDC_INPUT_8)->SetWindowText(GetTextReg(Get_RegID(multi_register_value[7],_T("Input8"))));
 
	     CString temp;
		int display=GetValueFromID(Get_RegID(multi_register_value[7],_T("Display Number")));
		temp.Format(_T("%d"),display);
		GetDlgItem(IDC_INPUT_9)->SetWindowText(temp);
	  
	 

	   int  FirstID=Get_RegID(multi_register_value[7],_T("GridFirstID"));	
   m_display_number=GetValueFromID(Get_RegID(multi_register_value[7],_T("Display Number")));
   if (m_display_number>=0)
   {
	  int display_number=m_display_number+1;
	   m_FlexGrid1.put_Rows(display_number);
	   m_FlexGrid1.put_Cols(2);
	   m_FlexGrid1.put_TextMatrix(0,0,_T("Seq_NO"));
	   m_FlexGrid1.put_TextMatrix(0,1,_T("Display_Choice"));
	   m_FlexGrid1.put_ColWidth(0,800);
	   m_FlexGrid1.put_ColWidth(1,2200);
	   for (int i=1;i<=m_display_number;i++)
	   {
		   CString str_No;
		   str_No.Format(_T("%d"),i);
		   m_FlexGrid1.put_TextMatrix(i,0,str_No);
		   int choice1=	 GetValueFromID(FirstID+i-1);
		 
		   m_FlexGrid1.put_TextMatrix(i,1,GetSel(choice1));
	 
		   
	   }
   }
	
	return TRUE;
}
//-1:Error RegID

int CDisplayDlg::Get_RegID(int Model_ID,CString Name)
{
	int RegID=-1;
	if (Model_ID==16)
	{
		if (Name.CompareNoCase(_T("Line1"))==0)
		{
			RegID=425;
		} 
		else if (Name.CompareNoCase(_T("Line2"))==0)
		{
			RegID=429;
		} 
		else if (Name.CompareNoCase(_T("Input1"))==0)
		{
			RegID=437;
		}
		else if (Name.CompareNoCase(_T("Input2"))==0)
		{
			RegID=441;
		}
		else if (Name.CompareNoCase(_T("Input3"))==0)
		{
			RegID=445;
		}
		else if (Name.CompareNoCase(_T("Input4"))==0)
		{
			RegID=449;
		}
		else if (Name.CompareNoCase(_T("Input5"))==0)
		{
			RegID=453;
		}
		else if (Name.CompareNoCase(_T("Input6"))==0)
		{
			RegID=457;
		}
		else if (Name.CompareNoCase(_T("Input7"))==0)
		{
			RegID=461;
		}
		else if (Name.CompareNoCase(_T("Input8"))==0)
		{
			RegID=465;
		}
		else if (Name.CompareNoCase(_T("Display Number"))==0)
		{
			RegID=497;
		}
		else if (Name.CompareNoCase(_T("GridFirstID"))==0)
		{
			RegID=498;
		}
		else
		{
			RegID=-1;
		}
	} 
	else if (Model_ID==PM_TSTAT6||Model_ID==PM_TSTAT7)
	{

		if (Name.CompareNoCase(_T("Line1"))==0)
		{
			RegID=435;
		} 
		else if (Name.CompareNoCase(_T("Line2"))==0)
		{
			RegID=439;
		} 
		else if (Name.CompareNoCase(_T("Input1"))==0)
		{
			RegID=447;
		}
		else if (Name.CompareNoCase(_T("Input2"))==0)
		{
			RegID=451;
		}
		else if (Name.CompareNoCase(_T("Input3"))==0)
		{
			RegID=455;
		}
		else if (Name.CompareNoCase(_T("Input4"))==0)
		{
			RegID=459;
		}
		else if (Name.CompareNoCase(_T("Input5"))==0)
		{
			RegID=463;
		}
		else if (Name.CompareNoCase(_T("Input6"))==0)
		{
			RegID=467;
		}
		else if (Name.CompareNoCase(_T("Input7"))==0)
		{
			RegID=471;
		}
		else if (Name.CompareNoCase(_T("Input8"))==0)
		{
			RegID=475;
		}
		else if (Name.CompareNoCase(_T("Display Number"))==0)
		{
			RegID=564;
		}
		else if (Name.CompareNoCase(_T("GridFirstID"))==0)
		{
			RegID=566;
		}
		else
		{
			RegID=-1;
		}
	} 
	else
	{
		RegID=-1;
	}
	return RegID; 
}
CString CDisplayDlg::GetTextFromReg(unsigned short reg){
	CString str_temp;
	unsigned short temp_buffer[4];
	unsigned short temp_buffer_Char[THE_CHAR_LENGTH];
	unsigned char p[THE_CHAR_LENGTH+1]={'\0'};
	if (Read_Multi(g_tstat_id,temp_buffer,reg,4)>0)
	{  unsigned short Hi_Char,Low_Char;

	for (int i=0;i<4;i++)
	{
		Hi_Char=temp_buffer[i];
		Hi_Char=Hi_Char&0xff00;
		Hi_Char=Hi_Char>>8;
		Low_Char=temp_buffer[i];
		Low_Char=Low_Char&0x00ff;
		temp_buffer_Char[2*i]=Hi_Char;
		temp_buffer_Char[2*i+1]=Low_Char;
	} 

	for (int i=0;i<THE_CHAR_LENGTH;i++)
	{
		p[i] =(unsigned char)temp_buffer_Char[i];

	}

	str_temp.Format(_T("%c%c%c%c%c%c%c%c"),p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7]);


	} 
	else
	{
		AfxMessageBox(_T("Reading Error"));
	}
	return str_temp;
}
void CDisplayDlg::OnEnKillfocusLine1()
{
	CString input;
	GetDlgItem(IDC_LINE_1)->GetWindowText(input);
	input.TrimRight();
	CString input2;
	GetDlgItem(IDC_LINE_2)->GetWindowText(input2);
	input2.TrimRight();

	unsigned char p[16];//input+input1
	//input
	for(int i=0;i<THE_CHAR_LENGTH;i++)
	{
		if(i<input.GetLength())
			p[i]=input.GetAt(i);
		else
			p[i]=' ';
	}
	//input1
	for(int i=0;i<THE_CHAR_LENGTH;i++)
	{
		if(i<input2.GetLength())
			p[i+THE_CHAR_LENGTH]=input2.GetAt(i);
		else
			p[i+THE_CHAR_LENGTH]=' ';
	}
	if (input.GetLength()>16)
	{
		GetDlgItem(IDC_LINE1)->SetWindowText(_T(">16 chars"));
	} 
	else
	{
		if (Write_Multi(g_tstat_id,p,Get_RegID(multi_register_value[7],_T("Line1")),16)>0)
		{
			GetDlgItem(IDC_LINE1)->SetWindowText(_T("OK"));
		} 
		else
		{
			GetDlgItem(IDC_LINE1)->SetWindowText(_T("Error"));
		}

	}

  GetDlgItem(IDC_LINE_1)->SetWindowText(GetTextFromReg(Get_RegID(multi_register_value[7],_T("Line1"))));
}


void CDisplayDlg::OnEnKillfocusLine2()
{
	// TODO: Add your control notification handler code here
	//OnEnKillfocusLine1();
	CString input;
	GetDlgItem(IDC_LINE_1)->GetWindowText(input);
	input.TrimRight();
	CString input2;
	GetDlgItem(IDC_LINE_2)->GetWindowText(input2);
	input2.TrimRight();

	unsigned char p[16];//input+input1
	//input
	for(int i=0;i<THE_CHAR_LENGTH;i++)
	{
		if(i<input.GetLength())
			p[i]=input.GetAt(i);
		else
			p[i]=' ';
	}
	//input1
	for(int i=0;i<THE_CHAR_LENGTH;i++)
	{
		if(i<input2.GetLength())
			p[i+THE_CHAR_LENGTH]=input2.GetAt(i);
		else
			p[i+THE_CHAR_LENGTH]=' ';
	}
	if (input.GetLength()>16)
	{
		GetDlgItem(IDC_LINE2)->SetWindowText(_T(">16 chars"));
	} 
	else
	{
		if (Write_Multi(g_tstat_id,p,Get_RegID(multi_register_value[7],_T("Line1")),16)>0)
		{
			GetDlgItem(IDC_LINE2)->SetWindowText(_T("OK"));
		} 
		else
		{
			GetDlgItem(IDC_LINE2)->SetWindowText(_T("Error"));
		}

	}
		GetDlgItem(IDC_LINE_2)->SetWindowText(GetTextFromReg(Get_RegID(multi_register_value[7],_T("Line2")))); 
}


void CDisplayDlg::OnEnKillfocusInput1()
{
	// TODO: Add your control notification handler code here
	CString input;
	GetDlgItem(IDC_INPUT_1)->GetWindowText(input);
	input.TrimRight();
	unsigned char p[8];//input+input1
	//input
	for(int i=0;i<THE_CHAR_LENGTH;i++)
	{
		if(i<input.GetLength())
			p[i]=input.GetAt(i);
		else
			p[i]=' ';
	}
	if (input.GetLength()>8)
	{
		GetDlgItem(IDC_INPUT1)->SetWindowText(_T(">8 chars"));
	} 
	else
	{
		if (Write_Multi(g_tstat_id,p,Get_RegID(multi_register_value[7],_T("Input1")),8))
		{
			GetDlgItem(IDC_INPUT1)->SetWindowText(_T("OK"));
		} 
		else
		{
			GetDlgItem(IDC_INPUT1)->SetWindowText(_T("Error"));
		}

	}

		GetDlgItem(IDC_INPUT_1)->SetWindowText(GetTextFromReg(Get_RegID(multi_register_value[7],_T("Input1"))));
}


void CDisplayDlg::OnEnKillfocusInput2()
{
	CString input;
	GetDlgItem(IDC_INPUT_2)->GetWindowText(input);
	input.TrimRight();
	unsigned char p[8];//input+input1
	//input
	for(int i=0;i<THE_CHAR_LENGTH;i++)
	{
		if(i<input.GetLength())
			p[i]=input.GetAt(i);
		else
			p[i]=' ';
	}
	if (input.GetLength()>8)
	{
		GetDlgItem(IDC_INPUT2)->SetWindowText(_T(">8 chars"));
	} 
	else
	{
		if (Write_Multi(g_tstat_id,p,Get_RegID(multi_register_value[7],_T("Input2")),8))
		{
			GetDlgItem(IDC_INPUT2)->SetWindowText(_T("OK"));
		} 
		else
		{
			GetDlgItem(IDC_INPUT2)->SetWindowText(_T("Error"));
		}

	}

		GetDlgItem(IDC_INPUT_2)->SetWindowText(GetTextFromReg(Get_RegID(multi_register_value[7],_T("Input2"))));
}


void CDisplayDlg::OnEnKillfocusInput3()
{
	// TODO: Add your control notification handler code here
	CString input;
	GetDlgItem(IDC_INPUT_3)->GetWindowText(input);
	input.TrimRight();
	unsigned char p[8];//input+input1
	//input
	for(int i=0;i<THE_CHAR_LENGTH;i++)
	{
		if(i<input.GetLength())
			p[i]=input.GetAt(i);
		else
			p[i]=' ';
	}
	if (input.GetLength()>8)
	{
		GetDlgItem(IDC_INPUT3)->SetWindowText(_T(">8 chars"));
	} 
	else
	{
		if (Write_Multi(g_tstat_id,p,Get_RegID(multi_register_value[7],_T("Input3")),8))
		{
			GetDlgItem(IDC_INPUT3)->SetWindowText(_T("OK"));
		} 
		else
		{
			GetDlgItem(IDC_INPUT3)->SetWindowText(_T("Error"));
		}

	}
		GetDlgItem(IDC_INPUT_3)->SetWindowText(GetTextFromReg(Get_RegID(multi_register_value[7],_T("Input3"))));
}


void CDisplayDlg::OnEnKillfocusInput4()
{
	CString input;
	GetDlgItem(IDC_INPUT_4)->GetWindowText(input);
	input.TrimRight();
	unsigned char p[8];//input+input1
	//input
	for(int i=0;i<THE_CHAR_LENGTH;i++)
	{
		if(i<input.GetLength())
			p[i]=input.GetAt(i);
		else
			p[i]=' ';
	}
	if (input.GetLength()>8)
	{
		GetDlgItem(IDC_INPUT4)->SetWindowText(_T(">8 chars"));
	} 
	else
	{
		if (Write_Multi(g_tstat_id,p,Get_RegID(multi_register_value[7],_T("Input4")),8))
		{
			GetDlgItem(IDC_INPUT4)->SetWindowText(_T("OK"));
		} 
		else
		{
			GetDlgItem(IDC_INPUT4)->SetWindowText(_T("Error"));
		}

	}
		GetDlgItem(IDC_INPUT_4)->SetWindowText(GetTextFromReg(Get_RegID(multi_register_value[7],_T("Input4"))));
}


void CDisplayDlg::OnEnKillfocusInput5()
{
	// TODO: Add your control notification handler code here
	CString input;
	GetDlgItem(IDC_INPUT_5)->GetWindowText(input);
	input.TrimRight();
	unsigned char p[8];//input+input1
	//input
	for(int i=0;i<THE_CHAR_LENGTH;i++)
	{
		if(i<input.GetLength())
			p[i]=input.GetAt(i);
		else
			p[i]=' ';
	}
	if (input.GetLength()>8)
	{
		GetDlgItem(IDC_INPUT5)->SetWindowText(_T(">8 chars"));
	} 
	else
	{
		if (Write_Multi(g_tstat_id,p,Get_RegID(multi_register_value[7],_T("Input5")),8))
		{
			GetDlgItem(IDC_INPUT5)->SetWindowText(_T("OK"));
		} 
		else
		{
			GetDlgItem(IDC_INPUT5)->SetWindowText(_T("Error"));
		}

	}
		GetDlgItem(IDC_INPUT_5)->SetWindowText(GetTextFromReg(Get_RegID(multi_register_value[7],_T("Input5"))));
}


void CDisplayDlg::OnEnKillfocusInput6()
{
	// TODO: Add your control notification handler code here
	CString input;
	GetDlgItem(IDC_INPUT_6)->GetWindowText(input);
	input.TrimRight();
	unsigned char p[8];//input+input1
	//input
	for(int i=0;i<THE_CHAR_LENGTH;i++)
	{
		if(i<input.GetLength())
			p[i]=input.GetAt(i);
		else
			p[i]=' ';
	}
	if (input.GetLength()>8)
	{
		GetDlgItem(IDC_INPUT6)->SetWindowText(_T(">8 chars"));
	} 
	else
	{
		if (Write_Multi(g_tstat_id,p,Get_RegID(multi_register_value[7],_T("Input6")),8))
		{
			GetDlgItem(IDC_INPUT6)->SetWindowText(_T("OK"));
		} 
		else
		{
			GetDlgItem(IDC_INPUT6)->SetWindowText(_T("Error"));
		}

	}
		GetDlgItem(IDC_INPUT_6)->SetWindowText(GetTextFromReg(Get_RegID(multi_register_value[7],_T("Input6"))));
}


void CDisplayDlg::OnEnKillfocusInput7()
{
	// TODO: Add your control notification handler code here
	CString input;
	GetDlgItem(IDC_INPUT_7)->GetWindowText(input);
	input.TrimRight();
	unsigned char p[8];//input+input1
	//input
	for(int i=0;i<THE_CHAR_LENGTH;i++)
	{
		if(i<input.GetLength())
			p[i]=input.GetAt(i);
		else
			p[i]=' ';
	}
	if (input.GetLength()>8)
	{
		GetDlgItem(IDC_INPUT7)->SetWindowText(_T(">8 chars"));
	} 
	else
	{
		if (Write_Multi(g_tstat_id,p,Get_RegID(multi_register_value[7],_T("Input7")),8))
		{
			GetDlgItem(IDC_INPUT7)->SetWindowText(_T("OK"));
		} 
		else
		{
			GetDlgItem(IDC_INPUT7)->SetWindowText(_T("Error"));
		}

	}
	GetDlgItem(IDC_INPUT_7)->SetWindowText(GetTextFromReg(Get_RegID(multi_register_value[7],_T("Input7"))));
}


void CDisplayDlg::OnEnKillfocusInput8()
{
	// TODO: Add your control notification handler code here
	CString input;
	GetDlgItem(IDC_INPUT_8)->GetWindowText(input);
	input.TrimRight();
	unsigned char p[8];//input+input1
	//input
	for(int i=0;i<THE_CHAR_LENGTH;i++)
	{
		if(i<input.GetLength())
			p[i]=input.GetAt(i);
		else
			p[i]=' ';
	}
	if (input.GetLength()>8)
	{
		GetDlgItem(IDC_INPUT8)->SetWindowText(_T(">8 chars"));
	} 
	else
	{
		if (Write_Multi(g_tstat_id,p,Get_RegID(multi_register_value[7],_T("Input8")),8))
		{
			GetDlgItem(IDC_INPUT8)->SetWindowText(_T("OK"));

		} 
		else
		{
			GetDlgItem(IDC_INPUT8)->SetWindowText(_T("Error"));
		}

	}
		GetDlgItem(IDC_INPUT_8)->SetWindowText(GetTextFromReg(Get_RegID(multi_register_value[7],_T("Input8"))));
}
int CDisplayDlg::GetValueFromID(unsigned short reg){
	int Value;
	if (multi_register_value[7]==PM_TSTAT6||multi_register_value[7]==PM_TSTAT7)
	{
	    Value=newtstat6[reg];
	 
	} 
	else
	{
		Value=multi_register_value[reg];
	 
	}
	return Value;
}
CString CDisplayDlg::GetTextReg(unsigned short reg){
	CString str_temp;
	unsigned short temp_buffer[4];
	unsigned short temp_buffer_Char[THE_CHAR_LENGTH];
	unsigned char p[THE_CHAR_LENGTH+1]={'\0'};
	if (multi_register_value[7]==PM_TSTAT6||multi_register_value[7]==PM_TSTAT7)
	{
		temp_buffer[0]=/*multi_register_value*/newtstat6[reg];
		temp_buffer[1]=/*multi_register_value*/newtstat6[reg+1];
		temp_buffer[2]=/*multi_register_value*/newtstat6[reg+2];
		temp_buffer[3]=/*multi_register_value*/newtstat6[reg+3];
	} 
	else
	{
		temp_buffer[0]=multi_register_value[reg];
		temp_buffer[1]=multi_register_value[reg+1];
		temp_buffer[2]=multi_register_value[reg+2];
		temp_buffer[3]=multi_register_value[reg+3];
	}

	unsigned short Hi_Char,Low_Char;

	for (int i=0;i<4;i++)
	{
		Hi_Char=temp_buffer[i];
		Hi_Char=Hi_Char&0xff00;
		Hi_Char=Hi_Char>>8;
		Low_Char=temp_buffer[i];
		Low_Char=Low_Char&0x00ff;
		temp_buffer_Char[2*i]=Hi_Char;
		temp_buffer_Char[2*i+1]=Low_Char;
	} 

	for (int i=0;i<THE_CHAR_LENGTH;i++)
	{
		p[i] =(unsigned char)temp_buffer_Char[i];
	}
	str_temp.Format(_T("%c%c%c%c%c%c%c%c"),p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7]);	 
	return str_temp;
}
void CDisplayDlg::OnEnKillfocusInput9()
{	
CString displaynum;
GetDlgItem(IDC_INPUT_9)->GetWindowText(displaynum);
    m_display_number=_wtoi(displaynum);
	if (m_display_number>14||m_display_number<0)
	{
		AfxMessageBox(_T("Input Error"));
	} 
	else
	{
		int ret=write_one(g_tstat_id,Get_RegID(multi_register_value[7],_T("Display Number")),m_display_number);
		if (ret>0)
		{
			if (multi_register_value[7]==PM_TSTAT6||multi_register_value[7]==PM_TSTAT7)
			{
				newtstat6[Get_RegID(multi_register_value[7],_T("Display Number"))]=m_display_number;
			} 
			else
			{
				multi_register_value[Get_RegID(multi_register_value[7],_T("Display Number"))]=m_display_number;
			}

			Fresh_Grid();
		} 
		else
		{
			AfxMessageBox(_T("Try again."));
		}

	}

}
void CDisplayDlg::Fresh_Grid(){
 
   int  FirstID=Get_RegID(multi_register_value[7],_T("GridFirstID"));	
   m_display_number=GetValueFromID(Get_RegID(multi_register_value[7],_T("Display Number")));
   if (m_display_number>=0)
   {
	  int display_number=m_display_number+1;
	   m_FlexGrid1.put_Rows(display_number);
	   m_FlexGrid1.put_Cols(2);
	   m_FlexGrid1.put_TextMatrix(0,0,_T("Seq_NO"));
	   m_FlexGrid1.put_TextMatrix(0,1,_T("Display_Choice"));
	   m_FlexGrid1.put_ColWidth(0,800);
	   m_FlexGrid1.put_ColWidth(1,2200);
	   for (int i=1;i<=m_display_number;i++)
	   {
		   CString str_No;
		   str_No.Format(_T("%d"),i);
		   m_FlexGrid1.put_TextMatrix(i,0,str_No);
		   int choice1=	 GetValueFromID(FirstID+i-1);
		  /* if (choice1>0)
		   {*/
		   m_FlexGrid1.put_TextMatrix(i,1,GetSel(choice1));
		/*   }
		   else
		   {
			  m_FlexGrid1.put_TextMatrix(i,1,_T("Read Error"));
		   }*/
		   
	   }
   }
	}

	CString CDisplayDlg::GetSel(int ID)
	{CString selection;
	//m_ItemValueCombx.AddString(_T("temperature"));
	//m_ItemValueCombx.AddString(_T("setpoint"));
	//m_ItemValueCombx.AddString(_T("AI1"));
	//m_ItemValueCombx.AddString(_T("AI2"));
	//m_ItemValueCombx.AddString(_T("AI3"));
	//m_ItemValueCombx.AddString(_T("AI4"));
	//m_ItemValueCombx.AddString(_T("AI5"));
	//m_ItemValueCombx.AddString(_T("AI6"));
	//m_ItemValueCombx.AddString(_T("AI7"));
	//m_ItemValueCombx.AddString(_T("AI8"));
	//m_ItemValueCombx.AddString(_T("MODE"));
	//m_ItemValueCombx.AddString(_T("USER INFO"));
	//m_ItemValueCombx.AddString(_T("CLOCK DATE"));
	//m_ItemValueCombx.AddString(_T("CLOCK TIME"));
	switch(ID)
	{
	case 0:
		selection=_T("temperature");
		break;
	case 1:
		selection=_T("setpoint");
		break;
	case 2:
		selection=_T("AI1");
		break;
	case 3:
		selection=_T("AI2");
		break;
	case 4:
		selection=_T("AI3");
		break;
	case 5:
		selection=_T("AI4");
		break;
	case 6:
		selection=_T("AI5");
		break;
	case 7:
		selection=_T("AI6");
		break;
	case 8:
		selection=_T("AI7");
		break;
	case 9:
		selection=_T("AI8");
		break;
	case 10:
		selection=_T("MODE");
		break;
	case 11:
		selection=_T("USER INFO");
		break;
	case 12:
		selection=_T("CLOCK DATE");
		break;
	case 13:
		selection=_T("CLOCK TIME");
		break;
	default:
		selection=_T("temperature");
		break;
	}
	return selection;
	}BEGIN_EVENTSINK_MAP(CDisplayDlg, CDialog)
		ON_EVENT(CDisplayDlg, IDC_INPUT_MSFLEXGRID, DISPID_CLICK, CDisplayDlg::ClickInputMsflexgrid, VTS_NONE)
		END_EVENTSINK_MAP()


	void CDisplayDlg::ClickInputMsflexgrid()
	{
		if(g_OutPutLevel==1)
	return;
	long lRow,lCol;
	lRow = m_FlexGrid1.get_RowSel();//获取点击的行号	
	lCol = m_FlexGrid1.get_ColSel(); //获取点击的列号
	if((lCol==0))
		return;
	if(lRow>m_FlexGrid1.get_Rows()) //如果点击区超过最大行号，则点击是无效的
		return; 
	if(lRow == 0) //如果点击标题行，也无效
		return;
	CRect rect;
	m_FlexGrid1.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形
	CDC* pDC =GetDC();
	//计算象素点和缇的转换比例
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_FlexGrid1.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_FlexGrid1.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_FlexGrid1.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_FlexGrid1.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rc.OffsetRect(rect.left+1,rect.top+1);
	//获取选中格的文本信息
	CString strValue = m_FlexGrid1.get_TextMatrix(lRow,lCol);
	m_nCurRow=lRow;
	m_nCurCol=lCol;
	 
	 
	if(lCol==1)
		{
		/*
		lcd rotate enable, 
		decide how many items will be shown on rotate mode,range0 to 21.
		Display item select:0:none 
		1:temperature 
		2:setpoint 
		3:AI1 
		4:AI2 
		5:AI3 
		6:AI4 
		7:AI5 
		8:AI6 
		9:AI7 
		10:AI8 
		11:MODE 
		12:USER INFO 
		13:CLOCK DATE 
		14:CLOCK TIME (TBD:OUTPUT)
		*/
		m_ItemValueCombx.ResetContent();
		m_ItemValueCombx.AddString(_T("temperature"));
		m_ItemValueCombx.AddString(_T("setpoint"));
		m_ItemValueCombx.AddString(_T("AI1"));
		m_ItemValueCombx.AddString(_T("AI2"));
		m_ItemValueCombx.AddString(_T("AI3"));
		m_ItemValueCombx.AddString(_T("AI4"));
		m_ItemValueCombx.AddString(_T("AI5"));
		m_ItemValueCombx.AddString(_T("AI6"));
		m_ItemValueCombx.AddString(_T("AI7"));
		m_ItemValueCombx.AddString(_T("AI8"));
		m_ItemValueCombx.AddString(_T("MODE"));
		m_ItemValueCombx.AddString(_T("USER INFO"));
		m_ItemValueCombx.AddString(_T("CLOCK DATE"));
		m_ItemValueCombx.AddString(_T("CLOCK TIME"));
		m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
		m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_ItemValueCombx.BringWindowToTop();
		m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
		m_ItemValueCombx.SetFocus(); //获取焦点
		}
	}


	void CDisplayDlg::OnCbnSelchangeSeqCom()
	{	int ID=Get_RegID(multi_register_value[7],_T("GridFirstID"))+m_nCurRow-1;
		int ret=write_one(g_tstat_id,ID,m_ItemValueCombx.GetCurSel());
		if (ret>0)
		{ 
			multi_register_value[Get_RegID(multi_register_value[7],_T("GridFirstID"))+m_nCurRow-1]=m_ItemValueCombx.GetCurSel();
			newtstat6[Get_RegID(multi_register_value[7],_T("GridFirstID"))+m_nCurRow-1]=m_ItemValueCombx.GetCurSel();
			m_FlexGrid1.put_TextMatrix(m_nCurRow,1,GetSel(m_ItemValueCombx.GetCurSel()));

		} 
		else
		{
		MessageBox(_T("Try again"));
		}

		//Fresh_Grid();
	}


	void CDisplayDlg::OnBnClickedButton1()
	{
		  
		pParamBackFresh = AfxBeginThread(_FreshThread,this);
		pParamBackFresh->m_bAutoDelete =FALSE;
	}
	void CDisplayDlg::OnDestroy()
	{

		CDialog::OnDestroy();

		DWORD dwCode;
		if (pParamBackFresh!=NULL)
		{
			GetExitCodeThread(pParamBackFresh->m_hThread,&dwCode);
			if (dwCode==STILL_ACTIVE)
			{
				Sleep(200);

				TerminateThread(pParamBackFresh->m_hThread,dwCode);
				delete pParamBackFresh;
			}
		}
		// TODO: Add your message handler code here
	}

	void CDisplayDlg::OnBnClickedOk()
	{
		// TODO: Add your control notification handler code here
		CDialog::OnOK();
	}
