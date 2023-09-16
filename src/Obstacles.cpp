#include "Obstacles.hpp"
#include "Enemy.hpp"
#include "AudioHelper.hpp"
#include "Turret.hpp"
#include "PlayScene.hpp"
#include "Bullet.hpp"

Obstacle::Obstacle(float x, float y):
	Enemy("play/enemy-obs.png", x, y, 64, 0, 200, 10) {

}

void Obstacle::Hit(float damage) {
		hp -= damage;
	if (hp <= 0) {
		OnExplode();
		// Remove all turret's reference to target.
		for (auto& it: lockedTurrets)
			it->Target = nullptr;
		for (auto& it: lockedBullets)
			it->Target = nullptr;
		getPlayScene()->EarnMoney(money);
		getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
		AudioHelper::PlayAudio("explosion.wav");
		int x = (Position.x - getPlayScene()->BlockSize/2)/getPlayScene()->BlockSize;
		int y = (Position.y - getPlayScene()->BlockSize/2)/getPlayScene()->BlockSize;
		getPlayScene()->mapState[y][x] = PlayScene::TILE_FLOOR;
	}

}
