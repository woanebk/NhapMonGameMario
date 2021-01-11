#pragma once
#include "define.h"
#include "GameObject.h"

class Hud:public CGameObject
{
public:
	Hud();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	~Hud();
};

