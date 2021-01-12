#include "Hud.h"
#include "Game.h"
#include "define.h"
#include "PlayScence.h"
#include "Mario.h"
#include <string>
#include <iostream>
#include <fstream>

#define	HUD_SPRITE_ID	14000
#define A_SPRITE_ID		14100
#define B_SPRITE_ID		14101
#define C_SPRITE_ID		14102
#define D_SPRITE_ID		14103
#define E_SPRITE_ID		14104
#define F_SPRITE_ID		14105
#define G_SPRITE_ID		14106
#define H_SPRITE_ID		14107
#define I_SPRITE_ID		14108
#define J_SPRITE_ID		14109
#define K_SPRITE_ID		14110
#define L_SPRITE_ID		14111
#define M_SPRITE_ID		14112
#define N_SPRITE_ID		14113
#define O_SPRITE_ID		14114
#define P_SPRITE_ID		14115
#define Q_SPRITE_ID		14116
#define R_SPRITE_ID		14117
#define S_SPRITE_ID		14118
#define T_SPRITE_ID		14119
#define U_SPRITE_ID		14120
#define V_SPRITE_ID		14121
#define W_SPRITE_ID		14122
#define X_SPRITE_ID		14123
#define Y_SPRITE_ID		14124
#define Z_SPRITE_ID		14125

#define NUMBER_0_SPRITE_ID	14200
#define NUMBER_1_SPRITE_ID	14201
#define NUMBER_2_SPRITE_ID	14202
#define NUMBER_3_SPRITE_ID	14203
#define NUMBER_4_SPRITE_ID	14204
#define NUMBER_5_SPRITE_ID	14205
#define NUMBER_6_SPRITE_ID	14206
#define NUMBER_7_SPRITE_ID	14207
#define NUMBER_8_SPRITE_ID	14208
#define NUMBER_9_SPRITE_ID	14209

#define STACK_ARROW_SPRITE_ID	14220
#define P_MAX_STACK_SPRITE		14221

using namespace std;

Hud::Hud()
{

	LPSPRITE arrow = (CSprite*)CSprites::GetInstance()->Get(STACK_ARROW_SPRITE_ID);
	LPSPRITE Pmaxstack = (CSprite*)CSprites::GetInstance()->Get(P_MAX_STACK_SPRITE);
	countdowntime = GetTickCount64();
	//add sprite for stack
	for (int i = 0; i < 6; i++)
		stackSprite.push_back(arrow);
	stackSprite.push_back(Pmaxstack);
}

void Hud::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void Hud::Update(DWORD dt)
{
	CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = scence->GetPlayer();

	if (GetTickCount64() - countdowntime > 1000 && RemainingTime > 0)
	{
		RemainingTime -= 1;
		countdowntime = GetTickCount64();
	}
	if (mario != NULL)
	{
		life = mario->getLife();
		score = mario->getScore();
		money = mario->getMoney();
		stack = mario->getStack();
	}

	string scores_str = to_string(score);
	string preset = "0";
	while (scores_str.length() < 7) scores_str = preset + scores_str; //string be like : 0000xxx;
	vector<LPSPRITE> s;
	for (int i = 0; i < 7; i++)
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
		stackSprite[0]->Draw(hud_x + 87, hud_y + 13);
		for (int i = 1; i < stack; i++)
		{
			stackSprite[i]->Draw(hud_x + 87 + i * 8, hud_y + 13 );
		}
	}

	// ======== render Life Count:
	//hang don vi:
	int life_units = life % 10;
	//hang chuc:
	int life_tens = life / 10;

	if (life_units >= 0)
		getNumberSprite(life_units)->Draw(hud_x + 73, hud_y + 21);
	if (life_tens > 0)
		getNumberSprite(life_tens)->Draw(hud_x + 65, hud_y + 21);

	// ====== render Money:
	//hang donvi:
	int money_units = money % 10;
	//hang chuc:
	int money_tens = money / 10;

	if (money_units >= 0)
		getNumberSprite(money_units)->Draw(hud_x + 176, hud_y + 13);
	if (money_tens > 0)
		getNumberSprite(money_tens)->Draw(hud_x + 167, hud_y + 13);

	// ====== render Remaining Time
	//hang don vi:
	int remaining_time_units = RemainingTime % 10;
	//hang chuc:
	int remaining_time_tens = (RemainingTime / 10) % 10;
	//hang tram:
	int remaining_time_hundreds = RemainingTime / 100;

	if (remaining_time_units >= 0)
	{
		getNumberSprite(remaining_time_units)->Draw(hud_x + 176, hud_y + 21);
		
	}
	if (remaining_time_hundreds > 0)
		getNumberSprite(remaining_time_hundreds)->Draw(hud_x + 160, hud_y + 21);

	if (remaining_time_hundreds != 0 && remaining_time_tens >= 0)
	{
		getNumberSprite(remaining_time_tens)->Draw(hud_x + 168, hud_y + 21);
	}
	else if (remaining_time_tens > 0)
		getNumberSprite(remaining_time_tens)->Draw(hud_x + 168, hud_y + 21);

	// ====== Render score:
	for (int i = 0; i < 7; i++)
			scoreSprite[i]->Draw(hud_x + 88 + i * 8, hud_y + 21);
	
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

