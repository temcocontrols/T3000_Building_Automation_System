/********************************************************************
	created:	2011/08/04
	created:	4:8:2011   14:11
	filename: 	e:\My NewTest\SlideBar\FlexSlideBar\FlexSlideBar\TripleLinkageOpt.h
	file path:	e:\My NewTest\SlideBar\FlexSlideBar\FlexSlideBar
	file base:	TripleLinkageOpt
	file ext:	h
	author:			
	purpose:	 此种样式Slide有3个Thumb，其中中间的Thumb为主Thumb（main Thumb），
	其余两个为从属Thumb（或者称为联动Thumb，LinkageThumb）。
	在操作时，当移动主Thumb时，从属Thumb也会同时联动，移动相等的距离，直到边缘。
	而移动任何一个从属Thumb，则不影响其他Thumb，但是不得越过其他Thumb和边界。
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
	// 将会返回当前活跃
	virtual int GetThumbNum();
	virtual int GetThumbPosition();	
	virtual int GetAllThumbPosition(vector<int>& szThumbPos);	
	virtual void SetThumbPosition(int nIndex, int nPosition);	

	void SetParentWnd(CWnd* pParent);
	//////////////////////////////////////////////////////////////////////////
	// 设置thumb的位置
	void SetThumbPosition(int nFirstPos, int nMidPos, int nLastPos);
	void GetThumbPosition(int& nFirstPos, int& nMidPos, int& nLastPos);

	virtual void  CalcThumbPosition();
	//////////////////////////////////////////////////////////////////////////
	// non virtual function
	// 设置Thumb的个数
	// void SetThumbNum(int nThumbNum);

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

	// 专门为两个从属thumb movepage使用
	void MovePageForOtherThumb(const CPoint& pt);
	// 专门为主Thumb使用
	void MovePageForMainThumb(const CPoint& pt);	

	// 专门为两个从属thumb movepage使用
	void MouseMoveForOtherThumb(const CPoint& point);
	// 专门为主Thumb使用
	void MouseMoveForMainThumb(const CPoint& point);	

	// 专门为两个从属thumb movepage使用
	void LBtnUpForOtherThumb(const CPoint& point);
	// 专门为主Thumb使用
	void LBtnUpForMainThumb(const CPoint& point);	
	
	// 计算Channel 各个部分rect。
	void ReCalcChannelRect();


protected:

	vector<CFSBThumb*>				m_szThumb;

	vector<int>*					m_szTicMarks;  // 位置与象素对应的数组
	int								m_nFocusThumb;

	

};
