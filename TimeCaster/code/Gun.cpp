#include "Gun.h"
#include "TextureHolder.h"
#include "Player.h"
using namespace sf;

Gun::Gun()
{
	// Store the type of this pickup
	//m_Type = type;

	// Associate the texture with the sprite
	/*
	if (m_Type == "bloodImpact")
	{
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/bloodImpact.png"));

		// How much is pickup worth
		

	}
	else
	{
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/ammo_pickup.png"));

		// How much is pickup worth
		
	}

	m_Sprite.setOrigin(25, 25);

	*/

	m_SecondsToLive = START_SECONDS_TO_LIVE;
	m_SecondsToWait = START_WAIT_TIME;
}

void Gun::setArena(IntRect arena)
{

	// Copy the details of the arena to the pickup's m_Arena
	m_Arena.left = arena.left + 50;
	m_Arena.width = arena.width - 50;
	m_Arena.top = arena.top + 50;
	m_Arena.height = arena.height - 50;

	//spawn();
}

void Gun::spawn(String type, float x, float y, Vector2f resolution)
{

	if (type == "pistol")
	{
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/guns/colt_45.png"));

		


	}
	else
	{
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/bloodImpat.png"));

		

	}
	// Spawn on zombie

	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;

	// Not currently spawned
	//m_Spawned = false;
	m_SecondsSinceSpawn = 0;
	m_Spawned = true;

	m_Sprite.setPosition(x,y);
}

FloatRect Gun::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Sprite Gun::getSprite()
{
	return m_Sprite;
}

bool Gun::isSpawned()
{
	return m_Spawned;
}

int Gun::gotIt()
{
	m_Spawned = false;
	m_SecondsSinceDeSpawn = 0;
	return m_Value;
}

void Gun::update(float elapsedTime, Player player, Vector2i mousePosition)
{
	m_Sprite.setPosition(player.getCenter());

	float angle = (atan2(mousePosition.y - m_Resolution.y / 2,
		mousePosition.x - m_Resolution.x / 2)
		* 180) / 3.141;

	m_Sprite.setRotation(angle);

}

void Gun::upgrade()
{
	if (m_Type == "bloodImpact")
	{
		
	}
	else
	{
		
	}

	// Make them more frequent and last longer
	m_SecondsToLive += (START_SECONDS_TO_LIVE / 10);
	m_SecondsToWait -= (START_WAIT_TIME / 10);
}
