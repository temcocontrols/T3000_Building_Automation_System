#pragma once
#include "thumboperator.h"
#include "FSBThumb.h"

class CDoubleLinkageOpt :
	public CThumbOperator
{
public:
	CDoubleLinkageOpt(void);
	virtual ~CDoubleLinkageOpt(void);


	virtual COLORREF GetThumbColor();
	virtual void SetThumbColor(COLORREF clr);
	virtual int GetThumbWidth();
	virtual void SetThumbWidth(int nWidth);
	virtual void SetThumbShape(FSB_THUMBSHAPE tbShape);
	virtual void Draw(CDC* pDC);
	virtual BOOL initThumb(BOOL bHorizontal, int nThumbWidth, CRect& rcChannel);

protected:

	CFSBThumb			m_thumb;


};
