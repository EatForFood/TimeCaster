#include "Weapon.h"
#include "TextureHolder.h"
using namespace sf;
using namespace std;

Weapon::Weapon(string type, Vector2f position)
	: Item(type, position) // call base constructor
{
	m_Name = type;

	if (m_Name == "Iron_Sword")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(960, 896, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(960, 896, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		// How much is item worth
		m_Value = 5;
		m_Damage = 5;

		m_Sprite.setOrigin(5, 5);
		m_Icon.setScale(2.34, 2.34);


		m_ItemType = MeleeWeapon;
		m_AnimationType = "slash";

		m_WeaponSize = 3;
		m_That = true;

	}
	else if (m_Name == "Wooden_Staff")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(194, 864, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(194, 864, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

		// How much is item worth
		m_Value = 5;

		m_Icon.setScale(2.34, 2.34);

		m_Damage = 5;

		m_ItemType = MagicWeapon;
		m_AnimationType = "thrust";
		m_WeaponSize = 1;
		m_That = true;
	}
	else if (m_Name == "Short_Spear")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(928, 896, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(928, 896, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

		// How much is item worth
		m_Value = 5;

		m_Icon.setScale(2.34, 2.34);

		m_Damage = 8;

		m_ItemType = MeleeWeapon;
		m_AnimationType = "thrust";
		m_WeaponSize = 1;
		m_That = true;
	}
	else if (m_Name == "Long_Spear")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(1376, 896, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(1376, 896, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

		// How much is item worth
		m_Value = 5;

		m_Icon.setScale(2.34, 2.34);

		m_Damage = 15;

		m_ItemType = MeleeWeapon;
		m_AnimationType = "thrust";
		m_WeaponSize = 3;
		m_That = true;
	}
	else if (m_Name == "Trident")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(1356, 896, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(1356, 896, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

		// How much is item worth
		m_Value = 5;

		m_Icon.setScale(2.34, 2.34);

		m_Damage = 20;

		m_ItemType = MeleeWeapon;
		m_AnimationType = "thrust";
		m_WeaponSize = 3;
		m_That = true;
	}
	else if (m_Name == "Dragon_Spear")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(1408, 896, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(1408, 896, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

		// How much is item worth
		m_Value = 5;

		m_Icon.setScale(2.34, 2.34);

		m_Damage = 30;

		m_ItemType = MeleeWeapon;
		m_AnimationType = "thrust";
		m_WeaponSize = 3;
		m_That = true;
		}
	else if (m_Name == "Pirate's_Scimitar")
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
		m_AnimationType = "slash";
		m_WeaponSize = 3;
		m_That = true;
	}
	else if (m_Name == "Steel_Mace")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(1184, 898, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(1184, 898, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		// How much is item worth
		m_Value = 10;
		m_Damage = 20;

		m_Sprite.setOrigin(5, 5);
		m_Icon.setScale(2.34, 2.34);

		m_ItemType = MeleeWeapon;
		m_AnimationType = "slash";
		m_WeaponSize = 2;
		m_That = true;
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
		m_AnimationType = "thrust";
		m_That = true;
	}
	else if (m_Name == "Golden_Wand")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(290, 864, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(290, 864, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		// How much is item worth
		m_Value = 20;
		m_Damage = 20;

		m_Sprite.setOrigin(5, 5);
		m_Icon.setScale(2.34, 2.34);

		m_ItemType = MagicWeapon;
		m_That = true;
	}
	else if (m_Name == "Reaper's_Scythe")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(1250, 896, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(1250, 896, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		m_Value = 20;
		m_Damage = 20;

		m_Sprite.setOrigin(5, 5);
		m_Icon.setScale(2.34, 2.34);

		m_ItemType = MeleeWeapon;
		m_That = true;
	}
	else // empty item for inventory if none of the above match
	{
		m_Value = 0;
		m_Sprite.setOrigin(5, 5);
		m_ItemType = null;
	}

	m_SecondsSinceSpawn = 0;
	m_Spawned = true;
	m_SecondsToLive = SECONDS_TO_DESPAWN;

	m_Sprite.setPosition(m_Position);

	m_MovementElapsed = 0;
	m_MovementDuration = 0.5 + (rand() % 50) / 100;

	// Random direction and speed
	float angle = (rand() % 360) * 3.14159 / 180;
	float speed = 20 + (rand() % 50);

	m_Velocity.x = cos(angle) * speed;
	m_Velocity.y = sin(angle) * speed;
}

int Weapon::getDamage()
{
	return m_Damage;
}

string Weapon::getName()
{
	return m_Name;
}

string Weapon::getAnimType()
{
	return m_AnimationType;
}

int Weapon::getWeaponSize()
{
	return m_WeaponSize;
}