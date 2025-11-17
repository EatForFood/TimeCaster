#include "TextureHolder.h"
#include "DragonBoss.h"
#include <random>

DragonBoss::DragonBoss() {
    m_Health = START_HEALTH;
    m_MaxHealth = START_HEALTH;
    m_IsAttacking = false;
    m_Speed = START_SPEED;
    m_Damage = 20;
}

void DragonBoss::update(float elapsedTime, const Vector2f& playerPos, Chunk* chunk, int playerChunk, vector<NavBox> navBox) {

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