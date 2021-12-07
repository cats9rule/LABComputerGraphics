#include "pch.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"

#define _USE_MATH_DEFINES
#include <math.h>

CGLRenderer::CGLRenderer()
{
	m_Rx = m_Ry = m_prevX = m_prevY = 0;
	rotAngleBranch = 0;
}

CGLRenderer::~CGLRenderer() { }

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

	if (nPixelFormat == 0)
		return false;

	BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

	if (!bResult)
		return false;

	m_hrc = wglCreateContext(pDC->m_hDC);

	if (!m_hrc)
		return false;

	return true;
}


void CGLRenderer::PrepareScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glClearColor(0.5, 0.75, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	wglMakeCurrent(NULL, NULL);
}


void CGLRenderer::DestroyScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//.. opengl fje za delokaciju
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}


void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 0.1, 250);
	glMatrixMode(GL_MODELVIEW);
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::RotateBranch(float rotAngle) {
	rotAngleBranch += rotAngle;
}


void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //brise se samo frame buffer (buffer u kom se iscrtava slika)
	glLoadIdentity();
	glTranslatef(0, - 2, - 8);
	glRotatef(m_Rx, 1.0, 0.0, 0.0);
	glRotatef(m_Ry, 0.0, 1.0, 0.0);

	DrawGrid(5, 5, 10, 10);
	DrawAxis(7);
	DrawFigure(rotAngleBranch);

	glFlush(); //forsira se iscrtavanje (iscrtavanje se inace vrsi kad je bafer pun)
	SwapBuffers(pDC->m_hDC); //radi slicno kao BitBlt, jos efikasnije jer se nista ne kopira, vec se samo menja referenca
							//swap back i front buffer 
	wglMakeCurrent(NULL, NULL);

}

void CGLRenderer::DrawFigure(double angle) {

	float gridCellSize = 0.5;

	glColor4f(0.87, 0.58, 0.0, 1.0);
	DrawCylinder(1.5 * gridCellSize, gridCellSize, 1.25 * gridCellSize, 8);
	glTranslatef(0.0, 1.5 * gridCellSize, 0.0);
	DrawCylinder(0.5 * gridCellSize, gridCellSize * 1.5, gridCellSize * 1.5, 8);
	glTranslatef(0, 0.5 * gridCellSize, 0);

	CactusPart* parts[11];
	for (int i = 0; i < 11; i++) {
		parts[i] = new CactusPart();
	}
	InitializeCactusParts(parts, 1, angle);
	DrawCactusTree(parts[0], 0, 0, 0);
	for (int i = 0; i < 11; i++) {
		delete parts[i];
	}
}


void CGLRenderer::InitializeCactusParts(CactusPart** parts, int rotatingPartIndex, double alpha) {

	parts[rotatingPartIndex]->isRotating = true;
	parts[rotatingPartIndex]->rotAngle = alpha;

	parts[0]->children[0] = parts[1];
	parts[0]->children[1] = parts[2];
	parts[0]->children[2] = parts[3];
	parts[0]->childrenCount = 3;

	parts[1]->children[0] = parts[4];
	parts[1]->childrenCount = 1;

	parts[3]->children[0] = parts[6];
	parts[3]->children[1] = parts[7];
	parts[3]->childrenCount = 2;

	parts[4]->children[0] = parts[5];
	parts[4]->childrenCount = 1;

	parts[6]->children[0] = parts[8];
	parts[6]->children[1] = parts[9];
	parts[6]->childrenCount = 2;

	parts[7]->children[0] = parts[10];
	parts[7]->childrenCount = 1;

	parts[0]->isCone = true;
	parts[1]->isCone = true;
	parts[5]->isCone = true;
	parts[6]->isCone = true;
	parts[8]->isCone = true;
	parts[9]->isCone = true;
}

void CGLRenderer::DrawCactusTree(CactusPart* root, double initialTreeAngle, float xCoord, float yCoord, float gridCellSize) {
	glPushMatrix();
	float sphereRadius = gridCellSize * 0.3;
	float alpha = root->isRotating ? initialTreeAngle + root->rotAngle : initialTreeAngle;
	glTranslatef(0, -sphereRadius, 0);
	glRotatef(alpha, 0, 0, 1);
	float height = gridCellSize * 1.2;
	float radius = gridCellSize * 0.4;
	glTranslatef(0, sphereRadius, 0);
	DrawCactusPart(root, height, radius);
	glTranslatef(0, height + sphereRadius, 0);
	glColor3f(0.1, 0.7, 0.1);
	DrawSphere(sphereRadius, 10, 10);
	glTranslatef(0, sphereRadius, 0);
	if (root->childrenCount == 1) {
		DrawCactusTree(root->children[0], 0, 0, 0);
	}
	else if (root->childrenCount > 1) {
		double rotations[3] = { 45, -45, 0 };
		for (int i = 0; i < root->childrenCount; i++) {
			DrawCactusTree(root->children[i], rotations[i], 0, 0, gridCellSize);
		}
	}
	glPopMatrix();
}

void CGLRenderer::DrawCactusPart(CactusPart* part, float height, float radius) {
	if (part->isRotating) {
		glColor3f(1.0, 1.0, 0.2);
	}
	else {
		glColor3f(0.1, 1, 0.1);
	}
	if (part->isCone) {
		DrawCone(height, radius, 15);
	}
	else {
		DrawCylinder(height, radius, radius, 15);
	}
}

void CGLRenderer::DrawAxis(double width) {
	float vert[12];
	float col[12];
	byte ind[6];
	PrepareAxisVectors(width, vert, col, ind);
	glVertexPointer(3, GL_FLOAT, 0, vert);
	glColorPointer(3, GL_FLOAT, 0, col);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glShadeModel(GL_FLAT);
	glDrawElements(GL_LINES, 6, GL_UNSIGNED_BYTE, ind);
	glShadeModel(GL_SMOOTH);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void CGLRenderer::PrepareAxisVectors(double width, float* vert, float* col, byte* ind) {
	vert[0] = 0.0; vert[1] = 0.0; vert[2] = 0.0; // vert 0
	vert[3] = width; vert[4] = 0.0; vert[5] = 0.0; // x-axis
	vert[6] = 0.0; vert[7] = width; vert[8] = 0.0; // y-axis
	vert[9] = 0.0; vert[10] = 0.0; vert[11] = width; // z-axis

	col[0] = 0.0; col[1] = 0.0; col[2] = 0.0;  // origin - not important
	col[3] = 1.0; col[4] = 0.0; col[5] = 0.0;  // x-axis - red
	col[6] = 0.0; col[7] = 1.0; col[8] = 0.0;  // y-axis - green
	col[9] = 0.0; col[10] = 0.0; col[11] = 1.0;  // z-axis - blue

	ind[0] = 0; ind[1] = 1;
	ind[2] = 0; ind[3] = 2;
	ind[4] = 0; ind[5] = 3;
}

void CGLRenderer::DrawGrid(double width, double height, int nSegW, int nSegH) {
	glLineWidth(1.0);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	double x = -width / 2;
	double z = -height / 2;
	for (int i = 0; i <= nSegW; i++) {
		glVertex3f(x, 0.0, -height / 2);
		glVertex3f(x, 0.0, height / 2);
		x += width / nSegW;
	}
	for (int i = 0; i <= nSegH; i++) {
		glVertex3f(-width / 2, 0.0, z);
		glVertex3f(width / 2, 0.0, z);
		z += height / nSegH;
	}
	glEnd();
}

void CGLRenderer::DrawCone(double h, double r, int nSeg) {
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, h, 0);
	for (int i = 0; i <= nSeg; i++) {
		glVertex3f(
			r * cos(2 * M_PI / nSeg * i),
			0.0,
			r * sin(2 * M_PI / nSeg * i)
		);
	}
	glEnd();
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg) {
	glBegin(GL_POLYGON);
	for (int i = 0; i <= nSeg; i++) {
		glVertex3f(
			r1 * cos(2 * M_PI / nSeg * i),
			0.0,
			r1 * sin(2 * M_PI / nSeg * i)
		);
	}
	glEnd();
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= nSeg; i++) {
		glVertex3f(
			r2 * cos(2 * M_PI / nSeg * i),
			h,
			r2 * sin(2 * M_PI / nSeg * i)
		);
		glVertex3f(
			r1 * cos(2 * M_PI / nSeg * i),
			0,
			r1 * sin(2 * M_PI / nSeg * i)
		);
	}
	glEnd();
	glBegin(GL_POLYGON);
	for (int i = 0; i <= nSeg; i++) {
		glVertex3f(
			r2 * cos(2 * M_PI / nSeg * i),
			h,
			r2 * sin(2 * M_PI / nSeg * i)
		);
	}
	glEnd();
}

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta) {
	double rotAlpha = M_PI / nSegAlpha;
	double rotBeta = M_PI / nSegBeta;
	double alpha = M_PI / 2;
	double beta = 0;
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= nSegAlpha; i++) {
		for (int j = 0; j <= nSegBeta * 2; j++) {
			glVertex3f(
				r * cos(alpha) * cos(beta),
				r * sin(alpha),
				r * cos(alpha) * sin(beta)
			);
			alpha -= rotAlpha;
			glVertex3f(
				r * cos(alpha) * cos(beta),
				r * sin(alpha),
				r * cos(alpha) * sin(beta)
			);
			alpha += rotAlpha;
			beta += rotBeta;
		}
		alpha -= rotAlpha;
		beta = 0;
	}
	glEnd();
}