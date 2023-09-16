#include <string>

#include "Enemy.hpp"
#include "RedNormalEnemy.hpp"
#include "GenArg.hpp"
using namespace Attributes;

RedNormalEnemy::RedNormalEnemy(int x, int y) : 
	Enemy("play/enemy-10.png", x, y, 
			EnemyAttrMap[gamemode][ENEMY_NORMIE][RANGE],
			EnemyAttrMap[gamemode][ENEMY_NORMIE][SPEED],
			EnemyAttrMap[gamemode][ENEMY_NORMIE][HP],
			EnemyAttrMap[gamemode][ENEMY_NORMIE][COST]
			) {}
