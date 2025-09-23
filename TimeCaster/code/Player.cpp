#include "player.h"
#include "TextureHolder.h"

Player::Player()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;

	// Associate a texture with the sprite
	// !!Watch this space!!
	//m_Sprite = Sprite(TextureHolder::GetTexture("graphics/player_pistol.png"));

	m_Sprite = Sprite(TextureHolder::GetTexture("graphics/player/playerWalk.png"));

	// Set the origin of the sprite to the centre, 
	// for smooth rotation
	m_Sprite.setOrigin(18, 18);

	m_Gun = 0;

}

void Player::resetPlayerStats()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;
}

void Player::spawn(IntRect arena, Vector2f resolution, int tileSize)
{
	// Place the player in the middle of the arena
	m_Position.x = 64;
	m_Position.y = 64;

	m_Hitbox.left = m_Position.x - 20;
	m_Hitbox.width = 40;
	m_Hitbox.top = m_Position.y - 20;
	m_Hitbox.height = 40;

	// Copy the details of the arena to the player's m_Arena
	m_Arena.left = arena.left;
	m_Arena.width = arena.width;
	m_Arena.top = arena.top;
	m_Arena.height = arena.height;

	// Remember how big the tiles are in this arena
	m_TileSize = tileSize;

	// Strore the resolution for future use
	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;

}

Time Player::getLastHitTime()
{
	return m_LastHit;
}

bool Player::hit(Time timeHit)
{
	if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 200)// 2 tenths of second
	{
		m_LastHit = timeHit;
		m_Health -= 10;
		return true;
	}
	else
	{
		return false;
	}

}

void Player::moveLeft()
{
	m_LeftPressed = true;
}

void Player::moveRight()
{
	m_RightPressed = true;
}

void Player::moveUp()
{
	m_UpPressed = true;
}

void Player::moveDown()
{
	m_DownPressed = true;
}

void Player::stopLeft()
{
	m_LeftPressed = false;
}

void Player::stopRight()
{
	m_RightPressed = false;
}

void Player::stopUp()
{
	m_UpPressed = false;
}

void Player::stopDown()
{
	m_DownPressed = false;
}

void Player::update(float elapsedTime, Vector2i mousePosition)
{
	
	timeElapsed = elapsedTime;

	if (!m_UpPressed || !m_DownPressed || !m_LeftPressed || !m_RightPressed) // if player is not moving set sprite to standing frame in last direction faced
	{
		if (direction == Vector2f(1, 0))
		{
			setSpriteFromSheet(IntRect(0, 0, 576, 64));
		}

		if (direction == Vector2f(-1, 0))
		{
			setSpriteFromSheet(IntRect(0, 128, 576, 64));
		}

		if (direction == Vector2f(0, -1))
		{
			setSpriteFromSheet(IntRect(0, 192, 576, 64));
		}

		if (direction == Vector2f(0, 1))
		{
			setSpriteFromSheet(IntRect(0, 64, 576, 64));
		}
	}

	if (m_UpPressed)
	{
		m_PositionLast = m_Position;
		m_Position.y -= m_Speed * elapsedTime;
		setSpriteFromSheet(IntRect(0, 0, 576, 64)); // set sprite depending on direction
		moveTextureRect(); // move to next frame
		direction = Vector2f(1, 0);
	}

	if (m_DownPressed)
	{
		m_PositionLast = m_Position;
		m_Position.y += m_Speed * elapsedTime;
		setSpriteFromSheet(IntRect(0, 128, 576, 64));
		moveTextureRect();
		direction = Vector2f(-1, 0);
	}

	if (m_RightPressed)
	{
		m_PositionLast = m_Position;
		m_Position.x += m_Speed * elapsedTime;
		setSpriteFromSheet(IntRect(0, 192, 576, 64));
		moveTextureRect();
		direction = Vector2f(0, -1);
	}

	if (m_LeftPressed)
	{
		m_PositionLast = m_Position;
		m_Position.x -= m_Speed * elapsedTime;
		setSpriteFromSheet(IntRect(0, 64, 576, 64));
		moveTextureRect();
		direction = Vector2f(0, 1);
	}

	m_Sprite.setPosition(m_Position);

	// Calculate the angle the player is facing
	float angle = (atan2(mousePosition.y - m_Resolution.y / 2,
		mousePosition.x - m_Resolution.x / 2)
		* 180) / 3.141;

	m_Speed = START_SPEED;

	if (m_LeftPressed && m_DownPressed || m_LeftPressed && m_UpPressed || // player was moving too quick diagonally
		m_RightPressed && m_DownPressed || m_RightPressed && m_UpPressed)
	{
		m_Speed = m_Speed * 0.75;
	}

}

void Player::upgradeSpeed()
{
	// 20% speed upgrade
	m_Speed += (START_SPEED * .2);
}

void Player::upgradeHealth()
{
	// 20% max health upgrade
	m_MaxHealth += (START_HEALTH * .2);

}

void Player::increaseHealthLevel(int amount)
{
	m_Health += amount;

	// But not beyond the maximum
	if (m_Health > m_MaxHealth)
	{
		m_Health = m_MaxHealth;
	}
}

void Player::changeGun(String gun)
{
	if (gun == "shotgun")
	{
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/player_shotgun.png"));
		m_Sprite.setOrigin(31, 18);
		m_Gun = 1;
	}
	else if (gun == "rifle")
	{
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/player_rifle.png"));
		m_Sprite.setOrigin(27, 18);
		m_Gun = 2;
	}
	else
	{
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/player_pistol.png"));
		m_Sprite.setOrigin(18, 18);
		m_Gun = 0;
	}
}

int Player::getGun()
{
	return m_Gun;
}

void Player::updateSprite()
{

}

