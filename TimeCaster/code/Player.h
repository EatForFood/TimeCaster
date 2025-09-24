#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"

using namespace sf;

class Player : public Character
{
private:
	const float START_SPEED = 150;
	const float START_HEALTH = 100;
	const float START_MANA = 100;
	const float START_STAMINA = 100;
	const float START_STAMINA_RECHARGE = 0.05;

	// Which directions is the player currently moving in
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	float m_Mana;
	float m_MaxMana;
	float m_Stamina;
	float m_MaxStamina;
	float m_StaminaRecharge;

	// Dodge variables
	bool isDodging = false;
	bool canDodge = false;
	Clock dodgeClock;
	Clock cooldownClock;
	float dodgeDuration = 0.2f; // 200ms dodge
	float dodgeCooldown = 1.0f; // 1 second cooldown on dodge

	// When was the player last hit
	Time m_LastHit;
	// How many iFrames the player has after being hit
	int m_IFrames = 0;
	// If the player is moving or not
	bool m_IsMoving = false;

public:

	Player();

	// reset player to default stats
	void resetPlayerStats();

	void spawn(IntRect arena, Vector2f resolution, int tileSize);

	// Handle the player getting hit by an enemy
	bool hit(Time timeHit, float damage, int iFrames);

	// How long ago was the player last hit
	Time getLastHitTime();

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

	// Give the player a health upgrade
	void upgradeHealth();

	// Give the player a Stamina upgrade
	void upgradeStamina();

	// Give the player a Mana upgrades
	void upgradeMana();

	// Increase the maximum amount of health the player can have
	void increaseHealthLevel(int amount);

	void dodge(); // check for dodge input and handle dodge timer

	// Player starts a dodge
	void startDodge();

	// Player stops a dodge
	void stopDodge();

	void increaseManaLevel(int amount);

	void increaseStaminaLevel(int amount);

	float getStamina();
	float getMaxStamina();

	float getMana();
	float getMaxMana();

};
#pragma once
