#include "BoomerangBro.h"
#include "Pine.h"
#include "Mario.h"
#include "Brick.h"
#include "PlayScence.h"
#include "Effect.h"
#include "Boomerang.h"


CBoomerangBro::CBoomerangBro()
{
	SetState(BOOMERANG_BRO_STATE_WALK);
	holding_boomerang = false;
	has_boomerang = false;
	count_has_boomerang = GetTickCount64();
}

void CBoomerangBro::Render()
{
	if(holding_boomerang)
		animation_set->at(BOOMERANG_BRO_ANI_RIGHT_HOLD)->Render(x, y);
	else
		animation_set->at(BOOMERANG_BRO_ANI_RIGHT_THROW)->Render(x, y);
}

void CBoomerangBro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!enable )
		return;

	CGameObject::Update(dt, coObjects);
	vy += KOOPAS_FLY_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (state == BOOMERANG_BRO_STATE_DIE)
		y += dy;

	HitByTail();
	//logic for boomerang
	if (!has_boomerang && GetTickCount64() - count_has_boomerang > BOOMERANG_APPEAR_TIME)
	{
		count_has_boomerang = 0;
		CreateBoomerang();
		has_boomerang = true;
		holding_boomerang = true;
	}
	//
	if (has_boomerang && holding_boomerang)
	{
		boomerang->SetPosition(this->x - BOOMERANG_BRO_BBOX_WIDTH/2, this->y - BOOMERANG_BRO_BBOX_HEIGHT / 2);
	}

	if (has_boomerang && holding_boomerang && GetTickCount64() - count_throw_boomerang > BOOMERANG_HOLD_TIME)
	{
		boomerang->setIsFlying(true);
		holding_boomerang = false;
		count_throw_boomerang = 0;
	}


	if (x > start_x + BOOMERANG_BRO_DISTANCE_X && vx>0)
		vx = -vx;
	if (x < start_x && vx < 0)
		vx = -vx;

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

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

		// block every object first!
		/*x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;*/


		/*if (nx != 0) vx = 0;*/
		if (ny != 0)
		{
			vy = 0;
			y += min_ty * dy + ny * 0.4f;
		}

		float bb_left, bb_top, bb_right, bb_bottom;
		GetBoundingBox(bb_left, bb_top, bb_right, bb_bottom);
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			float obj_l, obj_t, obj_r, obj_b;
			e->obj->GetBoundingBox(obj_l, obj_t, obj_r, obj_b);

			if (dynamic_cast<CPine*>(e->obj))
			{
				CPine *pine = dynamic_cast<CPine*>(e->obj);
				if (e->nx != 0) {
					x += min_tx * rdx + nx * 0.4f;
					vx = -vx;
					break;
				}
				if (e->ny != 0) {
					vy = 0;
					y += min_ty * rdy + ny * 0.4f;
					x += dx;
				}
			}// if Pine
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick *brick = dynamic_cast<CBrick*>(e->obj);
				float brick_left, brick_top, brick_right, brick_bottom;
				brick->GetBoundingBox(brick_left, brick_top, brick_right, brick_bottom);

				if (brick->getType() == BRICK_TYPE_INVISIBLE)
				{
					x += dx; y += dy;
				}
				else
				{
					if (e->nx != 0)
					{
						if (brick_top < bb_bottom)
							vx = -vx;
						break;
					}
					if (e->ny != 0)
					{
						x += dx;
					}
				} // if Brick
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBoomerangBro::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (!enable)
	{
		l = t = r = b = 0;
	}
	else
	{
		l = x;
		t = y;
		r = x + BOOMERANG_BRO_BBOX_WIDTH;
		b = y + BOOMERANG_BRO_BBOX_HEIGHT;
	}
}


CBoomerangBro::~CBoomerangBro()
{
}

void CBoomerangBro::CreateBoomerang()
{
	boomerang = new CBoomerang();
	boomerang->SetPosition(this->x - BOOMERANG_BRO_BBOX_WIDTH, this->y- BOOMERANG_BRO_BBOX_HEIGHT /2);
	boomerang->SetStartPosition(this->x - BOOMERANG_BRO_BBOX_WIDTH, this->y- BOOMERANG_BRO_BBOX_HEIGHT / 2);

	CPlayScene *currenscence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	currenscence->PushBackObject(boomerang);

	boomerang->AddtoGrid();
	count_throw_boomerang = GetTickCount64();
}

void CBoomerangBro::SetState(int state)
{
	int old_state = this->state;
	CGameObject::SetState(state);
	switch (state)
	{
	case BOOMERANG_BRO_STATE_WALK:
		vx = BOOMERANG_BRO_SPEED_X;
		break;
	case BOOMERANG_BRO_STATE_DIE:
		vy = -BOOMERANG_BRO_DEFLECT_SPPED;
		break;
	}
}

void CBoomerangBro::HitByTail()
{
	float bb_left, bb_top, bb_right, bb_bottom;
	float mario_bb_left, mario_bb_top, mario_bb_right, mario_bb_bottom;

	GetBoundingBox(bb_left, bb_top, bb_right, bb_bottom);
	CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = scence->GetPlayer();
	mario->GetBoundingBox(mario_bb_left, mario_bb_top, mario_bb_right, mario_bb_bottom);
	if (mario->isSpinning())
		if (bb_left <= mario_bb_right + MARIO_LEAF_BBOX_TAIL_WIDTH && bb_right >= mario_bb_left - MARIO_LEAF_BBOX_TAIL_WIDTH)
			if (bb_top <= mario_bb_bottom && bb_bottom >= mario_bb_top + (mario_bb_bottom - mario_bb_top) / 2)
			{
				SetState(BOOMERANG_BRO_STATE_DIE);
				mario->GainScore(SCORE_1000);
				mario->RenderPoint(EFFECT_TYPE_1000_POINT);
				Render_Tail_Hit();

			}
}

void CBoomerangBro::Render_Tail_Hit()
{
	CEffect *tailhiteffect = new CEffect(EFFECT_TYPE_TAIL_HIT);
	tailhiteffect->SetPosition(this->x, this->y - BOOMERANG_BRO_BBOX_WIDTH / 2);
	tailhiteffect->SetStartPosition(this->x, this->y - BOOMERANG_BRO_BBOX_HEIGHT / 2);

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(EFFECT_SET_ID);
	tailhiteffect->SetAnimationSet(ani_set);

	CPlayScene *currenscence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	currenscence->PushBackObject(tailhiteffect);
	tailhiteffect->AddtoGrid();
}