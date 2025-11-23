#include "Equipment.h"
#include "TextureHolder.h"

using namespace std;
using namespace sf;

Equipment::Equipment(string type, sf::Vector2f position) : Item(type, position) // call base constructor
{
	if (m_Name == "Family_Hood")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(1090, 1408, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(1090, 1408, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 2;
		m_Armour = 0;
		m_ItemType = HeadArmour;

		m_Sprite.setOrigin(25, 25);
		m_That = true;

	}
	else if (m_Name == "Family_Robe")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(96, 674, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(96, 674, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

		m_Icon.setScale(2.3125, 2.3125); // scale to (approx) 74x74
		m_Value = 4;
		m_Armour = 0;
		m_ItemType = ChestArmour;
		m_That = true;


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
		m_Armour = 0;
		m_ItemType = TrouserArmour;
		m_Sprite.setOrigin(25, 25);
		m_That = false;
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
		m_That = false;
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
		m_Value = 5;
		m_Armour = 8;
		m_ItemType = HeadArmour;

		m_Sprite.setOrigin(25, 25);
		m_That = true;
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
		m_Value = 8;
		m_Armour = 10;
		m_ItemType = ChestArmour;

		m_Sprite.setOrigin(25, 25);
		m_That = true;
	}
	else if (m_Name == "Leather_Leggings")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(1608, 1326, 16, 16)); // not the best trouser icon, probably change later, there aren't many on the sheet
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(1608, 1326, 16, 16));
		m_Icon.setColor(Color::Red);
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		m_Icon.setScale(4.625, 4.625);
		m_Value = 8;
		m_Armour = 5;
		m_ItemType = TrouserArmour;
		m_Sprite.setOrigin(25, 25);
		m_That = false;
	}
	else if (m_Name == "Leather_Boots")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(1954, 640, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(1954, 640, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 4;
		m_Armour = 4;
		m_ItemType = ShoeArmour;

		m_Sprite.setOrigin(25, 25);
		m_That = false;
	}
	else if (m_Name == "Steel_Plate_Helmet")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/player/armour/icon/"+m_Name+".png"));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/player/armour/icon/" + m_Name + ".png"));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 20;
		m_Armour = 15;

		m_ItemType = HeadArmour;

		m_Sprite.setOrigin(25, 25);
		m_That = true;
	}
	else if (m_Name == "Steel_Plate_Chestplate")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/player/armour/icon/" + m_Name + ".png"));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/player/armour/icon/" + m_Name + ".png"));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 35;
		m_Armour = 40;

		m_ItemType = ChestArmour;

		m_Sprite.setOrigin(25, 25);
		m_That = true;
	}
	else if (m_Name == "Steel_Plate_Pants")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/player/armour/icon/" + m_Name + ".png"));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/player/armour/icon/" + m_Name + ".png"));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 20;
		m_Armour = 25;

		m_ItemType = TrouserArmour;

		m_Sprite.setOrigin(25, 25);
		m_That = false;
	}
	else if (m_Name == "Steel_Plate_Boots")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/player/armour/icon/" + m_Name + ".png"));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/player/armour/icon/" + m_Name + ".png"));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 10;
		m_Armour = 20;

		m_ItemType = ShoeArmour;

		m_Sprite.setOrigin(25, 25);
		m_That = true;
	}
	else if (m_Name == "Chain_Mail")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/player/armour/icon/" + m_Name + ".png"));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/player/armour/icon/" + m_Name + ".png"));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 35;
		m_Armour = 15;

		m_ItemType = ChestArmour;

		m_Sprite.setOrigin(25, 25);
		m_That = true;
	}
	else if (m_Name == "Chain_Hood")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/player/armour/icon/" + m_Name + ".png"));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/player/armour/icon/" + m_Name + ".png"));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 20;
		m_Armour = 10;

		m_ItemType = HeadArmour;

		m_Sprite.setOrigin(25, 25);
		m_That = true;
	}
	else if (m_Name == "Soldiers_Helmet")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/player/armour/icon/" + m_Name + ".png"));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/player/armour/icon/" + m_Name + ".png"));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 10;
		m_Armour = 10;

		m_ItemType = HeadArmour;

		m_Sprite.setOrigin(25, 25);
		m_That = true;
	}
	else if (m_Name == "Amulet_of_Shielding")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(256, 640, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(256, 640, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 25;
		m_Armour = 10;

		m_ItemType = NeckArmour;
		m_Sprite.setOrigin(25, 25);
		m_That = true;
	}
	else if (m_Name == "Amulet_of_Healing")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(480, 640, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(480, 640, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 25;
		m_Armour = 0;

		m_ItemType = NeckArmour;
		m_Sprite.setOrigin(25, 25);
		m_That = true;
		}
	else if (m_Name == "Amulet_of_Mana")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Sprite.setTextureRect(IntRect(640, 640, 32, 32));
		m_Icon.setTexture(TextureHolder::GetTexture("graphics/items/DungeonCrawl_ProjectUtumnoTileset.png"));
		m_Icon.setTextureRect(IntRect(640, 640, 32, 32));
		FloatRect bounds = m_Icon.getLocalBounds();
		m_Icon.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
		m_Icon.setScale(2.3125, 2.3125);
		m_Value = 25;
		m_Armour = 0;

		m_ItemType = NeckArmour;
		m_Sprite.setOrigin(25, 25);
		m_That = true;
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
		m_Value = 15;
		m_Armour = 0;
		m_Sentimental = true;
		m_ItemType = NeckArmour;

		m_Sprite.setOrigin(25, 25);
		m_That = true;
	}
	else
	{
		m_Value = 0;
		m_Armour = 0;
		m_Sprite.setOrigin(5, 5);
		m_ItemType = null;
	}

	m_SecondsSinceSpawn = 0;
	m_Spawned = true;
	m_SecondsToLive = SECONDS_TO_DESPAWN;

	m_Sprite.setScale(Vector2f(0.5, 0.5));
	m_Sprite.setPosition(m_Position);

	m_MovementElapsed = 0;
	m_MovementDuration = 0.5 + (rand() % 50) / 100;

	// Random direction and speed
	float angle = (rand() % 360) * 3.14159 / 180;
	float speed = 20 + (rand() % 50);

	m_Velocity.x = cos(angle) * speed;
	m_Velocity.y = sin(angle) * speed;
}

int Equipment::getArmour()
{
	return m_Armour;
}

string Equipment::getName()
{
	return m_Name;
}