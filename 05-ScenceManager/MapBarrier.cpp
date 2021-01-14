#include "MapBarrier.h"



MapBarrier::MapBarrier(float barrier_x, float barrier_y, int goleft = 0, int goup = 0, int goright = 0, int godown = 0)
{
	x = barrier_x;
	y = barrier_y;
	if (goleft == 1)
		go_left = true;
	else
		go_left = false;

	if (goup == 1)
		go_up = true;
	else
		go_up = false;

	if (goright == 1)
		go_right = true;
	else
		go_right = false;

	if (godown == 1)
		go_down = true;
	else
		go_down = false;
}


MapBarrier::~MapBarrier()
{
}
