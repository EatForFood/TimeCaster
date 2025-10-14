#include "Weapon.h"
#include "TextureHolder.h"
using namespace sf;
using namespace std;

Weapon::Weapon(String type, Vector2f position)
	: Item(type, position) // call base constructor
{
	m_Type = type;

	// Associate the texture with the sprite
	if (m_Type == "staff")
	{
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/health_pickup.png"));

		m_Value = 10;
		m_Damage = 5;

		m_Sprite.setOrigin(25, 25);
	}

	if (m_Type == "StartingSword")
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


	}
}

