#ifndef SHOVEL_HPP
#define SHOVEL_HPP
#include "Turret.hpp"

class Shovel final : public Turret {
	public:
		static const int Price;
		Shovel(float x, float y);
		void CreateBullet() override;
		void Shove(Turret* del);
};

#endif
