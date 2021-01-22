#include "MovingPlatform.h"
#include "Mario.h"
#include "QuestionBrick.h"
#include "BreakableBrick.h"
#include "Game.h"
#include "PlayScence.h"


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
	
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario *mario = scene->GetPlayer();
	coObjects->push_back(mario);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
		
			if (dynamic_cast<CQuestionBrick*>(e->obj))
			{
				x += dx;
				y += dy;
			}
			if (dynamic_cast<CBreakableBrick*>(e->obj))
			{
				x += dx;
				y += dy;
			}
			if (dynamic_cast<CMario*>(e->obj))
			{
				CMario *mario = dynamic_cast<CMario*>(e->obj);
				if(e->nx!=0 )
					e->obj->x -= (min_tx * dx + nx * 0.6f);
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
