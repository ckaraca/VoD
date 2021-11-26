#pragma once



// CHtml html view

class CHtml : public CHtmlView
{
	DECLARE_DYNCREATE(CHtml)

protected:
	CHtml();           // protected constructor used by dynamic creation
	virtual ~CHtml();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	LONG SetFile(WPARAM wParam,LPARAM lParam);
};


