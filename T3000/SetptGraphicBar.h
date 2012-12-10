#ifndef _UNICODE
#define _UNICODE 
#endif
#pragma once

typedef struct _ThumTag
{
	Bitmap* pImg;
	CRect rcImg;
}ThumTag;
class CSetptGraphicBar
{
private:
	HWND m_hParentWnd;
public:
	CSetptGraphicBar(void);
	~CSetptGraphicBar(void);
	 
public:
	int m_nUnit;
	int m_nRulerHeight;
	int m_nRulerBegin;
	int m_nRulerEnd;

	CRect m_rcClient;
	int m_nType;
	CString m_strbkImge;
	CString m_strThmbImg;


	Bitmap* m_pBkImg;
	int m_nbkImgWidth;
	int m_nbkImgHeight;

	Bitmap* m_pThumbImg;
	ThumTag m_Thumb;
	int m_nThumbWidth;
	int m_nThumbHeight;

	ThumTag m_MaxImg;
	ThumTag m_MinImg;
	ThumTag m_SetPtImg;

	
	ThumTag m_CoolSetPt;
	ThumTag m_HeatSetPt;



	CRect m_rcTank;
	float m_fFirmwareVersion;
	BOOL m_bSetPtSelected;
	BOOL m_bMaxSelected;
	BOOL m_bMinSelected;
	CString m_strSrtPt;
	int m_nDragTempvalue;

	CString m_strMax;
	CString m_strMin;
	CString m_strHeat;
	CString m_strCool;

	BOOL m_bHeatSelected;
	BOOL m_bCoolSelected;

public:
	//nStyle: 1 means 3buttons ;2 means 2 buttons;
	void CreatSliderBar(CRect rcSliderBar,int nStyle,HWND hParent);
	void InitialBar();
	void DrawRuler(CDC* pDC);

	void DrawGraphicBar(CDC* pDC);
	void SetImg(Bitmap* pBkImg,Bitmap* pThumb);

	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);
	int PosTOValue(int nPix);
};
