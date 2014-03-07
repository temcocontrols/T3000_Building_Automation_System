// T3RTDView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T3RTDView.h"

#include "Dialog_Progess.h"
#include "ado/ADO.h"
#include "globle_function.h"
#include "MainFrm.h"
#include "EreaseDlg.h"

// CT3RTDView

DWORD WINAPI _BackFreshing_RTD(LPVOID pParam)
{
    CT3RTDView* dlg=(CT3RTDView*)(pParam);

    Sleep(1000);


    while(1)
    {
        
        if (!is_connect())
        {
           Sleep(1000);
            continue;
        }
        Sleep(1000);
        
        for(int i=1;i<3;i++) //Modify by Fance , tstat 6 has more register than 512;
        {
            register_critical_section.Lock();

            Read_Multi(g_tstat_id,&product_register_value[i*100],i*100,100);
            register_critical_section.Unlock();
        }
     
        //memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));
        dlg->InitialDialog();

    }



    return 1;
}
IMPLEMENT_DYNCREATE(CT3RTDView, CFormView)

CT3RTDView::CT3RTDView()
	: CFormView(CT3RTDView::IDD)
{

}

CT3RTDView::~CT3RTDView()
{

    //CMainFrame*pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
    //pMain->m_pFreshMultiRegisters->ResumeThread();
    ////	pMain->m_pFreshTree->SuspendThread();
    //pMain->m_pRefreshThread->ResumeThread();	//不能加这一段，否则 退出T3000的时候 报错; Fance comments
    if(hFirstThread != NULL)
        TerminateThread(hFirstThread, 0);
}

void CT3RTDView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MSFLEXGRID_INPUT, m_msflexgrid_input);
    DDX_Control(pDX, IDC_COMBO1, m_dataformat);
    DDX_Control(pDX, IDC_EDIT1, m_accuracy);
    DDX_Control(pDX, IDC_RANGE, m_comboxRange);
    DDX_Control(pDX, IDC_EDIT_NAME, m_inNameEdt);
}

BEGIN_MESSAGE_MAP(CT3RTDView, CFormView)
    ON_BN_CLICKED(IDC_CHANGEID, &CT3RTDView::OnBnClickedChangeid)
    ON_CBN_SELCHANGE(IDC_COMBO1, &CT3RTDView::OnCbnSelchangeCombo1)
    ON_EN_KILLFOCUS(IDC_EDIT1, &CT3RTDView::OnEnKillfocusEdit1)
    ON_CBN_SELCHANGE(IDC_RANGE, &CT3RTDView::OnCbnSelchangeRange)
    ON_EN_KILLFOCUS(IDC_EDIT_NAME, &CT3RTDView::OnEnKillfocusEdit2)
END_MESSAGE_MAP()


// CT3RTDView diagnostics

#ifdef _DEBUG
void CT3RTDView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CT3RTDView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CT3RTDView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    m_dataformat.ResetContent();
    m_dataformat.AddString(_T("Float"));
    m_dataformat.AddString(_T("Int"));;
//#if 1	
    //设置排/行数量
    m_msflexgrid_input.put_Cols(6);
    m_msflexgrid_input.put_Rows(10+1); 
    m_msflexgrid_input.put_TextMatrix(0,0,_T("Channel N0"));
    m_msflexgrid_input.put_TextMatrix(0,1,_T("Channel Name"));
    m_msflexgrid_input.put_TextMatrix(0,2,_T("Value"));
    m_msflexgrid_input.put_TextMatrix(0,3,_T("Range"));
    m_msflexgrid_input.put_TextMatrix(0,4,_T("Filter"));
    m_msflexgrid_input.put_TextMatrix(0,5,_T("Status"));
    //设置列宽	
    m_msflexgrid_input.put_ColWidth(0,1000);
    m_msflexgrid_input.put_ColWidth(1,1500);
  
    m_msflexgrid_input.put_ColWidth(2,1500);
    m_msflexgrid_input.put_ColWidth(3,1000);
    m_msflexgrid_input.put_ColWidth(4,1000);
    m_msflexgrid_input.put_ColWidth(5,1000);
    //居中显示
    for (int col=0;col<6;col++)
    { 
        m_msflexgrid_input.put_ColAlignment(col,4);
    }
    //彩色显示
    for(int i=1;i<10+1;i++)		//排数量
    {
        for(int k=0;k<6;k++)	//列数量
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
    for(int i=1;i<10+1;i++)
    {

        str.Format(_T("%d"),i);

        m_msflexgrid_input.put_TextMatrix(i,0,str);	 
    }
}
void CT3RTDView::Initial_RegisterList()
{
#if 1
    SN_LOW	=	0	;
    SN_HI	=	2	;
    EPROM_VER_NUMBER	=	4	;
    FIRMWARE_VER_NUMBER	=	5	;
    MODBUS_ID	=	6	;
    PRODUCT_MODEL	=	7	;
    HARDWARE_VER_NUMBER	=	8	;
    BAUDRATE	=	9	;
    FLAG_CALIBRATION	=	12	;
    MODBUS_FORMAT	=	13	;
    DEADMASTER_TIME	=	17	;
    RESOLUTION	=	18	;
    CHANNEL1_LOW	=	100	;
    CHANNEL1_HIGHT	=	101	;
    CHANNEL2_LOW	=	102	;
    CHANNEL2_HIGHT	=	103	;
    CHANNEL3_LOW	=	104	;
    CHANNEL3_HIGHT	=	105	;
    CHANNEL4_LOW	=	106	;
    CHANNEL4_HIGHT	=	107	;
    CHANNEL5_LOW	=	108	;
    CHANNEL5_HIGHT	=	109	;
    CHANNEL6_LOW	=	110	;
    CHANNEL6_HIGHT	=	111	;
    CHANNEL7_LOW	=	112	;
    CHANNEL7_HIGHT	=	113	;
    CHANNEL8_LOW	=	114	;
    CHANNEL8_HIGHT	=	115	;
    CHANNEL9_LOW	=	116	;
    CHANNEL9_HIGHT	=	117	;
    CHANNEL10_LOW	=	118	;
    CHANNEL10_HIGHT	=	119	;
    SECOND	=	130	;
    MINUTE	=	131	;
    HOUR	=	132	;
    DAY	=	133	;
    WEEK	=	134	;
    MONTH	=	135	;
    YEAR	=	136	;
    TEMP_OFFSET_CHANNEL1_LOW	=	140	;
    TEMP_OFFSET_CHANNEL1_HIGHT	=	141	;
    TEMP_OFFSET_CHANNEL2_LOW	=	142	;
    TEMP_OFFSET_CHANNEL2_HIGHT	=	143	;
    TEMP_OFFSET_CHANNEL3_LOW	=	144	;
    TEMP_OFFSET_CHANNEL3_HIGHT	=	145	;
    TEMP_OFFSET_CHANNEL4_LOW	=	146	;
    TEMP_OFFSET_CHANNEL4_HIGHT	=	147	;
    TEMP_OFFSET_CHANNEL5_LOW	=	148	;
    TEMP_OFFSET_CHANNEL5_HIGHT	=	149	;
    TEMP_OFFSET_CHANNEL6_LOW	=	150	;
    TEMP_OFFSET_CHANNEL6_HIGHT	=	151	;
    TEMP_OFFSET_CHANNEL7_LOW	=	152	;
    TEMP_OFFSET_CHANNEL7_HIGHT	=	153	;
    TEMP_OFFSET_CHANNEL8_LOW	=	154	;
    TEMP_OFFSET_CHANNEL8_HIGHT	=	155	;
    TEMP_OFFSET_CHANNEL9_LOW	=	156	;
    TEMP_OFFSET_CHANNEL9_HIGHT	=	157	;
    TEMP_OFFSET_CHANNEL10_LOW	=	158	;
    TEMP_OFFSET_CHANNEL10_HIGHT	=	159	;
    FOUR_POINT_CALIBRATION_1	=	160	;
    FOUR_POINT_CALIBRATION_2	=	161	;
    FOUR_POINT_CALIBRATION_3	=	162	;
    FOUR_POINT_CALIBRATION_4	=	163	;
    FOUR_POINT_CALIBRATION_5	=	164	;
    FOUR_POINT_CALIBRATION_6	=	165	;
    FOUR_POINT_CALIBRATION_7	=	166	;
    FOUR_POINT_CALIBRATION_8	=	167	;
    PARAMETER_CHANNEL1_1	=	168	;
    PARAMETER_CHANNEL1_2	=	169	;
    PARAMETER_CHANNEL1_3	=	170	;
    PARAMETER_CHANNEL1_4	=	171	;
    PARAMETER_CHANNEL1_5	=	172	;
    PARAMETER_CHANNEL1_6	=	173	;
    PARAMETER_CHANNEL2_1	=	174	;
    PARAMETER_CHANNEL2_2	=	175	;
    PARAMETER_CHANNEL2_3	=	176	;
    PARAMETER_CHANNEL2_4	=	177	;
    PARAMETER_CHANNEL2_5	=	178	;
    PARAMETER_CHANNEL2_6	=	179	;
    PARAMETER_CHANNEL3_1	=	180	;
    PARAMETER_CHANNEL3_2	=	181	;
    PARAMETER_CHANNEL3_3	=	182	;
    PARAMETER_CHANNEL3_4	=	183	;
    PARAMETER_CHANNEL3_5	=	184	;
    PARAMETER_CHANNEL3_6	=	185	;
    PARAMETER_CHANNEL4_1	=	186	;
    PARAMETER_CHANNEL4_2	=	187	;
    PARAMETER_CHANNEL4_3	=	188	;
    PARAMETER_CHANNEL4_4	=	189	;
    PARAMETER_CHANNEL4_5	=	190	;
    PARAMETER_CHANNEL4_6	=	191	;
    PARAMETER_CHANNEL5_1	=	192	;
    PARAMETER_CHANNEL5_2	=	193	;
    PARAMETER_CHANNEL5_3	=	194	;
    PARAMETER_CHANNEL5_4	=	195	;
    PARAMETER_CHANNEL5_5	=	196	;
    PARAMETER_CHANNEL5_6	=	197	;
    PARAMETER_CHANNEL6_1	=	198	;
    PARAMETER_CHANNEL6_2	=	199	;
    PARAMETER_CHANNEL6_3	=	200	;
    PARAMETER_CHANNEL6_4	=	201	;
    PARAMETER_CHANNEL6_5	=	202	;
    PARAMETER_CHANNEL6_6	=	203	;
    PARAMETER_CHANNEL7_1	=	204	;
    PARAMETER_CHANNEL7_2	=	205	;
    PARAMETER_CHANNEL7_3	=	206	;
    PARAMETER_CHANNEL7_4	=	207	;
    PARAMETER_CHANNEL7_5	=	208	;
    PARAMETER_CHANNEL7_6	=	209	;
    PARAMETER_CHANNEL8_1	=	210	;
    PARAMETER_CHANNEL8_2	=	211	;
    PARAMETER_CHANNEL8_3	=	212	;
    PARAMETER_CHANNEL8_4	=	213	;
    PARAMETER_CHANNEL8_5	=	214	;
    PARAMETER_CHANNEL8_6	=	215	;
    PARAMETER_CHANNEL9_1	=	216	;
    PARAMETER_CHANNEL9_2	=	217	;
    PARAMETER_CHANNEL9_3	=	218	;
    PARAMETER_CHANNEL9_4	=	219	;
    PARAMETER_CHANNEL9_5	=	220	;
    PARAMETER_CHANNEL9_6	=	221	;
    PARAMETER_CHANNEL10_1	=	222	;
    PARAMETER_CHANNEL10_2	=	223	;
    PARAMETER_CHANNEL10_3	=	224	;
    PARAMETER_CHANNEL10_4	=	225	;
    PARAMETER_CHANNEL10_5	=	226	;
    PARAMETER_CHANNEL10_6	=	227	;
    RANGE_CHANNEL1	=	228	;
    RANGE_CHANNEL2	=	229	;
    RANGE_CHANNEL3	=	230	;
    RANGE_CHANNEL4	=	231	;
    RANGE_CHANNEL5	=	232	;
    RANGE_CHANNEL6	=	233	;
    RANGE_CHANNEL7	=	234	;
    RANGE_CHANNEL8	=	235	;
    RANGE_CHANNEL9	=	236	;
    RANGE_CHANNEL10	=	237	;
    FILTER_CHANNEL1	=	238	;
    FILTER_CHANNEL2	=	239	;
    FILTER_CHANNEL3	=	240	;
    FILTER_CHANNEL4	=	241	;
    FILTER_CHANNEL5	=	242	;
    FILTER_CHANNEL6	=	243	;
    FILTER_CHANNEL7	=	244	;
    FILTER_CHANNEL8	=	245	;
    FILTER_CHANNEL9	=	246	;
    FILTER_CHANNEL10	=	247	;
    SENSOR_STATUS_CHANNEL1	=	248	;
    SENSOR_STATUS_CHANNEL2	=	249	;
    SENSOR_STATUS_CHANNEL3	=	250	;
    SENSOR_STATUS_CHANNEL4	=	251	;
    SENSOR_STATUS_CHANNEL5	=	252	;
    SENSOR_STATUS_CHANNEL6	=	253	;
    SENSOR_STATUS_CHANNEL7	=	254	;
    SENSOR_STATUS_CHANNEL8	=	255	;
    SENSOR_STATUS_CHANNEL9	=	256	;
    SENSOR_STATUS_CHANNEL10	=	257	;



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
        vartemp =m_ado.m_pRecordset->GetCollect(_T("T3-RTD"));
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
    BAUDRATE	=	Get_RegID(_T("	BAUDRATE	"))	;
    FLAG_CALIBRATION	=	Get_RegID(_T("	FLAG_CALIBRATION	"))	;
    MODBUS_FORMAT	=	Get_RegID(_T("	MODBUS_FORMAT	"))	;
    DEADMASTER_TIME	=	Get_RegID(_T("	DEADMASTER_TIME	"))	;
    RESOLUTION	=	Get_RegID(_T("	RESOLUTION	"))	;
    CHANNEL1_LOW	=	Get_RegID(_T("	CHANNEL1_LOW	"))	;
    CHANNEL1_HIGHT	=	Get_RegID(_T("	CHANNEL1_HIGHT	"))	;
    CHANNEL2_LOW	=	Get_RegID(_T("	CHANNEL2_LOW	"))	;
    CHANNEL2_HIGHT	=	Get_RegID(_T("	CHANNEL2_HIGHT	"))	;
    CHANNEL3_LOW	=	Get_RegID(_T("	CHANNEL3_LOW	"))	;
    CHANNEL3_HIGHT	=	Get_RegID(_T("	CHANNEL3_HIGHT	"))	;
    CHANNEL4_LOW	=	Get_RegID(_T("	CHANNEL4_LOW	"))	;
    CHANNEL4_HIGHT	=	Get_RegID(_T("	CHANNEL4_HIGHT	"))	;
    CHANNEL5_LOW	=	Get_RegID(_T("	CHANNEL5_LOW	"))	;
    CHANNEL5_HIGHT	=	Get_RegID(_T("	CHANNEL5_HIGHT	"))	;
    CHANNEL6_LOW	=	Get_RegID(_T("	CHANNEL6_LOW	"))	;
    CHANNEL6_HIGHT	=	Get_RegID(_T("	CHANNEL6_HIGHT	"))	;
    CHANNEL7_LOW	=	Get_RegID(_T("	CHANNEL7_LOW	"))	;
    CHANNEL7_HIGHT	=	Get_RegID(_T("	CHANNEL7_HIGHT	"))	;
    CHANNEL8_LOW	=	Get_RegID(_T("	CHANNEL8_LOW	"))	;
    CHANNEL8_HIGHT	=	Get_RegID(_T("	CHANNEL8_HIGHT	"))	;
    CHANNEL9_LOW	=	Get_RegID(_T("	CHANNEL9_LOW	"))	;
    CHANNEL9_HIGHT	=	Get_RegID(_T("	CHANNEL9_HIGHT	"))	;
    CHANNEL10_LOW	=	Get_RegID(_T("	CHANNEL10_LOW	"))	;
    CHANNEL10_HIGHT	=	Get_RegID(_T("	CHANNEL10_HIGHT	"))	;
    SECOND      	=	Get_RegID(_T("	SECOND	"))	;
    MINUTE	        =	Get_RegID(_T("	MINUTE	"))	;
    HOUR	        =	Get_RegID(_T("	HOUR	"))	;
    DAY 	                    =	Get_RegID(_T("	DAY	"))	;
    WEEK	                    =	Get_RegID(_T("	WEEK	"))	;
    MONTH	                    =	Get_RegID(_T("	MONTH	"))	;
    YEAR	                    =	Get_RegID(_T("	YEAR	"))	;
    TEMP_OFFSET_CHANNEL1_LOW	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL1_LOW	"))	;
    TEMP_OFFSET_CHANNEL1_HIGHT	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL1_HIGHT	"))	;
    TEMP_OFFSET_CHANNEL2_LOW	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL2_LOW	"))	;
    TEMP_OFFSET_CHANNEL2_HIGHT	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL2_HIGHT	"))	;
    TEMP_OFFSET_CHANNEL3_LOW	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL3_LOW	"))	;
    TEMP_OFFSET_CHANNEL3_HIGHT	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL3_HIGHT	"))	;
    TEMP_OFFSET_CHANNEL4_LOW	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL4_LOW	"))	;
    TEMP_OFFSET_CHANNEL4_HIGHT	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL4_HIGHT	"))	;
    TEMP_OFFSET_CHANNEL5_LOW	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL5_LOW	"))	;
    TEMP_OFFSET_CHANNEL5_HIGHT	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL5_HIGHT	"))	;
    TEMP_OFFSET_CHANNEL6_LOW	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL6_LOW	"))	;
    TEMP_OFFSET_CHANNEL6_HIGHT	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL6_HIGHT	"))	;
    TEMP_OFFSET_CHANNEL7_LOW	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL7_LOW	"))	;
    TEMP_OFFSET_CHANNEL7_HIGHT	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL7_HIGHT	"))	;
    TEMP_OFFSET_CHANNEL8_LOW	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL8_LOW	"))	;
    TEMP_OFFSET_CHANNEL8_HIGHT	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL8_HIGHT	"))	;
    TEMP_OFFSET_CHANNEL9_LOW	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL9_LOW	"))	;
    TEMP_OFFSET_CHANNEL9_HIGHT	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL9_HIGHT	"))	;
    TEMP_OFFSET_CHANNEL10_LOW	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL10_LOW	"))	;
    TEMP_OFFSET_CHANNEL10_HIGHT	=	Get_RegID(_T("	TEMP_OFFSET_CHANNEL10_HIGHT	"))	;
    FOUR_POINT_CALIBRATION_1	=	Get_RegID(_T("	FOUR_POINT_CALIBRATION_1	"))	;
    FOUR_POINT_CALIBRATION_2	=	Get_RegID(_T("	FOUR_POINT_CALIBRATION_2	"))	;
    FOUR_POINT_CALIBRATION_3	=	Get_RegID(_T("	FOUR_POINT_CALIBRATION_3	"))	;
    FOUR_POINT_CALIBRATION_4	=	Get_RegID(_T("	FOUR_POINT_CALIBRATION_4	"))	;
    FOUR_POINT_CALIBRATION_5	=	Get_RegID(_T("	FOUR_POINT_CALIBRATION_5	"))	;
    FOUR_POINT_CALIBRATION_6	=	Get_RegID(_T("	FOUR_POINT_CALIBRATION_6	"))	;
    FOUR_POINT_CALIBRATION_7	=	Get_RegID(_T("	FOUR_POINT_CALIBRATION_7	"))	;
    FOUR_POINT_CALIBRATION_8	=	Get_RegID(_T("	FOUR_POINT_CALIBRATION_8	"))	;
    PARAMETER_CHANNEL1_1	=	Get_RegID(_T("	PARAMETER_CHANNEL1_1	"))	;
    PARAMETER_CHANNEL1_2	=	Get_RegID(_T("	PARAMETER_CHANNEL1_2	"))	;
    PARAMETER_CHANNEL1_3	=	Get_RegID(_T("	PARAMETER_CHANNEL1_3	"))	;
    PARAMETER_CHANNEL1_4	=	Get_RegID(_T("	PARAMETER_CHANNEL1_4	"))	;
    PARAMETER_CHANNEL1_5	=	Get_RegID(_T("	PARAMETER_CHANNEL1_5	"))	;
    PARAMETER_CHANNEL1_6	=	Get_RegID(_T("	PARAMETER_CHANNEL1_6	"))	;
    PARAMETER_CHANNEL2_1	=	Get_RegID(_T("	PARAMETER_CHANNEL2_1	"))	;
    PARAMETER_CHANNEL2_2	=	Get_RegID(_T("	PARAMETER_CHANNEL2_2	"))	;
    PARAMETER_CHANNEL2_3	=	Get_RegID(_T("	PARAMETER_CHANNEL2_3	"))	;
    PARAMETER_CHANNEL2_4	=	Get_RegID(_T("	PARAMETER_CHANNEL2_4	"))	;
    PARAMETER_CHANNEL2_5	=	Get_RegID(_T("	PARAMETER_CHANNEL2_5	"))	;
    PARAMETER_CHANNEL2_6	=	Get_RegID(_T("	PARAMETER_CHANNEL2_6	"))	;
    PARAMETER_CHANNEL3_1	=	Get_RegID(_T("	PARAMETER_CHANNEL3_1	"))	;
    PARAMETER_CHANNEL3_2	=	Get_RegID(_T("	PARAMETER_CHANNEL3_2	"))	;
    PARAMETER_CHANNEL3_3	=	Get_RegID(_T("	PARAMETER_CHANNEL3_3	"))	;
    PARAMETER_CHANNEL3_4	=	Get_RegID(_T("	PARAMETER_CHANNEL3_4	"))	;
    PARAMETER_CHANNEL3_5	=	Get_RegID(_T("	PARAMETER_CHANNEL3_5	"))	;
    PARAMETER_CHANNEL3_6	=	Get_RegID(_T("	PARAMETER_CHANNEL3_6	"))	;
    PARAMETER_CHANNEL4_1	=	Get_RegID(_T("	PARAMETER_CHANNEL4_1	"))	;
    PARAMETER_CHANNEL4_2	=	Get_RegID(_T("	PARAMETER_CHANNEL4_2	"))	;
    PARAMETER_CHANNEL4_3	=	Get_RegID(_T("	PARAMETER_CHANNEL4_3	"))	;
    PARAMETER_CHANNEL4_4	=	Get_RegID(_T("	PARAMETER_CHANNEL4_4	"))	;
    PARAMETER_CHANNEL4_5	=	Get_RegID(_T("	PARAMETER_CHANNEL4_5	"))	;
    PARAMETER_CHANNEL4_6	=	Get_RegID(_T("	PARAMETER_CHANNEL4_6	"))	;
    PARAMETER_CHANNEL5_1	=	Get_RegID(_T("	PARAMETER_CHANNEL5_1	"))	;
    PARAMETER_CHANNEL5_2	=	Get_RegID(_T("	PARAMETER_CHANNEL5_2	"))	;
    PARAMETER_CHANNEL5_3	=	Get_RegID(_T("	PARAMETER_CHANNEL5_3	"))	;
    PARAMETER_CHANNEL5_4	=	Get_RegID(_T("	PARAMETER_CHANNEL5_4	"))	;
    PARAMETER_CHANNEL5_5	=	Get_RegID(_T("	PARAMETER_CHANNEL5_5	"))	;
    PARAMETER_CHANNEL5_6	=	Get_RegID(_T("	PARAMETER_CHANNEL5_6	"))	;
    PARAMETER_CHANNEL6_1	=	Get_RegID(_T("	PARAMETER_CHANNEL6_1	"))	;
    PARAMETER_CHANNEL6_2	=	Get_RegID(_T("	PARAMETER_CHANNEL6_2	"))	;
    PARAMETER_CHANNEL6_3	=	Get_RegID(_T("	PARAMETER_CHANNEL6_3	"))	;
    PARAMETER_CHANNEL6_4	=	Get_RegID(_T("	PARAMETER_CHANNEL6_4	"))	;
    PARAMETER_CHANNEL6_5	=	Get_RegID(_T("	PARAMETER_CHANNEL6_5	"))	;
    PARAMETER_CHANNEL6_6	=	Get_RegID(_T("	PARAMETER_CHANNEL6_6	"))	;
    PARAMETER_CHANNEL7_1	=	Get_RegID(_T("	PARAMETER_CHANNEL7_1	"))	;
    PARAMETER_CHANNEL7_2	=	Get_RegID(_T("	PARAMETER_CHANNEL7_2	"))	;
    PARAMETER_CHANNEL7_3	=	Get_RegID(_T("	PARAMETER_CHANNEL7_3	"))	;
    PARAMETER_CHANNEL7_4	=	Get_RegID(_T("	PARAMETER_CHANNEL7_4	"))	;
    PARAMETER_CHANNEL7_5	=	Get_RegID(_T("	PARAMETER_CHANNEL7_5	"))	;
    PARAMETER_CHANNEL7_6	=	Get_RegID(_T("	PARAMETER_CHANNEL7_6	"))	;
    PARAMETER_CHANNEL8_1	=	Get_RegID(_T("	PARAMETER_CHANNEL8_1	"))	;
    PARAMETER_CHANNEL8_2	=	Get_RegID(_T("	PARAMETER_CHANNEL8_2	"))	;
    PARAMETER_CHANNEL8_3	=	Get_RegID(_T("	PARAMETER_CHANNEL8_3	"))	;
    PARAMETER_CHANNEL8_4	=	Get_RegID(_T("	PARAMETER_CHANNEL8_4	"))	;
    PARAMETER_CHANNEL8_5	=	Get_RegID(_T("	PARAMETER_CHANNEL8_5	"))	;
    PARAMETER_CHANNEL8_6	=	Get_RegID(_T("	PARAMETER_CHANNEL8_6	"))	;
    PARAMETER_CHANNEL9_1	=	Get_RegID(_T("	PARAMETER_CHANNEL9_1	"))	;
    PARAMETER_CHANNEL9_2	=	Get_RegID(_T("	PARAMETER_CHANNEL9_2	"))	;
    PARAMETER_CHANNEL9_3	=	Get_RegID(_T("	PARAMETER_CHANNEL9_3	"))	;
    PARAMETER_CHANNEL9_4	=	Get_RegID(_T("	PARAMETER_CHANNEL9_4	"))	;
    PARAMETER_CHANNEL9_5	=	Get_RegID(_T("	PARAMETER_CHANNEL9_5	"))	;
    PARAMETER_CHANNEL9_6	=	Get_RegID(_T("	PARAMETER_CHANNEL9_6	"))	;
    PARAMETER_CHANNEL10_1	=	Get_RegID(_T("	PARAMETER_CHANNEL10_1	"))	;
    PARAMETER_CHANNEL10_2	=	Get_RegID(_T("	PARAMETER_CHANNEL10_2	"))	;
    PARAMETER_CHANNEL10_3	=	Get_RegID(_T("	PARAMETER_CHANNEL10_3	"))	;
    PARAMETER_CHANNEL10_4	=	Get_RegID(_T("	PARAMETER_CHANNEL10_4	"))	;
    PARAMETER_CHANNEL10_5	=	Get_RegID(_T("	PARAMETER_CHANNEL10_5	"))	;
    PARAMETER_CHANNEL10_6	=	Get_RegID(_T("	PARAMETER_CHANNEL10_6	"))	;
    RANGE_CHANNEL1	=	Get_RegID(_T("	RANGE_CHANNEL1	"))	;
    RANGE_CHANNEL2	=	Get_RegID(_T("	RANGE_CHANNEL2	"))	;
    RANGE_CHANNEL3	=	Get_RegID(_T("	RANGE_CHANNEL3	"))	;
    RANGE_CHANNEL4	=	Get_RegID(_T("	RANGE_CHANNEL4	"))	;
    RANGE_CHANNEL5	=	Get_RegID(_T("	RANGE_CHANNEL5	"))	;
    RANGE_CHANNEL6	=	Get_RegID(_T("	RANGE_CHANNEL6	"))	;
    RANGE_CHANNEL7	=	Get_RegID(_T("	RANGE_CHANNEL7	"))	;
    RANGE_CHANNEL8	=	Get_RegID(_T("	RANGE_CHANNEL8	"))	;
    RANGE_CHANNEL9	=	Get_RegID(_T("	RANGE_CHANNEL9	"))	;
    RANGE_CHANNEL10	=	Get_RegID(_T("	RANGE_CHANNEL10	"))	;
    FILTER_CHANNEL1	=	Get_RegID(_T("	FILTER_CHANNEL1	"))	;
    FILTER_CHANNEL2	=	Get_RegID(_T("	FILTER_CHANNEL2	"))	;
    FILTER_CHANNEL3	=	Get_RegID(_T("	FILTER_CHANNEL3	"))	;
    FILTER_CHANNEL4	=	Get_RegID(_T("	FILTER_CHANNEL4	"))	;
    FILTER_CHANNEL5	=	Get_RegID(_T("	FILTER_CHANNEL5	"))	;
    FILTER_CHANNEL6	=	Get_RegID(_T("	FILTER_CHANNEL6	"))	;
    FILTER_CHANNEL7	=	Get_RegID(_T("	FILTER_CHANNEL7	"))	;
    FILTER_CHANNEL8	=	Get_RegID(_T("	FILTER_CHANNEL8	"))	;
    FILTER_CHANNEL9	=	Get_RegID(_T("	FILTER_CHANNEL9	"))	;
    FILTER_CHANNEL10	=	Get_RegID(_T("	FILTER_CHANNEL10	"))	;
    SENSOR_STATUS_CHANNEL1	=	Get_RegID(_T("	SENSOR_STATUS_CHANNEL1	"))	;
    SENSOR_STATUS_CHANNEL2	=	Get_RegID(_T("	SENSOR_STATUS_CHANNEL2	"))	;
    SENSOR_STATUS_CHANNEL3	=	Get_RegID(_T("	SENSOR_STATUS_CHANNEL3	"))	;
    SENSOR_STATUS_CHANNEL4	=	Get_RegID(_T("	SENSOR_STATUS_CHANNEL4	"))	;
    SENSOR_STATUS_CHANNEL5	=	Get_RegID(_T("	SENSOR_STATUS_CHANNEL5	"))	;
    SENSOR_STATUS_CHANNEL6	=	Get_RegID(_T("	SENSOR_STATUS_CHANNEL6	"))	;
    SENSOR_STATUS_CHANNEL7	=	Get_RegID(_T("	SENSOR_STATUS_CHANNEL7	"))	;
    SENSOR_STATUS_CHANNEL8	=	Get_RegID(_T("	SENSOR_STATUS_CHANNEL8	"))	;
    SENSOR_STATUS_CHANNEL9	=	Get_RegID(_T("	SENSOR_STATUS_CHANNEL9	"))	;
    SENSOR_STATUS_CHANNEL10	=	Get_RegID(_T("	SENSOR_STATUS_CHANNEL10	"))	;

#endif
}
// CT3RTDView message handlers
void CT3RTDView::InitialDialog(){
   InitialTableName();

    CString strTemp;
    strTemp.Format(_T("%d"),product_register_value[MODBUS_ID]);
    GetDlgItem(IDC_EDIT_T3ADDRESS)->SetWindowText(strTemp);
    int temp;
    temp=product_register_value[SN_LOW]+product_register_value[SN_LOW+1]*256+product_register_value[SN_HI]*256*256+product_register_value[SN_HI+1]*256*256*256;
    m_sn=temp;
    strTemp.Format(_T("%d"),temp);
    GetDlgItem(IDC_EDIT_T3SERIAL)->SetWindowText(strTemp);

    GetDlgItem(IDC_EDIT_T3MODEL)->SetWindowText(_T("T3-PT10"));
    strTemp.Format(_T("%d"),product_register_value[FIRMWARE_VER_NUMBER]);
    GetDlgItem(IDC_EDIT_T3FIRMWARE)->SetWindowText(strTemp);
    strTemp.Format(_T("%d"),product_register_value[HARDWARE_VER_NUMBER]);
    GetDlgItem(IDC_EDIT_T3HARDWARE)->SetWindowText(strTemp);

    if (0==product_register_value[MODBUS_FORMAT])
    {
    m_dataformat.SetWindowText(_T("Float"));
    } 
    else
    {
    m_dataformat.SetWindowText(_T("Int"));
    }

    strTemp.Format(_T("%d"),product_register_value[RESOLUTION]);
    m_accuracy.SetWindowTextW(strTemp);

    CString strresult;
    int regValue;
    union{
    float floatData;
    char  charData[4];
    }DataFormat;
    if (product_register_value[MODBUS_FORMAT]==0)//Float
    {
       int accuracy=product_register_value[RESOLUTION];
        for(int i = 1;i<=10;i++)
        {   if (0==product_register_value[RANGE_CHANNEL1+i-1])
            {
                strresult=_T("C");
            } 
            else if (1==product_register_value[RANGE_CHANNEL1+i-1])
            {
                strresult=_T("F");
            }
            else if (2==product_register_value[RANGE_CHANNEL1+i-1])
            {
                strresult=_T("Ω");
            }
            m_msflexgrid_input.put_TextMatrix(i,3,strresult);
            CString unit=strresult;
            regValue=0;
            regValue=product_register_value[CHANNEL1_HIGHT+2*(i-1)];
            int Data=regValue;
            int Data1=regValue&0x00FF;
            int Data2=(regValue&0xFF00)>>8;
            DataFormat.charData[3]=Data2;
            DataFormat.charData[2]=Data1;

            regValue=0;
            regValue=product_register_value[CHANNEL1_LOW+2*(i-1)];
            Data=regValue;
            Data1=regValue&0x00FF;
            Data2=(regValue&0xFF00)>>8;
            DataFormat.charData[1]=Data2;
            DataFormat.charData[0]=Data1;
            if (1==accuracy)
            {
            strresult.Format(_T("%0.1f"),DataFormat.floatData);
            } 
            else if (2==accuracy)
            {
            strresult.Format(_T("%0.2f"),DataFormat.floatData);
            }
            else if (3==accuracy)
            {
            strresult.Format(_T("%0.3f"),DataFormat.floatData);
            }
            else if (4==accuracy)
            {
            strresult.Format(_T("%0.4f"),DataFormat.floatData);
            }
            else if (5==accuracy)
            {
            strresult.Format(_T("%0.5f"),DataFormat.floatData);
            }
            else if (6==accuracy)
            {
            strresult.Format(_T("%0.6f"),DataFormat.floatData);
            }
            

            strresult+=unit;
            m_msflexgrid_input.put_TextMatrix(i,2,strresult);
            strresult.Format(_T("%d"),product_register_value[FILTER_CHANNEL1+i-1]);
            m_msflexgrid_input.put_TextMatrix(i,4,strresult);
            if (0==product_register_value[SENSOR_STATUS_CHANNEL1+i-1])
            {
                strresult=_T("Pass");
            } 
            else if (1==product_register_value[SENSOR_STATUS_CHANNEL1+i-1])
            {
                strresult=_T("Short");
            }
            else if (2==product_register_value[SENSOR_STATUS_CHANNEL1+i-1])
            {
                strresult=_T("Unconnect");
            }
            m_msflexgrid_input.put_TextMatrix(i,5,strresult);
        }
    }
    else//Int
    {
        for(int i = 1;i<=10;i++)
        {  
            if (0==product_register_value[RANGE_CHANNEL1+i-1])
            {
                strresult=_T("C");
            } 
            else if (1==product_register_value[RANGE_CHANNEL1+i-1])
            {
                strresult=_T("F");
            }
            else if (2==product_register_value[RANGE_CHANNEL1+i-1])
            {
                strresult=_T("Ω");
            }
            m_msflexgrid_input.put_TextMatrix(i,3,strresult);
            CString unit=strresult;
            regValue=product_register_value[CHANNEL1_HIGHT+2*(i-1)];
            strresult.Format(_T("%0.1f"),((float)regValue)/10);
            strresult+=unit;
            m_msflexgrid_input.put_TextMatrix(i,2,strresult);
            strresult.Format(_T("%d"),product_register_value[FILTER_CHANNEL1+i-1]);
            m_msflexgrid_input.put_TextMatrix(i,4,strresult);
            if (0==product_register_value[SENSOR_STATUS_CHANNEL1+i-1])
            {
                strresult=_T("Pass");
            } 
            else if (1==product_register_value[SENSOR_STATUS_CHANNEL1+i-1])
            {
                strresult=_T("Short");
            }
            else if (2==product_register_value[SENSOR_STATUS_CHANNEL1+i-1])
            {
                strresult=_T("Unconnect");
            }
            m_msflexgrid_input.put_TextMatrix(i,5,strresult);
        }
    }




  


}

void CT3RTDView::InitialTableName(){
    m_sn=product_register_value[SN_LOW]+product_register_value[SN_LOW+1]*256+product_register_value[SN_HI]*256*256+product_register_value[SN_HI+1]*256*256*256;

    CString str_input;
    for(int i=1;i<11;i++)
    {
        str_input=Get_Table_Name(m_sn,_T("PT_Channel"),i);
        m_msflexgrid_input.put_TextMatrix(i,1,str_input);	 
    }

}

int  CT3RTDView::Get_RegID(CString Name)
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

void CT3RTDView::Fresh()
{


    CMainFrame*pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
    pMain->m_pFreshMultiRegisters->SuspendThread();
    //	pMain->m_pFreshTree->SuspendThread();
    pMain->m_pRefreshThread->SuspendThread();
    if (!is_connect())
    {
        pMain->OnConnect();
    }
    float progress;
    if (is_connect())
    {  
        CDialog_Progess* pDlg = new CDialog_Progess(this,1,100);
        pDlg->Create(IDD_DIALOG10_Progress, this);
        pDlg->ShowProgress(0,0);
        pDlg->ShowWindow(SW_SHOW);
        RECT RECT_SET1;
        GetClientRect(&RECT_SET1);
        pDlg->MoveWindow(RECT_SET1.left+400,RECT_SET1.bottom-19,RECT_SET1.right/2+20,20);

        for (int i=0;i<3;i++)
        {
            if (pDlg!=NULL)
            {
                progress=float((i+1)*(100/3));
                pDlg->ShowProgress(int(progress),(int)progress);
            } 
            Read_Multi(g_tstat_id,&product_register_value[i*100],i*100,100);
        }
        pDlg->ShowWindow(SW_HIDE);
        if(pDlg!=NULL)
        {delete pDlg;
        pDlg=NULL;}

        Initial_RegisterList();
        InitialDialog();

        if(hFirstThread != NULL)
            TerminateThread(hFirstThread, 0);
        hFirstThread=NULL;
        if (!hFirstThread)
        {
            hFirstThread = CreateThread(NULL,NULL,_BackFreshing_RTD,this,NULL,0);
        }

    }
    else
    {
        SetPaneString(1,_T("Disconnection"));
        AfxMessageBox(_T("Please Open COM Port"));
    }
}

void CT3RTDView::OnBnClickedChangeid()
{
    CEreaseDlg dlg;
    dlg.DoModal();
}


void CT3RTDView::OnCbnSelchangeCombo1()
{
    int sel=m_dataformat.GetCurSel();
    int ret=write_one(g_tstat_id,MODBUS_FORMAT,sel);
    if (ret>0)
    {
    product_register_value[MODBUS_FORMAT]=sel;
    }
    if (0==product_register_value[MODBUS_FORMAT])
    {
        m_dataformat.SetWindowText(_T("Float"));
    } 
    else
    {
        m_dataformat.SetWindowText(_T("Int"));
    }
}


void CT3RTDView::OnEnKillfocusEdit1()
{ CString strvalue;   
m_accuracy.GetWindowText(strvalue);
int value=_wtoi(strvalue);
    if (value!=product_register_value[RESOLUTION])
    {
     int ret=write_one(g_tstat_id,RESOLUTION,value);
     if (ret>0)
     {
      product_register_value[RESOLUTION]=value;
     }
     strvalue.Format(_T("%d"),product_register_value[RESOLUTION]);
     m_accuracy.SetWindowTextW(strvalue);
    }
}
BEGIN_EVENTSINK_MAP(CT3RTDView, CFormView)
    ON_EVENT(CT3RTDView, IDC_MSFLEXGRID_INPUT, DISPID_CLICK, CT3RTDView::ClickMsflexgridInput, VTS_NONE)
END_EVENTSINK_MAP()


void CT3RTDView::ClickMsflexgridInput()
{
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



    if (lRow==0)
    {
        return;
    }
    if(3==lCol)
    {
        m_comboxRange.MoveWindow(&rcCell,1);
        m_comboxRange.ResetContent();
        m_comboxRange.AddString(_T("C"));
        m_comboxRange.AddString(_T("F"));
        m_comboxRange.AddString(_T("Ω"));
        m_comboxRange.ShowWindow(SW_SHOW);
        m_comboxRange.BringWindowToTop();
        m_comboxRange.SetFocus(); //获取焦点
        m_comboxRange.SetWindowText(strValue);
    }
    else  
    {

        m_inNameEdt.MoveWindow(&rcCell,1);
        m_inNameEdt.ShowWindow(SW_SHOW);
        m_inNameEdt.SetWindowText(strValue);
        m_inNameEdt.SetFocus();
        m_inNameEdt.SetCapture();//LSC
        int nLenth=strValue.GetLength();
        m_inNameEdt.SetSel(nLenth,nLenth); //全选//
    }
}


void CT3RTDView::OnCbnSelchangeRange()
{
    BeginWaitCursor();
    int sel=m_comboxRange.GetCurSel();
    m_comboxRange.ShowWindow(FALSE);
    if (product_register_value[RANGE_CHANNEL1+m_currow-1]==sel)
    {
        return;
    }
    int ret=write_one(g_tstat_id,RANGE_CHANNEL1+m_currow-1,sel);
    if (ret>0)
    {
        product_register_value[RANGE_CHANNEL1+m_currow-1]=sel;
    }
    Sleep(1000);
    Read_Multi(g_tstat_id,&product_register_value[CHANNEL1_LOW],CHANNEL1_LOW,20);
    InitialDialog();
    EndWaitCursor();
}


void CT3RTDView::OnEnKillfocusEdit2()
{
    CString strTemp;
    GetDlgItem(IDC_EDIT_NAME)->GetWindowText(strTemp);
    int Value=_wtoi(strTemp);

    if (strTemp.Compare(m_oldname)==0)
    {
        return;
    }
    if (m_curcol==0)
    {

        Insert_Update_Table_Name(m_sn,_T("PT_Channel"),m_currow,strTemp);
        m_msflexgrid_input.put_TextMatrix(m_currow,m_curcol,strTemp);
    }
     
}
