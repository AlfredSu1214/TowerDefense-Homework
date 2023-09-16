#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "AudioHelper.hpp"
#include "Turret.hpp"
#include "WoodBullet.hpp"
#include "Group.hpp"
#include "PlugGunTurret.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "InplaySceneEffect.hpp"
#include "GenArg.hpp"
using namespace Attributes;
const float PlugGunTurret::Price[3] = {
	TurrAttrMap[NOMOD][PLUG_GUN_TURRET][COST],
	TurrAttrMap[MOD_EZ_ENABLED][PLUG_GUN_TURRET][COST],
	TurrAttrMap[MOD_HR_ENABLED][PLUG_GUN_TURRET][COST],
};
PlugGunTurret::PlugGunTurret(float x, float y) :
	// TODO 3 (1/5): You can imitate the 2 files: 'PlugGunTurret.hpp', 'PlugGunTurret.cpp' to create a new turret.
	Turret("play/tower-base.png", "play/turret-6.png", x, y, 
			TurrAttrMap[gamemode][PLUG_GUN_TURRET][RANGE], 
			TurrAttrMap[gamemode][PLUG_GUN_TURRET][COST],
			TurrAttrMap[gamemode][PLUG_GUN_TURRET][COOLDOWN],
			TurrAttrMap[gamemode][PLUG_GUN_TURRET][HP]
			) {
		// Move center downward, since we the turret head is slightly biased upward
		Anchor.y += 8.0f / GetBitmapHeight();
	}
void PlugGunTurret::CreateBullet() {
	Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
	float rotation = atan2(diff.y, diff.x);
	Engine::Point normalized = diff.Normalize();
	// Change bullet position to the front of the gun barrel.
	getPlayScene()->BulletGroup->AddNewObject(new WoodBullet(Position + normalized * 36, diff, rotation, this));
	// TODO 4 (2/2): Add a ShootEffect here. Remember you need to include the class.
	getPlayScene()->AddNewObject(new ShootEffect(Position.x + normalized.x*36, Position.y + normalized.y*36));
	AudioHelper::PlayAudio("gun.wav");
}
