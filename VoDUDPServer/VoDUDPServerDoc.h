// VoDUDPServerDoc.h : interface of the CVoDUDPServerDoc class
//


#pragma once

class CVoDUDPServerDoc : public CDocument
{
protected: // create from serialization only
	CVoDUDPServerDoc();
	DECLARE_DYNCREATE(CVoDUDPServerDoc)

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
	virtual ~CVoDUDPServerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


