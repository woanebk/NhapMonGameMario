#include "Fireball.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Brick.h"
#include "Pine.h"
#include "Block.h"
#include "Coin.h"
#include "BreakableBrick.h"
#include "EnemyFireBall.h"
#include "QuestionBrick.h"
#include "Utils.h"
#include "PiranhaPlant.h"

CFireBall::CFireBall(float left, float top)
{
	x = left;
	y = top;
	vx = FIREBALL_SPEED_X;
	vy = FIREBALL_SPEED_Y;
	nx = 1;
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	animation_set = animation_sets->Get(FIREBALL_SET_ID);
}

void CFireBall::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (!enable)
		left = top = right = bottom = 0;
	else
	{
		left = x;
		top = y;
		right = x + FIREBALL_BBOX_WIDTH;
		bottom = y + FIREBALL_BBOX_HEIGHT;
	}
}

void CFireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!enable)
		return;
	CGameObject::Update(dt, coObjects);
	if (vy < 0)
	{
		if (GetTickCount() - switchSpeedtime >= FIREBALL_REVERSE_SPEED_TIME)
		{
			SetSpeed(vx, -vy);
		}
	} // reverse speed y

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
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick *brick = dynamic_cast<CBrick*>(e->obj);
				if (brick->getType() == BRICK_TYPE_INVISIBLE)
				{
					x += dx; y == dy;
				}
				else
				{
					if (e->nx != 0)
					{
						if (brick->canBounce() == 1 || brick->getType() == BRICK_TYPE_CLOUD)
						{
							RenderExplosion();
							enable = false;
							visable = false;
						}// fix dissapear on ground, now only dissapear when hitting an edge
					}
					if (e->ny != 0)
					{
						SetSpeed(vx, -vy);
						StartSwitchSpeed();
					}
				}
			}
			else if (dynamic_cast<CBlock*>(e->obj))
			{
				CBlock *block = dynamic_cast<CBlock*>(e->obj);
				if (e->nx != 0)
				{
					x += dx;
					y += dy;
				}
				if (e->ny != 0)
				{
					SetSpeed(vx, -vy);
					StartSwitchSpeed();
				}
			}
			else if (dynamic_cast<CPine*>(e->obj))
			{
				CPine *pine = dynamic_cast<CPine*>(e->obj);
				if (e->nx != 0)
				{
					enable = false;
					visable = false;
				}
				if (e->ny != 0)
				{
					SetSpeed(vx, -vy);
					StartSwitchSpeed();
				}
			}
			else if (dynamic_cast<CGoomba*>(e->obj))
			{
				CGoomba *goomba = dynamic_cast<CGoomba*>(e->obj);
				goomba->Render_Tail_Hit();
				goomba->SetState(GOOMBA_STATE_DIE);
				enable = false;
				visable = false;
			}
			else if (dynamic_cast<CPiranhaPlant*>(e->obj))
			{
				CPiranhaPlant *plant = dynamic_cast<CPiranhaPlant*>(e->obj);
				plant->Killed();
				enable = false;
				visable = false;
			}
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas *koopas = dynamic_cast<CKoopas*>(e->obj);
				koopas->Render_Tail_Hit();
				koopas->SetState(KOOPAS_STATE_DIE);
				enable = false;
				visable = false;
			}
			else if (dynamic_cast<CCoin*>(e->obj))
			{
				CCoin *coin = dynamic_cast<CCoin*>(e->obj);
				x += dx;
				y += dy;
			}
			if (dynamic_cast<CQuestionBrick*>(e->obj))
			{
				CQuestionBrick *brick = dynamic_cast<CQuestionBrick*>(e->obj);
				if (e->nx != 0)
				{
					if (brick->canBounce() == 1)
					{
						RenderExplosion();
						enable = false;
						visable = false;
					}// fix dissapear on ground, now only dissapear when hitting an edge
				}
				if (e->ny != 0)
				{
					SetSpeed(vx, -vy);
					StartSwitchSpeed();
				}
			}
			if (dynamic_cast<CBreakableBrick*>(e->obj))
			{
				CBreakableBrick *brick = dynamic_cast<CBreakableBrick*>(e->obj);
				if (e->nx != 0)
				{
					if (brick->canBounce() == 1)
					{
						RenderExplosion();
						enable = false;
						visable = false;
					}// fix dissapear on ground, now only dissapear when hitting an edge
				}
				if (e->ny != 0)
				{
					SetSpeed(vx, -vy);
					StartSwitchSpeed();
				}
			}
			if (dynamic_cast<CEnemyFireBall*>(e->obj))
			{
				x += dx;
				y += dy;
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CFireBall::Render()
{
	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}

void CFireBall::RenderExplosion()
{
	//CAnimations * animations = CAnimations::GetInstance();
	
}



