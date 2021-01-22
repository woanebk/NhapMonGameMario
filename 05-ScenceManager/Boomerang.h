#pragma once
#include "GameObject.h"
#include "define.h"
class CBoomerang: public CGameObject
{
	bool is_flying = false;
	float start_flying_x;
	float start_flying_y;
public:
	CBoomerang();
	virtual void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	bool isFlying() { return is_flying; }
	void setIsFlying(bool f) 
	{ 
		is_flying = f; 
		if (f)
		{
			start_flying_x = x;
			start_flying_y = y;
		}
		else
		{
			start_flying_x = 0;
			start_flying_y = 0;
		}
		
	}
	~CBoomerang();
};

