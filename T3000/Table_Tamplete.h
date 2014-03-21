#pragma once

#include "msflexgrid1.h"
// CTable_Tamplete dialog

class CTable_Tamplete : public CDialog
{
	DECLARE_DYNAMIC(CTable_Tamplete)

public:
	CTable_Tamplete(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTable_Tamplete();

// Dialog Data
	enum { IDD = IDD_TABLE_TEMPLETE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CMsflexgrid m_msgrid;
};
