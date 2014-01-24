#pragma once
#include "msflexgrid1.h"
#include "IniFile.h"
#include<vector>
#include "afxwin.h"
using namespace std;
#include "RegisterWriteValueDlg.h"

// CRegisterViewerDlg dialog

class CRegisterViewerDlg : public CDialog
{
	DECLARE_DYNAMIC(CRegisterViewerDlg)

public:
	CRegisterViewerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRegisterViewerDlg();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_REGISTER_VIEWER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
void LoadDataFromDB();
void ShowCurPage();
void Fresh_GridCol();
void Write_GridCol();
CString Get_ProductModel();
public:
	CMsflexgrid m_registerlistGrid;
	CString m_oldname;
	CWinThread* pParamBackFresh;
	struct DBRegister 
	{
		int Register_Address;
		CString AddressName;
		CString DataType;
		int length;
		CString Description;
		CString Operation;
	};
	vector<DBRegister> m_VecregisterData;
	int m_recordcount;
	int m_Datapages;
	int m_curPage;
	BOOL m_CanEdit;
	int m_modelno;
	afx_msg void OnBnClickedLeft();
	afx_msg void OnBnClickedRight();
	afx_msg void OnBnClickedBegin();
	afx_msg void OnBnClickedEnd();
	DECLARE_EVENTSINK_MAP()
	void MouseDownInputMsflexgrid(short Button, short Shift, long x, long y);
private:
   int m_SelCurCol;
   int m_SelCurRow;
   CString m_CurColName;
   CIniFile m_Inifile;
   int m_regno;
	   int m_value;
	   int m_name;
	   int m_type;
	   int m_length;
	   int m_description;
	   int m_Operation;
	   CRegisterWriteValueDlg m_WriteValueDlg;
public:
	void MouseMoveInputMsflexgrid(short Button, short Shift, long x, long y);
	CStatic m_ColName;
	void MouseUpInputMsflexgrid(short Button, short Shift, long x, long y);
//	void RowColChangeInputMsflexgrid();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnBnClickedAddnewproduct();
	afx_msg void OnMainNew();
	void ClickInputMsflexgrid();
//	void ScrollInputMsflexgrid();
//	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
//	void ScrollInputMsflexgrid();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	CEdit m_inNameEdt;
	afx_msg void OnEnKillfocusCelledit();
	void DblClickInputMsflexgrid();
	afx_msg void OnBnClickedSavetodb();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	void Show_ColorData();
};
