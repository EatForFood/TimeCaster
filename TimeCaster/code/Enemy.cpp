#include "Enemy.h"
#include "TextureHolder.h"

Enemy::Enemy() {
	m_Speed = START_SPEED;
	m_Health = START_HEALTH;
	m_MaxHealth = START_HEALTH;
}

void Enemy::spawn(IntRect arena, Vector2f resolution, int tileSize, String type, int level) {
	m_Hitbox.left = m_Position.x - 20;
	m_Hitbox.width = 40;
	m_Hitbox.top = m_Position.y - 20;
	m_Hitbox.height = 40;
	m_level = level;
	m_Type = type;

	// Copy the details of the arena to the player's m_Arena
	m_Arena.left = arena.left;
	m_Arena.width = arena.width;
	m_Arena.top = arena.top;
	m_Arena.height = arena.height;

	// Remember how big the tiles are in this arena
	m_TileSize = tileSize;

	// Strore the resolution for future use
	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;

	// Associate a texture with the sprite
	if (type == "Knight") {
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/player/playerWalk.png"));
		// Set the origin of the sprite to the centre, 
		m_Sprite.setOrigin(32, 32);
		m_Sprite.setScale(0.75, 0.75);
	}
	else if (type == "Goblin") {
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/player/playerWalk.png"));
		// Set the origin of the sprite to the centre, 
		m_Sprite.setOrigin(32, 32);
		m_Sprite.setScale(0.75, 0.75);
	}
}

void Enemy::update(float elapsedTime, Vector2i mousePosition) {
	m_TimeElapsed = elapsedTime;

	if (m_Type == "Knight") {
		
	}
	else if (m_Type == "Goblin") {
		
	}
}

float Enemy::getCurrentHP() {
	return m_Health;
}

// Function to heal the enemy over time when not being attacked
void Enemy::healOverTime() {
	if (m_Health < m_MaxHealth) {
		m_Health += 0.1;
	}
	else if (m_Health > m_MaxHealth) {
		m_Health = m_MaxHealth;
	}
}