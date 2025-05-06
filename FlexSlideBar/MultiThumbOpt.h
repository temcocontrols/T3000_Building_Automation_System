
//////////////////////////////////////////////////////////////////////////
//  
// 
// 1
// 2LButtonDown
// 3
// 4yslide
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
	// 
	virtual int GetThumbNum();
	virtual int GetThumbPosition();	
	virtual int GetAllThumbPosition(vector<int>& szThumbPos);	
	virtual void SetThumbPosition(int nIndex, int nPosition);	

	virtual void CalcThumbPosition();
	//////////////////////////////////////////////////////////////////////////
	// non virtual function
	// Thumb
	void SetThumbNum(int nThumbNum);
    
protected:

	// Thumb()ThumbPosition
	// param1: nInded , thumb thumbthumbthumb
	// param2: nMin, thumb
	// param3: nMax,thumb
	//
	void GetThumbRange(int nIndex, int& nMin, int& nMax);
	int GetFocusThumb(const CPoint& point);
	void MoveToPosition(int nIndex, int nPosition);
	int SearchThumbPosition(const CPoint& pt);
	void ReCalcChannelRect();
    
protected:
	
	vector<int>*					m_szTicMarks;  // 

	vector<CFSBThumb*>				m_szThumb;

	int								m_nFocusThumb;
   /* int m_nMinPos;
    int m_nMidPos;
    int m_nMaxPos;*/
};
