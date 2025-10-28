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
					Chunk* currentChunk = world.getChunk(i);
					if (currentChunk && currentChunk->getChunkType() == "goblinVillage")
					{
						window.draw(world.getBackground(i), &textureBackground2);
					}
					else
					{
						window.draw(world.getBackground(i), &textureBackground);
					}
				}
			}
		}

		if (player.getInCell())
		{
			Chunk* currentChunk = world.getChunk(player.getChunk());
			if (currentChunk) {
				for (auto& cells : currentChunk->getCells()) {
					window.draw(cells.getBackground(), &textureBackground);
				}
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

		for (Enemy& enemies : enemyArr)
		{
			if (player.getRenderArea().intersects(enemies.getSprite().getGlobalBounds())) 
			{
				drawables.emplace_back(enemies.getSprite().getGlobalBounds().top + enemies.getSprite().getGlobalBounds().height, enemies.getSpriteFromSheet()); // place enemy into drawables if in RenderArea
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
				if (collision.distance(player.getCenter(), world.getChunkCenter(i)) < 2000) 
				{
					Chunk* currentChunk = world.getChunk(i);
					if (currentChunk && currentChunk->getChunkType() == "goblinVillage")
					{
						window.draw(world.getForground(i), &textureBackground2);
					}
					else
					{
						window.draw(world.getForground(i), &textureBackground);
					}	
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

			window.draw(wandFrame);
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

			// Drawing icons
			for (auto& icons : storedItems) {
				window.draw(icons.getIcon());
			}
			window.draw(equippedSwordIcon);
			window.draw(equippedWandIcon);
			window.draw(equippedHeadArmourIcon);
			window.draw(equippedChestArmourIcon);
			window.draw(equippedTrousersArmourIcon);
			window.draw(equippedShoeArmourIcon);
			window.draw(equippedNeckArmourIcon);
			window.draw(clickedItem.getIcon());
		
			window.setView(mainView);
			window.draw(spriteCursor);
			window.setView(hudView);

			for (int i = 0; i < storedItems.size(); i++)
			{
				if (storedItems[i].getIcon().getGlobalBounds().contains(worldPos) && !Mouse::isButtonPressed(Mouse::Left) && !draggingItem)
				{
					Item::ItemType type = storedItems[i].getType();

					window.setView(hudView);
					itemTooltipBackground.setPosition(storedItems[i].getIcon().getPosition().x + 35, storedItems[i].getIcon().getPosition().y - 40);
					window.draw(itemTooltipBackground);

					valueTooltipText.setString("Value: " + to_string(storedItems[i].getValue()) + " Gold");
					valueTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 55);
					window.draw(valueTooltipText);

					if (type == Item::ItemType::MeleeWeapon || type == Item::ItemType::MagicWeapon) {
						
						itemTooltipName.setString(storedItems[i].getName());
						itemTooltipBackground.setSize(Vector2f(itemTooltipName.getLocalBounds().width + 55, 100));

						statTooltipText.setString("Damage: " + to_string(storedItems[i].getDamage()));

						itemTooltipName.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 15);
						statTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 37);

						window.draw(itemTooltipName);
						window.draw(statTooltipText);
					}
					else if (type == Item::ItemType::HeadArmour || type == Item::ItemType::ChestArmour || type == Item::ItemType::TrouserArmour || 
						type == Item::ItemType::ShoeArmour || type == Item::ItemType::NeckArmour) {
						itemTooltipName.setString(storedItems[i].getName());
						itemTooltipBackground.setSize(Vector2f(itemTooltipName.getLocalBounds().width + 55, 100));

						statTooltipText.setString("Armour: " + to_string(storedItems[i].getArmour()));

						itemTooltipName.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 15);
						statTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 37);

						window.draw(itemTooltipName);
						window.draw(statTooltipText);
					}
					else if (type == Item::ItemType::Consumable) {
						
						itemTooltipName.setString(storedItems[i].getName());
						itemTooltipBackground.setSize(Vector2f(itemTooltipName.getLocalBounds().width + 55, 100));

						itemTooltipName.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 15);
				

						window.draw(itemTooltipName);

					}
				}
				

			}
			if (weaponFrame.getGlobalBounds().contains(worldPos) && !Mouse::isButtonPressed(Mouse::Left) && !draggingItem && 
				!player.getEquippedSword()->isNull())
			{
				window.setView(hudView);

				itemTooltipName.setString(player.getEquippedSword()->getName());

				itemTooltipBackground.setPosition(equippedSwordIcon.getPosition().x + 35, equippedSwordIcon.getPosition().y - 40);
				itemTooltipBackground.setSize(Vector2f(itemTooltipName.getLocalBounds().width + 55, 100));
				
				valueTooltipText.setString("Value: " + to_string(player.getEquippedSword()->getValue()) + " Gold");
				valueTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 55);
				
				itemTooltipName.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 15);
				
				statTooltipText.setString("Damage: " + to_string(player.getEquippedSword()->getDamage()));
				statTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 37);

				window.draw(itemTooltipBackground);
				window.draw(valueTooltipText);
				window.draw(itemTooltipName);
				window.draw(statTooltipText);
			}
			if (wandFrame.getGlobalBounds().contains(worldPos) && !Mouse::isButtonPressed(Mouse::Left) && !draggingItem
				&& !player.getEquippedWand()->isNull())
			{
				window.setView(hudView);
				itemTooltipName.setString(player.getEquippedWand()->getName());

				itemTooltipBackground.setPosition(equippedWandIcon.getPosition().x + 35, equippedWandIcon.getPosition().y - 40);
				itemTooltipBackground.setSize(Vector2f(itemTooltipName.getLocalBounds().width + 55, 100));
				
				valueTooltipText.setString("Value: " + to_string(player.getEquippedWand()->getValue()) + " Gold");
				valueTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 55);
				
				itemTooltipName.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 15);
				
				statTooltipText.setString("Damage: " + to_string(player.getEquippedWand()->getDamage()));
				statTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 37);
				
				window.draw(itemTooltipBackground);
				window.draw(valueTooltipText);
				window.draw(itemTooltipName);
				window.draw(statTooltipText);
			}
			if (headArmourFrame.getGlobalBounds().contains(worldPos) && !Mouse::isButtonPressed(Mouse::Left) && !draggingItem
				&& !player.getEquippedHeadArmour()->isNull())
			{
				window.setView(hudView);

				itemTooltipName.setString(player.getEquippedHeadArmour()->getName());

				itemTooltipBackground.setPosition(equippedHeadArmourIcon.getPosition().x + 35, equippedHeadArmourIcon.getPosition().y - 40);
				itemTooltipBackground.setSize(Vector2f(itemTooltipName.getLocalBounds().width + 55, 100));
				
				valueTooltipText.setString("Value: " + to_string(player.getEquippedHeadArmour()->getValue()) + " Gold");
				valueTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 55);
				
				itemTooltipName.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 15);
				
				statTooltipText.setString("Armour: " + to_string(player.getEquippedHeadArmour()->getArmour()));
				statTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 37);
				
				window.draw(itemTooltipBackground);
				window.draw(valueTooltipText);
				window.draw(itemTooltipName);
				window.draw(statTooltipText);
			}
			if (chestArmourFrame.getGlobalBounds().contains(worldPos) && !Mouse::isButtonPressed(Mouse::Left) && !draggingItem
				&& !player.getEquippedChestArmour()->isNull())
			{
				window.setView(hudView);
				itemTooltipName.setString(player.getEquippedChestArmour()->getName());
				itemTooltipBackground.setSize(Vector2f(itemTooltipName.getLocalBounds().width + 55, 100));
				itemTooltipBackground.setPosition(equippedChestArmourIcon.getPosition().x + 35, equippedChestArmourIcon.getPosition().y - 40);
				window.draw(itemTooltipBackground);
				valueTooltipText.setString("Value: " + to_string(player.getEquippedChestArmour()->getValue()) + " Gold");
				valueTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 55);
				
				itemTooltipName.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 15);
				
				statTooltipText.setString("Armour: " + to_string(player.getEquippedChestArmour()->getArmour()));
				statTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 37);

				window.draw(itemTooltipBackground);
				window.draw(valueTooltipText);
				window.draw(itemTooltipName);
				window.draw(statTooltipText);
			}
			if (trousersArmourFrame.getGlobalBounds().contains(worldPos) && !Mouse::isButtonPressed(Mouse::Left) && !draggingItem
				&& !player.getEquippedTrouserArmour()->isNull())
			{
				window.setView(hudView);
				itemTooltipName.setString(player.getEquippedTrouserArmour()->getName());
				itemTooltipBackground.setPosition(equippedTrousersArmourIcon.getPosition().x + 35, equippedTrousersArmourIcon.getPosition().y - 40);
				itemTooltipBackground.setSize(Vector2f(itemTooltipName.getLocalBounds().width + 55, 100));
				
				valueTooltipText.setString("Value: " + to_string(player.getEquippedTrouserArmour()->getValue()) + " Gold");
				valueTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 55);
				
				itemTooltipName.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 15);
				
				statTooltipText.setString("Armour: " + to_string(player.getEquippedTrouserArmour()->getArmour()));
				statTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 37);

				window.draw(itemTooltipBackground);
				window.draw(valueTooltipText);
				window.draw(itemTooltipName);
				window.draw(statTooltipText);
			}
			if (bootsArmourFrame.getGlobalBounds().contains(worldPos) && !Mouse::isButtonPressed(Mouse::Left) && !draggingItem 
				&& !player.getEquippedShoeArmour()->isNull())
			{
				window.setView(hudView);
				itemTooltipName.setString(player.getEquippedShoeArmour()->getName());
				itemTooltipBackground.setPosition(equippedShoeArmourIcon.getPosition().x + 35, equippedShoeArmourIcon.getPosition().y - 40);
				itemTooltipBackground.setSize(Vector2f(itemTooltipName.getLocalBounds().width + 55, 100));
				
				valueTooltipText.setString("Value: " + to_string(player.getEquippedShoeArmour()->getValue()) + " Gold");
				valueTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 55);
				
				itemTooltipName.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 15);
				
				statTooltipText.setString("Armour: " + to_string(player.getEquippedShoeArmour()->getArmour()));
				statTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 37);
				
				window.draw(itemTooltipBackground);
				window.draw(valueTooltipText);
				window.draw(itemTooltipName);
				window.draw(statTooltipText);
			}
			if (neckFrame.getGlobalBounds().contains(worldPos) && !Mouse::isButtonPressed(Mouse::Left) && !draggingItem
				&& !player.getEquippedNeckArmour()->isNull())
			{
				

				window.setView(hudView);
				itemTooltipName.setString(player.getEquippedNeckArmour()->getName());
				itemTooltipBackground.setPosition(equippedNeckArmourIcon.getPosition().x + 35, equippedNeckArmourIcon.getPosition().y - 40);
				itemTooltipBackground.setSize(Vector2f(itemTooltipName.getLocalBounds().width + 55, 100));
				
				valueTooltipText.setString("Value: " + to_string(player.getEquippedNeckArmour()->getValue()) + " Gold");
				valueTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 55);
				
				itemTooltipName.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 15);
				
				statTooltipText.setString("Armour: " + to_string(player.getEquippedNeckArmour()->getArmour()));
				statTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 37);
				
				window.draw(itemTooltipBackground);
				window.draw(valueTooltipText);
				window.draw(itemTooltipName);
				window.draw(statTooltipText);
			}
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