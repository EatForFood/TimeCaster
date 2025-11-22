#include "Entity.h"
#include "TextureHolder.h"
#include "Player.h"
using namespace sf;

Entity::Entity()
{
    m_Sprite = Sprite(TextureHolder::GetTexture("graphics/landscape.png"));
}

void Entity::spawn(int type, float x, float y)
{
    if (type == 1)
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/trees/tree1.png"));
        m_Sprite.setOrigin(32, 28);
    }

    if (type == 2)
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/trees/tree2.png"));
        m_Sprite.setOrigin(32, 28);
    }

    if (type == 3)
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/trees/tree3.png"));
        m_Sprite.setOrigin(32, 28);
    }

    if (type == 4)
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/trees/tree4.png"));
        m_Sprite.setOrigin(32, 28);
    }

    if (type == 7)
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/trees/tree7.png"));
        m_Sprite.setOrigin(32, 48);
    }

    if (type == 6)
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/trees/tree6.png"));
        m_Sprite.setOrigin(32, 28);
    }

    if (type == 5)
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/trees/tree5.png"));
        m_Sprite.setOrigin(96, 54);
    }

    if (type == 8)
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/trees/tree8.png"));
        m_Sprite.setOrigin(96, 36);
    }

    if (type == 11)
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/bushes/bush1.png"));
        m_Sprite.setOrigin(24, 28);
    }

    if (type == 12)
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/bushes/bush2.png"));
        m_Sprite.setOrigin(0, 0);
    }

    if (type == 13)
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/bushes/bush3.png"));
        m_Sprite.setOrigin(20, 54);
    }

    if (type == 14)
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/bush/bushes/bush4.png"));
        m_Sprite.setOrigin(m_Sprite.getTextureRect().width / 2, 60);
    }

    if (type == 15)
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/bushes/bush5.png"));
        m_Sprite.setOrigin(m_Sprite.getTextureRect().width / 2, m_Sprite.getTextureRect().height);
    }

    if (type == 16)
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/bushes/bush6.png"));
        m_Sprite.setOrigin(m_Sprite.getTextureRect().width / 2, m_Sprite.getTextureRect().height);
    }

    if (type == 21)
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/misc/log1.png"));
        m_Sprite.setOrigin(m_Sprite.getTextureRect().width / 2, m_Sprite.getTextureRect().height / 2);
    }

    if (type == 22)
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/misc/log2.png"));
        m_Sprite.setOrigin(m_Sprite.getTextureRect().width / 2, m_Sprite.getTextureRect().height / 2);
    }

    if (type == 31)
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/misc/pillar.png"));
        m_Sprite.setOrigin(m_Sprite.getTextureRect().width / 2, m_Sprite.getTextureRect().height / 2);
    }

    if (type == 32)
    {
        m_Sprite = Sprite(TextureHolder::GetTexture("graphics/foliage/misc/statue.png"));
        m_Sprite.setOrigin(m_Sprite.getTextureRect().width / 2, m_Sprite.getTextureRect().height / 2);
    }
    if (type == 33)
    {
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/shopSign.png")); 
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

