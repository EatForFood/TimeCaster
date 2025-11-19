#include "TextureHolder.h"
#include "DragonBoss.h"
#include <random>
#include <iostream>

DragonBoss::DragonBoss() 
{
   
}

void DragonBoss::spawn(const std::string& type, Vector2i position, int level) {
    m_IsAttacking = false;
    m_Speed = START_SPEED;
    m_Damage = 20;
    m_Sprite = Sprite(TextureHolder::GetTexture("graphics/enemies/flying_dragon-red.png"));

    m_Hitbox.left = m_Position.x - 20;
    m_Hitbox.width = 40;
    m_Hitbox.top = m_Position.y - 20;
    m_Hitbox.height = 40;
    m_Level = level;
    m_Type = type;
    m_Health = START_HEALTH * (1 + (m_Level - 1) * 0.1f); // Increase health by 10% per level
    m_MaxHealth = START_HEALTH * (1 + (m_Level - 1) * 0.1f); // Increase max health by 10% per level

    // Set the origin of the sprite to the centre
    m_Sprite.setOrigin(32, 32);
    m_Sprite.setScale(1, 1);

    m_RenderArea = FloatRect(0, 0, 1920, 1080);

    m_Position.x = position.x;
    m_Position.y = position.y;
    m_Sprite.setPosition(m_Position);

    m_Animation_It_Limit = 3;
    m_Ani_Counter = 0;
}

void DragonBoss::update(float elapsedTime, const Vector2f& playerPos, Chunk* chunk, int playerChunk, vector<NavBox> navBox) 
{
    moveTextureRect(elapsedTime);
    updateTextRect();
    
    if (!m_IsAttacking) {
        m_AttackChoice = rand() % 3;

        while (m_AttackChoice == 0 && m_CooldownClock.getElapsedTime().asSeconds() < m_ChargeCooldown) {
            m_AttackChoice = rand() % 3;
        }

        switch (m_AttackChoice) {
        case 0:
            state = attackState::Charge;
            combatType = combatType::Melee;
            m_TargetPosition = playerPos;
            charge(elapsedTime);
            break;

        case 1:
            state = attackState::Bite;
            combatType = combatType::Melee;
            bite();
            break;

        case 2:
            state = attackState::Shoot;
            combatType = combatType::Magic;
            break;
        }
    }

    if (m_Health < m_MaxHealth / 2 && !rageActivated) {
        state = attackState::Rage;
        rage();
    }

    if (state == attackState::Bite)
    {
        Vector2f delta = playerPos - m_Position;
        float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);

        if (distance <= 2.0f) {
            stopMoving();
            bite();
            return;
        }

        // Move horizontally
        if (delta.x > 0) { 
            moveRight(); 
            stopLeft(); 
        }
        else if (delta.x < 0) { 
            moveLeft(); 
            stopRight(); 
        }

        // Move vertically
        if (delta.y > 0) { 
            moveDown(); 
            stopUp(); 
        }
        else if (delta.y < 0) { 
            moveUp(); 
            stopDown(); 
        }
    }
}

void DragonBoss::startCharge() {
    m_Speed *= 2;
}

void DragonBoss::stopCharge() {
    m_Speed /= 2;
    m_IsAttacking = false;
}

void DragonBoss::charge(float elapsedTime)
{
    if (m_CanCharge && !m_IsCharging)
    {
        m_IsCharging = true;
        m_CanCharge = false;
        m_IsAttacking = true;

        // Compute the charge vector once
        m_ChargeDirection = m_TargetPosition - m_Position;
        float length = std::sqrt(m_ChargeDirection.x * m_ChargeDirection.x + m_ChargeDirection.y * m_ChargeDirection.y);
        
        if (length != 0) {
            m_ChargeDirection /= length;
        }

        m_ChargeClock.restart();
        m_CooldownClock.restart();
        startCharge();
    }

    // Move in the charge direction
    if (m_IsCharging)
    {
        m_Position += m_ChargeDirection * m_Speed * elapsedTime;
    }
    
    // After 500ms stop charging
    if (m_IsCharging && m_ChargeClock.getElapsedTime().asSeconds() > m_ChargeDuration) {
        m_IsCharging = false;
        stopCharge();
    }

    // Allows the dragon to charge again
    if (!m_CanCharge && m_CooldownClock.getElapsedTime().asSeconds() > m_ChargeCooldown) {
        m_CanCharge = true;
    }
}

void DragonBoss::bite()
{
    // Play bite animation
    // Do damage to player
}

void DragonBoss::rage() {
    m_Damage *= 2;
    m_Speed *= 2;
    rageActivated = true;
}

int DragonBoss::getRangedDamage() {
    return m_RangedDamage;
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

void DragonBoss::moveTextureRect(float elapsedTime) // animate sprite by moving texRect location
{
    // if the animation counter is greater than the animation limit go back in animation steps until reaching 1;
    if (!animatingBackwards && m_Ani_Counter >= m_Animation_It_Limit)
    {
        animatingBackwards = true;
        m_Ani_Counter = m_Animation_It_Limit - 1;
    }

    if (animatingBackwards && m_Ani_Counter <= 0)
    {
        animatingBackwards = false;
        m_Ani_Counter = 1;
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
        if (!animatingBackwards) {
            m_Ani_Counter++;
        }
        else {
            m_Ani_Counter--;
        }

        m_AnimationTimer = 0;
    }

    cout << animatingBackwards << m_Ani_Counter << endl;
}