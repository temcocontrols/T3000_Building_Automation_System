// T332AI.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T332AI.h"
#include "Dialog_Progess.h"
 
#include "global_function.h"
#include "MainFrm.h"
// T332AI
CString Range_Unit_T332AI[]={_T("RAW DATA"),_T("TYPE2 10K C"),_T("TYPE2 10K F"),_T("0-100%"),_T("ON/OFF"),_T("OFF/ON"),_T("Pulse Input"),_T("Lighting Control"),_T("TYPE3 10K C"),_T("TYPE3 10K F"),_T("NO USE"),_T("0-5V"),_T("0-10V"),_T("4-20ma")};

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
	DDX_Control(pDX, IDC_RANGECOMBO, m_comboxRange);
    
}

BEGIN_MESSAGE_MAP(T332AI, CFormView)
	ON_EN_KILLFOCUS(IDC_EDIT_NAME, &T332AI::OnEnKillfocusEditName)
	ON_MESSAGE(WM_HOTKEY,&T332AI::OnHotKey)//快捷键消息映射手动加入
	ON_CBN_SELCHANGE(IDC_RANGECOMBO, &T332AI::OnCbnSelchangeRangecombo)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &T332AI::OnBnClickedButtonReset)
//    ON_CBN_SELCHANGE(IDC_DELAY, &T332AI::OnCbnSelchangeDelay)
ON_EN_KILLFOCUS(IDC_EDIT_RESPONSE_DELAY, &T332AI::OnEnKillfocusEditResponseDelay)
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
	m_msflexgrid_input.put_Cols(4);
	m_msflexgrid_input.put_Rows(33);//包括标题栏
	//显示横标题
	m_msflexgrid_input.put_TextMatrix(0,0,_T("Input Name"));
	m_msflexgrid_input.put_TextMatrix(0,1,_T("Range"));
	m_msflexgrid_input.put_TextMatrix(0,2,_T("Register Value"));
    m_msflexgrid_input.put_TextMatrix(0,3,_T("Filter"));




	//设置列宽	
	m_msflexgrid_input.put_ColWidth(0,1000);
	m_msflexgrid_input.put_ColWidth(1,1500);
    m_msflexgrid_input.put_ColWidth(2,1500);
	//居中显示
	for (int col=0;col<3;col++)
	{ 
		m_msflexgrid_input.put_ColAlignment(col,4);
	}

	//彩色显示
	for(int i=1;i<33;i++)		//排数量
	{

		for(int k=0;k<3;k++)	//列数量
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
	//CString str;
	//for(int i=1;i<33;i++)
	//{

	//	str.Format(_T("Input%d"),i);

	//	m_msflexgrid_input.put_TextMatrix(i,0,str);	 
	//}

 




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
	
	CString strTemp;
	strTemp.Format(_T("%d"),product_register_value[MODBUS_ID]);
	GetDlgItem(IDC_EDIT_T3ADDRESS)->SetWindowText(strTemp);
	int temp;
	temp=product_register_value[SN_LOW]+product_register_value[SN_LOW+1]*256+product_register_value[SN_HI]*256*256+product_register_value[SN_HI+1]*256*256*256;
	m_sn=temp;
	strTemp.Format(_T("%d"),temp);
	GetDlgItem(IDC_EDIT_T3SERIAL)->SetWindowText(strTemp);

	GetDlgItem(IDC_EDIT_T3MODEL)->SetWindowText(_T("T3-32AI"));
	strTemp.Format(_T("%d"),product_register_value[FIRMWARE_VER_NUMBER]);
	GetDlgItem(IDC_EDIT_T3FIRMWARE)->SetWindowText(strTemp);
	strTemp.Format(_T("%d"),product_register_value[HARDWARE_VER_NUMBER]);
	GetDlgItem(IDC_EDIT_T3HARDWARE)->SetWindowText(strTemp);
	strTemp.Format(_T("%d"),product_register_value[PIC_VER_NUMBER]);
	GetDlgItem(IDC_EDIT_T3PICVERSION)->SetWindowText(strTemp);
    strTemp.Format(_T("%d"),product_register_value[RESPONSE_DELAY]);
    GetDlgItem(IDC_EDIT_RESPONSE_DELAY)->SetWindowText(strTemp);
	// Input
	CString strresult;
	int regValue;
	for(int i = 1;i<=32;i++)
	{  
	    strresult=Get_Table_Name(m_sn,_T("Input"),i);
		m_msflexgrid_input.put_TextMatrix(i,0,strresult);
		if (0==product_register_value[Range1+i-1])
		{
			strresult=_T("RAW DATA");

		} 
		else if (1==product_register_value[Range1+i-1])
		{
			strresult=_T("10K C");
		}
		else if (2==product_register_value[Range1+i-1])
		{
			strresult=_T("10K F");
		}
		else if (3==product_register_value[Range1+i-1])
		{
			strresult=_T("0-100%");
		}
		else if (4==product_register_value[Range1+i-1])
		{
			strresult=_T("ON/OFF");
		}
		else if (5==product_register_value[Range1+i-1])
		{
			strresult=_T("OFF/ON");
		}
		else if (6==product_register_value[Range1+i-1])
		{
			strresult=_T("Pulse Input");
		}
		else if (7==product_register_value[Range1+i-1])
		{
			strresult=_T("Lighting Control");
		}
		else if (8==product_register_value[Range1+i-1])
		{
			strresult=_T("TYPE3 10K C");
		}
		else if (9==product_register_value[Range1+i-1])
		{
			strresult=_T("TYPE3 10K F");
		}
		else if (10==product_register_value[Range1+i-1])
		{
			strresult=_T("NO USE");
		}
		else if (11==product_register_value[Range1+i-1])
		{
			strresult=_T("0-5V");
		}
		else if (12==product_register_value[Range1+i-1])
		{
			strresult=_T("0-10V");
		}
		else if (13==product_register_value[Range1+i-1])
		{
			strresult=_T("4-20ma");
		}

		m_msflexgrid_input.put_TextMatrix(i,1,strresult);


		regValue=(short)product_register_value[INPUT1+i-1];
		//regValue=(short)product_register_value[INPUT1_PULSE_COUNT_LOW+2*(i-1)];

		if (0==product_register_value[Range1+i-1])
		{
			strresult.Format(_T("%d"),regValue);

		} 
		else if (1==product_register_value[Range1+i-1])
		{
			strresult.Format(_T("%.1f C"),(float)regValue/10.0);
		}
		else if (2==product_register_value[Range1+i-1])
		{
			strresult=_T("10K F");
			strresult.Format(_T("%.1f F"),(float)regValue/10.0);
		}
		else if (3==product_register_value[Range1+i-1])
		{

			strresult.Format(_T("%.1f"),(float)regValue);
			strresult+=_T("%");
		}
		else if (4==product_register_value[Range1+i-1])
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
		else if (5==product_register_value[Range1+i-1])
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
		else if (6==product_register_value[Range1+i-1])
		{
			strresult.Format(_T("%d"),regValue);
		}
		else if (7==product_register_value[Range1+i-1])
		{
			strresult.Format(_T("%d"),regValue);
		}
		else if (8==product_register_value[Range1+i-1])
		{
			//strresult=_T("TYPE3 10K C");
			strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
		}
		else if (9==product_register_value[Range1+i-1])
		{
			/*strresult=_T("TYPE3 10K F");*/
			strresult.Format(_T("%0.1f F"),(float)regValue/10.0);
		}
		else if (10==product_register_value[Range1+i-1])
		{
			strresult=_T("0");
			//strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
		}
		else if (11==product_register_value[Range1+i-1])
		{
			//strresult=_T("0-5V");
			strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
		}
		else if (12==product_register_value[Range1+i-1])
		{
			//strresult=_T("0-10V");
			strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
		}
		else if (13==product_register_value[Range1+i-1])
		{
			//strresult=_T("0-20I");
			strresult.Format(_T("%0.1f ma"),(float)regValue/1000.0);
		}
		m_msflexgrid_input.put_TextMatrix(i,2,strresult);

		strresult.Format(_T("%d"),product_register_value[Filter1+i-1]);
		m_msflexgrid_input.put_TextMatrix(i,3,strresult);
	}

}
void T332AI::Fresh()
{

	float progress;
	if (is_connect())
	{  
		/*CDialog_Progess* pDlg = new CDialog_Progess(this,1,100);
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
		pDlg=NULL;}*/

		Initial_RegisterList();
		InitialDialog();
	}
	else
	{
		SetPaneString(1,_T("Disconnection"));
		AfxMessageBox(_T("Please Connect your device"));
	}
    
}
BEGIN_EVENTSINK_MAP(T332AI, CFormView)
	ON_EVENT(T332AI, IDC_MSFLEXGRID_INPUT, DISPID_CLICK, T332AI::ClickMsflexgridInput, VTS_NONE)
END_EVENTSINK_MAP()


void T332AI::ClickMsflexgridInput()
{
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

	if(0==lCol ||2==lCol||3==lCol)
	{
	   m_comboxRange.ShowWindow(SW_HIDE);
		m_inNameEdt.MoveWindow(&rcCell,1);
		m_inNameEdt.ShowWindow(SW_SHOW);
		m_inNameEdt.SetWindowText(strValue);
		m_inNameEdt.SetFocus();
		m_inNameEdt.SetCapture();//LSC
		int nLenth=strValue.GetLength();
		m_inNameEdt.SetSel(nLenth,nLenth); //全选//

	}
	else if (1==lCol)
	{m_inNameEdt.ShowWindow(SW_HIDE);
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


void T332AI::OnEnKillfocusEditName()
{
	CString strTemp;
	GetDlgItem(IDC_EDIT_NAME)->GetWindowText(strTemp);
	unsigned short Value=_wtoi(strTemp);
	if (strTemp.Compare(m_oldname)==0)
	{
		return;
	}
	if (m_curcol==0)
	{
		Insert_Update_Table_Name(m_sn,_T("Input"),m_currow,strTemp);
		m_msflexgrid_input.put_TextMatrix(m_currow,m_curcol,strTemp);
	}
	else if (m_curcol==2)
	{

		unsigned short regvalue=product_register_value[Range1+m_currow-1];
		if (regvalue>13)
		{
			regvalue=0;
		}
		Value=Value*Get_Unit_Process(Range_Unit_T332AI[regvalue]);

	      if (Value!= product_register_value[INPUT1+m_currow-1])
	      {
		    int ret=write_one(g_tstat_id,INPUT1+m_currow-1,Value,10);
			if (ret>0)
			{
			product_register_value[INPUT1+m_currow-1]=Value;
			InitialDialog();
			}
			else
			{
			AfxMessageBox(_T("Write Fail"));
			}
	      }
		 
	}
	else if (m_curcol==3)
	{
		int regvalue=product_register_value[Filter1+m_currow-1];
		if (Value!=regvalue)
		{
			int ret1=write_one(g_tstat_id,Filter1+m_currow-1,Value);
			if (ret1>0)
			{
				product_register_value[Filter1+m_currow-1]=Value;
				InitialDialog();
			}
		}
	}
}


LRESULT T332AI::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	if (wParam==VK_RETURN)
	{
		 OnEnKillfocusEditName(); 
	}
	return 0;
	 
}


void T332AI::OnCbnSelchangeRangecombo()
{
	BeginWaitCursor();
	CString strTemp;
	int lRow = m_msflexgrid_input.get_RowSel();	
	int lCol = m_msflexgrid_input.get_ColSel();

	int sel=m_comboxRange.GetCurSel();
	m_comboxRange.ShowWindow(FALSE);
	if (product_register_value[Range1+lRow-1]==sel)
	{
		return;
	}
	int ret=write_one(g_tstat_id,Range1+lRow-1,sel);
	if (ret>0)
	{
		product_register_value[Range1+lRow-1]=sel;
	}		  
	Sleep(500);
	Read_Multi(g_tstat_id,&product_register_value[Range1],Range1,32);
	InitialDialog();
	EndWaitCursor();
}


void T332AI::OnBnClickedButtonReset()
{
	if(AfxMessageBox(_T(" This will reset the module to the factory defaults,Are you sure to reset it ?"))==IDOK)
	{
		//  write_one(g_tstat_id,299,1);
		write_one(g_tstat_id,300,1);

		unsigned short RangeData[40];

		int multi_ret = Read_Multi(g_tstat_id,RangeData,Range1,40) ;
		if (multi_ret >0 )
		{
			for (int i=0; i<40 ; i++)
			{
			 product_register_value[Range1+i] = RangeData[i];
			}
			InitialDialog();
		}
	}
}


 


void T332AI::OnEnKillfocusEditResponseDelay()
{
    CString strValue;
    GetDlgItem(IDC_EDIT_RESPONSE_DELAY)->GetWindowText(strValue);
    int IntValue;
    IntValue = _wtoi(strValue);
    int ret = write_one(g_tstat_id,RESPONSE_DELAY,IntValue);
    if (ret>0)
    {
        product_register_value[RESPONSE_DELAY] = IntValue;
        AfxMessageBox (_T("OK"));
    }
    else
    {
        AfxMessageBox (_T("Failure!"));
    }
    InitialDialog();
}


BOOL T332AI::PreTranslateMessage(MSG* pMsg)
{
    
    if(pMsg->message == WM_KEYDOWN  )
    {
        if(pMsg->wParam == VK_RETURN)
        {
            CWnd *temp_focus=GetFocus();	//Maurice require ,click enter and the cursor still in this edit or combobox.
            GetDlgItem(IDC_EDIT1_TEST)->SetFocus();
            temp_focus->SetFocus();

            return 1;
        }
    }

    return CFormView::PreTranslateMessage(pMsg);
}
