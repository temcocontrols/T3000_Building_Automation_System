#pragma once


#include "msflexgrid1.h"
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "global_struct.h"
// T332AI form view

class T332AI : public CFormView
{
	DECLARE_DYNCREATE(T332AI)

public:
	T332AI();           // protected constructor used by dynamic creation
	virtual ~T332AI();

public:
	enum { IDD = IDD_T332AI };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate();
	DECLARE_MESSAGE_MAP()
public:
	void Fresh();
	void InitialDialog();
	void Initial_RegisterList();
	int  Get_RegID(CString Name);
	vector<T3Register> m_vecT3Register;
	 
	int m_curcol;
	int m_currow;
private:
	int	SN_LOW	;
	int	SN_HI	;
	int	EPROM_VER_NUMBER	;
	int	FIRMWARE_VER_NUMBER	;
	int	MODBUS_ID	;
	int	PRODUCT_MODEL	;
	int	HARDWARE_VER_NUMBER	;
	int	PIC_VER_NUMBER	;
	int	CALIBRATION_OUTPUTS	;
	int	BAUDRATE	;
	int	RESPONSE_DELAY	;
	int	INPUT1	;
	int	INPUT2	;
	int	INPUT3	;
	int	INPUT4	;
	int	INPUT5	;
	int	INPUT6	;
	int	INPUT7	;
	int	INPUT8	;
	int	INPUT9	;
	int	INPUT10	;
	int	INPUT11	;
	int	INPUT12	;
	int	INPUT13	;
	int	INPUT14	;
	int	INPUT15	;
	int	INPUT16	;
	int	INPUT17	;
	int	INPUT18	;
	int	INPUT19	;
	int	INPUT20	;
	int	INPUT21	;
	int	INPUT22	;
	int	INPUT23	;
	int	INPUT24	;
	int	INPUT25	;
	int	INPUT26	;
	int	INPUT27	;
	int	INPUT28	;
	int	INPUT29	;
	int	INPUT30	;
	int	INPUT31	;
	int	INPUT32	;

public:
	CMsflexgrid m_msflexgrid_input;
	CEdit m_inNameEdt;
};


