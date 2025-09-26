#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

using namespace sf;

class Entity
{
protected:

	// The sprite that represents this entity
	Sprite m_Sprite;

	Texture m_Texture;

	Vector2f m_Position;

public:
	Entity();

	void spawn(String type, float x, float y);

	// Check the position of the entity
	Vector2f getPosition();

	// Get the sprite for drawing
	const Sprite getSprite();

};

