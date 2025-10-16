#include "Engine.h"

using namespace sf;
using namespace std;

void Engine::draw()
{
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
		if (!player.getInCell())
		{
			for (int i = 0; i < world.getWorldSize(); i++)
			{
				if (collision.distance(player.getCenter(), world.getChunkCenter(i)) < 2000) { // check player distance to chunk
					window.draw(world.getBackground(i), &textureBackground);
				}
			}
		}

		if (player.getInCell())
		{
			for (auto& cells : world.getChunk(player.getChunk()).getCells()) {
				window.draw(cells.getBackground(), &textureBackground);
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

		if (!player.getInCell())
		{
			for (int i = 0; i < world.getWorldSize(); i++)
			{
				if (collision.distance(player.getCenter(), world.getChunkCenter(i)) < 2000) {
					for (auto& entity : world.getEntities(i)) {
						if (player.getRenderArea().intersects(entity.getSprite().getGlobalBounds())) {
							drawables.emplace_back(entity.getSprite().getGlobalBounds().top + entity.getSprite().getGlobalBounds().height, entity.getSprite());
						}
					}
				}
			}
		}

		for (int i = 0; i < size(enemyArr); ++i)
		{
			if (player.getRenderArea().intersects(enemyArr[i].getSprite().getGlobalBounds())) {
				drawables.emplace_back(enemyArr[i].getSprite().getGlobalBounds().top + enemyArr[i].getSprite().getGlobalBounds().height, enemyArr[i].getSpriteFromSheet()); // place enemy into drawables if in RenderArea
			}
		}

		drawables.emplace_back(player.getSprite().getGlobalBounds().top + player.getSprite().getGlobalBounds().height, player.getSpriteFromSheet()); // place player armour into drawables
		drawables.emplace_back(player.getSprite().getGlobalBounds().top + player.getSprite().getGlobalBounds().height + 0.01, player.getHead());
		drawables.emplace_back(player.getSprite().getGlobalBounds().top + player.getSprite().getGlobalBounds().height + 0.02, player.getTorso());
		drawables.emplace_back(player.getSprite().getGlobalBounds().top + player.getSprite().getGlobalBounds().height + 0.03, player.getPants());
		drawables.emplace_back(player.getSprite().getGlobalBounds().top + player.getSprite().getGlobalBounds().height + 0.04, player.getShoes());


		// Sort by y position (smaller y values come first)
		sort(drawables.begin(), drawables.end(), [](const DrawableItem& a, const DrawableItem& b)
			{
				return a.y < b.y;
			}
		);

		// Draw in sorted order
		for (auto& item : drawables) {
			window.draw(item.sprite);
		}

		drawables.clear();

		if (!player.getInCell())
		{
			for (int i = 0; i < world.getWorldSize(); i++)
			{
				if (collision.distance(player.getCenter(), world.getChunkCenter(i)) < 2000) {
					window.draw(world.getForground(i), &textureBackground);
				}
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
		//window.draw(spriteCursor);

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

			window.draw(ringFrame);
			for (auto& frame : emptyFrames) {
				window.draw(frame);
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

			// draw icons last 

			for (auto& icons : storedItems) {
				window.draw(icons.getIcon());
			}
			window.draw(clickedItem.getIcon());
			window.draw(equippedWeaponIcon);
			window.setView(mainView);
			window.draw(spriteCursor);
			window.setView(hudView);

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
			window.setView(mainView);
			window.draw(spriteCursor);
			window.setView(hudView);
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
}