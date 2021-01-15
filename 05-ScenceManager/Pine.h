#pragma once
#include "GameObject.h"

/*
	Green pine
*/
class CPine : public CGameObject
{	

	int width;
	int height;
	int scene_id = -1;
	bool can_switch_scence=false;
	int type; // animation 
public:
	CPine (float l, float t, float r, float b, int id, int ty );
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	bool canSwitchScence() { return can_switch_scence; }
	int getScenceID() { return scene_id; }
};