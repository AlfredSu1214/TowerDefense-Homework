#ifndef ARG_HPP
#define ARG_HPP
#include "Bullet.hpp"
#include <allegro5/allegro5.h>

namespace Attributes {
	extern int gamemode;
	
	/* the order does not matter , but leave the count member at the end */
	enum TurretID{
		PLUG_GUN_TURRET,
		MG_TURRET,
		PLUS_MG_TURRET,
		MOVER,
		SHOVEL,
		CYCLER_TURRET,
		SNIPER_TURRET,
		TURRET_TNT,
		TURRET_CNT
	};

	enum Attr{
		COST,
		BULLET_TYPE,
		RANGE,
		COOLDOWN,
		HP,
		DEF,
		SPEED,
		ATTR_CNT
	};

	enum TurretBasic {
		BASIC_PLUG_GUN,
		BASIC_MG,
		BASIC_CYCLER,
	};

	enum TurretAdvanced{
		ADV_PLUS_MG,
		ADV_CNT
	};

	enum Functional {
		FN_SHOVEL,
		FN_MOVER,
	};

	enum Modifier {
		NOMOD,
		MOD_EZ_ENABLED,
		MOD_HR_ENABLED,
		MOD_CNT
	};

	enum BulletType {
		NOBUL,
		BUL_WOOD,
		BUL_MG,
		BUL_MG_P,
		BUL_RO,
		BUL_EXP,
		BUT_TYPE_CNT
	};

	enum EnemyType {
		ENEMY_NORMIE,
		ENEMY_DICE,
		ENEMY_DUPER,
		ENEMY_OBS,
		ENEMY_OBSTACLER,
		ENEMY_CNT
	};

	static const float TurrAttrMap[MOD_CNT][TURRET_CNT][ATTR_CNT] = {
		{ /* nomod */
		/* | cost | bullet | range | cooldown | hp | def | speed |  */
			{40,    BUL_WOOD, 150,    1,         100, 0,    0}, /* PLUG_GUN_TURRET */
			{100,   BUL_MG,   300,    0.33,      80,  10,   0}, /* MG_TURRET */
			{200,   BUL_MG_P, 300,    0.25,      80,  10,   0}, /* PLUS_MG_TURRET */
			{10,    NOBUL,    0,      0,         0,   0,    0}, /* MOVER*/
			{0,     NOBUL,    0,      0,         0,   0,    0}, /* SHOVEL */
			{150,   BUL_RO,   250,  ALLEGRO_PI/4,100, 20,   0},  /* CYCLER_TURRET */
			{120,   BUL_EXP,  400,    1.5       ,80,  0,   0},  /* CYCLER_TURRET */
			{300,   NOBUL,    0,     1.5         ,0,  0,   0},  /* CYCLER_TURRET */
		}, 
		{ /* ez mod */
		/* | cost | bullet | range | cooldown | hp | def | speed |  */
			{40,    BUL_WOOD, 150,    1,         100, 0,    0}, /* PLUG_GUN_TURRET */
			{100,   BUL_MG,   300,    0.33,      80,  10,   0}, /* MG_TURRET */
			{200,   BUL_MG_P, 300,    0.25,      80,  10,   0}, /* PLUS_MG_TURRET */
			{10,    NOBUL,    0,      0,         0,   0,    0}, /* MOVER*/
			{0,     NOBUL,    0,      0,         0,   0,    0}, /* SHOVEL */
			{150,   BUL_RO,   250,  ALLEGRO_PI/4,100, 20,   0},  /* CYCLER_TURRET */
			{120,   BUL_EXP,  400,    1.5       ,80, 0,     0},  /* CYCLER_TURRET */
			{300,   NOBUL,    0,      0         ,0,  0,     0},  /* CYCLER_TURRET */

		}, 
		{ /* hr mod */
			  /* the cost is 1.2 times the original */
			  /* the hp is 0.8 times the original */
		/* | cost | bullet | range | cooldown | hp | def | speed |  */
			{48,    BUL_WOOD, 150,    1,        80,  0,    0}, /* PLUG_GUN_TURRET */
			{120,   BUL_MG,   300,    0.33,     64,  10,   0}, /* MG_TURRET */
			{240,   BUL_MG_P, 300,    0.25,     64,  10,   0}, /* PLUS_MG_TURRET */
			{10,    NOBUL,    0,      0,        0,   0,    0}, /* MOVER*/
			{0,     NOBUL,    0,      0,        0,   0,    0}, /* SHOVEL */
			{180,   BUL_RO,   250,  ALLEGRO_PI/4,80, 20,   0},  /* CYCLER_TURRET */
			{144,   BUL_EXP,  400,    1.5,      64, 0,    0},  /* CYCLER_TURRET */
			{360,   NOBUL,    0,      0         ,0,  0,     0},  /* CYCLER_TURRET */

		}
	};
	static const float EnemyAttrMap[MOD_CNT][ENEMY_CNT][ATTR_CNT] = {
		{ /* nomod */
		/* | cost | bullet | range | cooldown | hp | def | speed |  */
			{5,     NOBUL,   40,     0,         100,  10,    45}, /* NORMIE */
			{8,     NOBUL,   30,     0,         80,   0,     70}, /* DICE */
			{10,    NOBUL,   60,     0,         300,  20,    10}, /* DICE_DUPER */
			{5,     NOBUL,   30,     0,         200,  0,     0}, /* OBS */
			{10,    NOBUL,   40,     0,         300,  20,    45}, /* OBSTACLER */
		}, 
		{ /* ez mod */
		/* | cost | bullet | range | cooldown | hp | def | speed |  */
			{5,     NOBUL,   48,     0,         100,  10,    36}, /* NORMIE */
			{8,     NOBUL,   36,     0,         80,   0,     56}, /* DICE */
			{10,    NOBUL,   72,     0,         300,  20,     8}, /* DICE_DUPER */
			{5,     NOBUL,   36,     0,         200,  0,     0}, /* OBS */
			{10,    NOBUL,   48,     0,         300,  20,    45}, /* OBSTACLER */
		}, 
		{ /* hr mod */
		/* | cost | bullet | range | cooldown | hp | def | speed |  */
			{7,     NOBUL,   32,     0,        150,  10,    60}, /* NORMIE */
			{12,    NOBUL,   24,     0,        100,  0,    110}, /* DICE */
			{15,    NOBUL,   48,     0,        360,  40,    10}, /* DICE_DUPER */
			{6,     NOBUL,   28,     0,        300,  0,     0}, /* OBS */
			{12,    NOBUL,   38,     0,        350,  20,    60}, /* OBSTACLER */
		}
	};
		
};

#endif
