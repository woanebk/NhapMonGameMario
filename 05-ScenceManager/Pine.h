#pragma once
#include "GameObject.h"

/*
	Green pine
*/
class CPine : public CGameObject
{	

	int width;
	int height;
public:
	CPine (float l, float t, float r, float b);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};