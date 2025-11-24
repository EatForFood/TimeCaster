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
	m_MaxHealth = START_HEALTH * 2;
	m_Mana = START_MANA;
	m_MaxMana = START_MANA;
	m_ManaRecharge = START_MANA_RECHARGE;
	m_Stamina = START_STAMINA;
	m_MaxStamina = START_STAMINA;
	m_StaminaRecharge = START_STAMINA_RECHARGE;
	m_Gold = START_GOLD;
	m_Kills = START_KILLS;
	m_Level = START_LEVEL;

	// Associate a texture with the body sprite
	m_Sprite = Sprite(TextureHolder::GetTexture("graphics/player/playerWalk.png")); 
	m_Sprite.setOrigin(32, 32);
	m_Sprite.setScale(0.75,0.75);

	// manually set armour sprites
	m_SpriteHead = Sprite(TextureHolder::GetTexture("graphics/player/armour/Family_Hood.png"));
	m_SpriteHead.setOrigin(32, 32);
	m_SpriteHead.setScale(0.75, 0.75);

	m_SpriteTorso = Sprite(TextureHolder::GetTexture("graphics/player/armour/Family_Robe.png"));
	m_SpriteTorso.setOrigin(32, 32);
	m_SpriteTorso.setScale(0.75, 0.75);

	m_SpritePants = Sprite(TextureHolder::GetTexture("graphics/player/armour/Robe_Leggings.png"));
	m_SpritePants.setOrigin(32, 32);
	m_SpritePants.setScale(0.75, 0.75);

	m_SpriteShoes = Sprite(TextureHolder::GetTexture("graphics/player/armour/Basic_Shoes.png"));
	m_SpriteShoes.setOrigin(32, 32);
	m_SpriteShoes.setScale(0.75, 0.75);

	m_SpriteWeapon = Sprite(TextureHolder::GetTexture("graphics/player/weapon/slash/Iron_Sword.png"));
	m_SpriteWeapon.setOrigin(32, 32);
	m_SpriteWeapon.setScale(0.75, 0.75);

	m_Clothes.push_back(m_SpriteHead);
	m_Clothes.push_back(m_SpriteTorso);
	m_Clothes.push_back(m_SpritePants);
	m_Clothes.push_back(m_SpriteShoes);
}

void Player::spawn(Vector2f resolution, int tileSize, int level, bool setPos)
{

	if (setPos)
	{
		m_Position = Vector2f(50, 600);
	}
	// assign player's hitbox
	m_Hitbox.left = m_Position.x - HITBOX_WIDTH / 2;
	m_Hitbox.width = HITBOX_WIDTH;
	m_Hitbox.top = m_Position.y - HITBOX_HEIGHT / 2;
	m_Hitbox.height = HITBOX_HEIGHT;

	m_Level = level;

	// Remember how big the tiles are in this arena
	m_TileSize = tileSize;

	// Strore the resolution for future use
	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;

	m_RenderArea = FloatRect(0, 0, m_Resolution.x, m_Resolution.y);
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
	else if (timeHit.asMilliseconds() - m_LastHit.asMilliseconds() > m_IFrames && !m_Phasing) // you can phase through attacks, but it won't give you any iframes
	{
		m_IFrames = iFrames;
		m_LastHit = timeHit;
		m_Health -= (damage * m_DifficultyMult) * getArmourDamageReductionMult(); 
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

void Player::update(float elapsedTime, Vector2i mousePosition, const vector<NavBox>& navBox)
{
	//navBoxes = navBox;

	m_TimeElapsed = elapsedTime; 

	if (m_IsAttacking)
	{
		if (m_CombatType == Melee)
		{
			m_AttackDmg = m_EquippedWeapons[0].getDamage();
		}
		else if (m_CombatType == Magic) // if using spells dont let staff do dmg
		{
			m_AttackDmg = 0;
		}

		moveTextureRect();

		if (getAniCounter() == 0 || getAniCounter() == 10) // stop attacking when animation ends
		{
			m_IsAttacking = false;
			m_CastingSpell = false;
			resetAniCounter();
			// revert to idle animation
			m_Sprite = Sprite(TextureHolder::GetTexture("graphics/player/playerWalk.png"));
			m_Sprite.setOrigin(32, 32);
			m_Sprite.setScale(0.75, 0.75);

			equipArmour(m_EquippedArmour[0].getName());
			equipArmour(m_EquippedArmour[1].getName());
			equipArmour(m_EquippedArmour[2].getName());
			equipArmour(m_EquippedArmour[3].getName());

			if (m_CombatType == Melee)
			{
				equipWeapon(m_EquippedWeapons[0].getName());
			}
			else if (m_CombatType == Magic)
			{
				equipWeapon(m_EquippedWeapons[1].getName());
			}

			// reset armour sprites if needed
		}
		else
		{
			moveTextureRect();
		}
	}

	if (!m_UpPressed && !m_DownPressed && !m_LeftPressed && !m_RightPressed)
	{
		m_IsMoving = false;
	}
	else {
		m_IsMoving = true;
	}

	if (!m_UpPressed || !m_DownPressed || !m_LeftPressed || !m_RightPressed) // if player is not moving set sprite to standing frame in last direction faced
	{
		if (!m_IsAttacking)
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
	}

	if (!m_IsMoving && m_Stamina < m_MaxStamina  )
	{
		m_Stamina += m_StaminaRecharge * elapsedTime; //recharge stamina faster when not moving
	}
	else if (m_Stamina < m_MaxStamina) {
		m_Stamina += m_StaminaRecharge * elapsedTime * 0.33; //recharge stamina slower when moving
	}	

	if (m_Mana < m_MaxMana && m_CombatType == Melee)
	{
		m_Mana += m_ManaRecharge * elapsedTime; // recharge mana faster when not using magic
	}
	else if (m_Mana < m_MaxMana)
	{
		m_Mana += m_ManaRecharge * elapsedTime * 0.66; // recharge mana slower when using magic
	}

	if (m_Health < m_MaxHealth && regenHealth)
	{
		m_Health += 2.5f * elapsedTime;
	}
	
	m_InObject = false;

	for (auto& nav : navBox) { // if player walks into navBox 
		if (m_CollisionBox.intersects(nav.getShape().getGlobalBounds()))
		{
			if (collision.pointInShape(m_Position, nav.getShape()) && !m_Phasing) {
				revertPosition();
			}
			else if (m_Phasing)
			{
				m_InObject = true;
			}
		}
	}

	if (m_UpPressed && !upDisabled)
	{
		if (!m_InObject)
		{
			m_PositionLast = m_Position; // don't save the last position if inside an object
		}
		m_Position.y -= m_Speed * elapsedTime;
		direction = Vector2f(0, 1);
	}

	for (auto& nav : navBox) { // if player walks into navBox 
		if (m_CollisionBox.intersects(nav.getShape().getGlobalBounds()))
		{
			if (collision.pointInShape(m_Position, nav.getShape()) && !m_Phasing) {
				revertPosition();
			}
			else if (m_Phasing)
			{
				m_InObject = true;
			}
		}
	}

	if (m_DownPressed && !downDisabled)
	{
		if (!m_InObject)
		{
			m_PositionLast = m_Position; // don't save the last position if inside an object
		}
		m_Position.y += m_Speed * elapsedTime;
		direction = Vector2f(0, -1);
	}
	for (auto& nav : navBox) { // if player walks into navBox 
		if (m_CollisionBox.intersects(nav.getShape().getGlobalBounds()))
		{
			if (collision.pointInShape(m_Position, nav.getShape()) && !m_Phasing) {
				revertPosition();
			}
			else if (m_Phasing)
			{
				m_InObject = true;
			}
		}
	}

	if (m_RightPressed && !rightDisabled)
	{
		if (!m_InObject)
		{
			m_PositionLast = m_Position; // don't save the last position if inside an object
		}
		m_Position.x += m_Speed * elapsedTime;
		direction = Vector2f(1, 0);
	}

	for (auto& nav : navBox) { // if player walks into navBox 
		if (m_CollisionBox.intersects(nav.getShape().getGlobalBounds()))
		{
			if (collision.pointInShape(m_Position, nav.getShape()) && !m_Phasing) {
				revertPosition();
			}
			else if (m_Phasing)
			{
				m_InObject = true;
			}
		}
	}

	if (m_LeftPressed && !leftDisabled)
	{
		if (!m_InObject)
		{
			m_PositionLast = m_Position; // don't save the last position if inside an object
		}
		m_Position.x -= m_Speed * elapsedTime;
		direction = Vector2f(-1, 0);
	}

	for (auto& nav : navBox) { // if player walks into navBox 
		if (m_CollisionBox.intersects(nav.getShape().getGlobalBounds()))
		{
			if (collision.pointInShape(m_Position, nav.getShape()) && !m_Phasing) {
				revertPosition();
			}
			else if (m_Phasing)
			{
				m_InObject = true;
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

	// update player sprite and equipment
	m_SpritePants.setPosition(m_Position);
	m_SpriteHead.setPosition(m_Position);
	m_SpriteTorso.setPosition(m_Position);
	m_SpriteShoes.setPosition(m_Position);
	m_Sprite.setPosition(m_Position);
	m_SpriteWeapon.setPosition(m_Position);

	// update collision detection zone
	m_CollisionBox.left = m_Position.x - 100;
	m_CollisionBox.top = m_Position.y - 100;
	m_CollisionBox.width = 200;
	m_CollisionBox.height = 200;

	// update player hitbox
	m_Hitbox.left = m_Position.x - 12;
	m_Hitbox.width = 24;
	m_Hitbox.top = m_Position.y - 12;
	m_Hitbox.height = 24;

	// update render area
	m_RenderArea = FloatRect(m_Position.x - m_Resolution.x / 2, m_Position.y - m_Resolution.y / 2, m_Resolution.x, m_Resolution.y);

	// Calculate the angle between mouse and center of screen
	float angle = (atan2(mousePosition.y - m_Resolution.y / 2, mousePosition.x - m_Resolution.x / 2) * 180) / 3.141;

	if (angle < 0) angle += 360;

	if (Mouse::isButtonPressed(Mouse::Right)) // make player face mouse when holding right click
	{
		if (angle >= 45 && angle < 135)
		{
			// facing down
			direction = Vector2f(0, -1);

		}
		else if (angle >= 135 && angle < 225)
		{
			// facing left
			direction = Vector2f(-1, 0);
		}
		else if (angle >= 225 && angle < 315)
		{
			// facing up
			direction = Vector2f(0, 1);
		}
		else
		{
			// facing right
			direction = Vector2f(1, 0);
		}
	}

	if (m_UpPressed || m_DownPressed || m_LeftPressed || m_RightPressed) // animate sprite if player is moving
	{
		if (!m_IsAttacking)
		{
			moveTextureRect();
		}
	}

	upDisabled = false;
	downDisabled = false;
	leftDisabled = false;
	rightDisabled = false;

	if (m_Phasing)
	{
	m_Sprite.setColor(Color(255, 255, 255, 128)); // make player tranluscent while phasing
	m_SpriteHead.setColor(Color(255, 255, 255, 128));
	m_SpriteTorso.setColor(Color(255, 255, 255, 128));
	m_SpritePants.setColor(Color(255, 255, 255, 128));
	m_SpriteShoes.setColor(Color(255, 255, 255, 128));
	m_SpriteWeapon.setColor(Color(255, 255, 255, 128));
	}
}

void Player::upgradeSpeed()
{
	// 20% speed upgrade
	m_Speed += (START_SPEED * .2);
}

void Player::upgradeHealth()
{
	// 25% max health upgrade
	// Multiply by 2 as the player starts with half health
	m_MaxHealth += (START_HEALTH * 2 * .25);
	m_Health += (START_HEALTH * 2 * .25);
}

void Player::upgradeStamina()
{
	// 25% max Stamina upgrade
	m_MaxStamina += (START_STAMINA * 0.25f);
	m_Stamina += (START_STAMINA * 0.25f);
}

void Player::upgradeMana()
{
	// 25% max Mana upgrade
	m_MaxMana += (START_MANA * .25);
	m_Mana += (START_MANA * .25);
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

void Player::addGold(int amount)
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
	ofstream saveFile("gamedata/TCSave.txt");

	saveFile << fixed << setprecision(5) 
		<< START_SPEED << " " 
		<< START_HEALTH << " " 
		<< (START_HEALTH * 2) << " "
		<< START_STAMINA << " "
		<< START_STAMINA << " " 
		<< START_STAMINA_RECHARGE << " " 
		<< START_MANA << " " 
		<< START_MANA << " " 
		<< START_MANA_RECHARGE << " " 
		<< START_GOLD << " " 
		<< START_KILLS << " " 
		<< START_EXP << " "
		<< START_LEVEL << " " 
		<< START_SWORD << " "
		<< START_WAND << " " 
		<< START_HEAD_ARMOUR << " " 
		<< START_CHEST_ARMOUR << " " 
		<< START_TROUSER_ARMOUR << " " 
		<< START_SHOE_ARMOUR << " " 
		<< START_NECK_ARMOUR << " " 
		<< START_SWORD << " "
		<< START_WAND << " "
		<< START_HEAD_ARMOUR << " "
		<< START_CHEST_ARMOUR << " "
		<< START_TROUSER_ARMOUR << " "
		<< START_SHOE_ARMOUR << " "
		<< START_NECK_ARMOUR << " "
		<< false << " "
		<< "Health_Potion null null null null null null null null "
		<< 64 << " " 
		<< 64	
		<< endl;
	m_EquippedWeapons[0] = (Weapon(START_SWORD, Vector2f(0, 0)));
	m_EquippedWeapons[1] = (Weapon(START_WAND, Vector2f(0, 0)));
	m_EquippedArmour[0] = (Equipment(START_HEAD_ARMOUR, Vector2f(0, 0)));
	m_EquippedArmour[1] = (Equipment(START_CHEST_ARMOUR, Vector2f(0, 0)));
	m_EquippedArmour[2] = (Equipment(START_TROUSER_ARMOUR, Vector2f(0, 0)));
	m_EquippedArmour[3] = (Equipment(START_SHOE_ARMOUR, Vector2f(0, 0)));
	m_EquippedArmour[4] = (Equipment(START_NECK_ARMOUR, Vector2f(0, 0)));

	saveFile.close();
}

void Player::createConfigFile(string difficultyString, bool windowedMode, bool displayFPS, float volume, bool vSync, int fpsLimit)
{
	std::ofstream configFile("gamedata/TCConfig.txt");
	
	configFile << difficultyString << " " 
		<< windowedMode << " " 
		<< displayFPS << " " 
		<< volume << " "
		<< vSync << " "
		<< fpsLimit
		<< std::endl;
	
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
		loadFile >> m_VSync;
		loadFile >> m_FpsLimit;
		return true;
	}
	return false;
}

//remember to pass in all player stats to be saved
void Player::updateSaveFile()
{	
	ofstream saveFile("gamedata/TCSave.txt");

	saveFile << fixed << setprecision(5) 
		<< m_Speed << " " 
		<< m_Health << " "
		<< m_MaxHealth << " "
		<< m_Stamina << " "
		<< m_MaxStamina << " " 
		<< m_StaminaRecharge << " " 
		<< m_Mana << " " 
		<< m_MaxMana << " " 
		<< m_ManaRecharge << " "
		<< m_Gold << " " 
		<< m_Kills << " " 
		<< m_Exp << " "
		<< m_Level << " "
		<< m_EquippedWeapons[0].getName() << " " 
		<< m_EquippedWeapons[1].getName() << " " 
		<< m_EquippedArmour[0].getName() << " "
		<< m_EquippedArmour[1].getName() << " " 
		<< m_EquippedArmour[2].getName() << " " 
		<< m_EquippedArmour[3].getName() << " " 
		<< m_EquippedArmour[4].getName()  << " "
		<< m_StoredItems[0].getName() << " "
		<< m_StoredItems[1].getName() << " "
		<< m_StoredItems[2].getName() << " "
		<< m_StoredItems[3].getName() << " "
		<< m_StoredItems[4].getName() << " "
		<< m_StoredItems[5].getName() << " "
		<< m_StoredItems[6].getName() << " "
		<< m_StoredItems[7].getName() << " "
		<< m_StoredItems[8].getName() << " "
		<< m_StoredItems[9].getName() << " "
		<< m_StoredItems[10].getName() << " "
		<< m_StoredItems[11].getName() << " "
		<< m_StoredItems[12].getName() << " "
		<< m_StoredItems[13].getName() << " "
		<< m_StoredItems[14].getName() << " "
		<< m_StoredItems[15].getName() << " "
		<< m_SoldSentimentalItem << " "
		<< m_Position.x << " " 
		<< m_Position.y
		<< std::endl; 
		
	saveFile.close();
}

bool Player::loadSaveFile()
{
	for (int i = 0; i < m_StoredItems.size(); i++)
	{
		m_StoredItems[i] = Item("null", Vector2f(0, 0));
	}

	std::ifstream loadFile("gamedata/TCSave.txt");
	string itemsToStore[16];

	if (loadFile.is_open())
	{
		// load in all player stats
		loadFile >> m_Speed;
		loadFile >> m_Health;
		loadFile >> m_MaxHealth;
		loadFile >> m_Stamina;
		loadFile >> m_MaxStamina;
		loadFile >> m_StaminaRecharge;
		loadFile >> m_Mana;
		loadFile >> m_MaxMana;
		loadFile >> m_ManaRecharge;

		loadFile >> m_Gold;
		loadFile >> m_Kills;
		loadFile >> m_Exp;
		loadFile >> m_Level;
		
		loadFile >> m_EquippedSwordName;
		loadFile >> m_EquippedWandName;

		loadFile >> m_EquippedHeadArmourName;
		loadFile >> m_EquippedChestArmourName;
		loadFile >> m_EquippedTrouserArmourName;
		loadFile >> m_EquippedShoeArmourName;
		loadFile >> m_EquippedNeckArmourName;

		for (int i = 0; i < 16; i++)
		{
			loadFile >> itemsToStore[i];
		}

		loadFile >> m_SoldSentimentalItem;

  		loadFile >> m_Position.x;
		loadFile >> m_Position.y;
		// equip saved weapons and armour
		equipArmour(m_EquippedHeadArmourName);
		equipArmour(m_EquippedChestArmourName);
		equipArmour(m_EquippedTrouserArmourName);
		equipArmour(m_EquippedShoeArmourName);
		equipArmour(m_EquippedNeckArmourName);
		equipWeapon(m_EquippedWandName);
		equipWeapon(m_EquippedSwordName); // equip sword last so that melee is the default combat type
		for (int i = 0; i < 16; i++)
		{
			addItemToInventory(itemsToStore[i]);
		}

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
		m_WeaponSize = m_EquippedWeapons[1].getWeaponSize();
		m_SpriteWeapon.setOrigin((64 * m_WeaponSize) / 2, (64 * m_WeaponSize) / 2);		m_SpriteWeapon.setScale(0.75, 0.75);
	}
	else
	{
		m_CombatType = Melee;
		m_WeaponSize = m_EquippedWeapons[0].getWeaponSize();
		m_SpriteWeapon.setOrigin((64 * m_WeaponSize) / 2, (64 * m_WeaponSize) / 2);		m_SpriteWeapon.setScale(0.75, 0.75);
	}
}

void Player::switchSpell(int spell)
{
	switch (spell) {
	case 1:
		m_SpellType = Fireball;
		break;

	case 2:
		m_SpellType = Heal;
		break;

	case 3:
		m_SpellType = FreezeTime;
		break;

	case 4:
		m_SpellType = Phase;
		break;
	}
}

bool Player::equipWeapon(string weaponNameToEquip)
{
	Weapon weaponToEquip(weaponNameToEquip, Vector2f(0, 0));
	if (weaponToEquip.getType() == Item::MeleeWeapon)
	{
		m_EquippedSwordName = weaponToEquip.getName();
		m_EquippedWeapons[0] = weaponToEquip;
		m_SpriteWeapon = Sprite(TextureHolder::GetTexture("graphics/player/weapon/" + weaponToEquip.getAnimType() + "/" + weaponToEquip.getName() + ".png"));
		m_WeaponSize = weaponToEquip.getWeaponSize();
		m_SpriteWeapon.setOrigin((64 * m_WeaponSize) / 2, (64 * m_WeaponSize) / 2);		m_SpriteWeapon.setScale(0.75, 0.75);
		m_CombatType = Melee;
		updateTextRect();
		return true;
	}
	else if (weaponToEquip.getType() == Item::MagicWeapon)
	{
		m_EquippedWandName = weaponToEquip.getName();
		m_EquippedWeapons[1] = weaponToEquip;
		m_SpriteWeapon = Sprite(TextureHolder::GetTexture("graphics/player/weapon/" + weaponToEquip.getAnimType() + "/" + weaponToEquip.getName() + ".png"));
		m_WeaponSize = weaponToEquip.getWeaponSize();
		m_SpriteWeapon.setOrigin((64 * m_WeaponSize) / 2, (64 * m_WeaponSize) / 2);		m_SpriteWeapon.setScale(0.75, 0.75);
		m_CombatType = Magic;
		updateTextRect();
		return true;
	}
	else
	{
		return false;
	}
}

bool Player::equipArmour(string armourNameToEquip)
{
	Equipment armourToEquip(armourNameToEquip, Vector2f(0, 0));
	if (armourToEquip.getType() == Item::HeadArmour)
	{
		// equip the armour
		m_EquippedArmour[0] = armourToEquip;

		m_SpriteHead = Sprite(TextureHolder::GetTexture("graphics/player/armour/" + armourNameToEquip + ".png"));
		m_SpriteHead.setOrigin(32, 32);		m_SpriteHead.setScale(0.75, 0.75);

		updateTextRect();
		return true;
	}
	else if (armourToEquip.getType() == Item::ChestArmour)
	{
		// equip the armour
		m_EquippedArmour[1] = armourToEquip;
		// set the appropriate sprites

		m_SpriteTorso = Sprite(TextureHolder::GetTexture("graphics/player/armour/" + armourNameToEquip + ".png"));
		m_SpriteTorso.setOrigin(32, 32);		m_SpriteTorso.setScale(0.75, 0.75);

		updateTextRect();
		return true;
	}
	else if (armourToEquip.getType() == Item::TrouserArmour)
	{
		// equip the armour
		m_EquippedArmour[2] = armourToEquip;
		// set the appropriate sprites (1 for now, more later)

		m_SpritePants = Sprite(TextureHolder::GetTexture("graphics/player/armour/" + armourNameToEquip + ".png"));
		m_SpritePants.setOrigin(32, 32);		m_SpritePants.setScale(0.75, 0.75);

		updateTextRect();
		return true;
	}
	else if (armourToEquip.getType() == Item::ShoeArmour)
	{
		// equip the armour
		m_EquippedArmour[3] = armourToEquip;
		// set the appropriate sprites (1 for now, more later)

		m_SpriteShoes = Sprite(TextureHolder::GetTexture("graphics/player/armour/" + armourNameToEquip + ".png"));
		m_SpriteShoes.setOrigin(32, 32);		m_SpriteShoes.setScale(0.75, 0.75);

		updateTextRect();
		return true;
	}
	else if (armourToEquip.getType() == Item::NeckArmour)
	{
		// equip the armour
		m_EquippedArmour[4] = armourToEquip;
		if (armourNameToEquip == "Amulet_of_Mana")
		{
			m_ManaRecharge = START_MANA_RECHARGE * 1.5f; 
		}
		else 
		{
			m_ManaRecharge = START_MANA_RECHARGE; 
		}
		if (armourNameToEquip == "Amulet_of_Healing")
		{
			regenHealth = true;
		}
		else
		{
			regenHealth = false;
		}
		if (armourNameToEquip == "Amulet_of_Stamina")
		{
			m_StaminaRecharge = START_STAMINA_RECHARGE * 1.5f;
		}
		else
		{
			m_StaminaRecharge = START_STAMINA_RECHARGE;
		}
		
		return true;
	}
	else
	{
		return false;
	}
}

vector<Weapon>& Player::getEquippedWeapons()
{
	return m_EquippedWeapons;
}

Player::CombatType Player::getCombatType()
{
	return m_CombatType;
}

Player::SpellType Player::getSpellType() {
	return m_SpellType;
}

vector<Equipment>& Player::getEquippedArmour()
{
	return m_EquippedArmour;
}

vector<Item>& Player::getStoredItems()
{
	return m_StoredItems;
}

Weapon* Player::getEquippedSword()
{
	return &m_EquippedWeapons[0];
}

Weapon* Player::getEquippedWand()
{
	return &m_EquippedWeapons[1];
}

Equipment* Player::getEquippedHeadArmour()
{
	return &m_EquippedArmour[0];
}

Equipment* Player::getEquippedChestArmour()
{
	return &m_EquippedArmour[1];
}

Equipment* Player::getEquippedTrouserArmour()
{
	return &m_EquippedArmour[2];
}

Equipment* Player::getEquippedShoeArmour()
{
	return &m_EquippedArmour[3];
}

Equipment* Player::getEquippedNeckArmour()
{
	return &m_EquippedArmour[4];
}

void Player::updateTextRect()
{
	if (!m_IsAttacking)
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
	else
	{
		if (direction == Vector2f(0, 1)) // up
		{
			setSpriteFromSheet(IntRect(0, 0, 384, 64), 64);
		}

		if (direction == Vector2f(0, -1)) // down
		{
			setSpriteFromSheet(IntRect(0, 128, 384, 64), 64);
		}

		if (direction == Vector2f(1, 0)) // right
		{
			setSpriteFromSheet(IntRect(0, 192, 384, 64), 64);
		}

		if (direction == Vector2f(-1, 0)) // left
		{
			setSpriteFromSheet(IntRect(0, 64, 384, 64), 64);
		}
	}

	m_SpritePants.setPosition(m_Position);
	m_SpriteHead.setPosition(m_Position);
	m_SpriteTorso.setPosition(m_Position);
	m_SpriteShoes.setPosition(m_Position);
	m_SpriteWeapon.setPosition(m_Position);
}

void Player::Attack()
{
	string attackType;
	string equippedWeapon;

	if (!m_IsAttacking)
	{
		resetAniCounter();
	}

	if (m_CombatType == Melee)
	{
		attackType = m_EquippedWeapons[0].getAnimType();
		equippedWeapon = m_EquippedWeapons[0].getName();
	}
	else if (m_CombatType == Magic)
	{
		attackType = m_EquippedWeapons[1].getAnimType();
		equippedWeapon = m_EquippedWeapons[1].getName();
	}

	m_IsAttacking = true;

	// Associate a texture with the body sprite
	m_SpriteWeapon = Sprite(TextureHolder::GetTexture("graphics/player/weapon/" + attackType + "/" + equippedWeapon + ".png"));
	m_SpriteWeapon.setOrigin((64 * m_WeaponSize) / 2, (64 * m_WeaponSize) / 2);
	m_SpriteWeapon.setScale(0.75, 0.75);
	
	m_Sprite = Sprite(TextureHolder::GetTexture("graphics/player/" + attackType + "/playerAttack.png"));
	m_Sprite.setOrigin(32, 32);
	m_Sprite.setScale(0.75, 0.75);

	// manually set armour sprites
	m_SpriteHead = Sprite(TextureHolder::GetTexture("graphics/player/armour/" + attackType + "/" + m_EquippedArmour[0].getName() + ".png"));
	m_SpriteHead.setOrigin(32, 32);
	m_SpriteHead.setScale(0.75, 0.75);

	m_SpriteTorso = Sprite(TextureHolder::GetTexture("graphics/player/armour/" + attackType + "/" + m_EquippedArmour[1].getName() + ".png"));
	m_SpriteTorso.setOrigin(32, 32);
	m_SpriteTorso.setScale(0.75, 0.75);

	m_SpritePants = Sprite(TextureHolder::GetTexture("graphics/player/armour/" + attackType + "/" + m_EquippedArmour[2].getName() + ".png"));
	m_SpritePants.setOrigin(32, 32);
	m_SpritePants.setScale(0.75, 0.75);

	m_SpriteShoes = Sprite(TextureHolder::GetTexture("graphics/player/armour/" + attackType + "/" + m_EquippedArmour[3].getName() + ".png"));
	m_SpriteShoes.setOrigin(32, 32);
	m_SpriteShoes.setScale(0.75, 0.75);

	updateTextRect();
}

void Player::healHealth(float healthToRestore) {
	m_Health += healthToRestore;
	if (m_Health > m_MaxHealth) {
		m_Health = m_MaxHealth;
	}
}

void Player::healMana(float manaToRestore) {
	m_Mana += manaToRestore;
	if (m_Mana > m_MaxMana) {
		m_Mana = m_MaxMana;
	}
}

void Player::healStamina(float staminaToRestore) {
	m_Stamina += staminaToRestore;
	if (m_Stamina > m_MaxStamina) {
		m_Stamina = m_MaxStamina;
	}
}

bool Player::isAttacking()
{
	return m_IsAttacking;
}

void Player::setSpritePosition(Vector2f pos)
{
	m_SpritePants.setPosition(pos);
	m_SpriteHead.setPosition(pos);
	m_SpriteTorso.setPosition(pos);
	m_SpriteShoes.setPosition(pos);
	m_SpriteWeapon.setPosition(pos);
	m_Sprite.setPosition(pos);
}

void Player::setStaminaValue(int stam) {
	m_Stamina = stam;
}

void Player::setManaValue(int mana) {
	m_Mana = mana;
}

void Player::setHealthValue(int health) {
	m_Health = health;
}

bool Player::useMana(float manaCost) 
{
	if (m_Mana >= manaCost) {
		m_Mana -= manaCost;
		return true;
	}
	else {
		return false;
	}
}

bool Player::reward(int rewardAmount)
{
	m_Kills++;
	m_Exp += rewardAmount;
	if (m_Exp >= 100)
	{
		// level up
		m_Exp -= 100;
		m_Level++;
		return true;
	}
	else
	{
		return false;
	}
}

int Player::getExp()
{
	return m_Exp;
}

bool Player::getVSync()
{
	return m_VSync;
}

int Player::getFpsLimit()
{
	return m_FpsLimit;
}

void Player::startPhase()
{
	m_Phasing = true;
}

void Player::stopPhase()
{
	m_Sprite.setColor(Color(255, 255, 255, 255)); // make player opaque again
	m_SpriteHead.setColor(Color(255, 255, 255, 255));
	m_SpriteTorso.setColor(Color(255, 255, 255, 255));
	m_SpritePants.setColor(Color(255, 255, 255, 255));
	m_SpriteShoes.setColor(Color(255, 255, 255, 255));
	m_SpriteWeapon.setColor(Color(255, 255, 255, 255));
	m_Phasing = false;
}

bool Player::isPhasing()
{
	return m_Phasing;
}

float Player::getArmourDamageReductionMult()
{
	float armourMult = 0.0f;
	for (int i = 0 ; i < m_EquippedArmour.size(); i++)
	{
		armourMult += m_EquippedArmour[i].getArmour();
	}
	if (armourMult > 80.0f)
	{
		armourMult = 80.0f; 
	}
	armourMult = 1.0f - (armourMult / 100.0f);

	return armourMult;
}

bool Player::soldSentimentalItem()
{
	return m_SoldSentimentalItem;
}

void Player::setSoldSentimentalItem(bool sold)
{
	m_SoldSentimentalItem = sold;
}