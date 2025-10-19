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
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/Enemies/swordGoblin.png"));
	}

	// Set the origin of the sprite to the centre, 
	m_Sprite.setOrigin(32, 32);
	m_Sprite.setScale(0.75, 0.75);

	m_RenderArea = FloatRect(0, 0, 1920, 1080);
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

	Vector2i enemyTile(int(m_Position.x / TILE_SIZE), int(m_Position.y / TILE_SIZE));
	Vector2i playerTile(int(playerPos.x / TILE_SIZE), int(playerPos.y / TILE_SIZE));

	int distanceMoved = max(abs(playerTile.x - lastPlayerTile.x), abs(playerTile.y - lastPlayerTile.y));

	// Recalculate path only if player moved > 3 tiles or path empty
	if (cachedPath.empty() || distanceMoved > 3) {
		cachedPath = Pathfinder::findPath(chunk, enemyTile, playerTile, 20);
		lastPlayerTile = playerTile;
	}

	// --- Follow path ---
	if (cachedPath.size() > 1) {
		Vector2f nextPoint(
			cachedPath[1].x * TILE_SIZE + TILE_SIZE / 2,
			cachedPath[1].y * TILE_SIZE + TILE_SIZE / 2
		);

		Vector2f dir = nextPoint - m_Position;
		float length = sqrt(dir.x * dir.x + dir.y * dir.y);

		if (length > 0.01f) {
			dir /= length;
			m_PositionLast = m_Position;
			m_Position += dir * m_Speed * elapsedTime;
			direction = dir;
		}

		// If close enough to next tile, pop it from path
		if (length < 8.f && cachedPath.size() > 1) {
			cachedPath.erase(cachedPath.begin());
		}

		if (cachedPath.size() > 1) {
			m_IsMoving = true;
		}
		else {
			m_IsMoving = false;
		}
	}

	if (!isEnemyMoving()) // if enemy is not moving set sprite to standing frame in last direction faced
	{
		if (direction == Vector2f(0, -1)) // up
		{
			setSpriteFromSheet(IntRect(0, 0, 576, 64), 65);
		}

		if (direction == Vector2f(0, 1)) // down
		{
			setSpriteFromSheet(IntRect(0, 128, 576, 64), 65);
		}

		if (direction == Vector2f(-1, 0)) // right
		{
			setSpriteFromSheet(IntRect(0, 192, 576, 64), 65);
		}

		if (direction == Vector2f(1, 0)) // left
		{
			setSpriteFromSheet(IntRect(0, 64, 576, 64), 65);
		}
	}

	m_Sprite.setPosition(m_Position);
	m_CollisionBox.left = m_Position.x - 100;
	m_CollisionBox.top = m_Position.y - 100;
	m_CollisionBox.width = 200;
	m_CollisionBox.height = 200;

	if (isEnemyMoving()) // animate sprite if player is moving
	{
		moveTextureRect();
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