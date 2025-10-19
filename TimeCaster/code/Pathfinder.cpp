#include "Pathfinder.h"
#include "Chunk.h"
#include <set>
#include <algorithm>

using namespace sf;
using namespace std;

float Pathfinder::heuristic(Vector2i a, Vector2i b)
{
    return static_cast<float>(abs(a.x - b.x) + abs(a.y - b.y));
}

vector<Vector2i> Pathfinder::findPath(Chunk* chunk, Vector2i start, Vector2i goal, int radius)
{
    auto cmp = [](Node* a, Node* b) { return a->fCost() > b->fCost(); };
    priority_queue<Node*, vector<Node*>, decltype(cmp)> open(cmp);
    unordered_map<int, Node*> allNodes;
    set<int> closed;

    auto hash = [](Vector2i p) { return p.x * 1000 + p.y; };

    Node* startNode = new Node(start, 0, heuristic(start, goal));
    open.push(startNode);
    allNodes[hash(start)] = startNode;

    const Vector2i dirs[4] = { {1,0}, {-1,0}, {0,1}, {0,-1} };

    while (!open.empty())
    {
        Node* current = open.top();
        open.pop();

        if (current->pos == goal)
        {
            vector<Vector2i> path;
            for (Node* n = current; n; n = n->parent)
                path.push_back(n->pos);
            reverse(path.begin(), path.end());
            for (auto& kv : allNodes) delete kv.second;
            return path;
        }

        closed.insert(hash(current->pos));

        for (auto dir : dirs)
        {
            Vector2i neighbor = current->pos + dir;

            // Limit search to a local 20x20 radius
            if (abs(neighbor.x - start.x) > radius || abs(neighbor.y - start.y) > radius)
                continue;

            if (chunk->isTileBlocked(neighbor.x, neighbor.y))
                continue;

            if (closed.count(hash(neighbor)))
                continue;

            float gCost = current->gCost + 1;
            float hCost = heuristic(neighbor, goal);

            if (!allNodes.count(hash(neighbor)) || gCost < allNodes[hash(neighbor)]->gCost)
            {
                Node* next = new Node(neighbor, gCost, hCost, current);
                open.push(next);
                allNodes[hash(neighbor)] = next;
            }
        }
    }

    // No path found
    for (auto& kv : allNodes) delete kv.second;
    return {};
}
