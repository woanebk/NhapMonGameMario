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
	else if(type == EFFECT_TYPE_100_POINT)
		animation_set->at(ANI_100_POINT)->Render(x, y);
	else if (type == EFFECT_TYPE_200_POINT)
		animation_set->at(ANI_200_POINT)->Render(x, y);
	else if (type == EFFECT_TYPE_400_POINT)
		animation_set->at(ANI_400_POINT)->Render(x, y);
	else if (type == EFFECT_TYPE_800_POINT)
		animation_set->at(ANI_800_POINT)->Render(x, y);
	else if (type == EFFECT_TYPE_1000_POINT)
		animation_set->at(ANI_1000_POINT)->Render(x, y);
	else if (type == EFFECT_TYPE_2000_POINT)
		animation_set->at(ANI_2000_POINT)->Render(x, y);
	else if (type == EFFECT_TYPE_4000_POINT)
		animation_set->at(ANI_4000_POINT)->Render(x, y);
	else if (type == EFFECT_TYPE_8000_POINT)
		animation_set->at(ANI_8000_POINT)->Render(x, y);
	else if (type == EFFECT_TYPE_1_UP)
		animation_set->at(ANI_1_UP)->Render(x, y);
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
	else
	{
		if (GetTickCount64() - effect_start > EFFECT_POINT_TIME)
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
