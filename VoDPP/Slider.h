#pragma once
#include "MeMDC.h"
// CSlider

class CSlider : public CSliderCtrl 
{
	DECLARE_DYNAMIC(CSlider)

public:
	CSlider();
	COLORREF InRect;
	COLORREF OutRect;
	COLORREF Pressed;
	COLORREF Progress;
	virtual ~CSlider();
	
	CRect Ch;
	double intPos;
	void SetProgressPos(int pos);
	HDC hDC;
	void SetColor(COLORREF crColor);
protected:
	CPoint p;
	void Channel(LPNMCUSTOMDRAW pNMCD);
	void Thumb(LPNMCUSTOMDRAW pNMCD);
	void DrawBorder(HDC hDC);
	BOOL Selected,InSide;
	void DrawSlider(CDC *pDC, const CRect rect);
	void DrawProgress(HDC hDC, const CRect rect);
	void SetCTick(LPNMCUSTOMDRAW pNMCD);
protected:
	
	void DeletePens();
	void CreatePens();
	CPen m_penColor;
	CPen m_penColorLight;
	CPen m_penColorLighter;
	CPen m_penColorDark;
	CPen m_penColorDarker;
	CPen m_penDkShadow;
	CPen m_penShadow;
	CPen m_penLiteShadow;
	void GetColors();
	COLORREF m_crColor;
	COLORREF m_crColorLight;
	COLORREF m_crColorLighter;
	COLORREF m_crColorLightest;
	COLORREF m_crColorDark;
	COLORREF m_crColorDarker;
	COLORREF m_crDkShadow;
	COLORREF m_crShadow;
	COLORREF m_crLiteShadow;
	


	DECLARE_MESSAGE_MAP()
public:
	void GlbTicCnt(void);
	void SetProRange(int nLower, int nUpper);
	void GetProRange(int &nLower, int &nUpper) const;
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void StartBlinking(void);
	void StopBlinking(void);
	bool m_Blink;
	bool Set;
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
};


