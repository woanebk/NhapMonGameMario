#pragma once
#include"GameObject.h"

class Leaf: CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
public:
	Leaf();
	~Leaf();
};

