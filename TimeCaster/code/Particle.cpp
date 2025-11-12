#include "Particle.h"
#include "Decal.h"
#include "TextureHolder.h"
using namespace std;

// The constructor
Particle::Particle()
{
	// Temporary sprite setup, heal particle being tested first
	m_Sprite = Sprite(TextureHolder::GetTexture("graphics/particles/healParticle.png"));
	m_Sprite.setOrigin(32, 32);
	m_Sprite.setScale(1, 1);



}

void Particle::play(float startX, float startY, int particleID)
{

	// Keep track of the Particle
	m_IsPlaying = true;
	m_Position.x = startX;
	m_Position.y = startY;
	m_ParticleID = particleID;

	// Set the sprite based on particleID
	switch (m_ParticleID)
	{
	case 0: // Heal Particle
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/particles/healParticle.png"));
		break;
	case 1: // Blood Particle
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/particles/bloodParticle.png"));
		break;
	case 2: // Spark Particle
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/particles/sparkParticle.png"));
		break;
	default:
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/particles/healParticle.png")); // Most noticeable default
		break;
	}







	// Position the Particle ready to be drawn
	if (particleID == 0)
	{
		setSpriteFromSheet(IntRect(0, 0, 384, 64), 64); // Heal particle is larger and has more frames
	}
	else
	{
		setSpriteFromSheet(IntRect(0, 0, 256, 64), 64);
	}

	m_Sprite.setPosition(m_Position);
}

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

bool Particle::isPlaying()
{ 
	return m_IsPlaying;
}