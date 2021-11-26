#pragma once
#include "afxwin.h"
#include "Sql.h"

// CProject dialog
extern CSql Log;
typedef struct _tree {
	int id;
	TCHAR name[50];
	TV_INSERTSTRUCT tvis;
	HTREEITEM hWnd;
	TCHAR DBNAME[50];
	TCHAR DBID[50];
	struct _tree *next;
	struct _tree *prev;
} TREE;

typedef struct _MTS { // Message to send to owner window
	TCHAR DBID[100];
} MTS;

class CProject : public CDialog
{
	DECLARE_DYNAMIC(CProject)

public:
	CProject(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProject();

// Dialog Data
	enum { IDD = IDD_PROJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void TVAddItem(HTREEITEM iparent,TCHAR *itext,TREE*,int id);
	void TVAddItem(HTREEITEM iparent,TCHAR *itext,TREE *last,int id,TCHAR *RealName);
	void TVAddItem(HTREEITEM iparent,TCHAR *itext,TREE *last,int id,TCHAR *RealName,TCHAR *RealID);
	unsigned int list;
	void InitTree(void);
	CTreeCtrl m_Tree;
	TREE *temp,*Tree,*nT;
	HTREEITEM hItem;
	TCHAR name[1024];
	MTS *mts;
	DECLARE_MESSAGE_MAP()
public:
	CButton m_Select;
	afx_msg void OnBnCancel();
	virtual BOOL OnInitDialog();
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnBnProject();
protected:
	virtual void OnCancel();
};



