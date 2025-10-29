#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "NavBox.h"
#include "CollisionDetection.h"
#include "Item.h"
#include "Equipment.h"
#include "Weapon.h"

using namespace sf;
using namespace std;

class Player : public Character
{

public:

	Player();

	void spawn(IntRect arena, Vector2f resolution, int tileSize, int level);

	enum CombatType { Melee, Magic };

	// Handle the player getting hit by an enemy
	bool hit(Time timeHit, float damage, int iFrames);

	// How long ago was the player last hit
	Time getLastHitTime();

	// The next four functions move the player
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	// Stop the player moving in a specific direction
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();
	CombatType getCombatType();

	// We will call this function once every frame
	void update(float elapsedTime, Vector2i mousePosition, vector<NavBox> navBox);

	// Give player a speed boost
	void upgradeSpeed();

	// Give the player a health upgrade
	void upgradeHealth();

	// Give the player a Stamina upgrade
	void upgradeStamina();

	// Give the player a Mana upgrades
	void upgradeMana();

	// Increase the maximum amount of health the player can have
	void increaseHealthLevel(int amount);

	void dodge(); // check for dodge input and handle dodge timer

	// Player starts a dodge
	void startDodge();

	// Player stops a dodge
	void stopDodge();

	void increaseManaLevel(int amount);

	void increaseStaminaLevel(int amount);

	float getStamina();
	float getMaxStamina();
	float getStaminaRecharge();
	int getGold();
	void setGold(int amount);
	float getSpeed();
	int getPlayerLevel();
	void setPlayerLevel(int level);

	float getMana();
	float getMaxMana();

	void revertPosition();
	void disableUp();
	void disableDown();
	void disableRight();
	void disableLeft();

	// make a new save file with default values
	void createNewSave();
	void createConfigFile(string difficultyString, bool windowedMode, bool displayFPS, float volume);
	//save and load player stats/position/etc.
	void updateSaveFile();
	//load player stats/position/etc. from save file
	bool loadSaveFile();
	bool loadConfigFile();
	string getdifficultyString();
	bool getWindowedMode();
	bool getDisplayFps();
	float getVolume();

	void setChunk(int chunk);
	int getChunk();

	vector<Weapon>& getEquippedWeapons();

	FloatRect getRenderArea();

	void incrementKillCount();
	int getKillCount();

	//Item getSavedSword();
	//Item getSavedWand();
	//Item getEquippedWeapon();

	void switchWeapon();
	bool equipWeapon(string weaponNameToEquip);

	vector<Equipment>& getEquippedArmour();

	Weapon* getEquippedSword();
	Weapon* getEquippedWand();
	Equipment* getEquippedHeadArmour();
	Equipment* getEquippedChestArmour();
	Equipment* getEquippedTrouserArmour();
	Equipment* getEquippedShoeArmour();
	Equipment* getEquippedNeckArmour();
		

	void setInCell();
	bool getInCell();

	void AttackAnimation(string attackType);

	bool equipArmour(string armourNameToEquip);

private:
	const float START_SPEED = 100;
	const float START_HEALTH = 100;
	const float START_MANA = 100;
	const float START_STAMINA = 100;
	const float START_STAMINA_RECHARGE = 5;
	const int START_GOLD = 0;
	const int START_KILLS = 0;
	const int START_LEVEL = 1;
	const string START_SWORD = "Iron_Sword";
	const string START_WAND = "Wooden_Wand";
	const string START_HEAD_ARMOUR = "Family_Hood";
	const string START_CHEST_ARMOUR = "Family_Robe";
	const string START_TROUSER_ARMOUR = "Robe_Leggings";	
	const string START_SHOE_ARMOUR = "Basic_Shoes";
	const string START_NECK_ARMOUR = "Family_Locket";
	void updateTextRect();
	

	// Which directions is the player currently moving in
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;


	// Player stats
	float m_Mana;
	float m_MaxMana;
	float m_Stamina;
	float m_MaxStamina;
	float m_StaminaRecharge;
	int m_Gold;
	int m_Kills;

	string m_EquippedSwordName;
	string m_EquippedWandName;
	string m_EquippedHeadArmourName;
	string m_EquippedChestArmourName;
	string m_EquippedTrouserArmourName;
	string m_EquippedShoeArmourName;
	string m_EquippedNeckArmourName;

	bool m_WindowedMode;

	// Dodge variables
	bool m_IsDodging = false;
	bool m_CanDodge = false;
	Clock m_DodgeClock;
	Clock m_CooldownClock;
	float m_DodgeDuration = 0.2f; // 200ms dodge
	float m_DodgeCooldown = 1.0f; // 1 second cooldown on dodge
	string m_DifficultyString;
	bool m_DisplayFPS;
	float m_Volume;

	CombatType m_CombatType = Melee;
	
	// When was the player last hit
	Time m_LastHit;
	// How many iFrames the player has after being hit
	int m_IFrames = 0;
	
	// If the player is moving or not
	bool m_IsMoving = false;

	bool upDisabled = false;
	bool downDisabled = false;
	bool rightDisabled = false;
	bool leftDisabled = false;

	CollisionDetection collision;
	vector<NavBox> navBoxes;
//	vector<Item> equippedItems;
	// always store the sword at index 0 and wand at index 1
	vector<Weapon> m_EquippedWeapons;
	// store helmet at index 0, torso at index 1, pants at index 2, shoes at index 3, amulet/neck slot at index 4
	vector<Equipment> m_EquippedArmour;
	
	int m_Chunk; // player's current chunk

	FloatRect m_RenderArea;


	bool inCell = false;

	bool m_IsAttacking = false;
	float m_AttackTimer = 0.0f;
	float m_AttackDuration = 0.8f;
};
