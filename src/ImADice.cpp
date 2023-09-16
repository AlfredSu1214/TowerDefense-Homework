#include "ImADice.hpp"
#include "Enemy.hpp"
#include "GenArg.hpp"
using namespace Attributes;

DiceEnemy::DiceEnemy(int x, int y) : Enemy("play/dice-1.png", x, y, 
		EnemyAttrMap[gamemode][ENEMY_DICE][RANGE],
		EnemyAttrMap[gamemode][ENEMY_DICE][SPEED],
		EnemyAttrMap[gamemode][ENEMY_DICE][HP],
		EnemyAttrMap[gamemode][ENEMY_DICE][COST]
		) {}
