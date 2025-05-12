// BuildTable1.cpp : 实现文件
//

#include "stdafx.h"
#include "BuildTable1.h"
#include "stdafx.h"
#include "T3000.h"
#include "global_function.h"


#define	SLIDER_RANGE 65535
#define MAX_VALUE atoi(m_max)
#define OVER_MAX_OR_BELOW_MIN AfxMessageBox(_T("Please verify the value is between the minimum and maximum readings!"))

IMPLEMENT_DYNAMIC(CBuildTable1, CDialog)


short Get_Min(short *short_Array,int length){
		short min=short_Array[0];
		for (int i=1;i<length;i++)
		{
			if (min>short_Array[i])
			{
				min=short_Array[i];
			}
		}
		return min;
}
short Get_Max(short *short_Array,int length){
	short max=short_Array[0];
	for (int i=1;i<length;i++)
	{
		if (max<short_Array[i])
		{
			max=short_Array[i];
		}
	}
	return max;
}


CBuildTable1::CBuildTable1(int Input_NO,CWnd* pParent /*=NULL*/)
	: CDialog(CBuildTable1::IDD, pParent)	
    , m_slider1_i(0)
    , m_slider2_i(0)
    , m_slider3_i(0)
    , m_slider4_i(0)
    , m_slider5_i(0)
    , m_slider6_i(0)
    , m_slider7_i(0)
    , m_slider8_i(0)
    , m_slider9_i(0)
    , m_slider10_i(0)
    , m_slider11_i(0)
 
	, m_units_s(_T(""))
{
    m_InputNo=Input_NO;
    IsModfied = false;

    m_bIncreasingMode=TRUE;
}

CBuildTable1::~CBuildTable1()
{ 
}

void CBuildTable1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_slider1_ctrl);
	DDX_Control(pDX, IDC_SLIDER2, m_slider2_ctrl);
	DDX_Control(pDX, IDC_SLIDER3, m_slider3_ctrl);
	DDX_Control(pDX, IDC_SLIDER4, m_slider4_ctrl);
	DDX_Control(pDX, IDC_SLIDER5, m_slider5_ctrl);
	DDX_Control(pDX, IDC_SLIDER6, m_slider6_ctrl);
	DDX_Control(pDX, IDC_SLIDER7, m_slider7_ctrl);
	DDX_Control(pDX, IDC_SLIDER8, m_slider8_ctrl);
	DDX_Control(pDX, IDC_SLIDER9, m_slider9_ctrl);
	DDX_Control(pDX, IDC_SLIDER10, m_slider10_ctrl);
	DDX_Control(pDX, IDC_SLIDER11, m_slider11_ctrl);


	DDX_Text(pDX, IDC_EDIT10, m_max);
	DDX_Text(pDX, IDC_EDIT11, m_min);

	DDX_Text(pDX, IDC_EDIT12, m_units_s);
}

BEGIN_MESSAGE_MAP(CBuildTable1, CDialog)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
 
 

	ON_WM_TIMER()
	ON_WM_DESTROY()
//	ON_EN_SETFOCUS(IDC_EDIT1, OnEnSetfocusEdit1)
	 
	//ON_EN_SETFOCUS(IDC_EDIT12, OnEnSetfocusEdit12)
	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT8, OnEnChangeEdit8)
	ON_EN_CHANGE(IDC_EDIT9, OnEnChangeEdit9)
	ON_EN_CHANGE(IDC_EDIT13, OnEnChangeEdit13)
	ON_EN_CHANGE(IDC_EDIT14, OnEnChangeEdit14)
	ON_EN_CHANGE(IDC_EDIT15, OnEnChangeEdit15)
	ON_EN_CHANGE(IDC_EDIT16, OnEnChangeEdit16)
	ON_EN_CHANGE(IDC_EDIT17, OnEnChangeEdit17)
	ON_EN_CHANGE(IDC_EDIT18, OnEnChangeEdit18)
	ON_EN_CHANGE(IDC_EDIT19, OnEnChangeEdit19)
	ON_EN_CHANGE(IDC_EDIT20, OnEnChangeEdit20)
	ON_EN_CHANGE(IDC_EDIT12, OnEnChangeEdit12)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnEnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT8, OnEnKillfocusEdit8)
	ON_EN_KILLFOCUS(IDC_EDIT9, OnEnKillfocusEdit9)
	ON_EN_KILLFOCUS(IDC_EDIT13, OnEnKillfocusEdit13)
	ON_EN_KILLFOCUS(IDC_EDIT14, OnEnKillfocusEdit14)
	ON_EN_KILLFOCUS(IDC_EDIT15, OnEnKillfocusEdit15)
	ON_EN_KILLFOCUS(IDC_EDIT16, OnEnKillfocusEdit16)
	ON_EN_KILLFOCUS(IDC_EDIT17, OnEnKillfocusEdit17)
	ON_EN_KILLFOCUS(IDC_EDIT18, OnEnKillfocusEdit18)
	ON_EN_KILLFOCUS(IDC_EDIT19, OnEnKillfocusEdit19)
	ON_EN_KILLFOCUS(IDC_EDIT20, OnEnKillfocusEdit20)
	ON_EN_KILLFOCUS(IDC_EDIT12, OnEnKillfocusEdit12)
 
	ON_EN_CHANGE(IDC_EDIT10, OnEnChangeEdit10)
	ON_EN_SETFOCUS(IDC_EDIT10, OnEnSetfocusEdit10)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SAVEOK, OnBnClickedSaveok)
END_MESSAGE_MAP()


// CBuildTable1 消息处理程序

void CBuildTable1::OnBnClickedOk()
{
	
	//OnOK();
}

BOOL CBuildTable1::OnInitDialog()
{
	CDialog::OnInitDialog();

	//m_version=get_tstat_version(g_tstat_id);
 
	to_fresh();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CBuildTable1::to_fresh()
{
	//float m_version=get_tstat_version(g_tstat_id);
	if(m_InputNo==2)
	{
		

		short temp[11];
		for (int i=0;i<11;i++)
		{
			temp[i]=(short)product_register_value[MODBUS_TABLE1_ZERO+i];
		}

		m_nMax=Get_Max(&temp[0],11)/10;
		m_nMin=Get_Min(&temp[0],11)/10;

		
	}
	else if (m_InputNo==3)
	{
		
			m_nMax=product_register_value[MODBUS_TABLE2_FIVE]/10;
			m_nMin=product_register_value[MODBUS_TABLE2_ZERO]/10;


			short temp[11];
			for (int i=0;i<11;i++)
			{
				temp[i]=(short)product_register_value[MODBUS_TABLE2_ZERO+i];
			}

			m_nMax=Get_Max(&temp[0],11)/10;
			m_nMin=Get_Min(&temp[0],11)/10;


	}
	int nTemp;
	if(m_nMin>m_nMax)
	{
		nTemp=m_nMin;
    	m_nMin=m_nMax;
    	m_nMax=nTemp;
	}
 
 


	if(m_nMax>m_nMin)
	{
		m_bIncreasingMode=TRUE;
	}
	else
	{
		m_bIncreasingMode=FALSE;
	}

	CStatic * pstatic2 = (CStatic *)GetDlgItem(IDC_STATIC2);
	CStatic * pstatic3 = (CStatic *)GetDlgItem(IDC_STATIC3);
	CStatic * pstatic4 = (CStatic *)GetDlgItem(IDC_STATIC4);
	CStatic * pstatic5 = (CStatic *)GetDlgItem(IDC_STATIC5);
	CStatic * pstatic6 = (CStatic *)GetDlgItem(IDC_STATIC6);
	CStatic * pstatic7 = (CStatic *)GetDlgItem(IDC_STATIC7);
	CStatic * pstatic8 = (CStatic *)GetDlgItem(IDC_STATIC8);
	CStatic * pstatic9 = (CStatic *)GetDlgItem(IDC_STATIC9);
	CStatic * pstatic10 = (CStatic *)GetDlgItem(IDC_STATIC10);
	CStatic * pstatic11 = (CStatic *)GetDlgItem(IDC_STATIC11);

	m_slider1_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider2_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider3_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider4_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider5_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider6_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider7_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider8_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider9_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider10_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider11_ctrl.SetRange(m_nMin,m_nMax);

	if(m_InputNo==2)
	{ 
 
 
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

	m_slider1_ctrl.SetPos(m_nMax - (m_slider1_i-m_nMin));
	m_slider2_ctrl.SetPos(m_nMax - (m_slider2_i-m_nMin));
	m_slider3_ctrl.SetPos(m_nMax - (m_slider3_i-m_nMin));
	m_slider4_ctrl.SetPos(m_nMax - (m_slider4_i-m_nMin));
	m_slider5_ctrl.SetPos(m_nMax - (m_slider5_i-m_nMin));
	m_slider6_ctrl.SetPos(m_nMax - (m_slider6_i-m_nMin));
	m_slider7_ctrl.SetPos(m_nMax - (m_slider7_i-m_nMin));
	m_slider8_ctrl.SetPos(m_nMax - (m_slider8_i-m_nMin));
	m_slider9_ctrl.SetPos(m_nMax - (m_slider9_i-m_nMin));
   m_slider10_ctrl.SetPos(m_nMax - (m_slider10_i-m_nMin));
   m_slider11_ctrl.SetPos(m_nMax - (m_slider11_i-m_nMin));


		int npos=m_slider1_ctrl.GetPos();
		int fvalue=(m_nMax+m_nMin-npos);//IDC_EDIT1
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
  
}

 

 

void CBuildTable1::OnTimer(UINT nIDEvent)
{
	
	to_fresh();//fresh	
	CDialog::OnTimer(nIDEvent);
}

void CBuildTable1::OnDestroy()
{
	CDialog::OnDestroy();

	KillTimer(1);//killtimer
	
}

 

 


void CBuildTable1::OnEnChangeEdit1()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	
	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit8()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit9()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit13()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit14()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit15()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit16()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit17()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit18()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit19()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit20()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit12()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnKillfocusEdit1()
{

 
		 
		CString strtxt;
		GetDlgItem(IDC_EDIT1)->GetWindowText(strtxt);
		m_slider1_i=_wtoi(strtxt);
 
}

void CBuildTable1::OnEnKillfocusEdit8()
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
	int ftemp;
	CString strtxt;
	GetDlgItem(IDC_EDIT8)->GetWindowText(strtxt);
	ftemp=_wtoi(strtxt);
	 
	m_slider2_i=ftemp;
//}
	

	//UpdateData();	
	refresh();
	
}

void CBuildTable1::OnEnKillfocusEdit9()
{
	
// 	if(m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		float ftemp;
// 		CString strtxt;
// 		GetDlgItem(IDC_EDIT9)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 
// 		strtxt.Format(_T("%.1f"),ftemp);
// 		GetDlgItem(IDC_EDIT9)->SetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		m_slider3_i=(int)(ftemp*10);
// 	}
// 	else
// 	{
		int ftemp;
		CString strtxt;
		GetDlgItem(IDC_EDIT9)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);

	/*	if (ftemp<0)
		{
			AfxMessageBox(_T("your input can not be negative"));
			to_fresh();
			return;
		}*/
		strtxt.Format(_T("%d"),ftemp);
		GetDlgItem(IDC_EDIT9)->SetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider3_i=ftemp;

	//}
	

//	UpdateData();		
	refresh();


}

void CBuildTable1::OnEnKillfocusEdit13()
{

    int ftemp;
    CString strtxt;
    GetDlgItem(IDC_EDIT13)->GetWindowText(strtxt);
    ftemp = _wtoi(strtxt);

    strtxt.Format(_T("%d"), ftemp);
    GetDlgItem(IDC_EDIT13)->SetWindowText(strtxt);
    ftemp = _wtoi(strtxt);
    m_slider4_i = ftemp;


    refresh();

}

void CBuildTable1::OnEnKillfocusEdit14()
{

    int ftemp;
    CString strtxt;
    GetDlgItem(IDC_EDIT14)->GetWindowText(strtxt);
    ftemp = _wtoi(strtxt);

    strtxt.Format(_T("%d"), ftemp);
    GetDlgItem(IDC_EDIT14)->SetWindowText(strtxt);
    ftemp = _wtoi(strtxt);
    m_slider5_i = ftemp;

    refresh();
}

void CBuildTable1::OnEnKillfocusEdit15()
{
	
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		float ftemp;
// 		CString strtxt;
// 		GetDlgItem(IDC_EDIT15)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 
// 		strtxt.Format(_T("%.1f"),ftemp);
// 		GetDlgItem(IDC_EDIT15)->SetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		m_slider6_i=(int)(ftemp*10);
// 
// 	}
// 	else
// 	{
		int ftemp;
		CString strtxt;
		GetDlgItem(IDC_EDIT15)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);

		/*if (ftemp<0)
		{
			AfxMessageBox(_T("your input can not be negative"));
			to_fresh();
			return;
		}*/
		strtxt.Format(_T("%d"),ftemp);
		GetDlgItem(IDC_EDIT15)->SetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider6_i=ftemp;
	//}
	
	//	UpdateData();		
	refresh();
	
}

void CBuildTable1::OnEnKillfocusEdit16()
{
	
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		float ftemp;
// 		CString strtxt;
// 		GetDlgItem(IDC_EDIT16)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 
// 		strtxt.Format(_T("%.1f"),ftemp);
// 		GetDlgItem(IDC_EDIT16)->SetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		m_slider7_i=(int)(ftemp*10);
// 	}
// 	else
// 	{
		int ftemp;
		CString strtxt;
		GetDlgItem(IDC_EDIT16)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);

		/*if (ftemp<0)
		{
			AfxMessageBox(_T("your input can not be negative"));
			to_fresh();
			return;
		}*/
		strtxt.Format(_T("%d"),ftemp);
		GetDlgItem(IDC_EDIT16)->SetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider7_i=ftemp;
	//}
	
	//UpdateData();		
	refresh();

}

void CBuildTable1::OnEnKillfocusEdit17()
{
	
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		float ftemp;
// 		CString strtxt;
// 		GetDlgItem(IDC_EDIT17)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 
// 		strtxt.Format(_T("%.1f"),ftemp);
// 		GetDlgItem(IDC_EDIT17)->SetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		m_slider8_i=(int)(ftemp*10);
// 	}
// 	else
// 	{
		int ftemp;
		CString strtxt;
		GetDlgItem(IDC_EDIT17)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);

		//if (ftemp<0)
		//{
		//	AfxMessageBox(_T("your input can not be negative"));
		//	to_fresh();
		//	return;
		//}
		strtxt.Format(_T("%d"),ftemp);
		GetDlgItem(IDC_EDIT17)->SetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider8_i=ftemp;
	//}

	//UpdateData();		
	refresh();
}

void CBuildTable1::OnEnKillfocusEdit18()
{
	
	
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		float ftemp;
// 		CString strtxt;
// 		GetDlgItem(IDC_EDIT18)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 
// 		strtxt.Format(_T("%.1f"),ftemp);
// 		GetDlgItem(IDC_EDIT18)->SetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		m_slider9_i=(int)(ftemp*10);
// 	}
// 	else
// 	{
		int  ftemp;
		CString strtxt;
		GetDlgItem(IDC_EDIT18)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);

		/*if (ftemp<0)
		{
			AfxMessageBox(_T("your input can not be negative"));
			to_fresh();
			return;
		}*/
		strtxt.Format(_T("%d"),ftemp);
		GetDlgItem(IDC_EDIT18)->SetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider9_i=ftemp;
	//}
	
	//UpdateData();		
	refresh();
	
}

void CBuildTable1::OnEnKillfocusEdit19()
{
	
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION) 
// 	{
// 		float ftemp;
// 		CString strtxt;
// 		GetDlgItem(IDC_EDIT19)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 
// 		strtxt.Format(_T("%.1f"),ftemp);
// 		GetDlgItem(IDC_EDIT19)->SetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		m_slider10_i=(int)(ftemp*10);
// 
// 	}
// 	else
// 	{
		int ftemp;
		CString strtxt;
		GetDlgItem(IDC_EDIT19)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);

	/*	if (ftemp<0)
		{
			AfxMessageBox(_T("your input can not be negative"));
			to_fresh();
			return;
		}*/
		strtxt.Format(_T("%d"),ftemp);
		GetDlgItem(IDC_EDIT19)->SetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider10_i=ftemp;
	//}

	//UpdateData();		
	refresh();


}

void CBuildTable1::OnEnKillfocusEdit20()
{
	
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		float ftemp;
// 		CString strtxt;
// 		GetDlgItem(IDC_EDIT20)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 
// 		strtxt.Format(_T("%.1f"),ftemp);
// 		GetDlgItem(IDC_EDIT20)->SetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		m_slider11_i=(int)(ftemp*10);
// 	}
// 	else
// 	{
		int ftemp;
		CString strtxt;
		GetDlgItem(IDC_EDIT20)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);

		/*if (ftemp<0)
		{
			AfxMessageBox(_T("your input can not be negative"));
			to_fresh();
			return;
		}*/
		strtxt.Format(_T("%d"),ftemp);
		GetDlgItem(IDC_EDIT20)->SetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider11_i=ftemp;
	//}
	//	UpdateData();		
	refresh();

	 
}

void CBuildTable1::OnEnKillfocusEdit12()
{
	
	if(IsModfied)
	{
		UpdateData();		
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
			write_one(g_tstat_id, MODBUS_UNITS1_HIGH,first);
			write_one(g_tstat_id, MODBUS_UNITS1_LOW,second);
		}
		else if(m_InputNo==3)
		{
			write_one(g_tstat_id, MODBUS_UNITS2_HIGH,first);
			write_one(g_tstat_id, MODBUS_UNITS2_LOW,second);
		}
		IsModfied = false;
		//to_fresh();
		refresh();
	}
 
}
void CBuildTable1::OnEnChangeEdit10()
{
    
    // 发送该通知，除非重写 CDialog::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。
    IsModfied=true;
    //   在此添加控件通知处理程序代码
}

void CBuildTable1::OnEnSetfocusEdit10()
{
    
    KillTimer(1);//killtimer
}
BOOL CBuildTable1::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message == WM_KEYDOWN  )
	{
		if(pMsg->wParam == VK_RETURN)
		{
			this->GetFocus()->GetNextWindow()->SetFocus();
			return true;
		}
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

     


void CBuildTable1::OnBnClickedCancel()
{
	
	//SaveInfoDataToRegister();
	OnCancel();
}

short CBuildTable1::GetValueFromEdtCtrol(UINT ID_EDIT)
{
	CString strTxt;
	CEdit *pEdtbox=(CEdit *)GetDlgItem(ID_EDIT);
	pEdtbox->GetWindowText(strTxt);
	int n;
	if(strTxt.IsEmpty())
		return 0;
	if(strTxt.GetAt(0)=='-')
	{
		strTxt.TrimLeft(1);
		n=_wtoi(strTxt);
		return (0x1000 | n);
	}
	if(strTxt.GetAt(0)=='+')
	{
		strTxt.TrimLeft(1);
		n=_wtoi(strTxt);
		return n;
	}
	n=_wtoi(strTxt);
	return n;
}
CString CBuildTable1::GetValueFromRegister(short nValue)
{
	CString strTxt;
	int nNum=0;
	if((nValue & 0x1000)>>15==1)//为负数;
	{
		nNum= (0x7fff & nValue);
		CString strNum;
		strNum.Format(_T("%d"),nNum);
		return _T("-")+strNum;
	}
	else
	{
		CString strNum;
		strNum.Format(_T("%d"),nNum);
		return strNum;
	}
}
void CBuildTable1::refresh()
{
	

	if(m_slider11_i>=m_slider1_i)
	{
		
    	m_nMin=m_slider1_i;
    	m_nMax=m_slider11_i;
	}
	else
	{
		m_nMax=m_slider1_i;
    	m_nMin=m_slider11_i;
	}

// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		m_max.Format(_T("%.1f"),m_nMax/10.0);
// 		m_min.Format(_T("%.1f"),m_nMin/10.0);
// 		refresh_rule((float)(m_nMin/10.0),(float)(m_nMax/10.0));
// 	}
// 	else
// 	{
    
		m_max.Format(_T("%d"),m_nMax);
		m_min.Format(_T("%d"),m_nMin);
		//refresh_rule((float)m_nMin,(float)m_nMax);
//}



	

	GetDlgItem(IDC_EDIT10)->SetWindowText(m_max);
	GetDlgItem(IDC_EDIT11)->SetWindowText(m_min);

	CStatic * pstatic2 = (CStatic *)GetDlgItem(IDC_STATIC2);
	CStatic * pstatic3 = (CStatic *)GetDlgItem(IDC_STATIC3);
	CStatic * pstatic4 = (CStatic *)GetDlgItem(IDC_STATIC4);
	CStatic * pstatic5 = (CStatic *)GetDlgItem(IDC_STATIC5);
	CStatic * pstatic6 = (CStatic *)GetDlgItem(IDC_STATIC6);
	CStatic * pstatic7 = (CStatic *)GetDlgItem(IDC_STATIC7);
	CStatic * pstatic8 = (CStatic *)GetDlgItem(IDC_STATIC8);
	CStatic * pstatic9 = (CStatic *)GetDlgItem(IDC_STATIC9);
	CStatic * pstatic10 = (CStatic *)GetDlgItem(IDC_STATIC10);
	CStatic * pstatic11 = (CStatic *)GetDlgItem(IDC_STATIC11);

	
 	m_slider1_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider2_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider3_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider4_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider5_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider6_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider7_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider8_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider9_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider10_ctrl.SetRange(m_nMin,m_nMax);	
	m_slider11_ctrl.SetRange(m_nMin,m_nMax);	
	


	if(m_InputNo==2)
	{//table1 is click

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
	}
	else if(m_InputNo==3)
	{
		//table2 is click
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
	}

    m_slider1_ctrl.SetPos(m_nMax - (m_slider1_i-m_nMin));
    m_slider2_ctrl.SetPos(m_nMax - (m_slider2_i-m_nMin));
    m_slider3_ctrl.SetPos(m_nMax - (m_slider3_i-m_nMin));
    m_slider4_ctrl.SetPos(m_nMax - (m_slider4_i-m_nMin));
    m_slider5_ctrl.SetPos(m_nMax - (m_slider5_i-m_nMin));
    m_slider6_ctrl.SetPos(m_nMax - (m_slider6_i-m_nMin));
    m_slider7_ctrl.SetPos(m_nMax - (m_slider7_i-m_nMin));
    m_slider8_ctrl.SetPos(m_nMax - (m_slider8_i-m_nMin));
    m_slider9_ctrl.SetPos(m_nMax - (m_slider9_i-m_nMin));
    m_slider10_ctrl.SetPos(m_nMax - (m_slider10_i-m_nMin));
    m_slider11_ctrl.SetPos(m_nMax - (m_slider11_i-m_nMin));


	m_slider1_ctrl.Invalidate();
	m_slider2_ctrl.Invalidate();
	m_slider3_ctrl.Invalidate();
	m_slider4_ctrl.Invalidate();
	m_slider5_ctrl.Invalidate();
	m_slider6_ctrl.Invalidate();
	m_slider7_ctrl.Invalidate();
	m_slider8_ctrl.Invalidate();
	m_slider9_ctrl.Invalidate();
	m_slider10_ctrl.Invalidate();
	m_slider11_ctrl.Invalidate();
}

void CBuildTable1::SaveInfoDataToRegister()
{
	if(m_InputNo==2)
	{
	
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		CString strtxt;
// 		GetDlgItem(IDC_EDIT1)->GetWindowText(strtxt);
// 		float ftemp=(float)_wtof(strtxt);
// 		int ntemp=(int)(ftemp*10);
// 		short high=ntemp/65536;
// 		short low=ntemp%65536;
// 		write_one(g_tstat_id, MODBUS_TABLE1_ZERO,low);
// 		write_one(g_tstat_id, MODBUS_TABLE1_ZERO_HI,high);
// 
// 		GetDlgItem(IDC_EDIT8)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		ntemp=(int)(ftemp*10);
// 		high=ntemp/65536;
// 		low=ntemp%65536;
// 		write_one(g_tstat_id, MODBUS_TABLE1_HALFONE,low);
// 		write_one(g_tstat_id, MODBUS_TABLE1_HALFONE_HI,high);
// 		//3
// 		GetDlgItem(IDC_EDIT9)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		ntemp=(int)(ftemp*10);
// 		high=ntemp/65536;
// 		low=ntemp%65536;
// 		write_one(g_tstat_id, MODBUS_TABLE1_ONE,low);
// 		write_one(g_tstat_id, MODBUS_TABLE1_ONE_HI,high);
// 		//4
// 		GetDlgItem(IDC_EDIT13)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		ntemp=(int)(ftemp*10);
// 		high=ntemp/65536;
// 		low=ntemp%65536;
// 		write_one(g_tstat_id, MODBUS_TABLE1_HALFTWO,low);
// 		write_one(g_tstat_id, MODBUS_TABLE1_HALFTWO_HI,high);
// 		//5
// 		GetDlgItem(IDC_EDIT14)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		ntemp=(int)(ftemp*10);
// 		high=ntemp/65536;
// 		low=ntemp%65536;
// 		write_one(g_tstat_id, MODBUS_TABLE1_TWO,low);
// 		write_one(g_tstat_id, MODBUS_TABLE1_TWO_HI,high);
// 		//6
// 		GetDlgItem(IDC_EDIT15)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		ntemp=(int)(ftemp*10);
// 		high=ntemp/65536;
// 		low=ntemp%65536;
// 		write_one(g_tstat_id, MODBUS_TABLE1_HALFTHREE,low);
// 		write_one(g_tstat_id, MODBUS_TABLE1_HALFTHREE_HI,high);
// 		//7
// 		GetDlgItem(IDC_EDIT16)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		ntemp=(int)(ftemp*10);
// 		high=ntemp/65536;
// 		low=ntemp%65536;
// 		write_one(g_tstat_id, MODBUS_TABLE1_THREE,low);
// 		write_one(g_tstat_id, MODBUS_TABLE1_THREE_HI,high);
// 		//8
// 		GetDlgItem(IDC_EDIT17)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		ntemp=(int)(ftemp*10);
// 		high=ntemp/65536;
// 		low=ntemp%65536;
// 		write_one(g_tstat_id, MODBUS_TABLE1_HALFFOUR,low);
// 		write_one(g_tstat_id, MODBUS_TABLE1_HALFFOUR_HI,high);
// 		//9
// 		GetDlgItem(IDC_EDIT18)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		ntemp=(int)(ftemp*10);
// 		high=ntemp/65536;
// 		low=ntemp%65536;
// 		write_one(g_tstat_id, MODBUS_TABLE1_FOUR,low);
// 		write_one(g_tstat_id, MODBUS_TABLE1_FOUR_HI,high);
// 		//10
// 		GetDlgItem(IDC_EDIT19)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		ntemp=(int)(ftemp*10);
// 		high=ntemp/65536;
// 		low=ntemp%65536;
// 		write_one(g_tstat_id, MODBUS_TABLE1_HALFFIVE,low);
// 		write_one(g_tstat_id, MODBUS_TABLE1_HALFFIVE_HI,high);
// 		//11
// 		GetDlgItem(IDC_EDIT20)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		ntemp=(int)(ftemp*10);
// 		high=ntemp/65536;
// 		low=ntemp%65536;
// 		write_one(g_tstat_id, MODBUS_TABLE1_FIVE,low);
// 		write_one(g_tstat_id, MODBUS_TABLE1_FIVE_HI,high);
// 
// 	}
// 		else
// 		{
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

		//}
		

	}
	else if(m_InputNo==3)
	{
		
// 		if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 		{
// 			CString strtxt;
// 			GetDlgItem(IDC_EDIT1)->GetWindowText(strtxt);
// 			float ftemp=(float)_wtof(strtxt);
// 			int ntemp=(int)(ftemp*10);
// 			short high=ntemp/65536;
// 			short low=ntemp%65536;
// 			write_one(g_tstat_id, MODBUS_TABLE2_ZERO,low);
// 			write_one(g_tstat_id, MODBUS_TABLE2_ZERO_HI,high);
// 
// 			GetDlgItem(IDC_EDIT8)->GetWindowText(strtxt);
// 			ftemp=(float)_wtof(strtxt);
// 			ntemp=(int)(ftemp*10);
// 			high=ntemp/65536;
// 			low=ntemp%65536;
// 			write_one(g_tstat_id, MODBUS_TABLE2_HALFONE,low);
// 			write_one(g_tstat_id, MODBUS_TABLE2_HALFONE_HI,high);
// 			//3
// 			GetDlgItem(IDC_EDIT9)->GetWindowText(strtxt);
// 			ftemp=(float)_wtof(strtxt);
// 			ntemp=(int)(ftemp*10);
// 			high=ntemp/65536;
// 			low=ntemp%65536;
// 			write_one(g_tstat_id, MODBUS_TABLE2_ONE,low);
// 			write_one(g_tstat_id, MODBUS_TABLE2_ONE_HI,high);
// 			//4
// 			GetDlgItem(IDC_EDIT13)->GetWindowText(strtxt);
// 			ftemp=(float)_wtof(strtxt);
// 			ntemp=(int)(ftemp*10);
// 			high=ntemp/65536;
// 			low=ntemp%65536;
// 			write_one(g_tstat_id, MODBUS_TABLE2_HALFTWO,low);
// 			write_one(g_tstat_id, MODBUS_TABLE2_HALFTWO_HI,high);
// 			//5
// 			GetDlgItem(IDC_EDIT14)->GetWindowText(strtxt);
// 			ftemp=(float)_wtof(strtxt);
// 			ntemp=(int)(ftemp*10);
// 			high=ntemp/65536;
// 			low=ntemp%65536;
// 			write_one(g_tstat_id, MODBUS_TABLE2_TWO,low);
// 			write_one(g_tstat_id, MODBUS_TABLE2_TWO_HI,high);
// 			//6
// 			GetDlgItem(IDC_EDIT15)->GetWindowText(strtxt);
// 			ftemp=(float)_wtof(strtxt);
// 			ntemp=(int)(ftemp*10);
// 			high=ntemp/65536;
// 			low=ntemp%65536;
// 			write_one(g_tstat_id, MODBUS_TABLE2_HALFTHREE,low);
// 			write_one(g_tstat_id, MODBUS_TABLE2_HALFTHREE_HI,high);
// 			//7
// 			GetDlgItem(IDC_EDIT16)->GetWindowText(strtxt);
// 			ftemp=(float)_wtof(strtxt);
// 			ntemp=(int)(ftemp*10);
// 			high=ntemp/65536;
// 			low=ntemp%65536;
// 			write_one(g_tstat_id, MODBUS_TABLE2_THREE,low);
// 			write_one(g_tstat_id, MODBUS_TABLE2_THREE_HI,high);
// 			//8
// 			GetDlgItem(IDC_EDIT17)->GetWindowText(strtxt);
// 			ftemp=(float)_wtof(strtxt);
// 			ntemp=(int)(ftemp*10);
// 			high=ntemp/65536;
// 			low=ntemp%65536;
// 			write_one(g_tstat_id, MODBUS_TABLE2_HALFFOUR,low);
// 			write_one(g_tstat_id, MODBUS_TABLE2_HALFFOUR_HI,high);
// 			//9
// 			GetDlgItem(IDC_EDIT18)->GetWindowText(strtxt);
// 			ftemp=(float)_wtof(strtxt);
// 			ntemp=(int)(ftemp*10);
// 			high=ntemp/65536;
// 			low=ntemp%65536;
// 			write_one(g_tstat_id, MODBUS_TABLE2_FOUR,low);
// 			write_one(g_tstat_id, MODBUS_TABLE2_FOUR_HI,high);
// 			//10
// 			GetDlgItem(IDC_EDIT19)->GetWindowText(strtxt);
// 			ftemp=(float)_wtof(strtxt);
// 			ntemp=(int)(ftemp*10);
// 			high=ntemp/65536;
// 			low=ntemp%65536;
// 			write_one(g_tstat_id, MODBUS_TABLE2_HALFFIVE,low);
// 			write_one(g_tstat_id, MODBUS_TABLE2_HALFFIVE_HI,high);
// 			//11
// 			GetDlgItem(IDC_EDIT20)->GetWindowText(strtxt);
// 			ftemp=(float)_wtof(strtxt);
// 			ntemp=(int)(ftemp*10);
// 			high=ntemp/65536;
// 			low=ntemp%65536;
// 			write_one(g_tstat_id, MODBUS_TABLE2_FIVE,low);
// 			write_one(g_tstat_id, MODBUS_TABLE2_FIVE_HI,high);
// 
// 		}
// 		else
// 		{
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

		//}
	
	}
}
void CBuildTable1::OnBnClickedSaveok()
{
    if(!CheckDataisRight())
    {
        CString strPropt=_T("The volt values should be linear increase or decrease,otherwise will not be saved.\ndo you want to go back to change them now?");
        if((AfxMessageBox(strPropt, MB_YESNO|MB_ICONWARNING)==IDYES))
            return;
        else
        {
            OnOK();
            return;
        }
    }
    RestoreWaitCursor();
    SaveInfoDataToRegister();
    EndWaitCursor();
    OnOK();	
}

BOOL CBuildTable1::CheckDataisRight()
{
 
		CString strtxt;
		GetDlgItem(IDC_EDIT1)->GetWindowText(strtxt);
		int ftemp=_wtoi(strtxt);
		m_slider1_i=ftemp;

		GetDlgItem(IDC_EDIT8)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider2_i=ftemp;

		GetDlgItem(IDC_EDIT9)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider3_i=ftemp;


		GetDlgItem(IDC_EDIT13)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider4_i=ftemp;


		GetDlgItem(IDC_EDIT14)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider5_i=ftemp;


		GetDlgItem(IDC_EDIT15)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider6_i=ftemp;

		GetDlgItem(IDC_EDIT16)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider7_i=ftemp;

		GetDlgItem(IDC_EDIT17)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider8_i=ftemp;

		GetDlgItem(IDC_EDIT18)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider9_i=ftemp;

		GetDlgItem(IDC_EDIT19)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider10_i=ftemp;

		GetDlgItem(IDC_EDIT20)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider11_i=ftemp;

	//}
	
/*
	if((m_slider1_i<=m_slider2_i&&m_slider2_i<=m_slider3_i&&m_slider3_i<=m_slider4_i&&m_slider4_i<=m_slider5_i&&
		m_slider5_i<=m_slider6_i&&m_slider6_i<=m_slider7_i&&m_slider7_i
		<=m_slider8_i&&m_slider8_i<=m_slider9_i&&m_slider9_i<=m_slider10_i&&m_slider10_i<=m_slider11_i)||
		(m_slider1_i>=m_slider2_i&&m_slider2_i>=m_slider3_i&&m_slider3_i>=m_slider4_i&&m_slider4_i>=m_slider5_i&&
		m_slider5_i>=m_slider6_i&&m_slider6_i>=m_slider7_i
		&&m_slider7_i>=m_slider8_i&&m_slider8_i>=m_slider9_i&&m_slider9_i>=m_slider10_i&&m_slider10_i>=m_slider11_i))
		*/
		return TRUE;
	//return FALSE;
}

void CBuildTable1::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{

// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{	
// 		int CurPos;
// 	    float fvalue;
// 		if(pScrollBar->m_hWnd==m_slider1_ctrl.m_hWnd)
// 		{
// 
// 			CurPos= m_slider1_ctrl.GetPos();
// 		//	m_slider1_i=m_nMax+m_nMin-CurPos;
// 			fvalue=(float)((m_nMax+m_nMin-CurPos)/10.0);
// 			CString strtext;
// 			strtext.Format(_T("%.1f"),fvalue);
// 			GetDlgItem(IDC_EDIT1)->SetWindowText(strtext);
// 		}
// 		if(pScrollBar->m_hWnd==m_slider2_ctrl.m_hWnd)
// 		{
// 			CurPos= m_slider2_ctrl.GetPos();
// 
// 		//	m_slider2_i=m_nMax+m_nMin-CurPos;
// 			fvalue=(float)((m_nMax+m_nMin-CurPos)/10.0);
// 			CString strtext;
// 			strtext.Format(_T("%.1f"),fvalue);
// 			GetDlgItem(IDC_EDIT8)->SetWindowText(strtext);
// 		}
// 		if(pScrollBar->m_hWnd==m_slider3_ctrl.m_hWnd)
// 		{
// 			CurPos= m_slider3_ctrl.GetPos();
// 
// 		//	m_slider3_i==m_nMax+m_nMin-CurPos;
// 			fvalue=(float)((m_nMax+m_nMin-CurPos)/10.0);
// 			CString strtext;
// 			strtext.Format(_T("%.1f"),fvalue);
// 			GetDlgItem(IDC_EDIT9)->SetWindowText(strtext);
// 
// 
// 		}
// 		if(pScrollBar->m_hWnd==m_slider4_ctrl.m_hWnd)
// 		{
// 			CurPos= m_slider4_ctrl.GetPos();
// 		//	m_slider4_i==m_nMax+m_nMin-CurPos;
// 			fvalue=(float)((m_nMax+m_nMin-CurPos)/10.0);
// 			CString strtext;
// 			strtext.Format(_T("%.1f"),fvalue);
// 			GetDlgItem(IDC_EDIT13)->SetWindowText(strtext);
// 			m_slider4_i=CurPos;
// 
// 		}
// 		if(pScrollBar->m_hWnd==m_slider5_ctrl.m_hWnd)
// 		{
// 			CurPos= m_slider5_ctrl.GetPos();
// 		//	m_slider5_i==m_nMax+m_nMin-CurPos;
// 
// 			fvalue=(float)((m_nMax+m_nMin-CurPos)/10.0);
// 			CString strtext;
// 			strtext.Format(_T("%.1f"),fvalue);
// 			GetDlgItem(IDC_EDIT14)->SetWindowText(strtext);
// 			m_slider5_i=(int)fvalue;
// 		}
// 		if(pScrollBar->m_hWnd==m_slider6_ctrl.m_hWnd)
// 		{
// 			CurPos= m_slider6_ctrl.GetPos();
// 
// 		//	m_slider6_i==m_nMax+m_nMin-CurPos;
// 			fvalue=(float)((m_nMax+m_nMin-CurPos)/10.0);
// 			CString strtext;
// 			strtext.Format(_T("%.1f"),fvalue);
// 			GetDlgItem(IDC_EDIT15)->SetWindowText(strtext);
// 			m_slider6_i=(int)fvalue;
// 
// 		}
// 		if(pScrollBar->m_hWnd==m_slider7_ctrl.m_hWnd)
// 		{
// 			CurPos= m_slider7_ctrl.GetPos();
// 		//	m_slider7_i==m_nMax+m_nMin-CurPos;
// 			fvalue=(float)((m_nMax+m_nMin-CurPos)/10.0);
// 			CString strtext;
// 			strtext.Format(_T("%.1f"),fvalue);
// 			GetDlgItem(IDC_EDIT16)->SetWindowText(strtext);
// 			m_slider7_i=(int)fvalue;
// 		}
// 		if(pScrollBar->m_hWnd==m_slider8_ctrl.m_hWnd)
// 		{
// 			CurPos= m_slider8_ctrl.GetPos();	
// 		//	m_slider8_i==m_nMax+m_nMin-CurPos;
// 			fvalue=(float)((m_nMax+m_nMin-CurPos)/10.0);
// 			CString strtext;
// 			strtext.Format(_T("%.1f"),fvalue);
// 			GetDlgItem(IDC_EDIT17)->SetWindowText(strtext);
// 			m_slider8_i=(int)fvalue;
// 		}
// 		if(pScrollBar->m_hWnd==m_slider9_ctrl.m_hWnd)
// 		{
// 			CurPos= m_slider9_ctrl.GetPos();
// 		//	m_slider9_i==m_nMax+m_nMin-CurPos;
// 			fvalue=(float)((m_nMax+m_nMin-CurPos)/10.0);
// 			CString strtext;
// 			strtext.Format(_T("%.1f"),fvalue);
// 			GetDlgItem(IDC_EDIT18)->SetWindowText(strtext);
// 			m_slider9_i=(int)fvalue;
// 		}
// 		if(pScrollBar->m_hWnd==m_slider10_ctrl.m_hWnd)
// 		{
// 			CurPos= m_slider10_ctrl.GetPos();
// 	//		m_slider10_i==m_nMax+m_nMin-CurPos;
// 			fvalue=(float)((m_nMax+m_nMin-CurPos)/10.0);
// 			CString strtext;
// 			strtext.Format(_T("%.1f"),fvalue);
// 			GetDlgItem(IDC_EDIT19)->SetWindowText(strtext);
// 			m_slider10_i=(int)fvalue;
// 		}
// 		if(pScrollBar->m_hWnd==m_slider11_ctrl.m_hWnd)
// 		{
// 			CurPos= m_slider11_ctrl.GetPos();
// 	//		m_slider11_i==m_nMax+m_nMin-CurPos;
// 			fvalue=(float)((m_nMax+m_nMin-CurPos)/10.0);
// 			CString strtext;
// 			strtext.Format(_T("%.1f"),fvalue);
// 			GetDlgItem(IDC_EDIT20)->SetWindowText(strtext);
// 			m_slider11_i=(int)fvalue;
// 		}
// 		
// 
// 	}
// 	else
// 	{
		int CurPos;
		//float fvalue; 

		if(pScrollBar->m_hWnd==m_slider1_ctrl.m_hWnd)
		{

			CurPos= m_slider1_ctrl.GetPos();
			m_slider1_i=m_nMax+m_nMin-CurPos;
		//	fvalue=(m_nMax+m_nMin-CurPos)/10.0;
			CString strtext;
			strtext.Format(_T("%d"),m_slider1_i);
			GetDlgItem(IDC_EDIT1)->SetWindowText(strtext);
		//	m_slider1_i=CurPos;
		}
		if(pScrollBar->m_hWnd==m_slider2_ctrl.m_hWnd)
		{
			CurPos= m_slider2_ctrl.GetPos();

			m_slider2_i=m_nMax+m_nMin-CurPos;
			//fvalue=(m_nMax+m_nMin-CurPos)/10.0;
			CString strtext;
			strtext.Format(_T("%d"),m_slider2_i);
			GetDlgItem(IDC_EDIT8)->SetWindowText(strtext);
		//	m_slider2_i=CurPos;
		}
		if(pScrollBar->m_hWnd==m_slider3_ctrl.m_hWnd)
		{
			CurPos= m_slider3_ctrl.GetPos();

			m_slider3_i=m_nMax+m_nMin-CurPos;
			//fvalue=(m_nMax+m_nMin-CurPos)/10.0;
			CString strtext;
			strtext.Format(_T("%d"),m_slider3_i);
		//	AfxMessageBox(strtext);
			GetDlgItem(IDC_EDIT9)->SetWindowText(strtext);
			//m_slider3_i=CurPos;
		}
		if(pScrollBar->m_hWnd==m_slider4_ctrl.m_hWnd)
		{
			CurPos= m_slider4_ctrl.GetPos();
			m_slider4_i=m_nMax+m_nMin-CurPos;
			//fvalue=(m_nMax+m_nMin-CurPos)/10.0;
			CString strtext;
			strtext.Format(_T("%d"),m_slider4_i);
			GetDlgItem(IDC_EDIT13)->SetWindowText(strtext);
			//m_slider4_i=CurPos;
		}
		if(pScrollBar->m_hWnd==m_slider5_ctrl.m_hWnd)
		{
			CurPos= m_slider5_ctrl.GetPos();
			m_slider5_i=m_nMax+m_nMin-CurPos;
			//fvalue=(m_nMax+m_nMin-CurPos)/10.0;
			CString strtext;
			strtext.Format(_T("%d"),m_slider5_i);
			GetDlgItem(IDC_EDIT14)->SetWindowText(strtext);
		//	m_slider5_i=CurPos;
		}
		if(pScrollBar->m_hWnd==m_slider6_ctrl.m_hWnd)
		{
			CurPos= m_slider6_ctrl.GetPos();

			m_slider6_i=m_nMax+m_nMin-CurPos;
		//	fvalue=(m_nMax+m_nMin-CurPos)/10.0;
			CString strtext;
			strtext.Format(_T("%d"),m_slider6_i);
			GetDlgItem(IDC_EDIT15)->SetWindowText(strtext);
			//m_slider6_i=CurPos;

		}
		if(pScrollBar->m_hWnd==m_slider7_ctrl.m_hWnd)
		{
			CurPos= m_slider7_ctrl.GetPos();
			m_slider7_i=m_nMax+m_nMin-CurPos;
			//fvalue=(m_nMax+m_nMin-CurPos)/10.0;
			CString strtext;
			strtext.Format(_T("%d"),m_slider7_i);
			GetDlgItem(IDC_EDIT16)->SetWindowText(strtext);
		//	m_slider7_i=CurPos;
		}
		if(pScrollBar->m_hWnd==m_slider8_ctrl.m_hWnd)
		{
			CurPos= m_slider8_ctrl.GetPos();	
			m_slider8_i=m_nMax+m_nMin-CurPos;
			//fvalue=(m_nMax+m_nMin-CurPos)/10.0;
			CString strtext;
			strtext.Format(_T("%d"),m_slider8_i);
			GetDlgItem(IDC_EDIT17)->SetWindowText(strtext);
			//m_slider8_i=CurPos;
		}
		if(pScrollBar->m_hWnd==m_slider9_ctrl.m_hWnd)
		{
			CurPos= m_slider9_ctrl.GetPos();
			m_slider9_i=m_nMax+m_nMin-CurPos;
		//	fvalue=(m_nMax+m_nMin-CurPos)/10.0;
			CString strtext;
			strtext.Format(_T("%d"),m_slider9_i);
			GetDlgItem(IDC_EDIT18)->SetWindowText(strtext);
		//	m_slider9_i=CurPos;
		}
		if(pScrollBar->m_hWnd==m_slider10_ctrl.m_hWnd)
		{
			CurPos= m_slider10_ctrl.GetPos();
			m_slider10_i=m_nMax+m_nMin-CurPos;
			//fvalue=(m_nMax+m_nMin-CurPos)/10.0;
			CString strtext;
			strtext.Format(_T("%d"),m_slider10_i);
			GetDlgItem(IDC_EDIT19)->SetWindowText(strtext);
		//	m_slider10_i=CurPos;
		}
		if(pScrollBar->m_hWnd==m_slider11_ctrl.m_hWnd)
		{
			CurPos= m_slider11_ctrl.GetPos();
			m_slider11_i=m_nMax+m_nMin-CurPos;
			//fvalue=(m_nMax+m_nMin-CurPos)/10.0;
			CString strtext;
			strtext.Format(_T("%d"),m_slider11_i);
			GetDlgItem(IDC_EDIT20)->SetWindowText(strtext);
		//	m_slider11_i=CurPos;
		}
	//}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}



