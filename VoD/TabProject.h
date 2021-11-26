#pragma once
#include "TabPage.h"
#include "afxcmn.h"
#include "afxwin.h"


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
	BOOL isLocal;
	TCHAR Path[MAX_PATH];
	TCHAR DBID[100];
	int TabIndex;
} MTS;

class CFaculty;
class CDepartment;
class CCourse;
class CChapter;
class CProject;
// CTabProject form view

class CTabProject : public CTabPage
{
	DECLARE_DYNCREATE(CTabProject)
	CTabProject();           // protected constructor used by dynamic creation
	virtual ~CTabProject();

public:
	enum { IDD = IDD_TABPROJECT };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	MTS *mts;
	DECLARE_MESSAGE_MAP()
public:
	HTREEITEM hItem;
	CTreeCtrl m_Tree;
	TREE *temp,*Tree,*nT;
	virtual BOOL OnInitDialog();
	// Tree initilization
	void InitTree(void);
	unsigned int list;
	TCHAR  *AddS(TCHAR * sentence);
	void TVAddItem(HTREEITEM iparent,TCHAR *itext,TREE*,int id);
	void TVAddItem(HTREEITEM iparent,TCHAR *itext,TREE *last,int id,TCHAR *RealName);
	void TVAddItem(HTREEITEM iparent,TCHAR *itext,TREE *last,int id,TCHAR *RealName,TCHAR *RealID);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	int ContentServer;
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	BOOL Created;
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnAdd1();
	afx_msg void OnAdd2();
	afx_msg void OnAdd3();
	afx_msg void OnAdd4();
	afx_msg void OnAdd5();
	void Warn(TCHAR *);
	void GetFac(CFaculty& Fac); 
	void GetDep(CDepartment& Dep);
	void GetCou(CCourse& Cou);
	void GetCha(CChapter& Cha);
	void GetPro(CProject& Pro);
	TCHAR name[255],m_ID[100],m_Inst[100],m_Desc[1024],m_FTP[100];
	CString mn_VideoName;
	TCHAR Path[MAX_PATH+100];
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMenuEditthisproject();
};


// CFaculty dialog

class CFaculty : public CDialog
{
	friend void CTabProject::GetFac(CFaculty& Fac);
	DECLARE_DYNAMIC(CFaculty)
public:
	CFaculty(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFaculty();
	
// Dialog Data
	enum { IDD = IDD_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Faculty;
	TCHAR name[255];
protected:
	virtual void OnOK();
	
};


// CDepartment dialog

class CDepartment : public CDialog
{
	DECLARE_DYNAMIC(CDepartment)
	friend void CTabProject::GetDep(CDepartment& Dep);

public:
	CDepartment(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDepartment();

// Dialog Data
	enum { IDD = IDD_NAME1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_DepName;
	TCHAR name[255];
protected:
	virtual void OnOK();
};
#pragma once


// CCourse dialog

class CCourse : public CDialog
{
	DECLARE_DYNAMIC(CCourse)
	friend void CTabProject::GetCou(CCourse& Cou);

public:
	CCourse(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCourse();
	TCHAR Code[100],Name[100],Inst[100],Desc[1024];
// Dialog Data
	enum { IDD = IDD_NAME2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Code;
	CEdit m_Name;
	CEdit m_Inst;
	CEdit m_Desc;
protected:
	virtual void OnOK();
};
#pragma once


// CChapter dialog

class CChapter : public CDialog
{
	DECLARE_DYNAMIC(CChapter)
	friend void CTabProject::GetCha(CChapter& Cha);
public:
	CChapter(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChapter();
	TCHAR Name[100],Desc[1024];

// Dialog Data
	enum { IDD = IDD_NAME3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Name;
	CEdit m_Desc;
protected:
	virtual void OnOK();
public:
//	virtual BOOL OnInitDialog();
};
#pragma once


// CProject dialog

class CProject : public CDialog
{
	friend void CTabProject::GetPro(CProject& Fac);
	DECLARE_DYNAMIC(CProject)

public:
	TCHAR BrEdit[MAX_PATH],Name[256],Subject[100],Description[800],desc[200],FTP[100];
	CProject(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProject();

// Dialog Data
	enum { IDD = IDD_NAME4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_BrEdit;
	CButton m_Br;
	CString m_VideoName;
	CEdit m_Name;
	CEdit m_Subject;
	CEdit m_Description;
	CButton m_IDOK;
	afx_msg void OnBrowse();
	int ContentServer;
protected:
	virtual void OnOK();
public:
	TCHAR Path[MAX_PATH];
	virtual BOOL OnInitDialog();
	CComboBox m_combo;
	afx_msg void OnCbnSelchangeCombo3();
};
