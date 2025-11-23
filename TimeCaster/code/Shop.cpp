#include "Engine.h"
#include "Player.h"


bool Engine::sellItem(int itemIndex)
{
	if (m_StoredItems[itemIndex].isNull())
	{
		stringstream shopStream;
		shopStream << "You can't sell nothing.\nI already have enough of that.";
		shopText.setString(shopStream.str());
		return false;
	}
	else if (m_StoredItems[itemIndex].isEquipped())
	{
		stringstream shopStream;
		shopStream << "I can't buy equipped items.\nYou'll have to equip something else in that slot.";
		shopText.setString(shopStream.str());
		return false;
	}
	else if (m_StoredItems[itemIndex].isSentimental() && !attemptedToSellSentimentalItem)
	{
		stringstream shopStream;
		shopStream << "That looks important to you.\nAre you sure you'd like to sell it?\nI can't guarantee you'll be able to get it back.";
		shopText.setString(shopStream.str());
		attemptedToSellSentimentalItem = true;
		return false;
	}
	else
	{
		int goldToAdd = m_StoredItems[itemIndex].getValue() * 0.75f; // Sell for 75 percent of the value, But not less than 1 gold
		if (goldToAdd < 1)
		{
			goldToAdd = 1;
		}
		player.addGold(goldToAdd);
		bool textWasSet = false;
		if (m_StoredItems[itemIndex].isSentimental())
		{
			stringstream shopStream;
			shopStream << "Alright, if you're sure.\nHere's " << goldToAdd << " gold for it";
			shopText.setString(shopStream.str());
			textWasSet = true;
			player.setSoldSentimentalItem(true);
		}

		shopItems[11] = m_StoredItems[itemIndex];
		m_StoredItems[itemIndex] = Item("null", Vector2f(0, 0));
		initializeInventory();
		initializeShop();
		if (!textWasSet)
		{
			stringstream shopStream;
			shopStream << "I'll take it!\nHere's " << goldToAdd << " gold for it.";
			shopText.setString(shopStream.str());
		}

		return true;
	}
}

// 0 = item does not exist, 1 = success, 2 = no space in inventory, 3 = not enough gold
int Engine::buyItem(int itemIndex)
{
	if (shopItems[itemIndex].isNull())
	{
		stringstream shopStream;
		shopStream << "I don't have that item.";
		shopText.setString(shopStream.str());
		return 0; // 0 means item does not exist
	}
	else
	{
		int itemCost = shopItems[itemIndex].getValue();
		int playerGold = player.getGold();
		string itemName = shopItems[itemIndex].getName();
		if (playerGold >= itemCost)
		{

			if (player.addItemToInventory(itemName))
			{
				initializeInventory();
				player.addGold(-itemCost);
				stringstream shopStream;
				string thatThose = "";
				if (shopItems[itemIndex].useThat())
									{
					thatThose = "that ";
				}
				else
				{
					thatThose = "those ";
				}
				shopStream << "Thanks for the " << itemCost << " Gold!\nI hope you make good use of " << thatThose << cleanItemName(itemName) << "!";
				shopText.setString(shopStream.str());
				return 1; // 1 means success
			}
			else
			{
				stringstream shopStream;
				shopStream << "I'd love to sell that to you, but your inventory is full.";
				shopText.setString(shopStream.str());
				return 2; // 2 means no space in inventory
			}
		}
		else
		{
			int difference = itemCost - playerGold;
			stringstream shopStream;
			shopStream << "Sorry, you're about " << difference << " Gold short of affording that " << cleanItemName(itemName) << ".";
			shopText.setString(shopStream.str());
			return 3; // 3 means not enough gold
		}
	}
}


void Engine::initializeShop()
{
	int startX = viewCentre.x - 350;
	int startY = 200;

	// create empty item r
	shopFrames.resize(12);
	for (int i = 0; i < shopFrames.size(); i++) {
		shopFrames[i].setTexture(&textureEmptyFrame);
		shopFrames[i].setSize(Vector2f(75, 75));
		shopFrames[i].setOrigin(shopFrames[i].getSize() / 2.f);

		if (i != 0 && i % 6 == 0) {
			startY += 100;
			startX = viewCentre.x - 350;
		}

		shopFrames[i].setPosition(startX, startY);
		startX += 100;
	}
	startX = viewCentre.x - 350;
	startY = 600;

	// Position icons for items that actually exist
	for (int i = 0; i < shopItems.size(); i++) {
		if (!shopItems[i].isNull()) {
			shopItems[i].getIcon().setPosition(shopFrames[i].getPosition());
		}
	}
}
// 0 and 1 are weapons, 2-6 are equipment, 7 and 8 are unique random items, 9-10 are potions, 11 is sold item
void Engine::restockShop(int level)
{
	
	switch (level)
	{
	case 4:
		shopItems[0] = Weapon("Silver_Wand", Vector2f(300, 650));
		shopItems[1] = Weapon("Pirate's_Scimitar", Vector2f(450, 650));
		shopItems[2] = Equipment("Chain_Hood", Vector2f(600, 650));
		shopItems[3] = Equipment("Chain_Mail", Vector2f(750, 950));
		shopItems[4] = Equipment("Amulet_of_Mana", Vector2f(900, 650));
		shopItems[5] = Equipment("Amulet_of_Healing", Vector2f(1050, 650));
		shopItems[6] = Equipment("Amulet_of_Shielding", Vector2f(1200, 650));
		break;
	case 8:
		shopItems[0] = Weapon("Reaper's_Scythe", Vector2f(300, 650));
		shopItems[1] = Weapon("Golden_Wand", Vector2f(450, 650));
		shopItems[2] = Equipment("Steel_Plate_Helmet", Vector2f(600, 650));
		shopItems[3] = Equipment("Steel_Plate_Chestplate", Vector2f(750, 950));
		shopItems[4] = Equipment("Steel_Plate_Pants", Vector2f(1050, 650));
		shopItems[5] = Equipment("Steel_Plate_Boots", Vector2f(1200, 650));
		break;
	}

	// Additional potions as placeholders for shop iems 7 and 8

	//TODO: Add more unique items for shop slots 7 and 8 (sold items, more random items, etc), add more random items
	// Items 7 and 8 are random potions for now
	// TODO: Make last slot the sold item slot instead
	switch (rand() % 3)
	{
	case 0:
		shopItems[7] = Item("Health_Potion", Vector2f(1350, 650));
		break;
	case 1:
		shopItems[7] = Item("Mana_Potion", Vector2f(1350, 650));
		break;
	case 2:
		shopItems[7] = Item("Stamina_Potion", Vector2f(1350, 650));
		break;
	}
	// TODO: Change shop item 8 to something other than just potions
	switch (rand() % 3)
	{
	case 0:
		shopItems[8] = Item("Health_Potion", Vector2f(1350, 650));
		break;
	case 1:
		shopItems[8] = Item("Mana_Potion", Vector2f(1350, 650));
		break;
	case 2:
		shopItems[8] = Item("Stamina_Potion", Vector2f(1350, 650));
		break;
	}

	

	// Always have potions in stock
	shopItems[9] = Item("Stamina_Potion", Vector2f(1200, 650));
	shopItems[10] = Item("Health_Potion", Vector2f(1200, 650));
	//shopItems[11] = Item("Mana_Potion", Vector2f(1350, 650));

	


	initializeShop();
}
