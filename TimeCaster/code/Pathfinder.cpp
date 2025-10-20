#include "Pathfinder.h"
#include "Chunk.h"
#include <set>
#include <algorithm>

using namespace sf;
using namespace std;

float Pathfinder::heuristic(Vector2i a, Vector2i b) {
    int dx = abs(a.x - b.x);
    int dy = abs(a.y - b.y);
    float F = 1.4142f - 1.f; // sqrt(2) - 1
    return (dx < dy) ? F * dx + dy : F * dy + dx;
}

vector<Vector2i> Pathfinder::findPath(Chunk* chunk, Vector2i start, Vector2i goal, int radius) {
    auto cmp = [](Node* a, Node* b) { return a->fCost() > b->fCost(); };
    priority_queue<Node*, vector<Node*>, decltype(cmp)> open(cmp);
    unordered_map<int, Node*> allNodes;
    set<int> closed;

    auto hash = [](Vector2i p) { return (p.x << 16) ^ p.y; };

    Node* startNode = new Node(start, 0, heuristic(start, goal));
    open.push(startNode);
    allNodes[hash(start)] = startNode;

    // 8 directions (4 cardinal + 4 diagonal)
    const Vector2i dirs[8] = {
        {1,0}, {-1,0}, {0,1}, {0,-1},
        {1,1}, {-1,1}, {1,-1}, {-1,-1}
    };

    while (!open.empty()) {
        Node* current = open.top();
        open.pop();

        if (closed.count(hash(current->pos)))
            continue;
        closed.insert(hash(current->pos));

        if (current->pos == goal) {
            vector<Vector2i> path;
            for (Node* n = current; n; n = n->parent)
                path.push_back(n->pos);
            reverse(path.begin(), path.end());
            for (auto& kv : allNodes) delete kv.second;
            return path;
        }

        for (auto dir : dirs) {
            Vector2i neighbor = current->pos + dir;

            // Limit search to a local radius
            if (abs(neighbor.x - start.x) > radius || abs(neighbor.y - start.y) > radius)
                continue;

            if (chunk->isTileBlocked(neighbor.x, neighbor.y))
                continue;

            // Movement cost: diagonal = sqrt(2), straight = 1
            float moveCost = (dir.x != 0 && dir.y != 0) ? 1.4142f : 1.f;
            float gCost = current->gCost + moveCost;
            float hCost = heuristic(neighbor, goal);
            int hashVal = hash(neighbor);

            if (!allNodes.count(hashVal) || gCost < allNodes[hashVal]->gCost) {
                Node* next = new Node(neighbor, gCost, hCost, current);
                open.push(next);
                allNodes[hashVal] = next;
            }
        }
    }

    // No path found
    for (auto& kv : allNodes) delete kv.second;
    return {};
}

