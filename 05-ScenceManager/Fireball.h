#pragma once
#include "GameObject.h"

#include "define.h"

class CFireBall : public CGameObject
{
	DWORD switchSpeedtime = 0;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	CFireBall(float left, float top);
	void StartSwitchSpeed() { switchSpeedtime = GetTickCount(); }
};