#include "Item.h"
#include "TextureHolder.h"
#include <iostream>
using namespace sf;
using namespace std;


Item::Item(String type, Vector2f position)
{
	// Store the type of this pickup
	m_Name = type;
	m_Position = position;

	if (m_Name == "null") // empty item for inventory
	{
		//m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/goldCoin.png"));

		// How much is item worth

		/*
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(961, 896, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(961, 896, 32, 32));
		*/
		m_Value = 0;

		m_Sprite.setOrigin(5, 5);

		//m_NullItem = true;

		m_ItemType = null;

	}

	if (m_Name == "gold")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/goldCoin.png"));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/goldCoinIcon.png"));
		//m_Icon.setScale(5,5);
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

		// How much is item worth
		m_Value = 1;

		m_Sprite.setOrigin(5, 5);

		//m_NullItem = false;
		m_ItemType = Consumable;
	}

	if (m_Name == "Health_Potion")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(66, 800, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(66, 800, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 10;
	
		m_ItemType = Consumable;
		m_Sprite.setOrigin(25, 25);
	}
	if (m_Name == "Mana_Potion")
	{

		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(480, 1472, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(480, 1472, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 15;

		m_ItemType = Consumable;
		m_Sprite.setOrigin(25, 25);
	}
	if (m_Name == "")


	m_SecondsSinceSpawn = 0;
	m_Spawned = true;
	m_SecondsToLive = SECONDS_TO_DESPAWN;

	m_Sprite.setPosition(m_Position);

	m_MovementElapsed = 0;
	m_MovementDuration = 0.5 + (rand() % 50) / 100;

	// Random direction and speed
	float angle = (rand() % 360) * 3.14159 / 180;
	float speed = 20 + (rand() % 50); 

	m_Velocity.x = cos(angle) * speed;
	m_Velocity.y = sin(angle) * speed;
}

FloatRect Item::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Sprite Item::getSprite()
{
	return m_Sprite;
}

Sprite& Item::getIcon()
{

	return m_Icon;
}

IntRect Item::getTextureRect()
{
	return m_Sprite.getTextureRect();
}

int Item::getValue()
{
	return m_Value;
}

bool Item::isSpawned()
{
	return m_Spawned;
}

bool Item::isNull()
{
	if (m_ItemType == null)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Item::gotIt()
{
	m_Spawned = false;
	m_SecondsSinceDeSpawn = 0;
	return m_Value;
}

string Item::getName()
{
	return m_Name;
}

void Item::update(float elapsedTime)
{
	if (m_Spawned)
	{
		m_SecondsSinceSpawn += elapsedTime;
	}

	// Do we need to hide a pickup?
	if (m_SecondsSinceSpawn > m_SecondsToLive && m_Spawned)
	{
		// Revove the pickup and put it somewhere else
		m_Spawned = false;
		m_SecondsSinceDeSpawn = 0;
		
	}

	if (m_MovementElapsed < m_MovementDuration)
	{
		float progress = m_MovementElapsed / m_MovementDuration;
		float damping = 1.0f - progress; // decreases from 1 to 0

		float deltaX = m_Velocity.x * damping * elapsedTime;
		float deltaY = m_Velocity.y * damping * elapsedTime;

		m_Position.x += deltaX;
		m_Position.y += deltaY;

		m_Sprite.setPosition(m_Position.x, m_Position.y);

		m_MovementElapsed += elapsedTime;
	}
}

Item::ItemType Item::getType()
{
	return m_ItemType;
}

int Item::getDamage()
{
	return m_Damage;
}

int Item::getArmour()
{
	return m_Armour;
}