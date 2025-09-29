#pragma once
#include <SFML/Graphics.hpp>
#include "Item.h"

using namespace sf;

class Equipment : public Item
{
private:
	
	int m_Armour;

public:

	Equipment(String type, sf::Vector2f position);

};



