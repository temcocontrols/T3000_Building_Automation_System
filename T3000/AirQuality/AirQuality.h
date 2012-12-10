#pragma once
#include "../msflexgrid1.h"
#include "../ado/ADO.h"
#include "../MainFrm.h"


#define AirQualityTimer							1

typedef struct AIRQUALITY 
{
	int iaddress;
	CString CStvalue;
}airquality;


// CAirQuality form view

class CAirQuality : public CFormView
{
	DECLARE_DYNCREATE(CAirQuality)

protected:
public:
	CAirQuality();           // protected constructor used by dynamic creation
	virtual ~CAirQuality();

public:
	enum { IDD = IDD_DIALOG_AIRQUALITY };
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
	void Fresh();
	virtual void OnInitialUpdate();
	CMsflexgrid m_msflesgrid;
	BOOL GetDatabase();
	int comnum;//读取串口号
	vector<airquality>vecairquality;
	airquality m_vecairquality;
	void InitialFlesgrid();
	void ShowAirqualityDialog();
	int m_inaddress;
	int m_inSerialNum;
	float m_flFirmware;
	int m_inHardware;
	CString m_CStrModel;
	int m_inBaudrate;
	CString m_clock;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL prodtopc();
	unsigned short shmPRegister[512];
	void pctodb();
	BOOL GetSerialComm(vector<CString>& szComm);//读了串口
	vector<CString> m_szComm;
};


