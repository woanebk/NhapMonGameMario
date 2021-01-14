#include "Pine.h"
#include "define.h"

CPine::CPine(float l, float t, float r, float b, int id)
{
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
	if (scene_id == WORLD_1_1_SECRECT_SCENCE_ID)
		animation_set->at(PINE_ANI_TO_SECRECT)->Render(x, y);
	else if (scene_id == WORLD_1_1_SCENCE_ID)
		animation_set->at(PINE_ANI_SWITCH_BACK)->Render(x, y);

}

void CPine::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
