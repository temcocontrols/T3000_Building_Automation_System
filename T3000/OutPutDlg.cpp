﻿// OutPutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "OutPutDlg.h"
#include "globle_function.h"
#include "AfxMessageDialog.h"
// COutPutDlg dialog
#define FLEXGRID_CELL_COLOR RGB(128,128,128)
#define FLEX_GRID1_PUT_COLOR_STR(row,col,str)	{m_FlexGrid1.put_TextMatrix(row,col+5,_T(""));m_FlexGrid1.put_Row(row);m_FlexGrid1.put_Col(col+5);m_FlexGrid1.put_CellBackColor(FLEXGRID_CELL_COLOR);}
//#define FLEX_GRID1_PUT_COLOR_STR(row,col,str)	{m_FlexGrid1.put_TextMatrix(row,col+5,str);m_FlexGrid1.put_Row(row);m_FlexGrid1.put_Col(col+5);m_FlexGrid1.put_CellBackColor(FLEXGRID_CELL_COLOR);}
//#define FLEX_GRID2_PUT_COLOR_STR(row,col,str)	{m_FlexGrid2.put_TextMatrix(row,col+5,str);m_FlexGrid2.put_Row(row);m_FlexGrid2.put_Col(col+5);m_FlexGrid2.put_CellBackColor(FLEXGRID_CELL_COLOR);}
#define FLEX_GRID2_PUT_COLOR_STR(row,col,str)	{m_FlexGrid2.put_TextMatrix(row,col+5,_T(""));m_FlexGrid2.put_Row(row);m_FlexGrid2.put_Col(col+5);m_FlexGrid2.put_CellBackColor(FLEXGRID_CELL_COLOR);}
#define FLEX_GRID1_PUT_STR(row,col,str)			{m_FlexGrid1.put_TextMatrix(row,col+5,str);m_FlexGrid1.put_Row(row);m_FlexGrid1.put_Col(col+5);m_FlexGrid1.put_CellForeColor(RGB(0,0,0));m_FlexGrid1.put_CellBackColor(RGB(255,255,255));}//str must be declare
#define FLEX_GRID2_PUT_STR(row,col,str)			{m_FlexGrid2.put_TextMatrix(row,col+5,str);m_FlexGrid2.put_Row(row);m_FlexGrid2.put_Col(col+5);m_FlexGrid2.put_CellForeColor(RGB(0,0,0));m_FlexGrid2.put_CellBackColor(RGB(255,255,255));}//str must be declare

IMPLEMENT_DYNAMIC(COutPutDlg, CDialog)
CString heat_hand[6]={_T("Heat1"),_T("Heat2"),_T("Heat3"),_T("Heat4"),_T("Heat5"),_T("Heat6")};
CString cool_hand[6]={_T("Cool1"),_T("Cool2"),_T("Cool3"),_T("Cool4"),_T("Cool5"),_T("Cool6")};


int write_one_show_message(unsigned char device_var,unsigned short address,short value,int retry_times=3);
bool last_output_write_register_result=false;

UINT FreshOutputTable(LPVOID pParam)
{
	COutPutDlg* pDlg=(COutPutDlg*)pParam;
//	pDlg->below_26_7_fresh();
	return 1;
}
COutPutDlg::COutPutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COutPutDlg::IDD, pParent)
{
//	memset(&multi_register_value[0],0,sizeof(multi_register_value));
	m_bFanAutoOnly=FALSE;
	m_bOut4PWM=FALSE;
	m_bOut5PWM=FALSE;
	m_bFloat=FALSE;
	i_104_pid1=i_268_pid2=0;
	m_pids = -1;

}

COutPutDlg::~COutPutDlg()
{
}

void COutPutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FANMODE, m_fan_mode_ctrl);
	DDX_Control(pDX, IDC_CBFAN, m_fan);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_FlexGrid1);
	DDX_Control(pDX, IDC_FANAUTOCHECK, m_fanAutoCheck);
	DDX_Text(pDX, IDC_PID1_HEATSTAGEEDIT, m_PID1_heat_stages);
	DDX_Text(pDX, IDC_PID1COOLSTAGEEDIT, m_PID1_cool_stages);
	DDX_Control(pDX, IDC_PID1_HEATSTAGEEDIT, m_PID1_heatEdit);
	DDX_Control(pDX, IDC_PID1COOLSTAGEEDIT, m_PID1_coolEdit);
	DDX_Control(pDX, IDC_DESCRIPTEDIT, m_DescriptEdt);
	DDX_Control(pDX, IDC_VALUEITEMCOMBO, m_ItemValueCombx);
	DDX_Control(pDX, IDC_PID2_HEATSTAGEEDIT2, m_PID2_heatEdit);
	DDX_Control(pDX, IDC_PID2COOLSTAGEEDIT2, m_PID2_coolEdit);
	DDX_Control(pDX, IDC_MSFLEXGRID2, m_FlexGrid2);

}


BEGIN_MESSAGE_MAP(COutPutDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_FANMODE, &COutPutDlg::OnCbnSelchangeFanmode)
	ON_CBN_SELCHANGE(IDC_CBFAN, &COutPutDlg::OnCbnSelchangeCbfan)
	ON_EN_KILLFOCUS(IDC_PID1_HEATSTAGEEDIT, &COutPutDlg::OnEnKillfocusPid1Heatstageedit)
	ON_EN_KILLFOCUS(IDC_PID1COOLSTAGEEDIT, &COutPutDlg::OnEnKillfocusPid1coolstageedit)
	ON_BN_CLICKED(IDC_FANAUTOCHECK, &COutPutDlg::OnBnClickedFanautocheck)
	ON_CBN_KILLFOCUS(IDC_VALUEITEMCOMBO, &COutPutDlg::OnCbnKillfocusValueitemcombo)
	ON_CBN_SELCHANGE(IDC_VALUEITEMCOMBO, &COutPutDlg::OnCbnSelchangeValueitemcombo)
	ON_EN_KILLFOCUS(IDC_PID2_HEATSTAGEEDIT2, &COutPutDlg::OnEnKillfocusPid2Heatstageedit2)
	ON_EN_KILLFOCUS(IDC_PID2COOLSTAGEEDIT2, &COutPutDlg::OnEnKillfocusPid2coolstageedit2)
	ON_BN_CLICKED(IDCANCEL, &COutPutDlg::OnBnClickedCancel)
	ON_EN_KILLFOCUS(IDC_DESCRIPTEDIT, &COutPutDlg::OnEnKillfocusDescriptedit)
	ON_EN_SETFOCUS(IDC_DESCRIPTEDIT, &COutPutDlg::OnEnSetfocusDescriptedit)
	ON_BN_CLICKED(IDC_UPDATE, &COutPutDlg::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_REFRESH, &COutPutDlg::OnBnClickedRefresh)
END_MESSAGE_MAP()

BOOL COutPutDlg::OnInitDialog()
{

	strdemo = _T("1,");
	SetPaneString(2,strdemo);
	if(g_OutPutLevel==1)
	{
		strdemo = _T("1-1,");
		SetPaneString(2,strdemo);
		CAfxMessageDialog dlg;
		CString strPromption;
		strPromption.LoadString(IDS_STRNOPRIVILEGE);
		dlg.SetPromtionTxt(strPromption);
		dlg.DoModal();
	}

	strdemo = _T("1-2,");
	SetPaneString(2,strdemo);
	CDialog::OnInitDialog();
	m_ItemValueCombx.ShowWindow(SW_HIDE);
	m_DescriptEdt.ShowWindow(SW_HIDE);
//	OnCbnSelchangeFanmode();
	//129	107	1	Low byte	W/R	"AUTO_ONLY , enables or disables manual mode. 0 = Manual Fan Modes 1-x Allowed 
	//(depending on R122 value, 1 = Auto Mode Only, 2 = DDC mode,the user can not change setpoint and fan speed from keypad."

	if ((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7))
	{	
		if(product_register_value[107]==0)
		{
			m_bFanAutoOnly=FALSE;
			//m_fanAutoCheck.GetCheck()==BST_CHECKED;
			m_fanAutoCheck.SetCheck(BST_UNCHECKED);
		}	
		if(product_register_value[107]==1)
		{
			m_bFanAutoOnly=TRUE;
			m_fanAutoCheck.SetCheck(BST_CHECKED);
		}
		//122	105	1	Low byte	W/R	FAN MODE, number of fan speeds. Single speed = 1 up to three speed fan = 3
		if(product_register_value[105]>0&&product_register_value[105]<=3)
			m_fan_mode_ctrl.SetCurSel(product_register_value[105]-1);

		strdemo = _T("1-3,");
		SetPaneString(2,strdemo);
	}else
	{
		if(multi_register_value[129]==0)
		{
			m_bFanAutoOnly=FALSE;
			//m_fanAutoCheck.GetCheck()==BST_CHECKED;
			m_fanAutoCheck.SetCheck(BST_UNCHECKED);
		}	
		if(multi_register_value[129]==1)
		{
			m_bFanAutoOnly=TRUE;
			m_fanAutoCheck.SetCheck(BST_CHECKED);
		}
		if(multi_register_value[122]>0&&multi_register_value[122]<=3)
			m_fan_mode_ctrl.SetCurSel(multi_register_value[122]-1);

	}


	put_fan_variable();
	strdemo = _T("2,");
	SetPaneString(2,strdemo);
	
	if(g_ifanStatus<m_fan.GetCount())
		m_fan.SetCurSel(g_ifanStatus);
		
	if(!g_strFan.IsEmpty())
	{
		CString strLbText;
		for(int i=0;i<m_fan.GetCount();i++)
		{
			m_fan.GetLBText(i,strLbText);
			if(strLbText.CompareNoCase(g_strFan)==0)
			{
				m_fan.SetCurSel(i);
				break;
			}
		}
			
	}

	//FreshGrids();
//	283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
//	284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM


	int output4_value=0;//multi_register_value[283];//2 rows plug in one row
	int output5_value=0;//multi_register_value[284];

	//modify by Fance
	
	//---------------------------------------------------
	//Annul by Fance 2013 04 07
	if ((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7))
	{
		output4_value = product_register_value[MODBUS_MODE_OUTPUT4];// 283 205
		output5_value = product_register_value[MODBUS_MODE_OUTPUT5];//284 206
	}else
	{
		output4_value =multi_register_value[283];
		output5_value =multi_register_value[284];
	}
	if(output4_value==1||output5_value==1)
	{
		m_bFloat=TRUE;
	}
	//PWM 根据 336 337 判断，
	/*if (output4_value==2)
	{
		m_bOut4PWM=TRUE;
	}
	if(output5_value==2)
	{
		m_bOut5PWM=TRUE;
	}*/

	//288	247	1	Low byte	W/R	Interlock for  output3
	//104	384	2	Full	R	PID1, current PI calculation for PID number 1  (PID2 is in register 240)
	//有问题～～～;
	//i_104_pid1 = product_register_value[MODBUS_COOLING_PID];//104  384
	//i_268_pid2 = product_register_value[MODBUS_INTERLOCK_OUTPUT3];//     247
	//---------------------------------------------------
	//Annul by Fance 2013 04 07
	if ((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7))
	{
		i_104_pid1=product_register_value[384];
		i_268_pid2=product_register_value[247];
	}
	else
	{
		i_104_pid1=multi_register_value[104];
		i_268_pid2=multi_register_value[268];
	}
	//---------------------------------------------------

	//put_fan_variable();	
	FreshGrids();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void COutPutDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialog::OnOK();
}


//This function recoded by Fance 2013 04 08
void COutPutDlg::put_fan_variable()
{
	CString p[6]={_T("Fan Off"),_T("Fan On"),_T("Fan Low"),_T("Fan Mid"),_T("Fan High"),_T("Fan Aut")};
	m_fan.ResetContent(); //Comments by Fance . Removes all items from the list box and edit control of a combo box.
	//129	107	1	Low byte	W/R	"AUTO_ONLY , enables or disables manual mode. 0 = Manual Fan Modes 1-x Allowed 
	//(depending on R122 value, 1 = Auto Mode Only, 2 = DDC mode,the user can not change setpoint and fan speed from keypad."
	if ((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7))
	{
		strdemo = _T("1-4-0-1,");
		SetPaneString(2,strdemo);
		//if(read_one(g_tstat_id,107)==1)////here can't use  function
		if(product_register_value[107] == 1)////here can't use  function  如果去读，常会死在这里。
		{
			strdemo = _T("1-4-1,");
			SetPaneString(2,strdemo);
			m_fan.AddString(p[0]);
			m_fan.AddString(p[5]);	
			m_fan_mode_ctrl.EnableWindow(FALSE);
			
			strdemo = _T("1-4-3,");
			SetPaneString(2,strdemo);
			if (product_register_value[273]==0)
				m_fan.SetCurSel(0);
			else
				m_fan.SetCurSel(1);		
		}
		else
		{	
			//122	105	1	Low byte	W/R	FAN MODE, number of fan speeds. Single speed = 1 up to three speed fan = 3
			strdemo = _T("1-4-2,");
			SetPaneString(2,strdemo);
			if (product_register_value[105]>0)
				m_fan_mode_ctrl.SetCurSel(product_register_value[105]-1);
			else
				m_fan_mode_ctrl.SetCurSel(0);
			switch(m_fan_mode_ctrl.GetCurSel())
			{
			case 0:m_fan.AddString(p[0]);m_fan.AddString(p[1]);m_fan.AddString(p[5]);break;
			case 1:m_fan.AddString(p[0]);m_fan.AddString(p[2]);m_fan.AddString(p[4]);m_fan.AddString(p[5]);break;
			case 2:m_fan.AddString(p[0]);m_fan.AddString(p[2]);m_fan.AddString(p[3]);m_fan.AddString(p[4]);m_fan.AddString(p[5]);break;
			default:m_fan.AddString(p[0]);m_fan.AddString(p[2]);m_fan.AddString(p[3]);m_fan.AddString(p[4]);m_fan.AddString(p[5]);break;
			}

			m_fan_mode_ctrl.EnableWindow(TRUE);

			strdemo = _T("1-4-3,");
			SetPaneString(2,strdemo);
			if (product_register_value[273]>0)
				m_fan.SetCurSel(product_register_value[273]);
			else
				m_fan.SetCurSel(0);
		}
//137	273	1	Low byte	W/R	FAN_SPEED, current operating fan speed   
// Relay Output Tables (bit0 = relay1, bit1 = relay2, bit2 = relay3, bit3 = relay4, bit4 = relay5)


		
	}
	else
	{

// 		strdemo = _T("read_one(g_tstat_id,129)==1开始");
// 		SetPaneString(2,strdemo);
		//if(read_one(g_tstat_id,129)==1)////here can't use  function//0912
		if (multi_register_value[129] == 1)
		{
			m_fan.AddString(p[0]);
			m_fan.AddString(p[5]);			
		}
		else
		{				
			m_fan_mode_ctrl.SetCurSel(multi_register_value[122]-1);
			switch(m_fan_mode_ctrl.GetCurSel())
			{
			case 0:m_fan.AddString(p[0]);m_fan.AddString(p[1]);m_fan.AddString(p[5]);break;
			case 1:m_fan.AddString(p[0]);m_fan.AddString(p[2]);m_fan.AddString(p[4]);m_fan.AddString(p[5]);break;
			case 2:m_fan.AddString(p[0]);m_fan.AddString(p[2]);m_fan.AddString(p[3]);m_fan.AddString(p[4]);m_fan.AddString(p[5]);break;
		   default:m_fan.AddString(p[0]);m_fan.AddString(p[2]);m_fan.AddString(p[3]);m_fan.AddString(p[4]);m_fan.AddString(p[5]);break;
			}
		}
// 		strdemo = _T("read_one(g_tstat_id,129)==1完成");
// 		SetPaneString(2,strdemo);
		m_fan.SetCurSel(multi_register_value[137]);
	}

	strdemo = _T("1-5,");
	SetPaneString(2,strdemo);

}



int COutPutDlg::get_real_fan_select()
{

	int fan_i=m_fan.GetCurSel();
	if(m_fanAutoCheck.GetCheck()==BST_CHECKED)
	{
		if(fan_i==0)
			fan_i=0;
		else if(fan_i==1)
			fan_i=4;
	}
	else
	{
		int fan_mode_i=m_fan_mode_ctrl.GetCurSel();
		switch(fan_mode_i)
		{
		case 0:if(fan_i==0)return 0;if(fan_i==1)return 1;if(fan_i==2)return 4;break;
		case 1:if(fan_i==0)return 0;if(fan_i==1)return 1;if(fan_i==2)return 3;if(fan_i==3)return 4;break;
		case 2:if(fan_i==0)return 0;if(fan_i==1)return 1;if(fan_i==2)return 2;if(fan_i==3)return 3;if(fan_i==4)return 4;break;
		default:if(fan_i==0)return 0;if(fan_i==1)return 1;if(fan_i==2)return 2;if(fan_i==3)return 3;if(fan_i==4)return 4;break;	
		}
	}
	return fan_i;
}
void COutPutDlg::OnCbnSelchangeFanmode()
{
	//122	105	1	Low byte	W/R	FAN MODE, number of fan speeds. Single speed = 1 up to three speed fan = 3
	//137	273	1	Low byte	W/R	FAN_SPEED, current operating fan speed   
	//Relay Output Tables (bit0 = relay1, bit1 = relay2, bit2 = relay3, bit3 = relay4, bit4 = relay5)
	//	"Fan0 table is for the off state. Fan1, Fan2, and Fan3 are for the manual states. 
	//	Fan4 is for the Auto state.  These states are controlled by the user."
	//	The mode of operation (coasting, cooling, heating) is determined by the PID parameter.

 
	if ((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7))
	{
		write_one(g_tstat_id, 105,m_fan_mode_ctrl.GetCurSel()+1);
		write_one(g_tstat_id, 273,m_fan.GetCurSel());
		product_register_value[105] = m_fan_mode_ctrl.GetCurSel()+1;
		product_register_value[273] = m_fan.GetCurSel();
	 
	}else
	{
		write_one(g_tstat_id, 122,m_fan_mode_ctrl.GetCurSel()+1);
		write_one(g_tstat_id, 137,m_fan.GetCurSel());
	}

	put_fan_variable();
	FreshGrids();
}

void COutPutDlg::OnCbnSelchangeCbfan()
{
	// TODO: Add your control notification handler code here
	g_ifanStatus=m_fan.GetCurSel();
	//m_fan.GetWindowText(g_strFan);
	m_fan.GetLBText(g_ifanStatus,g_strFan);

// 	137	273	1	Low byte	W/R	FAN_SPEED, current operating fan speed   
// 		Relay Output Tables (bit0 = relay1, bit1 = relay2, bit2 = relay3, bit3 = relay4, bit4 = relay5)
// 		"Fan0 table is for the off state. Fan1, Fan2, and Fan3 are for the manual states. 
//		Fan4 is for the Auto state.  These states are controlled by the user."
// 		The mode of operation (coasting, cooling, heating) is determined by the PID parameter.
	//if (newtstat6[7] == PM_TSTAT6)
	  
	 if (product_register_value[MODBUS_AUTO_ONLY]==1)
	{
	   int sel=m_fan.GetCurSel();
	   if (sel==0)//OFF
	   {
		   int ret=write_one(g_tstat_id, MODBUS_FAN_SPEED,sel);
		   if (ret>0)
		   {

			   product_register_value[MODBUS_FAN_SPEED]=0;
		   }
	   } 
	   else//Auto
	   {
		   int ret=write_one(g_tstat_id, MODBUS_FAN_SPEED,4);
		   if (ret>0)
		   {
			   product_register_value[MODBUS_FAN_SPEED]=4;
		   }
	   }
	} 
	else
	{
		int ret=write_one(g_tstat_id, MODBUS_FAN_SPEED,m_fan.GetCurSel());
		if (ret>0)
		{

			product_register_value[MODBUS_FAN_SPEED]=m_fan.GetCurSel();
		}
	}
		
		
	 
	//put_fan_variable();
 
	FreshGrids();
}

void COutPutDlg::OnEnKillfocusPid1Heatstageedit()
{
	CString strText;
	GetDlgItem(IDC_PID1_HEATSTAGEEDIT)->GetWindowText(strText);
	m_PID1_heat_stages=_wtoi(strText);
	if(m_PID1_heat_stages+m_PID1_cool_stages>6)
	{

		AfxMessageBox(_T("Too many stages for PID1!"));
		m_PID1_heat_stages = 3;
		product_register_value[MODBUS_HEAT_ORIGINAL_TABLE] = 3;
		GetDlgItem(IDC_PID1_HEATSTAGEEDIT)->SetWindowText(_T("3"));
		//UpdateData(FALSE);
		return;
	}
	else
	{
 
		 
			int ret=write_one(g_tstat_id,MODBUS_HEAT_ORIGINAL_TABLE,m_PID1_heat_stages);
			 
			if (ret>0)
			{
				product_register_value[MODBUS_HEAT_ORIGINAL_TABLE] = m_PID1_heat_stages;
				 
				FreshGrid_PID1tstat6();
			} 
			else
			{
			   m_PID1_heat_stages=product_register_value[MODBUS_HEAT_ORIGINAL_TABLE];
			   AfxMessageBox(_T("Try again"));
			}
			
		 


	}
	 
}

void COutPutDlg::OnEnKillfocusPid1coolstageedit()
{
	CString strText;
	GetDlgItem(IDC_PID1COOLSTAGEEDIT)->GetWindowText(strText);
	m_PID1_cool_stages=_wtoi(strText);
	if(m_PID1_heat_stages+m_PID1_cool_stages>6)
	{
		AfxMessageBox(_T("Too many stages for PID1!"));
		m_PID1_cool_stages = 3;
		product_register_value[MODBUS_COOL_ORIGINAL_TABLE] = 3;
		GetDlgItem(IDC_PID1COOLSTAGEEDIT)->SetWindowText(_T("3"));
		//UpdateData(FALSE);
		return;
	}
	else
	{

		//277	333	1	Low byte	W/R	Number of Cooling Stages in Original Table - (Maximum # of total heating and cooling states is 6)


	 
			int ret=write_one(g_tstat_id,MODBUS_COOL_ORIGINAL_TABLE,m_PID1_cool_stages);
			//m_PID1_cool_stages=newtstat6[333];
			if (ret>0)
			{
				 
				product_register_value[MODBUS_COOL_ORIGINAL_TABLE]=m_PID1_cool_stages;
				FreshGrid_PID1tstat6();
			} 
			else
			{
			AfxMessageBox(_T("Try again"));
			}
		 
		}


	 
}
void COutPutDlg::FreshGrids()
{
	//if (newtstat6[7] == PM_TSTAT6)
	if ((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7))
	{
		FreshGrid_PID1tstat6();
		FreshGrid_PID2tstat6();
		//FreshGrid_PID1tstat6();
	}
	else if (product_register_value[7]==PM_TSTAT5E)
	{
		FreshGrid_PID1tstat6();
		FreshGrid_PID2tstat6();
	}
	else
	{
		FreshGrid_PID1();
		FreshGrid_PID2();
	}

}
void COutPutDlg::FreshGrid_PID1()
{

if(multi_register_value[129]==0)
	m_bFanAutoOnly=FALSE;
if(multi_register_value[129]==1)
	m_bFanAutoOnly=TRUE;
 

	if(multi_register_value[122]>0)
		m_fan_mode_ctrl.SetCurSel(multi_register_value[122]-1);
	
	if(multi_register_value[137]>=0)
		m_fan.SetCurSel(multi_register_value[137]);

	int output4_value=multi_register_value[283];//2 rows plug in one row
	int output5_value=multi_register_value[284];
	 
		if(output4_value==1||output5_value==1)
		{
			m_bFloat=TRUE;
		}
	 

	int output4_PWMvalue=multi_register_value[336];//2 rows plug in one row
	int output5_PWMvalue=multi_register_value[337];

	if (output4_PWMvalue==3)
		{
		m_bOut4PWM=TRUE;
		}
	if(output5_PWMvalue==3)
		{
		m_bOut5PWM=TRUE;
		}



	CString strTemp;
	m_PID1_heat_stages=multi_register_value[276];//276 register ,heat stages
	m_PID1_cool_stages=multi_register_value[277];//277 register ,cool stages
	if(m_PID1_heat_stages==0&&m_PID1_cool_stages==0)
	{
	//	write_one(g_tstat_id,276,3);
	//	write_one(g_tstat_id,277,3);
		m_PID1_heat_stages=3;
		m_PID1_cool_stages=3;
		if(m_PID1_heat_stages>6)
			m_PID1_heat_stages=6;
		if(m_PID1_cool_stages>6)
			m_PID1_cool_stages=6;
	}
	strTemp.Format(_T("%d"),m_PID1_heat_stages);
	m_PID1_heatEdit.SetWindowText(strTemp);
	strTemp.Format(_T("%d"),m_PID1_cool_stages);
	m_PID1_coolEdit.SetWindowText(strTemp);



	m_FlexGrid1.put_Rows(8);
	m_FlexGrid1.put_Cols(m_PID1_heat_stages+m_PID1_cool_stages+7);//interlock control first_blank coast

	m_FlexGrid1.put_TextMatrix(0,1,_T("Description"));
	m_FlexGrid1.put_TextMatrix(0,2,_T("Function"));
	m_FlexGrid1.put_TextMatrix(0,3,_T("Rotation"));
	m_FlexGrid1.put_ColWidth(3,0);//隐藏Rotation
	m_FlexGrid1.put_TextMatrix(0,4,_T("Control"));
	m_FlexGrid1.put_TextMatrix(0,5,_T("InterLock"));
	int i=0;
	for(i=0;i<m_PID1_heat_stages;i++)
		m_FlexGrid1.put_TextMatrix(0,6+i,heat_hand[m_PID1_heat_stages-i-1]);
	m_FlexGrid1.put_TextMatrix(0,m_PID1_heat_stages+6,_T("Coast"));
	for(i=0;i<m_PID1_cool_stages;i++)
		m_FlexGrid1.put_TextMatrix(0,m_PID1_heat_stages+7+i,cool_hand[i]);
		//Row1-7 "output1-7"
	m_FlexGrid1.put_TextMatrix(1,1,g_strOutName1);
	m_FlexGrid1.put_TextMatrix(2,1,g_strOutName2);
	m_FlexGrid1.put_TextMatrix(3,1,g_strOutName3);
	m_FlexGrid1.put_TextMatrix(4,1,g_strOutName4);
	m_FlexGrid1.put_TextMatrix(5,1,g_strOutName5);
	m_FlexGrid1.put_TextMatrix(6,1,g_strOutName6);
	m_FlexGrid1.put_TextMatrix(7,1,g_strOutName7);
	
	//Row 1-7
	m_FlexGrid1.put_TextMatrix(1,0,_T("1"));
	m_FlexGrid1.put_TextMatrix(2,0,_T("2"));
	m_FlexGrid1.put_TextMatrix(3,0,_T("3"));
	m_FlexGrid1.put_TextMatrix(4,0,_T("4"));
	m_FlexGrid1.put_TextMatrix(5,0,_T("5"));
	m_FlexGrid1.put_TextMatrix(6,0,_T("6"));
	m_FlexGrid1.put_TextMatrix(7,0,_T("7"));
	m_FlexGrid1.put_ColAlignment(0,4);
	m_FlexGrid1.put_ColAlignment(1,4);
		

	int moduleValue;

	int totalrows,row;
	float version=get_curtstat_version();
	if(version>=25)
	{
		moduleValue=multi_register_value[7];//product_model register

		switch(moduleValue)
		{
		case PM_PRESSURE:
		case 12:m_nmoduleType=3;break;//tstat 5d
		case PM_TSTAT6:
		case PM_TSTAT7:
		case 16:m_nmoduleType=3;break;//tstat 5e
		case 4:m_nmoduleType=2;break;//tstat 5c
		case 3:m_nmoduleType=0;break;//tstat 5b2 ,same to tstat 5b
		case 2:m_nmoduleType=1;break;//tstat 5a
		case 1:m_nmoduleType=0;break;//tstat 5b
		case 18:m_nmoduleType=3;break;//tstat 5G
		case 17:m_nmoduleType=2;break;//tstat 5f
		}
	}
	else
	{
		moduleValue=multi_register_value[130];//output_scale1
		if(moduleValue==0 || moduleValue==4)//add by jwq
			m_nmoduleType=0;//tstat 5a
		else
			m_nmoduleType=1;//tstat 5b
	}
	if(m_nmoduleType == 3)//5d
		m_FlexGrid1.put_Rows(8);
	else
		m_FlexGrid1.put_Rows(6);
	if(m_nmoduleType == 1 )//5a
		totalrows = 3;
	else 
		totalrows = 5;
		
	if(m_nmoduleType == 3)//2.5.0.99
		totalrows = 7;//只要是Tstat都是7个输出

	int pid_select2[7]={0};
	if (m_nmoduleType==1)
	{
		for(i=0;i<3;i++)
		pid_select2[i]=multi_register_value[247+i];/////////////////////////////get pid select ;col one 1
		pid_select2[3]=multi_register_value[252];
		pid_select2[4]=multi_register_value[253];
		//
	}
	else
	{
		for(i=0;i<7;i++)
			pid_select2[i]=multi_register_value[247+i];/////////////////////////////get pid select ;col one 1
	}
	m_pids = pid_select2[0];//lsc add 默认为pid1
	//new feature:
	short nRotation;
	short nFunction=multi_register_value[328];
	for (int k=0; k<4; k++)
	{
		nRotation=multi_register_value[329+k];
		strTemp.Format(_T("%d"),nRotation);
		m_FlexGrid1.put_TextMatrix(k+2,3,strTemp);
		if ((nFunction& (1<<k))>0)
		{
			m_FlexGrid1.put_TextMatrix(k+2,2,_T("Rotation"));
		}
		else//==0
		{
			m_FlexGrid1.put_TextMatrix(k+2,2,_T("Normal"));
		}
	}
	unsigned short interlock[7]={0};
	//	Read_Multi(tstat_id,interlock,286,7);
	unsigned short tstatval,pos,nValue;
	for(int col = 1 ;col <=(m_PID1_heat_stages+m_PID1_cool_stages+1);col++)
	{
		if(col < (m_PID1_heat_stages+1))
			pos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - col ;
		else
			pos = col - (m_PID1_heat_stages+1);
		for( row = 1;row<=totalrows;row++)//****************************
		{
			if (row==4&&m_bOut4PWM)
			{
				continue;
			}
			if (row==5&&m_bOut5PWM)
			{
				continue;
			}
			if (multi_register_value[7] == 18)//5G
			{
				if(multi_register_value[283] == 1&&row==4)
					continue;
				if(multi_register_value[284] == 1&&row==5)
					continue;
			}
			else
			{
			if(m_bFloat&&row==4)
				continue;
			if(m_bFloat&&row==5)
				continue;
			}

			int nOutReg;
			nOutReg=286+row-1;
			//int nValue=read_one(tstat_id,nOutReg);
			int nValue=multi_register_value[nOutReg];
			if(nValue==7)
				continue;

			int nFan=get_real_fan_select();
			//marked 20090511
			if(pid_select2[row-1]==2 || pid_select2[row-1]==3)//min(pid1,pid2) or max(pid1,pid2)
				tstatval = multi_register_value[138+4 * 7 + pos];/////////////////////fan auto
			else
				tstatval = multi_register_value[138+nFan * 7 + pos];
			CString str;
			{    //被选中Pid2，Pid1变灰
				if(digital2string((tstatval >> (row-1)) & 0x01,str,FAN))
					if(pid_select2[row-1]==1)
						FLEX_GRID1_PUT_COLOR_STR(row,col,str)
						//col +1//变灰不显示
					else
						FLEX_GRID1_PUT_STR(row,col,str)
						//col +1
			}

		}
	}
//以前只显示了前三排的数据
#if 1 //test
	if(m_nmoduleType == 1 || m_nmoduleType == 3)
	{
		if(m_nmoduleType == 1)
			totalrows = 5 ;//********************************************
		else
			totalrows = 7 ;		
		for(int col = 1 ;col <=(m_PID1_heat_stages+m_PID1_cool_stages+1);col++)
		{
			if(col < (m_PID1_heat_stages+1))
				pos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - col ;
			else
				pos = col - (m_PID1_heat_stages+1);
			//
			//tstatval = multi_register_value[173+ pos];
			
			//////////////////////////////////////////////

//这是原代码。
// 			if(m_fan.GetCurSel()==0&&(m_nmoduleType==1||m_nmoduleType==3))//a,d,g 3就是tstat5g
// 				tstatval = multi_register_value[351+ pos];
// 			else 
// 				tstatval = multi_register_value[173+ pos];	
// 		    	351	1	Analog output OFF table, coasting mode
// 				352	1	Analog output OFF table, cooling1 mode
// 				353	1	Analog output OFF table, cooling2 mode
// 				354	1	Analog output OFF table, cooling3 mode
// 				355	1	Analog output OFF table, heating1 mode
// 				356	1	Analog output OFF table, heating2 mode
// 				357	1	Analog output OFF table, heating3 mode


			//if (multi_register_value[7] == 18)//2.5.0.98
			//{
			//	if(m_fan.GetCurSel()==0/*&&(m_nmoduleType==1||m_nmoduleType==3)*/)//a,d,g 3就是tstat5g
			//	{
			//		 
			//		tstatval = multi_register_value[351+ pos];//2.5.0.99
			//	}
			// 
			//	else
			//		tstatval = multi_register_value[173+ pos];	//2.5.0.99
			//}else
			//{
		//if(m_fan.GetCurSel()==0&&(m_nmoduleType==1||m_nmoduleType==3))//a,d,g 3就是tstat5g
			if(m_fan.GetCurSel()==0/*&&m_nmoduleType==1*/)//a,d,g 3就是tstat5g//2.5.0.99
			/*{*/
					tstatval = multi_register_value[351+ pos];
			/*
			}*/
 			else
				   tstatval = multi_register_value[173+ pos];	
			/*}*/
	


			for(row=totalrows-1;row<=totalrows;row++)//row value
			{
				if (row==4&&m_bOut4PWM)
				{
					continue;
				}
				if (row==5&&m_bOut5PWM)
				{
					continue;
				}
				if(m_bFloat&&row==4)
					continue;
				if(m_bFloat&&row==5)
					continue;
// 	186	1	Ou1 - Output1 Scale - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
// 	187	1	Ou2 - Output2 Scale - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 

				/*if(multi_register_value[186] == 0&&row==6)
					continue;
				if(multi_register_value[187] == 0&&row==7)
					continue;*/
				int nOutReg;	


					nOutReg=286+row-1;
					nValue=multi_register_value[nOutReg];

					if(nValue==7)
						continue;
				CString str;
				//if(/*tstatval>63*/TRUE)//
				//{
			
					if(row==(totalrows-1))//倒数第二行
					{
					if (multi_register_value[186]!=0)
						{
						  if (tstatval & 64)
						  {
						  if(digital2string(4,str,VALVE))//for 7 or 8 bit
							  if(pid_select2[row-1]==1)
								  FLEX_GRID1_PUT_COLOR_STR(row,col,str)//col +1
							  else
							  FLEX_GRID1_PUT_STR(row,col,str)//col +1
						  } 
						  else
						  {
						    if(digital2string(tstatval & 0x03,str,VALVE))//*** value
							{
								if(pid_select2[row-1]==1)
								FLEX_GRID1_PUT_COLOR_STR(row,col, str)//col +1
								else
								FLEX_GRID1_PUT_STR(row,col, str)//col +1
							}
						  }
						}
					else
					{
					   int sel=tstatval & 0x03;
							 
							if (sel!=0)
							{
							if(digital2string(1,str,FAN))//*** value
							{
								if(pid_select2[row-1]==1)
								FLEX_GRID1_PUT_COLOR_STR(row,col, str)//col +1
								else
								FLEX_GRID1_PUT_STR(row,col,str)//col +1
							}
							} 
							else
							{
							if(digital2string(0,str,FAN))//*** value
							{
								if(pid_select2[row-1]==1)
								FLEX_GRID1_PUT_COLOR_STR(row,col, str)//col +1
								else
								FLEX_GRID1_PUT_STR(row,col,str)//col +1
							}
					       }
				 
					}
					}
					if(row==totalrows)//倒数第一个
					{
					if (multi_register_value[187]!=0)
						{
						if(tstatval & 128)
							{
							if(digital2string(4,str,VALVE))//for 7 or 8 bit
								{
								if(pid_select2[row-1]==1)
									FLEX_GRID1_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID1_PUT_STR(row,col,str)//col +1
								}

							}else
							{
							if(digital2string((tstatval >> 2) & 0x03,str,VALVE))//*** value
							{
								if(pid_select2[row-1]==1)
								FLEX_GRID1_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID1_PUT_STR(row,col,str)//col +1
							}
							}
						}

					else
						{
						int sel=(tstatval >> 2) & 0x03;
							if (sel!=0)
							{
							if(digital2string(1/*(tstatval >> 2) & 0x03*/,str,FAN))//*** value
							{
								if(pid_select2[row-1]==1)
								FLEX_GRID1_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID1_PUT_STR(row,col,str)//col +1
							}
							} 
							else
							{
							if(digital2string(0/*(tstatval >> 2) & 0x03*/,str,FAN))//*** value
							{
								if(pid_select2[row-1]==1)
								FLEX_GRID1_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID1_PUT_STR(row,col,str)//col +1
							}
							}
						}
					 
				}
				 
				//}
				//else
				//{
				///*
				//	if(digital2string((tstatval >> ((row-totalrows+1)*2)) & 0x03,str,VALVE))//*** value
				//		if(pid_select2[row-1]==1)
				//		FLEX_GRID1_PUT_COLOR_STR(row,col,str)//col +1
				//		else
				//		FLEX_GRID1_PUT_STR(row,col,str)//col +1
				//		*/
				//}

			}
		}
	}

// 	output4_value =multi_register_value[283]; // 1 表示floating
// 	output5_value =multi_register_value[284];
	if (multi_register_value[7] == 18) // 2.5.0.98
	{
		if(multi_register_value[283] == 1)
		{
			for(int col = 1 ;col <=(m_PID1_heat_stages+m_PID1_cool_stages+1);col++)
			{
				int nValue=0;
				if(col < (m_PID1_heat_stages+1))
					pos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - col ;
				else
					pos = col - (m_PID1_heat_stages+1);	

				if(m_fan.GetCurSel()==0)
					nValue = multi_register_value[351+ pos];
				else
					nValue = multi_register_value[173+ pos];
				int indext=-1;

				nValue=(nValue& 0x03)>>4;
				switch(nValue)
				{
				case 0:
					strTemp="Close";
					break;
				case 1:
					strTemp="Open";
					break;
				case 2:
					strTemp="0-100";
					break;
				case 3:
					strTemp="50-100";
					break;
				case 4:
					strTemp="0-50";
					break;
				default:
					strTemp="";
					break;
				}
				if(pid_select2[4-1]==1)
				{
					FLEX_GRID1_PUT_COLOR_STR(4,col,strTemp)//col +1
			

				}
				else
				{
					FLEX_GRID1_PUT_STR(4,col,strTemp)//col +1
	
				}


				//FLEX_GRID1_PUT_STR(4,col,strTemp)//col +1

				//	FLEX_GRID1_PUT_STR(5,col,strTemp)//col +1


			}
		}
		if(multi_register_value[284] == 1)
		{
			for(int col = 1 ;col <=(m_PID1_heat_stages+m_PID1_cool_stages+1);col++)
			{
				int nValue=0;
				if(col < (m_PID1_heat_stages+1))
					pos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - col ;
				else
					pos = col - (m_PID1_heat_stages+1);	


				if(m_fan.GetCurSel()==0)
					nValue = multi_register_value[351+ pos];
				else
					nValue = multi_register_value[173+ pos];
				int indext=-1;

				nValue=(nValue)>>4;
				switch(nValue)
					{
				case 0:
					strTemp="Close";
					break;
				case 1:
					strTemp="Open";
					break;
				case 2:
					strTemp="0-100";
					break;
				case 3:
					strTemp="50-100";
					break;
				case 4:
					strTemp="0-50";
					break;
				default:
					strTemp="";
					break;
					}
				if(pid_select2[5-1]==1)
				{
						FLEX_GRID1_PUT_COLOR_STR(5,col,strTemp)

				}
				else
				{
						FLEX_GRID1_PUT_STR(5,col,strTemp)//col +1
				}


 


			}
		}
	}
	else
	{
		if(m_bFloat)
		{
			for(int col = 1 ;col <=(m_PID1_heat_stages+m_PID1_cool_stages+1);col++)
			{
				int nValue=0;
				if(col < (m_PID1_heat_stages+1))
					pos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - col ;
				else
					pos = col - (m_PID1_heat_stages+1);	


				if(m_fan.GetCurSel()==0)
					nValue = multi_register_value[351+ pos];
				else
					nValue = multi_register_value[173+ pos];
				int indext=-1;

				nValue=(nValue& 0x03)>>4;
				switch(nValue)
				{
				case 0:
					strTemp="Close";
					break;
				case 1:
					strTemp="Open";
					break;
				case 2:
					strTemp="0-100";
					break;
				case 3:
					strTemp="50-100";
					break;
				case 4:
					strTemp="0-50";
					break;
				default:
					strTemp="";
					break;
				}
				if(pid_select2[4-1]==1)
				{
					FLEX_GRID1_PUT_COLOR_STR(4,col,strTemp)//col +1
					FLEX_GRID1_PUT_COLOR_STR(5,col,strTemp)

				}
				else
				{
					FLEX_GRID1_PUT_STR(4,col,strTemp)//col +1
					FLEX_GRID1_PUT_STR(5,col,strTemp)//col +1
				}


				//FLEX_GRID1_PUT_STR(4,col,strTemp)//col +1

				//	FLEX_GRID1_PUT_STR(5,col,strTemp)//col +1


			}
		}

	}
	if(m_bOut4PWM) // 2表示PWM
	{

			for(int col = 1 ;col <=(m_PID1_heat_stages+m_PID1_cool_stages+1);col++)
			{
				int nValue=0;
				if(col < (m_PID1_heat_stages+1))
					pos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - col ;
				else
					pos = col - (m_PID1_heat_stages+1);			
				tstatval = multi_register_value[173+ pos];
				int indext=-1;
				//nValue=read_one(tstat_id,341+pos);
				nValue=multi_register_value[341+pos];
				nValue=nValue>>4;
				switch(nValue)
				{
				case 0:
					strTemp=_T("Close");
					break;
				case 1:
					strTemp=_T("Open");
					break;
				case 2:
					strTemp="0-100";
					break;
				case 3:
					strTemp=_T("50-100");
					break;
				case 4:
					strTemp=_T("0-50");
					break;
				default:
					strTemp=_T("");
				}
			if(pid_select2[4-1]==1)//2.5.0.98  1表示pid2
				FLEX_GRID1_PUT_COLOR_STR(4,col,strTemp)
				else
				FLEX_GRID1_PUT_STR(4,col,strTemp)//col +1
			}
	}	
	if(m_bOut5PWM)
		{
			for(int col = 1 ;col <=(m_PID1_heat_stages+m_PID1_cool_stages+1);col++)
			{
				int nValue=0;
				if(col < (m_PID1_heat_stages+1))
					pos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - col ;
				else
					pos = col - (m_PID1_heat_stages+1);			
				tstatval = multi_register_value[173+ pos];
				int indext=-1;

				//nValue=read_one(tstat_id,341+pos);	
				nValue=multi_register_value[341+pos];
				nValue=nValue&0x0f;
				switch(nValue)
				{
				case 0:
					strTemp=_T("Close");
					break;
				case 1:
					strTemp=_T("Open");
					break;
				case 2:
					strTemp=_T("0-100");
					break;
				case 3:
					strTemp=_T("50-100");
					break;
				case 4:
					strTemp=_T("0-50");
					break;
				default:
					strTemp=_T("");
				}

				if(pid_select2[5-1]==1)//2.5.0.98
				FLEX_GRID1_PUT_COLOR_STR(5,col,strTemp)	
				else
				FLEX_GRID1_PUT_STR(5,col,strTemp)//col +1

				//FLEX_GRID1_PUT_STR(5,col,strTemp)//col +1//2.5.0.98
					//totalrows
			}
		}


	for( row = 1;row<=totalrows;row++)//****************************
		//for( row = totalrows-1;row<=totalrows;row++)//****************************
		{
			int nOutReg;
			nOutReg=286+row-1;
//			int nValue=read_one(tstat_id,nOutReg);
			int nValue=multi_register_value[nOutReg];
			if(nValue!=7)
					continue;
			for(int col = 1 ;col <=(m_PID1_heat_stages+m_PID1_cool_stages+1);col++)
			{
				int nValue=0;
				if(col < (m_PID1_heat_stages+1))
					pos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - col ;
				else
					pos = col - (m_PID1_heat_stages+1);			
				int indext=-1;

				int nReg=362+pos;
				nValue=multi_register_value[nReg]; 

				if(m_fan.GetCurSel()==0)
				{
					nValue=(nValue&0xF0)>>4;
					switch(nValue)
					{
					case 0:
						strTemp=_T("0%");
						break;
					case 1:
						strTemp=_T("100%");
						break;
					case 2:
						strTemp=_T("Min->100%");
						break;
					case 3:
						strTemp=_T("Min|100%");
						break;
					case 4:
						strTemp=_T("Min");
						break;
					default:
						strTemp=_T("0%");
					}
			 
					FLEX_GRID1_PUT_STR(row,col,strTemp);//col +1
				}
				else
				{
					nValue=(nValue&0x0F);
					switch(nValue)
					{
					case 0:
						strTemp=_T("0%");
						break;
					case 1:
						strTemp=_T("100%");
						break;
					case 2:
						strTemp=_T("Min->100%");
						break;
					case 3:
						strTemp=_T("Min|100%");
						break;
					case 4:
						strTemp=_T("Min");
						break;
					default:
						strTemp=_T("0%");
					}
				//	FLEX_GRID1_PUT_STR(row,col,strTemp)//col +1
					FLEX_GRID1_PUT_STR(row,col,strTemp);//col +1
					FLEX_GRID1_PUT_STR(totalrows,col,strTemp);//col +1

				}
		}		
	}
#endif

}


void COutPutDlg::FreshGrid_PID2()
{
	CString strTemp;
	m_PID2_heat_stages=multi_register_value[268];//276 register ,heat stages
	m_PID2_cool_stages=multi_register_value[269];//277 register ,cool stages
	if(m_PID2_heat_stages==0&&m_PID2_cool_stages==0)
	{
		//write_one(g_tstat_id,268,3);
		//write_one(g_tstat_id,269,3);
		m_PID2_heat_stages=3;
		m_PID2_cool_stages=3;
		if(m_PID2_heat_stages>6)
			m_PID2_heat_stages=6;
		if(m_PID2_cool_stages>6)
			m_PID2_cool_stages=6;
	}
	strTemp.Format(_T("%d"),m_PID2_heat_stages);
	m_PID2_heatEdit.SetWindowText(strTemp);
	strTemp.Format(_T("%d"),m_PID2_cool_stages);
	m_PID2_coolEdit.SetWindowText(strTemp);

	m_FlexGrid2.put_Rows(8);
	m_FlexGrid2.put_Cols(m_PID2_heat_stages+m_PID2_cool_stages+7);//3 because 1/pid;2/out1;3/coast 4/interlock
	//*****************************************
	m_FlexGrid2.put_TextMatrix(0,1,_T("Description"));
	m_FlexGrid2.put_TextMatrix(0,2,_T("Function"));
	m_FlexGrid2.put_TextMatrix(0,3,_T("Rotation"));
	m_FlexGrid2.put_ColWidth(3,0);//隐藏Rotation
	m_FlexGrid2.put_TextMatrix(0,4,_T("Control"));
	m_FlexGrid2.put_TextMatrix(0,5,_T("Interlock"));
	int i=0;
	for(i=0;i<m_PID2_heat_stages;i++)
		m_FlexGrid2.put_TextMatrix(0,6+i,heat_hand[m_PID2_heat_stages-i-1]);
	m_FlexGrid2.put_TextMatrix(0,m_PID2_heat_stages+6,_T("Coast"));
	for(i=0;i<m_PID2_cool_stages;i++)
		m_FlexGrid2.put_TextMatrix(0,m_PID2_heat_stages+7+i,cool_hand[i]);


	m_FlexGrid2.put_TextMatrix(1,1,g_strOutName1);
	m_FlexGrid2.put_TextMatrix(2,1,g_strOutName2);
	m_FlexGrid2.put_TextMatrix(3,1,g_strOutName3);
	m_FlexGrid2.put_TextMatrix(4,1,g_strOutName4);
	m_FlexGrid2.put_TextMatrix(5,1,g_strOutName5);
	m_FlexGrid2.put_TextMatrix(6,1,g_strOutName6);
	m_FlexGrid2.put_TextMatrix(7,1,g_strOutName7);

	m_FlexGrid2.put_TextMatrix(1,0,_T("1"));
	m_FlexGrid2.put_TextMatrix(2,0,_T("2"));
	m_FlexGrid2.put_TextMatrix(3,0,_T("3"));
	m_FlexGrid2.put_TextMatrix(4,0,_T("4"));
	m_FlexGrid2.put_TextMatrix(5,0,_T("5"));
	m_FlexGrid2.put_TextMatrix(6,0,_T("6"));
	m_FlexGrid2.put_TextMatrix(7,0,_T("7"));

	m_FlexGrid2.put_ColAlignment(0,4);
	m_FlexGrid2.put_ColAlignment(1,4);
	//*****************************888、
	int totalrows,row;
	unsigned short tstatval,pos;

	if(m_nmoduleType == 3)//5d
		m_FlexGrid2.put_Rows(8);
	else
		m_FlexGrid2.put_Rows(6);
	if(m_nmoduleType == 1 )//5a
		totalrows = 3;
	else 
		totalrows = 5;
	if (m_nmoduleType == 3) //2.5.0.99
	{
		totalrows = 7;
	}
	
	int pid_select2[7]={0};
	if (m_nmoduleType==1)
	{
		for(i=0;i<3;i++)
			pid_select2[i]=multi_register_value[247+i];/////////////////////////////get pid select ;col one 1

		pid_select2[3]=multi_register_value[252];
		pid_select2[4]=multi_register_value[253];
		//
	}
	else
	{
		for(i=0;i<7;i++)
			pid_select2[i]=multi_register_value[247+i];/////////////////////////////get pid select ;col one 1
	}
	////////////////////////down code is for m_flexgrid2
	for(int col = 1 ;col <= (m_PID2_heat_stages+m_PID2_heat_stages+1);col++)
	{
		if(col < (m_PID2_heat_stages+1))
			pos = (m_PID2_cool_stages+m_PID2_heat_stages+1) - col ;
		else
			pos = col - (m_PID2_heat_stages+1);

		if(m_fan.GetCurSel()==0&&(m_nmoduleType==1||m_nmoduleType==3))//min(pid1,pid2) or max(pid1,pid2)
			tstatval = multi_register_value[526+pos];/////////////////////fan auto
		else
			tstatval = multi_register_value[254+ pos];
		for( row = 1;row<=totalrows;row++)//****************************
		{
			CString str;
			{
				if(digital2string((tstatval >> (row-1)) & 0x01,str,FAN))
						if(pid_select2[row-1]==0)
							FLEX_GRID2_PUT_COLOR_STR(row,col,str)//col +1
						else
							FLEX_GRID2_PUT_STR(row,col,str)//col +1
			}
		}
	}//end if
	if(m_nmoduleType == 1 || m_nmoduleType == 3) //3就是TSTAT5G
	{
		if(m_nmoduleType == 1)
			totalrows = 5 ;//********************************************
		else
			totalrows = 7 ;		
		for(int col = 1 ;col <=(m_PID2_cool_stages+m_PID2_heat_stages+1);col++)
		{
			if(col < (m_PID2_heat_stages+1))
				pos = (m_PID2_cool_stages+m_PID2_heat_stages+1) - col ;//heat handles position
			else
				pos = col - (m_PID2_heat_stages+1);			//cool handles position

//2.5.0.95
			if(m_fan.GetCurSel()==0&&(m_nmoduleType==1||m_nmoduleType==3))//a,d,g 3就是tstat5g
			{
				// 				if (pid_select2[0] == 1)//只有第一行选择pid2，下面都是以PID2来计算  //PID1 = 0 PID2 =1
				// 					tstatval = multi_register_value[385+ pos];
				// 				else
				tstatval = multi_register_value[533+ pos];
			}
			// 			else if (pid_select2[0] == 1)
			// 				tstatval = multi_register_value[254+ pos];	
			else
			tstatval = multi_register_value[261+ pos];	


//			tstatval = multi_register_value[261+ pos];//2.5.0.95
// 			if (multi_register_value[7] == 18)//2.5.0.93//2.5.0.99
// 			{
// 				tstatval = multi_register_value[392+ pos];
// 			}

			for(row=totalrows-1;row<=totalrows;row++)//row value
			{

				if (row==4&&m_bOut4PWM)//2.5.0.99
				{
					continue;
				}
				if (row==5&&m_bOut5PWM)//2.5.0.99
				{
					continue;
				}
				if(m_bFloat&&row==4)//2.5.0.99
					continue;
				if(m_bFloat&&row==5)//2.5.0.99
					continue;
				// 	186	1	Ou1 - Output1 Scale - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 
				// 	187	1	Ou2 - Output2 Scale - 0=On/Off, 1=0-10V, 2=0-5V, 3=2-10V, 4= 10-0V 

				if(multi_register_value[186] == 0&&row==6)//2.5.0.99
					continue;
				if(multi_register_value[187] == 0&&row==7)//2.5.0.99
					continue;



				CString str;
				if( tstatval>63)
					{
					#if 1
if(row==(totalrows-1))
						{
							if(tstatval & 64)
							{
								if(digital2string(4,str,VALVE))//for 7 or 8 bit
								if(pid_select2[row-1]==0)
									FLEX_GRID2_PUT_COLOR_STR(totalrows-1,col,str)//col +1
								else
									FLEX_GRID2_PUT_STR(totalrows-1,col,str)//col +1
							}
							else
							{
								
							  if(digital2string(tstatval& 0x03,str,VALVE))//*** value
								if(pid_select2[row-1]==0)
									FLEX_GRID2_PUT_COLOR_STR(row,col,str)//col +1
								else
									FLEX_GRID2_PUT_STR(row,col,str)//col +1
							}
						}
						if(row==totalrows)
						{
							if(tstatval & 128)
							{
								if(digital2string(4,str,VALVE))//for 7 or 8 bit
									if(pid_select2[row-1]==0)
										FLEX_GRID2_PUT_COLOR_STR(totalrows,col,str)//col +1
									else
										FLEX_GRID2_PUT_STR(totalrows,col,str)//col +1
							}
							else
							{
								if(digital2string((tstatval >> 2) & 0x03,str,VALVE))//*** value
									if(pid_select2[row-1]==0)
										FLEX_GRID2_PUT_COLOR_STR(row,col,str)//col +1
									else
										FLEX_GRID2_PUT_STR(row,col,str)//col +1
							}

						}
                    #endif
					#if 0
					if(row==(totalrows-1))//倒数第二行
					{
					if (multi_register_value[186]!=0)
						{
						  if (tstatval & 64)
						  {
						  if(digital2string(4,str,VALVE))//for 7 or 8 bit
							  if(pid_select2[row-1]==1)
								  FLEX_GRID1_PUT_COLOR_STR(row,col,str)//col +1
							  else
							  FLEX_GRID1_PUT_STR(row,col,str)//col +1
						  } 
						  else
						  {
						    if(digital2string(tstatval & 0x03,str,VALVE))//*** value
							{
								if(pid_select2[row-1]==1)
								FLEX_GRID1_PUT_COLOR_STR(row,col, str)//col +1
								else
								FLEX_GRID1_PUT_STR(row,col, str)//col +1
							}
						  }
					 }
					else
					{
					   int sel=tstatval & 0x03;
							 
							if (sel!=0)
							{
							if(digital2string(1,str,FAN))//*** value
							{
								if(pid_select2[row-1]==1)
								FLEX_GRID1_PUT_COLOR_STR(row,col, str)//col +1
								else
								FLEX_GRID1_PUT_STR(row,col,str)//col +1
							}
							} 
							else
							{
							if(digital2string(0,str,FAN))//*** value
							{
								if(pid_select2[row-1]==1)
								FLEX_GRID1_PUT_COLOR_STR(row,col, str)//col +1
								else
								FLEX_GRID1_PUT_STR(row,col,str)//col +1
							}
					       }
				 
					}
					}
					if(row==totalrows)//倒数第一个
					{
					if (multi_register_value[187]!=0)
						{
						if(tstatval & 128)
							{
							if(digital2string(4,str,VALVE))//for 7 or 8 bit
								{
								if(pid_select2[row-1]==1)
									FLEX_GRID1_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID1_PUT_STR(row,col,str)//col +1
								}

							}else
							{
							if(digital2string((tstatval >> 2) & 0x03,str,VALVE))//*** value
							{
								if(pid_select2[row-1]==1)
								FLEX_GRID1_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID1_PUT_STR(row,col,str)//col +1
							}
							}
						}

					else
						{
						int sel=(tstatval >> 2) & 0x03;
							if (sel!=0)
							{
							if(digital2string(1/*(tstatval >> 2) & 0x03*/,str,FAN))//*** value
							{
								if(pid_select2[row-1]==1)
								FLEX_GRID1_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID1_PUT_STR(row,col,str)//col +1
							}
							} 
							else
							{
							if(digital2string(0/*(tstatval >> 2) & 0x03*/,str,FAN))//*** value
							{
								if(pid_select2[row-1]==1)
								FLEX_GRID1_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID1_PUT_STR(row,col,str)//col +1
							}
							}
						}
					 
				}
#endif
						
					}
					else
					{					
						BOOL b=digital2string((tstatval >> ((row-totalrows+1)*2)) & 0x03,str,VALVE);
						if( b)//*** value
								if(pid_select2[row-1]==0)
									FLEX_GRID2_PUT_COLOR_STR(row,col,str)//col +1
								else
									FLEX_GRID2_PUT_STR(row,col,str)//col +1
					}
				}
			}
		}//end if 	
	

	int  nFunction=multi_register_value[328];
	int nRotation;
	for (int k=0; k<4; k++)
	{
		nRotation=multi_register_value[329+k];
		strTemp.Format(_T("%d"),nRotation);
		m_FlexGrid2.put_TextMatrix(k+2,3,strTemp);
		//ntemp=1<<k;
		//kkkk=nFunction& ntemp;

		if ((nFunction& 1<<k)>0)
		{
			m_FlexGrid2.put_TextMatrix(k+2,2,_T("Rotation"));
		}
		else//==0
		{
			m_FlexGrid2.put_TextMatrix(k+2,2,_T("Normal"));
		}
	}





	unsigned short interlock[7]={0};
	for(int i=0;i<7;i++)
	{
		interlock[i]=multi_register_value[286+i];
	}
//	Read_Multi(tstat_id,interlock,286,7);

	CString str;
	TCHAR *interlock_str[]={_T("On"),_T("DI1"),_T("AI1"),_T("AI2"),_T("Timer Or"),_T("Timer And"),_T("Interlock Timer"),_T("FreeCool")};

	if(product_register_value[MODBUS_ANALOG_IN1]!=3)//on/off mode
		interlock_str[2]=_T("//AI1");
	if(multi_register_value[189]!=3)//on/off mode
		interlock_str[3]=_T("//AI2");

	for(i=0;i<totalrows;i++)
	{		

		switch(interlock[i])
		{
		//case 0:m_FlexGrid.put_TextMatrix(i+1,5,interlock_str[0]);m_FlexGrid2.put_TextMatrix(i+1,5,interlock_str[0]);break;
		case 0:m_FlexGrid1.put_TextMatrix(i+1,5,_T("      -"));m_FlexGrid2.put_TextMatrix(i+1,5,_T("      -"));break;
		case 1:m_FlexGrid1.put_TextMatrix(i+1,5,interlock_str[1]);m_FlexGrid2.put_TextMatrix(i+1,5,interlock_str[1]);break;
		case 2:m_FlexGrid1.put_TextMatrix(i+1,5,interlock_str[2]);m_FlexGrid2.put_TextMatrix(i+1,5,interlock_str[2]);break;
		case 3:m_FlexGrid1.put_TextMatrix(i+1,5,interlock_str[3]);m_FlexGrid2.put_TextMatrix(i+1,5,interlock_str[3]);break;
		case 4:m_FlexGrid1.put_TextMatrix(i+1,5,interlock_str[4]);m_FlexGrid2.put_TextMatrix(i+1,5,interlock_str[4]);break;
		case 5:m_FlexGrid1.put_TextMatrix(i+1,5,interlock_str[5]);m_FlexGrid2.put_TextMatrix(i+1,5,interlock_str[5]);break;
		case 6:m_FlexGrid1.put_TextMatrix(i+1,5,interlock_str[6]);m_FlexGrid2.put_TextMatrix(i+1,5,interlock_str[6]);break;
		case 7:m_FlexGrid1.put_TextMatrix(i+1,5,interlock_str[7]);m_FlexGrid2.put_TextMatrix(i+1,5,interlock_str[7]);break;

		default:m_FlexGrid1.put_TextMatrix(i+1,5,interlock_str[0]);m_FlexGrid2.put_TextMatrix(i+1,5,interlock_str[0]);break;
		}
			switch(pid_select2[i])
			{
			case 0:str = _T("PID1");break;
			case 1:str = _T("PID2");break;
			case 2:str = _T("MAX(PID1,PID2)");break;
			case 3:str = _T("MIN(PID1,PID2)");break;
			default:str = _T("PID1");break;
			}
 			m_FlexGrid1.put_TextMatrix(i+1,4,str);
 			m_FlexGrid2.put_TextMatrix(i+1,4,str);		
	}


#if 1
		for( row = 1;row<=totalrows;row++)//row value
		{  
			if(pid_select2[row-1]==2)
			{		
					if(i_104_pid1>i_268_pid2)
					{
						for(int col = 6 ;col <=(m_PID1_heat_stages+m_PID1_cool_stages+1)+5;col++)
						{
							m_FlexGrid1.put_Row(row);
							m_FlexGrid1.put_Col(col);
							m_FlexGrid1.put_CellForeColor(FLEXGRID_CELL_COLOR);
						}
						for(int col =6 ;col <=(m_PID2_cool_stages+m_PID2_heat_stages+1)+5;col++)
						{
							m_FlexGrid2.put_Row(row);
							m_FlexGrid2.put_Col(col);
							m_FlexGrid2.put_CellBackColor(FLEXGRID_CELL_COLOR);
						}
					}
					else//i_104_pid1<=i_268_pid2
					{
						for(int col = 6 ;col <=(m_PID1_heat_stages+m_PID1_cool_stages+1)+5;col++)
						{
							m_FlexGrid1.put_Row(row);
							m_FlexGrid1.put_Col(col);
							m_FlexGrid1.put_CellBackColor(FLEXGRID_CELL_COLOR);
			
						}
						for(int col = 6 ;col <=(m_PID2_cool_stages+m_PID2_heat_stages+1)+5;col++)
						{

							m_FlexGrid2.put_Row(row);
							m_FlexGrid2.put_Col(col);
							m_FlexGrid2.put_CellForeColor(FLEXGRID_CELL_COLOR);
						}

					}
				}
		}
#endif


		//2.5.0.98以下
		if(m_bOut4PWM)
		{

			for(int col = 1 ;col <=(m_PID2_heat_stages+m_PID2_cool_stages+1);col++)
			{
				int nValue=0;
				if(col < (m_PID2_heat_stages+1))
					pos = (m_PID2_heat_stages+m_PID2_cool_stages+1) - col ;
				else
					pos = col - (m_PID2_heat_stages+1);			
				tstatval = multi_register_value[173+ pos];
				int indext=-1;
				//nValue=read_one(tstat_id,341+pos);
				nValue=multi_register_value[341+pos];
				nValue=nValue>>4;
				switch(nValue)
				{
				case 0:
					strTemp=_T("Close");
					break;
				case 1:
					strTemp=_T("Open");
					break;
				case 2:
					strTemp="0-100";
					break;
				case 3:
					strTemp=_T("50-100");
					break;
				case 4:
					strTemp=_T("0-50");
					break;
				default:
					strTemp=_T("");
				}
				if(pid_select2[4-1]==1)//2.5.0.98  1表示pid2
				FLEX_GRID2_PUT_STR(4,col,strTemp)//col +1	
				else
				FLEX_GRID2_PUT_COLOR_STR(4,col,strTemp)
			}
		}	
		if(m_bOut5PWM)
		{
			for(int col = 1 ;col <=(m_PID2_heat_stages+m_PID2_cool_stages+1);col++)
			{
				int nValue=0;
				if(col < (m_PID2_heat_stages+1))
					pos = (m_PID2_heat_stages+m_PID2_cool_stages+1) - col ;
				else
					pos = col - (m_PID2_heat_stages+1);			
				tstatval = multi_register_value[173+ pos];
				int indext=-1;

				//nValue=read_one(tstat_id,341+pos);	
				nValue=multi_register_value[341+pos];
				nValue=nValue&0x0f;
				switch(nValue)
				{
				case 0:
					strTemp=_T("Close");
					break;
				case 1:
					strTemp=_T("Open");
					break;
				case 2:
					strTemp=_T("0-100");
					break;
				case 3:
					strTemp=_T("50-100");
					break;
				case 4:
					strTemp=_T("0-50");
					break;
				default:
					strTemp=_T("");
				}

				if(pid_select2[5-1]==1)//2.5.0.98
					FLEX_GRID2_PUT_STR(5,col,strTemp)//col +1		
				else
					FLEX_GRID2_PUT_COLOR_STR(5,col,strTemp)

				//FLEX_GRID1_PUT_STR(5,col,strTemp)//col +1//2.5.0.98
				//totalrows
			}
		}


}
void COutPutDlg::OnBnClickedFanautocheck()
{
	if(g_OutPutLevel==1)
		return;
	// TODO: Add your control notification handler code here

	//129	107	1	Low byte	W/R	"AUTO_ONLY , enables or disables manual mode. 0 = Manual Fan Modes 1-x Allowed 
	//(depending on R122 value, 1 = Auto Mode Only, 2 = DDC mode,the user can not change setpoint and fan speed from keypad."


//	m_bFanAutoOnly =(BOOL)m_fanAutoCheck.GetCheck();
//	write_one(g_tstat_id,MODBUS_AUTO_ONLY,m_bFanAutoOnly);//129  107
//	product_register_value[MODBUS_AUTO_ONLY]=m_bFanAutoOnly;
	//----------------------------------------------------------
	//Recode by Fance ,2013 04 08.
	if(m_fanAutoCheck.GetCheck()==BST_CHECKED)
	{
		m_bFanAutoOnly=TRUE;
		//m_fan_mode_ctrl.EnableWindow(FALSE);
		//if (newtstat6[7] == PM_TSTAT6)
		if ((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7))
		{
			write_one(g_tstat_id,MODBUS_AUTO_ONLY,1);
			product_register_value[MODBUS_AUTO_ONLY]=1;
		}else
			write_one(g_tstat_id,MODBUS_AUTO_ONLY,1);
	}
	else
	{
		m_bFanAutoOnly=FALSE;
	//	m_fan_mode_ctrl.EnableWindow(TRUE);
		//write_one(g_tstat_id,129,0);

		//if (newtstat6[7] == PM_TSTAT6)
		if ((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7))
		{
			write_one(g_tstat_id,MODBUS_AUTO_ONLY,0);
			product_register_value[MODBUS_AUTO_ONLY]=0;
		}else
			write_one(g_tstat_id,MODBUS_AUTO_ONLY,0);

	}

	//write_one(g_tstat_id,129,m_bFanAutoOnly);

	if (product_register_value[7] ==PM_TSTAT6)
	{
		if(product_register_value[107]==0)
			m_bFanAutoOnly=FALSE;
		if(product_register_value[107]==1)
			m_bFanAutoOnly=TRUE;
	}else
	{
		if(multi_register_value[129]==0)
			m_bFanAutoOnly=FALSE;
		if(multi_register_value[129]==1)
			m_bFanAutoOnly=TRUE;
	}
	//----------------------------------------------------------

	put_fan_variable();
	/*
	if(g_ifanStatus<m_fan.GetCount())
		m_fan.SetCurSel(g_ifanStatus);
		*/
	if(!g_strFan.IsEmpty())
	{
		CString strLbText;
		for(int i=0;i<m_fan.GetCount();i++)
		{
			m_fan.GetLBText(i,strLbText);
			if(strLbText.CompareNoCase(g_strFan)==0)
			{
				m_fan.SetCurSel(i);
				break;
			}
		}
			
	}

	FreshGrids();
}
BEGIN_EVENTSINK_MAP(COutPutDlg, CDialog)
	ON_EVENT(COutPutDlg, IDC_MSFLEXGRID1, DISPID_CLICK, COutPutDlg::ClickMsflexgrid1, VTS_NONE)
	ON_EVENT(COutPutDlg, IDC_MSFLEXGRID2, DISPID_CLICK, COutPutDlg::ClickMsflexgrid2, VTS_NONE)
END_EVENTSINK_MAP()

void COutPutDlg::ClickMsflexgrid1()
{
	if(g_OutPutLevel==1)
		return;
	//click grid 1 to select item.
	m_bflexgrid1_or_2=FALSE;
	long lRow,lCol;
	lRow = m_FlexGrid1.get_RowSel();//获取点击的行号	
	pid1lrow = lRow;//2.5.0.96 
	lCol = m_FlexGrid1.get_ColSel(); //获取点击的列号
	if((lCol==1 || lCol==2) && get_curtstat_version()<26)
		return;
	if(lRow>m_FlexGrid1.get_Rows()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow == 0) //如果点击标题行，也无效
		return;
	if(FLEXGRID_CELL_COLOR==m_FlexGrid1.get_CellBackColor())
		return;
	CRect rect;
	m_FlexGrid1.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	// MSFlexGrid控件的函数的长度单位是"缇(twips)"，
	//需要将其转化为像素，1440缇= 1英寸
	CDC* pDC =GetDC();
	//计算象素点和缇的转换比例
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_FlexGrid1.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_FlexGrid1.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_FlexGrid1.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_FlexGrid1.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rc.OffsetRect(rect.left+1,rect.top+1);
	//获取选中格的文本信息
	CString strValue = m_FlexGrid1.get_TextMatrix(lRow,lCol);
	m_nCurRow=lRow;
	m_nCurCol=lCol;

	
	if (lCol==1)
	{
		/*m_DescriptEdt.MoveWindow(rc);
		m_DescriptEdt.ShowWindow(SW_SHOW);
		m_DescriptEdt.SetFocus();
		m_DescriptEdt.SetWindowText(strValue);
		DWORD dwSel = m_DescriptEdt.GetSel();
		m_DescriptEdt.SetSel(HIWORD(dwSel), -1);*/
	}
	if(lCol==4)
	{
// 		m_ItemValueCombx.ResetContent();
// 		m_ItemValueCombx.AddString(_T("PID1"));
// 		m_ItemValueCombx.AddString(_T("PID2"));
// 		m_ItemValueCombx.AddString(_T("MAX(PID1,PID2)"));
// 		m_ItemValueCombx.AddString(_T("MIN(PID1,PID2)"));
// 		m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
// 		m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
// 		m_ItemValueCombx.BringWindowToTop();
// 		m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
// 		m_ItemValueCombx.SetFocus(); //获取焦点

//0911这样做
		m_ItemValueCombx.ResetContent();
		m_ItemValueCombx.AddString(_T("PID1"));
		m_ItemValueCombx.AddString(_T("PID2"));
		m_ItemValueCombx.AddString(_T("MAX(PID1,PID2)"));
		m_ItemValueCombx.AddString(_T("MIN(PID1,PID2)"));
		m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
		m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_ItemValueCombx.BringWindowToTop();
		m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
		m_ItemValueCombx.SetFocus(); //获取焦点
	}
	 if(lCol==5)
	{
	
			m_ItemValueCombx.ResetContent();
			m_ItemValueCombx.AddString(_T("On"));
			m_ItemValueCombx.AddString(_T("DI1"));
			if (product_register_value[7]== PM_TSTAT6)
			{
				if(product_register_value[MODBUS_ANALOG_IN1]==3)//on/off mode  //找不到对应的tstat6
					m_ItemValueCombx.AddString(_T("AI1"));
				else
					m_ItemValueCombx.AddString(_T("//AI1"));
				if(product_register_value[189]==3)//on/off mode
					m_ItemValueCombx.AddString(_T("AI2"));
				else
					m_ItemValueCombx.AddString(_T("//AI2"));
			}else
			{
				if(product_register_value[MODBUS_ANALOG_IN1]==3)//on/off mode
					m_ItemValueCombx.AddString(_T("AI1"));
				else
					m_ItemValueCombx.AddString(_T("//AI1"));
				if(multi_register_value[189]==3)//on/off mode
					m_ItemValueCombx.AddString(_T("AI2"));
				else
					m_ItemValueCombx.AddString(_T("//AI2"));
			}

			m_ItemValueCombx.AddString(_T("Timer Or"));
			m_ItemValueCombx.AddString(_T("Timer And"));
			m_ItemValueCombx.AddString(_T("InterLock Timer"));
			m_ItemValueCombx.AddString(_T("FreeCool"));
			m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
			m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_ItemValueCombx.BringWindowToTop();
			m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
			m_ItemValueCombx.SetFocus(); //获取焦点
	}


	 if(lCol>5)
	{
		
// 			286	245	1	Low byte	W/R	Interlock for  output1
// 			287	246	1	Low byte	W/R	Interlock for  output2
// 			288	247	1	Low byte	W/R	Interlock for  output3
// 			289	248	1	Low byte	W/R	Interlock for  output4
// 			290	249	1	Low byte	W/R	Interlock for  output5
// 			291	250	1	Low byte	W/R	Interlock for  output6
// 			292	251	1	Low byte	W/R	Interlock for  output7




		int nOutReg,nValue;

		//if (newtstat6[7] == 6)
		if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
		{
			nOutReg=245+m_nCurRow-1;
			nValue=product_register_value[nOutReg];


		}else
		{
			nOutReg=286+m_nCurRow-1;
			nValue=multi_register_value[nOutReg];
		}

	
		//int nValue=read_one(tstat_id,nOutReg);
		//nValue=multi_register_value[nOutReg];
#if 1
		//if(nValue==7&&(!m_bOut4PWM)&&(!m_bOut5PWM))
		//if((nValue==7&&lRow<4)||(lRow==4 && !m_bOut4PWM&&nValue==7)||(lRow==5 && !m_bOut5PWM&&nValue==7))
		if((nValue==7&&lRow<4)||(lRow==4 && !m_bOut4PWM&&nValue==7)||(lRow==5 && !m_bOut5PWM&&nValue==7)||(nValue==7&&lRow>5))//20121008
		{
			m_ItemValueCombx.ResetContent();
			m_ItemValueCombx.AddString(_T("0%"));
			m_ItemValueCombx.AddString(_T("100%"));
			m_ItemValueCombx.AddString(_T("MIN->100%"));
			m_ItemValueCombx.AddString(_T("MIN|100%"));
			m_ItemValueCombx.AddString(_T("MIN"));
			m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
			m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_ItemValueCombx.BringWindowToTop();
			m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
			m_ItemValueCombx.SetFocus(); //获取焦点
		}
	//	else if((lRow<4)||(lRow==4 && !m_bOut4PWM)||(lRow==5 && !m_bOut5PWM))
		else if((lRow<4) ||(lRow==4 && !m_bOut4PWM)||(lRow==5 && !m_bOut5PWM))//20121008
		{		
		 //Row 1-3
			m_ItemValueCombx.ResetContent();
			m_ItemValueCombx.AddString(_T("Off"));
			m_ItemValueCombx.AddString(_T("On"));
	/*		m_ItemValueCombx.AddString(_T("Close"));
			m_ItemValueCombx.AddString(_T("Open"));
			m_ItemValueCombx.AddString(_T("0-100"));
			m_ItemValueCombx.AddString(_T("50-100"));
			m_ItemValueCombx.AddString(_T("0-50"));*/
			m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
			m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_ItemValueCombx.BringWindowToTop();
			m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
			m_ItemValueCombx.SetFocus(); //获取焦点
		}
		else if((lRow>5))//20121008
		{		
		    


			 if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
			 {
			 if (lRow==6)
				 {
				 if (product_register_value[207]==0)
					 {
				m_ItemValueCombx.ResetContent();
			m_ItemValueCombx.AddString(_T("Off"));
			m_ItemValueCombx.AddString(_T("On")); 
			/*m_ItemValueCombx.AddString(_T("Close"));
			m_ItemValueCombx.AddString(_T("Open"));
			m_ItemValueCombx.AddString(_T("0-100"));
			m_ItemValueCombx.AddString(_T("50-100"));
			m_ItemValueCombx.AddString(_T("0-50"));*/
			m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
			m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_ItemValueCombx.BringWindowToTop();
			m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
			m_ItemValueCombx.SetFocus(); //获取焦点
				} 
				else
				{
				  m_ItemValueCombx.ResetContent();
			/*m_ItemValueCombx.AddString(_T("Off"));
			m_ItemValueCombx.AddString(_T("On"));*/
			m_ItemValueCombx.AddString(_T("Close"));
			m_ItemValueCombx.AddString(_T("Open"));
			m_ItemValueCombx.AddString(_T("0-100"));
			m_ItemValueCombx.AddString(_T("50-100"));
			m_ItemValueCombx.AddString(_T("0-50"));
			m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
			m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_ItemValueCombx.BringWindowToTop();
			m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
			m_ItemValueCombx.SetFocus(); //获取焦点
				}
			    } 
			    else if (lRow==7)//lRow==7
			    {
				  if (product_register_value[208]==0)
				  {
				  m_ItemValueCombx.ResetContent();
			m_ItemValueCombx.AddString(_T("Off"));
			m_ItemValueCombx.AddString(_T("On")); 
			 
			m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
			m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_ItemValueCombx.BringWindowToTop();
			m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
			m_ItemValueCombx.SetFocus(); //获取焦点
				  } 
				  else
				  {
				    m_ItemValueCombx.ResetContent();
			 
			m_ItemValueCombx.AddString(_T("Close"));
			m_ItemValueCombx.AddString(_T("Open"));
			m_ItemValueCombx.AddString(_T("0-100"));
			m_ItemValueCombx.AddString(_T("50-100"));
			m_ItemValueCombx.AddString(_T("0-50"));
			m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
			m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_ItemValueCombx.BringWindowToTop();
			m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
			m_ItemValueCombx.SetFocus(); //获取焦点
				  }
			    }
			 } 
			 else
			 {
			   
			 if (lRow==6)
				 {
				 if (multi_register_value[186]!=0)
					 {
					 m_ItemValueCombx.ResetContent();
					 m_ItemValueCombx.AddString(_T("Close"));
					 m_ItemValueCombx.AddString(_T("Open"));
					 m_ItemValueCombx.AddString(_T("0-100"));
					 m_ItemValueCombx.AddString(_T("50-100"));
					 m_ItemValueCombx.AddString(_T("0-50"));
					 m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
					 m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
					 m_ItemValueCombx.BringWindowToTop();
					 m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
					 m_ItemValueCombx.SetFocus(); //获取焦点
					 } 
				 else
					 {
					 m_ItemValueCombx.ResetContent();
					 m_ItemValueCombx.AddString(_T("Off"));
					 m_ItemValueCombx.AddString(_T("On"));
					 //m_ItemValueCombx.AddString(_T("0-100"));
					 //m_ItemValueCombx.AddString(_T("50-100"));
					 //m_ItemValueCombx.AddString(_T("0-50"));
					 m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
					 m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
					 m_ItemValueCombx.BringWindowToTop();
					 m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
					 m_ItemValueCombx.SetFocus(); //获取焦点
					 }
				 }
			else if (lRow==7)
			{
			if (multi_register_value[187]!=0)
				{
				m_ItemValueCombx.ResetContent();
				m_ItemValueCombx.AddString(_T("Close"));
				m_ItemValueCombx.AddString(_T("Open"));
				m_ItemValueCombx.AddString(_T("0-100"));
				m_ItemValueCombx.AddString(_T("50-100"));
				m_ItemValueCombx.AddString(_T("0-50"));
				m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
				m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
				m_ItemValueCombx.BringWindowToTop();
				m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
				m_ItemValueCombx.SetFocus(); //获取焦点
				} 
			else
				{
				m_ItemValueCombx.ResetContent();
				m_ItemValueCombx.AddString(_T("Off"));
				m_ItemValueCombx.AddString(_T("On"));
				//m_ItemValueCombx.AddString(_T("0-100"));
				//m_ItemValueCombx.AddString(_T("50-100"));
				//m_ItemValueCombx.AddString(_T("0-50"));
				m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
				m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
				m_ItemValueCombx.BringWindowToTop();
				m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
				m_ItemValueCombx.SetFocus(); //获取焦点
				}
			} 
			else
			{
			}
			 
			
			   
			  
			  
			 }
		}
		 
#endif
		
		 
	}
	//后来修改了，不在限制在四和第五行了。现在都支持PWM模式了
	if(lCol>5)
	{   
	    if (product_register_value[MODBUS_MODE_OUTPUT1+lRow-1]!=0)
	    {
				m_ItemValueCombx.ResetContent();
				m_ItemValueCombx.AddString(_T("Close"));
				m_ItemValueCombx.AddString(_T("Open"));
				m_ItemValueCombx.AddString(_T("0-100"));
				m_ItemValueCombx.AddString(_T("50-100"));
				m_ItemValueCombx.AddString(_T("0-50"));
				m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
				m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
				m_ItemValueCombx.BringWindowToTop();
				m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
				m_ItemValueCombx.SetFocus(); //获取焦点
	    }
		else
		{
			m_ItemValueCombx.ResetContent();
			m_ItemValueCombx.AddString(_T("Off"));
			m_ItemValueCombx.AddString(_T("On"));
	/*		m_ItemValueCombx.AddString(_T("Close"));
			m_ItemValueCombx.AddString(_T("Open"));
			m_ItemValueCombx.AddString(_T("0-100"));
			m_ItemValueCombx.AddString(_T("50-100"));
			m_ItemValueCombx.AddString(_T("0-50"));*/
			m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
			m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_ItemValueCombx.BringWindowToTop();
			m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
			m_ItemValueCombx.SetFocus(); //获取焦点
		}
	    
		//if(lRow==4 && (m_bOut4PWM/*||m_bFloat*/))
		//{
		//	m_ItemValueCombx.ResetContent();
		//	m_ItemValueCombx.AddString(_T("Close"));
		//	m_ItemValueCombx.AddString(_T("Open"));
		//	m_ItemValueCombx.AddString(_T("0-100"));
		//	m_ItemValueCombx.AddString(_T("50-100"));
		//	m_ItemValueCombx.AddString(_T("0-50"));
		//	m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
		//	m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
		//	m_ItemValueCombx.BringWindowToTop();
		//	m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
		//	m_ItemValueCombx.SetFocus(); //获取焦点
		//}
		//if(lRow==5 && (m_bOut5PWM/*||m_bFloat*/))
		//{
		//	m_ItemValueCombx.ResetContent();
		//	m_ItemValueCombx.AddString(_T("Close"));
		//	m_ItemValueCombx.AddString(_T("Open"));
		//	m_ItemValueCombx.AddString(_T("0-100"));
		//	m_ItemValueCombx.AddString(_T("50-100"));
		//	m_ItemValueCombx.AddString(_T("0-50"));
		//	m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
		//	m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
		//	m_ItemValueCombx.BringWindowToTop();
		//	m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
		//	m_ItemValueCombx.SetFocus(); //获取焦点
		//}



	}

}


void COutPutDlg::OnCbnKillfocusValueitemcombo()
{





// 	341	375				reserved, wall setpoint
// 		342	376				reserved,ceiling setpoint
// 		343	377				reserved,average setpoint
// 		344	378				reserved,unoccupied heating
// 		345	379				reserved,unoccupied cooling
// 		346	380				reserved,humidity setpoint

//	if ((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7))
//	{
//#if 1//0911
//		if(g_OutPutLevel==1)
//			return;
//
//		m_ItemValueCombx.ShowWindow(SW_HIDE);
//
//		long lRow,lCol,nCounts;
//		CString strOldText;
//		CString strNewText;
//		if(!m_bflexgrid1_or_2)
//		{
//			lRow = m_FlexGrid1.get_RowSel();//获取点击的行号	
//			lCol = m_FlexGrid1.get_ColSel(); //获取点击的列号
//			nCounts=m_FlexGrid1.get_Rows();
//
//			strOldText=m_FlexGrid1.get_TextMatrix(lRow,lCol);
//			if(strOldText.IsEmpty())
//				return;
//			m_ItemValueCombx.GetWindowText(strNewText);
//			int nItem=m_ItemValueCombx.GetCurSel();
//			if(nItem<0)
//				return;
//			m_ItemValueCombx.GetLBText(nItem,strNewText);
//			m_FlexGrid1.put_TextMatrix(lRow,lCol,strNewText);
//			OnWrite(MKBOOL(m_bflexgrid1_or_2),lCol,lRow);	
//
//		}
//		else//grid 2
//		{
//			lRow = m_FlexGrid2.get_RowSel();//获取点击的行号	
//			lCol = m_FlexGrid2.get_ColSel(); //获取点击的列号
//			nCounts=m_FlexGrid2.get_Rows();
//
//			strOldText=m_FlexGrid2.get_TextMatrix(lRow,lCol);
//			if(strOldText.IsEmpty())
//				return;
//			m_ItemValueCombx.GetWindowText(strNewText);
//			int nItem=m_ItemValueCombx.GetCurSel();
//			if(nItem<0)
//				return;
//			m_ItemValueCombx.GetLBText(nItem,strNewText);
//			m_FlexGrid2.put_TextMatrix(lRow,lCol,strNewText);
//			OnWrite(MKBOOL(m_bflexgrid1_or_2),lCol,lRow);
//
//
//
//		}
//		m_ItemValueCombx.ShowWindow(SW_HIDE);
//		//float are realized in onwrite:
//
//		BYTE nreg;
////		BYTE ntemp;
//		if(lRow==4 && m_bOut4PWM&&lCol>=6&&!m_bflexgrid1_or_2)//	if (lCol==6||lCol==7||lCol==8||lCol==9||lCol==10||lCol==11||lCol==12)
//		{
//			int CoastCol=m_PID1_heat_stages+6;
//			int nPos;
//			//	nPos=m_PID1_cool_stages;
//
//			//	CString strValue;
//			//	m_ItemValueCombx.GetWindowText(strValue);
//			//	int nValue=atoi(strValue);
//			int nValue=m_ItemValueCombx.GetCurSel();
//
//
//			int nCol= m_FlexGrid1.get_ColSel()-5;	
//
//			if(nCol < (m_PID1_heat_stages+1))
//				nPos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - nCol ;
//			else
//				nPos = nCol - (m_PID1_heat_stages+1);
//
//			//nreg=read_one(tstat_id,341+nPos);
//			nreg=(BYTE)multi_register_value[375+nPos];
//			nreg=nreg&0x0f;
//			nValue=nValue<<4;
//			nValue=nreg|nValue;
//			write_one(g_tstat_id,375+nPos,nValue);
//			m_FlexGrid1.put_TextMatrix(lRow,lCol,strNewText);
//			m_FlexGrid1.SetFocus();
//			FreshGrids();
//			return;
//		}
//		if(lRow==5 && m_bOut5PWM&&lCol>=6&&!m_bflexgrid1_or_2)
//		{
//			int CoastCol=m_PID1_heat_stages+6;
//			int nPos;
//			//	CString strValue;
//			//	m_ItemValueCombx.GetWindowText(strValue);
//
//			int nValue=m_ItemValueCombx.GetCurSel();
//
//			int nCol= m_FlexGrid1.get_ColSel()-5;
//			if(nCol < (m_PID1_heat_stages+1))
//				nPos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - nCol ;
//			else
//				nPos = nCol - (m_PID1_heat_stages+1);
//			//	nreg=read_one(tstat_id,341+nPos);
//			nreg=(BYTE)multi_register_value[375+nPos];
//			nreg=nreg&0xf0;
//			nValue=nreg|nValue;
//
//			write_one(g_tstat_id,375+nPos,nValue);
//			m_FlexGrid1.put_TextMatrix(lRow,lCol,strNewText);
//			m_FlexGrid1.SetFocus();
//			FreshGrids();
//			return;
//		}
//		/////FREE COOL:
//		//286	245	1	Low byte	W/R	Interlock for  output1
//
//		int nOutReg;
//		nOutReg=245+lRow-1;
//		int	nValue=multi_register_value[nOutReg];
//		if(nValue==7&&lCol>=6&&!m_bflexgrid1_or_2)
//		{
//
//			int nPos;
//			//CString strValue;
//			//m_ItemValueCombx.GetWindowText(strValue);
//			int nValue=m_ItemValueCombx.GetCurSel();
//
//			lCol=lCol-5;	
//
//			if(lCol < (m_PID1_heat_stages+1))
//				nPos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - lCol ;
//			else
//				nPos = lCol - (m_PID1_heat_stages+1);	
//			//362	125	1	Low byte	W/R	ANALOG INPUT4 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
//
//			if(m_fan.GetCurSel()==0)
//			{
//				int ndata;
//				nValue=nValue<<4;
//				int nReg=125+nPos;
//				ndata=multi_register_value[125+nPos];
//				if(ndata>=0)
//				{
//					ndata=ndata&0x0F;//清掉4-7位
//					nValue=nValue|ndata;
//					write_one(g_tstat_id,362+nPos,nValue);
//				}
//			}
//			else//fan speed is not off.
//			{
//				int ndata;
//				int nReg=125+nPos;
//				ndata=multi_register_value[125+nPos];
//				ndata=ndata&0xF0;
//				if(ndata>=0)
//				{
//					nValue=nValue|ndata;
//					write_one(g_tstat_id,125+nPos,nValue);
//				}
//			}
//			m_FlexGrid1.SetFocus();
//			FreshGrids();
//			return;
//		}
//
//#endif
//
//
//	}else
//	{
//#if 1//0911
//	if(g_OutPutLevel==1)
//		return;
//
//	m_ItemValueCombx.ShowWindow(SW_HIDE);
//
//	long lRow,lCol,nCounts;
//	CString strOldText;
//	CString strNewText;
//
//#if 1
//	if(!m_bflexgrid1_or_2)
//	{
//	//	lRow = m_FlexGrid1.get_RowSel();//获取点击的行号	
//		lRow = pid1lrow; //如果不这样，那么，修改第一任意排，则第5排会自动变会。
//		lCol = m_FlexGrid1.get_ColSel(); //获取点击的列号
//		nCounts=m_FlexGrid1.get_Rows();
//
//		strOldText=m_FlexGrid1.get_TextMatrix(lRow,lCol);
//		if(strOldText.IsEmpty())
//			return;
//
//		m_ItemValueCombx.GetWindowText(strNewText);
//		int nItem=m_ItemValueCombx.GetCurSel();
//		if(nItem<0)
//			return;
//		m_ItemValueCombx.GetLBText(nItem,strNewText);
//		m_FlexGrid1.put_TextMatrix(lRow,lCol,strNewText);
//		OnWrite(MKBOOL(m_bflexgrid1_or_2),lCol,lRow);	
//	
//	}
//	else//grid 2
//	{		//lRow = m_FlexGrid2.get_RowSel();//获取点击的行号
//		lRow = pid2lrow;
//
//		//lRow = m_FlexGrid2.get_Row();//获取点击的行号	 2.5.0.94 以前只要PID，OUTPUT6，7，只要修改6行，7行自动改变。
//		lCol = m_FlexGrid2.get_ColSel(); //获取点击的列号
//		nCounts=m_FlexGrid2.get_Rows();
//
//
//
//		//////////////////////////////////////////////////////////////////////////2.5.0.99
//
//
//
//		BYTE nreg;
////		BYTE ntemp;
//		if(lRow==4 && m_bOut4PWM&&lCol>=6)//	if (lCol==6||lCol==7||lCol==8||lCol==9||lCol==10||lCol==11||lCol==12)
//		{
//			int CoastCol=m_PID2_heat_stages+6;
//			int nPos;
//			//	nPos=m_PID1_cool_stages;
//
//			//	CString strValue;
//			//	m_ItemValueCombx.GetWindowText(strValue);
//			//	int nValue=atoi(strValue);
//			int nValue=m_ItemValueCombx.GetCurSel();
//
//
//			int nCol= m_FlexGrid2.get_ColSel()-5;	
//
//			if(nCol < (m_PID2_heat_stages+1))
//				nPos = (m_PID2_heat_stages+m_PID2_cool_stages+1) - nCol ;
//			else
//				nPos = nCol - (m_PID2_heat_stages+1);
//
//			//nreg=read_one(tstat_id,341+nPos);
//			nreg=(BYTE)multi_register_value[341+nPos];
//			nreg=nreg&0x0f;
//			nValue=nValue<<4;
//			nValue=nreg|nValue;
//			write_one(g_tstat_id,341+nPos,nValue);
//			multi_register_value[341+nPos] = nValue;//2.5.0.99
//			m_FlexGrid2.put_TextMatrix(lRow,lCol,strNewText);
//			m_FlexGrid2.SetFocus();
//				FreshGrids();//lsc 20120928 2.5.0.99
//			return;
//		}
//		if(lRow==5 && m_bOut5PWM&&lCol>=6)
//		{
//			int CoastCol=m_PID2_heat_stages+6;
//			int nPos;
//			//	CString strValue;
//			//	m_ItemValueCombx.GetWindowText(strValue);
//
//			int nValue=m_ItemValueCombx.GetCurSel();
//
//			int nCol= m_FlexGrid1.get_ColSel()-5;
//			if(nCol < (m_PID2_heat_stages+1))
//				nPos = (m_PID2_heat_stages+m_PID2_cool_stages+1) - nCol ;
//			else
//				nPos = nCol - (m_PID2_heat_stages+1);
//			//	nreg=read_one(tstat_id,341+nPos);
//			nreg=(BYTE)multi_register_value[341+nPos];
//			nreg=nreg&0xf0;
//			nValue=nreg|nValue;
//
//			write_one(g_tstat_id,341+nPos,nValue);
//			multi_register_value[341+nPos] = nValue;//2.5.0.99
//			m_FlexGrid2.put_TextMatrix(lRow,lCol,strNewText);
//			m_FlexGrid2.SetFocus();
//			FreshGrids(); //lsc 20120928  2.5.0.99
//			return;
//		}
//		//////////////////////////////////////////////////////////////////////////
//
//// 		//lRow = m_FlexGrid2.get_RowSel();//获取点击的行号//2.5.0.99
//// 		lRow = pid2lrow;
//// 	
//// 		//lRow = m_FlexGrid2.get_Row();//获取点击的行号	 2.5.0.94 以前只要PID，OUTPUT6，7，只要修改6行，7行自动改变。
//// 		lCol = m_FlexGrid2.get_ColSel(); //获取点击的列号
//// 		nCounts=m_FlexGrid2.get_Rows();
//
//		strOldText=m_FlexGrid2.get_TextMatrix(lRow,lCol);
//		if(strOldText.IsEmpty())
//			return;
//		m_ItemValueCombx.GetWindowText(strNewText);
//		int nItem=m_ItemValueCombx.GetCurSel();
//		if(nItem<0)
//			return;
//		m_ItemValueCombx.GetLBText(nItem,strNewText);
//		m_FlexGrid2.put_TextMatrix(lRow,lCol,strNewText); //2.5.0.94
//		OnWrite(MKBOOL(m_bflexgrid1_or_2),lCol,lRow);//2.5.0.94
//
//
//
//	}
//#endif
//	m_ItemValueCombx.ShowWindow(SW_HIDE);
//	//float are realized in onwrite:
//
//		BYTE nreg;
////		BYTE ntemp;
//		if(lRow==4 && m_bOut4PWM&&lCol>=6&&!m_bflexgrid1_or_2)//	if (lCol==6||lCol==7||lCol==8||lCol==9||lCol==10||lCol==11||lCol==12)
//		{
//			int CoastCol=m_PID1_heat_stages+6;
//			int nPos;
//		//	nPos=m_PID1_cool_stages;
//
//		//	CString strValue;
//		//	m_ItemValueCombx.GetWindowText(strValue);
//		//	int nValue=atoi(strValue);
//			int nValue=m_ItemValueCombx.GetCurSel();
//
//
//			int nCol= m_FlexGrid1.get_ColSel()-5;	
//				
//			if(nCol < (m_PID1_heat_stages+1))
//				nPos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - nCol ;
//			else
//				nPos = nCol - (m_PID1_heat_stages+1);
//
//			//nreg=read_one(tstat_id,341+nPos);
//			nreg=(BYTE)multi_register_value[341+nPos];
//			nreg=nreg&0x0f;
//			nValue=nValue<<4;
//			nValue=nreg|nValue;
//			write_one(g_tstat_id,341+nPos,nValue);
//			m_FlexGrid1.put_TextMatrix(lRow,lCol,strNewText);
//			m_FlexGrid1.SetFocus();
//		//	FreshGrids();//lsc 20120928
//			return;
//		}
//		if(lRow==5 && m_bOut5PWM&&lCol>=6&&!m_bflexgrid1_or_2)
//		{
//			int CoastCol=m_PID1_heat_stages+6;
//			int nPos;
//		//	CString strValue;
//		//	m_ItemValueCombx.GetWindowText(strValue);
//
//			int nValue=m_ItemValueCombx.GetCurSel();
//
//			int nCol= m_FlexGrid1.get_ColSel()-5;
//			if(nCol < (m_PID1_heat_stages+1))
//				nPos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - nCol ;
//			else
//				nPos = nCol - (m_PID1_heat_stages+1);
//		//	nreg=read_one(tstat_id,341+nPos);
//			nreg=(BYTE)multi_register_value[341+nPos];
//			nreg=nreg&0xf0;
//			nValue=nreg|nValue;
//
//			write_one(g_tstat_id,341+nPos,nValue);
//			m_FlexGrid1.put_TextMatrix(lRow,lCol,strNewText);
//			m_FlexGrid1.SetFocus();
//			//FreshGrids(); //lsc 20120928
//			return;
//		}
///////FREE COOL:
////
//	int nOutReg;
//	nOutReg=286+lRow-1;
//	int	nValue=multi_register_value[nOutReg];
//	if(nValue==7&&lCol>=6&&!m_bflexgrid1_or_2)
//	{
//
//		int nPos;
//		//CString strValue;
//		//m_ItemValueCombx.GetWindowText(strValue);
//		int nValue=m_ItemValueCombx.GetCurSel();
//
//		lCol=lCol-5;	
//		
//		if(lCol < (m_PID1_heat_stages+1))
//			nPos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - lCol ;
//		else
//			nPos = lCol - (m_PID1_heat_stages+1);	
//
//		if(m_fan.GetCurSel()==0)
//		{
//			int ndata;
//			nValue=nValue<<4;
//			int nReg=362+nPos;
//			ndata=multi_register_value[362+nPos];
//			if(ndata>=0)
//			{
//				ndata=ndata&0x0F;//清掉4-7位
//				nValue=nValue|ndata;
//				write_one(g_tstat_id,362+nPos,nValue);
//			}
//		}
//		else//fan speed is not off.
//		{
//			int ndata;
//			int nReg=362+nPos;
//			ndata=multi_register_value[362+nPos];
//			ndata=ndata&0xF0;
//			if(ndata>=0)
//			{
//				nValue=nValue|ndata;
//				write_one(g_tstat_id,362+nPos,nValue);
//			}
//		}
//		m_FlexGrid1.SetFocus();
//	//	FreshGrids(); //lsc20120928
//		return;
//	}
//
//#endif
//	}
}
void COutPutDlg::OnWrite(bool bflexgrid1_or_2,int col,int row)
{
	int ret=0;
	if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
	{
		if(g_OutPutLevel==1)
			return;
		unsigned char tstatval=0,pos,cellval=0;
		int row_temp,totalrows;
		bool ty;
		if(m_nmoduleType == 1)//5a
			totalrows = 3;
		else
			totalrows = 5 ;
		if(col==4)
		{//pid
			CString str2;
		
			{

				if(bflexgrid1_or_2==false)
					str2=m_FlexGrid1.get_TextMatrix(row,col);
				else
					str2=m_FlexGrid2.get_TextMatrix(row,col);
				if(str2.CompareNoCase(_T("PID1"))==0)
				{
					if(write_one(g_tstat_id,MODBUS_PID_OUTPUT1+row-1,0)<0)
					{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
					product_register_value[MODBUS_PID_OUTPUT1+row-1] = 0;
					product_register_value[MODBUS_PID_OUTPUT1+row-1] = 0;

				}
				else if(str2.CompareNoCase(_T("PID2"))==0)
				{
					if(write_one(g_tstat_id,MODBUS_PID_OUTPUT1+row-1,1)<0)
					{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
					product_register_value[MODBUS_PID_OUTPUT1+row-1] = 1;
					product_register_value[MODBUS_PID_OUTPUT1+row-1] = 1;

				}
				else if(str2.CompareNoCase(_T("MAX(PID1,PID2)"))==0)
				{
					if(write_one(g_tstat_id,MODBUS_PID_OUTPUT1+row-1,2)<0)
					{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}

					int aaa=read_one(g_tstat_id,MODBUS_PID_OUTPUT1+row-1,3);
					product_register_value[MODBUS_PID_OUTPUT1+row-1] = 2;
					product_register_value[MODBUS_PID_OUTPUT1+row-1] = 2;
				}
				else if(str2.CompareNoCase(_T("MIN(PID1,PID2)"))==0)
				{
					if(write_one(g_tstat_id,MODBUS_PID_OUTPUT1+row-1,3)<0)
					{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
					product_register_value[MODBUS_PID_OUTPUT1+row-1] = 3;
					product_register_value[MODBUS_PID_OUTPUT1+row-1] = 3;
				}
			}
		 


			FreshGrids(); //lsc add
		}
		else if(col==5)
		{
			CString str2;
			if(bflexgrid1_or_2==false)
				str2=m_FlexGrid1.get_TextMatrix(row,col);
			else
				str2=m_FlexGrid2.get_TextMatrix(row,col);
			if((str2.CompareNoCase(_T("On"))==0)||str2==_T("      -"))
			{
				if(write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,0)<0)
				{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1] = 0;
			}
			else if(str2.CompareNoCase(_T("DI1"))==0)
			{
				if(write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,1)<0)
				{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1] = 1;
			}
			else if((str2.CompareNoCase(_T("AI1"))==0)||str2.CompareNoCase(_T("//AI1"))==0)
			{
				if(write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,2)<0)
				{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1] = 2;
			}
			else if((str2.CompareNoCase(_T("AI2"))==0)||str2.CompareNoCase(_T("//AI2"))==0)
			{
				if(write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,3)<0)
				{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1] = 3;
			}
			else if(str2.CompareNoCase(_T("Timer OR"))==0)
			{
				if(write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,4)<0)
				{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1] = 4;
			}
			else if(str2.CompareNoCase(_T("Timer And"))==0)
			{
				if(write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,5)<0)
				{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1] = 5;
			}
			else if(str2.CompareNoCase(_T("InterLock Timer"))==0)
			{
				if(write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,6)<0)
				{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1] = 6;
			}
			else if(str2.CompareNoCase(_T("FreeCool"))==0)
			{
				if(write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,7)<0)
				{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1] = 7;
			}

		}
		CString strItem;

		if(col>5)
		{

			//	283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
			//	284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM



			// 		int output3_value=multi_register_value[283];//2 rows plug in one row//out 4
			// 		int output4_value=multi_register_value[284];//out 5;


			int output3_value;//=multi_register_value[283];//2 rows plug in one row//out 4
			int output4_value;//=multi_register_value[284];//out 5;


			//这里可能是变量定义的问题，让人难以理解 为什么 会错位;Fance
			output3_value = product_register_value[MODBUS_MODE_OUTPUT4]; // 283  205
			output4_value = product_register_value[MODBUS_MODE_OUTPUT5]; //284 206
			//------------------------------------------------------------------------
			//Annul by Fance 2013 04 10		//??有问题  205 对应的是output4 为什么要叫 output3_value
			//if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
			//{
			//	output3_value=newtstat6[205];//2 rows plug in one row//out 4
			//	output4_value=newtstat6[206];//out 5;
			//}else
			//{
			//	output3_value=multi_register_value[283];//2 rows plug in one row//out 4
			//	output4_value=multi_register_value[284];//out 5;
			//}
			//------------------------------------------------------------------------
			BOOL bFloat=FALSE;
			BOOL bONOFF=TRUE;

			if(bflexgrid1_or_2==false)
			{//grid one 1

				if((col-5) < (m_PID1_heat_stages+1))
					pos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - (col-5) ;
				else
					pos = (col-5) - (m_PID1_heat_stages+1);
				m_FlexGrid1.put_Col(col);

				for( row_temp = 1;row_temp<=totalrows;row_temp++)//***********************row_temp==totalrows+1
				{
					m_FlexGrid1.put_Row(row_temp);	
					strItem=m_FlexGrid1.get_TextMatrix(row_temp,col);
					if(string2digital(strItem,cellval,ty))
					{
						if(row_temp == 1)
							tstatval &= 0xFE;
						if(row_temp == 2)
							tstatval &= 0xFD;
						if(row_temp == 3)
							tstatval &= 0xFB ;
						if(!m_bFloat)//not floater out 4 and out5
						{
							if(row_temp == 4&&!m_bOut4PWM)
								tstatval &= 0xF7;
							if(row_temp == 5&&!m_bOut5PWM)
								tstatval &= 0xEF;
						}
						else
						{
							if(row_temp==4)
								break;
						}

						tstatval |= cellval << (row_temp -1) ;
					}
				}
				int n=get_real_fan_select() ;
				//138	288	1	Low byte	W/R	FAN0_OPERATION_TABLE_COAST
				//if (newtstat6[7] ==6)
				 
					if(write_one(g_tstat_id,MODBUS_FAN0_OPER_TABLE_COAST+get_real_fan_select() * 7 + pos,tstatval)<0)
					{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
					product_register_value[MODBUS_FAN0_OPER_TABLE_COAST+get_real_fan_select() * 7+pos] = tstatval;
			 

				if(m_bFloat)
				{
					int nValue=0;
					if(row==4&&output3_value==1)
					{
						strItem=m_FlexGrid1.get_TextMatrix(row,col);
						if(string2digital(strItem,cellval,ty) )
						{
							tstatval=tstatval&0xCF;
							nValue= cellval << 4 ;
							tstatval|=nValue;
						}
						//351	334	1	Low byte	W/R	analog output in OFF table, coating mode,bit1 means AO1 : 1 = on, 0 = off    bit3 means AO2 : 1 = on, 0 = off
						//173	323	1	Low byte	W/R	VALVE_OPER_TABLE_COAST, Analog output state for each of the 7 modes of operation

						if(m_fan.GetCurSel()==0)
						{
							//if (newtstat6[7] == 6)
							 
								if(write_one(g_tstat_id,MODBUS_VALVE_OFF_TABLE_COAST+pos,tstatval)<0)
								{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
								product_register_value[MODBUS_VALVE_OFF_TABLE_COAST+pos] = tstatval;
						 
						}
						else
						{
							 
						 
								if(write_one(g_tstat_id,MODBUS_VALVE_OPERATION_TABLE_BEGIN+pos,tstatval)<0)
								{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
								product_register_value[MODBUS_VALVE_OPERATION_TABLE_BEGIN+pos] = tstatval;
					 
						}
					}
					if(row==5&&output4_value==1)
					{
						strItem=m_FlexGrid1.get_TextMatrix(row,col);
						if(string2digital(strItem,cellval,ty) )
						{
							tstatval=tstatval&0xCF;
							nValue= cellval << 4 ;
							tstatval|=nValue;
						}
						if(m_fan.GetCurSel()==0)

						{
							//if (newtstat6[7] == 6)
							 
								if(write_one(g_tstat_id,MODBUS_VALVE_OFF_TABLE_COAST+pos,tstatval)<0)
								{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
								product_register_value[MODBUS_VALVE_OFF_TABLE_COAST+pos] = tstatval;
						 
						}
						else
						{
							 
								if(write_one(g_tstat_id,MODBUS_VALVE_OPERATION_TABLE_BEGIN+pos,tstatval)<0)
								{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
								product_register_value[323+pos]= tstatval;
							 
						}
					}
				}

		
			 

				
					
						totalrows = 6 ;////////////////
					if((col-5) < (m_PID1_heat_stages+1))
						pos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - (col-5) ;
					else
						pos = (col-5) - (m_PID1_heat_stages+1);
					m_FlexGrid1.put_Col(col);
					tstatval &= 0x3F ;//clear 7 bit and 8 bit  for 0-50

					for( row_temp=totalrows;row_temp<=totalrows+1;row_temp++)
					{
						CString strItemText=m_FlexGrid1.get_TextMatrix(row_temp,col);
						//	m_FlexGrid1.put_Row(row_temp);
						string2digital(strItemText,cellval,ty);	
						/*if()
						{*/						
						if(cellval==4)
						{//for 0-50
							if(row_temp==totalrows)
								tstatval |=64;//第6位
							else if(row_temp==(totalrows+1))//0-50
								tstatval |=128;//第7位
						}
						else
						{
							if(row_temp == totalrows)
							{
								tstatval &= 0xFC ;//1111 1100
							}
							if(row_temp == totalrows+1)
							{
								tstatval &= 0xF3 ;//1111 0011
								cellval <<= 2 ; 
							}
							tstatval |= cellval  ;
						}						
						/*}*/
					}
					if(m_fan.GetCurSel()==0)
					{
						//if (newtstat6[7] == 6)
						 
							if(write_one(g_tstat_id,MODBUS_VALVE_OFF_TABLE_COAST+pos,tstatval)<0)
							{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
							product_register_value[MODBUS_VALVE_OFF_TABLE_COAST+pos] = tstatval;
						 
					}
					else
					{
						//if (newtstat6[7] == 6)
						 
							if(write_one(g_tstat_id,MODBUS_VALVE_OPERATION_TABLE_BEGIN+pos,tstatval)<0)
							{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
							product_register_value[MODBUS_VALVE_OPERATION_TABLE_BEGIN+pos] = tstatval;
						 
					}

				
			}
			else//grid 2:
			{

					if(product_register_value[MODBUS_FAN_SPEED]==4)//Auto 
					{
						if((col-5) < (m_PID2_heat_stages+1))
						pos = (m_PID2_heat_stages+m_PID2_cool_stages+1) - (col-5) ;
					else
						pos = (col-5) - (m_PID2_heat_stages+1);
					m_FlexGrid2.put_Col(col);////////////////////////////////////////////////////////
					 if (m_nCurRow<=5)
					 {
					 for( row_temp = 1;row_temp<=totalrows;row_temp++)//***********************row_temp==totalrows+1
					{
						m_FlexGrid2.put_Row(row_temp);////////////////////////////////////////////////////
						if(string2digital(m_FlexGrid2.get_Text(),cellval,ty))
						{
							if(row_temp == 1)
								tstatval &= 0xFE;
							if(row_temp == 2)
								tstatval &= 0xFD;
							if(row_temp == 3)
								tstatval &= 0xFB ;
							if(row_temp == 4)
								tstatval &= 0xF7;
							if(row_temp == 5)
								tstatval &= 0xEF;
							tstatval |= cellval << (row_temp -1) ;
						}

					}
					if(write_one(g_tstat_id,MODBUS_UNIVERSAL_OUTPUT_BEGIN + pos,tstatval)<0)
						{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
						product_register_value[MODBUS_UNIVERSAL_OUTPUT_BEGIN + pos] = tstatval;
					 }
					 else
					 {
						 totalrows = 6 ;////////////////
						 if((col-5) < (m_PID2_heat_stages+1))
							 pos = (m_PID2_heat_stages+m_PID2_cool_stages+1) - (col-5) ;
						 else
							 pos = (col-5) - (m_PID2_heat_stages+1);
						 m_FlexGrid2.put_Col(col);
						 //tstatval &= 0x3F ;//clear 7 bit and 8 bit  for 0-50
						 tstatval=0;
						 for( row_temp=totalrows;row_temp<=totalrows+1;row_temp++)
						 {
							 m_FlexGrid2.put_Row(row_temp);	
							 if(string2digital(m_FlexGrid2.get_Text(),cellval,ty))
							 {
								 if(cellval==4 )
								 {//for 0-50
									 if(row_temp==totalrows)
										 tstatval |=64;
									 else if(row_temp==(totalrows+1))
										 tstatval |=128;
								 }
								 else
								 {
									 if(row_temp == totalrows)
									 {
										 tstatval &= 0xFC ;
									 }
									 if(row_temp == totalrows+1)
									 {
										 tstatval &= 0xF3 ;
										 cellval <<= 2 ; 
									 }
									 tstatval |= cellval  ;
								 }
							 }

						 }
						 //

						 if(write_one(g_tstat_id,MODBUS_UNIVERSAL_VALVE_BEGIN+pos,tstatval)<0)//没找到对应的值。
						 {MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
						 product_register_value[MODBUS_UNIVERSAL_VALVE_BEGIN+pos]=tstatval;
					 
					 }
			

					 
						 
							
					 
						}
					else
						{

							if((col-5) < (m_PID2_heat_stages+1))
							pos = (m_PID2_heat_stages+m_PID2_cool_stages+1) - (col-5) ;
						else
							pos = (col-5) - (m_PID2_heat_stages+1);
						m_FlexGrid2.put_Col(col);////////////////////////////////////////////////////////


						if (m_nCurRow<=5)
						{
						for( row_temp = 1;row_temp<=totalrows;row_temp++)//***********************row_temp==totalrows+1
						{
							m_FlexGrid2.put_Row(row_temp);////////////////////////////////////////////////////
							if(string2digital(m_FlexGrid2.get_Text(),cellval,ty))
							{
								if(row_temp == 1)
									tstatval &= 0xFE;
								if(row_temp == 2)
									tstatval &= 0xFD;
								if(row_temp == 3)
									tstatval &= 0xFB ;
								if(row_temp == 4)
									tstatval &= 0xF7;
								if(row_temp == 5)
									tstatval &= 0xEF;
								tstatval |= cellval << (row_temp -1) ;
							}

						}
						if(write_one(g_tstat_id,MODBUS_UNIVERSAL_OUTPUT_BEGIN + pos,tstatval)<0)
							{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
							product_register_value[MODBUS_UNIVERSAL_OUTPUT_BEGIN + pos] = tstatval;
						}
						else
						{
						  if (product_register_value[MODBUS_FAN_SPEED]==0)
						  {totalrows = 6 ;////////////////
							if((col-5) < (m_PID2_heat_stages+1))
								pos = (m_PID2_heat_stages+m_PID2_cool_stages+1) - (col-5) ;
							else
								pos = (col-5) - (m_PID2_heat_stages+1);
							m_FlexGrid2.put_Col(col);
							//tstatval &= 0x3F ;//clear 7 bit and 8 bit  for 0-50
							tstatval=0;
							for( row_temp=totalrows;row_temp<=totalrows+1;row_temp++)
							{
								m_FlexGrid2.put_Row(row_temp);	
								if(string2digital(m_FlexGrid2.get_Text(),cellval,ty))
								{
									if(cellval==4 )
									{//for 0-50
										if(row_temp==totalrows)
											tstatval |=64;
										else if(row_temp==(totalrows+1))
											tstatval |=128;
									}
									else
									{
										if(row_temp == totalrows)
										{
											tstatval &= 0xFC ;
										}
										if(row_temp == totalrows+1)
										{
											tstatval &= 0xF3 ;
											cellval <<= 2 ; 
										}
										tstatval |= cellval  ;
									}
								}

							}
							//

							if(write_one(g_tstat_id,MODBUS_UNIVERSAL_OFF_VALVE_BEGIN+pos,tstatval)<0)//没找到对应的值。
							{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
							product_register_value[MODBUS_UNIVERSAL_OFF_VALVE_BEGIN+pos]=tstatval;
						  }
							
						}

			

						 
							 

						 
						}
			   

				
			}
		}
		//	FreshGrids();

	}
	else if (product_register_value[7]==16)
	{
			if(g_OutPutLevel==1)
			return;
		unsigned char tstatval=0,pos,cellval=0;
		int row_temp,totalrows;
		bool ty;
		totalrows=5;
		if(col==4)
		{//pid
			CString str2;
			{

				if(bflexgrid1_or_2==false)
					str2=m_FlexGrid1.get_TextMatrix(row,col);
				else
					str2=m_FlexGrid2.get_TextMatrix(row,col);
				if(str2.CompareNoCase(_T("PID1"))==0)
				{
					if(write_one(g_tstat_id,MODBUS_PID_OUTPUT1+row-1,0)<0)
					{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
					 
					product_register_value[MODBUS_PID_OUTPUT1+row-1] = 0;

				}
				else if(str2.CompareNoCase(_T("PID2"))==0)
				{
					if(write_one(g_tstat_id,MODBUS_PID_OUTPUT1+row-1,1)<0)
					{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
					 
					product_register_value[MODBUS_PID_OUTPUT1+row-1] = 1;

				}
				else if(str2.CompareNoCase(_T("MAX(PID1,PID2)"))==0)
				{
					if(write_one(g_tstat_id,MODBUS_PID_OUTPUT1+row-1,2)<0)
					{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}

					/*int aaa=read_one(g_tstat_id,274+row-1,3);
					product_register_value[274+row-1] = 2;*/
					product_register_value[MODBUS_PID_OUTPUT1+row-1] = 2;
				}
				else if(str2.CompareNoCase(_T("MIN(PID1,PID2)"))==0)
				{
					if(write_one(g_tstat_id,MODBUS_PID_OUTPUT1+row-1,3)<0)
					{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
					 
					product_register_value[MODBUS_PID_OUTPUT1+row-1] = 3;
				}
			}
			// 			    	247	274	1	Low byte	W/R	Output 1 PID Interlock                    0 = PID1, can assign each output to either PID1 or 2, the max or the min of the two PIDS
			// 				286	245	1	Low byte	W/R	Interlock for  output1
			// 				287	246	1	Low byte	W/R	Interlock for  output2
			// 				288	247	1	Low byte	W/R	Interlock for  output3
			// 				289	248	1	Low byte	W/R	Interlock for  output4
			// 				290	249	1	Low byte	W/R	Interlock for  output5
			// 				291	250	1	Low byte	W/R	Interlock for  output6
			// 				292	251	1	Low byte	W/R	Interlock for  output7


			FreshGrids(); //lsc add
		}
		else if(col==5)
		{
			CString str2;
			if(bflexgrid1_or_2==false)
				str2=m_FlexGrid1.get_TextMatrix(row,col);
			else
				str2=m_FlexGrid2.get_TextMatrix(row,col);
			if((str2.CompareNoCase(_T("On"))==0)||str2==_T("      -"))
			{
				if(write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,0)<0)
				{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1] = 0;
			}
			else if(str2.CompareNoCase(_T("DI1"))==0)
			{
				if(write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,1)<0)
				{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1] = 1;
			}
			else if((str2.CompareNoCase(_T("AI1"))==0)||str2.CompareNoCase(_T("//AI1"))==0)
			{
				if(write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,2)<0)
				{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1] = 2;
			}
			else if((str2.CompareNoCase(_T("AI2"))==0)||str2.CompareNoCase(_T("//AI2"))==0)
			{
				if(write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,3)<0)
				{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1] = 3;
			}
			else if(str2.CompareNoCase(_T("Timer OR"))==0)
			{
				if(write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,4)<0)
				{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1] = 4;
			}
			else if(str2.CompareNoCase(_T("Timer And"))==0)
			{
				if(write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,5)<0)
				{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1] = 5;
			}
			else if(str2.CompareNoCase(_T("InterLock Timer"))==0)
			{
				if(write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,6)<0)
				{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1] = 6;
			}
			else if(str2.CompareNoCase(_T("FreeCool"))==0)
			{
				if(write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,7)<0)
				{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1] = 7;
			}

		}
		CString strItem;

		if(col>5)
		{

			//	283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
			//	284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM



			// 		int output3_value=multi_register_value[283];//2 rows plug in one row//out 4
			// 		int output4_value=multi_register_value[284];//out 5;


			int output3_value;//=multi_register_value[283];//2 rows plug in one row//out 4
			int output4_value;//=multi_register_value[284];//out 5;


			//这里可能是变量定义的问题，让人难以理解 为什么 会错位;Fance
			output3_value = product_register_value[MODBUS_MODE_OUTPUT4]; // 283  205
			output4_value = product_register_value[MODBUS_MODE_OUTPUT5]; //284 206
			//------------------------------------------------------------------------
			//Annul by Fance 2013 04 10		//??有问题  205 对应的是output4 为什么要叫 output3_value
			//if ((newtstat6[7] == 6)||(newtstat6[7] == 7))
			//{
			//	output3_value=newtstat6[205];//2 rows plug in one row//out 4
			//	output4_value=newtstat6[206];//out 5;
			//}else
			//{
			//	output3_value=multi_register_value[283];//2 rows plug in one row//out 4
			//	output4_value=multi_register_value[284];//out 5;
			//}
			//------------------------------------------------------------------------
			BOOL bFloat=FALSE;
			BOOL bONOFF=TRUE;

			if(bflexgrid1_or_2==false)
			{//grid one 1

				if((col-5) < (m_PID1_heat_stages+1))
					pos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - (col-5) ;
				else
					pos = (col-5) - (m_PID1_heat_stages+1);
				m_FlexGrid1.put_Col(col);

				for( row_temp = 1;row_temp<=totalrows;row_temp++)//***********************row_temp==totalrows+1
				{
					m_FlexGrid1.put_Row(row_temp);	
					strItem=m_FlexGrid1.get_TextMatrix(row_temp,col);
					if(string2digital(strItem,cellval,ty))
					{
						if(row_temp == 1)
							tstatval &= 0xFE;
						if(row_temp == 2)
							tstatval &= 0xFD;
						if(row_temp == 3)
							tstatval &= 0xFB ;
						if(!m_bFloat)//not floater out 4 and out5
						{
							if(row_temp == 4&&!m_bOut4PWM)
								tstatval &= 0xF7;
							if(row_temp == 5&&!m_bOut5PWM)
								tstatval &= 0xEF;
						}
						else
						{
							if(row_temp==4)
								break;
						}

						tstatval |= cellval << (row_temp -1) ;
					}
				}
				int n=get_real_fan_select() ;
				//138	288	1	Low byte	W/R	FAN0_OPERATION_TABLE_COAST
				//if (newtstat6[7] ==6)
				 
					if(write_one(g_tstat_id,MODBUS_FAN0_OPER_TABLE_COAST+get_real_fan_select() * 7 + pos,tstatval)<0)
					{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
					product_register_value[MODBUS_FAN0_OPER_TABLE_COAST+get_real_fan_select() * 7+pos] = tstatval;
			 

				if(m_bFloat)
				{
					int nValue=0;
					if(row==4&&output3_value==1)
					{
						strItem=m_FlexGrid1.get_TextMatrix(row,col);
						if(string2digital(strItem,cellval,ty) )
						{
							tstatval=tstatval&0xCF;
							nValue= cellval << 4 ;
							tstatval|=nValue;
						}
						//351	334	1	Low byte	W/R	analog output in OFF table, coating mode,bit1 means AO1 : 1 = on, 0 = off    bit3 means AO2 : 1 = on, 0 = off
						//173	323	1	Low byte	W/R	VALVE_OPER_TABLE_COAST, Analog output state for each of the 7 modes of operation

						if(m_fan.GetCurSel()==0)
						{
							//if (newtstat6[7] == 6)
							 
								if(write_one(g_tstat_id,MODBUS_VALVE_OFF_TABLE_COAST+pos,tstatval)<0)
								{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
								product_register_value[MODBUS_VALVE_OFF_TABLE_COAST+pos] = tstatval;
						 
						}
						else
						{
							 
						 
								if(write_one(g_tstat_id,MODBUS_VALVE_OPERATION_TABLE_BEGIN+pos,tstatval)<0)
								{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
								product_register_value[MODBUS_VALVE_OPERATION_TABLE_BEGIN+pos] = tstatval;
					 
						}
					}
					if(row==5&&output4_value==1)
					{
						strItem=m_FlexGrid1.get_TextMatrix(row,col);
						if(string2digital(strItem,cellval,ty) )
						{
							tstatval=tstatval&0xCF;
							nValue= cellval << 4 ;
							tstatval|=nValue;
						}
						if(m_fan.GetCurSel()==0)

						{
							//if (newtstat6[7] == 6)
							 
								if(write_one(g_tstat_id,MODBUS_VALVE_OFF_TABLE_COAST+pos,tstatval)<0)
								{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
								product_register_value[MODBUS_VALVE_OFF_TABLE_COAST+pos] = tstatval;
							 
						}
						else
						{
							 
								if(write_one(g_tstat_id,MODBUS_VALVE_OPERATION_TABLE_BEGIN+pos,tstatval)<0)
								{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
								product_register_value[MODBUS_VALVE_OPERATION_TABLE_BEGIN+pos]= tstatval;
							 
						}
					}
				}

		
			 

				
					
						totalrows = 6 ;////////////////
					if((col-5) < (m_PID1_heat_stages+1))
						pos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - (col-5) ;
					else
						pos = (col-5) - (m_PID1_heat_stages+1);
					m_FlexGrid1.put_Col(col);
					tstatval &= 0x3F ;//clear 7 bit and 8 bit  for 0-50

					for( row_temp=totalrows;row_temp<=totalrows+1;row_temp++)
					{
						CString strItemText=m_FlexGrid1.get_TextMatrix(row_temp,col);
						//	m_FlexGrid1.put_Row(row_temp);
						string2digital(strItemText,cellval,ty);	
						/*if()
						{*/						
						if(cellval==4)
						{//for 0-50
							if(row_temp==totalrows)
								tstatval |=64;//第6位
							else if(row_temp==(totalrows+1))//0-50
								tstatval |=128;//第7位
						}
						else
						{
							if(row_temp == totalrows)
							{
								tstatval &= 0xFC ;//1111 1100
							}
							if(row_temp == totalrows+1)
							{
								tstatval &= 0xF3 ;//1111 0011
								cellval <<= 2 ; 
							}
							tstatval |= cellval  ;
						}						
						/*}*/
					}
					if(m_fan.GetCurSel()==0)
					{
						//if (newtstat6[7] == 6)
						 
							if(write_one(g_tstat_id,MODBUS_VALVE_OFF_TABLE_COAST+pos,tstatval)<0)
							{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
							product_register_value[MODBUS_VALVE_OFF_TABLE_COAST+pos] = tstatval;
						 
					}
					else
					{
						//if (newtstat6[7] == 6)
						 
							if(write_one(g_tstat_id,MODBUS_VALVE_OPERATION_TABLE_BEGIN+pos,tstatval)<0)
							{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
							product_register_value[MODBUS_VALVE_OPERATION_TABLE_BEGIN+pos] = tstatval;
						 
					}

				
			}
			else//grid 2:
			{

					if(product_register_value[MODBUS_FAN_SPEED]==4)//Auto 
					{
						if((col-5) < (m_PID2_heat_stages+1))
						pos = (m_PID2_heat_stages+m_PID2_cool_stages+1) - (col-5) ;
					else
						pos = (col-5) - (m_PID2_heat_stages+1);
					m_FlexGrid2.put_Col(col);////////////////////////////////////////////////////////
					 if (m_nCurRow<=5)
					 {
					 for( row_temp = 1;row_temp<=totalrows;row_temp++)//***********************row_temp==totalrows+1
					{
						m_FlexGrid2.put_Row(row_temp);////////////////////////////////////////////////////
						if(string2digital(m_FlexGrid2.get_Text(),cellval,ty))
						{
							if(row_temp == 1)
								tstatval &= 0xFE;
							if(row_temp == 2)
								tstatval &= 0xFD;
							if(row_temp == 3)
								tstatval &= 0xFB ;
							if(row_temp == 4)
								tstatval &= 0xF7;
							if(row_temp == 5)
								tstatval &= 0xEF;
							tstatval |= cellval << (row_temp -1) ;
						}

					}
					if(write_one(g_tstat_id,MODBUS_UNIVERSAL_OUTPUT_BEGIN + pos,tstatval)<0)
						{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
						product_register_value[MODBUS_UNIVERSAL_OUTPUT_BEGIN + pos] = tstatval;
					 }
					 else
					 {
						 totalrows = 6 ;////////////////
						 if((col-5) < (m_PID2_heat_stages+1))
							 pos = (m_PID2_heat_stages+m_PID2_cool_stages+1) - (col-5) ;
						 else
							 pos = (col-5) - (m_PID2_heat_stages+1);
						 m_FlexGrid2.put_Col(col);
						 //tstatval &= 0x3F ;//clear 7 bit and 8 bit  for 0-50
						 tstatval=0;
						 for( row_temp=totalrows;row_temp<=totalrows+1;row_temp++)
						 {
							 m_FlexGrid2.put_Row(row_temp);	
							 if(string2digital(m_FlexGrid2.get_Text(),cellval,ty))
							 {
								 if(cellval==4 )
								 {//for 0-50
									 if(row_temp==totalrows)
										 tstatval |=64;
									 else if(row_temp==(totalrows+1))
										 tstatval |=128;
								 }
								 else
								 {
									 if(row_temp == totalrows)
									 {
										 tstatval &= 0xFC ;
									 }
									 if(row_temp == totalrows+1)
									 {
										 tstatval &= 0xF3 ;
										 cellval <<= 2 ; 
									 }
									 tstatval |= cellval  ;
								 }
							 }

						 }
						 //

						 if(write_one(g_tstat_id,MODBUS_UNIVERSAL_VALVE_BEGIN+pos,tstatval)<0)//没找到对应的值。
						 {MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
						 product_register_value[MODBUS_UNIVERSAL_VALVE_BEGIN+pos]=tstatval;
					 
					 }
			

					 
						 
							
					 
						}
					else
						{

							if((col-5) < (m_PID2_heat_stages+1))
							pos = (m_PID2_heat_stages+m_PID2_cool_stages+1) - (col-5) ;
						else
							pos = (col-5) - (m_PID2_heat_stages+1);
						m_FlexGrid2.put_Col(col);////////////////////////////////////////////////////////


						if (m_nCurRow<=5)
						{
						for( row_temp = 1;row_temp<=totalrows;row_temp++)//***********************row_temp==totalrows+1
						{
							m_FlexGrid2.put_Row(row_temp);////////////////////////////////////////////////////
							if(string2digital(m_FlexGrid2.get_Text(),cellval,ty))
							{
								if(row_temp == 1)
									tstatval &= 0xFE;
								if(row_temp == 2)
									tstatval &= 0xFD;
								if(row_temp == 3)
									tstatval &= 0xFB ;
								if(row_temp == 4)
									tstatval &= 0xF7;
								if(row_temp == 5)
									tstatval &= 0xEF;
								tstatval |= cellval << (row_temp -1) ;
							}

						}
						if(write_one(g_tstat_id,MODBUS_UNIVERSAL_OUTPUT_BEGIN + pos,tstatval)<0)
							{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
							product_register_value[MODBUS_UNIVERSAL_OUTPUT_BEGIN + pos] = tstatval;
						}
						else
						{
						  if (product_register_value[MODBUS_FAN_SPEED]==0)
						  {totalrows = 6 ;////////////////
							if((col-5) < (m_PID2_heat_stages+1))
								pos = (m_PID2_heat_stages+m_PID2_cool_stages+1) - (col-5) ;
							else
								pos = (col-5) - (m_PID2_heat_stages+1);
							m_FlexGrid2.put_Col(col);
							//tstatval &= 0x3F ;//clear 7 bit and 8 bit  for 0-50
							tstatval=0;
							for( row_temp=totalrows;row_temp<=totalrows+1;row_temp++)
							{
								m_FlexGrid2.put_Row(row_temp);	
								if(string2digital(m_FlexGrid2.get_Text(),cellval,ty))
								{
									if(cellval==4 )
									{//for 0-50
										if(row_temp==totalrows)
											tstatval |=64;
										else if(row_temp==(totalrows+1))
											tstatval |=128;
									}
									else
									{
										if(row_temp == totalrows)
										{
											tstatval &= 0xFC ;
										}
										if(row_temp == totalrows+1)
										{
											tstatval &= 0xF3 ;
											cellval <<= 2 ; 
										}
										tstatval |= cellval  ;
									}
								}

							}
							//

							if(write_one(g_tstat_id,MODBUS_UNIVERSAL_OFF_VALVE_BEGIN+pos,tstatval)<0)//没找到对应的值。
							{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);return;}
							product_register_value[MODBUS_UNIVERSAL_OFF_VALVE_BEGIN+pos]=tstatval;
						  }
							
						}

			

						 
							 

						 
						}
			   

				
			}
		}
	}
	else
	{
		if(g_OutPutLevel==1)
			return;
		unsigned char tstatval=0,pos,cellval=0;
		int row_temp,totalrows;
		bool ty;
		if(m_nmoduleType == 1)//5a
			totalrows = 3;
		else
			totalrows = 5 ;
		if(col==4)
		{//pid
			CString str2;
			 
			{
				if(bflexgrid1_or_2==false)
					str2=m_FlexGrid1.get_TextMatrix(row,col);
				else
					str2=m_FlexGrid2.get_TextMatrix(row,col);
				if(str2.CompareNoCase(_T("PID1"))==0)
					write_one(g_tstat_id,MODBUS_PID_OUTPUT1+row-1,0);
				else if(str2.CompareNoCase(_T("PID2"))==0)
					write_one(g_tstat_id,MODBUS_PID_OUTPUT1+row-1,1);
				else if(str2.CompareNoCase(_T("MAX(PID1,PID2)"))==0)
					write_one(g_tstat_id,MODBUS_PID_OUTPUT1+row-1,2);
				else if(str2.CompareNoCase(_T("MIN(PID1,PID2)"))==0)
					write_one(g_tstat_id,MODBUS_PID_OUTPUT1+row-1,3);
			}
			FreshGrids();//lsc add

		}
		else if(col==5)
		{
			CString str2;
			if(bflexgrid1_or_2==false)
				str2=m_FlexGrid1.get_TextMatrix(row,col);
			else
				str2=m_FlexGrid2.get_TextMatrix(row,col);
			if((str2.CompareNoCase(_T("On"))==0)||str2==_T("      -"))
				{write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,0);
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1]=0;}
			else if(str2.CompareNoCase(_T("DI1"))==0)
				{write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,1);
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1]=1;}
			else if((str2.CompareNoCase(_T("AI1"))==0)||str2.CompareNoCase(_T("//AI1"))==0)

				{write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,2);
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1]=2;}
			else if((str2.CompareNoCase(_T("AI2"))==0)||str2.CompareNoCase(_T("//AI2"))==0)
				{write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,3);
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1]=3;}
			else if(str2.CompareNoCase(_T("Timer OR"))==0)
				{write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,4);
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1]=4;}
			else if(str2.CompareNoCase(_T("Timer And"))==0)
				{write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,5);
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1]=5;}
			else if(str2.CompareNoCase(_T("InterLock Timer"))==0)
				{write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,6);
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1]=6;}
			else if(str2.CompareNoCase(_T("FreeCool"))==0)
				{write_one(g_tstat_id,MODBUS_INTERLOCK_OUTPUT1+row-1,7);
				product_register_value[MODBUS_INTERLOCK_OUTPUT1+row-1]=7;}

		}
		CString strItem;

		if(col>5)
		{

			//	283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
			//	284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM



			// 		int output3_value=multi_register_value[283];//2 rows plug in one row//out 4
			// 		int output4_value=multi_register_value[284];//out 5;


			int output3_value;//=multi_register_value[283];//2 rows plug in one row//out 4
			int output4_value;//=multi_register_value[284];//out 5;

			 
				output3_value=product_register_value[MODBUS_MODE_OUTPUT4];//2 rows plug in one row//out 4
				output4_value=product_register_value[MODBUS_MODE_OUTPUT4];//out 5;
			 
			BOOL bFloat=FALSE;
			BOOL bONOFF=TRUE;

			if(bflexgrid1_or_2==false)
			{//grid one 1

				if((col-5) < (m_PID1_heat_stages+1))
					pos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - (col-5) ;
				else
					pos = (col-5) - (m_PID1_heat_stages+1);
				m_FlexGrid1.put_Col(col);

				for( row_temp = 1;row_temp<=totalrows;row_temp++)//***********************row_temp==totalrows+1
				{
					m_FlexGrid1.put_Row(row_temp);	
					strItem=m_FlexGrid1.get_TextMatrix(row_temp,col);
					if(string2digital(strItem,cellval,ty))
					{
						if(row_temp == 1)
							tstatval &= 0xFE;
						if(row_temp == 2)
							tstatval &= 0xFD;
						if(row_temp == 3)
							tstatval &= 0xFB ;
						if(!m_bFloat)//not floater out 4 and out5
						{
							if(row_temp == 4&&!m_bOut4PWM)
								tstatval &= 0xF7;
							if(row_temp == 5&&!m_bOut5PWM)
								tstatval &= 0xEF;
						}
						else
						{
							if(row_temp==4)
								break;
						}

						tstatval |= cellval << (row_temp -1) ;
					}
				}
				int n=get_real_fan_select() ;
				//138	288	1	Low byte	W/R	FAN0_OPERATION_TABLE_COAST
				 
					write_one(g_tstat_id,MODBUS_FAN0_OPER_TABLE_COAST+get_real_fan_select() * 7 + pos,tstatval);		
					product_register_value[MODBUS_FAN0_OPER_TABLE_COAST+get_real_fan_select() * 7] = tstatval;
				 

				//if(m_bFloat)
				//{
				//	int nValue=0;
				//	if(row==4&&output3_value==1)
				//	{
				//		strItem=m_FlexGrid1.get_TextMatrix(row,col);
				//		if(string2digital(strItem,cellval,ty) )
				//		{
				//			tstatval=tstatval&0xCF;
				//			nValue= cellval << 4 ;
				//			tstatval|=nValue;
				//		}
				//		//351	334	1	Low byte	W/R	analog output in OFF table, coating mode,bit1 means AO1 : 1 = on, 0 = off    bit3 means AO2 : 1 = on, 0 = off
				//		//173	323	1	Low byte	W/R	VALVE_OPER_TABLE_COAST, Analog output state for each of the 7 modes of operation

				//		if(m_fan.GetCurSel()==0)
				//		{
				//			 
				//			if (m_pids == 1)
				//			{
				//				write_one(g_tstat_id,385+pos,tstatval);//lsc add ,目前只有当第一个选择为PID2，下面的才写到这个寄存器中
				//			}else
				//			{
				//				write_one(g_tstat_id,351+pos,tstatval);
				//			}
				//		}
				//		else
				//		{
				//			//if (newtstat6[7] == 6)
				//			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
				//			{
				//				write_one(g_tstat_id,323+pos,tstatval);
				//				product_register_value[323+pos] = tstatval;
				//			}
				//			else
				//			{
				//				write_one(g_tstat_id,173+pos,tstatval);
				//			}
				//		}
				//	}
				//	if(row==5&&output4_value==1)
				//	{
				//		strItem=m_FlexGrid1.get_TextMatrix(row,col);
				//		if(string2digital(strItem,cellval,ty) )
				//		{
				//			tstatval=tstatval&0xCF;
				//			nValue= cellval << 4 ;
				//			tstatval|=nValue;
				//		}
				//		if(m_fan.GetCurSel()==0)
				//			// 						write_one(g_tstat_id,351+pos,tstatval);
				//			// 					else
				//			// 						write_one(g_tstat_id,173+pos,tstatval);

				//		{
				//			//if (newtstat6[7] == 6)
				//			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
				//			{
				//				write_one(g_tstat_id,334+pos,tstatval);
				//				product_register_value[334+pos] = tstatval;
				//			}
				//			else
				//			{
				//				write_one(g_tstat_id,351+pos,tstatval);
				//			}
				//		}
				//		else
				//		{
				//			//if (newtstat6[7] == 6)
				//			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
				//			{
				//				write_one(g_tstat_id,323+pos,tstatval);
				//				product_register_value[323+pos]= tstatval;
				//			}
				//			else
				//			{
				//				write_one(g_tstat_id,173+pos,tstatval);
				//			}
				//		}
				//	}
				//}

				if(m_fan.GetCurSel()==0)
				{
					//if (newtstat6[7] == 6)
					 
						tstatval=(unsigned char)product_register_value[MODBUS_VALVE_OFF_TABLE_COAST+pos];
					 
				}
				else
				{
					//if (newtstat6[7] == 6)
					 
						tstatval=(unsigned char)product_register_value[MODBUS_VALVE_OPERATION_TABLE_BEGIN+pos];
					 
				}
				// 				tstatval=multi_register_value[351+pos];
				// 			else
				// 				tstatval=multi_register_value[173+pos];

				if(m_nmoduleType == 1 || m_nmoduleType == 3)
				{
					if(m_nmoduleType == 1)
						totalrows = 4 ;////////////////
					else
						totalrows = 6 ;////////////////
					if((col-5) < (m_PID1_heat_stages+1))
						pos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - (col-5) ;
					else
						pos = (col-5) - (m_PID1_heat_stages+1);
					m_FlexGrid1.put_Col(col);
					tstatval &= 0x3F ;//clear 7 bit and 8 bit  for 0-50

					for( row_temp=totalrows;row_temp<=totalrows+1;row_temp++)
					{
						CString strItemText=m_FlexGrid1.get_TextMatrix(row_temp,col);
						//	m_FlexGrid1.put_Row(row_temp);
						string2digital(strItemText,cellval,ty)	;
						//	if()
						//	{						
						if(cellval==4 )
						{//for 0-50
							if(row_temp==totalrows)
								tstatval |=64;//第6位
							else if(row_temp==(totalrows+1))//0-50
								tstatval |=128;//第7位
						}
						else
						{
							if(row_temp == totalrows)
							{
								tstatval &= 0xFC ;//1111 1100
							}
							if(row_temp == totalrows+1)
							{
								tstatval &= 0xF3 ;//1111 0011
								cellval <<= 2 ; 
							}
							tstatval |= cellval  ;
						}						
						//	}
					}
					int ret = 0;
					if(m_fan.GetCurSel()==0)
						// 					write_one(g_tstat_id,351+pos,tstatval);
						// 				else
						// 					write_one(g_tstat_id,173+pos,tstatval);
					{
						//if (newtstat6[7] == 6)
						if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
						{
							write_one(g_tstat_id,334+pos,tstatval);
							product_register_value[334+pos] = tstatval;
						}
						else
						{
							ret = write_one(g_tstat_id,351+pos,tstatval);
							// 						if (ret<=0)
							// 							AfxMessageBox(_T("setting failure!"));

						}
					}
					else
					{
						//if (newtstat6[7] == 6)
						if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
						{
							write_one(g_tstat_id,323+pos,tstatval);
							product_register_value[323+pos] = tstatval;
						}
						else
						{
							ret = write_one(g_tstat_id,173+pos,tstatval);
							// 						if (ret<=0)
							// 							AfxMessageBox(_T("setting failure!"));
						}
					}

				}
			}

			
			else//grid 2:
			{
				int ret1= 0;
				if((col-5) < (m_PID2_heat_stages+1))
					pos = (m_PID2_heat_stages+m_PID2_cool_stages+1) - (col-5) ;
				else
					pos = (col-5) - (m_PID2_heat_stages+1);
				m_FlexGrid2.put_Col(col);////////////////////////////////////////////////////////
				for( row_temp = 1;row_temp<=totalrows;row_temp++)//***********************row_temp==totalrows+1
				{
					m_FlexGrid2.put_Row(row_temp);////////////////////////////////////////////////////
					if(string2digital(m_FlexGrid2.get_Text(),cellval,ty))
					{
						if(row_temp == 1)
							tstatval &= 0xFE;
						if(row_temp == 2)
							tstatval &= 0xFD;
						if(row_temp == 3)
							tstatval &= 0xFB ;
						if(row_temp == 4)
							tstatval &= 0xF7;
						if(row_temp == 5)
							tstatval &= 0xEF;
						tstatval |= cellval << (row_temp -1) ;
					}

				}
				//254	281	1	Low byte	W/R	PID2 Output table- Coasting

				
				if (m_nmoduleType==3)
				{
					if(m_fan.GetCurSel()==0)
					{
						ret1 = write_one(g_tstat_id,526 + pos,tstatval);
						// 					if (ret1<=0)
						// 						AfxMessageBox(_T("setting failure!"));
					}else
					{
						ret1 = write_one(g_tstat_id,254 + pos,tstatval);
						// 					if (ret1<=0)
						// 						AfxMessageBox(_T("setting failure!"));
					}
				}
				else
				{
					write_one(g_tstat_id,254 + pos,tstatval);
				}

				if(m_nmoduleType == 1 || m_nmoduleType == 3)
				{
					if(m_nmoduleType == 1)
						totalrows = 4 ;////////////////
					else
						totalrows = 6 ;////////////////
				if((col-5) < (m_PID2_heat_stages+1))
						pos = (m_PID2_heat_stages+m_PID2_cool_stages+1) - (col-5) ;
					else
						pos = (col-5) - (m_PID2_heat_stages+1);
					m_FlexGrid2.put_Col(col);
					//tstatval &= 0x3F ;//clear 7 bit and 8 bit  for 0-50
					tstatval=0;
					for( row_temp=totalrows;row_temp<=totalrows+1;row_temp++)
					{
						m_FlexGrid2.put_Row(row_temp);	
						if(string2digital(m_FlexGrid2.get_Text(),cellval,ty))
						{
							if(cellval==4 )
							{//for 0-50
								if(row_temp==totalrows)
									tstatval |=64;
								else if(row_temp==(totalrows+1))
									tstatval |=128;
							}
							else
							{
								if(row_temp == totalrows)
								{
									tstatval &= 0xFC ;
								}
								if(row_temp == totalrows+1)
								{
									tstatval &= 0xF3 ;
									cellval <<= 2 ; 
								}
								tstatval |= cellval  ;
							}
						}

					}
				 


					if(m_fan.GetCurSel()==0)
					{
						//write_one(g_tstat_id,351+pos,tstatval);

						int ret392 = 0;
						ret392 = write_one(g_tstat_id,533+pos,tstatval);//2.5.0.93
						if (ret392>0)
						{
							product_register_value[533+pos] = tstatval;
						}else
						{
							//AfxMessageBox(_T("setting failure!"));
						}

					}
					else
					{
						//write_one(g_tstat_id,173+pos,tstatval);
						int ret392 = 0;
						ret392 = write_one(g_tstat_id,261+pos,tstatval);//2.5.0.93
						if (ret392>0)
						{
							product_register_value[261+pos] = tstatval;
						}else
						{
							//AfxMessageBox(_T("setting failure!"));
						}

					}

				}
			}
		}
		FreshGrids();//2.5.0.98
	}
}

BOOL COutPutDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN  )
	{		
		if(pMsg->wParam == VK_RETURN )
		{
			if(::GetFocus()==m_ItemValueCombx.GetSafeHwnd())
			{
				m_ItemValueCombx.ShowWindow(SW_HIDE);
			}
		GetDlgItem(IDCANCEL)->SetFocus();
		return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void COutPutDlg::OnCbnSelchangeValueitemcombo()
{
#if 1
 	if(g_OutPutLevel==1)
		return;
	long lRow,lCol,nCounts;
	CString strOldText;
	CString strNewText;
	if(!m_bflexgrid1_or_2)
	{
		lRow = m_FlexGrid1.get_RowSel();//获取点击的行号	
		lCol = m_FlexGrid1.get_ColSel(); //获取点击的列号
		nCounts=m_FlexGrid1.get_Rows();

		strOldText=m_FlexGrid1.get_TextMatrix(lRow,lCol);

		if(strOldText.IsEmpty())
			return;
		//m_ItemValueCombx.GetWindowText(strNewText);	//Marked by Fance ,no use.
		int nItem=m_ItemValueCombx.GetCurSel();
		if(nItem<0)
			return;
		m_ItemValueCombx.GetLBText(nItem,strNewText);
		m_FlexGrid1.put_TextMatrix(lRow,lCol,strNewText);
		OnWrite(MKBOOL(m_bflexgrid1_or_2),lCol,lRow);	
	
	}
	else//grid 2
	{
		lRow = m_FlexGrid2.get_RowSel();//获取点击的行号	
		lCol = m_FlexGrid2.get_ColSel(); //获取点击的列号
		nCounts=m_FlexGrid2.get_Rows();

		strOldText=m_FlexGrid2.get_TextMatrix(lRow,lCol);
		if(strOldText.IsEmpty())
			return;
		m_ItemValueCombx.GetWindowText(strNewText);
		int nItem=m_ItemValueCombx.GetCurSel();
		if(nItem<0)
			return;
		m_ItemValueCombx.GetLBText(nItem,strNewText);
		m_FlexGrid2.put_TextMatrix(lRow,lCol,strNewText);
		OnWrite(MKBOOL(m_bflexgrid1_or_2),lCol,lRow);



	}
	m_ItemValueCombx.ShowWindow(SW_HIDE);
	//float are realized in onwrite:
	
		BYTE nreg;
//		BYTE ntemp;
		if(lRow==4 && m_bOut4PWM&&lCol>=6&&!m_bflexgrid1_or_2)//	if (lCol==6||lCol==7||lCol==8||lCol==9||lCol==10||lCol==11||lCol==12)
		{
			int CoastCol=m_PID1_heat_stages+6;
			int nPos;
		//	nPos=m_PID1_cool_stages;
		//	CString strValue;
		//	m_ItemValueCombx.GetWindowText(strValue);
		//	int nValue=atoi(strValue);
			int nValue=m_ItemValueCombx.GetCurSel();


			int nCol= lCol-5;	
				
			if(nCol < (m_PID1_heat_stages+1))
				nPos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - nCol ;
			else
				nPos = nCol - (m_PID1_heat_stages+1);

			//nreg=read_one(tstat_id,341+nPos);
			nreg=(BYTE)multi_register_value[341+nPos];//341TSTAT6找不到对应
			nreg=nreg&0x0f;
			nValue=nValue<<4;
			nValue=nreg|nValue;
			write_one(g_tstat_id,341+nPos,nValue);
			m_FlexGrid1.put_TextMatrix(lRow,lCol,strNewText);
			m_FlexGrid1.SetFocus();
			//FreshGrids();//lsc0928
			return;
		}
		if(lRow==5 && m_bOut5PWM&&lCol>=6&&!m_bflexgrid1_or_2)
		{
			int CoastCol=m_PID1_heat_stages+6;
			int nPos;
		//	CString strValue;
		//	m_ItemValueCombx.GetWindowText(strValue);

			int nValue=m_ItemValueCombx.GetCurSel();

			//int nCol= m_FlexGrid1.get_ColSel()-5;
			int nCol= lCol-5;
			if(nCol < (m_PID1_heat_stages+1))
				nPos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - nCol ;
			else
				nPos = nCol - (m_PID1_heat_stages+1);
		//	nreg=read_one(tstat_id,341+nPos);
			nreg=(BYTE)multi_register_value[341+nPos];
			nreg=nreg&0xf0;
			nValue=nreg|nValue;

			write_one(g_tstat_id,341+nPos,nValue);
			m_FlexGrid1.put_TextMatrix(lRow,lCol,strNewText);
			m_FlexGrid1.SetFocus();
			//FreshGrids();//lsc0928
			return;
		}

		//       ///FREE COOL:
		// 			286	245	1	Low byte	W/R	Interlock for  output1
		// 			287	246	1	Low byte	W/R	Interlock for  output2
		// 			288	247	1	Low byte	W/R	Interlock for  output3
		// 			289	248	1	Low byte	W/R	Interlock for  output4
		// 			290	249	1	Low byte	W/R	Interlock for  output5
		// 			291	250	1	Low byte	W/R	Interlock for  output6
		// 			292	251	1	Low byte	W/R	Interlock for  output7
		//
	int nOutReg;
	int	nValue;

	//if (newtstat6[7] ==6)
	 
		nOutReg = MODBUS_INTERLOCK_OUTPUT1 + lRow-1;
		nValue=product_register_value[nOutReg];

 
	
	//int	nValue=multi_register_value[nOutReg];
	if(nValue==7&&lCol>=6&&!m_bflexgrid1_or_2)
	{

		int nPos;
		//CString strValue;
		//m_ItemValueCombx.GetWindowText(strValue);
		int nValue=m_ItemValueCombx.GetCurSel();

		lCol=lCol-5;
		if(lCol < (m_PID1_heat_stages+1))
			nPos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - lCol ;
		else
			nPos = lCol - (m_PID1_heat_stages+1);	
		//362	125	1	Low byte	W/R	ANALOG INPUT4 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON

		//if (newtstat6[7] == 6)
		if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
		{
			if(m_fan.GetCurSel()==0)
			{
				int ndata;
				nValue=nValue<<4;
				int nReg=362+nPos;
				ndata=product_register_value[125+nPos];
				if(ndata>=0)
				{
					ndata=ndata&0x0F;//清掉4-7位
					nValue=nValue|ndata;
					write_one(g_tstat_id,125+nPos,nValue);
					product_register_value[125] = nValue;

					m_ItemValueCombx.GetWindowText(strNewText);
					//int nItemwww=m_ItemValueCombx.GetCurSel();
					m_FlexGrid1.put_TextMatrix(lRow,lCol,strNewText);
				}
			}
			else//fan speed is not off.
			{
				int ndata;
				int nReg=125+nPos;
				ndata=multi_register_value[125+nPos];
				ndata=ndata&0xF0;
				if(ndata>=0)
				{
					nValue=nValue|ndata;
					write_one(g_tstat_id,125+nPos,nValue);
					product_register_value[125] = nValue;
				}
			}
		}else
		{
		if(m_fan.GetCurSel()==0)
		{
			int ndata;
			nValue=nValue<<4;
			int nReg=362+nPos;
			ndata=multi_register_value[362+nPos];
			if(ndata>=0)
			{
				ndata=ndata&0x0F;//清掉4-7位
				nValue=nValue|ndata;
				write_one(g_tstat_id,362+nPos,nValue);

				m_ItemValueCombx.GetWindowText(strNewText);
				//int nItemwww=m_ItemValueCombx.GetCurSel();
				m_FlexGrid1.put_TextMatrix(lRow,lCol,strNewText);
			}
		}
		else//fan speed is not off.
		{
			int ndata;
			int nReg=362+nPos;
			ndata=multi_register_value[362+nPos];
			ndata=ndata&0xF0;
			if(ndata>=0)
			{
				nValue=nValue|ndata;
				write_one(g_tstat_id,362+nPos,nValue);
			}
		}
		}

		m_FlexGrid1.SetFocus();
	//	FreshGrids();//lsc0928
		return;
	}
#endif

}

void COutPutDlg::ClickMsflexgrid2()
{

	//pid2lrow = m_FlexGrid2.get_RowSel();//2.5.0.94
	if(g_OutPutLevel==1)
		return;
	m_bflexgrid1_or_2=true;//flexgrid2 is selected
	long lRow,lCol;
	lRow = m_FlexGrid2.get_RowSel();//获取点击的行号	
	pid2lrow = lRow;
	lCol = m_FlexGrid2.get_ColSel(); //获取点击的列号
	
	if((lCol==1 || lCol==2) && get_curtstat_version()<26)
		return;
	if(lRow>m_FlexGrid2.get_Rows()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow == 0) //如果点击标题行，也无效
		return;
	if(FLEXGRID_CELL_COLOR==m_FlexGrid2.get_CellBackColor())
		return;
	CRect rect;
	m_FlexGrid2.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	// MSFlexGrid控件的函数的长度单位是"缇(twips)"，
	//需要将其转化为像素，1440缇= 1英寸
	CDC* pDC =GetDC();
	//计算象素点和缇的转换比例
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_FlexGrid2.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_FlexGrid2.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_FlexGrid2.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_FlexGrid2.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rc.OffsetRect(rect.left+1,rect.top+1);
	//获取选中格的文本信息
	CString strValue = m_FlexGrid2.get_TextMatrix(lRow,lCol);
	m_nCurRow=lRow;
	m_nCurCol=lCol;
	if (lCol==1)
	{
		/*
		m_DescriptEdt.MoveWindow(rc);
		m_DescriptEdt.ShowWindow(SW_SHOW);
		m_DescriptEdt.SetFocus();
		*/

	}

	if(lCol==4)
	{
// 		m_ItemValueCombx.ResetContent();
// 		m_ItemValueCombx.AddString(_T("PID1"));
// 		m_ItemValueCombx.AddString(_T("PID2"));
// 		m_ItemValueCombx.AddString(_T("MAX(PID1,PID2)"));
// 		m_ItemValueCombx.AddString(_T("MIN(PID1,PID2)"));
// 		m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
// 		m_ItemValueCombx.BringWindowToTop();
// 		m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
// 		m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
// 		m_ItemValueCombx.SetFocus(); //获取焦点


		m_ItemValueCombx.ResetContent();
		m_ItemValueCombx.AddString(_T("PID1"));
		m_ItemValueCombx.AddString(_T("PID2"));
		m_ItemValueCombx.AddString(_T("MAX(PID1,PID2)"));
		m_ItemValueCombx.AddString(_T("MIN(PID1,PID2)"));
		m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
		m_ItemValueCombx.BringWindowToTop();
		m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
		m_ItemValueCombx.SetFocus(); //获取焦点
	}
	 if(lCol==5)
	{
		m_ItemValueCombx.ResetContent();
		m_ItemValueCombx.AddString(_T("On"));
		m_ItemValueCombx.AddString(_T("DI1"));
		if(product_register_value[MODBUS_ANALOG_IN1]==3)//on/off mode
			m_ItemValueCombx.AddString(_T("AI1"));
		else
			m_ItemValueCombx.AddString(_T("//AI1"));
		if(multi_register_value[189]==3)//on/off
			m_ItemValueCombx.AddString(_T("AI2"));
		else
			m_ItemValueCombx.AddString(_T("//AI2"));
		m_ItemValueCombx.AddString(_T("Timer Or"));
		m_ItemValueCombx.AddString(_T("Timer And"));
		m_ItemValueCombx.AddString(_T("InterLock Timer"));

		m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
		m_ItemValueCombx.BringWindowToTop();
		m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
		m_ItemValueCombx.SetFocus(); //获取焦点
	}
	if (lCol>5)
	{
		//m_ItemValueCombx.ResetContent();
		//m_ItemValueCombx.AddString(_T("Off"));
		//m_ItemValueCombx.AddString(_T("On"));
		//m_ItemValueCombx.AddString(_T("Close"));
		//m_ItemValueCombx.AddString(_T("Open"));
		//m_ItemValueCombx.AddString(_T("0-100"));
		//m_ItemValueCombx.AddString(_T("50-100"));
		//m_ItemValueCombx.AddString(_T("0-50"));
		//m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
		//m_ItemValueCombx.BringWindowToTop();
		//m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
		//m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
		//m_ItemValueCombx.SetFocus(); //获取焦点
		#if 1
		//if(nValue==7&&(!m_bOut4PWM)&&(!m_bOut5PWM))
		//if((nValue==7&&lRow<4)||(lRow==4 && !m_bOut4PWM&&nValue==7)||(lRow==5 && !m_bOut5PWM&&nValue==7))
	int nOutReg,nValue;

	//if (newtstat6[7] == 6)
	if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
		{
		nOutReg=245+m_nCurRow-1;
		nValue=product_register_value[nOutReg];
		}else
		{
		nOutReg=286+m_nCurRow-1;
		nValue=multi_register_value[nOutReg];
		}
		if((nValue==7&&lRow<4)||(lRow==4 && !m_bOut4PWM&&nValue==7)||(lRow==5 && !m_bOut5PWM&&nValue==7)||(nValue==7&&lRow>5))//20121008
		{
			m_ItemValueCombx.ResetContent();
			m_ItemValueCombx.AddString(_T("0%"));
			m_ItemValueCombx.AddString(_T("100%"));
			m_ItemValueCombx.AddString(_T("MIN->100%"));
			m_ItemValueCombx.AddString(_T("MIN|100%"));
			m_ItemValueCombx.AddString(_T("MIN"));
			m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
			m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_ItemValueCombx.BringWindowToTop();
			m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
			m_ItemValueCombx.SetFocus(); //获取焦点
		}
	//	else if((lRow<4)||(lRow==4 && !m_bOut4PWM)||(lRow==5 && !m_bOut5PWM))
		else if((lRow<4)||(lRow==4 && !m_bOut4PWM)||(lRow==5 && !m_bOut5PWM)/*||(lRow>5)*/)//20121008
		{		
		    if (m_bFloat)
		    {
				m_ItemValueCombx.ResetContent();
			/*m_ItemValueCombx.AddString(_T("Off"));
			m_ItemValueCombx.AddString(_T("On"));*/
	 		m_ItemValueCombx.AddString(_T("Close"));
			m_ItemValueCombx.AddString(_T("Open"));
			m_ItemValueCombx.AddString(_T("0-100"));
			m_ItemValueCombx.AddString(_T("50-100"));
			m_ItemValueCombx.AddString(_T("0-50")); 
			m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
			m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_ItemValueCombx.BringWindowToTop();
			m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
			m_ItemValueCombx.SetFocus(); //获取焦点
		    } 
		    else
		    {m_ItemValueCombx.ResetContent();
			m_ItemValueCombx.AddString(_T("Off"));
			m_ItemValueCombx.AddString(_T("On"));
	/*		m_ItemValueCombx.AddString(_T("Close"));
			m_ItemValueCombx.AddString(_T("Open"));
			m_ItemValueCombx.AddString(_T("0-100"));
			m_ItemValueCombx.AddString(_T("50-100"));
			m_ItemValueCombx.AddString(_T("0-50"));*/
			m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
			m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_ItemValueCombx.BringWindowToTop();
			m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
			m_ItemValueCombx.SetFocus(); //获取焦点
		    }
			
		}
		else if (lRow>5)
		{
		   if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
			 {
			 if (lRow==6)
				 {
				 if (product_register_value[207]==0)
					 {
				m_ItemValueCombx.ResetContent();
			m_ItemValueCombx.AddString(_T("Off"));
			m_ItemValueCombx.AddString(_T("On")); 
			/*m_ItemValueCombx.AddString(_T("Close"));
			m_ItemValueCombx.AddString(_T("Open"));
			m_ItemValueCombx.AddString(_T("0-100"));
			m_ItemValueCombx.AddString(_T("50-100"));
			m_ItemValueCombx.AddString(_T("0-50"));*/
			m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
			m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_ItemValueCombx.BringWindowToTop();
			m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
			m_ItemValueCombx.SetFocus(); //获取焦点
				} 
				else
				{
				  m_ItemValueCombx.ResetContent();
			/*m_ItemValueCombx.AddString(_T("Off"));
			m_ItemValueCombx.AddString(_T("On"));*/
			m_ItemValueCombx.AddString(_T("Close"));
			m_ItemValueCombx.AddString(_T("Open"));
			m_ItemValueCombx.AddString(_T("0-100"));
			m_ItemValueCombx.AddString(_T("50-100"));
			m_ItemValueCombx.AddString(_T("0-50"));
			m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
			m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_ItemValueCombx.BringWindowToTop();
			m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
			m_ItemValueCombx.SetFocus(); //获取焦点
				}
			    } 
			    else if (lRow==7)//lRow==7
			    {
				  if (product_register_value[208]==0)
				  {
				  m_ItemValueCombx.ResetContent();
			m_ItemValueCombx.AddString(_T("Off"));
			m_ItemValueCombx.AddString(_T("On")); 
			 
			m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
			m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_ItemValueCombx.BringWindowToTop();
			m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
			m_ItemValueCombx.SetFocus(); //获取焦点
				  } 
				  else
				  {
				    m_ItemValueCombx.ResetContent();
			 
			m_ItemValueCombx.AddString(_T("Close"));
			m_ItemValueCombx.AddString(_T("Open"));
			m_ItemValueCombx.AddString(_T("0-100"));
			m_ItemValueCombx.AddString(_T("50-100"));
			m_ItemValueCombx.AddString(_T("0-50"));
			m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
			m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_ItemValueCombx.BringWindowToTop();
			m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
			m_ItemValueCombx.SetFocus(); //获取焦点
				  }
			    }
			 } 
			 else
			 {
			   
			 if (lRow==6)
				 {
				 if (multi_register_value[186]!=0)
					 {
					 m_ItemValueCombx.ResetContent();
					 m_ItemValueCombx.AddString(_T("Close"));
					 m_ItemValueCombx.AddString(_T("Open"));
					 m_ItemValueCombx.AddString(_T("0-100"));
					 m_ItemValueCombx.AddString(_T("50-100"));
					 m_ItemValueCombx.AddString(_T("0-50"));
					 m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
					 m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
					 m_ItemValueCombx.BringWindowToTop();
					 m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
					 m_ItemValueCombx.SetFocus(); //获取焦点
					 } 
				 else
					 {
					 m_ItemValueCombx.ResetContent();
					 m_ItemValueCombx.AddString(_T("Off"));
					 m_ItemValueCombx.AddString(_T("On"));
					 //m_ItemValueCombx.AddString(_T("0-100"));
					 //m_ItemValueCombx.AddString(_T("50-100"));
					 //m_ItemValueCombx.AddString(_T("0-50"));
					 m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
					 m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
					 m_ItemValueCombx.BringWindowToTop();
					 m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
					 m_ItemValueCombx.SetFocus(); //获取焦点
					 }
				 }
			else if (lRow==7)
			{
			if (multi_register_value[187]!=0)
				{
				m_ItemValueCombx.ResetContent();
				m_ItemValueCombx.AddString(_T("Close"));
				m_ItemValueCombx.AddString(_T("Open"));
				m_ItemValueCombx.AddString(_T("0-100"));
				m_ItemValueCombx.AddString(_T("50-100"));
				m_ItemValueCombx.AddString(_T("0-50"));
				m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
				m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
				m_ItemValueCombx.BringWindowToTop();
				m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
				m_ItemValueCombx.SetFocus(); //获取焦点
				} 
			else
				{
				m_ItemValueCombx.ResetContent();
				m_ItemValueCombx.AddString(_T("Off"));
				m_ItemValueCombx.AddString(_T("On"));
				//m_ItemValueCombx.AddString(_T("0-100"));
				//m_ItemValueCombx.AddString(_T("50-100"));
				//m_ItemValueCombx.AddString(_T("0-50"));
				m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
				m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
				m_ItemValueCombx.BringWindowToTop();
				m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
				m_ItemValueCombx.SetFocus(); //获取焦点
				}
			} 
			else
			{
			}
			 
			
			   
			  
			  
			 }
		}
		 
        #endif


		
	}
	if(lCol>5)
		{   
		if(lRow==4 && m_bOut4PWM)
			{
			m_ItemValueCombx.ResetContent();
			m_ItemValueCombx.AddString(_T("Close"));
			m_ItemValueCombx.AddString(_T("Open"));
			m_ItemValueCombx.AddString(_T("0-100"));
			m_ItemValueCombx.AddString(_T("50-100"));
			m_ItemValueCombx.AddString(_T("0-50"));
			m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
			m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_ItemValueCombx.BringWindowToTop();
			m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
			m_ItemValueCombx.SetFocus(); //获取焦点
			}
		if(lRow==5 &&m_bOut5PWM)
			{
			m_ItemValueCombx.ResetContent();
			m_ItemValueCombx.AddString(_T("Close"));
			m_ItemValueCombx.AddString(_T("Open"));
			m_ItemValueCombx.AddString(_T("0-100"));
			m_ItemValueCombx.AddString(_T("50-100"));
			m_ItemValueCombx.AddString(_T("0-50"));
			m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
			m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_ItemValueCombx.BringWindowToTop();
			m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
			m_ItemValueCombx.SetFocus(); //获取焦点
			}

		}
}

void COutPutDlg::OnEnKillfocusPid2Heatstageedit2()
{
	if(g_OutPutLevel==1)
		return;
	CString strText;
	GetDlgItem(IDC_PID2_HEATSTAGEEDIT2)->GetWindowText(strText);
	m_PID2_heat_stages=_wtoi(strText);

	if(m_PID2_heat_stages+m_PID2_cool_stages>6)
	{

		AfxMessageBox(_T("Too many stages for PID2!"));
		m_PID2_heat_stages = product_register_value[330];
		GetDlgItem(IDC_PID2_HEATSTAGEEDIT2)->SetWindowText(_T("1"));
	}
	else
	{
		//268	330	1	Low byte	W/R	Number of Heating Stages (Max heat+cool = 6)

		//if (newtstat6[7] == PM_TSTAT6)
		 
			int ret=write_one(g_tstat_id,MODBUS_HEAT_UNIVERSAL_TABLE,m_PID2_heat_stages);
			if (ret>0)
			{
			product_register_value[MODBUS_HEAT_UNIVERSAL_TABLE] = m_PID2_heat_stages;
			FreshGrid_PID2tstat6();
			}
			else
			{
			  m_PID2_heat_stages=product_register_value[MODBUS_HEAT_UNIVERSAL_TABLE];
			}
			
		 

	}
}

void COutPutDlg::OnEnKillfocusPid2coolstageedit2()
{
	if(g_OutPutLevel==1)
		return;
	CString strText;
	GetDlgItem(IDC_PID2COOLSTAGEEDIT2)->GetWindowText(strText);
	m_PID2_cool_stages=_wtoi(strText);

	if(m_PID2_heat_stages+m_PID2_cool_stages>6)
	{
		AfxMessageBox(_T("Too many stages for PID2!"));
		m_PID2_cool_stages = product_register_value[331];
		GetDlgItem(IDC_PID2_HEATSTAGEEDIT2)->SetWindowText(_T("1"));
	}
	else
	{
		//269	331	1	Low byte	W/R	Number of Cooling Stages (Max heat + Cool = 6) 

		//if (newtstat6[7] == PM_TSTAT6)
	 
			int ret=write_one(g_tstat_id,MODBUS_COOL_UNIVERSAL_TABLE,m_PID2_cool_stages);
			if (ret>0)
			{
				product_register_value[MODBUS_COOL_UNIVERSAL_TABLE] =m_PID2_cool_stages;
				FreshGrid_PID2tstat6();
			}
			else
			{
			 m_PID2_cool_stages=product_register_value[MODBUS_COOL_UNIVERSAL_TABLE];
			}
			
	 

	}
}

void COutPutDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void COutPutDlg::OnEnKillfocusDescriptedit()
{

	CString strText;
	m_DescriptEdt.GetWindowText(strText);
	m_DescriptEdt.ShowWindow(SW_HIDE);
	//int lRow = m_Output_Grid.get_RowSel();//获取点击的行号	
//	int lCol = m_Output_Grid.get_ColSel(); //获取点击的列号

	CString strInName;
	switch (m_nCurRow)
	{
		case 1:
			strInName=g_strOutName1;
			break;
		case 2:
			strInName=g_strOutName2;
			break;
		case 3:
			strInName=g_strOutName3;
			break;
		case 4:
			strInName=g_strOutName4;
			break;
		case 5:
			strInName=g_strOutName5;
			break;
		case 6:
			strInName=g_strOutName6;
			break;
		case 7:
			strInName=g_strOutName7;
			break;
	}
	if(strText.CompareNoCase(strInName)==0)
		return;
		

	//if(g_serialNum>0&&multi_register_value[6]>0)
	if(multi_register_value[6]>0)
	{
		_ConnectionPtr m_ConTmp;
		_RecordsetPtr m_RsTmp;
		m_ConTmp.CreateInstance("ADODB.Connection");
		m_RsTmp.CreateInstance("ADODB.Recordset");
		m_ConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);

		CString strSerial;
		strSerial.Format(_T("%d"),g_serialNum);

		CString strsql;
		strsql.Format(_T("select * from IONAME where SERIAL_ID = '%s'"),strSerial);
		m_RsTmp->Open((_variant_t)strsql,_variant_t((IDispatch *)m_ConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
		if(VARIANT_FALSE==m_RsTmp->EndOfFile)//update
		{
			
			CString strField;
			switch (m_nCurRow)
			{
				case 1:
					strField=_T("OUTPUT1");
					break;
				case 2:
					strField=_T("OUTPUT2");
					break;
				case 3:
					strField=_T("OUTPUT3");
					break;
				case 4:
					strField=_T("OUTPUT4");
					break;
				case 5:
					strField=_T("OUTPUT5");
					break;
				case 6:
					strField=_T("OUTPUT6");
					break;
				case 7:
					strField=_T("OUTPUT7");
					break;
			}

			try
			{

			CString str_temp;
			str_temp.Format(_T("update IONAME set "+strField+" = '"+strText+"' where SERIAL_ID = '"+strSerial+"'"));

			m_ConTmp->Execute(str_temp.GetString(),NULL,adCmdText);
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
			//m_Output_Grid.put_TextMatrix(lRow,lCol,strText);
			//m_bflexgrid1_or_2=true;//flexgrid2 is selected//
			//	m_nCurRow=lRow;
			//m_nCurCol=lCol;
			if(m_bflexgrid1_or_2)
			{
				m_FlexGrid2.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
			}
			else
			{
				m_FlexGrid1.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
			}

		}
		else//inerst
		{
			switch (m_nCurRow)
			{
				case 1:
					g_strOutName1=strText;
					break;
				case 2:
					g_strOutName2=strText;
					break;
				case 3:
					g_strOutName3=strText;
					break;
				case 4:
					g_strOutName4=strText;
					break;
				case 5:
					g_strOutName5=strText;
					break;
				case 6:
					g_strOutName6=strText;
					break;
				case 7:
					g_strOutName7=strText;
					break;
			}

			CString	str_temp;
			str_temp.Format(_T("insert into IONAME values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')"),
				strSerial,
				g_strInName1,
				g_strInName2,
				g_strInName3,
				g_strInName4,
				g_strInName5,
				g_strInName6,
				g_strInName7,
				g_strOutName1,
				g_strOutName2,
				g_strOutName3,
				g_strOutName4,
				g_strOutName5,
				g_strOutName6,
				g_strOutName7,
				g_strInName8,
				g_strInHumName,
				g_strSensorName);
			try
			{

				m_ConTmp->Execute(str_temp.GetString(),NULL,adCmdText);
			}
			catch(_com_error *e)
			{
				AfxMessageBox(e->ErrorMessage());
			}
			if(m_bflexgrid1_or_2)
			{
				m_FlexGrid2.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
			}
			else
			{
				m_FlexGrid1.put_TextMatrix(m_nCurRow,m_nCurCol,strText);
			}
		}

		switch (m_nCurRow)
		{
			case 1:
				g_strOutName1=strText;
				break;
			case 2:
				g_strOutName2=strText;
				break;
			case 3:
				g_strOutName3=strText;
				break;
			case 4:
				g_strOutName4=strText;
				break;
			case 5:
				g_strOutName5=strText;
				break;
			case 6:
				g_strOutName6=strText;
				break;
			case 7:
				g_strOutName7=strText;
				break;
		}
	
		if(m_RsTmp->State) 
			m_RsTmp->Close(); 
		if(m_ConTmp->State)
			m_ConTmp->Close();	
	}
	
	
}

void COutPutDlg::OnEnSetfocusDescriptedit()
{

}

void COutPutDlg::FreshGrid_PID1tstat6()
{

//	129	107	1	Low byte	W/R	"AUTO_ONLY , enables or disables manual mode. 0 = Manual Fan Modes 1-x Allowed 
//		(depending on R122 value, 1 = Auto Mode Only, 2 = DDC mode,the user can not change setpoint and fan speed from keypad."

	strdemo = _T("2,");
	SetPaneString(2,strdemo);

	//if(newtstat6[107]==0)
	//	m_bFanAutoOnly=FALSE;
	//if(newtstat6[107]==1)
	//	m_bFanAutoOnly=TRUE;


	if(product_register_value[MODBUS_AUTO_ONLY]==0)	//107
		{m_bFanAutoOnly=FALSE;
	if(product_register_value[MODBUS_FAN_SPEED]>=0)	//273
		m_fan.SetCurSel(product_register_value[MODBUS_FAN_SPEED]);	//273
	else
		m_fan.SetCurSel(0);
		}
	else//107
		{m_bFanAutoOnly=TRUE;
	if(product_register_value[MODBUS_FAN_SPEED]==0)	//273
		m_fan.SetCurSel(0);	//273
	else
		m_fan.SetCurSel(1);
		}


	//122	105	1	Low byte	W/R	FAN MODE, number of fan speeds. Single speed = 1 up to three speed fan = 3

	if(product_register_value[MODBUS_FAN_MODE]>0)
		m_fan_mode_ctrl.SetCurSel(product_register_value[MODBUS_FAN_MODE]-1);
	else
		m_fan_mode_ctrl.SetCurSel(0);




	//137	273	1	Low byte	W/R	FAN_SPEED, current operating fan speed   
// 	Relay Output Tables (bit0 = relay1, bit1 = relay2, bit2 = relay3, bit3 = relay4, bit4 = relay5)
// 		"Fan0 table is for the off state. Fan1, Fan2, and Fan3 are for the manual states. 
//		Fan4 is for the Auto state.  These states are controlled by the user."
// 		The mode of operation (coasting, cooling, heating) is determined by the PID parameter.




	//283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
	//284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM


	int output4_value=product_register_value[MODBUS_MODE_OUTPUT4];//205	2 rows plug in one row
	int output5_value=product_register_value[MODBUS_MODE_OUTPUT5];//206
	if((output4_value==1||output5_value==1)||(output4_value==2||output5_value==2))
	{
		m_bFloat=TRUE;
	}

	//336	269	1	Low byte	W/R	Output4 function setting (see above)
	//337	270	1	Low byte	W/R	Output5 function setting (see above)


	//Annul by Fance 20130410
	//int output4_PWMvalue=multi_register_value[MODBUS_OUTPUT4_FUNCTION];//269 //2 rows plug in one row
	//int output5_PWMvalue=multi_register_value[MODBUS_OUTPUT5_FUNCTION];//270

	//int output4_PWMvalue=product_register_value[MODBUS_OUTPUT4_FUNCTION];//269 //2 rows plug in one row
	//int output5_PWMvalue=product_register_value[MODBUS_OUTPUT5_FUNCTION];//270

	//if (output4_PWMvalue==3)
	//{
	//	m_bOut4PWM=TRUE;
	//}
	//if(output5_PWMvalue==3)
	//{
	//	m_bOut5PWM=TRUE;
	//}

	//276	332	1	Low byte	W/R	Number of Heating Stages in Original Table - (Maximum # of total heating and cooling states is 6)
	//277	333	1	Low byte	W/R	Number of Cooling Stages in Original Table - (Maximum # of total heating and cooling states is 6)

	if(product_register_value[MODBUS_MODE_OUTPUT4]==3)
	{
		m_bOut4PWM=TRUE;
	}
	if(product_register_value[MODBUS_MODE_OUTPUT5]==3)
	{
		m_bOut5PWM=TRUE;
	}

#if 1
	CString strTemp;//332 333 这两个寄存器 决定 有几个 阶段。;comments by Fance
	m_PID1_heat_stages=product_register_value[MODBUS_HEAT_ORIGINAL_TABLE];//332 register ,heat stages
	m_PID1_cool_stages=product_register_value[MODBUS_COOL_ORIGINAL_TABLE];//333 register ,cool stages
	if(m_PID1_heat_stages==0&&m_PID1_cool_stages==0)
	{
		//write_one(g_tstat_id,MODBUS_HEAT_ORIGINAL_TABLE,3);//332
		//write_one(g_tstat_id,MODBUS_COOL_ORIGINAL_TABLE,3);//333
		m_PID1_heat_stages=3;
		m_PID1_cool_stages=3;
		if(m_PID1_heat_stages>6)
			m_PID1_heat_stages=6;
		if(m_PID1_cool_stages>6)
			m_PID1_cool_stages=6;
	}
	strTemp.Format(_T("%d"),m_PID1_heat_stages);
	m_PID1_heatEdit.SetWindowText(strTemp);
	strTemp.Format(_T("%d"),m_PID1_cool_stages);
	m_PID1_coolEdit.SetWindowText(strTemp);



 	m_FlexGrid1.put_Rows(8);
 	m_FlexGrid1.put_Cols(m_PID1_heat_stages+m_PID1_cool_stages+7);//interlock control first_blank coast

#endif
	

	m_FlexGrid1.put_TextMatrix(0,1,_T("Description"));
	m_FlexGrid1.put_TextMatrix(0,2,_T("Function"));
	m_FlexGrid1.put_ColWidth(2,0);
	m_FlexGrid1.put_TextMatrix(0,3,_T("Rotation"));
	m_FlexGrid1.put_ColWidth(3,0);
	m_FlexGrid1.put_TextMatrix(0,4,_T("Control"));
	m_FlexGrid1.put_TextMatrix(0,5,_T("InterLock"));
	int i=0;
	for(i=0;i<m_PID1_heat_stages;i++)
		m_FlexGrid1.put_TextMatrix(0,6+i,heat_hand[m_PID1_heat_stages-i-1]);
	m_FlexGrid1.put_TextMatrix(0,m_PID1_heat_stages+6,_T("Coast"));

	for(i=0;i<m_PID1_cool_stages;i++)
		m_FlexGrid1.put_TextMatrix(0,m_PID1_heat_stages+7+i,cool_hand[i]);


#if 1
	m_FlexGrid1.put_TextMatrix(1,1,g_strOutName1);
	m_FlexGrid1.put_TextMatrix(2,1,g_strOutName2);
	m_FlexGrid1.put_TextMatrix(3,1,g_strOutName3);
	m_FlexGrid1.put_TextMatrix(4,1,g_strOutName4);
	m_FlexGrid1.put_TextMatrix(5,1,g_strOutName5);
	m_FlexGrid1.put_TextMatrix(6,1,g_strOutName6);
	m_FlexGrid1.put_TextMatrix(7,1,g_strOutName7);


	m_FlexGrid1.put_TextMatrix(1,0,_T("1"));
	m_FlexGrid1.put_TextMatrix(2,0,_T("2"));
	m_FlexGrid1.put_TextMatrix(3,0,_T("3"));
	m_FlexGrid1.put_TextMatrix(4,0,_T("4"));
	m_FlexGrid1.put_TextMatrix(5,0,_T("5"));
	m_FlexGrid1.put_TextMatrix(6,0,_T("6"));
	m_FlexGrid1.put_TextMatrix(7,0,_T("7"));
	m_FlexGrid1.put_ColAlignment(0,4);
	m_FlexGrid1.put_ColAlignment(1,4);


	

	int totalrows,row;
	totalrows=7;
	int pid_select2[7]={0};
	
		for(i=0;i<7;i++)
			{
			pid_select2[i]=product_register_value[MODBUS_PID_OUTPUT1+i];
			}//274/////////////////////////////get pid select ;col one 1
	
	//new feature:
//	short nRotation;
	short nFunction=product_register_value[MODBUS_OUTPUT1_FUNCTION];//266
	unsigned short interlock[7]={0};

	unsigned short tstatval,pos,nValue;
	for(int col = 1 ;col <=(m_PID1_heat_stages+m_PID1_cool_stages+1);col++)
	{
		if(col < (m_PID1_heat_stages+1))
			pos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - col ;
		else
			pos = col - (m_PID1_heat_stages+1);
		for( row = 1;row<=totalrows-2;row++)//****************************
		{
		/*	if (row==4&&m_bOut4PWM)
			{
				continue;
			}
			if (row==5&&m_bOut5PWM)
			{
				continue;
			}
			if(m_bFloat&&row==4)
				continue;
			if(m_bFloat&&row==5)
				continue;*/
			int nOutReg;

			//Modify  by  Fance  ,this  must be 245= MODBUS_INTERLOCK_OUTPUT1  286 is tstat5 value
			nOutReg=MODBUS_INTERLOCK_OUTPUT1+row-1;//286
			//nOutReg=286+row-1;//286   marked by Fance
			//int nValue=read_one(tstat_id,nOutReg);
			int nValue=product_register_value[nOutReg];
			if(nValue==7)
				continue;

			int nFan=get_real_fan_select();
			//marked 20090511

		//	138	288	1	Low byte	W/R	FAN0_OPERATION_TABLE_COAST

			if(pid_select2[row-1]==2 || pid_select2[row-1]==3)//min(pid1,pid2) or max(pid1,pid2)
				tstatval = product_register_value[MODBUS_FAN0_OPER_TABLE_COAST+4*7 + pos];/////////////////////fan auto
			else
				tstatval = product_register_value[MODBUS_FAN0_OPER_TABLE_COAST+nFan*7 + pos];
			CString str;
			{
				if(digital2string((tstatval >> (row-1)) & 0x01,str,FAN))
					if(pid_select2[row-1]==1)
					FLEX_GRID1_PUT_COLOR_STR(row,col,str)//col +1
					else
					FLEX_GRID1_PUT_STR(row,col,str)//col +1
			}

		}
	}
 
	totalrows = 7 ;		
	for(int col = 1 ;col <=(m_PID1_heat_stages+m_PID1_cool_stages+1);col++)
		{
			if(col < (m_PID1_heat_stages+1))
				pos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - col ;
			else
				pos = col - (m_PID1_heat_stages+1);
			//
			//tstatval = multi_register_value[173+ pos];

			//////////////////////////////////////////////


			//351	334	1	Low byte	W/R	analog output in OFF table, coating mode,bit1 means AO1 : 1 = on, 0 = off    bit3 means AO2 : 1 = on, 0 = off


			if(m_fan.GetCurSel()==0)//a,d,g
				tstatval = product_register_value[MODBUS_VALVE_OFF_TABLE_COAST+ pos];
			else
				//173	323	1	Low byte	W/R	VALVE_OPER_TABLE_COAST, Analog output state for each of the 7 modes of operation

				tstatval = product_register_value[MODBUS_VALVE_OPERATION_TABLE_BEGIN+ pos];	
			///////////////////////////////////////////

			//最后两行
			for(row=totalrows-1;row<=totalrows;row++)//row value
			{
				if (row==4&&m_bOut4PWM)
				{
					continue;
				}
				if (row==5&&m_bOut5PWM)
				{
					continue;
				}
			
				int nOutReg;
				nOutReg=MODBUS_INTERLOCK_OUTPUT1+row-1;
				nValue=product_register_value[nOutReg];
				if(nValue==7)
					continue;


				CString str;
				if(tstatval>63)//
				{
					//
					if(row==(totalrows-1))
					{
						if(tstatval & 64)
						{
							if(digital2string(4,str,VALVE))//for 7 or 8 bit
								if(pid_select2[row-1]==1)
								FLEX_GRID1_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID1_PUT_STR(row,col,str)//col +1
						}
						else
						{ 
							if(digital2string(tstatval & 0x03,str,VALVE))//*** value
							{
								if(pid_select2[row-1]==1)
								FLEX_GRID1_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID1_PUT_STR(row,col,str)//col +1
							}
						}
					}
					if(row==totalrows)
					{
						if(tstatval & 128)
						{
							if(digital2string(4,str,VALVE))//for 7 or 8 bit
							{
								if(pid_select2[row-1]==1)
								FLEX_GRID1_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID1_PUT_STR(row,col,str)//col +1
							}

						}else
						{
							if(digital2string((tstatval >> 2) & 0x03,str,VALVE))//*** value
							{
								if(pid_select2[row-1]==1)
									FLEX_GRID1_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID1_PUT_STR(row,col,str)//col +1
							}
						}
					}
				}
				else
				{
					if(digital2string((tstatval >> ((row-totalrows+1)*2)) & 0x03,str,VALVE))//*** value
						if(pid_select2[row-1]==1)
						FLEX_GRID1_PUT_COLOR_STR(row,col,str)//col +1
						else
						FLEX_GRID1_PUT_STR(row,col,str)//col +1
				}
			}
		}
    
    if (product_register_value[MODBUS_MODE_OUTPUT4]==3)
    {
	m_bOut4PWM=TRUE;
    }
	if (product_register_value[MODBUS_MODE_OUTPUT5]==3)
	{
	m_bOut5PWM=TRUE;
	}
	if(m_bOut4PWM)
	{

		for(int col = 1 ;col <=(m_PID1_heat_stages+m_PID1_cool_stages+1);col++)
		{
			int nValue=0;
			if(col < (m_PID1_heat_stages+1))
				pos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - col ;
			else
				pos = col - (m_PID1_heat_stages+1);			
			tstatval = product_register_value[323+ pos];
			int indext=-1;
			//nValue=read_one(tstat_id,341+pos);
			//nValue=multi_register_value[341+pos];//这项没找到对应项   //Annul by Fance

			nValue=product_register_value[MODBUS_OUTPUT_PWM_AUTO_COAST+pos];//modify by Fance  341  591

			nValue=nValue>>4;
			switch(nValue)
			{
			case 0:
				strTemp=_T("Close");
				break;
			case 1:
				strTemp=_T("Open");
				break;
			case 2:
				strTemp="0-100";
				break;
			case 3:
				strTemp=_T("50-100");
				break;
			case 4:
				strTemp=_T("0-50");
				break;
			default:
				strTemp=_T("");
			}
			if(pid_select2[3]==1)
			FLEX_GRID1_PUT_COLOR_STR(4,col,strTemp)//col +1
			else
			FLEX_GRID1_PUT_STR(4,col,strTemp)//col +1
			
		}
	}	
	if(m_bOut5PWM)
	{
		for(int col = 1 ;col <=(m_PID1_heat_stages+m_PID1_cool_stages+1);col++)
		{
			int nValue=0;
			if(col < (m_PID1_heat_stages+1))
				pos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - col ;
			else
				pos = col - (m_PID1_heat_stages+1);			
			tstatval = product_register_value[323+ pos];
			int indext=-1;

			//nValue=multi_register_value[341+pos];//这项没找到对应项		 //Annul by Fance
			nValue=product_register_value[MODBUS_OUTPUT_PWM_AUTO_COAST+pos];//modify by Fance  341  591

			nValue=nValue&0x0f;
			switch(nValue)
			{
			case 0:
				strTemp=_T("Close");
				break;
			case 1:
				strTemp=_T("Open");
				break;
			case 2:
				strTemp=_T("0-100");
				break;
			case 3:
				strTemp=_T("50-100");
				break;
			case 4:
				strTemp=_T("0-50");
				break;
			default:
				strTemp=_T("");
			}

			if(pid_select2[4]==1)
			FLEX_GRID1_PUT_COLOR_STR(5,col,strTemp)//col +1
			else
			FLEX_GRID1_PUT_STR(5,col,strTemp)//col +1
				//totalrows
		}
	}

	//Free Cool:
#if 1
	for( row = 1;row<=totalrows;row++)//****************************
		//for( row = totalrows-1;row<=totalrows;row++)//****************************
	{
		int nOutReg;
		nOutReg=MODBUS_INTERLOCK_OUTPUT1+row-1;//Modify by fance; 这里应该是 MODBUS_INTERLOCK_OUTPUT1 对应 245  而  286 是TSTAT 5的值
		//nOutReg=286+row-1;
		//			int nValue=read_one(tstat_id,nOutReg);
		int nValue=product_register_value[nOutReg];
		if(nValue!=7)
			continue;
		for(int col = 1 ;col <=(m_PID1_heat_stages+m_PID1_cool_stages+1);col++)
		{
			int nValue=0;
			if(col < (m_PID1_heat_stages+1))
				pos = (m_PID1_heat_stages+m_PID1_cool_stages+1) - col ;
			else
				pos = col - (m_PID1_heat_stages+1);			
			int indext=-1;

			int nReg=362+pos;
			nValue=product_register_value[nReg]; 

			if(m_fan.GetCurSel()==0)
			{
				nValue=(nValue&0xF0)>>4;
				switch(nValue)
				{
				case 0:
					strTemp=_T("0%");
					break;
				case 1:
					strTemp=_T("100%");
					break;
				case 2:
					strTemp=_T("Min->100%");
					break;
				case 3:
					strTemp=_T("Min|100%");
					break;
				case 4:
					strTemp=_T("Min");
					break;
				default:
					strTemp=_T("0%");
				}
				//FLEX_GRID1_PUT_STR(row,col,strTemp)//col +1
				//FLEX_GRID1_PUT_STR(row,col,strTemp);//col +1
				FLEX_GRID1_PUT_STR(row,col,strTemp);//col +1
			}
			else
			{
				nValue=(nValue&0x0F);
				switch(nValue)
				{
				case 0:
					strTemp=_T("0%");
					break;
				case 1:
					strTemp=_T("100%");
					break;
				case 2:
					strTemp=_T("Min->100%");
					break;
				case 3:
					strTemp=_T("Min|100%");
					break;
				case 4:
					strTemp=_T("Min");
					break;
				default:
					strTemp=_T("0%");
				}
				//	FLEX_GRID1_PUT_STR(row,col,strTemp)//col +1
				FLEX_GRID1_PUT_STR(row,col,strTemp);//col +1
				FLEX_GRID1_PUT_STR(totalrows,col,strTemp);//col +1

			}
		}		
	}
#endif



#endif
}

void COutPutDlg::FreshGrid_PID2tstat6()
{
//	268	330	1	Low byte	W/R	Number of Heating Stages (Max heat+cool = 6)
//	269	331	1	Low byte	W/R	Number of Cooling Stages (Max heat + Cool = 6) 

    int nValue;
	strdemo = _T("2,");
	SetPaneString(2,strdemo);

	CString strTemp;
	m_PID2_heat_stages=product_register_value[MODBUS_HEAT_UNIVERSAL_TABLE];//276 register ,heat stages
	m_PID2_cool_stages=product_register_value[MODBUS_COOL_UNIVERSAL_TABLE];//277 register ,cool stages
	if(m_PID2_heat_stages==0&&m_PID2_cool_stages==0)
	{
		//write_one(g_tstat_id,330,3);
		//write_one(g_tstat_id,331,3);
// 		newtstat6[330] = 3;
// 		newtstat6[331] =3;
		m_PID2_heat_stages=3;
		m_PID2_cool_stages=3;
		if(m_PID2_heat_stages>6)
			m_PID2_heat_stages=6;
		if(m_PID2_cool_stages>6)
			m_PID2_cool_stages=6;
	}
 

	strTemp.Format(_T("%d"),m_PID2_heat_stages);
	m_PID2_heatEdit.SetWindowText(strTemp);
	strTemp.Format(_T("%d"),m_PID2_cool_stages);
	m_PID2_coolEdit.SetWindowText(strTemp);

	m_FlexGrid2.put_Rows(8);
	m_FlexGrid2.put_Cols(m_PID2_heat_stages+m_PID2_cool_stages+7);//3 because 1/pid;2/out1;3/coast 4/interlock
	//*****************************************
	m_FlexGrid2.put_TextMatrix(0,1,_T("Description"));
	m_FlexGrid2.put_TextMatrix(0,2,_T("Function"));
	m_FlexGrid2.put_ColWidth(2,0);
	m_FlexGrid2.put_TextMatrix(0,3,_T("Rotation"));
	m_FlexGrid2.put_ColWidth(3,0);
	m_FlexGrid2.put_TextMatrix(0,4,_T("Control"));
	m_FlexGrid2.put_TextMatrix(0,5,_T("Interlock"));
	int i=0;
	for(i=0;i<m_PID2_heat_stages;i++)
		m_FlexGrid2.put_TextMatrix(0,6+i,heat_hand[m_PID2_heat_stages-i-1]);
	m_FlexGrid2.put_TextMatrix(0,m_PID2_heat_stages+6,_T("Coast"));
	for(i=0;i<m_PID2_cool_stages;i++)
		m_FlexGrid2.put_TextMatrix(0,m_PID2_heat_stages+7+i,cool_hand[i]);


//	m_FlexGrid1.put_Rows(8);
//	m_FlexGrid1.put_Cols(m_PID2_heat_stages+m_PID2_cool_stages+7);//interlock control first_blank coast


	m_FlexGrid2.put_TextMatrix(1,1,g_strOutName1);
	m_FlexGrid2.put_TextMatrix(2,1,g_strOutName2);
	m_FlexGrid2.put_TextMatrix(3,1,g_strOutName3);
	m_FlexGrid2.put_TextMatrix(4,1,g_strOutName4);
	m_FlexGrid2.put_TextMatrix(5,1,g_strOutName5);
	m_FlexGrid2.put_TextMatrix(6,1,g_strOutName6);
	m_FlexGrid2.put_TextMatrix(7,1,g_strOutName7);

	m_FlexGrid2.put_TextMatrix(1,0,_T("1"));
	m_FlexGrid2.put_TextMatrix(2,0,_T("2"));
	m_FlexGrid2.put_TextMatrix(3,0,_T("3"));
	m_FlexGrid2.put_TextMatrix(4,0,_T("4"));
	m_FlexGrid2.put_TextMatrix(5,0,_T("5"));
	m_FlexGrid2.put_TextMatrix(6,0,_T("6"));
	m_FlexGrid2.put_TextMatrix(7,0,_T("7"));

	m_FlexGrid2.put_ColAlignment(0,4);
	m_FlexGrid2.put_ColAlignment(1,4);
	//*****************************888、


	int totalrows,row;
	unsigned short tstatval,pos;


		totalrows = 7;

	int pid_select2[7]={0};

		for(i=0;i<7;i++)
			pid_select2[i]=product_register_value[MODBUS_PID_OUTPUT1+i];/////////////////////////////get pid select ;col one 1



#if 1
	////////////////////////down code is for m_flexgrid2
	//for(int col = 1 ;col <= (m_PID2_heat_stages+m_PID2_heat_stages+1);col++)
	if (product_register_value[MODBUS_FAN_SPEED]==4)
	 //Auto
	{
	for(int col = 1 ;col <= (m_PID2_heat_stages+m_PID2_cool_stages+1);col++)
	{
		if(col < (m_PID2_heat_stages+1))
			pos = (m_PID2_cool_stages+m_PID2_heat_stages+1) - col ;
		else
			pos = col - (m_PID2_heat_stages+1);
		//254	281	1	Low byte	W/R	PID2 Output table- Coasting
		tstatval = product_register_value[MODBUS_UNIVERSAL_OUTPUT_BEGIN + pos];
		for( row = 1;row<=totalrows;row++)//****************************
		{
			CString str;
			{
				if(digital2string((tstatval >> (row-1)) & 0x01,str,FAN))
					if(pid_select2[row-1]==0)
					FLEX_GRID2_PUT_COLOR_STR(row,col,str)//col +1
					else
					FLEX_GRID2_PUT_STR(row,col,str)//col +1
			}
		}
	}
	
	
	
	
	
	
	for(int col = 1 ;col <=(m_PID2_heat_stages+m_PID2_cool_stages+1);col++)
		{
			if(col < (m_PID2_heat_stages+1))
				pos = (m_PID2_heat_stages+m_PID2_cool_stages+1) - col ;
			else
				pos = col - (m_PID2_heat_stages+1);

				tstatval = product_register_value[MODBUS_UNIVERSAL_VALVE_BEGIN+ pos];
		 
			///////////////////////////////////////////


			for(row=totalrows-1;row<=totalrows;row++)//row value
			{
				if (row==4&&m_bOut4PWM)
				{
					continue;
				}
				if (row==5&&m_bOut5PWM)
				{
					continue;
				}
			
				int nOutReg;
				nOutReg=MODBUS_INTERLOCK_OUTPUT1+row-1;
				nValue=product_register_value[nOutReg];
				if(nValue==7)
					continue;


				CString str;
				if(tstatval>63)//
				{
					//
					if(row==(totalrows-1))
					{
						if(tstatval & 64)
						{
							if(digital2string(4,str,VALVE))//for 7 or 8 bit
								if(pid_select2[row-1]==0)
								FLEX_GRID2_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID2_PUT_STR(row,col,str)//col +1
						}
						else
						{ 
							if(digital2string(tstatval & 0x03,str,VALVE))//*** value
							{
								if(pid_select2[row-1]==0)
								FLEX_GRID2_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID2_PUT_STR(row,col,str)//col +1
							}
						}
					}
					if(row==totalrows)
					{
						if(tstatval & 128)
						{
							if(digital2string(4,str,VALVE))//for 7 or 8 bit
							{
								if(pid_select2[row-1]==0)
								FLEX_GRID2_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID2_PUT_STR(row,col,str)//col +1
							}

						}else
						{
							if(digital2string((tstatval >> 2) & 0x03,str,VALVE))//*** value
							{
								if(pid_select2[row-1]==0)
									FLEX_GRID2_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID2_PUT_STR(row,col,str)//col +1
							}
						}
					}
				}
				else
				{
					if(digital2string((tstatval >> ((row-totalrows+1)*2)) & 0x03,str,VALVE))//*** value
						if(pid_select2[row-1]==0)
						FLEX_GRID2_PUT_COLOR_STR(row,col,str)//col +1
						else
						FLEX_GRID2_PUT_STR(row,col,str)//col +1
				}
			}
		}
	} 
	else//OFF
	{

		for(int col = 1 ;col <= (m_PID2_heat_stages+m_PID2_cool_stages+1);col++)
	{
		if(col < (m_PID2_heat_stages+1))
			pos = (m_PID2_cool_stages+m_PID2_heat_stages+1) - col ;
		else
			pos = col - (m_PID2_heat_stages+1);
		//254	281	1	Low byte	W/R	PID2 Output table- Coasting
		tstatval = product_register_value[MODBUS_UNIVERSAL_OFF_OUTPUT_BEGIN + pos];
		for( row = 1;row<=totalrows;row++)//****************************
		{
			CString str;
			{
				if(digital2string((tstatval >> (row-1)) & 0x01,str,FAN))
					if(pid_select2[row-1]==0)
					FLEX_GRID2_PUT_COLOR_STR(row,col,str)//col +1
					else
					FLEX_GRID2_PUT_STR(row,col,str)//col +1
			}
		}
	}
	
	
	
	
	
	
	for(int col = 1 ;col <=(m_PID2_heat_stages+m_PID2_cool_stages+1);col++)
		{
			if(col < (m_PID2_heat_stages+1))
				pos = (m_PID2_heat_stages+m_PID2_cool_stages+1) - col ;
			else
				pos = col - (m_PID2_heat_stages+1);

				tstatval = product_register_value[MODBUS_UNIVERSAL_OFF_VALVE_BEGIN+ pos];
		 
			///////////////////////////////////////////


			for(row=totalrows-1;row<=totalrows;row++)//row value
			{
				if (row==4&&m_bOut4PWM)
				{
					continue;
				}
				if (row==5&&m_bOut5PWM)
				{
					continue;
				}
			
				int nOutReg;
				nOutReg=MODBUS_INTERLOCK_OUTPUT1+row-1;
				nValue=product_register_value[nOutReg];
				if(nValue==7)
					continue;


				CString str;
				if(tstatval>63)//
				{
					//
					if(row==(totalrows-1))
					{
						if(tstatval & 64)
						{
							if(digital2string(4,str,VALVE))//for 7 or 8 bit
								if(pid_select2[row-1]==0)
									FLEX_GRID2_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID2_PUT_STR(row,col,str)//col +1
						}
						else
						{ 
							if(digital2string(tstatval & 0x03,str,VALVE))//*** value
							{
								if(pid_select2[row-1]==0)
								FLEX_GRID2_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID2_PUT_STR(row,col,str)//col +1
							}
						}
					}
					if(row==totalrows)
					{
						if(tstatval & 128)
						{
							if(digital2string(4,str,VALVE))//for 7 or 8 bit
							{
								if(pid_select2[row-1]==0)
									FLEX_GRID2_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID2_PUT_STR(row,col,str)//col +1
							}

						}else
						{
							if(digital2string((tstatval >> 2) & 0x03,str,VALVE))//*** value
							{
								if(pid_select2[row-1]==0)
									FLEX_GRID2_PUT_COLOR_STR(row,col,str)//col +1
								else
								FLEX_GRID2_PUT_STR(row,col,str)//col +1
							}
						}
					}
				}
				else
				{
					if(digital2string((tstatval >> ((row-totalrows+1)*2)) & 0x03,str,VALVE))//*** value
						if(pid_select2[row-1]==0)
						FLEX_GRID2_PUT_COLOR_STR(row,col,str)//col +1
						else
						FLEX_GRID2_PUT_STR(row,col,str)//col +1
				}
			}
		}

	}
	
 
#endif
#if 1
	 

	//328	266	1	Low byte	W/R	"Output1 Function setting:
	//0=normal, default. 1 = rotation (old disabled feature) 2 = lighting control, one keypad button can be assigned to toggle a relay on & off "



	int  nFunction=product_register_value[MODBUS_OUTPUT1_FUNCTION];


	unsigned short interlock[7]={0};
	for(int i=0;i<7;i++)
	{
		interlock[i]=product_register_value[MODBUS_INTERLOCK_OUTPUT1+i];
	}
	//	Read_Multi(tstat_id,interlock,286,7);

	CString str;
	TCHAR *interlock_str[]={_T("On"),_T("DI1"),_T("AI1"),_T("AI2"),_T("Timer Or"),_T("Timer And"),_T("Interlock Timer"),_T("FreeCool")};


	for(i=0;i<totalrows;i++)
	{		

		switch(interlock[i])
		{
			//case 0:m_FlexGrid.put_TextMatrix(i+1,5,interlock_str[0]);m_FlexGrid2.put_TextMatrix(i+1,5,interlock_str[0]);break;
		case 0:
		m_FlexGrid1.put_TextMatrix(i+1,5,_T("      -"));
		m_FlexGrid2.put_TextMatrix(i+1,5,_T("      -"));
		break;
		case 1:
		m_FlexGrid1.put_TextMatrix(i+1,5,interlock_str[1]);
		m_FlexGrid2.put_TextMatrix(i+1,5,interlock_str[1]);
		break;
		case 2:
		m_FlexGrid1.put_TextMatrix(i+1,5,interlock_str[2]);
		m_FlexGrid2.put_TextMatrix(i+1,5,interlock_str[2]);
		break;
		case 3:
		m_FlexGrid1.put_TextMatrix(i+1,5,interlock_str[3]);
		m_FlexGrid2.put_TextMatrix(i+1,5,interlock_str[3]);
		break;
		case 4:
		m_FlexGrid1.put_TextMatrix(i+1,5,interlock_str[4]);
		m_FlexGrid2.put_TextMatrix(i+1,5,interlock_str[4]);
		break;
		case 5:
		m_FlexGrid1.put_TextMatrix(i+1,5,interlock_str[5]);
		m_FlexGrid2.put_TextMatrix(i+1,5,interlock_str[5]);
		break;
		case 6:m_FlexGrid1.put_TextMatrix(i+1,5,interlock_str[6]);
		m_FlexGrid2.put_TextMatrix(i+1,5,interlock_str[6]);
		break;
		case 7:
		m_FlexGrid1.put_TextMatrix(i+1,5,interlock_str[7]);
		m_FlexGrid2.put_TextMatrix(i+1,5,interlock_str[7]);
		break;

		default:m_FlexGrid1.put_TextMatrix(i+1,5,interlock_str[0]);m_FlexGrid2.put_TextMatrix(i+1,5,interlock_str[0]);break;
		}
		switch(pid_select2[i])
		{
		case 0:str = _T("PID1");break;
		case 1:str = _T("PID2");break;
		case 2:str = _T("MAX(PID1,PID2)");break;
		case 3:str = _T("MIN(PID1,PID2)");break;
		default:str = _T("PID2");break;
		}
 		m_FlexGrid1.put_TextMatrix(i+1,4,str);
 		m_FlexGrid2.put_TextMatrix(i+1,4,str);		

	/*	m_FlexGrid1.put_TextMatrix(i+1,4,_T("PID1"));//Marked by Fance ,don't know why???
		m_FlexGrid2.put_TextMatrix(i+1,4, _T("PID2"));*/
	}


#if 1
	for( row = 1;row<=totalrows;row++)//row value
	{  
		if(pid_select2[row-1]==2)
		{		
			if(i_104_pid1>i_268_pid2)
			{
				for(int col = 6 ;col <=(m_PID1_heat_stages+m_PID1_cool_stages+1)+5;col++)
				{
					m_FlexGrid1.put_Row(row);
					m_FlexGrid1.put_Col(col);
					m_FlexGrid1.put_CellForeColor(FLEXGRID_CELL_COLOR);
				}
				for(int col =6 ;col <=(m_PID2_cool_stages+m_PID2_heat_stages+1)+5;col++)
				{
					m_FlexGrid2.put_Row(row);
					m_FlexGrid2.put_Col(col);
					m_FlexGrid2.put_CellBackColor(FLEXGRID_CELL_COLOR);
				}
			}
			else//i_104_pid1<=i_268_pid2
			{
				for(int col = 6 ;col <=(m_PID1_heat_stages+m_PID1_cool_stages+1)+5;col++)
				{
					m_FlexGrid1.put_Row(row);
					m_FlexGrid1.put_Col(col);
					m_FlexGrid1.put_CellBackColor(FLEXGRID_CELL_COLOR);

				}
				for(int col = 6 ;col <=(m_PID2_cool_stages+m_PID2_heat_stages+1)+5;col++)
				{

					m_FlexGrid2.put_Row(row);
					m_FlexGrid2.put_Col(col);
					m_FlexGrid2.put_CellForeColor(FLEXGRID_CELL_COLOR);
				}

			}
		}
	}
#endif

	#endif


	if(product_register_value[MODBUS_MODE_OUTPUT4]==3)
	{
		m_bOut4PWM=TRUE;
	}
	if(product_register_value[MODBUS_MODE_OUTPUT5]==3)
	{
		m_bOut5PWM=TRUE;
	}

	if(m_bOut4PWM)
	{

		for(int col = 1 ;col <=(m_PID2_heat_stages+m_PID2_cool_stages+1);col++)
		{
			int nValue=0;
			if(col < (m_PID2_heat_stages+1))
				pos = (m_PID2_heat_stages+m_PID2_cool_stages+1) - col ;
			else
				pos = col - (m_PID2_heat_stages+1);			
			//tstatval = product_register_value[323+ pos];
			int indext=-1;
			//nValue=read_one(tstat_id,341+pos);
			//nValue=multi_register_value[341+pos];//这项没找到对应项   //Annul by Fance

			nValue=product_register_value[MODBUS_OUTPUT_PWM_AUTO_COAST+pos];//modify by Fance  341  591

			nValue=nValue>>4;
			switch(nValue)
			{
			case 0:
				strTemp=_T("Close");
				break;
			case 1:
				strTemp=_T("Open");
				break;
			case 2:
				strTemp="0-100";
				break;
			case 3:
				strTemp=_T("50-100");
				break;
			case 4:
				strTemp=_T("0-50");
				break;
			default:
				strTemp=_T("");
			}

			if(pid_select2[3]==0)
				FLEX_GRID2_PUT_COLOR_STR(4,col,strTemp)//col +1
			else
			FLEX_GRID2_PUT_STR(4,col,strTemp)//col +1


			//FLEX_GRID2_PUT_STR(4,col,strTemp)//col +1
		}
	}	
	if(m_bOut5PWM)
	{
		for(int col = 1 ;col <=(m_PID2_heat_stages+m_PID2_cool_stages+1);col++)
		{
			int nValue=0;
			if(col < (m_PID2_heat_stages+1))
				pos = (m_PID2_heat_stages+m_PID2_cool_stages+1) - col ;
			else
				pos = col - (m_PID2_heat_stages+1);			
			 
			int indext=-1;

			//nValue=multi_register_value[341+pos];//这项没找到对应项		 //Annul by Fance
			nValue=product_register_value[MODBUS_OUTPUT_PWM_AUTO_COAST+pos];//modify by Fance  341  591

			nValue=nValue&0x0f;
			switch(nValue)
			{
			case 0:
				strTemp=_T("Close");
				break;
			case 1:
				strTemp=_T("Open");
				break;
			case 2:
				strTemp=_T("0-100");
				break;
			case 3:
				strTemp=_T("50-100");
				break;
			case 4:
				strTemp=_T("0-50");
				break;
			default:
				strTemp=_T("");
			}
			if(pid_select2[4]==0)
				FLEX_GRID2_PUT_COLOR_STR(5,col,strTemp)//col +1
			else
			FLEX_GRID2_PUT_STR(5,col,strTemp)//col +1
				//totalrows
		}
	}
}


void COutPutDlg::OnBnClickedUpdate()
{

	FreshGrids();
}

//int write_one_show_message(unsigned char device_var,unsigned short address,short value,int retry_times)
//{//retry 
//
//	BOOL bTemp = g_bEnableRefreshTreeView;
//	g_bEnableRefreshTreeView = FALSE;
//	int j = write_one_org(device_var,address,value,retry_times);
//	g_bEnableRefreshTreeView |= bTemp;
//
//	if(j<0)
//	{
//		AfxMessageBox(_T("Write Register Fail!Please try it again!"),MB_OK | MB_ICONINFORMATION);
//		last_output_write_register_result =false;
//	}
//	else
//	{
//		last_output_write_register_result = true;
//	}
//
//	return j;
//}



void COutPutDlg::OnBnClickedRefresh()
{
//OnBnClickedUpdate();	// TODO: Add your control notification handler code here
}
