#pragma once
#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"SAMPLE 05 - SCENCE MANAGER"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define MAX_FRAME_RATE 120

//=======================================
#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP_BACKGROUND	7
#define SCENE_SECTION_MAP_BARRIERS		8



#define MAX_SCENE_LINE 1024

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

#define WORLD_1_1_SECRECT_TILESET			L"secrectmap_1-1_bank.png"
#define WORLD_1_1_SECRECT_MATRIX_TXT		L"secrect_map1-1_matrix.txt"

#define WORLDMAP_1_TILESET		L"worldmap_1_bank.png"
#define WORLDMAP_1_MATRIX_TXT	L"worldmap_1_matrix.txt"

#define WORLDMAP_1_WIDTH			240
#define WORLDMAP_1_HEIGHT			176
#define WORLDMAP_1_TILE_COLUMNS		15
#define WORLDMAP_1_TILE_ROWS		11
#define WORLDMAP_1_TILESET_COLUMNS	12
#define WORLDMAP_1_TILESET_ROWS		11


#define WORLD_1_1_SCENCE_ID					1
#define WORLD_1_1_SECRECT_SCENCE_ID			2
#define WORLDMAP_1_SCENCE_ID				3

#define WORLDMAP_1_START_X					24
#define WORLDMAP_1_START_Y					45

#define WORLD_1_1_START_X					17
#define WORLD_1_1_START_Y					350
#define WORLD_1_1_SECRECT_START_X			144
#define WORLD_1_1_SECRECT_START_Y			35

#define WORLD_1_1_SWITCHBACK_START_X			2326
#define WORLD_1_1_SWITCHBACK_START_Y			345

#define WORLD_1_1_ONMAP_X					56
#define WORLD_1_1_ONMAP_Y					13

#define WORLD_1_1_TILESET_ID	1
#define WORLD_1_1_MATRIX_ID		1
#define WORLD_1_1_SECRECT_TILESET_ID	2
#define WORLD_1_1_SECRECT_MATRIX_ID		2
#define WORLDMAP_1_TILESET_ID	3
#define WORLDMAP_1_MATRIX_ID		3

//INTRO SCENCE
#define INTRO_SCENE_ID	999
#define INTRO_SCENE_TILESET_ID	999
#define INTRO_SCENE_MATRIX_ID	999

#define INTRO_SCENE_TILESET		L"Intro_scence_bank.png"
#define INTRO_SCENE_MATRIX_TXT	L"Intro_scence_matrix.txt"

#define ANI_NUMBER_3_ID			1400
#define ANI_NUMBER_3_X			112
#define ANI_NUMBER_3_Y			102

#define SELECTOR_SPRITE_ID		21004

#define SELECT_1_PLAYER_X		70
#define SELECT_1_PLAYER_Y		150
#define SELECT_2_PLAYER_X		70
#define SELECT_2_PLAYER_Y		168

//GENERAL
#define EXTRA_RESET_SPACE		320
#define HALF_EXTRA_SPACE		100
//MARIO
#define MARIO_START_LIFE			4

#define MARIO_WALKING_SPEED		0.1f 
#define MARIO_LEAF_FLAPPING_SPEED	0.05f
//0.1f
#define MARIO_JUMP_SPEED_Y_MIN		0.15f
#define MARIO_JUMP_SPEED_Y_MAX		0.33f
#define MARIO_JUMP_DEFLECT_SPEED	0.25f //0.20
#define MARIO_LEAF_JUMP_DEFLECT_SPEED	0.30f
#define MARIO_GRAVITY				0.0015f //0.002f
#define MARIO_DIE_DEFLECT_SPEED		0.50f
#define MARIO_MIN_WALK_SPEED		0.05f
#define MARIO_MAX_WALK_SPEED		0.15f
#define MARIO_ACCELERATION			0.0003f
#define MARIO_EACH_STACK_SPEED		0.03f
#define MARIO_MAX_RUN_SPEED			0.20f
#define MARIO_FLY_SPEED_MAX			0.

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
#define MARIO_STATE_PINEDOWN		350
#define MARIO_STATE_PINEUP			351
#define MARIO_STATE_ICON					2
#define MARIO_STATE_ICON_GO_LEFT			3
#define MARIO_STATE_ICON_GO_RIGHT			4
#define MARIO_STATE_ICON_GO_UP				5
#define MARIO_STATE_ICON_GO_DOWN			6

#define MARIO_ICON_SPEED_DX				32
#define MARIO_ICON_SPEED				0.05f

#define MARIO_PINE_DOWN_SPEED			0.03f
#define MARIO_PINEDOWN_TIME				1000

#define MARIO_PINE_UP_SPEED				0.03f
#define MARIO_PINEUP_TIME				1000

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
#define MARIO_ANI_TRANSFORM_SMALL_TO_BIG_LEFT	75
#define MARIO_ANI_TRANSFORM_SMALL_TO_BIG_RIGHT	76
#define MARIO_ANI_TRANSFORM_TO_LEAF				77
#define MARIO_ANI_ICON							78
#define MARIO_ANI_LEAF_FLY_LEFT					79
#define MARIO_ANI_LEAF_FLY_RIGHT				80
#define MARIO_ANI_LEAF_PINEDOWN					81


#define TRANSFORM_TO_BIG_MARIO_TIME				1000
#define TRANSFORM_TO_LEAF_MARIO_TIME			500

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

#define GOOMBA_TYPE_NORMAL	1
#define GOOMBA_TYPE_RED		2

#define GOOMBA_FLY_GRAVITY	0.0015f
#define GOOMBA_WALKING_SPEED 0.05f //0.05
#define FLYGOOMBA_LOW_JUMP_SPEED	0.15f
#define FLYGOOMBA_HIGH_JUMP_SPEED	0.35f

#define GOOMBA_DIE_TIME					200
#define FLYGOOMBA_LOW_JUMP_TIME			1500
#define FLYGOOMBA_HIGH_JUMP_TIME		1200

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 9
#define FLYGOOMBA_BBOX_HEIGHT 21

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_DIE_KNOCKUP	201

#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1
#define GOOMBA_ANI_FLY_RED	2
#define GOOMBA_ANI_LV2_WAlKING	3
#define GOOMBA_ANI_LV2_DIE	4
#define GOOMBA_ANI_LV1_DIE_KNOCKUP	5
#define GOOMBA_ANI_LV2_DIE_KNOCKUP	6

//KOOPAS
#define KOOPAS_FLY_GRAVITY	0.001f

#define KOOPAS_LEVEL_NORMAL			1
#define KOOPAS_LEVEL_FLY			2
#define KOOPAS_WALKING_SPEED		0.03f
#define KOOPAS_SPINNING_SPEED		0.15f
#define KOOPAS_FLY_SPEED			0.25f

#define KOOPAS_BBOX_WIDTH			15
#define KOOPAS_BBOX_HEIGHT			26
#define KOOPAS_BBOX_HEIGHT_DIE		16
#define KOOPAS_BBOX_HEIGHT_SHELL	15

#define KOOPAS_TYPE_GREEN			1
#define KOOPAS_TYPE_RED				2

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
#define KOOPAS_ANI_SHELL_UP		7
#define KOOPAS_ANI_SPIN_UP		8
#define KOOPAS_ANI_WALK_LEFT_RED	9
#define KOOPAS_ANI_WALK_RIGHT_RED	10
#define KOOPAS_ANI_SHELL_RED		11
#define KOOPAS_ANI_SPIN_RED			12
#define KOOPAS_ANI_SHELL_UP_RED		13
#define KOOPAS_ANI_SPIN_UP_RED		14
//BLOCK

#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_JUMP_SPEED_Y	0.3f
#define COIN_JUMP_TIME		150
#define COIN_JUMP_HEIGHT	20


//PINE
#define PINE_ANI_TO_SECRECT				0
#define PINE_ANI_SWITCH_BACK			1
#define PINE_ANI_1ST_1_1				2
#define PINE_ANI_2ND_1_1				3
#define PINE_ANI_3RD_1_1				4
//BRICK
#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_TYPE_QUESTION 1
#define BRICK_TYPE_GROUND_1 2
#define BRICK_TYPE_GROUND_2 3
#define BRICK_TYPE_WOOD 4
#define BRICK_TYPE_CLOUD 5
#define BRICK_TYPE_INVISIBLE	6
#define BRICK_TYPE_MONEYBUTTON	7
#define BRICK_TYPE_BASEMENT		8
#define BRICK_TYPE_TREE_ICON	9
#define BRICK_TYPE_HELP_ICON	10

#define BRICK_ANI_WOOD					0
#define BRICK_ANI_QUESTION				1
#define BRICK_ANI_GROUND_1				2
#define BRICK_ANI_GROUND_2				3
#define BRICK_ANI_CLOUD					4
#define BRICK_ANI_BREAKABLE				5
#define BRICK_ANI_EMPTY_QUESTION		6
#define BRICK_ANI_MONEY_BUTTON			7
#define BRICK_ANI_MONEY_BUTTON_PRESSED	8
#define BRICK_ANI_BASEMENT				9
#define BRICK_ANI_TREE_ICON				10
#define BRICK_ANI_HELP_ICON				11


#define CANBOUNCE		1
#define CANNOTBOUNCE	0

#define BRICK_HELP_ICON_BLINK_TIME		400
#define BRICK_QUESTION_JUMP_SPEED		0.05f
#define BRICK_QUESTION_JUMP_TIME		100
//QUESTION BREAK
#define QUESTION_BRICK_TYPE_NORMAL						1
#define QUESTION_BRICK_TYPE_ONSKY_BREAKABLE_ALIKE		2
#define QUESTION_BRICK_TYPE_MONEY_BUTTON_CREATOR		3
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
#define LEAF_JUMP_TIME			200
#define LEAF_TURN_AROUND_TIME	1000


#define ITEM_MUSHROOM_RED		1
#define ITEM_MUSHROOM_GREEN		2
#define ITEM_LEAF				3
#define ITEM_MONEY_BUTTON		4
#define ITEM_MONEY				5


//MUSHROOM
#define MUSHROOM_TYPE_RED		1
#define MUSHROOM_TYPE_GREEN		2

#define MUSHROOM_BBOX_WIDTH		16
#define MUSHROOM_BBOX_HEIGHT	16

#define MUSHROOM_SPEED_X		0.04f
#define MUSHROOM_GRAVITY		0.02f

//ITEM ANIMATION
#define LEAF_ANI_GO_LEFT		0
#define LEAF_ANI_GO_RIGHT		1
#define MUSHROOM_ANI_GREEN		3
#define MUSHROOM_ANI_RED		2

//EFFECT ANIMATION
#define ANI_HIT_TAIL			0
#define ANI_BROKEN_PIECE		1
#define ANI_100_POINT			2
#define ANI_200_POINT			3
#define ANI_400_POINT			4
#define ANI_800_POINT			5
#define ANI_1000_POINT			6
#define ANI_2000_POINT			7
#define ANI_4000_POINT			8
#define ANI_8000_POINT			9
#define ANI_1_UP				10

#define EFFECT_HIT_TAIL_TIME	200
#define EFFECT_POINT_TIME		600

//CEffect TYPE:
#define EFFECT_TYPE_TAIL_HIT	1
#define EFFECT_TYPE_ENEMY_DIE		2
#define EFFECT_TYPE_100_POINT		3
#define EFFECT_TYPE_200_POINT		4
#define EFFECT_TYPE_400_POINT		5
#define EFFECT_TYPE_800_POINT		6
#define EFFECT_TYPE_1000_POINT		7
#define EFFECT_TYPE_2000_POINT		8
#define EFFECT_TYPE_4000_POINT		9
#define EFFECT_TYPE_8000_POINT		10
#define EFFECT_TYPE_1_UP			11
//BROKEN PIECES
#define BROKEN_PIECE_SPEED_X	0.05f
#define BROKEN_PIECE_SPEED_Y	0.05f
#define BROKEN_PIECE_FLYING_TIME		400
#define BROKEN_PIECE_GRAVITY			0.003f
#define COIN_CHANGE_TO_BRICK_TIME		13000

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
#define OBJECT_TYPE_MUSHROOM		11
#define OBJECT_TYPE_PLANT_PIRANHA		12

#define OBJECT_TYPE_PORTAL	50
//ANIMATION SET ID
#define MARIO_SET_ID			1
#define BRICK_SET_ID			2
#define GOOMBA_SET_ID			3
#define KOOPAS_SET_ID			4
#define COIN_SET_ID				5
#define FIREBALL_SET_ID			6
#define ITEM_SET_ID				7
#define EFFECT_SET_ID			8
#define PINE_SET_ID				9
#define PLANT_SET_ID			10

//PLANT PIRANHA
#define PLANT_ANI_PIRANHA_GREEN								0
#define PLANT_ANI_FIRE_GREEN_LEFT_DOWN_OPEN					1
#define PLANT_ANI_FIRE_GREEN_LEFT_DOWN_CLOSE				2
#define PLANT_ANI_FIRE_GREEN_LEFT_UP_OPEN					3
#define PLANT_ANI_FIRE_GREEN_LEFT_UP_CLOSE					4
#define PLANT_ANI_FIRE_GREEN_RIGHT_DOWN_OPEN				5
#define PLANT_ANI_FIRE_GREEN_RIGHT_DOWN_CLOSE				6
#define PLANT_ANI_FIRE_GREEN_RIGHT_UP_OPEN					7
#define PLANT_ANI_FIRE_GREEN_RIGHT_UP_CLOSE					8
#define PLANT_ANI_FIRE_RED_LEFT_DOWN_OPEN					9
#define PLANT_ANI_FIRE_RED_LEFT_DOWN_CLOSE					10
#define PLANT_ANI_FIRE_RED_LEFT_UP_OPEN						11
#define PLANT_ANI_FIRE_RED_LEFT_UP_CLOSE					12
#define PLANT_ANI_FIRE_RED_RIGHT_DOWN_OPEN					13
#define PLANT_ANI_FIRE_RED_RIGHT_DOWN_CLOSE					14
#define PLANT_ANI_FIRE_RED_RIGHT_UP_OPEN					15
#define PLANT_ANI_FIRE_RED_RIGHT_UP_CLOSE					16

#define PLANT_PIRANHA_BBOX_WIDTH			16
#define PLANT_PIRANHA_BBOX_HEIGHT			24
#define PLANT_FIRE_BBOX_HEIGHT			32

#define PLANT_TYPE_GREEN		1
#define PLANT_TYPE_RED			2

#define PLANT_LEVEL_PIRANHA		1
#define PLANT_LEVEL_FIRE		2

#define PLANT_PIRANHA_STATE_UP			1
#define PLANT_PIRANHA_STATE_DOWN		2

#define PLANT_PIRANHA_SPEED_Y		0.03f
#define PLANT_FIRE_SPEED_Y			0.02f
#define PLANT_UP_TIME				1500
#define PLANT_DOWN_TIME				1500
#define PLANT_STAND_TIME				1000
#define PLANT_SHOT_TIME					700
#define ENEMY_FIREBALL_SPEED_X			0.04
#define ENEMY_FIREBALL_SPEED_Y			0.02

#define PLANT_UP_TIME_FIRE				2000
#define PLANT_DOWN_TIME_FIRE				2000
//HUD
#define HUD_HEIGHT				40
#define HUD_WIDTH				310
#define GAME_LIMIT_TIME			300

#define SCORE_100			100
#define SCORE_200			200
#define SCORE_400			400
#define SCORE_800			800
#define SCORE_1000			1000

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

#define MARIO_SCORE_NUMBERS		7
#define STACK_SPRITE_0_X		87
#define STACK_SPRITE_0_Y		13
#define STACK_SPRITE_WIDTH		8

#define LIFE_SPRITE_UNITS_NUMBER_X		73
#define LIFE_SPRITE_UNITS_NUMBER_Y		21
#define LIFE_SPRITE_TENS_NUMBER_X		65
#define LIFE_SPRITE_TENS_NUMBER_Y		21

#define MONEY_SPRITE_UNITS_NUMBER_X		176
#define MONEY_SPRITE_UNITS_NUMBER_Y		13
#define MONEY_SPRITE_TENS_NUMBER_X		167
#define MONEY_SPRITE_TENS_NUMBER_Y		13

#define TIME_SPRITE_UNITS_NUMBER_X		176
#define TIME_SPRITE_UNITS_NUMBER_Y		21
#define TIME_SPRITE_TENS_NUMBER_X		168
#define TIME_SPRITE_TENS_NUMBER_Y		21
#define TIME_SPRITE_HUNDREDS_NUMBER_X		160
#define TIME_SPRITE_HUNDREDS_NUMBER_Y		21

#define SCORE_SPRITE_0_X				88
#define SCORE_SPRITE_0_Y				21
#define SCORE_SPRITE_WIDTH				8