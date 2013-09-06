// BuildTable1.cpp : 实现文件
//

#include "stdafx.h"
#include "BuildTable1.h"
#include "stdafx.h"
#include "T3000.h"
#include "globle_function.h"


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
 
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, OnNMReleasedcaptureSlider1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER2, OnNMReleasedcaptureSlider2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER3, OnNMReleasedcaptureSlider3)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER4, OnNMReleasedcaptureSlider4)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER5, OnNMReleasedcaptureSlider5)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER6, OnNMReleasedcaptureSlider6)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER7, OnNMReleasedcaptureSlider7)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER8, OnNMReleasedcaptureSlider8)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER9, OnNMReleasedcaptureSlider9)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER10, OnNMReleasedcaptureSlider10)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER11, OnNMReleasedcaptureSlider11)

	ON_WM_TIMER()
	ON_WM_DESTROY()
//	ON_EN_SETFOCUS(IDC_EDIT1, OnEnSetfocusEdit1)
	ON_EN_SETFOCUS(IDC_EDIT8, OnEnSetfocusEdit8)
	ON_EN_SETFOCUS(IDC_EDIT9, OnEnSetfocusEdit9)
	ON_EN_SETFOCUS(IDC_EDIT13, OnEnSetfocusEdit13)
	ON_EN_SETFOCUS(IDC_EDIT14, OnEnSetfocusEdit14)
	ON_EN_SETFOCUS(IDC_EDIT15, OnEnSetfocusEdit15)
	ON_EN_SETFOCUS(IDC_EDIT16, OnEnSetfocusEdit16)
	ON_EN_SETFOCUS(IDC_EDIT17, OnEnSetfocusEdit17)
	ON_EN_SETFOCUS(IDC_EDIT18, OnEnSetfocusEdit18)
	ON_EN_SETFOCUS(IDC_EDIT19, OnEnSetfocusEdit19)
	ON_EN_SETFOCUS(IDC_EDIT20, OnEnSetfocusEdit20)
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
//	ON_EN_KILLFOCUS(IDC_EDIT10, OnEnKillfocusEdit10)
	ON_EN_CHANGE(IDC_EDIT10, OnEnChangeEdit10)
	ON_EN_SETFOCUS(IDC_EDIT10, OnEnSetfocusEdit10)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SAVEOK, OnBnClickedSaveok)
END_MESSAGE_MAP()


// CBuildTable1 消息处理程序

void CBuildTable1::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
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
	//multi_read_tstat(g_tstat_id);	
	if(m_InputNo==2)
	{
		//m_nMax=multi_register_value[229];
		//m_nMin=multi_register_value[219];
// 		if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 		{
// 			short slider1_low=multi_register_value[MODBUS_TABLE1_ZERO];
// 			short slider1_high=multi_register_value[MODBUS_TABLE1_ZERO_HI];
// 			m_nMin=(slider1_high*65536+slider1_low);
// 
// 			short slider11_low=multi_register_value[MODBUS_TABLE1_FIVE];
// 			short slider11_high=multi_register_value[MODBUS_TABLE1_FIVE_HI];
// 			m_nMax=(slider11_high*65536+slider11_low);
// 		}
// 		else
// 		{

		short temp[11];
		for (int i=0;i<11;i++)
		{
			temp[i]=(short)product_register_value[MODBUS_TABLE1_ZERO+i];
		}

		m_nMax=Get_Max(&temp[0],11)/10;
		m_nMin=Get_Min(&temp[0],11)/10;

		/*	m_nMax=product_register_value[MODBUS_TABLE1_FIVE]/10;
			m_nMin=product_register_value[MODBUS_TABLE1_ZERO]/10;*/
		/*}*/
	}
	else if (m_InputNo==3)
	{
		//m_nMax=multi_register_value[240];
		//m_nMin=multi_register_value[230];
// 		if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 		{
// 			short slider1_low=multi_register_value[MODBUS_TABLE2_ZERO];
// 			short slider1_high=multi_register_value[MODBUS_TABLE2_ZERO_HI];
// 			m_nMin=(slider1_high*65536+slider1_low);
// 
// 			short slider11_low=multi_register_value[MODBUS_TABLE2_FIVE];
// 			short slider11_high=multi_register_value[MODBUS_TABLE2_FIVE_HI];
// 			m_nMax=(slider11_high*65536+slider11_low);
// 		}
// 		else
// 		{
			m_nMax=product_register_value[MODBUS_TABLE2_FIVE]/10;
			m_nMin=product_register_value[MODBUS_TABLE2_ZERO]/10;
		/*}*/

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

// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		refresh_rule((float)(m_nMin/10.0),(float)(m_nMax/10.0));
// 	}
// 	else
// 	{
		//refresh_rule((float)m_nMin,(float)m_nMax);
//	}
	
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		m_max.Format(_T("%.1f"),m_nMax/10.0);
// 		m_min.Format(_T("%.1f"),m_nMin/10.0);
// 	}
// 	else
// 	{
		m_max.Format(_T("%d"),m_nMax);
		m_min.Format(_T("%d"),m_nMin);
	//}
	
	GetDlgItem(IDC_EDIT10)->SetWindowText(m_max);
	GetDlgItem(IDC_EDIT11)->SetWindowText(m_min);

	GetDlgItem(IDC_EDIT10)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT11)->EnableWindow(FALSE);


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
// 		if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 		{
// 		short slider1_low=multi_register_value[MODBUS_TABLE1_ZERO];
// 		short slider1_high=multi_register_value[MODBUS_TABLE1_ZERO_HI];
// 		m_slider1_i=slider1_high*65536+slider1_low;
// 		short slider2_low=multi_register_value[MODBUS_TABLE1_HALFONE];
// 		short slider2_high=multi_register_value[MODBUS_TABLE1_HALFONE_HI];
// 		m_slider2_i=(slider2_high*65536+slider2_low);
// 		short slider3_low=multi_register_value[MODBUS_TABLE1_ONE];
// 		short slider3_high=multi_register_value[MODBUS_TABLE1_ONE_HI];
// 		m_slider3_i=(slider3_high*65536+slider3_low);	
// 		short slider4_low=multi_register_value[MODBUS_TABLE1_HALFTWO];
// 		short slider4_high=multi_register_value[MODBUS_TABLE1_HALFTWO_HI];
// 		m_slider4_i=(slider4_high*65536+slider4_low);
// 		short slider5_low=multi_register_value[MODBUS_TABLE1_TWO];
// 		short slider5_high=multi_register_value[MODBUS_TABLE1_TWO_HI];
// 		m_slider5_i=(slider5_high*65536+slider5_low);
// 		short slider6_low=multi_register_value[MODBUS_TABLE1_HALFTHREE];
// 		short slider6_high=multi_register_value[MODBUS_TABLE1_HALFTHREE_HI];
// 		m_slider6_i=(slider6_high*65536+slider6_low);
// 		short slider7_low=multi_register_value[MODBUS_TABLE1_THREE];
// 		short slider7_high=multi_register_value[MODBUS_TABLE1_THREE_HI];
// 		m_slider7_i=(slider7_high*65536+slider7_low);
// 		short slider8_low=multi_register_value[MODBUS_TABLE1_HALFFOUR];
// 		short slider8_high=multi_register_value[MODBUS_TABLE1_HALFFOUR_HI];
// 		m_slider8_i=(slider8_high*65536+slider8_low);
// 		short slider9_low=multi_register_value[MODBUS_TABLE1_FOUR];
// 		short slider9_high=multi_register_value[MODBUS_TABLE1_FOUR_HI];
// 		m_slider9_i=(slider9_high*65536+slider9_low);
// 		short slider10_low=multi_register_value[MODBUS_TABLE1_HALFFIVE];
// 		short slider10_high=multi_register_value[MODBUS_TABLE1_HALFFIVE_HI];
// 		m_slider10_i=(slider10_high*65536+slider10_low);
// 		short slider11_low=multi_register_value[MODBUS_TABLE1_FIVE];
// 		short slider11_high=multi_register_value[MODBUS_TABLE1_FIVE_HI];
// 		m_slider11_i=(slider11_high*65536+slider11_low);
// 		}
// 		else//<CUSTOM_TABLE_FLOAT_VERSION
// 		{
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
	   //table2 is click
		/*
		this->SetWindowText("Build Table for Analog Input2");
		m_slider1_i=multi_register_value[230];
		m_slider2_i=multi_register_value[231];
		m_slider3_i=multi_register_value[232];
		m_slider4_i=multi_register_value[233];
		m_slider5_i=multi_register_value[234];
		m_slider6_i=multi_register_value[235];
		m_slider7_i=multi_register_value[236];
		m_slider8_i=multi_register_value[237];
		m_slider9_i=multi_register_value[238];
		m_slider10_i=multi_register_value[239];
		m_slider11_i=multi_register_value[240];
		UpdateData(FALSE);
	*/
// 		if(m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 		{
// 			short slider1_low=multi_register_value[MODBUS_TABLE2_ZERO];
// 			short slider1_high=multi_register_value[MODBUS_TABLE2_ZERO_HI];
// 
// 			m_slider1_i=slider1_high*65536+slider1_low;
// 
// 			short slider2_low=multi_register_value[MODBUS_TABLE2_HALFONE];
// 			short slider2_high=multi_register_value[MODBUS_TABLE2_HALFONE_HI];
// 			m_slider2_i=(slider2_high*65536+slider2_low);
// 
// 			short slider3_low=multi_register_value[MODBUS_TABLE2_ONE];
// 			short slider3_high=multi_register_value[MODBUS_TABLE2_ONE_HI];
// 			m_slider3_i=(slider3_high*65536+slider3_low);
// 
// 			short slider4_low=multi_register_value[MODBUS_TABLE2_HALFTWO];
// 			short slider4_high=multi_register_value[MODBUS_TABLE2_HALFTWO_HI];
// 			m_slider4_i=(slider4_high*65536+slider4_low);
// 
// 			short slider5_low=multi_register_value[MODBUS_TABLE2_TWO];
// 			short slider5_high=multi_register_value[MODBUS_TABLE2_TWO_HI];
// 			m_slider5_i=(slider5_high*65536+slider5_low);
// 
// 			short slider6_low=multi_register_value[MODBUS_TABLE2_HALFTHREE];
// 			short slider6_high=multi_register_value[MODBUS_TABLE2_HALFTHREE_HI];
// 			m_slider6_i=(slider6_high*65536+slider6_low);
// 
// 			short slider7_low=multi_register_value[MODBUS_TABLE2_THREE];
// 			short slider7_high=multi_register_value[MODBUS_TABLE2_THREE_HI];
// 			m_slider7_i=(slider7_high*65536+slider7_low);
// 
// 			short slider8_low=multi_register_value[MODBUS_TABLE2_HALFFOUR];
// 			short slider8_high=multi_register_value[MODBUS_TABLE2_HALFFOUR_HI];
// 			m_slider8_i=(slider8_high*65536+slider8_low);
// 
// 			short slider9_low=multi_register_value[MODBUS_TABLE2_FOUR];
// 			short slider9_high=multi_register_value[MODBUS_TABLE2_FOUR_HI];
// 			m_slider9_i=(slider9_high*65536+slider9_low);
// 
// 
// 			short slider10_low=multi_register_value[MODBUS_TABLE2_HALFFIVE];
// 			short slider10_high=multi_register_value[MODBUS_TABLE2_HALFFIVE_HI];
// 			m_slider10_i=(slider10_high*65536+slider10_low);
// 
// 			short slider11_low=multi_register_value[MODBUS_TABLE2_FIVE];
// 			short slider11_high=multi_register_value[MODBUS_TABLE2_FIVE_HI];
// 			m_slider11_i=(slider11_high*65536+slider11_low);
// 
// 		}
// 		else//<CUSTOM_TABLE_FLOAT_VERSION
// 		{
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

		//}


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

// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION) 
// 	{
// 		int npos=m_slider1_ctrl.GetPos();
// 		float fvalue=(float)((m_nMax+m_nMin-npos)/10.0);//IDC_EDIT1
// 		CString strtext;
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT1)->SetWindowText(strtext);
// 
// 		fvalue=(float)(m_slider2_i/10.0);//IDC_EDIT1
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT8)->SetWindowText(strtext);
// 
// 		fvalue=(float)(m_slider3_i/10.0);//IDC_EDIT1
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT9)->SetWindowText(strtext);
// 
// 
// 		fvalue=(float)(m_slider4_i/10.0);//IDC_EDIT1
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT13)->SetWindowText(strtext);
// 
// 		fvalue=(float)(m_slider5_i/10.0);//IDC_EDIT1
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT14)->SetWindowText(strtext);
// 
// 		fvalue=(float)(m_slider6_i/10.0);//IDC_EDIT1
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT15)->SetWindowText(strtext);
// 
// 		fvalue=(float)(m_slider7_i/10.0);//IDC_EDIT1
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT16)->SetWindowText(strtext);
// 
// 
// 		fvalue=(float)(m_slider8_i/10.0);//IDC_EDIT1
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT17)->SetWindowText(strtext);
// 
// 		fvalue=(float)(m_slider9_i/10.0);//IDC_EDIT1
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT18)->SetWindowText(strtext);
// 
// 		fvalue=(float)(m_slider10_i/10.0);//IDC_EDIT1
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT19)->SetWindowText(strtext);
// 
// 
// 		fvalue=(float)(m_slider11_i/10.0);//IDC_EDIT1
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT20)->SetWindowText(strtext);
// 	}
// 	else
// 	{
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

	//}
	
	//UpdateData(false);
}
void CBuildTable1::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	short nTemp;
	short pos=m_slider1_ctrl.GetPos();
//	m_slider1_i=MAX_VALUE-pos;
	//m_slider1_i=m_nMax+m_nMin-pos;
	nTemp=m_nMax+m_nMin-pos;
	if(m_bIncreasingMode)
	{
		if(nTemp<m_slider2_i)
			m_slider1_i=nTemp;
		else
			return;
	}
	else
	{
	}
	UpdateData(false);
	*pResult = 0;
	*/

// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		int npos=m_slider1_ctrl.GetPos();
// 		float fvalue=(float)((m_nMax+m_nMin-npos)/10.0);//IDC_EDIT1
// 		CString strtext;
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT1)->SetWindowText(strtext);
// 		//UpdateData(false);
// 		m_slider1_i=npos;
// 
// 	}
// 	else
// 	{
		int npos=m_slider1_ctrl.GetPos();
		int fvalue=(m_nMax+m_nMin-npos);//IDC_EDIT1
		CString strtext;
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT1)->SetWindowText(strtext);
		m_slider1_i=npos;
	//}

	*pResult = 0;
}
void CBuildTable1::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	short nTemp;
	int pos=m_slider2_ctrl.GetPos();
//	m_slider2_i=m_nMax+m_nMin-pos;
	nTemp=m_nMax+m_nMin-pos;
	if(m_bIncreasingMode)
	{
		if(nTemp<m_slider3_i&&nTemp>m_slider1_i)
			m_slider2_i=nTemp;
		else
			return;
	}
	else
	{
	}
	UpdateData(false);
	*pResult = 0;
	*/
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		int npos=m_slider2_ctrl.GetPos();
// 		float fvalue=(float)((m_nMax+m_nMin-npos)/10.0);
// 		CString strtext;
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT8)->SetWindowText(strtext);
// 		//	UpdateData(false);
// 		m_slider2_i=npos;
// 	}
// 	else
// 	{
		int npos=m_slider2_ctrl.GetPos();
		int fvalue=(m_nMax+m_nMin-npos);
		CString strtext;
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT8)->SetWindowText(strtext);
		//	UpdateData(false);
		m_slider2_i=npos;

	//}

	*pResult = 0;
}
void CBuildTable1::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	short nTemp;
	int pos=m_slider3_ctrl.GetPos();
	nTemp=m_nMax+m_nMin-pos;
	if(m_bIncreasingMode)
	{
		if(nTemp<m_slider4_i&&nTemp>m_slider2_i)
			m_slider3_i=nTemp;
		else
			return;
	}
	else
	{

	}

	UpdateData(false);
	*pResult = 0;
*/
// 	if(m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		int npos=m_slider3_ctrl.GetPos();
// 		float fvalue=(float)((m_nMax+m_nMin-npos)/10.0);
// 		CString strtext;
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT9)->SetWindowText(strtext);
// 		//UpdateData(false);
// 		m_slider3_i=npos;
// 	}
// 	else
// 	{
		int npos=m_slider3_ctrl.GetPos();
		int fvalue=(m_nMax+m_nMin-npos);
		CString strtext;
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT9)->SetWindowText(strtext);
		//UpdateData(false);
		m_slider3_i=npos;

	//}
	
	*pResult = 0;

}

void CBuildTable1::OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	short nTemp;
	int pos=m_slider4_ctrl.GetPos();
	nTemp=m_nMax+m_nMin-pos;
	if(m_bIncreasingMode)
	{
		if(nTemp<m_slider5_i&&nTemp>m_slider3_i)
			m_slider4_i=nTemp;
		else
			return;
	}
	else
	{
	}
	UpdateData(false);
	*pResult = 0;
	*/
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		int npos=m_slider4_ctrl.GetPos();
// 		float fvalue=(float)((m_nMax+m_nMin-npos)/10.0);
// 		CString strtext;
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT13)->SetWindowText(strtext);
// 
// 		m_slider4_i=npos;
// 	}
// 	else
// 	{
		int npos=m_slider4_ctrl.GetPos();
		int fvalue=(m_nMax+m_nMin-npos);
		CString strtext;
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT13)->SetWindowText(strtext);
		m_slider4_i=npos;

//	}
	
	//UpdateData(false);
	*pResult = 0;
}
void CBuildTable1::OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	short nTemp;
	int pos=m_slider5_ctrl.GetPos();
	nTemp=m_nMax+m_nMin-pos;
		if(m_bIncreasingMode)
	{
		if(nTemp<m_slider6_i&&m_slider3_i&&nTemp>m_slider4_i)
			m_slider5_i=nTemp;
		else
			return;
	}
	else
	{

	}
	UpdateData(false);
	*pResult = 0;
	*/
// 	if(m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		int npos=m_slider5_ctrl.GetPos();
// 		float fvalue=(float)((m_nMax+m_nMin-npos)/10.0);
// 		CString strtext;
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT14)->SetWindowText(strtext);
// 		//UpdateData(false);
// 		m_slider5_i=npos;
// 	}
// 	else
// 	{
		int npos=m_slider5_ctrl.GetPos();
		int fvalue=(m_nMax+m_nMin-npos);
		CString strtext;
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT14)->SetWindowText(strtext);
		m_slider5_i=npos;

	//}

	*pResult = 0;
}

void CBuildTable1::OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	short nTemp;
	int pos=m_slider6_ctrl.GetPos();

	nTemp=m_nMax+m_nMin-pos;
	if(m_bIncreasingMode)
	{
		if(nTemp<m_slider7_i&&nTemp>m_slider5_i)
			m_slider6_i=nTemp;
		else
			return;
	}
	else
	{

	}
	UpdateData(false);
	*pResult = 0;
	*/
// 	if(m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		int npos=m_slider6_ctrl.GetPos();
// 		float fvalue=(float)((m_nMax+m_nMin-npos)/10.0);
// 		CString strtext;
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT15)->SetWindowText(strtext);
// 
// 		m_slider6_i=npos;
// 	}
// 	else
// 	{
		int npos=m_slider6_ctrl.GetPos();
		int fvalue=(m_nMax+m_nMin-npos);
		CString strtext;
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT15)->SetWindowText(strtext);

		m_slider6_i=npos;
	//}
	
//	UpdateData(false);
	*pResult = 0;
}

void CBuildTable1::OnNMCustomdrawSlider7(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int nTemp;
	int pos=m_slider7_ctrl.GetPos();
	nTemp=m_nMax+m_nMin-pos;
	if(m_bIncreasingMode)
	{
		if(nTemp<m_slider8_i&&nTemp>m_slider6_i)
			m_slider7_i=nTemp;
		else
			return;
	}
	else
	{

	}
	UpdateData(false);
	*pResult = 0;
	*/
// 	if(m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		int npos=m_slider7_ctrl.GetPos();
// 		float fvalue=(float)((m_nMax+m_nMin-npos)/10.0);
// 		CString strtext;
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT16)->SetWindowText(strtext);
// 		//UpdateData(false);
// 		m_slider7_i=npos;
// 	}
// 	else
// 	{
		int npos=m_slider7_ctrl.GetPos();
		int fvalue=(m_nMax+m_nMin-npos);
		CString strtext;
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT16)->SetWindowText(strtext);
		m_slider7_i=npos;

	//}

	*pResult = 0;
	
}

void CBuildTable1::OnNMCustomdrawSlider8(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	short nTemp;
	int pos=m_slider8_ctrl.GetPos();

	nTemp=m_nMax+m_nMin-pos;
	if(m_bIncreasingMode)
	{
		if(nTemp<m_slider9_i&&nTemp>m_slider7_i)
			m_slider8_i=nTemp;
		else
			return;
	}
	else
	{

	}

	UpdateData(false);
	*pResult = 0;
	*/
// 	if(m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		int npos=m_slider8_ctrl.GetPos();
// 		float fvalue=(float)((m_nMax+m_nMin-npos)/10.0);
// 		CString strtext;
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT17)->SetWindowText(strtext);
// 		//UpdateData(false);
// 		m_slider8_i=npos;
// 	}
// 	else
// 	{
		int npos=m_slider8_ctrl.GetPos();
		int fvalue=(m_nMax+m_nMin-npos);
		CString strtext;
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT17)->SetWindowText(strtext);
		//UpdateData(false);
		m_slider8_i=npos;
//	}

	*pResult = 0;
}

void CBuildTable1::OnNMCustomdrawSlider9(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	short nTemp;
	int pos=m_slider9_ctrl.GetPos();
	nTemp=m_nMax+m_nMin-pos;
	if(m_bIncreasingMode)
	{
		if(nTemp<m_slider10_i&&nTemp>m_slider8_i)
			m_slider9_i=nTemp;
		else
			return;
	}
	else
	{

	}

	UpdateData(false);
	*pResult = 0;
	*/
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		int npos=m_slider9_ctrl.GetPos();
// 		float fvalue=(float)((m_nMax+m_nMin-npos)/10.0);
// 		CString strtext;
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT18)->SetWindowText(strtext);
// 		//UpdateData(false);
// 		m_slider9_i=npos;
// 	}
// 	else
// 	{
		int npos=m_slider9_ctrl.GetPos();
		int fvalue=(m_nMax+m_nMin-npos);
		CString strtext;
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT18)->SetWindowText(strtext);
		//UpdateData(false);
		m_slider9_i=npos;
	//}
	*pResult = 0;

}

void CBuildTable1::OnNMCustomdrawSlider10(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int nTemp;
	int pos=m_slider10_ctrl.GetPos();
	//	m_slider10_i=MAX_VALUE-pos;
	nTemp=m_nMax+m_nMin-pos;
	if(m_bIncreasingMode)
	{
		if(nTemp<m_slider11_i&&nTemp>m_slider9_i)
			m_slider10_i=nTemp;
		else
			return;
	}
	else
	{

	}
	UpdateData(false);
	*pResult = 0;
	*/
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		int npos=m_slider10_ctrl.GetPos();
// 		float fvalue=(float)((m_nMax+m_nMin-npos)/10.0);
// 		CString strtext;
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT19)->SetWindowText(strtext);
// 		//	UpdateData(false);
// 		m_slider10_i=npos;
// 
// 	}
// 	else
// 	{
		int npos=m_slider10_ctrl.GetPos();
		int fvalue=(m_nMax+m_nMin-npos);
		CString strtext;
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT19)->SetWindowText(strtext);
		//	UpdateData(false);
		m_slider10_i=npos;
	//}
	
	*pResult = 0;
}
void CBuildTable1::OnNMCustomdrawSlider11(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	int nTemp;
	int pos=m_slider11_ctrl.GetPos();

	nTemp=m_nMax+m_nMin-pos;
	
	if(m_bIncreasingMode)
	{
		if(nTemp>m_slider10_i)
			m_slider11_i=nTemp;
		else
			return;
	}
	else
	{
	}
	UpdateData(false);
	*pResult = 0;
	*/
// 	if(m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		int npos=m_slider11_ctrl.GetPos();
// 		float fvalue=(float)((m_nMax+m_nMin-npos)/10.0);
// 		CString strtext;
// 		strtext.Format(_T("%.1f"),fvalue);
// 		GetDlgItem(IDC_EDIT20)->SetWindowText(strtext);
// 		//	UpdateData(false);
// 		m_slider11_i=npos;
// 	}
// 	else
// 	{
		int npos=m_slider11_ctrl.GetPos();
		int fvalue=(m_nMax+m_nMin-npos);
		CString strtext;
		strtext.Format(_T("%d"),fvalue);
		GetDlgItem(IDC_EDIT20)->SetWindowText(strtext);
		//	UpdateData(false);
		m_slider11_i=npos;
//	}

	*pResult = 0;
}

void CBuildTable1::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

//	if(m_bIncreasingMode)
//	{
//		m_slider1_ctrl.SetPos(m_nMax - (m_slider1_i-m_nMin));
//	}
	
	/*
	if(m_table1_table2==false)
		write_one(tstat_id,219,m_slider1_i);
	else
		write_one(tstat_id,230,m_slider1_i);
	*/
	//AfxMessageBox("");
//	refresh();
	*pResult = 0;
}

void CBuildTable1::OnNMReleasedcaptureSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	/*if(m_bIncreasingMode)
	{
		m_slider2_ctrl.SetPos(m_nMax - (m_slider2_i-m_nMin));
	}
	*/

/*	if(m_table1_table2==false)
		write_one(tstat_id,220,m_slider2_i);
	else
		write_one(tstat_id,231,m_slider2_i);
*/
	*pResult = 0;
}

void CBuildTable1::OnNMReleasedcaptureSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	/*if(m_bIncreasingMode)
	{
		m_slider3_ctrl.SetPos(m_nMax - (m_slider3_i-m_nMin));
	}
	*/
/*
	if(m_table1_table2==false)
		write_one(tstat_id,221,m_slider3_i);
	else
		write_one(tstat_id,232,m_slider3_i);
	*/
	*pResult = 0;
}

void CBuildTable1::OnNMReleasedcaptureSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	/*if(m_bIncreasingMode)
	{
		m_slider4_ctrl.SetPos(m_nMax - (m_slider4_i-m_nMin));
	}
	*/
/*
	if(m_table1_table2==false)
		write_one(tstat_id,222,m_slider4_i);
	else
		write_one(tstat_id,233,m_slider4_i);
	*/
	*pResult = 0;
}

void CBuildTable1::OnNMReleasedcaptureSlider5(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
/*	if(m_bIncreasingMode)
	{
		m_slider5_ctrl.SetPos(m_nMax - (m_slider5_i-m_nMin));
	}
	*/
/*
	if(m_table1_table2==false)
		write_one(tstat_id,223,m_slider5_i);
	else
		write_one(tstat_id,234,m_slider5_i);
	*/
	*pResult = 0;
}

void CBuildTable1::OnNMReleasedcaptureSlider6(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
/*	if(m_bIncreasingMode)
	{
		m_slider6_ctrl.SetPos(m_nMax - (m_slider6_i-m_nMin));
	}
	*/
/*
	if(m_table1_table2==false)
		write_one(tstat_id,224,m_slider6_i);
	else
		write_one(tstat_id,235,m_slider6_i);
	*/
	*pResult = 0;
}

void CBuildTable1::OnNMReleasedcaptureSlider7(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	/*if(m_bIncreasingMode)
	{
		m_slider7_ctrl.SetPos(m_nMax - (m_slider7_i-m_nMin));
	}
	*/

	/*
	if(m_table1_table2==false)
		write_one(tstat_id,225,m_slider7_i);
	else
		write_one(tstat_id,236,m_slider7_i);
	*/
	*pResult = 0;
}

void CBuildTable1::OnNMReleasedcaptureSlider8(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
/*	if(m_bIncreasingMode)
	{
		m_slider8_ctrl.SetPos(m_nMax - (m_slider8_i-m_nMin));
	}
	*/
	/*
	if(m_table1_table2==false)
		write_one(tstat_id,226,m_slider8_i);
	else
		write_one(tstat_id,237,m_slider8_i);
	*/
	*pResult = 0;
}

void CBuildTable1::OnNMReleasedcaptureSlider9(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
/*	if(m_bIncreasingMode)
	{
		m_slider9_ctrl.SetPos(m_nMax - (m_slider9_i-m_nMin));
	}
	*/
/*
	if(m_table1_table2==false)
		write_one(tstat_id,227,m_slider9_i);
	else
		write_one(tstat_id,238,m_slider9_i);
		*/
	*pResult = 0;
}

void CBuildTable1::OnNMReleasedcaptureSlider10(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	/*if(m_bIncreasingMode)
	{
		m_slider10_ctrl.SetPos(m_nMax - (m_slider10_i-m_nMin));
	}
	*/

	/*
	if(m_table1_table2==false)
		write_one(tstat_id,228,m_slider10_i);
	else
		write_one(tstat_id,239,m_slider10_i);
	*/
	*pResult = 0;
}

void CBuildTable1::OnNMReleasedcaptureSlider11(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
/*	if(m_bIncreasingMode)
	{
		m_slider11_ctrl.SetPos(m_nMax - (m_slider11_i-m_nMin));
	}
	*/
/*
	if(m_table1_table2==false)
		write_one(tstat_id,229,m_slider11_i);
	else
		write_one(tstat_id,240,m_slider11_i);
	*/
//	refresh();
	*pResult = 0;
}

void CBuildTable1::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	to_fresh();//fresh	
	CDialog::OnTimer(nIDEvent);
}

void CBuildTable1::OnDestroy()
{
	CDialog::OnDestroy();

	KillTimer(1);//killtimer
	// TODO: 在此处添加消息处理程序代码
}

//void CBuildTable1::OnEnSetfocusEdit1()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	//KillTimer(1);//killtimer
//}

void CBuildTable1::OnEnSetfocusEdit8()
{
	// TODO: 在此添加控件通知处理程序代码
//	KillTimer(1);//killtimer
}

void CBuildTable1::OnEnSetfocusEdit9()
{
	// TODO: 在此添加控件通知处理程序代码
//	KillTimer(1);//killtimer
}

void CBuildTable1::OnEnSetfocusEdit13()
{
	// TODO: 在此添加控件通知处理程序代码
//	KillTimer(1);//killtimer
}

void CBuildTable1::OnEnSetfocusEdit14()
{
	// TODO: 在此添加控件通知处理程序代码
	//KillTimer(1);//killtimer
}

void CBuildTable1::OnEnSetfocusEdit15()
{
	// TODO: 在此添加控件通知处理程序代码
	//KillTimer(1);//killtimer
}

void CBuildTable1::OnEnSetfocusEdit16()
{
	// TODO: 在此添加控件通知处理程序代码
//	KillTimer(1);//killtimer
}

void CBuildTable1::OnEnSetfocusEdit17()
{
	// TODO: 在此添加控件通知处理程序代码
//	KillTimer(1);//killtimer
}

void CBuildTable1::OnEnSetfocusEdit18()
{
	// TODO: 在此添加控件通知处理程序代码
//	KillTimer(1);//killtimer
}

void CBuildTable1::OnEnSetfocusEdit19()
{
	// TODO: 在此添加控件通知处理程序代码
//	KillTimer(1);//killtimer
}

void CBuildTable1::OnEnSetfocusEdit20()
{
	// TODO: 在此添加控件通知处理程序代码
//	KillTimer(1);//killtimer
}



void CBuildTable1::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	
	// TODO:  在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit8()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit9()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit13()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit14()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit15()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit16()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit17()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit18()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit19()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit20()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	IsModfied=true;
}

void CBuildTable1::OnEnChangeEdit12()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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

//	SetTimer(1,ONTIMER_TIME,NULL);//设定每五秒更新一次开始	
}

void CBuildTable1::OnEnKillfocusEdit13()
{
	// TODO: 在此添加控件通知处理程序代码
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		float ftemp;
// 		CString strtxt;
// 		GetDlgItem(IDC_EDIT13)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 
// 		strtxt.Format(_T("%.1f"),ftemp);
// 		GetDlgItem(IDC_EDIT13)->SetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		m_slider4_i=(int)(ftemp*10);
// 	}
// 	else
// 	{
		int ftemp;
		CString strtxt;
		GetDlgItem(IDC_EDIT13)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		/*if (ftemp<0)
		{
			AfxMessageBox(_T("your input can not be negative"));
			to_fresh();
			return;
		}*/

		strtxt.Format(_T("%d"),ftemp);
		GetDlgItem(IDC_EDIT13)->SetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider4_i=ftemp;
	//}

	refresh();

}

void CBuildTable1::OnEnKillfocusEdit14()
{
	// TODO: 在此添加控件通知处理程序代码
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION) 
// 	{
// 		float ftemp;
// 		CString strtxt;
// 		GetDlgItem(IDC_EDIT14)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 
// 		strtxt.Format(_T("%.1f"),ftemp);
// 		GetDlgItem(IDC_EDIT14)->SetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		m_slider5_i=(int)(ftemp*10);
// 	}
// 	else
// 	{
		int ftemp;
		CString strtxt;
		GetDlgItem(IDC_EDIT14)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
	/*	if (ftemp<0)
		{
			AfxMessageBox(_T("your input can not be negative"));
			to_fresh();
			return;
		}*/
		strtxt.Format(_T("%d"),ftemp);
		GetDlgItem(IDC_EDIT14)->SetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider5_i=ftemp;
	//}

	//UpdateData();		
	refresh();
}

void CBuildTable1::OnEnKillfocusEdit15()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
	
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
//	SetTimer(1,ONTIMER_TIME,NULL);//设定每五秒更新一次开始
}

BOOL CBuildTable1::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
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

 

void CBuildTable1::OnEnChangeEdit10()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	IsModfied=true;
	// TODO:  在此添加控件通知处理程序代码
}

void CBuildTable1::OnEnSetfocusEdit10()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);//killtimer
}

void CBuildTable1::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
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
void CBuildTable1::refresh_rule(float fMin,float fMax)
{
	float faverage=(float)((fMax-fMin)/10.0);
	CString strTmp;
	float fTmp;
	strTmp.Format(_T("%.1f"),fMin);
	GetDlgItem(IDC_STATIC1)->SetWindowText(strTmp);
	fTmp=fMin+faverage;
	strTmp.Format(_T("%.1f"),fTmp);
	GetDlgItem(IDC_STATIC2)->SetWindowText(strTmp);
		
	fTmp=fTmp+faverage;
	strTmp.Format(_T("%.1f"),fTmp);
	GetDlgItem(IDC_STATIC3)->SetWindowText(strTmp);

	fTmp=fTmp+faverage;
	strTmp.Format(_T("%.1f"),fTmp);
	GetDlgItem(IDC_STATIC4)->SetWindowText(strTmp);

	fTmp=fTmp+faverage;
	strTmp.Format(_T("%.1f"),fTmp);
	GetDlgItem(IDC_STATIC5)->SetWindowText(strTmp);


	fTmp=fTmp+faverage;
	strTmp.Format(_T("%.1f"),fTmp);
	GetDlgItem(IDC_STATIC6)->SetWindowText(strTmp);


	fTmp=fTmp+faverage;
	strTmp.Format(_T("%.1f"),fTmp);
	GetDlgItem(IDC_STATIC7)->SetWindowText(strTmp);


	fTmp=fTmp+faverage;
	strTmp.Format(_T("%.1f"),fTmp);
	GetDlgItem(IDC_STATIC8)->SetWindowText(strTmp);


	fTmp=fTmp+faverage;
	strTmp.Format(_T("%.1f"),fTmp);
	GetDlgItem(IDC_STATIC9)->SetWindowText(strTmp);


	fTmp=fTmp+faverage;
	strTmp.Format(_T("%.1f"),fTmp);
	GetDlgItem(IDC_STATIC10)->SetWindowText(strTmp);

	strTmp.Format(_T("%.1f"),fMax);
	GetDlgItem(IDC_STATIC11)->SetWindowText(strTmp);


}


CString  CBuildTable1::GetUnit()
{
	return m_units_s;
}