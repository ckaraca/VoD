#pragma once


// Empty form view

class Empty : public CFormView
{
	DECLARE_DYNCREATE(Empty)

protected:
	Empty();           // protected constructor used by dynamic creation
	virtual ~Empty();

public:
	enum { IDD = IDD_EMPTY };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};


