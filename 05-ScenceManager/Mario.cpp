#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"
#include "PlayScence.h"

#include "Fireball.h"
#include "Pine.h"
#include "Goomba.h"
#include "Portal.h"
#include "Brick.h"
#include "Koopas.h"
#include "Coin.h"
#include "Block.h"
#include "BreakableBrick.h"
#include "QuestionBrick.h"
#include "Leaf.h"
#include "Mushroom.h"
#include "Effect.h"

CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	if (scence->getScenceID() == WORLDMAP_1_SCENCE_ID)
	{
		SetState(MARIO_STATE_ICON);
		is_icon = true;
	}
	else
		SetState(MARIO_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
	ax = 0;
	ay = MARIO_GRAVITY;
	Stack = 0;
	Life = MARIO_START_LIFE;
	Money = 0;
	Score = 0;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	if (is_icon) // ==================== logic for ICON MARIO ON WORLD MAP
	{
		// stop when reach certain pint on map
		if (is_going_left)
		{
			if (abs(icon_old_x - x) >= MARIO_ICON_SPEED_DX)
			{
				SetState(MARIO_STATE_ICON);
				x = icon_new_x;
				is_lost_control = false;
				is_going_left = false;
			}
		}
		else if (is_going_right)
		{
			if (abs(x-icon_old_x )>= MARIO_ICON_SPEED_DX)
			{
				SetState(MARIO_STATE_ICON);
				x = icon_new_x;
				vx = 0;
				is_lost_control = false;
				is_going_right = false;
			}
		}
		else if (is_going_up)
		{
			if (abs(icon_old_y - y )>= MARIO_ICON_SPEED_DX)
			{
				SetState(MARIO_STATE_ICON);
				y = icon_new_y ;
				vy = 0;
				is_lost_control = false;
				is_going_up = false;
			}
		}
		else if (is_going_down)
		{
			if (abs(y - icon_old_y) >= MARIO_ICON_SPEED_DX)
			{
				SetState(MARIO_STATE_ICON);
				y = icon_new_y;
				vy = 0;
				is_lost_control = false;
				is_going_down = false;
			}
		}
		
		CPlayScene *scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		vector<LPMAPBARRIER> barriers_list = scence->getBarriersList();
		for (int i = 0; i < barriers_list.size(); i++)
		{
			float bar_x, bar_y;
			barriers_list[i]->getPosition(bar_x, bar_y);
			DebugOut(L"barrier: %d, %d \n", bar_x, bar_y);
			if (isEqual(bar_x, x) && isEqual(bar_y , y))
			{
				if (bar_x == WORLD_1_1_ONMAP_X && bar_y == WORLD_1_1_ONMAP_Y)
					can_select_scence = true;
				else
					can_select_scence = false;
				barriers_list[i]->getCanMove(can_go_left, can_go_up, can_go_right, can_go_down);
				
			}
		}

		x += dx;
		y += dy;
		DebugOut(L"%d, %d, %d, %d \t %d, %d \n", can_go_left, can_go_up, can_go_right, can_go_down, x, y);
		return;
	}// ====================================================================

	// Simple fall down
	vy += ay * dt;
	ay = MARIO_GRAVITY;
	//acceleration
	vx += ax * dt + Stack * ax;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	if (flapping)
		vy = MARIO_LEAF_FLAPPING_SPEED; //if mario is flapping then falling down slowly

	//calculate action flag:

	if (vy < 0 && isonground == false) { //IS JUMPING
		isjumping = true;
		/* DebugOut(L"Jumping \n");*/
	}
	else isjumping = false;

	if (vy > 0 && isonground == false) {
		isfalling = true; 
	/*DebugOut(L"Falling \n"); */ } //IS FALLING
	else isfalling = false;

	if (isonground)
		jumpable = true; //1 way

	ManageAccelerationAndSpeed();
	TimingEvent();

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		if (x < 0)
			x = 0;
		if (x > WORLD_1_1_WIDTH)
			x = WORLD_1_1_WIDTH; //relocate mario inside map
		isonground = false;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		if (ny < 0) { isonground = true; /*DebugOut(L"On_Groud \n");*/ }

		//get MARIO bounding box to use later
		float mario_bb_left, mario_bb_top, mario_bb_right, mario_bb_bottom;
		GetBoundingBox(mario_bb_left, mario_bb_top, mario_bb_right, mario_bb_bottom);
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CGoomba *>(e->obj) && e->obj->isEnabled()) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						if (goomba->getLevel() == GOOMBA_LEVEL_FLY)
						{
							goomba->LevelDown();
							GainScore(SCORE_400);
							RenderPoint(EFFECT_TYPE_100_POINT);
							vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
						else
						{
							goomba->SetState(GOOMBA_STATE_DIE);
							if (goomba->getType() == GOOMBA_TYPE_NORMAL)
							{
								GainScore(SCORE_100);
								RenderPoint(EFFECT_TYPE_100_POINT);
							}
							else
							{
								GainScore(SCORE_800);
								RenderPoint(EFFECT_TYPE_800_POINT);
							}
							//deflect
							if (level != MARIO_LEVEL_LEAF)
								vy = -MARIO_JUMP_DEFLECT_SPEED;
							else
								vy = -MARIO_LEAF_JUMP_DEFLECT_SPEED;
						}
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								LevelDown();
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Goomba
			//else if (dynamic_cast<CPortal *>(e->obj))
			//{
			//	CPortal *p = dynamic_cast<CPortal *>(e->obj);
			//	CGame::GetInstance()->SwitchScene(p->GetSceneId());
			//} //if Portal
			else if (dynamic_cast<CKoopas *>(e->obj))
			{
				CKoopas *koopas = dynamic_cast<CKoopas *>(e->obj);
				float koopas_bb_left, koopas_bb_top, koopas_bb_right, koopas_bb_bottom;
				e->obj->GetBoundingBox(koopas_bb_left, koopas_bb_top, koopas_bb_right, koopas_bb_bottom); //koopas bounding box

				if (e->nx != 0 || e->ny > 0)
				{
					
					if (koopas->GetState() == KOOPAS_STATE_SHELL)
					{
						if (speed_up == false)
						{
							if (this->nx > 0)
							{
								Kick();
								GainScore(SCORE_100);
								RenderPoint(EFFECT_TYPE_100_POINT);
								koopas->SetState(KOOPAS_STATE_SPIN_RIGHT);
							}
							else
							{
								Kick();
								GainScore(SCORE_100);
								RenderPoint(EFFECT_TYPE_100_POINT);
								koopas->SetState(KOOPAS_STATE_SPIN_LEFT);//kick the shell
							}
						}
						else
						{//hold koopas
							holding = true;
							koopas->setHolded(true);
						}


					}
					else
						if (untouchable == 0)
						{
							if (koopas->GetState() != KOOPAS_STATE_DIE)
							{
								if (level > MARIO_LEVEL_SMALL)
								{
									LevelDown();
									StartUntouchable();
								}
								else
									SetState(MARIO_STATE_DIE);
							}
						}
				}

				if (e->ny < 0)
				{

					if (koopas->GetState() != KOOPAS_STATE_DIE)
					{
						if (koopas->GetState() != KOOPAS_STATE_SHELL)
						{
							if (koopas->getLevel() == KOOPAS_LEVEL_FLY)
							{
								koopas->LevelDown();
								GainScore(SCORE_100);
								RenderPoint(EFFECT_TYPE_100_POINT);
								if(level != MARIO_LEVEL_LEAF)
									vy = -MARIO_JUMP_DEFLECT_SPEED;
								else
									vy = -MARIO_LEAF_JUMP_DEFLECT_SPEED;
							}
							else
							{
								koopas->SetState(KOOPAS_STATE_SHELL);
								GainScore(SCORE_100);
								RenderPoint(EFFECT_TYPE_100_POINT);
								if (level != MARIO_LEVEL_LEAF)
									vy = -MARIO_JUMP_DEFLECT_SPEED;
								else
									vy = -MARIO_LEAF_JUMP_DEFLECT_SPEED;
							}
						}
						else
							if (e->ny < 0 && koopas->GetState() == KOOPAS_STATE_SHELL && mario_bb_left + MARIO_BIG_BBOX_WIDTH / 2 < koopas_bb_left + KOOPAS_BBOX_WIDTH / 2)
							{
								koopas->SetState(KOOPAS_STATE_SPIN_RIGHT);
								y += dy;
							}
							else
							{
								koopas->SetState(KOOPAS_STATE_SPIN_LEFT);
								y += dy;
							}
					}
				}
			} //if Koopas
			else
				if (dynamic_cast<CBlock*> (e->obj))
				{
					CBlock *block = dynamic_cast<CBlock*>(e->obj);
					if (e->ny < 0) {
						vy = 0;
						x += dx;
					}
					else
						if (e->ny > 0)
						{
							x += dx;
							y += dy;
						}
					if (e->nx != 0)
					{
						x += dx;
					}
				}//if block
				else if (dynamic_cast<CCoin*>(e->obj))
				{
					CCoin *coin = dynamic_cast<CCoin*>(e->obj);
					isonground = false; // fix double jump bug
					GainMoney();
					coin->setEnable(false);
					coin->setVisable(false);
				}
				else if (dynamic_cast<CPine*>(e->obj))
				{
					CPine *pine = dynamic_cast<CPine*>(e->obj);
					if (e->nx != 0) {
						vx = 0;
						break;
					}
					if (e->ny != 0) {
						vy = 0;
						x += dx;
						if (e->ny < 0 && pine->canSwitchScence() && state == MARIO_STATE_SIT)
						{
							SetState(MARIO_STATE_PINEDOWN);
						}
						else if (e->ny > 0 && pine->canSwitchScence())
							SetState(MARIO_STATE_PINEUP);
					}
				} //if Pine
				else if (dynamic_cast<CBreakableBrick*>(e->obj))
				{
					CBreakableBrick *breakablebrick = dynamic_cast<CBreakableBrick*>(e->obj);
					if (e->nx != 0)
					{
						vx = 0;
						break; //to stop interact walking when collide on y
					}
					else
					if (e->ny != 0)
					{
						vy = 0;
						x += dx;
					}
				} //if breakable brick
				else if (dynamic_cast<CQuestionBrick*>(e->obj))
				{
					CQuestionBrick *questionbrick = dynamic_cast<CQuestionBrick*>(e->obj);
					if (e->nx != 0)
					{
						vx = 0;
						break; //to stop interact walking when collide on y
					}
					if (e->ny < 0)
					{
						vy = 0;
						x += dx;
					}
					else
					{
						if (questionbrick->getType() == QUESTION_BRICK_TYPE_NORMAL)
						{
							if (questionbrick->hasItem() && level == MARIO_LEVEL_SMALL)
								questionbrick->CreateItem(ITEM_MUSHROOM_RED);
							else if (questionbrick->hasItem() && level <= MARIO_LEVEL_LEAF)
								questionbrick->CreateItem(ITEM_LEAF);
							else if (questionbrick->hasCoin())
								questionbrick->CreateItem(ITEM_MONEY);
						}
						else if(questionbrick->getType() == QUESTION_BRICK_TYPE_ONSKY_BREAKABLE_ALIKE)//1 UP
						{
							if (questionbrick->hasItem())
								questionbrick->CreateItem(ITEM_MUSHROOM_GREEN);
						}
						else if (questionbrick->getType() == QUESTION_BRICK_TYPE_MONEY_BUTTON_CREATOR)
						{
							if (questionbrick->hasItem())
								questionbrick->CreateItem(ITEM_MONEY_BUTTON);
							questionbrick->getUsed();//coin and item set to 0
							vy = MARIO_GRAVITY; //push mario down a bit
							isfalling = true;
							jumpable = false;
							return; //no brick jumping needed
						}

						if (!questionbrick->isEmpty())
							questionbrick->Jump();
						if (questionbrick->hasCoin())
							GainMoney();
						questionbrick->getUsed();//coin and item set to 0
						vy = MARIO_GRAVITY; //push mario down a bit
						isfalling = true;
						jumpable = false;
					}
				} //if breakable brick
				else if (dynamic_cast<CBrick*>(e->obj))
				{
					CBrick *brick = dynamic_cast<CBrick*>(e->obj);

					float brick_bb_left, brick_bb_top, brick_bb_right, brick_bb_bottom;
					brick->GetBoundingBox(brick_bb_left, brick_bb_top, brick_bb_right, brick_bb_bottom);

					if (brick->getType() == BRICK_TYPE_INVISIBLE)
					{
						x += dx; y += dy;
					} //////INVISIBLE
					else if(brick->getType() == BRICK_TYPE_MONEYBUTTON)
					{
						x += dx; y += dy;
					} //////MONEY BUTTON
					else if (brick->getType() == BRICK_TYPE_CLOUD) //gach may
					{
						if (e->ny < 0)
						{
							vy = 0;
							x += dx;
							break;
						}
						else if (e->ny > 0)
							y += dy;
						else if (e->nx != 0)
						{
							x += dx;
							y += dy;
						}
						
					}
					else 
					{ //gach thuong
						if (e->nx != 0 && brick->canBounce())
						{
							vx = 0;
							vy = 0; //to stop interact walking when collide on y
							break;
						}
						else
						if (e->ny != 0 )
						{
							vy = 0;
							x += dx;//loi di xuyen dach	
							
						}
					}
				} //if brick
				else if (dynamic_cast<CLeaf*>(e->obj))
				{
					CLeaf *leaf = dynamic_cast<CLeaf*>(e->obj);
					if(level < MARIO_LEVEL_LEAF)
						LevelUp();
					else
					{
						GainScore(SCORE_1000);
						RenderPoint(EFFECT_TYPE_1000_POINT);
					}
					leaf->setVisable(false);
					leaf->setEnable(false);
				} //if Leaf
				else if (dynamic_cast<CMushroom*>(e->obj))
				{
					CMushroom *mushroom = dynamic_cast<CMushroom*>(e->obj);
					if (mushroom->getType() == MUSHROOM_TYPE_RED)
					{
						LevelUp();
						GainScore(SCORE_1000);
						RenderPoint(EFFECT_TYPE_1000_POINT);
					}
					else //type green
					{
						LifeUp();
						RenderPoint(EFFECT_TYPE_1_UP);
					}
					mushroom->setVisable(false);
					mushroom->setEnable(false);
				} //if Mushroom
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	for (int i = 0; i < firebullets.size(); i++)
	{
		if (firebullets[i]->isEnabled())
			firebullets[i]->Update(dt, coObjects);
	} //update fireballs
}

void CMario::Render()
{
	int ani = -1;
	if (state == MARIO_STATE_ICON || state == MARIO_STATE_ICON_GO_LEFT || state == MARIO_STATE_ICON_GO_RIGHT
		|| state == MARIO_STATE_ICON_GO_UP || state == MARIO_STATE_ICON_GO_DOWN)
		ani = MARIO_ANI_ICON;
	else
	if (istransformingtoBig)
	{
		if (nx < 0)
			ani = MARIO_ANI_TRANSFORM_SMALL_TO_BIG_LEFT;
		else
			ani = MARIO_ANI_TRANSFORM_SMALL_TO_BIG_RIGHT;
	}
	else if (istransformingtoLeaf)
	{
		ani = MARIO_ANI_TRANSFORM_TO_LEAF;
	}
	else
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else if (level == MARIO_LEVEL_BIG) //============================================================ MARIO BIG ANIMATION
	{
		if (state == MARIO_STATE_SIT) //mario sit
		{
			if (nx > 0)
				ani = MARIO_ANI_BIG_SIT_RIGHT;
			else
				ani = MARIO_ANI_BIG_SIT_LEFT;
		}
		else if (vy < 0)
		{
			if (nx < 0) ani = MARIO_ANI_BIG_JUMP_LEFT;
			else ani = MARIO_ANI_BIG_JUMP_RIGHT;
		}
		else if (vx == 0)
		{
			if (nx > 0)
			{
				ani = MARIO_ANI_BIG_IDLE_RIGHT;
				if (holding) ani = MARIO_ANI_BIG_HOLD_RIGHT;
				else if (kicking) ani = MARIO_ANI_BIG_KICK_RIGHT;
			}
			else
			{
				ani = MARIO_ANI_BIG_IDLE_LEFT;
				if (holding) ani = MARIO_ANI_BIG_HOLD_LEFT;
				else if (kicking) ani = MARIO_ANI_BIG_KICK_LEFT;
			}
		}
		else if (vx > 0)//walk or run right
		{
			if (vx * ax < 0 && state == MARIO_STATE_WALKING_LEFT)
				ani = MARIO_ANI_BIG_BRAKE_RIGHT;
			else
			{
				if (abs(vx) > MARIO_MAX_WALK_SPEED && Stack <= MARIO_WALKING_STACK_MAX)
					ani = MARIO_ANI_BIG_WALK_FAST_RIGHT;
				if (abs(vx) == MARIO_MAX_RUN_SPEED && Stack == MARIO_RUNNING_STACK_MAX)
					ani = MARIO_ANI_BIG_RUN_RIGHT;
				else
					ani = MARIO_ANI_BIG_WALKING_RIGHT;
			}
		}
		else //walk or run left
		{
			if (vx * ax < 0 && state == MARIO_STATE_WALKING_RIGHT)
				ani = MARIO_ANI_BIG_BRAKE_LEFT;
			else
			{
				if (abs(vx) > MARIO_MAX_WALK_SPEED && Stack <= MARIO_WALKING_STACK_MAX)
					ani = MARIO_ANI_BIG_WALK_FAST_LEFT;
				if (abs(vx) == MARIO_MAX_RUN_SPEED && Stack == MARIO_RUNNING_STACK_MAX)
					ani = MARIO_ANI_BIG_RUN_LEFT;
				else
					ani = MARIO_ANI_BIG_WALKING_LEFT;
			}
		}
	}
	else if (level == MARIO_LEVEL_SMALL)//==========================================================================MARIO SMALL ANIMATION
	{
		if (vy < 0)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_JUMP_RIGHT;
			else
				ani = MARIO_ANI_SMALL_JUMP_LEFT;
		} //jump animation
		else if (vx == 0)
		{
			if (nx > 0)
			{
				ani = MARIO_ANI_SMALL_IDLE_RIGHT;
				if (holding) ani = MARIO_ANI_SMALL_HOLD_RIGHT;
				else if (kicking) ani = MARIO_ANI_SMALL_KICK_RIGHT;
			}
			else
			{
				ani = MARIO_ANI_SMALL_IDLE_LEFT;
				if (holding) ani = MARIO_ANI_SMALL_HOLD_LEFT;
				else if (kicking) ani = MARIO_ANI_SMALL_KICK_LEFT;
			}
		}
		else if (vx > 0) //walk or run right
		{
			if (vx * ax < 0 && state == MARIO_STATE_WALKING_LEFT)
				ani = MARIO_ANI_SMALL_BRAKE_RIGHT;
			else
			{
				if (abs(vx) > MARIO_MAX_WALK_SPEED && Stack <= MARIO_WALKING_STACK_MAX)
					ani = MARIO_ANI_SMALL_WALK_FAST_RIGHT;
				if (abs(vx) == MARIO_MAX_RUN_SPEED && Stack == MARIO_RUNNING_STACK_MAX)
					ani = MARIO_ANI_SMALL_RUN_RIGHT;
				else
					ani = MARIO_ANI_SMALL_WALKING_RIGHT;
			}
		}
		else //walk or run left
		{
			if (vx * ax < 0 && state == MARIO_STATE_WALKING_RIGHT)
				ani = MARIO_ANI_SMALL_BRAKE_LEFT;
			else
			{
				if (abs(vx) > MARIO_MAX_WALK_SPEED && Stack <= MARIO_WALKING_STACK_MAX)
					ani = MARIO_ANI_SMALL_WALK_FAST_LEFT;
				if (abs(vx) == MARIO_MAX_RUN_SPEED && Stack == MARIO_RUNNING_STACK_MAX)
					ani = MARIO_ANI_SMALL_RUN_LEFT;
				else
					ani = MARIO_ANI_SMALL_WALKING_LEFT;
			}
		}
	}
	else
		if (level == MARIO_LEVEL_LEAF)//===================================================== MARIO LEAF ANIMATION ==========================
		{
			if (state == MARIO_STATE_PINEDOWN || state == MARIO_STATE_PINEDOWN)
				ani = MARIO_ANI_LEAF_PINEDOWN;
			else
			if (state == MARIO_STATE_SIT) //mario sit
			{
				if (nx > 0)
					ani = MARIO_ANI_LEAF_SIT_RIGHT;
				else
					ani = MARIO_ANI_LEAF_SIT_LEFT;
			}
			else
				if (flapping)
				{
					if (nx > 0)
						ani = MARIO_ANI_LEAF_FLAP_RIGHT;
					else
						ani = MARIO_ANI_LEAF_FLAP_LEFT;
				}
				else if (spinning == true)				{
					if (nx > 0)
						ani = MARIO_ANI_LEAF_SPIN_RIGHT;
					else
						ani = MARIO_ANI_LEAF_SPIN_LEFT;
				}
				else if (flying == true)
				{
					if (nx > 0)
						ani = MARIO_ANI_LEAF_FLY_RIGHT;
					else
						ani = MARIO_ANI_LEAF_FLY_LEFT;
				}
				else
					if (vy < 0)
					{
						if (nx < 0) ani = MARIO_ANI_LEAF_JUMP_LEFT_LOW;
						else ani = MARIO_ANI_LEAF_JUMP_RIGHT_LOW;
					}
					else
						if (vx == 0)
						{
							if (nx > 0)
							{
								ani = MARIO_ANI_LEAF_IDLE_RIGHT;
								if (holding) ani = MARIO_ANI_LEAF_HOLD_RIGHT;
								else if (kicking) ani = MARIO_ANI_LEAF_KICK_RIGHT;
							}
							else
							{
								ani = MARIO_ANI_LEAF_IDLE_LEFT;
								if (holding) ani = MARIO_ANI_LEAF_HOLD_LEFT;
								else if (kicking) ani = MARIO_ANI_LEAF_KICK_LEFT;
							}
						}
						else if (vx > 0) // walk or run right
						{
							if (vx * ax < 0 && state == MARIO_STATE_WALKING_LEFT)
								ani = MARIO_ANI_LEAF_BRAKE_RIGHT;
							else
							{
								if (abs(vx) > MARIO_MAX_WALK_SPEED && Stack <= MARIO_WALKING_STACK_MAX)
									ani = MARIO_ANI_LEAF_WALK_FAST_RIGHT;
								if (abs(vx) == MARIO_MAX_RUN_SPEED && Stack == MARIO_RUNNING_STACK_MAX)
									ani = MARIO_ANI_LEAF_RUN_RIGHT;
								else
									ani = MARIO_ANI_LEAF_WALK_RIGHT;
							}
						}
						else //walk or run left
						{
							if (vx * ax < 0 && state == MARIO_STATE_WALKING_RIGHT)
								ani = MARIO_ANI_LEAF_BRAKE_LEFT;
							else
							{
								if (abs(vx) > MARIO_MAX_WALK_SPEED && Stack <= MARIO_WALKING_STACK_MAX)
									ani = MARIO_ANI_LEAF_WALK_FAST_LEFT;
								if (abs(vx) == MARIO_MAX_RUN_SPEED && Stack == MARIO_RUNNING_STACK_MAX)
									ani = MARIO_ANI_LEAF_RUN_LEFT;
								else
									ani = MARIO_ANI_LEAF_WALK_LEFT;
							}
						}
		} ////////////////level LEAF
		else if (level == MARIO_LEVEL_FIRE)
		{
			if (state == MARIO_STATE_SIT) //mario sit
			{
				if (nx > 0)
					ani = MARIO_ANI_FIRE_SIT_RIGHT;
				else
					ani = MARIO_ANI_FIRE_SIT_LEFT;
			}
			else
				if (vy < 0)
				{
					if (nx < 0) ani = MARIO_ANI_FIRE_JUMP_LEFT_LOW;
					else ani = MARIO_ANI_FIRE_JUMP_RIGHT_LOW;
				}
				else
					if (vx == 0) //idle and hold shell
					{
						if (nx > 0)
						{
							ani = MARIO_ANI_FIRE_IDLE_RIGHT;
							if (holding) ani = MARIO_ANI_FIRE_HOLD_RIGHT;
							else if (kicking) ani = MARIO_ANI_FIRE_KICK_RIGHT;
						}
						else
						{
							ani = MARIO_ANI_FIRE_IDLE_LEFT;
							if (holding) ani = MARIO_ANI_FIRE_HOLD_LEFT;
							else if (kicking) ani = MARIO_ANI_FIRE_KICK_LEFT;
						}
					}
					else if (vx > 0) // walk or run right
					{
						if (vx * ax < 0 && state == MARIO_STATE_WALKING_LEFT)
							ani = MARIO_ANI_FIRE_BRAKE_RIGHT;
						else
						{
							if (abs(vx) > MARIO_MAX_WALK_SPEED && Stack <= MARIO_WALKING_STACK_MAX)
								ani = MARIO_ANI_FIRE_WALK_FAST_RIGHT;
							if (abs(vx) == MARIO_MAX_RUN_SPEED && Stack == MARIO_RUNNING_STACK_MAX)
								ani = MARIO_ANI_FIRE_RUN_RIGHT;
							else
								ani = MARIO_ANI_FIRE_WALK_RIGHT;
						}
					}
					else // walk or run left
					{
						if (vx * ax < 0 && state == MARIO_STATE_WALKING_RIGHT)
							ani = MARIO_ANI_FIRE_BRAKE_LEFT;
						else
						{
							if (abs(vx) > MARIO_MAX_WALK_SPEED && Stack <= MARIO_WALKING_STACK_MAX)
								ani = MARIO_ANI_FIRE_WALK_FAST_LEFT;
							if (abs(vx) == MARIO_MAX_RUN_SPEED && Stack == MARIO_RUNNING_STACK_MAX)
								ani = MARIO_ANI_FIRE_RUN_LEFT;
							else
								ani = MARIO_ANI_FIRE_WALK_LEFT;
						}
					}
		} //level FIRE


	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	for (int i = 0; i < firebullets.size(); i++)
	{
		if (firebullets[i]->isVisabled())
			firebullets[i]->Render();
	} //render shooting
	//RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		/*if (ax < 0 && vy > 0)
		{
			ischangingdirection = true;
		}*/
		ax = MARIO_ACCELERATION;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		/*if (ax > 0 && vy > 0)
			ischangingdirection = true;*/
		ax = -MARIO_ACCELERATION;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		if (vy == 0)
			vy = -MARIO_JUMP_SPEED_Y_MIN;
			ay = -MARIO_GRAVITY;
		
		break;
	case MARIO_STATE_IDLE:
		if (vx > 0)
		{
			ax = -MARIO_ACCELERATION;
		}
		if (vx < 0)
		{
			ax = MARIO_ACCELERATION;
		}

		if (abs(vx) <= MARIO_MIN_WALK_SPEED) //if mario is slow enough then stop
		{
			vx = 0;
			ax = 0;
		}
		break;
	case MARIO_STATE_SIT:
		vx = 0;
		break;
	case MARIO_STATE_PINEDOWN:
		vy = 0;
		is_lost_control = true;
		StartPineDown();
		break;
	case MARIO_STATE_PINEUP:
		vy = 0;
		is_lost_control = true;
		StartPineUp();
		break;
	case MARIO_STATE_ICON:
		vx = 0;
		vy = 0;
		ax = 0;
		ay = 0;
		break;
	case MARIO_STATE_ICON_GO_LEFT:
		icon_new_x = x - MARIO_ICON_SPEED_DX;
		icon_old_x = x;
		vx = -MARIO_ICON_SPEED;
		is_going_left = true;
		is_lost_control = true;
		//x -= MARIO_ICON_SPEED_DX;
		break;
	case MARIO_STATE_ICON_GO_RIGHT:
		icon_new_x = x + MARIO_ICON_SPEED_DX;
		icon_old_x = x;
		vx = MARIO_ICON_SPEED;
		is_going_right = true;
		is_lost_control = true;
		//x += MARIO_ICON_SPEED_DX;
		break;
	case MARIO_STATE_ICON_GO_UP:
		icon_new_y = y - MARIO_ICON_SPEED_DX;
		icon_old_y = y;
		vy = -MARIO_ICON_SPEED;
		is_going_up = true;
		is_lost_control = true;
		//y -= MARIO_ICON_SPEED_DX;
		break;
	case MARIO_STATE_ICON_GO_DOWN:
		icon_new_y = y + MARIO_ICON_SPEED_DX;
		icon_old_y = y;
		vy = MARIO_ICON_SPEED;
		is_going_down = true;
		is_lost_control = true;
		//y += MARIO_ICON_SPEED_DX;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CMario::SetLevel(int l)
{
	if (level == MARIO_LEVEL_SMALL)
	{
		level = l;
		SetPosition(x , y - (MARIO_LEAF_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT)); //push mario up a bit
	}
	else
	{
		level = l;
	}
}

void CMario::LevelUp()
{
	if (level == MARIO_LEVEL_SMALL)
	{
		startTransformtoBig();
		SetLevel(MARIO_LEVEL_BIG);
	}
	else
	if (level == MARIO_LEVEL_BIG)
	{
		startTransFormtoLeaf();
		SetLevel(MARIO_LEVEL_LEAF);
	}
}

void CMario::LevelDown()
{
	if (level == MARIO_LEVEL_BIG)
	{
		startTransformtoBig();
		SetLevel(MARIO_LEVEL_SMALL);
	}
	else
	if (level >= MARIO_LEVEL_LEAF)
	{
		startTransFormtoLeaf();
		SetLevel(MARIO_LEVEL_BIG);
	}
}

void CMario::LifeUp()
{
	Life++;
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	if (level == MARIO_LEVEL_BIG)
	{
		if (state == MARIO_STATE_SIT)
		{
			right = x + MARIO_BBOX_SIT_WIDTH;
			bottom = y + MARIO_BBOX_SIT_HEIGHT - 1;
		}
		else
		{
			right = x + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
	}//big mario
	else if (level == MARIO_LEVEL_LEAF)
	{
		if (state == MARIO_STATE_SIT)
		{
			right = x + MARIO_BBOX_LEAF_SIT_WIDTH;
			bottom = y + MARIO_BBOX_LEAF_SIT_HEIGHT - 1;
		}
		else
			if (spinning) //mario is spinning tail
			{
				if (nx < 0)
				{
					left = x ;
					top = y;
					right = x + MARIO_LEAF_BBOX_WIDTH;
					bottom = y + MARIO_LEAF_BBOX_HEIGHT;
				}
				else
				{
					left = x + MARIO_LEAF_BBOX_TAILDOWN_WIDTH;
					right = left + MARIO_LEAF_BBOX_WIDTH;
					top = y;
					bottom = y + MARIO_LEAF_BBOX_HEIGHT;
				}
			}
			else
			{ //standing
				if (nx < 0)
				{
					right = x + MARIO_LEAF_BBOX_WIDTH;
					bottom = y + MARIO_LEAF_BBOX_HEIGHT;
				}
				else
				{
					left = x + MARIO_LEAF_BBOX_TAILDOWN_WIDTH;
					right = left + MARIO_LEAF_BBOX_WIDTH;
					top = y;
					bottom = y + MARIO_LEAF_BBOX_HEIGHT;
				}
			}
	}//leaf mario
	else if (level == MARIO_LEVEL_FIRE)
	{
		if (state == MARIO_STATE_SIT)
		{
			right = x + MARIO_FIRE_BBOX_SIT_WIDTH;
			bottom = y + MARIO_FIRE_BBOX_SIT_HEIGHT - 1;
		}
		else
		{
			right = x + MARIO_FIRE_BBOX_WIDTH;
			bottom = y + MARIO_FIRE_BBOX_HEIGHT;
		}
	}// FIRE mario
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}//small mario


	//special:
	if (state == MARIO_STATE_PINEDOWN || state == MARIO_STATE_PINEUP)
		left = top = right = bottom = 0;
}

void CMario::ManageAccelerationAndSpeed()
{
	//All the limit put here instead of setstae to avoid laggy bug
	//limit mario jump speed:
	/*if ( vy == -MARIO_JUMP_SPEED_Y_MAX)
		ay = MARIO_GRAVITY;*/
	if (vy < -MARIO_JUMP_SPEED_Y_MAX)
	{
		vy = -MARIO_JUMP_SPEED_Y_MAX;
		if (!flying)
		{
			ay = MARIO_GRAVITY;
			jumpable = false;
		}
		else
		{
			ay = -MARIO_GRAVITY;
		}
	}
	
	if (flying && vy <= -MARIO_FLY_SPEED_MAX)
	{
		vy = -MARIO_FLY_SPEED_MAX;
	}

	

	//limit mario walk speed
	if (abs(vx) > MARIO_MAX_WALK_SPEED && !speed_up)
		vx = nx * MARIO_MAX_WALK_SPEED;
	//limit mario run speed
	if (abs(vx) > MARIO_MAX_RUN_SPEED) //on key up A button still limit
		vx = nx * MARIO_MAX_RUN_SPEED;
	//slow down if change direction when running
	if (vx * ax < 0 && abs(vx) > MARIO_MAX_WALK_SPEED && (state == MARIO_STATE_WALKING_LEFT || state == MARIO_STATE_WALKING_RIGHT))
	{
		vx = -nx * MARIO_MAX_WALK_SPEED;
		Stack = MARIO_WALKING_STACK_MAX;
	}

	if (isonground && speed_up && GetTickCount() - speedup_start > MARIO_STACKUP_TIME && vx != 0)
	{
		Stack++;
		speedup_start = GetTickCount();
		if (Stack > MARIO_RUNNING_STACK_MAX)
			Stack = MARIO_RUNNING_STACK_MAX;
	}
	if (!speed_up && GetTickCount() - speedup_stop > MARIO_STACKUP_TIME)
	{
		Stack--;
		speedup_stop = GetTickCount();
		if (Stack < 0)
			Stack = 0;
	}
}


/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_SMALL);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CMario::Shot()
{
	CFireBall *fireball = new CFireBall(x + MARIO_LEAF_BBOX_WIDTH, y + MARIO_LEAF_BBOX_HEIGHT / 2); //shoot right
	if (nx < 0) //if mario is facing left shoot left
	{
		fireball->SetSpeed(-FIREBALL_SPEED_X, FIREBALL_SPEED_Y);
		fireball->SetPosition(x, y + MARIO_LEAF_BBOX_HEIGHT / 2);
	}
	firebullets.push_back(fireball);
}

void CMario::Kick()
{
	kicking = true;
	kick_start = GetTickCount();
}

bool CMario::isSpecialAnimation(int ani)
{
	if (ani == MARIO_ANI_LEAF_SPIN_LEFT)
		return true;
	if (ani == MARIO_ANI_LEAF_SPIN_RIGHT)
		return true;
	return false;
}
void CMario::RenderSpecialAnimation(int ani) {
}

void CMario::TimingEvent() {
	//when transforming stop
	if (istransformingtoLeaf || istransformingtoBig) { vx = 0; vy = 0; }
	if (is_pine_down) { vx = 0; vy = MARIO_PINE_DOWN_SPEED; }
	if (is_pine_up) { vx = 0; vy = -MARIO_PINE_UP_SPEED; }

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	//if leaf mario is spinning tail then stop
	if (GetTickCount() - spin_start > 400) //4 sprites = 4*100ms
	{
		spin_start = 0; //reset timer
		spinning = false; //no more spinning
	}

	if (GetTickCount() - flap_start > 100) //reset timer flapping  2 sprites = (2*25ms)*2
	{
		flap_start = 0; //reset timer
		flapping = false; //no more spinning
	}

	if (GetTickCount() - kick_start > 300) //reset timer kicking
	{
		kick_start = 0; //reset timer
		kicking = false;//no more kicking
	}

	if (GetTickCount64() - leaf_transform_start > TRANSFORM_TO_LEAF_MARIO_TIME && istransformingtoLeaf)
	{
		istransformingtoLeaf = false;
		leaf_transform_start = 0;
	}

	if (GetTickCount64() - big_transform_start > TRANSFORM_TO_BIG_MARIO_TIME && istransformingtoBig)
	{
		istransformingtoBig = false;
		big_transform_start = 0;
	}
	if (is_pine_down && GetTickCount64() - start_pine_down > MARIO_PINEDOWN_TIME)
	{
		is_lost_control = false;
		is_pine_down = false;
		start_pine_down = 0;
		CGame *game = CGame::GetInstance();
		
			game->SwitchSceneEx(WORLD_1_1_SECRECT_SCENCE_ID, WORLD_1_1_SECRECT_START_X, WORLD_1_1_SECRECT_START_Y);
		
	}

	if (is_pine_up && GetTickCount64() - start_pine_up > MARIO_PINEUP_TIME)
	{
		is_lost_control = false;
		is_pine_up = false;
		start_pine_up = 0;
		CGame *game = CGame::GetInstance();
		game->SwitchBackScence(WORLD_1_1_SCENCE_ID, WORLD_1_1_SWITCHBACK_START_X, WORLD_1_1_SWITCHBACK_START_Y);
	}
	
}

void CMario::GainMoney()
{
	Money++;
}
void CMario::GainScore(int score) 
{
	Score += score;
}
bool CMario::isEqual(float x, float y)
{
	const float epsilon = 0.1f;
	/*if (fabs(x - y) < epsilon)*/
	if(abs(x-y) < 5)
		return true; //they are same
	return false; //they are not same
}

void CMario::RenderPoint(int type)
{
	CEffect *point_effect = new CEffect(type);
	point_effect->SetPosition(this->x, this->y - MARIO_BIG_BBOX_HEIGHT);
	point_effect->SetStartPosition(this->x, this->y - MARIO_BIG_BBOX_HEIGHT);

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(EFFECT_SET_ID);
	point_effect->SetAnimationSet(ani_set);

	CPlayScene *currenscence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	currenscence->PushBackObject(point_effect);
}