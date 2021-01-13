#include "Brick.h"
#include "PlayScence.h"
#include "BreakableBrick.h"
#include "Coin.h"
#include "define.h"

void CBrick::Render()
{
	if (type == BRICK_TYPE_WOOD)
		animation_set->at(BRICK_ANI_WOOD)->Render(x, y);
	else
		if (type == BRICK_TYPE_QUESTION)
			animation_set->at(BRICK_ANI_QUESTION)->Render(x, y);
		/*else
			if (type == BRICK_TYPE_GROUND_1)
				animation_set->at(BRICK_ANI_GROUND_1)->Render(x, y);
			else
				if (type == BRICK_TYPE_GROUND_2)
					animation_set->at(BRICK_ANI_GROUND_2)->Render(x, y);*/
				else
					if (type == BRICK_TYPE_CLOUD)
						animation_set->at(BRICK_ANI_CLOUD)->Render(x, y);
					else if (type == BRICK_TYPE_MONEYBUTTON)
					{
						if(!Pressed)
							animation_set->at(BRICK_ANI_MONEY_BUTTON)->Render(x, y);
						else
							animation_set->at(BRICK_ANI_MONEY_BUTTON_PRESSED)->Render(x, y);
					}
					else if(type == BRICK_TYPE_BASEMENT)
						animation_set->at(BRICK_ANI_BASEMENT)->Render(x, y);
					else if (type == BRICK_TYPE_TREE_ICON)
						animation_set->at(BRICK_ANI_TREE_ICON)->Render(x, y);
						
					
	RenderBoundingBox();
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (type != BRICK_TYPE_MONEYBUTTON && !Pressed)
		return;// optimize performance
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		// collide between mario and moneybutton
		float mario_bb_left, mario_bb_top, mario_bb_right, mario_bb_bottom;
		CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		CMario* mario = scence->GetPlayer();
		mario->GetBoundingBox(mario_bb_left, mario_bb_top, mario_bb_right, mario_bb_bottom);
		if (SpecialCollision(mario_bb_left, mario_bb_top, mario_bb_right, mario_bb_bottom) && type == BRICK_TYPE_MONEYBUTTON && !Pressed)
		{
			ChangeBreakableBricktoCoin();
			Pressed = true;
		}
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
			if (dynamic_cast<CMario*>(e->obj))
			{
				if (type == BRICK_TYPE_MONEYBUTTON && !Pressed)
				{
					ChangeBreakableBricktoCoin();
					Pressed = true;
				}
					
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (type == BRICK_TYPE_TREE_ICON)
		l = t = r = b = 0;
	else
	{
		l = x;
		t = y;
		r = x + BRICK_BBOX_WIDTH;
		b = y + BRICK_BBOX_HEIGHT;
	}
}


void CBrick::ChangeBreakableBricktoCoin()
{
	CPlayScene *currenscence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	vector<LPGAMEOBJECT> objects = currenscence->GetObjectList();
	for (UINT i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<CBreakableBrick*>(objects[i]))
		{
			CBreakableBrick *breakablebrick = dynamic_cast<CBreakableBrick*>(objects[i]);
			if (!breakablebrick->isBroken())
			{
				breakablebrick->enable = false;
				breakablebrick->visable = false;

				CCoin *coin = new CCoin();
				coin->SetPosition(breakablebrick->x, breakablebrick->y);
				coin->SetStartPosition(breakablebrick->x, breakablebrick->y);

				CAnimationSets * animation_sets = CAnimationSets::GetInstance();
				LPANIMATION_SET ani_set = animation_sets->Get(COIN_SET_ID);
				coin->SetAnimationSet(ani_set);

				coin->setCanTurnToBrick(true);
				currenscence->PushBackObject(coin);
				coin->StartCountingTime();
			}

		}
	}

}
