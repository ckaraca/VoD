#pragma once


// CInfo dialog

class CInfo : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CInfo)

public:
	CInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInfo();
// Overrides
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// Dialog Data
	enum { IDD = IDD_INFO, IDH = IDR_HTML_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
