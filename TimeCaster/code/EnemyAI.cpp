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

void Enemy::updateAI()
{

	m_ChunkOffset.x = m_ChunkPtr->getChunkLocation().x * 50;
	m_ChunkOffset.y = m_ChunkPtr->getChunkLocation().y * 50;

	m_UpdatePathTimer += m_TimeElapsed;

	float distToPlayer = collision.distance(m_Position, m_PlayerPosition);

	// Get enemy & player tile positions
	Vector2i enemyTile(screenToTileX(m_Position.x, m_Position.y),
		screenToTileY(m_Position.x, m_Position.y));
	Vector2i playerTile(screenToTileX(m_PlayerPosition.x, m_PlayerPosition.y),
		screenToTileY(m_PlayerPosition.x, m_PlayerPosition.y));

	int distanceMoved = max(abs(playerTile.x - m_LastPlayerTile.x), abs(playerTile.y - m_LastPlayerTile.y));

	if (m_Chunk != m_PlayerChunk && distToPlayer < FOLLOW_DISTANCE)
	{
		followPlayer();
		moveTextureRect();
	}
	else if (distToPlayer < 64 && distToPlayer > 10) // switch to follow behaviour when close
	{
		followPlayer();
		moveTextureRect();
	}
	else if (distToPlayer < FOLLOW_DISTANCE)
	{
		if (m_UpdatePathTimer >= PATH_UPDATE_INTERVAL)
		{
			if (m_CachedPath.empty() || distanceMoved > 2)
			{
				pathfindToPlayer(m_ChunkPtr);   // existing function
				m_CachedPath = m_Path;     // save result
				m_LastPlayerTile = playerTile;
			}
			else
			{
				// Reuse cached path
				m_Path = m_CachedPath;
			}

			m_UpdatePathTimer = 0.0f;
		}

		followPath(m_ChunkPtr);
		moveTextureRect();
	}
	else
	{
		wander();
		moveTextureRect();
	}

	/*
	for (auto& nav : navBoxes) { // if player walks into navBox
		if (collision.pointInShape(m_Position, nav.getShape())) {
			revertPosition();
			wander();
			break;
		}
	}
	*/
	
}

void Enemy::followPlayer()
{
	// Compute direction vector from enemy to player
	Vector2f toPlayer = m_PlayerPosition - m_Position;

	// Compute the length (distance)
	float length = sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

	// Avoid division by zero
	if (length != 0)
	{
		// Normalize the vector
		Vector2f playerDirection = toPlayer / length;

		// Move enemy toward player
		m_PositionLast = m_Position;
		m_Position += playerDirection * m_Speed * m_TimeElapsed;
	}

	// If your sprite uses m_Position for rendering:
	m_Sprite.setPosition(m_Position);

	// Calculate the angle between mouse and center of screen
	float angle = atan2(m_PlayerPosition.y - m_Position.y, m_PlayerPosition.x - m_Position.x) * 180 / 3.141;

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
}

struct Node {
	int x, y;
	float gCost; // cost from start
	float hCost; // heuristic to target
	Node* parent;

	Node(int _x, int _y, float _g = 0, float _h = 0, Node* _parent = nullptr)
		: x(_x), y(_y), gCost(_g), hCost(_h), parent(_parent) {
	}

	float fCost() const { return gCost + hCost; }

	bool operator==(const Node& other) const { return x == other.x && y == other.y; }
};

// Hash function for unordered_map
struct NodeHash {
	size_t operator()(const pair<int, int>& p) const {
		return hash<int>()(p.first) ^ hash<int>()(p.second << 16);
	}
};

float heuristic(int x1, int y1, int x2, int y2) {
	return abs(x1 - x2) + abs(y1 - y2);
}

void Enemy::pathfindToPlayer(Chunk* chunk)
{
	// 1. Get local chunk tiles
	int startX = screenToTileX(m_Position.x, m_Position.y);
	int startY = screenToTileY(m_Position.x, m_Position.y);
	int endX = screenToTileX(m_PlayerPosition.x, m_PlayerPosition.y);
	int endY = screenToTileY(m_PlayerPosition.x, m_PlayerPosition.y);

	// 2. Convert to world coordinates for pathfinding
	int worldStartX = startX + static_cast<int>(m_ChunkOffset.x);
	int worldStartY = startY + static_cast<int>(m_ChunkOffset.y);
	int worldEndX = endX + static_cast<int>(m_ChunkOffset.x);
	int worldEndY = endY + static_cast<int>(m_ChunkOffset.y);

	priority_queue<pair<float, Node*>,
		vector<pair<float, Node*>>,
		greater<>> openList;

	unordered_map<pair<int, int>, Node*, NodeHash> allNodes;
	unordered_map<pair<int, int>, bool, NodeHash> closedList;

	Node* startNode = new Node(worldStartX, worldStartY, 0, heuristic(worldStartX, worldStartY, worldEndX, worldEndY));
	openList.emplace(startNode->fCost(), startNode);
	allNodes[{worldStartX, worldStartY}] = startNode;

	Node* endNode = nullptr;
	const vector<pair<int, int>> directions = { {0,1},{1,0},{0,-1},{-1,0} };

	while (!openList.empty())
	{
		Node* current = openList.top().second;
		openList.pop();
		closedList[{current->x, current->y}] = true;

		if (current->x == worldEndX && current->y == worldEndY)
		{
			endNode = current;
			break;
		}

		for (auto& dir : directions)
		{
			int nx = current->x + dir.first;
			int ny = current->y + dir.second;

			// Convert world -> local coordinates for chunk query
			int localX = nx - static_cast<int>(m_ChunkOffset.x);
			int localY = ny - static_cast<int>(m_ChunkOffset.y);

			if (localX < 0 || localX >= 50 || localY < 0 || localY >= 50)
				continue;
			if (!chunk->getNode(localX, localY))
				continue;
			if (closedList[{nx, ny}])
				continue;

			float gNew = current->gCost + 1;
			float hNew = heuristic(nx, ny, worldEndX, worldEndY);

			auto it = allNodes.find({ nx, ny });
			if (it == allNodes.end())
			{
				Node* neighbor = new Node(nx, ny, gNew, hNew, current);
				openList.emplace(neighbor->fCost(), neighbor);
				allNodes[{nx, ny}] = neighbor;
			}
			else if (gNew < it->second->gCost)
			{
				it->second->gCost = gNew;
				it->second->parent = current;
				openList.emplace(it->second->fCost(), it->second);
			}
		}
	}

	// 5. Build path in local coordinates
	m_Path.clear();
	if (!endNode)
	{
		cout << "No path found!" << endl;
	}
	else
	{
		Node* n = endNode;
		while (n != nullptr)
		{
			int localX = n->x - static_cast<int>(m_ChunkOffset.x);
			int localY = n->y - static_cast<int>(m_ChunkOffset.y);
			m_Path.push_back(Vector2i(localX, localY));
			n = n->parent;
		}
		reverse(m_Path.begin(), m_Path.end());
		m_CurrentPathIndex = 1;
	}

	for (auto& p : allNodes) delete p.second;
}

void Enemy::followPath(Chunk* chunk)
{
	if (m_Path.size() <= 1 || m_CurrentPathIndex >= m_Path.size())
		return;

	int nextX = m_Path[m_CurrentPathIndex].x;
	int nextY = m_Path[m_CurrentPathIndex].y;

	// Check if the next tile is still walkable
	if (!chunk->getNode(nextX, nextY)) 
	{
		m_Path.clear();
		pathfindToPlayer(chunk);  // immediately find a new route
		m_UpdatePathTimer = 0.0f;
		return;
	}

	// Otherwise, follow the path normally
	Vector2f target = tileToScreen(nextX, nextY);
	Vector2f toNode = target - m_Position;
	float distance = sqrt(toNode.x * toNode.x + toNode.y * toNode.y);

	if (distance > 0.01f)
	{
		Vector2f direction = toNode / distance;
		float moveDist = m_Speed * m_TimeElapsed;
		if (moveDist > distance)
			moveDist = distance;

		m_Position += direction * moveDist;
	}

	if (distance < NODE_REACH_THRESHOLD && m_CurrentPathIndex < m_Path.size() - 1)
	{
		m_CurrentPathIndex++;
	}

	// Calculate the angle between enemy and player
	float angle = atan2(target.y - m_Position.y, target.x - m_Position.x) * 180 / 3.141;

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

	m_Sprite.setPosition(m_Position);
}

bool Enemy::reachedEndOfPath()
{
	return m_CurrentPathIndex >= m_Path.size();
}

int Enemy::screenToTileX(float x, float y)
{
	x -= TILE_SIZE / 2.0f;
	y -= TILE_SIZE / 4.0f;
	int tileX = static_cast<int>(floor((x / (TILE_SIZE / 2.0f) + y / (TILE_SIZE / 4.0f)) / 2.0f));
	return tileX - m_ChunkOffset.x;
}

int Enemy::screenToTileY(float x, float y)
{
	x -= TILE_SIZE / 2.0f;
	y -= TILE_SIZE / 4.0f;
	int tileY = static_cast<int>(floor((y / (TILE_SIZE / 4.0f) - x / (TILE_SIZE / 2.0f)) / 2.0f));
	return tileY - m_ChunkOffset.y;
}

Vector2f Enemy::tileToScreen(int tileX, int tileY)
{
	int worldTileX = tileX + m_ChunkOffset.x;
	int worldTileY = tileY + m_ChunkOffset.y;
	float x = (worldTileX - worldTileY) * (TILE_SIZE / 2.0f);
	float y = (worldTileX + worldTileY) * (TILE_SIZE / 4.0f);
	x += TILE_SIZE / 2.0f;
	y += TILE_SIZE / 4.0f;
	return Vector2f(x, y);
}

void Enemy::drawDebugPath(RenderWindow& window)
{
	if (m_Path.empty())
		return;

	Color nodeColor(0, 255, 0, 180); // green, semi-transparent
	Color lineColor(0, 200, 255, 150); // cyan, semi-transparent

	// Draw the nodes
	for (auto& tile : m_Path)
	{
		Vector2f pos = tileToScreen(tile.x, tile.y);

		CircleShape circle(6);
		circle.setOrigin(3, 3);
		circle.setPosition(pos);
		circle.setFillColor(nodeColor);

		window.draw(circle);
	}

	// Draw connecting lines between nodes
	for (size_t i = 1; i < m_Path.size(); ++i)
	{
		Vector2f start = tileToScreen(m_Path[i - 1].x, m_Path[i - 1].y);
		Vector2f end = tileToScreen(m_Path[i].x, m_Path[i].y);

		Vertex line[] = {
			Vertex(start, lineColor),
			Vertex(end, lineColor)
		};

		window.draw(line, 2, sf::Lines);
	}
}

int Enemy::tileDistance(const Vector2i& a, const Vector2i& b)
{
	return max(abs(a.x - b.x), abs(a.y - b.y));
}

void Enemy::pickNewWanderDirection()
{
	// Random direction 
	int r = rand() % 4;

	switch (r)
	{
	case 0: m_WanderDirection = Vector2f(1, 0);  break;  // right
	case 1: m_WanderDirection = Vector2f(-1, 0); break;  // left
	case 2: m_WanderDirection = Vector2f(0, 1);  break;  // down
	case 3: m_WanderDirection = Vector2f(0, -1); break;  // up
	}

	direction = m_WanderDirection;

	if (m_WanderDirection == Vector2f(0, -1))
	{
		direction = Vector2f(0, 1);
	}

	if (m_WanderDirection == Vector2f(0, 1))
	{
		direction = Vector2f(0, -1);
	}

	updateTextRect();

	m_WanderDuration = 0.5f + static_cast<float>(rand()) / RAND_MAX * 1.5f;
	m_WanderTimer = 0.0f;
}

void Enemy::wander()
{
	m_WanderTimer += m_TimeElapsed;

	if (m_WanderTimer >= m_WanderDuration)
	{
		pickNewWanderDirection();
	}

	// move in the wander direction
	Vector2f newPos = m_Position + m_WanderDirection * m_Speed * m_TimeElapsed;

	// collision check
	for (auto& nav : navBoxes) { // if player walks into navBox
		if (collision.pointInShape(m_Position, nav.getShape())) {
			revertPosition();
			pickNewWanderDirection();
			break;
		}
	}

	m_PositionLast = m_Position;
	m_Position = newPos;
	m_Sprite.setPosition(m_Position);
	
}