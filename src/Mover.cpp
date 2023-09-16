#include "Mover.hpp"
#include "Turret.hpp"

const int Mover::Price = 10;
/* move shouldn't be free because it breaks the balance */

Mover::Mover(float x, float y): Turret("play/transparent.png", "play/move.png", x, y, 0, Mover::Price, 1, 0) {}

void Mover::CreateBullet() {}
