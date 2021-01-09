#include "BrokenBrickPiece.h"
#include "define.h"


CBrokenBrickPiece::CBrokenBrickPiece(float nx, float ny)
{
	vx = nx * BROKEN_PIECE_SPEED_X;
	vy = ny * BROKEN_PIECE_SPEED_Y;
	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(EFFECT_SET_ID);
	SetAnimationSet(ani_set);
	flyling_start = GetTickCount64();
	flying = true;
}


CBrokenBrickPiece::~CBrokenBrickPiece()
{
}

void CBrokenBrickPiece::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = top = right = bottom = 0;
}

void CBrokenBrickPiece::Render()
{
	animation_set->at(ANI_BROKEN_PIECE)->Render(x, y);
}

void CBrokenBrickPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	if (GetTickCount64() - flyling_start > BROKEN_PIECE_FLYING_TIME && flying)
	{
		flyling_start = 0;
		flying = false;
		vy = BROKEN_PIECE_GRAVITY *dt;
	}
}

