#pragma once
#include "GameObject.h"
#include "define.h"

class CCoin : public CGameObject
{
	bool canturntobreak = false;
	DWORD couting_change_to_brick_time;
	bool iscounting = false;
	bool isjumping = false;
	DWORD jumptime = 0;
public:
	virtual void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void RenderPoint(int type);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	bool canTurnToBrick() { return canturntobreak;}
	void setCanTurnToBrick(bool c) { canturntobreak = c; }
	void StartCountingTime() { iscounting = true; couting_change_to_brick_time = GetTickCount64(); }
	void Jump();
	void setIsjumping(bool i) { isjumping = i; }
};