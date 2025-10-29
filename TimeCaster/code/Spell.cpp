#include "Spell.h"
#include "Decal.h"

// The constructor
Spell::Spell()
{
	m_SpellShape.setSize(sf::Vector2f(2, 2));
}

void Spell::shoot(float startX, float startY, float targetX, float targetY)
{
	// Keep track of the spell
	m_InFlight = true;
	m_Position.x = startX;
	m_Position.y = startY;

	// Calculate the gradient of the flight path
	float gradient = (startX - targetX) / (startY - targetY);

	// Any gradient less than zero needs to be negative
	if (gradient < 0)
	{
		gradient *= -1;
	}

	// Calculate the ratio between x and t
	float ratioXY = m_SpellSpeed / (1 + gradient);

	// Set the "speed" horizontally and vertically
	m_SpellDistanceY = ratioXY;
	m_SpellDistanceX = ratioXY * gradient;
	
	// Point the bullet in the right direction
	if (targetX < startX)
	{
		m_SpellDistanceX *= -1;
	}

	if (targetY < startY)
	{
		m_SpellDistanceY *= -1;
	}

	// Finally, assign the results to the
	// member variables
	m_XTarget = targetX;
	m_YTarget = targetY;

	// Set a max range of 1000 pixels
	float range = 1000;
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;
	
	// Position the spell ready to be drawn
	m_SpellShape.setPosition(m_Position);
}

void Spell::stop()
{
	m_InFlight = false;
}

bool Spell::isInFlight()
{
	return m_InFlight;
}

FloatRect Spell::getPosition()
{
	return m_SpellShape.getGlobalBounds();
}

RectangleShape Spell::getShape()
{
	return m_SpellShape;
}

void Spell::update(float elapsedTime)
{
	// Update the spell position variables
	m_Position.x += m_SpellDistanceX * elapsedTime;
	m_Position.y += m_SpellDistanceY * elapsedTime;

	// Move the spell
	m_SpellShape.setPosition(m_Position);

	// Has the spell gone out of range?
	if (m_Position.x < m_MinX || m_Position.x > m_MaxX ||
		m_Position.y < m_MinY || m_Position.y > m_MaxY)
	{
		m_InFlight = false;
	}
}

float Spell::bGetX()
{
	return m_Position.x;
}

float Spell::bGetY()
{
	return m_Position.y;
}