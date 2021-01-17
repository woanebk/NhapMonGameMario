#include "PiranhaPlant.h"
#include "PlayScence.h"
#include "Mario.h"
#include "Effect.h"
#include "Utils.h"
#include "EnemyFireBall.h"


void CPiranhaPlant::GetBoundingBox(float & left, float & top, float & right, float & bottom)
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
	right = x + PLANT_PIRANHA_BBOX_WIDTH;
	bottom = y + PLANT_PIRANHA_BBOX_HEIGHT;
}

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (!enable)
		return;
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	DetectMario();
	HitByTail();

	if (is_up)
	{
		if (level == PLANT_LEVEL_PIRANHA)
			vy = -PLANT_PIRANHA_SPEED_Y;
		else
			vy = -PLANT_FIRE_SPEED_Y;
	}
	else if (is_down)
	{
		if (level == PLANT_LEVEL_PIRANHA)
			vy = PLANT_PIRANHA_SPEED_Y;
		else
			vy = PLANT_FIRE_SPEED_Y;
	}
	else if (is_stand)
		vy = 0;
	
	//
	if (level == PLANT_LEVEL_PIRANHA)
	{
		if (is_up && y <= start_y - PLANT_PIRANHA_BBOX_HEIGHT)
		{
			is_up = false;
			StandStill();
		}
		if (is_down && y>= start_y + PLANT_PIRANHA_BBOX_HEIGHT)
		{
			is_down = false;
			StartUp();
		}
		if (is_stand && GetTickCount64() - stand_start >= PLANT_STAND_TIME)
		{
			is_stand = false;
			stand_start = 0;
			StartDown();
		}
	}
	else// level fire
	{
		if (is_up && y <= start_y - PLANT_PIRANHA_BBOX_HEIGHT)
		{
			is_up = false;
			StandStill();
			StartCalculateShot();
		}
		if (is_down && y >= start_y + PLANT_PIRANHA_BBOX_HEIGHT)
		{
			is_down = false;
			StartUp();
		}
		if (is_stand && GetTickCount64() - stand_start >= PLANT_STAND_TIME)
		{
			is_stand = false;
			stand_start = 0;
			StartDown();
		}
		if (is_stand && can_shot &&GetTickCount64() - shot_start > PLANT_SHOT_TIME)
		{
			Shot();
			can_shot = false;
			shot_start = 0;
		}
	}
	

	if (coEvents.size() == 0)
	{
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e != NULL)
			{
				y += dy;
			}
		}

	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CPiranhaPlant::Render()
{
	int ani = 0;
	if(level == PLANT_LEVEL_PIRANHA)
		ani = PLANT_ANI_PIRANHA_GREEN;
	else
	{
		if (type == PLANT_TYPE_GREEN)
		{
			if (nx < 0 && ny < 0)
				if(can_shot)
					ani = PLANT_ANI_FIRE_GREEN_LEFT_UP_OPEN;
				else
					ani = PLANT_ANI_FIRE_GREEN_LEFT_UP_CLOSE;
			if (nx < 0 && ny>0)
				if(can_shot)
					ani = PLANT_ANI_FIRE_GREEN_LEFT_DOWN_OPEN;
				else
					ani= PLANT_ANI_FIRE_GREEN_LEFT_DOWN_CLOSE;
			if (nx > 0 && ny < 0)
				if(can_shot)
					ani = PLANT_ANI_FIRE_GREEN_RIGHT_UP_OPEN;
				else
					ani = PLANT_ANI_FIRE_GREEN_RIGHT_UP_CLOSE;
			if (nx > 0 && ny > 0)
				if (can_shot)
					ani = PLANT_ANI_FIRE_GREEN_RIGHT_DOWN_OPEN;
				else
					ani = PLANT_ANI_FIRE_GREEN_RIGHT_DOWN_CLOSE;
		}
		else
		{
			if (nx < 0 && ny < 0)
				if (can_shot)
					ani = PLANT_ANI_FIRE_RED_LEFT_UP_OPEN;
				else
					ani = PLANT_ANI_FIRE_RED_LEFT_UP_CLOSE;
			if (nx < 0 && ny>0)
				if (can_shot)
					ani = PLANT_ANI_FIRE_RED_LEFT_DOWN_OPEN;
				else
					ani = PLANT_ANI_FIRE_RED_LEFT_DOWN_CLOSE;
			if (nx > 0 && ny < 0)
				if (can_shot)
					ani = PLANT_ANI_FIRE_RED_RIGHT_UP_OPEN;
				else
					ani = PLANT_ANI_FIRE_RED_RIGHT_UP_CLOSE;
			if (nx > 0 && ny > 0)
				if (can_shot)
					ani = PLANT_ANI_FIRE_RED_RIGHT_DOWN_OPEN;
				else
					ani = PLANT_ANI_FIRE_RED_RIGHT_DOWN_CLOSE;
		}
	}
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

CPiranhaPlant::CPiranhaPlant(int t, int l)
{
	type = t;
	level = l;
	is_up = true;
}


CPiranhaPlant::~CPiranhaPlant()
{
}
void CPiranhaPlant::HitByTail()
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
				Killed();
			}

}

void CPiranhaPlant::Render_Tail_Hit()
{
	CEffect *tailhiteffect = new CEffect(EFFECT_TYPE_TAIL_HIT);
	tailhiteffect->SetPosition(this->x, this->y - PLANT_PIRANHA_BBOX_HEIGHT / 2);
	tailhiteffect->SetStartPosition(this->x, this->y - PLANT_PIRANHA_BBOX_HEIGHT / 2);

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(EFFECT_SET_ID);
	tailhiteffect->SetAnimationSet(ani_set);

	CPlayScene *currenscence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	currenscence->PushBackObject(tailhiteffect);
}
void CPiranhaPlant::DetectMario() 
{
	CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = scence->GetPlayer();
	if (mario == NULL)
		return;
	float mario_bb_left, mario_bb_top, mario_bb_right, mario_bb_bottom;
	mario->GetBoundingBox(mario_bb_left, mario_bb_top, mario_bb_right, mario_bb_bottom);
	float bb_left, bb_top, bb_right, bb_bottom;
	GetBoundingBox(bb_left, bb_top, bb_right, bb_bottom);
	if (mario_bb_left > bb_right)
		nx = 1;
	else
		nx = -1;
	if (mario_bb_top < bb_top)
		ny = -1;
	else
		ny = 1;
}

void CPiranhaPlant::Shot()
{
	CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CEnemyFireBall *fireball = new CEnemyFireBall(x + PLANT_PIRANHA_BBOX_WIDTH / 2, y, nx, ny);
	scence->PushBackObject(fireball);

}

void CPiranhaPlant::Killed()
{
	CPlayScene* scence = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = scence->GetPlayer();
	Render_Tail_Hit();
	enable = false;
	visable = false;
	mario->GainScore(SCORE_100);
	mario->RenderPoint(EFFECT_TYPE_100_POINT);
}