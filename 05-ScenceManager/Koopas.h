#pragma once

#include "GameObject.h"
#include "define.h"

class CKoopas : public CGameObject
{
	int level;
	int start_level;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	bool holded = false;
	bool Resetable = false;
public:
	CKoopas(int lvl);
	virtual void SetState(int state);
	void setHolded(bool h) { holded = h; }
	bool isHolded() { return holded; }
	void getHoldedbyMario();
	void HitByTail();
	void getKicked();

	void setLevel(int l)
	{
		level = l;
	}
	int getLevel() { return level; }
	void LevelDown();
	void Reset();
};