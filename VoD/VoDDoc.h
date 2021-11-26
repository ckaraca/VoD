// VoDDoc.h : interface of the CVoDDoc class
//


#pragma once

class CVoDDoc : public CDocument
{
protected: // create from serialization only
	CVoDDoc();
	DECLARE_DYNCREATE(CVoDDoc)

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
	virtual ~CVoDDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLoginDisconnect();
};


