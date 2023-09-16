#ifndef BULLET_HPP
#define BULLET_HPP
#include <string>

#include "FLPlayScene.hpp"
#include "Sprite.hpp"
#include "PlayScene.hpp"

class Enemy;
class PlayScene;
class Turret;
namespace Engine {
struct Point;
}  // namespace Engine

class Bullet : public Engine::Sprite {
	friend void PlayScene::OnMouseUp(int button, int mx, int my);
	friend void FlPlayScene::OnMouseUp(int button, int mx, int my);
protected:
	float speed;
	float damage;
	Turret* parent;
	PlayScene* getPlayScene();
	virtual void OnExplode(Enemy* enemy);
public:
	Enemy* Target = nullptr;
	explicit Bullet(std::string img, float speed, float damage, Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent);
	void Update(float deltaTime) override;
};
#endif // BULLET_HPP
