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
	CQuestionBrick(int t, int b, int hasitem, int hascoin) {
		x = y = 0;
		vx = vy = 0;
		nx = 1;
		Item = hasitem;
		bounce = b;
		type = t;
		Coin = hascoin;
	}
	virtual void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	void HitByTail();

	void Render_Tail_Hit();

	int canBounce() { return bounce; }
	void setBounce(int b) { bounce = b; }

	bool hasCoin() { if (Coin != 0) return true; return false; }
	void setCoin(int c) { if(c >= 0)Coin = c; }
	int getCoin() { return Coin; }

	bool hasItem() { if (Item != 0) return true; return false; }
	void setHasItem(int r) { Item = r; }

	bool isEmpty();

	void getUsed();
	void CreateItem(int item);
	void Jump();
	int getType() { return type; }
	~CQuestionBrick();
};

