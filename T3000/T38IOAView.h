#pragma once
#include "global_struct.h"
#include "msflexgrid1.h"
#include "msflexgrid1.h"
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>


// CT38IOAView form view

class CT38IOAView : public CFormView
{
	DECLARE_DYNCREATE(CT38IOAView)

	public:
	CT38IOAView();           // protected constructor used by dynamic creation
	virtual ~CT38IOAView();
	void Fresh();
public:
	enum { IDD = IDD_DIALOG_8IOA_VIEW };
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
	void InitialRegister();
	int  Get_RegID(CString Name);
	
private:
	vector<T3Register> m_vecT3Register;
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

	int	OUTPUT1	;
	int	OUTPUT2	;
	int	OUTPUT3	;
	int	OUTPUT4	;
	int	OUTPUT5	;
	int	OUTPUT6	;
	int	OUTPUT7	;
	int	OUTPUT8	;
	int	INPUT1	;
	int	INPUT2	;
	int	INPUT3	;
	int	INPUT4	;
	int	INPUT5	;
	int	INPUT6	;
	int	INPUT7	;
	int	INPUT8	;
	int	SWITCH1_STATUS	;
	int	SWITCH2_STATUS	;
	int	RANGE_INPUT1	;
	int	RANGE_INPUT2	;
	int	RANGE_INPUT3	;
	int	RANGE_INPUT4	;
	int	RANGE_INPUT5	;
	int	RANGE_INPUT6	;
	int	RANGE_INPUT7	;
	int	RANGE_INPUT8	;
	int	FILTER_INPUT1	;
	int	FILTER_INPUT2	;
	int	FILTER_INPUT3	;
	int	FILTER_INPUT4	;
	int	FILTER_INPUT5	;
	int	FILTER_INPUT6	;
	int	FILTER_INPUT7	;
	int	FILTER_INPUT8	;
};


