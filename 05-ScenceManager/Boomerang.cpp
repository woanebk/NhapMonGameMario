#include "Boomerang.h"
#include "BoomerangBro.h"
#include "Effect.h"
#include "PlayScence.h"
#include "Mario.h"


CBoomerang::CBoomerang()
{
	LPANIMATION_SET ani_set = CAnimationSets::GetInstance()->Get(BOOMERANG_BRO_SET_ID);
	SetAnimationSet(ani_set);
	//set defalut speed
	vx = BOOMERANG_SPEED_X;
	vy = -BOOMERANG_SPEED_Y;
}

void CBoomerang::Render()
{
	if(is_flying)
		animation_set->at(BOOMERANG_ANI_FLY)->Render(x, y);
	else
		animation_set->at(BOOMERANG_ANI_HOLDED)->Render(x, y);
}

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!enable)
		return;
	CGameObject::Update(dt, coObjects);


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	x += dx;
	y += dy;

	if (is_flying)
	{
		if (vx > 0 && vy <0 && x > start_flying_x + 64)
			vy = BOOMERANG_SPEED_Y;
		if (vx > 0 && vy > 0 && x > start_flying_x + 128)
		{
			vx = -BOOMERANG_SPEED_X;
		}
		if (vx < 0 && vy > 0 && x < start_flying_x + 80)
		{
			vy = 0;
			vx = -BOOMERANG_SPEED_X;
		}
	}

	if (coEvents.size() == 0)
	{
		if (vx > 0 && x > WORLD_1_1_WIDTH) {
			vx = -vx;
			x = WORLD_1_1_WIDTH;
		}
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		float bb_left, bb_top, bb_right, bb_bottom;
		GetBoundingBox(bb_left, bb_top, bb_right, bb_bottom);
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBoomerangBro*>(e->obj))
			{
				CBoomerangBro *boomerangbro = dynamic_cast<CBoomerangBro*>(e->obj);
				boomerangbro->setHoldingBoomerang(true);
				is_flying = false;
				//reset defalut speed
				vx = BOOMERANG_SPEED_X;
				vy = -BOOMERANG_SPEED_Y;
			}
		}
		
	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}



void CBoomerang::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (!enable)
		l = t = r = b = 0;
	else
	{
		l = x;
		t = y;
		r = l + BOOMERANG_BBOX_WIDTH;
		b = t + BOOMERANG_BBOX_HEIGHT;
	}
}

CBoomerang::~CBoomerang()
{

}
