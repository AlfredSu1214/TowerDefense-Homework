#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP
#include "Enemy.hpp"

class Obstacle : public Enemy {
	public:
		Obstacle(float x, float y);
		void Hit(float damage) override;
	
};

#endif
