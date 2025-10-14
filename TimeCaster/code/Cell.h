#pragma once
#include <SFML/Graphics.hpp>
#include "NavBox.h"

using namespace sf;

class Cell
{
protected:

	Vector2f m_Position;

	Vector2f offset;

	const int TILE_SIZE = 64;
	const int CHUNK_SIZE = 64;
	const int CELL_SIZE = 50;
	const int VERTS_IN_QUAD = 4;

	int currentVertex = 0;
	VertexArray rVA;

	bool m_LoadCell = false;

public:
	Cell();

	Cell(String type, Vector2f position, Vector2f Chunk);

	void placeTile(int x, int y, int texX, int texY, bool save);

	VertexArray getBackground();

};

