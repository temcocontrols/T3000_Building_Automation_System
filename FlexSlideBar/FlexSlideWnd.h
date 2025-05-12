#pragma once

#include "FSBThumb.h"
#include "FSBChannel.h"
//#include "ThumbManager.h"
#include "ThumbOperator.h"

//#include "..\..\TestFSB\TestFSBDlg.h"

#include <vector>
using namespace std;

// CFlexSlideWnd

// typedef void (*pCallBackFunc)();
// typedef void (CFlexSlideWnd::pCallBackFunc)(void)

#define WM_USER_CALLBACK WM_USER + 1001

class CTestFSBDlg;
class CFSBContainer;
class CFlexSlideWnd : public CStatic
{
	DECLARE_DYNAMIC(CFlexSlideWnd)

public:
	CFlexSlideWnd();
	virtual ~CFlexSlideWnd();

	
protected:
	DECLARE_MESSAGE_MAP()



public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);


//////////////////////////////////////////////////////////////////////////

public:

	void SetFSBStyle(FSB_STYLE fsbStyle);
	FSB_STYLE GetFSBStyle();

	void SetFSBThumbShape(FSB_THUMBSHAPE fsbTbShape);
	
	// 设置控件的位置
	void SetFlexSlideBarRect(CRect& rc);	
	CRect& GetFlexSlideBarRect();

	// 设置channel的宽度，不得大于控件宽度，不小于1个象素
	// channel 长度应该依据控件长度来计算获得
	void SetChannelWidth(int nChannelWidth);
	int GetChannelWidth();

	// 设置Channel的颜色
	COLORREF GetChannelColor();					
	void SetChannelColor(COLORREF clrChannel);

	// 设置Thumb的长度，不小于channel的宽度，不大于控件的宽度，thumb的宽度为固定值
	int GetThumbWidth();					
	void SetThumbWidth(int nThumbLen);

	// 设置Thumb的颜色
	COLORREF GetThumbColor()const;					
	void SetThumbColor(COLORREF clrThumb);

	//Sets the maximum position for a slider. 
	void	SetRangeMax(int nMax);
	void	SetRangeMin(int nMin);
	
	// 控件是否水平放置
	void SetHorizontal(BOOL bHorizontal);
	BOOL IsHorizontal();
	

	void SetPageLength(int nPageLength);

	BOOL CreateFlexSlideBar(
		BOOL bHorizontal,	
		FSB_STYLE style=FSB_STYLE_SINGLETHUMB, 
		int nChannelWidth=10, 
		int nThumbWidth=12, 
		FSB_THUMBSHAPE tbShape=FSB_THUMBSHAPE_RECT									   
		);


	//Sets the minimum and maximum positions for a slider. 
	void SetRange(int nMax, int nMin);
	
	//Retrieves the minimum and maximum positions for a slider. 
	void GetRange(int& nMin, int& nMax)const;
	
	// Sets the current position of the slider. 
	// nMinPos 是最小的值，MidPos是中等值，Max是最大的值
	BOOL SetPos(int nMinPos, int nMidPos, int nMaxPos); 	

	BOOL SetPos_tstat6_2pos(int nMinPos, int nMidPos, int nMaxPos); 
	BOOL SetPos_tstat6_3pos(int nMinPos, int nMidPos, int nMaxPos); 
	// Retrieves the current position of the slider. 
	int	GetPos(vector<int>& szPos);

	//////////////////////////////////////////////////////////////////////////
	CFSBChannel* GetFSBChannel();

	//////////////////////////////////////////////////////////////////////////
	void SetParentWnd(CWnd* pParent, CFSBContainer* pContainer);

	void SendCallBackMsg();
protected:
	// 计算每个step对应多少个象素
	int CalcPixelNumOfTicLength();
	void CalcTicMarks(CRect& rcChannel);
	void Draw(CDC* pDC);
	void MoveStep(const CPoint& pt);
	void MovePage(const CPoint& pt);
	void DrawTics(CDC* pDC);					// 画两列刻度
	//void DrawTicMark(CDC* pDC, const CPoint& ptStart, const CPoint& ptEnd);			// 画一个刻度
	//GetBuddy 
// 	CRect GetChannelRect ()
// 		GetLineSize 		Retrieves the line size of a slider control. 
// 		GetNumTics 		Retrieves the number of tick marks in a slider control. 
// 		GetPageSize  	Retrieves the page size of a slider control. 
// 		GetPos 		Retrieves the current position of the slider. 
// 		GetRange 		Retrieves the minimum and maximum positions for a slider. 
// 		GetRangeMax 		Retrieves the maximum position for a slider. 
// 		GetRangeMin 		Retrieves the minimum position for a slider. 
// 		GetSelection 		Retrieves the range of the current selection. 


// 		GetThumbRect 		Retrieves the size of the slider control's thumb. 
// 		GetTic 		Retrieves the position of the specified tick mark. 
// 		GetTicArray 		Retrieves the array of tick mark positions for a slider control. 
// 		GetTicPos 		Retrieves the position of the specified tick mark, in client coordinates. 
// 		GetToolTips 		Retrieves the handle to the tooltip control assigned to the slider control, if any. 
// 		SetBuddy 		Assigns a window as the buddy window for a slider control. 
// 		SetLineSize 		Sets the line size of a slider control. 
// 		SetPageSize 		Sets the page size of a slider control. 
// 		SetPos 		Sets the current position of the slider. 
// 		SetRange 		Sets the minimum and maximum positions for a slider. 

// 		SetSelection 		Sets the range of the current selection. 

// 		SetTic 		Sets the position of the specified tick mark. 
// 		SetTicFreq 		Sets the frequency of tick marks per slider control increment. 
// 		SetTipSide 		Positions a tooltip control used by a trackbar control. 
// 		SetToolTips 


protected:
	CRect									m_rc;
	FSB_THUMBSHAPE				m_fsbTbShape;
	FSB_STYLE							m_fsbStyle;
	FSB_TICSIDE						m_fsbTicSide;


	CFSBChannel						m_fsbChannel;
	
	//CThumbManager*				m_pThumbManager;

	int										m_nMax;
	int										m_nMin;

	BOOL									m_bHorizontal;			// 控件是否水平放置

	BOOL									m_bStartDrag;

	int										m_nTicLength;			// 每个刻度单位长度（象素单位）
	

	int										m_nPageLength;		// 每步长几个刻度单位


	CThumbOperator*				m_pThumbOpter;		// 
	//
	COLORREF							m_clrTic;					// 刻度颜色

	vector<int>							m_szTics;				// 刻度数组
	CWnd*								m_pParent;
	
	CFSBContainer*					m_pContainer;
	CRITICAL_SECTION	*			m_pTicMarkLock;
public:
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

 
};


