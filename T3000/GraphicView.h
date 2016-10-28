#pragma once
#include "resource.h"
#include "RelayLabel.h"
#include "afxwin.h"
#include "../SQLiteDriver/CppSQLite3.h"

// CGraphicView form view


struct Label_information///////////////////////////////////////////////////////////////////////////////
{
	int cstatic_id;//the id of the CStatic control button
	int input_or_output;//true == output ,false == input
	int nSerialNum;
	int tstat_id;//the id of tstat	
//	bool picture_or_text;//true == a picture;false == a text
//	CString str;//path picture file path or text 
	CString the_value;//the value of select
	int width;//the cstatic width;
	int height;//the cstatic height
	CPoint point;//the rect left/top point
	int status;//for input or output selection,such as Relay 1 ,Relay 2,Relay 3...
	COLORREF clrTxt;
	COLORREF bkColor;
	CString strTips;//indcatcor
	CString strScreenName;
	CRelayLabel* plabelCtrl;
	
};///////////////////////////////////////////////////////////////////////////////


class CGraphicView : public CFormView
{
	DECLARE_DYNCREATE(CGraphicView)

public:
	CGraphicView();           // protected constructor used by dynamic creation
	virtual ~CGraphicView();
	afx_msg void OnPaint();
public:
	enum { IDD = IDD_GRAPHICVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif
private:
	int m_nFoucsIndext;
	BOOL m_LbtDown;
	
public:
	vector<Label_information> m_RelayLabelLst;
	vector<CRelayLabel> m_labelCtrlLst;
	CString m_strImgPathName;
	CString m_strScreenName;
	int m_nSerialNumber;
	int m_nTstatID;
	int m_InstanceID;
	unsigned short m_Mac;

	CRelayLabel m_Label;

	BOOL m_bImgExist;
public:
 
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	//unsigned short multi_register_value[513];
	CMenu m_menu;
	CMFCMenuButton m_delMenuBtn;
public:
	void InitGraphic(int nSerialNum,int nTstatID);
	void InitGraphic(int nInstanceID,unsigned short nMac);
	void PainNoImageInfo(CDC* pDC);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int HitTestEx(CPoint & point);
	void ClearAllLabels();
	void ReloadLabelsFromDB();
	void saveLabelInfo(int nItem);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	afx_msg void OnBnClickedAddbutton();
	afx_msg void OnBnClickedEdtbutton();
	afx_msg void OnBnClickedDelbutton();
	afx_msg void OnBnClickedApplybutton();

	afx_msg void OnDestroy();
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedGobackbutton();
	CMFCButton m_bLockBtn;
	CMFCMenuButton m_ImgConfigBtn;
	CMenu m_ImgConfigmenu;
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3Query q;
	afx_msg void OnBnClickedImgcnfigbutton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


