#include <iostream>
#include <fstream>

#include "PlayScence.h"
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
#include "Roulette.h"
#include "MovingPlatform.h"
#include "QuestionBrick.h"
#include "BoomerangBro.h"
#include "Leaf.h"
#include "Mushroom.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/



void CPlayScene::_ParseSection_TEXTURES(string line)
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

void CPlayScene::_ParseSection_SPRITES(string line)
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

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
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

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTSfromGRIDFILE(string gridfilepath)
{
	//wstring gridFilePath = ToWSTR(gridfilepath);
	ifstream gridfile;
	gridfile.open(gridfilepath);
	if (!gridfile)
	{
		DebugOut(L"Cannot open grid file to parse Objects \n");
	}
	else
	{
		char str[MAX_SCENE_LINE];
		while (gridfile.getline(str, MAX_SCENE_LINE))
		{
			string line(str);
			vector<string> tokens = split(line);
			if (tokens.size() < 5)
				continue; //skip cols and rows
			if (tokens[0] == "#")
				continue; //skip cmt
			int obj_type = atoi(tokens[0].c_str());
			float x = atof(tokens[1].c_str());
			float y = atof(tokens[2].c_str());
			int ani_set_id = atoi(tokens[3].c_str());

			// grid col row:
			int grid_col = atoi(tokens[tokens.size()- 2].c_str());
			int grid_row = atoi(tokens[tokens.size() - 1].c_str());

			CAnimationSets * animation_sets = CAnimationSets::GetInstance();

			CGameObject *obj = NULL;

			switch (obj_type)
			{
			case OBJECT_TYPE_MARIO:
				if (player != NULL)
				{
					DebugOut(L"[ERROR] MARIO object was created before!\n");
					return;
				}
				obj = new CMario(x, y);
				player = (CMario*)obj;

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
				int t = atof(tokens[5].c_str());
				obj = new CKoopas(level, t);
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
				int c = atof(tokens[7].c_str());
				obj = new CQuestionBrick(t, b, r, c);
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
			case OBJECT_TYPE_ROULETTE:
			{
				obj = new CRoulette();
			}
			break;
			case OBJECT_TYPE_MOVING_PLATFORM:
			{
				obj = new CMovingPlatform();
			}
			break;
			case OBJECT_TYPE_BOOMERANG_BRO:
			{
				obj = new CBoomerangBro();
			}
			break;
			default:
				DebugOut(L"[ERR] Invalid object type: %d\n", obj_type);
				return;
			}

			// General object setup
			obj->SetPosition(x, y);
			obj->SetStartPosition(x, y);
			

			LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

			obj->SetAnimationSet(ani_set);
			objects.push_back(obj);

			Unit* unit = new Unit(grid, x, y, obj);
			if(!dynamic_cast<CMario*>(unit->GetObj()))
			grid->Add(unit,grid_col,grid_row); //add Unit into its CELL
		}
	}
}
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	_ParseSection_OBJECTSfromGRIDFILE(line);
	//vector<string> tokens = split(line);

	////DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	//if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	//int object_type = atoi(tokens[0].c_str());
	//float x = atof(tokens[1].c_str());
	//float y = atof(tokens[2].c_str());

	//int ani_set_id = atoi(tokens[3].c_str());

	//CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	//CGameObject *obj = NULL;

	//switch (object_type)
	//{
	//case OBJECT_TYPE_MARIO:
	//	if (player != NULL)
	//	{
	//		DebugOut(L"[ERROR] MARIO object was created before!\n");
	//		return;
	//	}
	//	obj = new CMario(x, y);
	//	player = (CMario*)obj;

	//	DebugOut(L"[INFO] Player object created!\n");
	//	break;
	//case OBJECT_TYPE_GOOMBA: 
	//{
	//	int level = atof(tokens[4].c_str());
	//	obj = new CGoomba(level);
	//}
	//break;
	//case OBJECT_TYPE_BRICK: { 
	//	int t = atof(tokens[4].c_str());
	//	int b = atof(tokens[5].c_str());
	//	obj = new CBrick(t,b);
	//}
	//	break;
	//case OBJECT_TYPE_KOOPAS:
	//{
	//	int level = atof(tokens[4].c_str());
	//	int t = atof(tokens[5].c_str());
	//	obj = new CKoopas(level, t);
	//}
	//break;
	//case OBJECT_TYPE_PORTAL:
	//{
	//	float r = atof(tokens[4].c_str());
	//	float b = atof(tokens[5].c_str());
	//	int scene_id = atoi(tokens[6].c_str());
	//	obj = new CPortal(x, y, r, b, scene_id);
	//}
	//break;
	//case OBJECT_TYPE_BLOCK:
	//{
	//	float r = atof(tokens[4].c_str());
	//	float b = atof(tokens[5].c_str());
	//	obj = new CBlock(x, y, r, b);
	//}
	//	  break;
	//case OBJECT_TYPE_COIN: obj = new CCoin();
	//	break;
	//case OBJECT_TYPE_PINE: 
	//{
	//	float r = atof(tokens[4].c_str());
	//	float b = atof(tokens[5].c_str());
	//	float s= atof(tokens[6].c_str());
	//	float ty = atof(tokens[7].c_str());
	//	obj = new CPine(x, y, r, b, s, ty);
	//}
	//break;
	//case OBJECT_TYPE_BREAKABLE_BRICK:
	//{
	//	int b = atof(tokens[4].c_str());
	//	obj = new CBreakableBrick(b);
	//}
	//break;
	//case OBJECT_TYPE_QUESTION_BRICK:
	//{
	//	int t = atof(tokens[4].c_str());
	//	int b = atof(tokens[5].c_str());
	//	int r = atof(tokens[6].c_str());
	//	int c = atof(tokens[7].c_str());
	//	obj = new CQuestionBrick(t, b, r, c);
	//}
	//break;
	//case OBJECT_TYPE_LEAF:
	//{
	//	obj = new CLeaf();
	//}
	//break;
	//case OBJECT_TYPE_MUSHROOM:
	//{
	//	int t = atof(tokens[4].c_str());
	//	obj = new CMushroom(t);
	//}
	//break;
	//case OBJECT_TYPE_PLANT_PIRANHA:
	//{
	//	int t = atof(tokens[4].c_str());
	//	int l = atof(tokens[5].c_str());
	//	obj = new CPiranhaPlant(t, l);
	//}
	//break;
	//case OBJECT_TYPE_ROULETTE:
	//{
	//	obj = new CRoulette();
	//}
	//break;
	//case OBJECT_TYPE_MOVING_PLATFORM:
	//{
	//	obj = new CMovingPlatform();
	//}
	//break;
	//case OBJECT_TYPE_BOOMERANG_BRO:
	//{
	//	obj = new CBoomerangBro();
	//}
	//break;
	//default:
	//	DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
	//	return;
	//}

	//// General object setup
	//obj->SetPosition(x, y);
	//obj->SetStartPosition(x, y);

	//LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	//obj->SetAnimationSet(ani_set);
	//objects.push_back(obj);
}

void CPlayScene::_ParseSection_MAP_BACKGROUND(string line)
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

	if(tile_set == WORLD_1_1_TILESET_ID)
		mapbackground->SetTileSet(WORLD_1_1_TILESET, D3DCOLOR_XRGB(255, 255, 255));
	else if(tile_set == WORLD_1_1_SECRECT_TILESET_ID)
		mapbackground->SetTileSet(WORLD_1_1_SECRECT_TILESET, D3DCOLOR_XRGB(255, 255, 255));
	else if(tile_set == WORLDMAP_1_TILESET_ID)
		mapbackground->SetTileSet(WORLDMAP_1_TILESET, D3DCOLOR_XRGB(255, 255, 255));
	else if (tile_set == WORLD_1_4_TILESET_ID)
		mapbackground->SetTileSet(WORLD_1_4_TILESET, D3DCOLOR_XRGB(255, 255, 255));
	else if (tile_set == WORLD_1_4_SECRECT_TILESET_ID)
		mapbackground->SetTileSet(WORLD_1_4_SECRECT_TILESET, D3DCOLOR_XRGB(255, 255, 255));
	//load matrix:
	if(matrix == WORLD_1_1_MATRIX_ID)
		mapbackground->LoadMatrix(WORLD_1_1_MATRIX_TXT);
	else if (matrix == WORLD_1_1_SECRECT_MATRIX_ID)
		mapbackground->LoadMatrix(WORLD_1_1_SECRECT_MATRIX_TXT);
	else if (matrix == WORLDMAP_1_MATRIX_ID)
		mapbackground->LoadMatrix(WORLDMAP_1_MATRIX_TXT);
	else if (matrix == WORLD_1_4_MATRIX_ID)
		mapbackground->LoadMatrix(WORLD_1_4_MATRIX_TXT);
	else if (matrix == WORLD_1_4_SECRECT_MATRIX_ID)
		mapbackground->LoadMatrix(WORLD_1_4_SECRECT_MATRIX_TXT);

	//Add grid 
	_ParseSection_GRID(line);
}

// use when meet section mapbackground
void CPlayScene::_ParseSection_GRID(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines - an animation set must at least id and one animation id

	int map_columns_count = atoi(tokens[2].c_str());
	int map_rows_count = atoi(tokens[3].c_str());

	grid = new Grid(map_columns_count, map_rows_count);
}

void CPlayScene::Load()
{
	//Load background
	/*mapbackground = new MapBackground(WORLD_1_1_TILE_COLUMNS, WORLD_1_1_TILE_ROWS, TILE_WIDTH, TILE_HEIGHT, WORLD_1_1_MAP_TILESET_ROWS, WORLD_1_1_MAP_TILESET_COLUMNS);
	mapbackground->SetTileSet(WORLD_1_1_TILESET, D3DCOLOR_XRGB(255, 255, 255));
	mapbackground->LoadMatrix(WORLD_1_1_MATRIX_TXT);*/
	//
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
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
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

	hud = new Hud();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
	//fix bug:
	if (id == WORLD_1_1_SECRECT_SCENCE_ID)
		player->SetPosition(150, 80);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	objects.clear();
	listUnits.clear();

	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);

	grid->getListUnits(cam_x, cam_y, listUnits);

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < listUnits.size(); i++)
	{
		coObjects.push_back(listUnits[i]->GetObj());
		if (dynamic_cast<CKoopas*>(listUnits[i]->GetObj()))
			DebugOut(L"koopas\n");
		
	}
	CGame *game = CGame::GetInstance();
	for (size_t i = 0; i < listUnits.size(); i++)
	{
		if (!player->isTransForming())
			listUnits[i]->GetObj()->Update(dt, &coObjects);
	}
	if(player!=NULL)
		player->Update(dt, &coObjects);
	//==
	/*vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
			coObjects.push_back(objects[i]);
	}
	CGame *game = CGame::GetInstance();
	for (size_t i = 0; i < objects.size(); i++)
	{
		float l, t;
		objects[i]->GetPosition(l, t);
		if(game->isInCameraExSpaceHorizonal(l ,t) || dynamic_cast<CRoulette*>(objects[i]) || dynamic_cast<CMario*>(objects[i]))
			objects[i]->Update(dt, &coObjects);
	}*/

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return; 

	// Update camera to follow mario
	float cx = 0, cy = 0;
	player->GetPosition(cx, cy);

	if (id == WORLDMAP_1_SCENCE_ID)
	{
		CGame::GetInstance()->SetCamPos(-(SCREEN_WIDTH -WORLDMAP_1_WIDTH)/2 + 8, 0);
	}
	else if (id == WORLD_1_4_SCENCE_ID)
	{
		float cam_x = 1, cam_y = 0;
		CGame::GetInstance()->GetCamPos(cam_x, cam_y);
		cy -= game->GetScreenHeight() / 2;
		
		
		if (cy < 0)
			cy = 0;
		if (cy > mapbackground->GetMapHeight() - game->GetScreenHeight() + HUD_HEIGHT)
			cy = mapbackground->GetMapHeight() - game->GetScreenHeight() + HUD_HEIGHT;
		if(cam_x < WORLD_1_4_WIDTH - SCREEN_WIDTH + BRICK_BBOX_WIDTH)
			CGame::GetInstance()->SetCamPos(cam_x + WORLD_1_4_CAM_SPEED_X, WORLD_1_4_CAM_Y);
		else
			CGame::GetInstance()->SetCamPos(cam_x , WORLD_1_4_CAM_Y);
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
	}

	hud->Update(dt);
	//UpdateGrid();//relocate unit in grid after every CPlayScene::update
}

void CPlayScene::Render()
{
	//render background
	mapbackground->Render(mapbackground->GetTileSet());

	player->Render();
	for (int i = 0; i < listUnits.size(); i++)
	{
		if (listUnits[i]->GetObj()->isVisabled())
			if (dynamic_cast<CPine*>(listUnits[i]->GetObj()))
				continue;
			else
				listUnits[i]->GetObj()->Render();
	}
	for (int i = 0; i < listUnits.size(); i++)
	{
		if (listUnits[i]->GetObj()->isVisabled())
			if (dynamic_cast<CPine*>(listUnits[i]->GetObj()))
				listUnits[i]->GetObj()->Render();
			else
				continue;
	}

	

	//for (int i = 0; i < objects.size(); i++)
	//{
	//	if ((objects[i]->isVisabled() && objects[i]->isInCamera()) || dynamic_cast<CRoulette*>(objects[i]) || dynamic_cast<CMario*>(objects[i]))
	//	{
	//		/*if (dynamic_cast<CMario*>(objects[i]))
	//			continue;
	//		else*/
	//		objects[i]->Render();
	//	}
	//}
	//objects[0]->Render(); //mario is the last to be rendered
	hud->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}

	objects.clear();

	for (int i = 0; i < listUnits.size(); i++)
	{
		delete listUnits[i];
	}

	listUnits.clear();

	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}


void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	CGame *game = CGame::GetInstance();
	if (mario->isIcon() && !mario->isLostControl())
	{
		switch (KeyCode)
		{
		case DIK_LEFT:
			if (mario->canGoLeft())
				mario->SetState(MARIO_STATE_ICON_GO_LEFT);
			break;
		case DIK_RIGHT:
			if (mario->canGoRight())
				mario->SetState(MARIO_STATE_ICON_GO_RIGHT);
			break;
		case DIK_UP:
			if (mario->canGoUp())
				mario->SetState(MARIO_STATE_ICON_GO_UP);
			break;
		case DIK_DOWN:
			if (mario->canGoDown())
				mario->SetState(MARIO_STATE_ICON_GO_DOWN);
			break;
		case DIK_S:
			if (mario->canSelectScecne() && mario->getSelectedScene() == WORLD_1_1_SCENCE_ID)
				/*game->SwitchSceneEx(WORLD_1_1_SCENCE_ID, WORLD_1_1_START_X, WORLD_1_1_START_Y);*/
				game->SwitchScene(WORLD_1_1_SCENCE_ID);
			else if (mario->canSelectScecne() && mario->getSelectedScene() == WORLD_1_4_SCENCE_ID)
			{
				game->SwitchScene(WORLD_1_4_SCENCE_ID);
			}
			break;
		}
	}
	else if(!mario->isLostControl())
	{
		switch (KeyCode)
		{
		case DIK_Z: //small jump
			if (mario->isOnGround())
				mario->SetState(MARIO_STATE_JUMP);
			mario->setOnGround(false);
			break;
		case DIK_2: //tranform to leaf mario
			mario->SetLevel(MARIO_LEVEL_LEAF);
			mario->SetPosition(mario->x, mario->y - (MARIO_LEAF_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT)); //push mario up a bit
			break;
		case DIK_3: //tranform to fire mario
			mario->SetLevel(MARIO_LEVEL_FIRE);
			mario->SetPosition(mario->x, mario->y - (MARIO_FIRE_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT)); //push mario up a bit
			break;
		case DIK_4: //tranform to fire mario
			mario->SetLevel(MARIO_LEVEL_BIG);
			mario->SetPosition(mario->x, mario->y - (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT)); //push mario up a bit
			break;
		case DIK_1: //reset
			mario->Reset();
			break;
		case DIK_S:
			if (mario->getLevel() == MARIO_LEVEL_LEAF /*&& mario->isOnGround()*/)
			{
				mario->StartSpinning();
			}
			break;
		case DIK_A:
			if (!mario->isHolding())
			{
				if (mario->getLevel() == MARIO_LEVEL_FIRE)
				{
					mario->Shot();
				}
				else if (mario->getLevel() == MARIO_LEVEL_LEAF /*&& mario->isOnGround()*/)
				{
					mario->StartSpinning();
				}
			}
			break;
		case DIK_O: //reset
			game->SwitchSceneEx(WORLD_1_1_SECRECT_SCENCE_ID, WORLD_1_1_SECRECT_START_X, WORLD_1_1_SECRECT_START_Y);
			mario->SetState(MARIO_STATE_IDLE);
			break;
		case DIK_P: //
			game->SwitchBackScence(WORLD_1_1_SCENCE_ID, WORLD_1_1_SWITCHBACK_START_X, WORLD_1_1_SWITCHBACK_START_Y);
			mario->SetState(MARIO_STATE_IDLE);
			break;
		case DIK_I: //reset
			//mario->SetState(MARIO_STATE_ICON);
			game->SwitchSceneEx(WORLD_1_1_SCENCE_ID, WORLD_1_1_SWITCHBACK_START_X, WORLD_1_1_SWITCHBACK_START_Y);
			//game->SwitchSceneEx(WORLDMAP_1_SCENCE_ID,100,100);
			break;
		//case DIK_M: //
		//	game->SwitchSceneEx(WORLDMAP_1_SCENCE_ID, 100, 0);
		//	mario->SetState(MARIO_STATE_ICON);
		//	mario->setIsIcon(true);
		//	break;
		case DIK_V: //test gach kinh
			mario->SetPosition(2263, 0);
			break;
		case DIK_B: // chui cong
			mario->SetPosition(1950, 70);
			CGame::GetInstance()->SetCamPos(1900 - SCREEN_WIDTH / 2, 70);
			break;
		case DIK_N: // koopas
			mario->SetPosition(980, 95);
			CGame::GetInstance()->SetCamPos(980 - SCREEN_WIDTH / 2, 95);
			
			break;
		case DIK_G: //
			mario->SetPosition(180, 50);
			break;
		case DIK_M: //
			mario->EndScene();
			break;
		}
	}
	
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->isIcon())
	{
	}
	else if(!mario->isLostControl())
	{
		switch (KeyCode)
		{
		case DIK_DOWN:
			if(mario->getLevel() != MARIO_LEVEL_SMALL)
			mario->SetPosition(mario->x, mario->y - (MARIO_BIG_BBOX_HEIGHT - MARIO_BBOX_SIT_HEIGHT) - 1);
			break;//push mario up a bit after stand up
		case DIK_A:
			mario->StopRunning();
			//kick koopas shell
			mario->setHolding(false);
			break;
		case DIK_S:
			mario->setJumpable(false);
			mario->setIsFlying(false);
			break;
		}
	}
	
}


void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE || mario->isTransForming()) return;
	if (mario->isIcon())
	{

	}
	else if(!mario->isLostControl())
	{
		if (game->IsKeyDown(DIK_A))
			if (!mario->isSpeedUp())
				mario->StartRunning();

		if (game->IsKeyDown(DIK_X))
		{
			if (mario->isFalling() && mario->getLevel() == MARIO_LEVEL_LEAF)
				mario->StartFlapping();
		}
		else if (game->IsKeyDown(DIK_DOWN) /*&& mario->getLevel()!=MARIO_LEVEL_SMALL*/)
			mario->SetState(MARIO_STATE_SIT);
		
		else if (game->IsKeyDown(DIK_RIGHT))
		{
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		}
		else if (game->IsKeyDown(DIK_LEFT))
		{
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		}
		else
		{
			if (mario->isIcon())
				mario->SetState(MARIO_STATE_ICON);
			else
				mario->SetState(MARIO_STATE_IDLE);
		}

		 if (game->IsKeyDown(DIK_S))
			{
				if (mario->isJumpable())
				{
					if (mario->getStack() == MARIO_RUNNING_STACK_MAX && mario->getLevel() == MARIO_LEVEL_LEAF)
						mario->setIsFlying(true);
					mario->SetState(MARIO_STATE_JUMP);
				}
				else if (mario->isFalling() && !mario->isFlying() && mario->getLevel() == MARIO_LEVEL_LEAF)
					mario->StartFlapping();

			}
	}

	
}

void CPlayScene::LoadWorldMapBarriers(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines 

	float x = atof(tokens[0].c_str());
	float y = atof(tokens[1].c_str());
	int goleft = atoi(tokens[2].c_str());
	int goup = atoi(tokens[3].c_str());
	int goright = atoi(tokens[4].c_str());
	int godown = atoi(tokens[5].c_str());

	MapBarrier *bar = new MapBarrier(x, y, goleft, goup, goright, godown);
	barriers.push_back(bar);
}

void CPlayScene::UpdateGrid()
{
	for (unsigned int i = 0; i < listUnits.size(); i++)
	{
		Unit *unit = listUnits[i];
		float new_x, new_y;
		unit->GetObj()->GetPosition(new_x, new_y);
		grid->Move(unit, new_x, new_y);
	}
}