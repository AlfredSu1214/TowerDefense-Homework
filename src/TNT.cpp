#include "TNT.hpp"
#include "GenArg.hpp"
using namespace Attributes;

TurretTNT::TurretTNT(float x, float y):
	Turret("play/nuke.png", "play/transparent.png",
			x, y,
			TurrAttrMap[gamemode][TURRET_TNT][RANGE],
			TurrAttrMap[gamemode][TURRET_TNT][COST],
			TurrAttrMap[gamemode][TURRET_TNT][COOLDOWN],
			TurrAttrMap[gamemode][TURRET_TNT][HP]
			) {}

