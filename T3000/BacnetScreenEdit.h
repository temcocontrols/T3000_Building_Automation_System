#pragma once
#include "RelayLabel.h"
#include "GraphicView.h"
// CBacnetScreenEdit dialog

class CBacnetScreenEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetScreenEdit)

public:
	CBacnetScreenEdit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetScreenEdit();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_SCREENS_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	int m_nFoucsIndext;
	BOOL m_LbtDown;
public:
	afx_msg void OnBnClickedButtonScreenEditTest();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();




	void InitGraphic(int nSerialNum,int nInstanceID);
	void ClearAllLabels();
	void ReloadLabelsFromDB();
	void PainNoImageInfo(CDC* pDC);
	void Add_Label();
	void Edit_Label();
	int HitTestEx(CPoint & point);
	void saveLabelInfo(int nItem);
	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);

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
	_ConnectionPtr m_pCon;
	_RecordsetPtr m_pRs;
	int m_cxScreen;
	int m_cyScreen;
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT  RedrawScreeneditWindow(WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonScreenExit();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonDeleteAll();
};
