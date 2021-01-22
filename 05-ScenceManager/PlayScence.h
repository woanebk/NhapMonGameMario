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
#include  "MapBarrier.h"
#include "Grid.h"

class CPlayScene: public CScene
{
protected: 
	CMario *player;					// A play scene has to have player, right? 

	LPMAPBACKGROUND mapbackground;	//Add a map background

	vector<LPGAMEOBJECT> objects;
	vector<Unit*> listUnits;
	Hud* hud = NULL;
	Grid* grid = NULL;

	bool visited_base = false;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTSfromGRIDFILE(string gridfilepath);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP_BACKGROUND(string line);
	void _ParseSection_GRID(string line);
	void LoadWorldMapBarriers(string line);

	void UpdateGrid();

	vector<LPMAPBARRIER> barriers;
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void PushBackObject(CGameObject *gameobject) { objects.push_back(gameobject); }
	int getScenceID() { return id; }
	Hud* getHud() { return hud; }
	void setHud(Hud* h) { hud = h; }
	vector<LPGAMEOBJECT> GetObjectList() { return objects; }
	CMario * GetPlayer() { return player; } 
	void SetPlayer(CMario* p) { player = p; }
	void ReplaceMarioObjectWith(CMario* m) {
		//delete objects[0];
		objects[0] = m;
		} //obj [0] is always mario
	vector<LPMAPBARRIER> getBarriersList() { return barriers; }
	void setVisitedBase(bool v) { visited_base = v; }
	bool hasVisitedBase() { return visited_base; }

	Grid* getGrid() { return grid; }
	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

