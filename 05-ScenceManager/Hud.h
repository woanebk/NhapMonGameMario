#pragma once
#include "define.h"
#include "GameObject.h"

class Hud:public CGameObject
{
	int RemainingTime = GAME_LIMIT_TIME;
	int life;
	int score;
	int stack;
	int money;

	bool no_timing = false;
	vector<LPSPRITE> scoreSprite;
	vector<LPSPRITE> stackSprite;

	DWORD countdowntime;
public:
	Hud();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt);
	virtual void Render();
	~Hud();
	void setNoTiming(bool t) { no_timing = t; }

	LPSPRITE getNumberSprite(int num);
};

