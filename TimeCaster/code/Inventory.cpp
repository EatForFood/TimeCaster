#include "Engine.h"


void Engine::initializeInventory()
{
	int startX = viewCentre.x - 350;
	int startY = 600;

	// create empty item r
	emptyFrames.resize(16);
	for (int i = 0; i < emptyFrames.size(); i++) {
		emptyFrames[i].setTexture(&textureEmptyFrame);
		emptyFrames[i].setSize(Vector2f(75, 75));
		emptyFrames[i].setOrigin(emptyFrames[i].getSize() / 2.f);

		if (i != 0 && i % 8 == 0) {
			startY += 100;
			startX = viewCentre.x - 350;
		}

		emptyFrames[i].setPosition(startX, startY);
		startX += 100;
	}
	startX = viewCentre.x - 350;
	startY = 600;

	// Position icons for items that actually exist
	for (int i = 0; i < storedItems.size(); i++) {
		if (!storedItems[i].isNull()) {
			storedItems[i].getIcon().setPosition(emptyFrames[i].getPosition());
		}
	}
}

void Engine::moveDraggedIcon(Sprite& draggedIcon, Vector2f mousePos)
{
	float x = static_cast<float>(mousePos.x);
	float y = static_cast<float>(mousePos.y);
	draggedIcon.setPosition(x - 30, y - 30);
}

bool Engine::addItemToInventory(String itemType)
{
	//Check if item is a regular item
	Item item(itemType, Vector2f(0, 0));
	if (!item.isNull())
	{
		for (int i = 0; i < storedItems.size(); i++)
		{

			if (storedItems[i].isNull())
			{
				storedItems[i] = Item(itemType, Vector2f(0, 0));
				initializeInventory();
				return true;
			}
		}
		// No space in inventory
		return false;
	}

	// Check if item is an equipment
	Equipment equipment(itemType, Vector2f(0, 0));
	if (!equipment.isNull())
	{
		for (int i = 0; i < storedItems.size(); i++)
		{
			if (storedItems[i].isNull())
			{
				storedItems[i] = Equipment(itemType, Vector2f(0, 0));
				initializeInventory();
				return true;
			}
		}
		// No space in inventory
		return false;
	}

	// Check if Item is a weapon
	Weapon weapon(itemType, Vector2f(0, 0));
	if (!weapon.isNull())
	{
		for (int i = 0; i < storedItems.size(); i++)
		{
			if (storedItems[i].isNull())
			{
				storedItems[i] = Weapon(itemType, Vector2f(0, 0));
				initializeInventory();
				return true;
			}
		}
		// No space in inventory
		return false;
	}

	// Item does not exist
	return false;
}


// TODO: Move Inventory functions to a new Inventory cpp file

bool Engine::sellItem(int itemIndex)
{
	if (storedItems[itemIndex].isNull())
	{
		return false;
	}
	else
	{
		int goldToAdd = storedItems[itemIndex].getValue() * 0.75f; // Sell for 75 percent of the value, But not less than 1 gold
		if (goldToAdd < 1)
		{
			goldToAdd = 1;
		}
		player.addGold(goldToAdd);
		storedItems[itemIndex] = Item("null", Vector2f(0, 0));
		initializeInventory();
		return true;
	}

}
