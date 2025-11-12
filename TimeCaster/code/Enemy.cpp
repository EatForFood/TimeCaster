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
	m_MaxHealth = START_HEALTH * (1 + (m_Level - 1) * 0.1f); // Increase health by 10% per level
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
	}
	else if (type == "Skeleton") {
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/enemies/skeleton.png"));
		m_Speed = 50;
		m_Health = 80;
		m_AttackDmg = 30;
		m_KillValue = 20;
	}
	else if (type == "Dragon") {
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/enemies/dragon.png"));
	}

	// Set the origin of the sprite to the centre, 
	m_Sprite.setOrigin(32, 32);
	m_Sprite.setScale(0.75, 0.75);

	m_RenderArea = FloatRect(0, 0, 1920, 1080);

	m_Position.x = position.x;
	m_Position.y = position.y;
	m_Sprite.setPosition(m_Position);
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
	m_PositionLast = m_Position;

	m_ChunkOffset.x = chunk->getChunkLocation().x * 50;
	m_ChunkOffset.y = chunk->getChunkLocation().y * 50;

	m_UpdatePathTimer += elapsedTime;

	float distToPlayer = collision.distance(m_Position, m_PlayerPosition);

	// Get enemy & player tile positions
	sf::Vector2i enemyTile(screenToTileX(m_Position.x, m_Position.y),
		screenToTileY(m_Position.x, m_Position.y));
	sf::Vector2i playerTile(screenToTileX(m_PlayerPosition.x, m_PlayerPosition.y),
		screenToTileY(m_PlayerPosition.x, m_PlayerPosition.y));

	int distanceMoved = std::max(std::abs(playerTile.x - m_LastPlayerTile.x), std::abs(playerTile.y - m_LastPlayerTile.y));

	if (m_Chunk != playerChunk && distToPlayer < FOLLOW_DISTANCE)
	{
		followPlayer();
		moveTextureRect();
	}
	else if (distToPlayer < 128 && distToPlayer > 10) // switch to follow behaviour when close
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
				std::cout << "[DEBUG] Recalculating path (player moved " << distanceMoved << " tiles)\n";

				pathfindToPlayer(chunk);   // existing function
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

		followPath(chunk);
		moveTextureRect();
	}
	else
	{
		// wander behaviour
	}

	/*
	for (auto& nav : navBoxes) { // if player walks into navBox
		if (collision.pointInShape(m_Position, nav.getShape())) {
			revertPosition();
			m_Path.clear();

			pathfindToPlayer(m_ChunkPtr);  
			m_UpdatePathTimer = 0.0f;      
			break;                         
		}
	}
	*/

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

	resetAniCounter();
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

	
	setSpriteFromSheet(IntRect(0, 0, 385, 64), 64);

	m_SpriteWeapon = Sprite(TextureHolder::GetTexture("graphics/player/weapon/Pirate's_Scimtar.png"));
	m_SpriteWeapon.setOrigin(32, 32);
	m_SpriteWeapon.setScale(0.75, 0.75);

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
			setSpriteFromSheet(IntRect(0, 0, 385, 64), 64);
		}

		if (direction == Vector2f(0, -1)) // down
		{
			setSpriteFromSheet(IntRect(0, 128, 385, 64), 64);
		}

		if (direction == Vector2f(1, 0)) // right
		{
			setSpriteFromSheet(IntRect(0, 192, 385, 64), 64);
		}

		if (direction == Vector2f(-1, 0)) // left
		{
			setSpriteFromSheet(IntRect(0, 64, 385, 64), 64);
		}
	}
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
	size_t operator()(const std::pair<int, int>& p) const {
		return std::hash<int>()(p.first) ^ std::hash<int>()(p.second << 16);
	}
};

float heuristic(int x1, int y1, int x2, int y2) {
	return std::abs(x1 - x2) + std::abs(y1 - y2);
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

	if (!chunk->getNode(startX, startY))
		std::cout << "[DEBUG] Start node not walkable!" << std::endl;

	if (!chunk->getNode(endX, endY))
		std::cout << "[DEBUG] End node not walkable!" << std::endl;

	std::cout << "[DEBUG] Pathfinding from (" << worldStartX << "," << worldStartY
		<< ") to (" << worldEndX << "," << worldEndY << ")" << std::endl;

	std::priority_queue<std::pair<float, Node*>,
		std::vector<std::pair<float, Node*>>,
		std::greater<>> openList;

	std::unordered_map<std::pair<int, int>, Node*, NodeHash> allNodes;
	std::unordered_map<std::pair<int, int>, bool, NodeHash> closedList;

	Node* startNode = new Node(worldStartX, worldStartY, 0, heuristic(worldStartX, worldStartY, worldEndX, worldEndY));
	openList.emplace(startNode->fCost(), startNode);
	allNodes[{worldStartX, worldStartY}] = startNode;

	Node* endNode = nullptr;
	const std::vector<std::pair<int, int>> directions = { {0,1},{1,0},{0,-1},{-1,0} };

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
		std::cout << "[DEBUG] No path found!" << std::endl;
	}
	else
	{
		Node* n = endNode;
		while (n != nullptr)
		{
			int localX = n->x - static_cast<int>(m_ChunkOffset.x);
			int localY = n->y - static_cast<int>(m_ChunkOffset.y);
			m_Path.push_back(sf::Vector2i(localX, localY));
			n = n->parent;
		}
		std::reverse(m_Path.begin(), m_Path.end());
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

	std::cout << "[DEBUG] Checking next tile: (" << nextX << "," << nextY << ")" << std::endl;

	// Check if the next tile is still walkable
	if (!chunk->getNode(nextX, nextY)) {
		std::cout << "[DEBUG] Path blocked at (" << nextX << "," << nextY << ") — recalculating path..." << std::endl;

		m_Path.clear();
		pathfindToPlayer(chunk);  // immediately find a new route
		m_UpdatePathTimer = 0.0f;
		return;
	}

	// Otherwise, follow the path normally
	sf::Vector2f target = tileToScreen(nextX, nextY);
	sf::Vector2f toNode = target - m_Position;
	float distance = std::sqrt(toNode.x * toNode.x + toNode.y * toNode.y);

	if (distance > 0.01f)
	{
		sf::Vector2f direction = toNode / distance;
		float moveDist = m_Speed * m_TimeElapsed;
		if (moveDist > distance)
			moveDist = distance;

		m_Position += direction * moveDist;
	}

	if (distance < NODE_REACH_THRESHOLD && m_CurrentPathIndex < m_Path.size() - 1)
	{
		m_CurrentPathIndex++;
	}

	// Calculate the angle between mouse and center of screen
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
	int tileX = static_cast<int>(std::floor((x / (TILE_SIZE / 2.0f) + y / (TILE_SIZE / 4.0f)) / 2.0f));
	return tileX - m_ChunkOffset.x;
}

int Enemy::screenToTileY(float x, float y)
{
	x -= TILE_SIZE / 2.0f;
	y -= TILE_SIZE / 4.0f;
	int tileY = static_cast<int>(std::floor((y / (TILE_SIZE / 4.0f) - x / (TILE_SIZE / 2.0f)) / 2.0f));
	return tileY - m_ChunkOffset.y;
}

sf::Vector2f Enemy::tileToScreen(int tileX, int tileY)
{
	int worldTileX = tileX + m_ChunkOffset.x;
	int worldTileY = tileY + m_ChunkOffset.y;
	float x = (worldTileX - worldTileY) * (TILE_SIZE / 2.0f);
	float y = (worldTileX + worldTileY) * (TILE_SIZE / 4.0f);
	x += TILE_SIZE / 2.0f;
	y += TILE_SIZE / 4.0f;
	return sf::Vector2f(x, y);
}

void Enemy::drawDebugPath(sf::RenderWindow& window)
{
	if (m_Path.empty())
		return;

	sf::Color nodeColor(0, 255, 0, 180); // green, semi-transparent
	sf::Color lineColor(0, 200, 255, 150); // cyan, semi-transparent

	// Draw the nodes
	for (auto& tile : m_Path)
	{
		sf::Vector2f pos = tileToScreen(tile.x, tile.y);

		sf::CircleShape circle(6);
		circle.setOrigin(3, 3);
		circle.setPosition(pos);
		circle.setFillColor(nodeColor);

		window.draw(circle);
	}

	// Draw connecting lines between nodes
	for (size_t i = 1; i < m_Path.size(); ++i)
	{
		sf::Vector2f start = tileToScreen(m_Path[i - 1].x, m_Path[i - 1].y);
		sf::Vector2f end = tileToScreen(m_Path[i].x, m_Path[i].y);

		sf::Vertex line[] = {
			sf::Vertex(start, lineColor),
			sf::Vertex(end, lineColor)
		};

		window.draw(line, 2, sf::Lines);
	}
}

int Enemy::tileDistance(const sf::Vector2i& a, const sf::Vector2i& b)
{
	return std::max(std::abs(a.x - b.x), std::abs(a.y - b.y));
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