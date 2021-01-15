#pragma once

#include "GameObject.h"
#include "define.h"

class CKoopas : public CGameObject
{
	int level;
	int start_level;
	int type = KOOPAS_TYPE_GREEN;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	bool holded = false;
	bool Resetable = false;
	bool is_shell_up = false;
	bool is_reviving = false;
	DWORD start_revive;
public:
	CKoopas(int lvl, int t);
	virtual void SetState(int state);
	void setHolded(bool h) { holded = h; }
	bool isHolded() { return holded; }
	void getHoldedbyMario();
	void HitByTail();
	void Render_Tail_Hit();
	void getKicked();
	void StartRevive() { start_revive = GetTickCount64(); is_reviving = true; }

	void setLevel(int l)
	{
		level = l;
	}
	int getLevel() { return level; }
	void LevelDown();
	void Reset();
};