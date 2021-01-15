#include "Mushroom.h"
#include "PlayScence.h"
#include "Game.h"
#include "Pine.h"
#include "Mario.h"
#include "Brick.h"
#include "QuestionBrick.h"
#include "BreakableBrick.h"
#include "Block.h"
#include "Koopas.h"
#include "Goomba.h"

CMushroom::CMushroom(int t)
{
	type = t;
	
}

void CMushroom::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (enable == false)
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
		return;
	}
	left = x;
	top = y;
	right = x + MUSHROOM_BBOX_WIDTH;
	bottom = y + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*if (!enable)
		return;*/
	CGameObject::Update(dt, coObjects);

	// Simple fall down
	vy = MUSHROOM_GRAVITY * dt;


	HitMario();// in case item go through mario (fix bug)

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
			if (dynamic_cast<CPine*>(e->obj))
			{
				if (e->nx != 0)
				{
					vy = 0;
					vx = -vx;
					for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
					return;
				}
				else
				if (e->ny < 0)
				{
					x += dx;
					vy = 0;
				}
			}
			if (dynamic_cast<CBlock*>(e->obj))
			{
				if (e->ny < 0)
				{
					x += dx;
					vy = 0;
				}
				if (e->nx != 0)
				{
					x += dx;
				}
			}
			if (dynamic_cast<CBreakableBrick*>(e->obj))
			{
				CBreakableBrick *breakablebrick = dynamic_cast<CBreakableBrick*>(e->obj);
				if (e->nx != 0 && breakablebrick->canBounce())
				{
					vx = -vx;
					vy = 0;
					for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
					return;
				}
				if (e->ny < 0)
				{
					x += dx;
					vy = 0;
				}
			}
			if (dynamic_cast<CQuestionBrick*>(e->obj))
			{
				CQuestionBrick *questionbrick = dynamic_cast<CQuestionBrick*>(e->obj);
				/*if (e->nx != 0)
				{
					vx = -vx;
					vy = 0;
					x += dx;
					break;
				}*/
				if (e->ny < 0)
				{
					x += dx;
					vy = 0;
				}
			}
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick *brick = dynamic_cast<CBrick*>(e->obj);
				if (brick->getType() == BRICK_TYPE_INVISIBLE)
				{
					x += dx; y += dy;
				}//====INVISIBLE
				else
				{
					if (e->nx != 0 && brick->canBounce())
					{
						vx = -vx;
						x += dx;
						vy = 0;
					}
					if (e->nx != 0 && !brick->canBounce())
					{
						vy = 0;
						x += dx;
					}
					if (e->ny < 0)
					{
						x += dx;
						vy = 0;

						for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
						return;
					}
				}
			}
			if (dynamic_cast<CKoopas*>(e->obj))
			{
				x += dx;
			}
			if (dynamic_cast<CGoomba*>(e->obj))
			{
				x += dx;
			}
		} 
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


void CMushroom::Render()
{
	if(type == MUSHROOM_TYPE_RED)
		animation_set->at(MUSHROOM_ANI_RED)->Render(x, y);
	if(type == MUSHROOM_TYPE_GREEN)
		animation_set->at(MUSHROOM_ANI_GREEN)->Render(x, y);
	//RenderBoundingBox();
}

void CMushroom::HitMario()
{
	CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = scence->GetPlayer();
	float mario_bb_l, mario_bb_t, mario_bb_r, mario_bb_b;
	mario->GetBoundingBox(mario_bb_l, mario_bb_t, mario_bb_r, mario_bb_b);
	if (SpecialCollision(mario_bb_l, mario_bb_t, mario_bb_r, mario_bb_b))
	{
		mario->LevelUp();
		enable = false;
		visable = false;
	}
}
void CMushroom::ChooseDirection() //choose which direction is opposite to mario to run away
{
	CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = scence->GetPlayer();
	if (mario != NULL)
	{
		float mario_bb_l, mario_bb_t, mario_bb_r, mario_bb_b;
		mario->GetBoundingBox(mario_bb_l, mario_bb_t, mario_bb_r, mario_bb_b);

		float bb_l, bb_t, bb_r, bb_b;
		GetBoundingBox(bb_l, bb_t, bb_r, bb_b);
		if (mario->x + (mario_bb_r - mario_bb_l) / 2 > x + (bb_r - bb_l) / 2)
			vx = MUSHROOM_SPEED_X;
		else
			vx = -MUSHROOM_SPEED_X;
	}
}
CMushroom::~CMushroom()
{
}
