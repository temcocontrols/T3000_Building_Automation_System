// DetectOnlineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "DetectOnlineDlg.h"
#include "afxdialogex.h"
#include "global_function.h"
#include "MainFrm.h"
#include "crc.h"
#include <algorithm>
#include "../SQLiteDriver/CppSQLite3.h"
// CDetectOnlineDlg dialog
const UINT WM_HTTPDOWNLOAD_THREAD_FINISHED = WM_APP + 1;

const UINT WM_TIPS = WM_APP + 2;
IMPLEMENT_DYNAMIC(CDetectOnlineDlg, CDialogEx)



#define FLEXGRID_CELL_COLOR RGB(128,128,128) 
#define GREEN_COLOR RGB(0,255,0)
#define YELLOW_COLOR RGB(255,255,0)
#define ORANGE_COLOR RGB(255,165,0)
#define RED_COLOR RGB(255,0,0)


CDetectOnlineDlg::CDetectOnlineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDetectOnlineDlg::IDD, pParent)
	, m_minID(1)
	, m_maxID(55)
	, m_tips(_T(""))
	, m_scan_times(1)
 
{
m_pThread = NULL;
m_abort=FALSE;
m_scan_type=FALSE;//TRUR:One by One ,FALSE:Binary Scan
}

CDetectOnlineDlg::~CDetectOnlineDlg()
{
}

void CDetectOnlineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MINEDIT, m_minID);
	DDX_Text(pDX, IDC_MAXEDIT, m_maxID);
	DDX_Text(pDX, IDC_STATIC_TIPS, m_tips);
	DDX_Control(pDX, IDC_MSFLEXGRID_RESULT, m_detect_grid);
	DDX_Control(pDX, IDC_EDIT1, m_editor);
	DDX_Control(pDX, IDC_COMBO_TF, m_combox_tf);
	DDX_Control(pDX, IDC_EDIT2, m_edit_name);
	DDX_Control(pDX, IDC_MSFLEXGRID_RESULT2, m_allcomportTable);
	DDX_Control(pDX, IDC_EDIT_START_SERIALNO, m_edit_serialNo);
	DDX_Control(pDX, IDC_BUTTON_WRITE_SERIAL_NO, m_writesnBtn);
	DDX_Text(pDX, IDC_EDIT_TRY_TIMES, m_scan_times);
	DDV_MinMaxUInt(pDX, m_scan_times, 1, 1000);
	// 	DDX_Check(pDX, IDC_CHECK_ONE_BY_ONE, m_scan_type);
	// 	DDX_Check(pDX, IDC_CHECK_BINARY_SCAN, m_scan_type1);
	DDX_Control(pDX, IDC_CHECK_ONE_BY_ONE, m_check_onebyone);
	DDX_Control(pDX, IDC_CHECK_BINARY_SCAN, m_check_binaryScan);
}


BEGIN_MESSAGE_MAP(CDetectOnlineDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DETECT, &CDetectOnlineDlg::OnBnClickedButtonDetect)

	ON_MESSAGE(WM_HTTPDOWNLOAD_THREAD_FINISHED, OnThreadFinished)
	 
	//ON_EN_KILLFOCUS(IDC_EDIT1, &CDetectOnlineDlg::OnEnKillfocusEdit1)
	ON_BN_CLICKED(IDOK, &CDetectOnlineDlg::OnBnClickedOk)
	ON_WM_DESTROY()
	//ON_BN_CLICKED(IDC_BUTTON1, &CDetectOnlineDlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO_TF, &CDetectOnlineDlg::OnCbnSelchangeComboTf)
	ON_EN_SETFOCUS(IDC_EDIT2, &CDetectOnlineDlg::OnEnSetfocusEdit2)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CDetectOnlineDlg::OnEnKillfocusEdit2)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, &CDetectOnlineDlg::OnBnClickedButtonScan)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &CDetectOnlineDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_REPAIRCONFLICT, &CDetectOnlineDlg::OnBnClickedButtonRepairconflict)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_SERIAL_NO, &CDetectOnlineDlg::OnBnClickedButtonWriteSerialNo)
//	ON_EN_CHANGE(IDC_EDIT2, &CDetectOnlineDlg::OnEnChangeEdit2)
ON_BN_CLICKED(IDC_CHECK_ONE_BY_ONE, &CDetectOnlineDlg::OnBnClickedCheckOneByOne)
ON_BN_CLICKED(IDC_CHECK_BINARY_SCAN, &CDetectOnlineDlg::OnBnClickedCheckBinaryScan)
ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDetectOnlineDlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()


// CDetectOnlineDlg message handlers
UINT CDetectOnlineDlg::DetectThread(LPVOID pParam)
{
	//Convert from the SDK world to the C++ world
	CDetectOnlineDlg  * pDlg = (CDetectOnlineDlg *) pParam;
	ASSERT(pDlg);
	ASSERT(pDlg->IsKindOf(RUNTIME_CLASS(CDetectOnlineDlg  )));
	pDlg->GetAllNodes();
	return 0;
}
UINT CDetectOnlineDlg::RepairIDThread(LPVOID pParam){
	CDetectOnlineDlg  * pDlg = (CDetectOnlineDlg *) pParam;
	ASSERT(pDlg);
	ASSERT(pDlg->IsKindOf(RUNTIME_CLASS(CDetectOnlineDlg  )));
	pDlg->RepairID();
return 0;
}
void CDetectOnlineDlg::OnBnClickedButtonDetect()
{
	//m_detect_grid.put_TextMatrix(0,0,_T("COM Port"));
	//m_detect_grid.put_TextMatrix(0,1,_T("Modbus ID"));
	//m_detect_grid.put_TextMatrix(0,2,_T("Product ID"));
	//m_detect_grid.put_TextMatrix(0,3,_T("Room No"));
	//m_detect_grid.put_TextMatrix(0,4,_T("Serial No"));
	//m_detect_grid.put_TextMatrix(0,5,_T("Status"));
	CStdioFile default_file;
	CString saved_path=g_strExePth+_T("ScanData.txt");
	CString a_line;
	if(default_file.Open(saved_path.GetString(),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
	{
		default_file.SeekToEnd();
		a_line.Format(_T("ComPort		Modbus ID		Product ID		Node Name		Serial No		Status\n"));
		default_file.WriteString(a_line);
		for (int row=1;row<m_detect_grid.get_Rows();row++){
		a_line.Format(_T("%s		%s		%s		%s		%s		%s		\n"),m_detect_grid.get_TextMatrix(row,0),
																				 m_detect_grid.get_TextMatrix(row,1),
																				 m_detect_grid.get_TextMatrix(row,2),
																				 m_detect_grid.get_TextMatrix(row,3),
																				 m_detect_grid.get_TextMatrix(row,4),
																				 m_detect_grid.get_TextMatrix(row,5));
		default_file.WriteString(a_line);
		}
		

		default_file.Flush();
		default_file.Close();
	}

	AfxMessageBox(_T("Export Successful"));

	::ShellExecute(NULL, _T("open"), saved_path.GetString(), _T(""), _T(""), SW_SHOW);
	 
}

BOOL CDetectOnlineDlg::binary_search_crc(int a)
{//use this for binary search ,inspect
	static int for_binary_search_crc=0;//retry times 10;when return value is -2
	if(a==-2)
	{
		if(for_binary_search_crc<10)
			for_binary_search_crc++;
		else
		{
			for_binary_search_crc=0;
			return true;/////////////////more ten time
		}
	}
	else
		for_binary_search_crc=0;
	return false;
}
void CDetectOnlineDlg::GetAllNodes(){
	NODES_DETECTS nodetemp;
	m_allnodes.clear();
	unsigned short TempData[10];
	int a,b;
	int devLow,devHigh;
	for (int scantime=1;scantime<=m_scan_times;scantime++)
	{
  	for (int i=0;i<m_scanport.size();i++)
 	{
 		CString strComPort=m_scanport[i].COMPORT;//m_comports[i];
 		CString tc = strComPort.Mid(3);
 		int n = _wtoi(tc);
 		if (open_com(n))
 		{    
 			 devLow=_wtoi(m_scanport[i].LowID);
			 devHigh=_wtoi(m_scanport[i].HighID);
 			 if (m_abort)
 			 {
 			 break;
 			 }
 			 Change_BaudRate(19200);
		
			binarySearchforComDevice(n, 0, devLow, devHigh,i);
 			close_com();
 		}
        
 	}
	}
/*	 PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);*/
	PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,3);
	PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,4);
}

void CDetectOnlineDlg::binarySearchforComDevice(int nComPort, bool bForTStat, BYTE devLo, BYTE devHi,int index)
{
CString StrLog;
NODES_DETECTS nodetemp;
if (m_abort)
{
	return;
}
#if 0


Sleep(50);	
int nCount=0;

int a=CheckTstatOnline_a(devLo,devHi, bForTStat);

if (a == -3 || a > 0)
{

	a=CheckTstatOnline_a(devLo,devHi, bForTStat);
}
//TRACE("L:%d   H:%d  a:%d\n",devLo,devHi,a);
if(binary_search_crc(a))
	return ;
char c_array_temp[5]={'0'};
CString temp=_T("");
if(a>0)
{
	nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
	nodetemp.ModbusID.Format(_T("%d"),a);
	//nodetemp.ProductID.Format(_T("%d"),SerialNum[7]);
	//nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,SerialNum[6]);
	//nodetemp.SerialNO.Format(_T("%d"),SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256,SerialNum[3]*256*256*256);
	//nodetemp.TstatID.Format(_T("%d"),SerialNum[6]);
	nodetemp.Brandrate=_T("19200");
	nodetemp.Status=_T("Online");

	m_allnodes.push_back(nodetemp);

	PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);

#if 0
	int ntempID=0;
	BOOL bFindSameID=false;
	int nPos=-1;
	unsigned short SerialNum[9];
	memset(SerialNum,0,sizeof(SerialNum));
	int nRet=0;
	nRet=Read_Multi(a,&SerialNum[0],0,9,5);
	if(nRet>0)
	{
		nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
		nodetemp.ModbusID.Format(_T("%d"),a);
		nodetemp.ProductID.Format(_T("%d"),SerialNum[7]);
		nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,SerialNum[6]);
		nodetemp.SerialNO.Format(_T("%d"),SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256,SerialNum[3]*256*256*256);
		nodetemp.TstatID.Format(_T("%d"),SerialNum[6]);
		nodetemp.Brandrate=_T("19200");
		nodetemp.Status=_T("Online");
		BOOL Repeated=FALSE;
		for (int i=0;i<m_allnodes.size();i++)
		{
			if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
			{
				Repeated=TRUE;
				break;
			}
		}
		if (!Repeated)
		{
			m_allnodes.push_back(nodetemp);

			PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
		}

	}
	else
	{
		Sleep(1000);
		nRet=Read_Multi(a,&SerialNum[0],0,9,5);
		if(nRet>0)
		{
			nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
			nodetemp.ModbusID.Format(_T("%d"),a);
			nodetemp.ProductID.Format(_T("%d"),SerialNum[7]);
			nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,SerialNum[6]);
			nodetemp.SerialNO.Format(_T("%d"),SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256,SerialNum[3]*256*256*256);
			nodetemp.TstatID.Format(_T("%d"),SerialNum[6]);
			nodetemp.Brandrate=_T("19200");
			nodetemp.Status=_T("Online");
			BOOL Repeated=FALSE;
			for (int i=0;i<m_allnodes.size();i++)
			{
				if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
				{
					Repeated=TRUE;
					break;
				}
			}
			if (!Repeated)
			{
				m_allnodes.push_back(nodetemp);

				PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
			}

		}
		else
		{
			nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
			nodetemp.ModbusID.Format(_T("%d"),a);
			nodetemp.ProductID.Format(_T("Unknown"));
			nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,a);
			nodetemp.SerialNO.Format(_T("Unknown"));
			nodetemp.TstatID.Format(_T("Unknown"));
			nodetemp.Status=_T("No Read");
			nodetemp.Brandrate=_T("19200");
			BOOL Repeated=FALSE;
			for (int i=0;i<m_allnodes.size();i++)
			{
				if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
				{
					Repeated=TRUE;
					break;
				}
			}
			if (!Repeated)
			{
				m_allnodes.push_back(nodetemp);

				PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
			}
		}
	}
#endif


}
m_tips.Format(_T("COM%d  Between ID:%d and ID:%d"),nComPort,devLo,devHi);
SendMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,1);
switch(a)
{
case -2:
	//crc error
	if(devLo!=devHi)
	{
		binarySearchforComDevice(nComPort, bForTStat, devLo,(devLo+devHi)/2,index);
		binarySearchforComDevice(nComPort, bForTStat, (devLo+devHi)/2+1,devHi,index);
	}
	else
	{
		//binarySearchforComDevice(nComPort, bForTStat, devLo,devHi,index);
		int ntempID=0;
		BOOL bFindSameID=false;
		int nPos=-1;
		//		temp.baudrate=m_baudrate2;
		unsigned short SerialNum[9];
		memset(SerialNum,0,sizeof(SerialNum));
		int nRet=0;
		nRet=Read_Multi(devLo,&SerialNum[0],0,9,5);

		if(nRet>0)
		{
			nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
			nodetemp.ModbusID.Format(_T("%d"),devLo);
			nodetemp.ProductID.Format(_T("%d"),SerialNum[7]);
			nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,SerialNum[6]);
			nodetemp.SerialNO.Format(_T("%d"),SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256,SerialNum[3]*256*256*256);
			nodetemp.TstatID.Format(_T("%d"),SerialNum[6]);
			nodetemp.Brandrate=_T("19200");
			nodetemp.Status=_T("Online");
			BOOL Repeated=FALSE;
			for (int i=0;i<m_allnodes.size();i++)
			{
				if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
				{
					Repeated=TRUE;
					break;
				}
			}
			if (!Repeated)
			{
				m_allnodes.push_back(nodetemp);

				PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
			}

		}
		else
		{
			nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
			nodetemp.ModbusID.Format(_T("%d"),devLo);
			nodetemp.ProductID.Format(_T("Unknown"));
			nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,devLo);
			nodetemp.SerialNO.Format(_T("Unknown"));
			nodetemp.TstatID.Format(_T("Unknown"));
			nodetemp.Status=_T("Conflict");
			nodetemp.Brandrate=_T("19200");
			BOOL Repeated=FALSE;
			for (int i=0;i<m_allnodes.size();i++)
			{
				if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
				{
					Repeated=TRUE;
					break;
				}
			}
			if (!Repeated)
			{
				m_allnodes.push_back(nodetemp);
				PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
			}
		}



		// 			m_allnodes.push_back(nodetemp);
		// 			PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
	}
	break;
case -3:
	//more than 2 Tstat is connect
	if(devLo!=devHi)
	{
		binarySearchforComDevice(nComPort, bForTStat, devLo,(devLo+devHi)/2,index);
		binarySearchforComDevice(nComPort, bForTStat, (devLo+devHi)/2+1,devHi,index);
	}
	else
	{	


		int ntempID=0;
		BOOL bFindSameID=false;
		int nPos=-1;
		//		temp.baudrate=m_baudrate2;
		unsigned short SerialNum[9];
		memset(SerialNum,0,sizeof(SerialNum));
		int nRet=0;
		nRet=Read_Multi(devLo,&SerialNum[0],0,9,5);

		if(nRet>0)
		{
			nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
			nodetemp.ModbusID.Format(_T("%d"),devLo);
			nodetemp.ProductID.Format(_T("%d"),SerialNum[7]);
			nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,SerialNum[6]);
			nodetemp.SerialNO.Format(_T("%d"),SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256,SerialNum[3]*256*256*256);
			nodetemp.TstatID.Format(_T("%d"),SerialNum[6]);
			nodetemp.Brandrate=_T("19200");
			nodetemp.Status=_T("Online");
			BOOL Repeated=FALSE;
			for (int i=0;i<m_allnodes.size();i++)
			{
				if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
				{
					Repeated=TRUE;
					break;
				}
			}
			if (!Repeated)
			{
				m_allnodes.push_back(nodetemp);

				PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
			}

		}
		else
		{
			nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
			nodetemp.ModbusID.Format(_T("%d"),devLo);
			nodetemp.ProductID.Format(_T("Unknown"));
			nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,devLo);
			nodetemp.SerialNO.Format(_T("Unknown"));
			nodetemp.Status=_T("Conflict");
			nodetemp.TstatID.Format(_T("Unknown"));
			nodetemp.Brandrate=_T("19200");
			BOOL Repeated=FALSE;
			for (int i=0;i<m_allnodes.size();i++)
			{
				if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
				{
					Repeated=TRUE;
					break;
				}
			}
			if (!Repeated)
			{
				m_allnodes.push_back(nodetemp);

				PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
			}
		}

	}
	break;
case -4:break;
	//no connection 

case -5:break;
	//the input error
}
#endif
if (!m_scan_type)
{	
	//for (int scantime=1;scantime<=m_scan_times;scantime++)
	//{
		Sleep(50);	
		int nCount=0;

		int a=CheckTstatOnline_a(devLo,devHi, bForTStat);

		if (a == -3 || a > 0)
		{

			a=CheckTstatOnline_a(devLo,devHi, bForTStat);
		}
		//TRACE("L:%d   H:%d  a:%d\n",devLo,devHi,a);
		if(binary_search_crc(a))
			return ;
		char c_array_temp[5]={'0'};
		CString temp=_T("");

		if(a>0)
		{
		StrLog.Format(_T("Find One ID=%d (Between %d and %d)"),a,devLo,devHi);
		write_T3000_log_file(StrLog);
			//nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
			//nodetemp.ModbusID.Format(_T("%d"),a);
			////nodetemp.ProductID.Format(_T("%d"),SerialNum[7]);
			////nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,SerialNum[6]);
			////nodetemp.SerialNO.Format(_T("%d"),SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256,SerialNum[3]*256*256*256);
			////nodetemp.TstatID.Format(_T("%d"),SerialNum[6]);
			//nodetemp.Brandrate=_T("19200");
			//nodetemp.Status=_T("Online");

			//m_allnodes.push_back(nodetemp);

			//PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);

#if 1
			int ntempID=0;
			BOOL bFindSameID=false;
			int nPos=-1;
			unsigned short SerialNum[9];
			memset(SerialNum,0,sizeof(SerialNum));
			int nRet=0;
			nRet=Read_Multi(a,&SerialNum[0],0,9,5);
			if(nRet>0)
			{
				nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
				nodetemp.ModbusID.Format(_T("%d"),a);
				nodetemp.ProductID.Format(_T("%d"),SerialNum[7]);
				nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,SerialNum[6]);
				nodetemp.SerialNO.Format(_T("%d"),SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256,SerialNum[3]*256*256*256);
				nodetemp.TstatID.Format(_T("%d"),SerialNum[6]);
				nodetemp.Brandrate=_T("19200");
				nodetemp.Status=_T("Online");
				float firmwareversion=((float)(SerialNum[MODBUS_VERSION_NUMBER_HI]*256+SerialNum[MODBUS_VERSION_NUMBER_LO]))/10.0;	
				nodetemp.Firmware.Format(_T("%.1f"),firmwareversion);
				BOOL Repeated=FALSE;
				for (int i=0;i<m_allnodes.size();i++)
				{
					if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
					{
						Repeated=TRUE;
						break;
					}
				}
				if (!Repeated)
				{
					m_allnodes.push_back(nodetemp);

					PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
				}

			}
			else
			{
				Sleep(1000);
				nRet=Read_Multi(a,&SerialNum[0],0,9,5);
				if(nRet>0)
				{
					nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
					nodetemp.ModbusID.Format(_T("%d"),a);
					nodetemp.ProductID.Format(_T("%d"),SerialNum[7]);
					nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,SerialNum[6]);
					nodetemp.SerialNO.Format(_T("%d"),SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256,SerialNum[3]*256*256*256);
					nodetemp.TstatID.Format(_T("%d"),SerialNum[6]);
					nodetemp.Brandrate=_T("19200");
					nodetemp.Status=_T("Online");
					float firmwareversion=((float)(SerialNum[MODBUS_VERSION_NUMBER_HI]*256+SerialNum[MODBUS_VERSION_NUMBER_LO]))/10.0;	
					nodetemp.Firmware.Format(_T("%.1f"),firmwareversion);
					BOOL Repeated=FALSE;
					for (int i=0;i<m_allnodes.size();i++)
					{
						if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
						{
							Repeated=TRUE;
							break;
						}
					}
					if (!Repeated)
					{
						m_allnodes.push_back(nodetemp);

						PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
					}

				}
				else
				{
					nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
					nodetemp.ModbusID.Format(_T("%d"),a);
					nodetemp.ProductID.Format(_T("Unknown"));
					nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,a);
					nodetemp.SerialNO.Format(_T("Unknown"));
					nodetemp.TstatID.Format(_T("Unknown"));
					nodetemp.Status=_T("No Read");
					nodetemp.Brandrate=_T("19200");
					BOOL Repeated=FALSE;
					for (int i=0;i<m_allnodes.size();i++)
					{
						if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
						{
							Repeated=TRUE;
							break;
						}
					}
					if (!Repeated)
					{
						m_allnodes.push_back(nodetemp);

						PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
					}
				}
			}
#endif


		}
		m_tips.Format(_T("COM%d  Between ID:%d and ID:%d"),nComPort,devLo,devHi);
		SendMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,1);
		switch(a)
		{
		case -2:
			//crc error
			if(devLo!=devHi)
			{
				binarySearchforComDevice(nComPort, bForTStat, devLo,(devLo+devHi)/2,index);
				binarySearchforComDevice(nComPort, bForTStat, (devLo+devHi)/2+1,devHi,index);
			}
			else
			{
				//binarySearchforComDevice(nComPort, bForTStat, devLo,devHi,index);
				int ntempID=0;
				BOOL bFindSameID=false;
				int nPos=-1;
				//		temp.baudrate=m_baudrate2;
				unsigned short SerialNum[9];
				memset(SerialNum,0,sizeof(SerialNum));
				int nRet=0;
				nRet=Read_Multi(devLo,&SerialNum[0],0,9,5);

				if(nRet>0)
				{
					StrLog.Format(_T("Read good and Scan Back=%d (Between %d and %d)"),a,devLo,devHi);
					write_T3000_log_file(StrLog);

					nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
					nodetemp.ModbusID.Format(_T("%d"),devLo);
					nodetemp.ProductID.Format(_T("%d"),SerialNum[7]);
					nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,SerialNum[6]);
					nodetemp.SerialNO.Format(_T("%d"),SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256,SerialNum[3]*256*256*256);
					nodetemp.TstatID.Format(_T("%d"),SerialNum[6]);
					nodetemp.Brandrate=_T("19200");
					nodetemp.Status=_T("Online");
					float firmwareversion=((float)(SerialNum[MODBUS_VERSION_NUMBER_HI]*256+SerialNum[MODBUS_VERSION_NUMBER_LO]))/10.0;	
					nodetemp.Firmware.Format(_T("%.1f"),firmwareversion);
					BOOL Repeated=FALSE;
					for (int i=0;i<m_allnodes.size();i++)
					{
						if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
						{
							Repeated=TRUE;
							break;
						}
					}
					if (!Repeated)
					{
						m_allnodes.push_back(nodetemp);

						PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
					}

				}
				else
				{
					StrLog.Format(_T("Read Fail and and Scan Back=%d(Between %d and %d)"),a,devLo,devHi);
					write_T3000_log_file(StrLog);

					nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
					nodetemp.ModbusID.Format(_T("%d"),devLo);
					nodetemp.ProductID.Format(_T("Unknown"));
					nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,devLo);
					nodetemp.SerialNO.Format(_T("Unknown"));
					nodetemp.TstatID.Format(_T("Unknown"));
					nodetemp.Status=_T("Conflict");
					nodetemp.Brandrate=_T("19200");
					BOOL Repeated=FALSE;
					for (int i=0;i<m_allnodes.size();i++)
					{
						if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
						{
							Repeated=TRUE;
							break;
						}
					}
					if (!Repeated)
					{
						m_allnodes.push_back(nodetemp);
						PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
					}
				}



				// 			m_allnodes.push_back(nodetemp);
				// 			PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
			}
			break;
		case -3:
			//more than 2 Tstat is connect
			if(devLo!=devHi)
			{
				binarySearchforComDevice(nComPort, bForTStat, devLo,(devLo+devHi)/2,index);
				binarySearchforComDevice(nComPort, bForTStat, (devLo+devHi)/2+1,devHi,index);
			}
			else
			{	


				int ntempID=0;
				BOOL bFindSameID=false;
				int nPos=-1;
				//		temp.baudrate=m_baudrate2;
// 				unsigned short SerialNum[9];
// 				memset(SerialNum,0,sizeof(SerialNum));
// 				int nRet=0;
// 				nRet=Read_Multi(devLo,&SerialNum[0],0,9,5);
// 				if(nRet>0)
// 				{
// 					StrLog.Format(_T("Read and Scan Back=%d (Between %d and %d)"),a,devLo,devHi);
// 					write_T3000_log_file(StrLog);
// 
// 					nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
// 					nodetemp.ModbusID.Format(_T("%d"),devLo);
// 					nodetemp.ProductID.Format(_T("%d"),SerialNum[7]);
// 					nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,SerialNum[6]);
// 					nodetemp.SerialNO.Format(_T("%d"),SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256,SerialNum[3]*256*256*256);
// 					nodetemp.TstatID.Format(_T("%d"),SerialNum[6]);
// 					nodetemp.Brandrate=_T("19200");
// 					nodetemp.Status=_T("Online");
// 					float firmwareversion=((float)(SerialNum[MODBUS_VERSION_NUMBER_HI]*256+SerialNum[MODBUS_VERSION_NUMBER_LO]))/10.0;	
// 					nodetemp.Firmware.Format(_T("%.1f"),firmwareversion);
// 					BOOL Repeated=FALSE;
// 					for (int i=0;i<m_allnodes.size();i++)
// 					{
// 						if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
// 						{
// 							Repeated=TRUE;
// 							break;
// 						}
// 					}
// 					if (!Repeated)
// 					{
// 						m_allnodes.push_back(nodetemp);
// 
// 						PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
// 					}
// 
// 				}
// 				else
// 				{
					StrLog.Format(_T("Read Fail and Scan Back=%d (Between %d and %d)"),a,devLo,devHi);
					write_T3000_log_file(StrLog);
					nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
					nodetemp.ModbusID.Format(_T("%d"),devLo);
					nodetemp.ProductID.Format(_T("Unknown"));
					nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,devLo);
					nodetemp.SerialNO.Format(_T("Unknown"));
					nodetemp.Status=_T("Conflict");
					nodetemp.TstatID.Format(_T("Unknown"));
					nodetemp.Brandrate=_T("19200");
					BOOL Repeated=FALSE;
					for (int i=0;i<m_allnodes.size();i++)
					{
						if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
						{
							Repeated=TRUE;
							break;
						}
					}
					if (!Repeated)
					{
						m_allnodes.push_back(nodetemp);

						PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
					}
				//}

			}
			break;
		case -4://No connection

		    
		/*	StrLog.Format(_T("No Connection and Scan Back=%d (Between %d and %d)"),a,devLo,devHi);
			write_T3000_log_file(StrLog);*/

		      #if 1
			/*	if(devLo!=devHi)
			{
			binarySearchforComDevice(nComPort, bForTStat, devLo,(devLo+devHi)/2,index);
			binarySearchforComDevice(nComPort, bForTStat, (devLo+devHi)/2+1,devHi,index);
			}
			else*/
			if(devLo==devHi)
			{	


				int ntempID=0;
				BOOL bFindSameID=false;
				int nPos=-1;
				//		temp.baudrate=m_baudrate2;
				unsigned short SerialNum[9];
				memset(SerialNum,0,sizeof(SerialNum));
				int nRet=0;
				nRet=Read_Multi(devLo,&SerialNum[0],0,9,5);

				if(nRet>0)
				{
					StrLog.Format(_T("Read good and Scan Back=%d (Between %d and %d)"),a,devLo,devHi);
					write_T3000_log_file(StrLog);
					nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
					nodetemp.ModbusID.Format(_T("%d"),devLo);
					nodetemp.ProductID.Format(_T("%d"),SerialNum[7]);
					nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,SerialNum[6]);
					nodetemp.SerialNO.Format(_T("%d"),SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256,SerialNum[3]*256*256*256);
					nodetemp.TstatID.Format(_T("%d"),SerialNum[6]);
					nodetemp.Brandrate=_T("19200");
					nodetemp.Status=_T("Online");
					float firmwareversion=((float)(SerialNum[MODBUS_VERSION_NUMBER_HI]*256+SerialNum[MODBUS_VERSION_NUMBER_LO]))/10.0;	
					nodetemp.Firmware.Format(_T("%.1f"),firmwareversion);
					BOOL Repeated=FALSE;
					for (int i=0;i<m_allnodes.size();i++)
					{
						if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
						{
							Repeated=TRUE;
							break;
						}
					}
					if (!Repeated)
					{
						m_allnodes.push_back(nodetemp);

						PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
					}

				}
				else
				{
					StrLog.Format(_T("Read fail and Scan Back=%d (Between %d and %d)"),a,devLo,devHi);
					write_T3000_log_file(StrLog);

					nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
					nodetemp.ModbusID.Format(_T("%d"),devLo);
					nodetemp.ProductID.Format(_T("Unknown"));
					nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,devLo);
					nodetemp.SerialNO.Format(_T("Unknown"));
					nodetemp.Status=_T("Offline");
					nodetemp.TstatID.Format(_T("Unknown"));
					nodetemp.Brandrate=_T("19200");
					BOOL Repeated=FALSE;
					for (int i=0;i<m_allnodes.size();i++)
					{
						if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
						{
							Repeated=TRUE;
							break;
						}
					}
					if (!Repeated)
					{
						m_allnodes.push_back(nodetemp);

						PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
					}
				}

			}
              #endif

			break;
		case -5:
			#if 1
		/*	if(devLo!=devHi)
			{
				binarySearchforComDevice(nComPort, bForTStat, devLo,(devLo+devHi)/2,index);
				binarySearchforComDevice(nComPort, bForTStat, (devLo+devHi)/2+1,devHi,index);
			}
			else*/
			if(devLo==devHi)
			{	


				int ntempID=0;
				BOOL bFindSameID=false;
				int nPos=-1;
				//		temp.baudrate=m_baudrate2;
				unsigned short SerialNum[9];
				memset(SerialNum,0,sizeof(SerialNum));
				int nRet=0;
				nRet=Read_Multi(devLo,&SerialNum[0],0,9,5);

				if(nRet>0)
				{

					StrLog.Format(_T("Read good and Scan Back=%d (Between %d and %d)"),a,devLo,devHi);
					write_T3000_log_file(StrLog);

					nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
					nodetemp.ModbusID.Format(_T("%d"),devLo);
					nodetemp.ProductID.Format(_T("%d"),SerialNum[7]);
					nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,SerialNum[6]);
					nodetemp.SerialNO.Format(_T("%d"),SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256,SerialNum[3]*256*256*256);
					nodetemp.TstatID.Format(_T("%d"),SerialNum[6]);
					nodetemp.Brandrate=_T("19200");
					nodetemp.Status=_T("Online");
					float firmwareversion=((float)(SerialNum[MODBUS_VERSION_NUMBER_HI]*256+SerialNum[MODBUS_VERSION_NUMBER_LO]))/10.0;	
					nodetemp.Firmware.Format(_T("%.1f"),firmwareversion);
					BOOL Repeated=FALSE;
					for (int i=0;i<m_allnodes.size();i++)
					{
						if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
						{
							Repeated=TRUE;
							break;
						}
					}
					if (!Repeated)
					{
						m_allnodes.push_back(nodetemp);

						PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
					}

				}
				else
				{
					StrLog.Format(_T("Read fail and Scan Back=%d (Between %d and %d)"),a,devLo,devHi);
					write_T3000_log_file(StrLog);

					nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
					nodetemp.ModbusID.Format(_T("%d"),devLo);
					nodetemp.ProductID.Format(_T("Unknown"));
					nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,devLo);
					nodetemp.SerialNO.Format(_T("Unknown"));
					nodetemp.Status=_T("Offline");
					nodetemp.TstatID.Format(_T("Unknown"));
					nodetemp.Brandrate=_T("19200");
					BOOL Repeated=FALSE;
					for (int i=0;i<m_allnodes.size();i++)
					{
						if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
						{
							Repeated=TRUE;
							break;
						}
					}
					if (!Repeated)
					{
						m_allnodes.push_back(nodetemp);

						PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
					}
				}

			}
            #endif
			
			break;
			//the input error
		}
	//}

 
  
} 
else
{ 
// 	for (int scantime=1;scantime<=m_scan_times;scantime++)
// 	{
		for (int ID=devLo;ID<=devHi;ID++)
		{
			m_tips.Format(_T("COM%d  Between ID:%d and ID:%d Current ID=%d"),nComPort,devLo,devHi,ID);
			SendMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,1);
			unsigned short SerialNum[9];
			memset(SerialNum,0,sizeof(SerialNum));
			int nRet=0;
			nRet=Read_Multi(ID,&SerialNum[0],0,9,1);
			if(nRet>0)
			{
				StrLog.Format(_T("Read good and ID=%d"),ID);
				write_T3000_log_file(StrLog);
				nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
				nodetemp.ModbusID.Format(_T("%d"),ID);
				nodetemp.ProductID.Format(_T("%d"),SerialNum[7]);
				nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,SerialNum[6]);
				nodetemp.SerialNO.Format(_T("%d"),SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256,SerialNum[3]*256*256*256);
				nodetemp.TstatID.Format(_T("%d"),SerialNum[6]);
				nodetemp.Brandrate=_T("19200");
				nodetemp.Status=_T("Online");
				float firmwareversion=((float)(SerialNum[MODBUS_VERSION_NUMBER_HI]*256+SerialNum[MODBUS_VERSION_NUMBER_LO]))/10.0;	
				nodetemp.Firmware.Format(_T("%.1f"),firmwareversion);
				BOOL Repeated=FALSE;
				for (int i=0;i<m_allnodes.size();i++)
				{
					if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
					{
						Repeated=TRUE;
						break;
					}
				}
				if (!Repeated)
				{
					m_allnodes.push_back(nodetemp);
					PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
				}

			}
			else
			{

				StrLog.Format(_T("Read bad and ID=%d"),ID);
				write_T3000_log_file(StrLog);
				nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
				nodetemp.ModbusID.Format(_T("%d"),ID);
				nodetemp.ProductID.Format(_T("Unknown"));
				nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,ID);
				nodetemp.SerialNO.Format(_T("Unknown"));
				nodetemp.TstatID.Format(_T("Unknown"));
				nodetemp.Status=_T("No Read");
				nodetemp.Brandrate=_T("19200");
				nodetemp.Firmware=_T("Unknown");
				BOOL Repeated=FALSE;
				for (int i=0;i<m_allnodes.size();i++)
				{
					if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
					{
						Repeated=TRUE;
						break;
					}
				}
				if (!Repeated)
				{
					m_allnodes.push_back(nodetemp);

					PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
				}
				//}
			}
		}
	//}

}

#if 0
for (int ID=devLo;ID<=devHi;ID++)
{
	m_tips.Format(_T("COM%d  Between ID:%d and ID:%d Current ID=%d"),nComPort,devLo,devHi,ID);
	SendMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,1);
	unsigned short SerialNum[9];
	memset(SerialNum,0,sizeof(SerialNum));
	int nRet=0;
	nRet=Read_Multi(ID,&SerialNum[0],0,9,1);
	if(nRet>0)
	{
		nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
		nodetemp.ModbusID.Format(_T("%d"),ID);
		nodetemp.ProductID.Format(_T("%d"),SerialNum[7]);
		nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,SerialNum[6]);
		nodetemp.SerialNO.Format(_T("%d"),SerialNum[0]+SerialNum[1]*256+SerialNum[2]*256*256,SerialNum[3]*256*256*256);
		nodetemp.TstatID.Format(_T("%d"),SerialNum[6]);
		nodetemp.Brandrate=_T("19200");
		nodetemp.Status=_T("Online");
		float firmwareversion=((float)(SerialNum[MODBUS_VERSION_NUMBER_HI]*256+SerialNum[MODBUS_VERSION_NUMBER_LO]))/10.0;	
		nodetemp.Firmware.Format(_T("%.1f"),firmwareversion);
		BOOL Repeated=FALSE;
		for (int i=0;i<m_allnodes.size();i++)
		{
			if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
			{
				Repeated=TRUE;
				break;
			}
		}
		if (!Repeated)
		{
			m_allnodes.push_back(nodetemp);
			PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
		}

	}
	else
	{

		nodetemp.ComportNo.Format(_T("COM%d"),nComPort);
		nodetemp.ModbusID.Format(_T("%d"),ID);
		nodetemp.ProductID.Format(_T("Unknown"));
		nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,ID);
		nodetemp.SerialNO.Format(_T("Unknown"));
		nodetemp.TstatID.Format(_T("Unknown"));
		nodetemp.Status=_T("No Read");
		nodetemp.Brandrate=_T("19200");
		nodetemp.Firmware=_T("Unknown");
		BOOL Repeated=FALSE;
		for (int i=0;i<m_allnodes.size();i++)
		{
			if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
			{
				Repeated=TRUE;
				break;
			}
		}
		if (!Repeated)
		{
			m_allnodes.push_back(nodetemp);

			PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
		}
		//}
	}
}


#endif

}

LRESULT CDetectOnlineDlg::OnThreadFinished(WPARAM wParam, LPARAM /*lParam*/)
{
   int Step =(int)wParam;
   if (Step==1)
   {
     UpdateData(FALSE);
   }
   else if (Step==2)
   {
     //FreshGrid();
	    NODES_DETECTS TempNode=m_allnodes[m_allnodes.size()-1];
		int rows=m_detect_grid.get_Rows()+1;
	    m_detect_grid.put_Rows(rows);
		int CurrentRow=m_detect_grid.get_Rows()-1;
	   if (TempNode.Status.CompareNoCase(_T("Conflict"))==0)
	   {
	      
		   m_detect_grid.put_Row(CurrentRow);
		   m_detect_grid.put_Col(0);
		   m_detect_grid.put_CellForeColor(RED_COLOR);
		   m_detect_grid.put_Col(1);
		   m_detect_grid.put_CellForeColor(RED_COLOR);
		   m_detect_grid.put_Col(2);
		   m_detect_grid.put_CellForeColor(RED_COLOR);
		   m_detect_grid.put_Col(3);
		   m_detect_grid.put_CellForeColor(RED_COLOR);
		   m_detect_grid.put_Col(4);
		   m_detect_grid.put_CellForeColor(RED_COLOR);
		   m_detect_grid.put_Col(5);
		   m_detect_grid.put_CellForeColor(RED_COLOR);

		   m_detect_grid.put_TextMatrix(CurrentRow,0,TempNode.ComportNo);
		   m_detect_grid.put_TextMatrix(CurrentRow,1,TempNode.ModbusID);
		   m_detect_grid.put_TextMatrix(CurrentRow,2,TempNode.ProductID);
		   m_detect_grid.put_TextMatrix(CurrentRow,3,TempNode.RoomNO);
		   m_detect_grid.put_TextMatrix(CurrentRow,4,TempNode.SerialNO);
		   m_detect_grid.put_TextMatrix(CurrentRow,5,TempNode.Status);
		   m_detect_grid.put_TextMatrix(CurrentRow,6,TempNode.TstatID);
		   m_detect_grid.put_TextMatrix(CurrentRow,8,TempNode.Firmware);

	   }
	   else if (TempNode.Status.CompareNoCase(_T("Online"))==0)
	   {
		   m_detect_grid.put_Row(CurrentRow);
		   m_detect_grid.put_Col(0);
		   m_detect_grid.put_CellForeColor(GREEN_COLOR);
		   m_detect_grid.put_Col(1);
		   m_detect_grid.put_CellForeColor(GREEN_COLOR);
		   m_detect_grid.put_Col(2);
		   m_detect_grid.put_CellForeColor(GREEN_COLOR);
		   m_detect_grid.put_Col(3);
		   m_detect_grid.put_CellForeColor(GREEN_COLOR);
		   m_detect_grid.put_Col(4);
		   m_detect_grid.put_CellForeColor(GREEN_COLOR);
		   m_detect_grid.put_Col(5);
		   m_detect_grid.put_CellForeColor(GREEN_COLOR);

		   m_detect_grid.put_TextMatrix(CurrentRow,0,TempNode.ComportNo);
		   m_detect_grid.put_TextMatrix(CurrentRow,1,TempNode.ModbusID);
		   m_detect_grid.put_TextMatrix(CurrentRow,2,TempNode.ProductID);
		   m_detect_grid.put_TextMatrix(CurrentRow,3,TempNode.RoomNO);
		   m_detect_grid.put_TextMatrix(CurrentRow,4,TempNode.SerialNO);
		   m_detect_grid.put_TextMatrix(CurrentRow,5,TempNode.Status);
		   m_detect_grid.put_TextMatrix(CurrentRow,6,TempNode.TstatID);
		    m_detect_grid.put_TextMatrix(CurrentRow,8,TempNode.Firmware);

		/*   m_detect_grid.add*/

	   }
	   else
	   {

		   m_detect_grid.put_Row(CurrentRow);
		   m_detect_grid.put_Col(0);
		   m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
		   m_detect_grid.put_Col(1);
		   m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
		   m_detect_grid.put_Col(2);
		   m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
		   m_detect_grid.put_Col(3);
		   m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
		   m_detect_grid.put_Col(4);
		   m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
		   m_detect_grid.put_Col(5);
		   m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);

		   m_detect_grid.put_TextMatrix(CurrentRow,0,TempNode.ComportNo);
		   m_detect_grid.put_TextMatrix(CurrentRow,1,TempNode.ModbusID);
		   m_detect_grid.put_TextMatrix(CurrentRow,2,TempNode.ProductID);
		   m_detect_grid.put_TextMatrix(CurrentRow,3,TempNode.RoomNO);
		   m_detect_grid.put_TextMatrix(CurrentRow,4,TempNode.SerialNO);
		   m_detect_grid.put_TextMatrix(CurrentRow,5,TempNode.Status);
		   m_detect_grid.put_TextMatrix(CurrentRow,6,TempNode.TstatID);
		    m_detect_grid.put_TextMatrix(CurrentRow,8,TempNode.Firmware);

	   }
   }
   else if (Step==3)
   {

  
// 	   if (m_pThread)
// 	   {
// 		   WaitForSingleObject(m_pThread->m_hThread, INFINITE);
// 		   delete m_pThread;
// 		   m_pThread = NULL;
// 	   }
   }
   else if (Step==4)
   {
       SaveToDB();
	   GetDlgItem(IDC_BUTTON_DETECT)->EnableWindow(TRUE);
	   GetDlgItem(IDC_BUTTON_SCAN)->EnableWindow(TRUE);
	   GetDlgItem(IDC_BUTTON_REPAIRCONFLICT)->EnableWindow(TRUE);
   }
   else if (Step==6)
   {
	   GetDlgItem(IDC_BUTTON_DETECT)->EnableWindow(TRUE);
	   GetDlgItem(IDC_BUTTON_SCAN)->EnableWindow(TRUE);
	   GetDlgItem(IDC_BUTTON_REPAIRCONFLICT)->EnableWindow(TRUE);
   }
   else if (Step == 5)
   {
      int rows=m_detect_grid.get_Rows()+254;
	  m_detect_grid.put_Rows(rows);
	  int CurrentRow=m_detect_grid.get_Rows()-254;
      NODES_DETECTS TempNode; 
	  for (int i=0;i<254;i++)
	  {
	      TempNode=m_allnodes[m_allnodes.size()-254+i];
		  if (TempNode.Status.CompareNoCase(_T("Conflict"))==0)
		  {

			  m_detect_grid.put_Row(CurrentRow);
			  m_detect_grid.put_Col(0);
			  m_detect_grid.put_CellForeColor(RED_COLOR);
			  m_detect_grid.put_Col(1);
			  m_detect_grid.put_CellForeColor(RED_COLOR);
			  m_detect_grid.put_Col(2);
			  m_detect_grid.put_CellForeColor(RED_COLOR);
			  m_detect_grid.put_Col(3);
			  m_detect_grid.put_CellForeColor(RED_COLOR);
			  m_detect_grid.put_Col(4);
			  m_detect_grid.put_CellForeColor(RED_COLOR);
			  m_detect_grid.put_Col(5);
			  m_detect_grid.put_CellForeColor(RED_COLOR);

			  m_detect_grid.put_TextMatrix(CurrentRow,0,TempNode.ComportNo);
			  m_detect_grid.put_TextMatrix(CurrentRow,1,TempNode.ModbusID);
			  m_detect_grid.put_TextMatrix(CurrentRow,2,TempNode.ProductID);
			  m_detect_grid.put_TextMatrix(CurrentRow,3,TempNode.RoomNO);
			  m_detect_grid.put_TextMatrix(CurrentRow,4,TempNode.SerialNO);
			  m_detect_grid.put_TextMatrix(CurrentRow,5,TempNode.Status);
			  m_detect_grid.put_TextMatrix(CurrentRow,6,TempNode.TstatID);
			   m_detect_grid.put_TextMatrix(CurrentRow,8,TempNode.Firmware);


		  }
		  else if (TempNode.Status.CompareNoCase(_T("Online"))==0)
		  {
			  m_detect_grid.put_Row(CurrentRow);
			  m_detect_grid.put_Col(0);
			  m_detect_grid.put_CellForeColor(GREEN_COLOR);
			  m_detect_grid.put_Col(1);
			  m_detect_grid.put_CellForeColor(GREEN_COLOR);
			  m_detect_grid.put_Col(2);
			  m_detect_grid.put_CellForeColor(GREEN_COLOR);
			  m_detect_grid.put_Col(3);
			  m_detect_grid.put_CellForeColor(GREEN_COLOR);
			  m_detect_grid.put_Col(4);
			  m_detect_grid.put_CellForeColor(GREEN_COLOR);
			  m_detect_grid.put_Col(5);
			  m_detect_grid.put_CellForeColor(GREEN_COLOR);

			  m_detect_grid.put_TextMatrix(CurrentRow,0,TempNode.ComportNo);
			  m_detect_grid.put_TextMatrix(CurrentRow,1,TempNode.ModbusID);
			  m_detect_grid.put_TextMatrix(CurrentRow,2,TempNode.ProductID);
			  m_detect_grid.put_TextMatrix(CurrentRow,3,TempNode.RoomNO);
			  m_detect_grid.put_TextMatrix(CurrentRow,4,TempNode.SerialNO);
			  m_detect_grid.put_TextMatrix(CurrentRow,5,TempNode.Status);
			  m_detect_grid.put_TextMatrix(CurrentRow,6,TempNode.TstatID);
			   m_detect_grid.put_TextMatrix(CurrentRow,8,TempNode.Firmware);
		  }
		  else
		  {

			  m_detect_grid.put_Row(CurrentRow);
			  m_detect_grid.put_Col(0);
			  m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
			  m_detect_grid.put_Col(1);
			  m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
			  m_detect_grid.put_Col(2);
			  m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
			  m_detect_grid.put_Col(3);
			  m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
			  m_detect_grid.put_Col(4);
			  m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
			  m_detect_grid.put_Col(5);
			  m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);

			  m_detect_grid.put_TextMatrix(CurrentRow,0,TempNode.ComportNo);
			  m_detect_grid.put_TextMatrix(CurrentRow,1,TempNode.ModbusID);
			  m_detect_grid.put_TextMatrix(CurrentRow,2,TempNode.ProductID);
			  m_detect_grid.put_TextMatrix(CurrentRow,3,TempNode.RoomNO);
			  m_detect_grid.put_TextMatrix(CurrentRow,4,TempNode.SerialNO);
			  m_detect_grid.put_TextMatrix(CurrentRow,5,TempNode.Status);
			  m_detect_grid.put_TextMatrix(CurrentRow,6,TempNode.TstatID);
			   m_detect_grid.put_TextMatrix(CurrentRow,8,TempNode.Firmware);

		  }
		  CurrentRow++;
	  }
   }

	return 0L;
}
void CDetectOnlineDlg::GetNode(CString COM,int ID,NODES_DETECTS &nodes){
CString id;
 
id.Format(_T("%d"),ID);
for (int i=0;i<m_allnodes.size();i++)
{
  if((m_allnodes[i].ComportNo.CompareNoCase(COM)==0)&&(m_allnodes[i].ModbusID.CompareNoCase(id)==0))
  {
     nodes=m_allnodes[i];
  }
}
}
void CDetectOnlineDlg::SetNode(CString COM,CString ID,NODES_DETECTS nodeValue){
	for (int i=0;i<m_allnodes.size();i++)
	{
		if((m_allnodes[i].ComportNo.CompareNoCase(COM)==0)&&(m_allnodes[i].ModbusID.CompareNoCase(ID)==0))
		{
			//nodes=m_allnodes[i];
			m_allnodes[i]=nodeValue;
			break;
		}
	}
}
void CDetectOnlineDlg::FreshGrid(){
	int cols=9;
	int rows=m_comports.size()*(m_allnodes.size())+1;
	m_detect_grid.put_Cols(cols);
	m_detect_grid.put_Rows(rows);
	m_detect_grid.put_TextMatrix(0,0,_T("COM Port"));
	m_detect_grid.put_TextMatrix(0,1,_T("Modbus ID"));
	m_detect_grid.put_TextMatrix(0,2,_T("Product ID"));
	m_detect_grid.put_TextMatrix(0,3,_T("Node Name"));
	m_detect_grid.put_TextMatrix(0,4,_T("Serial No"));
	m_detect_grid.put_TextMatrix(0,5,_T("Status"));
	m_detect_grid.put_TextMatrix(0,6,_T("TstatID"));
	m_detect_grid.put_TextMatrix(0,7,_T("Write Serial No"));
   


	int CurrentRow=1;
	NODES_DETECTS TempNode;
	/*int CurrentCol=1;*/
	for (int i=0;i<m_comports.size();i++)
	{
		for(int j=1;j<=m_allnodes.size();j++)
		{
		   GetNode(m_comports[i],j,TempNode);
		   if (TempNode.Status.CompareNoCase(_T("Conflict"))==0)
		   {
			   m_detect_grid.put_Row(CurrentRow);
			   m_detect_grid.put_Col(0);
			   m_detect_grid.put_CellForeColor(RED_COLOR);
			   m_detect_grid.put_Col(1);
			   m_detect_grid.put_CellForeColor(RED_COLOR);
			   m_detect_grid.put_Col(2);
			   m_detect_grid.put_CellForeColor(RED_COLOR);
			   m_detect_grid.put_Col(3);
			   m_detect_grid.put_CellForeColor(RED_COLOR);
			   m_detect_grid.put_Col(4);
			   m_detect_grid.put_CellForeColor(RED_COLOR);
			   m_detect_grid.put_Col(5);
			   m_detect_grid.put_CellForeColor(RED_COLOR);

			   m_detect_grid.put_TextMatrix(CurrentRow,0,TempNode.ComportNo);
			   m_detect_grid.put_TextMatrix(CurrentRow,1,TempNode.ModbusID);
			   m_detect_grid.put_TextMatrix(CurrentRow,2,TempNode.ProductID);
			   m_detect_grid.put_TextMatrix(CurrentRow,3,TempNode.RoomNO);
			   m_detect_grid.put_TextMatrix(CurrentRow,4,TempNode.SerialNO);
			   m_detect_grid.put_TextMatrix(CurrentRow,5,TempNode.Status);
			   m_detect_grid.put_TextMatrix(CurrentRow,6,TempNode.TstatID);

		   }
		  else if (TempNode.Status.CompareNoCase(_T("Online"))==0)
		   {
			   m_detect_grid.put_Row(CurrentRow);
			   m_detect_grid.put_Col(0);
			   m_detect_grid.put_CellForeColor(GREEN_COLOR);
			   m_detect_grid.put_Col(1);
			   m_detect_grid.put_CellForeColor(GREEN_COLOR);
			   m_detect_grid.put_Col(2);
			   m_detect_grid.put_CellForeColor(GREEN_COLOR);
			   m_detect_grid.put_Col(3);
			   m_detect_grid.put_CellForeColor(GREEN_COLOR);
			   m_detect_grid.put_Col(4);
			   m_detect_grid.put_CellForeColor(GREEN_COLOR);
			   m_detect_grid.put_Col(5);
			   m_detect_grid.put_CellForeColor(GREEN_COLOR);

			   m_detect_grid.put_TextMatrix(CurrentRow,0,TempNode.ComportNo);
			   m_detect_grid.put_TextMatrix(CurrentRow,1,TempNode.ModbusID);
			   m_detect_grid.put_TextMatrix(CurrentRow,2,TempNode.ProductID);
			   m_detect_grid.put_TextMatrix(CurrentRow,3,TempNode.RoomNO);
			   m_detect_grid.put_TextMatrix(CurrentRow,4,TempNode.SerialNO);
			   m_detect_grid.put_TextMatrix(CurrentRow,5,TempNode.Status);
			   m_detect_grid.put_TextMatrix(CurrentRow,6,TempNode.TstatID);

		   }
		   else
		   {
			  
				 m_detect_grid.put_Row(CurrentRow);
				 m_detect_grid.put_Col(0);
				 m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
				 m_detect_grid.put_Col(1);
				 m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
				 m_detect_grid.put_Col(2);
				 m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
				 m_detect_grid.put_Col(3);
				 m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
				 m_detect_grid.put_Col(4);
				 m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
				 m_detect_grid.put_Col(5);
				 m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);

				 m_detect_grid.put_TextMatrix(CurrentRow,0,TempNode.ComportNo);
				 m_detect_grid.put_TextMatrix(CurrentRow,1,TempNode.ModbusID);
				 m_detect_grid.put_TextMatrix(CurrentRow,2,TempNode.ProductID);
				 m_detect_grid.put_TextMatrix(CurrentRow,3,TempNode.RoomNO);
				 m_detect_grid.put_TextMatrix(CurrentRow,4,TempNode.SerialNO);
				 m_detect_grid.put_TextMatrix(CurrentRow,5,TempNode.Status);
				 m_detect_grid.put_TextMatrix(CurrentRow,6,TempNode.TstatID);

		  
		   }
		   CurrentRow++;
		}
	}
}
void CDetectOnlineDlg::FLEX_GRID1_PUT_STR(int row,int col)			
{
	 
// 	m_detect_grid.put_Row(row);
// 	m_detect_grid.put_Col(col);
// 	m_detect_grid.put_CellForeColor(RGB(0,0,0));
// 	if(str.CompareNoCase(_T("Online"))==0)
// 	{
// 		m_detect_grid.put_CellBackColor(GREEN_COLOR);
// 	}
// 	else if (str.CompareNoCase(_T("Conflict"))==0)
// 	{
// 		m_detect_grid.put_CellBackColor(RED_COLOR);
// 	}
// 	else
// 	{
// 		m_detect_grid.put_CellBackColor(FLEXGRID_CELL_COLOR);
// 	}

}//str must be declare
void CDetectOnlineDlg::SaveToDB(){
  CppSQLite3DB SqliteDBT3000;
  SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
  CppSQLite3DB SqliteDBBuilding;
  CppSQLite3Table table;
  CppSQLite3Query q;

  SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
  
CString strSql ;
 
vector_unique(m_allnodes);

for (int i=0;i<m_allnodes.size();i++)
{
	strSql.Format(_T("insert into DetectTstatOnline(COM,ModbusID,ProductID,RoomNo,SerialNo,Status,TStatID,FirmwareVer) values('"+m_allnodes[i].ComportNo+"','"+m_allnodes[i].ModbusID+"','"+m_allnodes[i].ProductID+"','"+m_allnodes[i].RoomNO+"','"+m_allnodes[i].SerialNO+"','"+m_allnodes[i].Status+"','"+m_allnodes[i].TstatID+"','"+m_allnodes[i].Firmware+"')"));
   try
   {
	   SqliteDBT3000.execDML((UTF8MBSTR)strSql);
   }
   catch(_com_error *e)
   {
	   AfxMessageBox(_T("Error"));
	   return;
   }
   if (m_allnodes.at(i).Status.CompareNoCase(_T("Online"))==0)
   {
#if 1
	   CString strSql;
	   CString strMainBuildName,strSubBuildingName,strSID,strFloorName,strRoomName,strProID;
	   CString strProName,strProType,strScreenID,strBaudrate,strGraphicID,strHdVersion,strStVersion,strCom,strEPSize;
	   CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	   strMainBuildName= pFrame->m_strCurMainBuildingName;
	   strSubBuildingName= pFrame->m_strCurSubBuldingName;

	   strSID.Format(_T("%s"),m_allnodes.at(i).SerialNO);
	   strFloorName=_T("floor1");
	   strRoomName=m_allnodes.at(i).ComportNo;
	   strProName.Format(_T("%s:%s--%s"),GetProductName(_wtoi(m_allnodes.at(i).ProductID)).GetBuffer(),strSID,m_allnodes.at(i).ModbusID);
	   strProType.Format(_T("%s"),m_allnodes.at(i).ProductID);
	   strProID.Format(_T("%s"),m_allnodes.at(i).ModbusID);

	   strScreenID.Format(_T("Screen(S:%s--%s)"),strSID,strProID);
	   

	   strBaudrate=m_allnodes.at(i).Brandrate;
	   strGraphicID=_T("Clicking here to add a image...");
	   strHdVersion=_T("0.0");
	   strStVersion=m_allnodes.at(i).Firmware;



	   strCom = m_allnodes.at(i).ComportNo.Mid(3);


	   try
	   {


		   strSql.Format(_T("Select * from ALL_NODE where Product_name='%s'"),strProName);
		     q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
		   if (!q.eof())//有表但是没有对应序列号的值
		   {
			   strSql.Format(_T("update ALL_NODE set Software_Ver ='%s' where Serial_ID = '%s' "),m_allnodes.at(i).Firmware,m_allnodes.at(i).SerialNO);
			   SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
		   }
		   strSql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,Com_Port,EPsize) values('"
			   +strMainBuildName+"','"
			   +strSubBuildingName +"','"
			   +strSID+"','"
			   +strFloorName+"','"
			   +strRoomName+"','"
			   +strProName+"','"
			   +strProType+"','"
			   +strProID+"','"
			   +strScreenID+"','"
			   +strBaudrate+"','"
			   +strGraphicID+"','"
			   +strHdVersion+"','"
			   +strStVersion+"','"
			   +strCom+"','"
			   +strEPSize+"')"));
		  SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
		    

	   }

	   catch (CException* e)
	   {
		   AfxMessageBox(_T("Add Fail!"));
	   }
#endif
   }

}
 SqliteDBBuilding.closedb();
 SqliteDBT3000.closedb();
}
int CDetectOnlineDlg::vector_unique(vector<NODES_DETECTS> &vt)
{
	sort(vt.begin(), vt.end());
	vt.erase(unique(vt.begin(), vt.end()), vt.end());
	return 1;
}
BOOL CDetectOnlineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Update_Check();
	m_combox_tf.ResetContent();
	m_combox_tf.AddString(_T("FALSE"));
	m_combox_tf.AddString(_T("TRUE"));
	m_allnodes.clear();
	NODES_DETECTS TempNode;
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3Table table;
	CppSQLite3Query q;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
	CString temptable = _T("DetectTstatOnline");
	CString createtable = _T("create table DetectTstatOnline(COM text,ModbusID text,ProductID text,RoomNo text,SerialNo text,Status text,TStatID text,FirmwareVer text)");//
	CString strSql ;
	bool m_judge = SqliteDBT3000.tableExists("DetectTstatOnline");
	if (!m_judge)
	{
		SqliteDBT3000.execDML((UTF8MBSTR)createtable);
		//return TRUE;
	}

	strSql.Format(_T("select * from DetectTstatOnline"));
	try
	{
		 q=SqliteDBT3000.execQuery((UTF8MBSTR)strSql);	
	}
	catch(_com_error *e)
	{
		/*AfxMessageBox(_T("Error"));*/
		return TRUE;
	}
	_variant_t vartemp;
	
	while (!q.eof())
	{
	   
	 
	   TempNode.ComportNo = q.getValuebyName(L"COM");
	   TempNode.ModbusID = q.getValuebyName(L"ModbusID");
	   TempNode.ProductID= q.getValuebyName(L"ProductID");
	   TempNode.RoomNO=q.getValuebyName(L"RoomNo");
	   TempNode.SerialNO = q.getValuebyName(L"SerialNo");
	   TempNode.TstatID = q.getValuebyName(L"TStatID");
	   TempNode.Firmware = q.getValuebyName(L"FirmwareVer");
	   m_allnodes.push_back(TempNode);
	   q.nextRow();
	}
	int rows=m_allnodes.size();
	m_detect_grid.put_Rows(rows+1);
	m_detect_grid.put_Cols(9);
	m_detect_grid.put_TextMatrix(0,0,_T("COM Port"));
	m_detect_grid.put_TextMatrix(0,1,_T("Modbus ID"));
	m_detect_grid.put_TextMatrix(0,2,_T("Product ID"));
	m_detect_grid.put_TextMatrix(0,3,_T("Node Name"));
	m_detect_grid.put_TextMatrix(0,4,_T("Serial No"));
	m_detect_grid.put_TextMatrix(0,5,_T("Status"));
	m_detect_grid.put_TextMatrix(0,6,_T("TstatID"));
	m_detect_grid.put_TextMatrix(0,7,_T("Write Serial No"));
	m_detect_grid.put_TextMatrix(0,8,_T("Firmware Version"));
	for (int i=0;i<rows;i++)
	{

		if (m_allnodes[i].Status.CompareNoCase(_T("Conflict"))==0)
		{
			m_detect_grid.put_Row(i+1);
			m_detect_grid.put_Col(0);
			m_detect_grid.put_CellForeColor(RED_COLOR);
			m_detect_grid.put_Col(1);
			m_detect_grid.put_CellForeColor(RED_COLOR);
			m_detect_grid.put_Col(2);
			m_detect_grid.put_CellForeColor(RED_COLOR);
			m_detect_grid.put_Col(3);
			m_detect_grid.put_CellForeColor(RED_COLOR);
			m_detect_grid.put_Col(4);
			m_detect_grid.put_CellForeColor(RED_COLOR);
			m_detect_grid.put_Col(5);
			m_detect_grid.put_CellForeColor(RED_COLOR);

			m_detect_grid.put_TextMatrix(i+1,0,m_allnodes[i].ComportNo);
			m_detect_grid.put_TextMatrix(i+1,1,m_allnodes[i].ModbusID);
			m_detect_grid.put_TextMatrix(i+1,2,m_allnodes[i].ProductID);
			m_detect_grid.put_TextMatrix(i+1,3,m_allnodes[i].RoomNO);
			m_detect_grid.put_TextMatrix(i+1,4,m_allnodes[i].SerialNO);
			m_detect_grid.put_TextMatrix(i+1,5,m_allnodes[i].Status);
			m_detect_grid.put_TextMatrix(i+1,6,m_allnodes[i].TstatID);
			m_detect_grid.put_TextMatrix(i+1,8,m_allnodes[i].Firmware);
		}
		else if (m_allnodes[i].Status.CompareNoCase(_T("Online"))==0)
		{
			m_detect_grid.put_Row(i+1);
			m_detect_grid.put_Col(0);
			m_detect_grid.put_CellForeColor(GREEN_COLOR);
			m_detect_grid.put_Col(1);
			m_detect_grid.put_CellForeColor(GREEN_COLOR);
			m_detect_grid.put_Col(2);
			m_detect_grid.put_CellForeColor(GREEN_COLOR);
			m_detect_grid.put_Col(3);
			m_detect_grid.put_CellForeColor(GREEN_COLOR);
			m_detect_grid.put_Col(4);
			m_detect_grid.put_CellForeColor(GREEN_COLOR);
			m_detect_grid.put_Col(5);
			m_detect_grid.put_CellForeColor(GREEN_COLOR);

			m_detect_grid.put_TextMatrix(i+1,0,m_allnodes[i].ComportNo);
			m_detect_grid.put_TextMatrix(i+1,1,m_allnodes[i].ModbusID);
			m_detect_grid.put_TextMatrix(i+1,2,m_allnodes[i].ProductID);
			m_detect_grid.put_TextMatrix(i+1,3,m_allnodes[i].RoomNO);
			m_detect_grid.put_TextMatrix(i+1,4,m_allnodes[i].SerialNO);
			m_detect_grid.put_TextMatrix(i+1,5,m_allnodes[i].Status);
			m_detect_grid.put_TextMatrix(i+1,6,m_allnodes[i].TstatID);
			m_detect_grid.put_TextMatrix(i+1,8,m_allnodes[i].Firmware);

		}
		else
		{
			m_detect_grid.put_Row(i+1);
			m_detect_grid.put_Col(0);
			m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
			m_detect_grid.put_Col(1);
			m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
			m_detect_grid.put_Col(2);
			m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
			m_detect_grid.put_Col(3);
			m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
			m_detect_grid.put_Col(4);
			m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);
			m_detect_grid.put_Col(5);
			m_detect_grid.put_CellForeColor(FLEXGRID_CELL_COLOR);

			m_detect_grid.put_TextMatrix(i+1,0,m_allnodes[i].ComportNo);
			m_detect_grid.put_TextMatrix(i+1,1,m_allnodes[i].ModbusID);
			m_detect_grid.put_TextMatrix(i+1,2,m_allnodes[i].ProductID);
			m_detect_grid.put_TextMatrix(i+1,3,m_allnodes[i].RoomNO);
			m_detect_grid.put_TextMatrix(i+1,4,m_allnodes[i].SerialNO);
			m_detect_grid.put_TextMatrix(i+1,5,m_allnodes[i].Status);
			m_detect_grid.put_TextMatrix(i+1,6,m_allnodes[i].TstatID);
			m_detect_grid.put_TextMatrix(i+1,8,m_allnodes[i].Firmware);
		}



	}



//   CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
//   pFrame->SuspendRefreshThread();
//   pFrame->m_pFreshMultiRegisters->SuspendThread();
 
   GetSerialComPortNumber1(m_comports);

  m_allcomportTable.put_Rows(m_comports.size()+1);
  m_allcomportTable.put_Cols(5);
  m_allcomportTable.put_TextMatrix(0,0,_T("Scan Type"));
  m_allcomportTable.put_TextMatrix(0,1,_T("Selected"));
  m_allcomportTable.put_TextMatrix(0,2,_T("Name"));
  m_allcomportTable.put_TextMatrix(0,3,_T("From(ID)"));
  m_allcomportTable.put_TextMatrix(0,4,_T("To(ID)"));
  for (int i=0;i<m_comports.size();i++)
  {
	  m_allcomportTable.put_TextMatrix(i+1,0,m_comports[i]);
	  m_allcomportTable.put_TextMatrix(i+1,1,_T("FALSE"));
	  m_allcomportTable.put_TextMatrix(i+1,2,_T("NODE"));
	  m_allcomportTable.put_TextMatrix(i+1,3,_T("1"));
	  m_allcomportTable.put_TextMatrix(i+1,4,_T("254"));
  }

  CStdioFile default_file;
  CString saved_path=g_strExePth+_T("Serial.txt");
  CString a_line;
  CStringArray ArrayString;
  CString StrTemp;
  if(default_file.Open(saved_path.GetString(),CFile::modeReadWrite | CFile::modeNoTruncate))
  {
	  while(default_file.ReadString(StrTemp))
	  {
		  a_line = StrTemp;		
	  }	

	  if (!a_line.IsEmpty())
	  {
	  SplitCStringA(ArrayString,a_line,_T(":"));
	  if (ArrayString.GetSize()>5)
	  {
		  CString Seriallast;
		  Seriallast=ArrayString[4];
		  m_edit_serialNo.SetWindowTextW(Seriallast);
	  }
	 
	  }
	  default_file.Close();
  }
 
	return TRUE; 
 
}
BEGIN_EVENTSINK_MAP(CDetectOnlineDlg, CDialogEx)
	ON_EVENT(CDetectOnlineDlg, IDC_MSFLEXGRID_RESULT, DISPID_CLICK, CDetectOnlineDlg::ClickMsflexgridResult, VTS_NONE)
	ON_EVENT(CDetectOnlineDlg, IDC_MSFLEXGRID_RESULT2, DISPID_CLICK, CDetectOnlineDlg::ClickMsflexgridResult2, VTS_NONE)
END_EVENTSINK_MAP()


void CDetectOnlineDlg::ClickMsflexgridResult()
{
	long lRow,lCol;

	lRow = m_detect_grid.get_RowSel();//获取点击的行号	
	lCol = m_detect_grid.get_ColSel(); //获取点击的列号
	if(lRow>m_detect_grid.get_Rows()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow == 0) //如果点击标题行，也无效
		return;
	CRect rect;
	m_detect_grid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	// MSFlexGrid控件的函数的长度单位是"缇(twips)"，
	//需要将其转化为像素，1440缇= 1英寸
	CDC* pDC =GetDC();
	//计算象素点和缇的转换比例
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_detect_grid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_detect_grid.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_detect_grid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_detect_grid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rc.OffsetRect(rect.left+1,rect.top+1);
	//获取选中格的文本信息
	CString strValue = m_detect_grid.get_TextMatrix(lRow,lCol);
	CString strStatus= m_detect_grid.get_TextMatrix(lRow,5);
	m_nCurRow=lRow;
	m_nCurCol=lCol;
	if (lCol==0||lCol==1||lCol==5||lCol==4)//COM ,Modbus ID,Room ,Serial No, Status
	{
	return;
	}
	if (lCol==7)
	{
		m_writesnBtn.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_writesnBtn.ShowWindow(SW_SHOW);

		m_writesnBtn.BringWindowToTop();
		//m_RangCombox.SelectString(-1,strValue);
		m_writesnBtn.SetWindowText(strValue);
		m_writesnBtn.SetFocus(); //获取焦点
	}
	else
	{
		m_editor.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_editor.ShowWindow(SW_SHOW);

		m_editor.BringWindowToTop();
		//m_RangCombox.SelectString(-1,strValue);
		m_editor.SetWindowText(strValue);
		m_editor.SetFocus(); //获取焦点
	}
// 	if (strStatus.CompareNoCase(_T("Online"))==0)
// 	{

/*	}*/
}


void CDetectOnlineDlg::OnEnKillfocusEdit1()
{
	NODES_DETECTS TempNode;
	CString COMPort=m_detect_grid.get_TextMatrix(m_nCurRow,0);
	CString ModbusID=m_detect_grid.get_TextMatrix(m_nCurRow,1);
	CString ProductID=m_detect_grid.get_TextMatrix(m_nCurRow,2);
	CString RoomNo=m_detect_grid.get_TextMatrix(m_nCurRow,3);
	CString SerialNo=m_detect_grid.get_TextMatrix(m_nCurRow,4);

	int IntModbusID=_wtoi(ModbusID.GetBuffer());
	CString StrValue,StrGrid;
	StrGrid=m_detect_grid.get_TextMatrix(m_nCurRow,m_nCurCol);
	 m_editor.GetWindowTextW(StrValue);
	 if (StrValue.CompareNoCase(StrGrid)==0)
	 {
	 return ;
	 }

	 int port=_wtoi(COMPort.Mid(3));
	 if (m_nCurCol==1)//Modbus ID
	 {
	   for (int i=0;i<m_allnodes.size();i++)
	   {
	     if(m_allnodes[i].ComportNo.CompareNoCase(COMPort)==0)
	     {
			 if(m_allnodes[i].Status.CompareNoCase(_T("Online"))==0)
			 {
			     if (m_allnodes[i].ModbusID.CompareNoCase(_T("StrValue"))==0)
			     {				    
				    AfxMessageBox(_T("The Modbus ID is occupied"));
					return ;
			     }
			 }
	     }
	   }
	   int value=_wtoi(StrValue);
	   if (open_com(port))
	   {   int modelid=_wtoi(ProductID);
			int regAddress;
			if (modelid==6||modelid==7||modelid==8)
			{
			   regAddress=639;
			}
			else
			{
			   regAddress=568;
			}
		   int ret1=write_one(IntModbusID,regAddress,0);
		   Sleep(2000);
		   if (ret1>0)
		   {
			   int ret2=write_one(IntModbusID,6,value);
			   if (ret2>0)
			   {
				   TempNode.ComportNo=COMPort;
				   TempNode.ModbusID=StrValue;
				   TempNode.ProductID=ProductID;
				   TempNode.RoomNO=RoomNo;
				   TempNode.SerialNO=SerialNo;
				   TempNode.Status=_T("Online");
				   SetNode(COMPort,ModbusID,TempNode);
				   PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,4);
				   m_detect_grid.put_TextMatrix(m_nCurRow,m_nCurCol,StrValue);
			   }
		   }
		   close_com();
	   }



	 } 
	 else if (m_nCurCol==2)
	 {
	   int ProductID=_wtoi(StrValue);
	   if (open_com(port))
	   {
 	      int ret= write_one(IntModbusID,16,143);
 		    Sleep(1000);
		  int ii=0;
		  while(ii<15){
		  Sleep(1000);
		  ret=Read_One(IntModbusID,6);
		  if (ret<0)
		  {
		  ii++;
		  }
		  else{
		  if (ret==35)
		  {
		  ii++;
		  }
		  else{
		  break;
		  }
		  }
		  
		  }
		/*  if (ret<0)
		  {
		  AfxMessageBox(L"Communication Bad");
		  return;
		  }*/
		     ret= write_one(IntModbusID,7,ProductID);
		  if (ret>0)
		  {
			  TempNode.ComportNo=COMPort;
			  TempNode.ModbusID=ModbusID;
			  TempNode.ProductID=StrValue;
			  TempNode.RoomNO=RoomNo;
			  TempNode.SerialNO=SerialNo;
			  TempNode.Status=_T("Online");
			  SetNode(COMPort,ModbusID,TempNode);
			  PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,4);
			  m_detect_grid.put_TextMatrix(m_nCurRow,m_nCurCol,StrValue);
		  }
		  close_com();
	   }
	   else
	   {
	     AfxMessageBox(_T("Open comport ,Failed"));
	   }
	 }
	 else if (m_nCurCol==4)
	 {
		 int m_nSerialNumber = _wtoi(StrValue);

		 for (int i=0;i<m_allnodes.size();i++)
		 {
			 if(m_allnodes[i].ComportNo.CompareNoCase(COMPort)==0)
			 {
				 if(m_allnodes[i].Status.CompareNoCase(_T("Online"))==0)
				 {
					 if (m_allnodes[i].SerialNO.CompareNoCase(_T("StrValue"))==0)
					 {				    
						 AfxMessageBox(_T("The SrialNO is occupied"));
						 return ;
					 }
				 }
			 }
		 }
		 unsigned int loword,hiword;
		 loword=m_nSerialNumber & 0xffff;
		 hiword=(m_nSerialNumber >> 16) & 0xffff;

		 if (open_com(port))
		 {
// 			 Write_One(IntModbusID,16,142);
// 			 Sleep(12*1000);
			 if (write_one(IntModbusID,0,loword)>0 && write_one(IntModbusID,2,hiword)>0)
			 {
				 TempNode.ComportNo=COMPort;
				 TempNode.ModbusID=ModbusID;
				 TempNode.ProductID=ProductID;
				 TempNode.RoomNO=RoomNo;
				 TempNode.SerialNO=StrValue;
				 TempNode.Status=_T("Online");
				 SetNode(COMPort,ModbusID,TempNode);
				 PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,4);
				 m_detect_grid.put_TextMatrix(m_nCurRow,m_nCurCol,StrValue);

			 }
			 close_com();
		 }

	 }
	 else if(m_nCurCol==3)
	 {


		 for (int i=0;i<m_allnodes.size();i++)
		 {
			 if(m_allnodes[i].ComportNo.CompareNoCase(COMPort)==0)
			 {
				 if(m_allnodes[i].Status.CompareNoCase(_T("Online"))==0)
				 {
					 if (m_allnodes[i].RoomNO.CompareNoCase(_T("StrValue"))==0)
					 {				    
						 AfxMessageBox(_T("The Room NO is occupied"));
						 return ;
					 }
				 }
			 }
		 }


		 TempNode.ComportNo=COMPort;
		 TempNode.ModbusID=ModbusID;
		 TempNode.ProductID=ProductID;
		 TempNode.RoomNO=StrValue;
		 TempNode.SerialNO=SerialNo;
		 TempNode.Status=_T("Online");
		 SetNode(COMPort,ModbusID,TempNode);
		 PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,4);
		 m_detect_grid.put_TextMatrix(m_nCurRow,m_nCurCol,StrValue);
	 }
	 else if (m_nCurCol==6)
	 {
		 int m_nSerialNumber = _wtoi(StrValue);
		 if (open_com(port))
		 {
		             write_one(IntModbusID,401,0);
					 Sleep(500);
			int ret= write_one(IntModbusID,6,m_nSerialNumber);
					  
			 if (ret>0)
			 {
			     write_one(IntModbusID,401,1);
				 TempNode.ComportNo=COMPort;
				 TempNode.ModbusID=ModbusID;
				 TempNode.ProductID=ProductID;
				 TempNode.RoomNO=StrValue;
				 TempNode.SerialNO=SerialNo;
				 TempNode.Status=_T("Online");
				 TempNode.TstatID.Format(_T("%d"),m_nSerialNumber);
				 SetNode(COMPort,ModbusID,TempNode);
				 PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,4);
				 m_detect_grid.put_TextMatrix(m_nCurRow,m_nCurCol,StrValue);
				
			 }
			 close_com();
		 }
	 }
	 m_editor.ShowWindow(SW_HIDE);
}


void CDetectOnlineDlg::OnBnClickedOk()
{
 
}


void CDetectOnlineDlg::OnDestroy()
{
	
	m_abort=TRUE;
	Sleep(5000);
 
	if (m_pThread!=NULL)
	{

// 		if (WaitForSingleObject(m_pThread->m_hThread, 3000) == WAIT_OBJECT_0)
// 		{
// 
// 		}
// 		else
// 		{		
			BOOL bRet = TerminateThread(m_pThread->m_hThread,0);

			m_pThread = NULL;
		//}

	}
// 	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
// 	::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING,0,0);

	CDialogEx::OnDestroy();
	
}


void CDetectOnlineDlg::ClickMsflexgridResult2()
{
	long lRow,lCol;
	 
	lRow = m_allcomportTable.get_RowSel();//获取点击的行号	
	lCol = m_allcomportTable.get_ColSel(); //获取点击的列号
	if(lRow>m_allcomportTable.get_Rows()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow == 0) //如果点击标题行，也无效
		return;
	CRect rect;
	m_allcomportTable.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	// MSFlexGrid控件的函数的长度单位是"缇(twips)"，
	//需要将其转化为像素，1440缇= 1英寸
	CDC* pDC =GetDC();
	//计算象素点和缇的转换比例
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_allcomportTable.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_allcomportTable.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_allcomportTable.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_allcomportTable.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rc.OffsetRect(rect.left+1,rect.top+1);
	//获取选中格的文本信息
	CString strValue = m_allcomportTable.get_TextMatrix(lRow,lCol);
	//CString strStatus= m_allcomportTable.get_TextMatrix(lRow,5);
	m_nCurRow=lRow;
	m_nCurCol=lCol;
	//if (lCol==0||lCol==5)//COM ,Modbus ID,Room ,Serial No, Status
	//{
	//	return;
	//}
	// 	if (strStatus.CompareNoCase(_T("Online"))==0)
	// 	{
	m_edit_name.ShowWindow(FALSE);
	if (lCol==2||lCol==3||lCol==4)
	{
		m_edit_name.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_edit_name.ShowWindow(SW_SHOW);

		m_edit_name.BringWindowToTop();
		//m_RangCombox.SelectString(-1,strValue);
		m_edit_name.SetWindowText(strValue);
		m_edit_name.SetFocus(); //获取焦点


	}
	if (lCol==1)
	{

		m_combox_tf.ShowWindow(SW_SHOW);

		m_combox_tf.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_combox_tf.ShowWindow(SW_SHOW);
		m_combox_tf.BringWindowToTop();
		m_combox_tf.SetWindowText(strValue);
		m_combox_tf.SetFocus(); //获取焦点


	}
	/*	}*/
}


void CDetectOnlineDlg::OnCbnSelchangeComboTf()
{

    
    int sel=m_combox_tf.GetCurSel();
	if (sel==1)
	{
	m_allcomportTable.put_TextMatrix(m_nCurRow,m_nCurCol,_T("TRUE"));
	}
	else
	{
	m_allcomportTable.put_TextMatrix(m_nCurRow,m_nCurCol,_T("FALSE"));
	}

	m_combox_tf.ShowWindow(FALSE);
}


void CDetectOnlineDlg::OnEnSetfocusEdit2()
{	 
}


void CDetectOnlineDlg::OnEnKillfocusEdit2()
{
	CString StrTemp;
	m_edit_name.GetWindowTextW(StrTemp);
	m_allcomportTable.put_TextMatrix(m_nCurRow,m_nCurCol,StrTemp);
}


void CDetectOnlineDlg::OnBnClickedButtonScan()
{

    UpdateData(TRUE);
	 COMPORTNAME comnameTemp;
// 	 m_detect_grid.put_Rows(1);
// 	 m_detect_grid.put_Cols(9);
// 	 m_detect_grid.put_TextMatrix(0,0,_T("COM Port"));
// 	 m_detect_grid.put_TextMatrix(0,1,_T("Modbus ID"));
// 	 m_detect_grid.put_TextMatrix(0,2,_T("Product ID"));
// 	 m_detect_grid.put_TextMatrix(0,3,_T("Node Name"));
// 	 m_detect_grid.put_TextMatrix(0,4,_T("Serial No"));
// 	 m_detect_grid.put_TextMatrix(0,5,_T("Status"));
// 	 m_detect_grid.put_TextMatrix(0,6,_T("TstatID"));
// 	 m_detect_grid.put_TextMatrix(0,7,_T("Write Serial No"));
// 	 m_detect_grid.put_TextMatrix(0,8,_T("Firmware Version"));
	// m_detect_grid.put_TextMatrix(i+1,8,m_allnodes[i].Firmware);
	 m_scanport.clear();
	 for (int i=1;i<m_allcomportTable.get_Rows();i++)
	 {

	    if (m_allcomportTable.get_TextMatrix(i,1).CompareNoCase(_T("TRUE"))==0)
	    {
			comnameTemp.Name=m_allcomportTable.get_TextMatrix(i,2);
			comnameTemp.COMPORT=m_allcomportTable.get_TextMatrix(i,0);
			comnameTemp.LowID=m_allcomportTable.get_TextMatrix(i,3);
			comnameTemp.HighID=m_allcomportTable.get_TextMatrix(i,4);
			m_scanport.push_back(comnameTemp);
	    }

	 }
	 if (m_pThread!=NULL)
	 {
 
// 		 if (WaitForSingleObject(m_pThread->m_hThread, 3000) == WAIT_OBJECT_0)
// 		 {
// 
// 		 }
// 		 else
// 		 {		
			 BOOL bRet = TerminateThread(m_pThread->m_hThread,0);
		 
			  m_pThread = NULL;
		// }

	 }
	 m_pThread = AfxBeginThread(DetectThread, this);
	 if (m_pThread == NULL)
	 {
		 TRACE(_T("Failed to create download thread, dialog is aborting\n"));

	 }
// 	 m_pThread->m_bAutoDelete = FALSE;
// 	 m_pThread->ResumeThread();

	 //GetDlgItem(IDC_BUTTON_DETECT)->EnableWindow(FALSE);
	 //GetDlgItem(IDC_BUTTON_SCAN)->EnableWindow(FALSE);
	 GetDlgItem(IDC_BUTTON_DETECT)->EnableWindow(FALSE);
	 GetDlgItem(IDC_BUTTON_SCAN)->EnableWindow(FALSE);
	// GetDlgItem(IDC_BUTTON_REPAIRCONFLICT)->EnableWindow(FALSE);
				
}


void CDetectOnlineDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType==SIZE_RESTORED)
	{

		CRect ViewRect;
		GetClientRect(&ViewRect);
		//TRACE(_T(" View:T=%d,B=%d,L=%d,R=%d\n"),ViewRect.top,ViewRect.bottom,ViewRect.left,ViewRect.right);
		// m_MsDataGrid.SetWindowPos(this,ViewRect.top,ViewRect.left,ViewRect.Width(),ViewRect.Height(),SWP_SHOWWINDOW|SWP_NOZORDER);
		if (m_detect_grid.GetSafeHwnd())
		{
			m_detect_grid.MoveWindow(CRect(480,70,ViewRect.Width(),ViewRect.Height()),TRUE);
		}

	}
	
}


void CDetectOnlineDlg::OnBnClickedButton1()
{

}

int CDetectOnlineDlg::GetOneID(CString comport){
CString StrID;
int rowID;
int RetID;
BOOL is_Find=FALSE;
CString rowComport;
for (int ID=1;ID<255;ID++)
{
    
   for (int row=1;row<m_detect_grid.get_Rows();row++)
   {
      StrID=m_detect_grid.get_TextMatrix(row,1);
	  rowComport=m_detect_grid.get_TextMatrix(row,0);
	  rowID=_wtoi(StrID);
	  if ((rowID==ID)&&(rowComport.CompareNoCase(comport)==0))
	  {
		   is_Find=FALSE;
		   break;
	  }
	  else
	  {
	     is_Find=TRUE;
	  }
   }
   if (is_Find)
   {
   RetID=ID;
   break;
   }
}
return RetID;
}

void CDetectOnlineDlg::OnBnClickedButtonRepairconflict()
{
   
	GetDlgItem(IDC_BUTTON_DETECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SCAN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_REPAIRCONFLICT)->EnableWindow(FALSE);

	AfxBeginThread(RepairIDThread, this);
    
}

void CDetectOnlineDlg::RepairID(){
	CString StrStatus;
	CString StrModbusID;
	CString StrComPort;
	int ModbusID;
	int index;
	NODES_DETECTS nodetemp;
	for (int row=1;row<m_detect_grid.get_Rows();row++){
		StrStatus=m_detect_grid.get_TextMatrix(row,5);
		if (StrStatus.CompareNoCase(_T("Online"))==0)
		{
			continue;
		}

		if (StrStatus.CompareNoCase(_T("Conflict"))==0)
		{
			StrComPort=m_detect_grid.get_TextMatrix(row,0);
			StrModbusID=m_detect_grid.get_TextMatrix(row,1);
			ModbusID=_wtoi(StrModbusID);


			for (int i=0;i<m_scanport.size();i++)
			{
				if (StrComPort.CompareNoCase(m_scanport[i].COMPORT)==0)
				{
					index=i;
				}
			}
			CString tc = StrComPort.Mid(3);
			int n = _wtoi(tc);
			SetCommunicationType(0);
			if (open_com(n))
			{
				Change_BaudRate(19200);
				int scanback=-1;
				int ncount=0;
				while (scanback<0)
				{
					ncount++;
					if (ncount>254)
					{
						break;
					}
					unsigned short SerialNum[9];
					memset(SerialNum,0,sizeof(SerialNum));
					int nRet=0;
					scanback=CheckTstatOnline_a(ModbusID,ModbusID, 0);
					if (scanback>0)
					{
						nRet=read_multi2(ModbusID,&SerialNum[0],0,9,0);
						if(nRet>0)
						{

							nodetemp.ComportNo.Format(_T("COM%d"),n);
							nodetemp.ModbusID.Format(_T("%d"),SerialNum[6]);
							nodetemp.ProductID.Format(_T("%d"),SerialNum[7]);
							nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,SerialNum[6]);
							nodetemp.SerialNO.Format(_T("%d"),SerialNum[0],SerialNum[1]*256,SerialNum[2]*256*256,SerialNum[3]*256*256*256);

							nodetemp.Brandrate=_T("19200");
							nodetemp.Status=_T("Online");
							BOOL Repeated=FALSE;
							for (int i=0;i<m_allnodes.size();i++)
							{
								if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
								{
									Repeated=TRUE;
									break;
								}
							}
							if (!Repeated)
							{
								m_allnodes.push_back(nodetemp);

								PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
							}

						}
						close_com();
						break;
					}
					if(read_one(ModbusID,10,5)>=0)
					{
						int j=  GetOneID(StrComPort);
						if(write_one(ModbusID,10,j,5)>0){ 
							m_tips.Format(_T("COM%d  Repair ID:%d To :%d"),n,ModbusID,j);
							SendMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,1);


							nRet=read_multi2(j,&SerialNum[0],0,9,0);
							if(nRet>0)
							{
							   
								nodetemp.ComportNo.Format(_T("COM%d"),n);
								nodetemp.ModbusID.Format(_T("%d"),SerialNum[6]);
								nodetemp.ProductID.Format(_T("%d"),SerialNum[7]);
								nodetemp.RoomNO.Format(_T("%s%d"),m_scanport[index].Name,SerialNum[6]);
								nodetemp.SerialNO.Format(_T("%d"),SerialNum[0],SerialNum[1]*256,SerialNum[2]*256*256,SerialNum[3]*256*256*256);

								nodetemp.Brandrate=_T("19200");
								nodetemp.Status=_T("Online");
								BOOL Repeated=FALSE;
								for (int i=0;i<m_allnodes.size();i++)
								{
									if ((nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ComportNo)==0)&&(nodetemp.ModbusID.CompareNoCase(m_allnodes[i].ModbusID)==0))
									{
										Repeated=TRUE;
										break;
									}
								}
								if (!Repeated)
								{
									m_allnodes.push_back(nodetemp);

									PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,2);
								}

							}
							else
								return;
						}
					}   
				}


				close_com();
			}

		}
	}


	//PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,3);
	//OnBnClickedButtonScan();
	//PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,4);
}

void CDetectOnlineDlg::OnBnClickedButtonWriteSerialNo()
{
	NODES_DETECTS TempNode;
	CString StrValue;
	CString COMPort=m_detect_grid.get_TextMatrix(m_nCurRow,0);
	CString ModbusID=m_detect_grid.get_TextMatrix(m_nCurRow,1);
	CString ProductID=m_detect_grid.get_TextMatrix(m_nCurRow,2);
	CString RoomNo=m_detect_grid.get_TextMatrix(m_nCurRow,3);
	CString SerialNo=m_detect_grid.get_TextMatrix(m_nCurRow,4);

	int IntModbusID=_wtoi(ModbusID.GetBuffer());

	m_edit_serialNo.GetWindowTextW(StrValue);
	if (StrValue.IsEmpty())
	{
	  AfxMessageBox(_T("Input the start of serial number!"));
	  return;
	}
	
	int m_nSerialNumber = _wtoi(StrValue);

     BOOL isrepeat=FALSE;

	while(1){
	              isrepeat=FALSE;
				StrValue.Format(_T("%d"),m_nSerialNumber);
				for (int i=0;i<m_allnodes.size();i++){
						if (m_allnodes[i].SerialNO.CompareNoCase(StrValue)==0){
						   isrepeat = TRUE;
							break ;
						}
					}
					if (isrepeat)
					{
					++m_nSerialNumber;
					}
					else
					{
					   break;
					}
	}


	unsigned int loword,hiword;
	loword=m_nSerialNumber & 0xffff;
	hiword=(m_nSerialNumber >> 16) & 0xffff;
	 int port=_wtoi(COMPort.Mid(3));
	if (open_com(port))
	{
// 		Write_One(IntModbusID,16,142);
// 		Sleep(15*1000);
		if (write_one(IntModbusID,0,loword)>0 && write_one(IntModbusID,2,hiword)>0)
		{
		    write_one(IntModbusID,7,18);
			TempNode.ComportNo=COMPort;
			TempNode.ModbusID=ModbusID;
			TempNode.ProductID=ProductID;
			TempNode.RoomNO=RoomNo;
			TempNode.SerialNO=StrValue;
			TempNode.Status=_T("Online");
			SetNode(COMPort,ModbusID,TempNode);
			PostMessage(WM_HTTPDOWNLOAD_THREAD_FINISHED,4);
			m_detect_grid.put_TextMatrix(m_nCurRow,4,StrValue);


			CStdioFile default_file;
			CString saved_path=g_strExePth+_T("Serial.txt");
			CString a_line;
			if(default_file.Open(saved_path.GetString(),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
			{
				   default_file.SeekToEnd();
				 
					a_line.Format(_T("%s:%s:%s:%s:%s:%s\n"),m_detect_grid.get_TextMatrix(m_nCurRow,0),
						m_detect_grid.get_TextMatrix(m_nCurRow,1),
						m_detect_grid.get_TextMatrix(m_nCurRow,2),
						m_detect_grid.get_TextMatrix(m_nCurRow,3),
						m_detect_grid.get_TextMatrix(m_nCurRow,4),
						m_detect_grid.get_TextMatrix(m_nCurRow,5));
					    default_file.WriteString(a_line);
				default_file.Flush();
				default_file.Close();
			}


			AfxMessageBox(_T("Write OK"));
		}
		else
		{
		  AfxMessageBox(_T("Write Fail"));
		}
		close_com();
	}
}


//void CDetectOnlineDlg::OnEnChangeEdit2()
//{
//	
//	// send this notification unless you override the CDialogEx::OnInitDialog()
//	// function and call CRichEditCtrl().SetEventMask()
//	// with the ENM_CHANGE flag ORed into the mask.
//
//	// 
//}


BOOL CDetectOnlineDlg::PreTranslateMessage(MSG* pMsg)
{
	
	if(WM_KEYDOWN == pMsg->message)
	{

		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);
		ASSERT(pEdit);
		if(pMsg->hwnd == pEdit->GetSafeHwnd() && VK_RETURN == pMsg->wParam)
		{
			OnEnKillfocusEdit1();
        }
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDetectOnlineDlg::OnBnClickedCheckOneByOne()
{
	 m_scan_type=TRUE;
	 Update_Check();

}


void CDetectOnlineDlg::OnBnClickedCheckBinaryScan()
{
	m_scan_type=FALSE;
	Update_Check();
}
void CDetectOnlineDlg::Update_Check(){
if (m_scan_type)
{
  m_check_onebyone.SetCheck(TRUE);
  m_check_binaryScan.SetCheck(FALSE);
}
else
{
	m_check_onebyone.SetCheck(FALSE);
	m_check_binaryScan.SetCheck(TRUE);
}
}

void CDetectOnlineDlg::OnBnClickedButtonClear()
{
 
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3Table table;
	CppSQLite3Query q;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
	CString temptable = _T("DetectTstatOnline");
	CString createtable = _T("create table DetectTstatOnline(COM text,ModbusID text,ProductID text,RoomNo text,SerialNo text,Status text,TStatID text,FirmwareVer text)");//
	CString strSql ;
	bool m_judge = SqliteDBT3000.tableExists("DetectTstatOnline");
	if (!m_judge)
	{
		SqliteDBT3000.execDML((UTF8MBSTR)createtable);
	}
	strSql=_T("delete   from DetectTstatOnline ");
	try
	{
		 	SqliteDBT3000.execDML((UTF8MBSTR)strSql);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(_T("Error"));
		return;
	}
	SqliteDBT3000.closedb();
	//清buffer
	m_allnodes.clear();
	//清grid
	m_detect_grid.put_Rows(1);
	m_detect_grid.put_Cols(9);
	m_detect_grid.put_TextMatrix(0,0,_T("COM Port"));
	m_detect_grid.put_TextMatrix(0,1,_T("Modbus ID"));
	m_detect_grid.put_TextMatrix(0,2,_T("Product ID"));
	m_detect_grid.put_TextMatrix(0,3,_T("Node Name"));
	m_detect_grid.put_TextMatrix(0,4,_T("Serial No"));
	m_detect_grid.put_TextMatrix(0,5,_T("Status"));
	m_detect_grid.put_TextMatrix(0,6,_T("TstatID"));
	m_detect_grid.put_TextMatrix(0,7,_T("Write Serial No"));
	m_detect_grid.put_TextMatrix(0,8,_T("Firmware Version"));
}
