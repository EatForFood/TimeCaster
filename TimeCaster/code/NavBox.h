#pragma once
#include <SFML/Graphics.hpp>
#include "NavBox.h"

using namespace sf;

class NavBox
{
protected:

	int TILE_SIZE = 64;

	float ix;
	float iy;

	ConvexShape navArea;

	Vector2f m_Position;

public:
	NavBox();

	NavBox(float x, float y, float w, float h);

	void SetNavBox(float x, float y, float w, float h);

	Vector2f getPosition();

	ConvexShape getShape();

	void NavTree();

	void NavEmpty();
};

