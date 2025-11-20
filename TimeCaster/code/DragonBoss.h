#pragma once
#include <SFML/Graphics.hpp>
#include "enemy.h"

class DragonBoss : public Enemy
{
public:
	DragonBoss();

	void update(float elapsedTime, const Vector2f& playerPos, Chunk* chunk, int playerChunk, vector<NavBox> navBox) override;

	int getRangedDamage();

	void moveTextureRect();

	void spawn(const std::string& type, Vector2i position, int level);

	void updateTextRect();

	void setSpriteFromSheet(sf::IntRect textureBox);

	enum AttackState {
		Idle,
		Charge,
		Bite,
		Shoot,
		Rage
	};

	enum CombatType {
		Magic,
		Melee
	};

	AttackState getAttackState();
	CombatType getCombatType();

private:
	float START_HEALTH = 200;
	float START_SPEED = 25;
	
	int m_AttackChoice;
	int m_RangedDamage = 10;

	bool m_RequiresAiming = false;
	bool rageActivated = false;
	
	void bite();
	void rage();

	Vector2f m_TargetPosition;

	// Charging related variables & functions
	bool m_IsCharging = false;
	bool m_CanCharge = true;
	
	float m_ChargeDuration = 0.5f;
	float m_ChargeCooldown = 5.0f;

	Clock m_ChargeClock;
	Clock m_CooldownClock;

	void startCharge();
	void stopCharge();
	void charge(float elapsedTime);

	Vector2f m_ChargeDirection;

	AttackState state = AttackState::Idle;
	CombatType combatType = CombatType::Melee;
};