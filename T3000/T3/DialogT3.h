#pragma once
#include "../msflexgrid1.h"
#include "../resource.h"

#define FLEXGRID_CELL_COLOR						13421772
#define FLEXGRID_CELL_GRAY_COLOR				13421772





typedef struct T3_MODEL 
{
	int iaddress;
	CString CStName;
	CString CStvalue;
	CString CStautomannel;
	CString CStrange;
}t3model;




// CDialogT3 form view

class CDialogT3 : public CFormView
{
	DECLARE_DYNCREATE(CDialogT3)

protected:
public:
	CDialogT3();           // protected constructor used by dynamic creation
	virtual ~CDialogT3();

public:
	enum { IDD = IDD_DIALOG_T3 };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	DECLARE_MESSAGE_MAP()
	int m_address;
	float m_firmware;
	unsigned int m_serial;
	int m_hardware;
	CString m_model;
	int m_picVersion;
	CMsflexgrid m_msflexgrid_input;
	CMsflexgrid m_msflexgrid_output;
	virtual void OnInitialUpdate();
public:
	void Fresh();
	void ShowDialogData();
	void CreatDBTable();
	BOOL GetDatabase();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	unsigned short shmPRegister[512];
	vector<t3model>vect3model;
	t3model m_t3model;
	int comnum;//¶ÁÈ¡´®¿ÚºÅ
	BOOL MiniUpdateData();
	void Updatedatabase();
};


