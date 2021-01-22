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
	/*if (enable == false)
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
		return;
	}*/
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
	{
		//y -= (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE); //push up a bit
		bottom = y +  GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{
		if (level == GOOMBA_LEVEL_FLY)
		{
			top = y + GOOMBA_FLY_SMALL_EXTRA_HEIGHT;
			bottom = y + FLYGOOMBA_BBOX_HEIGHT;
		}
		else
			bottom = y +  GOOMBA_BBOX_HEIGHT;
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
	if (isrenderdie && GetTickCount64() - renderdie_start > GOOMBA_DIE_TIME)
	{
		isrenderdie = false;
		renderdie_start = 0;
		visable = false;
	}
	Reset();
	if (state != GOOMBA_STATE_DIE && state != GOOMBA_STATE_DIE_KNOCKUP)
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

		float goom_bb_l, goom_bb_t, goom_bb_r, goom_bb_b;
		GetBoundingBox(goom_bb_l, goom_bb_t, goom_bb_r, goom_bb_b);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CKoopas*>(e->obj) && e->obj->isEnabled())
			{
				CKoopas *koopas = dynamic_cast<CKoopas *>(e->obj);
				if (e->nx != 0)
				{
					if (koopas->GetState() == KOOPAS_STATE_SPIN_LEFT || koopas->GetState() == KOOPAS_STATE_SPIN_RIGHT || koopas->isHolded())
					{
						SetState(GOOMBA_STATE_DIE); //die if get hit by a spinning koopas
						Render_Tail_Hit();
					}
					else
						vx = -vx; //turn if meet an alive koopas
				}
				if (e->ny != 0)
				{
					x += dx;
					y += dy;
				}

			} //if koopas
			if (dynamic_cast<CMario*>(e->obj) && e->obj->isEnabled())
			{
				CGame *game = CGame::GetInstance();
				CMario *mario = ((CPlayScene*)game->GetCurrentScene())->GetPlayer();

				if (e->ny > 0)
				{
					if (state != GOOMBA_STATE_DIE)
					{
						if (level == GOOMBA_LEVEL_FLY)
						{
							LevelDown();
							mario->GainScore(SCORE_400);
							mario->RenderPoint(EFFECT_TYPE_100_POINT);
							mario->SetSpeedY(-MARIO_JUMP_DEFLECT_SPEED);
							y = (MARIO_JUMP_DEFLECT_SPEED);
						}
						else
						{
							SetState(GOOMBA_STATE_DIE);
							if (type == GOOMBA_TYPE_NORMAL)
							{
								mario->GainScore(SCORE_100);
								mario->RenderPoint(EFFECT_TYPE_100_POINT);
							}
							else
							{
								mario->GainScore(SCORE_800);
								mario->RenderPoint(EFFECT_TYPE_800_POINT);
							}
							//deflect
							if (mario->getLevel() != MARIO_LEVEL_LEAF)
								mario->SetSpeedY(-MARIO_JUMP_DEFLECT_SPEED);
							else
								mario->SetSpeedY(-MARIO_LEAF_JUMP_DEFLECT_SPEED);
						}
					}
				}
				else if (e->nx != 0)
				{
					if (mario->isUntouchable() == 0)
					{
						if (state != GOOMBA_STATE_DIE)
						{
							if (mario->getLevel() > MARIO_LEVEL_SMALL)
							{
								mario->LevelDown();
								mario->StartUntouchable();
							}
							else
							{
								mario->SetState(MARIO_STATE_DIE);
								mario->LifeDown();
							}
						}
					}
				}

			} 
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
					float brick_bb_l, brick_bb_t, brick_bb_r, brick_bb_b;
					brick->GetBoundingBox(brick_bb_l, brick_bb_t, brick_bb_r, brick_bb_b);
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
								//y += min_ty * rdy + ny * 0.4f;
								x += dx;
								/*if (brick_bb_t < goom_bb_b)
									vx = -vx;*/
							}
							else
							{
								/*if (brick_bb_t < goom_bb_b)
									vx = -vx;*/
								x += dx;
								//y += min_ty * rdy + ny * 0.4f;
								
							}

						}
						if (e->nx != 0)
						{
							x += min_tx * dx + nx * 0.4f;
							if (brick_bb_t < goom_bb_b)
								vx = -vx;
						}
					}
				}// if brick
				else if (dynamic_cast<CQuestionBrick*>(e->obj))
				{
					CQuestionBrick *questionbrick = dynamic_cast<CQuestionBrick*>(e->obj);
					vx = -vx;
					if(level == GOOMBA_LEVEL_FLY)
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

	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_DIE:
		y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
		enable = false;
		vx = 0;
		vy = 0;
		StartRenderDie();
		break;
	case GOOMBA_STATE_DIE_KNOCKUP:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		//vx = 0;
		//enable = false;
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
				Render_Tail_Hit();
				LevelDown();
				SetState(GOOMBA_STATE_DIE_KNOCKUP);
				y += dy;
				mario->GainScore(SCORE_100);
				mario->RenderPoint(EFFECT_TYPE_100_POINT);
			}	
	
}

void CGoomba::LevelDown()
{
	if (level == GOOMBA_LEVEL_FLY)
		level = GOOMBA_LEVEL_NORMAL;
}

void CGoomba::Reset()
{
	CGame *game = CGame::GetInstance();
	float bb_left, bb_top;
	GetPosition(bb_left, bb_top);
	if (!isInCamera() && !game->BBisinCamera(start_x - HALF_EXTRA_SPACE,start_y,start_x+SCREEN_WIDTH+ HALF_EXTRA_SPACE, start_y+GOOMBA_BBOX_HEIGHT))
	{
		enable = true;
		visable = true;
		SetPosition(start_x, start_y);
		setLevel(start_level);
		SetState(GOOMBA_STATE_WALKING);
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

	tailhiteffect->AddtoGrid();
}

void CGoomba::StartRenderDie()
{
	isrenderdie = true;
	renderdie_start = GetTickCount64();
}
