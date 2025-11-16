#include "Engine.h"
#include "Player.h"



// TODO for sunday: Move shop items to their own shop file, reload shop item on level up, etc.
bool Engine::sellItem(int itemIndex)
{
	if (m_StoredItems[itemIndex].isNull())
	{
		stringstream shopStream;
		shopStream << "You can't sell nothing. I already have enough of that.";
		shopText.setString(shopStream.str());
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
		m_StoredItems[itemIndex] = Item("null", Vector2f(0, 0));
		initializeInventory();
		stringstream shopStream;
		shopStream << "I'll take it!\nHere's " << goldToAdd << " gold for it.";
		shopText.setString(shopStream.str());
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
				shopStream << "Thanks for the " << itemCost << " Gold!\nI hope you make good use of that " << cleanItemName(itemName) << "!";
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
