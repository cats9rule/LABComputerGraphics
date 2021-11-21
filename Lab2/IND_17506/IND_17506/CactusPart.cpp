#include "pch.h"
#include "CactusPart.h"

CactusPart::CactusPart() { 
	for (int i = 0; i < 3; i++) {
		this->children[i] = nullptr;
	}
}

CactusPart::CactusPart(CactusPart** children, double* rotAngle, float initialRotation) {
	for (int i = 0; i < 3; i++) {
		this->children[i] = children[i];
	}
	this->rotAngle = rotAngle;
	this->initialRotation = initialRotation;
}

CactusPart::~CactusPart() { }
