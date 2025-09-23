#include "player.h"
#include "TextureHolder.h"

Player::Player()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;
	m_Mana = START_MANA;
	m_MaxMana = START_MANA;
	m_Stamina = START_STAMINA;
	m_MaxStamina = START_STAMINA;
	m_StaminaRecharge = START_STAMINA_RECHARGE;
	

	// Associate a texture with the sprite
	// !!Watch this space!!
	//m_Sprite = Sprite(TextureHolder::GetTexture("graphics/player_pistol.png"));

	m_Sprite = Sprite(TextureHolder::GetTexture("graphics/player/playerWalk.png"));

	// Set the origin of the sprite to the centre, 
	// for smooth rotation
	m_Sprite.setOrigin(18, 18);

}

void Player::resetPlayerStats()
{
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_Stamina = START_STAMINA;
	m_MaxHealth = START_HEALTH;
	m_MaxMana = START_MANA;
	m_MaxStamina = START_STAMINA;
	m_StaminaRecharge = START_STAMINA_RECHARGE;
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
	if (isDodging) // can't be hit while dodging
	{
		 m_LastHit = timeHit; // if you successfully dodge an attack it resets the hit timer so you can't be hit again straight away
		return false;
	}
	else if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > 200)// 2 tenths of second
	{
		
		m_LastHit = timeHit;
		m_Health -= 10; //Change this to be varible?
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
		if (direction == Vector2f(0, 1)) // up
		{
			setSpriteFromSheet(IntRect(0, 0, 576, 64));
		}

		if (direction == Vector2f(0, -1)) // down
		{
			setSpriteFromSheet(IntRect(0, 128, 576, 64));
		}

		if (direction == Vector2f(-1, 0)) // right
		{
			setSpriteFromSheet(IntRect(0, 192, 576, 64));
		}

		if (direction == Vector2f(1, 0)) // left
		{
			setSpriteFromSheet(IntRect(0, 64, 576, 64));
		}
	}
	
	if (!m_UpPressed && !m_DownPressed && !m_LeftPressed && !m_RightPressed && m_Stamina < m_MaxStamina  )
	{
		m_Stamina += m_StaminaRecharge; //recharge stamina faster when not moving
	}
	else if (m_Stamina < m_MaxStamina) {
		m_Stamina += m_StaminaRecharge * 0.33; //recharge stamina slower when moving
	}
	

	if (m_UpPressed)
	{
		m_PositionLast = m_Position;
		m_Position.y -= m_Speed * elapsedTime;
		setSpriteFromSheet(IntRect(0, 0, 576, 64)); // set sprite depending on direction
		direction = Vector2f(1, 0);
	}

	if (m_DownPressed)
	{
		m_PositionLast = m_Position;
		m_Position.y += m_Speed * elapsedTime;
		setSpriteFromSheet(IntRect(0, 128, 576, 64));
		direction = Vector2f(-1, 0);
	}

	if (m_RightPressed)
	{
		m_PositionLast = m_Position;
		m_Position.x += m_Speed * elapsedTime;
		setSpriteFromSheet(IntRect(0, 192, 576, 64));
		direction = Vector2f(0, -1);
	}

	if (m_LeftPressed)
	{
		m_PositionLast = m_Position;
		m_Position.x -= m_Speed * elapsedTime;
		setSpriteFromSheet(IntRect(0, 64, 576, 64));
		direction = Vector2f(0, 1);
	}

	if (!isDodging)
	{
		m_Speed = START_SPEED;
	}

	if (m_UpPressed && m_RightPressed || m_UpPressed && m_LeftPressed ||
		m_DownPressed && m_RightPressed || m_DownPressed && m_LeftPressed) // player moved too fast diagonally
	{
		if (!isDodging)
		{
			m_Speed = m_Speed * 0.75;
		}
	}

	dodge();
	
	m_Sprite.setPosition(m_Position);

	// Calculate the angle between mouse and center of screen
	float angle = (atan2(mousePosition.y - m_Resolution.y / 2,
		mousePosition.x - m_Resolution.x / 2)
		* 180) / 3.141;

	if (angle < 0) angle += 360;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) // make player face mouse when holding righ click
	{
		if (angle >= 45 && angle < 135)
		{
			// facing down
			setSpriteFromSheet({ 0, 128, 576, 64 });
		}
		else if (angle >= 135 && angle < 225)
		{
			// facing left
			setSpriteFromSheet({ 0, 64, 576, 64 });
		}
		else if (angle >= 225 && angle < 315)
		{
			// facing up
			setSpriteFromSheet({ 0, 0, 576, 64 });
		}
		else
		{
			// facing right
			setSpriteFromSheet({ 0, 192, 576, 64 });
		}
	}

	if (m_UpPressed || m_DownPressed || m_LeftPressed || m_RightPressed) // animate sprite if player is moving
	{
		moveTextureRect();
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

void Player::upgradeStamina()
{
	// 25% max Stamina upgrade
	//25% because dodging takes 50 stamina so you'd get half of one more dodge
	m_MaxStamina += (START_STAMINA * .25);
}

void Player::upgradeMana()
{
	// 20% max Mana upgrade
	m_MaxMana += (START_MANA * .2);
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

void Player::increaseStaminaLevel(int amount)
{
	m_Stamina += amount;

	// But not beyond the maximum
	if (m_Stamina > m_MaxStamina)
	{
		m_Stamina = m_MaxStamina;
	}
}

void Player::increaseManaLevel(int amount)
{
	m_Mana += amount;

	// But not beyond the maximum
	if (m_Mana > m_MaxMana)
	{
		m_Mana = m_MaxMana;
	}
}

float Player::getMana()
{
	return m_Mana;
}

float Player::getMaxMana()
{
	return m_MaxMana;
}

float Player::getStamina()
{
	return m_Stamina;
}

float Player::getMaxStamina()
{
	return m_MaxStamina;
}

void Player::dodge()
{
	// Dodging enemies using the space key
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canDodge) {
		isDodging = true;
		canDodge = false;

		dodgeClock.restart();
		cooldownClock.restart();
		startDodge();
	}

	// After 200ms stop dodge
	if (isDodging && dodgeClock.getElapsedTime().asSeconds() > dodgeDuration) {
		isDodging = false;
		stopDodge();
	}

	// Allows the player to dodge again
	if (!canDodge && cooldownClock.getElapsedTime().asSeconds() > dodgeCooldown && m_Stamina >= 50) {
		canDodge = true;
	}
}

// Multiplies the player's speed to simulate dodging
void Player::startDodge() {


	m_Speed = m_Speed * 2;
	m_Stamina -= 50;  



}

// Returns player's speed to original value to stop dodge
void Player::stopDodge() {

	m_Speed = m_Speed / 2;
}

