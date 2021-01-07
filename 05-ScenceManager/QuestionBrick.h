#pragma once
#include "GameObject.h"
#include "define.h"

class CQuestionBrick :public CGameObject
{
	int bounce = 0;
	int type = 1;
	int Coin = 0;
	int Item = 0;
	DWORD jumptime = 0;
public:
	CQuestionBrick(int t, int b, int hasitem) {
		x = y = 0;
		vx = vy = 0;
		nx = 1;
		Item = hasitem;
		bounce = b;
		type = t;
		if (Item == 0)
			Coin = 1;
	}
	virtual void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void HitByTail();

	int canBounce() { return bounce; }
	void setBounce(int b) { bounce = b; }

	bool hasCoin() { if (Coin != 0) return true; return false; }
	void setHasCoin(int c) { Coin = c; }

	bool hasItem() { if (Item != 0) return true; return false; }
	void setHasItem(int r) { Item = r; }

	bool isEmpty();

	void getUsed();
	void CreateItem(int item);
	void Jump();
	int getType() { return type; }
	~CQuestionBrick();
};

