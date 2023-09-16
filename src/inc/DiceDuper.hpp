#ifndef DICE_DUPER_HPP
#define DICE_DUPER_HPP
#include "Enemy.hpp"
#include "ImADice.hpp"

class DiceDuper: public Enemy {
private:
	int genNum;
	void generate(int x, int y) ;
public:
	static DiceDuper* create(int x, int y, int num);
	DiceDuper(std::string img, int x, int y,int num);
	virtual void Hit(float damage) override;
};


#endif
