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

using namespace sf;
using namespace std;

int main()
{
	// Here is the instance of TextureHolder
	TextureHolder holder;
	
	// The game will always be in one of four states
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
	// Start with the GAME_OVER state
	State state = State::GAME_OVER;


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

	// About the game
	int score = 0;
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

	// Game Over
	Text gameOverText;
	gameOverText.setFont(pixelFont);
	gameOverText.setCharacterSize(90);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(50, 950);
	gameOverText.setString("Press enter to play");

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

	// Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);

	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 100);

	// Load the high score from a text file/
	ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> hiScore;
		inputFile.close();
	}

	// Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());

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

	// Zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 980);
	zombiesRemainingText.setString("Zombies: 100");

	// Wave number
	int round = 1;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");

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



		
	// When did we last update the HUD?
	int framesSinceLastHUDUpdate = 0;

	// What time was the last update
	Time timeSinceLastUpdate;

	// How often (in frames) should we update the HUD
	int fpsMeasurementFrameInterval = 1000;

	// Dodge variables
	bool isDodging = false;
	bool canDodge = false;
	Clock dodgeClock;
	Clock cooldownClock;
	float dodgeDuration = 0.2f; // 200ms dodge
	float dodgeCooldown = 1.0f; // 1 second cooldown on dodge

	// Prepare the hit sound
	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);

	// Prepare the splat sound
	SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/splat.wav");
	sf::Sound splat;
	splat.setBuffer(splatBuffer);

	// Prepare the shoot sound
	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shootBuffer);

	// Prepare the reload sound
	SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload.wav");
	Sound reload;
	reload.setBuffer(reloadBuffer);

	// Prepare the failed sound
	SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedBuffer);

	// Prepare the powerup sound
	SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	Sound powerup;
	powerup.setBuffer(powerupBuffer);

	// Prepare the pickup sound
	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/pickup.wav");
	Sound pickup;
	pickup.setBuffer(pickupBuffer);

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
			if (event.type == Event::KeyPressed)
			{
				// Pause a game while playing
				if (event.key.code == Keyboard::Return &&
					state == State::PLAYING)
				{
					state = State::PAUSED;
				}

				// Restart while paused
				else if (event.key.code == Keyboard::Return &&
					state == State::PAUSED)
				{
					state = State::PLAYING;
					// Reset the clock so there isn't a frame jump
					clock.restart();
				}

				// Start a new game while in GAME_OVER state
				else if (event.key.code == Keyboard::Return &&
					state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
					round = 1;
					score = 0;

					// Prepare the gun and ammo for next game
					currentBullet = 0;
					bulletsSpare = 24;
					bulletsInClip = 6;
					clipSize = 6;
					fireRate = 1;

					// Reset the player's stats
					player.resetPlayerStats();
				}

				if (state == State::PLAYING)
				{
					// Reloading
					if (event.key.code == Keyboard::R)
					{
						if (bulletsSpare >= clipSize)
						{
							// Plenty of bullets. Reload.
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;		
							reload.play();
						}
						else if (bulletsSpare > 0)
						{
							// Only few bullets left
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;				
							reload.play();
						}
						else
						{
							// More here soon?!
							reloadFailed.play();
						}
					}
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
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (player.getGun() == 0) // pistol
				{
					if (gameTimeTotal.asMilliseconds()
						- lastPressed.asMilliseconds()
					> 1000 / fireRate && bulletsInClip > 0)
					{

						// Pass the centre of the player and the centre of the crosshair
						// to the shoot function
						bullets[currentBullet].shoot(
							player.getCenter().x, player.getCenter().y,
							mouseWorldPosition.x, mouseWorldPosition.y);

						currentBullet++;
						if (currentBullet > 99)
						{
							currentBullet = 0;
						}
						lastPressed = gameTimeTotal;
						shoot.play();
						bulletsInClip--;
					}
				}

				if (player.getGun() == 1) // shotgun
				{
					if (gameTimeTotal.asMilliseconds()
						- lastPressed.asMilliseconds()
					> 1000 / fireRate && bulletsInClip > 0)
					{

						// Pass the centre of the player and the centre of the crosshair
						// to the shoot function
						bullets[currentBullet].shoot(
							player.getCenter().x, player.getCenter().y,
							mouseWorldPosition.x, mouseWorldPosition.y);
						currentBullet++;

						bullets[currentBullet].shoot(
							player.getCenter().x, player.getCenter().y,
							mouseWorldPosition.x + 35, mouseWorldPosition.y);
						currentBullet++;

						bullets[currentBullet].shoot(
							player.getCenter().x, player.getCenter().y,
							mouseWorldPosition.x, mouseWorldPosition.y + 35);
						currentBullet++;

						bullets[currentBullet].shoot(
							player.getCenter().x, player.getCenter().y,
							mouseWorldPosition.x - 35, mouseWorldPosition.y);
						currentBullet++;

						bullets[currentBullet].shoot(
							player.getCenter().x, player.getCenter().y,
							mouseWorldPosition.x, mouseWorldPosition.y - 35);
						currentBullet++;

						if (currentBullet > 99)
						{
							currentBullet = 0;
						}
						lastPressed = gameTimeTotal;
						shoot.play();
						bulletsInClip--;
					}
				}

				if (player.getGun() == 2) // rifle
				{
					if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
					{
						// Pass the centre of the player and the centre of the crosshair
						// to the shoot function
						bullets[currentBullet].shoot(
							player.getCenter().x, player.getCenter().y,
							mouseWorldPosition.x, mouseWorldPosition.y);

						currentBullet++;
						if (currentBullet > 99)
						{
							currentBullet = 0;
						}
						lastPressed = gameTimeTotal;
						shoot.play();
						bulletsInClip--;
					}
				}
			} // End fire a bullet

			// Dodging enemies using the space key
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canDodge) {
				isDodging = true;
				canDodge = false;
				
				dodgeClock.restart();
				cooldownClock.restart();
				player.startDodge();
			}

			// After 200ms stop dodge
			if (isDodging && dodgeClock.getElapsedTime().asSeconds() > dodgeDuration) {
				isDodging = false;
				player.stopDodge();
			}
		
			// Allows the player to dodge again
			if (!canDodge && cooldownClock.getElapsedTime().asSeconds() > dodgeCooldown && player.getStamina() >= 50) {
				canDodge = true;
			}

		}// End WASD while playing

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

				// Play the powerup sound
				powerup.play();

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
			// if all zombies are dead create new round with more zombies

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

			// Collision detection
			// Have any zombies been shot?

			// Has the player touched health pickup
			if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealthLevel(healthPickup.gotIt());
				// Play a sound
				pickup.play();
			}

			// Has the player touched ammo pickup
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned()) 
			{
				bulletsSpare += ammoPickup.gotIt();
				// Play a sound
				reload.play();
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
			stringstream ssAmmo;
			stringstream ssScore;
			stringstream ssHiScore;
			stringstream ssWave;
			stringstream ssZombiesAlive;

			// Update the ammo text
			ssAmmo << bulletsInClip << "/" << bulletsSpare;
			ammoText.setString(ssAmmo.str());

			// Update the score text
			ssScore << "Points:" << score;
			scoreText.setString(ssScore.str());

			// Update the high score text
			ssHiScore << "Hi Score:" << hiScore;
			hiScoreText.setString(ssHiScore.str());

			// Update the wave
			ssWave << "Round:" << round;
			waveNumberText.setString(ssWave.str());

			// Update the high score text
			ssZombiesAlive << "Zombies:" << 1;
			zombiesRemainingText.setString(ssZombiesAlive.str());

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
    		window.draw(landscape.getLandscape(), &textureBackground);

			for (auto& txt : landscape.getDebugText()) {
				window.draw(txt);
			}

			// DRAW EFFECTS
			for (int i = 0; i < 249; i++) // draw decals
			{
				window.draw(decal[i].getSprite());
			}

			// Draw the zombies

			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					window.draw(bullets[i].getShape());
				}
			}

			// Draw the player
			window.draw(player.getSprite());

			// Draw the pickups is currently spawned
			if (ammoPickup.isSpawned())
			{
				window.draw(ammoPickup.getSprite());
			}
			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}

			//Draw the crosshair
			window.draw(spriteCrosshair);

			// Switch to the HUD view
			window.setView(hudView);
			
			// Draw all the HUD elements
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(emptyHealthBar);
			window.draw(healthBar);
			window.draw(emptyManaBar);
			window.draw(manaBar);
			window.draw(emptyStaminaBar);
			window.draw(staminaBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
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

		if (state == State::GAME_OVER)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}

		window.display();

	}// End game loop

	return 0;
}