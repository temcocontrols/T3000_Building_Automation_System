// TstatZigbeeLogic.cpp : implementation file
// This file is code by Fance 2014/03
// Use this file to get the RSSI of all the tstat under the NC , and shows dynamic rssi of the tstat.

#include "stdafx.h"
#include "T3000.h"
#include "TstatZigbeeLogic.h"
#include "afxdialogex.h"
#include "global_function.h"


#define WM_SHOW_ID_LIST WM_USER + 277

const int FRESH_ID_LIST = 1;
const int FRESH_MAIN_LIST = 2;


DWORD  nZigbeeInfoThreadID;
unsigned short zigbee_register_value[1024]={-1};
int MODBUS_ID;
static int loop_count = 0;
signed char RSSI[256][256];
// CTstatZigbeeLogic dialog
CWinThread* hGetZigbeeInfo=NULL;
IMPLEMENT_DYNAMIC(CTstatZigbeeLogic, CDialogEx)

CTstatZigbeeLogic::CTstatZigbeeLogic(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTstatZigbeeLogic::IDD, pParent)
{
m_abort=FALSE;
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
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTstatZigbeeLogic message handlers


BOOL CTstatZigbeeLogic::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetTimer(1,10000,NULL);
	loop_count = 0;
	Initial_ID_List();
	

	m_zigbee_logic_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	//m_monitor_list.SetExtendedStyle(m_monitor_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_zigbee_logic_list.SetExtendedStyle(m_tstat_id_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_zigbee_logic_list.Support_Keyboard(false);
	m_zigbee_logic_list.SetWhetherShowBkCol(false);

	Zigbee_Tstat_Id.push_back(g_tstat_id);	//第一个点击的 tstat 的ID先加到表里面去,因为它的寄存器里面没有它自己的;
	Zigbee_Show_Tstat_Id.push_back(g_tstat_id);

	
	//if(hGetZigbeeInfo == NULL)
	//{
	//	hGetZigbeeInfo =CreateThread(NULL,NULL,GetZigbeeInfo,this,NULL, &nZigbeeInfoThreadID);
	//}
	if (hGetZigbeeInfo==NULL)
	{
	hGetZigbeeInfo = AfxBeginThread(GetZigbeeInfo, this, THREAD_PRIORITY_NORMAL, CREATE_SUSPENDED);
	}
	
	
// 	if (hGetZigbeeInfo == NULL)
// 	{
// 		TRACE(_T("Failed to create download thread, dialog is aborting\n"));
// 
// 	}
// 	hGetZigbeeInfo->m_bAutoDelete = FALSE;
// 	hGetZigbeeInfo->ResumeThread(	

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

UINT   CTstatZigbeeLogic::GetZigbeeInfo(LPVOID lpVoid)
{
	//Read_Multi(g_tstat_id,&multi_register_value[i*64],i*64,64);

	CTstatZigbeeLogic *mparent = (CTstatZigbeeLogic *)lpVoid;
	//mparent->Zigbee_Tstat_Id.clear();
	//mparent->Zigbee_Show_Tstat_Id.clear();
	memset(RSSI,0,sizeof(RSSI));
	loop_count = 0;
	
	if (mparent->m_abort)
	{
		return 0;
	}
	if (!is_connect())
	{
	return 0;
	}
	MODBUS_ID = read_one(255,6,2);	//读到NC 的ID， NC 的表里面也没有NC和其他的 RSSI 的值;
	if(MODBUS_ID < 0)
	{
		return 0;
	}
	int product_id = read_one(255,7,2);
	if(product_id < 0)
	{
		return 0;
	}
	if(product_id == PM_MINIPANEL|| product_id == PM_MINIPANEL_ARM)
	{
		mparent->HandleOneMinipanel(g_tstat_id);
	}
	else
		mparent->HandleOneTSTAT(g_tstat_id);
	mparent->SendMessage(WM_SHOW_ID_LIST,FRESH_MAIN_LIST,NULL);	//处理完成后更新 List ;
	if (mparent->m_abort)
	{
	return 0;
	}
	
	if (hGetZigbeeInfo) 
	{
// 		if (WaitForSingleObject(hGetZigbeeInfo->m_hThread, 3000) == WAIT_OBJECT_0)
// 		{
// 		}
// 		else
// 		{		
			//BOOL bRet = TerminateThread(hGetZigbeeInfo->m_hThread,0);
			
			hGetZigbeeInfo=NULL;
/*		}*/

	}
	//hGetZigbeeInfo = NULL;
	return 0;
}


//两个线程同时操作一个vector可能会出问题，稍后修改;
void CTstatZigbeeLogic::HandleOneMinipanel(unsigned char nId)
{
	int nRetsingle = 0;
	int nRetMulty = 0;
	if (m_abort)
	{
		return;
	}

	nRetsingle = read_one(nId,MINIPANEL_ZIGBEE_RSSI_COUNT,5);	
	//if(nRetsingle<0)
	//{
	//	AfxMessageBox(_T("Read error!Please try again!"));
	//	PostMessage(WM_CLOSE,NULL,NULL);
	//}
	memset(zigbee_register_value,0,sizeof(zigbee_register_value));
	if(nRetsingle>0)
	{
		nRetMulty = Read_Multi(nId,zigbee_register_value,MINIPANEL_ZIGBEE_FIRST_ID,nRetsingle,3);
		if(nRetMulty > 0)
			AddMinipanel_ID(zigbee_register_value,nRetsingle,nId);
	}

	for (int i=loop_count;i<(int)Zigbee_Tstat_Id.size();i++)
	{
		if (m_abort)
		{
			break;
		}
		//if((nId == Zigbee_Tstat_Id.at(i)) && ((i+1) < (int)Zigbee_Tstat_Id.size()))
		if((i+1) < (int)Zigbee_Tstat_Id.size())
		{
			unsigned char ntemp_id = Zigbee_Tstat_Id.at(i + 1);
			loop_count ++;
			if((ntemp_id == MODBUS_ID) || (nId == ntemp_id))
			{
				//i++;
				continue;
			}
			if (m_abort)
			{
				break;
			}
			HandleOneTSTAT(ntemp_id);
			//SendMessage(WM_SHOW_ID_LIST,FRESH_MAIN_LIST,NULL);
			return;
		}
	}
}


//两个线程同时操作一个vector可能会出问题，稍后修改;
void CTstatZigbeeLogic::HandleOneTSTAT(unsigned char nId)
{
	int nRetsingle = 0;
	int nRetMulty = 0;
	if (m_abort)
	{
	return;
	}
	
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
		if (m_abort)
		{
			break;
		}
		//if((nId == Zigbee_Tstat_Id.at(i)) && ((i+1) < (int)Zigbee_Tstat_Id.size()))
		if((i+1) < (int)Zigbee_Tstat_Id.size())
		{
			unsigned char ntemp_id = Zigbee_Tstat_Id.at(i + 1);
			loop_count ++;
			if((ntemp_id == MODBUS_ID) || (nId == ntemp_id))
			{
				//i++;
				continue;
			}
			if (m_abort)
			{
				break;
			}
			HandleOneTSTAT(ntemp_id);
			//SendMessage(WM_SHOW_ID_LIST,FRESH_MAIN_LIST,NULL);
			return;
		}
	}
}

void CTstatZigbeeLogic::AddMinipanel_ID(unsigned short * npoint , int nlength,unsigned char nId)
{
	unsigned char ntemp_id = 0;
	unsigned short temp_rssi = 0;
	if (m_abort)
	{
		return;
	}
	for (int i=0;i<nlength;i++)
	{
		if (m_abort)
		{
			break;
		}
		ntemp_id = npoint[i] >> 8;
		if(ntemp_id == 0)
			continue;
		temp_rssi = npoint[i]& 0x00ff;

		//temp_rssi = npoint[i+nlength];
		RSSI[nId][ntemp_id] = (signed char)temp_rssi;
		bool is_exist = false;
		for (int j=0;j<(int)Zigbee_Tstat_Id.size();j++)
		{
			if (m_abort)
			{
				break;
			}
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
			if (m_abort)
			{
				break;
			}
			SendMessage(WM_SHOW_ID_LIST,FRESH_ID_LIST,NULL);
			SendMessage(WM_SHOW_ID_LIST,FRESH_MAIN_LIST,NULL);
		}
	}

	if (m_abort)
	{
		return;
	}

}


void CTstatZigbeeLogic::AddTSTAT_ID(unsigned short * npoint , int nlength,unsigned char nId)
{
	unsigned char ntemp_id = 0;
	unsigned short temp_rssi = 0;
	if (m_abort)
	{
		return;
	}
	for (int i=0;i<nlength;i++)
	{
		if (m_abort)
		{
			break;
		}
		ntemp_id = npoint[i];
		temp_rssi = npoint[i+nlength];
		RSSI[nId][ntemp_id] = (signed char)temp_rssi;
		bool is_exist = false;
		for (int j=0;j<(int)Zigbee_Tstat_Id.size();j++)
		{
			if (m_abort)
			{
				break;
			}
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
			if (m_abort)
			{
				break;
			}
			SendMessage(WM_SHOW_ID_LIST,FRESH_ID_LIST,NULL);
			SendMessage(WM_SHOW_ID_LIST,FRESH_MAIN_LIST,NULL);
		}
	}

	if (m_abort)
	{
		return;
	}

}

BOOL CTstatZigbeeLogic::PreTranslateMessage(MSG* pMsg)
{
	

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTstatZigbeeLogic::MainListRefresh()
{

	if (m_abort)
	{
		return ;
	}
	m_zigbee_logic_list.DeleteAllItems();
	while ( m_zigbee_logic_list.DeleteColumn (0)) ;


	unsigned char temp_char = 0;
	for (int x=0;x<(int)Zigbee_Show_Tstat_Id.size();x++)
	{
		for (int y=0;y<(Zigbee_Show_Tstat_Id.size() -x -1);y++)
		{
			if (m_abort)
			{
				break;
			}
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
		if (m_abort)
		{
			break;
		}
		colum_string.Format(_T("%d"),Zigbee_Show_Tstat_Id.at(i));
		m_zigbee_logic_list.InsertColumn(i + 1, colum_string , TSTAT_LIST_COLUM_WIDTH, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	}

	for (int i=0;i<(int)Zigbee_Show_Tstat_Id.size();i++)
	{
		if (m_abort)
		{
			break;
		}
		colum_string.Format(_T("%d"),Zigbee_Show_Tstat_Id.at(i));
		m_zigbee_logic_list.InsertItem(i,colum_string);
		for (int j=0;j<(int)Zigbee_Show_Tstat_Id.size();j++)
		{
			if (m_abort)
			{
				break;
			}
			int ColorValue=RSSI[Zigbee_Show_Tstat_Id.at(i)][Zigbee_Show_Tstat_Id.at(j)];
			cs_value.Format(_T("%d"),ColorValue);
			if(cs_value.CompareNoCase(_T("0")) == 0)
			{
				cs_value.Format(_T("X"));
				m_zigbee_logic_list.SetItemBkColor(i,j+1,RGB(255,0,0));
			}
			else
			{
			   
				m_zigbee_logic_list.SetItemBkColor(i,j+1,GetCOLORREF(ColorValue));
			}
			if (m_abort)
			{
				break;
			}
			m_zigbee_logic_list.SetItemText(i,j+1,cs_value);
		}
		

	}


}



//void color_step(COLORREF startColor,COLORREF endColor) 
//{ 
//  
//  int r = (GetRValue(endColor) - GetRValue(startColor));
//  int g = (GetGValue(endColor) - GetGValue(startColor)); 
//  int b = (GetBValue(endColor) - GetBValue(startColor));
//  int nSteps = max(abs(r), max(abs(g), abs(b))); 
//  if (nSteps < 1) nSteps = 1; 
//  // Calculate the step size for each color 
//  float rStep = r/(float)nSteps; 
//  float gStep = g/(float)nSteps; 
//  float bStep = b/(float)nSteps; 
//  // Reset the colors to the starting position
//   float fr = GetRValue(startColor); 
//   float fg = GetGValue(startColor); 
//   float fb = GetBValue(startColor);
//   for (int i = 0; i < nSteps; i++) { 
//   fr += rStep;
//   fg += gStep;
//   fb += bStep;
//   COLORREF color = RGB((int)(fr + 0.5),(int)(fg + 0.5),(int)(fb + 0.5));
//   //color 即为重建颜色
//   }
//  }
//


COLORREF  CTstatZigbeeLogic::GetCOLORREF(int Value){
	if (Value==0)
	{
		Value=-120;
	}

	if (Value>=20)
	{
		return RGB(0,255,0);
	}
	if (Value<=-120)
	{
		return RGB(255,0,0);
	}
	DWORD Red=RGB(255,0,0);
	DWORD Green=RGB(0,255,0);
	DWORD StepColor=(Green-Red)/140;
	DWORD ValueColor=Red+StepColor*(Value+120);
	int fr=GetRValue(ValueColor);
	int fg=GetGValue(ValueColor);
	int fb=GetBValue(ValueColor);
	COLORREF RetColor=RGB(fr,fg,fb);
	return RetColor;

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

				if (m_abort)
				{
				break;
				}
				
				m_tstat_id_list.InsertItem(0,cstemp_id);
			}
			return 0;
		}
		break;
	case FRESH_MAIN_LIST:
		{
			if (m_abort)
			{
				return 0;
			}

			MainListRefresh();
		}
		break;
	default:
		break;
	}
	return 1;
}


void CTstatZigbeeLogic::OnTimer(UINT_PTR nIDEvent)
{
	 
// 	if(hGetZigbeeInfo == NULL)
// 		hGetZigbeeInfo =CreateThread(NULL,NULL,GetZigbeeInfo,this,NULL, &nZigbeeInfoThreadID);

 	if (hGetZigbeeInfo==NULL)
 	{
 		hGetZigbeeInfo = AfxBeginThread(GetZigbeeInfo, this, THREAD_PRIORITY_NORMAL, CREATE_SUSPENDED);
 	}
	CDialogEx::OnTimer(nIDEvent);
}


void CTstatZigbeeLogic::OnDestroy()
{



	//if (hGetZigbeeInfo!=NULL)
	//{
	//	WaitForSingleObject(hGetZigbeeInfo->m_hThread, 2000);
	//	
	//	delete hGetZigbeeInfo;
	//	m_pThread = NULL;
	//}
	m_abort=TRUE;
	Sleep(2000);
	KillTimer(1);
	//if (hGetZigbeeInfo) 
	//{
	//	if (WaitForSingleObject(hGetZigbeeInfo->m_hThread, INFINITE) == WAIT_OBJECT_0)
	//	{

	//	}
	//	else
	//	{		
	//		BOOL bRet = TerminateThread(hGetZigbeeInfo->m_hThread,0);
	//		//delete m_pFreshMultiRegisters;
	//		hGetZigbeeInfo=NULL;
	//	}

	//}
	if (hGetZigbeeInfo) 
	{
		BOOL bRet = TerminateThread(hGetZigbeeInfo->m_hThread,0);
		hGetZigbeeInfo=NULL;
	}
	SetCommunicationType(1);
	close_com();
	CDialogEx::OnDestroy();



	
}
