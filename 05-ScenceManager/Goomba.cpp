#include "Goomba.h"
#include "Koopas.h"
#include "Brick.h"
#include "Pine.h"
#include "Block.h"
CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
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
	right = x + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else 	
		bottom = y + GOOMBA_BBOX_HEIGHT;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	// Simple fall down
	vy += MARIO_GRAVITY * dt;
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (vx > 0 && x > WORLD_1_1_WIDTH) {
			x = WORLD_1_1_WIDTH; vx = -vx;
		}
		
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
			if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas *koopas = dynamic_cast<CKoopas *>(e->obj);
				if (e->nx != 0)
				{
					if (koopas->GetState() == KOOPAS_STATE_SPIN_LEFT | koopas->GetState() == KOOPAS_STATE_SPIN_RIGHT)
						SetState(GOOMBA_STATE_DIE); //die if get hit by a spinning koopas
					else
						vx = -vx; //turn if meet an alive koopas
				}

			} //if koopas
			else if (dynamic_cast<CBlock*>(e->obj)) 
			{
				CBlock *block = dynamic_cast<CBlock*>(e->obj);
				if (e->ny < 0) {
					vy = 0;
					y += min_ty * rdy + ny * 0.4f;
					x += dx;
				}
				if (e->nx != 0)
				{
					vy = 0;
					x += dx;
				}
			}// if Block
			else
				if (dynamic_cast<CPine*>(e->obj))
				{
					CPine *pine = dynamic_cast<CPine*>(e->obj);
					if (e->nx != 0) {
						x += min_tx * rdx + nx * 0.4f;
						vx = -vx;
					}
					if (e->ny != 0) {
						vy = 0;
						y += min_ty * rdy + ny * 0.4f;
					}
				}// if Pine
			else
				if (dynamic_cast<CGoomba*>(e->obj))
				{
					CGoomba *goomba = dynamic_cast<CGoomba*>(e->obj);
					if (e->nx != 0) {
						x += min_tx * rdx + nx * 0.4f;
						vx = -vx;
					}
					
				}// if Goomba
				else
					if (dynamic_cast<CBrick*>(e->obj))
					{
						CBrick *brick = dynamic_cast<CBrick*>(e->obj);
						if (e->ny != 0)
						{
							vy = 0;
							y += min_ty * rdy + ny * 0.4f;
						}
						if (e->nx != 0)
						{
							x += min_tx * dx + nx * 0.4f;
							if (brick->canBounce() == 1)
								vx = -vx;
						}
					}// if brick
		}

	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}

	animation_set->at(ani)->Render(x,y);

	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
			enable = false;
			visable = false;
			vx = 0;
			vy = 0;
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
	}
}
