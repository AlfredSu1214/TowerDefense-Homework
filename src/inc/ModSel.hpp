#ifndef MODSEL_HPP
#define MODSEL_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include <string>
#include "IControl.hpp"
#include "IScene.hpp"
#include "Image.hpp"
#include "ImageButton.hpp"
#include "StageSelectScene.hpp"

class ModButton final : public Engine::ImageButton {
	private:
		std::shared_ptr<ALLEGRO_BITMAP> imgEn;
		int idx;
		bool State;
	public:
	void OnMouseDown(int button, int mx, int my) override;
	ModButton(int idx, std::string img, std::string img_hov, std::string img_en, int x, int y, int w, int h);
	void OnMouseMove(int mx, int my) override;

};

class ModSel final : public Engine::IScene {
	friend StageSelectScene;
private:
	std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
protected:
	int levelToload;
public:
	static bool modSet[6];
	Group* ModBtnGroup;
	explicit ModSel() = default;
	void Initialize() override;
	void Terminate() override;
	void PlayOnClick();
	void BackOnClick();
	void SettingOnClick();
	void ModeToggle(int i);
	void ClearMode();
	void OnMouseMove(int mx, int my) override;
	void OnMouseDown(int button, int mx, int my) override;
};


#endif // STAGESELECTSCENE_HPP
