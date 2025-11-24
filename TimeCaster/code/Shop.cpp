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
		shopKeeperSetEmotion(3);
		shopText.setString(shopStream.str());
		return false;
	}
	else if (m_StoredItems[itemIndex].isSentimental() && !attemptedToSellSentimentalItem)
	{
		stringstream shopStream;
		shopStream << "That looks important to you.\nAre you sure you'd like to sell it?\nI can't guarantee you'll be able to get it back.";
		shopKeeperSetEmotion(3);
		shopText.setString(shopStream.str());
		attemptedToSellSentimentalItem = true;
		return false;
	}
	else
	{
		string itemName = shopItems[itemIndex].getName();
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
			shopStream << "Alright, if you're sure.\nHere's " << goldToAdd << " gold for your " << cleanItemName(itemName) << ".";
			shopKeeperSetEmotion(0);
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
			shopStream << "I'll take it!\nHere's " << goldToAdd << " gold for your " << cleanItemName(itemName) << ".";
			shopKeeperSetEmotion(1);
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
		shopKeeperSetEmotion(0);
		shopText.setString(shopStream.str());
		return 0; // 0 means item does not exist
	}
	else
	{
		int itemCost = shopItems[itemIndex].getValue();
		int playerGold = player.getGold();
		string itemName = shopItems[itemIndex].getName();
		string thatThose = "";
		if (shopItems[itemIndex].useThat())
		{
			thatThose = "that ";
		}
		else
		{
			thatThose = "those ";
		}
		if (playerGold >= itemCost)
		{

			if (player.addItemToInventory(itemName))
			{
				initializeInventory();
				player.addGold(-itemCost);
				stringstream shopStream;

				shopStream << "Thanks for the " << itemCost << " Gold!\nI hope you make good use of " << thatThose << cleanItemName(itemName) << "!";
				shopKeeperSetEmotion(1);
				shopText.setString(shopStream.str());
				return 1; // 1 means success
			}
			else
			{
				stringstream shopStream;
				shopStream << "I'd love to sell " << thatThose << "to you, but your inventory is full.";
				shopKeeperSetEmotion(3);
				shopText.setString(shopStream.str());
				return 2; // 2 means no space in inventory
			}
		}
		else
		{
			int difference = itemCost - playerGold;
			stringstream shopStream;
			shopStream << "Sorry, you're about " << difference << " Gold short.\nI can't sell you " << thatThose << cleanItemName(itemName) << ".";
			shopKeeperSetEmotion(3);
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
// 0-8 are equipment, 9-10 are potions, 11 is sold item / stamina potion
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
		shopItems[5] = Equipment("Amulet_of_Stamina", Vector2f(1050, 650));
		shopItems[6] = Equipment("Amulet_of_Shielding", Vector2f(1200, 650));
		shopItems[7] = Weapon("Short_Spear", Vector2f(1350, 650));
		break;
	case 8:
		shopItems[0] = Weapon("Reaper's_Scythe", Vector2f(300, 650));
		shopItems[1] = Weapon("Golden_Wand", Vector2f(450, 650));
		shopItems[2] = Equipment("Steel_Plate_Helmet", Vector2f(600, 650));
		shopItems[3] = Equipment("Steel_Plate_Chestplate", Vector2f(750, 950));
		shopItems[4] = Equipment("Steel_Plate_Pants", Vector2f(1050, 650));
		shopItems[5] = Equipment("Steel_Plate_Boots", Vector2f(1200, 650));
		shopItems[6] = Equipment("Amulet_of_Healing", Vector2f(1050, 650));
		shopItems[7] = Weapon("Long_Spear", Vector2f(1350, 650));
		break;
	
	case 10: 
		shopItems[7] = Weapon("Trident", Vector2f(300, 650));
		break;
	case 12:
		shopItems[7] = Weapon("Dragon_Spear", Vector2f(300, 650));
		break;
	}

	switch (rand() % 3)
	{
	case 0:
		shopItems[8] = Equipment("Amulet_of_Mana", Vector2f(900, 650));
		break;
	case 1:
		shopItems[8] = Equipment("Amulet_of_Stamina", Vector2f(1050, 650));
		break;
	case 2:
		shopItems[8] = Equipment("Amulet_of_Shielding", Vector2f(1200, 650));
		break;
	}

	

	// Always have potions in stock
	shopItems[9] = Item("Mana_Potion", Vector2f(1200, 650));
	shopItems[10] = Item("Health_Potion", Vector2f(1200, 650));
	//Stamina potion will overwrite sold item, intentionally
	shopItems[11] = Item("Stamina_Potion", Vector2f(1200, 650));

	


	initializeShop();
}

void Engine::shopKeeperSetEmotion(int emotionIndex)
{
	switch (emotionIndex)
	{
	case 0:
		shopKeeperSprite.setTexture(shopKeeperNeutral);
		break;
	case 1:
		shopKeeperSprite.setTexture(shopKeeperHappy);
		break;
	case 2:
		shopKeeperSprite.setTexture(shopKeeperTalking);
		break;
	case 3:
		shopKeeperSprite.setTexture(shopKeeperUnsure);
		break;
	default:
		shopKeeperSprite.setTexture(shopKeeperNeutral);
		break;
	}
}