#pragma once
#include "GameObject.h"
#include "define.h"

class CPiranhaPlant:public CGameObject
{
	int type; //color
	int level;
	int ny = 1;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	bool is_up = false;
	bool is_down = false;
	bool is_stand = false;
	bool can_shot = false;
	DWORD up_start;
	DWORD down_start;
	DWORD stand_start;
	DWORD shot_start;
public:
	CPiranhaPlant(int t, int level);
	~CPiranhaPlant();

	void HitByTail();

	void Render_Tail_Hit();

	void DetectMario();

	void StartUp() { up_start = GetTickCount64(); is_up = true; }
	void StartDown() { down_start = GetTickCount64(); is_down = true; }
	void StandStill() { stand_start = GetTickCount64();  is_stand = true; }
	void StartCalculateShot() { shot_start = GetTickCount64(); can_shot = true; }
	void Shot();
};

