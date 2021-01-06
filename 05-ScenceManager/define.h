#pragma once
#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"SAMPLE 05 - SCENCE MANAGER"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define MAX_FRAME_RATE 120

//=======================================
#define WORLD_1_1_WIDTH 2800
#define WORLD_1_1_HEIGHT 416
#define WORLD_1_1_TILE_COLUMNS 176
#define WORLD_1_1_TILE_ROWS 27
#define WORLD_1_1_MAP_TILESET_COLUMNS 11
#define WORLD_1_1_MAP_TILESET_ROWS 12
#define TILE_WIDTH 16
#define TILE_HEIGHT 16

#define WORLD_1_1_TILESET L"map1-1_bank.png"
#define WORLD_1_1_MATRIX_TXT L"map1-1.txt"
//MARIO
#define MARIO_WALKING_SPEED		0.1f 
#define MARIO_LEAF_FLAPPING_SPEED	0.05f
//0.1f
#define MARIO_JUMP_SPEED_Y			0.2f
#define MARIO_JUMP_SPEED_Y_MAX		0.70f
#define MARIO_JUMP_DEFLECT_SPEED	0.20f
#define MARIO_GRAVITY				0.002f
#define MARIO_DIE_DEFLECT_SPEED		0.50f
#define MARIO_MIN_WALK_SPEED		0.05f
#define MARIO_MAX_WALK_SPEED		0.15f
#define MARIO_ACCELERATION			0.0003f
#define MARIO_EACH_STACK_SPEED		0.03f
#define MARIO_MAX_RUN_SPEED			0.20f

#define MARIO_RUNNING_STACK_MAX		7
#define MARIO_WALKING_STACK_MAX		4
#define MARIO_STACKUP_TIME		250
#define MARIO_STACKDOWN_TIME	500

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_SIT				1
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	101
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
#define MARIO_STATE_SPIN			200

#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_SMALL_IDLE_RIGHT		2
#define MARIO_ANI_SMALL_IDLE_LEFT			3

#define MARIO_ANI_BIG_WALKING_RIGHT			4
#define MARIO_ANI_BIG_WALKING_LEFT			5
#define MARIO_ANI_SMALL_WALKING_RIGHT		6
#define MARIO_ANI_SMALL_WALKING_LEFT		7

#define MARIO_ANI_DIE				8
#define MARIO_ANI_BIG_SIT_LEFT				9
#define MARIO_ANI_BIG_SIT_RIGHT				10
#define MARIO_ANI_SMALL_JUMP_LEFT	11
#define MARIO_ANI_SMALL_JUMP_RIGHT	12
#define MARIO_ANI_BIG_JUMP_LEFT		13
#define MARIO_ANI_BIG_JUMP_RIGHT	14

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27
//LEAF MARIO
#define MARIO_ANI_LEAF_IDLE_LEFT	15
#define MARIO_ANI_LEAF_IDLE_RIGHT	16
#define MARIO_ANI_LEAF_WALK_LEFT	17
#define MARIO_ANI_LEAF_WALK_RIGHT	18
#define MARIO_ANI_LEAF_JUMP_LEFT_LOW	19
#define MARIO_ANI_LEAF_JUMP_RIGHT_LOW	20
#define MARIO_ANI_LEAF_SIT_LEFT		21
#define MARIO_ANI_LEAF_SIT_RIGHT	22

//FIRE MARIO
#define MARIO_ANI_FIRE_IDLE_LEFT	23
#define MARIO_ANI_FIRE_IDLE_RIGHT	24
#define MARIO_ANI_FIRE_WALK_LEFT	25
#define MARIO_ANI_FIRE_WALK_RIGHT	26
#define MARIO_ANI_FIRE_JUMP_LEFT_LOW	27
#define MARIO_ANI_FIRE_JUMP_RIGHT_LOW	28
#define MARIO_ANI_FIRE_SIT_LEFT		29
#define MARIO_ANI_FIRE_SIT_RIGHT	30
#define MARIO_ANI_LEAF_SPIN_RIGHT	31
#define MARIO_ANI_LEAF_SPIN_LEFT	32
#define MARIO_ANI_SMALL_HOLD_LEFT	33
#define MARIO_ANI_SMALL_HOLD_RIGHT	34
#define MARIO_ANI_BIG_HOLD_LEFT		35
#define MARIO_ANI_BIG_HOLD_RIGHT	36
#define MARIO_ANI_LEAF_HOLD_LEFT	37
#define MARIO_ANI_LEAF_HOLD_RIGHT	38
#define MARIO_ANI_FIRE_HOLD_LEFT	39
#define MARIO_ANI_FIRE_HOLD_RIGHT	40
#define MARIO_ANI_LEAF_FLAP_LEFT	41
#define MARIO_ANI_LEAF_FLAP_RIGHT	42
#define MARIO_ANI_SMALL_KICK_LEFT	43
#define MARIO_ANI_SMALL_KICK_RIGHT	44
#define MARIO_ANI_BIG_KICK_LEFT		45
#define MARIO_ANI_BIG_KICK_RIGHT	46
#define MARIO_ANI_LEAF_KICK_LEFT	47
#define MARIO_ANI_LEAF_KICK_RIGHT	48
#define MARIO_ANI_FIRE_KICK_LEFT	49
#define MARIO_ANI_FIRE_KICK_RIGHT	50
#define MARIO_ANI_SMALL_BRAKE_LEFT	51
#define MARIO_ANI_SMALL_BRAKE_RIGHT	52
#define MARIO_ANI_BIG_BRAKE_LEFT	53
#define MARIO_ANI_BIG_BRAKE_RIGHT	54
#define MARIO_ANI_FIRE_BRAKE_LEFT	55
#define MARIO_ANI_FIRE_BRAKE_RIGHT	56
#define MARIO_ANI_LEAF_BRAKE_LEFT	57
#define MARIO_ANI_LEAF_BRAKE_RIGHT	58
#define MARIO_ANI_BIG_WALK_FAST_RIGHT	59
#define MARIO_ANI_BIG_WALK_FAST_LEFT	60
#define MARIO_ANI_SMALL_WALK_FAST_RIGHT	61
#define MARIO_ANI_SMALL_WALK_FAST_LEFT	62
#define MARIO_ANI_LEAF_WALK_FAST_LEFT	63
#define MARIO_ANI_LEAF_WALK_FAST_RIGHT	64
#define MARIO_ANI_FIRE_WALK_FAST_RIGHT	65
#define MARIO_ANI_FIRE_WALK_FAST_LEFT	66
#define MARIO_ANI_SMALL_RUN_LEFT		67
#define MARIO_ANI_SMALL_RUN_RIGHT		68
#define MARIO_ANI_BIG_RUN_LEFT			69
#define MARIO_ANI_BIG_RUN_RIGHT			70
#define MARIO_ANI_LEAF_RUN_LEFT			71
#define MARIO_ANI_LEAF_RUN_RIGHT		72
#define MARIO_ANI_FIRE_RUN_LEFT			73
#define MARIO_ANI_FIRE_RUN_RIGHT		74


#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_LEAF	3
#define MARIO_LEVEL_FIRE	4



#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

//LEAF MARIO BBOX
#define MARIO_LEAF_BBOX_WIDTH	14
#define MARIO_LEAF_BBOX_HEIGHT	27
#define MARIO_BBOX_LEAF_SIT_WIDTH	22
#define MARIO_BBOX_LEAF_SIT_HEIGHT	18

#define MARIO_BBOX_SIT_WIDTH 14
#define MARIO_BBOX_SIT_HEIGHT 18
//#define MARIO_LEAF_BBOX_SPIN_WIDTH 25
#define MARIO_LEAF_BBOX_TAIL_WIDTH 9
#define MARIO_LEAF_BBOX_TAILDOWN_WIDTH 7

#define MARIO_LEAF_ISSPINNING_BBOX_HEIGHT	13.5

//FIRE MARIO BBOX
#define MARIO_FIRE_BBOX_WIDTH	14
#define MARIO_FIRE_BBOX_HEIGHT	27

#define MARIO_FIRE_BBOX_SIT_WIDTH	14
#define MARIO_FIRE_BBOX_SIT_HEIGHT	18


#define MARIO_UNTOUCHABLE_TIME 5000



//GOOMBA
#define GOOMBA_LEVEL_NORMAL 1
#define GOOMBA_LEVEL_FLY	2

#define GOOMBA_FLY_GRAVITY	0.0015f
#define GOOMBA_WALKING_SPEED 0.05f //0.05
#define FLYGOOMBA_LOW_JUMP_SPEED	0.15f
#define FLYGOOMBA_HIGH_JUMP_SPEED	0.35f

#define FLYGOOMBA_LOW_JUMP_TIME			800
#define FLYGOOMBA_HIGH_JUMP_TIME		1200

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 9
#define FLYGOOMBA_BBOX_HEIGHT 24

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200

#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1
#define GOOMBA_ANI_FLAY_RED	2

//KOOPAS
#define KOOPAS_FLY_GRAVITY	0.001f

#define KOOPAS_LEVEL_NORMAL			1
#define KOOPAS_LEVEL_FLY			2
#define KOOPAS_WALKING_SPEED		0.03f
#define KOOPAS_SPINNING_SPEED		0.25f
#define KOOPAS_FLY_SPEED			0.25f

#define KOOPAS_BBOX_WIDTH			16
#define KOOPAS_BBOX_HEIGHT			26
#define KOOPAS_BBOX_HEIGHT_DIE		16
#define KOOPAS_BBOX_HEIGHT_SHELL	15

#define FLYKOOPAS_BBOX_HEIGHT		28

#define KOOPAS_STATE_WALKING		100
#define KOOPAS_STATE_DIE			200
#define KOOPAS_STATE_SHELL			300
#define KOOPAS_STATE_SPIN_LEFT		400
#define KOOPAS_STATE_SPIN_RIGHT		401

#define KOOPAS_ANI_WALKING_LEFT 0
#define KOOPAS_ANI_WALKING_RIGHT 1
#define KOOPAS_ANI_DIE 2
#define KOOPAS_ANI_SHELL 3
#define KOOPAS_ANI_SPIN 4
#define FLYKOOPAS_ANI_FLY_LEFT	5
#define FLYKOOPAS_ANI_FLY_RIGHT	6
//BLOCK

#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

//BRICK
#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_TYPE_QUESTION 1
#define BRICK_TYPE_GROUND_1 2
#define BRICK_TYPE_GROUND_2 3
#define BRICK_TYPE_WOOD 4
#define BRICK_TYPE_CLOUD 5

#define BRICK_ANI_WOOD					0
#define BRICK_ANI_QUESTION				1
#define BRICK_ANI_GROUND_1				2
#define BRICK_ANI_GROUND_2				3
#define BRICK_ANI_CLOUD					4
#define BRICK_ANI_BREAKABLE				5
#define BRICK_ANI_EMPTY_QUESTION		6

//FIREBALL
#define FIREBALL_BBOX_WIDTH 9
#define FIREBALL_BBOX_HEIGHT 9
#define FIREBALL_SPEED_X 0.15f
#define FIREBALL_SPEED_Y 0.1f
#define FIREBALL_REVERSE_SPEED_TIME 100

//LEAF
#define LEAF_BBOX_WIDTH			16
#define LEAF_BBOX_HEIGHT		14

#define LEAF_GRAVITY			0.0015f
#define LEAF_SPEED_x			0.03f
#define LEAF_JUMP_SPEED			0.15f
#define LEAF_TURN_AROUND_TIME	1000

#define LEAF_ANI_GO_LEFT		0
#define LEAF_ANI_GO_RIGHT		1

#define ITEM_MUSHROOM			1
#define ITEM_LEAF				2
//OBJECT TYPE
#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_BLOCK	4
#define OBJECT_TYPE_COIN	5
#define OBJECT_TYPE_PINE	6
#define OBJECT_TYPE_FIREBALL 7
#define OBJECT_TYPE_BREAKABLE_BRICK	8
#define OBJECT_TYPE_QUESTION_BRICK	9
#define OBJECT_TYPE_LEAF			10

#define OBJECT_TYPE_PORTAL	50
//ANIMATION SET ID
#define MARIO_SET_ID			1
#define BRICK_SET_ID			2
#define GOOMBA_SET_ID			3
#define KOOPAS_SET_ID			4
#define COIN_SET_ID				5
#define FIREBALL_SET_ID			6
#define ITEM_SET_ID				7