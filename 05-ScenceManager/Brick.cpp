#include "Brick.h"
#include "define.h"

void CBrick::Render()
{
	if (type == BRICK_TYPE_WOOD)
		animation_set->at(BRICK_ANI_WOOD)->Render(x, y);
	else
		if (type == BRICK_TYPE_QUESTION)
			animation_set->at(BRICK_ANI_QUESTION)->Render(x, y);
		else
			if (type == BRICK_TYPE_GROUND_1)
				animation_set->at(BRICK_ANI_GROUND_1)->Render(x, y);
			else
				if (type == BRICK_TYPE_GROUND_2)
					animation_set->at(BRICK_ANI_GROUND_2)->Render(x, y);
				else
					if (type == BRICK_TYPE_CLOUD)
						animation_set->at(BRICK_ANI_CLOUD)->Render(x, y);
					else if (type == BRICK_TYPE_MONEYBUTTON)
						animation_set->at(BRICK_ANI_MONEY_BUTTON)->Render(x, y);
					
	RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}



