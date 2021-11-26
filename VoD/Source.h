#pragma once



// Source view

class CSource : public CEditView
{
	DECLARE_DYNCREATE(CSource)
 
protected:
	CSource();           // protected constructor used by dynamic creation
	virtual ~CSource();
	CFont m_font;

public:
	void UpdateView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnEnChange();
};


