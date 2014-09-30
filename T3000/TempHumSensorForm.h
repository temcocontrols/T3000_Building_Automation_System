#pragma once


// CTempHumSensorForm dialog

class CTempHumSensorForm : public CDialogEx
{
	DECLARE_DYNAMIC(CTempHumSensorForm)

public:
	CTempHumSensorForm(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTempHumSensorForm();

// Dialog Data
	enum { IDD = IDD_DIALOG_HUM_TEMP_SENSOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
 

};
