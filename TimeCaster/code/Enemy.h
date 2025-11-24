#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "NavBox.h"
#include "CollisionDetection.h"
#include "Player.h"
#include "Chunk.h"

class Enemy : public Character
{
public:
	Enemy();

	virtual void spawn(string type, Vector2i position, int level);

	virtual void update(float elapsedTime, const Vector2f& playerPos, Chunk* chunk, int playerChunk, vector<NavBox> navBox);

	float getCurrentHP();

	void healOverTime();

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();

	void stopMoving();

	bool isEnemyMoving();

	void revertPosition();

	void setChunk(int chunk);

	int getChunk();

	FloatRect getRenderArea();

	float getDamage();

	virtual void updateTextRect();

	void followPlayer();
	void pathfindToPlayer(Chunk* chunk);
	void followPath(Chunk* chunk);

	bool reachedEndOfPath();

	bool isLooted();
	int loot();

	void Attack();

	float getAttackDamage();

	int screenToTileX(float x, float y);
	int screenToTileY(float x, float y);
	Vector2f tileToScreen(int tileX, int tileY);
	int tileDistance(const Vector2i& a, const Vector2i& b);

	void drawDebugPath(sf::RenderWindow& window);

	void pickNewWanderDirection();
	void wander();

	void updateAI();

	string getType();

	enum AttackState {
		Idle,
		Charge,
		Bite,
		Shoot,
		Rage
	};

	AttackState getAttackState();

	float getShotCooldown();

	int getShotsFired();

	void resetShotsFired();

	void shotFired();

protected:
	float m_Damage = 4; // Change this later to be set dynamically
	
	string m_Type;
	
	FloatRect m_RenderArea;

	AttackState state = AttackState::Idle;

private:
	float START_SPEED = 25;
	float START_HEALTH = 100;

	CollisionDetection collision;
	vector<NavBox> navBoxes;

	bool m_MoveLeft = false;
	bool m_MoveRight = false;
	bool m_MoveUp = false;
	bool m_MoveDown = false;
	bool m_IsMoving = false;
	bool m_Looted = false;

	float m_KillValue; // How much gold / Exp the enemy gives when killed

	int m_Chunk; // Enemy's current chunk

	Chunk* m_ChunkPtr = nullptr;
	Vector2i m_ChunkOffset;

	const int TILE_SIZE = 64;

	Vector2i m_LastPlayerTile;
	vector<Vector2i> m_CachedPath;

	int m_PlayerChunk;
	Vector2f m_PlayerPosition;

	// Pathfinding
	vector<Vector2i> m_Path; // path in grid coordinates
	size_t m_CurrentPathIndex = 1;
	bool m_AlignedToPath = false;
	const float PATH_UPDATE_INTERVAL = 0.2f; // Update path every 0.2s
	float m_UpdatePathTimer = 0.0f;
	const float NODE_REACH_THRESHOLD = 32.0f; // pixels
	const int FOLLOW_DISTANCE = 492;

	// Wander behaviour
	float m_WanderTimer = 0.0f;
	float m_WanderDuration = 0.0f;
	Vector2f m_WanderDirection = { 0.f, 0.f };

	// enemy hit-box size
	int m_Hitbox_Height = 12;
	int m_Hitbox_Width = 12;

	int shotsFired = 0;
	Clock shootingCooldown;

	void resetShotCooldown();
};