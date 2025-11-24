#include "Loot.h"
#include <random>
using namespace sf;
using namespace std;

Loot::Loot()
{
	m_CommonItems = { "Health_Potion", "Mana_Potion", "Stamina_Potion" };

	m_RareItems = { "Chain_Mail", "Chain_Hood", "Soldiers_Helmet", 
		"Pirate's_Scimitar", "Long_Spear", "Short_Spear"};

	m_EpicItems = { "Steel_Plate_Helmet", "Steel_Plate_Chestplate",
	"Steel_Plate_Pants", "Steel_Plate_Boots", "Trident", "Steel_Mace"};

	m_LegendaryItems = { "Dragon_Spear" };
}

string Loot::getLoot(int maxRarity)
{
	// set maximum loot rarity
	if (maxRarity == 1)
	{
		maxRarity = 50;
	}
	else if (maxRarity == 2)
	{
		maxRarity = 80;
	}
	else if (maxRarity == 3)
	{
		maxRarity = 95;
	}
	else
	{
		maxRarity = 100;
	}

	int chance = rand() % 100;

	// 50% chance to spawn an item
	if (chance > 70)
	{
		int chance = rand() % maxRarity;

		// chance for common item
		if (chance < 50)
		{
			return getRandomItem(m_CommonItems);
		}
		// chance for rare item
		else if (chance < 80)
		{
			return getRandomItem(m_RareItems);
		}
		// chance for epic item
		else if (chance < 95)
		{
			return getRandomItem(m_EpicItems);
		}
		// chance for legendary item
		else
		{
			return getRandomItem(m_LegendaryItems);
		}
	}
	else
	{
		return "nothing";
	}
}

string Loot::getCommonItem()
{
	return getRandomItem(m_CommonItems);
}

string Loot::getRareItem()
{
	return getRandomItem(m_RareItems);
}

string Loot::getEpicItem()
{
	return getRandomItem(m_EpicItems);
}

string Loot::getLegendaryItem()
{
	return getRandomItem(m_LegendaryItems);
}

string Loot::getRandomItem(vector<string> itemList)
{
	int item = rand() % itemList.size();
	return itemList[item];
}