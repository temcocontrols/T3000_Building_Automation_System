
// Modbus PollView.h : interface of the CModbusPollView class
//

#pragma once

#include "resource.h"
 
#include "afxwin.h"
#include "Modbus PollDoc.h"

#include "global_variable_extern.h"
#include "global_Function.h"
#include "ModbusDllforVC.h"
#include "global_struct.h"
#include "AutoRichEditCtrl.h"
#include "excel9.h"
#include "GridCtrl_src/GridCtrl.h"

class CModbusPollView : public CFormView
{
protected: // create from serialization only
	CModbusPollView();
	DECLARE_DYNCREATE(CModbusPollView)

public:
	enum{ IDD = IDD_MODBUSPOLL_FORM };

// Attributes
public:
	CModbusPollDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CModbusPollView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
	
	
private:
	CGridCtrl m_MsDataGrid;
	CAutoRichEditCtrl m_connectionState;

public:
  CAutoRichEditCtrl m_Tx_Rx;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//Custom functions
public:
   void Fresh_View();
   void Fresh_Data();
   //Give subscript, return data string
   CString Get_Data(int index);
   CString Get_Data_No_Address(int index);

   int Get_Reg_Add(int index);
   DECLARE_EVENTSINK_MAP()
 
 
 
    void Initial_RegName(); 
	CString Find_RegName(int index);
	struct DBRegister 
	{
		int RegAddress;
		CString RegName;
// 		CString DataType;
// 		int length;
// 		CString Description;
// 		CString Operation;
	};
	vector<DBRegister> m_VecregisterData;

public:
	int m_Slave_ID;
	int m_Function;
	int m_address;
	int m_Quantity;
	int m_Scan_Rate;
	int m_Rows;
	int m_Display;
	int m_Hide_Alias_Columns;
	int m_Address_Cell;
	int m_PLC_Addresses;

	BOOL m_apply;
	BOOL m_wronce;
	CStdioFile m_default_file;
	////////////////
    LONG64 m_Tx;
    LONG64 m_Err;
	//////////////////////////////////////////////////////////////////////////
	int m_MBPoll_Function;
	//////////////////////////////////////////////////////////////////////////
	int m_data_rows;
	int m_data_cols;
	int m_grid_rows;
	int m_grid_cols;

	int m_Current_Row;
	int m_Current_Col;

	BOOL m_close_dlg;
	BOOL m_function;
	BOOL m_logText;
	CString m_logTextPath;
	BOOL m_logExcel;
	/////////////////////////DataBuffer/////////////////////////////////////////
	unsigned short m_DataBuffer[127];
    CString m_Alias[127];
 
 
	BOOL Show_Name;
 
	virtual BOOL DestroyWindow();
	//afx_msg void OnViewRegistervalueanalyzer();
	afx_msg void OnEditAdd();
	COLORREF Get_Color(int i);
	CString Get_Now();

	BOOL m_isgetmodel;
	BOOL m_ischangedAddress;
	BOOL m_isnewmodel;
	BOOL m_ischangeModelName;
	CString m_modelname;
	unsigned short m_modeldata[2];
	CAutoRichEditCtrl m_ModelNameRichEdit;
	CAutoRichEditCtrl m_SlaveIDRichEditCtrl;
	CAutoRichEditCtrl m_modelib;
	int m_MultiReadReturnType;

	int m_cur_modelNo;
	CString m_cur_modelName;
	CString m_cur_TableName;
	CString m_cur_Col_RegName;
	CString m_cur_col_RegAddress;
	afx_msg void OnSetupTxtLog();
	afx_msg void OnSetupTxtlogoutOff();
	afx_msg void OnUpdateSetupTxtlogoutOff(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSetupTxtLog(CCmdUI *pCmdUI);
	afx_msg void OnSetupExcellogingoff();
	afx_msg void OnUpdateSetupExcellogingoff(CCmdUI *pCmdUI);
	afx_msg void OnSetupExcelloging();
	afx_msg void OnUpdateSetupExcelloging(CCmdUI *pCmdUI);
	afx_msg void OnEnKillfocusModelname();
	afx_msg void OnEditChangemodelname();




	_Application m_ExcelApp; 
	Workbooks m_wbsMyBooks; 
	_Workbook m_wbMyBook; 
	Worksheets m_wssMysheets; 
	_Worksheet m_wsMysheet; 
	Range m_rgMyRge;

	int m_curexcelrow;

	afx_msg void OnFunctionsTestcenter();
/*	void MouseMoveMsflexgrid1(short Button, short Shift, long x, long y);*/
	afx_msg void OnSetupUseasdefault();
	afx_msg void OnFunctionsTestwrite();
	 
	afx_msg void OnEditCopy();
	afx_msg void OnFilePrint32857();


	afx_msg void OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

};

#ifndef _DEBUG  // debug version in Modbus PollView.cpp
inline CModbusPollDoc* CModbusPollView::GetDocument() const
   { return reinterpret_cast<CModbusPollDoc*>(m_pDocument); }
#endif

