#include "Brick.h"
#include "define.h"

void CBrick::Render()
{
	if(type == BRICK_TYPE_WOOD)
	animation_set->at(0)->Render(x, y);
	else
		if(type == BRICK_TYPE_QUESTION)
			animation_set->at(1)->Render(x, y);
		else
			if (type == BRICK_TYPE_GROUND_1)
				animation_set->at(2)->Render(x, y);
			else
				if (type == BRICK_TYPE_GROUND_2)
					animation_set->at(3)->Render(x, y);
				else
					if (type == BRICK_TYPE_CLOUD)
						animation_set->at(4)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}