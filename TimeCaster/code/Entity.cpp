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
        m_Sprite.setOrigin(32, 28);
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

    if (type == "bush1")
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/bushes/bush1.png"));
        m_Sprite.setOrigin(24, 28);
    }

    if (type == "bush2")
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/bushes/bush2.png"));
        m_Sprite.setOrigin(0, 0);
    }

    if (type == "bush3")
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/bushes/bush3.png"));
        m_Sprite.setOrigin(20, 54);
    }

    if (type == "bush4")
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/bush/trees/bush4.png"));
        m_Sprite.setOrigin(m_Sprite.getTextureRect().width / 2, 60);
    }

    if (type == "bush5")
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/bushes/bush5.png"));
        m_Sprite.setOrigin(m_Sprite.getTextureRect().width / 2, m_Sprite.getTextureRect().height);
    }

    if (type == "bush6")
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/bushes/bush6.png"));
        m_Sprite.setOrigin(m_Sprite.getTextureRect().width / 2, m_Sprite.getTextureRect().height);
    }

    if (type == "log1")
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/misc/log1.png"));
        m_Sprite.setOrigin(m_Sprite.getTextureRect().width / 2, m_Sprite.getTextureRect().height / 2);
    }

    if (type == "log2")
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/misc/log2.png"));
        m_Sprite.setOrigin(m_Sprite.getTextureRect().width / 2, m_Sprite.getTextureRect().height / 2);
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

