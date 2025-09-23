#include "Character.h"
#include "TextureHolder.h"

FloatRect Character::getPosition()
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
		animation_it_limit = textureBox.width / tile_size;

		horizontal = true;
	}
	else if (textureBox.height > tile_size)
	{
		animation_it_limit = textureBox.height / tile_size;
		horizontal = false;
	}
	else
		throw std::logic_error("Animation bounding box must contain multiply sprites, setSprite(sf::IntRect )\n");

	//	cout << "setSpriteFromSheet  " << "animation_it limit" << animation_it_limit << "\n";
	//	cout << "setSpriteFromSheet func " << "ani_counter " << ani_counter << "\n";

	m_Sprite.setTextureRect(sf::IntRect{ sheetCoordinate, spriteSize });

}

void Character::moveTextureRect() // animate sprite by moving texRect location
{
	//	cout << " Move Texture Rect ani_counter " << ani_counter << "\n";
		// if the animation counter is greater than the animation limit reset to 1;
	if (ani_counter == animation_it_limit)
	{
		ani_counter = 0;
	}

	if (horizontal) {

		m_Sprite.setTextureRect(sf::IntRect(sheetCoordinate + sf::Vector2i(spriteSize.x * ani_counter, 0), spriteSize));
	}
	else {
		m_Sprite.setTextureRect(sf::IntRect(sheetCoordinate + sf::Vector2i(0, spriteSize.y * ani_counter), spriteSize));
	}

	//increment animation counter to point to the next frame
	double timePerFrame;
	timePerFrame = 1.0 / 6.0;
	animationTimer = animationTimer + timeElapsed;
	if (animationTimer > timePerFrame)
	{
		ani_counter++;
		animationTimer = 0;
	}

}