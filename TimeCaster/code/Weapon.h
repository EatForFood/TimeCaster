#pragma once
#include <SFML/Graphics.hpp>
#include "Item.h"

using namespace sf;

class Weapon : public Item
{
private:
	string m_AnimationType;
	int m_WeaponSize = 1;

public:

	Weapon(string type, sf::Vector2f position);
	int getDamage();
	string getName();
	string getAnimType();
	int getWeaponSize();
};



