#ifndef PLUSMGB_HPP
#define PLUSMGB_HPP
#include "Bullet.hpp"


class Enemy;
class Turret;
namespace Engine {
    struct Point;
}  // namespace Engine

class PlusMGBullet final: public Bullet {
public:
    explicit PlusMGBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent);
    void OnExplode(Enemy* enemy) override;
};
#endif // WOODBULLET_HPP
