#include "DiceDuper.hpp"
#include "GenArg.hpp"
#include "ImADice.hpp"
#include "Enemy.hpp"
#include "PlayScene.hpp"
#include "AudioHelper.hpp"
#include "Allegro5Exception.hpp"
#include "Turret.hpp"
#include "Bullet.hpp"
#include <random>
#include <iostream>
using namespace Attributes;

DiceDuper::DiceDuper(std::string img, int x, int y, int num):
	Enemy(img, x, y, 
			EnemyAttrMap[gamemode][ENEMY_DUPER][RANGE],
			EnemyAttrMap[gamemode][ENEMY_DUPER][SPEED],
			EnemyAttrMap[gamemode][ENEMY_DUPER][HP],
			EnemyAttrMap[gamemode][ENEMY_DUPER][COST]
	) {
		genNum = num;
		std::cout << genNum <<' ' << num << std::endl;
}

DiceDuper* DiceDuper::create(int x, int y, int num) {
	switch(num) {
		case 2:
			return new DiceDuper("play/dice-2.png", x, y, num);
		case 3:
			return new DiceDuper("play/dice-3.png", x, y, num);
		case 4:
			return new DiceDuper("play/dice-4.png", x, y, num);
		case 5:
			return new DiceDuper("play/dice-5.png", x, y, num);
		case 6:
			return new DiceDuper("play/dice-6.png", x, y, num);
		default:
			return new DiceDuper("play/dice-2.png", x, y, num);
	}
}

void DiceDuper::generate(int x, int y) {
	DiceEnemy* son;
	
	for(int i=1; i<=this->genNum; i++) {
		getPlayScene()->EnemyGroup->AddNewObject(son = new DiceEnemy(x, y));
		son->UpdatePath(getPlayScene()->mapDistance);
		son->Update(-0.3*i);
	}
		
}

void DiceDuper::Hit(float damage) {
	hp -= damage;
	if (hp <= 0) {
		OnExplode();
		// Remove all turret's reference to target.
		for (auto& it: lockedTurrets)
			it->Target = nullptr;
		for (auto& it: lockedBullets)
			it->Target = nullptr;
		getPlayScene()->EarnMoney(money);
		generate(Position.x, Position.y);
		getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
		AudioHelper::PlayAudio("boom.ogg");
	}

}
