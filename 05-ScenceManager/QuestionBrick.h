#pragma once
#include "GameObject.h"
#include "define.h"

class CQuestionBrick :public CGameObject
{
	int bounce = 0;
	int Coin = 0;
	int Item = 0;
public:
	CQuestionBrick(int hasreward, int b) {
		x = y = 0;
		vx = vy = 0;
		nx = 1;
		Item = hasreward;
		bounce = b;
		if (Item == 0)
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

	int hasItem() { return Item; }
	void setHasItem(int r) { Item = r; }

	bool isEmpty();

	void getUsed();
	void CreateItem(int item);
	~CQuestionBrick();
};

