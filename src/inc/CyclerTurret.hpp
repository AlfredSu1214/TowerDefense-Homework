#ifndef CYCLER_HPP
#define CYCLER_HPP

#include "InplaySceneEffect.hpp"
#include "PlayScene.hpp"
#include "FLPlayScene.hpp"
#include "RotateBullet.hpp"
#include "Turret.hpp"

class CyclerTurret : public Turret {
	friend RotateBullet;
	friend void PlayScene::OnMouseUp(int button, int mx, int my);
	friend void FlPlayScene::OnMouseUp(int button, int mx, int my);
	private:
		int bulletCnt;
		bool refill;
		int refillCount;
	public:
		static const float Price[3];
		static const int angelSpeed;
		CyclerTurret(float x, float y);
		void CreateBullet() override;
		void Update(float deltaTime) override;
};
#endif // PLUGGUNTURRET_HPP
