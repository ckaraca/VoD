#pragma once


// CWndSplitterEx

class CWndSplitterEx : public CSplitterWnd
{
	DECLARE_DYNAMIC(CWndSplitterEx)

public:
	CWndSplitterEx();
	virtual ~CWndSplitterEx();
	BOOL IsBarLocked(){return m_bBarLocked;}
	void LockBar(BOOL bState=TRUE){m_bBarLocked=bState;}
	BOOL ReplaceView(int row, int col,CRuntimeClass * pViewClass,SIZE size);
private:
	BOOL m_bBarLocked;

protected:
	DECLARE_MESSAGE_MAP()
};


