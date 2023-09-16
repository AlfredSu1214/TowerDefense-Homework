#ifndef PLUSMGTURRET_HPP
#define PLUSMGTURRET_HPP
#include "InplaySceneEffect.hpp"
#include "Turret.hpp"

class PlusMGTurret : public Turret {
public:
    static const float Price[3];
    PlusMGTurret(float x, float y);
    void CreateBullet() override;
};
#endif // PLUGGUNTURRET_HPP
