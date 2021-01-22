#include "Grid.h"




Grid::Grid(float map_width, float map_height)
{
	this->map_width = map_width;
	this->map_height = map_height;

	grid_columns = map_width / CELL_WIDTH;
	if ((int)map_width % CELL_WIDTH > 0)
		grid_columns += 1;

	grid_rows = map_height / CELL_HEIGHT;
	if ((int)map_height % CELL_HEIGHT > 0)
		grid_rows += 1;

	// resize grid to fit column and row
	cell.resize(grid_columns);

	for (int i = 0; i < grid_columns; i++)
		cell[i].resize(grid_columns);

	for (int i = 0; i < grid_rows; i++)
		for (int j = 0; j < grid_columns; j++)
			cell[i][j] = NULL;

}
Grid::Grid(int grid_columns, int grid_rows)
{
	this->map_width = CELL_WIDTH * grid_columns;
	this->map_height = CELL_HEIGHT * grid_rows;

	this->grid_columns = grid_columns;
	this->grid_rows = grid_rows;

	// resize grid to fit column and row
	cell.resize(grid_columns);

	for (int i = 0; i < grid_columns; i++)
		cell[i].resize(grid_columns);

	for (int i = 0; i < grid_rows; i++)
		for (int j = 0; j < grid_columns; j++)
			cell[i][j] = NULL;
}

//add Unit into its CELL
void Grid::Add(Unit* unit, int unit_column, int unit_row)
{
	unit->prev = NULL;
	unit->next = cell[unit_column][unit_row];
	cell[unit_column][unit_row] = unit;
	if (unit->next != NULL)
		unit->next->prev = unit;
}

void Grid::Add(Unit* unit)
{
	float unit_x, unit_y;
	unit->GetObj()->GetPosition(unit_x, unit_y);

	int unit_column = (int)(unit_x / CELL_WIDTH);
	int unit_row = (int)(unit_y / CELL_WIDTH);

	unit->prev = NULL;
	unit->next = cell[unit_column][unit_row];
	cell[unit_column][unit_row] = unit;
	if (unit->next != NULL)
		unit->next->prev = unit;
}




Grid::~Grid()
{
}

void Grid::Move(Unit * unit, float new_x, float new_y)
{
	float old_x = unit->x;
	float old_y = unit->y;

	//Get Old Cell:

	int old_column = (int)(old_x / CELL_WIDTH);
	int old_row = (int)(old_y / CELL_HEIGHT);

	//New Cell:

	int new_column = (int)(new_x / CELL_WIDTH);
	int new_row = (int)(new_y / CELL_HEIGHT);

	//Out of map return
	if (new_column < 0 || new_column > this->grid_columns
		|| new_row < 0 || new_row > this->grid_rows)
		return;

	//Update unit:
	unit->x = new_x;
	unit->y = new_y;
	
	//if cell changed then delete unit and add again
	if (new_column != old_column || new_row != old_row) 
	{
		if (unit->prev != NULL)
			unit->prev->next = unit->next;
		if (unit->next != NULL)
			unit->next->prev = unit->prev;
		if (cell[old_column][old_row] == unit)
			cell[old_column][old_row] == unit->next;

		this->Add(unit,new_column, new_row);
	}
}

//horizonal
void Grid::getListUnits(float cam_x, float cam_y, vector<Unit*> &listUnits)
{
	int start_column = (int)(cam_x / CELL_WIDTH);
	int end_column = (int)ceil((cam_x + SCREEN_WIDTH) / CELL_WIDTH);

	int start_row = (int)(cam_y / CELL_WIDTH);;
	int end_row = (int)ceil((cam_y + SCREEN_HEIGHT) / CELL_WIDTH);

	//int grid_last_column = (int)(map_width / CELL_WIDTH);
	
	
	for(int i=0;i<grid_columns;i++)
		for (int j = 0; j < grid_rows; j++)
		{
			Unit* unit = cell[i][j];
			if (unit != NULL) //fix bug null
			{
				if (cell[i][j]->getCol() >= start_column && cell[i][j]->getCol() <= end_column
					&& cell[i][j]->getRow() >= start_row && cell[i][j]->getRow() <= end_row)
				{
					while (unit != NULL)
					{
						listUnits.push_back(unit);
						unit = unit->next;
					}
				}
			}
			
		}

}


//=================================== UNIT =================
Unit::Unit(Grid * grid, float x, float y, LPGAMEOBJECT gameobj)
{
	this->x = x;
	this->y = y;
	this->obj = gameobj;
	this->grid = grid;
}


