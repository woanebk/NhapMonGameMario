#include "Pine.h"
#include "define.h"
#define PINE_TO_SECRECT_TYPE	1
#define PINE_SWITCHBACK_TYPE	2
#define PINE_1ST_TYPE			3
#define PINE_2ND_TYPE			4
#define PINE_3RD_TYPE			5
CPine::CPine(float l, float t, float r, float b, int id, int ty)
{
	type = ty;
	x = l;
	y = t;
	width = r - l + 1;
	height = b - t + 1;
	if (id == 0)
		can_switch_scence = false;
	else
	{
		scene_id = id;
		can_switch_scence = true;
	}
}

void CPine::Render()
{
	if (type == PINE_TO_SECRECT_TYPE)
		animation_set->at(PINE_ANI_TO_SECRECT)->Render(x, y);
	else if (type == PINE_SWITCHBACK_TYPE)
		animation_set->at(PINE_ANI_SWITCH_BACK)->Render(x, y);
	else if (type == PINE_1ST_TYPE)
		animation_set->at(PINE_ANI_1ST_1_1)->Render(x, y);
	else if (type == PINE_2ND_TYPE)
		animation_set->at(PINE_ANI_2ND_1_1)->Render(x, y);
	else if (type == PINE_3RD_TYPE)
		animation_set->at(PINE_ANI_3RD_1_1)->Render(x, y);
	//RenderBoundingBox();
}

void CPine::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
