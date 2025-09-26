#include "Character.h"
#include "TextureHolder.h"

Vector2f Character::getPosition()
{
	return m_Position;
}

void Character::setPosition(Vector2f newPosition)
{
	m_Position = newPosition;
	m_Sprite.setPosition(m_Position);
}


FloatRect Character::getGlobalBounds()
{
	return m_Sprite.getGlobalBounds();
}

Vector2f Character::getCenter()
{
	return m_Position;
}

Vector2f Character::getOrigin()
{
	return m_Sprite.getOrigin();
}

float Character::getRotation()
{
	return m_Sprite.getRotation();
}

Sprite Character::getSprite()
{
	return m_Sprite;
}

Sprite Character::getSpriteFromSheet()
{
	return m_Sprite;
}

float Character::getHealth()
{
	return m_Health;
}

float Character::getMaxHealth()
{
	return m_MaxHealth;
}

float Character::getY()
{
	return m_Position.y - 64;
}

void Character::setSpriteFromSheet(sf::IntRect textureBox) // set sprite
{
	int tile_size = 64;
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

void Character::moveTextureRect() // animate sprite by moving texRect location
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