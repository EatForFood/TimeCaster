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

FloatRect Character::getHitBox()
{
	return m_Hitbox;
}

FloatRect Character::getGlobalBounds()
{
	return m_Sprite.getGlobalBounds();
}

FloatRect Character::getColBox()
{
	return m_CollisionBox;
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

void Character::setSpriteFromSheet(sf::IntRect textureBox, int tileSize) // set sprite
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

	m_SpriteHead.setTextureRect(sf::IntRect{ sheetCoordinate, spriteSize });
	m_SpriteTorso.setTextureRect(sf::IntRect{ sheetCoordinate, spriteSize });
	m_SpritePants.setTextureRect(sf::IntRect{ sheetCoordinate, spriteSize });
	m_SpriteShoes.setTextureRect(sf::IntRect{ sheetCoordinate, spriteSize });

	m_SpriteWeapon.setTextureRect(sf::IntRect{ Vector2i(sheetCoordinate.x, sheetCoordinate.y * m_WeaponSize), spriteSize * m_WeaponSize});
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

		m_SpriteHead.setTextureRect(sf::IntRect(sheetCoordinate + sf::Vector2i(spriteSize.x * m_Ani_Counter, 0), spriteSize));
		m_SpriteTorso.setTextureRect(sf::IntRect(sheetCoordinate + sf::Vector2i(spriteSize.x * m_Ani_Counter, 0), spriteSize));
		m_SpritePants.setTextureRect(sf::IntRect(sheetCoordinate + sf::Vector2i(spriteSize.x * m_Ani_Counter, 0), spriteSize));
		m_SpriteShoes.setTextureRect(sf::IntRect(sheetCoordinate + sf::Vector2i(spriteSize.x * m_Ani_Counter, 0), spriteSize));
	}
	else {
		m_Sprite.setTextureRect(sf::IntRect(sheetCoordinate + sf::Vector2i(0, spriteSize.y * m_Ani_Counter), spriteSize));

		m_SpriteHead.setTextureRect(sf::IntRect(sheetCoordinate + sf::Vector2i(0, spriteSize.y * m_Ani_Counter), spriteSize));
		m_SpriteTorso.setTextureRect(sf::IntRect(sheetCoordinate + sf::Vector2i(0, spriteSize.y * m_Ani_Counter), spriteSize));
		m_SpritePants.setTextureRect(sf::IntRect(sheetCoordinate + sf::Vector2i(0, spriteSize.y * m_Ani_Counter), spriteSize));
		m_SpriteShoes.setTextureRect(sf::IntRect(sheetCoordinate + sf::Vector2i(0, spriteSize.y * m_Ani_Counter), spriteSize));
	}

	if (m_IsAttacking)
	{
		if (m_Horizontal)
		{
			m_SpriteWeapon.setTextureRect(sf::IntRect(Vector2i(sheetCoordinate.x, sheetCoordinate.y * m_WeaponSize) + sf::Vector2i(spriteSize.y * m_WeaponSize * m_Ani_Counter, 0), spriteSize * m_WeaponSize));
		}
		else
		{
			m_SpriteWeapon.setTextureRect(sf::IntRect(Vector2i(sheetCoordinate.x, sheetCoordinate.y * m_WeaponSize) + sf::Vector2i(0, spriteSize.y * m_WeaponSize * m_Ani_Counter), spriteSize * m_WeaponSize));
		}
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

Sprite Character::getPants()
{
	return m_SpritePants;
}

Sprite Character::getHead()
{
	return m_SpriteHead;
}

Sprite Character::getTorso()
{
	return m_SpriteTorso;
}

Sprite Character::getShoes()
{
	return m_SpriteShoes;
}

Sprite Character::getWeapon()
{
	return m_SpriteWeapon;
}

void Character::resetAniCounter()
{
	m_Ani_Counter = 1;
}

int Character::getAniCounter()
{
	return m_Ani_Counter;
}

bool Character::isAttacking()
{
	return m_IsAttacking;
}

void Character::setHealth(float health)
{
	m_Health = m_Health + health;
}

float Character::getAttackDamage()
{
	return m_AttackDmg;
}

bool Character::isDead()
{
	return m_IsDead;
}

bool Character::wasHit()
{
	return m_WasHit;
}

void Character::setWasHit(bool hit)
{
	m_WasHit = hit;
}

bool Character::isCastingSpell()
{
	return m_CastingSpell;
}

void Character::castingSpell(bool casting)
{
	m_CastingSpell = casting;
}


void Character::setDifficultyMult(float mult)
{
	m_DifficultyMult = mult;
}