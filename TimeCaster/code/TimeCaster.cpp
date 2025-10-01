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

int main()
{	
	CollisionDetection collision;

	// Here is the instance of TextureHolder
	TextureHolder holder;
	
	// Start with the MAIN_MENU state
	State state = State::MAIN_MENU;

	// Start with the Medium difficulty state
	Difficulty difficulty = Difficulty::Medium;

	// Get the screen resolution and create an SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	resolution.x = 1920;
	resolution.y = 1080;

	RenderWindow window(VideoMode(resolution.x, resolution.y), "TimeCaster", Style::Fullscreen);

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
	Player player;

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
	bool displayFps = false;
	
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

	// About the game
	
	int hiScore = 0;

	// For the home/game over screen
	Sprite spriteMainMenu;
	Texture textureMainMenu = TextureHolder::GetTexture("graphics/Castle (edited).jpg");
	spriteMainMenu.setTexture(textureMainMenu);
	spriteMainMenu.setPosition(0, 0);

	// Create a view for the HUD
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Main font
	Font font;
	font.loadFromFile("fonts/PixelifySans-Bold.ttf");

	// Load the high score from a text file
	ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> hiScore;
		inputFile.close();
	}

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
	Text goldCountText;
	goldCountText.setFont(font);
	goldCountText.setCharacterSize(55);
	goldCountText.setFillColor(Color::White);
	goldCountText.setPosition(1400, 0);

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
	Texture& textureMainMenuButton1 = TextureHolder::GetTexture("graphics/menuButton1.png");
	Texture& textureMainMenuButton2 = TextureHolder::GetTexture("graphics/menuButton2.png");
	int fontSize = 35;

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

	/***********
	Options Menu UI
	************/
	
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
	Text mainMenuButtonText("Main Menu", font, fontSize);
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
	Text storyIntroText("You desire retribution, but at what cost?", font, 40);
	storyIntroText.setFillColor(Color::White);
	textBounds = storyIntroText.getLocalBounds();
	viewCentre = mainView.getCenter();
	storyIntroText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 150);

	// Skip intro text
	Text skipIntroText("-- Space to skip --", font, 30);
	skipIntroText.setFillColor(Color::White);
	textBounds = skipIntroText.getLocalBounds();
	viewCentre = mainView.getCenter();
	skipIntroText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 1000);

	// When did we last update the HUD?
	int framesSinceLastHUDUpdate = 0;

	// What time was the last update
	Time timeSinceLastUpdate;

	// How often (in frames) should we update the HUD
	int fpsMeasurementFrameInterval = 1000;

	// struct to store y values and sprites together and vector to store each object
	struct DrawableItem {
		float y;
		sf::Sprite sprite;

		DrawableItem(float y, const sf::Sprite& sprite)
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

	// Setting volume to 50 by default
	Listener::setGlobalVolume(50);

	// Populate soundtrack
	sound.populateSoundtrack();

	// The main game loop
	while (window.isOpen())
	{
		// Calculating fps
		float deltaTime = fpsClock.restart().asSeconds();
		fps = 1.f / deltaTime;
		fpsText.setString("FPS: " + to_string((int)fps));
		
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
				if (event.key.code == Keyboard::Escape && state == State::PLAYING)
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
				else if (newGameButton.getGlobalBounds().contains(worldPos) && state == State::MAIN_MENU)
				{
					state = State::STORY_INTRO;
					
					// Play the start game sound
					if (!startSoundPlayed) {
						sound.playStartGameSound();
					}

					startSoundPlayed = true;
					
					player.createNewSave();
					player.loadSaveFile();
					world.newWorld();
				}

				// Player hit the load game button in the main menu
				else if (loadGameButton.getGlobalBounds().contains(worldPos) && state == State::MAIN_MENU)
				{
					state = State::PLAYING;
					world.newWorld(); // should be replaced with a loadWorld() function

					// Play the start game sound
					if (!startSoundPlayed) {
						sound.playStartGameSound();
					}

					startSoundPlayed = true;

					// Loads player stats from text file
					if (player.loadSaveFile() == true) {
						// Player loaded successfully
					}
					else {
						// No save file so create a new one with default values and load it	
						player.createNewSave();
						player.loadSaveFile();
					}
				}

				// Player hit the options button
				if (optionsButton.getGlobalBounds().contains(worldPos) && state == State::MAIN_MENU)
				{
					sound.playButtonClickSound();
					world.clearWorld();
					state = State::OPTIONS_MENU;
				}

				// Player hit the quit game button
				if (quitGameButton.getGlobalBounds().contains(worldPos) && state == State::MAIN_MENU)
				{
					sound.playButtonClickSound();
					// Save info to file before quitting
					window.close();
				}

				// Player hit the main menu button in the options menu
				if (mainMenuButton.getGlobalBounds().contains(worldPos) && state == State::OPTIONS_MENU)
				{
					sound.playButtonClickSound();
					world.clearWorld();
					state = State::MAIN_MENU;
				}

				// Player hit the main menu button in the pause menu
				if (mainMenuButton.getGlobalBounds().contains(worldPos) && state == State::PAUSED) 
				{
					sound.playButtonClickSound();
					world.clearWorld();
					player.updateSaveFile(player.getSpeed(), player.getHealth(), player.getMaxHealth(), player.getStamina(), player.getMaxStamina(), player.getStaminaRecharge(), player.getMana(), player.getMaxMana(), player.getGold(), player.getPosition());
					state = State::MAIN_MENU;
				}

				// Player hit the display fps button
				if (displayFPSButton.getGlobalBounds().contains(worldPos) && state == State::OPTIONS_MENU)
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

				// Player hit the difficulty button
				if (difficultyButton.getGlobalBounds().contains(worldPos) && state == State::OPTIONS_MENU)
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
					sound.playButtonClickSound();
				}
				
				if (state == State::STORY_INTRO && event.key.code == Keyboard::Space)
				{
						state = State::PLAYING;
				}
			}
		} // End event polling

		// Handle controls while playing
		if (state == State::PLAYING)
		{
			// Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
				for (int i = 0; i < world.getWorldSize(); i++)
				{
					for (auto& nav : world.getNavBoxes(i)) { // if player walks into navBox 
						if (collision.pointInShape(player.getPosition(), nav.getShape())) {
							player.revertPosition();
							player.setPosition(Vector2f(player.getPosition().x, player.getPosition().y + 1.5f));
						}
					}
				}
			}
			else
			{
				player.stopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
				for (int i = 0; i < world.getWorldSize(); i++)
				{
					for (auto& nav : world.getNavBoxes(i)) { // if player walks into navBox 
						if (collision.pointInShape(player.getPosition(), nav.getShape())) {
							player.revertPosition();
							player.setPosition(Vector2f(player.getPosition().x, player.getPosition().y - 1.5f));
						}
					}
				}
			}
			else
			{
				player.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
				for (int i = 0; i < world.getWorldSize(); i++)
				{
					for (auto& nav : world.getNavBoxes(i)) { // if player walks into navBox 
						if (collision.pointInShape(player.getPosition(), nav.getShape())) {
							player.revertPosition();
							player.setPosition(Vector2f(player.getPosition().x + 1.5f, player.getPosition().y));
						}
					}
				}
			}
			else
			{
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
				for (int i = 0; i < world.getWorldSize(); i++)
				{
					for (auto& nav : world.getNavBoxes(i)) { // if player walks into navBox 
						if (collision.pointInShape(player.getPosition(), nav.getShape())) {
							player.revertPosition();
							player.setPosition(Vector2f(player.getPosition().x - 1.5f, player.getPosition().y));
						}
					}
				}
			}
			else
			{
				player.stopRight();
			}
		} // End WASD while playing

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

		// Handle the levelling up state
		if (state == State::LEVELING_UP)
		{
			// Handle the player levelling up
			if (event.key.code == Keyboard::Num1)
			{
				// Increase fire rate
				//fireRate++;
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2)
			{
				// Increase clip size
				//clipSize += clipSize;
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num3)
			{
				// Increase health
				player.upgradeHealth();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num4)
			{
				// Increase speed
				player.upgradeSpeed();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num5)
			{
				healthPickup.upgrade();
				state = State::PLAYING;
			}
			
			if (event.key.code == Keyboard::Num6)
			{
				ammoPickup.upgrade();
				state = State::PLAYING;
			}
			
			if (event.key.code == Keyboard::Num7)
			{
				// Increase stamina
				player.upgradeStamina();
				state = State::PLAYING;
			}

			// Handle player and pickups spawning alongside terrain generation
			if (state == State::PLAYING)
			{
				// We will modify the next two lines later
				arena.width = 1900;
				arena.height = 800;
				arena.left = 1664;
				arena.top = 1664;

				// Pass the vertex array by reference 
				// to the createBackground function
				int tileSize = 64;

				// Spawn the player in the middle of the arena
				player.spawn(arena, resolution, tileSize);

				// Configure the pick-ups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);
				staminaPickup.setArena(arena);
				manaPickup.setArena(arena);

				// Reset the clock so there isn't a frame jump
				clock.restart();
			}
		}// End levelling up

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

			// Update the player
			player.update(dtAsSeconds, Mouse::getPosition());

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
		
		if (state == State::MAIN_MENU || state == State::LEVELING_UP || state == State::OPTIONS_MENU)
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
				window.draw(world.getBackground(i), &textureBackground);
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
				for (auto& entity : world.getEntities(i)) {
					drawables.emplace_back(entity.getPosition().y, entity.getSprite());
				}
			}
			drawables.emplace_back(player.getY(), player.getSpriteFromSheet());

			// Sort by y value using lambda function (ascending = top to bottom)
			std::sort(drawables.begin(), drawables.end(),
				[](const DrawableItem& a, const DrawableItem& b) {
					return a.y < b.y;
				}
			);

			// Draw in sorted order
			for (auto& item : drawables) {
				window.draw(item.sprite);
			}

			drawables.clear();

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
				window.draw(world.getForground(i), &textureBackground);
			}

			/*
			for (auto& txt : chunks[0].getDebugText()) { // draw debug text showing tile location
				window.draw(txt);
			}

			for (auto& nav : world.getNavBoxes(0)) { // draw debug text showing tile location
				window.draw(nav.getShape());
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
			
			// Draw all the HUD elements
			window.draw(goldCountText);
			window.draw(emptyHealthBar);
			window.draw(healthBar);
			window.draw(emptyManaBar);
			window.draw(manaBar);
			window.draw(emptyStaminaBar);
			window.draw(staminaBar);
			if (displayFps) {
				window.draw(fpsText);
			}
		}

		if (state == State::LEVELING_UP)
		{
			window.clear();
			window.draw(spriteMainMenu);
			window.draw(levelUpText);
		}

		if (state == State::PAUSED)
		{
			window.draw(pausedText);
			window.draw(mainMenuButton);
			window.draw(mainMenuButtonText);
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
		}

		if (state == State::STORY_INTRO) 
		{
			window.clear();
			window.draw(storyIntroText);
			window.draw(storyIntroText);
			window.draw(skipIntroText);
		}

		window.display();

	}// End game loop

	return 0;
}