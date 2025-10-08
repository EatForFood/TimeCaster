#include <sstream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "TimeCaster.h"
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

using namespace std;
using namespace sf;

string difficultyToString(Difficulty difficulty)
{
	switch (difficulty)
	{
	case Difficulty::Easy:   return "Easy";
	case Difficulty::Medium: return "Medium";
	case Difficulty::Hard:   return "Hard";
	}
	return "Unknown";
}

Difficulty stringToDifficulty(string str)
{
	if (str == "Easy") {return Difficulty::Easy; }
	else if (str == "Medium") {return Difficulty::Medium; }
	else if (str == "Hard") { return Difficulty::Hard; }
	else return Difficulty::Medium;
}

int main()
{	
	CollisionDetection collision;

	// Here is the instance of TextureHolder
	TextureHolder holder;
	
	// Start with the MAIN_MENU state
	State state = State::MAIN_MENU;

	bool windowedMode = false;

	// Start with the Medium difficulty state
	Difficulty difficulty = Difficulty::Medium;

	Player player;
	Enemy enemy;

	bool displayFps;

	player.loadConfigFile();

	difficulty = stringToDifficulty(player.getdifficultyString());
	windowedMode = player.getWindowedMode();
	displayFps = player.getDisplayFps();
	Listener::setGlobalVolume(player.getVolume());

	// Get the screen resolution and create an SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	resolution.x = 1920;
	resolution.y = 1080;

	//RenderWindow window(VideoMode(resolution.x, resolution.y), "TimeCaster", Style::Fullscreen);
	
	RenderWindow window;

	if (windowedMode == true)
	{
		 window.create(VideoMode(resolution.x, resolution.y), "TimeCaster", Style::Default);
	}
	else 	{
		 window.create(VideoMode(resolution.x, resolution.y), "TimeCaster", Style::Fullscreen);
	}

	// Create a an SFML View for the main action
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Zoom view
	mainView.zoom(0.3f);

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

	// Create an instance of the Player class

	// Colour filter 
	RectangleShape filter;
	filter.setSize(Vector2f(1920,1080));
	filter.setFillColor(Color(199, 56, 20, 40));

	player.loadConfigFile();


	// The boundaries of the arena
	IntRect arena;

	// Create an instance of the SoundManager class
	SoundManager sound;

	// Create an instance of the CreateBackground class
	//Chunk landscape;

	// Create the background
	// VertexArray background;
	// Load the texture for our background vertex array
	Texture textureBackground = TextureHolder::GetTexture("graphics/landscape.png");

	/*
	// 100 bullets should do
	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	*/
	World world; // world object to manage chunks

	// FPS float number
	float fps = 0.f;

	// Boolean for whether to display the fps
//	bool displayFps = false;
	
	// When was the fire button last pressed?
	Time lastPressed;

	//Decals 
	Decal decal[250];
	int currentDecal = 0;

	// Hide the mouse pointer and replace it with crosshair
	window.setMouseCursorVisible(true);
	Sprite spriteCursor;
	Texture textureCursorOpen = TextureHolder::GetTexture("graphics/knightCursorOpen.png");
	Texture textureCursorClosed = TextureHolder::GetTexture("graphics/knightCursorClosed.png");
	spriteCursor.setTexture(textureCursorOpen);
	spriteCursor.setScale(0.4,0.4);
	spriteCursor.setOrigin(25, 25);

	// Create a couple of pickups
	Pickup healthPickup(1);
	Pickup ammoPickup(2);
	Pickup staminaPickup(3);
	Pickup manaPickup(4);

	// Integer used to set all text font sizes
	int fontSize = 35;

	// For the home/game over screen
	Sprite spriteMainMenu;
	Texture textureMainMenu = TextureHolder::GetTexture("graphics/UI/Castle (edited).jpg");
	spriteMainMenu.setTexture(textureMainMenu);
	spriteMainMenu.setPosition(0, 0);

	// Create a view for the HUD
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Main font
	Font font;
	font.loadFromFile("fonts/PixelifySans-Bold.ttf");

	// Paused text
	Text pausedText("Press escape \nto continue", font, 130);
	pausedText.setFillColor(Color::White);
	FloatRect textBounds = pausedText.getLocalBounds();
	Vector2f viewCentre = mainView.getCenter();
	pausedText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 400);

	// Levelling up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	stringstream levelUpStream;
	levelUpStream <<
		"1- Increased rate of fire" <<
		"\n2- Increased clip size(next reload)" <<
		"\n3- Increased max health" <<
		"\n4- Increased run speed" <<
		"\n5- More and better health pickups" <<
		"\n6- More and better ammo pickups";
	levelUpText.setString(levelUpStream.str());

	// Gold text
	Text goldCountText("Gold: " + player.getGold(), font, fontSize);
	goldCountText.setFillColor(Color::Black);
	textBounds = goldCountText.getLocalBounds();
	goldCountText.setPosition(viewCentre.x - 45, 360);

	// FPS text
	Text fpsText;
	fpsText.setFont(font);
	fpsText.setCharacterSize(20);
	fpsText.setFillColor(Color::White);
	fpsText.setPosition(1800, 5);

	// Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(10, 10);

	// Empty health bar
	RectangleShape emptyHealthBar;
	emptyHealthBar.setFillColor(Color::Black);
	emptyHealthBar.setPosition(10, 10);

	// Stamina bar
	RectangleShape staminaBar;
	staminaBar.setFillColor(Color::Green);
	staminaBar.setPosition(10, 60);

	// Empty Stamina bar
	RectangleShape emptyStaminaBar;
	emptyStaminaBar.setFillColor(Color::Black);
	emptyStaminaBar.setPosition(10, 60);

	// Mana bar
	RectangleShape manaBar;
	manaBar.setFillColor(Color::Magenta);
	manaBar.setPosition(10, 110);

	// Empty mana bar
	RectangleShape emptyManaBar;
	emptyManaBar.setFillColor(Color::Black);
	emptyManaBar.setPosition(10, 110);

	/***********
	Main Menu UI
	************/
	Texture& textureMainMenuButton1 = TextureHolder::GetTexture("graphics/UI/menuButton1.png");
	Texture& textureMainMenuButton2 = TextureHolder::GetTexture("graphics/UI/menuButton2.png");

	// TimeCaster heading text
	Text mainHeadingText("TimeCaster", font, fontSize + 65);
	mainHeadingText.setFillColor(Color::White);
	textBounds = mainHeadingText.getLocalBounds();
	viewCentre = mainView.getCenter();
	mainHeadingText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 10);
	
	// New game button
	RectangleShape newGameButton;
	newGameButton.setPosition(200, 210);
	newGameButton.setSize(Vector2f(300, 80));
	newGameButton.setTexture(&textureMainMenuButton1);

	// New game button text
	Text newGameButtonText("New Game", font, fontSize);
	newGameButtonText.setFillColor(Color::Yellow);
	textBounds = newGameButtonText.getLocalBounds();
	float x = newGameButton.getPosition().x + (newGameButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	float y = newGameButton.getPosition().y + (newGameButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	newGameButtonText.setPosition(x - textBounds.left, y - textBounds.top - 8);

	// Load game button
	RectangleShape loadGameButton;
	loadGameButton.setPosition(200, 320);
	loadGameButton.setSize(Vector2f(300, 80));
	loadGameButton.setTexture(&textureMainMenuButton2);

	// Load game button text
	Text loadGameButtonText("Load Game", font, fontSize);
	loadGameButtonText.setFillColor(Color::White);
	textBounds = loadGameButtonText.getLocalBounds();
	x = loadGameButton.getPosition().x + (loadGameButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	y = loadGameButton.getPosition().y + (loadGameButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	loadGameButtonText.setPosition(x - textBounds.left, y - textBounds.top);

	// Options button
	RectangleShape optionsButton;
	optionsButton.setPosition(200, 430);
	optionsButton.setSize(Vector2f(300, 80));
	optionsButton.setTexture(&textureMainMenuButton2);

	// options button text
	Text optionsButtonText("Options", font, fontSize);
	optionsButtonText.setFillColor(Color::White);
	textBounds = optionsButtonText.getLocalBounds();
	x = optionsButton.getPosition().x + (optionsButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	y = optionsButton.getPosition().y + (optionsButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	optionsButtonText.setPosition(x - textBounds.left, y - textBounds.top);

	// Quit game button
	RectangleShape quitGameButton;
	quitGameButton.setPosition(200, 540);
	quitGameButton.setSize(Vector2f(300, 80));
	quitGameButton.setTexture(&textureMainMenuButton2);

	// Quit game button text
	Text quitGameButtonText("Quit Game", font, fontSize);
	quitGameButtonText.setFillColor(Color::White);
	textBounds = quitGameButtonText.getLocalBounds();
	x = quitGameButton.getPosition().x + (quitGameButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	y = quitGameButton.getPosition().y + (quitGameButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	quitGameButtonText.setPosition(x - textBounds.left, y - textBounds.top);

	/**************
	Options Menu UI
	***************/
	
	// Options heading text
	Text optionsHeadingText("Options", font, fontSize + 15);
	optionsHeadingText.setFillColor(Color::White);
	textBounds = optionsHeadingText.getLocalBounds();
	viewCentre = mainView.getCenter();
	optionsHeadingText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 10);

	// Main menu button
	RectangleShape mainMenuButton;
	mainMenuButton.setSize(Vector2f(300, 80));
	textBounds = mainMenuButton.getLocalBounds();
	viewCentre = mainView.getCenter();
	mainMenuButton.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 975);
	mainMenuButton.setTexture(&textureMainMenuButton2);

	// Main menu button text
	Text mainMenuButtonText("Save & Exit", font, fontSize - 5);
	mainMenuButtonText.setFillColor(Color::White);
	textBounds = mainMenuButtonText.getLocalBounds();
	x = mainMenuButton.getPosition().x + (mainMenuButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	y = mainMenuButton.getPosition().y + (mainMenuButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	mainMenuButtonText.setPosition(x - textBounds.left, y - textBounds.top);

	// Volume slider text
	Text volumeSliderText("Volume", font, fontSize);
	volumeSliderText.setFillColor(Color::Black);
	textBounds = volumeSliderText.getLocalBounds();
	viewCentre = mainView.getCenter();
	volumeSliderText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 150);

	// Slider track
	RectangleShape track(Vector2f(400, 5));
	track.setFillColor(sf::Color::Black);
	textBounds = track.getLocalBounds();
	viewCentre = mainView.getCenter();
	int trackY = 250;
	track.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, trackY);

	// Slider handle
	CircleShape handle(10);
	handle.setFillColor(Color::Red);
	handle.setOrigin(10, 10); // Centre the circle
	textBounds = handle.getLocalBounds();
	x = track.getPosition().x + (track.getSize().x / 2.f) - (textBounds.width / 2.f);
	handle.setPosition(x - textBounds.left, trackY + 2); // Slider start at 50% volume

	// Display FPS button
	RectangleShape displayFPSButton;
	if (displayFps) {
		displayFPSButton.setFillColor(Color::Green);
	}
	else {
		displayFPSButton.setFillColor(Color::Red);
	}
	displayFPSButton.setSize(Vector2f(300, 80));
	textBounds = displayFPSButton.getLocalBounds();
	viewCentre = mainView.getCenter();
	displayFPSButton.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 300);
	displayFPSButton.setTexture(&textureMainMenuButton2);

	// Display FPS button text
	Text displayFPSButtonText("Display FPS", font, fontSize - 5);
	displayFPSButtonText.setFillColor(Color::Black);
	textBounds = displayFPSButtonText.getLocalBounds();
	x = displayFPSButton.getPosition().x + (displayFPSButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	y = displayFPSButton.getPosition().y + (displayFPSButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	displayFPSButtonText.setPosition(x - textBounds.left, y - textBounds.top);

	// Windowed mode button 
	RectangleShape windowedModeButton;
	if (windowedMode)
	{
		windowedModeButton.setFillColor(Color::Green);
	}
	else
	{
		windowedModeButton.setFillColor(Color::Red);
	}
	windowedModeButton.setSize(Vector2f(400, 80));
	textBounds = windowedModeButton.getLocalBounds();
	viewCentre = mainView.getCenter();
	windowedModeButton.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 500);
	windowedModeButton.setTexture(&textureMainMenuButton2);

	// Windowed mode button text
	Text windowedModeButtonText("Windowed Mode", font, fontSize - 5);
	windowedModeButtonText.setFillColor(Color::Black);
	textBounds = windowedModeButtonText.getLocalBounds();
	x = windowedModeButton.getPosition().x + (windowedModeButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	y = windowedModeButton.getPosition().y + (windowedModeButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	windowedModeButtonText.setPosition(x - textBounds.left, y - textBounds.top);

	// Display difficulty button
	RectangleShape difficultyButton;
	if (difficulty == Difficulty::Easy)
	{
		difficultyButton.setFillColor(Color::Green);
	}
	else if (difficulty == Difficulty::Medium)
	{
		difficultyButton.setFillColor(Color::Yellow);
	}
	else
	{
		difficultyButton.setFillColor(Color::Red);
	}
	difficultyButton.setSize(Vector2f(450, 100));
	textBounds = difficultyButton.getLocalBounds();
	viewCentre = mainView.getCenter();
	difficultyButton.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 400);
	difficultyButton.setTexture(&textureMainMenuButton2);

	// Display difficulty button text
	Text difficultyButtonText("Difficulty: " + difficultyToString(difficulty), font, fontSize - 5);
	difficultyButtonText.setFillColor(Color::Black);
	textBounds = difficultyButtonText.getLocalBounds();
	x = difficultyButton.getPosition().x + (difficultyButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	y = difficultyButton.getPosition().y + (difficultyButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	difficultyButtonText.setPosition(x - textBounds.left, y - textBounds.top);

	// Story into text
	Text storyIntroText(
		"I was not always a man consumed by vengeance. Once, I had a family-warm laughter by the fire, \n"
		"the gentle touch of my children's hands, the steady love of my wife. \n"
		"All of it was torn from me in a single night, \n"
		"devoured by the fire of (name), a dragon whose name still burns in my mind. \n"
		"Since then, every spell I've mastered, every scar I've earned, has been for one purpose alone: \n"
		"to bring that beast to its knees. \n"
		"I do not seek glory, nor the hollow praise of men-I seek redemption. \n"
		"And when the dragon falls, so too shall the weight of my failure. \n\n"
		"You desire retribution dear player, but what will it cost you?",
		font, fontSize);
	storyIntroText.setFillColor(Color::White);
	textBounds = storyIntroText.getLocalBounds();
	viewCentre = mainView.getCenter();
	storyIntroText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 150);

	// Skip intro text
	Text skipIntroText("--- Press space to skip ---", font, fontSize - 5);
	skipIntroText.setFillColor(Color::White);
	textBounds = skipIntroText.getLocalBounds();
	viewCentre = mainView.getCenter();
	skipIntroText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 1000);

	/***********
	Inventory UI
	************/

	Texture& textureHeadArmourFrame = TextureHolder::GetTexture("graphics/UI/headFrame.png");
	Texture& textureChestArmourFrame = TextureHolder::GetTexture("graphics/UI/chestFrame.png");
	Texture& textureTrousersArmourFrame = TextureHolder::GetTexture("graphics/UI/trousersFrame.png");
	Texture& textureBootsArmourFrame = TextureHolder::GetTexture("graphics/UI/bootsFrame.png");
	Texture& textureWeaponFrame = TextureHolder::GetTexture("graphics/UI/weaponFrame.png");
	Texture& textureEmptyFrame = TextureHolder::GetTexture("graphics/UI/emptyFrame.png");
	Texture& texturePlayerFrame = TextureHolder::GetTexture("graphics/UI/playerFrame.png");
	Texture& texturePlayerInFrame = TextureHolder::GetTexture("graphics/UI/player.png");
	Texture& textureNeckFrame = TextureHolder::GetTexture("graphics/UI/neckFrame.png");
	Texture& textureRingFrame = TextureHolder::GetTexture("graphics/UI/ringFrame.png");
	
	Texture& textureItems = TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png");

	
	// Player frame
	RectangleShape playerFrame;
	playerFrame.setSize(sf::Vector2f(100.f, 200.f));
	playerFrame.setTexture(&texturePlayerFrame);
	playerFrame.setOrigin(playerFrame.getSize() / 2.f);
	playerFrame.setPosition(viewCentre.x - 200, 400);

	RectangleShape equippedWeaponIcon;
	
	equippedWeaponIcon.setTexture(&textureItems);
	//equippedWeaponIcon.setTextureRect(player.getEquippedWeaponIcon());
	equippedWeaponIcon.setSize(Vector2f(75, 75));
	equippedWeaponIcon.setOrigin(equippedWeaponIcon.getSize() / 2.f);
	equippedWeaponIcon.setPosition(viewCentre.x - 200, 550);

	// Player sprite for frame
	RectangleShape playerInFrame;
	playerInFrame.setSize(sf::Vector2f(60.f, 100.f));
	playerInFrame.setTexture(&texturePlayerInFrame);
	playerInFrame.setOrigin(playerInFrame.getSize() / 2.f);
	playerInFrame.setPosition(viewCentre.x - 200, 400);

	RectangleShape headArmourFrame;
	headArmourFrame.setTexture(&textureHeadArmourFrame);
	headArmourFrame.setSize(Vector2f(75, 75));
	headArmourFrame.setOrigin(headArmourFrame.getSize() / 2.f);
	headArmourFrame.setPosition(viewCentre.x - 300, 350);

	RectangleShape chestArmourFrame;
	chestArmourFrame.setTexture(&textureChestArmourFrame);
	chestArmourFrame.setSize(Vector2f(75, 75));
	chestArmourFrame.setOrigin(chestArmourFrame.getSize() / 2.f);
	chestArmourFrame.setPosition(viewCentre.x - 100, 350);

	RectangleShape trousersArmourFrame;
	trousersArmourFrame.setTexture(&textureTrousersArmourFrame);
	trousersArmourFrame.setSize(Vector2f(75, 75));
	trousersArmourFrame.setOrigin(trousersArmourFrame.getSize() / 2.f);
	trousersArmourFrame.setPosition(viewCentre.x - 300, 450);

	RectangleShape bootsArmourFrame;
	bootsArmourFrame.setTexture(&textureBootsArmourFrame);
	bootsArmourFrame.setSize(Vector2f(75, 75));
	bootsArmourFrame.setOrigin(bootsArmourFrame.getSize() / 2.f);
	bootsArmourFrame.setPosition(viewCentre.x - 100, 450);

	RectangleShape neckFrame;
	neckFrame.setTexture(&textureNeckFrame);
	neckFrame.setSize(Vector2f(75, 75));
	neckFrame.setOrigin(neckFrame.getSize() / 2.f);
	neckFrame.setPosition(viewCentre.x - 300, 550);

	RectangleShape weaponFrame;
	weaponFrame.setTexture(&textureWeaponFrame);
	weaponFrame.setSize(Vector2f(75, 75));
	weaponFrame.setOrigin(weaponFrame.getSize() / 2.f);
	weaponFrame.setPosition(viewCentre.x - 200, 550);

	RectangleShape ringFrame;
	ringFrame.setTexture(&textureRingFrame);
	ringFrame.setSize(Vector2f(75, 75));
	ringFrame.setOrigin(ringFrame.getSize() / 2.f);
	ringFrame.setPosition(viewCentre.x - 100, 550);

	RectangleShape emptyFrames[16];
	int startX = viewCentre.x - 300;
	int startY = 650;
	for (int i = 0; i < sizeof(emptyFrames) / sizeof(emptyFrames[0]); i++) {
		emptyFrames[i].setTexture(&textureEmptyFrame);
		emptyFrames[i].setSize(Vector2f(75, 75));
		emptyFrames[i].setOrigin(emptyFrames[i].getSize() / 2.f);
		if (i != 0 && i % 8 == 0) {
			startY += 100;
			startX = viewCentre.x - 300;
		}
		emptyFrames[i].setPosition(startX, startY);
		startX += 100;
	}

	// Display kill count inventory text
	Text killsText("Kills: " + player.getKillCount(), font, fontSize);
	killsText.setFillColor(Color::Black);
	textBounds = killsText.getLocalBounds();
	killsText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 310);

	RectangleShape invHealthBar;
	invHealthBar.setFillColor(Color::Red);
	invHealthBar.setPosition(viewCentre.x - 310, 825);

	RectangleShape backgroundInvHealthBar;
	backgroundInvHealthBar.setFillColor(Color::Black);
	backgroundInvHealthBar.setSize(Vector2f(200, 50));
	backgroundInvHealthBar.setPosition(viewCentre.x - 310, 825);

	// Display invHealthBar text
	Text invHealthBarText("0 / 0", font, fontSize - 5);
	invHealthBarText.setFillColor(Color::White);

	RectangleShape invStamBar;
	invStamBar.setFillColor(Color::Green);
	invStamBar.setPosition(viewCentre.x - 60, 825);

	RectangleShape backgroundInvStamBar;
	backgroundInvStamBar.setFillColor(Color::Black);
	backgroundInvStamBar.setSize(Vector2f(200, 50));
	backgroundInvStamBar.setPosition(viewCentre.x - 60, 825);

	// Display invStamBar text
	Text invStamBarText("0 / 0", font, fontSize - 5);
	invStamBarText.setFillColor(Color::White);

	RectangleShape invManaBar;
	invManaBar.setFillColor(Color::Magenta);
	invManaBar.setPosition(viewCentre.x + 190, 825);

	RectangleShape backgroundInvManaBar;
	backgroundInvManaBar.setFillColor(Color::Black);
	backgroundInvManaBar.setSize(Vector2f(200, 50));
	backgroundInvManaBar.setPosition(viewCentre.x + 190, 825);

	// Display invManaBar text
	Text invManaBarText("0 / 0", font, fontSize - 5);
	invManaBarText.setFillColor(Color::White);
	
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
			: y(y), sprite(sprite) {}
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



	// Setting volume to 50 by default
	Listener::setGlobalVolume(50);

	// Populate soundtrack
	sound.populateSoundtrack();

	// The main game loop
	while (window.isOpen())
	{
		if (displayFps) {
			// Calculating fps
			float deltaTime = fpsClock.restart().asSeconds();
			fps = 1.f / deltaTime;
			fpsText.setString("FPS: " + to_string((int)fps));
		}
		
		/***********
		Handle input
		************/

		// Handle events
		Event event;
		while (window.pollEvent(event))
		{
			// Getting the mouse position and mapping those pixels to coordinates
			Vector2i mousePos = Mouse::getPosition(window);
			Vector2f worldPos = window.mapPixelToCoords(mousePos);

			if (event.type == Event::MouseWheelScrolled)
			{
				if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel)
				{
					if (event.mouseWheelScroll.delta > 0)
					{
						mainView.zoom(0.9f);
						spriteCursor.scale(0.9f, 0.9f);
						
					}
					else if (event.mouseWheelScroll.delta < 0)
					{
						mainView.zoom(1.1f);
						spriteCursor.scale(1.1f, 1.1f);
					}
				}
			}

		if ((event.type == Event::MouseButtonPressed && event.key.code == Mouse::Middle && state == State::PLAYING) ||
			(event.type == Event::KeyPressed && event.key.code == Keyboard::F && state == State::PLAYING))
		{

			//cout << " Weapon switched" << endl;

			player.switchWeapon();
			equippedWeaponIcon.setTextureRect(player.getEquippedWeaponIcon());


		}

			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && state == State::OPTIONS_MENU)
			{
				if (handle.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
				{
					dragging = true;
				}
			}

			// Stop dragging
			if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
			{
				dragging = false;
			}

			if (event.type == Event::KeyPressed || Mouse::isButtonPressed(Mouse::Left))
			{
				// Pause a game while playing
				if (event.key.code == Keyboard::Escape && state == State::PLAYING && !drawInventory)
				{
					state = State::PAUSED;
				}

				// Restart while paused
				else if (event.key.code == Keyboard::Escape && state == State::PAUSED)
				{
					state = State::PLAYING;
					// Reset the clock so there isn't a frame jump
					clock.restart();
				}

				// Player hit the new game button in the main menu
				else if (newGameButton.getGlobalBounds().contains(worldPos) && state == State::MAIN_MENU && event.mouseButton.button == Mouse::Left )
				{
					state = State::STORY_INTRO;
					
					// Play the start game sound
					if (!startSoundPlayed) {
						sound.playStartGameSound();
					}

					sound.playStoryIntroSound();

					startSoundPlayed = true;
					
					player.createNewSave();
					player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume());
					player.loadSaveFile();



					// We will modify the next two lines later
					arena.width = 1900;
					arena.height = 800;
					arena.left = 1664;
					arena.top = 1664;

					// Pass the vertex array by reference 
					// to the createBackground function
					int tileSize = 64;

					// Spawn the player in the middle of the arena
					player.spawn(arena, resolution, tileSize, player.getPlayerLevel());
					enemy.spawn(arena, resolution, tileSize, "Goblin", player.getPlayerLevel());

					// Configure the pick-ups
					healthPickup.setArena(arena);
					ammoPickup.setArena(arena);
					staminaPickup.setArena(arena);
					manaPickup.setArena(arena);

					// Reset the clock so there isn't a frame jump
					clock.restart();
				
			

					equippedWeaponIcon.setTextureRect(player.getEquippedWeaponIcon());

					

					player.loadConfigFile();
					difficulty = stringToDifficulty(player.getdifficultyString());
					windowedMode = player.getWindowedMode();
					displayFps = player.getDisplayFps();
					Listener::setGlobalVolume(player.getVolume());
					world.newWorld();
				}

				// Player hit the load game button in the main menu
				else if (loadGameButton.getGlobalBounds().contains(worldPos) && state == State::MAIN_MENU && event.mouseButton.button == Mouse::Left)
				{
					state = State::PLAYING;

					// Play the start game sound
					if (!startSoundPlayed) {
						sound.playStartGameSound();
					}

					startSoundPlayed = true;

					// Loads player stats from text file
					if (player.loadSaveFile() == true) {
						// Player loaded successfully
						world.loadWorld();

						// We will modify the next two lines later
						arena.width = 1900;
						arena.height = 800;
						arena.left = 1664;
						arena.top = 1664;

						// Pass the vertex array by reference 
						// to the createBackground function
						int tileSize = 64;

						// Spawn the player in the middle of the arena
						player.spawn(arena, resolution, tileSize, player.getPlayerLevel());
						enemy.spawn(arena, resolution, tileSize, "Goblin", player.getPlayerLevel());

						// Configure the pick-ups
						healthPickup.setArena(arena);
						ammoPickup.setArena(arena);
						staminaPickup.setArena(arena);
						manaPickup.setArena(arena);

						// Reset the clock so there isn't a frame jump
						clock.restart();

						equippedWeaponIcon.setTextureRect(player.getEquippedWeaponIcon());


						player.loadConfigFile();
						difficulty = stringToDifficulty(player.getdifficultyString());
						windowedMode = player.getWindowedMode();
						displayFps = player.getDisplayFps();
						Listener::setGlobalVolume(player.getVolume());
					}
					else {
						// No save file so create a new one with default values and load it	
						player.createNewSave();
						player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume());
						player.loadSaveFile();



						// We will modify the next two lines later
						arena.width = 1900;
						arena.height = 800;
						arena.left = 1664;
						arena.top = 1664;

						// Pass the vertex array by reference 
						// to the createBackground function
						int tileSize = 64;

						// Spawn the player in the middle of the arena
						player.spawn(arena, resolution, tileSize, player.getPlayerLevel());
						enemy.spawn(arena, resolution, tileSize, "Goblin", player.getPlayerLevel());

						// Configure the pick-ups
						healthPickup.setArena(arena);
						ammoPickup.setArena(arena);
						staminaPickup.setArena(arena);
						manaPickup.setArena(arena);

						// Reset the clock so there isn't a frame jump
						clock.restart();





						equippedWeaponIcon.setTextureRect(player.getEquippedWeaponIcon());

						player.loadConfigFile();
						difficulty = stringToDifficulty(player.getdifficultyString());
						windowedMode = player.getWindowedMode();
						displayFps = player.getDisplayFps();
						Listener::setGlobalVolume(player.getVolume());
					}
				}

				// Player hit the options button
				if (optionsButton.getGlobalBounds().contains(worldPos) && state == State::MAIN_MENU && event.mouseButton.button == Mouse::Left )
				{
					sound.playButtonClickSound();
					world.clearWorld();
					state = State::OPTIONS_MENU;
				}

				// Player hit the quit game button
				if (quitGameButton.getGlobalBounds().contains(worldPos) && state == State::MAIN_MENU && event.mouseButton.button == Mouse::Left)
				{
					sound.playButtonClickSound();
					// Save info to file before quitting
					window.close();
				}

				// Player hit the main menu button in the options menu
				if (mainMenuButton.getGlobalBounds().contains(worldPos) && state == State::OPTIONS_MENU && event.mouseButton.button == Mouse::Left)
				{
					sound.playButtonClickSound();
					world.clearWorld();
					player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume());
					state = State::MAIN_MENU;
				}

				// Player hit the main menu button in the pause menu
				if (mainMenuButton.getGlobalBounds().contains(worldPos) && state == State::PAUSED && event.mouseButton.button == Mouse::Left)
				{
					sound.playButtonClickSound();
					world.clearWorld();
					player.updateSaveFile(player.getSpeed(), player.getHealth(), player.getMaxHealth(), player.getStamina(), player.getMaxStamina(), player.getStaminaRecharge(), player.getMana(), player.getMaxMana(), player.getGold(), player.getKillCount(), player.getPlayerLevel(), player.getEquippedWeapon(), player.getSavedSword(), player.getSavedWand(), player.getPosition());
					state = State::MAIN_MENU;
				}

				// Player hit the display fps button
				if (displayFPSButton.getGlobalBounds().contains(worldPos) && state == State::OPTIONS_MENU && event.mouseButton.button == Mouse::Left)
				{
					if (displayFps) {
						sound.playButtonClickSound();
						displayFps = false;
						player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume());
					}
					else {
						sound.playButtonClickSound();
						displayFps = true;
						player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume());
					}
				}

				// Player hit the windowed mode button

				// Player hit the display fps button
				if (windowedModeButton.getGlobalBounds().contains(worldPos) && state == State::OPTIONS_MENU && event.mouseButton.button == Mouse::Left)
				{
					if (windowedMode) {
						sound.playButtonClickSound();
						windowedMode = false;
						player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume());
						window.create(VideoMode(resolution.x, resolution.y), "TimeCaster", Style::Fullscreen);
					}
					else {
						sound.playButtonClickSound();
						windowedMode = true;
						player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume());
						window.create(VideoMode(resolution.x, resolution.y), "TimeCaster", Style::Default);
					}
				}

				// Player hit the difficulty button
				if (difficultyButton.getGlobalBounds().contains(worldPos) && state == State::OPTIONS_MENU && event.mouseButton.button == Mouse::Left)
				{
					switch (difficulty) {
					case Difficulty::Easy:
						difficulty = Difficulty::Medium;
						player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume());
						break;

					case Difficulty::Medium:
						difficulty = Difficulty::Hard;
						player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume());
						break;

					case Difficulty::Hard:
						difficulty = Difficulty::Easy;
						player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume());
						break;
					}

					difficultyButtonText.setString("Difficulty: " + difficultyToString(difficulty));
					textBounds = difficultyButtonText.getLocalBounds();
					x = difficultyButton.getPosition().x + (difficultyButton.getSize().x / 2.f) - (textBounds.width / 2.f);
					y = difficultyButton.getPosition().y + (difficultyButton.getSize().y / 2.f) - (textBounds.height / 2.f);
					difficultyButtonText.setPosition(x - textBounds.left, y - textBounds.top);
					sound.playButtonClickSound();
				}
				
				if (state == State::STORY_INTRO && event.key.code == Keyboard::Space)
				{
					sound.stopStoryIntroSound();
					state = State::PLAYING;
				}

				if (state == State::PLAYING && event.key.code == Keyboard::Tab) {
					if (drawInventory) {
						drawInventory = false;
					}
					else {
						drawInventory = true;
					}
				}
			}
		} // End event polling

		// Handle controls while playing
		if (state == State::PLAYING && !drawInventory)
		{
			// Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();					
			}
			else
			{
				player.stopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{				
				player.moveDown();				
			}
			else
			{
				player.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();				
			}
			else
			{
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{		
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}


		} // End WASD while playing
		else if (drawInventory)
		{
			player.stopRight();
			player.stopLeft();
			player.stopUp();
			player.stopDown();	
		}



		

		/* below are debug functions, comment them out in full build / when needed
		if you add any more, make sure they check if debug reset is false and set it to true or else it will run every loop while the key is pressed */
		if (event.key.code == Keyboard::Num0 && state == State::PLAYING)
		{
			debugreset = false;

			
		}
		if (event.key.code == Keyboard::Num1 && !debugreset && state == State::PLAYING)
		{

			
			// Increase health
			player.upgradeHealth();

			debugreset = true;
		}

		if (event.key.code == Keyboard::Num2 &&  !debugreset && state == State::PLAYING)
		{
			// Increase stamina
			player.upgradeStamina();
			debugreset = true;
		}

		if (event.key.code == Keyboard::Num3 && !debugreset && state == State::PLAYING)
		{
			// Increase health
			player.upgradeMana();
			debugreset = true;
		}

		if (event.key.code == Keyboard::Num8 && !debugreset && state == State::PLAYING)
		{
			player.hit(gameTimeTotal, 10, 200);
			debugreset = true;
		}

		if (event.key.code == Keyboard::Num9 && !debugreset && state == State::PLAYING)
		{
			player.hit(gameTimeTotal, 30, 1000);
			debugreset = true;
		}

		if (event.key.code == Keyboard::G && !debugreset && state == State::PLAYING)
		{
			for (int i = 0; i < (rand() % 10); i++) {
				items.emplace_back("gold", Vector2f(0, 300));
			}
			debugreset = true;
		}

		// Handle the display fps button changing colour based on boolean
		if (state == State::OPTIONS_MENU) 
		{
			// Change colour of displayFPSButton based on displayFps button
			if (displayFps) {
				displayFPSButton.setFillColor(Color::Green);
			}
			else {
				displayFPSButton.setFillColor(Color::Red);
			}

			if (windowedMode) {
				windowedModeButton.setFillColor(Color::Green);
			}
			else {
				windowedModeButton.setFillColor(Color::Red);
			}

			// Change colour of difficultyButton based on selected difficulty
			if (difficulty == Difficulty::Easy)
			{
				difficultyButton.setFillColor(Color::Green);
			}
			else if (difficulty == Difficulty::Medium)
			{
				difficultyButton.setFillColor(Color::Yellow);
			}
			else
			{
				difficultyButton.setFillColor(Color::Red);
			}
		}

		/***************
		UPDATE THE FRAME
		****************/
		if (state == State::PLAYING)
		{
			// Update the delta time
			Time dt = clock.restart();

			// Update the total game time
			gameTimeTotal += dt;

			// Make a decimal fraction of 1 from the delta time
			float dtAsSeconds = dt.asSeconds();
			
			// Where is the mouse pointer
			mouseScreenPosition = Mouse::getPosition();

			// Convert mouse position to world coordinates of mainView
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			// Set the crosshair to the mouse world location
			spriteCursor.setPosition(mouseWorldPosition);

			for (int i = 0; i < world.getWorldSize(); i++)
			{
				if (collision.pointInShape(player.getPosition(), world.getChunkArea(i).getShape())) // find players current chunk
				{
					player.setChunk(i);
				}

				if (collision.pointInShape(enemy.getPosition(), world.getChunkArea(i).getShape())) // find enemy's current chunk
				{
					enemy.setChunk(i);
				}
			}
			
			// Update the player
			player.update(dtAsSeconds, Mouse::getPosition(), world.getNavBoxes(player.getChunk()));
			enemy.update(dtAsSeconds, world.getNavBoxes(enemy.getChunk()));

			filter.setOrigin(player.getPosition());
			filter.setPosition(player.getPosition().x, player.getPosition().y);

			// Make a note of the players new position
			Vector2f playerPosition(player.getCenter());

			// Make the view centre around the player				
			mainView.setCenter(player.getCenter().x,player.getCenter().y-10);
			
			/*
			// Update any bullets that are in-flight
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					bullets[i].update(dtAsSeconds);
				}
			}
			*/

			// Update the pickups
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);
			staminaPickup.update(dtAsSeconds);
			manaPickup.update(dtAsSeconds);

			// Has the player touched health pickup
			if (player.getGlobalBounds().intersects(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealthLevel(healthPickup.gotIt());
			}

			// Has the player touched ammo pickup
			if (player.getGlobalBounds().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				//bulletsSpare += ammoPickup.gotIt();
			}

			// Has the player touched stamina pickup
			if (player.getGlobalBounds().intersects(staminaPickup.getPosition()) && staminaPickup.isSpawned())
			{
				player.increaseStaminaLevel(staminaPickup.gotIt()); 
				// Play a sound
			}

			// Has the player touched mana pickup
			if (player.getGlobalBounds().intersects(manaPickup.getPosition()) && manaPickup.isSpawned())
			{
				player.increaseManaLevel(manaPickup.gotIt());
				// Play a sound
			}

			// Has the player touched mana pickup
			if (player.getGlobalBounds().intersects(manaPickup.getPosition()) && manaPickup.isSpawned())
			{
				player.increaseManaLevel(manaPickup.gotIt());
				// Play a sound
			}
		
			if (currentDecal > 248)
			{
				currentDecal = 0;
			}

			// size up the health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 35));
			emptyHealthBar.setSize(Vector2f(player.getMaxHealth() * 3, 35));

			// Set size of the mana bar
			manaBar.setSize(Vector2f(player.getMana() * 3, 35));
			emptyManaBar.setSize(Vector2f(player.getMaxMana() * 3, 35));

			// Set size of the Stamina bar
			staminaBar.setSize(Vector2f(player.getStamina() * 3, 35));
			emptyStaminaBar.setSize(Vector2f(player.getMaxStamina() * 3, 35));

			// Increment the amount of time since the last HUD update
			timeSinceLastUpdate += dt;
			// Increment the number of frames since the last HUD calculation
			framesSinceLastHUDUpdate++;
			// Calculate FPS every fpsMeasurementFrameInterval frames
			
			// Update the gold text
			stringstream ssGoldCount;
			ssGoldCount << "Gold:" << player.getGold();
			goldCountText.setString(ssGoldCount.str());

			// Update the kills text
			if (drawInventory) {
				stringstream ssKillCount;
				ssKillCount << "Kills: " << player.getKillCount();
				killsText.setString(ssKillCount.str());

				stringstream ssHealthBar;
				ssHealthBar << player.getHealth() << " / " << player.getMaxHealth();
				invHealthBarText.setString(ssHealthBar.str());
				textBounds = invHealthBarText.getLocalBounds();
				x = backgroundInvHealthBar.getPosition().x + (backgroundInvHealthBar.getSize().x / 2.f) - (textBounds.width / 2.f);
				y = backgroundInvHealthBar.getPosition().y + (backgroundInvHealthBar.getSize().y / 2.f) - (textBounds.height / 2.f);
				invHealthBarText.setPosition(x - textBounds.left, y - textBounds.top);

				stringstream ssStamBar;
				ssStamBar << player.getStamina() << " / " << player.getMaxStamina();
				invStamBarText.setString(ssStamBar.str());
				textBounds = invStamBarText.getLocalBounds();
				x = backgroundInvStamBar.getPosition().x + (backgroundInvStamBar.getSize().x / 2.f) - (textBounds.width / 2.f);
				y = backgroundInvStamBar.getPosition().y + (backgroundInvStamBar.getSize().y / 2.f) - (textBounds.height / 2.f);
				invStamBarText.setPosition(x - textBounds.left, y - textBounds.top);

				stringstream ssManaBar;
				ssManaBar << player.getMana() << " / " << player.getMaxMana();
				invManaBarText.setString(ssManaBar.str());
				textBounds = invManaBarText.getLocalBounds();
				x = backgroundInvManaBar.getPosition().x + (backgroundInvManaBar.getSize().x / 2.f) - (textBounds.width / 2.f);
				y = backgroundInvManaBar.getPosition().y + (backgroundInvManaBar.getSize().y / 2.f) - (textBounds.height / 2.f);
				invManaBarText.setPosition(x - textBounds.left, y - textBounds.top);

				invHealthBar.setSize(Vector2f(200 * (player.getHealth() / player.getMaxHealth()), 50));
				invStamBar.setSize(Vector2f(200 * (player.getStamina() / player.getMaxStamina()), 50));
				invManaBar.setSize(Vector2f(200 * (player.getMana() / player.getMaxMana()), 50));
			}

			framesSinceLastHUDUpdate = 0;
			timeSinceLastUpdate = Time::Zero;
			// End HUD update

			// update items
			for (size_t i = 0; i < items.size(); )
			{
				items[i].update(dtAsSeconds);

				if (player.getGlobalBounds().intersects(items[i].getPosition()))
				{
					// give gold and remove gold from vector
					player.setGold(items[i].getValue());
					items.erase(items.begin() + i);
				}
				else
				{
					++i;
				}
			}

			if (!sound.isSoundtrackPlaying()) {
				sound.playSoundtrack();
			}

		} // End updating the scene

		if (state == State::MAIN_MENU)
		{
			startSoundPlayed = FALSE;
		}
		
		if (state == State::MAIN_MENU || state == State::OPTIONS_MENU)
		{
			if (sound.isSoundtrackPlaying()) {
				sound.stopSoundtrack();
			}
		}

		if (state == State::PLAYING)
		{
			window.setMouseCursorVisible(false);
			window.setMouseCursorGrabbed(true);
		}
		else if (state == State::PAUSED || state == State::MAIN_MENU || state == State::OPTIONS_MENU)
		{
			window.setMouseCursorVisible(true);
			window.setMouseCursorGrabbed(false);
		}

		if (state == State::OPTIONS_MENU) {
			if (dragging)
			{
				Vector2i mousePos = Mouse::getPosition(window);
				float x = static_cast<float>(mousePos.x);

				// Clamp within track
				if (x < track.getPosition().x) x = track.getPosition().x;
				if (x > track.getPosition().x + track.getSize().x) x = track.getPosition().x + track.getSize().x;

				handle.setPosition(x, handle.getPosition().y);

				// Map handle position to global volume
				float globalVolume = ((x - track.getPosition().x) / track.getSize().x) * 100.f;

				// Apply to everything
				Listener::setGlobalVolume(globalVolume);
				isDragging = true;
				
			}
			if (!dragging && isDragging) {
				// Save volume to config file
				player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume());
				isDragging = false;
			}
		}

		/*************
		Draw the scene
		**************/
		if (state == State::PLAYING)
		{
			window.clear();

			// set the mainView to be displayed in the window
			// And draw everything related to it
			window.setView(mainView);

			// Draw the background
			for (int i = 0; i < world.getWorldSize(); i++)
			{
				if (collision.distance(player.getCenter(), world.getChunkCenter(i)) < 2000) { // check player distance to chunk
					window.draw(world.getBackground(i), &textureBackground);
				}
			}

			// DRAW EFFECTS
			for (int i = 0; i < 249; i++) // draw decals
			{
				window.draw(decal[i].getSprite());
			}

			/*
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					window.draw(bullets[i].getShape());
				}
			}
			*/

			for (auto& item : items) {
				window.draw(item.getSprite());
			}

			for (int i = 0; i < world.getWorldSize(); i++)
			{
				if (collision.distance(player.getCenter(),world.getChunkCenter(i)) < 2000) {
					for (auto& entity : world.getEntities(i)) {
						if (player.getRenderArea().intersects(entity.getSprite().getGlobalBounds())) {
							drawables.emplace_back(entity.getSprite().getGlobalBounds().top + entity.getSprite().getGlobalBounds().height, entity.getSprite());
						}
					}
				}
			}
			drawables.emplace_back(player.getSprite().getGlobalBounds().top + player.getSprite().getGlobalBounds().height, player.getSpriteFromSheet()); // place player armour into drawables
			drawables.emplace_back(player.getSprite().getGlobalBounds().top + player.getSprite().getGlobalBounds().height + 0.01, player.getHead());
			drawables.emplace_back(player.getSprite().getGlobalBounds().top + player.getSprite().getGlobalBounds().height + 0.02, player.getTorso());
			drawables.emplace_back(player.getSprite().getGlobalBounds().top + player.getSprite().getGlobalBounds().height + 0.03, player.getPants());
			drawables.emplace_back(player.getSprite().getGlobalBounds().top + player.getSprite().getGlobalBounds().height + 0.04, player.getShoes());
			

			// Sort by y value using lambda function (ascending = top to bottom)
			sort(drawables.begin(), drawables.end(),[](const DrawableItem& a, const DrawableItem& b) 
				{
					return a.y < b.y;
				}
			);

			// Draw in sorted order
			for (auto& item : drawables) {
				window.draw(item.sprite);
			}

			drawables.clear();

			// Draw enemy
			window.draw(enemy.getSprite());

			// Draw the pickups that are currently spawned
			if (ammoPickup.isSpawned())
			{
				window.draw(ammoPickup.getSprite());
			}
			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}
			if (staminaPickup.isSpawned())
			{
				window.draw(staminaPickup.getSprite());
			}
			if (manaPickup.isSpawned())
			{
				window.draw(manaPickup.getSprite());
			}

			for (int i = 0; i < world.getWorldSize(); i++)
			{
				if (collision.distance(player.getCenter(), world.getChunkCenter(i)) < 2000) {
					window.draw(world.getForground(i), &textureBackground);
				}
			}

			/*
			for (auto& txt : chunks[0].getDebugText()) { // draw debug text showing tile location
				window.draw(txt);
			}

			for (auto& nav : world.getNavBoxes(0)) { // draw debug text showing tile location
				window.draw(nav.getShape());
			}
			*/

			/*
			for (auto& nav : world.getNavBoxes(player.getChunk())) { // draw debug text showing tile location
				window.draw(nav.getShape());
				window.draw(world.getChunkArea(player.getChunk()).getShape());
			}
			*/

			if (Mouse::isButtonPressed(Mouse::Left) && state == State::PLAYING) {
				spriteCursor.setTexture(textureCursorClosed);
			}
			else {
				spriteCursor.setTexture(textureCursorOpen);
			}
			
			//Draw the crosshair
			window.draw(spriteCursor);

			// Switch to the HUD view
			window.setView(hudView);
			
			if (drawInventory) {
				window.draw(filter);
				window.draw(playerFrame);
				window.draw(playerInFrame);
				window.draw(headArmourFrame);
				window.draw(chestArmourFrame);
				window.draw(trousersArmourFrame);
				window.draw(bootsArmourFrame);
				window.draw(neckFrame);
				window.draw(weaponFrame);
				window.draw(equippedWeaponIcon);
				window.draw(ringFrame);
				for (int i = 0; i < sizeof(emptyFrames) / sizeof(emptyFrames[0]); i++) {
					window.draw(emptyFrames[i]);
				}
				window.draw(killsText);
				window.draw(goldCountText);
				window.draw(backgroundInvHealthBar);
				window.draw(invHealthBar);
				window.draw(invHealthBarText);
				window.draw(backgroundInvStamBar);
				window.draw(invStamBar);
				window.draw(invStamBarText);
				window.draw(backgroundInvManaBar);
				window.draw(invManaBar);
				window.draw(invManaBarText);
			}
			else {
				// Draw all the HUD elements
				window.draw(emptyHealthBar);
				window.draw(healthBar);
				window.draw(emptyManaBar);
				window.draw(manaBar);
				window.draw(emptyStaminaBar);
				window.draw(staminaBar);
				if (displayFps) {
					window.draw(fpsText);
				}
				window.draw(filter);
			}
		}

		if (state == State::MAIN_MENU)
		{
			window.clear();
			window.draw(spriteMainMenu);
			window.draw(mainHeadingText);
			window.draw(newGameButton);
			window.draw(newGameButtonText);
			window.draw(loadGameButton);
			window.draw(loadGameButtonText);
			window.draw(optionsButton);
			window.draw(optionsButtonText);
			window.draw(quitGameButton);
			window.draw(quitGameButtonText);
		}

		if (state == State::OPTIONS_MENU)
		{
			window.clear();
			window.draw(spriteMainMenu);
			window.draw(optionsHeadingText);
			window.draw(mainMenuButton);
			window.draw(mainMenuButtonText);
			window.draw(volumeSliderText);
			window.draw(track);
			window.draw(handle);
			window.draw(displayFPSButton);
			window.draw(displayFPSButtonText);
			window.draw(difficultyButton);
			window.draw(difficultyButtonText);
			window.draw(windowedModeButton);
			window.draw(windowedModeButtonText);
		}

		if (state == State::STORY_INTRO) 
		{
			window.clear();
			window.draw(storyIntroText);
			window.draw(skipIntroText);
		}

		if (state == State::PAUSED)
		{
			window.draw(pausedText);
			window.draw(mainMenuButton);
			window.draw(mainMenuButtonText);
		}

		window.display();

	}// End game loop

	return 0;
}