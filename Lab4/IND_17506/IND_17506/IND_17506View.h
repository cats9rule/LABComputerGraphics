
// IND_17506View.h : interface of the CIND17506View class
//

#pragma once

#include "GLRenderer.h"

class CIND17506View : public CView
{
protected: // create from serialization only
	CIND17506View() noexcept;
	DECLARE_DYNCREATE(CIND17506View)
	CGLRenderer m_glRenderer;

// Attributes
public:
	CIND17506Doc* GetDocument() const;

// Operations
public:
	
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void OnInitialUpdate();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CIND17506View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in IND_17506View.cpp
inline CIND17506Doc* CIND17506View::GetDocument() const
   { return reinterpret_cast<CIND17506Doc*>(m_pDocument); }
#endif

