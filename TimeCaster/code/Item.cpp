#include "Item.h"
#include "TextureHolder.h"
using namespace sf;
using namespace std;

Item::Item(String type, Vector2f position)
{
	// Store the type of this pickup
	m_Type = type;
	m_Position = position;

	// Associate the texture with the sprite
	if (m_Type == "gold")
	{
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/items/goldCoin.png"));

		// How much is pickup worth
		m_Value = 1;

		m_Sprite.setOrigin(5, 5);

	}
	
	m_SecondsSinceSpawn = 0;
	m_Spawned = true;
	m_SecondsToLive = SECONDS_TO_DESPAWN;

	m_Sprite.setPosition(m_Position);
}

FloatRect Item::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Sprite Item::getSprite()
{
	return m_Sprite;
}

bool Item::isSpawned()
{
	return m_Spawned;
}

int Item::gotIt()
{
	m_Spawned = false;
	m_SecondsSinceDeSpawn = 0;
	return m_Value;
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

		m_Sprite.setPosition(m_Position.x + deltaX, m_Position.y + deltaY);

		m_MovementElapsed += elapsedTime;
	}
}

