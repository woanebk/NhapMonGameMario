#include "Roulette.h"
#include "define.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScence.h"

CRoulette::CRoulette()
{
	count_start = (DWORD)GetTickCount64();
	vx = 0;
	vy = 0;
}


CRoulette::~CRoulette()
{
}

void CRoulette::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + ROULETTE_BBOX_WIDTH;
	b = y + ROULETTE_BBOX_HEIGHT;
	if (!enable)
		l = t = r = b = 0;
}

void CRoulette::Render()
{
	if (!achived)
	{
		if (card == ROULETTE_MUSHROOM_ID)
			animation_set->at(MUSHROOM_ANI_ROULETTE)->Render(x, y);
		else if(card == ROULETTE_FLOWER_ID)
			animation_set->at(FLOWER_ANI_ROULETTE)->Render(x, y);
		else if (card == ROULETTE_STAR_ID)
			animation_set->at(STAR_ANI_ROULETTE)->Render(x, y);
	}
	else
	{
		if (card == ROULETTE_MUSHROOM_ID)
			animation_set->at(MUSHROOM_ANI_ROULETTE_ACHIVED)->Render(x, y);
		else if (card == ROULETTE_FLOWER_ID)
			animation_set->at(FLOWER_ANI_ROULETTE_ACHIVED)->Render(x, y);
		else if (card == ROULETTE_STAR_ID)
			animation_set->at(STAR_ANI_ROULETTE_ACHIVED)->Render(x, y);
	}

	//
	float cam_x, cam_y;
	CGame *game = CGame::GetInstance();
	game->GetCamPos(cam_x, cam_y);
	CPlayScene *scene = (CPlayScene*)game->GetCurrentScene();
	CMario *mario = (CMario*)scene->GetPlayer();
	
	if (achived && GetTickCount64() - end_course_start >= END_COURSE_TIME)
	{
		CSprites::GetInstance()->Get(END_COURSE_SPRITE_ID)->Draw(cam_x + END_COURSE_X, cam_y);
		end_course_start = 0;
		
	}
	

	
}

void CRoulette::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);

	CGameObject::Update(dt, coObjects);
	y += dy;
	if (!achived)
	{
		vy = 0;
	}
	else
		vy = -0.1f;
	/*if (y < 100)
		vy = 0;*/
	//change roulette
	if (!achived && GetTickCount64() - count_start >= CHANGE_ROULETTE_TIME)
	{
		if (card < ROULETTE_STAR_ID)
			card++;
		else
			card = ROULETTE_MUSHROOM_ID;
		count_start = (DWORD)GetTickCount64();
	}

	if (achived && GetTickCount64() - end_scene_start >= END_SCENE_TIME)
	{
		end_scene_start = 0;
		CGame *game = CGame::GetInstance();
		CPlayScene *scene = (CPlayScene*)game->GetCurrentScene();
		CMario *mario = (CMario*)scene->GetPlayer();
		mario->SetSpeedX(MARIO_WALKING_SPEED);
		mario->setLostControl(true);
		CallEndScene();// call hud to end scene and set time to 300
	}
}

void CRoulette::Achived()
{
	achived = true;
	end_course_start = (DWORD)GetTickCount64();
	end_scene_start = (DWORD)GetTickCount64();
}

void CRoulette::CallEndScene()
{
	CGame *game = CGame::GetInstance();
	CPlayScene *scene = (CPlayScene*)game->GetCurrentScene();
	Hud *hud = scene->getHud();
	hud->EndScene();
}