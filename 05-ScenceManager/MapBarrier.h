#pragma once
#include "GameObject.h"
class MapBarrier
{
	float x;
	float y;
	bool go_left;
	bool go_right;
	bool go_up;
	bool go_down;
public:
	MapBarrier(float barrier_x, float barrier_y, int goleft, int goup, int goright, int godown);
	~MapBarrier();

	void getPosition(float &p_x, float &p_y) {
		p_x = x;
		p_y = y;
	}
	void getCanMove(bool &l, bool &u, bool &r, bool &d)
	{
		l = go_left;
		u = go_up;
		r = go_right;
		d = go_down;
	}
};

typedef MapBarrier *LPMAPBARRIER;

