/********************************************************************
	created:	2011/08/04
	created:	4:8:2011   14:11
	filename: 	e:\My NewTest\SlideBar\FlexSlideBar\FlexSlideBar\TripleLinkageOpt.h
	file path:	e:\My NewTest\SlideBar\FlexSlideBar\FlexSlideBar
	file base:	TripleLinkageOpt
	file ext:	h
	author:			
	purpose:	 Slide3ThumbThumbThumbmain Thumb
	ThumbThumbLinkageThumb
	ThumbThumb
	ThumbThumbThumb
*********************************************************************/

#pragma once
#include "thumboperator.h"
#include "FSBThumb.h"


const int FOCUS_ON_EMPTY= -1;
const int FOCUS_ON_TOPLEFT = 0;
const int FOCUS_ON_MID = 1;
const int FOCUS_ON_RIGHTBTM = 2;


class CTripleLinkageOpt :
	public CThumbOperator
{
public:
	CTripleLinkageOpt(void);
	virtual ~CTripleLinkageOpt(void);


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

	void SetParentWnd(CWnd* pParent);
	//////////////////////////////////////////////////////////////////////////
	// thumb
	void SetThumbPosition(int nFirstPos, int nMidPos, int nLastPos);
	void GetThumbPosition(int& nFirstPos, int& nMidPos, int& nLastPos);

	virtual void  CalcThumbPosition();
	//////////////////////////////////////////////////////////////////////////
	// non virtual function
	// Thumb
	// void SetThumbNum(int nThumbNum);

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

	// thumb movepage
	void MovePageForOtherThumb(const CPoint& pt);
	// Thumb
	void MovePageForMainThumb(const CPoint& pt);	

	// thumb movepage
	void MouseMoveForOtherThumb(const CPoint& point);
	// Thumb
	void MouseMoveForMainThumb(const CPoint& point);	

	// thumb movepage
	void LBtnUpForOtherThumb(const CPoint& point);
	// Thumb
	void LBtnUpForMainThumb(const CPoint& point);	
	
	// Channel rect
	void ReCalcChannelRect();


protected:

	vector<CFSBThumb*>				m_szThumb;

	vector<int>*					m_szTicMarks;  // 
	int								m_nFocusThumb;

	

};
