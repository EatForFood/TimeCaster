#include "CollisionDetection.h"
using namespace sf;


bool CollisionDetection::pointInShape(const Vector2f& point, const ConvexShape& shape)
{
	collision = false;
	pCount = shape.getPointCount(); // number of points in shape
	int j = pCount - 1;
	for (int i = 0; i < pCount; i++) // loop over every edge of shape
	{
		pi = shape.getTransform().transformPoint(shape.getPoint(i)); // transforms the shapes points position from local to global
		pj = shape.getTransform().transformPoint(shape.getPoint(j));

		if (((pi.y > point.y) != (pj.y > point.y)) && // check if horizontal ray crosses the vertical edge
			(point.x < (pj.x - pi.x) * (point.y - pi.y) / (pj.y - pi.y) + pi.x)) // check if vertical ray crosses the horizontal edge
		{
			if (!collision) // toggle collision bool if ray crosses edge
			{
				collision = true;
			}
			else
			{
				collision = false;
			}
		}
		j = i;
	}
	return collision; // if collision true = odd number of crossings = inside shape
}

float CollisionDetection::distance(Vector2f point1, Vector2f point2) // calculate distance
{
	return sqrt((point2.x - point1.x) * (point2.x - point1.x) + (point2.y - point1.y) * (point2.y - point1.y));
}