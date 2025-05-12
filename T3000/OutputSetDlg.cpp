// OutputSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "OutputSetDlg.h"
#include "global_function.h"
#include "../SQLiteDriver/CppSQLite3.h" 
 
#define INDEX_OUTFIELD 0
#define NAME_OUTFIELD 1
#define VALUE_OUTFIELD 2
#define AM_OUTFIELD 3
#define RANG_OUTFIELD 4
#define FUN_OUTFIELD 5
#define INTER_LOCK 6
#define DELAY_OFFON 7
#define DELAY_ONOFF 8
#define SIGUAL_TYPE 9


#define TOTAL_OUTCOLS 6
#define TOTAL_OUTROWS 8

// CString Interlock[6]={_T("ON"),_T("DI1"),_T("AI1") ,_T("AI2"),_T("TIMER OR"),_T("TIMER AND")};
// CString ONTPUT_FUNS[]={_T("Normal"),_T("Rotation Timer"),_T("Lighting Control") ,_T("VAV Control"),_T("Transducer")};
// CString OUTPUT_ANRANGE[]={_T("On/Off"),_T("0-10V(100%)"),_T("0-5V(100%)"),_T("2-10V(100%)"),_T("10-0V(100%)"),_T("Internal Sensor"),_T("Setpoint"),_T("AI1"),_T("AI2"),_T("AI3"),_T("AI4"),_T("AI5"),_T("AI6"),_T("AI7"),_T("AI8"),_T("Hum Sensor"),_T("CO2 Sensor")};
// CString OUTPUT_RANGE5[3]={_T("On/Off"),_T("Float(0-100%)"),_T("PWM(0-100%)")};
// CString OUTPUT_RANGE45[7]={_T("On/Off"),_T("Float(Cooling)"),_T("Float(Heating)"),_T("PWM(0-100%)"),_T("Off/On"),_T("Open/Close"),_T("Close/Open")};//2.5.0.98
// CString OUTPUT_ANRANGE6[]={_T("On/Off"),_T("0-10V(100%)"),_T("0-5V(100%)"),_T("2-10V(100%)"),_T("PWM(0-100%)")};
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
	DDX_Control(pDX, IDC_COMBO_SIGNAL_TYPE, m_combox_signal_type);
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
	ON_CBN_SELCHANGE(IDC_COMBO_SIGNAL_TYPE, &COutputSetDlg::OnCbnSelchangeComboSignalType)
END_MESSAGE_MAP()


// COutputSetDlg message handlers

void COutputSetDlg::OnBnClickedOk()
{
	
	OnOK();
}

BOOL COutputSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_sn=product_register_value[0]+product_register_value[1]*256+product_register_value[2]*256*256+product_register_value[3]*256*256*256;

	m_outputNameEDIT.ShowWindow(FALSE);
	m_OutValueEdt.ShowWindow(SW_HIDE);
	m_OutValueCmbox.ShowWindow(SW_HIDE);
	m_outAMCmbox.ShowWindow(SW_HIDE);
	m_outRangCmbox.ShowWindow(SW_HIDE);
	m_outFunCmbox.ShowWindow(SW_HIDE);
	m_Interlockcombo.ShowWindow(SW_HIDE);//tsta5
	//m_FlexGrid.put_Cols(TOTAL_OUTCOLS);//tsta5
	m_FlexGrid.put_Cols(9);	//原来是7 后来加了ON/OFF Time Set,ON/OFFTime left,OFF/ON Time Set OFF/ON Time left 
	if (product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT5i||product_register_value[7]==PM_TSTAT7||(product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
	{
		m_FlexGrid.put_Cols(10);
		m_FlexGrid.put_TextMatrix(0,SIGUAL_TYPE,_T("Signal Type"));
		m_FlexGrid.put_ColWidth(SIGUAL_TYPE,1000);
	}
	//加了一行 signal type
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

	m_FlexGrid.put_TextMatrix(0,INTER_LOCK,_T("Interlock"));//tstat5
	m_FlexGrid.put_ColWidth(6,1200);//tstat5

	m_FlexGrid.put_TextMatrix(0,DELAY_OFFON,_T("OFF->ON Delay"));//tstat5
	m_FlexGrid.put_ColWidth(7,1500);//tstat5
	m_FlexGrid.put_TextMatrix(0,DELAY_ONOFF,_T("ON->OFF Delay"));//tstat5
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
	
	OnCancel();
}



void COutputSetDlg::Fresh_Grid()
{
	m_nModeType = product_register_value[7];
	/*if (m_nModeType==16)
	{
		Fresh_Grid_Tstat5E();
		return;
	}
	if (m_nModeType==6||m_nModeType==7)
	{
		Fresh_GridForTstat6();
		return;
	}*/
	switch (m_nModeType)
	{
	case 2:
	case 1:	m_outRows=6;break;
	case 4:	m_outRows=6;break;
	case PM_PRESSURE: break;
	case PM_TSTAT5D:m_outRows=8;break;
	case PM_TSTAT6:
    case PM_TSTAT5i:
    case PM_TSTAT8:
    case PM_TSTAT9:
	case PM_TSTAT7:
	case PM_TSTAT8_WIFI:
	case PM_TSTAT8_OCC:
	case PM_TSTAT7_ARM:
	case PM_TSTAT8_220V:
	case 16:m_outRows=8;break;

    case PM_PM5E:m_outRows=8;break;
    case PM_PM5E_ARM:m_outRows = 8;break;
	case 17:m_outRows=6;break;
	case 18:m_outRows=8;break;
	case 19:m_outRows=8;break;
	default:break;
	}
	CString strTemp;
	//row1:
	int nVAlue;int nValue=0;
	//这段代码的作用是当选择的是Floating或者PWM模式的时候，
	//要根据在outputtable中 用户选择的是PID1，还是PID2 PID1 从384 读值 PID2 就是从389读值
	/////前三行
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

	int nAMVAlue=0;//=product_register_value[310];
	nAMVAlue = product_register_value[MODBUS_OUTPUT_MANU_ENABLE];
	//Row 1-3 初始化 Value，A/M，Range

	for(int i=1;i<=3;i++)
	{
		


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
		 
		if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9) ||(product_register_value[7] == PM_TSTAT5i)
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
			
			)
		{
			CppSQLite3Table table;
			CppSQLite3Query q;

			CppSQLite3DB SqliteDBBuilding;
			SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

			if (SqliteDBBuilding.tableExists("Value_Range"))//有Version表
			 {
				 CString sql;
				 sql.Format(_T("Select * from Value_Range where CInputNo=%d%d and SN=%d"),i,i,m_sn);
				 q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);

				 if (!q.eof())//有表但是没有对应序列号的值
				 {    
					  
					 while (!q.eof())
					 {
						 m_crange=q.getIntField("CRange");
						 q.nextRow();
					 }
					 nRange=m_crange;	 
					 if(nRange>=0)
					 {
						 strTemp=OUTPUT_RANGE45[nRange];
					 }
				 } 
				 else
				 {
					  
					 if(nRange>=0)
					 {
						 strTemp=OUTPUT_RANGE45[nRange]; 
					 }
				 }
			 
			 }
			 else
			 {
				  
				 if(nRange>=0)
				 {
					 strTemp=OUTPUT_RANGE45[nRange]; 
				 }
			 }
			 SqliteDBBuilding.closedb();

			 if(nRange>=0&&nRange<7)
			 {
				 strTemp=OUTPUT_RANGE45[nRange];
			 }
		 } 
		 else
		 {
			 if(nRange>=0&&nRange<3)
			 {
				 strTemp=OUTPUT_RANGE5[nRange];
			 }
		}
				 
				
		

		m_FlexGrid.put_TextMatrix(i,RANG_OUTFIELD,strTemp);
		
		if (nRange==0||nRange==4||nRange==5||nRange==6)
		{if(nVAlue&(1<<(i-1)))
		{
			if (nRange==0||nRange==4)
			{
				strTemp=_T("On");
			} 
			else if (nRange==5||nRange==6)
			{
				strTemp=_T("Open");
			}


		}
		else
		{
			if (nRange==0||nRange==4)
			{
				strTemp=_T("Off");
			} 
			else if (nRange==5||nRange==6)
			{
				strTemp=_T("Closed");
			}
		}
		}
		else
		{
			//comments by Fance ,此前没有 348 -》对应 t6的598  ，现在有了。;所以该不该改为现在的？？？
			int nValueTemp = product_register_value[MODBUS_PWM_OUT4]; //348 //598
			strTemp.Format(_T("%d%%"), nValueTemp);
		}
		
		m_FlexGrid.put_TextMatrix(i,VALUE_OUTFIELD,strTemp);

	}
	//Fan  Comments 这里有问题 ， strTemp 判断了那么久 在后面直接赋值 NO_APPLICATION ，是为什么?;
	//Row1-3 其他几列
	
	#if 1 
	if(m_version>32.2)
	{
		//	328	266	1	Low byte	W/R	"Output1 Function setting:
		//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
		int nFun=0;//=product_register_value[328];//tstat6找不到对应的
		nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; //328   266
		if(nFun>=0&&nFun<5)
		{
			strTemp=ONTPUT_FUNS[nFun];
		}
	}
	else
	{
		strTemp=NO_APPLICATION;
	}
	//strTemp=ONTPUT_FUNS[0];//2.5.0.98
	m_FlexGrid.put_TextMatrix(1,FUN_OUTFIELD,strTemp);
	
	if (m_version<32.2)
	{

		short nFun=0;//product_register_value[328];
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
		int nFun=-1;
		//334	267	1	Low byte	W/R	Output2 function setting (see above)
		nFun = product_register_value[MODBUS_OUTPUT2_FUNCTION];
		strTemp=ONTPUT_FUNS[0];//2.5.0.98
		if(nFun>=0&&nFun<5)
		{
			strTemp=ONTPUT_FUNS[nFun];
		}
		m_FlexGrid.put_TextMatrix(2,FUN_OUTFIELD,strTemp);

		//335	268	1	Low byte	W/R	Output3 function setting (see above)
		nFun = product_register_value[MODBUS_OUTPUT3_FUNCTION];
		if(nFun>=0&&nFun<=4)
			strTemp=ONTPUT_FUNS[nFun];
		//strTemp=ONTPUT_FUNS[0];//2.5.0.98
		m_FlexGrid.put_TextMatrix(3,FUN_OUTFIELD,strTemp);

	}
#endif


//----------------------------------------------------------------------------------

	if(m_nModeType==1||m_nModeType==4||m_nModeType==12||m_nModeType==16 ||m_nModeType==PM_PM5E || m_nModeType == PM_PM5E_ARM
		||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT8 || m_nModeType == PM_TSTAT9 ||m_nModeType==PM_TSTAT5i||m_nModeType==PM_TSTAT7||m_nModeType==PM_PRESSURE
		|| (m_nModeType == PM_TSTAT8_WIFI) || (m_nModeType == PM_TSTAT8_OCC) || (m_nModeType == PM_TSTAT7_ARM) || (m_nModeType == PM_TSTAT8_220V)
		)//||m_nModeType==17||m_nModeType==18)
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
		//nRange=product_register_value[283];
		//283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
		 
		 
		{
			nRange = product_register_value[MODBUS_MODE_OUTPUT4];//283  205
			if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9)
				|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
				)
			{
				CppSQLite3Table table;
				CppSQLite3Query q;
				CppSQLite3DB SqliteDBBuilding;
				SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
				if (SqliteDBBuilding.tableExists("Value_Range"))//有Version表
				{
					CString sql;
					sql.Format(_T("Select * from Value_Range where CInputNo=%d%d and SN=%d"),4,4,m_sn);
					q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);
					if (!q.eof())//有表但是没有对应序列号的值
					{    
						 
						while (!q.eof())
						{
							m_crange=q.getIntField("CRange");
							q.nextRow();
						}
						nRange=m_crange;	 
						if(nRange>=0)
						{
							strTemp=OUTPUT_RANGE45[nRange];
						}
					} 
					else
					{

						if(nRange>=0)
						{
							strTemp=OUTPUT_RANGE45[nRange]; 
						}
					}
					 
				}
				else
				{

					if(nRange>=0)
					{
						strTemp=OUTPUT_RANGE45[nRange]; 
					}
				}
			 SqliteDBBuilding.closedb();

				if(nRange>=0&&nRange<7)
				{
					strTemp=OUTPUT_RANGE45[nRange];
				}
			} 
			else
			{
				if(nRange>=0&&nRange<=2)		//Modify by Fance_0412
				{
					strTemp=OUTPUT_RANGE5[nRange];
				}
			}
		}
		
		
		m_FlexGrid.put_TextMatrix(4,RANG_OUTFIELD,strTemp);

		if(nRange == 0||nRange==4||nRange==5||nRange==6 || !(nAMVAlue & 8)) // AM栏选择了Auto或者Range 栏选择了On/Off，value都显示ON/Off
		{  // output is on/off
			 

			if(nVAlue&8)
			{
				if (nRange==0||nRange==4)
				{
					strTemp=_T("On");
				} 
				else if (nRange==5||nRange==6)
				{
					strTemp=_T("Open");
				}


			}
			else
			{
				if (nRange==0||nRange==4)
				{
					strTemp=_T("Off");
				} 
				else if (nRange==5||nRange==6)
				{
					strTemp=_T("Closed");
				}
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
			short nFun=0;//product_register_value[328];
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
			//indext=product_register_value[336];
			//if (product_register_value[7] ==PM_TSTAT6)
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

		//nRange=product_register_value[284];
		//284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM
		nRange = product_register_value[MODBUS_MODE_OUTPUT5];
		if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9)
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
			)
		{
			CppSQLite3Table table;
			CppSQLite3Query q;

			CppSQLite3DB SqliteDBBuilding;
			SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

			if (SqliteDBBuilding.tableExists("Value_Range"))//有Version表
			{
				CString sql;
				sql.Format(_T("Select * from Value_Range where CInputNo=%d%d and SN=%d"),5,5,m_sn);
				q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);
				if (!q.eof())//有表但是没有对应序列号的值
				{    
					 
					while (!q.eof())
					{
						m_crange=q.getIntField("CRange");
						q.nextRow();
					}
					nRange=m_crange;	 
					if(nRange>=0)
					{
						strTemp=OUTPUT_RANGE45[nRange];
					}
				} 
				else
				{

					if(nRange>=0)
					{
						strTemp=OUTPUT_RANGE45[nRange]; 
					}
				}
				 
			}
			else
			{

				if(nRange>=0)
				{
					strTemp=OUTPUT_RANGE45[nRange]; 
				}
			}
		 SqliteDBBuilding.closedb();

			if(nRange>=0&&nRange<7)
			{
				strTemp=OUTPUT_RANGE45[nRange];
			}
		}
		else
		{
			if(nRange>=0&&nRange<=2)
			{
				strTemp=OUTPUT_RANGE5[nRange];
			}
		}
		
		m_FlexGrid.put_TextMatrix(5,RANG_OUTFIELD,strTemp);

		//////////////////////////////////////////////////////////////////////////
		if(nRange == 0||nRange==4||nRange==5||nRange==6  || !(nAMVAlue & 16))
		{			
			if(nVAlue&16)
			{
				if (nRange==0||nRange==4)
				{
					strTemp=_T("On");
				} 
				else if (nRange==5||nRange==6)
				{
					strTemp=_T("Open");
				}


			}
			else
			{
				if (nRange==0||nRange==4)
				{
					strTemp=_T("Off");
				} 
				else if (nRange==5||nRange==6)
				{
					strTemp=_T("Closed");
				}
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
			short nFun;//=product_register_value[328];
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

			if(indext>=0&&indext<=4)
				strTemp=ONTPUT_FUNS[indext];
			 
			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//row4,5,for 5F,5G:
	if(m_nModeType==17||m_nModeType==18)
	{
		//out4:
		int nRang=0;//product_register_value[283];
		nRang= product_register_value[MODBUS_OUTPUT1_FUNCTION];//328  266
		int nValue1;//=product_register_value[108];
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
		if((product_register_value[7] == PM_TSTAT6)||
            (product_register_value[7] == PM_TSTAT7)||
            (product_register_value[7] == PM_TSTAT5i)||
            product_register_value[7] == PM_TSTAT8 || 
            product_register_value[7] == PM_TSTAT9 || 
             (product_register_value[7] == PM_TSTAT8_WIFI) || 
                (product_register_value[7] == PM_TSTAT8_OCC) || 
                (product_register_value[7] == PM_TSTAT7_ARM) || 
                (product_register_value[7] == PM_TSTAT8_220V)
			)
		{if(nRange>=0&&nRange<=3)
		{
			strTemp=OUTPUT_RANGE45[nRange];
		}
		} 
		else
		{
			if(nRange>=0&&nRange<=2)
			{
				strTemp=OUTPUT_RANGE5[nRange];
			}
		}
		
		m_FlexGrid.put_TextMatrix(4,RANG_OUTFIELD,strTemp);

		strTemp.Empty();
		if (m_version<32.2)
		{
			//328	266	1	Low byte	W/R	"Output1 Function setting:
			//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "

			short nFun=0;//product_register_value[328];
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
			if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9)
				|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
				)
			{
				indext=product_register_value[269];
			}else
			{
				//indext=product_register_value[336];//2.5.0.96
				indext=product_register_value[283];//2.5.0.98		//Fance_?_   //269 对应 336 这里为什么 要变为 283;

			}
			if(indext>=0&&indext<=4)
				strTemp=ONTPUT_FUNS[indext];
		 

			m_FlexGrid.put_TextMatrix(4,FUN_OUTFIELD,strTemp);
		}

		//out5:
		//284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM

		////337	270	1	Low byte	W/R	Output5 function setting (see above)
        if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9)
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
			)
        {
			nRang=product_register_value[206];
		}else
		{
			nRang=product_register_value[337];			//Fance_?_ 不明白 为什么 tstat5 要用337=MODBUS_OUTPUT5_FUNCTION 而tstat 要用206=MODBUS_MODE_OUTPUT5;
		}
		//108	209	1	Low byte	W/R	Output1 tot 5, bit 0 thru 4 = relay 1 thru 5.

		nValue1 = product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS];//108   209
		if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9)
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
			)
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
			//	nRang=product_register_value[284];
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
		if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9)
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
			)
		{
			if(nRange>=0&&nRange<=3)
			{
				strTemp=OUTPUT_RANGE45[nRange];
			}
		} 
		else
		{
			if(nRange>=0&&nRange<=2)
			{
				strTemp=OUTPUT_RANGE5[nRange];
			}
		}
		
		m_FlexGrid.put_TextMatrix(5,RANG_OUTFIELD,strTemp);
		strTemp.Empty();
		if (m_version<32.2)
		{
			//328	266	1	Low byte	W/R	"Output1 Function setting:
			//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "

			short nFun;//=product_register_value[328];
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
			if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9)
				|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
				)
			{
				indext=product_register_value[270];
			}else
			{
				//indext=product_register_value[337];//2.5.0.96
				indext=product_register_value[284];//2.5.0.98			//Fance_?_  为什么要不对应;

			}

			if(indext>=0&&indext<=4)
				strTemp=ONTPUT_FUNS[indext];
			//if (indext >=2)//2.5.0.98
			//{
			//	strTemp=ONTPUT_FUNS[0];
			//}
			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
		}
	}
	//row45/67 ANALOG
	if ((m_nModeType==1||m_nModeType==3||m_nModeType==2)||m_nModeType==12||m_nModeType==16||m_nModeType==PM_PM5E || m_nModeType == PM_PM5E_ARM ||m_nModeType==PM_PRESSURE
		||m_nModeType==18||m_nModeType==6||m_nModeType==PM_TSTAT5i||m_nModeType==PM_TSTAT8 || m_nModeType == PM_TSTAT9 ||m_nModeType==7
		
		|| (m_nModeType == PM_TSTAT8_WIFI) || (m_nModeType == PM_TSTAT8_OCC) || (m_nModeType == PM_TSTAT7_ARM) || (m_nModeType == PM_TSTAT8_220V)
		
		)//5ADEG
	{
		//186	207	1	Low byte	W/R	Analog Output1 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
		//102	210	2	Full	W/R(write only when manual output6 enable)	Output6 ,Analog output1, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.
		if(m_nModeType==1||m_nModeType==3||m_nModeType==2)
		{
			//186	207	1	Low byte	W/R	Analog Output1 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
			//102	210	2	Full	W/R(write only when manual output6 enable)	Output6 ,Analog output1, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.

			short nRange;//=product_register_value[186];
			int nValue;//e=product_register_value[102];
			//if (product_register_value[7] == 6)
			
				nRange=product_register_value[MODBUS_OUTPUT1_SCALE];
				nValue=product_register_value[MODBUS_COOLING_VALVE];

			

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
				//strTemp.Format(_T("%.1f"),product_register_value[102]/100.0);
				float nvalue=0.0;
				 
			
				{


					//if(nRange==1)//0-10v
					//{
						//nvalue=product_register_value[102]/100 /10.0 * 100%;
						nvalue=product_register_value[102]/10.0f;
					//}
					//if(nRange==2)//0-5v
					//{
					//	nvalue=product_register_value[102]/5.0f;
					//}
					//if(nRange==3)//2-10v
					//{
					//	nvalue=product_register_value[102]/8.0f;
					//}
					//if(nRange==3)//10-0v
					//{
					//	nvalue=(10-product_register_value[102]/100.0f)/10.0f *100;
					//}
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

			if(nRange>=0&&nRange<17)
			{
				strTemp=OUTPUT_ANRANGE[nRange];
			}
			m_FlexGrid.put_TextMatrix(4,RANG_OUTFIELD,strTemp);

			int indext=-1;
			indext = product_register_value[MODBUS_OUTPUT6_FUNCTION];

			if(indext>=0&&indext<=4)
				strTemp=ONTPUT_FUNS[indext];

			m_FlexGrid.put_TextMatrix(4,FUN_OUTFIELD,strTemp);
		 
		 
				nRange=product_register_value[MODBUS_OUTPUT2_SCALE];
				nValue=product_register_value[MODBUS_HEATING_VALVE];

			 
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
				//if (product_register_value[7] == 6)
			
				//{
					//if(nRange==1)//0-10v
					//{
						//nvalue=product_register_value[102]/100 /10.0 * 100%;
						nvalue=product_register_value[103]/10.0f;
					//}
					//if(nRange==2)//0-5v
					//{
					//	nvalue=product_register_value[103]/5.0f;
					//}
					//if(nRange==3)//2-10v
					//{
					//	nvalue=product_register_value[103]/8.0f;
					//}
					//if(nRange==4)//10-0v
					//{
					//	nvalue=(10-product_register_value[103]/100.0f)/10.0f *100;
					//}
				//}
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

			if(nRange>=0&&nRange<17)
			{
				strTemp=OUTPUT_ANRANGE[nRange];
			}
			m_FlexGrid.put_TextMatrix(5,RANG_OUTFIELD,strTemp);


			  
			indext = product_register_value[MODBUS_OUTPUT7_FUNCTION];

			if(indext>=0&&indext<=4)
				strTemp=ONTPUT_FUNS[indext];
			 
			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);

		}
else
		{
			short nRange;//=product_register_value[186];
			int nValue;//e=product_register_value[102];
			//if (product_register_value[7] == 6)
			nRange = product_register_value[MODBUS_OUTPUT1_SCALE]; //186  207
			nValue = product_register_value[MODBUS_COOLING_VALVE]; //102  210

			if(nRange==0)
			{				
				if(nValue==0)	
					strTemp=_T("Off");
				else
					strTemp=_T("On");
			}
			else
			{
				float nvalue=0.0;
				//if (product_register_value[7] == 6)
				if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9)
					|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
					)
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
					if (nRange==17)
					{
						nvalue= product_register_value[MODBUS_COOLING_VALVE]/10.0f;
					}
				}
				else
				{


					if(nRange==1)//0-10v
					{
						//nvalue=product_register_value[102]/100 /10.0 * 100%;
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
			m_FlexGrid.put_TextMatrix(6,VALUE_OUTFIELD,strTemp);

		 
			{

				 

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
				if (nRange==17)
				{
					nRange=4;
				}
				if(nRange>=0&&nRange<5)
				{
					strTemp=OUTPUT_ANRANGE6[nRange];
				}
				 

				m_FlexGrid.put_TextMatrix(6,RANG_OUTFIELD,strTemp);


				//103	211	2	Full	W/R(write only when manual output7 enable)	Output7 Analog output2, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.    
				//187	208	1	Low byte	W/R	Analog Output2 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
				//if (product_register_value[7] == 6)



				nRange= product_register_value[MODBUS_OUTPUT2_SCALE];//187  208
				nValue= product_register_value[MODBUS_HEATING_VALVE];//103  211


				strTemp.Empty();
				if(nRange==0)
				{

					if(nValue==0)
						strTemp=_T("Off");
					else
						strTemp=_T("On");
				}
				else
				{
					//strTemp.Format(_T("%.1f"),nValue/100.0);
					float nvalue=0.0;
					 
					if ((product_register_value[7] == 6)||(product_register_value[7] == 7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9)
						|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
						)
					{
						if(nRange==1)//0-10v
						{
							//nvalue=product_register_value[102]/100 /10.0 * 100%;
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
						if (nRange==17)
						{
							nvalue=product_register_value[MODBUS_HEATING_VALVE]/10.0f;//103 211
						}
					}
					else
					{
						if(nRange==1)//0-10v
						{
							//nvalue=product_register_value[102]/100 /10.0 * 100%;
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
				if (nRange==17)
				{
					nRange=4;
				}
				if(nRange>=0&&nRange<5)
				{
					strTemp=OUTPUT_ANRANGE6[nRange];
				}
				m_FlexGrid.put_TextMatrix(7,RANG_OUTFIELD,strTemp);
			}
		 	int indext=-1;
			indext = product_register_value[MODBUS_OUTPUT6_FUNCTION];
			
			strTemp=ONTPUT_FUNS[0];
			if(indext>=0&&indext<=4)
				strTemp=ONTPUT_FUNS[indext];

			m_FlexGrid.put_TextMatrix(6,FUN_OUTFIELD,strTemp);

			 
			indext = product_register_value[MODBUS_OUTPUT7_FUNCTION];

			if(indext>=0&&indext<=4)
				strTemp=ONTPUT_FUNS[indext];

			m_FlexGrid.put_TextMatrix(7,FUN_OUTFIELD,strTemp); 

}
		

		
		strTemp.Empty();

	}
	
	
	
	//286	245	1	Low byte	W/R	Interlock for  output1

	CString strlock;
	int stradd;// = 286;
	//if (product_register_value[7] == 6)
	if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
	{
		stradd =MODBUS_INTERLOCK_OUTPUT1; //245;
		for (int i = 0;i<7;i++)
		{

			int itemp = product_register_value[stradd+i];
			if(itemp>=0&&itemp<6)
				m_FlexGrid.put_TextMatrix(i+1,INTER_LOCK,Interlock[itemp]);

		}
	}
	else
	{
		stradd =MODBUS_INTERLOCK_OUTPUT1;// 286;
		for (int i = 0;i<7;i++)
		{

			int itemp = product_register_value[stradd+i];
			if(itemp>=0&&itemp<6)
				m_FlexGrid.put_TextMatrix(i+1,INTER_LOCK,Interlock[itemp]);

		}


	}
	
	
	
	
	//Delay
	
	if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)

		)
	{  CString strdelay;
	for (int row=0;row<7;row++)
	{
		stradd=MODBUS_OUTPUT1_DELAY_OFF_TO_ON+row;	 //213
		int itemp=product_register_value[stradd];
		strdelay.Format(_T("%d"),itemp);
		m_FlexGrid.put_TextMatrix(row+1,DELAY_OFFON,strdelay);
	}
	for (int row=0;row<7;row++)
	{
		stradd=MODBUS_OUTPUT1_DELAY_ON_TO_OFF+row;	 //227
		int itemp=product_register_value[stradd];
		strdelay.Format(_T("%d"),itemp);
		m_FlexGrid.put_TextMatrix(row+1,DELAY_ONOFF,strdelay);
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
			m_FlexGrid.put_TextMatrix(row+1,DELAY_OFFON,strdelay);
		}
		for (int row=0;row<5;row++)
		{
			stradd=MODBUS_OUTPUT1_DELAY_ON_TO_OFF+row;	 //227
			int itemp=product_register_value[stradd];
			strdelay.Format(_T("%d"),itemp);
			m_FlexGrid.put_TextMatrix(row+1,DELAY_OFFON,strdelay);
		}
	}
	
	
	if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
	{
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

	///////////////////////////////////////重现写Range//////////////////////////
	if (product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT5i||product_register_value[7]==PM_TSTAT7||product_register_value[7]==PM_TSTAT8 || product_register_value[7] == PM_TSTAT9
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
	{
		for (int row=1;row<6;row++)
		{
			m_FlexGrid.put_TextMatrix(row,SIGUAL_TYPE,_T("UNUSED"));
		}
		CString strTemp;
		 int ST=product_register_value[692];
		 if (ST>=5&&ST<=16)
		 {
			 strTemp=OUTPUT_ANRANGE[ST];
		 }
		 else
		 {
			 strTemp=_T("UNUSED");
		 }
		m_FlexGrid.put_TextMatrix(6,SIGUAL_TYPE,strTemp);
		 strTemp=_T("UNUSED");
		  ST=product_register_value[693];
		if (ST>=5)
		{
			strTemp=OUTPUT_ANRANGE[ST];
		}
		else
		{
			strTemp=_T("UNUSED");
		}
		m_FlexGrid.put_TextMatrix(7,SIGUAL_TYPE,strTemp);

	}

}
//void COutputSetDlg::Fresh_Grid_Tstat5E()
//{
//	m_nModeType = product_register_value[7];
//
//	switch (m_nModeType)
//	{
//	case 2:
//	case 1:	m_outRows=6;break;
//	case 4:	m_outRows=6;break;
//	case PM_PRESSURE: break;
//	case PM_TSTAT5D:m_outRows=8;break;
//	case PM_TSTAT6:
//	case PM_TSTAT7:
//	case 16:m_outRows=8;break;
//	case 17:m_outRows=6;break;
//	case 18:m_outRows=8;break;
//	case 19:m_outRows=8;break;
//	default:break;
//	}
//	CString strTemp;
//	//row1:
//	int nVAlue;
//	//这段代码的作用是当选择的是Floating或者PWM模式的时候，
//	//要根据在outputtable中 用户选择的是PID1，还是PID2 PID1 从384 读值 PID2 就是从389读值
//	/////前三行
//#if 1
//	int pid_select2[7]={0};
//	for(int i=0;i<7;i++)
//		pid_select2[i]=product_register_value[MODBUS_PID_OUTPUT1+i];
//#endif
//
//
//	//108  209 Output1 tot 5, bit 0 thru 4 = relay 1 thru 5.  Fan.
//	nVAlue = product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS]; //t5=108   t6=209;
//	int nRange=0;
//
//
//
//	//310	254	1	Low byte	W/R	"Output auto/manual enable. Bit 0 to 4 correspond to output1 to output5, bit 5 correspond to 
//	//	output6, bit 6 correspond to output7. 0, auto mode; 1, manual mode."
//
//	int nAMVAlue=0;//=product_register_value[310];
//	nAMVAlue = product_register_value[MODBUS_OUTPUT_MANU_ENABLE];
//	//Row 1-3 初始化 Value，A/M，Range
//
//	for(int i=1;i<=3;i++)
//	{
//		if(nVAlue&(1<<(i-1)))
//		{
//			strTemp=_T("On");
//		}
//		else
//		{
//			strTemp=_T("Off");
//		}
//		m_FlexGrid.put_TextMatrix(i,VALUE_OUTFIELD,strTemp);
//
//
//		//int temp=1<<(i-1);
//		//int a = nAMVAlue;
//		//int b = nAMVAlue & (1<<(i-1));
//		if((int)(nAMVAlue & (1<<(i-1))) == (1<<(i-1)))
//		{
//			strTemp=_T("Manual");
//			m_FlexGrid.put_Col(VALUE_OUTFIELD);
//			m_FlexGrid.put_Row(i);
//			m_FlexGrid.put_CellBackColor(RGB(255,255,255));
//		}
//		else
//		{
//			strTemp=_T("Auto");
//			m_FlexGrid.put_Col(VALUE_OUTFIELD);
//			m_FlexGrid.put_Row(i);
//			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
//		} 
//		m_FlexGrid.put_TextMatrix(i,AM_OUTFIELD,strTemp);
//		//strTemp=_T("On/Off");
//		nRange=product_register_value[MODBUS_MODE_OUTPUT1+i-1];
//
//
//		 
//
//		if (product_register_value[7]==7||product_register_value[7]==6)
//		{
//			if(nRange>=0&&nRange<=3)
//			{
//				strTemp=OUTPUT_RANGE45[nRange];
//			}
//		} 
//		else
//		{
//			if(nRange>=0&&nRange<=2)
//			{
//				strTemp=OUTPUT_RANGE5[nRange];
//			}
//		}
//		m_FlexGrid.put_TextMatrix(i,RANG_OUTFIELD,strTemp);
//
//	}
//	//Fan  Comments 这里有问题 ， strTemp 判断了那么久 在后面直接赋值 NO_APPLICATION ，是为什么?;
//	//Row1-3 其他几列
//#if 1 
//	if(m_version>32.2)
//	{
//		//	328	266	1	Low byte	W/R	"Output1 Function setting:
//		//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
//		int nFun=0;//=product_register_value[328];//tstat6找不到对应的
//		nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; //328   266
//		if(nFun>=0&&nFun<4)
//		{
//			strTemp=ONTPUT_FUNS[nFun];
//		}
//	}
//	else
//	{
//		strTemp=NO_APPLICATION;
//	}
//	strTemp=ONTPUT_FUNS[0];//2.5.0.98
//	m_FlexGrid.put_TextMatrix(1,FUN_OUTFIELD,strTemp);
//
//	if (m_version<32.2)
//	{
//
//		short nFun=0;//product_register_value[328];
//		//328	266	1	int8	1	Low byte	W/R	"Output1 Function setting:
//		//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
//		nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION];
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
//
//
//		nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; //328  266
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
//
//	}
//	else
//	{
//		int indext=-1;
//		//334	267	1	Low byte	W/R	Output2 function setting (see above)
//		indext = product_register_value[MODBUS_OUTPUT2_FUNCTION];
//		strTemp=ONTPUT_FUNS[0];//2.5.0.98
//		m_FlexGrid.put_TextMatrix(2,FUN_OUTFIELD,strTemp);
//
//		//335	268	1	Low byte	W/R	Output3 function setting (see above)
//		indext = product_register_value[MODBUS_OUTPUT3_FUNCTION];
//		if(indext>=0&&indext<=3)
//			strTemp=ONTPUT_FUNS[indext];
//		strTemp=ONTPUT_FUNS[0];//2.5.0.98
//		m_FlexGrid.put_TextMatrix(3,FUN_OUTFIELD,strTemp);
//
//	}
//#endif
//
//
//	//----------------------------------------------------------------------------------
//
//	if(m_nModeType==1||m_nModeType==4||m_nModeType==12||m_nModeType==16 
//		||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7||m_nModeType==PM_PRESSURE)//||m_nModeType==17||m_nModeType==18)
//	{
//		// just for row4 ///////////////////////////////////////////////////////////////
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
//		//nRange=product_register_value[283];
//		//283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
//
//		nRange = product_register_value[MODBUS_MODE_OUTPUT4];//283  205
//
//		 
//		if (product_register_value[7]==7||product_register_value[7]==6)
//		{
//			if(nRange>=0&&nRange<=3)
//			{
//				strTemp=OUTPUT_RANGE45[nRange];
//			}
//		} 
//		else
//		{
//			if(nRange>=0&&nRange<=2)
//			{
//				strTemp=OUTPUT_RANGE5[nRange];
//			}
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
//			strTemp.Format(_T("%d%%"), nValueTemp);
//		}
//		m_FlexGrid.put_TextMatrix(4,VALUE_OUTFIELD,strTemp);
//
//		//////////////////////////////////////////////////////////////////////////
//
//		if (m_version<32.2)
//		{
//			short nFun=0;//product_register_value[328];
//			nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; //328  266
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
//			//indext=product_register_value[336];
//			//if (product_register_value[7] ==PM_TSTAT6)
//			indext = product_register_value[MODBUS_OUTPUT4_FUNCTION];
//			if(indext>=0&&indext<=3)
//				strTemp=ONTPUT_FUNS[indext];
//			strTemp=ONTPUT_FUNS[0];//2.5.0.98
//			m_FlexGrid.put_TextMatrix(4,FUN_OUTFIELD,strTemp);
//		}
//
//		///////////////////////////////////////////////////////////////////////////
//		// just for row5
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
//		//nRange=product_register_value[284];
//		//284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM
//		nRange = product_register_value[MODBUS_MODE_OUTPUT5];
//
//		if (product_register_value[7]==7||product_register_value[7]==6)
//		{
//			if(nRange>=0&&nRange<=3)
//			{
//				strTemp=OUTPUT_RANGE45[nRange];
//			}
//		} 
//		else
//		{
//			if(nRange>=0&&nRange<=2)
//			{
//				strTemp=OUTPUT_RANGE5[nRange];
//			}
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
//			int nValueTemp=product_register_value[MODBUS_PWM_OUT5];	//tstat6没有找到	349 ,599
//			strTemp.Format(_T("%d%%"), nValueTemp);
//		}
//		m_FlexGrid.put_TextMatrix(5,VALUE_OUTFIELD,strTemp);
//		//////////////////////////////////////////////////////////////////////////
//		if (m_version<32.2)
//		{
//			//328	266	1	Low byte	W/R	"Output1 Function setting:
//			//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
//			short nFun;//=product_register_value[328];
//			nFun= product_register_value[MODBUS_OUTPUT1_FUNCTION];
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
//
//			if(indext>=0&&indext<=3)
//				strTemp=ONTPUT_FUNS[indext];
//			strTemp=ONTPUT_FUNS[0];//2.5.0.98
//			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
//		}
//	}
//
//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	//row4,5,for 5F,5G:
//	if(m_nModeType==17||m_nModeType==18)
//	{
//		//out4:
//		int nRang=0;//product_register_value[283];
//		nRang= product_register_value[MODBUS_OUTPUT1_FUNCTION];//328  266
//		int nValue1;//=product_register_value[108];
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
//		//2.5.0.94
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
//		//AM
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
//
//		if (product_register_value[7]==7||product_register_value[7]==6)
//		{
//			if(nRange>=0&&nRange<=3)
//			{
//				strTemp=OUTPUT_RANGE45[nRange];
//			}
//		} 
//		else
//		{
//			if(nRange>=0&&nRange<=2)
//			{
//				strTemp=OUTPUT_RANGE5[nRange];
//			}
//		}
//		m_FlexGrid.put_TextMatrix(4,RANG_OUTFIELD,strTemp);
//
//		strTemp.Empty();
//		if (m_version<32.2)
//		{
//			//328	266	1	Low byte	W/R	"Output1 Function setting:
//			//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
//
//			short nFun=0;//product_register_value[328];
//			nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION];
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
//			//336	269	1	Low byte	W/R	Output4 function setting (see above)
//			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//			{
//				indext=product_register_value[269];
//			}else
//			{
//				//indext=product_register_value[336];//2.5.0.96
//				indext=product_register_value[283];//2.5.0.98		//Fance_?_   //269 对应 336 这里为什么 要变为 283;
//
//			}
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
//		////337	270	1	Low byte	W/R	Output5 function setting (see above)
//		if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//		{
//			nRang=product_register_value[206];
//		}else
//		{
//			nRang=product_register_value[337];			//Fance_?_ 不明白 为什么 tstat5 要用337=MODBUS_OUTPUT5_FUNCTION 而tstat 要用206=MODBUS_MODE_OUTPUT5;
//		}
//		//108	209	1	Low byte	W/R	Output1 tot 5, bit 0 thru 4 = relay 1 thru 5.
//
//		nValue1 = product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS];//108   209
//		if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//		{
//			if(nRang==0)
//			{
//				if(nValue1 &( 1<<3))
//					//if(nValue1 &( 1<<4))//2.5.0.94
//					strTemp=_T("ON");
//				else
//					strTemp=_T("OFF");
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
//				if(nValue1 &( 1<<4))//2.5.0.94
//					strTemp=_T("ON");
//				else
//					strTemp=_T("OFF");
//			}
//			else
//			{
//				strTemp.Format(_T("%d%%"),product_register_value[MODBUS_PWM_OUT5]);//349  599 //Fance_?
//			}
//			m_FlexGrid.put_TextMatrix(5,VALUE_OUTFIELD,strTemp);
//			//2.5.0.94
//		}else
//		{
//			nRang = product_register_value[MODBUS_MODE_OUTPUT5];// 284   206
//			//	nRang=product_register_value[284];
//			if(nRang==0)
//			{
//				if(nValue1 &( 1<<3))
//					//if(nValue1 &( 1<<4))//2.5.0.94
//					strTemp=_T("ON");
//				else
//					strTemp=_T("OFF");
//			}
//			else
//			{
//				strTemp.Format(_T("%d%%"),product_register_value[MODBUS_PWM_OUT5]);//找不到  349 599
//			}
//			m_FlexGrid.put_TextMatrix(5,VALUE_OUTFIELD,strTemp);
//
//		}
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
//		if (product_register_value[7]==7||product_register_value[7]==6)
//		{
//			if(nRange>=0&&nRange<=3)
//			{
//				strTemp=OUTPUT_RANGE45[nRange];
//			}
//		} 
//		else
//		{
//			if(nRange>=0&&nRange<=2)
//			{
//				strTemp=OUTPUT_RANGE5[nRange];
//			}
//		}
//		m_FlexGrid.put_TextMatrix(5,RANG_OUTFIELD,strTemp);
//		strTemp.Empty();
//		if (m_version<32.2)
//		{
//			//328	266	1	Low byte	W/R	"Output1 Function setting:
//			//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
//
//			short nFun;//=product_register_value[328];
//			nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; // 328  266;
//
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
//				//indext=product_register_value[337];//2.5.0.96
//				indext=product_register_value[284];//2.5.0.98			//Fance_?_  为什么要不对应;
//
//			}
//
//			if(indext>=0&&indext<=3)
//				strTemp=ONTPUT_FUNS[indext];
//			if (indext >=2)//2.5.0.98
//			{
//				strTemp=ONTPUT_FUNS[0];
//			}
//			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
//		}
//	}
//	//row45/67 ANALOG
//	if (m_nModeType==2||m_nModeType==12||m_nModeType==16||m_nModeType==PM_PRESSURE
//		||m_nModeType==18||m_nModeType==6||m_nModeType==7)//5ADEG
//	{
//		//186	207	1	Low byte	W/R	Analog Output1 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
//		//102	210	2	Full	W/R(write only when manual output6 enable)	Output6 ,Analog output1, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.
//
//		short nRange;//=product_register_value[186];
//		int nValue;//e=product_register_value[102];
//		//if (product_register_value[7] == 6)
//		nRange = product_register_value[MODBUS_OUTPUT1_SCALE]; //186  207
//		nValue = product_register_value[MODBUS_COOLING_VALVE]; //102  210
//
//		if(nRange==0)
//		{				
//			if(nValue==0)	
//				strTemp=_T("Off");
//			if(nValue==1000)
//				strTemp=_T("On");
//		}
//		else
//		{
//			float nvalue=0.0;
//			//if (product_register_value[7] == 6)
//			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//			{
//				//AfxMessageBox(_T("It's impossible to enter this place!"));
//				if(nRange==1)//0-10v
//				{
//					nvalue= product_register_value[MODBUS_COOLING_VALVE]/10.0f;//  T6=210
//				}
//				if(nRange==2)//0-5v
//				{
//					nvalue=product_register_value[MODBUS_COOLING_VALVE]/5.0f;
//				}
//				if(nRange==3)//2-10v
//				{
//					nvalue=product_register_value[MODBUS_COOLING_VALVE]/8.0f;
//				}
//				if(nRange==3)//10-0v
//				{
//					nvalue=(10-product_register_value[MODBUS_COOLING_VALVE]/100.0f)/10.0f *100;
//				}
//			}
//			else
//			{
//
//
//				if(nRange==1)//0-10v
//				{
//					//nvalue=product_register_value[102]/100 /10.0 * 100%;
//					nvalue=product_register_value[MODBUS_COOLING_VALVE]/10.0f;//102   210
//				}
//				if(nRange==2)//0-5v
//				{
//					nvalue=product_register_value[MODBUS_COOLING_VALVE]/5.0f;
//				}
//				if(nRange==3)//2-10v
//				{
//					nvalue=product_register_value[MODBUS_COOLING_VALVE]/8.0f;
//				}
//				if(nRange==3)//10-0v
//				{
//					nvalue=(10-product_register_value[MODBUS_COOLING_VALVE]/100.0f)/10.0f *100;
//				}
//			}
//			strTemp.Format(_T("%.1f%%"),nvalue);
//		}
//		if(m_nModeType==6)
//		{
//
//			m_FlexGrid.put_TextMatrix(6,VALUE_OUTFIELD,strTemp);
//
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
//			m_FlexGrid.put_TextMatrix(6,AM_OUTFIELD,strTemp);
//
//			if(nRange>=0&&nRange<4)
//			{
//				strTemp=OUTPUT_ANRANGE[nRange];
//			}
//			m_FlexGrid.put_TextMatrix(6,RANG_OUTFIELD,strTemp);
//
//			//row5
//
//
//			//103	211	2	Full	W/R(write only when manual output7 enable)	Output7 Analog output2, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.    
//			//187	208	1	Low byte	W/R	Analog Output2 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
//
//			nRange= product_register_value[MODBUS_OUTPUT2_SCALE];//187  208
//			nValue= product_register_value[MODBUS_HEATING_VALVE];//103  211
//
//			if(nRange==0)
//			{
//
//				if(nValue==0)
//					strTemp=_T("Off");
//				if(nValue==1)
//					strTemp=_T("On");
//			}
//			else
//			{
//				//strTemp.Format(_T("%.1f"),nValue/100.0);
//
//				float nvalue=0.0;
//				//if (product_register_value[7] == 6)
//				if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//				{
//					//AfxMessageBox(_T("It's impossible to enter this place!222"));
//					if(nRange==1)//0-10v
//					{
//						//nvalue=product_register_value[102]/100 /10.0 * 100%;
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
//					if(nRange==1)//0-10v
//					{
//						//nvalue=product_register_value[102]/100 /10.0 * 100%;
//						nvalue= product_register_value[MODBUS_HEATING_VALVE]/10.0f;//103   211
//					}
//					if(nRange==2)//0-5v
//					{
//						nvalue= product_register_value[MODBUS_HEATING_VALVE]/5.0f;
//					}
//					if(nRange==3)//2-10v
//					{
//						nvalue= product_register_value[MODBUS_HEATING_VALVE]/8.0f;
//					}
//					if(nRange==4)//10-0v
//					{
//						nvalue=(10- product_register_value[MODBUS_HEATING_VALVE]/100.0f)/10.0f *100;
//					}
//				}
//				strTemp.Format(_T("%.1f%%"),nvalue);
//			}
//			m_FlexGrid.put_TextMatrix(7,VALUE_OUTFIELD,strTemp);
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
//			m_FlexGrid.put_TextMatrix(7,AM_OUTFIELD,strTemp);
//
//			if(nRange>=0&&nRange<5)
//			{
//				strTemp=OUTPUT_ANRANGE[nRange];
//			}
//			m_FlexGrid.put_TextMatrix(7,RANG_OUTFIELD,strTemp);
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
//			//if (product_register_value[7] == 6)
//
//
//
//			nRange= product_register_value[MODBUS_OUTPUT2_SCALE];//187  208
//			nValue= product_register_value[MODBUS_HEATING_VALVE];//103  211
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
//			}
//			else
//			{
//				//strTemp.Format(_T("%.1f"),nValue/100.0);
//				float nvalue=0.0;
//				//if (product_register_value[7] == 6)
//				if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//				{
//					if(nRange==1)//0-10v
//					{
//						//nvalue=product_register_value[102]/100 /10.0 * 100%;
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
//					if(nRange==1)//0-10v
//					{
//						//nvalue=product_register_value[102]/100 /10.0 * 100%;
//						nvalue=product_register_value[MODBUS_HEATING_VALVE]/10.0f;
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
//				}
//				strTemp.Format(_T("%.1f%%"),nvalue);
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
//	//if (product_register_value[7] == 6)
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
//	}
//	else
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
//	//Delay
//
//	if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//	{  CString strdelay;
//	for (int row=0;row<7;row++)
//	{
//		stradd=MODBUS_OUTPUT1_DELAY_OFF_TO_ON+row;	 //213
//		int itemp=product_register_value[stradd];
//		strdelay.Format(_T("%d"),itemp);
//		m_FlexGrid.put_TextMatrix(row+1,7,strdelay);
//	}
//	for (int row=0;row<7;row++)
//	{
//		stradd=MODBUS_OUTPUT1_DELAY_ON_TO_OFF+row;	 //227
//		int itemp=product_register_value[stradd];
//		strdelay.Format(_T("%d"),itemp);
//		m_FlexGrid.put_TextMatrix(row+1,8,strdelay);
//	}
//	}
//	else
//	{
//		CString strdelay;
//		for (int row=0;row<5;row++)
//		{
//			stradd=MODBUS_OUTPUT1_DELAY_OFF_TO_ON+row;	 //213
//			int itemp=product_register_value[stradd];
//			strdelay.Format(_T("%d"),itemp);
//			m_FlexGrid.put_TextMatrix(row+1,7,strdelay);
//		}
//		for (int row=0;row<5;row++)
//		{
//			stradd=MODBUS_OUTPUT1_DELAY_ON_TO_OFF+row;	 //227
//			int itemp=product_register_value[stradd];
//			strdelay.Format(_T("%d"),itemp);
//			m_FlexGrid.put_TextMatrix(row+1,8,strdelay);
//		}
//	}
//	if ((product_register_value[7] == 6)||(product_register_value[7] == 7)){
//
//
//
//		nAMVAlue = product_register_value[MODBUS_OUTPUT_MANU_ENABLE];
//
//		for (int row=1;row<=5;row++)
//		{
//
//
//
//
//			if (product_register_value[MODBUS_MODE_OUTPUT1+row-1]==3)
//			{
//				strTemp=_T("Auto");
//				m_FlexGrid.put_Col(AM_OUTFIELD);
//				m_FlexGrid.put_Row(row);
//				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
//				m_FlexGrid.put_Col(VALUE_OUTFIELD);
//				m_FlexGrid.put_Row(row);
//				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
//			}
//			else
//			{
//				m_FlexGrid.put_Col(AM_OUTFIELD);
//				m_FlexGrid.put_Row(row);
//				m_FlexGrid.put_CellBackColor(RGB(255,255,255));
//
//				if((int)(nAMVAlue & (1<<(row-1))) == (1<<(row-1)))
//				{
//					strTemp=_T("Manual");
//					m_FlexGrid.put_Col(VALUE_OUTFIELD);
//					m_FlexGrid.put_Row(row);
//					m_FlexGrid.put_CellBackColor(RGB(255,255,255));
//				}
//				else
//				{
//					strTemp=_T("Auto");
//					m_FlexGrid.put_Col(VALUE_OUTFIELD);
//					m_FlexGrid.put_Row(row);
//					m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
//				}
//			}
//			m_FlexGrid.put_TextMatrix(row,AM_OUTFIELD,strTemp);
//		}
//
//	}
//
//
//
//}


//void COutputSetDlg::Fresh_GridForTstat6()
//{
//	m_outRows=8;
//
//	CString strTemp;
//	//row1:
//	int nVAlue;
//
//
//	//108  209 Output1 tot 5, bit 0 thru 4 = relay 1 thru 5.  Fan.
//	nVAlue = product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS]; //t5=108   t6=209;
//	int nRange=0;
//
//
//
//	//310	254	1	Low byte	W/R	"Output auto/manual enable. Bit 0 to 4 correspond to output1 to output5, bit 5 correspond to 
//	//	output6, bit 6 correspond to output7. 0, auto mode; 1, manual mode."
//
//	int nAMVAlue=0;//=product_register_value[310];
//	nAMVAlue = product_register_value[MODBUS_OUTPUT_MANU_ENABLE];
//
//	for(int i=1;i<=3;i++)
//	{
//		if(nVAlue&(1<<(i-1)))
//		{
//			strTemp=_T("On");
//		}
//		else
//		{
//			strTemp=_T("Off");
//		}
//		m_FlexGrid.put_TextMatrix(i,VALUE_OUTFIELD,strTemp);
//
//
//		//int temp=1<<(i-1);
//		//int a = nAMVAlue;
//		//int b = nAMVAlue & (1<<(i-1));
//		if((int)(nAMVAlue & (1<<(i-1))) == (1<<(i-1)))
//		{
//			strTemp=_T("Manual");
//			m_FlexGrid.put_Col(VALUE_OUTFIELD);
//			m_FlexGrid.put_Row(i);
//			m_FlexGrid.put_CellBackColor(RGB(255,255,255));
//		}
//		else
//		{
//			strTemp=_T("Auto");
//			m_FlexGrid.put_Col(VALUE_OUTFIELD);
//			m_FlexGrid.put_Row(i);
//			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
//		} 
//		m_FlexGrid.put_TextMatrix(i,AM_OUTFIELD,strTemp);
//		strTemp=_T("On/Off");
//		m_FlexGrid.put_TextMatrix(i,RANG_OUTFIELD,strTemp);
//
//	}
//	//Fan  Comments 这里有问题 ， strTemp 判断了那么久 在后面直接赋值 NO_APPLICATION ，是为什么?;
//	if(m_version>32.2)
//	{
//		//	328	266	1	Low byte	W/R	"Output1 Function setting:
//		//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
//		int nFun=0;//=product_register_value[328];//tstat6找不到对应的
//		nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; //328   266
//		if(nFun>=0&&nFun<4)
//		{
//			strTemp=ONTPUT_FUNS[nFun];
//		}
//	}
//	else
//	{
//		strTemp=NO_APPLICATION;
//	}
//	strTemp=ONTPUT_FUNS[0];//2.5.0.98
//	m_FlexGrid.put_TextMatrix(1,FUN_OUTFIELD,strTemp);
//
//	if (m_version<32.2)
//	{
//
//		short nFun=0;//product_register_value[328];
//		//328	266	1	int8	1	Low byte	W/R	"Output1 Function setting:
//		//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
//		nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION];
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
//
//
//		nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; //328  266
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
//
//	}
//	else
//	{
//		int indext=-1;
//		//334	267	1	Low byte	W/R	Output2 function setting (see above)
//		indext = product_register_value[MODBUS_OUTPUT2_FUNCTION];
//		strTemp=ONTPUT_FUNS[0];//2.5.0.98
//		m_FlexGrid.put_TextMatrix(2,FUN_OUTFIELD,strTemp);
//
//		//335	268	1	Low byte	W/R	Output3 function setting (see above)
//		indext = product_register_value[MODBUS_OUTPUT3_FUNCTION];
//		if(indext>=0&&indext<=3)
//			strTemp=ONTPUT_FUNS[indext];
//		strTemp=ONTPUT_FUNS[0];//2.5.0.98
//		m_FlexGrid.put_TextMatrix(3,FUN_OUTFIELD,strTemp);
//
//	}
//
//	//----------------------------------------------------------------------------------
//	//----------------------------------------------------------------------------------
//	if(m_nModeType==1||m_nModeType==4||m_nModeType==12||m_nModeType==16 
//		||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7||m_nModeType==PM_PRESSURE)//||m_nModeType==17||m_nModeType==18)
//	{
//		// just for row4 ///////////////////////////////////////////////////////////////
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
//		//nRange=product_register_value[283];
//		//283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
//
//		nRange = product_register_value[MODBUS_MODE_OUTPUT4];//283  205
//
//		if (product_register_value[7]==7||product_register_value[7]==6)
//		{
//			if(nRange>=0&&nRange<=3)
//			{
//				strTemp=OUTPUT_RANGE45[nRange];
//			}
//		} 
//		else
//		{
//			if(nRange>=0&&nRange<=2)
//			{
//				strTemp=OUTPUT_RANGE5[nRange];
//			}
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
//			strTemp.Format(_T("%d%%"), nValueTemp);
//		}
//		m_FlexGrid.put_TextMatrix(4,VALUE_OUTFIELD,strTemp);
//
//		//////////////////////////////////////////////////////////////////////////
//
//		if (m_version<32.2)
//		{
//			short nFun=0;//product_register_value[328];
//			nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; //328  266
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
//			//indext=product_register_value[336];
//			//if (product_register_value[7] ==PM_TSTAT6)
//			indext = product_register_value[MODBUS_OUTPUT4_FUNCTION];
//			if(indext>=0&&indext<=3)
//				strTemp=ONTPUT_FUNS[indext];
//			strTemp=ONTPUT_FUNS[0];//2.5.0.98
//			m_FlexGrid.put_TextMatrix(4,FUN_OUTFIELD,strTemp);
//		}
//
//		///////////////////////////////////////////////////////////////////////////
//		// just for row5
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
//		//nRange=product_register_value[284];
//		//284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM
//		nRange = product_register_value[MODBUS_MODE_OUTPUT5];
//
//		if (product_register_value[7]==7||product_register_value[7]==6)
//		{
//			if(nRange>=0&&nRange<=3)
//			{
//				strTemp=OUTPUT_RANGE45[nRange];
//			}
//		} 
//		else
//		{
//			if(nRange>=0&&nRange<=2)
//			{
//				strTemp=OUTPUT_RANGE5[nRange];
//			}
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
//			int nValueTemp=product_register_value[MODBUS_PWM_OUT5];	//tstat6没有找到	349 ,599
//			strTemp.Format(_T("%d%%"), nValueTemp);
//		}
//		m_FlexGrid.put_TextMatrix(5,VALUE_OUTFIELD,strTemp);
//		//////////////////////////////////////////////////////////////////////////
//		if (m_version<32.2)
//		{
//			//328	266	1	Low byte	W/R	"Output1 Function setting:
//			//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
//			short nFun;//=product_register_value[328];
//			nFun= product_register_value[MODBUS_OUTPUT1_FUNCTION];
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
//
//			if(indext>=0&&indext<=3)
//				strTemp=ONTPUT_FUNS[indext];
//			strTemp=ONTPUT_FUNS[0];//2.5.0.98
//			m_FlexGrid.put_TextMatrix(5,FUN_OUTFIELD,strTemp);
//		}
//	}
//
//	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	//row4,5,for 5F,5G:
//
//	//row45/67 ANALOG
//	if (m_nModeType==2||m_nModeType==12||m_nModeType==16||m_nModeType==PM_PRESSURE
//		||m_nModeType==18||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7)//5ADEG
//	{
//		//186	207	1	Low byte	W/R	Analog Output1 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
//		//102	210	2	Full	W/R(write only when manual output6 enable)	Output6 ,Analog output1, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.
//
//		short nRange;//=product_register_value[186];
//		int nValue;//e=product_register_value[102];
//		//if (product_register_value[7] == 6)
//		nRange = product_register_value[MODBUS_OUTPUT1_SCALE]; //186  207
//		nValue = product_register_value[MODBUS_COOLING_VALVE]; //102  210
//
//		if(nRange==0)
//		{				
//			if(nValue==0)	
//				strTemp=_T("Off");
//			if(nValue==1000)
//				strTemp=_T("On");
//		}
//		else
//		{
//			float nvalue=0.0;
//			//if (product_register_value[7] == 6)
//			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//			{
//				//AfxMessageBox(_T("It's impossible to enter this place!"));
//				if(nRange==1)//0-10v
//				{
//					nvalue= product_register_value[MODBUS_COOLING_VALVE]/10.0f;//  T6=210
//				}
//				if(nRange==2)//0-5v
//				{
//					nvalue=product_register_value[MODBUS_COOLING_VALVE]/5.0f;
//				}
//				if(nRange==3)//2-10v
//				{
//					nvalue=product_register_value[MODBUS_COOLING_VALVE]/8.0f;
//				}
//				if(nRange==3)//10-0v
//				{
//					nvalue=(10-product_register_value[MODBUS_COOLING_VALVE]/100.0f)/10.0f *100;
//				}
//			}
//			else
//			{
//
//
//				if(nRange==1)//0-10v
//				{
//					//nvalue=product_register_value[102]/100 /10.0 * 100%;
//					nvalue=product_register_value[MODBUS_COOLING_VALVE]/10.0f;//102   210
//				}
//				if(nRange==2)//0-5v
//				{
//					nvalue=product_register_value[MODBUS_COOLING_VALVE]/5.0f;
//				}
//				if(nRange==3)//2-10v
//				{
//					nvalue=product_register_value[MODBUS_COOLING_VALVE]/8.0f;
//				}
//				if(nRange==3)//10-0v
//				{
//					nvalue=(10-product_register_value[MODBUS_COOLING_VALVE]/100.0f)/10.0f *100;
//				}
//			}
//			strTemp.Format(_T("%.1f%%"),nvalue);
//		}
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
//			nRange = product_register_value[MODBUS_MODE_OUTPUT4];
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
//
//			if(nRange==0)
//			{
//
//				if(nValue==0)
//					strTemp=_T("Off");
//				if(nValue==1)
//					strTemp=_T("On");
//			}
//			else
//			{
//				//strTemp.Format(_T("%.1f"),nValue/100.0);
//
//				float nvalue=0.0;
//				//if (product_register_value[7] == 6)
//				if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//				{
//					//AfxMessageBox(_T("It's impossible to enter this place!222"));
//					if(nRange==1)//0-10v
//					{
//						//nvalue=product_register_value[102]/100 /10.0 * 100%;
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
//					if(nRange==1)//0-10v
//					{
//						//nvalue=product_register_value[102]/100 /10.0 * 100%;
//						nvalue= product_register_value[MODBUS_HEATING_VALVE]/10.0f;//103   211
//					}
//					if(nRange==2)//0-5v
//					{
//						nvalue= product_register_value[MODBUS_HEATING_VALVE]/5.0f;
//					}
//					if(nRange==3)//2-10v
//					{
//						nvalue= product_register_value[MODBUS_HEATING_VALVE]/8.0f;
//					}
//					if(nRange==4)//10-0v
//					{
//						nvalue=(10- product_register_value[MODBUS_HEATING_VALVE]/100.0f)/10.0f *100;
//					}
//				}
//				strTemp.Format(_T("%.1f%%"),nvalue);
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
//			nRange = product_register_value[MODBUS_MODE_OUTPUT5];
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
//			//if (product_register_value[7] == 6)
//
//
//
//			nRange= product_register_value[MODBUS_OUTPUT2_SCALE];//187  208
//			nValue= product_register_value[MODBUS_HEATING_VALVE];//103  211
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
//			}
//			else
//			{
//				//strTemp.Format(_T("%.1f"),nValue/100.0);
//				float nvalue=0.0;
//				//if (product_register_value[7] == 6)
//				if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//				{
//					if(nRange==1)//0-10v
//					{
//						//nvalue=product_register_value[102]/100 /10.0 * 100%;
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
//					if(nRange==1)//0-10v
//					{
//						//nvalue=product_register_value[102]/100 /10.0 * 100%;
//						nvalue=product_register_value[MODBUS_HEATING_VALVE]/10.0f;
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
//				}
//				strTemp.Format(_T("%.1f%%"),nvalue);
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
//	//if (product_register_value[7] == 6)
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
//}

//Fance 2013 03 28
//由于代码 逻辑混乱，已经重写。
//void COutputSetDlg::Fresh_Grid()
//{
//	int nValueTemp123=5;
//	//m_version=get_curtstat_version();
//	//m_nModeType=product_register_value[7];
//	//if (product_register_value[7] == PM_TSTAT6)
//	if ((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7))
//	{
//		m_nModeType = product_register_value[7];
//	}else
//	{
//		m_nModeType = product_register_value[7];
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
//// 	if (product_register_value[7] == 6)
//// 	{
//// 	
//// 	}else
//	//if (product_register_value[7] == PM_TSTAT6)
//
//	//annul by Fan
//	/*
//	if ((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7))
//	{
//		nVAlue = product_register_value[209];
//	}else
//	{
//		nVAlue=product_register_value[108];
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
//	int nAMVAlue=0;//=product_register_value[310];
//	nAMVAlue = product_register_value[MODBUS_OUTPUT_MANU_ENABLE];
//
//	//annul by Fan  2013-03-28
//	/*
//	if ((product_register_value[7]== PM_TSTAT6)||(product_register_value[7]== PM_TSTAT7))
//		nAMVAlue = product_register_value[254];
//	else
//		nAMVAlue =product_register_value[310];
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
//		int nFun=0;//=product_register_value[328];//tstat6找不到对应的
//		nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; //328   266
//		//annul by Fan
//		/*
//		if ((product_register_value[7] ==PM_TSTAT6)||(product_register_value[7] ==PM_TSTAT7))
//		{
//			nFun = product_register_value[266];
//		}
//		else
//		{
//				nFun = product_register_value[328];		
//		}
//		*/
//		if(nFun>=0&&nFun<4)
//		{
//			strTemp=ONTPUT_FUNS[nFun];
//		}
//		else
//		{
//// 			CString str;
//// 			str.Format(_T("product_register_value[328]=%d"),product_register_value[328]);
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
//		short nFun=0;//product_register_value[328];
//		//328	266	1	int8	1	Low byte	W/R	"Output1 Function setting:
//		//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "
//		nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION];
//		//annul by Fan
//		/*
//		if ((product_register_value[7] ==PM_TSTAT6)||(product_register_value[7] ==PM_TSTAT7))
//			nFun = product_register_value[266];
//		else
//			nFun = product_register_value[328];
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
//		if ((product_register_value[7] ==PM_TSTAT6)||(product_register_value[7] ==PM_TSTAT7))
//		{
//			indext = product_register_value[267];
//		}
//		else
//		{
//				indext = product_register_value[334];
//		}
//		*/
//			
//
//
//		//indext=product_register_value[334];
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
//		short nFun=0;//product_register_value[328];
//		nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; //328  266
//		//Annul by Fan
//		/*
//		if (product_register_value[7] ==PM_TSTAT6)
//			nFun = product_register_value[266];
//		else
//			nFun = product_register_value[328];
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
//		if ((product_register_value[7]==PM_TSTAT6)||(product_register_value[7]==PM_TSTAT7))
//		{
//			indext=product_register_value[268];
//		}else
//		{
//			indext=product_register_value[335];		
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
//		nRange=product_register_value[283];
//		if(nRange>=0&&nRange<3)
//		{
//			strTemp=OUTPUT_RANGE45[nRange];
//		}
//		m_FlexGrid.put_TextMatrix(4,RANG_OUTFIELD,strTemp);
//
//		strTemp.Empty();
//		if (m_version<32.2)
//		{
//			short nFun=product_register_value[328];
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
//			indext=product_register_value[336];
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
//		nRange=product_register_value[284];
//		if(nRange>=0&&nRange<3)
//		{
//			strTemp=OUTPUT_RANGE45[nRange];
//		}
//		m_FlexGrid.put_TextMatrix(5,RANG_OUTFIELD,strTemp);
//
//		if (m_version<32.2)
//		{
//			short nFun=product_register_value[328];
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
//			indext=product_register_value[337];
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
//		//nRange=product_register_value[283];
//		//283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
//
//		nRange = product_register_value[MODBUS_MODE_OUTPUT4];//283  205
//		//Annul by Fan
//		/*
//		if ((product_register_value[7] ==PM_TSTAT6)||(product_register_value[7] ==PM_TSTAT6))
//			nRange = product_register_value[205];
//		else
//			nRange = product_register_value[283];
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
//			//int nValueTemp = product_register_value[348];		//tstat6没找到对应的 //Fance_?_
//	//This Line Annul by Fance		int nValueTemp = product_register_value[244];		//tstat6没找到对应的//0914
//
//// 			if (product_register_value[7] ==PM_TSTAT6)
//// 				nValueTemp = product_register_value[205];
//// 			else
//// 				nValueTemp = product_register_value[348];
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
//			short nFun=0;//product_register_value[328];
//			nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; //328  266
//			//Annul by Fan
//			/*
//			if ((product_register_value[7] ==PM_TSTAT6)||(product_register_value[7] ==PM_TSTAT7))
//				nFun = product_register_value[266];
//			else
//				nFun = product_register_value[328];
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
//			//indext=product_register_value[336];
//			//if (product_register_value[7] ==PM_TSTAT6)
//			indext = product_register_value[MODBUS_OUTPUT4_FUNCTION];
//			//Annul by Fan
//			/*
//			if ((product_register_value[7] ==PM_TSTAT6)||(product_register_value[7] ==PM_TSTAT7))
//			{
//				indext = product_register_value[269];
//			}
//			else
//			{
//					indext = product_register_value[336];
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
//		//nRange=product_register_value[284];
//		//284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM
//		nRange = product_register_value[MODBUS_MODE_OUTPUT5];
//		//Annul by Fan
//		/*
//		if ((product_register_value[7] ==PM_TSTAT6)||(product_register_value[7] ==PM_TSTAT7))
//			nRange = product_register_value[206];
//		else
//			nRange = product_register_value[284];
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
//			short nFun;//=product_register_value[328];
//			nFun= product_register_value[MODBUS_OUTPUT1_FUNCTION];
//			//Annul by Fan
//			/*
//			if ((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7))
//			{
//				nFun=product_register_value[266];
//			}else
//			{
//				 nFun=product_register_value[328];
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
//			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//			{
//				indext=product_register_value[270];
//			}else
//			{
//					indext=product_register_value[337];
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
//		int nRang=0;//product_register_value[283];
//
//		nRang= product_register_value[MODBUS_OUTPUT1_FUNCTION];//328  266
//
//		//if (product_register_value[7] ==PM_TSTAT6)
//		//Annul by Fan
//		/*
//		if ((product_register_value[7] ==PM_TSTAT6)||(product_register_value[7] ==PM_TSTAT7))
//		{
//			nRang = product_register_value[266];
//		}
//		else
//		{
//			nRang = product_register_value[328];
//		}
//		*/
//// 		if (product_register_value[7] == 18)//2.5.0.94
//// 		{
//// 			nRang = product_register_value[310];
//// 		}
//		
//
//		//int nValue;
//		int nValue1;//=product_register_value[108];
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
//		//if (product_register_value[7] == 6)
//		if ((product_register_value[7] ==PM_TSTAT6)||(product_register_value[7] ==PM_TSTAT7))
//		{
//			int num = reg_tststold[108];
//			nValue1=product_register_value[num];
//		}else
//			nValue1=product_register_value[108];
//		
//		//tstat6
//		if ((product_register_value[7] ==PM_TSTAT6)||(product_register_value[7] ==PM_TSTAT7))
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
//				strTemp.Format(_T("%d%%"),product_register_value[348]);
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
//				strTemp.Format(_T("%d%%"),product_register_value[348]);
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
//		if (product_register_value[7]==18)
//		{
//			//int autmanl = product_register_value[310];
//			if(product_register_value[283]==0)
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
//				strTemp.Format(_T("%d%%"),product_register_value[348]);
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
//		//if (product_register_value[7] == 6)
//		/*//By Fan
//		if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//		{
//			nRange=product_register_value[205];
//
//		}else
//		{
//			nRange=product_register_value[283];
//		}
//		*/
//		if(nRange>=0&&nRange<=2)
//		{
//			strTemp=OUTPUT_RANGE45[nRange];
//		}
//
//#if 0
//		if (product_register_value[336] == 3)//2.5.0.96
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
//			short nFun=0;//product_register_value[328];
//			nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION];
//			/*//By Fan
//			//if (product_register_value[7] == PM_TSTAT6)
//			if ((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7))
//				nFun = product_register_value[266];
//			else
//				nFun = product_register_value[328];
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
//				//indext=product_register_value[336];//2.5.0.96
//				indext=product_register_value[283];//2.5.0.98		//Fance_?_   //269 对应 336 这里为什么 要变为 283;
//
//			}
//			//if (product_register_value[7] == 6)
//			/*
//			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//			{
//				indext=product_register_value[269];
//			}else
//			{
//
//				//indext=product_register_value[336];//2.5.0.96
//				indext=product_register_value[283];//2.5.0.98		
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
//		//if (product_register_value[7] == 6)
//		if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//		{
//			nRang=product_register_value[206];
//		}else
//		{
//			nRang=product_register_value[337];		
//		}
//*/
//		
//		//nValue1=product_register_value[108];
//		//108	209	1	Low byte	W/R	Output1 tot 5, bit 0 thru 4 = relay 1 thru 5.
//
//		nValue1 = product_register_value[MODBUS_DIGITAL_OUTPUT_STATUS];//108   209
//		/*//Fan
//		//tstat6
//		if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//		{
//			nValue1 = product_register_value[209];
//		}else
//		{
//			nValue1=product_register_value[108];
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
//			//int autmanl = product_register_value[310];
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
//		//	nRang=product_register_value[284];
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
//		//if (product_register_value[7] == 6)
//		/*
//		if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//		{
//			nRange=product_register_value[206];
//		}else
//		{
//			nRange=product_register_value[284];
//
//		}
//		*/
//		if(nRange>=0&&nRange<=2)
//		{
//			strTemp=OUTPUT_RANGE45[nRange];
//		}
////2.5.0.98
//// 		if (product_register_value[337]== 3)
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
//			short nFun;//=product_register_value[328];
//			nFun = product_register_value[MODBUS_OUTPUT1_FUNCTION]; // 328  266;
//			//if (product_register_value[7] == 6)
//			/*
//			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//			{
//				nFun=product_register_value[266];
//			}else
//			{
//				 nFun=product_register_value[328];
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
//				//indext=product_register_value[337];//2.5.0.96
//				indext=product_register_value[284];//2.5.0.98			//Fance_?_  为什么要不对应;
//
//			}
//
//			//if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//			//{
//			//	indext=product_register_value[270];
//			//}else
//			//{
//			//	//indext=product_register_value[337];//2.5.0.96
//			//	indext=product_register_value[284];//2.5.0.98
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
//			short nRange;//=product_register_value[186];
//			int nValue;//e=product_register_value[102];
//			//if (product_register_value[7] == 6)
//			nRange = product_register_value[MODBUS_OUTPUT1_SCALE]; //186  207
//			nValue = product_register_value[MODBUS_COOLING_VALVE]; //102  210
//			/*
//			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//			{
//				nRange=product_register_value[207];
//				nValue=product_register_value[210];
//			}else
//			{
//				nRange=product_register_value[186];
//				nValue=product_register_value[102];
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
//					//strTemp.Format(_T("%.1f"),product_register_value[102]/100.0);
//				float nvalue=0.0;
//				//if (product_register_value[7] == 6)
//				if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//				{
//					AfxMessageBox(_T("It's impossible to enter this place!"));
//					if(nRange==1)//0-10v
//					{
//						//nvalue=product_register_value[102]/100 /10.0 * 100%;
//						nvalue= product_register_value[MODBUS_COOLING_VALVE]/10.0f;//  T6=210
//					}
//					if(nRange==2)//0-5v
//					{
//						nvalue=product_register_value[210]/5.0f;
//					}
//					if(nRange==3)//2-10v
//					{
//						nvalue=product_register_value[210]/8.0f;
//					}
//					if(nRange==3)//10-0v
//					{
//						nvalue=(10-product_register_value[210]/100.0f)/10.0f *100;
//					}
//				}
//				else
//				{
//
//
//					if(nRange==1)//0-10v
//					{
//						//nvalue=product_register_value[102]/100 /10.0 * 100%;
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
//			//if (product_register_value[7] == 6)
//			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//			{
//				nRange=product_register_value[208];
//				nValue=product_register_value[211];
//			}else
//			{
//				nRange=product_register_value[187];
//				nValue=product_register_value[103];
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
//				//if (product_register_value[7] == 6)
//				if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//				{
//					AfxMessageBox(_T("It's impossible to enter this place!222"));
//					if(nRange==1)//0-10v
//					{
//						//nvalue=product_register_value[102]/100 /10.0 * 100%;
//						nvalue=product_register_value[211]/10.0f;
//					}
//					if(nRange==2)//0-5v
//					{
//						nvalue=product_register_value[211]/5.0f;
//					}
//					if(nRange==3)//2-10v
//					{
//						nvalue=product_register_value[211]/8.0f;
//					}
//					if(nRange==4)//10-0v
//					{
//						nvalue=(10-product_register_value[211]/100.0f)/10.0f *100;
//					}
//				}else
//				{
//						if(nRange==1)//0-10v
//						{
//							//nvalue=product_register_value[102]/100 /10.0 * 100%;
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
//			//if (product_register_value[7] == 6)
//
//
//
//			nRange= product_register_value[MODBUS_OUTPUT2_SCALE];//187  208
//			nValue= product_register_value[MODBUS_HEATING_VALVE];//103  211
//
//			//if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//			//{
//			//	nRange=product_register_value[208];
//			//	nValue=product_register_value[211];
//			//}else
//			//{
//			//	nRange=product_register_value[187];
//			//	nValue=product_register_value[103];
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
//				//if (product_register_value[7] == 6)
//				if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
//				{
//					if(nRange==1)//0-10v
//					{
//						//nvalue=product_register_value[102]/100 /10.0 * 100%;
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
//							//nvalue=product_register_value[102]/100 /10.0 * 100%;
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
//	//if (product_register_value[7] == 6)
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
	/*
	 When Transducer
	 Model=1,2,3 [Internal ,setpoint ,AI1]
	 Model=4,12,18,17[Internal ,setpoint ,AI1,AI2,AI3]
	 Model=5E[Internal ,setpoint ,AI1,AI2,AI3,AI4,AI5,AI6,AI7,AI8]
	 Model=6,7[Internal ,setpoint ,AI1,AI2,AI3,AI4,AI5,AI6,AI7,AI8,HUM,CO2]
	*/

 #if 1//Tstat67
	if ((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)

		)
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
			// 					int nTempValue = product_register_value[254];
			// 					if(!(nTempValue & 0x08))	 // A/M，如选择Auto，不动，否则看Range
			// 						m_OutValueCmbox.ShowWindow(SW_HIDE);	
			// 					else
			// 					{
			// 						//283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
			// 
			// 						int nTempValue = product_register_value[205];
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
				//	int nTempValue = product_register_value[254];		//Marked by Fance 2013 04 11
				int nTempValue = product_register_value[MODBUS_OUTPUT_MANU_ENABLE];  //T5=310  //T6=254
				DWORD dwFlag = 0x01;
				bool bRowAuto =MKBOOL(nTempValue & (dwFlag << (lRow-1)));

				if(!bRowAuto)	 // A/M，如选择Auto，不动，否则看Range
					m_OutValueCmbox.ShowWindow(SW_HIDE);	
				else
				{
					//	283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
					//	284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM

					int nTempValue = product_register_value[205+lRow-4];//180找不到对应的tstat6
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
			else
			{
				int nTempValue = product_register_value[MODBUS_OUTPUT1_SCALE+lRow-6];//180找不到对应的tstat6
				if ( nTempValue != 0 )
				{
					m_OutValueCmbox.ShowWindow(SW_HIDE);
					m_OutValueEdt.ShowWindow(SW_SHOW);	
					m_OutValueEdt.SetWindowText(strValue);
					m_OutValueEdt.MoveWindow(rc); //移动到选中格的位置，覆盖
					m_OutValueEdt.BringWindowToTop();
					m_OutValueEdt.SetFocus(); //获取焦点
				}


			    
// 				nRange= product_register_value[MODBUS_OUTPUT2_SCALE];//187  208
// 				nValue= product_register_value[MODBUS_HEATING_VALVE];//103  211
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
		m_outRangCmbox.ResetContent();
		if(lRow<=5)
		{
			 


			//	for(int i=0;i<2;i++)
			 

			if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9)
				|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
				)
			{
				for(int i=0;i<7;i++)//2.5.0.98
				{
					m_outRangCmbox.AddString(OUTPUT_RANGE45[i]);
				}
			} 
			else
			{
				for(int i=0;i<3;i++)//2.5.0.98
				{
					m_outRangCmbox.AddString(OUTPUT_RANGE5[i]);
				}
			}
	

		}
		if((lRow==6||lRow==7))//model DEG TStat6,TStat7
		{

			if (lRow==6)
			{
			 
			 
					for(int i=0;i<6;i++)//2.5.0.98
					{
						m_outRangCmbox.AddString(OUTPUT_ANRANGE6[i]);
					}
			 

			}
			else if(lRow==7)
			{
				 
				 
					for(int i=0;i<6;i++)//2.5.0.98
					{
						m_outRangCmbox.AddString(OUTPUT_ANRANGE6[i]);
					}
				 
			}
			else
				return;
		}
		m_outRangCmbox.ShowWindow(SW_SHOW);//显示控件
		m_outRangCmbox.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_outRangCmbox.BringWindowToTop();
		m_outRangCmbox.SelectString(-1,strValue);
		m_outRangCmbox.SetFocus(); //获取焦点
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
		else
		{
			m_outFunCmbox.ResetContent();
			//for(int i=0;i<4;i++)
			for(int i=0;i<5;i++)//2.5.0.98
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
	if(lCol==INTER_LOCK)
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
	if(lCol==SIGUAL_TYPE)
	{
		m_combox_signal_type.ResetContent();
		if(lRow<=5)
		{
				return;
		}
		if((lRow==6||lRow==7))//model DEG TStat6,TStat7
		{

			if (lRow==6)
			{
				if (product_register_value[MODBUS_OUTPUT6_FUNCTION]==4)//Transducer
				{
					for(int i=5;i<17;i++)//2.5.0.98
					{
						m_combox_signal_type.AddString(OUTPUT_ANRANGE[i]);
					}
				}
				else
				{
					return;
				}

			}
			else if(lRow==7)
			{
				if (product_register_value[MODBUS_OUTPUT7_FUNCTION]==4)//Transducer
				{
					for(int i=5;i<17;i++)//2.5.0.98
					{
						m_combox_signal_type.AddString(OUTPUT_ANRANGE[i]);
					}
				}
				else
				{
					 return;
				}
			}
			else
				return;
		}
		m_combox_signal_type.ShowWindow(SW_SHOW);//显示控件
		m_combox_signal_type.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_combox_signal_type.BringWindowToTop();
		m_combox_signal_type.SelectString(-1,strValue);
		m_combox_signal_type.SetFocus(); //获取焦点
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
			if((m_nModeType==16||m_nModeType==PM_PM5E || m_nModeType == PM_PM5E_ARM) && lRow==4)
			{
				int nTempValue = product_register_value[310];
				if(!(nTempValue & 0x08))	 // A/M，如选择Auto，不动，否则看Range
					m_OutValueCmbox.ShowWindow(SW_HIDE);	
				else
				{
					int nTempValue = product_register_value[283];
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

			if((product_register_value[7]==PM_TSTAT5D||product_register_value[7]==PM_TSTAT5E||product_register_value[7] == PM_PM5E|| product_register_value[7] == PM_PM5E_ARM || (product_register_value[7]==PM_TSTATRUNAR) ||
				(product_register_value[7] == PM_TSTAT5G)) && lRow>5)
			{
				int nTempValue = product_register_value[310];
				DWORD dwFlag = 0x01;
				bool bRowAuto =MKBOOL(nTempValue & (dwFlag << (lRow-1)));
				
				if(!bRowAuto)	 // A/M，如选择Auto，不动，否则看Range
					m_OutValueCmbox.ShowWindow(SW_HIDE);	
				else
				{
					int nTempValue = product_register_value[180+lRow];
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
	

			if((m_nModeType==1||m_nModeType==3||m_nModeType==2)&&lRow==4)
			{
				if(product_register_value[186]==0)//ON/oFF
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

			if((m_nModeType==1||m_nModeType==3||m_nModeType==2)&&lRow==5)
			{
				if(product_register_value[187]==0)//ON/oFF
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
				//if(product_register_value[283]==0)//ON/oFF
				if(product_register_value[283]==0)//ON/oFF
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
				if(product_register_value[284]==0)//ON/oFF
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
// 				if(product_register_value[283]==0)//ON/oFF
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
// 				if(product_register_value[284]==0)//ON/oFF
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




	
			if((m_nModeType==12||m_nModeType==16||m_nModeType==PM_PM5E|| m_nModeType == PM_PM5E_ARM || m_nModeType==18||m_nModeType==PM_PRESSURE)&&lRow==6)
			{
				if((m_nModeType==17||m_nModeType==18)&&lRow==4)
			{
				if(product_register_value[283]==0)//ON/oFF
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
				if(product_register_value[284]==0)//ON/oFF
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
			if(product_register_value[186]==0)//ON/oFF
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
			
			if((m_nModeType==12||m_nModeType==16||m_nModeType==PM_PM5E|| m_nModeType == PM_PM5E_ARM || m_nModeType==18||m_nModeType==PM_PRESSURE)&&lRow==7)
			{
				if(product_register_value[187]==0)//ON/oFF
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
	{   m_outRangCmbox.ResetContent();
		if (product_register_value[7]==1||product_register_value[7]==2||product_register_value[7]==3)
		{
			if (m_nCurRow<=3)
			{
				for(int i=0;i<3;i++)//2.5.0.98
				{
					m_outRangCmbox.AddString(OUTPUT_RANGE5[i]);
				}
			} 
			else 
			{
				if (m_nCurRow==4)
				{
					if (product_register_value[MODBUS_OUTPUT6_FUNCTION]==4)//Transducer
					{
						for(int i=5;i<8;i++)//2.5.0.98
						{
							m_outRangCmbox.AddString(OUTPUT_ANRANGE[i]);
						}
					}
					else
					{
						for(int i=0;i<4;i++)//2.5.0.98
						{
							m_outRangCmbox.AddString(OUTPUT_ANRANGE[i]);
						}
					}
					
				}
				else if(m_nCurRow==5)
					{
						if (product_register_value[MODBUS_OUTPUT7_FUNCTION]==4)//Transducer
						{
							for(int i=5;i<8;i++)//2.5.0.98
							{
								m_outRangCmbox.AddString(OUTPUT_ANRANGE[i]);
							}
						}
						else
						{
							for(int i=0;i<4;i++)//2.5.0.98
							{
								m_outRangCmbox.AddString(OUTPUT_ANRANGE[i]);
							}
						}
					}
			    else
					return;
			}
		} 
		else if (product_register_value[7]==4||product_register_value[7]==12||product_register_value[7]==17||product_register_value[7]==18)
		{

			if (m_nCurRow<=5)
			{
				for(int i=0;i<3;i++)//2.5.0.98
				{
					m_outRangCmbox.AddString(OUTPUT_RANGE5[i]);
				}
			} 
			else 
			{
				if (m_nCurRow==6)
				{
					if (product_register_value[MODBUS_OUTPUT6_FUNCTION]==4)//Transducer
					{
						for(int i=5;i<10;i++)//2.5.0.98
						{
							m_outRangCmbox.AddString(OUTPUT_ANRANGE[i]);
						}
					}
					else
					{
						for(int i=0;i<5;i++)//2.5.0.98
						{
							m_outRangCmbox.AddString(OUTPUT_ANRANGE[i]);
						}
					}

				}
				else if(m_nCurRow==7)
				{
					if (product_register_value[MODBUS_OUTPUT7_FUNCTION]==4)//Transducer
					{
						for(int i=5;i<10;i++)//2.5.0.98
						{
							m_outRangCmbox.AddString(OUTPUT_ANRANGE[i]);
						}
					}
					else
					{
						for(int i=0;i<5;i++)//2.5.0.98
						{
							m_outRangCmbox.AddString(OUTPUT_ANRANGE[i]);
						}
					}
				}
				else
					return;
			}
		}
		else if (product_register_value[7]==PM_TSTAT5E||product_register_value[7] == PM_PM5E|| product_register_value[7] == PM_PM5E_ARM || (product_register_value[7]==PM_TSTATRUNAR))
		{
			if (m_nCurRow<=5)
			{
				for(int i=0;i<3;i++)//2.5.0.98
				{
					m_outRangCmbox.AddString(OUTPUT_RANGE5[i]);
				}
			} 
			else 
			{
				if (m_nCurRow==6)
				{
					if (product_register_value[MODBUS_OUTPUT6_FUNCTION]==4)//Transducer
					{
						for(int i=5;i<15;i++)//2.5.0.98
						{
							m_outRangCmbox.AddString(OUTPUT_ANRANGE[i]);
						}
					}
					else
					{
						for(int i=0;i<5;i++)//2.5.0.98
						{
							m_outRangCmbox.AddString(OUTPUT_ANRANGE[i]);
						}
					}

				}
				else if(m_nCurRow==7)
				{
					if (product_register_value[MODBUS_OUTPUT7_FUNCTION]==4)//Transducer
					{
						for(int i=5;i<15;i++)//2.5.0.98
						{
							m_outRangCmbox.AddString(OUTPUT_ANRANGE[i]);
						}
					}
					else
					{
						for(int i=0;i<5;i++)//2.5.0.98
						{
							m_outRangCmbox.AddString(OUTPUT_ANRANGE[i]);
						}
					}
				}
				else
					return;
			}
		}

		m_outRangCmbox.ShowWindow(SW_SHOW);//显示控件
		m_outRangCmbox.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_outRangCmbox.BringWindowToTop();
		m_outRangCmbox.SelectString(-1,strValue);
		m_outRangCmbox.SetFocus(); 
		//获取焦点
		/*
			if(lRow<=5)
			{
				
				if (product_register_value[7]==7||product_register_value[7]==6)
				{
					for(int i=0;i<4;i++)//2.5.0.98
					{
						m_outRangCmbox.AddString(OUTPUT_RANGE45[i]);
					}
				} 
				else
				{
					for(int i=0;i<3;i++)//2.5.0.98
					{
						m_outRangCmbox.AddString(OUTPUT_RANGE5[i]);
					}
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
			*/
	}
 

	if(lCol==FUN_OUTFIELD)
	{
#if 1//2.5.0.98
		if (m_nModeType==2||m_nModeType==3||m_nModeType==1)
		{
			if(lRow>=1&&lRow<=3)
			{
				m_outFunCmbox.ResetContent();
				for(int i=0;i<4;i++)
				{
					m_outFunCmbox.AddString(ONTPUT_FUNS[i]);

				}
				m_outFunCmbox.ShowWindow(SW_SHOW);//显示控件
				m_outFunCmbox.MoveWindow(rc); //移动到选中格的位置，覆盖
				m_outFunCmbox.BringWindowToTop();
				m_outFunCmbox.SelectString(-1,strValue);
				m_outFunCmbox.SetFocus(); //获取焦点
			}
			else if(lRow==4||lRow==5)
			{
				m_outFunCmbox.ResetContent();
				for(int i=0;i<5;i++)
					//for(int i=0;i<3;i++)//2.5.0.98
				{
					m_outFunCmbox.AddString(ONTPUT_FUNS[i]);

				}
				m_outFunCmbox.ShowWindow(SW_SHOW);//显示控件
				m_outFunCmbox.MoveWindow(rc); //移动到选中格的位置，覆盖
				m_outFunCmbox.BringWindowToTop();
				m_outFunCmbox.SelectString(-1,strValue);
				m_outFunCmbox.SetFocus(); //获取焦点
			}
			else
			{
				return;
			}
		} 
		else
		{
			if(lRow>=1&&lRow<=5)
			{
				m_outFunCmbox.ResetContent();
				for(int i=0;i<4;i++)
					//for(int i=0;i<3;i++)//2.5.0.98
				{
					m_outFunCmbox.AddString(ONTPUT_FUNS[i]);

				}
				m_outFunCmbox.ShowWindow(SW_SHOW);//显示控件
				m_outFunCmbox.MoveWindow(rc); //移动到选中格的位置，覆盖
				m_outFunCmbox.BringWindowToTop();
				m_outFunCmbox.SelectString(-1,strValue);
				m_outFunCmbox.SetFocus(); //获取焦点
			}
			else
			{
				m_outFunCmbox.ResetContent();
				for(int i=0;i<5;i++)
					//for(int i=0;i<3;i++)//2.5.0.98
				{
					m_outFunCmbox.AddString(ONTPUT_FUNS[i]);

				}
				m_outFunCmbox.ShowWindow(SW_SHOW);//显示控件
				m_outFunCmbox.MoveWindow(rc); //移动到选中格的位置，覆盖
				m_outFunCmbox.BringWindowToTop();
				m_outFunCmbox.SelectString(-1,strValue);
				m_outFunCmbox.SetFocus(); //获取焦点
			}
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
	if(m_nModeType==1||m_nModeType==4||m_nModeType==12||m_nModeType==16  ||m_nModeType==PM_PM5E || m_nModeType == PM_PM5E_ARM
		||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7||m_nModeType==PM_TSTAT5i||m_nModeType==PM_TSTAT8 || m_nModeType == PM_TSTAT9 ||m_nModeType==PM_PRESSURE
		|| (m_nModeType == PM_TSTAT8_WIFI) || (m_nModeType == PM_TSTAT8_OCC) || (m_nModeType == PM_TSTAT7_ARM) || (m_nModeType == PM_TSTAT8_220V)
		) 
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

	if((m_nModeType==1||m_nModeType==3||m_nModeType==2))//A
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
	if(m_nModeType==12||m_nModeType==16||m_nModeType==18||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT5i||m_nModeType==PM_TSTAT8 || m_nModeType == PM_TSTAT9
		|| (m_nModeType == PM_TSTAT8_WIFI) || (m_nModeType == PM_TSTAT8_OCC) || (m_nModeType == PM_TSTAT7_ARM) || (m_nModeType == PM_TSTAT8_220V)
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
	if(m_nModeType==1||m_nModeType==4||m_nModeType==12||m_nModeType==16||m_nModeType==PM_PM5E|| m_nModeType == PM_PM5E_ARM || m_nModeType==17
		||m_nModeType==18||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7||m_nModeType==PM_TSTAT5i||m_nModeType==PM_TSTAT8 || m_nModeType == PM_TSTAT9
		|| (m_nModeType == PM_TSTAT8_WIFI) || (m_nModeType == PM_TSTAT8_OCC) || (m_nModeType == PM_TSTAT7_ARM) || (m_nModeType == PM_TSTAT8_220V) 
		||m_nModeType==PM_PRESSURE)
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
	if((m_nModeType==1||m_nModeType==3||m_nModeType==2))
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
	if(m_nModeType==12||m_nModeType==16||m_nModeType==18||m_nModeType==PM_PM5E || m_nModeType == PM_PM5E_ARM
		||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT7||m_nModeType==PM_TSTAT5i||m_nModeType==PM_TSTAT8 || m_nModeType == PM_TSTAT9
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V) 
		||m_nModeType==PM_PRESSURE)
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
    m_crange=nIndext;
 

	if (product_register_value[7]==1||product_register_value[7]==2||product_register_value[7]==3)
	{
		if (m_nCurRow<=3)
		{
			ret = write_one(g_tstat_id,MODBUS_MODE_OUTPUT1+(m_nCurRow-1) ,nIndext);	//280  202
			if (ret<=0)
			{
				AfxMessageBox(_T("Setting failure!"));
			}
			else
			{
				product_register_value[MODBUS_MODE_OUTPUT1+(m_nCurRow-1)] = nIndext;//280 202
			}
		} 
		else 
		{
			if (m_nCurRow==4)
			{
				if (product_register_value[MODBUS_OUTPUT6_FUNCTION]==4)//Transducer
				{
					
					ret =0;
					nIndext+=5;
					ret = write_one(g_tstat_id,MODBUS_OUTPUT1_SCALE ,nIndext ); //186   207
					if (ret<=0)
						AfxMessageBox(_T("Setting failure!"));
					else
						product_register_value[MODBUS_OUTPUT1_SCALE] = nIndext;
					
					 
				}
				else
				{ 
					ret =0;
					ret = write_one(g_tstat_id,MODBUS_OUTPUT1_SCALE ,nIndext ); //186   207
					if (ret<=0)
						AfxMessageBox(_T("Setting failure!"));
					else
						product_register_value[MODBUS_OUTPUT1_SCALE] = nIndext;
				}

				

			}
			else if(m_nCurRow==5)
			{
				if (product_register_value[MODBUS_OUTPUT7_FUNCTION]==4)//Transducer
				{
					ret =0;
					nIndext+=5;
					ret = write_one(g_tstat_id,MODBUS_OUTPUT2_SCALE ,nIndext ); //186   207
					if (ret<=0)
						AfxMessageBox(_T("Setting failure!"));
					else
						product_register_value[MODBUS_OUTPUT2_SCALE] = nIndext;
				}
				else
				{
					ret =0;
					ret = write_one(g_tstat_id,MODBUS_OUTPUT2_SCALE ,nIndext ); //186   207
					if (ret<=0)
						AfxMessageBox(_T("Setting failure!"));
					else
						product_register_value[MODBUS_OUTPUT2_SCALE] = nIndext;
				}
			}
			else
				return;
		}
	} 
	else 	
	{
		int pwmno=0;
		if (m_nCurRow<=5)
		{
			for (int i=0;i<5;i++)
			{
				if (product_register_value[MODBUS_MODE_OUTPUT1+i]==3)
				{
					++pwmno;
				}	
			}
			if (pwmno>=2)
			{
				if (nIndext==3)
				{
					AfxMessageBox(_T("PWM Modes that you selected can't be more than two!"));
					return;
				}
			}
			if (m_crange==0||m_crange==4||m_crange==5||m_crange==6)
			{
				nIndext=0;
			}
			ret = write_one(g_tstat_id,MODBUS_MODE_OUTPUT1+(m_nCurRow-1) ,nIndext );	//280  202
			if (ret<=0)
			{
				AfxMessageBox(_T("Setting failure!"));
			}
			else
			{
				product_register_value[MODBUS_MODE_OUTPUT1+(m_nCurRow-1)] = nIndext;//280 202


				CppSQLite3DB SqliteDBBuilding;
				SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
				CppSQLite3Table table;
				CppSQLite3Query q;
				CString sql;
				sql.Format(_T("Select * from Value_Range where CInputNo=%d%d and SN=%d"),m_nCurRow,m_nCurRow,m_sn);
				q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);

				if (!q.eof())//有表但是没有对应序列号的值
				{
					sql.Format(_T("update Value_Range set CRange = %d where CInputNo=%d%d and SN=%d "),m_crange,m_nCurRow,m_nCurRow,m_sn);
					 SqliteDBBuilding.execDML((UTF8MBSTR)sql);
				}
				else
				{
					sql.Format(_T("Insert into Value_Range ( SN,CInputNo,CRange) values('%d','%d%d','%d')"),m_sn,m_nCurRow,m_nCurRow,m_crange);
					SqliteDBBuilding.execDML((UTF8MBSTR)sql);
				}
				SqliteDBBuilding.closedb();

			}
		} 
		else 
		{
			if (m_nCurRow==6)
			{
				if (product_register_value[MODBUS_OUTPUT6_FUNCTION]==4)//Transducer
				{

					ret =0;
					nIndext+=5;
					ret = write_one(g_tstat_id,MODBUS_OUTPUT1_SCALE ,nIndext ); //186   207
					if (ret<=0)
						AfxMessageBox(_T("Setting failure!"));
					else
						product_register_value[MODBUS_OUTPUT1_SCALE] = nIndext;


				}
				else
				{
					if (nIndext==4)
					{
						nIndext=17;
					}
					ret =0;
					ret = write_one(g_tstat_id,MODBUS_OUTPUT1_SCALE ,nIndext ); //186   207
					if (ret<=0)
						AfxMessageBox(_T("Setting failure!"));
					else
						product_register_value[MODBUS_OUTPUT1_SCALE] = nIndext;
				}



			}
			else if(m_nCurRow==7)
			{
				if (product_register_value[MODBUS_OUTPUT7_FUNCTION]==4)//Transducer
				{
					ret =0;
					nIndext+=5;
					ret = write_one(g_tstat_id,MODBUS_OUTPUT2_SCALE ,nIndext ); //186   207
					if (ret<=0)
						AfxMessageBox(_T("Setting failure!"));
					else
						product_register_value[MODBUS_OUTPUT2_SCALE] = nIndext;
				}
				else
				{
					ret =0;
					if (nIndext==4)
					{
						nIndext=17;
					}
					ret = write_one(g_tstat_id,MODBUS_OUTPUT2_SCALE ,nIndext ); //186   207
					if (ret<=0)
						AfxMessageBox(_T("Setting failure!"));
					else
						product_register_value[MODBUS_OUTPUT2_SCALE] = nIndext;
				}
			}
			else
				return;
		}
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
//				product_register_value[280+(m_nCurRow-1)] = nIndext;
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
//				product_register_value[186] = nIndext;
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
//					product_register_value[187] = nIndext;
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
//	if ((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7))
//	{
//		if(m_nCurRow>0&&m_nCurRow<=5)
//		{	
//			ret = write_one(g_tstat_id,202+(m_nCurRow-1) ,nIndext );
//			if (ret>0)
//				product_register_value[202+m_nCurRow-1] = nIndext;
//		}
//
//		if(m_nCurRow==6)
//		{
//			ret = 0;
//			ret = write_one(g_tstat_id,207 ,nIndext );
//			if (ret>0)
//				product_register_value[207] = nIndext;
//
//		}
//		if(m_nCurRow==7)
//		{
//			ret = 0;
//			ret = write_one(g_tstat_id,208 ,nIndext );
//			if (ret>0)
//				product_register_value[207] = nIndext;
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
	//short nFun;//=product_register_value[328];
	CString strTemp;
	int nAMVAlue=0;
	//328	266	1	Low byte	W/R	"Output1 Function setting:
	//	0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "


// 
//		280	1	Determine the output1 mode. Output1 always is ON/OFF mode
// 		281	1	Determine the output2 mode. Output2 always is ON/OFF mode
// 		282	1	Determine the output3 mode. Output3 always is ON/OFF mode
// 		283	1	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
// 		284	1	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM
/*
	if (product_register_value[7]==6)
	{
		 nFun=product_register_value[266];
	}
	else
	{
		nFun=product_register_value[328];
	}
	if(m_nCurRow==1)
	{		
		//if (product_register_value[7] == 6)
		if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
		{
			write_one(g_tstat_id,266,nItem);
			product_register_value[266] = nItem;
		}else
		{
// 			if (product_register_value[7] == 18)//2.5.0.96
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
					if ((nItem == 3)&&(product_register_value[7] == 18))
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

			//if (product_register_value[7] == 6)
			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
			{
				write_one(g_tstat_id,266,nFun);
				product_register_value[266] = nFun;

			}
			else
			{
				 write_one(g_tstat_id,328,nFun);
	
			}
	
		}//334	267	1	Low byte	W/R	Output2 function setting (see above)

		else
		{
			//if (product_register_value[7] == 6)
			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
			{
				write_one(g_tstat_id,267,nItem);
				product_register_value[267] = nItem;

			}else
			{
// 				if (product_register_value[7] == 18)//2.5.0.96
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
						if ((nItem == 3)&&(product_register_value[7] == 18))
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
			//if (product_register_value[7] == 6)
			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
			{
				write_one(g_tstat_id,266,nFun);
				product_register_value[266] = nFun;

			}else
			{
				write_one(g_tstat_id,328,nFun);
			}

			
		}
		else
		{
			
			//335	268	1	Low byte	W/R	Output3 function setting (see above)

			//if (product_register_value[7] == 6)
			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
			{
				write_one(g_tstat_id,268,nItem);
				product_register_value[268] = nItem;

			}else
			{
// 				if (product_register_value[7] == 18)
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
					if ((nItem == 3)&&(product_register_value[7] == 18))
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
		if(m_nModeType==2)
		{

			short nRange;//=product_register_value[186];
			int nValue;//e=product_register_value[102];


			nRange=product_register_value[186];
			nValue=product_register_value[102];



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
				//strTemp.Format(_T("%.1f"),product_register_value[102]/100.0);
				float nvalue=0.0;
				//if (product_register_value[7] == 6)



				if(nRange==1)//0-10v
				{
					//nvalue=product_register_value[102]/100 /10.0 * 100%;
					nvalue=product_register_value[102]/10.0f;
				}
				if(nRange==2)//0-5v
				{
					nvalue=product_register_value[102]/5.0f;
				}
				if(nRange==3)//2-10v
				{
					nvalue=product_register_value[102]/8.0f;
				}
				if(nRange==3)//10-0v
				{
					nvalue=(10-product_register_value[102]/100.0f)/10.0f *100;
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


			//103	211	2	Full	W/R(write only when manual output7 enable)	Output7 Analog output2, a number from 0-1000 representing 0% (closed) to 100% (open). When Range = On/Off mode, On=1000, Off=0.    
			//187	208	1	Low byte	W/R	Analog Output2 range - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 




			nRange=product_register_value[187];
			nValue=product_register_value[103];


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
				//if (product_register_value[7] == 6)

				{
					if(nRange==1)//0-10v
					{
						//nvalue=product_register_value[102]/100 /10.0 * 100%;
						nvalue=product_register_value[103]/10.0f;
					}
					if(nRange==2)//0-5v
					{
						nvalue=product_register_value[103]/5.0f;
					}
					if(nRange==3)//2-10v
					{
						nvalue=product_register_value[103]/8.0f;
					}
					if(nRange==4)//10-0v
					{
						nvalue=(10-product_register_value[103]/100.0f)/10.0f *100;
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
		}

		else
		{

			//if (product_register_value[7] == 6)
			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
			{
				write_one(g_tstat_id,269,nItem);
				product_register_value[269] = nItem;
			}else
			{
// 				if (product_register_value[7] == 18)//2.5.0.96
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
			//if (product_register_value[7] == 6)
			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
			{
				write_one(g_tstat_id,266,nFun);
				product_register_value[266] = nFun;
			}else
			{
				write_one(g_tstat_id,328,nFun);

			}
			
		}
		else
		{//337	270	1	Low byte	W/R	Output5 function setting (see above)

			//if (product_register_value[7] ==6)
			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
			{
				write_one(g_tstat_id,270,nItem);
				product_register_value[270] = nItem;
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

			}

			
		}
	}

	 */
	if (m_nModeType==1||m_nModeType==2||m_nModeType==3)//5A,5B
	{
		if (m_nCurRow<=3)
		{
			if (m_nCurRow==1)
			{
				int ret=write_one(g_tstat_id,MODBUS_OUTPUT1_FUNCTION,nItem);
				if (ret>0)
				{
					product_register_value[MODBUS_OUTPUT1_FUNCTION]=nItem;
				} 
			} 
			else if(m_nCurRow==2)
			{
				int ret=write_one(g_tstat_id,MODBUS_OUTPUT2_FUNCTION,nItem);
				if (ret>0)
				{
					product_register_value[MODBUS_OUTPUT2_FUNCTION]=nItem;
				} 
			}
			else
			{
				int ret=write_one(g_tstat_id,MODBUS_OUTPUT3_FUNCTION,nItem);
				if (ret>0)
				{
					product_register_value[MODBUS_OUTPUT3_FUNCTION]=nItem;
				} 
			}
			
			 
		} 
		else if(m_nCurRow==4||m_nCurRow==5)
		{
			if (m_nCurRow==4)
			{
				int ret=write_one(g_tstat_id,MODBUS_OUTPUT6_FUNCTION,nItem);
				if (ret>0)
				{
					product_register_value[MODBUS_OUTPUT6_FUNCTION]=nItem;
				} 
			}
			else
			{
				int ret=write_one(g_tstat_id,MODBUS_OUTPUT7_FUNCTION,nItem);
				if (ret>0)
				{
					product_register_value[MODBUS_OUTPUT7_FUNCTION]=nItem;
				}
			}
			
		}
		else
		{
			return;
		}
	} 
	else
	{
		if (m_nCurRow==1)
		{
			int ret=write_one(g_tstat_id,MODBUS_OUTPUT1_FUNCTION,nItem);
			if (ret>0)
			{
				product_register_value[MODBUS_OUTPUT1_FUNCTION]=nItem;
			} 
		} 
		if (m_nCurRow==2)
		{
			int ret=write_one(g_tstat_id,MODBUS_OUTPUT2_FUNCTION,nItem);
			if (ret>0)
			{
				product_register_value[MODBUS_OUTPUT2_FUNCTION]=nItem;
			} 
		} 
		if (m_nCurRow==3)
		{
			int ret=write_one(g_tstat_id,MODBUS_OUTPUT3_FUNCTION,nItem);
			if (ret>0)
			{
				product_register_value[MODBUS_OUTPUT3_FUNCTION]=nItem;
			} 
		} 
		if (m_nCurRow==4)
		{
			int ret=write_one(g_tstat_id,MODBUS_OUTPUT4_FUNCTION,nItem);
			if (ret>0)
			{
				product_register_value[MODBUS_OUTPUT4_FUNCTION]=nItem;
			} 
		} 
		if (m_nCurRow==5)
		{
			int ret=write_one(g_tstat_id,MODBUS_OUTPUT5_FUNCTION,nItem);
			if (ret>0)
			{
				product_register_value[MODBUS_OUTPUT5_FUNCTION]=nItem;
			} 
		} 
		if (m_nCurRow==6)
		{
			int ret=write_one(g_tstat_id,MODBUS_OUTPUT6_FUNCTION,nItem);
			if (ret>0)
			{
				product_register_value[MODBUS_OUTPUT6_FUNCTION]=nItem;
			} 
		} 
		if (m_nCurRow==7)
		{
			int ret=write_one(g_tstat_id,MODBUS_OUTPUT7_FUNCTION,nItem);
			if (ret>0)
			{
				product_register_value[MODBUS_OUTPUT7_FUNCTION]=nItem;
			} 
		} 

	}

	Fresh_Grid();
	
}

void COutputSetDlg::OnEnKillfocusValueedit()
{
	BeginWaitCursor();
	if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8) || (product_register_value[7] == PM_TSTAT9)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
	{
		CString strText;
		m_OutValueEdt.GetWindowText(strText);
		if(strText.IsEmpty())
			return;
		int nValue=_wtof(strText);
	 
// 		if( nValue > 100 || nValue < 0)
// 		{
// 			AfxMessageBox(_T("Please enter a value between 0 and 100!"), MB_ICONWARNING);
// 			m_OutValueEdt.SetFocus();
// 			return;
// 		}
		int nSrcValue = (unsigned short)nValue;
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
			//int nRange = product_register_value[186];
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
	//		int nRange = product_register_value[207];
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

		if((m_nModeType==1||m_nModeType==3||m_nModeType==2))
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
			if(m_nCurRow==4/*&&product_register_value[283]!=0*/)//ON/oFF
			{
				write_one(g_tstat_id,MODBUS_PWM_OUT4,nValue);	//348
			}
			if(m_nCurRow==5)//ON/oFF
			{
				write_one(g_tstat_id,MODBUS_PWM_OUT5,nValue);//349
			}

		}
		if(m_nModeType==12||(m_nModeType==16||m_nModeType==PM_PM5E || m_nModeType == PM_PM5E_ARM)||(m_nModeType==18)||m_nModeType==PM_TSTAT6||m_nModeType==PM_TSTAT5i ||m_nModeType==PM_TSTAT8 || m_nModeType == PM_TSTAT9
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)

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
				//int nRange = product_register_value[186]; Annul by Fance
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
		Read_Multi(g_tstat_id,&product_register_value[i*64],i*64,64);	
	}
	register_critical_section.Unlock();
	memcpy_s(product_register_value,sizeof(product_register_value),product_register_value,sizeof(product_register_value));
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
		

	//if(g_serialNum>0&&product_register_value[6]>0)
	if(product_register_value[6]>0)
	{
		CppSQLite3Table table;
		CppSQLite3Query q;

		CppSQLite3DB SqliteDBBuilding;
		SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

		CString strSerial;
		strSerial.Format(_T("%d"),g_serialNum);

		CString strsql;
		strsql.Format(_T("select * from IONAME where SERIAL_ID = '%s'"),strSerial);
		q = SqliteDBBuilding.execQuery((UTF8MBSTR)strsql);

		//m_RsTmp->Open((_variant_t)strsql,_variant_t((IDispatch *)m_ConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
		if(!q.eof())//update
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
			SqliteDBBuilding.execDML((UTF8MBSTR)str_temp);
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

				 
				SqliteDBBuilding.execDML((UTF8MBSTR)str_temp);
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
	
		 SqliteDBBuilding.closedb();
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
////if (product_register_value[7] == 6)
//	if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
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
//		product_register_value[startadd] = nItem;
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
	 
	if(b_is_fresh==true)
	{
		AfxMessageBox(_T("It's refreshing ,can not exit at this time!"));
		return;
	}

	CDialog::OnClose();
}


void COutputSetDlg::OnCbnSelchangeComboSignalType()
{
	 int sel=m_combox_signal_type.GetCurSel();
	 sel+=5;
	 if (m_nCurRow==6)
	 {
		 int ret=write_one(g_tstat_id,692,sel);
		 if (ret>0)
		 {
			 product_register_value[692]=sel;
			 Fresh_Grid();
		 }
		 else
		 {
			 AfxMessageBox(_T("Save Fail"));
		 }
	 }
	 if (m_nCurRow==7)
	 {
		 int ret=write_one(g_tstat_id,693,sel);
		 if (ret>0)
		 {
			 product_register_value[693]=sel;
			 Fresh_Grid();
		 }
		 else
		 {
			 AfxMessageBox(_T("Save Fail"));
		 }
	 }

}
