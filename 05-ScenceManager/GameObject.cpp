#include <d3dx9.h>
#include <algorithm>


#include "Utils.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"
#include "PlayScence.h"
#include "define.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	ax = 0;
	nx = 1;	

	start_x = x;
	start_y = y;
}

void CGameObject::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	this->dt = dt;
	dx = vx*dt;
	dy = vy*dt;
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CGameObject::SweptAABBEx(LPGAMEOBJECT coO)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);

	// deal with moving object: m speed = original m speed - collide object speed
	float svx, svy;
	coO->GetSpeed(svx, svy);

	float sdx = svx*dt;
	float sdy = svy*dt;

	// (rdx, rdy) is RELATIVE movement distance/velocity 
	float rdx = this->dx - sdx;
	float rdy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);

	CGame::SweptAABB(
		ml, mt, mr, mb,
		rdx, rdy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent * e = new CCollisionEvent(t, nx, ny, rdx, rdy, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects 
	
	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CGameObject::CalcPotentialCollisions(
	vector<LPGAMEOBJECT> *coObjects, 
	vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
		{

			float mleft, mtop, mright, mbottom;
			GetBoundingBox(mleft, mtop, mright, mbottom);
			float oleft, otop, obottom, oright;
			e->obj->GetBoundingBox(oleft, otop, oright, obottom);
			if (e->nx != 0)
			{
				if (ceil(mbottom) == otop) //fix bug : go through wall and stuff
					continue;
			}
			coEvents.push_back(e);
		}
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CGameObject::FilterCollision(
	vector<LPCOLLISIONEVENT> &coEvents,
	vector<LPCOLLISIONEVENT> &coEventsResult,
	float &min_tx, float &min_ty, 
	float &nx, float &ny, float &rdx, float &rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty  && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
	}

	if (min_ix>=0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy>=0) coEventsResult.push_back(coEvents[min_iy]);
}


void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPDIRECT3DTEXTURE9 bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l,t,r,b; 

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	CGame::GetInstance()->Draw(l, t, bbox, rect.left, rect.top, rect.right, rect.bottom, 100); //32 // change (x, y... ==> (l, t...
}

bool CGameObject::SpecialCollision(float friend_left, float friend_top, float friend_right, float friend_bottom)
{
	float this_left, this_top, this_right, this_bottom;

	GetBoundingBox(
		this_left,
		this_top,
		this_right,
		this_bottom);
	if (friend_left <= this_right && friend_right >= this_left
		&& friend_top <= this_bottom && friend_bottom >= this_top)
		return true;
	return false; 
}


bool CGameObject::isInCamera()
{
	if (!visable)
		return false;
	float cam_left, cam_top;
	float bb_left, bb_top, bb_right, bb_bottom;
	float left, top, right, bottom;
	CGame* game = CGame::GetInstance();

	game->GetCamPos(cam_left, cam_top);
	float cam_right = cam_left + SCREEN_WIDTH;
	float cam_bottom = cam_top + SCREEN_HEIGHT;
	GetBoundingBox(bb_left, bb_top, bb_right, bb_bottom);
	left = x;
	right = x + (bb_right - bb_left);
	top = y;
	bottom = y + (bb_bottom - bb_top);
	if (right > cam_left && left < cam_right && top < cam_bottom && bottom > cam_top)
		return true;
	return false;
}

CGameObject::~CGameObject()
{

}
void CGameObject::AddtoGrid() {
	CPlayScene *scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	Grid* grid = scene->getGrid();
	Unit* unit = new Unit(grid, this->x, this->y, this);
	grid->Add(unit);
}