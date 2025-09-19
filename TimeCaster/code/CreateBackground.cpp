#include <SFML/Graphics.hpp>
#include "TimeCaster.h"
#include "CreateBackground.h"

CreateBackground::CreateBackground()
{
	
}

// Function to place isometric tiles and map textures to the tiles
void CreateBackground::placeTile(sf::VertexArray& rVA, int& currentVertex, int w, int h, int texX, int texY)
{
	// Cartesian isometric
	//const int TILE_SIZE = 64;
	//const int VERTS_IN_QUAD = 4;
	float x = (w - h) * (TILE_SIZE / 2);
	float y = (w + h) * (TILE_SIZE / 4);

	// Vertex positions (currently still square, can later be diamond)
	rVA[currentVertex + 0].position = sf::Vector2f(x, y);
	rVA[currentVertex + 1].position = sf::Vector2f(x + TILE_SIZE, y);
	rVA[currentVertex + 2].position = sf::Vector2f(x + TILE_SIZE, y + TILE_SIZE);
	rVA[currentVertex + 3].position = sf::Vector2f(x, y + TILE_SIZE);

	// Texture mapping
	rVA[currentVertex + 0].texCoords = sf::Vector2f(texX, texY);
	rVA[currentVertex + 1].texCoords = sf::Vector2f(texX + TILE_SIZE, texY);
	rVA[currentVertex + 2].texCoords = sf::Vector2f(texX + TILE_SIZE, texY + TILE_SIZE);
	rVA[currentVertex + 3].texCoords = sf::Vector2f(texX, texY + TILE_SIZE);

	currentVertex += VERTS_IN_QUAD;
}

int CreateBackground::createLandscape(VertexArray& rVA)
{

	// What type of primitive are we using?
	rVA.setPrimitiveType(Quads);

	// Set the size of the vertex array
	rVA.resize(800 * 800 * VERTS_IN_QUAD);

	// Start at the beginning of the vertex array
	int currentVertex = 0;

	// Grass and cliffs
	for (int w = 0; w < 400; w++)
	{
		for (int h = 0; h < 400; h++)
		{
			if (h <= 1 || w <= 1) // place cliffs at edges of map
			{
				placeTile(rVA, currentVertex, w, h, 256, 192);
			}
			else // place grass
			{
				placeTile(rVA, currentVertex, w, h, 0, 0);
			}

			currentVertex += VERTS_IN_QUAD;
		}
	}

	// House west wall
	for (int w = 15; w < 20; w++)
	{
		placeTile(rVA, currentVertex, w, 18, 64, 1140);
	}

	// House east wall
	for (int h = 15; h < 19; h++)
	{
		placeTile(rVA, currentVertex, 19, h, 64, 1140);
	}

	// House east roof
	placeTile(rVA, currentVertex, 17, 14, 448, 1076);
	placeTile(rVA, currentVertex, 18, 14, 448, 1076);

	// House 2nd floor east wall
	for (int h = 15; h < 17; h++)
	{
		placeTile(rVA, currentVertex, 18, h, 512, 1460);
	}

	// House west roof 1
	for (int w = 14; w < 19; w++)
	{
		placeTile(rVA, currentVertex, w, 17, 64, 1076);
	}

	// House west roof 2
	for (int w = 13; w < 18; w++)
	{
		placeTile(rVA, currentVertex, w, 15, 64, 1076);
	}

	return TILE_SIZE;
}
