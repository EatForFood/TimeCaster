#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"

using namespace sf;

class Player : public Character
{
private:
	const float START_SPEED = 150;
	const float START_HEALTH = 100;

	// Which directions is the player currently moving in
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	// When was the player last hit
	Time m_LastHit;

	//weapon
	int m_Gun;

public:

	Player();

	// reset player to default stats
	void resetPlayerStats();

	void spawn(IntRect arena, Vector2f resolution, int tileSize);

	// Handle the player getting hit by an enemy
	bool hit(Time timeHit);

	// How long ago was the player last hit
	Time getLastHitTime();

	void changeGun(String gun);

	// get players current gun
	int getGun();

	// The next four functions move the player
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	// Stop the player moving in a specific direction
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();

	// We will call this function once every frame
	void update(float elapsedTime, Vector2i mousePosition);

	// Give player a speed boost
	void upgradeSpeed();

	// Give the player some health
	void upgradeHealth();

	// Increase the maximum amount of health the player can have
	void increaseHealthLevel(int amount);

	// Animate player with sprite sheet
	void updateSprite();

};
#pragma once
