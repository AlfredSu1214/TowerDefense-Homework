#ifndef OBSTACLER_HPP
#define OBSTACLER_HPP

#include "Enemy.hpp"

class Obstacler: public Enemy {
	private:
		float countDown;
		float interval;
		int summonCount;
		bool summon;
	public:
		Obstacler(float x, float y);
		void Update(float deltaTime) override;
		bool Summon();
};

#endif
