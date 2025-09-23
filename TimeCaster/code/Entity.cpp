#include "Entity.h"
#include "TextureHolder.h"
#include "Player.h"
using namespace sf;

Entity::Entity()
{
    m_Sprite = Sprite(TextureHolder::GetTexture("graphics/landscape.png"));
}

void Entity::spawn(String type, float x, float y)
{
    if (type == "tree1")
    {
        setSpriteFromSheet(768, 128, 64, 128);
        m_Sprite.setOrigin(32,48);
    }

    if (type == "tree2")
    {
        setSpriteFromSheet(768, 192, 64, 128);
        m_Sprite.setOrigin(32, 48);
    }

    if (type == "tree3")
    {
        setSpriteFromSheet(830, 0, 64, 182);
        m_Sprite.setOrigin(32, 118);
    }

    if (type == "tree4")
    {
        setSpriteFromSheet(884, 64, 64, 128);
        m_Sprite.setOrigin(32, 64);
    }

    if (type == "tree5")
    {
        setSpriteFromSheet(884, 128, 64, 128);
        m_Sprite.setOrigin(32, 64);
    }

    if (type == "tree6")
    {
        setSpriteFromSheet(884, 192, 64, 128);
        m_Sprite.setOrigin(32, 64);
    }

    if (type == "tree7")
    {
        setSpriteFromSheet(884, 256, 192, 128);
        m_Sprite.setOrigin(96, 112);
    }

    if (type == "tree8")
    {
        setSpriteFromSheet(884, 448, 192, 128);
        m_Sprite.setOrigin(96, 112);
    }

    m_Position = Vector2f(x, y);
    m_Sprite.setPosition(m_Position);
}

Vector2f Entity::getPosition()
{
    return m_Position;
}

Sprite Entity::getSprite()
{
	return m_Sprite;
}

void Entity::setSpriteFromSheet(int row, int col, int frameWidth, int frameHeight)
{
    m_Sprite.setTextureRect(sf::IntRect(
        col,   // x offset
        row,  // y offset
        frameWidth,         // width
        frameHeight         // height
    ));
}

void Entity::update(float elapsedTime, Player player, Vector2i mousePosition)
{


}
