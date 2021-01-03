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
	//hold shell and speed up
	bool holding = false;
	bool speed_up = false;
	DWORD speedup_start;
	DWORD speedup_stop;
	//flap
	bool flapping = false;
	DWORD flap_start;
	//kick
	bool kicking = false;
	DWORD kick_start;
	//is changing direction when brake
	bool ischangingdirection = false;

	float start_x;			// initial position of Mario at scene
	float start_y; 

	int Stack; // running acceleration stack (7 stacks overall)
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
	void StartRunning() { speedup_start = GetTickCount(); speed_up = true; }
	void StopRunning() { speedup_stop = GetTickCount(); speed_up = false; }
	
	//spinning tail
	DWORD getStartSpinning() { return spin_start; }
	void StartSpinning() { spin_start = GetTickCount(); spinning = true; }
	bool isSpinning() { return spinning; }

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
	
	void ManageAccelerationAndSpeed();

	void Reset();

	void Shot();

	bool isSpecialAnimation(int ani);

	void RenderSpecialAnimation(int ani);

	void TimingEvent();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};