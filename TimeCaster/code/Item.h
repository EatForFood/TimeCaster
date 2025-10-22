#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;
#include <string>

class Item
{
public:

	enum ItemType 
	{ MeleeWeapon, MagicWeapon, 
		Consumable, 
		HeadArmour, ChestArmour, TrouserArmour, ShoeArmour, NeckArmour, 
		null };

	Item(String type, Vector2f position);

	ItemType getType();



	// Check the position of a pickup
	FloatRect getPosition();

	IntRect getTextureRect();

	// Get the sprite for drawing
	Sprite getSprite();

	Sprite& getIcon();

	int getValue();

	// Let the pickup update itself each frame
	void update(float elapsedTime);

	// Is this pickup currently spawned?
	bool isSpawned();

	bool isNull();

	string getName();

	// Get the goodness from the pickup
	int gotIt();

protected:

	int m_Value;
	float m_Weight; 
	int SECONDS_TO_DESPAWN = 120;
	String m_Name;



	ItemType m_ItemType;

	// The sprite that represents this pickup
	Sprite m_Sprite;
	
	Sprite m_Icon;

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

	

};



