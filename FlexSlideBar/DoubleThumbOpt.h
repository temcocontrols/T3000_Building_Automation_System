
//////////////////////////////////////////////////////////////////////////
//  
// Multiple sliders are different from a single slider
// �������Ĳ�ͬ��һ������
// 1. All can slide freely, but cannot cross the adjacent other two sliders.
// 1�����������⻬����������Խ�����ڵ����������顣
// 2. When the mouse left button clicks on a slider (LButtonDown), that slider becomes the focus slider.
// 2���������������˻��飨LButtonDown�����û���ͳ�Ϊ���㻬�顣
// 3. When the mouse left button clicks on the scale, the focus slider can move, non-focus sliders do not move.
// 3�����������������ʱ�����㻬���ܹ��ƶ����ǽ��㻬�鲻����
// 4��
//
//
//
//

#pragma once
#include "thumboperator.h"
#include "FSBThumb.h"

class CDoubleThumbOpt :
	public CThumbOperator
{
public:
	CDoubleThumbOpt(void);
	virtual ~CDoubleThumbOpt(void);

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
	virtual void SetThumbPosition(int nPosition);	


protected:
	void MoveToPosition(int nPosition);
	int GetThumbPosition(const CPoint& pt);
protected:

	BOOL						m_bStartDraging;
	// Array of position to pixel correspondence
	vector<int>*			m_szTicMarks;  // λ�������ض�Ӧ������

	// Left/top thumb
	CFSBThumb			m_ltThumb;		// ����
	// Right/bottom thumb
	CFSBThumb			m_rbThumb;		// ����
};
