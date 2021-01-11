#include "Hud.h"
#include "Game.h"
#include "define.h"

#define	HUD_SPRITE_ID	14000

Hud::Hud()
{
}

void Hud::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void Hud::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void Hud::Render()
{
	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);
	CSprites::GetInstance()->Get(HUD_SPRITE_ID)->Draw(cam_x, cam_y +SCREEN_HEIGHT - HUD_HEIGHT*2 + 1);
}


Hud::~Hud()
{
}
