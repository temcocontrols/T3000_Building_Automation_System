#if !defined(AFX_SLIDERBOTH_H__DDB3B9E1_263F_11DB_ACFE_0050BA012D9B__INCLUDED_)
#define AFX_SLIDERBOTH_H__DDB3B9E1_263F_11DB_ACFE_0050BA012D9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SliderBoth.h : header file
//

/////////////////////////////////////////////////////////////////////////////

#include "SliderBothBtn.h"

class SliderBoth : public CWnd
{
// Construction
public:
	SliderBoth();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SliderBoth)
	protected:
	virtual LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Create(CWnd*pMum,CRect rcBd,BOOL bHorz,int iID);
	BOOL SetRange(int iMin,int iMax,int iTick);
	void GetRange(int&iMin,int&iMax,int&iTick);
	void GetPosition(int&iL,int&iR);
	void SetPosition(int iL,int iR);

	void MoveWindow(CRect*pRect,BOOL bRepaint=TRUE);
	void MoveWindow(int iX,int iY,int iW,int iH,BOOL bRepaint=TRUE);
	virtual ~SliderBoth();

	// Generated message map functions
protected:
	//{{AFX_MSG(SliderBoth)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType,int cx,int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void P_SetPosition_Vert(int iL,int iR,BOOL bFst,BOOL bSnd);
	void P_SetPosition_Horz(int iL,int iR,BOOL bFst,BOOL bSnd);
	void P_SetPosition(int iL,int iR,BOOL bFst,BOOL bSnd);
	void P_PaintHorz(CDC*pDC,const CRect&rcMe);

	struct ThePos
	{
		void Init(int iMin0,int iMax0,int iTick0)
		{
			iL=iOldL=iMin=iMin0;
			iR=iOldR=iMax=iMax0;
			iTick=iTick0;
		}
		int iMin,iMax,iTick;
		int iL,iR,iOldL,iOldR;
	}pos;

	//for draw and move;
	int i_SliderY;
	int i_ID;
	BYTE b_Horz,b_Initialized;
	SliderBothBtn tic0,tic1;
	SliderBothBtn tic2,tic3;//LSC

public:
	void SetInstane(int minval,int maxval);
	void SetBlockColor();
private:
	int m_min,m_max;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLIDERBOTH_H__DDB3B9E1_263F_11DB_ACFE_0050BA012D9B__INCLUDED_)
