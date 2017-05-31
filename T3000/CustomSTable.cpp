// CustomSTable.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "CustomSTable.h"
#include "afxdialogex.h"
#include <vector>
 
#include "global_function.h"

// CCustomSTable dialog

IMPLEMENT_DYNAMIC(CCustomSTable, CDialogEx)

CCustomSTable::CCustomSTable(int Input_NO,CWnd* pParent /*=NULL*/)
	: CDialogEx(CCustomSTable::IDD, pParent)
{
m_InputNo=Input_NO;
}

CCustomSTable::~CCustomSTable()
{
}

void CCustomSTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCustomSTable, CDialogEx)
	ON_EN_KILLFOCUS(IDC_EDIT12, &CCustomSTable::OnEnKillfocusEdit12)
	ON_EN_KILLFOCUS(IDC_EDIT20, &CCustomSTable::OnEnKillfocusEdit20)
	ON_EN_KILLFOCUS(IDC_EDIT19, &CCustomSTable::OnEnKillfocusEdit19)
	ON_EN_KILLFOCUS(IDC_EDIT18, &CCustomSTable::OnEnKillfocusEdit18)
	ON_EN_KILLFOCUS(IDC_EDIT17, &CCustomSTable::OnEnKillfocusEdit17)
	ON_EN_KILLFOCUS(IDC_EDIT16, &CCustomSTable::OnEnKillfocusEdit16)
	ON_EN_KILLFOCUS(IDC_EDIT15, &CCustomSTable::OnEnKillfocusEdit15)
	ON_EN_KILLFOCUS(IDC_EDIT14, &CCustomSTable::OnEnKillfocusEdit14)
	ON_EN_KILLFOCUS(IDC_EDIT13, &CCustomSTable::OnEnKillfocusEdit13)
	ON_EN_KILLFOCUS(IDC_EDIT9, &CCustomSTable::OnEnKillfocusEdit9)
	ON_EN_KILLFOCUS(IDC_EDIT8, &CCustomSTable::OnEnKillfocusEdit8)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CCustomSTable::OnEnKillfocusEdit1)
	ON_BN_CLICKED(IDOK, &CCustomSTable::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CCustomSTable::OnInitDialog()
{
	CDialog::OnInitDialog();

	//m_version=get_tstat_version(g_tstat_id);

	to_fresh();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
// CCustomSTable message handlers
void CCustomSTable::to_fresh()
{
	
	 
	vector<int> vet_Row;
	for (int i=0;i<8;i++)
	{
	 if (product_register_value[MODBUS_ANALOG1_RANGE+i]==4)
	 {   
	 vet_Row.push_back(i);
	 }
	}
	if (vet_Row.size()>1)
	{
		m_row1=vet_Row[0]+1;
		m_row2=vet_Row[1]+1;
	}
	else if (vet_Row.size()==1)
	{
	    m_row1=vet_Row[0]+1;
	}
	else
	{
	    m_row1=0;
	    m_row2=0;
	}

	if (m_row1>m_row2)
	{
	  m_row1=m_row2;
	}
	 

	if(m_InputNo==2)
	{ 
	       if (m_row1>=1)
	       {
			   CString temp;
			   temp.Format(_T("AI%d"),m_row1);
			   GetDlgItem(IDC_ROWNO)->SetWindowText(temp);
	       }

		    m_slider1_i=(short)product_register_value[MODBUS_TABLE1_ZERO]/10;
		    m_slider2_i=(short)product_register_value[MODBUS_TABLE1_HALFONE]/10;
			m_slider3_i=(short)product_register_value[MODBUS_TABLE1_ONE]/10;
			m_slider4_i=(short)product_register_value[MODBUS_TABLE1_HALFTWO]/10;
			m_slider5_i=(short)product_register_value[MODBUS_TABLE1_TWO]/10;
			m_slider6_i=(short)product_register_value[MODBUS_TABLE1_HALFTHREE]/10;
			m_slider7_i=(short)product_register_value[MODBUS_TABLE1_THREE]/10;
			m_slider8_i=(short)product_register_value[MODBUS_TABLE1_HALFFOUR]/10;
			m_slider9_i=(short)product_register_value[MODBUS_TABLE1_FOUR]/10;
		   m_slider10_i=(short)product_register_value[MODBUS_TABLE1_HALFFIVE]/10;
		   m_slider11_i=(short)product_register_value[MODBUS_TABLE1_FIVE]/10;
		//}
	//	UpdateData(FALSE);
		int m_271=product_register_value[MODBUS_UNITS1_HIGH];
		int m_272=product_register_value[MODBUS_UNITS1_LOW];
		if(m_271>>8=='0')
		{
			if((m_271 & 0xFF) =='0')
			{
				if(m_272>>8=='0')
					m_units_s.Format(_T("%c"),m_272 & 0xFF);
				else
					m_units_s.Format(_T("%c%c"),m_272>>8,m_272 & 0xFF);
			}
			else
				m_units_s.Format(_T("%c%c%c"),m_271 & 0xFF,m_272>>8,m_272 & 0xFF);
		}
		else
			m_units_s.Format(_T("%c%c%c%c"),m_271>>8,m_271 & 0xFF,m_272>>8,m_272 & 0xFF);

		GetDlgItem(IDC_EDIT12)->SetWindowText(m_units_s);
	}
	else if (m_InputNo==3)
	{
		if (m_row2>=2)
		{
			CString temp;
			temp.Format(_T("AI%d"),m_row2);
			GetDlgItem(IDC_ROWNO)->SetWindowText(temp);
		}
 
			m_slider1_i=(short)product_register_value[MODBUS_TABLE2_ZERO]/10;
			m_slider2_i=(short)product_register_value[MODBUS_TABLE2_HALFONE]/10;
			m_slider3_i=(short)product_register_value[MODBUS_TABLE2_ONE]/10;
			m_slider4_i=(short)product_register_value[MODBUS_TABLE2_HALFTWO]/10;
			m_slider5_i=(short)product_register_value[MODBUS_TABLE2_TWO]/10;
			m_slider6_i=(short)product_register_value[MODBUS_TABLE2_HALFTHREE]/10;
			m_slider7_i=(short)product_register_value[MODBUS_TABLE2_THREE]/10;
			m_slider8_i=(short)product_register_value[MODBUS_TABLE2_HALFFOUR]/10;
			m_slider9_i=(short)product_register_value[MODBUS_TABLE2_FOUR]/10;
		   m_slider10_i=(short)product_register_value[MODBUS_TABLE2_HALFFIVE]/10;
		   m_slider11_i=(short)product_register_value[MODBUS_TABLE2_FIVE]/10;

	 


		int m_273=product_register_value[MODBUS_UNITS2_HIGH];
		int m_274=product_register_value[MODBUS_UNITS2_LOW];
		if(m_273>>8=='0')
		{
			if((m_273 & 0xFF)=='0')
			{
				if(m_274>>8=='0')
					m_units_s.Format(_T("%c"),m_274 & 0xFF);
				else
					m_units_s.Format(_T("%c%c"),m_274>>8,m_274 & 0xFF);
			}
			else
				m_units_s.Format(_T("%c%c%c"),m_273 & 0xFF,m_274>>8,m_274 & 0xFF);
		}		
		else
            m_units_s.Format(_T("%c%c%c%c"),m_273>>8,m_273 & 0xFF,m_274>>8,m_274 & 0xFF);

		GetDlgItem(IDC_EDIT12)->SetWindowText(m_units_s);
	}



 
		 
		int fvalue=m_slider1_i;//IDC_EDIT1
		CString strtext;
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT1)->SetWindowText(strtext);

		fvalue=m_slider2_i;//IDC_EDIT1
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT8)->SetWindowText(strtext);

		fvalue=m_slider3_i;//IDC_EDIT1
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT9)->SetWindowText(strtext);


		fvalue=m_slider4_i;//IDC_EDIT1
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT13)->SetWindowText(strtext);

		fvalue=m_slider5_i;//IDC_EDIT1
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT14)->SetWindowText(strtext);

		fvalue=m_slider6_i;//IDC_EDIT1
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT15)->SetWindowText(strtext);

		fvalue=m_slider7_i;//IDC_EDIT1
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT16)->SetWindowText(strtext);


		fvalue=m_slider8_i;//IDC_EDIT1
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT17)->SetWindowText(strtext);

		fvalue=m_slider9_i;//IDC_EDIT1
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT18)->SetWindowText(strtext);

		fvalue=m_slider10_i;//IDC_EDIT1
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT19)->SetWindowText(strtext);


		fvalue=m_slider11_i;//IDC_EDIT1
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT20)->SetWindowText(strtext);

	//}
	
	//UpdateData(false);
}


 


void CCustomSTable::OnEnKillfocusEdit1()
{
		//int ftemp;
		CString strtxt;
		GetDlgItem(IDC_EDIT1)->GetWindowText(strtxt);
		m_slider1_i=_wtoi(strtxt);
		 

	  // m_slider1_i=ftemp;
		
	
}

void CCustomSTable::OnEnKillfocusEdit8()
{
	
// if(m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// {
// 	float ftemp;
// 	CString strtxt;
// 	GetDlgItem(IDC_EDIT8)->GetWindowText(strtxt);
// 	ftemp=(float)_wtof(strtxt);
// 
// 	strtxt.Format(_T("%.1f"),ftemp);
// 	GetDlgItem(IDC_EDIT8)->SetWindowText(strtxt);
// 	ftemp=(float)_wtof(strtxt);
// 	m_slider2_i=(int)(ftemp*10);
// }
// else
// {
	//int ftemp;
	CString strtxt;
	GetDlgItem(IDC_EDIT8)->GetWindowText(strtxt);
	m_slider2_i=_wtoi(strtxt);
	 

	 
	//m_slider2_i=ftemp;
//}
	

	//UpdateData();	
	 
	
}

void CCustomSTable::OnEnKillfocusEdit9()
{
	
 
		/*int ftemp;*/
		CString strtxt;
		GetDlgItem(IDC_EDIT9)->GetWindowText(strtxt);
		m_slider3_i=_wtoi(strtxt);

 
		//m_slider3_i=ftemp;

	 
}

void CCustomSTable::OnEnKillfocusEdit13()
{
 
		 
		CString strtxt;
		GetDlgItem(IDC_EDIT13)->GetWindowText(strtxt);
		m_slider4_i=_wtoi(strtxt);
		 
	 
 

}

void CCustomSTable::OnEnKillfocusEdit14()
{
 
		 
		CString strtxt;
		GetDlgItem(IDC_EDIT14)->GetWindowText(strtxt);
		m_slider5_i=_wtoi(strtxt);
	 
 
}

void CCustomSTable::OnEnKillfocusEdit15()
{
  
		CString strtxt;
		GetDlgItem(IDC_EDIT15)->GetWindowText(strtxt);
		m_slider6_i=_wtoi(strtxt);

		 
	
}

void CCustomSTable::OnEnKillfocusEdit16()
{
  
		CString strtxt;
		GetDlgItem(IDC_EDIT16)->GetWindowText(strtxt);
		m_slider7_i=_wtoi(strtxt);

 

}

void CCustomSTable::OnEnKillfocusEdit17()
{
	  
		CString strtxt;
		GetDlgItem(IDC_EDIT17)->GetWindowText(strtxt);
		m_slider8_i=_wtoi(strtxt);

 
}

void CCustomSTable::OnEnKillfocusEdit18()
{
 
		 
		CString strtxt;
		GetDlgItem(IDC_EDIT18)->GetWindowText(strtxt);
		m_slider9_i=_wtoi(strtxt);
 
	
}

void CCustomSTable::OnEnKillfocusEdit19()
{
 
		 
		CString strtxt;
		GetDlgItem(IDC_EDIT19)->GetWindowText(strtxt);
		m_slider10_i=_wtoi(strtxt);

 


}

void CCustomSTable::OnEnKillfocusEdit20()
{
 
		 
		CString strtxt;
		GetDlgItem(IDC_EDIT20)->GetWindowText(strtxt);
		m_slider11_i=_wtoi(strtxt);

	 	
 

	 
}

void CCustomSTable::OnEnKillfocusEdit12()
{
	
	 
	/*	UpdateData();*/		
	GetDlgItem(IDC_EDIT12)->GetWindowText(m_units_s);
		unsigned short first,second;
		switch(m_units_s.GetLength())
		{
		case 1:	m_units_s=_T("000")+m_units_s;break;
		case 2:	m_units_s=_T("00")+m_units_s;break;
		case 3:	m_units_s=_T("0")+m_units_s;break;
		case 4:	break;
		default : AfxMessageBox(_T("Wrong input!"));m_units_s=_T("");return;			
		}
		first=m_units_s.GetAt(0)*256+m_units_s.GetAt(1);
		second=m_units_s.GetAt(2)*256+m_units_s.GetAt(3);
		if(m_InputNo==2)
		{
			int ret=write_one(g_tstat_id, MODBUS_UNITS1_HIGH,first);
			if (ret>0)
			{
			  product_register_value[MODBUS_UNITS1_HIGH]=first;
			}
			ret=write_one(g_tstat_id, MODBUS_UNITS1_LOW,second);
			if (ret>0)
			{
			 product_register_value[MODBUS_UNITS1_LOW]=second;
			}
		}
		else if(m_InputNo==3)
		{
			int ret=write_one(g_tstat_id, MODBUS_UNITS2_HIGH,first);
			if (ret>0)
			{
				product_register_value[MODBUS_UNITS2_HIGH]=first;
			}
			ret=write_one(g_tstat_id, MODBUS_UNITS2_LOW,second);
			if (ret>0)
			{
				product_register_value[MODBUS_UNITS2_LOW]=second;
			}
		}
 
		to_fresh();
	 
//	SetTimer(1,ONTIMER_TIME,NULL);//设定每五秒更新一次开始
}

void CCustomSTable::OnBnClickedOk()
{
	

	if(m_InputNo==2)
	{

		 
		write_one(g_tstat_id, MODBUS_TABLE1_ZERO,m_slider1_i*10);
		write_one(g_tstat_id, MODBUS_TABLE1_HALFONE,m_slider2_i*10);
		write_one(g_tstat_id, MODBUS_TABLE1_ONE,m_slider3_i*10);
		write_one(g_tstat_id, MODBUS_TABLE1_HALFTWO,m_slider4_i*10);
		write_one(g_tstat_id, MODBUS_TABLE1_TWO,m_slider5_i*10);
		write_one(g_tstat_id, MODBUS_TABLE1_HALFTHREE,m_slider6_i*10);
		write_one(g_tstat_id, MODBUS_TABLE1_THREE,m_slider7_i*10);
		write_one(g_tstat_id, MODBUS_TABLE1_HALFFOUR,m_slider8_i*10);
		write_one(g_tstat_id, MODBUS_TABLE1_FOUR,m_slider9_i*10);
		write_one(g_tstat_id, MODBUS_TABLE1_HALFFIVE,m_slider10_i*10);
		write_one(g_tstat_id, MODBUS_TABLE1_FIVE,m_slider11_i*10);

		product_register_value[MODBUS_TABLE1_ZERO]=m_slider1_i*10;
		product_register_value[MODBUS_TABLE1_HALFONE]=m_slider2_i*10;
		product_register_value[MODBUS_TABLE1_ONE]=m_slider3_i*10;
		product_register_value[MODBUS_TABLE1_HALFTWO]=m_slider4_i*10;
		product_register_value[MODBUS_TABLE1_TWO]=m_slider5_i*10;
		product_register_value[MODBUS_TABLE1_HALFTHREE]=m_slider6_i*10;
		product_register_value[MODBUS_TABLE1_THREE]=m_slider7_i*10;
		product_register_value[MODBUS_TABLE1_HALFFOUR]=m_slider8_i*10;
		product_register_value[MODBUS_TABLE1_FOUR]=m_slider9_i*10;
		product_register_value[MODBUS_TABLE1_HALFFIVE]=m_slider10_i*10;
		product_register_value[MODBUS_TABLE1_FIVE]=m_slider11_i*10;



	}
	else if(m_InputNo==3)
	{

	 
		write_one(g_tstat_id, MODBUS_TABLE2_ZERO,m_slider1_i*10);
		write_one(g_tstat_id, MODBUS_TABLE2_HALFONE,m_slider2_i*10);
		write_one(g_tstat_id, MODBUS_TABLE2_ONE,m_slider3_i*10);
		write_one(g_tstat_id, MODBUS_TABLE2_HALFTWO,m_slider4_i*10);
		write_one(g_tstat_id, MODBUS_TABLE2_TWO,m_slider5_i*10);
		write_one(g_tstat_id, MODBUS_TABLE2_HALFTHREE,m_slider6_i*10);
		write_one(g_tstat_id, MODBUS_TABLE2_THREE,m_slider7_i*10);
		write_one(g_tstat_id, MODBUS_TABLE2_HALFFOUR,m_slider8_i*10);
		write_one(g_tstat_id, MODBUS_TABLE2_FOUR,m_slider9_i*10);
		write_one(g_tstat_id, MODBUS_TABLE2_HALFFIVE,m_slider10_i*10);
		write_one(g_tstat_id, MODBUS_TABLE2_FIVE,m_slider11_i*10);

		product_register_value[MODBUS_TABLE2_ZERO]=m_slider1_i*10   ;
		product_register_value[MODBUS_TABLE2_HALFONE]=m_slider2_i*10;
		product_register_value[MODBUS_TABLE2_ONE]=m_slider3_i*10;
		product_register_value[MODBUS_TABLE2_HALFTWO]=m_slider4_i*10;
		product_register_value[MODBUS_TABLE2_TWO]=m_slider5_i*10;
		product_register_value[MODBUS_TABLE2_HALFTHREE]=m_slider6_i*10;
		product_register_value[MODBUS_TABLE2_THREE]=m_slider7_i*10;
		product_register_value[MODBUS_TABLE2_HALFFOUR]=m_slider8_i*10;
		product_register_value[MODBUS_TABLE2_FOUR]=m_slider9_i*10;
		product_register_value[MODBUS_TABLE2_HALFFIVE]=m_slider10_i*10;
		product_register_value[MODBUS_TABLE2_FIVE]=m_slider11_i*10;

	}

	to_fresh();
	//CDialogEx::OnOK();
}
