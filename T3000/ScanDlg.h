#pragma once
//#include "afxwin.h"
#include "msflexgrid1.h"
#include "resource.h"
#include "TStatScanner.h"
#include "afxwin.h"

#define WM_SCANFINISH WM_USER + 1100
#define WM_ADDCOMSCAN WM_USER + 1101
#define WM_ADDNETSCAN WM_USER + 1102
#define ICMP_ECHO 8 
#define ICMP_ECHOREPLY 0 
#define ICMP_MIN 8 // minimum 8 byte icmp packet (just header) 

typedef struct iphdr { 
	//	unsigned int h_len:4; // length of the header 
	//	unsigned int version:4; // Version of IP 
	BYTE  h_len:4;
	BYTE  ver:4;
	unsigned char tos; // Type of service 
	unsigned short total_len; // total length of the packet 
	unsigned short ident; // unique identifier 
	unsigned short frag_and_flags; // flags 
	unsigned char ttl; 
	unsigned char proto; // protocol (TCP, UDP etc) 
	unsigned short checksum; // IP checksum 
	unsigned int sourceIP; 
	unsigned int destIP; 
}IpHeader; 
// CScanDlg dialog
class CTStat_Dev;
class CTStat_Net;
class CTStatBase;
//class CTStatScanner;
class CGridButton;
#define xmalloc(s) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,(s)) 
#define xfree(p) HeapFree (GetProcessHeap(),0,(p)) 

// struct ALL_LOCAL_SUBNET_NODE{
// CString StrIP;
// CString StrMask;
// CString StrGetway;
// int NetworkCardType;
// };
class CScanDlg : public CDialog
{
	DECLARE_DYNAMIC(CScanDlg)
	
public:
	CScanDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScanDlg();
	CMsflexgrid	m_flexGrid;
// Dialog Data
	enum { IDD = IDD_DLG_SCANALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()


	//////////////////////////////////////////////////////////////////////////
public:
	afx_msg void OnBnClickedButtonScanall();
	afx_msg void OnBnClickedButtonAuto();
	afx_msg void OnBnClickedButtonManual();
	afx_msg void OnBnClickedButtonExit();
	afx_msg LRESULT OnScanFinish(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddComScanRet(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddNetScanRet(WPARAM wParam, LPARAM lParam);

BOOL	CheckTheSameSubnet(CString strIP);
BOOL    ChangeNetDeviceIP(CString strIP,int row_flags);
void FLEX_GRID_PUT_COLOR_STR(int row,int col,CString str);
COLORREF exsit_ref;
COLORREF new_ref;
bool exsit_serial;
void SetExsit(bool ref_exsit);
void GetIPMaskGetWay(vector<ALL_LOCAL_SUBNET_NODE>  &Vector_Subnet);
void GetIPMaskGetWay(CString &StrIP,CString &StrMask,CString &StrGetway);
BOOL GetNewIP(CString &newIP);
BOOL GetNewIP(CString &newIP,CString BaseIP);
BOOL TestPing(const CString& strIP);
void FillIcmpData(char * icmp_data, int datasize);
USHORT Checksum(USHORT *buffer, int size);
int DecodeResp(char *buf, int bytes,struct sockaddr_in *from);
void ChangeIPAddress(CString newip,CString oldip);
void SaveNewIPAddress(CString newip,CString oldip);
public:
	void InitScanGrid();

	void AddComDeviceToGrid(vector<_ComDeviceInfo*>& szList);
//	void AddNetDeviceToGrid(vector<_NetDeviceInfo*>& szList);
	void AddNetDeviceToGrid();

	void SetScanner(CTStatScanner* pScanner);
	void SetNode(tree_product product_Node);
	void Set_IsScan(BOOL Is_Scan);
	void Release();
protected:
	// 读出所有存在数据库中的设备节点。准备与搜索到的设备比较
	int GetAllNodeFromDataBase();
	void SetGridCellColor(int nRow, int nCol, COLORREF clr);
 

	tree_product m_net_product_node;
	BOOL m_IsScan;
	
	//void DrawButtonOnGrid(int iRow, int iCol);

	CTStat_Dev* FindComDeviceBySerialIDInDB(DWORD dwSerialID);
	CTStatBase* FindDeviceByRowNum( int nRow );
	CTStat_Net* FindNetDeviceBySerialIDInDB(DWORD dwSerialID);

	// destroy
	void DestroyFlexEdit();

	// 显示edit
	void ShowInputEditBox(int iRow, int iCol);
	
	// 计算gridcell rect, 根据行列号
	CRect CalcGridCellRect(int iRow, int iCol );	

	// 把一个tstat写成一条DB记录
	void WriteOneDevInfoToDB( CTStat_Dev* pDev);
	void WriteOneNetInfoToDB( CTStat_Net* pNet);

	// 只允许写 buildingname，floorname，roomname，address
	// 因此，需要向寄存器写的只有address
	void GetDataFromGrid();
	void SaveAllNodeToDB();
	void AutoFixComConflict();

	//////////////////////////////////////////////////////////////////////////
	//
	//////////////////////////////////////////////////////////////////////////
	typedef struct icmphdr { 
		BYTE i_type; 
		BYTE i_code; /* type sub code */ 
		USHORT i_cksum; 
		USHORT i_id; 
		USHORT i_seq; 
		/* This is not the std header, but we reserve space for time */ 
		ULONG timestamp; 
	}IcmpHeader; 
protected:
	void		CalcClickPos(CSize& size);
	void		GetGridEditString(); // 记录edit的数据
	int			IsValidClick(CSize szTemp);	
	void		CombineDBandScanRet();


protected:


	CTStatScanner*			m_pScanner;

	vector<CTStat_Net*>	m_szNetNodes;
	vector<CTStat_Dev*>	m_szComNodes;

	vector<CTStat_Net*>	m_szNCScanRet;
	vector<CTStat_Dev*>	m_szTSScanRet;

	//vector<CGridButton*>		m_szGridBtns;

	//CEdit*							m_pflexEdit;
	CRITICAL_SECTION			m_csGrid;

	CSize								m_szGridEditPos;

 
	
public:
	afx_msg void OnClose();
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgrid1();
	CEdit m_editGrid;
	int m_curcol;
	int m_currow;

	CString m_strlocalipaddress;
	CString m_strlocalsubnet;
	CString m_strlocalgateway;
	afx_msg void OnEnKillfocusEditGridedit();



	vector<ALL_LOCAL_SUBNET_NODE> allsubnets;
};
