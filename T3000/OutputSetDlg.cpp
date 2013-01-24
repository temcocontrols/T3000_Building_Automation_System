// OutputSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "OutputSetDlg.h"
#include "globle_function.h"

#define INDEX_OUTFIELD 0
#define NAME_OUTFIELD 1
#define VALUE_OUTFIELD 2
#define AM_OUTFIELD 3
//#define CAL_OUTFIELD 3
#define RANG_OUTFIELD 4
#define FUN_OUTFIELD 5

#define TOTAL_OUTCOLS 6
#define TOTAL_OUTROWS 8
CString Interlock[6]={_T("ON"),_T("DI1"),_T("AI1") ,_T("AI2"),_T("TIMER OR"),_T("TIMER AND")};
CString ONTPUT_FUNS[4]={_T("Normal"),_T("Rotation Timer"),_T("Lighting Control") ,_T("PWM Control")};
CString OUTPUT_ANRANGE[5]={_T("On/Off"),_T("0-10V(100%)"),_T("0-5V(100%)"),_T("2-10V(100%)"),_T("10-0V(100%)")};
CString OUTPUT_RANGE45[2]={_T("On/Off"),_T("Float(0-100%)")/*,_T("PWM(0-100%)")*/};

IMPLEMENT_DYNAMIC(COutputSetDlg, CDialog)

COutputSetDlg::COutputSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COutputSetDlg::IDD, pParent)
{
	m_version=0;
	m_nModeType=0;
	m_outRows=6;
}

COutputSetDlg::~COutputSetDlg()
{
}

void COutputSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_FlexGrid);
	DDX_Control(pDX, IDC_OVALUECOMBO, m_OutValueCmbox);
	DDX_Control(pDX, IDC_OAMCOMBO, m_outAMCmbox);
	DDX_Control(pDX, IDC_ORANGCOMBO, m_outRangCmbox);
	DDX_Control(pDX, IDC_OFUNCOMBO, m_outFunCmbox);
	DDX_Control(pDX, IDC_VALUEEDIT, m_OutValueEdt);
	DDX_Control(pDX, IDC_OUTPUTNAMEEDIT, m_outputNameEDIT);
	DDX_Control(pDX, IDC_COMBOLOCK, m_Interlockcombo);
}


BEGIN_MESSAGE_MAP(COutputSetDlg, CDialog)
	ON_BN_CLICKED(ID_EXIT, &COutputSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &COutputSetDlg::OnBnClickedCancel)
	ON_CBN_KILLFOCUS(IDC_OVALUECOMBO, &COutputSetDlg::OnCbnKillfocusOvaluecombo)
	ON_CBN_KILLFOCUS(IDC_OAMCOMBO, &COutputSetDlg::OnCbnKillfocusOamcombo)
	ON_CBN_SELCHANGE(IDC_OVALUECOMBO, &COutputSetDlg::OnCbnSelchangeOvaluecombo)
	ON_CBN_SELCHANGE(IDC_OAMCOMBO, &COutputSetDlg::OnCbnSelchangeOamcombo)
	ON_CBN_KILLFOCUS(IDC_ORANGCOMBO, &COutputSetDlg::OnCbnKillfocusOrangcombo)
	ON_CBN_SELCHANGE(IDC_ORANGCOMBO, &COutputSetDlg::OnCbnSelchangeOrangcombo)
	ON_CBN_KILLFOCUS(IDC_OFUNCOMBO, &COutputSetDlg::OnCbnKillfocusOfuncombo)
	ON_CBN_SELCHANGE(IDC_OFUNCOMBO, &COutputSetDlg::OnCbnSelchangeOfuncombo)
	ON_EN_KILLFOCUS(IDC_VALUEEDIT, &COutputSetDlg::OnEnKillfocusValueedit)
	ON_BN_CLICKED(IDC_REFRESHBUTTON, &COutputSetDlg::OnBnClickedRefreshbutton)
	ON_EN_KILLFOCUS(IDC_OUTPUTNAMEEDIT, &COutputSetDlg::OnEnKillfocusOutputnameedit)
	ON_CBN_SELCHANGE(IDC_COMBOLOCK, &COutputSetDlg::OnCbnSelchangeCombolock)
	ON_CBN_KILLFOCUS(IDC_COMBOLOCK, &COutputSetDlg::OnCbnKillfocusCombolock)
END_MESSAGE_MAP()


// COutputSetDlg message handlers

void COutputSetDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL COutputSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_outputNameEDIT.ShowWindow(FALSE);
	m_OutValueEdt.ShowWindow(SW_HIDE);
	m_OutValueCmbox.ShowWindow(SW_HIDE);
	m_outAMCmbox.ShowWindow(SW_HIDE);
	m_outRangCmbox.ShowWindow(SW_HIDE);
	m_outFunCmbox.ShowWindow(SW_HIDE);
	m_Interlockcombo.ShowWindow(SW_HIDE);//tsta5
	//m_FlexGrid.put_Cols(TOTAL_OUTCOLS);//tsta5
	m_FlexGrid.put_Cols(7);
	m_FlexGrid.put_Rows(TOTAL_OUTROWS);

	m_FlexGrid.put_TextMatrix(0,INDEX_OUTFIELD,_T(""));
	m_FlexGrid.put_ColWidth(INDEX_OUTFIELD,400);

	m_FlexGrid.put_TextMatrix(0,NAME_OUTFIELD,_T("Output Name"));
	m_FlexGrid.put_ColWidth(NAME_OUTFIELD,1000);

	m_FlexGrid.put_TextMatrix(0,VALUE_OUTFIELD,_T("Value"));
	m_FlexGrid.put_ColWidth(VALUE_OUTFIELD,1100);
	
	m_FlexGrid.put_TextMatrix(0,AM_OUTFIELD,_T("Auto/Man"));
	m_FlexGrid.put_ColWidth(AM_OUTFIELD,1100);

//	m_FlexGrid.put_TextMatrix(0,CAL_OUTFIELD,_T("Calbration"));
//	m_FlexGrid.put_ColWidth(CAL_OUTFIELD,1100);

	m_FlexGrid.put_TextMatrix(0,RANG_OUTFIELD,_T("Range"));
	m_FlexGrid.put_ColWidth(RANG_OUTFIELD,1200);

	m_FlexGrid.put_TextMatrix(0,FUN_OUTFIELD,_T("Function"));
	m_FlexGrid.put_ColWidth(FUN_OUTFIELD,1200);

	m_FlexGrid.put_TextMatrix(0,6,_T("Interlock"));//tstat5
	m_FlexGrid.put_ColWidth(6,1200);//tstat5

	m_FlexGrid.put_TextMatrix(1,NAME_OUTFIELD,g_strOutName1);
	m_FlexGrid.put_TextMatrix(2,NAME_OUTFIELD,g_strOutName2);
	m_FlexGrid.put_TextMatrix(3,NAME_OUTFIELD,g_strOutName3);
	m_FlexGrid.put_TextMatrix(4,NAME_OUTFIELD,g_strOutName4);
	m_FlexGrid.put_TextMatrix(5,NAME_OUTFIELD,g_strOutName5);
	m_FlexGrid.put_TextMatrix(6,NAME_OUTFIELD,g_strOutName6);
	m_FlexGrid.put_TextMatrix(7,NAME_OUTFIELD,g_strOutName7);
	
	for(int i=1;i<TOTAL_OUTROWS;i++)
	{
		m_FlexGrid.put_RowHeight(i,270);

		CString strIndex;
		strIndex.Format(_T("%d"), i);
		m_FlexGrid.put_TextMatrix(i, INDEX_OUTFIELD, strIndex);
		
	//	for(int k=0;k<TOTAL_OUTCOLS;k++)//tstat5
		for(int k=0;k<7;k++)
		{
			m_FlexGrid.put_ColAlignment(k,4);
			if (i%2==1)
			{
				m_FlexGrid.put_Row(i);
				m_FlexGrid.put_Col(k);
				m_FlexGrid.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				m_FlexGrid.put_Row(i);
				m_FlexGrid.put_Col(k);
				m_FlexGrid.put_CellBackColor(COLOR_CELL);
			}
		}
	}


	m_version=get_curtstat_version();
	Fresh_Grid();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void COutputSetDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
void COutputSetDlg::Fresh_Grid()
{
	
	m_nModeType=multi_register_value[7];
	switch (m_nModeType)
	{
		case 2:
		case 1:
			{
				m_outRows=6;
			}
			break;
		case 4:
			{
				m_outRows=6;
			 }
			break;
		case PM_PRESSURE:
		case PM_TSTAT5D:
			{
				m_outRows=8;

			 }break;
		case PM_TSTAT6:
		case PM_TSTAT7:
		case 16:
			{
				m_outRows=8;
			}break;
		case 17:
			{
				m_outRows=6;
			}
				break;
		case 18:
			{
				m_outRows=8;
			}
			break;
		case 19:
			{
				m_outRows=8;
			}
			break;
		default:break;
	}
	CString strTemp;
	//row1:
	int nVAlue=0;
	int nRange=0;

	if(nVAlue&1)
	{
		strTemp=_T("On");
	}
	else
	{
		strTemp=_T("Off");
	}
	m_FlexGrid.put_TextMatrix(1,VALUE_OUTFIELD,strTemp);

	int nAMVAlue=multi_register_value[310];

		//0bit
	if((int)(nAMVAlue & 0x01)==1)
	{
		strTemp=_T("Manual");
		m_FlexGrid.put_Col(VALUE_OUTFIELD);
		m_FlexGrid.put_Row(1);
		m_FlexGrid.put_CellBackColor(RGB(255,255,255));
	}
	else
	{
		strTemp=_T("Auto");
		m_FlexGrid.put_Col(VALUE_OUTFIELD);
		m_FlexGrid.put_Row(1);
		m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
	}
	m_FlexGrid.put_TextMatrix(1,AM_OUTFIELD,strTemp);
	strTemp=_T("On/Off");
	m_FlexGrid.put_TextMatrix(1,RANG_OUTFIELD,strTemp);
	if(m_version>32.2)
	{
		int nFun=multi_register_value[328];
		if(nFun>=0&&nFun<4)
		{
			strTemp=ONTPUT_FUNS[nFun];
		}
		else
		{
// 			CString str;
// 			str.Format(_T("multi_register_value[328]=%d"),multi_register_value[328]);
// 			AfxMessageBox(str);//tstat6
		}
	}
	else
	{
		strTemp=NO_APPLICATION;
	}

	m_FlexGrid.put_TextMatrix(1,FUN_OUTFIELD,strTemp);
	//row 2:
	if(nVAlue&2)
	{
		strTemp=_T("On");
	}
	else
	{
		strTemp=_T("Off");
	}
	m_FlexGrid.put_TextMatrix(2,VALUE_OUTFIELD,strTemp);

	if((int)(nAMVAlue & 2))
	{
		strTemp=_T("Manual");
		m_FlexGrid.put_Col(VALUE_OUTFIELD);
		m_FlexGrid.put_Row(2);
		m_FlexGrid.put_CellBackColor(COLOR_CELL);
	}
	else
	{
		strTemp=_T("Auto");
		m_FlexGrid.put_Col(VALUE_OUTFIELD);
		m_FlexGrid.put_Row(2);
		m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
	}
	m_FlexGrid.put_TextMatrix(2,AM_OUTFIELD,strTemp);

	strTemp=_T("On/Off");
	m_FlexGrid.put_TextMatrix(2,RANG_OUTFIELD,strTemp);
	strTemp.Empty();
	if (m_version<32.2)
	{
		short nFun=multi_register_value[328];
		int nMask;
		nMask=nFun&0x1;
		if (nMask>0)
		{
			strTemp=ONTPUT_FUNS[1];
		}
		else
		{
			strTemp=ONTPUT_FUNS[0];
		}
		m_FlexGrid.put_TextMatrix(2,FUN_OUTFIELD,strTemp);
	}
	else
	{
		int indext=-1;
		indext=multi_register_value[334];
		if(indext>=0&&indext<=3)
			strTemp=ONTPUT_FUNS[indext];
		m_FlexGrid.put_TextMatrix(2,FUN_OUTFIELD,strTemp);
	}
// row 3:
	if(nVAlue&4)
	{
		strTemp=_T("On");
	}
	else
	{
		strTemp=_T("Off");
	}
	m_FlexGrid.put_TextMatrix(3,VALUE_OUTFIELD,strTemp);
	if((int)(nAMVAlue & 4))
	{
		strTemp=_T("Manual");
		m_FlexGrid.put_Col(VALUE_OUTFIELD);
		m_FlexGrid.put_Row(3);
		m_FlexGrid.put_CellBackColor(RGB(255,255,255));
	}
	else
	{
		strTemp=_T("Auto");
		m_FlexGrid.put_Col(VALUE_OUTFIELD);
		m_FlexGrid.put_Row(3);
		m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
	}
	m_FlexGrid.put_TextMatrix(3,AM_OUTFIELD,strTemp);
	
	strTemp=_T("On/Off");
	m_FlexGrid.put_TextMatrix(3,RANG_OUTFIELD,strTemp);
	strTemp.Empty();
	if (m_version<32.2)
	{
		short nFun=multi_register_value[328];
		int nMask;
		nMask=nFun&0x2;
		if (nMask>0)
		{
			strTemp=ONTPUT_FUNS[1];
		}
		else
		{
			strTemp=ONTPUT_FUNS[0];
		}
		m_FlexGrid.put_TextMatrix(3,FUN_OUTFIELD,strTemp);
	}
	else
	{
		int indext=-1;
		indext=multi_register_value[335];
		if(indext>=0&&indext<=3)
			strTemp=ONTPUT_FUNS[indext];
		m_FlexGrid.put_TextMatrix(3,FUN_OUTFIELD,strTemp);
	}

	

	//BCDE 4,5 D/O
	if(m_nModeType==1||m_nModeType==4||m_nModeType==12||m_nModeType==16 
		||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7||m_nModeType==PM_PRESSURE)//||m_nModeType==17||m_nModeType==18)
	{
		// just for row4 ///////////////////////////////////////////////////////////////

		//	strTemp=_T("On/Off");
		//	m_FlexGrid.put_TextMatrix(5,RANG_OUTFIELD,strTemp);
		if((int)(nAMVAlue & 8))
		{
			strTemp=_T("Manual");
			m_FlexGrid.put_Col(VALUE_OUTFIELD);
			m_FlexGrid.put_Row(4);
			m_FlexGrid.put_CellBackColor(RGB(255,255,255));
		}
		else
		{
			strTemp=_T("Auto");
			m_FlexGrid.put_Col(VALUE_OUTFIELD);
			m_FlexGrid.put_Row(4);
			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		}
		m_FlexGrid.put_TextMatrix(4,AM_OUTFIELD,strTemp);
		strTemp.Empty();

		//////////////////////////////////////////////////////////////////////////
		nRange=multi_register_value[283];
		if(nRange>=0&&nRange<2)
		{
			strTemp=OUTPUT_RANGE45[nRange];
		}
		m_FlexGrid.put_TextMatrix(4,RANG_OUTFIELD,strTemp);

		if(nRange == 0 || !(nAMVAlue & 8)) // AM栏选择了Auto或者Range 栏选择了On/Off，value都显示ON/Off
		{  // output is on/off
			if(nVAlue&8)
			{
				strTemp=_T("On");
			}
			else
			{
				strTemp=_T("Off");
			}
		}
		else // output is value
		{
			int nValueTemp = multi_register_value[348];			
			strTemp.Format(_T("%d%%"), nValueTemp);
		}
		m_FlexGrid.put_TextMatrix(4,VALUE_OUTFIELD,strTemp);

		//////////////////////////////////////////////////////////////////////////

		if (m_version<32.2)
		{
			short nFun=multi_register_value[328];
			int nMask;
			nMask=nFun&0x4;
			if (nMask>0)
			{
				strTemp=ONTPUT_FUNS[1];
			}
			else
			{
				strTemp=ONTPUT_FUNS[0];
			}
			m_FlexGrid.put_TextMatrix(4,FUN_OUTFIELD,strTemp);
		}
		else
		{
			int indext=-1;
			indext=multi_register_value[336];
			if(indext>=0&&indext<=3)
				strTemp=ONTPUT_FUNS[indext];
			m_FlexGrid.put_TextMatrix(4,FUN_OUTFIELD,strTemp);
		}

		///////////////////////////////////////////////////////////////////////////
		// just for row5
	

		if((int)(nAMVAlue & 16))
		{
			strTemp=_T("Manual");
			m_FlexGrid.put_Col(VALUE_OUTFIELD);
			m_FlexGrid.put_Row(5);
			m_FlexGrid.put_CellBackColor(RGB(255,255,255));
		}
		else
		{
			strTemp=_T("Auto");
			m_FlexGrid.put_Col(VALUE_OUTFIELD);
			m_FlexGrid.put_Row(5);
			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		}
		m_FlexGrid.put_TextMatrix(5,AM_OUTFIELD,strTemp);
		strTemp.Empty();

		nRange=multi_register_value[284];
		if(nRange>=0&&nRange<2)
		{
			strTemp=OUTPUT_RANGE45[nRange];
		}
		m_FlexGrid.put_TextMatrix(5,RANG_OUTFIELD,strTemp);

		//////////////////////////////////////////////////////////////////////////
		if(nRange == 0 || !(nAMVAlue & 16))
		{			
			if(nVAlue&16)
			{
				strTemp=_T("On");
			}
			else
			{
				strTemp=_T("Off");
			}
		}
		else
		{
			int nValueTemp = multi_register_value[349];			
			strTemp.Format(_T("%d%%"), nValueTemp);
		}
		m_FlexGrid.put_TextMatrix(5,VALUE_OUTFIELD,strTemp);

		//////////////////////////////////////////////////////////////////////////

		if (m_version<32.2)
		{
			short nFun=multi_register_value[328];
			int nMask;
			nMask=nFun&0x8;
			if (nMask>0)
			{
				strTemp=ONTPUT_FUNS[1];
			}
			else
			{
				strTemp=ONTPUT_FUNS[0];
			}
			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
		}
		else
		{
			int indext=-1;
			indext=multi_register_value[337];
			if(indext>=0&&indext<=3)
				strTemp=ONTPUT_FUNS[indext];
			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//row4,5,for 5F,5G:
	if(m_nModeType==17||m_nModeType==18)
	{
		//out4:
		int nRang=multi_register_value[283];
		//int nValue;
		int nValue1=0;//=multi_register_value[108];



		if(nRang==0)
		{
			if(nValue1 &( 1<<4))
				strTemp=_T("ON");
			else
				strTemp=_T("OFF");
		}
		else
		{
			strTemp.Format(_T("%d%%"),multi_register_value[348]);
		}
		m_FlexGrid.put_TextMatrix(4,VALUE_OUTFIELD,strTemp);
//AM
		if((int)(nAMVAlue & 8))
		{
			strTemp=_T("Manual");
			m_FlexGrid.put_Col(VALUE_OUTFIELD);
			m_FlexGrid.put_Row(4);
			m_FlexGrid.put_CellBackColor(RGB(255,255,255));
		}
		else
		{
			strTemp=_T("Auto");
			m_FlexGrid.put_Col(VALUE_OUTFIELD);
			m_FlexGrid.put_Row(4);
			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		}
		m_FlexGrid.put_TextMatrix(4,AM_OUTFIELD,strTemp);
		strTemp.Empty();

			nRange=multi_register_value[283];
		if(nRange>=0&&nRange<2)
		{
			strTemp=OUTPUT_RANGE45[nRange];
		}
		m_FlexGrid.put_TextMatrix(4,RANG_OUTFIELD,strTemp);
		
		strTemp.Empty();
		if (m_version<32.2)
		{
			short nFun=multi_register_value[328];
			int nMask;
			nMask=nFun&0x4;
			if (nMask>0)
			{
				strTemp=ONTPUT_FUNS[1];
			}
			else
			{
				strTemp=ONTPUT_FUNS[0];
			}
			m_FlexGrid.put_TextMatrix(4,FUN_OUTFIELD,strTemp);
		}
		else
		{
			int indext=-1;
			indext=multi_register_value[336];
			if(indext>=0&&indext<=3)
				strTemp=ONTPUT_FUNS[indext];
			m_FlexGrid.put_TextMatrix(4,FUN_OUTFIELD,strTemp);
		}

		//out5:
		nRang=multi_register_value[284];
		//nValue1=multi_register_value[108];

	
		
		if(nRang==0)
		{
			if(nValue1 &( 1<<3))
					strTemp=_T("ON");
				else
					strTemp=_T("OFF");

		}
		else
		{
			strTemp.Format(_T("%d%%"),multi_register_value[349]);

		}
		m_FlexGrid.put_TextMatrix(5,VALUE_OUTFIELD,strTemp);

		//AM
		if((int)(nAMVAlue & 16))
		{
			strTemp=_T("Manual");
			m_FlexGrid.put_Col(VALUE_OUTFIELD);
			m_FlexGrid.put_Row(5);
			m_FlexGrid.put_CellBackColor(RGB(255,255,255));
		}
		else
		{
			strTemp=_T("Auto");
			m_FlexGrid.put_Col(VALUE_OUTFIELD);
			m_FlexGrid.put_Row(5);
			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		}
		m_FlexGrid.put_TextMatrix(5,AM_OUTFIELD,strTemp);
		strTemp.Empty();


		nRange=multi_register_value[284];
		if(nRange>=0&&nRange<2)
		{
			strTemp=OUTPUT_RANGE45[nRange];
		}
		m_FlexGrid.put_TextMatrix(5,RANG_OUTFIELD,strTemp);
		
		strTemp.Empty();
		if (m_version<32.2)
		{
			short nFun=multi_register_value[328];
			int nMask;
			nMask=nFun&0x8;
			if (nMask>0)
			{
				strTemp=ONTPUT_FUNS[1];
			}
			else
			{
				strTemp=ONTPUT_FUNS[0];
			}
			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
		}
		else
		{
			int indext=-1;
			indext=multi_register_value[337];
			if(indext>=0&&indext<=3)
				strTemp=ONTPUT_FUNS[indext];
			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
		}
	}
//row45/67 ANALOG
	if (m_nModeType==2||m_nModeType==12||m_nModeType==16||m_nModeType==PM_PRESSURE
		||m_nModeType==18||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7)//5ADEG
	{
		if(m_nModeType==2)
		{
			short nRange=multi_register_value[186];
			int nValue=multi_register_value[102];
			if(nRange==0)
			{				
				if(nValue==0)	
					strTemp=_T("Off");
				if(nValue==1000)
					strTemp=_T("On");
			}
			else
			{
				//strTemp.Format(_T("%.1f"),nValue/100.0);
					//strTemp.Format(_T("%.1f"),multi_register_value[102]/100.0);
				float nvalue=0.0;
				if(nRange==1)//0-10v
				{
					//nvalue=multi_register_value[102]/100 /10.0 * 100%;
					nvalue=multi_register_value[102]/10.0f;
				}
				if(nRange==2)//0-5v
				{
					nvalue=multi_register_value[102]/5.0f;
				}
				if(nRange==3)//2-10v
				{
					nvalue=multi_register_value[102]/8.0f;
				}
				if(nRange==3)//10-0v
				{
					nvalue=(10-multi_register_value[102]/100.0f)/10.0f *100;
				}
				strTemp.Format(_T("%.1f%%"),nvalue);
			}
			m_FlexGrid.put_TextMatrix(4,VALUE_OUTFIELD,strTemp);
			if((int)(nAMVAlue & 8))
			{
				strTemp=_T("Manual");
				m_FlexGrid.put_Col(VALUE_OUTFIELD);
				m_FlexGrid.put_Row(4);
				m_FlexGrid.put_CellBackColor(COLOR_CELL);
			}
			else
			{
				strTemp=_T("Auto");
				m_FlexGrid.put_Col(VALUE_OUTFIELD);
				m_FlexGrid.put_Row(4);
				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
			}
			m_FlexGrid.put_TextMatrix(4,AM_OUTFIELD,strTemp);

			if(nRange>=0&&nRange<5)
			{
				strTemp=OUTPUT_ANRANGE[nRange];
			}
			m_FlexGrid.put_TextMatrix(4,RANG_OUTFIELD,strTemp);

			//row5
			nRange=multi_register_value[187];
			nValue=multi_register_value[103];
			if(nRange==0)
			{
				
				if(nValue==0)
					strTemp=_T("Off");
				if(nValue==1)
					strTemp=_T("On");
				/*	
			if(nValue<500)
					strTemp=_T("Off");
				else
					strTemp=_T("On");
					*/


			}
			else
			{
				//strTemp.Format(_T("%.1f"),nValue/100.0);

				float nvalue=0.0;
						if(nRange==1)//0-10v
						{
							//nvalue=multi_register_value[102]/100 /10.0 * 100%;
							nvalue=multi_register_value[103]/10.0f;
						}
						if(nRange==2)//0-5v
						{
							nvalue=multi_register_value[103]/5.0f;
						}
						if(nRange==3)//2-10v
						{
							nvalue=multi_register_value[103]/8.0f;
						}
						if(nRange==4)//10-0v
						{
							nvalue=(10-multi_register_value[103]/100.0f)/10.0f *100;
						}
						strTemp.Format(_T("%.1f%%"),nvalue);
			}
			m_FlexGrid.put_TextMatrix(5,VALUE_OUTFIELD,strTemp);




			if((int)(nAMVAlue & 16))
			{
				strTemp=_T("Manual");
				m_FlexGrid.put_Col(VALUE_OUTFIELD);
				m_FlexGrid.put_Row(5);
				m_FlexGrid.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				strTemp=_T("Auto");
				m_FlexGrid.put_Col(VALUE_OUTFIELD);
				m_FlexGrid.put_Row(5);
				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
			}
			m_FlexGrid.put_TextMatrix(5,AM_OUTFIELD,strTemp);

			if(nRange>=0&&nRange<5)
			{
				strTemp=OUTPUT_ANRANGE[nRange];
			}
			m_FlexGrid.put_TextMatrix(5,RANG_OUTFIELD,strTemp);
		}
		else//out6/7
		{
			int nRange=multi_register_value[186];
			int nValue=multi_register_value[102];

			strTemp.Empty();
			if(nRange==0)
			{				
				if(nValue==0)
					strTemp=_T("Off");
				if(nValue==1000)
					strTemp=_T("On");			
			}
			else
			{
				//strTemp.Format(_T("%.1f"),nValue/100.0);
					//strTemp.Format(_T("%.1f"),multi_register_value[102]/100.0);
				float nvalue=0.0;
				if(nRange==1)//0-10v
				{
					//nvalue=multi_register_value[102]/100 /10.0 * 100%;
					nvalue=multi_register_value[102]/10.0f;
				}
				if(nRange==2)//0-5v
				{
					nvalue=multi_register_value[102]/5.0f;
				}
				if(nRange==3)//2-10v
				{
					nvalue=multi_register_value[102]/8.0f;
				}
				if(nRange==4)//10-0v
				{
					nvalue=(10-multi_register_value[102]/100.0f )/10.0f *100;
				}
				strTemp.Format(_T("%.1f%%"),nvalue);
			}
			m_FlexGrid.put_TextMatrix(6,VALUE_OUTFIELD,strTemp);

			if((int)(nAMVAlue & 32))
			{
				strTemp=_T("Manual");
				m_FlexGrid.put_Col(VALUE_OUTFIELD);
				m_FlexGrid.put_Row(6);
				m_FlexGrid.put_CellBackColor(COLOR_CELL);
			}
			else
			{
				strTemp=_T("Auto");
				m_FlexGrid.put_Col(VALUE_OUTFIELD);
				m_FlexGrid.put_Row(6);
				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
			}
			m_FlexGrid.put_TextMatrix(6,AM_OUTFIELD,strTemp);
			strTemp.Empty();

			if(nRange>=0&&nRange<5)
			{
				strTemp=OUTPUT_ANRANGE[nRange];
			}
			m_FlexGrid.put_TextMatrix(6,RANG_OUTFIELD,strTemp);

			nRange=multi_register_value[187];
			nValue=multi_register_value[103];
			strTemp.Empty();
			if(nRange==0)
			{
				
				if(nValue==0)
					strTemp=_T("Off");
				if(nValue==1000)
					strTemp=_T("On");
					/*
				if(nValue<500)
					strTemp=_T("Off");
				else
					strTemp=_T("On");
					*/
			}
			else
			{
				//strTemp.Format(_T("%.1f"),nValue/100.0);
				float nvalue=0.0;
						if(nRange==1)//0-10v
						{
							//nvalue=multi_register_value[102]/100 /10.0 * 100%;
							nvalue=multi_register_value[103]/10.0f;
						}
						if(nRange==2)//0-5v
						{
							nvalue=multi_register_value[103]/5.0f;
						}
						if(nRange==3)//2-10v
						{
							nvalue=multi_register_value[103]/8.0f;
						}
						if(nRange==4)//10-0v
						{
							nvalue=(10-multi_register_value[103]/100.0f)/10.0f *100;
						}
						strTemp.Format(_T("%.1f%%"),nvalue);
			}
			m_FlexGrid.put_TextMatrix(7,VALUE_OUTFIELD,strTemp);

			if((int)(nAMVAlue & 64))
			{
				strTemp=_T("Manual");
				m_FlexGrid.put_Col(VALUE_OUTFIELD);
				m_FlexGrid.put_Row(7);
				m_FlexGrid.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				strTemp=_T("Auto");
				m_FlexGrid.put_Col(VALUE_OUTFIELD);
				m_FlexGrid.put_Row(7);
				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
			}
			m_FlexGrid.put_TextMatrix(7,AM_OUTFIELD,strTemp);
			strTemp.Empty();
			if(nRange>=0&&nRange<5)
			{
				strTemp=OUTPUT_ANRANGE[nRange];
			}
			m_FlexGrid.put_TextMatrix(7,RANG_OUTFIELD,strTemp);


		}
		strTemp.Empty();

	}

	CString strlock;
	int stradd = 286;
	for (int i = 0;i<7;i++)
	{

		 int itemp = multi_register_value[stradd+i];
		 if(itemp>=0&&itemp<6)
		m_FlexGrid.put_TextMatrix(i+1,6,Interlock[itemp]);

	}




}
BEGIN_EVENTSINK_MAP(COutputSetDlg, CDialog)
	ON_EVENT(COutputSetDlg, IDC_MSFLEXGRID1, DISPID_CLICK, COutputSetDlg::ClickMsflexgrid1, VTS_NONE)
END_EVENTSINK_MAP()

void COutputSetDlg::ClickMsflexgrid1()
{
	m_FlexGrid.SetFocus();
	long lRow,lCol;
	lRow = m_FlexGrid.get_RowSel();//
	lCol = m_FlexGrid.get_ColSel(); //
	if(lRow>m_FlexGrid.get_Rows()) //
		return;
	if(lRow == 0) //
		return;
	CRect rect;
	m_FlexGrid.GetWindowRect(rect); //
	ScreenToClient(rect); //	
	
	CDC* pDC =GetDC();


}

void COutputSetDlg::OnCbnKillfocusOvaluecombo()
{
	m_OutValueCmbox.ShowWindow(SW_HIDE);
}

void COutputSetDlg::OnCbnKillfocusOamcombo()
{
	m_outAMCmbox.ShowWindow(SW_HIDE);
}

void COutputSetDlg::OnCbnSelchangeOvaluecombo()
{
	BeginWaitCursor();

	int n;

		n=multi_register_value[108];

	int i;
	int nchange=0;

	
	EndWaitCursor();

}

void COutputSetDlg::OnCbnSelchangeOamcombo()
{
	BeginWaitCursor();
	int n;
	if (multi_register_value[7] = 6)
	{
	}else
	{
		 n=multi_register_value[310];
	}
	
	int i;
	int nchange=0;

	//first 3 rows

	EndWaitCursor();
}

void COutputSetDlg::OnCbnKillfocusOrangcombo()
{
	m_outRangCmbox.ShowWindow(SW_HIDE);
}

void COutputSetDlg::OnCbnSelchangeOrangcombo()
{


}

void COutputSetDlg::OnCbnKillfocusOfuncombo()
{
	m_outFunCmbox.ShowWindow(SW_HIDE);
}

void COutputSetDlg::OnCbnSelchangeOfuncombo()
{
	int nItem=-1;
	nItem=m_outFunCmbox.GetCurSel();
	short nFun=multi_register_value[328];

}



void COutputSetDlg::OnEnKillfocusValueedit()
{
	BeginWaitCursor();
	CString strText;
	m_OutValueEdt.GetWindowText(strText);
	if(strText.IsEmpty())
		return;
	int nValue=_wtoi(strText);
	// added by zgq; 2010-12-06; 检测输入，必须在0－100之间

	EndWaitCursor();
}

BOOL COutputSetDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->wParam == VK_RETURN )
	{ 
		GetDlgItem(ID_EXIT)->SetFocus();
		return true; 
	} 

	return CDialog::PreTranslateMessage(pMsg);
}

void COutputSetDlg::OnBnClickedRefreshbutton()
{
	int i;

}

void COutputSetDlg::OnEnKillfocusOutputnameedit()
{
	CString strText;
	int lRow;
	int lCol;


}

void COutputSetDlg::OnCbnKillfocusCombolock()
{
	m_Interlockcombo.ShowWindow(SW_HIDE);
}

void COutputSetDlg::OnCbnSelchangeCombolock()
{

}