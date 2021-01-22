#pragma once
#include "GameObject.h"
#include "define.h"


class Unit
{
	friend class Grid;
private:
	float x;
	float y;
	LPGAMEOBJECT obj;
	Grid* grid;

	Unit* next;
	Unit* prev;
public: 
	Unit(Grid* grid, float x, float y, LPGAMEOBJECT gameobj);
	LPGAMEOBJECT GetObj() { return obj; }
	int getCol() { return (int)(x / CELL_WIDTH); }
	int getRow() { return (int)(y / CELL_HEIGHT); }
};

class Grid
{
	int grid_columns;
	int grid_rows;

	float map_width;
	float map_height;

	vector <vector<Unit*>> cell;

public:
	Grid(float map_width, float map_height);
	Grid(int grid_columns, int grid_rows);
	~Grid();
	void Move(Unit* unit, float new_x, float new_y);
	void getListUnits(float cam_x, float cam_y, vector<Unit*>& listUnits);

	void Add(Unit * unit, int unit_column, int unit_row);
	void Add(Unit * unit);
};

