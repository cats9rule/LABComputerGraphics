#pragma once
class CactusPart {
public:
	CactusPart* children[3];
	int childrenCount = 0;
	double rotAngle = 0;
	bool isCone = false;
	bool isRotating = false;
public:
	CactusPart();
	CactusPart(CactusPart** children, float rotAngle = 0);
	~CactusPart();
};

