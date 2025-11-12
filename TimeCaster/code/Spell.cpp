#include "Spell.h"
#include "Decal.h"
#include "TextureHolder.h"
using namespace std;

// The constructor
Spell::Spell()
{
	m_Sprite = Sprite(TextureHolder::GetTexture("graphics/spells/fireball.png"));
	m_Sprite.setOrigin(32, 32);
	m_Sprite.setScale(0.75, 0.75);
	// Set spell damage to 15 for now
	m_SpellDamage = 15;

	// Set piercing properties to false for now
	m_PierceEnemy = false;
	m_PierceObject = false;

	
}

void Spell::shoot(float startX, float startY, float targetX, float targetY, float spellDamage)
{
	// Take damage based off of wand
	m_SpellDamage = spellDamage;

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

	// angle spell towards target
	float dx = targetX - startX;
	float dy = targetY - startY;

	// Compute the angle of travel in degrees
	float angle = std::atan2(dy, dx) * 180.f / 3.14159265f;

	// Rotate the sprite
	m_Sprite.setRotation(angle);

	m_Hitbox.left = m_Position.x - m_HitboxWidth / 2;
	m_Hitbox.width = m_HitboxWidth;
	m_Hitbox.top = m_Position.y - m_HitboxHeight / 2;
	m_Hitbox.height = m_HitboxHeight;
	
	// Position the spell ready to be drawn
	setSpriteFromSheet(IntRect(0,0,512,64), 64);
	m_Sprite.setPosition(m_Position);
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

void Spell::update(float elapsedTime, const vector<NavBox>& navBox)
{
	m_TimeElapsed = elapsedTime;

	// Update the spell position variables
	m_Position.x += m_SpellDistanceX * elapsedTime;
	m_Position.y += m_SpellDistanceY * elapsedTime;

	// Move the spell
	m_Sprite.setPosition(m_Position);

	m_Hitbox.left = m_Position.x - m_HitboxWidth / 2;
	m_Hitbox.width = m_HitboxWidth;
	m_Hitbox.top = m_Position.y - m_HitboxHeight / 2;
	m_Hitbox.height = m_HitboxHeight;

	// Has the spell gone out of range?
	if (m_Position.x < m_MinX || m_Position.x > m_MaxX ||
		m_Position.y < m_MinY || m_Position.y > m_MaxY)
	{
		m_InFlight = false;
	}

	m_CollisionBox.left = m_Position.x - 100;
	m_CollisionBox.top = m_Position.y - 100;
	m_CollisionBox.width = 200;
	m_CollisionBox.height = 200;

	for (auto& nav : navBox) { // if player walks into navBox 
		if (m_CollisionBox.intersects(nav.getShape().getGlobalBounds()))
		{
			if (collision.pointInShape(m_Position, nav.getShape())) {
				m_InFlight = false;
			}
		}
	}

	moveTextureRect();

}

float Spell::bGetX()
{
	return m_Position.x;
}

float Spell::bGetY()
{
	return m_Position.y;
}

void Spell::setSpriteFromSheet(sf::IntRect textureBox, int tileSize) // set sprite
{
	int tile_size = tileSize;
	sheetCoordinate = sf::Vector2i(textureBox.left, textureBox.top);
	spriteSize = sf::Vector2i(tile_size, tile_size);
	if (textureBox.width > tile_size)
	{
		m_Animation_It_Limit = textureBox.width / tile_size;

		m_Horizontal = true;
	}
	else if (textureBox.height > tile_size)
	{
		m_Animation_It_Limit = textureBox.height / tile_size;
		m_Horizontal = false;
	}
	else
		throw std::logic_error("Animation bounding box must contain multiply sprites, setSprite(sf::IntRect )\n");

	//	cout << "setSpriteFromSheet  " << "animation_it limit" << m_Animation_It_Limit << "\n";
	//	cout << "setSpriteFromSheet func " << "m_Ani_Counter " << m_Ani_Counter << "\n";

	m_Sprite.setTextureRect(sf::IntRect{ sheetCoordinate, spriteSize });
}

void Spell::moveTextureRect() // animate sprite by moving texRect location
{
	//	cout << " Move Texture Rect m_Ani_Counter " << m_Ani_Counter << "\n";
	// if the animation counter is greater than the animation limit reset to 1;
	if (m_Ani_Counter == m_Animation_It_Limit)
	{
		m_Ani_Counter = 0;
	}

	if (m_Horizontal) {

		m_Sprite.setTextureRect(sf::IntRect(sheetCoordinate + sf::Vector2i(spriteSize.x * m_Ani_Counter, 0), spriteSize));
	}
	else {
		m_Sprite.setTextureRect(sf::IntRect(sheetCoordinate + sf::Vector2i(0, spriteSize.y * m_Ani_Counter), spriteSize));
	}

	//increment animation counter to point to the next frame
	double timePerFrame;
	timePerFrame = 1.0 / 6.0;
	m_AnimationTimer = m_AnimationTimer + m_TimeElapsed;
	if (m_AnimationTimer > timePerFrame)
	{
		m_Ani_Counter++;
		m_AnimationTimer = 0;
	}
}

Sprite Spell::getSprite()
{
	return m_Sprite;
}

// This returns false if spell does 0 damage (non-damaging spell)
float Spell::getSpellDamage()
{
	return m_SpellDamage;
}

bool Spell::piercesEnemy()
{
	return m_PierceEnemy;
}

bool Spell::piercesObject()
{
	return m_PierceObject;
}

FloatRect Spell::getHitbox()
{
	return m_Hitbox;
}