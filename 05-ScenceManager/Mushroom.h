#pragma once
#include "GameObject.h"
#include "define.h"

class CMushroom: public CGameObject
{
	int type = MUSHROOM_TYPE_RED;
public:
	CMushroom(int t);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void HitMario();
	void ChooseDirection();
	~CMushroom();
	int getType() { return type; }
};

