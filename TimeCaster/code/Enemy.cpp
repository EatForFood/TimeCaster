#include <iostream>
#include "Enemy.h"
#include "TextureHolder.h"
#include "CollisionDetection.h"
#include "Chunk.h"
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_map>
#include <algorithm>

using namespace std;
using namespace sf;

Enemy::Enemy() {
	m_Speed = START_SPEED;
}

void Enemy::spawn(string type, Vector2i position, int level) {

	m_Hitbox.left = m_Position.x - 20;
	m_Hitbox.width = 40;
	m_Hitbox.top = m_Position.y - 20;
	m_Hitbox.height = 40;
	m_Level = level;
	m_Type = type;
	m_Health = START_HEALTH * (1 + (m_Level - 1) * 0.1f); // Increase health by 10% per level
	m_MaxHealth = START_HEALTH * (1 + (m_Level - 1) * 0.1f); // Increase max health by 10% per level

	// Associate a texture with the sprite
	if (type == "Knight") {
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/player/playerWalk.png"));
	}
	else if (type == "Goblin") {
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/enemies/goblin.png"));
	}
	else if (type == "Orc") {
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/enemies/orc.png"));
		m_Speed = 60;
		m_Health = 50;
		m_AttackDmg = 20;
		m_KillValue = 10;
		m_SpriteWeapon = Sprite(TextureHolder::GetTexture("graphics/player/weapon/slash/Iron_Sword.png"));
		m_WeaponSize = 3;
	}
	else if (type == "Skeleton") {
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/enemies/skeleton.png"));
		m_Speed = 50;
		m_Health = 80;
		m_AttackDmg = 30;
		m_KillValue = 20;
		m_SpriteWeapon = Sprite(TextureHolder::GetTexture("graphics/player/weapon/slash/Pirate's_Scimitar.png"));
		m_WeaponSize = 3;
	}

	// Set the origin of the sprite to the centre, 
	m_Sprite.setOrigin(32, 32);
	m_Sprite.setScale(0.75, 0.75);

	m_RenderArea = FloatRect(0, 0, 1920, 1080);

	m_Position.x = position.x;
	m_Position.y = position.y;
	m_Sprite.setPosition(m_Position);
	m_SpriteWeapon.setOrigin((64 * m_WeaponSize) / 2, (64 * m_WeaponSize) / 2);		m_SpriteWeapon.setScale(0.75, 0.75);
}

void Enemy::update(float elapsedTime, const Vector2f& playerPos, Chunk* chunk, int playerChunk, vector<NavBox> navBox) {

	if (m_IsAttacking)
	{
		moveTextureRect();

		if (getAniCounter() == 0)
		{
			m_IsAttacking = false;
			resetAniCounter();

			if (m_Type == "Skeleton")
			{
				m_Sprite.setTexture(TextureHolder::GetTexture("graphics/enemies/skeleton.png"));
				m_Sprite.setOrigin(32, 32);
				m_Sprite.setScale(0.75, 0.75);
			}
			else if (m_Type == "Orc")
			{
				m_Sprite.setTexture(TextureHolder::GetTexture("graphics/enemies/orc.png"));
				m_Sprite.setOrigin(32, 32);
				m_Sprite.setScale(0.75, 0.75);
			}
		}
	}

	updateTextRect();

	navBoxes = navBox;
	m_ChunkPtr = chunk;
	m_TimeElapsed = elapsedTime;
	m_PlayerPosition = playerPos;
	m_PlayerChunk = playerChunk;
	m_PositionLast = m_Position;

	updateAI();

	// update collision detection zone
	m_CollisionBox.left = m_Position.x - 100;
	m_CollisionBox.top = m_Position.y - 100;
	m_CollisionBox.width = 200;
	m_CollisionBox.height = 200;

	// update enemy hitbox
	m_Hitbox.left = m_Position.x - m_Hitbox_Width / 2;
	m_Hitbox.width = m_Hitbox_Width;
	m_Hitbox.top = m_Position.y - m_Hitbox_Height / 2;
	m_Hitbox.height = m_Hitbox_Height;

	if (!m_IsDead && m_Health <= 0) {
		m_IsDead = true;
		m_Sprite.setRotation(90);
		m_Sprite.setOrigin(32, 56);
	}
}

void Enemy::Attack()
{

	if (m_IsAttacking)
	{
		return;
	}

	if (!m_IsAttacking)
	{
		resetAniCounter();
	}
	m_IsAttacking = true;

	if (m_Type == "Skeleton")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/enemies/skeletonAttack.png"));
		m_Sprite.setOrigin(32, 32);
		m_Sprite.setScale(0.75f, 0.75f);
	}
	else if (m_Type == "Orc")
	{
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/enemies/orcAttack.png"));
		m_Sprite.setOrigin(32, 32);
		m_Sprite.setScale(0.75f, 0.75f);
	}

	// Associate a texture with the body sprite
	// TODO: Change weapon based on enemy type

	m_SpriteWeapon.setOrigin((64 * m_WeaponSize) / 2, (64 * m_WeaponSize) / 2);
	m_SpriteWeapon.setScale(0.75, 0.75);

	setSpriteFromSheet(IntRect(0, 0, 384, 64), 64);



	updateTextRect();
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

void Enemy::stopMoving() 
{
	m_MoveLeft = false;
	m_MoveRight = false;
	m_MoveUp = false;
	m_MoveDown = false;
}

void Enemy::revertPosition() 
{
	setPosition(m_PositionLast);
}

bool Enemy::isEnemyMoving() {
	if (m_IsMoving) {
		return true;
	}

	return false;
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
	if (!m_IsAttacking)
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
	else 
	{
		if (direction == Vector2f(0, 1)) // up
		{
			setSpriteFromSheet(IntRect(0, 0, 384, 64), 64);
		}

		if (direction == Vector2f(0, -1)) // down
		{
			setSpriteFromSheet(IntRect(0, 128, 384, 64), 64);
		}

		if (direction == Vector2f(1, 0)) // right
		{
			setSpriteFromSheet(IntRect(0, 192, 384, 64), 64);
		}

		if (direction == Vector2f(-1, 0)) // left
		{
			setSpriteFromSheet(IntRect(0, 64, 384, 64), 64);
		}
	}
	m_SpriteWeapon.setPosition(m_Position);
}

int Enemy::loot()
{
	m_Looted = true;
	return m_KillValue;
}

bool Enemy::isLooted()
{
	return m_Looted;
}

float Enemy::getAttackDamage()
{
	return m_AttackDmg;
}

string Enemy::getType() {
	return m_Type;
}

Enemy::AttackState Enemy::getAttackState()
{
	return state;
}

float Enemy::getShotCooldown() {
	return shootingCooldown.getElapsedTime().asSeconds();
}

int Enemy::getShotsFired() {
	return shotsFired;
}

void Enemy::resetShotsFired() {
	shotsFired = 0;
}

void Enemy::shotFired() {
	cout << "Shot no. " << shotsFired << endl;
	shotsFired++;
	resetShotCooldown();

	if (shotsFired > 4) {
		m_IsAttacking = false;
	}
}

void Enemy::resetShotCooldown() {
	shootingCooldown.restart();
}