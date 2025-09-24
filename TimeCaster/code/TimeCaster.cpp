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
#include "CreateBackground.h"
#include "SoundManager.h"
#include "Entity.h"

using namespace sf;
using namespace std;

int main()
{
	// Here is the instance of TextureHolder
	TextureHolder holder;
	
	// The game will always be in one of four states
	enum class State { MAIN_MENU, OPTIONS, PLAYING, PAUSED, LEVELING_UP, GAME_OVER };
	
	// Start with the MAIN_MENU state
	State state = State::MAIN_MENU;


	// Get the screen resolution and create an SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	resolution.x = 1920;
	resolution.y = 1080;

	RenderWindow window(VideoMode(resolution.x, resolution.y),
		"TimeCaster", Style::Fullscreen);

	// Create a an SFML View for the main action
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Zoom view
	mainView.zoom(0.5f);

	// Here is our clock for timing everything
	Clock clock;

	// Clock used for tracking and displaying fps
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

	CreateBackground landscape;

	// Create the background
	//VertexArray background;
	// Load the texture for our background vertex array
	Texture textureBackground = TextureHolder::GetTexture("graphics/landscape.png");

	// 100 bullets should do
	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;

	// FPS float number
	float fps = 0.f;
	
	// When was the fire button last pressed?
	Time lastPressed;

	//Decals 
	Decal decal[250];
	int currentDecal = 0;

	// Hide the mouse pointer and replace it with crosshair
	window.setMouseCursorVisible(true);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	// Create a couple of pickups
	Pickup healthPickup(1);
	Pickup ammoPickup(2);
	Pickup staminaPickup(3);
	Pickup manaPickup(4);

	// About the game
	int goldCount = 0;
	int hiScore = 0;

	// For the home/game over screen
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/Castle (edited).jpg");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);

	// Create a view for the HUD
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Create a sprite for the ammo icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);

	// Load the font
	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");

	// Main menu font
	Font pixelFont;
	pixelFont.loadFromFile("fonts/PixelifySans-Bold.ttf");

	// Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press enter \nto continue");

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
	goldCountText.setFont(pixelFont);
	goldCountText.setCharacterSize(55);
	goldCountText.setFillColor(Color::White);
	goldCountText.setPosition(1400, 0);

	// Load the high score from a text file/
	ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> hiScore;
		inputFile.close();
	}

	// Buy Shotgun Text
	Text buyShotgunText;
	buyShotgunText.setFont(font);
	buyShotgunText.setCharacterSize(25);
	buyShotgunText.setFillColor(Color::White);
	buyShotgunText.setPosition(960,540);
	buyShotgunText.setString("E: Buy Shotgun 50 pts");

	// Buy Rifle Text
	Text buyRifleText;
	buyRifleText.setFont(font);
	buyRifleText.setCharacterSize(25);
	buyRifleText.setFillColor(Color::White);
	buyRifleText.setPosition(960, 540);
	buyRifleText.setString("E: Buy Assault Rifle 100 pts");

	// FPS text
	Text fpsText;
	fpsText.setFont(pixelFont);
	fpsText.setCharacterSize(20);
	fpsText.setFillColor(Color::White);
	fpsText.setPosition(1850, 5);

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

	// Play button
	RectangleShape playButton;
	playButton.setFillColor(Color::Green);
	playButton.setPosition(10, 10);
	playButton.setSize(Vector2f(100, 40));

	// Play button text
	Text playButtonText;
	playButtonText.setFont(pixelFont);
	playButtonText.setCharacterSize(20);
	playButtonText.setFillColor(Color::Black);
	playButtonText.setPosition(10, 20);
	playButtonText.setString("Play Game");

	// Options button
	RectangleShape optionsButton;
	optionsButton.setFillColor(Color::Green);
	optionsButton.setPosition(10, 60);
	optionsButton.setSize(Vector2f(100, 40));

	// options button text
	Text optionsButtonText;
	optionsButtonText.setFont(pixelFont);
	optionsButtonText.setCharacterSize(20);
	optionsButtonText.setFillColor(Color::Black);
	optionsButtonText.setPosition(10, 70);
	optionsButtonText.setString("Options");

	// Quit game button
	RectangleShape quitGameButton;
	quitGameButton.setFillColor(Color::Green);
	quitGameButton.setPosition(10, 110);
	quitGameButton.setSize(Vector2f(100, 40));

	// options button text
	Text quitGameButtonText;
	quitGameButtonText.setFont(pixelFont);
	quitGameButtonText.setCharacterSize(20);
	quitGameButtonText.setFillColor(Color::Black);
	quitGameButtonText.setPosition(10, 120);
	quitGameButtonText.setString("Quit Game");

	// When did we last update the HUD?
	int framesSinceLastHUDUpdate = 0;

	// What time was the last update
	Time timeSinceLastUpdate;

	// How often (in frames) should we update the HUD
	int fpsMeasurementFrameInterval = 1000;

	// struct to store y values and sprites together
	struct DrawableItem {
		float y;
		sf::Sprite sprite;

		DrawableItem(float y, const sf::Sprite& sprite)
			: y(y), sprite(sprite) {}
	};

	std::vector<DrawableItem> drawables;

	bool debugreset = false; //it's a bit of a hack but it works to stop multiple upgrades from one key press
	//press numpad0 to reset if you want to test again
	//remove this in full build

	// The main game loop
	while (window.isOpen())
	{
		// Calculating fps
		float deltaTime = fpsClock.restart().asSeconds();
		fps = 1.f / deltaTime;

		fpsText.setString("FPS: " + to_string((int)fps));
		
		/*
		************
		Handle input
		************
		*/

		// Handle events
		Event event;
		while (window.pollEvent(event))
		{
			// Getting the mouse position and mapping those pixels to coordinates
			Vector2i mousePos = Mouse::getPosition(window);
			Vector2f worldPos = window.mapPixelToCoords(mousePos);

			if (event.type == Event::KeyPressed || Mouse::isButtonPressed(Mouse::Left))
			{
				// Pause a game while playing
				if (event.key.code == Keyboard::Return && state == State::PLAYING)
				{
					state = State::PAUSED;
				}

				// Restart while paused
				else if (event.key.code == Keyboard::Return && state == State::PAUSED)
				{
					state = State::PLAYING;
					// Reset the clock so there isn't a frame jump
					clock.restart();
				}

				// Start a new game while in MAIN_MENU state
				else if (event.key.code == Keyboard::Return && state == State::MAIN_MENU)
				{
					state = State::LEVELING_UP;
					goldCount = 0;

					// Reset the player's stats
					player.resetPlayerStats();
				}

				// Player hit the play game button
				else if (playButton.getGlobalBounds().contains(worldPos))
				{
					state = State::LEVELING_UP;
					goldCount = 0;

					// Reset the player's stats
					player.resetPlayerStats();
				}

				// Player hit the options button
				else if (optionsButton.getGlobalBounds().contains(worldPos))
				{
					state = State::OPTIONS;
				}

				// Player hit the quit game button
				else if (quitGameButton.getGlobalBounds().contains(worldPos))
				{
					window.close();
				}
			}
		} // End event polling

		 // Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Handle controls while playing
		if (state == State::PLAYING)
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

			// Fire a bullet
			if (Mouse::isButtonPressed(Mouse::Left))
			{


			} // End fire a bullet



		}// End WASD while playing
		// below are debug functions, comment them out in full build / when needed
		// if you add any more, make sure they check if debug reset is false and set it to true or else it will run every loop while the key is pressed

		if (event.key.code == Keyboard::Numpad0)
		{
			debugreset = false;
		}
		if (event.key.code == Keyboard::Numpad1 && !debugreset)
		{
			// Increase health
			player.upgradeHealth();
			debugreset = true;
		}

		if (event.key.code == Keyboard::Numpad2 &&  !debugreset)
		{
			// Increase stamina
			player.upgradeStamina();
			debugreset = true;
		}

		if (event.key.code == Keyboard::Numpad3 && !debugreset)
		{
			// Increase health
			player.upgradeMana();
			debugreset = true;
		}


		if (event.key.code == Keyboard::Numpad8 && !debugreset)
		{
		
			player.hit(gameTimeTotal, 10, 200);
			debugreset = true;
		
		}
		if (event.key.code == Keyboard::Numpad9 && !debugreset)
		{

			player.hit(gameTimeTotal, 30, 1000);
			debugreset = true;

		}

		// Handle the levelling up state
		if (state == State::LEVELING_UP)
		{
			// Handle the player levelling up
			if (event.key.code == Keyboard::Num1)
			{
				// Increase fire rate
				fireRate++;
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2)
			{
				// Increase clip size
				clipSize += clipSize;
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

			if (state == State::PLAYING)
			{
				// Increase the wave number
				//wave++;

				// Prepare the level
				
				// We will modify the next two lines later
				arena.width = 1900;
				arena.height = 800;
				arena.left = 1664;
				arena.top = 1664;

				// Pass the vertex array by reference 
				// to the createBackground function
				int tileSize = landscape.createLandscape();

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

		 /*
		 ****************
		 UPDATE THE FRAME
		 ****************
		 */
		if (state == State::PLAYING)
		{
			ShowCursor(false); // hide the windows cursor

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
			spriteCrosshair.setPosition(mouseWorldPosition);

			// Update the player
			player.update(dtAsSeconds, Mouse::getPosition());

			// Make a note of the players new position
			Vector2f playerPosition(player.getCenter());

			// Make the view centre around the player				
			mainView.setCenter(player.getCenter());

			// Loop through each Zombie and update them
			
			// Update any bullets that are in-flight
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					bullets[i].update(dtAsSeconds);
				}
			}

			// Update the pickups
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);
			staminaPickup.update(dtAsSeconds);
			manaPickup.update(dtAsSeconds);

			// Has the player touched health pickup
			if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealthLevel(healthPickup.gotIt());
			}

			// Has the player touched ammo pickup
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned()) 
			{
				bulletsSpare += ammoPickup.gotIt();
			}


			// Has the player touched stamina pickup
			if (player.getPosition().intersects(staminaPickup.getPosition()) && staminaPickup.isSpawned())
			{
				player.increaseStaminaLevel(staminaPickup.gotIt()); 
				// Play a sound
	
			}


			// Has the player touched mana pickup
			if (player.getPosition().intersects(manaPickup.getPosition()) && manaPickup.isSpawned())
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
			
			// Update game HUD text
			stringstream ssGoldCount;

			// Update the gold text
			ssGoldCount << "Gold:" << goldCount;
			goldCountText.setString(ssGoldCount.str());

			framesSinceLastHUDUpdate = 0;
			timeSinceLastUpdate = Time::Zero;
			// End HUD update

		} // End updating the scene

		 /*
		 **************
		 Draw the scene
		 **************
		 */

		if (state == State::PLAYING)
		{
			window.clear();

			// set the mainView to be displayed in the window
			// And draw everything related to it
			window.setView(mainView);

			// Draw the background
    		window.draw(landscape.getBackground(), &textureBackground);

			for (auto& txt : landscape.getDebugText()) {
				window.draw(txt);
			}

			// DRAW EFFECTS
			for (int i = 0; i < 249; i++) // draw decals
			{
				window.draw(decal[i].getSprite());
			}

			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					window.draw(bullets[i].getShape());
				}
			}

			for (auto& entity : landscape.getEntities()) {
				drawables.emplace_back(entity.getPosition().y, entity.getSprite());
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

			// Draw the pickups is currently spawned
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

			window.draw(landscape.getForground(), &textureBackground);

			//Draw the crosshair
			window.draw(spriteCrosshair);

			// Switch to the HUD view
			window.setView(hudView);
			
			// Draw all the HUD elements
			window.draw(spriteAmmoIcon);
			window.draw(goldCountText);
			window.draw(emptyHealthBar);
			window.draw(healthBar);
			window.draw(emptyManaBar);
			window.draw(manaBar);
			window.draw(emptyStaminaBar);
			window.draw(staminaBar);
			window.draw(fpsText);
		}

		if (state == State::LEVELING_UP)
		{
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}

		if (state == State::PAUSED)
		{
			window.draw(pausedText);
		}

		if (state == State::MAIN_MENU)
		{
			window.draw(spriteGameOver);
			window.draw(goldCountText);
			window.draw(playButton);
			window.draw(playButtonText);
			window.draw(optionsButton);
			window.draw(optionsButtonText);
			window.draw(quitGameButton);
			window.draw(quitGameButtonText);
		}

		window.display();

	}// End game loop

	return 0;
}