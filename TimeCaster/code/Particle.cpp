#include "Particle.h"
#include "Decal.h"
#include "TextureHolder.h"
using namespace std;

// The constructor
Particle::Particle()
{
	m_Sprite = Sprite(TextureHolder::GetTexture("graphics/Particles/fireball.png"));
	m_Sprite.setOrigin(32, 32);
	m_Sprite.setScale(0.75, 0.75);



}
//
//void Particle::shoot(float startX, float startY, float targetX, float targetY, float ParticleDamage)
//{
//	// Take damage based off of wand
//	m_ParticleDamage = ParticleDamage;
//
//	// Keep track of the Particle
//	m_InFlight = true;
//	m_Position.x = startX;
//	m_Position.y = startY;
//
//	// Calculate the gradient of the flight path
//	float gradient = (startX - targetX) / (startY - targetY);
//
//	// Any gradient less than zero needs to be negative
//	if (gradient < 0)
//	{
//		gradient *= -1;
//	}
//
//	// Calculate the ratio between x and t
//	float ratioXY = m_ParticleSpeed / (1 + gradient);
//
//	// Set the "speed" horizontally and vertically
//	m_ParticleDistanceY = ratioXY;
//	m_ParticleDistanceX = ratioXY * gradient;
//
//	// Point the bullet in the right direction
//	if (targetX < startX)
//	{
//		m_ParticleDistanceX *= -1;
//	}
//
//	if (targetY < startY)
//	{
//		m_ParticleDistanceY *= -1;
//	}
//
//	// Finally, assign the results to the
//	// member variables
//	m_XTarget = targetX;
//	m_YTarget = targetY;
//
//	// Set a max range of 1000 pixels
//	float range = 1000;
//	m_MinX = startX - range;
//	m_MaxX = startX + range;
//	m_MinY = startY - range;
//	m_MaxY = startY + range;
//
//	// angle Particle towards target
//	float dx = targetX - startX;
//	float dy = targetY - startY;
//
//	// Compute the angle of travel in degrees
//	float angle = std::atan2(dy, dx) * 180.f / 3.14159265f;
//
//	// Rotate the sprite
//	m_Sprite.setRotation(angle);
//
//	m_Hitbox.left = m_Position.x - m_HitboxWidth / 2;
//	m_Hitbox.width = m_HitboxWidth;
//	m_Hitbox.top = m_Position.y - m_HitboxHeight / 2;
//	m_Hitbox.height = m_HitboxHeight;
//
//	// Position the Particle ready to be drawn
//	setSpriteFromSheet(IntRect(0, 0, 512, 64), 64);
//	m_Sprite.setPosition(m_Position);
//}
//
//void Particle::stop()
//{
//	m_InFlight = false;
//}
//
//bool Particle::isInFlight()
//{
//	return m_InFlight;
//}

FloatRect Particle::getPosition()
{
	return m_ParticleShape.getGlobalBounds();
}

RectangleShape Particle::getShape()
{
	return m_ParticleShape;
}

void Particle::update(float elapsedTime)
{
	m_TimeElapsed = elapsedTime;

	moveTextureRect();

}

float Particle::bGetX()
{
	return m_Position.x;
}

float Particle::bGetY()
{
	return m_Position.y;
}

void Particle::setSpriteFromSheet(sf::IntRect textureBox, int tileSize) // set sprite
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

void Particle::moveTextureRect() // animate sprite by moving texRect location
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

Sprite Particle::getSprite()
{
	return m_Sprite;
}
//
//// This returns false if Particle does 0 damage (non-damaging Particle)
//float Particle::getParticleDamage()
//{
//	return m_ParticleDamage;
//}
//
//bool Particle::piercesEnemy()
//{
//	return m_PierceEnemy;
//}
//
//bool Particle::piercesObject()
//{
//	return m_PierceObject;
//}
//
//FloatRect Particle::getHitbox()
//{
//	return m_Hitbox;
//}