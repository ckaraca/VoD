#pragma once
#include "MainFrm.h"

#define RULERWINDOW_CLASSNAME  _T( "CRuler" )
// CRuler
	   
//Notify Message
#define  NM_RULER_NOTIFICATIONMESSAGE   WM_USER+0x103
#define  NMSUB_RULER_SEPERATORCHANGE    WM_USER+0x104
#define  NMSUB_RULER_SEPERATORCHANGING  WM_USER+0x105


//Seperator Types
#define SEPTYPE_NOPOSCHANGE    0x0001


class CRuler : public CWnd
{
// Construction				 
public:
	CRuler();

// Attributes
public:
	typedef struct _tagRULERWNDNOTIFY_INFO
	{
		NMHDR   hdr;
		UINT    nSubMessage;
		DWORD   dwRulerStyle;
		int     iSepID;
		int     iNewPos;
		int     iOldPos;
		int     iParam1;
		int     iParam2;
	}	RULERWNDNOTIFY_INFO;

	typedef struct _tagSEPERATOR_TYPE
	{
		int      iPos;
		int      iType;
		int      iID;
		COLORREF	clrLine;
		COLORREF	clrFill;
		int      iMinMargin;
		int      iMaxMargin;
		LPARAM lParam;  
	}	SEPERATOR_TYPE;


// Operations
public:
	SEPERATOR_TYPE* GetSeperator( int iID );
	int DeleteAllSeperator();
	int DeleteSeperator( int iID );
	int AddSeperator( int iPos , int iID , int iType = 0 , LPARAM lParam = NULL , COLORREF	clrLine = RGB( 0 , 0 , 0 ) , COLORREF clrFill = RGB( 255 ,255 , 220 ) , int iMinMargin = 0 , int iMaxMargin = 0xFFFFFFF );

	//Get Property
	COLORREF GetBackGroundColor() { return m_clrBackGround; }
	COLORREF GetMilimeterLineColor() { return m_clrMilimeterLineColor; }
	COLORREF GetTextColor() { return m_clrTextColor; }
	UINT     GetStartSeperateSize() { return m_nSeperateSize; }
	UINT     GetMargin() { return m_nRulerMargin; }
	UINT     GetMilimeterPixel() { return m_nMilimeterPixel; }
	UINT     GetSeperatorSize() { return m_nSeperatorSize; }
	long     GetScrollPos() { return m_lScrolPos; }
	CWnd*	 GetMessageTarget() { return m_pMessageTarget; }


	//Set Property
	BOOL     SetStyle( DWORD dwStyle );
	BOOL     SetBackGroundColor( COLORREF clr );
	BOOL     SetMilimeterLineColor( COLORREF clr );
	BOOL     SetTextColor( COLORREF clr );
	BOOL     SetStartSeperateSize( UINT nSize );
	BOOL     SetMargin( UINT nMargin );
	BOOL     SetMilimeterPixel( UINT nPixel );
	BOOL     SetSeperatorSize( UINT nSize );
	BOOL     SetScrollPos( long lPos );
	BOOL     SetMessageTarget( CWnd *pTarget = NULL );


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRuler)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	LRESULT SendParentNotifyMessage( UINT nSubMessage , int iSepID , int iNewPos , int iOldPos , int iParam1 = 0 , int iParam2 = 0 );
	int GetClientPixelFromPos( int iPos );
	int GetMostPos( CPoint point );
	int SeperatorHitTest( CPoint point );
	int IndHitTest( CPoint point );
	BOOL DraggingSeperator( int iDragID , CPoint point );
	BOOL Create( DWORD dwStyle , const RECT& rect ,  CWnd* pParentWnd = NULL ,  UINT nID = 0  );
	virtual ~CRuler();

	// Generated message map functions
protected:
	void SetCursorClipRect();
	BOOL DrawSeperator( CDC *pDC , SEPERATOR_TYPE *pSep );
	BOOL DrawRuler( CDC *pDC );


	COLORREF  m_clrBackGround;
	COLORREF  m_clrMilimeterLineColor;
	COLORREF  m_clrTextColor;
	UINT      m_nSeperateSize;
	UINT      m_nRulerMargin;
	UINT      m_nMilimeterPixel;
	UINT      m_nSeperatorSize;

	long      m_lScrolPos;

	CPtrArray m_Seperators;

	BOOL      m_bLButtonDown;
	BOOL      m_bDraging;
	int       m_iDragID;
	int       m_iDragPos;
    int       m_iOldDragingPos;

	CWnd      *m_pMessageTarget;

	RULERWNDNOTIFY_INFO m_NotifyInfo;

	BOOL RegisterWindowClass();
	//{{AFX_MSG(CRuler)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnEditindDeletethisindicator();
	afx_msg void OnAddnewindicatorAddinformationtothispoint();
	afx_msg void OnEditindEditthisinformation();
};


