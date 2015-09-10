#pragma once
#include "FlexSlideBarDef.h"


class CFSBThumb
{


protected:
	void DrawBarThumb(CDC* pDC);
	void DrawRectThumb(CDC* pDC);
	void DrawCircleThumb(CDC* pDC);
	void DrawTriangleThumb(CDC* pDC);		
	

public:
	CFSBThumb(void);
	virtual ~CFSBThumb(void);


	void Draw(CDC* pDC);

	void SetWidth(int nWidth);
	int GetWidth();

	COLORREF GetColor();
	void SetColor(COLORREF clr);

	void SetShape(FSB_THUMBSHAPE fsbShape);

	void SetPosition(int nPos);
	int GetPosition();

	void SetPixPosition(CPoint pt);
	CPoint GetPixPosition();
	
	void SetHorizontal(BOOL bHorizontal);

	void InitThumb(CPoint ptCenter, int nPosition, int nWidth, int nHeight, BOOL bHorizontal);
	
	CRect GetThumbRect();

	void MoveThumb(const CPoint& ptNewPosition);

	BOOL IsPtOnThumb(const CPoint& pt);
     
    void SetMixValue(int min);
protected:
	BOOL							m_bHorizontal;
	int								m_nPosX;				// 象素的位置
	int								m_nPosY;

	int								m_nHeight;
	int								m_nWidth;
	CRect							m_rc;
	COLORREF					    m_clr;
	
	BOOL							m_bIsDragging;
	FSB_THUMBSHAPE		m_shape;
		
	int								m_nPosition;			// 刻度的位置
    
    int m_min;

};
