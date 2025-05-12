#pragma once



// CTransducer 窗体视图

class CTransducer : public CFormView
{
	DECLARE_DYNCREATE(CTransducer)

public:
	CTransducer();           // 动态创建所使用的受保护的构造函数
	virtual ~CTransducer();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TRANSDUCER };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void Fresh();
	afx_msg LRESULT UpdateUI(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedRadioTransducerDegC();
	afx_msg void OnBnClickedRadioTransducerDegF();
	afx_msg void OnEnKillfocusEditTempOffset();
	afx_msg void OnEnKillfocusEditHumOffset();
	afx_msg void OnBnClickedRadioTransducer420Ma();
	afx_msg void OnBnClickedRadioTransducer010v();
	afx_msg void OnBnClickedRadioDew();
	afx_msg void OnBnClickedRadioEnthalpy();
	afx_msg void OnBnClickedRadioAbs();
	afx_msg void OnBnClickedRadioReal();
	int is_new_firmware = false;
	afx_msg void OnCbnSelchangeComboTempRange();
	void EnableNewUI(bool flag);
	void InitialPaintParameter();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedRadioDefaultT();
	afx_msg void OnBnClickedRadioUserDefinedT();
	afx_msg void OnBnClickedRadioDefaultH();
	afx_msg void OnBnClickedRadioUserDefinedH();
	afx_msg void OnEnKillfocusEditVolatgeMinT();
	afx_msg void OnEnKillfocusEditVolatgeMaxT();
	afx_msg void OnEnKillfocusEditCurrentMinT();
	afx_msg void OnEnKillfocusEditCurrentMaxT();
	afx_msg void OnEnKillfocusEditMinT();
	afx_msg void OnEnKillfocusEditMaxT();
	afx_msg void OnEnKillfocusEditVolatgeMinH();
	afx_msg void OnEnKillfocusEditVolatgeMaxH();
	afx_msg void OnEnKillfocusEditCurrentMinH();
	afx_msg void OnEnKillfocusEditCurrentMaxH();
	afx_msg void OnEnKillfocusEditMinH();
	afx_msg void OnEnKillfocusEditMaxH();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
static DWORD WINAPI Update_xducer_Thread(LPVOID lPvoid);





#define HUM_DEWPOINT   0  
#define HUM_ENTHALPY   1
#define HUM_ABSOLUTE    2
#define HUM_REAL        3

const CString TransducerTempRange[] =
{
	_T("0.   0  - 100  Deg.C"),
	_T("1.  -20 - 80  Deg.C"),       
	_T("2.   0  - 50  Deg.C"),
	_T("3.  -50 - 50  Deg.C"),
	_T("4.  -20 - 140 Deg.F"),
	_T("5.   0  - 100 Deg.F"),
	_T("6.   40 - 90  Deg.F"),
	_T("7.  -40 - 140 Deg.F")
};

