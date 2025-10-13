#pragma once
#include <cmath>

struct Node {
	int x, y;
	float gCost;
	float hCost;
	Node* parent;

	Node(int x_ = 0, int y_ = 0) : x(x_), y(y_), gCost(0), hCost(0), parent(nullptr) {}

	float fCost() const {
		return gCost + hCost;
	}

	static float heuristic(const Node& a, const Node& b) {
		return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	}
};