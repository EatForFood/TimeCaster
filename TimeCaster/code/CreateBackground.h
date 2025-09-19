#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class CreateBackground
{
private:

	const int TILE_SIZE = 64;
	const int VERTS_IN_QUAD = 4;

public:

	CreateBackground();

	void placeTile(sf::VertexArray& rVA, int& currentVertex, int w, int h, int texX, int texY);

	int createLandscape(VertexArray& rVA);

};
