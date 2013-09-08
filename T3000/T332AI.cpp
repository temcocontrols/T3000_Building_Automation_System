// T332AI.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T332AI.h"

#include "ado/ADO.h"
#include "globle_function.h"
#include "MainFrm.h"
// T332AI

IMPLEMENT_DYNCREATE(T332AI, CFormView)

T332AI::T332AI()
	: CFormView(T332AI::IDD)
{

}

T332AI::~T332AI()
{
}

void T332AI::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID_INPUT, m_msflexgrid_input);
	DDX_Control(pDX, IDC_EDIT_NAME, m_inNameEdt);
}

BEGIN_MESSAGE_MAP(T332AI, CFormView)
END_MESSAGE_MAP()


// T332AI diagnostics

#ifdef _DEBUG
void T332AI::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void T332AI::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void T332AI::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
#if 1	
	//设置排/行数量
	m_msflexgrid_input.put_Cols(2);
	m_msflexgrid_input.put_Rows(33);//包括标题栏
	//显示横标题
	m_msflexgrid_input.put_TextMatrix(0,0,_T("Input Name"));
	m_msflexgrid_input.put_TextMatrix(0,1,_T("Register Value"));
 




	//设置列宽	
	m_msflexgrid_input.put_ColWidth(0,1000);
	m_msflexgrid_input.put_ColWidth(1,1500);
 
	//居中显示
	for (int col=0;col<2;col++)
	{ 
		m_msflexgrid_input.put_ColAlignment(col,4);
	}

	//彩色显示
	for(int i=1;i<33;i++)		//排数量
	{

		for(int k=0;k<2;k++)	//列数量
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
	for(int i=1;i<33;i++)
	{

		str.Format(_T("Input%d"),i);

		m_msflexgrid_input.put_TextMatrix(i,0,str);	 
	}

 




#endif  
}
// T332AI message handlers
void T332AI::Initial_RegisterList()
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
	INPUT1	=	100	;
	INPUT2	=	101	;
	INPUT3	=	102	;
	INPUT4	=	103	;
	INPUT5	=	104	;
	INPUT6	=	105	;
	INPUT7	=	106	;
	INPUT8	=	107	;
	INPUT9	=	108	;
	INPUT10	=	109	;
	INPUT11	=	110	;
	INPUT12	=	111	;
	INPUT13	=	112	;
	INPUT14	=	113	;
	INPUT15	=	114	;
	INPUT16	=	115	;
	INPUT17	=	116	;
	INPUT18	=	117	;
	INPUT19	=	118	;
	INPUT20	=	119	;
	INPUT21	=	120	;
	INPUT22	=	121	;
	INPUT23	=	122	;
	INPUT24	=	123	;
	INPUT25	=	124	;
	INPUT26	=	125	;
	INPUT27	=	126	;
	INPUT28	=	127	;
	INPUT29	=	128	;
	INPUT30	=	129	;
	INPUT31	=	130	;
	INPUT32	=	131	;

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
		vartemp =m_ado.m_pRecordset->GetCollect(_T("T3-32AI"));
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
	INPUT1	=	Get_RegID(_T("	INPUT1	"))	;
	INPUT2	=	Get_RegID(_T("	INPUT2	"))	;
	INPUT3	=	Get_RegID(_T("	INPUT3	"))	;
	INPUT4	=	Get_RegID(_T("	INPUT4	"))	;
	INPUT5	=	Get_RegID(_T("	INPUT5	"))	;
	INPUT6	=	Get_RegID(_T("	INPUT6	"))	;
	INPUT7	=	Get_RegID(_T("	INPUT7	"))	;
	INPUT8	=	Get_RegID(_T("	INPUT8	"))	;
	INPUT9	=	Get_RegID(_T("	INPUT9	"))	;
	INPUT10	=	Get_RegID(_T("	INPUT10	"))	;
	INPUT11	=	Get_RegID(_T("	INPUT11	"))	;
	INPUT12	=	Get_RegID(_T("	INPUT12	"))	;
	INPUT13	=	Get_RegID(_T("	INPUT13	"))	;
	INPUT14	=	Get_RegID(_T("	INPUT14	"))	;
	INPUT15	=	Get_RegID(_T("	INPUT15	"))	;
	INPUT16	=	Get_RegID(_T("	INPUT16	"))	;
	INPUT17	=	Get_RegID(_T("	INPUT17	"))	;
	INPUT18	=	Get_RegID(_T("	INPUT18	"))	;
	INPUT19	=	Get_RegID(_T("	INPUT19	"))	;
	INPUT20	=	Get_RegID(_T("	INPUT20	"))	;
	INPUT21	=	Get_RegID(_T("	INPUT21	"))	;
	INPUT22	=	Get_RegID(_T("	INPUT22	"))	;
	INPUT23	=	Get_RegID(_T("	INPUT23	"))	;
	INPUT24	=	Get_RegID(_T("	INPUT24	"))	;
	INPUT25	=	Get_RegID(_T("	INPUT25	"))	;
	INPUT26	=	Get_RegID(_T("	INPUT26	"))	;
	INPUT27	=	Get_RegID(_T("	INPUT27	"))	;
	INPUT28	=	Get_RegID(_T("	INPUT28	"))	;
	INPUT29	=	Get_RegID(_T("	INPUT29	"))	;
	INPUT30	=	Get_RegID(_T("	INPUT30	"))	;
	INPUT31	=	Get_RegID(_T("	INPUT31	"))	;
	INPUT32	=	Get_RegID(_T("	INPUT32	"))	;



#endif





}

int  T332AI::Get_RegID(CString Name)
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


void T332AI::InitialDialog(){
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
	for(int i = 1;i<=32;i++)
	{  
		regValue=product_register_value[INPUT1+i-1];
		strresult.Format(_T("%d"),regValue);
		m_msflexgrid_input.put_TextMatrix(i,1,strresult);
	}

}
void T332AI::Fresh()
{
	InitialDialog();
}