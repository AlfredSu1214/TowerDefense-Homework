#include "CyclerTurret.hpp"
#include "GenArg.hpp"
#include "Point.hpp"
#include "RotateBullet.hpp"
#include "Turret.hpp"
#include "PlayScene.hpp"
#include "Enemy.hpp"
#include "AudioHelper.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/base.h>
#include <cmath>
#include <functional>
using namespace Attributes;
const float CyclerTurret::Price[3] = {
	TurrAttrMap[NOMOD][CYCLER_TURRET][COST],
	TurrAttrMap[MOD_EZ_ENABLED][CYCLER_TURRET][COST],
	TurrAttrMap[MOD_HR_ENABLED][CYCLER_TURRET][COST],
};
const int CyclerTurret::angelSpeed = ALLEGRO_PI*2/3;
const double epsilon = 0.00000001;

CyclerTurret::CyclerTurret(float x, float y):
	Turret("play/tower-base.png", "play/circular.png", x, y, 
			TurrAttrMap[gamemode][CYCLER_TURRET][RANGE], 
			TurrAttrMap[gamemode][CYCLER_TURRET][COST],
			TurrAttrMap[gamemode][CYCLER_TURRET][COOLDOWN],
			TurrAttrMap[gamemode][CYCLER_TURRET][HP]
	), bulletCnt(0), refill(true), refillCount(0){}

void CyclerTurret::CreateBullet() {
		Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
	float rotation = atan2(diff.y, diff.x);
	Engine::Point normalized = diff.Normalize();
	// Change bullet position to the front of the gun barrel.
	getPlayScene()->BulletGroup->AddNewObject(new RotateBullet(Position + normalized * 36, diff, rotation, this, refillCount, angelSpeed));
	// TODO 4 (2/2): Add a ShootEffect here. Remember you need to include the class.
	getPlayScene()->AddNewObject(new ShootEffect(Position.x + normalized.x*36, Position.y + normalized.y*36));
	AudioHelper::PlayAudio("laser.wav");

}

void CyclerTurret::Update(float deltaTime) {
	Sprite::Update(deltaTime);
	PlayScene* scene = getPlayScene();
	imgBase.Position = Position;
	imgBase.Tint = Tint;
	if (!Enabled) {
		return;
	}
	if(bulletCnt == 0 && !refill) {
		refill = true;
		refillCount = 0;
	}
	if(refillCount == 4 && refill)
		refill = false;
//	if (Target) {
		// Might have floating-point precision error.
	Rotation += angelSpeed*deltaTime;
	if(Rotation > 2*ALLEGRO_PI)
		Rotation = 0.0;
	// Shoot reload.
	if(refill) {
		reload -= deltaTime;
		if (reload <= 0) {
			// shoot.
			reload = coolDown;
			CreateBullet();
			bulletCnt++;
			refillCount++;
		}
	}
//	}

}
