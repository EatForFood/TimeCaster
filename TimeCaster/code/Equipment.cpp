#include "Equipment.h"
#include "TextureHolder.h"
using namespace std;
using namespace sf;

Equipment::Equipment(String type, sf::Vector2f position)
	: Item(type, position) // call base constructor
{
	// Associate the texture with the sprite


	if (m_Name == "StartingArmour")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(96, 674, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(96, 674, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

		m_Icon.setScale(2.34, 2.34);
		m_Value = 5;
		m_Armour = 1;
		m_ItemType = Armour;

		m_Sprite.setOrigin(25, 25);

	}
	else if (m_Name == "UpgradedArmour")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(128, 674, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(128, 674, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

		m_Icon.setScale(2.34, 2.34);
		m_Value = 10;
		m_Armour = 3;
		m_ItemType = Armour;

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
