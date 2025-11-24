#pragma once
#include <SFML/Graphics.hpp>
#include "NavBox.h"

using namespace sf;

class NavBox
{
protected:

	int TILE_SIZE = 64;

	int ix;
	int iy;

	ConvexShape navArea;

	Vector2f m_Position;

public:
	NavBox();

	NavBox(int x, int y, int w, int h);

	void SetNavBox(int x, int y, int w, int h);

	Vector2f getPosition();

	const ConvexShape& getShape() const;

	void NavTree();

	void NavTreeLarge();

	void NavEmpty();
};

