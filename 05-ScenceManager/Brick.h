#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class CBrick : public CGameObject
{
	int bounce = 0;
public:
	CBrick(int b) {
		x = y = 0;
		vx = vy = 0;
		nx = 1;
		bounce = b;
	}
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int canBounce() { return bounce; }
	void setBounce(int b) { bounce = b; }
};