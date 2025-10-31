#include <iostream>
#include "Enemy.h"
#include "TextureHolder.h"
#include "CollisionDetection.h"
#include "Pathfinder.h"
#include "Chunk.h"

using namespace std;
using namespace sf;

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
	m_Level = level;
	m_Type = type;

	// Copy the details of the arena to the enemy's m_Arena
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
	}
	else if (type == "Goblin") {
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/enemies/goblin.png"));
	}
	else if (type == "Skeleton") {
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/enemies/skeleton.png"));
		m_Speed = 50;
		m_Health = 50;
		m_AttackDmg = 20;
	}

	// Set the origin of the sprite to the centre, 
	m_Sprite.setOrigin(32, 32);
	m_Sprite.setScale(0.75, 0.75);

	m_RenderArea = FloatRect(0, 0, 1920, 1080);

	m_Position.x = 100;
	m_Position.y = 100;
}

void Enemy::moveLeft()
{
	m_MoveLeft = true;
}

void Enemy::moveRight()
{
	m_MoveRight = true;
}

void Enemy::moveUp()
{
	m_MoveUp = true;
}

void Enemy::moveDown()
{
	m_MoveDown = true;
}

void Enemy::stopLeft()
{
	m_MoveLeft = false;
}

void Enemy::stopRight()
{
	m_MoveRight = false;
}

void Enemy::stopUp()
{
	m_MoveUp = false;
}

void Enemy::stopDown()
{
	m_MoveDown = false;
}

void Enemy::stopMoving() {
	m_MoveLeft = false;
	m_MoveRight = false;
	m_MoveUp = false;
	m_MoveDown = false;
}

void Enemy::revertPosition() {
	setPosition(m_PositionLast);
}

bool Enemy::isEnemyMoving() {
	if (m_IsMoving) {
		return true;
	}

	return false;
}

void Enemy::update(float elapsedTime, const Vector2f& playerPos, Chunk* chunk) {

	m_TimeElapsed = elapsedTime;

	// Compute direction vector from enemy to player
	Vector2f toPlayer = playerPos - m_Position;

	// Compute the length (distance)
	float length = sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

	// Avoid division by zero
	if (length != 0)
	{
		// Normalize the vector
		Vector2f playerDirection = toPlayer / length;

		// Move enemy toward player
		m_PositionLast = m_Position;
		m_Position += playerDirection * m_Speed * elapsedTime;
	}

	// If your sprite uses m_Position for rendering:
	m_Sprite.setPosition(m_Position);

	// Calculate the angle between mouse and center of screen
	float angle = atan2(playerPos.y - m_Position.y, playerPos.x - m_Position.x) * 180 / 3.141;

	if (angle < 0) angle += 360;

	if (angle >= 45 && angle < 135)
	{
		// facing down
		direction = Vector2f(0, -1);

	}
	else if (angle >= 135 && angle < 225)
	{
		// facing left
		direction = Vector2f(-1, 0);
	}
	else if (angle >= 225 && angle < 315)
	{
		// facing up
		direction = Vector2f(0, 1);
	}
	else
	{
		// facing right
		direction = Vector2f(1, 0);
	}

	updateTextRect();
	moveTextureRect();

	if (m_Health < 0)
	{
		m_IsDead = true;
		m_Sprite.setRotation(90);
		m_Sprite.setOrigin(32, 56);
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

void Enemy::setChunk(int chunk)
{
	m_Chunk = chunk;
}

int Enemy::getChunk()
{
	return m_Chunk;
}

FloatRect Enemy::getRenderArea()
{
	return m_RenderArea;
}

float Enemy::getDamage()
{
	return m_Damage;
}

void Enemy::updateTextRect()
{
	if (direction == Vector2f(0, 1)) // up
	{
		setSpriteFromSheet(IntRect(0, 0, 576, 64), 64);
	}

	if (direction == Vector2f(0, -1)) // down
	{
		setSpriteFromSheet(IntRect(0, 128, 576, 64), 64);
	}

	if (direction == Vector2f(1, 0)) // right
	{
		setSpriteFromSheet(IntRect(0, 192, 576, 64), 64);
	}

	if (direction == Vector2f(-1, 0)) // left
	{
		setSpriteFromSheet(IntRect(0, 64, 576, 64), 64);
	}
}