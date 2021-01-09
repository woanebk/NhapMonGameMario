#include "BreakableBrick.h"
#include "PlayScence.h"
#include "Mario.h"
#include "BrokenBrickPiece.h"


CBreakableBrick::CBreakableBrick(int b)
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;
	bounce = b;
}

void CBreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
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

void CBreakableBrick::Render()
{
	animation_set->at(BRICK_ANI_BREAKABLE)->Render(x, y);
}

void CBreakableBrick::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
	if (!enable)
		l = t = r = b = 0;
}

void CBreakableBrick::HitByTail()
{
	float bb_left, bb_top, bb_right, bb_bottom;
	float mario_bb_left, mario_bb_top, mario_bb_right, mario_bb_bottom;

	GetBoundingBox(bb_left, bb_top, bb_right, bb_bottom);
	CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = scence->GetPlayer();
	mario->GetBoundingBox(mario_bb_left, mario_bb_top, mario_bb_right, mario_bb_bottom);
	if (mario->isSpinning())
		if (bb_left <= mario_bb_right + MARIO_LEAF_BBOX_TAIL_WIDTH && bb_right >= mario_bb_left - MARIO_LEAF_BBOX_TAIL_WIDTH) 
			if (bb_top <= mario_bb_bottom && bb_bottom >= mario_bb_top + (mario_bb_bottom - mario_bb_top)/2)
			{
				enable = false;
				visable = false;
				Broken = true;
				//broken pieces:
				CBrokenBrickPiece *piece_top_left = new CBrokenBrickPiece(-1, -1);
				piece_top_left->SetPosition(this->x + BRICK_BBOX_WIDTH/2 , this->y + BRICK_BBOX_HEIGHT/2);
				piece_top_left->SetStartPosition(this->x + BRICK_BBOX_WIDTH / 2, this->y + BRICK_BBOX_HEIGHT / 2);
				scence->PushBackObject(piece_top_left);

				CBrokenBrickPiece *piece_top_right = new CBrokenBrickPiece(1, -1);
				piece_top_right->SetPosition(this->x + BRICK_BBOX_WIDTH / 2, this->y + BRICK_BBOX_HEIGHT / 2);
				piece_top_right->SetStartPosition(this->x + BRICK_BBOX_WIDTH / 2, this->y + BRICK_BBOX_HEIGHT / 2);
				scence->PushBackObject(piece_top_right);

				CBrokenBrickPiece *piece_bottom_left = new CBrokenBrickPiece(-1, 1);
				piece_bottom_left->SetPosition(this->x + BRICK_BBOX_WIDTH / 2, this->y + BRICK_BBOX_HEIGHT / 2);
				piece_bottom_left->SetStartPosition(this->x + BRICK_BBOX_WIDTH / 2, this->y + BRICK_BBOX_HEIGHT / 2);
				scence->PushBackObject(piece_bottom_left);

				CBrokenBrickPiece *piece_bottom_right = new CBrokenBrickPiece(1, 1);
				piece_bottom_right->SetPosition(this->x + BRICK_BBOX_WIDTH / 2, this->y + BRICK_BBOX_HEIGHT / 2);
				piece_bottom_right->SetStartPosition(this->x + BRICK_BBOX_WIDTH / 2, this->y + BRICK_BBOX_HEIGHT / 2);
				scence->PushBackObject(piece_bottom_right);

			}
}

CBreakableBrick::~CBreakableBrick()
{
}
void CBreakableBrick::Break()
{
	visable = false;
	enable = false;
	Broken = true;
	//need to add break animation
}


