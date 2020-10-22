#pragma once

#include "GameObject.h"


class CMisc : public CGameObject
{
protected:
	float vx;
public:
	CMisc (float x, float y, float vx);
	void Update(DWORD dt);
	void Render();
};