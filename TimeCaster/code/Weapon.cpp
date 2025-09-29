#include "Weapon.h"
#include "TextureHolder.h"

Weapon::Weapon(String type, sf::Vector2f position)
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
}

