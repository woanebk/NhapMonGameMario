#include "Koopas.h"
#include "Brick.h"
#include "Utils.h"
#include "Mario.h"

CKoopas::CKoopas()
{
	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		if (state == KOOPAS_STATE_SHELL)
			bottom = y + KOOPAS_BBOX_HEIGHT_SHELL ;
		else
			if (state == KOOPAS_STATE_SPIN_LEFT || state == KOOPAS_STATE_SPIN_RIGHT)
				bottom = y + KOOPAS_BBOX_HEIGHT_SHELL ;
			else
		
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 

	// Simple fall down
	vy += MARIO_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != KOOPAS_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		if (vx < 0 && x < 0) {
			x = 0; vx = -vx;
		}

		if (vx > 0 && x > WORLD_1_1_WIDTH ) {
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
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		

		/*if (nx != 0) vx = 0;*/
		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->nx != 0)
				{
					
					vx = -vx;
				}
				

			}
			if (dynamic_cast<CMario*>(e->obj))
			{
				if (e->ny != 0) {}
			}
		}

		//
		// Collision logic with other objects
		//
		

	}
	//code mau cua thay
	/*x += dx;
	y += dy;

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}*/
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	DebugOut(L"koopas %d ", y);
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}
	else if (state == KOOPAS_STATE_SHELL) {
		ani = KOOPAS_ANI_SHELL;
	}
	else if (state == KOOPAS_STATE_SPIN_LEFT || state == KOOPAS_STATE_SPIN_RIGHT) {
		ani = KOOPAS_ANI_SPIN;
	}
	else if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
	else if (vx <= 0) ani = KOOPAS_ANI_WALKING_LEFT;

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_SHELL:
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_SPIN_LEFT:
		vx = -KOOPAS_SPINNING_SPEED;
		break;
	case KOOPAS_STATE_SPIN_RIGHT:
		vx = KOOPAS_SPINNING_SPEED;
		break;
	case KOOPAS_STATE_WALKING:
		vx = KOOPAS_WALKING_SPEED;
	}

}