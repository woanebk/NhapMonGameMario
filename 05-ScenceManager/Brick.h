#pragma once
#include "GameObject.h"
#include "define.h"



class CBrick : public CGameObject
{
	int bounce = 0;
	int type = BRICK_TYPE_WOOD;
	bool Pressed = false;
public:
	CBrick(int t,int b) {
		x = y = 0;
		vx = vy = 0;
		nx = 1;
		type = t;
		bounce = b;
	}
	virtual void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void ChangeBreakableBricktoCoin();
	int canBounce() { return bounce; }
	void setBounce(int b) { bounce = b; }

	int getType() { return type; }
	void setType(int t) { type = t; }
};