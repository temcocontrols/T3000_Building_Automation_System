// T38IOAView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T38IOAView.h"
#include "ado/ADO.h"
#include "globle_function.h"
#include "MainFrm.h"

// CT38IOAView

IMPLEMENT_DYNCREATE(CT38IOAView, CFormView)

CT38IOAView::CT38IOAView()
	: CFormView(CT38IOAView::IDD)
{

}

CT38IOAView::~CT38IOAView()
{
}

void CT38IOAView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CT38IOAView, CFormView)
END_MESSAGE_MAP()


// CT38IOAView diagnostics

#ifdef _DEBUG
void CT38IOAView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CT38IOAView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG
void CT38IOAView::InitialRegister(){
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
		vartemp =m_ado.m_pRecordset->GetCollect(_T("T3-8AI8AO"));
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
	SN_LOW	            =	Get_RegID(_T("SN_LOW	            "));
	SN_HI	            =	Get_RegID(_T("SN_HI	            "));
	EPROM_VER_NUMBER	=	Get_RegID(_T("EPROM_VER_NUMBER	"));
	FIRMWARE_VER_NUMBER	=	Get_RegID(_T("FIRMWARE_VER_NUMBER	"));
	MODBUS_ID	        =	Get_RegID(_T("MODBUS_ID	        "));
	PRODUCT_MODEL	    =	Get_RegID(_T("PRODUCT_MODEL	    "));
	HARDWARE_VER_NUMBER	=	Get_RegID(_T("HARDWARE_VER_NUMBER	"));
	PIC_VER_NUMBER	    =	Get_RegID(_T("PIC_VER_NUMBER	 "));
	CALIBRATION_OUTPUTS	=	Get_RegID(_T("CALIBRATION_OUTPUTS"));
	BAUDRATE	        =	Get_RegID(_T("BAUDRATE	        "));
	RESPONSE_DELAY	    =	Get_RegID(_T("RESPONSE_DELAY	"));
	OUTPUT1	            =	Get_RegID(_T("OUTPUT1	        "));
	OUTPUT2	            =	Get_RegID(_T("OUTPUT2	        "));
	OUTPUT3         	=	Get_RegID(_T("OUTPUT3         	"));
	OUTPUT4	            =	Get_RegID(_T("OUTPUT4	        "));
	OUTPUT5	            =	Get_RegID(_T("OUTPUT5	        "));
	OUTPUT6	            =	Get_RegID(_T("OUTPUT6	        "));
	OUTPUT7	            =	Get_RegID(_T("OUTPUT7	        "));
	OUTPUT8	            =	Get_RegID(_T("OUTPUT8	        "));
	INPUT1	            =	Get_RegID(_T("INPUT1	        "));
	INPUT2	            =	Get_RegID(_T("INPUT2	        "));
	INPUT3	            =	Get_RegID(_T("INPUT3	        "));
	INPUT4	            =	Get_RegID(_T("INPUT4	        "));
	INPUT5	            =	Get_RegID(_T("INPUT5	        "));
	INPUT6	            =	Get_RegID(_T("INPUT6	        "));
	INPUT7	            =	Get_RegID(_T("INPUT7	        "));
	INPUT8	            =	Get_RegID(_T("INPUT8	        "));
	SWITCH1_STATUS	    =	Get_RegID(_T("SWITCH1_STATUS	"));
	SWITCH2_STATUS	    =	Get_RegID(_T("SWITCH2_STATUS	"));
	RANGE_INPUT1	    =	Get_RegID(_T("RANGE_INPUT1	    "));
	RANGE_INPUT2	    =	Get_RegID(_T("RANGE_INPUT2	    "));
	RANGE_INPUT3	    =	Get_RegID(_T("RANGE_INPUT3	    "));
	RANGE_INPUT4	    =	Get_RegID(_T("RANGE_INPUT4	    "));
	RANGE_INPUT5	    =	Get_RegID(_T("RANGE_INPUT5	    "));
	RANGE_INPUT6	    =	Get_RegID(_T("RANGE_INPUT6	    "));
	RANGE_INPUT7	    =	Get_RegID(_T("RANGE_INPUT7	    "));
	RANGE_INPUT8	    =	Get_RegID(_T("RANGE_INPUT8	    "));
	FILTER_INPUT1	    =	Get_RegID(_T("FILTER_INPUT1	    "));
	FILTER_INPUT2	    =	Get_RegID(_T("FILTER_INPUT2	    "));
	FILTER_INPUT3	    =	Get_RegID(_T("FILTER_INPUT3	    "));
	FILTER_INPUT4	    =	Get_RegID(_T("FILTER_INPUT4	    "));
	FILTER_INPUT5	    =	Get_RegID(_T("FILTER_INPUT5	    "));
	FILTER_INPUT6	    =	Get_RegID(_T("FILTER_INPUT6	    "));
	FILTER_INPUT7	    =	Get_RegID(_T("FILTER_INPUT7	    "));
	FILTER_INPUT8	    =	Get_RegID(_T("FILTER_INPUT8	    "));

}
int  CT38IOAView::Get_RegID(CString Name)
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
void CT38IOAView::Fresh(){
	InitialRegister();
}
void CT38IOAView::OnInitialUpdate(){
	CFormView::OnInitialUpdate();
}