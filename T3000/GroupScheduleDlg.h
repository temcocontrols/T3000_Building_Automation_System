#pragma once
#include "afxcmn.h"
#include "CM5/ListCtrlEx.h"
#include <vector>
#include <algorithm>
using namespace std;

// CGroupScheduleDlg dialog
struct TSTatInfoNode
{
	CString TstatID;
	bool OnlineStatus;
	CString TstatName;
};
class CGroupScheduleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGroupScheduleDlg)

public:
	CGroupScheduleDlg(CWnd* pParent,CString minipanelip);   // standard constructor
	virtual ~CGroupScheduleDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MINIPANEL_GROUP_SCHEDULE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void Initial_datalist();
public:
	ListCtrlEx::CListCtrlEx m_datalist;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSave();
	CString m_Minipanel_IP;
	vector<TSTatInfoNode> m_vecTstatID;
	vector<CString> m_ScheduleName;
};
