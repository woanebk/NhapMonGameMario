#pragma once

#include "GameObject.h"
#include "define.h"

class CKoopas : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	bool holded = false;

public:
	CKoopas();
	virtual void SetState(int state);
	void setHolded(bool h) { holded = h; }
	bool isHolded() { return holded; }
	void getHoldedbyMario();
	void HitByTail();
};