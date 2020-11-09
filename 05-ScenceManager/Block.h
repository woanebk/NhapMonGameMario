#pragma once
#include "GameObject.h"

class CBlock : public CGameObject
{
	float width;
	float height;
public:
	CBlock(float l, float t, float r, float b);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Render();
};