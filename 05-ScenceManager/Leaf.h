#pragma once
#include "GameObject.h"
#include "define.h"

class CLeaf : public CGameObject
{
	DWORD jumptime = 0;
	bool isjumping = false;
public:
	CLeaf();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	DWORD turn_around_time;
	~CLeaf();
	void HitMario();
	void Jump();
	void ManageJumping();
};

