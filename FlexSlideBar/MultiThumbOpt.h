
//////////////////////////////////////////////////////////////////////////
//  
// 多个滑块的不同于一个滑块
// 1，均可以随意滑动，但不能越过相邻的另两个滑块。
// 2，当鼠标左键单击了滑块（LButtonDown），该滑块就成为焦点滑块。
// 3，当鼠标左键单击标尺时，焦点滑块能够移动，非焦点滑块不动。
// 4，注意，窗口坐标y值向下为＋递增，而slide刻度向上递增，因此，需要时刻注意坐标的转换
//
//
//
//

#pragma once
#include "thumboperator.h"


class CFSBThumb;
class CMultiThumbOpt :
	public CThumbOperator
{
public:
	CMultiThumbOpt(void);
	virtual ~CMultiThumbOpt(void);

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
	// 将会返回当前活跃
	virtual int GetThumbNum();
	virtual int GetThumbPosition();	
	virtual int GetAllThumbPosition(vector<int>& szThumbPos);	
	virtual void SetThumbPosition(int nIndex, int nPosition);	

	virtual void CalcThumbPosition();
	//////////////////////////////////////////////////////////////////////////
	// non virtual function
	// 设置Thumb的个数
	void SetThumbNum(int nThumbNum);
    
protected:

	// 获得一个Thumb可以移动到的边界值(刻度单位)，即它的相邻的两个Thumb的刻度位置（Position）。（不能重合）
	// param1: nInded , thumb 的索引，即在thumb数组中的位置，根据规定，所有的thumb都不能越过其他相邻的thumb。
	// param2: nMin, 下边界，thumb可以移动到的最小的刻度位置
	// param3: nMax,上边界，thumb可以移动到的最大的刻度位置
	//
	void GetThumbRange(int nIndex, int& nMin, int& nMax);
	int GetFocusThumb(const CPoint& point);
	void MoveToPosition(int nIndex, int nPosition);
	int SearchThumbPosition(const CPoint& pt);
	void ReCalcChannelRect();
    
protected:
	
	vector<int>*					m_szTicMarks;  // 位置与象素对应的数组

	vector<CFSBThumb*>				m_szThumb;

	int								m_nFocusThumb;
   /* int m_nMinPos;
    int m_nMidPos;
    int m_nMaxPos;*/
};
