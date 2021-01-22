#include "MovingPlatform.h"



CMovingPlatform::CMovingPlatform()
{
}

void CMovingPlatform::Render()
{
	animation_set->at(MOVING_PLATFORM_ANI)->Render(x, y);
	RenderBoundingBox();
}

void CMovingPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	if (!touched)
	{
		vx = -MOVING_PLATFORM_SPEED_X;
		vy = 0;
	}
	else
	{
		vx = 0;
		vy +=MOVING_PLATFORM_GRAVITY;
	}
}


void CMovingPlatform::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + MOVING_PLATFORM_WIDTH;
	b = y + MOVING_PLATFORM_HEIGHT;
}

CMovingPlatform::~CMovingPlatform()
{
}
