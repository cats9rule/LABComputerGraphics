
// IND_17506View.cpp : implementation of the CIND17506View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_17506.h"
#endif

#include "IND_17506Doc.h"
#include "IND_17506View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _USE_MATH_DEFINES
#include <math.h>

// CIND17506View

IMPLEMENT_DYNCREATE(CIND17506View, CView)

BEGIN_MESSAGE_MAP(CIND17506View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIND17506View construction/destruction

CIND17506View::CIND17506View() noexcept
{
	// TODO: add construction code here

}

CIND17506View::~CIND17506View()
{
}

BOOL CIND17506View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CIND17506View::OnInitialUpdate() {
	CView::OnInitialUpdate();
	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}

// CIND17506View drawing

void CIND17506View::OnDraw(CDC* pDC)
{
	CIND17506Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	m_glRenderer.DrawScene(pDC);
}


// CIND17506View printing

BOOL CIND17506View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIND17506View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIND17506View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIND17506View diagnostics

#ifdef _DEBUG
void CIND17506View::AssertValid() const
{
	CView::AssertValid();
}

void CIND17506View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND17506Doc* CIND17506View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND17506Doc)));
	return (CIND17506Doc*)m_pDocument;
}
#endif //_DEBUG


// CIND17506View message handlers


int CIND17506View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);
	return 0;
}


BOOL CIND17506View::OnEraseBkgnd(CDC* pDC)
{
	return true;
	/*return CView::OnEraseBkgnd(pDC);*/
}


void CIND17506View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CIND17506View::OnDestroy()
{
	CView::OnDestroy();
	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);
}


void CIND17506View::OnMouseMove(UINT nFlags, CPoint point)
{
	if ((GetKeyState(VK_LBUTTON) & 0x8000) != 0) {
		if (m_glRenderer.m_prevX < point.x) {
			m_glRenderer.m_Ry += 1;
		}
		if (m_glRenderer.m_prevX > point.x) {
			m_glRenderer.m_Ry -= 1;
		}
		if (m_glRenderer.m_prevY < point.y) {
			m_glRenderer.m_Rx += 1;
		}
		if (m_glRenderer.m_prevY > point.y) {
			m_glRenderer.m_Rx -= 1;
		}
		m_glRenderer.m_prevX = point.x;
		m_glRenderer.m_prevY = point.y;
		Invalidate();
	}
	CView::OnMouseMove(nFlags, point);
}


void CIND17506View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'R') {
		m_glRenderer.RotateBranch(- 30);
		Invalidate();
	}
	if (nChar == 'L') {
		m_glRenderer.RotateBranch(30);
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
