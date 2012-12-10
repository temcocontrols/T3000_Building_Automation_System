#pragma once

#include"..\msflexgrid1.h"
#include "..\MainFrm.h"
#include "../DialogInfo.h"
// CCDialogISPTOOL dialog

#define AN_MAINNAME 1
#define AN_NAME		2
#define AN_SerialID	3
#define AN_FLOORNAME	4
#define	AN_ROOMNAME	5
#define	AN_PRUDUCTNAME	6
#define AN_PRODUCTTYPE	7
#define AN_PRODUCTID	8
#define AN_SCREENID	    9
#define AN_BAUDRATE		10
#define	AN_GRAPHICID	11
#define AN_HDVERSION	12
#define AN_SWVERSION	13
#define AN_EPSIZE		14



class CCDialogISPTOOL : public CDialog
{
	DECLARE_DYNAMIC(CCDialogISPTOOL)

public:
	CCDialogISPTOOL(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCDialogISPTOOL();

// Dialog Data
	enum { IDD = IDD_DIALOG10_ISPTOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedButtonFlash();
	virtual BOOL OnInitDialog();

	_ConnectionPtr m_pCon;
	_RecordsetPtr m_pRs;

	void AllBuildingDBDisplay();
	CMsflexgrid m_flexgridISP;


	CString m_strMainBuildingName;
	CString m_strSubNetName;
	CString m_strID;

	CString	m_strHexFileName;			// hex文件名，包含路径，要烧录的文件，实际上也可能是bin文件
	CDialogInfo* pDialogInfo;
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgridAllnode();
};
