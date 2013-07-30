// TStatScheduleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "TStatScheduleDlg.h"
#include "globle_function.h"
#include "global_variable_extern.h"
#include "schedule.h"

// CTStatScheduleDlg dialog


const int SCHED_START_ADDR= 418;
const int TIME_FORMAT_REG = 348;

IMPLEMENT_DYNAMIC(CTStatScheduleDlg, CDialog)

CTStatScheduleDlg::CTStatScheduleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTStatScheduleDlg::IDD, pParent)
{

}

CTStatScheduleDlg::~CTStatScheduleDlg()
{
}

void CTStatScheduleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID_WORKING, m_fgWorkDay);
	DDX_Control(pDX, IDC_MSFLEXGRID_WEEKEND, m_fgWeekend);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_editInput);
	DDX_Control(pDX, IDC_COMBO_ONOFF, m_scheduleonoff);
}


BEGIN_MESSAGE_MAP(CTStatScheduleDlg, CDialog)
	ON_EN_KILLFOCUS(IDC_EDIT_INPUT, &CTStatScheduleDlg::OnEnKillfocusEditInput)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CTStatScheduleDlg::OnBnClickedButtonSave)
	ON_CBN_SELCHANGE(IDC_COMBO_PMAM, &CTStatScheduleDlg::OnCbnSelchangeComboPmam)
	ON_CBN_KILLFOCUS(IDC_COMBO_PMAM, &CTStatScheduleDlg::OnCbnKillfocusComboPmam)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CTStatScheduleDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_RADIO_24H, &CTStatScheduleDlg::OnBnClickedRadio24h)
	ON_BN_CLICKED(IDC_RADIO_12H, &CTStatScheduleDlg::OnBnClickedRadio12h)
	ON_CBN_SELCHANGE(IDC_COMBO_ONOFF, &CTStatScheduleDlg::OnCbnSelchangeComboOnoff)
END_MESSAGE_MAP()


// CTStatScheduleDlg message handlers

BOOL CTStatScheduleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_scheduleonoff.AddString(_T("Disable"));
	m_scheduleonoff.AddString(_T("Enable"));
	m_scheduleonoff.SetCurSel(product_register_value[MODBUS_SCHEDULE_ON_OFF]);
	InitRegisterTable();
	InitGrid();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTStatScheduleDlg::InitGrid()
{
	// working day
	m_fgWorkDay.put_Rows(5);
	m_fgWorkDay.put_Cols(4);

	m_fgWorkDay.put_ColWidth(0,1200);
	m_fgWorkDay.put_ColWidth(1,800);
	m_fgWorkDay.put_ColWidth(2,800);
	m_fgWorkDay.put_ColWidth(3,1200);
	

	m_fgWorkDay.put_TextMatrix(0,1,_T("Hour"));	
	m_fgWorkDay.put_TextMatrix(0,2,_T("Minute"));
	//m_fgWorkDay.put_TextMatrix(0,3,_T("P.M./A.M."));	
	m_fgWorkDay.put_TextMatrix(0,3,_T("Time Setting"));	

	m_fgWorkDay.put_TextMatrix(1,0,_T("Awaking Time"));	
	m_fgWorkDay.put_TextMatrix(2,0,_T("Away Time"));
	m_fgWorkDay.put_TextMatrix(3,0,_T("Back Time"));	
	m_fgWorkDay.put_TextMatrix(4,0,_T("Sleeping Time"));	



	// weekend day

	m_fgWeekend.put_Rows(5);
	m_fgWeekend.put_Cols(4);

	m_fgWeekend.put_ColWidth(0,1200);
	m_fgWeekend.put_ColWidth(1,800);
	m_fgWeekend.put_ColWidth(2,800);
	m_fgWeekend.put_ColWidth(3,1200);
	//m_fgWeekend.put_ColWidth(4,1200);

	m_fgWeekend.put_TextMatrix(0,1,_T("Hour"));	
	m_fgWeekend.put_TextMatrix(0,2,_T("Minute"));
	//m_fgWeekend.put_TextMatrix(0,3,_T("P.M./A.M."));	
	m_fgWeekend.put_TextMatrix(0,3,_T("Time Setting"));	

	m_fgWeekend.put_TextMatrix(1,0,_T("Awaking Time"));	
	m_fgWeekend.put_TextMatrix(2,0,_T("Away Time"));
	m_fgWeekend.put_TextMatrix(3,0,_T("Back Time"));	
	m_fgWeekend.put_TextMatrix(4,0,_T("Sleeping Time"));	
	for (int i = 0; i <4; i++)
	{
		m_fgWorkDay.put_ColAlignment(i,4);
		m_fgWeekend.put_ColAlignment(i,4);
	}

	FillGridByRegister();
}





void CTStatScheduleDlg::FillGridByRegister()
{
	for (int i = 1; i <=4; i++)
	{
		int nWorkDayAwakeH = m_szRegTable[0]; //357...
		int nWorkDayAwakeM = m_szRegTable[1]; 
		int nHour = product_register_value[nWorkDayAwakeH+(i-1)*2];
		int nMin = product_register_value[nWorkDayAwakeM+(i-1)*2];
	//	int nSec = multi_register_value[1];
		CString strTime,strHour,strMin,strPMAM;

		strHour.Format(_T("%.2d"), nHour);
		strMin.Format(_T("%.2d"), nMin);
		//strSec.Format(_T("%d"), nSec);
		/*strPMAM = nHour > 12 ? _T("P.M.") : _T("A.M.");*/
		strTime = strHour+_T(":")+strMin;

// 		int nHour2 = multi_register_value[466+i];
// 		int nMin2 = multi_register_value[467+i];
// 		int nSec2 = multi_register_value[1];

		m_fgWorkDay.put_TextMatrix(i,1,strHour);	
		m_fgWorkDay.put_TextMatrix(i,2,strMin);	
		/*m_fgWorkDay.put_TextMatrix(i,3,strPMAM);*/	
		m_fgWorkDay.put_TextMatrix(i,3,strTime);	

		//////////////////////////////////////////////////////////////////////////
		int nWeekenAwakeH = m_szRegTable[SCHED_WEEKEN_AWAKE_HOUR]; 
		int nWeekenAwakeM = m_szRegTable[SCHED_WEEKEN_AWAKE_MIN]; 
		nHour = product_register_value[nWeekenAwakeH+(i-1)*2];
		nMin = product_register_value[nWeekenAwakeM+(i-1)*2];
		//nSec = multi_register_value[1];		
		strHour.Format(_T("%.2d"), nHour);
		strMin.Format(_T("%.2d"), nMin);
		//strSec.Format(_T("%d"), nSec);
		//strPMAM = nHour > 12 ? _T("P.M.") : _T("A.M.");
		strTime = strHour+_T(":")+strMin;

		m_fgWeekend.put_TextMatrix(i,1,strHour);	
		m_fgWeekend.put_TextMatrix(i,2,strMin);	
		m_fgWeekend.put_TextMatrix(i,3,strTime);	
		//m_fgWeekend.put_TextMatrix(i,4,strTime);	
	}
	
}


void CTStatScheduleDlg::SaveToRegister()
{
	int nRet =0;	
	BYTE szTime[16]={0};

	for (int i =0; i <4 ; i++)
	{	
		CString strTime,strHour,strMin,strSec;
		strHour=m_fgWorkDay.get_TextMatrix(i+1,1);	
		strMin = m_fgWorkDay.get_TextMatrix(i+1,2);	
		szTime[i*2] =(BYTE) _wtoi(strHour);
		szTime[i*2+1] = (BYTE)_wtoi(strMin);
			
	//////////////////////////////////////////////////////////////////////////

		strHour=m_fgWeekend.get_TextMatrix(i+1,1);	
		strMin = m_fgWeekend.get_TextMatrix(i+1,2);	
		szTime[i*2+8] = _wtoi(strHour);
		szTime[i*2+1+8] = _wtoi(strMin);
		
	}
	
	Write_Multi(g_tstat_id, szTime, SCHED_START_ADDR, 16, 3);

	
}


BEGIN_EVENTSINK_MAP(CTStatScheduleDlg, CDialog)
ON_EVENT(CTStatScheduleDlg, IDC_MSFLEXGRID_WORKING, DISPID_CLICK, CTStatScheduleDlg::ClickMsflexgridWorking, VTS_NONE)
ON_EVENT(CTStatScheduleDlg, IDC_MSFLEXGRID_WEEKEND, DISPID_CLICK, CTStatScheduleDlg::ClickMsflexgridWeekend, VTS_NONE)
END_EVENTSINK_MAP()


void CTStatScheduleDlg::ClickMsflexgridWeekend()
{
	long lRow,lCol;
	lRow = m_fgWeekend.get_RowSel();//��ȡ������к�	
	lCol = m_fgWeekend.get_ColSel(); //��ȡ������к�

	if(lRow<=0)
		return;
	m_szEditPosInWeekend.cx=lRow;
	m_szEditPosInWeekend.cy = lCol;
	//////////////////////////////////////////////////////////////////////////

	ShowInputEditBox(lRow, lCol, FALSE);
	m_bInWorkDay = FALSE;
}

void CTStatScheduleDlg::ClickMsflexgridWorking()
{
	long lRow,lCol;
	lRow = m_fgWorkDay.get_RowSel();//��ȡ������к�	
	lCol = m_fgWorkDay.get_ColSel(); //��ȡ������к�

	if(lRow<=0)
		return;
	m_szEditPosInWork.cx=lRow;
	m_szEditPosInWork.cy = lCol;
	//////////////////////////////////////////////////////////////////////////

	ShowInputEditBox(lRow, lCol, TRUE);
	m_bInWorkDay = TRUE;
}


void CTStatScheduleDlg::ShowInputEditBox(int iRow, int iCol, BOOL bWorkDay)
{
	if (iCol == 3)
	{
		(CComboBox*)GetDlgItem(IDC_COMBO_PMAM)->ShowWindow(SW_HIDE);
		m_editInput.ShowWindow(SW_HIDE);
		return;
	}

	if (iCol != 0)
	{
		m_editInput.SetWindowText(_T(""));
		CRect rc = CalcGridCellRect(iRow, iCol, bWorkDay);  // �϶�ID��
		m_editInput.MoveWindow(rc, TRUE);
		m_editInput.SetFocus();
		m_editInput.BringWindowToTop();
		m_editInput.ShowWindow(SW_NORMAL);
		(CComboBox*)GetDlgItem(IDC_COMBO_PMAM)->ShowWindow(SW_HIDE);
		//m_editInput.cx = iRow;
		//m_editInput.cy = iCol;
	}
	else
	{
		//CComboBox* pCom = (CComboBox*)GetDlgItem(IDC_COMBO_PMAM);
		//CRect rc = CalcGridCellRect(iRow, iCol, bWorkDay);  // �϶�ID��
		//pCom->MoveWindow(rc, TRUE);
		//pCom->SetFocus();
		//pCom->BringWindowToTop();
		//pCom->ShowWindow(SW_NORMAL);
		//m_editInput.ShowWindow(SW_HIDE);

	}
}


CRect CTStatScheduleDlg::CalcGridCellRect(int iRow, int iCol, BOOL bWorkDay)
{
	CRect rect;
	if (bWorkDay)
			m_fgWorkDay.GetWindowRect(rect); //��ȡ���ؼ��Ĵ��ھ���
	else
			m_fgWeekend.GetWindowRect(rect); //��ȡ���ؼ��Ĵ��ھ���


	ScreenToClient(rect); //ת��Ϊ�ͻ�������	
	CDC* pDC = GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	long x,y,width,height;
	if (bWorkDay)
	{
		//����ѡ�и�����Ͻǵ�����(����Ϊ��λ)
		 y = m_fgWorkDay.get_RowPos(iRow)/nTwipsPerDotY;
		 x = m_fgWorkDay.get_ColPos(iCol)/nTwipsPerDotX;
		//����ѡ�и�ĳߴ�(����Ϊ��λ)����1��ʵ�ʵ����У����ּ�1��Ч������
		 width = m_fgWorkDay.get_ColWidth(iCol)/nTwipsPerDotX+1;
		 height = m_fgWorkDay.get_RowHeight(iRow)/nTwipsPerDotY+1;
	}
	else
	{
		//����ѡ�и�����Ͻǵ�����(����Ϊ��λ)
		y = m_fgWeekend.get_RowPos(iRow)/nTwipsPerDotY;
		x = m_fgWeekend.get_ColPos(iCol)/nTwipsPerDotX;
		//����ѡ�и�ĳߴ�(����Ϊ��λ)����1��ʵ�ʵ����У����ּ�1��Ч������
		width = m_fgWeekend.get_ColWidth(iCol)/nTwipsPerDotX+1;
		height = m_fgWeekend.get_RowHeight(iRow)/nTwipsPerDotY+1;
	}

	//�γ�ѡ�и����ڵľ�������
	CRect rcCell(x,y,x+width,y+height);
	//ת������ԶԻ��������
	rcCell.OffsetRect(rect.left+2,rect.top+2);	
	rcCell.InflateRect(-1,-1,-1,-1);
	//CString strValue = m_flexGrid.get_TextMatrix(iRow,iCol);

	//pDC->Draw3dRect(rcCell, RGB(255,0,0), RGB(192,192,192));
	//pDC->DrawText(_T("Fix"), &rcCell, DT_CENTER);

	return rcCell;
	ReleaseDC(pDC);

}


void CTStatScheduleDlg::CalcClickPos(CSize& size)
{

}


void CTStatScheduleDlg::OnEnKillfocusEditInput()
{
	CString strText;
	m_editInput.GetWindowText(strText);
	if (strText.GetLength() <= 0)
	{
		strText = _T("0");
// 		if (m_bInWorkDay)
// 		{
// 			strText = m_fgWorkDay.get_TextMatrix(m_szEditPosInWork.cx, m_szEditPosInWork.cy);
// 		}
// 		else
// 		{
// 			strText = m_fgWeekend.get_TextMatrix(m_szEditPosInWeekend.cx, m_szEditPosInWeekend.cy);
// 		}
		return;
	}

	int nValue = _wtoi(strText);
	// �Ϸ����ж�
	if ((m_szEditPosInWork.cy == 1 && m_bInWorkDay) || (m_szEditPosInWeekend.cy == 1 && !m_bInWorkDay)) // hour
	{
		if (nValue > 24)
		{
			AfxMessageBox(_T("Please input a number between 0 to 12."));
			return;
		}
	}
	else if((m_szEditPosInWork.cy == 2 && m_bInWorkDay) || (m_szEditPosInWeekend.cy == 2 && !m_bInWorkDay)) // minute
	{
		if (nValue > 60)
		{
			AfxMessageBox(_T("Please input a number between 0 to 60."));
			return;
		}
	}

	if (m_bInWorkDay)
	{
		m_fgWorkDay.put_TextMatrix(m_szEditPosInWork.cx, m_szEditPosInWork.cy, strText);
		CString strHour = m_fgWorkDay.get_TextMatrix(m_szEditPosInWork.cx, 1);
		CString strMin = m_fgWorkDay.get_TextMatrix(m_szEditPosInWork.cx, 2);
		CString strPMAM = m_fgWorkDay.get_TextMatrix(m_szEditPosInWork.cx, 3);

		CString strTime = strHour+_T(":")+strMin;	
		m_fgWorkDay.put_TextMatrix(m_szEditPosInWork.cx, 3, strTime);
	}
	else
	{
		m_fgWeekend.put_TextMatrix(m_szEditPosInWeekend.cx, m_szEditPosInWeekend.cy, strText);
		CString strHour = m_fgWeekend.get_TextMatrix(m_szEditPosInWeekend.cx, 1);
		CString strMin = m_fgWeekend.get_TextMatrix(m_szEditPosInWeekend.cx, 2);
		CString strPMAM = m_fgWeekend.get_TextMatrix(m_szEditPosInWeekend.cx, 3);

		CString strTime = strHour+_T(":")+strMin;	
		m_fgWeekend.put_TextMatrix(m_szEditPosInWeekend.cx, 3, strTime);

	}
}



void CTStatScheduleDlg::OnCbnSelchangeComboPmam()
{
	CString strText;

	if (m_bInWorkDay)
	{
		CComboBox * pCom = (CComboBox *)GetDlgItem(IDC_COMBO_PMAM);
		int nIdx = pCom->GetCurSel();
		pCom->GetLBText(nIdx, strText);
		m_fgWorkDay.put_TextMatrix(m_szEditPosInWork.cx, 3, strText);
	}
	else
	{
		CComboBox * pCom = (CComboBox *)GetDlgItem(IDC_COMBO_PMAM);
		int nIdx = pCom->GetCurSel();
		pCom->GetLBText(nIdx, strText);
		m_fgWeekend.put_TextMatrix(m_szEditPosInWeekend.cx, 3, strText);
	}
	
}

void CTStatScheduleDlg::OnCbnKillfocusComboPmam()
{
	CString strText;

	if (m_bInWorkDay)
	{
		//	m_fgWorkDay.put_TextMatrix(m_szEditPosInWork.cx, m_szEditPosInWork.cy, strText);
		CString strHour = m_fgWorkDay.get_TextMatrix(m_szEditPosInWork.cx, 1);
		CString strMin = m_fgWorkDay.get_TextMatrix(m_szEditPosInWork.cx, 2);
		//CString strPMAM = m_fgWorkDay.get_TextMatrix(m_szEditPosInWork.cx, 3);

		CString strTime = strHour+_T(":")+strMin;	
		m_fgWorkDay.put_TextMatrix(m_szEditPosInWork.cx, 3, strTime);
	}
	else
	{
		//	m_fgWeekend.put_TextMatrix(m_szEditPosInWeekend.cx, m_szEditPosInWeekend.cy, strText);
		CString strHour = m_fgWeekend.get_TextMatrix(m_szEditPosInWeekend.cx, 1);
		CString strMin = m_fgWeekend.get_TextMatrix(m_szEditPosInWeekend.cx, 2);
	//	CString strPMAM = m_fgWeekend.get_TextMatrix(m_szEditPosInWeekend.cx, 3);

		CString strTime = strHour+_T(":")+strMin;	
		m_fgWeekend.put_TextMatrix(m_szEditPosInWeekend.cx, 3, strTime);

	}
}

void CTStatScheduleDlg::OnBnClickedButtonSave()
{	
	BeginWaitCursor();
	//SaveToRegister();
	///*
	CString strTime,strHour,strMin,strPMAM;
	for (int i = 1; i <=4; i++)
	{
		strHour = m_fgWorkDay.get_TextMatrix(i,1);	
		strMin = m_fgWorkDay.get_TextMatrix(i,2);	
		 	
		
		int nHour = _wtoi(strHour);
		int nMin = _wtoi(strMin);
			
		int nWorkDayAwakeH = m_szRegTable[0]; 
		int nWorkDayAwakeM = m_szRegTable[1]; 
		write_one(g_tstat_id, nWorkDayAwakeH+(i-1)*2, nHour);
		write_one(g_tstat_id, nWorkDayAwakeM+(i-1)*2, nMin);	

		// 		int nHour2 = multi_register_value[466+i];
		// 		int nMin2 = multi_register_value[467+i];
		// 		int nSec2 = multi_register_value[1];

		//////////////////////////////////////////////////////////////////////////

		strHour = m_fgWeekend.get_TextMatrix(i,1);	
		strMin = m_fgWeekend.get_TextMatrix(i,2);	
		strPMAM = m_fgWeekend.get_TextMatrix(i,3);	

		nHour = _wtoi(strHour);
		nMin = _wtoi(strMin);
		int nWeekenAwakeH = m_szRegTable[SCHED_WEEKEN_AWAKE_HOUR]; 
		int nWeekenAwakeM = m_szRegTable[SCHED_WEEKEN_AWAKE_MIN]; 
		write_one(g_tstat_id, nWeekenAwakeH+(i-1)*2, nHour);
		write_one(g_tstat_id, nWeekenAwakeM+(i-1)*2, nMin);	
	}
//*/
	EndWaitCursor();
}



void CTStatScheduleDlg::InitRegisterTable()
{
	m_szRegTable.clear();

	if (multi_register_value[7] == PM_TSTAT6 || multi_register_value[7] == PM_TSTAT7 ) // TStat 6
	{
		for (int i = 0 ; i < 16; i++)
		{
			m_szRegTable.push_back(MODBUS_DAY1_EVENT1_HI + i);
		}		
	}
 
}

void CTStatScheduleDlg::OnBnClickedButtonExit()
{
	
	//OnBnClickedButtonSave();
	OnOK();

}




void CTStatScheduleDlg::OnBnClickedRadio24h()
{
	//Write_One(g_tstat_id, TIME_FORMAT_REG, 1);
}

void CTStatScheduleDlg::OnBnClickedRadio12h()
{
//	Write_One(g_tstat_id, TIME_FORMAT_REG, 0);	
}


void CTStatScheduleDlg::OnCbnSelchangeComboOnoff()
{
	int sel=m_scheduleonoff.GetCurSel();
	int ret;
	if (product_register_value[MODBUS_SCHEDULE_ON_OFF]!=sel)
	 {
	   ret=write_one(g_tstat_id,MODBUS_SCHEDULE_ON_OFF,sel);
	   if (ret>0)
	   {
	   product_register_value[MODBUS_SCHEDULE_ON_OFF]=sel;
	   m_scheduleonoff.SetCurSel(sel);

	   } 
	   else
	   {
	   m_scheduleonoff.SetCurSel(product_register_value[MODBUS_SCHEDULE_ON_OFF]);
	   }
	 } 
	 else
	 {
	 }
}
