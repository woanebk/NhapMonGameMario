#pragma once
#include "GameObject.h"
#include "define.h"

class CBreakableBrick : public CGameObject
{
	int bounce = 0;

public:
	CBreakableBrick( int b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void HitByTail();
	int canBounce() { return bounce; }
	void setBounce(int b) { bounce = b; }

	~CBreakableBrick();
	void Break();
};

