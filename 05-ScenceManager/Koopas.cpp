#include "Koopas.h"
#include "Brick.h"
#include "Utils.h"
#include "Mario.h"
#include "Pine.h"
#include "Goomba.h"
#include "Block.h"
#include "Playscence.h"
#include "BreakableBrick.h"
#include "QuestionBrick.h"
#include "Coin.h"
#include "Effect.h"
CKoopas::CKoopas(int lvl, int t)
{
	level = lvl;
	type = t;
	start_level = lvl;
	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;


	if (level == KOOPAS_LEVEL_FLY)
	{
		bottom = y + FLYKOOPAS_BBOX_HEIGHT;
		return;
	}

	if (state == KOOPAS_STATE_DIE)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else if (state == KOOPAS_STATE_SHELL)
		bottom = y + KOOPAS_BBOX_HEIGHT_SHELL;
	else if (state == KOOPAS_STATE_SPIN_LEFT || state == KOOPAS_STATE_SPIN_RIGHT)
		bottom = y + KOOPAS_BBOX_HEIGHT_SHELL;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (!enable)
		return;
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 
	// Simple fall down
	vy += KOOPAS_FLY_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	//Reset();
	if (state != KOOPAS_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	//Revire if shell:
	if (is_reviving && GetTickCount64() - start_revive > 8000 && state == KOOPAS_STATE_SHELL)
	{
		is_shell_up = false;
		SetState(KOOPAS_STATE_WALKING);
		is_reviving = false;
		start_revive = 0;
	}
	Reset();
	//holded by mario///////////////////
	getHoldedbyMario();
	// detect when get hit by tail///////////////////
	if(!holded)
	HitByTail();
	// detect when get kick
	getKicked();
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (vx > 0 && x > WORLD_1_1_WIDTH) {
			vx = -vx;
			x = WORLD_1_1_WIDTH;
		}
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!
		/*x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;*/


		/*if (nx != 0) vx = 0;*/
		if (ny != 0) vy = 0;

		float koo_bb_left, koo_bb_top, koo_bb_right, koo_bb_bottom;
		GetBoundingBox(koo_bb_left, koo_bb_top, koo_bb_right, koo_bb_bottom);
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			
			if (dynamic_cast<CBlock*> (e->obj) && e->obj->isEnabled())
			{
				CBlock *block = dynamic_cast<CBlock*>(e->obj);
				if (e->ny < 0)
				{
					if (level == KOOPAS_LEVEL_FLY)
					{
						x += dx;
						vy = -KOOPAS_FLY_SPEED;
					}
					else
					{
						vy = 0;
						x += dx;
						y += min_ty * rdy + ny * 0.4f;
					}
				}
				if (e->nx != 0)
				{
					vy = 0;
					x += dx;
				}
			}//if block
			else if (dynamic_cast<CPine*>(e->obj))
			{
				CPine *pine = dynamic_cast<CPine*>(e->obj);
				if (e->nx != 0) {
					x += min_tx * rdx + nx * 0.4f;
					vx = -vx;
					break;
				}
				if (e->ny != 0) {
					vy = 0;
					y += min_ty * rdy + ny * 0.4f;
				}
			}// if Pine
			else if (dynamic_cast<CGoomba*> (e->obj))
			{
				CGoomba *goomba = dynamic_cast<CGoomba*>(e->obj);
				if (e->nx != 0)
				{
					if (state != KOOPAS_STATE_SPIN_LEFT && state != KOOPAS_STATE_SPIN_RIGHT)
						vx = -vx;
					//y += dy;
				}
			}//if Goomba
			else if (dynamic_cast<CKoopas*> (e->obj))
			{
				CKoopas *koopas = dynamic_cast<CKoopas*>(e->obj);
				if (state == KOOPAS_STATE_SPIN_LEFT || state == KOOPAS_STATE_SPIN_RIGHT)
				{
					koopas->SetState(KOOPAS_STATE_SHELL);
					koopas->is_shell_up = true;
					Render_Tail_Hit();
				}
				else
					vx = -vx;
			}//if Koopas
			else if (dynamic_cast<CBreakableBrick*> (e->obj))
			{
				CBreakableBrick *breakablebrick = dynamic_cast<CBreakableBrick*>(e->obj);
				if (state == KOOPAS_STATE_SPIN_LEFT || state == KOOPAS_STATE_SPIN_RIGHT)
				{
					breakablebrick->Break();
					vx = -vx;
				}
			}//if Breakable brick
			else if (dynamic_cast<CQuestionBrick*> (e->obj))
			{
				CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
				CMario* mario = scence->GetPlayer();
				CQuestionBrick *questionbrick = dynamic_cast<CQuestionBrick*>(e->obj);
				if (e->nx != 0)
				{
					if (state == KOOPAS_STATE_SPIN_LEFT || state == KOOPAS_STATE_SPIN_RIGHT)
					{
						vx = -vx;
						if (questionbrick->hasItem() && mario->getLevel() == MARIO_LEVEL_SMALL)
							questionbrick->CreateItem(ITEM_MUSHROOM_RED);
						else if (questionbrick->hasItem() && mario->getLevel() <= MARIO_LEVEL_LEAF)
							questionbrick->CreateItem(ITEM_LEAF);
						if (!questionbrick->isEmpty())
							questionbrick->Jump();
						questionbrick->getUsed();
						break;
						//add get reward from brick here...
					}
					else
					{
						vx = -vx;
						break;
					}
				}
				if (e->ny != 0)
				{
					x += dx;
				}
			}//if Question brick
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick *brick = dynamic_cast<CBrick*>(e->obj);
				float brick_left, brick_top, brick_right, brick_bottom;
				brick->GetBoundingBox(brick_left, brick_top, brick_right, brick_bottom);
				
				if (brick->getType() == BRICK_TYPE_INVISIBLE)
				{
					if (level == KOOPAS_LEVEL_NORMAL && state == KOOPAS_STATE_WALKING && e->nx != 0)
					{
						vx = -vx;
						break;
					}
					else
					{
						x += dx; y += dy;
					}
				}
				else
				{
					if (e->nx != 0)
					{
						if (brick_top < koo_bb_bottom)
							vx = -vx;
						break;
					}
					if (e->ny != 0)
					{
						if (level == KOOPAS_LEVEL_FLY && e->ny < 0)
						{
							x += dx;
							y += dy;
							vy = -KOOPAS_FLY_SPEED;
						}
						else
						{
							x += dx;
							return;
						}
						//if (brick->canBounce() == 1)
						//	if (state != KOOPAS_STATE_SPIN_LEFT && state != KOOPAS_STATE_SPIN_RIGHT)
						//		vx = -vx; //turn around on edge
					}
				} // if Brick
			}
			else if (dynamic_cast<CCoin*> (e->obj))
				{
				x += dx;
				if(e->ny<0)
					y += dy;
				}//if Coin
		}

		//
		// Collision logic with other objects
		//


	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}
	else if (state == KOOPAS_STATE_SHELL) {
		if (type == KOOPAS_TYPE_GREEN)
		{
			if (is_shell_up)
				ani = KOOPAS_ANI_SHELL_UP;
			else
				ani = KOOPAS_ANI_SHELL;
		}
		else
		{
			if (is_shell_up)
				ani = KOOPAS_ANI_SHELL_UP_RED;
			else
				ani = KOOPAS_ANI_SHELL_RED;
		}
		
	}
	else if (state == KOOPAS_STATE_SPIN_LEFT || state == KOOPAS_STATE_SPIN_RIGHT) {
		if (type == KOOPAS_TYPE_GREEN)
		{
			if (is_shell_up)
				ani = KOOPAS_ANI_SPIN_UP;
			else
				ani = KOOPAS_ANI_SPIN;
		}
		else
		{
			if (is_shell_up)
				ani = KOOPAS_ANI_SPIN_UP_RED;
			else
				ani = KOOPAS_ANI_SPIN_RED;
		}
	}
	else if (vx > 0)
	{
		if (level == KOOPAS_LEVEL_NORMAL)
		{
			if (type == KOOPAS_TYPE_GREEN)
				ani = KOOPAS_ANI_WALKING_RIGHT;
			else
				ani = KOOPAS_ANI_WALK_RIGHT_RED;
		}
		else
			ani = FLYKOOPAS_ANI_FLY_RIGHT;
	}
	else if (vx <= 0)
	{
		if (level == KOOPAS_LEVEL_NORMAL)
		{
			if (type == KOOPAS_TYPE_GREEN)
				ani = KOOPAS_ANI_WALKING_LEFT;
			else
				ani = KOOPAS_ANI_WALK_LEFT_RED;
		}
		else
			ani = FLYKOOPAS_ANI_FLY_LEFT;
	}

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	int old_state = this->state;
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case KOOPAS_STATE_SHELL:
		vx = 0;
		StartRevive();
		break;
	case KOOPAS_STATE_SPIN_LEFT:
		vx = -KOOPAS_SPINNING_SPEED;
		break;
	case KOOPAS_STATE_SPIN_RIGHT:
		vx = KOOPAS_SPINNING_SPEED;
		break;
	case KOOPAS_STATE_WALKING:
		vx = KOOPAS_WALKING_SPEED;
		if (old_state == KOOPAS_STATE_SHELL)
		{
			if(!holded)
				SetPosition(x, y - (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL));
			else
			{
				SetPosition(x, y - (KOOPAS_BBOX_HEIGHT ));
				holded = false;
			}
		}
	}
}

void CKoopas::getHoldedbyMario()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float left, top, right, bottom;
	mario->GetBoundingBox(left, top, right, bottom);
	if (!mario->isHolding()) holded = false;	//if mario stop Holding every koopas then holded == false
	if (holded) //stick koopas with mario depend on bounding box(more accurate)
	{
		vy = 0;
		vx = 0;
		if (mario->getLevel() != MARIO_LEVEL_SMALL)
		{
			if (mario->nx > 0)
				SetPosition(right - 1, top + (bottom - top) / 3);
			else
				SetPosition(left - KOOPAS_BBOX_WIDTH + 1, top + (bottom - top) / 3);
		}
		else //small mario need to hold lower
		{
			if (mario->nx > 0)
				SetPosition(right - 1, top - (bottom - top) / 3);
			else
				SetPosition(left - KOOPAS_BBOX_WIDTH + 1, top - (bottom - top) / 3);
		}

	}
}

void CKoopas::HitByTail()
{
	float bb_left, bb_top, bb_right, bb_bottom;
	float mario_bb_left, mario_bb_top, mario_bb_right, mario_bb_bottom;

	GetBoundingBox(bb_left, bb_top, bb_right, bb_bottom);
	CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = scence->GetPlayer();
	mario->GetBoundingBox(mario_bb_left, mario_bb_top, mario_bb_right, mario_bb_bottom);
	if (mario->isSpinning())
		if (bb_left <= mario_bb_right + MARIO_LEAF_BBOX_TAIL_WIDTH && bb_right >= mario_bb_left - MARIO_LEAF_BBOX_TAIL_WIDTH)
			if (bb_top <= mario_bb_bottom && bb_bottom >= mario_bb_top + (mario_bb_bottom - mario_bb_top) / 2)
			{
				is_shell_up = true;
				if(abs(vy)< MARIO_JUMP_DEFLECT_SPEED)
					vy = -MARIO_JUMP_DEFLECT_SPEED;
				SetState(KOOPAS_STATE_SHELL);
				mario->GainScore(SCORE_100);
				mario->RenderPoint(EFFECT_TYPE_100_POINT);
				Render_Tail_Hit();
				
			}
}

void CKoopas::Render_Tail_Hit()
{
	CEffect *tailhiteffect = new CEffect(EFFECT_TYPE_TAIL_HIT);
	tailhiteffect->SetPosition(this->x, this->y - KOOPAS_BBOX_HEIGHT / 2);
	tailhiteffect->SetStartPosition(this->x, this->y - KOOPAS_BBOX_HEIGHT / 2);

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(EFFECT_SET_ID);
	tailhiteffect->SetAnimationSet(ani_set);

	CPlayScene *currenscence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	currenscence->PushBackObject(tailhiteffect);
}

void CKoopas::getKicked()
{
	if (state == KOOPAS_STATE_SHELL)
	{
		float mario_bb_left, mario_bb_top, mario_bb_right, mario_bb_bottom;

		CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		CMario* mario = scence->GetPlayer();
		mario->GetBoundingBox(mario_bb_left, mario_bb_top, mario_bb_right, mario_bb_bottom);
		if (SpecialCollision(mario_bb_left, mario_bb_top, mario_bb_right, mario_bb_bottom) && !mario->isHolding())
		{
			if (mario->nx < 0)
				SetState(KOOPAS_STATE_SPIN_LEFT);
			else
				SetState(KOOPAS_STATE_SPIN_RIGHT);
		}
	}


}

void CKoopas::LevelDown()
{
	if (level == KOOPAS_LEVEL_FLY)
		level = KOOPAS_LEVEL_NORMAL;
}

void CKoopas::Reset()
{
	CGame *game = CGame::GetInstance();
	float bb_left, bb_top;
	GetPosition(bb_left, bb_top);
	if (!isInCamera() && !game->BBisinCamera(start_x - HALF_EXTRA_SPACE, start_y, start_x + SCREEN_WIDTH + HALF_EXTRA_SPACE, start_y + GOOMBA_BBOX_HEIGHT))
	{
		enable = true;
		visable = true;
		SetPosition(start_x, start_y);
		setLevel(start_level);
		SetState(KOOPAS_STATE_WALKING);
	}
	//if (!Resetable)
	//{
	//	if (!isInCamera() && !game->isInCamera(start_x - EXTRA_RESET_SPACE, start_y, start_x + (bb_right - bb_left) + EXTRA_RESET_SPACE, start_y + (bb_bottom - bb_top))) //10 tiles away from mario then reset
	//	{
	//		Resetable = true;
	//	}
	//}
	//else
	//{
	//	if (game->isInCamera(start_x, start_y, start_x + (bb_right - bb_left), start_y + (bb_bottom - bb_top)))
	//	{
	//		enable = true;
	//		visable = true;
	//		SetPosition(start_x, start_y);
	//		setLevel(start_level);
	//		SetState(KOOPAS_STATE_WALKING);
	//		Resetable = false;
	//	}
	//}
}