#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Decal
{
private:

	// The sprite that represents this decal
	Sprite m_Sprite;
	// What type of decal is this? 
	String m_Type;

	// Handle spawning and disappearing
	bool m_Spawned;

	
public:


	void spawn(String type, float x, float y);

	// Get the sprite for drawing
	Sprite getSprite();
};
