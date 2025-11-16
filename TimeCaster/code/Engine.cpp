#include <sstream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Engine.h"
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
#include "Weapon.h"
#include "Equipment.h"
#include "Enemy.h"
#include <string>
#include <thread>

using namespace std;
using namespace sf;

Engine::Engine() : m_EquippedWeapons(player.getEquippedWeapons()), m_EquippedArmour(player.getEquippedArmour()), m_StoredItems(player.getStoredItems())
{
	player.loadConfigFile();

	// Set a high framerate limit (TODO: allow player to configure FPS limit)
	window.setFramerateLimit(144);
	// difficulty = Difficulty::Medium;
	difficulty = stringToDifficulty(player.getdifficultyString());
	windowedMode = player.getWindowedMode();
	vSync = player.getVSync();
	displayFps = player.getDisplayFps();
	Listener::setGlobalVolume(player.getVolume());

	// Get the screen resolution and create an SFML window
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	resolution.x = 1920;
	resolution.y = 1080;

	//RenderWindow window(VideoMode(resolution.x, resolution.y), "TimeCaster", Style::Fullscreen);
	
	if (windowedMode == true)
	{
		window.create(VideoMode(resolution.x, resolution.y), "TimeCaster", Style::Default);
	}
	else {
		window.create(VideoMode(resolution.x, resolution.y), "TimeCaster", Style::Fullscreen);
	}

	if (vSync == true)
	{
		window.setVerticalSyncEnabled(true);
	}
	else {
		window.setVerticalSyncEnabled(false);
	}

	filter.setSize(resolution);
	
	if (!timeFrozen) {
		filter.setFillColor(defaultFilter);
	}

	player.loadConfigFile();

	difficulty = stringToDifficulty(player.getdifficultyString());

	FloatRect viewRect(0, 0, resolution.x, resolution.y);
	mainView.reset(viewRect);
	// Optional: set the center to the middle of the view
	mainView.setCenter(resolution.x / 2.f, resolution.y / 2.f);

	// Zoom view
	mainView.zoom(0.3f);

	// Create the background
	// VertexArray background;
	// Load the texture for our background vertex array
	textureBackground = TextureHolder::GetTexture("graphics/landscape.png");
	textureBackground2 = TextureHolder::GetTexture("graphics/landscape2.png");
	textureBackground3 = TextureHolder::GetTexture("graphics/landscape3.png");

	// Boolean for whether to display the fps
//	bool displayFps = false;

	// Hide the mouse pointer and replace it with crosshair
	window.setMouseCursorVisible(true);
	textureCursorOpen = TextureHolder::GetTexture("graphics/knightCursorOpen.png");
	textureCursorClosed = TextureHolder::GetTexture("graphics/knightCursorClosed.png");
	spriteCursor.setTexture(textureCursorOpen);
	spriteCursor.setScale(0.4, 0.4);
	spriteCursor.setOrigin(25, 25);

	textureMainMenu = TextureHolder::GetTexture("graphics/UI/Castle (edited).jpg");
	spriteMainMenu.setTexture(textureMainMenu);
	spriteMainMenu.setPosition(0, 0);

	textureStoryIntro = TextureHolder::GetTexture("graphics/UI/ForestFire.jpg");
	spriteStoryIntro.setTexture(textureStoryIntro);
	spriteStoryIntro.setPosition(0, 0);

	// Create a view for the HUD
	FloatRect hudRect(0, 0, resolution.x, resolution.y);
	hudView.reset(hudRect);

	/*
	Pickup healthPickup(1);
	Pickup ammoPickup(2);
	Pickup staminaPickup(3);
	Pickup manaPickup(4);
	*/

	// Main font
	font.loadFromFile("fonts/Jersey10-Regular.ttf");

	// Paused text
	pausedText.setFont(font);
	pausedText.setCharacterSize(160);
	pausedText.setString("Press escape \nto continue");
	pausedText.setFillColor(Color::White);
	textBounds = pausedText.getLocalBounds();
	viewCentre = mainView.getCenter();
	pausedText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 400);

	// FPS text
	fpsText.setFont(font);
	fpsText.setCharacterSize(fontSize - 5);
	fpsText.setFillColor(Color::Yellow);
	fpsText.setPosition(1800, 5);

	barContainer = TextureHolder::GetTexture("graphics/UI/barContainer.png");

	// Health bar
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(25,10);

	emptyHealthBar.setFillColor(Color::Black);
	emptyHealthBar.setPosition(25, 10);

	healthBarContainer.setTexture(&barContainer);
	healthBarContainer.setPosition(10, 10);

	// Stamina bar
	staminaBar.setFillColor(Color::Green);
	staminaBar.setPosition(25, 60);

	emptyStaminaBar.setFillColor(Color::Black);
	emptyStaminaBar.setPosition(25, 60);

	// Empty Stamina bar
	staminaBarContainer.setTexture(&barContainer);
	staminaBarContainer.setPosition(10, 60);

	// Mana bar
	manaBar.setFillColor(Color::Magenta);
	manaBar.setPosition(25, 110);

	emptyManaBar.setFillColor(Color::Black);
	emptyManaBar.setPosition(25, 110);

	// Empty mana bar
	manaBarContainer.setTexture(&barContainer);
	manaBarContainer.setPosition(10, 110);

	m_StoredItems.resize(16, Item("null", Vector2f(300, 650)));
	shopItems.resize(8, Item("null", Vector2f(0, 0)));
	// Debug shop item initialization
	shopItems[0] = Weapon("Silver_Wand", Vector2f(300, 650));
	shopItems[1] = Weapon("Pirate's_Scimitar", Vector2f(450, 650));
	shopItems[2] = Equipment("Leather_Cap", Vector2f(600, 650));
	shopItems[3] = Equipment("Leather_Chestplate", Vector2f(750, 950));
	shopItems[4] = Equipment("Leather_Leggings", Vector2f(900, 650));
	shopItems[5] = Equipment("Leather_Boots", Vector2f(1050, 650));
	shopItems[6] = Item("Health_Potion", Vector2f(1200, 650));
	shopItems[7] = Item("Mana_Potion", Vector2f(1350, 650));


	//Item 0 is sword (melee combat), item 1 is wand (magic combat)
	m_EquippedWeapons.resize(2, Weapon("null", Vector2f(0, 0)));
	
	// Item 0 is head, item 1 is torso, item 2 is pants, item 3 is shoes, item 4 is neck
	m_EquippedArmour.resize(5, Equipment("null", Vector2f(0, 0)));
	
	// Debug inventory initalization
	/*
	m_StoredItems[0] = Weapon("Wooden_Wand", Vector2f(300, 650));
	m_StoredItems[1] = Weapon("Pirate's_Scimitar", Vector2f(450, 650));
	m_StoredItems[2] = Weapon("Iron_Sword", Vector2f(600, 650));
	m_StoredItems[3] = Weapon("Silver_Wand", Vector2f(750, 650));
	m_StoredItems[4] = Equipment("Family_Robe", Vector2f(900, 650));
	m_StoredItems[5] = Equipment("Family_Hood", Vector2f(0, 0));
	m_StoredItems[6] = Equipment("Leather_Cap", Vector2f(0, 0));
	m_StoredItems[7] = Equipment("Leather_Chestplate", Vector2f(0, 0));
	m_StoredItems[8] = Equipment("Basic_Shoes", Vector2f(0, 0));
	m_StoredItems[9] = Equipment("Robe_Leggings", Vector2f(0, 0));
	m_StoredItems[10] = Weapon("Reaper's_Scythe", Vector2f(0, 0));
	m_StoredItems[11] = Equipment("Family_Locket", Vector2f(0, 0));
	m_StoredItems[12] = Weapon("Armoured_Boots", Vector2f(0, 0));
	m_StoredItems[14] = Item("Mana_Potion", Vector2f(0, 0));
	m_StoredItems[15] = Item("Stamina_Potion", Vector2f(0, 0));
	*/
	/***********
	Main Menu UI
	************/
	textureMainMenuButton1 = TextureHolder::GetTexture("graphics/UI/menuButton1.png");
	textureMainMenuButton2 = TextureHolder::GetTexture("graphics/UI/menuButton2.png");

	// TimeCaster heading text
	mainHeadingText.setFont(font);
	mainHeadingText.setCharacterSize(fontSize + 65);
	mainHeadingText.setString("TimeCaster");
	mainHeadingText.setFillColor(Color::White);
	textBounds = mainHeadingText.getLocalBounds();
	viewCentre = mainView.getCenter();
	mainHeadingText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 10);

	// New game button
	newGameButton.setPosition(200, 210);
	newGameButton.setSize(Vector2f(300, 80));
	newGameButton.setTexture(&textureMainMenuButton1);

	// New game button text
	newGameButtonText.setFont(font);
	newGameButtonText.setCharacterSize(fontSize);
	newGameButtonText.setString("New Game");
	newGameButtonText.setFillColor(Color::Yellow);
	textBounds = newGameButtonText.getLocalBounds();
	float x = newGameButton.getPosition().x + (newGameButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	float y = newGameButton.getPosition().y + (newGameButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	newGameButtonText.setPosition(x - textBounds.left, y - textBounds.top - 8);

	// Load game button
	loadGameButton.setPosition(200, 320);
	loadGameButton.setSize(Vector2f(300, 80));
	loadGameButton.setTexture(&textureMainMenuButton2);

	// Load game button text
	loadGameButtonText.setString("Load Game");  // Set the text
	loadGameButtonText.setFont(font);           // Set the font
	loadGameButtonText.setCharacterSize(fontSize); // Set the font size
	loadGameButtonText.setFillColor(Color::White);
	textBounds = loadGameButtonText.getLocalBounds();
	x = loadGameButton.getPosition().x + (loadGameButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	y = loadGameButton.getPosition().y + (loadGameButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	loadGameButtonText.setPosition(x - textBounds.left, y - textBounds.top);

	// Options button
	optionsButton.setPosition(200, 430);
	optionsButton.setSize(Vector2f(300, 80));
	optionsButton.setTexture(&textureMainMenuButton2);

	// options button text
	optionsButtonText.setString("Options"); // Set the text content
	optionsButtonText.setFont(font);        // Apply the font
	optionsButtonText.setCharacterSize(fontSize); // Set the size of the text
	optionsButtonText.setFillColor(Color::White);
	textBounds = optionsButtonText.getLocalBounds();
	x = optionsButton.getPosition().x + (optionsButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	y = optionsButton.getPosition().y + (optionsButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	optionsButtonText.setPosition(x - textBounds.left, y - textBounds.top);

	// Quit game button
	quitGameButton.setPosition(200, 540);
	quitGameButton.setSize(Vector2f(300, 80));
	quitGameButton.setTexture(&textureMainMenuButton2);

	// Quit game button text
	quitGameButtonText.setString("Quit Game");  // Set the displayed text
	quitGameButtonText.setFont(font);           // Assign the font
	quitGameButtonText.setCharacterSize(fontSize); // Set the text size
	quitGameButtonText.setFillColor(Color::White);
	textBounds = quitGameButtonText.getLocalBounds();
	x = quitGameButton.getPosition().x + (quitGameButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	y = quitGameButton.getPosition().y + (quitGameButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	quitGameButtonText.setPosition(x - textBounds.left, y - textBounds.top);

	/**************
	Options Menu UI
	***************/

	// Options heading text
	optionsHeadingText.setString("Options");      // Set the text content
	optionsHeadingText.setFont(font);             // Assign the font
	optionsHeadingText.setCharacterSize(fontSize + 15); // Make it larger than regular buttons
	optionsHeadingText.setFillColor(Color::White);
	textBounds = optionsHeadingText.getLocalBounds();
	viewCentre = mainView.getCenter();
	optionsHeadingText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 10);

	// Main menu button
	mainMenuButton.setSize(Vector2f(300, 80));
	textBounds = mainMenuButton.getLocalBounds();
	viewCentre = mainView.getCenter();
	mainMenuButton.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 975);
	mainMenuButton.setTexture(&textureMainMenuButton2);

	// Main menu button text
	mainMenuButtonText.setString("Save & Exit");    // Set the displayed text
	mainMenuButtonText.setFont(font);               // Assign the font
	mainMenuButtonText.setCharacterSize(fontSize);
	mainMenuButtonText.setFillColor(Color::White);
	textBounds = mainMenuButtonText.getLocalBounds();
	x = mainMenuButton.getPosition().x + (mainMenuButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	y = mainMenuButton.getPosition().y + (mainMenuButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	mainMenuButtonText.setPosition(x - textBounds.left, y - textBounds.top);

	volumeSliderText.setString("Volume");   // Set the text label
	volumeSliderText.setFont(font);         // Assign the font
	volumeSliderText.setCharacterSize(fontSize); // Apply the text size
	volumeSliderText.setFillColor(Color::Black);
	textBounds = volumeSliderText.getLocalBounds();
	viewCentre = mainView.getCenter();
	volumeSliderText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 150);

	// Slider track
	track.setSize(Vector2f(400, 5));
	track.setFillColor(sf::Color::Black);
	textBounds = track.getLocalBounds();
	viewCentre = mainView.getCenter();
	int trackY = 250;
	track.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, trackY);

	// Slider handle
	handle.setRadius(10);
	handle.setPointCount(10);
	handle.setFillColor(Color::Red);
	handle.setOrigin(10, 10); // Centre the circle
	textBounds = handle.getLocalBounds();
	x = track.getPosition().x + (track.getSize().x / 2.f) - (textBounds.width / 2.f);
	handle.setPosition(x - textBounds.left, trackY + 2); // Slider start at 50% volume

	// Display FPS button
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
	displayFPSButtonText.setString("Display FPS"); // Set the label text
	displayFPSButtonText.setFont(font); // Assign the font
	displayFPSButtonText.setCharacterSize(fontSize - 5);
	displayFPSButtonText.setFillColor(Color::Black);
	textBounds = displayFPSButtonText.getLocalBounds();
	x = displayFPSButton.getPosition().x + (displayFPSButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	y = displayFPSButton.getPosition().y + (displayFPSButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	displayFPSButtonText.setPosition(x - textBounds.left, y - textBounds.top);

	// Windowed mode button
	if (windowedMode)
	{
		windowedModeButton.setFillColor(Color::Green);
	}
	else
	{
		windowedModeButton.setFillColor(Color::Red);
	}
	windowedModeButton.setSize(Vector2f(350, 80));
	textBounds = windowedModeButton.getLocalBounds();
	viewCentre = mainView.getCenter();
	windowedModeButton.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 500);
	windowedModeButton.setTexture(&textureMainMenuButton2);

	// Windowed mode button text
	windowedModeButtonText.setString("Windowed Mode"); // Set the display text
	windowedModeButtonText.setFont(font); // Assign the font
	windowedModeButtonText.setCharacterSize(fontSize - 5);
	windowedModeButtonText.setFillColor(Color::Black);
	textBounds = windowedModeButtonText.getLocalBounds();
	x = windowedModeButton.getPosition().x + (windowedModeButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	y = windowedModeButton.getPosition().y + (windowedModeButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	windowedModeButtonText.setPosition(x - textBounds.left, y - textBounds.top);

	// VSync button
	if (vSync)
	{
		vSyncButton.setFillColor(Color::Green);
	}
	else
	{
		vSyncButton.setFillColor(Color::Red);
	}
	vSyncButton.setSize(Vector2f(200, 80));
	textBounds = vSyncButton.getLocalBounds();
	viewCentre = mainView.getCenter();
	vSyncButton.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 600);
	vSyncButton.setTexture(&textureMainMenuButton2);

	vSyncButtonText.setString("Vsync"); // Set the display text
	vSyncButtonText.setFont(font); // Assign the font
	vSyncButtonText.setCharacterSize(fontSize - 5);
	vSyncButtonText.setFillColor(Color::Black);
	textBounds = vSyncButtonText.getLocalBounds();
	x = vSyncButton.getPosition().x + (vSyncButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	y = vSyncButton.getPosition().y + (vSyncButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	vSyncButtonText.setPosition(x - textBounds.left, y - textBounds.top);

	// debugMode button
	if (debugMode)
	{
		debugModeButton.setFillColor(Color::Green);
	}
	else
	{
		debugModeButton.setFillColor(Color::Red);
	}
	debugModeButton.setSize(Vector2f(200, 80));
	textBounds = debugModeButton.getLocalBounds();
	viewCentre = mainView.getCenter();
	debugModeButton.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 700);
	debugModeButton.setTexture(&textureMainMenuButton2);

	debugModeButtonText.setString("Debug Mode"); // Set the display text
	debugModeButtonText.setFont(font); // Assign the font
	debugModeButtonText.setCharacterSize(fontSize - 5);
	debugModeButtonText.setFillColor(Color::Black);
	textBounds = debugModeButtonText.getLocalBounds();
	x = debugModeButton.getPosition().x + (debugModeButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	y = debugModeButton.getPosition().y + (debugModeButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	debugModeButtonText.setPosition(x - textBounds.left, y - textBounds.top);

	// Display difficulty button
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
	difficultyButtonText.setString("Difficulty: " + difficultyToString(difficulty)); // Set the label text
	difficultyButtonText.setFont(font); // Assign the font
	difficultyButtonText.setCharacterSize(fontSize - 5);
	difficultyButtonText.setFillColor(Color::Black);
	textBounds = difficultyButtonText.getLocalBounds();
	x = difficultyButton.getPosition().x + (difficultyButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	y = difficultyButton.getPosition().y + (difficultyButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	difficultyButtonText.setPosition(x - textBounds.left, y - textBounds.top);


	difficultyButton.setSize(Vector2f(textBounds.width + 100, 100));
	difficultyButton.setPosition(viewCentre.x - (difficultyButton.getSize().x / 2.f), 400);

	// Story into text
	storyIntroText.setFont(font);
	storyIntroText.setCharacterSize(fontSize + 10);
	storyIntroText.setFillColor(Color::White);
	storyIntroText.setPosition(150, 200);

	fullText = "I was not always a man consumed by vengeance. Once, I had a family-warm laughter by the fire, \n"
		"the gentle touch of my children's hands, the steady love of my wife. \n"
		"All of it was torn from me in a single night, \n"
		"devoured by the fire of (name), a dragon whose name still burns in my mind. \n"
		"Since then, every spell I've mastered, every scar I've earned, has been for one purpose alone: \n"
		"to bring that beast to its knees. \n"
		"I do not seek glory, nor the hollow praise of men-I seek redemption. \n"
		"And when the dragon falls, so too shall the weight of my failure. \n\n"
		"You desire retribution dear player, but what will it cost you?";

	// Skip intro text
	skipIntroText.setFont(font); // Assign the font
	skipIntroText.setCharacterSize(fontSize - 5); // Slightly smaller text size
	skipIntroText.setFillColor(Color::White);

	/***********
	Inventory UI
	************/
	textureHeadArmourFrame = TextureHolder::GetTexture("graphics/UI/headFrame.png");
	textureChestArmourFrame = TextureHolder::GetTexture("graphics/UI/chestFrame.png");
	textureTrousersArmourFrame = TextureHolder::GetTexture("graphics/UI/trousersFrame.png");
	textureBootsArmourFrame = TextureHolder::GetTexture("graphics/UI/bootsFrame.png");
	textureWeaponFrame = TextureHolder::GetTexture("graphics/UI/weaponFrame.png");
	textureEmptyFrame = TextureHolder::GetTexture("graphics/UI/emptyFrame.png");
	texturePlayerFrame = TextureHolder::GetTexture("graphics/UI/playerFrame.png");
	texturePlayerInFrame = TextureHolder::GetTexture("graphics/UI/player.png");
	textureNeckFrame = TextureHolder::GetTexture("graphics/UI/neckFrame.png");
	textureWandFrame = TextureHolder::GetTexture("graphics/UI/ringFrame.png");
	textureItems = TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png");
	tooltipBackground = TextureHolder::GetTexture("graphics/UI/tooltipBackground.png");
	eKeyTexture = TextureHolder::GetTexture("graphics/UI/eKey.png");
	inventoryBackgroundTexture = TextureHolder::GetTexture("graphics/UI/inventoryBackground2.png");
	indicator = TextureHolder::GetTexture("graphics/UI/indicator.png");

	// Equipped item icons
	equippedSwordIcon.setTexture(&textureItems);
	equippedSwordIcon.setTextureRect(IntRect(0, 0, 0, 0));
	equippedSwordIcon.setSize(Vector2f(75, 75));
	equippedSwordIcon.setOrigin(equippedSwordIcon.getSize() / 2.f);
	equippedSwordIcon.setPosition(viewCentre.x - 250, 500);

	equippedWandIcon.setTexture(&textureItems);
	equippedWandIcon.setTextureRect(IntRect(0, 0, 0, 0));
	equippedWandIcon.setSize(Vector2f(75, 75));
	equippedWandIcon.setOrigin(equippedWandIcon.getSize() / 2.f);
	equippedWandIcon.setPosition(viewCentre.x - 150, 500);

	equippedHeadArmourIcon.setTexture(&textureItems);
	equippedHeadArmourIcon.setTextureRect(IntRect(0, 0, 0, 0));
	equippedHeadArmourIcon.setSize(Vector2f(75, 75));
	equippedHeadArmourIcon.setOrigin(equippedHeadArmourIcon.getSize() / 2.f);
	equippedHeadArmourIcon.setPosition(viewCentre.x - 150, 300);

	equippedChestArmourIcon.setTexture(&textureItems);
	equippedChestArmourIcon.setTextureRect(IntRect(0, 0, 0, 0));
	equippedChestArmourIcon.setSize(Vector2f(75, 75));
	equippedChestArmourIcon.setOrigin(equippedChestArmourIcon.getSize() / 2.f);
	equippedChestArmourIcon.setPosition(viewCentre.x - 350, 500);

	equippedTrousersArmourIcon.setTexture(&textureItems);
	equippedTrousersArmourIcon.setTextureRect(IntRect(0, 0, 0, 0));
	equippedTrousersArmourIcon.setSize(Vector2f(75, 75));
	equippedTrousersArmourIcon.setOrigin(equippedTrousersArmourIcon.getSize() / 2.f);
	equippedTrousersArmourIcon.setPosition(viewCentre.x - 350, 400);

	equippedShoeArmourIcon.setTexture(&textureItems);
	equippedShoeArmourIcon.setTextureRect(IntRect(0, 0, 0, 0));
	equippedShoeArmourIcon.setSize(Vector2f(75, 75));
	equippedShoeArmourIcon.setOrigin(equippedShoeArmourIcon.getSize() / 2.f);
	equippedShoeArmourIcon.setPosition(viewCentre.x - 150, 400);

	equippedNeckArmourIcon.setTexture(&textureItems);
	equippedNeckArmourIcon.setTextureRect(IntRect(0, 0, 0, 0));
	equippedNeckArmourIcon.setSize(Vector2f(75, 75));
	equippedNeckArmourIcon.setOrigin(equippedNeckArmourIcon.getSize() / 2.f);
	equippedNeckArmourIcon.setPosition(viewCentre.x - 350, 300);

	// Player frame
	playerFrame.setSize(sf::Vector2f(100.f, 200.f));
	playerFrame.setTexture(&texturePlayerFrame);
	playerFrame.setOrigin(playerFrame.getSize() / 2.f);
	playerFrame.setPosition(viewCentre.x - 250, 350);

	// Player sprite for frame
	playerInFrame.setSize(sf::Vector2f(60.f, 100.f));
	playerInFrame.setTexture(&texturePlayerInFrame);
	playerInFrame.setOrigin(playerInFrame.getSize() / 2.f);
	playerInFrame.setPosition(viewCentre.x - 250, 360);

	neckFrame.setTexture(&textureNeckFrame);
	neckFrame.setSize(Vector2f(75, 75));
	neckFrame.setOrigin(neckFrame.getSize() / 2.f);
	neckFrame.setPosition(viewCentre.x - 350, 300);

	headArmourFrame.setTexture(&textureHeadArmourFrame);
	headArmourFrame.setSize(Vector2f(75, 75));
	headArmourFrame.setOrigin(headArmourFrame.getSize() / 2.f);
	headArmourFrame.setPosition(viewCentre.x - 150, 300);

	trousersArmourFrame.setTexture(&textureTrousersArmourFrame);
	trousersArmourFrame.setSize(Vector2f(75, 75));
	trousersArmourFrame.setOrigin(trousersArmourFrame.getSize() / 2.f);
	trousersArmourFrame.setPosition(viewCentre.x - 350, 400);

	bootsArmourFrame.setTexture(&textureBootsArmourFrame);
	bootsArmourFrame.setSize(Vector2f(75, 75));
	bootsArmourFrame.setOrigin(bootsArmourFrame.getSize() / 2.f);
	bootsArmourFrame.setPosition(viewCentre.x - 150, 400);

	chestArmourFrame.setTexture(&textureChestArmourFrame);
	chestArmourFrame.setSize(Vector2f(75, 75));
	chestArmourFrame.setOrigin(chestArmourFrame.getSize() / 2.f);
	chestArmourFrame.setPosition(viewCentre.x - 350, 500);

	weaponFrame.setTexture(&textureWeaponFrame);
	weaponFrame.setSize(Vector2f(75, 75));
	weaponFrame.setOrigin(weaponFrame.getSize() / 2.f);
	weaponFrame.setPosition(viewCentre.x - 250, 500);

	wandFrame.setTexture(&textureWandFrame);
	wandFrame.setSize(Vector2f(75, 75));
	wandFrame.setOrigin(wandFrame.getSize() / 2.f);
	wandFrame.setPosition(viewCentre.x - 150, 500);

	itemTooltipBackground.setTexture(&tooltipBackground);
	itemTooltipBackground.setPosition(0, 0);

	itemTooltipName.setFont(font);
	itemTooltipName.setCharacterSize(fontSize - 8);
	itemTooltipName.setFillColor(Color::White);
	itemTooltipName.setPosition(0, 0);
	
	statTooltipText.setFont(font);
	statTooltipText.setCharacterSize(fontSize - 12);
	statTooltipText.setFillColor(Color::White);
	statTooltipText.setPosition(0, 0);

	valueTooltipText.setFont(font);
	valueTooltipText.setCharacterSize(fontSize - 12);
	valueTooltipText.setFillColor(Color::White);
	valueTooltipText.setPosition(0, 0);
	
	// Display kill count inventory text
	killsText.setString("Kills: " + to_string(player.getKillCount())); // Set the label text
	killsText.setFont(font); // Assign the font
	killsText.setCharacterSize(fontSize); // Set the text size
	killsText.setFillColor(Color::White);
	textBounds = killsText.getLocalBounds();
	killsText.setPosition((viewCentre.x - (textBounds.width / 2.f) - textBounds.left) - 50, 260);

	// Gold text
	goldCountText.setFont(font);
	goldCountText.setCharacterSize(fontSize);
	goldCountText.setString("Gold: " + player.getGold());
	goldCountText.setFillColor(Color::White);
	textBounds = goldCountText.getLocalBounds();
	goldCountText.setPosition((viewCentre.x - (textBounds.width / 2.f) - textBounds.left) - 50, 310);

	// Exp text
	expText.setFont(font);
	expText.setCharacterSize(fontSize);
	expText.setString("EXP: " + to_string(player.getPlayerLevel()) + " / " + "100");
	expText.setFillColor(Color::White);
	textBounds = expText.getLocalBounds();
	expText.setPosition((viewCentre.x - (textBounds.width / 2.f) - textBounds.left) - 20, 360);

	// Level text
	levelsText.setFont(font);
	levelsText.setCharacterSize(fontSize);
	levelsText.setString("Level: " + to_string(player.getPlayerLevel()));
	levelsText.setFillColor(Color::White);
	textBounds = levelsText.getLocalBounds();
	levelsText.setPosition((viewCentre.x - (textBounds.width / 2.f) - textBounds.left) - 40, 410);

	invHealthBar.setFillColor(Color::Red);
	invHealthBar.setPosition(viewCentre.x - 360, 775);

	backgroundInvHealthBar.setFillColor(Color::Black);
	backgroundInvHealthBar.setSize(Vector2f(200, 50));
	backgroundInvHealthBar.setPosition(viewCentre.x - 360, 775);

	// Display invHealthBar text
	invHealthBarText.setString("0 / 0"); // Set the initial text
	invHealthBarText.setFont(font); // Assign the font
	invHealthBarText.setCharacterSize(fontSize - 5); // Slightly smaller text size
	invHealthBarText.setFillColor(Color::White);

	invStamBar.setFillColor(Color::Green);
	invStamBar.setPosition(viewCentre.x - 110, 775);

	backgroundInvStamBar.setFillColor(Color::Black);
	backgroundInvStamBar.setSize(Vector2f(200, 50));
	backgroundInvStamBar.setPosition(viewCentre.x - 110, 775);
	
	// Display invStamBar text
	invStamBarText.setString("0 / 0"); // Set the initial text
	invStamBarText.setFont(font); // Assign the font
	invStamBarText.setCharacterSize(fontSize - 5); // Slightly smaller text size
	invStamBarText.setFillColor(Color::White);

	invManaBar.setFillColor(Color::Magenta);
	invManaBar.setPosition(viewCentre.x + 140, 775);

	backgroundInvManaBar.setFillColor(Color::Black);
	backgroundInvManaBar.setSize(Vector2f(200, 50));
	backgroundInvManaBar.setPosition(viewCentre.x + 140, 775);

	// Display invManaBar text
	invManaBarText.setString("0 / 0"); // Set the initial text
	invManaBarText.setFont(font); // Assign the font
	invManaBarText.setCharacterSize(fontSize - 5); // Slightly smaller text size
	invManaBarText.setFillColor(Color::White);

	// Setting volume to 50 by default
	Listener::setGlobalVolume(50);

	// Populate soundtrack
	sound.populateSoundtrack();

	eKey.setTexture(&eKeyTexture);
	eKey.setSize(Vector2f(50, 50));

	tutorialText.setFont(font);
	tutorialText.setCharacterSize(fontSize);
	tutorialText.setFillColor(Color::White);
	tutorialText.setString("You sustained some damage while fleeing from the dragon. Press Tab to open your inventory and heal");
	textBounds = tutorialText.getLocalBounds();
	tutorialText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 900);

	levelUpText.setFont(font);
	levelUpText.setCharacterSize(fontSize);
	levelUpText.setFillColor(Color::Green);
	levelUpText.setString("Level up! Click on the bar you wish to upgrade.");
	textBounds = levelUpText.getLocalBounds();
	levelUpText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 950);

	shopText.setFont(font);
	shopText.setCharacterSize(fontSize);
	shopText.setFillColor(Color::Green);
	shopText.setString("Welcome to the shop! \nClick on one of my an items up top to buy it.\nClick on one of your own items down below to sell it!");
	textBounds = shopText.getLocalBounds();
	shopText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 50);

	inventoryBackground.setTexture(&inventoryBackgroundTexture);
	inventoryBackground.setSize(Vector2f(1000, 800));
	textBounds = inventoryBackground.getLocalBounds();
	inventoryBackground.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, viewCentre.y - (textBounds.height / 2.f) - textBounds.top);

	/*****************
	Hotbar UI elements
	*****************/
	swordBox.setTexture(&textureEmptyFrame);
	swordBox.setSize(Vector2f(75, 75));
	swordBox.setPosition(viewCentre.x - 288, 1000);

	wandBox.setTexture(&textureEmptyFrame);
	wandBox.setSize(Vector2f(75, 75));
	wandBox.setPosition(viewCentre.x - 188, 1000);
	
	spellBox1.setTexture(&textureEmptyFrame);
	spellBox1.setSize(Vector2f(75, 75));
	spellBox1.setPosition(viewCentre.x - 88, 1000);

	spellBox2.setTexture(&textureEmptyFrame);
	spellBox2.setSize(Vector2f(75, 75));
	spellBox2.setPosition(viewCentre.x + 13, 1000);

	spellBox3.setTexture(&textureEmptyFrame);
	spellBox3.setSize(Vector2f(75, 75));
	spellBox3.setPosition(viewCentre.x + 113, 1000);

	spellBox4.setTexture(&textureEmptyFrame);
	spellBox4.setSize(Vector2f(75, 75));
	spellBox4.setPosition(viewCentre.x + 213, 1000);

	weaponIndicator.setTexture(&indicator);
	weaponIndicator.setSize(Vector2f(48, 36));
	weaponIndicator.setOrigin(weaponIndicator.getSize() / 2.f);
	weaponIndicator.setPosition(swordBox.getPosition().x + 37.5, swordBox.getPosition().y - 25);

	spellIndicator.setTexture(&indicator);
	spellIndicator.setSize(Vector2f(48, 36));
	spellIndicator.setOrigin(spellIndicator.getSize() / 2.f);
	spellIndicator.setPosition(spellBox1.getPosition().x + 37.5, spellBox1.getPosition().y - 25);

	swordIcon.setTexture(&textureItems);
	swordIcon.setSize(Vector2f(75, 75));
	swordIcon.setOrigin(swordIcon.getSize() / 2.f);
	swordIcon.setPosition(swordBox.getPosition() + swordBox.getSize() / 2.f);
	equippedSwordIcon.setTextureRect(player.getEquippedSword()->getTextureRect());

	wandIcon.setTexture(&textureItems);
	wandIcon.setSize(Vector2f(75, 75));
	wandIcon.setOrigin(wandIcon.getSize() / 2.f);
	wandIcon.setPosition(wandBox.getPosition() + wandBox.getSize() / 2.f);
	equippedWandIcon.setTextureRect(player.getEquippedWand()->getTextureRect());

	healSpell = TextureHolder::GetTexture("graphics/UI/healSpell.png");
	fireBallSpell = TextureHolder::GetTexture("graphics/UI/fireballSpell.png");
	phaseSpell = TextureHolder::GetTexture("graphics/UI/phaseSpell.png");
	freezeTimeSpell = TextureHolder::GetTexture("graphics/UI/freezeTimeSpell.png");
	
	spell1Icon.setTexture(&fireBallSpell);
	spell1Icon.setSize(Vector2f(60, 60));
	spell1Icon.setOrigin(spell1Icon.getSize() / 2.f);
	spell1Icon.setPosition(spellBox1.getPosition() + spellBox1.getSize() / 2.f);

	spell2Icon.setTexture(&healSpell);
	spell2Icon.setSize(Vector2f(60, 60));
	spell2Icon.setOrigin(spell2Icon.getSize() / 2.f);
	spell2Icon.setPosition(spellBox2.getPosition() + spellBox2.getSize() / 2.f);

	spell3Icon.setTexture(&freezeTimeSpell);
	spell3Icon.setSize(Vector2f(60, 60));
	spell3Icon.setOrigin(spell3Icon.getSize() / 2.f);
	spell3Icon.setPosition(spellBox3.getPosition() + spellBox3.getSize() / 2.f);

	spell4Icon.setTexture(&phaseSpell);
	spell4Icon.setSize(Vector2f(60, 60));
	spell4Icon.setOrigin(spell4Icon.getSize() / 2.f);
	spell4Icon.setPosition(spellBox4.getPosition() + spellBox4.getSize() / 2.f);

	// Making the inventory background filter black and 25% alpha
	darkInventoryBackground.setFillColor(Color(0, 0, 0, 64));
	darkInventoryBackground.setSize(resolution);
	darkInventoryBackground.setPosition(0, 0);

	// Is the time currently frozen by a spell?
	timeFrozen = false;

	// Text box setup
	textBox.setSize({ 300, 50 });
	textBounds = textBox.getLocalBounds();
	viewCentre = mainView.getCenter();
	textBox.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 800);
	textBox.setFillColor(sf::Color(40, 40, 40));
	textBox.setOutlineThickness(2.f);
	textBox.setOutlineColor(Color::White);

	// Text that shows the number the user types
	userInputText.setFont(font);
	userInputText.setCharacterSize(fontSize - 11);
	userInputText.setFillColor(Color::White);
	userInputText.setPosition(textBox.getPosition().x + 10, textBox.getPosition().y + 10);

	// Feedback message
	feedback.setFont(font);
	feedback.setCharacterSize(fontSize - 15);
	feedback.setFillColor(Color::Yellow);
	feedback.setString("Enter a number whose square is odd (e.g. 5 or 7)\nHit enter key when done");
	textBounds = feedback.getLocalBounds();
	viewCentre = mainView.getCenter();
	feedback.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 870);

	// Is the player currently typing in the text box?
	textBoxActive = false;
}

// Function to convert difficulty state to string
string Engine::difficultyToString(Difficulty difficulty)
{
	switch (difficulty)
	{
	case Difficulty::Easy:   return "Easy";
	case Difficulty::Medium: return "Medium";
	case Difficulty::Hard:   return "Hard";
	}
	return "Unknown";
}

// Function to convert difficulty in string form to state
Engine::Difficulty Engine::stringToDifficulty(string str)
{
	if (str == "Easy") {return Difficulty::Easy; }
	else if (str == "Medium") {return Difficulty::Medium; }
	else if (str == "Hard") { return Difficulty::Hard; }
	else return Difficulty::Medium;
}

// Function to populate the chunk vector with chunks
void Engine::populateChunkVector()
{
	for (int i = 0; i < world.getWorldSize(); i++) {
		chunks.push_back(*world.getChunk(i));
	}
}

// Function to detect the current chunk an entity finds themselves in
Chunk* Engine::getCurrentChunk(float x, float y) {
	for (Chunk& chunk : chunks) {
		FloatRect area = chunk.getChunkArea().getShape().getGlobalBounds();
		if (collision.pointInShape(Vector2f(x,y), chunk.getChunkArea().getShape())) {
			return &chunk;
		}
	}
	return nullptr;
}

void Engine::run()
{
	initializeInventory();
	initializeShop();
	// Posistion shop items
	for (int i = 0; i < shopItems.size(); i++) {
		if (!shopItems[i].isNull()) {
			shopItems[i].getIcon().setPosition(emptyFrames[i].getPosition());
		}
	}

	for (int i = 0; i < shopItems.size(); i++) {
		if (!shopItems[i].isNull()) {
			shopItems[i].getIcon().setPosition(shopFrames[i].getPosition());
		}
	}



	// The main game loop
	while (window.isOpen())
	{
		if (displayFps) {
			// Calculating fps
			float deltaTime = fpsClock.restart().asSeconds();
			fps = fps * 0.9f + (1.f / deltaTime) * 0.1f;
			fpsText.setString("FPS: " + to_string((int)fps));
		}
		//initializeInventory();

		// Getting the mouse position and mapping those pixels to coordinates
		mousePos = Mouse::getPosition(window);
		worldPos = window.mapPixelToCoords(mousePos);
		/***********
		Handle input
		************/

		// Handle events
		Event event;
		while (window.pollEvent(event))
		{
			// Getting the mouse position and mapping those pixels to coordinates
			//Vector2i mousePos = Mouse::getPosition(window);
			//Vector2f worldPos = window.mapPixelToCoords(mousePos);
			//Moved code to main game loop

			// Zooming in and out using the scroll wheel
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

			// Switch between sword and wand when middle mouse / f key pressed
			if ((event.type == Event::MouseButtonPressed && event.key.code == Mouse::Middle && state == State::PLAYING) ||
				(event.type == Event::KeyPressed && event.key.code == Keyboard::F && state == State::PLAYING))
			{
				player.switchWeapon();
			}

			// Handles dragging of the volume slider
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left && state == State::OPTIONS_MENU)
			{
				if (handle.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
				{
					dragging = true;
				}
			}

			// Activate textbox when clicked
			if (event.type == sf::Event::MouseButtonPressed && state == State::OPTIONS_MENU)
			{
				if (event.mouseButton.button == Mouse::Left)
				{
					sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					textBoxActive = textBox.getGlobalBounds().contains(mousePos);
					textBox.setOutlineColor(textBoxActive ? Color::Green : Color::White);
				}
			}

			// Handle typing if text box active
			if (textBoxActive && event.type == sf::Event::TextEntered && state == State::OPTIONS_MENU)
			{
				if (event.text.unicode >= '0' && event.text.unicode <= '9')
				{
					userInputString += static_cast<char>(event.text.unicode);
				}
				else if (event.text.unicode == 8 && !userInputString.empty()) // User hits backspace
				{
					userInputString.pop_back();
				}
				else if (event.text.unicode == 13) // User hits enter key
				{
					if (!userInputString.empty())
					{
						int num = stoi(userInputString);
						if (num > 0 && (num * num) % 2 == 1)
						{
							feedback.setString(to_string(num) + " is valid!");
							world.setWorldSize(num);
						}
						else
						{
							feedback.setString("Invalid! Try another positive number.");
						}
					}
				}

				userInputText.setString(userInputString);
			}

			// Stop dragging
			if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
			{
				dragging = false;
			}

			if (event.type == Event::KeyPressed || Mouse::isButtonPressed(Mouse::Left))
			{
				// Pause the game if escape key pressed
				if (event.key.code == Keyboard::Escape && state == State::PLAYING && !drawInventory)
				{
					state = State::PAUSED;
				}

				// Unpause game if escape key pressed and game is paused
				else if (event.key.code == Keyboard::Escape && state == State::PAUSED)
				{
					state = State::PLAYING;
					// Reset the clock so there isn't a frame jump
					clock.restart();
				}

				// Player hit the new game button in the main menu
				else if (newGameButton.getGlobalBounds().contains(worldPos) && state == State::MAIN_MENU && event.mouseButton.button == Mouse::Left)
				{
					state = State::STORY_INTRO;

					skipAnimation = false;

					// Play the start game sound
					if (!startSoundPlayed) {
						sound.playStartGameSound();
					}

					sound.playStoryIntroSound();

					startSoundPlayed = true;

					player.createNewSave();
					player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume(), vSync);
					player.loadSaveFile();

					equippedSwordIcon.setTextureRect(player.getEquippedSword()->getTextureRect());
					equippedWandIcon.setTextureRect(player.getEquippedWand()->getTextureRect());
					equippedHeadArmourIcon.setTextureRect(player.getEquippedHeadArmour()->getTextureRect());
					equippedChestArmourIcon.setTextureRect(player.getEquippedChestArmour()->getTextureRect());
					equippedTrousersArmourIcon.setTextureRect(player.getEquippedTrouserArmour()->getTextureRect());
					equippedShoeArmourIcon.setTextureRect(player.getEquippedShoeArmour()->getTextureRect());
					equippedNeckArmourIcon.setTextureRect(player.getEquippedNeckArmour()->getTextureRect());

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
				
					// Reset the clock so there isn't a frame jump
					clock.restart();

					player.loadConfigFile();
					difficulty = stringToDifficulty(player.getdifficultyString());
					windowedMode = player.getWindowedMode();
					displayFps = player.getDisplayFps();
					Listener::setGlobalVolume(player.getVolume());

					mainView.setCenter(resolution.x / 2.f, resolution.y / 2.f);
					
					// Skip intro text
					skipIntroText.setString("--- Press space to skip ---"); // Set the text content
					textBounds = skipIntroText.getLocalBounds();
					viewCentre = mainView.getCenter();
					skipIntroText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 1030);
				
					// Loads world using multi-threading
					worldLoaded = false;
					thread worldThread(&Engine::generateWorld, this);
					worldThread.detach();

					setDifficulty();

					tutorialStage = 0;

					// addItemToInventory("Health_Potion");
					initializeInventory();
				}

				// Player hit the load game button in the main menu
				else if (loadGameButton.getGlobalBounds().contains(worldPos) && state == State::MAIN_MENU && event.mouseButton.button == Mouse::Left)
				{
					state = State::PLAYING;

					skipAnimation = false;

					// Play the start game sound
					if (!startSoundPlayed) {
						sound.playStartGameSound();
					}

					startSoundPlayed = true;

					// Loads player stats from text file
					if (player.loadSaveFile() == true) 
					{
						// Player loaded successfully
						world.loadWorld();

						// Update equipped item icons
						equippedSwordIcon.setTextureRect(player.getEquippedSword()->getTextureRect());
						equippedWandIcon.setTextureRect(player.getEquippedWand()->getTextureRect());
						equippedHeadArmourIcon.setTextureRect(player.getEquippedHeadArmour()->getTextureRect());
						equippedChestArmourIcon.setTextureRect(player.getEquippedChestArmour()->getTextureRect());
						equippedTrousersArmourIcon.setTextureRect(player.getEquippedTrouserArmour()->getTextureRect());
						equippedShoeArmourIcon.setTextureRect(player.getEquippedShoeArmour()->getTextureRect());
						equippedNeckArmourIcon.setTextureRect(player.getEquippedNeckArmour()->getTextureRect());

						// We will modify the next two lines later
						arena.width = 1900;
						arena.height = 800;
						arena.left = 1664;
						arena.top = 1664;

						// Pass the vertex array by reference to the createBackground function
						int tileSize = 64;

						// Spawn the player in the middle of the arena
						player.spawn(arena, resolution, tileSize, player.getPlayerLevel());

						/*
						for (int i = 0; i < 1; ++i)
						{
							enemies.spawn(arena, resolution, tileSize, "Skeleton", player.getPlayerLevel());
						}
						*/

						// Reset the clock so there isn't a frame jump
						clock.restart();

						player.loadConfigFile();
						difficulty = stringToDifficulty(player.getdifficultyString());
						windowedMode = player.getWindowedMode();
						vSync = player.getVSync();
						displayFps = player.getDisplayFps();
						Listener::setGlobalVolume(player.getVolume());
						populateChunkVector();
						setDifficulty();
						spawnEnemies();
						initializeInventory();
					}
					else {
						// No save file so create a new one with default values and load it	
						player.createNewSave();
						player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume(), vSync);
						player.loadSaveFile();

						equippedSwordIcon.setTextureRect(player.getEquippedSword()->getTextureRect());
						equippedWandIcon.setTextureRect(player.getEquippedWand()->getTextureRect());
						equippedHeadArmourIcon.setTextureRect(player.getEquippedHeadArmour()->getTextureRect());
						equippedChestArmourIcon.setTextureRect(player.getEquippedChestArmour()->getTextureRect());
						equippedTrousersArmourIcon.setTextureRect(player.getEquippedTrouserArmour()->getTextureRect());
						equippedShoeArmourIcon.setTextureRect(player.getEquippedShoeArmour()->getTextureRect());
						equippedNeckArmourIcon.setTextureRect(player.getEquippedNeckArmour()->getTextureRect());

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

						/*
						for (int i = 0; i < 1; ++i)
						{
							Enemy e;
							e.spawn(arena, resolution, tileSize, "Goblin", player.getPlayerLevel());
							enemyArr.push_back(e);
						}
						*/

						// Reset the clock so there isn't a frame jump
						clock.restart();

						player.loadConfigFile();
						difficulty = stringToDifficulty(player.getdifficultyString());
						windowedMode = player.getWindowedMode();
						vSync = player.getVSync();
						displayFps = player.getDisplayFps();
						Listener::setGlobalVolume(player.getVolume());
						populateChunkVector();
						setDifficulty();
						spawnEnemies();
						initializeInventory();
					}
				}

				// Player hit the options button
				if (optionsButton.getGlobalBounds().contains(worldPos) && state == State::MAIN_MENU && event.mouseButton.button == Mouse::Left)
				{
					player.loadConfigFile();
					difficulty = stringToDifficulty(player.getdifficultyString());
					sound.playButtonClickSound();
					world.clearWorld();

					float savedVolume = track.getPosition().x + track.getSize().x * (player.getVolume() / 100.0f);
					handle.setPosition(savedVolume, handle.getPosition().y);

					// Map handle position to global volume

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
					player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume(), vSync);
					state = State::MAIN_MENU;
				}

				// Player hit the main menu button in the pause menu
				if (mainMenuButton.getGlobalBounds().contains(worldPos) && state == State::PAUSED && event.mouseButton.button == Mouse::Left)
				{
					sound.playButtonClickSound();
					world.clearWorld();
					player.updateSaveFile();
					enemyArr.clear();
					state = State::MAIN_MENU;
				}

				// Player hit the display fps button
				if (displayFPSButton.getGlobalBounds().contains(worldPos) && state == State::OPTIONS_MENU && event.mouseButton.button == Mouse::Left)
				{
					if (displayFps) {
						sound.playButtonClickSound();
						displayFps = false;
					}
					else {
						sound.playButtonClickSound();
						displayFps = true;
					}
				}

				// Player hit the windowed mode button
				if (windowedModeButton.getGlobalBounds().contains(worldPos) && state == State::OPTIONS_MENU && event.mouseButton.button == Mouse::Left)
				{
					if (windowedMode) {
						sound.playButtonClickSound();
						windowedMode = false;
						window.create(VideoMode(resolution.x, resolution.y), "TimeCaster", Style::Fullscreen);
					}
					else {
						sound.playButtonClickSound();
						windowedMode = true;
						window.create(VideoMode(resolution.x, resolution.y), "TimeCaster", Style::Default);
					}
				}
				//Player hit the vSync button
				if (vSyncButton.getGlobalBounds().contains(worldPos) && state == State::OPTIONS_MENU && event.mouseButton.button == Mouse::Left)
				{
					if (vSync) {
						sound.playButtonClickSound();
						vSync = false;
						window.setVerticalSyncEnabled(false);
					}
					else {
						sound.playButtonClickSound();
						vSync = true;
						window.setVerticalSyncEnabled(true);
					}
				}

				// Player hit the debug mode button
				if (debugModeButton.getGlobalBounds().contains(worldPos) && state == State::OPTIONS_MENU && event.mouseButton.button == Mouse::Left)
				{
					if (debugMode) {
						sound.playButtonClickSound();
						debugMode = false;
					}
					else {
						sound.playButtonClickSound();
						debugMode = true;
					}
				}

				// Player hit the difficulty button
				if (difficultyButton.getGlobalBounds().contains(worldPos) && state == State::OPTIONS_MENU && event.mouseButton.button == Mouse::Left)
				{
					switch (difficulty) {
					case Difficulty::Easy:
						difficulty = Difficulty::Medium;
						break;

					case Difficulty::Medium:
						difficulty = Difficulty::Hard;
						break;

					case Difficulty::Hard:
						difficulty = Difficulty::Easy;
						break;
					}

					difficultyButtonText.setString("Difficulty: " + difficultyToString(difficulty));
					textBounds = difficultyButtonText.getLocalBounds();
					x = difficultyButton.getPosition().x + (difficultyButton.getSize().x / 2.f) - (textBounds.width / 2.f);
					y = difficultyButton.getPosition().y + (difficultyButton.getSize().y / 2.f) - (textBounds.height / 2.f);
					difficultyButtonText.setPosition(x - textBounds.left, y - textBounds.top);
					difficultyButton.setSize(Vector2f(textBounds.width + 100, 100));
					difficultyButton.setPosition(viewCentre.x - (difficultyButton.getSize().x /2.f), 400);

					sound.playButtonClickSound();
				}

				// Player pressed space bar in the intro scene
				if (event.key.code == Keyboard::Space)
				{
					if (state == State::STORY_INTRO && !skipAnimation) {
						storyIntroText.setString(fullText);
						skipAnimation = true;
					}
					else if (state == State::STORY_INTRO && skipAnimation && worldLoaded) {
						sound.stopStoryIntroSound();
						state = State::PLAYING;
						displayedText = "";
						storyIntroText.setString(displayedText);
						currentChar = 0;
					}
				}

				// Player hit tab while playing
				if (state == State::PLAYING && event.key.code == Keyboard::Tab && tutorialStage != 1 && !drawShop)
				{
					if (drawInventory) {
						drawInventory = false;
					}
					else {
						drawInventory = true;
					}
				}
			}

			if (event.type == Event::KeyPressed)
			{
				// below are debug functions, comment them out in full build / when needed
				
				// Debug shop toggle
				// Shop is still very much WIP
				if (event.key.code == Keyboard::O && state == State::PLAYING && debugMode)
				{
					if (drawShop) {
						drawShop = false;
						cout << "Closing shop" << endl;
					}
					else {
						drawShop = true;
						cout << "Opening shop" << endl;
					}
				}

				if (event.key.code == Keyboard::Num1 && state == State::PLAYING && debugMode)
				{
					// Increase health
					player.upgradeHealth();
					player.switchSpell(1);
			
				}

				if (event.key.code == Keyboard::Num2 && state == State::PLAYING && debugMode)
				{
					// Increase stamina
					player.upgradeStamina();
					player.switchSpell(2);
				}

				if (event.key.code == Keyboard::Num3 && state == State::PLAYING && debugMode)
				{
					// Increase health
					player.upgradeMana();
					player.switchSpell(3);
				}

				if (event.key.code == Keyboard::Num4 && state == State::PLAYING && debugMode)
				{
					if (player.addItemToInventory("Iron_Sword"))
					{
						initializeInventory();
						cout << "Item added to inventory" << endl;
					}
					else
					{
						cout << "No space in inventory" << endl;
					}
					player.switchSpell(4);
				}

				if (event.key.code == Keyboard::Num5 && state == State::PLAYING && debugMode)
				{
					player.addGold(100);
				}

				if (event.key.code == Keyboard::Num6 && state == State::PLAYING && debugMode)
				{
					if (player.reward(80))
					{
						drawInventory = true;
						levelUp = true;
					}
				}

				if (event.key.code == Keyboard::Num8 && state == State::PLAYING && debugMode)
				{
					player.hit(gameTimeTotal, 10, 200);
				}

				if (event.key.code == Keyboard::Num9 && state == State::PLAYING && debugMode)
				{
					player.hit(gameTimeTotal, 30, 1000);
				}

				if (event.key.code == Keyboard::G && state == State::PLAYING && debugMode)
				{
					for (int i = 0; i < (rand() % 10); i++) {
						items.emplace_back("Gold", Vector2f(player.getPosition().x, player.getPosition().y));
					}
				}

				if (event.key.code == Keyboard::C && state == State::PLAYING && debugMode)
				{
					player.setInCell();
				}
			}

		} // End event polling

		// Handle controls while playing
		if (state == State::PLAYING && !drawInventory)
		{
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				player.Attack();
				if (player.getCombatType() == Melee) {
					sound.playSwordSwing();
				}
				
				if (player.getCombatType() == Magic && !player.isCastingSpell() && player.getSpellType() == Player::SpellType::Fireball)
				{
					if (player.useMana(5.0f))
					{
						spells[currentSpell].shoot(player.getCenter().x, player.getCenter().y + 10, mouseWorldPosition.x, mouseWorldPosition.y, player.getEquippedWand()->getDamage());

						// Play fireball sound
						sound.playFireballSound();

						currentSpell++;
						if (currentSpell > 99)
						{
							currentSpell = 0;
						}
						player.castingSpell(true);
					}
				}
				else if (player.getCombatType() == Magic && !player.isCastingSpell() && player.getSpellType() == Player::SpellType::FreezeTime && !timeFrozen && timeFrozenTimer.getElapsedTime().asSeconds() > 1)
				{
					timeFrozen = true;
					sound.playTimeStopCastSound();
					timeFrozenTimer.restart();
				}
				else if (player.getCombatType() == Magic && !player.isCastingSpell() && player.getSpellType() == Player::SpellType::FreezeTime && timeFrozen && timeFrozenTimer.getElapsedTime().asSeconds() > 1)
				{
					timeFrozen = false;
					sound.playTimeStopEndSound();
					sound.stopTimeStopActiveSound();
					timeFrozenTimer.restart();
				}
				else if (player.getCombatType() == Magic && !player.isCastingSpell() && player.getSpellType() == Player::SpellType::Heal && player.getHealth() < player.getMaxHealth())
				{
					if (player.useMana(0.5f))
					{
						sound.playHealSound();
						player.healHealth(0.25f);
						particles[100].play(player.getCenter().x -30, player.getCenter().y -30, 0); // 100 is the player's particle, 0-99 for the enemies
					}		
				}
			}

			// Handle the pressing and releasing the WASD keys
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

			if (vSync) {
				vSyncButton.setFillColor(Color::Green);
			}
			else {
				vSyncButton.setFillColor(Color::Red);
			}

			if (debugMode) {
				debugModeButton.setFillColor(Color::Green);
			}
			else {
				debugModeButton.setFillColor(Color::Red);
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
			else // Hard
			{
				difficultyButton.setFillColor(Color::Red);
			}
		}

		// Sets health to 0 if it goes below 0
		if (player.getHealth() < 0) {
			player.setHealthValue(0);
		}

		// Sets stamina to 0 if it goes below 0
		if (player.getStamina() < 0) {
			player.setStaminaValue(0);
		}

		// Sets mana to 0 if it goes below 0
		if (player.getMana() < 0) {
			player.setManaValue(0);
		}

		update(); // update game 

		if (state == State::MAIN_MENU)
		{
			startSoundPlayed = FALSE;
		}

		// Displays intro text in a typewriter style
		if (state == State::STORY_INTRO && !skipAnimation) {
			if (currentChar < (int)fullText.size())
			{
				if (clock.getElapsedTime().asSeconds() >= delay)
				{
					displayedText += fullText[currentChar];
					storyIntroText.setString(displayedText);
					currentChar++;
					clock.restart();
				}
			}
		}

		// Stops sound track in options and main menus
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

		// Handles dragging the volume slider in the options menu
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
				player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume(), vSync);
				isDragging = false;
			}
		}
		draw();
	} // End of main game loop
}

// Function used to generate the world and inform player that world generation is in progress
void Engine::generateWorld()
{
	skipIntroText.setString("World is loading...");
	textBounds = skipIntroText.getLocalBounds();
	viewCentre = mainView.getCenter();
	skipIntroText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 1030);
	world.newWorld();
	populateChunkVector();
	spawnEnemies();
	worldLoaded = true;
	skipIntroText.setString("--- Press space to skip ---");
	textBounds = skipIntroText.getLocalBounds();
	viewCentre = mainView.getCenter();
	skipIntroText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 1030);
}

// Sets the player's difficulty multiplier
void Engine::setDifficulty()
{
	if (difficulty == Difficulty::Easy) {
		player.setDifficultyMult(0.75f);
	}
	else if (difficulty == Difficulty::Medium) {
		player.setDifficultyMult(1.0f);
	}
	else if (difficulty == Difficulty::Hard) {
		player.setDifficultyMult(1.25f);
	}
}

// Function used to spawn enemies into the game world
void Engine::spawnEnemies()
{
	vector<string> worldEnemyTypes;
	vector<Vector2i> worldEnemyLocations;

	for (int i = 0; i < world.getWorldSize(); i++)
	{
		vector<string> chunkEnemyTypes = world.getChunk(i)->getEnemyTypes();
		vector<Vector2i> chunkEnemyLocations = world.getChunk(i)->getEnemyLocations();

		worldEnemyTypes.insert(worldEnemyTypes.end(), chunkEnemyTypes.begin(), chunkEnemyTypes.end());
		worldEnemyLocations.insert(worldEnemyLocations.end(), chunkEnemyLocations.begin(), chunkEnemyLocations.end());
	}

	for (int i = 0; i < worldEnemyTypes.size(); i++)
	{
		Enemy e;
		e.spawn(worldEnemyTypes[i], worldEnemyLocations[i], player.getPlayerLevel());
		enemyArr.push_back(e);
	}
}

bool Engine::isEquipment(const string& name)
{
	static const vector<string> equipmentList = {
		"Steel_Plate_Helmet", "Steel_Plate_Chestplate", "Steel_Plate_Pants",
		"Steel_Plate_Boots", "Chain_Mail", "Chain_Hood", "Soldiers_Helmet",
		"Leather_Cap", "Leather_Chestplate", "Leather_Leggings", "Leather_Boots",
		"Family_Hood", "Family_Robe", "Robe_Leggings", "Basic_Shoes"
	};
	return find(equipmentList.begin(), equipmentList.end(), name) != equipmentList.end();
}

bool Engine::isWeapon(const string& name)
{
	static const vector<string> weaponList = {
		"Iron_Sword", "Rapier"
	};
	return find(weaponList.begin(), weaponList.end(), name) != weaponList.end();
}