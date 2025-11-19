#pragma once
#include "Enemy.h"
#include "NavBox.h"
#include "Chunk.h"
#include <vector>

class AI
{
private:
	Enemy& enemy;  // Reference to the owner enemy

	// AI-specific state moved from Enemy
	Vector2i lastPlayerTile;
	vector<Vector2i> m_CachedPath;
	vector<Vector2i> path;
	size_t currentPathIndex = 1;
	float pathUpdateTimer = 0.0f;

	// Pathfinding
	vector<Vector2i> m_Path; // path in grid coordinates
	size_t m_CurrentPathIndex = 1;
	bool m_AlignedToPath = false;
	const float PATH_UPDATE_INTERVAL = 0.2f; // Update path every 0.2s
	float m_UpdatePathTimer = 0.0f;
	const float NODE_REACH_THRESHOLD = 32.0f; // pixels
	const int FOLLOW_DISTANCE = 492;

	// AI logic:
	void followPlayer();
	void pathfindToPlayer(Chunk* chunk);
	void followPath(Chunk* chunk);
	bool reachedEndOfPath();
	int screenToTileX(float x, float y);
	int screenToTileY(float x, float y);
	Vector2f tileToScreen(int tileX, int tileY);
	int tileDistance(const Vector2i& a, const Vector2i& b);

public:
	AI(Enemy& owner);

	void update(float dt, const Vector2f& playerPos, Chunk* chunk, int playerChunk, const vector<NavBox>& navBoxes);

};