#pragma once


// CGridButton

class CGridButton : public CButton
{
	DECLARE_DYNAMIC(CGridButton)

public:
	CGridButton();
	virtual ~CGridButton();

	void SetPosition(int nRow, int nCol);
	void SetPosition(CSize szPos);
	CSize GetPosition()const;
protected:
	DECLARE_MESSAGE_MAP()



	int				m_nRow;
	int				m_nCol;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};


