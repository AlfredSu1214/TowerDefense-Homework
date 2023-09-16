#ifndef MOVER_HPP
#define MOVER_HPP

#include "Turret.hpp"
class Mover: public Turret {
	public:
		Mover(float x, float y);
		static const int Price;
		void CreateBullet() override;
};

#endif
