#pragma once
#include "GameObject.h"

class CBrokenBrickPiece: public CGameObject
{
	bool flying = false;;
	DWORD flyling_start;
public:
	CBrokenBrickPiece(float vx, float vy);
	~CBrokenBrickPiece();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

