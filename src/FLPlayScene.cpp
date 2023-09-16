#include <allegro5/allegro.h>
#include <allegro5/bitmap.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/bitmap_lock.h>
#include <allegro5/allegro_image.h>
#include <allegro5/color.h>
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <cstring>
#include <iterator>
#include <random>
#include <fstream>
#include <functional>
#include <list>
#include <vector>
#include <queue>
#include <string>
#include <memory>

#include "FLPlayScene.hpp"
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

typedef struct ARGB {
	unsigned char a;
	unsigned char b;
	unsigned char g;
	unsigned char r;
} ARGB;


void FlPlayScene::Mask::SetBit(int x, int y) {
	ARGB* p = (ARGB*)(raw + (y * workArea->pitch) + (x * workArea->pixel_size));
	p->r = 0; p->g = 0; p->b = 0; p->a =255;
}

void FlPlayScene::Mask::UnSetBit(int x, int y, int i) {
	ARGB* p = (ARGB*)(raw + (y * workArea->pitch) + (x * workArea->pixel_size));
	p->r = 0; p->g = 0; p->b = 0; p->a =0;
	//occuCount[y][x] += 1;
}

void FlPlayScene::Mask::Repair(ALLEGRO_BITMAP *bg, int x, int y, int e) {
	workArea = al_lock_bitmap(bg, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_READWRITE);
	raw = (unsigned char*)workArea->data;
	int startX = x+1250, startY=y+1250;
	int endX = startX+e, endY = startY+e;
	for(int i=startX; i<=endX; i++)
		for(int j=startY; j<=endY; j++) {
					ARGB* p = (ARGB*)(raw + (j * workArea->pitch) + (i * workArea->pixel_size));
					p->r = 0; p->g = 0; p->b = 0; p->a = occuList[j][i].back();

		}
	al_unlock_bitmap(bg);
}


void FlPlayScene::Mask::DoMask(ALLEGRO_BITMAP *bg, int cx, int cy, int rd) {
	workArea = al_lock_bitmap(bg, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_READWRITE);
	raw = (unsigned char*)workArea->data;
	int partR = rd*0.6;
	int endR = partR+50;
	int startX = cx-endR+1250;
	int startY = cy-endR+1250;
	int endX = cx+endR+1250;
	int endY = cy+endR+1250;
	int eSqr = endR*endR;
	int posCx = cx+1250;
	int posCy = cy+1250;
	for(int y = startY; y <= endY; y++)
		for(int x = startX; x <= endX; x++) {
			if((x-posCx)*(x-posCx) + (y-posCy)*(y-posCy) <= eSqr) {
					ARGB* p = (ARGB*)(raw + (y * workArea->pitch) + (x * workArea->pixel_size));
					p->r = 0; p->g = 0; p->b = 0; p->a = 255;
			
			/*if(occuList[y][x].size() == 2) {
					ARGB* p = (ARGB*)(raw + (y * workArea->pitch) + (x * workArea->pixel_size));
					p->r = 0; p->g = 0; p->b = 0; p->a = 255;
					occuList[y][x].pop_back();
			}
			else if(occuList[y][x].size() > 2) {
					occuList[y][x].pop_back();
			}*/
			}
		}
	al_unlock_bitmap(bg);
}

void FlPlayScene::Mask::UnMask(ALLEGRO_BITMAP *bg, int cx, int cy, int rd) {

	/* brute force go brrrrrr */
	/* the actual mask has 1250 px offset */

	workArea = al_lock_bitmap(bg, ALLEGRO_PIXEL_FORMAT_RGBA_8888, ALLEGRO_LOCK_READWRITE);
	raw = (unsigned char*)workArea->data;
	int partR = rd * 0.6;
	std::cout << partR << std::endl;
	int endR = partR+50;
	int startX = cx-endR+1250;
	int startY = cy-endR+1250;
	int endX = cx+endR+1250;
	int endY = cy+endR+1250;
	int eSqr = endR*endR;
	int rsqur = partR*partR;
	int posCx = cx+1250;
	int posCy = cy+1250;
	for(int y = startY; y <= endY; y++) {
		for(int x = startX; x <= endX; x++) {
			int sqr = (x-posCx)*(x-posCx) + (y-posCy)*(y-posCy);
			/*if(sqr <= eSqr) {
				ARGB* p = (ARGB*)(raw + (y * workArea->pitch) + (x * workArea->pixel_size));
				if(p->a >= line[sqr]) p->a = line[sqr];
			}*/
			
			int na;
			if(sqr <= rsqur) {
			//occuList[y][x].push_back(0);
				ARGB* p = (ARGB*)(raw + (y * workArea->pitch) + (x * workArea->pixel_size));
				p->r = 0; p->g = 0; p->b = 0; p->a =0;
			}
			else if(sqr < eSqr) {
				ARGB* p = (ARGB*)(raw + (y * workArea->pitch) + (x * workArea->pixel_size));
				p->r = 0; p->g = 0; p->b = 0; 
				na = 255*(((std::sqrt(sqr*1.0)-partR)/50));
				p->a = na < p->a ? na : p->a;
				//occuList[y][x].push_back(p->a);
			}
		}
	}

	al_unlock_bitmap(bg);
}

void FlPlayScene::Initialize() {
	PlayScene::Initialize();
	blind = al_load_bitmap("resources/images/play/blind.png");
	msk.maskW = al_get_bitmap_width(blind);
	msk.maskH = al_get_bitmap_height(blind);
	msk.occuList = new std::vector<unsigned char>*[msk.maskH+1];
	std::cout << msk.maskH << ' ' << msk.maskW << std::endl;
	/*for(int i=0; i<=msk.maskH; i++) {
		msk.occuList[i] = new std::vector<unsigned char>[msk.maskW+1];
		for(int j=0; j<=msk.maskW; j++)
			msk.occuList[i][j].push_back(255);
	}*/


}

void FlPlayScene::Draw() const{
	TileMapGroup->Draw();
	GroundEffectGroup->Draw();
	BulletGroup->Draw();
	TowerGroup->Draw();
	EnemyGroup->Draw();
	ObstacleGroup->Draw();
	EffectGroup->Draw();
	

	al_draw_scaled_bitmap(blind, 1250, 1250, 1600, 832, 0, 0, 1600, 832, 0);
	UIGroup->Draw();
}

void FlPlayScene::Terminate() {
	/*for(int i=0; i<=msk.maskH; i++) {
		delete[] msk.occuList[i];
	}
	delete[] msk.occuList;*/
	PlayScene::Terminate();
}

void FlPlayScene::OnMouseUp(int button, int mx, int my) {
	IScene::OnMouseUp(button, mx, my);
	if (!imgTarget->Visible) {
		Moving = false;
		return;
	}
	const int x = mx / BlockSize;
	const int y = my / BlockSize;
	if (button & 1) {
		if(preview->GetPrice() == Mover::Price) {
			if(mapState[y][x] != TILE_DIRT && mapState[y][x] != TILE_FLOOR) {
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
				msk.DoMask(blind, preview->Position.x, preview->Position.y, preview->CollisionRadius);
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
				if(money > 0) {
					EarnMoney(-INFINITY);
					UIGroup->AddNewObject(new Engine::Label("moving is cheating", "death.ttf", 30, 1440, 780, 255, 0, 0, 255, 0.5, 0.5));
				}
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
			msk.DoMask(blind, rmTarg->Position.x, rmTarg->Position.y, rmTarg->CollisionRadius);
			if(!isRX)
				EarnMoney(-0.7*rmTarg->GetPrice());
			TowerGroup->RemoveObject(rmTarg->GetObjectIterator());
			auto lt = TowerGroup->GetObjects();
			//msk.Repair(blind, X-400, Y-400);
			for(auto i: lt) {
				auto tt = dynamic_cast<Turret*>(i);
				if(tt->Position.x > X-400 && tt->Position.x < X+400)
					if(tt->Position.y > Y-400 && tt->Position.y < Y+400)
						msk.UnMask(blind, tt->Position.x, tt->Position.y, tt->CollisionRadius);
			}

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

		if (mapState[y][x] != TILE_OCCUPIED) {
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
			msk.UnMask(blind, preview->Position.x, preview->Position.y, preview->CollisionRadius);
			if(preview->GetPrice() == TurretMG::Price[Attributes::gamemode])
				mapState[y][x] = TILE_OCCU_MG;
			else if(preview->GetPrice() == CyclerTurret::Price[Attributes::gamemode]) {
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
