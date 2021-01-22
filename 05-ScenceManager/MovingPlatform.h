#pragma once
#include "GameObject.h"
#include "define.h"

class CMovingPlatform:public CGameObject
{
	bool touched = false;
public:
	CMovingPlatform();
	virtual void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void setTouched(bool t) { touched = t; }
	~CMovingPlatform();
};

