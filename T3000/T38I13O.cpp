// T38I13O.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T38I13O.h"
#include "ado/ADO.h"
#include "globle_function.h"
#include "MainFrm.h"

// T38I13O

IMPLEMENT_DYNCREATE(T38I13O, CFormView)

T38I13O::T38I13O()
	: CFormView(T38I13O::IDD)
{

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
}

BEGIN_MESSAGE_MAP(T38I13O, CFormView)
	ON_CBN_SELCHANGE(IDC_RANGECOMBO, &T38I13O::OnCbnSelchangeRangecombo)
	ON_EN_KILLFOCUS(IDC_EDIT_NAME, &T38I13O::OnEnKillfocusEditName)
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
//������/������
m_msflexgrid_input.put_Cols(5);
m_msflexgrid_input.put_Rows(9);//����������
//��ʾ�����
m_msflexgrid_input.put_TextMatrix(0,0,_T("Input Name"));
m_msflexgrid_input.put_TextMatrix(0,1,_T("Register Value"));
m_msflexgrid_input.put_TextMatrix(0,2,_T("Date Stamp"));
m_msflexgrid_input.put_TextMatrix(0,3,_T("Range"));
m_msflexgrid_input.put_TextMatrix(0,4,_T("Filter"));




//�����п�	
m_msflexgrid_input.put_ColWidth(0,1000);
m_msflexgrid_input.put_ColWidth(1,1500);
m_msflexgrid_input.put_ColWidth(2,1500);
m_msflexgrid_input.put_ColWidth(3,1500);
m_msflexgrid_input.put_ColWidth(4,1000);
//������ʾ
for (int col=0;col<5;col++)
{ 
	m_msflexgrid_input.put_ColAlignment(col,4);
}

//��ɫ��ʾ
for(int i=1;i<9;i++)		//������
{

	for(int k=0;k<5;k++)	//������
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




//��ʾ�ݱ���
CString str;
for(int i=1;i<9;i++)
{

	str.Format(_T("Input%d"),i);

	m_msflexgrid_input.put_TextMatrix(i,0,str);	 
}

//============================================================================================================����Output DO�����б��

//������/������
m_msflexgrid_output.put_Rows(14);
m_msflexgrid_output.put_Cols(3);
//�����п�	


//��ʾ�����
m_msflexgrid_output.put_TextMatrix(0,0,_T("Output Name"));
m_msflexgrid_output.put_TextMatrix(0,1,_T("Output Value"));
m_msflexgrid_output.put_TextMatrix(0,2,_T("Light Switch"));
//m_msflexgrid_output.put_TextMatrix(0,3,_T("Auto/Manual"));

m_msflexgrid_output.put_ColWidth(0,1000);
m_msflexgrid_output.put_ColWidth(1,1500);
m_msflexgrid_output.put_ColWidth(2,1500);
//m_msflexgrid_output.put_ColWidth(3,1500);//������ʾ
for (int col=0;col<3;col++)
{ 
	m_msflexgrid_output.put_ColAlignment(col,4);
}

//��ɫ��ʾ
for(int i=1;i<14;i++)		//������
{

	for(int k=0;k<3;k++)	//������
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
//��ʾ�ݱ���
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
	T3Register temp;
	CADO m_ado;
	m_ado.OnInitADOConn();
#if 1
	CString SQL = _T("select * from T3_RegisterList");
	m_ado.m_pRecordset = m_ado.OpenRecordset(SQL);
	_variant_t vartemp;
	while(!m_ado.m_pRecordset->EndOfFile)
	{
		temp.regID=m_ado.m_pRecordset->GetCollect(_T("RegID"));
		vartemp =m_ado.m_pRecordset->GetCollect(_T("T3-8I13O"));
		if (vartemp.vt==VT_NULL)
			temp.regName=_T("");
		else
			temp.regName =vartemp;
		m_ado.m_pRecordset->MoveNext();
		m_vecT3Register.push_back(temp);
	}
	m_ado.CloseRecordset();
	m_ado.CloseConn();
#endif 


#if 1
	SN_LOW	=	Get_RegID(_T("	SN_LOW	"))	;
	SN_HI	=	Get_RegID(_T("	SN_HI	"))	;
	EPROM_VER_NUMBER	=	Get_RegID(_T("	EPROM_VER_NUMBER	"))	;
	FIRMWARE_VER_NUMBER	=	Get_RegID(_T("	FIRMWARE_VER_NUMBER	"))	;
	MODBUS_ID	=	Get_RegID(_T("	MODBUS_ID	"))	;
	PRODUCT_MODEL	=	Get_RegID(_T("	PRODUCT_MODEL	"))	;
	HARDWARE_VER_NUMBER	=	Get_RegID(_T("	HARDWARE_VER_NUMBER	"))	;
	PIC_VER_NUMBER	=	Get_RegID(_T("	PIC_VER_NUMBER	"))	;
	CALIBRATION_OUTPUTS	=	Get_RegID(_T("	CALIBRATION_OUTPUTS	"))	;
	BAUDRATE	=	Get_RegID(_T("	BAUDRATE	"))	;
	RESPONSE_DELAY	=	Get_RegID(_T("	RESPONSE_DELAY	"))	;
	OUTPUT1	=	Get_RegID(_T("	OUTPUT1	"))	;
	OUTPUT2	=	Get_RegID(_T("	OUTPUT2	"))	;
	OUTPUT3	=	Get_RegID(_T("	OUTPUT3	"))	;
	OUTPUT4	=	Get_RegID(_T("	OUTPUT4	"))	;
	OUTPUT5	=	Get_RegID(_T("	OUTPUT5	"))	;
	OUTPUT6	=	Get_RegID(_T("	OUTPUT6	"))	;
	OUTPUT7	=	Get_RegID(_T("	OUTPUT7	"))	;
	OUTPUT8	=	Get_RegID(_T("	OUTPUT8	"))	;
	OUTPUT9	=	Get_RegID(_T("	OUTPUT9	"))	;
	OUTPUT10	=	Get_RegID(_T("	OUTPUT10	"))	;
	OUTPUT11	=	Get_RegID(_T("	OUTPUT11	"))	;
	OUTPUT12	=	Get_RegID(_T("	OUTPUT12	"))	;
	OUTPUT13	=	Get_RegID(_T("	OUTPUT13	"))	;
	SWITCH1_STATUS	=	Get_RegID(_T("	SWITCH1_STATUS	"))	;
	SWITCH2_STATUS	=	Get_RegID(_T("	SWITCH2_STATUS	"))	;
	INPUT1_PULSE_COUNT_HIGHT	=	Get_RegID(_T("	INPUT1_PULSE_COUNT_HIGHT	"))	;
	INPUT1_PULSE_COUNT_LOW	=	Get_RegID(_T("	INPUT1_PULSE_COUNT_LOW	"))	;
	INPUT2_PULSE_COUNT_HIGHT	=	Get_RegID(_T("	INPUT2_PULSE_COUNT_HIGHT	"))	;
	INPUT2_PULSE_COUNT_LOW	=	Get_RegID(_T("	INPUT2_PULSE_COUNT_LOW	"))	;
	INPUT3_PULSE_COUNT_HIGHT	=	Get_RegID(_T("	INPUT3_PULSE_COUNT_HIGHT	"))	;
	INPUT3_PULSE_COUNT_LOW	=	Get_RegID(_T("	INPUT3_PULSE_COUNT_LOW	"))	;
	INPUT4_PULSE_COUNT_HIGHT	=	Get_RegID(_T("	INPUT4_PULSE_COUNT_HIGHT	"))	;
	INPUT4_PULSE_COUNT_LOW	=	Get_RegID(_T("	INPUT4_PULSE_COUNT_LOW	"))	;
	INPUT5_PULSE_COUNT_HIGHT	=	Get_RegID(_T("	INPUT5_PULSE_COUNT_HIGHT	"))	;
	INPUT5_PULSE_COUNT_LOW	=	Get_RegID(_T("	INPUT5_PULSE_COUNT_LOW	"))	;
	INPUT6_PULSE_COUNT_HIGHT	=	Get_RegID(_T("	INPUT6_PULSE_COUNT_HIGHT	"))	;
	INPUT6_PULSE_COUNT_LOW	=	Get_RegID(_T("	INPUT6_PULSE_COUNT_LOW	"))	;
	INPUT7_PULSE_COUNT_HIGHT	=	Get_RegID(_T("	INPUT7_PULSE_COUNT_HIGHT	"))	;
	INPUT7_PULSE_COUNT_LOW	=	Get_RegID(_T("	INPUT7_PULSE_COUNT_LOW	"))	;
	INPUT8_PULSE_COUNT_HIGHT	=	Get_RegID(_T("	INPUT8_PULSE_COUNT_HIGHT	"))	;
	INPUT8_PULSE_COUNT_LOW	=	Get_RegID(_T("	INPUT8_PULSE_COUNT_LOW	"))	;
	DATE_STAMP_INPUT1_YEAR	=	Get_RegID(_T("	DATE_STAMP_INPUT1_YEAR	"))	;
	DATE_STAMP_INPUT1_MONTH	=	Get_RegID(_T("	DATE_STAMP_INPUT1_MONTH	"))	;
	DATE_STAMP_INPUT1_DAY	=	Get_RegID(_T("	DATE_STAMP_INPUT1_DAY	"))	;
	DATE_STAMP_INPUT1_HOUR	=	Get_RegID(_T("	DATE_STAMP_INPUT1_HOUR	"))	;
	DATE_STAMP_INPUT1_MINUTE	=	Get_RegID(_T("	DATE_STAMP_INPUT1_MINUTE	"))	;
	DATE_STAMP_INPUT2_YEAR	=	Get_RegID(_T("	DATE_STAMP_INPUT2_YEAR	"))	;
	DATE_STAMP_INPUT2_MONTH	=	Get_RegID(_T("	DATE_STAMP_INPUT2_MONTH	"))	;
	DATE_STAMP_INPUT2_DAY	=	Get_RegID(_T("	DATE_STAMP_INPUT2_DAY	"))	;
	DATE_STAMP_INPUT2_HOUR	=	Get_RegID(_T("	DATE_STAMP_INPUT2_HOUR	"))	;
	DATE_STAMP_INPUT2_MINUTE	=	Get_RegID(_T("	DATE_STAMP_INPUT2_MINUTE	"))	;
	DATE_STAMP_INPUT3_YEAR	=	Get_RegID(_T("	DATE_STAMP_INPUT3_YEAR	"))	;
	DATE_STAMP_INPUT3_MONTH	=	Get_RegID(_T("	DATE_STAMP_INPUT3_MONTH	"))	;
	DATE_STAMP_INPUT3_DAY	=	Get_RegID(_T("	DATE_STAMP_INPUT3_DAY	"))	;
	DATE_STAMP_INPUT3_HOUR	=	Get_RegID(_T("	DATE_STAMP_INPUT3_HOUR	"))	;
	DATE_STAMP_INPUT3_MINUTE	=	Get_RegID(_T("	DATE_STAMP_INPUT3_MINUTE	"))	;
	DATE_STAMP_INPUT4_YEAR	=	Get_RegID(_T("	DATE_STAMP_INPUT4_YEAR	"))	;
	DATE_STAMP_INPUT4_MONTH	=	Get_RegID(_T("	DATE_STAMP_INPUT4_MONTH	"))	;
	DATE_STAMP_INPUT4_DAY	=	Get_RegID(_T("	DATE_STAMP_INPUT4_DAY	"))	;
	DATE_STAMP_INPUT4_HOUR	=	Get_RegID(_T("	DATE_STAMP_INPUT4_HOUR	"))	;
	DATE_STAMP_INPUT4_MINUTE	=	Get_RegID(_T("	DATE_STAMP_INPUT4_MINUTE	"))	;
	DATE_STAMP_INPUT5_YEAR	=	Get_RegID(_T("	DATE_STAMP_INPUT5_YEAR	"))	;
	DATE_STAMP_INPUT5_MONTH	=	Get_RegID(_T("	DATE_STAMP_INPUT5_MONTH	"))	;
	DATE_STAMP_INPUT5_DAY	=	Get_RegID(_T("	DATE_STAMP_INPUT5_DAY	"))	;
	DATE_STAMP_INPUT5_HOUR	=	Get_RegID(_T("	DATE_STAMP_INPUT5_HOUR	"))	;
	DATE_STAMP_INPUT5_MINUTE	=	Get_RegID(_T("	DATE_STAMP_INPUT5_MINUTE	"))	;
	DATE_STAMP_INPUT6_YEAR	=	Get_RegID(_T("	DATE_STAMP_INPUT6_YEAR	"))	;
	DATE_STAMP_INPUT6_MONTH	=	Get_RegID(_T("	DATE_STAMP_INPUT6_MONTH	"))	;
	DATE_STAMP_INPUT6_DAY	=	Get_RegID(_T("	DATE_STAMP_INPUT6_DAY	"))	;
	DATE_STAMP_INPUT6_HOUR	=	Get_RegID(_T("	DATE_STAMP_INPUT6_HOUR	"))	;
	DATE_STAMP_INPUT6_MINUTE	=	Get_RegID(_T("	DATE_STAMP_INPUT6_MINUTE	"))	;
	DATE_STAMP_INPUT7_YEAR	=	Get_RegID(_T("	DATE_STAMP_INPUT7_YEAR	"))	;
	DATE_STAMP_INPUT7_MONTH	=	Get_RegID(_T("	DATE_STAMP_INPUT7_MONTH	"))	;
	DATE_STAMP_INPUT7_DAY	=	Get_RegID(_T("	DATE_STAMP_INPUT7_DAY	"))	;
	DATE_STAMP_INPUT7_HOUR	=	Get_RegID(_T("	DATE_STAMP_INPUT7_HOUR	"))	;
	DATE_STAMP_INPUT7_MINUTE	=	Get_RegID(_T("	DATE_STAMP_INPUT7_MINUTE	"))	;
	DATE_STAMP_INPUT8_YEAR	=	Get_RegID(_T("	DATE_STAMP_INPUT8_YEAR	"))	;
	DATE_STAMP_INPUT8_MONTH	=	Get_RegID(_T("	DATE_STAMP_INPUT8_MONTH	"))	;
	DATE_STAMP_INPUT8_DAY	=	Get_RegID(_T("	DATE_STAMP_INPUT8_DAY	"))	;
	DATE_STAMP_INPUT8_HOUR	=	Get_RegID(_T("	DATE_STAMP_INPUT8_HOUR	"))	;
	DATE_STAMP_INPUT8_MINUTE	=	Get_RegID(_T("	DATE_STAMP_INPUT8_MINUTE	"))	;
	ORIGIGINAL_AD_DATA_INPUT1	=	Get_RegID(_T("	ORIGIGINAL_AD_DATA_INPUT1	"))	;
	ORIGIGINAL_AD_DATA_INPUT2	=	Get_RegID(_T("	ORIGIGINAL_AD_DATA_INPUT2	"))	;
	ORIGIGINAL_AD_DATA_INPUT3	=	Get_RegID(_T("	ORIGIGINAL_AD_DATA_INPUT3	"))	;
	ORIGIGINAL_AD_DATA_INPUT4	=	Get_RegID(_T("	ORIGIGINAL_AD_DATA_INPUT4	"))	;
	ORIGIGINAL_AD_DATA_INPUT5	=	Get_RegID(_T("	ORIGIGINAL_AD_DATA_INPUT5	"))	;
	ORIGIGINAL_AD_DATA_INPUT6	=	Get_RegID(_T("	ORIGIGINAL_AD_DATA_INPUT6	"))	;
	ORIGIGINAL_AD_DATA_INPUT7	=	Get_RegID(_T("	ORIGIGINAL_AD_DATA_INPUT7	"))	;
	ORIGIGINAL_AD_DATA_INPUT8	=	Get_RegID(_T("	ORIGIGINAL_AD_DATA_INPUT8	"))	;
	RANGE_INPUT1	=	Get_RegID(_T("	RANGE_INPUT1	"))	;
	RANGE_INPUT2	=	Get_RegID(_T("	RANGE_INPUT2	"))	;
	RANGE_INPUT3	=	Get_RegID(_T("	RANGE_INPUT3	"))	;
	RANGE_INPUT4	=	Get_RegID(_T("	RANGE_INPUT4	"))	;
	RANGE_INPUT5	=	Get_RegID(_T("	RANGE_INPUT5	"))	;
	RANGE_INPUT6	=	Get_RegID(_T("	RANGE_INPUT6	"))	;
	RANGE_INPUT7	=	Get_RegID(_T("	RANGE_INPUT7	"))	;
	RANGE_INPUT8	=	Get_RegID(_T("	RANGE_INPUT8	"))	;
	FILTER_INPUT1	=	Get_RegID(_T("	FILTER_INPUT1	"))	;
	FILTER_INPUT2	=	Get_RegID(_T("	FILTER_INPUT2	"))	;
	FILTER_INPUT3	=	Get_RegID(_T("	FILTER_INPUT3	"))	;
	FILTER_INPUT4	=	Get_RegID(_T("	FILTER_INPUT4	"))	;
	FILTER_INPUT5	=	Get_RegID(_T("	FILTER_INPUT5	"))	;
	FILTER_INPUT6	=	Get_RegID(_T("	FILTER_INPUT6	"))	;
	FILTER_INPUT7	=	Get_RegID(_T("	FILTER_INPUT7	"))	;
	FILTER_INPUT8	=	Get_RegID(_T("	FILTER_INPUT8	"))	;
	LIGHTING_ZONE_TIME_INPUT1	=	Get_RegID(_T("	LIGHTING_ZONE_TIME_INPUT1	"))	;
	LIGHTING_ZONE_TIME_INPUT2	=	Get_RegID(_T("	LIGHTING_ZONE_TIME_INPUT2	"))	;
	LIGHTING_ZONE_TIME_INPUT3	=	Get_RegID(_T("	LIGHTING_ZONE_TIME_INPUT3	"))	;
	LIGHTING_ZONE_TIME_INPUT4	=	Get_RegID(_T("	LIGHTING_ZONE_TIME_INPUT4	"))	;
	LIGHTING_ZONE_TIME_INPUT5	=	Get_RegID(_T("	LIGHTING_ZONE_TIME_INPUT5	"))	;
	LIGHTING_ZONE_TIME_INPUT6	=	Get_RegID(_T("	LIGHTING_ZONE_TIME_INPUT6	"))	;
	LIGHTING_ZONE_TIME_INPUT7	=	Get_RegID(_T("	LIGHTING_ZONE_TIME_INPUT7	"))	;
	LIGHTING_ZONE_TIME_INPUT8	=	Get_RegID(_T("	LIGHTING_ZONE_TIME_INPUT8	"))	;
	ZONE_TIME_LEFT_INPUT1	=	Get_RegID(_T("	ZONE_TIME_LEFT_INPUT1	"))	;
	ZONE_TIME_LEFT_INPUT2	=	Get_RegID(_T("	ZONE_TIME_LEFT_INPUT2	"))	;
	ZONE_TIME_LEFT_INPUT3	=	Get_RegID(_T("	ZONE_TIME_LEFT_INPUT3	"))	;
	ZONE_TIME_LEFT_INPUT4	=	Get_RegID(_T("	ZONE_TIME_LEFT_INPUT4	"))	;
	ZONE_TIME_LEFT_INPUT5	=	Get_RegID(_T("	ZONE_TIME_LEFT_INPUT5	"))	;
	ZONE_TIME_LEFT_INPUT6	=	Get_RegID(_T("	ZONE_TIME_LEFT_INPUT6	"))	;
	ZONE_TIME_LEFT_INPUT7	=	Get_RegID(_T("	ZONE_TIME_LEFT_INPUT7	"))	;
	ZONE_TIME_LEFT_INPUT8	=	Get_RegID(_T("	ZONE_TIME_LEFT_INPUT8	"))	;
	AUTO_MANUAL_OUTPUTS	=	Get_RegID(_T("	AUTO_MANUAL_OUTPUTS	"))	;
	LIGHT_SWITCH_OUTPUT1	=	Get_RegID(_T("	LIGHT_SWITCH_OUTPUT1	"))	;
	LIGHT_SWITCH_OUTPUT2	=	Get_RegID(_T("	LIGHT_SWITCH_OUTPUT2	"))	;
	LIGHT_SWITCH_OUTPUT3	=	Get_RegID(_T("	LIGHT_SWITCH_OUTPUT3	"))	;
	LIGHT_SWITCH_OUTPUT4	=	Get_RegID(_T("	LIGHT_SWITCH_OUTPUT4	"))	;
	LIGHT_SWITCH_OUTPUT5	=	Get_RegID(_T("	LIGHT_SWITCH_OUTPUT5	"))	;
	LIGHT_SWITCH_OUTPUT6	=	Get_RegID(_T("	LIGHT_SWITCH_OUTPUT6	"))	;
	LIGHT_SWITCH_OUTPUT7	=	Get_RegID(_T("	LIGHT_SWITCH_OUTPUT7	"))	;
	LIGHT_SWITCH_OUTPUT8	=	Get_RegID(_T("	LIGHT_SWITCH_OUTPUT8	"))	;
	LIGHT_SWITCH_OUTPUT9	=	Get_RegID(_T("	LIGHT_SWITCH_OUTPUT9	"))	;
	LIGHT_SWITCH_OUTPUT10	=	Get_RegID(_T("	LIGHT_SWITCH_OUTPUT10	"))	;
	LIGHT_SWITCH_OUTPUT11	=	Get_RegID(_T("	LIGHT_SWITCH_OUTPUT11	"))	;
	LIGHT_SWITCH_OUTPUT12	=	Get_RegID(_T("	LIGHT_SWITCH_OUTPUT12	"))	;
	LIGHT_SWITCH_OUTPUT13	=	Get_RegID(_T("	LIGHT_SWITCH_OUTPUT13	"))	;


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
// Input
CString strresult;
int regValue;
for(int i = 1;i<=8;i++)
{  
	regValue=product_register_value[INPUT1_PULSE_COUNT_HIGHT+2*(i-1)]*65535+product_register_value[INPUT1_PULSE_COUNT_LOW+2*(i-1)];
	strresult.Format(_T("%d"),regValue);
	m_msflexgrid_input.put_TextMatrix(i,1,strresult);

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
		strresult=_T("0-20I");
	}

	m_msflexgrid_input.put_TextMatrix(i,3,strresult);
	strresult.Format(_T("%d"),product_register_value[FILTER_INPUT1+i-1]);
	m_msflexgrid_input.put_TextMatrix(i,4,strresult);
}

CString CstresultDO;
for(int i = 1;i<=13;i++)
{  

	CstresultDO.Format(_T("%d"),product_register_value[OUTPUT1+i-1]);
	m_msflexgrid_output.put_TextMatrix(i,1,CstresultDO);
	//m_msflexgrid_output.put_TextMatrix(i,2,vect3model.at(100-1+i).CStName);
	CstresultDO.Format(_T("%d"),product_register_value[LIGHT_SWITCH_OUTPUT1+i-1]);
	m_msflexgrid_output.put_TextMatrix(i,2,CstresultDO);
 

}
}
void T38I13O::Fresh()
{
InitialDialog();
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
	lRow = m_msflexgrid_input.get_RowSel();//��ȡ������к�	
	lCol = m_msflexgrid_input.get_ColSel(); //��ȡ������к�
	TRACE(_T("Click input grid!\n"));

	CRect rect;
	m_msflexgrid_input.GetWindowRect(rect); //��ȡ���ؼ��Ĵ��ھ���
	ScreenToClient(rect); //ת��Ϊ�ͻ�������	
	CDC* pDC =GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//����ѡ�и�����Ͻǵ�����(����Ϊ��λ)
	long y = m_msflexgrid_input.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_msflexgrid_input.get_ColPos(lCol)/nTwipsPerDotX;
	//����ѡ�и�ĳߴ�(����Ϊ��λ)����1��ʵ�ʵ����У����ּ�1��Ч������
	long width = m_msflexgrid_input.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_msflexgrid_input.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//�γ�ѡ�и����ڵľ�������
	CRect rcCell(x,y,x+width,y+height);
	//ת������ԶԻ��������
	rcCell.OffsetRect(rect.left+1,rect.top+1);
	ReleaseDC(pDC);
	CString strValue = m_msflexgrid_input.get_TextMatrix(lRow,lCol);
	m_curcol=lCol;
	m_currow=lRow;


	if(3!=lCol && lRow != 0)
	{

		m_inNameEdt.MoveWindow(&rcCell,1);
		m_inNameEdt.ShowWindow(SW_SHOW);
		m_inNameEdt.SetWindowText(strValue);
		m_inNameEdt.SetFocus();
		m_inNameEdt.SetCapture();//LSC
		int nLenth=strValue.GetLength();
		m_inNameEdt.SetSel(nLenth,nLenth); //ȫѡ//

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
		m_comboxRange.AddString(_T("0-20I"));

		m_comboxRange.ShowWindow(SW_SHOW);



		m_comboxRange.BringWindowToTop();

		m_comboxRange.SetFocus(); //��ȡ����
		m_comboxRange.SetWindowText(strValue);
}

}
void T38I13O::OnCbnSelchangeRangecombo()
	{
		BeginWaitCursor();
		CString strTemp;
		int lRow = m_msflexgrid_input.get_RowSel();	
		int lCol = m_msflexgrid_input.get_ColSel();
		//if ((strText.GetLength()<=0)||strText.CompareNoCase(m_tempGridString)==0)
		//{
		//	return;
		//}
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
		InitialDialog();
		EndWaitCursor();
	}


void T38I13O::ClickMsflexgridOutput()
	{
	    m_isinput=FALSE;
		long lRow,lCol;
		lRow = m_msflexgrid_output.get_RowSel();//��ȡ������к�	
		lCol = m_msflexgrid_output.get_ColSel(); //��ȡ������к�
		TRACE(_T("Click input grid!\n"));

		CRect rect;
		m_msflexgrid_output.GetWindowRect(rect); //��ȡ���ؼ��Ĵ��ھ���
		ScreenToClient(rect); //ת��Ϊ�ͻ�������	
		CDC* pDC =GetDC();

		int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
		int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
		//����ѡ�и�����Ͻǵ�����(����Ϊ��λ)
		long y = m_msflexgrid_output.get_RowPos(lRow)/nTwipsPerDotY;
		long x = m_msflexgrid_output.get_ColPos(lCol)/nTwipsPerDotX;
		//����ѡ�и�ĳߴ�(����Ϊ��λ)����1��ʵ�ʵ����У����ּ�1��Ч������
		long width = m_msflexgrid_output.get_ColWidth(lCol)/nTwipsPerDotX+1;
		long height = m_msflexgrid_output.get_RowHeight(lRow)/nTwipsPerDotY+1;
		//�γ�ѡ�и����ڵľ�������
		CRect rcCell(x,y,x+width,y+height);
		//ת������ԶԻ��������
		rcCell.OffsetRect(rect.left+1,rect.top+1);
		ReleaseDC(pDC);
		CString strValue = m_msflexgrid_output.get_TextMatrix(lRow,lCol);
		m_curcol=lCol;
		m_currow=lRow;

		if(lRow != 0)
		{
			//return; // 2012.2.7��ë˵�������޸�
			m_inNameEdt.MoveWindow(&rcCell,1);
			m_inNameEdt.ShowWindow(SW_SHOW);
			m_inNameEdt.SetWindowText(strValue);
			m_inNameEdt.SetFocus();
			m_inNameEdt.SetCapture();//LSC
			int nLenth=strValue.GetLength();
			m_inNameEdt.SetSel(nLenth,nLenth); //ȫѡ//

		}
	}


void T38I13O::OnEnKillfocusEditName()
{
    CString strTemp;
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(strTemp);
	 int Value=_wtoi(strTemp);
	if (m_isinput)
	{
	  if (m_curcol==1)//Value
	  {
	    int RegValue=product_register_value[INPUT1_PULSE_COUNT_HIGHT+2*(m_currow-1)]*65535+product_register_value[INPUT1_PULSE_COUNT_LOW+2*(m_currow-1)];

	     int high;
		 int low;
	    if(RegValue==Value)
	    {
		return;
	    } 
	    else
	    {
		high=Value/65535;
		low=Value%65535;
	    int ret1=write_one(g_tstat_id,INPUT1_PULSE_COUNT_HIGHT+2*(m_currow-1),high);
		int ret2=write_one(g_tstat_id,INPUT1_PULSE_COUNT_LOW+2*(m_currow-1),low);
		if (ret1>0&&ret2>0)
		{
		product_register_value[INPUT1_PULSE_COUNT_HIGHT+2*(m_currow-1)]=high;
		product_register_value[INPUT1_PULSE_COUNT_LOW+2*(m_currow-1)]=low;
		InitialDialog();
		} 
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
	}
	else
	{

	}
}
