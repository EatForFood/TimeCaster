#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "TextureHolder.h"
#include "Spell.h"
#include "Pickup.h"
#include "Decal.h"
#include "Windows.h"
#include "World.h"
#include "SoundManager.h"
#include "CollisionDetection.h"
#include "Item.h"
#include "Enemy.h"
#include <string>
#include <atomic>
#include "Particle.h"
#include "Loot.h"

class Engine
{

public:
	Engine();

	void run();

	void initializeInventory();

	void initializeShop();

	enum Difficulty { Easy, Medium, Hard };

	Difficulty stringToDifficulty(std::string str);

	string difficultyToString(Difficulty difficulty);

	void moveDraggedIcon(Sprite& draggedIcon, Vector2f mousePos);


	void draw();

	void update();

	Chunk* getCurrentChunk(float x, float y);

	void populateChunkVector();

	void generateWorld();

	void spawnEnemies();

	void setDifficulty();
	
	int buyItem(int itemIndex);

	bool sellItem(int itemIndex);

	void displayInventoryTooltips();

	void displayShopTooltips();
	
	bool isEquipment(const string& name);
	bool isWeapon(const string& name);

	void restockShop(int level);

	void equipAllItems();

protected: 
	string cleanItemName(string itemName);

private:
	// The games difficulty will always be in one of these states

	// The game will always be in one of these states
	enum State { MAIN_MENU, OPTIONS_MENU, STORY_INTRO, PLAYING, PAUSED, GAME_OVER };

	CollisionDetection collision;

	// Here is the instance of TextureHolder
	TextureHolder holder;

	// Start with the MAIN_MENU state
	State state = State::MAIN_MENU;

	Vector2f resolution;

	bool windowedMode = false;

	bool vSync = false;

	int fpsLimit = 144;

	bool debugMode = true;	

	bool drawEKey = false;

	// Anything using multiple threads should use atomic variables
	std::atomic<bool> worldLoaded;

	bool levelUp = false;

	Difficulty difficulty;

	Player player;

	FloatRect viewRect;
	RenderWindow window;

	// Create a an SFML View for the main action
	View mainView;

	// Here is our clock for timing everything
	Clock clock;

	// Clock used for tracking fps
	Clock fpsClock;

	// How long has the PLAYING state been active
	Time gameTimeTotal;

	// Where is the mouse in relation to world coordinates
	Vector2f mouseWorldPosition;
	// Where is the mouse in relation to screen coordinates
	Vector2i mouseScreenPosition;

	// Colour filter 
	RectangleShape filter;

	Color defaultFilter = Color(199, 56, 20, 40);
	Color skeletonRuinsFilter = Color(199, 56, 30, 60);
	Color timeFrozenFilter = Color(173, 216, 230, 50);

	// The boundaries of the arena
	IntRect arena;

	// Create an instance of the SoundManager class
	SoundManager sound;

	Texture textureBackground;
	Texture textureBackground2;
	Texture textureBackground3;
	Texture textureBackground4;

	World world; // world object to manage chunks

	// When was the fire button last pressed?
	Time lastPressed;

	// FPS float number
	float fps = 0.f;

	bool displayFps;
	bool draggingItem = true;

	float itemLastX = 0;
	float itemLastY = 0;
	int itemLastIndex = -1;
	bool itemPlaced = false;

	//Decals 
	Decal decal[250];
	int currentDecal = 0;

	Sprite spriteCursor;
	Texture textureCursorOpen;
	Texture textureCursorClosed;

	// Integer used to set all text font sizes
	int fontSize = 35;

	// For the main menu screen
	Sprite spriteMainMenu;
	Texture textureMainMenu;

	// For the story intro screen
	Texture textureStoryIntro;
	Sprite spriteStoryIntro;

	Sprite playerInFrame;
	Sprite playerInFrameHead;
	Sprite playerInFrameTorso;
	Sprite playerInFramePants;
	Sprite playerInFrameShoes;

	View hudView;

	Font font;

	Text pausedText;

	FloatRect textBounds;
	Vector2f viewCentre;

	Text levelUpText;

	Text shopText;

	Text goldCountText;

	Text expText;

	Text levelsText;

	Text fpsText;

	RectangleShape healthBar; // Health bar (in world)
	RectangleShape healthBarContainer; // Health bar container (in world)
	RectangleShape emptyHealthBar; // Empty health bar (in world)

	RectangleShape staminaBar; // Stamina bar (in world)
	RectangleShape staminaBarContainer; // Stamina bar container (in world)
	RectangleShape emptyStaminaBar; // Empty Stamina bar (in world)

	RectangleShape manaBar; // Mana bar (in world)
	RectangleShape manaBarContainer; // Mana bar container (in world)
	RectangleShape emptyManaBar; // Empty mana bar (in world)

	Texture textureMainMenuButton1;
	Texture textureMainMenuButton2;

	Text mainHeadingText;

	// New game button
	RectangleShape newGameButton;

	// New game button text
	Text newGameButtonText;

	// Load game button
	RectangleShape loadGameButton;

	Text loadGameButtonText; // Create the Text object first

	RectangleShape optionsButton;

	Text optionsButtonText; // Create the Text object

	RectangleShape quitGameButton;

	Text quitGameButtonText; // Create the Text object

	Text optionsHeadingText; // Create the Text object

	RectangleShape mainMenuButton;

	Text mainMenuButtonText; // Create the Text object

	Text volumeSliderText;

	RectangleShape track;

	CircleShape handle;

	RectangleShape displayFPSButton;

	Text displayFPSButtonText;

	// Windowed mode button 
	RectangleShape windowedModeButton;

	Text windowedModeButtonText;

	// VSync button
	RectangleShape vSyncButton;

	Text vSyncButtonText;

	RectangleShape difficultyButton;

	Text difficultyButtonText; // Create the Text object

	RectangleShape debugModeButton;

	Text debugModeButtonText; // Create the Text object

	Text storyIntroText;
	string fullText;

	string displayedText;

	Clock textClock;
	float delay = 0.05f; // seconds between characters
	int currentChar = 0;

	Text skipIntroText; // Create the Text object

	Text loadWorldText;

	Texture textureHeadArmourFrame;
	Texture textureChestArmourFrame;
	Texture textureTrousersArmourFrame;
	Texture textureBootsArmourFrame;
	Texture textureWeaponFrame;
	Texture textureEmptyFrame;
	Texture texturePlayerFrame;
	Texture textureNeckFrame;
	Texture textureWandFrame;
	Texture textureItems;
	Texture tooltipBackground;
	Texture eKeyTexture;
	Texture inventoryBackgroundTexture;
	Texture barContainer;

	RectangleShape playerFrame;
	RectangleShape equippedSwordIcon;
	RectangleShape equippedWandIcon;
	RectangleShape equippedHeadArmourIcon;
	RectangleShape equippedChestArmourIcon;
	RectangleShape equippedTrousersArmourIcon;
	RectangleShape equippedShoeArmourIcon;
	RectangleShape equippedNeckArmourIcon;
	RectangleShape headArmourFrame;
	RectangleShape chestArmourFrame;
	RectangleShape trousersArmourFrame;
	RectangleShape bootsArmourFrame;
	RectangleShape neckFrame;
	RectangleShape weaponFrame;
	RectangleShape wandFrame;
	RectangleShape inventoryBackground;
	RectangleShape shopBackground;
	RectangleShape darkInventoryBackground;

	// Item tooltip elements
	RectangleShape itemTooltipBackground;
	Text itemTooltipName;
	Text statTooltipText;
	Text valueTooltipText;

	Text tutorialText;

	//RectangleShape emptyFrames[16];
	int draggedIndex = -1;
	int maxItems = 16; // max amount of item slots
	vector<Item>& m_StoredItems;
	vector<Item> shopItems;
	vector<RectangleShape> shopFrames;
	vector<RectangleShape> emptyFrames;
	vector<Item> allItems;     // all possible item
	vector<Weapon>& m_EquippedWeapons;
	vector<Equipment>& m_EquippedArmour;
	int currentItems = 0; // current item amount
	Item clickedItem = Item("null",Vector2f(0,0));

	RectangleShape* clickedShape = nullptr;

	Text killsText; // Create the Text object
	
	RectangleShape invHealthBar;
	RectangleShape backgroundInvHealthBar;
	Text invHealthBarText; // Create the Text object

	RectangleShape invStamBar;
	RectangleShape backgroundInvStamBar;
	Text invStamBarText;

	RectangleShape invManaBar;
	RectangleShape backgroundInvManaBar;
	Text invManaBarText;

	RectangleShape eKey;

	// When did we last update the HUD?
	int framesSinceLastHUDUpdate = 0;

	// What time was the last update
	Time timeSinceLastUpdate;

	// How often (in frames) should we update the HUD
	int fpsMeasurementFrameInterval = 1000;

	// struct to store y values and sprites together and vector to store each object
	struct DrawableItem {
		float y;
		Sprite sprite;

		DrawableItem(float y, const Sprite& sprite)
			: y(y), sprite(sprite) {
		}
	};
	vector<DrawableItem> drawables;

	vector<Item> items;

	// Boolean for whether the start game sound has played
	bool startSoundPlayed = false;

	// Boolean for whether the player is dragging the slider or not
	bool dragging = false;

	bool isDragging = false;

	// Boolean for whether to draw the inventory or not
	bool drawInventory = false;

	bool drawShop = false;

	int x;
	int y;

	vector<Enemy> enemyArr;

	vector<Chunk> chunks;

	Vector2i mousePos;
	Vector2f worldPos;

	// Skip intro animation boolean
	bool skipAnimation;

	Spell spells[100];
	Particle particles[101];
	int currentSpell = 0;
	int currentParticle = 0;

	int tutorialStage = 0;

	// Boxes and textures for player spell/melee options
	RectangleShape spellBox1; // Heal
	RectangleShape spellBox2; // Fireball
	RectangleShape spellBox3; // Phase Shift
	RectangleShape spellBox4; // Freeze/slow time
	RectangleShape wandBox;
	RectangleShape swordBox;
	RectangleShape weaponIndicator;
	RectangleShape spellIndicator;

	Texture indicator;
	Texture healSpell;
	Texture freezeTimeSpell;
	Texture fireBallSpell;
	Texture phaseSpell;

	RectangleShape swordIcon;
	RectangleShape wandIcon;
	RectangleShape spell1Icon;
	RectangleShape spell2Icon;
	RectangleShape spell3Icon;
	RectangleShape spell4Icon;

	enum CombatType { Melee, Magic };

	bool timeFrozen;
	Clock timeFrozenTimer;

	// Textbox logic
	sf::RectangleShape textBox;
	sf::Text feedback;
	sf::Text userInputText;
	std::string userInputString;
	bool textBoxActive;


	// Textbox logic
	sf::RectangleShape textBoxFps;
	sf::Text feedbackFps;
	sf::Text userInputTextFps;
	std::string userInputStringFps;
	bool textBoxActiveFps;

	string lastSoldItemName = "";

	Loot loot;
};