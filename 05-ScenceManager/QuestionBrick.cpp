#include "QuestionBrick.h"
#include "Mario.h"
#include "PlayScence.h"

bool CQuestionBrick::isEmpty()
{
	if (Coin == 0 && Reward == 0) return true;
	return false;
}

void CQuestionBrick::getUsed()
{
	Coin = 0;
	Reward = 0;
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