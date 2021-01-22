#include "Hud.h"
#include "Game.h"
#include "define.h"
#include "PlayScence.h"
#include "Mario.h"
#include <string>
#include <iostream>
#include <fstream>



using namespace std;

Hud::Hud()
{

	LPSPRITE arrow = (CSprite*)CSprites::GetInstance()->Get(STACK_ARROW_SPRITE_ID);
	LPSPRITE Pmaxstack = (CSprite*)CSprites::GetInstance()->Get(P_MAX_STACK_SPRITE);
	countdowntime = GetTickCount64();
	//add sprite for stack
	for (int i = 0; i < MARIO_RUNNING_STACK_MAX - 1; i++)
		stackSprite.push_back(arrow);
	stackSprite.push_back(Pmaxstack);
}

void Hud::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void Hud::Update(DWORD dt)
{
	CGame* game = CGame::GetInstance();
	CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = scence->GetPlayer();
	
	if (!no_timing && GetTickCount64() - countdowntime > 1000)
	{
		if (RemainingTime > 0)
		{
			RemainingTime -= 1;
			countdowntime = GetTickCount64();
		}
		else
			EndScene();
	}


	life = game->getLife();
	score = game->getScore();
	money = game->getMoney();
	if (mario != NULL)
		stack = mario->getStack();

	// change score from int to sprite
	string scores_str = to_string(score);
	string preset = "0";
	while (scores_str.length() < MARIO_SCORE_NUMBERS) scores_str = preset + scores_str; //string be like : 0000xxx;
	vector<LPSPRITE> s;
	for (int i = 0; i < MARIO_SCORE_NUMBERS; i++)
	{
		string score_th(1, scores_str.at(i));
		int i_scoresprite = stoi(score_th);
		s.push_back(getNumberSprite(i_scoresprite));
	}
	scoreSprite = s;
}

void Hud::Render()
{
	float cam_x, cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);
	float hud_x = cam_x;
	float hud_y = cam_y + SCREEN_HEIGHT - HUD_HEIGHT * 2 + 1;
	//render HUD
	CSprites::GetInstance()->Get(HUD_SPRITE_ID)->Draw(cam_x, cam_y + SCREEN_HEIGHT - HUD_HEIGHT * 2 + 1);

	// ======== render Speed Stack:
	if (stack > 0)
	{
		stackSprite[0]->Draw(hud_x + STACK_SPRITE_0_X, hud_y + STACK_SPRITE_0_Y);
		for (int i = 1; i < stack; i++)
		{
			stackSprite[i]->Draw(hud_x + STACK_SPRITE_0_X + i * STACK_SPRITE_WIDTH, hud_y + STACK_SPRITE_0_Y);
		}
	}

	// ======== render Life Count:
	//hang don vi:
	int life_units = life % 10;
	//hang chuc:
	int life_tens = life / 10;

	if (life_units >= 0)
		getNumberSprite(life_units)->Draw(hud_x + LIFE_SPRITE_UNITS_NUMBER_X, hud_y + LIFE_SPRITE_UNITS_NUMBER_Y);
	if (life_tens > 0)
		getNumberSprite(life_tens)->Draw(hud_x + LIFE_SPRITE_TENS_NUMBER_X, hud_y + LIFE_SPRITE_TENS_NUMBER_Y);

	// ====== render Money:
	//hang donvi:
	int money_units = money % 10;
	//hang chuc:
	int money_tens = money / 10;

	if (money_units >= 0)
		getNumberSprite(money_units)->Draw(hud_x + MONEY_SPRITE_UNITS_NUMBER_X, hud_y + MONEY_SPRITE_UNITS_NUMBER_Y);
	if (money_tens > 0)
		getNumberSprite(money_tens)->Draw(hud_x + MONEY_SPRITE_TENS_NUMBER_X, hud_y + MONEY_SPRITE_TENS_NUMBER_Y);

	// ====== render Remaining Time
	//hang don vi:
	int remaining_time_units = RemainingTime % 10;
	//hang chuc:
	int remaining_time_tens = (RemainingTime / 10) % 10;
	//hang tram:
	int remaining_time_hundreds = RemainingTime / 100;

	if (remaining_time_units >= 0)
	{
		getNumberSprite(remaining_time_units)->Draw(hud_x + TIME_SPRITE_UNITS_NUMBER_X, hud_y + TIME_SPRITE_UNITS_NUMBER_Y);
		
	}
	if (remaining_time_hundreds > 0)
		getNumberSprite(remaining_time_hundreds)->Draw(hud_x + TIME_SPRITE_HUNDREDS_NUMBER_X, hud_y + TIME_SPRITE_HUNDREDS_NUMBER_Y);

	if (remaining_time_hundreds != 0 && remaining_time_tens >= 0)
	{
		getNumberSprite(remaining_time_tens)->Draw(hud_x + TIME_SPRITE_TENS_NUMBER_X, hud_y + TIME_SPRITE_TENS_NUMBER_Y);
	}
	else if (remaining_time_tens > 0)
		getNumberSprite(remaining_time_tens)->Draw(hud_x + TIME_SPRITE_TENS_NUMBER_X, hud_y + TIME_SPRITE_TENS_NUMBER_Y);

	// ====== Render score:
	for (int i = 0; i < 7; i++)
		if(scoreSprite.size() != 0)
			scoreSprite[i]->Draw(hud_x + SCORE_SPRITE_0_X + i * SCORE_SPRITE_WIDTH, hud_y + SCORE_SPRITE_0_Y);
	
}


Hud::~Hud()
{
}

LPSPRITE Hud::getNumberSprite(int num)
{
	int spriteid = -1;
	switch (num)
	{
	case 0 :
		spriteid = NUMBER_0_SPRITE_ID;
		break;
	case 1:
		spriteid = NUMBER_1_SPRITE_ID;
		break;
	case 2:
		spriteid = NUMBER_2_SPRITE_ID;
		break;
	case 3:
		spriteid = NUMBER_3_SPRITE_ID;
		break;
	case 4:
		spriteid = NUMBER_4_SPRITE_ID;
		break;
	case 5:
		spriteid = NUMBER_5_SPRITE_ID;
		break;
	case 6:
		spriteid = NUMBER_6_SPRITE_ID;
		break;
	case 7:
		spriteid = NUMBER_7_SPRITE_ID;
		break;
	case 8:
		spriteid = NUMBER_8_SPRITE_ID;
		break;
	case 9:
		spriteid = NUMBER_9_SPRITE_ID;
		break;

	default:
		spriteid = NUMBER_0_SPRITE_ID;
		break;
	}
	LPSPRITE numspirte = (CSprite*)CSprites::GetInstance()->Get(spriteid);
	return numspirte;
}

void Hud::EndScene()
{
	CGame *game = CGame::GetInstance();
	CPlayScene *scene = (CPlayScene*)game->GetCurrentScene();
	CMario *mario = scene->GetPlayer();
	mario->setLostControl(true);
	RemainingTime = GAME_LIMIT_TIME;
	game->SwitchScene(WORLDMAP_1_SCENCE_ID);
}

