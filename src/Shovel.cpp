#include "Shovel.hpp"
#include "PlayScene.hpp"
#include "Turret.hpp"

const int Shovel::Price = 0;

Shovel::Shovel(float x, float y): Turret("play/transparent.png", "play/shovel.png", x, y, 0, Price, 0, 0) {}

void Shovel::CreateBullet() {}

void Shovel::Shove 
(Turret* del) {
	auto obj = del->GetObjectIterator();
	getPlayScene()->TowerGroup->RemoveObject(obj);
}
