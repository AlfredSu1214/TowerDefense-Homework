#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <iostream>

#include "AudioHelper.hpp"
#include "GameEngine.hpp"
#include "ImageButton.hpp"
#include "Label.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Resources.hpp"
#include "Slider.hpp"
#include "SettingScene.hpp"
#include "StageSelectScene.hpp"

void SettingScene::Initialize() {
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;
	Engine::ImageButton* btn;
	// TODO 1 (2/7): Add the button which can change to setting scene.
	// TODO 1 (3/7): Move the slider to the setting scene.
	VolumeSlider *sliderBGM, *sliderSFX;

	sliderBGM = new VolumeSlider(40 + halfW - 95, halfH - 50 - 2, 190, 4, "pop.ogg", AudioHelper::BGMVolume);

	sliderBGM->SetOnValueChangedCallback(std::bind(&SettingScene::BGMSlideOnValueChanged, this, std::placeholders::_1));
	
	AddNewControlObject(sliderBGM);
	AddNewObject(new Engine::Label("BGM: ", "pirulen.ttf", 28, 40 + halfW - 60 - 95, halfH - 50, 255, 255, 255, 255, 0.5, 0.5));

	sliderSFX = new VolumeSlider(40 + halfW - 95, halfH + 50 - 2, 190, 4, "pop.ogg", AudioHelper::SFXVolume);

	sliderSFX->SetOnValueChangedCallback(std::bind(&SettingScene::SFXSlideOnValueChanged, this, std::placeholders::_1));
	AddNewControlObject(sliderSFX);
	AddNewObject(new Engine::Label("SFX: ", "pirulen.ttf", 28, 40 + halfW - 60 - 95, halfH + 50, 255, 255, 255, 255, 0.5, 0.5));

	btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", w - 600, h - 250, 400, 100);
	btn->SetOnClickCallback(std::bind(&SettingScene::BackOnClick, this));
	AddNewControlObject(btn);

	AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, w - 400, h - 200, 0, 0, 0, 255, 0.5, 0.5));

	// Not safe if release resource while playing, however we only free while change scene, so it's fine.
	bgmInstance = AudioHelper::PlaySample("perl.oga", true, AudioHelper::BGMVolume);
	//bgmInstance = nullptr;
	sliderBGM->SetValue(AudioHelper::BGMVolume);
	sliderSFX->SetValue(AudioHelper::SFXVolume);
}

void SettingScene::Terminate() {
	AudioHelper::StopSample(bgmInstance);
	bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}
void SettingScene::BackOnClick() {
	//PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
	//scene->MapId = stage;
	Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void SettingScene::BGMSlideOnValueChanged(float value) {
	//AudioHelper::ChangeSampleVolume(bgmInstance, value);
	AudioHelper::BGMVolume = value;
	AudioHelper::ChangeSampleVolume(bgmInstance , value);
}

void SettingScene::SFXSlideOnValueChanged(float value) {
	//bgmInstance = AudioHelper::PlaySample("gun.wav", false, value);
	AudioHelper::SFXVolume = value;
}


VolumeSlider::VolumeSlider(float _x, float _y, float _w, float _h, std::string _effect, float& _targ) : Slider(_x, _y, _w, _h), effect(_effect), targ(_targ){};

void VolumeSlider::OnMouseUp(int button, int mx, int my) {
	if(Down)
		AudioHelper::PlaySample(effect, false, targ, 0);
	Down = false;
}

