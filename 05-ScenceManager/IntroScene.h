#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "MapBackground.h"
#include "Hud.h"

class CIntroScene : public CScene
{
protected:

	LPMAPBACKGROUND mapbackground;	//Add a map background
	vector<LPGAMEOBJECT> objects;

	float selector_x = SELECT_1_PLAYER_X;
	float selector_y = SELECT_1_PLAYER_Y;

	float curtain_x = 0;
	float curtain_y = 0;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP_BACKGROUND(string line);
	void LoadWorldMapBarriers(string line);
public:

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	CIntroScene(int id, LPCWSTR filePath);
	~CIntroScene();

	void setSelectorPosition(float sel_x, float sel_y) { selector_x = sel_x; selector_y = sel_y; }
	bool canSelect1Player() { if (selector_x == SELECT_1_PLAYER_X && selector_y == SELECT_1_PLAYER_Y) return true; return false; }
};

class CIntroScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CIntroScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};