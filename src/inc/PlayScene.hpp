#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>

#include "IScene.hpp"
#include "Point.hpp"

class Turret;
namespace Engine {
	class Group;
	class Image;
	class Label;
	class Sprite;
}  // namespace Engine

class PlayScene : public Engine::IScene {
private:
	ALLEGRO_SAMPLE_ID bgmId;
	std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> deathBGMInstance;
	std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
protected:
	int lives;
	int money;
	int SpeedMult;
public:
	enum TileType {
		TILE_DIRT,
		TILE_FLOOR,
		TILE_OCCUPIED,
		TILE_OCCU_MG,
		TILE_OCCU_R,
		TILE_OCCU_OB,
	};
	static bool DebugMode;
	static const std::vector<Engine::Point> directions;
	static const int MapWidth, MapHeight;
	static const int BlockSize;
	static const float DangerTime;
	static const Engine::Point SpawnGridPoint;
	static const Engine::Point EndGridPoint;
	static const std::vector<int> code;
	bool mute;
	bool isPF;
	bool isNF;
	bool isRX;
	bool Paused;
	bool Moving;
	int MapId;
	int diff;
	float ticks;
	float deathCountDown;
	// Map tiles.
	Group* UIGroup;
	Group* TileMapGroup;
	Group* GroundEffectGroup;
	Group* DebugIndicatorGroup;
	Group* BulletGroup;
	Group* TowerGroup;
	Group* EnemyGroup;
	Group* EffectGroup;
	Group* ObstacleGroup;
	Engine::Label* UIMoney;
	Engine::Label* UILives;
	Engine::Image* imgTarget;
	Engine::Sprite* dangerIndicator;
	Turret* preview;
	std::vector<std::vector<TileType>> mapState;
	std::vector<std::vector<int>> mapDistance;
	std::vector<std::pair<int, int>> floorCoord;
	std::list<std::pair<int, float>> enemyWaveData;
	std::list<int> keyStrokes;
	static Engine::Point GetClientSize();
	explicit PlayScene();
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Draw() const override;
	void OnMouseDown(int button, int mx, int my) override;
	void OnMouseMove(int mx, int my) override;
	void OnMouseUp(int button, int mx, int my) override;
	void OnKeyDown(int keyCode) override;
	void Hit();
	int GetMoney() const;
	void EarnMoney(int money);
	void ReadMap();
	void ReadEnemyWave();
	void ConstructUI();
	void ConstructButton(int id, std::string sprite, int price);
	void UIBtnClicked(int id);
	bool CheckSpaceValid(int x, int y);
	std::vector<std::vector<int>> CalculateBFSDistance();
	// void ModifyReadMapTiles();
};
#endif // PLAYSCENE_HPP
