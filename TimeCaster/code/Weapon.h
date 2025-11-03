#pragma once
#include <SFML/Graphics.hpp>
#include "Item.h"

using namespace sf;

class Weapon : public Item
{
private:
	string m_AnimationType;

public:

	Weapon(String type, sf::Vector2f position);
	int getDamage();
	string getName();
	string getAnimType();
};



