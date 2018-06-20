// T38I13O.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T38I13O.h"
 
#include "global_function.h"
#include "MainFrm.h"
#include "WriteSingleRegDlg.h"
#include "Dialog_Progess.h"
CString Range_Unit[]={_T("RAW DATA"),_T("TYPE2 10K C"),_T("TYPE2 10K F"),_T("0-100%"),_T("ON/OFF"),_T("OFF/ON"),_T("Pulse Input"),_T("Lighting Control"),_T("TYPE3 10K C"),_T("TYPE3 10K F"),_T("NO USE"),_T("0-5V"),_T("0-10V"),_T("4-20ma")};
 
#define  WM_FRESH_T38I13O WM_USER+1005
DWORD WINAPI  _ReadMulti_T38I13ORegisters(LPVOID pParam){
	//return 0;
	T38I13O *pParent = (T38I13O *)pParam;
	while (TRUE)
	{
	    if (!pParent->IsWindowVisible())
	    {
		   
		   return 0;
	    }
		
		if(g_bPauseMultiRead)
		{
			 return 0;
		}
		if (!is_connect())
	  	{
			Sleep(5000);
			continue;
		} 
		else
		{
			Sleep(3000);
			//Read_Multi(g_tstat_id,&product_register_value[pParent->ZONE_TIME_LEFT_INPUT1],pParent->ZONE_TIME_LEFT_INPUT1,8);
			for (int i=0;i<3;i++)
			{
				if(g_bPauseMultiRead)
				{
				    TRACE(L"return Multi Read -T3");
					return 0;
				}
				TRACE(L"Multi Read -T3");
				Read_Multi(g_tstat_id,&product_register_value[i*100],i*100,100);
			}
			::SendMessage(pParent->m_hWnd,WM_FRESH_T38I13O,0,0);
		}
	}
}
IMPLEMENT_DYNCREATE(T38I13O, CFormView)

T38I13O::T38I13O()
	: CFormView(T38I13O::IDD)
{
hFirstThread=NULL;
}

T38I13O::~T38I13O()
{

}

void T38I13O::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MSFLEXGRID_INPUT, m_msflexgrid_input);
    DDX_Control(pDX, IDC_MSFLEXGRID_OUTPUT, m_msflexgrid_output);
    DDX_Control(pDX, IDC_RANGECOMBO, m_comboxRange);
    DDX_Control(pDX, IDC_EDIT_NAME, m_inNameEdt);
    DDX_Control(pDX, IDC_BRANDRATE, m_brandratebombox);
    //  DDX_Control(pDX, IDC_DELAY, m_delaycombox);
}

BEGIN_MESSAGE_MAP(T38I13O, CFormView)
	ON_CBN_SELCHANGE(IDC_RANGECOMBO, &T38I13O::OnCbnSelchangeRangecombo)
	ON_EN_KILLFOCUS(IDC_EDIT_NAME, &T38I13O::OnEnKillfocusEditName)
	ON_CBN_SELCHANGE(IDC_BRANDRATE, &T38I13O::OnCbnSelchangeBrandrate)
//	ON_CBN_SELCHANGE(IDC_DELAY, &T38I13O::OnCbnSelchangeDelay)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_RESET, &T38I13O::OnBnClickedButtonReset)
    ON_EN_KILLFOCUS(IDC_EDIT_RESPONSE_DELAY, &T38I13O::OnEnKillfocusEditResponseDelay)
END_MESSAGE_MAP()


// T38I13O diagnostics

#ifdef _DEBUG
void T38I13O::AssertValid() const
{
    CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void T38I13O::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG
void T38I13O::OnInitialUpdate()
{
CFormView::OnInitialUpdate();
#if 1	
//设置排/行数量
m_msflexgrid_input.put_Cols(7);
m_msflexgrid_input.put_Rows(9);//包括标题栏
//显示横标题
m_msflexgrid_input.put_TextMatrix(0,0,_T("Input Name"));
m_msflexgrid_input.put_TextMatrix(0,1,_T("Register Value"));
m_msflexgrid_input.put_TextMatrix(0,2,_T("Date Stamp"));
m_msflexgrid_input.put_TextMatrix(0,3,_T("Range"));
m_msflexgrid_input.put_TextMatrix(0,4,_T("Filter"));
m_msflexgrid_input.put_TextMatrix(0,5,_T("Lighting Time"));
m_msflexgrid_input.put_TextMatrix(0,6,_T("Time Left"));



//设置列宽	
m_msflexgrid_input.put_ColWidth(0,1000);
m_msflexgrid_input.put_ColWidth(1,1200);
m_msflexgrid_input.put_ColWidth(2,1300);
m_msflexgrid_input.put_ColWidth(3,1000);
m_msflexgrid_input.put_ColWidth(4,800);
m_msflexgrid_input.put_ColWidth(5,1300);
m_msflexgrid_input.put_ColWidth(6,1000);
//居中显示
for (int col=0;col<7;col++)
{ 
	m_msflexgrid_input.put_ColAlignment(col,4);
}

//彩色显示
for(int i=1;i<9;i++)		//排数量
{

	for(int k=0;k<7;k++)	//列数量
	{
		if (i%2==1)
		{
			m_msflexgrid_input.put_Row(i);m_msflexgrid_input.put_Col(k);m_msflexgrid_input.put_CellBackColor(RGB(255,255,255));
		}
		else
		{
			m_msflexgrid_input.put_Row(i);m_msflexgrid_input.put_Col(k);m_msflexgrid_input.put_CellBackColor(COLOR_CELL);
		}
	}
}




//显示纵标题
CString str;
for(int i=1;i<9;i++)
{

	str.Format(_T("Input%d"),i);

	m_msflexgrid_input.put_TextMatrix(i,0,str);	 
}

//============================================================================================================界面Output DO部份列表框

//设置行/列数量
m_msflexgrid_output.put_Rows(14);
m_msflexgrid_output.put_Cols(5);
//设置列宽	


//显示横标题
m_msflexgrid_output.put_TextMatrix(0,0,_T("Output Name"));
m_msflexgrid_output.put_TextMatrix(0,1,_T("Output Value"));
m_msflexgrid_output.put_TextMatrix(0,2,_T("Light Switch"));
m_msflexgrid_output.put_TextMatrix(0,3,_T("A/M"));
m_msflexgrid_output.put_TextMatrix(0,4,_T("Switch Status"));

m_msflexgrid_output.put_ColWidth(0,1000);
m_msflexgrid_output.put_ColWidth(1,1000);
m_msflexgrid_output.put_ColWidth(2,1500);
m_msflexgrid_output.put_ColWidth(3,700);//居中显示
m_msflexgrid_output.put_ColWidth(4,1500);
for (int col=0;col<4;col++)
{ 
	m_msflexgrid_output.put_ColAlignment(col,4);
}

//彩色显示
for(int i=1;i<14;i++)		//排数量
{

	for(int k=0;k<4;k++)	//列数量
	{
		if (i%2==1)
		{
			m_msflexgrid_output.put_Row(i);m_msflexgrid_output.put_Col(k);m_msflexgrid_output.put_CellBackColor(RGB(255,255,255));
		}
		else
		{
			m_msflexgrid_output.put_Row(i);m_msflexgrid_output.put_Col(k);m_msflexgrid_output.put_CellBackColor(COLOR_CELL);
		}
	}
}
//显示纵标题
CString str_output;
for(int i=1;i<14;i++)
{

	str_output.Format(_T("Output%d"),i);
	m_msflexgrid_output.put_TextMatrix(i,0,str_output);	


}




#endif  
}
void T38I13O::Initial_RegisterList()
{
 #if 1
	SN_LOW	=	0	;
	SN_HI	=	2	;
	EPROM_VER_NUMBER	=	4	;
	FIRMWARE_VER_NUMBER	=	5	;
	MODBUS_ID	=	6	;
	PRODUCT_MODEL	=	7	;
	HARDWARE_VER_NUMBER	=	8	;
	PIC_VER_NUMBER	=	9	;
	CALIBRATION_OUTPUTS	=	13	;
	BAUDRATE	=	15	;
	RESPONSE_DELAY	=	19	;
	OUTPUT1	=	100	;
	OUTPUT2	=	101	;
	OUTPUT3	=	102	;
	OUTPUT4	=	103	;
	OUTPUT5	=	104	;
	OUTPUT6	=	105	;
	OUTPUT7	=	106	;
	OUTPUT8	=	107	;
	OUTPUT9	=	108	;
	OUTPUT10	=	109	;
	OUTPUT11	=	110	;
	OUTPUT12	=	111	;
	OUTPUT13	=	112	;
	SWITCH1_STATUS	=	116	;
	SWITCH2_STATUS	=	117	;
	INPUT1_PULSE_COUNT_HIGHT	=	118	;
	INPUT1_PULSE_COUNT_LOW	=	119	;
	INPUT2_PULSE_COUNT_HIGHT	=	120	;
	INPUT2_PULSE_COUNT_LOW	=	121	;
	INPUT3_PULSE_COUNT_HIGHT	=	122	;
	INPUT3_PULSE_COUNT_LOW	=	123	;
	INPUT4_PULSE_COUNT_HIGHT	=	124	;
	INPUT4_PULSE_COUNT_LOW	=	125	;
	INPUT5_PULSE_COUNT_HIGHT	=	126	;
	INPUT5_PULSE_COUNT_LOW	=	127	;
	INPUT6_PULSE_COUNT_HIGHT	=	128	;
	INPUT6_PULSE_COUNT_LOW	=	129	;
	INPUT7_PULSE_COUNT_HIGHT	=	130	;
	INPUT7_PULSE_COUNT_LOW	=	131	;
	INPUT8_PULSE_COUNT_HIGHT	=	132	;
	INPUT8_PULSE_COUNT_LOW	=	133	;
	DATE_STAMP_INPUT1_YEAR	=	134	;
	DATE_STAMP_INPUT1_MONTH	=	135	;
	DATE_STAMP_INPUT1_DAY	=	136	;
	DATE_STAMP_INPUT1_HOUR	=	137	;
	DATE_STAMP_INPUT1_MINUTE	=	138	;
	DATE_STAMP_INPUT2_YEAR	=	139	;
	DATE_STAMP_INPUT2_MONTH	=	140	;
	DATE_STAMP_INPUT2_DAY	=	141	;
	DATE_STAMP_INPUT2_HOUR	=	142	;
	DATE_STAMP_INPUT2_MINUTE	=	143	;
	DATE_STAMP_INPUT3_YEAR	=	144	;
	DATE_STAMP_INPUT3_MONTH	=	145	;
	DATE_STAMP_INPUT3_DAY	=	146	;
	DATE_STAMP_INPUT3_HOUR	=	147	;
	DATE_STAMP_INPUT3_MINUTE	=	148	;
	DATE_STAMP_INPUT4_YEAR	=	149	;
	DATE_STAMP_INPUT4_MONTH	=	150	;
	DATE_STAMP_INPUT4_DAY	=	151	;
	DATE_STAMP_INPUT4_HOUR	=	152	;
	DATE_STAMP_INPUT4_MINUTE	=	153	;
	DATE_STAMP_INPUT5_YEAR	=	154	;
	DATE_STAMP_INPUT5_MONTH	=	155	;
	DATE_STAMP_INPUT5_DAY	=	156	;
	DATE_STAMP_INPUT5_HOUR	=	157	;
	DATE_STAMP_INPUT5_MINUTE	=	158	;
	DATE_STAMP_INPUT6_YEAR	=	159	;
	DATE_STAMP_INPUT6_MONTH	=	160	;
	DATE_STAMP_INPUT6_DAY	=	161	;
	DATE_STAMP_INPUT6_HOUR	=	162	;
	DATE_STAMP_INPUT6_MINUTE	=	163	;
	DATE_STAMP_INPUT7_YEAR	=	164	;
	DATE_STAMP_INPUT7_MONTH	=	165	;
	DATE_STAMP_INPUT7_DAY	=	166	;
	DATE_STAMP_INPUT7_HOUR	=	167	;
	DATE_STAMP_INPUT7_MINUTE	=	168	;
	DATE_STAMP_INPUT8_YEAR	=	169	;
	DATE_STAMP_INPUT8_MONTH	=	170	;
	DATE_STAMP_INPUT8_DAY	=	171	;
	DATE_STAMP_INPUT8_HOUR	=	172	;
	DATE_STAMP_INPUT8_MINUTE	=	173	;
	ORIGIGINAL_AD_DATA_INPUT1	=	175	;
	ORIGIGINAL_AD_DATA_INPUT2	=	176	;
	ORIGIGINAL_AD_DATA_INPUT3	=	177	;
	ORIGIGINAL_AD_DATA_INPUT4	=	178	;
	ORIGIGINAL_AD_DATA_INPUT5	=	179	;
	ORIGIGINAL_AD_DATA_INPUT6	=	180	;
	ORIGIGINAL_AD_DATA_INPUT7	=	181	;
	ORIGIGINAL_AD_DATA_INPUT8	=	182	;
	RANGE_INPUT1	=	183	;
	RANGE_INPUT2	=	184	;
	RANGE_INPUT3	=	185	;
	RANGE_INPUT4	=	186	;
	RANGE_INPUT5	=	187	;
	RANGE_INPUT6	=	188	;
	RANGE_INPUT7	=	189	;
	RANGE_INPUT8	=	190	;
	FILTER_INPUT1	=	191	;
	FILTER_INPUT2	=	192	;
	FILTER_INPUT3	=	193	;
	FILTER_INPUT4	=	194	;
	FILTER_INPUT5	=	195	;
	FILTER_INPUT6	=	196	;
	FILTER_INPUT7	=	197	;
	FILTER_INPUT8	=	198	;
	LIGHTING_ZONE_TIME_INPUT1	=	199	;
	LIGHTING_ZONE_TIME_INPUT2	=	200	;
	LIGHTING_ZONE_TIME_INPUT3	=	201	;
	LIGHTING_ZONE_TIME_INPUT4	=	202	;
	LIGHTING_ZONE_TIME_INPUT5	=	203	;
	LIGHTING_ZONE_TIME_INPUT6	=	204	;
	LIGHTING_ZONE_TIME_INPUT7	=	205	;
	LIGHTING_ZONE_TIME_INPUT8	=	206	;
	ZONE_TIME_LEFT_INPUT1	=	207	;
	ZONE_TIME_LEFT_INPUT2	=	208	;
	ZONE_TIME_LEFT_INPUT3	=	209	;
	ZONE_TIME_LEFT_INPUT4	=	210	;
	ZONE_TIME_LEFT_INPUT5	=	211	;
	ZONE_TIME_LEFT_INPUT6	=	212	;
	ZONE_TIME_LEFT_INPUT7	=	213	;
	ZONE_TIME_LEFT_INPUT8	=	214	;
	AUTO_MANUAL_OUTPUTS	=	215	;
	LIGHT_SWITCH_OUTPUT1	=	216	;
	LIGHT_SWITCH_OUTPUT2	=	217	;
	LIGHT_SWITCH_OUTPUT3	=	218	;
	LIGHT_SWITCH_OUTPUT4	=	219	;
	LIGHT_SWITCH_OUTPUT5	=	220	;
	LIGHT_SWITCH_OUTPUT6	=	221	;
	LIGHT_SWITCH_OUTPUT7	=	222	;
	LIGHT_SWITCH_OUTPUT8	=	223	;
	LIGHT_SWITCH_OUTPUT9	=	224	;
	LIGHT_SWITCH_OUTPUT10	=	225	;
	LIGHT_SWITCH_OUTPUT11	=	226	;
	LIGHT_SWITCH_OUTPUT12	=	227	;
	LIGHT_SWITCH_OUTPUT13	=	228	;

#endif
	 


 





}

int  T38I13O::Get_RegID(CString Name)
{
	Name.TrimLeft();
	Name.TrimRight();
	int regid=-1;
	vector<T3Register>::iterator iter;
	for (iter=m_vecT3Register.begin();iter!=m_vecT3Register.end();iter++)
	{
		if (iter->regName==Name)
		{
			regid=iter->regID;
			break;
		}
	}
	return regid;
}
// T38I13O message handlers
void T38I13O::InitialDialog(){
Initial_RegisterList();
InitialTableName();
CString strTemp;
strTemp.Format(_T("%d"),product_register_value[MODBUS_ID]);
GetDlgItem(IDC_EDIT_T3ADDRESS)->SetWindowText(strTemp);
int temp;
temp=product_register_value[SN_LOW]+product_register_value[SN_LOW+1]*256+product_register_value[SN_HI]*256*256+product_register_value[SN_HI+1]*256*256*256;
strTemp.Format(_T("%d"),temp);
GetDlgItem(IDC_EDIT_T3SERIAL)->SetWindowText(strTemp);

GetDlgItem(IDC_EDIT_T3MODEL)->SetWindowText(_T("T3-8I13O"));
strTemp.Format(_T("%d"),product_register_value[FIRMWARE_VER_NUMBER]);
GetDlgItem(IDC_EDIT_T3FIRMWARE)->SetWindowText(strTemp);
strTemp.Format(_T("%d"),product_register_value[HARDWARE_VER_NUMBER]);
GetDlgItem(IDC_EDIT_T3HARDWARE)->SetWindowText(strTemp);
strTemp.Format(_T("%d"),product_register_value[PIC_VER_NUMBER]);
GetDlgItem(IDC_EDIT_T3PICVERSION)->SetWindowText(strTemp);
m_brandratebombox.SetCurSel(product_register_value[BAUDRATE]);
//m_delaycombox.SetCurSel(product_register_value[RESPONSE_DELAY]);
strTemp.Format(_T("%d"),product_register_value[RESPONSE_DELAY]);
GetDlgItem(IDC_EDIT_RESPONSE_DELAY)->SetWindowText(strTemp);
// Input
CString strresult;
int regValue;
for(int i = 1;i<=8;i++)
{  
	

	strresult.Format(_T("%4d/%4d/%4d  %2d:%2d"),product_register_value[DATE_STAMP_INPUT1_YEAR+3*(i-1)],product_register_value[DATE_STAMP_INPUT1_MONTH+3*(i-1)],product_register_value[DATE_STAMP_INPUT1_DAY+3*(i-1)],product_register_value[DATE_STAMP_INPUT1_HOUR+3*(i-1)],product_register_value[DATE_STAMP_INPUT1_MINUTE+3*(i-1)]);
	m_msflexgrid_input.put_TextMatrix(i,2,strresult);

	strresult.Format(_T("%d"),product_register_value[RANGE_INPUT1+i-1]);
	if (0==product_register_value[RANGE_INPUT1+i-1])
	{
		strresult=_T("RAW DATA");

	} 
	else if (1==product_register_value[RANGE_INPUT1+i-1])
	{
		strresult=_T("10K C");
	}
	else if (2==product_register_value[RANGE_INPUT1+i-1])
	{
		strresult=_T("10K F");
	}
	else if (3==product_register_value[RANGE_INPUT1+i-1])
	{
		strresult=_T("0-100%");
	}
	else if (4==product_register_value[RANGE_INPUT1+i-1])
	{
		strresult=_T("ON/OFF");
	}
	else if (5==product_register_value[RANGE_INPUT1+i-1])
	{
		strresult=_T("OFF/ON");
	}
	else if (6==product_register_value[RANGE_INPUT1+i-1])
	{
		strresult=_T("Pulse Input");
	}
	else if (7==product_register_value[RANGE_INPUT1+i-1])
	{
		strresult=_T("Lighting Control");
	}
	else if (8==product_register_value[RANGE_INPUT1+i-1])
	{
		strresult=_T("TYPE3 10K C");
	}
	else if (9==product_register_value[RANGE_INPUT1+i-1])
	{
		strresult=_T("TYPE3 10K F");
	}
	else if (10==product_register_value[RANGE_INPUT1+i-1])
	{
		strresult=_T("NO USE");
	}
	else if (11==product_register_value[RANGE_INPUT1+i-1])
	{
		strresult=_T("0-5V");
	}
	else if (12==product_register_value[RANGE_INPUT1+i-1])
	{
		strresult=_T("0-10V");
	}
	else if (13==product_register_value[RANGE_INPUT1+i-1])
	{
		strresult=_T("4-20 ma");
	}

	m_msflexgrid_input.put_TextMatrix(i,3,strresult);


	regValue=(short)product_register_value[INPUT1_PULSE_COUNT_LOW+2*(i-1)];
	
	if (0==product_register_value[RANGE_INPUT1+i-1])
	{
		strresult.Format(_T("%d"),regValue);

	} 
	else if (1==product_register_value[RANGE_INPUT1+i-1])
	{
		strresult.Format(_T("%.1f C"),(float)regValue/10.0);
	}
	else if (2==product_register_value[RANGE_INPUT1+i-1])
	{
		strresult=_T("10K F");
		strresult.Format(_T("%.1f F"),(float)regValue/10.0);
	}
	else if (3==product_register_value[RANGE_INPUT1+i-1])
	{
		 
		strresult.Format(_T("%.1f"),(float)regValue);
		strresult+=_T("%");
	}
	else if (4==product_register_value[RANGE_INPUT1+i-1])
	{
		//strresult.Format(_T("%.1f F"),(float)regValue/10.0);
		if (regValue==0)
		{
		strresult=_T("OFF");
		} 
		else
		{
		strresult=_T("ON");
		}
	}
	else if (5==product_register_value[RANGE_INPUT1+i-1])
	{
		if (regValue==0)
		{
			strresult=_T("ON");
		} 
		else
		{
			strresult=_T("OFF");
		}
	}
	else if (6==product_register_value[RANGE_INPUT1+i-1])
	{
		 strresult.Format(_T("%d"),regValue);
	}
	else if (7==product_register_value[RANGE_INPUT1+i-1])
	{
		strresult.Format(_T("%d"),regValue);
	}
	else if (8==product_register_value[RANGE_INPUT1+i-1])
	{
		//strresult=_T("TYPE3 10K C");
		strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
	}
	else if (9==product_register_value[RANGE_INPUT1+i-1])
	{
		/*strresult=_T("TYPE3 10K F");*/
		strresult.Format(_T("%0.1f F"),(float)regValue/10.0);
	}
	else if (10==product_register_value[RANGE_INPUT1+i-1])
	{
		strresult=_T("0");
		//strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
	}
	else if (11==product_register_value[RANGE_INPUT1+i-1])
	{
		//strresult=_T("0-5V");
		strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
	}
	else if (12==product_register_value[RANGE_INPUT1+i-1])
	{
		//strresult=_T("0-10V");
		strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
	}
	else if (13==product_register_value[RANGE_INPUT1+i-1])
	{
		//strresult=_T("0-20I");
		strresult.Format(_T("%0.1f ma"),(float)regValue/1000.0);
	}
	
	
	
	
	m_msflexgrid_input.put_TextMatrix(i,1,strresult);



	strresult.Format(_T("%d"),product_register_value[FILTER_INPUT1+i-1]);
	m_msflexgrid_input.put_TextMatrix(i,4,strresult);

	strresult.Format(_T("%d min"),product_register_value[LIGHTING_ZONE_TIME_INPUT1+i-1]);
	m_msflexgrid_input.put_TextMatrix(i,5,strresult);

	strresult.Format(_T("%d min"),product_register_value[ZONE_TIME_LEFT_INPUT1+i-1]);
	m_msflexgrid_input.put_TextMatrix(i,6,strresult);
}

bitset<16> BitSwitchValue(product_register_value[SWITCH1_STATUS]);
int SwitchValue[13];
for (int i=0;i<8;i++)
{
	SwitchValue[i]=BitSwitchValue[2*i]+BitSwitchValue[2*i+1]*2;
}

bitset<16> BitSwitchValue2(product_register_value[SWITCH2_STATUS]);
for (int i=8;i<13;i++)
{
	SwitchValue[i]=BitSwitchValue2[2*(i-8)]+BitSwitchValue2[2*(i-8)+1]*2;
}


CString CstresultDO;
for(int i = 1;i<=13;i++)
{  

	CstresultDO.Format(_T("%d"),product_register_value[OUTPUT1+i-1]);
// 	if (product_register_value[OUTPUT1+i-1]==0)
// 	{
// 	CstresultDO=_T("Off");
// 	}
// 	else
// 	{
// 	CstresultDO=_T("On");
// 	}
	m_msflexgrid_output.put_TextMatrix(i,1,CstresultDO);
    
	if (product_register_value[LIGHT_SWITCH_OUTPUT1+i-1]>0)
	{
	CstresultDO=Get_Table_Name(m_sn,_T("Input"),product_register_value[LIGHT_SWITCH_OUTPUT1+i-1]);
	}
	else
	{
	CstresultDO=_T("UNUSED");
	}
	m_msflexgrid_output.put_TextMatrix(i,2,CstresultDO);
    if (((product_register_value[AUTO_MANUAL_OUTPUTS]>>(i-1))&0x01)==1)
    {
	CstresultDO=_T("Manual");
    } 
    else
    {
	CstresultDO=_T("Auto");
    }
	m_msflexgrid_output.put_TextMatrix(i,3,CstresultDO);

	m_msflexgrid_output.put_TextMatrix(i,4,STRING_SWITCH_STATUS[SwitchValue[i-1]]); 
}
}
void T38I13O::InitialTableName(){
	m_sn=product_register_value[SN_LOW]+product_register_value[SN_LOW+1]*256+product_register_value[SN_HI]*256*256+product_register_value[SN_HI+1]*256*256*256;
	
	CString str_input;
	for(int i=1;i<9;i++)
	{

		 str_input=Get_Table_Name(m_sn,_T("Input"),i);

		 m_msflexgrid_input.put_TextMatrix(i,0,str_input);	 
	}

    CString str_output;
    for(int i=1;i<14;i++)
    {

        str_output=Get_Table_Name(m_sn,_T("Output"),i);
        m_msflexgrid_output.put_TextMatrix(i,0,str_output);	


    }
}
void T38I13O::Fresh()
{
   
	if (is_connect())
	{  
		 

		/*for (int i=0;i<3;i++)
		{
			  
			Read_Multi(g_tstat_id,&product_register_value[i*100],i*100,100);
		}*/
		 
		g_bPauseMultiRead=FALSE;
		InitialDialog();
		SetTimer(1,2000,NULL);
		//AfxBeginThread(_ReadMulti_T38I13ORegisters,this);
		if(hFirstThread != NULL)
			TerminateThread(hFirstThread, 0);
		hFirstThread=NULL;
		if (!hFirstThread)
		{
			hFirstThread = CreateThread(NULL,NULL,_ReadMulti_T38I13ORegisters,this,NULL,0);
		}
	}
	else
	{
		SetPaneString(1,_T("Disconnection"));
		AfxMessageBox(_T("Please Connect your device"));
	}

}
BEGIN_EVENTSINK_MAP(T38I13O, CFormView)
	ON_EVENT(T38I13O, IDC_MSFLEXGRID_INPUT, DISPID_CLICK, T38I13O::ClickMsflexgridInput, VTS_NONE)
	ON_EVENT(T38I13O, IDC_MSFLEXGRID_OUTPUT, DISPID_CLICK, T38I13O::ClickMsflexgridOutput, VTS_NONE)
END_EVENTSINK_MAP()


void T38I13O::ClickMsflexgridInput()
{
    m_isinput=TRUE;
	m_comboxRange.ShowWindow(FALSE);
	UpdateData(FALSE);

	long lRow,lCol;
	lRow = m_msflexgrid_input.get_RowSel();//获取点击的行号	
	lCol = m_msflexgrid_input.get_ColSel(); //获取点击的列号
	TRACE(_T("Click input grid!\n"));

	CRect rect;
	m_msflexgrid_input.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	CDC* pDC =GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_msflexgrid_input.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_msflexgrid_input.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_msflexgrid_input.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_msflexgrid_input.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rcCell(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rcCell.OffsetRect(rect.left+1,rect.top+1);
	ReleaseDC(pDC);
	CString strValue = m_msflexgrid_input.get_TextMatrix(lRow,lCol);
	m_oldname=strValue;
	m_curcol=lCol;
	m_currow=lRow;

	if (lCol==6)//Time left --ReadOnly.
	{
	return;
	}
	if(3!=lCol && lRow != 0)
	{

		m_inNameEdt.MoveWindow(&rcCell,1);
		m_inNameEdt.ShowWindow(SW_SHOW);
		m_inNameEdt.SetWindowText(strValue);
		m_inNameEdt.SetFocus();
		m_inNameEdt.SetCapture();//LSC
		int nLenth=strValue.GetLength();
		m_inNameEdt.SetSel(nLenth,nLenth); //全选//

	}
	if((3==lCol)&&lRow!=0)
	{


		m_comboxRange.MoveWindow(&rcCell,1);
		m_comboxRange.ResetContent();
		m_comboxRange.AddString(_T("RAW DATA"));
		m_comboxRange.AddString(_T("10K C"));
		m_comboxRange.AddString(_T("10K F"));
		m_comboxRange.AddString(_T("0-100%"));
		m_comboxRange.AddString(_T("ON/OFF"));
		m_comboxRange.AddString(_T("OFF/ON"));
		m_comboxRange.AddString(_T("Pulse Input"));
		m_comboxRange.AddString(_T("Lighting Control"));
		m_comboxRange.AddString(_T("TYPE3 10K C"));
		m_comboxRange.AddString(_T("TYPE3 10K F"));
		m_comboxRange.AddString(_T("NO USE"));
		m_comboxRange.AddString(_T("0-5V"));
		m_comboxRange.AddString(_T("0-10V"));
		m_comboxRange.AddString(_T("4-20ma"));

		m_comboxRange.ShowWindow(SW_SHOW);



		m_comboxRange.BringWindowToTop();

		m_comboxRange.SetFocus(); //获取焦点
		m_comboxRange.SetWindowText(strValue);
}
    
}
void T38I13O::OnCbnSelchangeRangecombo()
	{
	   if (m_isinput)
	   {
		   BeginWaitCursor();
		   CString strTemp;
		   int lRow = m_msflexgrid_input.get_RowSel();	
		   int lCol = m_msflexgrid_input.get_ColSel();

		   int sel=m_comboxRange.GetCurSel();
		   m_comboxRange.ShowWindow(FALSE);
		   if (product_register_value[RANGE_INPUT1+lRow-1]==sel)
		   {
			   return;
		   }
		   int ret=write_one(g_tstat_id,RANGE_INPUT1+lRow-1,sel);
		   if (ret>0)
		   {
			   product_register_value[RANGE_INPUT1+lRow-1]=sel;
		   }
		   Sleep(500);
		   Read_Multi(g_tstat_id,&product_register_value[INPUT1_PULSE_COUNT_HIGHT],INPUT1_PULSE_COUNT_HIGHT,16);
		   InitialDialog();
		   EndWaitCursor();
	   }
	   else
	   {
	         if (m_curcol==1)
	         {
				 int sel=m_comboxRange.GetCurSel();
				 m_comboxRange.ShowWindow(FALSE);

				 if (product_register_value[OUTPUT1+m_currow-1]==sel)
				 {
					 return;
				 }
				 int ret=write_one(g_tstat_id,OUTPUT1+m_currow-1,sel);
				 if (ret>0)
				 {
					 product_register_value[OUTPUT1+m_currow-1]=sel;
				 }
				 if (product_register_value[OUTPUT1+m_currow-1]==0)
				 {
					 m_msflexgrid_output.put_TextMatrix(m_currow,m_curcol,_T("Off"));
				 }
				 else
				 {
					 m_msflexgrid_output.put_TextMatrix(m_currow,m_curcol,_T("On"));
				 }
	         }
		     
		    else if (m_curcol==2)
			  {
				  int sel=m_comboxRange.GetCurSel();
				  m_comboxRange.ShowWindow(FALSE);
				  if (product_register_value[LIGHT_SWITCH_OUTPUT1+m_currow-1]==sel)
				  {
					  return;
				  }
				  int ret=write_one(g_tstat_id,LIGHT_SWITCH_OUTPUT1+m_currow-1,sel);
				  if (ret>0)
				  {
					  product_register_value[LIGHT_SWITCH_OUTPUT1+m_currow-1]=sel;
				  }
				 
				  CString CstresultDO;
				   if (product_register_value[LIGHT_SWITCH_OUTPUT1+m_currow-1]>0)
				   {
					   CstresultDO=Get_Table_Name(m_sn,_T("Input"),product_register_value[LIGHT_SWITCH_OUTPUT1+m_currow-1]);
				   }
				   else
				   {
					   CstresultDO=_T("UNUSED");
				   }
				   m_msflexgrid_output.put_TextMatrix(m_currow,m_curcol,CstresultDO);
			  }
		    else if (m_curcol==3)
		    {
			   CString strtemp;
		     	int sel=m_comboxRange.GetCurSel();
				m_comboxRange.ShowWindow(FALSE); 
				if (sel==0)
				{
				   strtemp=_T("Auto");
				}
				else
				{
				  strtemp=_T("Manual");
				}
				m_msflexgrid_output.put_TextMatrix(m_currow,m_curcol,strtemp);

				int Value=0;
				int temp;
				for (int i=1;i<=13;i++)
				{
					CString strValue = m_msflexgrid_output.get_TextMatrix(i,m_curcol);
					if (strValue.CompareNoCase(_T("Auto"))==0)
					{
						temp=0;
					}
					else
					{
						temp=1;
					}

					temp=temp<<(i-1);
					Value+=temp;
				}
				int ret=write_one(g_tstat_id,AUTO_MANUAL_OUTPUTS,Value);
				if (ret>0)
				{
				product_register_value[AUTO_MANUAL_OUTPUTS]=Value;
				}
				//InitialDialog();
		    }
	   }
	
	}
void T38I13O::ClickMsflexgridOutput()
	{
	    m_isinput=FALSE;
		long lRow,lCol;
		lRow = m_msflexgrid_output.get_RowSel();//获取点击的行号	
		lCol = m_msflexgrid_output.get_ColSel(); //获取点击的列号
		TRACE(_T("Click input grid!\n"));

		CRect rect;
		m_msflexgrid_output.GetWindowRect(rect); //获取表格控件的窗口矩形
		ScreenToClient(rect); //转换为客户区矩形	
		CDC* pDC =GetDC();

		int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
		int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
		//计算选中格的左上角的坐标(象素为单位)
		long y = m_msflexgrid_output.get_RowPos(lRow)/nTwipsPerDotY;
		long x = m_msflexgrid_output.get_ColPos(lCol)/nTwipsPerDotX;
		//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
		long width = m_msflexgrid_output.get_ColWidth(lCol)/nTwipsPerDotX+1;
		long height = m_msflexgrid_output.get_RowHeight(lRow)/nTwipsPerDotY+1;
		//形成选中个所在的矩形区域
		CRect rcCell(x,y,x+width,y+height);
		//转换成相对对话框的坐标
		rcCell.OffsetRect(rect.left+1,rect.top+1);
		ReleaseDC(pDC);
		CString strValue = m_msflexgrid_output.get_TextMatrix(lRow,lCol);
		m_oldname=strValue;
		m_curcol=lCol;
		m_currow=lRow;
		CString str_input;
		if(lRow != 0&&lCol==0)
		{
			//return; // 2012.2.7老毛说不允许修改
			m_inNameEdt.MoveWindow(&rcCell,1);
			m_inNameEdt.ShowWindow(SW_SHOW);
			m_inNameEdt.SetWindowText(strValue);
			m_inNameEdt.SetFocus();
			m_inNameEdt.SetCapture();//LSC
			int nLenth=strValue.GetLength();
			m_inNameEdt.SetSel(nLenth,nLenth); //全选//

		}
		if (lCol==1&&lRow!=0)
		{
			m_comboxRange.MoveWindow(&rcCell,1);
			m_comboxRange.ResetContent();
			m_comboxRange.AddString(_T("Off"));
			m_comboxRange.AddString(_T("On"));
			m_comboxRange.ShowWindow(SW_SHOW);
			m_comboxRange.BringWindowToTop();
			m_comboxRange.SetFocus(); //获取焦点
			m_comboxRange.SetWindowText(strValue);
		}
		if (lCol==2&&lRow!=0)
		{
			m_comboxRange.MoveWindow(&rcCell,1);
			m_comboxRange.ResetContent();
			//m_comboxRange.AddString(_T("Off"));
			m_comboxRange.AddString(_T("UNUSED"));

		 
			for(int i=1;i<9;i++)
			{

				str_input=Get_Table_Name(m_sn,_T("Input"),i);
				m_comboxRange.AddString(str_input);
			}
			m_comboxRange.ShowWindow(SW_SHOW);
			m_comboxRange.BringWindowToTop();
			m_comboxRange.SetFocus(); //获取焦点
			m_comboxRange.SetWindowText(strValue);
		}
		if (lCol==3&&lRow!=0)
		{
			m_comboxRange.MoveWindow(&rcCell,1);
			m_comboxRange.ResetContent();
			m_comboxRange.AddString(_T("Auto"));
			m_comboxRange.AddString(_T("Manual"));
			m_comboxRange.ShowWindow(SW_SHOW);
			m_comboxRange.BringWindowToTop();
			m_comboxRange.SetFocus(); //获取焦点
			m_comboxRange.SetWindowText(strValue);
			
		}
	}
void T38I13O::OnEnKillfocusEditName()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(strTemp);
	int Value=_wtoi(strTemp);
	if (strTemp.Compare(m_oldname)==0)
	{
		return;
	}
	if (m_isinput)
	{
	  if (m_curcol==0)
	  {
	   
		Insert_Update_Table_Name(m_sn,_T("Input"),m_currow,strTemp);
		//InitialTableName();
		m_msflexgrid_input.put_TextMatrix(m_currow,m_curcol,strTemp);
	  }
	  if (m_curcol==1)//Value
	  {
	  

			int regvalue=product_register_value[RANGE_INPUT1+m_currow-1];
			if (regvalue>13)
			{
				regvalue=0;
			}
			Value=Value*Get_Unit_Process(Range_Unit[regvalue]);
			
			int low;
		
		low=Value%65535;
	   // int ret1=write_one(g_tstat_id,INPUT1_PULSE_COUNT_HIGHT+2*(m_currow-1),high);
		int ret2=write_one(g_tstat_id,INPUT1_PULSE_COUNT_LOW+2*(m_currow-1),low);
		if (ret2>0)
		{
		product_register_value[INPUT1_PULSE_COUNT_LOW+2*(m_currow-1)]=low;
		InitialDialog();
		} 
	 
	    
	  } 
	  if (m_curcol==4)
	  {
	    int regvalue=product_register_value[FILTER_INPUT1+m_currow-1];
		if (Value!=regvalue)
		{
		  int ret1=write_one(g_tstat_id,FILTER_INPUT1+m_currow-1,Value);
		  if (ret1>0)
		  {
			  product_register_value[FILTER_INPUT1+m_currow-1]=Value;
			  InitialDialog();
		  }
		}
	  }
	  if (m_curcol==5)
	  {
		  int regvalue=product_register_value[LIGHTING_ZONE_TIME_INPUT1+m_currow-1];
		  if (Value!=regvalue)
		  {
			  int ret1=write_one(g_tstat_id,LIGHTING_ZONE_TIME_INPUT1+m_currow-1,Value);
			  if (ret1>0)
			  {
				  product_register_value[LIGHTING_ZONE_TIME_INPUT1+m_currow-1]=Value;
				  InitialDialog();
			  }
			  else
			  {
			  AfxMessageBox(_T("Try again"));
			  }
		  }
	  }
	
	}
	else
	{
		if (m_curcol==0)
		{
			if (strTemp.Compare(m_oldname)==0)
			{
				return;
			}
			Insert_Update_Table_Name(m_sn,_T("Output"),m_currow,strTemp);
			m_msflexgrid_output.put_TextMatrix(m_currow,m_curcol,strTemp);
		}


		//CstresultDO.Format(_T("%d"),product_register_value[OUTPUT1+i-1]);
		//m_msflexgrid_output.put_TextMatrix(i,1,CstresultDO);
		////m_msflexgrid_output.put_TextMatrix(i,2,vect3model.at(100-1+i).CStName);
		//CstresultDO.Format(_T("%d"),product_register_value[LIGHT_SWITCH_OUTPUT1+i-1]);
		//m_msflexgrid_output.put_TextMatrix(i,2,CstresultDO);
	 
		if (m_curcol==2)
		{

		}

	}

}


void T38I13O::OnCbnSelchangeBrandrate()
{
	
}


//void T38I13O::OnCbnSelchangeDelay()
//{
//	 int item=m_delaycombox.GetCurSel();
//	 int ret=write_one(g_tstat_id,RESPONSE_DELAY,item);
//	 if (ret>0)
//	 {
//	 product_register_value[RESPONSE_DELAY]=item;
//	 } 
//	 else
//	 {
//	 AfxMessageBox(_T("Write Error"));
//	 }
//
//	 m_delaycombox.SetCurSel(product_register_value[RESPONSE_DELAY]);
//
//}


void T38I13O::OnTimer(UINT_PTR nIDEvent)
{
	 
	CMainFrame*pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CView* pNewView = pMain->m_pViews[8];
	CView* pActiveView = pMain->GetActiveView();

	if(1 == nIDEvent)// )&& (counter%60 == 0)
	{

		//Read_Multi(g_tstat_id,&product_register_value[ZONE_TIME_LEFT_INPUT1],ZONE_TIME_LEFT_INPUT1,8);

		CString strresult;
		 
		for(int i = 1;i<=8;i++)
		{  
			strresult.Format(_T("%d min"),product_register_value[ZONE_TIME_LEFT_INPUT1+i-1]);
			m_msflexgrid_input.put_TextMatrix(i,6,strresult);
		}

	
	}

	CFormView::OnTimer(nIDEvent);
}


void T38I13O::OnDestroy()
{

   if (hFirstThread!=NULL)
   {
   TerminateThread(hFirstThread, 0);
   }
	CFormView::OnDestroy();

	 
}


//BOOL T38I13O::PreTranslateMessage(MSG* pMsg)
//{
//	
//	// CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
//	if(pMsg->message==WM_FRESH_T38I13O){
//		if (no_mouse_keyboard_event_enable_refresh)
//		{
//			CString strresult;
//			int regValue;
//			for(int i = 1;i<=8;i++)
//			{  
//
//
//				strresult.Format(_T("%4d/%4d/%4d  %2d:%2d"),product_register_value[DATE_STAMP_INPUT1_YEAR+3*(i-1)],product_register_value[DATE_STAMP_INPUT1_MONTH+3*(i-1)],product_register_value[DATE_STAMP_INPUT1_DAY+3*(i-1)],product_register_value[DATE_STAMP_INPUT1_HOUR+3*(i-1)],product_register_value[DATE_STAMP_INPUT1_MINUTE+3*(i-1)]);
//				m_msflexgrid_input.put_TextMatrix(i,2,strresult);
//
//				strresult.Format(_T("%d"),product_register_value[RANGE_INPUT1+i-1]);
//				if (0==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult=_T("RAW DATA");
//
//				} 
//				else if (1==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult=_T("10K C");
//				}
//				else if (2==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult=_T("10K F");
//				}
//				else if (3==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult=_T("0-100%");
//				}
//				else if (4==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult=_T("ON/OFF");
//				}
//				else if (5==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult=_T("OFF/ON");
//				}
//				else if (6==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult=_T("Pulse Input");
//				}
//				else if (7==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult=_T("Lighting Control");
//				}
//				else if (8==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult=_T("TYPE3 10K C");
//				}
//				else if (9==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult=_T("TYPE3 10K F");
//				}
//				else if (10==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult=_T("NO USE");
//				}
//				else if (11==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult=_T("0-5V");
//				}
//				else if (12==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult=_T("0-10V");
//				}
//				else if (13==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult=_T("0-20 ma");
//				}
//
//				m_msflexgrid_input.put_TextMatrix(i,3,strresult);
//
//
//				regValue=(short)product_register_value[INPUT1_PULSE_COUNT_LOW+2*(i-1)];
//
//				if (0==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult.Format(_T("%d"),regValue);
//
//				} 
//				else if (1==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult.Format(_T("%.1f C"),(float)regValue/10.0);
//				}
//				else if (2==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult=_T("10K F");
//					strresult.Format(_T("%.1f F"),(float)regValue/10.0);
//				}
//				else if (3==product_register_value[RANGE_INPUT1+i-1])
//				{
//
//					strresult.Format(_T("%.1f"),(float)regValue);
//					strresult+=_T("%");
//				}
//				else if (4==product_register_value[RANGE_INPUT1+i-1])
//				{
//					//strresult.Format(_T("%.1f F"),(float)regValue/10.0);
//					if (regValue==0)
//					{
//						strresult=_T("OFF");
//					} 
//					else
//					{
//						strresult=_T("ON");
//					}
//				}
//				else if (5==product_register_value[RANGE_INPUT1+i-1])
//				{
//					if (regValue==0)
//					{
//						strresult=_T("ON");
//					} 
//					else
//					{
//						strresult=_T("OFF");
//					}
//				}
//				else if (6==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult.Format(_T("%d"),regValue);
//				}
//				else if (7==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult.Format(_T("%d"),regValue);
//				}
//				else if (8==product_register_value[RANGE_INPUT1+i-1])
//				{
//					//strresult=_T("TYPE3 10K C");
//					strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
//				}
//				else if (9==product_register_value[RANGE_INPUT1+i-1])
//				{
//					/*strresult=_T("TYPE3 10K F");*/
//					strresult.Format(_T("%0.1f F"),(float)regValue/10.0);
//				}
//				else if (10==product_register_value[RANGE_INPUT1+i-1])
//				{
//					strresult=_T("0");
//					//strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
//				}
//				else if (11==product_register_value[RANGE_INPUT1+i-1])
//				{
//					//strresult=_T("0-5V");
//					strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
//				}
//				else if (12==product_register_value[RANGE_INPUT1+i-1])
//				{
//					//strresult=_T("0-10V");
//					strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
//				}
//				else if (13==product_register_value[RANGE_INPUT1+i-1])
//				{
//					//strresult=_T("0-20I");
//					strresult.Format(_T("%0.1f ma"),(float)regValue/1000.0);
//				}
//
//
//
//
//				m_msflexgrid_input.put_TextMatrix(i,1,strresult);
//
//
//
//				strresult.Format(_T("%d"),product_register_value[FILTER_INPUT1+i-1]);
//				m_msflexgrid_input.put_TextMatrix(i,4,strresult);
//
//				strresult.Format(_T("%d min"),product_register_value[LIGHTING_ZONE_TIME_INPUT1+i-1]);
//				m_msflexgrid_input.put_TextMatrix(i,5,strresult);
//
//				strresult.Format(_T("%d min"),product_register_value[ZONE_TIME_LEFT_INPUT1+i-1]);
//				m_msflexgrid_input.put_TextMatrix(i,6,strresult);
//			}
//
//			CString CstresultDO;
//			for(int i = 1;i<=13;i++)
//			{  
//
//				CstresultDO.Format(_T("%d"),product_register_value[OUTPUT1+i-1]);
//				if (product_register_value[OUTPUT1+i-1]==0)
//				{
//					CstresultDO=_T("Off");
//				}
//				else
//				{
//					CstresultDO=_T("On");
//				}
//				m_msflexgrid_output.put_TextMatrix(i,1,CstresultDO);
//
//				if (product_register_value[LIGHT_SWITCH_OUTPUT1+i-1]>0)
//				{
//					CstresultDO=Get_Table_Name(m_sn,_T("Input"),product_register_value[LIGHT_SWITCH_OUTPUT1+i-1]);
//				}
//				else
//				{
//					CstresultDO=_T("UNUSED");
//				}
//				m_msflexgrid_output.put_TextMatrix(i,2,CstresultDO);
//				if (((product_register_value[AUTO_MANUAL_OUTPUTS]>>(i-1))&0x01)==1)
//				{
//					CstresultDO=_T("Manual");
//				} 
//				else
//				{
//					CstresultDO=_T("Auto");
//				}
//				m_msflexgrid_output.put_TextMatrix(i,3,CstresultDO);
//			}
//		}
//		
//	}
//	return CFormView::PreTranslateMessage(pMsg);
//}


LRESULT T38I13O::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	 
	if(message==WM_FRESH_T38I13O){
				if (no_mouse_keyboard_event_enable_refresh)
				{
					CString strresult;
					int regValue;
					for(int i = 1;i<=8;i++)
					{  
		
		
						strresult.Format(_T("%4d/%4d/%4d  %2d:%2d"),product_register_value[DATE_STAMP_INPUT1_YEAR+3*(i-1)],product_register_value[DATE_STAMP_INPUT1_MONTH+3*(i-1)],product_register_value[DATE_STAMP_INPUT1_DAY+3*(i-1)],product_register_value[DATE_STAMP_INPUT1_HOUR+3*(i-1)],product_register_value[DATE_STAMP_INPUT1_MINUTE+3*(i-1)]);
						m_msflexgrid_input.put_TextMatrix(i,2,strresult);
		
						strresult.Format(_T("%d"),product_register_value[RANGE_INPUT1+i-1]);
						if (0==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult=_T("RAW DATA");
		
						} 
						else if (1==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult=_T("10K C");
						}
						else if (2==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult=_T("10K F");
						}
						else if (3==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult=_T("0-100%");
						}
						else if (4==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult=_T("ON/OFF");
						}
						else if (5==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult=_T("OFF/ON");
						}
						else if (6==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult=_T("Pulse Input");
						}
						else if (7==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult=_T("Lighting Control");
						}
						else if (8==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult=_T("TYPE3 10K C");
						}
						else if (9==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult=_T("TYPE3 10K F");
						}
						else if (10==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult=_T("NO USE");
						}
						else if (11==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult=_T("0-5V");
						}
						else if (12==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult=_T("0-10V");
						}
						else if (13==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult=_T("4-20 ma");
						}
		
						m_msflexgrid_input.put_TextMatrix(i,3,strresult);
		
		
						regValue=(short)product_register_value[INPUT1_PULSE_COUNT_LOW+2*(i-1)];
		
						if (0==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult.Format(_T("%d"),regValue);
		
						} 
						else if (1==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult.Format(_T("%.1f C"),(float)regValue/10.0);
						}
						else if (2==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult=_T("10K F");
							strresult.Format(_T("%.1f F"),(float)regValue/10.0);
						}
						else if (3==product_register_value[RANGE_INPUT1+i-1])
						{
		
							strresult.Format(_T("%.1f"),(float)regValue);
							strresult+=_T("%");
						}
						else if (4==product_register_value[RANGE_INPUT1+i-1])
						{
							//strresult.Format(_T("%.1f F"),(float)regValue/10.0);
							if (regValue==0)
							{
								strresult=_T("OFF");
							} 
							else
							{
								strresult=_T("ON");
							}
						}
						else if (5==product_register_value[RANGE_INPUT1+i-1])
						{
							if (regValue==0)
							{
								strresult=_T("ON");
							} 
							else
							{
								strresult=_T("OFF");
							}
						}
						else if (6==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult.Format(_T("%d"),regValue);
						}
						else if (7==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult.Format(_T("%d"),regValue);
						}
						else if (8==product_register_value[RANGE_INPUT1+i-1])
						{
							//strresult=_T("TYPE3 10K C");
							strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
						}
						else if (9==product_register_value[RANGE_INPUT1+i-1])
						{
							/*strresult=_T("TYPE3 10K F");*/
							strresult.Format(_T("%0.1f F"),(float)regValue/10.0);
						}
						else if (10==product_register_value[RANGE_INPUT1+i-1])
						{
							strresult=_T("0");
							//strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
						}
						else if (11==product_register_value[RANGE_INPUT1+i-1])
						{
							//strresult=_T("0-5V");
							strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
						}
						else if (12==product_register_value[RANGE_INPUT1+i-1])
						{
							//strresult=_T("0-10V");
							strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
						}
						else if (13==product_register_value[RANGE_INPUT1+i-1])
						{
							//strresult=_T("0-20I");
							strresult.Format(_T("%0.1f ma"),(float)regValue/1000.0);
						}
		
		
		
		
						m_msflexgrid_input.put_TextMatrix(i,1,strresult);
		
		
		
						strresult.Format(_T("%d"),product_register_value[FILTER_INPUT1+i-1]);
						m_msflexgrid_input.put_TextMatrix(i,4,strresult);
		
						strresult.Format(_T("%d min"),product_register_value[LIGHTING_ZONE_TIME_INPUT1+i-1]);
						m_msflexgrid_input.put_TextMatrix(i,5,strresult);
		
						strresult.Format(_T("%d min"),product_register_value[ZONE_TIME_LEFT_INPUT1+i-1]);
						m_msflexgrid_input.put_TextMatrix(i,6,strresult);
					}
					bitset<16> BitSwitchValue(product_register_value[SWITCH1_STATUS]);

					int SwitchValue[13];
					for (int i=0;i<8;i++)
					{
						SwitchValue[i]=BitSwitchValue[2*i]+BitSwitchValue[2*i+1]*2;
					}

					bitset<16> BitSwitchValue2(product_register_value[SWITCH2_STATUS]);
					for (int i=8;i<13;i++)
					{
						SwitchValue[i]=BitSwitchValue2[2*(i-8)]+BitSwitchValue2[2*(i-8)+1]*2;
					}

					CString CstresultDO;
					for(int i = 1;i<=13;i++)
					{  
		
						CstresultDO.Format(_T("%d"),product_register_value[OUTPUT1+i-1]);
						if (product_register_value[OUTPUT1+i-1]==0)
						{
							CstresultDO=_T("Off");
						}
						else
						{
							CstresultDO=_T("On");
						}
						m_msflexgrid_output.put_TextMatrix(i,1,CstresultDO);
		
						if (product_register_value[LIGHT_SWITCH_OUTPUT1+i-1]>0)
						{
							CstresultDO=Get_Table_Name(m_sn,_T("Input"),product_register_value[LIGHT_SWITCH_OUTPUT1+i-1]);
						}
						else
						{
							CstresultDO=_T("UNUSED");
						}
						m_msflexgrid_output.put_TextMatrix(i,2,CstresultDO);
						if (((product_register_value[AUTO_MANUAL_OUTPUTS]>>(i-1))&0x01)==1)
						{
							CstresultDO=_T("Manual");
						} 
						else
						{
							CstresultDO=_T("Auto");
						}
						m_msflexgrid_output.put_TextMatrix(i,3,CstresultDO);
						m_msflexgrid_output.put_TextMatrix(i,4,STRING_SWITCH_STATUS[SwitchValue[i-1]]); 
					}
				}
				
			}
	return CFormView::WindowProc(message, wParam, lParam);
}


void T38I13O::OnBnClickedButtonReset()
{
	if(AfxMessageBox(_T(" This will reset the module to the factory defaults,Are you sure to reset it ?"))==IDOK)
	{
		//  write_one(g_tstat_id,299,1);
		write_one(g_tstat_id,300,1);
	}
}


void T38I13O::OnEnKillfocusEditResponseDelay()
{
    CString strValue;
    GetDlgItem(IDC_EDIT_RESPONSE_DELAY)->GetWindowText(strValue);
    int IntValue;
    IntValue = _wtoi(strValue);
    int ret = write_one(g_tstat_id,RESPONSE_DELAY,IntValue);
    if (ret>0)
    {
        product_register_value[RESPONSE_DELAY] = IntValue;
    }
    InitialDialog();
}
