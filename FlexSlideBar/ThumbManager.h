#pragma once
#include "FSBThumb.h"
#include "FlexSlideBarDef.h"

class CThumbManager
{
public:
	CThumbManager(void);
	virtual ~CThumbManager(void);

	void SetFSBStyle(FSB_STYLE fsbStyle);

	void SetThumbShape(FSB_THUMBSHAPE tbShape);

	void SetChannelRect(CRect rc);

	//void SetParentRect();

	void SetThumbWidth(int nWidth);
	int GetThumbWidth();
	
	COLORREF GetThumbColor();
	void SetThumbColor(COLORREF clr);

	void SetHorizontal(BOOL bHorizontal);

	BOOL initThumb(BOOL bHorizontal, int nThumbWidth, CRect& rcChannel);

	void SetStepLength(int nStep);

	void Draw(CDC* pDC);

	void MoveThumb(const CPoint& ptNewPosition);

	void MoveStep(const CPoint& pt);
protected:
	void DrawSingleThumb(CDC* pDC);
	void DrawDoubleThumb(CDC* pDC);
	void DrawTripleThumb(CDC* pDC);

	void DoubleLinkageMoveThumb(const CPoint& ptNewPosition);

private:
	FSB_STYLE			m_fbStyle;

	CFSBThumb			m_ltThumb;			// left top thumb
	CFSBThumb			m_rbThumb;			// right bottom thumb
	CFSBThumb			m_centralThumb;		// central thumb

	BOOL					m_bHorizontal;
	CRect					m_rcChannel;

	CRect					m_rcParent;
	int							m_nStepLength;		// 步长，通常是指几个刻度单位。


};
