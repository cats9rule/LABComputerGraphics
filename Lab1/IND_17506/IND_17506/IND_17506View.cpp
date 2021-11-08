
// IND_17506View.cpp : implementation of the CIND17506View class
//

#include "pch.h"
#include "framework.h"
#define _USE_MATH_DEFINES
#include <math.h>
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


// CIND17506View

IMPLEMENT_DYNCREATE(CIND17506View, CView)

BEGIN_MESSAGE_MAP(CIND17506View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
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

// CIND17506View drawing

void CIND17506View::OnDraw(CDC* pDC)
{
	CIND17506Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	int tangramDimension = 500;
	int gridCellCount = 20;
	int pieceCount = 7;

	DrawBackground(pDC, tangramDimension);

	CPoint points[23] = {};
	CPoint regularPolygonCenters[5] = {};
	int polygonIncircleRadius[5] = {};
	int pointCount[7] = { 3,3,3,3,3,4,4};
	COLORREF colors[] = { RGB(255, 255, 0), RGB(255, 153, 51), RGB(153, 0, 204), RGB(50, 205, 50), RGB(100, 150, 200), RGB(255, 153, 204), RGB(255, 0, 0) };
	int hatchStyles[] = { -1, -1, -1, -1, HS_CROSS, -1, -1 };

	ResolvePointsForPieces(points, regularPolygonCenters, polygonIncircleRadius, tangramDimension, gridCellCount);
	DrawPieces(pDC, points, pointCount, colors, hatchStyles, pieceCount);
	DrawRegularPolygons(pDC, regularPolygonCenters, polygonIncircleRadius);
	if (shouldDisplayGrid) DrawGrid(pDC, tangramDimension, gridCellCount);
}

void CIND17506View::DrawPieces(CDC* pDC, CPoint* points, int* pointsCountByPiece, COLORREF* colors, int* hatchStyles, int pieceCount) {
	LOGBRUSH logBrush;
	logBrush.lbColor = RGB(20, 20, 240);
	logBrush.lbStyle = BS_SOLID;
	CPen* pen = new CPen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, 6, &logBrush);
	CPen* originalPen = pDC->SelectObject(pen);
	CBrush* brush = new CBrush();
	CBrush* originalBrush = pDC->GetCurrentBrush();

	int pointsTaken = 0;
	for (int i = 0; i < pieceCount; i++) {
		if (hatchStyles[i] > -1) {
			brush->CreateHatchBrush(hatchStyles[i], colors[i]);
		}
		else {
			brush->CreateSolidBrush(colors[i]);
		}
		pDC->SelectObject(brush);
		CPoint* figurePoints = new CPoint[pointsCountByPiece[i]];
		for (int j = 0; j < pointsCountByPiece[i]; j++) {
			figurePoints[j] = points[pointsTaken++];
		}
		pDC->Polygon(figurePoints, pointsCountByPiece[i]);
		brush->DeleteObject();
		delete[] figurePoints;
	}

	pDC->SelectObject(originalBrush);
	pDC->SelectObject(originalPen);
	delete brush;
	delete pen;
}

void CIND17506View::DrawRegularPolygons(CDC* pDC, CPoint* polygonCenters, int* polygonRadius) {
	for (int i = 0; i < 5; i++) {
		DrawRegularPolygon(pDC, polygonCenters[i].x, polygonCenters[i].y, polygonRadius[i], i + 4);
	}
}

void CIND17506View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n) {
	LOGBRUSH logBrush;
	logBrush.lbColor = RGB(20, 20, 240);
	logBrush.lbStyle = BS_SOLID;
	CPen* pen = new CPen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, 3, &logBrush);
	CPen* originalPen = pDC->SelectObject(pen);
	CGdiObject* originalBrush = pDC->SelectStockObject(HOLLOW_BRUSH);

	double innerAngle = (double)360 / n * M_PI / 180;
	double alpha = 0;
	CPoint* points = new CPoint[n];
	for (int i = 0; i < n; i++) {
		int x = r * cos(alpha) + cx;
		int y = r * sin(alpha) + cy;
		points[i].x = x;
		points[i].y = y;
		alpha += innerAngle;
	}
	pDC->Polygon(points, n);
	delete[] points;
	pDC->SelectObject(originalBrush);
	pDC->SelectObject(originalPen);
	delete pen;
}

void CIND17506View::DrawGrid(CDC* pDC, int tangramDimension, int gridCellCount) {
	const int gridCellDimension = tangramDimension / gridCellCount;
	//int oldBkMode = pDC->SetROP2(R2_MERGEPEN);
	CPen* gridPen = new CPen(PS_SOLID, 1, RGB(220, 220, 220));
	CPen* originalPen = pDC->SelectObject(gridPen);
	CGdiObject* originalBrush = pDC->SelectStockObject(HOLLOW_BRUSH);
	int x = 0; int y = 0;
	for (int i = 0; i < gridCellCount; i++) {
		pDC->MoveTo(x, 0);
		pDC->LineTo(x, tangramDimension);
		x += gridCellDimension;
		pDC->MoveTo(0, y);
		pDC->LineTo(tangramDimension, y);
		y += gridCellDimension;
	}
	pDC->SelectObject(originalPen);
	pDC->SelectObject(originalBrush);
	//pDC->SetROP2(oldBkMode);
	delete gridPen;
}

void CIND17506View::DrawBackground(CDC* pDC, int tangramDimension) {
	CBrush* brush = new CBrush();
	brush->CreateSolidBrush(RGB(230, 230, 230));
	CBrush* originalBrush = pDC->SelectObject(brush);
	CGdiObject* originalPen = pDC->SelectStockObject(NULL_PEN);
	pDC->Rectangle(CRect(0, 0, tangramDimension, tangramDimension));
	pDC->SelectObject(originalBrush);
	pDC->SelectObject(originalPen);
	delete brush;
}

void CIND17506View::ResolvePointsForPieces(CPoint* points, CPoint* regularPolygonCenters, int* regularPolygonR, int tangramDimension, int gridGranularity) {
	const int gridCellSize = tangramDimension / gridGranularity;
	const float smallTriangleDimension = 6;
	GenerateRightTriangle(1, 7, smallTriangleDimension, 270, points, &regularPolygonCenters[1], &regularPolygonR[1], gridCellSize); // small 1
	GenerateRightTriangle(1, 19, smallTriangleDimension, 270, &points[3], &regularPolygonCenters[2], &regularPolygonR[2], gridCellSize); // small 2
	GenerateRightTriangle(13, 13, smallTriangleDimension * sqrt(2), 135, &points[6], &regularPolygonCenters[4], &regularPolygonR[4], gridCellSize); // medium
	GenerateRightTriangle(13, 1, smallTriangleDimension * 2, 90, &points[9], &regularPolygonCenters[3], &regularPolygonR[3], gridCellSize); // large 1
	GenerateRightTriangle(19, 19, smallTriangleDimension * 2, 180, &points[12], &regularPolygonCenters[0], &regularPolygonR[0], gridCellSize); // large 2

	GenerateSquare(13, 1, smallTriangleDimension, 0, &points[15], gridCellSize);
	GenerateParallelogram(1, 7, smallTriangleDimension, 45, true, &points[19], gridCellSize);
}

void CIND17506View::GenerateRightTriangle(float gridX, float gridY, float a, int rotationAlpha, CPoint* points, CPoint* incenter, int* inpolygonRadius, int gridCellSize) {
	double alpha = (double)rotationAlpha * M_PI / 180;
	int cX = gridX * gridCellSize;
	int cY = gridY * gridCellSize;
	a *= gridCellSize;
	points[0].x = cX;
	points[0].y = cY;
	for (int i = 1; i < 3; i++) {
		int x = a * cos(alpha) + cX;
		int y = a * sin(alpha) + cY;
		points[i].x = x;
		points[i].y = y;
		alpha += (M_PI / 2);
	}
	*incenter = ResolveTriangleIncircleCenter(points[2], points[1], points[0]); // points A, B and C in order
	*inpolygonRadius = ResolveTrianglePolygonRadius(a);
}

CPoint CIND17506View::ResolveTriangleIncircleCenter(CPoint a, CPoint b, CPoint c) {
	CPoint o = CPoint();
	o.x = abs(a.x + b.x) / 2;
	o.y = abs(a.y + b.y) / 2;
	CPoint center = CPoint();
	center.x = abs(c.x + sqrt(2) * o.x) / (sqrt(2) + 1);
	center.y = abs(c.y + sqrt(2) * o.y) / (sqrt(2) + 1);
	return center;
}

int CIND17506View::ResolveTrianglePolygonRadius(float side) {
	return side / (2 + sqrt(2))/2;
}

void CIND17506View::GenerateSquare(float gridX, float gridY, float a, int rotationAlpha, CPoint* points, int gridCellSize) {
	double alpha = (double)rotationAlpha * M_PI / 180;
	int cX = gridX * gridCellSize;
	int cY = gridY * gridCellSize;
	a *= gridCellSize;
	points[0].x = cX;
	points[0].y = cY;

	int x = a * cos(alpha) + cX;
	int y = a * sin(alpha) + cY;
	points[1].x = x;
	points[1].y = y;

	alpha += M_PI / 4;
	a = a * sqrt(2);
	x = a * cos(alpha) + cX;
	y = a * sin(alpha) + cY;
	points[2].x = x;
	points[2].y = y;

	alpha += M_PI / 4;
	a = a / sqrt(2);
	x = a * cos(alpha) + cX;
	y = a * sin(alpha) + cY;
	points[3].x = x;
	points[3].y = y;
}

void CIND17506View::GenerateParallelogram(float gridX, float gridY, float a, int rotationAlpha, bool mirrored, CPoint* points, int gridCellSize) {
	double alpha = rotationAlpha * M_PI / 180;

	int cX = gridX * gridCellSize;
	int cY = gridY * gridCellSize;
	a *= gridCellSize;
	int l = mirrored ? a * sqrt(2) : a;

	points[0].x = cX;
	points[0].y = cY;

	int x = cX + l * cos(alpha);
	int y = cY + l * sin(alpha);
	points[1].x = x;
	points[1].y = y;

	l = mirrored ? a : a * sqrt(2);
	x = x + l * cos(alpha + M_PI / 4);
	y = y + l * sin(alpha + M_PI / 4);
	points[2].x = x;
	points[2].y = y;

	l = mirrored ? a * sqrt(2) : a ;
	x = x + l * cos(alpha + M_PI);
	y = y + l * sin(alpha + M_PI);
	points[3].x = x;
	points[3].y = y;
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


void CIND17506View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == 'G') {
		shouldDisplayGrid = !shouldDisplayGrid;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
