// CSlider Class 
// Copyright 2003 Cem KARACA
// Some parts are adapted from
// CMacProgressCtrl class - Paul M. Meidinger (pmmeidinger@yahoo.com)
// 

#include "stdafx.h"
#include "Slider.h"

IMPLEMENT_DYNAMIC(CSlider, CSliderCtrl)
CSlider::CSlider()
: m_Blink(false)
, Set(false)
{
	intPos = 0;
	InSide = FALSE;
	Selected = FALSE;
	InRect = RGB(255,170,0);
	OutRect = RGB(51,220,25);
	Pressed = RGB(255,255,0);
	Progress = RGB(102,102,204);
	SetColor(Progress);
}

void CSlider::SetColor(COLORREF crColor)
{
	m_crColor = crColor;
	GetColors();
	CreatePens();
}	

CSlider::~CSlider()
{
	DeletePens();
	//KillTimer(56);
}


BEGIN_MESSAGE_MAP(CSlider, CSliderCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
//ON_WM_CTLCOLOR_REFLECT()
ON_WM_LBUTTONUP()
ON_WM_ERASEBKGND()
ON_WM_TIMER()
END_MESSAGE_MAP()


void CSlider::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
//	pNMHDR->hwndFrom 
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	//CDC *dC = CDC::FromHandle(pNMCD->hdc);
	//if(pNMHDR->hwndFrom = this->m_hWnd)
	//{
	switch(pNMCD->dwDrawStage)
	{
		case CDDS_PREPAINT:
			*pResult = CDRF_NOTIFYITEMDRAW;
		break;
		case CDDS_POSTPAINT:
			*pResult = CDRF_DODEFAULT;
			break;
		case CDDS_POSTERASE:
			*pResult = CDRF_SKIPDEFAULT;
			break;
		case CDDS_PREERASE:
			*pResult = CDRF_SKIPDEFAULT;
			//*pResult = CDRF_NOTIFYITEMDRAW;
			break;

		case CDDS_ITEMPREPAINT:
			switch(pNMCD->dwItemSpec)
			{
				case TBCD_TICS:
					SetCTick(pNMCD);
					*pResult = CDRF_SKIPDEFAULT;
					break;
				case TBCD_CHANNEL:
					DrawBorder(pNMCD->hdc);
					Channel(pNMCD);
					*pResult = CDRF_SKIPDEFAULT;
					break;
				case TBCD_THUMB:
					Thumb(pNMCD);
					*pResult = CDRF_SKIPDEFAULT;
				break;
				default:
					*pResult = CDRF_DODEFAULT;
					break;

			}
			break;
	}//}
}

void CSlider::DrawBorder(HDC hDC)
{
	SetColor(Progress);
	RECT border;
	GetClientRect(&border);
	CDC *dC = CDC::FromHandle(hDC);
	DrawSlider(dC,&border);
}

extern int Cnt,ProRange,ProPos;

void CSlider::SetProRange(int /*Lower*/, int nUpper)
{
	ProRange = nUpper;
}
void CSlider::GetProRange(int &nLower, int &nUpper) const
{
	nLower = 0;
	nUpper = ProRange;
}

void CSlider::GlbTicCnt()
{
	Cnt = (const int)this->SendMessage(TBM_GETNUMTICS, 0, 0) - 2;
}


void CSlider::SetCTick(LPNMCUSTOMDRAW pNMCD)
{
	RECT RThumb = {0};
	this->SendMessage(TBM_GETTHUMBRECT, 0,reinterpret_cast<LPARAM>(&RThumb));
	const int num_ticks = (const int)this->SendMessage(TBM_GETNUMTICS, 0, 0) - 2;
	for (int iTick = 0; iTick < Cnt; ++iTick)
	{
		const int x_pos =(const int)this->SendMessage(TBM_GETTICPOS, iTick, 0);
		Ellipse(pNMCD->hdc, x_pos - 2, RThumb.top - 6,
            x_pos + 2, RThumb.top);
	}
	CBrush brushBlue(RGB(0, 0, 255));
	CDC *dC = CDC::FromHandle(pNMCD->hdc);
	CBrush* pOldBrush = dC->SelectObject(&brushBlue);
	for (int iTick = Cnt; iTick < num_ticks; ++iTick)
	{
		const int x_pos =(const int)this->SendMessage(TBM_GETTICPOS, iTick, 0);
		Rectangle(pNMCD->hdc, x_pos - 2, RThumb.top - 6,
            x_pos + 2, RThumb.top);
	}
	dC->SelectObject(pOldBrush);	
}

void CSlider::Thumb(LPNMCUSTOMDRAW pNMCD)
{
	CRect crect,crect1;
	GetThumbRect(&crect);
	COLORREF col;
	
	if (pNMCD->uItemState & CDIS_SELECTED)
	{
		col = Pressed;
		SetColor(col);
		Selected = TRUE;
	}
	else
	{
		col = OutRect;
		Selected = FALSE;
		SetColor(col);
	}
	CDC *dC = CDC::FromHandle(pNMCD->hdc);
	DrawSlider(dC,&crect);
	if(!InSide)SetColor(Progress); 
}

BOOL CSlider::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CRect rc;
	GetThumbRect(&rc);
	COLORREF col;
	CDC *dC = GetDC();
	if(nHitTest==HTCLIENT && rc.PtInRect(p))
	{
		col = InRect;
		if(!Selected) SetColor(col);
		DrawSlider(dC,&rc);
				
	}else if(nHitTest==HTCLIENT && !rc.PtInRect(p))
	{
		col = OutRect;
		SetColor(col);
		DrawSlider(dC,&rc);
	}
	SetColor(Progress);
	return CSliderCtrl::OnSetCursor(pWnd, nHitTest, message);
}

// extern int Cnt,ProRange,ProPos;
void CSlider::Channel(LPNMCUSTOMDRAW pNMCD)
{
	/*CRect crect;
	crect.CopyRect(&pNMCD->rc);
	crect.InflateRect(3, 5, 3, 5);
	DrawEdge(pNMCD->hdc, &crect, EDGE_SUNKEN, BF_RECT | BF_ADJUST);
	crect.CopyRect(&pNMCD->rc);
	crect.InflateRect(0, 3, 0, 3);
	double rr = crect.right;
	double right;
	if(ProRange!=0){ right= ((rr*ProPos)/ProRange);} else right=4;
	crect.right =(LONG)right;
	crect.left = 4;
	DrawProgress(pNMCD->hdc,&crect); */
	CRect crect;
	double rr;
	//CDC *pDC = CDC::FromHandle(pNMCD->hdc);
	crect.CopyRect(&pNMCD->rc);
	crect.InflateRect(0, 5, 0, 5);
	DrawEdge(pNMCD->hdc, &crect, EDGE_SUNKEN, BF_RECT | BF_ADJUST);
	crect.InflateRect(0, 1, 0, 1);
    if(ProRange!=0)
	{
		rr= (crect.Width()*ProPos)/ProRange;
		crect.right =(LONG)rr+10;
	} else crect.right=9;
	crect.left = 9;
	DrawProgress(pNMCD->hdc,&crect);
}

void CSlider::DrawProgress(HDC hDC, const CRect rect)
{
	SetColor(Progress);
	if (!rect.Width())
		return;
	int nLeft = rect.left;
	int nTop = rect.top;
	int nBottom = rect.bottom;
	CDC *pDC = CDC::FromHandle(hDC);
	CPen *pOldPen = pDC->SelectObject(&m_penColorLight);
	int nRight = rect.right;

	pDC->MoveTo(nLeft + 2, nBottom - 4);
	pDC->LineTo(nRight - 2, nBottom - 4);
	pDC->MoveTo(nLeft + 2, nTop + 2);
	pDC->LineTo(nRight - 2, nTop + 2);
	pDC->SetPixel(nLeft + 1, nBottom - 3, m_crColorLight);
	pDC->SetPixel(nLeft + 1, nTop + 1, m_crColorLight);
	pDC->SelectObject(&m_penColorLighter);
	pDC->MoveTo(nLeft + 2, nBottom - 5);
	pDC->LineTo(nRight - 3, nBottom - 5);
	pDC->LineTo(nRight - 3, nTop + 3);
	pDC->LineTo(nLeft + 1, nTop + 3);
	pDC->SetPixel(nLeft + 1, nBottom - 4, m_crColorLighter);
	pDC->SetPixel(nLeft + 1, nTop + 2, m_crColorLighter);
	pDC->SelectObject(&m_penColor);
	pDC->MoveTo(nLeft, nBottom - 1);
	pDC->LineTo(nLeft, nTop);
	pDC->LineTo(nLeft + 2, nTop);
	pDC->SetPixel(nLeft + 1, nBottom - 2, m_crColor);
	pDC->MoveTo(nLeft + 2, nBottom - 3);
	pDC->LineTo(nRight - 2, nBottom - 3);
	pDC->MoveTo(nLeft + 2, nTop + 1);
	pDC->LineTo(nRight - 1, nTop + 1);
	
	pDC->SelectObject(&m_penColorDark);
	pDC->MoveTo(nLeft + 2, nBottom - 2);
	pDC->LineTo(nRight - 2, nBottom - 2);
	pDC->LineTo(nRight - 2, nTop + 1);
	pDC->MoveTo(nLeft + 2, nTop);
	pDC->LineTo(nRight, nTop);
	pDC->SetPixel(nLeft + 1, nBottom - 1, m_crColorDark);
	pDC->SelectObject(&m_penColorDarker);
	pDC->MoveTo(nLeft + 2, nBottom - 1);
	pDC->LineTo(nRight - 1, nBottom - 1);
	pDC->LineTo(nRight - 1, nTop);
	pDC->SelectObject(&m_penShadow);
	pDC->MoveTo(nRight, nTop);
	pDC->LineTo(nRight, nBottom);
	pDC->SelectObject(&m_penLiteShadow);
	pDC->MoveTo(nRight + 1, nTop);
	pDC->LineTo(nRight + 1, nBottom);

	pDC->SelectObject(pOldPen);
}	


void CSlider::SetProgressPos(int pos)
{
	ProPos = pos;
	int min,max;
	GetRange(min,max);
	CSliderCtrl::SetRange(min,max,TRUE);
}

void CSlider::OnMouseMove(UINT nFlags, CPoint point)
{
	p = point;
	CRect rect;
	GetThumbRect(&rect);
	if(!rect.PtInRect(point))
	{
		Selected = FALSE;
		InSide = FALSE;
		
	} else { InSide = TRUE; }
	CSliderCtrl::OnMouseMove(nFlags, point);
}




void CSlider::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetThumbRect(&rect);
	rect.InflateRect(2,2,2,2);
	if(rect.PtInRect(point))
	{
		Selected = TRUE;
		SetColor(Pressed);
		
	}
	else 
	{
		Selected = FALSE;
		SetColor(OutRect);
		
	}
	if(Set) return; else
	CSliderCtrl::OnLButtonDown(nFlags, point);
}


void CSlider::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == 56 && Set)//TimerisSet
	{
		if(m_Blink)
		{
			Progress = RGB(102,102,204);
			m_Blink = FALSE;
			OutRect = RGB(102,102,204);
			int min,max;
			GetRange(min,max);
			CSliderCtrl::SetRange(min,max,TRUE);
			
		} else 
		{
			Progress = RGB(204,204,204);
			int min,max;
			OutRect = RGB(204,204,204);
			GetRange(min,max);
			CSliderCtrl::SetRange(min,max,TRUE);			
			m_Blink = TRUE;			
		}
	}
	CSliderCtrl::OnTimer(nIDEvent);
}

void CSlider::StartBlinking(void)
{
	if(!Set)
	{
        SetTimer(56,1000,NULL);
		Set=TRUE;
	}

}

void CSlider::StopBlinking(void)
{
	KillTimer(56);
	Set = FALSE;
	Progress = RGB(102,102,204);
	OutRect = RGB(51,220,25);
	int min,max;
	GetRange(min,max);
	CSliderCtrl::SetRange(min,max,TRUE);
		
}

/// Inýt stuff
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

COLORREF LightenColor(const COLORREF crColor, BYTE byIncreaseVal);
void CSlider::DrawSlider(CDC *pDC, const CRect rect)
{
	RECT crect;
	GetThumbRect(&crect);
	CBrush Brush;
    Brush.CreateSolidBrush((LightenColor(m_crColor,30)));
	HBRUSH hbOld = (HBRUSH)SelectObject(pDC->m_hDC,Brush);
	RoundRect(pDC->m_hDC,crect.left,crect.top,crect.right,crect.bottom,2,2);
	SelectObject(pDC->m_hDC,hbOld);
	DeleteObject(hbOld);
	DeleteObject(Brush);

	int nLeft = rect.left;
	int nTop = rect.top;
	int nRight = rect.right;
	int nBottom = rect.bottom;

	CPen *pOldPen = pDC->SelectObject(&m_penColor);
	pDC->MoveTo(nLeft, nTop + 1);
	pDC->LineTo(nLeft, nTop);
	pDC->LineTo(nRight, nTop);
	pDC->MoveTo(nLeft + 1, nBottom - 2);
	pDC->LineTo(nLeft + 1, nTop + 1);
	pDC->MoveTo(nRight - 3, nBottom - 3);
	pDC->LineTo(nRight - 3, nTop + 1);
	pDC->SetPixel(nRight - 2, nTop + 1, m_crColor);

	pDC->SelectObject(&m_penColorLight);
	pDC->MoveTo(nLeft + 2, nBottom - 3);
	pDC->LineTo(nLeft + 2, nTop + 1);
	pDC->MoveTo(nRight - 4, nBottom - 3);
	pDC->LineTo(nRight - 4, nTop + 1);
	pDC->SetPixel(nLeft + 1, nTop + 1, m_crColorLight);
	pDC->SetPixel(nRight - 3, nTop + 1, m_crColorLight);
	
	pDC->SelectObject(&m_penColorLighter);
	pDC->MoveTo(nLeft + 3, nBottom - 3);
	pDC->LineTo(nLeft + 3, nTop + 1);
	pDC->MoveTo(nRight - 5, nBottom - 3);
	pDC->LineTo(nRight - 5, nTop + 1);
	pDC->SetPixel(nLeft + 2, nTop + 1, m_crColorLighter);
	pDC->SetPixel(nRight - 4, nTop + 1, m_crColorLighter);

	pDC->SelectObject(&m_penColorDark);
	pDC->MoveTo(nLeft, nBottom - 1);
	pDC->LineTo(nLeft, nTop + 1);
	pDC->MoveTo(nLeft + 2, nBottom - 2);
	pDC->LineTo(nRight - 2, nBottom - 2);
	pDC->LineTo(nRight - 2, nTop + 1);
	pDC->SetPixel(nRight - 1, nTop + 1, m_crColorDark);
	pDC->SelectObject(&m_penColorDarker);
	pDC->MoveTo(nLeft + 1, nBottom - 1);
	pDC->LineTo(nRight - 1, nBottom - 1);
	pDC->LineTo(nRight - 1, nTop + 1);
		
	pDC->SelectObject(pOldPen);
}	

COLORREF LightenColor(const COLORREF crColor, BYTE byIncreaseVal)
{
	BYTE byRed = GetRValue(crColor);
	BYTE byGreen = GetGValue(crColor);
	BYTE byBlue = GetBValue(crColor);

	if ((byRed + byIncreaseVal) <= 255)
		byRed = BYTE(byRed + byIncreaseVal);
	if ((byGreen + byIncreaseVal)	<= 255)
		byGreen = BYTE(byGreen + byIncreaseVal);
	if ((byBlue + byIncreaseVal) <= 255)
		byBlue = BYTE(byBlue + byIncreaseVal);

	return RGB(byRed, byGreen, byBlue);
}	


COLORREF DarkenColor(const COLORREF crColor, BYTE byReduceVal)
{
	BYTE byRed = GetRValue(crColor);
	BYTE byGreen = GetGValue(crColor);
	BYTE byBlue = GetBValue(crColor);

	if (byRed >= byReduceVal)
		byRed = BYTE(byRed - byReduceVal);
	if (byGreen >= byReduceVal)
		byGreen = BYTE(byGreen - byReduceVal);
	if (byBlue >= byReduceVal)
		byBlue = BYTE(byBlue - byReduceVal);

	return RGB(byRed, byGreen, byBlue);
}	


void CSlider::CreatePens()
{
	DeletePens();

	m_penColorLight.CreatePen(PS_SOLID, 1, m_crColorLight);
	m_penColorLighter.CreatePen(PS_SOLID, 1, m_crColorLighter);
	m_penColor.CreatePen(PS_SOLID, 1, m_crColor);
	m_penColorDark.CreatePen(PS_SOLID, 1, m_crColorDark);
	m_penColorDarker.CreatePen(PS_SOLID, 1, m_crColorDarker);
	m_penDkShadow.CreatePen(PS_SOLID, 1, m_crDkShadow);
	m_penShadow.CreatePen(PS_SOLID, 1, m_crShadow);
	m_penLiteShadow.CreatePen(PS_SOLID, 1, m_crLiteShadow);
}	


void CSlider::DeletePens()
{
	if (m_penColorLight.m_hObject)
		m_penColorLight.DeleteObject();
	if (m_penColorLighter.m_hObject)
		m_penColorLighter.DeleteObject();
	if (m_penColor.m_hObject)
		m_penColor.DeleteObject();
	if (m_penColorDark.m_hObject)
		m_penColorDark.DeleteObject();
	if (m_penColorDarker.m_hObject)
		m_penColorDarker.DeleteObject();
	if (m_penDkShadow.m_hObject)
		m_penDkShadow.DeleteObject();
	if (m_penShadow.m_hObject)
		m_penShadow.DeleteObject();
	if (m_penLiteShadow.m_hObject)
		m_penLiteShadow.DeleteObject();
}	

void CSlider::GetColors()
{
	m_crColorLight = LightenColor(m_crColor, 21);
	m_crColorLighter = LightenColor(m_crColorLight, 21);
	m_crColorLightest = LightenColor(m_crColorLighter, 21);
	m_crColorDark = DarkenColor(m_crColor, 21);
	m_crColorDarker = DarkenColor(m_crColorDark, 21);
	m_crDkShadow = ::GetSysColor(COLOR_3DDKSHADOW);
	m_crLiteShadow = ::GetSysColor(COLOR_3DSHADOW);

	BYTE byRed3DDkShadow = GetRValue(m_crDkShadow);
	BYTE byRed3DLiteShadow = GetRValue(m_crLiteShadow);
	BYTE byGreen3DDkShadow = GetGValue(m_crDkShadow);
	BYTE byGreen3DLiteShadow = GetGValue(m_crLiteShadow);
	BYTE byBlue3DDkShadow = GetBValue(m_crDkShadow);
	BYTE byBlue3DLiteShadow = GetBValue(m_crLiteShadow);

	m_crShadow = RGB(byRed3DLiteShadow + ((byRed3DDkShadow - byRed3DLiteShadow) >> 1),
						  byGreen3DLiteShadow + ((byGreen3DDkShadow - byGreen3DLiteShadow) >> 1),
						  byBlue3DLiteShadow + ((byBlue3DDkShadow - byBlue3DLiteShadow) >> 1));
}	



void CSlider::OnLButtonUp(UINT nFlags, CPoint point)
{
	int min,max;
	GetRange(min,max);
	CSliderCtrl::SetRange(min,max,TRUE);

	CSliderCtrl::OnLButtonUp(nFlags, point);
}

BOOL CSlider::OnEraseBkgnd(CDC* /*pDC*/)
{
	
	return FALSE;
}


