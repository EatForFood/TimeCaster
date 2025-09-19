#include "Decal.h"
#include "TextureHolder.h"
using namespace sf;

void Decal::spawn(String type, float x, float y) // spawn decal of type at location
{

	if (type == "bloodImpact")
	{
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/bloodImpact.png"));
	}
	else if (type == "bulletImpact")
	{
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/bulletImpact.png"));
	}

	m_Sprite.setPosition(x - 25, y - 25);
	m_Spawned = true;
}

Sprite Decal::getSprite()
{
	return m_Sprite;
}



