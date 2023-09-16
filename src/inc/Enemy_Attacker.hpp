#ifndef ENEMY_ATK_HPP
#define ENEMY_ATK_HPP
#include "Enemy.hpp"
#include <cerrno>

class EnemyAttacker: public Enemy {
	protected:
		int atk;
		float coolDown;
		float reload = 0;
		virtual void CreateBullet() = 0;
	public:
		EnemyAttacker(std::string img, float x, float y, float radius, float speed, float hp, float money, int atk);
		void Update(float deltaTime) override;
		void Draw() const override;
};

#endif
