// T36CT.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T36CT.h"
 #include "../SQLiteDriver/CppSQLite3.h"
 
#include "Dialog_Progess.h"
 
#include "global_function.h"
#include "MainFrm.h"

// T36CT
#define FRESH_CT6_BACK  WM_USER+1006

DWORD WINAPI _BackFreshing_6CT(LPVOID pParam)
{
	T36CT* dlg=(T36CT*)(pParam);

	Sleep(1000);


	while(dlg->IsWindowVisible())
	{

		if (!is_connect())
		{
			Sleep(1000);
			continue;
		}
		Sleep(1000);
		/* register_critical_section.Lock();*/
		//          for(int i=1;i<3;i++) 
		//          {

		if(!no_mouse_keyboard_event_enable_refresh) 
		{
			Sleep(3000);
			continue ;
		}
		else
		{
			Sleep(2000);
		}

		if(g_bPauseMultiRead)
		{
			 
			return 0;
		}
		Read_Multi(g_tstat_id,&product_register_value[0],0,100);  

		Read_Multi(g_tstat_id,&product_register_value[100],100,100); 

		PostMessage(dlg->m_hWnd,FRESH_CT6_BACK,0,0);
	}



	return 1;
}


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
	DDX_Control(pDX, IDC_BAUDRATECOMBO, m_brandrateBoxbox);
}

BEGIN_MESSAGE_MAP(T36CT, CFormView)
    ON_BN_CLICKED(IDC_CHANGEID, &T36CT::OnBnClickedChangeid)
   // ON_EN_KILLFOCUS(IDC_EDIT_NAME, &T36CT::OnEnKillfocusEditName)
    ON_CBN_SELCHANGE(IDC_RANGECOMBO, &T36CT::OnCbnSelchangeRangecombo)
	ON_CBN_SELCHANGE(IDC_BAUDRATECOMBO, &T36CT::OnCbnSelchangeBaudratecombo)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_RESET, &T36CT::OnBnClickedButtonReset)
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
    m_msflexgrid_output.put_Cols(3);
    //设置列宽	


    //显示横标题
    m_msflexgrid_output.put_TextMatrix(0,0,_T("Output Name"));
    m_msflexgrid_output.put_TextMatrix(0,1,_T("Switch Status"));
    m_msflexgrid_output.put_TextMatrix(0,2,_T("Output Value"));


    for (int col=0;col<3;col++)
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

	if (product_register_value[BAUDRATE]==0)
	{
	  m_brandrateBoxbox.SetCurSel(0);
	}
	else
	{
	   m_brandrateBoxbox.SetCurSel(1);
	}
	
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

    m_msflexgrid_input.put_TextMatrix(i,2,strresult);
    strresult.Format(_T("%d"),product_register_value[FILTER_INPUT1+i-1]);
    m_msflexgrid_input.put_TextMatrix(i,3,strresult);
}


for(int i = 1;i<=10;i++)
{  
    regValue=product_register_value[INPUT1+i-1];
    if (0==product_register_value[RANGE_INPUT7+i-1])
    {
        strresult.Format(_T("%d"),regValue);
    } 
    else if (1==product_register_value[RANGE_INPUT7+i-1])
    {
	   regValue=(short)product_register_value[INPUT1+i-1];

        strresult.Format(_T("%.1f C"),(float)regValue/10.0);
    }
    else if (2==product_register_value[RANGE_INPUT7+i-1])
    {

	    regValue=(short)product_register_value[INPUT1+i-1];
        strresult=_T("10K F");
        strresult.Format(_T("%.1f F"),(float)regValue/10.0);
    }
    else if (3==product_register_value[RANGE_INPUT7+i-1])
    {

        strresult.Format(_T("%.1f"),(float)regValue);
        strresult+=_T("%");
    }
    else if (4==product_register_value[RANGE_INPUT7+i-1])
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
    else if (5==product_register_value[RANGE_INPUT7+i-1])
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
    else if (6==product_register_value[RANGE_INPUT7+i-1])
    {
        strresult.Format(_T("%d"),regValue);
    }
    else if (7==product_register_value[RANGE_INPUT7+i-1])
    {
        strresult.Format(_T("%d"),regValue);
    }
    else if (8==product_register_value[RANGE_INPUT7+i-1])
    {
        //strresult=_T("TYPE3 10K C");
        strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
    }
    else if (9==product_register_value[RANGE_INPUT7+i-1])
    {
        /*strresult=_T("TYPE3 10K F");*/
        strresult.Format(_T("%0.1f F"),(float)regValue/10.0);
    }
    else if (10==product_register_value[RANGE_INPUT7+i-1])
    {
        strresult=_T("0");
        //strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
    }
    else if (11==product_register_value[RANGE_INPUT7+i-1])
    {
        //strresult=_T("0-5V");
        strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
    }
    else if (12==product_register_value[RANGE_INPUT7+i-1])
    {
        //strresult=_T("0-10V");
        strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
    }
    else if (13==product_register_value[RANGE_INPUT7+i-1])
    {
         
        strresult.Format(_T("%0.1f ma"),(float)regValue/1000.0);
    }
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
        strresult=_T("4-20 ma");
    }

    m_msflexgrid_Ainput.put_TextMatrix(i,2,strresult);
    strresult.Format(_T("%d"),product_register_value[FILTER_INPUT7+i-1]);
    m_msflexgrid_Ainput.put_TextMatrix(i,3,strresult);
}
CString CstresultDO;
int b0,b1,b;
for(int i = 1;i<=5;i++)
{  

    CstresultDO.Format(_T("%d"),product_register_value[OUTPUT1+i-1]);
    m_msflexgrid_output.put_TextMatrix(i,2,CstresultDO);
    b0=Get_Bit_FromRegister(product_register_value[SWITCH1_BANK],2*(i-1)+1);
    b1=Get_Bit_FromRegister(product_register_value[SWITCH1_BANK],2*(i-1)+2);
    b=b1*2+b0;

    if (i==5)
    {
        b0=Get_Bit_FromRegister(product_register_value[SWITCH1_BANK],1);
        b1=Get_Bit_FromRegister(product_register_value[SWITCH1_BANK],2);
        b=b1*2+b0;

    }
    if (b==0)
    {
        CstresultDO=_T("OFF");
    }
    else if (b==1)
    {
        CstresultDO=_T("ON");
    }
    else if (b==2)
    {
        CstresultDO=_T("AUTO");
    }
    else
    {
        CstresultDO=_T("");
    }
    m_msflexgrid_output.put_TextMatrix(i,1,CstresultDO);

}


}

void T36CT::Fresh()
{
    
    if (is_connect())
    {  
         

        Initial_RegisterList();
        InitialDialog();
		if(hFirstThread != NULL)
			TerminateThread(hFirstThread, 0);
		hFirstThread=NULL;
		if (!hFirstThread)
		{
			hFirstThread = CreateThread(NULL,NULL,_BackFreshing_6CT,this,NULL,0);
		}
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
	ON_EVENT(T36CT, IDC_MSFLEXGRID_OUTPUT, 71, T36CT::EnterCellMsflexgridOutput, VTS_NONE)
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
        if (m_curcol==0||m_curcol==2)
        {
            m_inNameEdt.MoveWindow(&rcCell,1);
            m_inNameEdt.ShowWindow(SW_SHOW);
            m_inNameEdt.SetWindowText(strValue);
            m_inNameEdt.SetFocus();
            m_inNameEdt.SetCapture();//LSC
            int nLenth=strValue.GetLength();
            m_inNameEdt.SetSel(nLenth,nLenth); //全选//
        } 
        //else 
        //{
        //    m_comboxRange.MoveWindow(&rcCell,1);
        //    m_comboxRange.ResetContent();
        //    m_comboxRange.AddString(_T("OFF"));
        //    m_comboxRange.AddString(_T("ON"));
        //    m_comboxRange.ShowWindow(SW_SHOW);
        //    m_comboxRange.BringWindowToTop();
        //    m_comboxRange.SetFocus(); //获取焦点
        //    m_comboxRange.SetWindowText(strValue);
        //}
        
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
        m_comboxRange.AddString(_T("4-20 ma"));

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
        m_comboxRange.AddString(_T("4-20 ma"));
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
	   if (m_curcol==2)
	   {
		   int regvalue=product_register_value[OUTPUT1+m_currow-1];
		   if (Value!=regvalue)
		   {
			   int ret1=write_one(g_tstat_id,OUTPUT1+m_currow-1,Value);
			   if (ret1>0)
			   {
				   product_register_value[OUTPUT1+m_currow-1]=Value;
				   InitialDialog();
			   }
		   }
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
    for (int i=0;i<3;i++)
    {
         
        Read_Multi(g_tstat_id,&product_register_value[i*100],i*100,100);
    }
     InitialDialog();
}


void T36CT::OnCbnSelchangeBaudratecombo()
{
   int sel=m_brandrateBoxbox.GetCurSel();
   int bandrate;
   if (product_register_value[BAUDRATE]!=sel)
   {
        write_one(g_tstat_id,BAUDRATE,sel);

	   if (sel==0)
	   {
		   bandrate=9600;
	   }
	   else
	   {
		   bandrate=19200;
	   }

	    Change_BaudRate(bandrate);
		Sleep(1000);

		int nowbandrate=Read_One(g_tstat_id,BAUDRATE);
		if (nowbandrate!=sel)
		{
		   return;
		}
		
	    product_register_value[BAUDRATE] = sel;
		CppSQLite3DB SqliteDBT3000;
		CppSQLite3Query q;
		SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
		CString sql;
		sql.Format(_T("Select * from ALL_NODE where Serial_ID = '%d' "),m_sn);
		q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);
		if (!q.eof())//有表但是没有对应序列号的值
		{
			sql.Format(_T("update ALL_NODE set Bautrate = '%d' where Serial_ID = '%d'"),bandrate,m_sn);
			 SqliteDBT3000.execDML((UTF8MBSTR)sql);
		}
		 SqliteDBT3000.closedb();
		 
		InitialDialog();
		CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
		::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING,0,0);
	  
	   
   }
}


LRESULT T36CT::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	
	if (message==FRESH_CT6_BACK)
	{
	    InitialDialog();
	}
	return CFormView::WindowProc(message, wParam, lParam);
}


void T36CT::OnDestroy()
{



	if(hFirstThread != NULL)
		TerminateThread(hFirstThread, 0);
	hFirstThread=NULL;
 

	CFormView::OnDestroy();

	
}


void T36CT::EnterCellMsflexgridOutput()
{
	 OnEnKillfocusEditName();
}


void T36CT::OnBnClickedButtonReset()
{
	if(AfxMessageBox(_T(" This will reset the module to the factory defaults,Are you sure to reset it ?"))==IDOK)
	{
		//  write_one(g_tstat_id,299,1);
		write_one(g_tstat_id,300,1);
	}
}
