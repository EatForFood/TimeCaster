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

		if (world.getChunk(player.getChunk())->getChunkType() == "skeletonRuins")
		{
			filter.setFillColor(skeletonRuinsFilter);
		}
		else if (timeFrozen)
		{
			filter.setFillColor(timeFrozenFilter);
		}
		else {
			filter.setFillColor(defaultFilter);
		}

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
					else if (currentChunk && currentChunk->getChunkType() == "skeletonRuins")
					{
						window.draw(world.getBackground(i), &textureBackground3);
					}
					else
					{
						window.draw(world.getBackground(i), &textureBackground);
					}
				}
			}
		}

		// Ekey prompt in here temporarily
		// TODO: make ekey prompt draw when near / inside shop instead
		if (player.getInCell())
		{
			drawEKey = true;
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


		for (auto& item : items) {
			window.draw(item.getSprite());
		}

		if (!player.getInCell())
		{
			drawEKey = false;
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
			if (player.getRenderArea().intersects(enemies.getSprite().getGlobalBounds()) && !enemies.isDead()) 
			{
	
				drawables.emplace_back(enemies.getSprite().getGlobalBounds().top + enemies.getSprite().getGlobalBounds().height, enemies.getSpriteFromSheet()); // place enemy into drawables if in RenderArea
				if (enemies.isAttacking())
				{
					drawables.emplace_back(enemies.getSprite().getGlobalBounds().top + enemies.getSprite().getGlobalBounds().height + 0.05f, enemies.getWeapon());
				}
		
			}
		}

			drawables.emplace_back(player.getSprite().getGlobalBounds().top + player.getSprite().getGlobalBounds().height, player.getSpriteFromSheet()); // place player armour into drawables
			drawables.emplace_back(player.getSprite().getGlobalBounds().top + player.getSprite().getGlobalBounds().height + 0.01, player.getHead());
			drawables.emplace_back(player.getSprite().getGlobalBounds().top + player.getSprite().getGlobalBounds().height + 0.02, player.getTorso());
			drawables.emplace_back(player.getSprite().getGlobalBounds().top + player.getSprite().getGlobalBounds().height + 0.03, player.getPants());
			drawables.emplace_back(player.getSprite().getGlobalBounds().top + player.getSprite().getGlobalBounds().height + 0.04, player.getShoes());
		
		if (player.isAttacking())
		{
			drawables.emplace_back(player.getSprite().getGlobalBounds().top + player.getSprite().getGlobalBounds().height + 0.05, player.getWeapon());
		}

		for (auto& spell : spells)
		{
			if (spell.isInFlight())
			{
				drawables.emplace_back(spell.getSprite().getGlobalBounds().top + spell.getSprite().getGlobalBounds().height, spell.getSprite());
			}
		}

		for (auto& particle : particles)
		{
			if (particle.isPlaying())
			{
				drawables.emplace_back(particle.getSprite().getGlobalBounds().top + particle.getSprite().getGlobalBounds().height, particle.getSprite());
			}
		}
		
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

		/*
		enemyArr[0].drawDebugPath(window);
		*/

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
					else if (currentChunk && currentChunk->getChunkType() == "skeletonRuins")
					{
						window.draw(world.getForground(i), &textureBackground3);
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
		*/

		/*
		for (auto& nav : world.getNavBoxes(player.getChunk())) { // draw debug text showing tile location
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

		// Switch to the HUD view
		window.setView(hudView);
		if (drawEKey)
		{
			window.draw(eKey);
		}

		if (drawShop)
		{
			window.draw(filter);
			window.draw(darkInventoryBackground);
			window.draw(shopBackground);
			window.draw(shopText);

			// Draw the shop frames
			for (int i = 0; i < shopFrames.size(); i++) {
				window.draw(shopFrames[i]);
			}

			// Draw shop item icons
			for (auto& icons : shopItems) {
				window.draw(icons.getIcon());
			}

			for (auto& frame : emptyFrames) {
				window.draw(frame);
			}

			for (auto& icons : m_StoredItems) {
				window.draw(icons.getIcon());
			}

			window.setView(mainView);
			window.draw(spriteCursor);
			window.setView(hudView);
			displayShopTooltips();
		}
		else if (drawInventory) 
		{
			window.draw(filter);
			window.draw(darkInventoryBackground);
			window.draw(inventoryBackground);
			window.draw(playerFrame);
			window.draw(playerInFrame); 
			window.draw(playerInFrameHead);
			window.draw(playerInFrameTorso);
			window.draw(playerInFramePants);
			window.draw(playerInFrameShoes);
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
			window.draw(expText);
			window.draw(levelsText);
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
			for (auto& icons : m_StoredItems) {
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

			displayInventoryTooltips();
		}
		else {
			// Draw all the HUD elements
			window.draw(filter);
			window.draw(emptyHealthBar);
			window.draw(healthBar);
			window.draw(healthBarContainer);
			window.draw(emptyManaBar);
			window.draw(manaBar);
			window.draw(manaBarContainer);
			window.draw(emptyStaminaBar);
			window.draw(staminaBar);
			window.draw(staminaBarContainer);
			if (displayFps) {
				window.draw(fpsText);
			}
			window.draw(swordBox);
			window.draw(swordIcon);
			window.draw(wandBox);
			window.draw(wandIcon);
			window.draw(spellBox1);
			window.draw(spell1Icon);
			window.draw(spellBox2);
			window.draw(spell2Icon);
			window.draw(spellBox3);
			window.draw(spell3Icon);
			window.draw(spellBox4);
			window.draw(spell4Icon);
			window.draw(weaponIndicator);
			window.draw(spellIndicator);
			window.setView(mainView);
			window.draw(spriteCursor);
			window.setView(hudView);
		}
	}

	if (tutorialStage < 2) {
		window.draw(tutorialText);
	}

	if (levelUp)
	{
		window.draw(levelUpText);
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
		window.draw(vSyncButton);
		window.draw(vSyncButtonText);
		window.draw(debugModeButton);
		window.draw(debugModeButtonText);
		window.draw(textBox);
		window.draw(userInputText);
		window.draw(feedback);
	}

	if (state == State::STORY_INTRO)
	{
		window.clear();
		window.draw(spriteStoryIntro);
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

string Engine::cleanItemName(string itemName)
{
	for (int i = 0; i < itemName.length(); i++)
	{
		if (itemName[i] == '_')
		{
			itemName.replace(i, 1, " ");
		}
	}
	return itemName;
}

void Engine::displayInventoryTooltips()
{
	// Map mouse pixel coordinates to HUD view coordinates so bounds checks match HUD elements
	Vector2f hudPos = window.mapPixelToCoords(Mouse::getPosition(window), hudView);

	for (int i = 0; i < m_StoredItems.size(); i++)
	{
		if (m_StoredItems[i].getIcon().getGlobalBounds().contains(hudPos) && !Mouse::isButtonPressed(Mouse::Left) && !draggingItem)
		{
			Item::ItemType type = m_StoredItems[i].getType();

			string itemName = m_StoredItems[i].getName();

			// Remove all underscores from the name for display purposes

			window.setView(hudView);
			itemTooltipBackground.setPosition(m_StoredItems[i].getIcon().getPosition().x + 35, m_StoredItems[i].getIcon().getPosition().y - 40);
			window.draw(itemTooltipBackground);

			valueTooltipText.setString("Value: " + to_string(m_StoredItems[i].getValue()) + " Gold");
			valueTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 55);
			window.draw(valueTooltipText);

			itemTooltipName.setString(cleanItemName(itemName));

			if (type == Item::ItemType::MeleeWeapon || type == Item::ItemType::MagicWeapon) {

				statTooltipText.setString("Damage: " + to_string(m_StoredItems[i].getDamage()));

				if (itemTooltipName.getLocalBounds().width > statTooltipText.getLocalBounds().width && itemTooltipName.getLocalBounds().width > valueTooltipText.getLocalBounds().width)
				{
					itemTooltipBackground.setSize(Vector2f(itemTooltipName.getLocalBounds().width + 55, 100));
				}
				else if (statTooltipText.getLocalBounds().width > itemTooltipName.getLocalBounds().width && statTooltipText.getLocalBounds().width > valueTooltipText.getLocalBounds().width)
				{
					itemTooltipBackground.setSize(Vector2f(statTooltipText.getLocalBounds().width + 55, 100));
				}
				else
				{
					itemTooltipBackground.setSize(Vector2f(valueTooltipText.getLocalBounds().width + 55, 100));
				}

				itemTooltipName.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 15);
				statTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 37);

				window.draw(itemTooltipName);
				window.draw(statTooltipText);
			}
			else if (type == Item::ItemType::HeadArmour || type == Item::ItemType::ChestArmour || type == Item::ItemType::TrouserArmour ||
				type == Item::ItemType::ShoeArmour || type == Item::ItemType::NeckArmour) {

				statTooltipText.setString("Armour: " + to_string(m_StoredItems[i].getArmour()));

				if (itemTooltipName.getLocalBounds().width > statTooltipText.getLocalBounds().width && itemTooltipName.getLocalBounds().width > valueTooltipText.getLocalBounds().width)
				{
					itemTooltipBackground.setSize(Vector2f(itemTooltipName.getLocalBounds().width + 55, 100));
				}
				else if (statTooltipText.getLocalBounds().width > itemTooltipName.getLocalBounds().width && statTooltipText.getLocalBounds().width > valueTooltipText.getLocalBounds().width)
				{
					itemTooltipBackground.setSize(Vector2f(statTooltipText.getLocalBounds().width + 55, 100));
				}
				else
				{
					itemTooltipBackground.setSize(Vector2f(valueTooltipText.getLocalBounds().width + 55, 100));
				}

				itemTooltipName.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 15);
				statTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 37);

				window.draw(itemTooltipName);
				window.draw(statTooltipText);
			}
			else if (type == Item::ItemType::Consumable) {

				if (itemTooltipName.getLocalBounds().width > statTooltipText.getLocalBounds().width && itemTooltipName.getLocalBounds().width > valueTooltipText.getLocalBounds().width)
				{
					itemTooltipBackground.setSize(Vector2f(itemTooltipName.getLocalBounds().width + 55, 100));
				}
				else if (statTooltipText.getLocalBounds().width > itemTooltipName.getLocalBounds().width && statTooltipText.getLocalBounds().width > valueTooltipText.getLocalBounds().width)
				{
					itemTooltipBackground.setSize(Vector2f(statTooltipText.getLocalBounds().width + 55, 100));
				}
				else
				{
					itemTooltipBackground.setSize(Vector2f(valueTooltipText.getLocalBounds().width + 55, 100));
				}

				itemTooltipName.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 15);

				window.draw(itemTooltipName);

			}
		}
	}
	if (weaponFrame.getGlobalBounds().contains(hudPos) && !Mouse::isButtonPressed(Mouse::Left) && !draggingItem &&
		!player.getEquippedSword()->isNull())
	{
		window.setView(hudView);

		string itemName = player.getEquippedSword()->getName();

		itemTooltipName.setString(cleanItemName(itemName));

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
	if (wandFrame.getGlobalBounds().contains(hudPos) && !Mouse::isButtonPressed(Mouse::Left) && !draggingItem
		&& !player.getEquippedWand()->isNull())
	{
		window.setView(hudView);
		string itemName = player.getEquippedWand()->getName();

		itemTooltipName.setString(cleanItemName(itemName));

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
	if (headArmourFrame.getGlobalBounds().contains(hudPos) && !Mouse::isButtonPressed(Mouse::Left) && !draggingItem
		&& !player.getEquippedHeadArmour()->isNull())
	{
		window.setView(hudView);

		string itemName = player.getEquippedHeadArmour()->getName();

		itemTooltipName.setString(cleanItemName(itemName));

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
	if (chestArmourFrame.getGlobalBounds().contains(hudPos) && !Mouse::isButtonPressed(Mouse::Left) && !draggingItem
		&& !player.getEquippedChestArmour()->isNull())
	{
		window.setView(hudView);
		string itemName = player.getEquippedChestArmour()->getName();

		itemTooltipName.setString(cleanItemName(itemName));
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
	if (trousersArmourFrame.getGlobalBounds().contains(hudPos) && !Mouse::isButtonPressed(Mouse::Left) && !draggingItem
		&& !player.getEquippedTrouserArmour()->isNull())
	{
		window.setView(hudView);
		string itemName = player.getEquippedTrouserArmour()->getName();

		itemTooltipName.setString(cleanItemName(itemName));
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
	if (bootsArmourFrame.getGlobalBounds().contains(hudPos) && !Mouse::isButtonPressed(Mouse::Left) && !draggingItem
		&& !player.getEquippedShoeArmour()->isNull())
	{
		window.setView(hudView);
		string itemName = player.getEquippedShoeArmour()->getName();

		itemTooltipName.setString(cleanItemName(itemName));
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
	if (neckFrame.getGlobalBounds().contains(hudPos) && !Mouse::isButtonPressed(Mouse::Left) && !draggingItem
		&& !player.getEquippedNeckArmour()->isNull())
	{
		window.setView(hudView);
		string itemName = player.getEquippedNeckArmour()->getName();

		itemTooltipName.setString(cleanItemName(itemName));
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

void Engine::displayShopTooltips()
{

	for (int i = 0; i < shopItems.size(); i++)
	{
		if (shopItems[i].getIcon().getGlobalBounds().contains(worldPos) && !Mouse::isButtonPressed(Mouse::Left) && !draggingItem)
		{
			Item::ItemType type = shopItems[i].getType();

			string itemName = shopItems[i].getName();

			// Remove all underscores from the name for display purposes

			window.setView(hudView);
			itemTooltipBackground.setPosition(shopItems[i].getIcon().getPosition().x + 35, shopItems[i].getIcon().getPosition().y - 40);
			window.draw(itemTooltipBackground);

			valueTooltipText.setString("Value: " + to_string(shopItems[i].getValue()) + " Gold");
			valueTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 55);
			window.draw(valueTooltipText);

			itemTooltipName.setString(cleanItemName(itemName));

			if (type == Item::ItemType::MeleeWeapon || type == Item::ItemType::MagicWeapon) {

				statTooltipText.setString("Damage: " + to_string(shopItems[i].getDamage()));

				if (itemTooltipName.getLocalBounds().width > statTooltipText.getLocalBounds().width && itemTooltipName.getLocalBounds().width > valueTooltipText.getLocalBounds().width)
				{
					itemTooltipBackground.setSize(Vector2f(itemTooltipName.getLocalBounds().width + 55, 100));
				}
				else if (statTooltipText.getLocalBounds().width > itemTooltipName.getLocalBounds().width && statTooltipText.getLocalBounds().width > valueTooltipText.getLocalBounds().width)
				{
					itemTooltipBackground.setSize(Vector2f(statTooltipText.getLocalBounds().width + 55, 100));
				}
				else
				{
					itemTooltipBackground.setSize(Vector2f(valueTooltipText.getLocalBounds().width + 55, 100));
				}

				itemTooltipName.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 15);
				statTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 37);

				window.draw(itemTooltipName);
				window.draw(statTooltipText);
			}
			else if (type == Item::ItemType::HeadArmour || type == Item::ItemType::ChestArmour || type == Item::ItemType::TrouserArmour ||
				type == Item::ItemType::ShoeArmour || type == Item::ItemType::NeckArmour) {

				statTooltipText.setString("Armour: " + to_string(shopItems[i].getArmour()));

				if (itemTooltipName.getLocalBounds().width > statTooltipText.getLocalBounds().width && itemTooltipName.getLocalBounds().width > valueTooltipText.getLocalBounds().width)
				{
					itemTooltipBackground.setSize(Vector2f(itemTooltipName.getLocalBounds().width + 55, 100));
				}
				else if (statTooltipText.getLocalBounds().width > itemTooltipName.getLocalBounds().width && statTooltipText.getLocalBounds().width > valueTooltipText.getLocalBounds().width)
				{
					itemTooltipBackground.setSize(Vector2f(statTooltipText.getLocalBounds().width + 55, 100));
				}
				else
				{
					itemTooltipBackground.setSize(Vector2f(valueTooltipText.getLocalBounds().width + 55, 100));
				}

				itemTooltipName.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 15);
				statTooltipText.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 37);

				window.draw(itemTooltipName);
				window.draw(statTooltipText);
			}
			else if (type == Item::ItemType::Consumable) {

				if (itemTooltipName.getLocalBounds().width > statTooltipText.getLocalBounds().width && itemTooltipName.getLocalBounds().width > valueTooltipText.getLocalBounds().width)
				{
					itemTooltipBackground.setSize(Vector2f(itemTooltipName.getLocalBounds().width + 55, 100));
				}
				else if (statTooltipText.getLocalBounds().width > itemTooltipName.getLocalBounds().width && statTooltipText.getLocalBounds().width > valueTooltipText.getLocalBounds().width)
				{
					itemTooltipBackground.setSize(Vector2f(statTooltipText.getLocalBounds().width + 55, 100));
				}
				else
				{
					itemTooltipBackground.setSize(Vector2f(valueTooltipText.getLocalBounds().width + 55, 100));
				}

				itemTooltipName.setPosition(itemTooltipBackground.getPosition().x + 25, itemTooltipBackground.getPosition().y + 15);

				window.draw(itemTooltipName);

			}
		}
	}
}