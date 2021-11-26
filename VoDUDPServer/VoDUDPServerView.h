// VoDUDPServerView.h : interface of the CVoDUDPServerView class
//


#pragma once


class CVoDUDPServerView : public CListView
{
protected: // create from serialization only
	CVoDUDPServerView();
	DECLARE_DYNCREATE(CVoDUDPServerView)

// Attributes
public:
	CVoDUDPServerDoc* GetDocument() const;
	CWinThread *Udp;
// Operations
public:
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
	virtual ~CVoDUDPServerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	int index;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnTimer(UINT nIDEvent);
};

#ifndef _DEBUG  // debug version in VoDUDPServerView.cpp
inline CVoDUDPServerDoc* CVoDUDPServerView::GetDocument() const
   { return reinterpret_cast<CVoDUDPServerDoc*>(m_pDocument); }
#endif

