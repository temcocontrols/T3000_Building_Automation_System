// T36CT.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T36CT.h"
#include "T3000.h"
 
#include "Dialog_Progess.h"
#include "ado/ADO.h"
#include "globle_function.h"
#include "MainFrm.h"

// T36CT

IMPLEMENT_DYNCREATE(T36CT, CFormView)

T36CT::T36CT()
	: CFormView(T36CT::IDD)
{
grid_flag=0;
}

T36CT::~T36CT()
{
}

void T36CT::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MSFLEXGRID_INPUT, m_msflexgrid_input);
    DDX_Control(pDX, IDC_MSFLEXGRID_OUTPUT, m_msflexgrid_output);
    DDX_Control(pDX, IDC_MSFLEXGRID_INPUT2, m_msflexgrid_Ainput);
    DDX_Control(pDX, IDC_EDIT_NAME, m_inNameEdt);
    DDX_Control(pDX, IDC_RANGECOMBO, m_comboxRange);
}

BEGIN_MESSAGE_MAP(T36CT, CFormView)
    ON_BN_CLICKED(IDC_CHANGEID, &T36CT::OnBnClickedChangeid)
    ON_EN_KILLFOCUS(IDC_EDIT_NAME, &T36CT::OnEnKillfocusEditName)
    ON_CBN_SELCHANGE(IDC_RANGECOMBO, &T36CT::OnCbnSelchangeRangecombo)
END_MESSAGE_MAP()


// T36CT diagnostics

#ifdef _DEBUG
void T36CT::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void T36CT::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void T36CT::Initial_RegisterList()
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
    DEADMASTER_TIME	=	17	;
    REAL_STATUS	=	18	;
    RESPONSE_DELAY	=	19	;
    OUTPUT1	=	100	;
    OUTPUT2	=	101	;
    OUTPUT3	=	102	;
    OUTPUT4	=	103	;
    OUTPUT5	=	104	;
    INPUT1_CT	=	108	;
    INPUT2_CT	=	109	;
    INPUT3_CT	=	110	;
    INPUT4_CT	=	111	;
    INPUT5_CT	=	112	;
    INPUT6_CT	=	113	;
    INPUT1	=	114	;
    INPUT2	=	115	;
    INPUT3	=	116	;
    INPUT4	=	117	;
    INPUT5	=	118	;
    INPUT6	=	119	;
    INPUT7	=	120	;
    INPUT8	=	121	;
    INPUT9	=	122	;
    INPUT10	=	123	;
    SWITCH1_BANK	=	124	;
    SWITCH2_BANK	=	125	;
    RANGE_INPUT1	=	126	;
    RANGE_INPUT2	=	127	;
    RANGE_INPUT3	=	128	;
    RANGE_INPUT4	=	129	;
    RANGE_INPUT5	=	130	;
    RANGE_INPUT6	=	131	;
    RANGE_INPUT7	=	132	;
    RANGE_INPUT8	=	133	;
    RANGE_INPUT9	=	134	;
    RANGE_INPUT10	=	135	;
    RANGE_INPUT11	=	136	;
    RANGE_INPUT12	=	137	;
    RANGE_INPUT13	=	138	;
    RANGE_INPUT14	=	139	;
    RANGE_INPUT15	=	140	;
    RANGE_INPUT16	=	141	;
    FILTER_INPUT1	=	142	;
    FILTER_INPUT2	=	143	;
    FILTER_INPUT3	=	144	;
    FILTER_INPUT4	=	145	;
    FILTER_INPUT5	=	146	;
    FILTER_INPUT6	=	147	;
    FILTER_INPUT7	=	148	;
    FILTER_INPUT8	=	149	;
    FILTER_INPUT9	=	150	;
    FILTER_INPUT10	=	151	;
    FILTER_INPUT11	=	152	;
    FILTER_INPUT12	=	153	;
    FILTER_INPUT13	=	154	;
    FILTER_INPUT14	=	155	;
    FILTER_INPUT15	=	156	;
    FILTER_INPUT16	=	157	;


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
        vartemp =m_ado.m_pRecordset->GetCollect(_T("T3-6CT"));
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
        DEADMASTER_TIME	=	Get_RegID(_T("	DEADMASTER_TIME	"))	;
        REAL_STATUS	=	Get_RegID(_T("	REAL_STATUS	"))	;
        RESPONSE_DELAY	=	Get_RegID(_T("	RESPONSE_DELAY	"))	;
        OUTPUT1	=	Get_RegID(_T("	OUTPUT1	"))	;
        OUTPUT2	=	Get_RegID(_T("	OUTPUT2	"))	;
        OUTPUT3	=	Get_RegID(_T("	OUTPUT3	"))	;
        OUTPUT4	=	Get_RegID(_T("	OUTPUT4	"))	;
        OUTPUT5	=	Get_RegID(_T("	OUTPUT5	"))	;
        INPUT1_CT	=	Get_RegID(_T("	INPUT1_CT	"))	;
        INPUT2_CT	=	Get_RegID(_T("	INPUT2_CT	"))	;
        INPUT3_CT	=	Get_RegID(_T("	INPUT3_CT	"))	;
        INPUT4_CT	=	Get_RegID(_T("	INPUT4_CT	"))	;
        INPUT5_CT	=	Get_RegID(_T("	INPUT5_CT	"))	;
        INPUT6_CT	=	Get_RegID(_T("	INPUT6_CT	"))	;
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
        SWITCH1_BANK	=	Get_RegID(_T("	SWITCH1_BANK	"))	;
        SWITCH2_BANK	=	Get_RegID(_T("	SWITCH2_BANK	"))	;
        RANGE_INPUT1	=	Get_RegID(_T("	RANGE_INPUT1	"))	;
        RANGE_INPUT2	=	Get_RegID(_T("	RANGE_INPUT2	"))	;
        RANGE_INPUT3	=	Get_RegID(_T("	RANGE_INPUT3	"))	;
        RANGE_INPUT4	=	Get_RegID(_T("	RANGE_INPUT4	"))	;
        RANGE_INPUT5	=	Get_RegID(_T("	RANGE_INPUT5	"))	;
        RANGE_INPUT6	=	Get_RegID(_T("	RANGE_INPUT6	"))	;
        RANGE_INPUT7	=	Get_RegID(_T("	RANGE_INPUT7	"))	;
        RANGE_INPUT8	=	Get_RegID(_T("	RANGE_INPUT8	"))	;
        RANGE_INPUT9	=	Get_RegID(_T("	RANGE_INPUT9	"))	;
        RANGE_INPUT10	=	Get_RegID(_T("	RANGE_INPUT10	"))	;
        RANGE_INPUT11	=	Get_RegID(_T("	RANGE_INPUT11	"))	;
        RANGE_INPUT12	=	Get_RegID(_T("	RANGE_INPUT12	"))	;
        RANGE_INPUT13	=	Get_RegID(_T("	RANGE_INPUT13	"))	;
        RANGE_INPUT14	=	Get_RegID(_T("	RANGE_INPUT14	"))	;
        RANGE_INPUT15	=	Get_RegID(_T("	RANGE_INPUT15	"))	;
        RANGE_INPUT16	=	Get_RegID(_T("	RANGE_INPUT16	"))	;
        FILTER_INPUT1	=	Get_RegID(_T("	FILTER_INPUT1	"))	;
        FILTER_INPUT2	=	Get_RegID(_T("	FILTER_INPUT2	"))	;
        FILTER_INPUT3	=	Get_RegID(_T("	FILTER_INPUT3	"))	;
        FILTER_INPUT4	=	Get_RegID(_T("	FILTER_INPUT4	"))	;
        FILTER_INPUT5	=	Get_RegID(_T("	FILTER_INPUT5	"))	;
        FILTER_INPUT6	=	Get_RegID(_T("	FILTER_INPUT6	"))	;
        FILTER_INPUT7	=	Get_RegID(_T("	FILTER_INPUT7	"))	;
        FILTER_INPUT8	=	Get_RegID(_T("	FILTER_INPUT8	"))	;
        FILTER_INPUT9	=	Get_RegID(_T("	FILTER_INPUT9	"))	;
        FILTER_INPUT10	=	Get_RegID(_T("	FILTER_INPUT10	"))	;
        FILTER_INPUT11	=	Get_RegID(_T("	FILTER_INPUT11	"))	;
        FILTER_INPUT12	=	Get_RegID(_T("	FILTER_INPUT12	"))	;
        FILTER_INPUT13	=	Get_RegID(_T("	FILTER_INPUT13	"))	;
        FILTER_INPUT14	=	Get_RegID(_T("	FILTER_INPUT14	"))	;
        FILTER_INPUT15	=	Get_RegID(_T("	FILTER_INPUT15	"))	;
        FILTER_INPUT16	=	Get_RegID(_T("	FILTER_INPUT16	"))	;
#endif
}
// T36CT message handlers
int  T36CT::Get_RegID(CString Name)
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

void T36CT::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
#if 1	
    //设置排/行数量
    m_msflexgrid_input.put_Cols(4);
    m_msflexgrid_input.put_Rows(6+1); 
    m_msflexgrid_input.put_TextMatrix(0,0,_T("Input Name"));
    m_msflexgrid_input.put_TextMatrix(0,1,_T("Register Value"));
    m_msflexgrid_input.put_TextMatrix(0,2,_T("Range"));
    m_msflexgrid_input.put_TextMatrix(0,3,_T("Filter"));
    //设置列宽	
    m_msflexgrid_input.put_ColWidth(0,1000);
    m_msflexgrid_input.put_ColWidth(1,1500);
    //m_msflexgrid_input.put_ColWidth(2,1500);
    m_msflexgrid_input.put_ColWidth(2,1500);
    m_msflexgrid_input.put_ColWidth(3,1000);
    //居中显示
    for (int col=0;col<4;col++)
    { 
        m_msflexgrid_input.put_ColAlignment(col,4);
    }
    //彩色显示
    for(int i=1;i<4+1;i++)		//排数量
    {
        for(int k=0;k<4;k++)	//列数量
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
    for(int i=1;i<6+1;i++)
    {

        str.Format(_T("Input%d"),i);

        m_msflexgrid_input.put_TextMatrix(i,0,str);	 
    }

    //============================================================================================================界面Output DO部份列表框
    m_msflexgrid_Ainput.put_Cols(4);
    m_msflexgrid_Ainput.put_Rows(10+1); 
    m_msflexgrid_Ainput.put_TextMatrix(0,0,_T("Input Name"));
    m_msflexgrid_Ainput.put_TextMatrix(0,1,_T("Register Value"));
    m_msflexgrid_Ainput.put_TextMatrix(0,2,_T("Range"));
    m_msflexgrid_Ainput.put_TextMatrix(0,3,_T("Filter"));
    //设置列宽	
    m_msflexgrid_Ainput.put_ColWidth(0,1000);
    m_msflexgrid_Ainput.put_ColWidth(1,1500);
    //m_msflexgrid_input.put_ColWidth(2,1500);
    m_msflexgrid_Ainput.put_ColWidth(2,1500);
    m_msflexgrid_Ainput.put_ColWidth(3,1000);
    //居中显示
    for (int col=0;col<4;col++)
    { 
        m_msflexgrid_Ainput.put_ColAlignment(col,4);
    }
    //彩色显示
    for(int i=1;i<4+1;i++)		//排数量
    {
        for(int k=0;k<4;k++)	//列数量
        {
            if (i%2==1)
            {
                m_msflexgrid_Ainput.put_Row(i);m_msflexgrid_Ainput.put_Col(k);m_msflexgrid_Ainput.put_CellBackColor(RGB(255,255,255));
            }
            else
            {
                m_msflexgrid_Ainput.put_Row(i);m_msflexgrid_Ainput.put_Col(k);m_msflexgrid_Ainput.put_CellBackColor(COLOR_CELL);
            }
        }
    }
    //显示纵标题
//    CString str;
    for(int i=1;i<10+1;i++)
    {

        str.Format(_T("Input%d"),i);

        m_msflexgrid_Ainput.put_TextMatrix(i,0,str);	 
    }


    //设置行/列数量
    m_msflexgrid_output.put_Rows(6);
    m_msflexgrid_output.put_Cols(2);
    //设置列宽	


    //显示横标题
    m_msflexgrid_output.put_TextMatrix(0,0,_T("Output Name"));
    m_msflexgrid_output.put_TextMatrix(0,1,_T("Output Value"));
  

   /* m_msflexgrid_output.put_ColWidth(0,1500);
    m_msflexgrid_output.put_ColWidth(1,1200);*/
    //m_msflexgrid_output.put_ColWidth(2,1500);
    //m_msflexgrid_output.put_ColWidth(3,1500);//居中显示
    for (int col=0;col<2;col++)
    { 
        m_msflexgrid_output.put_ColAlignment(col,4);
    }

    //彩色显示
    for(int i=1;i<6;i++)		//排数量
    {

        for(int k=0;k<2;k++)	//列数量
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
    for(int i=1;i<5+1;i++)
    {

        str_output.Format(_T("Output%d"),i);
        m_msflexgrid_output.put_TextMatrix(i,0,str_output);	


    }




#endif  
}

void T36CT::InitialDialog(){
    InitialTableName();
    CString strTemp;
    strTemp.Format(_T("%d"),product_register_value[MODBUS_ID]);
    GetDlgItem(IDC_EDIT_T3ADDRESS)->SetWindowText(strTemp);
    int temp;
    temp=product_register_value[SN_LOW]+product_register_value[SN_LOW+1]*256+product_register_value[SN_HI]*256*256+product_register_value[SN_HI+1]*256*256*256;
    m_sn=temp;
    strTemp.Format(_T("%d"),temp);
    GetDlgItem(IDC_EDIT_T3SERIAL)->SetWindowText(strTemp);

    GetDlgItem(IDC_EDIT_T3MODEL)->SetWindowText(_T("T3-6CT"));
    strTemp.Format(_T("%d"),product_register_value[FIRMWARE_VER_NUMBER]);
    GetDlgItem(IDC_EDIT_T3FIRMWARE)->SetWindowText(strTemp);
    strTemp.Format(_T("%d"),product_register_value[HARDWARE_VER_NUMBER]);
    GetDlgItem(IDC_EDIT_T3HARDWARE)->SetWindowText(strTemp);
    strTemp.Format(_T("%d"),product_register_value[PIC_VER_NUMBER]);
    GetDlgItem(IDC_EDIT_T3PICVERSION)->SetWindowText(strTemp);

CString strresult;
int regValue;
for(int i = 1;i<=6;i++)
{  
    regValue=product_register_value[INPUT1_CT+i-1];
    strresult.Format(_T("%d"),regValue);
    m_msflexgrid_input.put_TextMatrix(i,1,strresult);


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

    m_msflexgrid_input.put_TextMatrix(i,2,strresult);
    strresult.Format(_T("%d"),product_register_value[FILTER_INPUT1+i-1]);
    m_msflexgrid_input.put_TextMatrix(i,3,strresult);
}


for(int i = 1;i<=10;i++)
{  
    regValue=product_register_value[INPUT1+i-1];
    strresult.Format(_T("%d"),regValue);
    m_msflexgrid_Ainput.put_TextMatrix(i,1,strresult);


    strresult.Format(_T("%d"),product_register_value[RANGE_INPUT7+i-1]);
    if (0==product_register_value[RANGE_INPUT7+i-1])
    {
        strresult=_T("RAW DATA");
    } 
    else if (1==product_register_value[RANGE_INPUT7+i-1])
    {
        strresult=_T("10K C");
    }
    else if (2==product_register_value[RANGE_INPUT7+i-1])
    {
        strresult=_T("10K F");
    }
    else if (3==product_register_value[RANGE_INPUT7+i-1])
    {
        strresult=_T("0-100%");
    }
    else if (4==product_register_value[RANGE_INPUT7+i-1])
    {
        strresult=_T("ON/OFF");
    }
    else if (5==product_register_value[RANGE_INPUT7+i-1])
    {
        strresult=_T("OFF/ON");
    }
    else if (6==product_register_value[RANGE_INPUT7+i-1])
    {
        strresult=_T("Pulse Input");
    }
    else if (7==product_register_value[RANGE_INPUT7+i-1])
    {
        strresult=_T("Lighting Control");
    }
    else if (8==product_register_value[RANGE_INPUT7+i-1])
    {
        strresult=_T("TYPE3 10K C");
    }
    else if (9==product_register_value[RANGE_INPUT7+i-1])
    {
        strresult=_T("TYPE3 10K F");
    }
    else if (10==product_register_value[RANGE_INPUT7+i-1])
    {
        strresult=_T("NO USE");
    }
    else if (11==product_register_value[RANGE_INPUT7+i-1])
    {
        strresult=_T("0-5V");
    }
    else if (12==product_register_value[RANGE_INPUT7+i-1])
    {
        strresult=_T("0-10V");
    }
    else if (13==product_register_value[RANGE_INPUT7+i-1])
    {
        strresult=_T("0-20I");
    }

    m_msflexgrid_Ainput.put_TextMatrix(i,2,strresult);
    strresult.Format(_T("%d"),product_register_value[FILTER_INPUT7+i-1]);
    m_msflexgrid_Ainput.put_TextMatrix(i,3,strresult);
}
CString CstresultDO;
for(int i = 1;i<=5;i++)
{  
    if (product_register_value[OUTPUT1+i-1]==0)
    {
    CstresultDO=_T("OFF");
    }
    else
    {
    CstresultDO=_T("ON");
    }
    m_msflexgrid_output.put_TextMatrix(i,1,CstresultDO);
}


}

void T36CT::Fresh()
{
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
    }
    else
    {
        SetPaneString(1,_T("Disconnection"));
        AfxMessageBox(_T("Please Open COM Port"));
    }
}

void T36CT::OnBnClickedChangeid()
{
    CMainFrame*pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
    pMain->OnToolErease();
}
BEGIN_EVENTSINK_MAP(T36CT, CFormView)
    ON_EVENT(T36CT, IDC_MSFLEXGRID_OUTPUT, DISPID_CLICK, T36CT::ClickMsflexgridOutput, VTS_NONE)
    ON_EVENT(T36CT, IDC_MSFLEXGRID_INPUT, DISPID_CLICK, T36CT::ClickMsflexgridInput, VTS_NONE)
    ON_EVENT(T36CT, IDC_MSFLEXGRID_INPUT2, DISPID_CLICK, T36CT::ClickMsflexgridInput2, VTS_NONE)
END_EVENTSINK_MAP()


void T36CT::ClickMsflexgridOutput()
{
    grid_flag=3;
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

    if(lRow!=0)
    {
        //return; // 2012.2.7老毛说不允许修改
        if (m_curcol==0)
        {
            m_inNameEdt.MoveWindow(&rcCell,1);
            m_inNameEdt.ShowWindow(SW_SHOW);
            m_inNameEdt.SetWindowText(strValue);
            m_inNameEdt.SetFocus();
            m_inNameEdt.SetCapture();//LSC
            int nLenth=strValue.GetLength();
            m_inNameEdt.SetSel(nLenth,nLenth); //全选//
        } 
        else
        {
            m_comboxRange.MoveWindow(&rcCell,1);
            m_comboxRange.ResetContent();
            m_comboxRange.AddString(_T("OFF"));
            m_comboxRange.AddString(_T("ON"));
            m_comboxRange.ShowWindow(SW_SHOW);
            m_comboxRange.BringWindowToTop();
            m_comboxRange.SetFocus(); //获取焦点
            m_comboxRange.SetWindowText(strValue);
        }
        
    }
    
}


void T36CT::ClickMsflexgridInput()
{
   grid_flag=1;
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
    if(2==lCol)
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


void T36CT::ClickMsflexgridInput2()
{
    grid_flag=2;
    m_comboxRange.ShowWindow(FALSE);
    UpdateData(FALSE);

    long lRow,lCol;
    lRow = m_msflexgrid_Ainput.get_RowSel();//获取点击的行号	
    lCol = m_msflexgrid_Ainput.get_ColSel(); //获取点击的列号
    TRACE(_T("Click input grid!\n"));

    CRect rect;
    m_msflexgrid_Ainput.GetWindowRect(rect); //获取表格控件的窗口矩形
    ScreenToClient(rect); //转换为客户区矩形	
    CDC* pDC =GetDC();

    int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
    int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
    //计算选中格的左上角的坐标(象素为单位)
    long y = m_msflexgrid_Ainput.get_RowPos(lRow)/nTwipsPerDotY;
    long x = m_msflexgrid_Ainput.get_ColPos(lCol)/nTwipsPerDotX;
    //计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
    long width = m_msflexgrid_Ainput.get_ColWidth(lCol)/nTwipsPerDotX+1;
    long height = m_msflexgrid_Ainput.get_RowHeight(lRow)/nTwipsPerDotY+1;
    //形成选中个所在的矩形区域
    CRect rcCell(x,y,x+width,y+height);
    //转换成相对对话框的坐标
    rcCell.OffsetRect(rect.left+1,rect.top+1);
    ReleaseDC(pDC);
    CString strValue = m_msflexgrid_Ainput.get_TextMatrix(lRow,lCol);
    m_oldname=strValue;
    m_curcol=lCol;
    m_currow=lRow;
    if (lRow==0)
    {
        return;
    }
    if(2==lCol)
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


void T36CT::OnEnKillfocusEditName()
{
    CString strTemp;
    GetDlgItem(IDC_EDIT_NAME)->GetWindowText(strTemp);
    int Value=_wtoi(strTemp);

    if (strTemp.Compare(m_oldname)==0)
    {
        return;
    }

   if (grid_flag==1)
   {
       if (m_curcol==0)
       {

           Insert_Update_Table_Name(m_sn,_T("CT_Input"),m_currow,strTemp);
           m_msflexgrid_input.put_TextMatrix(m_currow,m_curcol,strTemp);
       }
       else if (m_curcol==3)//Filter
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
   else if (grid_flag==2)
   {
       if (m_curcol==0)
       {
           Insert_Update_Table_Name(m_sn,_T("AN_Input"),m_currow,strTemp);
           m_msflexgrid_Ainput.put_TextMatrix(m_currow,m_curcol,strTemp);
       }
       else if (m_curcol==3)//Filter
       {
           int regvalue=product_register_value[FILTER_INPUT7+m_currow-1];
           if (Value!=regvalue)
           {
               int ret1=write_one(g_tstat_id,FILTER_INPUT7+m_currow-1,Value);
               if (ret1>0)
               {
                   product_register_value[FILTER_INPUT7+m_currow-1]=Value;
                   InitialDialog();
               }
           }
       }
   }
   else if (grid_flag==3)
   {
       if (m_curcol==0)
       {
           Insert_Update_Table_Name(m_sn,_T("Output"),m_currow,strTemp);
           m_msflexgrid_output.put_TextMatrix(m_currow,m_curcol,strTemp);
       }
   }
}
void T36CT::InitialTableName(){
    m_sn=product_register_value[SN_LOW]+product_register_value[SN_LOW+1]*256+product_register_value[SN_HI]*256*256+product_register_value[SN_HI+1]*256*256*256;

    CString str_input;
    for(int i=1;i<7;i++)
    {

        str_input=Get_Table_Name(m_sn,_T("CT_Input"),i);

        m_msflexgrid_input.put_TextMatrix(i,0,str_input);	 
    }
    for(int i=1;i<11;i++)
    {

        str_input=Get_Table_Name(m_sn,_T("AN_Input"),i);

        m_msflexgrid_Ainput.put_TextMatrix(i,0,str_input);	 
    }
    CString str_output;
    for(int i=1;i<6;i++)
    {

        str_output=Get_Table_Name(m_sn,_T("Output"),i);
        m_msflexgrid_output.put_TextMatrix(i,0,str_output);	


    }
}

void T36CT::OnCbnSelchangeRangecombo()
{
    if (grid_flag==1)
    {
        BeginWaitCursor();
        int sel=m_comboxRange.GetCurSel();
        m_comboxRange.ShowWindow(FALSE);
        if (product_register_value[RANGE_INPUT1+m_currow-1]==sel)
        {
            return;
        }
        int ret=write_one(g_tstat_id,RANGE_INPUT1+m_currow-1,sel);
        if (ret>0)
        {
            product_register_value[RANGE_INPUT1+m_currow-1]=sel;
        }
        Sleep(500);
        Read_Multi(g_tstat_id,&product_register_value[INPUT1_CT],INPUT1_CT,16);
        InitialDialog();
        EndWaitCursor();
    }
    else if (grid_flag==2)
    {
        BeginWaitCursor();
        CString strTemp;
         

        int sel=m_comboxRange.GetCurSel();
        m_comboxRange.ShowWindow(FALSE);
        if (product_register_value[RANGE_INPUT7+m_currow-1]==sel)
        {
            return;
        }
        int ret=write_one(g_tstat_id,RANGE_INPUT7+m_currow-1,sel);
        if (ret>0)
        {
            product_register_value[RANGE_INPUT7+m_currow-1]=sel;
        }
        Sleep(500);
        Read_Multi(g_tstat_id,&product_register_value[INPUT1_CT],INPUT1_CT,16);
        InitialDialog();
        EndWaitCursor();
    }
    else if (grid_flag==3)
    {
        if (m_curcol==1)
        {   BeginWaitCursor();
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
          
            InitialDialog();
            EndWaitCursor();
        }
    }
}
