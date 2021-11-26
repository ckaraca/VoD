// VoDTCPServerView.h : interface of the CVoDTCPServerView class
//


#pragma once
#include "TCPListener.h"

class CVoDTCPServerView : public CListView
{
protected: // create from serialization only
	CVoDTCPServerView();
	DECLARE_DYNCREATE(CVoDTCPServerView)

// Attributes
public:
	CVoDTCPServerDoc* GetDocument() const;

// Operations
public:
	int index;
	void SetList(TCHAR *Code,TCHAR *Event);
	void SetListSuccess(TCHAR *Success);
	LONG SetListEx(WPARAM wParam,LPARAM lParam);
	LONG SetListSuccessEx(WPARAM wParam,LPARAM /*lParam*/);
// Overrides
	public:
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CVoDTCPServerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in VoDTCPServerView.cpp
inline CVoDTCPServerDoc* CVoDTCPServerView::GetDocument() const
   { return reinterpret_cast<CVoDTCPServerDoc*>(m_pDocument); }
#endif

