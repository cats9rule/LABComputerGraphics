#pragma once
class CactusPart {
public:
	CactusPart* children[3];
	int childrenCount = 0;
	double* rotAngle = nullptr;
	float initialRotation = 0;

public:
	CactusPart();
	CactusPart(CactusPart** children, double* rotAngle = nullptr, float initialRotation = 0);
	~CactusPart();
};

