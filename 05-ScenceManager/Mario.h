#pragma once
#include "GameObject.h"
#include "define.h"



class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
	float ay = 0; //acceleration for jump

	bool is_icon = false;
	//status
	int Life;
	int Money;
	int Score;
	//jump
	bool isonground = false;
	bool jumpable = false;
	bool isjumping = false;
	bool flying = false;
	
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
	//transformation
	bool istransformingtoBig = false;
	bool istransformingtoLeaf = false;
	DWORD big_transform_start;
	DWORD leaf_transform_start;

	//pine down
	bool is_pine_down = false;
	DWORD start_pine_down;

	//pine up
	bool is_pine_up = false;
	DWORD start_pine_up;

	//icon movement on map:
	bool can_go_left = false;
	bool can_go_right = false;
	bool can_go_up = false;
	bool can_go_down = false;

	bool is_going_left = false;
	bool is_going_up = false;
	bool is_going_right = false;
	bool is_going_down = false;

	float icon_old_x;
	float icon_old_y;
	float icon_new_x;
	float icon_new_y;

	bool is_lost_control = false;

	bool can_select_scence = false;


	float start_x;			// initial position of Mario at scene
	float start_y; 

	int Stack; // running acceleration stack (7 stacks overall)
public: 
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void setLostControl(bool c) { is_lost_control = c; }
	void SetState(int state);
	void SetLevel(int l);
	void LevelUp();
	void LevelDown();
	void LifeUp();
	void LifeDown() { if (Life > 0)Life -= 1; }
	int getLevel() { return level; }

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	
	int getStack() { return Stack; }
	int getLife() { return Life; }
	int getMoney() { return Money; }
	int getScore() { return Score; }

	bool isIcon() { return is_icon; }
	void setIsIcon(bool i) { is_icon = i; }
	// hold to speed up
	void setSpeedUp(bool s) { speed_up = s; }
	bool isSpeedUp() { return speed_up; }
	void StartRunning() { speedup_start = GetTickCount(); speed_up = true; }
	void StopRunning() { speedup_stop = GetTickCount(); speed_up = false; }

	//fly
	bool isFlying() { return flying; }
	void setIsFlying(bool f) { flying = f; }
	
	//spinning tail
	DWORD getStartSpinning() { return spin_start; }
	void StartSpinning() { spin_start = GetTickCount(); spinning = true; } 
	bool isSpinning() { return spinning; }

	//flapping tail
	DWORD getStartFlapping() { return flap_start; }
	void StartFlapping() { flap_start = GetTickCount(); flapping = true; }

	//can jump or not
	void setOnGround(bool jump) { isonground = jump; }
	bool isOnGround() { return isonground; }
	bool isJumpable() { return jumpable; }
	void setJumpable(bool j) { jumpable = j; }
	//jumping
	void setIsJumping(bool j) { isjumping = j; }
	bool isJumping() { return isjumping; }
	//falling
	void setIsFalling(bool f) { isfalling = f; }
	bool isFalling() { return isfalling; }

	//holding
	void setHolding(bool h) { holding = h; }
	bool isHolding() { return holding; }

	//transform
	void startTransformtoBig() { istransformingtoBig = true; big_transform_start = GetTickCount64(); }
	void startTransFormtoLeaf() { istransformingtoLeaf = true; leaf_transform_start = GetTickCount64(); }
	bool isTransForming() { if (istransformingtoBig || istransformingtoLeaf) return true; return false; }

	// pine down
	void StartPineDown() { is_pine_down = true; start_pine_down = GetTickCount64(); }

	// pine up
	void StartPineUp() { is_pine_up = true; start_pine_up = GetTickCount64(); }

	//icon movement 
	bool canGoLeft() { return can_go_left; }
	bool canGoRight() { return can_go_right; }
	bool canGoUp() { return can_go_up; }
	bool canGoDown() { return can_go_down; }
	bool canSelectScecne() { return can_select_scence; }


	bool isLostControl() { return is_lost_control; }
	
	void ManageAccelerationAndSpeed();

	void Reset();

	void Shot();

	void Kick();

	bool isSpecialAnimation(int ani);

	void RenderSpecialAnimation(int ani);

	void TimingEvent();

	void GainMoney();

	void GainScore(int point);

	bool isEqual(float x, float y);

	void RenderPoint(int point);

	void CallEndScene();
	void EndScene();
	void StopCallEndScene();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};