#include "Engine.h"
#include "Player.h"


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
	for (int i = 0; i < m_StoredItems.size(); i++) {
		if (!m_StoredItems[i].isNull()) {
			m_StoredItems[i].getIcon().setPosition(emptyFrames[i].getPosition());
		}
	}
}


void Engine::moveDraggedIcon(Sprite& draggedIcon, Vector2f mousePos)
{
	float x = static_cast<float>(mousePos.x);
	float y = static_cast<float>(mousePos.y);
	draggedIcon.setPosition(x - 30, y - 30);
}

bool Player::addItemToInventory(String itemType)
{
	//Check if item is a regular item
	Item item(itemType, Vector2f(0, 0));
	if (!item.isNull())
	{
		for (int i = 0; i < m_StoredItems.size(); i++)
		{
			if (m_StoredItems[i].isNull())
			{
				m_StoredItems[i] = Item(itemType, Vector2f(0, 0));
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
		for (int i = 0; i < m_StoredItems.size(); i++)
		{
			if (m_StoredItems[i].isNull())
			{
				m_StoredItems[i] = Equipment(itemType, Vector2f(0, 0));
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
		for (int i = 0; i < m_StoredItems.size(); i++)
		{
			if (m_StoredItems[i].isNull())
			{
				m_StoredItems[i] = Weapon(itemType, Vector2f(0, 0));
				return true;
			}
		}
		// No space in inventory
		return false;
	}

	// Item does not exist
	return false;
}

void Engine::equipAllItems()
{

	// Equip melee weapon
	string meleeWeaponName = player.getEquippedSword()->getName();
	for (int i = 0; i < player.getStoredItems().size(); i++)
	{
		if (player.getStoredItems()[i].getName() == meleeWeaponName)
		{
			player.getStoredItems()[i].setEquipped(true);
			break;
		}
	}

	// Equip magic weapon
	string magicWeaponName = player.getEquippedWand()->getName();
	for (int i = 0; i < player.getStoredItems().size(); i++)
	{
		if (player.getStoredItems()[i].getName() == magicWeaponName)
		{
			player.getStoredItems()[i].setEquipped(true);
			break;
		}
	}

	// Equip head armour
	string headArmourName = player.getEquippedHeadArmour()->getName();
	for (int i = 0; i < player.getStoredItems().size(); i++)
	{
		if (player.getStoredItems()[i].getName() == headArmourName)
		{
			player.getStoredItems()[i].setEquipped(true);
			break;
		}
	}

	// Equip chest armour
	string chestArmourName = player.getEquippedChestArmour()->getName();
	for (int i = 0; i < player.getStoredItems().size(); i++)
	{
		if (player.getStoredItems()[i].getName() == chestArmourName)
		{
			player.getStoredItems()[i].setEquipped(true);
			break;
		}
	}

	// Equip trouser armour
	string trouserArmourName = player.getEquippedTrouserArmour()->getName();
	for (int i = 0; i < player.getStoredItems().size(); i++)
	{
		if (player.getStoredItems()[i].getName() == trouserArmourName)
		{
			player.getStoredItems()[i].setEquipped(true);
			break;
		}
	}
	// Equip shoe armour
	string shoeArmourName = player.getEquippedShoeArmour()->getName();
	for (int i = 0; i < player.getStoredItems().size(); i++)
	{
		if (player.getStoredItems()[i].getName() == shoeArmourName)
		{
			player.getStoredItems()[i].setEquipped(true);
			break;
		}
	}

	// Equip neck armour
	string neckArmourName = player.getEquippedNeckArmour()->getName();
	for (int i = 0; i < player.getStoredItems().size(); i++)
	{
		if (player.getStoredItems()[i].getName() == neckArmourName)
		{
			player.getStoredItems()[i].setEquipped(true);
			break;
		}
	}

}