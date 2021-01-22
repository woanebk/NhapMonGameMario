#pragma once
#include "GameObject.h"
#include "define.h"
#include "Boomerang.h"

class CBoomerangBro:public CGameObject
{
	bool holding_boomerang;
	bool has_boomerang;
	CBoomerang *boomerang;
	DWORD count_has_boomerang;
	DWORD count_throw_boomerang;
public:
	CBoomerangBro();
	virtual void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	~CBoomerangBro();
	void CreateBoomerang();
	void SetState(int state);
	void HitByTail();
	void Render_Tail_Hit();
	void setHoldingBoomerang(bool h) { holding_boomerang = h; if (h) count_throw_boomerang = (DWORD)GetTickCount64(); }
};

