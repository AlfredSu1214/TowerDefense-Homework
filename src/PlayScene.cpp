#include <allegro5/allegro.h>
#include <algorithm>
#include <allegro5/allegro_audio.h>
#include <allegro5/color.h>
#include <allegro5/keycodes.h>
#include <cmath>
#include <random>
#include <fstream>
#include <functional>
#include <list>
#include <vector>
#include <queue>
#include <string>
#include <memory>

#include "AudioHelper.hpp"
#include "DirtyEffect.hpp"
#include "Enemy.hpp"
#include "GameEngine.hpp"
#include "GenArg.hpp"
#include "Group.hpp"
#include "IObject.hpp"
#include "Image.hpp"
#include "Label.hpp"
// Turret
#include "PlugGunTurret.hpp"
#include "Plane.hpp"
#include "RotateBullet.hpp"
#include "Turret.hpp"
#include "Turret_MG.hpp"
#include "CyclerTurret.hpp"
#include "PlusMGTurret.hpp"
#include "Shovel.hpp"
#include "Mover.hpp"
// Enemy
#include "Obstacler.hpp"
#include "Point.hpp"
#include "RedNormalEnemy.hpp"
#include "ImADice.hpp"
#include "PlayScene.hpp"
#include "Resources.hpp"
#include "Sprite.hpp"
#include "TurretButton.hpp"
#include "LOG.hpp"
#include "WinScene.hpp"
#include "DiceDuper.hpp"


bool PlayScene::DebugMode = false;
const std::vector<Engine::Point> PlayScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int PlayScene::MapWidth = 20, PlayScene::MapHeight = 13;
const int PlayScene::BlockSize = 64;
const float PlayScene::DangerTime = 7.61;
const Engine::Point PlayScene::SpawnGridPoint = Engine::Point(-1, 0);
const Engine::Point PlayScene::EndGridPoint = Engine::Point(MapWidth, MapHeight - 1);
std::mt19937 ranGen;
std::uniform_int_distribution<int> uniGen;
// TODO 5 (2/3): Set the cheat code correctly.
const std::vector<int> PlayScene::code = {
	ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_LEFT, 
	ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, 
	ALLEGRO_KEY_ENTER
};
int counter = 0;
int exit_counter;

int screenw;
int screenh;
int multBuf;
unsigned int bgmSamplePos;
const std::string bgmPath = "derwald.ogg";

Engine::Point PlayScene::GetClientSize() {
	return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}

void PlayScene::Initialize() {
	// TODO 6 (1/2): There's a bug in this file, which crashes the game when you win. Try to find it.
	// TODO 6 (2/2): There's a bug in this file, which doesn't update the player's life correctly when getting the first attack. Try to find it.
	screenw = Engine::GameEngine::GetInstance().GetScreenWidth();
	screenh = Engine::GameEngine::GetInstance().GetScreenHeight();
	ranGen.seed(0);
	uniGen = std::uniform_int_distribution<int>(1, 6);
	mapState.clear();
	keyStrokes.clear();
	ticks = 0;
	deathCountDown = -1;
	lives = isPF ? 1 : 10;
	money = 300;
	SpeedMult = 1;
	DebugMode = false;
	Paused = false;
	counter = 0;
	exit_counter = 0;
	Moving = false;
	mute = false;
	// Add groups from bottom to top.
	AddNewObject(TileMapGroup = new Group());
	AddNewObject(GroundEffectGroup = new Group());
	AddNewObject(DebugIndicatorGroup = new Group());
	AddNewObject(TowerGroup = new Group());
	AddNewObject(EnemyGroup = new Group());
	AddNewObject(BulletGroup = new Group());
	AddNewObject(EffectGroup = new Group());
	AddNewObject(ObstacleGroup = new Group());
	// Should support buttons.
	AddNewControlObject(UIGroup = new Group());
	ReadMap();
	ReadEnemyWave();
	mapDistance = CalculateBFSDistance();
	ConstructUI();
	imgTarget = new Engine::Image("play/target.png", 0, 0);
	imgTarget->Visible = false;
	preview = nullptr;
	UIGroup->AddNewObject(imgTarget);
	// Preload Lose Scene
	deathBGMInstance = Engine::Resources::GetInstance().GetSampleInstance("astronomia.ogg");
   bgmInstance = AudioHelper::PlaySample(bgmPath, true, AudioHelper::BGMVolume);
	Engine::Resources::GetInstance().GetBitmap("lose/benjamin-happy.png");
	// Start BGM.
	// bgmId = AudioHelper::PlayBGM("play.ogg");
	if (!mute)
		AudioHelper::ChangeSampleVolume(bgmInstance, AudioHelper::BGMVolume);
	else
		AudioHelper::ChangeSampleVolume(bgmInstance, 0.0);
	std::cout << AudioHelper::GetSampleLength(bgmInstance) << std::endl;
}

void PlayScene::Terminate() {
	AudioHelper::StopBGM(bgmId);
	AudioHelper::StopSample(deathBGMInstance);
	AudioHelper::StopSample(bgmInstance);
	deathBGMInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}

void PlayScene::Update(float deltaTime) {
	// If we use deltaTime directly, then we might have Bullet-through-paper problem.
	// Reference: Bullet-Through-Paper
	if (SpeedMult == 0)
		deathCountDown = -1;
	else if (deathCountDown != -1)
		SpeedMult = 1;
if(!isNF) {
	// Calculate danger zone.
	std::vector<float> reachEndTimes;
	for (auto& it : EnemyGroup->GetObjects()) {
		reachEndTimes.push_back(dynamic_cast<Enemy*>(it)->reachEndTime);
	}
	// Can use Heap / Priority-Queue instead. But since we won't have too many enemies, sorting is fast enough.
	std::sort(reachEndTimes.begin(), reachEndTimes.end());
	float newDeathCountDown = -1;
	int danger = lives;
	for (auto& it : reachEndTimes) {
		if (it <= DangerTime) {
			danger--;
			if (danger <= 0) {
				// Death Countdown
				float pos = DangerTime - it;
				if (it > deathCountDown) {
					// Restart Death Count Down BGM.
					AudioHelper::StopSample(deathBGMInstance);
					if (SpeedMult != 0)
						deathBGMInstance = AudioHelper::PlaySample("astronomia.ogg", false, AudioHelper::BGMVolume, pos);
				}
				float alpha = pos / DangerTime;
				alpha = std::max(0, std::min(255, static_cast<int>(alpha * alpha * 255)));
				dangerIndicator->Tint = al_map_rgba(255, 255, 255, alpha);
				newDeathCountDown = it;
				break;
			}
		}
	}
	deathCountDown = newDeathCountDown;
}
	if (SpeedMult == 0)
		AudioHelper::StopSample(deathBGMInstance);
	if (deathCountDown == -1 && lives > 0) {
		AudioHelper::StopSample(deathBGMInstance);
		dangerIndicator->Tint.a = 0;
	}
	if (SpeedMult == 0)
		deathCountDown = -1;
	for (int i = 0; i < SpeedMult; i++) {
		IScene::Update(deltaTime);
		// Check if we should create new enemy.
		ticks += deltaTime;
		if (enemyWaveData.empty()) {
			if (EnemyGroup->GetObjects().empty()) {
				AudioHelper::StopBGM(bgmId);
				AudioHelper::StopSample(bgmInstance);
				//delete imgTarget;
            Engine::GameEngine::GetInstance().ChangeScene("win");
				// Free resources.
			}
			continue;
		}
		auto current = enemyWaveData.front();
		if (ticks < current.second)
			continue;
		ticks -= current.second;
		enemyWaveData.pop_front();
		const Engine::Point SpawnCoordinate = Engine::Point(SpawnGridPoint.x * BlockSize + BlockSize / 2, SpawnGridPoint.y * BlockSize + BlockSize / 2);
		Enemy* enemy;
		int pick;
		switch (current.first) {
		case 1:
			EnemyGroup->AddNewObject(enemy = new RedNormalEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
			break;
		// TODO 2 (2/3): You need to modify 'resources/enemy1.txt', or 'resources/enemy2.txt' to spawn the new enemy.
		// The format is "[EnemyId] [TimeDelay] [Repeat]".
		// TODO 2 (3/3): Enable the creation of the new enemy.
		case 2:
			EnemyGroup->AddNewObject(enemy = new DiceEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
			break;
		case 3:
			pick = uniGen(ranGen);
			if(pick >= 1 && pick <=6)
					EnemyGroup->AddNewObject(enemy = DiceDuper::create(SpawnCoordinate.x, SpawnCoordinate.y, pick));
			else
				EnemyGroup->AddNewObject(enemy = DiceDuper::create(SpawnCoordinate.x, SpawnCoordinate.y, 3));
			break;
		case 4:
			EnemyGroup->AddNewObject(enemy = new Obstacler(SpawnCoordinate.x, SpawnCoordinate.y));
			break;
		default:
			continue;
		}
		enemy->UpdatePath(mapDistance);
		// Compensate the time lost.
		enemy->Update(ticks);
	}
	if (preview) {
		preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
		// To keep responding when paused.
		preview->Update(deltaTime);
	}
}
void PlayScene::Draw() const {
	IScene::Draw();
	if (DebugMode) {
		// Draw reverse BFS distance on all reachable blocks.
		for (int i = 0; i < MapHeight; i++) {
			for (int j = 0; j < MapWidth; j++) {
				if (mapDistance[i][j] != -1) {
					// Not elegant nor efficient, but it's quite enough for debugging.
					Engine::Label label(std::to_string(mapDistance[i][j]), "pirulen.ttf", 32, (j + 0.5) * BlockSize, (i + 0.5) * BlockSize);
					label.Anchor = Engine::Point(0.5, 0.5);
					label.Draw();
				}
			}
		}
	}
	if (Paused) {
		Engine::Label hint("The Game is Paused\n", "pirulen.ttf", 32, screenw/2.0, screenh/2.0);
		Engine::Label hint2("Press P to Toggle", "pirulen.ttf", 32, screenw/2.0, screenh/2.0+32);
		hint.Anchor = Engine::Point(0.5, 0.5);
		hint2.Anchor = Engine::Point(0.5, 0.5);
		hint.Draw();
		hint2.Draw();
	}
}

void PlayScene::OnMouseDown(int button, int mx, int my) {
	if ((button & 1) && !imgTarget->Visible && preview) {
		// Cancel turret construct.
		UIGroup->RemoveObject(preview->GetObjectIterator());
		preview = nullptr;
	}
	IScene::OnMouseDown(button, mx, my);
}

void PlayScene::OnMouseMove(int mx, int my) {
	IScene::OnMouseMove(mx, my);
	const int x = mx / BlockSize;
	const int y = my / BlockSize;
	if (!preview || x < 0 || x >= MapWidth || y < 0 || y >= MapHeight) {
		imgTarget->Visible = false;
		return;
	}
	imgTarget->Visible = true;
	imgTarget->Position.x = x * BlockSize;
	imgTarget->Position.y = y * BlockSize;
}

void PlayScene::OnMouseUp(int button, int mx, int my) {
	IScene::OnMouseUp(button, mx, my);
	if (!imgTarget->Visible) {
		Moving = false;
		return;
	}
	const int x = mx / BlockSize;
	const int y = my / BlockSize;
	if (button & 1) {
		if(dynamic_cast<Mover*>(preview)) {
			if(mapState[y][x] != TILE_DIRT && mapState[y][x] != TILE_FLOOR && mapState[y][x] != TILE_OCCU_OB) {
				float X = x*BlockSize + BlockSize/2;
				float Y = y*BlockSize + BlockSize/2;
				auto ls = TowerGroup->GetObjects();
				preview->GetObjectIterator()->first = false;
				UIGroup->RemoveObject(preview->GetObjectIterator());
				for(auto it = ls.begin(); it != ls.end(); ++it) {
					if((*it)->Position.x == X && (*it)->Position.y == Y) {
						preview = dynamic_cast<Turret*>(*it);
						break;
					}
				}
				if(mapState[y][x] == TILE_OCCU_R) {
					auto ls = BulletGroup->GetObjects();
					for(auto it = ls.begin(); it != ls.end(); ++it) {
						Bullet* obj = dynamic_cast<Bullet*>(*it);
						if(obj->parent == preview)
							BulletGroup->RemoveObject(obj->objectIterator);
					}
				}

				preview->Enabled = false;	
				preview->Preview = true;
				Moving = true;
				mapState[y][x] = TILE_FLOOR;
				if(!isRX)
					EarnMoney(-Mover::Price);
				/* here I emphasize again, tower defense game shouldn't be allowed to move character freely */
				OnMouseMove(mx, my);
				return;
			}
			preview->GetObjectIterator()->first = false;
			UIGroup->RemoveObject(preview->GetObjectIterator());
			preview->Enabled = true;	
			preview->Preview = false;
			preview = nullptr;
			return;
		}
		if(preview->GetPrice() == Shovel::Price) {
			if(mapState[y][x] == TILE_DIRT || 
					mapState[y][x] == TILE_FLOOR) {
				preview->GetObjectIterator()->first = false;
				UIGroup->RemoveObject(preview->GetObjectIterator());
				preview->Enabled = true;	
				preview->Preview = false;
				preview = nullptr;
				return;
			}
			float X = x*BlockSize + BlockSize/2;
			float Y = y*BlockSize + BlockSize/2;
			Shovel* sh = dynamic_cast<Shovel*>(preview);
			auto ls = TowerGroup->GetObjects();
			Turret* rmTarg;
			for(auto it = ls.begin(); it != ls.end(); ++it) {
				if((*it)->Position.x == X && (*it)->Position.y == Y) {
					rmTarg = dynamic_cast<Turret*>(*it);
				}
			}
			if(mapState[y][x] == TILE_OCCU_R) {
				auto ls = BulletGroup->GetObjects();
				for(auto it = ls.begin(); it != ls.end(); ++it) {
					Bullet* obj = dynamic_cast<Bullet*>(*it);
					if(obj->parent == rmTarg)
						BulletGroup->RemoveObject(obj->objectIterator);
				}
			}
			EarnMoney(rmTarg->GetPrice()/2);
			sh->Shove(rmTarg);
			preview->GetObjectIterator()->first = false;
			UIGroup->RemoveObject(preview->GetObjectIterator());
			preview->Enabled = true;	
			preview->Preview = false;
			preview = nullptr;
			mapState[y][x] = TILE_FLOOR;
			return;
		}
		if(mapState[y][x] == TILE_OCCU_MG) {
			if(preview && preview->GetPrice() == TurretMG::Price[Attributes::gamemode]) {
				preview->GetObjectIterator()->first = false;
				UIGroup->RemoveObject(preview->GetObjectIterator());
				delete preview;
				float X = x*BlockSize + BlockSize/2;
				float Y = y*BlockSize + BlockSize/2;
				preview = new PlusMGTurret(X, Y);
				preview->Enabled = true;	
				preview->Preview = false;
				preview->Tint = al_map_rgba(255, 255, 255, 255);
				auto ls = TowerGroup->GetObjects();
				std::list<std::pair<bool, Engine::IObject*>>::iterator obj;
				for(auto it = ls.begin(); it != ls.end(); ++it) {
					if((*it)->Position.x == X && (*it)->Position.y == Y)
						obj = (*it)->GetObjectIterator();
				}
				TowerGroup->RemoveObject(obj);
				TowerGroup->AddNewObject(preview);
				mapState[y][x] = TILE_OCCUPIED;
				preview = nullptr;
			}
				return;

		}
		if (mapState[y][x] == TILE_FLOOR) {
			if (!preview)
				return;
			// Check if valid.
			if (!CheckSpaceValid(x, y)) {
				Engine::Sprite* sprite;
				GroundEffectGroup->AddNewObject(sprite = new DirtyEffect("play/target-invalid.png", 1, x * BlockSize + BlockSize / 2, y * BlockSize + BlockSize / 2));
				sprite->Rotation = 0;
				return;
			}
			// Purchase.
			if(!Moving && !isRX)
				EarnMoney(-preview->GetPrice());
			else
				Moving = false;
			// Remove Preview.
			preview->GetObjectIterator()->first = false;
			UIGroup->RemoveObject(preview->GetObjectIterator());
			// Construct real turret.
			preview->Position.x = x * BlockSize + BlockSize / 2;
			preview->Position.y = y * BlockSize + BlockSize / 2;
			preview->Enabled = true;
			preview->Preview = false;
			preview->Tint = al_map_rgba(255, 255, 255, 255);
			if(dynamic_cast<TurretMG*>(preview))
				mapState[y][x] = TILE_OCCU_MG;
			else if(dynamic_cast<CyclerTurret*>(preview)) {
				mapState[y][x] = TILE_OCCU_R;
				CyclerTurret* tr = dynamic_cast<CyclerTurret*>(preview);
				tr->refillCount = 0;
				tr->refill = true;
				tr->bulletCnt = 0;
			}
			else
				mapState[y][x] = TILE_OCCUPIED;
			TowerGroup->AddNewObject(preview);
			// To keep responding when paused.
			preview->Update(0);
			// Remove Preview.
			preview = nullptr;

			OnMouseMove(mx, my);
		}
	}
}

void PlayScene::OnKeyDown(int keyCode) {
	IScene::OnKeyDown(keyCode);
	if (keyCode == ALLEGRO_KEY_TAB) {
		// TODO 5 (1/3): Set Tab as a code to active / de-active the debug mode.
		DebugMode = !DebugMode;	
	}
	else if(keyCode == ALLEGRO_KEY_ESCAPE) {
		exit_counter++;
		if(exit_counter == 2) {
				AudioHelper::StopBGM(bgmId);
				AudioHelper::StopSample(bgmInstance);
            Engine::GameEngine::GetInstance().ChangeScene("stage-select");
		}
	}
	else {
		exit_counter = 0;
		keyStrokes.push_back(keyCode);
		if (keyStrokes.size() > code.size()) {
			keyStrokes.pop_front();
		}
		if(keyCode == code[counter]) counter++;
		if(counter == 7) {
			EffectGroup->AddNewObject(new Plane());
			money += 10000;
			counter = 0;
		}
		// TODO 5 (3/3): Check whether the input eeesequence corresponds to the code. If so, active a plane and earn 10000 money.
        // Active a plane : 
		// Earn money : 
	}
	if (keyCode == ALLEGRO_KEY_Q) {
		// Hotkey for PlugGunTurret.
		UIBtnClicked(0);
	}
	// TODO 3 (5/5): Make the W key to create the new turret.
	else if (keyCode == ALLEGRO_KEY_W) {
		// Hotkey for new turret.
		UIBtnClicked(1);
	}
	else if (keyCode == ALLEGRO_KEY_E) {
		UIBtnClicked(4);
	}
	else if (keyCode == ALLEGRO_KEY_R) {
		UIBtnClicked(3);
	}
	else if(keyCode == ALLEGRO_KEY_BACKSPACE) {
		UIBtnClicked(2);
	}
	else if (keyCode >= ALLEGRO_KEY_1 && keyCode <= ALLEGRO_KEY_9) {
		// Hotkey for Speed up.
		if(!Paused)
			SpeedMult = keyCode - ALLEGRO_KEY_0;
	}
	else if (keyCode == ALLEGRO_KEY_M) {
		// Hotkey for mute / unmute.
        mute = !mute;
        if (!mute)
            AudioHelper::ChangeSampleVolume(bgmInstance, AudioHelper::BGMVolume);
        else
            AudioHelper::ChangeSampleVolume(bgmInstance, 0.0);
	}
	else if (keyCode == ALLEGRO_KEY_P) {
		Paused = !Paused;
		if(Paused) {
			multBuf = SpeedMult;
			SpeedMult = 0;
			std::cout << bgmSamplePos << std::endl;
			if(!mute)
				bgmSamplePos = AudioHelper::GetSamplePosition(bgmInstance);
			AudioHelper::StopSample(bgmInstance);
		}
		else {
			SpeedMult = multBuf;
			if(mute)
				bgmInstance = AudioHelper::PlaySample(bgmPath, true, AudioHelper::BGMVolume, 0);
			else
				bgmInstance = AudioHelper::PlaySample(bgmPath, true, AudioHelper::BGMVolume, 0.0);
			AudioHelper::ChangeSamplePosition(bgmInstance, bgmSamplePos);
		}
	}
}
void PlayScene::Hit() {
	if(isNF) return;

	UILives->Text = std::string("Life ") + std::to_string(--lives);
		if (lives <= 0) {
			Engine::GameEngine::GetInstance().ChangeScene("lose");
		}
}
int PlayScene::GetMoney() const {
	return money;
}
void PlayScene::EarnMoney(int money) {
	this->money += money;
	UIMoney->Text = std::string("$") + std::to_string(this->money);
}
void PlayScene::ReadMap() {
	std::string filename = std::string("resources/map") + std::to_string(MapId) + ".txt";
	// Read map file.
	char c;
	std::vector<bool> mapData;
	std::ifstream fin(filename);
	while (fin >> c) {
		switch (c) {
			case '0': mapData.push_back(false); break;
			case '1': mapData.push_back(true); break;
			case '\n':
			case '\r':
						 if (static_cast<int>(mapData.size()) / MapWidth != 0)
							 throw std::ios_base::failure("Map data is corrupted.");
						 break;
			default: throw std::ios_base::failure("Map data is corrupted.");
		}
	}
	fin.close();
	// Validate map data.
	if (static_cast<int>(mapData.size()) != MapWidth * MapHeight)
		throw std::ios_base::failure("Map data is corrupted.");
	// Store map in 2d array.
	mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
	for (int i = 0; i < MapHeight; i++) {
		for (int j = 0; j < MapWidth; j++) {
			const int num = mapData[i * MapWidth + j];
			mapState[i][j] = num ? TILE_FLOOR : TILE_DIRT;
			if (num) {
				TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
				floorCoord.push_back(std::pair(i, j));
			}
			else
				TileMapGroup->AddNewObject(new Engine::Image("play/dirt.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
		}
	}
}
void PlayScene::ReadEnemyWave() {
	std::string filename = std::string("resources/enemy") + std::to_string(MapId) + ".txt";
	// Read enemy file.
	float type, wait, repeat;
	enemyWaveData.clear();
	std::ifstream fin(filename);
	while (fin >> type && fin >> wait && fin >> repeat) {
		for (int i = 0; i < repeat; i++)
			enemyWaveData.emplace_back(type, wait);
	}
	fin.close();
}
void PlayScene::ConstructUI() {
	// Background
	UIGroup->AddNewObject(new Engine::Image("play/sand.png", 1280, 0, 320, 832));
	// Text
	UIGroup->AddNewObject(new Engine::Label(std::string("Stage ") + std::to_string(MapId), "pirulen.ttf", 32, 1294, 0));
	UIGroup->AddNewObject(UIMoney = new Engine::Label(std::string("$") + std::to_string(money), "pirulen.ttf", 24, 1294, 48));
	if(isNF)
		UIGroup->AddNewObject(UILives = new Engine::Label(std::string("No Fail Enabled"), "pirulen.ttf", 24, 1294, 88));
	else if (isPF)
		UIGroup->AddNewObject(UILives = new Engine::Label(std::string("Brink Of Extinction"), "pirulen.ttf", 24, 1294, 88));
	else
		UIGroup->AddNewObject(UILives = new Engine::Label(std::string("Life ") + std::to_string(lives), "pirulen.ttf", 24, 1294, 88));
	// Buttons
	ConstructButton(0, "play/turret-6.png", PlugGunTurret::Price[Attributes::gamemode]);
	// TODO 3 (3/5): Create a button to support constructing the new turret.
	ConstructButton(1, "play/turret-7.png", TurretMG::Price[Attributes::gamemode]);
	ConstructButton(2, "play/shovel.png", Shovel::Price);
	ConstructButton(3, "play/move.png", Mover::Price);
	ConstructButton(4, "play/circular.png", CyclerTurret::Price[Attributes::gamemode]);

	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int shift = 135 + 25;
	dangerIndicator = new Engine::Sprite("play/benjamin.png", w - shift, h - shift);
	dangerIndicator->Tint.a = 0;
	UIGroup->AddNewObject(dangerIndicator);
}

void PlayScene::ConstructButton(int id, std::string sprite, int price) {
	TurretButton* btn;
	btn = new TurretButton("play/floor.png", "play/dirt.png",
			Engine::Sprite("play/tower-base.png", 1294 + (id%3) * 76, 136 + floor(id/3.0)*76, 0, 0, 0, 0),
			Engine::Sprite(sprite, 1294 + (id%3) * 76, 136 + floor(id/3.0)*76 - 8, 0, 0, 0, 0)
			, 1294 + (id%3) * 76, 136 + floor(id/3.0) * 76, price);
	// Reference: Class Member Function Pointer and std::bind.
	btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, id));
	UIGroup->AddNewControlObject(btn);
}

void PlayScene::UIBtnClicked(int id) {
	if (preview) {
		UIGroup->RemoveObject(preview->GetObjectIterator());
		preview = nullptr;
	}
	if(Paused) return;

	if(isRX) {
		switch(id) {
			case 0:
				preview = new PlugGunTurret(0, 0);
				break;
			case 1:
				preview = new TurretMG(0, 0);
				break;
			case 2:
				preview = new Shovel(0, 0);
				break;
			case 3:
				preview = new Mover(0, 0);
				break;
			case 4:
				preview = new CyclerTurret(0, 0);
				break;
			default:
				break;
		}
	}
	else {
		if ((id == 0 && money >= PlugGunTurret::Price[Attributes::gamemode])) 
			preview = new PlugGunTurret(0, 0);
		// TODO 3 (4/5): On the new turret button callback, create the new turret.
		else if((id == 1 && money >= TurretMG::Price[Attributes::gamemode] ))
			preview = new TurretMG(0, 0);
		else if(id == 2)
			preview = new Shovel(0, 0);
		else if((id == 3 && money >= Mover::Price))
			preview = new Mover(0, 0);
		else if((id == 4 && money >= CyclerTurret::Price[Attributes::gamemode]))
			preview = new CyclerTurret(0, 0);
	}


	if (!preview)
		return;
	preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
	preview->Tint = al_map_rgba(255, 255, 255, 200);
	preview->Enabled = false;
	preview->Preview = true;
	UIGroup->AddNewObject(preview);
	OnMouseMove(Engine::GameEngine::GetInstance().GetMousePosition().x, Engine::GameEngine::GetInstance().GetMousePosition().y);
}

bool PlayScene::CheckSpaceValid(int x, int y) {
	if (x < 0 || x >= MapWidth || y < 0 || y >= MapHeight)
		return false;
	auto map00 = mapState[y][x];
	mapState[y][x] = TILE_OCCUPIED;
	std::vector<std::vector<int>> map = CalculateBFSDistance();
	mapState[y][x] = map00;
	if (map[0][0] == -1)
		return false;
	for (auto& it : EnemyGroup->GetObjects()) {
		Engine::Point pnt;
		pnt.x = floor(it->Position.x / BlockSize);
		pnt.y = floor(it->Position.y / BlockSize);
		if (pnt.x < 0) pnt.x = 0;
		if (pnt.x >= MapWidth) pnt.x = MapWidth - 1;
		if (pnt.y < 0) pnt.y = 0;
		if (pnt.y >= MapHeight) pnt.y = MapHeight - 1;
		if (map[pnt.y][pnt.x] == -1)
			return false;
	}
	// All enemy have path to exit.
	mapState[y][x] = TILE_OCCUPIED;
	mapDistance = map;
	for (auto& it : EnemyGroup->GetObjects())
		dynamic_cast<Enemy*>(it)->UpdatePath(mapDistance);
	return true;
}
std::vector<std::vector<int>> PlayScene::CalculateBFSDistance() {
	// Reverse BFS to find path.
	std::vector<std::vector<int>> map(MapHeight, std::vector<int>(std::vector<int>(MapWidth, -1)));
	std::queue<Engine::Point> que;
	// Push end point.
	// BFS from end point.
	if (mapState[MapHeight - 1][MapWidth - 1] != TILE_DIRT)
		return map;
	que.push(Engine::Point(MapWidth - 1, MapHeight - 1));
	map[MapHeight - 1][MapWidth - 1] = 0;
	while (!que.empty()) {
		Engine::Point p = que.front();
		que.pop();
        for (auto &c : directions) {
            int x = p.x + c.x;
            int y = p.y + c.y;
            if (x < 0 || x >= MapWidth || y < 0 || y >= MapHeight ||
                map[y][x] != -1 || mapState[y][x] != TILE_DIRT) {
                continue;
            } else {
                map[y][x] = map[p.y][p.x] + 1;
                que.push(Engine::Point(x, y));
            }
        }
	}
	return map;
}

PlayScene::PlayScene(): 
	IScene(), 
	TileMapGroup(nullptr), 
	GroundEffectGroup(nullptr), 
	DebugIndicatorGroup(nullptr), 
	BulletGroup(nullptr),
	TowerGroup(nullptr),
	EnemyGroup(nullptr),
	EffectGroup(nullptr),
	UIGroup(nullptr)
	{}
