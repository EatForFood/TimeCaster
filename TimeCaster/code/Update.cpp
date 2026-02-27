#include "Engine.h"
#include <iomanip>
#include "Windows.h"
#include <thread>

using namespace sf;
using namespace std;

void Engine::update()
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
		// Zooming in and out using the scroll wheel
		if (event.type == Event::MouseWheelScrolled)
		{
			if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel)
			{
				if (event.mouseWheelScroll.delta > 0)
				{
					if (mainView.getSize().x > 250 && mainView.getSize().y > 150 || debugMode)
					{
						mainView.zoom(0.9f);
						spriteCursor.scale(0.9f, 0.9f);
					}
				}
				else if (event.mouseWheelScroll.delta < 0)
				{
					if (mainView.getSize().x < 1000 && mainView.getSize().y < 800 || debugMode) {
						mainView.zoom(1.1f);
						spriteCursor.scale(1.1f, 1.1f);
					}
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

		// Activate Fps textbox when clicked
		if (event.type == sf::Event::MouseButtonPressed && state == State::OPTIONS_MENU)
		{
			if (event.mouseButton.button == Mouse::Left)
			{
				sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				textBoxActiveFps = textBoxFps.getGlobalBounds().contains(mousePos);
				textBoxFps.setOutlineColor(textBoxActiveFps ? Color::Green : Color::White);
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

		// Handle typing if text box active
		if (textBoxActiveFps && event.type == sf::Event::TextEntered && state == State::OPTIONS_MENU)
		{
			if (event.text.unicode >= '0' && event.text.unicode <= '9')
			{
				userInputStringFps += static_cast<char>(event.text.unicode);
			}
			else if (event.text.unicode == 8 && !userInputStringFps.empty()) // User hits backspace
			{
				userInputStringFps.pop_back();
			}
			else if (event.text.unicode == 13) // User hits enter key
			{
				if (!userInputStringFps.empty())
				{
					int num = stoi(userInputStringFps);
					if (num > 0)
					{
						fpsLimit = num;
						feedbackFps.setString("FPS limit set to " + to_string(fpsLimit));
						player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume(), vSync, fpsLimit);
						window.setFramerateLimit(fpsLimit);
					}
					else
					{
						feedbackFps.setString("Invalid! You must set your FPS to a number higher than 0.");
					}
				}
			}
			userInputTextFps.setString(userInputStringFps);
		}

		// Stop dragging
		if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
		{
			dragging = false;
		}

		if (event.type == Event::KeyPressed && event.key.code == Keyboard::E && drawEKey)
		{
			if (drawShop)
			{
				drawShop = false;
			}
			else
			{
				if (firstTimeInShop)
				{
					shopText.setString("Welcome to the shop! \nClick on one of my items up top to buy it.\nClick on one of your own items down below to sell it!");
					shopKeeperSetEmotion(2);
					firstTimeInShop = false;
				}
				else
				{
					shopText.setString("Welcome back!\nIt's always nice to see a familiar face.");
					shopKeeperSetEmotion(1);
				}
				shopKeeperSetEmotion(2);
				attemptedToSellSentimentalItem = false;
				drawShop = true;
			}
		}

		if (event.type == Event::KeyPressed || Mouse::isButtonPressed(Mouse::Left))
		{
			// Pause the game if escape key pressed
			if (event.key.code == Keyboard::Escape && state == State::PLAYING && !drawInventory && !drawShop)
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
				sound.stopMainMenuSound();
				enemyArr.clear();
				state = State::STORY_INTRO;
				timeToBeat.restart();

				skipAnimation = false;

				// Play the start game sound
				if (!startSoundPlayed) {
					sound.playStartGameSound();
				}

				sound.playStoryIntroSound();

				startSoundPlayed = true;

				player.createNewSave();
				player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume(), vSync, fpsLimit);
				player.loadSaveFile();

				equippedSwordIcon.setTextureRect(player.getEquippedSword()->getTextureRect());
				equippedWandIcon.setTextureRect(player.getEquippedWand()->getTextureRect());
				equippedHeadArmourIcon = (player.getEquippedHeadArmour()->getIcon());
				equippedHeadArmourIcon.setPosition(headArmourFrame.getPosition());
				equippedChestArmourIcon = (player.getEquippedChestArmour()->getIcon());
				equippedChestArmourIcon.setPosition(chestArmourFrame.getPosition());
				equippedTrousersArmourIcon = (player.getEquippedTrouserArmour()->getIcon());
				equippedTrousersArmourIcon.setPosition(trousersArmourFrame.getPosition());
				equippedShoeArmourIcon = (player.getEquippedShoeArmour()->getIcon());
				equippedShoeArmourIcon.setPosition(bootsArmourFrame.getPosition());
				equippedNeckArmourIcon = (player.getEquippedNeckArmour()->getIcon());
				equippedNeckArmourIcon.setPosition(neckFrame.getPosition());
				equipAllItems();

				// We will modify the next two lines later
				arena.width = 1900;
				arena.height = 800;
				arena.left = 1664;
				arena.top = 1664;

				// Pass the vertex array by reference to the createBackground function
				int tileSize = 64;

				// Spawn the player in the middle of the arena
				player.spawn(resolution, tileSize, player.getPlayerLevel(), true);

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

				initializeInventory();
			}

			// Player hit the load game button in the main menu
			else if (loadGameButton.getGlobalBounds().contains(worldPos) && state == State::MAIN_MENU && event.mouseButton.button == Mouse::Left)
			{
				sound.stopMainMenuSound();
				state = State::LOADING;
				skipAnimation = false;

				// Play the start game sound
				if (!startSoundPlayed) {
					sound.playStartGameSound();
				}

				startSoundPlayed = true;

				// Loads player stats from text file
				if (player.loadSaveFile() == true)
				{
					enemyArr.clear();
					// Player loaded successfully

					// Update equipped item icons
					equippedSwordIcon.setTextureRect(player.getEquippedSword()->getTextureRect());
					equippedWandIcon.setTextureRect(player.getEquippedWand()->getTextureRect());

					equippedHeadArmourIcon = (player.getEquippedHeadArmour()->getIcon());
					equippedHeadArmourIcon.setPosition(headArmourFrame.getPosition());
					equippedChestArmourIcon = (player.getEquippedChestArmour()->getIcon());
					equippedChestArmourIcon.setPosition(chestArmourFrame.getPosition());
					equippedTrousersArmourIcon = (player.getEquippedTrouserArmour()->getIcon());
					equippedTrousersArmourIcon.setPosition(trousersArmourFrame.getPosition());
					equippedShoeArmourIcon = (player.getEquippedShoeArmour()->getIcon());
					equippedShoeArmourIcon.setPosition(bootsArmourFrame.getPosition());
					equippedNeckArmourIcon = (player.getEquippedNeckArmour()->getIcon());
					equippedNeckArmourIcon.setPosition(neckFrame.getPosition());
					equipAllItems();

					// We will modify the next two lines later
					arena.width = 1900;
					arena.height = 800;
					arena.left = 1664;
					arena.top = 1664;

					// Pass the vertex array by reference to the createBackground function
					int tileSize = 64;

					// Spawn the player, don't set their position since it loaded from the save file
					player.spawn(resolution, tileSize, player.getPlayerLevel(), false);

					// Reset the clock so there isn't a frame jump
					clock.restart();

					player.loadConfigFile();
					difficulty = stringToDifficulty(player.getdifficultyString());
					windowedMode = player.getWindowedMode();
					vSync = player.getVSync();
					displayFps = player.getDisplayFps();
					Listener::setGlobalVolume(player.getVolume());

					setDifficulty();

					loadWorldText.setString("Loading game...");
					textBounds = loadWorldText.getLocalBounds();
					viewCentre = hudView.getCenter();
					loadWorldText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, viewCentre.y - loadWorldText.getCharacterSize());

					initializeInventory();
					if (world.worldFileExists())
					{
						thread worldThread(&Engine::loadGameWorld, this);
						worldThread.detach();
					}
					else
					{
						thread worldThread(&Engine::generateWorld, this);
						worldThread.detach();
					}
					tutorialStage = 2;
				}
				else {
					enemyArr.clear();
					// No save file so create a new one with default values and load it	
					player.createNewSave();
					player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume(), vSync, fpsLimit);
					player.loadSaveFile();

					equippedSwordIcon.setTextureRect(player.getEquippedSword()->getTextureRect());
					equippedWandIcon.setTextureRect(player.getEquippedWand()->getTextureRect());
					equippedHeadArmourIcon = (player.getEquippedHeadArmour()->getIcon());
					equippedHeadArmourIcon.setPosition(headArmourFrame.getPosition());
					equippedChestArmourIcon = (player.getEquippedChestArmour()->getIcon());
					equippedChestArmourIcon.setPosition(chestArmourFrame.getPosition());
					equippedTrousersArmourIcon = (player.getEquippedTrouserArmour()->getIcon());
					equippedTrousersArmourIcon.setPosition(trousersArmourFrame.getPosition());
					equippedShoeArmourIcon = (player.getEquippedShoeArmour()->getIcon());
					equippedShoeArmourIcon.setPosition(bootsArmourFrame.getPosition());
					equippedNeckArmourIcon = (player.getEquippedNeckArmour()->getIcon());
					equippedNeckArmourIcon.setPosition(neckFrame.getPosition());
					equipAllItems();

					// We will modify the next two lines later
					arena.width = 1900;
					arena.height = 800;
					arena.left = 1664;
					arena.top = 1664;

					// Pass the vertex array by reference 
					// to the createBackground function
					int tileSize = 64;

					// Spawn the player in the middle of the arena
					player.spawn(resolution, tileSize, player.getPlayerLevel(), true);

					// Reset the clock so there isn't a frame jump
					clock.restart();

					player.loadConfigFile();
					difficulty = stringToDifficulty(player.getdifficultyString());
					windowedMode = player.getWindowedMode();
					vSync = player.getVSync();
					displayFps = player.getDisplayFps();
					Listener::setGlobalVolume(player.getVolume());
					setDifficulty();

					loadWorldText.setString("Loading game...");
					textBounds = loadWorldText.getLocalBounds();
					viewCentre = hudView.getCenter();
					loadWorldText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, viewCentre.y - loadWorldText.getCharacterSize());

					initializeInventory();
					if (world.worldFileExists())
					{
						thread worldThread(&Engine::loadGameWorld, this);
						worldThread.detach();
					}
					else
					{
						thread worldThread(&Engine::generateWorld, this);
						worldThread.detach();
					}

					tutorialStage = 0;
				}
			}

			// Player hit the controls button
			if (controlsButton.getGlobalBounds().contains(worldPos) && state == State::MAIN_MENU && event.mouseButton.button == Mouse::Left)
			{
				sound.playButtonClickSound();
				world.clearWorld();

				state = State::CONTROLS_MENU;
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

				state = State::OPTIONS_MENU;
			}

			// Player hit the credits button
			if (creditsButton.getGlobalBounds().contains(worldPos) && state == State::MAIN_MENU && event.mouseButton.button == Mouse::Left)
			{
				sound.playButtonClickSound();
				world.clearWorld();
				state = State::CREDITS_MENU;
			}

			// Player hit the quit game button
			if (quitGameButton.getGlobalBounds().contains(worldPos) && state == State::MAIN_MENU && event.mouseButton.button == Mouse::Left)
			{
				sound.playButtonClickSound();
				// Save info to file before quitting
				window.close();
			}

			// Player hit the game over quit game button
			if (gameOverQuitButton.getGlobalBounds().contains(worldPos) && (state == State::GAME_OVER || state == State::VICTORY) && event.mouseButton.button == Mouse::Left)
			{
				sound.playButtonClickSound();
				// Save info to file before quitting
				window.close();
			}

			// Player hit the main menu button in the game over screen
			if (gameOverMainMenuButton.getGlobalBounds().contains(worldPos) && (state == State::GAME_OVER || state == State::VICTORY) && event.mouseButton.button == Mouse::Left)
			{
				sound.stopGameOverSound();
				sound.stopVictorySound();
				sound.playButtonClickSound();
				world.clearWorld();
				state = State::MAIN_MENU;
				sound.playMainMenuSound();
			}

			// Player hit the main menu button in the pause menu
			if (mainMenuButton.getGlobalBounds().contains(worldPos) && state == State::PAUSED && event.mouseButton.button == Mouse::Left)
			{
				sound.playButtonClickSound();
				world.clearWorld();
				player.updateSaveFile();
				enemyArr.clear();
				state = State::MAIN_MENU;
				sound.playMainMenuSound();
			}

			// Player hit the main menu button in the controls menu
			if (mainMenuButton.getGlobalBounds().contains(worldPos) && state == State::CONTROLS_MENU && event.mouseButton.button == Mouse::Left)
			{
				sound.playButtonClickSound();
				world.clearWorld();
				state = State::MAIN_MENU;
			}

			// Player hit the main menu button in the credits menu
			if (mainMenuButton.getGlobalBounds().contains(worldPos) && state == State::CREDITS_MENU && event.mouseButton.button == Mouse::Left)
			{
				sound.playButtonClickSound();
				world.clearWorld();
				state = State::MAIN_MENU;
			}
			
			// Player hit the main menu button in the options menu
			if (mainMenuButton.getGlobalBounds().contains(worldPos) && state == State::OPTIONS_MENU && event.mouseButton.button == Mouse::Left)
			{
				sound.playButtonClickSound();
				world.clearWorld();
				player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume(), vSync, fpsLimit);
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
					window.setFramerateLimit(fpsLimit);
				}
				else {
					sound.playButtonClickSound();
					windowedMode = true;
					window.create(VideoMode(resolution.x, resolution.y), "TimeCaster", Style::Default);
					window.setFramerateLimit(fpsLimit);
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
				difficultyButtonText.setPosition(Vector2f(difficultyButton.getPosition().x + (difficultyButton.getSize().x / 2.f), difficultyButton.getPosition().y + (difficultyButton.getSize().y / 2.f) - 10));
				difficultyButton.setPosition(viewCentre.x - (difficultyButton.getSize().x / 2.f), 400);

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

			if (event.key.code == Keyboard::V && state == State::PLAYING && debugMode)
			{
				if (sound.isSoundtrackPlaying()) {
					sound.stopSoundtrack();
				}
				sound.playVictorySound();
				gameOverText.setString("Victory!");
				gameOverText.setFillColor(Color::Yellow);
				textBounds = gameOverText.getLocalBounds();
				viewCentre = hudView.getCenter();
				gameOverText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, viewCentre.y - gameOverText.getCharacterSize());

				int silly = rand() % 100;
				if (silly == 67)
				{
					if (player.soldSentimentalItem()) {
						gameOverText2.setString("You sold a sentimental item... \nWas it worth it?\nYOU MONSTER!");
					}
					else
					{
						gameOverText2.setString("You managed to keep all your sentimental items! \nWell done!");
					}
				}
				else
				{
					if (!player.soldSentimentalItem()) {
						gameOverText2.setString("Ignis has been slain.\nYou can finally rest...");
					}
					else {
						gameOverText2.setString("Ignis has been slain.\nBut the locket containing last memories of your family... It's gone.");
					}
				}

				textBounds = gameOverText2.getLocalBounds();
				viewCentre = hudView.getCenter();
				gameOverText2.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, viewCentre.y - 400 - gameOverText2.getCharacterSize());

				stringstream ssStatText;
				ssStatText << fixed << setprecision(0) << "Level Reached: " << player.getPlayerLevel() << "\nGold Earned: " << player.getGold()
					<< "\nEnemies Killed: " << player.getKillCount() << "\nTime Taken: " << timeToBeat.getElapsedTime().asSeconds() << " seconds";
				statText.setString(ssStatText.str());
				textBounds = statText.getLocalBounds();
				viewCentre = hudView.getCenter();
				statText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, viewCentre.y + 100 - statText.getCharacterSize());

				state = State::VICTORY;
			}

			if (event.key.code == Keyboard::Num1 && state == State::PLAYING)
			{
				if (debugMode)
				{
					// Increase health
					player.upgradeHealth();
				}
				player.switchSpell(1);
			}

			if (event.key.code == Keyboard::Num2 && state == State::PLAYING)
			{
				if (debugMode)
				{
					// Increase stamina
					player.upgradeStamina();
				}
				player.switchSpell(2);
			}

			if (event.key.code == Keyboard::Num3 && state == State::PLAYING)
			{
				if (debugMode)
				{
					// Increase mana
					player.upgradeMana();
				}
				player.switchSpell(3);
			}

			if (event.key.code == Keyboard::Num4 && state == State::PLAYING)
			{
				if (debugMode)
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
					restockShop(player.getPlayerLevel());
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
	if (state == State::PLAYING && !drawInventory && !drawShop)
	{
		if (Mouse::isButtonPressed(Mouse::Left) && !player.isSprinting())
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
					particles[100].play(player.getCenter().x - 30, player.getCenter().y - 30, 0); // 100 is the player's particle, 0-99 for the enemies
				}
			}
			else if (player.getCombatType() == Magic && !player.isCastingSpell() && player.getSpellType() == Player::SpellType::Phase && !player.isPhasing() && phaseTimer.getElapsedTime().asSeconds() > 0.5f)
			{
				player.startPhase();
				sound.playPhaseCastSound();
				cout << "Phasing started" << endl;
				phaseTimer.restart();
			}
			else if (player.getCombatType() == Magic && !player.isCastingSpell() && player.getSpellType() == Player::SpellType::Phase && player.isPhasing() && phaseTimer.getElapsedTime().asSeconds() > 0.5f)
			{
				player.stopPhase();
				sound.playPhaseEndSound();
				cout << "Phasing ended" << endl;
				phaseTimer.restart();
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

		if (Keyboard::isKeyPressed(Keyboard::LShift))
		{
			player.sprinting(true);
		}
		else
		{
			player.sprinting(false);
		}

	} // End WASD while playing
	if (drawInventory || drawShop)
	{
		player.stopRight();
		player.stopLeft();
		player.stopUp();
		player.stopDown();
	}

	/*********************************************************************
							   Playing State
	**********************************************************************/
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

		// Update the player
		if (state == State::PLAYING && !drawInventory && !drawShop) {
			player.update(dtAsSeconds, Mouse::getPosition(), world.getNavBoxes(player.getChunk()));
		}

		if (state == State::PLAYING && !drawInventory && !timeFrozen && !drawShop) {


			if (playerNearShop() && tutorialStage != 0) {
				drawEKey = true;
			}
			else {
				drawEKey = false;
			}

			// Update the vector of enemies if within player's render area
			for (auto& enemyPtr : enemyArr)
			{
				if (player.getRenderArea().intersects(enemyPtr->getSprite().getGlobalBounds()))
				{
					if (!enemyPtr->isDead())
					{
						enemyPtr->update(dtAsSeconds, player.getPosition(), getCurrentChunk(enemyPtr->getPosition().x, enemyPtr->getPosition().y), player.getChunk(), world.getNavBoxes(enemyPtr->getChunk()));

						for (int i = 0; i < world.getWorldSize(); i++)
						{
							if (collision.pointInShape(enemyPtr->getPosition(), world.getChunkArea(i).getShape())) // find enemies current chunk
							{
								enemyPtr->setChunk(i);
							}
						}

						if (player.getWeapon().getGlobalBounds().intersects(enemyPtr->getSprite().getGlobalBounds()))
						{
							enemyPtr->Attack();
							// Handle player getting hit by enemy
							if (enemyPtr->getSprite().getGlobalBounds().intersects(player.getHitBox()) && enemyPtr->isAttacking() && enemyPtr->getAniCounter() >= 5 && enemyPtr->getAniCounter() <= 7)
							{
								// Play the blood particle effect
								if (player.hit(gameTimeTotal, enemyPtr->getAttackDamage(), 1000))
								{
									sound.playHitSound();
									particles[100].play(player.getCenter().x - 30, player.getCenter().y - 30, 1);
									decal[currentDecal].spawn("bloodImpact", player.getPosition().x, player.getPosition().y);
									currentDecal++;
								}

							
							}
							if (player.getWeapon().getGlobalBounds().intersects(enemyPtr->getHitBox()) && player.isAttacking() && !enemyPtr->wasHit() && player.getAniCounter() >= 3 && player.getAniCounter() <= 7)
							{
								enemyPtr->setHealth(-player.getAttackDamage());
								enemyPtr->setWasHit(true);
								// Play the blood particle effect
								for (int i = 0; i < 100; i++)
								{
									if (!particles[i].isPlaying())
									{
										particles[i].play(enemyPtr->getCenter().x - 30, enemyPtr->getCenter().y - 30, 1);
										break;
									}
								}

								// Play enemy hit sound
								sound.playHitSound();
								decal[currentDecal].spawn("bloodImpact", enemyPtr->getPosition().x, enemyPtr->getPosition().y);
								currentDecal++;
							}
							else if (!player.isAttacking())
							{
								enemyPtr->setWasHit(false);
							}
						}

						if (enemyPtr->getType() == "Dragon")
						{
							if (enemyPtr->getAttackState() == Enemy::AttackState::Shoot && enemyPtr->getShotsFired() < 5 && enemyPtr->getShotCooldown() > 0.5)
							{
								dragonSpells[currentSpell].shoot(enemyPtr->getCenter().x, enemyPtr->getCenter().y, player.getPosition().x, player.getPosition().y, enemyPtr->getDamage());

								// Play fireball sound
								sound.playFireballSound();

								currentDragonSpell++;
								if (currentDragonSpell > 99)
								{
									currentDragonSpell = 0;
								}

								enemyPtr->shotFired();
							}
							else if (enemyPtr->getShotsFired() > 4)
							{
								enemyPtr->resetShotsFired();
							}

							if (enemyPtr->getAttackState() == Enemy::AttackState::Charge && !roarPlayed) {
								sound.playDragonRoar();
								roarPlayed = true;
								cout << "Sound played" << endl;
							}
							else if (enemyPtr->getAttackState() != Enemy::AttackState::Charge && roarPlayed) {
								roarPlayed = false;
							}

							// Player touches the dragon hitbox
							if (enemyPtr->getGlobalBounds().intersects(player.getHitBox())) {
								// Play the blood particle effect
								if (player.hit(gameTimeTotal, enemyPtr->getAttackDamage(), 1000))
								{
									sound.playDragonBite();
									sound.playHitSound();
									particles[100].play(player.getCenter().x - 30, player.getCenter().y - 30, 1);
									decal[currentDecal].spawn("bloodImpact", player.getPosition().x, player.getPosition().y);
									currentDecal++;
								}
							}
						}
					}
					if (enemyPtr->isDead() && !enemyPtr->isLooted())
					{
						if (enemyPtr->getType() == "Dragon") {
							sound.playVictorySound();
							gameOverText.setString("Victory!");
							gameOverText.setFillColor(Color::Yellow);
							textBounds = gameOverText.getLocalBounds();
							viewCentre = hudView.getCenter();
							gameOverText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, viewCentre.y - gameOverText.getCharacterSize());

							int silly = rand() % 100;
							if (silly == 67)
							{
								if (player.soldSentimentalItem()) {
									gameOverText2.setString("You sold a sentimental item... \nWas it worth it?\nYOU MONSTER!");
								}
								else
								{
									gameOverText2.setString("You managed to keep all your sentimental items! \nWell done!");
								}
							}
							else
							{
								if (!player.soldSentimentalItem()) {
									gameOverText2.setString("Ignis has been slain.\nYou can finally rest...");
								}
								else {
									gameOverText2.setString("Ignis has been slain.\nBut the locket containing last memories of your family... It's gone!");
								}
							}

							stringstream ssStatText;
							ssStatText << fixed << setprecision(0) << "Level Reached: " << player.getPlayerLevel() << "\nGold Earned: " << player.getGold()
								<< "\nEnemies Killed: " << player.getKillCount() << "\nTime Taken: " << timeToBeat.getElapsedTime().asSeconds() << " seconds";
							statText.setString(ssStatText.str());
							textBounds = statText.getLocalBounds();
							viewCentre = hudView.getCenter();
							statText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, viewCentre.y + 100 - statText.getCharacterSize());

							state = State::VICTORY;
						}

						if (player.reward(enemyPtr->loot()))
						{
							drawInventory = true;
							levelUp = true;
							restockShop(player.getPlayerLevel());
						}

						// spawn some gold
						for (int i = 0; i < (rand() % 10); i++) {
							items.emplace_back("Gold", enemyPtr->getPosition());
						}

						// potentially spawn random item
						string item = loot.getLoot(4);
						if (item != "nothing")
						{
							if (isEquipment(item))
							{
								items.emplace_back(Equipment(item, enemyPtr->getPosition()));
							}
							else if (isWeapon(item))
							{
								items.emplace_back(Weapon(item, enemyPtr->getPosition()));
							}
							else
							{
								items.emplace_back(item, enemyPtr->getPosition());
							}
						}
					}
				}
			}
		}

		// update the spells that require mana over time
		if (timeFrozen) {
			if (player.useMana(15.0f * dtAsSeconds))
				sound.playTimeStopActiveSound();
			else
			{
				sound.playTimeStopEndSound();
				sound.stopTimeStopActiveSound();
				timeFrozen = false;
			}
		}

		if (player.isPhasing())
		{
			if (player.useMana(20.0f * dtAsSeconds)) // use mana while phasing
			{
				// player is phasing
			}
			else
			{
				player.stopPhase();
				sound.playPhaseEndSound();
			}
		}

		filter.setOrigin(player.getPosition());
		filter.setPosition(player.getPosition().x, player.getPosition().y);

		// Make a note of the players new position
		Vector2f playerPosition(player.getCenter());

		// Make the view centre around the player				
		mainView.setCenter(player.getCenter().x, player.getCenter().y - 10);

		// Update any spells that are in-flight
		if (!timeFrozen) {
			for (int i = 0; i < 100; i++)
			{
				if (spells[i].isInFlight())
				{
					spells[i].update(dtAsSeconds, world.getNavBoxes(player.getChunk()));

					FloatRect spellBounds = spells[i].getSprite().getGlobalBounds();

					for (auto& enemyPtr : enemyArr)
					{
						if (!enemyPtr->isDead())
						{
							if (spells[i].getSprite().getGlobalBounds().intersects(enemyPtr->getHitBox()))
							{
								decal[currentDecal].spawn("bloodImpact", enemyPtr->getPosition().x, enemyPtr->getPosition().y);
								currentDecal++;
								// Apply damage from spell to enemy
								enemyPtr->setHealth(-spells[i].getSpellDamage());

								// Mark enemy as hit
								enemyPtr->setWasHit(true);

								// Stop the spell; Add check for piercing spells later
								spells[i].stop();

								// Play the sparks particle effect
								for (int i = 0; i < 100; i++)
								{
									if (!particles[i].isPlaying())
									{
										particles[i].play(enemyPtr->getCenter().x - 30, enemyPtr->getCenter().y - 30, 2);
										break;
									}
								}

								// Play hit sound
								sound.playHitSound();

								// This spell hit an enemy; stop checking other enemies
								// Add check for piercing spells later
								break;
							}
						}
					}
				}
			}

			for (int i = 0; i < 100; i++)
			{
				if (dragonSpells[i].isInFlight())
				{
					dragonSpells[i].update(dtAsSeconds, world.getNavBoxes(player.getChunk()));

					FloatRect spellBounds = dragonSpells[i].getSprite().getGlobalBounds();

					if (!player.isDead())
					{
						if (dragonSpells[i].getSprite().getGlobalBounds().intersects(player.getHitBox()))
						{
							decal[currentDecal].spawn("bloodImpact", player.getPosition().x, player.getPosition().y);
							currentDecal++;
							// Apply damage from spell to enemy
							//player.setHealth(-dragonSpells[i].getSpellDamage());
							player.hit(gameTimeTotal, -dragonSpells[i].getSpellDamage(), 1000);

							// Mark enemy as hit
							player.setWasHit(true);

							// Stop the spell; Add check for piercing spells later
							dragonSpells[i].stop();

							// Play the sparks particle effect
							for (int i = 0; i < 100; i++)
							{
								if (!particles[i].isPlaying())
								{
									particles[i].play(player.getCenter().x - 30, player.getCenter().y - 30, 2);
									break;
								}
							}

							// Play hit sound
							sound.playHitSound();

							// This spell hit an enemy; stop checking other enemies
							// Add check for piercing spells later
							break;
						}
					}
				}
			}
		}

		//update any particles that are active
		for (int i = 0; i < 101; i++)
		{
			if (particles[i].isPlaying())
			{
				particles[i].update(dtAsSeconds);
			}
		}

		//make the player's particles follow them
		if (particles[100].isPlaying())
		{
			particles[100].setPosition(player.getCenter().x - 30, player.getCenter().y - 30);
		}

		swordIcon.setTextureRect(player.getEquippedSword()->getTextureRect());
		swordIcon.setOrigin(swordIcon.getSize() / 2.f);

		wandIcon.setTextureRect(player.getEquippedWand()->getTextureRect());
		wandIcon.setOrigin(wandIcon.getSize() / 2.f);

		if (player.getCombatType() == Player::CombatType::Melee)
		{
			weaponIndicator.setPosition(swordBox.getPosition().x + 37.5, swordBox.getPosition().y - 25);
		}
		else
		{
			weaponIndicator.setPosition(wandBox.getPosition().x + 37.5, wandBox.getPosition().y - 25);
		}

		switch (player.getSpellType()) {
		case Player::SpellType::Fireball:
			spellIndicator.setPosition(spell1Icon.getPosition().x, spell1Icon.getPosition().y - 62.5);
			break;

		case Player::SpellType::Heal:
			spellIndicator.setPosition(spell2Icon.getPosition().x, spell2Icon.getPosition().y - 62.5);
			break;

		case Player::SpellType::FreezeTime:
			spellIndicator.setPosition(spell3Icon.getPosition().x, spell3Icon.getPosition().y - 62.5);
			break;

		case Player::SpellType::Phase:
			spellIndicator.setPosition(spell4Icon.getPosition().x, spell4Icon.getPosition().y - 62.5);
			break;
		}

		if (drawShop)
		{
			for (int i = 0; i < shopItems.size(); i++)
			{
				if (shopItems[i].getIcon().getGlobalBounds().contains(worldPos.x - 25, worldPos.y - 25) &&
					Mouse::isButtonPressed(Mouse::Left))
				{
					if (buyItem(i) == 1)
					{
						shopItems[i] = Item("null", Vector2f(0, 0));
					}
					else
					{
						// Handle other failed purchase cases here
					}
					break;
				}
			}

			// Check clicks on inventory items
			for (int i = 0; i < m_StoredItems.size(); i++)
			{
				if (m_StoredItems[i].getIcon().getGlobalBounds().contains(worldPos.x - 25, worldPos.y - 25) &&
					Mouse::isButtonPressed(Mouse::Left))
				{
					if (sellClock.getElapsedTime().asSeconds() > 0.25f && sellItem(i)) // weird solution to prevent multiple sells but it works
					{
						m_StoredItems[i] = Item("null", Vector2f(0, 0)); // empty original slot

					}
					sellClock.restart();
					break;
				}
			}
		}

		// Level up the player
		if (drawInventory && levelUp)
		{
			if (Mouse::isButtonPressed(Mouse::Left) && invHealthBar.getGlobalBounds().contains(Vector2f(worldPos.x - 25, worldPos.y - 25)))
			{
				player.upgradeHealth();
				levelUp = false;
			}
			else if (Mouse::isButtonPressed(Mouse::Left) && invStamBar.getGlobalBounds().contains(Vector2f(worldPos.x - 25, worldPos.y - 25)))
			{
				player.upgradeStamina();
				levelUp = false;
			}
			else if (Mouse::isButtonPressed(Mouse::Left) && invManaBar.getGlobalBounds().contains(Vector2f(worldPos.x - 25, worldPos.y - 25)))
			{
				player.upgradeMana();
				levelUp = false;
			}
		}
		// Drag items the player clicks on
		else if (drawInventory)
		{
			// If tutorial hasn't been started and player opens inventory, start tutorial
			if (tutorialStage == 0) {
				tutorialStage = 1;
				tutorialText.setString("Welcome to your inventory! Here you can manage your items and equipment. Drag the health potion onto the player and release to heal.");
				textBounds = tutorialText.getLocalBounds();
				tutorialText.setPosition(viewCentre.x - 200, 930);
			}


			playerInFrame = player.getSprite();
			playerInFrameHead = player.getHead();
			playerInFrameTorso = player.getTorso();
			playerInFramePants = player.getPants();
			playerInFrameShoes = player.getShoes();
			Vector2f framePos = Vector2f(viewCentre.x - 250, 340);
			playerInFrame.setPosition(framePos);
			playerInFrameHead.setPosition(framePos);
			playerInFrameTorso.setPosition(framePos);
			playerInFramePants.setPosition(framePos);
			playerInFrameShoes.setPosition(framePos);
			playerInFrame.setScale(Vector2f(2.25f, 2.25f));
			playerInFrameHead.setScale(Vector2f(2.25f, 2.25f));
			playerInFrameTorso.setScale(Vector2f(2.25f, 2.25f));
			playerInFramePants.setScale(Vector2f(2.25f, 2.25f));
			playerInFrameShoes.setScale(Vector2f(2.25f, 2.25f));


			bool draggingFromInventory = false;

			// Check clicks on inventory items
			for (int i = 0; i < m_StoredItems.size(); i++)
			{
				if (!m_StoredItems[i].isNull() && m_StoredItems[i].getIcon().getGlobalBounds().contains(worldPos.x - 25, worldPos.y - 25) &&
					Mouse::isButtonPressed(Mouse::Left) && !draggingItem)
				{
					clickedItem = m_StoredItems[i]; // copy to clickedItem

					itemLastIndex = i; // save original slot
					itemLastX = m_StoredItems[i].getIcon().getPosition().x;
					itemLastY = m_StoredItems[i].getIcon().getPosition().y;

					m_StoredItems[i] = Item("null", Vector2f(0, 0)); // empty original slot
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
				bool itemWasEquipped = false;

				// Try to drop in the first empty slot
				for (int i = 0; i < m_StoredItems.size(); i++)
				{
					if (m_StoredItems[i].isNull() &&
						clickedItem.getIcon().getGlobalBounds().intersects(emptyFrames[i].getGlobalBounds()))
					{
						m_StoredItems[i] = clickedItem;  // copy item into slot
						m_StoredItems[i].getIcon().setPosition(emptyFrames[i].getPosition().x, emptyFrames[i].getPosition().y); //set icon position to slot position
						clickedItem = Item("null", Vector2f(0, 0)); // empty clickedItem
						placed = true;
						break;
					}
				}

				// Try to equip as sword if dropped on sword slot
				if (clickedItem.getIcon().getGlobalBounds().intersects(weaponFrame.getGlobalBounds())
					&& clickedItem.getType() == Item::MeleeWeapon)
				{
					//unequip current melee weapon first
					for (int i = 0; i < m_StoredItems.size(); i++)
					{
						if (m_StoredItems[i].isEquipped() && m_StoredItems[i].getType() == Item::MeleeWeapon)
						{
							m_StoredItems[i].setEquipped(false);
							break;
						}
					}
					// equip new melee weapon
					if (player.equipWeapon(clickedItem.getName())) equippedSwordIcon.setTextureRect(clickedItem.getTextureRect());
					itemWasEquipped = true;
				}

				// placed is not being made true on purpose, the player will still need to carry the items they equip
				// we can change this later though

				// Try to equip as wand if dropped on wand slot
				if (clickedItem.getIcon().getGlobalBounds().intersects(wandFrame.getGlobalBounds())
					&& clickedItem.getType() == Item::MagicWeapon)
				{
					//unequip current magic weapon first
					for (int i = 0; i < m_StoredItems.size(); i++)
					{
						if (m_StoredItems[i].isEquipped() && m_StoredItems[i].getType() == Item::MagicWeapon)
						{
							m_StoredItems[i].setEquipped(false);
							break;
						}
					}
					// equip new magic weapon
					if (player.equipWeapon(clickedItem.getName())) equippedWandIcon.setTextureRect(clickedItem.getTextureRect());
					itemWasEquipped = true;
				}

				// Try to equip as head armour if dropped on head armour slot
				if (clickedItem.getIcon().getGlobalBounds().intersects(headArmourFrame.getGlobalBounds())
					&& clickedItem.getType() == Item::HeadArmour)
				{
					// unequip current head armour first
					for (int i = 0; i < m_StoredItems.size(); i++)
					{
						if (m_StoredItems[i].isEquipped() && m_StoredItems[i].getType() == Item::HeadArmour)
						{
							m_StoredItems[i].setEquipped(false);
							break;
						}
					}
					// equip new head armour
					if (player.equipArmour(clickedItem.getName()))
					{
						equippedHeadArmourIcon = clickedItem.getIcon();
						equippedHeadArmourIcon.setPosition(headArmourFrame.getPosition());
					}
					itemWasEquipped = true;
				}

				// Try to equip as chest armour if dropped on chest armour slot
				if (clickedItem.getIcon().getGlobalBounds().intersects(chestArmourFrame.getGlobalBounds())
					&& clickedItem.getType() == Item::ChestArmour)
				{
					// unequip current chest armour first
					for (int i = 0; i < m_StoredItems.size(); i++)
					{
						if (m_StoredItems[i].isEquipped() && m_StoredItems[i].getType() == Item::ChestArmour)
						{
							m_StoredItems[i].setEquipped(false);
							break;
						}
					}
					// equip new chest armour
					if (player.equipArmour(clickedItem.getName()))
					{
						equippedChestArmourIcon = clickedItem.getIcon();
						equippedChestArmourIcon.setPosition(chestArmourFrame.getPosition());
					}
					itemWasEquipped = true;
				}

				// Try to equip as trouser armour if dropped on trouser armour slot
				if (clickedItem.getIcon().getGlobalBounds().intersects(trousersArmourFrame.getGlobalBounds())
					&& clickedItem.getType() == Item::TrouserArmour)
				{
					// unequip current trouser armour first
					for (int i = 0; i < m_StoredItems.size(); i++)
					{
						if (m_StoredItems[i].isEquipped() && m_StoredItems[i].getType() == Item::TrouserArmour)
						{
							m_StoredItems[i].setEquipped(false);
							break;
						}
					}
					// equip new trouser armour
					if (player.equipArmour(clickedItem.getName()))
					{
						equippedTrousersArmourIcon = clickedItem.getIcon();
						equippedTrousersArmourIcon.setPosition(trousersArmourFrame.getPosition());
					}
					itemWasEquipped = true;
				}

				// Try to equip as shoe armour if dropped on shoe armour slot
				if (clickedItem.getIcon().getGlobalBounds().intersects(bootsArmourFrame.getGlobalBounds())
					&& clickedItem.getType() == Item::ShoeArmour)
				{
					// unequip current shoe armour first
					for (int i = 0; i < m_StoredItems.size(); i++)
					{
						if (m_StoredItems[i].isEquipped() && m_StoredItems[i].getType() == Item::ShoeArmour)
						{
							m_StoredItems[i].setEquipped(false);
							break;
						}
					}
					// equip new shoe armour
					if (player.equipArmour(clickedItem.getName()))
					{
						equippedShoeArmourIcon = clickedItem.getIcon();
						equippedShoeArmourIcon.setPosition(bootsArmourFrame.getPosition());
					}
					itemWasEquipped = true;
				}

				// Try to equip as neck armour if dropped on neck armour slot
				if (clickedItem.getIcon().getGlobalBounds().intersects(neckFrame.getGlobalBounds())
					&& clickedItem.getType() == Item::NeckArmour)
				{
					// unequip current neck armour first
					for (int i = 0; i < m_StoredItems.size(); i++)
					{
						if (m_StoredItems[i].isEquipped() && m_StoredItems[i].getType() == Item::NeckArmour)
						{
							m_StoredItems[i].setEquipped(false);
							break;
						}
					}
					// equip new neck armour
					if (player.equipArmour(clickedItem.getName()))
					{
						equippedNeckArmourIcon = clickedItem.getIcon();
						equippedNeckArmourIcon.setPosition(neckFrame.getPosition());
					}
					itemWasEquipped = true;
				}

				if (clickedItem.getIcon().getGlobalBounds().intersects(playerInFrame.getGlobalBounds()) && clickedItem.getType() == Item::Consumable)
				{
					cout << "Item used!";
					if (clickedItem.getName() == "Health_Potion" && player.getHealth() < player.getMaxHealth())
					{
						player.healHealth(clickedItem.getRestoreValue()); // heal health
						clickedItem = Item("null", Vector2f(0, 0));
						placed = true;
					}
					else if (clickedItem.getName() == "Mana_Potion" && player.getMana() < player.getMaxMana())
					{
						player.healMana(clickedItem.getRestoreValue()); // restore mana
						clickedItem = Item("null", Vector2f(0, 0));
						placed = true;
					}
					else if (clickedItem.getName() == "Stamina_Potion" && player.getStamina() < player.getMaxStamina())
					{
						player.healStamina(clickedItem.getRestoreValue()); // restore stamina
						clickedItem = Item("null", Vector2f(0, 0));
						placed = true;
					}

					tutorialStage = 2;
				}

				// If no slot found, return to original position
				if (!placed)
				{
					clickedItem.getIcon().setPosition(itemLastX, itemLastY);
					if (draggingItem && draggedIndex >= 0 && draggedIndex < m_StoredItems.size())
						m_StoredItems[draggedIndex] = clickedItem; // restore in original slot
					if (itemWasEquipped) m_StoredItems[draggedIndex].setEquipped(true);
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
		emptyHealthBar.setSize(Vector2f(200, 35));
		healthBar.setSize(Vector2f(200 * (player.getHealth() / player.getMaxHealth()), 35));
		healthBarContainer.setSize(Vector2f(230, 35));

		// Set size of the mana bar
		emptyManaBar.setSize(Vector2f(200, 35));
		manaBar.setSize(Vector2f(200 * (player.getMana() / player.getMaxMana()), 35));
		manaBarContainer.setSize(Vector2f(230, 35));

		// Set size of the Stamina bar
		emptyStaminaBar.setSize(Vector2f(200, 35));
		staminaBar.setSize(Vector2f(200 * (player.getStamina() / player.getMaxStamina()), 35));
		staminaBarContainer.setSize(Vector2f(230, 35));

		// Increment the amount of time since the last HUD update
		timeSinceLastUpdate += dt;
		// Increment the number of frames since the last HUD calculation
		framesSinceLastHUDUpdate++;
		// Calculate FPS every fpsMeasurementFrameInterval frames

		if (drawInventory) {
			// Update the kills text
			stringstream ssKillCount;
			ssKillCount << "Kills: " << player.getKillCount();
			killsText.setString(ssKillCount.str());

			// Update the gold text
			stringstream ssGoldCount;
			ssGoldCount << "Gold: " << player.getGold();
			goldCountText.setString(ssGoldCount.str());

			// Update the levels text
			stringstream ssLevels;
			ssLevels << "Level: " << player.getPlayerLevel();
			levelsText.setString(ssLevels.str());

			// Update the exp text
			stringstream ssExp;
			ssExp << "EXP: " << player.getExp() << " / " << "100";
			expText.setString(ssExp.str());

			stringstream ssHealthBar;
			ssHealthBar << int(player.getHealth()) << " / " << int(player.getMaxHealth());
			invHealthBarText.setString(ssHealthBar.str());
			textBounds = invHealthBarText.getLocalBounds();
			x = backgroundInvHealthBar.getPosition().x + (backgroundInvHealthBar.getSize().x / 2.f) - (textBounds.width / 2.f);
			y = backgroundInvHealthBar.getPosition().y + (backgroundInvHealthBar.getSize().y / 2.f) - (textBounds.height / 2.f);
			invHealthBarText.setPosition(x + 20, y);

			stringstream ssStamBar;
			ssStamBar << int(player.getStamina()) << " / " << int(player.getMaxStamina());
			invStamBarText.setString(ssStamBar.str());
			textBounds = invStamBarText.getLocalBounds();
			x = backgroundInvStamBar.getPosition().x + (backgroundInvStamBar.getSize().x / 2.f) - (textBounds.width / 2.f);
			y = backgroundInvStamBar.getPosition().y + (backgroundInvStamBar.getSize().y / 2.f) - (textBounds.height / 2.f);
			invStamBarText.setPosition(x + 20, y);

			stringstream ssManaBar;
			ssManaBar << int(player.getMana()) << " / " << int(player.getMaxMana());
			invManaBarText.setString(ssManaBar.str());
			textBounds = invManaBarText.getLocalBounds();
			x = backgroundInvManaBar.getPosition().x + (backgroundInvManaBar.getSize().x / 2.f) - (textBounds.width / 2.f);
			y = backgroundInvManaBar.getPosition().y + (backgroundInvManaBar.getSize().y / 2.f) - (textBounds.height / 2.f);
			invManaBarText.setPosition(x + 20, y);

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
				if (items[i].getName() == "Gold")
				{
					player.addGold(1);
					sound.playGoldSound();
					items.erase(items.begin() + i);
				}
				else
				{
					bool placed = false;
					for (size_t j = 0; j < m_StoredItems.size(); j++)
					{
						if (m_StoredItems[j].isNull())
						{
							sound.playCollectSound();
							m_StoredItems[j] = move(items[i]);
							m_StoredItems[j].getIcon().setPosition(emptyFrames[j].getPosition());
							currentItems++;
							items.erase(items.begin() + i);
							placed = true;
							break;
						}
					}
					if (!placed) ++i;
				}
			}
			else
			{
				++i;
			}
		}

		if (!sound.isSoundtrackPlaying()) {
			sound.playSoundtrack();
		}

		// Sets health to 0 if it goes below 0
		if (player.getHealth() <= 0) {
			player.setHealthValue(0);

			if (sound.isSoundtrackPlaying()) {
				sound.stopSoundtrack();
			}
			sound.playGameOverSound();
			gameOverText.setString("Game Over!");
			gameOverText.setFillColor(Color::Red);
			textBounds = gameOverText.getLocalBounds();
			viewCentre = hudView.getCenter();
			gameOverText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, viewCentre.y - gameOverText.getCharacterSize());

			gameOverText2.setString("Your family will be left unavenged.\nIgnis has won...");
			textBounds = gameOverText2.getLocalBounds();
			viewCentre = hudView.getCenter();
			gameOverText2.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, viewCentre.y + 100 - gameOverText2.getCharacterSize());

			state = State::GAME_OVER;
		}

		// Sets stamina to 0 if it goes below 0
		if (player.getStamina() < 0) {
			player.setStaminaValue(0);
		}

		// Sets mana to 0 if it goes below 0
		if (player.getMana() < 0) {
			player.setManaValue(0);
		}

	} // End of PLAYING state update

	if (state == MAIN_MENU) {

		if (currentDecal > 0) {
			for (int i = 0; i < sizeof(decal) / sizeof(decal[0]); i++) {
				decal[i] = Decal();
			}
		}

		currentDecal = 0;
	}

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

	// Stops sound track in options, main and controls menus
	if (state == State::MAIN_MENU || state == State::OPTIONS_MENU || state == State::CONTROLS_MENU || state == State::CREDITS_MENU)
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
	else if (state == State::PAUSED || state == State::MAIN_MENU || state == State::OPTIONS_MENU || state == State::GAME_OVER || state == State::VICTORY || state == State::CONTROLS_MENU)
	{
		window.setMouseCursorVisible(true);
		window.setMouseCursorGrabbed(false);
	}

	/*********************************************************************
						      Options State
    **********************************************************************/
	if (state == State::OPTIONS_MENU) {

		// Handle volume slider dragging
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
			player.createConfigFile(difficultyToString(difficulty), windowedMode, displayFps, Listener::getGlobalVolume(), vSync, fpsLimit);
			isDragging = false;
		}


		// Handle the display fps button changing colour based on boolean
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
}