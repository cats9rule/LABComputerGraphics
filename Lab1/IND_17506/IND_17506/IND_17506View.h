
// IND_17506View.h : interface of the CIND17506View class
//

#pragma once


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

// Operations
public:
	void DrawBackground(CDC* pDC, int tangramDimension);
	void DrawGrid(CDC* pDc, int tangramDimension, int gridGranularity);
	void DrawPieces(CDC* pDC, CPoint* points, int* pointsCountByPiece, COLORREF* colors, int* hatchStyles, int pieceCount = 7);
	void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n);
	void DrawRegularPolygons(CDC* pDC, CPoint* polygonCenters, int* polygonRadius);
	void ResolvePointsForPieces(CPoint* points, CPoint* regularPolygonCenters, int* regularPolygonR, int tangramDimension, int gridGranularity);
	void GenerateRightTriangle(float gridX, float gridY, float a, int rotationAlpha, CPoint* points, CPoint* incenter, int* inpolygonRadius, int gridCellSize);
	void GenerateSquare(float gridX, float gridY, float a, int rotationAlpha, CPoint* points, int gridCellSize);
	void GenerateParallelogram(float gridX, float gridY, float a, int rotationAlpha, bool mirrored, CPoint* points, int gridCellSize);
	CPoint ResolveTriangleIncircleCenter(CPoint a, CPoint b, CPoint c);
	int ResolveTrianglePolygonRadius(float side);

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

