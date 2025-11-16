#include "Engine.h"

using namespace sf;
using namespace std;

void Engine::update()
{
	/*********************************************************************
							   UPDATE THE FRAME
	**********************************************************************/
	if (timeFrozen) {
		if (player.useMana(0.25f))
			sound.playTimeStopActiveSound();
		else
		{
			sound.playTimeStopEndSound();
			sound.stopTimeStopActiveSound();
			timeFrozen = false;
		}
	}

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
		if (state == State::PLAYING && !drawInventory) {
			player.update(dtAsSeconds, Mouse::getPosition(), world.getNavBoxes(player.getChunk()));
		}

		if (state == State::PLAYING && !drawInventory && !timeFrozen) {

			// Update the vector of enemies if within player's render area
			for (Enemy& enemies : enemyArr)
			{
				if (player.getRenderArea().intersects(enemies.getSprite().getGlobalBounds()))
				{
					if (!enemies.isDead())
					{
						enemies.update(dtAsSeconds, player.getPosition(), getCurrentChunk(enemies.getPosition().x, enemies.getPosition().y), player.getChunk(), world.getNavBoxes(enemies.getChunk()));

						for (int i = 0; i < world.getWorldSize(); i++)
						{
							if (collision.pointInShape(enemies.getPosition(), world.getChunkArea(i).getShape())) // find enemies current chunk
							{
								enemies.setChunk(i);
							}
						}

						if (player.getSprite().getGlobalBounds().intersects(enemies.getSprite().getGlobalBounds()))
						{
							enemies.Attack();
							// Handle player getting hit by enemy
							// TODO: Add weapon to enemy hands and use that instead
							if (player.getHitBox().intersects(enemies.getHitBox()) && enemies.isAttacking())
							{
								// Play the blood particle effect
								if (player.hit(gameTimeTotal, enemies.getAttackDamage(), 1000))
								{
									sound.playHitSound();
									particles[100].play(player.getCenter().x - 30, player.getCenter().y - 30, 1); 
									decal[currentDecal].spawn("bloodImpact", player.getPosition().x, player.getPosition().y);
									currentDecal++;
								}
							}
							if (player.getWeapon().getGlobalBounds().intersects(enemies.getHitBox()) && player.isAttacking() && !enemies.wasHit())
							{
								enemies.setHealth(-player.getAttackDamage());
								enemies.setWasHit(true);
								// Play the blood particle effect
								for (int i = 0; i < 100; i++)
								{
									if (!particles[i].isPlaying())
									{
										particles[i].play(enemies.getCenter().x - 30, enemies.getCenter().y - 30, 1);
										break;
									}
								}

								// Play enemy hit sound
								sound.playHitSound();
								decal[currentDecal].spawn("bloodImpact", enemies.getPosition().x, enemies.getPosition().y);
								currentDecal++;
							}
							else if (!player.isAttacking())
							{
								enemies.setWasHit(false);
							}
						}
					}
					if (enemies.isDead() && !enemies.isLooted())
					{
						if (player.reward(enemies.loot()))
						{
							drawInventory = true;
							levelUp = true;
							restockShop(player.getPlayerLevel());
						}

						// spawn some gold
						for (int i = 0; i < (rand() % 10); i++) {
							items.emplace_back("Gold", enemies.getPosition());
						}

						// potentially spawn random item
						string item = loot.getLoot(4);
						if (item != "nothing")
						{
							if (isEquipment(item))
							{
								items.emplace_back(Equipment(item, enemies.getPosition()));
							}
							else if (isWeapon(item))
							{
								items.emplace_back(Weapon(item, enemies.getPosition()));
							}
							else
							{
								items.emplace_back(item, enemies.getPosition());
							}
						}

					}
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
		if (!timeFrozen) {
			for (int i = 0; i < 100; i++)
			{
				if (spells[i].isInFlight())
				{
					spells[i].update(dtAsSeconds, world.getNavBoxes(player.getChunk()));

					FloatRect spellBounds = spells[i].getSprite().getGlobalBounds();

					for (Enemy& enemies : enemyArr)
					{
						if (!enemies.isDead())
						{
							if (spells[i].getSprite().getGlobalBounds().intersects(enemies.getHitBox()))
							{
								decal[currentDecal].spawn("bloodImpact", enemies.getPosition().x, enemies.getPosition().y);
								currentDecal++;
								// Apply damage from spell to enemy
								enemies.setHealth(-spells[i].getSpellDamage());
								//cout << "Enemy hit for " << spells[i].getSpellDamage() << " damage. Enemy health now " << enemies.getCurrentHP() << endl;

								// Mark enemy as hit
								enemies.setWasHit(true);

								// Stop the spell; Add check for piercing spells later
								spells[i].stop();

								// Play the sparks particle effect
								for (int i = 0; i < 100; i++)
								{
									if (!particles[i].isPlaying())
									{
										particles[i].play(enemies.getCenter().x - 30, enemies.getCenter().y - 30, 2);
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
					if (sellItem(i))
					{
						m_StoredItems[i] = Item("null", Vector2f(0, 0)); // empty original slot
					}
					break;
				}
			}

		}

		// Level up the player
		if (drawInventory && levelUp)
		{
			if (Mouse::isButtonPressed(Mouse::Left) && invHealthBar.getGlobalBounds().contains(worldPos))
			{
				player.upgradeHealth();
				levelUp = false;
			}
			else if (Mouse::isButtonPressed(Mouse::Left) && invStamBar.getGlobalBounds().contains(worldPos))
			{
				player.upgradeStamina();
				levelUp = false;
			}
			else if (Mouse::isButtonPressed(Mouse::Left) && invManaBar.getGlobalBounds().contains(worldPos))
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
				tutorialText.setPosition(viewCentre.x - (textBounds.width / 2.f) - textBounds.left, 900);
			}

			player.setSpritePosition(Vector2f(player.getPosition().x, player.getPosition().y));

			bool draggingFromInventory = false;
			//int draggedIndex = -1;

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
					m_StoredItems[itemLastIndex].setEquipped(true); 
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
					m_StoredItems[itemLastIndex].setEquipped(true);
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
					if (player.equipArmour(clickedItem.getName())) equippedHeadArmourIcon.setTextureRect(clickedItem.getTextureRect());
					m_StoredItems[itemLastIndex].setEquipped(true);
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
					if (player.equipArmour(clickedItem.getName())) equippedChestArmourIcon.setTextureRect(clickedItem.getTextureRect());
					m_StoredItems[itemLastIndex].setEquipped(true);
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
					if (player.equipArmour(clickedItem.getName())) equippedTrousersArmourIcon.setTextureRect(clickedItem.getTextureRect());
					m_StoredItems[itemLastIndex].setEquipped(true);
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
					if (player.equipArmour(clickedItem.getName())) equippedShoeArmourIcon.setTextureRect(clickedItem.getTextureRect());
					m_StoredItems[itemLastIndex].setEquipped(true);
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
					if (player.equipArmour(clickedItem.getName())) equippedNeckArmourIcon.setTextureRect(clickedItem.getTextureRect());
					m_StoredItems[itemLastIndex].setEquipped(true);
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

	} // End updating the scene

	if (state == MAIN_MENU) {
		
		if (currentDecal > 0) {
			for (int i = 0; i < sizeof(decal) / sizeof(decal[0]); i++) {
				decal[i] = Decal();
			}
		}
		
		currentDecal = 0;
	}
}