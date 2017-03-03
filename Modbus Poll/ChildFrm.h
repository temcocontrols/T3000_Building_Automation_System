
// ChildFrm.h : interface of the CChildFrame class
//

#pragma once
#include "Modbus PollView.h"
class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetupRead();
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
	int m_PLC_Adresses;

	BOOL m_apply;
	BOOL m_wronce;

public:
	afx_msg void OnSetupRead32783();
//	afx_msg void OnDispalyCommunication();
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void Show_Data_Traffic_Window();
	afx_msg void OnDispalySigned();
	afx_msg void OnDispalyUnsigned();
	afx_msg void OnDispalyHex();
	afx_msg void OnDispalyLong();
	afx_msg void OnDispalyLonginverse();
	afx_msg void OnDispalyFloatinverse();
	afx_msg void OnDispalyFloat();
	afx_msg void OnDispalyDoubleinverse();
	afx_msg void OnDispalyDouble();
	afx_msg void OnDispalyBinary();
	afx_msg void OnUpdateDispalyBinary(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDispalyDouble(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDispalyDoubleinverse(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDispalyFloat(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDispalyFloatinverse(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDispalyHex(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDispalyLong(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDispalyLonginverse(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDispalySigned(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDispalyUnsigned(CCmdUI *pCmdUI);
	afx_msg void OnDispalyPlcaddress();
	afx_msg void OnDispalyProtocoladdresses();
	afx_msg void OnUpdateDispalyPlcaddress(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDispalyProtocoladdresses(CCmdUI *pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFunctions06();
	afx_msg void OnFunctions16();
	afx_msg void OnFunctions23();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnFunctions05();
	afx_msg void OnFunctions15();
};
