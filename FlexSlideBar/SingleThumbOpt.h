#pragma once
#include "thumboperator.h"
#include "FSBThumb.h"
//#include "MultiThumbOpt.h"//lsc

class CSingleThumbOpt :
	public CThumbOperator
	//public CMultiThumbOpt //lsc
{
public:
	CSingleThumbOpt(void);
	virtual ~CSingleThumbOpt(void);

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
	virtual void SetThumbPosition(int nIndex, int nPosition);		// 在只有一个thumb时，nIndex是无意义的
	virtual int GetAllThumbPosition(vector<int>& szThumbPos);	
	virtual void CalcThumbPosition();






	//
//	virtual void OnLButtonDown(const CPoint& point)=0;
//	virtual	 void OnMouseMove(const CPoint& point)=0;
//	virtual COLORREF GetThumbColor()=0;
//	virtual void SetThumbColor(COLORREF clr)=0;
//	virtual int GetThumbWidth() = 0;
	//virtual void SetThumbWidth(int nWidth) = 0;

//	virtual void SetThumbShape(FSB_THUMBSHAPE tbShape) = 0;
//	virtual void Draw(CDC* pDC) = 0;
//	virtual BOOL InitThumb(BOOL bHorizontal, int nThumbWidth, CRect& rcChannel)=0;
//	virtual void MovePage(const CPoint& pt)=0;
//	virtual void MoveNextStep()=0;
//	virtual void MovePreStep()=0;
//	virtual void OnLButtonUp(const CPoint& pt)=0;

//	virtual void SetTicsArray(vector<int>& szTics) = 0;
//	virtual int GetThumbPosition()=0;	
//	virtual void SetThumbPosition(int nIndex, int nPosition)=0;	
//	virtual int GetAllThumbPosition(vector<int>& szThumbPos)=0;
//	virtual void CalcThumbPosition()=0;
//	virtual int GetThumbNum()=0;
	//

















protected:
	void MoveToPosition(int nPosition);
	int GetThumbPosition(const CPoint& pt);
protected:

	CFSBThumb				m_thumb;

	
	vector<int>*			m_szTicMarks;  // 位置与象素对应的数组

};
