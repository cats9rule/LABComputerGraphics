#pragma once

#include "CactusPart.h"

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			
	void PrepareScene(CDC* pDC);			
	void Reshape(CDC* pDC, int w, int h);	
	void DrawScene(CDC* pDC);				
	void DestroyScene(CDC* pDC);
	void RotateBranch(float rotAngle);

private:
	void DrawFigure(double angle);
	void DrawAxis(double width);
	void PrepareAxisVectors(double width, float* vert, float* colors, byte* ind);
	void DrawGrid(double width, double height, int nSegW, int nSegH);
	void DrawCone(double h, double r, int nSeg);
	void DrawCylinder(double h, double r1, double r2, int nSeg);
	void DrawSphere(double r, int nSegAlpha, int nSegBeta);

	void DrawCactusTree(CactusPart* root, double initialTreeAngle, float xCoord, float yCoord, float gridCellSize = 0.5);
	void InitializeCactusParts(CactusPart** parts, int rotatingPartIndex, double angle);
	void DrawCactusPart(CactusPart* part, float height, float radius);

public:
	float m_Rx; float m_Ry;
	double m_prevX; double m_prevY;
private:
	float rotAngleBranch;
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
