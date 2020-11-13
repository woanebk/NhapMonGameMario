#include "Fireball.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Brick.h"
#include "Pine.h"
#include "Block.h"
#include "Utils.h"

CFireBall::CFireBall(float left, float top)
{
	x = left;
	y = top;
	vx = FIREBALL_SPEED_X;
	vy = FIREBALL_SPEED_Y;
	nx = 1;
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	animation_set = animation_sets->Get(6);
}

void CFireBall::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + FIREBALL_BBOX_WIDTH;
	bottom = y + FIREBALL_BBOX_HEIGHT;
}

void CFireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (vy < 0)
	{
		if (GetTickCount() - switchSpeedtime >= 100)
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

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		/*x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;*/

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick *brick = dynamic_cast<CBrick*>(e->obj);
				if (e->nx != 0)
				{
					if(brick->canBounce() == 1)
					{
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
				goomba->SetState(GOOMBA_STATE_DIE);
				enable = false;
				visable = false;
			}
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas *koopas = dynamic_cast<CKoopas*>(e->obj);
				koopas->SetState(KOOPAS_STATE_DIE);
				enable = false;
				visable = false;
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


