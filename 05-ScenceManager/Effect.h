#pragma once
#include "GameObject.h"
#include "define.h"

class CEffect: public CGameObject
{
	int type = 0;
	DWORD effect_start;
public:
	CEffect(int t);
	virtual void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Start_Render_Effect() { effect_start = GetTickCount64(); }
	~CEffect();
};

