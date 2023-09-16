#ifndef PLAYSCENE_FL_HPP
#define PLAYSCENE_FL_HPP

#include "IScene.hpp"
#include <allegro5/bitmap.h>
#include <allegro5/bitmap_lock.h>
#include <memory>
#include <vector>
#include "PlayScene.hpp"
#include "Point.hpp"



class FlPlayScene: public PlayScene {
private:
	ALLEGRO_SAMPLE_ID bgmId;
	std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> deathBGMInstance;
	std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
	ALLEGRO_BITMAP* blind;
public:
	class Mask {
		unsigned char* raw;
		public:
		ALLEGRO_LOCKED_REGION* workArea;
		std::vector<unsigned char>** occuList;
		void DoMask(ALLEGRO_BITMAP* bg, int cx, int cy, int rd);
		void UnMask(ALLEGRO_BITMAP* bg, int cx, int cy, int rd);
		void Repair(ALLEGRO_BITMAP* bg, int x, int y, int e=1000);
		void SetBit(int x, int y);
		void UnSetBit(int x, int y, int i);
		int maskW;
		int maskH;

	} msk;
	void Initialize() override;
	void Draw() const override;
	void OnMouseUp(int button, int mx, int my) override;
	void Terminate() override;
};

#endif
