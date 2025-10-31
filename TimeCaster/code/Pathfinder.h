#pragma once
#include <SFML/System.hpp>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>

using namespace std;
using namespace sf;

class Chunk;

struct Node
{
    sf::Vector2i pos;
    float gCost, hCost;
    Node* parent;
    float fCost() const { return gCost + hCost; }

    Node(sf::Vector2i p, float g, float h, Node* par = nullptr)
        : pos(p), gCost(g), hCost(h), parent(par) {
    }
};

class Pathfinder
{
public:
   

private:
    
};