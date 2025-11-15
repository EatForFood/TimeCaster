#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Loot
{
protected:

	vector<string> m_CommonItems;
	vector<string> m_RareItems;
	vector<string> m_EpicItems;
	vector<string> m_LegendaryItems;

public:
	Loot();

	string getLoot(int maxRarity);

	string getCommonItem();
	string getRareItem();
	string getEpicItem();
	string getLegendaryItem();

	string getRandomItem(vector<string> itemList);

};

