// [main.cpp]
// this template is provided for the 2D tower defense game.
// Program entry point.
// Returns program exit code.
#include "GameEngine.hpp"
#include "LOG.hpp"
#include "LoseScene.hpp"
#include "PlayScene.hpp"
#include "FLPlayScene.hpp"
#include "StartScene.hpp"
#include "StageSelectScene.hpp"
#include "WinScene.hpp"
#include "SettingScene.hpp"
#include "ModSel.hpp"
#include "GenArg.hpp"
using namespace Attributes;

int main(int argc, char **argv) {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();
	game.AddNewScene("stage-select", new StageSelectScene());
	game.AddNewScene("play", new PlayScene());
	game.AddNewScene("fl-play", new FlPlayScene());
	game.AddNewScene("lose", new LoseScene());
	game.AddNewScene("win", new WinScene());
	game.AddNewScene("start-scene", new StartScene());
	game.AddNewScene("setting", new SettingScene());
	game.AddNewScene("mod-select", new ModSel());

	game.Start("start-scene", 60, 1600, 832);

	return 0;
}
