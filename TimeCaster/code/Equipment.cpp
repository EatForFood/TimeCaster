#include "Equipment.h"
#include "TextureHolder.h"
using namespace std;
using namespace sf;

Equipment::Equipment(String type, sf::Vector2f position)
	: Item(type, position) // call base constructor
{
	// Associate the texture with the sprite
	if (m_Name == "cloakBody")
	{
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/health_pickup.png"));

		// How much is pickup worth
		m_Value = 10;

		m_Sprite.setOrigin(25, 25);

	}

}

