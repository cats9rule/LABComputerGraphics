
// IND_17506View.h : interface of the CIND17506View class
//

#pragma once
#include "CactusPart.h"

class CIND17506View : public CView
{
protected: // create from serialization only
	CIND17506View() noexcept;
	DECLARE_DYNCREATE(CIND17506View)

// Attributes
public:
	CIND17506Doc* GetDocument() const;
private:
	bool shouldDisplayGrid = false;
	double rotationAngle1 = 0;
	double rotationAngle2 = 0;
// Operations
public:

private:
	void InitializeCactusParts(CactusPart** parts, int rotatingPartIndex1, int rotatingPartIndex2);

	void DrawFigure(CDC* pDC);
	void DrawSignatureText(CDC* pDC, CString text, float gridX, float gridY = 1, int gridCellSize = 25);
	void DrawPot(CDC* pDC, float gridX, int gridCellSize = 25, int dimension = 500);
	void DrawGrid(CDC* pDC, int dimension = 500, int gridCellCount = 20);
	void DrawBackground(CDC* pDC, int dimension = 500);

	void DrawCactusTree(CDC* pDC, CactusPart* root, double initialTreeAngle, CPoint* rootPoint, int sizeModifier, int gridCellSize = 25);
	void DrawCactusPart(CDC* pDC, int cX, int cY, int sizeModifier, int gridCellSize = 25, bool isMobile = false);
	void DrawCactusCircle(CDC* pDC, int cX, int cY, int radius = 10);

	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply = false);
	void Rotate(CDC* pDC, float angle, bool rightMultiply = false);

	void HandleRotation1(bool isLeft);
	void HandleRotation2(bool isLeft);
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
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
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in IND_17506View.cpp
inline CIND17506Doc* CIND17506View::GetDocument() const
   { return reinterpret_cast<CIND17506Doc*>(m_pDocument); }
#endif

