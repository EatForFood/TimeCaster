#include "Cell.h"
using namespace sf;
using namespace std;

Cell::Cell()
{

}

Cell::Cell(String type, Vector2f Position, Vector2f chunk)
{
	offset.x = chunk.x * CHUNK_SIZE;
	offset.y = chunk.y * CHUNK_SIZE;

	rVA.setPrimitiveType(Quads);

	// Set the size of the vertex array
	rVA.resize(50 * 50 * VERTS_IN_QUAD);

    if (type == "house2")
    {
		placeTile(Position.x, Position.y-2, 1, 15, true);
		placeTile(Position.x +1, Position.y -2, 0, 16, true);
		placeTile(Position.x, Position.y -1, 1, 16, true);
		placeTile(Position.x, Position.y, 1, 16, true);
		placeTile(Position.x +1, Position.y -1, 0, 17, true);
		placeTile(Position.x +1, Position.y, 0, 17, true);
    }
   
}

void Cell::placeTile(int x, int y, int texX, int texY, bool save)
{

	texX *= TILE_SIZE;
	texY *= TILE_SIZE;

	x += offset.x;
	y += offset.y;

	// Cartesian to isometric
	float ix = (x - y) * (TILE_SIZE / 2);
	float iy = (x + y) * (TILE_SIZE / 4);


	rVA[currentVertex + 0].position = sf::Vector2f(ix, iy);
	rVA[currentVertex + 1].position = sf::Vector2f(ix + TILE_SIZE, iy);
	rVA[currentVertex + 2].position = sf::Vector2f(ix + TILE_SIZE, iy + TILE_SIZE);
	rVA[currentVertex + 3].position = sf::Vector2f(ix, iy + TILE_SIZE);

	// Texture mapping
	rVA[currentVertex + 0].texCoords = sf::Vector2f(texX, texY);
	rVA[currentVertex + 1].texCoords = sf::Vector2f(texX + TILE_SIZE, texY);
	rVA[currentVertex + 2].texCoords = sf::Vector2f(texX + TILE_SIZE, texY + TILE_SIZE);
	rVA[currentVertex + 3].texCoords = sf::Vector2f(texX, texY + TILE_SIZE);

	currentVertex += VERTS_IN_QUAD;

}

VertexArray Cell::getBackground() {

	return rVA;
}