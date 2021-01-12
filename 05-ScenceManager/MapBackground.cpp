#include "MapBackground.h"
#include <fstream>
#include <string>
#include "Utils.h"
#include "Game.h"
#include "define.h"

MapBackground::MapBackground(int mapcolumns, int maprows, float tileheight, float tilewidth, int tilesetrows, int tilesetcolumns)
{
		MapColumns = mapcolumns;
		MapRows = maprows;
		TileHeight = tileheight;
		TileWidth = tilewidth;
		TileSetColumns = tilesetcolumns;
		TileSetRows = tilesetrows;
}

//load map as matrix from a txt file
void MapBackground::LoadMatrix(LPCWSTR MatrixfilePath)
{
	ifstream f;
	int temp;
	f.open(MatrixfilePath, ifstream::in);
	if (f.fail() )
	{
		DebugOut(L"Fail to load matrix");
		return;
	}
	for (int i = 0; i < MapRows; i++)
	{
		for (int j = 0; j < MapColumns; j++)
		{
			f >> temp;
			map[i][j] = temp;
		}
	}
	//test show matrix:
	for (int i = 0; i < MapRows; i++)
	{
		DebugOut(L"\n");
		for (int j = 0; j < MapColumns; j++)
		{
			DebugOut(L"%d	", map[i][j]);
		}
	}
	//Load matrix suscessful
}

//draw a tile knowing its id and position in matrix
void MapBackground::DrawTile(int tileID, LPDIRECT3DTEXTURE9 tileset, int i, int j)
{
	CGame * game = CGame::GetInstance();
	game->Draw(j*TileWidth, i*TileHeight, tileset, ((tileID-1)%TileSetColumns)*TileWidth, ((tileID-1)/TileSetColumns)*TileHeight, ((tileID-1) % TileSetColumns)*TileWidth + TileWidth, ((tileID-1) /TileSetColumns)*TileHeight + TileHeight);
}

//draw every tile
void MapBackground::Render(LPDIRECT3DTEXTURE9 tileset)
{
	CGame *game = CGame::GetInstance();
	float camX, camY;
	game->GetCamPos(camX, camY);
	int rendered_start_column = (int)camX / TILE_WIDTH ;
	int rendered_end_column = rendered_start_column + (int)SCREEN_WIDTH / TILE_WIDTH;
	int rendered_start_row = (int)camY / TILE_HEIGHT ;
	int rendered_end_row = rendered_start_row + (int)SCREEN_HEIGHT / TILE_HEIGHT ;
	for (int i = 0; i < MapRows; i++)
	{
		for (int j = 0; j < MapColumns; j++)
		{
			if(rendered_start_column <= j && j <= rendered_end_column && rendered_start_row <= i && i <= rendered_end_row)
			DrawTile(map[i][j], tileset, i, j);
		}
	}
	

}

//load tileset (a texture) from file
void MapBackground::SetTileSet(LPCWSTR filePath, D3DCOLOR transparentColor)
{
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filePath, &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", filePath);
		return;
	}

	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();
	LPDIRECT3DTEXTURE9 tileSet;

	result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		filePath,							// Path to the image to load
		info.Width,							// Texture(tileset) width
		info.Height,						// Texture(tileset) height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transparentColor,
		&info,
		NULL,
		&tileSet);								// Created texture(tileset) pointer

	if (result != D3D_OK)
	{
		OutputDebugString(L"[ERROR] LoadTileSet failed\n");
		return;
	}
	tileset = tileSet;
	DebugOut(L"[INFO] Texture(TileSet) loaded Ok: %s\n", filePath);
}

float MapBackground::GetMapHeight()
{
	return MapRows * TileHeight;
}

float MapBackground::GetMapWidth()
{
	return MapColumns * TileWidth;
}

LPDIRECT3DTEXTURE9 MapBackground::GetTileSet()
{
	return tileset;
}




