#pragma once
#include "GameObject.h"

#include "define.h"

class CGoomba : public CGameObject
{
	int level;
	int start_level;
	int type; //color
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	int jumpCount = 0;
	DWORD fly_goomba_start_walking;
	bool Resetable = false;
	DWORD renderdie_start;
	bool isrenderdie = false;
public:
	void setLevel(int l)
	{
		level = l;
	}
	int getLevel() { return level; }

	void setType(int t)
	{
		type = t;
	}
	int getType() { return type; }

	CGoomba(int lvl);
	virtual void SetState(int state);
	void HitByTail();

	void LevelDown();

	void Reset();

	void Render_Tail_Hit();

	void FlyGoombaStartWalking()
	{
		fly_goomba_start_walking = GetTickCount64();
	}
	void StartRenderDie();
};