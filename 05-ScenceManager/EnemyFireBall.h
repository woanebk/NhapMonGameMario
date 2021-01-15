#pragma once
#include "GameObject.h"

#include "define.h"
class CEnemyFireBall:public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
public:
	CEnemyFireBall(float x, float y, int nx, int ny);
	~CEnemyFireBall();
};

