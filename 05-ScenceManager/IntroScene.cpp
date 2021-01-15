#include "IntroScene.h"
#include <iostream>
#include <fstream>

#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Fireball.h"
#include "Pine.h"
#include "PiranhaPlant.h"
#include "Block.h"
#include "Portal.h"
#include "Coin.h"
#include "BreakableBrick.h"
#include "QuestionBrick.h"
#include "Leaf.h"
#include "Mushroom.h"

using namespace std;

CIntroScene::CIntroScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CIntroScenceKeyHandler(this);
}


CIntroScene::~CIntroScene()
{
}

void CIntroScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));

}

void CIntroScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CIntroScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CIntroScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void CIntroScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		obj = new CMario(x, y);

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA:
	{
		int level = atof(tokens[4].c_str());
		obj = new CGoomba(level);
	}
	break;
	case OBJECT_TYPE_BRICK: {
		int t = atof(tokens[4].c_str());
		int b = atof(tokens[5].c_str());
		obj = new CBrick(t, b);
	}
							break;
	case OBJECT_TYPE_KOOPAS:
	{
		int level = atof(tokens[4].c_str());
		obj = new CKoopas(level);
	}
	break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	case OBJECT_TYPE_BLOCK:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		obj = new CBlock(x, y, r, b);
	}
	break;
	case OBJECT_TYPE_COIN: obj = new CCoin();
		break;
	case OBJECT_TYPE_PINE:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		float s = atof(tokens[6].c_str());
		float ty = atof(tokens[7].c_str());
		obj = new CPine(x, y, r, b, s, ty);
	}
	break;
	case OBJECT_TYPE_BREAKABLE_BRICK:
	{
		int b = atof(tokens[4].c_str());
		obj = new CBreakableBrick(b);
	}
	break;
	case OBJECT_TYPE_QUESTION_BRICK:
	{
		int t = atof(tokens[4].c_str());
		int b = atof(tokens[5].c_str());
		int r = atof(tokens[6].c_str());
		obj = new CQuestionBrick(t, b, r);
	}
	break;
	case OBJECT_TYPE_LEAF:
	{
		obj = new CLeaf();
	}
	break;
	case OBJECT_TYPE_MUSHROOM:
	{
		int t = atof(tokens[4].c_str());
		obj = new CMushroom(t);
	}
	break;
	case OBJECT_TYPE_PLANT_PIRANHA:
	{
		int t = atof(tokens[4].c_str());
		int l = atof(tokens[5].c_str());
		obj = new CPiranhaPlant(t, l);
	}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);
	obj->SetStartPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void CIntroScene::_ParseSection_MAP_BACKGROUND(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines - an animation set must at least id and one animation id

	int tile_set = atoi(tokens[0].c_str());
	int matrix = atoi(tokens[1].c_str());
	int map_columns_count = atoi(tokens[2].c_str());
	int map_rows_count = atoi(tokens[3].c_str());
	int tile_set_rows_count = atoi(tokens[4].c_str());
	int tile_set_columns_count = atoi(tokens[5].c_str());
	//LOAD BACKGROUND:

	mapbackground = new MapBackground(map_columns_count, map_rows_count, TILE_HEIGHT, TILE_WIDTH, tile_set_rows_count, tile_set_columns_count);

	if (tile_set == WORLD_1_1_TILESET_ID)
		mapbackground->SetTileSet(WORLD_1_1_TILESET, D3DCOLOR_XRGB(255, 255, 255));
	else if (tile_set == WORLD_1_1_SECRECT_TILESET_ID)
		mapbackground->SetTileSet(WORLD_1_1_SECRECT_TILESET, D3DCOLOR_XRGB(255, 255, 255));
	else if (tile_set == WORLDMAP_1_TILESET_ID)
		mapbackground->SetTileSet(WORLDMAP_1_TILESET, D3DCOLOR_XRGB(255, 255, 255));
	else if (tile_set == INTRO_SCENE_TILESET_ID)
		mapbackground->SetTileSet(INTRO_SCENE_TILESET, D3DCOLOR_XRGB(255, 255, 255));
	//load matrix:
	if (matrix == WORLD_1_1_MATRIX_ID)
		mapbackground->LoadMatrix(WORLD_1_1_MATRIX_TXT);
	else if (matrix == WORLD_1_1_SECRECT_MATRIX_ID)
		mapbackground->LoadMatrix(WORLD_1_1_SECRECT_MATRIX_TXT);
	else if (matrix == WORLDMAP_1_MATRIX_ID)
		mapbackground->LoadMatrix(WORLDMAP_1_MATRIX_TXT);
	else if (matrix == INTRO_SCENE_MATRIX_ID)
		mapbackground->LoadMatrix(INTRO_SCENE_MATRIX_TXT);
}

void CIntroScene::LoadWorldMapBarriers(string line)
{
}

void CIntroScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[MAP_BARRIERS]") { section = SCENE_SECTION_MAP_BARRIERS; continue; }
		if (line == "[MAP_BACKGROUND]") { section = SCENE_SECTION_MAP_BACKGROUND; continue; }
		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }


		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_MAP_BARRIERS: LoadWorldMapBarriers(line); break;
		case SCENE_SECTION_MAP_BACKGROUND: _ParseSection_MAP_BACKGROUND(line); break;
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;

		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));


	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CIntroScene::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	//if (player == NULL) return;

	// Update camera to follow mario
	/*float cx, cy;
	player->GetPosition(cx, cy);
	CGame *game = CGame::GetInstance();

	if (id == WORLDMAP_1_SCENCE_ID)
	{
		CGame::GetInstance()->SetCamPos(-(SCREEN_WIDTH - WORLDMAP_1_WIDTH) / 2 + 8, 0);
	}
	else
	{
		cx -= game->GetScreenWidth() / 2;
		cy -= game->GetScreenHeight() / 2;
		if (cx < 0)
			cx = 0;
		if (cx > mapbackground->GetMapWidth() - game->GetScreenWidth())
			cx = mapbackground->GetMapWidth() - game->GetScreenWidth();
		if (cy < 0)
			cy = 0;
		if (cy > mapbackground->GetMapHeight() - game->GetScreenHeight() + HUD_HEIGHT)
			cy = mapbackground->GetMapHeight() - game->GetScreenHeight() + HUD_HEIGHT;
		CGame::GetInstance()->SetCamPos((int)cx, (int)cy);
	}*/
	CGame::GetInstance()->SetCamPos(-23, 0);
}

void CIntroScene::Render()
{
	//render background
	mapbackground->Render(mapbackground->GetTileSet());

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->isVisabled() && objects[i]->isInCamera())
		{
			objects[i]->Render();
		}
	}
	// 
	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);
	float x = cam_x;
	float y = cam_y;
	CAnimations::GetInstance()->Get(ANI_NUMBER_3_ID)->Render(ANI_NUMBER_3_X, ANI_NUMBER_3_Y, 255);
	CSprites::GetInstance()->Get(SELECTOR_SPRITE_ID)->Draw(selector_x, selector_y);
}

void CIntroScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CIntroScenceKeyHandler::KeyState(BYTE * states)
{
	CGame *game = CGame::GetInstance();
	if (game->IsKeyDown(DIK_A)) 
	{
	}
}

void CIntroScenceKeyHandler::OnKeyDown(int KeyCode)
{
	CIntroScene *scence = (CIntroScene*)CGame::GetInstance()->GetCurrentScene();
	switch (KeyCode)
	{
	case DIK_UP:
		scence->setSelectorPosition(SELECT_1_PLAYER_X, SELECT_1_PLAYER_Y);
		break;
	case DIK_DOWN:
		scence->setSelectorPosition(SELECT_2_PLAYER_X, SELECT_2_PLAYER_Y);
		break;
	case DIK_S:
		if (scence->canSelect1Player())
			CGame::GetInstance()->SwitchScene(WORLDMAP_1_SCENCE_ID);
		break;
	}
}

void CIntroScenceKeyHandler::OnKeyUp(int KeyCode)
{
	switch (KeyCode)
	{
	case DIK_DOWN:
		
		break;
	}
}
