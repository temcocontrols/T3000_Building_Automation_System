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
//CString ONTPUT_FUNS[4]={_T("Normal"),_T("Rotation Timer"),_T("Lighting Control") ,_T("PWM Control")};
CString ONTPUT_FUNS[3]={_T("Normal"),_T("Normal"),_T("Normal") };//2.5.0.98
CString OUTPUT_ANRANGE[5]={_T("On/Off"),_T("0-10V(100%)"),_T("0-5V(100%)"),_T("2-10V(100%)"),_T("10-0V(100%)")};
//CString OUTPUT_RANGE45[2]={_T("On/Off"),_T("Float(0-100%)")/*,_T("PWM(0-100%)")*/};
CString OUTPUT_RANGE45[4]={_T("On/Off"),_T("Float(Cooling)"),_T("Float(Heating)"),_T("PWM(0-100%)")};//2.5.0.98

const int REFRESH_GRID=1;
const int ENABLE_REFRESH_BUTTON=2;

IMPLEMENT_DYNAMIC(COutputSetDlg, CDialog)

COutputSetDlg::COutputSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COutputSetDlg::IDD, pParent)
{
	m_version=0;
	m_nModeType=0;
	m_outRows=6;
	OutputThread=NULL;
	b_is_fresh = false;
}

COutputSetDlg::~COutputSetDlg()
{
	if(OutputThread!=NULL)
		TerminateThread(OutputThread, 0);
	OutputThread=NULL;
	b_is_fresh = false;
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
	//DDX_Control(pDX, IDC_ONOFFSET, m_onoffset);
	//DDX_Control(pDX, IDC_OFFONSET, m_offonset);
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
	ON_MESSAGE(WM_REFRESH_OUTPUTDLG, &COutputSetDlg::RefreshGrid)//Add by Fan
	ON_WM_CLOSE()
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
	m_FlexGrid.put_Cols(9);	//原来是7 后来加了ON/OFF Time Set,ON/OFFTime left,OFF/ON Time Set OFF/ON Time left 
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

	m_FlexGrid.put_TextMatrix(0,7,_T("OFF->ON Delay"));//tstat5
	m_FlexGrid.put_ColWidth(7,1500);//tstat5
	m_FlexGrid.put_TextMatrix(0,8,_T("ON->OFF Delay"));//tstat5
	m_FlexGrid.put_ColWidth(8,1500);//tstat5

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
		for(int k=0;k<9;k++)
		{
			m_FlexGrid.put_ColAlignment(k,4);
			if (i%2==1)
			{
				m_FlexGrid.put_Row(i);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				m_FlexGrid.put_Row(i);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(COLOR_CELL);
			}
		}
	}


	//Add by Fan,  add a icon to  the button 2013 03 29
	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_REFRESH);
	((CButton *)GetDlgItem(IDC_REFRESHBUTTON))->SetIcon(hIcon);
	hIcon_Exit = AfxGetApp()->LoadIcon(IDI_ICON_EXIT);
	((CButton *)GetDlgItem(ID_EXIT))->SetIcon(hIcon_Exit);

	m_version=(int)get_curtstat_version();
	 
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
	m_nModeType = product_register_value[7];

	switch (m_nModeType)
	{
	case 2:
	case 1:	m_outRows=6;break;
	case 4:	m_outRows=6;break;
	case PM_PRESSURE: break;
	case PM_TSTAT5D:m_outRows=8;break;
	case PM_TSTAT6:
	case PM_TSTAT7:
	case 16:m_outRows=8;break;
	case 17:m_outRows=6;break;
	case 18:m_outRows=8;break;
	case 19:m_outRows=8;break;
	default:break;
	}
	CString strTemp;
	//row1:
	int nVAlue;
	//这段代码的作用是当选择的是Floating或者PWM模式的时候，
	//要根据在outputtable中 用户选择的是PID1，还是PID2 PID1 从384 读值 PID2 就是从389读值
	#if 1
	int pid_select2[7]={0};
	for(int i=0;i<7;i++)
		pid_select2[i]=product_register_value[MODBUS_PID_OUTPUT1+i];
#endif
	

	//108  209 Output1 tot 5, bit 0 thru 4 = relay 1 thru 5.  Fan.
	nVAlue = product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS]; //t5=108   t6=209;
	int nRange=0;



	//310	254	1	Low byte	W/R	"Output auto/manual enable. Bit 0 to 4 correspond to output1 to output5, bit 5 correspond to 
	//	output6, bit 6 correspond to output7. 0, auto mode; 1, manual mode."

	int nAMVAlue=0;//=multi_register_value[310];
	nAMVAlue = product_register_value[MODBUS_OUTPUT_MANU_ENABLE];
	//Row 1-3 初始化 Value，A/M，Range
	for(int i=1;i<=3;i++)
	{
		if(nVAlue&(1<<(i-1)))
		{
			strTemp=_T("On");
		}
		else
		{
			strTemp=_T("Off");
		}
		m_FlexGrid.put_TextMatrix(i,VALUE_OUTFIELD,strTemp);


		//int temp=1<<(i-1);
		//int a = nAMVAlue;
		//int b = nAMVAlue & (1<<(i-1));
		if((int)(nAMVAlue & (1<<(i-1))) == (1<<(i-1)))
		{
			strTemp=_T("Manual");
			m_FlexGrid.put_Col(VALUE_OUTFIELD);
			m_FlexGrid.put_Row(i);
			m_FlexGrid.put_CellBackColor(RGB(255,255,255));
		}
		else
		{
			strTemp=_T("Auto");
			m_FlexGrid.put_Col(VALUE_OUTFIELD);
			m_FlexGrid.put_Row(i);
			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		} 
		m_FlexGrid.put_TextMatrix(i,AM_OUTFIELD,strTemp);
		//strTemp=_T("On/Off");
		nRange=product_register_value[MODBUS_MODE_OUTPUT1+i-1];
		 
		 
		if(nRange>=0&&nRange<4)
		{
			strTemp=OUTPUT_RANGE45[nRange];
		}


		m_FlexGrid.put_TextMatrix(i,RANG_OUTFIELD,strTemp);
		
	}
	//Fan  Comments 这里有问题 ， strTemp 判断了那么久 在后面直接赋值 NO_APPLICATION ，是为什么?;
	//Row1-3 其他几列
	#if 1 
	if(m_version>32.2)
	{
		//	328	266	1	Low byte	W/R	"Output1 Function setting:
		//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
		int nFun=0;//=multi_register_value[328];//tstat6找不到对应的
		nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; //328   266
		if(nFun>=0&&nFun<4)
		{
			strTemp=ONTPUT_FUNS[nFun];
		}
	}
	else
	{
		strTemp=NO_APPLICATION;
	}
	strTemp=ONTPUT_FUNS[0];//2.5.0.98
	m_FlexGrid.put_TextMatrix(1,FUN_OUTFIELD,strTemp);
	
	if (m_version<32.2)
	{

		short nFun=0;//multi_register_value[328];
		//328	266	1	int8	1	Low byte	W/R	"Output1 Function setting:
		//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
		nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION];
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
		strTemp=ONTPUT_FUNS[0];//2.5.0.98
		m_FlexGrid.put_TextMatrix(2,FUN_OUTFIELD,strTemp);


		nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; //328  266
		nMask=nFun&0x2;
		if (nMask>0)
		{
			strTemp=ONTPUT_FUNS[1];
		}
		else
		{
			strTemp=ONTPUT_FUNS[0];
		}
		strTemp=ONTPUT_FUNS[0];//2.5.0.98
		m_FlexGrid.put_TextMatrix(3,FUN_OUTFIELD,strTemp);

	}
	else
	{
		int indext=-1;
		//334	267	1	Low byte	W/R	Output2 function setting (see above)
		indext = product_register_value[MODBUS_OUTPUT2_FUNCTION];
		strTemp=ONTPUT_FUNS[0];//2.5.0.98
		m_FlexGrid.put_TextMatrix(2,FUN_OUTFIELD,strTemp);

		//335	268	1	Low byte	W/R	Output3 function setting (see above)
		indext = product_register_value[MODBUS_OUTPUT3_FUNCTION];
		if(indext>=0&&indext<=3)
			strTemp=ONTPUT_FUNS[indext];
		strTemp=ONTPUT_FUNS[0];//2.5.0.98
		m_FlexGrid.put_TextMatrix(3,FUN_OUTFIELD,strTemp);

	}
#endif


//----------------------------------------------------------------------------------

	if(m_nModeType==1||m_nModeType==4||m_nModeType==12||m_nModeType==16 
		||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7||m_nModeType==PM_PRESSURE)//||m_nModeType==17||m_nModeType==18)
	{
		// just for row4 ///////////////////////////////////////////////////////////////
		if((int)(nAMVAlue & 8))
		{
			strTemp=_T("Manual");
			m_FlexGrid.put_Col(VALUE_OUTFIELD);
			m_FlexGrid.put_Row(4);
			//m_FlexGrid.put_CellBackColor(RGB(255,255,255));//COLOR_CELL RGB
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
		strTemp.Empty();

		//////////////////////////////////////////////////////////////////////////
		//nRange=multi_register_value[283];
		//283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 

		nRange = product_register_value[MODBUS_MODE_OUTPUT4];//283  205

		if(nRange>=0&&nRange<=3)		//Modify by Fance_0412
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
			//comments by Fance ,此前没有 348 -》对应 t6的598  ，现在有了。;所以该不该改为现在的？？？
			int nValueTemp = product_register_value[MODBUS_PWM_OUT4]; //348 //598
			strTemp.Format(_T("%d%%"), nValueTemp);
		}
		m_FlexGrid.put_TextMatrix(4,VALUE_OUTFIELD,strTemp);

		//////////////////////////////////////////////////////////////////////////

		if (m_version<32.2)
		{
			short nFun=0;//multi_register_value[328];
			nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; //328  266
			int nMask;
			nMask=nFun&0x4;
			if (nMask>0)
			{
				strTemp=ONTPUT_FUNS[1];//Fance_?_  为什么 所有的值都是 Normal;
			}
			else
			{
				strTemp=ONTPUT_FUNS[0];
			}
			strTemp=ONTPUT_FUNS[0];//2.5.0.98
			m_FlexGrid.put_TextMatrix(4,FUN_OUTFIELD,strTemp);
		}
		else
		{
			/////336	407	1	Low byte	R	Show the size of E2 chip. 0 = 24c02, 1 = 24c08/24c16. xxxx
			//336	269	1	Low byte	W/R	Output4 function setting (see above)vvvv

			int indext=-1;
			//indext=multi_register_value[336];
			//if (newtstat6[7] ==PM_TSTAT6)
			indext = product_register_value[MODBUS_OUTPUT4_FUNCTION];
			if(indext>=0&&indext<=3)
				strTemp=ONTPUT_FUNS[indext];
			strTemp=ONTPUT_FUNS[0];//2.5.0.98
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

		//nRange=multi_register_value[284];
		//284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM
		nRange = product_register_value[MODBUS_MODE_OUTPUT5];

		if(nRange>=0&&nRange<=3)
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
			int nValueTemp=product_register_value[MODBUS_PWM_OUT5];	//tstat6没有找到	349 ,599
			strTemp.Format(_T("%d%%"), nValueTemp);
		}
		m_FlexGrid.put_TextMatrix(5,VALUE_OUTFIELD,strTemp);
		//////////////////////////////////////////////////////////////////////////
		if (m_version<32.2)
		{
			//328	266	1	Low byte	W/R	"Output1 Function setting:
			//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
			short nFun;//=multi_register_value[328];
			nFun= product_register_value[MODBUS_OUTPUT1_FUNCTION];
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
			strTemp=ONTPUT_FUNS[0];//2.5.0.98
			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
		}
		else
		{
			////337	270	1	Low byte	W/R	Output5 function setting (see above)
			int indext=-1;
			indext = product_register_value[MODBUS_OUTPUT5_FUNCTION];

			if(indext>=0&&indext<=3)
				strTemp=ONTPUT_FUNS[indext];
			strTemp=ONTPUT_FUNS[0];//2.5.0.98
			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//row4,5,for 5F,5G:
	if(m_nModeType==17||m_nModeType==18)
	{
		//out4:
		int nRang=0;//multi_register_value[283];
		nRang= product_register_value[MODBUS_OUTPUT1_FUNCTION];//328  266
		int nValue1;//=multi_register_value[108];
		//---------------------Modify by Fan--------------------------------------------
		nValue1 = product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS];//108   209
		if(nRang==0)
		{
			if(nValue1 &( 1<<4))
				strTemp=_T("ON");
			else
				strTemp=_T("OFF");
		}
		else
		{
			strTemp.Format(_T("%d%%"),product_register_value[MODBUS_PWM_OUT4]);//348  598
		}
		m_FlexGrid.put_TextMatrix(4,VALUE_OUTFIELD,strTemp);
		//2.5.0.94
		if(m_nModeType == 18)
		{
			if(product_register_value[MODBUS_MODE_OUTPUT4])  //283   205
			{
				if(nValue1 &( 1<<3))
					strTemp=_T("ON");
				else
					strTemp=_T("OFF");
			}
			else
			{
				strTemp.Format(_T("%d%%"),product_register_value[MODBUS_PWM_OUT4]);//348  598
			}
		}
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
		//283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 

		nRange = product_register_value[MODBUS_MODE_OUTPUT4]; //283  205

		if(nRange>=0&&nRange<=3)
		{
			strTemp=OUTPUT_RANGE45[nRange];
		}
		m_FlexGrid.put_TextMatrix(4,RANG_OUTFIELD,strTemp);

		strTemp.Empty();
		if (m_version<32.2)
		{
			//328	266	1	Low byte	W/R	"Output1 Function setting:
			//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "

			short nFun=0;//multi_register_value[328];
			nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION];
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
			//336	269	1	Low byte	W/R	Output4 function setting (see above)
			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
			{
				indext=product_register_value[269];
			}else
			{
				//indext=multi_register_value[336];//2.5.0.96
				indext=product_register_value[283];//2.5.0.98		//Fance_?_   //269 对应 336 这里为什么 要变为 283;

			}
			if(indext>=0&&indext<=3)
				strTemp=ONTPUT_FUNS[indext];
			if (indext >=2)//2.5.0.98
			{
				strTemp=ONTPUT_FUNS[0];
			}

			m_FlexGrid.put_TextMatrix(4,FUN_OUTFIELD,strTemp);
		}

		//out5:
		//284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM

		////337	270	1	Low byte	W/R	Output5 function setting (see above)
		if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
		{
			nRang=product_register_value[206];
		}else
		{
			nRang=product_register_value[337];			//Fance_?_ 不明白 为什么 tstat5 要用337=MODBUS_OUTPUT5_FUNCTION 而tstat 要用206=MODBUS_MODE_OUTPUT5;
		}
		//108	209	1	Low byte	W/R	Output1 tot 5, bit 0 thru 4 = relay 1 thru 5.

		nValue1 = product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS];//108   209
		if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
		{
			if(nRang==0)
			{
				if(nValue1 &( 1<<3))
					//if(nValue1 &( 1<<4))//2.5.0.94
					strTemp=_T("ON");
				else
					strTemp=_T("OFF");
			}
			else
			{	
				strTemp.Format(_T("%d%%"),product_register_value[MODBUS_PWM_OUT5]);//找不到 现在找到了 349  599;
			}
			m_FlexGrid.put_TextMatrix(5,VALUE_OUTFIELD,strTemp);

		}else if (product_register_value[7]==18)
		{
			//2.5.0.94
			if(product_register_value[MODBUS_MODE_OUTPUT5]==0)// 284   206
			{
				if(nValue1 &( 1<<4))//2.5.0.94
					strTemp=_T("ON");
				else
					strTemp=_T("OFF");
			}
			else
			{
				strTemp.Format(_T("%d%%"),product_register_value[MODBUS_PWM_OUT5]);//349  599 //Fance_?
			}
			m_FlexGrid.put_TextMatrix(5,VALUE_OUTFIELD,strTemp);
			//2.5.0.94
		}else
		{
			nRang = product_register_value[MODBUS_MODE_OUTPUT5];// 284   206
			//	nRang=multi_register_value[284];
			if(nRang==0)
			{
				if(nValue1 &( 1<<3))
					//if(nValue1 &( 1<<4))//2.5.0.94
					strTemp=_T("ON");
				else
					strTemp=_T("OFF");
			}
			else
			{
				strTemp.Format(_T("%d%%"),product_register_value[MODBUS_PWM_OUT5]);//找不到  349 599
			}
			m_FlexGrid.put_TextMatrix(5,VALUE_OUTFIELD,strTemp);

		}
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

		//284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM



		nRange = product_register_value[MODBUS_MODE_OUTPUT5];  //284  206
		if(nRange>=0&&nRange<=3)
		{
			strTemp=OUTPUT_RANGE45[nRange];
		}
		m_FlexGrid.put_TextMatrix(5,RANG_OUTFIELD,strTemp);
		strTemp.Empty();
		if (m_version<32.2)
		{
			//328	266	1	Low byte	W/R	"Output1 Function setting:
			//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "

			short nFun;//=multi_register_value[328];
			nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; // 328  266;

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
			//337	270	1	Low byte	W/R	Output5 function setting (see above)

			int indext=-1;
			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
			{
				indext=product_register_value[270];
			}else
			{
				//indext=multi_register_value[337];//2.5.0.96
				indext=product_register_value[284];//2.5.0.98			//Fance_?_  为什么要不对应;

			}

			if(indext>=0&&indext<=3)
				strTemp=ONTPUT_FUNS[indext];
			if (indext >=2)//2.5.0.98
			{
				strTemp=ONTPUT_FUNS[0];
			}
			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
		}
	}
	//row45/67 ANALOG
	if (m_nModeType==2||m_nModeType==12||m_nModeType==16||m_nModeType==PM_PRESSURE
		||m_nModeType==18||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7)//5ADEG
	{
		//186	207	1	Low byte	W/R	Analog Output1 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
		//102	210	2	Full	W/R(write only when manual output6 enable)	Output6 ,Analog output1, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.

		short nRange;//=multi_register_value[186];
		int nValue;//e=multi_register_value[102];
		//if (newtstat6[7] == 6)
		nRange = product_register_value[MODBUS_OUTPUT1_SCALE]; //186  207
		nValue = product_register_value[MODBUS_COOLING_VALVE]; //102  210

		if(nRange==0)
		{				
			if(nValue==0)	
				strTemp=_T("Off");
			if(nValue==1000)
				strTemp=_T("On");
		}
		else
		{
			float nvalue=0.0;
			//if (newtstat6[7] == 6)
			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
			{
				//AfxMessageBox(_T("It's impossible to enter this place!"));
				if(nRange==1)//0-10v
				{
					nvalue= product_register_value[MODBUS_COOLING_VALVE]/10.0f;//  T6=210
				}
				if(nRange==2)//0-5v
				{
					nvalue=product_register_value[MODBUS_COOLING_VALVE]/5.0f;
				}
				if(nRange==3)//2-10v
				{
					nvalue=product_register_value[MODBUS_COOLING_VALVE]/8.0f;
				}
				if(nRange==3)//10-0v
				{
					nvalue=(10-product_register_value[MODBUS_COOLING_VALVE]/100.0f)/10.0f *100;
				}
			}
			else
			{


				if(nRange==1)//0-10v
				{
					//nvalue=multi_register_value[102]/100 /10.0 * 100%;
					nvalue=product_register_value[MODBUS_COOLING_VALVE]/10.0f;//102   210
				}
				if(nRange==2)//0-5v
				{
					nvalue=product_register_value[MODBUS_COOLING_VALVE]/5.0f;
				}
				if(nRange==3)//2-10v
				{
					nvalue=product_register_value[MODBUS_COOLING_VALVE]/8.0f;
				}
				if(nRange==3)//10-0v
				{
					nvalue=(10-product_register_value[MODBUS_COOLING_VALVE]/100.0f)/10.0f *100;
				}
			}
			strTemp.Format(_T("%.1f%%"),nvalue);
		}
		if(m_nModeType==2)
		{

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

			if(nRange>=0&&nRange<4)
			{
				strTemp=OUTPUT_ANRANGE[nRange];
			}
			m_FlexGrid.put_TextMatrix(4,RANG_OUTFIELD,strTemp);

			//row5


			//103	211	2	Full	W/R(write only when manual output7 enable)	Output7 Analog output2, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.    
			//187	208	1	Low byte	W/R	Analog Output2 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 

			nRange= product_register_value[MODBUS_OUTPUT2_SCALE];//187  208
			nValue= product_register_value[MODBUS_HEATING_VALVE];//103  211

			if(nRange==0)
			{

				if(nValue==0)
					strTemp=_T("Off");
				if(nValue==1)
					strTemp=_T("On");
			}
			else
			{
				//strTemp.Format(_T("%.1f"),nValue/100.0);

				float nvalue=0.0;
				//if (newtstat6[7] == 6)
				if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
				{
					//AfxMessageBox(_T("It's impossible to enter this place!222"));
					if(nRange==1)//0-10v
					{
						//nvalue=multi_register_value[102]/100 /10.0 * 100%;
						nvalue=product_register_value[MODBUS_HEATING_VALVE]/10.0f;//103 211
					}
					if(nRange==2)//0-5v
					{
						nvalue=product_register_value[MODBUS_HEATING_VALVE]/5.0f;
					}
					if(nRange==3)//2-10v
					{
						nvalue=product_register_value[MODBUS_HEATING_VALVE]/8.0f;
					}
					if(nRange==4)//10-0v
					{
						nvalue=(10-product_register_value[MODBUS_HEATING_VALVE]/100.0f)/10.0f *100;
					}
				}else
				{
					if(nRange==1)//0-10v
					{
						//nvalue=multi_register_value[102]/100 /10.0 * 100%;
						nvalue= product_register_value[MODBUS_HEATING_VALVE]/10.0f;//103   211
					}
					if(nRange==2)//0-5v
					{
						nvalue= product_register_value[MODBUS_HEATING_VALVE]/5.0f;
					}
					if(nRange==3)//2-10v
					{
						nvalue= product_register_value[MODBUS_HEATING_VALVE]/8.0f;
					}
					if(nRange==4)//10-0v
					{
						nvalue=(10- product_register_value[MODBUS_HEATING_VALVE]/100.0f)/10.0f *100;
					}
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
		}//end ==2
		else//out6/7
		{

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


			//103	211	2	Full	W/R(write only when manual output7 enable)	Output7 Analog output2, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.    
			//187	208	1	Low byte	W/R	Analog Output2 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
			//if (newtstat6[7] == 6)



			nRange= product_register_value[MODBUS_OUTPUT2_SCALE];//187  208
			nValue= product_register_value[MODBUS_HEATING_VALVE];//103  211


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
				float nvalue=0.0;
				//if (newtstat6[7] == 6)
				if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
				{
					if(nRange==1)//0-10v
					{
						//nvalue=multi_register_value[102]/100 /10.0 * 100%;
						nvalue=product_register_value[MODBUS_HEATING_VALVE]/10.0f;//103 211
					}
					if(nRange==2)//0-5v
					{
						nvalue=product_register_value[MODBUS_HEATING_VALVE]/5.0f;
					}
					if(nRange==3)//2-10v
					{
						nvalue=product_register_value[MODBUS_HEATING_VALVE]/8.0f;
					}
					if(nRange==4)//10-0v
					{
						nvalue=(10-product_register_value[MODBUS_HEATING_VALVE]/100.0f)/10.0f *100;
					}
				}else
				{
					if(nRange==1)//0-10v
					{
						//nvalue=multi_register_value[102]/100 /10.0 * 100%;
						nvalue=product_register_value[MODBUS_HEATING_VALVE]/10.0f;
					}
					if(nRange==2)//0-5v
					{
						nvalue=product_register_value[MODBUS_HEATING_VALVE]/5.0f;
					}
					if(nRange==3)//2-10v
					{
						nvalue=product_register_value[MODBUS_HEATING_VALVE]/8.0f;
					}
					if(nRange==4)//10-0v
					{
						nvalue=(10-product_register_value[MODBUS_HEATING_VALVE]/100.0f)/10.0f *100;
					}
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
	//286	245	1	Low byte	W/R	Interlock for  output1

	CString strlock;
	int stradd;// = 286;
	//if (newtstat6[7] == 6)
	if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
	{
		stradd =MODBUS_INTERLOCK_OUTPUT1; //245;
		for (int i = 0;i<7;i++)
		{

			int itemp = product_register_value[stradd+i];
			if(itemp>=0&&itemp<6)
				m_FlexGrid.put_TextMatrix(i+1,6,Interlock[itemp]);

		}
	}else
	{
		stradd =MODBUS_INTERLOCK_OUTPUT1;// 286;
		for (int i = 0;i<7;i++)
		{

			int itemp = product_register_value[stradd+i];
			if(itemp>=0&&itemp<6)
				m_FlexGrid.put_TextMatrix(i+1,6,Interlock[itemp]);

		}


	}
	if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
	{  CString strdelay;
	for (int row=0;row<7;row++)
	{
		stradd=MODBUS_OUTPUT1_DELAY_OFF_TO_ON+row;	 //213
		int itemp=product_register_value[stradd];
		strdelay.Format(_T("%d"),itemp);
		m_FlexGrid.put_TextMatrix(row+1,7,strdelay);
	}
	for (int row=0;row<7;row++)
	{
		stradd=MODBUS_OUTPUT1_DELAY_ON_TO_OFF+row;	 //227
		int itemp=product_register_value[stradd];
		strdelay.Format(_T("%d"),itemp);
		m_FlexGrid.put_TextMatrix(row+1,8,strdelay);
	}
	}
	else
	{
		CString strdelay;
		for (int row=0;row<5;row++)
		{
			stradd=MODBUS_OUTPUT1_DELAY_OFF_TO_ON+row;	 //213
			int itemp=product_register_value[stradd];
			strdelay.Format(_T("%d"),itemp);
			m_FlexGrid.put_TextMatrix(row+1,7,strdelay);
		}
		for (int row=0;row<5;row++)
		{
			stradd=MODBUS_OUTPUT1_DELAY_ON_TO_OFF+row;	 //227
			int itemp=product_register_value[stradd];
			strdelay.Format(_T("%d"),itemp);
			m_FlexGrid.put_TextMatrix(row+1,8,strdelay);
		}
	}
	if ((product_register_value[7] == 6)||(product_register_value[7] == 7)){
	nAMVAlue = product_register_value[MODBUS_OUTPUT_MANU_ENABLE];

	for (int row=1;row<=5;row++)
	{

		 
		

	   if (product_register_value[MODBUS_MODE_OUTPUT1+row-1]==3)
	   {
	       strTemp=_T("Auto");
		   m_FlexGrid.put_Col(AM_OUTFIELD);
		   m_FlexGrid.put_Row(row);
		   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		   m_FlexGrid.put_Col(VALUE_OUTFIELD);
		   m_FlexGrid.put_Row(row);
		   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
	   }
	   else
	   {
		   m_FlexGrid.put_Col(AM_OUTFIELD);
		   m_FlexGrid.put_Row(row);
		   m_FlexGrid.put_CellBackColor(RGB(255,255,255));

		   if((int)(nAMVAlue & (1<<(row-1))) == (1<<(row-1)))
		   {
			   strTemp=_T("Manual");
			   m_FlexGrid.put_Col(VALUE_OUTFIELD);
			   m_FlexGrid.put_Row(row);
			   m_FlexGrid.put_CellBackColor(RGB(255,255,255));
		   }
		   else
		   {
		    strTemp=_T("Auto");
			   m_FlexGrid.put_Col(VALUE_OUTFIELD);
			   m_FlexGrid.put_Row(row);
			   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		   }
	   }
	   m_FlexGrid.put_TextMatrix(row,AM_OUTFIELD,strTemp);
	}

	}



}


void COutputSetDlg::Fresh_GridForTstat6()
{
	m_outRows=8;

	CString strTemp;
	//row1:
	int nVAlue;


	//108  209 Output1 tot 5, bit 0 thru 4 = relay 1 thru 5.  Fan.
	nVAlue = product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS]; //t5=108   t6=209;
	int nRange=0;



	//310	254	1	Low byte	W/R	"Output auto/manual enable. Bit 0 to 4 correspond to output1 to output5, bit 5 correspond to 
	//	output6, bit 6 correspond to output7. 0, auto mode; 1, manual mode."

	int nAMVAlue=0;//=multi_register_value[310];
	nAMVAlue = product_register_value[MODBUS_OUTPUT_MANU_ENABLE];

	for(int i=1;i<=3;i++)
	{
		if(nVAlue&(1<<(i-1)))
		{
			strTemp=_T("On");
		}
		else
		{
			strTemp=_T("Off");
		}
		m_FlexGrid.put_TextMatrix(i,VALUE_OUTFIELD,strTemp);


		//int temp=1<<(i-1);
		//int a = nAMVAlue;
		//int b = nAMVAlue & (1<<(i-1));
		if((int)(nAMVAlue & (1<<(i-1))) == (1<<(i-1)))
		{
			strTemp=_T("Manual");
			m_FlexGrid.put_Col(VALUE_OUTFIELD);
			m_FlexGrid.put_Row(i);
			m_FlexGrid.put_CellBackColor(RGB(255,255,255));
		}
		else
		{
			strTemp=_T("Auto");
			m_FlexGrid.put_Col(VALUE_OUTFIELD);
			m_FlexGrid.put_Row(i);
			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		} 
		m_FlexGrid.put_TextMatrix(i,AM_OUTFIELD,strTemp);
		strTemp=_T("On/Off");
		m_FlexGrid.put_TextMatrix(i,RANG_OUTFIELD,strTemp);

	}
	//Fan  Comments 这里有问题 ， strTemp 判断了那么久 在后面直接赋值 NO_APPLICATION ，是为什么?;
	if(m_version>32.2)
	{
		//	328	266	1	Low byte	W/R	"Output1 Function setting:
		//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
		int nFun=0;//=multi_register_value[328];//tstat6找不到对应的
		nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; //328   266
		if(nFun>=0&&nFun<4)
		{
			strTemp=ONTPUT_FUNS[nFun];
		}
	}
	else
	{
		strTemp=NO_APPLICATION;
	}
	strTemp=ONTPUT_FUNS[0];//2.5.0.98
	m_FlexGrid.put_TextMatrix(1,FUN_OUTFIELD,strTemp);

	if (m_version<32.2)
	{

		short nFun=0;//multi_register_value[328];
		//328	266	1	int8	1	Low byte	W/R	"Output1 Function setting:
		//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
		nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION];
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
		strTemp=ONTPUT_FUNS[0];//2.5.0.98
		m_FlexGrid.put_TextMatrix(2,FUN_OUTFIELD,strTemp);


		nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; //328  266
		nMask=nFun&0x2;
		if (nMask>0)
		{
			strTemp=ONTPUT_FUNS[1];
		}
		else
		{
			strTemp=ONTPUT_FUNS[0];
		}
		strTemp=ONTPUT_FUNS[0];//2.5.0.98
		m_FlexGrid.put_TextMatrix(3,FUN_OUTFIELD,strTemp);

	}
	else
	{
		int indext=-1;
		//334	267	1	Low byte	W/R	Output2 function setting (see above)
		indext = product_register_value[MODBUS_OUTPUT2_FUNCTION];
		strTemp=ONTPUT_FUNS[0];//2.5.0.98
		m_FlexGrid.put_TextMatrix(2,FUN_OUTFIELD,strTemp);

		//335	268	1	Low byte	W/R	Output3 function setting (see above)
		indext = product_register_value[MODBUS_OUTPUT3_FUNCTION];
		if(indext>=0&&indext<=3)
			strTemp=ONTPUT_FUNS[indext];
		strTemp=ONTPUT_FUNS[0];//2.5.0.98
		m_FlexGrid.put_TextMatrix(3,FUN_OUTFIELD,strTemp);

	}

	//----------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------
	if(m_nModeType==1||m_nModeType==4||m_nModeType==12||m_nModeType==16 
		||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7||m_nModeType==PM_PRESSURE)//||m_nModeType==17||m_nModeType==18)
	{
		// just for row4 ///////////////////////////////////////////////////////////////
		if((int)(nAMVAlue & 8))
		{
			strTemp=_T("Manual");
			m_FlexGrid.put_Col(VALUE_OUTFIELD);
			m_FlexGrid.put_Row(4);
			//m_FlexGrid.put_CellBackColor(RGB(255,255,255));//COLOR_CELL RGB
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
		strTemp.Empty();

		//////////////////////////////////////////////////////////////////////////
		//nRange=multi_register_value[283];
		//283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 

		nRange = product_register_value[MODBUS_MODE_OUTPUT4];//283  205

		if(nRange>=0&&nRange<=3)		//Modify by Fance_0412
			//if(nRange>=0&&nRange<2)
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
			//comments by Fance ,此前没有 348 -》对应 t6的598  ，现在有了。;所以该不该改为现在的？？？
			int nValueTemp = product_register_value[MODBUS_PWM_OUT4]; //348 //598
			strTemp.Format(_T("%d%%"), nValueTemp);
		}
		m_FlexGrid.put_TextMatrix(4,VALUE_OUTFIELD,strTemp);

		//////////////////////////////////////////////////////////////////////////

		if (m_version<32.2)
		{
			short nFun=0;//multi_register_value[328];
			nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; //328  266
			int nMask;
			nMask=nFun&0x4;
			if (nMask>0)
			{
				strTemp=ONTPUT_FUNS[1];//Fance_?_  为什么 所有的值都是 Normal;
			}
			else
			{
				strTemp=ONTPUT_FUNS[0];
			}
			strTemp=ONTPUT_FUNS[0];//2.5.0.98
			m_FlexGrid.put_TextMatrix(4,FUN_OUTFIELD,strTemp);
		}
		else
		{
			/////336	407	1	Low byte	R	Show the size of E2 chip. 0 = 24c02, 1 = 24c08/24c16. xxxx
			//336	269	1	Low byte	W/R	Output4 function setting (see above)vvvv

			int indext=-1;
			//indext=multi_register_value[336];
			//if (newtstat6[7] ==PM_TSTAT6)
			indext = product_register_value[MODBUS_OUTPUT4_FUNCTION];
			if(indext>=0&&indext<=3)
				strTemp=ONTPUT_FUNS[indext];
			strTemp=ONTPUT_FUNS[0];//2.5.0.98
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

		//nRange=multi_register_value[284];
		//284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM
		nRange = product_register_value[MODBUS_MODE_OUTPUT5];

		if(nRange>=0&&nRange<=3)
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
			int nValueTemp=product_register_value[MODBUS_PWM_OUT5];	//tstat6没有找到	349 ,599
			strTemp.Format(_T("%d%%"), nValueTemp);
		}
		m_FlexGrid.put_TextMatrix(5,VALUE_OUTFIELD,strTemp);
		//////////////////////////////////////////////////////////////////////////
		if (m_version<32.2)
		{
			//328	266	1	Low byte	W/R	"Output1 Function setting:
			//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
			short nFun;//=multi_register_value[328];
			nFun= product_register_value[MODBUS_OUTPUT1_FUNCTION];
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
			strTemp=ONTPUT_FUNS[0];//2.5.0.98
			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
		}
		else
		{
			////337	270	1	Low byte	W/R	Output5 function setting (see above)
			int indext=-1;
			indext = product_register_value[MODBUS_OUTPUT5_FUNCTION];

			if(indext>=0&&indext<=3)
				strTemp=ONTPUT_FUNS[indext];
			strTemp=ONTPUT_FUNS[0];//2.5.0.98
			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//row4,5,for 5F,5G:

	//row45/67 ANALOG
	if (m_nModeType==2||m_nModeType==12||m_nModeType==16||m_nModeType==PM_PRESSURE
		||m_nModeType==18||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7)//5ADEG
	{
		//186	207	1	Low byte	W/R	Analog Output1 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
		//102	210	2	Full	W/R(write only when manual output6 enable)	Output6 ,Analog output1, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.

		short nRange;//=multi_register_value[186];
		int nValue;//e=multi_register_value[102];
		//if (newtstat6[7] == 6)
		nRange = product_register_value[MODBUS_OUTPUT1_SCALE]; //186  207
		nValue = product_register_value[MODBUS_COOLING_VALVE]; //102  210

		if(nRange==0)
		{				
			if(nValue==0)	
				strTemp=_T("Off");
			if(nValue==1000)
				strTemp=_T("On");
		}
		else
		{
			float nvalue=0.0;
			//if (newtstat6[7] == 6)
			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
			{
				//AfxMessageBox(_T("It's impossible to enter this place!"));
				if(nRange==1)//0-10v
				{
					nvalue= product_register_value[MODBUS_COOLING_VALVE]/10.0f;//  T6=210
				}
				if(nRange==2)//0-5v
				{
					nvalue=product_register_value[MODBUS_COOLING_VALVE]/5.0f;
				}
				if(nRange==3)//2-10v
				{
					nvalue=product_register_value[MODBUS_COOLING_VALVE]/8.0f;
				}
				if(nRange==3)//10-0v
				{
					nvalue=(10-product_register_value[MODBUS_COOLING_VALVE]/100.0f)/10.0f *100;
				}
			}
			else
			{


				if(nRange==1)//0-10v
				{
					//nvalue=multi_register_value[102]/100 /10.0 * 100%;
					nvalue=product_register_value[MODBUS_COOLING_VALVE]/10.0f;//102   210
				}
				if(nRange==2)//0-5v
				{
					nvalue=product_register_value[MODBUS_COOLING_VALVE]/5.0f;
				}
				if(nRange==3)//2-10v
				{
					nvalue=product_register_value[MODBUS_COOLING_VALVE]/8.0f;
				}
				if(nRange==3)//10-0v
				{
					nvalue=(10-product_register_value[MODBUS_COOLING_VALVE]/100.0f)/10.0f *100;
				}
			}
			strTemp.Format(_T("%.1f%%"),nvalue);
		}
		if(m_nModeType==2)
		{

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


			//103	211	2	Full	W/R(write only when manual output7 enable)	Output7 Analog output2, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.    
			//187	208	1	Low byte	W/R	Analog Output2 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 

			nRange= product_register_value[MODBUS_OUTPUT2_SCALE];//187  208
			nValue= product_register_value[MODBUS_HEATING_VALVE];//103  211

			if(nRange==0)
			{

				if(nValue==0)
					strTemp=_T("Off");
				if(nValue==1)
					strTemp=_T("On");
			}
			else
			{
				//strTemp.Format(_T("%.1f"),nValue/100.0);

				float nvalue=0.0;
				//if (newtstat6[7] == 6)
				if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
				{
					//AfxMessageBox(_T("It's impossible to enter this place!222"));
					if(nRange==1)//0-10v
					{
						//nvalue=multi_register_value[102]/100 /10.0 * 100%;
						nvalue=product_register_value[MODBUS_HEATING_VALVE]/10.0f;//103 211
					}
					if(nRange==2)//0-5v
					{
						nvalue=product_register_value[MODBUS_HEATING_VALVE]/5.0f;
					}
					if(nRange==3)//2-10v
					{
						nvalue=product_register_value[MODBUS_HEATING_VALVE]/8.0f;
					}
					if(nRange==4)//10-0v
					{
						nvalue=(10-product_register_value[MODBUS_HEATING_VALVE]/100.0f)/10.0f *100;
					}
				}else
				{
					if(nRange==1)//0-10v
					{
						//nvalue=multi_register_value[102]/100 /10.0 * 100%;
						nvalue= product_register_value[MODBUS_HEATING_VALVE]/10.0f;//103   211
					}
					if(nRange==2)//0-5v
					{
						nvalue= product_register_value[MODBUS_HEATING_VALVE]/5.0f;
					}
					if(nRange==3)//2-10v
					{
						nvalue= product_register_value[MODBUS_HEATING_VALVE]/8.0f;
					}
					if(nRange==4)//10-0v
					{
						nvalue=(10- product_register_value[MODBUS_HEATING_VALVE]/100.0f)/10.0f *100;
					}
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
		}//end ==2
		else//out6/7
		{

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


			//103	211	2	Full	W/R(write only when manual output7 enable)	Output7 Analog output2, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.    
			//187	208	1	Low byte	W/R	Analog Output2 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
			//if (newtstat6[7] == 6)



			nRange= product_register_value[MODBUS_OUTPUT2_SCALE];//187  208
			nValue= product_register_value[MODBUS_HEATING_VALVE];//103  211


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
				float nvalue=0.0;
				//if (newtstat6[7] == 6)
				if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
				{
					if(nRange==1)//0-10v
					{
						//nvalue=multi_register_value[102]/100 /10.0 * 100%;
						nvalue=product_register_value[MODBUS_HEATING_VALVE]/10.0f;//103 211
					}
					if(nRange==2)//0-5v
					{
						nvalue=product_register_value[MODBUS_HEATING_VALVE]/5.0f;
					}
					if(nRange==3)//2-10v
					{
						nvalue=product_register_value[MODBUS_HEATING_VALVE]/8.0f;
					}
					if(nRange==4)//10-0v
					{
						nvalue=(10-product_register_value[MODBUS_HEATING_VALVE]/100.0f)/10.0f *100;
					}
				}else
				{
					if(nRange==1)//0-10v
					{
						//nvalue=multi_register_value[102]/100 /10.0 * 100%;
						nvalue=product_register_value[MODBUS_HEATING_VALVE]/10.0f;
					}
					if(nRange==2)//0-5v
					{
						nvalue=product_register_value[MODBUS_HEATING_VALVE]/5.0f;
					}
					if(nRange==3)//2-10v
					{
						nvalue=product_register_value[MODBUS_HEATING_VALVE]/8.0f;
					}
					if(nRange==4)//10-0v
					{
						nvalue=(10-product_register_value[MODBUS_HEATING_VALVE]/100.0f)/10.0f *100;
					}
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
	//286	245	1	Low byte	W/R	Interlock for  output1

	CString strlock;
	int stradd;// = 286;
	//if (newtstat6[7] == 6)
	if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
	{
		stradd =MODBUS_INTERLOCK_OUTPUT1; //245;
		for (int i = 0;i<7;i++)
		{

			int itemp = product_register_value[stradd+i];
			if(itemp>=0&&itemp<6)
				m_FlexGrid.put_TextMatrix(i+1,6,Interlock[itemp]);

		}
	}else
	{
		stradd =MODBUS_INTERLOCK_OUTPUT1;// 286;
		for (int i = 0;i<7;i++)
		{

			int itemp = product_register_value[stradd+i];
			if(itemp>=0&&itemp<6)
				m_FlexGrid.put_TextMatrix(i+1,6,Interlock[itemp]);

		}


	}

}

//Fance 2013 03 28
//由于代码 逻辑混乱，已经重写。
//void COutputSetDlg::Fresh_Grid()
//{
//	int nValueTemp123=5;
//	//m_version=get_curtstat_version();
//	//m_nModeType=multi_register_value[7];
//	//if (newtstat6[7] == PM_TSTAT6)
//	if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//	{
//		m_nModeType = newtstat6[7];
//	}else
//	{
//		m_nModeType = multi_register_value[7];
//	}
//	switch (m_nModeType)
//	{
//		case 2:
//		case 1:
//			{
//				m_outRows=6;
//			}
//			break;
//		case 4:
//			{
//				m_outRows=6;
//
//			 }
//			break;
//		case PM_PRESSURE:
//		case PM_TSTAT5D:
//			{
//				m_outRows=8;
//
//			 }break;
//		case PM_TSTAT6:
//		case PM_TSTAT7:
//		case 16:
//			{
//				m_outRows=8;
//			}break;
//		case 17:
//			{
//				m_outRows=6;
//			}
//				break;
//		case 18:
//			{
//				m_outRows=8;
//
//			}
//			break;
//		case 19:
//			{
//				m_outRows=8;
//			}
//			break;
//		default:break;
//	}
//	CString strTemp;
//	//row1:
//	int nVAlue;
//
//
////	108	209	1	Low byte	W/R	Output1 tot 5, bit 0 thru 4 = relay 1 thru 5.
//
//	//tstat6
//// 	if (multi_register_value[7] == 6)
//// 	{
//// 	
//// 	}else
//	//if (newtstat6[7] == PM_TSTAT6)
//
//	//annul by Fan
//	/*
//	if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//	{
//		nVAlue = newtstat6[209];
//	}else
//	{
//		nVAlue=multi_register_value[108];
//	}
//	*/
//	//108  209 Output1 tot 5, bit 0 thru 4 = relay 1 thru 5.  Fan.
//	nVAlue = product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS]; //t5=108   t6=209;
//
//	//tstat6
//
//
//
//
//	int nRange=0;
//
//	if(nVAlue&1)
//	{
//		strTemp=_T("On");
//	}
//	else
//	{
//		strTemp=_T("Off");
//	}
//	m_FlexGrid.put_TextMatrix(1,VALUE_OUTFIELD,strTemp);
//
//
//	//310	254	1	Low byte	W/R	"Output auto/manual enable. Bit 0 to 4 correspond to output1 to output5, bit 5 correspond to 
////	output6, bit 6 correspond to output7. 0, auto mode; 1, manual mode."
//
//	int nAMVAlue=0;//=multi_register_value[310];
//	nAMVAlue = product_register_value[MODBUS_OUTPUT_MANU_ENABLE];
//
//	//annul by Fan  2013-03-28
//	/*
//	if ((newtstat6[7]== PM_TSTAT6)||(newtstat6[7]== PM_TSTAT7))
//		nAMVAlue = newtstat6[254];
//	else
//		nAMVAlue =multi_register_value[310];
//	*/
//
//		//0bit
//	if((int)(nAMVAlue & 0x01)==1)
//	{
//		strTemp=_T("Manual");
//		m_FlexGrid.put_Col(VALUE_OUTFIELD);
//		m_FlexGrid.put_Row(1);
//		m_FlexGrid.put_CellBackColor(RGB(255,255,255));
//	}
//	else
//	{
//		strTemp=_T("Auto");
//		m_FlexGrid.put_Col(VALUE_OUTFIELD);
//		m_FlexGrid.put_Row(1);
//		m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
//	} 
//	m_FlexGrid.put_TextMatrix(1,AM_OUTFIELD,strTemp);
//	strTemp=_T("On/Off");
//	m_FlexGrid.put_TextMatrix(1,RANG_OUTFIELD,strTemp);
//	if(m_version>32.2)
//	{
//	//	328	266	1	Low byte	W/R	"Output1 Function setting:
//		//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
//		int nFun=0;//=multi_register_value[328];//tstat6找不到对应的
//		nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; //328   266
//		//annul by Fan
//		/*
//		if ((newtstat6[7] ==PM_TSTAT6)||(newtstat6[7] ==PM_TSTAT7))
//		{
//			nFun = newtstat6[266];
//		}
//		else
//		{
//				nFun = multi_register_value[328];		
//		}
//		*/
//		if(nFun>=0&&nFun<4)
//		{
//			strTemp=ONTPUT_FUNS[nFun];
//		}
//		else
//		{
//// 			CString str;
//// 			str.Format(_T("multi_register_value[328]=%d"),multi_register_value[328]);
//// 			AfxMessageBox(str);//tstat6
//		}
//	}
//	else
//	{
//		strTemp=NO_APPLICATION;
//	}
//
//	strTemp=ONTPUT_FUNS[0];//2.5.0.98
//	m_FlexGrid.put_TextMatrix(1,FUN_OUTFIELD,strTemp);
//	//row 2:
//	if(nVAlue&2)
//	{
//		strTemp=_T("On");
//	}
//	else
//	{
//		strTemp=_T("Off");
//	}
//	m_FlexGrid.put_TextMatrix(2,VALUE_OUTFIELD,strTemp);
//
//	if((int)(nAMVAlue & 2))
//	{
//		strTemp=_T("Manual");
//		m_FlexGrid.put_Col(VALUE_OUTFIELD);
//		m_FlexGrid.put_Row(2);
//		m_FlexGrid.put_CellBackColor(COLOR_CELL);
//	}
//	else
//	{
//		strTemp=_T("Auto");
//		m_FlexGrid.put_Col(VALUE_OUTFIELD);
//		m_FlexGrid.put_Row(2);
//		m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
//	}
//	m_FlexGrid.put_TextMatrix(2,AM_OUTFIELD,strTemp);
//
//	strTemp=_T("On/Off");
//	m_FlexGrid.put_TextMatrix(2,RANG_OUTFIELD,strTemp);
//	strTemp.Empty();
//	if (m_version<32.2)
//	{
//
//		short nFun=0;//multi_register_value[328];
//		//328	266	1	int8	1	Low byte	W/R	"Output1 Function setting:
//		//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
//		nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION];
//		//annul by Fan
//		/*
//		if ((newtstat6[7] ==PM_TSTAT6)||(newtstat6[7] ==PM_TSTAT7))
//			nFun = newtstat6[266];
//		else
//			nFun = multi_register_value[328];
//		*/
//		int nMask;
//		nMask=nFun&0x1;
//		if (nMask>0)
//		{
//			strTemp=ONTPUT_FUNS[1];
//		}
//		else
//		{
//			strTemp=ONTPUT_FUNS[0];
//		}
//		strTemp=ONTPUT_FUNS[0];//2.5.0.98
//		m_FlexGrid.put_TextMatrix(2,FUN_OUTFIELD,strTemp);
//	}
//	else
//	{
//		int indext=-1;
//		//334	267	1	Low byte	W/R	Output2 function setting (see above)
//		indext = product_register_value[MODBUS_OUTPUT2_FUNCTION];
//		//Annul by Fan
//		/*
//		if ((newtstat6[7] ==PM_TSTAT6)||(newtstat6[7] ==PM_TSTAT7))
//		{
//			indext = newtstat6[267];
//		}
//		else
//		{
//				indext = multi_register_value[334];
//		}
//		*/
//			
//
//
//		//indext=multi_register_value[334];
//// 		if(indext>=0&&indext<=3)
//// 			strTemp=ONTPUT_FUNS[indext];
//		strTemp=ONTPUT_FUNS[0];//2.5.0.98
//		m_FlexGrid.put_TextMatrix(2,FUN_OUTFIELD,strTemp);
//	}
//// row 3:
//	if(nVAlue&4)
//	{
//		strTemp=_T("On");
//	}
//	else
//	{
//		strTemp=_T("Off");
//	}
//	m_FlexGrid.put_TextMatrix(3,VALUE_OUTFIELD,strTemp);
//	if((int)(nAMVAlue & 4))
//	{
//		strTemp=_T("Manual");
//		m_FlexGrid.put_Col(VALUE_OUTFIELD);
//		m_FlexGrid.put_Row(3);
//		m_FlexGrid.put_CellBackColor(RGB(255,255,255));
//	}
//	else
//	{
//		strTemp=_T("Auto");
//		m_FlexGrid.put_Col(VALUE_OUTFIELD);
//		m_FlexGrid.put_Row(3);
//		m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
//	}
//	m_FlexGrid.put_TextMatrix(3,AM_OUTFIELD,strTemp);
//	
//	strTemp=_T("On/Off");
//	m_FlexGrid.put_TextMatrix(3,RANG_OUTFIELD,strTemp);
//	strTemp.Empty();
//	if (m_version<32.2)
//	{
//		short nFun=0;//multi_register_value[328];
//		nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; //328  266
//		//Annul by Fan
//		/*
//		if (newtstat6[7] ==PM_TSTAT6)
//			nFun = newtstat6[266];
//		else
//			nFun = multi_register_value[328];
//		*/
//		int nMask;
//		nMask=nFun&0x2;
//		if (nMask>0)
//		{
//			strTemp=ONTPUT_FUNS[1];
//		}
//		else
//		{
//			strTemp=ONTPUT_FUNS[0];
//		}
//		strTemp=ONTPUT_FUNS[0];//2.5.0.98
//		m_FlexGrid.put_TextMatrix(3,FUN_OUTFIELD,strTemp);
//	}
//	else
//	{
//		int indext=-1;
//		//335	268	1	Low byte	W/R	Output3 function setting (see above)
//		indext = product_register_value[MODBUS_OUTPUT3_FUNCTION];
//		//Annul by Fan
//		/*
//		if ((newtstat6[7]==PM_TSTAT6)||(newtstat6[7]==PM_TSTAT7))
//		{
//			indext=newtstat6[268];
//		}else
//		{
//			indext=multi_register_value[335];		
//		}
//		*/
//		if(indext>=0&&indext<=3)
//			strTemp=ONTPUT_FUNS[indext];
//		strTemp=ONTPUT_FUNS[0];//2.5.0.98
//		m_FlexGrid.put_TextMatrix(3,FUN_OUTFIELD,strTemp);
//	}
//
//	//BCDE 4,5 D/O
///*  commented by zgq; 2010-12-7; 新的需求，5E需要当选择float和PMW时输入值，并存在348， 349
//	if(m_nModeType==1||m_nModeType==4||m_nModeType==12||m_nModeType==16)//||m_nModeType==17||m_nModeType==18)
//	{
//		if(nVAlue&8)
//		{
//			strTemp=_T("On");
//		}
//		else
//		{
//			strTemp=_T("Off");
//		}
//		m_FlexGrid.put_TextMatrix(4,VALUE_OUTFIELD,strTemp);
//		if((int)(nAMVAlue & 8))
//		{
//			strTemp=_T("Manual");
//			m_FlexGrid.put_Col(VALUE_OUTFIELD);
//			m_FlexGrid.put_Row(4);
//			m_FlexGrid.put_CellBackColor(RGB(255,255,255));
//		}
//		else
//		{
//			strTemp=_T("Auto");
//			m_FlexGrid.put_Col(VALUE_OUTFIELD);
//			m_FlexGrid.put_Row(4);
//			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
//		}
//		m_FlexGrid.put_TextMatrix(4,AM_OUTFIELD,strTemp);
//		strTemp.Empty();		
//	//	strTemp=_T("On/Off");
//	//	m_FlexGrid.put_TextMatrix(4,RANG_OUTFIELD,strTemp);
//		nRange=multi_register_value[283];
//		if(nRange>=0&&nRange<3)
//		{
//			strTemp=OUTPUT_RANGE45[nRange];
//		}
//		m_FlexGrid.put_TextMatrix(4,RANG_OUTFIELD,strTemp);
//
//		strTemp.Empty();
//		if (m_version<32.2)
//		{
//			short nFun=multi_register_value[328];
//			int nMask;
//			nMask=nFun&0x4;
//			if (nMask>0)
//			{
//				strTemp=ONTPUT_FUNS[1];
//			}
//			else
//			{
//				strTemp=ONTPUT_FUNS[0];
//			}
//			m_FlexGrid.put_TextMatrix(4,FUN_OUTFIELD,strTemp);
//		}
//		else
//		{
//			int indext=-1;
//			indext=multi_register_value[336];
//			if(indext>=0&&indext<=3)
//				strTemp=ONTPUT_FUNS[indext];
//			m_FlexGrid.put_TextMatrix(4,FUN_OUTFIELD,strTemp);
//		}
//
//		if(nVAlue&16)
//		{
//			strTemp=_T("On");
//		}
//		else
//		{
//			strTemp=_T("Off");
//		}
//		m_FlexGrid.put_TextMatrix(5,VALUE_OUTFIELD,strTemp);
//
//		if((int)(nAMVAlue & 16))
//		{
//			strTemp=_T("Manual");
//			m_FlexGrid.put_Col(VALUE_OUTFIELD);
//			m_FlexGrid.put_Row(5);
//			m_FlexGrid.put_CellBackColor(RGB(255,255,255));
//		}
//		else
//		{
//			strTemp=_T("Auto");
//			m_FlexGrid.put_Col(VALUE_OUTFIELD);
//			m_FlexGrid.put_Row(5);
//			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
//		}
//		m_FlexGrid.put_TextMatrix(5,AM_OUTFIELD,strTemp);
//		strTemp.Empty();
//		
//	//	strTemp=_T("On/Off");
//	//	m_FlexGrid.put_TextMatrix(5,RANG_OUTFIELD,strTemp);
//		nRange=multi_register_value[284];
//		if(nRange>=0&&nRange<3)
//		{
//			strTemp=OUTPUT_RANGE45[nRange];
//		}
//		m_FlexGrid.put_TextMatrix(5,RANG_OUTFIELD,strTemp);
//
//		if (m_version<32.2)
//		{
//			short nFun=multi_register_value[328];
//			int nMask;
//			nMask=nFun&0x8;
//			if (nMask>0)
//			{
//				strTemp=ONTPUT_FUNS[1];
//			}
//			else
//			{
//				strTemp=ONTPUT_FUNS[0];
//			}
//			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
//		}
//		else
//		{
//			int indext=-1;
//			indext=multi_register_value[337];
//			if(indext>=0&&indext<=3)
//				strTemp=ONTPUT_FUNS[indext];
//			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
//		}
//	}
//*/
//
//	//BCDE 4,5 D/O
//	if(m_nModeType==1||m_nModeType==4||m_nModeType==12||m_nModeType==16 
//		||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7||m_nModeType==PM_PRESSURE)//||m_nModeType==17||m_nModeType==18)
//	{
//		// just for row4 ///////////////////////////////////////////////////////////////
//
//		//	strTemp=_T("On/Off");
//		//	m_FlexGrid.put_TextMatrix(5,RANG_OUTFIELD,strTemp);
//		if((int)(nAMVAlue & 8))
//		{
//			strTemp=_T("Manual");
//			m_FlexGrid.put_Col(VALUE_OUTFIELD);
//			m_FlexGrid.put_Row(4);
//			//m_FlexGrid.put_CellBackColor(RGB(255,255,255));//COLOR_CELL RGB
//			m_FlexGrid.put_CellBackColor(COLOR_CELL);
//		}
//		else
//		{
//			strTemp=_T("Auto");
//			m_FlexGrid.put_Col(VALUE_OUTFIELD);
//			m_FlexGrid.put_Row(4);
//			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
//		}
//		m_FlexGrid.put_TextMatrix(4,AM_OUTFIELD,strTemp);
//		strTemp.Empty();
//
//		//////////////////////////////////////////////////////////////////////////
//		//nRange=multi_register_value[283];
//		//283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
//
//		nRange = product_register_value[MODBUS_MODE_OUTPUT4];//283  205
//		//Annul by Fan
//		/*
//		if ((newtstat6[7] ==PM_TSTAT6)||(newtstat6[7] ==PM_TSTAT6))
//			nRange = newtstat6[205];
//		else
//			nRange = multi_register_value[283];
//		*/
//		
//		if(nRange>=0&&nRange<2)
//		{
//			strTemp=OUTPUT_RANGE45[nRange];
//		}
//		m_FlexGrid.put_TextMatrix(4,RANG_OUTFIELD,strTemp);
//
//		if(nRange == 0 || !(nAMVAlue & 8)) // AM栏选择了Auto或者Range 栏选择了On/Off，value都显示ON/Off
//		{  // output is on/off
//			if(nVAlue&8)
//			{
//				strTemp=_T("On");
//			}
//			else
//			{
//				strTemp=_T("Off");
//			}
//		}
//		else // output is value
//		{
//			//comments by Fance ,此前没有 348 -》对应 t6的598  ，现在有了。;所以该不该改为现在的？？？
//			int nValueTemp = product_register_value[MODBUS_PWM_OUT4]; //348 //598
//			//int nValueTemp = multi_register_value[348];		//tstat6没找到对应的 //Fance_?_
//	//This Line Annul by Fance		int nValueTemp = newtstat6[244];		//tstat6没找到对应的//0914
//
//// 			if (newtstat6[7] ==PM_TSTAT6)
//// 				nValueTemp = newtstat6[205];
//// 			else
//// 				nValueTemp = multi_register_value[348];
//
//
//			strTemp.Format(_T("%d%%"), nValueTemp);
//		}
//		m_FlexGrid.put_TextMatrix(4,VALUE_OUTFIELD,strTemp);
//
//		//////////////////////////////////////////////////////////////////////////
//
//		if (m_version<32.2)
//		{
//			short nFun=0;//multi_register_value[328];
//			nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; //328  266
//			//Annul by Fan
//			/*
//			if ((newtstat6[7] ==PM_TSTAT6)||(newtstat6[7] ==PM_TSTAT7))
//				nFun = newtstat6[266];
//			else
//				nFun = multi_register_value[328];
//			*/
//
//			int nMask;
//			nMask=nFun&0x4;
//			if (nMask>0)
//			{
//				strTemp=ONTPUT_FUNS[1];//Fance_?_  为什么 所有的值都是 Normal;
//			}
//			else
//			{
//				strTemp=ONTPUT_FUNS[0];
//			}
//			strTemp=ONTPUT_FUNS[0];//2.5.0.98
//			m_FlexGrid.put_TextMatrix(4,FUN_OUTFIELD,strTemp);
//		}
//		else
//		{
//			/////336	407	1	Low byte	R	Show the size of E2 chip. 0 = 24c02, 1 = 24c08/24c16. xxxx
//			//336	269	1	Low byte	W/R	Output4 function setting (see above)vvvv
//
//			int indext=-1;
//			//indext=multi_register_value[336];
//			//if (newtstat6[7] ==PM_TSTAT6)
//			indext = product_register_value[MODBUS_OUTPUT4_FUNCTION];
//			//Annul by Fan
//			/*
//			if ((newtstat6[7] ==PM_TSTAT6)||(newtstat6[7] ==PM_TSTAT7))
//			{
//				indext = newtstat6[269];
//			}
//			else
//			{
//					indext = multi_register_value[336];
//			}
//			*/
//
//
//			if(indext>=0&&indext<=3)
//				strTemp=ONTPUT_FUNS[indext];
//			strTemp=ONTPUT_FUNS[0];//2.5.0.98
//			m_FlexGrid.put_TextMatrix(4,FUN_OUTFIELD,strTemp);
//		}
//
//		///////////////////////////////////////////////////////////////////////////
//		// just for row5
//	
//
//		if((int)(nAMVAlue & 16))
//		{
//			strTemp=_T("Manual");
//			m_FlexGrid.put_Col(VALUE_OUTFIELD);
//			m_FlexGrid.put_Row(5);
//			m_FlexGrid.put_CellBackColor(RGB(255,255,255));
//			
//		}
//		else
//		{
//			strTemp=_T("Auto");
//			m_FlexGrid.put_Col(VALUE_OUTFIELD);
//			m_FlexGrid.put_Row(5);
//			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
//		}
//		m_FlexGrid.put_TextMatrix(5,AM_OUTFIELD,strTemp);
//		strTemp.Empty();
//
//		//nRange=multi_register_value[284];
//		//284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM
//		nRange = product_register_value[MODBUS_MODE_OUTPUT5];
//		//Annul by Fan
//		/*
//		if ((newtstat6[7] ==PM_TSTAT6)||(newtstat6[7] ==PM_TSTAT7))
//			nRange = newtstat6[206];
//		else
//			nRange = multi_register_value[284];
//		*/
//
//		if(nRange>=0&&nRange<2)
//		{
//			strTemp=OUTPUT_RANGE45[nRange];
//		}
//		m_FlexGrid.put_TextMatrix(5,RANG_OUTFIELD,strTemp);
//
//		//////////////////////////////////////////////////////////////////////////
//		if(nRange == 0 || !(nAMVAlue & 16))
//		{			
//			if(nVAlue&16)
//			{
//				strTemp=_T("On");
//			}
//			else
//			{
//				strTemp=_T("Off");
//			}
//		}
//		else
//		{
//			
//			nValueTemp123=product_register_value[MODBUS_PWM_OUT5];	//tstat6没有找到	349 ,599
//			strTemp.Format(_T("%d%%"), nValueTemp123);
//		}
//		m_FlexGrid.put_TextMatrix(5,VALUE_OUTFIELD,strTemp);
//
//		//////////////////////////////////////////////////////////////////////////
//
//		if (m_version<32.2)
//		{
//			//328	266	1	Low byte	W/R	"Output1 Function setting:
//		//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
//
//			short nFun;//=multi_register_value[328];
//			nFun= product_register_value[MODBUS_OUTPUT1_FUNCTION];
//			//Annul by Fan
//			/*
//			if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//			{
//				nFun=newtstat6[266];
//			}else
//			{
//				 nFun=multi_register_value[328];
//			}
//			*/
//			int nMask;
//			nMask=nFun&0x8;
//			if (nMask>0)
//			{
//				strTemp=ONTPUT_FUNS[1];
//			}
//			else
//			{
//				strTemp=ONTPUT_FUNS[0];
//			}
//			strTemp=ONTPUT_FUNS[0];//2.5.0.98
//			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
//		}
//		else
//		{
//			////337	270	1	Low byte	W/R	Output5 function setting (see above)
//			int indext=-1;
//			indext = product_register_value[MODBUS_OUTPUT5_FUNCTION];
//			/*
//			if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
//			{
//				indext=newtstat6[270];
//			}else
//			{
//					indext=multi_register_value[337];
//			}
//			*/
//			if(indext>=0&&indext<=3)
//				strTemp=ONTPUT_FUNS[indext];
//			strTemp=ONTPUT_FUNS[0];//2.5.0.98
//			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
//		}
//	}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//	//row4,5,for 5F,5G:
//	//Annul by Fan if(m_nModeType==17||m_nModeType==18) 下面又有对 18进行这个处理所以要改为下面的
//	if(m_nModeType==17)
//	{
//		//out4:
//		int nRang=0;//multi_register_value[283];
//
//		nRang= product_register_value[MODBUS_OUTPUT1_FUNCTION];//328  266
//
//		//if (newtstat6[7] ==PM_TSTAT6)
//		//Annul by Fan
//		/*
//		if ((newtstat6[7] ==PM_TSTAT6)||(newtstat6[7] ==PM_TSTAT7))
//		{
//			nRang = newtstat6[266];
//		}
//		else
//		{
//			nRang = multi_register_value[328];
//		}
//		*/
//// 		if (multi_register_value[7] == 18)//2.5.0.94
//// 		{
//// 			nRang = multi_register_value[310];
//// 		}
//		
//
//		//int nValue;
//		int nValue1;//=multi_register_value[108];
//
//		//---------------------Modify by Fan--------------------------------------------
//		nValue1 = product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS];//108   209
//		if(nRang==0)
//		{
//			if(nValue1 &( 1<<4))
//				strTemp=_T("ON");
//			else
//				strTemp=_T("OFF");
//		}
//		else
//		{
//			strTemp.Format(_T("%d%%"),product_register_value[MODBUS_PWM_OUT4]);//348  598
//		}
//		m_FlexGrid.put_TextMatrix(4,VALUE_OUTFIELD,strTemp);
//		//------------------------------------------------------------------------------
//		//Annul by Fan
//		/*
//		//tstat6
//		//if (multi_register_value[7] == 6)
//		if ((newtstat6[7] ==PM_TSTAT6)||(newtstat6[7] ==PM_TSTAT7))
//		{
//			int num = reg_tststold[108];
//			nValue1=multi_register_value[num];
//		}else
//			nValue1=multi_register_value[108];
//		
//		//tstat6
//		if ((newtstat6[7] ==PM_TSTAT6)||(newtstat6[7] ==PM_TSTAT7))
//		{	
//			if(nRang==0)
//			{
//				if(nValue1 &( 1<<4))
//					strTemp=_T("ON");
//				else
//					strTemp=_T("OFF");
//			}
//			else
//			{
//				strTemp.Format(_T("%d%%"),multi_register_value[348]);
//			}
//			m_FlexGrid.put_TextMatrix(4,VALUE_OUTFIELD,strTemp);
//		}else
//		{
//			if(nRang==0)
//			{
//				if(nValue1 &( 1<<4))
//					strTemp=_T("ON");
//				else
//					strTemp=_T("OFF");
//			}
//			else
//			{
//				strTemp.Format(_T("%d%%"),multi_register_value[348]);
//			}
//			m_FlexGrid.put_TextMatrix(4,VALUE_OUTFIELD,strTemp);
//
//		}
//		*/
//
////2.5.0.94
//		if(m_nModeType == 18)
//		{
//			if(product_register_value[MODBUS_MODE_OUTPUT4])  //283   205
//			{
//				if(nValue1 &( 1<<3))
//					strTemp=_T("ON");
//				else
//					strTemp=_T("OFF");
//			}
//			else
//			{
//				strTemp.Format(_T("%d%%"),product_register_value[MODBUS_PWM_OUT4]);//348  598
//			}
//		}
//		//Annul by Fan
//		/*
//		if (multi_register_value[7]==18)
//		{
//			//int autmanl = multi_register_value[310];
//			if(multi_register_value[283]==0)
//			{
//				//int xx = 1;
//				//xx = xx<<3;
//				//int flag =autmanl&xx;
//				//if(flag)
//				{
//					if(nValue1 &( 1<<3))
//						strTemp=_T("ON");
//					else
//						strTemp=_T("OFF");
//				}
//			}
//			else
//			{
//				strTemp.Format(_T("%d%%"),multi_register_value[348]);
//			}
//
//			m_FlexGrid.put_TextMatrix(4,VALUE_OUTFIELD,strTemp);
//			//2.5.0.94		
//		}
//		*/
////AM
//		if((int)(nAMVAlue & 8))
//		{
//			strTemp=_T("Manual");
//			m_FlexGrid.put_Col(VALUE_OUTFIELD);
//			m_FlexGrid.put_Row(4);
//			m_FlexGrid.put_CellBackColor(RGB(255,255,255));
//		}
//		else
//		{
//			strTemp=_T("Auto");
//			m_FlexGrid.put_Col(VALUE_OUTFIELD);
//			m_FlexGrid.put_Row(4);
//			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
//		}
//		m_FlexGrid.put_TextMatrix(4,AM_OUTFIELD,strTemp);
//		strTemp.Empty();
//		//283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
//
//		nRange = product_register_value[MODBUS_MODE_OUTPUT4]; //283  205
//		//if (newtstat6[7] == 6)
//		/*//By Fan
//		if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
//		{
//			nRange=multi_register_value[205];
//
//		}else
//		{
//			nRange=multi_register_value[283];
//		}
//		*/
//		if(nRange>=0&&nRange<=2)
//		{
//			strTemp=OUTPUT_RANGE45[nRange];
//		}
//
//#if 0
//		if (multi_register_value[336] == 3)//2.5.0.96
//		{
//			strTemp=OUTPUT_RANGE45[1];
//		}else
//		{
//			strTemp=OUTPUT_RANGE45[0];
//		}
//#endif
//		m_FlexGrid.put_TextMatrix(4,RANG_OUTFIELD,strTemp);
//		
//		strTemp.Empty();
//		if (m_version<32.2)
//		{
//			//328	266	1	Low byte	W/R	"Output1 Function setting:
//		//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
//
//			short nFun=0;//multi_register_value[328];
//			nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION];
//			/*//By Fan
//			//if (newtstat6[7] == PM_TSTAT6)
//			if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//				nFun = newtstat6[266];
//			else
//				nFun = multi_register_value[328];
//			*/
//			int nMask;
//			nMask=nFun&0x4;
//			if (nMask>0)
//			{
//				strTemp=ONTPUT_FUNS[1];
//			}
//			else
//			{
//				strTemp=ONTPUT_FUNS[0];
//			}
//			m_FlexGrid.put_TextMatrix(4,FUN_OUTFIELD,strTemp);
//		}
//		else
//		{
//			int indext=-1;
//
//			//336	269	1	Low byte	W/R	Output4 function setting (see above)
//			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//			{
//				indext=product_register_value[269];
//			}else
//			{
//
//				//indext=multi_register_value[336];//2.5.0.96
//				indext=product_register_value[283];//2.5.0.98		//Fance_?_   //269 对应 336 这里为什么 要变为 283;
//
//			}
//			//if (newtstat6[7] == 6)
//			/*
//			if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
//			{
//				indext=multi_register_value[269];
//			}else
//			{
//
//				//indext=multi_register_value[336];//2.5.0.96
//				indext=multi_register_value[283];//2.5.0.98		
//
//			}
//			*/
//			if(indext>=0&&indext<=3)
//				strTemp=ONTPUT_FUNS[indext];
//			if (indext >=2)//2.5.0.98
//			{
//				strTemp=ONTPUT_FUNS[0];
//			}
//
//			m_FlexGrid.put_TextMatrix(4,FUN_OUTFIELD,strTemp);
//		}
//
//		//out5:
//		//284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM
//
//////337	270	1	Low byte	W/R	Output5 function setting (see above)
//
//
//		if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//		{
//			nRang=product_register_value[206];
//		}else
//		{
//			nRang=product_register_value[337];			//Fance_?_ 不明白 为什么 tstat5 要用337=MODBUS_OUTPUT5_FUNCTION 而tstat 要用206=MODBUS_MODE_OUTPUT5;
//		}
///*//by Fan
//		//if (newtstat6[7] == 6)
//		if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
//		{
//			nRang=multi_register_value[206];
//		}else
//		{
//			nRang=multi_register_value[337];		
//		}
//*/
//		
//		//nValue1=multi_register_value[108];
//		//108	209	1	Low byte	W/R	Output1 tot 5, bit 0 thru 4 = relay 1 thru 5.
//
//		nValue1 = product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS];//108   209
//		/*//Fan
//		//tstat6
//		if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
//		{
//			nValue1 = newtstat6[209];
//		}else
//		{
//			nValue1=multi_register_value[108];
//		}
//		*/
//		//tstat6
//
//		if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//		{
//			if(nRang==0)
//			{
//				if(nValue1 &( 1<<3))
//					//if(nValue1 &( 1<<4))//2.5.0.94
//					strTemp=_T("ON");
//				else
//					strTemp=_T("OFF");
//
//			}
//			else
//			{	
//				strTemp.Format(_T("%d%%"),product_register_value[MODBUS_PWM_OUT5]);//找不到 现在找到了 349  599;
//			}
//			m_FlexGrid.put_TextMatrix(5,VALUE_OUTFIELD,strTemp);
//
//		}else if (product_register_value[7]==18)
//		{
//			//2.5.0.94
//			if(product_register_value[MODBUS_MODE_OUTPUT5]==0)// 284   206
//			{
//			//int autmanl = multi_register_value[310];
//			//int xx_ = 1;
//			//xx_ = xx_<<4;
//			//int flag_ =autmanl&xx_; 
//			//if(flag_)
//			{
//				if(nValue1 &( 1<<4))//2.5.0.94
//					strTemp=_T("ON");
//				else
//					strTemp=_T("OFF");
//			}
//			}else
//			{
//				strTemp.Format(_T("%d%%"),product_register_value[MODBUS_PWM_OUT5]);//349  599 //Fance_?
//			}
//			m_FlexGrid.put_TextMatrix(5,VALUE_OUTFIELD,strTemp);
//
//			//2.5.0.94
//		}else
//		{
//			nRang = product_register_value[MODBUS_MODE_OUTPUT5];// 284   206
//		//	nRang=multi_register_value[284];
//			if(nRang==0)
//			{
//				if(nValue1 &( 1<<3))
//					//if(nValue1 &( 1<<4))//2.5.0.94
//					strTemp=_T("ON");
//				else
//					strTemp=_T("OFF");
//
//			}
//			else
//			{
//				strTemp.Format(_T("%d%%"),product_register_value[MODBUS_PWM_OUT5]);//找不到  349 599
//
//			}
//			m_FlexGrid.put_TextMatrix(5,VALUE_OUTFIELD,strTemp);
//
//		}
//
//
//
//		//AM
//		if((int)(nAMVAlue & 16))
//		{
//			strTemp=_T("Manual");
//			m_FlexGrid.put_Col(VALUE_OUTFIELD);
//			m_FlexGrid.put_Row(5);
//			m_FlexGrid.put_CellBackColor(RGB(255,255,255));
//		}
//		else
//		{
//			strTemp=_T("Auto");
//			m_FlexGrid.put_Col(VALUE_OUTFIELD);
//			m_FlexGrid.put_Row(5);
//			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
//		}
//		m_FlexGrid.put_TextMatrix(5,AM_OUTFIELD,strTemp);
//		strTemp.Empty();
//
//		//284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM
//
//
//		
//		nRange = product_register_value[MODBUS_MODE_OUTPUT5];  //284  206
//		//if (newtstat6[7] == 6)
//		/*
//		if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
//		{
//			nRange=newtstat6[206];
//		}else
//		{
//			nRange=multi_register_value[284];
//
//		}
//		*/
//		if(nRange>=0&&nRange<=2)
//		{
//			strTemp=OUTPUT_RANGE45[nRange];
//		}
////2.5.0.98
//// 		if (multi_register_value[337]== 3)
//// 		{
//// 			strTemp=OUTPUT_RANGE45[1];
//// 		}else
//// 		{
//// 			strTemp=OUTPUT_RANGE45[0];
//// 		}
//		m_FlexGrid.put_TextMatrix(5,RANG_OUTFIELD,strTemp);
//		
//		strTemp.Empty();
//		if (m_version<32.2)
//		{
//			//328	266	1	Low byte	W/R	"Output1 Function setting:
//		//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
//
//			short nFun;//=multi_register_value[328];
//			nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; // 328  266;
//			//if (newtstat6[7] == 6)
//			/*
//			if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
//			{
//				nFun=newtstat6[266];
//			}else
//			{
//				 nFun=multi_register_value[328];
//			}
//			*/
//			int nMask;
//			nMask=nFun&0x8;
//			if (nMask>0)
//			{
//				strTemp=ONTPUT_FUNS[1];
//			}
//			else
//			{
//				strTemp=ONTPUT_FUNS[0];
//			}
//			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
//		}
//		else
//		{
//			//337	270	1	Low byte	W/R	Output5 function setting (see above)
//
//			int indext=-1;
//			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//			{
//				indext=product_register_value[270];
//			}else
//			{
//				//indext=multi_register_value[337];//2.5.0.96
//				indext=product_register_value[284];//2.5.0.98			//Fance_?_  为什么要不对应;
//
//			}
//
//			//if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
//			//{
//			//	indext=newtstat6[270];
//			//}else
//			//{
//			//	//indext=multi_register_value[337];//2.5.0.96
//			//	indext=multi_register_value[284];//2.5.0.98
//
//			//}
//			if(indext>=0&&indext<=3)
//				strTemp=ONTPUT_FUNS[indext];
//			if (indext >=2)//2.5.0.98
//			{
//				strTemp=ONTPUT_FUNS[0];
//			}
//			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
//		}
//	}
////row45/67 ANALOG
//	if (m_nModeType==2||m_nModeType==12||m_nModeType==16||m_nModeType==PM_PRESSURE
//		||m_nModeType==18||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7)//5ADEG
//	{
//				//186	207	1	Low byte	W/R	Analog Output1 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
//			//102	210	2	Full	W/R(write only when manual output6 enable)	Output6 ,Analog output1, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.
//
//			short nRange;//=multi_register_value[186];
//			int nValue;//e=multi_register_value[102];
//			//if (newtstat6[7] == 6)
//			nRange = product_register_value[MODBUS_OUTPUT1_SCALE]; //186  207
//			nValue = product_register_value[MODBUS_COOLING_VALVE]; //102  210
//			/*
//			if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
//			{
//				nRange=newtstat6[207];
//				nValue=newtstat6[210];
//			}else
//			{
//				nRange=multi_register_value[186];
//				nValue=multi_register_value[102];
//
//			}
//			*/
//			if(nRange==0)
//			{				
//				if(nValue==0)	
//					strTemp=_T("Off");
//				if(nValue==1000)
//					strTemp=_T("On");
//			}
//			else
//			{
//				//strTemp.Format(_T("%.1f"),nValue/100.0);
//					//strTemp.Format(_T("%.1f"),multi_register_value[102]/100.0);
//				float nvalue=0.0;
//				//if (newtstat6[7] == 6)
//				if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//				{
//					AfxMessageBox(_T("It's impossible to enter this place!"));
//					if(nRange==1)//0-10v
//					{
//						//nvalue=multi_register_value[102]/100 /10.0 * 100%;
//						nvalue= product_register_value[MODBUS_COOLING_VALVE]/10.0f;//  T6=210
//					}
//					if(nRange==2)//0-5v
//					{
//						nvalue=newtstat6[210]/5.0f;
//					}
//					if(nRange==3)//2-10v
//					{
//						nvalue=newtstat6[210]/8.0f;
//					}
//					if(nRange==3)//10-0v
//					{
//						nvalue=(10-newtstat6[210]/100.0f)/10.0f *100;
//					}
//				}
//				else
//				{
//
//
//					if(nRange==1)//0-10v
//					{
//						//nvalue=multi_register_value[102]/100 /10.0 * 100%;
//						nvalue=product_register_value[MODBUS_COOLING_VALVE]/10.0f;//102   210
//					}
//					if(nRange==2)//0-5v
//					{
//						nvalue=product_register_value[MODBUS_COOLING_VALVE]/5.0f;
//					}
//					if(nRange==3)//2-10v
//					{
//						nvalue=product_register_value[MODBUS_COOLING_VALVE]/8.0f;
//					}
//					if(nRange==3)//10-0v
//					{
//						nvalue=(10-product_register_value[MODBUS_COOLING_VALVE]/100.0f)/10.0f *100;
//					}
//				}
//				strTemp.Format(_T("%.1f%%"),nvalue);
//			}
//		if(m_nModeType==2)
//		{
//	
//			m_FlexGrid.put_TextMatrix(4,VALUE_OUTFIELD,strTemp);
//			if((int)(nAMVAlue & 8))
//			{
//				strTemp=_T("Manual");
//				m_FlexGrid.put_Col(VALUE_OUTFIELD);
//				m_FlexGrid.put_Row(4);
//				m_FlexGrid.put_CellBackColor(COLOR_CELL);
//			}
//			else
//			{
//				strTemp=_T("Auto");
//				m_FlexGrid.put_Col(VALUE_OUTFIELD);
//				m_FlexGrid.put_Row(4);
//				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
//			}
//			m_FlexGrid.put_TextMatrix(4,AM_OUTFIELD,strTemp);
//
//			if(nRange>=0&&nRange<5)
//			{
//				strTemp=OUTPUT_ANRANGE[nRange];
//			}
//			m_FlexGrid.put_TextMatrix(4,RANG_OUTFIELD,strTemp);
//
//			//row5
//
//
//			//103	211	2	Full	W/R(write only when manual output7 enable)	Output7 Analog output2, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.    
//			//187	208	1	Low byte	W/R	Analog Output2 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
//
//			nRange= product_register_value[MODBUS_OUTPUT2_SCALE];//187  208
//			nValue= product_register_value[MODBUS_HEATING_VALVE];//103  211
//			/*//Fan
//			//if (newtstat6[7] == 6)
//			if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
//			{
//				nRange=newtstat6[208];
//				nValue=newtstat6[211];
//			}else
//			{
//				nRange=multi_register_value[187];
//				nValue=multi_register_value[103];
//
//			}
//			*/
//			if(nRange==0)
//			{
//				
//				if(nValue==0)
//					strTemp=_T("Off");
//				if(nValue==1)
//					strTemp=_T("On");
//				/*	
//			if(nValue<500)
//					strTemp=_T("Off");
//				else
//					strTemp=_T("On");
//					*/
//
//
//			}
//			else
//			{
//				//strTemp.Format(_T("%.1f"),nValue/100.0);
//
//				float nvalue=0.0;
//				//if (newtstat6[7] == 6)
//				if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//				{
//					AfxMessageBox(_T("It's impossible to enter this place!222"));
//					if(nRange==1)//0-10v
//					{
//						//nvalue=multi_register_value[102]/100 /10.0 * 100%;
//						nvalue=newtstat6[211]/10.0f;
//					}
//					if(nRange==2)//0-5v
//					{
//						nvalue=newtstat6[211]/5.0f;
//					}
//					if(nRange==3)//2-10v
//					{
//						nvalue=newtstat6[211]/8.0f;
//					}
//					if(nRange==4)//10-0v
//					{
//						nvalue=(10-newtstat6[211]/100.0f)/10.0f *100;
//					}
//				}else
//				{
//						if(nRange==1)//0-10v
//						{
//							//nvalue=multi_register_value[102]/100 /10.0 * 100%;
//							nvalue= product_register_value[MODBUS_HEATING_VALVE]/10.0f;//103   211
//						}
//						if(nRange==2)//0-5v
//						{
//							nvalue= product_register_value[MODBUS_HEATING_VALVE]/5.0f;
//						}
//						if(nRange==3)//2-10v
//						{
//							nvalue= product_register_value[MODBUS_HEATING_VALVE]/8.0f;
//						}
//						if(nRange==4)//10-0v
//						{
//							nvalue=(10- product_register_value[MODBUS_HEATING_VALVE]/100.0f)/10.0f *100;
//						}
//				}
//						strTemp.Format(_T("%.1f%%"),nvalue);
//			}
//			m_FlexGrid.put_TextMatrix(5,VALUE_OUTFIELD,strTemp);
//
//
//
//
//			if((int)(nAMVAlue & 16))
//			{
//				strTemp=_T("Manual");
//				m_FlexGrid.put_Col(VALUE_OUTFIELD);
//				m_FlexGrid.put_Row(5);
//				m_FlexGrid.put_CellBackColor(RGB(255,255,255));
//			}
//			else
//			{
//				strTemp=_T("Auto");
//				m_FlexGrid.put_Col(VALUE_OUTFIELD);
//				m_FlexGrid.put_Row(5);
//				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
//			}
//			m_FlexGrid.put_TextMatrix(5,AM_OUTFIELD,strTemp);
//
//			if(nRange>=0&&nRange<5)
//			{
//				strTemp=OUTPUT_ANRANGE[nRange];
//			}
//			m_FlexGrid.put_TextMatrix(5,RANG_OUTFIELD,strTemp);
//		}//end ==2
//		else//out6/7
//		{
//
//			m_FlexGrid.put_TextMatrix(6,VALUE_OUTFIELD,strTemp);
//
//			if((int)(nAMVAlue & 32))
//			{
//				strTemp=_T("Manual");
//				m_FlexGrid.put_Col(VALUE_OUTFIELD);
//				m_FlexGrid.put_Row(6);
//				m_FlexGrid.put_CellBackColor(COLOR_CELL);
//			}
//			else
//			{
//				strTemp=_T("Auto");
//				m_FlexGrid.put_Col(VALUE_OUTFIELD);
//				m_FlexGrid.put_Row(6);
//				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
//			}
//			m_FlexGrid.put_TextMatrix(6,AM_OUTFIELD,strTemp);
//			strTemp.Empty();
//
//			if(nRange>=0&&nRange<5)
//			{
//				strTemp=OUTPUT_ANRANGE[nRange];
//			}
//
//			m_FlexGrid.put_TextMatrix(6,RANG_OUTFIELD,strTemp);
//
//
//			//103	211	2	Full	W/R(write only when manual output7 enable)	Output7 Analog output2, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.    
//			//187	208	1	Low byte	W/R	Analog Output2 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
//			//if (newtstat6[7] == 6)
//
//
//
//			nRange= product_register_value[MODBUS_OUTPUT2_SCALE];//187  208
//			nValue= product_register_value[MODBUS_HEATING_VALVE];//103  211
//
//			//if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
//			//{
//			//	nRange=newtstat6[208];
//			//	nValue=newtstat6[211];
//			//}else
//			//{
//			//	nRange=multi_register_value[187];
//			//	nValue=multi_register_value[103];
//
//			//}
//
//
//			strTemp.Empty();
//			if(nRange==0)
//			{
//				
//				if(nValue==0)
//					strTemp=_T("Off");
//				if(nValue==1000)
//					strTemp=_T("On");
//					/*
//				if(nValue<500)
//					strTemp=_T("Off");
//				else
//					strTemp=_T("On");
//					*/
//			}
//			else
//			{
//				//strTemp.Format(_T("%.1f"),nValue/100.0);
//				float nvalue=0.0;
//				//if (newtstat6[7] == 6)
//				if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//				{
//					if(nRange==1)//0-10v
//					{
//						//nvalue=multi_register_value[102]/100 /10.0 * 100%;
//						nvalue=product_register_value[MODBUS_HEATING_VALVE]/10.0f;//103 211
//					}
//					if(nRange==2)//0-5v
//					{
//						nvalue=product_register_value[MODBUS_HEATING_VALVE]/5.0f;
//					}
//					if(nRange==3)//2-10v
//					{
//						nvalue=product_register_value[MODBUS_HEATING_VALVE]/8.0f;
//					}
//					if(nRange==4)//10-0v
//					{
//						nvalue=(10-product_register_value[MODBUS_HEATING_VALVE]/100.0f)/10.0f *100;
//					}
//				}else
//				{
//						if(nRange==1)//0-10v
//						{
//							//nvalue=multi_register_value[102]/100 /10.0 * 100%;
//							nvalue=product_register_value[MODBUS_HEATING_VALVE]/10.0f;
//						}
//						if(nRange==2)//0-5v
//						{
//							nvalue=product_register_value[MODBUS_HEATING_VALVE]/5.0f;
//						}
//						if(nRange==3)//2-10v
//						{
//							nvalue=product_register_value[MODBUS_HEATING_VALVE]/8.0f;
//						}
//						if(nRange==4)//10-0v
//						{
//							nvalue=(10-product_register_value[MODBUS_HEATING_VALVE]/100.0f)/10.0f *100;
//						}
//				}
//						strTemp.Format(_T("%.1f%%"),nvalue);
//			}
//			m_FlexGrid.put_TextMatrix(7,VALUE_OUTFIELD,strTemp);
//
//			if((int)(nAMVAlue & 64))
//			{
//				strTemp=_T("Manual");
//				m_FlexGrid.put_Col(VALUE_OUTFIELD);
//				m_FlexGrid.put_Row(7);
//				m_FlexGrid.put_CellBackColor(RGB(255,255,255));
//			}
//			else
//			{
//				strTemp=_T("Auto");
//				m_FlexGrid.put_Col(VALUE_OUTFIELD);
//				m_FlexGrid.put_Row(7);
//				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
//			}
//			m_FlexGrid.put_TextMatrix(7,AM_OUTFIELD,strTemp);
//			strTemp.Empty();
//			if(nRange>=0&&nRange<5)
//			{
//				strTemp=OUTPUT_ANRANGE[nRange];
//			}
//			m_FlexGrid.put_TextMatrix(7,RANG_OUTFIELD,strTemp);
//
//
//		}
//		strTemp.Empty();
//
//	}
//	//286	245	1	Low byte	W/R	Interlock for  output1
//
//	CString strlock;
//	int stradd;// = 286;
//	//if (newtstat6[7] == 6)
//	if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//	{
//		stradd =MODBUS_INTERLOCK_OUTPUT1; //245;
//		for (int i = 0;i<7;i++)
//		{
//
//			int itemp = product_register_value[stradd+i];
//			if(itemp>=0&&itemp<6)
//				m_FlexGrid.put_TextMatrix(i+1,6,Interlock[itemp]);
//
//		}
//	}else
//	{
//		stradd =MODBUS_INTERLOCK_OUTPUT1;// 286;
//		for (int i = 0;i<7;i++)
//		{
//
//			int itemp = product_register_value[stradd+i];
//			if(itemp>=0&&itemp<6)
//				m_FlexGrid.put_TextMatrix(i+1,6,Interlock[itemp]);
//
//		}
//
//
//	}
//
//
//
//
//
//}

BEGIN_EVENTSINK_MAP(COutputSetDlg, CDialog)
	ON_EVENT(COutputSetDlg, IDC_MSFLEXGRID1, DISPID_CLICK, COutputSetDlg::ClickMsflexgrid1, VTS_NONE)
END_EVENTSINK_MAP()

void COutputSetDlg::ClickMsflexgrid1()
{ 

 #if 1//Tstat67
	if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
	{
	m_FlexGrid.SetFocus();
	long lRow,lCol;
	lRow = m_FlexGrid.get_RowSel();//获取点击的行号	
	lCol = m_FlexGrid.get_ColSel(); //获取点击的列号
	if(lRow>m_FlexGrid.get_Rows()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow == 0) //如果点击标题行，也无效
		return;
	CRect rect;
	m_FlexGrid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); 
	//转换为客户区矩形	
	// MSFlexGrid控件的函数的长度单位是"缇(twips)"，
	//需要将其转化为像素，1440缇= 1英寸
	CDC* pDC =GetDC();
	//计算象素点和缇的转换比例
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_FlexGrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_FlexGrid.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_FlexGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_FlexGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rc.OffsetRect(rect.left+1,rect.top+1);
	//获取选中格的文本信息
	CString strValue = m_FlexGrid.get_TextMatrix(lRow,lCol);
	m_nCurRow=lRow;
	m_nCurCol=lCol;

	if(VALUE_OUTFIELD==lCol)
	{
		if(lRow==1||lRow==2||lRow==3||lRow==4||lRow==5||lRow==6||lRow==7)
		{
			if(DISABLE_COLOR_CELL==m_FlexGrid.get_CellBackColor())
				return;

			m_OutValueCmbox.ResetContent();
			m_OutValueCmbox.AddString(_T("Off"));
			m_OutValueCmbox.AddString(_T("On"));
			m_OutValueCmbox.ShowWindow(SW_SHOW);//显示控件

			m_OutValueCmbox.MoveWindow(rc);			//移动到选中格的位置，覆盖
			m_OutValueCmbox.BringWindowToTop();
			m_OutValueCmbox.SelectString(-1,strValue);
			m_OutValueCmbox.SetFocus();					//获取焦点

			//310	254	1	Low byte	W/R	"Output auto/manual enable. Bit 0 to 4 correspond to output1 to output5, bit 5 correspond to 
			//output6, bit 6 correspond to output7. 0, auto mode; 1, manual mode."

			/////////////////////////////////////////////////////////////////////////////////
			// below added by zgq;
			// 				if(m_nModeType==16 && lRow==4)
			// 				{
			// 					int nTempValue = newtstat6[254];
			// 					if(!(nTempValue & 0x08))	 // A/M，如选择Auto，不动，否则看Range
			// 						m_OutValueCmbox.ShowWindow(SW_HIDE);	
			// 					else
			// 					{
			// 						//283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
			// 
			// 						int nTempValue = newtstat6[205];
			// 						if ( nTempValue != 0 )
			// 						{
			// 							m_OutValueCmbox.ShowWindow(SW_HIDE);
			// 							m_OutValueEdt.ShowWindow(SW_SHOW);	
			// 							m_OutValueEdt.SetWindowText(strValue);
			// 							m_OutValueEdt.MoveWindow(rc); //移动到选中格的位置，覆盖
			// 							m_OutValueEdt.BringWindowToTop();
			// 							m_OutValueEdt.SetFocus(); //获取焦点
			// 						}					
			// 					}
			// 				}

			//if((m_nModeType==PM_TSTAT5D||m_nModeType==PM_TSTAT5E ||m_nModeType==PM_TSTAT6 ||m_nModeType==PM_TSTAT7  ) && lRow>5)
			if(lRow<=5)
			{  //  MODBUS_DIGITAL_OUTPUT_STATUS
				//	int nTempValue = newtstat6[254];		//Marked by Fance 2013 04 11
				int nTempValue = product_register_value[MODBUS_OUTPUT_MANU_ENABLE];  //T5=310  //T6=254
				DWORD dwFlag = 0x01;
				bool bRowAuto =MKBOOL(nTempValue & (dwFlag << (lRow-1)));

				if(!bRowAuto)	 // A/M，如选择Auto，不动，否则看Range
					m_OutValueCmbox.ShowWindow(SW_HIDE);	
				else
				{
					//	283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
					//	284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM

					int nTempValue = newtstat6[205+lRow-4];//180找不到对应的tstat6
					if ( nTempValue != 0 )
					{
						m_OutValueCmbox.ShowWindow(SW_HIDE);
						m_OutValueEdt.ShowWindow(SW_SHOW);	
						m_OutValueEdt.SetWindowText(strValue);
						m_OutValueEdt.MoveWindow(rc); //移动到选中格的位置，覆盖
						m_OutValueEdt.BringWindowToTop();
						m_OutValueEdt.SetFocus(); //获取焦点
					}					
				}
			}
		
		}
	}

	if(lCol==AM_OUTFIELD)
	{   
		 
			


		if(lRow==1||lRow==2||lRow==3||lRow==4||lRow==5||lRow==6||lRow==7)
		{
			if(DISABLE_COLOR_CELL==m_FlexGrid.get_CellBackColor())
				return;

			m_outAMCmbox.ResetContent();
			m_outAMCmbox.AddString(_T("Auto"));
			m_outAMCmbox.AddString(_T("Manual"));
			m_outAMCmbox.ShowWindow(SW_SHOW);



			//显示控件
			m_outAMCmbox.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_outAMCmbox.BringWindowToTop();
			m_outAMCmbox.SelectString(-1,strValue);
			m_outAMCmbox.SetFocus(); //获取焦点
		}

	}
	if(lCol==RANG_OUTFIELD)
	{
		
		if(lRow<=5)
		{
			m_outRangCmbox.ResetContent();


			//	for(int i=0;i<2;i++)
			for(int i=0;i<4;i++)//2.5.0.98
			{
				m_outRangCmbox.AddString(OUTPUT_RANGE45[i]);
			}

			m_outRangCmbox.ShowWindow(SW_SHOW);//显示控件
			m_outRangCmbox.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_outRangCmbox.BringWindowToTop();
			m_outRangCmbox.SelectString(-1,strValue);
			m_outRangCmbox.SetFocus(); //获取焦点

		}
		if((lRow==6||lRow==7))//model DEG TStat6,TStat7
		{

			m_outRangCmbox.ResetContent();
			for(int i=0;i<5;i++)
			{
				m_outRangCmbox.AddString(OUTPUT_ANRANGE[i]);
			}

			m_outRangCmbox.ShowWindow(SW_SHOW);//显示控件
			m_outRangCmbox.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_outRangCmbox.BringWindowToTop();
			m_outRangCmbox.SelectString(-1,strValue);
			m_outRangCmbox.SetFocus(); //获取焦点
		}

	}
	if(lCol==FUN_OUTFIELD)
	{
		if(lRow>=1&&lRow<=5)
		{
			m_outFunCmbox.ResetContent();
			//for(int i=0;i<4;i++)
			for(int i=0;i<3;i++)//2.5.0.98
			{
				m_outFunCmbox.AddString(ONTPUT_FUNS[i]);
				//AfxMessageBox(ONTPUT_FUNS[i]);
			}
			m_outFunCmbox.ShowWindow(SW_SHOW);//显示控件
			m_outFunCmbox.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_outFunCmbox.BringWindowToTop();
			m_outFunCmbox.SelectString(-1,strValue);
			m_outFunCmbox.SetFocus(); //获取焦点
		}

	}
	if((lCol==NAME_OUTFIELD)||(lCol==7)||(lCol==8))
	{
		m_outputNameEDIT.MoveWindow(&rc,1);
		m_outputNameEDIT.ShowWindow(SW_SHOW);
		m_outputNameEDIT.SetWindowText(strValue);
		m_outputNameEDIT.SetFocus();
		int nLenth=strValue.GetLength();
		m_outputNameEDIT.SetSel(nLenth,nLenth); //全选//
	}


	if(lCol==6)
	{

		m_Interlockcombo.ResetContent();
		for(int i=0;i<6;i++)
		{
			m_Interlockcombo.AddString(Interlock[i]);
			//AfxMessageBox(ONTPUT_FUNS[i]);
		}
		m_Interlockcombo.ShowWindow(SW_SHOW);//显示控件
		m_Interlockcombo.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_Interlockcombo.BringWindowToTop();
		m_Interlockcombo.SelectString(-1,strValue);
		m_Interlockcombo.SetFocus(); //获取焦点

	}

	}
#endif
	else
	{
		m_FlexGrid.SetFocus();
		long lRow,lCol;
		lRow = m_FlexGrid.get_RowSel();//获取点击的行号	
		lCol = m_FlexGrid.get_ColSel(); //获取点击的列号
		if(lRow>m_FlexGrid.get_Rows()) //如果点击区超过最大行号，则点击是无效的
			return;
		if(lRow == 0) //如果点击标题行，也无效
		return;
	CRect rect;
	m_FlexGrid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	// MSFlexGrid控件的函数的长度单位是"缇(twips)"，
	//需要将其转化为像素，1440缇= 1英寸
	CDC* pDC =GetDC();
	//计算象素点和缇的转换比例
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_FlexGrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_FlexGrid.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_FlexGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_FlexGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rc.OffsetRect(rect.left+1,rect.top+1);
	//获取选中格的文本信息
	CString strValue = m_FlexGrid.get_TextMatrix(lRow,lCol);
	m_nCurRow=lRow;
	m_nCurCol=lCol;

	if(VALUE_OUTFIELD==lCol)
	{
		if(lRow==1||lRow==2||lRow==3||lRow==4||lRow==5||lRow==6||lRow==7)
		{
			if(DISABLE_COLOR_CELL==m_FlexGrid.get_CellBackColor())
					return;

			m_OutValueCmbox.ResetContent();
			m_OutValueCmbox.AddString(_T("Off"));
			m_OutValueCmbox.AddString(_T("On"));
			m_OutValueCmbox.ShowWindow(SW_SHOW);//显示控件

			m_OutValueCmbox.MoveWindow(rc);			//移动到选中格的位置，覆盖
			m_OutValueCmbox.BringWindowToTop();
			m_OutValueCmbox.SelectString(-1,strValue);
			m_OutValueCmbox.SetFocus();					//获取焦点
			
			/////////////////////////////////////////////////////////////////////////////////
			// below added by zgq;
			if(m_nModeType==16 && lRow==4)
			{
				int nTempValue = multi_register_value[310];
				if(!(nTempValue & 0x08))	 // A/M，如选择Auto，不动，否则看Range
					m_OutValueCmbox.ShowWindow(SW_HIDE);	
				else
				{
					int nTempValue = multi_register_value[283];
					if ( nTempValue != 0 )
					{
						m_OutValueCmbox.ShowWindow(SW_HIDE);
						m_OutValueEdt.ShowWindow(SW_SHOW);	
						m_OutValueEdt.SetWindowText(strValue);
						m_OutValueEdt.MoveWindow(rc); //移动到选中格的位置，覆盖
						m_OutValueEdt.BringWindowToTop();
						m_OutValueEdt.SetFocus(); //获取焦点
					}					
				}
			}

			if((m_nModeType==PM_TSTAT5D||m_nModeType==PM_TSTAT5E ||m_nModeType==PM_TSTAT6 ||m_nModeType==PM_TSTAT7  ) && lRow>5)
			{
				int nTempValue = multi_register_value[310];
				DWORD dwFlag = 0x01;
				bool bRowAuto =MKBOOL(nTempValue & (dwFlag << (lRow-1)));
				
				if(!bRowAuto)	 // A/M，如选择Auto，不动，否则看Range
					m_OutValueCmbox.ShowWindow(SW_HIDE);	
				else
				{
					int nTempValue = multi_register_value[180+lRow];
					if ( nTempValue != 0 )
					{
						m_OutValueCmbox.ShowWindow(SW_HIDE);
						m_OutValueEdt.ShowWindow(SW_SHOW);	
						m_OutValueEdt.SetWindowText(strValue);
						m_OutValueEdt.MoveWindow(rc); //移动到选中格的位置，覆盖
						m_OutValueEdt.BringWindowToTop();
						m_OutValueEdt.SetFocus(); //获取焦点
					}					
				}
			}
			// on top added by zgq;2010-12-07; output4，5，使用新的规则，详细见资料“OutPut各栏对应关系规则.xls”
			//////////////////////////////////////////////////////////////////////////
	

			if(m_nModeType==2&&lRow==4)
			{
				if(multi_register_value[186]==0)//ON/oFF
					m_OutValueEdt.ShowWindow(SW_HIDE);	
				else
				{
					m_OutValueCmbox.ShowWindow(SW_HIDE);
					m_OutValueEdt.ShowWindow(SW_SHOW);	
					
					m_OutValueEdt.SetWindowText(strValue);
					m_OutValueEdt.MoveWindow(rc); //移动到选中格的位置，覆盖
					m_OutValueEdt.BringWindowToTop();
					m_OutValueEdt.SetFocus(); //获取焦点
				}
			}

			if(m_nModeType==2&&lRow==5)
			{
				if(multi_register_value[187]==0)//ON/oFF
					m_OutValueEdt.ShowWindow(SW_HIDE);	
				else
				{
					m_OutValueCmbox.ShowWindow(SW_HIDE);
					m_OutValueEdt.ShowWindow(SW_SHOW);	
					m_OutValueEdt.SetWindowText(strValue);
					m_OutValueEdt.MoveWindow(rc); //移动到选中格的位置，覆盖
					m_OutValueEdt.BringWindowToTop();
					m_OutValueEdt.SetFocus(); //获取焦点
				}

			}

	if((m_nModeType==17||m_nModeType==18)&&lRow==4)//2.5.0.94
		//	if((m_nModeType==17)&&lRow==4)
			{
				//if(multi_register_value[283]==0)//ON/oFF
				if(multi_register_value[283]==0)//ON/oFF
					m_OutValueEdt.ShowWindow(SW_HIDE);	
				else
				{
					m_OutValueCmbox.ShowWindow(SW_HIDE);
					m_OutValueEdt.ShowWindow(SW_SHOW);	
					m_OutValueEdt.SetWindowText(strValue);
					m_OutValueEdt.MoveWindow(rc); //移动到选中格的位置，覆盖
					m_OutValueEdt.BringWindowToTop();
					m_OutValueEdt.SetFocus(); //获取焦点
				}

			}

				if((m_nModeType==17||m_nModeType==18)&&lRow==5)//2.5.0.94
			//if((m_nModeType==17)&&lRow==5)
			{
				if(multi_register_value[284]==0)//ON/oFF
					m_OutValueEdt.ShowWindow(SW_HIDE);	
				else
				{
					m_OutValueCmbox.ShowWindow(SW_HIDE);
					m_OutValueEdt.ShowWindow(SW_SHOW);	
					m_OutValueEdt.SetWindowText(strValue);
					m_OutValueEdt.MoveWindow(rc); //移动到选中格的位置，覆盖
					m_OutValueEdt.BringWindowToTop();
					m_OutValueEdt.SetFocus(); //获取焦点
				}

			}
//2.5.0.94
#if 0
			if((m_nModeType==18)&&lRow==4)
			{
// 				if(multi_register_value[283]==0)//ON/oFF
// 					m_OutValueEdt.ShowWindow(SW_HIDE);	
// 				else
// 				{
// 					m_OutValueCmbox.ShowWindow(SW_HIDE);
// 					m_OutValueEdt.ShowWindow(SW_SHOW);	
// 					m_OutValueEdt.SetWindowText(strValue);
// 					m_OutValueEdt.MoveWindow(rc); //移动到选中格的位置，覆盖
// 					m_OutValueEdt.BringWindowToTop();
// 					m_OutValueEdt.SetFocus(); //获取焦点
// 				}

			}

			if((m_nModeType==17||m_nModeType==18)&&lRow==5)
			{
// 				if(multi_register_value[284]==0)//ON/oFF
// 					m_OutValueEdt.ShowWindow(SW_HIDE);	
// 				else
// 				{
// 					m_OutValueCmbox.ShowWindow(SW_HIDE);
// 					m_OutValueEdt.ShowWindow(SW_SHOW);	
// 					m_OutValueEdt.SetWindowText(strValue);
// 					m_OutValueEdt.MoveWindow(rc); //移动到选中格的位置，覆盖
// 					m_OutValueEdt.BringWindowToTop();
// 					m_OutValueEdt.SetFocus(); //获取焦点
// 				}

			}

#endif




	
			if((m_nModeType==12||m_nModeType==16||m_nModeType==18||m_nModeType==PM_PRESSURE)&&lRow==6)
			{
				if((m_nModeType==17||m_nModeType==18)&&lRow==4)
			{
				if(multi_register_value[283]==0)//ON/oFF
					m_OutValueEdt.ShowWindow(SW_HIDE);	
				else
				{
					m_OutValueCmbox.ShowWindow(SW_HIDE);
					m_OutValueEdt.ShowWindow(SW_SHOW);	
					m_OutValueEdt.SetWindowText(strValue);
					m_OutValueEdt.MoveWindow(rc); //移动到选中格的位置，覆盖
					m_OutValueEdt.BringWindowToTop();
					m_OutValueEdt.SetFocus(); //获取焦点
				}

			}

			if((m_nModeType==17||m_nModeType==18)&&lRow==5)
			{
				if(multi_register_value[284]==0)//ON/oFF
					m_OutValueEdt.ShowWindow(SW_HIDE);	
				else
				{
					m_OutValueCmbox.ShowWindow(SW_HIDE);
					m_OutValueEdt.ShowWindow(SW_SHOW);	
					m_OutValueEdt.SetWindowText(strValue);
					m_OutValueEdt.MoveWindow(rc); //移动到选中格的位置，覆盖
					m_OutValueEdt.BringWindowToTop();
					m_OutValueEdt.SetFocus(); //获取焦点
				}

			}
			if(multi_register_value[186]==0)//ON/oFF
					m_OutValueEdt.ShowWindow(SW_HIDE);	
				else
				{
					m_OutValueCmbox.ShowWindow(SW_HIDE);
					m_OutValueEdt.ShowWindow(SW_SHOW);
						m_OutValueEdt.SetWindowText(strValue);
					m_OutValueEdt.MoveWindow(rc); //移动到选中格的位置，覆盖
					m_OutValueEdt.BringWindowToTop();
					m_OutValueEdt.SetFocus(); //获取焦点
				}
			}

			if((m_nModeType==12||m_nModeType==16||m_nModeType==18||m_nModeType==PM_PRESSURE)&&lRow==7)
			{
				if(multi_register_value[187]==0)//ON/oFF
					m_OutValueEdt.ShowWindow(SW_HIDE);	
				else
				{
					m_outAMCmbox.ShowWindow(SW_HIDE);
					m_OutValueEdt.ShowWindow(SW_SHOW);	
					m_OutValueEdt.SetWindowText(strValue);
					m_OutValueEdt.MoveWindow(rc); //移动到选中格的位置，覆盖
					m_OutValueEdt.BringWindowToTop();
					m_OutValueEdt.SetFocus(); //获取焦点
				}
			}
		}
	}

	if(lCol==AM_OUTFIELD)
	{
		if(lRow==1||lRow==2||lRow==3||lRow==4||lRow==5||lRow==6||lRow==7)
		{
			m_outAMCmbox.ResetContent();
			m_outAMCmbox.AddString(_T("Auto"));
			m_outAMCmbox.AddString(_T("Manual"));
			m_outAMCmbox.ShowWindow(SW_SHOW);

		

			//显示控件
			m_outAMCmbox.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_outAMCmbox.BringWindowToTop();
			m_outAMCmbox.SelectString(-1,strValue);
			m_outAMCmbox.SetFocus(); //获取焦点
		}
		
	}
	if(lCol==RANG_OUTFIELD)
	{
		if((lRow==4||lRow==5)&&m_nModeType==2)//model a
		{	
			m_outRangCmbox.ResetContent();
	
			//for(int i=0;i<5;i++)
			for(int i=0;i<3;i++)//2.5.0.98
			{
				m_outRangCmbox.AddString(OUTPUT_ANRANGE[i]);
			}


			m_outRangCmbox.ShowWindow(SW_SHOW);//显示控件
			m_outRangCmbox.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_outRangCmbox.BringWindowToTop();
			m_outRangCmbox.SelectString(-1,strValue);
			m_outRangCmbox.SetFocus(); //获取焦点
			
			
		}
		if((lRow==4||lRow==5)&&m_nModeType!=2)//model a
		{
			m_outRangCmbox.ResetContent();


			//for(int i=0;i<2;i++)
			for(int i=0;i<4;i++)//2.5.0.98
			{
				m_outRangCmbox.AddString(OUTPUT_RANGE45[i]);
			}

			m_outRangCmbox.ShowWindow(SW_SHOW);//显示控件
			m_outRangCmbox.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_outRangCmbox.BringWindowToTop();
			m_outRangCmbox.SelectString(-1,strValue);
			m_outRangCmbox.SetFocus(); //获取焦点
	
		}
		if((lRow==6||lRow==7)
			&&(m_nModeType==12||m_nModeType==16||m_nModeType==18||m_nModeType==PM_TSTAT6
			||m_nModeType==PM_TSTAT7||m_nModeType==PM_PRESSURE))//model DEG TStat6,TStat7
		{

			m_outRangCmbox.ResetContent();
			//for(int i=0;i<5;i++)
			for(int i=0;i<3;i++)//2.5.0.98
			{
				m_outRangCmbox.AddString(OUTPUT_ANRANGE[i]);
			}

			m_outRangCmbox.ShowWindow(SW_SHOW);//显示控件
			m_outRangCmbox.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_outRangCmbox.BringWindowToTop();
			m_outRangCmbox.SelectString(-1,strValue);
			m_outRangCmbox.SetFocus(); //获取焦点
		}

	}
	if(lCol==FUN_OUTFIELD)
	{
#if 0//2.5.0.98
		if(lRow>=1&&lRow<=5)
		{
			m_outFunCmbox.ResetContent();
		//	for(int i=0;i<4;i++)
			for(int i=0;i<3;i++)//2.5.0.98
			{
				m_outFunCmbox.AddString(ONTPUT_FUNS[i]);
				//AfxMessageBox(ONTPUT_FUNS[i]);
			}
			m_outFunCmbox.ShowWindow(SW_SHOW);//显示控件
			m_outFunCmbox.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_outFunCmbox.BringWindowToTop();
			m_outFunCmbox.SelectString(-1,strValue);
			m_outFunCmbox.SetFocus(); //获取焦点
		}
#endif
		
	}
	if((lCol==NAME_OUTFIELD)||(lCol==7)||(lCol==8))
	{
		m_outputNameEDIT.MoveWindow(&rc,1);
		m_outputNameEDIT.ShowWindow(SW_SHOW);
		m_outputNameEDIT.SetWindowText(strValue);
		m_outputNameEDIT.SetFocus();
		int nLenth=strValue.GetLength();
		m_outputNameEDIT.SetSel(nLenth,nLenth); //全选//
	}


 	if(lCol==6)
 	{

			m_Interlockcombo.ResetContent();
			for(int i=0;i<6;i++)
			{
				m_Interlockcombo.AddString(Interlock[i]);
				//AfxMessageBox(ONTPUT_FUNS[i]);
			}
			m_Interlockcombo.ShowWindow(SW_SHOW);//显示控件
			m_Interlockcombo.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_Interlockcombo.BringWindowToTop();
			m_Interlockcombo.SelectString(-1,strValue);
			m_Interlockcombo.SetFocus(); //获取焦点

 	}

}
}

void COutputSetDlg::OnCbnKillfocusOvaluecombo()
{
	m_OutValueCmbox.ShowWindow(SW_HIDE);
}

void COutputSetDlg::OnCbnKillfocusOamcombo()
{
	m_outAMCmbox.ShowWindow(SW_HIDE);
}
//Recoding by Fan  2013 03 29
void COutputSetDlg::OnCbnSelchangeOvaluecombo()
{
	BeginWaitCursor();
	//tstat6
	//108	209	1	Low byte	W/R	Output1 tot 5, bit 0 thru 4 = relay 1 thru 5.

	int n;
	n=product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS];//108  209

	int i;
	int nchange=0;
	if(m_nCurRow==1&&m_nCurCol==VALUE_OUTFIELD)
	{
		if(m_OutValueCmbox.GetCurSel()==1)
		{
			i=1;
			nchange=n|i;	
		}
		else
		{
			i=0xfe;
			nchange=n & 0xfe;
		}
		if(nchange>=0)
		{
			write_one(g_tstat_id,MODBUS_DIGITAL_OUTPUT_STATUS,nchange);
			product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS]=nchange;
		}
	}

	if(m_nCurRow==2&&m_nCurCol==VALUE_OUTFIELD)
	{
		if(m_OutValueCmbox.GetCurSel()==1)
		{
			i=2;
			nchange=n|i;	
		}
		else
		{
			i=0xfd;
			nchange=n & i;
		}
		if(nchange>=0)
		{
			write_one(g_tstat_id,MODBUS_DIGITAL_OUTPUT_STATUS,nchange);
			product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS]=nchange;

		}
	}

	if(m_nCurRow==3&&m_nCurCol==VALUE_OUTFIELD)
	{

		if(m_OutValueCmbox.GetCurSel()==1)
		{
			i=4;
			nchange=n|i;	
		}
		else
		{
			i=0xfb;
			nchange=n & i;
		}
		if(nchange>=0)
		{
			write_one(g_tstat_id,MODBUS_DIGITAL_OUTPUT_STATUS,nchange);
			product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS]=nchange;
		}
	}

	//BCDE;
	if(m_nModeType==1||m_nModeType==4||m_nModeType==12||m_nModeType==16
		||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7||m_nModeType==PM_PRESSURE) 
	{
		if(m_nCurRow==4&&m_nCurCol==VALUE_OUTFIELD)
		{
			if(m_OutValueCmbox.GetCurSel()==1)
			{
				i=8;
				nchange=n|i;	
			}
			else
			{
				i=0xf7;
				nchange=n & i;
			}
			if(nchange>=0)
			{
				write_one(g_tstat_id,MODBUS_DIGITAL_OUTPUT_STATUS,nchange);
				product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS]=nchange;

				//tstat6
			}
		}

		if(m_nCurRow==5&&m_nCurCol==VALUE_OUTFIELD)
		{

			if(m_OutValueCmbox.GetCurSel()==1)
			{
				i=16;
				nchange=n|i;	
			}
			else
			{
				i=0xef;
				nchange=n & i;
			}
			if(nchange>=0)
			{
				write_one(g_tstat_id,MODBUS_DIGITAL_OUTPUT_STATUS,nchange);
				product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS]=nchange;

				//tstat6
			}
		}

	}
	//A
	//186	207	1	Low byte	W/R	Analog Output1 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
	//102	210	2	Full	W/R(write only when manual output6 enable)	Output6 ,Analog output1, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.

	if(m_nModeType==2)//A
	{
		if(m_nCurRow==4&&m_nCurCol==VALUE_OUTFIELD)
		{
			if(product_register_value[MODBUS_OUTPUT1_SCALE]==0)	//186  207
			{
				if(m_OutValueCmbox.GetCurSel()==1)//ON
				{
					write_one(g_tstat_id,MODBUS_COOLING_VALVE,1);//102 210
					product_register_value[MODBUS_COOLING_VALVE]=1;
				}
				else
				{
					write_one(g_tstat_id,MODBUS_COOLING_VALVE,0);
					product_register_value[MODBUS_COOLING_VALVE]=0;
				}
			}
		}

		//103	211	2	Full	W/R(write only when manual output7 enable)	Output7 Analog output2, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.    

		if(m_nCurRow==5&&m_nCurCol==VALUE_OUTFIELD)
		{
			if(product_register_value[MODBUS_OUTPUT2_SCALE]==0)  //187  208
			{
				if(m_OutValueCmbox.GetCurSel()==1)//ON
				{
					write_one(g_tstat_id,MODBUS_HEATING_VALVE,1);//103 211
					product_register_value[MODBUS_HEATING_VALVE]=1;
				}
				else
				{
					write_one(g_tstat_id,MODBUS_HEATING_VALVE,0);
					product_register_value[MODBUS_HEATING_VALVE]=0;
				}
			}
		}
	}

	//FG
	//283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
	//	if(m_nModeType==17)
	if(m_nModeType==17||m_nModeType==18)//2.5.0.94
	{
		if(m_nCurRow==4&&m_nCurCol==VALUE_OUTFIELD)
		{
			int tmep =0;

			tmep = product_register_value[MODBUS_MODE_OUTPUT4];//283  205

			if(tmep==0)//ON/oFF
			{
				if(m_OutValueCmbox.GetCurSel()==1)
				{
					i=8;
					nchange=n|i;	
				}
				else
				{
					i=0xf7;
					nchange=n & i;
				}
				if(nchange>=0)
				{
					//tstat6
					//108	209	1	Low byte	W/R	Output1 tot 5, bit 0 thru 4 = relay 1 thru 5.

					write_one(g_tstat_id,MODBUS_DIGITAL_OUTPUT_STATUS,0);//108  209
					product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS]=0;

				}
			}
		}
		if(m_nCurRow==5&&m_nCurCol==VALUE_OUTFIELD)
		{
			//284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM

			int tin = 0;
			tin = product_register_value[MODBUS_MODE_OUTPUT5];//284  206

			if(tin==0)//ON/oFF
			{
				if(m_OutValueCmbox.GetCurSel()==1)
				{
					i=16;
					nchange=n|i;	
				}
				else
				{
					i=0xef;
					nchange=n & i;
				}
				if(nchange>=0)
				{
					write_one(g_tstat_id,MODBUS_DIGITAL_OUTPUT_STATUS,0);//108  209
					product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS]=0;
				}

			}
		}
	}

	//DEG:out6/out7:
	//186	207	1	Low byte	W/R	Analog Output1 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
	if(m_nModeType==12||m_nModeType==16||m_nModeType==18||m_nModeType==PM_TSTAT6
		||m_nModeType==PM_TSTAT7 ||m_nModeType==PM_PRESSURE)
	{
		if(m_nCurRow==6&&m_nCurCol==VALUE_OUTFIELD)
		{
			int iit = 0;

			iit = product_register_value[MODBUS_OUTPUT1_SCALE];//186 207
			if(iit==0)//ON/oFF
			{
				if(m_OutValueCmbox.GetCurSel()==1)//ON
				{
					write_one(g_tstat_id,MODBUS_COOLING_VALVE,1000);//102  210
					product_register_value[MODBUS_COOLING_VALVE]=1000;
				}
				else
				{
					write_one(g_tstat_id,MODBUS_COOLING_VALVE,0);//102  210
					product_register_value[MODBUS_COOLING_VALVE]=0;
				}
			}
		}
		if(m_nCurRow==7&&m_nCurCol==VALUE_OUTFIELD)
		{
			//187	208	1	Low byte	W/R	Analog Output2 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 

			int iti =0;

			iti = product_register_value[MODBUS_OUTPUT2_SCALE];	//187  208
			if(iti==0)//ON/oFF
			{

				if(m_OutValueCmbox.GetCurSel()==1)//ON
				{
					write_one(g_tstat_id,MODBUS_HEATING_VALVE,1000);//103  211
					product_register_value[MODBUS_HEATING_VALVE]=1000;
				}
				else
				{
					write_one(g_tstat_id,MODBUS_HEATING_VALVE,0);//103  211
					product_register_value[MODBUS_HEATING_VALVE]=0;
				}
			}
		}
	}

	Fresh_Grid();

	EndWaitCursor();

}


//Recode By Fan Du 2013-03-29
//void COutputSetDlg::OnCbnSelchangeOvaluecombo()
//{
//	BeginWaitCursor();
////tstat6
//	//108	209	1	Low byte	W/R	Output1 tot 5, bit 0 thru 4 = relay 1 thru 5.
//
//	int n;
////	if (newtstat6[7] == PM_TSTAT6)
//	if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//	{
//// 		int num = reg_tststold[108];
//// 		n = multi_register_value[num];
//		n = newtstat6[209];
//	}else
//		n=multi_register_value[108];
//
//	int i;
//	int nchange=0;
//	if(m_nCurRow==1&&m_nCurCol==VALUE_OUTFIELD)
//	{
//		if(m_OutValueCmbox.GetCurSel()==1)
//		{
//			i=1;
//			nchange=n|i;	
//		}
//		else
//		{
//			i=0xfe;
//			nchange=n & 0xfe;
//		}
//		if(nchange>=0)
//		{
////tstat6
//		//	if (newtstat6[7] == PM_TSTAT6)
//			if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//			{
//				//int num = reg_tststold[108];
//				write_one(g_tstat_id,209,nchange);
//				newtstat6[209] = nchange;
//			}else
//				write_one(g_tstat_id,108,nchange);
//
////tstat6
//		}
//
//
//			
//	}
//
//	if(m_nCurRow==2&&m_nCurCol==VALUE_OUTFIELD)
//	{
//		if(m_OutValueCmbox.GetCurSel()==1)
//		{
//			i=2;
//			nchange=n|i;	
//		}
//		else
//		{
//			i=0xfd;
//			nchange=n & i;
//		}
//		if(nchange>=0)
//		{
//			//tstat6
//		//	if (newtstat6[7] == PM_TSTAT6)
//			if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//			{
//				//int num = reg_tststold[108];
//				write_one(g_tstat_id,209,nchange);
//				newtstat6[209] = nchange;
//			}else
//				write_one(g_tstat_id,108,nchange);
//
//			//tstat6
//
//		}
//	}
//
//	if(m_nCurRow==3&&m_nCurCol==VALUE_OUTFIELD)
//	{
//
//		if(m_OutValueCmbox.GetCurSel()==1)
//		{
//			i=4;
//			nchange=n|i;	
//		}
//		else
//		{
//			i=0xfb;
//			nchange=n & i;
//		}
//		if(nchange>=0)
//		{
//			//tstat6
//			//if (newtstat6[7] == PM_TSTAT6)
//			if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//			{
//				//int num = reg_tststold[108];
//				write_one(g_tstat_id,209,nchange);
//				newtstat6[209] = nchange;
//			}else
//				write_one(g_tstat_id,108,nchange);
//
//			//tstat6
//		}
//	}
//
//	//BCDE;
// 	if(m_nModeType==1||m_nModeType==4||m_nModeType==12||m_nModeType==16
// 		||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7||m_nModeType==PM_PRESSURE) 
//	{
//		if(m_nCurRow==4&&m_nCurCol==VALUE_OUTFIELD)
//		{
//			if(m_OutValueCmbox.GetCurSel()==1)
//			{
//				i=8;
//				nchange=n|i;	
//			}
//			else
//			{
//				i=0xf7;
//				nchange=n & i;
//			}
//			if(nchange>=0)
//
//			{
//				//tstat6
//				//if (newtstat6[7] == PM_TSTAT6)
//				if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//				{
//					//int num = reg_tststold[108];
//					write_one(g_tstat_id,209,nchange);
//					newtstat6[209] = nchange;
//				}else
//					write_one(g_tstat_id,108,nchange);
//
//				//tstat6
//			}
//		}
//
//		if(m_nCurRow==5&&m_nCurCol==VALUE_OUTFIELD)
//		{
//
//			if(m_OutValueCmbox.GetCurSel()==1)
//			{
//				i=16;
//				nchange=n|i;	
//			}
//			else
//			{
//				i=0xef;
//				nchange=n & i;
//			}
//			if(nchange>=0)
//			{
//				//tstat6
//			//	if (newtstat6[7] == PM_TSTAT6)
//				if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//				{
//					//int num = reg_tststold[108];
//					write_one(g_tstat_id,209,nchange);
//					newtstat6[209] = nchange;
//				}else
//					write_one(g_tstat_id,108,nchange);
//
//				//tstat6
//			}
//		}
//
//	}
//	//A
//	//186	207	1	Low byte	W/R	Analog Output1 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
//	//102	210	2	Full	W/R(write only when manual output6 enable)	Output6 ,Analog output1, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.
//
//	if(m_nModeType==2)//A
//	{
//		if(m_nCurRow==4&&m_nCurCol==VALUE_OUTFIELD)
//		{
//			//if (newtstat6[7] == PM_TSTAT6)
//			if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//			{
//				if(newtstat6[207]==0)//ON/oFF
//				{
//					if(m_OutValueCmbox.GetCurSel()==1)//ON
//					{
//						write_one(g_tstat_id,210,1);
//						newtstat6[210] = 1;
//
//					}
//					else
//					{
//						write_one(g_tstat_id,210,0);
//						newtstat6[210] = 0;
//					}
//				}
//			}else
//			{
//					if(multi_register_value[186]==0)//ON/oFF
//					{
//						if(m_OutValueCmbox.GetCurSel()==1)//ON
//						{
//							write_one(g_tstat_id,102,1);
//
//						}
//						else
//						{
//							write_one(g_tstat_id,102,0);
//						}
//					}
//			}
//
//		}
//
//		//103	211	2	Full	W/R(write only when manual output7 enable)	Output7 Analog output2, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.    
//
//		if(m_nCurRow==5&&m_nCurCol==VALUE_OUTFIELD)
//		{
//			if(multi_register_value[187]==0)
//			{
//			//	if (newtstat6[7] == PM_TSTAT6)
//				if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//				{
//					if(m_OutValueCmbox.GetCurSel()==1)//ON
//					{
//
//						write_one(g_tstat_id,211,1);//>500
//						newtstat6[211] = 1;
//					}
//					else
//					{
//						write_one(g_tstat_id,211,0);//<499
//						newtstat6[211] = 0;
//					}
//				}
//				else
//				{
//					if(m_OutValueCmbox.GetCurSel()==1)//ON
//					{
//
//						write_one(g_tstat_id,103,1);//>500
//					}
//					else
//					{
//						write_one(g_tstat_id,103,0);//<499
//					}
//
//				}
//			}
//		}
//	}
//
////FG
//	//283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
////	if(m_nModeType==17)
//	if(m_nModeType==17||m_nModeType==18)//2.5.0.94
//	{
//		if(m_nCurRow==4&&m_nCurCol==VALUE_OUTFIELD)
//		{
//			int tmep =0;
//			//if (newtstat6[7] == PM_TSTAT6)
//			if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//				tmep = newtstat6[205];
//			else
//				tmep = multi_register_value[283];
//			if(tmep==0)//ON/oFF
//			{
//				if(m_OutValueCmbox.GetCurSel()==1)
//				{
//					i=8;
//					nchange=n|i;	
//				}
//				else
//				{
//					i=0xf7;
//					nchange=n & i;
//				}
//				if(nchange>=0)
//				{
//					//tstat6
//					//108	209	1	Low byte	W/R	Output1 tot 5, bit 0 thru 4 = relay 1 thru 5.
//
//					//if (newtstat6[7] == PM_TSTAT6)
//					if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//					{
//						//int num = reg_tststold[108];
//						write_one(g_tstat_id,209,nchange);
//						newtstat6[209] = nchange;
//					}else
//						write_one(g_tstat_id,108,nchange);
//
//					//tstat6
//				}
//			}
//		}
//		if(m_nCurRow==5&&m_nCurCol==VALUE_OUTFIELD)
//		{
//			//284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM
//
//			int tin = 0;
//			//if (newtstat6[7] == PM_TSTAT6)
//			if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//				tin = newtstat6[206];
//			else
//				tin = multi_register_value[284];
//			if(tin==0)//ON/oFF
//			{
//				if(m_OutValueCmbox.GetCurSel()==1)
//				{
//					i=16;
//					nchange=n|i;	
//				}
//				else
//				{
//					i=0xef;
//					nchange=n & i;
//				}
//				if(nchange>=0)
//				{
//					//tstat6
//				//	if (newtstat6[7] == PM_TSTAT6)
//					if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//					{
//						//int num = reg_tststold[108];
//						write_one(g_tstat_id,209,nchange);
//						newtstat6[209] = nchange;
//					}else
//						write_one(g_tstat_id,108,nchange);
//
//					//tstat6
//				}
//			
//			}
//		}
//	}
//	
////DEG:out6/out7:
//	//186	207	1	Low byte	W/R	Analog Output1 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
//	if(m_nModeType==12||m_nModeType==16||m_nModeType==18||m_nModeType==PM_TSTAT6
//		||m_nModeType==PM_TSTAT7 ||m_nModeType==PM_PRESSURE)
//	{
//		if(m_nCurRow==6&&m_nCurCol==VALUE_OUTFIELD)
//		{
//			int iit = 0;
//			//if (newtstat6[7] == PM_TSTAT6)
//			if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//
//				iit = newtstat6[207];
//			else
//				iit = multi_register_value[186];
//			if(iit==0)//ON/oFF
//			{
//				//if (newtstat6[7] == PM_TSTAT6)
//				if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//
//				{	
//					//102	210	2	Full	W/R(write only when manual output6 enable)	Output6 ,Analog output1, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.
//
//					if(m_OutValueCmbox.GetCurSel()==1)//ON
//					{
//						write_one(g_tstat_id,210,1000);//>500
//						newtstat6[210] = 1000;
//					}
//					else
//					{
//						write_one(g_tstat_id,210,0);//<499
//						newtstat6[210] =0;
//					}
//				}else
//				{
//					if(m_OutValueCmbox.GetCurSel()==1)//ON
//					{
//						write_one(g_tstat_id,102,1000);//>500
//					}
//					else
//					{
//						write_one(g_tstat_id,102,0);//<499
//					}
//
//				}
//
//			}
//		}
//		if(m_nCurRow==7&&m_nCurCol==VALUE_OUTFIELD)
//		{
//			//187	208	1	Low byte	W/R	Analog Output2 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
//
//			int iti =0;
//			//if (newtstat6[7] == PM_TSTAT6)
//			if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//				iti = newtstat6[208];
//			else
//				iti = multi_register_value[187];
//
//			if(iti==0)//ON/oFF
//			{
//				//if (newtstat6[7] == PM_TSTAT6)
//				if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//				{
//					//103	211	2	Full	W/R(write only when manual output7 enable)	Output7 Analog output2, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.    
//
//					if(m_OutValueCmbox.GetCurSel()==1)//ON
//					{
//						write_one(g_tstat_id,211,1000);
//						newtstat6[211] = 1000;
//					}
//					else
//					{
//						write_one(g_tstat_id,211,0);
//						newtstat6[211] =0;
//					}
//				}else
//				{
//
//					if(m_OutValueCmbox.GetCurSel()==1)//ON
//					{
//						write_one(g_tstat_id,103,1000);
//					}
//					else
//					{
//						write_one(g_tstat_id,103,0);
//					}
//				}
//
//			}
//		}
//	}
//
//	Fresh_Grid();
//	
//	EndWaitCursor();
//
//}
//310	254	1	Low byte	W/R	"Output auto/manual enable. Bit 0 to 4 correspond to output1 to output5, bit 5 correspond to 
//output6, bit 6 correspond to output7. 0, auto mode; 1, manual mode."
//Fan 改写的逻辑 也比较混乱，器件太多，又有很多特例，无法用 For 或Switch，只能替换简单的。;
void COutputSetDlg::OnCbnSelchangeOamcombo()
{
	BeginWaitCursor();
	int n;
	n = product_register_value[MODBUS_OUTPUT_MANU_ENABLE]; //310 254

	int i;
	int nchange=0;
	//first 3 rows
	if(m_nCurRow==1&&AM_OUTFIELD==m_nCurCol)
	{
		if(m_outAMCmbox.GetCurSel()==1)
		{
			i=1;
			nchange=n|i;	
		}
		else
		{
			i=0xfe;
			nchange=n & i;
		}
	}

	if(m_nCurRow==2&&AM_OUTFIELD==m_nCurCol)
	{
		if(m_outAMCmbox.GetCurSel()==1)
		{
			i=2;
			nchange=n|i;	
		}
		else
		{
			i=0xFD;;
			nchange=n & i;
		}
	}
	if(m_nCurRow==3&&AM_OUTFIELD==m_nCurCol)
	{
		if(m_outAMCmbox.GetCurSel()==1)
		{
			i=4;
			nchange=n|i;	
		}
		else
		{
			i=0xFB;;
			nchange=n & i;
		}
	}
	if(nchange>=0)
	{
		write_one(g_tstat_id,MODBUS_OUTPUT_MANU_ENABLE,nchange);	//310  254
		product_register_value[MODBUS_OUTPUT_MANU_ENABLE] = nchange;
	}
	//end of first 3 rows.

	//4,5
	if(m_nModeType==1||m_nModeType==4||m_nModeType==12||m_nModeType==16||m_nModeType==17
		||m_nModeType==18||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7||m_nModeType==PM_PRESSURE)
	{
		if(m_nCurRow==4&&AM_OUTFIELD==m_nCurCol)
		{
			if(m_outAMCmbox.GetCurSel()==1)
			{
				i=8;
				nchange=n|i;	
			}
			else
			{
				i=0xF7;;
				nchange=n & i;
			}
		}
		if(m_nCurRow==5&&AM_OUTFIELD==m_nCurCol)
		{
			if(m_outAMCmbox.GetCurSel()==1)
			{
				i=16;
				nchange=n|i;	
			}
			else
			{
				i=0xEF;;
				nchange=n & i;
			}
		}
		if(nchange>=0)
		{
			write_one(g_tstat_id,MODBUS_OUTPUT_MANU_ENABLE,nchange);	//310  254
			product_register_value[MODBUS_OUTPUT_MANU_ENABLE] = nchange	;	
		}
	}
	//A 4,5
	if(m_nModeType==2)
	{
		if(m_nCurRow==4&&AM_OUTFIELD==m_nCurCol)
		{
			if(m_outAMCmbox.GetCurSel()==1)
			{
				i=8;
				nchange=n|i;	
			}
			else
			{
				i=0xF7;
				nchange=n & i;
			}
		}
		if(m_nCurRow==5&&AM_OUTFIELD==m_nCurCol)
		{
			if(m_outAMCmbox.GetCurSel()==1)
			{
				i=16;
				nchange=n|i;	
			}
			else
			{
				i=0xEF;;
				nchange=n & i;
			}
		}
		if(nchange>=0)
		{
			write_one(g_tstat_id,MODBUS_OUTPUT_MANU_ENABLE,nchange);	//310  254
			product_register_value[MODBUS_OUTPUT_MANU_ENABLE] = nchange	;	
		}
		//		write_one(g_tstat_id,310,nchange);
	}

	//6.7
	if(m_nModeType==12||m_nModeType==16||m_nModeType==18
		||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7||m_nModeType==PM_PRESSURE)
	{
		if(m_nCurRow==6&&AM_OUTFIELD==m_nCurCol)
		{
			if(m_outAMCmbox.GetCurSel()==1)
			{
				i=32;
				nchange=n|i;	
			}
			else
			{
				i=0xDF;;
				nchange=n & i;
			}
		}
		if(m_nCurRow==7&&AM_OUTFIELD==m_nCurCol)
		{
			if(m_outAMCmbox.GetCurSel()==1)
			{
				i=64;
				nchange=n|i;	
			}
			else
			{
				i=0xBF;;
				nchange=n & i;
			}
		}
		if(nchange>=0)
		{
			write_one(g_tstat_id,MODBUS_OUTPUT_MANU_ENABLE,nchange);	//310  254
			product_register_value[MODBUS_OUTPUT_MANU_ENABLE] = nchange	;	
		}
	}

	Fresh_Grid();
	EndWaitCursor();
}


//void COutputSetDlg::OnCbnSelchangeOamcombo()
//{
//	BeginWaitCursor();
//	int n;
//	//if (newtstat6[7] == PM_TSTAT6)
//	if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//	{
//		//int temp = reg_tstat6[310];
//		// n=multi_register_value[temp];
//		n = newtstat6[254];
//
//	}else
//	{
//		 n=multi_register_value[310];
//	}
//	
//	int i;
//	int nchange=0;
//
//	//first 3 rows
//	if(m_nCurRow==1&&AM_OUTFIELD==m_nCurCol)
//	{
//		if(m_outAMCmbox.GetCurSel()==1)
//		{
//			i=1;
//			nchange=n|i;	
//		}
//		else
//		{
//			i=0xfe;
//			nchange=n & i;
//		}
//	}
//
//	if(m_nCurRow==2&&AM_OUTFIELD==m_nCurCol)
//	{
//		if(m_outAMCmbox.GetCurSel()==1)
//		{
//			i=2;
//			nchange=n|i;	
//		}
//		else
//		{
//			i=0xFD;;
//			nchange=n & i;
//		}
//	}
//	if(m_nCurRow==3&&AM_OUTFIELD==m_nCurCol)
//	{
//		if(m_outAMCmbox.GetCurSel()==1)
//		{
//			i=4;
//			nchange=n|i;	
//		}
//		else
//		{
//			i=0xFB;;
//			nchange=n & i;
//		}
//	}
//	if(nchange>=0)
//	{
////tstat6
//		//if (newtstat6[7] == PM_TSTAT6)
//		if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//		{
//			//int temp = reg_tststold[310];
//			write_one(g_tstat_id,254,nchange);
//			newtstat6[254] = nchange;
//		}else
////tstat6
//			write_one(g_tstat_id,310,nchange);
//
//	}
//	//end of first 3 rows.
//
//	//4,5
//	if(m_nModeType==1||m_nModeType==4||m_nModeType==12||m_nModeType==16||m_nModeType==17
//		||m_nModeType==18||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7||m_nModeType==PM_PRESSURE)
//	{
//		if(m_nCurRow==4&&AM_OUTFIELD==m_nCurCol)
//		{
//			if(m_outAMCmbox.GetCurSel()==1)
//			{
//				i=8;
//				nchange=n|i;	
//			}
//			else
//			{
//				i=0xF7;;
//				nchange=n & i;
//			}
//		}
//		if(m_nCurRow==5&&AM_OUTFIELD==m_nCurCol)
//		{
//			if(m_outAMCmbox.GetCurSel()==1)
//			{
//				i=16;
//				nchange=n|i;	
//			}
//			else
//			{
//				i=0xEF;;
//				nchange=n & i;
//			}
//		}
//		if(nchange>=0)
//		{
////tstat6
//			//if (newtstat6[7] == PM_TSTAT6)
//			if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//			{
//				//int temp = reg_tststold[310];
//				write_one(g_tstat_id,254,nchange);
//				newtstat6[254] = nchange;
//			}else
////tstat6
//			write_one(g_tstat_id,310,nchange);
//		}
//	}
//	//A 4,5
//	if(m_nModeType==2)
//	{
//		if(m_nCurRow==4&&AM_OUTFIELD==m_nCurCol)
//		{
//			if(m_outAMCmbox.GetCurSel()==1)
//			{
//				i=8;
//				nchange=n|i;	
//			}
//			else
//			{
//				i=0xF7;
//				nchange=n & i;
//			}
//		}
//		if(m_nCurRow==5&&AM_OUTFIELD==m_nCurCol)
//		{
//			if(m_outAMCmbox.GetCurSel()==1)
//			{
//				i=16;
//				nchange=n|i;	
//			}
//			else
//			{
//				i=0xEF;;
//				nchange=n & i;
//			}
//		}
//		if(nchange>=0)
//		{
//			//if (newtstat6[7] == PM_TSTAT6)
//			if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//			{
//				//int temp = reg_tststold[310];
//				write_one(g_tstat_id,254,nchange);
//				newtstat6[254] = nchange;
//			}else
//				//tstat6
//				write_one(g_tstat_id,310,nchange);
//		}
//	//		write_one(g_tstat_id,310,nchange);
//	}
//
//	//6.7
//	if(m_nModeType==12||m_nModeType==16||m_nModeType==18
//		||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7||m_nModeType==PM_PRESSURE)
//	{
//		if(m_nCurRow==6&&AM_OUTFIELD==m_nCurCol)
//		{
//			if(m_outAMCmbox.GetCurSel()==1)
//			{
//				i=32;
//				nchange=n|i;	
//			}
//			else
//			{
//				i=0xDF;;
//				nchange=n & i;
//			}
//		}
//		if(m_nCurRow==7&&AM_OUTFIELD==m_nCurCol)
//		{
//			if(m_outAMCmbox.GetCurSel()==1)
//			{
//				i=64;
//				nchange=n|i;	
//			}
//			else
//			{
//				i=0xBF;;
//				nchange=n & i;
//			}
//		}
//		if(nchange>=0)
//		{
//			//if (newtstat6[7] == PM_TSTAT6)
//			if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//			{
//				//int temp = reg_tststold[310];
//				write_one(g_tstat_id,254,nchange);
//				newtstat6[254] = nchange;
//			}else
//				//tstat6
//				write_one(g_tstat_id,310,nchange);
//		}
//		//	write_one(g_tstat_id,310,nchange);
//	}
//	
//	Fresh_Grid();
//	EndWaitCursor();
//}

void COutputSetDlg::OnCbnKillfocusOrangcombo()
{
	m_outRangCmbox.ShowWindow(SW_HIDE);
}


//Recode by Fan	2013 03 29
void COutputSetDlg::OnCbnSelchangeOrangcombo()
{

	//range
	// 	280	202	1	Low byte	W/R	Determine the output1 mode. Output1 always is ON/OFF mode
	// 	281	203	1	Low byte	W/R	Determine the output2 mode. Output2 always is ON/OFF mode
	// 	282	204	1	Low byte	W/R	Determine the output3 mode. Output3 always is ON/OFF mode
	// 	283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
	// 	284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM
	//	186	207	1	Low byte	W/R	Analog Output1 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
	//	187	208	1	Low byte	W/R	Analog Output2 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 


	//row4+1 	283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
	//row5+1 	284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM

	//row6+1	102	210	2	Full	W/R(write only when manual output6 enable)	Output6 ,Analog output1, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.
	//row7+1	103	211	2	Full	W/R(write only when manual output7 enable)	Output7 Analog output2, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.    



	int  ret =0;
	BeginWaitCursor();
	int nIndext=m_outRangCmbox.GetCurSel();


	//prior judge whether it is Type 2
	if(m_nModeType==2)//A
	{
		if(m_nCurRow>0&&m_nCurRow<=3)
		{	
			write_one(g_tstat_id,MODBUS_MODE_OUTPUT1+(m_nCurRow-1) ,nIndext );	//280
		}
		if(m_nCurRow==4)
			write_one(g_tstat_id,MODBUS_OUTPUT1_SCALE ,nIndext );//186
		if(m_nCurRow==5)
			write_one(g_tstat_id,MODBUS_OUTPUT2_SCALE ,nIndext );//187
		Fresh_Grid();
		EndWaitCursor();
		return;
	}



	if(m_nCurRow>0&&m_nCurRow<=5)
	{
		ret = write_one(g_tstat_id,MODBUS_MODE_OUTPUT1+(m_nCurRow-1) ,nIndext );	//280  202
		if (ret<=0)
		{
			AfxMessageBox(_T("Setting failure!"));
		}else
		{
			product_register_value[MODBUS_MODE_OUTPUT1+(m_nCurRow-1)] = nIndext;//280 202
		}
	}
	
	if(m_nCurRow==6)
	{
		ret =0;
		ret = write_one(g_tstat_id,MODBUS_OUTPUT1_SCALE ,nIndext ); //186   207
		if (ret<=0)
			AfxMessageBox(_T("Setting failure!"));
		else
			product_register_value[MODBUS_OUTPUT1_SCALE] = nIndext;
	}

	if(m_nCurRow==7)
	{
		ret =0;
		ret = write_one(g_tstat_id,MODBUS_OUTPUT2_SCALE ,nIndext ); //187   208
		if (ret<=0)
			AfxMessageBox(_T("Setting failure!"));
		else
			product_register_value[MODBUS_OUTPUT2_SCALE] = nIndext;
	}

	Fresh_Grid();
	EndWaitCursor();

}

//void COutputSetDlg::OnCbnSelchangeOrangcombo()
//{
//	/*
//	if(m_nModeType==2)
//	{
//		int nIndext=m_outRangCmbox.GetCurSel();
//		write_one(g_tstat_id,186 ,nIndext );
//		if (nIndext==2)
//		{
//			int	indext=3;
//			write_one(g_tstat_id,327,3);//timer on register
//		}
//	}else
//	{
//		short nIndext=m_outRangCmbox.GetCurSel();
//		write_one(g_tstat_id,283 ,nIndext);
//	}
//	*/
//	int  ret =0;
//	BeginWaitCursor();
//	int nIndext=m_outRangCmbox.GetCurSel();
//	if(m_nModeType==1||m_nModeType==4||m_nModeType==12||m_nModeType==16||m_nModeType==17
//		//||m_nModeType==18||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7||m_nModeType==PM_PRESSURE)//0914
//		||m_nModeType==18||m_nModeType==PM_PRESSURE)
//	{
//		if(m_nCurRow>0&&m_nCurRow<=5)
//		{	
//			ret = write_one(g_tstat_id,280+(m_nCurRow-1) ,nIndext );
//			if (ret<=0)
//			{
//				AfxMessageBox(_T("setting failure"));
//			}else
//			{
//				multi_register_value[280+(m_nCurRow-1)] = nIndext;
//			}
//		}
//		if(m_nModeType==12||m_nModeType==16||m_nModeType==18||m_nModeType==PM_TSTAT6
//			||m_nModeType==PM_TSTAT7||m_nModeType==PM_PRESSURE) //
//		{
//			if(m_nCurRow==6)
//			{
//				ret =0;
//				ret = write_one(g_tstat_id,186 ,nIndext );
//			if (ret<=0)
//			{
//				AfxMessageBox(_T("setting failure"));
//			}else
//			{
//				multi_register_value[186] = nIndext;
//			}
//			}
//			if(m_nCurRow==7)
//			{
//				ret =0;
//				ret = write_one(g_tstat_id,187 ,nIndext );
//				if (ret<=0)
//				{
//					AfxMessageBox(_T("setting failure"));
//				}else
//				{
//					multi_register_value[187] = nIndext;
//				}
//			}
//				
//		}
//
//	}
//	if(m_nModeType==2)//A
//	{
//		if(m_nCurRow>0&&m_nCurRow<=3)
//		{	
//			write_one(g_tstat_id,280+(m_nCurRow-1) ,nIndext );
//		}
//		if(m_nCurRow==4)
//			write_one(g_tstat_id,186 ,nIndext );
//		if(m_nCurRow==5)
//			write_one(g_tstat_id,187 ,nIndext );
//	}
//
////以下//0914
////range
//// 	280	202	1	Low byte	W/R	Determine the output1 mode. Output1 always is ON/OFF mode
//// 	281	203	1	Low byte	W/R	Determine the output2 mode. Output2 always is ON/OFF mode
//// 	282	204	1	Low byte	W/R	Determine the output3 mode. Output3 always is ON/OFF mode
//// 	283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
//// 	284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM
////	186	207	1	Low byte	W/R	Analog Output1 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
////	187	208	1	Low byte	W/R	Analog Output2 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
//
//
////row4+1 	283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
////row5+1 	284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM
//
////row6+1	102	210	2	Full	W/R(write only when manual output6 enable)	Output6 ,Analog output1, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.
////row7+1	103	211	2	Full	W/R(write only when manual output7 enable)	Output7 Analog output2, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.    
//
//
//	 ret =0;
//	if ((newtstat6[7] == PM_TSTAT6)||(newtstat6[7] == PM_TSTAT7))
//	{
//		if(m_nCurRow>0&&m_nCurRow<=5)
//		{	
//			ret = write_one(g_tstat_id,202+(m_nCurRow-1) ,nIndext );
//			if (ret>0)
//				newtstat6[202+m_nCurRow-1] = nIndext;
//		}
//
//		if(m_nCurRow==6)
//		{
//			ret = 0;
//			ret = write_one(g_tstat_id,207 ,nIndext );
//			if (ret>0)
//				newtstat6[207] = nIndext;
//
//		}
//		if(m_nCurRow==7)
//		{
//			ret = 0;
//			ret = write_one(g_tstat_id,208 ,nIndext );
//			if (ret>0)
//				newtstat6[207] = nIndext;
//		}
//
//	}
//	Fresh_Grid();
//	EndWaitCursor();
//
//}

void COutputSetDlg::OnCbnKillfocusOfuncombo()
{
	m_outFunCmbox.ShowWindow(SW_HIDE);
}

void COutputSetDlg::OnCbnSelchangeOfuncombo()
{
	int retfun =0;
	int nItem=-1;
	nItem=m_outFunCmbox.GetCurSel();
	short nFun;//=multi_register_value[328];

	//328	266	1	Low byte	W/R	"Output1 Function setting:
	//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "


// 
//		280	1	Determine the output1 mode. Output1 always is ON/OFF mode
// 		281	1	Determine the output2 mode. Output2 always is ON/OFF mode
// 		282	1	Determine the output3 mode. Output3 always is ON/OFF mode
// 		283	1	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
// 		284	1	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM

	if (newtstat6[7]==6)
	{
		 nFun=newtstat6[266];
	}else
	{
		nFun=multi_register_value[328];
	}
	if(m_nCurRow==1)
	{		
		//if (newtstat6[7] == 6)
		if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
		{
			write_one(g_tstat_id,266,nItem);
			newtstat6[266] = nItem;
		}else
		{
// 			if (multi_register_value[7] == 18)//2.5.0.96
// 			{
// 				retfun = write_one(g_tstat_id,280,nItem);
// 				if (retfun<=0)
// 					AfxMessageBox(_T("setting failure!"));
// 			}else
			{
				if(m_version<32.2)
					return;
				retfun = write_one(g_tstat_id,328,nItem);
				if (retfun<=0)
				{
					AfxMessageBox(_T("setting failure!"));
				}else
				{
					if ((nItem == 3)&&(multi_register_value[7] == 18))
					{
						retfun = 0;
						retfun = write_one(g_tstat_id,280,2);
						if (retfun<=0)
							AfxMessageBox(_T("setting failure!"));

					}
				}


			}

		}
	}	
	if(m_nCurRow==2)
	{
		if(m_version<32.2)
		{
	
			if (nItem==0)
			{
				nFun=nFun&0xFE;
			}
			if (nItem==1)
			{
				nFun=nFun|1;
			}

			//if (newtstat6[7] == 6)
			if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
			{
				write_one(g_tstat_id,266,nFun);
				newtstat6[266] = nFun;

			}else
			{
				 write_one(g_tstat_id,328,nFun);
	
			}
	
		}//334	267	1	Low byte	W/R	Output2 function setting (see above)

		else
		{
			//if (newtstat6[7] == 6)
			if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
			{
				write_one(g_tstat_id,267,nItem);
				newtstat6[267] = nItem;

			}else
			{
// 				if (multi_register_value[7] == 18)//2.5.0.96
// 				{
// 					retfun = write_one(g_tstat_id,281,nItem);
// 					if (retfun<=0)
// 						AfxMessageBox(_T("setting failure!"));
// 				}else
				//{
				retfun = 0;
					retfun = write_one(g_tstat_id,334,nItem);
				//}

					if (retfun<=0)
					{
						AfxMessageBox(_T("setting failure!"));
					}else
					{
						if ((nItem == 3)&&(multi_register_value[7] == 18))
						{
							retfun = 0;
							retfun = write_one(g_tstat_id,281,2);
							if (retfun<=0)
								AfxMessageBox(_T("setting failure!"));

						}
					}

			
			}
			
		}

	}

	if(m_nCurRow==3)
	{
		if(m_version<32.2)
		{
			if (nItem==0)
			{
				nFun=nFun&0xFD;
			}
			if (nItem==1)
			{
				nFun=nFun|2;
			}
			//if (newtstat6[7] == 6)
			if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
			{
				write_one(g_tstat_id,266,nFun);
				newtstat6[266] = nFun;

			}else
			{
				write_one(g_tstat_id,328,nFun);
			}

			
		}
		else
		{
			
			//335	268	1	Low byte	W/R	Output3 function setting (see above)

			//if (newtstat6[7] == 6)
			if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
			{
				write_one(g_tstat_id,268,nItem);
				newtstat6[268] = nItem;

			}else
			{
// 				if (multi_register_value[7] == 18)
// 				{
// 					retfun = write_one(g_tstat_id,282,nItem);
// 					if (retfun<=0)
// 						AfxMessageBox(_T("setting failure!"));
// 				}else
// 				{
				retfun = 0;
				retfun = write_one(g_tstat_id,335,nItem);
				if (retfun<=0)
				{
					AfxMessageBox(_T("setting failure!"));
				}else
				{
					if ((nItem == 3)&&(multi_register_value[7] == 18))
					{
						retfun = 0;
						retfun = write_one(g_tstat_id,282,2);
						if (retfun<=0)
							AfxMessageBox(_T("setting failure!"));

					}
				}
			
	//			}
				
			}
		}
	}
	if(m_nCurRow==4)
	{
		if(m_version<32.2)
		{
			if (nItem==0)
			{
				nFun=nFun&0xFB;
			}
			if (nItem==1)
			{
				nFun=nFun|0x04;
			}
			//if (newtstat6[7] == 6)
			if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
			{
				write_one(g_tstat_id,266,nFun);
				newtstat6[266] = nFun;
			}else
			{
				write_one(g_tstat_id,328,nFun);
			}
			
		}//336	269	1	Low byte	W/R	Output4 function setting (see above)

		else
		{

			//if (newtstat6[7] == 6)
			if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
			{
				write_one(g_tstat_id,269,nItem);
				newtstat6[269] = nItem;
			}else
			{
// 				if (multi_register_value[7] == 18)//2.5.0.96
// 				{
					retfun = 0;
					if (nItem == 3)
					{
						nItem = 2;
					}
					retfun = write_one(g_tstat_id,283,nItem);
					if (retfun<=0)
						AfxMessageBox(_T("setting failure!"));
			//	}
#if 0//2.5.0.98
// 				if (multi_register_value[7] == 18)//2.5.0.96
// 				{
// 					retfun = write_one(g_tstat_id,283,nItem);
// 					if (retfun<=0)
// 						AfxMessageBox(_T("setting failure!"));
// 				}else
// 				{
				retfun = 0;
				retfun = write_one(g_tstat_id,336,nItem);
	//			}

				if (retfun<=0)
				{
					AfxMessageBox(_T("setting failure!"));
				}else
				{
					if ((nItem == 3)&&(multi_register_value[7] == 18))
					{
						retfun = 0;
						retfun = write_one(g_tstat_id,283,2);
						if (retfun<=0)
							AfxMessageBox(_T("setting failure!"));

					}
				}
	#endif			
			}

			
		}

	}
	if(m_nCurRow==5)
	{
		if(m_version<32.2)
		{
			if (nItem==0)
			{
				nFun=nFun&0xF7;
			}
			if (nItem==1)
			{
				nFun=nFun|8;
			}
			//if (newtstat6[7] == 6)
			if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
			{
				write_one(g_tstat_id,266,nFun);
				newtstat6[266] = nFun;
			}else
			{
				write_one(g_tstat_id,328,nFun);

			}
			
		}
		else
		{//337	270	1	Low byte	W/R	Output5 function setting (see above)

			//if (newtstat6[7] ==6)
			if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
			{
				write_one(g_tstat_id,270,nItem);
				newtstat6[270] = nItem;
			}else
			{
				retfun = 0;
				if (nItem == 3)
				{
					nItem = 2;
				}
				retfun = write_one(g_tstat_id,284,nItem);
				if (retfun<=0)
					AfxMessageBox(_T("setting failure!"));

#if 0//2.5.0.98
// 				if (multi_register_value[7] == 18)//2.5.0.96
// 				{
// 					retfun = write_one(g_tstat_id,284,nItem);
// 					if (retfun<=0)
// 						AfxMessageBox(_T("setting failure!"));
// 				}else
// 				{
				retfun = 0;
				retfun = write_one(g_tstat_id,337,nItem);
//				}

				if (retfun<=0)
				{
					AfxMessageBox(_T("setting failure!"));
				}else
				{
					if ((nItem == 3)&&(multi_register_value[7] == 18))
					{
						retfun = 0;
						retfun = write_one(g_tstat_id,281,2);
						if (retfun<=0)
							AfxMessageBox(_T("setting failure!"));

					}
				}
				
#endif
			}

			
		}
	}

	Fresh_Grid();
	
}

void COutputSetDlg::OnEnKillfocusValueedit()
{
	BeginWaitCursor();
	if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7))
	{
		CString strText;
		m_OutValueEdt.GetWindowText(strText);
		if(strText.IsEmpty())
			return;
		int nValue=_wtoi(strText);
		// added by zgq; 2010-12-06; 检测输入，必须在0－100之间
		if( nValue > 100 || nValue < 0)
		{
			AfxMessageBox(_T("Please enter a value between 0 and 100!"), MB_ICONWARNING);
			m_OutValueEdt.SetFocus();
			return;
		}
		int nSrcValue = nValue;
		nValue=nValue*100; 


		//{
		//  commented by zgq;2010-12-06; 写入的值应当为实际设置电压值×100。
		//  但界面输入的值为range值中标定范围的百分比。例如，输入50，range为0－10v(100%)，
		//   则实际电压为10×50% = 5V。写入为5×100 = 500。
		if((m_nCurRow==4)||(m_nCurRow == 5))
		{	
			//	write_one(g_tstat_id,348,nSrcValue);
			int ret = 0;
			ret = write_one(g_tstat_id,MODBUS_VALVE_PERCENT,nSrcValue);//285   244
			if(ret>0)
				product_register_value[MODBUS_VALVE_PERCENT] = nSrcValue;
		}


		//186	207	1	Low byte	W/R	Analog Output1 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
		//187	208	1	Low byte	W/R	Analog Output2 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 

		if(m_nCurRow==6)
		{			
			//int nRange = multi_register_value[186];
			int nRange = product_register_value[MODBUS_OUTPUT1_SCALE];	// 186   207
			int nValueW = 0;
			switch(nRange)
			{
			case 1: //0-10v
				//nValueW = (nSrcValue * 1.0) /100.0 * 10 * 100; // 这是原始的等式，简化一下
				nValueW = nSrcValue * (10-0);
				break;

			case 2: //0-5v
				nValueW = nSrcValue * (5-0);
				break;

			case 3: //2-10v
				nValueW = nSrcValue * (10-2) + 2;
				break;

			case 4: //10-0v
				nValueW = nSrcValue * (0-10);
				break;

			default:
				break;			
			}
			//102	210	2	Full	W/R(write only when manual output6 enable)	Output6 ,Analog output1, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.

			int rt = 0;


			rt = write_one(g_tstat_id,MODBUS_COOLING_VALVE,nValueW);//102 210
			if (rt>0)
			{
				product_register_value[MODBUS_COOLING_VALVE] = nValueW;
			}
		}
		if(m_nCurRow==7)
		{		
			int nRange = product_register_value[MODBUS_OUTPUT2_SCALE]; //208 Fance_?_  这里应该是208吧
	//		int nRange = newtstat6[207];
			int nValueW = 0;
			switch(nRange)
			{
			case 1: //0-10v
				//nValueW = (nSrcValue * 1.0) /100.0 * 10 * 100; // 这是原始的等式，简化一下
				nValueW = nSrcValue * (10-0);
				break;

			case 2: //0-5v
				nValueW = nSrcValue * (5-0);
				break;

			case 3: //2-10v
				nValueW = nSrcValue * (10-2) + 2;
				break;

			case 4: //10-0v
				nValueW = nSrcValue * (0-10);
				break;

			default:
				break;			
			}
			int rt2 = 0;

			//103	211	2	Full	W/R(write only when manual output7 enable)	Output7 Analog output2, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.    

			//write_one(g_tstat_id,103,nValueW);	
			rt2 = write_one(g_tstat_id,MODBUS_HEATING_VALVE,nValueW);//103 211
			if (rt2>0)
			{
				product_register_value[MODBUS_HEATING_VALVE] = nValueW;
			}
		}
	}
	else
	{
		CString strText;
		m_OutValueEdt.GetWindowText(strText);
		if(strText.IsEmpty())
			return;
		int nValue=_wtoi(strText);
		// added by zgq; 2010-12-06; 检测输入，必须在0－100之间
		if( nValue > 100 || nValue < 0)
		{
			AfxMessageBox(_T("Please enter a value between 0 and 100!"), MB_ICONWARNING);
			m_OutValueEdt.SetFocus();
			return;
		}
		int nSrcValue = nValue;
		nValue=nValue*100; 

		if(m_nModeType==2)
		{
			if(m_nCurRow==4)
			{
				write_one(g_tstat_id,102,nValue);
			}
			if(m_nCurRow==5)
			{
				write_one(g_tstat_id,103,nValue);
			}
		}
		//if(m_nModeType==17||m_nModeType==18)//17//18
		if(m_nModeType==17)//17//18 2.5.0.94
		{
			if(m_nCurRow==4/*&&multi_register_value[283]!=0*/)//ON/oFF
			{
				write_one(g_tstat_id,MODBUS_PWM_OUT4,nValue);	//348
			}
			if(m_nCurRow==5)//ON/oFF
			{
				write_one(g_tstat_id,MODBUS_PWM_OUT5,nValue);//349
			}

		}
		if(m_nModeType==12||(m_nModeType==16)||(m_nModeType==18)||m_nModeType==PM_TSTAT6
			||m_nModeType==PM_TSTAT7||m_nModeType==PM_PRESSURE)
		{
			//  commented by zgq;2010-12-06; 写入的值应当为实际设置电压值×100。
			//  但界面输入的值为range值中标定范围的百分比。例如，输入50，range为0－10v(100%)，
			//   则实际电压为10×50% = 5V。写入为5×100 = 500。
			if(m_nCurRow==4)
			{	
				write_one(g_tstat_id,MODBUS_PWM_OUT4,nSrcValue);//348
			}
			if(m_nCurRow==5)
			{			
				write_one(g_tstat_id,MODBUS_PWM_OUT5,nSrcValue);//349
			}

			if(m_nCurRow==6)
			{			
				int nRange = product_register_value[MODBUS_OUTPUT1_SCALE];//186
				int nValueW = 0;
				switch(nRange)
				{
				case 1: //0-10v
					//nValueW = (nSrcValue * 1.0) /100.0 * 10 * 100; // 这是原始的等式，简化一下
					nValueW = nSrcValue * (10-0);
					break;

				case 2: //0-5v
					nValueW = nSrcValue * (5-0);
					break;

				case 3: //2-10v
					nValueW = nSrcValue * (10-2) + 2;
					break;

				case 4: //10-0v
					nValueW = nSrcValue * (0-10);
					break;

				default:
					break;			
				}
				write_one(g_tstat_id,102,nValueW);
			}
			if(m_nCurRow==7)
			{		
				int nRange = product_register_value[MODBUS_OUTPUT2_SCALE];// Fance_?_ 这里应该是187吧   第6已经是186了，难道一样？？
				//int nRange = multi_register_value[186]; Annul by Fance
				int nValueW = 0;
				switch(nRange)
				{
				case 1: //0-10v
					//nValueW = (nSrcValue * 1.0) /100.0 * 10 * 100; // 这是原始的等式，简化一下
					nValueW = nSrcValue * (10-0);
					break;

				case 2: //0-5v
					nValueW = nSrcValue * (5-0);
					break;

				case 3: //2-10v
					nValueW = nSrcValue * (10-2) + 2;
					break;

				case 4: //10-0v
					nValueW = nSrcValue * (0-10);
					break;

				default:
					break;			
				}
				write_one(g_tstat_id,MODBUS_HEATING_VALVE,nValueW);//103
			}
		}

	}
	m_OutValueEdt.ShowWindow(SW_HIDE);
	Fresh_Grid();
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
	GetDlgItem(IDC_REFRESHBUTTON)->EnableWindow(FALSE);
	GetDlgItem(ID_EXIT)->EnableWindow(FALSE);
	if(OutputThread==NULL)
	OutputThread = CreateThread(NULL,NULL,StartRefresh,this,NULL,NULL);

	
}

LRESULT  COutputSetDlg::RefreshGrid(WPARAM wParam,LPARAM lParam)
{
	int command = wParam;
	switch(command)
	{
	case REFRESH_GRID:
		Fresh_Grid();
		break;
	case ENABLE_REFRESH_BUTTON:
		GetDlgItem(IDC_REFRESHBUTTON)->EnableWindow(TRUE);
		GetDlgItem(ID_EXIT)->EnableWindow(TRUE);
		break;
	default:
		break;
	}

	return 0;
}


//Add by Fan  2013 03 29
DWORD WINAPI COutputSetDlg::StartRefresh(LPVOID lpVoid)
{
	COutputSetDlg *pParent = (COutputSetDlg *)lpVoid;
	int i;
	register_critical_section.Lock();
	pParent->b_is_fresh = true;
	for(i=0;i<16;i++)
	{	
		Read_Multi(g_tstat_id,&multi_register_value[i*64],i*64,64);	
	}
	register_critical_section.Unlock();
	memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));
	pParent->PostMessage(WM_REFRESH_OUTPUTDLG,REFRESH_GRID,0);
	pParent->PostMessage(WM_REFRESH_OUTPUTDLG,ENABLE_REFRESH_BUTTON,0);
	//pParent->Fresh_Grid();
	pParent->OutputThread=NULL;
	pParent->b_is_fresh = false;
	return 0;
}

void COutputSetDlg::OnEnKillfocusOutputnameedit()
{
	CString strText;
	int lRow;
	int lCol; 
	m_outputNameEDIT.GetWindowText(strText);
	m_outputNameEDIT.ShowWindow(SW_HIDE);
	 lRow = m_FlexGrid.get_RowSel();//获取点击的行号	
	lCol = m_FlexGrid.get_ColSel(); //获取点击的列号
	



			if (lCol==NAME_OUTFIELD)
			{
			#if 1
			   try
	{
	

	CString strInName;
	if(lCol!=NAME_OUTFIELD||lRow==0)
		return;
	switch (lRow)
	{
		case 1:
			strInName=g_strOutName1;
			break;
		case 2:
			strInName=g_strOutName2;
			break;
		case 3:
			strInName=g_strOutName3;
			break;
		case 4:
			strInName=g_strOutName4;
			break;
		case 5:
			strInName=g_strOutName5;
			break;
		case 6:
			strInName=g_strOutName6;
			break;
		case 7:
			strInName=g_strOutName7;
			break;
	}
	if(strText.Compare(strInName)==0)
		return;
		

	//if(g_serialNum>0&&multi_register_value[6]>0)
	if(multi_register_value[6]>0)
	{
		_ConnectionPtr m_ConTmp;
		_RecordsetPtr m_RsTmp;
		m_ConTmp.CreateInstance("ADODB.Connection");
		m_RsTmp.CreateInstance("ADODB.Recordset");
		m_ConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);

		CString strSerial;
		strSerial.Format(_T("%d"),g_serialNum);

		CString strsql;
		strsql.Format(_T("select * from IONAME where SERIAL_ID = '%s'"),strSerial);
		m_RsTmp->Open((_variant_t)strsql,_variant_t((IDispatch *)m_ConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
		if(VARIANT_FALSE==m_RsTmp->EndOfFile)//update
		{
			
			CString strField;
			switch (lRow)
			{
				case 1:
					strField=_T("OUTPUT1");
					break;
				case 2:
					strField=_T("OUTPUT2");
					break;
				case 3:
					strField=_T("OUTPUT3");
					break;
				case 4:
					strField=_T("OUTPUT4");
					break;
				case 5:
					strField=_T("OUTPUT5");
					break;
				case 6:
					strField=_T("OUTPUT6");
					break;
				case 7:
					strField=_T("OUTPUT7");
					break;
			}

			try
			{

			CString str_temp;
			str_temp.Format(_T("update IONAME set "+strField+" = '"+strText+"' where SERIAL_ID = '"+strSerial+"'"));
			//AfxMessageBox(str_temp );
			m_ConTmp->Execute(str_temp.GetString(),NULL,adCmdText);
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
			m_FlexGrid.put_TextMatrix(lRow,lCol,strText);

		}
		else//inerst
		{
			switch (lRow)
			{
				case 1:
					g_strOutName1=strText;
					break;
				case 2:
					g_strOutName2=strText;
					break;
				case 3:
					g_strOutName3=strText;
					break;
				case 4:
					g_strOutName4=strText;
					break;
				case 5:
					g_strOutName5=strText;
					break;
				case 6:
					g_strOutName6=strText;
					break;
				case 7:
					g_strOutName7=strText;
					break;
			}

			CString	str_temp;
			str_temp.Format(_T("insert into IONAME values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')"),
				strSerial,
				g_strInName1,
				g_strInName2,
				g_strInName3,
				g_strInName4,
				g_strInName5,
				g_strInName6,
				g_strInName7,
				g_strOutName1,
				g_strOutName2,
				g_strOutName3,
				g_strOutName4,
				g_strOutName5,
				g_strOutName6,
				g_strOutName7,
				g_strInName8,
				g_strInHumName,
				g_strSensorName
				);
			try
			{

				m_ConTmp->Execute(str_temp.GetString(),NULL,adCmdText);
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
			m_FlexGrid.put_TextMatrix(lRow,lCol,strText);
		}

		switch (lRow)
		{
			case 1:
				g_strOutName1=strText;
				break;
			case 2:
				g_strOutName2=strText;
				break;
			case 3:
				g_strOutName3=strText;
				break;
			case 4:
				g_strOutName4=strText;
				break;
			case 5:
				g_strOutName5=strText;
				break;
			case 6:
				g_strOutName6=strText;
				break;
			case 7:
				g_strOutName7=strText;
				break;
		}
	
		if(m_RsTmp->State) 
			m_RsTmp->Close(); 
		if(m_ConTmp->State)
			m_ConTmp->Close();	
	}
	
			}
			catch (...)
			{
				CString str;
				str.Format(_T("edit output name"));
				SetPaneString(2,str);
				m_FlexGrid.put_TextMatrix(lRow,lCol,strText);

			}
#endif
			} 
			else if (lCol==7)
			{
			int onoff=_wtoi(strText);
			int stradd=MODBUS_OUTPUT1_DELAY_OFF_TO_ON+lRow-1;	 //213
			if (product_register_value[stradd]==onoff)
			{
			return;
			}
			int ret=write_one(g_tstat_id,stradd,onoff,5);//[stradd];
			  if (ret>0)
			  {
			  product_register_value[stradd]=onoff;
			  } 
			  else
			  {
			  AfxMessageBox(_T("Try again"));
			  }
			}
			else if (lCol==8)
			{
				int onoff=_wtoi(strText);
				int stradd=MODBUS_OUTPUT1_DELAY_ON_TO_OFF+lRow-1;	 //227
				if (product_register_value[stradd]==onoff)
				{
					return;
				}
				int ret=write_one(g_tstat_id,stradd,onoff,5);//[stradd];
				if (ret>0)
				{
					product_register_value[stradd]=onoff;
				} 
				else
				{
					AfxMessageBox(_T("Try again"));
				}
			}
			else
			{
			}

			Fresh_Grid();

}



void COutputSetDlg::OnCbnSelchangeCombolock()
{

	// 	    286	245	1	Low byte	W/R	Interlock for  output1	MODBUS_INTERLOCK_OUTPUT1
	// 		287	246	1	Low byte	W/R	Interlock for  output2	MODBUS_INTERLOCK_OUTPUT2
	// 		288	247	1	Low byte	W/R	Interlock for  output3	MODBUS_INTERLOCK_OUTPUT3
	// 		289	248	1	Low byte	W/R	Interlock for  output4	MODBUS_INTERLOCK_OUTPUT4
	// 		290	249	1	Low byte	W/R	Interlock for  output5	MODBUS_INTERLOCK_OUTPUT5
	// 		291	250	1	Low byte	W/R	Interlock for  output6	MODBUS_INTERLOCK_OUTPUT6
	// 		292	251	1	Low byte	W/R	Interlock for  output7	MODBUS_INTERLOCK_OUTPUT7

	int nItem=-1;
	nItem=m_Interlockcombo.GetCurSel();
	int startadd;

	switch(m_nCurRow)
	{
	case 1:
		startadd = MODBUS_INTERLOCK_OUTPUT1;  //245 286
		break;
	case 2:
		startadd = MODBUS_INTERLOCK_OUTPUT2;
		break;
	case 3:
		startadd = MODBUS_INTERLOCK_OUTPUT3;
		break;
	case 4:
		startadd = MODBUS_INTERLOCK_OUTPUT4;
		break;
	case 5:
		startadd = MODBUS_INTERLOCK_OUTPUT5;
		break;
	case 6:
		startadd = MODBUS_INTERLOCK_OUTPUT6;
		break;
	case 7:
		startadd = MODBUS_INTERLOCK_OUTPUT7;
		break;
	}

	int ret = 0;
	ret = write_one(g_tstat_id,startadd,nItem);
	if(ret>0)
	{
		product_register_value[startadd] = nItem;
		Fresh_Grid();
	}
}


//由于要兼容多个版本，由Fan Du  recoding
//void COutputSetDlg::OnCbnSelchangeCombolock()
//{
//
//// 	    286	245	1	Low byte	W/R	Interlock for  output1
//// 		287	246	1	Low byte	W/R	Interlock for  output2
//// 		288	247	1	Low byte	W/R	Interlock for  output3
//// 		289	248	1	Low byte	W/R	Interlock for  output4
//// 		290	249	1	Low byte	W/R	Interlock for  output5
//// 		291	250	1	Low byte	W/R	Interlock for  output6
//// 		292	251	1	Low byte	W/R	Interlock for  output7
//
//
//
//	int nItem=-1;
//	nItem=m_Interlockcombo.GetCurSel();
//	int startadd;
////if (newtstat6[7] == 6)
//	if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
//{
//	switch(m_nCurRow)
//	{
//	case 1:
//		startadd = 245;
//		break;
//	case 2:
//		startadd = 246;
//		break;
//	case 3:
//		startadd = 247;
//		break;
//	case 4:
//		startadd = 248;
//		break;
//	case 5:
//		startadd = 249;
//		break;
//	case 6:
//		startadd = 250;
//		break;
//	case 7:
//		startadd = 251;
//		break;
//	}
//
//	int ret = 0;
//	ret = write_one(g_tstat_id,startadd,nItem);
//	if(ret>0)
//	{
//		newtstat6[startadd] = nItem;
//		Fresh_Grid();
//	}
//}else
//{
//	switch(m_nCurRow)
//	{
//	case 1:
//		startadd = 286;
//		break;
//	case 2:
//		startadd = 287;
//		break;
//	case 3:
//		startadd = 288;
//		break;
//	case 4:
//		startadd = 289;
//		break;
//	case 5:
//		startadd = 290;
//		break;
//	case 6:
//		startadd = 291;
//		break;
//	case 7:
//		startadd = 292;
//		break;
//	}
//
//	int ret = 0;
//	ret = write_one(g_tstat_id,startadd,nItem);
//	if(ret>0)
//		Fresh_Grid();
//}
//
//}

void COutputSetDlg::OnCbnKillfocusCombolock()
{
	m_Interlockcombo.ShowWindow(SW_HIDE);
}


void COutputSetDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if(b_is_fresh==true)
	{
		AfxMessageBox(_T("It's refreshing ,can not exit at this time!"));
		return;
	}

	CDialog::OnClose();
}
