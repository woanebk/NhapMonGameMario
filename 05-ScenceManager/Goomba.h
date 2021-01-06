#pragma once
#include "GameObject.h"

#include "define.h"

class CGoomba : public CGameObject
{
	int level;
	int start_level;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	int jumpCount = 0;
	DWORD fly_goomba_start_walking;
public:
	void setLevel(int l)
	{
		level = l;
	}
	int getLevel() { return level; }
	CGoomba(int lvl);
	virtual void SetState(int state);
	void HitByTail();

	void LevelDown();

	void Reset();

	void FlyGoombaStartWalking()
	{
		fly_goomba_start_walking = GetTickCount64();
	}
};