#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;
#include <string>

class Item
{
protected:

	int m_Value;
	float m_Weight; 
	int SECONDS_TO_DESPAWN = 120;
	String m_Type;

	// The sprite that represents this pickup
	Sprite m_Sprite;

	// Handle spawning and disappearing
	bool m_Spawned;
	float m_SecondsSinceSpawn;
	float m_SecondsSinceDeSpawn;
	float m_SecondsToLive;
	float m_SecondsToWait;

	float m_MovementElapsed = 0;
	float m_MovementDuration = 2;

	Vector2f m_Velocity;

	Vector2f m_Position;

	// Public prototypes go here
public:

	Item(String type, sf::Vector2f position);

	// Check the position of a pickup
	FloatRect getPosition();

	// Get the sprite for drawing
	Sprite getSprite();

	int getValue();

	// Let the pickup update itself each frame
	void update(float elapsedTime);

	// Is this pickup currently spawned?
	bool isSpawned();

	// Get the goodness from the pickup
	int gotIt();

};



