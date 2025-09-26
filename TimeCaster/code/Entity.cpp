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
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/trees/tree1.png"));
        m_Sprite.setOrigin(32,28);
    }

    if (type == "tree2")
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/trees/tree2.png"));
        m_Sprite.setOrigin(32, 28);
    }

    if (type == "tree3")
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/trees/tree3.png"));
        m_Sprite.setOrigin(32, 30);
    }

    if (type == "tree4")
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/trees/tree4.png"));
        m_Sprite.setOrigin(32, 48);
    }

    if (type == "tree5")
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/trees/tree5.png"));
        m_Sprite.setOrigin(32, 40);
    }

    if (type == "tree6")
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/trees/tree6.png"));
        m_Sprite.setOrigin(32, 40);
    }

    if (type == "tree7")
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/trees/tree7.png"));
        m_Sprite.setOrigin(96, 34);
    }

    if (type == "tree8")
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/trees/tree8.png"));
        m_Sprite.setOrigin(96, 54);
    }

    m_Position = Vector2f(x, y);
    m_Sprite.setPosition(m_Position);
}

Vector2f Entity::getPosition()
{
    return m_Position;
}

const Sprite Entity::getSprite()
{
	return m_Sprite;
}

