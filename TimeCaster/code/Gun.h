#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

using namespace sf;

class Gun
{
private:

	const int START_WAIT_TIME = 10;
	const int START_SECONDS_TO_LIVE = 5;

	// The sprite that represents this pickup
	Sprite m_Sprite;

	// The arena it exists in
	IntRect m_Arena;

	Vector2f m_Resolution;

	// How much is this pickup worth?
	int m_Value;

	// What type of pickup is this? 
	// 1 = health, 2 = ammo
	String m_Type;

	// Handle spawning and disappearing
	bool m_Spawned;
	float m_SecondsSinceSpawn;
	float m_SecondsSinceDeSpawn;
	float m_SecondsToLive;
	float m_SecondsToWait;

	// Public prototypes go here
public:

	Gun();

	// Prepare a new pickup
	void setArena(IntRect arena);

	void spawn(String type, float x, float y, Vector2f resolution);

	// Check the position of a pickup
	FloatRect getPosition();

	// Get the sprite for drawing
	Sprite getSprite();

	// Let the pickup update itself each frame
	void update(float elapsedTime, Player player, Vector2i mousePosition);

	// Is this pickup currently spawned?
	bool isSpawned();

	// Get the goodness from the pickup
	int gotIt();

	// Upgrade the value of each pickup
	void upgrade();

};
