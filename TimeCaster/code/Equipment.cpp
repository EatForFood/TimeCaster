#include "Equipment.h"
#include "TextureHolder.h"

using namespace std;
using namespace sf;

Equipment::Equipment(String type, sf::Vector2f position) : Item(type, position) // call base constructor
{
	// Associate the texture with the sprite
	if (m_Name == "Family_Robe")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(96, 674, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(96, 674, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

		m_Icon.setScale(2.3125, 2.3125); // scale to (approx) 74x74
		m_Value = 4;
		m_Armour = 2;
		m_ItemType = ChestArmour;

		m_Sprite.setOrigin(25, 25);
	}
	else if (m_Name == "Leather_Cap")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(1058, 1408, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(1058, 1408, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);	

		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 10;
		m_Armour = 3;
		m_ItemType = HeadArmour;

		m_Sprite.setOrigin(25, 25);
	}
	else if (m_Name == "Family_Hood")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(1090, 1408, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(1090, 1408, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 2;
		m_Armour = 1;
		m_ItemType = HeadArmour;

		m_Sprite.setOrigin(25, 25);

	}
	else if (m_Name == "Leather_Chestplate")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(290, 672, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(290, 672, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 10;
		m_Armour = 3;
		m_ItemType = ChestArmour;

		m_Sprite.setOrigin(25, 25);
	}
	else if (m_Name == "Basic_Shoes")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(1920, 640, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(1920, 640, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 1;
		m_Armour = 0;
		m_ItemType = ShoeArmour;

		m_Sprite.setOrigin(25, 25);
	}
	else if (m_Name == "Robe_Leggings")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(1608, 1326, 16, 16)); // not the best trouser icon, probably change later, there aren't many on the sheet
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(1608, 1326, 16, 16));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		m_Icon.setScale(4.625, 4.625);
		m_Value = 2;
		m_Armour = 1;
		m_ItemType = TrouserArmour;
		m_Sprite.setOrigin(25, 25);
	}
	else if (m_Name == "Shield_Amulet")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(256, 640, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(256, 640, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 25;
		m_Armour = 8;
		m_ItemType = NeckArmour;
		m_Sprite.setOrigin(25, 25);
	}
	else if (m_Name == "Family_Locket")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(672, 640, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(672, 640, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 15; // easy to sell, not great armour, sentimental value
		// maybe set a bool if you have this equipped, and another if you sell/drop it, affecting some story elements?
		// also neck armour is probably gonna be the most valuable slot, it could be an interesting choice to make whether to unequip it or not
		m_Armour = 0;
		m_ItemType = NeckArmour;
		m_Sprite.setOrigin(25, 25);
	}
	else
	{
		m_Value = 0;
		m_Armour = 0;
		m_Sprite.setOrigin(5, 5);
		m_ItemType = null;
	}
}

int Equipment::getArmour()
{
	return m_Armour;
}

string Equipment::getName()
{
	return m_Name;
}