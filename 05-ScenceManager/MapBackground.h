#pragma once

#pragma once

#include <unordered_map>
#include <d3dx9.h>
#include "Sprites.h"

using namespace std;

#define WORLD_1_1_COLUMNS 176
#define WORLD_1_1_ROWS 27


class MapBackground
{
	int MapColumns; 
	int MapRows;
	int TileHeight;
	int TileWidth;
	int TileSetColumns; //number of column in a tileset image
	int TileSetRows;	//number of row in a tileset image
	int map[500][500];	//map as matrix
	LPDIRECT3DTEXTURE9 tileset;	//a texture called tileset


public:
	MapBackground::MapBackground(int mapcolumns, int maprows, int tileheight, int tilewidth, int tilesetrows, int tilesetcolumns);
	
	void LoadMatrix(LPCWSTR MatrixfilePath);
	void DrawTile(int tileID, LPDIRECT3DTEXTURE9 tileset, int i, int j);
	void Render(LPDIRECT3DTEXTURE9 tileset );
	void SetTileSet(LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 GetTileSet();
};

typedef MapBackground *LPMAPBACKGROUND;