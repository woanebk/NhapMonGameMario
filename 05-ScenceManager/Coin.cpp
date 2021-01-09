#include "Coin.h"
#include "PlayScence.h"
#include "Mario.h"
#include "BreakableBrick.h"

void CCoin::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CCoin::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (enable == false)
	{
		l = 0;
		t = 0;
		r = 0;
		b = 0;
		return;
	}
	l = x;
	t = y;
	r = x + COIN_BBOX_WIDTH;
	b = y + COIN_BBOX_HEIGHT;
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (!enable)
		return;
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	//
	if (canturntobreak && iscounting && GetTickCount64() - couting_change_to_brick_time > COIN_CHANGE_TO_BRICK_TIME)
	{
		//hide coin
		iscounting = false;
		couting_change_to_brick_time = 0;
		canturntobreak = false;
		enable = false;
		visable = false;
		//create breakable brick
		CBreakableBrick *breakablebrick = new CBreakableBrick(CANBOUNCE);
		breakablebrick->SetPosition(this->x, this->y);
		breakablebrick->SetStartPosition(this->x, this->y);

		CAnimationSets * animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(BRICK_SET_ID);
		breakablebrick->SetAnimationSet(ani_set);

		CPlayScene *currenscence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		currenscence->PushBackObject(breakablebrick);
	}

	if (coEvents.size() == 0)
	{
		// collide between mario and Coin
		float mario_bb_left, mario_bb_top, mario_bb_right, mario_bb_bottom;
		CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		CMario* mario = scence->GetPlayer();
		mario->GetBoundingBox(mario_bb_left, mario_bb_top, mario_bb_right, mario_bb_bottom);
		if (SpecialCollision(mario_bb_left, mario_bb_top, mario_bb_right, mario_bb_bottom))
		{
			enable = false;
			visable = false;
			mario->setOnGround(false);
		}
	}
	else
	{
		//float min_tx, min_ty, nx = 0, ny;
		//float rdx = 0;
		//float rdy = 0;


		//// TODO: This is a very ugly designed function!!!!
		//FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		//for (UINT i = 0; i < coEventsResult.size(); i++)
		//{
		//	LPCOLLISIONEVENT e = coEventsResult[i];
		//	if (dynamic_cast<CMario*>(e->obj))
		//	{
		//		

		//	}
		//}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}