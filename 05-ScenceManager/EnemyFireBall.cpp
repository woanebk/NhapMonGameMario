#include "EnemyFireBall.h"
#include "Utils.h"
#include "Mario.h"


void CEnemyFireBall::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (!enable)
	{
		left = top = right = bottom = 0;
	}
	else
	{
		left = x;
		top = y;
		right = x + FIREBALL_BBOX_WIDTH;
		bottom = y + FIREBALL_BBOX_HEIGHT;
	}
}

void CEnemyFireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
}

void CEnemyFireBall::Render()
{
	animation_set->at(0)->Render(x, y);
}

CEnemyFireBall::CEnemyFireBall(float x, float y, int nx, int ny)
{
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	animation_set = animation_sets->Get(FIREBALL_SET_ID);
	vx = ENEMY_FIREBALL_SPEED_X * nx;
	vy = ENEMY_FIREBALL_SPEED_Y * ny;
	SetPosition(x, y);
	SetStartPosition(x, y);
}


CEnemyFireBall::~CEnemyFireBall()
{
}
