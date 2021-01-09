#include "Goomba.h"
#include "Koopas.h"
#include "Brick.h"
#include "Pine.h"
#include "Block.h"
#include "Block.h"
#include "PlayScence.h"
#include "Mario.h"
#include "Utils.h"
#include "QuestionBrick.h"
#include "Game.h"
#include "Effect.h"

CGoomba::CGoomba(int lvl)
{
	level = lvl;
	start_level = lvl;
	if (lvl == GOOMBA_LEVEL_FLY)
		type = GOOMBA_TYPE_RED;
	else
		type = GOOMBA_TYPE_NORMAL;
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (enable == false)
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
		return;
	}
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else
	{
		if (level == GOOMBA_LEVEL_FLY)
			bottom = y + FLYGOOMBA_BBOX_HEIGHT;
		else
			bottom = y + GOOMBA_BBOX_HEIGHT;
	}
		
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	// Simple fall down
	vy += GOOMBA_FLY_GRAVITY * dt;
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	//Reset();
	CalcPotentialCollisions(coObjects, coEvents);
	if (level == GOOMBA_LEVEL_FLY)
	{
			if (GetTickCount64() - fly_goomba_start_walking > FLYGOOMBA_HIGH_JUMP_TIME)
			{
					vy = -FLYGOOMBA_HIGH_JUMP_SPEED;
				fly_goomba_start_walking = GetTickCount64();
			}	
	}
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (vx > 0 && x > WORLD_1_1_WIDTH) {
			x = WORLD_1_1_WIDTH; vx = -vx;
		}

	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		HitByTail();
		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		/*x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;*/

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas *koopas = dynamic_cast<CKoopas *>(e->obj);
				if (e->nx != 0)
				{
					if (koopas->GetState() == KOOPAS_STATE_SPIN_LEFT || koopas->GetState() == KOOPAS_STATE_SPIN_RIGHT)
						SetState(GOOMBA_STATE_DIE); //die if get hit by a spinning koopas
					else
						vx = -vx; //turn if meet an alive koopas
				}
				if (e->ny != 0)
				{
					x += dx;
					y += dy;
				}

			} //if koopas
			else if (dynamic_cast<CBlock*>(e->obj))
			{
				CBlock *block = dynamic_cast<CBlock*>(e->obj);
				if (e->ny < 0) {
					vy = 0;
					x += dx;
				}
				if (e->nx != 0)
				{
					vy = 0;
					x += dx;
				}
			}// if Block
			else
				if (dynamic_cast<CPine*>(e->obj))
				{
					CPine *pine = dynamic_cast<CPine*>(e->obj);
					if (e->nx != 0) {
						x += min_tx * rdx + nx * 0.4f;
						vx = -vx;
					}
					if (e->ny != 0) {
						vy = 0;
						y += min_ty * rdy + ny * 0.4f;
					}
				}// if Pine
				else if (dynamic_cast<CGoomba*>(e->obj))
				{
					CGoomba *goomba = dynamic_cast<CGoomba*>(e->obj);
					if (e->nx != 0) {
						x += min_tx * rdx + nx * 0.4f;
						vx = -vx;
					}
					if (e->ny != 0)
					{
						x += dx;
					}

				}// if Goomba
				else if (dynamic_cast<CBrick*>(e->obj))
				{
					CBrick *brick = dynamic_cast<CBrick*>(e->obj);
					if (brick->getType() == BRICK_TYPE_INVISIBLE)
					{
						x += dx; y += dy;
					}//====INVISIBLE
					else
					{
						if (e->ny != 0)
						{
							if (level == GOOMBA_LEVEL_NORMAL)
							{
								vy = 0;
								y += min_ty * rdy + ny * 0.4f;
								x += dx;
								if (brick->canBounce() == 1)
									vx = -vx;
							}
							else
							{
								if (brick->canBounce() == 1)
									vx = -vx;
								x += dx;
								y += min_ty * rdy + ny * 0.4f;
								
							}

						}
						if (e->nx != 0)
						{
							x += min_tx * dx + nx * 0.4f;
							if (brick->canBounce() == 1)
								vx = -vx;
						}
					}
				}// if brick
				else if (dynamic_cast<CQuestionBrick*>(e->obj))
				{
					CQuestionBrick *questionbrick = dynamic_cast<CQuestionBrick*>(e->obj);
					vx = -vx;
					y += dy;
				}// if brick
		}

	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{
	int ani = 0;

	if (type == GOOMBA_TYPE_NORMAL)
		 ani = GOOMBA_ANI_WALKING;
	else
		ani = GOOMBA_ANI_LV2_WAlKING;

	if (state == GOOMBA_STATE_DIE) {
		if (type == GOOMBA_TYPE_NORMAL)
			ani = GOOMBA_ANI_DIE;
		else
			ani = GOOMBA_ANI_LV2_DIE;
	}
	else if (level == GOOMBA_LEVEL_FLY)
		ani = GOOMBA_ANI_FLY_RED;

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
		enable = false;
		visable = false;
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_WALKING:
		if (level == GOOMBA_LEVEL_NORMAL)
		{
			vx = -GOOMBA_WALKING_SPEED;
		}
		else
		{
			vx = -GOOMBA_WALKING_SPEED;
			FlyGoombaStartWalking();
		}
		break;
	}
}

void CGoomba::HitByTail()
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
				SetState(GOOMBA_STATE_DIE);
				Render_Tail_Hit();
			}
	
	/*CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(EFFECT_SET_ID);
	ani_set->at(ANI_HIT_TAIL)->Render(x, y);*/
	
}

void CGoomba::LevelDown()
{
	if (level == GOOMBA_LEVEL_FLY)
		level = GOOMBA_LEVEL_NORMAL;
}

void CGoomba::Reset()
{
	CGame *game = CGame::GetInstance();
	float bb_left, bb_top, bb_right, bb_bottom;
	GetBoundingBox(bb_left, bb_top, bb_right, bb_bottom);
	if (!Resetable)
	{
		if (!isInCamera() && !game->isInCamera(start_x - EXTRA_RESET_SPACE, start_y, start_x + (bb_right - bb_left) + EXTRA_RESET_SPACE, start_y + (bb_bottom - bb_top))) //10 tiles away from mario then reset
		{
			Resetable = true;
		}
	}
	else
	{
		if (game->isInCamera(start_x, start_y, start_x + (bb_right - bb_left), start_y + (bb_bottom - bb_top)))
		{
			enable = true;
			visable = true;
			SetPosition(start_x, start_y);
			setLevel(start_level);
			SetState(KOOPAS_STATE_WALKING);
			Resetable = false;
		}
	}
	
		
}

void CGoomba::Render_Tail_Hit() 
{
	CEffect *tailhiteffect = new CEffect( EFFECT_TYPE_TAIL_HIT);
	tailhiteffect->SetPosition(this->x, this->y - GOOMBA_BBOX_HEIGHT/2);
	tailhiteffect->SetStartPosition(this->x, this->y - GOOMBA_BBOX_HEIGHT / 2);

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(EFFECT_SET_ID);
	tailhiteffect->SetAnimationSet(ani_set);

	CPlayScene *currenscence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	currenscence->PushBackObject(tailhiteffect);
}