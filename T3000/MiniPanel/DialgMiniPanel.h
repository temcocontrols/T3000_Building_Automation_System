#pragma once
#include "..//ado//ADO.h"
#include "../MainFrm.h"
#include "afxwin.h"


#define FLEXGRID_CELL_COLOR						13421772
#define FLEXGRID_CELL_GRAY_COLOR				13421772
#define MiniPanelTimer							11

// CDialgMiniPanel form view


typedef struct MINIPANEL 
{
	int iaddress;
	CString CStName;
	CString CStvalue;
	CString CStautomannel;
	CString CStrange;
}m_minipanel;

class CDialgMiniPanel : public CFormView
{
	DECLARE_DYNCREATE(CDialgMiniPanel)

protected:
public:
	CDialgMiniPanel();           // protected constructor used by dynamic creation
	virtual ~CDialgMiniPanel();

public:
	enum { IDD = IDD_DIALOG_MINIPANEL };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void Fresh(); //进入miniPanel的入口函数
	CMsflexgrid m_msflexgridinput;

	CMsflexgrid m_msflexgridDO;
	CMsflexgrid m_msflexgridAO;
	void ShowMinipDialog();//界面显示数据
	BOOL GetDatabase();		  //从数据库中读取数据
	int m_inaddress;
	int m_inSerialNum;
	float m_flFirmware;
	int m_inHardware;
	CString m_CStrModel;
	int m_inBaudrate;
	unsigned short shmPRegister[512];
	vector<m_minipanel>vecminipanel;
	m_minipanel MINImini;
	BOOL  MiniUpdateData();//更新数据并写入数据库

	
	int comnum;


	CString m_datetime;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void Updatedatabase();
	afx_msg void OnEnKillfocusEditInput();
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgridInput();
	CEdit m_editinput;
	CEdit m_editoutputdo;
	CEdit m_editoutputao;
	afx_msg void OnEnKillfocusEditoutputdo();
	afx_msg void OnEnKillfocusEditOutputao();
	void ClickMsflexgridOutputDo();
	void ClickMsflexgridOutputAo();
	void writedbinputname(int& num,CString& str );
};


