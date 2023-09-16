#include <allegro5/allegro_audio.h>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "AudioHelper.hpp"
#include "GameEngine.hpp"
#include "ImageButton.hpp"
#include "LOG.hpp"
#include "Label.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Resources.hpp"
#include "Slider.hpp"
#include "StageSelectScene.hpp"
#include "SettingScene.hpp"
#include "ModSel.hpp"
void CheckNDestroy(void) {
	PlayScene* ex = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
}
void StageSelectScene::Initialize() {
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;
	Engine::ImageButton* btn;
	// Stage 1 button
	btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH / 2 - 50, 400, 100);
	btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 1));
	AddNewControlObject(btn);

	AddNewObject(new Engine::Label("Stage 1", "pirulen.ttf", 48, halfW, halfH / 2, 0, 0, 0, 255, 0.5, 0.5));


	// Stage 2 button
	btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 2 / 2 - 50, 400, 100);
	btn->SetOnClickCallback(std::bind(&StageSelectScene::PlayOnClick, this, 2));
	AddNewControlObject(btn);

	AddNewObject(new Engine::Label("Stage 2", "pirulen.ttf", 48, halfW, halfH * 2 / 2, 0, 0, 0, 255, 0.5, 0.5));


	// settings button
	btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", halfW - 200, halfH * 2 / 2 + 130, 400, 100);
	btn->SetOnClickCallback(std::bind(&StageSelectScene::SettingOnClick, this));
	AddNewControlObject(btn);

	AddNewObject(new Engine::Label("Settings", "pirulen.ttf", 48, halfW, halfH * 2 / 2 + 180, 0, 0, 0, 255, 0.5, 0.5));

	// TODO 1 (2/7): Add the button which can change to setting scene.
	// TODO 1 (3/7): Move the slider to the setting scene.
	//
	// Not safe if release resource while playing, however we only free while change scene, so it's fine.
	bgmInstance = AudioHelper::PlaySample("select.ogg", true, AudioHelper::BGMVolume);
	//sliderBGM->SetValue(AudioHelper::BGMVolume);
	//sliderSFX->SetValue(AudioHelper::SFXVolume);
	CheckNDestroy();
}

void StageSelectScene::Terminate() {
	AudioHelper::StopSample(bgmInstance);
	bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}

void StageSelectScene::PlayOnClick(int stage) {
	ModSel* scene = dynamic_cast<ModSel*>(Engine::GameEngine::GetInstance().GetScene("mod-select"));
	scene->levelToload = stage;
	scene->ClearMode();
	Engine::GameEngine::GetInstance().ChangeScene("mod-select");
}

void StageSelectScene::SettingOnClick() {
	Engine::GameEngine::GetInstance().ChangeScene("setting");
}

