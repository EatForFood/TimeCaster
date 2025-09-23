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
	const float START_STAMINA_RECHARGE = 0.33;

	IntRect m_Hitbox;


	IntRect room1;
    IntRect room2;
	IntRect room3;
	IntRect room4;

	IntRect door1;
	IntRect door2;
	IntRect door3;

	
	// Where is the player
	Vector2f m_Position;

	Vector2f m_PositionLast;

	// Where the bullet should spawn from
	Vector2f m_Bullet_Spawn;

	// Of course we will need a sprite
	Sprite m_Sprite;

	// And a texture
	// !!Watch this space!!
	Texture m_Texture;

	// What is the screen resolution
	Vector2f m_Resolution;

	// What size is the current arena
	IntRect m_Arena;

	// How big is each tile of the arena
	int m_TileSize;

	// Which directions is the player currently moving in
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	// How much health has the player got?
	int m_Health;
	// What is the maximum health the player can have
	int m_MaxHealth;

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

	// Where is the player
	FloatRect getPosition();

	// Where is the center of the player
	Vector2f getCenter();

	// Where is the origin of the player
	Vector2f getOrigin();

	// Which angle is the player facing
	float getRotation();

	// Send a copy of the sprite to main
	Sprite getSprite();

	// How much health has the player currently got?
	int getHealth();

	// get players current room
	int getRoom();

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

	// Give the player a health upgrade
	void upgradeHealth();

	// Give the player a Stamina upgrade
	void upgradeStamina();

	// Give the player a Mana upgrades
	void upgradeMana();

	// Increase the maximum amount of health the player can have
	void increaseHealthLevel(int amount);


	// Player starts a dodge
	void startDodge();

	// Player stops a dodge
	void stopDodge();
};
#pragma once
