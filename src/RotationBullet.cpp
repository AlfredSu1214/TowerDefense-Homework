#include "Bullet.hpp"
#include "Collider.hpp"
#include "RotateBullet.hpp"
#include "Turret.hpp"
#include "Enemy.hpp"
#include "PlayScene.hpp"
#include "InplaySceneEffect.hpp"
#include "DirtyEffect.hpp"
#include "CyclerTurret.hpp"
#include <algorithm>
#include <allegro5/base.h>
#include <cmath>
#include <random>
const double epsilon = 1e-7;
RotateBullet::RotateBullet(
		Engine::Point position, 
		Engine::Point forwardDirection, 
		float rotation, 
		Turret* parent, int id, 
		float angleSpeed
): Bullet(
	"play/bullet-1.png", 
	250, 150,
	forwardDirection, 
	position, 0, 
	parent
	), 
	angleSpeed(angleSpeed),
	anglePos(0) {}

void RotateBullet::Update(float deltaTime) {
	anglePos += angleSpeed*deltaTime;
	Rotation = anglePos+ALLEGRO_PI/2;
	if(anglePos > ALLEGRO_PI*2)
		anglePos = 0.0;
	Position.x = parent->Position.x + speed * cos(anglePos + ALLEGRO_PI/2);
	Position.y = parent->Position.y + speed * sin(anglePos + ALLEGRO_PI/2);

		PlayScene* scene = getPlayScene();
	// Can be improved by Spatial Hash, Quad Tree, ...
	// However simply loop through all enemies is enough for this program.
	 	for (auto& it : scene->ObstacleGroup->GetObjects()) {
		Enemy* enemy = dynamic_cast<Enemy*>(it);
		if (!enemy->Visible)
			continue;
		if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, enemy->Position, enemy->CollisionRadius)) {
			OnExplode(enemy);
			enemy->Hit(damage);
			getPlayScene()->BulletGroup->RemoveObject(objectIterator);
			return;
		}
	}

	for (auto& it : scene->EnemyGroup->GetObjects()) {
		Enemy* enemy = dynamic_cast<Enemy*>(it);
		if (!enemy->Visible)
			continue;
		if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, enemy->Position, enemy->CollisionRadius)) {
			OnExplode(enemy);
			enemy->Hit(damage);
			getPlayScene()->BulletGroup->RemoveObject(objectIterator);
			return;
		}
	}
	// Check if out of boundary.
}

void RotateBullet::OnExplode(Enemy *enemy) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 5);
    getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-2.png", dist(rng), enemy->Position.x, enemy->Position.y));
	 CyclerTurret* cy = dynamic_cast<CyclerTurret*>(parent);
	 --cy->bulletCnt;
}
