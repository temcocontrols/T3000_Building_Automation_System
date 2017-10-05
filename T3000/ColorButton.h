
#ifndef _color_button_
#define _color_button_

/////////////////////////////////////////////////////////////////////////////
// CColorButton window

class CColorButton : public CButton
{
  const COLORREF *prgb_color_;
  int dx_;
  int dy_;

  void PaintIt(int offset);

  // Construction
public:
  CColorButton();

  // Attributes
public:
  void SetColorRef(const COLORREF *color);
  afx_msg LRESULT OnSetState(WPARAM wParam, LPARAM /* lParam */);

  // Operations
public:
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CColorButton)
  //}}AFX_VIRTUAL
  
  // Implementation
public:
  virtual ~CColorButton();
  
  // Generated message map functions
protected:
  //{{AFX_MSG(CColorButton)
  afx_msg void OnPaint();
	//}}AFX_MSG
  
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif
