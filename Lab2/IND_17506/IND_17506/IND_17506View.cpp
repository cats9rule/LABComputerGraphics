
// IND_17506View.cpp : implementation of the CIND17506View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_17506.h"
#endif

#define _USE_MATH_DEFINES
#include <math.h>

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

void DrawCoordinateAxes(CDC* pDC) {
	pDC->MoveTo({ 0,0 });
	pDC->LineTo({ 100,0 });
	pDC->MoveTo({ 0,0 });
	pDC->LineTo({ 0, 100 });
	pDC->MoveTo({ 0,0 });
}

void CIND17506View::OnDraw(CDC* pDC)
{
	CIND17506Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DrawBackground(pDC);

	const int previousMode = pDC->SetGraphicsMode(GM_ADVANCED);
	DrawFigure(pDC);
	pDC->SetGraphicsMode(previousMode);


	if (shouldDisplayGrid) DrawGrid(pDC);
}

void CIND17506View::DrawFigure(CDC* pDC) {
	const int gridCellSize = 25;
	CactusPart* parts[11] = {};
	InitializeCactusParts(parts, 4, 0);
	XFORM originalXform;
	pDC->GetWorldTransform(&originalXform);
	Translate(pDC, 0, 500);
	Scale(pDC, 1, -1);
	Translate(pDC, 500, 0);
	Rotate(pDC, M_PI_2);
	CPoint* initialRootPoint = new CPoint(10 * gridCellSize, 3 * gridCellSize);
	DrawCactusTree(pDC, parts[0], 0, initialRootPoint, 1, gridCellSize);
	DrawCactusCircle(pDC, initialRootPoint->x, initialRootPoint->y);
	Scale(pDC, 1, -1);
	DrawPot(pDC, 7.6);
	Translate(pDC, 0, -500);
	DrawSignatureText(pDC, CString("17506 Nevena Tufegdzic"), 1);
	pDC->SetWorldTransform(&originalXform);
}

void CIND17506View::InitializeCactusParts(CactusPart** parts, int rotatingPartIndex1, int rotatingPartIndex2) {
	for (int i = 0; i < 11; i++) {
		parts[i] = new CactusPart();
	}
	parts[rotatingPartIndex1]->rotAngle = &rotationAngle1;
	parts[rotatingPartIndex2]->rotAngle = &rotationAngle2;

	parts[0]->children[0] = parts[1];
	parts[0]->children[1] = parts[2];
	parts[0]->children[2] = parts[3];
	parts[0]->childrenCount = 3;

	parts[2]->children[0] = parts[4];
	parts[2]->children[1] = parts[5];
	parts[2]->children[2] = parts[6];
	parts[2]->childrenCount = 3;

	parts[3]->children[0] = parts[9];
	parts[3]->childrenCount = 1;

	parts[5]->children[0] = parts[7];
	parts[5]->childrenCount = 1;

	parts[6]->children[0] = parts[8];
	parts[6]->childrenCount = 1;

	parts[9]->children[0] = parts[10];
	parts[9]->childrenCount = 1;
}

void CIND17506View::DrawSignatureText(CDC* pDC, CString text, float gridX, float gridY, int gridCellSize) {
	COLORREF originalColor = pDC->SetTextColor(RGB(70, 70, 70));
	int originalBkMode = pDC->SetBkMode(TRANSPARENT);
	CFont* font = new CFont();
	font->CreateFont(27, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, CString("Arial"));
	CFont* originalFont = pDC->SelectObject(font);
	pDC->TextOut(gridX * gridCellSize + 2, gridY * gridCellSize + 2, text);
	pDC->SetTextColor(RGB(255, 255, 10));
	pDC->TextOut(gridX * gridCellSize, gridY * gridCellSize, text);
	pDC->SetBkMode(originalBkMode);
	pDC->SelectObject(originalFont);
	pDC->SetTextColor(originalColor);
	delete font;
}

void CIND17506View::DrawPot(CDC* pDC, float gridX, int gridCellSize, int dimension) {
	int potHeight = 2.8 * gridCellSize;
	int potRimHeight = 0.8 * gridCellSize;
	int potRimWidth = 4.8 * gridCellSize;
	int potBaseWidth = 3.2 * gridCellSize;
	int startingPoint = gridX * gridCellSize;
	COLORREF potColor = RGB(250, 180, 10);
	XFORM originalXform;
	pDC->GetWorldTransform(&originalXform);
	Translate(pDC, startingPoint, 0);
	CBrush* potBrush = new CBrush();
	potBrush->CreateSolidBrush(potColor);
	CBrush* originalBrush = pDC->SelectObject(potBrush);
	CPoint points[4] = { 
		CPoint(0 + potRimWidth / 2 - 2 * gridCellSize, 0 - potHeight + potRimHeight - 1), 
		CPoint(0 + potRimWidth / 2 + 2 * gridCellSize, 0 - potHeight + potRimHeight - 1),
		CPoint(0 + potRimWidth / 2 + potBaseWidth / 2, 0 - 1),
		CPoint(0 + potRimWidth / 2 - potBaseWidth / 2, 0 - 1)
	};
	pDC->Polygon(points, 4);
	pDC->RoundRect(
		0, 
		0 - potHeight - 1, 
		0 + potRimWidth, 
		0 - potHeight + potRimHeight, 
		2, 
		2
	);
	pDC->SelectObject(originalBrush);
	pDC->SetWorldTransform(&originalXform);
	delete potBrush;
}

void CIND17506View::DrawGrid(CDC* pDC, int dimension, int gridCellCount) {
	const int gridCellDimension = dimension / gridCellCount;
	CPen* gridPen = new CPen(PS_SOLID, 1, RGB(240, 240, 240));
	CPen* originalPen = pDC->SelectObject(gridPen);
	CGdiObject* originalBrush = pDC->SelectStockObject(HOLLOW_BRUSH);
	int x = 0; int y = 0;
	for (int i = 0; i < gridCellCount; i++) {
		pDC->MoveTo(x, 0);
		pDC->LineTo(x, dimension);
		x += gridCellDimension;
		pDC->MoveTo(0, y);
		pDC->LineTo(dimension, y);
		y += gridCellDimension;
	}
	pDC->SelectObject(originalPen);
	pDC->SelectObject(originalBrush);
	delete gridPen;
}

void CIND17506View::DrawBackground(CDC* pDC, int dimension) {
	CBrush* brush = new CBrush();
	brush->CreateSolidBrush(RGB(150, 225, 250));
	CBrush* originalBrush = pDC->SelectObject(brush);
	CGdiObject* originalPen = pDC->SelectStockObject(NULL_PEN);
	pDC->Rectangle(CRect(0, 0, dimension, dimension));
	pDC->SelectObject(originalBrush);
	pDC->SelectObject(originalPen);
	delete brush;
}

void CIND17506View::DrawCactusTree(CDC* pDC, CactusPart* root, double initialTreeAngle, CPoint* rootPoint, int sizeModifier, int gridCellSize) {
	XFORM originalXform;
	pDC->GetWorldTransform(&originalXform);
	double alpha = root->rotAngle != nullptr ? initialTreeAngle + *root->rotAngle : initialTreeAngle;
	alpha = -alpha;
	int x = rootPoint->x;
	int y = gridCellSize*3;
	int circleRadius = gridCellSize * 0.4;
	Translate(pDC, x, y);
	Rotate(pDC, alpha);
	// DrawCoordinateAxes(pDC);

	bool isMobile = root->rotAngle != nullptr;
	DrawCactusPart(pDC, circleRadius, circleRadius, sizeModifier, gridCellSize, isMobile);
	if (root->childrenCount > 0) {
		CPoint* nextRootPoint = new CPoint(0, 3 * gridCellSize);
		if (root->childrenCount == 1) {
			DrawCactusTree(pDC, root->children[0], 0, nextRootPoint, 1, gridCellSize);
		}
		else {
			double rotations[3] = { -M_PI_4, M_PI_4, 0 };
			for (int i = 0; i < root->childrenCount; i++) {
				DrawCactusTree(pDC, root->children[i], rotations[i], nextRootPoint, root->childrenCount, gridCellSize);
			}
		}
		DrawCactusCircle(pDC, nextRootPoint->x, nextRootPoint->y);
		delete nextRootPoint;
	}
	pDC->SetWorldTransform(&originalXform);
}

void CIND17506View::DrawCactusPart(CDC* pDC, int cX, int cY, int sizeModifier, int gridCellSize, bool isMobile) {
	int circleRadius = gridCellSize * 0.4;
	int height = gridCellSize * 3;
	int width = gridCellSize * 2.4;
	XFORM originalXform;
	pDC->GetWorldTransform(&originalXform);
	if (sizeModifier == 2) {
		width = gridCellSize * 1.6;
	}
	else if (sizeModifier == 3) {
		width = gridCellSize * 0.8;
	}
	HENHMETAFILE cactusPart = isMobile 
		? GetEnhMetaFile(CString("cactus_part_light.emf")) 
		: GetEnhMetaFile(CString("cactus_part.emf"));
	CRect* rect = new CRect(width / 2, 0, - width / 2, height);
	pDC->PlayMetaFile(cactusPart, rect);
	pDC->SetWorldTransform(&originalXform);
	DeleteEnhMetaFile(cactusPart);
}

void CIND17506View::DrawCactusCircle(CDC* pDC, int cX, int cY, int radius) {
	CBrush* brush = new CBrush();
	brush->CreateSolidBrush(RGB(50, 240, 50));
	CBrush* oldBrush = pDC->SelectObject(brush);
	XFORM oldXform;
	pDC->GetWorldTransform(&oldXform);
	Translate(pDC, cX-radius, cY-radius);
	pDC->Ellipse(0, 0, 2*radius, 2*radius);
	pDC->SetWorldTransform(&oldXform);
	pDC->SelectObject(oldBrush);
	delete brush;
}

void CIND17506View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply) {
	int mwt = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	XFORM Xform;
	Xform.eM11 = 1.0;
	Xform.eM12 = 0.0;
	Xform.eM21 = 0.0;
	Xform.eM22 = 1.0;
	Xform.eDx = dX;
	Xform.eDy = dY;
	pDC->ModifyWorldTransform(&Xform, mwt);
}

void CIND17506View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply) {
	int mwt = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	XFORM Xform;
	Xform.eM11 = sX;
	Xform.eM12 = 0.0;
	Xform.eM21 = 0.0;
	Xform.eM22 = sY;
	Xform.eDx = 0.0;
	Xform.eDy = 0.0;
	pDC->ModifyWorldTransform(&Xform, mwt);
}

void CIND17506View::Rotate(CDC* pDC, float angle, bool rightMultiply) {
	int mwt = rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY;
	float c = cos(angle);
	float s = sin(angle);
	XFORM Xform;
	Xform.eM11 = c;
	Xform.eM12 = s;
	Xform.eM21 = -s;
	Xform.eM22 = c;
	Xform.eDx = 0.0;
	Xform.eDy = 0.0;
	pDC->ModifyWorldTransform(&Xform, mwt);
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
	if (nChar == 'G') {
		shouldDisplayGrid = !shouldDisplayGrid;
		Invalidate();
	}

	if (nChar == 'A') {
		HandleRotation1(true);
		Invalidate();
	}
	if (nChar == 'D') {
		HandleRotation1(false);
		Invalidate();
	}

	if (nChar == 'W') {
		HandleRotation2(true);
		Invalidate();
	}
	if (nChar == 'S') {
		HandleRotation2(false);
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CIND17506View::HandleRotation1(bool isLeft) {
	if (isLeft) {
		rotationAngle1 -= M_PI / 6;
		rotationAngle1 = rotationAngle1 < -2 * M_PI ? 0 : rotationAngle1;
	}
	else {
		rotationAngle1 += M_PI / 6;
		rotationAngle1 = rotationAngle1 > 2 * M_PI ? 0 : rotationAngle1;
	}
}

void CIND17506View::HandleRotation2(bool isLeft) {
	if (isLeft) {
		rotationAngle2 -= M_PI / 6;
		rotationAngle2 = rotationAngle2 < -2 * M_PI ? 0 : rotationAngle2;
	}
	else {
		rotationAngle2 += M_PI / 6;
		rotationAngle2 = rotationAngle2 > 2 * M_PI ? 0 : rotationAngle2;
	}
}
