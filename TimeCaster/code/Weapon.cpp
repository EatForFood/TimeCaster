#include "Weapon.h"
#include "TextureHolder.h"
using namespace sf;
using namespace std;

Weapon::Weapon(String type, Vector2f position)
	: Item(type, position) // call base constructor
{
	m_Name = type;

	// Associate the texture with the sprite
	if (m_Name == "staff")
	{
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/health_pickup.png"));

		m_Value = 10;
		m_Damage = 5;

		m_Sprite.setOrigin(25, 25);
	}
	else if (m_Name == "Iron_Sword")
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
	else if (m_Name == "Wooden_Wand")
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
	else if (m_Name == "Scimitar")
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
	else if (m_Name == "Silver_Wand")
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
	else if (m_Name == "Golden_Wand")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(290, 894, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(290, 894, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		// How much is item worth
		m_Value = 20;
		m_Damage = 20;

		m_Sprite.setOrigin(5, 5);
		m_Icon.setScale(2.34, 2.34);

		m_ItemType = MagicWeapon;
	}
	else if (m_Name == "Scythe")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(1250, 896, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(1250, 896, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		// How much is item worth
		m_Value = 20;
		m_Damage = 20;

		m_Sprite.setOrigin(5, 5);
		m_Icon.setScale(2.34, 2.34);

		m_ItemType = MeleeWeapon;
	}
	else // empty item for inventory if none of the above match
	{
		m_Value = 0;
		m_Sprite.setOrigin(5, 5);
		m_ItemType = null;
	}
}

int Weapon::getDamage()
{
	return m_Damage;
}

string Weapon::getName()
{
	return m_Name;
}