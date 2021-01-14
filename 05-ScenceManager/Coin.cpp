#include "Coin.h"
#include "PlayScence.h"
#include "Mario.h"
#include "Effect.h"
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

void CCoin::Jump()
{
	jumptime = GetTickCount64();
	isjumping = true;
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (!enable)
		return;
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	y += dy;
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

	if (isjumping)
		vy = -COIN_JUMP_SPEED_Y;

	// jump and go back to old position======
	if (isjumping && GetTickCount64() - jumptime > COIN_JUMP_TIME)
	{
		isjumping = false;
		jumptime = 0;
		vy = COIN_JUMP_SPEED_Y;
		

	}
	if (y > start_y)
	{
		y = start_y;
		vy = 0;
		RenderPoint(EFFECT_TYPE_100_POINT);
		CPlayScene *currenscence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		CMario *mario = currenscence->GetPlayer();
		mario->GainScore(SCORE_100);
		visable = false;
		enable = false;
	}
	//==========================================

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

void CCoin::RenderPoint(int type)
{
	CEffect *point_effect = new CEffect(type);
	point_effect->SetPosition(this->x, this->y - BRICK_BBOX_HEIGHT);
	point_effect->SetStartPosition(this->x, this->y - BRICK_BBOX_HEIGHT);

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(EFFECT_SET_ID);
	point_effect->SetAnimationSet(ani_set);

	CPlayScene *currenscence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	currenscence->PushBackObject(point_effect);
}