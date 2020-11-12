#include "Coin.h"

void CCoin::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CCoin::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (enable == false)
	{
		l = 0;
		t = 0;
		r = 0;
		b = 0;
		return;
	}
	l = x;
	t = y;
	r = x + COIN_BBOX_WIDTH;
	b = y + COIN_BBOX_HEIGHT;
}
