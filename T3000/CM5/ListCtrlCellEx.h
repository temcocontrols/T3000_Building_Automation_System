#pragma  once
#include <list>
using namespace std;
namespace ListCtrlEx
{
	
	typedef list<CString> CStrList;

	//////////////////////////////////////////////////////////////////////////
	class CInPlaceCombo : public CComboBox
	{
	public:
		static CInPlaceCombo* GetInstance(); 
		static void DeleteInstance(); 
		BOOL ShowComboCtrl(DWORD dwStyle, const CRect& rCellRect, CWnd* pParentWnd, UINT uiResourceID,
			int iRowIndex, int iColumnIndex, CStrList &lstDropDown, CString strCurSel = _T(""), int iCurSel = -1);
		virtual BOOL PreTranslateMessage(MSG* pMsg);

	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnKillFocus(CWnd* pNewWnd);
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg void OnCloseup();

		DECLARE_MESSAGE_MAP()
	private:
		CInPlaceCombo();
		CInPlaceCombo (CInPlaceCombo&) {}
		const CInPlaceCombo& operator = (CInPlaceCombo) { return *this; }
		virtual ~CInPlaceCombo();

		static CInPlaceCombo* m_pInPlaceCombo;
		int m_iRowIndex;
		int m_iColumnIndex;
		CString m_strWindowText;
		CStrList m_DropDownList;
		// To indicate whether ESC key was pressed
		BOOL m_bESC;
	};
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	class CInPlaceEdit : public CEdit
	{
	#define CTRL_C	0x3
	#define CTRL_V	0x16
	#define CTRL_X	0x18
	public:
		static CInPlaceEdit* GetInstance(); 
		static void DeleteInstance(); 
		BOOL ShowEditCtrl(DWORD dwStyle, const RECT& rCellRect, CWnd* pParentWnd, 
			UINT uiResourceID, int iRowIndex, int iColumnIndex,
			CString& strValidChars, CString& rstrCurSelection);
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		void SetEditLimit(int ncount);
	protected:	
		// Generated message map functions
		//{{AFX_MSG(CInPlaceEdit)
		afx_msg void OnKillFocus(CWnd* pNewWnd);
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg LRESULT OnPaste(WPARAM wParam, LPARAM lParam);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()

	private:
		CInPlaceEdit();
		CInPlaceEdit (CInPlaceEdit&) {}
		const CInPlaceEdit& operator = (CInPlaceEdit) { return *this; }
		virtual ~CInPlaceEdit();
		
		static CInPlaceEdit* m_pInPlaceEdit;
		int m_iRowIndex;
		int m_iColumnIndex;
		CString m_strValidChars;
		CString m_strWindowText;
		// To indicate whether ESC key was pressed
		BOOL m_bESC;
		int m_charlimit;
	};

	//////////////////////////////////////////////////////////////////////////
	// CSortHeadCtrl
	class  CListCtrlExSortHead : public CHeaderCtrl
	{
		DECLARE_DYNAMIC(CListCtrlExSortHead)

	public:
		CListCtrlExSortHead();

		virtual ~CListCtrlExSortHead();

		void			SetSortArrow( const int nSortColumn, const BOOL bDesc );
		inline int		GetCrntSortCol() const;
		inline BOOL	GetCrntSortDirection() const;
	protected:
		int m_iSortColumn;
		BOOL m_bSortDesc;
		DECLARE_MESSAGE_MAP()
		afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
		virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
		
	};


}

//////////////////////////////////////////////////////////////////////////
