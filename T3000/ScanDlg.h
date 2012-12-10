#pragma once
//#include "afxwin.h"
#include "msflexgrid1.h"
#include "resource.h"
#include "TStatScanner.h"
#include "afxwin.h"

#define WM_SCANFINISH WM_USER + 1100
#define WM_ADDCOMSCAN WM_USER + 1101
#define WM_ADDNETSCAN WM_USER + 1102



// CScanDlg dialog
class CTStat_Dev;
class CTStat_Net;
class CTStatBase;
//class CTStatScanner;
class CGridButton;


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

public:
	void InitScanGrid();

	void AddComDeviceToGrid(vector<_ComDeviceInfo*>& szList);
	void AddNetDeviceToGrid(vector<_NetDeviceInfo*>& szList);

	void SetScanner(CTStatScanner* pScanner);
	void Release();
protected:
	// 读出所有存在数据库中的设备节点。准备与搜索到的设备比较
	int GetAllNodeFromDataBase();
	void SetGridCellColor(int nRow, int nCol, COLORREF clr);

	// 打开原来的端口
	void OpenDefaultCom();

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

	_RecordsetPtr m_pRs;
	_ConnectionPtr m_pCon;
	
public:
	afx_msg void OnClose();
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgrid1();
	CEdit m_editGrid;
};
