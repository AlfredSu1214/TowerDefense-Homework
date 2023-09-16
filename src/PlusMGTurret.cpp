#include "PlusMGTurret.hpp"
#include "PlusMGBullet.hpp"
#include "PlayScene.hpp"
#include "AudioHelper.hpp"
#include "Point.hpp"
#include "GenArg.hpp"

#include <cmath>
using namespace Attributes;
const float PlusMGTurret::Price[3] = {
	TurrAttrMap[NOMOD][PLUS_MG_TURRET][COST],
	TurrAttrMap[MOD_EZ_ENABLED][PLUS_MG_TURRET][COST],
	TurrAttrMap[MOD_HR_ENABLED][PLUS_MG_TURRET][COST],
};

PlusMGTurret::PlusMGTurret(float x, float y):
	Turret("play/tower-base.png", "play/turret-2.png", x, y, 
		TurrAttrMap[gamemode][PLUS_MG_TURRET][RANGE], 
		TurrAttrMap[gamemode][PLUS_MG_TURRET][COST],
		TurrAttrMap[gamemode][PLUS_MG_TURRET][COOLDOWN],
		TurrAttrMap[gamemode][PLUS_MG_TURRET][HP]
	) {
	Anchor.y += 8.0f / GetBitmapHeight();
}

void PlusMGTurret::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
	 Engine::Point gap = Engine::Point(-3*sin(rotation), 3*cos(rotation));
    Engine::Point normalized = diff.Normalize();
    // Change bullet position to the front of the gun barrel.
    getPlayScene()->BulletGroup->AddNewObject(new PlusMGBullet(Position + normalized * 36 + gap, diff, rotation, this));
    getPlayScene()->BulletGroup->AddNewObject(new PlusMGBullet(Position + normalized * 36 - gap, diff, rotation, this));
	 getPlayScene()->AddNewObject(new ShootEffect(Position.x, Position.y));
    AudioHelper::PlayAudio("gun.wav");
}



