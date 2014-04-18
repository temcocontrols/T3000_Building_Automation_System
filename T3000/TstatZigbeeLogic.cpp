// TstatZigbeeLogic.cpp : implementation file
// This file is code by Fance 2014/03
// Use this file to get the RSSI of all the tstat under the NC , and shows dynamic rssi of the tstat.

#include "stdafx.h"
#include "T3000.h"
#include "TstatZigbeeLogic.h"
#include "afxdialogex.h"
#include "globle_function.h"


#define WM_SHOW_ID_LIST WM_USER + 277

const int FRESH_ID_LIST = 1;
const int FRESH_MAIN_LIST = 2;

HANDLE hGetZigbeeInfo;
DWORD  nZigbeeInfoThreadID;
unsigned short zigbee_register_value[1024]={-1};
int NC_ID;
static int loop_count = 0;
signed char RSSI[256][256];
// CTstatZigbeeLogic dialog

IMPLEMENT_DYNAMIC(CTstatZigbeeLogic, CDialogEx)

CTstatZigbeeLogic::CTstatZigbeeLogic(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTstatZigbeeLogic::IDD, pParent)
{

}

CTstatZigbeeLogic::~CTstatZigbeeLogic()
{
}

void CTstatZigbeeLogic::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ZIGBEE_LOGIC_LIST, m_zigbee_logic_list);
	DDX_Control(pDX, IDC_LIST_FIND_TSTAT_ID, m_tstat_id_list);
}


BEGIN_MESSAGE_MAP(CTstatZigbeeLogic, CDialogEx)
	ON_MESSAGE(WM_SHOW_ID_LIST, HandleMessage)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTstatZigbeeLogic message handlers


BOOL CTstatZigbeeLogic::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetTimer(1,5000,NULL);
	loop_count = 0;
	Initial_ID_List();
	

	m_zigbee_logic_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	//m_monitor_list.SetExtendedStyle(m_monitor_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_zigbee_logic_list.SetExtendedStyle(m_tstat_id_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_zigbee_logic_list.Support_Keyboard(false);
	m_zigbee_logic_list.SetWhetherShowBkCol(false);

	Zigbee_Tstat_Id.push_back(g_tstat_id);	//第一个点击的 tstat 的ID先加到表里面去,因为它的寄存器里面没有它自己的;
	Zigbee_Show_Tstat_Id.push_back(g_tstat_id);

	// TODO:  Add extra initialization here
	hGetZigbeeInfo =CreateThread(NULL,NULL,GetZigbeeInfo,this,NULL, &nZigbeeInfoThreadID);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CTstatZigbeeLogic::Initial_ID_List()
{
	m_tstat_id_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	//m_monitor_list.SetExtendedStyle(m_monitor_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_tstat_id_list.SetExtendedStyle(m_tstat_id_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_tstat_id_list.InsertColumn(0, _T("ID"), 100, ListCtrlEx::CheckBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
}

DWORD WINAPI   CTstatZigbeeLogic::GetZigbeeInfo(LPVOID lpVoid)
{
	//Read_Multi(g_tstat_id,&multi_register_value[i*64],i*64,64);

	CTstatZigbeeLogic *mparent = (CTstatZigbeeLogic *)lpVoid;
	//mparent->Zigbee_Tstat_Id.clear();
	//mparent->Zigbee_Show_Tstat_Id.clear();
	memset(RSSI,0,sizeof(RSSI));
	loop_count = 0;
	

	NC_ID = read_one(255,6,2);	//读到NC 的ID， NC 的表里面也没有NC和其他的 RSSI 的值;
	mparent->HandleOneTSTAT(g_tstat_id);
	mparent->PostMessage(WM_SHOW_ID_LIST,FRESH_MAIN_LIST,NULL);	//处理完成后更新 List ;
	hGetZigbeeInfo = NULL;
	return 0;
}
//两个线程同时操作一个vector可能会出问题，稍后修改;
void CTstatZigbeeLogic::HandleOneTSTAT(unsigned char nId)
{
	int nRetsingle = 0;
	int nRetMulty = 0;
	nRetsingle = read_one(nId,TSTAT_ZIGBEE_RSSI_COUNT,5);	
	//if(nRetsingle<0)
	//{
	//	AfxMessageBox(_T("Read error!Please try again!"));
	//	PostMessage(WM_CLOSE,NULL,NULL);
	//}
	memset(zigbee_register_value,0,sizeof(zigbee_register_value));
	if(nRetsingle>0)
	{
		nRetMulty = Read_Multi(nId,zigbee_register_value,TSTAT_ZIGBEE_FIRST_ID,nRetsingle*2,3);
		if(nRetMulty > 0)
			AddTSTAT_ID(zigbee_register_value,nRetsingle,nId);
	}

	for (int i=loop_count;i<(int)Zigbee_Tstat_Id.size();i++)
	{
		//if((nId == Zigbee_Tstat_Id.at(i)) && ((i+1) < (int)Zigbee_Tstat_Id.size()))
		if((i+1) < (int)Zigbee_Tstat_Id.size())
		{
			unsigned char ntemp_id = Zigbee_Tstat_Id.at(i + 1);
			loop_count ++;
			if((ntemp_id == NC_ID) || (nId == ntemp_id))
			{
				//i++;
				continue;
			}
			
			HandleOneTSTAT(ntemp_id);
			return;
		}
	}
}

void CTstatZigbeeLogic::AddTSTAT_ID(unsigned short * npoint , int nlength,unsigned char nId)
{
	unsigned char ntemp_id = 0;
	unsigned short temp_rssi = 0;
	for (int i=0;i<nlength;i++)
	{
		ntemp_id = npoint[i];
		temp_rssi = npoint[i+nlength];
		RSSI[nId][ntemp_id] = (signed char)temp_rssi;
		bool is_exist = false;
		for (int j=0;j<(int)Zigbee_Tstat_Id.size();j++)
		{
			if(Zigbee_Tstat_Id.at(j) == ntemp_id)
			{
				is_exist = true;
				break;
			}
		}
		if(!is_exist)
		{
			Zigbee_Tstat_Id.push_back(ntemp_id);
			Zigbee_Show_Tstat_Id.push_back(ntemp_id);
			PostMessage(WM_SHOW_ID_LIST,FRESH_ID_LIST,NULL);
		}
	}
}

BOOL CTstatZigbeeLogic::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTstatZigbeeLogic::MainListRefresh()
{
	m_zigbee_logic_list.DeleteAllItems();
	while ( m_zigbee_logic_list.DeleteColumn (0)) ;


	unsigned char temp_char = 0;
	for (int x=0;x<(int)Zigbee_Show_Tstat_Id.size();x++)
	{
		for (int y=0;y<(Zigbee_Show_Tstat_Id.size() -x -1);y++)
		{
			if(Zigbee_Show_Tstat_Id.at(y) > Zigbee_Show_Tstat_Id.at(y + 1))
			{
				temp_char = Zigbee_Show_Tstat_Id.at(y);
				Zigbee_Show_Tstat_Id.at(y) = Zigbee_Show_Tstat_Id.at(y + 1);
				Zigbee_Show_Tstat_Id.at(y + 1) = temp_char;
			}
		}
	}
	m_zigbee_logic_list.InsertColumn(0, _T("") , TSTAT_LIST_COLUM_WIDTH, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	CString colum_string;
	CString cs_value;
	for (int i=0;i<(int)Zigbee_Show_Tstat_Id.size();i++)
	{
		colum_string.Format(_T("%d"),Zigbee_Show_Tstat_Id.at(i));
		m_zigbee_logic_list.InsertColumn(i + 1, colum_string , TSTAT_LIST_COLUM_WIDTH, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	}

	for (int i=0;i<(int)Zigbee_Show_Tstat_Id.size();i++)
	{
		colum_string.Format(_T("%d"),Zigbee_Show_Tstat_Id.at(i));
		m_zigbee_logic_list.InsertItem(i,colum_string);
		for (int j=0;j<(int)Zigbee_Show_Tstat_Id.size();j++)
		{
			cs_value.Format(_T("%d"),RSSI[Zigbee_Show_Tstat_Id.at(i)][Zigbee_Show_Tstat_Id.at(j)]);
			if(cs_value.CompareNoCase(_T("0")) == 0)
			{
				cs_value.Format(_T("X"));
				m_zigbee_logic_list.SetItemBkColor(i,j+1,RGB(255,0,0));
			}
			else
			{
				m_zigbee_logic_list.SetItemBkColor(i,j+1,RGB(0,255,0));
			}
			m_zigbee_logic_list.SetItemText(i,j+1,cs_value);
		}
		

	}


}

LRESULT  CTstatZigbeeLogic::HandleMessage(WPARAM wParam, LPARAM lParam)
{
	int temp_command = (int) wParam;
	switch(temp_command)
	{
	case FRESH_ID_LIST:
		{
			m_tstat_id_list.DeleteAllItems();
			for (int i=0;i<Zigbee_Show_Tstat_Id.size();i++)
			{
				CString cstemp_id;
				cstemp_id.Format(_T("%d"),Zigbee_Show_Tstat_Id.at(i));
				m_tstat_id_list.InsertItem(0,cstemp_id);
			}
			return 0;
		}
		break;
	case FRESH_MAIN_LIST:
		{
			MainListRefresh();
		}
		break;
	default:
		break;
	}

}


void CTstatZigbeeLogic::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(hGetZigbeeInfo == NULL)
		hGetZigbeeInfo =CreateThread(NULL,NULL,GetZigbeeInfo,this,NULL, &nZigbeeInfoThreadID);
	CDialogEx::OnTimer(nIDEvent);
}
