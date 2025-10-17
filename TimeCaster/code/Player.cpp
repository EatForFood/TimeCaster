#include "Player.h"
#include "TextureHolder.h"
#include "Engine.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include "CollisionDetection.h"
#include "Item.h"
#include "Weapon.h"

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
	m_Gold = START_GOLD;
	m_Kills = START_KILLS;
	m_Level = START_LEVEL;

	// Associate a texture with the sprite
	m_Sprite = Sprite(TextureHolder::GetTexture("graphics/player/playerWalk.png"));

	// Set the origin of the sprite to the centre, 
	m_Sprite.setOrigin(32, 32);
	m_Sprite.setScale(0.75,0.75);

	// manually set armour sprites
	m_SpriteHead = Sprite(TextureHolder::GetTexture("graphics/player/armour/robe/Head_robe_hood.png"));
	m_SpriteHead.setOrigin(32, 32);
	m_SpriteHead.setScale(0.75, 0.75);

	m_SpriteTorso = Sprite(TextureHolder::GetTexture("graphics/player/armour/robe/Torso_robe_shirt_brown.png"));
	m_SpriteTorso.setOrigin(32, 32);
	m_SpriteTorso.setScale(0.75, 0.75);

	m_SpritePants = Sprite(TextureHolder::GetTexture("graphics/player/armour/robe/Legs_robe_skirt.png"));
	m_SpritePants.setOrigin(32, 32);
	m_SpritePants.setScale(0.75, 0.75);

	m_SpriteShoes = Sprite(TextureHolder::GetTexture("graphics/player/armour/robe/Feet_shoes_brown.png"));
	m_SpriteShoes.setOrigin(32, 32);
	m_SpriteShoes.setScale(0.75, 0.75);


	m_Clothes.push_back(m_SpriteHead);
	m_Clothes.push_back(m_SpriteTorso);
	m_Clothes.push_back(m_SpritePants);
	m_Clothes.push_back(m_SpriteShoes);
}

void Player::spawn(IntRect arena, Vector2f resolution, int tileSize, int level)
{
	m_Hitbox.left = m_Position.x - 20;
	m_Hitbox.width = 40;
	m_Hitbox.top = m_Position.y - 20;
	m_Hitbox.height = 40;

	m_Level = level;

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

	//cout << m_Resolution.x << " 1 " << m_Resolution.y << endl

	m_RenderArea = FloatRect(0, 0, 1920, 1080);
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

void Player::update(float elapsedTime, Vector2i mousePosition, vector<NavBox> navBox)
{
	navBoxes = navBox;

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
			setSpriteFromSheet(IntRect(0, 0, 576, 64), 64);
		}

		if (direction == Vector2f(0, -1)) // down
		{
			setSpriteFromSheet(IntRect(0, 128, 576, 64), 64);
		}

		if (direction == Vector2f(1, 0)) // right
		{
			setSpriteFromSheet(IntRect(0, 192, 576, 64), 64);
		}

		if (direction == Vector2f(-1, 0)) // left
		{
			setSpriteFromSheet(IntRect(0, 64, 576, 64), 64);
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
		setSpriteFromSheet(IntRect(0, 0, 576, 64), 64); // set sprite depending on direction
		direction = Vector2f(0, 1);
	}

	for (auto& nav : navBoxes) { // if player walks into navBox 
		if (m_CollisionBox.intersects(nav.getShape().getGlobalBounds()))
		{
			if (collision.pointInShape(m_Position, nav.getShape())) {
				revertPosition();
			}
		}
	}

	if (m_DownPressed && !downDisabled)
	{
		m_PositionLast = m_Position;
		m_Position.y += m_Speed * elapsedTime;
		setSpriteFromSheet(IntRect(0, 128, 576, 64), 64);
		direction = Vector2f(0, -1);
	}

	for (auto& nav : navBoxes) { // if player walks into navBox 
		if (m_CollisionBox.intersects(nav.getShape().getGlobalBounds()))
		{
			if (collision.pointInShape(m_Position, nav.getShape())) {
				revertPosition();
			}
		}
	}

	if (m_RightPressed && !rightDisabled)
	{
		m_PositionLast = m_Position;
		m_Position.x += m_Speed * elapsedTime;
		setSpriteFromSheet(IntRect(0, 192, 576, 64), 64);
		direction = Vector2f(1, 0);
	}

	for (auto& nav : navBoxes) { // if player walks into navBox 
		if (m_CollisionBox.intersects(nav.getShape().getGlobalBounds()))
		{
			if (collision.pointInShape(m_Position, nav.getShape())) {
				revertPosition();
			}
		}
	}

	if (m_LeftPressed && !leftDisabled)
	{
		m_PositionLast = m_Position;
		m_Position.x -= m_Speed * elapsedTime;
		setSpriteFromSheet(IntRect(0, 64, 576, 64), 64);
		direction = Vector2f(-1, 0);
	}

	for (auto& nav : navBoxes) { // if player walks into navBox 
		if (m_CollisionBox.intersects(nav.getShape().getGlobalBounds()))
		{
			if (collision.pointInShape(m_Position, nav.getShape())) {
				revertPosition();
			}
		}
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

	m_SpritePants.setPosition(m_Position);
	m_SpriteHead.setPosition(m_Position);
	m_SpriteTorso.setPosition(m_Position);
	m_SpriteShoes.setPosition(m_Position);

	m_Sprite.setPosition(m_Position);
	m_CollisionBox.left = m_Position.x - 100;
	m_CollisionBox.top = m_Position.y - 100;
	m_CollisionBox.width = 200;
	m_CollisionBox.height = 200;

	m_RenderArea = FloatRect(m_Position.x - m_Resolution.x / 2, m_Position.y - m_Resolution.y / 2, m_Resolution.x, m_Resolution.y);
	

	/*cout << m_Resolution.x << " 2 " << m_Resolution.y << endl;

	m_Resolution.x = VideoMode::getDesktopMode().width;
	m_Resolution.y = VideoMode::getDesktopMode().height;

	cout << m_Resolution.x << " 3 " << m_Resolution.y << endl;*/

	// Calculate the angle between mouse and center of screen
	float angle = (atan2(mousePosition.y - m_Resolution.y / 2, mousePosition.x - m_Resolution.x / 2) * 180) / 3.141;

	if (angle < 0) angle += 360;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) // make player face mouse when holding righ click
	{
		if (angle >= 45 && angle < 135)
		{
			// facing down
			setSpriteFromSheet({ 0, 128, 576, 64 }, 64);
		}
		else if (angle >= 135 && angle < 225)
		{
			// facing left
			setSpriteFromSheet({ 0, 64, 576, 64 }, 64);
		}
		else if (angle >= 225 && angle < 315)
		{
			// facing up
			setSpriteFromSheet({ 0, 0, 576, 64 }, 64);
			
		}
		else
		{
			// facing right
			setSpriteFromSheet({ 0, 192, 576, 64 }, 64);
		}
	}

	if (m_UpPressed || m_DownPressed || m_LeftPressed || m_RightPressed) // animate sprite if player is moving
	{
		moveTextureRect();
	}

	upDisabled = false;
	downDisabled = false;
	leftDisabled = false;
	rightDisabled = false;
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

void Player::setPlayerLevel(int level)
{
	m_Level = level;
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

int Player::getPlayerLevel()
{
	return m_Level;
}

void Player::createNewSave()
{
	std::ofstream saveFile("gamedata/TCSave.txt");

	saveFile << std::fixed << std::setprecision(5) << START_SPEED << " " << START_HEALTH << " " << START_HEALTH << " " << START_STAMINA << " "
		<< START_STAMINA << " " << START_STAMINA_RECHARGE << " " << START_MANA << " " << START_MANA << " " << START_GOLD << " " << START_KILLS << " " << START_LEVEL << " " << START_SWORD << " " << START_WAND << " " << 64 << " " << 64 << std::endl;
	m_EquippedWeapons[0] = (Weapon(START_SWORD, Vector2f(760, 550)));
	m_EquippedWeapons[1] = (Weapon(START_WAND, Vector2f(860, 550)));
	saveFile.close();
}
void Player::createConfigFile(string difficultyString, bool windowedMode, bool displayFPS, float volume)
{
	std::ofstream configFile("gamedata/TCConfig.txt");
	
	configFile << difficultyString << " " << windowedMode << " " << displayFPS << " " << volume << std::endl;
	
	configFile.close();
}

bool Player::loadConfigFile()
{
	std::ifstream loadFile("gamedata/TCConfig.txt");
	if (loadFile.is_open())
	{
		loadFile >> m_DifficultyString;
		loadFile >> m_WindowedMode;
		loadFile >> m_DisplayFPS;
		loadFile >> m_Volume;
		return true;
	}
	return false;
}

//remember to pass in all player stats to be saved
void Player::updateSaveFile()
{	
	std::ofstream saveFile("gamedata/TCSave.txt");

	saveFile << std::fixed << std::setprecision(5) << m_Speed << " " << m_Health << " " << m_MaxHealth << " " << m_Stamina << " "
    << m_MaxStamina << " " << m_StaminaRecharge << " " << m_Mana << " " << m_MaxMana << " " << m_Gold << " " << m_Kills << " " << m_Level << " "
    << m_EquippedWeapons[0].getName() << " " << m_EquippedWeapons[1].getName() << " " << m_Position.x << " " << m_Position.y << std::endl;

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
		loadFile >> m_Kills;
		loadFile >> m_Level;
		loadFile >> m_EquippedSwordName;
		loadFile >> m_EquippedWandName;
		loadFile >> m_Position.x;
		loadFile >> m_Position.y;
		m_EquippedWeapons[0] = (Weapon(m_EquippedSwordName, Vector2f(760, 550)));
		m_EquippedWeapons[1] = (Weapon(m_EquippedWandName, Vector2f(860, 550)));
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
}

string Player::getdifficultyString()
{
	return m_DifficultyString;
}

void Player::setChunk(int chunk)
{
	m_Chunk = chunk;
}

void Player::setInCell()
{
	if (inCell)
	{
		inCell = false;
	}
	else
	{
		inCell = true;
	}
}

bool Player::getInCell()
{
	return inCell;
}

int Player::getChunk()
{
	return m_Chunk;
}

FloatRect Player::getRenderArea()
{
	return m_RenderArea;
}

bool Player::getWindowedMode()
{
	return m_WindowedMode;
}

bool Player::getDisplayFps()
{
	return m_DisplayFPS;
}

float Player::getVolume()
{
	return m_Volume;
}

void Player::incrementKillCount() {
	m_Kills++;
}

int Player::getKillCount() {
	return m_Kills;
}

void Player::switchWeapon()
{

	if (m_CombatType == Melee)
	{
		m_CombatType = Magic;
	}
	else
	{
		m_CombatType = Melee;
	}

}

bool Player::equipWeapon(Item weaponToEquip)
{
	if (weaponToEquip.getType() == Item::MeleeWeapon)
	{
		m_EquippedSwordName = weaponToEquip.getName();
		m_EquippedWeapons[0] = weaponToEquip;
		m_CombatType = Melee;
		return true;
	}
	else if (weaponToEquip.getType() == Item::MagicWeapon)
	{
		m_EquippedWandName = weaponToEquip.getName();
		m_EquippedWeapons[1] = weaponToEquip;
		m_CombatType = Magic;
		return true;
	}
	else
	{
		return false;
	}
}

vector<Item>& Player::getEquippedWeapons()
{
	return m_EquippedWeapons;
}

Player::CombatType Player::getCombatType()
{
	return m_CombatType;
}