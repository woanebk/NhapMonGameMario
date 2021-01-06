#include "Leaf.h"
#include "Utils.h"


CLeaf::CLeaf()
{
	vx = -LEAF_SPEED_x;
	turn_around_time = GetTickCount64();
}

void CLeaf::GetBoundingBox(float & left, float & top, float & right, float & bottom)
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
	right = x + LEAF_BBOX_WIDTH;
	bottom = y + LEAF_BBOX_HEIGHT;
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!enable)
		return;
	CGameObject::Update(dt, coObjects);

	// Simple fall down
	vy = LEAF_GRAVITY * dt;
	DebugOut(L"%d \t,%d \n ", vx, vy);
	if (GetTickCount64() - turn_around_time > LEAF_TURN_AROUND_TIME)
	{
		vx = -vx;
		turn_around_time = GetTickCount64();
	}

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

		x += dx;
		y += dy;

	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


void CLeaf::Render()
{
	if
		(vx<0)
		animation_set->at(LEAF_ANI_GO_LEFT)->Render(x, y);
	else
		animation_set->at(LEAF_ANI_GO_RIGHT)->Render(x, y);
}

CLeaf::~CLeaf()
{
}
