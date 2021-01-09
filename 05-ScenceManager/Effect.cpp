#include "Effect.h"



CEffect::CEffect(int t)
{
	type = t;
	Start_Render_Effect();
}

void CEffect::Render()
{
	if (type == EFFECT_TYPE_TAIL_HIT)
		animation_set->at(ANI_HIT_TAIL)->Render(x, y);
}

void CEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!enable)
		return;
	if (type == EFFECT_TYPE_TAIL_HIT)
	{
		if (GetTickCount64() - effect_start > EFFECT_HIT_TAIL_TIME)
		{
			enable = false;
			visable = false;
			effect_start = 0;
		}
	}
}

void CEffect::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = t = r = b = 0;
}


CEffect::~CEffect()
{

}
