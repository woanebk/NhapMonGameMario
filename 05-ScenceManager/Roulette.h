#pragma once
#include "GameObject.h"

class CRoulette:public CGameObject
{
	int card = 0;
	bool achived = false;
	DWORD count_start;
	DWORD end_course_start;
	DWORD end_scene_start;
public:
	CRoulette();
	~CRoulette();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void setAchived(bool a) { achived = a; }
	void Achived();
	void CallEndScene();
};

