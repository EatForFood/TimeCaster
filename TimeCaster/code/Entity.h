#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Entity.h"

using namespace sf;

class Entity
{
protected:

	// The sprite that represents this entity
	Sprite m_Sprite;

	Texture m_Texture;

	Vector2f m_Position;

	// Public prototypes go here
public:
	Entity();

	void spawn(String type, float x, float y);

	// Check the position of a pickup
	Vector2f getPosition();

	// Get the sprite for drawing
	const Sprite getSprite();

	// Let the pickup update itself each frame
	void update(float elapsedTime, Player player, Vector2i mousePosition);

	void setSpriteFromSheet(int row, int col, int frameWidth, int frameHeight);

};

