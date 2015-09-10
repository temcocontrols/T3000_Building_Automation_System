#pragma once
#include "afxwin.h"
#include "CM5/ListCtrlEx.h"
#include "CM5/CStatic/staticex.h"


// CBoatMonitorViewer form view

class CBoatMonitorViewer : public CFormView
{
	DECLARE_DYNCREATE(CBoatMonitorViewer)

public:
	CBoatMonitorViewer();           // protected constructor used by dynamic creation
	virtual ~CBoatMonitorViewer();

public:
	enum { IDD = IDD_DIALOG_BOATMONITOR_VIEW };
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
CComboBox m_combox_zigbee;
CComboBox m_combox_channel;
CComboBox m_combox_brandrate;
ListCtrlEx::CListCtrlEx m_zigbee_list;
void Initial_List();
};


