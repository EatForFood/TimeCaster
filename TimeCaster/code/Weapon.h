#pragma once
#include <SFML/Graphics.hpp>
#include "Item.h"

using namespace sf;

class Weapon : public Item
{
private:
	
	int m_Damage;

public:

	Weapon(String type, sf::Vector2f position);
	int getDamage();

};



