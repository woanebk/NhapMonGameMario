// CreateGrid.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <vector>
#include "..\05-ScenceManager\Utils.cpp"
#include "..\\05-ScenceManager\define.h"

using namespace std;


void WriteGridFile(int scene_id, int grid_columns, int grid_rows)
{
	char gridFilePath[100];

	switch (scene_id)
	{
	case WORLDMAP_1_SCENCE_ID:
		sprintf_s(gridFilePath, "..\\05-ScenceManager\\GridFile\\worldmap1-grid.txt");
		break;
	case WORLD_1_1_SCENCE_ID:
		sprintf_s(gridFilePath, "..\\05-ScenceManager\\GridFile\\world-1-1-grid.txt");
		break;
	case WORLD_1_1_SECRECT_SCENCE_ID:
		sprintf_s(gridFilePath, "..\\05-ScenceManager\\GridFile\\world-1-1-secrect-grid.txt");
		break;
	case WORLD_1_4_SCENCE_ID:
		sprintf_s(gridFilePath, "..\\05-ScenceManager\\GridFile\\world-1-4-grid.txt");
		break;
	case WORLD_1_4_SECRECT_SCENCE_ID:
		sprintf_s(gridFilePath, "..\\05-ScenceManager\\GridFile\\world-1-4-secrect-grid.txt");
		break;
	default:
		break;
	}
	//create grid.txt
	ofstream gridFile(gridFilePath);
	gridFile << grid_columns << "\t" << grid_rows << "\n";

	//==================================================== read object from old txt file




	char OBJFilePath[100];
	// choose file from scene id
	switch (scene_id)
	{
	case WORLDMAP_1_SCENCE_ID:
		sprintf_s(OBJFilePath, "..\\05-ScenceManager\\SceneTxt\\scence_worldmap_1.txt");
		break;
	case WORLD_1_1_SCENCE_ID:
		sprintf_s(OBJFilePath, "..\\05-ScenceManager\\SceneTxt\\scence-world-1-1.txt");
		break;
	case WORLD_1_1_SECRECT_SCENCE_ID:
		sprintf_s(OBJFilePath, "..\\05-ScenceManager\\SceneTxt\\scence-world-1-1-secrect.txt");
		break;
	case WORLD_1_4_SCENCE_ID:
		sprintf_s(OBJFilePath, "..\\05-ScenceManager\\SceneTxt\\scence-world-1-4.txt");
		break;
	case WORLD_1_4_SECRECT_SCENCE_ID:
		sprintf_s(OBJFilePath, "..\\05-ScenceManager\\SceneTxt\\scence-world-1-4-secrect.txt");
		break;
	default:
		break;
	}

	ifstream objFile(OBJFilePath);
	if (!objFile)
	{
		cout << "Cannot open file \n";
	}
	else
	{
		cout << "File opened susscesful \n";

		int section = SCENE_SECTION_UNKNOWN;

		char str[MAX_SCENE_LINE];
		while (objFile.getline(str, MAX_SCENE_LINE))
		{
			string line(str);

			if (line[0] == '#') continue;	// skip comment lines	

			if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; }
			if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }


			//
			//
			//
			switch (section)
			{
			case SCENE_SECTION_OBJECTS:
				vector<string> tokens = split(line);

				if (tokens.size() < 3 ) continue; // skip invalid lines - 
				if (tokens[0].c_str() == "") continue; // skip invalid lines - 

				int object_type = atoi(tokens[0].c_str());
				float x = atoi(tokens[1].c_str());
				float y = atoi(tokens[2].c_str());

				gridFile << object_type ;
				for (int i = 1; i < tokens.size(); i++)
				{
					float x = atof(tokens[i].c_str());
					gridFile << "\t" << x;
					if (i == tokens.size() - 1)
						gridFile << "\t";
				}

				int row = (int)(y / CELL_HEIGHT);
				int col = (int)(x / CELL_WIDTH);
				gridFile <<col << "\t" << row << "\n";
				break;
			}
		}
	}
	objFile.close();
	gridFile.close();
}

//Read file scene .txt and return map columns, rows (== -1 if fail to open)
void ReadSceneInformation(int scene_id, int &mapColumns, int &mapRows)
{
	//flag
	mapColumns = -1;
	mapRows = -1;
	//
	char FilePath[100];
	// choose file from scene id
	switch (scene_id)
	{
	case WORLDMAP_1_SCENCE_ID:
		sprintf_s(FilePath, "..\\05-ScenceManager\\scence_worldmap_1.txt");
		break;
	case WORLD_1_1_SCENCE_ID:
		sprintf_s(FilePath, "..\\05-ScenceManager\\scence-world-1-1.txt");
		break;
	case WORLD_1_1_SECRECT_SCENCE_ID:
		sprintf_s(FilePath, "..\\05-ScenceManager\\scence-world-1-1-secrect.txt");
		break;
	case WORLD_1_4_SCENCE_ID:
		sprintf_s(FilePath, "..\\05-ScenceManager\\scence-world-1-4.txt");
		break;
	case WORLD_1_4_SECRECT_SCENCE_ID:
		sprintf_s(FilePath, "..\\05-ScenceManager\\scence-world-1-4-secrect.txt");
		break;
	default:
		break;
	}

	ifstream mapFile(FilePath);
	if (!mapFile)
	{
		cout << "Cannot open file \n";
	}
	else
	{
		cout << "File opened susscesful \n";

		int section = SCENE_SECTION_UNKNOWN;

		char str[MAX_SCENE_LINE];
		while (mapFile.getline(str, MAX_SCENE_LINE))
		{
			string line(str);

			if (line[0] == '#') continue;	// skip comment lines	

			if (line == "[MAP_BACKGROUND]") { section = SCENE_SECTION_MAP_BACKGROUND; continue; }
			if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }


			//
			//
			//
			switch (section)
			{
			case SCENE_SECTION_MAP_BACKGROUND:
				vector<string> tokens = split(line);

				if (tokens.size() < 5) continue; // skip invalid lines - 

				int map_columns_count = atoi(tokens[2].c_str());
				int map_rows_count = atoi(tokens[3].c_str());

				mapColumns = map_columns_count;
				mapRows = map_rows_count;

				cout << mapColumns << "\t" << mapRows << "\n";
				break;
			}
		}
	}
	mapFile.close();
}


void CreateGridFile(int scene_id)
{
	int grid_coluns_count, grid_rows_count;
	ReadSceneInformation(scene_id, grid_coluns_count, grid_rows_count);

	if (grid_coluns_count == -1 && grid_rows_count == -1)
		return;
	WriteGridFile(scene_id, grid_coluns_count, grid_rows_count);
}

int main()
{
	int scene_id;
	std::cout << "Please Enter scene id to create Grid File!\n 1.WORLD-1-1 \n 2.WORLD-1-1-SECRECT \n 3.WORLDMAP-1 \n 4.WORLD-1-4 \n 5.WORLD-1-4-SECRECT\n";
	cin >> scene_id;
	CreateGridFile(scene_id);
	system("Pause");
	return 0;
}