#pragma once


// CDesign view

class CDesign : public CHtmlEditView
{
	DECLARE_DYNCREATE(CDesign)

protected:
	CDesign();           // protected constructor used by dynamic creation
	virtual ~CDesign();
	CToolBar    m_wndFormatBar;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_DHTMLEDITING_CMDMAP(CDesign)
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	HRESULT CDesign::NewDocument();
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
public:
	LONG SetFile(WPARAM wParam,LPARAM lParam);
	virtual BOOL DestroyWindow();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	virtual void OnQuit();
};


