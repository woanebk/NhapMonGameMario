#pragma once
#include "GameObject.h"
#include "define.h"

class CQuestionBrick :public CGameObject
{
	int bounce = 0;
	int Coin = 0;
	int Reward = 0;
public:
	CQuestionBrick(int hasreward, int b) {
		x = y = 0;
		vx = vy = 0;
		nx = 1;
		Reward = hasreward;
		bounce = b;
		if (Reward == 0)
			Coin = 1;
	}
	virtual void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void HitByTail();

	int canBounce() { return bounce; }
	void setBounce(int b) { bounce = b; }

	int hasCoin() { return Coin; }
	void setHasCoin(int c) { Coin = c; }

	int hasReward() { return Reward; }
	void setHasReward(int r) { Reward = r; }

	bool isEmpty();

	void getUsed();
	~CQuestionBrick();
};

