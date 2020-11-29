#pragma once
#include "GameObject.h"
#include "define.h"



class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
	//jump
	bool jumpable = false;
	bool isjumping = false;
	//fall
	bool isfalling = false;
	//shoot
	vector <LPGAMEOBJECT> firebullets;
	//spin tail
	bool spinning = false;
	DWORD spin_start;
	//hold shell
	bool speed_up = false;
	bool holding = false;
	//flap
	bool flapping = false;
	DWORD flap_start;
	//kick
	bool kicking = false;
	DWORD kick_start;

	float start_x;			// initial position of Mario at scene
	float start_y; 
public: 
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int getLevel() { return level; }

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	
	// hold to speed up
	void setSpeedUp(bool s) { speed_up = s; }
	bool isSpeedUp() { return speed_up; }
	//spinning tail
	DWORD getStartSpinning() { return spin_start; }
	void StartSpinning() { spin_start = GetTickCount(); spinning = true; }

	//flapping tail
	DWORD getStartFlapping() { return flap_start; }
	void StartFlapping() { flap_start = GetTickCount(); flapping = true; }

	//can jump or not
	void setJumpable(bool jump) { jumpable = jump; }
	bool canJump() { return jumpable; }
	//jumping
	void setIsJumping(bool j) { isjumping = j; }
	bool isJumping() { return isjumping; }
	//falling
	void setIsFalling(bool f) { isfalling = f; }
	bool isFalling() { return isfalling; }

	//holding
	void setHolding(bool h) { holding = h; }
	bool isHolding() { return holding; }


	void Reset();

	void Shot();


	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};