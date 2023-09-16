#include <allegro5/allegro_audio.h>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <sys/types.h>

#include "AudioHelper.hpp"
#include "FLPlayScene.hpp"
#include "GameEngine.hpp"
#include "Group.hpp"
#include "IScene.hpp"
#include "ImageButton.hpp"
#include "LOG.hpp"
#include "Label.hpp"
#include "PlayScene.hpp"
#include "Point.hpp"
#include "Resources.hpp"
#include "Slider.hpp"
#include "ModSel.hpp"
#include "GenArg.hpp"
#include "Collider.hpp"


bool ModSel::modSet[6];

ModButton::ModButton(int _idx, std::string img, std::string img_hov, std::string img_en, int x, int y, int w, int h) : ImageButton(img, img_hov, x, y, w, h) , State(false){
	imgEn = Engine::Resources::GetInstance().GetBitmap(img_en);
	idx = _idx;
}

void ModButton::OnMouseMove(int mx, int my) {
	mouseIn = Engine::Collider::IsPointInBitmap(Engine::Point((mx - Position.x) * GetBitmapWidth() / Size.x + Anchor.x * GetBitmapWidth(), (my - Position.y) * GetBitmapHeight() / Size.y + Anchor.y * GetBitmapHeight()), bmp);
		if (!mouseIn || !Enabled)
			bmp = ModSel::modSet[idx] ? imgEn : imgOut;
		else bmp = imgIn;

}

void ModButton::OnMouseDown(int button, int mx, int my) {
	if(!OnClickCallback) return;
	
	if((button & 1) && mouseIn) {
		State = !State;
		OnClickCallback();
		if(State) {
			bmp = imgEn;
		}
		else {
			bmp = imgOut;
		}
	}
}

void ModSel::Initialize() {
	ClearMode();
	AddNewControlObject(ModBtnGroup = new Engine::Group());
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	ModButton* btn;
	Engine::ImageButton* bt;

	/* mod scene info */
	AddNewObject(new Engine::Label("mod selection:", "pirulen.ttf", 48, w - 360, 50, 255, 255, 255, 255, 0.5, 0.5));
	AddNewObject(new Engine::Label("All mods are optional, ", "roboto.ttf", 48, w - 360, 150, 255, 255, 255, 255, 0.5, 0.5));
	AddNewObject(new Engine::Label("Pick the mods that", "roboto.ttf", 48, w - 360, 210, 255, 255, 255, 255, 0.5, 0.5));
	AddNewObject(new Engine::Label("suits your skill the most.", "roboto.ttf", 48, w - 360, 270, 255, 255, 255, 255, 0.5, 0.5));
	AddNewObject(new Engine::Label("(hover on the button to get info)", "roboto.ttf", 24, w - 360, 340, 255, 255, 255, 255, 0.5, 0.5));

	/* difficulty reduce mod */
	AddNewObject(new Engine::Label("enbrace timidity", "death.ttf", 72, 300, 50, 255, 255, 255, 255, 0.5, 0.5));

	/* ez mod: mod that reduce enemy speed, damage, hp */
	btn = new ModButton(0, "mods/mod-ez.png", "mods/mod-ez_sel.png", "mods/mod-ez_enabled.png", 100, 100, 200, 142);
	btn->SetOnClickCallback(std::bind(&ModSel::ModeToggle, this, 0));
	ModBtnGroup->AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Easy", "pirulen.ttf", 24, 200, 280, 255, 255, 255, 255, 0.5, 0.5));

	/* nf mod: the tower is indestructable, so there's no failure */
	btn = new ModButton(1, "mods/mod-nf.png", "mods/mod-nf_sel.png", "mods/mod-nf_enabled.png", 100 + 240, 100, 200, 142);
	btn->SetOnClickCallback(std::bind(&ModSel::ModeToggle, this, 1));
	ModBtnGroup->AddNewControlObject(btn);
	AddNewObject(new Engine::Label("No Fail", "pirulen.ttf", 24, 200 + 240, 280, 255, 255, 255, 255, 0.5, 0.5));

	/* rx mod: don't need to worry about financial situation, its free */
	btn = new ModButton(2, "mods/mod-rx.png", "mods/mod-rx_sel.png", "mods/mod-rx_enabled.png", 101 + 480, 100, 200, 142);
	btn->SetOnClickCallback(std::bind(&ModSel::ModeToggle, this, 2));
	ModBtnGroup->AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Relax", "pirulen.ttf", 24, 200 + 480, 280, 255, 255, 255, 255, 0.5, 0.5));
	AddNewObject(new Engine::Label("Relax", "pirulen.ttf", 24, 200 + 480, 280, 255, 255, 255, 255, 0.5, 0.5));


	/* difficulty increment mod */
	AddNewObject(new Engine::Label("be a real chad", "death.ttf", 72, 280, 360, 255, 255, 255, 255, 0.5, 0.5));

	/* hr mod: increase enemy attribute, cost increases 20% */
	btn = new ModButton(3, "mods/mod-hard.png", "mods/mod-hard_sel.png", "mods/mod-hard_enabled.png", 100, 420, 200, 142);
	btn->SetOnClickCallback(std::bind(&ModSel::ModeToggle, this, 3));
	ModBtnGroup->AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Hard", "pirulen.ttf", 24, 200, 600, 255, 255, 255, 255, 0.5, 0.5));
	
	/* pf mod: if enemy touched the tower, you failed */
	btn = new ModButton(4, "mods/mod-pf.png", "mods/mod-pf_sel.png", "mods/mod-pf_enabled.png", 100 + 240, 420, 200, 142);
	btn->SetOnClickCallback(std::bind(&ModSel::ModeToggle, this, 4));
	ModBtnGroup->AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Perfect", "pirulen.ttf", 24, 200 + 240, 600, 255, 255, 255, 255, 0.5, 0.5));

	/* fl mod: pure darkness until it's in the turret distance */
	btn = new ModButton(5, "mods/mod-fl.png", "mods/mod-fl_sel.png", "mods/mod-fl_enabled.png", 100 + 480, 420, 200, 142);
	btn->SetOnClickCallback(std::bind(&ModSel::ModeToggle, this, 5));
	ModBtnGroup->AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Flashlight", "pirulen.ttf", 24, 200 + 480, 600, 255, 255, 255, 255, 0.5, 0.5));

	/* back to select scene */
	bt = new Engine::ImageButton("mods/btn.png", "mods/btn_hov.png", 100 + w - 700, h - 200, 400, 100);
	AddNewControlObject(bt);
	bt->SetOnClickCallback(std::bind(&ModSel::BackOnClick, this));
	AddNewObject(new Engine::Label("back", "pirulen.ttf", 60, w - 440, h - 150, 0, 0, 0, 255, 0.5, 0.5));

	/* submit mod selection and enter the selected level */
	bt = new Engine::ImageButton("mods/btn.png", "mods/btn_hov.png", 100 + w - 700, h - 400, 400, 100);
	bt->SetOnClickCallback(std::bind(&ModSel::PlayOnClick, this));
	AddNewControlObject(bt);
	AddNewObject(new Engine::Label("go", "pirulen.ttf", 60, w - 440, h - 350, 0, 0, 0, 255, 0.5, 0.5));

	bgmInstance = AudioHelper::PlaySample("edge.oga", true, AudioHelper::BGMVolume*0.8, 0);
}

void ModSel::OnMouseMove(int mx, int my) {
	IScene::OnMouseMove(mx, my);
}

void ModSel::OnMouseDown(int button, int mx, int my) {
	IScene::OnMouseDown(button, mx, my);
	ModBtnGroup->OnMouseMove(mx, my);
	AudioHelper::PlayAudio("clicked.ogg");
}

void ModSel::Terminate() {
	AudioHelper::StopSample(bgmInstance);
	bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
	IScene::Terminate();
}

void ModSel::PlayOnClick() {
	if(!modSet[5]) {
		PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
		if(modSet[0]) Attributes::gamemode = Attributes::MOD_EZ_ENABLED;
		else if(modSet[3]) Attributes::gamemode = Attributes::MOD_HR_ENABLED;
		else Attributes::gamemode = Attributes::NOMOD;
		std::cout << Attributes::gamemode << std::endl;

		scene->isNF = modSet[1];
		scene->isPF = modSet[4];
		scene->isRX = modSet[2];
		scene->MapId = levelToload;
		Engine::GameEngine::GetInstance().ChangeScene("play");
	}
	else {
		PlayScene* scene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("fl-play"));
		if(modSet[0]) Attributes::gamemode = Attributes::MOD_EZ_ENABLED;
		else if(modSet[3]) Attributes::gamemode = Attributes::MOD_HR_ENABLED;
		else Attributes::gamemode = Attributes::NOMOD;
		scene->isNF = modSet[1];
		scene->isPF = modSet[4];
		scene->isRX = modSet[2];
		scene->MapId = levelToload;
		Engine::GameEngine::GetInstance().ChangeScene("fl-play");
	}
}

void ModSel::BackOnClick() {
	Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void ModSel::SettingOnClick() {
	Engine::GameEngine::GetInstance().ChangeScene("setting");
}

void ModSel::ModeToggle(int i) {
	ModSel::modSet[i] = !ModSel::modSet[i];
	if(i % 3 == 2) return;
	if(ModSel::modSet[(i+3)%6])
		ModSel::modSet[(i+3)%6] = false;
}


void ModSel::ClearMode() {
	memset(ModSel::modSet, 0, sizeof(ModSel::modSet));
}
