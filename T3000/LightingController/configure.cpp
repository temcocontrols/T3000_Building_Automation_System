// configure.cpp : implementation file
//

#include "stdafx.h"
#include "..\T3000.h"
#include "configure.h"


// Cconfigure dialog

#define SETTIMER    SetTimer(1,10000,NULL);
#define KILLTIMER 	KillTimer(1);
IMPLEMENT_DYNAMIC(Cconfigure, CDialog)
DWORD WINAPI _UpdateThread_LC(LPVOID pParam);

Cconfigure::Cconfigure(CWnd* pParent /*=NULL*/)
	: CDialog(Cconfigure::IDD, pParent)
	, m_strcombo(_T(""))
{
hFirstThread=NULL;
}

Cconfigure::~Cconfigure()
{
    if(hFirstThread != NULL)
        TerminateThread(hFirstThread, 0);
}

void Cconfigure::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID_Configure, m_msflexgrid);
	DDX_Control(pDX, IDC_COMBO_SELECT, m_controlcombo);
	DDX_CBString(pDX, IDC_COMBO_SELECT, m_strcombo);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(Cconfigure, CDialog)
ON_WM_TIMER()
	ON_CBN_KILLFOCUS(IDC_COMBO_SELECT, &Cconfigure::OnCbnKillfocusComboSelect)
	ON_EN_KILLFOCUS(IDC_EDIT1, &Cconfigure::OnEnKillfocusEdit1)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &Cconfigure::OnBnClickedButtonSend)
	ON_CBN_SELCHANGE(IDC_COMBO_SELECT, &Cconfigure::OnCbnSelchangeRangecombo)
END_MESSAGE_MAP()


// Cconfigure message handlers

BOOL Cconfigure::OnInitDialog()
{
	CDialog::OnInitDialog();
	unsigned short Data[4];
	Read_Multi(g_tstat_id,Data,0,4);
	m_sn=Data[0]+Data[1]*256+Data[2]*256*256+Data[3]*256*256*256;
	m_msflexgrid.put_Rows(25); //行
	//set row high
	m_msflexgrid.put_WordWrap(TRUE);
	m_msflexgrid.put_RowHeight(0,500);
	//title middle show
	for (int n=0;n<=3;n++)
	{
		m_msflexgrid.put_ColAlignment(n,4);
	}
//设置行的高度
    
	CString str;
	for(int i=1;i<25;i++)
	{
		str=Get_Table_Name(m_sn,_T("Input"),i);
		m_msflexgrid.put_RowHeight(i,300);
		m_msflexgrid.put_TextMatrix(i,0,str);
		m_msflexgrid.put_TextMatrix(i,1,_T("ON/OFF"));
	}
	m_msflexgrid.put_Cols(7);
	//设置列宽
	m_msflexgrid.put_ColWidth(0,1200);
	m_msflexgrid.put_ColWidth(1,1500);
	m_msflexgrid.put_ColWidth(2,1500);
	m_msflexgrid.put_ColWidth(3,1500);



	m_msflexgrid.put_TextMatrix(0,0,_T("INPUTS"));
	m_msflexgrid.put_TextMatrix(0,1,_T("TYPE"));
	m_msflexgrid.put_TextMatrix(0,2,_T("Function"));
	m_msflexgrid.put_TextMatrix(0,3,_T("Override Time(s)"));
	m_msflexgrid.put_TextMatrix(0,4,_T("Auto/Manual"));
	m_msflexgrid.put_TextMatrix(0,5,_T("Delay Time(s)"));
    m_msflexgrid.put_TextMatrix(0,6,_T("Time Left(s)"));
	//读取硬件数据

	//201…224	1 * 24	switch (1..24) types: 0 --- low active,  1--- high active,   2 --- falling edge active,    3 --- rising edge active
	//252…275	2 * 24	override time for each switch. Uint is second. 2bytes = 65536s =~18hours max.
	memset(m_switch,0,sizeof(m_switch));
    memset(m_overridetime,0,sizeof(m_overridetime));
    memset(m_MB,0,sizeof(m_MB));
    memset(m_AM,0,sizeof(m_AM));
    memset(m_DT,0,sizeof(m_DT));
    memset(m_LT,0,sizeof(m_LT));
	 int flg = Read_Multi(g_tstat_id,m_switch,201,24);
	 int flag =Read_Multi(g_tstat_id,m_overridetime,252,24);

	 Read_Multi(g_tstat_id,m_MB,4172,24);
     Read_Multi(g_tstat_id,m_AM,4148,24);
	 Read_Multi(g_tstat_id,m_DT,14536,24);
     Read_Multi(g_tstat_id,m_LT,276,24);
	WORD itemp[24];
	memcpy(itemp,m_switch,sizeof(m_switch));
	CString strtemp;
	for (int i = 0;i<24;i++)
	{
		//TYPE
		itemp[i]= itemp[i]&0x0F;
		switch(itemp[i])
		{

		case 1:
			m_msflexgrid.put_TextMatrix(i+1,1,_T("Low"));
			break;
		case 2:
			m_msflexgrid.put_TextMatrix(i+1,1,_T("High"));
			break;
		case 4:
			m_msflexgrid.put_TextMatrix(i+1,1,_T("Edge"));
			break;
		}
		//Function
		m_switch[i] = m_switch[i]>>4;
		switch(m_switch[i])
		{
		case 1:
			m_msflexgrid.put_TextMatrix(i+1,2,_T("ONLY ON"));
			break;
		case 2:
			m_msflexgrid.put_TextMatrix(i+1,2,_T("ON/OFF"));
			break;
		}		
		//Overridetime
		strtemp.Format(_T("%d"),m_overridetime[i]);
		m_msflexgrid.put_TextMatrix(i+1,3,strtemp);
        if (m_AM[i]==0)
        {
        strtemp=_T("Auto");
        } 
        else
        {
            if (m_MB[i]==1)
            {
                strtemp=_T("ON");
            }
            else
            {
                strtemp=_T("OFF");
            }
        }
		
	 
		m_msflexgrid.put_TextMatrix(i+1,4,strtemp);
		strtemp.Format(_T("%d"),m_DT[i]);
		m_msflexgrid.put_TextMatrix(i+1,5,strtemp);
        strtemp.Format(_T("%d"),m_LT[i]);
        m_msflexgrid.put_TextMatrix(i+1,6,strtemp);
}
    if(hFirstThread != NULL)
        TerminateThread(hFirstThread, 0);
    hFirstThread=NULL;
    if (!hFirstThread)
    {
        hFirstThread = CreateThread(NULL,NULL,_UpdateThread_LC,this,NULL,0);
    }
    SETTIMER
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void Cconfigure::Refresh(){
    memset(m_switch,0,sizeof(m_switch));
    memset(m_overridetime,0,sizeof(m_overridetime));
    memset(m_MB,0,sizeof(m_MB));
    memset(m_AM,0,sizeof(m_AM));
    memset(m_DT,0,sizeof(m_DT));
    memset(m_LT,0,sizeof(m_LT));
    int flg = Read_Multi(g_tstat_id,m_switch,201,24);
    int flag =Read_Multi(g_tstat_id,m_overridetime,252,24);

    Read_Multi(g_tstat_id,m_MB,4172,24);
    Read_Multi(g_tstat_id,m_AM,4148,24);
    Read_Multi(g_tstat_id,m_DT,14536,24);
    Read_Multi(g_tstat_id,m_LT,276,24);
    WORD itemp[24];
    memcpy(itemp,m_switch,sizeof(m_switch));
    CString strtemp;
    for (int i = 0;i<24;i++)
    {
        //TYPE
        itemp[i]= itemp[i]&0x0F;
        switch(itemp[i])
        {

        case 1:
            m_msflexgrid.put_TextMatrix(i+1,1,_T("Low"));
            break;
        case 2:
            m_msflexgrid.put_TextMatrix(i+1,1,_T("High"));
            break;
        case 4:
            m_msflexgrid.put_TextMatrix(i+1,1,_T("Edge"));
            break;
        }
        //Function
        m_switch[i] = m_switch[i]>>4;
        switch(m_switch[i])
        {
        case 1:
            m_msflexgrid.put_TextMatrix(i+1,2,_T("ONLY ON"));
            break;
        case 2:
            m_msflexgrid.put_TextMatrix(i+1,2,_T("ON/OFF"));
            break;
        }		
        //Overridetime
        strtemp.Format(_T("%d"),m_overridetime[i]);
        m_msflexgrid.put_TextMatrix(i+1,3,strtemp);
        if (m_AM[i]==0)
        {
            strtemp=_T("Auto");
        } 
        else
        {
            if (m_MB[i]==1)
            {
                strtemp=_T("ON");
            }
            else
            {
                strtemp=_T("OFF");
            }
        }


        m_msflexgrid.put_TextMatrix(i+1,4,strtemp);
        strtemp.Format(_T("%d"),m_DT[i]);
        m_msflexgrid.put_TextMatrix(i+1,5,strtemp);
        strtemp.Format(_T("%d"),m_LT[i]);
        m_msflexgrid.put_TextMatrix(i+1,6,strtemp);
    }
}

BEGIN_EVENTSINK_MAP(Cconfigure, CDialog)
	ON_EVENT(Cconfigure, IDC_MSFLEXGRID_Configure, DISPID_CLICK, Cconfigure::ClickMsflexgridConfigure, VTS_NONE)
END_EVENTSINK_MAP()
DWORD WINAPI _UpdateThread_LC(LPVOID pParam)
{
     
        Cconfigure* pDlg = (Cconfigure*)(pParam);
        /*pDlg->Fresh_Hum_Temp();*/
        while(1){
        pDlg->ChangingLeftTime();
        Sleep(2000);
        }
    
    return 1;
}
void Cconfigure::ClickMsflexgridConfigure()
{
    KILLTIMER
	long lRow = m_msflexgrid.get_RowSel();//获取点击的行号	
	long lCol = m_msflexgrid.get_ColSel(); //获取点击的列号	

	CRect rect;	
	m_msflexgrid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形
	// MSFlexGrid控件的函数的长度单位是"缇(twips)"，
	//需要将其转化为像素，1440缇= 1英寸
	CDC* pDC =GetDC();
	//计算象素点和缇的转换比例
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_msflexgrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_msflexgrid.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_msflexgrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_msflexgrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标	
	rc.OffsetRect(rect.left+1,rect.top+1);	
	//获取选中格的文本信息	
	m_CurRow=lRow;
	m_CurCol=lCol;
	CString strValue = m_msflexgrid.get_TextMatrix(lRow,lCol);
	m_oldname=strValue;
	if (lCol == 1||lCol ==2||lCol==4)
	{
		//使用combobox控件形式显示
		m_edit.ShowWindow(SW_HIDE);
		m_controlcombo.ShowWindow(SW_SHOW);

		m_controlcombo.ResetContent();

		if (lCol == 1)
		{

			m_controlcombo.AddString(_T("Low"));
			m_controlcombo.AddString(_T("High"));
			m_controlcombo.AddString(_T("Edge"));
		 
		}
		else if (lCol == 2)
		{
			m_controlcombo.AddString(_T("ONLY ON"));
			m_controlcombo.AddString(_T("ON/OFF"));
		}

		else
		{
            m_controlcombo.AddString(_T("Auto"));
            m_controlcombo.AddString(_T("ON"));
			m_controlcombo.AddString(_T("OFF"));
			 
		}
		m_controlcombo.MoveWindow(&rc,1); //移动到选中格的位置
		m_controlcombo.BringWindowToTop();	
		m_controlcombo.SelectString(-1,strValue);
		m_controlcombo.ShowWindow(SW_SHOW);//显示控件
	}
	else if (lCol ==3||lCol ==5)
	{  
        if (lCol==3)
        {
        CString StrContent=m_msflexgrid.get_TextMatrix(lRow,lCol-1);
        if (StrContent.CompareNoCase(_T("ON/OFF"))==0)
        {
        return;
        }
        //m_msflexgrid.put_TextMatrix(i+1,2,_T("ONLY ON"));
        }
        
         
       
		//使用edition控件形式显示
		m_controlcombo.ShowWindow(SW_HIDE);
		m_edit.MoveWindow(&rc,1);
		m_edit.ShowWindow(SW_SHOW);	
		m_edit.SetWindowText(strValue);	
		m_edit.SetFocus();
		m_edit.SetCapture();//使随后的鼠标输入都被发送到这个CWnd 
		int nLenth=strValue.GetLength();	
		m_edit.SetSel(nLenth,nLenth); 	

	}
	else
	{
		m_edit.ShowWindow(SW_HIDE);
		m_controlcombo.ShowWindow(SW_HIDE);
	}
    SETTIMER
}

void Cconfigure::OnCbnKillfocusComboSelect()
{KILLTIMER
	long lRow = m_msflexgrid.get_RowSel();//获取点击的行号	
	long lCol = m_msflexgrid.get_ColSel(); //获取点击的列号	



	CString strTemp;
	int nindext = m_controlcombo.GetCurSel();//GetEditSel();	 GetCurSel
	if (nindext>=0)
	{	   
		m_controlcombo.GetLBText(nindext,strTemp);
		m_msflexgrid.put_TextMatrix(lRow,lCol,strTemp);
	}	

    SETTIMER

	
}

void Cconfigure::OnEnKillfocusEdit1()
{
SETTIMER
	CString strText;
	m_edit.GetWindowText(strText); 
	m_edit.ShowWindow(SW_HIDE);
	if (strText.CompareNoCase(m_oldname)==0)
	{
	return;
	}
	int Value=_wtoi(strText);
	if (5==m_CurCol)
	{
		
		write_one(g_tstat_id,14535+m_CurRow,Value);  
	} 
	else
	{
	   write_one(g_tstat_id,251+m_CurRow,Value);
	}
	
	 

	  Refresh(); 
      KILLTIMER
}

void Cconfigure::OnBnClickedButtonSend()
{
//	CString str,str1,str3;
//	BYTE Sendswitch[24];
//	BYTE SendTimebye[48];
//	WORD SendTimeword[24];
//	WORD Wtemp = 0;
//
//	BYTE bytetmep = 0;
//
//	memset(Sendswitch,0,sizeof(Sendswitch));
//	memset(SendTimebye,0,sizeof(SendTimebye));
//	memset(SendTimeword,0,sizeof(SendTimeword));
//
//	for (int i = 0;i<24;i++)
//	{
//		 str = m_msflexgrid.get_TextMatrix(i+1,1);
//		if(str.CompareNoCase(_T("ON/OFF")) == 0) 
//		{
//			Sendswitch[i] = 0;
//
//		}else if (str.CompareNoCase(_T("OFF/ON") )== 0)
//		{
//			Sendswitch[i] = 1;
//
//		}else if (str.CompareNoCase(_T("SW OPEN PULSE")) == 0)
//		{
//			Sendswitch[i] = 2;
//
//		}else if (str.CompareNoCase(_T("SW CLOSE PULSE")) == 0)
//		{
//			Sendswitch[i] = 3;
//
//		}
//
//		str1 = m_msflexgrid.get_TextMatrix(i+1,2);
//	     if (str1.CompareNoCase(_T("ON&OFF")) == 0)
//		{
//			bytetmep = 1;
//			bytetmep = bytetmep<<4;
//			Sendswitch[i] = Sendswitch[i]|bytetmep;
//		}
//
//		 //time
//		 str3 = m_msflexgrid.get_TextMatrix(i+1,3);
//		 SendTimeword[i] = _ttoi(str3);
//// 		 Wtemp = SendTimeword[i];
//// 		 Wtemp = Wtemp&0x0F;
//// 		 SendTimebye[i*2+1] = (BYTE)Wtemp;
//// 		 Wtemp = SendTimeword[i];
//// 		 Wtemp = Wtemp>>8;
//// 		 SendTimebye[i*2] = (BYTE)Wtemp;
//
//	}
//
//
//	//201…224	1 * 24	switch (1..24) types: 0 --- low active,  1--- high active,   2 --- falling edge active,    3 --- rising edge active
//	//252…275	2 * 24	override time for each switch. Uint is second. 2bytes = 65536s =~18hours max.
//	int ret1=0,ret=0;
//	ret = Write_Multi(g_tstat_id,Sendswitch,201,24);
//	//int ret1 = Write_Multi(g_tstat_id,SendTimebye,252,48);
//	for (int i = 0;i<24;i++)
//	{
//		 ret1= write_one(g_tstat_id,252+i,SendTimeword[i]);
//	
//	}
//	
//
//	if ((ret>0)&&(ret1>0))
//		AfxMessageBox(_T("Send successful!"));
//	else
//		AfxMessageBox(_T("Send unsuccessful!"));

}
void Cconfigure::OnCbnSelchangeRangecombo()
{
KILLTIMER
  int Item=m_controlcombo.GetCurSel();
  int RegValue,Value;
  int ret=0;
  if (1==m_CurCol)
  {
    RegValue=read_one(g_tstat_id,200+m_CurRow);
	if (RegValue<0)
	{
	RegValue=0;
	}
    if (0==Item)
    {
	  Value=1;
    }
	else if (1==Item)
	{
	Value=2;
	}
	else if (2==Item)
	{
	Value=4;
	}
	RegValue&=0xF0;
	RegValue+=Value;
    ret=write_one(g_tstat_id,200+m_CurRow,RegValue);
    if (ret<0)
    {
    AfxMessageBox(_T("Setting Fail!"));
    }
  }
  else if (2==m_CurCol)
  {
    RegValue=read_one(g_tstat_id,200+m_CurRow);
	if (RegValue<0)
	{
		RegValue=0;
	}
	if (1==Item)
	{
		Value=32;
	}
	else if (0==Item)
	{
		Value=16;
	}
	RegValue&=0x0F;//留底位，去高位
	RegValue+=Value;//置高位
	ret=write_one(g_tstat_id,200+m_CurRow,RegValue);


  }
  else if (4==m_CurCol)
  {
     // write_one(g_tstat_id,4171+m_CurRow,Item);
     if (Item==0)
     {
     ret=write_one(g_tstat_id,4171+m_CurRow,Item);
     if (ret<0)
     {
         AfxMessageBox(_T("Setting Fail!"));
     }
     ret=write_one(g_tstat_id,4147+m_CurRow,Item);
     if (ret<0)
     {
         AfxMessageBox(_T("Setting Fail!"));
     }
     } 
     else if (Item==1)
     {
         ret=write_one(g_tstat_id,4171+m_CurRow,Item);
         if (ret<0)
         {
             AfxMessageBox(_T("Setting Fail!"));
         }
         ret=write_one(g_tstat_id,4147+m_CurRow,Item);
         if (ret<0)
         {
             AfxMessageBox(_T("Setting Fail!"));
         }
     }
     else
     {
     ret=write_one(g_tstat_id,4171+m_CurRow,0);
     if (ret<0)
     {
         AfxMessageBox(_T("Setting Fail!"));
     }
     ret=write_one(g_tstat_id,4147+m_CurRow,1);
     if (ret<0)
     {
         AfxMessageBox(_T("Setting Fail!"));
     }
     }

  }
  Refresh();
  SETTIMER
}
void Cconfigure::ChangingLeftTime(){
    memset(m_LT,0,sizeof(m_LT));
    Read_Multi(g_tstat_id,m_LT,276,24);
    CString strtemp;
    for (int i = 0;i<24;i++)
    {
        strtemp.Format(_T("%d"),m_LT[i]);
        m_msflexgrid.put_TextMatrix(i+1,6,strtemp);
    }
}
void Cconfigure::OnTimer(UINT_PTR nIDEvent)
{
    //CMainFrame*pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
    //CView* pNewView = pMain->m_pViews[8];
    //CView* pActiveView = pMain->GetActiveView();
    //if (pNewView != pActiveView)
    //{
    //    KillTimer(1);
    //    //恢复T3000主线程
    //   /* pMain->m_pFreshMultiRegisters->ResumeThread();
    //    pMain->m_pRefreshThread->ResumeThread();*/

    //}

    if(1 == nIDEvent)
    {

    }
  CDialog::OnTimer(nIDEvent);
}