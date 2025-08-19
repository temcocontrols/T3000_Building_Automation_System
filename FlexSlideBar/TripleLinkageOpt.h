/********************************************************************
	created:	2011/08/04
	created:	4:8:2011   14:11
	filename: 	e:\My NewTest\SlideBar\FlexSlideBar\FlexSlideBar\TripleLinkageOpt.h
	file path:	e:\My NewTest\SlideBar\FlexSlideBar\FlexSlideBar
	file base:	TripleLinkageOpt
	file ext:	h
	author:			
	purpose:	 This style Slide has 3 Thumbs, where the middle Thumb is the main Thumb (main Thumb),
	the other two are subordinate Thumbs (or called linkage Thumbs, LinkageThumb).
	During operation, when moving the main Thumb, the subordinate Thumbs will also move simultaneously, moving equal distances, until the edge.
	When moving any subordinate Thumb, it does not affect other Thumbs, but cannot cross other Thumbs and boundaries.
	purpose:	 ������ʽSlide��3��Thumb�������м��ThumbΪ��Thumb��main Thumb����
	��������Ϊ����Thumb�����߳�Ϊ����Thumb��LinkageThumb����
	�ڲ���ʱ�����ƶ���Thumbʱ������ThumbҲ��ͬʱ�������ƶ���ȵľ��룬ֱ����Ե��
	���ƶ��κ�һ������Thumb����Ӱ������Thumb�����ǲ���Խ������Thumb�ͱ߽硣
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
	// Will return the currently active one
	// ���᷵�ص�ǰ��Ծ
	virtual int GetThumbNum();
	virtual int GetThumbPosition();	
	virtual int GetAllThumbPosition(vector<int>& szThumbPos);	
	virtual void SetThumbPosition(int nIndex, int nPosition);	

	void SetParentWnd(CWnd* pParent);
	//////////////////////////////////////////////////////////////////////////
	// Set thumb position
	// ����thumb��λ��
	void SetThumbPosition(int nFirstPos, int nMidPos, int nLastPos);
	void GetThumbPosition(int& nFirstPos, int& nMidPos, int& nLastPos);

	virtual void  CalcThumbPosition();
	//////////////////////////////////////////////////////////////////////////
	// non virtual function
	// Set the number of Thumbs
	// ����Thumb�ĸ���
	// void SetThumbNum(int nThumbNum);

protected:

	// Get the boundary values (scale units) that a Thumb can move to, i.e., the scale positions (Position) of its two adjacent Thumbs. (Cannot overlap)
	// param1: nIndex, thumb index, i.e., position in thumb array, according to regulations, all thumbs cannot cross other adjacent thumbs.
	// param2: nMin, lower boundary, minimum scale position that thumb can move to
	// param3: nMax, upper boundary, maximum scale position that thumb can move to
	// ���һ��Thumb�����ƶ����ı߽�ֵ(�̶ȵ�λ)�����������ڵ�����Thumb�Ŀ̶�λ�ã�Position�����������غϣ�
	// param1: nInded , thumb ������������thumb�����е�λ�ã����ݹ涨�����е�thumb������Խ���������ڵ�thumb��
	// param2: nMin, �±߽磬thumb�����ƶ�������С�Ŀ̶�λ��
	// param3: nMax,�ϱ߽磬thumb�����ƶ��������Ŀ̶�λ��
	//
	void GetThumbRange(int nIndex, int& nMin, int& nMax);
	int GetFocusThumb(const CPoint& point);
	void MoveToPosition(int nIndex, int nPosition);
	int SearchThumbPosition(const CPoint& pt);

	// Specifically for two subordinate thumbs movepage use
	// ר��Ϊ��������thumb movepageʹ��
	void MovePageForOtherThumb(const CPoint& pt);
	// Specifically for main Thumb use
	// ר��Ϊ��Thumbʹ��
	void MovePageForMainThumb(const CPoint& pt);	

	// Specifically for two subordinate thumbs movepage use
	// ר��Ϊ��������thumb movepageʹ��
	void MouseMoveForOtherThumb(const CPoint& point);
	// Specifically for main Thumb use
	// ר��Ϊ��Thumbʹ��
	void MouseMoveForMainThumb(const CPoint& point);	

	// Specifically for two subordinate thumbs movepage use
	// ר��Ϊ��������thumb movepageʹ��
	void LBtnUpForOtherThumb(const CPoint& point);
	// Specifically for main Thumb use
	// ר��Ϊ��Thumbʹ��
	void LBtnUpForMainThumb(const CPoint& point);	
	
	// Calculate Channel various parts rect.
	// ����Channel ��������rect��
	void ReCalcChannelRect();


protected:

	vector<CFSBThumb*>				m_szThumb;

	vector<int>*					m_szTicMarks;  // Array corresponding to position and pixels
	                                               // λ�������ض�Ӧ������
	int								m_nFocusThumb;

	

};
