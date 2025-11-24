#pragma once
#include <SFML/Graphics.hpp>
#include "enemy.h"

class DragonBoss : public Enemy
{
public:
	DragonBoss();

	void update(float elapsedTime, const Vector2f& playerPos, Chunk* chunk, int playerChunk, vector<NavBox> navBox) override;

	void moveTextureRect();

	void spawn(const std::string& type, Vector2i position, int level);

	void updateTextRect();

	void setSpriteFromSheet(sf::IntRect textureBox);

private:
	float START_HEALTH = 200;
	float START_SPEED = 25;
	
	int m_AttackChoice;

	bool m_RequiresAiming = false;
	bool rageActivated = false;
	
	void bite(const Vector2f& playerPos);
	void rage();

	Vector2f m_TargetPosition;

	// Charging related variables & functions
	bool m_IsCharging = false;
	bool m_CanCharge = true;
	
	float m_ChargeDuration = 3.0f;
	float m_ChargeCooldown = 5.0f;

	Clock m_ChargeClock;
	Clock m_CooldownClock;

	void startCharge();
	void stopCharge();
	void charge();

	Vector2f m_ChargeDirection;

	Clock m_BiteClock;
	float m_BiteDuration = 5.0f;
};