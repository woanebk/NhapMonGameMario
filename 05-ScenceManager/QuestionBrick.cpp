#include "QuestionBrick.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Leaf.h"
#include "Mushroom.h"

bool CQuestionBrick::isEmpty()
{
	if (Coin == 0 && Item == 0) return true;
	return false;
}

void CQuestionBrick::getUsed()
{
	Coin = 0;
	Item = 0;
}

void CQuestionBrick::CreateItem(int item)
{
	if (item == ITEM_LEAF)
	{
		CLeaf *leaf = new CLeaf();
		leaf->SetPosition(this->x, this->y);
		leaf->SetStartPosition(this->x, this->y);

		CAnimationSets * animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(ITEM_SET_ID);
		leaf->SetAnimationSet(ani_set);

		CPlayScene *currenscence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		currenscence->PushBackObject(leaf);
		leaf->Jump();
	}
	if (item == ITEM_MUSHROOM_RED)
	{
		CMushroom *red_mushroom = new CMushroom(MUSHROOM_TYPE_RED);
		red_mushroom->SetPosition(this->x, this->y - MUSHROOM_BBOX_HEIGHT - 1);
		red_mushroom->SetStartPosition(this->x, this->y - MUSHROOM_BBOX_HEIGHT - 1);
		red_mushroom->ChooseDirection();
		CAnimationSets * animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(ITEM_SET_ID);
		red_mushroom->SetAnimationSet(ani_set);

		CPlayScene *currenscence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		currenscence->PushBackObject(red_mushroom);
	}
	if (item == ITEM_MUSHROOM_GREEN)
	{
		CMushroom *green_mushroom = new CMushroom(MUSHROOM_TYPE_GREEN);
		green_mushroom->SetPosition(this->x, this->y - MUSHROOM_BBOX_HEIGHT - 1);
		green_mushroom->SetStartPosition(this->x, this->y - MUSHROOM_BBOX_HEIGHT - 1);
		green_mushroom->ChooseDirection();

		CAnimationSets * animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(ITEM_SET_ID);
		green_mushroom->SetAnimationSet(ani_set);

		CPlayScene *currenscence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		currenscence->PushBackObject(green_mushroom);
	}
	if (item == ITEM_MONEY_BUTTON)
	{
		CBrick *moneybutton = new CBrick(BRICK_TYPE_MONEYBUTTON, CANBOUNCE);
		moneybutton->SetPosition(this->x, this->y - BRICK_BBOX_HEIGHT );
		moneybutton->SetStartPosition(this->x, this->y - BRICK_BBOX_HEIGHT );

		CAnimationSets * animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(BRICK_SET_ID);
		moneybutton->SetAnimationSet(ani_set);

		CPlayScene *currenscence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		currenscence->PushBackObject(moneybutton);
	}
}

void CQuestionBrick::Jump()
{
	vy = -BRICK_QUESTION_JUMP_SPEED;
	jumptime = GetTickCount64();
}

CQuestionBrick::~CQuestionBrick()
{
}

void CQuestionBrick::Render()
{
	if(isEmpty())
		animation_set->at(BRICK_ANI_EMPTY_QUESTION)->Render(x, y);
	else
	{
		if(type == QUESTION_BRICK_TYPE_NORMAL)
			animation_set->at(BRICK_ANI_QUESTION)->Render(x, y);
		else if (type== QUESTION_BRICK_TYPE_ONSKY_BREAKABLE_ALIKE)
			animation_set->at(BRICK_ANI_BREAKABLE)->Render(x, y);
		else if(type == QUESTION_BRICK_TYPE_MONEY_BUTTON_CREATOR)
			animation_set->at(BRICK_ANI_BREAKABLE)->Render(x, y);
	}
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	
	// jump and go back to old position======
	if (GetTickCount64() - jumptime > BRICK_QUESTION_JUMP_TIME)
	{
		jumptime = 0;
		if (y < start_y)
			vy = BRICK_QUESTION_JUMP_SPEED;
		
	}
	if (y > start_y)
	{
		y = start_y;
		vy = 0;
	}
	//==========================================

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		HitByTail();
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		x += dx;
		y += dy;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CMario*>(e->obj))
			{
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CQuestionBrick::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (!enable)
	{
		l = t = r = b = 0;
		return;
	}
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}
void CQuestionBrick::HitByTail()
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
				if (type == QUESTION_BRICK_TYPE_MONEY_BUTTON_CREATOR)
				{
					if (hasItem())
						CreateItem(ITEM_MONEY_BUTTON);
				}
				else
				{
					if (hasItem() && mario->getLevel() == MARIO_LEVEL_SMALL)
						CreateItem(ITEM_MUSHROOM_RED);
					else if (hasItem() && mario->getLevel() <= MARIO_LEVEL_LEAF)
						CreateItem(ITEM_LEAF);
					if (!isEmpty())
						Jump();
				}
				getUsed();
			}
}