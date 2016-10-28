#pragma once

#include "msflexgrid1.h"
 
 
#include "afxwin.h"
struct NODES_DETECTS 
{
CString ComportNo;
CString TstatID;
CString ModbusID;
CString ProductID;
CString RoomNO;
CString SerialNO;
CString Status;
CString Brandrate;
CString Firmware;

 //
 // bool operator==(const NODES_DETECTS& lhs, const NODES_DETECTS& rhs)
 //{
 //	if (lhs.SerialNO.CompareNoCase(rhs.SerialNO)==0)
 //	{
 //	return true;
 //	}
 //	else
 //		return false;
 //}
  bool operator==(const NODES_DETECTS &t1)const{
	  if (SerialNO.CompareNoCase(t1.SerialNO)==0)
	  {
		  return true;
	  }
	  else
		  return false;
  } 
  bool operator<(const NODES_DETECTS &t1)const{
	  if (SerialNO.Compare(t1.SerialNO)<0)
	  {
		  return true;
	  }
	  else
		  return false;
  } 
};




struct COMPORTNAME{
CString COMPORT;
CString Name;
CString LowID;
CString HighID;
};


class CDetectOnlineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDetectOnlineDlg)

public:
	CDetectOnlineDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDetectOnlineDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DETECTONLINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	UINT m_minID;
	UINT m_maxID;
	CString m_tips;
	vector<NODES_DETECTS> m_allnodes;
	 
	vector<COMPORTNAME>  m_scanport;
	vector<CString> m_comports;
	static UINT DetectThread(LPVOID pParam);
	static UINT RepairIDThread(LPVOID pParam);
	afx_msg void OnBnClickedButtonDetect();
	int vector_unique(vector<NODES_DETECTS> &vt);
	void GetAllNodes();
	afx_msg LRESULT OnThreadFinished(WPARAM wParam, LPARAM lParam);
	 void FreshGrid();
	 void GetNode(CString COM,int ID,NODES_DETECTS &nodes);
	 void SetNode(CString COM,CString ID,NODES_DETECTS nodeValue);
	 void SaveToDB();
	 void Update_Check();
	 CWinThread*   m_pThread;
	 CMsflexgrid m_detect_grid;
	 virtual BOOL OnInitDialog();
	 DECLARE_EVENTSINK_MAP()
	 void ClickMsflexgridResult();
	 CEdit m_editor;
	 int m_nCurRow;
	 int m_nCurCol;
	 BOOL m_abort;
	 void FLEX_GRID1_PUT_STR(int ,int);
	   void OnEnKillfocusEdit1();
	 afx_msg void OnBnClickedOk();
	 afx_msg void OnDestroy();
	// afx_msg void OnBnClickedButton1();
	 CComboBox m_combox_tf;
	 CEdit m_edit_name;
	 CMsflexgrid m_allcomportTable;
	 void ClickMsflexgridResult2();
	 afx_msg void OnCbnSelchangeComboTf();
	 afx_msg void OnEnSetfocusEdit2();
	 afx_msg void OnEnKillfocusEdit2();
	 afx_msg void OnBnClickedButtonScan();
	 BOOL	binary_search_crc(int a);
	void binarySearchforComDevice(int nComPort, bool bForTStat, BYTE devLo, BYTE devHi,int index);
	CEdit m_edit_serialNo;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonRepairconflict();
	void RepairID();
	int GetOneID(CString comport);
	CButton m_writesnBtn;
	afx_msg void OnBnClickedButtonWriteSerialNo();
//	afx_msg void OnEnChangeEdit2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCheckOneByOne();
	afx_msg void OnBnClickedCheckBinaryScan();
	UINT m_scan_times;
	BOOL m_scan_type;
 
	CButton m_check_onebyone;
	CButton m_check_binaryScan;
	afx_msg void OnBnClickedButtonClear();
};
