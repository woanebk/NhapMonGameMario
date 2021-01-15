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
	bool is_ending_scence = false;
	DWORD start_end_scene;
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
	void setTime(int t) { RemainingTime = t; }

	LPSPRITE getNumberSprite(int num);
	void EndScene();
	void StartEndScene() { is_ending_scence = true; start_end_scene = GetTickCount64(); }
	void StopEndingScene() { is_ending_scence = false; start_end_scene = 0; }
};

