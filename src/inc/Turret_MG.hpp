#ifndef TURRET_MG_HPP
#define TURRET_MG_HPP
#include "Turret.hpp"

class TurretMG : public Turret {
	public:
		static const float Price[3];
		TurretMG(float x, float y);
		void CreateBullet() override;
};

#endif
