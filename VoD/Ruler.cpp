// Ruler.cpp : implementation file
//

#include "stdafx.h"
#include "VoD.h"
#include "Ruler.h"
#include <math.h>
#include "MemDC.h"
#include "VoDView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRuler
CRuler::CRuler()
{
	m_bLButtonDown   = m_bDraging = FALSE;
	m_iDragID	    = -1;
	m_iDragPos		= -1;
	m_iOldDragingPos = -1;

	m_nSeperateSize   = 10;
	m_nSeperatorSize	 = 5;
	m_nRulerMargin    = 500;
	m_lScrolPos	     = 0;
	m_nMilimeterPixel = 4;

	m_clrBackGround         = GetSysColor( COLOR_3DFACE );
	m_clrMilimeterLineColor = RGB( 0 , 0 , 0 );
	m_clrTextColor          = RGB( 0 , 0 , 0 );

	m_pMessageTarget        = NULL;

	RegisterWindowClass();
	DeleteAllSeperator();
}


CRuler::~CRuler()
{
	DeleteAllSeperator();
	DestroyWindow();
	WNDCLASS wndcls = { 0 };
	if(( ::GetClassInfo( AfxGetResourceHandle() , RULERWINDOW_CLASSNAME , &wndcls ) ) )
	{
		::UnregisterClass( wndcls.lpszClassName , AfxGetResourceHandle() );
	}
}


BEGIN_MESSAGE_MAP(CRuler, CWnd)
	//{{AFX_MSG_MAP(CRuler)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDITIND_DELETETHISINDICATOR, OnEditindDeletethisindicator)
	ON_COMMAND(ID_ADDNEWINDICATOR_ADDINFORMATIONTOTHISPOINT, OnAddnewindicatorAddinformationtothispoint)
	ON_COMMAND(ID_EDITIND_EDITTHISINFORMATION, OnEditindEditthisinformation)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRuler message handlers
BOOL CRuler::RegisterWindowClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetResourceHandle();
	if ( !( ::GetClassInfo( hInst, RULERWINDOW_CLASSNAME , &wndcls ) ) )
    {
		// otherwise we need to register a new class
		wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc      = ::DefWindowProc;
		wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
		wndcls.hInstance        = hInst;
		wndcls.hIcon            = NULL;
#ifndef _WIN32_WCE_NO_CURSOR
		wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
#else
		wndcls.hCursor          = 0;
#endif										
		wndcls.hbrBackground    = (HBRUSH)( COLOR_3DFACE + 1 );
		wndcls.lpszMenuName     = NULL;
		wndcls.lpszClassName    = RULERWINDOW_CLASSNAME;
		if( !AfxRegisterClass( &wndcls ) )
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CRuler::Create( DWORD dwStyle, const RECT &rect, CWnd *pParentWnd , UINT nID )
{
	ASSERT(pParentWnd->GetSafeHwnd());
	if (!CWnd::Create( RULERWINDOW_CLASSNAME , NULL, dwStyle | WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID ) )
		return FALSE;
	return TRUE;
}

BOOL CRuler::OnEraseBkgnd(CDC* pDC) 
{
	CMemDC memDC( pDC );
	CRect rect;
	GetClientRect( &rect );
	memDC.FillSolidRect( &rect , m_clrBackGround );
	memDC.Draw3dRect( &rect , RGB( 230 , 230 , 230 ) , RGB( 100 ,100 ,100 ) );
	DrawRuler( &memDC );
	return TRUE;
}

void CRuler::PreSubclassWindow() 
{
	CWnd::PreSubclassWindow();
	ModifyStyleEx( 0, WS_EX_CLIENTEDGE | /*WS_EX_DLGMODALFRAME |*/ SWP_DRAWFRAME  );
	SetWindowPos( NULL ,  0 ,0 , 0 , 0 , SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER );
}


BOOL CRuler::DrawRuler(CDC *pDC)
{
	CRect rectRuler;
	GetClientRect( &rectRuler );
	CPen pen;
	pen.CreatePen( PS_SOLID , 1 , m_clrMilimeterLineColor );
	CPen *pOldPen = pDC->SelectObject( &pen );
	int iStartPos = m_nSeperateSize;  
	CFont font;
	font.CreatePointFont( 70 , _T( "Tahoma" ) );
	CRect rectText;
	int iLLineStart , iLLineEnd;
	int iMLineStart , iMLineEnd;
	int iSLineStart , iSLineEnd;
	
	rectText.top     = rectRuler.bottom - 40;
	rectText.bottom  = rectRuler.bottom + 30;
	
	iLLineStart = rectRuler.bottom - 19;
	iLLineEnd	 = rectRuler.bottom - 2;
	
	iMLineStart = rectRuler.bottom - 10;
	iMLineEnd	 = rectRuler.bottom - 2;

	iSLineStart = rectRuler.bottom - 7;
	iSLineEnd	 = rectRuler.bottom - 2;
	
	CFont *pOldFont = pDC->SelectObject( &font );
	CString str;  
	int iPos;
	int iSel = 0;

	pDC->SetBkMode( TRANSPARENT );
	pDC->SetTextColor( m_clrTextColor );

	for( unsigned iSayac = 0 ; iSayac <= m_nRulerMargin; iSayac++ ) 
	{
		iPos = iStartPos + ( iSayac * m_nMilimeterPixel );
		iPos -= m_lScrolPos;
		if( iPos < rectRuler.left )
		{
			iSel++;
			if( iSel == 10 )
				iSel = 0;
			continue;
		}
		if( iPos > rectRuler.right )
		break;
		if( iSel == 0 )
		{
			rectText.left     = iPos - 20;
			rectText.right    = iPos + 22;
			str.Format( _T( "%ld" ) , iSayac / 10 );
			pDC->DrawText( str , &rectText , DT_CENTER /*| DT_VCENTER | DT_SINGLELINE*/ );
			pDC->MoveTo(  iPos , iLLineStart );
			pDC->LineTo(  iPos , iLLineEnd   );
		}
		else
		if( iSel == 5 ) 
		{
			pDC->MoveTo( iPos , iMLineStart );
			pDC->LineTo( iPos , iMLineEnd   );
		}
		else 
		{
			pDC->MoveTo( iPos , iSLineStart );
			pDC->LineTo( iPos , iSLineEnd   );
		}
		iSel++;
		if( iSel == 10 )
		iSel = 0;
	};
	SEPERATOR_TYPE *pSep = NULL;
	for( iSayac = 0 ; iSayac < (unsigned)m_Seperators.GetSize() ; iSayac++ )
	{
		pSep = (SEPERATOR_TYPE *)m_Seperators.GetAt( iSayac );
		if( pSep != NULL ) 
		{
			iPos = iStartPos + ( pSep->iPos * m_nMilimeterPixel );
			iPos -= m_lScrolPos;
			if( iPos < rectRuler.left - (int)m_nSeperatorSize )
			continue;
			if( iPos > rectRuler.right + (int)m_nSeperatorSize )
			continue;
			DrawSeperator( pDC , pSep ); 
		}
	}
	//pDC->MoveTo(rectRuler.right/6,rectRuler.top);
	//pDC->LineTo(rectRuler.right/6,rectRuler.bottom);
	pDC->SelectObject( pOldFont );
	font.DeleteObject();

	pDC->SelectObject( pOldPen );
	pen.DeleteObject();
	return TRUE;
}

CRuler::SEPERATOR_TYPE* CRuler::GetSeperator( int iID ) 
{
	SEPERATOR_TYPE *pSep = NULL;
	if( m_Seperators.GetSize() > 0 ) 
	{
		for( int iSayac = 0 ; iSayac < m_Seperators.GetSize() ; iSayac++ )
		{
			pSep = (SEPERATOR_TYPE *)m_Seperators.GetAt( iSayac );
			if( pSep != NULL && pSep->iID == iID ) 
			{
				return pSep;
			}
		}
	}
	return NULL;
}


int CRuler::DeleteAllSeperator() 
{
	SEPERATOR_TYPE *pSep = NULL;
	if( m_Seperators.GetSize() > 0 ) 
	{
		for( int iSayac = 0 ; iSayac < m_Seperators.GetSize() ; iSayac++ ) 
		{
			pSep = (SEPERATOR_TYPE *)m_Seperators.GetAt( iSayac );
			if( pSep != NULL )
				delete pSep;
		}
	}
	m_Seperators.RemoveAll();
	return 0;
}

int CRuler::DeleteSeperator( int iID )
{
	SEPERATOR_TYPE *pSep = NULL;
	if( m_Seperators.GetSize() > 0 )
	{
		for( int iSayac = 0 ; iSayac < m_Seperators.GetSize() ; iSayac++ )
		{
			pSep = (SEPERATOR_TYPE *)m_Seperators.GetAt( iSayac );
			if( pSep != NULL && pSep->iID == iID ) 
			{
				delete pSep;
				m_Seperators.RemoveAt( iSayac );
				return iSayac;
			}
		}
	}
	return -1;
}

int CRuler::AddSeperator( int iPos , int iID , int iType , LPARAM lParam , COLORREF	clrLine , COLORREF clrFill , int iMinMargin , int iMaxMargin )
{
	SEPERATOR_TYPE *pSep = new SEPERATOR_TYPE;
	if( pSep != NULL )
	{
		memset( pSep , 0 , sizeof( *pSep ) );
		pSep->iPos    = iPos;
		pSep->iID     = iID;
		pSep->iType   = iType;
		pSep->lParam  = lParam;
		pSep->clrLine = clrLine;
		pSep->clrFill = clrFill;
		pSep->iMinMargin = iMinMargin;
		pSep->iMaxMargin = iMaxMargin;
		return (int)m_Seperators.Add( pSep );
	}
	return -1;
}
								
BOOL CRuler::DrawSeperator( CDC *pDC , SEPERATOR_TYPE *pSep )
{
	if( pSep == NULL || pSep->iPos < 0 || pSep->iPos > (int)m_nRulerMargin )
		return FALSE;
	CRect rect , rectClient;
	GetClientRect( &rectClient );
	int iNewPos = m_nSeperateSize + ( pSep->iPos * m_nMilimeterPixel );
	iNewPos -= m_lScrolPos;
	CPen pen;
	pen.CreatePen( PS_SOLID , 1 , pSep->clrLine );
	CPen *pOldPen = pDC->SelectObject( &pen );
	rect.left    = iNewPos - m_nSeperatorSize;
	rect.right   = iNewPos + m_nSeperatorSize;
	rect.top    = rectClient.bottom -15 - ( ( m_nSeperatorSize * 2 ) + 2 ) ;
	rect.bottom = rectClient.bottom -15;
	pDC->FillSolidRect( &rect , pSep->clrFill );
	pDC->MoveTo( rect.left  , rect.top );
	pDC->LineTo( rect.right , rect.top );
	pDC->LineTo( rect.right , rect.bottom );
	pDC->LineTo( rect.left  , rect.bottom );
	pDC->LineTo( rect.left  , rect.top );
	
	pDC->MoveTo( rect.left  + 2  , rect.top + 2 );
	pDC->LineTo( rect.right - 2  , rect.top + 2 );
	pDC->LineTo( iNewPos         , rect.bottom - 2 );
	pDC->LineTo( rect.left + 2   , rect.top + 2 );
	pDC->SelectObject( pOldPen );
	return TRUE;
}

BOOL CRuler::SetBackGroundColor( COLORREF clr )
{ 
	m_clrBackGround = clr;
	return TRUE;
}

BOOL CRuler::SetMilimeterLineColor( COLORREF clr ) 
{ 
	m_clrMilimeterLineColor = clr;
	return TRUE;
}

BOOL CRuler::SetTextColor( COLORREF clr )
{ 
	m_clrTextColor = clr; 
	return TRUE;
}

BOOL CRuler::SetStartSeperateSize( UINT nSize )
{ 
	m_nSeperateSize = nSize; 
	return TRUE;
}

BOOL CRuler::SetMargin( UINT nMargin )
{ 
	m_nRulerMargin = nMargin; 
	return TRUE;
}

BOOL CRuler::SetMilimeterPixel( UINT nPixel )
{
	m_nMilimeterPixel = nPixel; 
	return TRUE;
}

BOOL CRuler::SetSeperatorSize( UINT nSize )
{ 
	m_nSeperatorSize = nSize; 
	return TRUE;
}

BOOL CRuler::SetScrollPos( long lPos )
{ 
	m_lScrolPos = lPos; 
	return TRUE;
}

BOOL CRuler::SetMessageTarget( CWnd *pTarget )
{
	m_pMessageTarget = pTarget;
	return TRUE;
}

void CRuler::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLButtonDown = TRUE;
	m_iDragID = SeperatorHitTest( point );
	if( m_iDragID != -1 ) 
	{
		SEPERATOR_TYPE *pSep = GetSeperator( m_iDragID );
		if( pSep != NULL )
			m_iDragPos = pSep->iPos;
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CRuler::OnMouseMove(UINT nFlags, CPoint point) 
{
	if( m_bLButtonDown && !m_bDraging && m_iDragID >= 0 )
	{
		m_bDraging = TRUE;
		SetCursorClipRect();
		m_iOldDragingPos = -1;
	}
	if( m_bDraging )
	{
        DraggingSeperator( m_iDragID , point );
	}
	CWnd::OnMouseMove(nFlags, point);
}

void CRuler::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ClipCursor( NULL );
	if( m_bDraging && m_iDragID >= 0 )
	{
		int  iNewPos = GetMostPos( point );
		if( iNewPos >= 0 )
		{
			SEPERATOR_TYPE *pSep = GetSeperator( m_iDragID );
			if( pSep != NULL && pSep->iPos != iNewPos )
			{
				SendParentNotifyMessage( NMSUB_RULER_SEPERATORCHANGE  , pSep->iID , iNewPos , pSep->iPos );
				pSep->iPos = iNewPos;
			}
		}
		Invalidate();
	};
	m_bLButtonDown = m_bDraging = FALSE;
	m_iDragPos = m_iDragID = -1;
	CWnd::OnLButtonUp(nFlags, point);
}

BOOL CRuler::DraggingSeperator( int iDragID , CPoint point )
{
	CDC *pDC = GetDC();
	if( pDC == NULL )
		return FALSE;
	CRect rect;
	GetClientRect( &rect );
	int iNewPos = 0;
	int iPos = GetMostPos( point );
	if( iPos != m_iOldDragingPos )
	{
        if( m_iOldDragingPos != -1 )
		{	
			iNewPos = m_nSeperateSize + ( m_iOldDragingPos * m_nMilimeterPixel );
			iNewPos -= m_lScrolPos;
			rect.left    = iNewPos;
			rect.right   = iNewPos + 1;
			pDC->InvertRect( &rect );
		}
		if( iPos != -1 )
		{
			SendParentNotifyMessage( NMSUB_RULER_SEPERATORCHANGING  , iDragID , iPos , m_iDragPos );
			m_iOldDragingPos = iPos;
			iNewPos = m_nSeperateSize + ( iPos * m_nMilimeterPixel );
			iNewPos -= m_lScrolPos;
			rect.left    = iNewPos;
			rect.right   = iNewPos + 1;
			pDC->InvertRect( &rect );
		}
	}
	return TRUE;
}



int CRuler::SeperatorHitTest(CPoint point)
{
	CRect rect , rectClient;
	GetClientRect( &rectClient );
	int iNewPos = 0;
	SEPERATOR_TYPE *pSep = NULL;
	for( int iSayac = 0 ; iSayac < m_Seperators.GetSize() ; iSayac++ ) 
	{
		pSep = (SEPERATOR_TYPE *)m_Seperators.GetAt( iSayac );
		if( pSep != NULL && !( pSep->iType & SEPTYPE_NOPOSCHANGE ) )
		{
			iNewPos = m_nSeperateSize + ( pSep->iPos * m_nMilimeterPixel );
			iNewPos -= m_lScrolPos;
			rect.left    = iNewPos - m_nSeperatorSize;
			rect.right   = iNewPos + m_nSeperatorSize;
			if( rect.right < rectClient.left )
				continue;
			if( rect.left > rectClient.right )
				continue;
			rect.top    = rectClient.bottom - 15 -( ( m_nSeperatorSize * 2 ) + 2 ) ;
			rect.bottom = rectClient.bottom - 15;
			if( rect.PtInRect( point ) && pSep->iPos >= 0 && pSep->iPos <= (int)m_nRulerMargin )
			return pSep->iID;
		}
	}	
	return -1;
}

int CRuler::GetMostPos( CPoint point )
{
	double dfToplam , dfKesir;	
	int    iNewPos  = -1;
	iNewPos = ( point.x - m_nSeperateSize ) + m_lScrolPos;
	dfToplam = (double)iNewPos / (double)m_nMilimeterPixel;
	iNewPos  = iNewPos / m_nMilimeterPixel;
	dfKesir = modf( dfToplam , &dfToplam );
	if( dfKesir >= 0.5 )
		iNewPos++;
	if( iNewPos < 0 || iNewPos > (int)m_nRulerMargin )
		return -1;
	return iNewPos;	
}

void CRuler::SetCursorClipRect()
{
	CRect rect;
	GetClientRect( &rect );
	int iMinMargin = 0;
	int iMaxMargin = 0xFFFFFFF;
	if( m_iDragID != -1 )
	{
		SEPERATOR_TYPE *pSep = GetSeperator( m_iDragID );
		if( pSep != NULL )
		{
			iMinMargin = pSep->iMinMargin;
			iMaxMargin = pSep->iMaxMargin;
		}
	}
	int iLeft    = m_nSeperateSize;
	int iRight   = m_nSeperateSize + ( m_nRulerMargin * m_nMilimeterPixel );
	int iMinRange = m_nSeperateSize + ( iMinMargin * m_nMilimeterPixel );
	int iMaxRange = m_nSeperateSize + ( iMaxMargin * m_nMilimeterPixel );
	iLeft   -= m_lScrolPos;
	iRight  -= m_lScrolPos;
	iMinRange   -= m_lScrolPos;
	iMaxRange   -= m_lScrolPos;
	if( rect.left < iLeft )
		rect.left  = iLeft;
	if( rect.left < iMinRange )
		rect.left  = iMinRange;
	if( rect.right > iMaxRange )
		rect.right = iMaxRange;
	ClientToScreen( &rect );
	ClipCursor( &rect );
}

int CRuler::GetClientPixelFromPos( int iPos )
{
	int iClientPos = m_nSeperateSize + ( iPos * m_nMilimeterPixel );
	iClientPos -= m_lScrolPos;
	return iClientPos;
}

LRESULT CRuler::SendParentNotifyMessage( UINT nSubMessage, int iSepID , int iNewPos , int iOldPos , int iParam1 , int iParam2 )
{
	memset( &m_NotifyInfo , 0 , sizeof( m_NotifyInfo ) );
	//Header
	m_NotifyInfo.hdr.hwndFrom = m_hWnd;
	m_NotifyInfo.hdr.idFrom   = GetDlgCtrlID();
	m_NotifyInfo.hdr.code     = NM_RULER_NOTIFICATIONMESSAGE;
	//Data
	m_NotifyInfo.nSubMessage  = nSubMessage;
	m_NotifyInfo.iNewPos      = iNewPos;
	m_NotifyInfo.iOldPos      = iOldPos;
	m_NotifyInfo.iSepID       = iSepID;
	m_NotifyInfo.iParam1      = iParam1;
	m_NotifyInfo.iParam2      = iParam2;
	
	CWnd *pWnd = GetParent();
	if( m_pMessageTarget != NULL && IsWindow( m_pMessageTarget->m_hWnd ) )
		pWnd = m_pMessageTarget;
	if( pWnd != NULL && IsWindow( pWnd->m_hWnd ) )
		return pWnd->SendMessage( WM_NOTIFY , (WPARAM)m_NotifyInfo.hdr.idFrom , (LPARAM)&m_NotifyInfo );
	return -1;
}
void CRuler::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CVoDView * cF = (CVoDView*)GetParent();
	cF->SetPlay(0);
	cF->OnPlay();
	
	m_iDragID = IndHitTest( point );
	HMENU hMenu,hSubMenu;
	HINSTANCE hThis=AfxGetInstanceHandle( );
	if(m_iDragID==-1)
	{
		//r mouse button pressed outside an indicator
		hMenu = LoadMenu(hThis,(LPCTSTR)IDR_MENU7);
		hSubMenu = GetSubMenu(hMenu,0);
		TrackPopupMenu(hSubMenu,TPM_LEFTALIGN | TPM_RIGHTBUTTON ,
			point.x+10, point.y, 0,this->m_hWnd,NULL);
		DestroyMenu(hMenu); 
	}
	else
	{
		hMenu = LoadMenu(hThis,(LPCTSTR)IDR_MENU8);
		hSubMenu = GetSubMenu(hMenu,0);
		TrackPopupMenu(hSubMenu,TPM_LEFTALIGN | TPM_RIGHTBUTTON ,
			point.x+10, point.y, 0,this->m_hWnd,NULL);
		DestroyMenu(hMenu); 
	}
	//CVoDView::SetPlay(1);
}


int CRuler::IndHitTest(CPoint point)
{
	CRect rect , rectClient;
	GetClientRect( &rectClient );
	ScreenToClient(&point);
	int iNewPos = 0;
	SEPERATOR_TYPE *pSep = NULL;
	for( int iSayac = 0 ; iSayac < m_Seperators.GetSize() ; iSayac++ ) 
	{
		pSep = (SEPERATOR_TYPE *)m_Seperators.GetAt( iSayac );
		if( pSep != NULL )
		{
			iNewPos = m_nSeperateSize + ( pSep->iPos * m_nMilimeterPixel );
			iNewPos -= m_lScrolPos;
			rect.left    = iNewPos - m_nSeperatorSize;
			rect.right   = iNewPos + m_nSeperatorSize;
			if( rect.right < rectClient.left )
				continue;
			if( rect.left > rectClient.right )
				continue;
			rect.top    = rectClient.bottom - 15 -( ( m_nSeperatorSize * 2 ) + 2 ) ;
			rect.bottom = rectClient.bottom - 15;
			if( rect.PtInRect( point ))// && pSep->iPos >= 0 && pSep->iPos <= (int)m_nRulerMargin )
			return pSep->iID;
		}
	}	
	return -1;
}
void CRuler::OnEditindDeletethisindicator()
{
	if(m_iDragID==0)MessageBox("The starting page cannot be deleted!","Warning",MB_OK|MB_ICONWARNING);
	else DeleteSeperator(m_iDragID);
	Invalidate(TRUE);
}

void CRuler::OnAddnewindicatorAddinformationtothispoint()
{
	//	A new page adding request has come, prepare the new page now.
	// get the time
	CVoDView * cF = (CVoDView*)GetParent();
	
	
	cF->AddPage((int)GetScrollPos()/3,"",0);

}
#include "playvideo.h"

void CRuler::OnEditindEditthisinformation()
{
	CVoDView * cF = (CVoDView*)GetParent();
	cF->SetPage(cF->Pages[m_iDragID].PageName);
	// If the file is not seekable, the trackbar is disabled. 
	DWORD dwPosition = ((cF->Pages[m_iDragID].Frame)/10)+1; // = m_Slider.GetPos();

    // Update the position continuously.
	ULONGLONG nTotalMS = cF->g_rtTotalTime / 10000; // 100ns -> ms
	//int nSeconds = nTotalMS / 1000;
	
	HRESULT hr;
    REFERENCE_TIME rtNew = (cF->g_rtTotalTime * dwPosition) / (nTotalMS/1000);

    hr = pMS->SetPositions(&rtNew, AM_SEEKING_AbsolutePositioning,
                           NULL,   AM_SEEKING_NoPositioning);

       // Update the 'current position' string on the main dialog.
    pMC->Run();
	Sleep(10);
	pMC->Pause();
}
