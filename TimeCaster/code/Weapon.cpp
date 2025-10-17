#include "Weapon.h"
#include "TextureHolder.h"
using namespace sf;
using namespace std;

Weapon::Weapon(String type, Vector2f position)
	: Item(type, position) // call base constructor
{
	m_Name = type;

	if (m_Name == "null") // empty item for inventory
	{
		m_Value = 0;
		m_Sprite.setOrigin(5, 5);
		m_ItemType = null;

	}


	// Associate the texture with the sprite
	if (m_Name == "staff")
	{
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/health_pickup.png"));

		m_Value = 10;
		m_Damage = 5;

		m_Sprite.setOrigin(25, 25);
	}

	if (m_Name == "StartingSword")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(961, 896, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(961, 896, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		// How much is item worth
		m_Value = 5;
		m_Damage = 5;

		m_Sprite.setOrigin(5, 5);
		m_Icon.setScale(2.34, 2.34);

		m_ItemType = MeleeWeapon;


	}


	if (m_Name == "StartingWand")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(194, 864, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(194, 864, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		//	m_Sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

			// How much is item worth
		m_Value = 5;

		m_Icon.setScale(2.34, 2.34);
		//m_NullItem = false;

		m_Damage = 5;

		m_ItemType = MagicWeapon;
	}

	if (m_Name == "UpgradedSword")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(1184, 898, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(1184, 898, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		// How much is item worth
		m_Value = 10;
		m_Damage = 10;

		m_Sprite.setOrigin(5, 5);
		m_Icon.setScale(2.34, 2.34);

		m_ItemType = MeleeWeapon;


	}

	if (m_Name == "UpgradedWand")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(130, 864, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(130, 864, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		// How much is item worth
		m_Value = 10;
		m_Damage = 10;

		m_Sprite.setOrigin(5, 5);
		m_Icon.setScale(2.34, 2.34);

		m_ItemType = MagicWeapon;


	}
	
}

