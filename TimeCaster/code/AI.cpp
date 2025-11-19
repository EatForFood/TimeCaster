#include "AI.h"
using namespace sf;
using namespace std;


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

void AI::pathfindToPlayer(Chunk* chunk)
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

void AI::followPath(Chunk* chunk)
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

bool AI::reachedEndOfPath()
{
	return m_CurrentPathIndex >= m_Path.size();
}

int AI::screenToTileX(float x, float y)
{
	x -= TILE_SIZE / 2.0f;
	y -= TILE_SIZE / 4.0f;
	int tileX = static_cast<int>(std::floor((x / (TILE_SIZE / 2.0f) + y / (TILE_SIZE / 4.0f)) / 2.0f));
	return tileX - m_ChunkOffset.x;
}

int AI::screenToTileY(float x, float y)
{
	x -= TILE_SIZE / 2.0f;
	y -= TILE_SIZE / 4.0f;
	int tileY = static_cast<int>(std::floor((y / (TILE_SIZE / 4.0f) - x / (TILE_SIZE / 2.0f)) / 2.0f));
	return tileY - m_ChunkOffset.y;
}

Vector2f AI::tileToScreen(int tileX, int tileY)
{
	int worldTileX = tileX + m_ChunkOffset.x;
	int worldTileY = tileY + m_ChunkOffset.y;
	float x = (worldTileX - worldTileY) * (TILE_SIZE / 2.0f);
	float y = (worldTileX + worldTileY) * (TILE_SIZE / 4.0f);
	x += TILE_SIZE / 2.0f;
	y += TILE_SIZE / 4.0f;
	return sf::Vector2f(x, y);
}

void AI::drawDebugPath(sf::RenderWindow& window)
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

int AI::tileDistance(const sf::Vector2i& a, const sf::Vector2i& b)
{
	return std::max(std::abs(a.x - b.x), std::abs(a.y - b.y));
}