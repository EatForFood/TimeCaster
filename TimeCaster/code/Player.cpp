#include "player.h"
#include "TextureHolder.h"
#include "TimeCaster.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace sf;


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
	m_Sprite = Sprite(TextureHolder::GetTexture("graphics/player/playerWalk.png"));

	// Set the origin of the sprite to the centre, 
	m_Sprite.setOrigin(32, 32);
	m_Sprite.setScale(0.75,0.75);

}



void Player::spawn(IntRect arena, Vector2f resolution, int tileSize)
{
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

bool Player::hit(Time timeHit, float damage, int iFrames)
{
	if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > m_IFrames && m_IsDodging) // can't be hit while dodging, also checks for i frames
	{
		 m_LastHit = timeHit; // if you successfully dodge an attack it resets the hit timer so you can't be hit again straight away
		 m_IFrames = iFrames;
	
		return false;
	}
	else if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > m_IFrames)
	{
		m_IFrames = iFrames;
		m_LastHit = timeHit;
		m_Health -= damage; 
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

	m_TimeElapsed = elapsedTime; 

	if (!m_UpPressed && !m_DownPressed && !m_LeftPressed && !m_RightPressed)
	{
		m_IsMoving = false;
	}
	else {
		m_IsMoving = true;
	}


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

		if (direction == Vector2f(1, 0)) // right
		{
			setSpriteFromSheet(IntRect(0, 192, 576, 64));
		}

		if (direction == Vector2f(-1, 0)) // left
		{
			setSpriteFromSheet(IntRect(0, 64, 576, 64));
		}
	}
	


	if (!m_IsMoving && m_Stamina < m_MaxStamina  )
	{
		m_Stamina += m_StaminaRecharge; //recharge stamina faster when not moving
	}
	else if (m_Stamina < m_MaxStamina) {
		m_Stamina += m_StaminaRecharge * 0.33; //recharge stamina slower when moving
	}
	

		

	if (m_UpPressed && !upDisabled)
	{
		m_PositionLast = m_Position;
		m_Position.y -= m_Speed * elapsedTime;
		setSpriteFromSheet(IntRect(0, 0, 576, 64)); // set sprite depending on direction
		direction = Vector2f(0, 1);
	}

	if (m_DownPressed && !downDisabled)
	{
		m_PositionLast = m_Position;
		m_Position.y += m_Speed * elapsedTime;
		setSpriteFromSheet(IntRect(0, 128, 576, 64));
		direction = Vector2f(0, -1);
	}

	if (m_RightPressed && !rightDisabled)
	{
		m_PositionLast = m_Position;
		m_Position.x += m_Speed * elapsedTime;
		setSpriteFromSheet(IntRect(0, 192, 576, 64));
		direction = Vector2f(1, 0);
	}

	if (m_LeftPressed && !leftDisabled)
	{
		m_PositionLast = m_Position;
		m_Position.x -= m_Speed * elapsedTime;
		setSpriteFromSheet(IntRect(0, 64, 576, 64));
		direction = Vector2f(-1, 0);
	}

	if (!m_IsDodging)
	{
		m_Speed = START_SPEED;
	}

	if (m_UpPressed && m_RightPressed || m_UpPressed && m_LeftPressed ||
		m_DownPressed && m_RightPressed || m_DownPressed && m_LeftPressed) // player moved too fast diagonally
	{
		if (!m_IsDodging)
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
	/*
	if (rightDisabled)
	{
		leftDisabled = false;
	}

	if (leftDisabled)
	{
		rightDisabled = false;
	}

	if (upDisabled)
	{
		downDisabled = false;
	}

	if (downDisabled)
	{
		upDisabled = false;
	}
	*/
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
	m_MaxStamina += (START_STAMINA * 0.25f);
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

float Player::getStaminaRecharge()
{
	return m_StaminaRecharge;
}


int Player::getGold()
{
	return m_Gold;
}

void Player::setGold(int amount)
{
	m_Gold += amount;
}

float Player::getSpeed()
{
	return m_Speed;
}

void Player::createNewSave()
{

	

	std::ofstream saveFile("gamedata/TCSave.txt");


	saveFile << std::fixed << std::setprecision(5) << START_SPEED << " " << START_HEALTH << " " << START_HEALTH << " " << START_STAMINA << " "
		<< START_STAMINA << " " << START_STAMINA_RECHARGE << " " << START_MANA << " " << START_MANA << " " << 0 << " " << 64 << " " << 64 << std::endl;

	saveFile.close();
}
void Player::createConfigFile(string dfficultyString)
{
	std::ofstream configFile("gamedata/TCConfig.txt");
	
	configFile << dfficultyString << std::endl;
	
	configFile.close();
}

bool Player::loadConfigFile()
{
	std::ifstream loadFile("gamedata/TCConfig.txt");
	if (loadFile.is_open())
	{
		loadFile >> m_DifficultyString;
		return true;
	}
	return false;
}

//remember to pass in all player stats to be saved
void Player::updateSaveFile(float currentSpeed, float currentHealth, float maxHealth, float currentStamina, float maxStamina, float staminaRecharge, float currentMana, float maxMana, int gold, Vector2f position)
{
		
	std::ofstream saveFile("gamedata/TCSave.txt");

	saveFile << std::fixed << std::setprecision(5) << currentSpeed << " " << currentHealth << " " << maxHealth << " " << currentStamina << " "
	<< maxStamina << " " << staminaRecharge << " " << currentMana << " " << maxMana << " " << gold << " " << position.x << " " << position.y << std::endl;

	saveFile.close();
}

bool Player::loadSaveFile()
{
	std::ifstream loadFile("gamedata/TCSave.txt");

	if (loadFile.is_open())
	{
		loadFile >> m_Speed;
		loadFile >> m_Health;
		loadFile >> m_MaxHealth;
		loadFile >> m_Stamina;
		loadFile >> m_MaxStamina;
		loadFile >> m_StaminaRecharge;
		loadFile >> m_Mana;
		loadFile >> m_MaxMana;
		loadFile >> m_Gold;
		loadFile >> m_DifficultyString;
		loadFile >> m_Position.x;
		loadFile >> m_Position.y;
		return true;
	}
	else
	{
		return false;
	}
}

void Player::dodge()
{
	// Dodging enemies using the space key
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_CanDodge && m_IsMoving ) {
		m_IsDodging = true;
		m_CanDodge = false;

		m_DodgeClock.restart();
		m_CooldownClock.restart();
		startDodge();
	}

	// After 200ms stop dodge
	if (m_IsDodging && m_DodgeClock.getElapsedTime().asSeconds() > m_DodgeDuration) {
		m_IsDodging = false;
		stopDodge();
	}

	// Allows the player to dodge again
	if (!m_CanDodge && m_CooldownClock.getElapsedTime().asSeconds() > m_DodgeCooldown && m_Stamina >= 50) {
		m_CanDodge = true;
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

void Player::disableDown() {
	downDisabled = true;
}

void Player::disableUp() {
	upDisabled = true;
}

void Player::disableRight() {
	rightDisabled = true;
}

void Player::disableLeft() {
	leftDisabled = true;
}


void Player::revertPosition() {
	setPosition(m_PositionLast);
	m_Position = m_PositionLast;
}

string Player::getdifficultyString()
{
	return m_DifficultyString;
}