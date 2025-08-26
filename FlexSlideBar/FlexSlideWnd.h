#pragma once

#include "FSBThumb.h"
#include "FSBChannel.h"
//#include "ThumbManager.h"
#include "ThumbOperator.h"

//#include "..\..\TestFSB\TestFSBDlg.h"

#include <vector>
using namespace std;

// CFlexSlideWnd

// typedef void (*pCallBackFunc)();
// typedef void (CFlexSlideWnd::pCallBackFunc)(void)

#define WM_USER_CALLBACK WM_USER + 1001

class CTestFSBDlg;
class CFSBContainer;
class CFlexSlideWnd : public CStatic
{
	DECLARE_DYNAMIC(CFlexSlideWnd)

public:
	CFlexSlideWnd();
	virtual ~CFlexSlideWnd();

	
protected:
	DECLARE_MESSAGE_MAP()



public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);


//////////////////////////////////////////////////////////////////////////

public:

	void SetFSBStyle(FSB_STYLE fsbStyle);
	FSB_STYLE GetFSBStyle();

	void SetFSBThumbShape(FSB_THUMBSHAPE fsbTbShape);
	
	// Set the position of the control
	// ���ÿؼ���λ��
	void SetFlexSlideBarRect(CRect& rc);	
	CRect& GetFlexSlideBarRect();

	// Set channel width, not greater than control width, not less than 1 pixel
	// Channel length should be calculated based on control length
	// ����channel�Ŀ�ȣ����ô��ڿؼ���ȣ���С��1������
	// channel ����Ӧ�����ݿؼ�������������
	void SetChannelWidth(int nChannelWidth);
	int GetChannelWidth();

	// Set Channel color
	// ����Channel����ɫ
	COLORREF GetChannelColor();					
	void SetChannelColor(COLORREF clrChannel);

	// Set Thumb length, not less than channel width, not greater than control width, thumb width is fixed value
	// ����Thumb�ĳ��ȣ���С��channel�Ŀ�ȣ������ڿؼ��Ŀ�ȣ�thumb�Ŀ��Ϊ�̶�ֵ
	int GetThumbWidth();					
	void SetThumbWidth(int nThumbLen);

	// Set Thumb color
	// ����Thumb����ɫ
	COLORREF GetThumbColor()const;					
	void SetThumbColor(COLORREF clrThumb);

	//Sets the maximum position for a slider. 
	void	SetRangeMax(int nMax);
	void	SetRangeMin(int nMin);
	
	// Whether the control is horizontally placed
	// �ؼ��Ƿ�ˮƽ����
	void SetHorizontal(BOOL bHorizontal);
	BOOL IsHorizontal();
	

	void SetPageLength(int nPageLength);

	BOOL CreateFlexSlideBar(
		BOOL bHorizontal,	
		FSB_STYLE style=FSB_STYLE_SINGLETHUMB, 
		int nChannelWidth=10, 
		int nThumbWidth=12, 
		FSB_THUMBSHAPE tbShape=FSB_THUMBSHAPE_RECT									   
		);


	//Sets the minimum and maximum positions for a slider. 
	void SetRange(int nMax, int nMin);
	
	//Retrieves the minimum and maximum positions for a slider. 
	void GetRange(int& nMin, int& nMax)const;
	
	// Sets the current position of the slider. 
	// nMinPos is the minimum value, MidPos is the medium value, Max is the maximum value
	// nMinPos ����С��ֵ��MidPos���е�ֵ��Max������ֵ
	BOOL SetPos(int nMinPos, int nMidPos, int nMaxPos); 	

	BOOL SetPos_tstat6_2pos(int nMinPos, int nMidPos, int nMaxPos); 
	BOOL SetPos_tstat6_3pos(int nMinPos, int nMidPos, int nMaxPos); 
	// Retrieves the current position of the slider. 
	int	GetPos(vector<int>& szPos);

	//////////////////////////////////////////////////////////////////////////
	CFSBChannel* GetFSBChannel();

	//////////////////////////////////////////////////////////////////////////
	void SetParentWnd(CWnd* pParent, CFSBContainer* pContainer);

	void SendCallBackMsg();
protected:
	// Calculate how many pixels each step corresponds to
	// ����ÿ��step��Ӧ���ٸ�����
	int CalcPixelNumOfTicLength();
	void CalcTicMarks(CRect& rcChannel);
	void Draw(CDC* pDC);
	void MoveStep(const CPoint& pt);
	void MovePage(const CPoint& pt);
	void DrawTics(CDC* pDC);					// Draw two columns of scales
	                                            // �����п̶�
	//void DrawTicMark(CDC* pDC, const CPoint& ptStart, const CPoint& ptEnd);			// Draw a scale
	                                                                                        // ��һ���̶�
	//GetBuddy 
// 	CRect GetChannelRect ()
// 		GetLineSize 		Retrieves the line size of a slider control. 
// 		GetNumTics 		Retrieves the number of tick marks in a slider control. 
// 		GetPageSize  	Retrieves the page size of a slider control. 
// 		GetPos 		Retrieves the current position of the slider. 
// 		GetRange 		Retrieves the minimum and maximum positions for a slider. 
// 		GetRangeMax 		Retrieves the maximum position for a slider. 
// 		GetRangeMin 		Retrieves the minimum position for a slider. 
// 		GetSelection 		Retrieves the range of the current selection. 


// 		GetThumbRect 		Retrieves the size of the slider control's thumb. 
// 		GetTic 		Retrieves the position of the specified tick mark. 
// 		GetTicArray 		Retrieves the array of tick mark positions for a slider control. 
// 		GetTicPos 		Retrieves the position of the specified tick mark, in client coordinates. 
// 		GetToolTips 		Retrieves the handle to the tooltip control assigned to the slider control, if any. 
// 		SetBuddy 		Assigns a window as the buddy window for a slider control. 
// 		SetLineSize 		Sets the line size of a slider control. 
// 		SetPageSize 		Sets the page size of a slider control. 
// 		SetPos 		Sets the current position of the slider. 
// 		SetRange 		Sets the minimum and maximum positions for a slider. 

// 		SetSelection 		Sets the range of the current selection. 

// 		SetTic 		Sets the position of the specified tick mark. 
// 		SetTicFreq 		Sets the frequency of tick marks per slider control increment. 
// 		SetTipSide 		Positions a tooltip control used by a trackbar control. 
// 		SetToolTips 


protected:
	CRect									m_rc;
	FSB_THUMBSHAPE				m_fsbTbShape;
	FSB_STYLE							m_fsbStyle;
	FSB_TICSIDE						m_fsbTicSide;


	CFSBChannel						m_fsbChannel;
	
	//CThumbManager*				m_pThumbManager;

	int										m_nMax;
	int										m_nMin;

	BOOL									m_bHorizontal;			// Whether the control is horizontally placed
	                                                        // �ؼ��Ƿ�ˮƽ����

	BOOL									m_bStartDrag;

	int										m_nTicLength;			// Length of each scale unit (pixel unit)
	                                                        // ÿ���̶ȵ�λ���ȣ����ص�λ��
	

	int										m_nPageLength;		// How many scale units per step
	                                            // ÿ���������̶ȵ�λ


	CThumbOperator*				m_pThumbOpter;		// 
	//
	COLORREF							m_clrTic;					// Scale color
	                                                // �̶���ɫ

	vector<int>							m_szTics;				// Scale array
	                                            // �̶�����
	CWnd*								m_pParent;
	
	CFSBContainer*					m_pContainer;
	CRITICAL_SECTION	*			m_pTicMarkLock;
public:
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

 
};


