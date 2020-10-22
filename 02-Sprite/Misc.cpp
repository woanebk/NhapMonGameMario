#include "Misc.h"
#include "Game.h"

CMisc::CMisc(float x, float y, float vx):CGameObject(x, y)
{
	this->vx = vx;
}

void CMisc::Update(DWORD dt)
{
}

void CMisc::Render()
{
	LPANIMATION ani;
	ani = CAnimations::GetInstance()->Get(510);

	ani->Render(x, y);
}
