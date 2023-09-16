#include "Enemy.hpp"
#include "Enemy_Attacker.hpp"

EnemyAttacker::EnemyAttacker(std::string img, float x, float y, float radius, float speed, float hp, float money, int atk) :
	Enemy(img, x, y, radius, speed, hp, money){
	this->atk = atk;
}



