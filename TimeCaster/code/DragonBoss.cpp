#include "TextureHolder.h"
#include "DragonBoss.h"
#include <random>
#include <iostream>

DragonBoss::DragonBoss() 
{
    state = AttackState::Idle;
}

void DragonBoss::spawn(const std::string& type, Vector2i position, int level) {
    m_Speed = START_SPEED;
    m_Damage = 20;
    m_AttackDmg = 20;
    m_Sprite = Sprite(TextureHolder::GetTexture("graphics/enemies/flyingdragonedited.png"));

    m_Position.x = position.x;
    m_Position.y = position.y;

    // Set the origin of the sprite to the centre
    m_Sprite.setOrigin(spriteSize.x / 2.0f, spriteSize.y / 2.0f);
    m_Sprite.setScale(1, 1);

    // Hitbox centered on sprite
    m_Hitbox.left = m_Position.x - spriteSize.x / 2.0f;
    m_Hitbox.top = m_Position.y - spriteSize.y / 2.0f;
    m_Hitbox.width = spriteSize.x / 2;
    m_Hitbox.height = spriteSize.y / 2;

    m_Level = level;
    m_Type = type;
    m_Health = START_HEALTH * (1 + (m_Level - 1) * 0.1f); // Increase health by 10% per level
    m_MaxHealth = START_HEALTH * (1 + (m_Level - 1) * 0.1f); // Increase max health by 10% per level

    m_RenderArea = FloatRect(0, 0, 1920, 1080);

    m_Sprite.setPosition(m_Position);

    m_Animation_It_Limit = 3;
    m_Ani_Counter = 0;
}


void DragonBoss::update(float elapsedTime, const Vector2f& playerPos, Chunk* chunk, int playerChunk, vector<NavBox> navBox) 
{
    m_TimeElapsed = elapsedTime;
    
    if (state == AttackState::Idle) {
        m_AttackChoice = rand() % 3;

        while (m_AttackChoice == 0 && m_CooldownClock.getElapsedTime().asSeconds() < m_ChargeCooldown) {
            m_AttackChoice = rand() % 3;
        }

        Vector2f delta = playerPos - m_Position;

        if (std::abs(delta.x) > std::abs(delta.y)) {
            direction = (delta.x > 0) ? Vector2f(1, 0) : Vector2f(-1, 0);
        }
        else {
            direction = (delta.y > 0) ? Vector2f(0, -1) : Vector2f(0, 1);
        }

        switch (m_AttackChoice) {
        case 0:
            state = AttackState::Charge;
            m_TargetPosition = playerPos;
            charge();      
            cout << "Charging" << endl;
            break;

        case 1:
            state = AttackState::Bite;
            m_IsBiting = true;
            m_BiteClock.restart();
            m_Speed *= 3;
            bite(playerPos);
            cout << "Biting" << endl;
            break;

        case 2:
            state = AttackState::Shoot;
            cout << "Shooting" << endl;
            break;
        }
    }
    else {
        if (!m_IsCharging) {
            Vector2f delta = playerPos - m_Position;

            if (std::abs(delta.x) > std::abs(delta.y)) {
                direction = (delta.x > 0) ? Vector2f(1, 0) : Vector2f(-1, 0);
            }
            else {
                direction = (delta.y > 0) ? Vector2f(0, -1) : Vector2f(0, 1);
            }
        }
    }

    m_Sprite.setPosition(m_Position);

    updateTextRect();
    moveTextureRect();

    m_PositionLast = m_Position;

    // Hitbox centered on sprite
    m_Hitbox.left = m_Position.x - spriteSize.x / 2.0f;
    m_Hitbox.top = m_Position.y - spriteSize.y / 2.0f;
    m_Hitbox.width = spriteSize.x / 2;
    m_Hitbox.height = spriteSize.y / 2;

    if (m_Health < m_MaxHealth / 2 && !rageActivated) {
        state = AttackState::Rage;
        rage();
    }

    if (m_IsBiting) {
        bite(playerPos);

        if (m_BiteClock.getElapsedTime().asSeconds() >= m_BiteDuration) {
            m_Speed /= 3;
            m_IsBiting = false;
            state = AttackState::Idle;
        }
    }

    if (m_IsCharging) {
        charge();
    }

    if (!m_IsDead && m_Health <= 0) {
        m_IsDead = true;
        m_Sprite.setRotation(90);
        m_Sprite.setOrigin(spriteSize.x / 2.0f, spriteSize.y / 2.0f);
    }
}

void DragonBoss::startCharge() {
    m_Speed *= 6;
}

void DragonBoss::stopCharge() {
    m_Speed /= 6;
    state = AttackState::Idle;
    cout << "Set to idle" << endl;
}

void DragonBoss::charge()
{
    if (state == AttackState::Charge && !m_IsCharging)
    {
        m_IsCharging = true;
        m_ChargeDirection = m_TargetPosition - m_Position;
        float length = std::sqrt(m_ChargeDirection.x * m_ChargeDirection.x + m_ChargeDirection.y * m_ChargeDirection.y);
        if (length != 0) m_ChargeDirection /= length;
        m_ChargeClock.restart();
        startCharge();
    }

    // Move in the charge direction
    if (m_IsCharging)
    {
        m_Position += m_ChargeDirection * m_Speed * m_TimeElapsed;
        m_Sprite.setPosition(m_Position);

        // After 3 seconds, stop charging
        if (m_ChargeClock.getElapsedTime().asSeconds() >= m_ChargeDuration)
        {
            m_IsCharging = false;
            stopCharge();
        }
    }

    // Allows the dragon to charge again
    if (!m_CanCharge && m_CooldownClock.getElapsedTime().asSeconds() > m_ChargeCooldown) {
        m_CanCharge = true;
    }
}

void DragonBoss::bite(const Vector2f& playerPos)
{
    // Direction vector from enemy to player
    Vector2f diff = playerPos - m_Position;

    // Get distance
    float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    // Prevent division by zero
    if (length == 0)
        return;

    // Normalise
    sf::Vector2f dir = diff / length;

    // Move enemy
    m_Position += dir * m_Speed * m_TimeElapsed;

    // Update sprite
    m_Sprite.setPosition(m_Position);
}


void DragonBoss::rage() {
    m_Damage *= 2;
    m_AttackDmg *= 2;
    m_Speed *= 2;
    rageActivated = true;
}

void DragonBoss::setSpriteFromSheet(sf::IntRect textureBox) // set sprite
{
    sheetCoordinate = sf::Vector2i(textureBox.left, textureBox.top);
    spriteSize = sf::Vector2i(191, 158);
    if (textureBox.width > spriteSize.x)
    {
        m_Animation_It_Limit = textureBox.width / spriteSize.x;

        m_Horizontal = true;
    }
    else if (textureBox.height > spriteSize.y)
    {
        m_Animation_It_Limit = textureBox.height / spriteSize.y;
        m_Horizontal = false;
    }
    else
        throw std::logic_error("Animation bounding box must contain multiply sprites, setSprite(sf::IntRect )\n");

    m_Sprite.setTextureRect(sf::IntRect{ sheetCoordinate, spriteSize });
    m_Sprite.setOrigin(spriteSize.x / 2.0f, spriteSize.y / 2.0f);
}

void DragonBoss::updateTextRect()
{
    if (direction == Vector2f(0, 1)) // up
    {
        setSpriteFromSheet(IntRect(0, 0, 573, 158));
    }

    if (direction == Vector2f(0, -1)) // down
    {
        setSpriteFromSheet(IntRect(0, 316, 573, 474));
    }

    if (direction == Vector2f(1, 0)) // right
    {
        setSpriteFromSheet(IntRect(0, 158, 573, 316));
    }

    if (direction == Vector2f(-1, 0)) // left
    {
        setSpriteFromSheet(IntRect(0, 474, 573, 632));
    }
}

void DragonBoss::moveTextureRect() // animate sprite by moving texRect location
{
    // if the animation counter is greater than the animation limit go back in animation steps until reaching 1;
    if (m_Ani_Counter == m_Animation_It_Limit)
    {
        m_Ani_Counter = 0;
    }

    if (m_Horizontal) {

        m_Sprite.setTextureRect(sf::IntRect(sheetCoordinate + sf::Vector2i(spriteSize.x * m_Ani_Counter, 0), spriteSize));
    }
    else {
        m_Sprite.setTextureRect(sf::IntRect(sheetCoordinate + sf::Vector2i(0, spriteSize.y * m_Ani_Counter), spriteSize));
    }

    //increment animation counter to point to the next frame
    double timePerFrame;
    timePerFrame = 1.0 / 6.0;
    m_AnimationTimer = m_AnimationTimer + m_TimeElapsed;
    if (m_AnimationTimer > timePerFrame)
    {
        m_Ani_Counter++;
        m_AnimationTimer = 0;
    }
}