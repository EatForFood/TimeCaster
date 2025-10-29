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

using namespace std;
using namespace sf;

Engine::Engine() : m_EquippedWeapons(player.getEquippedWeapons()), m_EquippedArmour(player.getEquippedArmour())	
{
	player.loadConfigFile();

//	difficulty = Difficulty::Medium;
	difficulty = stringToDifficulty(player.getdifficultyString());
	windowedMode = player.getWindowedMode();
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

	filter.setSize(resolution);
	filter.setFillColor(defaultFilter);

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

	// Levelling up
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	levelUpStream <<
		"1- Increased rate of fire" <<
		"\n2- Increased clip size(next reload)" <<
		"\n3- Increased max health" <<
		"\n4- Increased run speed" <<
		"\n5- More and better health pickups" <<
		"\n6- More and better ammo pickups";
	levelUpText.setString(levelUpStream.str());

	// FPS text
	fpsText.setFont(font);
	fpsText.setCharacterSize(fontSize - 5);
	fpsText.setFillColor(Color::Yellow);
	fpsText.setPosition(1800, 5);

	// Health bar
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(10,10);

	emptyHealthBar.setFillColor(Color::Black);
	emptyHealthBar.setPosition(10, 10);

	// Stamina bar
	staminaBar.setFillColor(Color::Green);
	staminaBar.setPosition(10, 60);

	// Empty Stamina bar
	emptyStaminaBar.setFillColor(Color::Black);
	emptyStaminaBar.setPosition(10, 60);

	// Mana bar
	manaBar.setFillColor(Color::Magenta);
	manaBar.setPosition(10, 110);

	storedItems.resize(16, Item("null", Vector2f(300, 650)));

	//Item 0 is sword (melee combat), item 1 is wand (magic combat)
	m_EquippedWeapons.resize(2, Weapon("null", Vector2f(0, 0)));
	
	// Item 0 is head, item 1 is torso, item 2 is pants, item 3 is shoes, item 4 is neck
	m_EquippedArmour.resize(5, Equipment("null", Vector2f(0, 0)));
	
	// Debug inventory initalization
	storedItems[0] = Weapon("Wooden_Wand", Vector2f(300, 650));
	storedItems[1] = Weapon("Pirate's_Scimitar", Vector2f(450, 650));
	storedItems[2] = Weapon("Iron_Sword", Vector2f(600, 650));
	storedItems[3] = Weapon("Silver_Wand", Vector2f(750, 650));
	storedItems[4] = Equipment("Family_Robe", Vector2f(900, 650));
	storedItems[5] = Equipment("Family_Hood", Vector2f(0, 0));
	storedItems[6] = Equipment("Leather_Cap", Vector2f(0, 0));
	storedItems[7] = Equipment("Leather_Chestplate", Vector2f(0, 0));
	storedItems[8] = Equipment("Basic_Shoes", Vector2f(0, 0));
	storedItems[9] = Equipment("Robe_Leggings", Vector2f(0, 0));
	storedItems[10] = Weapon("Reaper's_Scythe", Vector2f(0, 0));
	storedItems[11] = Equipment("Family_Locket", Vector2f(0, 0));
	storedItems[12] = Weapon("Armoured_Boots", Vector2f(0, 0));
	storedItems[13] = Item("Health_Potion", Vector2f(0, 0));

	// Empty mana bar
	emptyManaBar.setFillColor(Color::Black);
	emptyManaBar.setPosition(10, 110);

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
	windowedModeButtonText.setString("Windowed Mode");  // Set the display text
	windowedModeButtonText.setFont(font);               // Assign the font
	windowedModeButtonText.setCharacterSize(fontSize - 5);
	windowedModeButtonText.setFillColor(Color::Black);
	textBounds = windowedModeButtonText.getLocalBounds();
	x = windowedModeButton.getPosition().x + (windowedModeButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	y = windowedModeButton.getPosition().y + (windowedModeButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	windowedModeButtonText.setPosition(x - textBounds.left, y - textBounds.top);

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
	difficultyButtonText.setFont(font);                              // Assign the font
	difficultyButtonText.setCharacterSize(fontSize - 5);
	difficultyButtonText.setFillColor(Color::Black);
	textBounds = difficultyButtonText.getLocalBounds();
	x = difficultyButton.getPosition().x + (difficultyButton.getSize().x / 2.f) - (textBounds.width / 2.f);
	y = difficultyButton.getPosition().y + (difficultyButton.getSize().y / 2.f) - (textBounds.height / 2.f);
	difficultyButtonText.setPosition(x - textBounds.left, y - textBounds.top);

	// Story into text
	storyIntroText.setFont(font);
	storyIntroText.setCharacterSize(fontSize + 10);
	storyIntroText.setFillColor(Color::White);
	storyIntroText.setPosition(150, 150);

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
	skipIntroText.setString("--- Press space to skip ---"); // Set the text content
	skipIntroText.setFont(font);                    // Assign the font
	skipIntroText.setCharacterSize(fontSize - 5);   // Slightly smaller text size
	skipIntroText.setFillColor(Color::White);
	textBounds = skipIntroText.getLocalBounds();
	viewCentre = mainView.getCenter();
	skipIntroText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 1000);

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

	// Equipped item icons
	equippedSwordIcon.setTexture(&textureItems);
	equippedSwordIcon.setTextureRect(IntRect(0, 0, 0, 0));
	equippedSwordIcon.setSize(Vector2f(75, 75));
	equippedSwordIcon.setOrigin(equippedSwordIcon.getSize() / 2.f);
	equippedSwordIcon.setPosition(viewCentre.x - 200, 550);

	equippedWandIcon.setTexture(&textureItems);
	equippedWandIcon.setTextureRect(IntRect(0, 0, 0, 0));
	equippedWandIcon.setSize(Vector2f(75, 75));
	equippedWandIcon.setOrigin(equippedWandIcon.getSize() / 2.f);
	equippedWandIcon.setPosition(viewCentre.x - 100, 550);

	equippedHeadArmourIcon.setTexture(&textureItems);
	equippedHeadArmourIcon.setTextureRect(IntRect(0, 0, 0, 0));
	equippedHeadArmourIcon.setSize(Vector2f(75, 75));
	equippedHeadArmourIcon.setOrigin(equippedHeadArmourIcon.getSize() / 2.f);
	equippedHeadArmourIcon.setPosition(viewCentre.x - 100, 350);

	equippedChestArmourIcon.setTexture(&textureItems);
	equippedChestArmourIcon.setTextureRect(IntRect(0, 0, 0, 0));
	equippedChestArmourIcon.setSize(Vector2f(75, 75));
	equippedChestArmourIcon.setOrigin(equippedChestArmourIcon.getSize() / 2.f);
	equippedChestArmourIcon.setPosition(viewCentre.x - 300, 550);

	equippedTrousersArmourIcon.setTexture(&textureItems);
	equippedTrousersArmourIcon.setTextureRect(IntRect(0, 0, 0, 0));
	equippedTrousersArmourIcon.setSize(Vector2f(75, 75));
	equippedTrousersArmourIcon.setOrigin(equippedTrousersArmourIcon.getSize() / 2.f);
	equippedTrousersArmourIcon.setPosition(viewCentre.x - 300, 450);

	equippedShoeArmourIcon.setTexture(&textureItems);
	equippedShoeArmourIcon.setTextureRect(IntRect(0, 0, 0, 0));
	equippedShoeArmourIcon.setSize(Vector2f(75, 75));
	equippedShoeArmourIcon.setOrigin(equippedShoeArmourIcon.getSize() / 2.f);
	equippedShoeArmourIcon.setPosition(viewCentre.x - 100, 450);

	equippedNeckArmourIcon.setTexture(&textureItems);
	equippedNeckArmourIcon.setTextureRect(IntRect(0, 0, 0, 0));
	equippedNeckArmourIcon.setSize(Vector2f(75, 75));
	equippedNeckArmourIcon.setOrigin(equippedNeckArmourIcon.getSize() / 2.f);
	equippedNeckArmourIcon.setPosition(viewCentre.x - 300, 350);

	// Player frame
	playerFrame.setSize(sf::Vector2f(100.f, 200.f));
	playerFrame.setTexture(&texturePlayerFrame);
	playerFrame.setOrigin(playerFrame.getSize() / 2.f);
	playerFrame.setPosition(viewCentre.x - 200, 400);

	// Player sprite for frame
	playerInFrame.setSize(sf::Vector2f(60.f, 100.f));
	playerInFrame.setTexture(&texturePlayerInFrame);
	playerInFrame.setOrigin(playerInFrame.getSize() / 2.f);
	playerInFrame.setPosition(viewCentre.x - 200, 410);

	neckFrame.setTexture(&textureNeckFrame);
	neckFrame.setSize(Vector2f(75, 75));
	neckFrame.setOrigin(neckFrame.getSize() / 2.f);
	neckFrame.setPosition(viewCentre.x - 300, 350);

	headArmourFrame.setTexture(&textureHeadArmourFrame);
	headArmourFrame.setSize(Vector2f(75, 75));
	headArmourFrame.setOrigin(headArmourFrame.getSize() / 2.f);
	headArmourFrame.setPosition(viewCentre.x - 100, 350);

	trousersArmourFrame.setTexture(&textureTrousersArmourFrame);
	trousersArmourFrame.setSize(Vector2f(75, 75));
	trousersArmourFrame.setOrigin(trousersArmourFrame.getSize() / 2.f);
	trousersArmourFrame.setPosition(viewCentre.x - 300, 450);

	bootsArmourFrame.setTexture(&textureBootsArmourFrame);
	bootsArmourFrame.setSize(Vector2f(75, 75));
	bootsArmourFrame.setOrigin(bootsArmourFrame.getSize() / 2.f);
	bootsArmourFrame.setPosition(viewCentre.x - 100, 450);

	chestArmourFrame.setTexture(&textureChestArmourFrame);
	chestArmourFrame.setSize(Vector2f(75, 75));
	chestArmourFrame.setOrigin(chestArmourFrame.getSize() / 2.f);
	chestArmourFrame.setPosition(viewCentre.x - 300, 550);

	weaponFrame.setTexture(&textureWeaponFrame);
	weaponFrame.setSize(Vector2f(75, 75));
	weaponFrame.setOrigin(weaponFrame.getSize() / 2.f);
	weaponFrame.setPosition(viewCentre.x - 200, 550);

	wandFrame.setTexture(&textureWandFrame);
	wandFrame.setSize(Vector2f(75, 75));
	wandFrame.setOrigin(wandFrame.getSize() / 2.f);
	wandFrame.setPosition(viewCentre.x - 100, 550);

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
	killsText.setFillColor(Color::Black);
	textBounds = killsText.getLocalBounds();
	killsText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 310);

	// Gold text
	goldCountText.setFont(font);
	goldCountText.setCharacterSize(fontSize);
	goldCountText.setString("Gold: " + player.getGold());
	goldCountText.setFillColor(Color::Black);
	textBounds = goldCountText.getLocalBounds();
	goldCountText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 360);

	invHealthBar.setFillColor(Color::Red);
	invHealthBar.setPosition(viewCentre.x - 310, 825);

	backgroundInvHealthBar.setFillColor(Color::Black);
	backgroundInvHealthBar.setSize(Vector2f(200, 50));
	backgroundInvHealthBar.setPosition(viewCentre.x - 310, 825);

	// Display invHealthBar text
	invHealthBarText.setString("0 / 0");     // Set the initial text
	invHealthBarText.setFont(font);          // Assign the font
	invHealthBarText.setCharacterSize(fontSize - 5); // Slightly smaller text size
	invHealthBarText.setFillColor(Color::White);

	invStamBar.setFillColor(Color::Green);
	invStamBar.setPosition(viewCentre.x - 60, 825);

	backgroundInvStamBar.setFillColor(Color::Black);
	backgroundInvStamBar.setSize(Vector2f(200, 50));
	backgroundInvStamBar.setPosition(viewCentre.x - 60, 825);
	
	// Display invStamBar text
	invStamBarText.setString("0 / 0"); // Set the initial text
	invStamBarText.setFont(font); // Assign the font
	invStamBarText.setCharacterSize(fontSize - 5); // Slightly smaller text size
	invStamBarText.setFillColor(Color::White);

	invManaBar.setFillColor(Color::Magenta);
	invManaBar.setPosition(viewCentre.x + 190, 825);

	backgroundInvManaBar.setFillColor(Color::Black);
	backgroundInvManaBar.setSize(Vector2f(200, 50));
	backgroundInvManaBar.setPosition(viewCentre.x + 190, 825);

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
}

void Engine::initializeInventory()
{
	int startX = viewCentre.x - 300;
	int startY = 650;

	// create empty item r
	emptyFrames.resize(16);
	for (int i = 0; i < emptyFrames.size(); i++) {
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
	startX = viewCentre.x - 300;
	startY = 650;

	// Position icons for items that actually exist
	for (int i = 0; i < storedItems.size(); i++) {
		if (!storedItems[i].isNull()) {
			storedItems[i].getIcon().setPosition(emptyFrames[i].getPosition());
		}
	}
}

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


Engine::Difficulty Engine::stringToDifficulty(string str)
{
	if (str == "Easy") {return Difficulty::Easy; }
	else if (str == "Medium") {return Difficulty::Medium; }
	else if (str == "Hard") { return Difficulty::Hard; }
	else return Difficulty::Medium;
}


void Engine::moveDraggedIcon(Sprite& draggedIcon, Vector2f mousePos)
{
	float x = static_cast<float>(mousePos.x);
	float y = static_cast<float>(mousePos.y);
	draggedIcon.setPosition(x - 30, y - 30);
}

bool Engine::addItemToInventory(String itemType)
{
	int startX = viewCentre.x - 300;
	int startY = 650;
	
	for (int i = 0; i < storedItems.size(); i++)
	{
		if (i != 0 && i % 8 == 0) {
			startY += 100;
			startX = viewCentre.x - 300;
		}
		if (storedItems[i].isNull())
		{
			storedItems[i] = Item(itemType, Vector2f(startX, startY));
			return true;
		}
		startX += 100;
	}

	return false;
}

void Engine::populateChunkVector()
{
	for (int i = 0; i < world.getWorldSize(); i++) {
		chunks.push_back(*world.getChunk(i));
	}
}

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
	// The main game loop
	while (window.isOpen())
	{
		if (displayFps) {
			// Calculating fps
			float deltaTime = fpsClock.restart().asSeconds();
			fps = fps * 0.9f + (1.f / deltaTime) * 0.1f;
			fpsText.setString("FPS: " + to_string((int)fps));
		}

		initializeInventory();

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
				player.switchWeapon();
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
					player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume());
					player.loadSaveFile();

					equippedSwordIcon.setTextureRect(player.getEquippedWeapons().at(0).getTextureRect());
					equippedWandIcon.setTextureRect(player.getEquippedWeapons().at(1).getTextureRect());
					equippedHeadArmourIcon.setTextureRect(player.getEquippedArmour().at(0).getTextureRect());
					equippedChestArmourIcon.setTextureRect(player.getEquippedArmour().at(1).getTextureRect());
					equippedTrousersArmourIcon.setTextureRect(player.getEquippedArmour().at(2).getTextureRect());
					equippedShoeArmourIcon.setTextureRect(player.getEquippedArmour().at(3).getTextureRect());
					equippedNeckArmourIcon.setTextureRect(player.getEquippedArmour().at(4).getTextureRect());

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

					for (int i = 0; i < 0; ++i)
					{
						Enemy e;
						e.spawn(arena, resolution, tileSize, "Goblin", player.getPlayerLevel());
						enemyArr.push_back(e);
					}

					// Reset the clock so there isn't a frame jump
					clock.restart();

					player.loadConfigFile();
					difficulty = stringToDifficulty(player.getdifficultyString());
					windowedMode = player.getWindowedMode();
					displayFps = player.getDisplayFps();
					Listener::setGlobalVolume(player.getVolume());
					world.newWorld();
					populateChunkVector();
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
						equippedSwordIcon.setTextureRect(player.getEquippedWeapons().at(0).getTextureRect());
						equippedWandIcon.setTextureRect(player.getEquippedWeapons().at(1).getTextureRect());
						equippedHeadArmourIcon.setTextureRect(player.getEquippedArmour().at(0).getTextureRect());
						equippedChestArmourIcon.setTextureRect(player.getEquippedArmour().at(1).getTextureRect());
						equippedTrousersArmourIcon.setTextureRect(player.getEquippedArmour().at(2).getTextureRect());
						equippedShoeArmourIcon.setTextureRect(player.getEquippedArmour().at(3).getTextureRect());
						equippedNeckArmourIcon.setTextureRect(player.getEquippedArmour().at(4).getTextureRect());

						// We will modify the next two lines later
						arena.width = 1900;
						arena.height = 800;
						arena.left = 1664;
						arena.top = 1664;

						// Pass the vertex array by reference to the createBackground function
						int tileSize = 64;

						// Spawn the player in the middle of the arena
						player.spawn(arena, resolution, tileSize, player.getPlayerLevel());

						for (Enemy& enemies : enemyArr)
						{
							enemies.spawn(arena, resolution, tileSize, "Goblin", player.getPlayerLevel());
						}

						// Reset the clock so there isn't a frame jump
						clock.restart();

						player.loadConfigFile();
						difficulty = stringToDifficulty(player.getdifficultyString());
						windowedMode = player.getWindowedMode();
						displayFps = player.getDisplayFps();
						Listener::setGlobalVolume(player.getVolume());
						populateChunkVector();
					}
					else {
						// No save file so create a new one with default values and load it	
						player.createNewSave();
						player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume());
						player.loadSaveFile();

						equippedSwordIcon.setTextureRect(player.getEquippedWeapons().at(0).getTextureRect());
						equippedWandIcon.setTextureRect(player.getEquippedWeapons().at(1).getTextureRect());
						equippedHeadArmourIcon.setTextureRect(player.getEquippedArmour().at(0).getTextureRect());
						equippedChestArmourIcon.setTextureRect(player.getEquippedArmour().at(1).getTextureRect());
						equippedTrousersArmourIcon.setTextureRect(player.getEquippedArmour().at(2).getTextureRect());
						equippedShoeArmourIcon.setTextureRect(player.getEquippedArmour().at(3).getTextureRect());
						equippedNeckArmourIcon.setTextureRect(player.getEquippedArmour().at(4).getTextureRect());

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

						for (Enemy& enemies : enemyArr)
						{
							enemies.spawn(arena, resolution, tileSize, "Goblin", player.getPlayerLevel());
						}

						// Reset the clock so there isn't a frame jump
						clock.restart();

						player.loadConfigFile();
						difficulty = stringToDifficulty(player.getdifficultyString());
						windowedMode = player.getWindowedMode();
						displayFps = player.getDisplayFps();
						Listener::setGlobalVolume(player.getVolume());
						populateChunkVector();
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
					player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume());
					state = State::MAIN_MENU;
				}

				// Player hit the main menu button in the pause menu
				if (mainMenuButton.getGlobalBounds().contains(worldPos) && state == State::PAUSED && event.mouseButton.button == Mouse::Left)
				{
					sound.playButtonClickSound();
					world.clearWorld();
					player.updateSaveFile();
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

				if (event.key.code == Keyboard::Space)
				{
					if (state == State::STORY_INTRO && !skipAnimation) {
						storyIntroText.setString(fullText);
						skipAnimation = true;
					}
					else if (state == State::STORY_INTRO && skipAnimation) {
						sound.stopStoryIntroSound();
						state = State::PLAYING;
						displayedText = "";
						storyIntroText.setString(displayedText);
						currentChar = 0;
					}
				}

				if (state == State::PLAYING && event.key.code == Keyboard::Tab && tutorialStage != 1) {
					if (drawInventory) {
						drawInventory = false;
					}
					else {
						drawInventory = true;
					}

					if (tutorialStage == 0) {
						tutorialStage = 1;
						tutorialText.setString("Welcome to your inventory! Here you can manage your items and equipment. Drag the health potion onto the player to heal.");
						textBounds = tutorialText.getLocalBounds();
						tutorialText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 900);
					}
				}
			}
		} // End event polling

		// Handle controls while playing
		if (state == State::PLAYING && !drawInventory)
		{

			if (Mouse::isButtonPressed(Mouse::Left))
			{
				player.AttackAnimation("slash");
			}

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
		if you add any more, make sure they check if debug reset is false and set it to true or else it will run every loop while the key is pressed
		Press tilde to activate debug commands */
		if (event.key.code == Keyboard::Tilde && state == State::PLAYING)
		{
			debugreset = false;
			//	storedItems[2] = Weapon("Wooden_Wand", Vector2f(600, 650));

		}
		if (event.key.code == Keyboard::Num1 && !debugreset && state == State::PLAYING)
		{
			// Increase health
			player.upgradeHealth();
			//	storedItems[2] = Weapon("Iron_Sword", Vector2f(600, 650));


			debugreset = true;
		}

		if (event.key.code == Keyboard::Num2 && !debugreset && state == State::PLAYING)
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

		if (event.key.code == Keyboard::Num4 && !debugreset && state == State::PLAYING)
		{
			if (addItemToInventory("Iron_Sword"))
			{
				std::cout << "Item added to inventory" << std::endl;
			}
			else
			{
				std::cout << "No space in inventory" << std::endl;
			}
			debugreset = true;
		}

		if (event.key.code == Keyboard::Num5 && !debugreset && state == State::PLAYING)
		{

			//Selling item example, the storedItems index will need to be set somehow but the rest can be copy and pasted 
			// I can make a function for it if needed	
			if (storedItems[0].isNull())	cout << "null item attempted to be sold" << endl;

			player.setGold(player.getGold() + storedItems[0].getValue());
			cout << "Sold " << " for " << storedItems[0].getValue() << " gold." << endl;
			cout << "You now have " << player.getGold() << " gold." << endl;
			storedItems[0] = Item("null", Vector2f(0, 0));
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
				items.emplace_back("gold", Vector2f(0, 0));
			}
			debugreset = true;
		}

		if (event.key.code == Keyboard::C && !debugreset && state == State::PLAYING)
		{
			player.setInCell();
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
			}

			if (state == State::PLAYING && !drawInventory) {
				// Update the player
				player.update(dtAsSeconds, Mouse::getPosition(), world.getNavBoxes(player.getChunk()));

				// Update the vector of enemies if within player's render area
				for (Enemy& enemies : enemyArr)
				{
					if (player.getRenderArea().intersects(enemies.getSprite().getGlobalBounds()))
					{
						enemies.update(dtAsSeconds, player.getPosition(), getCurrentChunk(enemies.getPosition().x, enemies.getPosition().y));
					}
				}
			}

			filter.setOrigin(player.getPosition());
			filter.setPosition(player.getPosition().x, player.getPosition().y);

			// Make a note of the players new position
			Vector2f playerPosition(player.getCenter());

			// Make the view centre around the player				
			mainView.setCenter(player.getCenter().x, player.getCenter().y - 10);

			// Update any spells that are in-flight
			for (int i = 0; i < 100; i++)
			{
				if (spells[i].isInFlight())
				{
					spells[i].update(dtAsSeconds);
				}
			}

			// Drag items the player clicks on
			if (drawInventory)
			{
				bool draggingFromInventory = false;
				//int draggedIndex = -1;

				// Check clicks on inventory items
				for (int i = 0; i < storedItems.size(); i++)
				{
					if (!storedItems[i].isNull() && storedItems[i].getIcon().getGlobalBounds().contains(worldPos.x - 25, worldPos.y - 25) &&
						Mouse::isButtonPressed(Mouse::Left) && !draggingItem)
					{
						clickedItem = storedItems[i]; // copy to clickedItem
					
						itemLastIndex = i; // save original slot
						itemLastX = clickedItem.getIcon().getPosition().x;
						itemLastY = clickedItem.getIcon().getPosition().y;

						storedItems[i] = Item("null", Vector2f(0, 0)); // empty original slot
						draggingItem = true;
						itemPlaced = false;
						draggingFromInventory = true;
						draggedIndex = i;
						break;
					}
				}

				// Check clicks on world items
				for (int i = 0; i < allItems.size(); i++)
				{
					if (allItems[i].getIcon().getGlobalBounds().contains(worldPos.x - 25, worldPos.y - 25) &&
						Mouse::isButtonPressed(Mouse::Left) && !draggingItem)
					{
						clickedItem = allItems[i]; // copy to clickedItem
						
						itemLastIndex = -1; // world item
						itemLastX = clickedItem.getIcon().getPosition().x;
						itemLastY = clickedItem.getIcon().getPosition().y;

						draggingItem = true;
						itemPlaced = false;
						draggingFromInventory = false;
						draggedIndex = i;
						break;
					}
				}

				// Move the dragged item
				if (draggingItem)
					moveDraggedIcon(clickedItem.getIcon(), Vector2f(worldPos));

				// Drop the item
				if (draggingItem && !Mouse::isButtonPressed(Mouse::Left))
				{
					bool placed = false;

					// Try to drop in the first empty slot
					for (int i = 0; i < storedItems.size(); i++)
					{
						if (storedItems[i].isNull() &&
							clickedItem.getIcon().getGlobalBounds().intersects(emptyFrames[i].getGlobalBounds()))
						{
							storedItems[i] = clickedItem;                  // copy item into slot
							clickedItem.getIcon().setPosition(emptyFrames[i].getPosition());
							placed = true;
							break;
						}
					}

					// Try to equip as sword if dropped on sword slot
					if (clickedItem.getIcon().getGlobalBounds().intersects(weaponFrame.getGlobalBounds())
						&& clickedItem.getType() == Item::MeleeWeapon)
					{	if (player.equipWeapon(clickedItem.getName())) equippedSwordIcon.setTextureRect(clickedItem.getTextureRect());	}

							// placed is not being made true on purpose, the player will still need to carry the items they equip
							// we can change this later though

					// Try to equip as wand if dropped on wand slot
					if (clickedItem.getIcon().getGlobalBounds().intersects(wandFrame.getGlobalBounds()) 
						&& clickedItem.getType() == Item::MagicWeapon)
					{	if (player.equipWeapon(clickedItem.getName())) equippedWandIcon.setTextureRect(clickedItem.getTextureRect()); }

					// Try to equip as head armour if dropped on head armour slot
					if (clickedItem.getIcon().getGlobalBounds().intersects(headArmourFrame.getGlobalBounds()) 
						&& clickedItem.getType() == Item::HeadArmour)
					{ if (player.equipArmour(clickedItem.getName())) equippedHeadArmourIcon.setTextureRect(clickedItem.getTextureRect()); }

					// Try to equip as chest armour if dropped on chest armour slot
					if (clickedItem.getIcon().getGlobalBounds().intersects(chestArmourFrame.getGlobalBounds())
						&& clickedItem.getType() == Item::ChestArmour)
					{	if (player.equipArmour(clickedItem.getName())) equippedChestArmourIcon.setTextureRect(clickedItem.getTextureRect()); }

					// Try to equip as trouser armour if dropped on trouser armour slot
					if (clickedItem.getIcon().getGlobalBounds().intersects(trousersArmourFrame.getGlobalBounds()) 
						&& clickedItem.getType() == Item::TrouserArmour)
					{	if (player.equipArmour(clickedItem.getName())) equippedTrousersArmourIcon.setTextureRect(clickedItem.getTextureRect()); }

					// Try to equip as shoe armour if dropped on shoe armour slot
					if (clickedItem.getIcon().getGlobalBounds().intersects(bootsArmourFrame.getGlobalBounds()) 
						&& clickedItem.getType() == Item::ShoeArmour)
					{	if (player.equipArmour(clickedItem.getName())) equippedShoeArmourIcon.setTextureRect(clickedItem.getTextureRect());}

					if (clickedItem.getIcon().getGlobalBounds().intersects(playerInFrame.getGlobalBounds())
						&& clickedItem.getType() == Item::Consumable)
					{
						cout << "Item used!";
						if (clickedItem.getName() == "Health_Potion")
						{
							player.healHealth(50); // heal 50 health
						}
						else if (clickedItem.getName() == "Mana_Potion")
						{
							player.healMana(50); // restore 50 mana
						}
						else if (clickedItem.getName() == "Stamina_Potion")
						{
							player.healStamina(50); // restore 50 stamina
						}
						clickedItem = Item("null", Vector2f(0, 0));
						placed = true;

						tutorialStage = 2;
					}

					// If no slot found, return to original position
					if (!placed)
					{
						clickedItem.getIcon().setPosition(itemLastX, itemLastY);
						if (draggingItem && draggedIndex >= 0 && draggedIndex < storedItems.size())
							storedItems[draggedIndex] = clickedItem; // restore in original slot
						placed = true;

					}

					draggingItem = false;
				}

			
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
				ssHealthBar << int(player.getHealth()) << " / " << int(player.getMaxHealth());
				invHealthBarText.setString(ssHealthBar.str());
				textBounds = invHealthBarText.getLocalBounds();
				x = backgroundInvHealthBar.getPosition().x + (backgroundInvHealthBar.getSize().x / 2.f) - (textBounds.width / 2.f);
				y = backgroundInvHealthBar.getPosition().y + (backgroundInvHealthBar.getSize().y / 2.f) - (textBounds.height / 2.f);
				invHealthBarText.setPosition(x - textBounds.left, y - textBounds.top);

				stringstream ssStamBar;
				ssStamBar << int(player.getStamina()) << " / " << int(player.getMaxStamina());
				invStamBarText.setString(ssStamBar.str());
				textBounds = invStamBarText.getLocalBounds();
				x = backgroundInvStamBar.getPosition().x + (backgroundInvStamBar.getSize().x / 2.f) - (textBounds.width / 2.f);
				y = backgroundInvStamBar.getPosition().y + (backgroundInvStamBar.getSize().y / 2.f) - (textBounds.height / 2.f);
				invStamBarText.setPosition(x - textBounds.left, y - textBounds.top);

				stringstream ssManaBar;
				ssManaBar << int(player.getMana()) << " / " << int(player.getMaxMana());
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

			size_t i = 0;
			while (i < items.size())
			{
				items[i].update(dtAsSeconds);

				if (player.getGlobalBounds().intersects(items[i].getPosition()))
				{
					bool placed = false;
					for (size_t j = 0; j < storedItems.size(); j++)
					{
						if (storedItems[j].isNull())
						{
							storedItems[j] = move(items[i]);
							storedItems[j].getIcon().setPosition(emptyFrames[j].getPosition());
							currentItems++;
							items.erase(items.begin() + i);
							placed = true;
							break;
						}
					}
					if (!placed) ++i;
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
		draw();
	} // End of main game loop
}

