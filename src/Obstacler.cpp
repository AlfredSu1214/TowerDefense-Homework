#include "Obstacler.hpp"
#include "Enemy.hpp"
#include "Obstacles.hpp"
#include "GenArg.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include <algorithm>
#include <cmath>
#include <random>

using namespace Attributes;
static std::mt19937 ranNum(0);
Obstacler::Obstacler(float x, float y): 
	Enemy("play/enemy-7.png", x, y, 
			EnemyAttrMap[gamemode][ENEMY_OBSTACLER][RANGE],
			EnemyAttrMap[gamemode][ENEMY_OBSTACLER][SPEED],
			EnemyAttrMap[gamemode][ENEMY_OBSTACLER][HP],
			EnemyAttrMap[gamemode][ENEMY_OBSTACLER][COST]
			),
	countDown(2.0),
	interval(3.0),
	summonCount(0.0),
	summon(true) {}

void Obstacler::Update(float deltaTime) {
	// Slow effect.
	SlowEffect(deltaTime);
	// Pre-calculate the velocity.
	if(countDown > 0) {
		countDown -= deltaTime;
	}
	else {
		if(summon) {
			if(interval > 0) {
				interval -= deltaTime;
			}
			else {
				interval = 10.0;
				summonCount++;
				summon = Summon();
			}
		}
	}

	if(summonCount >= 4)	summon = false;

	float remainSpeed = speed * deltaTime;
	while (remainSpeed != 0) {
		if (path.empty()) {
			// Reach end point.
			Hit(hp);
			getPlayScene()->Hit();
			reachEndTime = 0;
			return;
		}
		Engine::Point target = path.back() * PlayScene::BlockSize + Engine::Point(PlayScene::BlockSize / 2, PlayScene::BlockSize / 2);
		Engine::Point vec = target - Position;
		// Add up the distances:
		// 1. to path.back()
		// 2. path.back() to border
		// 3. All intermediate block size
		// 4. to end point
		reachEndTime = (vec.Magnitude() + (path.size() - 1) * PlayScene::BlockSize - remainSpeed) / speed;
		Engine::Point normalized = vec.Normalize();
		if (remainSpeed - vec.Magnitude() > 0) {
			Position = target;
			path.pop_back();
			remainSpeed -= vec.Magnitude();
		}
		else {
			Velocity = normalized * remainSpeed / deltaTime;
			remainSpeed = 0;
		}
	}
	Rotation = atan2(Velocity.y, Velocity.x);
	Sprite::Update(deltaTime);

}

bool Obstacler::Summon() {
	PlayScene* p = getPlayScene();

	std::uniform_int_distribution<int> unif(0, p->floorCoord.size());

	std::pair<int, int> a = p->floorCoord.at(unif(ranNum));

	int X = a.second*p->BlockSize + p->BlockSize/2;
	int Y = a.first*p->BlockSize + p->BlockSize/2;
	if(p->mapState[a.first][a.second] != PlayScene::TILE_FLOOR) return false;

	Obstacle* ob = new Obstacle(X, Y);
	p->ObstacleGroup->AddNewObject(ob);
	p->mapState[a.first][a.second] = PlayScene::TILE_OCCU_OB;
	return true;
}
