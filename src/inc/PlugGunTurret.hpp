#ifndef PLUGGUNTURRET_HPP
#define PLUGGUNTURRET_HPP
#include "InplaySceneEffect.hpp"
#include "Turret.hpp"

class PlugGunTurret : public Turret {
public:
    static const float Price[3];
    PlugGunTurret(float x, float y);
    void CreateBullet() override;
};
#endif // PLUGGUNTURRET_HPP
