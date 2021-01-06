#include "QuestionBrick.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Leaf.h"

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
		CAnimationSets * animation_sets = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = animation_sets->Get(ITEM_SET_ID);
		leaf->SetAnimationSet(ani_set);

		
		leaf->SetPosition(this->x, this->y);
		leaf->SetStartPosition(this->x, this->y);

		CPlayScene *currenscence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		currenscence->PushBackObject(leaf);
	}
}

CQuestionBrick::~CQuestionBrick()
{
}

void CQuestionBrick::Render()
{
	if(isEmpty())
		animation_set->at(BRICK_ANI_EMPTY_QUESTION)->Render(x, y);
	else
		animation_set->at(BRICK_ANI_QUESTION)->Render(x, y);
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

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
		if ((bb_left <= mario_bb_right && bb_right >= mario_bb_left) || (bb_right >= mario_bb_left && bb_left <= mario_bb_right))
			if ((bb_top <= mario_bb_bottom && bb_bottom >= mario_bb_top) || (bb_bottom >= mario_bb_top && bb_top <= mario_bb_bottom))
			{
				getUsed();
			}
}