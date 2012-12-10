
// T3000Doc.h : interface of the CT3000Doc class
//


#pragma once


class CT3000Doc : public CDocument
{
protected: // create from serialization only
	CT3000Doc();
	DECLARE_DYNCREATE(CT3000Doc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CT3000Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


