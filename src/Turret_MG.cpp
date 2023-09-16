#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "GenArg.hpp"
#include "Turret_MG.hpp"
#include "Turret.hpp"

#include "AudioHelper.hpp"
#include "MGBullet.hpp"
#include "Group.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "InplaySceneEffect.hpp"
using namespace Attributes;
const float TurretMG::Price[3] = {
	TurrAttrMap[NOMOD][MG_TURRET][COST],
	TurrAttrMap[MOD_EZ_ENABLED][MG_TURRET][COST],
	TurrAttrMap[MOD_HR_ENABLED][MG_TURRET][COST],
};

TurretMG::TurretMG(float x, float y): 
	Turret("play/tower-base.png", "play/turret-7.png", x, y, 
		TurrAttrMap[gamemode][MG_TURRET][RANGE], 
		TurrAttrMap[gamemode][MG_TURRET][COST],
		TurrAttrMap[gamemode][MG_TURRET][COOLDOWN],
		TurrAttrMap[gamemode][MG_TURRET][HP]
	) {
	Anchor.y += 8.0f / GetBitmapHeight();
}
void TurretMG::CreateBullet() {
    Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
    float rotation = atan2(diff.y, diff.x);
    Engine::Point normalized = diff.Normalize();
    // Change bullet position to the front of the gun barrel.
    getPlayScene()->BulletGroup->AddNewObject(new MGBullet(Position + normalized * 36, diff, rotation, this));
	 getPlayScene()->AddNewObject(new ShootEffect(Position.x + normalized.x*36, Position.y + normalized.y*36));
    AudioHelper::PlayAudio("gun.wav");
}
