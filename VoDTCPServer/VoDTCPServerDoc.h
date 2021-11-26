// VoDTCPServerDoc.h : interface of the CVoDTCPServerDoc class
//


#pragma once

class CVoDTCPServerDoc : public CDocument
{
protected: // create from serialization only
	CVoDTCPServerDoc();
	DECLARE_DYNCREATE(CVoDTCPServerDoc)

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
	virtual ~CVoDTCPServerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


