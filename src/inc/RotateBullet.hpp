#ifndef RTBULLET_HPP
#define RTBULLET_HPP
#include "Bullet.hpp"


class Enemy;
class Turret;
namespace Engine {
    struct Point;
}  // namespace Engine

class RotateBullet final: public Bullet {
private:
	float angleSpeed;
	float anglePos;
public:
    explicit RotateBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent, int id, float angleSpeed);
    void OnExplode(Enemy* enemy) override;
	 void Update(float deltaTime) override;
};
#endif // WOODBULLET_HPP
