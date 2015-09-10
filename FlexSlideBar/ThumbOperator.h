#pragma once
//#include "FlexSlideBarDef.h"
#include <vector>
using namespace std;


class CThumbOperator
{
public:
	CThumbOperator(void);
	virtual ~CThumbOperator(void);


	void SetFSBStyle(FSB_STYLE fsbStyle);

	void SetChannelRect(CRect rc);

	void SetParentRect(CRect rc);

	void SetHorizontal(BOOL bHorizontal);

	void SetLengthOfPage(int nLengthOfPage);
		
	void SetLengthOfTic(int nLengthOfTic);

	virtual void OnLButtonDown(const CPoint& point)=0;
	virtual	 void OnMouseMove(const CPoint& point)=0;
	virtual COLORREF GetThumbColor()=0;
	virtual void SetThumbColor(COLORREF clr)=0;
	virtual int GetThumbWidth() = 0;
	virtual void SetThumbWidth(int nWidth) = 0;
		
	virtual void SetThumbShape(FSB_THUMBSHAPE tbShape) = 0;
	virtual void Draw(CDC* pDC) = 0;
	virtual BOOL InitThumb(BOOL bHorizontal, int nThumbWidth, CRect& rcChannel)=0;
	virtual void MovePage(const CPoint& pt)=0;
	virtual void MoveNextStep()=0;
	virtual void MovePreStep()=0;
	virtual void OnLButtonUp(const CPoint& pt)=0;

	virtual void SetTicsArray(vector<int>& szTics) = 0;
	virtual int GetThumbPosition()=0;	
	virtual void SetThumbPosition(int nIndex, int nPosition)=0;	
	virtual int GetAllThumbPosition(vector<int>& szThumbPos)=0;
	virtual void CalcThumbPosition()=0;
	virtual int GetThumbNum()=0;




	virtual void SetParentWnd(CWnd* pParent);
      void SetPosValue(int nMinPos, int nMidPos, int nMaxPos) ;

protected:
	FSB_STYLE			    m_fbStyle;

	BOOL					m_bHorizontal;
	CRect					m_rcChannel;

	CRect					m_rcParent;
	int						m_nLengthOfTic;			// 刻度，通常是指每个刻度单位的象素数。
	int						m_nLengthOfPage;		// 步长，通常是指每个步长单位的刻度数。

	BOOL					m_bStartDraging;

	CWnd*				    m_pParent;

    int m_nMinPos;
    int m_nMidPos;
    int m_nMaxPos;
};
