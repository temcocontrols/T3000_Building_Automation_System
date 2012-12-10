#pragma once
#include "thumboperator.h"
#include "FSBThumb.h"
//#include "TripleLinkageOpt.h"//lsc

class CTripleThumbOpt :
	public CThumbOperator
//public CTripleLinkageOpt //lsc

{
public:
	CTripleThumbOpt(void);
	virtual ~CTripleThumbOpt(void);

	virtual COLORREF GetThumbColor();
	virtual void SetThumbColor(COLORREF clr);
	virtual int GetThumbWidth();
	void SetThumbWidth(int nWidth);
	virtual void SetThumbShape(FSB_THUMBSHAPE tbShape);
	virtual void Draw(CDC* pDC);
	virtual BOOL initThumb(BOOL bHorizontal, int nThumbWidth, CRect& rcChannel);

protected:

	CFSBThumb			m_thumb;
};
