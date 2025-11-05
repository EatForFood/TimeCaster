#pragma once
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "NavBox.h"
#include "CollisionDetection.h"
#include "Player.h"
#include "Chunk.h"

class Enemy : public Character
{
private:
	float START_SPEED = 25;
	float START_HEALTH = 100;
	string m_Type;

	CollisionDetection collision;
	vector<NavBox> navBoxes;

	FloatRect m_RenderArea;

	bool m_MoveLeft = false;
	bool m_MoveRight = false;
	bool m_MoveUp = false;
	bool m_MoveDown = false;
	bool m_IsMoving = false;

	int m_Chunk; // Enemy's current chunk

	float m_Damage = 4; // Change this later to be set dynamically

	Chunk* m_ChunkPtr = nullptr;
	Vector2f m_ChunkOffset;

	const int TILE_SIZE = 64;

	Vector2i lastPlayerTile = { -999, -999 };
	vector<Vector2i> cachedPath;

	Vector2f m_PlayerPosition;

	// Pathfinding
	vector<Vector2i> m_Path; // path in grid coordinates
	size_t m_CurrentPathIndex = 1;
	bool m_AlignedToPath = false;
	const float PATH_UPDATE_INTERVAL = 0.2f; // Update path every 0.2s
	float m_UpdatePathTimer = 0.0f;
	const float NODE_REACH_THRESHOLD = 32.0f; // pixels

public:
	Enemy();

	void spawn(IntRect arena, Vector2f resolution, int tileSize, String type, int level);

	void update(float elapsedTime, const Vector2f& playerPos, Chunk* chunk, vector<NavBox> navBox);

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

	void updateTextRect();

	void followPlayer();
	void pathfindToPlayer(Chunk* chunk);
	void followPath(Chunk* chunk);

	bool reachedEndOfPath();

	int screenToTileX(float x, float y);
	int screenToTileY(float x, float y);
	Vector2f tileToScreen(int tileX, int tileY);

	void drawDebugPath(sf::RenderWindow& window);
};