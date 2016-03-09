#if !defined(AFX_TARGETGRID_H__5F71A8F3_8820_4A80_9CC0_F478C9AAB812__INCLUDED_)
#define AFX_TARGETGRID_H__5F71A8F3_8820_4A80_9CC0_F478C9AAB812__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TargetGrid.h : header file
//
#include "MSFlexGrid1.h"
#include "IniFile.h"


 

/////////////////////////////////////////////////////////////////////////////
// CTemcoDataGrid window

class CTemcoDataGrid : public CMsflexgrid
{
// Construction
public:
	CTemcoDataGrid();
	void Set_ParentWind(CWnd* pParent);
// Attributes
public:
CWnd* m_pParent;
// Operations

public:
   
	void Initialize();
	CStatic  *m_ColName;
	CString m_CurColName;
	int m_SelCurCol;
	int m_SelCurRow;
	CString m_Section;
	int m_Cols;
	int m_Rows;
	CIniFile m_Inifile;
	int STATIC_ID;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTemcoDataGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTemcoDataGrid();

protected:
 
	void v_ClearCell(int i_row, int i_col, BOOL b_informSource=FALSE);
 
    //void Write_GridCol();
	// Generated message map functions
protected:
	//{{AFX_MSG(CTemcoDataGrid)
	afx_msg void OnClearCell();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg BOOL OnKeyUp(short FAR* KeyCode, short Shift);
	
	afx_msg BOOL OnMouseUp(short Button, short Shift, long x, long y);
	afx_msg BOOL OnMouseMove(short Button, short Shift, long x, long y);

	DECLARE_EVENTSINK_MAP()



	
public:
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TARGETGRID_H__5F71A8F3_8820_4A80_9CC0_F478C9AAB812__INCLUDED_)
