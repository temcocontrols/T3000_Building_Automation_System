// T3ModulesView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T3ModulesView.h"
#include "globle_function.h"
#include "T3RTDView.h"
// CT3ModulesView

IMPLEMENT_DYNCREATE(CT3ModulesView, CFormView)

	CT3ModulesView::CT3ModulesView()
	: CFormView(CT3ModulesView::IDD)
{

}

CT3ModulesView::~CT3ModulesView()
{
}

void CT3ModulesView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_T3INPUTS, m_T3_Input_List);
}

BEGIN_MESSAGE_MAP(CT3ModulesView, CFormView)
END_MESSAGE_MAP()


// CT3ModulesView diagnostics

#ifdef _DEBUG
void CT3ModulesView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CT3ModulesView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CT3ModulesView message handlers


void CT3ModulesView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

}

void CT3ModulesView::Fresh(){

	g_hwnd_now = this->m_hWnd;
	m_sn=product_register_value[0]+product_register_value[1]*256+product_register_value[2]*256*256+product_register_value[3]*256*256*256;	
	CString strTemp; 
	m_T3_Input_List.ShowWindow(SW_HIDE);
	m_T3_Input_List.DeleteAllItems();  
	while ( m_T3_Input_List.DeleteColumn (0)) ;
	if (product_register_value[7]==PM_T34AO)
	{  
		m_T3_Input_List.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
		m_T3_Input_List.SetExtendedStyle(m_T3_Input_List.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));	
		m_T3_Input_List.InsertColumn(0, _T("Label"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
		//m_T3_Input_List.InsertColumn(1, _T("Full Label"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(1, _T("Value"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(2, _T("Date Stamp"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(3, _T("Range"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(4, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

		for (int i=0;i<10;i++)
		{
			strTemp=Get_Table_Name(m_sn,_T("Input"),i+1);
			m_T3_Input_List.InsertItem(i,strTemp);
		}


		//==================================================Input============================================================
		//T3-4AO
		//address: input 119~138  INPUT1-10 119(high word) 120(low word)
		//address:Switch 
		//Range Setting for each input  200
		/*
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
		*/
		CString strresult;
		int regValue;
		for(int i = 1;i<=10;i++)
		{  
			regValue=product_register_value[119+2*(i-1)]*65535+product_register_value[120+2*(i-1)];
			if (0==product_register_value[200+i-1])
			{
				strresult.Format(_T("%d"),regValue);

			} 
			else if (1==product_register_value[200+i-1])
			{
				strresult.Format(_T("%.1f C"),(float)regValue/10.0);
			}
			else if (2==product_register_value[200+i-1])
			{
				strresult=_T("10K F");
				strresult.Format(_T("%.1f F"),(float)regValue/10.0);
			}
			else if (3==product_register_value[200+i-1])
			{

				strresult.Format(_T("%.1f"),(float)regValue);
				strresult+=_T("%");
			}
			else if (4==product_register_value[200+i-1])
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
			else if (5==product_register_value[200+i-1])
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
			else if (6==product_register_value[200+i-1])
			{
				strresult.Format(_T("%d"),regValue);
			}
			else if (7==product_register_value[200+i-1])
			{
				strresult.Format(_T("%d"),regValue);
			}
			else if (8==product_register_value[200+i-1])
			{
				//strresult=_T("TYPE3 10K C");
				strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
			}
			else if (9==product_register_value[200+i-1])
			{
				/*strresult=_T("TYPE3 10K F");*/
				strresult.Format(_T("%0.1f F"),(float)regValue/10.0);
			}
			else if (10==product_register_value[200+i-1])
			{
				strresult=_T("0");
				//strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
			}
			else if (11==product_register_value[200+i-1])
			{
				//strresult=_T("0-5V");
				strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
			}
			else if (12==product_register_value[200+i-1])
			{
				//strresult=_T("0-10V");
				strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
			}
			else if (13==product_register_value[200+i-1])
			{
				//strresult=_T("0-20I");
				strresult.Format(_T("%0.1f ma"),(float)regValue/1000.0);
			}
			 
			m_T3_Input_List.SetItemText(i-1,1,strresult);
			strresult.Format(_T("%4d/%4d/%4d  %2d:%2d"),product_register_value[139+3*(i-1)],product_register_value[140+3*(i-1)],product_register_value[141+3*(i-1)],product_register_value[142+3*(i-1)],product_register_value[143+3*(i-1)]);
			 
			m_T3_Input_List.SetItemText(i-1,2,strresult);
			strresult.Format(_T("%d"),product_register_value[200+i-1]);
			if (0==product_register_value[200+i-1])
			{
				strresult=_T("RAW DATA");
			} 
			else if (1==product_register_value[200+i-1])
			{
				strresult=_T("T10K C");
			}
			else if (2==product_register_value[200+i-1])
			{
				strresult=_T("10K F");
			}
			else if (3==product_register_value[200+i-1])
			{
				strresult=_T("0-100%");
			}
			else if (4==product_register_value[200+i-1])
			{
				strresult=_T("ON/OFF");
			}
			else if (5==product_register_value[200+i-1])
			{
				strresult=_T("OFF/ON");
			}
			else if (6==product_register_value[200+i-1])
			{
				strresult=_T("Pulse Input");
			}
			else if (7==product_register_value[200+i-1])
			{
				strresult=_T("Lighting Control");
			}
			else if (8==product_register_value[200+i-1])
			{
				strresult=_T("TYPE3 10K C");
			}
			else if (9==product_register_value[200+i-1])
			{
				strresult=_T("TYPE3 10K F");
			}
			else if (10==product_register_value[200+i-1])
			{
				strresult=_T("NO USE");
			}
			else if (11==product_register_value[200+i-1])
			{
				strresult=_T("0-5V");
			}
			else if (12==product_register_value[200+i-1])
			{
				strresult=_T("0-10V");
			}
			else if (13==product_register_value[200+i-1])
			{
				strresult=_T("0-20 ma");
			}

		 
			m_T3_Input_List.SetItemText(i-1,3,strresult);
			strresult.Format(_T("%d"),product_register_value[210+i-1]);
			m_T3_Input_List.SetItemText(i-1,4,strresult); 
		}
	}
	else if (product_register_value[7]==PM_T38I13O)
	{
		m_T3_Input_List.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
		m_T3_Input_List.SetExtendedStyle(m_T3_Input_List.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));	
		m_T3_Input_List.InsertColumn(0, _T("Label"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
		//m_T3_Input_List.InsertColumn(1, _T("Full Label"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(1, _T("Value"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(2, _T("Date Stamp"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(3, _T("Range"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(4, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(5, _T("Lighting Time"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(6, _T("Time Left"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

		for (int i=0;i<8;i++)
		{
			strTemp=Get_Table_Name(m_sn,_T("Input"),i+1);
			m_T3_Input_List.InsertItem(i,strTemp);
		}
		// Input
		CString strresult;
		int regValue;
		for(int i = 1;i<=8;i++)
		{  


			strresult.Format(_T("%4d/%4d/%4d  %2d:%2d"),product_register_value[134+3*(i-1)]
													   ,product_register_value[135+3*(i-1)]
													   ,product_register_value[136+3*(i-1)]
													   ,product_register_value[137+3*(i-1)]
													   ,product_register_value[138+3*(i-1)]);
			 
			m_T3_Input_List.SetItemText(i-1,2,strresult);
			strresult.Format(_T("%d"),product_register_value[183+i-1]);
			if (0==product_register_value[183+i-1])
			{
				strresult=_T("RAW DATA");

			} 
			else if (1==product_register_value[183+i-1])
			{
				strresult=_T("10K C");
			}
			else if (2==product_register_value[183+i-1])
			{
				strresult=_T("10K F");
			}
			else if (3==product_register_value[183+i-1])
			{
				strresult=_T("0-100%");
			}
			else if (4==product_register_value[183+i-1])
			{
				strresult=_T("ON/OFF");
			}
			else if (5==product_register_value[183+i-1])
			{
				strresult=_T("OFF/ON");
			}
			else if (6==product_register_value[183+i-1])
			{
				strresult=_T("Pulse Input");
			}
			else if (7==product_register_value[183+i-1])
			{
				strresult=_T("Lighting Control");
			}
			else if (8==product_register_value[183+i-1])
			{
				strresult=_T("TYPE3 10K C");
			}
			else if (9==product_register_value[183+i-1])
			{
				strresult=_T("TYPE3 10K F");
			}
			else if (10==product_register_value[183+i-1])
			{
				strresult=_T("NO USE");
			}
			else if (11==product_register_value[183+i-1])
			{
				strresult=_T("0-5V");
			}
			else if (12==product_register_value[183+i-1])
			{
				strresult=_T("0-10V");
			}
			else if (13==product_register_value[183+i-1])
			{
				strresult=_T("0-20 ma");
			}

			m_T3_Input_List.SetItemText(i-1,3,strresult);


			regValue=(short)product_register_value[119+2*(i-1)];

			if (0==product_register_value[183+i-1])
			{
				strresult.Format(_T("%d"),regValue);

			} 
			else if (1==product_register_value[183+i-1])
			{
				strresult.Format(_T("%.1f C"),(float)regValue/10.0);
			}
			else if (2==product_register_value[183+i-1])
			{
				strresult=_T("10K F");
				strresult.Format(_T("%.1f F"),(float)regValue/10.0);
			}
			else if (3==product_register_value[183+i-1])
			{

				strresult.Format(_T("%.1f"),(float)regValue);
				strresult+=_T("%");
			}
			else if (4==product_register_value[183+i-1])
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
			else if (5==product_register_value[183+i-1])
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
			else if (6==product_register_value[183+i-1])
			{
				strresult.Format(_T("%d"),regValue);
			}
			else if (7==product_register_value[183+i-1])
			{
				strresult.Format(_T("%d"),regValue);
			}
			else if (8==product_register_value[183+i-1])
			{
				//strresult=_T("TYPE3 10K C");
				strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
			}
			else if (9==product_register_value[183+i-1])
			{
				/*strresult=_T("TYPE3 10K F");*/
				strresult.Format(_T("%0.1f F"),(float)regValue/10.0);
			}
			else if (10==product_register_value[183+i-1])
			{
				strresult=_T("0");
				//strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
			}
			else if (11==product_register_value[183+i-1])
			{
				//strresult=_T("0-5V");
				strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
			}
			else if (12==product_register_value[183+i-1])
			{
				//strresult=_T("0-10V");
				strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
			}
			else if (13==product_register_value[183+i-1])
			{
				//strresult=_T("0-20I");
				strresult.Format(_T("%0.1f ma"),(float)regValue/1000.0);
			}

			m_T3_Input_List.SetItemText(i-1,1,strresult);


			strresult.Format(_T("%d"),product_register_value[191+i-1]);
			m_T3_Input_List.SetItemText(i-1,4,strresult);

			strresult.Format(_T("%d min"),product_register_value[199+i-1]);
			m_T3_Input_List.SetItemText(i-1,5,strresult);

			strresult.Format(_T("%d min"),product_register_value[207+i-1]);
			m_T3_Input_List.SetItemText(i-1,6,strresult);
		}

	}
	else if (product_register_value[7]==PM_T36CT)
	{
		m_T3_Input_List.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
		m_T3_Input_List.SetExtendedStyle(m_T3_Input_List.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));	
		m_T3_Input_List.InsertColumn(0, _T("Label"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
		m_T3_Input_List.InsertColumn(1, _T("Value"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(2, _T("Range"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(3, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

		for (int i=0;i<6;i++)
		{
			strTemp=Get_Table_Name(m_sn,_T("Input"),i+1);
			m_T3_Input_List.InsertItem(i,strTemp);
		}
		for (int i=6;i<16;i++)
		{
			strTemp=Get_Table_Name(m_sn,_T("Analog_Input"),i+1);
			m_T3_Input_List.InsertItem(i,strTemp);
		}
		CString strresult;
		int regValue;
		for(int i = 1;i<=6;i++)
		{  
			regValue=product_register_value[108+i-1];


			if (0==product_register_value[126+i-1])
			{
				strresult.Format(_T("%d"),regValue);

			} 
			else if (1==product_register_value[126+i-1])
			{
				strresult.Format(_T("%.1f C"),(float)regValue/10.0);
			}
			else if (2==product_register_value[126+i-1])
			{
				strresult=_T("10K F");
				strresult.Format(_T("%.1f F"),(float)regValue/10.0);
			}
			else if (3==product_register_value[126+i-1])
			{

				strresult.Format(_T("%.1f"),(float)regValue);
				strresult+=_T("%");
			}
			else if (4==product_register_value[126+i-1])
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
			else if (5==product_register_value[126+i-1])
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
			else if (6==product_register_value[126+i-1])
			{
				strresult.Format(_T("%d"),regValue);
			}
			else if (7==product_register_value[126+i-1])
			{
				strresult.Format(_T("%d"),regValue);
			}
			else if (8==product_register_value[126+i-1])
			{
				//strresult=_T("TYPE3 10K C");
				strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
			}
			else if (9==product_register_value[126+i-1])
			{
				/*strresult=_T("TYPE3 10K F");*/
				strresult.Format(_T("%0.1f F"),(float)regValue/10.0);
			}
			else if (10==product_register_value[126+i-1])
			{
				strresult=_T("0");
				//strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
			}
			else if (11==product_register_value[126+i-1])
			{
				//strresult=_T("0-5V");
				strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
			}
			else if (12==product_register_value[126+i-1])
			{
				//strresult=_T("0-10V");
				strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
			}
			else if (13==product_register_value[126+i-1])
			{
				//strresult=_T("0-20I");
				strresult.Format(_T("%0.1f ma"),(float)regValue/1000.0);
			}

			 
			m_T3_Input_List.SetItemText(i-1,1,strresult);

			strresult.Format(_T("%d"),product_register_value[126+i-1]);
			if (0==product_register_value[126+i-1])
			{
				strresult=_T("RAW DATA");
			} 
			else if (1==product_register_value[126+i-1])
			{
				strresult=_T("10K C");
			}
			else if (2==product_register_value[126+i-1])
			{
				strresult=_T("10K F");
			}
			else if (3==product_register_value[126+i-1])
			{
				strresult=_T("0-100%");
			}
			else if (4==product_register_value[126+i-1])
			{
				strresult=_T("ON/OFF");
			}
			else if (5==product_register_value[126+i-1])
			{
				strresult=_T("OFF/ON");
			}
			else if (6==product_register_value[126+i-1])
			{
				strresult=_T("Pulse Input");
			}
			else if (7==product_register_value[126+i-1])
			{
				strresult=_T("Lighting Control");
			}
			else if (8==product_register_value[126+i-1])
			{
				strresult=_T("TYPE3 10K C");
			}
			else if (9==product_register_value[126+i-1])
			{
				strresult=_T("TYPE3 10K F");
			}
			else if (10==product_register_value[126+i-1])
			{
				strresult=_T("NO USE");
			}
			else if (11==product_register_value[126+i-1])
			{
				strresult=_T("0-5V");
			}
			else if (12==product_register_value[126+i-1])
			{
				strresult=_T("0-10V");
			}
			else if (13==product_register_value[126+i-1])
			{
				strresult=_T("0-20 ma");
			}

			 
			m_T3_Input_List.SetItemText(i-1,2,strresult);
			strresult.Format(_T("%d"),product_register_value[142+i-1]);
		 
			m_T3_Input_List.SetItemText(i-1,3,strresult);
		}


		for(int i = 7;i<=16;i++)
		{  
			regValue=product_register_value[114+i-7];
			if (0==product_register_value[132+i-7])
			{
				strresult.Format(_T("%d"),regValue);
			} 
			else if (1==product_register_value[132+i-7])
			{
				regValue=(short)product_register_value[114+i-7];

				strresult.Format(_T("%.1f C"),(float)regValue/10.0);
			}
			else if (2==product_register_value[132+i-7])
			{

				regValue=(short)product_register_value[114+i-7];
				strresult=_T("10K F");
				strresult.Format(_T("%.1f F"),(float)regValue/10.0);
			}
			else if (3==product_register_value[132+i-7])
			{

				strresult.Format(_T("%.1f"),(float)regValue);
				strresult+=_T("%");
			}
			else if (4==product_register_value[132+i-7])
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
			else if (5==product_register_value[132+i-7])
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
			else if (6==product_register_value[132+i-7])
			{
				strresult.Format(_T("%d"),regValue);
			}
			else if (7==product_register_value[132+i-7])
			{
				strresult.Format(_T("%d"),regValue);
			}
			else if (8==product_register_value[132+i-7])
			{
				//strresult=_T("TYPE3 10K C");
				strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
			}
			else if (9==product_register_value[132+i-7])
			{
				/*strresult=_T("TYPE3 10K F");*/
				strresult.Format(_T("%0.1f F"),(float)regValue/10.0);
			}
			else if (10==product_register_value[132+i-7])
			{
				strresult=_T("0");
				//strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
			}
			else if (11==product_register_value[132+i-7])
			{
				//strresult=_T("0-5V");
				strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
			}
			else if (12==product_register_value[132+i-1])
			{
				//strresult=_T("0-10V");
				strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
			}
			else if (13==product_register_value[132+i-7])
			{

				strresult.Format(_T("%0.1f ma"),(float)regValue/1000.0);
			}
			 
			 m_T3_Input_List.SetItemText(i-1,1,strresult);

			strresult.Format(_T("%d"),product_register_value[132+i-7]);
			if (0==product_register_value[132+i-1])
			{
				strresult=_T("RAW DATA");
			} 
			else if (1==product_register_value[132+i-7])
			{
				strresult=_T("10K C");
			}
			else if (2==product_register_value[132+i-7])
			{
				strresult=_T("10K F");
			}
			else if (3==product_register_value[132+i-7])
			{
				strresult=_T("0-100%");
			}
			else if (4==product_register_value[132+i-7])
			{
				strresult=_T("ON/OFF");
			}
			else if (5==product_register_value[132+i-7])
			{
				strresult=_T("OFF/ON");
			}
			else if (6==product_register_value[132+i-7])
			{
				strresult=_T("Pulse Input");
			}
			else if (7==product_register_value[132+i-7])
			{
				strresult=_T("Lighting Control");
			}
			else if (8==product_register_value[132+i-7])
			{
				strresult=_T("TYPE3 10K C");
			}
			else if (9==product_register_value[132+i-7])
			{
				strresult=_T("TYPE3 10K F");
			}
			else if (10==product_register_value[132+i-7])
			{
				strresult=_T("NO USE");
			}
			else if (11==product_register_value[132+i-7])
			{
				strresult=_T("0-5V");
			}
			else if (12==product_register_value[132+i-7])
			{
				strresult=_T("0-10V");
			}
			else if (13==product_register_value[132+i-7])
			{
				strresult=_T("0-20 ma");
			}

			 
			m_T3_Input_List.SetItemText(i-1,2,strresult);
			strresult.Format(_T("%d"),product_register_value[148+i-7]);
			m_T3_Input_List.SetItemText(i-1,3,strresult);
		 
		}

	}
	else if (product_register_value[7]==PM_T3IOA)
	{
		m_T3_Input_List.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
		m_T3_Input_List.SetExtendedStyle(m_T3_Input_List.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));	
		m_T3_Input_List.InsertColumn(0, _T("Label"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
		m_T3_Input_List.InsertColumn(1, _T("Value"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(2, _T("Range"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(3, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

		for (int i=0;i<8;i++)
		{
			strTemp=Get_Table_Name(m_sn,_T("Input"),i+1);
			m_T3_Input_List.InsertItem(i,strTemp);
		}

		CString strresult;
		int regValue;
		for(int i = 1;i<=8;i++)
		{  
			regValue=product_register_value[108];


			if (0==product_register_value[118+i-1])
			{
				strresult.Format(_T("%d"),regValue);

			} 
			else if (1==product_register_value[118+i-1])
			{
				strresult.Format(_T("%.1f C"),(float)regValue/10.0);
			}
			else if (2==product_register_value[118+i-1])
			{
				strresult=_T("10K F");
				strresult.Format(_T("%.1f F"),(float)regValue/10.0);
			}
			else if (3==product_register_value[118+i-1])
			{

				strresult.Format(_T("%.1f"),(float)regValue);
				strresult+=_T("%");
			}
			else if (4==product_register_value[118+i-1])
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
			else if (5==product_register_value[118+i-1])
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
			else if (6==product_register_value[118+i-1])
			{
				strresult.Format(_T("%d"),regValue);
			}
			else if (7==product_register_value[118+i-1])
			{
				strresult.Format(_T("%d"),regValue);
			}
			else if (8==product_register_value[118+i-1])
			{
				//strresult=_T("TYPE3 10K C");
				strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
			}
			else if (9==product_register_value[118+i-1])
			{
				/*strresult=_T("TYPE3 10K F");*/
				strresult.Format(_T("%0.1f F"),(float)regValue/10.0);
			}
			else if (10==product_register_value[118+i-1])
			{
				strresult=_T("0");
				//strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
			}
			else if (11==product_register_value[118+i-1])
			{
				//strresult=_T("0-5V");
				strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
			}
			else if (12==product_register_value[118+i-1])
			{
				//strresult=_T("0-10V");
				strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
			}
			else if (13==product_register_value[118+i-1])
			{
				//strresult=_T("0-20I");
				strresult.Format(_T("%0.1f ma"),(float)regValue/1000.0);
			}


			 
			 m_T3_Input_List.SetItemText(i-1,1,strresult);

			strresult.Format(_T("%d"),product_register_value[118+i-1]);
			if (0==product_register_value[118+i-1])
			{
				strresult=_T("RAW DATA");
			} 
			else if (1==product_register_value[118+i-1])
			{
				strresult=_T("10K C");
			}
			else if (2==product_register_value[118+i-1])
			{
				strresult=_T("10K F");
			}
			else if (3==product_register_value[118+i-1])
			{
				strresult=_T("0-100%");
			}
			else if (4==product_register_value[118+i-1])
			{
				strresult=_T("ON/OFF");
			}
			else if (5==product_register_value[118+i-1])
			{
				strresult=_T("OFF/ON");
			}
			else if (6==product_register_value[118+i-1])
			{
				strresult=_T("Pulse Input");
			}
			else if (7==product_register_value[118+i-1])
			{
				strresult=_T("Lighting Control");
			}
			else if (8==product_register_value[118+i-1])
			{
				strresult=_T("TYPE3 10K C");
			}
			else if (9==product_register_value[118+i-1])
			{
				strresult=_T("TYPE3 10K F");
			}
			else if (10==product_register_value[118+i-1])
			{
				strresult=_T("NO USE");
			}
			else if (11==product_register_value[118+i-1])
			{
				strresult=_T("0-5V");
			}
			else if (12==product_register_value[118+i-1])
			{
				strresult=_T("0-10V");
			}
			else if (13==product_register_value[118+i-1])
			{
				strresult=_T("0-20 ma");
			}

			 
			 m_T3_Input_List.SetItemText(i-1,2,strresult);
			strresult.Format(_T("%d"),product_register_value[126+i-1]);
			 m_T3_Input_List.SetItemText(i-1,3,strresult);
		}
	}
	else if (product_register_value[7]==PM_T332AI)
	{
		m_T3_Input_List.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
		m_T3_Input_List.SetExtendedStyle(m_T3_Input_List.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));	
		m_T3_Input_List.InsertColumn(0, _T("Label"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
		m_T3_Input_List.InsertColumn(1, _T("Value"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(2, _T("Range"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(3, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
		for (int i=0;i<32;i++)
		{
			strTemp=Get_Table_Name(m_sn,_T("Input"),i+1);
			m_T3_Input_List.InsertItem(i,strTemp);
		}

		CString strresult;
		int regValue;
		for(int i = 1;i<=32;i++)
		{  
		 
			
			if (0==product_register_value[228+i-1])
			{
				strresult=_T("RAW DATA");

			} 
			else if (1==product_register_value[228+i-1])
			{
				strresult=_T("10K C");
			}
			else if (2==product_register_value[228+i-1])
			{
				strresult=_T("10K F");
			}
			else if (3==product_register_value[228+i-1])
			{
				strresult=_T("0-100%");
			}
			else if (4==product_register_value[228+i-1])
			{
				strresult=_T("ON/OFF");
			}
			else if (5==product_register_value[228+i-1])
			{
				strresult=_T("OFF/ON");
			}
			else if (6==product_register_value[228+i-1])
			{
				strresult=_T("Pulse Input");
			}
			else if (7==product_register_value[228+i-1])
			{
				strresult=_T("Lighting Control");
			}
			else if (8==product_register_value[228+i-1])
			{
				strresult=_T("TYPE3 10K C");
			}
			else if (9==product_register_value[228+i-1])
			{
				strresult=_T("TYPE3 10K F");
			}
			else if (10==product_register_value[228+i-1])
			{
				strresult=_T("NO USE");
			}
			else if (11==product_register_value[228+i-1])
			{
				strresult=_T("0-5V");
			}
			else if (12==product_register_value[228+i-1])
			{
				strresult=_T("0-10V");
			}
			else if (13==product_register_value[228+i-1])
			{
				strresult=_T("0-20ma");
			}

			 
			m_T3_Input_List.SetItemText(i-1,1,strresult);

			regValue=product_register_value[100+i-1];
			//regValue=(short)product_register_value[INPUT1_PULSE_COUNT_LOW+2*(i-1)];

			if (0==product_register_value[228+i-1])
			{
				strresult.Format(_T("%d"),regValue);

			} 
			else if (1==product_register_value[228+i-1])
			{
				strresult.Format(_T("%.1f C"),(float)regValue/10.0);
			}
			else if (2==product_register_value[228+i-1])
			{
				strresult=_T("10K F");
				strresult.Format(_T("%.1f F"),(float)regValue/10.0);
			}
			else if (3==product_register_value[228+i-1])
			{

				strresult.Format(_T("%.1f"),(float)regValue);
				strresult+=_T("%");
			}
			else if (4==product_register_value[228+i-1])
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
			else if (5==product_register_value[228+i-1])
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
			else if (6==product_register_value[228+i-1])
			{
				strresult.Format(_T("%d"),regValue);
			}
			else if (7==product_register_value[228+i-1])
			{
				strresult.Format(_T("%d"),regValue);
			}
			else if (8==product_register_value[228+i-1])
			{
				//strresult=_T("TYPE3 10K C");
				strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
			}
			else if (9==product_register_value[228+i-1])
			{
				/*strresult=_T("TYPE3 10K F");*/
				strresult.Format(_T("%0.1f F"),(float)regValue/10.0);
			}
			else if (10==product_register_value[228+i-1])
			{
				strresult=_T("0");
				//strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
			}
			else if (11==product_register_value[228+i-1])
			{
				//strresult=_T("0-5V");
				strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
			}
			else if (12==product_register_value[228+i-1])
			{
				//strresult=_T("0-10V");
				strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
			}
			else if (13==product_register_value[228+i-1])
			{
				//strresult=_T("0-20I");
				strresult.Format(_T("%0.1f ma"),(float)regValue/1000.0);
			}
			 
			m_T3_Input_List.SetItemText(i-1,2,strresult);
			strresult.Format(_T("%d"),product_register_value[260+i-1]);
			 m_T3_Input_List.SetItemText(i-1,3,strresult);
		}
	}
	else if (product_register_value[7]==PM_T3PT10)
	{
		m_T3_Input_List.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
		m_T3_Input_List.SetExtendedStyle(m_T3_Input_List.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));	
		m_T3_Input_List.InsertColumn(0, _T("Channel Number"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
		m_T3_Input_List.InsertColumn(1, _T("Channel Name"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(2, _T("Value"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(3, _T("Range"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(4, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
		m_T3_Input_List.InsertColumn(5, _T("Status"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

		for (int i=1;i<11;i++)
		{
			strTemp.Format(_T("%d"),i);
			m_T3_Input_List.InsertItem(i-1,strTemp);
			strTemp=Get_Table_Name(m_sn,_T("PT_Channel"),i+1);
			m_T3_Input_List.SetItemText(i-1,1,strTemp);

		}
		CString strresult;
		int regValue;
		if (product_register_value[13]==0)//Float
		{
			int accuracy=product_register_value[18];
			for(int i = 1;i<=10;i++)
			{   if (0==product_register_value[228+i-1])
			{
				strresult=_T("C");
			} 
			else if (1==product_register_value[228+i-1])
			{
				strresult=_T("F");
			}
			else if (2==product_register_value[228+i-1])
			{
				strresult=_T("¦¸");
			}
			m_T3_Input_List.SetItemText(i-1,3,strresult);
			 
			DataFormat mydata;
			CString unit=strresult;
			regValue=0;
			regValue=product_register_value[101+2*(i-1)];
			int Data=regValue;
			int Data1=regValue&0x00FF;
			int Data2=(regValue&0xFF00)>>8;
			mydata.charData[3]=Data2;
			mydata.charData[2]=Data1;

			regValue=0;
			regValue=product_register_value[100+2*(i-1)];
			Data=regValue;
			Data1=regValue&0x00FF;
			Data2=(regValue&0xFF00)>>8;
			mydata.charData[1]=Data2;
			mydata.charData[0]=Data1;
			if (1==accuracy)
			{
				strresult.Format(_T("%0.1f"),mydata.floatData);
			} 
			else if (2==accuracy)
			{
				strresult.Format(_T("%0.2f"),mydata.floatData);
			}
			else if (3==accuracy)
			{
				strresult.Format(_T("%0.3f"),mydata.floatData);
			}
			else if (4==accuracy)
			{
				strresult.Format(_T("%0.4f"),mydata.floatData);
			}
			else if (5==accuracy)
			{
				strresult.Format(_T("%0.5f"),mydata.floatData);
			}
			else if (6==accuracy)
			{
				strresult.Format(_T("%0.6f"),mydata.floatData);
			}


			strresult+=unit;
			 
			m_T3_Input_List.SetItemText(i-1,2,strresult);
			strresult.Format(_T("%d"),product_register_value[238+i-1]);
			 
			m_T3_Input_List.SetItemText(i-1,4,strresult);
			if (0==product_register_value[248+i-1])
			{
				strresult=_T("Pass");
			} 
			else if (1==product_register_value[248+i-1])
			{
				strresult=_T("Unconnect");
			}
			else if (2==product_register_value[248+i-1])
			{
				strresult=_T("Unconnect");
			}
			 
			m_T3_Input_List.SetItemText(i-1,5,strresult);
			}
		}
		else//Int
		{
			for(int i = 1;i<=10;i++)
			{  
				if (0==product_register_value[228+i-1])
				{
					strresult=_T("C");
				} 
				else if (1==product_register_value[228+i-1])
				{
					strresult=_T("F");
				}
				else if (2==product_register_value[228+i-1])
				{
					strresult=_T("¦¸");
				}
				 
				m_T3_Input_List.SetItemText(i-1,3,strresult);
				CString unit=strresult;
				regValue=product_register_value[101+2*(i-1)];
				strresult.Format(_T("%0.1f"),((float)regValue)/10);
				strresult+=unit;
				 
				m_T3_Input_List.SetItemText(i-1,2,strresult);
				strresult.Format(_T("%d"),product_register_value[238+i-1]);
				 
				m_T3_Input_List.SetItemText(i-1,4,strresult);
				if (0==product_register_value[248+i-1])
				{
					strresult=_T("Pass");
				} 
				else if (1==product_register_value[248+i-1])
				{
					strresult=_T("Short");
				}
				else if (2==product_register_value[248+i-1])
				{
					strresult=_T("Unconnect");
				}
				 
				m_T3_Input_List.SetItemText(i-1,5,strresult);
			}
		}
	}
	else 
	{
	AfxMessageBox(_T("This Product is not T3 Modules!")) ;
	}
	m_T3_Input_List.ShowWindow(SW_SHOW);
}

