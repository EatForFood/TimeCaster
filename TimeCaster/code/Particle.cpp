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
	// Manually reset the animation if the particle is not active

	if (!m_IsPlaying)
	{
		m_Ani_Counter = 0;
		m_AnimationTimer = 0.0f;
	}

	// Keep track of the Particle
	m_IsPlaying = true;
	m_Position.x = startX;
	m_Position.y = startY;
	m_ParticleID = particleID;

	m_TimeElapsed = 0.0f;

	// Set the sprite based on particleID
	switch (m_ParticleID)
	{
	case 0: // Heal Particle
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/particles/healParticle.png"));
		break;
	case 1: // Blood Particle
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/particles/bloodParticle.png"));
		break;
	case 2: // Fire Particle
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/particles/fireParticle.png"));
		break;
	default:
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/particles/healParticle.png")); // Most noticeable default
		break;
	}

	// Position the Particle ready to be drawn
	if (particleID == 0)
	{
		setSpriteFromSheet(IntRect(0, 0, 384, 64), 64); // Heal particle is larger and has more frames
		m_TimePerFrame = 0.25f;
		m_TimeToPlay = 1.0f;
	}
	else
	{
		setSpriteFromSheet(IntRect(0, 0, 256, 64), 64);
		m_TimePerFrame = 0.06f;
		m_TimeToPlay = 0.25f;
	}

	m_Sprite.setPosition(m_Position);
}

void Particle::setPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Sprite.setPosition(m_Position);
}

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
	m_TimeElapsed += elapsedTime;

	if (m_TimeElapsed >= m_TimeToPlay)
	{
		m_IsPlaying = false;
	}
	else {
		moveTextureRect(elapsedTime);
	}
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

	m_Sprite.setTextureRect(sf::IntRect{ sheetCoordinate, spriteSize });
}

void Particle::moveTextureRect(float elapsedTime) // animate sprite by moving texRect location
{
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
	m_AnimationTimer += elapsedTime;
	if (m_AnimationTimer > m_TimePerFrame)
	{
		m_Ani_Counter++;
		m_AnimationTimer = 0;
	}
}

Sprite Particle::getSprite()
{
	return m_Sprite;
}

bool Particle::isPlaying()
{ 
	return m_IsPlaying;
}