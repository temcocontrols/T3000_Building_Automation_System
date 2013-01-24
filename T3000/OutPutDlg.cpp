// OutPutDlg.cpp : implementation file
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
END_MESSAGE_MAP()

BOOL COutPutDlg::OnInitDialog()
{
	if(g_OutPutLevel==1)
	{
		CAfxMessageDialog dlg;
		CString strPromption;
		strPromption.LoadString(IDS_STRNOPRIVILEGE);
		dlg.SetPromtionTxt(strPromption);
		dlg.DoModal();
	}

	CDialog::OnInitDialog();
	m_ItemValueCombx.ShowWindow(SW_HIDE);
	m_DescriptEdt.ShowWindow(SW_HIDE);
//	OnCbnSelchangeFanmode();

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

	//FreshGrids();

	int output4_value=multi_register_value[283];//2 rows plug in one row
	int output5_value=multi_register_value[284];
	if(output4_value==1||output5_value==1)
	{
		m_bFloat=TRUE;
	}
	if (output4_value==2)
	{
		m_bOut4PWM=TRUE;
	}
	if(output5_value==2)
	{
		m_bOut5PWM=TRUE;
	}
	i_104_pid1=multi_register_value[104];
	i_268_pid2=multi_register_value[268];
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
void COutPutDlg::put_fan_variable()
{
	CString p[6]={_T("Fan Off"),_T("Fan On"),_T("Fan Low"),_T("Fan Mid"),_T("Fan High"),_T("Fan Aut")};
//		UpdateData(true);
	m_fan.ResetContent();
	if(read_one(g_tstat_id,129)==1)////here can't use read_multi function
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
	m_fan.SetCurSel(multi_register_value[137]);
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

}

void COutPutDlg::OnCbnSelchangeCbfan()
{
	// TODO: Add your control notification handler code here


	FreshGrids();
}

void COutPutDlg::OnEnKillfocusPid1Heatstageedit()
{
	CString strText;
	GetDlgItem(IDC_PID1_HEATSTAGEEDIT)->GetWindowText(strText);
	m_PID1_heat_stages=_wtoi(strText);

}

void COutPutDlg::OnEnKillfocusPid1coolstageedit()
{
	CString strText;
	GetDlgItem(IDC_PID1COOLSTAGEEDIT)->GetWindowText(strText);

}
void COutPutDlg::FreshGrids()
{
	FreshGrid_PID1();
	FreshGrid_PID2();
}
void COutPutDlg::FreshGrid_PID1()
{
	if(multi_register_value[129]==0)
		m_bFanAutoOnly=FALSE;
	if(multi_register_value[129]==1)
		m_bFanAutoOnly=TRUE;
	if(m_bFanAutoOnly)
	{
		 
	}
	else
	{
	 
	}
 

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
		write_one(g_tstat_id,276,3);
		write_one(g_tstat_id,277,3);
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
	m_FlexGrid1.put_TextMatrix(0,4,_T("Control"));
	m_FlexGrid1.put_TextMatrix(0,5,_T("InterLock"));
	int i=0;
	for(i=0;i<m_PID1_heat_stages;i++)
		m_FlexGrid1.put_TextMatrix(0,6+i,heat_hand[m_PID1_heat_stages-i-1]);
	m_FlexGrid1.put_TextMatrix(0,m_PID1_heat_stages+6,_T("Coast"));
	for(i=0;i<m_PID1_cool_stages;i++)
		m_FlexGrid1.put_TextMatrix(0,m_PID1_heat_stages+7+i,cool_hand[i]);

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
			if(m_bFloat&&row==4)
				continue;
			if(m_bFloat&&row==5)
				continue;
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
			{
				if(digital2string((tstatval >> (row-1)) & 0x01,str,FAN))
					if(pid_select2[row-1]==1)
						FLEX_GRID1_PUT_COLOR_STR(row,col,str)//col +1
					else
						FLEX_GRID1_PUT_STR(row,col,str)//col +1
			}

		}
	}
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
			if(m_fan.GetCurSel()==0&&(m_nmoduleType==1||m_nmoduleType==3))//a,d,g
				tstatval = multi_register_value[351+ pos];
			else
				tstatval = multi_register_value[173+ pos];	
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
				if(m_bFloat&&row==4)
					continue;
				if(m_bFloat&&row==5)
					continue;
				int nOutReg;
				nOutReg=286+row-1;
				nValue=multi_register_value[nOutReg];
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
	}

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

				nValue=(nValue&0x30)>>4;
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

		
		if(m_bOut4PWM)
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
				FLEX_GRID1_PUT_STR(5,col,strTemp)//col +1
					//totalrows
			}
		}

//Free Cool:

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
				//	FLEX_GRID1_PUT_STR(row,col,strTemp)//col +1
					FLEX_GRID1_PUT_STR(row,col,strTemp);//col +1
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
}


void COutPutDlg::FreshGrid_PID2()
{
	CString strTemp;
	m_PID2_heat_stages=multi_register_value[268];//276 register ,heat stages
	m_PID2_cool_stages=multi_register_value[269];//277 register ,cool stages
	if(m_PID2_heat_stages==0&&m_PID2_cool_stages==0)
	{
		write_one(g_tstat_id,268,3);
		write_one(g_tstat_id,269,3);
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
	//*****************************888¡¢
	int totalrows,row;
	unsigned short tstatval,pos,nValue;

	if(m_nmoduleType == 3)//5d
		m_FlexGrid2.put_Rows(8);
	else
		m_FlexGrid2.put_Rows(6);
	if(m_nmoduleType == 1 )//5a
		totalrows = 3;
	else 
		totalrows = 5;
	
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
		tstatval = multi_register_value[254 + pos];
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
	if(m_nmoduleType == 1 || m_nmoduleType == 3)
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
			tstatval = multi_register_value[261+ pos];
			for(row=totalrows-1;row<=totalrows;row++)//row value
			{
				CString str;
				if(tstatval > 63)
					{
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

	if(multi_register_value[188]!=3)//on/off mode
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

}
void COutPutDlg::OnBnClickedFanautocheck()
{
	if(g_OutPutLevel==1)
		return;


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
	lRow = m_FlexGrid1.get_RowSel();//
	lCol = m_FlexGrid1.get_ColSel(); //

}


void COutPutDlg::OnCbnKillfocusValueitemcombo()
{

}
void COutPutDlg::OnWrite(bool bflexgrid1_or_2,int col,int row)
{

	FreshGrids();
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


}

void COutPutDlg::ClickMsflexgrid2()
{
	if(g_OutPutLevel==1)
		return;
	m_bflexgrid1_or_2=true;//flexgrid2 is selected

}

void COutPutDlg::OnEnKillfocusPid2Heatstageedit2()
{
	if(g_OutPutLevel==1)
		return;
	CString strText;
	GetDlgItem(IDC_PID2_HEATSTAGEEDIT2)->GetWindowText(strText);

}

void COutPutDlg::OnEnKillfocusPid2coolstageedit2()
{
	if(g_OutPutLevel==1)
		return;
	CString strText;
	GetDlgItem(IDC_PID2COOLSTAGEEDIT2)->GetWindowText(strText);

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



	
	
}

void COutPutDlg::OnEnSetfocusDescriptedit()
{
	
}


