#pragma once
#include <SFML/Graphics.hpp>
#include "Item.h"

using namespace sf;

class Equipment : public Item
{
public:

	Equipment(String type, sf::Vector2f position);
	int getArmour();
	string getName();
};