#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class CollisionDetection
{
private:

	bool collision = false;
	int pCount;
	
	Vector2f pi;
	Vector2f pj;

public:
	bool pointInShape(const sf::Vector2f& point, const sf::ConvexShape& poly);
	float distance(Vector2f point1, Vector2f point2);
	
};

