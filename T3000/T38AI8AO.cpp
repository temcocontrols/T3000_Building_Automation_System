// T38AI8AO.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T38AI8AO.h"
 #include "../SQLiteDriver/CppSQLite3.h"
 #include "Dialog_Progess.h"
#include "global_function.h"
#include "MainFrm.h"
#include <bitset>
// T38AI8AO
#define  INPUT_ROWS 8
#define  INPUT_COLS 4
#define  OUTPUT_ROWS 8
#define  OUTPUT_COLS 3


vector<T3Register> g_VectorT3Register;
 BOOL bPauseMultiRead=FALSE;
IMPLEMENT_DYNCREATE(T38AI8AO, CFormView)
DWORD WINAPI _BackFreshing_8IAO(LPVOID pParam)
{
	T38AI8AO* dlg=(T38AI8AO*)(pParam);
	
	Sleep(10*1000);
	 

	while(1)
	{
	 
		if (!is_connect())
		{
			continue;
		}
		 Sleep(3*1000);
		 
		for(int i=0;i<3;i++) //Modify by Fance , tstat 6 has more register than 512;
		{
			 
			if(g_bPauseMultiRead)
			{

				return 0;
			}
			Read_Multi(g_tstat_id,&multi_register_value[i*100],i*100,100);
			 
		}
		 
		memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));
		dlg->InitialDialog();

	}



	return 1;
}
T38AI8AO::T38AI8AO()
	: CFormView(T38AI8AO::IDD)
	
	, m_address(_T(""))
{
hFirstThread=NULL;
}

T38AI8AO::~T38AI8AO()
{
}

void T38AI8AO::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID_INPUT, m_msflexgrid_input);
	DDX_Control(pDX, IDC_MSFLEXGRID_OUTPUT, m_msflexgrid_output);
	//DDX_Text(pDX, IDC_88ADDRESS, m_address);
	DDX_Control(pDX, IDC_BRANDRATE, m_brandratebombox);
	DDX_Control(pDX, IDC_DELAY, m_delaycombox);
	DDX_Control(pDX, IDC_RANGECOMBO, m_comboxRange);
	DDX_Control(pDX, IDC_EDIT_NAME, m_inNameEdt);
}

BEGIN_MESSAGE_MAP(T38AI8AO, CFormView)
	ON_CBN_SELCHANGE(IDC_BRANDRATE, &T38AI8AO::OnCbnSelchangeBrandrate)
//	ON_CBN_SELCHANGE(IDC_DELAY, &T38AI8AO::OnCbnSelchangeDelay)
	ON_EN_KILLFOCUS(IDC_EDIT_NAME, &T38AI8AO::OnEnKillfocusEditName)
	ON_CBN_SELCHANGE(IDC_RANGECOMBO, &T38AI8AO::OnCbnSelchangeRangecombo)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_RESET, &T38AI8AO::OnBnClickedButtonReset)
    ON_EN_KILLFOCUS(IDC_EDIT_RESPONSE_DELAY, &T38AI8AO::OnEnKillfocusEditResponseDelay)
END_MESSAGE_MAP()


// T38AI8AO diagnostics

#ifdef _DEBUG
void T38AI8AO::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void T38AI8AO::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG
void T38AI8AO::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
#if 1	
	//设置排/行数量
	m_msflexgrid_input.put_Cols(INPUT_COLS);
	m_msflexgrid_input.put_Rows(INPUT_ROWS+1);//包括标题栏
	//显示横标题
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
	for (int col=0;col<INPUT_COLS;col++)
	{ 
		m_msflexgrid_input.put_ColAlignment(col,4);
	}

	//彩色显示
	for(int i=1;i<INPUT_ROWS+1;i++)		//排数量
	{

		for(int k=0;k<INPUT_COLS;k++)	//列数量
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
	for(int i=1;i<INPUT_ROWS+1;i++)
	{

		str.Format(_T("Input%d"),i);

		m_msflexgrid_input.put_TextMatrix(i,0,str);	 
	}

	//============================================================================================================界面Output DO部份列表框

	//设置行/列数量
	m_msflexgrid_output.put_Rows(OUTPUT_ROWS+1);
	m_msflexgrid_output.put_Cols(OUTPUT_COLS);
	//设置列宽	


	//显示横标题
	m_msflexgrid_output.put_TextMatrix(0,0,_T("Output Name"));
	m_msflexgrid_output.put_TextMatrix(0,1,_T("Output Value"));
	m_msflexgrid_output.put_TextMatrix(0,2,_T("Switch Status"));
	//m_msflexgrid_output.put_TextMatrix(0,3,_T("Auto/Manual"));

	m_msflexgrid_output.put_ColWidth(0,1500);
	m_msflexgrid_output.put_ColWidth(1,1200);
	m_msflexgrid_output.put_ColWidth(2,1200);
	//m_msflexgrid_output.put_ColWidth(3,1500);//居中显示
	for (int col=0;col<OUTPUT_COLS;col++)
	{ 
		m_msflexgrid_output.put_ColAlignment(col,4);
	}

	//彩色显示
	for(int i=1;i<OUTPUT_ROWS+1;i++)		//排数量
	{

		for(int k=0;k<OUTPUT_COLS;k++)	//列数量
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
    for(int i=1;i<OUTPUT_ROWS+1;i++)
    {
        str_output.Format(_T("Output%d"),i);
        m_msflexgrid_output.put_TextMatrix(i,0,str_output);	
    }




#endif  
}
void T38AI8AO::InitialRegister(){
    T3Register temp;
	g_VectorT3Register.clear();
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3Table table;
	CppSQLite3Query q;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
#if 1
	CString SQL = _T("select * from T3_RegisterList");
	q = SqliteDBT3000.execQuery((UTF8MBSTR)SQL);
	_variant_t vartemp;
	while(!q.eof())
	{
		temp.regID=q.getIntField("RegID");
		temp.regName =q.getValuebyName(L"T3_8AI8AO");
		q.nextRow();
		g_VectorT3Register.push_back(temp);
	}
	SqliteDBT3000.closedb();
	SN_LOW=Get_RegID(_T("SN_LOW"))	;
	SN_HI=Get_RegID(_T("SN_HI"))	;
	EPROM_VER_NUMBER=Get_RegID(_T("EPROM_VER_NUMBER"))	;
	FIRMWARE_VER_NUMBER=Get_RegID(_T("FIRMWARE_VER_NUMBER"))	;
	MODBUS_ID=Get_RegID(_T("MODBUS_ID"))	;
	PRODUCT_MODEL	=Get_RegID(_T("PRODUCT_MODEL"));
	HARDWARE_VER_NUMBER=Get_RegID(_T("HARDWARE_VER_NUMBER"))	;
	PIC_VER_NUMBER=Get_RegID(_T("PIC_VER_NUMBER"))	;
	CALIBRATION_OUTPUTS	=Get_RegID(_T("CALIBRATION_OUTPUTS"));
	BAUDRATE=Get_RegID(_T("BAUDRATE"))	;
	RESPONSE_DELAY	=Get_RegID(_T("RESPONSE_DELAY"));

	OUTPUT1	=Get_RegID(_T("OUTPUT1	"));
	OUTPUT2	=Get_RegID(_T("OUTPUT2	"));
	OUTPUT3	=Get_RegID(_T("OUTPUT3	"));
	OUTPUT4	=Get_RegID(_T("OUTPUT4	"));
	OUTPUT5	=Get_RegID(_T("OUTPUT5	"));
	OUTPUT6	=Get_RegID(_T("OUTPUT6	"));
	OUTPUT7	=Get_RegID(_T("OUTPUT7	"));
	OUTPUT8	=Get_RegID(_T("OUTPUT8	"));
	INPUT1	=Get_RegID(_T("INPUT1	"));
	INPUT2	=Get_RegID(_T("INPUT2	"));
	INPUT3	=Get_RegID(_T("INPUT3	"));
	INPUT4	=Get_RegID(_T("INPUT4	"));
	INPUT5	=Get_RegID(_T("INPUT5	"));
	INPUT6	=Get_RegID(_T("INPUT6	"));
	INPUT7	=Get_RegID(_T("INPUT7	"));
	INPUT8	=Get_RegID(_T("INPUT8	"));
	SWITCH1_STATUS=Get_RegID(_T("SWITCH1_STATUS"))	;
	SWITCH2_STATUS=Get_RegID(_T("SWITCH2_STATUS"))	;
	RANGE_INPUT1	=Get_RegID(_T("RANGE_INPUT1	"));
	RANGE_INPUT2	=Get_RegID(_T("RANGE_INPUT2	"));
	RANGE_INPUT3	=Get_RegID(_T("RANGE_INPUT3	"));
	RANGE_INPUT4	=Get_RegID(_T("RANGE_INPUT4	"));
	RANGE_INPUT5	=Get_RegID(_T("RANGE_INPUT5	"));
	RANGE_INPUT6	=Get_RegID(_T("RANGE_INPUT6	"));
	RANGE_INPUT7	=Get_RegID(_T("RANGE_INPUT7	"));
	RANGE_INPUT8	=Get_RegID(_T("RANGE_INPUT8	"));
	FILTER_INPUT1	=Get_RegID(_T("FILTER_INPUT1	"));
	FILTER_INPUT2	=Get_RegID(_T("FILTER_INPUT2	"));
	FILTER_INPUT3	=Get_RegID(_T("FILTER_INPUT3	"));
	FILTER_INPUT4	=Get_RegID(_T("FILTER_INPUT4	"));
	FILTER_INPUT5	=Get_RegID(_T("FILTER_INPUT5	"));
	FILTER_INPUT6	=Get_RegID(_T("FILTER_INPUT6	"));
	FILTER_INPUT7	=Get_RegID(_T("FILTER_INPUT7	"));
	FILTER_INPUT8	=Get_RegID(_T("FILTER_INPUT8	"));
#endif 
}
int  T38AI8AO::Get_RegID(CString Name)
{
    Name.TrimLeft();
	Name.TrimRight();
	int regid=-1;
	vector<T3Register>::iterator iter;
	for (iter=g_VectorT3Register.begin();iter!=g_VectorT3Register.end();iter++)
	{
		if (iter->regName==Name)
		{
			regid=iter->regID;
			break;
		}
	}
	return regid;
}

void T38AI8AO::InitialDialog(){
	
	CString strTemp;
	strTemp.Format(_T("%d"),product_register_value[MODBUS_ID]);
    GetDlgItem(IDC_88ADDRESS)->SetWindowText(strTemp);
	int temp;
	temp=product_register_value[SN_LOW]+product_register_value[SN_LOW+1]*256+product_register_value[SN_HI]*256*256+product_register_value[SN_HI+1]*256*256*256;
	strTemp.Format(_T("%d"),temp);
	 GetDlgItem(IDC_88SERIAL)->SetWindowText(strTemp);

	 GetDlgItem(IDC_88MODEL)->SetWindowText(_T("T3-8I8OA"));
	strTemp.Format(_T("%d"),product_register_value[FIRMWARE_VER_NUMBER]);
	 GetDlgItem(IDC_88FIRMWARE)->SetWindowText(strTemp);
	strTemp.Format(_T("%d"),product_register_value[HARDWARE_VER_NUMBER]);
	 GetDlgItem(IDC_88HARDWARE)->SetWindowText(strTemp);
	strTemp.Format(_T("%d"),product_register_value[PIC_VER_NUMBER]);
	 GetDlgItem(IDC_88PIC)->SetWindowText(strTemp); 
	 m_brandratebombox.SetCurSel(product_register_value[BAUDRATE]);
	 //m_delaycombox.SetCurSel(product_register_value[RESPONSE_DELAY]);
     strTemp.Format(_T("%d"),product_register_value[RESPONSE_DELAY]);
     GetDlgItem(IDC_EDIT_RESPONSE_DELAY)->SetWindowText(strTemp);
	// Input
	CString strresult;
	int regValue;
	for(int i = 1;i<=INPUT_ROWS;i++)
	{  
		regValue=product_register_value[INPUT1];
		 

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

    bitset<16> BitSwitchValue(product_register_value[SWITCH1_STATUS]);
    int SwitchValue[OUTPUT_ROWS];
    for (int i=0;i<OUTPUT_ROWS;i++)
    {
        SwitchValue[i]=BitSwitchValue[2*i]+BitSwitchValue[2*i+1]*2;
    }
	CString CstresultDO;
	for(int i = 1;i<=OUTPUT_ROWS;i++)
	{  

        CstresultDO.Format(_T("%d"),product_register_value[OUTPUT1+i-1]);
        m_msflexgrid_output.put_TextMatrix(i,1,CstresultDO);

        m_msflexgrid_output.put_TextMatrix(i,2,STRING_SWITCH_STATUS[SwitchValue[i-1]]); 


	}
}

void T38AI8AO::Fresh(){

	 
	if (is_connect())
	{  
		 

		for (int i=0;i<3;i++)
		{
			 
			Read_Multi(g_tstat_id,&product_register_value[i*100],i*100,100);
		}
	 

		InitialRegister();	
		InitialDialog();
		if(hFirstThread != NULL)
			TerminateThread(hFirstThread, 0);
		hFirstThread=NULL;
		if (!hFirstThread)
		{
			hFirstThread = CreateThread(NULL,NULL,_BackFreshing_8IAO,this,NULL,0);
		}
		//AfxBeginThread(_BackFreshing,this);
	}
	else
	{
		SetPaneString(1,_T("Disconnection"));
		AfxMessageBox(_T("Please Connect your device"));
	}


}


void T38AI8AO::OnCbnSelchangeBrandrate()
{   bPauseMultiRead=TRUE;
	int sel=m_brandratebombox.GetCurSel();
	if (product_register_value[BAUDRATE]!=sel)
	{
		int ret=write_one(g_tstat_id,BAUDRATE,sel);
		if (ret>0)
		{  
		 if (sel==0)
		{
		Change_BaudRate(9600);
		} 
		else
		{
		Change_BaudRate(19200);
		}
		    
			product_register_value[BAUDRATE]=sel;
			InitialDialog();
		
		}
		else
		{
		  AfxMessageBox(_T("Fail"));
		}
	} 
	 
	bPauseMultiRead=FALSE;
}


//void T38AI8AO::OnCbnSelchangeDelay()
//{bPauseMultiRead=TRUE;
//	int sel=m_delaycombox.GetCurSel();
//	if (product_register_value[RESPONSE_DELAY]!=sel)
//	{
//		int ret=write_one(g_tstat_id,RESPONSE_DELAY,sel);
//		if (ret>0)
//		{
//			product_register_value[RESPONSE_DELAY]=sel;
//			InitialDialog();
//		}
//		else
//		{
//			AfxMessageBox(_T("Fail"));
//		}
//	} 
//	bPauseMultiRead=FALSE;
//}
BEGIN_EVENTSINK_MAP(T38AI8AO, CFormView)
	ON_EVENT(T38AI8AO, IDC_MSFLEXGRID_INPUT, DISPID_CLICK, T38AI8AO::ClickMsflexgridInput, VTS_NONE)
	ON_EVENT(T38AI8AO, IDC_MSFLEXGRID_OUTPUT, DISPID_CLICK, T38AI8AO::ClickMsflexgridOutput, VTS_NONE)
END_EVENTSINK_MAP()


void T38AI8AO::ClickMsflexgridInput()
{
	bPauseMultiRead=TRUE;
	m_isinput=TRUE;
	m_comboxRange.ShowWindow(FALSE);
	UpdateData(FALSE);

	long lRow,lCol;
	lRow = m_msflexgrid_input.get_RowSel();//获取点击的行号	
	lCol = m_msflexgrid_input.get_ColSel(); //获取点击的列号
	 

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
	m_curcol=lCol;
	m_currow=lRow;


	if(((3==lCol)||(1==lCol)) && lRow != 0)
	{

		m_inNameEdt.MoveWindow(&rcCell,1);
		m_inNameEdt.ShowWindow(SW_SHOW);
		m_inNameEdt.SetWindowText(strValue);
		m_inNameEdt.SetFocus();
		m_inNameEdt.SetCapture();//LSC
		int nLenth=strValue.GetLength();
		m_inNameEdt.SetSel(nLenth,nLenth); //全选//

	}
	if((2==lCol)&&lRow!=0)
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
	bPauseMultiRead=FALSE;
}


void T38AI8AO::ClickMsflexgridOutput()
{
bPauseMultiRead=TRUE;
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
	m_curcol=lCol;
	m_currow=lRow;

	if(lCol==1&&lRow!=0)
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
	bPauseMultiRead=FALSE;
}


void T38AI8AO::OnEnKillfocusEditName()
{bPauseMultiRead=TRUE;
CString strTemp;
GetDlgItem(IDC_EDIT_NAME)->GetWindowText(strTemp);
int Value=_wtoi(strTemp);
	 if (m_isinput)
	 {
		 if (m_curcol==1)
		 {
			 if (Value!=product_register_value[INPUT1+m_currow-1])
			 {
				 int ret=write_one(g_tstat_id,INPUT1+m_currow-1,Value);
				  if (ret>0)
				  {
					  product_register_value[INPUT1+m_currow-1]=Value;
					  InitialDialog();
				  }
				  else
				  {
					  AfxMessageBox(_T("Fail"));
				  }
				
			 }
		 } 
		 else if(m_curcol==3)
		 {
			 if (Value!=product_register_value[FILTER_INPUT1+m_currow-1])
			 {
				 int ret=write_one(g_tstat_id,FILTER_INPUT1+m_currow-1,Value);
				 if (ret>0)
				 {
					 product_register_value[FILTER_INPUT1+m_currow-1]=Value;
					  InitialDialog();
				 }
				 else
				 {
					 AfxMessageBox(_T("Fail"));
				 }

			 }
		 }
	 } 
	 else
	 {
		 if (m_curcol==1)
		 {
			 if (Value!=product_register_value[OUTPUT1+m_currow-1])
			 {
				 int ret=write_one(g_tstat_id,OUTPUT1+m_currow-1,Value);
				 if (ret>0)
				 {
					 product_register_value[OUTPUT1+m_currow-1]=Value;
					 InitialDialog();
				 }
				 else
				 {
					 AfxMessageBox(_T("Fail"));
				 }

			 }
		 } 
	 }
	 bPauseMultiRead=FALSE;
}


void T38AI8AO::OnCbnSelchangeRangecombo()
{
bPauseMultiRead=TRUE;
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
	else
	{
		AfxMessageBox(_T("Fail"));
	}
	InitialDialog();
	EndWaitCursor();
	bPauseMultiRead=FALSE;
}


void T38AI8AO::OnDestroy()
{
	
	if(hFirstThread != NULL)
		TerminateThread(hFirstThread, 0);
	hFirstThread=NULL;


CFormView::OnDestroy();

	
}


void T38AI8AO::OnBnClickedButtonReset()
{
	if(AfxMessageBox(_T(" This will reset the module to the factory defaults,Are you sure to reset it ?"))==IDOK)
	{
		//  write_one(g_tstat_id,299,1);
		write_one(g_tstat_id,300,1);
		unsigned short RangeData[10];

		int multi_ret = Read_Multi(g_tstat_id,RangeData,RANGE_INPUT1,10) ;
		if (multi_ret >0 )
		{
			for (int i=0; i<10 ; i++)
			{
				product_register_value[RANGE_INPUT1+i] = RangeData[i];
			}
			InitialDialog();
		}
	}
}


void T38AI8AO::OnEnKillfocusEditResponseDelay()
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
