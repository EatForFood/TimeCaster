#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"
#include "Decal.h"
#include "Windows.h"
#include "World.h"
#include "SoundManager.h"
#include "CollisionDetection.h"
#include "Item.h"
#include "Enemy.h"
#include <string>;

using namespace sf;

class Engine
{
private:
	// The games difficulty will always be in one of these states
	enum class Difficulty { Easy, Medium, Hard };

	// The game will always be in one of these states
	enum class State { MAIN_MENU, OPTIONS_MENU, STORY_INTRO, PLAYING, PAUSED, GAME_OVER };

	CollisionDetection collision;

	// Here is the instance of TextureHolder
	TextureHolder holder;

	// Start with the MAIN_MENU state
	State state = State::MAIN_MENU;

	Vector2f resolution;

	bool windowedMode = false;

	Difficulty difficulty;

	Player player;
	Enemy enemy;

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

	// The boundaries of the arena
	IntRect arena;

	// Create an instance of the SoundManager class
	SoundManager sound;

	Texture textureBackground;

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

	// For the home/game over screen
	Sprite spriteMainMenu;
	Texture textureMainMenu;

	View hudView;

	Font font;

	Text pausedText;

	FloatRect textBounds;
	Vector2f viewCentre;

	Text levelUpText;
	stringstream levelUpStream;

	Text goldCountText;

	Text fpsText;

	RectangleShape healthBar;
	// Empty health bar
	RectangleShape emptyHealthBar;

	// Stamina bar
	RectangleShape staminaBar;
	// Empty Stamina bar
	RectangleShape emptyStaminaBar;

	// Mana bar
	RectangleShape manaBar;
	// Empty mana bar
	RectangleShape emptyManaBar;

	Texture textureMainMenuButton1;
	Texture textureMainMenuButton2;

	Text mainHeadingText;

	// New game button
	RectangleShape newGameButton;

	// New game button text
	Text newGameButtonText;

	// Load game button
	RectangleShape loadGameButton;

	Text loadGameButtonText;     // Create the Text object first

	RectangleShape optionsButton;

	Text optionsButtonText;                 // Create the Text object

	RectangleShape quitGameButton;

	Text quitGameButtonText;                    // Create the Text object

	Text optionsHeadingText;                      // Create the Text object

	RectangleShape mainMenuButton;

	Text mainMenuButtonText;                        // Create the Text object

	Text volumeSliderText;

	RectangleShape track;

	CircleShape handle;

	RectangleShape displayFPSButton;

	Text displayFPSButtonText;

	// Windowed mode button 
	RectangleShape windowedModeButton;

	Text windowedModeButtonText;

	RectangleShape difficultyButton;

	Text difficultyButtonText;                                       // Create the Text object

	Text storyIntroText;
	string fullText;

	string displayedText;

	Clock textClock;
	float delay = 0.05f; // seconds between characters
	int currentChar = 0;

	Text skipIntroText;                             // Create the Text object

	Texture textureHeadArmourFrame;
	Texture textureChestArmourFrame;
	Texture textureTrousersArmourFrame;
	Texture textureBootsArmourFrame;
	Texture textureWeaponFrame;
	Texture textureEmptyFrame;
	Texture texturePlayerFrame;
	Texture texturePlayerInFrame;
	Texture textureNeckFrame;
	Texture textureRingFrame;
	Texture textureItems;

	RectangleShape playerFrame;
	RectangleShape equippedWeaponIcon;
	RectangleShape playerInFrame;
	RectangleShape headArmourFrame;
	RectangleShape chestArmourFrame;
	RectangleShape trousersArmourFrame;
	RectangleShape bootsArmourFrame;
	RectangleShape neckFrame;
	RectangleShape weaponFrame;
	RectangleShape ringFrame;
	//RectangleShape emptyFrames[16];
	int draggedIndex = -1;
	int maxItems = 16; // max amount of item slots
	vector<Item> storedItems;
	vector<RectangleShape> emptyFrames;
	vector<Item> allItems;     // all possible ite
	int currentItems = 0; // current item amount
	Item clickedItem = Item("null",Vector2f(0,0));

	RectangleShape itemIcon[16];
	RectangleShape* clickedShape = nullptr;

	Text killsText;                                      // Create the Text objec
	
	RectangleShape invHealthBar;
	RectangleShape backgroundInvHealthBar;
	Text invHealthBarText;                   // Create the Text object

	RectangleShape invStamBar;
	RectangleShape backgroundInvStamBar;
	Text invStamBarText;

	RectangleShape invManaBar;
	RectangleShape backgroundInvManaBar;
	Text invManaBarText;

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

	bool debugreset = true; //it's a bit of a hack but it works to stop multiple upgrades from one key press
	//press numpad0 to reset if you want to test again
	//remove this in full build

	// Boolean for whether the start game sound has played
	bool startSoundPlayed = false;

	// Boolean for whether the player is dragging the slider or not
	bool dragging = false;

	bool isDragging = false;

	// Boolean for whether to draw the inventory or not
	bool drawInventory = false;

	int x;
	int y;

public:
	Engine();

	void run();

	void initializeInventory();

	//Difficulty stringToDifficulty(std::string str);

	string difficultyToString(Difficulty difficulty);

	void moveDraggedIcon(Sprite& draggedIcon, Vector2f mousePos);
};