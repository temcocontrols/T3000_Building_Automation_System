
//////////////////////////////////////////////////////////////////////////
//  
// Multiple sliders are different from a single slider
// �������Ĳ�ͬ��һ������
// 1. All can slide freely, but cannot cross the other two adjacent sliders.
// 1�����������⻬����������Խ�����ڵ����������顣
// 2. When the left mouse button clicks on a slider (LButtonDown), that slider becomes the focus slider.
// 2���������������˻��飨LButtonDown�����û���ͳ�Ϊ���㻬�顣
// 3. When the left mouse button clicks on the ruler, the focus slider can move, non-focus sliders don't move.
// 3�����������������ʱ�����㻬���ܹ��ƶ����ǽ��㻬�鲻����
// 4. Note that window coordinate y values increase downward as +, while slide scales increase upward, so coordinate conversion must be considered at all times.
// 4��ע�⣬��������yֵ����Ϊ����������slide�̶����ϵ�������ˣ���Ҫʱ��ע�������ת��
//
//
//
//

#pragma once
#include "thumboperator.h"


class CFSBThumb;
class CMultiThumbOpt :
	public CThumbOperator
{
public:
	CMultiThumbOpt(void);
	virtual ~CMultiThumbOpt(void);

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

	virtual void CalcThumbPosition();
	//////////////////////////////////////////////////////////////////////////
	// non virtual function
	// Set the number of Thumbs
	// ����Thumb�ĸ���
	void SetThumbNum(int nThumbNum);
    
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
	void ReCalcChannelRect();
    
protected:
	
	vector<int>*					m_szTicMarks;  // Array corresponding to position and pixels
	                                               // λ�������ض�Ӧ������

	vector<CFSBThumb*>				m_szThumb;

	int								m_nFocusThumb;
   /* int m_nMinPos;
    int m_nMidPos;
    int m_nMaxPos;*/
};
