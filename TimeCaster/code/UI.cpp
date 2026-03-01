#include "Engine.h"
#include <iomanip>
#include "TextHelper.hpp"

using namespace sf;
using namespace std;

void Engine::initializeUI()
{
	FloatRect viewRect(0, 0, resolution.x, resolution.y);
	mainView.reset(viewRect);
	// Optional: set the center to the middle of the view
	mainView.setCenter(resolution.x / 2.f, resolution.y / 2.f);

	// Zoom view
	mainView.zoom(0.3f);

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

	// Main font
	font.loadFromFile("fonts/Jersey10-Regular.ttf");

	textBounds = pausedText.getLocalBounds();
	viewCentre = mainView.getCenter();

	// Paused text
	pausedText = TextHelper::makeText(font, "Press escape \nto continue", 160, Vector2f(viewCentre.x + 50, viewCentre.y - 150), Color::White);

	// FPS text
	fpsText = TextHelper::makeText(font, "FPS:", fontSize - 5, Vector2f(1800, 5), Color::Yellow);

	barContainer = TextureHolder::GetTexture("graphics/UI/barContainer.png");

	// Health bar
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(25, 10);

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
	shopItems.resize(12, Item("null", Vector2f(0, 0)));
	// Debug shop item initialization
	shopItems[0] = Weapon("Silver_Wand", Vector2f(300, 650));
	shopItems[1] = Weapon("Pirate's_Scimitar", Vector2f(450, 650));
	shopItems[2] = Equipment("Leather_Cap", Vector2f(600, 650));
	shopItems[3] = Equipment("Leather_Chestplate", Vector2f(750, 950));
	shopItems[4] = Equipment("Leather_Leggings", Vector2f(900, 650));
	shopItems[5] = Equipment("Leather_Boots", Vector2f(1050, 650));
	shopItems[9] = Item("Mana_Potion", Vector2f(1200, 650));
	shopItems[10] = Item("Health_Potion", Vector2f(1200, 650));
	shopItems[11] = Item("Stamina_Potion", Vector2f(1350, 650));


	//Item 0 is sword (melee combat), item 1 is wand (magic combat)
	m_EquippedWeapons.resize(2, Weapon("null", Vector2f(0, 0)));

	// Item 0 is head, item 1 is torso, item 2 is pants, item 3 is shoes, item 4 is neck
	m_EquippedArmour.resize(5, Equipment("null", Vector2f(0, 0)));

	/***********
	Main Menu UI
	************/
	textureMainMenuButton1 = TextureHolder::GetTexture("graphics/UI/menuButton1.png");
	textureMainMenuButton2 = TextureHolder::GetTexture("graphics/UI/menuButton2.png");

	// TimeCaster heading text
	mainHeadingText = TextHelper::makeText(font, "TimeCaster", fontSize + 65, Vector2f(viewCentre.x, 100), Color::White);

	// New game button
	newGameButton.setPosition(200, 210);
	newGameButton.setSize(Vector2f(300, 80));
	newGameButton.setTexture(&textureMainMenuButton1);

	// New game button text
	newGameButtonText = TextHelper::makeText(font, "New Game", fontSize, Vector2f(newGameButton.getPosition().x + (newGameButton.getSize().x / 2.f), newGameButton.getPosition().y + (newGameButton.getSize().y / 2.f) - 15), Color::Yellow);

	// Load game button
	loadGameButton.setPosition(200, 320);
	loadGameButton.setSize(Vector2f(300, 80));
	loadGameButton.setTexture(&textureMainMenuButton2);

	// Load game button text
	loadGameButtonText = TextHelper::makeText(font, "Load Game", fontSize, Vector2f(loadGameButton.getPosition().x + (loadGameButton.getSize().x / 2.f), loadGameButton.getPosition().y + (loadGameButton.getSize().y / 2.f) - 10), Color::White);

	// Game over button
	gameOverMainMenuButton.setPosition(viewCentre.x - 400, 900);
	gameOverMainMenuButton.setSize(Vector2f(300, 80));
	gameOverMainMenuButton.setTexture(&textureMainMenuButton2);

	// Game over button text
	gameOverMainMenuButtonText = TextHelper::makeText(font, "Main Menu", fontSize, Vector2f(gameOverMainMenuButton.getPosition().x + (gameOverMainMenuButton.getSize().x / 2.f), gameOverMainMenuButton.getPosition().y + (gameOverMainMenuButton.getSize().y / 2.f) - 10), Color::White);

	// Game over quit button
	gameOverQuitButton.setPosition(viewCentre.x, 900);
	gameOverQuitButton.setSize(Vector2f(300, 80));
	gameOverQuitButton.setTexture(&textureMainMenuButton2);

	// Game over quit button text
	gameOverQuitButtonText = TextHelper::makeText(font, "Quit Game", fontSize, Vector2f(gameOverQuitButton.getPosition().x + (gameOverQuitButton.getSize().x / 2.f), gameOverQuitButton.getPosition().y + (gameOverQuitButton.getSize().y / 2.f) - 10), Color::White);

	// Controls button
	controlsButton.setPosition(200, 430);
	controlsButton.setSize(Vector2f(300, 80));
	controlsButton.setTexture(&textureMainMenuButton2);

	// Controls button text
	controlsButtonText = TextHelper::makeText(font, "Controls", fontSize, Vector2f(controlsButton.getPosition().x + (controlsButton.getSize().x / 2.f), controlsButton.getPosition().y + (controlsButton.getSize().y / 2.f) - 10), Color::White);

	// Options button
	optionsButton.setPosition(200, 540);
	optionsButton.setSize(Vector2f(300, 80));
	optionsButton.setTexture(&textureMainMenuButton2);

	// options button text
	optionsButtonText = TextHelper::makeText(font, "Options", fontSize, Vector2f(optionsButton.getPosition().x + (optionsButton.getSize().x / 2.f), optionsButton.getPosition().y + (optionsButton.getSize().y / 2.f) - 10), Color::White);

	// Quit game button
	quitGameButton.setPosition(200, 950);
	quitGameButton.setSize(Vector2f(300, 80));
	quitGameButton.setTexture(&textureMainMenuButton2);

	// Quit game button text
	quitGameButtonText = TextHelper::makeText(font, "Quit Game", fontSize, Vector2f(quitGameButton.getPosition().x + (quitGameButton.getSize().x / 2.f), quitGameButton.getPosition().y + (quitGameButton.getSize().y / 2.f) - 10), Color::White);

	// Credits button
	creditsButton.setPosition(200, 650);
	creditsButton.setSize(Vector2f(300, 80));
	creditsButton.setTexture(&textureMainMenuButton2);

	// Quit game button text
	creditsButtonText = TextHelper::makeText(font, "Credits", fontSize, Vector2f(creditsButton.getPosition().x + (creditsButton.getSize().x / 2.f), creditsButton.getPosition().y + (creditsButton.getSize().y / 2.f) - 10), Color::White);

	// Credits text
	developersText = TextHelper::makeText(font, "Developers:\nJoshua Muller\nToby Womack\nJake O'Sullivan", fontSize, Vector2f(viewCentre.x, viewCentre.y - 200), Color::White);
	creditsText = TextHelper::makeText(font, "Credits for assets used are located\nin the game files and documentation", fontSize, Vector2f(viewCentre.x, viewCentre.y), Color::White);

	textBounds = creditsText.getLocalBounds();
	creditsText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
	creditsText.setPosition(viewCentre);

	creditsHeadingText = TextHelper::makeText(font, "Credits", fontSize + 15, Vector2f(viewCentre.x, 50), Color::White);

	/*************************
	Controls & options Menu UI
	**************************/

	// Controls Text
	controlsText = TextHelper::makeText(font, "Controls:\nWASD: Movement \nTAB: Open inventory\nE: Interact\nLeft click: Attack\nRight click: Block\nShift: Sprint\n1-4: Select spell\nF / middle mouse button: Switch combat types\nScroll wheel: Zoom in and out\nEsc: Pause", fontSize, Vector2f(viewCentre.x, viewCentre.y), Color::White);

	textBounds = controlsText.getLocalBounds();
	controlsText.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
	controlsText.setPosition(viewCentre);
	
	// Controls heading text
	controlsHeadingText = TextHelper::makeText(font, "Controls", fontSize + 15, Vector2f(viewCentre.x, 50), Color::White);

	// Options heading text
	optionsHeadingText = TextHelper::makeText(font, "Options", fontSize + 15, Vector2f(viewCentre.x, 50), Color::White);

	// Main menu button
	mainMenuButton.setSize(Vector2f(300, 80));
	mainMenuButton.setPosition(viewCentre.x - 150, 975);
	mainMenuButton.setTexture(&textureMainMenuButton2);

	// Main menu button text
	mainMenuButtonText = TextHelper::makeText(font, "Save & Exit", fontSize, Vector2f(mainMenuButton.getPosition().x + (mainMenuButton.getSize().x / 2.f), mainMenuButton.getPosition().y + (mainMenuButton.getSize().y / 2.f) - 10), Color::White);
	backButtonText = TextHelper::makeText(font, "Back", fontSize, Vector2f(mainMenuButton.getPosition().x + (mainMenuButton.getSize().x / 2.f), mainMenuButton.getPosition().y + (mainMenuButton.getSize().y / 2.f) - 10), Color::White);

	// Volume slider text
	volumeSliderText = TextHelper::makeText(font, "Volume", fontSize, Vector2f(viewCentre.x, 190), Color::White);

	// Slider track
	track.setSize(Vector2f(400, 5));
	track.setFillColor(sf::Color::Black);
	textBounds = track.getLocalBounds();
	int trackY = 250;
	track.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, trackY);

	// Slider handle
	handle.setRadius(10);
	handle.setPointCount(10);
	handle.setFillColor(Color::Red);
	handle.setOrigin(10, 10); // Centre the circle
	textBounds = handle.getLocalBounds();

	// Determine initial volume: load from config if present, otherwise default to 50%
	float initialVolume = 50.f;
	if (player.loadConfigFile())
	{
		initialVolume = player.getVolume();
		if (initialVolume < 0.f) initialVolume = 0.f;
		if (initialVolume > 100.f) initialVolume = 100.f;
	}

	// Apply global volume
	Listener::setGlobalVolume(initialVolume);

	// Position handle along the track based on initialVolume (0-100)
	float handleCenterX = track.getPosition().x + track.getSize().x * (initialVolume / 100.0f);
	float handleX = handleCenterX - (textBounds.width / 2.f) - textBounds.left;
	handle.setPosition(handleX, trackY + 2); // slider handle vertically aligned with track

	// Display FPS button
	if (displayFps) {
		displayFPSButton.setFillColor(Color::Green);
	}
	else {
		displayFPSButton.setFillColor(Color::Red);
	}
	displayFPSButton.setSize(Vector2f(300, 80));
	textBounds = displayFPSButton.getLocalBounds();
	displayFPSButton.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 300);
	displayFPSButton.setTexture(&textureMainMenuButton2);

	// Display FPS button text
	displayFPSButtonText = TextHelper::makeText(font, "Display FPS", fontSize - 5, Vector2f(displayFPSButton.getPosition().x + (displayFPSButton.getSize().x / 2.f), displayFPSButton.getPosition().y + (displayFPSButton.getSize().y / 2.f) - 10), Color::Black);

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
	windowedModeButton.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 500);
	windowedModeButton.setTexture(&textureMainMenuButton2);

	// Windowed mode button text
	windowedModeButtonText = TextHelper::makeText(font, "Windowed Mode", fontSize - 5, Vector2f(windowedModeButton.getPosition().x + (windowedModeButton.getSize().x / 2.f), windowedModeButton.getPosition().y + (windowedModeButton.getSize().y / 2.f) - 10), Color::Black);

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
	vSyncButton.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 600);
	vSyncButton.setTexture(&textureMainMenuButton2);

	// VSync buttion text
	vSyncButtonText = TextHelper::makeText(font, "Vsync", fontSize - 5, Vector2f(vSyncButton.getPosition().x + (vSyncButton.getSize().x / 2.f), vSyncButton.getPosition().y + (vSyncButton.getSize().y / 2.f) - 10), Color::Black);

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
	debugModeButton.setPosition(viewCentre.x * 1.9 - (textBounds.width / 2.f) - textBounds.left, viewCentre.y * 1.8);
	debugModeButton.setTexture(&textureMainMenuButton2);

	// debugMode button text
	debugModeButtonText = TextHelper::makeText(font, "Debug Mode", fontSize - 5, Vector2f(debugModeButton.getPosition().x + (debugModeButton.getSize().x / 2.f), debugModeButton.getPosition().y + (debugModeButton.getSize().y / 2.f) - 10), Color::Black);

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
	difficultyButton.setSize(Vector2f(350, 100));
	textBounds = difficultyButton.getLocalBounds();
	difficultyButton.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 400);
	difficultyButton.setTexture(&textureMainMenuButton2);

	// Display difficulty button text
	difficultyButtonText = TextHelper::makeText(font, "Difficulty: " + difficultyToString(difficulty), fontSize - 5, Vector2f(difficultyButton.getPosition().x + (difficultyButton.getSize().x / 2.f), difficultyButton.getPosition().y + (difficultyButton.getSize().y / 2.f) - 10), Color::Black);

	// Story into text
	storyIntroText = TextHelper::makeText(font, " ", fontSize + 10, Vector2f(150, 200), Color::White);

	fullText = "I was not always a man consumed by vengeance. Once, I had a family-warm laughter by the fire, \n"
		"the gentle touch of my children's hands, the steady love of my wife. \n"
		"All of it was torn from me in a single night, \n"
		"devoured by the fire of Ignis, a dragon whose name still burns in my mind. \n"
		"Since then, every spell I've mastered, every scar I've earned, has been for one purpose alone: \n"
		"to bring that beast to its knees. \n"
		"I do not seek glory, nor the hollow praise of men-I seek redemption. \n"
		"And when the dragon falls, so too shall the weight of my failure. \n\n"
		"You desire retribution dear player, but what will it cost you?";

	// Skip intro text
	skipIntroText = TextHelper::makeText(font, " ", fontSize - 5, Vector2f(0, 0), Color::White);
	// Loading text
	loadWorldText = TextHelper::makeText(font, " ", fontSize + 100, Vector2f(0, 0), Color::White);
	// Game over text
	gameOverText = TextHelper::makeText(font, " ", fontSize + 100, Vector2f(0, 0), Color::Red);
	// Game over text
	gameOverText2 = TextHelper::makeText(font, " ", fontSize + 20, Vector2f(0, 0), Color::White);
	// Stat text (for victory screen)
	statText = TextHelper::makeText(font, " ", fontSize + 20, Vector2f(0, 0), Color::White);

	/***********
	Inventory UI
	************/
	textureHeadArmourFrame = TextureHolder::GetTexture("graphics/UI/headFrame.png");
	textureChestArmourFrame = TextureHolder::GetTexture("graphics/UI/chestFrame.png");
	textureTrousersArmourFrame = TextureHolder::GetTexture("graphics/UI/trousersFrame.png");
	textureBootsArmourFrame = TextureHolder::GetTexture("graphics/UI/bootsFrame.png");
	textureWeaponFrame = TextureHolder::GetTexture("graphics/UI/weaponFrame.png");
	textureEmptyFrame = TextureHolder::GetTexture("graphics/UI/emptyFrame.png");
	textureNeckFrame = TextureHolder::GetTexture("graphics/UI/neckFrame.png");
	texturePlayerFrame = TextureHolder::GetTexture("graphics/UI/playerFrame.png");
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

	equippedHeadArmourIcon.setPosition(viewCentre.x - 150, 300);
	equippedChestArmourIcon.setPosition(viewCentre.x - 350, 500);
	equippedTrousersArmourIcon.setPosition(viewCentre.x - 350, 400);
	equippedShoeArmourIcon.setPosition(viewCentre.x - 150, 400);
	equippedNeckArmourIcon.setPosition(viewCentre.x - 350, 300);

	// Player frame
	playerFrame.setSize(sf::Vector2f(100.f, 200.f));
	playerFrame.setTexture(&texturePlayerFrame);
	playerFrame.setOrigin(playerFrame.getSize() / 2.f);
	playerFrame.setPosition(viewCentre.x - 250, 350);

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

	itemTooltipName = TextHelper::makeText(font, " ", fontSize - 8, Vector2f(0, 0), Color::White);
	statTooltipText = TextHelper::makeText(font, " ", fontSize - 12, Vector2f(0, 0), Color::White);
	valueTooltipText = TextHelper::makeText(font, " ", fontSize - 12, Vector2f(0, 0), Color::White);

	// Display kill count inventory text
	killsText = TextHelper::makeText(font, "Kills: " + to_string(player.getKillCount()), fontSize, Vector2f(viewCentre.x - 50, 265), Color::White);

	// Gold text
	goldCountText = TextHelper::makeText(font, "Gold: " + player.getGold(), fontSize, Vector2f(viewCentre.x - 50, 315), Color::White);

	// Exp text
	expText = TextHelper::makeText(font, "EXP: " + to_string(player.getPlayerLevel()) + " / " + "100", fontSize, Vector2f(viewCentre.x - 20, 365), Color::White);

	// Level text
	levelsText = TextHelper::makeText(font, "Level: " + to_string(player.getPlayerLevel()), fontSize, Vector2f(viewCentre.x - 40, 415), Color::White);

	invHealthBar.setFillColor(Color::Red);
	invHealthBar.setPosition(viewCentre.x - 360, 775);

	backgroundInvHealthBar.setFillColor(Color::Black);
	backgroundInvHealthBar.setSize(Vector2f(200, 50));
	backgroundInvHealthBar.setPosition(viewCentre.x - 360, 775);

	// Display invHealthBar text
	invHealthBarText = TextHelper::makeText(font, "0 / 0", fontSize - 5, Vector2f(20, 20), Color::White);

	invStamBar.setFillColor(Color::Green);
	invStamBar.setPosition(viewCentre.x - 110, 775);

	backgroundInvStamBar.setFillColor(Color::Black);
	backgroundInvStamBar.setSize(Vector2f(200, 50));
	backgroundInvStamBar.setPosition(viewCentre.x - 110, 775);

	// Display invStamBar text
	invStamBarText = TextHelper::makeText(font, "0 / 0", fontSize - 5, Vector2f(20, 20), Color::White);

	invManaBar.setFillColor(Color::Magenta);
	invManaBar.setPosition(viewCentre.x + 140, 775);

	backgroundInvManaBar.setFillColor(Color::Black);
	backgroundInvManaBar.setSize(Vector2f(200, 50));
	backgroundInvManaBar.setPosition(viewCentre.x + 140, 775);

	// Display invManaBar text
	invManaBarText = TextHelper::makeText(font, "0 / 0", fontSize - 5, Vector2f(20, 20), Color::White);

	// Populate soundtrack
	sound.populateSoundtrack();

	eKey.setTexture(&eKeyTexture);
	eKey.setSize(Vector2f(50, 50));
	eKey.setPosition(viewCentre.x - 25, viewCentre.y - 100);

	tutorialText = TextHelper::makeText(font, "You sustained some damage while fleeing from the dragon. Press Tab to open your inventory and heal", fontSize, Vector2f(viewCentre.x, 900), Color::White);

	levelUpText = TextHelper::makeText(font, "Level up! Click on the bar you wish to upgrade.", fontSize, Vector2f(viewCentre.x, 950), Color::Green);

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
	textBox.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 800);
	textBox.setFillColor(sf::Color(40, 40, 40));
	textBox.setOutlineThickness(2.f);
	textBox.setOutlineColor(Color::White);

	// Text that shows the number the user types
	userInputText = TextHelper::makeText(font, "", fontSize - 11, Vector2f(textBox.getPosition().x + 10, textBox.getPosition().y + 10), Color::White);

	// Feedback message
	feedback = TextHelper::makeText(font, "Enter a number whose square is odd (e.g. 5 or 7)\nHit enter key when done", fontSize - 15, Vector2f(viewCentre.x, 865), Color::Yellow);

	// Feedback message
	feedbackFps = TextHelper::makeText(font, "Enter what you'd like to limit the FPS to.", fontSize - 15, Vector2f(viewCentre.x, 678), Color::Yellow);

	// Text box setup
	textBoxFps.setSize({ 300, 50 });
	textBounds = textBoxFps.getLocalBounds();
	textBoxFps.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 700);
	textBoxFps.setFillColor(sf::Color(40, 40, 40));
	textBoxFps.setOutlineThickness(2.f);
	textBoxFps.setOutlineColor(Color::White);

	// Text that shows the number the user types
	userInputTextFps = TextHelper::makeText(font, "", fontSize - 11, Vector2f(textBoxFps.getPosition().x + 10, textBoxFps.getPosition().y + 10), Color::White);

	// Is the player currently typing in the text box?
	textBoxActive = false;

	textBoxActiveFps = false;
}