#pragma once
#include "afxwin.h"
#include "MainFrm.h"


// CFaculty dialog

class CFaculty : public CDialog 
{
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
	TCHAR name[255];
	CEdit m_Edit;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
#pragma once


// CDepartment dialog

class CDepartment : public CDialog
{
	DECLARE_DYNAMIC(CDepartment)

public:
	CDepartment(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDepartment();

// Dialog Data
	enum { IDD = IDD_NAME1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Edit;
};
#pragma once


#pragma once


// CCourse dialog

class CCourse : public CDialog
{
	DECLARE_DYNAMIC(CCourse)

public:
	CCourse(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCourse();

// Dialog Data
	enum { IDD = IDD_NAME2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// Course Code
	CEdit m_Code;
	// Course Name
	CEdit m_Name;
	// Course Instructor
	CEdit m_Instructor;
	// Course description
	CEdit m_Description;
};
#pragma once


#pragma once


// CProject dialog

class CProject : public CDialog
{
	DECLARE_DYNAMIC(CProject)

public:
	CProject(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProject();

// Dialog Data
	enum { IDD = IDD_NAME4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// Video locaiton
	CEdit m_Video;
	// Browse video locaiton
	CButton m_Browse;
	// Project Name
	CEdit m_ProjectName;
	// Project subject
	CEdit m_PrSubject;
	// Project Description
	CEdit m_PrDescription;
};
#pragma once


// CChapter dialog

class CChapter : public CDialog
{
	DECLARE_DYNAMIC(CChapter)

public:
	CChapter(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChapter();

// Dialog Data
	enum { IDD = IDD_NAME3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// Chapter name
	CEdit m_ChName;
	// Chapter Description
	CEdit m_ChDescription;
};
#pragma once


// CTreeWndEx view

class CTreeWndEx : public CTreeView
{
	DECLARE_DYNCREATE(CTreeWndEx)

protected:
	CTreeWndEx();           // protected constructor used by dynamic creation
	virtual ~CTreeWndEx();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


