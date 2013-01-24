
// T3000.h : main header file for the T3000 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "define.h"

#include "global_variable_extern.h"
/************************************************************************/
/*Author:Alex 
/*Date£º2012/11/26
/*FileName:D:\T3000\T3000\T3000.H
/*Functionality:                                                        */
//this is globe variable
/************************************************************************/


extern int g_MainScreenLevel;
extern int g_ParamLevel;
extern int g_OutPutLevel;
extern int g_NetWorkLevel;
extern int g_GraphicModelevel;
extern int g_BurnHexLevel;
extern int g_LoadConfigLevel;
extern int g_BuildingsetLevel;
extern int g_AllscreensetLevel;

// CT3000App:
// See T3000.cpp for the implementation of this class
//
/********************************************************************
	created:	2012/11/26
	created:	26:11:2012   18:11
	filename: 	D:\T3000\T3000\T3000.h
	file path:	D:\T3000\T3000
	file base:	T3000
	file ext:	h
	author:		alex
	function:  COwnMemDC
*********************************************************************/
class COwnMemDC : public CDC {
	private:
		CBitmap m_bitmap; // Offscreen bitmap
		CBitmap* m_oldBitmap; // bitmap originally found in CMemDC
		CDC* m_pDC; // Saves CDC passed in constructor
		CRect m_rect; // Rectangle of drawing area.
		BOOL m_bMemDC; // TRUE if CDC really is a Memory DC.
	public:
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
		COwnMemDC(CDC* pDC, CRect rect = CRect(0,0,0,0), BOOL bCopyFirst = FALSE) : CDC(), m_oldBitmap(NULL), m_pDC(pDC)
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		{
			ASSERT(m_pDC != NULL); // If you asserted here, you passed in a NULL CDC.
			
			m_bMemDC = !pDC->IsPrinting();
			
			if (m_bMemDC){
				// Create a Memory DC
				CreateCompatibleDC(pDC);
				if ( rect == CRect(0,0,0,0) )
					pDC->GetClipBox(&m_rect);
				else
					m_rect = rect;

				m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
				m_oldBitmap = SelectObject(&m_bitmap);
				SetWindowOrg(m_rect.left, m_rect.top);
				if(bCopyFirst)
				{
					this->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
							m_pDC, m_rect.left, m_rect.top, SRCCOPY);
				}
			} else {
				// Make a copy of the relevent parts of the current DC for printing
				m_bPrinting = pDC->m_bPrinting;
				m_hDC = pDC->m_hDC;
				m_hAttribDC = pDC->m_hAttribDC;
			}
		}
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
		~COwnMemDC()
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		{
			if (m_bMemDC) {
				// Copy the offscreen bitmap onto the screen.
				m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
					this, m_rect.left, m_rect.top, SRCCOPY);
				//Swap back the original bitmap.
				SelectObject(m_oldBitmap);
			} else {
				// All we need to do is replace the DC with an illegal value,
				// this keeps us from accidently deleting the handles associated with
				// the CDC that was passed to the constructor.
				m_hDC = m_hAttribDC = NULL;
			}
		}
		
		// Allow usage as a pointer
		COwnMemDC* operator->() {return this;}
		
		// Allow usage as a pointer
		operator COwnMemDC*() {return this;}
};
class CT3000App : public CWinAppEx
{
public:
	CT3000App();
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	BOOL RegisterOcx(LPCTSTR   OcxFileName);
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	void InitModeName();
	void OnVersionInfo();
	DECLARE_MESSAGE_MAP()
public:
	BOOL user_login();
	virtual int ExitInstance();
	void CopyDirectory(CString strSrcPath,CString strDstPath);
//	bool cm5_timer;	  //CM5

	BOOL haveRegister();
	CString GetModulePath();
	int GetSoftInstallDays();
	void WriteNumber(CRegKey& key,CStringW valueName,DWORD value);
	BOOL ReadNameber(CRegKey& key,CStringW valueName,DWORD& value);
	void Judgestore();
	void ReadREG();
	DWORD m_maxClients;
	DWORD password;
	
};

extern CT3000App theApp;
