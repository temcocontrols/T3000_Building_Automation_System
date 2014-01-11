#pragma once
#include "msflexgrid1.h"
#include "afxwin.h"
#include "Dialog_Progess.h"
#include <vector>
using namespace std;
// CLCOutNameConfig dialog

class CLCOutNameConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CLCOutNameConfig)

public:
	CLCOutNameConfig(int sn,CWnd* pParent = NULL);   // standard constructor
	virtual ~CLCOutNameConfig();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_LC_OUTPUTS_NAMECONFIG };
	void to_fresh();
	BOOL LoadFromDB();
	void Read_FromOneCard(int cardno);
	CString get_OutputName(int card,int output);
	void    Insert_Update_OutputName(int sn,int card,int output ,CString outputname);
	void Update_IntoDB();
	int m_sn;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	DECLARE_MESSAGE_MAP()
public:
	CMsflexgrid m_outputname;
	DECLARE_EVENTSINK_MAP()
	void ClickInputMsflexgrid();
	 
	CEdit m_editName;
	CString m_stroldername;
	int m_currow;
	int m_curcol;
	afx_msg void OnEnKillfocusEdit1();
	struct struct_LCName{
	int card;
	int out;
	CString name;
	};
	vector<struct_LCName> m_vecLcName;
};
