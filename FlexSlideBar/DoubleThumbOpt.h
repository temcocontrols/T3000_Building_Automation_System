
//////////////////////////////////////////////////////////////////////////
//  
// Multiple sliders are different from a single slider
// 多个滑块的不同于一个滑块
// 1. All can slide freely, but cannot cross the adjacent other two sliders.
// 1，均可以随意滑动，但不能越过相邻的另两个滑块。
// 2. When the mouse left button clicks on a slider (LButtonDown), that slider becomes the focus slider.
// 2，当鼠标左键单击了滑块（LButtonDown），该滑块就成为焦点滑块。
// 3. When the mouse left button clicks on the scale, the focus slider can move, non-focus sliders do not move.
// 3，当鼠标左键单击标尺时，焦点滑块能够移动，非焦点滑块不动。
// 4，
//
//
//
//

#pragma once
#include "thumboperator.h"
#include "FSBThumb.h"

class CDoubleThumbOpt :
	public CThumbOperator
{
public:
	CDoubleThumbOpt(void);
	virtual ~CDoubleThumbOpt(void);

	virtual COLORREF GetThumbColor();
	virtual void SetThumbColor(COLORREF clr);
	virtual int GetThumbWidth();
	virtual void SetThumbWidth(int nWidth);
	virtual void SetThumbShape(FSB_THUMBSHAPE tbShape);
	virtual void Draw(CDC* pDC);
	virtual BOOL InitThumb(BOOL bHorizontal, int nThumbWidth, CRect& rcChannel);
	virtual void MovePage(const CPoint& pt);
	virtual void MoveNextStep();
	virtual void MovePreStep();
	virtual void OnLButtonDown(const CPoint& point);
	virtual void OnMouseMove(const CPoint& point);
	virtual void OnLButtonUp(const CPoint& point);
	virtual void SetTicsArray(vector<int>& szTics);
	virtual int GetThumbPosition();	
	virtual void SetThumbPosition(int nPosition);	


protected:
	void MoveToPosition(int nPosition);
	int GetThumbPosition(const CPoint& pt);
protected:

	BOOL						m_bStartDraging;
	// Array of position to pixel correspondence
	vector<int>*			m_szTicMarks;  // 位置与象素对应的数组

	// Left/top thumb
	CFSBThumb			m_ltThumb;		// 依次
	// Right/bottom thumb
	CFSBThumb			m_rbThumb;		// 右下
};
